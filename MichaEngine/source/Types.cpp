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

void eng::Vector2i::operator-=(const Vector2i &change) {
  this->x -= change.x;
  this->y += change.y;
}

eng::Vector2i eng::Vector2i::operator/(const int &change) const {
  return {this->x / change, this->y / change};
}
eng::Object::Object(ObjectOptions &options)
    : p_options(options), p_flip(options.getFlip()) {}

void eng::Object::accept(Visitor &visitor) { visitor.visit(*this); }

void eng::Object::setPosition(eng::Vector2i newPosition) {
  p_options.setPosition(newPosition);
}

void eng::Object::setVelocity(eng::Vector2i newVelocity) {
  p_options.setVelocity(newVelocity);
}

void eng::Object::setAcceleration(eng::Vector2i newAcceleration) {
  p_options.setAcceleration(newAcceleration);
}

int eng::Object::getId() { return p_options.getId(); }

eng::ObjectOptions eng::Object::getOptions() { return p_options; }

SDL_Rect &eng::Sprite::getSrc() { return p_src; }

float eng::Object::getAngle() { return p_angle; }

SDL_RendererFlip eng::Object::getFlip() { return p_flip; }

bool eng::Object::isRising() { return p_rising; }

bool eng::Object::isGrounded() { return p_grounded; }

void eng::Object::setRising(bool arg) { p_rising = arg; }

void eng::Object::setGrounded(bool arg) { p_grounded = arg; }

void eng::Sprite::accept(Visitor &visitor) { visitor.visit(*this); }

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

eng::UpdateTextureVisitor::UpdateTextureVisitor(int frame) : frame(frame) {}

void eng::UpdateTextureVisitor::visit(Object &obj) { return; }

void eng::UpdateTextureVisitor::visit(Sprite &sprite) {
  sprite.updateTexture(frame);
}
