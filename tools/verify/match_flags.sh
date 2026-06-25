# Canonical compiler flags for the matching ARM build. Read by
# cmake/orbstack-ndk-arm.toolchain.cmake (CMAKE_CXX_FLAGS) and by relink.py's
# -fPIC rebuilds — keep this the ONE source.
#
# Toolchain: Android NDK r18b, clang 7.0.2, armeabi-v7a, libc++ (from the .so's
# .comment). These are the STARTING POINT for byte-matching; tune per the verify
# report. The big knob is GOF2_MATCH_OPT (opt level): ROADMAP found -Oz; the old
# flags.sh claimed -O2. Override it from the environment to A/B test.
#
# Paths below are paths INSIDE the OrbStack ubuntu machine (where the compiler
# runs); /opt is the Linux machine's own fs, /Users is mirrored from macOS.

: "${NDK:=/opt/android-ndk-r18b}"
: "${GOF2_MATCH_OPT:=-Oz}"
# Min API only gates libc declarations, not the recovered functions' codegen; 21
# is needed so libc++ <cmath> finds bionic's C99 math (16 fails to compile).
: "${GOF2_MATCH_API:=21}"

GOF2_MATCH_CXXFLAGS="\
-target armv7-none-linux-androideabi${GOF2_MATCH_API} \
-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp \
-fpic -frtti -fstack-protector ${GOF2_MATCH_OPT} \
-stdlib=libc++ \
-isystem ${NDK}/sources/cxx-stl/llvm-libc++/include \
-isystem ${NDK}/sources/cxx-stl/llvm-libc++abi/include \
-isystem ${NDK}/sources/android/support/include \
--sysroot=${NDK}/sysroot \
-isystem ${NDK}/sysroot/usr/include \
-isystem ${NDK}/sysroot/usr/include/arm-linux-androideabi \
-D__ANDROID_API__=${GOF2_MATCH_API} \
-Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast \
-Isrc -Ithird_party/fmod/inc -Ithird_party/gl -Ithird_party/jni -Ithird_party/libzip -Ithird_party/crypto"

export GOF2_MATCH_CXXFLAGS
