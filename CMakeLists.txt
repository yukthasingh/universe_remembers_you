cmake_minimum_required(VERSION 3.10)

project(universe_remembers_you
    VERSION 1.0
    DESCRIPTION "A simple C++ project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# tell CMake where SFML is installed
set(SFML_DIR "C:/SFML/SFML-3.1.0/lib/cmake/SFML")

# Find SFML
find_package(SFML 3.1.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(universe_remembers_you
    src/main.cpp
    src/universe.cpp
    src/celestialBody.cpp
    src/renderer.cpp
    src/physics.cpp
)

target_link_libraries(universe_remembers_you
    PRIVATE
    SFML::Graphics
    SFML::Window 
    SFML::System 
)