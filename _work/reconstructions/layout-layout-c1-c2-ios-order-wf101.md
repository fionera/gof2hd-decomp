# Layout::Layout C1/C2 — the iOS source order is the true shape, but it is not
# the fuzzy-metric optimum (wf101, wave 101)

Worker **wf101**, branch `wave101-wf101`, base `3856949ab`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B** (code `0xd1028..0xd2802`, end pool `0xd2854..0xd2bc8`; the ctor's
literal pools live inside `.text`). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`, so the ctor is `0xe1028` in the Ghidra project).

Continues `layout-layout-c1-c2-pass11-w4802.md` (wave 98, the landed 47.733
state), `layout-layout-c1-c2-order-search-w4901.md` (wave 99, the landed
47.772 state), `layout-layout-c1-c2-store-order-w5001.md` (wave 100) and
`layout_ctor/ios_source_order.md`.

## Result

**No landable improvement found.** The tree stays at **7072 B /
47.7724741448 %** (unrounded `fscore.py`, `autojunk=False`). This pass adds one
new, strong structural result that the earlier notes did not have:

> The **iOS 1.1.4 source order** is the true Android 2.0.16 source order.
> It reproduces the original's **literal-pool integer run exactly for the first
> 44 words**, and it puts the six disputed fields (`0x94`, `0x140`, `0x144`,
> `0x204`, `0x23c`, `0x2bc`) in the same HD/tail blocks as the original. The
> current hill-climbed order does the opposite. But the iOS order compiles to
> **6936 B**, not 7072 B, so it is **not landable** (`size_exact` would drop
> 4524 -> 4523); its fuzzy score is only **38.0072 %** because the register
> allocator assigns different registers from the first HD store onward.

Gate is unchanged and green (`check` on the pristine tree):

| metric | value |
|---|---:|
| `byte_exact` | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 |
| `size_exact` | 4524 / 4524 |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 |
| `data_exact` | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

`RATCHET PASS`.

## 1. Baseline and the two candidate bodies

| body | `usize` | raw % | insns | code end | code bytes | pool bytes |
|---|---:|---:|---:|---:|---:|---:|
| original | 7072 | 100 | 2529 | `0xd2804` (pop at `0xd2800`) | **6106** | **966** |
| current hill order (HEAD) | 7072 | **47.7724741448** | 2529/2499 | `0xd28c0` (pop at `0xd28be`) | **6296** | **776** |
| iOS source order | 6936 | 38.00723182 | 2529/2449 | `0xd27bc` (pop at `0xd27ba`) | **6036** | **900** |

The hill order is +190 B of code and -190 B of pool relative to the original;
the iOS order is -70 B of code and -66 B of pool. The iOS order is
structurally much closer even though its fuzzy score is 9.8 pp lower.

## 2. New evidence: the literal pool proves the iOS order is the source order

The original's end pool is a single contiguous run. After the four floats
`0.325, 0.65, 100, 50` at `0xd2854` and the GOT-offset words, the integer run
starts at **`0xd2890`**:

```
4 22 25 12 11 11 10 270 60 120 300 600 360 146 8 6 198 180 189 54 13 37 22 2
-1 2 1 -1 2 22 60 120 19 19 19 243 13 14 10 360 6 8 4 180 ...
```

The **iOS-order build's** integer run starts at `0xd2858` and is
**word-for-word identical for those first 44 words**:

```
4 22 25 12 11 11 10 270 60 120 300 600 360 146 8 6 198 180 189 54 13 37 22 2
-1 2 1 -1 2 22 60 120 19 19 19 243 13 14 10 360 6 8 4 180 ...
```

The hill order's pool starts with a different integer sequence. This is not a
scheduling coincidence: the pool is emitted in first-use order, and a 44-word
exact match means the iOS order's first 44 pool-using fields are in the same
order as the original's. (The divergence after word 44 is the mid-block
scheduling difference; see §4.)

## 3. New evidence: the HD-block field set is only right in the iOS order

The original's HD (`iPadHD`) block stores 137 fields. Its field set differs
from the current build's in exactly six fields (w4502 §2.3a):

| | offsets |
|---|---|
| original-only in HD | `0x94`, `0x140`, `0x144` |
| current-build-only in HD | `0x204`, `0x23c`, `0x2bc` |

Source-order index (0-based, 204 units):

| field | iOS order index | current hill order index | block |
|---|---:|---:|---|
| `0x94` | **43** | 178 | HD vs tail |
| `0x140` | **96** | 186 | HD vs tail |
| `0x144` | **97** | 182 | HD vs tail |
| `0x204` | **141** | 102 | tail vs HD |
| `0x23c` | **155** | 87 | tail vs HD |
| `0x2bc` | **181** | 103 | tail vs HD |

The iOS order puts all six on the original's side of the 137/204 split
(`0x1e0` is index 136, `0x1e4` index 137). The hill order puts all six on the
wrong side. This is the root cause of the HD-block field-set mismatch that
w4502/w4701 could only attack field-by-field (and every individual move lost
score).

## 4. Why the iOS order still scores lower

`verify`'s normalized multiset (pools excluded), iOS order vs original:

| ours - original | instruction |
|---:|---|
| +23 | `cmp.w rN, #IMM` |
| +13 | `mov.w rN, #IMM` |
| +12 | `strd rN, rN, [rN, #IMM]` |
| +11 | `str.w rN, [rN, #IMM]` |
| +7 | `movne rN, #IMM` / `it ne` |
| +6 | `moveq.w rN, #IMM` |
| +4 | `vceq.i16`, `moveq`, `add rN, rN, #IMM` |

| original - ours | instruction |
|---:|---|
| +41 | `movs rN, rN` (2-byte NOP/alignment padding) |
| +14 | `movs rN, #IMM` |
| +13 | `cmp rN, #IMM` (2-byte, low register) |
| +11 | `addw rN, rN, #IMM` |
| +11 | `ldrb rN, [rN, #IMM]` (2-byte, low register) |
| +9 | `add.w rN, rN, #IMM` |
| +7 | `it eq`, `lsls rN, rN, #IMM` |
| +6 | `str rN, [rN, #IMM]`, `vld1.64`, `vmov.f32` |
| +5 | `clz`, `vorr`, `vstr` |
| +4 | `vbsl`, `vcvt`, `vst1.32` |

The dominant term is **+41 `movs rN,rN`** (82 B): the original is padded with
2-byte NOPs that the iOS-order build does not emit. The rest is register
allocation: the original keeps the flag bytes in low registers (`cmp`/`ldrb`/
`cbz`), the iOS build keeps them in high registers (`cmp.w`/`ldrb.w`). The
original prologue saves six registers (`r6,r7,r8,r9,sl,fp`); the iOS build
saves seven (`+r5`). The retina/default block alone is 584 instructions in the
original vs 508 in the iOS build (12 `b.n` + 10 `cbz` vs 3 + 2), i.e. the
original branches where the iOS build if-converts.

## 5. Negative searches run this pass

All sizes are real `link_so` sizes; percentages are the official unrounded
`fscore.py`.

### 5.1 Expression/type probes on the current hill order

The original's `vstr` set differs from the current build's in 8 missing and 6
extra fields (w4502 §3). Each was converted to the type shape the original's
store implies (float-typed ternary for a missing `vstr`, integer literal for an
extra `vstr`) and measured singly:

| edit | `usize` | raw % |
|---|---:|---:|
| baseline | 7072 | **47.7724741448** |
| `0x1f4` HD `16*1.40625f` -> `22` | 7056 | 47.7404 |
| `0x3f8` rewardBoxY2 float ternary | 7072 | 47.7232 |
| `0x148` HD `2*1.40625f` -> `2` | 7056 | 47.5811 |
| `0x9c` if/else -> float ternary | 7072 | 47.4448 |
| `0x204` HD `9*1.40625f` -> `12` | 7072 | 47.4354 |
| `0x150` int -> float ternary | 7080 | 47.2460 |
| `0x158`/`0x15c`/`0x170` int -> float | 7072 | 47.7725 (code-identical) |
| `0x1e4` int -> float ternary | 7072 | 41.0817 |
| `0x20` HD `5*1.40625f` -> `7` | 6944 | 20.6239 |
| `0x10c` HD `1*1.40625f` -> `1` | 7008 | 42.4677 |
| rewardBoxWidth HD `100*1.40625f` -> `140` | 7076 | 42.4677 |

All 15 pairs of the six closest edits were also measured; the best is
`{1f4int}` at 47.7404. **Nothing beats 47.7725 at 7072 B.**

### 5.2 The same edits on the iOS order

The iOS order is 6936 B. Its individual size deltas are: `0x10c` +36,
`0x9c` +24, `0x150` +8, `0x1e4` +40, `0x3f8` +40, `0x1f4` -8, `0x20` -28,
`0x148`/`0x204`/rewardBoxWidth/`0x158`/`0x15c`/`0x170` 0. The best subset of the positive edits is `{0x9c, 0x150, 0x158, 0x15c, 0x170,
0x1e4, 0x3f8}` at **7048 B / 38.9699 %** (all seven float-typed). No subset
lands exactly on 7072 B; the closest is 24 B short, and it is 8.8 pp below the
hill order.

### 5.3 Order blends

| variant | `usize` | raw % |
|---|---:|---:|
| iOS head (first 137) + hill tail | 6952 | 37.2643 |
| hill head + iOS tail | 6964 | 35.8275 |
| hill order, `0x7c..0x1dc` -> iOS order | 6912 | 38.7862 |
| hill order, `0x108..0x1dc` -> iOS order | 7024 | 41.2996 |
| hill order, `0x294..0x2b8` -> iOS order | 7072 | 47.3841 |
| hill order, move `0x94` to iOS position | 7024 | 44.5774 |
| hill order, move `0x140` to iOS position | 7000 | 44.3779 |
| hill order, move `0x144` to iOS position | 7016 | 44.2450 |
| hill order, move `0x204` to iOS position | 7032 | 44.3291 |
| hill order, move `0x23c` to iOS position | 7008 | 40.5190 |
| hill order, move `0x2bc` to iOS position | 7072 | 44.3030 |
| hill order, move all six to iOS positions | 7000 | 38.2605 |
| hill order, move `0x94/0x140/0x144` | 7040 | 44.8874 |
| hill order, move `0x204/0x23c/0x2bc` | 7024 | 37.3552 |

Every move that fixes the structural field-set defect *lowers* the fuzzy score.
The current hill order is a genuine local optimum of the fuzzy metric.

## 6. Conclusion / recommendation

* **Proven:** the iOS source order is the original Android source order. The
  first 44 literal-pool integers match exactly, and the six disputed HD/tail
  fields are on the original's side of the 137/204 split.
* **Proven:** the current hill order is the fuzzy-metric local optimum at
  7072 B. Every structural correction tried this pass lowers the score, and
  every expression/type probe lowers it too.
* **Hypothesis:** byte-exactness needs the iOS order **plus** the exact
  original expression types and a register allocation that keeps the flag
  bytes in low registers. The iOS order's 6936 B body is 70 B of code short,
  dominated by 41 missing `movs rN,rN` alignment NOPs; those are compiler
  artifacts, not source statements.
* **Recommendation:** a future from-scratch reconstruction should start from
  the iOS order (it is the real shape) and add the missing expression/type
  detail, not from the current hill order. Do not land the iOS order as-is:
  it breaks `size_exact` (6936 != 7072) and drops the fuzzy score to 38.0 %.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf101
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/wf101/fscore.py                            # 47.7724741448, 7072 B
python3 /dev/shm/wf101/reorder.py $(cat /dev/shm/wf101/ios_order.txt)
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/wf101/fscore.py                            # 38.00723182, 6936 B
```

Scratch (`/dev/shm/wf101`): `fscore.py` (official unrounded linked scorer),
`reorder.py` (204-unit splitter, header offset map, reorder by offset list),
`ios_order.txt` (the 204-offset iOS source order), `units.py`, `edits.py`,
`try_edits.py`, `try_ios_edits.py`, `combo_hill.py`, `combo_ios.py`,
`moveS.py`, `partial.py`, `blend.py`, `ifrange.py`, `ms2.py` (pool-excluded
multiset), `stores.py`, `Layout.hill.cpp` (pristine ctor), `Layout.ios.cpp`
(iOS-order ctor).
