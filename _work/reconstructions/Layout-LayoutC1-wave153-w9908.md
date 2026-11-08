# `Layout::Layout` C1/C2 — wave 153 / worker w9908
## The "9 missing `vld1.64` vectors" are NOT extra constant vectors: the
## multiset of distinct 16-byte vector contents is **identical** (34 each).
## They are *extra reloads of the same constants*, caused by the SLP pass
## grouping our stores at `0xb4/0xc4` and `0x148/0x158/0x168` where the
## original groups at the 16-byte-aligned `0xb0/0xc0` and `0x140..0x170`.
## Source unchanged (baseline 7,072 B / raw 5,800 / aligned 2,148 kept).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-152 HEAD `4a6104a43`: raw same-address diff **5,800**,
aligned instruction diff **2,148**, size 7,072.  Gate on entry and exit:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, `RATCHET PASS`.

## 1. Method / tools

`/dev/shm/w9908/`: `vecs.py` (tab-aware objdump parser that resolves every
`vld1.64 {dN-dN+1}, [rX]` back through `add/addw/subw rX, pc, #imm` and
`ldr rX, [pc,#imm]` to the 16-byte vector it loads; **the earlier regex
parsers silently consumed `add`/`adc` as hex bytes** — `add` is 3 hex chars),
`vdiff.py` (vector-content multiset diff), `stores.py` (store-offset table),
`pool.py` (final literal-pool word dump), `fdiff.py`, `align_diff.py`.

Original code/data split: code `d1028..d2854` (6,188 B), final pool
`d2854..d2bc8` (221 words).  Baseline: code `d1028..d28d0` (6,312 B), pool
`d28d0..d2bc8` (190 words).  Both end at `d2bc8`; the baseline trades
**+124 B code for −31 pool words**.

## 2. Store table — first (iPadHD) arm, original vs baseline

Both arms have the same number of `vst1.32` groups (27), but the *offsets*
differ.  `vst1.32 {dN-dN+1}` stores 4 consecutive int fields.

```
ORIGINAL (27 groups)                         BASELINE (27 groups)
0x14  [1e,0c,1c,07]                          0x14  [1e,0c,1c,07]
0x4   [15,2a,28,34]                          0x4   [15,2a,28,34]
0x24  [07,07,04,2a]                          0x34  [08,09,52,8c]   <-- swapped
0x34  [08,09,52,8c]                          0x24  [07,07,04,2a]   <-- swapped
0x44  [07,0f,07,7d]                          0x44  [07,0f,07,7d]
0x54  [e7,01,55,01,3f,5a]                    0x54  [e7,01,55,01,3f,5a]
0x64  [70,236,bf,3f]                         0x64  [70,236,bf,3f]
0x2c8 [3f,7e,3f,70]                          0x2c8 [3f,7e,3f,70]
0x7c  [0c,0e,70,2a]                          0x7c  [0c,0e,70,2a]
0xc0  [09,0b,05,fd]    <-- 16B aligned        0xb4  [14,19,0b,09]   <-- +4
0xb0  [04,14,19,0b]    <-- 16B aligned        0xc4  [0b,05,fd,15]   <-- +4
0xec  [38,70,1901,3202]                      0xdc  [float,float,float,float]
0x294 [1901,76,10,10]                        0x294 [1901,76,10,10]
0x2a4 [9a,8c,93,2a]                          0x2a4 [9a,8c,93,2a]
0x108 [fe,01,02,01]                          0x118 [09,23,15,01]   <-- swapped
0x118 [09,23,15,01]                          0x108 [fe,01,02,01]   <-- swapped
0x128 [01,15,4e,97]                          0x128 [01,15,4e,97]
0x140 [0f,12,02,bd]    <-- 16B aligned        0x148 [02,bd,05,46]   <-- +8
0x150 [05,46,46,ce]                          0x158 [46,ce,44,00]   <-- +8
0x160 [44,00,23,b8]                          0x168 [23,b8,ae,54]   <-- +8
0x170 [ae,54,54,d9]                          0x180 [17,a3,17,f4]   <-- +0x10
0x180 [17,a3,17,f4]                          0x190 [fa,08,10,5a]
0x190 [fa,08,10,5a]                          0x1a0 [12,16,16,f3]
0x1a0 [12,16,16,f3]                          0x1b0 [12,2a,2e,01]   <-- different
0x1b0 [reg]                                  0x1c0 [5c,1a,28,32]
0x1c0 [5c,1a,28,32]                          0x1d0 [07,11,05,0e]
0x1d0 [07,11,05,0e]
```

The original's groups are **16-byte aligned** (`0xb0`, `0xc0`, `0x140`,
`0x150`, `0x160`, `0x170`, ...).  Ours are offset by 4 or 8 bytes because the
first store of each run is scalarised (e.g. `field_0xb0` is emitted as
`movs r3,#4; str.w r3,[r0,#176]` at `d1194` — the float-valued ternary
`3 * 1.40625f` is *not* folded into the int SLP run).  The tail `0x140`
group is missing entirely because `field_0x140`/`field_0x144` are assigned
near the end of the source (lines 284/295), not in the `0x13c..0x15c` run.

## 3. Vector-content multiset diff (the decisive evidence)

Resolving every `vld1.64` to its 16-byte data: **the original and the
baseline reference exactly the same 34 distinct vector contents** (85 vs 76
loads).  The diff is a pure *rotation* of the same words:

```
ORIG-only                                    BASELINE-only
04000000 14000000 19000000 0b000000  <->   14000000 19000000 0b000000 09000000
09000000 0b000000 05000000 fd000000  <->   0b000000 05000000 fd000000 15000000
0f000000 12000000 02000000 bd000000  <->   02000000 bd000000 05000000 46000000
05000000 46000000 46000000 ce000000  <->   46000000 ce000000 44000000 00000000
44000000 00000000 23000000 b8000000  <->   23000000 b8000000 ae000000 54000000
ae000000 54000000 54000000 d9000000  <->   (d9 appears as the next word)
...
```

i.e. ours is the original's word stream shifted by one word.  There are **no
extra distinct constant vectors** in the original.  The 9 extra `vld1.64` are
re-loads needed because the run boundaries differ (each misaligned run costs
one extra load/store pair and one extra `adr`).

## 4. Final-pool table (word multiset)

Original 221 words, baseline 190 words.  ORIG-only words are dominated by
small integers (the SLP vector constants that the original places in the
final pool, loaded via `addw rX, pc, #imm` into `d29xx`):

```
x2 1  x2 2  3  x3 4  5  x2 6  x3 8  x3 a  b  c  x2 d  e  x2 f  x2 10  11  12
x3 13  14  15  1a  1d  1e  22  24  28  x2 50  64  69  6e  78  a0  b4  c8  d2
dc  f3  12c  15a  168  190  1be  258  320
0013db84 0013dc04 0013dc9a 0013dd96 0013ddb4 0013de3c 0013df2e
3ea66666 3f266666 ffffffff
```

The seven `0013dxxx` words are the per-arm GOT reloads (`iPad`/`retina`/`n9`/
`iPadLarge`), matching w9809.  The 31-word gap is **not** 51 words on the
current baseline (that figure was ASC2's).

## 5. Experiments (real `link_so` + fdiff + align_diff)

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| HEAD baseline | **7,072** | **5,800** | **2,148** | landed |
| full ascending (all 206 stmts sorted by offset) | 6,908 | 6,026 | 3,919 | −164 B |
| 2-arm `if(iPadHD){consts}else{ternaries}` over 0x4..0x1e0 | 6,660 | 5,812 | 3,740 | −412 B |
| `0xb0..0xcc` reordered ascending | 7,040 | **5,768** | 2,235 | best raw, −32 B |
| + `0x140/0x144` moved into the `0x13c..0x15c` run | 6,976 | 5,868 | 2,653 | |
| + `0x160..0x1ac` ascending | 7,112 | 5,992 | 2,306 | +40 B |
| `0x170..0x184` ascending | 7,068 | 5,870 | — | −4 B |
| `0x94..0xa4` ascending | 7,092 | 6,007 | — | +20 B |
| `0x7c..0x94` ascending | 7,060 | 5,830 | — | −12 B |
| `0xdc..0xf8` ascending | 7,016 | 5,903 | — | −56 B |
| `0x108..0x12c` ascending | 7,036 | 5,808 | — | −36 B |
| `0x13c..0x15c` ascending | 7,004 | 5,888 | — | −68 B |

Key negatives:
* **The function size is a hard gate.**  `byte_exact` compares raw bytes at
  the *same address*; a 32-byte `.text` shrink moves every following function
  and drops `byte_exact` 4507 → 2587 (measured on the `0xb0` variant).
  `linked_exact` stays 4507.  So any variant must land at exactly 7,072 B.
* Full ascending sort *shrinks* the pool (197 words) and the vector count is
  88 — already more than the original's 86.  So "ascending" alone is not the
  missing structure.
* Making `field_0xb0` a plain `4` (instead of `3 * 1.40625f`) changes the
  ternary type to int and *grows* the function to 7,096 with raw 6,133 —
  the float-typed ternary is what the original's codegen matches.

## 6. Conclusion / next lever

The mission premise ("the 51 missing pool words are 9 extra `vld1.64`
constant vectors") is **falsified**: the distinct-vector multiset is
identical.  The real defect is SLP *run alignment*: the original groups every
four consecutive int stores at a 16-byte-aligned field offset, while ours
scalarises the first store of several runs (`0xb0`, `0x140`, `0x170`) and
therefore groups at `+4`/`+8`.

Next pass: force the runs to start at the aligned offsets by making the
*first* store of each run an int-typed constant (e.g. `(int)(3 * 1.40625f)`
only where the original vectorises it) and by moving `0x140`/`0x144` into the
`0x13c..0x15c` run **without** removing their late stores (the w9809 ASC2
move removed them and lost 120 B).  A +32 B companion is still needed to keep
`.text` exact.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9908
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9908/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5800
python3 /dev/shm/w9908/align_diff.py _ZN6LayoutC1Ev   # 2148
python3 /dev/shm/w9908/vecs.py ... d1028 7072        # per-vector contents
python3 /dev/shm/w9908/vdiff.py                       # vector multiset diff
python3 /dev/shm/w9908/stores.py ... d1028 7072       # store table
```

Scratch `/dev/shm/w9908/`: `vecs.py`, `vdiff.py`, `stores.py`, `pool.py`,
`fdiff.py`, `align_diff.py`, `ovec.txt`, `uvec.txt`, `ost.txt`, `ust.txt`,
`opool.txt`, `upool.txt`, `Layout.b0.cpp` (best-raw variant, 7,040 B).
