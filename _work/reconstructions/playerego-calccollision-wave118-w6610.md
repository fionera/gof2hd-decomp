# PlayerEgo::calcCollision — wave 118 (w6610): the 89.4 % v2 shape is 976 B
# and gate-breaking; 968 B ceiling re-confirmed with the BF tail-merge root cause

**Status:** research note, 2026-11-05. Worktree `wt/w6610`, branch
`wave118-w6610`, base `3d4a98a3e`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B body (the "landed" body: nested
`{ Vector surface … }` block, `A1` = `if (i > objects->size() - 1) continue;`,
`A2` = `if (objects->size() == i) continue;`, no `i == 0` in the visible
arm) is still the best gate-safe measurement. This note extends
`playerego-calccollision-wave114-w6211.md` … `-wave117-w6510.md`.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* Baseline re-verified: **87.6436781609 % / 968 B / 347-349** normalised
  instructions; `check` **RATCHET PASS** with byte **4501**, linked
  **4501**, size 4524, data 843/843, imports 207, lint CLEAN, drift 0,
  27 sections, file size 2,187,328 B.
* **The best shape measured is "v2": 89.3982808023 % / 976 B / 347-351.**
  It is the landed body with three edits: delete `A1`, insert
  `if (objects->size() == 0) continue;` in the visible arm (after
  `if (!object->isVisible()) continue;` and before
  `Vector collision = …`), and spell the compare
  `if (collision != Vector{0,0,0})` (operand swap; `A2` stays).
  v2 keeps the **original register file** (`r4`=objects, `r5`=this,
  `r6`=surface, `r8`=object, `r9`=dock, `sl`=distance, `fp`=i), the
  **original frame (112 B)** and the **original block order** (latch
  last at `0x9f38c`-relative, cold wormhole blocks after it).
* v2 cannot land: it is 976 B and the gate's `.text` is a **step
  function** of the calcCollision size. Any size in **969…984 B**
  yields `.text = 1259496` (orig 1259464) and collapses
  `byte_exact 4501 → 2109`. Only **≤ 968 B** gives `.text = 1259464`.
* ~90 new builds this wave re-confirm the 968 B ceiling. **Nothing
  beats 87.6436781609 % at ≤ 968 B.**
* Root cause of the 968 B ceiling re-confirmed and narrowed with the
  patched LLVM: the faithful shape's `Vector surface` must be in a
  **narrow block** to get the original **frame 112** and original
  register file, but that shape makes BranchFolder tail-merge two
  `ldr r6, [sp,#20]` (canary restore) tails, `BFCHANGED=1`, a second
  `buildCFGChains()`, and `ROT ROTATING` (996 B). v2 avoids the merge
  only because its two extra `objects->size()` compares raise register
  pressure enough to spill the canary to `[sp,#24]`, so the steal-arm
  continue block no longer ends with a canary restore.

## 1. Exact scores (this wave)

All 968 B unless stated; `lens` = orig/ours normalised instructions.

```
landed (A1=gt, A2, nested surface, no i0)                 87.6436781609  968  347/349
M1      (A1=ge, no A2, nested surface, no i0)             86.9942196532  968  347/345
landed, compare swapped (collision != zero)               87.3563218391  968  347/349
landed, A2 -> i==size / A1 -> size-1<i / null -> !object  87.6436781609  968  (identical)
landed, isVisible()==0 / Vector{0,0,0} / Vector()         87.6436781609  968  (identical)
landed, A1 removed + bottom i0                            70.6723891273  988  347/352
landed, A2 removed + bottom i0                            82.5899280576  968  347/348
landed, both anchors removed + bottom i0                  59.7997138770  996  347/352
landed + bottom i0 (both anchors kept)                    84.9785407725  976  347/352
landed, A2 removed                                        79.5389048991  964  347/347
landed, A1 moved before isVisible / after isVisible       85.6321839080  976  347/349
landed, A1 moved after null / before top i0               70.4707560628  992  347/354
landed, A2 moved after null / before top i0               79.5389048991  964  347/347
landed, A2 moved into steal arm                           81.6091954023  968  347/349

v2  (no A1, size0 guard before collision, swap, A2)       89.3982808023  976  347/351  <- best, gate-breaking
v2 + bottom i0                                            89.3009985735  984  347/354
v2, A2 -> i>=size                                         89.3982808023  976  347/351
v2, A2 -> i>size-1                                        89.1428571429  980  347/353
v2, A2 removed                                            60.5187319885  980  347/347
v2, size0 removed                                         63.0372492837  988  347/351
v2, A2 removed + i0                                       59.6843615495  988  347/350
v2, size0 removed + i0                                    62.4821683310  996  347/354
v2, both guards removed + i0                              60.0858369099  996  347/352
v2, A2 -> redundant null check                            60.5187319885  980  347/347
v2, A2 -> i==0                                            59.3123209169  988  347/351
v2, A2 -> field_0x71                                      87.4285714286  984  347/353
v2, visible arm wrapped in if (isVisible())               69.2417739628  992  347/352
v2, size0 moved after hitCamera                           62.2857142857  992  347/353
v2 with shared `unsigned n = objects->size()`             68.9855072464  944  347/343
  + bottom i0                                             69.0751445087  952  347/345

faithful (no anchors, bottom i0, nested {surface} block)  60.0858369099  996  347/352
faithful, loop-scope surface                              63.7931034483  968  347/349
faithful, nested `if (object != nullptr)` shape           64.0804597701  968  347/349
faithful, else-chain with shared hitCamera                59.9713055954  992  347/350
faithful, `bool hit` + nested block                       59.6843615495  992  347/350
faithful, `const Vector surface`                          60.0858369099  996  347/352
faithful, `Vector surface(...)` ctor form                 60.0858369099  996  347/352
faithful, steal-arm `continue` hoisted                    60.0858369099  996  347/352
faithful, wormhole strength test inverted                 59.7997138770  996  347/352
faithful, playing test inverted                           60.0858369099  996  347/352
faithful, named `pos` for getPosition in steal arm        49.8567335244  988  347/351
faithful, named `pos2` for getPosition in visible arm     63.1276901004  972  347/350
faithful, `Vector surface;` at function scope             44.6629213483  1028  347/365
```

Anchor-free / structural (all with the real visible `i == 0` guard):

```
loop-scope surface, no anchors                            63.7931034483  968  347/349
nested surface, no anchors                                60.0858369099  996  347/352
nested `if (object != nullptr)` + surface in body         64.0804597701  968  347/349
  + A2                                                    56.2942008487 1012  347/360
  + A1                                                    71.4486638537 1008  347/364
  + A1 + A2                                               56.8627450980 1016  347/367
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
            if (objects->size() == i)          // A2
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

v2's normalised diff against the original is exactly three sites:

1. `A2` — `ldr r0,[r4]; cmp fp,r0; bcs latch` (+3 insns, +6 B),
2. the `size()==0` guard — `ldr r0,[r4]; cmp r0,#0; beq latch`
   (+3 insns, +6 B),
3. the missing visible-arm `i == 0` test — `cmp.w fp,#0; bne`
   (−2 insns, −4 B).

Net **+4 instructions / +8 B**. Everything else is the original
instruction stream shifted by 8 B, with the original register file and
block order.

## 3. Why v2 cannot land

`check` with v2 spliced in:

```
verify: byte 2109  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4501 -> 2109
RATCHET FAIL
```

Section sizes (orig -> v2, measured this wave):

```
.text       1259464 -> 1259496  (+32)
.rodata      269159 ->  269351  (+192)
.ARM.extab   156312 ->  156300  (-12)
```

`.text` is a **step function** of the calcCollision size: 969…984 B
all give `.text = 1259496` (checked at 972, 976, 980, 984 B), while
≤ 968 B gives `1259464`. Only a **968 B** calcCollision can land.

## 4. Mechanism: BF tail-merge of the canary restore rotates the loop

Patched LLVM (`/tmp/llvm_android_build/bin/llc`, `MBP_TRACE=1`,
`-O2 -mtriple=armv7-none-linux-androideabi16 -mattr=+neon,+thumb-mode`).

**Faithful shape** (`Vector surface` in a narrow `{ … }` block, no
anchors, bottom `i == 0` guard; 996 B):

* frame **112**, `r5`=surface, `r8`=object, `r9`=dock, `sl`=this,
  `r6`=canary spilled to `[sp,#20]`, `fp`=i.
* Pre-BF chain: `[bb.4 … bb.40(latch), bb.21, bb.22, bb.19, bb.20]`,
  bottom `bb.20` (wormhole continue) → branches to the latch, **not**
  to the header → `ROT no-backedge bail` (no rotation).
* Then BranchFolder (post-placement, `TailMergeSize = TailDupSize+1 = 2`
  for `-Oz`) strips the trailing `b` from each predecessor and compares
  tails. `bb.33` (steal-arm continue: `mov r10,r6; ldr r6,[sp,#20]`)
  and `bb.20` (wormhole continue: `…; ldr r6,[sp,#20]`) share the
  **canary restore** → `Common tail length … is 1`, `EffectiveTailLen
  = 1 + 1 (stripped branch) = 2 >= 2` → merge, `Splitting bb.33, size 1`,
  `Using common tail … for bb.20`.
* `BFCHANGED=1` → second `buildCFGChains()`; the new chain bottom is
  the latch `bb.37` (successor = header `bb.4`) →
  `ROT ROTATING exit=bb.4` → 996 B, header moved to the end.

**v2** (976 B): the two extra `objects->size()` compares raise register
pressure enough that the canary is spilled to `[sp,#24]` (the original
slot) and the steal-arm continue block no longer ends with a canary
restore → `BFCHANGED=0`, bottom `bb.21` (wormhole continue) →
`ROT no-backedge bail`. Original register file, frame 112, block order.

This is the two-way coupling the previous waves described, now pinned
to a single instruction (`ldr r6,[sp,#20]`): **frame 112 + original RA
requires the narrow surface block; the narrow surface block makes the
canary live in `r6`; the canary restore then tail-merges and rotates
the loop.**

## 5. Dead ends measured this wave (don't redo)

* v2 guard removal/relocation: A2 → `i>=size`, `i>size-1`, redundant
  null check, `i==0`, `field_0x71`; size0 → after hitCamera, before
  isVisible, combined with `!isVisible()`; both guards removed; plus
  bottom `i0` in every combination. Best 89.4 % only at 976 B.
* A shared `unsigned n = objects->size()` for both guards: 69.0 % /
  944 B (+i0: 69.1 % / 952 B).
* landed anchor relocation: A1/A2 to loop top, after null check,
  before/after `isVisible`, steal arm, end of body; operand swaps
  (`size()==i` ↔ `i==size()`, `i>size-1` ↔ `size-1<i`); `!object`;
  `isVisible()==0`; `Vector{0,0,0}` / `Vector()`. All codegen-identical
  or worse.
* M1 anchor relocation (after null / before top i0 / before isVisible
  / end of body), `int i`, `++i`, `i != size`, `size > i`,
  `i <= size-1`: best 87.0 %.
* Faithful-shape surface variants: `const`, ctor form, assignment form,
  function scope, loop scope, nested block, `bool hit`; named
  `getPosition()` temps in the steal/visible arm; `if (object != nullptr)`
  nesting; else-chain with a shared `hitCamera`; inverted `strength` /
  `playing` tests; hoisted steal-arm `continue`. Best 64.1 % / 968 B
  (nested shape, frame 120) or 63.8 % / 968 B (loop-scope surface).

## 6. Conclusion / recommendation

The original source is **v2 minus the `objects->size() == 0` guard**,
i.e. the faithful shape (narrow surface block, bottom `i == 0` guard,
`collision != Vector{…}` operand order, no redundant checks) — but
clang 7.0.2 at `-Oz` rotates that shape to 996 B because of the
canary-restore tail merge. Every construct that keeps `BFCHANGED=0`
either costs ≥ 8 B (v2: 976) or permutes the register file (M1:
`r9`/`fp` swap).

A future pass should look for a **zero-byte** construct that keeps the
narrow `surface` lifetime (original `r6`/frame-112 RA) but stops the
`bb.33`/`bb.20` canary-restore merge — that is the only remaining lever
between 87.6 % and ~100 %. Note that the merge is on
`ldr r6, [sp,#20]` (the **canary**, not the dock pointer); changing the
canary's register/spill slot without adding instructions is the target.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6610
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice the body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6610/score.py                         # pct/size/lens
python3 /dev/shm/w6610/score.py -d                      # normalised diff
```

Scratch harness (`/dev/shm/w6610`): `score.py`, `splice.py`,
`build.sh`, `ir.sh`, `sweep*.py`, body files (`body_landed.txt`,
`body_m1.txt`, `body_v2.txt`, `body_true.txt`, `body_nest.txt`,
`body_else.txt`, `s_*.txt`). Patched LLVM
`/tmp/llvm_android_build/bin/llc` with `MBP_TRACE=1` and
`-debug-only=branch-folder` (must pass `-O2`, not `-Oz`, plus
`-mattr=+neon,+thumb-mode`).

## 8. Gate

Unchanged: `byte_exact 4501 / linked_exact 4501 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B. No source change, so
`tools/verify_baseline.json` is untouched.
