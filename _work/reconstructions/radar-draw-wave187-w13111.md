# Radar::draw wave 187 (w13111) — expect/alias re-sweep on the 6,311 body: no size-exact raw win

**Status:** analysis complete, 2026-11-12; **no size-exact raw win found**, `src/`
left at the base. This pass commits only this note. Worktree `wt/w13111`, branch
`wave187-w13111`, base `master` `8b5d4281c` (wave-186 harvest 6). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave186-w13011.md` (the same expect/alias re-sweep plus
the MIR spill-weight evidence), `radar-draw-wave186-w13010.md` (campaign
`sl` vs `sp+0x90` greedy trace), `radar-draw-wave185-w12911.md` (expect/alias/
combination sweeps), `radar-draw-wave183-w12711.md` (arithmetic-order cells)
and `radar-draw-wave184-w12811.md` (the 6,315 -> 6,311 hull-bar cell).

`Radar.cpp` has not changed since wave-184 harvest 2 (`00e525cb3`); the
wave-185/186 harvests did not touch it, so this is the identical body swept by
w12911 and w13011. Every number below was measured in this worktree on the
6,311 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,311** |
| instruction-index alignment | 1,903 |
| instruction count | 3,544 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this
pass.

## 1. Expect sweep (31 sites, remove + weight-flip = 62 builds)

Every `__builtin_expect(EXPR, W)` in `draw` removed (replaced by `(EXPR)`) and
weight-flipped. Reproduces w13011/w12911 exactly: no edit beats 6,311.

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base | 6,311 | 1,903 | 3,544 | 9,908 |
| `e4_rm` (`screenX > centerX - field_0x128`) | 6,311 | 1,903 | 3,544 | 9,908 |
| `e14_rm` / `e14_flip` (`enemy->player->enemyFlagsLo`) | 6,311 | 1,903 | 3,544 | 9,908 |
| `e16_flip` (`carriesFriendCargoFlag`, weight 0 -> 1) | 6,311 | **1,879** | **3,543** | 9,908 |
| nearest worse: `e22_rm` | 6,314 | 1,903 | 3,544 | 9,908 |
| `e29_flip` | 6,435 | 1,908 | 3,545 | 9,908 |
| `e24_rm` | 6,487 | 1,808 | 3,541 | 9,916 |

`e16_flip` is the same raw-neutral / aligned-improving stepping stone reported
by w12911 and w13011; it is not a raw win on its own.

## 2. Alias sweep (block-scoped local removal, 97 declarations)

Every initialised local declaration in `draw` was inlined into its enclosing
brace block (single- and multi-use; assigned locals skipped; `.size()` member
calls excluded from the `size` substitution). 18 declarations are raw-neutral;
none is a win.

Byte-identical neutrals (6,311 / 1,903 / 3,544 / 9,908): `system@42`,
`size@102`, `left@103`, `top@104`, `topOk@105`, `landmark@133`,
`landmark@139`, `size@261`, `left@262`, `cx@387`, `target@394`, `e@479`,
`size@509`, `cargo@562`, `current@581`, `total@598`, `level@754`,
`size@777`.

Non-identical neutrals (same raw, different bytes):

| edit | raw | aligned | insn |
|---|---:|---:|---:|
| `blip@583` (`Sprite *blip = this->blipSprite`) | 6,311 | **1,888** | **3,543** |
| `size@629` / `left@630` / `top@631` | 6,311 | 1,905 | 3,544 |
| `size@677` / `level@754` / `size@777` | 6,311 | 1,905 | 3,544 |
| `left@678` | 6,311 | 1,907 | 3,544 |

Nearest worse: `race@990` 6,415, `distance@842` 7,008, `index@837` 7,124.

## 3. Combinations of the neutral cells

* All pairs of the 18 raw-neutral cells (153 builds): no win; best 6,311
  (`e16_flip + e4_rm`, aligned 1,879, insn 3,543).
* Each anchor (`e16_flip`, `e14_flip`, `e4_rm`) combined with every one of the
  159 single expect/alias edits (474 builds): no win; best 6,311
  (`e16_flip + *`, aligned 1,879, insn 3,543). This is the same conclusion as
  w12911's pair/triple sweeps, re-measured on the current body.
* All pairs of the 26 size-exact edits with raw <= 6,320 (325 builds): no
  win; best 6,311 (`e14_flip + e16_flip`, aligned 1,879, insn 3,543). The
  nearest worse pair is `e0_rm + e16_flip` 6,312.
* All triples of the eight closest expect edits (`e0_rm`, `e12_rm`,
  `e16_flip`, `e14_flip`, `e4_rm`, `e22_rm`, `e19_rm`, `e8_rm`; 56 builds):
  no win; best 6,311 (`e16_flip + e14_flip + e4_rm`).

## 4. Additional cells measured this pass (all size-exact unless noted)

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13111/`).

### 4.1 Zero / prologue split

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| `unsigned char zero = 0; plasmaInRange = zero; lockConfirmed = zero;` | 6,311 | 1,903 | 3,544 | 9,908 |
| `int zero = 0; ...` | 6,311 | 1,903 | 3,544 | 9,908 |
| `const unsigned char zero = 0; ...` | 6,311 | 1,903 | 3,544 | 9,908 |
| zero declared after `SetColor` | 6,311 | 1,903 | 3,544 | 9,908 |
| `plasmaInRange = false; lockConfirmed = false;` | 6,311 | 1,903 | 3,544 | 9,908 |
| `(uint8_t) 0` stores | 6,311 | 1,903 | 3,544 | 9,908 |
| `lockConfirmed` store before `plasmaInRange` | 6,313 | 1,903 | 3,544 | 9,908 |

Splitting the shared zero does not change the allocator's choice: the zero
still takes `sl` and `campaign` still spills to `sp+0x90`.

### 4.2 `missionBlocksPlanets` / `campaign` / `otherLock` spellings

* `missionBlocksPlanets` init: `(bool)false`, `!!0`, `0^0`, `0|0`, `0&0`,
  `0-0`, `0*0`, `0<<1`, `false ? 1 : 0`, `long`, `short`, `int32_t`,
  `unsigned int`, `register int` — all byte-identical 6,311 / 9,908.
  `long long` 9,134 / 9,920; `const int` fails to build.
* `campaign` type: `bool` 9,657 / size **8,572** (fatal); `char` /
  `unsigned char` / `uint8_t` 8,381 / 9,916 (fatal); `short` 8,445 / 9,912
  (fatal); `unsigned` / `unsigned int` 6,312 / 9,908; `long long`,
  `int32_t`, `register int` byte-identical 6,311.
* `campaign` expression: parentheses, `+0`, `|0`, `^0`, `*1`, `/1`, a
  self-assignment, and a trailing label are all byte-identical 6,311. The
  `$r0` copy hint is not removable by source spelling.
* `alienOrbit` type: `bool` 6,319 / insn 3,543 (aligned 1,875); `char`,
  `unsigned char`, `uint8_t`, `unsigned`, `long`, `short`, `int32_t`,
  `register int` byte-identical 6,311.
* `otherLock` type: `char`, `unsigned char`, `uint8_t`, `short`,
  `unsigned short`, `int8_t`, `int16_t`, `long`, `unsigned`, `int32_t`,
  `unsigned int`, `register int` byte-identical 6,311; `const int` fails.
  `otherLockFlag` as `char`/`short`/`long`/`unsigned` -> 9,448 / size 9,896
  (fatal). `notTurretMode` / `notOtherLock` as `char`/`uint8_t`/`short`/
  `unsigned`/`long` byte-identical.
* `otherLockFlag = field_0x14` (pointer truthiness), `!!field_0x14`,
  `(bool) field_0x14`, `field_0x14 != nullptr`; `otherLock = (int)
  otherLockFlag`, `otherLockFlag + 0` — all byte-identical 6,311.
  `otherLock = !!otherLockFlag` is 6,311 / aligned 1,904.

### 4.3 Declaration-position sweeps

* `int campaign = ...` moved to every position in the early statement region
  (24 builds): base is the unique best 6,311. Nearest: after `turretMode`
  6,318; after `alienOrbit` 6,318; after the level getters 6,388; before
  `Mission *mission` 8,612 / size 9,900; after the transform inverse
  6,435.
* `int missionBlocksPlanets = false;` moved among the early declarations:
  byte-identical 6,311.
* `bool alwaysEnemySeen` / `bool alwaysEnemy` moved among the early
  declarations: byte-identical 6,311.

### 4.4 `stations` / crosshair / global aliases

* `Array<int> *stations` spellings: `*const`, `const Array<int> *`, `auto`,
  extra parentheses, a two-step `SolarSystem *ss` hoist, and a
  `int *stationData = stations->data()` hoist are all byte-identical 6,311.
  `volatile` 9,869 / size 10,240.
* `Vector *crosshair = &PlayerEgo::crosshairPos;` (then `(*crosshair)`) and
  the reference form are byte-identical 6,311. The by-value `Vector
  crosshair` form is 9,357 / size 9,704 (fatal); `float crossX` 9,498 /
  9,700; `float crossY` 9,504 / 9,836.
* `Globals *gl = Globals::globals;` byte-identical 6,311. `Layout *lay =
  Globals::layout` 7,100 / 9,908; `PaintCanvas *cv` 9,370 / 9,916;
  `FModSound *snd` 9,405 / 9,788; `GameText *gt` 9,423 / 9,888;
  `Status *st` 9,549 / 10,128; `Level *lvl` 9,847 / 10,272;
  `StarSystem *sys` 9,435 / 9,844; `Player *pl` fails to build (type).

### 4.5 `enabled` / `SetColor` spellings

`enabled == 0`, `enabled == false` byte-identical 6,311; `enabled != 1`
8,465 / 9,916. `SetColor(-1)`, `SetColor(0xffffffffu)`, `SetColor((unsigned
int) 0xffffffff)` byte-identical 6,311.

## 5. Residual (unchanged)

The dominant residual is still the `campaign` allocation. The original keeps
`campaign` in `sl` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses
`sl` for the stations array with the planet-loop index in `fp`; ours spills
`campaign` to `sp+0x90`, spills `stations` to `sp+0x84`, and uses `sl` for the
planet-loop index. The stack-slot rotation (`otherLock`/`turretMode` at
`0x94`/`0x98` vs the original's `0x90`/`0x94`) and the `bossSeen`
`subs/clz/lsrs` vs `it eq; moveq` materialisation follow from that cascade.
The music-tail cold-block layout and the tail literal-pool placement also
remain. No source spelling of the expect/alias/type/position cells tried here
or in waves 185-186 changes the allocator's choice without changing the
function size.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6311 aligned= 1903 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13111/`: `Radar.base.cpp`
(6,311 base), `harness.py` (build+measure), `sweep_expect.py`,
`sweep_alias.py`, `edits.py`, `combine.py`, `combine2.py`, `try_cells.py`,
`try_types.py`, `try_ol.py`, `try_stations.py`, `try_pos.py`, `try_pos2.py`,
`try_cross.py`, `try_globals.py`, `try_misc.py`, and the `*_results*.json`
dumps.
