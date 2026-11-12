# Radar::draw wave 186 (w13011) — expect/alias re-sweep on the 6,311 body + MIR spill-weight evidence: no size-exact raw win

**Status:** analysis complete, 2026-11-12; **no size-exact raw win found**, `src/`
left at the base. Worktree `wt/w13011`, branch `wave186-w13011`, base `master`
`2d02567f6` (wave-185 harvest 9). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave185-w12911.md` (the same sweep on the same 6,311 body;
this pass re-ran it and adds the MIR evidence), `radar-draw-wave184-w12811.md`
(the 6,315 -> 6,311 hull-bar cell) and `radar-draw-wave183-w12709/12710/12711.md`.
`Radar.cpp` has not changed since wave-184 harvest 2 (`00e525cb3`); the wave-185
harvest did not touch it, so this is the identical body w12911 swept. Every
number below was measured in this worktree.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,311** |
| instruction-index alignment | 1,903 |
| instruction count | 3,544 ours / 3,532 orig |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this
pass; only this note.

## 1. Expect sweep (32 sites, remove + weight-flip = 64 builds)

Every `__builtin_expect(EXPR, W)` in `draw` removed (replaced by `(EXPR)`) and
weight-flipped. Reproduces w12911 exactly: no edit beats 6,311.

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base | 6,311 | 1,903 | 3,544 | 9,908 |
| `rm4` (`screenX > centerX - field_0x128`) | 6,311 | 1,903 | 3,544 | 9,908 |
| `rm12` (inner `isDockingToPlanet` of the double expect) | 6,311 | 1,903 | 3,544 | 9,908 |
| `rm15` (`enemy->player->enemyFlagsLo`, lock-image arm) | 6,311 | 1,903 | 3,544 | 9,908 |
| `fl12` / `fl15` | 6,311 | 1,903 | 3,544 | 9,908 |
| `flip17` (`carriesFriendCargoFlag`) | 6,311 | **1,879** | **3,543** | 9,908 |
| nearest worse: `rm0` | 6,312 | 1,903 | 3,544 | 9,908 |
| `rm13` / `rm23` | 6,314 | 1,903/1,908 | 3,544 | 9,908 |
| `rm8` (freighter status-value guard) | 6,317 | 1,869 | 3,544 | 9,908 |
| `rm20` (`field_0x1a9`) | 6,320 | 1,894 | 3,544 | 9,908 |

`flip17` is raw-neutral but drops one instruction (3,543) and improves the
instruction-index alignment 1,903 -> 1,879; still a free stepping stone, not a
raw win.

## 2. Alias sweep (82 local declarations, block-scoped inline)

Every initialised local declaration in `draw` was inlined into its enclosing
brace block (single- and multi-use; assigned locals skipped; `.size()` member
calls excluded from the `size` substitution). 24 declarations are raw-neutral;
none is a win. Non-identical neutrals (same raw, different bytes):

| edit | raw | aligned | insn |
|---|---:|---:|---:|
| `blip@583` (`Sprite *blip = this->blipSprite`) | 6,311 | **1,888** | **3,543** |
| `size@629` / `size@677` / `size@777` (gas/asteroid crosshair) | 6,311 | 1,905 | 3,544 |
| `left@630` / `top@631` | 6,311 | 1,905 | 3,544 |
| `left@678` | 6,311 | 1,907 | 3,544 |

Byte-identical neutrals (6,311 / 1,903 / 3,544): `size@102`, `left@103`,
`top@104`, `topOk@105`, `landmark@133`, `landmark@139`, `size@261`,
`left@262`, `cx@387`, `target@394`, `e@479`, `size@508`, `cargo@562`,
`current@581`, `total@598`, `size@629`, `left@630`, `top@631`, `size@677`,
`left@678`, `level@754`, `size@777`.
Nearest worse: `techFont@127` 6,312, `target@466` 6,312, `lmFont@121` 6,313,
`enFont@469` 6,314, `left@509` / `left@778` 6,319.

## 3. Additional cell classes re-measured on the current body

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13011/`).

### 3.1 Declaration order / type of the early locals

* `turretMode`/`alienOrbit`/`campaign` declaration permutations (5 non-base
  orders): `perm_021` 6,318, `perm_102` 6,331, `perm_120` 6,336,
  `perm_210` 6,336, `perm_201` 8,560 (size 9,900). Base order is best.
* `campaign` type: `long` / `int32_t` / `const int` byte-identical 6,311;
  `unsigned` / `uint32_t` / `unsigned int` 6,312; `short` 8,445 (size 9,912).
* `missionBlocksPlanets` type: `char` / `uint8_t` / `unsigned char` / `long`
  byte-identical 6,311; `bool` 6,424 / **size 9,916** (fatal); `unsigned int`
  byte-identical.
* `otherLock` / `notTurretMode` / `notOtherLock` as `char`/`uint8_t`/`unsigned`
  byte-identical; `hasDockNav` as `int`/`char`/`uint8_t`/`unsigned`
  byte-identical.
* `turretMode` as `int`/`char`/`uint8_t`/`unsigned` is **size-fatal**
  (9,856, 3,516 insn) — do not touch.
* `campaign` two-step init (`int campaign = 0; campaign = ...;`) 6,311 /
  aligned 1,905; `campaign` declared at top and assigned at the original point
  6,311 / aligned 1,905. Both still spill `campaign` (see §5).
* Moving `campaign` after the level getters: 6,388 (size exact); after the
  transform inverse: 9,083 / size 9,912; before `mission`: 8,612 / size 9,900;
  after `turretMode`: 6,318; after `alienOrbit`: 6,318. All worse.
* `radarFont()` replaced by `(unsigned int) (uintptr_t) Globals::font` at every
  site: build fails (the helper is used in a `static_cast` context elsewhere in
  the file); not a draw cell.

### 3.2 Condition-operand permutations (all top-level `&&`/`||` chains)

72 conditions in `draw` with a homogeneous top-level separator; every
permutation of 2–4 operands (323 builds). **No win**; ~20 permutations are
raw-neutral 6,311 and the rest are worse. Notable:
`c10_0312` (the 4-term `campaign >= 2` planet guard) 6,414; `c13_102` 6,311
(aligned 1,908); `c5_0132`/`c6_*`/`c8_*` landmark-loop conditions neutral.
The 4-term `campaign` guard was also permuted exhaustively in wave 185 with the
same result (base order unique-best).

### 3.3 Global aliases (whole-function)

Introducing a function-scope alias for a repeatedly loaded global/member is
size-fatal and much worse: `Sprite *blipSpriteAlias` 8,019 / size 9,796;
`FModSound *soundAlias` 8,076 / size 9,412; `Level *levelAlias` 9,499 /
size 9,812; `Status *statusAlias` 9,516 / size 9,708; `PaintCanvas
*canvasAlias` 9,646 / size 9,636.

### 3.4 Other local/statement cells

* Drop `otherLockFlag` (compute `int otherLock` directly): 9,448 / size 9,896.
* Drop `notTurretMode`/`notOtherLock` aliases: 9,532 / size 9,868.
* Drop `alwaysEnemy` (assign `alwaysEnemySeen = true` in the loop): 8,492 /
  size 9,912.
* Swap the `otherLockFlag` if/else bodies (negated condition): byte-identical.
* `landmarkHit`/`otherLock`/`notTurretMode`/`notOtherLock` declaration-order
  permutations: byte-identical.
* `missionBlocksPlanets` assignment spellings (`(x != 172)`, ternary, `!`,
  `if (x != 172) mbp = 1;`): byte-identical 6,311.
* `campaign >= 2` spellings (`> 1`, `2 <= campaign`, parenthesised,
  `campaign - 2 >= 0`): byte-identical.
* `cloud->isDead() || cloud->isDying()` order swap (the gas-cloud re-check):
  **6,312** (raw +1) but the call order then matches the original at that site.
  The swap changes only the two `blx` target bytes (offsets `0x15ae`, `0x15ba`
  relative to the function); it is not a win on its own.
* `this->field_0x1c == nullptr && wreck && this->tractorMode2` permutations:
  `tractorMode2` last 6,312 / 3,543 insn; `tractorMode2` first 6,313;
  `wreck` first 7,564 / size 9,904.

## 4. The 36-byte head is already matching

The first `0x37e` (894 B) of the function has only **36** differing bytes, and
they are all PC-relative branch offsets plus the three `fp`/`sl` register
choices (see §5). The remaining ~6,275 diff bytes are the layout/stack-slot
cascade that follows from the `campaign` allocation. Fixing `campaign` is the
only lever that can move the whole function; every local cell tried so far is
noise against it.

## 5. MIR evidence: why `campaign` is spilled (new in this pass)

`clang++ -mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN5Radar4drawEP6PlayerP3Hudi` (NDK r18b clang 7.0.2, the
exact TU flags) gives the machine IR after the Live Register Matrix and after
the Greedy Register Allocator. In the LRM dump:

* `campaign` is virtual register `%417` (`COPY $r0` immediately after the
  `Status::getCurrentCampaignMission` call at slot 2128B).
* `missionBlocksPlanets` is virtual register `%2481` (`t2MOVi 0` in the
  mission-type branches, `t2SUBri ...,172` + `t2MOVCCi ...,1` in the
  `type != 172` branch).

After greedy, `%417` is `t2STRi12 $r0, %stack.45` (spilled) and `%2481` is
allocated to `sl`. This is exactly our 6,311 body.

Reconstructing the natural loops from the MIR CFG (9 loops: landmark 46 blocks,
planet 67, enemy 124, gas 57, asteroid 66, plus four tiny ones) and computing
LLVM's `getSpillWeight = (isDef + isUse) * 10^loopDepth` per def/use gives:

| vreg | value | defs | uses | total weight |
|---|---|---:|---:|---:|
| `%2481` | `missionBlocksPlanets` | 8.0 | 6.0 | **14.0** |
| `%417` | `campaign` | 1.0 | 1.0 | **2.0** |

The greedy queue is highest-weight-first, so `missionBlocksPlanets` is
allocated before `campaign` and takes `sl`; `campaign` then loses the last
callee-saved slot and is spilled to `sp+0x90`. The original instead has
`campaign` in `sl` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and
`missionBlocksPlanets` in `fp` (spilled to `sp+0x70` at `12e3ba`), i.e. the
original's weight ordering must have been the other way round. No source
spelling of the `campaign` local tried in this pass or in wave 185 changes that
ordering; the value has a single use at the planet guard and is live across the
landmark loop, so its weight stays 2. Raising it would require a use inside a
loop, which the original binary does not show (`sl` is untouched between
`12e0f8` and `12e38a`).

The stack-slot rotation follows mechanically: original `otherLock` at `0x90`,
`turretMode` at `0x94`, `planetHit` at `0x98`, `stations` at `0x74`; ours
`campaign` at `0x90` (reused by `planetHit`), `otherLock` at `0x94`,
`turretMode` at `0x98`, `stations` at `0x84`. That is why almost every
instruction from `0x37e` on differs even where the source shape is identical.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6311 aligned= 1903 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

MIR dump (≈71 MB, delete after use):

```sh
.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  <exact Radar.cpp compile command from compile_commands.json> \
  -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN5Radar4drawEP6PlayerP3Hudi \
  -o /home/debian/GalaxyOnFire2/scratch/w13011/Radar.obj \
  src/game/weapons/Radar.cpp > /home/debian/GalaxyOnFire2/scratch/w13011/afterall.txt 2>&1
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13011/` — `Radar.base.cpp`
(6,311 base), `harness.py` (build+measure), `sweep_expect.py`,
`sweep_alias.py`, `sweep_types.py`, `sweep_types2.py`, `sweep_pos.py`,
`sweep_planetguard.py`, `sweep_condperm.py`, `sweep_globalalias.py`,
`sweep_locals.py`, `sweep_alloc.py`, `sweep_more.py`, `sweep_final.py`,
`try_cells.py`, `try_cells2.py`, `regions.py`, `side.py`, and the
`*_results*.json` dumps; `afterall.txt` / `draw.ll` are the MIR/IR dumps.
