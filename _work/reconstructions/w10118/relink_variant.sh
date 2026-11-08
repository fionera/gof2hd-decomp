#!/bin/bash
# Relink the already-built gof2 objects with extra gold flags and emit a
# stripped+shstrtab-normalised .so, then print the section sizes.
# Usage: relink_variant.sh <outdir> <tag> [extra gold flags...]
# The base gold command is taken verbatim from cmake-build-match/build.ninja so
# the variant differs from the canonical link only by the extra flags.
set -euo pipefail
REPO=${REPO:-$(cd "$(dirname "$0")/../../.." && pwd)}
BUILD=${BUILD_DIR:-$REPO/cmake-build-match}
OUT=${1:?outdir}; TAG=${2:?tag}; shift 2
EXTRA=("$@")
mkdir -p "$OUT"
CMD=$(python3 - "$BUILD/build.ninja" <<'PY'
import sys,re
txt=open(sys.argv[1]).read()
m=re.search(r'COMMAND = (cd [^\n]*?ld\.gold[^\n]*?)(?: && |\n)', txt)
print(m.group(1))
PY
)
CWD=${CMD#cd }; CWD=${CWD%% &&*}
GOLDCMD=${CMD#* && }
UN=$OUT/$TAG.unstripped.so
FINAL=$OUT/$TAG.so
GOLDCMD=${GOLDCMD/ -o $BUILD\/libgof2hdaa.unstripped.so / -o $UN }
GOLDCMD=${GOLDCMD/--build-id/--build-id ${EXTRA[*]}}
if [ -n "${DROP:-}" ]; then GOLDCMD=${GOLDCMD//$DROP/}; fi
echo "+ $GOLDCMD" >&2
( cd "$CWD" && eval "$GOLDCMD" )
STRIP=$REPO/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip
"$STRIP" --strip-all -o "$FINAL" "$UN"
python3 "$REPO/tools/reorder_shstrtab.py" "$UN" "$FINAL"
rm -f "$UN"
RE=$REPO/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -SW "$FINAL" | awk '$2 ~ /^\.(text|rodata|ARM.exidx|ARM.extab|rel.plt|dynsym|data|bss)$/ {printf "%s %s\n",$2,$6}'
