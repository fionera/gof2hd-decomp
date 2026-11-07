# Near-miss trio wave 139 (w8418): MenuTouchWindow 29 -> 28 raw bytes

Status: **one landed `src/` change** (MenuTouchWindow), plus negative results for
MGame::OnUpdate and ModStation. Worktree `wt/w8418`, base `bf123bd47`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave138-w8318.md`, `long-tail-wave137-w8220.md`,
`near-miss-wave133-w7741.md`, `mgame-onupdate-wave129-w7661.md`,
`mgame-onupdate-r5-cache-wave97-w4717.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.
`.text` differing bytes **85,476 -> 85,475**. `.rel.plt` still byte-identical.

## 0. Residual classification (raw same-address bytes)

| target | size | raw diff before | after | hunks |
|---|---:|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 41 | 2 real insns + 2-byte shift |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 29 | **28** | 2 clusters |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 11 | 1 scheduling hunk |

### MGame::OnUpdate — every differing byte

`r9 = &Globals::status` in both. The whole run is the `Status *st` anchor:

```
17e090  ldr.w r0,[r9]        |  ldr.w r5,[r9] ; mov r0,r5   (ccm)
17e094  blx getCurrent...    |  (shifted +2)
...
17e0b8  ldr.w r0,[r9]        |  mov r0,r5                   (getPlayingTime)
```

The anchor is CSE'd with the ccm load, so the ccm emits `ldr r5,[r9]; mov r0,r5`
(4+2 B) instead of `ldr r0,[r9]` (4 B). The +2 B shift at 17e094 is compensated
by the -2 B `mov` at 17e0b8, so the size stays 11440; the 2-byte shift is what
turns 2 real instruction differences into 41 differing bytes (every halfword in
17e094..17e0b8 is misaligned). Confirmed: dropping the anchor gives the exact
four fresh loads but re-colours the GOT base to r5 and re-rolls the function to
11392 B / 5148 raw bytes.

### MenuTouchWindow::OnTouchBegin — two clusters

Cluster A (2 bytes, case-3 iPad hit test, `0x129fd2`):

```
ldr.w ip,[sl,#340]  |  ldr.w r4,[sl,#340]     ; this->listEntryWidth
add   r3,ip         |  add   r3,r4
```

Cluster B (27 bytes, case-15 scrollbar block, `0x12a1e0..0x12a24c`): the
`Layout &layout` reference keeps the layout *value* live across the first
`GetImage2DWidth` call (r6 in HEAD) instead of reloading `Globals::layout`
fresh at `0x12a218` (r2 in the original). That one live value rotates the whole
local assignment by one register and reorders the `ldr.w r1,[r0,r1,lsl#2]` /
`ldr r2,[r2]` pair.

### ModStation ctor — one scheduling hunk

```
d5456  ldr.w r3,[r0,r8,lsl#2]   |  str.w r8,[sp,#184]
d545a  movs  r0,#2              |  ldr.w r4,[r0,r8,lsl#2]
d545c  str.w r8,[sp,#184]       |  movs  r0,#2
...
d546c  mov   r0,r8              |  mov   r3,r4
d546e  mov   r4,r2              |  mov   r0,r8
```

The original saves `pitch` (r2) in r4 for the far call and loads `yaw` straight
into r3. HEAD saves `yaw` in r4 and moves it to r3. The far call itself
(`d54e0..d5502`) is byte-identical.

## 1. MenuTouchWindow: landed 1-byte win

Change (`src/game/ui/MenuTouchWindow.cpp:2348`): keep the `Layout &layout`
reference for `buttonInsetX`, but read `field_0x20_top` through the pointer and
`field_0xc_leftMargin` through the reference:

```cpp
y < Globals::layout->field_0x20_top + (layout.field_0xc_leftMargin + ...)
```

This makes the `field_0x20_top` load come from a fresh `Globals::layout` load
(the original's `ldr r2,[r2,#0]; ldr r6,[r2,#32]` shape) while the reference
stays live, so `&Globals::layout` is still remat'd from `[sp,#16]` exactly as
in the original. The remaining difference is that `field_0xc_leftMargin` is
still read from the cached reference (r6) and therefore hoisted before the
`heapBufB` load. 28 raw bytes, size 1896 exact.

Variants measured this wave (all reverted unless noted):

| variant | raw diff | size | note |
|---|---:|---:|---|
| HEAD | 29 | 1896 | reference live for `top` |
| **pointer `top` + reference `leftMargin`** | **28** | **1896** | landed |
| pointer for both fields (pure all-direct, no `right` local) | 358 | 1892 | GOT base kept in r5, not remat |
| `Layout *&lp = Globals::layout` + `lp->` | 358 | 1892 | same as all-direct |
| `Layout *volatile &lpv` + `lpv->` for both fields | 300 | 1892 | |
| `const Layout &layout` | 29 | 1896 | identical code |
| `(layout.field_0x20_top + Globals::layout->field_0xc_leftMargin) + h` | 45 | 1896 | grouping |
| `leftMargin + top + h` | 45 | 1896 | grouping |
| `leftMargin + (top + h)` | 29 | 1896 | grouping |
| `h + (top + leftMargin)` | 116 | 1896 | grouping |
| `int lew = this->listEntryWidth` (case 3) | 538 | 1896 | re-rolls case 3 |
| `Layout &l3` local in case 3 | 29 | 1896 | identical |
| volatile `top` read | 363 | 1892 | |

The case-3 `ip` vs `r4` cluster is a pure Greedy-RA low-register choice and did
not move under any of the case-3 spellings tried (`local_lew`, `local_lew2`,
`Layout &l3`).

## 2. MGame::OnUpdate: new probes, still 41

New this wave (all reverted):

| variant | raw diff | size | note |
|---|---:|---:|---|
| HEAD anchor at getPlayingTime | **41** | 11440 | |
| `Status &st = *Globals::status` + `st.getPlayingTime()` | 41 | 11440 | identical code |
| same, ccm via `st.getCurrentCampaignMission()` | 41 | 11440 | identical |
| anchor at getMission (direct) | 5129 | 11396 | anchor takes r4, ccm CSE'd |
| anchor at getMission via comma | 5148 | 11392 | r5 |
| anchor at getPlayingTime via comma | 5148 | 11392 | r5 |
| anchor at field read via comma | 5148 | 11392 | r5 |
| anchor at field read (direct) | 4993 | 11436 | r5 |
| `long long &fld = reinterpret_cast<...>(field_0x100)` | 55 | 11440 | closest non-HEAD |
| anchor at getPlayingTime + field via `st` | 4981 | 11436 | |
| anchor at ccm+pt+field | 4981 | 11436 | |
| anchor at mission+pt | 4982 | 11436 | |
| anchor at all four sites | 5033 | 11432 | |
| `__builtin_expect(cond, 0)` on ccm/mining/mission/pt | 5106..5126 | 11444..11452 | changes block layout |
| `__builtin_expect(cond, 1)` on ccm/mining/mission/pt | 4938 | 11428 | changes block layout |

The wave-97/129 conclusion stands: the original IR is the no-anchor shape, but
the GOT base is kept whole in r9; every source shape that reproduces the four
fresh loads re-colours it to r5. `__builtin_expect` flips block weights but also
changes the emitted code size, so it is not a code-neutral lever here.

## 3. ModStation: new probes, still 11

| variant | raw diff | size | note |
|---|---:|---:|---|
| HEAD (`far = yaw`, volatile yaw load) | **11** | 1008 | |
| `far = pitch` | 43 | 1008 | pool order Yaw/YawIPad swaps |
| `far = pitch` + explicit `yawIPadTable` local | 43 | 1008 | pool still swaps |
| `far = pitch, yaw` (both locals) | 580 | 984 | far y CSE'd |
| `far = yaw, yaw` (both locals) | 613 | 968 | |
| `float farYaw = (ternary)[race]` after near call, far y = farYaw | 481 | 1008 | |
| `far = pitch`, far y = farYaw local | 495 | 1008 | |
| `far y = yawTable[race]` | 613 | 968 | |
| swap `yaw`/`pitch` declaration order | 11 | 1008 | identical |
| swap the two volatile bit loads | 694 | 1000 | |

The 11 bytes are the pre-RA scheduler ordering of the `yaw` load against the
`race` spill (`str.w r8,[sp,#184]`), plus the `mov r4,r2` / `mov r3,r4` role
swap. No source spelling found that orders the load before the spill while
keeping the pool order Pitch,YawIPad,Yaw.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8418
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Scratch (not committed), `/dev/shm/w8418/`: `fdiff.py`, `score.sh` (one-TU
incremental link + raw diff), `cc.sh` (exact per-TU compile), `mgvar*.py`,
`mtwvar*.py`, `modvar*.py` (variant drivers), `mg_pa.txt` / `mg_plain_pa.txt`
(`-mllvm -print-after-all` MIR dumps for HEAD and no-anchor).

## 5. Files changed

`src/game/ui/MenuTouchWindow.cpp` (1 line), this note.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, `.text` 85,476 -> 85,475.
