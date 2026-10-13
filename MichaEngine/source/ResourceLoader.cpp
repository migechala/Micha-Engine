#include "ResourceLoader.h"

#include <SDL_image.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "FileManager.h"
#include "Logger.h"
#include "SDLDeleter.h"

namespace {
std::shared_ptr<SDL_Texture> loadAnimatedGifAsSpritesheet(SDL_Renderer *renderer, const std::string &path) {
  IMG_Animation *animation = IMG_LoadAnimation(path.c_str());
  if (!animation || animation->count <= 0) {
    if (animation) {
      IMG_FreeAnimation(animation);
    }
    return nullptr;
  }

  int frameWidth = 0;
  int frameHeight = 0;
  for (int i = 0; i < animation->count; ++i) {
    if (animation->frames[i]) {
      frameWidth = std::max(frameWidth, animation->frames[i]->w);
      frameHeight = std::max(frameHeight, animation->frames[i]->h);
    }
  }

  if (frameWidth <= 0 || frameHeight <= 0) {
    IMG_FreeAnimation(animation);
    return nullptr;
  }

  SDL_Surface *spritesheet = SDL_CreateRGBSurfaceWithFormat(0, frameWidth * animation->count, frameHeight, 32,
                                                            SDL_PIXELFORMAT_RGBA32);
  if (!spritesheet) {
    IMG_FreeAnimation(animation);
    return nullptr;
  }

  SDL_FillRect(spritesheet, nullptr, SDL_MapRGBA(spritesheet->format, 0, 0, 0, 0));
  SDL_SetSurfaceBlendMode(spritesheet, SDL_BLENDMODE_NONE);

  for (int i = 0; i < animation->count; ++i) {
    SDL_Surface *frame = animation->frames[i];
    if (!frame) {
      continue;
    }

    SDL_Surface *convertedFrame = SDL_ConvertSurfaceFormat(frame, SDL_PIXELFORMAT_RGBA32, 0);
    if (!convertedFrame) {
      continue;
    }

    Uint32 colorKey = 0;
    if (SDL_GetColorKey(frame, &colorKey) == 0) {
      Uint8 keyR = 0;
      Uint8 keyG = 0;
      Uint8 keyB = 0;
      SDL_GetRGB(colorKey, frame->format, &keyR, &keyG, &keyB);

      auto *pixels = static_cast<Uint32 *>(convertedFrame->pixels);
      const int pixelCount = convertedFrame->w * convertedFrame->h;
      for (int pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex) {
        Uint8 r = 0;
        Uint8 g = 0;
        Uint8 b = 0;
        Uint8 a = 0;
        SDL_GetRGBA(pixels[pixelIndex], convertedFrame->format, &r, &g, &b, &a);
        if (r == keyR && g == keyG && b == keyB) {
          pixels[pixelIndex] = SDL_MapRGBA(convertedFrame->format, r, g, b, 0);
        }
      }
    }

    SDL_SetSurfaceBlendMode(convertedFrame, SDL_BLENDMODE_BLEND);
    SDL_Rect destination{i * frameWidth, frameHeight - convertedFrame->h, convertedFrame->w, convertedFrame->h};
    SDL_BlitSurface(convertedFrame, nullptr, spritesheet, &destination);
    SDL_FreeSurface(convertedFrame);
  }

  std::shared_ptr<SDL_Texture> texturePtr(SDL_CreateTextureFromSurface(renderer, spritesheet), SDLDeleter());
  if (texturePtr) {
    SDL_SetTextureBlendMode(texturePtr.get(), SDL_BLENDMODE_BLEND);
  }
  SDL_FreeSurface(spritesheet);
  IMG_FreeAnimation(animation);
  return texturePtr;
}
} // namespace

std::shared_ptr<SDL_Texture> ResourceLoader::loadTexture(std::shared_ptr<SDL_Renderer> renderer,
                                                         const std::string &location) {
  std::string path = std::string(FileManager::getInstance()->getPath(location));
  static std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textureCache;
  std::string cacheKey = std::to_string(reinterpret_cast<uintptr_t>(renderer.get())) + ":" + path;

  if (auto cachedTexture = textureCache[cacheKey].lock()) {
    return cachedTexture;
  }

  std::string extension = std::filesystem::path(path).extension().string();
  std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

  std::shared_ptr<SDL_Texture> texturePtr;
  if (extension == ".gif") {
    texturePtr = loadAnimatedGifAsSpritesheet(renderer.get(), path);
  }

  if (!texturePtr) {
    SDL_Surface *srf = IMG_Load(path.c_str());
    if (!srf) {
      LOG_ERR("Surface is null " + path + " >>> " + SDL_GetError());
      throw std::invalid_argument("File '" + path + "' could not be loaded");
    }
    texturePtr.reset(SDL_CreateTextureFromSurface(renderer.get(), srf), SDLDeleter());
    SDL_FreeSurface(srf);
  }

  if (!texturePtr) {
    LOG_ERR("Texture is null " + path + " >>> " + SDL_GetError());
    return nullptr;
  }
  LOG_INFO("Loaded texture " + path + " to address: " + std::to_string(reinterpret_cast<uintptr_t>(texturePtr.get())),
           LOG_LEVEL::PRIORITY);
  textureCache[cacheKey] = texturePtr;
  return texturePtr;
}
