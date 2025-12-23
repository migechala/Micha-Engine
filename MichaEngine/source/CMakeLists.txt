file(GLOB sources "*.cpp")
add_library(source ${sources})

target_include_directories(source PUBLIC "${CMAKE_SOURCE_DIR}/MichaEngine/include")

set(BASE_PATH "${CMAKE_SOURCE_DIR}/")
get_filename_component(BASE_PATH "${BASE_PATH}" REALPATH)

target_compile_definitions(source PUBLIC
    BASE_PATH=\"${BASE_PATH}\"
)
