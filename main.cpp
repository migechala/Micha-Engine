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

    windowManager->setParallex(
        ResourceLoader::getInstance()->loadTextures(
            windowManager->getRenderer(), "../assets/background/1.png",
            "../assets/background/2.png", "../assets/background/3.png",
            "../assets/background/4.png", "../assets/background/5.png",
            "../assets/background/6.png", "../assets/background/7.png",
            "../assets/background/8.png", "../assets/background/9.png"),
        {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f});

    type::Vector2i offset = {100, windowManager->getCenter().y};
    mainCharacterID = create_sprite(
        ResourceLoader::getInstance()->loadTextures(
            windowManager->getRenderer(), "../assets/character/jetpack.png",
            "../assets/character/running.png",
            "../assets/character/standing.png"),
        {15, 15}, offset, {692, 599}, {200, 200}, {0, 0}, {0, -1});
    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_SPACE,
        [&]() {
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->acceleration.y = 1;
        },
        true);
    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_D,
        [&]() {
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->position.x += 5;
          ObjectManager::getInstance()->getSprite(mainCharacterID)->flip =
              SDL_FLIP_NONE;
        },
        true);
    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_A,
        [&]() {
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->position.x -= 5;
          ObjectManager::getInstance()->getSprite(mainCharacterID)->flip =
              SDL_FLIP_HORIZONTAL;
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
    if (ObjectManager::getInstance()->getSprite(mainCharacterID)->grounded &&
        (KeyboardManager::getInstance()->isNotPressed(
             SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_SPACE) ||
         (KeyboardManager::getInstance()->isNotPressed(SDL_SCANCODE_SPACE) &&
          KeyboardManager::getInstance()->isPressed(SDL_SCANCODE_A) &&
          KeyboardManager::getInstance()->isPressed(SDL_SCANCODE_D)))) {
      ObjectManager::getInstance()
          ->getSprite(mainCharacterID)
          ->changeSpritesheet(2);
    }
  }
};

int main() {
  std::unique_ptr<Game> game;
  game.reset(new Game());
  start(game.get());
  return 0;
}
