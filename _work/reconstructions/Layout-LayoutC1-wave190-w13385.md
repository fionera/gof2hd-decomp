# `Layout::Layout` C1/C2 — wave 190 / worker w13385

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13385`, base master `dd8cf7a29`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,256,
aligned 2,726, size 7,072/7,072, insns 2,520/2,552).**

## 1. Semantic oracle: the current body is value-identical on all 32 flag combos

The checked-in `layout_ctor/emu.py` (Unicorn-free Thumb/VFP emulator) was
re-pointed at our linked `.so` (`EMU_SO=...`) and run over all 32 combinations
of `iPadHD/n9/iPad/iPadLarge/retinaDisplay`.  Every 4-byte field value at every
offset matches the original exactly.  The four "semantic diffs" reported by
the wave-188 oracle are therefore codegen-only: the current body deliberately
keeps the byte-matching (but semantically wrong) values at `0x108`, `0x114`
and `0x1f4`; `0x190` was fixed by w13314.

The destructor (`_ZN6LayoutD1Ev`/`D2Ev`) is already byte-exact (raw 0).

## 2. Store-order target vs ours

The emulator's store trace gives the original's exact default-path store
order (202 fields).  Our compiled order differs substantially (difflib ratio
0.50); the first divergence is the `0x14..0x20` SLP group: the original emits
`0x14,0x18,0x1c,0x20` as one `vst1`, ours splits `0x20` out because
`field_0x20_top` is far from `0x14..0x1c` in the source.

## 3. Searches run (all measured, none landed)

* **K=6 exhaustive, windows 196..198** (batched compile-only proxy, 100
  functions per clang invocation, `-mllvm -enable-machine-outliner=0`):
  best size-exact proxy raw **5,255** (window 196, `(0,1,3,2,4,5)`) —
  +1 vs the 5,254 proxy base, i.e. a raw loss.
* **K=6 exhaustive, windows 100..103**: best size-exact proxy raw **5,320**
  (window 100, `(0,1,3,4,2,5)`); no candidate below 5,254.
* **Ascending-offset sorts** of the early block `0x24..0x74` (R1),
  `0x204..0x274` (R2), `0x140..0x1d0` (R3), `0x108..0x138` (R4),
  `0x1e0..0x1f8` (R5): proxy raw 5,836 / 6,396 / 6,453 / 6,358 / 6,423,
  sizes 7,000 / 7,092 / 7,132 / 7,064 / 7,104 — all worse and not size-exact.
* **iOS 1.1.4 source order** (full ctor): raw 6,338, size 7,108.
  **iOS order for the first 76 fields only** (through `0x104`): raw 6,309,
  size 6,880.  The wave-185/188 hill-climbed order is a much better basin.
* **Original `strd` pair adjacencies** `0x74/0x78` (both orders) and
  `0x7c/0x80`: proxy 5,695 / 5,799 / 5,426 — raw losses.
* **`(int)(float)` round-trip** on `0x60,0x68,0x6c,0x15c,0x168,0x1a8`:
  codegen-neutral (proxy 5,254/7,072).

## 4. Why the residual is hard

The original is 6,104 B code + 968 B literal pool; ours is 6,288 B code +
784 B pool (w13013).  The 184 B code/pool split difference is exactly the
~10 extra SLP groups the original has (86 `vld1`/81 `vst1`/62 `strd` vs
76/71/40).  Every faithful shape that adds SLP groups shrinks the function
below 7,072 B, so it is size-fatal unless the pool grows by the same amount.
The landed wave-185/188 order is a sharp local optimum for the same-address
byte metric; the remaining K=6 tail windows sampled here have no improving
size-exact permutation.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13385
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5256 aligned=2726 size=7072/7072 insn=2520/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13385/`: `emu.py` (re-pointed
oracle), `vals.py`/`cmpvals.py`, `reorder.py`/`gen_perm2.py`,
`batch.py`/`bsearch.py` (batched proxy), `orig.dis`/`ours.dis`.
