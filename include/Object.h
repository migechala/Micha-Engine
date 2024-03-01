//
// Created by Mikhail Chalakov on 2/18/24.
//

#pragma once
#include "Types.h"
struct Object
{
    Object(type::Vector2i position, type::Vector2i size, SDL_Color color);
    Object(type::Vector2i position, type::Vector2i size, SDL_Texture *texture);
    type::Vector2i position;
    type::Vector2i velocity;
    type::Vector2i size;
    SDL_Rect dst;
    SDL_Rect src;
    SDL_Color color;
    SDL_Texture *texture;
    bool rising;
    bool animated;
};