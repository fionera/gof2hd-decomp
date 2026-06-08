#!/usr/bin/env bash
# Compile the decompiled engine for native arm64 macOS -> libgof2.dylib (platform symbols left
# undefined; the host provides them). Run on the Mac host: bash mac/build_game.sh
set -u; cd "$(dirname "$0")/.."
SDK=$(xcrun --show-sdk-path)
F="-target arm64-apple-macos11 -std=c++14 -O1 -fno-exceptions -fno-rtti -w -include mac/macshim.h -Wno-error -isysroot $SDK"
mkdir -p mac/obj; : > mac/arm64.log
compile(){ d=$(dirname "$1"); o="mac/obj/$(echo "$1"|sed 's#[/.]#_#g').o"
  clang++ $F -I"$d" -Iinclude -c "$1" -o "$o" 2>"$o.err" && echo OK || echo "FAIL $1"; }
export -f compile; export F
find work/classes -path '*/src/*.cpp' | xargs -P10 -I{} bash -c 'compile "$@"' _ {} > mac/arm64.log
echo "arm64 compile: OK=$(grep -c '^OK' mac/arm64.log) FAIL=$(grep -c '^FAIL' mac/arm64.log)"
