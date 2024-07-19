#include "ResourceLoader.h"

#include <SDL2/SDL_image.h>

#include <iostream>
#include <memory>

#include "Logger.h"
#include "SDLDeleter.h"

ResourceLoader *ResourceLoader::instance = nullptr;

ResourceLoader *ResourceLoader::getInstance() {
  if (instance == nullptr) {
    instance = new ResourceLoader();
  }
  return instance;
}

std::shared_ptr<SDL_Texture> ResourceLoader::loadTexture(
    SDL_Renderer *renderer, const std::string &location) {
  SDL_Surface *srf = IMG_Load(location.c_str());
  if (!srf) {
    LOG_ERR("Surface is null " + location + " >>> " + SDL_GetError());
    return nullptr;
  }
  std::shared_ptr<SDL_Texture> texturePtr(
      SDL_CreateTextureFromSurface(renderer, srf), SDLDeleter());

  SDL_FreeSurface(srf);

  if (!texturePtr) {
    LOG_ERR("Texture is null " + location + " >>> " + SDL_GetError());
    return nullptr;
  }

  std::cout << "Loaded texture " << location << " to address: " << texturePtr
            << std::endl;
  return texturePtr;
}
