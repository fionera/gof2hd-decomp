# `Layout::Layout` C1/C2 — wave 150 / worker w9708
## Device-major reconstruction of the `0x2c4..0x2d8`, `0x2f4..0x318` and
## `0x7c..0x94` groups: explicit if/else grows the pool only for `0x2f4`
## (+12 words), but every pool-growing variant shrinks the 7,072 B function.
## Source unchanged (baseline kept).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-150 HEAD `db9658a51`: raw same-address diff **5,741**, aligned
instruction diff **2,189**, size 7,072.  Gate on exit: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
`RATCHET PASS`.

## 1. Method

`/dev/shm/w9708/` has a 0.8 s rebuild loop (`eval.sh` = `link_so` +
`fdiff.py` + `align_diff.py` + `pool2.py`).  `pool2.py` finds the final literal
pool as the **minimum pc-relative pool reference** in the tail (the w150
`pool.py` reachability heuristic over-counts by ~10 words when the last cold
block's bytes look pool-like).  `storeseq.py` is an SLP-aware store table
(`str/strd/strb/strh/vstr/vst1/stmia`, bases from the preceding
`add.w rX, r0, #imm`).  `vgroup.py` maps every `vld1.64` to the store offset it
feeds.

## 2. Code / literal-pool split (the real budget)

| build | size | code end | code | pool start | pool words | pool bytes |
|---|---:|---:|---:|---:|---:|---:|
| ORIGINAL | 7,072 | `d2854` | 6,188 | `d2854` | **221** | **884** |
| baseline | 7,072 | `d28d0` | 6,312 | `d28d0` | **190** | **760** |

So the gap is **31 words / 124 B** (not 44 words): the original spends 124 B
*less* code and 124 B *more* pool.  The "44" in the wave-150 note is the
original's count of `vld1.64` constant-vector **loads** (44 vs our 34), not
literal words.

## 3. Literal-pool multiset difference (baseline vs original)

Words present in the original but not in ours (count = extra copies):
`1 2 4 5 6 8 a b c d e f 10 11 13 14 15 1a 1e 22 28 50 64 69 78 a0 b4 c8 d2
f3 12c 15a 168 190 1be 258 320`, the GOT offsets
`13db84 13dc04 13dc9a 13dd96 13ddb4 13de3c 13df2e`, the floats
`3ea66666 (0.325) 3f266666 (0.65)`, and one extra `ffffffff`.
Words present only in ours: `1c 2a 2c 2d 3c 40 45 54 6a 73 7c 80 83 8a a8`,
GOT offsets `13daca 13db80 13dbe0 13dcda 13dd38 13dd7c 13ddce 13dea0`, and the
floats `3fc00000 (1.5) 41400000 (12.0) 41700000 (15.0) 42240000 (40.0)
42a40000 (82.0)`.

The missing words are *duplicated per-device-arm constants*: the original's
`vld1.64` count is 86 (ours 77), and the extra loads are concentrated in the
mid-body `0xb0..0x128` group and the `0x1b0/0x1c0/0x1d0` group, where the
original has a second full copy of the group on the `fp == 0` path
(`d1ac6..d1bc6`, 10 `vld1.64`) that ours shares/if-converts away.

## 4. Store-offset table, target groups (original vs baseline)

`0x2c4..0x2d8` (function window `0x7a0..0x890`):

```
ORIG   2c4@17e0 2c4@17ee 2c4@180a 2c8@1820 2cc@182c 2cc@183a 2cc@1856
       2d0@1882 2d4/2d8@18b6(strd)
BASE   2c4@17ce 2c4@17f8 2c4@1816 2d0@1830 2cc@183a 2cc@184a 2cc@1866
       2c8@1888 2d8@18aa 2d4@18d4
```

`0x2f4..0x318` (function window `0x1640..0x1760`):

```
ORIG   2f4/2f8@266a(strd) 2fc/300@2672(strd) 304@2676 308@2706
       30c/310@2716(strd) 314@271a 318@2778
BASE   2f4/2f8@2720(strd) 2fc/300@2728(strd) 308@2732 2f8@2756
       2f4@2766 2fc/300@276e(strd) 308@2780 304@278c 30c..314@27a0(stmia)
       314@2824 318@2836
```

`0x7c..0x94` (function window `0x880..0x970`):

```
ORIG   7c/80@18cc(vst1) 7c/80@1934(strd) 84/88@1938(strd) 8c@1958 90@195c
       94@196c 98@199a 9c@19a8
BASE   7c/80@18e2(vst1) 7c/80@18fe(strd) 84/88@1902(strd) 8c@190c 90@1910
       94@1920 98@192c 9c@1940
```

The original's `0x2d4`+`0x2d8` are a single `strd` at `d18b6`; ours stores
them separately (`d18aa`, `d18d4`) — the w9504 open question is still open.

## 5. Vector-load counts per store group (original vs baseline)

```
ORIG   b0:3 c0:3 ec:3 108:3 118:2 128:2 | 1b0:3 1c0:3 1d0:3 | 294:2 2a4:2
BASE   b0:2 c0:1 ec:0 108:2 118:2 128:1 | 1b0:2 1c0:2 1d0:1 | 294:1 2a4:1
```

(ours stores the `0xec` group as `0xdc/0xe0`; the offset shift is the SLP base
tracking, not a struct difference.)  This is where the 31 words live.

## 6. Experiments (real `link_so` + fdiff + align_diff + pool2)

| variant | size | raw | aligned | code | pool words |
|---|---:|---:|---:|---:|---:|
| HEAD baseline | **7,072** | **5,741** | **2,189** | 6,312 | 190 |
| B: explicit if/else `0x2c4`/`0x2c8` + `0x2cc`/`0x2d0` | 6,952 | 5,883 | 2,315 | 6,192 | 190 |
| C: B + explicit if/else `0x2f4..0x318` | 6,988 | 5,856 | 2,403 | 6,180 | **202** |
| D: C + explicit if/else `0x7c..0x9c` (0x94/0x98 moved in) | 7,036 | 5,835 | 2,393 | 6,224 | **203** |
| D + swap(`0x1d8`,`0x1c8`) | 7,044 | 5,795 | 2,397 | 6,240 | 201 |
| C + swap(`0xa8`,`0xb0`) | 7,044 | 6,298 | 4,310 | 5,618 | (mid pools) |
| D + swap(`0xa8`,`0xb0`) | **7,072** | 6,348 | 4,270 | 6,554 | 129 |
| baseline + swap(`0xa8`,`0xb0`) | 7,168 | 6,440 | 3,979 | 5,376 | (mid pools) |
| `0x2f4`-only explicit if/else | 7,116 | 5,811 | 2,239 | 5,386 | (mid pools) |

Key results:
* The explicit if/else for `0x2c4`/`0x2c8`/`0x2cc`/`0x2d0` (variant B) leaves the
  pool multiset **exactly unchanged** (190 words, same values) — the
  `movs`-immediate materialisation is identical.
* The explicit if/else for `0x2f4..0x318` (C) is the only group that **grows**
  the pool: 190 -> 202 words (+12).  It also drops the code to 6,180 B (8 B
  *below* the original's 6,188 B), i.e. the code shape is nearly right, but the
  function shrinks to 6,988 B.
* The explicit if/else for `0x7c..0x9c` (D) **shrinks** the pool again
  (202 -> 203 with 48 B more code; net the code grows 44 B and the pool drops
  by ~1 word relative to C's ratio).
* Every size-exact variant found (`D + swap(0xa8,0xb0)`, `baseline +
  swap(0xa8,0xb0)`) regresses raw and aligned badly (the `0xa8/0xb0` swap
  breaks the SLP pairing of the `0xa0..0xa8` group).

## 7. Conclusion / next lever

The three groups are **not** independently worth 44 words.  Only `0x2f4..0x318`
responds to the device-major rewrite (+12 words); `0x2c4` is pool-neutral and
`0x7c` is pool-negative.  The remaining ~19 words are in the **second copy of
the mid-body `0xb0..0x128` group** (original `d1ac6..d1bc6`, 10 `vld1.64`, the
`fp == 0` path) and the second `0x1b0/0x1c0/0x1d0` copy (`d21aa..d21de`).
Next pass: reconstruct the mid-body group as a device-major `if/else` on the
flag that selects `d1ac6` (the flag loaded at `d1272`, the else of iPadHD) so
clang emits the full second copy, then re-tune the tail to pay back the code
growth.  Candidate C (6,988 B / pool 202) is the best pool-growth anchor and is
saved at `/dev/shm/w9708/Layout.C.cpp`.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9708
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9708/fdiff.py _ZN6LayoutC1Ev        # 7072 / 5741
python3 /dev/shm/w9708/align_diff.py _ZN6LayoutC1Ev   # 2189
python3 /dev/shm/w9708/pool2.py cmake-build-match/libgof2hdaa.so
python3 /dev/shm/w9708/storeseq.py /dev/shm/w9708/ours.dis
```

Scratch `/dev/shm/w9708/`: `mk.py` (0x2c4), `mkC.py` (0x2f4), `mkD.py` (0x7c),
`mkE.py` (0x1b0), `search.py`, `eval.sh`, `fdiff.py`, `align_diff.py`,
`pool.py`, `pool2.py`, `storeseq.py`, `vgroup.py`, `vecmap.py`, `maxref.py`,
`Layout.base.cpp`, `Layout.hit.cpp`, `ostore.txt`, `ustore.txt`, `vgroup.txt`,
`opool.txt`, `upool.txt`, `upoolB.txt`, `upoolC.txt`, `orig.dis`, `ours.dis`.
