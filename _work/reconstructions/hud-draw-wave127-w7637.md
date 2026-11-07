# Hud::draw — wave 127 (w7637): faithful gamma reload vs frame/slot census

**Status:** research note, 2026-11-07 (wave 127). Worktree `wt/w7637`, base
`master 4bbe88776`. **No source change landed**: HEAD remains the best measured
variant at **9332 B / 96.587843 %** (85 hunks). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

This note extends `hud-draw-wave126-w7614.md` (gamma reload vs 224-byte frame)
and `hud-draw-wave125-w7520.md` (dock1 type tie-breaks). It re-measures the
gamma block on the current HEAD and isolates the two missing stack slots.

---

## 0. TL;DR

The faithful gamma spelling

```cpp
Status *st = Globals::status;
if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                   Globals::status->getCurrentCampaignMission()) > 0.0f)
```

does reproduce the original's 19-instruction gamma block byte-for-byte
(`ldr r6,[r0,#0]` / `ldr r4,[r6,#0]` / `ldr r0,[r6,#0]` reload), but the whole
frame shrinks **224 -> 216** and the function **9332 -> 9312 B**, collapsing the
official score to **75.17 %** (538 hunks).  The frame is load-bearing: any
non-9332 size shifts every following function and drops `byte_exact` from 4506
to ~4039.

Hoisting the station index with a `Globals::status` receiver restores 224 B and
the reload, but the receiver becomes the second `Globals::status` load:
**96.460879 % / 86 hunks**.  The all-`int` dock block also restores the 224-byte
frame and the original's exact stack-slot offsets, but is **4 B short**
(9328 B / 94.138 %).  No tested spelling keeps both the 224-byte frame and the
original's load-#1 receiver.

---

## 1. Measured baseline

```
_ZN3Hud4drawExxP9PlayerEgobjj orig=9332 ours=9332
  ratio=96.587843  insns=3149/3152  hunks=85  cost=133
```

The 85 hunks are: gamma block (5), shield/armour spill schedule (16), dock1
register/type tie-breaks (~40), second-bar scheduling (~20), EH pad (4).

---

## 2. Gamma variants (HEAD everything else)

| variant | size | insns | raw % | hunks |
|---|---:|---:|---:|---:|
| **HEAD (`st` for mission)** | **9332** | **3149/3152** | **96.587843** | **85** |
| G1 faithful reload (`Globals::status` for mission) | 9312 | 3149/3146 | 75.170770 | 538 |
| G2 faithful + `int stationIdx = st->getStation()->getIndex()` | 9312 | 3149/3146 | 75.170770 | 538 |
| G3 hoisted index, `Globals::status` receiver | 9332 | 3149/3152 | 96.460879 | 86 |
| G4 hoisted index via `Globals::status`, `st` receiver | 9312 | 3149/3146 | 75.170770 | 538 |
| G5 mission local loaded first, `st` receiver | 9332 | 3149/3152 | 96.460879 | 86 |
| G6 station + mission locals, `st` receiver | 9312 | 3149/3146 | 75.170770 | 538 |
| G7 station + mission locals both via `st` | 9332 | 3149/3152 | 96.587843 | 85 |
| G8 mission local via `st`, station inline | 9332 | 3149/3152 | 96.460879 | 86 |

G1's gamma block is instruction-identical to the original (same registers
`r6`/`r4`/`r5`, same `ldr r0,[r6,#0]` reload, same `str r6,[sp,#108]` spill).
G3's block is also reload-faithful but swaps the receiver/mission registers
(`mov r0,r5` / `mov r1,r4` instead of `mov r0,r4` / `mov r1,r5`).

---

## 3. The two missing stack slots

A stack-slot census (`[sp,#N]` operands anywhere in the function) shows HEAD
and the original are identical:

```
orig/HEAD: 0 4 8 12 16 20 24 28 32 40 60 64 68 72 76 80 84 88 92 96 100
           104 108 112 116 120 160 220
G1       : 0 4 8 12 16 20 24 28 32 40 56 60 64 68 72 76 80 84 88 92 96
           100 104 108 112 152 212
```

Aligning the two instruction streams by opcode (stack operands masked) maps
original slots `60..120` to G1 slots `56..112` (a **-4** shift) and original
slots `160..220` to G1 slots `152..212` (a **-8** shift).  The original
therefore has **two extra 4-byte slots**: one between offsets 40 and 60 and one
between 120 and 160.  Neither extra slot is referenced by any instruction in
the original; they are unused padding that keeps the frame at 224 B.

The all-`int` dock block (the original's real dock local types: no `sxth` on
`frameH`/`total`/`barW`/`top`) restores the 224-byte frame and the exact
original slot offsets, but compiles to 9328 B (4 B short) and scores 94.138 %
(130 hunks).  Its first difference is in the dock block: the original keeps
`barW` in `r8` and `barH` in `fp` (with a `mov r5,fp` preservation), ours uses
`r6`/`r8`.  The missing 4 B is an allocator artefact, not a missing source
statement.

---

## 4. Anchor and bar-order checks

- The `Globals::status &&` anchor at `field_0x4b8` is a genuine codegen hack:
  the original has no test at `0x1641a0` (it loads `field_0x4b8` directly at
  instruction 614).  Removing it drops the function to **9268 B / 86.47 %**;
  the anchor is load-bearing for the 224-byte frame.
- Second bar `String`-before-`halfW` (the original's decompiler order) is
  worse: HEAD 9328 B / 94.745 %, G3 9328 B / 94.618 %, G3 all-`int`
  9324 B / 94.359 %.  `halfW`-first is the allocator optimum.
- Moving the dock1 `String text` after `barW`/`barH` is worse (9344 B /
  92.63 % with G3).
- Caching `Globals::globals`, `Globals::Canvas` or `ego->player` in extra
  locals, `long long`/`int` copies of `frameTime`/`timeLeft`, `volatile`
  status pointers, and `const` status pointers all move away from the
  original allocation (9304-9372 B, 58-75 %).

---

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7637
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7637`: `fastscore.sh` (link + single-function
score in ~1 s), `score1.py` (raw ratio/hunks/opcode diff), `showrange.py`
(indexed side-by-side disassembly), `mapslots.py` (stack-offset alignment),
`sweep_gamma.py`, `sweep_dock1.py`, `sweep_dock2.py`.  Pristine HEAD source
snapshot: `/dev/shm/w7637/Hud.cpp.orig`.
