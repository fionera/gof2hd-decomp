#!/usr/bin/env python3
"""relink.py - assemble the recovered objects into libgof2hdaa.so.

Final-assembly step toward symbol/byte parity (see docs/RELINK.md). The target was
linked with NDK r18b **GNU gold 1.12** against Android platform stub libs + libc++.
Those aren't extracted in every environment, so this script:
  1. collects every match-build object (cmake-build-match/verify/base/*.o),
  2. checks for the linker + sysroot stub libs,
  3. links libgof2hdaa.so with the right SONAME / NEEDED / --build-id if it can,
  4. otherwise prints exactly what's missing (so the gate is actionable, not silent).

After a successful link, run:  python3 tools/symdiff.py  (must show 0 extra / 0 missing)
"""
import os, sys, glob, shutil, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE = os.path.join(ROOT, "cmake-build-match", "verify", "base")
OUT = os.path.join(ROOT, "cmake-build-match", "libgof2hdaa.so")
NDK = os.environ.get("NDK", "/opt/android-ndk-r18b")

SONAME = "libgof2hdaa.so"
NEEDED = ["liblog.so", "libGLESv2.so", "libGLESv1_CM.so", "libEGL.so", "libandroid.so",
          "libfmodex.so", "libfmodevent.so", "libm.so", "libdl.so", "libc.so"]


def find_gold():
    for c in ("arm-linux-androideabi-ld.gold", "ld.gold", "arm-linux-gnueabihf-ld.gold"):
        p = shutil.which(c)
        if p:
            return p
    cand = os.path.join(NDK, "toolchains", "arm-linux-androideabi-4.9", "prebuilt")
    for sub in glob.glob(os.path.join(cand, "*", "arm-linux-androideabi", "bin", "ld.gold")):
        return sub
    return None


def main():
    objs = sorted(glob.glob(os.path.join(BASE, "*.o")))
    print(f"[relink] recovered objects: {len(objs)} in {os.path.relpath(BASE, ROOT)}")
    if not objs:
        print("[relink] BLOCKED: no objects. Build the match target first:")
        print("         cmake --preset match && python3 tools/verify/verify.py")
        return 2
    gold = find_gold()
    sysroot_lib = os.path.join(NDK, "platforms", "android-16", "arch-arm", "usr", "lib")
    blockers = []
    if not gold:
        blockers.append("GNU gold linker not found (need NDK r18b arm-linux-androideabi-ld.gold; "
                        "set $NDK or run tools/verify/setup.sh)")
    if not os.path.isdir(sysroot_lib):
        blockers.append(f"Android platform stub libs not found at {sysroot_lib} "
                        "(extract NDK platforms/ to resolve liblog/libGLESv2/... )")
    if blockers:
        print("[relink] BLOCKED toward a real link:")
        for b in blockers:
            print("   - " + b)
        print("[relink] (docs/RELINK.md: byte-identical needs gold 1.12 + version script + platform libs)")
        return 2
    cmd = [gold, "-shared", "-soname", SONAME, "--build-id", "-o", OUT]
    for n in NEEDED:
        cmd += ["-l", n[3:-3]]
    cmd += ["-L", sysroot_lib, *objs]
    print("[relink] linking:", " ".join(cmd[:8]), "...")
    r = subprocess.run(cmd)
    if r.returncode != 0:
        print("[relink] link failed")
        return r.returncode
    print(f"[relink] wrote {OUT}\n[relink] now run: python3 tools/symdiff.py")
    return 0


if __name__ == "__main__":
    sys.exit(main())
