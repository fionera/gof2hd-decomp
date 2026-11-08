# `Layout::Layout` C1/C2 — wave 154 / worker w10010
## Five single-statement moves found by re-running the exhaustive move sweep on
## the w9909 base: aligned **2,119 -> 2,006** (-113), raw 5,830 -> 5,849
## (+19), size stays **7,072 B**.  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-153 HEAD `4417db0b4` (w9909 landed): raw same-address diff
**5,830**, aligned instruction diff **2,119**, size 7,072.
Exit: raw **5,849**, aligned **2,006**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. Method: exhaustive single-move sweep, real-linked

w9910 swept the *ascending-only* base with a size-only compile filter.
This wave re-ran the full sweep on the landed w9909 base and, crucially,
measured the **aligned instruction diff** for every size-7,072 candidate.

* `split.py` splits the constructor body into **205 top-level statements**
  (if/else chains are one statement each).  A move `i -> j` removes
  statement `i` and re-inserts it before statement `j`.
* 41,820 variants per round.  Each variant is compiled with the exact
  NDK r18b clang 7.0.2 flags (0.26 s), then linked into a **mini shared
  object** (`ld.gold -shared` on just `Layout.o`, then `strip --strip-all`)
  so the literal pool is disassembled exactly like the stripped original
  (0.007 s), then `align_diff` (0.09 s).
* The mini-link proxy was validated against the real link: for the sweep
  population it reads **real_aligned = mini_aligned - 21** and
  **real_raw = mini_raw + 7** (e.g. baseline mini 2,140 / 5,823 vs real
  2,119 / 5,830; pre-w9909 mini 2,163 / 5,793 vs real 2,148 / 5,800).
  Every candidate with mini aligned <= 2,160 was then measured through the
  **real full gold link** (0.5 s each) before being accepted.
* The real-link measurement is `real_measure.py`: it re-runs the checked-in
  link command with only the `Layout.cpp.obj` input swapped, then strips and
  reorders `.shstrtab` exactly like the normal build.

## 2. The five landed moves (each round re-swept from the previous result)

Statement indices are relative to that round's source.

| # | move (round index) | source change | fn size | raw | aligned |
|---|---|---|---:|---:|---:|
| 0 | — | w9909 base | **7,072** | **5,830** | **2,119** |
| 1 | `114 -> 121` | `field_0x184` from after `field_0x194` to before `field_0x1a0` | 7,072 | 5,855 | **2,089** |
| 2 | `52 -> 59` | `marqueeWidth` from before `centerYBase` to before `oreTextYOffset` | 7,072 | 5,827 | **2,075** |
| 3 | `202 -> 198` | `field_0x284_sliderSlot5Enabled` from after `field_0x318` to before `field_0x310` | 7,072 | 5,833 | **2,051** |
| 4 | `15 -> 14` | `field_0x2c_rowHeight` before `field_0x38` (swap `0x38`/`0x2c`) | 7,072 | 5,927 | **2,014** |
| 5 | `8 -> 10` | `field_0x40` from before `buttonInsetX` to after it | 7,072 | **5,849** | **2,006** |

Every move is a pure statement relocation; no expression, constant or type
was edited.

Sweep sizes per round: 1,672 / 1,802 / 1,769 / 2,428 / 2,422 size-7,072
candidates.  The winning move was the unique minimum in round 1
(mini 2,102, next 2,135) and the clear minimum in each later round.

## 3. Why these moves are plausible source order

The original's store order supports the relocated fields (manual Thumb
disassembly of the original, function-relative):

* `0x184`: original stores at `d1e9a..d1ec4` in **ascending** order
  `0x180, 0x184, 0x188, 0x18c/0x190 (strd), 0x194/0x198/0x19c (stmia),
  0x1a0`.  Our source had `0x184` far earlier (before `0x1a4`/the `0x190`
  chain); move 1 puts it next to `0x18c`/`0x1a0`.
* `0x2c`/`0x38`/`0x3c`/`0x44`: original stores at `d160e..d163e` in
  ascending order `0x2c, 0x38, 0x3c, 0x44`; our source had `0x3c, 0x38,
  0x2c, 0x44`.  Move 4 swaps `0x38`/`0x2c`.
* `0x104` (`oreTextYOffset`): original stores it early at `d11da` (first
  device arm) and again at `d1a68`/`d1bdc`; `0xd4` (`marqueeWidth`) is
  stored later at `d1b3e`.  Move 2 moves `marqueeWidth` down past the
  early `0x104` store.
* `0x284`: not in the original's first device arm; move 3 places it just
  before the `0x310..0x318` block, matching the late `if/else` cluster.

## 4. Negative results (same sweep, real-linked)

Best size-7,072 alternatives that were **not** landed:

| round | move | raw | aligned |
|---|---|---:|---:|
| 1 | `104 -> 100` | 5,851 | 2,114 |
| 1 | `163 -> 167` | 5,825 | 2,118 |
| 2 | `151 -> 155` | 5,875 | 2,080 |
| 2 | `188 -> 195` | 5,864 | 2,081 |
| 3 | `202 -> 189` | **5,802** | 2,057 |
| 3 | `13 -> 16` | 5,945 | 2,057 |
| 4 | `200 -> 199` | 5,927 | 2,011 |
| 4 | `64 -> 67` | 5,896 | 2,013 |
| 5 | `199 -> 201` | 5,927 | 2,011 |

`202 -> 189` is the best raw-diff alternative seen (5,802) but costs +51
aligned versus the landed chain.

The mission's suggested groups were also covered by the exhaustive sweep:
no `0x2c4..0x2d8` or `0x7c..0x94` single-statement move reaches size 7,072
with aligned < 2,006 on this base (the best `0x7c..0x94`-area move is
`field_0x94` after `field_0x90`, which w9910 already measured at 2,216 on
the older base and is worse here too).

## 5. Remaining gap / next lever

The 124 B code/pool split from w9909 is unchanged: the original still has
221 pool words / 6,188 B of code, ours 190 / 6,312.  The SLP group bases
are still `b4/c4/dc/148/208` instead of `b0/c0/ec/140/204`.  The next lever
remains a device-major rewrite of `0x04..0x1e0` (the w9909 note's open
question), not more statement moves: five rounds of exhaustive single-move
search on the current base are now exhausted.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10010
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10010/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5849
python3 /dev/shm/w10010/align_diff.py _ZN6LayoutC1Ev   # 2006
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w10010/`: `split.py` (statement splitter), `sweep.py`
(41,820-variant compile + mini-link + align sweep), `align_so.py`
(align diff for an arbitrary .so), `align_obj.py` (object variant),
`real_measure.py` (full gold link of a single swapped object),
`measure_top.py` / `measure_all.py` (real-link confirmation),
`sweep_results_base*.json` (per-round results), `orig.dis` (original
disassembly of the function).
