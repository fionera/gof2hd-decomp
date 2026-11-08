# Near-miss trio wave 146 (w9216): ModStation ctor faithful `far = pitch` landed;
# MenuTouchWindow / MGame anchor+reference sweeps

Status: **one landed `src/` change** (ModStation ctor). Worktree `wt/w9216`,
base `31297fadc` (wave 145). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`).
Read with `near-miss-trio-wave145-w9019.md`, `near-miss-trio-wave144-w8918.md`,
`near-miss-trio-wave143-w8815.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
RATCHET PASS. `.rel.plt` byte-identical. File size exact.

## 0. Residual classification (raw same-address bytes, clean tree)

| target | size | raw before | raw after | hunks |
|---|---:|---:|---:|---|
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 28 | 4 clusters (2 + 26 B) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 (metric-only) | **43** (faithful) | literal-pool order + one pre-RA copy |
| `MGame::OnUpdate` | 11440 B | 41 | 41 | 1 (anchor CSE at ccm/pt) |

**Measurement trap re-confirmed.** `MenuTouchWindow.cpp` and `ModStation.cpp`
link before `MGame.cpp`.  The landed ModStation change is size-neutral
(1008 B), so `MGame::OnUpdate` keeps its address; the 41 B is a true diff,
not an address shift.

## 1. ModStation ctor — LANDED the faithful `far = pitch` (METRIC-ONLY trade)

The original's far `MatrixSetRotation` x argument is **pitch**, not yaw:
the near-call save at `0xd546e` is `mov r4,r2` where `r2` is the
`gHangarCamPitch[race]` load at `0xd5446`, and the far call consumes
`r2 = r4` at `0xd54f0`.  HEAD (wave 143-145) used `far = yaw`, which is
semantically wrong and only scores 11 raw bytes because it saves yaw in
`r4` and copies it to `r3` for the near call.

Landed change (one line):

```cpp
MatrixSetRotation(farKey, pitch, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
```

This is a **faithfulness fix, not a metric win**: raw goes **11 -> 43**
(size stays 1008, gate unchanged).  The coordinator may revert to the
11-byte metric-only `far = yaw` if the raw count is preferred.

The 43 bytes are exactly two independent backend artefacts:

1. **Literal load order.**  Original materialises the iPad yaw table
   (`r3`) before the default yaw table (`r4`); ours emits `r4` first.
   This is a 2-byte shift through `0xd5438..0xd546f` plus the two
   literal-pool words at `0xd56b4`/`0xd56b8` (`0x128c16`/`0x128c38` vs
   `0x128c3a`/`0x128c0c`).  The final IR is the same
   `select (iPad==0), Yaw, YawIPad`; InstCombine canonicalises every
   ternary spelling to that form, and the backend picks the load order.
2. **Pre-RA copy placement.**  Ours hoists `str.w r8,[sp,#184]` and
   `mov r4,r2` above the `ldr.w r3,[r0,r8,lsl#2]` yaw load; the original
   places them after it (`0xd545c`/`0xd546e`).  `-mllvm
   -enable-misched=false` and `-enable-post-misched=false` leave the
   order unchanged, so it is not the machine scheduler.

New probes this wave (all reverted unless landed; all 1008 B):

| variant | raw | note |
|---|---:|---|
| HEAD (`far = yaw`) | **11** | METRIC-ONLY, semantically wrong |
| **`far = pitch` (LANDED)** | **43** | faithful |
| `iPad != 0 ? YawIPad : Yaw` | 43 | same IR |
| `iPad == 0 ? Yaw : YawIPad` | 43 | same IR |
| `!iPad ? Yaw : YawIPad` | 43 | same IR |
| `bool isPad = iPad != 0;` then ternary | 43 | same IR |
| `unsigned char isPad = iPad;` then ternary | 43 | same IR |
| `if (iPad) yawTable = YawIPad; else yawTable = Yaw;` | 43 | same IR |
| yaw table declared before pitch table | 46 | |
| volatile pitch only, yaw plain | 44 | literal order flips, interleave differs |
| volatile pitch only, yawBits volatile | 47 | |
| volatile pitch + yaw table local | 47 | |
| `float pitch = *(volatile float *)&pitchTable[race]` | 206 | |
| plain (no volatile) | 667 | codegen re-rolls |
| `far = *(const float *)&pitchBits` | 47 | |
| `float farPitch = pitch;` before far call | 47 | |
| near call with `*(const float *)&pitchBits` / `&yawBits` | 47 | |
| far call `yawTable[race]` (reuse local) | 580 | 984 B |
| two-element pointer array indexed by iPad | 624 | 996 B |
| swapped ternary (semantically wrong) | 688 | 1000 B |

Conclusion: the faithful source is landed.  The residual is a
backend literal-materialisation order + a pre-RA copy hoist that clang
7.0.2 does not expose through any source spelling tested.

## 2. MenuTouchWindow::OnTouchBegin — the 26 B is the reference live range;
##    every direct-C shape loses 4 B

Re-confirmed on this base: HEAD (`Layout &layout` used for A and C,
`Globals::layout->` for B and D) = **28** raw / 1896 B.  The original's
case-15 block reloads `Globals::layout` from `[sp,#16]` after the first
`GetImage2DWidth` call (fresh `ldr r2,[sp,#16]; ldr r2,[r2]` for both
`leftMargin` and `top`), while HEAD caches the `Layout*` in `r6` across
the call.  The original's colouring is `r4 = &Globals::layout`,
`fp = &Globals::Canvas`, `[sp,#16] = &Globals::layout`.

New probes this wave (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD | **28** | 1896 | |
| `const Layout *lp = Globals::layout;` used for A only, B/C/D direct | 400 | 1892 | address hoisted to fp |
| `Layout &layout` A only, B/C/D direct | 400 | 1892 | same |
| `const Layout *lp` used for all four | 352 | 1888 | |
| two references (`layout` for A, `layout2` for C) | 85 | 1896 | keeps size, re-colours |
| case-8 `listEntryWidth` local | (wave 143: 62) | 1896 | |

The 2-byte case-8 cluster is `ldr.w ip,[sl,#340]` (original) vs
`ldr.w r4,[sl,#340]` (ours) for `this->listEntryWidth` in the up-button
y-sum; it is a pure RA tie-break and is present in every direct-C shape
tested.  The 26-byte cluster is the `Layout*` live range across the first
call; every shape that removes it also re-colours `&Globals::Canvas`
from `fp` to `r5` and shrinks the function by 4 B.

## 3. MGame::OnUpdate — the anchor is a value CSE; the original keeps
##    only the *address* in `r9`

Re-confirmed: HEAD (`Status *st = Globals::status;` used at
`st->getPlayingTime()`) = **41** raw / 11440 B.  The original keeps
`&Globals::status` in `r9` from `0x17e08c` through the whole
if/else-if chain and emits four fresh `ldr.w r0,[r9]` loads
(`0x17e090`, `0x17e0a8`, `0x17e0b8`, `0x17e0c0`).  Ours emits
`ldr.w r5,[r9]; mov r0,r5` (anchor) and reuses `r5` at pt.

New probes this wave (all reverted; 11440 B unless noted):

| variant | raw | note |
|---|---:|---|
| HEAD | **41** | |
| no anchor (all direct) | 5148 | 11392 B, base -> r5 |
| anchor at ccm only | 5148 | 11392 B |
| anchor at mission only | 5129 | 11396 B |
| anchor at field only | 4993 | 11436 B; base stays r9, anchor in r5 |
| anchor at ccm+mission | 5129 | 11396 B |
| anchor at ccm+pt | 41 | 11440 B (same as HEAD) |
| anchor at ccm+field | 4993 | 11436 B |
| anchor at mission+pt | 4982 | 11436 B |
| anchor at mission+field | 4966 | 11436 B |
| anchor at pt+field | 4981 | 11436 B |
| anchor at all four | 5033 | 11432 B |
| `Status **stp = &Globals::status` at top of OnUpdate | 5148 | 11392 B |
| `Status *&str = Globals::status` at top of OnUpdate | 5148 | 11392 B |
| `Status *stp = Globals::status` at top of OnUpdate | 10713 | 11368 B |
| `Status **stp` declared in the outer `elapsedTime` block | 5148 | 11392 B |
| `Status *&str` declared in the outer block | 5148 | 11392 B |
| volatile ccm read + reference alias at pt | 5148 | 11392 B |
| volatile ccm read + `(*stp)` at pt | 5148 | 11392 B |
| volatile ccm read only (no anchor) | 5148 | 11392 B |
| volatile ccm read + anchor at pt | 853 | 11440 B; r9 + fresh ccm, extra anchor load |
| volatile ccm read + anchor at field | 5145 | 11392 B |

Conclusion: the original's `r9` is the allocator's choice for the
*address* of `Globals::status` when no value is pinned across the
condition; clang 7.0.2 in our build always picks `r5` for that address
when it is the only long-lived value, and picks `r9` only when a value
(anchor) occupies `r5`.  The 41-byte anchor CSE is unchanged and remains
irreducible with this compiler.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9216
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9216/fdiff.py _ZN10ModStationC1Ev     # 43
python3 /dev/shm/w9216/fdiff.py _ZN15MenuTouchWindow12OnTouchBeginEiiPv   # 28
python3 /dev/shm/w9216/fdiff.py _ZN5MGame8OnUpdateEv    # 41
```

Scratch (not committed), `/dev/shm/w9216/`: `fdiff.py`, `bdiff.py`,
`dd.py`, `cc.sh`, `batch_mod*.py`, `batch_mg*.py`, `batch_mtw*.py`,
`*.cpp.base` (pristine sources).

## 5. Files changed

* `src/game/menu/ModStation.cpp` — far `MatrixSetRotation` x argument
  `yaw -> pitch` (faithful; raw 11 -> 43, size unchanged).
* `_work/reconstructions/near-miss-trio-wave146-w9216.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
