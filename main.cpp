#include <Engine.h>

#include <iostream>

class Game : public ExecutableClass {
 public:
  Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    Logger::setLogLevel(LOG_LEVEL::MEDIUM);
    maxFPS = 120;

    windowManager.reset(new WindowManager(
        "Test Engine", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));

    type::Vector2i center = windowManager->getCenter();
    LOG_INFO(std::to_string(center.x), LOG_LEVEL::MEDIUM)
    LOG_INFO(std::to_string(center.y), LOG_LEVEL::MEDIUM)
    create_sprite({0, 0}, {100, 100});
    // create_sprite_with_velocity(center, {100, 100}, {1, 1});
    // create_sprite_with_velocity(center, {100, 100}, {-1, 1});
    // create_sprite_with_velocity(center, {100, 100}, {1, -1});
    // create_sprite_with_velocity(center, {100, 100}, {-1, -1});
    // create_sprite_with_velocity(center, {100, 100}, {4, 4});
    // create_sprite_with_velocity(center, {100, 100}, {2, 1});
    // create_sprite_with_velocity(center, {100, 100}, {1, 3});
    // create_sprite_with_velocity(center, {100, 100}, {9, 1});
  }

 public:
  void mainloop() override {
    LOG_INFO("Running mainloop", LOG_LEVEL::LOW)
    windowManager.get()->debugFrame();
  }
};

int main() {
  std::unique_ptr<Game> game;
  game.reset(new Game());
  start(game.get());
  return 0;
}
