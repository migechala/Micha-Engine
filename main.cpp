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
        ResourceLoader::loadTextures(
            windowManager->getRenderer(), "../assets/background/1.png",
            "../assets/background/2.png", "../assets/background/3.png",
            "../assets/background/4.png", "../assets/background/5.png",
            "../assets/background/6.png", "../assets/background/7.png",
            "../assets/background/8.png", "../assets/background/9.png"),
        {0.9f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f});

    type::Vector2i offset = {100, 0};
    auto mainCharacter = create_object(ResourceLoader::loadTextures(
                                           windowManager->getRenderer(),
                                           "../assets/character/jetpack.png",
                                           "../assets/character/running.png",
                                           "../assets/character/standing.png"),
                                       {15, 15, 15}, {692, 599})
                             ->setPosition(offset)
                             ->setSize({200, 200})
                             ->gravityOn();

    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_SPACE,
        [&]() {
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->setAcceleration({0, 1});
        },
        true);
    windowManager->getInternalWindow()->showFPS();
  }

 public:
  void mainloop() override {
    LOG_INFO("Running mainloop", LOG_LEVEL::LOW);
    static int projFrame = 0;
    if (ObjectManager::getInstance()
            ->getSprite(mainCharacterID)
            ->isGrounded()) {
      ObjectManager::getInstance()
          ->getSprite(mainCharacterID)
          ->changeSpritesheet(1);
    } else {
      ObjectManager::getInstance()
          ->getSprite(mainCharacterID)
          ->changeSpritesheet(0);
    }
    if (projFrame == 100) {
      projFrame = 0;
      type::Vector2i start = {windowManager->getSize().x,
                              windowManager->getSize().y / 2};
      LOG_INFO("Spawning Projectile", LOG_LEVEL::PRIORITY);
      create_object(ResourceLoader::loadTextures(windowManager->getRenderer(),
                                                 "../assets/projectiles/1.png"),
                    {1}, {249, 144})
          ->setPosition(start)
          ->setSize({249, 144})
          ->setVelocity({-10, 0})
          ->setFlip(SDL_FLIP_HORIZONTAL);
    }
    for (int i = mainCharacterID + 1;
         i < ObjectManager::getInstance()->getNumObjects(); i++) {
      auto obj = ObjectManager::getInstance()->getObject(i);
      if (obj && obj->getPosition().x + obj->getVelocity().x <= 0) {
        ObjectManager::getInstance()->removeObject(i);
      }
    }
    projFrame++;
  }
};

int main() {
  std::unique_ptr<Game> game;
  game.reset(new Game());
  start(game.get());
  return 0;
}
