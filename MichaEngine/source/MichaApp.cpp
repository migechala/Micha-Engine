#include "MichaApp.h"

#include "Logger.h"

void MichaApp::mainloop() {
  std::cout << "Hellfdadsf" << std::endl;
  LOG_ERR("MAINLOOP MUST BE SET IN GAME CLASS");
}

MichaApp::MichaApp() {
  CHECK_RESULT(SDL_Init(SDL_INIT_EVERYTHING));
  eng::Vector2i position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};
  windowManager = std::make_shared<WindowManager>("Test Engine", position,
                                                  SDL_WINDOW_SHOWN);
}