# Layout::Layout C1/C2 — wave 119 / worker w6821

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6821`, base = wave-119 master `d4d2d3f91`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable improvement found (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (full link) | 48.824232762 % | **48.824232762 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed. File size stays **2,187,328 B**; 27 section headers
unchanged.

## Why the body must stay exactly 7072 B

`byte_exact` compares each function's raw bytes, and every literal-pool word
in a GOT-relative `ldr/add` pair holds `GOT_slot - pool_address`.  Any change
to `Layout::Layout`'s size shifts `.text` and rewrites those words in every
following function: a 7076-B ctor drops `byte_exact` 4505 → 2454.  Every
candidate below was scored with the 7072-B constraint.

## Structural state (measured on HEAD)

* Original code (main body + cold outlined tails) = `0x1828` B; ours = `0x18a6` B
  (**+126 B**).
* Original literal pool = 221 words (`0xd2854..0xd2bc8`); ours = 190 words
  (`0xd28d0..0xd2bc8`) → **31 pool words (124 B) fewer**.
* Original has **4 cold outlined tails** (`0xd2802`, `0xd2828`, `0xd2836`,
  `0xd2844`, 80 B total); ours has **1** (`0xd28be..0xd28ce`, 16 B).
* Store mix: orig `strd` 62 / `str` 121 / `vst1.32` 81 / `vld1.64` 86;
  ours 47 / 192 / 70 / 77.  Ours has 11 fewer SLP vector stores.
* The first **81 reconciled instructions match exactly** (prologue + the
  first nine `vst1.32` groups `0x14,0x4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c`
  and the scalar `0x74/0x2c0/0x2d8/0xac/0x8c/0x94/0xa4` batch).  The first
  divergence is the order of the next scalar batch.
* Ours' block A is **12 B longer** than the original's (`beq.w` target
  `0x250` vs `0x244`), which shifts every later `loc_*` branch target.
* Original block-A SLP groups (27) vs ours (24).  Orig-only destinations:
  `0xc0, 0xb0, 0xec, 0x140, 0x150, 0x160, 0x170, 0x1c0` (plus the tail
  groups are shifted by one field: orig `0x204/0x214/0x224/0x23c/0x24c/0x25c/0x26c`
  vs ours `0x208/0x218/0x228/0x240/0x250/0x260/0x270`).
* The original merges `{0.325f, 0.65f, 100.0f, 50.0f}` into one 4-word
  `.rodata` island at `0xd2854`; ours keeps `{0.325,0.65}` at `0xd23d8` and
  `{100,50}` at `0xd28d0` (same words, different pool placement).
* Original block A ends `b.w 0xd21e2`; ours ends `b.w 0xd228c`.

## Best measured variants (full-link `difflib`, 7072 B required)

| variant | size | ratio % |
|---|---:|---:|
| HEAD (baseline) | **7072** | **48.824232762** |
| `0x130/0x134/0x138` as three nested ternaries | 7088 | 48.825169 (unlandable, +0.0009) |
| `0x130` chain: `if (iPadHD)` + nested-ternary else | 7072 | 48.824233 (identical codegen) |
| `0x130` chain: `if (iPadHD) / if (n9)` + ternary else | 7072 | 48.824233 (identical codegen) |
| `field_0x1cc` if/else → ternary | 7072 | 48.744520 |
| `field_0x280` if/else → ternary | 7072 | 48.744520 |
| `field_0x188` `23.0f` → `23` | 7072 | 48.824233 (no-op) |
| `field_0x18c` `244.0f` → `244` | 7072 | 48.824233 (no-op) |
| all `field_0x9c` cast/temp forms (5 variants) | 7072 | 48.824233 (no-op) |
| `||` operand order (`n9||retina`, `iPad||…`) | 7072 | 48.824233 (no-op) |
| drop `volatile` on the `retinaDisplay` pointer | 7064 | 48.773679 |
| `field_0x2d4` if/else → ternary | 7072 | 48.285486 |
| `field_0x190` if/else → ternary | 7060 | 48.494516 |
| `field_0x20c` if/else → ternary | 7108 | 47.199046 |
| `orbitInfoTextX` if/else → ternary | 7108 | 47.199046 |
| `field_0x218` if/else → ternary | 7080 | 47.619048 |
| `orbitInfoStationY` if/else → ternary | 7080 | 47.619048 |
| `field_0x260` if/else → ternary | 7144 | 45.497818 |
| `int v` arms → int literals | 7024 | 48.730762 |
| `field_0x204` moved before `0x208` | 7024 | 45.694834 |
| `field_0x94` moved before `0x98` | 7008 | 46.255507 |
| all 24 permutations of `{0x94,0x98,0x9c,0xa0}` positions | 6928–7088 | best = current (48.824233) |
| `0x140`/`0x144` moved next to `0x148` (3 forms) | 7000–7032 | 46.19–46.48 |
| `0x148..0x17c` address-order permutation | 7040 | 44.448882 |
| `0x140..0x1dc` address-order permutation | 6928 | 42.483923 |
| `0xb0`/`0xc0` reorders (6 forms) | 7008–7080 | 42.9–48.67 |
| `blk16.b` moved next to `blk16.c/d` (18 positions) | 6928–7016 | 40.8–46.52 |
| `field_0x2c4` moved next to `0x2c0` / to end (20 positions) | 6936–6992 | 34.4–45.6 |
| `0x130` chain moved to 24 positions | 6888–7072 | best = current position |
| retina/iPad arm swaps in `0x5c/0x60/0x64/0x68/0x6c` | 7056–7072 | 45.86–46.49 |
| duplicate tail re-store (12 fields) | 7000–7096 | 37.9–48.69 |
| random 1–4 unit moves, 8 workers × 800 s (~4.3k builds) | — | no 7072 hit > baseline |
| random 1–2 unit moves from the 7088 ternary variant, 8 × 300 s (~2.4k builds) | — | no 7072 hit > baseline |

## Proven vs hypothesis

**Proven (measured):**

* The current statement order is a strong local optimum: ~10k fresh builds
  (single moves ±20, all 24 permutations of the `0x94..0xa0` block, adjacent
  and random multi-moves) produced **no 7072-B variant above 48.824233 %**.
* The only variant that scores higher at all is the `0x130/0x134/0x138`
  nested-ternary form: **48.825169 %**, but it links at **7088 B** (unlandable
  without a compensating −16 B edit; no such edit was found).
* The 31 missing pool words are real: the original materialises eight more
  four-int SLP groups and merges `{0.325,0.65,100,50}` into one island.  Ours
  has the same constants but at different pool offsets.
* The original outlines four cold tails; clang 7.0.2 at `-Oz` outlines only
  one for our source shape.

**Hypothesis (not proven):**

* Matching the original's block-A schedule (so the `beq.w` target is `0x244`
  instead of `0x250`) would align every later `loc_*` branch target and is
  the single largest remaining score lever.  No source permutation tried here
  achieved it while keeping 7072 B.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6821
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch harnesses (not committed): `/dev/shm/w6821/{harness.py,diffsum.py,
diffshow.py,idx.py,vstdest.py,stores.py,pool.py,search1.py,search2.py,
search3.py,cast_sweep.py,exp*.py,units.py,perm184.py}`.
