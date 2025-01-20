# Micha Engine

## Basic C++ SDL2 Game Engine

### Overview

Micha Engine is a basic game engine using SDL2 for rendering. It provides a simple framework for creating games with C++. The engine is designed to be easy to use and extend, with core functionalities for creating and managing sprites. Keep in mind, in this engine, each window position is based off of bottom-center.

### Prerequisites

- C++ compiler (e.g., g++, clang++)
- CMake
- SDL2

### Building the Project

To build the project, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/migechala/Micha-Engine.git
    cd Micha-Engine
    ```

2. Create a build directory and navigate to it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to generate the build files:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make
    ```

### Basic Usage

To use the Micha Engine, follow these steps:

1. **Include the Necessary Headers:**
    Make sure to include the relevant headers for the engine in your `main.cpp`.

    ```cpp
    #include "Engine.h"
    ```

2. **Create a Class that Inherits `MichaApp`:**
    Define a class that inherits from `MichaApp` and override the `void mainloop` method with your game logic.

    ```cpp
    class MyGame : public MichaApp {
    public:
        void mainloop() override {
            // Your game logic here
        }
    };
    ```
    In this class, make Sure to reset `windowManager`, a `shared_ptr` that is crucial to rendering on the screen, and set your `maxFPS`
    ```cpp
    class MyGame : public MichaApp {
    public:
        void mainloop() override {...}
        MyGame(){
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

A complete example of a `main.cpp` file using the Micha Engine can be found in the file `main.cpp`

### **Class: `eng::Vector2i`**

A 2D integer vector class used to represent positions, velocities, and other vector-related operations in the game engine.

#### **Constructors:**
- **`Vector2i()`**: Initializes the vector to (0, 0).
- **`Vector2i(int x, int y)`**: Initializes the vector with the provided `x` and `y` values.

#### **Operators:**
- **Arithmetic Operators:**
  - `+`: Adds two `Vector2i` vectors.
  - `-`: Subtracts one `Vector2i` vector from another.
  - `*`: Multiplies two `Vector2i` vectors element-wise.
  - `/`: Divides one `Vector2i` vector by another or a scalar.
- **Assignment Operators:**
  - `+=`: Adds another `Vector2i` to the current vector.

---

### **Class: `eng::Object`**

Represents a game object with position, velocity, and other customizable options.

#### **Key Methods:**
- **`setPosition(Vector2i newPosition)`**: Updates the object's position.
- **`setVelocity(Vector2i newVelocity)`**: Sets the object's velocity.
- **`setAcceleration(Vector2i newAcceleration)`**: Updates the object's acceleration.
- **`getId()`**: Returns the object's unique ID.
- **`isSprite()`**: Returns whether the object is a sprite (default: `false`).

---

### **Class: `eng::Sprite`**

A specialized subclass of `Object` that includes functionality for texture management and animations.

#### **Key Features:**
- **Cut-Out Management**: Handles sprite sheet subregion selection for animations.
- **Texture Updates**:
  - **`updateTexture(int frame)`**: Updates the current texture based on the frame count and animation parameters.
  - **`changeSpritesheet(int index)`**: Changes the current sprite sheet.
- **Animation Control**:
  - **`setFramesPerUpdate(int fpu)`**: Configures how many frames are needed for a texture update.
  - **`toggleUpdate()`**: Toggles automatic updates for the sprite's animation.
- **`isSprite()`**: Overrides the parent method to return `true`.

---

### **Class: `eng::ObjectOptions`**

Provides a configuration interface for game objects, allowing customization during object initialization.

#### **Key Methods:**
- **Setters:**
  - Position, size, velocity, acceleration, hitbox, and hitbox offset.
  - Color and flip settings (`setColor`, `setFlip`).
  - Gravity control (`enableGravity`).
- **Getters:**
  - Retrieves various object properties like position, size, and gravity status.

---

### **Class: `eng::SpriteOptions`**

Extends `ObjectOptions` with additional parameters for sprite configuration.

#### **Key Features:**
- **Texture Management:**
  - **`setTextures()`**: Sets textures for the sprite.
  - **`setNumberOfSpritesPerSheet()`**: Configures the number of sprites per texture sheet.
  - **`setRealSpriteSize()`**: Defines the size of individual sprites on the sheet.
- **Animation Timing:**
  - **`setFramesPerTextureUpdate(int numFrames)`**: Sets the number of frames between texture updates.

---

## Dependencies

- **`ObjectManager.h`**: Manages the lifecycle of game objects.
- **`ResourceLoader.h`**: Handles the loading of textures and other game resources.
- **`SDL`**: Used for rendering and texture management.

---

## Example Usage

### Creating a New Object:
```cpp
eng::ObjectOptions options;
options.setPosition({100, 200}).setSize({50, 50});
eng::Object player(options);