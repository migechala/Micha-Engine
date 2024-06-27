//
// Created by Mikhail Chalakov on 2/18/24.
//
#include "WindowManager.h"

#include <imgui.h>

#include <string>

#include "Logger.h"
#include "ObjectManager.h"
#include "imgui.h"
#if __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>  // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>  // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>  // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or
                           // multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or
                           // multiple definition errors.
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>  // Initialize with glbinding::initialize()
using namespace gl;
#endif

bool WindowManager::hasQuit() {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return true;
    }
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        return true;
      }
    }
  }
  return false;
}

type::Vector2i WindowManager::getMonitorSize() {
  SDL_DisplayMode t_dm;
  if (SDL_GetCurrentDisplayMode(0, &t_dm) != 0) {
    std::string err = "DP error: ";
    LOG_ERR(err + SDL_GetError())
    exit(-1);
  }
  LOG_INFO(std::to_string(t_dm.w), LOG_LEVEL::LOW)
  return {t_dm.w, t_dm.h};
}

int WindowManager::draw(SDL_Texture *txt, const SDL_Rect *src,
                        const SDL_Rect *dst) {
  return SDL_RenderCopy(renderer.get(), txt, src, dst);
}

type::Vector2i WindowManager::getAbsolutePosition(type::Vector2i position) {
  int h;
  SDL_GetWindowSize(window.get(), &h, nullptr);
  return {position.x, h - position.y};
}
type::Vector2i WindowManager::getCenter() { return windowSize / 2; }
int WindowManager::draw(type::Object *object) {
  SDL_Color oldColor;

  type::Vector2i pos = getAbsolutePosition(object->position);
  object->dst.x = pos.x;
  object->dst.y = pos.y;

  SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g, &oldColor.b,
                         &oldColor.a);
  SDL_SetRenderDrawColor(renderer.get(), object->color.r, object->color.g,
                         object->color.b, object->color.a);
  int ret = SDL_RenderFillRect(renderer.get(), &object->dst);
  SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                         oldColor.a);
  return ret;
}

int WindowManager::draw(type::Sprite *sprite) {
  type::Vector2i pos = getAbsolutePosition(sprite->getPosition());
  SDL_Rect realPosition = SDL_Rect({pos.x, pos.y});
  if (sprite->getTexture()) {
    return SDL_RenderCopy(renderer.get(), sprite->getTexture(), nullptr,
                          &realPosition);
  }
  SDL_Color oldColor;
  SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g, &oldColor.b,
                         &oldColor.a);
  SDL_SetRenderDrawColor(renderer.get(), 128, 128, 128, 255);
  int ret = SDL_RenderFillRect(renderer.get(), &realPosition);
  SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                         oldColor.a);
  return ret;
}

void WindowManager::update() {
  if (background) {
    if (draw(background.get(), nullptr, nullptr) != 0) {
      std::string err = "Draw BG error: ";
      LOG_ERR(err + SDL_GetError());
      exit(-1);
    }
  } else {
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer.get(), &oldColor.r, &oldColor.g,
                           &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(renderer.get(), 128, 128, 128, 255);
    int ret = SDL_RenderFillRect(renderer.get(), NULL);
    SDL_SetRenderDrawColor(renderer.get(), oldColor.r, oldColor.g, oldColor.b,
                           oldColor.a);
  }
  for (int i = 0; i < ObjectManager::getInstance()->getNumObjects(); ++i) {
    auto curObject = ObjectManager::getInstance()->getObject(i);
    if (!curObject) continue;
    LOG_INFO("Drawing object with id: " + std::to_string(i), LOG_LEVEL::MEDIUM)
    if (draw(curObject) != 0) {
      LOG_ERR(SDL_GetError());
      exit(-1);
    }
  }
  ++frameCount;
  //
  SDL_RenderPresent(renderer.get());
}
void WindowManager::setBackground(SDL_Texture *bkg) { background.reset(bkg); }

WindowManager::WindowManager(const std::string &windowName, type::Vector2i pos,
                             Uint32 flag)
    : frameCount(0), windowSize(getMonitorSize()), imgui_open(true) {
  LOG_INFO("Created Window", LOG_LEVEL::MEDIUM);

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  window.reset(SDL_CreateWindow(windowName.c_str(), pos.x, pos.y, windowSize.x,
                                windowSize.y, flag));

  if (!window) {
    LOG_ERR("Error! Window is null");
  }
  gl_context = SDL_GL_CreateContext(window.get());
  SDL_GL_MakeCurrent(window.get(), gl_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = ImGui::GetIO();
  (void)&io;

  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window.get(), gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  renderer.reset(
      SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

  if (!renderer) {
    std::string err = "Renderer error: ";
    LOG_ERR(err + SDL_GetError());
    exit(-1);
  }

  SDL_Surface *color = SDL_CreateRGBSurfaceWithFormat(
      0, 100, 100, 8, SDL_PixelFormatEnum::SDL_PIXELFORMAT_BGR24);
  if (!color) {
    std::string err = "Color error: ";
    LOG_ERR(err + SDL_GetError());
    exit(-1);
  }
}

void WindowManager::debugFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window.get());
  ImGui::NewFrame();
  LOG_INFO("Rendering demo window", LOG_LEVEL::LOW);
  ImGui::ShowDemoWindow(&imgui_open);
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window.get());
}

WindowManager::~WindowManager() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_Quit();
}
