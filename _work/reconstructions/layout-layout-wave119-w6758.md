# Layout::Layout C1/C2 — wave 119 / worker w6758
## Landed: `field_0x9c` float arms + `field_0x2c8` cast drop + `0x1a8`/`0x1a0` order (48.5948 -> 48.6250 %, 7072 B)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6758`, base `4efc90c64` (wave-118 harvest + createMission note).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

Prior notes read and built on:
`_work/reconstructions/layout-literal-pool-got-wave119-w6720.md`,
`layout-layout-wave118-w6602.md`, `layout-layout-c1-c2-source-shape-w3905.md`.

## Result

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 48.59477775563085 % | **48.62495017935433 %** |
| normalized insns (orig / ours) | 2529 / 2488 | 2529 / **2489** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` -> `RATCHET PASS`.

### Section / structural state (measured before and after this change)

| section | original | HEAD | after |
|---|---:|---:|---:|
| section headers | 27 | 27 | **27** |
| file size | 2,187,328 | 2,187,328 | **2,187,328** |
| `.text` | 0x1337c8 | 0x1337c8 | **0x1337c8** (3964 bytes differ, all inside `Layout::Layout`) |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | **0x6f70** (byte-identical) |
| `.ARM.extab` | 0x26298 | 0x2628c | **0x2628c** (byte-identical) |
| `.rodata` | 0x41b67 | 0x41c27 | **0x41c27** (byte-identical) |
| `.data` | 0x296c | 0x296c | **0x296c** (byte-identical) |

Note: the task brief quotes `.ARM.extab` as `0x26290` (8 B short).  At the
actual branch base `4efc90c64` it is **`0x2628c`** (12 B short); the
`Generator::createMission` commit (`LSDA 168 B`) moved it.  This change does
**not** touch `.ARM.extab` (0 differing bytes), `.rodata`, `.data` or
`.ARM.exidx`; only `.text` changes.

## The three source edits

```cpp
// 1. field_0x9c: integer arms -> float arms
- this->field_0x9c = (int)(float)(Globals::iPadHD ? 70 : (Globals::n9 ? 75 :
-                    (Globals::retinaDisplay ? 100 : 50)));
+ this->field_0x9c = (int)(Globals::iPadHD ? 70.0f : (Globals::n9 ? 75.0f :
+                    (Globals::retinaDisplay ? 100.0f : 50.0f)));

// 2. field_0x2c8: t_field_0x2c8 is already int; the (int)(float) round trip is an identity
- this->field_0x2c8 = (int)(float)t_field_0x2c8;
+ this->field_0x2c8 = t_field_0x2c8;

// 3. swap two independent assignments (0x1a8 before 0x1a0)
- this->field_0x1a8 = ...;
  this->field_0x1a0 = ...;
+ this->field_0x1a8 = ...;
```

All three are value-preserving for every platform-flag configuration (the
`0x9c` arms are `70/75/100/50` in both spellings; `t_field_0x2c8` is a small
int so `(int)(float)x == x`; `0x1a0`/`0x1a8` are distinct fields).

Edit 1 is the construct the wave-119 w6720 note identified as the source of
the original's extra `100.0f` / `50.0f` literal-pool words (its §3b).  It is
worth **+12 B** on its own (7072 -> 7084) and **+0.071 pp** of `difflib`
(48.5948 -> 48.6659).  Edit 2 is worth **-8 B** (7072 -> 7064) and +0.019 pp.
The pair is **7076 B** at **48.6853 %**; the `0x1a8`/`0x1a0` swap is the only
-4 B edit found that brings it back to the mandatory 7072 B.

## Measured search (all numbers are real `link_so` builds, unrounded ratio)

### Single edits on the pristine source

| edit | size | score % |
|---|---:|---:|
| HEAD | **7072** | **48.594778** |
| A = `0x9c` float arms | 7084 | 48.665870 |
| B = `0x138 = v138` (drop double round trip) | 7048 | 48.573139 |
| B2 = `0x138 = (int)(float)v138` | 7064 | 48.574277 |
| C = `0x2c8` cast drop | 7064 | 48.614158 |
| D = `0x150` cast drop | 7060 | 48.623853 |
| E = `0x10c` drop `(int)(float)` | 7060 | 48.295115 |
| F = `0x160` drop `(int)(float)` | 7064 | 48.534397 |
| G = `0xa8` cast drop | 7064 | 48.414756 |
| H = `0x2d0` cast drop | 7072 | 48.594778 (no code change) |
| J = `0x150` float arms (keep cast) | 7072 | 48.245614 |
| K = `0x2d0` float arms (keep cast) | 7088 | 48.216776 |

### Combinations

| variant | size | score % | landable |
|---|---:|---:|---|
| A + C | 7076 | 48.685259 | no (7076 collapses byte_exact) |
| A + B2 | 7076 | 48.685259 | no |
| A + D | 7068 | 48.674507 | no |
| A + B | 7060 | **48.724083** | no (best raw score seen) |
| A + B2 + D | 7060 | 48.733799 | no |
| **A + C + swap(`0x1a8`,`0x1a0`)** | **7072** | **48.624950** | **yes** |
| A + B2 + swap(`0x1a8`,`0x1a0`) | 7072 | 48.624950 | yes (same code) |

### Exhaustive sweeps run this wave

* All 177 adjacent-statement swaps on top of **A + C**: exactly one 7072-B
  hit, `0x1a8`/`0x1a0` at 48.624950 %.  All other 7072 hits are <= 48.5356 %.
* All 177 adjacent-statement swaps on top of **A** (7084, needs -12): one
  7072-B hit (swap at `0x9c`, 48.425668 %).
* All 177 adjacent-statement swaps on top of **A + B** (7060, needs +12):
  **zero** 7072-B hits.
* All 177 adjacent-statement swaps on top of the champion: no second swap
  improves on 48.624950 %.
* Moving `0x1a8` to every position 178..199 on top of A + C: only position
  188 (the champion) yields 7072 B.
* Single `(int)(float)` / `(int)` / `(float)` wraps, float-literal-arm
  conversions, `||` -> `|` / `+` rewrites, `volatile` removal, and
  `0x9c` arm-spelling variants: none produced a 7072-B variant above
  48.594778 % except the champion above.

## Why the remaining gap is hard (recap of w6720, re-verified)

The original's end-pool census is 221 words vs our 188 (+33): +32 words from
eight extra 4-int SLP groups, +2 from an extra `100.0f`/`50.0f` pair, -1 GOT
offset; plus 52 B more cold outlined `else` code (four tails vs one).  The
original's SLP destinations are `0xb0`, `0xec`, `0x13c`, `0x140`, `0x16c`,
`0x180`, `0x2a0` runs plus a `0x204` window; ours are shifted one field
(`0xb4`, `0x148`, `0x208`, ...).  Reordering source statements into the
original's ascending-offset order does restore the SLP groups but shrinks the
body far below 7072 B (e.g. `0xec..0xf8` adjacent -> 6920 B / 38.3 %;
`0x140`/`0x144` moved into the `0x140` run -> 6816 B / 32.4 %), and the
7072-B wall is hard: a 7076-B ctor collapses whole-`.so` `byte_exact`
4505 -> 2454.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6758
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > ours.dis
```

Scratch harnesses (not committed): `/dev/shm/w6758/{score.py,edits.py,
reord2.py,floatify.py,slpdst.py,sweep_*.py}` plus the `sweep_*.json`
measurement dumps.
