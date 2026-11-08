# `Layout::Layout` C1/C2 — wave 150 / worker w9709
## The w9609 `0x13c..0x148` SLP-group move is **already size-neutral on current
## HEAD** (7,072 B, not 7,080).  Landed it together with the `0x2c0`/`0x1d0`
## swap: size 7,072, raw 5,741 -> 5,828, aligned 2,189 -> 2,276.  Gate green.
## New lead: the `0x2f4..0x318` ascending reorder improves aligned 2,189 ->
## 2,139 but changes the function size by -4 (7,068) / +4 (7,076).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = current HEAD `db9658a51` (w9610's `0x184` chain + reorders):
raw same-address diff **5,741**, aligned **2,189**, size 7,072, `.text`
0x1337c8 (exact), file size exact.  Gate on entry/exit: byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207,
lint CLEAN, `RATCHET PASS`.

## 1. The "+8 B with w9610" claim does not reproduce

The w9609 note (`Layout-LayoutC1-wave150-w9609.md`) says the four-statement
group `[0x13c,0x140,0x144,0x148]` placed *after* `field_0x16c` is +8 B
(7,080) on top of w9610.  On current HEAD `db9658a51` it measures **7,072 B**
exactly, both alone and with the note's `0x2c0`/`0x1d0` swap:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD baseline | **7,072** | **5,741** | **2,189** |
| group after `0x16c` (no swap) | 7,072 | 5,859 | 2,313 |
| group after `0x16c` + `0x2c0`/`0x1d0` swap | **7,072** | **5,828** | **2,276** |
| group after `0x16c` + both swaps (`+0x1bc`/`0x1b8`) | 7,072 | 5,846 | 2,272 |

So the landed source is the w9609 change (group + `0x2c0`/`0x1d0` swap) at the
mandatory exact size.  It is structurally right — the tail now emits
`str.w 0x138` followed by one `vst1.32 {d16-d17}` group at `0x13c`, matching
the original's `0x138` scalar + `0xcfe` group shape — but the raw/aligned
metrics are worse because the group is emitted at function offset `0xf04`
instead of `0xcfe`.

## 2. Exhaustive anchor sweep (new)

Placing the four-statement group after **every** one of the 104 candidate
statements in the tail was rebuilt and measured (`/dev/shm/w9709/sweep_anchor.py`).
**`field_0x16c` is the unique size-neutral anchor**: every other anchor gives
a different function size (6,844..7,084 B; only `0x16c` is 7,072).  The best
aligned among the 7,072 variants is `0x16c` at 2,313.

## 3. Moving only `0x148` (new)

Single-statement `0x148` moves were swept through all 204 positions
(`sweep148.py`).  Every single move keeps the function at 7,072 B.  Best:

| `0x148` position | raw | aligned |
|---|---:|---:|
| after `0x158` | 5,778 | **2,189** (tie) |
| after `0x16c` | 5,808 | 2,202 |
| after `0x13c` | 5,780 | 2,672 |
| before `0x13c` | **5,708** | 2,677 |

None forms the four-float group and none beats baseline on both metrics.  The
`after 0x158` move is size-neutral and ties aligned but does not reach the
structural target.

## 4. New lead: `0x2f4..0x318` ascending reorder (w9608's device group)

The original's `0x2f4..0x318` block is device-major and ascending; our source
is `0x2f8,0x2f4,0x300,0x2fc,0x308,0x304,0x310,0x30c,0x314,0x318`.  Reordering
it ascending (`0x2f4,0x2f8,0x2fc,0x300,0x304,0x308,0x30c,0x310,0x314,0x318`)
is a large alignment win but shifts the function size by 4:

| variant | fn size | raw | aligned | `.text` |
|---|---:|---:|---:|---:|
| `0x2f4` before `0x2f8` only | 7,076 (+4) | 5,753 | **2,163** | 0x1337e8 (+0x20) |
| `0x2f4`/`0x2fc` | 7,076 | 5,753 | **2,140** | +0x20 |
| `0x2f4`/`0x2fc`/`0x304` | 7,068 (-4) | 5,801 | **2,139** | 0x1337c8 (exact) |
| `0x2f4`/`0x2fc`/`0x304`/`0x30c` | 7,076 | 5,798 | 2,141 | +0x20 |
| `0x2f4`/`0x2fc`/`0x304`/`0x314` | 7,068 | 5,801 | **2,139** | exact |

The 7,068 variant keeps `.text`/`.rodata`/file size exact (the 4-byte shortfall
is absorbed by padding) but drops the per-function `size` metric 4524 -> 4523,
so it cannot land under the gate as-is.  A companion **+4 B** source change
(or one extra original instruction in that block) would make it land at
7,072 with aligned ~2,139, i.e. a 50-instruction structural win.  The original
indeed has one more store in that block than we emit (see w9608's 7-vs-0
constant-vector count), so the +4 is very likely a real missing instruction,
not padding.  This is the best next lever.

## 5. Landed source change

Five statements move; no expression changes:

* `field_0x13c`, `field_0x140`, `field_0x144`, `field_0x148` are emitted
  consecutively immediately after `field_0x16c`.
* `field_0x2c0` and `field_0x1d0` swapped.

The tail now emits `str.w 0x138` (scalar) followed by a single
`vst1.32 {d16-d17}` at `0x13c`, matching the original's tail shape.

## 6. Gate

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

Function size is 7,072, `.text` size 0x1337c8 exact, file size exact.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9709
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9709/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5828
python3 /dev/shm/w9709/align_diff.py _ZN6LayoutC1Ev   # 2276
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w9709/`: `units.py` (statement parser), `mk.py`
(statement reorder/swap emitter), `measure.py` (build+fdiff+align),
`sweep_anchor.py` / `sweep148.py` (exhaustive sweeps), `climb.py`
(adjacent-swap hill climb), `search_asc.py` (size-filtered swap search),
`stores.py` (store-offset table), `orig_full.st` / `ours_base.st`.

## 8. Open questions

* The `0x2f4..0x318` ascending reorder is a ~50-instruction alignment win but
  lands at 7,068/7,076.  Find the missing original instruction (or a +4 B
  companion reorder) to hit 7,072.
* The original emits the `0x13c..0x148` group at `0xcfe`; ours emits it at
  `0xf04`.  Closing that 0x206 gap needs the device-major early SLP block
  (`0x140,0x150,...,0x1d0`) reconstructed, not a tail reorder.
