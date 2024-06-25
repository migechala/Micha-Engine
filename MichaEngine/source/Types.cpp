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

type::Vector2i::Vector2i(int p_x, int p_y) : x(p_x), y(p_y) {}

type::Vector2i type::Vector2i::operator+(const type::Vector2i &p_change) const {
  return {this->x + p_change.x, this->y + p_change.y};
}
type::Vector2i type::Vector2i::operator-(const type::Vector2i &p_change) const {
  return {this->x - p_change.x, this->y - p_change.y};
}
type::Vector2i type::Vector2i::operator*(const Vector2i &p_change) const {
  return {this->x * p_change.x, this->y * p_change.y};
}

type::Vector2i type::Vector2i::operator/(const type::Vector2i &p_change) const {
  return {this->x / p_change.x, this->y / p_change.y};
}

void type::Vector2i::operator+=(const type::Vector2i &p_change) {
  this->x += p_change.x;
  this->y += p_change.y;
}

type::Vector2d::Vector2d(double p_x, double p_y) : x(p_x), y(p_y) {}

type::Vector2d type::Vector2d::operator+(const type::Vector2d &p_change) const {
  return {this->x - p_change.x, this->y - p_change.y};
}

type::Vector2d type::Vector2d::operator*(const Vector2d &p_change) const {
  return {this->x * p_change.x, this->y * p_change.y};
}

type::Vector2d type::Vector2d::operator/(const type::Vector2d &p_change) const {
  return {this->x / p_change.x, this->y / p_change.y};
}
type::Vector2i type::Vector2i::operator/(const int &p_change) const {
  return {this->x / p_change, this->y / p_change};
}

void type::Vector2d::operator+=(const type::Vector2d &p_change) {
  this->x += p_change.x;
  this->y += p_change.y;
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

void type::Sprite::setPosition(type::Vector2i p_newPos) { position = p_newPos; }

void type::Sprite::changePosition(type::Vector2i p_change) {
  position += p_change;
}

void type::Sprite::setSize(type::Vector2i p_newSize) { size = p_newSize; }

type::Vector2i type::Sprite::getPosition() const { return position; }

SDL_Texture *type::Sprite::getTexture() { return textures[m_counter]; }

int type::Sprite::updateTexture() {
  if (m_counter < textures.size() - 1) {
    return ++m_counter;
  }
  return --m_counter;
}

type::Sprite::Sprite(type::Vector2i p_position, type::Vector2i p_size)
    : m_counter(0), Object(p_position, p_size, {255, 0, 0, 255}) {}

type::Sprite::Sprite(type::Vector2i p_position, type::Vector2i p_size,
                     type::Vector2i p_velocity)
    : m_counter(0), Object(p_position, p_size, p_velocity, {255, 0, 0, 255}) {}

type::Sprite::Sprite(type::Vector2i p_position, type::Vector2i p_size,
                     std::vector<SDL_Texture *> p_textures)
    : m_counter(0),
      Object(p_position, p_size, {0, 0, 0, 0}),
      textures(p_textures) {}

type::Sprite::Sprite(type::Vector2i p_position, type::Vector2i p_size,
                     type::Vector2i p_velocity,
                     std::vector<SDL_Texture *> p_textures)
    : m_counter(0),
      Object(p_position, p_size, p_velocity, {0, 0, 0, 0}),
      textures(p_textures) {}
