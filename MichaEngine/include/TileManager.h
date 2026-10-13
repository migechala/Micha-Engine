#pragma once

#include <SDL.h>

#include <algorithm>
#include <exception>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Logger.h"
#include "Types.h"

class TileManager {
  using TileGrid = std::vector<std::vector<int>>;

  std::shared_ptr<SDL_Texture> tilesImage;
  std::shared_ptr<TileGrid> tiles;
  eng::Vector2<int> tileSourceSize{0, 0};
  std::unordered_map<std::string, std::shared_ptr<TileGrid>> cache;

  static std::vector<int> parseRow(const std::string &line) {
    std::vector<int> row;

    if (line.find(',') != std::string::npos) {
      std::stringstream stream(line);
      for (std::string token; std::getline(stream, token, ',');) {
        if (token.empty() || token == ".") {
          row.push_back(-1);
          continue;
        }
        try {
          row.push_back(std::stoi(token));
        } catch (const std::exception &) {
          row.push_back(-1);
        }
      }
      return row;
    }

    for (char c : line) {
      if (c >= '0' && c <= '9') {
        row.push_back(c - '0');
      }
    }
    return row;
  }

  std::shared_ptr<TileGrid> readFile(const std::string &tileFile) {
    auto grid = std::make_shared<TileGrid>();
    std::ifstream file(tileFile);
    if (!file.is_open()) {
      LOG_ERR("Could not open tile file: " + tileFile);
      return grid;
    }

    for (std::string line; std::getline(file, line);) {
      auto row = parseRow(line);
      if (!row.empty()) {
        grid->push_back(std::move(row));
      }
    }
    return grid;
  }

public:
  void setTiles(const std::string &tileFile, eng::Vector2<int> sourceTileSize, std::shared_ptr<SDL_Texture> img) {
    tilesImage = std::move(img);
    tileSourceSize = sourceTileSize;

    auto it = cache.find(tileFile);
    if (it == cache.end()) {
      it = cache.emplace(tileFile, readFile(tileFile)).first;
    }
    tiles = it->second;
  }

  void renderTile(SDL_Renderer *renderer, int tileId, eng::Vector2<int> position, eng::Vector2<int> size) {
    if (!renderer || !tilesImage || tileId < 0 || tileSourceSize.x <= 0 || tileSourceSize.y <= 0) {
      return;
    }

    int textureWidth = 0;
    int textureHeight = 0;
    if (SDL_QueryTexture(tilesImage.get(), nullptr, nullptr, &textureWidth, &textureHeight) != 0 || textureWidth <= 0) {
      return;
    }

    const int columns = std::max(1, textureWidth / tileSourceSize.x);
    SDL_Rect srcRect{(tileId % columns) * tileSourceSize.x, (tileId / columns) * tileSourceSize.y, tileSourceSize.x,
                     tileSourceSize.y};
    SDL_Rect destRect{position.x, position.y, size.x, size.y};

    SDL_RenderCopy(renderer, tilesImage.get(), &srcRect, &destRect);
  }

  void renderMap(SDL_Renderer *renderer, eng::Vector2<int> position, eng::Vector2<int> tileSize) {
    if (!tiles) {
      return;
    }

    for (size_t y = 0; y < tiles->size(); ++y) {
      for (size_t x = 0; x < (*tiles)[y].size(); ++x) {
        int tileId = (*tiles)[y][x];
        if (tileId >= 0) {
          eng::Vector2<int> tilePosition = {position.x + static_cast<int>(x * tileSize.x),
                                            position.y + static_cast<int>(y * tileSize.y)};
          renderTile(renderer, tileId, tilePosition, tileSize);
        }
      }
    }
  }
};
