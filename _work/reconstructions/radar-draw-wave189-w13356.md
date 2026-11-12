# Radar::draw wave 189 (w13356) — ternary-operand-order pass: `offset` select inverted,
# raw 6,309 -> 6,308 (size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13356`, branch `wave189-w13356`, base `master`
`6f5e6a957`. Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13331.md`, `radar-draw-wave189-w13335.md`,
`radar-draw-wave189-w13342.md` and `radar-draw-wave188-w13210.md`. This pass deliberately
avoided the register-allocation / planet-loop-latch cells (w13346) and worked the
landmark / asteroid / gas-cloud / music-tail regions instead.

## 0. The win

One source edit in the landmark-label block:

```cpp
-                int offset = i == 0 ? this->originX : this->originY;
+                int offset = i != 0 ? this->originY : this->originX;
```

| metric | base | after |
|---|---:|---:|
| raw same-address diff | **6,309** | **6,308** |
| instruction-index alignment | 1,858 | 1,858 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 73,939 | **73,938** |
| `.ARM.extab` differing bytes | 614 | 614 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total differing bytes | 74,573 | **74,572** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / parity 5,370. The function was already non-byte-exact, so `byte_exact` is
unchanged; the gain is in the raw residual only.

The inverted ternary makes clang emit the original's `it eq / moveq` select shape on the
two `originX`/`originY` loads instead of the base's `it ne` form, which removes exactly one
differing byte without changing the instruction count, size, section layout or PLT.

## 1. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | 6,309 |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| frame | `sub sp,#296` (`0x128`) |
| `mode` argument slot | `sp+0x64` (matches original) |
| `campaign` | spilled `sp+0x90` |
| shared zero | `sl` (`12e07a mov.w sl,#0`) |
| planet-loop counter `i` | `sl` |
| planet-loop `stations` | spilled `sp+0x84` |
| `.rel.plt` | byte-identical |
| `.text` differing bytes (whole `.so`) | 73,939 |
| `.ARM.extab` differing bytes | 614 (size/base exact at `0x1a3b78`, 156,312 B) |
| build-id | 20 |
| total differing bytes | 74,573 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

## 2. Cells measured this pass (all reverted except the landed one)

Every row is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13356/`).

### 2.1 Landmark / asteroid / gas-cloud statement and operand order

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,309** | 1,858 | 9,908 |
| **`offset = i != 0 ? originY : originX`** | **6,308** | 1,858 | **9,908** |
| `offset = i != 0 ? originY : originX` (re-measured after configure) | 6,308 | 1,858 | 9,908 |
| landmark `egoPos` before `targetPos` | 6,318 | 1,843 | 9,908 |
| `if (i <= 1)` -> `if (i < 2)` | 6,309 | 1,858 | 9,908 |
| `if (i == 0 && !alienOrbit)` -> `if (!alienOrbit && i == 0)` | 8,521 | 1,928 | **9,920** |
| crosshair-bounds `&&` reorder (`topOk` first) | 6,309 | 1,858 | 9,908 |
| `field_0x1c == nullptr && tractorAuto` swap | 6,500 | 1,867 | 9,908 |
| `(wreck && hasTractor13) || (field_0x1b0 && !field_0x1a8)` swap | 8,345 | 1,866 | **9,904** |
| `tractorMode2` reorder (`wreck` first) | 8,316 | 1,839 | **9,904** |
| `navWreck` `isDead`/`isDying` swap | 6,309 | 1,858 | 9,908 |
| `isDockingToAsteroid() && lockedAsteroid` swap | 6,610 | 1,862 | 9,908 |
| `field_0x10 != data()[best]` swap | 6,309 | 1,859 | 9,908 |
| `candidateWreck` `isDead`/`isDying` swap | 6,309 | 1,858 | 9,908 |
| gas-cloud 24000 comparisons `>`/`<` swap | 6,309 | 1,858 | 9,908 |
| `isAutoPilot() || otherLock || enemyHit` reorder | 7,304 | 1,836 | 9,908 |
| `__builtin_expect(cloud == locked, 1)` removed | 8,497 | 1,771 | **9,904** |
| `candidate == nullptr || candidate == lockedGas` swap | 7,829 | 1,815 | **9,912** |
| asteroid loop `isDead`/`isDying` swap | 6,314 | 1,858 | 9,908 |
| lockedStation else-branch fold (`if (null) field_0x28=0; timer=0;`) | 6,980 | 1,775 | **9,896** |
| asteroid-lock else-branch fold | 7,053 | 2,046 | 9,908 |

### 2.2 `otherLock` recompute (pre-gas-cloud) — all 24 OR permutations

The original's evaluation order at `12f40e` is `(dockNavPtr && !lockedEnemy)`, `field_0x1c`,
`lockedStation`, then the out-of-line `field_0x14` test at `12f8a6`. The base source order is
`lockedStation`, `(dockNavPtr && !lockedEnemy)`, `field_0x1c`, `field_0x14` (CABD), which is
*not* the original's order, yet it is the best raw value:

| permutation | raw | size |
|---|---:|---:|
| CABD (base) | **6,309** | 9,908 |
| DABC | 6,310 | 9,908 |
| CADB | 6,311 | 9,908 |
| ACBD | 6,318 | 9,908 |
| ABCD (original's order) | 6,320 | 9,908 |
| ADBC | 6,319 | 9,908 |
| B*/C-first-with-B-second | 8,035–8,466 | 9,912–9,948 |

ABCD matches the original's instruction *sequence* at `12f40a..12f41c` but sits 4 bytes
earlier (the preceding zero-store block is 4 B short), so it loses 11 bytes in block
`12f350` and wins nothing overall.

### 2.3 Music tail

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,309** | 1,858 | 9,908 |
| music guard `range \|\| mask \|\| expect(...)` (mask before expect) | 6,332 | 1,856 | 9,908 |
| music guard `range \|\| mask \|\| (current != 151 && alwaysEnemySeen)` (no expect) | 7,295 | 1,830 | **9,900** |
| `camZ` local removed, `this->cameraZ` at the call | 6,373 | 1,847 | 9,908 |

## 3. Where the residual still lives (unchanged by this pass)

The base is byte-identical over `12e1b9..12e30c` (343 B), `12f985..12fc54`, and most of the
tail from `130091`; the residual is block-placement/literal-pool shift driven by the
`campaign`-in-`sl` vs `sp+0x90` allocation (see w13210/w13331/w13335/w13342) plus the
`field_0x11d/11e/11f/120` x/y decision tree at `12fc40..12fca0`, where ours is 12 B shorter
than the original. The music tail (`130264+`) still uses `r8` for `FModSound*` and
`sp+0x60/0x68` for `alwaysEnemySeen`/`bossSeen` where the original uses `r9` and
`sp+0x5c/0x68`.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6308 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13356/`: `Radar.base.cpp` (6,309 base),
`base.so` / `base.blocks` (per-256-byte raw heat), `abcd.so` / `swap.so`,
`sweep_or.py` (24 `otherLock` permutations), `try.py` + `cells1.json` (the 19-cell
landmark/asteroid/gas-cloud batch), `rawblocks.py`, `gaps0.py`, `adiff_all.txt`.
