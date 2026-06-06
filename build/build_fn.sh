#!/usr/bin/env bash
# Compile a C++ TU with the NDK r18b toolchain and dump its Thumb disassembly.
# Usage:  NDK=/path/to/android-ndk-r18b ./build_fn.sh source.cpp [symbol]
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
. "$here/flags.sh"
src="$1"; sym="${2:-}"
out="${src%.cpp}.o"
"$CXX" $GOF2_CXXFLAGS -c "$src" -o "$out"
echo "== built $out =="
"$READELF" -p .comment "$out" 2>/dev/null | sed -n '2,4p' || true
echo "== disassembly =="
if [ -n "$sym" ]; then
  "$OBJDUMP" -d --no-show-raw-insn "$out" | awk -v s="$sym" '
    $0 ~ ("<.*"s".*>:"){p=1} p; p&&/^$/{exit}'
else
  "$OBJDUMP" -d --no-show-raw-insn "$out"
fi
