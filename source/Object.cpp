//
// Created by Mikhail Chalakov on 2/18/24.
//
#include "Object.h"

Object::Object(type::Vector2i position, type::Vector2i size, SDL_Color color): position(position), size(size), color(color), dst({position.x, position.y, size.x, size.y}), velocity(0, 0), rising(false){}
Object::Object(type::Vector2i position, type::Vector2i size, SDL_Texture *texture): position(position), size(size), texture(texture), dst({position.x, position.y, size.x, size.y}), velocity(0, 0), rising(false),src({0, 0}) {}
