# PlayerEgo::calcCollision — wave 109 (w5711): layout/pair sweep, no new best

**Status:** research note, 2026-11-04. Worktree `wt/w5711`, branch
`wave109-w5711`, base `7c34c4098`. **No source change landed** — the
checked-in 87.6 % / 968 B shape is still the best measured. This note
adds the wave-109 measurements to the wave-107/108 fixed-point notes
(`playerego-calccollision-wave107-w5513.md`,
`playerego-calccollision-wave108-w5614.md`).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed two-anchor body still scores **87.6436781609 %** exactly
  (968 B, 347/349 normalised instructions).  Nothing measured this wave
  beats it.
* The landed body's **register file is already exactly the original's**:
  `this=r5, objects=r4, object=r8, dock=r9, i=fp, surface=r6`; the
  prologue is byte-identical through `0x9f12e`.  The remaining diff is
  the missing preheader `str.w r9,[sp,#20]`, the A1/A2 anchor blocks,
  the rotated latch (`0x9f192` vs `0x9f38c`), the collision-arm
  register shuffle, the missing bottom guard, and the consequent
  branch-target/literal-pool-offset shifts.
* The un-rotated layout (latch `0x2c0`) is reproducible at 968 B with
  `ge`/`wrap`/`hit` anchors, but every such shape trades the
  `dock`/`i` pair (`dock=fp,i=r9` or `dock=r9,i=sl` or `dock=sl,i=fp`).
  `le/le` gets both the layout and the pair but is 984 B.
* ~450 new source variants this wave (hoists, surface scopes, check
  forms/positions, cold-path checks, condition permutations, random
  statement reorders) produce nothing above 87.6437 %.

## 1. Exact scores (unrounded)

`tools/verify/verify.py` rounds `pct` to one decimal.  Re-running the
same `disassemble`/`reconcile`/`SequenceMatcher` path unrounded gives:

```
landed (A1+A2, guard w/o i==0)    pct=87.6436781609  size=968  lens=347/349
wrap (single i<size wrap)         pct=87.2832369942  size=968  lens=347/345
ge anchor + real guard            pct=83.4782608696  size=968  lens=347/343
le/le + real guard                pct=85.6733524355  size=984  lens=347/351
landed + real bottom guard        pct=83.9260312945  size=988  lens=347/356
```

## 2. The landed body is register-identical to the original

`-mllvm -print-after-all` (`VirtRegRewriter`) for the landed body gives

```
16B  renamable $r4  = COPY $r1      ; objects
32B  renamable $r5  = COPY $r0      ; this
384B renamable $r9  = t2ADDri $r5, 328   ; dock
544B renamable $r6  = t2ADDri %stack.8   ; surface
720B renamable $r11 = t2MOVi 0       ; i
864B renamable $r8  = t2LDSs ...     ; object
```

which is exactly the original's file.  The prologue bytes
(`0x9f0cc..0x9f12e`) are identical to the original.  The wave-107
hypothesis that the remaining gap is a single allocator decision is
therefore confirmed for the landed shape: the only prologue difference
is the absent `str.w r9,[sp,#20]` at `0x9f12a` (the dock address is
spilled lazily at `0x9f1ce` instead of at its def).

## 3. Un-rotated-layout variants measured this wave

All 968 B unless noted.  `dock/i` is the pair the wave-107/108 notes
identify as the remaining attractor.

| variant | pct | latch | this | dock | i | surface | note |
|---|---:|---|---|---|---|---|---|
| **landed (A1+A2)** | **87.6437** | `0x192` rotated | r5 | r9 | fp | r6 | best |
| wrap (`if (i<size)` around arms) | 87.2832 | `0x2c0` | r5 | fp | r9 | r6 | right layout, pair swapped |
| wrap + real `i==0` guard | 87.2832 | `0x2c0` | r5 | fp | r9 | r6 | same size |
| `ge` (`size>=i`) + real guard | 83.4783 | `0x2c0` | r5 | r9 | sl | r6 | right layout, `i` wrong |
| `le/le` + real guard | 85.6734 | `0x2ce` | r5 | r9 | fp | r6 | right layout+pair, 984 B |
| `int hit = outerCollide(...)` | 64.0805 | `0x2c0` | r5 | sl | fp | r6 | frame 120, object=r9 |
| nested `if (outerCollide)` | 64.0805 | `0x2c0` | r5 | sl | fp | r6 | frame 120 |
| `T2/gt1` (`size>i` + `i>size-1`) | 87.6437 | `0x194` rotated | r5 | r9 | fp | r6 | codegen-equivalent |
| `gt/gt1` (`i>size` + `i>size-1`) | 87.6437 | `0x194` rotated | r5 | r9 | fp | r6 | codegen-equivalent |

`T2/gt1` and `gt/gt1` are new this wave: they are codegen-equivalent
to the landed body (same score, same register file, same 968 B) but
with a slightly different latch (`0x9f190` rather than `0x9f192`).

## 4. What was tried and did not move the needle

* **Hoisted locals** (`dock`/`pull`/`drift` refs and pointers,
  `AEGeometry*`, `Player*`, `Status*`, `FModSound*`, `PaintCanvas*`,
  `unsigned n = objects->size()`, `AbyssEngine::AEMath::VectorLength(
  dockOffsetVec)` as a discarded preheader call).  On the landed base
  every unused local is DCE'd (identical codegen); the used ones
  (`geo`, `status`, `sound`, `canvas`, `player`) lose the register file
  and score 73–82 %.
* **Surface scope** — block-scoped (current), loop-scoped, declared
  before `object`, function-scoped, `const Vector&`, direct temporary
  `outerCollide(getPosition())`, `int hit = outerCollide(...)`.  Only
  the block-scoped form keeps the landed register file.
* **Check forms/positions** — `ge/le/lt/eq/gt/T/T2/m1/m2/ge0/lt1/eq0`
  at `after_outer`, `after_shrink`, `before_steal`, `before_vis`,
  `before_coll`, `null`, `top`, `type`, plus cold-path positions
  (`strength<1` stop path, `play` path, `updateEvent3DAttributes`
  path).  No form/position beats the landed pair at 968 B.
* **Condition permutations** of the `i==0 && !inAlienOrbit() &&
  VectorLength(dockOffsetVec)<16000 && field_0x71` chain (all 24).
  The original order is the best; every other order loses the pair
  and/or the layout.
* **Combined conditions** (`stealFlagByte && i<size`,
  `!isVisible() || i>=size`, `i<size && collision!=0`,
  `type==16994 && visible && i<size`), hoisted `bool inRange`, `int i`
  vs `unsigned i`, `KIPlayer*& object`, `if (obj==null) continue;` on
  one line, `hitCamera()` position, `Vector` ctor forms.
* **Random statement reorders** of the wormhole-arm prologue
  (`pull=getPosition; pull-=getPosition; distance; strength; playing`)
  — 120 samples, only the landed permutation reaches 87.6437 %.

## 5. Proven vs. hypothesis

**Proven (measured):**

* The landed body's register file is exactly the original's
  (`this=r5, objects=r4, object=r8, dock=r9, i=fp, surface=r6`) and its
  prologue is byte-identical through `0x9f12e`.
* The remaining diff is the lazy dock spill, the two anchor blocks,
  the rotated latch, the collision-arm shuffle, the missing bottom
  guard, and the resulting branch/pool offsets.
* The un-rotated latch and the `dock=r9,i=fp` pair are each reachable
  at 968 B, but not together: `le/le` gets both only at 984 B.
* No source-level construct tried this wave forces the preheader dock
  spill without also changing the block layout.

**Hypothesis (not proven):**

* The original source is the natural (REA/oracle) shape compiled with
  `&__stack_chk_guard` spilled at its def and the dock address spilled
  at its def.  That single RA decision frees `r6` for `surface`,
  keeps `this` in `r5`, and lets `r9` be reused for `strength`/`pull`
  in the wormhole arm — i.e. the original's exact code.  The landed
  body reproduces it by raising pressure with the two anchor blocks,
  at the cost of the rotated latch.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5711
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
# per-variant: splice body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5711/score.py                      # exact unrounded score
python3 /dev/shm/w5711/analyze.py                    # register file / latch
python3 /dev/shm/w5711/dump.py _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w5711/ours.dis
```

Scratch harness (`/dev/shm/w5711`): `try.py` (splice+build+score),
`score.py` (exact unrounded score / normalised diff), `analyze.py`
(register file), `latch.py`, `dump.py`, `intervals.py`,
`randsearch.py`, `rand2.py`, `v2.py`…`v24.py` (variant generators),
`body_*.txt` (bodies).

## 7. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0.
`RATCHET PASS`.  No source change, so `tools/verify_baseline.json` is
untouched.
