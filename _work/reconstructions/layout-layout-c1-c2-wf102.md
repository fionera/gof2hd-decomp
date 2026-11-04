# Layout::Layout C1/C2 — values are fully proven; only literal-pool/vector-store
# codegen remains (wf102, wave 102)

Worker **wf102**, branch `wave102-wf201`, base `53d9ee4d5`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**, current fuzzy **47.7724741448 %** (unrounded `fscore2.py`,
`autojunk=False`). Continues `layout-layout-c1-c2-ios-order-wf101.md` (wave 101)
and `layout_ctor/ios_source_order.md`.

## Result

**No landable improvement found.** The tree stays at **7072 B /
47.7724741448 %**. This pass adds four new, strong results:

1. **Every one of the 204 initialised fields has the same 32-bit value as the
   original in all 32 flag combinations.** Proven by emulating both binaries
   (`emu2.py`), not by inspection. The ctor's *values* are therefore complete
   and correct; the remaining delta is pure codegen.
2. **All 50 other `Layout` methods are already byte-exact**, including
   `reload`, `drawBox`, `formatNumber`, `resetWindowDimensions`, the
   destructor and every `draw*`. So the `Layout` struct layout and field types
   are confirmed correct; a "root-cause struct fix" cannot help the ctor
   without breaking the other 50 methods.
3. **The Ghidra decompile identifies the 20 fields whose initialiser is
   float-typed** (int→float→int round trip survives in the non-HD branch):
   `0x60 0x68 0x6c 0x70 0x8c 0x9c 0xa8 0x138 0x150 0x158 0x15c 0x160 0x168
   0x170 0x1a8 0x1e4 0x2c8 0x2d0 0x3e0 0x3f8`.
4. **The residual difference is literal-pool / vector-store grouping, not
   order or values.** The original has 81 `vst1.32`, 86 `vld1.64`, 62 `strd`,
   121 `str` and an **884-byte pool** (221 words). The iOS source order has
   75 / 80 / 75 / 134 and a **744-byte pool** (186 words). The current hill
   order has 70 / 77 / 45 / 195 and a **704-byte pool** (176 words). The
   original's code region is 6188 B, the iOS order's is 6192 B — the *code
   sizes agree*; the original simply folds 35 more constants into pool-loaded
   vector groups.

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

## 1. Proof that all values are right

`cmp_truth.py` runs the original and our `.so` through the Thumb emulator for
all 32 combinations of `iPadHD, n9, retinaDisplay, iPadLarge, iPad` and
compares the raw 32-bit word at each of the 204 field offsets:

```
mismatches 0
```

This subsumes the partial `truth_orig.json` table from wave 101 and rules out
every "wrong constant / wrong flag order / wrong int-vs-float value" theory:
if a flag were tested in the wrong order the *values* would still agree, but
every value is bit-identical, so all remaining differences are instruction
selection, scheduling and literal-pool layout.

## 2. Proof that the struct is correct

`verify.py` on every `_ZN6Layout*` symbol: all 50 methods other than
`C1`/`C2` are `byte=True`, including the 1388-byte `drawBox`, the 1064-byte
`reload`, and the 672-byte `drawMissionRewardMessage`. A field reorder or type
change would break at least one of them.

## 3. The decompile's int/float split

`layout_decomp.c` (Ghidra `decompile_function` at `0xe1028`) renders the
float-typed initialisers as

```
fVar = (float)VectorSignedToFloat(uVar, ...);
*(int *)(this + OFF) = (int)fVar;
```

Parsing every `*(int *)(this + N) = (int)fVar…` gives exactly 20 offsets
(listed above). All other fields are stored from integer registers or from
constant pairs, i.e. their initialiser is int-typed. This is the only
source-visible type information the binary exposes.

## 4. Measured table (all real `link_so` sizes, official unrounded scorer)

| body | `usize` | raw % | notes |
|---|---:|---:|---|
| original | 7072 | 100 | code 6188, pool 884 |
| current hill order (HEAD) | **7072** | **47.7724741448** | code 6296, pool 704 |
| iOS source order | 6936 | 38.00723182 | code 6192, pool 744 |
| iOS order + all 20 float-typed | 6984 | 38.0360721443 | |
| iOS order + int HD arms + float non-HD | 7064 | 16.7025229806 | size closest to 7072 |
| def-execution store order | 6804 | 27.5512274295 | |
| decompile tree, HD block first (truth-table values) | 6560 | 15.9705159705 | |
| decompile tree, `!iPadHD` first (split HD) | 6688 | 26.3942798774 | |
| hill order + all 20 float-typed | 7088 | 42.542204568 | |
| hill order + non-float HD arms made int | 6952 | 14.9793591508 | |
| **unconstrained adjacent-swap hill (2 swaps)** | **6976** | **48.1725584182** | size regresses |

The unconstrained adjacent-swap improvement is two swaps:
`0xac <-> 0x98` (unit index 44) and `0x19c <-> 0x1a8` (index 120). The first
alone is `6976 B / 48.1022772673 %`; the second alone is
`7072 B / 47.6834360708 %`. The score gain comes entirely with a 96-byte size
loss, so it is not landable while `size_exact` must stay 4524/4524.

## 5. Negative searches run this pass

* **Size-constrained adjacent-swap hill climb, 3 passes** (only accept
  `size == 7072` and a strictly better score): no move accepted; the hill
  order is a strict local optimum in the adjacent-swap neighbourhood.
* **250 random 2-swaps** at `size == 7072`: no improvement over
  `47.7724741448`.
* **Ternary vs if/else**: converting `field_0x94` to an explicit
  `if/else if/else` chain produces a **byte-identical** binary (same
  `47.7724741448`), confirming LLVM canonicalises the two forms and that
  source-level `?:`/`if` spelling is irrelevant.
* **Branch polarity inversion**: rewriting every `iPadHD ? A : B` as
  `!iPadHD ? B : A` is a no-op (byte-identical, same score).
* **Vector-store counts** (`objdump`): original 81 `vst1.32` / 86 `vld1.64` /
  62 `strd` / 121 `str`; iOS order 75 / 80 / 75 / 134; hill order 70 / 77 /
  45 / 195. The original's extra pool words are 4-word constant groups
  (`vld1.64 {dN-dN+1}` + `vst1.32 {dN-dN+1}`) in the retina/default branch,
  e.g. `0xb0..0xbc`, `0xc0..0xcc`, `0xec..0xf8`, `0x2a0..0x2ac`,
  `0x180..0x18c`, `0x1b0..0x1bc`, `0x1c0..0x1cc`, `0x1d0..0x1dc`.

## 6. Conclusion

* **Proven:** all 204 field values are correct in all 32 flag combinations.
* **Proven:** the `Layout` struct is correct (50/50 other methods byte-exact).
* **Proven:** the iOS 1.1.4 source order is the Android 2.0.16 source order
  (wave 101 pool evidence; re-confirmed here).
* **Proven:** the hill order is a strict local optimum at 7072 B under
  adjacent swaps and 250 random 2-swaps.
* **Hypothesis:** byte-exactness needs the iOS order *plus* a register
  allocator that keeps the flag bytes in low registers and a store-merging
  pass that produces the original's 81 four-word vector groups. The iOS
  order's code size already matches the original (6192 vs 6188); only the
  pool/vector grouping is 140 bytes short.
* **Recommendation:** do not land the iOS order (size 6936) or the tree
  (6560/6688) — both regress `size_exact`. The next attempt should start
  from the iOS order and try to force the retina/default 4-word groups into
  the pool, e.g. by making the grouped fields' initialisers share a
  float-typed temporary, rather than by reordering the current hill body.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf201
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/wf201/fscore2.py                            # 47.7724741448, 7072 B
# iOS source order
python3 /dev/shm/wf201/reorder.py $(cat /dev/shm/wf101/ios_order.txt)
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/wf201/fscore2.py                            # 38.00723182, 6936 B
# full value proof
PYTHONPATH=tools/verify EMU_SO=../_work/bins/android_2.0.16_libgof2hdaa.so \
  python3 /dev/shm/wf201/cmp_truth.py                        # mismatches 0
```

Scratch (`/dev/shm/wf201`): `fscore2.py` (official unrounded scorer),
`emu2.py` (Thumb emulator with `vld1`/`vst1`/`pc` trace entries),
`cmp_truth.py` (32-config value comparison), `gen_tree.py` (decompile-tree
generator), `int_hd_float_non.py`, `reorder.py`/`units.py` (copies of the
wave-101 reorder tooling), `hill.py`, `hill7072.py`, `rand2.py`,
`Layout.hill.cpp` (pristine ctor), `Layout.ios.cpp` (iOS order),
`layout_decomp.c` (Ghidra decompilation at `0xe1028`).
