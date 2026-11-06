# Radar::draw pass (wave119, w6976) — distance-first reproduces the exact prologue; ~45 variants, no score win

**Status:** research note, 2026-11-06. Worktree `wt/w6976`, branch
`wave119-w6976`, based on `master` `be3034dc1`. **No source change landed**;
this commit contains this note only. The target remains the pristine baseline
(74.541284 %).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.541284 %** (official `verify.py` display 74.4), `M = 2600` matching
instructions.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. No section/`.ARM.extab` change (note-only
commit; whole-file size still 2,187,328 B).

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w6976
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS

NDK=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$NDK -d --start-address=0x12e050 --stop-address=0x130800 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6976/orig.asm
$NDK -d --start-address=0x12e050 --stop-address=0x130800 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6976/ours.asm
```

Per-variant loop (≈1.3 s each): patch `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then score the single symbol with a
scratch harness that mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` (identical 74.541284 % on the baseline). Harness:
`/dev/shm/w6976/score2.py` (prints `M=`), `batch.py`, `tryvar.py`.

## 2. NEW finding: the original's `inSight` block is distance-first, and that is the
only source shape found that reproduces the *exact* prologue

The original's enemy `inSight` block (`12ef54`) is:

```
12ef4c: ldr  r0,[pc,#688]     ; &PlayerEgo::crosshairPos
12ef4e: movs r6,#0
12ef50: strb.w r6,[r8,#115]   ; enemy->field_0x73 = 0
12ef54: add  r5,sp,#232       ; &(egoPos - targetPos)
12ef58: ldr  r0,[r0,#0]
12ef5a: vldr s26,[r0]         ; crosshair.x  -> callee-saved s26
12ef5e: vldr s24,[r0,#4]      ; crosshair.y  -> callee-saved s24
12ef62: mov  r0,r5
12ef64: ldrd r2,r1,[sp,#108]  ; &targetPos / &egoPos
12ef68: blx  AEMath::mi
12ef6c: mov  r0,r5
12ef6e: vcvt.s32.f32 s24,s24  ; (int)crosshair.y
12ef72: vcvt.s32.f32 s26,s26  ; (int)crosshair.x
12ef76: blx  AEMath::VectorLength
12ef7a: vmov s0,r0
12ef80: vcmpe.f32 s0,s20      ; 60000.0f
12ef88: bpl.n 12efc0          ; distance check FIRST
12ef8a: vmov r1,s26           ; then the x bounds
...
```

i.e. `VectorLength(egoPos - targetPos) < 60000.0f` is evaluated **before** the
four screen-bounds comparisons, and the two crosshair floats stay live across
the `mi`/`VectorLength` calls in `s24`/`s26` (hence the original's
`vpush {d8-d13}`).

Every earlier pass spelled the condition bounds-first (the baseline does), so
clang computes `(int)crosshairPos.x/y` up front and keeps them in `s0/s2`,
giving `vpush {d8-d11}`.  Rewriting the condition as

```cpp
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
    this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
    inSight = true;
}
```

(x-pair before y-pair, i.e. the original's order) gives **`vpush {d8-d13}`,
`sub sp,#296` and `egoPos`/`targetPos` at `sp+108`/`sp+112` — all exactly as
in the original** (`oursX.asm`).  The only prologue residual left is a
`mode`/`lockLabel` stack-slot swap:

| slot | original | distance-first build |
|---|---|---|
| `mode` | `sp+100` | `sp+104` |
| `lockLabel` ptr | `sp+104` | `sp+100` |
| `egoPos` ptr | `sp+108` | `sp+108` |
| `targetPos` ptr | `sp+112` | `sp+112` |

Despite the exact prologue this variant scores only **9896 B / 68.053364 %**
(`M = 2372`): the body re-rolls (the `mode`/`lockLabel` slot swap plus a
different register allocation in the enemy loop).  So the distance-first shape
is *faithful* but not *tuned*; it is the natural starting point for a future
pass that wants to attack the body with the correct prologue in place.

## 3. Variant table (all measured this pass; baseline = 9908 B / 74.541284 %)

`M` is the matching-instruction count from `difflib`.

| variant | size | raw % | M |
|---|---:|---:|---:|
| **baseline (pristine)** | 9908 | **74.541284** | **2600** |
| `inSight` distance-first (df) | 9960 | 65.339444 | 2281 |
| df, distance cached in a `float dist` local | 9928 | 63.417431 | 2212 |
| df, nested `if (dist < 60000)` | 9928 | 63.417431 | 2212 |
| df + `float crossX/crossY` locals | 9960 | 65.282154 | 2279 |
| **df, x-pair before y-pair** | **9896** | **68.053364** | 2372 |
| df, no `cx`/`cy` locals (inline `crosshairPos`) | 9960 | 64.259100 | 2242 |
| df as `inSight = cond && cond && ...` | 9924 | 64.453349 | 2252 |
| guard `!= 109 && != 110 && (!=111 \|\| <=93)` | 9912 | 64.966328 | 2267 |
| guard split into two `!(...)` groups | 9908 | 74.541284 | 2600 |
| guard `bool skipStation` local | 9916 | 70.372493 | 2456 |
| guard wrapped in `__builtin_expect(...,1)` | 9908 | 68.711481 | 2397 |
| guard fully negated `!(... \|\| ...)` | 9896 | 72.442978 | 2525 |
| `notTurretMode` declared before `notOtherLock` | 9908 | 74.541284 | 2600 |
| `bool notOtherLock/notTurretMode` | 9920 | 71.379409 | 2489 |
| `uint8_t notOtherLock/notTurretMode` | 9908 | 74.541284 | 2600 |
| otherLock test without `__builtin_expect` | 9908 | 73.308486 | 2557 |
| `bool otherLock` | 9860 | 68.284279 | 2378 |
| `i == 1 \|\| i == 3` (no `& 0x7fffffff`) | 9908 | 74.541284 | 2600 |
| same, no `__builtin_expect` | 9908 | 74.541284 | 2600 |
| `__builtin_expect(..., 1)` on the i==1/3 arm | 9908 | 74.541284 | 2600 |
| i==1/3 arm as `if/else-if` chain | 9932 | 72.232565 | 2522 |
| landmark `i==0` arm `String label` -> `this->lockLabel` | 9904 | 71.614845 | 2499 |
| same, but `String label; label = ...` | 9940 | 66.628506 | 2328 |
| both landmark arms -> `this->lockLabel` | 9904 | 71.614845 | 2499 |
| both arms -> lockLabel + df | 9896 | 67.488532 | 2354 |
| both arms -> lockLabel + recompute deleted | 9816 | 63.804035 | 2214 |
| invented planet-course recompute deleted | 9832 | 63.451850 | 2204 |
| `navWreck` with `__builtin_expect(ptr != 0, 0)` | 9916 | 71.559633 | 2496 |
| `navWreck` with `__builtin_expect(whole cond, 0)` | 9912 | 71.780900 | 2503 |
| `navWreck` `isDying() \|\| isDead()` (swapped) | 9908 | 74.512615 | 2599 |
| landmark loop `uint32_t i` | 9920 | 73.237822 | 2556 |
| landmark loop `uint32_t i` + no mask | 9908 | 74.541284 | 2600 |
| planet loop `int i` + `(int)size()` | 9908 | 74.541284 | 2600 |
| enemy `int cx/cy` -> `float cx/cy` + casts | 9872 | 70.933410 | 2466 |
| landmark guard `!landmarkHit && notTurretMode && i!=3 && notOtherLock` | 9924 | 71.004730 | 2477 |
| ... `notTurretMode && notOtherLock && !landmarkHit && i!=3` | 9932 | 70.822585 | 2471 |
| ... `!landmarkHit && i!=3 && notOtherLock && notTurretMode` | 9912 | 71.346705 | 2490 |
| `topOk` moved to 2nd / dropped (inline `top < screenY`) | 9908 | 74.397936 | 2595 |
| `int size = (int) this->field_0x124` | 9908 | 74.541284 | 2600 |
| `x = x + mode` instead of `x += mode` | 9908 | 74.541284 | 2600 |
| blip `setRefPixelPosition` via `int px/py` locals | 9908 | 74.541284 | 2600 |
| `cx`/`cy` hoisted to top of `if (onScreen)` | 9876 | 61.290786 | 2132 |
| df + `int otherLock` declared before `Status *status` | 9896 | 68.053364 | 2372 |
| df + negations not hoisted | 9868 | 66.714553 | 2322 |

## 4. Call-multiset status (baseline vs original)

The baseline still has these call-count deltas (orig / ours):

```
String::operator=            6 / 5
SolarSystem::getStations     2 / 3
SolarSystem::getStationEnumIndex 1 / 2
Level::getPlayer            29 / 32
Status::getSystem            5 / 7
KIPlayer::isDead            17 / 16
KIPlayer::isDying           13 / 12
PlayerEgo::goingToPlanet     1 / 2
PlayerEgo::isDockingToPlanet 4 / 5
PlayerEgo::getAutoPilotTarget 1 / 2
```

The 6 extra calls (`getStations`, `getStationEnumIndex`, 2x `getSystem`,
`goingToPlanet`, `isDockingToPlanet`, `getAutoPilotTarget`, 3x `getPlayer`)
all come from the **invented second planet-course recompute** inside the
planet block, which the original does not have (`strb #424` occurs once in the
original, twice in ours).  Deleting it fixes 7 of the 10 deltas but costs
~11 points (63.45 %) because the block layout then falls apart — see
`radar-draw-wave119-w6950.md` §2.  The remaining `String::operator=` delta is
the landmark `i==0` arm: the original assigns `this->lockLabel` there too
(`String::operator=` at `12e9b0` and `12eb0a`), not a local `String label`.
Making that change fixes the whole call multiset but costs ~3 points
(71.61 %).

So the *faithful* source shape is now fully pinned down (distance-first
`inSight`, no second recompute, both landmark arms -> `lockLabel`); it scores
far below the tuned baseline.  The blocker remains block placement /
register allocation, exactly as `w6947`/`w6950`/`w6957` concluded.

## 5. Residual structural deltas (unchanged)

* `vpush {d8-d13}` vs `{d8-d11}` — fixed only by the distance-first shape (§2).
* `mode` stack slot `sp+100` (orig) vs `sp+112` (baseline) / `sp+104` (df).
  `lockLabel` pointer swaps with it.
* Literal pools: orig at `12e340, 12e79c, 12ebf0, 12f200, 12f794, 12faf8,
  12feac, 1302dc`; ours at `12e2e0, 12e7a8, 12ebaa, 12f21c, 12f780, 12fb14`
  (6 pools vs 8).
* The planet-lock tail sits after the landmark block in ours, before it in the
  original; the invented recompute occupies the original's slot and is
  load-bearing for layout.
