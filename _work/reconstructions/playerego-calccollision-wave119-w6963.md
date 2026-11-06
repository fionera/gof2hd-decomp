# PlayerEgo::calcCollision — wave 119 (w6963): register-file / loop-rotation
# sweep, 95.50 % stays; the visible arm is now provably reproducible

**Status:** analysis only, no source change landed, 2026-11-06.
Worktree `wt/w6963`, branch `wave119-w6963`, base `438df77e7` (current
master, wave 119). Extends `playerego-calccollision-wave119-w6926.md`
(the shuffle is reproducible but +8 B), `-w6905.md` (95.53 %, the landed
body) and `-w6738.md` (94.37 %).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* **No improvement found.** The landed body still measures
  **95.500000 %** (`347/346` normalised instructions, **968 B**).
  `check` = **RATCHET PASS**: byte_exact **4505**, linked_exact
  **4505**, size-exact 4524, data_exact 843/843, imports 207, lint
  **CLEAN**, drift 0.
* The new result this wave: the original's **whole visible arm**
  (`isVisible` … `operator!=`) is reproduced **instruction for
  instruction** by the *real* compare order (`collision != Vector{…}`)
  plus a redundant guard before `projectCollisionOnSurface`.  The
  previous wave only matched the reversed compare (`Vector{…} !=
  collision`).  The shape is still 8–12 B too big, so the score is
  89.2 %, not 95.5 %.
* Root cause of the size gap is now understood at the LLVM level:
  the un-rotated loop is produced by **`MachineBlockPlacement::
  rotateLoop`** (LLVM 7 `lib/CodeGen/MachineBlockPlacement.cpp`), not
  by the IR `LoopRotate` pass.  `-Oz` sets the IR rotation threshold
  to 0, so the IR stays un-rotated; the machine pass then rotates the
  chain to put the exiting header at the bottom.  The A2 guard changes
  the block frequencies/chain just enough to suppress that rotation.
  Removing it rotates the loop (968 → 976 B, 66.3 %) and also moves
  `i` from `fp` to `r9`; removing both guards rotates it and gives
  996 B (60.1 %).
* ~140 builds this wave.  Nothing at 968 B beats 95.50 %.

## 1. Landed body (unchanged)

The w6905 body: faithful control flow + two redundant guards
(`objects->size() == i` in the wormhole arm, `objects->empty()` after
`projectCollisionOnSurface`) + the real `i == 0` tail storing
`object->field_0x71` (drops the original's `movs r0,#1`, buying the
2 B that keep the function at 968 B).

## 2. Exact remaining delta (16 instructions)

`ndiff.py` on the landed body (all other instructions are identical):

```
@@ top block / steal-arm targets (all shifted +8 by the A2 guard)
-bne.n  loc_178      +bne.n  loc_180
-beq.n  loc_178      +beq.n  loc_180
@@ wormhole arm
                     +ldr    r0, [r4, #0]     <- A2 guard
                     +cmp    fp, r0
                     +beq.w  loc_2c0
@@ steal arm
-beq.n  loc_22a      +beq.n  loc_232
-cbnz   r0, loc_1a4  +cbnz   r0, loc_1ac
-cbz    r0, loc_1ae  +cbz    r0, loc_1b6
-add    r1, pc, #436 +add    r1, pc, #428
@@ visible arm — the shuffle
-mov    sl, r5       (8 B)   +mov  r1, r8
-mov    r5, r6               +mov  r2, r6
-add    r6, sp, #96          +ldr  r3, [r0, #88]
-mov    r1, r8               +add  r0, sp, #96
-mov    r2, r5               +blx  r3
-ldr    r3, [r0, #88]        +ldr  r0, [r4, #0]   <- SZ guard
-mov    r0, r6               +cbz  r0, loc_2c0
-blx    r3                   +movs r0, #0
-movs   r0, #0               +add  r1, sp, #96
-strd   r0, r0, [sp, #32]    +strd r0, r0, [sp, #32]
-str    r0, [sp, #40]        +str  r0, [sp, #40]
-mov    r0, r6               +mov  r0, r6
-mov    r6, r5               +blx  ne
-mov    r5, sl
-mov    r1, r6
-blx    ne
@@ tail
-movs   r0, #1
```

The byte budget is exact: A2 guard **+8 B**, SZ guard **+4 B**,
visible-arm shuffle **−6 B**, missing `movs` **−2 B** → net 0, which is
why both bodies are 968 B and re-converge at `hitCamera` (`0x9f386`).

## 3. The visible arm is now provably reproducible

Source shape (no A2 guard, SZ guard *before* the call, **real** compare
order):

```c++
if (!object->isVisible())
    continue;
if (objects->empty())                 // redundant; splits the block
    continue;
Vector collision = object->projectCollisionOnSurface(this->geometry->getPosition());
if (collision != Vector{0.0f, 0.0f, 0.0f}) { … }
```

compiles to (objdump, our build):

```
9f30e: ldr  r1, [r5, #8]
9f310: mov  r0, r6
9f312: blx  getPosition
9f316: ldr.w r0, [r8]
9f31a: mov  sl, r5
9f31c: mov  r5, r6
9f31e: add  r6, sp, #96
9f320: mov  r1, r8
9f322: mov  r2, r5
9f324: ldr  r3, [r0, #88]
9f326: mov  r0, r6
9f328: blx  r3
9f32a: movs r0, #0
9f32c: strd r0, r0, [sp, #32]
9f330: str  r0, [sp, #40]
9f332: mov  r0, r6
9f334: mov  r6, r5
9f336: mov  r5, sl
9f338: mov  r1, r6
9f33a: blx  ne
```

which is the original at `0x9f300…0x9f32c` **verbatim** (only the base
address differs, by +14 B).  The `Vector{…} != collision` spelling
produces the *reversed* argument order and loses this match (89.0 %).

## 4. Why the shuffle cannot be paid back

The shuffle needs a *real branch* between the surface `getPosition()`
and the `projectCollisionOnSurface` call (an `llvm.expect` hint is not
enough — measured below).  That branch is the SZ guard, 6 B
(`ldr`+`cmp`+`beq.n`) at that position.  The shuffle itself costs
+6 B.  The original has neither the guard nor the extra spill, so it
is 14 B smaller than the smallest shuffle shape that keeps the
original register file; the landed body pays for its 968 B with the
A2 guard (8 B) instead.

Two independent extra costs show up in every shuffle shape:

* **+4 B dock-pointer spill.**  The original spills `r9` (=
  `&this->dockOffsetVec`) once in the preheader
  (`str.w r9,[sp,#20]`), and reloads it in the wormhole arm.  Our
  shuffle shapes sink that store into the wormhole arm *and* repeat it
  in the visible arm.  `__builtin_expect(object == nullptr, 0)` moves
  it back to the preheader (see `srn_null0` below) but simultaneously
  swaps `i` (→ `sl`) and `this` (→ `fp` in the shuffle).
* **−2 B missing `movs r0,#1`** in the tail (the landed body stores
  the loaded `field_0x71` instead of the constant).

## 5. Loop rotation: where it really happens

`-mllvm -print-after-all` shows the IR is **never** rotated:
`Rotate Loops` runs twice (early and late) and both dumps still have
`header: cmp; bcc body` at the top and `latch: i++; br header` at the
bottom.  The rotation is done by `MachineBlockPlacement::rotateLoop`
(LLVM 7), which moves the exiting header block to the bottom of the
chain.  `findBestLoopTop` returns the header unconditionally at `-Oz`
(`optForSize()`), so the rotation is decided entirely by
`findBestLoopExit` + `rotateLoop`'s chain-shape bail-outs.  The A2
guard's extra edge into the latch is what changes that decision.

## 6. Measurements (all 968 B unless stated; pct = verify.py)

```
landed (current)                                         95.500000  347/346
landed, real compare order (collision != Vector{})       95.200000
landed, tail `= 1`                                       87.800000  972
landed, top store `= f71`, tail `= 1`                    94.700000
landed, A2 = empty()/size()==0/size()<=i/i>=size()       95.500000
landed, SZ = size()==0                                  95.500000
landed, SZ = size()==i / i>=size()                       85.600000  976
landed, `i` as int                                       95.200000
landed, `i` as size_t / `++i`                            95.500000
landed, `const Vector collision` / `Vector&` alias       95.500000
landed, `auto`/`Vec3`/`AEMath::Vector` surface           95.500000
landed, `const Vector&`/`const Vector` surface           95.500000
landed, outerCollide(getPosition()) inline               95.500000
landed, `expect(object==nullptr,0)`                      90.900000  968
landed, `expect(!outerCollide,1)`                        95.500000
landed, `expect(outerCollide,0)`                         95.500000
landed, `expect(!isVisible,0)`                           66.900000  988
landed, `expect(strength>=1,1)`                          70.900000  948
landed, `expect(playing,0)`                              52.800000  972
landed, `expect(collision!=0,1)`                         58.000000 1004
landed, `expect(collision!=0,0)`                         57.300000 1012
landed, `expect(shr,1)`                                  87.400000  984
landed, `expect(A2,0)` / `expect(A2,1)`                  64.8 / 78.4  984/996
landed, SZ moved before isVisible                        88.100000  980
landed, SZ before call, real compare                     89.000000  980
landed, SZ before call, real compare, tail=1             89.300000  984
landed, SZ before call + expect(shr,1)                   88.400000  976
landed, SZ before call + expect(shr,1), real compare     89.200000  976
landed, same, no A2                                      89.200000  976
landed, same, no A2 + expect(null,0)  (`srn_null0`)      85.600000  972
landed, same, no A2 + expect(null,0) + tail=0 hint       85.100000  976
landed, same, no A2 + expect(null,0) + expect(loop,1)    79.300000  976
landed, no A2 (rotated!)                                 66.300000  976
landed, no A2 + tail=1                                   65.700000  976
landed, no A2, SZ before call (rotated!)                 59.600000  984
landed, no SZ (rotated!)                                 70.400000  988
landed, A2 before call                                   64.100000  988
landed, A2 before call, no SZ (rotated!)                 56.700000 1012
landed, A2 in steal arm                                  65.700000  972
landed, A2 after null / pre-hitCamera / post-hitCamera   66.3 / 65.5 / 66.3
landed, A2 after the compare                             66.300000  976
faithful (no guards, real tail `= 1`, rotated)           60.100000  996
faithful + expect(loop,1) (un-rotated, i=sl)             49.600000  964
faithful + expect(shr,1)                                 63.000000  988
faithful + expect(shr,1) + A2                            68.700000 1004
faithful + A2 + SZ after                                 75.600000  972
faithful + A2 + SZ before                                76.700000  984
faithful + A2 + SZ before + tail=f71                     77.200000  980
faithful + outerCollide(getPosition()) inline            60.100000  996
named `Vector surface` reused in the visible arm         58.800000 1012
named surface + no A2                                    52.100000  984
surface block without braces                             56.300000 1000
```

## 7. Dead ends (don't redo)

* **A2 guard expression**: `size()==i`, `size()==0`, `empty()`,
  `size()<=i`, `i>=size()` all compile to the same 8 B
  (`ldr`+`cmp`+`beq.w`) at the wormhole-arm position and all give
  95.5 %.  `cbz` cannot reach the latch (454 B away).
* **A2 guard position**: every position other than immediately after
  the `isShrinking() || miningGame` test either rotates the loop or
  changes the register file; the best alternatives are 65–66 %.
* **SZ guard position**: after the call (landed) is the only position
  that keeps 968 B; before the call gives the shuffle but 976–980 B.
* **`__builtin_expect` on every branch of the landed body** (loop,
  null, steal, getType, isVisible, outerCollide, isShrinking,
  strength, playing, distance, volatileGoods, collision, tail, A2,
  SZ): only `expect(!outerCollide,1)` is a no-op (95.5 %); everything
  else changes the register file or the layout and loses.
* **`__builtin_assume(i < objects->size())`** in the wormhole arm: no
  code, but does not restore the original register file (976 B, 66.3 %).
* **Type/qualifier spellings** of `surface` / `collision`
  (`const`, `const&`, `auto`, `Vec3`, `AEMath::Vector`, ctor form,
  reference alias): all byte-identical to the landed body (95.5 %).
* **Reusing a named `surface`** for the visible-arm `getPosition()`
  extends its lifetime and moves the stack slots (1008–1016 B).

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6963
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch harness (`/dev/shm/w6963`): `score.py` (pct/size/insn count),
`try.sh` (splice body + `link_so` + score), `ndiff.py` (unified
normalised diff), `regions.py` (opcode diff), `cc_ir.sh` (compile
`PlayerEgo.cpp` with arbitrary `-mllvm` flags), `gen1.py`–`gen3.py`
(variant generators), `body_cur.cpp` (landed body), `body_faithful.cpp`,
`v_*.cpp` (all measured variants).

## 9. Gate

`byte_exact 4505 / linked_exact 4505 / size-exact 4524 / data_exact
843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B, `.text` 1,259,464 B.
