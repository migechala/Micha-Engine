#pragma once
#include "Types.h"

struct ObjectBase{
    ObjectBase(type::Vector2i position, type::Vector2i size, SDL_Color color);
    type::Vector2i velocity;
    type::Vector2i size;
    SDL_Rect dst;
    SDL_Color color;
};

class Tile : public ObjectBase{
public:
    Tile (type::Vector2i position, type::Vector2i size, SDL_Color color);
};