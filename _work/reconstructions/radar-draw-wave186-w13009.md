# Radar::draw wave 186 (w13009) — branch/landing-pad pass on the 6,311 body: no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13009`, branch
`wave186-w13009`, base `master` `2d02567f6` (wave-185 notes commit). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave185-w12911.md` (expect/alias re-sweep), the wave-182/183
planet-lock notes (`radar-draw-wave182-w12609.md`, `radar-draw-wave183-w12709.md`)
and `radar-draw-wave184-w12811.md` (hull-bar cell). This note is self-contained:
every number below was measured in this worktree on the 6,311 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,311** |
| instruction-index alignment | 1,903 |
| instruction count | 3,544 ours / 3,532 orig |
| `.text` differing bytes (whole `.so`) | 74,402 |
| `.ARM.extab` differing bytes | 647 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.ARM.exidx` / `.rel.plt` / `.rodata` / `.data` / `.bss` | 0 / 0 / 0 / 0 / 0 |
| `.note.gnu.build-id` differing bytes | 20 |
| total differing bytes | 75,069 |
| file size | 2,187,328 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` and `DRIFT: 0 field(s)` with `src/` at the base. No code change is
committed by this pass.

## 1. Assigned cells and every measured variant

All builds are a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`). "identical" means byte-identical to the base build.

### 1.1 Planet-lock guard / `field_0x1a8` dock guard / +8 B site

The wave-182 landed shape is the base: plain dock guard, `expect(!lockConfirmed, 0)`
sound guard, order `store; dock; sound`. Re-measured spellings on the 6,311 body:

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base | **6,311** | 1,903 | 3,544 | 9,908 |
| `field_0x1a8 != 0` | 6,311 | 1,903 | 3,544 | 9,908 |
| `(bool) field_0x1a8` | 6,311 | 1,903 | 3,544 | 9,908 |
| `field_0x1a8 == 1` | 6,311 | 1,904 | 3,544 | 9,908 |
| sound `lockConfirmed == 0` | 6,311 | 1,903 | 3,544 | 9,908 |
| sound `expect(lockConfirmed,1) == 0` | 6,311 | 1,903 | 3,544 | 9,908 |
| `lockConfirmed = true` | 6,311 | 1,903 | 3,544 | 9,908 |

Inner `if (planetLockTimer > field_0x1b8)` block, mission-guard placement (the
wave-185 `plsweep` cross product did not vary this guard):

| edit | raw | insn | size |
|---|---:|---:|---:|
| base (`missionBlocksPlanets` first, then store/dock/sound) | **6,311** | 3,544 | 9,908 |
| mission guard moved after store/dock/sound | 6,910 | 3,543 | 9,908 |
| `if (!missionBlocksPlanets) {core} else {hud; goto}` | 6,311 | 3,544 | 9,908 (identical) |
| `if (missionBlocksPlanets) {hud; goto} else {core}` | 6,311 | 3,544 | 9,908 (identical) |

`field_0x14` guard spellings: `if (field_0x14)` / `else if (!field_0x14)` /
`field_0x18 = nullptr` / `store field_0x14 = 0` first → all 6,311, 3,544, 9,908
(the first two and the last are byte-identical; `store`-first is identical).
A `bool same` hoist in the `field_0x14 == field_0x18` test is raw-neutral but
alignment-worse (1,907).

`planetLockTimer`/`missionBlocksPlanets` spellings: `| missionBlocksPlanets`,
`| (missionBlocksPlanets == 1)`, `>= field_0x1b8 + 1`,
`planetLockTimer = planetLockTimer + mode`, `+= (int) mode` — all 6,311 /
3,544 / 9,908 (byte-identical).

### 1.2 `shipGroup` / `wreck` branches

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base | **6,311** | 1,903 | 3,544 | 9,908 |
| skip guard: `isActive` before `shipGroup == 10` | 6,311 | 1,908 | 3,544 | 9,908 |
| skip guard: `!(isDead() || isDying())` | 6,311 | 1,903 | 3,544 | 9,908 |
| `wreck = !(!hasCargo || (!isDead && !isDying))` | 9,847 | 3,578 | 3,639 | 10,240 |
| `wreck = (isDead || isDying) && hasCargo` | 9,807 | 3,554 | 3,640 | 10,196 |
| `if (!hasCargo && (isDead || isDying)) continue` | 8,574 | 1,916 | 3,546 | 9,916 |
| `steal` De Morgan (`!exclude && !wreck`) | 6,311 | 1,903 | 3,544 | 9,908 (identical) |
| `steal` De Morgan (`exclude == 0 && !wreck`) | 6,311 | 1,903 | 3,544 | 9,908 (identical) |
| skip guard De Morgan (drop inner parens) | 6,311 | 1,903 | 3,544 | 9,908 (identical) |
| skip guard flattened to one `||` chain | 6,311 | 1,903 | 3,544 | 9,908 (identical) |
| `isAlwaysEnemy()` before `field_0x42` | 8,549 | 1,983 | 3,538 | 9,900 |

`bossSeen` materialisation in the skip branch (the original emits
`cmp r0,#10; it eq; moveq r6,#1`; ours emits `subs r0,#10; clz; lsrs`):

| edit | raw | insn | size |
|---|---:|---:|---:|
| base `bossSeen = enemy->shipGroup == 10` | **6,311** | 3,544 | 9,908 |
| `(bool)` cast | 6,311 | 3,544 | 9,908 (identical) |
| `(x == 10) & 1` | 6,311 | 3,544 | 9,908 (identical) |
| `!!(x == 10)` | 6,311 | 3,544 | 9,908 (identical) |
| `(x == 10) != 0` | 6,311 | 3,544 | 9,908 (identical) |
| `10 == x` | 6,311 | 3,544 | 9,908 (identical) |
| `(x - 10) == 0` | 6,311 | 3,544 | 9,908 (identical) |
| `bossSeen \|= true` | 8,537 | 3,542 | 9,900 |
| `bossSeen = (x == 10) ? true : bossSeen` | 9,223 | 3,558 | 9,996 |
| `if (x == 10) bossSeen = true` | 9,213 | 3,557 | 9,988 |
| `bossSeen \|= (x == 10)` | 9,223 | 3,558 | 9,996 |
| `bossSeen = (x == 10) \|\| bossSeen` | 9,223 | 3,558 | 9,996 |
| `bossSeen & (x == 10)` | 9,170 | 3,558 | 9,992 |

### 1.3 Additional cells measured this pass

* **Declaration/evaluation order of the three initial locals** (`turretMode`,
  `alienOrbit`, `campaign`; not covered by the wave-185 720-perm of the six
  level-getter assignments): base `TAC` is the unique best.
  `TCA` 6,318; `ATC` 6,331; `ACT` 6,336; `CAT` 6,336; `CTA` 8,560 / 9,900.
* **Landmark-loop guard** `i == 2 || expect(data[i] == nullptr, 0) || !isVisible()`:
  base 6,311 (ours if-converts the first two tests into `ittt ne`; the original
  branches). Removing the `expect` 6,340; splitting into 2/3 `if`s 6,340;
  `!data[i]` 6,340; `data[i] == nullptr` first 6,311 (byte-identical);
  `isVisible()` first 8,417 / 9,912.
* **Landmark 4-term AND** `!landmarkHit && i != 3 && notTurretMode && notOtherLock`:
  all 24 permutations measured; base is the unique best 6,311. Nearest:
  `!landmarkHit && notTurretMode && i != 3 && notOtherLock` 6,346 (3,543 insn);
  `i != 3` first 9,482 / 9,876.
* **Music-tail 3-term OR** (`isAttackedByAliens`, `campaign == 16`,
  `inAlienOrbit`): base `aco` 6,311; `aoc` 6,316; `cao`/`coa`/`oac` 6,326;
  `oca` 6,327. The original's call order is `o a c`, but that shape is
  size-exact yet 15 raw worse.
* **Asteroid `field_0x11d/11e/11f/120` image-position chain**: base 6,311;
  `int x = screenX; int y;` 7,020 / 9,904; both initialised 7,556 / 9,940;
  `x`/`y` initialised but assignments kept 7,066 / 9,916; swapping the two
  `field_0x11e` arms 6,311 (identical); merging the duplicate `field_0x11f`
  arm 6,311 (identical).
* **`bool` -> `int` for every one of the 21 `bool` locals in `draw`** (one at a
  time). Byte-identical neutrals: `onPlanetCourse`, `topOk`, `isTarget`,
  `inSight`, `navWreck`, `hasDockNav`, `candidateWreck`, `ambient`. Worse:
  `alwaysEnemySeen` 8,997/9,920, `alwaysEnemy` 8,585/9,920,
  `turretMode` 9,232/9,856, `otherLockFlag` 9,448/9,896,
  `landmarkHit` 8,720/9,940, `planetHit` 9,227/9,952, `marked` 8,428/9,912,
  `bossSeen` 9,136/9,932, `enemyHit` 8,625/9,936, `wreck` 8,201/9,912,
  `gasHit` 7,959/9,904, `asteroidHit` 7,619/9,924.
* **Landmark `String` expression**
  `*getText(133) + String(": ") + String(String(getTecLevel()), false)`:
  base 6,311; `(A + B) + C` 6,311 (identical); `String(": ", false)` 6,311
  (identical); swapping the first two operands 8,462 / 9,904;
  `A + (B + C)` 8,496 / 9,904; dropping the inner `String` copy 9,492 / 9,828;
  reordering to `C + A + B` 8,524 / 9,920.

## 2. Landing-pad observation (`.ARM.extab`)

The wave-185 note's "13 vs 12 landing pads" is reproduced with the compact-LSDA
decoder in scratch (`extab3.py`): the LSDA span is 132 bytes for both, but ours
decodes 13 pad entries vs the original's 12. The extra entry is the tail
`pad2` in ours (`words ['0x19']` region), i.e. an extra call-site range, not an
extra destructor target: both have the same five distinct `String::~String`
targets (`sp+0xe8`, `sp+0xc4`, `sp+0xb8`, `sp+0xdc`, `sp+0xd0`).

The likely cause is the stack-slot colouring of the `calcDistance` `String`
temporary in the `enemy == locked` arm: the original uses `sp+0xe8`
(`12efca add r5, sp, #232`), ours uses `sp+0xdc` (`12efce add r5, sp, #220`).
That rotates the live set for the surrounding call-site ranges and produces the
extra range. No source spelling tried in this pass moved the slot (the `String`
expression variants above all keep the extra pad), so this remains open.

## 3. Residual analysis (unchanged)

The dominant residual is still the `campaign` allocation. The original keeps it
in `sl` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`); ours spills it to
`sp+0x90` (`12e0f8 str r0,[sp,#144]`) and reloads it at the test
(`12e38a str r0,[sp,#148]; 12e38c ldr r0,[sp,#144]; 12e38e cmp r0,#2`), two
extra instructions at that site. `sl` is instead used for `missionBlocksPlanets`
(orig keeps that in `fp`) and later for the planet-loop index (orig `fp`).
This cascades into the whole stack-slot rotation (`turretMode` at `sp+0x98` vs
the original `sp+0x94`, `otherLock`/`marked` at `sp+0x94` vs `sp+0x90`).

Other visible shape residuals, all re-confirmed this pass:

* `bossSeen` materialised as `subs/clz/lsrs` instead of the original
  `it eq; moveq` (same 6 B, different bytes).
* The if-converter in ours emits `ittt eq` conditional loads at `12fec4`
  (`navWreck || dockNavPtr->field_0x24`) and `12ff26`, where the original
  branches.
* The music-tail cold-block layout: ours places the `field_0x1bc < 1` else
  body (`field_0x54 = 0`, ambient bitmask) after the epilogue; the original
  interleaves it before the epilogue (`1302f0` vs ours `1304e2`).
* The landmark guard `i == 2 || data[i] == nullptr` is if-converted in ours
  (`ittt ne`) but branchy in the original.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6311 aligned= 1903 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # DRIFT: 0 field(s)
```

## 5. Scratch inventory (`/home/debian/GalaxyOnFire2/scratch/w13009`)

* `Radar.base.cpp` — exact base source (raw 6,311).
* `measure.sh <cpp>` — copy into `src/`, `link_so -j2`, run `tools/fn_diff.py`.
* `decl.py` (3-local order), `lmguard.py` (landmark guard), `andguard.py`
  (4-term AND, 24 perms), `boss.py` / `boss2.py` (`bossSeen` forms),
  `musicor.py` (3-term OR), `plblock.py` / `pl2.py` / `pl3.py` / `pl4.py`
  (planet-lock / dock / `field_0x14`), `ships.py` (`shipGroup`/`wreck`/skip),
  `astblock.py` / `astblock2.py` (asteroid `field_0x11d` chain),
  `bools.py` (all `bool` -> `int`), `str.py` (landmark `String` expression).
* `dd.py` / `dd2.py` + `dd.txt` / `dd2.txt` — aligned orig-vs-ours disassembly
  diff (dd2 includes source line numbers), base build.
* `runs.py` / `runs.base.txt` — per-byte differing-run census (382 runs,
  6,311 bytes).
* `netextra.py` — per-`0x100`-block net instruction surplus/deficit.
* `extab.py` / `extab2.py` / `extab3.py` / `extab4.py` — `.ARM.extab`/`.exidx`
  parsers (compact-LSDA pad counts).
* `orig.dis` / `ours.dis` — full function disassembly dumps (base).
