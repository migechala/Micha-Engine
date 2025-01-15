#include "Engine.h"

#include "imgui_impl_sdl2.h"

void start(MichaApp* execute) {
  LOG_INFO("Beginning Program...", LOG_LEVEL::PRIORITY);
  LOG_INFO("Max FPS Set to " + std::to_string(execute->maxFPS),
           LOG_LEVEL::MEDIUM);
  //
  int frame = 0;
  bool done = false;
  while (!done) {
    Uint32 start = SDL_GetPerformanceCounter();
    execute->mainloop();
    // Constant Game Loop
    KeyboardManager::getInstance()->update();

    ObjectManager::getInstance()->updateAllObjects(frame);
    execute->windowManager->update();
    if (execute->windowManager->hasQuit()) {
      done = true;
    }
    // Limit fps
    Uint32 end = SDL_GetPerformanceCounter();
    float elapsedMS =
        (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(floor(1000.00f / execute->maxFPS - elapsedMS));
    frame++;
    if (frame == execute->maxFPS) frame = 0;
  }

  LOG_INFO("Program Finished.", LOG_LEVEL::PRIORITY);
}

int create_object(eng::ObjectOptions& options) {
  return ObjectManager::getInstance()->addObject(
      std::make_shared<eng::Object>(options));
}

int create_sprite(eng::SpriteOptions& options) {
  if (options.getTextures().empty()) {
    LOG_ERR("NO TEXTURES FOUND")
  }
  auto obj = std::make_shared<eng::Sprite>(options);

  return ObjectManager::getInstance()->addObject(obj);
}
