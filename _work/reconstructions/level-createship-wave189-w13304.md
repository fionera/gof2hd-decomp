# `Level::createShip` — wave 189 (w13304): register-allocation / EH-LSDA map

Worktree `wt/w13304`, branch `wave189-w13304`, base master `247466fde`
(wave 188).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13304`.

**Status: no code win landed; note only.**  `src/` is at the wave-188
base.  Base is raw **1,202**, aligned 484, size **2,764/2,764**, insns
1,116/1,117.  Section state reproduced: `.text` diff **74,047**,
`.ARM.extab` **156,312 B @ 0x1a3b78, diff 639**, `.ARM.exidx` 0,
`.rodata` 0, `.rel.plt` 0.

This pass was asked to map every callee-saved/spilled slot to its source
local, try declaration-order / lifetime / one-use aliases, and inspect
the `.ARM.extab` / LSDA scope differences.  Everything below was
measured against the real link (not estimated).

## 0. Bottom line

The base is a strong local optimum.  Roughly 90 further variants were
compiled and measured this wave; every size-exact variant is **>= 1,202
raw**.  The two closest are 1,203 (`g_rmS_addH`, `.ARM.extab` content
639 -> 638) and 1,204 (natural jitter pairing, aligned 399, insns
1117/1117).  The only variant that is a *raw* win is `iff_rmM`
(**1,177**, a 25-byte win) but it is **not landable**: its `.ARM.extab`
is 156,308 (4 B short), which shifts the whole section and is gate-fatal.
No source shape found this wave both (a) keeps `.ARM.extab` at 156,312
and (b) beats 1,202 raw.

## 1. Prologue map (identical through `0xc02b4`)

Original and ours agree byte-for-byte through the first ~40 insns:

```
str  r1,[sp,#60]      ; hostile  (5th arg)
mov  r9,r0            ; this
mov  fp,r3            ; type
mov  r5,r2            ; shipClass
mov  r8,r0            ; mission  (getCurrentCampaignMission result)
mov  sl,r5            ; shipClass
...
mov  r6,sl            ; shipClass (c02b4)
```

So the argument/`this` assignment is not the problem; the divergence is
purely in *spill-slot creation order* and downstream live ranges.

### 1.1 Coordinate-local stack slots (`+0x2c`, six 4-byte slots)

| value | original slot | ours slot |
|---|---:|---:|
| `wpx` | 44 | 40 |
| `wpy` | 48 | 36 |
| `wpz` | 40 | 48 |
| first `nextInt` result (`jitter3` in base source) | 36 | 44 |
| second `nextInt` (`jitter2`) | 32 | 32 |
| third `nextInt` (`jitter1`) | 28 | 28 |

Original slot order by address: `jitter1(28), jitter2(32), jitter3(36),
wpz(40), wpx(44), wpy(48)`.  Ours: `jitter1(28), jitter2(32), wpy(36),
wpx(40), jitter3(44), wpz(48)`.

These are *spill slots created by RegAlloc*, not IR allocas, so they
follow live-interval order, not the source declaration order.  Evidence:

* Reordering the `if (waypoint)` assignments (`wpx/wpy/wpz` in any of the
  6 orders, and the `else` zero-stores in any order) is codegen-neutral
  (raw 1,202 exactly).  Clang canonicalises the loads/stores.
* Reordering the *declaration* `int wpz, wpx, wpy;` to `wpx, wpy, wpz`
  (or `wpy, wpx, wpz`, or `wpx, wpz, wpy`) does **not** move the slots
  (still 40/36/48/44/32/28) but changes the rest of the function
  massively: raw **2,423**, size **2,768** (+4).  Not landable.
* Changing the jitter declaration/assignment order *does* move the
  jitter slots (see §3).

### 1.2 First real divergence: `empPoints` clamp polarity + `this` spill

`0xc0314` region (original vs base):

```
orig:  ldr r0,[r4]; blx getLevel; cmp r0,#20
       ble.n c0322                 ; fall-through = ">20 -> 20"
       movs r0,#20; b c0328
c0322: ldr r0,[r4]; blx getLevel
c0328: add.w r0,r0,r0,lsl#2
       cmp r6,#1
       add.w sl,r0,#40
       str.w r9,[sp,#56]           ; spill `this`
       str.w fp,[sp,#52]           ; spill `type`
       bne.n c0352
```

```
ours:  ldr r0,[r4]; blx getLevel; cmp r0,#20
       str.w r9,[sp,#56]           ; spill `this` HERE
       bgt.n c032a                 ; opposite polarity
       ldr r0,[r4]; blx getLevel; b c032c
c032a: movs r0,#20
c032c: add.w r0,r0,r0,lsl#2
       cmp r6,#1
       add.w sl,r0,#40
       str.w fp,[sp,#52]
       bne.n c0352
```

Two independent defects:

1. **Branch polarity.**  Base source is
   `(getLevel() <= 20 ? getLevel() : 20)`; the original is the same
   ternary *polarity as the `level` local*:
   `(getLevel() > 20 ? 20 : getLevel())`.  Flipping it (`emp_flip`)
   produces the original `ble`/fall-through shape, but the `str.w r9`
   spill is still emitted at `0xc031c`, so the whole window shifts by 4
   bytes and raw goes **1,202 -> 1,209**.
2. **`this` spill placement.**  The original sinks the `str.w r9` to
   `0xc0332` (after the ternary join, immediately before the
   `shipClass == 1` branch); ours emits it at `0xc031c` (end of the
   ternary's first block).  `fp` (`type`) is spilled at `0xc0336` in
   both.  Because the store is 4 bytes, its position shifts every
   branch offset in `0xc031c..0xc0332`, which is the bulk of the
   94-byte `0xc0314` region diff.

Trying to move the spill by source reordering was uniformly worse:
`armor` before `empPoints` -> 2,451 (size 2,768); `mission == 0x9a`
before `empPoints` -> 2,536 (size 2,744); `empPoints` computed inside
each `shipClass` arm -> 2,509 (size 2,784).  The spill position is a
RegAlloc decision that no local source permutation reached.

### 1.3 Coordinate conversion block (`0xc03c8..0xc03e4`)

Original computes **all three ints before `setEmpData`** and converts
them afterwards:

```
add r2,r3; ldr r3,[sp,#40]; ldr r4,[sp,#28]; add r0,r1
movw r1,#20000
sub.w fp,r0,r1        ; x = wpx + jitter1 - 20000   (int, fp)
add r3,r4
subs r4,r2,r1         ; y = wpy + jitter2 - 20000   (int, r4)
sub.w r8,r3,r1        ; z = wpz + jitter3 - 20000   (int, r8)
mov r0,r5; mov r1,sl; mov r2,r9; blx setEmpData
...
vmov s0,fp; vcvt.f32.s32 s20   ; x -> float
vmov s0,r4; vcvt.f32.s32 s18   ; y -> float
vmov s0,r8; vcvt.f32.s32 s16   ; z -> float
```

Base source declares `float z` before the guard and `int x` / `float y`
after, so ours converts `z` early and `y`/`x` late.  The exact original
shape is `int x, y, z` declared before `setEmpData` — but `iii` is
**size 2,752 (-12)**, because clang sinks the int computations *after*
`setEmpData` and the ints never live across the call.  `iff`
(`int x, float y, float z` before the guard) is the only size-exact
coordinate shape: raw **1,213**, aligned **396**, insns **1117/1117**,
`.ARM.extab` exact (diff 639).  `fii`/`ffi`/`ifi` before the guard are raw
2,489 / 1,225 / 2,492 (sizes 2,780 / 2,764 / 2,780).

### 1.4 Tail register coalescing

Original keeps `ship` in `r9`, `fighter` in `sl`, `x` in `fp`, and
`fixed` in `fp` (capital branch); ours coalesces `ship`/`fighter`/
`fixed` into `fp` and keeps `x` in `r4`.  This is why the capital-branch
BoundingAAB region (`0xc0414..0xc0b14`, ~1,100 of the 1,202 raw bytes)
differs even where the instruction shapes are identical: every
`BoundingAAB` ctor call passes `x,y,z` from different registers
(original `sl,r8,r4`; ours `r6,r8,r3`).  The cleanup landing pads at
`0xc0bba..0xc0bd6` also differ in the same way (`mov r0,sl/fp/r9/r5`).

## 2. `.ARM.extab` / LSDA scope map

`.ARM.extab` entry for `0xc0214` is at **`0x1ab09c`** (from `.ARM.exidx`
word `0xb178` at `0x19ff24`); the next entry is at `0x1ab1ec`, so the
original LSDA entry is **336 B**.  Entry layout: `word prel31(personality
= 0x5eb60)`, then a 5-byte header `ff ff 01 bb 02`, then a ULEB128
call-site table of `(start, len, landing_pad, action)` tuples relative to
`0xc0214`.  All rows have `action = 0` (cleanup-only, no action table).

Parsed original table (57 rows, 315 table bytes):

```
row0  (0,406,0,0)          <- [0,406) has NO landing pad
row1  (406,30,2488,0)      <- new Player
row2  (436,94,0,0)         <- includes setEmpData: NO landing pad
row3  (530,32,2482,0)      <- new PlayerFixedObject
row4  (562,8,0,0)
row5  (570,4,2476,0)       <- new Array
row6  (574,78,0,0)
row7  (652,30,2448,0)      <- first new BoundingAAB
...   alternating (lp=0) / (lp!=0) through row55
row56 (2316,448,0,0)       <- tail, no landing pad; ends exactly at 2764
```

Key findings:

* **The original has no `EmpGuard` around `getCurrentCampaignMission`.**
  Row0 covers `[0,406)` with `lp = 0`, i.e. the mission call at
  `0xc0230` (offset 28) has no landing pad.  Base's row0 is
  `(28,4,2482,0)` (an extra landing pad for the mission call).
* **The original has no `EmpGuard` around `player->setEmpData` either.**
  `setEmpData` at `0xc03ee` (offset 474) falls inside row2
  `(436,94,0,0)`, `lp = 0`.
* Removing the mission guard (`g_rmM`) makes rows 0 and 1 match the
  original exactly, but the entry becomes **332 B / 156,308 section** —
  4 B short.  `iff_rmM` parses to **56 call-site rows**; the original has
  **57**.  The entry is 4 B shorter, i.e. one missing call-site tuple;
  the header/action layout is otherwise the same.
* The original's landing pads are one per `new BoundingAAB` (28 LP
  regions total), each with a distinct address, so adding a guard around
  any of them adds both a landing pad (code) and a call-site entry.

Consequence: any source shape that removes the mission guard is
`.ARM.extab`-short unless it also adds exactly one call-site entry with
zero code.  No such shape was found (see §4).

## 3. Sweeps measured this wave (all size 2,764 unless noted)

### 3.1 Jitter declaration/assignment order

| variant | raw | aligned | insn |
|---|---:|---:|---:|
| base (`jitter3, jitter2, jitter1`) | **1,202** | 484 | 1116/1117 |
| `jitter1, jitter2, jitter3` | 1,204 | 399 | 1117/1117 |
| `jitter1, jitter3, jitter2` | 1,203 | 398 | 1117/1117 |
| `jitter2, jitter1, jitter3` | 1,204 | 486 | 1116/1117 |
| `jitter2, jitter3, jitter1` | 1,203 | 485 | 1116/1117 |
| `jitter3, jitter1, jitter2` | **1,202** | 397 | 1117/1117 |

All keep `.ARM.extab` exact (diff 639) and `.rel.plt` identical.  The
natural pairing (first `nextInt` -> `jitter1`) is structurally closer
(aligned 399, insns 1117/1117) but 2 raw bytes worse.

### 3.2 `empPoints` clamp

| variant | raw | size |
|---|---:|---:|
| base `<= 20 ? getLevel() : 20` | **1,202** | 2,764 |
| `> 20 ? 20 : getLevel()` (`emp_flip`) | 1,209 | 2,764 |
| `if (empPoints > 20) 20; else getLevel();` | 1,209 | 2,764 |
| `if (empPoints <= 20) getLevel(); else 20;` | 1,202 | 2,764 |
| `min`-form | 2,157 | 2,756 |

### 3.3 Coordinate-local types

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (`float z` before, `int x`/`float y` after) | **1,202** | 484 | 2,764 |
| `iff` (`int x, float y, float z` before) | 1,213 | **396** | 2,764 |
| `fii` before | 2,489 | — | 2,780 |
| `ffi` before | 1,225 | — | 2,764 |
| `ifi` before | 2,492 | — | 2,780 |
| `iii` before | 2,238 | 502 | **2,752** |
| `iii` after | 2,238 | 502 | 2,752 |

### 3.4 Guard add/remove around the fighter branch

| variant | raw | `.ARM.extab` |
|---|---:|---:|
| base | **1,202** | 156,312 (diff 639) |
| remove `setShipGroup` guard (`g_rmS`) | 1,198 | 156,304 (short) |
| remove `setShipGroup`, guard `getShipGroup` (`g_rmS_addH`) | 1,203 | 156,312 (diff **638**) |
| remove `setShipGroup`, guard `getShipGroup` + natural jitters | 1,205 | 156,312 (diff 638) |
| `g_rmS_addL` | 1,203 | 156,312 |
| `g_rmS_addAO` (guard `inAlienOrbit`) | 1,204 | 156,312 (diff 661) |
| `g_rmS_addI` / `addB` / `addX` / `addV` | 1,206 / 1,209 / 1,212 / 1,214 | 156,312 |
| remove mission guard (`g_rmM`) | **1,166** | 156,308 (short) |
| `g_rmM_addH` / `addL` | 1,202 | 156,320 (8 long) |
| `g_rmM_addA/B/I/W/X` | 1,205–1,211 | 156,312 |
| `iff` + remove mission guard (`iff_rmM`) | **1,177** | 156,308 (short) |
| `iff_rmM` + natural jitters (`im_132`) | 1,178 | 156,308 (short) |
| `iff` + `g_rmS_addH` | 1,214 | 156,312 |

`g_rmS_addH` is the best `.ARM.extab`-content result (diff 638, one byte
better than base) but is 1 raw byte worse.

### 3.5 Aliases / declaration order (all codegen-neutral, raw 1,202)

`Level *self = this`, `LODManager *lm = lodManager`, `int shipType =
type`, `int shipRace = race`, `ship = fixed` early, `this->` forms,
`volumes`/`fixed` declaration reorder, `type` copy, `race` copy,
`missionPtr` hoist.  All raw 1,202, size 2,764, `.rel.plt` 0.  Only
`ship = nullptr` removal changes size (2,760).

### 3.6 `__builtin_expect` sweep

All `__builtin_expect` variants on `shipClass`/`mission`/`waypoint`/
`type` change size (mostly +8 or -48).  `exp_mrange_0` is size-exact
2,764 but raw 1,392.

## 4. Why no `iff_rmM`-based landable win

`iff_rmM` is the only raw win (1,177) and is 4 B short of `.ARM.extab`.
To land it we need +4 B of LSDA with +0 B of code (one extra call-site
entry whose landing pad is shared with an existing one).  Every guard
added around a call in the capital/fighter branch either adds a fresh
landing pad (+36 B typical) or is optimised away (`delete fighter->cargo`
guard: no change).  A nested guard around `setEmpData` reuses the same
call site and adds nothing.  The original's 57th row corresponds to a
call-site split that no source-level guard reproduced at the current
code layout.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13304
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1202 aligned=484 size=2764/2764 insn=1116/1117
```

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13304`): `base_body.cpp`
(pristine body), `h.py` (splice + build + fn_diff), `run2.sh` (sweep
driver), `sec.py` (section sizes/diffs), `regions.py` (per-0x100 raw
diff), `byinsn.py`, `lsda2.py` (LSDA call-site parser), `v_*.cpp`
(~120 variant bodies).

## 6. Files changed

* this note (no `src/` change; `Level.cpp` left at the wave-188 base).
