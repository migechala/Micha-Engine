# Micha Engine

## Basic C++ SDL2 Game Engine

### Overview

Micha Engine is a basic game engine using SDL2 for rendering. It provides a simple framework for creating games with C++. The engine is designed to be easy to use and extend, with core functionalities for creating and managing sprites.

### Basic Usage

To use the Micha Engine, follow these steps:

1. **Include the Necessary Headers:**
    Make sure to include the relevant headers for the engine in your `main.cpp`.

    ```cpp
    #include "Engine.h"
    ```

2. **Create a Class that Inherits `ExecutableClass`:**
    Define a class that inherits from `ExecutableClass` and override the `void mainloop` method with your game logic.

    ```cpp
    class MyGame : public ExecutableClass {
    public:
        void mainloop() override {
            // Your game logic here
        }
    };
    ```
    In this Class, Make Sure to Init SDL2, reset `windowManager`, and set your `maxFPS`
    ```cpp
    class MyGame : public ExecutableClass {
    public:
        void mainloop() override {...}
        MyGame(){
        SDL_Init(SDL_INIT_VIDEO);
        maxFPS = 120;

        windowManager.reset(new WindowManager("Hello World!", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI));
        }
    };
    ```

3. **Define the `main` Function to Start:**
    In your main function, create an instance of `WindowManager` and set the `windowManager` pointer.

    ```cpp
    int main(int argc, char* argv[]) {
        std::unique_ptr<Game> game;
        game.reset(new Game());
        start(game.get());
        return 0;
    }
    ```

4. **Create Sprites:**
    Use the `create_sprite` functions to create and manage sprites in your game.

    ```cpp
    MyGame() {
        // Initializations...
        //
        // Example: Create a still, red square sprite
        int spriteID = create_sprite(windowManager->getCenter(), {100, 100}, {0, 0}, {0, 0});
        // When the space bar is pressed, set the acceleration of the sprite to 1
        KeyboardManager::getInstance()->addListener(SDL_SCANCODE_SPACE, [spriteID]() { ObjectManager::getInstance()->getObject(spriteID)->acceleration.y = 1;} ,true);
    }
    ```

### Example `main.cpp`

Here's an example of a complete `main.cpp` file using the Micha Engine:

```cpp
#include <Engine.h>

#include <iostream>

class Game : public ExecutableClass {
 public:
  Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    Logger::setLogLevel(LOG_LEVEL::PRIORITY); // set a log level to show
    maxFPS = 120;

    windowManager.reset(new WindowManager(
        "Hello World!", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI));

    int mainCharacterID = create_sprite(windowManager->getCenter();, {100, 100}, {0, 0}, {0, -1});

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
```