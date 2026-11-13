# LevelScript::process — wave 191b / w13696: case-105 s6 raw recovery

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13696`, branch
`wave191b-w13696`, base master `6e208a911` (raw **51,614**, aligned **21,594**,
`.text` diffbytes 71,340, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

w13687 reconstructed the case-105 state-6 teardown + camera block from Thumb and
paid raw +79 for aligned -113.  This pass re-derived the block from the binary,
confirmed it is instruction-identical to the original modulo register names, and
searched the block and its immediate neighbours for a size-exact,
faithful-order shape that recovers the raw cost.

## Why the faithful block costs raw +79

The block itself is instruction-for-instruction identical to the original
(`0x142582..0x1426c0`) except for register allocation.  The raw cost is *not*
inside the block: it is the block's **+0xe address shift**.  The preceding
enemies[2..4] positioning block is 0xe bytes longer in ours, so the case-105
block starts at `0x142590` instead of `0x142582` and every same-address byte in
the block differs.  In-block same-address diff:

| build | in-block diff (0x142590..0x1426cc) | teardown | camera |
|---|---:|---:|---:|
| old (pre-w13687) block | 221 / 316 | 92 / 136 | 129 / 180 |
| faithful (w13687) block | 299 / 316 | 127 / 136 | 172 / 180 |

The 0xe comes from the upstream positioning block using high registers
(`r8`/`r9`) for eight stack-temporary `add`s where the original uses low
registers (`r4`/`r5`), e.g. `add.w r9,sp,#256` (4 B) vs `add r4,sp,#256` (2 B).
The temporary *stack offsets* are identical (256, 208, 244, 344, 184, 196, 356,
172, 268, 160, 148, 94), so the upstream source is faithful; only the greedy
register allocator's colouring differs.  The `this` pointer is in `fp` in ours
vs `r4` in the original (the zero constant takes `r4` in ours because `player`
takes `r6`; original has `player` in `r5`, zero in `r6`, `this` in `r4`).

## Landed change

Two edits, both size-exact:

1. **case-105 state-6 store order** (the task's allowed "store order" lever):
   `m_bCinematicBreak`, `m_pRadar->enabled`, `player->stopShooting(0)`,
   `m_pHud->visible`.  The teardown *call* order is unchanged (stopShooting is
   still the last call); only the interleaving of the one call with the three
   disjoint stores changes.  Raw -3 / aligned +3 on its own.
2. **case-105 state-10 right-vector scaling** (a real semantic bug found by
   w13687 but not landed): the original scales the right-vector term by
   `1000.0f`; our source added the unscaled right vector and applied
   `*= 1000.0f` to the up vector only.  Moving `offset *= 1000.0f;` before the
   `*camPos += offset;` is size-exact, raw +1 / aligned -3.

The combination is a strict Pareto improvement: raw -2, aligned unchanged.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,614 -> 51,612  (-2)
aligned    21,594 -> 21,594  (0)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,340 -> 71,338 (-2); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Candidate table (all size-exact unless noted; extab 577, rel.plt 289b2a06
unless noted)

| candidate | raw | aligned | size |
|---|---:|---:|---:|
| baseline (w13687 landed) | 51,614 | 21,594 | 57,232 |
| old pre-w13687 block (unfaithful) | 51,535 | 21,707 | 57,232 |
| **state-6 store order + state-10 right*1000 (landed)** | **51,612** | **21,594** | **57,232** |
| state-10 right*1000 alone | 51,615 | 21,591 | 57,232 |
| state-6 store order alone (`p1302`) | 51,611 | 21,597 | 57,232 |
| `p1302` + resetCamera before setVisible + s10 | 51,610 | 21,599 | 57,232 |
| `p1302` + setTarget before setPosition + s10 | 51,604 | 21,617 | 57,232 |
| `p1302` + both + s10 | 51,602 | 21,622 | 57,232 |
| setTarget just before setPosition (unfaithful) | 51,605 | 21,619 | 57,232 |
| resetCamera before setVisible | 51,612 | 21,599 | 57,232 |
| resetCamera before setFreeze | 51,612 | 21,599 | 57,232 |
| resetCamera moved to block top | 51,611 | 21,599 | 57,232 |
| drop resetCamera + extra setTarget(player->geometry) | 51,611 | 21,600 | 57,232 |
| pointer/reference alias for m_tempVec/m_dirVec | 51,614 | 21,594 | 57,232 |
| named temporary per add | 51,614 | 21,595 | 57,232 |
| `Vector(m_dirVec * k)` add spelling | 51,614 | 21,594 | 57,232 |
| `m_nState` += / ++ / ++prefix | 51,614 | 21,594 | 57,232 |
| timer store order A/B | 51,614 | 21,594 | 57,232 |
| `stopShooting(false)` / `this->` prefix | 51,614 | 21,594 | 57,232 |
| local Vector accumulator | 52,526 | 21,652 | 57,216 |
| local Vector reuse | 51,674 | 21,643 | 57,232 |
| direct `ref2->getX()` without m_dirVec | 52,699 | 21,641 | 57,208 |
| one-expression `m_tempVec = pos + dir*… + right*… + up*…` | 52,673 | 21,670 | 57,208 |
| `m_camVec` scratch | 52,686 | 21,689 | 57,264 |
| `= m_tempVec + …` first add only | 52,698 | 21,667 | 57,256 |
| `= m_tempVec + …` second add only | 52,655 | 21,644 | 57,248 |
| `= m_tempVec + …` third add only | 52,682 | 21,641 | 57,264 |
| `= m_tempVec + …` all three | 52,653 | 21,668 | 57,264 |
| `6800.0f * m_dirVec` (scale-first) | 52,660 | 21,568 | 57,248 |
| state-7 moveForward ladder 4/3/2 | 51,614 | 21,601 | 57,232 |
| state-7 moveForward ladder 2/3/4 (no expect) | 51,615 | 21,594 | 57,232 |

### Store-order sweep (24 permutations of stopShooting / cinematic / hud / radar)

`p1302` = `[cinematic, radar, stopShooting, hud]` is the only permutation that
beats the faithful order: 51,611 / 21,597.  All six permutations that keep
`stopShooting` first are canonicalised to 51,614 / 21,594 (and to 51,615 /
21,591 with the state-10 fix).  The landed combination uses `p1302` + s10.

### Neighbour (upstream enemies[2..4] positioning) sweep

| candidate | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 51,614 | 21,594 | 57,232 |
| playerPos after the awake loop | 51,791 | 21,613 | 57,232 |
| cloak before setAIDisabled in the awake loop | 51,634 | 21,598 | 57,232 |
| awake last in the loop | 51,645 | 21,604 | 57,232 |
| awake loop first (before lightDir) | 52,527 | 21,593 | 57,224 |
| awake loop after the translate loop | 52,861 | 21,679 | 57,216 |
| `AEGeometry *g2` alias | 52,815 | 21,632 | 57,184 |
| local `Vector p3/p4` for the two chains | 52,838 | 24,709 | 57,248 |
| `* 30000.0f * -1.0f` translate scale | 52,799 | 21,666 | 57,208 |
| `1e5f` / `2.2e3f` literals | 51,614 | 21,594 | 57,232 |

All upstream reshapes that change codegen either grow the function or worsen
raw; the faithful upstream order is the local optimum.

### Other clusters measured (not landed)

* **case-105 state 9** (source ~3810): the original calls `revive()` *before*
  the three `nextInt(1000)` draws and assigns them x, y, z; our source draws
  x, z, `revive()`, y.  Size-exact, raw +7 / aligned -8.
* **case-102 state 3** (source ~3455): the original uses `getRightVector()` for
  the `* 0.04f` add and `getUpVector()` for the `* 0.004f` add.  Size-exact,
  raw +2 / aligned 0.
* **mission-1 intro** (source L947-953): original order is `forceUpdate`,
  `rotate`, `getPosition`, `setToSleep`; ours is `rotate`, `setToSleep`,
  `getPosition`, `forceUpdate`.  Full faithful reorder: raw 51,618 /
  aligned 21,585 (size exact).  getPosition/setToSleep swap alone: 51,615 /
  21,594.
* **prologue declaration order**: moving `enemies` before `messages` gives
  51,628; `player` first 51,626; `status` first 55,334 / 57,312.

## Conclusion

The residual raw cost is a global greedy-RA colouring (`this` in `fp` vs `r4`,
`player` in `r6` vs `r5`, the upstream stack temporaries in `r8`/`r9` vs
`r4`/`r5`), not a statement-order difference in the case-105 block.  No
size-exact, faithful-order shape flips it.  The landed store-order + state-10
combination recovers 2 raw bytes at zero aligned cost; the larger raw recovery
(`p1302` + setTarget-before-setPosition: -12) costs aligned +28.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13696/` (`harness.py`, `batch1.py` …
`batch28.py`, `*.out`, `old.so`, `new.so`, `orig_block.txt`).
