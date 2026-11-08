# `Layout::Layout` C1/C2 — wave 155 / worker w10106
## Re-ran the exhaustive single-statement move sweep from the w10010 base
## (240 top-level statements, 57,360 variants, real-linked).  Five pure
## statement moves land: raw **5,849 -> 5,837** (-12), aligned **2,006 ->
## 1,986** (-20), size stays **7,072 B**.  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-154 HEAD `f4e0ea40b` (w10010 landed): raw same-address diff
**5,849**, aligned instruction diff **2,006**, size 7,072.
Exit: raw **5,837**, aligned **1,986**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. Method

The w10010 note's five-move chain was found by re-sweeping the *w9909* base.
This wave re-ran the same exhaustive sweep on the **w10010 base**:

* `split.py` splits the ctor body into **240 top-level statements**
  (if/else chains are one statement each; `else if` arms are separate).
* Every single move `i -> j` (57,360 variants) is compiled with the exact
  NDK r18b clang 7.0.2 flags (0.27 s), filtered on the object's
  `_ZN6LayoutC2Ev` size == 7,072, then mini-linked (`gold -shared` on the
  single object + `strip --strip-all`) and measured with the committed
  `align_diff.py`.  2,430 variants keep size 7,072.
* The mini-link proxy reads `mini_aligned = real_aligned + ~17..23`; the
  top 42 (`mini_aligned <= 2028`) were re-measured through the **real full
  gold link** (`link_so` + `fdiff.py` + `align_diff.py`).
* A second sweep round was then run from the best landed base
  (`[67,70]+[125,127]`), 57,360 more variants, 2,559 size-7,072 hits; its
  top valid candidates were composed and re-measured.

## 2. The five landed moves (original statement indices)

All are pure statement relocations; no expression, constant or type edited.

| # | move | source change | raw | aligned |
|---|---|---|---:|---:|
| 0 | — | w10010 base | **5,849** | **2,006** |
| 1 | `67 -> 70` | `field_0xc0` after `field_0xc8`/`oreLabelYOffset`, before `field_0x294` | 5,818 | 2,001 |
| 2 | `125 -> 127` | swap the `iPad` and `retinaDisplay` arms of the `field_0x190` chain | 5,818 | 2,001 |
| 3 | `234 -> 236` | `field_0x310` after `field_0x30c`, before `field_0x314` | 5,818 | 1,998 |
| 4 | `77 -> 74` | `animSpeedScale` before `field_0xdc` | 5,858 | 1,996 |
| 5 | `217 -> 223` | `field_0x28c` after `field_0x1f8`, before `field_0x140` | **5,837** | **1,986** |

Moves 1-2 were measured on the w10010 base; moves 3-5 on the resulting base
(indices are that round's source).  The final source is exactly
`[67,70],[125,127],[234,236],[77,74],[217,223]` applied in order.

## 3. Why these moves are plausible source order

* `field_0xc0`: the original emits its `vst1.32` group at `0x1a80`, i.e.
  **after** the `0x294`/`0x2a4` groups (`0x1182`/`0x1190`).  Our source had
  `0xc0` before `0x294`; the move pushes it past `0xc8`/`oreLabelYOffset`
  toward the original's late position.
* `field_0x190` chain: the original's `-4` (iPad) and `-2` (retina) arms
  are both materialised in the shared `0x18c/0x190` tail (`mvn.w r1,#3` /
  `mvn.w r3,#1` around `d1fe8..d2010`); the exact arm order is not
  recoverable from the merged tail, but the swap is a pure source-order
  change and is the only in-chain move that improves both metrics.
* `field_0x310`: the original stores `0x30c`/`0x310`/`0x314` in ascending
  order (`0x30c` first).  Our source had `0x310` before `0x30c`.
* `animSpeedScale` (`0xe4`): the original's float block starts at `0xec`;
  moving `animSpeedScale` next to `field_0xdc` groups the float
  initialisers and helps the shared-tail scheduler.
* `field_0x28c`: a plain `n9 ? -19 : 0` store; the original places it in
  the reward-box tail next to `0x1f8`/`0x140`.

## 4. Store / pool tables (final build)

`vst1.32` group bases, original vs ours (iPadHD arm then shared tail):

```
ORIG  iPadHD: 14 4 24 34 44 54 64 2c8 7c c0 b0 ec 294 2a4 108 118 128
              140 150 160 170 180 190 1a0 1b0 1c0 1d0
OURS  iPadHD: 14 4 24 34 44 54 64 2c8 7c b4 c4 294 2a4 dc 118 108 128
              148 158 168 180 190 1a0 1b0 1c0 1d0
ORIG  tail:   5c 7c b0 c0 ec 294 2a4 118 108 128 140 b0 c0 ec 2a0 108
              13c 16c 180 1c0 1b0 1d0 1c0 1b0 1d0 204 214 224 23c 24c
              25c 26c 23c 264 204 214 224 23c 24c 25c 26c
OURS  tail:   5c 7c b4 c4 294 2a4 dc 118 108 128 b0 298 16c 16c 16c
              1b0 1c0 1b0 1d0 1c0 208 218 228 240 250 260 270 208 218
              228 240 250 260 270 240 264
```

The five SLP group-base offsets are **unchanged** by this wave:
`b4/c4/dc/148/208` vs the original's `b0/c0/ec/140/204`.  The win is in
the shared-tail scheduler / register allocation, not in the SLP grouping.

Code/pool split is also unchanged from w9909: original 221 pool words /
6,188 B code, ours 190 / 6,312.

## 5. Negative results (real-linked)

| base | move | raw | aligned |
|---|---|---:|---:|
| w10010 | `101 -> 126` | 5,869 | 1,999 |
| w10010 | `220 -> 203` | 5,847 | 2,002 |
| w10010 | `110 -> 120` | 5,932 | 2,004 |
| w10010 | `217 -> 223` | 5,839 | 2,008 |
| base2 | `77 -> 74` alone | 5,858 | 1,996 |
| base2 | `234 -> 236` alone | 5,818 | 1,998 |
| base2 | `77,74`+`234,236` | 5,857 | 1,993 |
| base2 | `77,74`+`234,236`+`217,223` | 5,837 | **1,986** |
| base2 | `77,74`+`217,223`+`172,182` | 5,840 | 1,999 |
| base2 | `77,74`+`217,223`+`91,93` | 5,827 | 2,005 |
| base2 | `77,74`+`217,223`+`80,77` | 5,817 | 2,079 |

`101 -> 126` and `220 -> 203` are `else if` arms moved across chains, so
they change which `if` they bind to (not pure reorders); they were not
landed.  `234,236` alone is the best raw-preserving single move
(raw 5,818, aligned 1,998); the landed 5-move chain trades +19 raw for
-12 aligned versus that.

## 6. Remaining gap / next lever

The SLP group bases are still one field late (`b4/c4/dc/148/208`).  The
w10008 `field_0xa0`-before-`field_0xb0` shape does move `b0/c0` but costs
size (7,076) and aligned (2,128) on the older base; a third-round sweep
should re-test it on this base together with the `0x140`/`0x144` move and
a size companion.  The device-major rewrite of `0x04..0x1e0` (w9909's open
question) remains the structural lever.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10106
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10106/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5837
python3 /dev/shm/w10106/align_diff.py _ZN6LayoutC1Ev   # 1986
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w10106/`: `split.py`/`gen.py` (statement splitter),
`sweep.py` (57,360-variant compile + mini-link + align sweep),
`align_so.py`, `align_obj.py`, `batch2.py` (real-link confirmation),
`sweep_results.json` (round 2), `stores2.py`, `orig_full.dis`.
