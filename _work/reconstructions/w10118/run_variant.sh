#!/bin/bash
set -uo pipefail
REPO=${REPO:-$(cd "$(dirname "$0")/../../.." && pwd)}
SCRATCH=${SCRATCH:-/dev/shm/w10118}
V=$SCRATCH/variants
TAG=$1; shift
"$REPO/_work/reconstructions/w10118/relink_variant.sh" "$V" "$TAG" "$@" >$SCRATCH/$TAG.sections 2>/dev/null || { echo "$TAG LINK FAIL"; exit 1; }
python3 "$REPO/tools/verify/verify.py" --build-dir $SCRATCH/verify-$TAG \
  --original ${ORIG:-/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so} \
  --ours "$V/$TAG.so" \
  --objdump "$REPO/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump" \
  --fail-on-wrong-type --top 0 > $SCRATCH/$TAG.verify 2>&1
grep -E "^verify:|^data:" $SCRATCH/$TAG.verify
python3 "$REPO/tools/sodiff.py" --original ${ORIG:-/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so} --ours "$V/$TAG.so" --allow "$REPO/tools/sodiff_allow.txt" > $SCRATCH/$TAG.sodiff 2>&1
grep -E "extra-beyond-allow|imports" $SCRATCH/$TAG.sodiff | head -2
cat $SCRATCH/$TAG.sections
