#include <SDL2/SDL_image.h>
#include "ResourceLoader.h"
#include "Logger.h"
#include <iostream>

ResourceLoader *ResourceLoader::m_instance = nullptr;
ResourceLoader *ResourceLoader::getInstance() {
    if(m_instance == nullptr){
        m_instance = new ResourceLoader();
    }
    return  m_instance;
}

SDL_Texture *ResourceLoader::loadTexture(const std::string& location, SDL_Renderer* renderer) {
    SDL_Surface *srf = IMG_Load(location.c_str());

    SDL_Texture* txtr = SDL_CreateTextureFromSurface(renderer, srf);

    SDL_FreeSurface(srf);

    if(txtr == nullptr){
        LOG_ERR("Surface is null " + location + " >>> " + SDL_GetError());
    }

    return txtr;
}
