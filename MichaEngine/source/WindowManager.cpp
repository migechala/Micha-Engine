//
// Created by Mikhail Chalakov on 2/18/24.
//
#include "WindowManager.h"

#include <string>

#include "InternalWindow.h"
#include "Logger.h"
#include "ObjectManager.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

bool WindowManager::hasQuit() {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return true;
    }
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        return true;
      }
    }
  }
  return false;
}

type::Vector2i WindowManager::getMonitorSize() {
  SDL_DisplayMode t_dm;
  if (SDL_GetCurrentDisplayMode(0, &t_dm) != 0) {
    std::string err = "DP error: ";
    LOG_ERR(err + SDL_GetError())
    exit(-1);
  }
  LOG_INFO(std::to_string(t_dm.w), LOG_LEVEL::LOW)
  return {t_dm.w, t_dm.h};
}

int WindowManager::draw(SDL_Texture *txt, const SDL_Rect *src,
                        const SDL_Rect *dst) {
  return SDL_RenderCopy(renderer.get(), txt, src, dst);
}

type::Vector2i WindowManager::getAbsolutePosition(type::Vector2i position) {
  int h;
  SDL_GetWindowSize(window.get(), &h, nullptr);
  return {position.x, h - position.y};
}
type::Vector2i WindowManager::getCenter() { return windowSize / 2; }
type::Vector2i WindowManager::getSize() { return windowSize; }
int WindowManager::draw(type::Object *object) {
  SDL_Color oldColor;

  type::Vector2i pos = getAbsolutePosition(object->position);
  object->dst.x = pos.x;
  object->dst.y = pos.y;
  LOG_INFO(std::to_string(object->dst.x), LOG_LEVEL::LOW);
  LOG_INFO(std::to_string(object->dst.y), LOG_LEVEL::LOW);

  SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g, &oldColor.b,
                         &oldColor.a);
  SDL_SetRenderDrawColor(renderer.get(), object->color.r, object->color.g,
                         object->color.b, object->color.a);
  int ret = SDL_RenderFillRect(renderer.get(), &object->dst);
  SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                         oldColor.a);
  return ret;
}

int WindowManager::draw(type::Sprite *sprite) {
  type::Vector2i pos = getAbsolutePosition(sprite->getPosition());
  SDL_Rect realPosition = SDL_Rect({pos.x, pos.y});
  if (sprite->getTexture()) {
    return SDL_RenderCopy(renderer.get(), sprite->getTexture(), nullptr,
                          &realPosition);
  }
  SDL_Color oldColor;
  SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g, &oldColor.b,
                         &oldColor.a);
  SDL_SetRenderDrawColor(renderer.get(), 128, 128, 128, 255);
  int ret = SDL_RenderFillRect(renderer.get(), &realPosition);
  SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                         oldColor.a);
  return ret;
}

void WindowManager::update() {
  if (background) {
    if (draw(background.get(), nullptr, nullptr) != 0) {
      std::string err = "Draw BG error: ";
      LOG_ERR(err + SDL_GetError());
      exit(-1);
    }
  } else {
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g,
                           &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(renderer.get(), 128, 128, 128, 255);
    int ret = SDL_RenderFillRect(renderer.get(), NULL);
    SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                           oldColor.a);
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
  // iw->update();

  SDL_RenderPresent(renderer.get());
}
void WindowManager::setBackground(SDL_Texture *bkg) { background.reset(bkg); }

WindowManager::~WindowManager() {
  SDL_DestroyRenderer(renderer.get());
  SDL_DestroyWindow(window.get());
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_Quit();
}

WindowManager::WindowManager(const std::string &windowName, type::Vector2i pos,
                             Uint32 flag)
    : frameCount(0), windowSize(getMonitorSize()), imgui_open(true) {
  LOG_INFO("Created Window", LOG_LEVEL::MEDIUM);

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
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)&io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  SDL_RenderSetScale(renderer.get(), io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
  ImGui_ImplSDLRenderer2_Init(renderer.get());
  iw = new InternalWindow(renderer);
  SDL_Surface *color = SDL_CreateRGBSurfaceWithFormat(
      0, 100, 100, 8, SDL_PixelFormatEnum::SDL_PIXELFORMAT_BGR24);
  if (!color) {
    std::string err = "Color error: ";
    LOG_ERR(err + SDL_GetError());
    exit(-1);
  }
}
