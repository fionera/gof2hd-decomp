# Radar::draw wave 157 (w10306) — the landmark `68` hoist is causal; the original has no tail `String`; campaign-in-`sl` shapes measured

**Status:** analysis pass, no source change landed, 2026-11-08. Worktree `wt/w10306`,
based on `master` (`08575efff`, wave 156). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave156-w10205.md`, `radar-draw-wave156-w10204.md`,
`radar-draw-wave154-w10002.md`, `radar-draw-wave153-w10003.md`.

Baseline (HEAD, wave 156): original 9,908 B; ours 9,908 B; raw same-address diff
**8,320**; instruction-index alignment **1,296**; all 27 section sizes exact;
file size 2,187,328 B.

## 1. New ground truth: the original has no tail `String`

The w10204 landing added `String dummy;` at the function tail as a +12 B size
companion.  The original has **no `StringC1Ev`/`StringD1Ev` pair at the tail**:
its last body instruction is `str.w r0,[r4,#444]` (`field_0x1bc = 0`) at
`0x130328`, followed directly by the stack-canary epilogue at `0x13032c`.

Removing `String dummy;` from the wave-156 HEAD measures:

| shape | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (tail `String dummy`) | 9908 | 8320 | 1296 |
| HEAD without tail `String` | **9896** | **8317** | **1292** |

So the tail `String` is a pure size filler that slightly *hurts* alignment, and
the real source is missing ~12 B of genuine body code (not a tail temp).

## 2. The landmark `mov.w fp,#68` hoist is causal (confirmed)

Replacing the first landmark `DrawImage2D(this->image_0x88, ..., 17, 68)` with
`..., 17, 17)` (removing the `68` constant) moves `campaign` into `sl`:

```
12e0f6  blx  Status::getCurrentCampaignMission
12e0fa  mov  sl, r0          ; campaign -> sl   (original: 12e0f8 mov sl,r0)
```

Size drops to 9,852 B / raw 9,425.  So the `68` hoist is exactly what forces
`missionBlocksPlanets` into `sl` and spills `campaign`; the w10205 fixpoint
diagnosis is confirmed.

`const int` / `unsigned char` / non-const `int` locals holding 68 are
constant-folded and codegen-identical (still hoisted).  Passing a field
(`this->planetDockIndex`, `this->field_0x124`, `mode`, `i`) adds +4 B
(9,912) and does **not** move `campaign` to `sl`.

## 3. What the original actually does with `fp` in the landmark loop

The original's first landmark `DrawImage2D` call is

```
12e8dc  ldr.w r1,[r4,#136]     ; image_0x88
12e8e0  movs  r6,#17
12e8e2  ldrd  r2,r3,[r4,#252]  ; screenX, screenY
12e8e6  ldr   r0,[sp,#140]
12e8e8  ldr   r0,[r0,#0]
12e8ea  strd  r6, fp, [sp]     ; regionAlign=17, placeFlags=fp
12e8ee  blx   PaintCanvas::DrawImage2D
```

The callee `DrawImage2D(Ejiihh)` (member, so `this`=r0) reads `regionAlignFlags`
from `[r7,#8]` and `placeFlags` from `[r7,#12]`, i.e. both stack words are real.
In the original `fp` is the **planet-loop index** (set by `mov.w fp,#0` at
`0x12e3c8`, incremented at `0x12e4ec`, stored as `planetDockIndex` at
`0x12e5ae`), and it is never reloaded before `0x12e8ea`.  So the original does
**not** hoist `68` into `fp` in the landmark loop; it rematerialises `68` into
`r6` in the two `else if` arms (`0x12e918`, `0x12ea10`).

This is the structural reason `fp` is unavailable for `68`: the planet-loop
index stays live across the landmark loop as the `placeFlags` argument.

## 4. Register map (wave-156 HEAD vs original)

| value | original | HEAD |
|---|---|---|
| `campaign` | `sl` (0x12e0f8) | `[sp,#144]` |
| `missionBlocksPlanets` | `fp` (0x12e07a) | `sl` |
| `turretMode` | `[sp,#148]` | `[sp,#152]` |
| `otherLock` | `[sp,#144]` | `[sp,#148]` |
| landmark `i` | `sl` | `sl` |
| landmark `68` | `r6` (remat) | `fp` (hoist) |
| planet `i` | `fp` | `sl` |
| enemy `i` | `fp` | `r5` |
| enemy `68` | `sl` | `r6` (remat) |
| `&lockLabel` | `r9` | `[sp,#108]` |
| `wreck` | `[sp,#152]` | `sl` |
| `bossSeen` | `r6` | `r9` |

## 5. `bool otherLock` re-measured on wave-156 HEAD

`bool otherLock` (the only single toggle that reproduces `campaign`->`sl` /
`missionBlocksPlanets`->`fp`) is still short:

| shape | size | raw | aligned |
|---|---:|---:|---:|
| `bool otherLock` | 9792 | 9124 | 1653 |
| `bool otherLock` + `int otherLockFlag` | 9808 | 9123 | 1647 |
| `bool otherLock` + `int otherLockFlag` + `int bossSeen` | 9820 | 9147 | **1423** |
| `bool otherLock` + `int otherLockFlag` + `int marked` | 9816 | 9119 | 1626 |
| `bool otherLock` + `int otherLockFlag` + `int planetHit` | 9832 | 9100 | 1658 |
| `bool otherLock` + `int otherLockFlag` + `int alwaysEnemy` | 9812 | 9160 | 1646 |

All are 88-116 B short and none reaches 9,908 B.  The bool shape also uses more
16-bit instructions (3,512 vs the original's 3,559), so it is not the original
`otherLock` type; w10205's conclusion (original `otherLock` is `int`) stands.

## 6. Falsified / codegen-identical this wave

- `const int` / `const unsigned char` / non-const `int` `imagePlaceFlags = 68`
  (function scope or landmark preheader): codegen-identical.
- Declaration-order moves (`campaign` early, `missionBlocksPlanets` uninit,
  `alwaysEnemySeen` after `campaign`): codegen-identical.
- `const int campaign`, `unsigned int campaign`: codegen-identical.
- Hoisting `KIPlayer *landmark = this->landmarks->data()[i]` to the loop top:
  9,860 / 8,750 / 1,418; campaign still spilled.
- Replacing all three landmark `68`s with `this->field_0x124`: 9,932 / 8,651 / 1,406.
- Moving the first landmark `DrawImage2D` after the `DrawString`: 9,880 / 8,699 / 1,328.
- Removing the `if (this->lockedStation == nullptr)` guard around
  `field_0x28 = 0` in the landmark-lock else: 9,900 / 8,481 / 1,300.

## 7. Open questions

1. The original's `placeFlags` in the first landmark `DrawImage2D` is `fp` =
   planet-loop index.  A faithful source shape that keeps the planet index live
   across the landmark loop (e.g. a shared function-scope index or a variable
   `placeFlags`) should stop the `68` hoist and snap the prologue.  All
   constant-valued attempts so far are folded.
2. The real source is ~12 B larger than HEAD-minus-tail-`String` (9,896).  The
   original has 7 `stationLockTimer` stores vs our 5; two genuine stores are
   missing, likely in the landmark-lock else (`field_0x28 = 0;
   stationLockTimer = 0;` appears unconditional in the original at `0x12fe60`).
3. Once `campaign` is in `sl`, the enemy loop must also move `68` from `r6` to
   `sl` and `&lockLabel` from the stack to `r9`; the bool shape shows the
   `r9`/`sl` tie-break is still wrong.

## 8. Gate

No source change landed; HEAD gate unchanged (byte_exact 4507, linked_exact
4507, size 4524, data 843, imports 207, lint clean, drift 0).
Radar::draw stays raw **8,320** / aligned **1,296** at 9,908 B.
