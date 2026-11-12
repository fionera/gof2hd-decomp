# `Layout::Layout` C1/C2 — wave 191 / worker w13491

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191-w13491`, base master `f325e2d87` (raw **5,243**, aligned
**2,728**, size **7,072/7,072**, insns 2,521/2,552).

**Result: no size-exact raw win found.  `src/` left at the base.**  Gate re-run
green: `RATCHET PASS`, byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. Semantic check: the constructor is already exact for every config

The Thumb/VFP emulator (`_work/reconstructions/layout_ctor/emu.py`, pointed at
both `.so`s) was run for all **32** combinations of
`{iPadHD, n9, retinaDisplay, iPad, iPadLarge}`.  The resulting store maps
(`offset -> 32-bit value`) are **identical** for every combination.  So the
residual is purely instruction scheduling / vector grouping, not a wrong field
offset, value, or flag priority.

## 2. Exhaustive order search on the current body (all negative)

A compile-only proxy (`scratch/w13491/genproxy.py`, one plain
`void proxy_N(Layout*)` per variant, scored against the original bytes) was
calibrated against the linked build: proxy baseline **5,241** vs linked
**5,243**.  At a fixed function index the proxy reproduces the known w13458
block-move win (reverting it scores 5,245), so it is reliable for pure
statement-order changes.

| search | candidates | best raw |
|---|---:|---:|
| all pair swaps `(i,j)` | 20,706 | **5,241** (identity; no win) |
| all single moves `i -> j` | 41,412 | **5,241** (identity; no win) |
| late block moves, length 2-3, units 95..203 | 22,898 | **5,241** (neutral only) |
| early block moves, length 2-3, units 0..94 | 17,298 | **5,241** (neutral only) |

Every non-identity candidate is worse.  The current order is a very sharp
local optimum under all 1- and 2-statement moves and all length-2/3 block
moves across the whole constructor.

## 3. Structural residue: the original HD block vector groups

The original iPadHD block is `d1042..d1268` = **554 B / 164 insns**; ours is
`d1042..d127a` = **568 B / 169 insns** (+14 B).  The original groups stores
into NEON `vst1.32` in a different pattern:

| group | original `vst1` bases | ours |
|---|---|---|
| early | `0x4 0x14 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c` | `0x4 0x14 0x24 0x38 0x48 0x58 0x68 0x2c8 0x7c` |
| 0x294 | `0x294 0x2a4 0x108 0x118` | `0x294 0x2a4 0xdc` |
| late | `0x128 0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0` | `0x128 0x150 0x160 0x170 0x190 0x1a0 0x180 0x1b0 0x1d0` |

So ours is shifted by 4 bytes in the early group (`0x38..` instead of
`0x34..`), misses the `0x140` and `0x1c0` groups, and swaps `0x180/0x190`.
The original also stores `0xdc/0xe4` as `strd` where ours vectorizes
`0xdc..0xe8`.  These are the same "config-major vs flat-ternary" residue
already documented in w13383/w13420: the original source has an explicit
`if (iPadHD) { ... } else if (n9) { ... } ...` around the first ~130 fields,
while our flat ternary sequence lets clang merge a different set into the HD
block.

## 4. Structural experiments (all worse / not size-exact)

* 2-way split of the exact 130-field original HD set (`if (hd) { ... } else
  { false arms }`) with the current expressions: raw **6,361**, size
  **6,560**.
* 4/5-way split of the same set with literal per-config values: raw
  **6,725**, size **5,420**.
* HD set sorted ascending (current non-HD order preserved): raw **6,171**,
  size **6,792**.
* Moving `field_0x34` into the `0x38/0x3c/0x40` group (positions 9-13):
  raw 5,961-5,965, size 7,008.
* Moving the `0x140/0x144/0x148/0x14c` group together before `0x150`: raw
  5,955, size 7,000.
* Moving `0x1c0/0x1cc` next to `0x1c4/0x1c8`: raw 5,969, size 6,964.
* Moving `eventBannerScale` to the end of the HD region / file: raw
  5,923-6,012, size 6,828-6,920.

## 5. Expression experiments (neutral or worse)

* `field_0x90` collapsed to `(retina || iPad || n9) ? 0.325f : 0.65f`:
  **neutral** (5,243/7,072).
* `field_0x1e8` collapsed to `(retina || iPadHD || n9) ? 1 : 0`: **neutral**.
* `field_0x2d8` `retina || n9` -> `n9 || retina`: **neutral**.
* `helpWindowX` OR order / `iPad`-first form: neutral / raw 5,403, size 7,080.
* Removing the float round-trips on `0x150`, `0x160`, `0x2d0`: **neutral**;
  on `0x2c8` raw 5,420 size 7,064; on `0xa8` raw 5,378 size 7,064; on
  `0x10c` raw 5,403 size 7,064.
* Hoisting `const unsigned char rd_flag = Globals::retinaDisplay;` and
  replacing every use: raw **6,390**, size **6,456**.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13491
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5243 aligned=2728 size=7072/7072 insn=2521/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13491/`: `emu.py`,
`cmp_emu.py`, `genproxy.py`, `batchscore.py`, `sweep_pairs.py`,
`sweep_moves.py`, `sweep_blocks.py`, `sweep_blocks_early.py`, `gen_split.py`,
`gen_4way.py`, `orig.dis`, `ours.dis`.
