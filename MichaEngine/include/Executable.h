#include "ResourceLoader.h"
#include "WindowManager.h"
struct ExecutableClass {
  virtual void mainloop() = 0;
  std::unique_ptr<WindowManager> windowManager;
  int maxFPS = 60;

  template <class... Args>
  std::vector<SDL_Texture*> loadTextures(Args... file_name) {
    std::vector<SDL_Texture*> textures;
    (textures.push_back(ResourceLoader::getInstance()->loadTexture(
         file_name, windowManager->getRenderer())),
     ...);
    return textures;
  }
};