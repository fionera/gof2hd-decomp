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
    # Compile every TU with the NDK r18b toolchain into a TRACKED object library.
    # Ninja owns the source->object mapping, so deleting a .cpp removes its object
    # from the link automatically -- the stale-orphan class of bug that the old
    # glob-based build_objs.sh + relink-by-glob suffered is now impossible.
    file(GLOB_RECURSE _match_srcs CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
    # Superseded recovered platform TUs that no longer compile -- their symbols are
    # provided by the C++ reimplementations (ApkLoader.cpp / FileInterfaceAndroid.cpp /
    # JniBridge.cpp / BlowfishSubkeyFile.cpp). Excluded to keep the build green.
    list(FILTER _match_srcs EXCLUDE REGEX
         "/platform/(recovered_a462c|recovered_7a41c|recovered_128188|jni_bridge)\\.cpp$")
    add_library(gof2_match OBJECT ${_match_srcs})
    set_target_properties(gof2_match PROPERTIES POSITION_INDEPENDENT_CODE OFF)  # -fpic already in CMAKE_CXX_FLAGS

    # src/runtime/* are thin wrappers over the NDK libc++abi runtime sources; they
    # need the libc++abi private src/ on the include path + C++17 aligned-new so the
    # objects byte-match what the original linked from libc++abi.
    file(GLOB _match_runtime CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/runtime/*.cpp")
    if(_match_runtime)
        set(_ndk "$ENV{NDK}")
        if(NOT _ndk)
            set(_ndk "/opt/android-ndk-r18b")
        endif()
        set_source_files_properties(${_match_runtime} PROPERTIES COMPILE_OPTIONS
            "-faligned-allocation;-isystem;${_ndk}/sources/cxx-stl/llvm-libc++abi/src")
    endif()

    # Authoritative, tracked object list consumed by relink.py -- NO globbing of the
    # build tree, so a stale orphan object can never sneak into the link.
    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/match_objects.txt"
         CONTENT "$<TARGET_OBJECTS:gof2_match>")

    set(_verify_py "${CMAKE_SOURCE_DIR}/tools/verify/verify.py")
    set(_relink_py "${CMAKE_SOURCE_DIR}/tools/verify/relink.py")
    set(_symdiff_py "${CMAKE_SOURCE_DIR}/tools/symdiff.py")

    # Per-function ASM validation vs the original .so (objects built by gof2_match).
    add_custom_target(verify
        COMMAND ${CMAKE_COMMAND} -E env "GOF2_MATCH_OPT=${GOF2_MATCH_OPT}"
                python3 ${_verify_py} --build-dir ${CMAKE_BINARY_DIR}/verify --no-build --fail-on-wrong-type
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
        COMMENT "Validating ARM build vs original .so (opt=${GOF2_MATCH_OPT})")
    add_dependencies(verify gof2_match)
    # Single-function side-by-side diff:  FN=<mangled> cmake --build ... --target verify-fn
    add_custom_target(verify-fn
        COMMAND ${CMAKE_COMMAND} -E env "GOF2_MATCH_OPT=${GOF2_MATCH_OPT}"
                python3 ${_verify_py} --build-dir ${CMAKE_BINARY_DIR}/verify --no-build --show
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
        COMMENT "Diffing one function (set FN=<mangled symbol>)")
    add_dependencies(verify-fn gof2_match)
    # Whole-.so symbol parity: link the tracked objects, then diff vs the original.
    add_custom_target(relink
        COMMAND python3 ${_relink_py}
        COMMAND python3 ${_symdiff_py}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
        COMMENT "Linking libgof2hdaa.so from tracked objects + symbol diff")
    add_dependencies(relink gof2_match)
    return()  # native library/host targets below are not part of the matching build
endif()

# Single source tree for the native (macOS, 64-bit) port of Galaxy on Fire 2, recovered by
# decompilation of the Android libgof2hdaa.so. This replaces the original byte-matching ARM decomp
# (which lived under src/recovered, work/, tools/ and was driven by a Makefile + NDK toolchain) —
# that work is preserved in git history. Layout:
#   src/           one .cpp + its co-located .h per class; shared foundation (common.h/math.h/
#                  externs.h, platform/) at the tree root. Include paths are relative to src/.
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
target_include_directories(gof2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_options(gof2 PRIVATE -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast)

# JNI
target_include_directories(gof2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/jni")

# OpenGL ES 2.0 (Khronos registry headers, header-only)
target_include_directories(gof2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/gl")

# ---- FMOD Ex 4.44.07 (audio) ------------------------------------------------------------------
target_include_directories(gof2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmod/inc")

# libzip 0.9.3 (asset/APK archive access)
target_include_directories(gof2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libzip")

# OpenSSL libcrypto (vendored SHA-256 API subset for record integrity hashing)
target_include_directories(gof2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/crypto")
