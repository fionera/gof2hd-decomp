# Layout::Layout C1/C2 — wave 108 (w5605): 48.0 % is a hard local
# optimum; the 7064-B 48.0287 % near-miss and the shared-int re-tests

Worker **w5605**, branch `wave108-w5605`, base `8f98ec4d6`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

**No landable improvement found.** The ctor stays at **7072 B /
48.000000000000 %** (unrounded linked `difflib.SequenceMatcher`,
`autojunk=False`, same normalisation as `tools/verify/verify.py`), with
**2529/2496** normalised instruction entries. `src/game/ui/Layout.cpp` is
byte-identical to the base. Gate before == after:
`byte_exact` **4500/4524**, `linked_exact` **4500/4524**, `size_exact`
4524/4524, `data_exact` 843/843, imports 207, lint `CLEAN`, drift 0,
`RATCHET PASS`. No ELF section or `.ARM.extab` change is introduced.

This pass contributes four measured facts:

1. The wave-107 landed body is a **genuine local optimum** of the fuzzy
   metric at 7072 B: every one of the ~2 500 single edits and ~1 400
   reorder moves tried here leaves it at or below 48.0 %.
2. The best fuzzy variant measured for this function is the adjacent swap
   `field_0x180`↔`field_0x188` at **7064 B / 48.028673835125 %** — only
   +0.0287 pp and **8 B short** of 7072, so unlandable (size quantization).
3. The shared-int round trips (`0xa8/0xac`, `0x5c/0x70`, `0x3f4/0x3f8`)
   reproduce the original's `vmov/vcvt.f32.s32/vcvt.s32.f32/vstr`
   fingerprint in isolation but **collapse the full ctor to 16–47 %**
   because clang hoists the shared computation and re-shuffles the HD
   block; the flat body is still the better source shape for the metric.
4. The original's literal pool has four float constants ours lacks
   (`28.125f`, `14.0625f`, `5.625f`, `75.0f`); the corresponding
   float-arm rewrites are code-identical or shrink the function, so they
   cannot be used to close the 7072-B gap.

## 1. Baseline and harness

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5605/score1u.py _ZN6LayoutC1Ev        # 7072 B, 48.000000000000
```

The harness (`/dev/shm/w5605/try.sh`) restores the pristine
`Layout.cpp`, swaps the ctor body, runs `link_so` and prints the unrounded
score. A full build+link+score is **0.85 s**, so ~4 000 variants were
measured. `score1u.py` is the unrounded wrapper over
`tools/verify/verify.py` (`Elf32`, `Resolver`, `disassemble`,
`reconcile`) used by waves 106/107.

## 2. Single-edit scans (all real `link_so` builds)

### 2.1 Missing/extra `vstr` fields

The original's `vstr` set is `0x60 0x68 0x6c 0x70 0x8c 0x90 0x9c 0xa8
0xdc 0xe0 0xe4 0xe8 0x138 0x150 0x158 0x15c 0x160 0x168 0x170 0x1a8
0x1e0 0x1e4 0x2c8 0x2d0 0x2f8 0x3e0 0x3f8` (27). The landed body has 28:
it misses `0x9c 0x138 0x158 0x15c 0x170 0x1e4 0x3f8` and adds
`0x20 0x10c 0x148 0x194 0x1a4 0x1f4 0x204 0x3e8`.

| edit (single field) | size | % |
|---|---:|---:|
| baseline | **7072** | **48.0000** |
| `0x9c` if/else → float ternary (no cast) | 7064 | 47.8088 |
| `0x9c` `(int)(float)(ternary)` | 7064 | 47.8088 |
| `0x138` all-float arms | 7056 | 47.7291 |
| `0x138` `(int)(float)(...)` | 7072 | 48.0000 (code-identical) |
| `0x138` `int v; ...; (int)(float)v` | 7072 | 47.9697 |
| `0x158` / `0x15c` / `0x170` all-float arms | 7072 | 48.0000 (code-identical) |
| `0x1e4` all-float arms | 7064 | 40.1354 |
| `0x1e4` `(int)(float)(...)` | 7048 | 41.7298 |
| `0x10c` HD arm `1*1.40625f`→`1` | 7056 | 42.2434 |
| `0x124` HD arm →`1` | 7080 | 41.5357 |
| `0x128`/`0x1bc`/`0x230`/`0x234`/`0x250` HD arm →`1` | 7072 | 48.0000 (code-identical) |
| `0x3e0` (scrollBarInset) HD arm →`1` | 7048 | 46.4115 |
| `0x20` HD arm → int | 7056 | 46.8221 |
| `0x148` HD arm → int | 7056 | 47.7689 |
| `0x1f4` HD arm → int | 7056 | 47.8088 |
| `0x204` HD arm → int | 7056 | 46.7424 |
| `0x3e8` HD arm → int | 7048 | 41.9606 |

### 2.2 Exhaustive expression scans

* **All 94 `N * 1.40625f` occurrences**, each replaced by `N.0f *
  1.40625f` (float literal) and by `N` (int): 188 builds. No 7072-B
  variant above 48.0 %. The only size-increasing single edits are
  `7080 B / 47.9109 %` (value `12` → int) and `7080 B / 47.2554 %`
  (value `173` → int).
* **All ~200 single-line assignments** wrapped in `(int)(float)(...)`:
  no 7072-B hit above 48.0 %.
* **All ~200 single-line assignments** with every bare int arm
  floatified (`N` → `N.0f`): no 7072-B hit above 48.0 %.
* **All 14 `if/else` blocks** converted to a single ternary: the four
  convertible blocks measure 7064/47.59, 7064/47.81, 7064/47.99 and
  7072/48.00; the rest are code-identical or parse-identical.
* **Global floatify** of every `N * 1.40625f` → `N.0f * 1.40625f`:
  7072 B / 48.0000 % (code-identical — clang folds the literal type).

## 3. Reorder searches (all real `link_so` builds)

| variant | size | % |
|---|---:|---:|
| baseline order | **7072** | **48.0000** |
| adjacent swap `0x180`↔`0x188` (best overall) | 7064 | **48.0287** |
| adjacent swap `0x180`↔`0x188` + `12*1.40625f`→`12` | 7072 | 47.8105 |
| adjacent swap `0x180`↔`0x188` + `7*1.40625f`→`7` | 7064 | 47.9888 |
| adjacent swap `0x180`↔`0x188` + `80*1.40625f`→`80` | 7064 | 47.9888 |
| adjacent swap `0x180`↔`0x188` + `15*1.40625f`→`15` | 7064 | 47.9888 |
| iOS ascending source order (all units) | 6912 | 36.2243 |
| HD block sorted ascending, in place | 6996 | 20.6745 |
| HD + mid blocks sorted ascending, in place | 7076 | 19.4532 |
| HD units in the original's exact store order | 6788 | 33.8331 |
| float fields `0xdc,0xe0,0xe4,0xe8` grouped | 7064 | 43.1990 |
| `rewardBoxY2` moved directly after `rewardBoxY` | 7064 | 47.8391 |

Searches performed:

* **All 202 adjacent swaps** of the 203-unit body: no 7072-B variant
  above 48.0 %; the only score gain is the `0x180`↔`0x188` swap at
  7064 B / 48.0287 %.
* **All 202 adjacent swaps of the `0x180`↔`0x188` body**: no 7072-B
  variant above 48.0287 %.
* **400 random unit moves / pair swaps** (seed 1) and **800 random
  moves / swaps** (seed 7, ±3 positions): no 7072-B improvement.
* **7-edit subset search** over `{9c-floattern, 138-floatarms,
  1e4-floatarms, 10c-intHD, 124-intHD, 3e0-intHD, 138-intvar}`
  (127 builds): no 7072-B variant above 48.0 %; the only 7072-B
  combinations are `138-intvar` (47.9697) and `9c+138-intvar` (47.6608).

## 4. Block-tree / shared-int re-tests (new measurements)

The original is a block tree: `if (iPadHD) { 137 straight-line fields }
else { n9/retina 34 + mid 103 }`, then 66 tail fields. Rebuilding it from
`lay_orig.json` (the 32-flag-combination store table) with the checked-in
`_work/reconstructions/layout_ctor/gen.py` and the current field order:

| body | size | % |
|---|---:|---:|
| `gen.py` `hdblock` (straight-line HD, chain else) | 6472 | 35.5783 |
| `gen.py` `hdblocksorted` | 6472 | 32.0654 |
| `gen.py` `chain` (flat shortest chains) | 6568 | 8.1732 |
| hybrid: straight-line HD (current order) + flat else + tail | 6848 | 26.0130 |
| hybrid: straight-line HD (original store order) + flat else + tail | 7856 | 36.5706 |
| hybrid: straight-line HD (ascending) + flat else + tail | 7848 | 39.6892 |

The hybrid is **784 B too large** (the straight-line HD block is an extra
copy on top of the flat body), so the flat single-copy body is the only
size-exact shape at 7072 B.

Shared-int round trips (`int v = <ternary>; this->X = v; this->Y =
(int)(float)v;`) reproduce the original's two-`vcvt` fingerprint in
isolation, but in the full ctor clang hoists the computation and
re-shuffles the HD block:

| shared pair | size | % |
|---|---:|---:|
| `0x5c`/`0x70` (int `v5c`) | 7076 | 16.9261 |
| `0xa8`/`0xac` (int `vac`) | 7116 | 18.2705 |
| `0xa8`/`0xac` (float `fac`) | 7016 | 38.9111 |
| `0x3f4`/`0x3f8` (int `ry`) | 7080 | 46.7662 |

This confirms the wave-106/107 conclusion: the shared-int form is the
true original spelling but not the metric-optimal source shape.

## 5. Literal-pool delta (new)

The verifier's reconciled pool entries number **38 (original) / 39
(ours)**. The original has four float words ours lacks:

```
0x41e10000 = 28.125f    (20 * 1.40625f)
0x41610000 = 14.0625f   (10 * 1.40625f)
0x40b40000 = 5.625f     ( 4 * 1.40625f)
0x42960000 = 75.0f      (field_0x9c n9 arm)
```

Ours instead pools `0x42000000` (32.0f) and `0x430ca000` (140.625f).
Making `0x1d8`, `0x1dc`, `0x1e4` and `0x9c` float-typed adds those words
but shrinks the function (7048–7064 B) and lowers the score, so the pool
delta cannot be closed at 7072 B.

## 6. Proven vs hypothesis

**Proven (binary / measured):**

* The wave-107 body is a local optimum at 7072 B: ~2 500 single edits and
  ~1 400 reorder moves produce no 7072-B variant above 48.0 %.
* The best fuzzy variant for this function is the `0x180`↔`0x188` swap at
  **7064 B / 48.028673835125 %**, 8 B short of the required size.
* Shared-int round trips collapse the full ctor (16–47 %); the flat body
  is the better source shape for the metric.
* The original's pool has four float constants ours lacks; the
  corresponding source edits are code-identical or shrink the function.
* The hybrid block tree is 784 B too large; the `gen.py` block tree is
  600 B too small.

**Hypothesis (not proven):**

* The remaining gap is dominated by the original's SLP vectorisation of
  the mid-block ternaries (`vdup/vshl/vbsl/vmov.f32` groups at
  `d1cba..d1d20`) and its `d0-d1` long-lived constant group, which the
  flat body's hill-climbed statement order does not reproduce. Matching
  it would require the original's exact per-field flag-test order, not
  just the field order.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5605
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5605/score1u.py _ZN6LayoutC1Ev        # 7072 B, 48.000000000000
```

Scratch harness (all under `/dev/shm/w5605`): `try.sh` (swap ctor body →
`link_so` → score), `score1u.py` (unrounded linked scorer),
`winratio.py` (200-entry window ratios), `regions.py`, `pooldiff.py`,
`adjscan.py` / `adjscan2.py` (adjacent-swap scans), `randsearch.py` /
`randsearch2.py` (random moves), `scanfloat.py` / `scanfloat2.py`,
`scancast.py`, `scanfy.py`, `scanifelse.py`, `combo.py` / `combo2.py` /
`combo3.py` / `combo4.py`, `hybrid.py`, `uniform.py`, `vstrswap.py`,
`body.base.txt` (pristine ctor body), `units.txt` (203 top-level units).
