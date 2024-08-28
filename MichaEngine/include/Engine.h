#pragma once
#include <string>
#include <vector>

#include "Executable.h"
#include "KeyboardManager.h"
#include "Logger.h"
#include "ObjectManager.h"
#include "ResourceLoader.h"
#include "Types.h"
#include "WindowManager.h"
void start(ExecutableClass* execute);

//
int create_object(eng::ObjectOptions& options);
int create_sprite(eng::SpriteOptions& options);
