#include "ResourceLoader.h"

#include <SDL2/SDL_image.h>

#include <iostream>
#include <memory>

#include "FileManager.h"
#include "Logger.h"
#include "SDLDeleter.h"

std::shared_ptr<SDL_Texture> ResourceLoader::loadTexture(std::shared_ptr<SDL_Renderer> renderer,
                                                         const std::string &location) {
  std::string path = std::string(FileManager::getInstance()->getPath(location));
  SDL_Surface *srf = IMG_Load(path.c_str());
  if (!srf) {
    LOG_ERR("Surface is null " + path + " >>> " + SDL_GetError());
    throw std::invalid_argument("File '" + path + "' could not be loaded");
  }
  std::shared_ptr<SDL_Texture> texturePtr(SDL_CreateTextureFromSurface(renderer.get(), srf), SDLDeleter());

  SDL_FreeSurface(srf);

  if (!texturePtr) {
    LOG_ERR("Texture is null " + path + " >>> " + SDL_GetError());
    return nullptr;
  }
  LOG_INFO("Loaded texture " + path + " to address: " + std::to_string(reinterpret_cast<uintptr_t>(texturePtr.get())),
           LOG_LEVEL::PRIORITY);
  std::cout << "Loaded texture " << path << " to address: " << texturePtr << std::endl;
  return texturePtr;
}
