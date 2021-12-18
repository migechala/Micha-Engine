#include <iostream>
#include "Objects.h"
#include "Logger.h"
#include "Types.h"

ObjectBase::ObjectBase(type::Vector2i position, type::Vector2i size, SDL_Color color): size(size), color(color), dst({position.x, position.y, size.x, size.y}), velocity(0, 0){}

Tile::Tile(type::Vector2i position, type::Vector2i size, SDL_Color color): ObjectBase(position, size, color){}