cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AvocadoThing)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:/Users/nepat/OneDrive/Desktop/SFML_Setup/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(AvocadoThing main.cpp textureManager.cpp textureManager.h textBox.cpp textBox.h button.cpp button.h avocado.cpp avocado.h spriteButton.cpp spriteButton.h Graph.h edge.cpp edge.h)
target_link_libraries(AvocadoThing sfml-graphics sfml-audio)