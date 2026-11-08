# `Layout::Layout` C1/C2 — wave 163 / worker w10906

## n9 fast-path explicit chain sweep.  Landed: first two fields as an
## explicit `if (iPadHD) / else if (n9) / else` chain.  Raw **5,705 ->
## 5,695** (-10), aligned **2,082 -> 2,083** (+1), size **7,072 B exact**.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-162 HEAD `e51db2fc7`: raw **5,705**, aligned **2,082**,
7,072 B.  Exit: raw **5,695**, aligned **2,083**, 7,072 B.
Gate: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.
`.text` diff **85,042 -> 85,032** (-10); `.rel.plt` byte-identical (0);
`.ARM.extab` 725; all section sizes exact.

## 1. Block map (original vs baseline vs landed)

Original (`0xd1028`, code 6,186 B + final pool 884 B):

```
d1042..d126a  iPadHD block   552 B  -> b d21e2 (tail)
d126c..d127e  n9 check        18 B
d1280..d12fe  n9 fast path   126 B  -> b d1860 (common)
d1300..d156e  inline pool    622 B
d1570..d1860  else block     752 B  -> falls into common
d1860..d215e  common       2,302 B  -> b d21e2
d215e..d21e2  cold subblocks 132 B
d21e2..d2802  tail         1,568 B  (pop at d2800)
d2802..d2854  out-of-line     82 B
d2854..d2bc8  final pool     884 B
```

Baseline (`0xd1028`, code 6,310 B + final pool 760 B):

```
d1042..d1278  iPadHD block   566 B  -> b d22a6
d127a..d128c  n9 check        18 B
d128c..d1310  n9 fast path   132 B  -> b d186e
d1310..d1568  inline pool    600 B
d1568..d186e  else block     774 B  -> falls into common
d186e..d225a  common       2,540 B  -> b d1272 (shared epilogue)
d225c..d22a6  cold subblock   74 B
d22a6..d28be  tail         1,560 B  (pop at d28bc)
d28be..d28d0  out-of-line     18 B
d28d0..d2bc8  final pool     760 B
```

The 128-B residual is **not** the n9 block (132 vs 126 B): it is the
common block (+238 B) partly offset by the cold subblocks (-58),
out-of-line checks (-62) and the n9 block (+6).  The n9 fast path is
already a 132-B block that jumps into a shared common block; the
w10806 note's "730 B n9 block" was measured on a different variant.

## 2. Explicit-chain sweep (all real link+fdiff, size checked first)

Generated `if (iPadHD) {..} else if (n9) {..} else {..}` around the
first N constructor statements, rest unchanged:

| N | size | raw | aligned |
|---:|---:|---:|---:|
| 1 | 7,072 | 5,820 | 2,090 |
| **2** | **7,072** | **5,695** | **2,083** |
| 3 | 7,072 | 5,705 | 2,082 |
| 8 | 7,072 | 6,011 | 2,272 |
| 24 | 7,072 | 5,952 | 2,510 |
| 27 | 7,072 | 5,905 | 2,869 |
| 30 (first 8 groups) | 7,016 | — | — |
| whole body (variant A) | 6,768 | — | — |

Only N=2 is a raw win.  Larger chains (including the full first-8-groups
chain the mission asked for) are size-inexact (7,016 B) or much worse
on raw.  Variant A (whole body in the chain, HD branch explicit) gives
code 6,100 B + pool 668 B — code is 86 B short of the original's 6,186
but the pool is 216 B short, so the code/pool split still does not match.

## 3. Landed change

```cpp
if (Globals::iPadHD) {
    this->field_0x4 = 15 * 1.40625f;
    this->footerTextInset = 22 * 1.40625f;
} else if (Globals::n9) {
    this->field_0x4 = 22;
    this->footerTextInset = 37;
} else {
    this->field_0x4 = Globals::retinaDisplay ? 30 : 15;
    this->footerTextInset = Globals::retinaDisplay ? 50 : 22;
}
```

Pure source restructuring, no constant/type change.  Size stays 7,072 B.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10906
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10906/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5695
python3 /dev/shm/w10906/align_diff.py _ZN6LayoutC1Ev   # 2083
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10906/`: `cc.sh` (0.13 s single-TU compile),
`size.sh`, `link.sh` (extracted gold link), `trial.sh` (compile+link+
fdiff in 0.8 s), `gen.py`/`genN.py`/`sweepB.py`/`sweepC.py` (chain
generators/sweeps), `variantA_full.cpp`, `vn2.cpp`, `vclean.cpp`.

## 5. Open questions

* The original's final pool is 884 B vs our 760 B.  It has extra
  constants (0.325f/0.65f at the head, 60/120/300/600, a repeated 11)
  that our common block emits as `movs`/`movw` instead of `vld1`
  groups.  Getting the common block to group 4 consecutive fields per
  `vld1` (as the original does) is the remaining 128-B lever.
* The original's n9 block sets `0x2c0..0x2cc` as one `stmia`; our
  source has `0x2c0` far away.  Moving it next to `0x2c4` gives
  7,044/7,080 B (not size-exact) and worse raw — the surrounding
  `0x98`/`0x2d0` placement must move with it.
