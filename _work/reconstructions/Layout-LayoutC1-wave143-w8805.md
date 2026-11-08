# `Layout::Layout` C1/C2 — wave 143 / worker w8805
## The `vbsl` select is plain contiguous-ternary SLP; grouping the fields saves ~192 B but the original's compensating code lives in the shared tail. No size-neutral win found (raw diff stays 5,704 @ 7,072 B).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base 0x10000.
Baseline = wave-142 master `bf083ab41` (target-function raw same-address diff **5,704**,
`.text` differing bytes 85,119).

## 1. Ground truth: the block is a run of contiguous `retina ? A : B` ternaries

The mission's `0xd1ac6..0xd1c14` region is the **retina/default select** (the field list in
the brief, `0x1b0..0x1e0`, is a typo: the actual fields are `0xb0..0x114` plus `0x294/0x298/
0x29c/0x2a0..0x2ac`).  Decoding the literal pools (`d2920..d29b0`) and the seven `vbsl`
gives five exact 4-wide SLP groups:

| fields | retina | default |
|---|---|---|
| `0xb0..0xbc` | `6,29,36,16` | `3,15,18,8` |
| `0xc0..0xcc` | `13,14,10,360` | `6,8,4,180` |
| `0xec..0xf8` | `80,160,400,800` | `40,80,200,400` |
| `0x108..0x114` | `-2,2,1,-1` | `0,1,2,1` |
| `0x2a0..0x2ac` | `24,220,200,210` | `12,110,100,105` |

`0x294` and `0x29c` are **lane extractions** from the `0xec` and `0x2a0` groups (CSE: same
value as `0xf4` / `0x2a0`).  The mask is the `clz(r8) >> 5` idiom (`!retinaDisplay`) built
once at `d1ae2` and reused for all seven `vbsl`.  This is exactly clang's SLP output for a
contiguous run of `this->f = Globals::retinaDisplay ? A : B;` assignments — no min/max, no
struct, no manual table.

## 2. Why it does not land: size neutrality

Every variant that makes the fields contiguous SLP-vectorises them and **shrinks** the
function below 7,072 B, which breaks `size_exact` (4,524/4,524).  Measured (real `link_so`):

| variant | size | raw diff |
|---|---:|---:|
| HEAD (wave-142 baseline) | **7072** | **5704** |
| 0x1d0 group contiguous + `0x1cc` ternary (flat) | 7024 | 5895 |
| 0x1d0 group contiguous, `0x1cc` if/else kept | 7088 | 5825 |
| 0x1d0 group contiguous, `0x2c0` before group | 7080 | 5803 |
| 0xb0..0xcc grouped ascending (flat) | 7024 | 5867 |
| nested device-major group (`w8709` shape) | 7064 | 5875 |
| nested group + `eventBannerScale` in group | 7000 | 5901 |
| cache `retina` local in 0x1b0..0x1e0 region only | 7048 | 5906 |
| cache `retina` local for the whole ctor | **6880** | 5871 |
| `Globals::retinaDisplay` retyped `bool` | 7064 | 5820 |
| drop the `volatile` retina reload | 7064 | 5843 |
| swap 0xb8/0xb4 and 0xc4/0xc0 (lane order) | 7064 | 5793 |
| reorder 0x1bc/0x1b8/0x1b4 to ascending | 7072 | 5704 (identical code) |

The whole-ctor `retina` local is the strongest evidence: it vectorises aggressively and
drops the function to **6,880 B** (−192 B).  The original is 7,072 B *with* that
vectorisation, so it must carry ~192 B more code than us elsewhere — the device-major
shared tail (`d1860..d20b4` = 2,132 B in the original vs `d1de4..d219c` = 952 B in ours).
The original's retina arm is 750 B (`d1570..d185e`); ours is 1,412 B (`d1568..d1aec`) plus a
758 B default sub-branch.  Reconstructing that split is the prerequisite for landing the
`vbsl` groups at 7,072 B.

## 3. Not landed

No source change: every measured variant either changed the size or raised the raw diff.
The wave-142 statement order is a strong local optimum; single moves among the 34
"interesting" (`0xb0..0x114`, `0x1b0..0x1e0`, `0x294..0x2ac`) units all cost +100..+240
raw bytes because they break an existing SLP group.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8805
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8805/fdiff.py _ZN6LayoutC1Ev   # 7072/7072, diff 5704
```

Scratch (`/dev/shm/w8805/`): `fdiff.py`, `region.py`, `align.py`, `climb.py`,
`variantA..E.py`, `Layout.base.cpp`.

## 5. Open questions

* Reconstruct the device-major shared tail (`d1860..d20b4`) so the ~192 B saved by the five
  `vbsl` groups is compensated and the function stays 7,072 B.
* The original's `clz` mask (vs our `vceq`) comes from a *stable* `retinaDisplay` value held
  in one register across the whole select; the `volatile` reload at `Layout.cpp:51` forces
  our reload and the `vceq` idiom.
* `0x294`/`0x29c` are CSE lane extractions — a faithful source should keep them adjacent to
  their value twins (`0xf4`, `0x2a0`), not as independent scalars.
