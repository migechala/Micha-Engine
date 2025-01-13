#pragma once

#include <SDL2/SDL.h>

#include <iostream>

#include "imgui.h"

class InternalWindow {
 private:
  bool show_debug = true;  // Flag to control whether debug info is shown
  std::shared_ptr<SDL_Renderer> renderer;  // Renderer for the window
  ImGuiIO* io;                             // ImGui input/output interface
  bool show_fps = false;  // Flag to control whether FPS is shown
  ImVec4 clear_color =
      ImVec4(0.45f, 0.55f, 0.60f, 1.00f);  // Clear color for the window

 public:
  // Constructor: Initializes the internal window with the renderer and window
  InternalWindow(std::shared_ptr<SDL_Renderer> rendererToAttach,
                 std::shared_ptr<SDL_Window> windowToAttach);

  // Updates the internal window (called every frame)
  int update();

  // Displays the FPS counter in the window
  void showFPS();

  // Destructor: Cleans up the resources when the window is destroyed
  ~InternalWindow();
};
