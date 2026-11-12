# Radar::draw wave 190 (w13422) — raw-win hunt: no size-exact raw win found;
# the `const auto camZ` CSE is the remaining 4-byte shift in the enemy loop

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13422`, branch `wave190-w13422`,
base `master` `03b9dfe03`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13404.md` (spark `int top`, aligned-only), `-w13398.md`
(24000-box aligned win), `-w13381.md` (tractor-guard rotation), `-w13346.md` (music-block
layout) and `radar-draw-wave189-w13342.md` (campaign-in-`sl` body family).

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function address | `0x12e050` |
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,291** |
| instruction-index alignment | 1,841 |
| instruction count | 3,542 ours / 3,532 orig |
| `.text` differing bytes (whole `.so`) | 72,934 |
| `.ARM.extab` differing bytes | 603 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.rel.plt` | byte-identical |
| total differing bytes | 73,557 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. The new structural finding: `const auto camZ` is CSE'd into `s16`

The original reloads `cameraZ` (`[r4,#0x244]`) at **every** `calcDistance` call site:
`12e26c`, `12e97e`, `12ead4`, `12efda`, `12f6e6`, `12f9fe`. Ours reloads at five of them but
**not** at the enemy-loop site `12f6dc`: the source local

```cpp
const auto camZ = this->cameraZ;   // line 958, gas-cloud block
```

is hoisted to the gas-block preheader at `12f438` (`vldr s16,[r4,#0x244]`) and reused at the
enemy-loop `calcDistance` (`12f6fa vstr s16,[sp,#12]`). That costs the original's
`12f6e6 vldr s6,[r4,#0x244]` (4 B) and gives `cameraZ` the `s16` slot, pushing the enemy-loop
24000 constants to `s18/s20` instead of the original's `s16/s18`.

Removing the local and using `this->cameraZ` at the gas `calcDistance` (line 1010) reproduces
the original's VFP rotation exactly (`s16/s18` constants, `vldr s6` at `12f6e6`):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,291** | 1,841 | 9,908 |
| remove `const auto camZ`, use `this->cameraZ` | 6,366 | **1,818** | 9,908 |
| same + w13404 spark `int top` | 6,368 | **1,793** | 9,908 |

The aligned metric improves by 23-48 instructions, but raw regresses by 75: the gas block's
frame layout changes (the `Vector` temp moves `sp+0xe8` -> `sp+0xdc`) and its first 0x200 B
lose ~84 raw bytes, more than the enemy-loop region gains. This is the best aligned body
found so far and the obvious starting point for a future raw pass that can re-pin the gas
block's frame slots.

## 2. Sweeps measured this pass (all reverted; full `link_so -j2` + `fn_diff`)

Every row is a full incremental build (`ulimit -v 12000000`, `-j2`; scratch
`/home/debian/GalaxyOnFire2/scratch/w13422/`). The base is a strong local optimum: every
sweep bottoms out at exactly **6,291**.

| sweep | variants | best raw | note |
|---|---:|---:|---|
| enemy tractor guard, all 120 operand orders | 120 | 6,291 (`ABCDE`) | `ADEBC` = 6,293 / aligned 1,817 |
| every top-level `&&`/`||` chain in `draw`, all permutations (<=4 operands) | 333 | 6,291 | 20+ byte-identical |
| `__builtin_expect` on every plain `if` (weights 0 and 1) | 334 | 6,291 | many byte-identical |
| every `if/else` block inverted (`if (!c) {B} else {A}`) | 44 | 6,291 | music block = 6,295 |
| `otherLock` OR-chain permutations (both assignments) | 46 | 6,292 | `o2_20` |
| declaration order / type of `missionBlocksPlanets`, `campaign`, `alienOrbit`, `turretMode` | 20 | 6,291 | `long campaign` byte-identical |
| `const auto camZ` spellings (`float`, `volatile`, cast, pointer, `+0.0f`) | 8 | 6,291 | `float camZ` byte-identical |
| music-tail condition orders (`stop`/`inAlienOrbit`/`attacked`/`campaign==16`) | 4 | 6,296 | original order = 6,306 |
| music `current` local types / `expect(current==143)` | 4 | 6,291 | `unsigned` byte-identical |
| box re-associations (`a > b - c` -> `a + c > b`) at the planet and gas-spark boxes | 5 | 6,292 | `sx1`, `pb1` = 6,292 / aligned 1,837-1,839 |
| `navWreck`/`lockedEnemy`/`dockNavPtr` local aliases, timer threshold hoists | 6 | 6,291 | `t2`/`t3`/`nv3` byte-identical |
| music `FModSound *sound = Globals::sound` alias | 2 | 6,291 | local alias = 9,852 (size-fatal); declaration-only byte-identical |

The only raw values below 6,291 were never observed; the nearest misses are 6,292
(`sx1`, `pb1`, `o2_20`) and 6,293 (`tr_ABECD`, `tr_ABEDC`, `tr_ADCBE`, `tr_ADEBC`,
`o1_00`, `o1_04`, `o2_00`).

## 3. Where the residual lives (unchanged from w13346)

The raw diff is almost entirely shift artefact: the base is byte-identical to the original
from `12f6ea` to `12fc54` (delta 0, ~1,400 B, raw 64) and from `12e050` to `12e3cc`
(raw 11). The remaining ~6,200 raw bytes are in the planet/crosshair region
(`12e3ce`-`12ec0c`, delta -50..-78) and the music tail (`130250`-`130704`, delta
-138..-176), where the block placement differs. The first shift is still the
campaign-in-`sl` rotation at `12e3ce`; the second is the gas-block pool island at `12f488`
(ours) vs `12f20c` (original).

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6291 aligned= 1841 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13422/`: `Radar.base.cpp` (6,291 base),
`Radar.h.base`, `sweep.py`, `gen_tractor.py`/`tractor.pkl`, `gen_ops.py`/`ops.pkl`,
`gen_expect_if.py`/`expif.pkl`, `gen_inv2.py`/`inv2.pkl`, `cz_nolocal.cpp` (aligned 1,818),
`c1.cpp`..`c4.cpp`, `sx1.cpp`..`sx3.cpp`, `pb1.cpp`, `k1.cpp`/`k2.cpp`, `shiftprof.py`.
