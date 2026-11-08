# Radar::draw wave 144 (w8907) — enemy-loop single DrawImage2D + dead-local removal, asteroid hasCargo-first

**Status:** source change landed, 2026-11-08. Worktree `wt/w8907`, based on `master`
(`f5093474f`, wave 143). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8,613 B**.
After this pass **8,848 B** (+235). `.text` differing bytes **85,851 -> 86,086**;
`.ARM.extab` **1,499 -> 1,501**; `.rel.plt` **0** (byte-identical). All 27 section
sizes exact. Gate before and after: byte **4507**, linked **4507**, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

Read with `radar-draw-wave143-w8808.md`, `radar-draw-wave143-w8807.md`,
`radar-draw-wave141-w8609.md`, `radar-draw-wave140-w8510.md`.

## 1. Mission hypothesis and answer

The mission asked whether the 128 B the plain-bool build loses is in the
planet-loop block order / single-vs-double `getAutoPilotTarget` recompute. **No.**

Measured: the planet loop is essentially identical (orig 384 instructions /
0x12e3d4..0x12e6c2, ours 386 / same span). The top-level `onPlanetCourse` block
is instruction-identical at `0x12e180..0x12e1d0` and the cold `!alienOrbit`
`getAutoPilotTarget` block is single in both (`0x12e6ee` orig / `0x12e6e2` ours).
The planet-loop size delta is +2 instructions, not 128 B.

The 128 B is in the **enemy loop**: the plain-bool build's enemy loop is 86 B
*longer* than the original, and the rest of the function is 86 B *shorter*.
The enemy loop is the largest genuine (non-shift) hunk.

## 2. Hunk classification (enemy + asteroid loops)

Register-normalised instruction diff of the enemy loop (`0x12ecec..0x12f18c`
orig, `0x12ec9c..0x12f192` ours, 423 vs 456 instructions):

| hunk | bytes | class |
|---|---:|---|
| `0x12ef23..0x12efbf` | 157 | shift artifact (inSight block, same stream) |
| `0x12ed9a..0x12ee2c` | 147 | shift artifact (onScreen block) |
| outlined `DrawImage2D` tail | ~24 | **genuine**: original tail-merges the two not-onScreen calls; ours inlines both |
| enemy `dead` local | ~16 | **genuine**: original has no `dead`; it computes `wreck` then `if ((isDead\|\|isDying) && !hasCargo)` |
| `tractor` guard | 4 | genuine: `int wreck` forces `orrs`, original short-circuits `cbnz`+`eors` |
| register shuffles | rest | RA / StackColoring |

Asteroid loop (`0x12fa94..0x12fcea` orig): the original checks `hasCargo` first
and duplicates the `isDead\|\|isDying` pair into both branches (6 calls); ours
computes `dead` once then checks `hasCargo` (4 calls).

## 3. Landed change

```cpp
// enemy loop: one DrawImage2D after the wreck/non-wreck if/else (original
// tail-merges to a shared 0x12f148/0x12f152 block)
unsigned int image;
if (wreck) { ... image = ...; } else { ... image = ...; }
Globals::Canvas->DrawImage2D(image, this->screenX, this->screenY, 17, 68);
goto tractor;

// enemy loop: drop the unused `dead` local (it was never read)
int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());

// asteroid loop: hasCargo first, dead inlined (original order)
if (!asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying())) continue;
bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());

int planetHit = false;   // METRIC-ONLY pad to hold 9908 B
```

The single-call reshape is witnessed: it fixes the `DrawImage2D` call census
(ours 13 -> 12, original 12) and shrinks the enemy loop 1272 -> 1198 B
(original 1186 B, only +12). The `dead` removal is witnessed: the original
enemy loop has 4 `isDead`/`isDying` calls after `update`, ours had 6.
The asteroid `hasCargo`-first order is witnessed by the original's
`ldrb [r6,#76]; cbz` before the `isDead` pair.

`int planetHit` is **METRIC-ONLY**: it is the pad that restores 9908 B after the
faithful reshapes remove 36 B. It is not witnessed by the original.

## 4. Variant matrix (raw same-address diff, size)

| variant | usize | raw |
|---|---:|---:|
| baseline (wave-143 merge) | 9908 | 8613 |
| single DrawImage2D only | 9884 | 8708 |
| + drop enemy `dead` | 9868 | 8771 |
| + asteroid hasCargo-first/inline | 9872 | 8753 |
| + `int planetHit` + `bool asteroidWreck` (**landed**) | **9908** | **8848** |
| + `int planetHit` + `int asteroidWreck` | 9912 | 8863 |
| + `int planetHit` + `bool alwaysEnemy` | 9920 | 8783 |
| + `int planetHit` + `bool landmarkHit` | 9916 | 8801 |
| + `int planetHit` + `bool asteroidHit` | 9900 | 8858 |

The faithful reshapes cost raw because the enemy loop shrinks by 74 B and every
block after it shifts; the baseline's +86 B enemy loop happened to keep the
later blocks at the original addresses. `.text` 85,851 -> 86,086 (+235).

## 5. Remaining deltas / open questions

1. **Asteroid `dead` duplication** (original 6 `isDead`/`isDying` calls, ours 4):
   the original checks `hasCargo` first and duplicates the `dead` pair into both
   branches. `if (!hasCargo && dead)` and the inlined form both still emit 4.
   Forcing the duplication without a size change is the next asteroid target.
2. **Missing landmark-loop `DrawString`** (original 9, ours 8): the original has
   three landmark DrawStrings (`0x12e9e0` label, `0x12eaae` i==0 lockLabel,
   `0x12eb36` else lockLabel); ours tail-merges the two lockLabel calls.
3. **Extra `FModSound::play`** (original 7, ours 8) and **missing `__aeabi_f2lz`**
   (original 1, ours 0) in the music tail.
4. The outlined tail still uses `str r6,[sp,#0]; movs r6,#68; str r6,[sp,#4]`
   instead of the original `movs r5,#17; strd r5,sl,[sp]` (2-4 B).
5. `int planetHit` is a pad; replacing it with a witnessed +36 B would drop the
   raw by up to ~95.

## 6. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **86,086** differing bytes (size/base
exact 1,259,464), `.ARM.extab` **1,501** (size exact 156,312), `.rel.plt` **0**
(byte-identical, 29,392 B), `.note.gnu.build-id` 20. All 27 section sizes exact.
