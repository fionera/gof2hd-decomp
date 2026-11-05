# PlayerEgo::calcCollision — wave 114 (w6211): exact block *order* at 968 B, RA swap isolated

**Status:** research note, 2026-11-05. Worktree `wt/w6211`, branch
`wave114-w6211`, base `d8557f7e2`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B shape is still the best measured.
This note extends waves 107–113
(`playerego-calccollision-wave107-w5513.md` … `-wave113-w6110.md`).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed two-anchor (`A1` = `if (i > objects->size() - 1) continue;`,
  `A2` = `if (objects->size() == i) continue;`) *continue* body still
  scores **87.6436781609 %** (968 B, 347/349 normalised instructions).
  ~150 new builds this wave find nothing above it.
* **New: the original's block *order* is reproducible at exactly 968 B.**
  Changing only the `A1` spelling from `i > objects->size() - 1` to
  `i >= objects->size()` (and dropping `A2`) gives **87.3 % / 968 B /
  347/345** with the latch at `0x9f38c` and the cold wormhole blocks
  after it, i.e. the original's layout order.  The residual delta is a
  pure register swap: `i` lands in `r9` and `&dockOffsetVec` in `fp`
  (original: `i`=`fp`, `&dockOffsetVec`=`r9`).
* **New: the RA swap is controlled by a single `objects->size()` load in
  the wormhole arm.**  Putting `A2` back (or any `objects->size()`
  compare, e.g. `objects->size() == 0`, `objects->size() <= i`) restores
  the original register file (`i`=`fp`, `&dockOffsetVec`=`r9`) but costs
  8 B → 976 B / 85.5 %.  A compare that does *not* reload
  `objects->size()` (`i == 0xffffffffu`) does **not** fix the RA.
* **New (mechanism):** the latch placement is decided by
  `MachineBlockPlacement::selectBestSuccessor`'s tie-break.  At 50/50 it
  keeps the *first* successor, and `MachineBasicBlock::successors()` is
  ordered `[fall-through, branch-target]`.  `i > size - 1` compiles to
  `subs r0,#1; cmp i,r0; bls getType` (fall-through = latch) → the latch
  is chained right after the A1 block.  `i >= size` compiles to
  `cmp i,r0; bcs latch` (fall-through = getType) → the latch is chained
  last, matching the original.
* **New (mechanism):** the anchor-free shapes (`L0`, REA `temp`/`bool`)
  have BranchFolder `BFCHANGED=1`, which re-runs `buildCFGChains()` and
  rotates the loop (`bottomIsSuccTop=1` → `ROT ROTATING`, header moved to
  the end, 988–992 B).  The landed / `M1` / `L3` shapes have
  `BFCHANGED=0` and the chain bottom is the wormhole continuation
  (`bb.22`, `bottomIsSuccTop=0` → `ROT no-backedge bail`), so no
  rotation.

## 1. Exact scores (this wave)

```
landed (A1 = i > size-1, A2, no i0)        pct=87.6436781609  size=968  lens=347/349
M1 (A1 = i >= size, no A2, no i0)          pct=87.3000000000  size=968  lens=347/345
L3 (A1 = i >= size, A2, no i0)             pct=85.5000000000  size=976  lens=347/348
L3a (A1 = i >= size, A2, i0)               pct=85.7000000000  size=984  lens=347/351
landed + i0                                pct=85.0000000000  size=976  lens=347/352
landed + i0, A2 removed                    pct=82.6000000000  size=968  lens=347/348
landed, A2 removed                         pct=79.5000000000  size=964  lens=347/347
landed, A1 removed                         pct=70.8000000000  size=984  lens=347/351
landed, both anchors removed               pct=59.8000000000  size=988  lens=347/349
M1 + i0                                    pct=79.4000000000  size=976  lens=347/348
M1 + A2 (= L3)                             pct=85.5000000000  size=976  lens=347/348
REA if/else wide, i0                       pct=64.1000000000  size=968  lens=347/349
REA if/else temp, i0                       pct=59.7000000000  size=992  lens=347/350
REA if/else temp, i0, null-continue        pct=64.1000000000  size=968  lens=347/349
REA if/else bool, i0                       pct=59.7000000000  size=992  lens=347/350
```

`i0` = the original's `if (i == 0 && object->field_0x71)` guard in the
visible arm (the landed body is missing it).

## 2. The A1 spelling selects the block order

`M1` is the landed body with

```c++
        if (i > objects->size() - 1)     // landed
            continue;
```
replaced by
```c++
        if (i >= objects->size())        // M1
            continue;
```
and the `A2` block (`if (objects->size() == i) continue;`, inside the
wormhole arm) deleted.

The two spellings produce the same *semantics* but different
fall-throughs:

| | landed | M1 |
|---|---|---|
| A1 code | `ldr r0,[r4]; subs r0,#1; cmp i,r0; bls getType` | `ldr r0,[r4]; cmp i,r0; bcs latch` |
| A1 fall-through | latch | getType |
| MBP successors | `[bb.39(latch), bb.13(getType)]` 50/50 | `[bb.13(getType), bb.39(latch)]` 50/50 |
| tie-break picks | latch → latch chained after A1 | getType → latch chained last |
| latch address | `0x9f192` | `0x9f38c` (original) |

`MachineBlockPlacement::selectBestSuccessor` does
`if (BestSucc.BB && BestProb >= SuccProb) continue;`, so on an exact tie
the first successor in `collectViableSuccessors` order wins, and that
order is the terminator's `[fall-through, target]` order.  Hence the
branch *spelling* alone decides whether the latch is placed early or
last.

`M1`'s branch targets that match the original exactly:
`bcs.w 0x9f386` (loop exit), `beq.w 0x9f2c0` (null → latch),
`beq.n 0x9f2ba` (volatileGoods → hitCamera).  The blocks between the A1
anchor and the latch are shifted by the anchor's 10 B, and the latch's
`b.n` target is `loc_64` vs the original's `loc_68`.

## 3. The RA swap and the `objects->size()` load

`M1` register file: `r4`=objects, `r5`=this, `r6`=surface, `r8`=object,
`r9`=**i**, `r10`(sl)=distance, `r11`(fp)=**&dockOffsetVec**.
Original: `r9`=**&dockOffsetVec**, `r11`(fp)=**i**.

`-debug-only=regalloc` on the two shapes (LLVM 7, standalone function):

```
landed:  assigning %72 to $r11 ... unassigning %72 from $r11
         assigning %168 to $r11        ; loop PHI (i) -> fp
M1:      assigning %70 to $r11 ... unassigning %70 from $r11
         assigning %166 to $r9         ; loop PHI (i) -> r9
```

The first nine callee-saved assignments are identical in both
(`%10`→r4, `%9`→r5, `%2`→r6→r8, `%1`→r6→r9, `%0`→r6→r10, `%32`→r6);
only the loop-PHI assignment differs.  Adding **any** extra
`objects->size()` load inside the wormhole arm flips the PHI back to
`r11` and restores the original register file:

```
M1 + if (objects->size() == i) continue;   pct=85.5  size=976  (== L3)
M1 + if (objects->size() == 0) continue;   pct=85.5  size=976
M1 + if (objects->size() <= i) continue;   pct=85.5  size=976
M1 + if (objects->size() <  i) continue;   pct=85.5  size=976
M1 + if (i == 0xffffffffu) continue;       pct=87.3  size=968  (RA still swapped)
```

So it is the `ldr r0,[r4]` (a use of `objects`, not of `i`) in the cold
wormhole arm that tips the spill-weight ordering, not the compare
itself.  `objects->size()` is exactly what the original does *not* do
there, so the original's RA must come from a different source shape that
this wave did not find.

## 4. MBP / BranchFolder trace (mechanism)

The patched LLVM build at `/tmp/llvm_android_build` (7.0.0svn, with
`MBPTRACE` instrumentation) reproduces the real clang 7.0.2 `-Oz` layout
**only with `-mattr=+neon,+thumb-mode`** — without `+thumb-mode` llc
emits ARM and the layout differs.

```
landed / M1 / L1 / L2 / L3:
  FBLX header=%bb.4 nblocks=36 ...
  ROT enter top=%bb.4 bottom=%bb.22 exit=%bb.4
  ROT viableTop=1
  ROT bottom succ=%bb.39 inloop=1 chainbegin=%bb.4
  ROT next=%bb.5 exitIsSuccNext=1 bottomIsSuccTop=0
  ROT no-backedge bail
  BFCHANGED=0            (single buildCFGChains pass)

L0 (no anchors):
  ... ROT no-backedge bail
  BFCHANGED=1            -> second buildCFGChains
  ... ROT enter top=%bb.4 bottom=%bb.36 exit=%bb.4
  ROT bottomIsSuccTop=1
  ROT ROTATING           (header moved to the end, 988 B)

REA temp / REA bool:
  BFCHANGED=0
  ROT bottomIsSuccTop=1
  ROT ROTATING           (992 B)
```

`rotateLoop` bails only when
`ViableTopFallthrough && ExitingBB->isSuccessor(NextBlockInChain) &&
!Bottom->isSuccessor(Top)`; i.e. it bails when the chain bottom is *not*
the latch.  In the landed/M1/L3 shapes the chain bottom is the wormhole
continuation `bb.22` (which branches to the latch `bb.39`), so no
rotation.  In the REA temp/bool shapes the chain bottom *is* the latch,
so the loop is rotated.

## 5. Other measured dead ends this wave

| variant | pct | size | note |
|---|---:|---:|---|
| M1, A1 = `objects->size() <= i` | 87.3 | 968 | codegen-identical to M1 |
| M1, A1 = `i != objects->size()` | 87.0 | 968 | `bne` instead of `bcs` |
| M1, A1 = `i == objects->size()` | 79.5 | 960 | |
| M1, A1 = `i + 1 > objects->size()` | 79.1 | 964 | |
| M1, A1 = `objects->size() - 1 < i` | 79.5 | 964 | |
| M1, A1 = `objects->size() == 0` | 65.5 | 996 | no `i` use |
| M1, `int i` | 87.3 | 968 | codegen-identical |
| M1, `!i` / `0 == i` / `i < 1` / `i == 0u` | 87.3 | 968 | codegen-identical |
| M1, `i` declared outside the `for` | 87.3 | 968 | codegen-identical |
| M1, A1 hoisted before the surface block | 87.3 | 968 | codegen-identical |
| M1, `Vector &dock = this->dockOffsetVec` | 87.3 | 968 | codegen-identical |
| M1, `const Vector &dock = ...` | 87.3 | 968 | codegen-identical |
| M1, `for (...; i != size; ...)` | 87.0 | 968 | |
| M1, `while` loop | 66.2 | 988 | |
| M1, `do/while` loop | 31.7 | 976 | |
| M1, i0 check moved after the surface block | 73.2 | 976 | |
| M1, i0 operand order W1/W2/W3/W4 | 72.5/72.8/57.0/76.7 | 964/984/1016/976 | |
| M1, `VectorLength(dock)` twice | 78.8 | 988 | |
| M1, redundant check in the steal arm | 79.1 | 976 | |
| M1, redundant check in the visible arm | 72.0 | 992 | |
| M1, redundant check at the end of the wormhole arm | 79.5 | 964 | |
| M1, redundant check after the surface block | 87.3 | 968 | folded into A1 |
| M1, surface as an inline temporary | 87.3 | 968 | same as the narrow block |
| M1, `if (oc) {} else { continue; }` | 59.8 | 988 | |
| M1, body inside `if (oc)` + `else { continue; }` | 58.6 | 960 | |
| M1, i0 check inside the `outerCollide` if | 56.3 | 992 | |
| landed + `dockOffsetVec.x` use at loop end | 87.6 | 968 | codegen-identical |
| landed + `VectorLength(dockOffsetVec)` at loop end | 84.0 | 980 | |
| landed + `Vector surface` at function scope | 47.0 | 1028 | |
| landed + `Vector surface` at loop scope | 56.0 | 988 | |
| landed + i0, A1 = `size-1 < i` | 85.0 | 976 | |
| landed + i0, A1 = `size <= i` | 85.7 | 984 | |
| landed + i0, A1 = `i+1 > size` | 85.1 | 976 | |
| landed + i0, A1 = `size < i+1` | 85.1 | 976 | |
| REA wide i0=False | 58.6 | 960 | |
| REA temp i0=False | 59.7 | 984 | |
| cont wide/temp/bool i0 nc=True | 64.1/59.7/59.7 | 968/992/992 | |
| cont wide/temp/bool i0=False nc=True | 58.6/59.7/59.7 | 960/984/984 | |
| cont wide/temp/bool i0 nc=False | 58.7 | 980 | |
| cont wide/temp/bool i0=False nc=False | 55.0 | 976 | |

## 6. Proven vs. hypothesis

**Proven (measured):**

* The landed body's exact score is 87.6436781609 % (347 vs 349
  normalised instructions, 968 B); ~150 new variants this wave find no
  better point.
* Changing only `A1` from `i > objects->size() - 1` to
  `i >= objects->size()` (and dropping `A2`) reproduces the original's
  block *order* (latch `0x9f38c`, cold wormhole blocks after it) at
  exactly 968 B / 87.3 %, with all of the original's branch targets
  matching except the 10 B shift introduced by the anchor itself.
* That shape's residual delta is a pure `i` ↔ `&dockOffsetVec` register
  swap (`i`=r9, `dock`=fp instead of `i`=fp, `dock`=r9).
* Any extra `objects->size()` load in the wormhole arm restores the
  original register file, at a cost of 8 B (976 B / 85.5 %).
* The latch placement is decided by MBP's 50/50 tie-break over the
  `[fall-through, target]` successor order, and the anchor-free shapes
  additionally hit `BFCHANGED=1` → a second `buildCFGChains()` that
  rotates the loop.

**Hypothesis (not proven):**

* The original source has the narrow `Vector surface` scope (frame 112,
  `surface` sharing `sp#32` with the 60-byte Matrix temp) *and* a
  natural construct in the wormhole arm that reloads `objects->size()`
  (or otherwise re-weights `objects`), which this wave did not
  reconstruct.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6211
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice the body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6211/score.py                         # pct/size/lens
python3 /dev/shm/w6211/score.py -d                      # normalised diff
```

Layout trace (real clang 7.0.2, block order after MBP):

```sh
CC=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CC <flags from build.ninja> -mllvm -print-after-all \
    -mllvm -filter-print-funcs=_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE \
    -c src/game/ship/PlayerEgo.cpp -o /dev/null 2> mir.txt
# then read the bb.* order after
#   "*** IR Dump After Branch Probability Basic Block Placement ***"
```

MBP / BranchFolder trace (patched LLVM 7.0.0svn, **must** pass
`+thumb-mode` or the layout differs from the real build):

```sh
MBP_TRACE=1 /tmp/llvm_android_build/bin/llc \
  -mtriple=armv7-none-linux-androideabi -mattr=+neon,+thumb-mode \
  -float-abi=soft -O2 fn.ll -o /dev/null 2>&1 | grep calcCollision
```

Register-allocator trace (per-function, `-debug-only=regalloc`):

```sh
/tmp/llvm_android_build/bin/llc \
  -mtriple=armv7-none-linux-androideabi -mattr=+neon,+thumb-mode \
  -float-abi=soft -O2 -debug-only=regalloc fn.ll -o /dev/null 2>&1 \
  | grep -E '^(assigning|unassigning) %'
```

Scratch harness (`/dev/shm/w6211`): `score.py` (single-function
score/diff), `splice.py`, `emitll.sh`, `layout.sh`, `extract_fn.py`,
`gen.py`, `sweep1.py`, and the body files (`body_landed.txt`,
`body_M1.txt`, `body_L3.txt`, `body_L3a.txt`, `body_rea.txt`,
`body_temp2.txt`, `body_X1.txt`, `body_BB1.txt`, `body_ZZ*` sweeps).

## 9. Follow-up sweep (w6211, same wave)

A second sweep (~40 more builds) on the faithful `L0+i0` shape (no
redundant anchors, `i >= size` spelling, visible-arm `i == 0` guard)
confirms the anchor/RA trade-off and rules out several source-shape
explanations:

| variant (base `L0+i0` unless stated) | pct | size | note |
|---|---:|---:|---|
| `i == objects->size()` anchor after `outerCollide` | 82.3 | 968 | RA swapped (`i`=r9) |
| `objects->size() - 1 < i` anchor | 82.6 | 968 | RA swapped |
| `i > objects->size() - 1` anchor | 82.6 | 968 | RA swapped |
| `i >= objects->size()` anchor (= `M1+i0`) | 79.4 | 976 | RA swapped |
| `i + 1 > size` / `size < i + 1` anchors | 79.3 | 972 | |
| `i >= size` / `i == size` as `break` | 68.3 | 964 | |
| `objects->size() == 0` anchor after `outerCollide` | 65.2 | 1004 | |
| `objects->size() == i` in wormhole arm (= `L1+i0`) | 70.7 | 988 | rotation |
| `objects->size() == 0` in wormhole arm | 70.7 | 988 | |
| `objects->size() == 0` before loop | 72.3 | 992 | |
| `objects->size() == 0` at loop top | 79.4 | 976 | |
| `objects->size() == 0` in steal arm | 79.1 | 984 | |
| `objects->size() == 0` in visible arm | 76.2 | 988 | |
| `objects->size() == 0` at loop end | 79.4 | 976 | |
| `objects->size() == 0` in wormhole arm | 85.7 | 984 | best M1-family |
| `Vector surface` at function scope | 44.7 | 1028 | |
| `Vector surface` at loop scope | 58.4 | 984 | |
| `Vector surface` initialized before null check | 59.6 | 964 | |
| `surface` uninitialized before null check | 58.4 | 984 | |
| `objects->count` instead of `size()` (all shapes) | same | same | codegen-identical |
| `objects->data_[i]` instead of `(*objects)[i]` | same | same | codegen-identical |
| `bool first = (i == 0)` + reuse in both guards | same | same | codegen-identical |
| nested / reversed `i == 0 && field_0x71` | same | same | codegen-identical |
| visible-arm `i0` moved outside `if (collision)` | 68.4 | 992 | |

The `L0+i0` MBP trace shows why the anchor is needed: the first
`buildCFGChains()` bottoms the chain at `bb.20` with
`bottomIsSuccTop=0` (no rotation), then `BFCHANGED=1` re-runs
`buildCFGChains()`, the chain bottom becomes the latch `bb.37`, and
`bottomIsSuccTop=1` triggers `ROT ROTATING`.  The redundant
`objects->size()` load in the wormhole arm is the only construct found
that both restores the original register file (`i`=fp, dock=r9) and
keeps `BFCHANGED=0`; it costs 8 B.  The landed two-anchor body remains
the best measured point at 87.6436781609 % / 968 B.

## 8. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0,
`RATCHET PASS`.  27 sections exact, file size 2,187,328 B.  No source
change, so `tools/verify_baseline.json` is untouched.
