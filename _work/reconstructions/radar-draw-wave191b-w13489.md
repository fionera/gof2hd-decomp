# Radar::draw wave 191b (w13489) — station-blip pointer spelling: raw 6,060 -> 6,059

**Status:** landed, 2026-11-12. Worktree `wt/w13489`, branch `wave191b-w13489`,
base `master` `f65db27f2`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191-w13489.md` (the enemy-guard `A D E B C` landing,
raw 6,060 / aligned 2,027) and `radar-draw-wave190-w13440.md` (the multi-line
blip-call permutation sweep; its §4 landed the station `sf, dr, rp` order).

## 0. The change

In the station-lock blip block the three sprite calls were spelled

```cpp
blip->setFrame(frame);
this->blipSprite->draw(1.0f, 1.0f);
this->blipSprite->setRefPixelPosition(...);
```

The local `blip` was loaded from `this->blipSprite` at the top of the block and
nothing writes that member in between, so the two spellings are the same
pointer.  Swapping *which* spelling each call uses (the call order is unchanged)
shifts the global register allocation:

```cpp
-                    blip->setFrame(frame);
-                    this->blipSprite->draw(1.0f, 1.0f);
+                    this->blipSprite->setFrame(frame);
+                    blip->draw(1.0f, 1.0f);
                     this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                           (int) PlayerEgo::crosshairPos.y);
```

No call order changes, so this is semantically identical (not just a codegen
lever).

## 1. Measurement

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,060 | **6,059** |
| instruction-index alignment | 2,027 | 2,027 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,804 | **71,803 (-1)** |
| `.ARM.extab` differing bytes | 582 | 582 (no growth) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.data` differing bytes | 0 | 0 |
| `.note.gnu.build-id` | 20 | 20 |
| total (text+extab+build-id) | 72,406 | **72,405 (-1)** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 2. How it was found

A full 6-order x 8-pointer-spelling sweep of the station blip triple
(`scratch/w13489/sweep_block.py station`) found exactly two size-exact 6,059
cells:

| variant (order / sf,dr,rp spelling) | raw | size |
|---|---:|---:|
| base (`sf,dr,rp` / B,A,A) | 6,060 | 9,908 |
| `sf,dr,rp` / **A,B,A (landed)** | **6,059** | 9,908 |
| `dr,rp,sf` / B,A,B | 6,059 | 9,908 (draw before setFrame, semantically wrong) |

The same 48-cell sweep of the enemy blip block (`sweep_enemy2.py`) and the
planet blip block (`sweep_block.py planet`) found no cell below 6,060: the enemy
matrix's best is 6,060 (`rp,sf,dr` A/A/A and `dr,rp,sf` A/A/A), the planet
matrix's best is 6,060 (`sf,rp,dr` with `rp`/`dr` via `blip`).  This confirms the
w13489 note's finding that the enemy-blip reorder is an aligned-only shape win
(raw +4) and that the planet-frame reassociations are raw losses.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6059 aligned= 2027 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
