# `Layout::Layout` C1/C2 — wave 136 / worker w8108
## SLP store-group census (full vst1/vstr) + ascending-store-region reorder: 5,783 → 5,754 diff bytes

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-135 master `2e51d765d` (7072 B, target-function differing bytes **5,783**,
`.text` differing bytes 84,960).

## Result — landed: ascending reorder of the `0xb8..0x118` store region + `0x1c0` group

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| target-function differing bytes | 5,783 | **5,754** (−29) |
| `.text` size | `0x1337c8` | **`0x1337c8`** (unchanged) |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.

## 1. Full store census (base-register-tracked, `/dev/shm/w8108/stores.py`)

Original HD-arm `vst1` destinations, instruction order:

```
0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118
0x128 0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
```

Non-HD arm reuses `0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x5c 0x7c 0xb0 0xc0 0xec
0x294 0x2a4 0x118 0x108 0x128 0x140 0x2a0 0x13c 0x16c 0x180 0x1c0 0x1b0 0x1d0
0x204 0x214 0x224 0x23c 0x24c 0x25c 0x26c 0x264`.

`vstr` singles in the original: `0x60 0x68 0x6c 0x70 0x2c8 0x2d0 0x8c 0x90 0x9c
0xa8 0xdc 0xe0 0xe4 0xe8 0x138 0x150 0x158 0x15c 0x160 0x168 0x170 0x1a8 0x1e0
0x3f8` (plus the non-HD arm's `0x138/0x158/0x15c/0x170/0x1e4`). Ours emits a
`vst1` at `0xdc` where the original emits four `vstr` — still unmatched.

## 2. One-group-at-a-time sweep (all real `link_so` builds, symbol-size-checked)

Every original group base was made contiguous ascending at its earliest member.
Only the already-landed groups keep **size 7072**; all others change the symbol size:

| group | size | diff | group | size | diff |
|---|---:|---:|---|---:|---:|
| `0x44/0x54/0x64/0x7c/0xc0/0x160/0x1b0/0x24c/0x264/0x26c/0x294/0x2a4` | 7072 | 5783 | `0x1e4` | 6976 | 5784 |
| `0x4` | 7076 | 5954 | `0x204` | 7040 | 6018 |
| `0x14` | 7048 | 5902 | `0x214` | 7140 | 5893 |
| `0x24` | 7060 | 5983 | `0x224` | 7140 | 5890 |
| `0x34` | 7076 | 5954 | `0x25c` | 7072 | 5785 |
| `0xb0` | 6960 | 5833 | `0x2b4` | 7124 | 6051 |
| `0xec` | 7080 | 5984 | `0xdc` | 7032 | 5868 |
| `0x108` | 7056 | 5860 | `0x150` | 7024 | 5790 |
| `0x118` | 7048 | 5871 | `0x170` | 7080 | 5840 |
| `0x128` | 7064 | 5882 | `0x180` | 7148 | 6054 |
| `0x140` | 7012 | 5926 | `0x190` | 7052 | 5930 |
| `0x1a0` | 7056 | 5899 | `0x1c0` | 7056 | 5901 |
| `0x1d0` | 7072 | 5823 | | | |

No single group beats 5,783 at size 7072 (confirms w7752/w7772).

## 3. NEW hypothesis — whole-region ascending reorder

The original's HD arm stores ascending runs (`0xc0` group then `0xb0` group, then
the `0x140..0x1d0` tail). Sorting a *region* of our member assignments by struct
offset changes which four stores the SLP vectorizer bundles, not just register
allocation. A sweep of `[lo,hi]` ranges (symbol-size checked) found:

| range | symbol size | diff | `.text` |
|---|---:|---:|---|
| `0xb8..0x118` | 7088 | 5661 | `0x1337e8` |
| `0xbc..0x110` | 7052 | 5682 | `0x1337c8` |
| `0xb8..0x110` | 7048 | 5694 | `0x1337c8` |
| `0xb0..0x110` | 7048 | 5715 | `0x1337c8` |

The `0xb8..0x118` sort is the strongest raw-byte lever but grows the function by 16 B.
Pairing it with a −16 B group move restores 7072 B:

```
sort [0xb8..0x118] ascending  +  group 0x1c0 contiguous ascending
  ->  size 7072, diff 5754, .text 0x1337c8  (RATCHET PASS)
```

The `0x1c0` move makes `field_0x1c0/0x1c4/0x1c8` contiguous before the existing
`field_0x1cc` if/else chain, matching the original's `vst1.32 {d2-d3},[r0,#0x1c0]`
group. The landed change is a pure member-store reorder; no expression changed.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8108
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506
```

Scratch harnesses (`/dev/shm/w8108/`, not committed): `stores.py` (base-register
`vst1`/`vstr` census), `score.py` (fixed-window raw diff), `score2.py` (dynsym-size
checked raw diff + `.text` size), `reorder.py` (source-unit parser + group/region
reorder), `sweep.py`/`sweep2.py`/`sweep4.py` (one-group and region sweeps),
`multisort.py` (sequential region sorts), `Layout.head.cpp` (pristine wave-135 body).

## 5. Open questions

- The `0xdc..0xe8` window: original scalar `vstr`, ours `vst1`; preventing the
  vectorizer from merging it is the next structural lever.
- The `0x140..0x1d0` tail still groups at `0x148/0x158/0x168` (ours) vs
  `0x140/0x150/0x160/0x170` (original); every ascending sort of that tail that
  was tried changes the symbol size away from 7072.
