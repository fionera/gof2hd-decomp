#!/usr/bin/env bash
# Full reassembly: compile EVERY authored function and link a complete libgof2hdaa.so.
# Goal = a structurally complete recompiled binary (byte-exactness is a later finetune).
# Run in the guest: orb run bash build/probe/reassemble.sh
set -u
cd /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
TC=/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64
CXX="$TC/bin/clang++"
FLAGS="-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fstack-protector -fpic -frtti -Iinclude"
NPROC="$(nproc)"
OUT=build/full; rm -rf "$OUT"; mkdir -p "$OUT/obj"

# one .cpp per target function (work copies = the complete deduplicated set)
mapfile -t SRCS < <(find work/classes -path '*/src/*.cpp' | sort)
echo "compiling ${#SRCS[@]} sources on $NPROC cores..."

compile() {
  local s="$1" o
  o="build/full/obj/$(echo "$s" | sed 's#[/.]#_#g').o"
  if $CXX $FLAGS -c "$s" -o "$o" 2>"$o.err"; then echo "OK"; else echo "FAIL $s"; fi
}
export -f compile; export CXX FLAGS
printf '%s\n' "${SRCS[@]}" | xargs -P"$NPROC" -d '\n' -I{} bash -c 'compile "$@"' _ {} > "$OUT/compile.log"
nok=$(grep -c '^OK' "$OUT/compile.log"); nfail=$(grep -c '^FAIL' "$OUT/compile.log")
echo "compiled OK=$nok FAIL=$nfail"
grep '^FAIL' "$OUT/compile.log" | head -10

echo "linking $(ls "$OUT/obj"/*.o 2>/dev/null | wc -l) objects..."
NEEDED="-llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lfmodex -lfmodevent -lm -ldl"
$CXX -target armv7-none-linux-androideabi16 -shared -fuse-ld=lld \
  -Wl,-soname,libgof2hdaa.so -Wl,--build-id -Wl,--allow-shlib-undefined \
  -Wl,--allow-multiple-definition -nostdlib \
  "$OUT"/obj/*.o -o "$OUT/libgof2hdaa.so" 2>"$OUT/link.err"
if [ -f "$OUT/libgof2hdaa.so" ]; then
  echo "=== LINKED OK ==="
  ls -la "$OUT/libgof2hdaa.so" | awk '{print "  size:",$5,"bytes"}'
  echo "  target .so size: $(stat -c %s ../_work/bins/android_2.0.16_libgof2hdaa.so) bytes"
  echo "  defined funcs:   $(arm-linux-gnueabihf-readelf -sW "$OUT/libgof2hdaa.so" 2>/dev/null | grep -c 'FUNC.*GLOBAL.*DEFAULT.*[0-9]')"
  echo "  undefined syms:  $(arm-linux-gnueabihf-readelf -sW "$OUT/libgof2hdaa.so" 2>/dev/null | grep -c 'UND')"
else
  echo "=== LINK FAILED ==="; head -20 "$OUT/link.err"
fi
