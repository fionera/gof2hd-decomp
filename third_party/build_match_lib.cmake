# build_match_lib.cmake -- CMake-native builder for one vendored static lib (run via `cmake -P`).
#
# Replaces third_party/{openssl,zlib,libzip}/build.sh. Builds the exact subset of
# libcrypto/zlib/libzip the original game statically linked, for armeabi-v7a with the
# downloaded NDK r18b C clang, into third_party/<lib>/lib<...>_gof2.a -- which the
# CMake `relink` target links into the verification libgof2hdaa.so.
#
# Invoked by third_party/match_libs.cmake with -D variables:
#   GOF2_LIB              openssl | zlib | libzip
#   GOF2_NDK_ROOT GOF2_NDK_CLANG GOF2_NDK_AR GOF2_NDK_GCC_BINDIR
#   TP_DIR                absolute path of third_party/
#
# The source trees are downloaded + extracted on first use (pinned versions/URLs below);
# the hand-written libzip lib/config.h is generated below. libcrypto is compiled with the real
# GCC 4.9 from NDK r13b (fetched once next to the r18b NDK), see the openssl branch.

cmake_minimum_required(VERSION 3.20)

set(SYSROOT "${GOF2_NDK_ROOT}/platforms/android-16/arch-arm")

# Common C flags shared by all three libs.
set(CC_BASE
    --target=armv7a-linux-androideabi16 -mfpu=neon -mfloat-abi=softfp
    --sysroot=${SYSROOT}
    -isystem ${GOF2_NDK_ROOT}/sysroot/usr/include
    -isystem ${GOF2_NDK_ROOT}/sysroot/usr/include/arm-linux-androideabi
    -fPIC -ffunction-sections -Wno-everything)

# ---- helpers -----------------------------------------------------------------------------
function(ensure_source dir url)
    if(NOT EXISTS "${dir}")
        get_filename_component(_parent "${dir}" DIRECTORY)
        get_filename_component(_name "${url}" NAME)
        set(_tar "${_parent}/${_name}")
        message(STATUS "[${GOF2_LIB}] fetching ${url}")
        file(DOWNLOAD "${url}" "${_tar}" STATUS _st)
        list(GET _st 0 _code)
        if(NOT _code EQUAL 0)
            list(GET _st 1 _msg)
            file(REMOVE "${_tar}")
            message(FATAL_ERROR "[${GOF2_LIB}] download failed: ${_msg}")
        endif()
        file(ARCHIVE_EXTRACT INPUT "${_tar}" DESTINATION "${_parent}")
        file(REMOVE "${_tar}")
    endif()
endfunction()

# compile(<out.o> <workdir> <extra-flags-list> <src>) -- run the NDK C clang.
function(compile out workdir extra src)
    execute_process(
        COMMAND ${GOF2_NDK_CLANG} ${CC_BASE} ${extra} -c ${src} -o ${out}
        WORKING_DIRECTORY "${workdir}" RESULT_VARIABLE _rc)
    if(NOT _rc EQUAL 0)
        message(FATAL_ERROR "[${GOF2_LIB}] compile failed: ${src}")
    endif()
endfunction()

function(archive out)                                          # archive(<out.a> <obj>...)
    file(REMOVE "${out}")                                      # rcs replaces but never removes
    execute_process(COMMAND ${GOF2_NDK_AR} rcs ${out} ${ARGN} RESULT_VARIABLE _rc)
    if(NOT _rc EQUAL 0)
        message(FATAL_ERROR "[${GOF2_LIB}] ar failed: ${out}")
    endif()
endfunction()

# ==========================================================================================
if(GOF2_LIB STREQUAL "zlib")
    # zlib 1.2.3 -- the public ABI the original exports (adler32/crc32_combine but no *_combine64,
    # no gz*). Flat .c set, no configure needed; the gz file-I/O layer is omitted (no gz* symbols).
    set(SRC "${TP_DIR}/zlib/zlib-1.2.3")
    ensure_source("${SRC}" "https://zlib.net/fossils/zlib-1.2.3.tar.gz")
    set(OBJDIR "${SRC}/_obj_gof2")
    file(MAKE_DIRECTORY "${OBJDIR}")
    set(units adler32 compress crc32 deflate infback inffast inflate inftrees trees uncompr zutil)
    set(objs "")
    # -mthumb: the original .so has all zlib functions in Thumb mode (clang defaults to ARM).
    foreach(u ${units})
        compile("${OBJDIR}/${u}.o" "${SRC}" "-Oz;-g;-fstack-protector-strong;-I.;-mthumb" "${SRC}/${u}.c")
        list(APPEND objs "${OBJDIR}/${u}.o")
    endforeach()
    archive("${TP_DIR}/zlib/libz_gof2.a" ${objs})

# ==========================================================================================
elseif(GOF2_LIB STREQUAL "libzip")
    # libzip 0.9.3 -- bracketed from the original's symbol set (pre-0.11 zip_add names,
    # _zip_err_str[] ends at ZIP_ER_DELETED). Needs zlib headers; relink links libzip before zlib.
    set(SRC "${TP_DIR}/libzip/libzip-0.9.3")
    set(ZSRC "${TP_DIR}/zlib/zlib-1.2.3")
    ensure_source("${ZSRC}" "https://zlib.net/fossils/zlib-1.2.3.tar.gz")
    ensure_source("${SRC}" "https://libzip.org/download/libzip-0.9.3.tar.gz")
    # Minimal config.h matching the Android (NDK r18b / API 16) feature set; replaces the
    # autotools-generated header. HAVE_MKSTEMP is set: the original zip_close calls libc
    # mkstemp@plt (mkstemp.c is still compiled, so _zip_mkstemp is exported regardless; the
    # original imports mkstemp@LIBC).
    # HAVE_STRUCT_TM_TM_ZONE: the original _zip_dirent_torrent_normalize calls time() +
    # localtime() and copies tm_gmtoff/tm_zone (bionic struct tm has both).
    file(WRITE "${SRC}/lib/config.h"
"/* Hand-written config.h for the GOF2 armeabi-v7a build (NDK r18b / API 16). */
#define HAVE_DLFCN_H 1
#define HAVE_FSEEKO 1
#define HAVE_FTELLO 1
#define HAVE_INTTYPES_H 1
#define HAVE_LIBZ 1
#define HAVE_MEMORY_H 1
#define HAVE_MKSTEMP 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_STRUCT_TM_TM_ZONE 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#define STDC_HEADERS 1
#define PACKAGE \"libzip\"
#define VERSION \"0.9.3\"
#define PACKAGE_VERSION \"0.9.3\"
")
    set(OBJDIR "${SRC}/lib/_obj_gof2")
    file(MAKE_DIRECTORY "${OBJDIR}")
    file(GLOB _csrc "${SRC}/lib/zip_*.c")
    list(APPEND _csrc "${SRC}/lib/mkstemp.c")
    set(objs "")
    # -mthumb: the original .so has all libzip functions in Thumb mode (clang defaults to ARM).
    foreach(c ${_csrc})
        get_filename_component(_n "${c}" NAME_WE)
        compile("${OBJDIR}/${_n}.o" "${SRC}/lib" "-Oz;-g;-fstack-protector-strong;-I.;-I${ZSRC};-mthumb" "${c}")
        list(APPEND objs "${OBJDIR}/${_n}.o")
    endforeach()
    archive("${TP_DIR}/libzip/libzip_gof2.a" ${objs})

# ==========================================================================================
elseif(GOF2_LIB STREQUAL "openssl")
    # OpenSSL 1.1.0 libcrypto subset: SHA-224/256 (+ ARMv4 asm), and armv4cpuid.S
    # (OPENSSL_atomic_add/cleanse, CRYPTO_memcmp, ARMv7/ARMv8 probes, wipe_cpu, instrument_bus).
    #
    # Version evidence: CRYPTO_memcmp sits between OPENSSL_cleanse and _armv7_neon_probe in the
    # original .text and is the 56-byte armv4cpuid.pl routine -- CRYPTO_memcmp only moved into
    # armv4cpuid in 1.1.0 (1.0.2 has it in C in cryptlib.c). 1.1.0a..1.1.0l and 1.1.1w produce
    # identical code for this subset; 1.1.0l (last 1.1.0) is pinned.
    #
    # Compiler evidence: the original's libcrypto objects are ARM-mode GCC code (subs rX,rY,#0 /
    # rsb idioms, push {r3,...}) and the .so's .comment carries "GCC: (GNU) 4.9.x 20150123
    # (prerelease)" next to clang 7.0.2. r18b's arm-linux-androideabi-gcc is only a clang wrapper,
    # so the real GCC 4.9 is fetched from NDK r13b (toolchain dir only). GCC builds differ between
    # NDKs: r11c/r13b reproduce SHA256_Update exactly, r16b/r17c do not (jump-threading change).
    # Flags = OpenSSL 1.1.0 `Configure android-armeabi` CFLAGS + -march=armv7-a (GCC defaults
    # to armv5te; the original uses movw/movt).
    string(REGEX MATCH "prebuilt/([^/]+)/bin" _m "${GOF2_NDK_CLANG}")
    set(_host "${CMAKE_MATCH_1}")
    get_filename_component(_ndkcache "${GOF2_NDK_ROOT}" DIRECTORY)
    set(_gccndk "android-ndk-r13b")
    set(_gcctc "${_ndkcache}/${_gccndk}/toolchains/arm-linux-androideabi-4.9/prebuilt/${_host}")
    set(GCC "${_gcctc}/bin/arm-linux-androideabi-gcc")
    if(NOT EXISTS "${GCC}")
        set(_zip "${_ndkcache}/${_gccndk}-${_host}.zip")
        message(STATUS "[openssl] fetching ${_gccndk} GCC 4.9 toolchain (~650MB zip, one time)")
        file(DOWNLOAD "https://dl.google.com/android/repository/${_gccndk}-${_host}.zip" "${_zip}"
             STATUS _st)
        list(GET _st 0 _code)
        if(NOT _code EQUAL 0)
            file(REMOVE "${_zip}")
            message(FATAL_ERROR "[openssl] ${_gccndk} download failed: ${_st}")
        endif()
        file(ARCHIVE_EXTRACT INPUT "${_zip}" DESTINATION "${_ndkcache}"
             PATTERNS "${_gccndk}/toolchains/arm-linux-androideabi-4.9/*")
        file(REMOVE "${_zip}")
        if(NOT EXISTS "${GCC}")
            message(FATAL_ERROR "[openssl] ${_gccndk} extraction incomplete: ${GCC}")
        endif()
    endif()

    set(SRC "${TP_DIR}/openssl/openssl-1.1.0l")
    ensure_source("${SRC}" "https://www.openssl.org/source/old/1.1.0/openssl-1.1.0l.tar.gz")
    find_program(PERL_EXECUTABLE perl REQUIRED)
    # Configure once (writes configdata.pm), then render opensslconf.h from its template the way
    # the generated Makefile does. Invoke through `perl` explicitly (no usable shebang).
    if(NOT EXISTS "${SRC}/include/openssl/opensslconf.h")
        execute_process(COMMAND ${PERL_EXECUTABLE} Configure android-armeabi no-shared
                        WORKING_DIRECTORY "${SRC}" OUTPUT_QUIET ERROR_QUIET RESULT_VARIABLE _rc)
        if(NOT _rc EQUAL 0)
            message(FATAL_ERROR "[openssl] perl Configure android-armeabi failed")
        endif()
        execute_process(COMMAND ${PERL_EXECUTABLE} -I. -Mconfigdata util/dofile.pl -oMakefile
                                include/openssl/opensslconf.h.in
                        WORKING_DIRECTORY "${SRC}" RESULT_VARIABLE _rc
                        OUTPUT_FILE "${SRC}/include/openssl/opensslconf.h.tmp")
        if(NOT _rc EQUAL 0)
            message(FATAL_ERROR "[openssl] opensslconf.h generation failed")
        endif()
        file(RENAME "${SRC}/include/openssl/opensslconf.h.tmp" "${SRC}/include/openssl/opensslconf.h")
    endif()
    set(OBJDIR "${SRC}/_obj_gof2")
    file(MAKE_DIRECTORY "${OBJDIR}")
    # perlasm -> .S (linux32 flavour), as the 1.1.0 build does.
    foreach(_pl "crypto/armv4cpuid.pl;armv4cpuid.S" "crypto/sha/asm/sha256-armv4.pl;sha256-armv4.S")
        list(GET _pl 0 _in)
        list(GET _pl 1 _out)
        execute_process(COMMAND ${PERL_EXECUTABLE} ${_in} linux32 ${OBJDIR}/${_out}
                        WORKING_DIRECTORY "${SRC}" OUTPUT_QUIET RESULT_VARIABLE _rc)
        if(NOT _rc EQUAL 0 OR NOT EXISTS "${OBJDIR}/${_out}")
            message(FATAL_ERROR "[openssl] perlasm failed: ${_in}")
        endif()
    endforeach()
    set(OSSL_FLAGS
        --sysroot=${SYSROOT}
        -isystem ${GOF2_NDK_ROOT}/sysroot/usr/include
        -isystem ${GOF2_NDK_ROOT}/sysroot/usr/include/arm-linux-androideabi
        -D__ANDROID_API__=16 -march=armv7-a
        -DDSO_DLFCN -DHAVE_DLFCN_H -DNDEBUG -DOPENSSL_THREADS -DOPENSSL_NO_DYNAMIC_ENGINE
        -DOPENSSL_PIC -DSHA256_ASM -DOPENSSL_USE_NODELETE
        -Wall -O3 -pthread -mandroid -fPIC -Wa,--noexecstack
        -I. -Icrypto/include -Iinclude -Icrypto -w)
    set(objs "")
    foreach(_s "${OBJDIR}/armv4cpuid.S" "${SRC}/crypto/sha/sha256.c" "${OBJDIR}/sha256-armv4.S")
        get_filename_component(_n "${_s}" NAME_WE)
        execute_process(COMMAND ${GCC} ${OSSL_FLAGS} -c ${_s} -o ${OBJDIR}/${_n}.o
                        WORKING_DIRECTORY "${SRC}" RESULT_VARIABLE _rc)
        if(NOT _rc EQUAL 0)
            message(FATAL_ERROR "[openssl] compile failed: ${_s}")
        endif()
        list(APPEND objs "${OBJDIR}/${_n}.o")
    endforeach()
    archive("${TP_DIR}/openssl/libcrypto_gof2.a" ${objs})

else()
    message(FATAL_ERROR "unknown GOF2_LIB '${GOF2_LIB}'")
endif()

message(STATUS "[${GOF2_LIB}] built lib for the match link")
