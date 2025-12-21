#pragma once
#include <ResourceLoader.h>
#include <SDL2/SDL.h>
#include <Types.h>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
class TileManager {
  std::shared_ptr<SDL_Texture> tilesImage;
  std::shared_ptr<std::vector<std::vector<int>>> tiles;
  std::unordered_map<std::string, std::shared_ptr<std::vector<std::vector<int>>>> cache;
  std::shared_ptr<std::vector<std::vector<int>>> readFile(std::string tileFile) {
    std::ifstream file(tileFile);
    std::vector<int> cur;
    for (std::string l; std::getline(file, l);) {
      // std::for_each(l.begin(), l.end(),std::back_insert_iterator<std::vector<int>>(cur));
      for (char c : l) {
        if (c >= '0' && c <= '9') {
          cur.push_back(c - '0');
        }
      }
      tiles->emplace_back(cur);
    }
    return tiles;
  }

public:
  inline void setTiles(std::string tileFile, eng::Vector2<int> size, std::shared_ptr<SDL_Texture> img) {
    std::unordered_map<std::string, std::shared_ptr<std::vector<std::vector<int>>>>::iterator it = cache.find(tileFile);
    if (it == cache.end()) {
      readFile(tileFile);
      cache[tileFile] = tiles;
    }
    tiles.swap(it->second);
  }
  void renderTile(SDL_Renderer *renderer, int tileId, eng::Vector2<int> position, eng::Vector2<int> size) {
    if (!tiles || tileId < 0 || tileId >= tiles->size()) {
      return;
    }

    SDL_Rect srcRect;
    srcRect.x = (tileId % (tiles->at(0).size())) * size.x;
    srcRect.y = (tileId / (tiles->at(0).size())) * size.y;
    srcRect.w = size.x;
    srcRect.h = size.y;

    SDL_Rect destRect;
    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = size.x;
    destRect.h = size.y;

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