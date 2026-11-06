# `Layout::Layout` C1/C2 — wave 125 / worker w7501

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`
(constructor only).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Baseline = wave-124 final master `02513de33` (48.934051 %, 7072 B).

## Result — no source win

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.934051 % | **48.934051 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.ARM.extab` size/base | 0x26298 @ 0x1a3b78 | **0x26298 @ 0x1a3b78** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. Exact common1 CFG recovered (this pass)

I rebuilt the original CFG with a recursive capstone traversal (the original
has inline literal-pool islands after each unconditional branch, so a linear
sweep stops at `0xd12fe`; follow branches instead).  The common1 region is
`0xd1860..0xd21e2` (entered only when `!iPadHD`), 785 instructions /
2434 B, and is a **sequence of grouped if/else blocks**, not one tree:

| block | condition (flags: `n9`,`iPad`,`iPadLarge`,`retina`) | stores |
|---|---|---|
| `d1860` | all non-HD | `2d0 2d4 2d8` |
| `d18bc` | `n9` | `7c 80 84 88` (`vst1`) |
| `d1904` | `!n9` | `7c 80 84 88` (two `strd`) |
| `d193c` | all | `8c 90` |
| `d1962` | `n9` | `94` |
| `d1972` | `!n9` | `94` |
| `d199a` | all | `98 9c` |
| `d19ca` | all | `ac a8` |
| `d19e4` | `n9` | `a0 a4 d0 d4 d8 dc e0 e4 e8 fc 100 104 2b4 2b8 b0 b4 b8 bc c0 c4 c8 cc ec f0 f4 f8 294 298 29c 2a0 2a4 2a8 2ac 2b0 118 11c 120 124 108 10c 110 114 128 12c 130 134 140 144 148 14c 138 13c` |
| `d1ac6` | `!n9` | `d4 d0 a0 a4 d8 fc 100 104 298 dc e0 e4 e8 294 29c b0 b4 b8 bc c0 c4 c8 cc ec f0 f4 f8 2a0 2a4 2a8 2ac 108 10c 110 114 2b0 2b4 b8 118 11c 120 124 128 12c` |
| `d1c9a` | `!n9 && iPadLarge` | `130 134` |
| `d1cae` | `!n9 && !iPadLarge && retina && !iPad` | `130 134` |
| `d2802` (cold) | `!n9 && !iPadLarge && !retina` | `130 134` |
| `d1cba` | `!n9 && !iPadLarge` | `14c 138 13c 140 144 148` |
| `d1d2a` | all | `150` |
| `d1d38` | `iPadLarge` | `154 158 15c` |
| `d1d52` | `iPad` | `154` |
| `d1d62` | `n9` (in the `!iPadLarge && !iPad` arm) | `154` |
| `d1d9c` | `!n9` (same arm) | `154` |
| `d1da6` | all `!iPadLarge` | `158` (int phi -> `(int)(float)`) |
| `d1dd8` | all `!iPadLarge` | `15c` (int phi -> `(int)(float)`) |
| `d1e02` | all | `160` |
| `d1e18`/`d1e2a`/`d1e4c` | `iPadLarge` / `iPad` / `n9` | `164` |
| `d1e56` | all | `168` |
| `d1e64`/`d1ec8`/`d1efe`/`d1f1a` | `iPadLarge` / `iPad` / `n9` / `!n9` | `16c` |
| `d1f24` | all | `170` |
| `d1f64`/`d1f40` | `iPad` / `n9` | `174` |
| `d1f4e`/`d1f74`/`d1f96` | `iPadLarge` / `iPad` / `n9` | `178 17c` |
| `d1fa2` | all | `180` |
| `d1fb8`/`d1faa`/`d1fd4` | `iPadLarge` / `iPad` / `n9` | `184` |
| `d1fde` | all | `188` |
| `d1ffa`/`d1fe6`/`d2032` | `iPadLarge` / `iPad` / `n9` | `18c 190` |
| `d2042` | all | `194` |
| `d204c`/`d1ef2`/`d1ec0` | `iPadLarge` / `iPad` / `n9` | `198 19c 1a0` |
| `d2064` | all | `1a4` |
| `d2092` | all | `1a8` |
| `d20ac`/`d2184`/`d219c` | `iPadLarge` / `iPad` / `n9` | `1ac` |
| `d21a2`/`d20b4` | `n9` / `!n9` | `1b0 1b4 1b8 1bc 1c0 1c4 1c8 1cc 1d0 1d4 1d8 1dc 1e0` |

The four outlined cold tails after the `pop` are `d2802..d2852`:
`field_0x130/0x134` iPad arm, `t_field_0x2c8` iPad arm, `field_0x2d0`
iPad arm, `field_0x60` iPad arm.

The round-trip (`vcvt.f32.s32` / `vcvt.s32.f32`) fields in common1 are
`0x2d0 0x8c 0x9c 0xa8 0x138 0x150 0x158 0x15c 0x160 0x168 0x170 0x1a8`.
They survive only because the value is an int phi across a store-bearing
branch (e.g. `0x158` at `0xd1da6` reads `r4` set in the `154` arms).

## 2. Variants measured (real `link_so` builds, exact unrounded ratio)

Starting from the wave-121 config-major body (6672 B / 27.729914 %):

| variant | size | ratio % |
|---|---:|---:|
| wave-121 config-major (baseline for this branch) | 6672 | 27.729914 |
| + `(int)(float)` casts on the 11 round-trip fields | 6816 | 28.389662 |
| + `154/158/15c` if/else with int locals (`v158`,`v15c`) | 6848 | 28.180737 |
| + big `n9` group `a0..12c` if/else | 6832 | 28.321748 |
| + `1b0..1e0` group if/else | 6832 | 28.321748 |
| + `7c..88` group if/else | 6832 | 28.321748 |
| `154/158/15c` if/else alone (wave-121) | 6720 | 28.023952 |

The config-major branch reaches the original's common1 size exactly
(2434 B, 19 `vcvt`) but its **common1-only** difflib ratio is only
**16.4 %** — the branch layout and SLP grouping are still different, so the
whole-function score stays far below the field-major baseline.

Field-major (current body) variants:

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.934051** |
| big `a0..12c` group as `if (iPadHD)/n9/else` (current order) | 7028 | 37.094220 |
| same, reordered to the original HD order | 6936 | 33.526474 |
| `7c..88` group as if/else (current order `84 80 7c 88`) | 7096 | 47.524359 |
| `7c..88` reordered `7c 80 84 88` (no if/else) | 7072 | 48.814505 |
| `2d8` moved before `2d4` | 7072 | 48.814505 |
| `2d0/2d4/2d8` contiguous, `2cc/2c8` moved after | 7080 | 47.180713 |
| common1 first part reordered to the compiled default order | 7220 | 17.692907 |
| first group reordered to the original HD order | 7044 | 30.099800 |
| `0x170` / `0x15c` / `0x94` / `0x1f4` wrapped in `(int)(float)` | 7072 | 48.934051 (folded) |
| `0x138` as `(int)(float)v138` | 7088 | 48.616365 |
| `0x2d8` as `(int)(float)` | 7088 | 48.885350 |
| `0x1e4` as `(int)(float)` | 7016 | 42.275449 |
| `0x2d0` / `0x8c` via int/float locals | 7072 | 48.934051 (folded) |

## 3. Remaining root cause

The original's common1 is a grouped if/else tree with duplicated stores in
the arms and int phis that keep the 12 round-trips.  The current body is
field-major and already contains the same grouped if/else blocks for
`0x130/0x134/0x138`, `0x190`, `0x1cc`, `0x20c`, `orbitInfoTextX`, `0x218`,
`orbitInfoStationY`, `0x260`, `0x280`, `rewardBoxY2` etc.; the only fields
still flat are the ones the original also emits as per-field selects.
Converting the remaining flat groups (`a0..12c`, `7c..88`) to if/else
**shrinks** the body (the compiler merges the arms) and drops the score,
because the 7072-B wall is a knife edge and the current statement order is a
strong hill-climbed local optimum of the fuzzy metric.

The true config-major body is structurally faithful but scores 28 %, not
48.9 %: the fuzzy metric rewards the field-major body's long matching HD
prefix.  Reaching a byte-identical body needs the original's exact
branch-outline/SLP decisions (the 400-B code gap), which this pass did not
recover.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7501
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7501/score.py                         # 7072 / 48.934051
```

Scratch harnesses (not committed, `/dev/shm/w7501/`): `cfg2.py` (recursive
capstone CFG of the original, handles inline pools), `conds.py` (per-block
reachability truth table over the 32 flag combinations), `trace2.py`
(per-block store sets), `genfull.py` (config-major body generator with
casts/groups), `trybig_cur.py` / `reorder_common1.py` / `reorder_first2.py`
(field-major reorders), `variants.py` / `v2.py` (round-trip variants).
