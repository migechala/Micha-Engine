#include "WindowManager.h"
struct ExecutableClass {
  virtual void mainloop() = 0;

  WindowManager* windowManager;
  int maxFPS = 60;
};