# Per-TU compile-flag override for the optimization-level sweep.
#
# Usage (passed to cmake, together with -DCMAKE_PROJECT_INCLUDE=tools/per_tu_flags.cmake):
#   -DGOF2_AB_TU=src/game/ui/Layout.cpp    # repo-relative or src-relative TU path
#   -DGOF2_AB_FLAGS=-O2                    # flags appended after the canonical set
#
# CMAKE_PROJECT_INCLUDE is processed after project(), i.e. before generation, so the
# source-file property is in place when the compile command is written.  COMPILE_FLAGS is
# appended after the target/global flags (and after any COMPILE_OPTIONS), so an optimization
# level here overrides the canonical -Oz.
if(GOF2_AB_TU)
  if(GOF2_AB_TU MATCHES "^src/")
    set(_gof2_ab_tu "${CMAKE_SOURCE_DIR}/${GOF2_AB_TU}")
  else()
    set(_gof2_ab_tu "${CMAKE_SOURCE_DIR}/src/${GOF2_AB_TU}")
  endif()
  set_source_files_properties("${_gof2_ab_tu}"
      PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
endif()
