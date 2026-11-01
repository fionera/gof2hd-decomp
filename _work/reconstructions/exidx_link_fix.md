# `.ARM.exidx` link-order fix — the missing CANTUNWIND sentinels

Research pass on the 277-entry (2216-byte) `.ARM.exidx` deficit. The deficit is a **link-order
artifact**, not missing code. This note identifies the exact gold mechanism, gives a reproducible
relink that reproduces the original's exidx *size* (3568 vs 3566 entries), and measures the gate
cost. The change is **not gate-green** (byte_exact 1458 -> 1457), so only this note is committed.

- Worktree: `/home/debian/GalaxyOnFire2/wt/w3308`
- Branch: `research-w3308`, based on master `b676324a`
- Original: `../_work/bins/android_2.0.16_libgof2hdaa.so`
  (sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`)
- Baseline build: `cmake --preset default && cmake --build cmake-build-match --target check -j32`
  -> `byte_exact 1458  linked_exact 4493  size-exact 4522/4523  .text 0x1343d0  RATCHET PASS`
- All addresses below are real ELF addresses (Ghidra address minus `0x10000`).

## 1. Root cause: empty plain `.text` sections and gold's exidx fix-up

Gold 1.12 (`gold/arm.cc`, binutils 2.27) builds the output `.ARM.exidx` in
`Arm_output_section::fix_exidx_coverage()` by walking the **text** input sections in output order:

```c++
const Arm_exidx_input_section* exidx_input_section =
    arm_relobj->exidx_input_section_by_link(shndx);
// If this text section has no EXIDX section ... force an EXIDX_CANTUNWIND entry
if (exidx_input_section == NULL || exidx_input_section->has_errors())
  {
    exidx_fixup.add_exidx_cantunwind_as_needed();
    continue;
  }
```

`Arm_exidx_fixup::add_exidx_cantunwind_as_needed()` appends one `EXIDX_CANTUNWIND` entry pointing at
the end of the last exidx section it processed:

```c++
if (this->last_unwind_type_ != UT_EXIDX_CANTUNWIND
    && this->last_input_section_ != NULL)
  {
    Relobj* relobj = this->last_input_section_->relobj();
    unsigned int text_shndx = this->last_input_section_->link();
    ... new Arm_exidx_cantunwind(relobj, text_shndx) ...
  }
```

Every object compiled with `-ffunction-sections` still has a **plain `.text` section** (size 0) in
addition to its `.text.SYM` sections. That empty `.text` has no `.ARM.exidx` companion, so gold sees
it as a "text section without exidx" and emits a CANT at the end of the previous function.

The shipped `cmake-build-match/section_order.txt` lists `.text` **once** (near the libcrypto block).
All 281 empty `.text` sections therefore get the same ordering index and are grouped together
(`Output_section::Input_section_sort_section_order_index_compare` keeps input order for ties), so
gold emits only **6** CANT entries. The original was linked without an ordering file: its per-TU
`.text`/`.ARM.exidx` sections are interleaved in object order, so the compiler-emitted per-TU
trailing CANT entries survive, giving **272** CANT entries.

This also explains the audit's no-ordering-file result (3587 entries / 275 CANT): the CANTs are
present in the objects' empty `.text` sections; the ordering file groups them away.

Two audit experiments are red herrings:

- The "ordered + `.ARM.exidx.*` listed" run used names like `.ARM.exidx._ZN...`, but the real input
  sections are `.ARM.exidx.text._ZN...` (the `.text.` is part of the name). It matched nothing.
  Even with the correct names it cannot help: gold rebuilds the exidx output order from the *text*
  section order (`fix_exidx_coverage` iterates `sorted_text_sections`), not from the exidx input
  section order.
- `--no-merge-exidx-entries` only expands compact inline records (4536 entries, still 6 CANT); it
  does not synthesize the missing per-TU sentinels.

## 2. Fix: unique per-object empty-text markers

The ordering file can only distinguish sections by name, so each object's empty `.text` must be
renamed to a unique name and listed at that object's position in the original function order.

1. For every object (gof2 objects and every archive member), if its plain `.text` has size 0,
   `objcopy --rename-section .text=.text.tuend.<n>` and patch the renamed section's
   `sh_addralign` to **1**. Alignment 1 is required: the original empty `.text` has alignment 4, and
   inserting a 4-aligned zero-size section between 2-byte-aligned Thumb functions adds padding
   (`.text` grew to `0x1343e8` before the alignment patch).
2. In `tools/gen_section_order.py`, insert `.text.tuend.<n>` immediately **after** the object's last
   `.text.*` section in the generated order. The CANT address is then the end of that object's last
   function, i.e. the same place the original's per-TU compiler sentinel sits.
3. Add `-Tdata 0x20cb80` to the gold link. Without it, the extra exidx bytes push the read-only
   segment past the read-write segment's file-offset window; gold then moves the whole RW segment
   (`.fini_array`, `.data.rel.ro`, `.got`, `.data`, `.bss`) by `+0x1000`. `-Tdata 0x20cb80` anchors
   the RW segment at the baseline address, keeping `.got` at `0x20fcf4` and `.data` at `0x214000`
   (both match the original). It is neutral on the baseline build (byte_exact stays 1458).

The exact scripts used are in §5. They operate on copies of the objects/archives in `/tmp`, so they
do not touch the repo build.

## 3. Measured results

`readelf -SW` / exidx parser (8-byte entries, `word2 == 1` is CANTUNWIND):

| link | `.text` | `.ARM.exidx` | entries | CANT | dups | `.rodata` | `.got` | `.data` | byte | linked | size-exact | data |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| original | `0x1337c8` | `0x6f70` | 3566 | 272 | 194 | `0x1c9e10` | `0x20fcf4` | `0x214000` | — | — | — | — |
| baseline (shipped) | `0x1343d0` | `0x66c8` | 3289 | 6 | 1 | `0x1ca180` | `0x20fcf4` | `0x214000` | **1458** | 4493 | 4522/4523 | 843/843 |
| markers only | `0x1343d0` | `0x6f80` | 3568 | 268 | 213 | `0x1caa40` | `0x210cf4` | `0x215000` | **1414** | 4493 | 4522/4523 | 843/843 |
| markers + `-Tdata 0x20cb80` | `0x1343d0` | `0x6f80` | 3568 | 268 | 213 | `0x1caa40` | `0x20fcf4` | `0x214000` | **1457** | 4493 | 4522/4523 | 843/843 |

Other sections with `-Tdata`: `.ARM.extab` `0x1a4790` size `0x262a4` (original `0x1a3b78` /
`0x26298`), `.bss` `0x216980` size `0x69d8` (original `0x216970` / `0x69ac`).

The exidx structure is now essentially the original's: **3568 vs 3566 entries**, **268 vs 272
CANT**, compact 859 vs 856, extab refs 2441 vs 2438. Real entries: original 3294, baseline 3283,
new 3300. The net +2 entries is `-4 CANT +6 real`: the markers sit at our object boundaries, so a
few original per-TU sentinels are missing (4) while some adjacent real entries that the original
merged are kept separate (6). See §4.

`sodiff` is unchanged: original 5370 / ours 5370, extra 0, missing 0, imports 207.

## 4. Why the gate drops, and why it is not committed

The exidx fix necessarily grows the read-only segment by `0x8b8` (the missing entries). With
`.text` pinned at `0x1343d0`, `.rodata` must move from `0x1ca180` to `0x1caa40` (`+0x8c0`).

- **Without `-Tdata`**: `.rodata` crosses the RO/RW file-offset boundary, so gold shifts the whole
  RW segment by `+0x1000`. `.got` moves `0x20fcf4 -> 0x210cf4`. 44 functions with GOT-relative
  literal-pool deltas lose `byte_exact` (1458 -> 1414).
- **With `-Tdata 0x20cb80`**: the RW segment stays put, recovering 43 of those 44. The one remaining
  loss is `_ZN4Item27canBeInstalledMultipleTimesEv`. Its literal pool word is an `R_ARM_REL32` to
  the local array `_ZL33canBeInstalledMultipleTimesBySort` in `Item.cpp`'s `.rodata` (object offset
  `0x108`). The literal changes by exactly the `.rodata` base shift (`0x0011e138 -> 0x0011e9f8`,
  `+0x8c0`). To recover it, `Item.cpp`'s `.rodata` block would have to move `0x8c0` earlier within
  `.rodata` (e.g. rename that `.rodata` input section and place it earlier in the ordering file);
  not attempted here.

Ratchet output with the `-Tdata` link:

```text
verify: byte 1457  linked 4493  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 1458 -> 1457
RATCHET FAIL
```

So the change is a **1-function regression** and cannot be committed without
`--update-baseline`, which the task forbids. Commit the note only.

### CANT address alignment

The CANT addresses are at *our* object boundaries, not the original's TU ends, because our `.text`
layout has small per-region shifts (audit §2.5). Raw comparison against the original's CANT set:

- exact address matches: **7 of 268**
- within `0x20`: **27 of 268**
- the rest differ by the same per-region `.text` deltas (typically 0x30–0x60).

The audit's "166 of 193 objects" figure is after mapping each object's last function back to the
original address space; the raw addresses still differ. This is why the duplicate-address count is
213 vs the original's 194 and unique addresses 3355 vs 3372.

### File-offset side effect of `-Tdata`

`-Tdata 0x20cb80` makes the RW segment's `p_offset == p_vaddr` (`0x20cb80`), whereas the original and
the baseline have `p_offset = p_vaddr - 0x1000` (`0x20bb80`). The RO segment is now too large
(`0x20c187` file end) to fit the `-0x1000` window, so this is unavoidable with the current `.text`
size. Whole-file byte identity will still require shrinking `.text` by ~`0x8c0` (or a custom
section script), independent of the exidx fix.

## 5. Reproduction

### 5.1 Baseline

```sh
cd /home/debian/GalaxyOnFire2/wt/w3308
cmake --preset default
cmake --build cmake-build-match --target check -j32
# byte 1458  linked 4493  size-exact 4522/4523  .text 0x1343d0  RATCHET PASS
```

### 5.2 Build the marker objects and ordering file

Save as `/tmp/w3308_exidx_exp.py` (uses the NDK readelf/objcopy/ar):

```python
#!/usr/bin/env python3
import os, subprocess, shutil

W = "/home/debian/GalaxyOnFire2/wt/w3308"
NDK = W + "/.cache/ndk/android-ndk-r18b"
BIN = NDK + "/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin"
OBJCOPY = BIN + "/arm-linux-androideabi-objcopy"
READELF = BIN + "/arm-linux-androideabi-readelf"
AR = BIN + "/arm-linux-androideabi-ar"
WORK = "/tmp/exidx_exp"

def sh(cmd, **kw):
    r = subprocess.run(cmd, capture_output=True, text=True, **kw)
    if r.returncode != 0:
        print("FAILED:", cmd, r.stdout, r.stderr); raise SystemExit(1)
    return r.stdout

def sections(path):
    out = sh([READELF, "-SW", path])
    secs = []
    for line in out.splitlines():
        line = line.strip()
        if not line.startswith("["):
            continue
        try:
            rest = line.split("]", 1)[1].split()
            secs.append((rest[0], int(rest[4], 16)))
        except Exception:
            pass
    return secs

def text_sections(path):
    return [n for n, s in sections(path) if n == ".text" or n.startswith(".text.")]

def patch_align(path, name, align):
    data = bytearray(open(path, "rb").read())
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    shstr_off, = struct.unpack_from("<I", data, shoff + shstrndx*40 + 16)
    def sname(off):
        end = data.index(b"\0", shstr_off + off)
        return data[shstr_off+off:end].decode()
    for i in range(shnum):
        base = shoff + i*shentsize
        nm, = struct.unpack_from("<I", data, base)
        if sname(nm) == name:
            struct.pack_into("<I", data, base + 32, align)
            open(path, "wb").write(data)
            return
    raise SystemExit("section not found: " + name)

def rename_empty_text(src, dst, newname):
    if dict(sections(src)).get(".text") == 0:
        sh([OBJCOPY, "--rename-section", ".text=" + newname, src, dst])
        patch_align(dst, newname, 1)          # no alignment padding in .text
        return True
    shutil.copyfile(src, dst)
    return False

def main():
    os.makedirs(WORK + "/objs", exist_ok=True)
    os.makedirs(WORK + "/ars", exist_ok=True)
    objs = [l.strip() for l in open(W + "/cmake-build-match/link_order.rsp") if l.strip()]
    newobjs, markers = [], []
    for i, o in enumerate(objs):
        dst = f"{WORK}/objs/g{i:03d}_{os.path.basename(o)}"
        if rename_empty_text(o, dst, f".text.tuend.g{i:03d}"):
            markers.append((f".text.tuend.g{i:03d}", text_sections(o)))
        newobjs.append(dst)
    open(WORK + "/link_order_new.rsp", "w").write("\n".join(newobjs) + "\n")

    archives = [
        (W + "/third_party/zlib/libz_gof2.a", "libz_gof2.a"),
        (W + "/third_party/libzip/libzip_gof2.a", "libzip_gof2.a"),
        (W + "/third_party/openssl/libcrypto_gof2.a", "libcrypto_gof2.a"),
        (NDK + "/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a", "libc++abi.a"),
        (NDK + "/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a", "libandroid_support.a"),
        (NDK + "/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a", "libgcc.a"),
    ]
    newarchives = []
    for ai, (a, base) in enumerate(archives):
        d = f"{WORK}/ars/a{ai}"; os.makedirs(d, exist_ok=True)
        members = sh([AR, "t", a]).split()
        newmembers = []
        for mi, m in enumerate(members):
            sh([AR, "x", a, m], cwd=d)
            src = os.path.join(d, m)
            dst = os.path.join(d, f"m{mi:03d}_{m}")
            if rename_empty_text(src, dst, f".text.tuend.a{ai:03d}m{mi:03d}"):
                markers.append((f".text.tuend.a{ai:03d}m{mi:03d}", text_sections(src)))
            newmembers.append(dst)
        out = f"{d}/{base}"
        sh([AR, "rcs", out] + newmembers)
        newarchives.append(out)
    open(WORK + "/archives.txt", "w").write("\n".join(newarchives) + "\n")

    order = [l.rstrip("\n") for l in open(W + "/cmake-build-match/section_order.txt")]
    pos = {}
    for i, n in enumerate(order):
        pos.setdefault(n, []).append(i)
    inserts = {}
    for marker, tsecs in markers:
        idxs = [i for t in tsecs if t != ".text" for i in pos.get(t, [])]
        if idxs:
            inserts.setdefault(max(idxs), []).append(marker)
    out = []
    for i, n in enumerate(order):
        out.append(n)
        out.extend(inserts.get(i, []))
    # archive members with no listed function section: keep their markers after the listed
    # sections (before the trailing '*' glob), as the original object order would.
    tail = [m for m, t in markers if not any(x in pos for x in t if x != ".text")]
    if tail:
        out[-1:-1] = tail
    open(WORK + "/section_order_new.txt", "w").write("\n".join(out) + "\n")
    print("markers", len(markers), "insert points", len(inserts), "order lines", len(out))

if __name__ == "__main__":
    import struct
    main()
```

Run it:

```sh
python3 /tmp/w3308_exidx_exp.py
# markers 974 insert points 265 order lines 5885
```

(974 markers exist across all archive members; only the members actually pulled into the link
contribute a CANT.)

### 5.3 Relink with the marker objects

Save as `/tmp/w3308_relink_new.sh`:

```sh
#!/bin/bash
set -e
W=/home/debian/GalaxyOnFire2/wt/w3308
NDK=$W/.cache/ndk/android-ndk-r18b
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
OUT=$1; shift
AR=/tmp/exidx_exp/archives.txt
ZLIB=$(sed -n 1p $AR); LIBZIP=$(sed -n 2p $AR); LIBCRYPTO=$(sed -n 3p $AR)
LIBCXXABI=$(sed -n 4p $AR); LIBAOS=$(sed -n 5p $AR); LIBGCC=$(sed -n 6p $AR)
cd $W/cmake-build-match
$GOLD -shared -soname libgof2hdaa.so --build-id -z now \
  --unresolved-symbols=ignore-all --exclude-libs=libgcc.a \
  "$@" \
  -o "$OUT" \
  crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @/tmp/exidx_exp/link_order_new.rsp \
  --whole-archive $ZLIB $LIBZIP --no-whole-archive \
  $LIBCRYPTO \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  $W/../_work/bins/apk_libs/libfmodex.so $W/../_work/bins/apk_libs/libfmodevent.so \
  $LIBCXXABI \
  -l m \
  $LIBAOS \
  $LIBGCC \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

```sh
# markers only (RW segment shifts +0x1000)
/tmp/w3308_relink_new.sh /tmp/exidx_new.so \
  --section-ordering-file=/tmp/exidx_exp/section_order_new.txt
# markers + RW anchor (the useful variant)
/tmp/w3308_relink_new.sh /tmp/exidx_tdata.so \
  --section-ordering-file=/tmp/exidx_exp/section_order_new.txt -Tdata 0x20cb80
```

### 5.4 Measure

```sh
cd /home/debian/GalaxyOnFire2/wt/w3308
readelf -SW /tmp/exidx_tdata.so | grep -E '\.text |\.ARM.exidx|\.ARM.extab|\.rodata|\.got |\.data |\.bss '
python3 tools/verify/verify.py --build-dir /tmp/verify_tdata \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/exidx_tdata.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
# compared 4523  avg 99.9%  byte_exact 1457  linked_exact 4493  fuzzy_perfect 4494

python3 tools/sodiff.py --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/exidx_tdata.so --allow tools/sodiff_allow.txt
# original: 5370  ours: 5370  extra: 0  missing: 0  SYMBOL PARITY
```

To run the ratchet against the relinked artifact without rebuilding:

```sh
mkdir -p /tmp/w3308_gate/verify
cp /tmp/exidx_tdata.so /tmp/w3308_gate/libgof2hdaa.so
cp /tmp/verify_tdata/report.json /tmp/w3308_gate/verify/report.json
cp cmake-build-match/CMakeCache.txt /tmp/w3308_gate/CMakeCache.txt
touch /tmp/w3308_gate/libgof2hdaa.so /tmp/w3308_gate/verify/report.json
python3 tools/ratchet.py --build-dir /tmp/w3308_gate --no-rebuild
# FAIL  byte_exact 1458 -> 1457
```

## 6. Suggested patch for the coordinator

If the 1-function regression is acceptable (or the `.rodata` block is moved in a follow-up), the
minimal build change is:

1. `tools/gen_section_order.py`: after building `order`, insert a `.text.tuend.<n>` marker after
   each object's last `.text.*` section (the script already walks the objects; keep the
   `(relobj, section)` association).
2. `CMakeLists.txt`: add a custom command that makes renamed copies of every gof2 object and every
   archive member with a zero-size `.text`, using
   `objcopy --rename-section .text=.text.tuend.<n>` + `sh_addralign = 1`; point `link_order.rsp`
   and the archive paths at the copies.
3. Add `-Tdata 0x20cb80` to the gold link line.
4. Re-run `check`; expect `byte 1457` until the `Item.cpp` `.rodata` block is moved.

The alternative (no `-Tdata`) is not usable: `byte 1414`.

## 7. Bottom line

- The 277-entry deficit is **266 CANT + 11 real**, and the CANTs are gold's per-TU sentinels
  generated from the empty plain `.text` section of every `-ffunction-sections` object.
- The shipped ordering file groups all `.text` sections at one index, collapsing 272 sentinels to 6.
- Renaming each empty `.text` uniquely and placing it at the object's boundary reproduces the
  original's exidx size almost exactly: **3568 vs 3566 entries, 268 vs 272 CANT**.
- `.text` stays exactly `0x1343d0`; `.ARM.exidx` becomes `0x6f80` vs the original `0x6f70`.
- The fix moves `.rodata` by `+0x8c0` and costs one `byte_exact` function
  (`Item::canBeInstalledMultipleTimes`, an `R_ARM_REL32` to a local `.rodata` array). With
  `-Tdata 0x20cb80` the gate is **byte 1457 / linked 4493**, one short of the 1458 baseline, so
  the change is not committed.

## 8. Build-system implementation and anchor-removal re-measurement (w3266)

Follow-up pass that turns §2's `/tmp` experiment into a reproducible build-system change and
re-measures the `MenuTouchWindow` anchor against the new `.rodata` address.

- Worktree: `/home/debian/GalaxyOnFire2/wt/w3266`
- Branch: `research-w3266`, based on master `0f2316f7`
- Original: `../_work/bins/android_2.0.16_libgof2hdaa.so`
  (sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`)

**Baseline note.** The task brief quotes byte 1415, but the actual baseline at this HEAD is
**byte 1458 / linked 4493 / size-exact 4522/4523 / `.text` 0x1343d0** (the 1415 figure is the
older w3304 audit HEAD). All numbers below are measured against 1458.

### 8.1 Implementation

New `tools/gen_exidx_markers.py` (called from a CMake custom command) makes renamed copies of every
link input:

1. Reads the gold response file produced by `gen_link_order.py` (200 gof2 objects).
2. Copies each object to `cmake-build-match/exidx_markers/objects/o<NNN>_<name>`.  If the copy's
   plain `.text` is size 0, `objcopy --rename-section .text=.text.tuend.o<NNN>` renames it and the
   script patches `sh_addralign` to 1 (a 4-aligned zero-size section between 2-aligned Thumb
   functions adds padding; without the patch `.text` grows by 0x18).
3. Unpacks every archive member (zlib, libzip, libcrypto, libc++abi, libandroid_support, libgcc),
   does the same rename to `.text.tuend.a<AA>m<MMM>`, and re-packs the archive under
   `cmake-build-match/exidx_markers/archives/`.
4. Writes `link_order.rsp` / `archives.rsp` listing the copies; the gold link uses those instead
   of the originals.

`tools/gen_section_order.py` now accepts `@response-file` inputs and separates
`.text.tuend.*` from the ordinary `.text.*` names.  After building the normal order it places each
marker **after the last of its owner's sections in the finished order** (not merely after the
owner's last dynsym-listed section: a weak/local section can be emitted at an earlier member's
position, and several markers then collapse onto one ordering index; gold emits one CANT per run,
so the sentinels would disappear again).  Markers of members with no listed section stay just
before the trailing `*`.

`CMakeLists.txt` adds the marker custom command, feeds the copies to `gen_section_order.py`,
points the link at the copies, and adds `-Tdata 0x20cb80` (the extra exidx bytes push the RO
segment past the RW file-offset window; without the anchor gold moves the whole RW segment by
+0x1000).

### 8.2 Measured table

`readelf -SW` plus an 8-byte exidx parser (`word2 == 1` is CANTUNWIND).  All addresses are real
ELF addresses.  "anchor removal" deletes the `mtwTextLayoutAnchor` body and the
`MenuTouchWindow_languageIds[11]` address element; "anchor stub" keeps the address element and
replaces the body with `{}` (2 bytes) so the `.rel.dyn` R_ARM_ABS32 relocation survives.

| link | `.text` | `.ARM.exidx` | entries | CANT | `.rodata` | `.rel.plt` | `.got` | `.data` | `.fini_array` | byte | linked | size-exact |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| original | `0x1337c8` | `0x6f70` | 3566 | 272 | `0x1c9e10` | `0x5751c` | `0x20fcf4` | `0x214000` | `0x20cba0` | — | — | — |
| baseline (HEAD) | `0x1343d0` | `0x66c8` | 3289 | 6 | `0x1ca180` | `0x5751c` | `0x20fcf4` | `0x214000` | `0x20cb80` | **1458** | 4493 | 4522/4523 |
| markers only | `0x1343d0` | `0x6fa8` | 3573 | 273 | `0x1caa60` | `0x5751c` | `0x210cf4` | `0x215000` | `0x20db80` | **1432** | 4493 | 4522/4523 |
| markers + `-Tdata 0x20cb80` (committed) | `0x1343d0` | `0x6fa8` | 3573 | 273 | `0x1caa60` | `0x5751c` | `0x20fcf4` | `0x214000` | `0x20cb80` | **1495** | 4493 | 4522/4523 |
| markers + anchor removal (full), no `-Tdata` | `0x133850` | `0x6fa0` | 3572 | 273 | `0x1c9ed0` | `0x57514` | `0x20fcf4` | `0x214000` | `0x20cba0` | **1400** | 4493 | 4522/4523 |
| markers + anchor removal (full) + `-Tdata` | `0x133850` | `0x6fa0` | 3572 | 273 | `0x1c9ed0` | `0x57514` | `0x20fcf4` | `0x214000` | `0x20cba0` | **1400** | 4493 | 4522/4523 |
| markers + anchor stub + `-Tdata` | `0x133850` | `0x6fa8` | 3573 | 273 | `0x1c9ed0` | `0x5751c` | `0x20fcf4` | `0x214000` | `0x20cb80` | **1473** | 4493 | 4522/4523 |

`-Tdata` is neutral once the anchor is removed (the RO segment fits the old RW window), and the
anchor-stub variant is identical with and without it (byte 1473 both ways).

### 8.3 What the anchor does

The anchor body is 0xb76 object bytes (0xb78 linked).  Removing it moves `.text` from
`0x1343d0` to `0x133850`, i.e. from +0xc08 to +0x88 versus the original.  `.rodata` moves from
`0x1caa60` to `0x1c9ed0` (+0xc0 versus the original `0x1c9e10`).  With the full removal the RW
segment lands on the original addresses (`.fini_array` `0x20cba0`, `.data.rel.ro` `0x20cbb0`,
`.got` `0x20fcf4`, `.data` `0x214000`); the stub keeps the pre-existing `-Tdata` anchor
(`.fini_array` `0x20cb80`).  So the layout does converge; the remaining +0x88 is the small
prologue anchors from `layout_anchor_audit_v2.md` §2.5, not the MenuTouchWindow anchor.

The byte_exact cost depends on whether the anchor's relocation is kept:

- **Full removal** also removes the `R_ARM_ABS32` in `.rel.dyn` that keeps the anchor alive.
  `.rel.dyn` shrinks by 8 bytes, which shifts `.rel.plt`/`.plt` from `0x5751c`/`0x5e7ec` to
  `0x57514`/`0x5e7e4`.  Every PLT call site then differs by 8, costing ~73 byte_exact
  (1495 -> 1400).
- **Anchor stub** (keep the array element, shrink the body to `{}`) preserves the relocation.
  `.rel.plt` stays at `0x5751c`, and the gate is **byte 1473 / linked 4493** — still above the
  1458 baseline, with `.text` 0x133850.

The 1495 -> 1473 difference (anchor present vs stub) is the `.rodata` literal-pool trade-off:
the anchor-present `.rodata` at `0x1caa60` nets 22 byte_exact (8 functions gained, 30 lost)
versus `0x1c9ed0`, even though `0x1c9ed0` is arithmetically closer to the original.

### 8.4 Gate

Committed build-system-only configuration (anchor present, markers + `-Tdata`):

```text
verify: byte 1495  linked 4493  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 1458 -> 1495 (lock with --update-baseline)
RATCHET PASS
```

`size-exact` stays 4522/4523; `drift` reports 0 fields across 0 classes; `lint` is clean.
`--update-baseline` was **not** used (per the task), so the committed baseline remains 1458 and the
ratchet reports the gain.

Anchor-stub configuration (needs the `MenuTouchWindow.cpp` edit, not committed by this research
pass):

```text
verify: byte 1473  linked 4493  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 1458 -> 1473 (lock with --update-baseline)
RATCHET PASS
```

### 8.5 Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3266
cmake --preset default
cmake --build cmake-build-match --target check -j32
# byte 1495  linked 4493  size-exact 4522/4523  .text 0x1343d0  .ARM.exidx 0x6fa8
```

The marker generation is automatic; its output is
`cmake-build-match/exidx_markers/{link_order.rsp,archives.rsp}` and the generated order is
`cmake-build-match/section_order.txt` (5885 lines: 4911 baseline + 974 markers).

To reproduce the anchor variants, edit `src/game/ui/MenuTouchWindow.cpp`:

- full removal: delete the forward declaration, the
  `reinterpret_cast<int>(mtwTextLayoutAnchor)` array element (change `[11]` to `[10]`), and the
  whole function body;
- stub: keep the declaration and array element, replace the body with `{}`.

Then `cmake --build cmake-build-match --target check -j32`.  For the "no `-Tdata`" rows, relink
the generated marker copies directly (the build-system link line minus `-Tdata`):

```sh
W=$PWD; NDK=$W/.cache/ndk/android-ndk-r18b
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
MD=$W/cmake-build-match/exidx_markers
$GOLD -shared -soname libgof2hdaa.so --build-id -z now \
  --unresolved-symbols=ignore-all --exclude-libs=libgcc.a \
  --section-ordering-file=$W/cmake-build-match/section_order.txt \
  -o /tmp/relinked.so \
  $W/cmake-build-match/crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @$MD/link_order.rsp \
  --whole-archive $MD/archives/libz_gof2.a $MD/archives/libzip_gof2.a --no-whole-archive \
  $MD/archives/libcrypto_gof2.a \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  $W/../_work/bins/apk_libs/libfmodex.so $W/../_work/bins/apk_libs/libfmodevent.so \
  $MD/archives/libc++abi.a -l m $MD/archives/libandroid_support.a $MD/archives/libgcc.a \
  -l dl -l c $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

### 8.6 Recommendation

Commit the build-system change (markers + `-Tdata`): it is gate-green at **byte 1495** (up 37 from
1458), leaves `.text`/size-exact unchanged, and moves `.ARM.exidx` from 3289/6 to **3573/273**
entries/CANT (original 3566/272).  The remaining exidx delta is +7 entries (6 real entries that
gold does not merge plus 1 CANT), not a missing-sentinel problem.

The anchor-stub variant is the layout-converging follow-up: **byte 1473**, `.text` 0x133850
(+0x88 vs original), `.rodata` 0x1c9ed0 (+0xc0), RW segment on the original addresses.  It is
gate-green but requires the `MenuTouchWindow.cpp` source edit, so it is left to the coordinator
(this research pass was confined to the build system).
