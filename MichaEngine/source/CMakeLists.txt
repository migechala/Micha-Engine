file(GLOB sources "*.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ../include)

add_library(
            source
            ${sources}
)
