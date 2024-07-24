//
// Created by Mikhail Chalakov on 2/18/24.
//

//
// Created by mikhail on 5/20/21.
//

#include "Types.h"

#include <iostream>
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

type::Vector2i type::Vector2i::operator/(const int &change) const {
  return {this->x / change, this->y / change};
}

type::Object::Object()
    : p_position({0, 0}),
      p_size({100, 00}),
      p_color({0, 0, 0, 0}),
      p_velocity({0, 0}),
      p_acceleration({0, 0}),
      p_gravity(false),
      p_rising(false),
      p_grounded(false) {}

bool type::Object::isSprite() { return false; }

type::Object *type::Object::gravityOn() {
  p_gravity = true;
  return this;
}

type::Object *type::Object::setPosition(type::Vector2i newPos) {
  p_position = newPos;
  return this;
}

type::Object *type::Object::setSize(type::Vector2i newSize) {
  p_size = newSize;
  return this;
}

type::Object *type::Object::setVelocity(Vector2i newVelocity) {
  p_velocity = newVelocity;
  return this;
}

type::Object *type::Object::setColor(SDL_Color color) {
  p_color = color;
  return this;
}

type::Object *type::Object::setAcceleration(Vector2i newAcceleration) {
  p_acceleration = newAcceleration;
  return this;
}

type::Object *type::Object::setId(int id) {
  p_id = id;
  return this;
}

int type::Object::getId() { return p_id; }

type::Object *type::Object::setFlip(SDL_RendererFlip flip) {
  p_flip = flip;
  return this;
}

type::Vector2i type::Object::getPosition() { return p_position; }

type::Vector2i type::Object::getSize() { return p_size; }

type::Vector2i type::Object::getVelocity() { return p_velocity; }

type::Vector2i type::Object::getAcceleration() { return p_acceleration; }

SDL_Rect &type::Sprite::getSrc() { return p_src; }

float type::Object::getAngle() { return p_angle; }

SDL_RendererFlip type::Object::getFlip() { return p_flip; }

SDL_Color type::Object::getColor() { return p_color; }

bool type::Object::isRising() { return p_rising; }

bool type::Object::isGrounded() { return p_grounded; }

bool type::Object::hasGravity() { return p_gravity; }

void type::Object::setRising(bool arg) { p_rising = arg; }

void type::Object::setGrounded(bool arg) { p_grounded = arg; }

void type::Sprite::setCutOut(type::Vector2i pos) {
  p_src.x = pos.x;
  p_src.y = pos.y;
}

std::shared_ptr<SDL_Texture> type::Sprite::getTexture() {
  return spritesheets[spritesheetIndex];
}

std::vector<type::Vector2i> *type::Sprite::getCutOuts() { return &cutOuts; }

void type::Sprite::updateTexture() {
  if (currentCutIndex >= cutOuts.size()) {
    currentCutIndex = 0;
  } else {
    ++currentCutIndex;
  }
  setCutOut(cutOuts[currentCutIndex]);
}

void type::Sprite::changeSpritesheet(int index) { spritesheetIndex = index; }

bool type::Sprite::isSprite() { return true; }

type::Sprite::Sprite(std::vector<std::shared_ptr<SDL_Texture>> spritesheets,
                     std::vector<int> numSpritesPerSheet,
                     type::Vector2i spriteSize)
    : spritesheetIndex(0),
      spritesheets(spritesheets),
      p_src({0, 0, spriteSize.x, spriteSize.y}),
      numSpritesPerSheet(numSpritesPerSheet),
      currentCutIndex(0) {
  setSize(spriteSize);
  int w, h;
  SDL_QueryTexture(getTexture().get(), NULL, NULL, &w, &h);

  int numColumns = w / spriteSize.x;
  for (int i = 0; i < numSpritesPerSheet[spritesheetIndex]; ++i) {
    cutOuts.push_back(
        {(i % numColumns) * spriteSize.x, (i / numColumns) * spriteSize.y});
  }
}
