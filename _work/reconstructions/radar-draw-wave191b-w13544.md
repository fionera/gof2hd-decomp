# Radar::draw wave 191b (w13544) — enemy-guard ABCDE: raw 6,015 -> 6,013

**Status:** landed, 2026-11-12. Worktree `wt/w13544`, branch `wave191b-w13544`,
base `master` `e32ec1a09`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13535.md` (base 6,015, `lockedEnemy` isDying/isDead
order), `radar-draw-wave191b-w13528.md` (gas/planet/type/statement sweeps) and
`radar-draw-wave191-w13489.md` (the ADEBC guard reorder this pass reverts).

## 0. The change

The per-enemy guard (`Radar.cpp:835`) was in w13489's reordered form `A D E B C`
(`isAutoPilot`, `isDockedToDockingPoint`, exclude-flag, enemyHit/field_0x24,
wreck/isDead).  The pre-w13489 source order is `A B C D E`:

```cpp
-            if (!this->level->getPlayer()->isAutoPilot() &&
-                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) && // lint: arm_builtin anchor: Radar::draw
-                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck) &&
-                (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&  // lint: arm_builtin
-                (!wreck || enemy->isDead())) {
+            if (!this->level->getPlayer()->isAutoPilot() &&
+                (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&  // lint: arm_builtin
+                (!wreck || enemy->isDead()) &&
+                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) && // lint: arm_builtin anchor: Radar::draw
+                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck)) {
```

All five operands are pure getters/field reads, so the short-circuit order is
semantically irrelevant.  The `__builtin_expect` hints stay attached to their own
operands (`B` is the `,0` hint, `D` the `,1` anchor).

This is the same source change w13541 landed on its own base (`2dcf1de0d`); it was
re-verified independently here on `e32ec1a09`.  An exhaustive 120-permutation sweep of
the five terms found `ABCDE` the unique raw winner (all other size-exact orders
6,015 or worse).

## 1. Measured result

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,015 | **6,013 (-2)** |
| instruction-index alignment | 1,941 | 1,965 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,276 | **71,274 (-2)** |
| `.ARM.extab` differing bytes | 576 | 576 (no growth) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` size delta | +485 | +485 |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

The aligned metric is 24 worse; the landing rule is a net-positive raw
(same-address) win with size exact and `.rel.plt` identical.

## 2. Negative cells measured this pass (all reverted; base 6,015)

| cell | raw |
|---|---:|
| base | 6,015 |
| `campaign` type `unsigned`/`long`/`const`/`register`/`auto` | 6,015 (neutral) |
| `campaign` type `short`/`char`/`uint8_t` | size-fatal (9,912) |
| `campaign` type `bool` | size-fatal (8,572) |
| `campaign` definition moved down to first use | size-fatal (9,904, raw 9,239) |
| `mission` block moved after `campaign` decl | 6,015 (neutral) |
| `mission` block moved elsewhere | size-fatal |
| `alwaysEnemySeen`/`alwaysEnemy` retyped int/char/short | size-fatal |
| `alwaysEnemySeen`/`alwaysEnemy` decl moved down to `bossSeen` | 6,015 (neutral) |
| `notTurretMode`/`notOtherLock`/`otherLock` as bool / inlined | size-fatal (9,860-9,876) |
| `mode` local copy (`frameTime`) | 6,015 (neutral) |
| tractor condition reorder `tractorMode2 && 1c==nullptr && wreck` | 6,017 |
| ... same reorder + guard ABCDE | 6,015 |
| `enemy->field_0x76/0x73` store swap | 6,015 (aligned 1,961) |
| ... same swap + guard ABCDE | 6,013 (neutral raw, aligned 1,985) |
| `dockNavPtr`/`field_0x1c` store swap in tractor block | 6,015 (neutral) |
| enemy-blip `setFrame` before `setRefPixelPosition` | 6,019 (aligned 1,921) |
| gas 24000 chain x-y-z ego-target | 6,020 |
| enemy 24000 upper-first (`EN`) | 6,015 (aligned 1,954) |
| asteroid `>500` block local `cur`/if-else | 6,015 (neutral) |
| random same-depth statement swap 810<->862 (`} else {` <-> `tractor:`) | 6,013 (aligned 1,962), not composable with guard win |

## 3. Root cause (unchanged)

`campaign` spills `sp+0x90` in ours vs the original keeping it in `sl`; `mode` spill
slot `sp+0x68` ours vs `sp+0x64` orig (swap with landmark `offset`); shared zero/loop
index in `sl` ours vs `fp` orig. Greedy RA spills `campaign` immediately
(`t2STRi12 $r0, %stack.45`) while the original keeps it in `sl`. No source spelling
tried across waves 162-191b changes this without a size change. Orig's gas-loop
literal pool at `0x12f200` is `{922402, 922254, 922092, 921814, 24000.0, -24000.0}`;
ours at `0x12f180` differs at entry #2 (922242) — a branch-target artifact of the gas
24000-chain layout.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13544/`: `try.py`/`try2.py`/`try3.py`
(1 s compile+link+fn_diff cell harness), `sbs.py`, `seqcmp.py`, `addrline.py`,
`buckets.py`, `randswap.py`, `aldiff.out`, `mi.txt` (MIR dump).

## 5. Addendum — root-cause probes (all reverted; base 6,013)

### 5.1 The `zero`↔`campaign` register swap is reachable but size-fatal

The original keeps the shared zero constant in `fp` (`mov.w fp,#0` at `0x12e07a`,
re-materialised at `0x12e0d8`/`0x12e3c8`) and `campaign` in `sl` (`mov sl,r0` at
`0x12e0f8`, `cmp.w sl,#2` at `0x12e38a`).  Ours gives the zero `sl` and spills
`campaign` to `[sp,#144]`, reloading it at the `>= 2` test — the +2-instruction
seed of the whole `0x12e390`-`0x12f710` byte shift.

Retyping **`alwaysEnemySeen`** to `int` (or `unsigned char` / `signed char`) does
flip the zero to `fp` and moves `campaign` into `r5` (no spill), i.e. the RA
tie-break is reachable from source.  It is size-fatal though:

| cell | raw | size | insn |
|---|---:|---:|---:|
| base | **6,013** | 9,908 | 3,541 |
| `int alwaysEnemySeen` | 9,191 | 9,952 (+44) | 3,537 |
| `unsigned char alwaysEnemySeen` | 9,191 | 9,952 (+44) | 3,537 |
| `int alwaysEnemy` (only) | 8,599 | 9,920 (+12) | 3,546 |
| `bool`→`int` both | 9,125 | 9,944 (+36) | 3,533 |

`alwaysEnemySeen` is a `bool` in the original (`eors.w r2,r2,#1` / `cbnz` at
`0x13029e`/`0x130356` prove 0/1), so the flip is a pure RA artefact of the
`bool`-vs-`int` vreg class, not a type we can adopt.

### 5.2 Asteroid lock timer: `= 0` in the candidate block, `+= mode` in the else

The original's `if (best > -1)` candidate-change arm is `asteroidLockTimer = 0`
(`movne r1,#0; strne.w r1,[r4,#412]` at `0x12fd8a`), and the `+= mode`
(`ldr; ldr; add; str` at `0x12fe38`) sits in the `else` arm immediately before
the `> field_0x1b8 - 200` test — not in the candidate block as our source has it.
Moving it is size-exact but raw-worse (6,190).  Dropping the
`this->lockedAsteroid = nullptr;` from the else arm is size-fatal (9,904): the
original does not emit a `str 0,[this+0xc]` in `draw` at all, so the store must
survive for the size.

### 5.3 Music-tail predicate order

The original evaluates `(uint32_t)(current-136) > 15`, then
`((1 << (current-136)) & 0xe071) == 0`, then `(current != 151 && alwaysEnemySeen)`
(`0x130280`-`0x1302a2`).  Swapping our last two operands is size-exact but the
compiler re-orders the `__builtin_expect(...,0)` arm back in front; raw 6,036.
Dropping the hint is size-fatal (9,900).

### 5.4 Adjacent statement swaps

Every same-indent adjacent (distance <= 3) statement swap in `Radar.cpp:950-1360`
(72 cells) is raw-neutral at 6,013 / size 9,908; none is a win.
