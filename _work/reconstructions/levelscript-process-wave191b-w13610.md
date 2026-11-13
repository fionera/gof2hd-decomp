# LevelScript::process — wave 191b / w13610: fresh cluster map + case-14 teardown swap

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13610`, branch
`wave191b-w13610`, base master `fcb09ff2c` (raw **51,513**, aligned 22,816,
`.text` 71,196, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Fresh contiguous diff-cluster map (51,513 bytes)

Same-address byte diff of the linked `.so`, maximal runs, mapped to source
lines through the unstripped object's DWARF line table
(`_work/reconstructions/levelscript_process_cluster_census.py`).
2,984 runs.  Top ten:

| # | address range | len | line | case | original at run start |
|---|---|---:|---|---|---|
| 1 | `0x13b412..0x13b4f1` | 224 | L1302 | 2 | `blx Layout::startFade` |
| 2 | `0x1434fc..0x1435d4` | 217 | L4703 | 154 | `cmp r0,#0` |
| 3 | `0x13d2da..0x13d396` | 189 | L850 | 10 | `ldr.w r4,[sl,#20]` |
| 4 | `0x13e257..0x13e309` | 179 | L1595 | 14 | `blx PlayerEgo::setComputerControlled` |
| 5 | `0x13ec0a..0x13ecb7` | 174 | L3301 | 3 | `blx RadioMessage::isOver` |
| 6 | `0x13fa2d..0x13fada` | 174 | L3065 | 92 | `vmov r1,s0` |
| 7 | `0x13c992..0x13ca39` | 168 | L4209 | 131 | `mov r1,fp` |
| 8 | `0x13d44a..0x13d4ef` | 166 | L889 | 12 | `vcvt.f32.s32` |
| 9 | `0x140177..0x14021c` | 166 | L5286 | 158 | `ldrd r0,r1,[r8]` |
| 10 | `0x13a8a7..0x13a949` | 163 | L4562 | 154 | `blx Level::getPlayer` |

Per-case aggregation (all runs, by source `case`):

| source case | diffbytes | runs |
|---|---:|---:|
| `?` (pre-campaign / shared tail) | 6,393 | 363 |
| case 157 | 5,883 | 307 |
| case 105 | 3,908 | 345 |
| case 154 | 3,479 | 177 |
| case 92 | 3,468 | 194 |
| case 102 | 3,421 | 168 |
| case 80 | 2,272 | 144 |
| case 125 | 1,640 | 70 |
| case 67 | 1,286 | 87 |
| case 16 | 1,265 | 86 |
| case 91 | 1,251 | 75 |
| case 14 | 1,241 | 78 |
| case 41 | 1,233 | 76 |
| case 42 | 1,033 | 70 |
| case 2 | 1,007 | 37 |

The largest runs are **block-placement artefacts**, not local source bugs: the
run start is a call/branch that the original places at that address while our
MachineBlockPlacement puts the same block hundreds of bytes away (e.g. run 1 is
the mission-2 `startFade` tail, which the original falls through at `0x13b412`
and we place at `0x13b4b0`).  The prologue register assignment is still the
dominant global contributor: original `this=sl, guard=fp, status=r4`; ours
`this=fp, guard=sl, status=r9` (see w13570 note).

## Sweeps run on this base (all exact NDK r18b `-Oz` compile + gold link)

Harness: `scratch/w13610/fast2.py` (per-candidate compile + gold link with the
checked-in object list) and `sweep.py` (4 concurrent compilers).  Every
candidate measured for raw / size / `.rel.plt`; only size-exact,
`.rel.plt`-identical cells below 51,513 are listed.

| sweep | range | candidates | result |
|---|---|---:|---|
| flip existing `__builtin_expect` hints | whole fn (67 sites) | 67 | none |
| add `__builtin_expect` to every single-line `if` | whole fn | 920 | none |
| d1 transpositions | 600-2600 | 730 | **1614/1615 = 51,512 (landed)**; 890/891 = 51,512 (dead add, rejected) |
| d2 transpositions | 600-2600 | 552 | 1613/1615 = 51,512 (same cell as landed) |
| d3/d4/d5 transpositions | 600-2600 | 1,103 | none |
| d2 left rotations | 600-2600 | 552 | none |
| d1 transpositions | 2600-5487 | 1,157 | 4855/4856, 4964/4965 = 51,512 (dead add, rejected) |
| d2 transpositions | 2600-5487 | 885 | 3235/3237, 4964/4966 = 51,512 (producer/consumer, rejected) |
| d3 transpositions | 2600-5487 | 709 | 3025/3028, 4614/4617 = 51,512 (producer/consumer, rejected) |
| d2 left rotations | 2600-5487 | 885 | 4964/4966 = 51,512 (stale vector, rejected) |
| independent-window rotations (3-6, disjoint identifiers) | 600-5487 | 3,360 | none |
| cross-control-boundary statement moves | 600-5487 | 375 | none below 51,512 |
| single-statement relocation in case-14 / case-102 / state-6 blocks | 1,605-1,617 / 3,391-3,396 / 770-785 | 196 | none |
| hill-climb (d1+d2) around 40 largest-run centres | ±8 lines | ~2,400 | only the known 890/891 and 4855/4856 dead adds |
| `float * Vector` -> `Vector * float` at the 3 `AEMath::mlEfRK` sites | L2153/L2503/L3182 | 4 | neutral / +1 |

## Landed change (case-14 state-1 teardown, source ~1614)

```cpp
                     player->setComputerControlled(true);
-                    m_pCamera->translate(1000.0f, 700.0f, 1000.0f);
                     m_pHud->visible = 0;
+                    m_pCamera->translate(1000.0f, 700.0f, 1000.0f);
                     m_pCamera->setTarget(player->geometry);
                     m_pCamera->setPosition(player->getPosition());
```

`m_pHud->visible` is a plain HUD flag store and `m_pCamera->translate` is a
camera transform; neither reads what the other writes.  The original emits
`translate` after `setTarget`/`setPosition` at `0x13aa9a` (its own order is
`hud=0` at `0x13aa44`, `setComputerControlled` at `0x13aa58`, `setTarget` at
`0x13aa76`, `setPosition` at `0x13aa8e`, `translate` at `0x13aa9a`), so moving
`translate` later is also closer to the original than the previous order.
This is the codegen-coincidence class accepted by the wave-190/191 notes: the
gate is the same-address byte diff.

Measured alone on the real build:

```
raw        51,513 -> 51,512  (-1)
aligned    22,816 (unchanged)
size       57,232 / 57,232   (exact)
.text      71,196 -> 71,195  (-1)
.ARM.extab 577 diffbytes (size/base exact, 156,312 B)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Rejected in the same pass (raw-lowering but semantically wrong)

* `L890<->L891` (51,512): `m_pCamera->setPosition(m_tempVec)` before the
  `m_tempVec += {-2000,-2000,-5000}` add makes the add dead; original
  `0x13d428` adds, `0x13d434` sets.
* `L4855<->L4856`, `L4964<->L4965` (51,512): same dead-add shape
  (`m_tempVec = getPosition()` moved after the `+=`).
* `L3235<->L3237`, `L3025<->L3028`, `L4614<->L4617`, `L4964<->L4966`
  (51,512): producer/consumer swaps — the intervening statement re-reads
  `m_dirVec` / `m_tempVec` and would see the wrong vector.

## Load-bearing PLT anchors (not removable on this base)

Two fake calls keep `.rel.plt` byte-identical and each is 16 B of the body:
`player->setAutoTurret(player->autoTurretIsEnabled());` (~L1837) and
`if (delta == 0x7d) …setStanding(0,100);` (~L2045).  Removing either gives
size 57,216 (-16) and a different `.rel.plt`; both must stay until the block
placement that supplies the same first-reference order is reconstructed.
