# Layout::Layout C1/C2 — wave 119 / worker w6750

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6750`, base `d2a5ce778` (wave-119 harvest).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — three real source changes, 7072 B preserved

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.594777756 % | **48.624950179 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No baseline update.  File size stays exactly **2,187,328 B**; 27 section headers
unchanged.  `.ARM.extab` / `.rodata` sizes are identical to the pristine wave-119
base (156,308 / 269,351 B) — this change does not touch them.

## The three changes

### 1. `field_0x9c` — float arms (matches the original's `vldr` from a float pair)

```cpp
// before
this->field_0x9c = (int)(float)(Globals::iPadHD ? 70 : (Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50)));
// after
this->field_0x9c = (int)(Globals::iPadHD ? 70.0f : (Globals::n9 ? 75.0f : (Globals::retinaDisplay ? 100.0f : 50.0f)));
```

Original code (`d1962`/`d1972`): the n9 arm is `vldr s0,[pc,#-104]` (75.0f) and
the non-n9 arm is `addw r3,pc,#3812; addeq r3,#4; vldr s0,[r3]` — i.e. a loaded
**float** pair (100.0f / 50.0f) followed by `vcvt.s32.f32` and `vstr s0,[r0,#156]`.
The old int arms forced `mov.w #100; moveq #50; vmov s0,r1; vcvt.f32.s32`.
The float form reproduces the original's `vldr`+`vcvt` shape and adds the
missing scalar `100.0f`/`50.0f` pool words.  Alone this is **+12 B** (7084).

### 2. `t_field_0x2c8` — HD arm is a plain int constant

```cpp
// before
int t_field_0x2c8 = Globals::iPadHD ? 45 * 1.40625f : (...);
// after
int t_field_0x2c8 = Globals::iPadHD ? (int)(45 * 1.40625f) : (...);
```

The original's HD block stores `0x2c8..0x2d4` as one SLP group with the plain
ints `[63,126,63,112]` (pool `0xd1410`), i.e. the HD arm is already an int; the
`vcvt.f32.s32`/`vcvt.s32.f32` round-trip only exists on the non-HD arms
(`d1814`).  Pre-converting the HD arm removes the float materialisation there.
Alone this is **−8 B** (7064).

### 3. `field_0x1a0` before `field_0x1a8`

The iOS-recovered source order (`_work/reconstructions/layout_ctor/ios_source_order.md`)
puts `1a0 1a4 1a8 1ac`; the original's SLP group at `0x1a0` confirms the same
address run.  HEAD had `1a8` before `1a0`.  Swapping the two adjacent statements
moves the `0x1a8` float constant into the following literal island and shifts the
whole pool by 8 B, which aligns several `addw rX,pc,#imm` immediates with the
original.  Alone on the 7076 base this is **−4 B** (7072).

### Combined

`+12 − 8 − 4 = 0` → exactly **7072 B**, ratio **48.624950179 %**.
Measured variants (real `link_so` builds):

| variant | size | ratio % |
|---|---:|---:|
| HEAD | 7072 | 48.594778 |
| 1 (`0x9c` floats) | 7084 | 48.665870 |
| 2 (`0x2c8` int HD arm) | 7064 | 48.614158 |
| 1+2 | 7076 | 48.685259 |
| 1+2+3 (landed) | **7072** | **48.624950** |
| HEAD + 3 only | 7072 | 48.554913 |

## Dead ends measured this wave (all real builds)

| variant | size | ratio % |
|---|---:|---:|
| `0x9c` + `0x150` cast removal | 7068 | 48.674507 |
| `0x9c` + `0x10c` cast removal | 7068 | 48.385811 |
| `0x9c` + `0xa8` temp removal | 7076 | 48.605578 |
| `0x9c` + `0x138` outer-cast removal | 7076 | 48.685259 |
| `0x9c` + `0x2c8` round-trip removal | 7076 | 48.685259 |
| `0x9c` + `volatile` removal | 7076 | 48.615262 |
| `field_0x2d0` cast removal | 7072 | 48.594778 (no codegen change) |
| `field_0x8c` cast addition | 7072 | 48.594778 (no codegen change) |
| `eventBannerScale` int cast | 7072 | 47.987246 |
| iOS statement order (current statements) | 6868 | 35.0 |
| iOS order + `0x9c`/`0x2c8` edits | 6912 | 35.398230 |
| `0x1a8` before `0x1a0` (HEAD) | 7072 | 48.594778 |

A systematic sweep of **all 202 adjacent statement swaps** on the landed
variant found no further 7072-B improvement (best stays 48.624950); the
`0x1a0`/`0x1a8` swap is the unique single-swap win.  Random adjacent-swap
searches (150 builds, two seeds) likewise found nothing better.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6750
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so
```

Scratch harnesses (not committed): `/dev/shm/w6750/{score.py,parse_units2.py,
move_units.py,sweep.py,randswap2.py,editlib.py}` plus the pristine and variant
sources `Layout.base.cpp` / `Layout.7076.cpp` / `Layout.best1.cpp`.
