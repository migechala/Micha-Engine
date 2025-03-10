#include "Types.h"
#include "Logger.h"

namespace eng {

// Vector2i implementation
Vector2i::Vector2i(int x, int y) : x(x), y(y) {}

Vector2i Vector2i::operator+(const Vector2i &change) const { return {x + change.x, y + change.y}; }

Vector2i Vector2i::operator-(const Vector2i &change) const { return {x - change.x, y - change.y}; }

Vector2i Vector2i::operator/(const Vector2i &change) const { return {x / change.x, y / change.y}; }

Vector2i Vector2i::operator/(int change) const { return {x / change, y / change}; }

Vector2i Vector2i::operator*(const Vector2i &change) const { return {x * change.x, y * change.y}; }

void Vector2i::operator+=(const Vector2i &change) {
  x += change.x;
  y += change.y;
}

void Vector2i::operator=(const Vector2i &newVector) {
  x = newVector.x;
  y = newVector.y;
}

void Vector2i::operator-=(const Vector2i &change) {
  x -= change.x;
  y -= change.y;
}

// SpriteOptions implementation
SpriteOptions &SpriteOptions::setPosition(Vector2i newPos) {
  p_position = newPos;
  return *this;
}

SpriteOptions &SpriteOptions::setSize(Vector2i newSize) {
  p_size = newSize;

  return *this;
}

SpriteOptions &SpriteOptions::setHitbox(Vector2i size) {
  p_hitbox = size;
  return *this;
}

SpriteOptions &SpriteOptions::setHitboxOffset(Vector2i offset) {
  p_hitboxOffset = offset;
  return *this;
}

SpriteOptions &SpriteOptions::setVelocity(Vector2i velocity) {
  p_velocity = velocity;
  return *this;
}

SpriteOptions &SpriteOptions::setAcceleration(Vector2i acceleration) {
  p_acceleration = acceleration;
  return *this;
}

SpriteOptions &SpriteOptions::setFlip(SDL_RendererFlip flip) {
  p_flip = flip;
  return *this;
}

SpriteOptions &SpriteOptions::setColor(SDL_Color color) {
  p_color = color;
  return *this;
}

SpriteOptions &SpriteOptions::enableGravity() {
  p_gravity = true;
  return *this;
}

SpriteOptions &SpriteOptions::setTextures(std::vector<std::shared_ptr<SDL_Texture>> textures) {
  p_textures = textures;
  return *this;
}

SpriteOptions &SpriteOptions::setNumberOfSpritesPerSheet(std::vector<int> num) {
  p_numSpritesPerSheet = num;
  return *this;
}

SpriteOptions &SpriteOptions::setRealSpriteSize(Vector2i spriteSize) {
  p_size = spriteSize;
  p_realSpriteSize = spriteSize;
  return *this;
}

SpriteOptions &SpriteOptions::setFramesPerTextureUpdate(int numFrames) {
  p_fptu = numFrames;
  return *this;
}

// Sprite implementation
Sprite::Sprite(SpriteOptions &options)
    : spritesheetIndex(0), p_src{0, 0, options.getRealSpriteSize().x, options.getRealSpriteSize().y},
      numSpritesPerSheet(options.getNumberOfSpritesPerSheet()), currentCutIndex(0),
      framesPerUpdate(options.getFramesPerTextureUpdate()), p_update(true), p_options(options) {
  int w, h;
  SDL_QueryTexture(getTexture().get(), NULL, NULL, &w, &h);
  int numColumns = w / options.getRealSpriteSize().x;
  cutOuts.resize(numSpritesPerSheet.size());
  for (size_t i = 0; i < numSpritesPerSheet.size(); i++) {
    for (int j = 0; j < numSpritesPerSheet[i]; j++) {
      cutOuts[i].push_back(
          {(j % numColumns) * options.getRealSpriteSize().x, (j / numColumns) * options.getRealSpriteSize().y});
    }
  }
}

void Sprite::updateTexture(int frame) {
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

void Sprite::changeSpritesheet(int index) {
  if (spritesheetIndex != index) {
    spritesheetIndex = index;
    currentCutIndex = 0;
  }
}

} // namespace eng
