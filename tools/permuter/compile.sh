#!/usr/bin/env bash
# decomp-permuter compile script for GOF2 (NDK r18b clang 7.0.2, armeabi-v7a Thumb).
# Permuter invokes:  compile.sh <input.c> -o <output.o>
set -e
NDK="${NDK:-/opt/android-ndk-r18b}"
CXX="$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++"
IN="$1"; OUT="${3:-out.o}"
exec "$CXX" -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti \
     -I"$(dirname "$0")/../../include" -c "$IN" -o "$OUT"
