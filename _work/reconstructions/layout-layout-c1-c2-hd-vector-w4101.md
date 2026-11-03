# Layout::Layout C1/C2 — HD-block SLP structure, register allocation, and the
# 7072-byte ceiling (w4101, pass 8)

Status: analysis-only pass (no code change). Worktree `wt/w4101`, branch
`wave91-w4101`, base `a4557300`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF `0xd1028`, 7072 B).

This note continues `layout-layout-c1-c2-sweeps-w4001b.md` (pass 7),
`layout-layout-c1-c2-midtail-vstr-w4001.md` (pass 6) and
`layout-layout-c1-c2-source-shape-w3905.md`. It records new evidence about
*why* the body cannot be improved further with the current toolchain, and the
measurements that close the remaining candidate directions.

## Result

`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` stay at **47.7% / 7072 B**. Gate unchanged:

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4498 / 4524 | **4498 / 4524** |
| `linked_exact` | 4498 / 4524 | **4498 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| `.text` / `.rodata` | `0x1337c8` / `0x41b67` | **unchanged** |
| `.ARM.exidx` / `.ARM.extab` | `0x6f70` / `0x26290` | **unchanged** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`. No size-7072 source variant above 47.7 was found.

## 1. The original HD block is SLP-vectorized, and our body vectorizes it the same way

The original `if (iPadHD)` block (`0xd1042..0xd1268`) is **not** a scalar
store sequence. clang 7.0.2 `-Oz` SLP-vectorizes the contiguous HD constant
assignments into 4-word `vst1.32 {dN-dM}` stores, with 3-word `stmia`/`strd`
for the leftovers. The exact store order recovered from the disassembly
(`parse3.py` on the 137 unique HD offsets) is:

```
14 18 1c 20 | 4 8 c 10 | 24 28 2c 30 | 34 38 3c 40 | 44 48 4c 50 |
54 58 5c 60 | 64 68 6c 70 | 2c8 2cc 2d0 2d4 | 7c 80 84 88 | 74 78 |
2c0 2c4 | 2d8 | ac | 8c 90 | 94 98 9c a0 | a4 a8 | c0 c4 c8 cc |
b0 b4 b8 bc | ec f0 f4 f8 | 294 298 29c 2a0 | 2a4 2a8 2ac 2b0 |
108 10c 110 114 | 118 11c 120 124 | d0 d4 d8 | dc e0 e4 e8 |
fc 100 104 | 2b4 2b8 | 128 12c 130 134 | 140 144 148 14c |
150 154 158 15c | 160 164 168 16c | 170 174 178 17c | 180 184 188 18c |
190 194 198 19c | 1a0 1a4 1a8 1ac | 1b0 1b4 1b8 1bc | 1c0 1c4 1c8 1cc |
1d0 1d4 1d8 1dc | 138 13c | 1e0
```

The groups are exactly the contiguous 4-word runs of the iOS source order
(`4 8 c 10 14 18 1c 20 24 28 2c 30 ...`); the scheduler rotates the first two
groups (`14..20` before `4..10`) and later re-orders the scalar/`strd`
leftovers. The current body's HD block uses the **same vector groups in the
same order** for the first eight groups; the two bodies diverge only in how
the non-vectorizable leftovers (`74 78`, `2c0 2c4`, `2d8`, `ac`, `8c 90`,
`94 98 9c a0`, ...) are interleaved. So the HD-block delta is *scheduling and
register allocation*, not a missing statement, a different expression, or a
different grouping.

## 2. The compiler reproduces the tail-merged branch tree from the flat-ternary source

The original is a tail-merged branch tree:

| region | original range | fields |
|---|---|---:|
| HD block | `d1042..d1268`, `b.w d21e2` | 137 |
| n9 block | `d127e..d12fc`, `b.n d1860` | 34 |
| retina/default block | `d1570..d1830` | 34 |
| mid block | `d1860..d215c`, `b.n d21e2` | 103 |
| tail | `d21e2..d2bd0` (second `if (iPadHD)`) | 69 |

Our flat-ternary body is compiled into the **same tree**: the prologue test is
`beq.w d1278` (skip HD), the n9 test is `beq.w d1568` (original `beq.w d1570`), and the tail has
its own `cmp.w sl,#0` / `beq.n` HD branch (original `d21f6`/`d2202`, ours
`d227c`/`d2284`). So the source form (flat ternaries vs explicit blocks) is
*not* the remaining difference — clang's if-conversion/tail-merging produces
the block tree either way. This is why the explicit block-tree source
(5900-6744 B) is *smaller* than the original: it lets the compiler share more,
whereas the original's 7072 B is the flat-ternary + 13 if/else codegen.

## 3. The residual is register allocation (prologue and `cbz`)

| | original | current |
|---|---|---|
| prologue save | `stmdb sp!, {r6,r7,r8,r9,sl,fp}` (6) | `stmdb sp!, {r5,r6,r7,r8,r9,sl,fp}` (7) |
| `cbz` (16-bit compare-and-branch) | 28 | 17 |
| `beq` | 36 | 52 |
| `b.n` | 54 | 62 |
| instructions | 2529 | 2503 |

The original keeps the five `Globals` flag values in **low** registers at each
test site, so clang can use the 16-bit `cbz`; our body keeps them in
`r8/r9/sl/fp`, so clang emits `cmp; beq` (4 B). The original also uses `r5`
as scratch (e.g. `movs r5,#14`, `movw r5,#369`), while our body keeps a live
value in `r5` across the HD block (`add.w r5,r0,#448` ... `vst1.32 [r5]`),
which is why `r5` is callee-saved in our prologue. This single difference
shifts every register assignment from the first instruction onward and is the
dominant cause of the HD-block mismatch (25.8% matched).

## 4. Order experiments: the current order is a genuine local optimum

Per-region fuzzy match (`regions.py`, official verifier, `autojunk=False`):

| body | size | hd | n9 | def | mid | tail | overall |
|---|---:|---:|---:|---:|---:|---:|---:|
| current (hill-climbed) | 7072 | 25.8% | 55.3% | 60.2% | 28.9% | 49.4% | **47.7%** |
| iOS source order, current expressions | 6724 | 3.7% | 92.1% | 19.3% | 20.1% | 35.9% | 34.6% |
| HD store order as source order, iOS order elsewhere | 6636 | 4.9% | 28.9% | 31.3% | 17.7% | 31.6% | 32.4% |

The iOS order is the *true* source order (recovered from the iOS 1.1.4
Mach-O; §2 of the w3905 note) and it reproduces the n9 block almost exactly
(92.1%), but the scheduler reorders the HD block so heavily that the true
source order scores worse on the fuzzy metric than the hill-climbed order.
Using the recovered HD store order as the source order does not help either
(4.9%): the store order is the scheduler's output, not the input.

## 5. The float-store (`vstr`) set and per-field float/int conversions

The original's 27 scalar `vstr sN,[r0,#off]` stores (decimal offsets):

```
96 104 108 112 140 144 156 168 220 224 228 232 312 336 344 348 352 360
368 424 480 484 712 720 760 992 1016
```

The current body emits 26:

```
32 96 104 108 112 140 144 168 220 224 228 232 292 312 328 352 360 424
480 500 516 712 720 760 992 1000
```

Common (20): `60 68 6c 70 8c 90 a8 dc e0 e4 e8 138 160 168 1a8 1e0 2c8
2d0 2f8 3e0`. Original-only (7): `9c 150 158 15c 170 1e4 3f8`. Current-only
(6): `20 124 148 204 3e8 1f4`.

Measured on the pristine 7072-byte body (official verifier):

| change | size | pct |
|---|---:|---:|
| `0x150` `5` -> `5.0f` | 7048 | 47.7 |
| `0x158` `70` -> `70.0f` | 7064 | 47.6 |
| `0x170` `174` -> `174.0f` | 6984 | 47.6 |
| `0x1e4` `5` -> `5.0f` | 7032 | 41.3 |
| `0x20` `5*1.40625f` -> `7` | 7076 | 40.2 |
| `0x124` `1*1.40625f` -> `1` | 6992 | 43.2 |
| `0x148` `2*1.40625f` -> `2` | 7048 | 47.4 |
| `0x204` `9*1.40625f` -> `12` | 7064 | 47.6 |
| `0x3e8` `100*1.40625f` -> `140` | 7024 | 47.3 |
| `0x1f4` `16*1.40625f` -> `22` | 7064 | 47.4 |
| `helpWindowX` innermost `(float)` | 7072 | 47.7 |
| `rewardBoxY2` whole chain float-typed | 7080 | 47.6 |
| `0x1f4` float-typed fallback | 7072 | 47.7 |

None reaches 7072 with a score above 47.7.

## 6. `field_0x98` if/else body: the +56-byte search

The `field_0x98` if/else body is the only variant above 47.7: **7016 B /
47.8%**. To land it, a real construct must add exactly 56 B without disturbing
the instruction stream. On that 7016-byte body, single ternary->if/else
conversions give:

| field | size | pct |
|---|---:|---:|
| `0x134` | 7024 | **47.8** |
| `0x2cc` | 7036 | 47.5 |
| `0x2c4` | 7052 | 47.4 |
| `0x308` | 7056 | 45.8 |
| `0x310` | 7060 | 47.1 |
| `0xdc` | 7064 | 43.0 |
| `0x120` | 7064 | 43.0 |
| `0x74` | 7076 | 47.2 |
| `0x224` | 7076 | 47.2 |
| `0xa0` | 7072 | 43.2 |

Combinations that hit exactly 7072 (all measured): `0x308+0xd4` 41.9,
`0xdc+0x18` 41.0, `0x120+0xcc` 43.4, `0x120+0xa4` 43.4. The size deltas are
strongly non-additive (e.g. `0x2c4` +36 and `0x2cc` +20 together give only
+12), and every 7072 combination loses more than the 0.1 point the
`field_0x98` change gains. The 7016-byte body therefore cannot be grown to
7072 without a regression.

## 7. Conclusion

* The 272-byte gap is the 13 if/else conversions (all-ternary 6800 B ->
  current 7072 B); there is no missing assignment, helper, or cold block.
* The original HD block is SLP-vectorized and our body groups it identically;
  the residual is scheduler/register-allocation, dominated by the original's
  six-register prologue and low-register `cbz` flag tests.
* The true (iOS) source order is *not* the fuzzy-metric optimum; the
  hill-climbed order is a genuine local optimum for the verifier.
* No source variant at exactly 7072 B beats 47.7; `field_0x98` if/else is
  47.8 only at 7016 B, which shifts `.text` and drops `byte_exact`.
* Byte-exactness requires reproducing clang's register allocation exactly,
  which needs the exact Android 2.0.16 statement order *and* expression types
  (the 7 missing / 6 extra `vstr` fields). That is the remaining work.

## 8. Reproduction

```sh
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# variant scorer (w4101 scratch)
python3 /dev/shm/w4101/try.sh <Layout.cpp>                # copy, link, score
python3 /dev/shm/w4101/regions.py                         # per-region match
python3 /dev/shm/w4101/conv_field.py <in> <out> <off>...  # ternary -> if/else
python3 /dev/shm/w4101/conv_ifelse.py <off> <in> <out>    # if/else -> ternary

# HD store order
python3 - <<'PY'
import parse3 as P3
o=[]
for a,k,offs in P3.parse('/dev/shm/w4101/orig_full.txt',0xd1042,0xd126c): o+=offs
seen=set(); print(' '.join('%x'%x for x in o if not (x in seen or seen.add(x))))
PY

# vstr set
<ndk-objdump> -d --start-address=0xd1028 --stop-address=0xd2bc8 <so> |
  grep 'vstr'
```

Ghidra: image base `0x10000`; `Layout::Layout` at `0xe1028`. The working
project is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`
(program `android_2.0.16_libgof2hdaa.so`); decompile with
`ADDR=0xe1028 analyzeHeadless ... -postScript DecompAt2.java`.
