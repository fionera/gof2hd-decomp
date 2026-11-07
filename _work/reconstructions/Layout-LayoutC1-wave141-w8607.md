# `Layout::Layout` C1/C2 — wave 141 / worker w8607
## Whole-constructor move-to-position hill climb: raw diff 5,759 -> 5,728 at 7,072 B

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (image base 0x10000).
Baseline = wave-140 master `e18e0c2a4` (target-function raw same-address diff **5,759**,
`.text` differing bytes **86,472**).

## 1. NEW hypothesis

Previous waves hill-climbed only the **first 16 assignments** (wave-140 w8507) or probed
single structural flips (`0xdc` strd, tail group).  Wave 141 treats the whole constructor as
a permutation problem:

* Split the constructor body into 205 top-level statements (if/else chains kept as one unit,
  local declarations attached to their first use).
* Random *move-to-position* climb: pick a statement, move it 1..21 slots earlier/later,
  rebuild, keep the move only when the function stays at exactly **7072 B** and the raw
  same-address diff strictly decreases.
* Metric: raw same-address differing bytes (deterministic), not the hash-seeded fuzzy %.

This is a different search space from the wave-140 first-16 permutation: it moves statements
in the *common/cold* arm and the *tail* (units 60..204), which the earlier climb never touched.

## 2. Result — landed

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| target-function raw same-address diff | 5,759 | **5,728** |
| `.text` differing bytes | 86,472 | **86,441** |
| `byte_exact` / `linked_exact` | 4507 / 4507 | **4507 / 4507** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |
| symbol parity | 5370 / 5370 | 5370 / 5370 |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.
`drift` -> **0 drifted fields**.

Representative moves (source statement reorderings, all size-neutral):

* `field_0x58` before `field_0x54`
* `field_0xa4` before `hudInset`
* `field_0xf0` / `field_0xcc` hoisted above `field_0xc4` / `field_0xc0`
* `field_0x294_buttonWidth` before `driftSpeedDivisor`; `field_0x29c_buttonRowGap`
  after `field_0x2a0_listRowGap`
* `field_0x2b0` after `field_0x2a4_metricA`; `field_0x12c` before `field_0x110_strip58`
* `field_0x124` before `field_0x128`; `field_0x16c` before `field_0x178`
* `field_0x1b0` before `field_0x1bc`; `field_0x1c4` before `field_0x1c8`
* `field_0x1dc` / `field_0x1c0` / `field_0x1d4` moved *after* the `field_0x1cc` if-chain
* `field_0x238_blk16.a`, `orbitInfoSecurityY`, `field_0x214` hoisted above
  `quickMenuIconGap`

## 3. What did NOT work (measured, reverted)

| variant | size | raw diff |
|---|---:|---:|
| HEAD (wave-140) | 7072 | 5,759 |
| whole body sorted ascending by field offset | 6976 | 6,065 |
| first 16 in original HD SLP-group order | 7048 | 5,890 |
| every single adjacent swap (230 tried) | 7072 | no improvement |

The full-ascending and HD-group orders are both size-negative (clang forms extra `vst1`
groups), confirming the wave-140 conclusion: the original's cold (`iPadHD==false`) arm must
be reconstructed before the store-order reorder can land at 7,072 B.

## 4. Where the residual diff lives (HEAD, 128-byte blocks)

Arms `0x000..0x2d8` ~ 75-97 % differing; literal pool `0x2d8..0x548` ~ 35-98 % differing;
default arm + common tail `0x548..0x1880` ~ 96-100 % differing; last 0x1800..0x1ba0 ~ 34-46 %.
The dominant remaining blocker is still the ~4.9 KB default-arm/common-tail region, where the
original emits `vbsl`-vectorised ternaries (e.g. at `0x1090..0x1134`) that our scalar
`ldrb/cmp/it/moveq` chains do not reproduce.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8607
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8607/fdiff.py _ZN6LayoutC1Ev          # 7072/7072, diff 5728
```

Scratch harnesses (`/dev/shm/w8607/`, not committed): `lib.py` (constructor statement
splitter + build/score), `climb.py` (random move-to-position hill climb with size gate),
`exp1..exp4.py` (HD-group / ascending-order probes), `fdiff.py` / `dis.py` / `regions.py`
(raw diff, disassembly, diff-run census).

## 6. Open questions

* The `vbsl` block at `0x1090..0x1134` (fields `0x1b0..0x1e0`) is a vectorised select, not a
  ternary chain: reproducing it needs the original expression shape (likely
  `min`/`max` or a signed-shift idiom), not more reordering.
* The literal pool starts at `0x2d8` in the original but `0x2e8` in ours (16 B of extra
  pre-pool code); fixing that shifts ~370 pool bytes.
* Continuing the move climb from 5,728 with more seeds is cheap (~1 s/iteration) and still
  monotone; a longer run may reach the 5,7xx floor.
