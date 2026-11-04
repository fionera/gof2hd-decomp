# PlayerEgo::calcCollision — wave 102 (wf211): the wrap form is the original's
# *block layout*, but its `dock`/`i` register pair cannot be un-swapped

**Status:** research note, 2026-11-04. Worktree `wt/wf211`, branch
`wave102-wf211`, base master `53d9ee4d5`. **No source change landed** — the
checked-in 87.6 % / 968 B shape is still the best measured. This note adds
~200 new measured source variants (mostly around the pass-10/11 *wrap* form)
and pins the remaining gap to a single register-allocation fixed point.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B,
347 instructions, 87.6 %).

## TL;DR

The original is a **top-tested, un-rotated** loop with the latch **after**
`hitCamera` (`0x9f38c: add.w fp,fp,#1; b.n 0x9f134`), **one** `objects->size()`
load per iteration (header only), the real bottom guard
`if (i == 0 && object->field_0x71)` (`0x9f374`), `Vector surface` sharing the
`sp+32` Matrix/velocity scratch slot (frame 112), and the register file
`this=r5 objects=r4 object=r8 dock=r9 i=fp surface=r6`.

The pass-10/11 *wrap* form (`if (i < objects->size()) { …body… }` after the
`outerCollide` block) reproduces that **exact block layout** at 968 B and
98 % mnemonic identity — but the allocator puts `dock` in `fp` and `i` in
`r9` (swapped). The committed two-anchor shape gets `dock=r9 i=fp` right but
pays 8 instructions of redundant bounds checks and moves the latch to the
front. No source spelling found this wave gets **both**.

## 1. What is proven about the original (from the binary)

All re-confirmed by manual Thumb disassembly of
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`0x9f0cc`–`0x9f492`):

* one `[r4,#0]` size load (`0x9f134`) and one `[r4,#4]` data load
  (`0x9f13c`) — **no redundant size checks anywhere in the loop**;
* exactly two `cmp fp,#0`: `0x9f14a` (top guard) and `0x9f374` (bottom
  guard, `if (i == 0 && object->field_0x71)`);
* `&dockOffsetVec` computed once in the preheader (`0x9f110 add.w r9,r5,#328`)
  and spilled once (`0x9f12a str.w r9,[sp,#20]`), then reloaded inline at
  `0x9f362`, `0x9f392`, `0x9f44c` — **no shared r9-restore block**;
* `sp+32` is the shared 12-byte scratch vector: `surface` (`0x9f17c`),
  wormhole `velocity` (`0x9f21e`), steal `impact` (`0x9f290`), collision
  `getPosition` temp (`0x9f302`) and the `ml` Matrix result (`0x9f360`) all
  alias it, so `surface`'s lifetime ends at `outerCollide` and the frame is
  **112**;
* `hitCamera` is one tail-merged block (`0x9f386`) reached from the steal
  arm (`0x9f2f4 b.n`) and the collision arm (`0x9f330 cbz` / fallthrough);
* the literal pool is `16000.0f` (`0x9f470`), `0.2f` (`0x9f474`), `0.1f`
  (`0x9f478`), then the GOT slots for `__stack_chk_guard`, `Globals::status`,
  `Globals::sound`, `Globals::canvas` ×2.

## 2. Measured variant table (official `tools/verify/verify.py`)

All bodies were spliced into `src/game/ship/PlayerEgo.cpp`, built with
`cmake --build cmake-build-match --target link_so -j8`, and scored with
`python3 tools/verify/verify.py … --show …`. `frame`/registers are from the
objdump of `0x9f0cc`.

| variant | pct | size | frame | this | dock | obj | i | surface | note |
|---|---:|---:|---:|---|---|---|---|---|---|
| **checked-in** (2 anchors, no `i==0`) | **87.6** | 968 | 112 | r5 | r9 | r8 | fp | r6 | best |
| checked-in + real `i==0` guard | 85.0 | 976 | 112 | r5 | r9 | r8 | fp | r6 | |
| checked-in + guard, `i>=size` anchors | 85.7 | 984 | 112 | r5 | r9 | r8 | fp | r6 | |
| checked-in, `bool first = i==0` + guard | 85.0 | 976 | 112 | r5 | r9 | r8 | fp | r6 | |
| **wrap** (`if (i<size)`) no guard | 83.5 | 968 | 112 | r5 | **r9** | r8 | **sl** | r6 | |
| **wrap + `if (field_0x71)` guard** | **87.3** | 968 | 112 | r5 | **fp** | r8 | **r9** | r6 | 98 % mnemonic |
| wrap + `i==0&&field_0x71` guard | 79.4 | 976 | 112 | r5 | fp | r8 | r9 | r6 | |
| wrap, `objects->size() > i` | 83.5 | 968 | 112 | r5 | r9 | r8 | sl | r6 | |
| wrap + `bool first` + guard | 87.3 | 968 | 112 | r5 | fp | r8 | r9 | r6 | |
| wrap + `Vector &dock` local | 87.3 | 968 | 112 | r5 | fp | r8 | r9 | r6 | |
| wrap + `Array *objs` local | 87.3 | 968 | 112 | r5 | fp | r8 | r9 | r6 | |
| wrap + guard + `size()==i` in wormhole | 85.5 | 976 | 112 | r5 | **r9** | r8 | **fp** | r6 | |
| wrap + guard + `size()==i` + `i==0` | 85.7 | 984 | 112 | r5 | r9 | r8 | fp | r6 | |
| wrap **before collision arm**, no guard | 67.9 | 956 | 112 | r5 | **r9** | r8 | **fp** | **sl** | right pair, wrong surf |
| wrap before collision + guard | 66.9 | 960 | 112 | r5 | r9 | r8 | sl | r6 | |
| wrap before steal arm | 48.1 | 956 | 112 | r5 | r0 | r8 | fp | – | |
| wrap after top guard | 58.9 | 964 | 112 | fp | sl | r8 | r9 | r5 | |
| wrap before surface block | 60.7 | 976 | 120 | r5 | r9 | r8 | sl | – | |
| wrap on `miningGame==0` (no guard) | 75.7 | 960 | 112 | r5 | r9 | r8 | sl | r6 | |
| wrap on `miningGame==0` + `i==0` | 80.4 | 968 | 112 | r5 | fp | r8 | r9 | r6 | |
| wrap on `field_0x71` (no guard) | 42.0 | 956 | 120 | r5 | r0 | r8 | fp | – | |
| nested `if (object != nullptr)` + scoped surf | 59.8 | 996 | 112 | **sl** | r9 | r8 | fp | r5 | rotated |
| nested + loop-scope `Vector surface` | 64.1 | 968 | **120** | r5 | sl | r9 | fp | – | header-first |
| `continue` + loop-scope `Vector surface` | 64.1 | 968 | **120** | r5 | sl | r9 | fp | – | header-first |
| `continue` + scoped `Vector surface` (real) | 59.8 | 996 | 112 | sl | r9 | r8 | fp | r5 | rotated |
| outerCollide as wrap (real condition) | 64.1 | 968 | 120 | r5 | sl | r9 | fp | – | |
| `goto next_object` for outerCollide | 59.8 | 996 | 112 | sl | r9 | r8 | fp | r5 | identical to continue |
| `else if` arm chain | 59.8 | 996 | 112 | sl | r9 | r8 | fp | r5 | identical to continue |
| `while` loop | 42.6 | 948 | 120 | r5 | r0 | r8 | r9 | – | |
| `do … while` | 22.6 | 956 | 112 | sl | r6 | – | fp | – | |
| check1 = `break` | 68.1 | 1008 | 112 | r5 | r0 | r8 | sl | – | |
| check1 moved to end of body | 62.1 | 996 | 112 | r9 | r5 | r8 | fp | – | |
| check1 before surface block | 68.9 | 992 | 112 | r5 | r9 | r8 | fp | – | |
| `collision != Vector` (operand swap) | 87.4 | 968 | 112 | r5 | r9 | r8 | fp | r6 | |
| `Vector()` zero vector | 87.6 | 968 | 112 | r5 | r9 | r8 | fp | r6 | identical code |
| `!object`, `field_0x71 != 0`, `isVisible()==0` | 87.6 | 968 | 112 | r5 | r9 | r8 | fp | r6 | identical code |

Every variant in the table was built and scored; nothing beat 87.6.

## 3. The fixed point (new this pass)

There are two attractors, and they are mutually exclusive with this compiler:

* **latch-early** (committed shape): the anchor `continue` is the latch's
  hot predecessor, MBP lays the latch right after the anchor, the loop is
  un-rotated, `this=r5`, `dock=r9`, `i=fp` — but the body is emitted *after*
  the latch and the two anchors cost 8 instructions.
* **latch-late** (wrap shape): the wrap's `then` is the body, so MBP leaves
  the latch after `hitCamera` (the original layout), `this=r5` — but the
  extra `i` use in the wrap block changes the greedy allocator's spill
  weights so `i` evicts `dock` from `r9` (`dock=fp i=r9`).

New measured facts that sharpen the pass-11/12 conclusions:

1. **Removing the `i` reference from the wrap does not help.** Wraps on
   `miningGame == 0` / `dockedFlag` (no `i` in the condition) still give
   `dock=fp i=r9` (75.7 / 75.7) or `dock=r9 i=sl` (no guard). The swap is
   not caused by `i`'s *uses* in the wrap, it is the layout change itself.
2. **`i` can be moved out of `r9` by adding the wormhole `size()==i` anchor**
   (a `continue` in the wormhole arm): `dock=r9 i=fp` at 85.5 % / 976 B. But
   it is a second redundant size load, so the size grows and the score stays
   under 87.6.
3. **`bool first = (i == 0);`** (one `cmp`, reused by both guards) does not
   shorten `i`'s live interval enough to change the allocation: 85.0 %
   (committed + guard) / 87.3 % (wrap + guard).
4. **The collision-arm register shuffle is real source, not an artifact.**
   The original does `mov sl,r5; mov r5,r6; add r6,sp,#96` before the
   `projectCollisionOnSurface` call and `mov r6,r5; mov r5,sl; mov r1,r6`
   after it. That is only produced when `this` is in `r5` *and* `surface`
   is in `r6` at that point; every shape that gets `dock=r9 i=fp` also
   removes the shuffle (4–5 instructions), which is why the fuzzy score does
   not simply track the register file.
5. **`goto next_object` and `else if` chains produce byte-identical code to
   `continue`** (59.8 % / 996 B), so the IR builder folds them; the pass-12
   suggestion of a `goto` that lands in the latch does not work.

## 4. Proven vs. hypothesis

**Proven (measured):**

* The original has no redundant `objects->size()` loads, one bottom
  `i == 0 && object->field_0x71` guard, frame 112 with `surface` at
  `sp+32`, and the register file `this=r5 dock=r9 i=fp surface=r6 obj=r8`.
* The wrap form reproduces the original's block order and latch position at
  968 B but swaps `dock`/`i`.
* The committed two-anchor shape is a genuine local optimum of the official
  fuzzy metric: ~200 source variants (anchor matrix, wrap positions, wrap
  conditions, guard positions/forms, loop forms, `goto`, `else if`,
  operand-order and spelling variants) were built and scored; none beat
  87.6 %.
* Adding the real `i == 0` guard to the committed shape costs 2.6 points
  (87.6 → 85.0) because it shifts the whole tail.

**Hypothesis (not proven):**

* The original source is the wrap/nested structure (the Ghidra decompile and
  the Java oracle both render the body as `if (object != nullptr &&
  outerCollide(...))`), and the `dock`/`i` pair is decided by
  `RAGreedy`'s `calculateSpillWeightsAndHints`, whose `SlotIndex` numbering
  follows the machine block layout. With this clang 7.0.2 build the two
  layouts that give the original's block order and the original's register
  file appear to be mutually exclusive.
* The remaining 42 non-matching instructions are: the two anchors (8), the
  missing `i==0` guard (2), the missing preheader `str.w r9,[sp,#20]` (1),
  the collision-arm shuffle (4–5), and the `.w`/`.n` branch-width changes
  caused by the latch offset.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf211
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch (not committed): `/dev/shm/wf211/` — `gen3.py`/`gen5.py`/`gen6.py`
(body generators), `batch.sh` (splice + build + score + register summary),
`score1.py`, `diffnorm.py`, `ir.sh` (`-emit-llvm`), `mir7.sh`
(`-mllvm -stop-before=block-placement`), plus the ~200 `*.txt` bodies and
`cur_show.txt` / `c0_dis2.txt` / `a01_dis.txt` disassembly dumps.

## 6. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0, `.text 0x1337c8`.
`RATCHET PASS`.
