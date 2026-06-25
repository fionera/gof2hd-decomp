# Matching ARM (armeabi-v7a, NDK r18b) toolchain for the `match` preset.
#
# Normal development uses the local Apple-clang `debug`/`release` presets and is
# unaffected by this file. The `match` preset selects this toolchain to build the
# symbol-/byte-matching ARM objects and link the verification libgof2hdaa.so.
#
# The matching compiler only runs on Linux, so it lives in the OrbStack `ubuntu`
# machine and is invoked through tools/verify/orbcc. CMake DOES compile here now
# (an OBJECT library): Ninja tracks the source->object mapping, so a deleted source
# can never leave a stale orphan object in the link the way the old glob-based
# build_objs.sh could.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

get_filename_component(_gof2_root "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(CMAKE_CXX_COMPILER "${_gof2_root}/tools/verify/orbcc")
set(CMAKE_CXX_COMPILER_ID Clang)
set(CMAKE_CXX_COMPILER_WORKS TRUE)     # skip CMake's try_compile probe (cross compiler)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# The canonical match flags live in tools/verify/match_flags.sh (the ONE source,
# also used by relink.py's -fPIC rebuilds). Pull them in and make the repo-relative
# include paths absolute, because CMake compiles from the build directory.
execute_process(
    COMMAND bash -c ". '${_gof2_root}/tools/verify/match_flags.sh' >/dev/null 2>&1 && printf '%s' \"\$GOF2_MATCH_CXXFLAGS\""
    OUTPUT_VARIABLE _match_flags)
string(REPLACE "-Isrc" "-I${_gof2_root}/src" _match_flags "${_match_flags}")
string(REPLACE "-Ithird_party" "-I${_gof2_root}/third_party" _match_flags "${_match_flags}")
set(CMAKE_CXX_FLAGS "${_match_flags}" CACHE STRING "matching ARM compile flags (from match_flags.sh)" FORCE)

set(GOF2_MATCH ON CACHE BOOL "Matching ARM verification build (objdump differ + relink)")
set(GOF2_MATCH_OPT "-Oz" CACHE STRING
    "Optimization level for the matching build; the main flag to tune (-Oz/-O2/-Os/...)")
