# Radar::draw wave 146 (w9206) — enemy-loop tail merge landed at 9908 with the witnessed `bool alwaysEnemy`

**Status:** source change landed, 2026-11-08. Worktree `wt/w9206`, based on `master`
(`31297fadc`, wave 145). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8,373 B** (w9009
state). After this pass **8,733 B** (+360). `.text` differing bytes
**85,651 -> 86,011** (+360); `.ARM.extab` **1,370** (unchanged, size exact
156,312); `.rel.plt` **0** (byte-identical); `.rodata`/`.data`/`.bss`/
`.ARM.exidx` byte-identical. All 27 section sizes exact; file size exact.
Gate: `RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

Read with `radar-draw-wave145-w9010.md`, `radar-draw-wave145-w9009.md`,
`radar-draw-wave144-w8907.md`.

## 1. Mission hypothesis and answer

The mission asked for w9010's faithful enemy-loop single-`DrawImage2D` tail
merge, dead-local drop and `hasCargo`-first reorder on top of the w9009 base
(8,373 raw), plus a **genuine +4 B companion** instead of a type pad.

**The +4 B companion is `bool alwaysEnemy`, and it is witnessed — not a pad.**
w9010 labelled it METRIC-ONLY, but the original widens it to the int
`alwaysEnemySeen` slot with an explicit bit-0 mask at the loop exit:

```
12f3b6: ldr   r1, [sp, #80]      ; alwaysEnemy
12f3b8: cmp   r0, #0
12f3ba: and.w r1, r1, #1         ; bool -> int widening
12f3be: str   r1, [sp, #104]     ; alwaysEnemySeen = alwaysEnemy
```

`and #1` is only emitted for a `bool` source; an `int` source that is already
0/1 would need no mask. So `bool alwaysEnemy` is the faithful type, and it is
exactly the +4 B the mission was missing. The same widening at `12f3c2`
(`ldr r1,[sp,#88]; and r1,#1; str r1,[sp,#132]`) proves `enemyHit` is also a
`bool` in the original — see section 3.

## 2. Landed source

Exactly the w9010 diff rebased onto w9009 (asteroid dead-pair duplication kept
in w9009's inline form):

```cpp
// enemy loop: one DrawImage2D after the wreck/non-wreck if/else
unsigned int image;
if (wreck) { ... image = ...; } else { ... image = ...; }
Globals::Canvas->DrawImage2D(image, this->screenX, this->screenY, 17, 68);
goto tractor;

int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
int enemyHit = false;          // METRIC-ONLY (bool is the witnessed type)
bool alwaysEnemy = false;      // witnessed bool (+4 B companion)
bool asteroidHit = false;      // witnessed bool
```

`bool asteroidHit` is witnessed by the asteroid loop's bit-0 test
`12fb38 lsls r0,r0,#31` over the
`isAutoPilot || otherLock || enemyHit || asteroidHit || turretMode` chain.

`int wreck` is witnessed by the word store `12ed82 str r0,[sp,#152]`.

## 3. Variant matrix (raw same-address diff, size)

All on the w9009 base (8,373 / 9908). `Ew` = `int enemy wreck`, `EH` =
`int enemyHit`, `AE` = `bool alwaysEnemy`, `AH` = `bool asteroidHit`,
`AS` = `bool alwaysEnemySeen`, `MB` = `int missionBlocksPlanets`,
`dead` = w9010's named `bool dead` asteroid form.

| variant | usize | raw |
|---|---:|---:|
| baseline (w9009) | 9908 | 8373 |
| merge only | 9880 | 8795 |
| `Ew` | 9888 | 8791 |
| `Ew`+`EH` | 9892 | 8776 |
| `Ew`+`EH`+`AH` | 9904 | 8740 |
| **`Ew`+`EH`+`AE`+`AH` (landed)** | **9908** | **8733** |
| `Ew`+`EH`+`AE`+`AH`+`dead` (w9010 exact) | 9908 | 8731 |
| `Ew`+`EH`+`AH`+`AS`+`MB` | 9908 | 8318 |
| `Ew`+`EH`+`AH`+`AS` | 9912 | 8311 |
| `Ew`+`EH`+`AH`+`MK` | 9912 | 8326 |
| `Ew`+`EH`+`AH`+`AS`+`LH` | 9912 | 8257 |
| `Ew`+`AE`+`AH` (bool enemyHit, fully faithful) | 9884 | 8768 |

`Ew`+`EH`+`AH`+`AS`+`MB` is the lowest raw at 9908 (8,318, below the 8,373
baseline) but `AS` (`bool alwaysEnemySeen`) contradicts the original's word
store `12f3be str r1,[sp,#104]`; it is recorded as a metric bound only.

## 4. Remaining deltas / open questions

1. The fully faithful type set (`bool enemyHit`, `bool alwaysEnemy`,
   `bool asteroidHit`, `int wreck`) lands at **9884**, 24 B short. The missing
   24 B is the landmark-loop `DrawString` un-merge (original 9 calls, ours 8):
   ours tail-merges the `i==0` and `else` lockLabel draws at `12eaf4`.
   `String labelCopy` un-merges but costs +20 B (9924) and grows `.ARM.extab`.
   A `const String&` alias and `int labelX/labelY` locals are both optimised
   away (still 9904). This is the next lever.
2. The extra `FModSound::play` (ours 8, original 7) is the 146/147 branch.
3. The outlined tails still use `str r6,[sp,#0]; movs r6,#68; str r6,[sp,#4]`
   where the original uses `strd r5,sl,[sp]` (2-4 B per site).

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **86,011** differing bytes (size/base
exact 1,259,464), `.ARM.extab` **1,370** (size exact 156,312), `.rel.plt`
**0** (byte-identical, 29,392 B), `.note.gnu.build-id` 19. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are
byte-identical. All 27 section sizes exact.
