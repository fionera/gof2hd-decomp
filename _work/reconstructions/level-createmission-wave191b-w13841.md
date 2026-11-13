# Level::createMission — wave 191b w13841 (harvest of w13817)

Target: `_ZN5Level13createMissionEv` (`0xaffd8`, 17,172 B). Base master `44ad5324b`
(raw 3,375 / aligned 463 / size 17,172 exact / insn 6200/6197). All variants below keep
`.rel.plt` byte-identical.

## Landed: w13817 variant W (commit c992c1370)

Three source shapes from the unmerged `wave191b-w13817` branch, landed together:

1. **raid `guns` after the stream-out `if`.** The original computes
   `guns = alienGuns + fortressGuns` at the loop setup (`b37e6 add.w r5, sl, fp`), i.e.
   *after* the `raidRace == 8` spawn block, not before it.
2. **case-4 hunter loop drops the `enemies` alias.** The loop bound is
   `this->enemies->size()` so the original's per-iteration reload
   (`b41c8 ldr.w r0,[r8,#248]`) is reproduced. This leaves the function 4 B short, so the
   dead `if (hunterCount == 0) hunterCount = 1;` guard added by w13803 pass 27 is removed
   to restore size (the guard was only ever a size compensator).
3. **pirateFortress second rotate uses a named `q` temp** (`Vector q = ...; v = q;`),
   matching the original's reused `sp+212` output temp (`b34a2`).

Measured from the base: raw **3,375 -> 4,453** (+1,078), aligned **463 -> 393** (-70),
size 17,172/17,172 exact, insn 6198/6197. `.text` differing bytes 76,006 -> 77,084;
`.ARM.extab` content 552 -> 602. The raw rise is branch-offset/instruction-shift from the
`guns` move; the instruction-level alignment is the structural progress (same convention as
the w13820/w13799 passes). Gate `check` green: byte_exact 4508, linked_exact 4508,
data_exact 843/843, imports 207, lint CLEAN, parity 5370/5370.

## Experiments this pass (all reverted; no measured win)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| landed W | 4,453 | 393 | 17,172 ✓ | |
| W minus `q` temp | 4,791 | 478 | 17,164 ✗ | `q` is load-bearing for size |
| W with `Globals *globals` local restored | 4,453 | 393 | 17,172 ✓ | no codegen change |
| W, `start = pirateEscorts + guns + stationPirates` | 4,453 | 393 | 17,172 ✓ | addition order canonicalised |
| W, escort `int tenK = 10000` named | 4,453 | 393 | 17,172 ✓ | constant folded, no hoist |
| W, `Level *self = this` alias over the whole body | 4,453 | 393 | 17,172 ✓ | alias eliminated |
| W, `Level *self` alias in `case4Tail` only | 4,453 | 393 | 17,172 ✓ | alias eliminated |
| `bool hasWanted = wanted != nullptr` | 16,576 | 1,830 | 17,272 ✗ | +100 B, rejected |

## Remaining aligned clusters (landed W)

Per-region bad original instructions (aligned metric 393 total):

* `b3752..b39a0` raid block — 94. Dominated by a straight `r4`/`r8` swap:
  original keeps `this` in **r4** and `Globals::globals` in **r8**; ours keeps `this` in
  **r8** and globals in **r4** (and `alienGuns` in r6 vs the original's `sl`). The reload
  point is identical (`b34de ldr r4,[sp,#124]` / `ldr.w r8,[sp,#108]` / `ldr.w sl,[sp,#52]`
  vs ours `ldr.w r8,[sp,#124]` / `ldr r4,[sp,#108]` / `ldr r6,[sp,#100]`); only the
  allocator colouring differs.
* `b3274..b33a2` alien turret loop — 56. The `(bigAlienWave + (i < 0))` selector anchor is
  load-bearing (w100/w101); the residual is register colouring plus the `i<0` sign-bit
  `and`/`cmp` select vs the original's `cbz`.
* `b33da..b3512` pirateFortress body — 42. Original spills the loop index to `[sp,#116]`
  around `createStaticObject` because its `i` lives in r4; ours keeps `i` in r8 and needs
  no spill. Vector temp slots also differ (`sp+212` vs `sp+188/200`).
* `b4152..b42c6` shared tail — 27. Same colouring rotation: original this=r8, i=r9,
  globals=sl; ours this=sl, i=r8, globals=r9.
* `b3512..b3752` pirateFortress tail — 25. Mostly `ldr [pc,#...]` displacement shifts from
  a 4-byte earlier size difference, plus stack-slot reassignment.
* `b3dd0..b3dfe` extraAliens init — 12. Original order
  `pirateEscorts + guns + stationPirates + pirates`, `limit = start + extraAliens`, with
  `mov.w r8,#0` early; ours hoists `r8 = extraAliens` and emits a trailing `nop`.
* `b19bc..b1a00` case-4 goto — 8. Ours hoists `ldr.w sl,[sp,#124]` before `cmp r0,#0`;
  the original reloads `r8` only on the else path (`b19e0`) and again at `b19fc`.
* `b3ba0..b3bb8` escort init — 6. Original materialises `movw fp,#10000` once before the
  loop; ours swaps `fp`/`sl` and rematerialises `movw sl,#10000` inside the loop (3 extra
  instructions, size-compensated elsewhere).
* `b2aa2..b2ab2` wanted-loop spill — 3. Original spills `hasWanted` to `[sp,#104]`
  immediately after the `cmp`; ours spills it after the `orrs`.

These are all register-allocation / scheduler artefacts: the prologue is byte-identical
through `0xb005e` (both `mov sl, r0`), so the divergence is purely which callee-saved
register each long-lived value ends up in after the mid-function reloads. No source-level
alias, naming, or statement-order change tried so far moves the allocator.
