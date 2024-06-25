#pragma once
#include <string>
#include <vector>

#include "Executable.h"
#include "KeyboardManager.h"
#include "Logger.h"
#include "ObjectManager.h"
#include "Types.h"
#include "WindowManager.h"

void start(ExecutableClass *execute) {
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
    ObjectManager::getInstance()->updateAllObjects();
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

int create_sprite(type::Vector2i position, type::Vector2i size) {
  return ObjectManager::getInstance()->addObject(
      new type::Sprite(position, size));
}

int create_sprite_with_velocity(type::Vector2i position, type::Vector2i size,
                                type::Vector2i velocity) {
  return ObjectManager::getInstance()->addObject(
      new type::Sprite(position, size, velocity));
}