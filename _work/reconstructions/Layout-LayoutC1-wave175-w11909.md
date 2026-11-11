# `Layout::Layout` C1/C2 — wave 175 / worker w11909

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (branch `wave175-w11909`, base master `5f599a137`): raw **5,633**,
aligned **2,290**, 7,072 B, 2,519 / 2,552 instructions.

**Exit: no code change landed.**  The faithful per-branch tail dispatch was
reconstructed and made size-exact (7,072 B), but its raw is **5,931** (aligned
2,700), i.e. worse than the baseline.  A 128-state feature search over the
duplicated-store shapes found no size-7,072 state with raw < 5,633.  The tree
is left at the base.

## 1. What the binary says about the missing duplication

A store-map parse of the checked-in original (`objdump -M force-thumb`,
`d1028..d2bc8`) confirms the earlier wave-173/174 conclusions:

* the store set is complete (201 distinct offsets), but the original has
  **more stores per field** because the tail is a real `if/else` dispatch,
  not a flat ternary cascade;
* the original has **8 `stmia` groups** (`d113e`, `d11c0`, `d12c0`, `d15e2`,
  `d1c5c`, `d1c88`, `d1ebc`, `d26f8`); the baseline has 3 (`0x7c..0x84`,
  `0x1f0..0x1f8`, `0x2fc..0x304`);
* the original tail (`d21e2..d2548`) is:

```cpp
field_0x1e4 = iPadHD ? 5 : (n9 ? 6 : (retina ? 8 : 4));
field_0x1e8 = retina ? 1 : (iPadHD ? 1 : (n9 ? 1 : 0));
if (iPadHD) {
    field_0x1fc = 2;  field_0x1ec = 26;
    field_0x234 = 1 * 1.40625f;  field_0x238_blk16.a = 50 * 1.40625f;
    field_0x27c = 20 * 1.40625f; field_0x280 = 0;
    scrollBarInset = 1 * 1.40625f; scrollBarHandle = 0.0f;
    rewardBoxWidth = 100 * 1.40625f; rewardBoxY = 55 * 1.40625f;
    rewardBoxY2 = 132;
    /* 0x204..0x278 ascending, 7 groups of 4 words */
    rewardBoxHeight = n9 ? 352 : (retina ? 392 : 196);
    rewardBoxX      = n9 ? 75  : (retina ? 100 : 50);
} else {
    if (n9) {
        field_0x1fc = 4; field_0x1ec = 34;
        field_0x234 = 2; field_0x238_blk16.a = 90; field_0x27c = 30;
        /* 0x204..0x278 ascending constants */
    } else {
        field_0x1fc = retina ? 4 : 2;  /* .. same 0x1ec..0x278, retina selects */
    }
    field_0x280      = n9 ? 4 : (retina ? 5 : 0);
    scrollBarHandle  = n9 ? 1 : retina;
    scrollBarInset   = n9 ? 2.0f : (retina ? 2.0f : 1.0f);   /* float expr */
    if (n9)            { rewardBoxWidth = 112; rewardBoxY = 57;  rewardBoxY2 = 111; }
    else if (iPadLarge){ rewardBoxWidth = 200; rewardBoxY = 110; rewardBoxY2 = (Y+17)<<retina; }
    else if (iPad)     { rewardBoxWidth = 100; rewardBoxY = 55;  rewardBoxY2 = (Y+17)<<retina; }
    else               { rewardBoxWidth = retina ? 150 : 75;
                         rewardBoxY     = retina ? 76  : 38;
                         rewardBoxY2    = (Y+17)<<retina; }
    rewardBoxHeight = retina ? 392 : 196;
    rewardBoxX      = retina ? 100 : 50;
}
```

The original also stores `field_0x2bc`, `field_0x200` and `field_0x285` in
the T block (`d25f8`, `d2608`, `d2620`), not in the first half / tail.

## 2. Size-exact faithful-tail variant (raw 5,931)

Replacing source units `0x1e4..rewardBoxX` with the dispatch above, and
moving `0x2bc`, `0x200`, `0x285` into the T block immediately before
`field_0x1f4`, gives exactly **7,072 B**:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | **5,633** | **2,290** | **7,072** |
| faithful tail, T units kept | 5,984 | 2,709 | 7,056 |
| faithful tail + `0x2bc/0x200/0x285` in T | **5,931** | **2,700** | **7,072** |
| faithful tail + ascending `0x1b0..0x1dc` | 5,993 | 4,006 | 7,060 |
| faithful tail + faithful T order | 6,517 | 3,956 | 7,340 |

The first half is still **+122 B** wider than the original (`d225c` vs
`d21e2` tail start), so the tail is shifted and the raw metric stays high.
The original's first half is 4,538 B; ours is 4,660 B.  The literal pool is
correspondingly smaller (ours ~776 B vs. the original's 966 B), exactly the
trade the task describes: the original pays for duplicated branch stores with
a larger pool and a smaller code body.

## 3. Duplicated-store feature search

Seven independent source features were toggled exhaustively (128 states),
all measured with a real `link_so` + `tools/fn_diff.py`:

* **A** `0x94` as a 3-way `if/else` in the first half,
* **B** `0x140` as a 4-way `if/else` in the first half,
* **C** `rewardBoxWidth` as a 5-way `if/else`,
* **D** `rewardBoxY` as a 5-way `if/else`,
* **E** `0x2cc` as a 5-way `if/else`,
* **F** `0x2d0` after `0x2cc` (restores the `vcvt` chain),
* **G** `0x2d0` triple `(int)(float)(int)` cast.

Only eight states hit 7,072 B:

| features | raw | aligned |
|---|---:|---:|
| (base) | **5,633** | **2,290** |
| F | 5,637 | 2,285 |
| BCDE | 5,809 | 2,627 |
| BCD | 5,820 | 2,598 |
| BCDFG | 5,853 | 2,609 |
| ACDEG | 5,931 | 2,613 |
| ACDFG | 5,980 | 2,487 |
| ABCDEFG | 6,032 | 2,913 |

No size-7,072 state beats the baseline raw.  The closest aligned win is
`F` (2,290 -> 2,285) but it costs +4 raw; it is the same wave-173 shape
(`0x2d0` after `0x2cc`) and was not landed.

Other measured shapes (not size-exact):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `0x94` moved before `0x98` | 5,948 | 2,261 | 7,016 |
| `0x94` 3-way + `rewardBoxY` 5-way | 5,966 | 2,244 | 7,024 |
| `0x2cc` 5-way + `0x2d0` after | 5,642 | 2,277 | 7,084 |
| `0x2cc` 5-way | 5,803 | 2,303 | 7,056 |
| `rewardBoxY` 5-way | 5,719 | 2,285 | 7,080 |
| `rewardBoxWidth` 5-way | 5,967 | 2,498 | 7,024 |
| ascending `0x1b0..0x1dc` | 6,010 | 3,946 | 6,996 |
| ascending `0x118..0x12c` | 5,759 | 2,336 | 7,056 |
| `0x2b0/0x2b4/0x2b8` adjacent | 5,933 | 2,713 | 6,976 |
| full first-half ascending sort | 6,170 | 2,987 | 6,752 |

## 4. Conclusion

The baseline's scrambled store order is a strong local optimum for the raw
metric.  The original's extra `stmia` groups and duplicated branch stores are
real, but reproducing them shrinks the code body by 16-320 B; every
size-7,072 compensation found so far costs more raw than it saves.  The
remaining residue is first-half codegen (register allocation / wide Thumb
encodings) and the 190 B literal-pool deficit, not missing stores.  Landing a
raw win requires reconstructing the first half's source order together with
the tail, not the tail alone.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11909
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11909/`: `Layout.base.cpp`
(pristine ctor), `newtail.txt` (faithful tail), `harness.py`/`harness2.py`
(build + `fn_diff` + restore), `search.py` (128-state feature search),
`stores.py`/`branches.py` (store/branch maps), `orig.dis`/`ours*.dis`
(disassembly).

Gate at exit (base tree): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data_exact **843/843**, imports
**207**, lint **CLEAN**, drift **0**, parity **5370/5370**, `.rel.plt`
byte-identical.  `Layout` C1/C2 raw **5,633**, aligned **2,290**, size
**7,072/7,072**.
