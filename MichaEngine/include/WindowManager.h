// Created by Mikhail Chalakov on 2/18/24.

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>
#include <string>
#include <vector>

#include "InternalWindow.h"
#include "Logger.h"
#include "ResourceLoader.h"
#include "Types.h"
#include "imgui.h"

class WindowManager {
private:
  // Private member variables
  float cameraX = 0;
  std::shared_ptr<SDL_Window> window;
  std::shared_ptr<SDL_Renderer> renderer;
  std::shared_ptr<InternalWindow> internalWindow;
  std::shared_ptr<TTF_Font> font;
  std::vector<std::shared_ptr<SDL_Texture>> background;
  std::vector<float> backgroundSpeeds;
  SDL_Event event;
  eng::Vector2<int> windowSize;
  bool quit;

  /**
   * Render the parallax background.
   */
  void renderParallex();

  /**
   * Get the size of the primary monitor.
   * \return Vector2<int> representing the width and height of the monitor.
   */
  static eng::Vector2<int> getMonitorSize();

protected:
  int frameCount;

public:
  // Public member variable for debug
  bool debugDraw;

  // Public methods
  /**
   * Set the size of window through SDL_SetWindowSize()
   * \param newSize the new size of the window
   */
  void setSize(eng::Vector2<int> newSize) {
    windowSize = newSize;
    SDL_SetWindowSize(window.get(), newSize.x, newSize.y);
  }
  /**
   * Set the texture of the background of the game
   * \param bkg the shared_ptr that will become the background
   */
  void setBackground(std::shared_ptr<SDL_Texture> bkg) {
    background.clear();
    background.push_back(bkg);
  }
  /**
   * Set the parallex which will continue to move during the game.
   * The new background vector size must be equal to the size of the speeds.
   * \param newBackgrounds vector of shared_ptr of the new textures to be set in parallax
   * \param speed vector of speeds for each layer of parallax to be moved at
   */
  void setParallex(std::vector<std::shared_ptr<SDL_Texture>> newBackgrounds, std::vector<float> speeds) {
    if (newBackgrounds.size() != speeds.size()) {
      LOG_ERR("Mismatch between number of parallax layers and speeds");
      return;
    }
    background = newBackgrounds;
    backgroundSpeeds = speeds;
  }
  /**
   * Draw manually given location, source, and texture
   * \param txt pointer towards a texture
   * \param src pointer towards src
   * \param dst pointer towards dst
   */
  void draw(SDL_Texture *txt, const SDL_Rect *src, const SDL_Rect *dst) {
    CHECK_RESULT(SDL_RenderCopy(renderer.get(), txt, src, dst));
  }
  void draw(std::shared_ptr<eng::Sprite> object);

  void openFont(std::string path, int fontSize) { font.reset(TTF_OpenFont(path.c_str(), fontSize), SDLDeleter()); }
  void addText(std::string text, SDL_Color color, eng::Vector2<int> pos, int fontSize) {}

  eng::Vector2<int> getAbsolutePosition(eng::Vector2<int> pos) { return {pos.x, (windowSize.y - pos.y)}; }
  eng::Vector2<int> getSize() { return windowSize; }
  // eng::Vector2<int> getCenter() { return getSize() / 2; }

  std::shared_ptr<SDL_Renderer> getRenderer() { return renderer; }
  std::shared_ptr<SDL_Window> getWindow() { return window; }
  std::shared_ptr<InternalWindow> getInternalWindow() { return internalWindow; }

  bool hasQuit() { return quit; }

  void update();

  // Constructor and Destructor
  WindowManager(const std::string &windowName, eng::Vector2<int> pos, Uint32 flag);
  ~WindowManager() {
    SDL_DestroyRenderer(renderer.get());
    SDL_DestroyWindow(window.get());
    SDL_Quit();
  }
};
