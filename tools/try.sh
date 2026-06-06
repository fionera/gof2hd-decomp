#!/usr/bin/env bash
# try.sh <src.cpp> <sym_substr> <elf_vaddr_hex> <nbytes> — compile one TU in the guest (NDK) to a
# unique temp .o and diff it against the target. The worker's iterate-and-check command.
set -e
src="$1"; sym="$2"; vaddr="$3"; n="${4:-48}"
b=$(basename "$src" .cpp | tr -c 'A-Za-z0-9_' _)
REPO=/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
T=/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64
orb run bash -lc "cd $REPO && \
  $T/bin/clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti -Iinclude \
    -c '$src' -o /tmp/try_$b.o 2>/tmp/try_$b.err \
  && python3 tools/gofdiff.py --so ../_work/bins/android_2.0.16_libgof2hdaa.so --vaddr $vaddr --n $n --obj /tmp/try_$b.o --sym '$sym' \
  || { echo 'COMPILE_ERROR:'; head -6 /tmp/try_$b.err; }"
