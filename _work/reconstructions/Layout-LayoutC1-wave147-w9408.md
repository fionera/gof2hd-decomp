# `Layout::Layout` C1/C2 — wave 147 / worker w9408
## Device-major tail reconstruction: the 0x164 block can be moved to 0xe02 but not
## 0xdf4 without shrinking the shared tail by ~0xea; explicit if/else duplication is
## counter-productive. Landed a 6-swap reorder that improves align_diff 2304 -> 2213
## at exact size 7072 (raw 5704 -> 5743).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.  Baseline (wave-147
master `cbe3b5f7f`): raw same-address diff **5704**, align_diff **2304**
non-matching instructions, size 7072.

## 1. Where the 0x164 block actually sits

The original's 0x164..0x1a0 block is at `d1e16..d1ec4` (function offset
`0xdee..0xe9c`); the first `0x164` store is at `d1e1c` (**offset 0xdf4**).  It is
inside the *shared !iPadHD tail* (`d1860..d21e2`).  That tail is itself
device-major: the compiler hoisted `iPadLarge` for `0x16c..0x1a0` (constant arm
`d1e64..d1ec4`, per-field arm `d1ec6..d1f5c`) and `iPad|iPadLarge` for
`0x164/0x168` (`d1e16..d1e56`).

Our baseline has the first `0x164` store at `d1f00` (**offset 0xed8**), i.e.
0xe4 bytes later.  Decomposing the shift by field:

| field | orig off | ours off | delta |
|---|---:|---:|---:|
| 0x2d0 | 0x85a | 0x808 | -0x52 |
| 0x2d4 | 0x88e | 0x8ac | +0x1e |
| 0x7c  | 0x90c | 0x93c | +0x30 |
| 0x138 | 0xcfa | 0xda0 | +0xa6 |
| 0x14c | 0xcec | 0xdd6 | +0xea |
| 0x164 | 0xdf4 | 0xeb8 | +0xc4 |

So the *shared tail before 0x14c* is ~0xea bytes longer in ours; the 0x14c..0x164
span is actually ~0x1a bytes shorter.  The whole discrepancy is the shared tail,
not the 0x164 block itself.

## 2. Why the original is bigger despite a more compact tail

Original code ends at `d2852` (pool `d2854..d2bc8`, **968 B / 242 words**); ours
ends at `d28c0` (pool ~776 B / 194 words).  The original's extra pool words and
`movs` are the duplicated device arms (HD/n9/default early arms plus the
iPadLarge sub-arms).  This is the same "less-vectorised, duplicated-arm" shape
noted by waves 143/145/146.

## 3. Reorder experiments (all real `link_so` + fdiff + align_diff)

| variant | source change | size | raw | align |
|---|---|---:|---:|---:|
| HEAD baseline | — | **7072** | **5704** | **2304** |
| v13 (=w9307) | `0x180..0x1a0` ascending | 7080 | 5873 | 2421 |
| C | full `0x164..0x1ac` ascending | 7080 | 5873 | 2421 |
| F | `0x140/144/148/14c` group only | 7000 | 5852 | 2744 |
| N | `0x148` before `0x14c` only | 7008 | 5769 | 2270 |
| A | group + `204/2bc` moved + ascending | 6948 | 5743 | 2552 |
| B | group + `204/2bc` moved + ascending (v13) | 7008 | 5783 | 2325 |
| M | whole ctor in `order_default.txt` order | 6700 | 5787 | 3594 |
| T | `0x2d4` block before `0x2d8` | 7064 | 5802 | 2304 |
| E | explicit `iPadLarge` if/else for `0x16c..0x1a0` | 7152 | 6353 | 4122 |
| J | explicit `n9` if/else for `0x164..0x1a0` | 6944 | 6047 | 3371 |
| U | explicit `n9` if/else for `0x16c..0x178` | 7200 | 6160 | 4179 |

Key negative results:
* Making `0x180..0x1a0` ascending is worth 8 bytes in window 14 (250 -> 242) but
  costs +8 B overall and does not move `0x164` to 0xdf4 (it only moves it from
  0xed8 to 0xedc).  The preceding shared tail dominates.
* Joining `0x140/0x144/0x148/0x14c` into one SLP group is worth ~72 B (not 104),
  and it moves `0x164` to ~0xe50 (variant B) — still 0x5c short.
* **Explicit source-level device duplication does not reproduce the original
  arms**: writing `if (iPadLarge) { constants } else { per-field }` (or `n9`)
  makes clang emit a large diamond (size 7152/7200, align > 4100), because the
  source-level branch is not the same IR shape as if-converted selects with PGO
  branch weights.  The original's duplication is a *backend* effect of flat
  ternaries + profile weights, not a source-level if/else.
* Full `order_default.txt` reorder shrinks to 6700 and worsens align to 3594:
  the current hill-climbed order is a good local optimum for align.

## 4. Landed: 6 adjacent reorderings (size-exact, gate-green)

A local search over adjacent statement swaps, constrained to size 7072, found a
chain that lowers align_diff while keeping the exact size:

| swap | statements exchanged |
|---|---|
| 1 | `centerYBase` <-> `marqueeWidth` |
| 2 | `field_0x238_blk16.b` <-> `field_0x120` |
| 3 | `field_0x198` <-> `field_0x1a8` |
| 4 | `field_0x1c8` <-> `field_0x1d8` |
| 5 | `field_0x1c0` <-> `field_0x1d4` |
| 6 | `orbitInfoSecurityY` <-> `field_0x214` |

All are pure statement reorderings (expressions unchanged).  Result:

| metric | before | after |
|---|---:|---:|
| size | 7072 | **7072** |
| raw same-address diff | 5704 | **5743** (+39) |
| align_diff (non-matching insns) | 2304 | **2213** (-91) |
| verify avg_match | 99.94 | 99.93 |

Gate: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

The raw metric regresses by 39 bytes because the reorder shifts code later in
the function; the instruction-index alignment (which normalises branch targets)
improves by 91 instructions.  This is a structural-alignment win, not a raw-byte
win.

## 5. Open questions

* To put `0x164` at 0xdf4 the shared tail must shrink by ~0xea.  That needs the
  original's PGO branch weights (the duplicated HD/n9/iPadLarge arms), which are
  not expressible with `__builtin_expect` on the ternaries alone (tested:
  `expect(iPadHD,1)` gives 7020 / 6451 / 4208).
* The original's 242-word pool vs our 194 is the size budget that a real
  device-major reconstruction must spend; explicit if/else spends it in the
  wrong shape.
* `0x148` joining `0x13c..0x148` (rather than pairing with `0x14c`) needs the
  group to be emitted in the shared tail, which only happens with the full
  `0x13c..0x1ac` reorder (variant A/B), and that reorder is size-negative.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9408
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9408/fdiff.py _ZN6LayoutC1Ev      # 7072/7072, diff 5743
python3 /dev/shm/w9408/align_diff.py _ZN6LayoutC1Ev # 2213
```

Scratch (`/dev/shm/w9408/`): `Layout.base.cpp`, `varA..varW.cpp`,
`hillbest2.cpp`, `mkv2.py`, `hill*.py`, `combo.py`, `fdiff.py`,
`align_diff.py`, `win.py`, `orig.dis`, `base.dis`.
