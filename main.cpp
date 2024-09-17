#include <random>

#include "Engine.h"

class Game : public ExecutableClass {
  int mainCharacterID;
  eng::SpriteOptions bulletOptions;
  bool characterDead;
  bool changedToEnd = false;
  std::random_device dev;

 public:
  Game() : characterDead(false) {
    Logger::setLogLevel(LOG_LEVEL::PRIORITY);
    maxFPS = 120;
    windowManager->setParallex(
        ResourceLoader::loadTextures(
            windowManager->getRenderer(), "../assets/background/1.png",
            "../assets/background/2.png", "../assets/background/3.png",
            "../assets/background/4.png", "../assets/background/5.png",
            "../assets/background/6.png", "../assets/background/7.png",
            "../assets/background/8.png", "../assets/background/9.png"),
        {-0.9f, -0.2f, -0.3f, -0.4f, -0.5f, -0.6f, -0.7f, -0.8f, -0.9f});
    eng::Vector2i start = {windowManager->getSize().x,
                           windowManager->getSize().y / 2};
    bulletOptions
        .setTextures(ResourceLoader::loadTextures(
            windowManager->getRenderer(), "../assets/projectiles/1.png"))
        .setNumberOfSpritesPerSheet({1})
        .setRealSpriteSize({249, 144})
        .setPosition(start)
        .setSize({249, 144})
        .setVelocity({-10, 0})
        .setFlip(SDL_FLIP_HORIZONTAL);

    eng::Vector2i offset = {100, 0};

    eng::SpriteOptions mainCharacterOptions;
    mainCharacterOptions
        .setTextures(ResourceLoader::loadTextures(
            windowManager->getRenderer(), "../assets/character/jetpack.png",
            "../assets/character/running.png",
            "../assets/character/standing.png", "../assets/character/die.png"))
        .setNumberOfSpritesPerSheet({15, 15, 15, 4})
        .setFramesPerTextureUpdate(1)
        .setRealSpriteSize({692, 599})
        .setPosition(offset)
        .setSize({200, 200})
        .enableGravity();
    mainCharacterID = create_sprite(mainCharacterOptions);
    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_SPACE,
        [&]() {
          if (!characterDead) {
            ObjectManager::getInstance()
                ->getSprite(mainCharacterID)
                ->setAcceleration({0, 1});
          }
        },
        true);
    windowManager->getInternalWindow()->showFPS();
  }
  int new_random(int lower, int upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(static_cast<unsigned long>(
        std::chrono::system_clock::now().time_since_epoch().count()));

    std::uniform_int_distribution<> distrib(lower, upper);
    return distrib(gen);
  }

 public:
  void mainloop() override {
    LOG_INFO("Running mainloop", LOG_LEVEL::LOW);
    static int projFrame = 0;
    if (!characterDead) {
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
      if (projFrame == 50) {
        projFrame = 0;
        create_sprite(bulletOptions);
        bulletOptions.setPosition(
            {bulletOptions.getPosition().x,
             new_random(10, windowManager->getSize().y - 10)});
      }
    } else {
      if (!changedToEnd) {
        windowManager->setBackground(ResourceLoader::loadTexture(
            windowManager->getRenderer(), "../assets/background/end.png"));
      }
      changedToEnd = true;
    }

    for (int i = mainCharacterID + 1;
         i < ObjectManager::getInstance()->getNumObjects(); i++) {
      auto obj = ObjectManager::getInstance()->getObject(i);
      if (obj && obj->getPosition().x + obj->getVelocity().x <= 0) {
        ObjectManager::getInstance()->removeObject(i);
      }
    }
    if (ObjectManager::getInstance()->getNumObjects() > 1) {
      for (int i = 1; i < ObjectManager::getInstance()->getNumObjects(); i++) {
        if (!characterDead &&
            ObjectManager::getInstance()->collide(mainCharacterID, i)) {
          characterDead = true;
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->changeSpritesheet(3);
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->setFramesPerUpdate(15);
          ObjectManager::getInstance()
              ->getSprite(mainCharacterID)
              ->toggleUpdate();

          LOG_INFO("Collided", LOG_LEVEL::PRIORITY);
        }
      }
    }
    projFrame++;
  }
};

int main() {
  std::unique_ptr<Game> game = std::make_unique<Game>();
  start(game.get());
  return 0;
}
