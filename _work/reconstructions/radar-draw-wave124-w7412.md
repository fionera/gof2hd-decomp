# Radar::draw pass (wave124, w7412) — first literal pool: water-block analysis, no source win

**Status:** research note, 2026-11-06. Worktree `wt/w7412`, based on current
`master` (`6f8ee2320`, wave-123 final). **No source change landed**; this
commit contains this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.541284 %** (official `verify.py` display 74.5), `M = 2600` matching
instructions. Gate at start and end: `RATCHET PASS`, byte **4506**, linked
**4506**, data 843/843, imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave123-w7307.md`, `radar-draw-wave122-w7209.md`,
`radar-draw-body-wave122-w7210.md`, `radar-draw-wave119-w6967.md`. This wave
re-derives the pool-placement mechanism from the LLVM 7.0.2 source and tests
a further ~120 guard/declaration/expect variants. The conclusion is unchanged:
the pool move and the `otherLock`/`status` register split are coupled through
the block layout, and every spelling that moves the pool re-rolls the body.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7412
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch harness `/dev/shm/w7412/score_radar.py` mirrors
`tools/verify/verify.py`'s `disassemble`/`reconcile` and reproduces the
official 74.541284 % exactly. Per-variant loop: edit `Radar.cpp`,
`cmake --build cmake-build-match --target link_so -j6`, re-score.

## 2. Exact mechanism: ARMConstantIslands water-block placement

The first literal pool is **not** placed after the last `ldr [pc,#…]` user.
It is placed by `ARMConstantIslands::handleConstantPoolUser` /
`findAvailableWater` (`llvm-7.0.2/lib/Target/ARM/ARMConstantIslandPass.cpp`):

* all CP entries start at the end of the function;
* CP users are processed in address order;
* for the first user that is out of range, `findAvailableWater` scans the
  `WaterList` (blocks that do **not** fall through, i.e. end in an
  unconditional `b`/`bx lr`/`pop {…pc}`) from the highest block number
  backwards and picks the **last water block whose post-offset is within
  ±1020 bytes** of the user (`isWaterInRange`);
* the island is inserted immediately before the block that follows that water
  block (`NewMBB = &*++WaterBB->getIterator()`).

The first CP user is `__stack_chk_guard` at `0x12e064`
(`ldr r0,[pc,#728]`, PC = `0x12e068`, Thumb-1 LDR imm8 range 1020 = `0x3fc`).
So the pool is placed after the **last water block in `[0x12e068, 0x12e464]`**.

### 2a. Baseline water blocks in range

`12e050`-relative and absolute water blocks (unconditional `b.n`/`b.w`) up to
`0x12e800` in the baseline:

```
12e1a6, 12e2de, | 12e490, 12e4c6, 12e4ca, 12e554, 12e558, 12e588,
12e5f2, 12e6c6, 12e720, 12e774, 12e7a4, ...
```

Only `12e1a6` and `12e2de` are inside `[12e068, 12e464]`; the last is the
route `onScreen` arm `12e2de b.n 12e310`. Hence the pool lands at `12e2e0`
(function-relative `0x290`).

### 2b. Original water blocks in range

```
12e1a6, 12e2de, 12e33c, | 12e4a6, 12e4da, 12e4f0, 12e562, 12e592,
12e5f8, 12e5fe, 12e604, 12e6e2, 12e6e8, 12e6ec, 12e746, 12e798, ...
```

The original has an extra water block at `12e33c` — the tail of the
**109/110 guard-skip block**:

```
12e328: beq.n   12e338      ; station index == 109 -> skip1
12e336: bne.n   12e354      ; station index != 110 -> check 111
12e338: movs    r1, #0      ; skip1  (landmarkHit = 0)
12e33a: mov     r5, r6      ; r5 = &Globals::status
12e33c: b.n     12e376      ; -> merge point  (WATER)
12e33e: nop                 ; 4-byte pool alignment
12e340: <first literal pool: 5 words>
```

`12e33c` is the last water block in `[12e068, 12e464]`, so the pool lands at
`12e340` (function-relative `0x2f0`). The pool contents are identical in both
binaries: `__stack_chk_guard`, `Globals::Canvas`, `Globals::status`,
`PaintCanvas::DrawImage2D`, `Globals::font`.

### 2c. Why the baseline has no water block there

In the baseline the 109/110/111 skip is a **single** block placed *after* the
111 check, and it falls through into the second `otherLock` chain:

```
12e340: beq.n   12e36c      ; 109 -> skip
12e34e: beq.n   12e36c      ; 110 -> skip
12e35c: bne.w   12e776      ; !=111 -> loop
12e368: ble.w   12e776      ; campaign <= 93 -> loop
12e36c: movs    r1, #0      ; skip (NOT water: falls through)
12e36e: ldr     r0, [sp, #124]  ; second otherLock chain
```

The original instead places the 109/110 skip *between* the 110 check and the
111 check, so it must branch over the 111 check to reach the merge point, and
the compiler clones a second skip block (`12e372`) for the
`landmarks == nullptr` / `campaign > 93` paths.

## 3. Variants measured this wave

All rows are single edits on the pristine baseline unless noted. `pool1` is
the first island address (function-relative; add `0x12e050` for absolute).

| variant | size | raw % | pool1 |
|---|---:|---:|---:|
| **baseline** | 9908 | **74.541284** | `0x290` |
| guard `!(109\|\|110) && (111!= \|\| <=93)` (`or`) | 9912 | 64.966328 | **`0x340`** |
| guard `109!= && 110!= && (111!= \|\| <=93)` (`and`) | 9912 | 64.966328 | **`0x340`** |
| guard `!(109\|\|110) && !(111 && c>93)` | 9908 | 74.541284 | `0x290` |
| guard `109!= && 110!= && !(111 && c>93)` | 9908 | 74.541284 | `0x290` |
| `or` + `expect(landmarks!=null,1)` | 9812 | 61.259707 | `0x344` |
| `or` + `expect(landmarks!=null,0)` | 9928 | 56.758690 | `0x290` |
| `or` + `expect(109==idx,0)` | 9828 | 60.658330 | `0x344` |
| `or` + `expect(110==idx,0)` | 9828 | 66.963772 | `0x294` |
| `or` + `expect(campaign<=93,0)` | 9904 | 72.169676 | `0x290` |
| `or` + `expect(campaign>=2,1)` | 9904 | 62.602209 | `0x340` |
| `or` + `expect(campaign>=2,0)` | 9928 | 48.375555 | `0x340` |
| `or` + `campaign > 1` | 9912 | 64.966328 | `0x340` |
| `or` + tail-before-recompute swap | 9932 | 64.470858 | `0x348` |
| `or` + `status` declared after chain | 9920 | 65.396734 | `0x348` |
| `or` + `bool otherLock` | 9920 | 71.379409 | `0x290` |
| `or` + `bool notOtherLock` / `bool notTurretMode` | 9920 / 9892 | 71.379 / 72.811 | `0x290` |
| `status` after chain | 9920 | 73.284630 | `0x2c0` |
| `status` after `notTurretMode` | 9920 | 73.284630 | `0x2c0` |
| `status` in `if (landmarks)` scope | 9856 | 70.154817 | `0x330` |
| `status` last (after `notTurretMode`) | 9920 | 73.284630 | `0x2c0` |
| `status` after chain + no inner expect | 9920 | 72.682997 | `0x2c0` |
| `status` after chain + `expect(chain,1)` | 9904 | 71.475692 | `0x328` |
| `expect(chain,1)` only | 9892 | 70.949240 | `0x298` |
| route `expect(route!=null,1)` | 9908 | 74.541284 | `0x290` |
| route `expect(route!=null,0)` | 9924 | 70.755123 | `0x39c` |
| `expect(!enabled,0)` | 10464 | 52.024236 | `0x2a4` |
| `expect(!enabled,1)` | 10272 | 36.624070 | `0x2d8` |
| `otherLock` chain field_0x1c first | 9924 | 66.303413 | `0x290` |
| `otherLock` chain gasCloud first | 9896 | 71.449067 | `0x290` |
| `otherLock` chain no inner expect | 9908 | 74.512615 | `0x290` |
| guard `landmarks` last | 9916 | 71.250537 | `0x290` |
| guard `landmarks` in the middle | 9908 | 73.308486 | `0x290` |
| guard 111-first | 9912 | 71.174785 | `0x290` |
| guard 109-111-110 | 9912 | 71.328271 | `0x290` |
| guard 110-109-111 | 9908 | 74.455275 | `0x290` |
| nested outer `!(111 && c>93)` then `109!= && 110!=` | 9912 | 71.174785 | `0x290` |
| `goto` skip (compiler folds away) | 9908 | 74.541284 | `0x290` |
| `landmarkHit` declared first | 9908 | 74.541284 | `0x290` |
| `notTurretMode` declared first | 9908 | 74.541284 | `0x290` |
| `otherLock` declared before `status` | 9908 | 74.541284 | `0x290` |
| tail-before-recompute swap (baseline guard) | 9928 | 71.451082 | `0x298` |

Two systematic searches were also run:

* 300 random 2–4-edit combinations of 10 independent tweaks (guard forms,
  `status` positions, `bool`/`int` types, `expect(landmarks)`): best
  `73.284630` (`status` after `notTurretMode`), pool `0x2c0`.
* exhaustive 1–3-edit combinations of 12 independent tweaks: best
  `74.541284` (`otherLock` declared before `status`, codegen-identical to
  baseline), pool `0x290`.

## 4. The `or` variant: pool exact, body re-rolls

`or` is the only spelling found that puts the pool at exactly `0x340` while
keeping the route/scanner order and the straight `cbnz` chain:

```
12e2de: b.n     12e2fc          ; route onScreen arm (same as original)
12e2e0: <scanner block>         ; same bytes as original 12e2e0-12e2fa
12e2fc: ldr     r5, [sp, #156]  ; status  (original: mov r1,r4)
12e2fe: mov     r1, r4
12e300: ldr.w   r0, [r1, #12]!
12e304: str     r1, [sp, #124]
12e306: ldr     r6, [r5, #0]
12e308: cbnz    r0, 12e316      ; straight cbnz chain preserved
...
12e340: beq.w   12e724          ; 110 -> skip
...
12e35e: <loop setup>
12e38c: b.n     12e6e6          ; WATER
12e390: <first literal pool>    ; = 0x340, exact
```

But the body block layout flips: the landmark loop is placed **before** the
planet block (`12e6e6` loop head) instead of after it (`12e7a8` in the
original), so `M` drops from 2600 to 2267 and the size grows to 9912 B. The
`or` AST (`(A!=111 || campaign<=93)`) makes the campaign check a `bgt skip`
whose fall-through is the body, whereas the baseline/original
(`!(A==111 && campaign>93)`) makes it a `ble body` whose fall-through is the
skip; that single branch-polarity difference drives the whole block layout.

`or_lmexp1` (`or` + `expect(landmarks!=null,1)`) gets within 4 bytes of the
target (`0x344`) but shrinks the body to 9812 B and scores 61.26 %.

**Gate impact of `or`.** The 4-byte size growth (9908 -> 9912 B) shifts every
function after `Radar::draw` in the linked image, which breaks 21 downstream
byte-exact functions:

```
verify: byte 4485  linked 4506  stubs 0  missing 0  extra 0  avg 99.93
  FAIL  byte_exact 4506 -> 4485
RATCHET FAIL
```

So even setting the score aside, `or` cannot be landed without the
`--update-baseline` the rules forbid. The baseline (9908 B, pool `0x2e0`) is
the only shape that keeps the gate green.

## 5. Remaining root cause

The first island is a pure **block-layout** artifact:

1. The pool goes after the last water block within 1020 bytes of `12e064`.
2. The original's last such water block is the 109/110 guard-skip at
   `12e33c`; the baseline's 109/110/111 skip is a single fall-through block
   at `12e36c`, so the route arm `12e2de` wins.
3. To recreate `12e33c` the compiler must (a) place the 109/110 skip *before*
   the 111 check, (b) make it branch to the merge point, and (c) keep the
   pre-guard code compact (no `ldr r5,[sp,#156]` before the chain,
   `movs r5,#1` rather than `mov.w r9,#1`). The original gets this from the
   `!(A==109 || A==110 || (A==111 && campaign>93))` AST plus its block
   placement; the baseline's identical AST produces a different layout
   because the local `Status *status` is live across the `otherLock` chain.
4. Every spelling that moves the skip block (and hence the pool) also flips
   the campaign-check polarity or the loop/planet-block order, which costs
   1–10 points — far more than the ~0.3 % the pool itself is worth.

No source-only edit found this wave improves the baseline.

## 6. Artifacts

Scratch (not committed, `/dev/shm/w7412`): `Radar.base.cpp` (pristine source),
`score_radar.py`, `tryv.py`, `trydump.py`, `diffruns.py`, `batch_guard.py`,
`batch2.py`–`batch13.py`, `search2.py`, `search3.py`, `cpi701.log`
(`-print-after=arm-cp-islands` from an LLVM 7.0.1 build, used only to inspect
the water list). The analysis is self-contained in this note.
