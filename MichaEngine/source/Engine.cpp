#include "Engine.h"

#include "imgui_impl_sdl2.h"

/**
 * Handle the main loop of the Engine and execute the provided MichaApp.
 * \param execute The MichaApp instance to run.
 */
void start(MichaApp *execute) {
  LOG_INFO("Beginning Program...", LOG_LEVEL::PRIORITY);
  LOG_INFO("Max FPS Set to " + std::to_string(execute->maxFPS), LOG_LEVEL::MEDIUM);
  //
  int frame = 0;
  bool done = false;
  while (!done) {
    Uint32 start = SDL_GetPerformanceCounter();
    execute->mainloop();
    // Constant Game Loop
    KeyboardManager::getInstance()->update();

    ObjectManager::getInstance()->updateAllSprites(frame);
    execute->windowManager->update();
    if (execute->windowManager->hasQuit()) {
      done = true;
    }
    // Limit fps
    Uint32 end = SDL_GetPerformanceCounter();
    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(floor(1000.00f / execute->maxFPS - elapsedMS));
    frame++;
    if (frame == execute->maxFPS)
      frame = 0;
  }

  LOG_INFO("Program Finished.", LOG_LEVEL::PRIORITY);
}

/**
 * Create a sprite with the provided options and add it to the ObjectManager.
 * \param options The SpriteOptions to create the sprite with.
 * \return The ID of the created sprite.
 */
int create_sprite(eng::SpriteOptions &options) {
  return ObjectManager::getInstance()->addSprite(std::make_shared<eng::Sprite>(options));
}

/**
 * Get the current time in milliseconds since the SDL library was initialized.
 * \return The current time in milliseconds.
 */
int getTime() { return SDL_GetTicks(); }