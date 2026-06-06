#!/usr/bin/env bash
# Extract a function's Thumb disassembly from the target libgof2hdaa.so.
# Usage: NDK=/path ./extract_target.sh <elf_vaddr_hex> <num_bytes>
#   elf_vaddr_hex is the ELF virtual address (Ghidra addr MINUS 0x10000).
#   (Thumb funcs: pass the even address; objdump --start-address handles it.)
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
. "$here/flags.sh"
SO="${SO:-$here/../bins/android_2.0.16_libgof2hdaa.so}"
start="0x$1"; n="${2:-64}"
end=$(printf '0x%x' $(( start + n )))
# --triple forces thumb decoding for the v7 code
"$OBJDUMP" -d --no-show-raw-insn --triple=thumbv7-none-linux-androideabi \
  --start-address="$start" --stop-address="$end" "$SO"
