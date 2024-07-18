#include "Engine.h"

class Game : public ExecutableClass {
  int mainCharacterID;

 public:
  Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    Logger::setLogLevel(LOG_LEVEL::PRIORITY);
    maxFPS = 20;

    windowManager.reset(new WindowManager(
        "Test Engine", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WINDOW_SHOWN));

    // windowManager->setBackground(ResourceLoader::getInstance()->loadTexture(
    // windowManager->getRenderer(), "../assets/background/9.png"));

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
            windowManager->getRenderer(), "../assets/character/man.png"),
        offset, {32, 32}, {200, 200}, {0, 0}, {0, -1});

    ObjectManager::getInstance()
        ->getSprite(mainCharacterID)
        ->getCutOuts()
        ->pop_back();

    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_SPACE,
        [&]() {
          if (ObjectManager::getInstance()
                  ->getSprite(mainCharacterID)
                  ->grounded) {
            ObjectManager::getInstance()
                ->getSprite(mainCharacterID)
                ->velocity.y = 20;
          }
        },
        false);
  }

 public:
  void mainloop() override {
    LOG_INFO("Running mainloop", LOG_LEVEL::LOW);
    // if (ObjectManager::getInstance()->getSprite(mainCharacterID)->grounded) {
    //   ObjectManager::getInstance()
    //       ->getSprite(mainCharacterID)
    //       ->changeSpritesheet(1);
    // } else {
    //   ObjectManager::getInstance()
    //       ->getSprite(mainCharacterID)
    //       ->changeSpritesheet(0);
    // }
  }
};

int main() {
  std::unique_ptr<Game> game;
  game.reset(new Game());
  start(game.get());
  return 0;
}
