# LevelScript::process — wave 148 / w9406: case-92 state-1 block,
# the 3-call getPosition shape and the field_0x38/field_0x128 store order

Status: **store-order swap landed**; the fully faithful 3-call `getPosition()`
shape is reconstructed and measured but **not landed** because it is +8 B and
would drop `byte_exact` (it needs a -8 companion).  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Scratch in `/dev/shm/w9406/` (not tracked): `block_align5.py`,
`fdiff.py`, `align_diff.py`, `case92_3call_swap.txt`.

## Result

```
                         HEAD (wave 147)   this pass (landed)   3-call variant
process raw diffbytes    54,913            54,912   (-1)        55,220
process symbol size      57,232            57,232   exact        57,240 (+8)
global align_diff        15,740            15,739   (-1)         15,734 (-6)
block-local ratio        0.6782            0.6782               0.7165
block local eq/insns     177/261           177/261              187/261
byte_exact               4507              4507                 2741 (not landed)
linked_exact             4507              4507                 4507
size-exact               4524/4524         4524/4524            4523/4524
data_exact               843/843           843/843              843/843
imports                  207               207                  207
lint                     CLEAN ✓           CLEAN ✓              CLEAN ✓
RATCHET                  PASS              PASS                 FAIL
```

The landed change is a **one-line statement-order swap** in the state-1 loop:

```diff
-                            ((PlayerFighter *) (*enemies)[i])->field_0x128 = 0;
                             (*enemies)[i]->field_0x38 = 3;
+                            ((PlayerFighter *) (*enemies)[i])->field_0x128 = 0;
```

The original emits the stores in the order `field_0x128` (`str.w r5,[r0,#296]`)
then `field_0x38` (`str.w r8,[r0,#56]`).  With the pre-wave source order
(`0x128` then `0x38`) clang 7 emits the **opposite** order; swapping the source
statements makes clang emit the original order.  The swap is size-neutral
(57,232 B) and improves the raw same-address diff by 1 byte.  The block-local
instruction-alignment count is unchanged (177/261) because both stores were
already mismatched before and after; the win is that the *positions* now match,
which is what the original source order says.

## The fully faithful 3-call shape (measured, not landed)

The original calls `(*enemies)[3]->getPosition()` **three times**, once per
`setPosition` (calls at `0x13a06e`, `0x13a0ac`, `0x13a0e2`).  The wave-147
source used a `Vector ref` for the first/third call and inlined the second, to
hold 57,232 B.  The faithful source is:

```cpp
(*enemies)[0]->setPosition((*enemies)[3]->getPosition() + Vector{-76000.0f, 5000.0f, 5000.0f});
(*enemies)[1]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 7500.0f});
(*enemies)[2]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 2500.0f});
```

(i.e. delete the `Vector ref = ...` line and use `getPosition()` in all three
statements).  Measured on top of the landed store-order swap:

* raw same-address diff: **55,220** (vs 54,912 landed)
* symbol size: **57,240 B (+8)** — the third virtual `getPosition()` sequence
  costs 8 B in this block
* block-local instruction alignment: **0.7165 (187/261)** vs 0.6782 (177/261),
  i.e. **+10 matching instructions** in the 261-instruction block
* global `align_diff.py`: **15,734** vs 15,739 (-5)
* `byte_exact` drops to **2741** because every following function shifts by 8 B

So the 3-call shape is the right reconstruction but needs an **-8 B companion**
elsewhere in `LevelScript::process`.  It is not a source-level `Vector ref`
artifact: the block is semantically complete with three calls.

## Where the -8 should come from (case-125 lead)

The state-1 block is `0x13a036..0x13a2ca` in the original (261 insns, 656 B
excluding the `b.w` tail).  Ours is `0x13a1bc..0x13a440` (257 insns, 640 B).
The block is *16 B shorter* than the original, yet the function is +8 B: the
rest of the function is +24 B.  The 3-call shape adds 8 B to the block.

The dispatch itself shows a concrete 6 B of recoverable code:

* original: `13a024 e8df f010 tbh [pc, r0, lsl #1]`, table at `0x13a028`,
  state-1 block immediately at `0x13a036`.
* ours: `13a176 f20f 010c addw r1, pc, #12`, `13a17a e8d1 f010 tbh [r1,...]`,
  `13a17e bf00 nop`, table at `0x13a186`, state-1 block at `0x13a1bc`.

The `addw`+`nop` (10 B vs 4 B) exists because 40 B of the **case-125** literal
pool sits between the tbh table and the state-1 block (`0x13a194..0x13a1bc`).
The wave-147 w9303 note already localized case 125: the original puts its
state-1/2 continuation at `0x13b43e`/`0x13b518`, ours keeps it inline after the
case-125 entry.  Fixing case-125 placement should let clang emit
`tbh [pc, ...]` and recover 6 B; the remaining 2 B are table/pool alignment.
That is the expected companion for the 3-call shape.  **Do not land the 3-call
shape alone** — test it together with the case-125 placement fix.

## Other hypotheses tried (all worse)

* `Vector pos = (*enemies)[3]->getPosition();` + two reassignments: **+48 B**
  (the named local is not elided; `Vector` has a zeroing default ctor).
* `Vector pos;` + three assignments: **+56 B** (zero-init stores).
* `__builtin_expect(Globals::status->field_178 == 0, 1)` on the block guard:
  size unchanged, raw diff +1.
* `__builtin_expect(m_nState == 1, 1)`: size unchanged, raw diff +465.
* `int state0 = m_nState; ... m_nState = state0 + 1;` (or `= 1`): saves 16 B
  but moves the state-1 block to `0x13c3e8` (outlined, mixed with the next
  state), block ratio 0.6471 — a layout regression, not a companion.
  `m_nState++` / `m_nState += 1` with the local are size-neutral.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9406/fdiff.py _ZN11LevelScript7processEi          # 54,912 / 57,232
python3 /dev/shm/w9406/block_align5.py 0x13a036 0x294 0x13a1bc 0x27c  # 0.6782
```

`block_align5.py` is the w9320 metric: difflib over the block instruction
streams after stripping `;` comments, `<...>` symbol annotations, and
normalising branch-target/PC-relative addresses to `T`.  Ranges are
`[start, start+size)`; `0x294`/`0x27c` include the final `b.w` and exclude the
original's trailing `nop`.  The note's w9320 ranges (`0x296`/`0x27c`) give
0.6756 for the landed source and 0.7137 for the 3-call shape — the same
+0.038 delta.
