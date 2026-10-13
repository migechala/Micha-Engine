#include "Types.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>
#include <utility>
namespace eng {
// SpriteOptions implementation
SpriteOptions &SpriteOptions::setPosition(Vector2<int> newPos) {
  p_position = newPos;
  return *this;
}

SpriteOptions &SpriteOptions::setSize(Vector2<int> newSize) {
  if (newSize.x <= 0 || newSize.y <= 0) {
    Logger::log("Warning: Sprite size must be positive. No changes made.", LOG_VALUES::WARNING, LOG_LEVEL::MEDIUM,
                __FILE__, __LINE__);
    return *this;
  }
  p_size = newSize;
  return *this;
}

SpriteOptions &SpriteOptions::setHitbox(Vector2<int> size) {
  if (size.x <= 0 || size.y <= 0) {
    Logger::log("Warning: Hitbox size must be positive. No changes made.", LOG_VALUES::WARNING, LOG_LEVEL::MEDIUM,
                __FILE__, __LINE__);
    return *this;
  }
  p_hitbox = size;
  return *this;
}

SpriteOptions &SpriteOptions::setHitboxOffset(Vector2<int> offset) {
  p_hitboxOffset = offset;
  return *this;
}

SpriteOptions &SpriteOptions::setVelocity(Vector2<float> velocity) {
  p_velocity = velocity;
  return *this;
}

SpriteOptions &SpriteOptions::setAcceleration(Vector2<float> acceleration) {
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

SpriteOptions &SpriteOptions::setSolid(bool solid) {
  p_solid = solid;
  return *this;
}

SpriteOptions &SpriteOptions::setStatic(bool isStatic) {
  p_static = isStatic;
  return *this;
}

SpriteOptions &SpriteOptions::setTextures(std::vector<std::shared_ptr<SDL_Texture>> textures, bool suppressWarning) {
  if (textures.empty() && !suppressWarning) {
    Logger::log("Warning: no textures were provided for sprite.", LOG_VALUES::WARNING, LOG_LEVEL::MEDIUM, __FILE__,
                __LINE__);
  }
  p_textures = std::move(textures);
  return *this;
}

SpriteOptions &SpriteOptions::setNumberOfSpritesPerSheet(std::vector<int> num) {
  if (num.size() != p_textures.size()) {
    Logger::log("Warning: number of sprites per sheet mismatch with sheet textures. Adjusting to match textures.",
                LOG_VALUES::WARNING, LOG_LEVEL::LOW, __FILE__, __LINE__);
    num.resize(p_textures.size(), 1);
  }
  p_numSpritesPerSheet = std::move(num);
  return *this;
}

SpriteOptions &SpriteOptions::setRealSpriteSize(Vector2<int> spriteSize) {
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
  const auto &textures = options.getTextures();
  if (textures.empty() || numSpritesPerSheet.empty() || options.getRealSpriteSize().x <= 0 ||
      options.getRealSpriteSize().y <= 0) {
    return;
  }

  cutOuts.resize(std::min(numSpritesPerSheet.size(), textures.size()));
  sourceSizes.resize(cutOuts.size(), options.getRealSpriteSize());
  for (size_t i = 0; i < cutOuts.size(); i++) {
    int w = 0;
    int h = 0;
    if (!textures[i] || SDL_QueryTexture(textures[i].get(), NULL, NULL, &w, &h) != 0) {
      continue;
    }

    eng::Vector2<int> sourceSize = options.getRealSpriteSize();
    if (numSpritesPerSheet[i] > 1) {
      sourceSize = {std::max(1, w / numSpritesPerSheet[i]), h};
    } else {
      sourceSize = {std::min(sourceSize.x, w), std::min(sourceSize.y, h)};
    }
    sourceSizes[i] = sourceSize;

    int numColumns = std::max(1, w / sourceSize.x);
    for (int j = 0; j < numSpritesPerSheet[i]; j++) {
      cutOuts[i].push_back({(j % numColumns) * sourceSize.x, (j / numColumns) * sourceSize.y});
    }
  }

  if (!sourceSizes.empty()) {
    setSourceSize(sourceSizes[0]);
  }
}

void Sprite::updateTexture(int frame) {
  if (p_manualSource || framesPerUpdate <= 0 || cutOuts.empty() || spritesheetIndex < 0 ||
      spritesheetIndex >= static_cast<int>(cutOuts.size()) || cutOuts[spritesheetIndex].empty()) {
    return;
  }
  if (frame % framesPerUpdate == 0) {
    setCutOut(cutOuts[spritesheetIndex][currentCutIndex]);
    if (currentCutIndex + 1 >= static_cast<int>(cutOuts[spritesheetIndex].size())) {
      if (!p_update) {
        return;
      }
      currentCutIndex = 0;
    } else {
      currentCutIndex++;
    }
  }
}

void Sprite::changeSpritesheet(int index) {
  if (index < 0 || index >= static_cast<int>(cutOuts.size())) {
      LOG_WARN("Warning: Invalid spritesheet index. No changes made.", LOG_LEVEL::LOW);
    return;
  }
  if (spritesheetIndex != index) {
    spritesheetIndex = index;
    currentCutIndex = 0;
    if (spritesheetIndex < static_cast<int>(sourceSizes.size())) {
      setSourceSize(sourceSizes[spritesheetIndex]);
    }
    if (!cutOuts[spritesheetIndex].empty()) {
      setCutOut(cutOuts[spritesheetIndex][currentCutIndex]);
    }
  }
}

} // namespace eng
