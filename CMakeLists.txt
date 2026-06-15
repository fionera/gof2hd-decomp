cmake_minimum_required(VERSION 3.20)
project(GalaxyOnFire2 LANGUAGES CXX)

# ---- matching ASM-validation build (the `match` preset) ----------------------------------------
# When configured with cmake/orbstack-ndk-arm.toolchain.cmake, this is NOT the native build: it
# exposes function-level validation of our ARM build output against the original libgof2hdaa.so.
# `cmake --build cmake-build-match --target verify` compiles every TU with the NDK r18b toolchain
# (in OrbStack), delinks the matching originals out of the .so, disassembles both with GNU objdump
# (objdiff can't read ARMv7), and prints a per-function match-% table. Tune GOF2_MATCH_OPT (and the
# flags in tools/verify/match_flags.sh) and re-run to chase byte-exactness.
if(GOF2_MATCH)
    set(_verify_py "${CMAKE_SOURCE_DIR}/tools/verify/verify.py")
    set(_verify_dir "${CMAKE_BINARY_DIR}/verify")
    add_custom_target(verify
        COMMAND ${CMAKE_COMMAND} -E env "GOF2_MATCH_OPT=${GOF2_MATCH_OPT}"
                python3 ${_verify_py} --build-dir ${_verify_dir}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
        COMMENT "Validating ARM build vs original .so (opt=${GOF2_MATCH_OPT})")
    # Single-function side-by-side diff:  FN=<mangled> cmake --build ... --target verify-fn
    # verify.py reads the symbol from the FN environment variable (passed through
    # to the build at run time).
    add_custom_target(verify-fn
        COMMAND ${CMAKE_COMMAND} -E env "GOF2_MATCH_OPT=${GOF2_MATCH_OPT}"
                python3 ${_verify_py} --build-dir ${_verify_dir} --no-build --show
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
        COMMENT "Diffing one function (set FN=<mangled symbol>)")
    return()  # native library/host targets below are not part of the matching build
endif()

# Single source tree for the native (macOS, 64-bit) port of Galaxy on Fire 2, recovered by
# decompilation of the Android libgof2hdaa.so. This replaces the original byte-matching ARM decomp
# (which lived under src/recovered, work/, tools/ and was driven by a Makefile + NDK toolchain) —
# that work is preserved in git history. Layout:
#   include/gof2/  shared foundation (common.h/math.h/fwd.h) + one real-struct header per class
#   src/           one .cpp per class (struct cleanup in progress: not every TU compiles yet)
#   host/          macOS entry point + platform layer (Milestone 2)
#   tools/         transformation/build helpers (rewrite_body.py, reconcile.py, build.sh, ...)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)               # gnu++14 (the recovered code relies on GNU extensions)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)      # compile_commands.json for CLion/clangd code intelligence
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# ---- engine + game library --------------------------------------------------------------------
# The whole tree now compiles as proper C++ with NO -fpermissive and NO -w. The only suppressed
# warnings are the int<->pointer casts inherent to the recovered 32-bit handle fields stored as int.
file(GLOB_RECURSE GOF2_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(gof2 STATIC ${GOF2_SOURCES})
target_include_directories(gof2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(gof2 PRIVATE -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast)

# ---- host executable (Milestone 2 entry point; placeholder until the platform layer lands) -----
add_executable(gof2_host host/main.cpp)
target_include_directories(gof2_host PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Enable once the library builds cleanly and the SDL2/GLES2 platform layer exists:
#   target_link_libraries(gof2_host PRIVATE gof2)
