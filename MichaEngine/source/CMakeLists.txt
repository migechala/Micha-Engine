file(GLOB sources "*.cpp")

# Create a library from the source files
add_library(source ${sources})

# Include the project-level include directory
target_include_directories(source PUBLIC "${CMAKE_SOURCE_DIR}/MichaEngine/include")
