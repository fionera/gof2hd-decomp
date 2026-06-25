cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake")
endif()

project(GalaxyOnFire2 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # compile_commands.json for clangd/CLion code intelligence

file(GLOB_RECURSE _srcs CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library(gof2 OBJECT ${_srcs})