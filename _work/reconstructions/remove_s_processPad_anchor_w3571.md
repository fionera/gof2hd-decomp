# Removing the `s_processPad` .bss anchor (w3571)

Status: research note + landed gate-neutral source cleanup. Worktree `wt/w3571`,
branch `wave87-w3571`, based on current master. All numbers are from this worktree
against `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 `-Oz`, image base 0x10000).

## TL;DR

The original's `TargetFollowCamera.cpp` TU owns a **16-byte, completely
unreferenced `.bss` object** at `0x2183a0..0x2183b0`. The tree reproduced it with
an invented external hidden `s_processPad[4]` anchor. The anchor can be replaced
by a real file-static object in the same TU with no gate change:

```
before: byte 4496  linked 4496  .text 0x1337c8  .bss 0x69ac  lint CLEAN  drift 0
after:  byte 4496  linked 4496  .text 0x1337c8  .bss 0x69ac  lint CLEAN  drift 0
```

The 16 bytes are **not alignment padding**: removing the anchor and adding
`alignas(16)` to `Mission::empty` still moves `Mission::empty` to `0x2183a0`
(0x2183a0 is already 16-aligned), shrinking `.bss` by 0x10 and losing 10
`byte_exact` functions. A real 16-byte object must exist in the preceding TU.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes/bases (all exact except the pre-existing `.ARM.extab` -8 residue):

| section | original | ours |
|---|---|---|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x26290 @ 0x1a3b78` |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` |
| `.data.rel.ro` | `0x2fe8 @ 0x20cbb0` | `0x2fe8 @ 0x20cbb0` |
| `.got` | `0x430c @ 0x20fcf4` | `0x430c @ 0x20fcf4` |
| `.data` | `0x296c @ 0x214000` | `0x296c @ 0x214000` |
| `.bss` | `0x69ac @ 0x216970` | `0x69ac @ 0x216970` |

## 2. The original `.bss` gap

The original's exported `.bss` symbols around the gap:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D -S --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | sort |
  awk '$1>="00218380" && $1<="002183c0"'
```

```
00218380 00000001 B _ZN7Globals19iap_hack_dlc1BoughtE
00218381 00000001 B _ZN7Globals19iap_hack_dlc2BoughtE
00218382 00000001 B _ZN7Globals19iap_hack_dlc3BoughtE
00218383 00000001 B _ZN5Radar10drawTargetE
00218384 00000004 B _ZN10StarSystem16orbitPlanetIndexE
002183b0 00000004 B _ZN7Mission5emptyE
002183b4 00000004 B _ZN3Hud11RADAR_WIDTHE
002183b8 00000004 B _ZN3Hud12RADAR_HEIGHTE
002183bc 00000004 B _ZN3Hud14wingmanCommandE
```

`StarSystem::orbitPlanetIndex` ends at `0x218388`; `Mission::empty` starts at
`0x2183b0`: a **0x28-byte hole**. The unstripped link fills it with:

```sh
/tmp/relink_map.sh /tmp/ours_unstripped.so /tmp/ours.map   # exact gold line + -Map, no strip
$NM -n -S /tmp/ours_unstripped.so | awk '$1>="00218380" && $1<="002183c0"'
```

```
00218384 00000004 B _ZN10StarSystem16orbitPlanetIndexE
00218388 00000014 b _ZL10CHAR_BRENT
0021839c 00000004 b _ZL12g_sw_charDef
002183a0 00000010 b s_processPad
002183b0 00000004 B _ZN7Mission5emptyE
```

and the gold map gives the input-section ownership:

```
.bss  0x00218388  0x14  .../o151_Radio.cpp.obj
.bss  0x0021839c  0x04  .../o158_StatusWindow.cpp.obj
.bss  0x002183a0  0x10  .../o159_TargetFollowCamera.cpp.obj
.bss  0x002183b0  0x04  .../o160_Mission.cpp.obj
```

`CHAR_BRENT` (Radio.cpp) is 0x14 and `g_sw_charDef` (StatusWindow.cpp) is 0x4,
leaving exactly 0x10 bytes in the `TargetFollowCamera.cpp` input section.

## 3. Why the 0x10 bytes belong to `TargetFollowCamera.cpp`

The original was linked without a section-ordering file, so its `.text` order is
its link order. The `.text` addresses show the TU order directly:

```sh
$NM -D -S --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | sort |
  awk '$1>="0015a000" && $1<="0015b660"'
```

```
0015a1c4 00000300 T _ZN12StatusWindow10OnTouchEndEii     # ends 0x15a4c4
0015a4c8 000001bc T _ZN18TargetFollowCameraC1Ej...        # starts 0x15a4c8
...
0015b644 00000006 T _ZN18TargetFollowCamera7setRollEf
0015b64c 00000124 T _ZN7MissionC1EiN11AbyssEngine6StringEPiiiii  # starts 0x15b64c
```

`StatusWindow` ends at `0x15a4c4`, `TargetFollowCamera` occupies
`0x15a4c8..0x15b64a`, and `Mission` starts at `0x15b64c`. There is no local
function or data-only TU between them. Gold concatenates input `.bss` sections
in the same link order, so the 0x10-byte input section between
`StatusWindow`'s `g_sw_charDef` and `Mission`'s `Mission::empty` is
`TargetFollowCamera.cpp`'s `.bss`.

## 4. The object is unreferenced in the original

A full capstone scan of the original `.text` (all 469,558 decoded Thumb
instructions) for any address in `0x2183a0..0x2183b0` finds **zero** hits. The
scan resolves:

* absolute literal-pool words,
* PIC deltas consumed by a matching `add rX, pc`,
* `adr`,
* `movw`/`movt` pairs,
* base-address loads from `.bss` followed by an immediate `add`/`sub` that
  reaches the range.

```python
# /tmp/scan_range_refs2.py, run as:
# python3 /tmp/scan_range_refs2.py 0x69440 0x19cc08 0x2183a0 0x2183b0
# insns 469558
# refs 0
```

There is also no 4-byte value in the range anywhere in the file, and no
relocation addend in the range:

```sh
python3 - <<'PY'
import struct
data=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
for v in range(0x2183a0,0x2183b0,4):
    print(hex(v), data.count(struct.pack('<I',v)))
PY
# 0x2183a0 0 / 0x2183a4 0 / 0x2183a8 0 / 0x2183ac 0
```

The only `.rel.dyn` addend in the whole `0x218384..0x2183b0` window is
`0x218388` (the `CHAR_IMAGES[0]` pointer to `CHAR_BRENT`). There is no
`.dynsym` symbol at `0x2183a0`.

So the original's `TargetFollowCamera.cpp` has a 16-byte **unreferenced**
file-static `.bss` object. Its source spelling cannot be recovered from the
stripped binary; any 16-byte unreferenced file-static object in that TU is
observationally equivalent.

## 5. It is a real object, not alignment padding

If the 0x10 bytes were padding for a 16-byte-aligned `Mission.cpp` `.bss`
section, then removing the anchor and adding `alignas(16)` to `Mission::empty`
would reproduce the original layout. It does not: `0x2183a0` is already
16-aligned, so the linker inserts no padding and `Mission::empty` moves to
`0x2183a0`.

## 6. Controlled A/B experiments

Each row is a full `cmake --build cmake-build-match --target check -j8` after
the listed source edit. No `--update-baseline` was used.

| # | source change | byte | linked | `.bss` | `Mission::empty` | verdict |
|---|---|---:|---:|---:|---|---|
| E0 | baseline (`s_processPad` external hidden) | **4496** | **4496** | `0x69ac` | `0x2183b0` | — |
| E1 | remove `s_processPad` definition + header extern | **4486** | 4496 | `0x699c` | `0x2183a0` | -10 byte |
| E2 | E1 + `alignas(16) Mission *Mission::empty;` | **4486** | 4496 | `0x699c` | `0x2183a0` | alignment cannot reproduce the gap |
| E3 | replace with `[[gnu::used]] static int s_cameraPad[4];` in `TargetFollowCamera.cpp` | **4496** | **4496** | `0x69ac` | `0x2183b0` | landed |

E1/E2 lose 10 `byte_exact` functions because every `.bss` address after
`0x2183a0` shifts by -0x10, changing the PIC literal-pool deltas of functions
that reference those globals. `linked_exact` stays 4496 because the instruction
streams and symbol targets are unchanged.

E3's final local `.bss` map is exactly the original's:

```
00218388 00000014 b _ZL10CHAR_BRENT
0021839c 00000004 b _ZL12g_sw_charDef
002183a0 00000010 b _ZL11s_cameraPad
002183b0 00000004 B _ZN7Mission5emptyE
```

The object is file-static with internal linkage and `[[gnu::used]]`, so it is
emitted without a `.dynsym` export and without the cross-TU hidden extern.

## 7. Landed change

`src/game/ship/TargetFollowCamera.cpp`:

```c++
// The original TargetFollowCamera TU carries a 16-byte, unreferenced .bss object
// between StatusWindow's g_sw_charDef and Mission::empty (0x2183a0..0x2183b0).
// No code, pointer or relocation in the original refers to it; a file-static
// [[gnu::used]] object is the smallest source shape that reproduces it without
// exporting a symbol.
[[gnu::used]] static int s_cameraPad[4];
```

`src/game/ship/TargetFollowCamera.h` loses the
`[[gnu::visibility("hidden")]] extern volatile int s_processPad[4];`
declaration, and `src/game/world/LevelScript.cpp` loses its stale
`s_processPad` comment.

Final gate:

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8
cmake --build cmake-build-match --target lint -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
original: 5370  ours: 5370  extra: 0  missing: 0 (wrong_type: 0  absent: 0)
SYMBOL PARITY ✓
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`.text`, `.bss` and every other section size/base are unchanged from the
baseline table in §1.

## 8. Proven vs still hypothesis

**Proven by measurement**

* The original `.bss` has a 0x28-byte hole between `StarSystem::orbitPlanetIndex`
  (`0x218388`) and `Mission::empty` (`0x2183b0`).
* `CHAR_BRENT` (Radio.cpp, 0x14) and `g_sw_charDef` (StatusWindow.cpp, 0x4)
  account for 0x18 of it; the remaining 0x10 bytes are the `.bss` input section
  of the `TargetFollowCamera.cpp` object (link order = `.text` order, and
  `TargetFollowCamera` is the TU between `StatusWindow` and `Mission`).
* No code, literal-pool, pointer, relocation or `.dynsym` reference to
  `0x2183a0..0x2183b0` exists in the original.
* The 0x10 bytes are not alignment padding: `alignas(16)` on `Mission::empty`
  does not reproduce them because `0x2183a0` is already 16-aligned.
* Removing the anchor without a replacement shrinks `.bss` by 0x10 and loses 10
  `byte_exact` functions (4496 -> 4486).
* A file-static `[[gnu::used]] int[4]` in `TargetFollowCamera.cpp` reproduces the
  exact `.bss` layout and keeps the gate at 4496/4496.

**Still hypothesis / not claimed**

* The original source spelling and purpose of the 16-byte object are
  unrecoverable from the stripped binary. It may have been a debug leftover, a
  `static` `Vector`/`float[4]`/`int[4]`, or a deliberately `used` static. The
  landed `s_cameraPad[4]` is an observationally equivalent placeholder, not a
  recovered original name.
* No strict gate improvement was available: `byte_exact` and `linked_exact` are
  already 4496/4496. The change is gate-neutral by construction.

## 9. Reproduction scripts

`/tmp/relink_map.sh` (exact gold line from `build.ninja`, with `-Map`, no strip):

```bash
#!/bin/bash
set -e
B=/home/debian/GalaxyOnFire2/wt/w3571/cmake-build-match
NDK=/home/debian/GalaxyOnFire2/wt/w3571/.cache/ndk/android-ndk-r18b
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
OUT=${1:-/tmp/ours_unstripped.so}
MAP=${2:-/tmp/ours.map}
"$GOLD" -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  --section-ordering-file=$B/section_order.txt -Map "$MAP" -o "$OUT" \
  $B/crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @$B/exidx_markers/link_order.rsp \
  --whole-archive $B/exidx_markers/archives/libz_gof2.a \
                 $B/exidx_markers/archives/libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  /home/debian/GalaxyOnFire2/wt/w3571/../_work/bins/apk_libs/libfmodex.so \
  /home/debian/GalaxyOnFire2/wt/w3571/../_work/bins/apk_libs/libfmodevent.so \
  $B/exidx_markers/archives/libcrypto_gof2.a -l m \
  $B/exidx_markers/archives/libc++abi.a \
  $B/exidx_markers/archives/libandroid_support.a \
  $B/exidx_markers/archives/libgcc.a -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

`/tmp/scan_range_refs2.py` (capstone scan for code references to a vaddr range;
matches the destination register of `ldr [pc]` and `add pc`):

```python
import sys, struct, capstone, re
sys.path.insert(0,'tools/verify')
from elf import Elf32
orig=Elf32('../_work/bins/android_2.0.16_libgof2hdaa.so')
text=next(s for s in orig.sections if s['name']=='.text')
data=orig.data[text['offset']:text['offset']+text['size']]
base=text['addr']
lo,hi=int(sys.argv[1],0),int(sys.argv[2],0)
target_lo,target_hi=(int(sys.argv[3],0),int(sys.argv[4],0)) if len(sys.argv)>4 else (0x216970,0x21d31c)
md=capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
md.skipdata=True
insns=list(md.disasm(data[lo-base:hi-base],lo))
def reg(s):
    m=re.match(r'\s*(r\d+|ip|sp|lr|pc)',s)
    return m.group(1) if m else None
refs=[]
for k,i in enumerate(insns):
    if i.mnemonic.startswith('ldr') and '[pc' in i.op_str:
        ops=i.op_str.split(','); dst=reg(ops[0])
        try: imm=int(i.op_str.split('#')[1].split(']')[0],0)
        except Exception: continue
        pool=i.address+4+imm; off=pool-base
        if not (0<=off<=len(data)-4): continue
        w=struct.unpack_from('<I',data,off)[0]
        if target_lo<=w<target_hi:
            refs.append((i.address,'abs',w,i.mnemonic+' '+i.op_str))
        for j in range(k+1,min(k+8,len(insns))):
            q=insns[j]
            if q.mnemonic in ('add','adds') and 'pc' in q.op_str:
                qops=q.op_str.split(',')
                if reg(qops[0])!=dst: continue
                tgt=(w+q.address+4)&0xffffffff
                if target_lo<=tgt<target_hi:
                    refs.append((i.address,'delta',tgt,
                                 i.mnemonic+' '+i.op_str+' / '+q.mnemonic+' '+q.op_str))
                break
print('insns',len(insns))
print('refs',len(refs))
for r in refs: print(hex(r[0]),r[1],hex(r[2]),r[3])
```

Run it as:

```sh
python3 /tmp/scan_range_refs2.py 0x69440 0x19cc08 0x2183a0 0x2183b0
# insns 469558
# refs 0
```
