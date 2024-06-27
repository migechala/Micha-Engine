//
// Created by Mikhail Chalakov on 2/18/24.
//

//
// Created by mikhail on 5/20/21.
//

#include "Types.h"

#include <string>

#include "ObjectManager.h"
#include "ResourceLoader.h"

type::Vector2i::Vector2i(int x, int y) : x(x), y(y) {}

type::Vector2i type::Vector2i::operator+(const type::Vector2i &change) const {
  return {this->x + change.x, this->y + change.y};
}
type::Vector2i type::Vector2i::operator-(const type::Vector2i &change) const {
  return {this->x - change.x, this->y - change.y};
}
type::Vector2i type::Vector2i::operator*(const Vector2i &change) const {
  return {this->x * change.x, this->y * change.y};
}

type::Vector2i type::Vector2i::operator/(const type::Vector2i &change) const {
  return {this->x / change.x, this->y / change.y};
}

void type::Vector2i::operator+=(const type::Vector2i &change) {
  this->x += change.x;
  this->y += change.y;
}

type::Vector2d::Vector2d(double x, double y) : x(x), y(y) {}

type::Vector2d type::Vector2d::operator+(const type::Vector2d &change) const {
  return {this->x - change.x, this->y - change.y};
}

type::Vector2d type::Vector2d::operator*(const Vector2d &change) const {
  return {this->x * change.x, this->y * change.y};
}

type::Vector2d type::Vector2d::operator/(const type::Vector2d &change) const {
  return {this->x / change.x, this->y / change.y};
}
type::Vector2i type::Vector2i::operator/(const int &change) const {
  return {this->x / change, this->y / change};
}

void type::Vector2d::operator+=(const type::Vector2d &change) {
  this->x += change.x;
  this->y += change.y;
}

type::Object::Object(type::Vector2i position, type::Vector2i size,
                     SDL_Color color)
    : position(position),
      size(size),
      color(color),
      dst({position.x, position.y, size.x, size.y}),
      velocity(0, 0),
      rising(false) {}

type::Object::Object(type::Vector2i position, type::Vector2i size,
                     type::Vector2i velocity, SDL_Color color)
    : position(position),
      size(size),
      color(color),
      dst({position.x, position.y, size.x, size.y}),
      velocity(velocity),
      rising(false) {}

void type::Sprite::setPosition(type::Vector2i newPos) { position = newPos; }

void type::Sprite::changePosition(type::Vector2i change) { position += change; }

void type::Sprite::setSize(type::Vector2i newSize) { size = newSize; }

type::Vector2i type::Sprite::getPosition() const { return position; }

SDL_Texture *type::Sprite::getTexture() { return textures[counter]; }

int type::Sprite::updateTexture() {
  if (counter < textures.size() - 1) {
    return ++counter;
  }
  return --counter;
}

type::Sprite::Sprite(type::Vector2i position, type::Vector2i size)
    : counter(0), Object(position, size, {255, 0, 0, 255}) {}

type::Sprite::Sprite(type::Vector2i position, type::Vector2i size,
                     type::Vector2i velocity)
    : counter(0), Object(position, size, velocity, {255, 0, 0, 255}) {}

type::Sprite::Sprite(type::Vector2i position, type::Vector2i size,
                     std::vector<SDL_Texture *> textures)
    : counter(0), Object(position, size, {0, 0, 0, 0}), textures(textures) {}

type::Sprite::Sprite(type::Vector2i position, type::Vector2i size,
                     type::Vector2i velocity,
                     std::vector<SDL_Texture *> textures)
    : counter(0),
      Object(position, size, velocity, {0, 0, 0, 0}),
      textures(textures) {}
