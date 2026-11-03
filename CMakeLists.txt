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
file(GLOB_RECURSE _srcs CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")
add_library(gof2 OBJECT ${_srcs})
# Whole-.so byte/hash identity requires reproducing the original's (interleaved) .text function
# order, since every inter-function bl/blx and GOT/PLT relative offset depends on it. The original's
# layout is NOT one-class-per-file, so it cannot arise from our source organisation; instead we emit
# each function in its own section (-ffunction-sections) and hand gold a --section-ordering-file
# derived from the original's symbol addresses (see the link step). This keeps one-class-per-file,
# human-style source while letting the linker place functions in the original's exact sequence.
option(GOF2_MATCH_LAYOUT "Reproduce the original .text function order via section ordering" ON)
if(GOF2_MATCH_LAYOUT)
    target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)
endif()

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

set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/weapons/Radar.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=80;-mllvm;-unlikely-branch-weight=1")
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/menu/SpaceLounge.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=20;-mllvm;-unlikely-branch-weight=12")

# StarMap::drawOnScreenInfo's campaign-target arm (getStationEnumIndex(93) >= 0) is reached from
# both the station and system paths; the original's register allocation keeps `campaign` in r6
# only when that edge's block frequency crosses the allocator's spill-cost threshold.  The natural
# zero-compare heuristic (20:12) lands just below it, so weight the hint 7:1.
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/world/StarMap.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=7;-mllvm;-unlikely-branch-weight=1")

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

# crtbegin_so.o's code lives in a plain `.text` section.  The original (linked without an ordering
# file) places it directly after the `.text.startup` group, before the first app function; a plain
# `.text` entry in the ordering file would instead group it with every archive's plain `.text`
# (libcrypto, libgcc, ...).  Rename it to a unique section so the ordering file can place it
# exactly where the original has it.
set(_crtbegin "${CMAKE_BINARY_DIR}/crtbegin_so_renamed.o")
if(GOF2_MATCH_LAYOUT)
    add_custom_command(OUTPUT "${_crtbegin}"
        COMMAND ${GOF2_NDK_GCC_BINDIR}/objcopy
                --rename-section .text=.text.crtbegin ${_libdir}/crtbegin_so.o "${_crtbegin}"
        DEPENDS ${_libdir}/crtbegin_so.o
        COMMENT "Renaming crtbegin .text -> .text.crtbegin for the section-ordering file")
else()
    set(_crtbegin "${_libdir}/crtbegin_so.o")
endif()

# Object link order recovered from the original's .dynsym (gold emits symbols in input order).
set(_linkorder "${CMAKE_BINARY_DIR}/link_order.rsp")
add_custom_command(OUTPUT "${_linkorder}"
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/gen_link_order.py
            "${GOF2_ORIGINAL_SO}" ${GOF2_NDK_NM} "${_linkorder}" $<TARGET_OBJECTS:gof2>
    DEPENDS "${GOF2_ORIGINAL_SO}" ${CMAKE_SOURCE_DIR}/tools/gen_link_order.py gof2 $<TARGET_OBJECTS:gof2>
    COMMENT "Ordering link objects like the original"
    COMMAND_EXPAND_LISTS VERBATIM)

# `.ARM.exidx` per-TU sentinels.  Gold's fix_exidx_coverage() emits one CANTUNWIND entry for every
# text input section with no `.ARM.exidx` companion.  With -ffunction-sections every object still
# carries an empty plain `.text`; the original (un-ordered) link kept one sentinel per TU, but our
# ordering file lists `.text` once, so all 272 empty sections collapse to one index and only 6
# survive.  Copy every object/archive member, rename its empty `.text` to a unique `.text.tuend.*`
# marker (sh_addralign=1 so a 4-aligned zero-size section adds no padding; two mid-game
# markers use align 4 to reproduce the original TU boundary), and have
# gen_section_order.py place each marker after its owner's last section in the finished order.
set(_marker_dir "${CMAKE_BINARY_DIR}/exidx_markers")
set(_marker_stamp "${_marker_dir}/stamp")
set(_marker_objs "${_marker_dir}/link_order.rsp")
set(_marker_archives "${_marker_dir}/archives.rsp")
set(_marker_zlib "${_marker_dir}/archives/libz_gof2.a")
set(_marker_libzip "${_marker_dir}/archives/libzip_gof2.a")
set(_marker_openssl "${_marker_dir}/archives/libcrypto_gof2.a")
set(_marker_libcxxabi "${_marker_dir}/archives/libc++abi.a")
set(_marker_aos "${_marker_dir}/archives/libandroid_support.a")
set(_marker_libgcc "${_marker_dir}/archives/libgcc.a")

# Generate the section-ordering file from the original's function-symbol addresses, then feed it to
# gold so our per-function sections are laid out in the original's exact order.
set(_order "${CMAKE_BINARY_DIR}/section_order.txt")
set(_order_flag "")
set(_tdata_flag "")
if(GOF2_MATCH_LAYOUT)
    add_custom_command(OUTPUT "${_marker_stamp}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${_marker_dir}"
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/gen_exidx_markers.py
                "${_marker_dir}" ${GOF2_NDK_GCC_BINDIR}/objcopy ${GOF2_NDK_GCC_BINDIR}/ar
                "${_linkorder}" ${_openssl_out} ${_zlib_out} ${_libzip_out}
                ${GOF2_NDK_LIBCXXABI_A} ${GOF2_NDK_ANDROID_SUPPORT_A} ${GOF2_NDK_LIBGCC_A}
        COMMAND ${CMAKE_COMMAND} -E touch "${_marker_stamp}"
        DEPENDS "${_linkorder}" ${CMAKE_SOURCE_DIR}/tools/gen_exidx_markers.py
                ${_openssl_out} ${_zlib_out} ${_libzip_out} ${GOF2_NDK_LIBCXXABI_A}
                ${GOF2_NDK_ANDROID_SUPPORT_A} ${GOF2_NDK_LIBGCC_A}
        COMMENT "Renaming empty .text sections to unique .ARM.exidx markers"
        VERBATIM)
    add_custom_command(OUTPUT "${_order}"
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/gen_section_order.py
                "${GOF2_ORIGINAL_SO}" ${GOF2_NDK_NM} "${_order}"
                "@${_marker_objs}" "@${_marker_archives}"
        DEPENDS "${GOF2_ORIGINAL_SO}" ${CMAKE_SOURCE_DIR}/tools/gen_section_order.py
                gof2 gof2_libs "${_marker_stamp}"
        COMMENT "Generating section-ordering file from the original .so + object source order"
        COMMAND_EXPAND_LISTS VERBATIM)
    set(_order_flag "--section-ordering-file=${_order}")
    set(_link_objs "@${_marker_objs}")
    set(_link_zlib "${_marker_zlib}")
    set(_link_libzip "${_marker_libzip}")
    set(_link_openssl "${_marker_openssl}")
    set(_link_libcxxabi "${_marker_libcxxabi}")
    set(_link_aos "${_marker_aos}")
    set(_link_libgcc "${_marker_libgcc}")
    set(_marker_dep "${_marker_stamp}")
else()
    set(_link_objs "@${_linkorder}")
    set(_link_zlib "${_zlib_out}")
    set(_link_libzip "${_libzip_out}")
    set(_link_openssl "${_openssl_out}")
    set(_link_libcxxabi "${GOF2_NDK_LIBCXXABI_A}")
    set(_link_aos "${GOF2_NDK_ANDROID_SUPPORT_A}")
    set(_link_libgcc "${GOF2_NDK_LIBGCC_A}")
    set(_marker_dep "")
endif()

add_custom_command(OUTPUT "${_so}"
    # A shared-library link allows undefined imports by default, as the original's app-provided
    # imports require; the import set is gated by tools/ratchet.py (a typo'd callee would otherwise
    # become a silent new import). NO --allow-multiple-definition: a duplicate strong definition
    # must be a link error, not a silent first-wins pick.
    COMMAND ${GOF2_NDK_GOLD}
            -shared -soname libgof2hdaa.so --build-id
            # Gold's own strip, not a post-link GNU strip: the original's .shstrtab is in gold's
            # Stringpool order (the raw link order, debug names removed), while GNU strip rewrites
            # it in section-header order. `-s` reproduces the original .shstrtab byte-for-byte and
            # emits the same 27-section set (no .symtab/.strtab/.debug_*).
            -s
            # The original is fully RELRO-bound: DT_FLAGS=BIND_NOW, DT_FLAGS_1=NOW, and its .got
            # sits before .data inside PT_GNU_RELRO (gold only emits that order with -z now).
            # Evidence: readelf -d/-lW on the original; without -z now gold puts .got after .data.
            -z now
            --exclude-libs=libgcc.a
            ${_order_flag}
            -o "${_so}"
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
    # Depend on the object FILES (not just the gof2 target): an OBJECT library produces no single
    # artifact, so depending on the target alone does not re-link when a member object changes,
    # leaving a stale .so. Listing the objects makes the link re-run whenever any one of them does.
    DEPENDS gof2 gof2_libs $<TARGET_OBJECTS:gof2> ${_order} ${_linkorder} ${_crtbegin}
            ${_marker_dep} ${_openssl_out} ${_libzip_out} ${_zlib_out}
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
