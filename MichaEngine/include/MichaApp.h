#include "ResourceLoader.h"
#include "WindowManager.h"
struct MichaApp {
  virtual void mainloop();
  std::shared_ptr<WindowManager> windowManager;
  int maxFPS = 60;
  MichaApp();
};