#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Engine.h"
#include "FileManager.h"

class PlatformerGame : public MichaApp {
private:
  static constexpr int kViewportWidth = 1536;
  static constexpr int kViewportHeight = 864;
  static constexpr int kTileSize = 64;
  static constexpr float kMoveSpeed = 5.0f;
  static constexpr float kJumpVelocity = 17.5f;
  static constexpr float kWallJumpHorizontalVelocity = 9.0f;
  static constexpr int kWallJumpInputLockFrames = 10;

  struct TileDefinition {
    eng::Vector2<int> source;
    bool solid;
  };

  int playerID = -1;
  SDL_RendererFlip playerFacing = SDL_FLIP_NONE;
  bool playerMoving = false;
  bool jumpPressedLastFrame = false;
  int wallJumpInputLockFrames = 0;
  int playerAnimationState = -1;

public:
  PlatformerGame() {
    Logger::setLogLevel(LOG_LEVEL::PRIORITY);
    maxFPS = 60;

    windowManager->setSize({kViewportWidth, kViewportHeight});
    windowManager->debugDraw = false;
    ObjectManager::getInstance()->updateFrameSize({kViewportWidth, kViewportHeight});
    ObjectManager::getInstance()->setGravity(0.8f);

    windowManager->setCameraParallax(ResourceLoader::loadTextures(windowManager->getRenderer(),
                                                                  "assets/jungle/parallax background/plx-1.png",
                                                                  "assets/jungle/parallax background/plx-2.png",
                                                                  "assets/jungle/parallax background/plx-3.png",
                                                                  "assets/jungle/parallax background/plx-4.png",
                                                                  "assets/jungle/parallax background/plx-5.png"),
                                     {0.05f, 0.12f, 0.2f, 0.35f, 0.5f});

    createLevel();
    createPlayer();
    windowManager->getInternalWindow()->showFPS();
    KeyboardManager::getInstance()->addListener({SDL_SCANCODE_F3}, [this]() { windowManager->debugDraw = !windowManager->debugDraw; });
  }

  void mainloop() override {
    auto player = ObjectManager::getInstance()->getSprite(playerID);
    if (!player) {
      return;
    }

    handleMovement(player);
    updateCamera(player);
  }

private:
  void createPlayer() {
    eng::SpriteOptions playerOptions;
    playerOptions
        .setTextures(ResourceLoader::loadTextures(windowManager->getRenderer(),
                                                  "assets/jungle/Character with outline/sprites/idle outline.gif",
                                                  "assets/jungle/Character with outline/sprites/run outline.gif",
                                                  "assets/jungle/Character with outline/sprites/jump outline.png",
                                                  "assets/jungle/Character with outline/sprites/mid air outline.gif"))
        .setNumberOfSpritesPerSheet({12, 8, 1, 2})
        .setFramesPerTextureUpdate(6)
        .setRealSpriteSize({23, 37})
        .setPosition({560, 220})
        .setSize({69, 111})
        .setHitbox({42, 104})
        .setHitboxOffset({0, 0})
        .enableGravity();

    playerID = create_sprite(playerOptions);
  }

  void createLevel() {
    auto tileTexture =
        ResourceLoader::loadTextures(windowManager->getRenderer(), "assets/jungle/jungle tileset/jungle tileset.png");

    std::ifstream mapFile(FileManager::getPath("assets/jungle/maps/platformer.csv"));
    if (!mapFile.is_open()) {
      LOG_ERR("Could not open jungle platformer map CSV");
      return;
    }

    std::vector<std::vector<std::string>> rows;
    for (std::string line; std::getline(mapFile, line);) {
      std::vector<std::string> row;
      std::stringstream stream(line);
      for (std::string token; std::getline(stream, token, ',');) {
        row.push_back(token);
      }
      rows.push_back(row);
    }

    int maxColumns = 0;
    for (const auto &row : rows) {
      maxColumns = std::max(maxColumns, static_cast<int>(row.size()));
    }
    ObjectManager::getInstance()->updateFrameSize({maxColumns * kTileSize, static_cast<int>(rows.size()) * kTileSize});

    for (int row = 0; row < static_cast<int>(rows.size()); ++row) {
      for (int column = 0; column < static_cast<int>(rows[row].size()); ++column) {
        TileDefinition tile;
        if (!getTileDefinition(rows[row][column], tile)) {
          continue;
        }

        int x = column * kTileSize + kTileSize / 2;
        int y = (static_cast<int>(rows.size()) - row - 1) * kTileSize;
        createTile(tileTexture, {x, y}, tile.source, tile.solid);
      }
    }
  }

  bool getTileDefinition(const std::string &id, TileDefinition &tile) {
    if (id == "." || id == "-1" || id.empty()) {
      return false;
    }

    try {
      constexpr int sourceTileSize = 16;
      constexpr int tilesetColumns = 48;
      constexpr int tilesetRows = 23;
      int tileId = std::stoi(id);
      if (tileId < 0 || tileId >= tilesetColumns * tilesetRows) {
        return false;
      }

      tile = {{(tileId % tilesetColumns) * sourceTileSize, (tileId / tilesetColumns) * sourceTileSize}, true};
      return true;
    } catch (const std::exception &) {
      return false;
    }
  }

  void createTile(const std::vector<std::shared_ptr<SDL_Texture>> &tileTexture, eng::Vector2<int> position,
                  eng::Vector2<int> source, bool solid) {
    eng::SpriteOptions tileOptions;
    tileOptions.setTextures(tileTexture)
        .setNumberOfSpritesPerSheet({1})
        .setFramesPerTextureUpdate(1)
        .setRealSpriteSize({16, 16})
        .setPosition(position)
        .setSize({kTileSize, kTileSize})
        .setHitbox({kTileSize, kTileSize})
        .setHitboxOffset({0, 0})
        .setStatic();

    if (solid) {
      tileOptions.setSolid();
    }

    int tileID = create_sprite(tileOptions);
    auto tile = ObjectManager::getInstance()->getSprite(tileID);
    if (tile) {
      tile->setSourcePosition(source);
    }
  }

  void handleMovement(std::shared_ptr<eng::Sprite> player) {
    float horizontalVelocity = 0.0f;
    auto keyboard = KeyboardManager::getInstance();
    playerMoving = false;

    const bool moveLeft = keyboard->isPressed(SDL_SCANCODE_A) || keyboard->isPressed(SDL_SCANCODE_LEFT);
    const bool moveRight = keyboard->isPressed(SDL_SCANCODE_D) || keyboard->isPressed(SDL_SCANCODE_RIGHT);
    const bool jumpPressed = keyboard->isPressed(SDL_SCANCODE_SPACE) || keyboard->isPressed(SDL_SCANCODE_W) ||
                             keyboard->isPressed(SDL_SCANCODE_UP);
    const bool jumpJustPressed = jumpPressed && !jumpPressedLastFrame;

    int wallDirection = getWallContactDirection(player);
    bool didWallJump = false;

    if (jumpJustPressed && player->isGrounded()) {
      player->setVelocity({player->getOptions().getVelocity().x, kJumpVelocity});
      player->setGrounded(false);
    } else if (jumpJustPressed && wallDirection != 0) {
      horizontalVelocity = -wallDirection * kWallJumpHorizontalVelocity;
      player->setVelocity({horizontalVelocity, kJumpVelocity});
      playerFacing = wallDirection > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
      playerMoving = true;
      wallJumpInputLockFrames = kWallJumpInputLockFrames;
      didWallJump = true;
    }

    if (!didWallJump) {
      if (wallJumpInputLockFrames > 0) {
        horizontalVelocity = player->getOptions().getVelocity().x;
        --wallJumpInputLockFrames;
      } else {
        if (moveLeft) {
          horizontalVelocity -= kMoveSpeed;
          playerFacing = SDL_FLIP_HORIZONTAL;
          playerMoving = true;
        }
        if (moveRight) {
          horizontalVelocity += kMoveSpeed;
          playerFacing = SDL_FLIP_NONE;
          playerMoving = true;
        }
      }
      player->setVelocity({horizontalVelocity, player->getOptions().getVelocity().y});
    }

    jumpPressedLastFrame = jumpPressed;
    player->setFlip(playerFacing);
    updatePlayerAnimation(player);
  }

  int getWallContactDirection(std::shared_ptr<eng::Sprite> player) {
    if (player->isGrounded()) {
      return 0;
    }

    auto options = player->getOptions();
    const int playerLeft = options.getPosition().x - options.getHitbox().x / 2 + options.getHitboxOffset().x;
    const int playerRight = options.getPosition().x + options.getHitbox().x / 2 + options.getHitboxOffset().x;
    const int playerBottom = options.getPosition().y + options.getHitboxOffset().y;
    const int playerTop = options.getPosition().y + options.getHitbox().y + options.getHitboxOffset().y;

    bool touchingLeftWall = false;
    bool touchingRightWall = false;
    auto objectManager = ObjectManager::getInstance();
    for (int id = 0; id < objectManager->getNumObjects(); ++id) {
      if (id == player->getId() || !objectManager->doesExist(id)) {
        continue;
      }

      auto other = objectManager->getSprite(id);
      if (!other || !other->getOptions().isSolid()) {
        continue;
      }

      auto otherOptions = other->getOptions();
      const int otherLeft = otherOptions.getPosition().x - otherOptions.getHitbox().x / 2 + otherOptions.getHitboxOffset().x;
      const int otherRight = otherOptions.getPosition().x + otherOptions.getHitbox().x / 2 + otherOptions.getHitboxOffset().x;
      const int otherBottom = otherOptions.getPosition().y + otherOptions.getHitboxOffset().y;
      const int otherTop = otherOptions.getPosition().y + otherOptions.getHitbox().y + otherOptions.getHitboxOffset().y;
      const bool verticallyOverlaps = playerTop > otherBottom + 4 && playerBottom < otherTop - 4;
      if (!verticallyOverlaps) {
        continue;
      }

      touchingLeftWall = touchingLeftWall || std::abs(playerLeft - otherRight) <= 2;
      touchingRightWall = touchingRightWall || std::abs(playerRight - otherLeft) <= 2;
    }

    if (touchingLeftWall) {
      return -1;
    }
    if (touchingRightWall) {
      return 1;
    }
    return 0;
  }

  void updatePlayerAnimation(std::shared_ptr<eng::Sprite> player) {
    if (!player->isGrounded()) {
      if (player->getOptions().getVelocity().y > 0.0f) {
        setPlayerAnimation(player, 2);
      } else {
        setPlayerAnimation(player, 3);
      }
      return;
    }

    if (playerMoving) {
      setPlayerAnimation(player, 1);
    } else {
      setPlayerAnimation(player, 0);
    }
  }

  void setPlayerAnimation(std::shared_ptr<eng::Sprite> player, int state) {
    if (playerAnimationState != state) {
      player->changeSpritesheet(state);
      playerAnimationState = state;
    }
  }

  void updateCamera(std::shared_ptr<eng::Sprite> player) {
    auto windowSize = windowManager->getSize();
    auto frameSize = ObjectManager::getInstance()->getFrameSize();

    int targetX = player->getPosition().x - windowSize.x / 2;
    int targetY = player->getPosition().y - windowSize.y / 2;

    int maxCameraX = std::max(0, frameSize.x - windowSize.x);
    int maxCameraY = std::max(0, frameSize.y - windowSize.y);

    targetX = std::clamp(targetX, 0, maxCameraX);
    targetY = std::clamp(targetY, 0, maxCameraY);
    windowManager->setCameraPosition(eng::Vector2<int>{targetX, targetY});
  }
};

int main() {
  std::unique_ptr<PlatformerGame> game = std::make_unique<PlatformerGame>();
  start(game.get());
  return 0;
}
