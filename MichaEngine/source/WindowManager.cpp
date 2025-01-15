//
// Created by Mikhail Chalakov on 2/18/24.
//
#include "WindowManager.h"

#include <iostream>
#include <string>

#include "InternalWindow.h"
#include "Logger.h"
#include "ObjectManager.h"
#include "ResourceLoader.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDLDeleter.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

bool WindowManager::hasQuit() { return quit; }

eng::Vector2i WindowManager::getMonitorSize() {
  SDL_DisplayMode t_dm;
  if (SDL_GetCurrentDisplayMode(0, &t_dm) != 0) {
    std::string err = "DP error: ";
    LOG_ERR(err + SDL_GetError())
    exit(-1);
  }
  LOG_INFO(std::to_string(t_dm.w), LOG_LEVEL::LOW)
  return {t_dm.w, t_dm.h};
}

void WindowManager::renderParallex() {
  for (size_t i = 0; i < background.size(); ++i) {
    float offset = frameCount * backgroundSpeeds[i];

    SDL_Rect destRect;
    destRect.x = static_cast<int>(offset) % windowSize.x;
    destRect.y = 0;
    destRect.w = windowSize.x;
    destRect.h = windowSize.y;

    SDL_RenderCopy(renderer.get(), background[i].get(), NULL, &destRect);

    if (destRect.x < 0) {
      destRect.x += windowSize.x;
      SDL_RenderCopy(renderer.get(), background[i].get(), NULL, &destRect);
    }
  }
}

int WindowManager::draw(SDL_Texture *txt, const SDL_Rect *src,
                        const SDL_Rect *dst) {
  return SDL_RenderCopy(renderer.get(), txt, src, dst);
}
eng::Vector2i WindowManager::getCenter() { return getSize() / 2; }
eng::Vector2i WindowManager::getSize() { return windowSize; }

std::shared_ptr<SDL_Renderer> WindowManager::getRenderer() { return renderer; }

std::shared_ptr<SDL_Window> WindowManager::getWindow() { return window; }

std::shared_ptr<InternalWindow> WindowManager::getInternalWindow() {
  return internalWindow;
}

eng::Vector2i WindowManager::getAbsolutePosition(eng::Vector2i position) {
  return {position.x, (windowSize.y - position.y)};
}
int WindowManager::draw(std::shared_ptr<eng::Object> object) {
  if (!object) return -1;
  SDL_Color oldColor;
  SDL_Rect dst;
  eng::Vector2i pos = getAbsolutePosition(object->getPosition());
  dst.x = pos.x - object->getSize().x / 2;
  dst.y = pos.y - object->getSize().y;
  dst.w = object->getSize().x;
  dst.h = object->getSize().y;

  if (object->isSprite()) {
    auto sprite = std::reinterpret_pointer_cast<eng::Sprite>(object);
    LOG_INFO("Rendering Texture", LOG_LEVEL::LOW)
    return SDL_RenderCopyEx(renderer.get(), sprite->getTexture().get(),
                            &sprite->getSrc(), &dst, sprite->getAngle(), NULL,
                            sprite->getFlip());
    return SDL_RenderCopy(renderer.get(), sprite->getTexture().get(),
                          &sprite->getSrc(), &dst);
  }

  SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g, &oldColor.b,
                         &oldColor.a);
  SDL_SetRenderDrawColor(renderer.get(), object->getColor().r,
                         object->getColor().g, object->getColor().b,
                         object->getColor().a);
  int ret = SDL_RenderFillRect(renderer.get(), &dst);
  SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                         oldColor.a);
  return ret;
}

void WindowManager::update() {
  SDL_Color oldColor;
  SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g, &oldColor.b,
                         &oldColor.a);
  SDL_SetRenderDrawColor(renderer.get(), 128, 128, 128, 255);
  int ret = SDL_RenderFillRect(renderer.get(), NULL);
  SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                         oldColor.a);
  if (!background.empty()) {
    if (background.size() == 1) {
      SDL_RenderCopy(renderer.get(), background[0].get(), NULL, NULL);
    } else {
      renderParallex();
    }
  }
  for (int i = 0; i < ObjectManager::getInstance()->getNumObjects(); ++i) {
    auto curObject = ObjectManager::getInstance()->getObject(i);
    if (!curObject) continue;

    LOG_INFO("Drawing object with id: " + std::to_string(i), LOG_LEVEL::LOW)
    if (draw(curObject) != 0) {
      LOG_ERR(SDL_GetError());
      exit(-1);
    }
  }
  ++frameCount;
  //
  SDL_Event e;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    if (event.type == SDL_QUIT) {
      quit = true;
    }
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
      }
    }
  }
  internalWindow->update();

  SDL_RenderPresent(renderer.get());
}
void WindowManager::setSize(eng::Vector2i newSize) {
  windowSize = newSize;
  SDL_SetWindowSize(window.get(), newSize.x, newSize.y);
}
void WindowManager::setBackground(std::shared_ptr<SDL_Texture> bkg) {
  background.clear();
  background.push_back(bkg);
}

void WindowManager::setParallex(
    std::vector<std::shared_ptr<SDL_Texture>> newBackgrounds,
    std::vector<float> speeds) {
  if (newBackgrounds.size() != speeds.size()) {
    LOG_ERR("Mismatch between number of parallax layers and speeds");
    return;
  }
  background = newBackgrounds;
  backgroundSpeeds = speeds;
}

void WindowManager::setTiles(std::string file_path,
                             std::vector<std::vector<int>> tiles,
                             eng::Vector2i size) {
  auto tilesImage = ResourceLoader::loadTexture(getRenderer(), file_path);
}

WindowManager::~WindowManager() {
  SDL_DestroyRenderer(renderer.get());
  SDL_DestroyWindow(window.get());
  SDL_Quit();
}

WindowManager::WindowManager(const std::string &windowName, eng::Vector2i pos,
                             Uint32 flag)
    : frameCount(0), windowSize(getMonitorSize()), quit(false) {
  LOG_INFO("Created Window", LOG_LEVEL::MEDIUM);
  windowSize.y -= 100;
  window.reset(SDL_CreateWindow(windowName.c_str(), pos.x, pos.y, windowSize.x,
                                windowSize.y, flag),
               SDLDeleter());

  if (!window) {
    LOG_ERR("Error! Window is null");
  }
  renderer.reset(
      SDL_CreateRenderer(window.get(), -1,
                         SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED),
      SDLDeleter());

  if (!renderer) {
    std::string err = "Renderer error: ";
    LOG_ERR(err + SDL_GetError());
    exit(-1);
  }

  internalWindow =
      std::make_unique<InternalWindow>(InternalWindow(renderer, window));

  SDL_Surface *color = SDL_CreateRGBSurfaceWithFormat(
      0, 100, 100, 8, SDL_PixelFormatEnum::SDL_PIXELFORMAT_BGR24);
  if (!color) {
    std::string err = "Color error: ";
    LOG_ERR(err + SDL_GetError());
    exit(-1);
  }
}
