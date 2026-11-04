# `Generator::createMission` — wave 106 (w5409)

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(orig `0x971b8`, 1996 B, `.ARM.extab` LSDA record 168 B / `0xa8` in the object).

## Result

| variant | fuzzy | body B | `.ARM.extab` (obj) | gate |
|---|---|---|---|---|
| wave-105 landed (`amount = 0;` before `if (type == 8)`) | 81.5 | 1996 | 0xa8 | green |
| **landed**: `amount = 0;` moved into the non-8 `else` arm | **82.1** | **1996** | **0xa8** | **green** |

Full gate after landing: byte 4500, linked 4500, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `.ARM.extab` 0x26298, **RATCHET PASS**.

## The change

```c++
    int itemId = 0;
-   amount = 0;
    if (type == 8) {
        ...
    } else {
+       amount = 0;
        ++difficulty;
        switch (type) {
```

Semantically identical: `amount` is only read in the `type == 8` arm
(`setProductionGoods` at the end, which is reached on every path) and in the
`type == 11` reward arm (inside the same `else`).  The volatile store is still
performed on every non-8 path, before any read.

### Why it helps (measured)

Moving the store lets clang keep `type` in `r1` across the branch instead of
reloading it: the emitted `cmp r1,#8` now matches the original's `cmp r1,#8` at
`0x97422`, and three instructions disappear from the non-8 prologue
(`ldr r2,[sp,#36]; movs r1,#0; str r1,[sp,#104]`).  Body size is unchanged
(1996 B) because the store is simply relocated into the else arm.

Equivalent variants that all measure 82.1 (same size / extab):
`amount = 0;` before or after `++difficulty`; `int itemId;` uninitialised at the
top with `itemId = 0; amount = 0;` both in the else; `amount = itemId;` in the
else (itemId is provably 0 on that path).

## Measured variants (official scorer, all on the 81.5 base)

| variant | pct | usize |
|---|---:|---:|
| **`amount=0` in else (landed)** | **82.1** | 1996 |
| `amount=0` in else + `itemId` uninit + `itemId=0` in else | 82.1 | 1996 |
| `amount = itemId` in else | 82.1 | 1996 |
| `amount = 0; (void) amount;` in else | 81.1 | 1996 |
| `amount=0` in else + `pick==12` plain (no cold hint) | 79.3 | 1996 |
| `amount=0` in else + `pick==12` expect 1 | 69.4 | 1996 |
| `amount=0` in else + `rem` volatile | 78.1 | 1992 |
| `amount=0` in else + `amount` non-volatile | 69.8 | 1992 |
| `amount=0` in else + `getRoutes()==nullptr` expect 0 / 1 | 61.9 / 47.7 | 2000 / 2040 |
| `amount=0` in else + `race<4 && type==13` expect 1 / 0 | 63.5 / 41.6 | 1968 / 2028 |
| `amount=0` in else + `level` at first use | 80.1 | 2000 |

Other shapes re-measured on the old 81.5 base (none better): `pick==10 && race>3`
`&&` form 80.8; inner `race>3` expect 0/1 51.4/51.7; `pick==10 && race>3` expect 0
51.7; loop-condition expect 1/0 42.9/54.0; `do { } while (!selected)` 45.9;
`selected = true;` hoisted before p12 78.7; p8 expect 0/1 56.1/52.3; p10 plain
52.1; p10 plain + no pad 57.4; pad sweep over all 42 top-level statements — p15
(landed position) and the two statements above it tie at 81.5; pad as
`float`/`int`/`char`/`short` 77.6/77.3/77.4/77.4; no pad 71.0;
`volatile int amount = 0;` at decl 76.5; non-volatile `int amount;` 69.8.

## Remaining diff (proven vs hypothesis)

Proven (from the side-by-side dump at 82.1):
* the loop exit polarity is still inverted: ours `cmp r6,#12; beq` into the
  p12 body, original `cmp r6,#12; bne` over it (original's cold `getSystem`
  block is the fall-through).  This is the p10-expect → MBP rotation coupling:
  `__builtin_expect(pick == 10, 1)` gives `branch_weights 1, 2001, 2000`, MBP's
  `findBestLoopExit` picks the loop header as the exiting block and `rotateLoop`
  rotates the chain so the bottom is the `mov fp,#1; mov r2,r6` block -> `beq`.
  Without the p10 expect the loop does not rotate the same way (`bne`) but the
  p10 block goes cold and the score collapses (~52 %).
* frame 112 vs original 96; `agent`/`targetStation` stack slots swapped
  (`ldrd r4,r9,[sp,#24]` vs `ldrd r9,r4,[sp,#24]`); `rem` kept in `r9` instead
  of `[sp,#32]`; `getRoutes()==nullptr` inline `cbz` vs out-of-line
  `cmp/beq.w`; `nextInt(2)==0` inline vs out-of-line; `0.5f` vs `0.5000001f`
  pool placement.

Hypothesis: closing the remaining 18 % needs a source shape that makes the p12
body cold *and* keeps the p10 arm hot without triggering the MBP rotation above;
every form tried so far gives one or the other.

## Commands

```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target verify -j6   # syncs report.json
```

## Follow-up: p10/p12 branch-weight sweep (this session, no further gain)

The residual p12 block order is coupled to the `pick == 10` branch weight through
MBP's frequency-based chain formation, not just through `findBestLoopExit` as the
wave-105 note assumed.  Measured with the official scorer (all variants keep
`amount=0` in the else):

| variant | pct | usize | p12 branch | loop-cond branch |
|---|---:|---:|---|---|
| **landed** (p10 expect 1, p12 expect 0) | **82.1** | 1996 | `beq` (wrong) | `beq` (right) |
| p12 plain (50/50) | 79.3 | 1996 | `bne` (right) | `bne` (wrong) |
| p12 plain + p10 plain | 52.4 | 1988 | `bne` | `beq` |
| p12 plain + p10 expect 0 | 51.8 | 1992 | `bne` | `beq` |
| p12 expect 1 | 69.4 | 1992 | `bne` | n/a |
| p12 plain + p10 expect 1 + `used->data[pick]` expect 1 | 79.3 | 1996 | `bne` | `bne` |
| p12 plain + `pick==10 && race<=3` | 61.6 | 1972 | `bne` | `beq` |

The original has `p12=bne` **and** `cond=beq` (cond at the bottom).  Both are only
reached together when the p10 block is *not* hot, which moves the p10 block out of
line and collapses the score.  The full MBP trace (`-debug-only=block-placement`)
shows why: the p12 weight changes `FREQ %bb.18` (hot `selected=true` block) from
`500966735` (cold p12) to `250608609` (plain p12); that flips the
`hasBetterLayoutPredecessor` verdict for `%bb.19` from "non-cold CFG conflict"
(cold) to a viable `(CFG break)` candidate (plain).  Selecting `%bb.19` first makes
the loop chain start with the cond block, so `rotateLoop` bails and the cond stays
at the top.

`__builtin_expect` only emits 1:2000 / 2000:1 weights in clang 7, so there is no
intermediate weight available from source; `__builtin_expect_with_probability`
does not exist in clang 7.  The `assignType` shared label is still required (any
form that duplicates `type = pick` in both arms is CVP-folded to 1988 B / ~50 %).
