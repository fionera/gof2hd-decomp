# `Layout::Layout` C1/C2 — wave 142 / worker w8708
## Continued whole-constructor move climb: raw same-address diff 5,728 -> 5,719 at 7,072 B

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base 0x10000.
Baseline = wave-141 master `1d10d0694` (target-function raw same-address diff **5,728**,
`.text` differing bytes **84,567**).

## 1. Hypothesis

Continue w8607's whole-constructor move-to-position hill climb from its 5,728 result with
more seeds and a longer monotone run; separately re-test w8608's `0xdc` strd flip on top of
the 5,728 shape.

## 2. Result — landed

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| `_ZN6LayoutC2Ev` size | 7072 | **7072** |
| target-function raw same-address diff | 5,728 | **5,719** |
| `.text` differing bytes | 84,567 | **84,558** |
| `byte_exact` / `linked_exact` | 4507 / 4507 | **4507 / 4507** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |
| symbol parity | 5370 / 5370 | 5370 / 5370 |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.
`drift` -> **0 drifted fields**.

The landed delta is four size-neutral statement moves on top of the w8607 permutation:

* `field_0x2a8_metricB` after `animSpeedScale` (swap);
* `field_0x270` after the `field_0x260` if/else chain;
* `rewardBoxY` after `field_0x28c` (still immediately before the `rewardBoxY2` chain that
  reads it);
* `field_0x284_sliderSlot5Enabled` after `field_0x318` (before `helpWindowX`).

All four are independent assignments; the `rewardBoxY` move keeps the `rewardBoxY2`
dependency ordered.

## 3. Search

Harness: Python statement splitter (202 top-level units: braceless `else` chains merged,
local declarations attached to their first use) + random move-to-position climb
(1..21 slots, `size == 7072` gate, strict raw-diff improvement, 25 % plateau walk on equal
diff).  ~0.8 s/iteration (incremental `link_so`).  Three independent runs from the 5,728
baseline:

| run | seed | move range | best |
|---|---:|---:|---:|
| main worktree | 7 | 1..21 | 5,724 |
| alt worktree | 11 | 1..21 | **5,720** |
| alt2 worktree | 23 | 1..80 | 5,728 (no improvement) |

The 5,720 order was then crossed with the 5,724 order's individual moves; only
`rewardBoxY` transferred, giving **5,719**.  A further 300-iteration climb from 5,719
(seed 31) did not improve it.

## 4. w8608 `0xdc` strd flip — tested, not landed

On the 5,728 shape, moving `field_0xdc`, `field_0x2b8`, `animSpeedScale`,
`driftSpeedDivisor`, `ringRadiusScale` immediately after `field_0xcc`:

| variant | size | raw diff | `0xdc` store (HD path) |
|---|---:|---:|---|
| 5,728 base | 7072 | 5,728 | one `vst1.32 {d2-d3}` |
| flip group after `field_0xcc` | 7024 | 5,953 | four `str.w` |
| flip group + `field_0xf0`/`field_0xf8` | 7024 | 5,840 | four `str.w` |

The flip is still **+112..+225** and does not reproduce the original's two `strd` pairs in
the HD arm (the scheduler emits `0xdc,0xe4,0xe0,0xe8` rather than `0xdc,0xe0,0xe4,0xe8`).
w8608's companion reorder was tuned to the wave-140 order; on the w8607 permutation it does
not transfer.  Not landed.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8708
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8708/fdiff.py _ZN6LayoutC1Ev          # 7072/7072, diff 5719
```

Scratch harnesses (not committed, `/dev/shm/w8708/`): `lib.py` (statement splitter +
build/score), `climb.py` / `climb_alt.py` / `climb_alt2.py` (move-to-position climbs),
`permtest.py` (flip probes), `fdiff.py` / `fdiff_alt.py` (raw same-address diff).

## 6. Open questions

* The climb is still monotone but the last three runs each found only 0..8 bytes; the
  local-move basin is nearly exhausted at 5,719.
* The dominant residual is still the default-arm/common-tail `vbsl` vectorised ternary
  block (`0xd1ac6..0xd1c14`, fields `0x1b0..0x1e0`): the original selects 4 fields at a
  time with `vbsl` masks built from `clz`/`vshl`, while our source emits scalar
  `ldrb/cmp/it/moveq` chains.  Reproducing it needs the original expression shape, not more
  reordering.
* The literal pool starts at `0x2d8` in the original but later in ours; aligning the common
  arm would shift ~370 pool bytes and make store-form flips (like `0xdc`) cheap.
