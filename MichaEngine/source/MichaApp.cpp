#include "MichaApp.h"
#include "Logger.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void MichaApp::mainloop() { LOG_ERR("MAINLOOP MUST BE SET IN GAME CLASS"); }

MichaApp::MichaApp() {
  CHECK_RESULT(SDL_Init(SDL_INIT_EVERYTHING));
  CHECK_RESULT(TTF_Init());
  eng::Vector2<int> position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};
  windowManager = std::make_shared<WindowManager>("Test Engine", position, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
}
