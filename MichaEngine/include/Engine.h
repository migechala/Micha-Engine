#pragma once
#include <string>
#include <vector>

#include "Executable.h"
#include "KeyboardManager.h"
#include "Logger.h"
#include "ObjectManager.h"
#include "Types.h"
#include "WindowManager.h"
void start(ExecutableClass *execute);
void start(std::unique_ptr<ExecutableClass> execute);

int create_sprite(type::Vector2i position, type::Vector2i size,
                  type::Vector2i velocity = {0, 0},
                  type::Vector2i acceleration = {0, 0});