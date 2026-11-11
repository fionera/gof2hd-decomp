# `Layout::Layout` C1/C2 — wave 175 / worker w11911

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (branch `wave175-w11911`, base master `5f599a137`, wave-174 tree):
raw **5,633**, aligned **2,290**, 7,072 B, 2,519 / 2,552 instructions,
`.text` differing bytes **80,439**.

**Exit: one size-exact raw win landed.**  Moving the `field_0x17c`
assignment from its old position (after `field_0x170`) to just before
`field_0x158` gives raw **5,624** (-9), aligned **2,287** (-3), size
**7,072/7,072**, 2,518 instructions, `.text` differing bytes **80,430**
(-9).  No value, type or control-flow change.

## 1. The landed move

`src/game/ui/Layout.cpp`, constructor body, before:

```cpp
this->field_0x178 = ...;
this->field_0x170 = ...;
this->field_0x17c = Globals::iPadHD ? 155 * 1.40625f : (...);
this->field_0x174 = ...;
...
this->field_0x160 = ...;
this->field_0x158 = Globals::iPadHD ? 70 : (... this->field_0x154 - 1 ...);
this->field_0x2bc = ...;
this->field_0x15c = ...;
```

after:

```cpp
this->field_0x178 = ...;
this->field_0x170 = ...;
this->field_0x174 = ...;
...
this->field_0x160 = ...;
this->field_0x17c = Globals::iPadHD ? 155 * 1.40625f : (...);
this->field_0x158 = Globals::iPadHD ? 70 : (... this->field_0x154 - 1 ...);
this->field_0x2bc = ...;
this->field_0x15c = ...;
```

The store now sits at the head of the non-iPadHD `0x158`/`0x160`/`0x2bc`/
`0x15c` branch tree instead of after the `0x16c..0x178` vector-store group.
The compiler duplicates the `0x17c` store into each branch of that tree
(`str.w r1,[r0,#380]` at `d1e6c`, `d1e9e`, `d1eca`, `d1eec`), which removes
one instruction and nine differing bytes from the tail of the group.  The
`0x16c..0x178` `vst1.32` group is still emitted (shifted by 0x16 bytes).

## 2. Search performed

The wave-174 note (`Layout-LayoutC1-wave174-w11912.md`) had already exhausted
the four scoped store groups by statement permutation.  This pass added the
following searches, all measured with a real `link_so` + `tools/fn_diff.py`
on the checked-in ground-truth `.so`; size had to stay 7,072 B:

* **Exhaustive adjacent swaps** of every pair of adjacent single-line
  assignment statements in the constructor (197 pairs): no state below
  5,633 raw.
* **Pairwise swaps** of 26 statements spanning the four scoped groups
  (`0x170..0x17c`, `0x2c4..0x2d0`, `0x94..0xac`, `0x284..0x304`) — 276 pairs:
  no state below 5,633 raw.
* **Ternary-condition permutations** for `0x170`, `0x174`, `0x178`,
  `0x17c` (all 120 orders each, default fixed) and for `0x2c4`/`0x2cc`
  (all 120 orders each): the checked-in order is the best in every case.
* **Local single-line moves** of 19 scoped statements to every assignment
  position within ±18 lines: this found the `0x17c -> before 0x158` move
  (raw 5,624, size exact).
* **Full relocation sweep** of `field_0x17c` to every assignment position
  (~330): position 151 (before `field_0x158`) is the unique best size-exact
  state at 5,624; the next size-exact state is the base 5,633.
* **Second-move sweeps** on top of the win: every scoped statement within
  ±16 lines, and every statement in the `0x150..0x17c` neighbourhood within
  ±18 lines.  No second move improves below 5,624; many are code-neutral.

## 3. Measurements

| metric | before | after |
|---|---:|---:|
| `Layout` C1/C2 raw same-address diff | 5,633 | **5,624** |
| aligned instructions | 2,290 | **2,287** |
| size | 7,072 | **7,072** |
| instruction count | 2,519 | **2,518** |
| `.text` differing bytes | 80,439 | **80,430** |

Full `check` gate at exit: `byte_exact` **4507/4524**, `linked_exact`
**4507/4524**, size-exact **4524/4524**, `stub_zero_size` **0**, missing
**0**, wrong_type **0**, extra **0**, `data_exact` **843/843**, imports
**207**, lint **CLEAN**, drift **0**, parity **5370/5370**, `.rel.plt`
byte-identical.  `RATCHET PASS`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11911
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11911/`: `Layout.base.cpp`
(pristine wave-174 ctor), `Layout.win5624.cpp` (landed source), `harness.py`
(build + `fn_diff` + restore), `sweep_*.py` (adjacent/pair/permutation/local
move/full relocation searches), `*_results.json` (raw/aligned/size for every
variant), `orig.dis`/`ours.dis`/`win.dis` (disassembly).
