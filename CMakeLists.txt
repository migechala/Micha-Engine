cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER "/usr/bin/gcc")

project(Game)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/MichaEngine/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)


include(MichaEngine/cmake/imgui.cmake)


include_directories(
    "/opt/homebrew/Cellar/sdl2/2.30.3/include"
    "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include"
    "/usr/include/SDL2/"
    ${SDL2TTF_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES arm64)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${OPENGL_LIBRARIES} ${IMGUI_LIBRARIES})


# Include the source directory
add_subdirectory(MichaEngine/source)

# Link the 'source' library with the main executable
target_link_libraries(${PROJECT_NAME} source)
