#include "Types.h"
#include "Logger.h"
#include <iostream>
namespace eng {
Vector2i::Vector2i(const Vector2f &vec) : x(static_cast<int>(vec.x)), y(static_cast<int>(vec.y)) {}

// SpriteOptions implementation
SpriteOptions &SpriteOptions::setPosition(Vector2i newPos) {
  p_position = newPos;
  return *this;
}

SpriteOptions &SpriteOptions::setSize(Vector2i newSize) {
  if (newSize.x <= 0 || newSize.y <= 0) {
    Logger::log("Warning: Sprite size must be positive. No changes made.", LOG_VALUES::WARNING, LOG_LEVEL::MEDIUM,
                __FILE__, __LINE__);
    return *this;
  }
  p_size = newSize;
  return *this;
}

SpriteOptions &SpriteOptions::setHitbox(Vector2i size) {
  if (size.x <= 0 || size.y <= 0) {
    Logger::log("Warning: Hitbox size must be positive. No changes made.", LOG_VALUES::WARNING, LOG_LEVEL::MEDIUM,
                __FILE__, __LINE__);
    return *this;
  }
  p_hitbox = size;
  return *this;
}

SpriteOptions &SpriteOptions::setHitboxOffset(Vector2i offset) {
  if (offset.x <= 0 || offset.y <= 0) {
    Logger::log("Warning: Sprite hitbox offset must be positive. No changes made.", LOG_VALUES::WARNING,
                LOG_LEVEL::MEDIUM, __FILE__, __LINE__);
    return *this;
  }
  p_hitboxOffset = offset;
  return *this;
}

SpriteOptions &SpriteOptions::setVelocity(Vector2f velocity) {
  p_velocity = velocity;
  return *this;
}

SpriteOptions &SpriteOptions::setAcceleration(Vector2f acceleration) {
  p_acceleration = acceleration;
  return *this;
}

SpriteOptions &SpriteOptions::setFlip(SDL_RendererFlip flip) {
  if (flip != SDL_FLIP_NONE && flip != SDL_FLIP_HORIZONTAL && flip != SDL_FLIP_VERTICAL) {
    Logger::log("Warning: Invalid sprite flip value. No changes made.", LOG_VALUES::WARNING, LOG_LEVEL::LOW, __FILE__,
                __LINE__);
    return *this;
  }
  p_flip = flip;
  return *this;
}

SpriteOptions &SpriteOptions::setColor(SDL_Color color) {
  if (color.r < 0 || color.r > 255 || color.g < 0 || color.g > 255 || color.b < 0 || color.b > 255 || color.a < 0 ||
      color.a > 255) {
    Logger::log("Warning: Color must be valid (0, 0, 0) - (255, 255, 255) bound.", LOG_VALUES::WARNING,
                LOG_LEVEL::MEDIUM, __FILE__, __LINE__);
    return *this;
  }
  p_color = color;
  return *this;
}

SpriteOptions &SpriteOptions::enableGravity() {
  p_gravity = true;
  return *this;
}

SpriteOptions &SpriteOptions::setTextures(std::vector<std::shared_ptr<SDL_Texture>> textures, bool surpress) {
  if (textures.empty()) {
    Logger::log("Warning: Sprite size must be positive. Changes have been made however. SURPRESS WARNING BY ADDING "
                "TRUE BOOL PARAM",
                LOG_VALUES::WARNING, LOG_LEVEL::MEDIUM, __FILE__, __LINE__);
  }
  p_textures = textures;
  return *this;
}

SpriteOptions &SpriteOptions::setNumberOfSpritesPerSheet(std::vector<int> num) {
  if (num.size() != p_textures.size()) {
    Logger::log("Warning: number of sprite per sheet missmatch with sheet textures. Truncated.", LOG_VALUES::WARNING,
                LOG_LEVEL::LOW, __FILE__, __LINE__);
    if (num.size() > p_textures.size()) {
      num.resize(p_textures.size());
    }
    return *this;
  }
  p_numSpritesPerSheet = num;
  return *this;
}

SpriteOptions &SpriteOptions::setRealSpriteSize(Vector2i spriteSize) {
  if (spriteSize.x <= 0 || spriteSize.y <= 0) {
    Logger::log("Warning: Sprite size must be positive. No changes made.", LOG_VALUES::WARNING, LOG_LEVEL::LOW,
                __FILE__, __LINE__);
    return *this;
  }
  p_size = spriteSize;
  p_realSpriteSize = spriteSize;
  return *this;
}

SpriteOptions &SpriteOptions::setFramesPerTextureUpdate(int numFrames) {
  if (numFrames <= 0) {
    Logger::log("Warning: The number of frames per texture update must be > 0. No changes made.", LOG_VALUES::WARNING,
                LOG_LEVEL::LOW, __FILE__, __LINE__);
    return *this;
  }
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
