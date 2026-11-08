# Radar::draw wave 148 (w9401) — the expect(enemies) shape is real but the missing 136 B is spill code, not a block

**Status:** source change landed, 2026-11-08. Worktree `wt/w9401`, based on `master`
(`cbe3b5f7f`, wave 147). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Baseline (unchanged, gate green): original 9908 B; ours 9908 B; raw same-address diff
**8,672**; instruction-index alignment **2,726**. `.text` 86,160 differing bytes;
`.ARM.extab` 1,309 (size exact 156,312); `.rel.plt` 4; `.note.gnu.build-id` 20.

Read with `radar-draw-wave147-w9309.md`, `radar-draw-wave147-w9308.md`,
`radar-draw-wave146-w9206.md`.

## 1. The expect shape on the current (w9308) base

`__builtin_expect(this->enemies != nullptr, 1)` on the enemy-loop guard, on the
current base (which already carries w9308's `rowHeight` hoist and the witnessed
`bool enemyHit` / `bool asteroid wreck`):

| variant | usize | raw | align | DrawString | plays |
|---|---:|---:|---:|---:|---:|
| baseline | 9908 | 8672 | 2726 | 9 | 8 |
| **E2 `expect(enemies != nullptr, 1)`** | **9772** | 9147 | 2029 | 9 | 8 |
| E2 + no `rowHeight` hoist | 9772 | 9151 | 1855 | **8** | 8 |

So on this base the shrink is **136 B**, not 108 (w9309's 108 was measured on the
wave-146 base without the `rowHeight` hoist; the hoist is +28 B). The expect shape
keeps the 9-call DrawString census but **still emits 8 plays** — it does *not* fix
the 146/147 tail by itself.

## 2. Where the 136 B goes (region byte counts, orig vs E2)

Split at the two `scannerAvailable` (`[this,#0x1ab]`) loads and the `play` sites:

| region | orig | E2 | delta |
|---|---:|---:|---:|
| prologue + route | 836 | 800 | -36 |
| planet loop | 1088 | 894 | **-194** |
| landmark loop | 878 | 1062 | **+184** |
| enemy loop | 1886 | 1898 | +12 |
| gas/asteroid/music/cold | 4958 | 4898 | -60 |

The planet loop and landmark loop trade ~190 B: the expect shape keeps `wreck` in
`sl` and spills `&this->lockLabel` to a stack slot, whereas the original keeps
`&lockLabel` in `r9` and spills `wreck` to `[sp,#152]`. The original's planet loop
therefore carries extra `str`/`ldr` spill traffic (the original has ~15 more `ldr`,
~18 more `mov`, ~5 more `ldr.w`, ~3 more `str` than E2). **The missing body is
spill/reload code, not a source block.**

Concrete witnesses:
- original enemy-loop preheader: `add.w r9,r4,#396` (r9 = &lockLabel), `mov.w sl,#68`;
  E2: `add r1,r4,#396; str r1,[sp,#84]` (spilled), `mov.w r9,#68`.
- original `12ef44: strd r6,sl,[sp]` / `12f154: strd r5,sl,[sp]` / `12f39c`; E2 uses
  `strd r6,r9` / `strd r5,r9` — same width, different register.
- original spills `wreck`: `12ed82 str r0,[sp,#152]`, `12edb8 ldr r1,[sp,#152]`.

## 3. Forcing the original allocation with bool→int locals

The enemy-loop locals are `bool` in our source. Converting the *non-witnessed*
ones to `int` changes register pressure and, in some combinations, restores the
original's `sl`/`r9` split. Witnessed-bool locals (from w9206/w9308) are
`enemyHit`, `alwaysEnemy`, `asteroidHit`, and the asteroid-loop `wreck`.

All-int variants (extab exact 156,312, size exact unless noted):

| variant | usize | raw | align |
|---|---:|---:|---:|
| all bool→int | 9880 | 9083 | 1999 |
| all except `alwaysEnemy` | 9888 | 9082 | 2010 |
| all except `alienOrbit` | 9884 | 9101 | 2005 |
| all except `alwaysEnemy`+`alienOrbit` | 9892 | 9049 | 2012 |
| all except `alwaysEnemy`+`alienOrbit`+`missionBlocksPlanets` | 9896 | 9100 | 2007 |
| + `expect(enemy == locked, 0)` on that base | 9904 | 9093 | 2337 |
| + `expect(gasClouds != nullptr, 1)` on that base | 9916 | 8993 | 2247 |

None hit 9908 with `.ARM.extab` exact.

## 4. The 9908 hit — and why it cannot land

`String labelCopy2 = this->lockLabel;` in the landmark else-arm (w9308's un-merge
trick) on top of the all-int-except-`alienOrbit` base gives **exactly 9908 B**,
raw **8,549**, align **2,103**, DrawString 9, **plays 7** (the 146/147 tail merges
as in the original). But the extra `String` destructor grows `.ARM.extab` by 12 B
(156,312 -> 156,324), which pushes `.rodata` from `0x1c9e10` to `0x1c9e20`; every
function with a `.rodata` literal then differs, and `byte_exact` collapses
**4507 -> 4044**. Fatal for the ratchet. Every `String`-copy site (landmark i==0
arm, else arm, enemy lock, gas lock, route waypoint) grows extab by 12; the original
has 11 `StringD1Ev` and 1 `_Unwind_Resume`, same as our baseline, so the original
does **not** contain an extra `String` temporary.

## 5. Open questions / next levers

1. The 136 B is spill code. A source shape that keeps `&this->lockLabel` in `r9`
   across the enemy loop (so 68 lands in `sl` and `wreck` spills) without adding a
   temporary would reproduce the original's planet-loop/landmark-loop split. A
   `String *`/`String &` local forces the allocation but overshoots (+272 B,
   10044) and drops the DrawString un-merge to 8.
2. The extab +12 is the only blocker for the 9908/8549/2103 shape. A compensation
   that removes exactly one String cleanup elsewhere in `Radar::draw` while adding
   ~36 B of code would make it landable.
3. The original's planet-lock `!missionBlocksPlanets && planetLockTimer >= 1`
   compiles to `orrs r0,r5,r1` (8 ins) vs our 5; a `!(missionBlocksPlanets ||
   planetLockTimer < 1)` rewrite does not change it.

## 6. Gate (unchanged, note-only commit)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```


## 7. LANDED: the 9908 shape (wave 148)

The missing ~136 B is the **bool→int widening of the non-witnessed locals plus one
`__builtin_expect` on the `lockedEnemy` guard**. Measured on top of E2:

| variant (E2 base) | usize | raw | align | extab |
|---|---:|---:|---:|---:|
| baseline (wave 147) | 9908 | 8672 | 2726 | 156312 |
| E2 + all-int | 9880 | 9083 | 1999 | 156312 |
| E2 + all-int except `asteroidHit`,`gasHit`,`alienOrbit` | 9884 | 9095 | 2001 | 156312 |
| **+ `expect(lockedEnemy != nullptr && (isDying||isDead), 0)`** | **9908** | **8973** | **2127** | **156312** |
| E2 + all-int-except-`alienOrbit` + `String labelCopy2` | 9908 | 8549 | 2103 | **156324 (fatal)** |

The landed source keeps `bool asteroidHit` (witnessed), `bool gasHit`, `bool alienOrbit`;
the other non-witnessed locals (`enemyHit`, asteroid `wreck`, `alwaysEnemy`, …) are `int`.
The `expect(lockedEnemy …, 0)` grows the function from 9884 to 9908 (+24 B) and lifts the
alignment 2001 -> 2127 without touching `.ARM.extab`.

Trade: raw same-address regresses **8,672 -> 8,973** (+301); aligned improves
**2,726 -> 2,127** (-599). `.text` differing bytes **86,160 -> 86,461**. `.ARM.extab`
size exact (156,312), `.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical, `.rel.plt` 4,
`.note.gnu.build-id` 20. Gate: byte 4507, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

The `String labelCopy2` shape is still the best raw/aligned pair (8549/2103) but is
unlandable: its extra `String` cleanup grows `.ARM.extab` by 12 B, which shifts `.rodata`
by 16 B and collapses byte_exact 4507 -> 4044. Landing it needs `.text` to shrink by 12 B
in some *other* non-exact function so `.rodata` stays put.
