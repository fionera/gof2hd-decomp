# HangarWindow::render — wave 192 (w13870): buy-list String declaration order

Status: landed.  Gate green (`check`): byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Change

In the buy-credits list loop (`if (this->buyCreditsActive)` / `if (this->listModeFlag)`),
swap the two default-constructed String declarations back to `description` first:

```c++
String description("");
String price("");
```

The base tree (after w13852) had `String price("");` before `String description("");`.

## Measurement

| metric | before | after |
|---|---:|---:|
| raw | 1,393 | **1,395** (+2) |
| aligned | 407 | **396** (-11) |
| size | 9,700 | 9,700 exact |
| insn | 3,734 | 3,733 |
| `.text` | 76,786 | 76,786 (+0) |
| `.ARM.extab` | 594 | 594 (+0) |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / identical | unchanged |

The +2 raw is a branch-offset shift; the aligned drop is the structural progress
(same trade as `Level::createMission` w13849 and the earlier w13799 pass).

## Why it helps (Thumb evidence)

Original buy loop `0x14a8ec`: `str.w r9,[sp,#132]` (only `this` stored) and the ctor
order at `0x14a93a` is `mov r0,r6` (r6 = sp+312 = price) then `mov r0,r5`
(r5 = sp+300 = description).  With `description` declared first, clang assigns
`sl` (sp+312) to the price temp and `r5` (sp+300) to description, matching the
original's r5/r6 pair instead of the previous r5/sl pairing.

Note `sl` is not `layout` throughout the function: it is a String temp at
`0x14923c`, `0x14956a`, `0x149924`, `0x149a38`; layout lives at `[sp,#132]`
early and is reloaded at `0x1492a8`, `0x1496da`, `0x149c16`.

## Remaining clusters (not yet landed)

* buy-loop `sl` liveness: original keeps `layout` in `sl` across the loop
  (`ldr r2,[sl]` at `0x14abc2`), ours reloads from `[sp,#132]`.
* ship-guard early `ldrd r5,r4,[sp,#132]` hoist before `cbz` (`0x149b20`).
* free-credits `textY` kept in `r6` for the second DrawString (no second vcvt/spill).
* literal-pool order at `0x14ae94` (`"+", ""` vs ours reversed).
