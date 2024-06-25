#include <Engine.h>

#include <iostream>

class Game : public ExecutableClass {
 public:
  Game() {
    SDL_Init(SDL_INIT_EVERYTHING);

    maxFPS = 120;

    windowManager = new WindowManager(
        "Test Engine", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    type::Vector2i center = windowManager->getCenter();
    LOG_INFO(std::to_string(center.x), LOG_LEVEL::LOW)
    create_sprite(center, {100, 100});
    create_sprite_with_velocity(center, {100, 100}, {1, 1});
    create_sprite_with_velocity(center, {100, 100}, {-1, 1});
    create_sprite_with_velocity(center, {100, 100}, {1, -1});
    create_sprite_with_velocity(center, {100, 100}, {-1, -1});
    create_sprite_with_velocity(center, {100, 100}, {4, 4});
    create_sprite_with_velocity(center, {100, 100}, {2, 1});
    create_sprite_with_velocity(center, {100, 100}, {1, 3});
    create_sprite_with_velocity(center, {100, 100}, {9, 1});
  }

 public:
  void mainloop() override {
    LOG_INFO("Running mainloop", LOG_LEVEL::MEDIUM)
    windowManager->debugFrame();
  }
};

int main() {
  Game *game = new Game();
  start(game);
  return 0;
}
