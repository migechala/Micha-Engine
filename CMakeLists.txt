cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER "/usr/bin/gcc")

project(project)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-11" CACHE STRING "C++ compiler" FORCE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories("/opt/homebrew/Cellar/sdl2/2.28.3/include" "/opt/homebrew/Cellar/sdl2/2.28.3/include/SDL2/" "/opt/homebrew/Cellar/sdl2_image/2.6.3_2/include" "${CMAKE_SOURCE_DIR}/include/" "/usr/include/SDL2/" ${SDL2TTF_INCLUDE_DIR})

add_executable(${PROJECT_NAME} source/main.cpp
        include/WindowManager.h
        source/Object.cpp
        include/Object.h
        include/Types.h)

set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES arm64)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY})
target_link_directories(${PROJECT_NAME} PRIVATE include/)


add_subdirectory(source)
target_link_libraries(${PROJECT_NAME} source)
