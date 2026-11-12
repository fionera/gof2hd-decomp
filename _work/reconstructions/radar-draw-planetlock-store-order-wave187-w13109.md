# Radar::draw wave 187 (w13109) — planet-lock store order + `||` timer guard: raw 6,311 -> 6,309

**Status:** landed, 2026-11-12. Worktree `wt/w13109`, branch `wave187-w13109`,
base `master` `8b5d4281c` (wave-186 harvest 6). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave186-w13009.md` (the 432-variant planet-lock sweep),
`radar-draw-wave186-w13010.md` (campaign `sl` vs `sp+0x90` regalloc) and
`radar-draw-wave186-w13011.md` (expect/alias re-sweep). This note is
self-contained: every number was measured in this worktree on the 6,311 base.

## 1. The change

```diff
@@ planet-lock body (line ~637)
-                    this->field_0x14 = this->field_0x18;
                     if (this->field_0x1a8) {
                         this->level->getPlayer()->dockToPlanet();
                     }
                     if (__builtin_expect(!this->lockConfirmed, 0)) {  // lint: arm_builtin
                         Globals::sound->play(26, nullptr, nullptr, 0.0f);
                     }
+                    this->field_0x14 = this->field_0x18;
                 }
-                if ((this->planetLockTimer < 1) | (missionBlocksPlanets != 0)) {
+                if ((this->planetLockTimer < 1) || (missionBlocksPlanets != 0)) {
                     goto enemies;
                 }
```

Two independent, semantics-preserving edits:

1. The `field_0x14 = field_0x18` member store moves after the two independent
   guards (`dockToPlanet()` and `FModSound::play` neither read nor write
   `field_0x14`). This is the `DPS` statement order (dock, sound, store).
2. The timer guard's bitwise `|` becomes short-circuit `||`. Both operands are
   side-effect-free (`planetLockTimer < 1` and the local
   `missionBlocksPlanets != 0`), so the short-circuit is identical.

Neither edit alone is a win on the 6,311 body: the store move alone is
6,424-6,426 (size exact) and `||` alone is 6,426 / 9,908. Together they are
6,309.

## 2. Measured result (this worktree)

| metric | before | after |
|---|---:|---:|
| `Radar::draw` raw same-address diff | **6,311** | **6,309** |
| instruction-index alignment | 1,903 | 1,858 |
| instruction count | 3,544 / 3,532 | 3,541 / 3,532 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes (whole `.so`) | 74,274 | **74,272** |
| `.ARM.extab` differing bytes | 647 | 647 |
| `.ARM.exidx` / `.rel.plt` / `.rodata` / `.data` / `.bss` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| total differing bytes | 74,941 | **74,939** |
| file size | 2,187,328 | 2,187,328 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 | same |

`RATCHET PASS`. `.rel.plt` stays byte-identical; `.ARM.extab` size/base
unchanged at `0x1a3b78`, 156,312 B. The change is size-exact and removes three
instructions from the body (the `||` form emits a short-circuit pair instead
of the branchless `orrs` materialisation).

## 3. Why it helps

The base `|` form emits a branchless materialisation of
`(planetLockTimer < 1)` into a scratch register and ORs it with
`missionBlocksPlanets` (`movs r1,#0; cmp; it lt; movlt r1,#1; orrs`), while
`||` emits a short-circuit `cmp r5,#0; bne enemies; cmp r0,#1; blt enemies`.
The short-circuit shape is 2 raw bytes closer to the original at the same
addresses and drops one instruction. Moving the store after the sound guard
lets clang schedule it into the same slot as the original's
`ldr field_0x18; str field_0x14` pair.

## 4. Search evidence

The winning cell was found by a 144-variant cross product of
{dock plain, dock `expect(0)`} x {sound plain, `expect(!lockConfirmed,0)`,
`expect(!lockConfirmed,1)`} x all six store/dock/sound orders x
{timer `|`, timer `||`} x {int, bool `missionBlocksPlanets`}. Best results:

| variant | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base (SDP, `|`, int) | 6,311 | 1,903 | 3,544 | 9,908 |
| **DPS, `||`, int (landed)** | **6,309** | **1,858** | **3,541** | **9,908** |
| DSP, `||`, int | 6,309 | 1,852 | 3,541 | 9,908 |
| SPD, `||`, int | 6,310 | 1,853 | 3,541 | 9,908 |
| DPS, `||`, bool | 6,319 | 1,832 | 3,542 | 9,908 |
| DPS, `|`, int | 6,424 | — | 3,544 | 9,908 |

`DSP` (dock, store, sound) is raw-equal but alignment-worse; `DPS` is landed.
On top of the landed body these are raw-neutral (6,309): `field_0x1a8 != 0`,
`this->planetLockTimer < 1 || missionBlocksPlanets`, the wreck
`isDying()/isDead()` swap, the skip-guard `isActive`-first order, and the
`lockImage` arm swap. These are `+1` (6,310): the enemy-loop `lockedEnemy`
`isDead()/isDying()` swap and the gas-cloud `isDead()/isDying()` swap.
`blip` alias inlining in the station lock is size-fatal (9,916).

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13109/`: `Radar.base.cpp`
(6,311 base), `measure.sh`, `sweep.sh`, `pl_*.cpp` / `pl_results.txt` (the
144-variant planet-lock cross product), `x_*.cpp` / `x_results.txt` and
`y_*.cpp` / `y_results.txt` (follow-up cells on the landed body),
`seccmp.py` (per-section diff census), `afterall.txt` (MIR dump).
