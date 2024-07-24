#include "Engine.h"

#include "imgui_impl_sdl2.h"

void start(ExecutableClass* execute) {
  LOG_INFO("Beginning Program...", LOG_LEVEL::PRIORITY);
  LOG_INFO("Max FPS Set to " + std::to_string(execute->maxFPS),
           LOG_LEVEL::MEDIUM);
  //
  bool done = false;
  while (!done) {
    Uint32 start = SDL_GetPerformanceCounter();
    execute->mainloop();
    // Constant Game Loop
    KeyboardManager::getInstance()->update();

    ObjectManager::getInstance()->updateAllObjects(execute->windowManager);
    execute->windowManager->update();
    if (execute->windowManager->hasQuit()) {
      done = true;
    }
    // Limit fps
    Uint32 end = SDL_GetPerformanceCounter();
    float elapsedMS =
        (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(floor(1000.00f / execute->maxFPS - elapsedMS));
  }

  LOG_INFO("Program Finished.", LOG_LEVEL::PRIORITY);
}

std::shared_ptr<type::Object> create_object() {
  return ObjectManager::getInstance()->addObject(new type::Object());
}

std::shared_ptr<type::Object> create_object(
    std::vector<std::shared_ptr<SDL_Texture>> textures,
    std::vector<int> numberOfSpritesPerTexture, type::Vector2i spriteSize) {
  if (textures.empty()) {
    LOG_ERR("NO TEXTURES FOUND")
    return create_object();
  }

  return ObjectManager::getInstance()->addObject(
      new type::Sprite(textures, numberOfSpritesPerTexture, spriteSize));
}
