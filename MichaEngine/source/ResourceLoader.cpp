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

  SDL_Texture *txtr = SDL_CreateTextureFromSurface(renderer, srf);
  SDL_FreeSurface(srf);

  if (!txtr) {
    LOG_ERR("Texture is null " + location + " >>> " + SDL_GetError());
    return nullptr;
  }

  std::shared_ptr<SDL_Texture> texturePtr;

  texturePtr.reset(txtr, SDLDeleter());
  std::cout << "Loaded texture " << location << " to address: " << txtr
            << std::endl;
  return texturePtr;
}
