# PlayerEgo::calcCollision — wave 108 (w5614): shape sweep, no new best

**Status:** research note, 2026-11-04. Worktree `wt/w5614`, branch
`wave108-w5614`, base `8f98ec4d6`. **No source change landed** — the
checked-in 87.6 % / 968 B shape is still the best measured. This note
adds the wave-108 measurements (structural variants that *do* reproduce
the original's register file and un-rotated latch, and the `canvas`
local lever) to the wave-107 fixed-point note
(`playerego-calccollision-wave107-w5513.md`).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed two-anchor body still scores **87.6436781609 %** exactly
  (968 B, 347/349 normalised instructions).  Nothing measured this wave
  beats it; the best *new* variant is **85.6733524355 %** (984 B).
* Two variants reproduce the original's **register file**
  (`this=r5, dock=r9, surface=r6, i=fp`) **and** the original's
  **un-rotated latch** (end of body, `loc_2c0`-like), but pay 8–16 B for
  two always-false `objects->size()` checks:

  | variant | pct | size | latch | this | dock | surface | i |
  |---|---:|---:|---|---|---|---|---|
  | landed (A1+A2, guard w/o `i==0`) | **87.6437** | 968 | rotated 0xc6 | r5 | r9 | r6 | fp |
  | `le/le` + real guard | 85.6734 | 984 | end 0x2ce | r5 | r9 | r6 | fp |
  | `le/le` + old guard | 85.4676 | 976 | end 0x2c8 | r5 | r9 | r6 | fp |
  | `ge` anchor + real guard | 83.4783 | 968 | end 0x2c0 | r5 | r9 | r6 | **sl** |
  | `gt-1` + A2 + real guard | 84.9785 | 976 | rotated 0xc6 | r5 | r9 | r6 | fp |

* The `le/le` family is the closest *faithful* shape found so far: its
  only instruction differences from the original are the two redundant
  size checks, the missing preheader `str.w r9,[sp,#20]` (dock spill at
  def), and the collision-arm shuffle.  It is 8–16 B too long, so the
  fuzzy metric prefers the rotated-latch landed body.
* The `PaintCanvas *canvas = Globals::Canvas;` local lever reported in
  wave 107 is real but only with the A1+A2 anchor pair; in the variants
  tested here it is optimised away (identical codegen).

## 1. Exact scores (unrounded)

`tools/verify/verify.py` rounds `pct` to one decimal.  Re-running the
same `disassemble`/`reconcile`/`SequenceMatcher` path unrounded gives:

```
landed                           pct=87.6436781609  size=968  lens=347/349
le/le + real guard               pct=85.6733524355  size=984  lens=347/351
le/le + old guard                pct=85.4676258993  size=976  lens=347/348
le/eq + real guard               pct=85.6733524355  size=984  lens=347/351
ge anchor + real guard           pct=83.4782608696  size=968  lens=347/343
gt-1 + A2 + real guard           pct=84.9785407725  size=976  lens=347/352
natural (no anchors, real guard) pct=59.7997138770  size=996  lens=347/352
natural no braces, real guard    pct=51.4935988620  size=996  lens=347/356
```

## 2. The `le/le` variant (best faithful shape, 85.67 %)

`A1` after `outerCollide`, `A2` after the `isShrinking()/miningGame`
check, real bottom guard:

```cpp
void PlayerEgo::calcCollision(Array<KIPlayer *> *objects) {
    if (objects == nullptr)
        return;
    if (this->dockedFlag && (unsigned int) (this->dockingPointIndex - 1) < 3)
        return;
    for (unsigned int i = 0; i < objects->size(); i++) {
        KIPlayer *object = (*objects)[i];
        if (object == nullptr)
            continue;
        if (i == 0 && !Globals::status->inAlienOrbit()
            && AbyssEngine::AEMath::VectorLength(this->dockOffsetVec) < 16000.0f
            && object->field_0x71)
            this->collidesWithStationFlag = 1;
        {
            Vector surface = this->geometry->getPosition();
            if (!object->outerCollide(surface))
                continue;
        }
        if (objects->size() <= i)          // A1, always false
            continue;
        if (object->getType() == 16994 && object->isVisible()) {
            if (((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0)
                continue;
            if (objects->size() <= i)      // A2, always false
                continue;
            Vector &pull = this->moveTarget;
            /* ... unchanged wormhole / steal arms ... */
        }
        /* ... unchanged steal / collision arms ... */
        if (i == 0 && object->field_0x71)  // real bottom guard
            this->collidesWithStationFlag = 1;
        this->hitCamera();
    }
}
```

Exact normalised diff vs the original: every branch target is
`loc_2ce` instead of `loc_2c0` (a 14-byte shift), the two checks add
`ldr r0,[r4]; cmp r0,fp; bls.w` twice (12 B), the preheader dock spill
`str.w r9,[sp,#20]` is missing, and the collision arm keeps the
`add.w sl,sp,#96` form instead of the original's
`mov sl,r5; mov r5,r6; add r6,sp,#96; ...` shuffle.  The register file
and the un-rotated latch match exactly.

Removing either check loses the register file (`le/none` → `dock=fp,
i=r9`; `none/eq` → `dock=r0, i=fp`), which is why the landed body keeps
both.

## 3. The `ge` anchor: right layout, `i` in `sl`

`if (objects->size() >= i) continue;` after `outerCollide` + the real
bottom guard gives the original's un-rotated latch at `0x2c0`, the
original's `this=r5, dock=r9, surface=r6`, and the original's frame
(112) — but `i=sl` instead of `fp`.  That single register swap cascades
into the whole wormhole arm (`vmov fp,s0` vs `vmov sl,s0`,
`add.w r9,sp,#96` vs `add.w sl,sp,#96`) and the collision arm, costing
~4 % fuzzy score.  Equivalent spellings (`i <= objects->size()`,
`objects->size() >= (unsigned)i`, `i <= objects->size() + 0`, ...) are
codegen-identical.

## 4. The `canvas` local lever

`PaintCanvas *canvas = Globals::Canvas;` before the matrix assignment in
the wormhole arm flips the `dock`/`i` pair in the A1+A2 shapes:

| shape | pct | size | this | dock | i | latch |
|---|---:|---:|---|---|---|---|
| A1+A2, no guard, no canvas (wave-107) | 87.283 | 968 | r5 | fp | r9 | end |
| A1+A2, no guard, + canvas | 83.954 | 968 | r5 | r9 | fp | rotated 0xc6 |
| `ge` + real guard, + canvas | 81.214 | 968 | r5 | sl | fp | end 0x2c0 |
| `ge` + real guard, no canvas | 83.478 | 968 | r5 | r9 | sl | end 0x2c0 |

In the `le/le` shapes the local is optimised away (identical codegen),
so it does not help the faithful variant.

## 5. Other measured variants (all ≤ landed)

| variant | pct | size | note |
|---|---:|---:|---|
| `gt-1` + A2 + real guard | 84.9785 | 976 | rotated latch, right pair |
| `le/none` + real guard | 79.4245 | 976 | dock=fp, i=r9 |
| `none/eq` + real guard | 70.6724 | 988 | dock=r0 |
| `le/eq` + old guard | 85.4676 | 976 | same as `le/le` |
| `ge/eq` + real guard | 74.1703 | 976 | dock=fp, i=sl |
| `gt-1/none` + real guard | 82.5899 | 968 | dock=fp, i=r9 |
| `ge/none` + old guard | 87.2827 | 968 | below landed by 1 insn |
| natural, no braces, real guard | 51.4936 | 996 | frame 120 |
| `while` loop | 65.1363 | 1000 | dock=r0 |
| `pos` hoists, `int`/`size_t` i, `const`/`auto` object, `Vector()` ctor, `!=`/`==` operand order, `>>8` vs `/256`, nested guards | 87.6437 | 968 | codegen-identical to landed |
| `impact_zero` (`zero != impact`) | 87.0690 | 968 | 1 insn lost |
| `obj_ref` (`KIPlayer *&object`) | 47.4432 | 976 | frame 112, dock=r0 |

## 6. Proven vs. hypothesis

**Proven (measured):**

* The landed body is still the best at 87.6436781609 % / 968 B.
* The original's register file + un-rotated latch is reproducible with
  the `le/le` pair of always-false size checks (85.6734 %, 984 B) or
  with the `ge` anchor (83.4783 %, 968 B, `i=sl`).
* The `le/le` variant's residual diff is only: the two redundant size
  checks, the preheader dock spill, and the collision-arm shuffle.
* `canvas` only moves the pair in the A1+A2 shapes; it is dead in the
  `le/le` family.

**Hypothesis (not proven):**

* The original source is the natural/REA shape compiled with the
  `&__stack_chk_guard` pointer spilled at its def (freeing `r6` for
  `surface` and keeping `this=r5`).  The wave-107 note already isolates
  this as the single remaining allocator decision.
* A source construct that raises prologue register pressure *without*
  adding a basic block (e.g. an address-taken local whose lifetime ends
  before the loop) is still the most promising untried lever.  It would
  need to reproduce the dock-spill-at-def and the collision shuffle
  while keeping the un-rotated latch, i.e. the `le/le` shape minus its
  two checks.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5614
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
# per-variant: splice body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5614/score.py                      # exact unrounded score
python3 /dev/shm/w5614/analyze.py                    # register file / latch
python3 /dev/shm/w5614/vdiff.py /dev/shm/w5614/orig.dis /dev/shm/w5614/lele.dis
```

Scratch harness (`/dev/shm/w5614`): `score.py` (exact single-function
score), `analyze.py` (register file / latch offset), `vdiff.py`
(normalised diff), `exp.py`/`variant.py` (splice + build), `rand2.py`
(random anchor-position sweep), `ge.dis`/`lele.dis`/`base.dis`
(disassembly dumps), `ge_all.txt` (`-mllvm -print-machineinstrs`).

Gate at hand-off: `RATCHET PASS`, byte 4500, linked 4500, size 4524,
data 843/843, imports 207, lint CLEAN, drift 0.
