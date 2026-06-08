#!/usr/bin/env bash
# reassemble.sh — compile EVERY authored function and link a complete libgof2hdaa.so.
# This closes the disassemble->reassemble loop for the whole game. Byte-exact matching of
# individual functions is a separate finetuning pass (see matches.tsv / make verify).
# Unrecovered data globals/vtables (PC-relative refs the decompiler couldn't resolve) are
# weak-stubbed so the corpus links; giving them real values is part of finetuning.
# Run in the guest:  orb run bash work/reassemble.sh   (or: orb run make reassemble)
set -u
cd "$(dirname "$0")/.."
TC="${NDK:-/opt/android-ndk-r18b}/toolchains/llvm/prebuilt/linux-x86_64"
CXX="$TC/bin/clang++"; CC="$TC/bin/clang"
FLAGS="-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fstack-protector -fpic -frtti -Iinclude"
NPROC="$(nproc 2>/dev/null || echo 4)"
OUT=build/full; rm -rf "$OUT"; mkdir -p "$OUT/obj"

mapfile -t SRCS < <( { find work/classes -path '*/src/*.cpp'; find work/runtime -name '*.cpp' 2>/dev/null; } | sort)
echo "[1/4] compiling ${#SRCS[@]} sources on $NPROC cores..."
compile() { local s="$1" o="build/full/obj/$(echo "$1" | sed 's#[/.]#_#g').o"
  $CXX $FLAGS -c "$s" -o "$o" 2>"$o.err" && echo OK || echo "FAIL $s"; }
export -f compile; export CXX FLAGS
printf '%s\n' "${SRCS[@]}" | xargs -P"$NPROC" -d '\n' -I{} bash -c 'compile "$@"' _ {} > "$OUT/compile.log"
echo "     compiled OK=$(grep -c '^OK' "$OUT/compile.log") FAIL=$(grep -c '^FAIL' "$OUT/compile.log")"
grep '^FAIL' "$OUT/compile.log" || true

echo "[2/4] enumerating undefined symbols (unrecovered globals/vtables/thunks)..."
$CXX -target armv7-none-linux-androideabi16 -shared -fuse-ld=lld -Wl,--allow-multiple-definition \
  -Wl,--error-limit=0 -Wl,--no-demangle -nostdlib "$OUT"/obj/*.o -o /tmp/probe.so 2>"$OUT/u.err" || true
grep -oE 'undefined symbol: \S+' "$OUT/u.err" | sed 's/undefined symbol: //' | sort -u > "$OUT/undef.txt"
echo "     $(wc -l < "$OUT/undef.txt") symbols to stub"

echo "[3/4] generating + compiling weak stubs..."
{ echo '.section .bss.stubs,"aw",%nobits'
  while read -r s; do printf '.weak %s\n.type %s,%%object\n.size %s,256\n%s:\n.zero 256\n' "$s" "$s" "$s" "$s"; done < "$OUT/undef.txt"
} > "$OUT/stubs.s"
$CC -target armv7-none-linux-androideabi16 -c "$OUT/stubs.s" -o "$OUT/stubs.o"

echo "[4/4] linking libgof2hdaa.so..."
$CXX -target armv7-none-linux-androideabi16 -shared -fuse-ld=lld \
  -Wl,-soname,libgof2hdaa.so -Wl,--build-id -Wl,--allow-multiple-definition \
  -Wl,--unresolved-symbols=ignore-all -nostdlib \
  "$OUT"/obj/*.o "$OUT/stubs.o" -o "$OUT/libgof2hdaa.so" 2>"$OUT/link.err"
if [ -f "$OUT/libgof2hdaa.so" ]; then
  b=$(stat -c %s "$OUT/libgof2hdaa.so"); t=$(stat -c %s ../_work/bins/android_2.0.16_libgof2hdaa.so 2>/dev/null || echo 0)
  fn=$(arm-linux-gnueabihf-readelf -sW "$OUT/libgof2hdaa.so" 2>/dev/null | grep ' FUNC ' | grep -v ' UND ' | wc -l)
  echo "=== REASSEMBLED $OUT/libgof2hdaa.so : $b bytes (target $t), $fn functions ==="
else echo "=== LINK FAILED ==="; head -20 "$OUT/link.err"; exit 1; fi
