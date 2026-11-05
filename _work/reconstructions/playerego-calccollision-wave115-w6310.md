# PlayerEgo::calcCollision — wave 115 (w6310): exact block order at 968 B, RA still permuted

**Status:** research note, 2026-11-05. Worktree `wt/w6310`, branch
`wave115-w6310`, base `b847d5d46`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B shape (the "landed" body with the two
redundant anchors) is still the best measured. This note extends
`playerego-calccollision-wave114-w6211.md` (and waves 107–113).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed body is still the best point: **87.6436781609 % / 968 B /
  347-349** normalised instructions. ~120 new builds this wave find
  nothing above it.
* **New: the original's exact block order is reproducible at exactly
  968 B with *no* anchors**, by using the `!=`-with-inner-`continue`
  steal arm (which the landed body already has) together with a
  loop-scope (not nested) `Vector surface`. The result is
  **63.8 % / 968 B / 347-349**, non-rotated, latch at `0x2c0`-shape,
  shared `hitCamera` — i.e. the original's layout. The residual delta is
  a pure register permutation: `surface`=r8, `object`=r9, `dock`=sl
  (original: `surface`=r6, `object`=r8, `dock`=r9, `distance`=sl).
* **New: the pivot is the `surface` stack slot.** A nested
  `{ Vector surface … }` block shortens the lifetime so `surface`
  shares `sp#32` with the velocity/zero/Matrix temps and lands in r6
  (the original's RA) — but the shortened lifetime changes the IR
  enough that the MBP-internal BranchFolder tail-merges the two
  one-instruction `b <latch>` tails (`bb.33` and `bb.20`), sets
  `BFCHANGED=1`, re-runs `buildCFGChains()` and **rotates the loop**
  (996 B). A loop-scope `Vector surface` keeps its own slot at `sp#104`,
  `BFCHANGED=0`, no rotation (968 B), but `surface` is rematerialised
  per iteration in r8 and the RA permutes.
* **New: hoisting `Vector &drift = this->moveDirection;` to loop scope**
  changes the RA of the anchor-free shape: `object` moves to r8
  (matching the original) and the score rises **63.8 → 72.4 %** at
  968 B. `dock` is still sl and `surface` still r9.
* **New (source clean-ups, codegen-identical at 87.6436781609 %):**
  the A2 anchor can be folded into the mining test
  (`isShrinking() || miningGame != 0 || objects->size() == i`) and the
  A1 anchor can be moved after the type check; both emit byte-identical
  code to the landed body.

## 1. Exact scores (this wave)

All sizes are 968 B unless stated; `lens` is orig/ours normalised
instructions.

```
landed (nested surface, A1 = i > size-1, A2, no visible i0)   87.6436781609  968  347/349
landed, A1 moved after the type check                         87.6436781609  968  347/349
landed, A2 folded into the mining test                        87.6436781609  968  347/349
landed, A2 = objects->empty()                                 87.6436781609  968  347/349
landed, surface as an inline temp                             87.6436781609  968  347/349
landed, visible-arm `collision != Vector{...}` (swap)         87.4000000000  968  347/349
landed, steal-arm `Vector{...} != impact` (swap)              87.1000000000  968  347/349
landed, A1 = i >= size (= L3)                                 85.5000000000  976  347/348
landed + visible i0 guard                                     85.0000000000  976  347/352
landed + visible i0, A2 removed                               82.6000000000  968  347/348
landed, A1 removed (A2 only)                                  70.8000000000  984  347/351
landed, both anchors removed                                  59.8000000000  988  347/349
```

Anchor-free (`!=`-with-inner-`continue` steal arm, visible i0 guard):

```
loop-scope Vector surface (no anchors)                        63.8000000000  968  347/349
  + Vector &drift hoisted to loop scope                       72.4000000000  968  347/349
  + drift hoisted at function scope                           72.4000000000  968  347/349
  + drift hoisted, visible i0 removed                         67.0000000000  960  347/346
  + A2 before the surface block                               67.2000000000 1008  347/358
  + A2 in the wormhole arm                                    57.2000000000 1012  347/359
  + drift hoisted + A2 in the wormhole arm                    67.4000000000 1012  347/359
nested Vector surface (no anchors)                            60.1000000000  996  347/352
  + drift hoisted                                             49.8000000000  996  347/352
inline temp (no anchors)                                      60.1000000000  996  347/352
```

`i0` = the original's `if (i == 0 && object->field_0x71)` guard in the
visible arm. The landed body is missing it.

## 2. The anchor-free shape that reproduces the original layout

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
        Vector surface = this->geometry->getPosition();      // loop scope
        if (!object->outerCollide(surface))
            continue;
        if (object->getType() == 16994 && object->isVisible()) {
            if (((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0)
                continue;
            Vector &pull = this->moveTarget;
            pull = object->getPosition();
            pull -= this->geometry->getPosition();
            int distance = (int) AbyssEngine::AEMath::VectorLength(pull);
            int strength = 40000 - distance;
            bool playing = Globals::sound->isPlaying(34);
            if (strength >= 1) {
                if (playing) {
                    Vector velocity = {0.0f, 0.0f, 0.0f};
                    Vector position = object->getPosition();
                    Globals::sound->updateEvent3DAttributes(34, &position, &velocity, false);
                } else {
                    Globals::sound->play(34, nullptr, nullptr, 0.0f);
                }
                pull = AbyssEngine::AEMath::VectorNormalize(pull);
                Vector &drift = this->moveDirection;
                drift = Vector(pull);
                drift *= (float) (strength >> 8);
                this->geometry->translate(drift);
                this->hitCamera();
                if (distance < 1000)
                    this->inWormhole = 1;
                reinterpret_cast<Matrix &>(this->player->transform[0]) =
                    *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                    * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
            } else {
                if (playing)
                    Globals::sound->stop(34);
            }
            continue;
        }
        if (object->stealFlagByte) {
            if (object->isDying() || object->isDead())
                continue;
            if ((this->dockingState || this->dockedFlag) && object == (KIPlayer *) (intptr_t) this->asteroidTarget)
                continue;
            Vector impact = object->getProjectionVector(this->geometry->getPosition());
            if (impact != Vector{0.0f, 0.0f, 0.0f}) {
                object->player->setHitVector(impact.x, impact.y, impact.z);
                object->player->setBombForce(this->boostingFlag ? 0.2f : 0.1f);
                object->player->damage(9999);
                this->player->damage(20);
                if (this->volatileGoods)
                    this->player->flShake += 0.2f;
                this->hitCamera();
                continue;
            }
            continue;
        }
        if (!object->isVisible())
            continue;
        Vector collision = object->projectCollisionOnSurface(this->geometry->getPosition());
        if (Vector{0.0f, 0.0f, 0.0f} != collision) {
            this->geometry->setPosition(collision);
            reinterpret_cast<Matrix &>(this->player->transform[0]) =
                *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
            if (i == 0 && object->field_0x71)
                this->collidesWithStationFlag = 1;
        }
        this->hitCamera();
    }
}
```

This is the shape whose machine block order matches the original exactly
(header → null → i0 → surface → outerCollide → type → isVisible →
isShrinking → mining → wormhole body → strength → playing → cold
`b` → steal → visible → shared `hitCamera` → latch → cold
strength<1/stop/play/wormhole-tail). It is 968 B, `BFCHANGED=0`, and
non-rotated. The only mismatch is the register file.

## 3. Why the nested `surface` rotates the loop

The landed body uses a nested block:

```c++
        {
            Vector surface = this->geometry->getPosition();
            if (!object->outerCollide(surface))
                continue;
        }
```

The `llvm.lifetime.end` for `surface` sits in the outerCollide-false
block. In the machine CFG this makes the wormhole tail (`bb.20`) branch
straight to the latch (`bb.40`), and the steal-arm `continue` block
(`bb.33`) also branch to `bb.40`. Both blocks end in a one-instruction
`b bb.40`, so the MBP-internal BranchFolder tail-merges them
(`EffectiveTailLen = 1 + 1 = 2 >= TailMergeSize = 2`), sets
`BFCHANGED=1`, re-runs `buildCFGChains()`, and the chain bottom becomes
the latch → `ROT ROTATING` (996 B).

With a loop-scope `Vector surface` the lifetime spans the loop body, the
early BranchFolder instead merges the wormhole tail's `continue` with the
steal arm's `continue` into a shared block (`bb.20 -> bb.33 -> bb.40`,
`bb.33 = ldr; b`), the two tails no longer have the same successor, no
tail merge fires, `BFCHANGED=0`, and the first-pass layout (the
original's order) is kept.

## 4. The register file

Real NDK clang 7.0.2, `-Oz`:

```
landed (anchors, nested surface):  r4=objects r5=this r6=surface r8=object r9=dock sl=distance fp=i
original:                          r4=objects r5=this r6=surface r8=object r9=dock sl=distance fp=i
anchor-free, loop-scope surface:   r4=objects r5=this r8=surface r9=object sl=dock   fp=i  (r6=canary)
anchor-free + drift hoisted:       r4=objects r5=this r9=surface r8=object sl=dock   fp=i  (r6=canary)
```

The anchor-free shape keeps the stack-canary pointer in r6 across the
whole function (saved at `[sp,#24]`, reloaded in the epilogue); the
original spills the canary and uses r6 for `surface`. This follows from
the `surface` slot: loop-scope `surface` is a separate 12-byte slot at
`sp#104` (frame 120 B) and is rematerialised with `add r8, sp, #104`
inside the loop; the original's `surface` shares `sp#32` with the
velocity/zero/Matrix temps (frame 112 B) and its address is hoisted to
r6 in the prologue (`add r6, sp, #32`).

The landed body's two redundant anchors are what force the allocator
back to the original register file (the A2 anchor's `ldr r0,[r4]` in the
wormhole arm re-weights the spill costs; see wave 114 §3), at the price
of the early latch.

## 5. Other measured dead ends this wave

| variant (anchor-free base unless stated) | pct | size | note |
|---|---:|---:|---|
| `int i` / `i != size` | 63.5 | 968 | codegen-identical to base |
| `objects->data_[i]` | 63.8 | 968 | codegen-identical |
| `Vector &dock = this->dockOffsetVec` at loop scope | 63.8 | 968 | codegen-identical |
| `bool first = (i == 0)` | 63.8 | 968 | codegen-identical |
| `Vector &pull` hoisted to loop scope | 63.8 | 968 | codegen-identical |
| `const Vector &surface = …` | 63.8 | 968 | codegen-identical |
| `Vector surface; surface = …` | 59.2 | 984 | |
| `Vector surface` at function scope | 44.4 | 1028 | |
| `Vector surface` at loop scope before the i0 check | 48.6 | 1008 | |
| `bool hit; { … }` block | 60.1 | 996 | rotated |
| `outerCollide(Vector(getPosition()))` copy temp | 60.1 | 996 | rotated |
| `if (collision == zero) { hitCamera(); continue; }` | 45.7 | 1032 | |
| `hitCamera()` inside the visible `if (collision)` | 43.4 | 1020 | |
| `hitCamera()` before the volatileGoods block | 63.8 | 968 | codegen-identical |
| `Vector collision; collision = …` | 72.1 | 1012 | (landed base) |
| `Vector collision` at loop scope | 77.6 | 988 | (landed base) |
| `Vector impact` at loop scope | 76.7 | 988 | (landed base) |
| `if (i == 0 && objects->size() > i && field_0x71)` | 79.5 | 972 | (A2 removed) |
| A2 in the steal arm | 81.6 | 968 | |
| A2 in the visible arm | 72.0 | 984 | |
| A2 at the end of the wormhole arm | 79.5 | 964 | |
| A1 at the end of the loop body | 70.8 | 984 | |
| A1 before the surface block | 68.9 | 992 | |
| A1 after the steal arm | 79.2 | 980 | |

## 6. Proven vs. hypothesis

**Proven (measured):**

* The landed body's exact score is 87.6436781609 % (347 vs 349
  normalised instructions, 968 B); ~120 new variants this wave find no
  better point.
* The original's machine block order is reproducible at exactly 968 B
  with no anchors, using the `!=`-with-inner-`continue` steal arm and a
  loop-scope `Vector surface` (63.8 %); the residual delta is a pure
  register permutation.
* The `surface` stack slot (nested `sp#32` vs loop-scope `sp#104`)
  selects between the original RA and the original layout: the nested
  form gives r6/surface but tail-merges the `b` tails and rotates the
  loop (996 B); the loop-scope form keeps the layout (968 B) but
  rematerialises `surface` in r8.
* Hoisting `Vector &drift` to loop scope moves `object` to r8 and lifts
  the anchor-free shape from 63.8 % to 72.4 % at 968 B.
* The A2 anchor can be folded into the mining test and the A1 anchor can
  be moved after the type check; both are codegen-identical to the
  landed body at 87.6436781609 % / 968 B.

**Hypothesis (not proven):**

* The original source has the narrow `surface` scope (so it shares
  `sp#32` and gets r6) *and* some construct that keeps the MBP-internal
  BranchFolder from merging the two `b <latch>` tails, which this wave
  did not find. The landed body's anchors are the only construct found
  that reproduces the original register file, and they cost the latch
  placement.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6310
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice the body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6310/score.py                         # pct/size/lens
python3 /dev/shm/w6310/score.py -d                      # normalised diff
```

Layout trace (real clang 7.0.2, block order after MBP):

```sh
CC=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CC <flags from build.ninja> -mllvm -print-after-all \
    -mllvm -filter-print-funcs=_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE \
    -c src/game/ship/PlayerEgo.cpp -o /dev/null 2> mir.txt
```

MBP / BranchFolder trace (patched LLVM 7.0.0svn; note it does **not**
reproduce the real 7.0.2 RA for these shapes, only the block-order
mechanism):

```sh
MBP_TRACE=1 /tmp/llvm_android_build/bin/llc \
  -mtriple=armv7-none-linux-androideabi -mattr=+neon,+thumb-mode \
  -float-abi=soft -O2 fn.ll -o /dev/null 2>&1 | grep calcCollision
/tmp/llvm_android_build/bin/llc ... -debug-only=branch-folder fn.ll -o /dev/null
```

Scratch harness (`/dev/shm/w6310`): `score.py` (single-function
score/diff), `splice.py`, `emitll.sh`, `sweep*.py`, and the body files
(`body_current.txt` = landed, `body_es2.txt` = anchor-free loop-scope
surface, `body_driftloop.txt` = anchor-free + drift hoisted,
`body_L3.txt`, `body_early_steal*.txt`, …).

## 8. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0,
`RATCHET PASS`.  27 sections exact, file size 2,187,328 B.  No source
change, so `tools/verify_baseline.json` is untouched.
