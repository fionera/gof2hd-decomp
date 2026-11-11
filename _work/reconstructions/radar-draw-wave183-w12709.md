# Radar::draw wave 183 (w12709) — branch/landing-pad pass: revert the second `lockedEnemy` cleanup order (raw 6,322 -> 6,321)

**Status:** landed, 2026-11-11. Worktree `wt/w12709`, branch `wave183-w12709`,
base `master` `9555474d8` (wave-182 harvest 5). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave182-w12609.md` (the planet-lock body / dock guard /
+8 B site), `radar-draw-wave182-w12610.md` (per-site `radarFont()` hoists),
`radar-draw-wave182-w12611.md` (the `hasTractor13` if/else inversion) and
`radar-draw-wave180-w12408.md` §3.3/§3.4. This note is self-contained: every
number was measured in this worktree on the 6,322 base.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,322 | **6,321** (-1) |
| instruction-index alignment | 1,782 | 1,782 |
| instruction count | 3,543 ours / 3,532 orig | 3,543 / 3,532 |
| whole-`.so` `.text` differing bytes | 75,015 | **75,014** (-1) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 646 (size/base exact at `0x1a3b78`, 156,312 B) | **646** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** (diff 0) |
| `.rel.dyn` / `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` / `.got` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| file size | 2,187,328 | 2,187,328 |
| total differing bytes | 75,681 | **75,680** |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (one operand-order revert, net size 0)

In the cold enemy-loop tail (line ~869), the second `lockedEnemy` dead test is
reverted from the wave-182 w12609 order back to the original `isDying`-first
order:

```diff
@@ enemy-loop tail, lockedEnemy cleanup (line ~869)
-            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDead() || this->lockedEnemy->isDying())) {
+            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDying() || this->lockedEnemy->isDead())) {
```

The two predicates are side-effect-free and the OR is commutative, so this is
semantics-preserving. The first `lockedEnemy` cleanup (line ~675, before the
enemy loop) already uses `isDying`-first and is unchanged.

## 2. Why this moves a byte

The original emits the cold enemy tail at `12fe6c`:

```
12fe6c: ldr  r0, [r4, #8]       ; lockedEnemy
12fe6e: blx  KIPlayer::isDying
12fe72: cbnz r0, 12fe8a
12fe74: ldr  r0, [sp, #128]
12fe76: ldr  r0, [r0]
12fe78: cbz  r0, 12fe90
12fe7a: blx  KIPlayer::isDying
...
12fe84: blx  KIPlayer::isDead
```

i.e. the cleanup tests `isDying` before `isDead`. Wave-182 w12609 had changed
the source to `isDead`-first for a -1 raw win on the 6,339 base; after the
wave-182 w12610 (`radarFont()` hoists) and w12611 (`hasTractor13` inversion)
harvest the surrounding cold block moved, and `isDying`-first is again the
better order: reverting it is -1 raw with the same 9,908 B size. Only the two
`blx` encodings at `12fe6e`/`12fe78` change (the `isDying`/`isDead` PLT call
targets swap); the relocation set is unchanged, so `.rel.plt` stays
byte-identical.

## 3. Sweeps performed on the 6,322 base

Every measurement is a full `link_so` + `tools/fn_diff.py` (`-j2`,
`ulimit -v 12000000`).

### 3.1 Planet-lock block (54 variants)

Cross product of the 6 statement orders of `store`/`dock`/`sound` × dock-guard
spellings (plain / `expect(0)` / `expect(1)`) × sound-guard spellings (plain /
`expect(0)` / `expect(1)`). The landed base (`store, dock, sound`; plain dock;
`expect(!lockConfirmed, 0)` sound) is the unique best at **6,322 / 9,908**.
Next: `sound, store, dock` + `expect(dock,1)` + `expect(sound,0)` = 6,324;
`sound, dock, sound`... all other size-exact shapes are 6,335+.

### 3.2 Planet guard / rare `missionBlocksPlanets` arm

`campaign > 1`, `!(campaign < 2)`, `!(planetTargets == nullptr)`, reordering
`alienOrbit`/`planetTargets`, `isDockingToPlanet` first, and the
`if (!missionBlocksPlanets) {...} else {hudEvent; goto enemies;}` inversion are
all exactly **6,322 / 9,908** (byte-identical). Moving `alienOrbit` before
`planetTargets` grows to 9,916; reordering the dock test grows to 9,912.

### 3.3 `field_0x1a8` dock guard / +8 B site

Already at the wave-182 local optimum: the dock body is inline and matches the
original byte-for-byte; the `expect(!lockConfirmed, 0)` sound guard is the
+8 B compensation. Any plain/`expect(1)` sound spelling is size-fatal (9,904)
or raw-worse.

### 3.4 `shipGroup` / wreck branches

`bossSeen = true` shrinks to 9,900 (raw 8,549); `bossSeen = ... ? true :
bossSeen` grows to 9,996; `if (shipGroup == 10) bossSeen = true` grows to
9,988. The base `bossSeen = enemy->shipGroup == 10` stays (it emits
`subs/clz/lsrs`; every source form that forces the original's `it eq; moveq`
changes size). Wreck `isDead`/`isDying` swaps, `hasCargo && ...` reorders,
`stealFlagByte` De Morgan / operand reorders and the enemy active-guard
reorders are all neutral (6,322) or worse.

### 3.5 Enemy tail

`navWreck` OR reorders (`dockNavPtr->field_0x24 || navWreck`) are worse
(6,330-7,071); `lockedEnemy != dockNavPtr` -> `dockNavPtr != lockedEnemy` is
6,323; the ternary/if rewrites of `elapsed`/`totalInt` and the
`*(navWreck ? ...)` null test are neutral. Only the §1 cleanup swap wins.

### 3.6 Broader branch/landing-pad re-sweep on the landed 6,321 base

Because the base moved by the §1 revert, the wave-182 expect sweep was redone
on the new base (all 31 `__builtin_expect` sites removed and weight-flipped,
62 variants): no edit beats 6,321. `rm8` (the freight-mission status-value
guard) is the only interesting one: aligned drops 1,782 -> **1,748** but raw
rises to 6,327, so it is not a raw win. `rm14`/`fl14`/`rm4` are byte-identical
(6,321).

Also re-measured on the 6,321 base, all size-exact and none below 6,321:

- declaration/type flips: `missionBlocksPlanets` -> `bool` (9,916, 6,449) /
  `uint8_t` (6,321, identical); `alwaysEnemySeen`/`alwaysEnemy`/`bossSeen`/
  `enemyHit`/`landmarkHit` -> `int`, `turretMode` -> `int`,
  `alienOrbit` -> `bool` all grow or worsen; `campaign` declaration moved
  before/after `mission`/`turretMode`/`alienOrbit` (best = original position).
- the three `otherLock` recomputations and the `otherLockFlag` if/else:
  every operand reordering is 6,325+ (best `olf_inv`/`olf_cond` identical).
- gas-cloud block (`sparks`/equipment guard, active/dying guard, the six-term
  24000 range test, `isAutoPilot || otherLock || enemyHit`, `field_0x1b0 &&
  !field_0x1a8`, `candidate == nullptr || candidate == lockedGas`): best
  6,321 (identical) or worse.
- asteroid block (`isAutoPilot || otherLock || enemyHit || asteroidHit ||
  turretMode`, `isMinable && !field_0x1a8`, wreck/tractor guards, the
  `field_0x10 != nullptr && ...` pair, `frame < ...`, docking-procedure
  guard): best 6,321 (identical) or worse.
- landmark/station-lock block (`i == 2 || ...` split/local forms, the
  on-screen 5-term test, `landmarkHit && ...`, `stationLockTimer > 0`,
  `field_0x28 != 0 && ...`): best 6,321 (identical) or worse.
- music tail (`isAttackedByAliens || campaign == 16 || inAlienOrbit`,
  `(current - 136) > 15 || ...`, the `case` label order, `campaign > 105`
  -> `>= 106`): best 6,321 (identical) or worse.
- planet-lock block re-check on the new base: the 54-variant cross product
  still picks the landed shape; `!(timer <= x)`, `timer >= x + 1`,
  `(missionBlocksPlanets != 0) | (timer < 1)`, and hoisting the
  `missionBlocksPlanets` test are all identical (6,321) or worse.

## 4. Remaining residual

Unchanged from wave 182: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses `sl` for the
stations array, with the planet-loop index in `fp`; ours spills `campaign` to
`sp+0x90`, keeps `stations` at `sp+0x84` and uses `sl` for the
`missionBlocksPlanets` zero constant. The stack-slot rotation and the
`bossSeen` `subs/clz/lsrs` vs `it eq; moveq` materialisation follow from that
cascade.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6321 aligned= 1782 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check   # RATCHET PASS
cmake --build cmake-build-match --target drift   # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12709/` — `Radar.base.cpp`
(6,322 base), `sweep_planet.py` (planet-lock cross product), `sweep2.py` +
`vars_branch.py` / `vars_tail.py` (named-cell sweeps), `le2.cpp` (landed
source), `struct2.py` / `align2.py` (alignment aids).
