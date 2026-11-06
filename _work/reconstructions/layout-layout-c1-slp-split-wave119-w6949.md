# Layout::Layout C1/C2 — wave 119 / worker w6949
## SLP-group enumeration and the "split with a non-int store" experiment

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6949`, base = wave-119 master `e2cd5f80f`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

This pass continues the SLP thread of `layout-layout-c1-c2-wave119-w6912.md`
(config-major arm + window shift) and `layout-layout-c1-coldblocks-wave119-w6932.md`
(cold tails).  It enumerates the end-pool `vld1.64` groups of both binaries,
pairs them by the 16-byte window shift, identifies the exact source store
quadruples behind every unpaired group, and runs the requested
"split it with a non-int store / different type" experiment.

## Result — note only, no source change landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.963317384 % | **48.963317384 %** |
| `report.json` pct (1 dp) | 49.0 | 49.0 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the 27 section headers, the file size
(**2,187,328 B**), `.text` (`0x1337c8`), `.ARM.exidx` (`0x6f70`),
`.ARM.extab` (`0x26298`, exact), `.data` (`0x296c`) and `.rel.plt`
(`0x72d0`) are unchanged.  `tools/verify_baseline.json` was **not** touched.

Every 7072-B variant measured this pass scores **exactly 48.963317384 %**
(the base): all simple type/cast/temp edits are codegen no-ops, and every
edit that actually changes the SLP grouping also changes the body size.
The best raw score seen at any size is still the base.

## 1. Enumeration method

`/dev/shm/w6949/enum_raw.py`: walk the Thumb disassembly, track
`add/adds/addw rX,pc,#imm` (pool address = `((a+4)&~3)+imm`), and record every
`vld1.64 {dA-dB}, [rX]` whose pool address is `>= 0xd2800` (the function's
end literal pool).  The 4 words are read from the ELF VMA.  A second script
(`stores.py`) tracks the d-register contents through `vst1.32` / `vbsl` /
`vmov` to classify each group as a direct store or a `vbsl` operand.

| | original | base (HEAD) | delta |
|---|---:|---:|---:|
| total `vld1.64` in the function | **86** | **77** | +9 |
| `vld1.64` loading from the end pool (`>= 0xd2800`) | **50** | **42** | **+8** |
| `vst1.32` in the function | 81 | 70 | +11 |

So the task's "8 extra SLP groups" is exactly the **raw end-pool count
difference 50 − 42 = 8** (it is *not* 8 groups that our build has extra:
our build has **fewer** groups than the original).

## 2. Full multiset diff and window-shift pairing

Comparing the 4-word value multiset of the 50 vs 42 end-pool groups
(`pair.py`, values not destinations) gives **31 original-only** and
**23 base-only** groups.  Pairing each base-only group with an
original-only group where one is the other shifted by one word
(`ours [a,b,c,d]` ↔ `orig [x,a,b,c]` or `orig [b,c,d,y]`) resolves
**16 window-shift pairs** (the 7 HD + 7 n9 `0x204..0x26c` runs, plus the
`0x23c..0x24c` pair), leaving **15 unpaired original-only** and
**7 unpaired base-only** groups.  15 − 7 = **8**, the net count.

### 2a. The 7 unpaired base-only groups (groups our build has, original does not)

These are the groups the "split with a non-int store" experiment targets
(the 8th group of the raw count is the float group below, which is a
`vld1.64` of four *float* constants, not four ints).

| # | dest | 4 words | source store quadruple (source lines) | arm |
|---|---|---|---|---|
| A | `0xdc` | `[15.0, 1.5, 12.0, 1.5]` (float) | `field_0xdc`(93), `animSpeedScale`(96), `driftSpeedDivisor`(88), `ringRadiusScale`(102) | n9 |
| B | `0x294` | `[400, 168, 24, 24]` | `field_0x294_buttonWidth`(91), `field_0x298_buttonYBias`(95), `field_0x29c_buttonRowGap`(90), `field_0x2a0_listRowGap`(92) | retina (`vbsl` operand) |
| C | `0x294` | `[200, 84, 12, 12]` | same quadruple | default (`vbsl` operand) |
| D | `0x16c` | `[131, 124, 42, 60]` | `field_0x16c`(152), `field_0x170`(155), `field_0x174`(153), `field_0x178`(151) | iPad |
| E | `0x16c` | `[115, 106, 45, 27]` | same quadruple | n9 |
| F | `0x180` | `[138, 128, 60, 44]` | same quadruple (`vbsl` operand, final store at `0x16c`) | retina |
| G | `0x180` | `[69, 64, 30, 22]` | same quadruple (`vbsl` operand, final store at `0x16c`) | default |

So the 7 groups come from only **three source quadruples**: the float
`0xdc..0xe8` run (1 group), the `0x294..0x2a0` run (2 groups), and the
`0x16c..0x178` run (4 groups: iPad, n9, retina, default).  The original
does **not** vectorise any of these arms; it vectorises the *iPadLarge*
arm of the `0x16c..0x178` run instead (`[262,248,84,120]`, see below).

### 2b. The 15 unpaired original-only groups

| # | dest | 4 words | source store quadruple | arm / role |
|---|---|---|---|---|
| 1 | `0xec` | `[60, 120, 300, 600]` | `ringNearMidThreshold`(100), `field_0xf0`(85), `ringMidFarThreshold`(94), `field_0xf8`(89) | n9, direct store |
| 2 | `0x140` | `[19, 19, 19, 243]` | `field_0x140`(297), `field_0x144`(287), `field_0x148`(143), `field_0x14c`(140) | n9, direct store |
| 3 | `0xb0` | `[6, 29, 36, 16]` | `field_0xb0`(71), `field_0xb4`(78), `field_0xb8`(77), `field_0xbc`(83) | retina, `vbsl` operand |
| 4 | `0xb0` | `[3, 15, 18, 8]` | same quadruple | default, `vbsl` operand |
| 5 | `0x108` | `[-2, 2, 1, -1]` | `field_0x108`(115), `field_0x10c_rowBaseY`(111), `field_0x110_strip58`(104), `field_0x114_strip5c`(114) | retina, `vbsl` operand |
| 6 | `0x108` | `[0, 1, 2, 1]` | same quadruple | default, `vbsl` operand |
| 7 | `0x2a0` | `[24, 220, 200, 210]` | `field_0x2a0_listRowGap`(92), `field_0x2a4_metricA`(101), `field_0x2a8_metricB`(97), `field_0x2ac_metricC`(99) | retina, `vbsl` operand |
| 8 | `0x2a0` | `[12, 110, 100, 105]` | same quadruple | default, `vbsl` operand |
| 9 | `0xec` | `[80, 160, 400, 800]` | `ringNearMidThreshold`, `field_0xf0`, `ringMidFarThreshold`, `field_0xf8` | retina, `vbsl` operand |
| 10 | `0xec` | `[40, 80, 200, 400]` | same quadruple | default, `vbsl` operand |
| 11 | `0x13c` | `[34, 22, 26, 4]` | `field_0x13c`(139), `field_0x140`(297), `field_0x144`(287), `field_0x148`(143) | retina, `vbsl` operand |
| 12 | `0x13c` | `[17, 11, 13, 2]` | same quadruple | default, `vbsl` operand |
| 13 | `0x180` | `[30, 346, 30, 446]` | `field_0x180`(173), `field_0x184`(157), `field_0x188`(172), `field_0x18c`(175) | iPadLarge, direct store |
| 14 | `0x1d0` | `[10, 24, 8, 20]` | `field_0x1d0`(188), `field_0x1d4`(190), `field_0x1d8`(186), `field_0x1dc`(191) | retina, `vbsl` operand |
| 15 | `0x1d0` | `[5, 12, 4, 10]` | same quadruple | default, `vbsl` operand |

All 15 are the *else-path* (`retinaDisplay`/`iPad`/`iPadLarge`) operands or
direct stores.  The original's else path is a vectorised select chain
(`vbsl`), which is the config-major shape described in w6912; our build's
else path uses scalar `movs`/`moveq`/`str` for most of these fields and
vectorises a different subset.

## 3. The split experiment (all real builds, exact scorer)

Method: `/dev/shm/w6949/fast.sh <variant.cpp>` copies the variant over
`src/game/ui/Layout.cpp`, runs the real incremental `link_so` (gold + GNU
strip), and scores `_ZN6LayoutC1Ev` with the `tools/verify/verify.py`
`Resolver` + `reconcile` + `difflib(autojunk=False)` pipeline
(`/dev/shm/w6949/score.py`).  It reproduces the full-link number bit-for-bit
(base 7072 B / 48.963317384 %).  ~0.9 s/variant.

### 3a. Cast / type changes on the 7 unpaired groups' source stores

`(int)(float)(X)`, `(int)(double)(X)`, `(int)(long long)(X)`,
`(long)(X)`, `(short)(X)`, `(unsigned)(X)`, a local `int` temp, and a local
`float` temp were applied to **every** field of the three quadruples
(12 fields × 9 forms = 108 builds).  Because the assigned values are
compile-time constants, clang 7 folds all of them and the codegen is
identical:

| form | fields that stay 7072 B / 48.963317 | fields that change size |
|---|---|---|
| `(int)(float)(X)` | 27 of 32 tested fields | `0xf8`→7000, `0xb4`→7080, `0x10c`→7096, `0x180`→7088, `0xdc`→7088, `0xe4`→7016 |
| `(int)(double)(X)` / `(int)(long long)(X)` | all 12 group fields | — |
| `(long)` / `(short)` / `(unsigned)` | all group fields except `0x148` short/unsigned, `0x114` unsigned | `0x148`→7064, `0x114`→7056 |
| local `int` temp | all 12 group fields | — |
| local `float` temp | all 12 group fields | — |
| float retina ternary (`retina ? A.0f : B.0f`) | 10 of 12 group fields | `0x29c`→7080, `0x174`→7184 |
| double n9 literal (`15.0` instead of `15.0f`) | all 4 float fields | — |

**No type/cast/temp edit both keeps 7072 B and changes the instruction
stream.**  The 7072-B variants score exactly 48.963317384 %, i.e. the
groups are still merged.

### 3b. Forcing an actual split

The only way to make clang 7 stop merging is to move a store out of the
basic block.  Moving `field_0x174` to the end of the ctor does split the
`0x16c..0x178` quadruple: the end-pool group count drops **42 → 37**
(the four `Q16c` groups plus one more disappear), but the body becomes
**6928 B / 34.455 %** — unlandable (any size change from 7072 B collapses
whole-`.so` `byte_exact`).

| variant | source change | size | ratio % |
|---|---|---:|---:|
| base | — | **7072** | **48.963317** |
| `split174` | move `field_0x174` (line 153) to end of ctor | 6928 | 34.454967 |
| `field_0x174` → position 140 | | 7024 | 47.680000 |
| `field_0x16c` → position 70 | | 7072 | 44.493129 |
| `field_0x16c` → position 180 | | 7072 | 44.284859 |
| `field_0x178` → position 120 | | 6968 | 48.141451 |
| `field_0x170` → position 120 | | 6952 | 46.971222 |

### 3c. Reorders and explicit if/else

| variant | source change | size | ratio % |
|---|---|---:|---:|
| `Q16c_asc` | reorder `0x16c,0x170,0x174,0x178` ascending | 7072 | 48.963317 (no-op) |
| `Q16c_desc` | reorder descending | 7080 | 48.854353 |
| `Q294_asc` | reorder `0x294,0x298,0x29c,0x2a0` ascending | 7064 | 48.714371 |
| `Q294_desc` | reorder descending | 7056 | 44.023904 |
| `Q240_asc/desc` | reorder `blk16.c,blk16.d,0x248,0x24c` | 7072 | 48.963317 (no-op) |
| explicit if/else (one field) | `int tv; if (retina) tv=A; else tv=B;` | 6972–7136 | 22.76–38.03 |
| `0x204` before `0x208` | move `field_0x204` (line 146) | 7040 | 45.378151 |
| `0x294` → `0x298` +1 | adjacent move | 7072 | 48.963317 (no-op) |
| `0x16c` → ±1..3 | adjacent moves | 7072 | 48.963317 (no-op) |
| `0xdc`/`anim`/`drift`/`ring` → +1 | adjacent moves | 7072 | 48.963317 (no-op) |

The `Q16c_asc` and `Q240_asc` reorders are codegen no-ops because clang
already schedules those stores in ascending offset order.  `Q294_asc`
shrinks the body by 8 B.

## 4. Proven vs hypothesis

**Proven (measured):**

* The raw end-pool `vld1.64` count difference is exactly **50 − 42 = 8**
  (86 vs 77 total).  The `(4-word)` multiset diff is 31 original-only vs
  23 base-only; 16 of those are the 16-byte window shift
  (`orig 0x204.. ↔ base 0x208..`), leaving 15 unpaired original-only and
  7 unpaired base-only groups (net 8).
* The 7 base-only groups come from only three source quadruples:
  `0xdc..0xe8` (float, n9), `0x294..0x2a0` (retina + default), and
  `0x16c..0x178` (iPad, n9, retina, default).  The original vectorises the
  **iPadLarge** arm of `0x16c..0x178` (`[262,248,84,120]`) and the
  **iPadLarge** arm of `0x180..0x18c` (`[30,346,30,446]`) instead.
* Every simple cast/type/temp edit on those stores is a codegen no-op
  (clang 7 folds compile-time constants); 7072-B variants score exactly
  48.963317384 %.
* Forcing a real split (move `field_0x174` out of the block) removes the
  groups (42 → 37) but changes the body to 6928 B.
* Every 7072-B variant found this pass ties the base; no variant beats
  48.963317384 %.

**Hypothesis (not proven):**

* The original's else path is a vectorised `vbsl` select chain: the 15
  unpaired original-only groups are mostly the retina/default *operands*
  of `vbsl` selects (`0xb0`, `0x108`, `0xec`, `0x13c`, `0x1d0`, `0x2a0`),
  while our build emits scalar `movs`/`moveq`/`str` for the same fields.
  Reproducing them needs the original's config-major source order, which
  w6912 measured at 6984 B (`vm2`) — the windows match but the body is
  88 B short.  The 8-group deficit is therefore a *symptom* of the
  source-order difference, not an independent knob.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6949
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6949/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6949/ours.dis

python3 /dev/shm/w6949/enum_raw.py     # raw 50 vs 42 + multiset diff
python3 /dev/shm/w6949/pair.py         # 16 window-shift pairs, 15/7 unpaired
python3 /dev/shm/w6949/stores.py       # store/consumer classification
python3 /dev/shm/w6949/score.py        # single-function exact score
bash    /dev/shm/w6949/fast.sh <variant.cpp>
```

Scratch harnesses (not committed, `/dev/shm/w6949/`): `enum_raw.py`,
`enum8.py`, `stores.py`, `pair.py`, `find8.py`, `loads.py`, `order.py`,
`harness.py`, `score.py`, `fast.sh`, `try.py`, `exp2.py`…`exp16.py`,
`mkvar.py`, `Layout.base.cpp`, `orig.dis`, `ours.dis`, `show.txt`,
`v_split174.cpp`.
