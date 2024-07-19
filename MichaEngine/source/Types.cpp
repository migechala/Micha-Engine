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

type::Object::Object(type::Vector2i position, type::Vector2i size,
                     SDL_Color color)
    : position(position),
      size(size),
      color(color),
      dst({position.x, position.y, size.x, size.y}),
      src({0, 0, size.x, size.y}),
      velocity(0, 0),
      acceleration(0, 0),
      rising(false),
      grounded(false) {}

type::Object::Object(type::Vector2i position, type::Vector2i size,
                     type::Vector2i velocity, type::Vector2i acceleration,
                     SDL_Color color)
    : position(position),
      size(size),
      color(color),
      dst({position.x, position.y, size.x, size.y}),
      src({0, 0, size.x, size.y}),
      velocity(velocity),
      acceleration(acceleration),
      rising(false),
      grounded(false) {}

bool type::Object::isSprite() { return false; }

void type::Sprite::setPosition(type::Vector2i newPos) { position = newPos; }

void type::Sprite::changePosition(type::Vector2i change) { position += change; }

void type::Sprite::setSize(type::Vector2i newSize) { size = newSize; }

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
  src.x = cutOuts[currentCutIndex].x;
  src.y = cutOuts[currentCutIndex].y;
}

void type::Sprite::changeSpritesheet(int index) { spritesheetIndex = index; }

bool type::Sprite::isSprite() { return true; }

type::Sprite::Sprite(type::Vector2i position, type::Vector2i size,
                     std::vector<std::shared_ptr<SDL_Texture>> spritesheets,
                     std::vector<int> numSpritesPerSheet,
                     type::Vector2i spriteSize)
    : spritesheetIndex(0),
      Object(position, size, {0, 0, 0, 0}),
      spritesheets(spritesheets),
      numSpritesPerSheet(numSpritesPerSheet),
      currentCutIndex(0) {
  src = {0, 0, spriteSize.x, spriteSize.y};
}

type::Sprite::Sprite(type::Vector2i position, type::Vector2i size,
                     type::Vector2i velocity, type::Vector2i acceleration,
                     std::vector<std::shared_ptr<SDL_Texture>> spritesheets,
                     std::vector<int> numSpritesPerSheet,
                     type::Vector2i spriteSize)
    : spritesheetIndex(0),
      Object(position, size, velocity, acceleration, {0, 0, 0, 0}),
      spritesheets(spritesheets),
      numSpritesPerSheet(numSpritesPerSheet),
      currentCutIndex(0) {
  src = {0, 0, spriteSize.x, spriteSize.y};
  int w, h;
  SDL_QueryTexture(getTexture().get(), NULL, NULL, &w, &h);

  int numColumns = w / spriteSize.x;
  for (int i = 0; i < numSpritesPerSheet[spritesheetIndex]; ++i) {
    cutOuts.push_back(
        {(i % numColumns) * spriteSize.x, (i / numColumns) * spriteSize.y});
  }
}
