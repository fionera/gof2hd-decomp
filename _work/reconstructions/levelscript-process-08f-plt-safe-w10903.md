# LevelScript::process — wave 163 / w10903: PLT-safe second 0.8f load

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w10903`, base master `e51db2fc7` (wave 162).

## Baseline (HEAD, gate green)

```
process raw diffbytes    54,698
process aligned insns    17,456
orig / ours size         57232 / 57232
.rel.plt                 byte-identical (0 diffs)
byte_exact               4507 / 4524
```

## What is missing

The original has **three** `0.8f` (0x3f4ccccd) literal-pool words inside
`process` (`0x13c8f8`, `0x13e87c`, `0x145c78`); the baseline has **two**.
The missing load is the mission-0x4e post-switch tail at `0x13c88c`:

```
13c88c: ldr.w r0,[sl,#28]     ; m_nState
13c890: subs  r0,#5
13c892: cmp   r0,#3
13c894: bhi.n 13c8ba
13c896: ldr   r0,[sp,#76]     ; delta
13c898: movs  r2,#0
13c89a: vldr  s2, 0.8f
13c89e: movs  r3,#0
13c8a0: negs  r0,r0
13c8a2: vmov  s0,r0
13c8a6: vcvt.f32.s32 s0,s0
13c8aa: ldr.w r0,[sl,#20]     ; m_pCamera
13c8ae: vmul.f32 s0,s0,s2
13c8b2: vmov  r1,s0
13c8b6: blx   TargetFollowCamera::translate
13c8ba: ldrb.w r5,[sl,#17]    ; m_bCinematicBreak
```

`0x13c88c` is the switch's `bhi` (default) target **and** the common break
target of every `case` (`subs #1; cmp #11; bhi 13c88c` at `0x13837c`), i.e.
the block is the *post-switch* source.  The baseline has the `if` *inside*
the switch after `case 11`, where every case `break`s, so clang proves it
unreachable and deletes it.

## Landed shape: block hoisted to the top of the mission-0x4e arm

Moving the `if` *after* the switch (the faithful position) restores the
block but reorders clang's global block linearisation: the shared active
tail (0x137f7c) moves to ~0x139f60 and the mission-2/5/26/50/51/42
dispatches are pushed to the end.  That rotates `.rel.plt` (30 slots:
`isTriggered` moves after `moveForward`/`setFreeLookMode`/`getSpeed`, and
mission-89's state-4 `scaleSun`/`updateSupernova` move before
`setRocketControl`).

Instead, hoisting the same `if` to the **top of the mission-0x4e arm**
(before the `m_nScriptTime >= 7901` gate) keeps the baseline dispatch order
(0,1,2,81,5,78,26,89,50,51,42,106,95,...) exactly, so `.rel.plt` stays
byte-identical.  The block is emitted (three 0.8f loads) and the
instruction-index alignment is essentially unchanged.

```c++
} else if (Globals::status->getCurrentCampaignMission() == 0x4e) {
    int st78 = m_nState;
    if (st78 >= 5 && st78 <= 8) {
        m_pCamera->translate((float) -delta * 0.8f, 0.0f, 0.0f);
    }
    ...
    switch (st78) { ... }
```

The `int st78 = m_nState;` local is shared by the block and the switch; it
saves one `m_nState` reload (-8 B) and keeps the dispatch order.

A second, equally PLT-safe shape is the same block **without** the `st78`
local (`m_nState` read twice): raw 55,416, aligned 17,456 (identical to the
baseline aligned), size 57,272.  The `st78` form is preferred on the raw
metric.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,698            55,380            (+682)
process aligned insns    17,456            17,681            (+225)
orig / ours size         57232 / 57264     57264             (+32)
.rel.plt                 0                 0                 BYTE-IDENTICAL
0.8f literal loads       2                 3                 (block restored)
```

## The +32 B residual

The hoisted block is 44 B (the original's post-switch copy is 46 B: it
reloads `delta` from `[sp,#76]`, ours reuses `r6`).  The function grows by
32 B net because the block is emitted (+44) and one `m_nState` reload is
saved (-8) plus 4 B of codegen width changes.  **The gate is therefore red
until a -32 B companion is found**: `byte_exact` drops 4507 -> 2741 purely
because the +32 B shifts every later function's address.

Candidate -32 B companions (not yet found): the mission-0x4e case bodies
use two `str.w` where the original uses one `strd` for `m_nScriptTimerA = 0`
(case 5: 8 B vs 4 B; six such stores exist in the arm), and the
`itt ge`/`strge.w` conditional-store shape in case 3 is 2 B wider than the
original's `blt`+`movs`.  Recovering those codegen widths is the next lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10903/fdiff.py _ZN11LevelScript7processEi        # 55,380 / 57,264
python3 /dev/shm/w10903/align_diff.py _ZN11LevelScript7processEi  # 17,681
python3 /dev/shm/w10903/relplt.py                                 # 0 diffs
python3 /dev/shm/w10903/poolscan.py _ZN11LevelScript7processEi 0.8 # 3 loads
```

## Open questions

1. Find the -32 B companion so the size returns to 57,232.  The `strd`
   stores and the `itt ge` conditional store in the mission-0x4e case
   bodies are the measured candidates.
2. The faithful post-switch placement is still worth revisiting: if the
   mission-0/89 block order can be pinned (e.g. by reordering the mission
   chain to the original's dispatch order 0,1,2,5,81,26,78,50,89,42,...),
   the block can move back after the switch and `.rel.plt` may still hold.
