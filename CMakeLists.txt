cmake_minimum_required(VERSION 3.20)

# The byte-matching ARM (armeabi-v7a, NDK r18b) toolchain compiles every TU in src/ the way the
# original libgof2hdaa.so was built, links the same objects + vendored archives into a whole .so,
# and compares it against the original. Apply the toolchain automatically so a plain `cmake -B build`
# configures the matching build.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake")
endif()

project(GalaxyOnFire2 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # compile_commands.json for clangd/CLion code intelligence

# The original release .so we are matching (ELF 32-bit ARM, armeabi-v7a softfp, stripped).
set(GOF2_ORIGINAL_SO "${CMAKE_SOURCE_DIR}/../_work/bins/android_2.0.16_libgof2hdaa.so"
    CACHE FILEPATH "The original libgof2hdaa.so to match against")
# The app's prebuilt FMOD Ex / FMOD Event shared libraries from the original APK
# (lib/armeabi-v7a/libfmodex.so, libfmodevent.so): link inputs only, as in the original link.
get_filename_component(_orig_dir "${GOF2_ORIGINAL_SO}" DIRECTORY)
set(GOF2_FMOD_LIBDIR "${_orig_dir}/apk_libs" CACHE PATH "Directory holding the APK's libfmodex.so/libfmodevent.so")

# One OBJECT library feeds both the per-function ASM verify (diff vs the original) and the whole-.so
# link (gold link below). The original .so is -fPIC, so these objects match it and link cleanly.
#
# The object link order is the original TU order. It is a plain checked-in source list
# (tools/source_order.txt), not something derived from the original .so at build time: gold emits
# symbols in input order, so this list is the only thing that places every function at its original
# address. Keep the list in the original TU order; a new TU must be inserted at its witnessed
# position (see _work/reconstructions/static_link_order_w4217.md).
file(STRINGS "${CMAKE_SOURCE_DIR}/tools/source_order.txt" _srcs)
list(TRANSFORM _srcs STRIP)
# Keep only source paths, so comment/blank lines (and CMake's ';' list splitting) cannot leak in.
list(FILTER _srcs INCLUDE REGEX "^src/")
add_library(gof2 OBJECT ${_srcs})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/tools/source_order.txt")

# Guard against adding a source file without choosing its link position: the explicit list must
# cover every C/C++ TU in src/.
file(GLOB_RECURSE _all_srcs CONFIGURE_DEPENDS RELATIVE "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")
set(_srcs_sorted ${_srcs})
list(SORT _srcs_sorted)
list(SORT _all_srcs)
if(NOT _srcs_sorted STREQUAL _all_srcs)
    message(FATAL_ERROR "tools/source_order.txt does not list exactly the src/ TUs; "
                        "add/remove the file at its original link position")
endif()
# Whole-.so byte/hash identity requires reproducing the original's .text function order, since
# every inter-function bl/blx and GOT/PLT relative offset depends on it. The original was compiled
# with -ffunction-sections (proven by the 193 empty-.text CANTUNWIND sentinels in its .ARM.exidx)
# and linked without a section-ordering file: its .text is plain object input order. The source
# tree reproduces the original TU membership and within-TU function order, so a plain link places
# every function at its original address (see
# _work/reconstructions/link_order_tu_reconstruction_w4210.md).
target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)

# ModStation::OnInitialize's lazy CutScene check is a plain pointer comparison.  LLVM's
# BranchProbabilityInfo pointer heuristic already gives the original 20:12 successor probability,
# so the source must NOT wrap it in __builtin_expect (which would override the heuristic with the
# 2000:1 default and lay the EH landing pads out in the wrong order).
# =================================================================================================
# Whole-.so link -- produce libgof2hdaa.so the original's shape
# =================================================================================================
# Vendored static libs (libcrypto/zlib/libzip subsets) the original statically linked, built with
# the downloaded NDK clang. Sets _openssl_out / _zlib_out / _libzip_out + target gof2_libs.
include("${CMAKE_SOURCE_DIR}/third_party/match_libs.cmake")


# Per-file branch-weight overrides.  Both are matching knobs, not original build flags.
#
# StarMap::drawOnScreenInfo's campaign-target arm (getStationEnumIndex(93) >= 0) is reached from
# both the station and system paths.  The branch is `icmp sgt i32 %x, -1`; the natural
# zero-compare heuristic is 20:12, and the default __builtin_expect weight is 2000:1 -- both lay
# the block out differently (0xd98 / 0xdac instead of the original 0xda0).  Only the 7:1 ratio
# reproduces the original's block frequencies and the campaign-in-r6 allocation.  Clang 7.0.2 has
# no __builtin_expect_with_probability, and no source-level comparison form yields 7:1 (see
# _work/reconstructions/branch-weight-overrides_w4218.md).
#
# SpaceLounge::drawLounge's single __builtin_expect (the `agent->isKnown()` width-add guard) feeds
# the EH landing-pad block frequencies.  At the default 2000:1 the landing pads are laid out in a
# different order and the two tail `b.n` selectors disagree with the original by 4 text bytes; a
# 7:1 weight restores the original ordering (raw fuzzy 95.538778 -> 95.676047, official 95.5 ->
# 95.7).  A plain (no-expect) guard is 1:1 and scores 95.061728, so the expect must stay.  See
# _work/reconstructions/spacelounge-drawlounge-wave112-w6015.md.
# The flag A/B harness (tools/flag_fingerprint.py) can suppress these per-file overrides so a
# global -mllvm branch-weight default can be tested: clang 7 rejects a repeated
# -likely-branch-weight on one command line, and the per-file property would duplicate it.
option(GOF2_NO_PER_FILE_WEIGHTS "Omit per-file branch-weight overrides (flag A/B harness)" OFF)
if(NOT GOF2_NO_PER_FILE_WEIGHTS)
    set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/world/StarMap.cpp"
        PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=7;-mllvm;-unlikely-branch-weight=1")
    set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/menu/SpaceLounge.cpp"
        PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=7;-mllvm;-unlikely-branch-weight=1")
endif()

# LevelScript.cpp: the PostRAMachineSink pass is a matching knob.  With the pass on (the clang
# default), LevelScript::process's 8-byte `mov r4,r3` / `mov r3,r4` reordering at +0x8d46 differs
# from the original by 2 more text bytes; disabling the pass for this TU alone leaves the function
# size (57,232 B), .ARM.extab (725 differing bytes) and every gate count unchanged and takes the
# raw .text diff 84,652 -> 84,650.  Found by the wave-164 round-5 flag sweep (about 2,000 per-TU
# cells); it is the only size-exact, byte_exact-neutral raw win in that sweep.  See
# _work/reconstructions/build-provenance-round5-flags.md.
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/world/LevelScript.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-disable-postra-machine-sink")

# Radar.cpp: PostRA machine LICM is a matching knob.  Disabling it leaves Radar::draw's size
# (9,908 B) and every gate count unchanged, moves 4 text bytes into agreement and 2 out, for a
# net raw .text diff 83,141 -> 83,139.  The instruction-index alignment moves +9 (1,446 -> 1,455);
# raw same-address bytes are the byte-identicality ground truth.  Found by the wave-166 round-6
# per-TU flag sweep.  See _work/reconstructions/build-provenance-round6-per-tu-flags.md.
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/weapons/Radar.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-disable-postra-machine-licm")

# Radar.cpp used to carry a per-file 80:1 branch-weight override.  Wave 100 (w5004) removed it:
# the landmark on-screen test now hints both the `onScreen` load and the first bounds comparison
# (`__builtin_expect(onScreen,1) && __builtin_expect(screenX > centerX - field_0x128,1) && ...`),
# which reproduces the 80:1 block layout at the default 2000:1 weight.  Radar::draw stays 9908 B
# and the official percentage stays 73.9 % (raw 73.9255 -> 73.8682, normalized stream 3488/3488);
# the remaining 0.057 raw points are the otherLock cold-block address (0x261e vs 0x2480).  See
# _work/reconstructions/radar-draw-pass15-w5004.md.

# MenuTouchWindow.cpp (5:1) used to carry a per-file override too.  It was a matching knob, not an
# original build flag: with the default 2000:1 __builtin_expect weight the linked .so is
# byte-identical (byte_exact/linked_exact unchanged and the official per-function percentage
# unchanged), so that override was removed.  SpaceLounge.cpp's old 20:12 override was likewise
# gate-neutral and was removed in wave 92; wave 112 re-added it as 7:1 because it is *not*
# score-neutral (95.5 -> 95.7 official) even though the gate counts are unchanged.

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
# The original link was gold *without* -s, followed by a GNU strip: the shipped APK library has
# bfd-normalized section headers (.rel.plt SHF_INFO_LINK/sh_info, INIT/FINI_ARRAY sh_entsize=4,
# unaligned NOBITS .bss sh_offset) and a build-id hashed over the *unstripped* link output.  Gold's
# -s instead hashes the stripped file and leaves gold's own header form.  We keep the unstripped
# gold output only long enough to recover its Stringpool .shstrtab order (bfd rewrites .shstrtab
# into section-header order); tools/reorder_shstrtab.py restores it.
set(_so_unstripped "${CMAKE_BINARY_DIR}/libgof2hdaa.unstripped.so")
set(_libdir "${GOF2_NDK_CRT_LIBDIR}")

# crtbegin_so.o is a plain link input; the original linked it directly, as we do.
set(_crtbegin "${_libdir}/crtbegin_so.o")

# The original was linked without a section-ordering file, so the object order alone reproduces
# the original .text layout now that TU membership and within-TU function order are faithful.
# $<TARGET_OBJECTS:gof2> is in tools/source_order.txt order (the original TU order).
set(_link_objs "$<TARGET_OBJECTS:gof2>")
set(_link_zlib "${_zlib_out}")
set(_link_libzip "${_libzip_out}")
set(_link_openssl "${_openssl_out}")
set(_link_libcxxabi "${GOF2_NDK_LIBCXXABI_A}")
set(_link_aos "${GOF2_NDK_ANDROID_SUPPORT_A}")
set(_link_libgcc "${GOF2_NDK_LIBGCC_A}")

add_custom_command(OUTPUT "${_so}"
    # A shared-library link allows undefined imports by default, as the original's app-provided
    # imports require; the import set is gated by tools/ratchet.py (a typo'd callee would otherwise
    # become a silent new import). NO --allow-multiple-definition: a duplicate strong definition
    # must be a link error, not a silent first-wins pick.
    COMMAND ${GOF2_NDK_GOLD}
            -shared -soname libgof2hdaa.so --build-id
            # Do NOT pass -s: the original's build-id is the SHA-1 of the unstripped link output
            # (gold zeroes the build-id note descriptor and hashes the whole file), so a gold -s
            # link can never reproduce it even with identical section contents.
            # The original is fully RELRO-bound: DT_FLAGS=BIND_NOW, DT_FLAGS_1=NOW, and its .got
            # sits before .data inside PT_GNU_RELRO (gold only emits that order with -z now).
            # Evidence: readelf -d/-lW on the original; without -z now gold puts .got after .data.
            -z now
            --exclude-libs=libgcc.a
            -o "${_so_unstripped}"
            ${_crtbegin} -L ${_libdir}
            ${_link_objs}
            --whole-archive ${_link_zlib} ${_link_libzip} --no-whole-archive
            -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
            ${GOF2_FMOD_LIBDIR}/libfmodex.so ${GOF2_FMOD_LIBDIR}/libfmodevent.so
            ${_link_openssl}
            -l m
            ${_link_libcxxabi}
            ${_link_aos}
            ${_link_libgcc}
            -l dl -l c
            ${_libdir}/crtend_so.o
    # The original was stripped with GNU binutils after the link.  bfd normalizes the section
    # headers (the 5 bytes the gold -s output gets wrong) and rewrites .shstrtab into section
    # order; reorder_shstrtab.py then restores gold's Stringpool order from the unstripped file.
    COMMAND ${GOF2_NDK_STRIP} --strip-all -o "${_so}" "${_so_unstripped}"
    COMMAND ${CMAKE_COMMAND} -E env python3
            "${CMAKE_SOURCE_DIR}/tools/reorder_shstrtab.py"
            "${_so_unstripped}" "${_so}"
    COMMAND ${CMAKE_COMMAND} -E remove "${_so_unstripped}"
    # Depend on the object FILES (not just the gof2 target): an OBJECT library produces no single
    # artifact, so depending on the target alone does not re-link when a member object changes,
    # leaving a stale .so. Listing the objects makes the link re-run whenever any one of them does.
    DEPENDS gof2 gof2_libs $<TARGET_OBJECTS:gof2> ${_crtbegin}
            ${_openssl_out} ${_libzip_out} ${_zlib_out}
            ${CMAKE_SOURCE_DIR}/tools/source_order.txt
            ${CMAKE_SOURCE_DIR}/tools/reorder_shstrtab.py
    COMMENT "Linking libgof2hdaa.so (gold + GNU strip) from gof2 objects + vendored archives"
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
            --allow "${CMAKE_SOURCE_DIR}/tools/sodiff_allow.txt"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Symbol-parity diff vs the original .so (allow: documented irreducible delta)")

# verify -- per-function ASM match of our linked .so vs the original (byte-exact / linked-exact /
# fuzzy %), plus the missing/wrong_type/extra symbol-set deltas.
add_custom_target(verify
    DEPENDS "${_so}"
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/verify/verify.py
            --build-dir ${CMAKE_BINARY_DIR}/verify
            --original "${GOF2_ORIGINAL_SO}" --ours "${_so}"
            --objdump ${GOF2_NDK_OBJDUMP}
            --fail-on-wrong-type
    # Sync the tracked repo-root snapshot in the same step, so it can never go stale again
    # (it drifted once when it was hand-copied per pass).
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/verify/report.json
            ${CMAKE_SOURCE_DIR}/report.json
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

# Field name/offset drift: every field_0xNN must sit at offset 0xNN (its offset in the original
# binary). Authoritative check via clang -fdump-record-layouts (works for non-standard-layout too).
add_custom_target(drift
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/layout.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "Field name/offset drift check (layout.py)")

# check -- the machine-checked no-degradation gate: relinks/re-verifies if stale, then compares
# byte_exact/linked_exact/stubs/parity/imports/lint against tools/verify_baseline.json and the
# tools/sodiff_allow.txt irreducible delta. This is what the pre-push hook runs.
add_custom_target(check
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/ratchet.py --build-dir ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} USES_TERMINAL
    COMMENT "No-degradation ratchet vs tools/verify_baseline.json")
