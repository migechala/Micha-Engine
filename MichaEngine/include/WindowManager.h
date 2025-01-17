// Created by Mikhail Chalakov on 2/18/24.

#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <string>
#include <vector>

#include "InternalWindow.h"
#include "Types.h"
#include "imgui.h"

class WindowManager {
 private:
  // Private member variables
  float cameraX = 0;
  std::shared_ptr<SDL_Window> window;
  std::shared_ptr<SDL_Renderer> renderer;
  std::shared_ptr<InternalWindow> internalWindow;
  std::vector<std::shared_ptr<SDL_Texture>> background;
  std::vector<float> backgroundSpeeds;
  SDL_Event event;
  eng::Vector2i windowSize;
  bool quit;

  // Private method to render parallax effect
  void renderParallex();

  // Static method to get monitor size
  static eng::Vector2i getMonitorSize();

 public:
  // Public member variable for frame count
  int frameCount;

  bool debugDraw;

  // Public methods
  void setSize(eng::Vector2i newSize);
  void setBackground(std::shared_ptr<SDL_Texture> bkg);
  void setParallex(std::vector<std::shared_ptr<SDL_Texture>> newBackgrounds,
                   std::vector<float> speeds);
  void setTiles(std::string file_path, std::vector<std::vector<int>> tiles,
                eng::Vector2i size);

  int draw(SDL_Texture* txt, const SDL_Rect* src, const SDL_Rect* dst);
  int draw(std::shared_ptr<eng::Object> object);

  eng::Vector2i getAbsolutePosition(eng::Vector2i pos);
  eng::Vector2i getCenter();
  eng::Vector2i getSize();

  std::shared_ptr<SDL_Renderer> getRenderer();
  std::shared_ptr<SDL_Window> getWindow();
  std::shared_ptr<InternalWindow> getInternalWindow();

  bool hasQuit();

  void update();

  // Constructor and Destructor
  WindowManager(const std::string& windowName, eng::Vector2i pos, Uint32 flag);
  ~WindowManager();
};
