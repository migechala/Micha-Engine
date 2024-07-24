#include "ResourceLoader.h"
#include "WindowManager.h"
struct ExecutableClass {
  virtual void mainloop() = 0;

  std::shared_ptr<WindowManager> windowManager;
  int maxFPS = 60;
};