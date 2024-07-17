#include "InternalWindow.h"

#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

InternalWindow::InternalWindow(
    std::shared_ptr<SDL_Renderer> rendererToAttatch) {
  renderer.swap(rendererToAttatch);
}

int InternalWindow::update() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
  }
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
  ImGui::Render();

  SDL_SetRenderDrawColor(renderer.get(), (Uint8)(clear_color.x * 255),
                         (Uint8)(clear_color.y * 255),
                         (Uint8)(clear_color.z * 255),
                         (Uint8)(clear_color.w * 255));
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.get());
  return 0;
}

InternalWindow::~InternalWindow() {}
