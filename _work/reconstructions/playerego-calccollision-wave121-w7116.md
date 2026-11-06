# PlayerEgo::calcCollision — wave 121 (w7116): byte-exact at 968 B

**Status:** source change landed, 2026-11-06.
Worktree `wt/w7116`, base `bd061a489` (wave 120). Target:
`_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B, 347 normalised
instructions).

## Result

| | before | after |
|---|---|---|
| verify pct | 95.500000 | **100.000000** |
| size | 968 B | 968 B |
| normalised insns | 346/347 | **347/347** |
| `byte_exact` | no | **yes** |
| `linked_exact` | no | **yes** |

Full gate: `byte_exact 4505 -> 4506`, `linked_exact 4505 -> 4506`,
size-exact 4524/4524, data_exact 843/843, imports 207, lint **CLEAN**,
drift **0**, `RATCHET PASS`. `tools/verify_baseline.json` locked in the same
commit.

## The winning source shape

The body is the *faithful* reconstruction (no redundant guards) plus two
branch-weight hints:

```c++
if (object->getType() == 16994 && object->isVisible()) {
    if (__builtin_expect(((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0, 1))
        continue;
    ...
}
...
if (object->stealFlagByte) {
    if (__builtin_expect(object->isDying() || object->isDead(), 0))
        continue;
    ...
}
...
if (!object->isVisible())
    continue;
Vector collision = object->projectCollisionOnSurface(this->geometry->getPosition());
if (collision != Vector{0.0f, 0.0f, 0.0f}) {
    ...
    if (i == 0 && object->field_0x71)
        this->collidesWithStationFlag = 1;
}
this->hitCamera();
```

The two `// lint: arm_builtin` waivers are same-line, as required. There are no
other source changes; the old body's wormhole `size() == i` guard, visible-arm
`empty()` guard and `field_0x71` tail store are gone.

## Why this works (rotation + compare, resolved)

The residual was the interaction the wave-119 notes identified but could not
land: the original's loop is **un-rotated** (header at the top, latch at the
bottom) and its visible arm contains the register shuffle

```
9f30c: mov sl,r5        ; sl = this
9f30e: mov r5,r6        ; r5 = &surface
9f310: add r6,sp,#96    ; r6 = &collision
...
9f324: mov r0,r6        ; &collision
9f326: mov r6,r5        ; &surface (zero vector)
9f328: mov r5,sl        ; this
9f32a: mov r1,r6        ; &zero
9f32c: blx operator!=
```

Both fall out of `MachineBlockPlacement` (LLVM 7, `lib/CodeGen/
MachineBlockPlacement.cpp`), not the IR loop-rotate pass:

* `rotateLoop` bails via
  `if (ViableTopFallthrough) { ... if (ExitingBB->isSuccessor(NextBlockInChain)) if (!Bottom->isSuccessor(Top)) return; }`.
  The rotation therefore depends on which block ends up **last in the loop
  chain**.
* With the faithful body, the chain ends with the latch (`bb.40`), which
  branches back to the header, so the loop rotates (`Bottom->isSuccessor(Top)`
  is true).
* With the two hints, the chain ends with the cold wormhole block `bb.20`
  (the `strength >= 1` else / normalize tail), which branches to the latch,
  not the header, so `rotateLoop` bails. The loop stays un-rotated and the
  register file is the original's (`this=r5`, `i=fp`, `object=r8`,
  `surface=r6`, `distance=sl`).
* With the un-rotated register file and the call+compare still in one block
  (no size guard between them), the allocator reproduces the original's
  visible-arm shuffle exactly.

MIR evidence (`-mllvm -print-after-all`, dump after
`Branch Probability Basic Block Placement`):

```
faithful:  bb.0..bb.3, bb.5..bb.39, bb.21, bb.22, bb.19, bb.20, bb.33,
           bb.40 (latch), bb.4 (header), bb.41   -> rotated
winning:   bb.0..bb.3, bb.4 (header), bb.5..bb.18, bb.23..bb.38,
           bb.21, bb.22, bb.19, bb.20, bb.39     -> un-rotated, chain ends
                                                    on cold bb.20
```

The `shr:1` hint marks the wormhole `continue` hot, and `dying:0` marks the
steal-arm dying check cold; together they move the latch ahead of the cold
wormhole blocks in `buildChain`'s greedy order, which is exactly the chain
shape the original has.

## How it was found

A pair sweep over `__builtin_expect` on 11 branches (all single hints plus all
pairs, both values, ~500 builds) against the faithful body. `shr:1 + dying:0`
was the **only** 100 % hit; the next best pair was 72.6 % (`shr:1 + vis:1`,
968 B). Single hints were all <= 70.1 %.

Note that the previous landed body (95.5 %) paid for the un-rotated loop with
two *real* redundant guards (wormhole `size() == i` = 8 B, visible-arm
`empty()` = 4 B) and dropped the tail `movs r0,#1` (2 B). The hints are free:
the loop is un-rotated without any guard, so the visible arm keeps the
original's 10 B shuffle and the tail keeps the constant store, all inside the
original 968 B.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7116
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

`ndiff.py` on the final `.so` prints `354 354` with no diff hunks; the
function is byte-identical at `0x9f0cc` (968 B).
