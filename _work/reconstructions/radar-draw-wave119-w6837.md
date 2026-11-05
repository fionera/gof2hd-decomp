# Radar::draw pass (wave119, w6837) — faithful inSight, `i==1||i==3` fold, stack-slot rotation

**Status:** research note, 2026-11-05. Worktree `wt/w6837`, branch
`wave119-w6837`, based on current `master` (after `radar-draw-wave119-w6817.md`).
**No source change landed**; this commit contains this note only. The target
remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.426606 %** (official `verify.py` display 74.4). 2596 instructions match.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0.

## 1. Residual split: half of the 25.6 % is branch-target offsets

`SequenceMatcher` opcode accounting on the baseline (orig side):

```
mismatched: branch 406, non-branch 490   (ours: 404 / 484)
total branches: orig 832, ours 830
```

Branch targets are normalised to `loc_<function-relative offset>` (see
`tools/verify/verify.py:normalize`), so a branch only matches when the target
block sits at the *same* function-relative byte offset. A constant byte delta
between the two layouts therefore does not help — the delta must be **0**.
That is why every faithful shape change loses so much: it shifts every later
block and breaks ~200 branch matches even when the block itself is byte-identical.

## 2. Layout drift map (baseline vs original)

`layout2.py` (scratch) aligns the two objdump streams and prints the delta
`orig_addr - ours_addr` at each equal run. First divergence and the jumps:

```
delta  0x000  through 12e2b4 (prologue, draws, route)
delta +0x030  at 12e33e      <- otherLock block: ours' constant pool is at
                                 12e310, the original's at 12e340 (after the
                                 landmark guard); the 5 pool words are identical
delta  0x000  at 12e3d4      <- planet loop head re-synchronises
delta +0x008 .. +0x048       <- planet loop body / second recompute
delta -0x012  at 12ec22      <- planet-tail / enemies boundary (0x5a jump)
delta -0x012 .. -0x028       <- enemy section
delta +0x00c  at 12f432      <- asteroid section (0x34 jump)
delta +0x012 .. +0x014
delta -0x030  at 12f8a6      <- enemy-tail / asteroid-tail (0x44 jump)
delta  0x000  at 12fe6c, 130184 (88 instrs), 1302e6 (32 instrs), 130526
```

The first drift is *pool placement*, not code size: the otherLock block is
24 B of code in the original (12e2fc..12e314) but 40 B in ours (the
if-converted duplicate of `otherLock = 1`), and LLVM's ARM constant-island
pass then puts the 5-word pool before the landmark guard instead of after it.

## 3. New finding: `i == 1 || i == 3` is folded by clang, not by the original

`else if (__builtin_expect((i & 0x7fffffff) == 1 || (i & 0x7fffffff) == 3, 0))`
in the landmark loop (the `image_0xbc : image_0xb8` arm).

Original (12e9ea):

```
bic.w  r0, sl, #0x80000000
cmp    r0, #1
beq    12e9fc              ; image_0xbc
cmp    r0, #3
bne    12ea18              ; skip
ldr.w  r1, [r4, #184]      ; image_0xb8
b      12ea00
12e9fc: ldr.w r1, [r4, #188] ; image_0xbc
12ea00: <shared DrawImage2D>
```

Ours (baseline):

```
orr.w  r0, sl, #2
cmp    r0, #3
bne    ...
movs   r0, #184
cmp.w  sl, #1
it     eq
moveq  r0, #188
ldr    r1, [r4, r0]
```

clang 7.0.2 folds `(i&0x7fffffff)==1 || (i&0x7fffffff)==3` to `(i|2)==3`, and
if-converts the `image` ternary. Every spelling tried still folds:

| form | usize | raw % |
|---|---:|---:|
| baseline (mask + outer `expect(...,0)`) | 9908 | **74.426606** |
| mask, no `__builtin_expect` | 9912 | 72.667336 |
| `i == 1 \|\| i == 3` + expect | 9908 | 74.426606 (identical code) |
| `expect(...,1)` | 9896 | 71.883061 |
| `3 \|\| 1` reversed | 9908 | 74.426606 (identical code) |
| `(int)(i & 0x7fffffff)` / `0x7fffffffu` / `switch`-style | 9908 | 74.426606 |
| `!(... != 1 && ... != 3)` | 9924 | 72.682997 |
| expect on each / first / second term | 9912 | 72.667336 |
| `uint32_t i` loop counter | 9920 | 73.180516 |
| `uint32_t i` + no expect | 9912 | 72.667336 |

The unmasked `i == 1 || i == 3` form is a *tie* (identical codegen) and is the
more readable spelling, but it is not a measured improvement.

## 4. Faithful inSight block: float crosshairPos reproduces `d8-d13`/`s24`/`s26`

The original caches the **float** `PlayerEgo::crosshairPos.x/y` in `s26`/`s24`
across the `operator-` / `VectorLength` calls, converts with `vcvt.s32.f32`
between the two calls, and therefore pushes `{d8-d13}` (12 B) instead of the
baseline's `{d8-d11}` (8 B).  The only source shape found that reproduces this
is **distance-check first** plus **float locals**:

```cpp
float crossX = PlayerEgo::crosshairPos.x;
float crossY = PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > (int) crossX - this->field_0x124 &&
    this->screenX < (int) crossX + this->field_0x124 &&
    this->screenY > (int) crossY - this->field_0x124 &&
    this->screenY < (int) crossY + this->field_0x124) {
    inSight = true;
}
```

This reproduces the original block almost instruction-for-instruction
(`vldr s26/s24` before the call, `vcvt` between the calls, `bpl` then the four
bounds tests, `movlt r6,#1`) and gives `vpush {d8-d13}`.  It measures
**9864 B / 66.685813 %** — the same size/score as the `int cx/cy` distance-first
form (66.743251 %), because the block re-layout dominates.

Baseline inSight (bounds first, distance last) is also 114 B of code — the
same size — but computes the four bounds tests first and if-converts the
distance result with `ite mi; movmi/movpl`.  Keeping bounds-first and merely
using float locals does **not** hoist the loads (9872 B / 70.818352 %).

## 5. Stack-slot rotation in the 100..112 window

Original vs baseline slot map (from the entry stores and the planet/enemy
bodies):

| slot | original | baseline |
|---:|---|---|
| 100 | `mode` (entry `str r3`) | planet `getStatusValue()` spill / landmark `&labelStrings` |
| 104 | `getStatusValue()` spill | `landmarkHit` |
| 108 | `landmarkHit` | `missionBlocksPlanets` |
| 112 | `missionBlocksPlanets` | `mode` (entry `str r3`) |
| 116 | `stations` | (reused) |
| 148 | `turretMode` | `planetHit` |
| 152 | `planetHit` | `turretMode` |

This is a one-position rotation of 100/104/108/112 plus a straight swap of
148/152.  Nothing tried moves it:

* hoisting `bool planetHit` to function scope, next to `landmarkHit`, before
  `otherLock`, or after `turretMode` — identical codegen;
* `int` / `uint8_t` / `char` for `missionBlocksPlanets`, `planetHit`,
  `turretMode`, `landmarkHit`, `alienOrbit`, `onPlanetCourse`,
  `alwaysEnemySeen`, `bossSeen`, `enemyHit`, `asteroidHit`, `gasHit`;
* copying `mode` to a local (`int modeDelta = mode;`) — CSE'd away;
* freeing the `getStatusValue()` spill by swapping the `&` operands of the
  campaign-125 test — slot 100 is then taken by `&this->labelStrings` instead
  (9916 B / 74.311927 %).

## 6. `otherLock`: r5 is reachable, the straight chain is not

`int otherLock = a != nullptr || b != nullptr || c != nullptr || field_0x14 != 0;`
(OR4 form) puts `otherLock` in **r5** (`movs r5,#1`, matching the original) but
still if-converts the first two pointer tests and grows the body:

| form | usize | raw % |
|---|---:|---:|
| baseline `expect(all-null, 0)` if/else | 9908 | **74.426606** |
| OR4 assignment (`a\|\|b\|\|c\|\|d`) | 9916 | 70.110489 |
| `bool otherLock` OR4 | 9868 | 67.317914 |
| `int otherLock = (a\|\|b\|\|c) ? 1 : (d!=0)` | 9908 | 73.193807 |
| plain `&&` / inverted `\|\|` if/else / if-else chain | 9908 | 73.193807 |
| inner expect removed (outer kept) | 9908 | 74.397936 |
| `== 0` / `static_cast<bool>` / `!(a!=0\|\|...)` | 9908 | 73.193807 |
| OR4 + `int missionBlocksPlanets` | 9912 | 69.870875 |

The original's `cbnz / cbnz / ldr+cmp+beq` chain (single shared `movs r5,#1`)
was not reproducible from any C++ spelling tried; clang canonicalises them all
to the same IR and if-converts the second/third test.

## 7. Variants measured this session (all on the pristine baseline)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.426606** |
| distance-first inSight, int cx/cy (faithful) | 9864 | 66.743251 |
| distance-first inSight, float crossX/Y (reproduces d8-d13) | 9864 | 66.685813 |
| distance-first, y before x | 9864 | 66.657094 |
| bounds-first, float locals | 9872 | 70.818352 |
| bounds-first, x>,x<,y>,y< | 9936 | 71.238368 |
| bounds-first, y>,y<,x>,x< | 9936 | 71.238368 |
| bounds-first, x>,y<,x<,y> | 9908 | 74.387272 |
| `int inSight` | 9908 | 74.426606 |
| cx/cy declared before `enemy->field_0x73 = 0` | 9908 | 74.426606 |
| cx/cy hoisted to function scope | 9908 | 74.426606 |
| `float dist` local | 9920 | 63.533458 |
| nested distance check | 9908 | 74.369266 |
| recompute deleted (`norec`) | 9824 | 63.746398 |
| norec + faithful inSight | 9832 | 54.851713 |
| norec + faithful inSight + plain otherLock | 9828 | 55.011521 |
| norec + faithful inSight + OR4 otherLock | 9820 | 60.521388 |
| recompute before `enemies:` | 9936 | 62.925219 |
| recompute after the first `otherLock` | 9924 | 57.573150 |
| recompute after the second `otherLock` | 9932 | 58.121864 |
| prologue `else if` form | 9908 | 74.426606 (identical) |
| prologue hoisted `goingToPlanet` guard | 9832 | 70.444412 |
| `i == 1 \|\| i == 3` (no mask) | 9908 | 74.426606 (identical) |
| `i < 2` / `i == 0 \|\| i == 1` / `!(i > 1)` | 9908 | 74.426606 (identical) |
| `i==1?0xbc:0xb8` as if/else, reversed, or reversed ternary | 9908 | 74.426606 (identical) |
| `isDockingToAsteroid` condition flattened | 9908 | 74.426606 (identical) |
| `isDockingTo*` De Morgan | 9908 | 74.426606 (identical) |
| `isDockingTo*` swapped / reordered | 9884 / 9876 | 68.99 / 68.90 |
| landmark loop `egofirst` (egoPos before hit test) | 9908 | 69.912394 |
| landmark hit condition reversed | 9924 | 68.828932 |
| landmark hit condition split | 9912 | 70.704751 |
| `topOk` as `screenY > top` | 9908 | 74.426606 (identical) |
| `labelStrings` hoisted to local (2 placements) | 9916 / 9904 | 72.69 / 72.19 |
| campaign-125 `&` operands swapped | 9916 | 74.311927 |
| `int missionBlocksPlanets` | 9904 | 73.211982 |
| `uint8_t missionBlocksPlanets` / `char` | 9904 | 73.211982 |
| `uint8_t turretMode` | 9856 | 66.455969 |
| `uint8_t alienOrbit` | 9908 | 74.415938 |
| `uint8_t onPlanetCourse` | 9908 | 74.426606 |
| `uint8_t landmarkHit` | 9896 | 70.508961 |
| `uint8_t planetHit` | 9912 | 71.930076 |
| `uint8_t alwaysEnemySeen` | 9908 | 74.426606 |
| `uint8_t bossSeen` | 9908 | 72.630824 |
| `uint8_t enemyHit` | 9932 | 70.312053 |
| `uint8_t asteroidHit` | 9912 | 69.064336 |
| `uint8_t gasHit` | 9908 | 74.397936 |
| OR4 + `int missionBlocksPlanets` | 9912 | 69.870875 |
| OR4 + `uint8_t landmarkHit` | 9904 | 71.134021 |
| OR4 + `uint8_t turretMode` | 9888 | 65.834410 |

## 8. Conclusion

The baseline remains the best measured shape at **74.426606 % / 9908 B**.
The residual is now well characterised:

1. **~406 branch mismatches** from the block-layout drift (§2).  The first
   drift is the otherLock if-conversion (§6), which moves the constant pool
   and cascades through the whole function.  Fixing it requires the original's
   straight `cbnz` chain, which clang 7.0.2 will not emit from any spelling
   tried.
2. **~490 non-branch mismatches**, dominated by the inSight block (§4, 22+25),
   the invented second recompute (24), the otherLock chain (12), and the
   `i==1||i==3` fold (§3, 4).
3. **Stack-slot rotation** 100/104/108/112 and the 148/152 swap (§5) is
   insensitive to declaration order and integer width.

The most faithful source shape found (no second recompute, distance-first
inSight with float crosshairPos, plain otherLock) is 9828 B / 55.01 % — a
useful reference for a future byte-exact pass, but not a difflib improvement.
