# PlayerEgo::calcCollision — wave 119 (w6952): the 2-byte barrier;
# 95.50 % stays, no source change landed

**Status:** analysis only, **no source change landed**, 2026-11-06.
Worktree `wt/w6952`, branch `wave119-w6952`, base `db1106935` (current
master, wave 119; includes w6905's real `i == 0` tail and w6926's
shuffle analysis). Extends `playerego-calccollision-wave119-w6905.md`
(95.53 %), `-w6926.md` (shuffle reproducible, +8 B), `-w6738.md`
(94.37 %) and `-w6732.md` (95.24 %).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* **No improvement found.** HEAD's body still measures
  **95.500000 %** (`347/346` normalised instructions, **968 B**).
  `check` = **RATCHET PASS**: byte_exact **4505**, linked_exact
  **4505**, size-exact 4524, data_exact 843/843, imports 207, lint
  **CLEAN**, drift **0**, 27 sections, file 2,187,328 B.
* New this wave: an exact **byte accounting of the remaining delta**
  (§2) and the proof that the last missing instruction (`movs r0,#1`
  in the tail) is blocked by a hard **2-byte** wall (§3).  Every
  variant that adds the `movs` grows the code region from 932 B to
  934 B, the literal pool slides from `0x9f470` to `0x9f474`, every
  `ldr r0,[pc,#N]` and branch displacement shifts, and the score
  collapses to **87.8 %**.
* ~260 builds this wave.  Nothing at 968 B beats 95.50 %; two distinct
  A2 guard placements (wormhole-arm head, before `Vector &pull`) tie
  at 95.50 %.

## 1. The landed body (unchanged, reference)

Faithful control flow plus two **redundant** guards and the real tail:

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
            if (objects->size() == i)          // A2 guard (redundant)
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
        if (objects->empty())                // SZ guard (redundant)
            continue;
        if (Vector{0.0f, 0.0f, 0.0f} != collision) {
            this->geometry->setPosition(collision);
            reinterpret_cast<Matrix &>(this->player->transform[0]) =
                *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
            if (i == 0 && object->field_0x71)
                this->collidesWithStationFlag = object->field_0x71;
        }
        this->hitCamera();
    }
}
```

## 2. Exact byte accounting of the remaining delta (new)

Both functions are 968 B and the literal pool starts at the same
address (`0x9f470`), so the code region is 932 B in both.  Aligning
the two disassemblies on the sequence of `blx` call targets isolates
exactly three regions where the sizes differ:

```
region (between consecutive calls)                 orig   ours   delta
----------------------------------------------------------------------
wormhole arm, up to `object->getPosition()` (r2)     30     38     +8   <- A2 guard
visible arm, up to `projectCollisionOnSurface` (r3)  22     16     -6   <- shuffle
visible arm, up to `hitCamera`                       24     22     -2   <- tail `movs`
----------------------------------------------------------------------
net                                                    0      0      0
```

The `+8` is the A2 guard `ldr r0,[r4,#0] / cmp fp,r0 / beq.w` (2+2+4;
the branch is 454 B away, so it cannot be a 16-bit `beq.n`).
The `-6` is the visible-arm register shuffle: the original spends four
extra 2-byte `mov`s (`mov sl,r5 / mov r5,r6 / add r6,sp,#96 /
mov r0,r6`) before the call and three more (`mov r6,r5 / mov r5,sl /
mov r1,r6`) after it, while we rematerialise `add r0,sp,#96` /
`add r1,sp,#96`; the SZ guard's `ldr r0,[r4,#0] / cbz r0` (4 B) is
inside the post-call region and exactly cancels the shuffle-restore
part of it.
The `-2` is the missing `movs r0,#1` in the tail (we store
`object->field_0x71` so clang reuses the loaded byte).

## 3. The 2-byte wall (new)

To add the original `movs r0,#1` the tail must be spelled
`this->collidesWithStationFlag = 1;` (or `= true`,
`= (bool) object->field_0x71`, `= object->field_0x71 != 0`, or with
the field declared `bool` — all five compile to the same `movs`).  That makes the code region 934 B,
the pool must be 4-byte aligned, so the function becomes **972 B** and
the pool slides to `0x9f474`:

```
tail `= object->field_0x71`      968 B   95.5 %   (landed)
tail `= 1`                       972 B   87.8 %
tail `= object->field_0x71 != 0` 972 B   87.8 %
tail `= true`                    972 B   87.8 %
tail `= (bool) field`            972 B   87.8 %
header field `bool` + `= field`  972 B   87.8 %
```

So the last instruction is blocked on **2 bytes of code**: the code
region would have to shrink from 934 B back to 932 B (the current
no-`movs` size) for the pool to stay at `0x9f470`.
The only 4-byte-or-more levers are the two redundant guards, and both
are load-bearing for the register file (see §4).

## 4. Guard sweep (re-confirmed, extended)

`gen.py`-style body generator, `bs.sh` = ninja the one object + link +
score (~1.1 s).  `pct` is verify.py's normalised-instruction ratio.

### 4.1 Guard presence

```
A2 only (`size()==i` in wormhole arm)   988 B   70.4 %
SZ only (`empty()` after collision)     976 B   66.3 %
neither (faithful)                      992 B   59.7 %
both (landed)                           968 B   95.5 %
```

Both are required: either one alone changes greedy RA away from the
original register file (`this=r5`, `&surface=r6`, `distance=sl`,
`i=fp`, `object=r8`, `&dockOffsetVec=r9`, `objects=r4`).

### 4.2 Guard expression (A2 x SZ, 7 x 6 = 42 builds)

Every guard that reads an `Array` field is code-identical and scores
95.5 %: `size()==i`, `size()==0`, `count==i`, `count==0`,
`data_==nullptr`, `capacity_==0`, `i>=size()`.  Only
`object == nullptr` differs (it drops the `objects` reference and
breaks the register file: 988 B / 70.4 %).

### 4.3 Guard position (8 A2 positions x 7 SZ positions x 2 tails = 112 builds)

Best, all **968 B / 95.5 %** — two distinct A2 placements times two
spellings of the SZ guard (`empty()` and `size()==0`, which are
code-identical):

```
A2 = before `Vector &pull` (mid)  , SZ = after projectCollision (mid/vis)
A2 = head of wormhole arm (head)  , SZ = after projectCollision (mid/vis)
```

Everything else is worse.  Notably:

```
A2 in steal arm only                     972 B   66.3 %
A2 before/after `isVisible` only         988 B   64.1 %
A2 at end of wormhole arm                976 B   66.3 %
SZ before `projectCollisionOnSurface`    980 B   88.1 %   (shuffle exact, +6)
SZ before projectCollision + tail `= 1`  984 B   88.4 %
SZ in wormhole arm                       996 B   62.5 %
SZ in steal arm                         1004 B   55.8 %
```

`SZ = precall` is the only placement that reproduces the original's
visible-arm shuffle instruction-for-instruction (w6926's result,
re-confirmed here at 980 B / 88.1 %; the SZ guard then sits before
`getPosition` and costs 6 B instead of 4 B, so it is +12 B net).

## 5. Other source shapes measured this wave (all dead ends)

```
landed (reference)                                  968 B   95.5
cmp `collision != Vector{...}` (unswapped, = orig)  968 B   95.2
cmp `Vector() != collision`                         968 B   95.5
cmp `!(collision == Vector{...})`                   968 B   94.7
`const Vector collision`                            968 B   95.5
`const Vector &collision`                           968 B   95.5
`Vector collision(...)` ctor form                   968 B   95.5
`Vector collision; collision = ...`                1028 B   55.2
`AbyssEngine::AEMath::Vector` spelling              968 B   95.5
`Vector zero; if (zero != collision)`               968 B   84.8
`Vector zero; if (collision != zero)`               968 B   84.8
`KIPlayer *object;` declared outside the loop       968 B   95.5
`i == 0` block nested (`if (i == 0) { if (...) }`)  968 B   95.5
`i == 0` block nested 2 (split at inAlienOrbit)     968 B   95.5
`i == 0` operands reordered (f71 first)             988 B   81.0
`i == 0` operands reordered (VectorLength first)    968 B   93.5
steal-arm compare swapped (`Vector{} != impact`)    968 B   94.9
`if (getType()==16994) { if (!isVisible()) cont; }` 960 B   64.9
  + SZ + A2                                         976 B   83.2
  + SZ + A2 + tail `= 1`                            976 B   83.5
`AEGeometry *geometry = this->geometry;` hoist     1000 B   48.3
`Status *status` hoist                              992 B   47.9
`FModSound *sound` hoist                            988 B   46.9
`PaintCanvas *canvas` hoist                         988 B   48.8
reuse one `Vector surface` for outerCollide and
  projectCollisionOnSurface (with guards)          1016 B   52.6
  (faithful)                                        960 B   56.0
split isShrinking / miningGame into two `if`s       992 B   59.7
split isDying / isDead                              992 B   59.7
split dockingState / dockedFlag                     992 B   59.7
`__builtin_expect` hints on the landed body         968 B  <=95.5
  (this wave: vis0 988/66.9, null0 968/90.9,
   coll0 1012/57.3; the w6905 full sweep found
   nothing above 95.5)
`__builtin_expect` hints on the faithful body       <=972 B <=69.5
  best: vis0                                        956 B   69.5
`__builtin_expect(i==0, 0)` on the tail             968 B   88.8
`__builtin_expect(field_0x71, 0)` on the tail       972 B   86.9
```

The `else if` chain form
(`if (worm) {...} else if (steal) {...} else if (visible) {...}`)
compiles to exactly the same code as the `continue` form (992 B
faithful / 59.7 %), so the source's nesting style is irrelevant.

## 6. Why the guards matter (mechanism)

The guards do not change liveness — `objects` is live across the loop
anyway.  They change greedy RA's *tie-breaks* in the entry block.  In
the faithful build the `__stack_chk_guard` value stays in `r6` until
the loop setup and is spilled with `strd r6,r9,[sp,#20]`; `r5` then
takes `&surface` and `this` is pushed to `sl`, which forces `distance`
into `r6` and a `str r6,[sp,#4]` spill in the wormhole arm.  With the
guards the guard value is spilled immediately (`str r0,[sp,#24]`),
`r6` becomes `&surface`, `this` stays in `r5`, and `sl` is free for
`distance` — the original register file.  `-mllvm -print-machineinstrs`
confirms the landed build's prologue and stack slots now match the
original exactly (`sp+8` `&moveDirection`, `sp+12` `&moveTarget`,
`sp+16` `&Globals::sound`, `sp+20` `&dockOffsetVec`, `sp+24` stack
guard, `sp+28` `&Globals::status`, `sp+32` temp `Vector`,
`sp+96` `collision`, `sp+108` canary).

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6952
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target lint           # CLEAN
cmake --build cmake-build-match --target drift          # 0 fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch harness (`/dev/shm/w6952`): `score.py` (size + pct),
`bs.sh` (ninja one object + link + score, ~1.1 s), `ra.sh` (prologue
register file), `region.py` (per-call-region byte deltas — the tool
that produced §2), `gen.py` / `gen2.py` (body generators),
`gsweep*.py` / `sweep5.py` (sweeps), `PlayerEgo.base.cpp` (exact copy
of HEAD's `PlayerEgo.cpp`), `orig.txt` (original disassembly
`0x9f0cc`-`0x9f494`).

## 8. Gate

`byte_exact 4505 / linked_exact 4505 / size-exact 4524 / data_exact
843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B.  No section or `.ARM.extab`
change (source unchanged from HEAD).
