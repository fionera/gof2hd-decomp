# Radar::draw wave 191b (w13829) — campaign upper-bound guard

**Status:** complete, 2026-11-13. Worktree `wt/w13829`, branch `wave191b-w13829`,
base `master` `da998189d`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13815.md` and the wave-176..191 notes.

**Landed:** the campaign guard gains an explicit upper bound:

```cpp
// before: if (campaign >= 2 && this->planetTargets != nullptr && !alienOrbit && ...)
if (campaign >= 2 && campaign < 0x100 && this->planetTargets != nullptr && !alienOrbit &&
    !this->level->getPlayer()->isDockingToPlanet()) {
```

Size-exact (9,908 B), `.rel.plt` byte-identical, and a 16-byte raw / `.text`
win with aligned unchanged.

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 6,925 | 1,303 | 9,908 | 76,629 | 623 |
| 1 (w13829) | `campaign >= 2 && campaign < 0x100` | **6,909** | **1,303** | **9,908** | **76,613** | **623** |

`.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical, MGame triple
(`OnTouchBegin` / `switchCamera` / `dockEvent`) 0/0/0. Full `check` gate green:
byte_exact 4508, linked_exact 4508, size-exact 4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, parity 5370/5370.

## 1. Why the upper bound helps

The original tests the campaign value in `sl` with `cmp.w sl,#2` and then an
unsigned range check.  The plain `campaign >= 2` form leaves the compiler free
to fold the later uses of `campaign` differently; adding `campaign < 0x100`
forces the same unsigned-range shape clang emits for the original, recovering
16 bytes in the guard/loop-entry cluster without changing the frame or any
branch target outside the function's own tail.  Unlike the `(unsigned char)
campaign` cast it does **not** perturb the object relocation order (see §3).

## 2. Sweep results (all size-checked; size 9,908 unless marked)

| campaign guard | raw | rel.plt | MGame | notes |
|---|---:|---|---|---|
| base `campaign >= 2` | 6,925 | OK | 0/0/0 | |
| **`campaign >= 2 && campaign < 0x100`** | **6,909** | **OK** | **0/0/0** | landed |
| `(unsigned char) campaign >= 2` | 6,358 | DIFF | 7/1/1 | campaign stays in `sl`; PLT reorder |
| `int campaign = (unsigned char) getCurrentCampaignMission()` | 6,369 | DIFF | 7/1/1 | same `sl` assignment, slightly different tail |
| `campaign > 1u` / `>= 2u` / `(campaign>>1)!=0` / `!=0&&!=1` / `(campaign&~1)!=0` | 6,926 | OK | 0/0/0 | +1 |
| `2 <= campaign`, `campaign - 2 >= 0` | 6,925 | OK | 0/0/0 | identical to base |
| `(unsigned short) campaign >= 2` | 8,404 | DIFF | — | size 9,916 ✗ |
| `campaign % 256 >= 2` | 9,515 | OK | — | size 9,856 ✗ |

The 16-byte win is a plateau: `(unsigned) campaign - 2 < 254`, `2 <= campaign && campaign < 0x100`,
`campaign <= 0xff`, `long campaign`, `uint32_t campaign`, and upper bounds `0x40`/`0x80` all give
exactly `raw=6909, aligned=1303, size=9908, rel.plt OK, MGame 0/0/0`.  Upper bounds `>= 0x200`
change codegen (size 9,912+ ✗); `campaign < 256u` changes codegen to 9,848 ✗.

Earlier sessions also tried `short campaign` (7,606, size 9,912 ✗),
`volatile int campaign` (9,006, size 9,844 ✗), `volatile int mbp` (9,527 ✗),
`bool alienOrbit` (9,457 ✗), `unsigned campaign` (6,926), and several
`__builtin_expect` / arm-order variants of the planet-lock and landmark guards;
none were size-exact wins.

## 3. The `unsigned char` cast and the `.rel.plt` order (root cause)

`(unsigned char) campaign >= 2` is the best raw result (6,358) but is gate-fatal:
`.rel.plt` slots 2731/2732/2733 at `0x213140..` reorder from the original
`getAutoPilotTarget, isDockingToAsteroid, isDockingToDockingPoint` to
`isDockingToAsteroid, isDockingToDockingPoint, getAutoPilotTarget`, and the
MGame triple becomes 7/1/1.

PLT order is the first relocation in the **object** relocation stream:

* base object: `getAutoPilotTarget 0x686`.
* cast object: `isDockingToAsteroid 0x6ae`, `isDockingToDockingPoint 0x6ba`,
  `getAutoPilotTarget 0x6da`.

In the original the `onPlanetCourse` autoPilot block is out-of-line at
`12e6ee` (reached by `12e1b6 beq.w 12e6ee`, ending `b.n 12e1bc`), i.e. placed
**before** the landmark tail at `12e748`.  Base places the same block at
`12e6d0`, before the landmark tail `12e722`.  The cast moves the block to
`12e724`, **after** the landmark tail at `12e6fe`, so `isDockingToAsteroid`
gets relocated first.  The block is fallthrough-reached at `12e1bc` in the
cast, so simply moving the source block breaks size (9,984/9,996 ✗).

## 4. Residual structure of the cast variant (if pursued further)

Cast `.text` is byte-identical to the original from `12e3c6`..`12febc`.
Remaining diffs:

1. `12e3cb..12e3da` stations setup / loop entry: original 5 insns vs ours 3.
2. `12fe60..12ff4a` planetHit/station-lock tail: original explicit
   `cmp.w r8,#0; beq; ldr r1,[r4,#8]; ldrb; cmp; cbz`; base/cast if-convert to
   `ittt eq / ldreq / ldrbeq / cmpeq` (original 2 insns longer).
3. Block placement of the landmarkHit/stationLockTimer tail
   (`12ff8e..130086`, `130454..1304ae`); the original `130486` guard is
   `movs r1,#0; cmp r0,#1; it lt; movlt r1,#1; orrs.w r0,r5,r1` (bool + `|`),
   while base/cast emit `movlt.w r1,#0xffffffff; cmp r5,r1`.
4. `1300xx..1302xx`: ~222+226 bytes in base, only 6/4 in cast — the cast fixes
   that whole region.

To capture the 6,358 raw win, the cast must emit the `getAutoPilotTarget`
block before the `isDockingToAsteroid` call site in the object relocation
stream while keeping the `12e1bc` fallthrough.  Untried ideas: move the
`landmarkHit` tail after the planet-lock block, or compute `onPlanetCourse`
in a nested scope placed textually before the landmark block.
