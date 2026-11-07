# LevelScript::process — wave 130 / w7668 (case-91 state machine + case-40 bogus timer store)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.102467171      60.086050387     (-0.016416784)
official verify pct      60.1              60.1             (same, rounded)
orig / ours insns        20169 / 20039     20169 / 20040
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54577             54574            (-3)
whole .text diffbytes    85441             85438            (-3)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.rodata/.data/.ARM.exidx/.rel.plt/.dynsym/.dynstr/.hash/.rel.dyn/.got   all 0 diff
```

The raw difflib ratio dips 0.016 (≈3 instructions of alignment noise) while the real
differing-byte count falls by 3; the official rounded pct is unchanged (60.1).  Both edits are
behavioural corrections, not metric tricks.

## Landed change 1 — case 91 (`m_nState` 0/1): the defector-freighter escort

Original state-0 body `0x13ac5c..0x13ac7c`:

```
13ac5c  ldr   r4, [sp, #88]        ; this
13ac5e  movs  r0, #0
13ac60  movs  r1, #1
13ac62  strd  r0, r0, [r4, #152]   ; m_nScriptTimerB = 0
13ac66  movs  r0, #1
13ac68  str   r0, [r4, #28]        ; m_nState = 1        <-- was 2 in our source
13ac6a  ldr.w r0, [r8, #4]         ; enemies
13ac6e  ldr   r0, [r0, #0]         ; enemies[0]
13ac70  blx   0xfff2b5dc           ; enemies[0]->setActive(true)  <-- missing in ours
13ac74  ldr   r0, [r4, #24]        ; m_pLevel
13ac76  movs  r1, #0
13ac78  blx   0xfff2ad30           ; getDockingTarget(0)
13ac7c  mov   r4, r0
...                                 ; ->setName(getText(3211))
```

Original state 1 `0x13ebec..0x13ebf6`:

```
13ebec  ldr   r0, [sp, #84]        ; player
13ebee  blx   0xfff2dc10           ; isDockedToDockingPoint()
13ebf2  cbz   r0, 0x6ee0
13ebf4  movs  r0, #2
13ebf6  b.n   0x6f1c               ; m_nState = 2
```

Our source had the two bodies swapped: state 0 jumped straight to 2 and never called
`setActive`, while state 1 called `setActive` instead of advancing the state.  After the fix our
state-0 body is instruction-for-instruction identical to the original apart from one register
(`mov r4,r0` vs `mov r5,r0`), and state 1 matches too.  This is a real mission-progression bug:
the freighter's `setActive(true)` was delayed by one state and the state counter skipped 1.

## Landed change 2 — case 40 state 0: bogus `m_nScriptTime = 0`

The original state-0 body (`0x13a4d8..0x13a622`) contains **no** store to `this+8`; it stores
only shipGroup (`str r6,[r0,#40]`), the two HUD/radar bytes, `m_bCinematicBreak` and
`m_nState`.  Our source emitted `strd r9,r9,[sl,#8]`.  Removing the store is size-neutral and
drops the process diffbytes from 54581 to 54574.

## Per-case diff map (new evidence)

Decoded the original dispatch at `0x13879e..0x13885e` and both `tbh` tables
(`0x138a8a` cases 131..145, `0x138c3e` cases 64..73) and compared per-case normalised streams
against our own dispatch (`0x1387da`, tables `0x138a5e`/`0x138c16`).  The body order is identical
(confirmed wave 128), but the state-0 branch polarity differs for the mid cases:

| case | original entry | original state-0 test | ours (baseline) |
|---|---|---|---|
| 40 | `0x13a4d8` | `cmp r0,#0; bne.w cold` (hot) | `cmp r0,#0; beq.w cold` (cold) |
| 91 | `0x13ac42` | `cmp r0,#0; bne.w cold` (hot) | `cmp r0,#0; beq.w cold` (cold) |
| 102 | `0x13a658` | `cmp r0,#0; bne.w cold` (hot) | `cmp r0,#0; beq.w cold` (cold) |
| 154 | `0x13a76e` | `cmp r0,#0; bne.w cold` (hot) | `cmp r0,#0; beq.w cold` (cold) |
| 92 | `0x139ffe` | `cbnz r1, cold` (hot) | `cbnz r1, cold` (hot) — already matches |

The source shape that produces the original polarity is the nested
`if (m_nState == 0) { if (cond) { ... } } else if (...)` rather than the compound
`if (m_nState == 0 && cond)`.  Tested on case 40: the polarity flips correctly
(`bne.w cold`), but clang then re-permutes the whole switch and the function lands on 57224 B
with raw 58.63, so it is not landable without a size compensator.

## Experiments measured this pass (not landed; all reverted)

Baseline: raw 60.102467171, process diffbytes 54577, size 57232.

| variant | raw % | size | process diffbytes |
|---|---:|---:|---:|
| **landed (case-91 state fix + case-40 timer removal)** | **60.086050** | **57232** | **54574** |
| case-91 state fix only | 60.087545 | 57232 | 54581 |
| case-40 timer removal only | 60.100972 | 57232 | 54586 |
| case-40 nested `if` state 0 (polarity flips) | 58.625749 | 57224 | — |
| case-40 `setVisible` moved state1 -> state0 | 59.963191 | 57232 | 54600 |
| case-40 state-4 `m_nState = 5` (instead of `speed = 0`) | 59.294633 | 57228 | 55213 |
| case-102 add `m_pCamera->setTarget(enemies[0]->geometry)` | 59.332571 | 57248 | 55080 |
| case-154 re-index `(*enemies)[i]` in the shipGroup loop | 49.934062 | 57372 | — |
| prologue accessors reordered to `messages, player, enemies` | 58.619231 | 57416 | — |

The prologue reorder is the most interesting negative: it reproduces the original's exact
accessor order (`getMessages` -> r5, `getPlayer` -> `[sp,#84]`, `getEnemies` -> r8) and the
prologue becomes instruction-faithful, but `messages` moving from r4 to r5 re-colours every case
body and grows the function by 184 B, which collapses `byte_exact` (4506 -> 2741) because every
later symbol shifts.  It is left for a pass that can also absorb the +184 B.

## Remaining root cause

Unchanged: global `MachineBlockPlacement` permutation plus the prologue register map
(`messages` r4 vs r5, `&Globals::status` r9 vs r4, `this` in `sl` vs `[sp,#88]`).  The case-0
branch-polarity family (40/91/102/154) is the largest remaining source-shape lever, but every
faithful shape change moves the global layout and needs a size-neutralising companion.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7668/score.py                         # 60.086050387
python3 /dev/shm/w7668/textdiff.py                      # process 54574 / whole .text 85438
```

Scratch helpers (`common.py`, `score.py`, `textdiff.py`, `sect.py`, `dumpaddr.py`,
`dumpours.py`, `casemap2.py`, `cmp_ranges.py`, `tbh.py`, `base.cpp`, `cur.cpp`) live in
`/dev/shm/w7668/` and are not part of the tree.
