# Layout::Layout C1/C2 — literal-pool constant identification and the
# confirmed 7072-byte local optimum (wf102, wave 102)

Worker **wf102**, branch `wave102-wf202`, base `53d9ee4d5`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`, so the ctor is `0xe1028` in the Ghidra project).

Continues `layout-layout-c1-c2-ios-order-wf101.md` (wave 101, the landed
47.7724741448 state), `layout-layout-c1-c2-store-order-w5001.md`,
`layout-layout-c1-c2-order-search-w4901.md` and
`layout-layout-c1-c2-hd-block-order-w4221.md`.

## Result

Landed a **size- and score-neutral** source-fidelity fix: the `field_0x168`
non-HD `n9` branch is `37.5f`, not `37.0f`. The original materialises the float
constant `0x42160000` (37.5) and `vcvt.s32.f32`s it to the stored int 37; our
old source materialised `0x42140000` (37.0). The stored field value was already
correct (both truncate to 37), so the fuzzy score is unchanged
(**47.77247414478918 %**, `usize` 7072) and no gate number moves.

Gate before == after:

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4500 / 4524 | **4500 / 4524** |
| `linked_exact` | 4500 / 4524 | **4500 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`.

## 1. The literal pool is a direct window into the source values

The ctor's literal pools live inside `.text` (the function is
`0xd1028..0xd2bc8`). Dumping every word in that range and decoding the
IEEE-754 floats gives the complete set of float constants the original
materialises. Comparing it with our build identifies four source-value
differences that the stored-int comparison cannot see:

| constant | original | ours (before) | source field | meaning |
|---|---:|---:|---|---|
| `37.5` (`0x42160000`) | `d1d88` | `37.0` (`0x42140000`) | `field_0x168` n9 | `(int)37.5f == 37` |
| `5.625` (`0x40b40000`) | `d14d8` | absent | `field_0x1e4`/`0x150` HD | `(int)(4*1.40625f) == 5` |
| `75.0` (`0x42960000`) | `d1900` | absent | `field_0x9c` n9 | `(int)75.0f == 75` |
| `140.625` (`0x430ca000`) | absent | present | `rewardBoxWidth` HD | original stores int `140` |
| `1.5` (`0x3fc00000`) | absent | present (x2) | `ringRadiusScale` n9 | original encodes it as a `vmov` immediate |

The original's `field_0x1e4` trace is explicit: `vldr s0,[d14d8]` (5.625) at
`d1254`, carried across the HD block's `b.w d21e2`, then
`vcvt.s32.f32 s0,s0` at `d21e2` and `vstr s0,[r0,#484]` (0x1e4) at `d21fe`.
`field_0x150` has the same shape (`4*1.40625f` -> 5). `field_0x9c` loads
`75.0` from the mid-block island at `d1900` and `vcvt`s it at `d199e`; the
`100.0`/`50.0` arms are the end-pool words at `d285c`/`d2860`.

The **landed** change is only the `37.5f` one: it is the only value fix that is
both size- and score-neutral. The others are documented below because they are
the faithful source but are not landable at 7072 B.

## 2. Measured single-edit table (official linked scorer, `autojunk=False`)

Baseline: `usize = 7072`, raw **47.77247414478918 %**.

| edit | `usize` | raw % |
|---|---:|---:|
| `0x168` n9 `37.0f` -> `37.5f` | 7072 | **47.7724741448** (code-identical score) |
| `0x158` HD `70` -> `50*1.40625f` | 7072 | 47.7724741448 (code-identical) |
| `0x170` HD `174` -> `174.0f` | 7072 | 47.7724741448 (code-identical) |
| `0x9c` if/else -> single ternary | 7072 | 47.4448200437 |
| `0x204` HD `9*1.40625f` -> `12` | 7072 | 47.4353876740 |
| `0x1f4` HD `16*1.40625f` -> `22` | 7056 | 47.7403941867 |
| `0x148` HD `2*1.40625f` -> `2` | 7056 | 47.5811268166 |
| `0x150` HD `5` -> `4*1.40625f` | 7080 | 47.7232054086 |
| `0x194`/`0x1a4` HD int -> `N.0f` | 7080 | 47.7232054086 |
| `0x188` HD `23` -> `23.0f` | 7088 | 47.6550079491 |
| `0x1e4` HD `5` -> `4*1.40625f` | 7056 | 44.9174457927 |
| `0x124` HD `1*1.40625f` -> `1` | 7032 | 42.7662957075 |
| `rewardBoxWidth` HD `100*1.40625f` -> `140` | 7076 | 42.4676616915 |
| `0x20` HD `5*1.40625f` -> `7` | 6944 | 20.6238823763 |

`0x158_f`/`0x170_f` are code-identical: clang folds `50*1.40625f` to 70 and
`174.0f` to 174 before the select, so they do not add pool words.

## 3. Negative searches (all size-7072 filtered)

The tree is a genuine local optimum in both order and expression space:

* **861 random order moves** (block move length 1-8, swap, segment reversal;
  seed 202, ~15 min): zero improvements over 47.7724741448.
* **831 simulated-annealing order moves** (accept equal-score plateau moves,
  seed 99, ~14 min): zero improvements; the best state never left the landed
  order.
* **589 random subsets of the 15 expression edits above** (1-6 edits each,
  seed 7): zero size-7072 variants beat 47.7724741448.
* **Targeted structural moves** (current order, all worse):
  `0x1e0` -> just before `0x1e4` = 6984 / 40.625;
  `0x2c0` -> before `0x2c4` = 7036 / 43.423;
  `0x2c0` -> before `0x2c8` = 7096 / 40.928;
  `0x2c0` -> before `0x70`/`0x74`/`0x78` = 7052/7040/7052, 21.1/43.6/43.5.
* **iOS source order + the four faithful value fixes** (`37.5f`, `4*1.40625f`
  for `0x1e4`, `rewardBoxWidth = 140`, `0x9c` ternary): 6992 B / 35.464.
  iOS order alone is 6936 B / 38.007 (reproducing wf101).

## 4. Store-order trace (new, via the w4301 Thumb/VFP emulator)

Running `_work/reconstructions/layout_ctor/emu.py` (path-adapted to this
worktree) over the original and our build gives the execution-order store
sequence. For `iPadHD=1` the original performs 615 stored bytes / 603 ours,
byte-sequence ratio **0.824**; for the default config 435 / 363, ratio
**0.529**. The first divergence in the HD trace is the known field-set
mismatch (`0x2c0`/`0x1e0`) and then the `0x94`, `0x140`, `0x144`, `0x204`,
`0x23c`, `0x2bc` placements already documented in wf101. The default-path
store *count* difference is SLP grouping: ours merges adjacent fields into
`vst1.32`/`strd`, the original keeps scalar `str` runs between the per-field
flag branches.

## 5. Conclusion

* **Proven:** the original's `field_0x168` n9 constant is `37.5`, and
  `field_0x1e4`/`0x150` HD constants are `5.625 = 4*1.40625f`, not the folded
  int `5`. `field_0x9c` n9 is `75.0f` through an `fptosi`, and
  `rewardBoxWidth` HD is the int `140`, not `100*1.40625f`.
* **Proven:** the landed 7072-byte tree is a local optimum of the official
  fuzzy metric under both statement reordering and the 15 known expression
  edits; ~2300 measured candidates found no size-7072 variant above
  47.7724741448.
* **Hypothesis:** byte-exactness needs the iOS source order (the literal-pool
  integer run proves it) plus a register allocation and literal-pool-island
  placement that clang 7.0.2 only produces for the exact original expression
  types. The current hill order is the metric optimum but is structurally
  wrong (six HD/tail fields on the wrong side of the 137/204 split).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf202
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/wf202/fscore.py                            # 47.7724741448, 7072 B
python3 /dev/shm/wf202/emu.py _ZN6LayoutC1Ev iPadHD=1 n9=0 retinaDisplay=0 \
    iPadLarge=0 iPad=0 iPadLargePossible=0 iPadAssetsWithLowerRes=0
```

Scratch (`/dev/shm/wf202`): `fscore.py` (official unrounded linked scorer),
`units.py` (204-unit splitter + header offset map), `reorder.py`,
`search.py` (861-move search), `sa.py` (831-move annealing), `edits.py`
(15-edit table), `combo.py` (589-subset search), `emu.py`/`trace.py`/
`trace2.py` (store-order trace), `Layout.base.cpp` (pristine ctor).
