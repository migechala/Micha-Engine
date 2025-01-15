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

eng::Vector2i::Vector2i() : x(0), y(0) {}

eng::Vector2i::Vector2i(int x, int y) : x(x), y(y) {}

eng::Vector2i eng::Vector2i::operator+(const eng::Vector2i &change) const {
  return {this->x + change.x, this->y + change.y};
}
eng::Vector2i eng::Vector2i::operator-(const eng::Vector2i &change) const {
  return {this->x - change.x, this->y - change.y};
}
eng::Vector2i eng::Vector2i::operator*(const Vector2i &change) const {
  return {this->x * change.x, this->y * change.y};
}

eng::Vector2i eng::Vector2i::operator/(const eng::Vector2i &change) const {
  return {this->x / change.x, this->y / change.y};
}

void eng::Vector2i::operator+=(const eng::Vector2i &change) {
  this->x += change.x;
  this->y += change.y;
}

eng::Vector2i eng::Vector2i::operator/(const int &change) const {
  return {this->x / change, this->y / change};
}
eng::Object::Object(ObjectOptions &options)
    : p_position(options.getPosition()),
      p_size(options.getSize()),
      p_velocity(options.getVelocity()),
      p_acceleration(options.getAcceleration()),
      p_flip(options.getFlip()),
      p_color(options.getColor()),
      p_gravity(options.isGravityEnabled()) {}

bool eng::Object::isSprite() { return false; }

void eng::Object::setPosition(eng::Vector2i newPosition) {
  p_position = newPosition;
}

void eng::Object::setVelocity(eng::Vector2i newVelocity) {
  p_velocity = newVelocity;
}

void eng::Object::setAcceleration(eng::Vector2i newAcceleration) {
  p_acceleration = newAcceleration;
}

int eng::Object::getId() { return p_id; }

eng::Vector2i eng::Object::getPosition() { return p_position; }

eng::Vector2i eng::Object::getSize() { return p_size; }

eng::Vector2i eng::Object::getVelocity() { return p_velocity; }

eng::Vector2i eng::Object::getAcceleration() { return p_acceleration; }

SDL_Rect &eng::Sprite::getSrc() { return p_src; }

float eng::Object::getAngle() { return p_angle; }

SDL_RendererFlip eng::Object::getFlip() { return p_flip; }

SDL_Color eng::Object::getColor() { return p_color; }

bool eng::Object::isRising() { return p_rising; }

bool eng::Object::isGrounded() { return p_grounded; }

bool eng::Object::hasGravity() { return p_gravity; }

void eng::Object::setRising(bool arg) { p_rising = arg; }

void eng::Object::setGrounded(bool arg) { p_grounded = arg; }

void eng::Sprite::setCutOut(eng::Vector2i pos) {
  p_src.x = pos.x;
  p_src.y = pos.y;
}

std::shared_ptr<SDL_Texture> eng::Sprite::getTexture() {
  return spritesheets[spritesheetIndex];
}

std::vector<eng::Vector2i> eng::Sprite::getCutOuts() {
  return cutOuts[spritesheetIndex];
}

void eng::Sprite::updateTexture(int frame) {
  if (frame % framesPerUpdate == 0) {
    if (currentCutIndex >= cutOuts[spritesheetIndex].size()) {
      if (p_update) {
        currentCutIndex = 0;
      } else {
        return;
      }
    } else {
      ++currentCutIndex;
    }
    setCutOut(cutOuts[spritesheetIndex][currentCutIndex]);
  }
}

void eng::Sprite::changeSpritesheet(int index) {
  if (spritesheetIndex != index) {
    spritesheetIndex = index;
    currentCutIndex = 0;
  }
}

bool eng::Sprite::isSprite() { return true; }

void eng::Sprite::toggleUpdate() { p_update = !p_update; }

void eng::Sprite::setFramesPerUpdate(int fpu) { framesPerUpdate = fpu; }

eng::Sprite::Sprite(SpriteOptions &options)
    : spritesheetIndex(0),
      spritesheets(options.getTextures()),
      Object(options),
      p_src(
          {0, 0, options.getRealSpriteSize().x, options.getRealSpriteSize().y}),
      numSpritesPerSheet(options.getNumberOfSpritesPerSheet()),
      currentCutIndex(0),
      framesPerUpdate(options.getFramesPerTextureUpdate()),
      p_update(true) {
  int w, h;
  SDL_QueryTexture(getTexture().get(), NULL, NULL, &w, &h);
  int numColumns = w / options.getRealSpriteSize().x;
  cutOuts.resize(numSpritesPerSheet.size());
  for (int i = 0; i < numSpritesPerSheet.size(); i++) {
    for (int j = 0; j < numSpritesPerSheet[i]; j++) {
      cutOuts[i].push_back({(j % numColumns) * options.getRealSpriteSize().x,
                            (j / numColumns) * options.getRealSpriteSize().y});
    }
  }
}

eng::ObjectOptions::ObjectOptions() {}

eng::ObjectOptions &eng::ObjectOptions::setPosition(eng::Vector2i position) {
  p_position = position;
  return (*this);
}

eng::ObjectOptions &eng::ObjectOptions::setSize(eng::Vector2i size) {
  p_size = size;
  return (*this);
}

eng::ObjectOptions &eng::ObjectOptions::setVelocity(eng::Vector2i velocity) {
  p_velocity = velocity;
  return (*this);
}

eng::ObjectOptions &eng::ObjectOptions::setAcceleration(
    eng::Vector2i acceleration) {
  p_acceleration = acceleration;
  return (*this);
}

eng::ObjectOptions &eng::ObjectOptions::setFlip(SDL_RendererFlip flip) {
  p_flip = flip;
  return (*this);
}

eng::ObjectOptions &eng::ObjectOptions::setColor(SDL_Color color) {
  p_color = color;
  return (*this);
}

eng::ObjectOptions &eng::ObjectOptions::enableGravity() {
  p_gravity = true;
  return (*this);
}

eng::Vector2i eng::ObjectOptions::getPosition() const { return p_position; }

eng::Vector2i eng::ObjectOptions::getSize() const { return p_size; }

eng::Vector2i eng::ObjectOptions::getVelocity() const { return p_velocity; }

eng::Vector2i eng::ObjectOptions::getAcceleration() const {
  return p_acceleration;
}

SDL_RendererFlip eng::ObjectOptions::getFlip() const { return p_flip; }

SDL_Color eng::ObjectOptions::getColor() const { return p_color; }

bool eng::ObjectOptions::isGravityEnabled() const { return p_gravity; }

eng::SpriteOptions::SpriteOptions() {}

eng::SpriteOptions &eng::SpriteOptions::setTextures(
    std::vector<std::shared_ptr<SDL_Texture>> textures) {
  p_textures = textures;
  return (*this);
}

eng::SpriteOptions &eng::SpriteOptions::setNumberOfSpritesPerSheet(
    std::vector<int> num) {
  p_numSpritesPerSheet = num;
  return (*this);
}

eng::SpriteOptions &eng::SpriteOptions::setRealSpriteSize(
    eng::Vector2i spriteSize) {
  p_spriteSize = spriteSize;
  return (*this);
}

eng::SpriteOptions &eng::SpriteOptions::setFramesPerTextureUpdate(
    int numFrames) {
  p_fptu = numFrames;
  return (*this);
}

void eng::ObjectOptions::setId(int id) { id = id; }

int eng::SpriteOptions::getFramesPerTextureUpdate() { return p_fptu; }

std::vector<int> eng::SpriteOptions::getNumberOfSpritesPerSheet() {
  return p_numSpritesPerSheet;
}

std::vector<std::shared_ptr<SDL_Texture>> eng::SpriteOptions::getTextures() {
  return p_textures;
}

eng::Vector2i eng::SpriteOptions::getRealSpriteSize() { return p_spriteSize; }

int eng::ObjectOptions::getId() { return id; }
