# PlayerEgo::calcCollision — wave 113 (w6110): if/else shape + stack-slot audit, no new best

**Status:** research note, 2026-11-05. Worktree `wt/w6110`, branch
`wave113-w6110`, base `a979ace18`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B shape is still the best measured.
This note extends the wave-107…112 fixed-point notes
(`playerego-calccollision-wave107-w5513.md` … `-wave112-w6008.md`).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed two-anchor (`A1`/`A2`) *continue* body still scores
  **87.6436781609 %** exactly (968 B, 347/349 normalised instructions).
  ~110 new builds this wave (the full REA if/else shape, its
  nullptr/outerCollide wrapper permutations, three surface-scope
  variants, `__builtin_expect` anchors, loop forms, arm order, and
  `distance`-liveness perturbations) produce nothing above it.
* **New: the original is the REA if/else shape, not the continue
  shape.**  Reconstructing the exact Ghidra/REA control flow
  (`if (object != nullptr) { … if (outerCollide(surface)) { if
  (wormhole) … else if (stealFlagByte == 0) … else … } }`, with a
  shared `hitCamera()` tail) gives **968 B, 347/349, 64.1 %** and the
  original's *exact block layout*: loop header at `0x9f130`, wormhole
  arm `0x9f1a2`, steal arm `0x9f248`, visible arm `0x9f2fc`, shared
  `hitCamera` at `0x9f386`, latch at `0x9f38c`, then the cold wormhole
  blocks `0x9f392…0x9f450`.  The landed continue body puts the latch at
  `0x9f192` instead.
* **New: the remaining delta is a stack-slot / RA chain, not control
  flow.**  The original frame is **112 B** and `Vector surface` shares
  `sp#32` with `Vector velocity`/the zero vector/the 60-byte Matrix
  temp.  The REA if/else body is **120 B** with `surface` at `sp#104`,
  because `Vector surface` is declared in the `if (object != nullptr)`
  scope and its `llvm.lifetime.end` lands at the loop latch
  (IR `%242`), so `StackColoring` cannot merge it with the Matrix.
  Narrow-scope surface variants give the original **112 B** frame but
  trigger a post-PEI block-placement loop rotation (condition at the
  bottom, 992 B), which the original does not have.
* **New: `__builtin_expect` fixes the rotation but not the RA.**
  `__builtin_expect(object->isVisible(), 1)` (or the same on
  `outerCollide`) restores the original top-tested layout at **112 B /
  960 B / 347 instructions / 69.7 %**, but the allocator then gives
  `surface=sl`, `distance=r6`, hoists `movw r6,#16994` into the
  preheader, and keeps `collision=r9` — not the original
  `surface=r6, distance=sl, strength=r9, constant inside the loop`.
* The original's collision-arm shuffle (`mov sl,r5; mov r5,r6; add
  r6,sp,#96` … `mov r6,r5; mov r5,sl`) is a consequence of
  `surface=r6` + `object=r8`; the landed body has both but uses `sl`
  for the collision sret because `distance`'s interval does not cover
  the collision block.

## 1. Exact scores (unrounded, this wave)

```
landed (A1+A2, guard w/o i==0)          pct=87.6436781609  size=968  lens=347/349
REA if/else (named surface, i==0 guard) pct=64.0804597701  size=968  lens=347/349
REA if/else + continue-nullptr (hybrid) pct=64.0804597701  size=968  lens=347/349
REA if/else, temp surface               pct=59.7701149425  size=992  lens=347/350
REA if/else, bool-narrow surface        pct=59.7701149425  size=992  lens=347/350
REA if/else, narrow-block + continue    pct=59.7701149425  size=992  lens=347/350
REA if/else, temp + expect(isVisible,1) pct=69.7             size=960  lens=347/347
REA if/else, temp + expect(outerColl,1) pct=68.0             size=960  lens=347/347
REA if/else, temp + expect(outerColl,0) pct=57.7             size=940  lens=347/346
landed + i==0 guard (A1+A2 kept)        pct=84.9785407725  size=976  lens=347/352
landed + i==0, A2 removed               pct=82.6             size=968  lens=347/348
landed + i==0, A1 removed               pct=70.7             size=988  lens=347/352
landed, A2 removed                      pct=79.5             size=964  lens=347/347
landed, A1 removed                      pct=70.8             size=984  lens=347/351
landed, both removed                    pct=59.8             size=988  lens=347/349
```

## 2. The original is the REA if/else shape

Ghidra/REA renders the body as an if/else chain with a shared
`hitCamera()` tail, not a sequence of `continue`s.  Reconstructing it
verbatim (the `if (object != nullptr)` wrapper, `Vector surface =
geometry->getPosition()`, `if (outerCollide(surface))`, then
`if (getType()==16994 && isVisible()) … else if (stealFlagByte == 0) …
else …`, with `i == 0 &&` in the bottom `collidesWithStationFlag`
guard) reproduces the original's block order byte-for-byte up to
register names:

| block | original | REA if/else |
|---|---|---|
| loop header | `0x9f130` | `0x9f130` |
| wormhole arm | `0x9f1a2` | `0x9f1a2` |
| steal arm | `0x9f244` | `0x9f248` |
| visible arm | `0x9f2f6` | `0x9f2fc` |
| shared hitCamera | `0x9f386` | `0x9f386` |
| latch | `0x9f38c` | `0x9f38c` |
| cold wormhole W2/W3/W4/W5 | `0x9f392…0x9f450` | `0x9f392…0x9f450` |

The landed continue body instead places the latch at `0x9f192`
(immediately after the `A1` block), which is why every `continue`
branch target differs from the original.

## 3. Stack-slot / frame audit (new)

Original frame **112 B**.  Stack slots (from the disassembly):

```
sp#0   bool arg (updateEvent3DAttributes)
sp#8   &moveDirection
sp#12  &moveTarget
sp#16  &Globals::sound
sp#20  &dockOffsetVec (spilled)
sp#24  &__stack_chk_guard (spilled)
sp#28  &Globals::status
sp#32  Vector surface  / Vector velocity / zero vector / 60-byte Matrix temp
sp#96  Vector collision / Vector position
sp#108 guard value
```

`surface` shares `sp#32` with the Matrix temp, so its live range must
end before the Matrix is used.  The REA if/else body is **120 B**
because `surface` gets its own 12-byte slot at `sp#104`:

```
sp#32  60-byte Matrix temp
sp#92  Vector collision / position
sp#104 Vector surface
sp#116 guard value
```

`-mllvm -stop-before=stack-coloring` MIR shows why: in the REA if/else
`LIFETIME_START %stack.1` is in the surface block but
`LIFETIME_END %stack.1` is in the loop latch block (`%bb.42`), because
`Vector surface` is declared in the `if (object != nullptr)` scope.
The narrow-scope variants (temp, `bool` + nested block, nested block +
`continue`) move `LIFETIME_END` next to the `outerCollide` call and
give the original **112 B** frame — but then the post-PEI
`block-placement` pass rotates the loop (condition at the bottom,
`992 B`).

The pass order (NDK clang `-mllvm -debug-pass=Structure`) is
`slotindexes -stack-coloring … -greedy … -stack-slot-coloring
-prologepilog … -block-placement`.  StackColoring runs on the IR block
order (before RA/block placement), so the surface lifetime is decided
by the *source scope*, not the final layout.

## 4. `__builtin_expect` restores the layout, not the RA

The only way found to keep the narrow 112-B frame *and* the original
top-tested layout is a branch-weight anchor.  `__builtin_expect(
object->isVisible(), 1)` on the visible-arm test (or
`__builtin_expect(object->outerCollide(...), 1)`) gives:

```
frame 112, size 960, 347/347 instructions, pct 69.7
prologue: this=r5 objects=r4 object=r8 i=fp surface=sl collision=r9
          movw r6,#16994   (hoisted constant)
```

The original instead has `surface=r6`, `distance=sl`, `strength=r9`,
`collision` reusing `r6`, and `movw r1,#16994` *inside* the loop.  The
hoisted constant occupies `r6`, so `surface` falls to `sl`; the RA is
a fixed point that no source spelling tried here breaks.

## 5. Other measured dead ends this wave

| variant | pct | size | note |
|---|---:|---:|---|
| REA if/else + A1/A2 in the wormhole arm | 56.9 | 1016 | |
| REA if/else + `Vector surface` at function scope | 44.7 | 1028 | `this=r8`, frame 128 |
| REA if/else + `Vector surface` at loop scope | 58.4 | 984 | frame 120, top-tested |
| REA if/else + `const`/ctor/`auto` surface | 64.1 | 968 | codegen-identical |
| REA if/else + `Vector surface; surface = …` | 59.5 | 984 | |
| REA if/else + surface reused as velocity/zero | 50.7 | 1016 | |
| REA if/else + arm order swapped | 64.1 | 968 | codegen-identical |
| REA if/else + `do/while` | 55.1 | 960 | |
| REA if/else + `while` / `for(;;)` | 69.7 | 960 | same as temp+expect |
| landed + `__builtin_expect` on loop cond | 77.7 | 968 | |
| landed + `__builtin_expect(object==nullptr,0)` | 81.2 | 964 | |
| landed + `__builtin_expect` on A1 | 70.5 | 992 | |
| landed + `__builtin_expect` on A2 | 81.6 | 968 | |
| landed + `__builtin_expect` on isVisible | 71.0 | 1008 | |
| landed + `__builtin_expect` on strength | 78.4 | 984 | |
| landed + `__builtin_expect` on getType | 51.9 | 1028 | |
| landed + `__builtin_expect` on stealFlag | 54.4 | 1016 | |
| landed + `distance` hoisted + dead use | 87.6 | 968 | codegen-identical |
| landed + `dockOffsetVec` use at loop end | 87.6 | 968 | codegen-identical |
| landed + `Vector collision(...)` copy-ctor | 87.6 | 968 | codegen-identical |
| landed + named `position` in collision arm | 57.7 | 996 | |
| landed + `Vector collision; collision = …` | 72.1 | 1012 | |
| landed + `hitCamera` inside `if (isVisible)` | 80.5 | 988 | |

## 6. Proven vs. hypothesis

**Proven (measured):**

* The landed body's exact score is 87.6436781609 % (347 vs 349
  normalised instructions, 968 B); ~110 new variants this wave find no
  better point.
* The original's control flow is the REA if/else shape: the REA body
  reproduces the original's block layout exactly (latch `0x9f38c`,
  shared `hitCamera` `0x9f386`, cold wormhole blocks after the latch)
  at 968 B / 64.1 %.
* The original frame is 112 B with `surface` sharing `sp#32` with the
  Matrix/velocity/zero vector; the REA if/else body is 120 B with
  `surface` at `sp#104` because its `lifetime.end` is at the loop
  latch.
* Narrow-scope surface variants give the 112-B frame but trigger a
  post-PEI loop rotation (992 B); `__builtin_expect` on
  `isVisible`/`outerCollide` restores the top-tested layout at 960 B
  but leaves `surface=sl`, `distance=r6`, and a hoisted `#16994`.

**Hypothesis (not proven):**

* The original source is the REA if/else body with a narrow-scope
  `Vector surface` *and* a branch-weight hint that keeps the loop
  top-tested, compiled with an RA state that spills
  `&__stack_chk_guard` and gives `surface=r6` — a state no source
  spelling tried in waves 107–113 reproduces without adding code.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6110
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice the body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6110/harness.py                       # pct/size/lens
python3 /dev/shm/w6110/harness.py -d                    # normalised diff
```

Stack-slot audit (single function):

```sh
cd cmake-build-match
CMD=$(grep -m1 -A2 "build CMakeFiles/gof2.dir/src/game/ship/PlayerEgo.cpp.obj" \
      build.ninja | grep "FLAGS" | sed 's/^  FLAGS = //')
CC=../.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CC $CMD -mllvm -stop-before=stack-coloring \
    -mllvm -filter-print-funcs=_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE \
    -c ../src/game/ship/PlayerEgo.cpp -o /dev/shm/w6110/prestack.mir
# inspect LIFETIME_START/END and the `stack:` object list
```

Scratch harness (`/dev/shm/w6110`): `harness.py` (single-function
score/diff), `splice.py`, `try.sh`, `mirstack.sh`, `stacks.sh`,
`gen.py` (96-variant if/else cross product), and the body files
(`body_natural.txt`, `body_hybrid.txt`, `body_temp.txt`,
`body_bool.txt`, `body_v1.txt`, `body_t2vis.txt`, `body_fs.txt`,
`body_ls.txt`, the landed `PlayerEgo.cpp.landed`, and the `body_L*`,
`body_A*`, `body_p_*`, `body_a2_*`, `body_t2vis*` sweeps).

## 8. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0,
`RATCHET PASS`.  27 sections exact, file size 2,187,328 B.  No source
change, so `tools/verify_baseline.json` is untouched.
