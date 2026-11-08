# `Layout::Layout` C1/C2 — wave 150 / worker w9608
## Device-major tail: the 268 B pool gap is NOT in the `0x13c..0x1e8` tail.
## It is distributed over the `0x2c4..0x2d8` and `0x2f4..0x318` device groups.
## No size-exact (7,072 B) source variant beats the baseline; source unchanged.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (wave-149 HEAD `7eed2966e`): raw same-address diff **5,768**,
aligned instruction diff **2,198**, size 7,072, code **6,312 B**, final
literal pool **760 B / 190 words**, first `0x164` store at **`0xed8`**.
Gate on entry and exit: byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. Method

The ctor body was parsed into 231 top-level statements (`/dev/shm/w9608/gen.py`,
`parse.py`) so any permutation can be regenerated and rebuilt.  `eval.sh` =
`link_so` + `fdiff.py` + `align_diff.py` + `pool.py` (~0.9 s per variant).
`stores.py` is an SLP-aware store table (resolves `vst1`/`stmia` bases from the
preceding `add.w rX, r0, #imm`).  `pool.py` does a real reachability sweep to
find the code end, so `code`/`pool` are exact (not the 242-vs-194 word estimate
of w9503, which counted the mid-function pools too).

## 2. Original store-offset table (window `0xce0..0x1100`, function offsets)

```
332@cec 312@cfa 316@cfe 336@d0a 340@d16 348@d1c 340@d2e 340@d48 340@d78
344@d90 348@dbe 352@dea 356@df4 356@e0c 356@e2a 360@e36 380@e4a 364@e52
384@e72 388@e7a 392@e80 404@e94 416@e98 364@ea2 400@eba 384@ec4 408@eca
416@ece 364@ee4 364@ef6 368@f0c 372@f22 376@f36 372@f46 376@f56 376@f72
384@f7c 388@f86 388@f9a 388@fae 392@fb8 396@fc8 396@ff8 396@1012 404@101c
408@1032 416@1036 420@1058 420@1064 424@1072 428@1086 480@1100
```
`316@cfe` is the `0x13c..0x148` SLP group; `364@e52` is the iPadLarge
`0x16c..0x178` group; `384@ec4` is the iPadLarge+n9 `0x180..0x18c` group.

## 3. Baseline store-offset table (same window)

```
288@ce8 276@cf8 264@d0c 296@d10 268@d14 304@d1e 172@d3e 304@d4e 304@d88
312@da0 332@db0 336@dcc 340@de2 328@df4 340@dfa 340@e0a 328@e28 344@e44
344@e62 700@e82 516@e86 352@ecc 356@ed8 356@ef0 356@f00 360@f1e 380@f42
364@f4e 364@f6a 404@f70 380@f76 364@fb4 380@fc0 404@fd2 388@fe0 400@fea
392@ff2 420@ff6 384@ffa 400@1014 420@101c 384@1020 392@1024 400@102a
392@1038 388@1050 344@106a 388@107e 400@1090 384@109e 420@10ae 396@10ce
412@10d2 396@10ea 412@10f0 424@10fa
```
First `0x164` store at `ed8` (ORIG `df4`, +0xe4).

## 4. Full ascending tail (reproduced)

Reordering `0x13c..0x1e8` ascending (moving `0x140`/`0x144` up, and
`eventBannerScale`/`0x1e4`/`0x1e8` after `0x1dc`) reproduces w9503's structural
targets exactly:

```
172@cfa 304@d2a 304@d48 320@d8a 328@d90 336@dac 340@dcc 344@dd0 348@dd4
352@de8 356@df4 356@e0c 340@e1e 348@e26 356@e36 360@e54 380@e78 364@e84
364@ea2 380@ea8 340@ed8 364@f16 380@f22 384@f34 388@f42 392@f52 400@f58
388@f6e 392@f84 400@f8a 404@f96 388@fb8 392@fc8 396@fec 400@ff6 400@1018
404@102a 408@103a 416@103e 408@1058 416@105c 408@1080 416@1084 420@10a0
424@10ac 428@10c4 480@10e8 448@10f0 432@10f8
```
First `0x164` store at **`0xdf4`**, `0x140..0x14c` SLP group at `d8a`.
Size **6,836 B**, code 6,128, pool **708 B / 177 words**.

## 5. Where the pool words actually are (new result)

Counting the `addw rX, pc, #imm` + `vld1.64` constant-vector loads over the whole
function:

| region (function offset) | ORIG vectors | ours (ASC) |
|---|---:|---:|
| `0x996..0xa14` (early `0x04..0x98` groups) | 9 | 9 |
| `0xa9e..0xaee` (`0x2c4`/`0x2d0`/`0x2cc`/`0x2c8`/`0x2d4`/`0x2d8`) | **8** | **2** |
| `0xb62..0xb9e` (`0x7c`/`0x80`/`0x84`/`0x88`) | **2** | **0** |
| `0xcd8` (`0x90`/`0x94`) | **1** | **0** |
| `0xe3c`/`0xeaa` (`0x16c..0x1a0` tail) | 2 | 4 |
| `0x10ac..0x10e6` (`0x1a4..0x1e8`) | 4 | 3 |
| `0x117a..0x1212` (`0x1ec..0x21c`) | 3 | 4 |
| `0x1230..0x1252` (`0x238..0x260`) | 7 | 7 |
| `0x1298..0x12c2` (`0x264..0x280`) | 3 | 4 |
| `0x13e6..0x141a` (`0x2f4..0x318`) | **7** | **0** |
| **total** | **44** | **34** |

So the 44-word pool gap is **not** in the `0x13c..0x1e8` tail (where ours has
*two more* vectors than the original).  It is concentrated in:
* `0x2c4..0x2d8` — the original has 8 constant vectors, ours 2 (6 missing = 24 words);
* `0x2f4..0x318` — the original has 7, ours 0 (7 missing = 28 words);
* `0x7c..0x94` — the original has 3, ours 0 (3 missing = 12 words).

Those three groups alone account for 64 words; the tail over-supply (-2 vectors)
and the other regions net it back to 44.  **The "device-major tail" premise of
w9503 is therefore only half true: the duplicated arms that carry the extra
constant vectors live in the `0x2c4..0x2d8` (curved/`iPadLarge` block) and
`0x2f4..0x318` (choice-window/help-window block) groups, not in the
`0x13c..0x1e8` group.**

## 6. Experiments (all real `link_so` + fdiff + align_diff + pool)

| variant | size | raw | aligned | code | pool words | `0x164` @ |
|---|---:|---:|---:|---:|---:|---|
| HEAD baseline | **7,072** | **5,768** | **2,198** | 6,312 | 190 | `ed8` |
| ASC (full ascending tail) | 6,836 | 5,681 | 2,658 | 6,128 | 177 | **`df4`** |
| ASC + `0x204`/`0x2bc` left in place | 6,904 | 5,785 | 2,536 | 6,150 | 188 | `e88` |
| ASC + explicit if/else `0x16c..0x1a0` (dev1) | 6,840 | 6,056 | 4,202 | 6,422 | 104 | — |
| ASC + `__builtin_expect(flag,1)` all 5 flags | 6,988 | — | — | 6,460 | 132 | — |
| ASC + `__builtin_expect(flag,0)` all 5 flags | 7,056 | — | — | 6,082 | 243 | `f2e` |
| ASC + `expect(iPadHD,0)` early only | 6,900 | — | — | 5,978 | 230 | — |
| ASC + `expect(iPadHD,0)` tail only | 6,836 | — | — | 6,128 | 177 | `df4` |
| random adjacent-swap search, size 7,072 | 7,072 | 6,353 | 3,430 | 6,664 | 102 | `ed0` |

Key negatives:
* `__builtin_expect` on the device flags moves the pool in the **wrong**
  direction: `expect(...,1)` shrinks the pool (177 -> 132 words) and
  `expect(...,0)` inflates it (177 -> 243) but sinks the code by 200+ B and
  pushes the first `0x164` store to `f2e`.
* Explicit source-level device duplication for `0x16c..0x1a0` *shrinks* the
  pool to 104 words: clang materialises the per-arm constants as `movs`
  immediates instead of constant vectors.
* 120 adjacent-swap hill-climb steps constrained to `0x164 @ df4` never left
  6,904 B; the ASC tail's size is a hard local optimum at ~6.9 KB.

## 7. Conclusion / next lever

The ASC tail is structurally right (`0x164 @ df4`, `0x140..0x14c` SLP group)
but the function is 236 B short.  The budget is not recoverable by reordering
the `0x13c..0x1e8` statements: the missing constant vectors are in the
`0x2c4..0x2d8`, `0x2f4..0x318` and `0x7c..0x94` device groups, which the ASC
reorder leaves untouched.  Next pass: reconstruct those three groups as
device-major arms (they are the `field_0x2c4`/`0x2d0`/`0x2cc`/`0x2c8`/`0x2d4`/
`0x2d8` and `field_0x2f8`/`0x2f4`/`0x300`/`0x2fc`/`0x304`/`0x30c`/`0x310`/
`0x314`/`0x318` ternaries) **together with** the ASC tail, and expect the pool
to grow toward the original's 884 B / 221 words.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9608
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9608/fdiff.py _ZN6LayoutC1Ev        # 7072 / 5768
python3 /dev/shm/w9608/align_diff.py _ZN6LayoutC1Ev   # 2198
python3 /dev/shm/w9608/pool.py ours _ZN6LayoutC1Ev    # code 6312 pool 760/190
python3 /dev/shm/w9608/stores.py ours _ZN6LayoutC1Ev 0xce0 0x1100
```

Scratch `/dev/shm/w9608/`: `gen.py`, `parse.py`, `mkasc.py`, `mkasc2.py`,
`mkbest.py`, `mkvar.py`, `sweep_exp.py`, `sweep_exp0.py`, `sweep_tail.py`,
`sweep_early.py`, `sweep_early2.py`, `search.py`, `search2.py`, `search3.py`,
`eval.sh`, `fdiff.py`, `align_diff.py`, `pool.py`, `stores.py`,
`dumppool.py`, `pool_orig.txt`, `pool_ours.txt`, `orig.dis`, `ours.dis`.

Source is unchanged (baseline kept: every 7,072 B variant measured regressed
both raw and aligned).
