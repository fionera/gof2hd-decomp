# Radar::draw wave 173 (w11809) — planet-lock guard OR order + frame-test expect

**Status:** complete, 2026-11-11. Worktree `wt/w11809`, branch `wave173-w11809`,
base `master` `3fafa5b03`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave171-w11707.md` (the full re-sweep on the post-w11607
base) and `radar-draw-wave171-w11607.md` (guard-clause restructure).

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,054 |
| instruction-index alignment | 1,582 |
| instruction count | 3,540 ours / 3,532 orig |
| `.rel.plt` | byte-identical (0) |
| `.ARM.extab` differing bytes | 711 |
| `.text` differing bytes | 81,503 |

## 1. Landed (one commit, two sites)

| # | site | change | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | planet-lock guard (line 642) | `missionBlocksPlanets \|\| planetLockTimer < 1` -> `planetLockTimer < 1 \|\| missionBlocksPlanets` | 7,054 -> **6,864** | 1,582 -> **1,574** | 9,908 |
| 2 | planet-lock frame test (line 646) | `__builtin_expect(field_0x18 != 0, 1)` on the first `&&` operand | 6,864 -> **6,849** | 1,574 -> **1,577** | 9,908 |

Both are size-exact; `.rel.plt` stays byte-identical; `.ARM.extab` is unchanged
at 711.  Whole-`.so` `.text` differing bytes 81,503 -> **81,298** (-205).
`check` green after the commit (byte 4,507 / linked 4,507 / size 4,524 /
data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370).

### 1.1 Why the OR swap is the win

The original planet-lock guard is a single combined test after the
`planetLockTimer > field_0x1b8` block (original `0x130486`):

```
130486 ldr.w r0,[r4,#416]   ; planetLockTimer
13048a movs  r1,#0
13048c cmp   r0,#1
13048e it    lt
130490 movlt r1,#1
130492 orrs.w r0,r5,r1      ; missionBlocksPlanets | (timer < 1)
130496 bne.w <enemies>
```

With the source order `missionBlocksPlanets || timer < 1` clang 7.0.2
path-splits the test: the `timer <= field_0x1b8` path gets a hoisted
`missionBlocksPlanets` check and the fall-through path only re-tests
`timer < 1`, so the `orrs` never appears and the surrounding block layout
shifts.  Writing `timer < 1 || missionBlocksPlanets` makes clang emit the
original `movs/cmp/it lt/movlt/orrs/bne` sequence verbatim (the two operands
are independent loads/compares, so the swap is semantics-preserving).  This
is the single largest local win in the block: 190 raw bytes.

### 1.2 Why the frame-test expect is the win

The original planet frame test (original `0x13049a`) is a plain
`cbz`/`beq` pair:

```
13049a ldr  r0,[r4,#24]     ; field_0x18
13049c cbz  r0, <else>
13049e ldr  r1,[r4,#20]     ; field_0x14
1304a0 cmp  r0,r1
1304a2 beq.n <else>
```

`__builtin_expect((this->field_0x18 != 0), 1)` on the first operand keeps the
true path as fall-through and re-places the `else` (`isDockingToPlanet`
guard) exactly where the original has it, removing a 2-byte layout shift in
the station-lock block.  The expect on the *second* operand
(`field_0x18 != field_0x14`) measures identically (6,849 / 1,577 / 9,908) but
produces a different body; the first-operand form was kept because it matches
the existing `expect(field_0x28 != 0, 1)` idiom in the same function.  The
same-line `// lint: arm_builtin` waiver is required.

## 2. Lock-landmark `!lockConfirmed` expect (re-confirmed size-fatal)

`if (__builtin_expect(!this->lockConfirmed, 0))` in the station-lock block
(original `0x1303d2`) is still **+8 B** (9,916) on this base and raw-worse
(6,905).  Measured variants, all 9,916 / 6,905: `expect(!lockConfirmed,0)`,
`!expect(lockConfirmed,1)`, `expect(lockConfirmed,1)==0`,
`expect(!lockConfirmed,0)==1`, and the `if (expect(lockConfirmed,1)) {} else`
form.  `expect(!lockConfirmed,1)` is a codegen no-op (6,849 / 9,908).  The +8
is the extra `sp+220` `String` destructor landing-pad site in the exception
cleanup tail (same effect reported in waves 166-171).  The plain
`if (!this->lockConfirmed)` already matches the original `ldrb/cbnz` shape,
so no expect is needed there.

## 3. Sweeps run (all size-exact cells only)

On the post-OR-swap base (`raw 6,864`):

* **whole-condition expect sweep** — every `if`/`while` condition in the
  landmark + planet-lock region (50 unique conditions, 86 cells, expect 0/1):
  no cell below 6,849.
* **top-level subterm expect sweep** — every top-level `&&`/`||` operand
  (106 cells, expect 0/1): only the frame-test first/second operand at
  expect 1 (6,849).  `expect(field_0x28 != lockedStation, 0)` is 6,874;
  `expect(field_0x28 != 0, 1)` wrapped in an outer expect 0 is 6,850.
* **operand-order swaps** — 23 adjacent `&&`/`||` reorderings in the landmark
  and planet blocks: all neutral (6,864) or worse; the OR swap above was the
  only win.
* **type/shape variants** — `planetHit`/`landmarkHit`/`marked`/`turretMode`
  as `int`, `otherLock` as `bool`, `onPlanetCourse` as `int`, local
  `Sprite *blip` pointers, nested-if restructures, `int frame = 0`: all
  neutral or worse (several are non-size-exact).
* **`isDockingToPlanet` guards** (planet-block entry line 532 and frame
  else-arm line 650): plain / `expect(...,0)` / `expect(...,1)` / nested
  outer-inner forms are all neutral (6,849 / 9,908); the nested
  `expect(expect(...,1),0)` currently in the source is codegen-equivalent to
  plain here.

## 4. Remaining residual

The dominant residual is unchanged: the original keeps `campaign` in `sl`
(`12e0f8: mov sl,r0`, `12e38e: cmp.w sl,#2`), `missionBlocksPlanets` in `fp`,
and `stations` in `sl` after the campaign check; ours spills `campaign` to
`sp+144` and keeps `missionBlocksPlanets` in `sl`.  This cascades through the
landmark/planet loops and also makes clang pick `r5 = this+0xf4` (pre-index
`ldr.w r0,[r5,#244]!`) instead of the original's `r5 = blipSprite` in the
three frame paths.  Fixing the campaign allocation is the next lever.

## 5. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: scratch/w11809/{sweep.py,gen_expect.py,gen_sub.py,gen_reorder.py,
#          expect.json,sub.json,reorder.json,planet.json,lock.json,more.json}
```
