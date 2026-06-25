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

# src/runtime/* are thin wrappers over the NDK libc++abi runtime sources; they need the libc++abi
# private src/ on the include path + C++17 aligned-new so the objects byte-match what the original
# linked from libc++abi.
file(GLOB _runtime CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/runtime/*.cpp")
if(_runtime)
    set_source_files_properties(${_runtime} PROPERTIES COMPILE_OPTIONS
        "-faligned-allocation;-isystem;${GOF2_NDK_LIBCXXABI_SRC}")
endif()

# Tracked object list the verifier reads (no globbing of the build tree -> no stale orphans).
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/match_objects.txt" CONTENT "$<TARGET_OBJECTS:gof2>")

# ---- per-function ASM validation vs the original .so --------------------------------------------
set(_verify_py "${CMAKE_SOURCE_DIR}/tools/verify/verify.py")
set(_match_env
    "GOF2_MATCH_OPT=${GOF2_MATCH_OPT}"
    "GOF2_NDK_ROOT=${GOF2_NDK_ROOT}"
    "GOF2_NDK_OBJDUMP=${GOF2_NDK_OBJDUMP}"
    "GOF2_NDK_NM=${GOF2_NDK_NM}"
    "GOF2_NDK_AS=${GOF2_NDK_AS}")

add_custom_target(verify
    COMMAND ${CMAKE_COMMAND} -E env ${_match_env}
            python3 ${_verify_py} --build-dir ${CMAKE_BINARY_DIR}/verify --no-build --fail-on-wrong-type
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Validating ARM build vs original .so (opt=${GOF2_MATCH_OPT})")
add_dependencies(verify gof2)
# Single-function side-by-side diff:  FN=<mangled> cmake --build ... --target verify-fn
add_custom_target(verify-fn
    COMMAND ${CMAKE_COMMAND} -E env ${_match_env}
            python3 ${_verify_py} --build-dir ${CMAKE_BINARY_DIR}/verify --no-build --show
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Diffing one function (set FN=<mangled symbol>)")
add_dependencies(verify-fn gof2)

# ---- whole-.so link + symbol-parity gate -------------------------------------------------------
# Vendored static libs (libcrypto/zlib/libzip subsets) the original statically linked, built with
# the downloaded NDK clang. Sets _openssl_out / _zlib_out / _libzip_out + target gof2_libs.
include("${CMAKE_SOURCE_DIR}/third_party/match_libs.cmake")

# Link the same gof2 objects + the vendored archives into libgof2hdaa.so with the NDK gold linker,
# exactly as the original was linked: crtbegin/crtend_so.o, the same NEEDED libs, the static C++ ABI
# runtime (libc++abi.a) and the zlib/libzip archives pulled whole (the original exports their full
# symbol set). Undefined app-provided imports (FMOD, ...) stay unresolved, as in the original.
set(_so "${CMAKE_BINARY_DIR}/libgof2hdaa.so")
set(_libdir "${GOF2_NDK_CRT_LIBDIR}")
add_custom_command(OUTPUT "${_so}"
    COMMAND ${GOF2_NDK_GOLD}
            -shared -soname libgof2hdaa.so --build-id
            --unresolved-symbols=ignore-all --allow-multiple-definition
            ${_libdir}/crtbegin_so.o -L ${_libdir}
            -l log -l GLESv2 -l GLESv1_CM -l EGL -l android -l m -l dl -l c
            $<TARGET_OBJECTS:gof2>
            ${_openssl_out}
            --whole-archive ${_libzip_out} ${_zlib_out} --no-whole-archive
            ${GOF2_NDK_LIBCXXABI_A}
            ${_libdir}/crtend_so.o
            -o "${_so}"
    DEPENDS gof2 gof2_libs
    COMMENT "Linking libgof2hdaa.so (gold) from gof2 objects + vendored archives"
    COMMAND_EXPAND_LISTS VERBATIM)

# exit 0 iff our defined dynsyms == the original's (0 extra, 0 missing).
add_custom_target(relink
    DEPENDS "${_so}"
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/symdiff.py "${_so}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Symbol-parity diff vs the original .so")
