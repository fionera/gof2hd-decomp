cmake_minimum_required(VERSION 3.20)

# The byte-matching ARM (armeabi-v7a, NDK r18b) toolchain compiles every TU in src/ the way the
# original libgof2hdaa.so was built, links the same objects + vendored archives into a whole .so,
# and compares it against the original. Apply the toolchain automatically so a plain `cmake -B build`
# configures the matching build.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake")
endif()

project(GalaxyOnFire2 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # compile_commands.json for clangd/CLion code intelligence

# The original release .so we are matching (ELF 32-bit ARM, armeabi-v7a softfp, stripped).
set(GOF2_ORIGINAL_SO "${CMAKE_SOURCE_DIR}/../_work/bins/android_2.0.16_libgof2hdaa.so"
    CACHE FILEPATH "The original libgof2hdaa.so to match against")

# One OBJECT library feeds both the per-function ASM verify (diff vs the original) and the whole-.so
# link (gold link below). The original .so is -fPIC, so these objects match it and link cleanly.
file(GLOB_RECURSE _srcs CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library(gof2 OBJECT ${_srcs})

# =================================================================================================
# Whole-.so link -- produce libgof2hdaa.so the original's shape
# =================================================================================================
# Vendored static libs (libcrypto/zlib/libzip subsets) the original statically linked, built with
# the downloaded NDK clang. Sets _openssl_out / _zlib_out / _libzip_out + target gof2_libs.
include("${CMAKE_SOURCE_DIR}/third_party/match_libs.cmake")

# Link the gof2 objects + the vendored archives into libgof2hdaa.so with the NDK gold linker,
# exactly as the original was linked: crtbegin/crtend_so.o, the same NEEDED libs, the static C++ ABI
# runtime (libc++abi.a), and the zlib/libzip archives pulled whole (the original exports their full
# symbol set). Undefined app-provided imports (FMOD, ...) stay unresolved, as in the original.
#
# This is a single, plain gold link -- NO second pass and NO --version-script. Any symbol the
# original does not export but we do is fixed by reshaping the C++ (internal linkage, structure),
# not by hiding it at link time. The compiler-emitted duplicate C1/C2 & D1/D2 constructor/destructor
# aliases that cannot be removed in source are the documented irreducible delta (see docs/VALIDATION.md).
set(_so "${CMAKE_BINARY_DIR}/libgof2hdaa.so")
set(_libdir "${GOF2_NDK_CRT_LIBDIR}")
add_custom_command(OUTPUT "${_so}"
    COMMAND ${GOF2_NDK_GOLD}
            -shared -soname libgof2hdaa.so --build-id
            --unresolved-symbols=ignore-all --allow-multiple-definition
            -o "${_so}"
            ${_libdir}/crtbegin_so.o -L ${_libdir}
            -l log -l GLESv2 -l GLESv1_CM -l EGL -l android -l m -l dl -l c
            $<TARGET_OBJECTS:gof2>
            ${_openssl_out}
            --whole-archive ${_libzip_out} ${_zlib_out} --no-whole-archive
            ${GOF2_NDK_LIBCXXABI_A}
            ${_libdir}/crtend_so.o
    # Depend on the object FILES (not just the gof2 target): an OBJECT library produces no single
    # artifact, so depending on the target alone does not re-link when a member object changes,
    # leaving a stale .so. Listing the objects makes the link re-run whenever any one of them does.
    DEPENDS gof2 gof2_libs $<TARGET_OBJECTS:gof2>
    COMMENT "Linking libgof2hdaa.so (gold) from gof2 objects + vendored archives"
    COMMAND_EXPAND_LISTS VERBATIM)
add_custom_target(link_so DEPENDS "${_so}")

# =================================================================================================
# Validation targets
# =================================================================================================
# diff -- whole-.so dynamic-symbol parity gate. Builds the .so, then compares our exported .dynsym
# against the original's: prints original / ours / extra / missing / wrong_type. Exit 0 iff parity.
add_custom_target(diff
    DEPENDS "${_so}"
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/sodiff.py
            --original "${GOF2_ORIGINAL_SO}" --ours "${_so}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Symbol-parity diff vs the original .so")

# verify -- per-function ASM match of our linked .so vs the original (byte-exact / linked-exact /
# fuzzy %), plus the missing/wrong_type/extra symbol-set deltas.
add_custom_target(verify
    DEPENDS "${_so}"
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/verify/verify.py
            --build-dir ${CMAKE_BINARY_DIR}/verify
            --original "${GOF2_ORIGINAL_SO}" --ours "${_so}"
            --objdump ${GOF2_NDK_OBJDUMP}
            --fail-on-wrong-type
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Validating ARM build vs original .so (opt=${GOF2_MATCH_OPT})")

# verify-fn -- single-function side-by-side diff:  FN=<mangled> cmake --build <dir> --target verify-fn
add_custom_target(verify-fn
    DEPENDS "${_so}"
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/verify/verify.py
            --build-dir ${CMAKE_BINARY_DIR}/verify
            --original "${GOF2_ORIGINAL_SO}" --ours "${_so}"
            --objdump ${GOF2_NDK_OBJDUMP}
            --show "$ENV{FN}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Side-by-side ASM diff for FN")

# lint -- structural invariants (one class per file, no void*/union, no explicit template
# instantiation, no asm/__attribute__/extern-C/linker hacks, no circular includes).
add_custom_target(lint
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/goal_lint.py ${CMAKE_SOURCE_DIR}/src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Structural lint of src/")
