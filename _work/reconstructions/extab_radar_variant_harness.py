#!/usr/bin/env python3
"""Compile one Radar.cpp variant and compare its Radar::Radar body + LSDA.

Usage:
  SRC=/path/to/Radar_variant.cpp python3 extab_radar_variant_harness.py

Compiles with the exact match-build TU flags, extracts
`.text._ZN5RadarC2EP5Level` and `.ARM.extab.text._ZN5RadarC2EP5Level` from the
object, compares the body against a baseline object (same relocations, so
`diffbytes 0` means the linked body is still byte-exact) and the LSDA rows
against the original `.so`.

Set SRC to the repo's src/game/weapons/Radar.cpp to reproduce the baseline.
Requires the NDK r18b cache at <repo>/.cache/ndk/android-ndk-r18b.
"""
import os
import struct
import subprocess
import sys

ROOT = os.environ.get("GOF2_ROOT", "/home/debian/GalaxyOnFire2/wt/w7742")
ORIG = os.environ.get(
    "GOF2_ORIGINAL_SO",
    "/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so")
NDK = ROOT + "/.cache/ndk/android-ndk-r18b"
CXX = NDK + "/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++"
OBJ = "/tmp/radar_variant.o"
BASE_OBJ = "/tmp/radar_baseline.o"
FN = "_ZN5RadarC2EP5Level"
ORIG_FN_ADDR = 0x12D484
ORIG_FN_SIZE = 2488

FLAGS = [
    "-target", "armv7-none-linux-androideabi16", "-march=armv7-a", "-mthumb",
    "-mfpu=neon", "-mfloat-abi=softfp", "-std=c++11", "-fPIC", "-frtti",
    "-fstack-protector-strong", "-Oz", "-g", "-stdlib=libc++",
    "-isystem", NDK + "/sources/cxx-stl/llvm-libc++/include",
    "-isystem", NDK + "/sources/cxx-stl/llvm-libc++abi/include",
    "-isystem", NDK + "/sources/android/support/include",
    "--sysroot=" + NDK + "/sysroot",
    "-isystem", NDK + "/sysroot/usr/include",
    "-isystem", NDK + "/sysroot/usr/include/arm-linux-androideabi",
    "-D__ANDROID_API__=16", "-DANDROID", "-DNDEBUG",
    "-Wno-int-to-pointer-cast", "-Wno-int-to-void-pointer-cast",
    "-I" + ROOT + "/src", "-I" + ROOT + "/third_party/fmod/inc",
    "-I" + ROOT + "/third_party/gl", "-I" + ROOT + "/third_party/jni",
    "-I" + ROOT + "/third_party/libzip", "-I" + ROOT + "/third_party/crypto",
    "-ffunction-sections", "-funwind-tables",
]


def sections(data):
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    hdrs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize)
            for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    out = {}
    for h in hdrs:
        name, typ, flags, addr, off, size, link, info, align, entsize = h
        end = data.index(b"\0", stroff + name)
        out[data[stroff + name:end].decode()] = (off, size, addr)
    return out


def uleb(data, off):
    r = s = 0
    while True:
        b = data[off]
        off += 1
        r |= (b & 0x7F) << s
        if not (b & 0x80):
            return r, off
        s += 7


def sleb(data, off):
    r = s = 0
    b = 0
    while True:
        b = data[off]
        off += 1
        r |= (b & 0x7F) << s
        s += 7
        if not (b & 0x80):
            break
    if b & 0x40:
        r -= 1 << s
    return r, off


def enc_val(data, off, enc):
    if enc == 0xFF:
        return 0, off
    fmt = enc & 0x0F
    if fmt == 0x00:
        return struct.unpack_from("<I", data, off)[0], off + 4
    if fmt == 0x01:
        return uleb(data, off)
    if fmt == 0x02:
        return struct.unpack_from("<H", data, off)[0], off + 2
    if fmt == 0x03:
        return struct.unpack_from("<I", data, off)[0], off + 4
    if fmt == 0x09:
        return sleb(data, off)
    if fmt == 0x0A:
        return struct.unpack_from("<h", data, off)[0], off + 2
    if fmt == 0x0B:
        return struct.unpack_from("<i", data, off)[0], off + 4
    raise ValueError(hex(enc))


def decode_record(data, off):
    size = data[off + 7]
    i = off + 4 + (size + 1) * 4
    lps_enc = data[i]
    i += 1
    _, i = enc_val(data, i, lps_enc)
    tt_enc = data[i]
    i += 1
    if tt_enc != 0xFF:
        _, i = uleb(data, i)
    cs_enc = data[i]
    i += 1
    n, i = uleb(data, i)
    end = i + n
    rows = []
    while i < end:
        start, i = enc_val(data, i, cs_enc)
        length, i = enc_val(data, i, cs_enc)
        lp, i = enc_val(data, i, cs_enc)
        action, i = uleb(data, i)
        rows.append((start, length, lp, action))
    return rows


def orig_record():
    data = open(ORIG, "rb").read()
    secs = sections(data)
    ex, et = secs[".ARM.exidx"], secs[".ARM.extab"]
    for i in range(ex[1] // 8):
        eoff = ex[0] + i * 8
        eaddr = ex[2] + i * 8
        w0, = struct.unpack_from("<I", data, eoff)
        v = w0 & 0x7FFFFFFF
        if v & 0x40000000:
            v -= 0x80000000
        if eaddr + v != ORIG_FN_ADDR:
            continue
        w1, = struct.unpack_from("<I", data, eoff + 4)
        if w1 == 1 or (w1 & 0x80000000):
            continue
        v1 = w1 & 0x7FFFFFFF
        if v1 & 0x40000000:
            v1 -= 0x80000000
        rec = eaddr + 4 + v1
        return decode_record(data, et[0] + (rec - et[2]))
    raise SystemExit("original record not found")


def compile_one(src, out):
    r = subprocess.run([CXX] + FLAGS + ["-o", out, "-c", src],
                       capture_output=True, text=True)
    if r.returncode != 0:
        print(r.stderr[-3000:])
        raise SystemExit(1)


def main():
    src = os.environ.get("SRC", ROOT + "/src/game/weapons/Radar.cpp")
    compile_one(src, OBJ)
    if not os.path.exists(BASE_OBJ):
        compile_one(ROOT + "/src/game/weapons/Radar.cpp", BASE_OBJ)
    data = open(OBJ, "rb").read()
    secs = sections(data)
    tsec = secs[".text." + FN]
    body = data[tsec[0]:tsec[0] + tsec[1]]
    bdata = open(BASE_OBJ, "rb").read()
    bsec = sections(bdata)[".text." + FN]
    bbody = bdata[bsec[0]:bsec[0] + bsec[1]]
    if len(body) == len(bbody):
        bdiff = sum(1 for a, b in zip(body, bbody) if a != b)
    else:
        bdiff = -1
    esec = secs[".ARM.extab.text." + FN]
    ours = decode_record(data, esec[0])
    orig = orig_record()
    print(f"body vs baseline: size {len(body)}/{len(bbody)} diffbytes {bdiff}")
    diffs = 0
    for i in range(max(len(orig), len(ours))):
        a = orig[i] if i < len(orig) else None
        b = ours[i] if i < len(ours) else None
        if a != b:
            diffs += 1
            print(f"  ** {i:3d} orig {a}  ours {b}")
    print(f"row diffs: {diffs}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
