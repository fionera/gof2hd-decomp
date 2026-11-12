# Radar::draw wave 185 (w12911) — expect/alias re-sweep on the 6,311 body: no size-exact raw win

**Status:** analysis complete, 2026-11-12; **no size-exact raw win found**, `src/`
left at the base. Worktree `wt/w12911`, branch `wave185-w12911`, base `master`
`f390b0e68` (wave-184 harvest 10). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave184-w12811.md` (the 6,315 -> 6,311 hull-bar cell),
`radar-draw-wave183-w12709/12710/12711.md` (lockedEnemy revert, campaign
`sl` analysis, arithmetic-order cells) and
`radar-draw-wave182-w12609/12610/12611.md` (planet-lock body, `radarFont()`
hoists, `hasTractor13` inversion). This note is self-contained: every number
below was measured in this worktree on the 6,311 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,311** |
| instruction-index alignment | 1,903 |
| instruction count | 3,544 ours / 3,532 orig |
| `.text` differing bytes | 74,591 |
| `.ARM.extab` differing bytes | 647 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.ARM.exidx` / `.rel.plt` / `.rodata` / `.data` / `.bss` | 0 / 0 / 0 / 0 / 0 |
| `.note.gnu.build-id` differing bytes | 20 |
| total differing bytes | 75,258 |
| file size | 2,187,328 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this
pass; only this note.

## 1. Expect sweep (all 32 sites, remove + weight-flip = 64 builds)

Every `__builtin_expect(EXPR, W)` in `draw` was removed (replaced by `EXPR`)
and weight-flipped. No edit beats 6,311.

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base | 6,311 | 1,903 | 3,544 | 9,908 |
| `rm4` (`screenX > centerX - field_0x128`) | 6,311 | 1,903 | 3,544 | 9,908 |
| `rm12` (inner `isDockingToPlanet` of the double expect) | 6,311 | 1,903 | 3,544 | 9,908 |
| `rm15` (`enemy->player->enemyFlagsLo`, lock-image arm) | 6,311 | 1,903 | 3,544 | 9,908 |
| `flip17` (`carriesFriendCargoFlag`) | 6,311 | **1,879** | **3,543** | 9,908 |
| nearest worse: `rm0` | 6,312 | 1,903 | 3,544 | 9,908 |
| `rm13` / `rm23` | 6,314 | 1,903 | 3,544 | 9,908 |
| `rm8` (freighter status-value guard) | 6,317 | 1,869 | 3,544 | 9,908 |
| `rm20` (`field_0x1a9`) | 6,320 | 1,894 | 3,544 | 9,908 |

`flip17` is raw-neutral but drops one instruction (3,543) and improves the
instruction-index alignment 1,903 -> 1,879; it is a free stepping stone for a
later pass, not a raw win.

## 2. Alias sweep (block-scoped local removal, 80 declarations)

Every local declaration in `draw` was inlined into its enclosing brace block
(single- and multi-use; assigned locals that would break were skipped, and
`.size()` member calls are excluded from the `size` substitution). 25
declarations are raw-neutral; none is a win.

Neutral (6,311 / 9,908): `system@41`, `size@101`, `left@102`, `top@103`,
`topOk@104`, `landmark@132`, `landmark@138`, `size@260`, `left@261`,
`cx@386`, `target@393`, `e@478`, `size@507`, `cargo@561`, `current@580`,
`total@597`, `current@928`, `level@753`, `size@776`.

Non-identical neutrals (same raw, different bytes):

| edit | raw | aligned | insn |
|---|---:|---:|---:|
| `blip@582` (`Sprite *blip = this->blipSprite`) | 6,311 | **1,888** | **3,543** |
| `size@628` / `size@676` / `size@776` (gas/asteroid crosshair) | 6,311 | 1,905 | 3,544 |
| `left@629` / `top@630` | 6,311 | 1,905 | 3,544 |
| `left@677` | 6,311 | 1,907 | 3,544 |

Nearest worse: `target@465` / `techFont@126` 6,312, `lmFont@120` 6,313,
`enFont@468` 6,314, `left@508` / `left@777` 6,319.

## 3. Add-hint sweep (170 plain conditions x 2 weights = 340 builds)

Every `if`/`while` condition without an existing expect was wrapped whole in
`__builtin_expect((cond), 0)` and `__builtin_expect((cond), 1)`. No win; the
best is neutral 6,311 (many conditions compile identically).

## 4. Sub-expression hint sweep (198 conditions, every top-level `&&`/`||`
operand x 2 weights)

No win; best neutral 6,311. The base operand order of the `otherLock` 4-term
OR, the `otherLockFlag` 3-term AND and the 4-term `campaign` guard was also
re-permuted exhaustively (24 + 6 + 24 = 54 builds): base order is the unique
best at 6,311; next best 6,313.

## 5. Combination sweeps

* 29 single-edit neutrals (4 expects + 25 aliases) x all pairs (406 builds):
  no pair below 6,311. The 23 build failures are dependency overlaps
  (`size`/`left`/`top` inlining).
* The 10 non-byte-identical neutrals (`li_swap`, `flip17`, `blip@582`,
  `size@628`, `left@629`, `top@630`, `size@676`, `left@677`, `level@753`,
  `size@776`) x all triples (120 builds): no triple below 6,311.
* Near-miss (raw <= 6,320) x neutral pairs (3 fails, 175 builds): best 6,312
  (`A_techFont_126 + *`). No win.

## 6. Other cell classes re-measured on the 6,311 body

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`).

### 6.1 `li_swap` (aligned-only, kept out of `src/`)

Swapping the two `lockImage` arms in the `enemy == locked` block (test
`enemyFlagsLo` first, then `carriesFriendCargoFlag`) is raw-neutral **6,311**
but improves the instruction-index alignment **1,903 -> 1,899** at size
9,908. Combined with every one of the 29 neutrals it stays 6,311. It is a
free stepping stone for a later pass, not a raw win.

### 6.2 Statement / block / declaration cells

* Adjacent same-indent single-line statement swaps (134 pairs): no win
  (best neutral 6,311; 8 dependency-order build failures).
* Adjacent multi-line statement/block swaps (33 pairs, new class): no win.
* If/else inversions of every brace-brace block (44 blocks): no win; best
  neutral 6,311. The big `field_0x1bc >= 1` if/else inversion is 6,315.
* 4 background `DrawImage2D` quadrant permutations (24): only the base order
  is 6,311.
* 6 level-getter assignment permutations (720): base order is the unique
  6,311; next best 6,315.
* 3 camera-assignment permutations (6): all neutral 6,311.
* first-3-statement (`plasmaInRange`/`lockConfirmed`/`SetColor`) permutations
  (6): base is 6,311; `SetColor` first is 6,313.

### 6.3 Spelling / type / arithmetic cells

* `!= 0` -> truthiness, `== 0` -> `!x`, `== 1` -> `!= 0`, `!= 1` -> `== 0`,
  `(int) i` -> `i`, `(uint32_t)` -> `(unsigned)`, `(i & 0x7fffffff)` -> `i`,
  `size * 2` -> `size + size`, `size >> 1` -> `size / 2`, `++i`/`i++`,
  `nullptr` -> `0`, `false` -> `0`, `true` -> `1`, `+=` -> `= x +`:
  no win; the interesting spellings are byte-identical.
* `int left` (was `float`) in the landmark crosshair block: 6,314 (worse).
  `int topOk`, inline `topOk`, `float top` are byte-identical.
* `missionBlocksPlanets` explicit `else { = 0; }`, ternary form, and
  declaration after the mission block: all byte-identical 6,311.
  `bool mbpFlag` + copy: 6,424 / 9,916.
* `alwaysEnemySeen`/`alwaysEnemy`/`bossSeen`/`enemyHit` type flips: grow or
  worsen (as in earlier waves).
* Music-tail condition reorders (`inAlienOrbit` first / campaign first /
  `inAlienOrbit`+campaign): 6,326-6,327 (worse). `field_0x1bc > 0`,
  `!= 0`, `<= 2`, `<= 4` spellings: neutral 6,311.
* `(planetLockTimer < 1) || (missionBlocksPlanets != 0)`: 6,427 / 9,912;
  the bitwise `|` form is required for size exactness.
* Planet-lock `blip`/`frame` structure variants (direct `this->blipSprite`,
  declaration order, store order): best neutral 6,311 (`int frame; Sprite
  *blip;`), others 6,423-6,433 and/or size-fatal.

### 6.4 CSE / alias introduction

* `Station *guardStation = Globals::status->getStation()` in the landmark
  guard: 8,472 / 9,896.
* `KIPlayer *lm = this->landmarks->data()[i]` in the landmark loop:
  8,548 / 9,900; `Array<KIPlayer*> *lms = this->landmarks`: 9,450 / 9,892.
* `int *stationData = stations->data()` and `SolarSystem *sys = ...` in the
  planet loop: byte-identical 6,311.
* `int ccm = Globals::status->getCurrentCampaignMission()` in the campaign
  block: 9,159 / 9,888.
* Per-site `radarFont()` hoists at the 5 remaining `DrawString(radarFont(),`
  sites plus the multi-line planet-name site: 6,311-6,329; none wins.

## 7. Campaign-allocation experiment (analysis only, not landed)

Removing the single-use `int campaign` local entirely (calling
`Globals::status->getCurrentCampaignMission()` directly at the `campaign >= 2`
test) yields a **9,904 B** function with **3,531 instructions** (one fewer
than the original's 3,532) and raw 9,220. The extra instruction is lost
because clang tail-merges the two landmark distance `DrawString` calls into
one shared call site (the variant has 8 `PaintCanvas::DrawString` calls vs the
original's 9). The stack-slot rotation is *not* fixed (the zero still takes
`sl`, `turretMode` still lands at `sp+0x98`), so the campaign local is not the
lever by itself. This shape is a possible starting point for a later pass that
restores the folded `DrawString` (e.g. by making the two call arguments differ
in a way that survives optimization) and adds the missing 4 B.

## 8. Remaining residual (unchanged)

The dominant residual is still the `campaign` allocation: the original keeps
`campaign` in `sl` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses
`sl` for the stations array, with the planet-loop index in `fp`; ours spills
`campaign` to `sp+0x90`, keeps `stations` at `sp+0x84` and uses `sl` for the
loop index. The stack-slot rotation (`otherLock`/`turretMode` at `0x94`/`0x98`
vs the original's `0x90`/`0x94`) and the `bossSeen` `subs/clz/lsrs` vs
`it eq; moveq` materialisation follow from that cascade. The music-tail
cold-block layout (ours places the `field_0x1bc < 1` else block after the
epilogue) and the tail literal-pool placement (`1302dc` vs `130330`) also
remain. The function has 12 extra instructions (3,544 vs 3,532) and one extra
exception landing pad (7 vs 6), the latter caused by one extra `String`
temporary with a destructor.

## 9. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6311 aligned= 1903 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12911/` — `Radar.base.cpp`
(6,311 base), `harness.py` (build+measure), `sweep_expect.py`,
`sweep_alias2.py`, `sweep_addexpect.py`, `sweep_subexpect.py`,
`sweep_spell.py`, `sweep_perm.py`, `sweep_blocks.py`, `sweep_invert.py`,
`sweep_stmt.py`, `combo.py` / `try3.py` / `try5.py` / `try10.py` (combination
sweeps), `try2.py`-`try14.py` (targeted cells), `heat.py` / `aldiff.py` /
`calls.py` / `imm.py` (diff aids), and the `*_results*.json` dumps.
