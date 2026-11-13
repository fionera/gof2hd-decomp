# `Level::createShip` — wave 191b (w13821): tail dispatch / spawn int-float shape

Worktree `wt/w13821`, branch `wave191b-w13821`, base master `6b680e716`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13821`.

**Status: note-only, no source change landed.**  Every candidate in the
assigned scope (shared `setWreckedMeshId` tail; spawn-block int/float shape)
either misses the 2,764-byte size or measures worse than the current base.
Baseline re-confirmed: raw **1,190** / aligned **366** / size
**2,764/2,764** exact, insns 1,117/1,117.  `.text` contentdiff **76,743**,
`.ARM.extab` contentdiff **625** (size 156,312), `.ARM.exidx` **0**,
`.rodata` **0**, `.rel.plt` byte-identical, `.note.gnu.build-id` 20.
`src/` is clean.

Scope excludes w13804's EmpGuard drop / hull-exit `setBV` reorder /
race-0-box / spawn-coordinate / `empPoints` work.

## 0. Baseline

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1190 aligned=366 size=2764/2764
# .text 76,743; .ARM.extab 625 (size 156,312); .ARM.exidx 0; .rodata 0
```

## 1. Tail dispatch / shared `setWreckedMeshId` — confirmed, but size-fatal

The original has **four** `setWreckedMeshId` call sites:

| VMA | value | owner |
|---|---|---|
| `0xc0946` | `movw r1,#18303` (`0x477f`) | race 1 |
| `0xc09f4` | `movw r1,#18301` (`0x477d`) | race 2 |
| `0xc0a6e` | `movw r1,#18300` (`0x477c`) | race 3 |
| `0xc0b2e` | `movw r1,#18302` (`0x477e`) | race 0, `type != 14` |

The **race-0** case shares one tail: the `type == 14` arm loads
`mov.w r1,#18304` (`0x4780`) at `0xc06ae`, stores the last AAB into
`(*volumes)[10]`, then `b.n 0xc0b2c`; the `else` arm falls into the same
`0xc0b2c` (`mov r0,r9` / `blx setWreckedMeshId`).  So the source shape is a
single `fixed->setWreckedMeshId(meshId)` after the race-0 `if/else`, with
`meshId` a phi (`0x4780` vs `0x477e`).

Our source emits **two** calls (one inline in each race-0 arm), i.e. an extra
`mov r0; blx; b.n` at the end of the else arm (ours `0xc0c0`).  Reproducing
the shared tail is a real fidelity fix but is **size-fatal**: the extra `tbh`
dispatch / `meshId` phi adds instructions.  Measured with the shared tail
alone (base spawn): **2,764 exact but raw 2,007 / aligned 573 / 1,122 insns**
(TD2); other placements give 2,768–2,776.  w13741 §2 independently recorded
2,776 for the same restructure.

## 2. Spawn-block int/float shape — original computes ints before `setEmpData`

Original `0xc03c8..0xc03e4` (immediately after `new Player`, before the
`setEmpData` call at `0xc03ee`) computes **all three coordinates as ints**:

```
fp = [sp,#44] + [sp,#36] - 20000   ; wpx + jitter3 - 20000  -> x
r4 = [sp,#48] + [sp,#32] - 20000   ; wpy + jitter1 - 20000  -> y
r8 = [sp,#40] + [sp,#28] - 20000   ; wpz + jitter2 - 20000  -> z
```

(sp44/48/40 = waypoint x/y/z at `+0x124/0x128/0x12c`; sp36/32/28 = the three
`nextInt(40000)` jitters in source order `jitter3/jitter1/jitter2`.)

The int→float conversions happen **inside each shipClass branch, after the
`new`**:

* fighter (`0xc06c0..0xc06d8`): `vmov s0,fp / s2,r8 / s4,r4` then three
  `vcvt.f32.s32`, passed to `PlayerFighter(...)` (floats).
* fixed (`0xc040c..0xc0422`): `vmov s0,fp / s0,r4 / s0,r8` then
  `vcvt s20/s18/s16` after `new PlayerFixedObject`, reused by every
  `BoundingAAB` (`vmov sl,s20 / r8,s18 / r4,s16`).

Our base source instead mixes types: `z` is a `float` computed before the
guard, `x` is an `int` computed after the guard, `y` is a `float` computed
after the guard — and the formulas are the landed non-faithful ones
(`z=wpx+wpy`, `x=wpz+jitter3`, `y=j1+j2+j3`).  The faithful three-2-add int
shape is **12 B short** (2,752) because the shared-tail `mov r0; blx; b.n`
and the extra `vcvt`/`vmov` setup are absent; adding the shared tail back
hits 2,764 exactly but costs raw (2,007).

## 3. Measured table (all size 2,764 unless noted; `.text`/extab unchanged
unless stated)

Baseline **1,190 / 366**.

### Tail / shared `setWreckedMeshId`

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | **1,190** | **366** | 2,764 | 1,117 |
| TD1: single `setWreckedMeshId(type==14?0x4780:0x477e)` in case 0 | 2,354 | 1,339 | 2,768 | 1,124 |
| TD2: `int meshId` in case-0 block, one call | 2,007 | 573 | 2,764 | 1,122 |
| TD3: `meshId` before switch | build fail | — | — | — |
| G: TD2 + remove `fx` | 2,608 | 1,322 | 2,776 | 1,130 |

### Spawn int/float shape (base formulas, no coordinate change)

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | **1,190** | **366** | 2,764 | 1,117 |
| C: remove `fx`, inline `(float)x` | 1,195 | 367 | 2,764 | 1,117 |
| F: ints before guard + x/y/z floats after | 2,272 | 472 | 2,756 | 1,106 |
| H: volumes created before `fixed` | 2,338 | 503 | 2,772 | 1,122 |
| I: declare pointers then assign | 1,190 | 366 | 2,764 | 1,117 |
| J: `fx` decl before `new` | 1,190 | 366 | 2,764 | 1,117 |
| K: ctor `(float)x` only | 1,190 | 366 | 2,764 | 1,117 |
| L: AAB `(float)x` only | 1,190 | 366 | 2,764 | 1,117 |
| N2: all three floats after guard, base formulas | 1,342 | 399 | 2,764 | 1,117 |
| Q1: ints before guard + TD2 | 2,622 | 1,331 | 2,776 | 1,127 |
| Q2: ints before guard, `fx/fy/fz` after guard | 2,611 | 1,073 | 2,776 | 1,115 |
| Q3: ints before guard, direct `(float)` casts | 2,245 | 517 | 2,752 | 1,115 |
| Q4/R4: only `x` int moved before guard | 1,191 | 366 | 2,764 | 1,117 |
| Q5: only `y` int moved before guard | 2,379 | 453 | 2,768 | 1,118 |
| Q6: `x` before, `y` int after guard | 2,379 | 453 | 2,768 | 1,118 |
| R1: Q4 + remove `fx` | 2,141 | 446 | 2,756 | 1,104 |
| R2: Q4 + `fx=(float)x` after guard | 2,593 | 1,065 | 2,776 | 1,116 |
| R3: Q4 + `float fx=x` after guard | 1,191 | 366 | 2,764 | 1,117 |

### Faithful spawn formulas (spawn-coordinate scope; for reference)

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| S1: faithful formulas, landed structure | 1,199 | 376 | 2,764 | 1,117 |
| B: S1 + remove `fx` | 1,204 | 377 | 2,764 | 1,117 |
| F1: faithful ints before guard | — | — | 2,752 | 1,115 |
| F1TD2: F1 + TD2 shared tail | 2,007 | 573 | 2,764 | 1,122 |
| M1: faithful z only | 2,481 | 604 | 2,744 | 1,096 |
| M2: faithful x,y only | 2,592 | 1,357 | 2,736 | 1,118 |
| M4: faithful x only | 2,331 | 491 | 2,756 | 1,116 |
| M5: faithful y only | 2,596 | 1,352 | 2,736 | 1,113 |
| M6: faithful z,x | 2,462 | 497 | 2,752 | 1,114 |
| M7: faithful z,y | 2,467 | 474 | 2,752 | 1,115 |
| N1: all three floats after guard, faithful | 1,339 | 442 | 2,764 | 1,118 |
| N3: z int before, x int after, y float after, faithful | 1,504 | 476 | 2,764 | 1,118 |
| P1: all ints before guard, order z,x,y | 2,296 | 505 | 2,768 | 1,115 |
| P2: all floats before guard, order z,x,y | 2,524 | 1,071 | 2,772 | 1,116 |
| P3: all ints before + fz/fx/fy floats after | 2,591 | 1,071 | 2,780 | 1,117 |
| S3: all three floats before guard, faithful order z,y,x | 1,216 | 416 | 2,764 | 1,118 |

Older-session rows still valid: P8 1,191/366; P9/P11 1,998/363/2,760;
E* 1,998/363/2,760; G* 1,342/399; Mzxy/Mzyx neutral; Mxzy/Mxyz
2,272/473/2,756; Myzx/Myxz 2,250/471/2,768; H1–H7 2,379/453/2,768;
J1–J3 1,195/367; K* 2,752–2,816; M1/M3/M6 neutral; M5 1,190/368;
O1/O2 neutral; N3 2,478/455/2,780; T1/T3 2,337/465/2,768; T2/T5/T6
neutral.

## 4. Conclusion / root cause

No candidate beats base **1,190 / 366** at size 2,764 exact.  The residual is
a register-rotation artifact, not a source-shape error:

* Original keeps `fixed` in **r9** and `volumes` in **fp**; ours keeps
  `fixed` in **fp** and `volumes` in **r9**.
* Original reloads `this` into **r6** at `0xc0b5e`; ours reloads into **sl**
  at `0xc0b34`.
* Original `tbh` dispatches on **r6** (race) with `type` already in **r4**
  (`0xc0426`); ours dispatches on **r4** with `type` loaded straight to
  **r1** (`0xc042a`).
* Original race-1/2/3 epilogues hoist `type` (`ldr r4,[sp,#52]`) before the
  `b.n`; ours hoist `race` (`ldr r4,[sp,#60]`).

`delta.py` shows the whole function as a narrow ±4/±6/±8-byte shuffle of
address displacements that re-converges at `0xc0bdc` (delta 0).  The
`str.w r9,[sp,#56]` `this` spill sits at `0xc031c` (fixed by the MI scheduler
in `bb.17`, right after `tCMPi8`) whereas the original spills at `0xc0332`
after `add.w sl,r0,#40`; that single 4-byte insertion is what rotates
`fp`/`r9` for the entire hull block.  No source lever in the assigned scope
flips it back while keeping the 2,764-byte size.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13821
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1190 aligned=366 size=2764/2764
```

Scratch helpers (`scratch/w13821`): `measure.sh`, `side.py`, `delta.py`,
`secdiff.py`, `probe_w21k.py` (F1TD1–TD3), `probe_w21l.py` (B,C,F,G,H,I,J,K,L),
`probe_w21m.py` (M1–N3), `probe_w21n.py` (P1–P3), `probe_w21q.py` (Q1–Q6),
`probe_w21r.py` (R1–R4), `Level.cpp.base` (pristine source).

## 6. Files changed

* this note only.  `src/` untouched.
