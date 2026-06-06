# GOF2 Android (libgof2hdaa.so, 2.0.16) build flags — best-effort reconstruction.
# Target toolchain (from the .so .comment): NDK r18b, clang 7.0.2, armeabi-v7a, libc++.
#
# These are the STARTING POINT for byte-matching. Matching is an iterative process:
# compile -> diff against target -> adjust flags/source -> repeat. The exact set
# (opt level, -mfpu, -fno-exceptions/-frtti, stack protector, ABI level) must be
# tuned per-function until the disassembly matches.

: "${NDK:?set NDK to the extracted android-ndk-r18b dir}"
TOOLCHAIN="$NDK/toolchains/llvm/prebuilt/linux-x86_64"
export CXX="$TOOLCHAIN/bin/clang++"
export OBJDUMP="$TOOLCHAIN/bin/llvm-objdump"
export READELF="$TOOLCHAIN/bin/llvm-readelf"

# VERIFIED flag set (matches the whole 376-fn regression set + flips 16 more vs -O2):
#   -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti
# Notes that mattered for the match:
#   * KEEP the frame pointer (do NOT pass -fomit-frame-pointer) — target emits `add r7,sp,#N`.
#   * -Oz (SIZE-optimized) — toolchain archaeology showed the target is size-optimized, not -O2;
#     -Oz keeps every known match exact and recovers FP/control-flow functions -O2 could not.
#   * arch v7+VFPv3+NEONv1 is the -march=armv7-a default (matches the target .ARM.attributes) —
#     no explicit -mfpu needed.
TARGET="armv7-none-linux-androideabi16"
ARCH_FLAGS="-march=armv7-a -mthumb"            # default fpu already = VFPv3+NEONv1 (matches target)
OPT_FLAGS="-Oz -fpic"                            # add -DNDEBUG for real builds
CXX_FLAGS=""                                      # add -stdlib=libc++ -frtti (+sysroot) for TUs using libc++/STL
export GOF2_CXXFLAGS="-target $TARGET $ARCH_FLAGS $OPT_FLAGS $CXX_FLAGS"
# For TUs that #include libc++/Android headers, also add:
#   --sysroot=$NDK/sysroot -isystem $NDK/sysroot/usr/include/arm-linux-androideabi -stdlib=libc++
