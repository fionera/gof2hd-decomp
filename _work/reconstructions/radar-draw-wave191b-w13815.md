# Radar::draw wave 191b (w13815) — mission-blocks-planets flag as `int`

**Status:** complete, 2026-11-13. Worktree `wt/w13815`, branch `wave191b-w13815`,
base `master` `342cf7a86`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13811.md`, `-w13797.md`, `-w13793.md`.

**Landed:** `missionBlocksPlanets` is an `int` (0/1), not a `bool`, and the
planet-lock guard is written as an additive disjunction:

```cpp
int missionBlocksPlanets = 0;          // was: bool missionBlocksPlanets = false;
...
    missionBlocksPlanets = mission->getType() != 172;
...
if (missionBlocksPlanets + (this->planetLockTimer < 1)) {
    goto enemies;
}
```

Size-exact (9,908 B) and a 70-byte raw / `.text` win with aligned unchanged.

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 6,995 | 1,303 | 9,908 | 75,919 | 594 |
| 1 (w13815) | `int` flag + additive guard | **6,925** | **1,303** | **9,908** | **75,849** | **594** |

`.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical. Full `check` gate green:
byte_exact 4508, linked_exact 4508, size-exact 4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, parity 5370/5370.

## 1. Why `int` helps

The original keeps `campaign` in `sl` (`mov sl,r0` at `12e0f8`, `cmp.w sl,#2`
at `12e38a`).  The `bool` build spills it to `[sp,#144]` and reuses `sl` for the
planet-loop index, which also moves `stations` to `[sp,#132]`/`0x84` and shifts
the entire frame (`str r3,[sp,#108]` at `12e05e` vs the original `[sp,#100]`).
Typing the flag `int` changes the register-pressure balance enough that the
frame and several branch shapes snap back toward the original; 30 of the 542
aligned-diff hunks change, all in the planet-lock tail and the frame setup.
The 70 recovered bytes are the branch-target / block-order fixes in the
planet-lock tail (`13044c..13053a`): the `bool` build carries a 6-byte-longer
layout at `13044e` (`ldrb.w r0,[r4,#536]` where the original has the
`planetLockTimer` threshold reload `ldr.w r0,[r4,#440]; b.n 130026`), which
shifts every following block and every branch target in the tail.

Note the flag's own guard gets *worse* under this variant: the original
`130486` sequence (`movs r1,#0; cmp r0,#1; it lt; movlt r1,#1; orrs.w r0,r5,r1`)
is matched byte-for-byte by the committed `bool` + `|` form, while the `int` +
`+` form emits `movlt.w r1,#0xffffffff; cmp r5,r1` (same instruction count).
The net is still -70 raw because the tail layout elsewhere dominates.

## 2. Guard-shape sweep (each row size-checked)

All rows start from the w13811 base (`6,995 / 1,303 / 9,908`).  `✗` marks a
size change (fatal under the gate).

| flag type / guard | raw | aligned | size | `.text` |
|---|---:|---:|---:|---:|
| **`int` + `mBP + (timer < 1)`** | **6,925** | **1,303** | **9,908** | **75,849** |
| `int` + `(mBP != 0) + (timer < 1)` | 6,925 | 1,303 | 9,908 | 75,849 |
| `int` + `mBP + (int)(timer < 1)` | 6,925 | 1,303 | 9,908 | 75,849 |
| `int` + `mBP + (timer < 1) != 0` | 6,925 | 1,303 | 9,908 | 75,849 |
| `int` + `mBP \| (timer < 1)` | 6,899 | 1,296 | 9,904 ✗ | — |
| `int` + `mBP \|\| timer < 1` | 7,088 | 1,299 | 9,904 ✗ | — |
| `int` + `mBP != 0 \|\| timer < 1` | 7,088 | 1,299 | 9,904 ✗ | — |
| `bool` + `(mBP != 0) \| (timer < 1)` (base) | 6,995 | 1,303 | 9,908 | 75,919 |

The `+` form is the only one that keeps the 9,908 B size, so it is the landed
shape.

## 3. Campaign marked region — hint is load-bearing

The original campaign condition falls through into the marked body
(`12e466 blt.n 12e474` skips only the second `getCurrentCampaignMission`), i.e.
the body is laid out inline.  Our `__builtin_expect(..., 0)` puts the body
out-of-line at `12e5ca`, which is the 22-instruction aligned cluster at
`orig@12e472`.  Every attempt to flip or move that hint is size-fatal:

| campaign condition | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---:|
| base: `expect(cond, 0)` | 6,925 | 1,303 | 9,908 | 594 |
| `expect(cond, 1)` | 9,268 | 1,735 | 9,956 ✗ | 1,355 |
| no hint | 9,340 | 1,572 | 9,980 ✗ | 1,428 |
| `expect(range, 0)` only (no outer hint) | 6,925 | 1,303 | 9,908 | 594 |
| `expect(range, 1)` only | 8,916 | 1,666 | 9,972 ✗ | 1,352 |
| `expect(cond, 0)` + inner `!=120` plain | 6,933 | 1,303 | 9,908 | 594 |
| `expect(cond, 0)` + inner `!=120` expect 0 | 6,932 | 1,309 | 9,908 | 594 |

The `.ARM.extab` blow-up (594 -> 1,355/1,428) on the flipped hints shows the
hint also drives `-Oz` inlining of the callees, so it cannot be moved without a
compensating change elsewhere.  The `expect(range, 0)`-only row is
codegen-identical to the base; the inner-hint variants are 7-8 bytes worse.

The range sub-expression itself is also load-bearing as a `||`; rewriting it as
`(unsigned)(cur - 148) >= 4`, `(cur < 148) + (cur >= 152)`, or a bitwise `|`
is size-fatal (9,936 / 10,024 / 9,912).

## 4. EMP block — still exhausted

Untouched by this pass.  The residual is the hoisted `ldr.w r6,[fp]` before
`getEmpDamageRate` plus `mov r0,r6` at the call (2 instructions vs the
original's late `ldr r0,[r5,#0]`); every `empCanvas` lifetime/declaration shape
tested by w13797/w13758 is neutral or size-fatal.  See
`radar-draw-wave191b-w13797.md` §3.
