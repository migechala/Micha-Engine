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
  eng::Vector2i windowSize;
  bool quit;

  // Private method to render parallax effect
  void renderParallex();

  // Static method to get monitor size
  static eng::Vector2i getMonitorSize();

protected:
  int frameCount;

public:
  // Public member variable for debug
  bool debugDraw;

  // Public methods
  void setSize(eng::Vector2i newSize) {
    windowSize = newSize;
    SDL_SetWindowSize(window.get(), newSize.x, newSize.y);
  }
  void setBackground(std::shared_ptr<SDL_Texture> bkg) {
    background.clear();
    background.push_back(bkg);
  }
  void setParallex(std::vector<std::shared_ptr<SDL_Texture>> newBackgrounds, std::vector<float> speeds) {
    if (newBackgrounds.size() != speeds.size()) {
      LOG_ERR("Mismatch between number of parallax layers and speeds");
      return;
    }
    background = newBackgrounds;
    backgroundSpeeds = speeds;
  }

  void draw(SDL_Texture *txt, const SDL_Rect *src, const SDL_Rect *dst) {
    CHECK_RESULT(SDL_RenderCopy(renderer.get(), txt, src, dst));
  }
  void draw(std::shared_ptr<eng::Sprite> object);

  void openFont(std::string path, int fontSize) { font.reset(TTF_OpenFont(path.c_str(), fontSize), SDLDeleter()); }
  void addText(std::string text, SDL_Color color, eng::Vector2i pos, int fontSize) {}

  eng::Vector2i getAbsolutePosition(eng::Vector2i pos) { return {pos.x, (windowSize.y - pos.y)}; }
  eng::Vector2i getCenter() { return getSize() / 2; }
  eng::Vector2i getSize() { return windowSize; }

  std::shared_ptr<SDL_Renderer> getRenderer() { return renderer; }
  std::shared_ptr<SDL_Window> getWindow() { return window; }
  std::shared_ptr<InternalWindow> getInternalWindow() { return internalWindow; }

  bool hasQuit() { return quit; }

  void update();

  // Constructor and Destructor
  WindowManager(const std::string &windowName, eng::Vector2i pos, Uint32 flag);
  ~WindowManager() {
    SDL_DestroyRenderer(renderer.get());
    SDL_DestroyWindow(window.get());
    SDL_Quit();
  }
};
