# Near-miss trio wave 145 (w9019): MGame / MenuTouchWindow / ModStation —
# value-vs-address anchor sweep, volatile-reference layout, and the far-pitch
# scheduler/pool split

Status: **no landed `src/` change.** Worktree `wt/w9019`, base `eb13ada9b`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave144-w8918.md`, `near-miss-trio-wave143-w8815.md`,
`near-miss-trio-wave142-w8716.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes, re-verified on a clean tree)

| target | size | raw diff | hunks |
|---|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 1 (anchor CSE at the ccm/pt loads) |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 4 clusters (2 + 26 B) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 1 (near-call RA: yaw r4 vs r3) |

**Measurement trap (new).** `MenuTouchWindow.cpp` and `ModStation.cpp` are
linked *before* `MGame.cpp` in `tools/source_order.txt`.  While sweeping one
target, any size change in the other two shifts `MGame::OnUpdate`'s address
and makes `fdiff.py` report thousands of spurious raw bytes.  Always revert
the other two TUs before measuring MGame.

## 1. MGame::OnUpdate — the anchor must be a *value*, and every address form
##    is rematerialised to r5

The original keeps `&Globals::status` in `r9` from `17e08c` to the end of the
m23/m24/m122 block and emits four fresh `ldr.w r0,[r9]` loads in the m23
condition (`17e090`, `17e0a8`, `17e0b8`, `17e0c0`).  Our HEAD emits
`ldr.w r9,[r0]; ldr.w r5,[r9]; mov r0,r5` (anchor) and reuses `r5` at pt.
The 41 bytes are exactly that CSE/2-byte-shift.

New this wave: every *address* spelling (`Status **st = &Globals::status`,
`Status *&st = Globals::status`, `(*st)->` throughout the whole block or the
whole function) is rematerialised by the allocator into `r5`, giving
**11392 B** (the 48-byte shrink is the m23 block's ~24 `[r9]` 4-byte loads
becoming 2-byte `[r5]` loads).  Only a *pointer value* live across the ccm
call pins the address to `r9`.

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Status *st` at pt) | **41** | 11440 | |
| `Status **st = &Globals::status`, `(*st)->` whole block | 5148 | 11392 | r5 |
| same, whole `OnUpdate` | 5148 | 11392 | r5 |
| `Status *&st = Globals::status`, `st->` whole block | 5148 | 11392 | r5 |
| anchor used at ccm only | 5148 | 11392 | r5 |
| anchor used at mission only | 5129 | 11396 | |
| anchor used at field only | 4993 | 11436 | |
| anchor used at ccm+field | 4993 | 11436 | |
| anchor used at mission+field | 4966 | 11436 | |
| anchor used at m24 pt (live across m23) | 270 | 11440 | m23 still CSEs |
| `Status *volatile st` | 6076 | 11340 | stack traffic |
| `Status *st = *(Status *volatile *)&Globals::status` | 41 | 11440 | identical to HEAD |
| `Status *st = nullptr;` then `(st = Globals::status)->` at pt | 5148 | 11392 | |
| `Status *st;` then `(st = Globals::status)->` at pt | 5148 | 11392 | |
| anchor used at m23 body tail (`st->field_0x100`) | 6234 | 11524 | body re-rolls |

Conclusion: unchanged.  The original's `r9` is not evidence of a hidden
address local; it is the allocator's response to a value that occupies `r5`
across the condition.  Our compiler only produces that value via the anchor,
and the anchor is CSE'd with the ccm load.  Irreducible with clang 7.0.2.

## 2. MenuTouchWindow::OnTouchBegin — the 26-byte cluster is the layout
##    value's live range, and every B=dir shape loses 4 bytes

The original's case-15 block is fully direct:
`ldr r2,[r4]` (Globals::layout) → `buttonInsetX`; then `ldr r2,[sp,#16]` →
`Globals::layout` → `leftMargin` (r5) and `top` (r6); then
`ldr r0,[sp,#16]` → `Globals::layout` → `buttonInsetX`.  HEAD caches the
`Layout*` in `r6` across the first `GetImage2DWidth`, so `leftMargin` comes
from `r6`.  The original's colouring is `r4 = &Globals::layout`,
`r11 = &Globals::Canvas`, `[sp,#16] = &Globals::layout`; HEAD matches it.

New probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Layout &layout`, B=ref, C=dir) | **28** | 1896 | |
| inner `Layout *layout = Globals::layout;` (pointer) | 28 | 1896 | identical to HEAD |
| `Layout *volatile &layout = Globals::layout;` all `layout->` | 127 | 1896 | fresh loads, but r11=&layout, r5=&Canvas |
| `Layout *volatile &layout` A/B/C ref, D dir | 126 | 1896 | |
| `Layout *volatile &layout` A only | 357 | 1892 | |
| A=ref B=dir C=dir D=ref | 314 | 1888 | |
| A=ref B=dir C=ref D=ref | 288 | 1892 | |
| A=ref B=ref C=dir D=ref | 287 | 1892 | |
| A=dir B=dir C=dir D=ref | 314 | 1888 | |
| A=dir B=ref C=dir D=ref | 287 | 1892 | |
| A=dir B=dir C=ref D=ref | 288 | 1892 | |
| outer `layout` local used for A only | 1761 | 1908 | |
| outer `layout` local used for A+B | 1762 | 1912 | |
| y-sum reorder (`scroll+top+left`, `top+scroll+left`, `left+scroll+top`, nested) | 28 | 1896 | 2-byte cluster unchanged |

Conclusion: every shape with B direct shrinks to <=1892 and re-colours
`&Globals::Canvas` from `r11` to `r5`.  The original's 1896 needs the layout
value live across the first call, which is exactly the cached B.  The 2-byte
`ip`/`r4` cluster is insensitive to every sum reordering.

## 3. ModStation ctor — the faithful `far = pitch` is 43; the residual is
##    a scheduling + literal-pool pair, not a source-spelling choice

HEAD (`far = yaw`) = **11** raw / 1008 B, METRIC-ONLY.  Faithful
`far = pitch` = **43** raw / 1008 B.  The 43 bytes are exactly:

1. literal-pool order: original `Pitch, YawIPad, Yaw`; ours `Pitch, Yaw,
   YawIPad` (`ldr r3`/`ldr r4` swap + pool entries at `d56b4`/`d56b8`);
2. `str.w r8,[sp,#184]` and `mov r4,r2` (save pitch) are hoisted above the
   yaw load (ours `d5456`/`d545a`; original `d545c`/`d546e`).

New probes (all reverted, all 43 unless noted):

| variant | raw | size |
|---|---:|---:|
| HEAD (`far = yaw`) | **11** | 1008 |
| `far = pitch` | 43 | 1008 |
| near `yawTable[race]`, far `pitch` + ternary yaw | 43 | 1008 |
| `if (iPad) yawTable=YawIPad; else yawTable=Yaw;` | 43 | 1008 |
| `!iPad`, `iPad!=0`, `iPad==0`, `iPad>0`, `iPad!=0u` spellings | 43 | 1008 |
| `(iPad & 1)` spelling | 572 | 1008 |
| swap table declarations (yaw before pitch) | 46 | 1008 |
| pitch declared before yaw (float and int) | 43 | 1008 |
| int declarations swapped (yawBits first) | 744 | 1000 |
| `float pitch2 = pitch;` near call | 43 | 1008 |
| near `yawTable[race]`, far `pitch` + `yawTable[race]` | 580 | 984 |
| no `yaw` local, far `pitch` + `yawTable[race]` | 676 | 976 |
| direct `gHangarCamPitch[race]` both calls | 701 | 984 |
| direct pitch + ternary yaw | 670 | 1008 |

The literal-pool order is the backend's `select` materialisation order; it
does not follow the ternary operand order (InstCombine canonicalises every
spelling to `select (iPad==0), Yaw, YawIPad`).  The hoist is the same
pre-RA copy placement reported in waves 143/144.  HEAD stays for the metric;
the semantic fix is still `farKey, yaw` -> `farKey, pitch`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9019
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9019/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w9019/`: `fdiff.py`, `bdiff.py` (raw
byte-run report), `dd.py` (per-function disasm), `seqdiff.py` (aligned
instruction diff), `build.sh` (incremental link), `mgv2.py` / `mtwv.py` /
`mtwsum.py` / `modv.py` (variant drivers), `*.cpp.base` (pristine sources).

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave145-w9019.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
