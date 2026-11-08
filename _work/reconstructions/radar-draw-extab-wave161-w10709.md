# Radar::draw LSDA/extab attack (wave 161, w10709)

**Status:** source change landed 2026-11-08. Worktree `wt/w10709`, base master
`01c7f92d6` (wave 160). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `wave160-merge-tests.md`, `radar-draw-wave160-w10608.md`,
`build-provenance-wave160-w10620.md`.

Baseline (wave-160 HEAD): original 9,908 B; ours 9,908 B; raw same-address diff
**8,124**; instruction-index alignment **1,320**; `.text` differing bytes
**85,518**; `.ARM.extab` differing bytes **1,258**; `.rel.plt` 0; all 27
section sizes exact; file size 2,187,328 B; gate byte_exact 4507/4524.

## 1. Every throwing call in Radar::draw

The original LSDA record lives at `.ARM.extab` offset `0x1c203c` (same offset
in ours), 16-byte extab header + 112-byte LSDA, 19 call-site rows, 11 with a
non-zero landing pad.  All 11 are `AbyssEngine::String` operations; every other
`blx` in the function (Status/Level/Mission/Sprite/FModSound getters) is
outside the EH table.  Function start `0x12e050`:

| row | orig start/len/lp | call (orig address) | cleanup |
|---:|---|---|---|
| 1 | `0x238/8/0x265a` | `String::operator=` @0x12e28c (`lockLabel = calcDistance`) | destroy sp+232 |
| 3 | `0x95a/a/0x2680` | `String::operator=` @0x12e9b0 (landmark `lockLabel=`) | destroy sp+232 |
| 5 | `0xa02/a/0x267c` | `String::operator+` @0x12ea58 (`getText(133)+": "`) | destroy sp+208 |
| 6 | `0xa10/10/0x2682` | `getStation`+`getTecLevel`+`String(int)` @0x12ea60..6c | destroy sp+220 |
| 7 | `0xa20/c/0x2672` | `String(const String&,bool)` @0x12ea78 | destroy sp+184 |
| 8 | `0xa2c/10/0x2668` | `String::operator+` @0x12ea88 | destroy sp+196 |
| 9 | `0xa4e/14/0x265e` | `PaintCanvas::DrawString` @0x12eaae | destroy sp+232,196,184,208,220 |
| 11 | `0xab0/e/0x265c` | `String::operator=` @0x12eb0a (landmark `lockLabel=`) | destroy sp+232 |
| 13 | `0xfa6/8/0x2690` | `String::operator=` @0x12effa (planet `lockLabel=`) | destroy sp+232 |
| 15 | `0x16b2/8/0x268e` | `String::operator=` @0x12f706 (enemy `lockLabel=`) | destroy sp+232 |
| 17 | `0x19ce/8/0x2658` | `String::operator=` @0x12fa22 (asteroid `lockLabel=`) | destroy sp+232 |

Rows 0/2/4/10/12/14/16/18 are the `lp=0` gaps between call sites.  The
`action` field is 0 everywhere (no type table, no catch handlers) — this is a
pure cleanup LSDA.

## 2. Per-row diff (baseline, 50 bytes)

| row | orig | ours | bytes |
|---:|---|---|---:|
| 1 | (568,8,9818) | (568,8,9816) | 1 |
| 2 | (576,1818,0) | (576,1776,0) | 2 |
| 3 | (2394,10,9856) | (2352,12,9854) | 4 |
| 4 | (2404,158,0) | (2364,136,0) | 2 |
| 5 | (2562,10,9852) | (2500,10,9850) | 3 |
| 6 | (2576,16,9858) | (2514,16,9856) | 3 |
| 7 | (2592,12,9842) | (2530,14,9840) | 4 |
| 8 | (2604,16,9832) | (2544,12,9830) | 4 |
| 9 | (2638,20,9822) | (2568,20,9820) | 2 |
| 10 | (2658,78,0) | (2588,76,0) | 2 |
| 11 | (2736,14,9820) | (2664,10,9818) | 4 |
| 12 | (2750,1256,0) | (2674,1294,0) | 4 |
| 13 | (4006,8,9872) | (3968,12,9874) | 3 |
| 14 | (4014,1796,0) | (3980,1774,0) | 3 |
| 15 | (5810,8,9870) | (5754,8,9868) | 3 |
| 16 | (5818,788,0) | (5762,856,0) | 2 |
| 17 | (6606,8,9816) | (6618,8,9808) | 2 |
| 18 | (6614,3294,0) | (6626,3282,0) | 2 |

The `start`/`length` columns are code-layout consequences (the body is 8,124
bytes different); they cannot match until the body matches.  The `lp` column is
the actionable part.

## 3. Root cause of the LP column: constant-pool placement

Every `lp` is exactly **2 less** than the original (`9816` vs `9818`,
`9854` vs `9856`, ...).  The cleanup chain is identical in *content* and
*order* (row17, row1, row11, row9, row8, row7, row5, row3, row6, row15,
row13), but it starts at a different offset because of the ARM constant-pool
island:

* original: main body ends at `0x1306a4`; the single-word mask pool
  (`0x023c1c8f`, the ambient-music switch mask) sits at `0x1306a4`; **all**
  landing pads start at `0x1306a8` (= `0x2658`).
* ours: main body ends at `0x1306a0`; row17's landing pad (`b.n`) is emitted
  *before* the pool at `0x1306a0`, a `nop` pads to `0x1306a4`, the pool is at
  `0x1306a4`, and the remaining pads start at `0x1306a8`.

This is the same 4-byte main-body deficit documented in
`radar-draw-wave160-w10608.md` §4.

## 4. The scope/order change that fixes the LP column

The original keeps the `!missionBlocksPlanets` test in

```cpp
if (!missionBlocksPlanets && this->planetLockTimer >= 1) {
```

even though the preceding `if (missionBlocksPlanets) { ...; goto enemies; }`
already proves it false.  The original lowers it as
`movs r1,#0; cmp r0,#1; it lt; movlt r1,#1; orrs.w r0,r5,r1; bne`
(16 B); ours drops it to `cmp r0,#1; blt.w` (6 B).

Swapping the operands

```cpp
if (this->planetLockTimer >= 1 && !missionBlocksPlanets) {
```

restores the original's 4-byte-longer main body, moves the mask pool to the
end of the main body, and puts **all** landing pads at `0x2658` — the LP column
then matches the original exactly for rows 3/5/6/7/8/9/11/15/17:

| metric | baseline | operand swap |
|---|---:|---:|
| LSDA bytes | 50 | **39** |
| rows | 19 | 19 |
| function size | 9,908 | **9,912** |
| raw | 8,124 | 8,492 |
| aligned | 1,320 | **1,309** |

The remaining 4 bytes are the row13 landing pad: the original's planet-branch
`calcDistance` temporary is at `sp+232` (so row15's pad is a bare `b.n` to
row13's pad), ours is at `sp+220` (so row15 needs its own
`mov r4,r0; add r0,sp,#232; b.n`, 4 B more).  Every attempt to force the slot
(named `String`, local `Vector`, pointer alias, float locals, expression
regrouping) keeps `sp+220`.  **This is the open lead**: getting row13's temp
to `sp+232` should make the swap variant size-exact at 9,908 with LSDA 39.

## 5. Landed size-exact variant

Because the swap is +4 B (gate-fatal), the landed change is a size-exact
variant that still lowers the record:

```diff
-    int alwaysEnemySeen = false;
+    bool alwaysEnemySeen = false;
...
-    int alienOrbit = Globals::status->inAlienOrbit();
+    bool alienOrbit = Globals::status->inAlienOrbit();
...
     Globals::status->getCurrentCampaignMission();
-    Globals::status->getCurrentCampaignMission();
```

The two `bool` types change the register/stack allocation enough that the
cleanup chain moves 4 bytes later, and dropping one dead getter compensates
the size.  Measured:

| metric | before | after |
|---|---:|---:|
| size | 9,908 | **9,908** |
| raw | 8,124 | 8,528 |
| aligned | 1,320 | 1,355 |
| LSDA bytes | 50 | **48** |
| `.text` diff | 85,518 | 85,922 |
| `.ARM.extab` diff | 1,258 | **1,256** |
| `.rel.plt` diff | 0 | 0 |
| byte_exact / linked_exact | 4507/4507 | **4507/4507** |

`check` is green (size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370).  The raw/aligned regression is the price of the
2-byte extab win; the operand-swap variant above is the better extab result
but is not size-exact.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10709
python3 /dev/shm/w10709/lsda.py _ZN5Radar4drawEP6PlayerP3Hudi
python3 /dev/shm/w10709/rowdiff.py
python3 /dev/shm/w10709/secdiff.py
```
