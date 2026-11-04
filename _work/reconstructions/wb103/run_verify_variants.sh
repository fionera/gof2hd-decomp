#!/bin/bash
set -u
cd /home/debian/GalaxyOnFire2/wt/wb103
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
tag="$1"
so=/dev/shm/wb103/flagmatrix/$tag.so
out=/dev/shm/wb103/verify_logs/$tag.log
mkdir -p /dev/shm/wb103/verify_logs /dev/shm/wb103/verify_$tag
python3 tools/verify/verify.py --build-dir /dev/shm/wb103/verify_$tag --original "$ORIG" --ours "$so" --objdump "$OBJ" --fail-on-wrong-type --top 0 > "$out" 2>&1
