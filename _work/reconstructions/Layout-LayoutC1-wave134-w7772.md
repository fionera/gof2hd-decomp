# `Layout::Layout` C1/C2 — wave 134 / worker w7772
## SLP store-group census continued: the `0x7c` group is a real (small) `.text` win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-133 master `bbffef482` (7072 B, target-function differing bytes **5,784**,
`.text` differing bytes **85,022**).

## Result — landed: `0x7c` group reorder (7072 B, diff bytes 5,784 → 5,783)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 45.0 % | **44.8 %** |
| target-function differing bytes | 5,784 | **5,783** (−1) |
| `.text` differing bytes | 85,022 | **85,021** (−1) |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
`.rodata`, `.data`, `.ARM.exidx` remain byte-identical; `.ARM.extab` unchanged at 1,416
differing bytes (size 156,312 exact).

## 1. Corrected full single-group sweep (every original `vst1` group, real `link_so` builds)

The w7752 census is confirmed: the original's `vst1` destinations are (HD arm, instruction
order)

```
0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c 0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118 0x128
0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
```

Each group's four member assignments were made contiguous ascending at the position of the
earliest member (base already contains the w7752/w7751 landed moves `0x26c,0x2a4,0x24c,0x264,
0x16c,0x170`). Ours size / target diff bytes:

| group | size | diff | group | size | diff |
|---|---:|---:|---|---:|---:|
| **HEAD** | **7072** | **5,784** | `0x140` | 7012 | 5,926 |
| `0x4` | 7076 | 5,950 | `0x150` | 7024 | 5,791 |
| `0x14` | 7048 | 5,902 | `0x158` | 7076 | 5,927 |
| `0x24` | 7060 | 5,983 | `0x15c` | 7036 | 5,914 |
| `0x34` | 7076 | 5,950 | `0x160` | 7072 | 5,784 |
| `0x44` | 7072 | 5,784 | `0x168` | 7072 | 5,784 |
| `0x54` | 7072 | 5,784 | `0x16c` | 7072 | 5,784 |
| `0x5c` | 7072 | 5,784 | `0x170` | 7080 | 5,833 |
| `0x64` | 7072 | 5,784 | `0x180` | 7148 | 5,978 |
| **`0x7c`** | **7072** | **5,783** | `0x1a8` | 7072 | 5,784 |
| `0x8c` | 7072 | 5,831 | `0x1b0` | 7072 | 5,784 |
| `0x90` | 7072 | 5,831 | `0x1d0` | 7072 | 5,824 |
| `0xb0` | 6960 | 5,834 | `0x1e0` | 6744 | 6,011 |
| `0xc0` | 7072 | 5,784 | `0x1e4` | 6976 | 5,785 |
| `0xdc` | 7032 | 5,869 | `0x224` | 7140 | 5,822 |
| `0xe0` | 7040 | 5,911 | `0x24c` | 7072 | 5,784 |
| `0xe4` | 7056 | 5,933 | `0x264` | 7072 | 5,784 |
| `0xe8` | 7000 | 5,802 | `0x26c` | 7072 | 5,784 |
| `0xec` | 7080 | 5,976 | `0x294` | 7072 | 5,784 |
| `0x108` | 7056 | 5,861 | `0x2a0` | 7064 | 5,862 |
| `0x118` | 7048 | 5,872 | `0x2a4` | 7072 | 5,784 |
| `0x128` | 7088 | 6,232 | `0x138` | 7000 | 5,889 |

`0x7c` is the only single move that keeps 7072 B **and** lowers the diff-byte count.
`0x9c/0xa8/0x1c0/0x204/0x214/0x25c/0x2c8/0x2d0` have a multi-line `if/else` member and
were not movable as a single-line block.

## 2. Landed change

`field_0x84`/`field_0x80_touchMargin` are moved after `field_0x7c`, so the four stores are
source-adjacent ascending:

```c++
this->field_0x7c = ...;
this->field_0x80_touchMargin = ...;
this->field_0x84 = ...;
this->field_0x88 = ...;
```

This exactly matches the original's non-HD-arm source order
(`*(this+0x7c)`, `*(this+0x80)`, `*(this+0x84)`, `*(this+0x88)` in the Ghidra decompile,
`layout-orig-ghidra-decompile.txt` lines 470-478). The body stays 7072 B and the raw
same-address differing bytes fall by one. The fuzzy ratio dips 0.2 pt because the literal
pool shifts by 4 B; this is the same trade-off the w7752 `0xc0`/`0x294` reorders made.

## 3. Recorded lever: a 6-group combination at 6864 B (not landed — size regression)

Greedy forward selection over the sweep finds a much larger raw-byte win if a smaller body
is allowed:

```
{0x7c, 0x1e4, 0x150, 0xe8, 0x1d0, 0x170}  ->  6864 B, 5,601 diff bytes  (−183)
```

Adding any positive-delta group to restore 7072 B costs more than it saves
(`+0x180` → 6928/5,806; `+0x224` → 6904/5,690; `+0x180+0x224` → 6976/5,781;
`+0x180+0x224+0x128+0xec` → 7088/6,231). A 100-build random search over 2–4 group subsets
filtered to `ours size == 7072` found nothing below 5,783. If the project ever relaxes
`size_exact` for this function, the 6-group set is the next structural target.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7772
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7772/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7772/ours.dis
python3 /dev/shm/w7772/score.py      # 7072 / 5783
python3 /dev/shm/w7772/textdiff.py   # .text 85,021
```

Scratch harnesses (not committed, `/dev/shm/w7772/`): `score.py` (exact single-function
size + same-address raw-byte diff), `stores.py` (base-register-tracked `vst1`/`vstr`
census), `groupmove.py`/`combo.py`/`sweep.py` (group reorder + real build harness),
`reorder2.py` (arbitrary assignment reorder), `randsearch.py` (subset search),
`textdiff.py` (section diff bytes), `Layout.base.cpp` (pristine wave-134 body).
