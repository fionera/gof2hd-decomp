# Radar::draw wave 191b (w13824) — EMP block + campaign/marked audit (note-only)

**Status:** complete, 2026-11-13. Worktree `wt/w13824`, branch `wave191b-w13824`,
base `master` `a37bfa0a7`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13811.md` (planet rect), `-w13797.md` (bossSeen
hint drop) and `-w13793.md` (planet-lock OR). Those three are landed; this pass
did **not** touch them.

**Result: no measured win.** Every source variant that kept the function at
9,908 B produced **byte-identical** code to the baseline (raw 6,995 / aligned
1,303 / `.text` 76,746). The remaining EMP and campaign/marked deltas are pure
register-allocation / stack-slot artefacts of the whole function, not local
source-order choices. `src/` is left clean; this note is the only artefact.

Baseline (re-verified this session, full `check` green):

| raw | aligned | size | `.text` | `.ARM.extab` | `.ARM.exidx` | `.rodata` |
|---:|---:|---:|---:|---:|---:|---:|
| 6,995 | 1,303 | 9,908 / 9,908 | 76,746 | 625 | 0 | 0 |

`.rel.plt` byte-identical, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. Variants measured this pass (all size-exact unless noted)

`drive.py` builds `link_so -j2` under `ulimit -v 12000000` and prints
raw/aligned/size + section byte-diff counts.

| variant | source change | raw | aligned | size | `.text` | extab | exidx |
|---|---|---:|---:|---:|---:|---:|---:|
| base | — | 6,995 | 1,303 | 9,908 | 76,746 | 625 | 0 |
| n1 | nested `if (x) { if (y) {` rect test | 6,995 | 1,303 | 9,908 | 76,746 | 625 | 0 |
| n2 | nested last y-condition only | 6,995 | 1,303 | 9,908 | 76,746 | 625 | 0 |
| n3 | `screenY < top + size` spelling | 8,375 | 1,303 | **9,904** | 384,426 | 625 | 1,047 |
| n4 | `int bottom = top + size;` local | 6,995 | 1,303 | 9,908 | 76,746 | 625 | 0 |
| n5 | nested `if (screenY < top + size)` | 8,375 | 1,303 | **9,904** | 384,426 | 625 | 1,047 |
| hull-temp | `float rate = getDamageRate()/100.0f;` then `hull = field * rate` | 6,999 | **1,360** | 9,908 | 76,750 | 625 | 0 |
| emp-direct | drop `empCanvas` local, call `Globals::Canvas->` twice | 7,505 | 1,306 | **9,912** | 89,630 | 624 | 25 |
| emp-direct+hull-temp | both of the above | 6,997 | **1,339** | 9,908 | 76,748 | 625 | 0 |
| ph-before-turret | hoist `bool planetHit` above `turretMode` | 6,995 | 1,303 | 9,908 | 76,746 | 625 | 0 |
| ph-top | hoist `bool planetHit` to the top local group | 6,995 | 1,303 | 9,908 | 76,746 | 625 | 0 |
| ph-before-turret+otherlock-bool | `bool otherLock` | 9,230 | 2,839 | **9,860** | 441,063 | 1,287 | 3,544 |
| int-marked | `int marked = 0` | 7,591 | 1,401 | **9,912** | 89,716 | 623 | 25 |
| int-marked+int-ph | `int marked` + `int planetHit` | 9,201 | 1,861 | **9,964** | 442,586 | 1,349 | 3,775 |
| int-marked+expect1 | `int marked` + `__builtin_expect(..., 1)` | 9,208 | 1,629 | **9,956** | 442,312 | 1,346 | 3,814 |
| int-marked+int-ph+expect1 | all three | 9,338 | 1,749 | **9,960** | 442,540 | 1,347 | 3,813 |

Earlier w13824-session variants (same conclusion, kept for the record):
a/b int/uint-marked 7,591/1,401/9,912; c-or-form 6,995/1,303/9,908;
d-order-tp 9,204/2,808/9,948; e-order-po 6,995/1,303/9,908; f-int-planethit
9,045/1,875/9,968; g-emp-2stmt 6,995/1,303/9,908; h-emp-barwidth-after-call
7,941/1,441/9,900; i-emp-barwidth-after-call2 6,995/1,471/9,908; k-emp-ref
7,505/1,306/9,912; l-emp-constptr 6,995/1,303/9,908; m-stations-const,
n-stationsdata, o-marked-before-mission, p-marked-static-scope, q2-marked-hoist,
r2-marked-toplevel, w1..w5, x3, y1/y2, z1, z3, aa, ab, af, ag all
6,995/1,303/9,908 (byte-identical); q3-int-marked-hoist 7,591/1,401/9,912;
t-emp-local-both 8,181/1,313/9,904; u2-emp-ptrimages 6,998/1,400/9,908;
v-emp-intimages 6,995/1,303/9,908; x1-canvas-before-call 7,505/1,306/9,912;
x2-canvas-after-emp 6,996/1,353/9,908; z2-bitwise-and 8,821/1,416/9,944;
ac-campaign-early 8,160/1,302/9,900; ad-campaign-before-mission
8,212/1,309/9,900; ba-ternary-images 6,995/1,311/9,908; bb-bar-first
6,995/1,319/9,908.

## 2. Why nothing moves: the original's stack-slot map differs

The three OR operands sit in different stack slots, which changes load fusion:

- original `12e4dc`: `ldr r5,[sp,#156]; ldr r0,[sp,#144]; ldr r1,[sp,#152];
  orrs r0,r1; ldr r1,[sp,#148]; orrs r0,r1; lsls r0,r0,#31; beq`
  → **three separate loads**, slots `144 / 152 / 148`.
- ours `12e4ae`: `ldr r5,[sp,#156]; ldrd r0,r1,[sp,#144]; orrs;
  ldr r1,[sp,#152]; orrs; lsls; bne`
  → `ldrd` fuses `144+148`.

Slot identification (from the stores):
`12e0e8 str r0,[sp,#N]` is `turretMode = isInTurretMode()`, and
`12e536/12e550 str r6,[sp,#N]` (r6=0) is `planetHit = false`:

| local | original slot | ours slot |
|---|---:|---:|
| `otherLock` (int) | 144 | 148 |
| `turretMode` (bool) | 148 | 152 |
| `planetHit` (bool) | 152 | 144 |
| `Globals::status` ptr | 156 | 156 |

So the original's slot order is `otherLock, turretMode, planetHit`; ours is
`planetHit, otherLock, turretMode`. Moving `planetHit`'s declaration above
`turretMode` (ph-before-turret, ph-top) does **not** change clang's slot
assignment — the order is a spill-order artefact, not declaration order.

Same story for the loop index: original `mov.w fp,#0` at `12e0d8` (entry) and
`12e3c8`, index lives in `fp` with `ldr.w r1,[r0,fp,lsl#2]`; ours uses `sl`
(`mov.w sl,#0` at `12e0d8`) and spills `campaign` to `[sp,#144]` instead of
keeping it in `sl` (`12e0f8 mov sl,r0` in the original).

## 3. Campaign block: `marked` is an int in the original

Original `12e67a`:

```
ldr r1,[sp,#132] ; marked
cmp r1,#0
it ne
movne r1,#1      ; normalize to 0/1 for `marked ? 14 : 0`
```

i.e. the original's `marked` is an **int** (needs the 0/1 normalization), not a
`bool`. Declaring `int marked = 0` grows the function to 9,912 B (+4) and
degrades aligned to 1,401, so it is not a win on its own; a compensating −4 B
elsewhere was not found (int-marked+int-ph / +expect1 only grow further).

Also, the original keeps the campaign target-station block **inline** at
`12e474` (`ldr.w r0,[sl,#4]; ldr.w sl,[r0,fp,lsl#2]; mov r0,r6; blx
getTargetStation`), whereas ours is out-of-line at `12e5ca`. That is driven by
the `__builtin_expect(..., 0)` on the campaignMission condition; the original
behaves like an *unlikely-free* (or likely) body. Removing the expect
(C-noexpect) or flipping it to 1 (D-expect1) both grow the function
(9,980 / 9,956), so the expect must stay as-is for the size gate.

## 4. EMP block: canvas is re-dereferenced per call in the original

Original `12f2e6`/`12f2f2`/`12f34a`: `ldr r5,[sp,#140]` (address of
`Globals::Canvas`), then `ldr r0,[r5,#0]` **before each** `DrawImage2D` /
`DrawRegion2D`. Ours keeps the canvas pointer in `fp` and hoists
`ldr.w r6,[fp]` before `getEmpDamageRate()`, adding a `mov r0,r6`. Removing the
`empCanvas` local (emp-direct) makes it re-dereference but grows the function to
9,912 B (+4, `.ARM.exidx` 25) because the earlier `DrawImage2D` then also
re-loads.

Original hull rate also loads `this->field_0x1c8` **after** the
`getDamageRate()` call (`vldr s2,[r4,#456]` at `12f278`); ours hoists it
(`vldr s0,[r4,#456]` at `12f238`, kept in s24). A `rate` temporary fixes the
ordering but costs 57 aligned instructions (hull-temp: 6,999/1,360).

## 5. Conclusion / next angle

The EMP + campaign/marked residuals are downstream of the function-wide
register allocation (`sl` vs `fp`, spill slots 144/148/152). Local rewrites
either produce identical code or break size. The productive next step is a
whole-function restructure that makes `campaign` and the planet-loop index the
*only* long-lived values competing for `sl`/`fp` (e.g. shrink the live range of
the `stations` local and of `Globals::status` so the allocator picks the
original's assignment), then re-derive the EMP canvas handling from that
allocation. Do not chase the individual instructions.
