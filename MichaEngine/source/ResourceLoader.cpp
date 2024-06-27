#include "ResourceLoader.h"

#include <SDL2/SDL_image.h>

#include <iostream>

#include "Logger.h"

ResourceLoader *ResourceLoader::instance = nullptr;
ResourceLoader *ResourceLoader::getInstance() {
  if (instance == nullptr) {
    instance = new ResourceLoader();
  }
  return instance;
}

SDL_Texture *ResourceLoader::loadTexture(const std::string &location,
                                         SDL_Renderer *renderer) {
  SDL_Surface *srf = IMG_Load(location.c_str());

  SDL_Texture *txtr = SDL_CreateTextureFromSurface(renderer, srf);

  SDL_FreeSurface(srf);

  if (txtr == nullptr) {
    LOG_ERR("Surface is null " + location + " >>> " + SDL_GetError());
  }

  return txtr;
}
