# `Layout::Layout` C1/C2 — wave 174 / worker w11912

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (branch `wave174-w11912`, base master `be9dd429b`, wave-173 tree):
raw **5,633**, aligned **2,290**, 7,072 B, 2,519 / 2,552 instructions.

**Exit: no code change landed.**  Every size-exact store-order/SROA variant
tried around the four scoped groups was >= 5,633 raw; the tree is left at the
base.  The wave-173 `-7` reorder is a strong local optimum.

## 1. Scope

Wave-173 (`4ff2e4cc9`) reordered three groups and landed raw 5,640 -> 5,633:

* `field_0x174` after `field_0x170`/`field_0x17c` (was before them),
* `field_0x284_sliderSlot5Enabled` before `field_0x2fc`/`field_0x300`,
* `choiceWindowTitleHeight` after `field_0x300` (was after `field_0x304`).

This pass continued the sweep around the four named groups:

1. `0x170..0x17c` — `field_0x178`, `field_0x170`, `field_0x17c`, `field_0x174`.
2. `0x284..0x300` — `field_0x284_sliderSlot5Enabled`, `field_0x2fc`,
   `field_0x300`, `choiceWindowTitleHeight`, `field_0x94`, `field_0x304`
   (`field_0x290` fixed after the group; `field_0x2f4`/`field_0x2f8` before it).
3. `0x94..0xa0` — `field_0x98`, `field_0x9c`, `field_0xa0`, `field_0x94`
   (and the neighbouring `field_0xac`/`field_0xa8`/`field_0xa4`).
4. `0x2c0..0x2d0` — `field_0x2c4`, `field_0x2c8`, `field_0x2d0`,
   `field_0x2cc`, `field_0x2d8`, `field_0x2d4`, plus the late `field_0x2c0`
   assignment (source line 193).

All measurements are real `link_so` + `tools/fn_diff.py` on the checked-in
ground-truth `.so`; size must stay 7,072 B.

## 2. Exhaustive store-order results

### `0x170..0x17c` (all 24 permutations)

18 of the 24 permutations compile to raw **5,633** (the compiler reorders the
stores within the block); the 6 permutations that put `field_0x174` first give
raw 5,704 / size 7,080.  No permutation beats the base.

### `0x284..0x300` (all 720 permutations of the six statements)

Best size-exact states:

| order (`P`=`0x284`, `A`=`0x2fc`, `B`=`0x300`, `C`=`choiceTitle`,
`D`=`0x94`, `E`=`0x304`) | raw | aligned | size |
|---|---:|---:|---:|
| `PABCDE` (base) | **5,633** | 2,290 | 7,072 |
| `PBACDE` | 5,633 | 2,295 | 7,072 |
| `ABPCDE` / `BAPCDE` / `CPABDE` / `CPBADE` / `CPDABE` / `CPDBAE` | 5,634 | 2,290-2,302 | 7,072 |
| `PABCED` (0x94 after 0x304) | 5,878 | 2,365 | 7,116 |

No permutation lowers raw.  Moving `field_0x290` to any of the seven slots
also fails (best 5,633; next 5,830), as does moving `field_0x2f4`/`field_0x2f8`
into the group (best 5,633).

### `0x2c0..0x2d0`

* All 6 permutations of `{0x2d0, 0x2cc, 0x2d8}`: base `5,633`; the faithful
  `0x2d0`-after-`0x2cc` order (the original's vcvt-chain placement) is
  **5,637**, aligned 2,285, size 7,072 — a +4 raw regression.
* `field_0x2c0` relocated to each of the 250 assignment positions: the only
  size-exact state below 5,798 is the base (5,633).  The faithful
  `0x2c0`-next-to-`0x2c4` order shrinks the body to 7,012/7,016 B.
* `field_0x2d0` relocated to every position: base 5,633, next 5,637.
* `field_0x2cc` relocated to every position: base 5,633, next 5,637.
* `field_0x2d8` relocated to every position: best size-exact 5,702.
* Moving the whole `field_0x2d4` if/else block before `0x2d8` gives 5,702;
  after `0x2d0` gives 6,026 / size 7,128.

### `0x94..0xa0`

`field_0x94` relocated to every assignment position: base 5,633; next 5,637.
Moving `field_0x98` next to `0x9c` gives 5,780; moving `0x94` next to
`0x9c`/`0xa0`/`0xa4` gives 5,880-5,969 and shrinks the body by 40-72 B.

## 3. Random and SROA sweeps

* 400 random single-statement moves across the whole ctor: no size-7,072
  state below 5,633.
* 400 random statement swaps: no improvement.
* 500 random two-statement moves: no improvement.

Source-shape / SROA variants (all size-exact unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **5,633** | 2,290 | 7,072 |
| `t_field_0x2c8` temporary folded into the store | 5,633 | 2,290 | 7,072 |
| `field_0x2f8` float literals -> doubles | 5,633 | 2,290 | 7,072 |
| `field_0x98` `420 * 1.40625f` -> `590` / `(int)(...)` | 5,633 | 2,290 | 7,072 |
| `field_0x160` inner `(int)` removed | 5,633 | 2,290 | 7,072 |
| `field_0xa8` temporary folded | 5,715 | 2,286 | 7,064 |
| `field_0x2d0` `(int)(float)(int)` triple cast | 5,801 | 2,311 | 7,112 |
| `field_0x10c_rowBaseY` inner `(int)` removed | 5,741 | 2,336 | 7,064 |
| `volatile rd` removed | 5,732 | 2,290 | 7,064 |
| `field_0x174` `84.0f` -> `84` | 6,322 | 4,161 | 7,020 |
| `field_0x164` `0.0f` -> `0` | 6,226 | 4,039 | 6,900 |
| `0x2c4`/`0x2c8` faithful if/else | 5,708 | 2,325 | 7,032 |
| `0x2cc`/`0x2d0` faithful if/else | 5,864 | 2,352 | 7,068 |
| `0x2d4`/`0x2d8` shared-local if/else | 5,738 | 2,305 | 7,056 |

The `84.0f`/`0.0f` float literals are load-bearing: replacing them with int
literals changes the vector-constant codegen drastically (raw 6,322 / 6,226).

## 4. Why the scoped store order is saturated

The original's store map has more `strd`/`stmia` merges than ours (8 `stmia`
groups vs. 3), but those merges come from the original's **nested per-device
`if/else` dispatch with duplicated branch stores**, not from statement order.
Reproducing the merges faithfully (e.g. `0x2c0`+`0x2c4` `strd`,
`0x94..0xa0` `stmia`, `0x2fc..0x304` `stmia`) requires the missing non-iPadHD
branch code: the wave-173 note measured the original non-iPadHD paths at
3,962 B vs. our 4,144 B, while our literal pool is 776 B vs. the original's
966 B.  A faithful merge shrinks the body below 7,072 B (7,012-7,056 B in the
`0x2c0` experiments) and the size is only restored by scrambling the rest of
the order, which costs more raw than it saves.

The `0x2d0` `(int)(float)` vcvt chain is the clearest witness: the original
keeps the conversion after a branch merge (shared local), while our flat
ternary is constant-folded to a plain `str`.  Swapping `0x2d0` after `0x2cc`
restores the vcvt placement (raw 5,637, aligned 2,285) but costs +4 raw, so it
was not landed.

## 5. Conclusion

The wave-173 tree is a strong local optimum for the four scoped store groups.
No size-exact raw win was found; `src/game/ui/Layout.cpp` is unchanged.  The
remaining raw residue (5,633 B) is dominated by the literal-pool deficit
(190 B) and the missing non-iPadHD branch duplication, not by the store order
of `0x170..0x17c`, `0x284..0x300`, `0x94..0xa0` or `0x2c0..0x2d0`.

Gate at exit (base tree, `check` green): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, parity **5370/5370**,
`.rel.plt` byte-identical.  `Layout` C1/C2 raw **5,633**, aligned **2,290**,
size **7,072/7,072**.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11912
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11912/`: `Layout.base.cpp`
(pristine wave-173 ctor), `harness.py` (build + `fn_diff` + restore),
`exhaustive.py` (single-statement relocation), `side.txt`/`orig.dis`/
`ours.dis` (side-by-side disassembly).
