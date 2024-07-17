#pragma once
#include <SDL2/SDL.h>

#include <iostream>

#include "imgui.h"

class InternalWindow {
  std::shared_ptr<SDL_Renderer> renderer;
  bool show_demo_window = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

 public:
  InternalWindow(std::shared_ptr<SDL_Renderer> rendererToAttatch);
  int update();
  ~InternalWindow();
};