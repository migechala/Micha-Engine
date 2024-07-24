#pragma once

#include <SDL2/SDL_system.h>

#include <memory>
#include <string>
#include <vector>

#include "SDLDeleter.h"

namespace ResourceLoader {
std::shared_ptr<SDL_Texture> loadTexture(std::shared_ptr<SDL_Renderer> renderer,
                                         const std::string &location);

template <class... Args>
std::vector<std::shared_ptr<SDL_Texture>> loadTextures(
    std::shared_ptr<SDL_Renderer> renderer, Args... fileNames) {
  std::vector<std::shared_ptr<SDL_Texture>> textures;
  (textures.push_back(loadTexture(renderer, fileNames)), ...);
  return textures;
}
};  // namespace ResourceLoader
