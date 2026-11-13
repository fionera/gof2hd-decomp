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

## 5. Second sweep (checkpoint 2) — campaign guard is locked

All rows start from the landed HEAD (`6,925 / 1,303 / 9,908`, `.text` 75,849,
`.ARM.extab` 594).  `✗` = size change (fatal).  The original lays the marked
body out **inline** (false edges go to `12e4a8`, body at `12e474`); the landed
build puts the body out-of-line at `12e5ca`.  That is the 22-instruction
`orig@12e472` cluster.  Every guard rewrite that moves the body inline is
size-fatal:

| campaign guard shape | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---:|
| **landed: whole-cond `expect(...,0)`** | **6,925** | **1,303** | **9,908** | **594** |
| `expect(campaignMission != nullptr, 1)` on first term only | 8,916 | 1,666 | 9,972 ✗ | 1,352 |
| nested `if (ptr && !isEmpty && isVisible) { if (range) }`, no hint | 9,340 | 1,572 | 9,980 ✗ | 1,428 |
| nested, outer `expect(...,1)` | 9,244 | 1,780 | 9,920 ✗ | 1,279 |
| nested, outer `expect(...,0)` | 9,184 | 2,733 | 9,952 ✗ | 1,313 |
| implicit bool `if (campaignMission && ...)`, `expect(...,0)` | 6,925 | 1,303 | 9,908 | 594 (identical) |
| sub-expression `expect(ptr && !isEmpty && isVisible, 1) && range` | 9,244 | 1,780 | 9,920 ✗ | 1,279 |
| sub-expression `expect(ptr && !isEmpty && isVisible, 0) && range` | 9,184 | 2,733 | 9,952 ✗ | 1,313 |

## 6. Landmark guard — the `expect` is load-bearing for size

The original emits explicit branches for `i == 2 || data[i] == nullptr ||
!data[i]->isVisible()` (`12e7e0..12e7fa`); ours if-converts the nullptr test
into `itt ne / ldrne / ldrne / cmpne / bne` (`12e7bc`).  Splitting the `||`
into separate `continue`s produces the original branch shape but changes the
function size:

| landmark guard shape | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---:|
| landed: single `||` with `expect(...,0)` on nullptr | 6,925 | 1,303 | 9,908 | 594 |
| three separate `if (...) continue;` | 9,237 | 1,734 | 9,976 ✗ | 1,352 |
| single `||`, no expect | 9,237 | 1,734 | 9,976 ✗ | 1,352 |
| single `||`, implicit `!data[i]` | 9,237 | 1,734 | 9,976 ✗ | 1,352 |
| two `if`s (`i==2 || null`, then `!isVisible`) | 9,237 | 1,734 | 9,976 ✗ | 1,352 |
| `expect(i == 2, 0)` on the first term | 9,502 | 1,430 | 9,860 ✗ | 1,225 |

## 7. Type / declaration sweep — no size-exact win

`identical` rows are codegen-neutral (same 6,925 / 1,303 / 9,908 / 594).
Everything else changes size:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `const int campaign` | 6,925 | 1,303 | 9,908 (identical) |
| `int turretMode` | 9,191 | 2,868 | 9,856 ✗ |
| `int alwaysEnemySeen` | 9,509 | 2,462 | 9,984 ✗ |
| `int alwaysEnemy` | 8,568 | 1,406 | 9,916 ✗ |
| `int landmarkHit` | 8,351 | 1,368 | 9,904 ✗ |
| `int bossSeen` | 8,715 | 1,420 | 9,880 ✗ |
| `int enemyHit` | 8,615 | 1,621 | 9,936 ✗ |
| `int wreck` | 7,371 | 1,373 | 9,912 ✗ |
| `int inSight` (both sites) | 6,925 | 1,303 | 9,908 (identical) |
| `int onPlanetCourse` | 6,925 | 1,303 | 9,908 (identical) |
| `unsigned alienOrbit` | 6,925 | 1,303 | 9,908 (identical) |
| `uint8_t missionBlocksPlanets` | 6,925 | 1,303 | 9,908 (identical) |
| `bool missionBlocksPlanets` (+ `+` guard) | 6,996 | 1,305 | 9,908 (worse) |
| `int marked` / `int8_t` / `char` / `uint8_t` / `int32_t` / `unsigned` / `short` | 7,591 | 1,407 | 9,912 ✗ |
| `bool marked = 0` / `bool marked(false)` / decl before `campaignMission` / `(int)marked*14` | 6,925 | 1,303 | 9,908 (identical) |
| `bool marked; marked = false;` | 6,925 | 1,303 | 9,908 (identical) |
| `volatile bool marked` | 9,705 | 3,970 | 10,180 ✗ |
| `volatile int marked` | 9,789 | 3,970 | 10,168 ✗ |
| `volatile bool planetHit` | 9,024 | 1,501 | 9,920 ✗ |
| `volatile bool landmarkHit` | 9,628 | 3,719 | 9,844 ✗ |
| `Array<int> *const stations` / `auto *stations` | 6,925 | 1,303 | 9,908 (identical) |
| `planetHit` declared before `stations` | 6,925 | 1,303 | 9,908 (identical) |
| `unsigned i` loop index | 6,925 | 1,303 | 9,908 (identical) |
| `marked` hoisted to loop top | 6,925 | 1,303 | 9,908 (identical) |
| `stations` declared outside the `if` | 9,468 | 1,788 | 9,988 ✗ |
| `int otherLockFlag` | 9,388 | 1,526 | 9,896 ✗ |
| `bool otherLock` | 9,227 | 2,840 | 9,860 ✗ |
| `bool notOtherLock` | 9,519 | 1,478 | 9,880 ✗ |
| `bool notTurretMode` | 9,500 | 1,487 | 9,876 ✗ |

## 8. Root cause of the remaining plateau

`fn_diff` reports `insn=3539/3532` — ours is 7 instructions *longer* at the
same 9,908 B, i.e. it uses more 16-bit encodings.  The call sequence is
identical (354 `bl`/`blx` each, same order modulo block reordering), so no
callee is wrong; the residue is pure block placement + register allocation.

The single lever is `marked`:

- original: `marked` lives in `[sp,#132]`, the planet-loop index in `fp`, and
  `campaign` in `sl` (`mov sl,r0 @12e0f8`, `cmp.w sl,#2 @12e38a`);
- ours: `marked` lives in `fp`, the loop index in `sl`, and `campaign` is
  spilled to `[sp,#144]`.  That shifts the whole frame slot map (e.g.
  `turretMode` is `[sp,#0x94]` in the original but `[sp,#0x98]` here, and the
  three-flag guard `orrs` reads `[144],[152],[148]` vs `[144],[148],[152]`).

Every source shape that forces `marked` out of `fp` (`int`/`int8_t`/`char`/
`volatile`, hoisting, reordering) either changes the size or makes the raw
diff worse, so the plateau is not reachable from `Radar.cpp` alone without a
compensating change elsewhere in the TU.  The EMP block residual
(`ldr.w r6,[fp]` hoisted before `getEmpDamageRate` + `mov r0,r6` at the call,
vs the original's late `ldr r0,[r5,#0] @12f34a`) is the same artefact: our
`Globals::Canvas` slot address is in `fp`, the original's is in `r5`.
