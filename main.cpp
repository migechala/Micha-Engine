#include <random>

#include "Engine.h"
#include "FileManager.h"

class Game : public MichaApp {
private:
  int mainCharacterID;
  eng::SpriteOptions bulletOptions;
  bool characterDead;
  bool changedToEnd = false;
  std::random_device dev;
  static constexpr int bulletSpawnFrameInterval = 50;

public:
  Game() : characterDead(false) {
    windowManager->debugDraw = true;
    FileManager::getInstance()->readSettings("../assets/settings.ini");
    Logger::setLogLevel(LOG_LEVEL::PRIORITY);
    maxFPS = 120;
    windowManager->setSize({std::stoi(FileManager::getInstance()->getSettings("height")),
                            std::stoi(FileManager::getInstance()->getSettings("width"))});
    ObjectManager::getInstance()->updateFrameSize(windowManager->getSize());
    windowManager->getInternalWindow()->show_debug = true;
    windowManager->setParallex(ResourceLoader::loadTextures(windowManager->getRenderer(), "../assets/background/1.png",
                                                            "../assets/background/2.png", "../assets/background/3.png",
                                                            "../assets/background/4.png", "../assets/background/5.png",
                                                            "../assets/background/6.png", "../assets/background/7.png",
                                                            "../assets/background/8.png", "../assets/background/9.png"),
                               {-0.9f, -0.2f, -0.3f, -0.4f, -0.5f, -0.6f, -0.7f, -0.8f, -0.9f});
    bulletOptions.setTextures(ResourceLoader::loadTextures(windowManager->getRenderer(), "../assets/projectiles/1.png"))
        .setNumberOfSpritesPerSheet({1})
        .setRealSpriteSize({249, 144})
        .setPosition({windowManager->getSize().x, windowManager->getSize().y / 2})
        .setSize({249, 144})
        .setHitbox({20, 50})
        .setHitboxOffset({-50, 50})
        .setVelocity({-10, 0})
        .setFlip(SDL_FLIP_HORIZONTAL);
    eng::SpriteOptions mainCharacterOptions;
    mainCharacterOptions
        .setTextures(ResourceLoader::loadTextures(windowManager->getRenderer(), "../assets/character/jetpack.png",
                                                  "../assets/character/running.png", "../assets/character/standing.png",
                                                  "../assets/character/die.png"))
        .setNumberOfSpritesPerSheet({15, 15, 15, 4})
        .setFramesPerTextureUpdate(1)
        .setRealSpriteSize({692, 599})
        .setPosition({100, 0})
        .setSize({200, 200})
        .setHitbox({120, 200})
        .enableGravity();

    mainCharacterID = create_sprite(mainCharacterOptions);
    KeyboardManager::getInstance()->addListener(
        SDL_SCANCODE_SPACE,
        [&]() {
          if (!characterDead) {
            ObjectManager::getInstance()->getSprite(mainCharacterID)->setAcceleration({0, 1});
          }
        },
        true);
    windowManager->getInternalWindow()->showFPS();
  }

  void mainloop() override {
    LOG_INFO("Running mainloop", LOG_LEVEL::LOW);
    static int projFrame = 0;

    if (!characterDead) {
      handleCharacterAnimation();
      handleBulletSpawning(projFrame);
    } else if (!changedToEnd) {
      transitionToEndScreen();
    }

    cleanupOffScreenObjects();
    detectCollisions();

    projFrame++;
  }

private:
  void handleCharacterAnimation() {
    auto character = ObjectManager::getInstance()->getSprite(mainCharacterID);
    if (character->isGrounded()) {
      character->changeSpritesheet(1);
    } else {
      character->changeSpritesheet(0);
    }
  }

  void handleBulletSpawning(int &projFrame) {
    if (projFrame >= bulletSpawnFrameInterval) {
      projFrame = 0;
      bulletOptions.setPosition({bulletOptions.getPosition().x, newRandom(10, windowManager->getSize().y - 10)})
          .setVelocity({newRandom(-20, -1), 0});
      create_sprite(bulletOptions);
    }
  }

  void transitionToEndScreen() {
    windowManager->setBackground(
        ResourceLoader::loadTexture(windowManager->getRenderer(), "../assets/background/end.png"));
    changedToEnd = true;
  }

  void cleanupOffScreenObjects() {
    for (int i = mainCharacterID + 1; i < ObjectManager::getInstance()->getNumObjects(); i++) {
      auto obj = ObjectManager::getInstance()->getSprite(i);
      if (obj && obj->getOptions().getPosition().x + obj->getOptions().getVelocity().x <= 0) {
        ObjectManager::getInstance()->removeSprite(i);
      }
    }
  }

  void detectCollisions() {
    for (int i = 1; i < ObjectManager::getInstance()->getNumObjects(); i++) {
      if (!characterDead && ObjectManager::getInstance()->doesExist(i) &&
          ObjectManager::getInstance()->collide(mainCharacterID, i)) {
        handleCollision(i);
        break;
      }
    }
  }

  void handleCollision(int collidingObjectID) {
    characterDead = true;
    auto character = ObjectManager::getInstance()->getSprite(mainCharacterID);
    character->changeSpritesheet(3);
    character->setFramesPerUpdate(15);
    character->toggleUpdate();

    for (int j = 1; j < ObjectManager::getInstance()->getNumObjects(); j++) {
      if (j != collidingObjectID) {
        ObjectManager::getInstance()->removeSprite(j);
      }
    }

    ObjectManager::getInstance()->getSprite(collidingObjectID)->setVelocity({0, 0});
  }

  int newRandom(int lower, int upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> distrib(lower, upper);
    return distrib(gen);
  }
};

int main() {
  std::unique_ptr<Game> game = std::make_unique<Game>();
  start(game.get());
  return 0;
}
