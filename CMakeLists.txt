cmake_minimum_required(VERSION 3.20)

# This project IS the byte-matching ARM build: it compiles every TU with the original NDK r18b
# toolchain, validates the result against the original libgof2hdaa.so function by function, and
# relinks a symbol-identical .so. Apply the toolchain automatically so a plain `cmake -B build`
# configures it. cmake/DownloadNDK.cmake fetches the NDK; it runs natively (darwin-x86_64 under
# Rosetta 2 on Apple Silicon).
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake")
endif()

project(GalaxyOnFire2 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # compile_commands.json for clangd/CLion code intelligence

# ---- the library: every TU compiled to ARM objects (-fPIC, from the toolchain flags) ------------
# One OBJECT library serves both the per-function verify (diff vs the original) and the whole-.so
# relink (gold link below): the original .so is -fPIC, so these objects match it and link cleanly.
# Ninja tracks the source->object mapping, so deleting a .cpp drops its object automatically.
file(GLOB_RECURSE _srcs CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library(gof2 OBJECT ${_srcs})