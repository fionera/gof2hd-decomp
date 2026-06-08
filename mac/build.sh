#!/usr/bin/env bash
# Build the native macOS host (standalone — the engine dylib is loaded at runtime via dlopen).
# Run on the Mac host (NOT the OrbStack guest): bash mac/build.sh ; then ./mac/gof2_host
set -e
cd "$(dirname "$0")/.."
clang++ -std=c++14 -ObjC++ -arch arm64 -O2 -DGL_SILENCE_DEPRECATION \
  -framework Cocoa -framework OpenGL \
  mac/host.mm -o mac/gof2_host
echo "built mac/gof2_host"
