# PlayerEgo::calcCollision — wave 119 (w6738): a branch-weight hint buys
# 87.6436781609 % -> 94.3722943723 % at the same 968 B

**Status:** source change landed, 2026-11-05. Worktree `wt/w6738`, branch
`wave119-w6738`, base `6552d3af5`. This note extends
`playerego-calccollision-wave119-w6708.md` (v2 = 976 B, gate-breaking) and
`-wave118-w6610.md` (BF tail-merge of the canary restore rotates the loop).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* **Landed: 87.6436781609 % -> 94.3722943723 %** (`347/346` normalised
  instructions, 968 B, unchanged size). `check` = **RATCHET PASS**:
  byte_exact **4505**, linked_exact **4505**, size-exact 4524,
  data_exact 843/843, imports 207, lint **CLEAN**, drift 0.
  File size 2,187,328 B, `.text` 1,259,464 B, `.ARM.extab` 156,304 B
  (still 8 B short — unchanged, `createShip` only).
* The win is a **`__builtin_expect(..., 1)` branch-weight hint on the
  wormhole-arm continue**, plus moving the two redundant
  `objects->size()` guards to *new* cold positions (steal arm before
  `getProjectionVector`, visible arm after `projectCollisionOnSurface`).
  The hint changes the pre-RA block frequencies, which is what pins the
  original register file and block order **without** the two extra
  guards of v2 and **without** growing the function.
* The resulting instruction stream is the original's except for:
  the preheader `str.w r9,[sp,#20]` (ours sinks it into the wormhole
  arm), the two redundant guards (5 instructions), the missing
  visible-arm `i == 0` test (2 instructions), and the visible-arm
  register shuffle (5 instructions). 17 of 347 instructions differ.
* ~330 builds this wave. Nothing else at ≤ 968 B beats 94.37 %.

## 1. The landed body

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
            // Branch-weight hint: marks the wormhole continue hot.
            if (__builtin_expect(((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0, 1)) // lint: arm_builtin
                continue;
            ... wormhole arm unchanged (v2/full body) ...
            continue;
        }
        if (object->stealFlagByte) {
            if (object->isDying() || object->isDead())
                continue;
            if ((this->dockingState || this->dockedFlag) && object == (KIPlayer *) (intptr_t) this->asteroidTarget)
                continue;
            if (objects->size() == i)          // redundant (i < size)
                continue;
            Vector impact = object->getProjectionVector(this->geometry->getPosition());
            ...
        }
        if (!object->isVisible())
            continue;
        Vector collision = object->projectCollisionOnSurface(this->geometry->getPosition());
        if (objects->size() == 0)              // redundant (i < size)
            continue;
        if (collision != Vector{0.0f, 0.0f, 0.0f}) {
            this->geometry->setPosition(collision);
            ... matrix ...
            if (object->field_0x71)
                this->collidesWithStationFlag = 1;
        }
        this->hitCamera();
    }
}
```

The `__builtin_expect` carries the same-line `// lint: arm_builtin`
waiver used elsewhere in the tree (`Generator.cpp`, `StarMap.cpp`,
`PlayerEgo::update`); without the waiver the per-file lint ratchet
(`arm_builtin` baseline 0) fails.

## 2. Why the hint is load-bearing

The v2 shape (wave 119 w6708) is the original's instruction stream plus
4 instructions and 976 B — gate-breaking, because `.text` is a step
function of the calcCollision size (969…984 B -> `.text` 1,259,496;
≤ 968 B -> 1,259,464). The faithful shape without any guard compiles to
996 B because the loop rotates.

The hint marks `isShrinking() || miningGame != 0` as likely, which
lowers the estimated frequency of the wormhole-arm blocks that use
`&this->moveTarget`. That is exactly the lever the two `objects->size()`
guards of v2 were pulling (they split the same blocks), but the hint
costs **zero bytes** and does not perturb the instruction stream.
With the hint in place the allocator produces the original register
file (`r4`=objects, `r5`=this, `r6`=surface, `r8`=object, `r9`=dock,
`sl`=distance, `fp`=i, canary address at `[sp,#24]`, dock at
`[sp,#20]`) and the original block order (latch at `loc_2c0`, cold
wormhole tails after it) at 968 B.

The two redundant `objects->size()` guards are still needed — removing
either one rotates the loop again (`noA2` 87.2 % / 960 B,
`noSZ` 69.2 % / 996 B) — but their *positions* are now free: the guard
in the steal arm sits after the `dockingState/asteroidTarget` check and
the visible-arm guard sits after `projectCollisionOnSurface`, where the
compare folds to a bare `cbz`.

## 3. Measurements (this wave, all ≤ 968 B unless marked)

`pct` is the verify.py normalised-instruction ratio; `lens` = orig/ours.

```
landed (previous, checked in)                              87.6436781609  968  347/349
NEW  expect(shr,1) + A2 after docking check + size0 after proj  94.3722943723  968  347/346
     ... same, A2 in the steal-arm head                    93.8           968  347/346
     ... same, A2 right after isDying/isDead               93.8           968  347/346
     ... same, A2 after impact!=0                          88.2           968  347/347
     ... same, A2 after the wormhole-arm head              93.2           968  347/346
     ... same, size0 spelled objects->empty()              94.4           968  347/346
     ... same, compare spelled collision != Vector{...}    94.4           968  347/346
     ... same, const Vector collision                      94.4           968  347/346
     ... same, + visible i == 0 (the real original test)   86.8           976  347/349
     ... same, visible i == 0 and A2 dropped               88.9           968  347/346
     ... same, size0 dropped                               69.2           996  347/355
     ... same, A2 dropped                                  87.2           960  347/343
     ... same, expect on outerCollide / impact / dist /
         volatileGoods / dockedState (0 or 1)              94.4           968  347/346
     ... same, expect on the steal flag (1)                71.9           956  347/348
     ... same, expect on !isVisible (0)                    57.8           992  347/352
     ... same, expect on the top i == 0 (1)                78.4           972  347/344
     ... same, expect(shr,0)                               48.3          1004  347/353
     ... same, expect(isShrinking(),0) only                63.2           952  347/346
     ... same, surface at loop scope                       57.8           996  347/362
     ... same, zero vector hoisted to loop scope           94.4           968  347/346
     ... same, zero vector hoisted to function scope       85.5           968  347/345
     ... same, redundant steal-arm `continue` removed      88.0           968  347/346
     ... same, impact test inverted                        93.8           968  347/346
v2 (previous best, gate-breaking)                          89.3982808023  976  347/351
v2 + expect(shr,1) (SZ still before collision)             93.2           968  347/346
v2 + expect(shr,1) + size0 moved after proj (== NEW)       94.4           968  347/346
SZ_proj (v2, size0 after proj, no hint)                    87.3           964  347/345
faithful (no guards, no hint)                              60.1           996  347/352
faithful + expect(i==0,1) on the top i0                    70.6           960  347/347
faithful + expect(!isVisible,0)                            69.8           960  347/346
faithful + expect(collision!=0,1)                          68.0           960  347/347
faithful loop-scope surface                                64.1           968  347/349
faithful loop-scope surface + Player *pl local             67.7           968  347/350
REA-temp + expect(isVisible,1)                             69.5           944  347/344
```

## 4. Dead ends measured this wave (don't redo)

* **Splitting `||` conditions** into two `if (...) continue;` — identical
  codegen (SimplifyCFG re-merges them). Same for splitting the
  `dockedFlag &&` prologue guard and the `dockingState || dockedFlag`
  test.
* **Loop forms**: `++i`, `i != size`, `size > i`, `i <= size - 1`,
  `objects->count`, `objects->data_[i]`, `objects->data()[i]`,
  `Array& objs = *objects` alias — all byte-identical to the faithful
  shape (996 B).
* **Local aliases** `AEGeometry *geo`, `FModSound *snd`, `Status *st`,
  `PaintCanvas *cv`, `Player *pl`, `Vector &dock`, `Vector *dock`,
  `Vector &pull`/`&drift` at function scope — none beat 94.4 % at
  ≤ 968 B (best 88.9 %).
* **Nesting the `i == 0` tests**, reordering the top i0 condition,
  `const Vector surface`, ctor form, `Vector surface;` at function
  scope — all ≤ 996 B and worse.
* **Moving the top i0 check** after the surface block / to the end of
  the loop body — 74.9 % / 972-980 B.
* **Guard sweeps**: 18 anchor positions × 3 expressions on the faithful
  body, 16 positions on the landed body, 11 positions on the v2 body,
  7 positions on the hint body — the only > 90 % results are the ones in
  the table above.
* **`__builtin_expect` sweeps**: 17 conditions × {0,1} on the faithful,
  landed and v2 bodies. Only the wormhole-continue hint at value 1
  reaches 94.4 % at 968 B.

## 5. Remaining delta to the original (17 instructions)

1. preheader `str.w r9,[sp,#20]` — ours sinks the dock-pointer spill
   into the wormhole arm (and re-spills it in the visible arm); this
   also shifts 4 short branch targets (`loc_aa` vs `loc_ae`).
2. the two redundant `objects->size()` guards (5 instructions).
3. the visible-arm `i == 0` test (2 instructions) — adding it costs
   +8 B and drops the ratio to 86.8 %, so it is not landed.
4. the visible-arm register shuffle: the original keeps `&collision` in
   `r6` and rotates `r5`/`sl`; ours re-materialises `add r0, sp, #96`
   (5 instructions).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6738
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice the body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6738/score.py                         # pct/size/lens
python3 /dev/shm/w6738/score.py -d                      # normalised diff
```

Scratch harness (`/dev/shm/w6738`): `score.py`, `splice.py`, `build.sh`,
`ir.sh`, `reg.sh`, `mk.py`, `mk2.py`, `batch.sh`, the `sweep*.py`
generators and the `body_*.txt` / `final.txt` bodies.

## 7. Gate

`byte_exact 4505 / linked_exact 4505 / size-exact 4524 / data_exact
843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B, `.text` 1,259,464 B,
`.ARM.extab` 156,304 B (8 B short, unchanged).
