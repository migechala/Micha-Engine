set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/imgui*.cpp ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

message(IMGUI_SOURCES ${IMGUI_DIR})

add_library(imgui STATIC ${IMGUI_SOURCES})

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL2_INCLUDE_DIR}
    )
    
target_link_libraries(imgui)
    
set_target_properties(imgui PROPERTIES LINKER_LANGUAGE CXX)

set(IMGUI_LIBRARIES imgui)