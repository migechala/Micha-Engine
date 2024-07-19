#include "Engine.h"

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
    ObjectManager::getInstance()->updateAllObjects(
        execute->windowManager.get());
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

int create_sprite(type::Vector2i position, type::Vector2i size,
                  type::Vector2i velocity, type::Vector2i acceleration,
                  SDL_Color color) {
  return ObjectManager::getInstance()->addObject(
      new type::Object(position, size, velocity, acceleration, color));
}

int create_sprite(std::vector<std::shared_ptr<SDL_Texture>> textures,
                  std::vector<int> numberOfSpritesPerTexture,
                  type::Vector2i position, type::Vector2i spriteSize,
                  type::Vector2i size, type::Vector2i velocity,
                  type::Vector2i acceleration) {
  if (textures.empty()) {
    LOG_ERR("NO TEXTURES FOUND")
    create_sprite(position, size, velocity, acceleration);
  }
  return ObjectManager::getInstance()->addObject(
      new type::Sprite(position, size, velocity, acceleration, textures,
                       numberOfSpritesPerTexture, spriteSize));
}
