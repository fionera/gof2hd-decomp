# Radar::draw wave 103 — flattening the enemy/station lock tails is a real
# fuzzy-metric win (wf306)

**Status:** landed source change + analysis note, 2026-11-04. Worktree `wt/wf306`,
branch `wave103-wf306`, base `1d2b4398e`.

**Change (semantic no-op, codegen shape only):**

```diff
@@ station lock tail @@
-            } else if (this->lockedStation == nullptr) {
-                this->stationLockTimer = 0;
-                this->field_0x28 = 0;
             } else {
+                if (this->lockedStation == nullptr) {
+                    this->field_0x28 = 0;
+                }
                 this->stationLockTimer = 0;
             }

@@ enemy lock tail @@
             } else {
                 if (*(navWreck ? &this->field_0x1c : &this->lockedEnemy) == nullptr) {
-                    this->enemyLockTimer = 0;
-                    enemyHit = false;
                     this->dockNavPtr = nullptr;
-                } else {
-                    this->enemyLockTimer = 0;
-                    enemyHit = false;
                 }
+                this->enemyLockTimer = 0;
+                enemyHit = false;
             }
```

**Result:** `_ZN5Radar4drawEP6PlayerP3Hudi` official-verifier raw ratio
**73.896848 % -> 73.972210 %** (`usize 9908` unchanged, size-exact). The official
report percentage rounds to **74.0 %** (was 73.9 %). Function SHA-1
`9727e84d6452...` -> `b8e72ed2d1d1...`. Gate unchanged:
`byte 4500 / linked 4500 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0 /
RATCHET PASS`. The lint soft-criterion `operator_call` count also shrank 19 -> 17
(the duplicate member assignments were removed), so `tools/goal_lint_baseline.json`
is re-baselined in the same commit.

## 0. Baseline and commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Baseline:

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  ratio 73.896848 norm 3492/3488 sha 9727e84d6452
```

Final:

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  ratio 73.972210 norm 3492/3489 sha b8e72ed2d1d1
```

## 1. New oracle: what it confirms and what it does not

The wave-103 oracle (`_work/oracles/.../native/game/Radar.c`, KiritoJPK
NDK-r14/clang-3.8 -O2 rebuild) was read end-to-end for `Radar::draw`. It is a
different revision/compiler, but its control-flow shape matches the original v2
binary on every branch checked:

* landmark loop indices (`i != 2` skip, `i != 3` image) and the lock-hit box
  (`size = field_0x124`, `left = crosshair.x - size`, `left + 2*size`) match;
* the planet lock-hit box uses `size >> 1` in the original binary too (`asrs r3,r1,#1`
  at original `+0x4be`), so the oracle's missing `>>1` is a decompiler artifact;
* the gas-cloud spark in-sight box, the asteroid slot-distance loop and the music
  tail are instruction-for-instruction identical between our build and the original
  (only link-relative offsets differ);
* the literal-pool sequence is identical (45 entries, same order), so the source's
  global-reference order is right.

The oracle's one genuinely different statement is the asteroid dead predicate:
the original evaluates `(hasCargo && dead) || !dead` (6 calls) while our source
evaluates `dead` once and reuses it (4 calls). A faithful translation
(`if (!((hasCargo && (isDead()||isDying())) || (!isDead()&&!isDying()))) continue;`)
reproduces the original's 6-call asteroid loop **instruction-for-instruction**, but
the body grows 9908 -> 9928 B and the fuzzy ratio drops to 71.30 % because the
global shift moves everything after it. That is the faithful/tuned trade-off.

## 2. Measured variant table (official verifier API, `autojunk=False`)

| variant | usize | ratio % |
|---|---:|---:|
| baseline (landed before this pass) | 9908 | 73.896848 |
| **enemy lock tail flattened (landed)** | **9908** | **73.972210** |
| + station lock tail flattened (landed) | 9908 | 73.972210 |
| station flatten alone | 9908 | 73.896848 |
| planet lock tail flattened | 9912 | 72.071040 |
| asteroid lock tail flattened | 9924 | 69.101848 |
| enemy tail with `else` restored | 9888 | 68.359151 |
| enemy tail `enemyHit`/timer swapped | 9908 | 73.972210 |
| enemy tail `enemyHit=false` dropped | 9908 | 73.972210 |
| faithful asteroid 6-call predicate | 9928 | 71.303850 |
| faithful asteroid, `bool dead` local | 9908 | 73.743017 |
| first `otherLock` positive OR | 9900 | 70.838117 |
| first `otherLock` positive OR + bool | 9876 | 67.796123 |
| enemy in-sight distance-first (oracle shape) | 9924 | 71.246601 |
| duplicate campaign autopilot block removed | 9752 | 62.312139 |
| landmark condition permutations (24) | 9908 | 73.896848 best |

The enemy/station tail flattening is the only source shape found this pass that
raises the ratio without changing size. It is a natural C++ spelling (the timer
reset happens once after the conditional `dockNavPtr`/`field_0x28` clear), not a
tuned hack.

## 3. Remaining residuals (unchanged by this pass)

These are all codegen/allocation, not statement differences:

1. first `otherLock` chain: original `cbnz/cbnz/cmp/beq` (positive OR), ours
   `cbz + itt eq/ldreq/cmpeq/beq` (negated form with two `__builtin_expect`
   anchors). Every positive-OR spelling tried loses 8 B and ~3 points.
2. duplicate `if (!alienOrbit)` autopilot block in our source (original has one
   `getAutoPilotTarget` call); it is load-bearing for the 9908 B size.
3. asteroid dead predicate: 4 calls in ours vs 6 in the original (see §1).
4. landmark `DrawString` tail-merge (orig 9 sites, ours 8).
5. enemy tractor `isDead()` CSE (orig 17 `isDead`/13 `isDying`, ours 16/12).
6. `vpush {d8-d11}` vs `{d8-d13}` (crosshair floats) and the
   `mode`/`&lockLabel`/`missionBlocksPlanets` stack-slot rotation.

## 4. Evidence / scratch

All scratch under `/dev/shm/wf306` (not committed): `Radar.base.cpp`,
`Radar.enemyelse.cpp`, `measure.py` (official-verifier ratio for one symbol),
`calls2.py` (call multiset), `opcodes.py` / `subdiff.py` (normalized-stream
opcode diff), `pools.py` (literal-pool sequence), `sweep*.py` (the variant
sweeps above).
