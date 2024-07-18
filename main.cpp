#include "Engine.h"

class Game : public ExecutableClass {
  int mainCharacterID;

 public:
  Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    Logger::setLogLevel(LOG_LEVEL::PRIORITY);
    maxFPS = 120;

    windowManager.reset(new WindowManager(
        "Test Engine", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WINDOW_SHOWN));

    type::Vector2i offset = {100, windowManager->getCenter().y};

    mainCharacterID = create_sprite(
        loadTextures("../assets/flying.png", "../assets/running.png"), offset,
        {692, 599}, {200, 200}, {0, 0}, {0, -1});

    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_SPACE,
        [&]() {
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->acceleration.y = 1;
        },
        true);
  }

 public:
  void mainloop() override {
    LOG_INFO("Running mainloop", LOG_LEVEL::LOW);
    if (ObjectManager::getInstance()->getSprite(mainCharacterID)->grounded) {
      ObjectManager::getInstance()
          ->getSprite(mainCharacterID)
          ->changeSpritesheet(1);
    } else {
      ObjectManager::getInstance()
          ->getSprite(mainCharacterID)
          ->changeSpritesheet(0);
    }
  }
};

int main() {
  std::unique_ptr<Game> game;
  game.reset(new Game());
  start(game.get());
  return 0;
}
