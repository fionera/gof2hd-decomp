# `Layout::Layout` C1/C2 — wave 144 / worker w8908
## The five `vbsl` groups decoded to exact source statements; making them contiguous saves 212 B (6860), so the shared tail must carry ~212 B of code we do not have. No size-neutral win found (raw diff stays 5,704 @ 7,072 B).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base 0x10000.
Baseline = wave-143 master `f5093474f` (target-function raw same-address diff **5,704**,
`.text` differing bytes 85,851).

## 1. New ground truth: the five `vbsl` groups are five exact contiguous field runs

Decoding the original's `d1ac6..d1c14` block and the pool at `d2920..d29b0` gives the five
4-wide SLP groups as **contiguous ascending field runs**, each a plain
`this->f = Globals::iPadHD ? HD : (Globals::n9 ? N9 : (Globals::retinaDisplay ? R : D));`
ternary chain:

| run | fields | retina / default |
|---|---|---|
| `0xb0..0xbc` | `field_0xb0, field_0xb4, field_0xb8, field_0xbc` | 6,29,36,16 / 3,15,18,8 |
| `0xc0..0xcc` | `field_0xc0, field_0xc4, field_0xc8, field_0xcc` | 13,14,10,360 / 6,8,4,180 |
| `0xec..0xf8` | `ringNearMidThreshold, field_0xf0, ringMidFarThreshold, field_0xf8` | 80,160,400,800 / 40,80,200,400 |
| `0x108..0x114` | `field_0x108, field_0x10c_rowBaseY, field_0x110_strip58, field_0x114_strip5c` | -2,2,1,-1 / 0,1,2,1 |
| `0x2a0..0x2ac` | `field_0x2a0_listRowGap, field_0x2a4_metricA, field_0x2a8_metricB, field_0x2ac_metricC` | 24,220,200,210 / 12,110,100,105 |

(`0x294`/`0x29c` are CSE lane extractions of `0xf4`/`0x2a0`, exactly as wave 143 said.)

The original's **second** `vbsl` block at `d20b4..d215c` is the same shape for
`0x1b0..0x1bc`, `0x1c0..0x1cc`, `0x1d0..0x1dc` **plus `eventBannerScale` (0x1e0)** — all
2× retina/default pairs — and the original stores `eventBannerScale` at `d2128`, i.e. inside
that run, not near `field_0x90` where our source currently puts it.

## 2. Measured: making the runs contiguous lands the `vbsl` but shrinks the function

All variants built with `link_so`, scored with raw same-address diff.

| variant | size | raw diff | `vbsl` count |
|---|---:|---:|---:|
| HEAD (wave 143) | **7072** | **5704** | 0 |
| five `0xb0..0x2ac` runs contiguous ascending | **6860** | 5828 | 5 (1 in the first block) |
| `0x1b0..0x1dc` ascending contiguous, `0x1cc` ternary | **7024** | 5895 | 5 |
| `0x1b0..0x1dc` ascending contiguous, `0x1cc` if/else kept | 7024 | 5895 | 5 |
| `eventBannerScale` moved after `field_0x1e8` | 6924 | 5854 | — |
| `field_0x94`+`field_0x98` moved after `field_0x90` | 7032 | 5888 | — |
| `field_0x94,0x98,0x9c` moved after `field_0x90` | 7008 | 5871 | — |
| `0x94/0x98/0x9c` after `0x90` + `eventBannerScale` late | 6992 | 5896 | — |
| `0x94/0x98` after `0x90` + `0x9c` after `0xac` | 7048 | 5952 | — |
| `0x1d4` moved out of the `0x1b0` run (from the 7024 shape) | 6900 | 5896 | — |

The five-run reorder lands the `vbsl` groups but **removes 212 B** — the mission's "~192 B"
is real. The original therefore has ~212 B more code in the `!iPadHD` shared tail than our
source produces, and the two must be reconstructed together.

## 3. Structural finding: the original's shared tail is the `!iPadHD` path

The original's prologue tests **`iPadHD` first** (`d1036 ldrb.w sl,[r1]`, `d103e beq d126c`),
the HD arm ends `d1268 b.w d21e2`, and the `n9` arm (`d127e..d12fc`) and the
`!n9 && !iPadHD` arm (`d1570..d185e`) both fall into `d1860`.  So `d1860..d20b4` is entered
**only when `iPadHD == 0`** — yet it still contains an `iPadHD` load/`cmp` at `d188e`
(for `field_0x2d4`/`field_0x2d8`), which means the original source is *not* one flat ternary
chain: it has an explicit `if (Globals::iPadHD) { ...HD stores... } else { ...shared tail... }`
split (or the HD stores are hoisted into the first arm), with the HD values emitted in the
`d1042..d1268` arm and the shared tail emitting only `n9 ? ... : (retina ? ... : ...)`.

That is the most likely home of the missing 212 B: our source's inline
`iPadHD ? HD : (...)` ternaries in the shared tail are compiled as `cmp iPadHD / it eq /
moveq` chains, while the original has already branched on `iPadHD` and emits only the
`n9`/`retina`/default selects there.

## 4. Witnessed statement-order fixes (all shrink alone, none size-neutral)

* `field_0x94` and `field_0x98` are **adjacent in the original** at `d196c`/`d199a`, right
  after `field_0x90` (`d1958`/`d195c`).  Our source has `field_0x98` at line 45 (HD-arm
  region) and `field_0x94` at line 282 (tail end) — both wrong.  Moving them after
  `field_0x90` is faithful but costs 40 B.
* `eventBannerScale` (0x1e0) belongs with the `0x1b0..0x1dc` run (original `d2128`), not
  after `field_0x90`.  Moving it there costs 148 B.
* The original's `0x1b0..0x1dc` run is **ascending** (`0x1b0,0x1b4,0x1b8,0x1bc,0x1c0,
  0x1c4,0x1c8,0x1cc,0x1d0,0x1d4,0x1d8,0x1dc`), with `0x1cc` a plain ternary
  (`36*1.40625f / 54 / 72 / 36`).  Our source is `0x1b0,0x1bc,0x1b8,0x1b4,...` with an
  if/else chain for `0x1cc`.

## 5. Not landed

No source change: every faithful reorder either shrinks the function (breaking
`size_exact`) or raises the raw diff.  The wave-143 statement order remains the best
size-neutral local optimum at **7072 B / 5,704**.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8908
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8908/fdiff.py _ZN6LayoutC1Ev   # 7072/7072, diff 5704
```

Scratch (`/dev/shm/w8908/`): `fdiff.py`, `bm.sh` (build+score), `parse.py`, `stores.py`,
`brs.py`, `align.py`, `Layout.base.cpp`, `Layout.vbsl1.cpp` (five-run + `0x1b0` reorder),
`Layout.7024.cpp`.

## 7. Open questions

* Reconstruct the `if (Globals::iPadHD) { HD stores } else { shared tail }` split so the
  shared tail stops re-testing `iPadHD`; that should recover the ~212 B and let the five
  `vbsl` runs land at 7,072 B.
* The `clz` mask (`!retinaDisplay` held in `r8`) vs our `vceq` idiom is the same problem:
  the original hoists `retinaDisplay` once per arm; our inline ternaries reload it.
