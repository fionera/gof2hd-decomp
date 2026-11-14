# Radar::draw — wave 192, w13940 (branch-condition pass)

Function `_ZN5Radar4drawEP6PlayerP3Hudi`, 9,908 B, size-exact throughout.

## Landed win

`src/game/weapons/Radar.cpp:346`: `int missionBlocksPlanets = 0;` -> `bool missionBlocksPlanets = false;`
(assignment at :351 stays `missionBlocksPlanets = mission->getType() != 172;`).

| metric | before | after |
|---|---|---|
| raw | 6,945 | 7,016 |
| aligned | 1,322 | **1,319** |
| usize | 9,908 | 9,908 |
| `.text` diff bytes | 76,875 | 76,991 |
| `.ARM.extab` diff bytes | 597 | 552 |
| `.ARM.exidx` / `.rodata` | 0 | 0 |

Gate: byte_exact 4508/4524, linked_exact 4508/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, parity 5370/5370, RATCHET PASS.

## Branch-condition findings (first divergence +0x2c)

- `+0x2c` is pure register allocation, not a condition: orig `12e07a mov.w fp,#0`
  vs ours `mov.w sl,#0`. Original zero/false register is fp/r11 (83 uses) and it
  doubles as the `planetTargets` loop index (`12e3da cmp fp,r1`, `12e4ec add.w fp,fp,#1`,
  `12e500/12e55c/12e566 cmp fp,r0`) and the `planetHit=false` store
  (`12e3ba str.w fp,[sp,#112]`). Ours splits this: sl = zero register, fp = loop counter.
- `12e0a4..12e0d4` byte-identical: mission-type chain (source 348-350) is correct.
- `12e0f8`: orig `mov sl, r0` (campaign kept live in r10) vs ours `str r0,[sp,#144]`
  (spill). This is the dominant remaining allocation difference; campaign is live
  only from decl (:356) to the `campaign >= 2` test (:537), yet the original holds it
  in a callee-saved register across that whole range.
- `12e0e8`: orig `str r0,[sp,#148]` (turretMode) vs ours `[sp,#152]`.
- `12e3a6`: orig `str r1,[sp,#108]` vs ours `[sp,#112]`; `12e3b8` orig
  `str.w fp,[sp,#112]` vs ours `[sp,#116]` — stack-slot shifts, not semantics.
- `12e388 movs r0,#1 / 12e38a cmp.w sl,#2 / 12e38e str r0,[sp,#144] / 12e390 blt.w 12ec24`
  => `otherLock = 1` (true), signed `blt`, and **no upper bound**: the original tests
  only `campaign >= 2` (`cmp.w sl,#2`), not `campaign < 0x100`.
  Removing our `< 0x100` makes `12e38e` byte-identical shape (`cmp r0,#2 / blt`, only
  register r0 vs sl and the `str [sp,#148]` vs `[sp,#144]` differ), but measures
  7,031 / 1,319 / 9,908 — same aligned, higher raw, so not kept.
- `12e394`: `planetTargets == nullptr || alienOrbit` skip (source :537) confirmed.
- `12e552`: orig uses two separate signed compares where ours fuses
  `itt gt / addgt / bgt` (source :602) — same semantics, shape-only.
- KIPlayer cull `0x12ee2e..0x12ee8c` does `+24000` before `-24000` per axis; swapping
  source order measures 1,339 aligned (worse).

## Measured variants (all usize 9,908 unless noted)

| edit | raw | aligned |
|---|---|---|
| clean tree | 6,945 | 1,322 |
| `bool missionBlocksPlanets` | 7,016 | **1,319** |
| `int missionBlocksPlanets` + ternary | 6,945 | 1,322 |
| swap `alienOrbit`/`campaign` decls | 7,023 | 1,324 |
| `SetColor` before zero stores | 7,047 | 1,327 |
| `bool notOtherLock` + `bool notTurretMode` | 9,373 | 1,462 (usize 9,896) |
| `bool notTurretMode` alone | 9,428 | 1,455 (usize 9,884) |
| `bool alienOrbit` | 9,483 | 1,428 (usize 9,880) |
| `campaign` decl before `turretMode`/`alienOrbit` | 8,592 | 1,355 (usize 9,896) |
| `campaign` decl after `planetTargets` | 7,093 | 1,324 |
| remove `campaign < 0x100` | 7,031 | 1,319 |
| `campaign <= 0xff` / `const int campaign` | 7,016 | 1,319 |
| swap `lockConfirmed`/`plasmaInRange` zero stores | 6,947 | 1,322 |
| :602 `screenY < top+size` rewrite | 8,305 | 1,323 (usize 9,904) |
| ±24000 compare-order swap (:893-898) | 7,016 | 1,339 |
| swap ±24000 order in KIPlayer cull | 7,016 | 1,339 |

Dead ends not to retry: `(unsigned char)campaign`; `(planetLockTimer<1)|missionBlocksPlanets`;
`planetHit`; `|` instead of `+`; `rowHeight` into the `i == 0` arm; `bool` on
`alwaysEnemySeen`/`alwaysEnemy`; all rows above that did not lower aligned.
