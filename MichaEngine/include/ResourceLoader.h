#pragma once

#include <cstdio>
#include <SDL2/SDL_system.h>
#include <string>

class ResourceLoader {
    static ResourceLoader* m_instance;
public:
    static ResourceLoader* getInstance();

    static SDL_Texture *loadTexture(const std::string &location, SDL_Renderer *renderer);
};
