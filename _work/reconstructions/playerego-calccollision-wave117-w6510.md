# PlayerEgo::calcCollision — wave 117 (w6510): a 89.4 % shape exists but is
# gate-breaking (976 B); 968 B ceiling re-confirmed

**Status:** research note, 2026-11-05. Worktree `wt/w6510`, branch
`wave117-w6510`, base `c22678d64`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B body (the "landed" body: nested
`Vector surface` block, `A1` = `if (i > objects->size() - 1) continue;`,
`A2` = `if (objects->size() == i) continue;`, no `i == 0` in the visible
arm) is still the best **gate-safe** measurement. This note extends
`playerego-calccollision-wave114-w6211.md` … `-wave116-w6410.md`.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* **New: a 89.4000000000 % / 976 B shape ("v2") exists.** It is the
  landed body with three edits:
  1. delete `A1` from its landed position,
  2. insert `if (objects->size() == 0) continue;` in the visible arm,
     after `if (!object->isVisible()) continue;` and before
     `Vector collision = object->projectCollisionOnSurface(...)`,
  3. spell the visible-arm compare `if (collision != Vector{0,0,0})`
     (operand swap; `A2` stays in the wormhole arm).

  v2 keeps the **original register file** (`r4`=objects, `r5`=this,
  `r6`=surface, `r8`=object, `r9`=dock, `sl`=distance, `fp`=i), the
  **original frame (112 B)** and the **original block order** (latch at
  `0x9f394`/`0x9f38c`, i.e. last, with the cold wormhole blocks after
  it). Its only deviation is the 6-byte `size()==0` guard itself.
* **But v2 cannot land**: it is 976 B, and the `check` gate collapses
  `byte_exact 4501 -> 2109` (`.text` 1259464 -> 1259496, `.rodata`
  269159 -> 269351, `.ARM.exidx` 28528 -> 28560). `linked_exact` stays
  4501, everything else unchanged. `RATCHET FAIL: byte_exact 4501 ->
  2109`. The `.text` size is a **step function**: any calcCollision size
  in 969…984 B yields `.text = 1259496`; only <= 968 B yields
  `1259464`. So a 976 B function can never pass without a baseline
  update (forbidden).
* ~180 new builds this wave re-confirm the 968 B ceiling:
  **87.6436781609 % (landed) is the best gate-safe point**, M1
  (`A1 = i >= size`, no `A2`) second at 87.3 %.
* Root cause of the 968 B ceiling re-confirmed with the patched LLVM:
  the nested `{ Vector surface … }` block (which is what reproduces the
  original `r6`=surface / frame-112 register file) makes the
  MBP-internal BranchFolder tail-merge two one-instruction `b <latch>`
  tails (`bb.33` = the r10/r6 restore block before the latch, and
  `bb.20` = the wormhole continuation), sets `BFCHANGED=1`, re-runs
  `buildCFGChains()`, and the chain bottom becomes the latch →
  `ROT ROTATING` (996 B, header at the end). The landed body's `A1`/`A2`
  are the only constructs found that keep `BFCHANGED=0`.

## 1. Exact scores (this wave)

All 968 B unless stated; `lens` = orig/ours normalised instructions.

```
landed (A1=gt, A2, nested surface, no i0)                 87.6436781609  968  347/349
M1      (A1=ge, no A2, nested surface, no i0)             87.3000000000  968  347/345
landed, A1 = size-1 < i / !(i <= size-1)                  87.6436781609  968  347/349
landed, A1 = size > i / i < size                          87.4000000000  968  347/349
landed, A1 = i == size                                    87.4000000000  968  347/349
landed, A1 = objects->size() <= i                         85.5000000000  976  347/348
landed, A1 = i + 1 > objects->size()                      84.9000000000  972  347/350
landed, A1 = !(i < objects->size())                       85.5000000000  976  347/348
landed + visible i0 guard                                 85.0000000000  976  347/352
landed + i0, A2 removed                                   82.6000000000  968  347/348
landed, A2 removed                                        79.5000000000  964  347/347
landed, A1 removed                                        70.8000000000  984  347/351
landed, both anchors removed                              59.8000000000  988  347/349
M1 + i0                                                   79.4000000000  976  347/348
M1 + A2 (= L3)                                            85.5000000000  976  347/348
M1, shrink/mining test order swapped                      86.4000000000  968  347/345
M1, type/isVisible order swapped                          64.7000000000  960  347/342
M1, uint distance                                         83.6000000000  972  347/347
M1, uint strength                                         86.7000000000  968  347/345

v2  (no A1, size0 guard before collision, swap, A2)       89.4000000000  976  347/351  <- gate-breaking
v1  (same, no swap)                                       88.5000000000  976  347/349  <- gate-breaking
v2 + i0                                                   89.3000000000  984  347/354  <- gate-breaking
v2 + A1=ge                                                85.6000000000  976  347/349
v2 + A1=gt                                                84.8000000000  980  347/351
v2, guard merged into the collision compare (i<size)      85.5000000000  972  347/348
v2, guard merged into the collision compare (size!=0)     69.8000000000  972  347/355
v2, A2 -> i0                                              59.7000000000  988  347/350
v2, guard at loop top                                     63.0000000000  988  347/351
```

Anchor-free / structural (all with the real visible `i0` guard):

```
loop-scope surface, no anchors                            63.8000000000  968  347/349
  + Vector &drift hoisted to loop scope                   72.7000000000  968  347/349
  + drift hoisted + A1=ge                                 72.4000000000 1004  347/363
  + drift hoisted + A2                                    67.4000000000 1012  347/359
nested surface, no anchors                                59.8000000000  996  347/352
  + drift hoisted                                         49.8000000000  996  347/352
steal-arm `continue` hoisted out of the `if (impact)`     59.8000000000  988  347/349
  (with A1+A2: codegen-identical to landed, 87.6/968)
steal-arm `if (strength < 1) { … continue; }` early form  59.5000000000  988  347/349
steal-arm `else if (playing)`                             59.8000000000  988  347/349
loop `while (i < size) { … i++; }`                        42.8000000000  940  347/340
loop `do { … } while`                                     29.9000000000  952  347/343
```

## 2. v2 in detail

```c++
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
        if (object->getType() == 16994 && object->isVisible()) {
            if (((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0)
                continue;
            if (objects->size() == i)          // A2 (kept)
                continue;
            ... wormhole arm unchanged ...
            continue;
        }
        ... steal arm unchanged ...
        if (!object->isVisible())
            continue;
        if (objects->size() == 0)              // NEW guard (6 B)
            continue;
        Vector collision = object->projectCollisionOnSurface(this->geometry->getPosition());
        if (collision != Vector{0.0f, 0.0f, 0.0f}) {   // swapped operands
            ...
            if (object->field_0x71)
                this->collidesWithStationFlag = 1;
        }
        this->hitCamera();
    }
}
```

Codegen of the guard (`0x9f308`):

```
9f308: 6820    ldr   r0, [r4, #0]        ; objects->size()
9f30a: 2800    cmp   r0, #0
9f30c: d042    beq.n 9f394               ; -> latch
```

Everything else in v2 is the original instruction stream shifted by 8 B;
the null-check `beq.w` target is `0x2c8` (original `0x2c0`), i.e. the
latch is last, and the register file is the original one. This is the
closest shape found so far to the real source.

## 3. Why the gate rejects it

`check` with v2 spliced in:

```
verify: byte 2109  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4501 -> 2109
RATCHET FAIL
```

Section sizes (orig -> v2):

```
.text       1259464 -> 1259496  (+32)
.rodata      269159 ->  269351  (+192)
.ARM.exidx    28528 ->   28560  (+32)
```

The `.text` growth is a step function of the calcCollision size: the
whole PlayerEgo TU is padded, so 969…984 B all give `.text = 1259496`
(checked at 972, 976, 980, 984 B), while <= 968 B gives `1259464`.
`byte_exact` collapses because every later function's literal-pool
addresses shift. Only a **968 B** calcCollision can land.

## 4. Mechanism re-confirmed (patched LLVM MBP trace)

* Landed / M1 / L3: `BFCHANGED=0`, `ROT no-backedge bail`
  (`bottom=bb.22`, whose successor is the latch, not the header).
* Anchor-free shapes: `BFCHANGED=1` → second `buildCFGChains()` →
  `ROT ROTATING` (header moved to the end, 988–996 B).
* v2: `BFCHANGED=0`, `ROT no-backedge bail` (`bottom=bb.21`) — the
  guard changes the IR enough that the BranchFolder tail merge that
  fires on the anchor-free nested-surface shape no longer fires.
* The tail merge (plain body, `-debug-only=branch-folder`):

```
TryTailMergeBlocks: bb.5, bb.11, bb.14, bb.15, bb.21, bb.22, bb.24,
                    bb.25, bb.28, bb.33, bb.34, bb.39, bb.20
  with successor bb.40 which has fall-through from bb.39
Common tail length of bb.33 and bb.20 is 1
Splitting bb.33, size 1
Using common tail in bb.42 for bb.20
```

  i.e. the wormhole continuation (`bb.20`, ir-block.127) and the
  r10/r6 restore block before the latch (`bb.33`) share the trailing
  `b <latch>`; merging them makes `bb.20` branch to a new block instead
  of the latch, so the second placement's chain bottom becomes the latch
  and the loop rotates.

## 5. Dead ends measured this wave

* `i0` guard spellings: `i == 0 && …`, `… && i == 0`, nested `if`,
  `i < 1`, `i == 0u`, `i == 0 && objects->size() != 0 && …` — all
  codegen-identical to each other; none prevents the rotation.
* Brace-wrapping: visible arm, steal arm, i0 check, empty block — no
  change (996 B rotated).
* `if (object != nullptr) { … }` instead of `continue` — 996 B rotated.
* `if (!object->isVisible() || objects->size() == 0) continue;` and
  `if (objects->size() == 0 || !object->isVisible()) continue;` — 980 B,
  76.5 % (M1 base).
* `if (objects->getType() == 16994 && objects->size() != 0 && …)` —
  1008 B, 69.3 %.
* `objects->size()` in the loop condition
  (`objects->size() != 0 && i < objects->size()`) — codegen-identical.
* `objects->size()` at the loop top / before the null check —
  codegen-identical.
* A shared `unsigned int n = objects->size();` used by both `A1` and
  `A2` — codegen-identical to M1 when `A2` is absent (87.3/968);
  65.5/952 when `A2` is present.
* `(*objects)[i]` / `objects == nullptr` as a wormhole-arm anchor —
  85.3/980 and 87.3/968.
* `A2` moved to the steal arm — 81.6/968; to the visible arm —
  72.0/984; to the shrink/mining test — codegen-identical to landed.
* `Vector &drift` hoisted (loop scope) on the landed body — 71.8/968;
  on the nested anchor-free body — 49.8/996.
* `int`/`unsigned`/`short`/`long`/`size_t`/`float` `distance` — best
  87.3/968 (short/long identical to M1).
* `uint strength` — 86.7/968.

## 6. Conclusion / recommendation

The original source almost certainly looks like **v2 minus the
`objects->size() == 0` guard** (i.e. no redundant checks at all, the
real visible-arm `i == 0 && object->field_0x71` guard, and the
`collision != Vector{…}` operand order). That shape is the anchor-free
nested-surface body, which clang 7.0.2 rotates to 996 B because of the
BranchFolder tail merge. Every construct tried that keeps
`BFCHANGED=0` either costs bytes (v2: +8 B) or permutes the register
file (M1: `r9`/`fp` swap).

A future pass should look for a **zero-byte** construct that keeps the
nested `surface` lifetime (original `r6`/frame-112 RA) but breaks the
`bb.33`/`bb.20` one-instruction tail merge — that is the only remaining
lever between 87.6 % and ~100 %.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6510
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice the body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6510/score.py                         # pct/size/lens
python3 /dev/shm/w6510/score.py -d                      # normalised diff
```

Scratch harness (`/dev/shm/w6510`): `score.py`, `splice.py`, `build.sh`,
`mir.sh`, `sweep*.py`, body files (`body_landed.txt`, `body_m1.txt`,
`body_v2.txt`, `body_plain.txt`, `body_af_drift.txt`, …). Patched LLVM
`/tmp/llvm_android_build/bin/llc` with `MBP_TRACE=1` and
`-debug-only=branch-folder` (must pass `-mattr=+neon,+thumb-mode`).

## 8. Gate

Unchanged: `byte_exact 4501 / linked_exact 4501 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B. No source change, so
`tools/verify_baseline.json` is untouched.
