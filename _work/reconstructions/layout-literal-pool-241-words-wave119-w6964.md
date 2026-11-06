# Layout::Layout C1/C2 — wave 119 / worker w6964
## 241-word literal-pool census, independently re-derived, plus the one semantic mismatch

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6964`, base = wave-119 master `d512ec24e`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

This pass re-derives the w6958 census from scratch with independent scripts,
confirms every number in it, and adds the one thing the census could not see
from the disassembly alone: **a real semantic difference between our source and
the original at `field_0x184`**, proven by emulating both constructors over all
32 flag combinations.  No source change landed (see §8 for why).

## Result — note only, no source change landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.963317384370 % | **48.963317384370 %** |
| `report.json` pct (1 dp) | 49.0 | 49.0 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`;
`--target drift` → `0 field(s) across 0 class(es)`.
No source change landed, so the 27 section headers, the file size
(**2,187,328 B**), `.text` (`0x1337c8`), `.ARM.exidx` (`0x6f70`),
`.ARM.extab` (`0x26298`, exact), `.rodata` (`0x41c27`), `.data` (`0x296c`)
and `.rel.plt` (`0x72d0`) are unchanged.  `tools/verify_baseline.json` was
**not** touched.

## 1. Method / exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w6964
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6964/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6964/ours.dis

python3 /dev/shm/w6964/enumgrp.py     # 50 vs 42 end-pool vld1.64 groups
python3 /dev/shm/w6964/diffgrp.py     # 31 orig-only / 23 ours-only multisets
python3 /dev/shm/w6964/pair2.py       # 16 window-shift pairs -> 15 vs 7 unpaired
python3 /dev/shm/w6964/scalars.py     # 14 vs 14 scalar floats, values listed
python3 /dev/shm/w6964/poolwords.py   # 7 vs 8 GOT deltas
python3 /dev/shm/w6964/score.py       # single-function exact scorer
python3 /dev/shm/w6964/tracecmp.py    # store-order agreement per flag combo
```

Scratch harnesses (not committed, `/dev/shm/w6964/`): `enumgrp.py`,
`grpmod.py`, `diffgrp.py`, `pair2.py`, `scalars.py`, `poolwords.py`,
`score.py`, `count.py`, `align.py`, `blocks.py`, `trace.py`, `tracecmp.py`,
`checktrace.py`, `t184.py`, `harness.py`, `try1.py`…`try5.py`,
`reorder.py`, `reorder2.py`, `reorder3.py`, `unitorder.py`, `randsearch.py`,
`hill.py`, `census2.py`, plus `Layout.base.cpp`, `Layout.ios.cpp`,
`Layout.orddef.cpp`, `order_base.txt`, `order_best.txt`, `orig.dis`, `ours.dis`.

## 2. Independent census — every w6958 number reproduced

The last hot instruction is `pop {r4,r5,r6,r7,pc}`.  Everything after it is
the cold region (outlined tails + alignment pad + literal pool):

| | `pop` | region after `pop` | cold blocks | pad | end pool | end-pool words |
|---|---:|---:|---:|---:|---:|---:|
| **original** | `0xd2800` | **966 B** | `0xd2802..0xd2852` = **80 B** | 2 B | `0xd2854..0xd2bc8` = **884 B** | **221** |
| HEAD (base) | `0xd28bc` | **778 B** | `0xd28be..0xd28ce` = **16 B** | 2 B | `0xd28d0..0xd2bc8` = **760 B** | **190** |
| delta | | **+188 B = 47 words** | **+64 B** | 0 | **+124 B** | **+31** |

`(0xd2bc8 - 0xd2854)/4 = 221`, `(0xd2bc8 - 0xd28d0)/4 = 190`.
The task brief's "241 words" is the 966 B post-`pop` region / 4 = 241.5
(rounds to 241); the literal pool proper is 221 words and the other 5 words
are the four outlined `iPad` tails + pad.

The end-pool `vld1.64` groups (pool address ≥ pool start), re-enumerated
from scratch by tracking `add/addw/subw rX,pc,#imm` and `ldr rX,[pc,#N];
add rX,pc`:

| | original | base |
|---|---:|---:|
| end-pool `vld1.64` groups | **50** (200 words) | **42** (168 words) |
| end-pool scalar floats | **14** | **14** |
| end-pool GOT deltas | **7** | **8** |
| total | **221** | **190** |

The multiset diff of the 4-word group payloads is **31 original-only** vs
**23 base-only**.  Pairing each base-only group with an original-only group
that is the same four values shifted by one word resolves **16 window-shift
pairs** (the seven HD and seven n9 `0x204..0x26c` runs, plus two more),
leaving **15 original-only** and **7 base-only** unpaired.  15 − 7 = **8
groups = 32 words**, and 32 − 1 (extra GOT) = **31 words**, exactly the
end-pool deficit.

## 3. The missing/duplicated constants — full decomposition

The end-pool word deficit is **31**, and it decomposes exactly as:

```
  +32   eight missing SLP groups (15 original-only − 7 base-only)
   -1   one extra GOT delta on our side (Globals::iPadLarge)
   +2   our pool has 82.0f and 41.0f; the original has them as int immediates
   -2   our pool has 0.325f/0.65f in a .text island, not the end pool
  ----
   31
```

The scalar-float **count** is equal (14 vs 14), which is why the w6958 note
correctly called the task brief's "extra 100.0f + 50.0f" stale; but the
scalar **contents** differ by four words (two each way), which the count
hides.

### 3a. The 15 original-only unpaired groups, mapped to source fields

| # | 4 words | config / window | source fields |
|---|---|---|---|
| 1 | `3c 78 12c 258` | n9 `0xec` | `ringNearMidThreshold`=60, `field_0xf0`=120, `ringMidFarThreshold`=300, `field_0xf8`=600 |
| 2 | `13 13 13 f3` | n9 `0x140` | `field_0x140`=19, `0x144`=19, `0x148`=19, `0x14c`=243 |
| 3 | `6 1d 24 10` | retina `0xb0` | `field_0xb0`=6, `0xb4`=29, `0xb8`=36, `0xbc`=16 |
| 4 | `3 f 12 8` | default `0xb0` | `field_0xb0`=3, `0xb4`=15, `0xb8`=18, `0xbc`=8 |
| 5 | `fffffffe 2 1 ffffffff` | retina `0x108` | `field_0x108`=−2, `0x10c`=2, `0x110`=1, `0x114`=−1 |
| 6 | `0 1 2 1` | default `0x108` | `field_0x108`=0, `0x10c`=1, `0x110`=2, `0x114`=1 |
| 7 | `18 dc c8 d2` | retina `0x2a0` | `field_0x2a0`=24, `0x2a4`=220, `0x2a8`=200, `0x2ac`=210 |
| 8 | `50 a0 190 320` | retina `0xec` | `ringNearMidThreshold`=80, `0xf0`=160, `ringMidFarThreshold`=400, `0xf8`=800 |
| 9 | `28 50 c8 190` | default `0xec` | `ringNearMidThreshold`=40, `0xf0`=80, `ringMidFarThreshold`=200, `0xf8`=400 |
| 10 | `c 6e 64 69` | default `0x2a0` | `field_0x2a0`=12, `0x2a4`=110, `0x2a8`=100, `0x2ac`=105 |
| 11 | `22 16 1a 4` | retina `0x13c` | `field_0x13c`=34, `0x140`=22, `0x144`=26, `0x148`=4 |
| 12 | `11 b d 2` | default `0x13c` | `field_0x13c`=17, `0x140`=11, `0x144`=13, `0x148`=2 |
| 13 | `1e 15a 1e 1be` | iPadLarge `0x180` | `field_0x180`=30, `0x184`=346, `0x188`=30, `0x18c`=446 |
| 14 | `a 18 8 14` | retina `0x1d0` | `field_0x1d0`=10, `0x1d4`=24, `0x1d8`=8, `0x1dc`=20 |
| 15 | `5 c 4 a` | default `0x1d0` | `field_0x1d0`=5, `0x1d4`=12, `0x1d8`=4, `0x1dc`=10 |

### 3b. The 7 base-only unpaired groups

| # | 4 words | config / window | source fields |
|---|---|---|---|
| A | `41700000 3fc00000 41400000 3fc00000` | n9 `0xdc` (float) | `field_0xdc`=15.0f, `animSpeedScale`=1.5f, `driftSpeedDivisor`=12.0f, `ringRadiusScale`=1.5f |
| B | `190 a8 18 18` | retina `0x294` | `field_0x294`=400, `0x298`=168, `0x29c`=24, `0x2a0`=24 |
| C | `c8 54 c c` | default `0x294` | `field_0x294`=200, `0x298`=84, `0x29c`=12, `0x2a0`=12 |
| D | `83 7c 2a 3c` | iPad `0x16c` | `field_0x16c`=131, `0x170`=124, `0x174`=42, `0x178`=60 |
| E | `73 6a 2d 1b` | n9 `0x16c` | `field_0x16c`=115, `0x170`=106, `0x174`=45, `0x178`=27 |
| F | `8a 80 3c 2c` | retina `0x16c` | `field_0x16c`=138, `0x170`=128, `0x174`=60, `0x178`=44 |
| G | `45 40 1e 16` | default `0x16c` | `field_0x16c`=69, `0x170`=64, `0x174`=30, `0x178`=22 |

So the original vectorises the **retina/default** arms of `0xb0..0xbc`,
`0x108..0x114`, `0xec..0xf8`, `0x13c..0x148`, `0x1d0..0x1dc`, `0x2a0..0x2ac`
(plus the n9 `0xec`/`0x140` and iPadLarge `0x180` runs); our build vectorises
the **n9 float** `0xdc..0xe8` run and the `0x294`/`0x16c` runs instead.

### 3c. Scalar constants — the exact value lists

Original end-pool scalars (14):
`0.325f, 0.65f, 100f, 50f, 281.25f, 400f, 200f, 150f, 200f, 100f, 150f,
144f, 150f, 76f`.

Base end-pool scalars (14):
`100f, 50f, 281.25f, 400f, 200f, 150f, 82f, 41f, 200f, 100f, 150f, 144f,
150f, 76f`.

* `0.325f` (`0x3ea66666`) and `0.65f` (`0x3f266666`) are at
  `0xd2854/0xd2858` in the original but at `0xd2548/0xd254c` (a `.text`
  island) in the base.  Both values are present; only the placement differs.
* `82.0f` (`0x42a40000`) and `41.0f` (`0x42240000`) are at
  `0xd2ba0/0xd2ba4` in the base and **absent** from the original, which
  materialises them as `movs r2,#82` / `moveq r2,#41` immediates at
  `0xd19c0/0xd19c8`.  This is the one genuine **type** mismatch: the
  original's `field_0xa8` helper `v` is an `int` chain, not a `float`
  chain (see w6958 §4b for the faithful shape).
* The other ten scalars are value-identical in both pools.

### 3d. GOT deltas

`7` in the original (`0x13df2e, 0x13de3c, 0x13ddb4, 0x13dd96, 0x13dc9a,
0x13dc04, 0x13db84`) vs `8` in the base (`0x13de9a, 0x13ddc8, 0x13dd32,
0x13dd92, 0x13dcd4, 0x13dbd4, 0x13db3a, 0x13dabe`).  The extra base site is
the `field_0x2f4`/`field_0x2f8` pair, which reads `Globals::iPadLarge`
twice in the base and once in the original (w6958 §4c).

## 4. The one semantic mismatch: `field_0x184`

The census matches pool words to *source fields*, which cannot detect a
source expression whose **arm priority** is wrong.  Emulating both
constructors over all 32 combinations of the five flags
(`_work/reconstructions/layout_ctor/emu.py`, adapted in
`/dev/shm/w6964/`) and comparing every byte written to `this` finds exactly
**two** failing combinations out of 32, both at byte offset **0x184**:

| flags (`iPadHD n9 iPad iPadLarge retina`) | original | base |
|---|---:|---:|
| `01100` | 173 | 210 |
| `01101` | 173 | 210 |

All other 806 bytes written by the constructor are identical for all 32
combinations (the emulator compares the raw byte map, so this is a
whole-word difference, not just a low-byte artefact).  The cause is the arm
order in the current source:

```cpp
// base (n9 before iPad)
this->field_0x184 = Globals::iPadHD ? 163
    : (Globals::iPadLarge ? 346
    : (Globals::n9 ? 210
    : (Globals::iPad ? 173
    : (Globals::retinaDisplay ? 232 : 116))));
```

The original selects the **iPad arm (173)** when both `n9` and `iPad` are
set, i.e. it checks iPad before n9 for this field:

```cpp
// faithful
this->field_0x184 = Globals::iPadHD ? 163
    : (Globals::iPadLarge ? 346
    : (Globals::iPad ? 173
    : (Globals::n9 ? 210
    : (Globals::retinaDisplay ? 232 : 116))));
```

This is a latent bug (the flags are normally mutually exclusive), but it is
a real difference from the binary.  It is **not** landable on the fuzzy
metric: the faithful shape keeps 7072 B but scores **48.704663 %**
(48.963317 → 48.704663), and the explicit `if/else` form of the same
ordering scores **47.727273 %** (§8).

## 5. Store-trace analysis — why the pool differs

`emu.run.last.trace` records the executed store order.  For the default
(all-flags-zero) path the original's store order is **exactly**
`_work/reconstructions/layout_ctor/order_default.txt` (204 offsets, checked
equal by `/dev/shm/w6964/checktrace.py`).  That file is therefore the
original's *compiled* store order, and `ios_source_order.md`'s list is the
original *source* order; the two differ by a scheduler permutation.

Store-position agreement with the original over all 32 combos:

| build | matching store positions |
|---|---:|
| base (hill-climbed source order) | **1744 / 6528 = 26.7 %** |
| iOS source order | **3172 / 6528 = 48.6 %** |

So the iOS order is structurally closer (as `ios_source_order.md` says), but
it compiles to **6864 B / 35.691 %** — 208 B short of 7072 B — and the
`order_default.txt` order compiles to **6740 B / 21.723 %**.  The base is a
hill-climbed order that trades store-order fidelity for hot-body instruction
alignment, which is what `difflib` actually rewards.

## 6. Measured variants (all real `link_so` builds, exact scorer)

Scorer `/dev/shm/w6964/score.py` reproduces the full-link number bit-for-bit
(base 7072 B / 48.963317384370 %).

### 6a. Expression / type edits

| variant | source change | size | ratio % |
|---|---|---:|---:|
| **base** | — | **7072** | **48.963317384370** |
| `184_ipad_first` | `field_0x184` iPad before n9 (faithful) | 7072 | 48.704663212435 |
| `184_ifelse` | same, explicit `if/else` chain | 7072 | 48.704663212435 |
| `184_ifelse_n9first` | explicit `if/else`, n9 before iPad | 7072 | 47.727272727273 |
| `a8_int` | `field_0xa8` helper `v` as int chain | 7032 | 48.831168831169 |
| `2c8_plain` | drop `(int)(float)` on `field_0x2c8` | 7064 | 47.695070844143 |
| `2d0_plain` | drop `(int)(float)` on `field_0x2d0` | 7072 | 48.963317384370 (no-op) |
| `150_plain` | drop `(int)(float)` on `field_0x150` | 7064 | 48.853439680957 |
| `160_plain` | drop `(int)(float)(int)` on `field_0x160` | 7064 | 48.942959712804 |
| `10c_plain` | drop `(int)(float)(int)` on `field_0x10c` | 7064 | 48.295114656032 |
| `90_variantD` | `field_0x90` = `(retina\|\|iPad\|\|n9) ? 0.325f : 0.65f` | 7072 | 48.963317384370 (no-op) |
| `154_ifelse` / `158_ifelse` / `16c_ifelse` | nested ternary → `if/else` | 7072 | 48.963317384370 (no-op) |
| `2d4_ternary` | `field_0x2d4` chain → ternary | 7064 | 48.654037886341 |
| `190_ternary` | `field_0x190` chain → ternary | 7060 | 48.434071414323 |
| `1cc_ternary` | `field_0x1cc` chain → ternary | 7072 | 48.883572567783 |
| `20c_ternary` | `field_0x20c` chain → ternary | 7116 | 47.885646217987 |
| `oitx_ternary` | `orbitInfoTextX` chain → ternary | 7116 | 47.885646217987 |
| `218_ternary` | `field_0x218` chain → ternary | 7088 | 47.450199203187 |
| `oisy_ternary` | `orbitInfoStationY` chain → ternary | 7088 | 47.450199203187 |
| `260_ternary` | `field_0x260` chain → ternary | 7152 | 45.894486314954 |
| `280_ternary` | `field_0x280` chain → ternary | 7072 | 48.883572567783 |
| `rby2_ternary` | `rewardBoxY2` chain → ternary | 7072 | 48.963317384370 (no-op) |

### 6b. Reorderings (source order is the only lever that moves the SLP groups)

| variant | source change | size | ratio % |
|---|---|---:|---:|
| **base** | — | **7072** | **48.963317384370** |
| `b0grp` | make `0xb0,0xb8,0xb4,0xbc` adjacent | 7056 | 48.108323377141 |
| `108grp` | make `0x108,0x10c,0x110,0x114` adjacent | 7016 | 44.098262432594 |
| `ecgrp` | make `0xec,0xf0,0xf4,0xf8` adjacent | 7032 | 43.261562998405 |
| `13cgrp` | make `0x13c,0x140,0x144,0x148` adjacent | 7004 | 46.462216877130 |
| `1d0grp` | make `0x1d0,0x1d4,0x1d8,0x1dc` adjacent | 7072 | 47.429254683141 |
| `2a0grp` | make `0x2a0,0x2a4,0x2a8,0x2ac` adjacent | 7048 | 43.677702433187 |
| `294grp` | make `0x294,0x298,0x29c,0x2a0` adjacent | 7016 | 43.858597962852 |
| `16cgrp` | make `0x16c,0x170,0x174,0x178` adjacent | 7072 | 48.963317384370 (no-op) |
| iOS source order | `ios_source_order.md` list | 6864 | 35.691188358933 |
| `order_default.txt` order | original compiled store order | 6740 | 21.723152307064 |
| iOS + `a8_int` | | 6824 | 36.595744680851 |
| iOS + `184` | | 6896 | 35.593220338983 |
| iOS + both | | 6848 | 35.708502024291 |

### 6c. Searches

| search | variants | best 7072-B result |
|---|---:|---:|
| random single-statement moves from the base | 150 | **48.963317384370** (no hit) |
| random two-statement moves, hill-climbed | 250 | **48.963317384370** (no hit) |
| random two-statement moves from the `184`-fixed base | 150 | **48.704663212435** (no hit) |

No 7072-B variant measured in this pass beats the base.

## 7. Proven vs hypothesis

**Proven (measured):**

* The original post-`pop` region is 966 B = 80 B cold + 2 B pad + 884 B
  (221 words) of literal pool; the base is 778 B = 16 B cold + 2 B pad +
  760 B (190 words).
* The end-pool composition is **50 groups + 14 scalars + 7 GOT = 221** in
  the original and **42 groups + 14 scalars + 8 GOT = 190** in the base.
* The 4-word multiset diff is 31 original-only / 23 base-only; after the
  16 window-shift pairs, **15 original-only / 7 base-only** remain, net
  **8 groups = 32 words**.
* The 31-word deficit decomposes exactly as `+32 groups − 1 GOT + 2
  (82f,41f) − 2 (0.325f,0.65f moved to a .text island)`.
* The 15/7 unpaired groups are the retina/default (and n9 `0xec`/`0x140`,
  iPadLarge `0x180`) runs listed in §3a/§3b.
* `0.325f`/`0.65f` are at `0xd2854/0xd2858` in the original and
  `0xd2548/0xd254c` in the base; `82.0f`/`41.0f` are at `0xd2ba0/0xd2ba4`
  in the base and absent from the original (int immediates instead).
* The original's default-path store order is exactly
  `_work/reconstructions/layout_ctor/order_default.txt` (204/204 equal).
* The base's store-order agreement with the original is 26.7 %; the iOS
  source order's is 48.6 %, yet it compiles to 6864 B / 35.7 %.
* Emulating all 32 flag combinations finds **exactly one semantic
  mismatch**: `field_0x184` for `n9=1 && iPad=1` (original 173, base 210),
  i.e. the original checks `iPad` before `n9` for that field.
* Every 7072-B variant measured this pass ties or loses to the base; 550
  random reorderings (150 single-move, 250 two-move, 150 two-move from the
  `184`-fixed base) found no improvement.

**Hypothesis (not proven):**

* The Android 2.0.16 source order is the iOS order, and the 208-B size gap
  between our iOS-order build (6864 B) and the original (7072 B) is caused
  by the expression/type differences (the `field_0xa8` int chain and the
  extra `(int)(float)` round-trips), not by a different statement order.
  The evidence is indirect: the original's *compiled* store order is a
  scheduler permutation of the iOS order, and our iOS-order build keeps
  source order instead of reproducing that permutation, which implies the
  expressions (and hence register pressure) still differ.
* The remaining SLP-group deficit is a symptom of that same expression /
  scheduling difference, not an independent knob: every attempt to move
  the six retina/default windows together shrinks the body and lowers the
  fuzzy score.

## 8. Why no source change landed

The only *correctness* improvement found is the `field_0x184` arm order
(§4).  It keeps the body at 7072 B but lowers the fuzzy ratio from
48.963317 % to 48.704663 %, and the gate's `byte_exact`/`linked_exact`
counts do not change either way (Layout is not byte-exact).  The task's
"body must stay 7072 B" is satisfied, but the metric moves the wrong way,
so the base was left untouched and only this note is committed.
