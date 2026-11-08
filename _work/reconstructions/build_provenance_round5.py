#!/usr/bin/env python3
"""build_provenance_round5.py -- reproduce the wave-5 toolchain/build-system audit.

Reads the shipped libgof2hdaa.so, the canonical match build, the original APK and the
NDK r18b package, and prints the evidence used by
_work/reconstructions/build-provenance-round5-toolchain.md:

  * the four byte-identical toolchain fingerprint sections (.comment,
    .note.android.ident, .note.gnu.gold-version, .ARM.attributes) and their decoded values;
  * the section-size / differing-byte table vs the canonical build;
  * the APK packaging metadata (AGP version, build date, versionName/Code, package,
    minSdkVersion/targetSdkVersion, ABIs);
  * the NDK package identity (source.properties, AndroidVersion.txt, clang/gold/strip).

Usage:
  python3 _work/reconstructions/build_provenance_round5.py \
      --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
      --ours cmake-build-match/libgof2hdaa.so \
      --apk "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" \
      --ndk .cache/ndk/android-ndk-r18b
"""
import argparse
import hashlib
import re
import struct
import subprocess
import sys
import zipfile
from pathlib import Path

SHT_NOBITS = 8
SHT_DYNSYM = 11


def elf_sections(path):
    """name -> (type, addr, offset, size, blob)."""
    d = Path(path).read_bytes()
    e_shoff = struct.unpack_from("<I", d, 0x20)[0]
    e_shentsize = struct.unpack_from("<H", d, 0x2E)[0]
    e_shnum = struct.unpack_from("<H", d, 0x30)[0]
    e_shstrndx = struct.unpack_from("<H", d, 0x32)[0]
    shstr_off = struct.unpack_from("<I", d, e_shoff + e_shstrndx * e_shentsize + 0x10)[0]
    out = {}
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        nm = struct.unpack_from("<I", d, o)[0]
        typ = struct.unpack_from("<I", d, o + 4)[0]
        addr, off, size = struct.unpack_from("<III", d, o + 0x0C)
        end = d.find(b"\0", shstr_off + nm)
        name = d[shstr_off + nm:end].decode("ascii", "replace")
        blob = b"" if typ == SHT_NOBITS else d[off:off + size]
        out[name] = (typ, addr, off, size, blob)
    return d, out


def build_id(data):
    m = re.search(rb"GNU\0", data)
    # .note.gnu.build-id: namesz=4 descsz=20 type=3 "GNU\0" <20 bytes>
    for off in range(0, len(data) - 16):
        if data[off:off + 16] == b"\x04\x00\x00\x00\x14\x00\x00\x00\x03\x00\x00\x00GNU\0":
            return data[off + 16:off + 36].hex()
    return None


def fingerprint(path):
    d, secs = elf_sections(path)
    print(f"== {path}")
    print(f"   size {len(d)}  sha256 {hashlib.sha256(d).hexdigest()}")
    print(f"   build-id {build_id(d)}")
    for name in (".comment", ".note.android.ident", ".note.gnu.gold-version", ".ARM.attributes"):
        if name in secs:
            blob = secs[name][4]
            print(f"   {name:26s} {len(blob):4d} B  sha256 {hashlib.sha256(blob).hexdigest()}")
    cm = secs.get(".comment")
    if cm:
        print("   .comment strings:", [s.decode("utf-8", "replace") for s in cm[4].split(b"\0") if s])
    ai = secs.get(".note.android.ident")
    if ai:
        b = ai[4]
        # namesz, descsz, type, "Android\0", api(u32), ndk[64], build[64]
        # note header 12 B + name "Android\0" (8 B) = 20; desc = api u32, ndk[64], build[64]
        api = struct.unpack_from("<I", b, 20)[0]
        ndk = b[24:88].split(b"\0")[0].decode()
        build = b[88:152].split(b"\0")[0].decode()
        print(f"   .note.android.ident: API {api}  ndk {ndk!r}  build {build!r}")
    gv = secs.get(".note.gnu.gold-version")
    if gv:
        parts = [x for x in gv[4].split(b"\0") if x]
        print("   .note.gnu.gold-version:", parts[-1].decode())
    at = secs.get(".ARM.attributes")
    if at:
        print("   .ARM.attributes:", at[4].hex())


def compare(orig, ours, names):
    do, so = elf_sections(orig)
    dn, sn = elf_sections(ours)
    print(f"\n== section diff {orig} vs {ours}")
    print(f"   {'section':22s} {'orig size':>10s} {'ours size':>10s} {'diff bytes':>11s}")
    for name in names:
        if name not in so or name not in sn:
            continue
        a, b = so[name][4], sn[name][4]
        diff = sum(1 for x, y in zip(a, b) if x != y) + abs(len(a) - len(b))
        print(f"   {name:22s} {so[name][3]:10d} {sn[name][3]:10d} {diff:11d}")


def axml_strings(d):
    def u16(o):
        return struct.unpack_from("<H", d, o)[0]

    def u32(o):
        return struct.unpack_from("<I", d, o)[0]

    o = 8
    sc, flags, ss = u32(o + 8), u32(o + 16), u32(o + 20)
    offs = [u32(o + 28 + 4 * i) for i in range(sc)]
    out = []
    for off in offs:
        p = o + ss + off
        if flags & 0x100:
            n = u16(p); p += 2
            if n & 0x8000:
                n = ((n & 0x7FFF) << 8) | d[p]; p += 1
            nb = d[p]; p += 1
            out.append(d[p:p + nb].decode("utf-8", "replace"))
        else:
            n = u16(p); p += 2
            if n & 0x8000:
                n = ((n & 0x7FFF) << 8) | u16(p); p += 2
            out.append(d[p:p + 2 * n].decode("utf-16le", "replace"))
    return out


def axml_manifest(d):
    def u16(o):
        return struct.unpack_from("<H", d, o)[0]

    def u32(o):
        return struct.unpack_from("<I", d, o)[0]

    strings = axml_strings(d)
    o = 8
    sz = u32(o + 4)
    p = o + sz
    out = {}
    while p < len(d):
        typ, hs, sz = u16(p), u16(p + 2), u32(p + 4)
        if typ == 0x0102:
            name = strings[u32(p + 20)] if u32(p + 20) < len(strings) else "?"
            astart, asize, acount = u16(p + 24), u16(p + 26), u16(p + 28)
            if name in ("manifest", "uses-sdk"):
                for i in range(acount):
                    a = p + 16 + astart + i * asize
                    an = u32(a + 4)
                    dtype, data = d[a + 15], u32(a + 16)
                    anm = strings[an] if an < len(strings) else "?"
                    val = strings[data] if dtype == 0x03 and data < len(strings) else data
                    out[f"{name}.{anm}"] = val
        p += sz
    return out


def apk_metadata(apk):
    print(f"\n== APK {apk}")
    with zipfile.ZipFile(apk) as z:
        names = z.namelist()
        print("   ABIs:", sorted({n.split('/')[1] for n in names if n.startswith("lib/")}))
        mf = z.read("META-INF/MANIFEST.MF").decode("utf-8", "replace")
        for line in mf.splitlines():
            if line.startswith(("Created-By:", "Built-By:")):
                print("  ", line)
        try:
            cb = z.read("assets/crashlytics-build.properties").decode("utf-8", "replace")
            for line in cb.splitlines():
                if line.startswith(("#", "version_name", "version_code", "package_name", "build_id")):
                    print("  ", line)
        except KeyError:
            pass
        man = axml_manifest(z.read("AndroidManifest.xml"))
        for k in ("manifest.versionCode", "manifest.versionName", "manifest.package",
                  "uses-sdk.minSdkVersion", "uses-sdk.targetSdkVersion"):
            if k in man:
                print(f"   {k} = {man[k]}")


def ndk_identity(ndk):
    ndk = Path(ndk)
    print(f"\n== NDK {ndk}")
    for rel in ("source.properties", "toolchains/llvm/prebuilt/linux-x86_64/AndroidVersion.txt"):
        p = ndk / rel
        if p.exists():
            print(f"   {rel}: {p.read_text().strip()!r}")
    for rel in ("toolchains/llvm/prebuilt/linux-x86_64/bin/clang",
                "toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld.gold",
                "toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip"):
        p = ndk / rel
        if p.exists():
            r = subprocess.run([str(p), "--version"], capture_output=True, text=True)
            print(f"   {p.name}: {r.stdout.splitlines()[0] if r.stdout else r.stderr.splitlines()[0]}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--original", required=True)
    ap.add_argument("--ours")
    ap.add_argument("--apk")
    ap.add_argument("--ndk")
    a = ap.parse_args()
    fingerprint(a.original)
    if a.ours:
        fingerprint(a.ours)
        compare(a.original, a.ours, [
            ".text", ".rodata", ".data", ".bss", ".dynsym", ".dynstr", ".hash",
            ".rel.plt", ".rel.dyn", ".ARM.exidx", ".ARM.extab", ".got", ".plt",
            ".data.rel.ro", ".dynamic", ".init_array", ".fini_array",
            ".note.android.ident", ".note.gnu.gold-version", ".note.gnu.build-id",
            ".comment", ".ARM.attributes", ".shstrtab", ".gnu.version",
            ".gnu.version_d", ".gnu.version_r",
        ])
    if a.apk:
        apk_metadata(a.apk)
    if a.ndk:
        ndk_identity(a.ndk)


if __name__ == "__main__":
    main()
