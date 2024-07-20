#pragma once
#include <SDL2/SDL.h>

#include <iostream>

#include "imgui.h"

class InternalWindow {
  bool show_debug = true;  // set thi
  std::shared_ptr<SDL_Renderer> renderer;
  ImGuiIO *io;
  bool show_fps = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

 public:
  InternalWindow(std::shared_ptr<SDL_Renderer> rendererToAttatch,
                 std::shared_ptr<SDL_Window> windowToAttatch);
  int update();
  void showFPS();
  ~InternalWindow();
};