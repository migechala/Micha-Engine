#include <Engine.h>

#include <iostream>

class Game : public ExecutableClass {
 public:
  Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    Logger::setLogLevel(LOG_LEVEL::PRIORITY);
    maxFPS = 120;

    windowManager.reset(new WindowManager(
        "Test Engine", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI));

    type::Vector2i offset = windowManager->getCenter();
    int mainCharacterID = create_sprite(offset, {100, 100}, {0, 0}, {0, -1});

    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_SPACE,
        [mainCharacterID]() {
          ObjectManager::getInstance()
              ->getObject(mainCharacterID)
              ->acceleration.y = 1;
        },
        true);
  }

 public:
  void mainloop() override { LOG_INFO("Running mainloop", LOG_LEVEL::LOW) }
};

int main() {
  std::unique_ptr<Game> game;
  game.reset(new Game());
  start(game.get());
  return 0;
}
