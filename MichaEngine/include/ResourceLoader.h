#pragma once

#include <SDL2/SDL_system.h>

#include <memory>
#include <string>
#include <vector>

#include "SDLDeleter.h"

class ResourceLoader {
  static ResourceLoader *instance;

 public:
  static ResourceLoader *getInstance();

  std::shared_ptr<SDL_Texture> loadTexture(SDL_Renderer *renderer,
                                           const std::string &location);

  template <class... Args>
  std::vector<std::shared_ptr<SDL_Texture> > loadTextures(
      SDL_Renderer *renderer, Args... fileNames) {
    std::vector<std::shared_ptr<SDL_Texture> > textures;
    (textures.push_back(loadTexture(renderer, fileNames)), ...);
    return textures;
  }
};
