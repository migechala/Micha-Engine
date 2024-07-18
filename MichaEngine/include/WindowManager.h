//
// Created by Mikhail Chalakov on 2/18/24.
//

#pragma once
#include <SDL2/SDL.h>

#include "InternalWindow.h"
#include "ResourceLoader.h"
#include "Types.h"
#include "imgui.h"

class WindowManager {
  float cameraX = 0;
  std::shared_ptr<SDL_Window> window;
  std::shared_ptr<SDL_Renderer> renderer;
  std::vector<std::shared_ptr<SDL_Texture>> background;
  SDL_Event event;
  type::Vector2i windowSize;
  static type::Vector2i getMonitorSize();
  InternalWindow *iw;
  bool imgui_open;

  void renderParallex();

 public:
  int frameCount;
  void setBackground(std::shared_ptr<SDL_Texture> bkg);
  void setParallex(std::vector<std::shared_ptr<SDL_Texture>> newBackgrounds,
                   std::vector<float> speeds);
  int draw(SDL_Texture *txt, const SDL_Rect *src, const SDL_Rect *dst);
  int draw(type::Object *object);

  type::Vector2i getAbsolutePosition(type::Vector2i pos);
  type::Vector2i getCenter();
  type::Vector2i getSize();

  SDL_Renderer *getRenderer();

  bool hasQuit();

  void update();

  WindowManager(const std::string &windowName, type::Vector2i pos, Uint32 flag);

  ~WindowManager();
};
