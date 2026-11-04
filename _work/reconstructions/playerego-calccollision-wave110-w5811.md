# PlayerEgo::calcCollision — wave 110 (w5811): the `ge` fixed point, no new best

**Status:** research note, 2026-11-04. Worktree `wt/w5811`, branch
`wave110-w5811`, base `390f58052`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B shape is still the best measured.
This note extends the wave-107/108/109 fixed-point notes
(`playerego-calccollision-wave107-w5513.md`,
`-wave108-w5614.md`, `-wave109-w5711.md`).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed two-anchor body still scores **87.6436781609 %** exactly
  (968 B, 347/349 normalised instructions).  ~200 new builds this wave
  (single-check form/position sweeps, the full 8×8 A1×A2 cross product
  with and without the real bottom guard, hoists, loop forms, surface
  forms, `distance`/`strength` forms) produce nothing above it.
* **New measured fixed point:** a *single* always-true check
  `if (objects->size() >= i) continue;` after `outerCollide` plus the
  real bottom guard `if (i == 0 && object->field_0x71)` reproduces the
  original's **exact prologue** — `this=r5`, `surface=r6`,
  `&__stack_chk_guard` spilled **at def** to `[sp,#24]`,
  `&dockOffsetVec` spilled **at def** to `[sp,#20]` in the preheader
  (`str20` offset `0x5e`) — and the original's **un-rotated latch**
  (`0x2c0`, header `0x9f134`).  Its only remaining register difference
  is the `i`/scratch pair: **`i=sl`, `fp` scratch** instead of the
  original's `i=fp`, `sl` scratch.  Score 83.4783 %, 968 B, 343 insns.
* The full A1×A2 cross product confirms the landed pair
  (`i > size-1` / `size == i`, no real guard) is the unique 87.6437 %
  point; every other pair is 84.98–87.36 % (or worse).
* The `le/le` + real guard shape (85.6734 %, 984 B) is still the best
  *faithful* shape (original layout + original `this/dock/surface/i`
  register file), but pays 16 B for the two always-false checks.

## 1. Exact scores (unrounded)

`tools/verify/verify.py` rounds `pct` to one decimal; re-running the
same `disassemble`/`reconcile`/`SequenceMatcher` path unrounded gives:

```
landed (A1+A2, guard w/o i==0)     pct=87.6436781609  size=968  lens=347/349
le/le + real guard                 pct=85.6733524355  size=984  lens=347/351
eq/le + real guard                 pct=85.2224184741  size=972  lens=347/347
landed + real bottom guard         pct=84.9785407725  size=976  lens=347/353
ge + real guard                    pct=83.4782608696  size=968  lens=347/343
ge + real guard, no i==0 in guard  pct=87.2832369942  size=968  lens=347/345
natural (no checks, real guard)    pct=59.7997138770  size=996  lens=347/352
```

## 2. The `ge` fixed point (new this wave)

Body (`A1` is the only extra statement; bottom guard is the real one):

```cpp
        {
            Vector surface = this->geometry->getPosition();
            if (!object->outerCollide(surface))
                continue;
        }
        if (objects->size() >= i)          // always true; not provable
            continue;
        if (object->getType() == 16994 && object->isVisible()) { ... }
        ...
        if (i == 0 && object->field_0x71)  // real bottom guard
            this->collidesWithStationFlag = 1;
```

Prologue (identical to the original through `0x9f132`):

```
9f0da: mov  r5, r0
9f0e4: ldr  r0, [r0, #0]      ; &__stack_chk_guard
9f0e6: str  r0, [sp, #24]     ; guard spilled AT DEF  (original)
9f0e8: ldr  r0, [r0, #0]      ; canary
9f0ea: str  r0, [sp, #108]
...
9f110: add.w r9, r5, #328     ; dock
9f118: add  r6, sp, #32       ; surface
9f12a: str.w r9, [sp, #20]    ; dock spilled AT DEF   (original)
```

and the loop latch is at `0x2c0` (bottom, un-rotated) exactly like the
original.  The only diff is `mov.w sl,#0` for `i` (original
`mov.w fp,#0`) and the `fp`/`sl` swap that cascades through the
wormhole arm (`vmov fp,s0` / `add.w r9,sp,#96` / Canvas in `fp`).

### Why the latch is un-rotated here but rotated in the landed body

The two anchor families differ in the **direction of the always-taken
branch**:

* landed A1 `if (i > objects->size() - 1) continue;` is always
  **false** — the fall-through is hot, and MachineBlockPlacement places
  the latch early (`0xc6`, rotated).
* `ge` `if (objects->size() >= i) continue;` is always **true** — the
  `continue` edge is hot, the latch is placed last (`0x2c0`,
  un-rotated), matching the original.

This is a second, independent rotation lever from the wave-101
`r9`-restore-block cause: the check's branch direction alone decides
the rotation, and only the always-true form keeps the original layout.

### Why `i` lands in `sl`

In the `ge` shape the loop variable `i` gets `r10` (sl) and the
short-lived `distance` value is spilled to `[sp,#4]`; in the original
`i` is in `r11` (fp) and `distance` lives in `sl`.  The `ge` shape
therefore has **7** stack slots (`fi#16..22`) where the original/landed
have **6**.  Both allocations are valid; the difference is which of
`{i, distance}` gets the last callee-saved register.  Every attempt to
move `i` to `fp` while keeping the `ge` layout changed something else:

| variant (ge + real guard) | pct | size | this | dock | surf | i | latch | str20 |
|---|---:|---:|---|---|---|---|---|---|
| **ge** | **83.4783** | 968 | r5 | r9 | r6 | **sl** | 0x2c0 | 0x5e |
| `le` (`i <= size`) | 83.4783 | 968 | r5 | r9 | r6 | sl | 0x2c0 | 0x5e |
| `int i` / `size_t i` | 83.4783 | 968 | r5 | r9 | r6 | sl | 0x2c0 | 0x5e |
| `ge` + hoisted `FModSound*` | 51.8950 | 956 | r5 | r9 | r6 | **fp** | 0x2b6 | 0x102 |
| `ge` + hoisted `unsigned n=size()` | 67.6301 | 976 | r5 | **sl** | — | **fp** | 0x2c6 | — |
| `ge` + hoisted `KIPlayer** data` | 67.6301 | 976 | r5 | **sl** | — | **fp** | 0x2c6 | — |
| `ge` + `Vector& pull` hoist | 71.9884 | 956 | r5 | r0 | r6 | sl | 0x2ba | — |
| `ge` + second `ge` in wormhole arm | 74.1703 | 976 | r5 | **fp** | r6 | sl | 0x2c8 | 0x5e |
| `ge` + `distance` as `unsigned` | 80.0578 | 972 | r5 | r9 | r6 | sl | 0x2c0 | 0x5e |
| `ge` + `surf-first` (surface before top guard) | 55.9541 | 996 | **fp** | r0 | r5 | sl | 0x396 | 0x60 |

`ge` + hoisted `sound` is the only variant that gets `i=fp` with the
right `this/dock/surface`, but it moves `object` to `sl` and changes
the sound reloads (`Globals::sound` is no longer reloaded from
`[sp,#16]`), so it is not the original.

## 3. Full A1×A2 cross product

All 64 pairs of the eight check forms
(`i > size-1`, `size <= i`, `size >= i`, `size == i`, `size != i`,
`i >= size`, `i < size`, `size > i`) at the landed A1/A2 positions,
with and without the real bottom guard.  Best per family:

| family | best pair | pct | size | i | latch | str20 |
|---|---|---:|---:|---|---|---|
| no real guard | `gt1/eq` (landed) | **87.6437** | 968 | fp | 0xc6 | 0x102 |
| no real guard | `eq/gt1`, `gt/gt1`, `ilt/gt1` | 87.6437 | 968 | fp | 0xc4 | 0x102 |
| real guard | `le/le`, `le/ge`, `le/eq`, `le/neq`, … | 85.6734 | 984 | fp | 0x2ce | 0x108 |
| real guard | `eq/le`, `ilt/le`, `gt/le` | 85.2224 | 972 | fp | 0xc4 | 0x100 |
| real guard | `gt1/*` | 84.9785 | 976 | fp | 0xc6 | 0x102 |
| real guard | `ge/*` (except `ge/gt1`) | 74.1703 | 976 | sl | 0x2c8 | 0x5e |

Only `ge` *alone* (a single always-true check) gives the original
prologue + un-rotated latch; adding any second check moves `dock` to
`fp` or `i` to `sl`.

## 4. What else was tried (all ≤ landed)

* Single-check forms at A1: `size>=i`, `size>i`, `size!=i`, `i<size`,
  `i<=size`, `i!=size`, `size-i!=0`, `size-i>0`, `i<size+1`,
  `size+1>i`, `i+1<=size`, `size-1>=i`, `i<=size-1`, `size>=i+1`,
  `size>i-1`, `size>=i+0`, `(unsigned)size>=i`, `size>=i-0`,
  `size>=0x4262u`, `size>=dockingPointIndex`, `size>=miningGame`,
  `object->isVisible()`, `object->getType()==16994`,
  `object->stealFlagByte`, `object->field_0x71`, `this->miningGame`,
  `this->dockedFlag`, `object->isDying()` — 80.4–83.5 %.
* Single-check positions: wormhole arm, steal arm, before `isVisible`,
  before `projectCollisionOnSurface`, after `hitCamera`, top of loop
  (top-of-loop `size>=i` is provably true and deletes the body:
  2 bytes) — all ≤ 83.5 %.
* Loop forms: `while`, `do-while` (with `size()==0` early return),
  predeclared `i`, `++i`, `i != size`, `size_t`/`int`/`unsigned short`
  `i` — 21.7–83.5 %.
* Surface forms: `const`, `auto`, `Vector(...)`, `Vector{...}`,
  default-construct + assign, direct
  `outerCollide(getPosition())`, `bool hit`, `int hit` — all
  codegen-identical to the natural shape (59.7997 %) or worse.
* Hoists: `AEGeometry*`, `Player*`, `FModSound*`, `Status*`,
  `PaintCanvas*`, `Vector& pull`, `Vector& drift`, `unsigned n`,
  `KIPlayer** data`, function-scope `Vector collision`/`impact`,
  pre-loop `Vector velocity`/`position` — 42.5–71.99 %.
* Bottom-guard forms: `i==0`, `!i`, `0==i`, `i==0u`, `i==0 && f71!=0`,
  `f71 && i==0`, nested `if(i==0){if(f71)…}`, guard outside the
  `collision!=0` block — identical codegen within each base.
* Pre-loop `dockOffsetVec` uses (`x!=x`, `y!=y`, `z!=z`,
  `VectorLength<0`) — 78.6–80.1 %, all flip `i` to `sl`.
* `distance`/`strength` forms: `unsigned distance`, `long distance`,
  `strength` first — identical or worse.

## 5. Proven vs. hypothesis

**Proven (measured):**

* The landed body's exact score is 87.6436781609 % (347 vs 349
  normalised instructions, 968 B); the full A1×A2 cross product (64
  pairs, ± real guard) finds no better point.
* A single always-true `objects->size() >= i` check after
  `outerCollide` reproduces the original's **entire prologue**
  (`this=r5`, guard@def→`[sp,#24]`, dock@def→`[sp,#20]`,
  `surface=r6`) and the original's **un-rotated latch** (`0x2c0`),
  at the original 968 bytes.  Its only register difference is
  `i=sl`/`fp`-scratch vs the original's `i=fp`/`sl`-scratch.
* The loop rotation is controlled by the **branch direction** of the
  in-body size check: always-false checks (landed) rotate the latch
  (`0xc6`); always-true checks (`ge`) keep it at the bottom
  (`0x2c0`).  This is independent of the wave-101 `r9`-restore-block
  cause.
* The `ge` shape uses 7 stack slots (`fi#16..22`) vs the original's 6;
  the extra slot is the spilled `distance`.  The `i`/`distance`
  register pair is the only remaining allocator decision.

**Hypothesis (not proven):**

* The original source is the natural (REA/oracle) shape compiled with
  the `&__stack_chk_guard` pointer spilled at its def.  The `ge` shape
  proves that a single always-true size check is enough to force that
  spill *and* keep the un-rotated latch; the original presumably had
  no such check, so the same allocator state must have arisen from the
  real bottom guard plus the impact/`distance` live ranges alone.  No
  source spelling found so far reproduces `i=fp` together with the
  `ge` layout.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5811
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
# per-variant: splice body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5811/score.py                      # exact unrounded score
python3 /dev/shm/w5811/analyze.py                    # register file / latch / str20
python3 /dev/shm/w5811/vdiff.py                      # normalised diff vs original
```

Scratch harness (`/dev/shm/w5811`): `try.py` (splice+build+score, ~1.2 s),
`score.py`, `analyze.py`, `vdiff.py`, `dump.py`, `gen.py`
(parameterised body generator), `batch.py` (batch runner),
`exp1.py`…`exp29.py` (the measured sweeps in this note),
`body_ge1.txt` (the `ge` body), `ge1.dis`, `nat.dis`, `landed.dis`,
`orig.dis`.

## 7. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0.
`RATCHET PASS`.  No source change, so `tools/verify_baseline.json` is
untouched.
