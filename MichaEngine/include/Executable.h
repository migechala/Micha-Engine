#include "WindowManager.h"

struct ExecutableClass {
  virtual void mainloop() = 0;

  std::unique_ptr<WindowManager> windowManager;
  int maxFPS = 60;
};