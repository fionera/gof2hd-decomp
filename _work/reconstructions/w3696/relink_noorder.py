#!/usr/bin/env python3
"""Relink the current marker objects with the exact gold line, minus the ordering file.

Usage:
  relink_noorder.py OUT.so [LINK_ORDER.rsp]

The default LINK_ORDER.rsp is cmake-build-match/exidx_markers/link_order.rsp.
The script uses the same crtbegin/archives/NEEDED inputs as CMakeLists.txt and
strips the result, so verify.py can compare it directly with the original.
"""
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
B = ROOT / "cmake-build-match"
NDK = ROOT / ".cache/ndk/android-ndk-r18b"
GOLD = NDK / "toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold"
STRIP = NDK / "toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip"
MARK = B / "exidx_markers"


def main():
    out = sys.argv[1]
    rsp = sys.argv[2] if len(sys.argv) > 2 else str(MARK / "link_order.rsp")
    args = [
        str(GOLD), "-shared", "-soname", "libgof2hdaa.so", "--build-id", "-z", "now",
        "--exclude-libs=libgcc.a", "-o", out,
        str(B / "crtbegin_so_renamed.o"),
        "-L", str(NDK / "platforms/android-16/arch-arm/usr/lib"),
        "@" + rsp,
        "--whole-archive", str(MARK / "archives/libz_gof2.a"),
        str(MARK / "archives/libzip_gof2.a"), "--no-whole-archive",
        "-l", "log", "-l", "GLESv2", "-l", "GLESv1_CM", "-l", "EGL", "-l", "android",
        str(ROOT.parent / "_work/bins/apk_libs/libfmodex.so"),
        str(ROOT.parent / "_work/bins/apk_libs/libfmodevent.so"),
        str(MARK / "archives/libcrypto_gof2.a"), "-l", "m",
        str(MARK / "archives/libc++abi.a"),
        str(MARK / "archives/libandroid_support.a"),
        str(MARK / "archives/libgcc.a"),
        "-l", "dl", "-l", "c",
        str(NDK / "platforms/android-16/arch-arm/usr/lib/crtend_so.o"),
    ]
    subprocess.run(args, check=True)
    subprocess.run([str(STRIP), "--strip-all", out], check=True)


if __name__ == "__main__":
    main()
