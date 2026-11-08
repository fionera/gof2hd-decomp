# `Layout::Layout` C1/C2 — wave 152 / worker w9810
## The w9709 `0x2f4..0x318` ascending reorder lands at exactly 7,072 B with a
## +4 B companion: swapping the adjacent `field_0x1a8` / `field_0x198`
## statements.  The swap makes clang emit the original's `strd @198/@19c` +
## `str @1a0` shape and pays the missing 4-byte instruction.  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-151 HEAD `9d491d46a`: raw same-address diff **5,741**,
aligned instruction diff **2,189**, size 7,072, `.text` 0x1337c8 (exact),
file size exact.  Gate on entry/exit: byte_exact 4507/4524, linked_exact
4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
`RATCHET PASS`.

## 1. Reproduce w9709: ascending `0x2f4..0x318` is -4 B

Source order is `0x2f8, 0x2f4, 0x300, 0x2fc, 0x308, 0x304, 0x310, 0x30c,
0x314, 0x318`.  Moving `0x2f4` before `0x2f8`, `0x2fc` before `0x300` and
`0x304` before `0x308` (three statement moves, no expression change) gives:

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| HEAD baseline | **7,072** | **5,741** | **2,189** |
| ascending `0x2f4`/`0x2fc`/`0x304` | 7,068 (-4) | 5,801 | **2,139** |
| ascending + `0x30c` before `0x310` | 7,076 (+4) | 5,798 | 2,141 |

The fast path now emits `strd @2f4/@2f8`, `strd @2fc/@300`, `str.w @304`
before `str.w @308`, exactly the original's ascending store order
(`0x1642 strd @2f4/@2f8`, `0x164a strd @2fc/@300`, `0x164e str @304`,
`0x16de str @308`).

## 2. The +4 B companion: `field_0x1a8` <-> `field_0x198`

Swapping the two adjacent statements `this->field_0x1a8 = ...` and
`this->field_0x198 = ...` (source order `0x19c, 0x1a0, 0x1a8, 0x198, 0x1ac`
becomes `0x19c, 0x1a0, 0x198, 0x1a8, 0x1ac`) adds exactly one 4-byte
instruction and lands the function at 7,072 B:

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| ascending only | 7,068 | 5,801 | 2,139 |
| **ascending + 1a8/198 swap** | **7,072** | **5,800** | **2,148** |

The swap is not arbitrary: it changes the SLP pairing from `strd @19c/@1a0`
to `strd @198/@19c` + `str @1a0`, which is the original's store shape
(original `0x0eca`, `0x1032`, `0x114c` all `strd @198/@19c`, followed by
`str @1a0`).  The +4 comes from an extra `mov.w r6, #12` that clang emits in
the `0x198` branch.  Both statement moves are disjoint, so the two edit
orders (ascending-then-swap / swap-then-ascending) produce byte-identical
source; the combined raw diff is **5,800** either way.

## 3. Store tables (function-relative offsets)

`0x2f4..0x318` (final == original shape):

```
ORIG   1642 strd @2f4/@2f8  164a strd @2fc/@300  164e str @304
       16a8 str @2f4        16bc vstr @2f8        16d0 stmia @2fc(3)
       16de str @308        16ee strd @30c/@310  16f2 str @314
       170a strd @30c/@310  1728 strd @30c/@310  173e strd @30c/@310
       1742 str @314        1750 str @318
FINAL  170c strd @2f4/@2f8  1714 strd @2fc/@300  1718 str @304
       1772 str @2f4        1786 vstr @2f8        17a6 str @308
       17d4 strd @30c/@310  17e6 strd @30c/@310  17fa strd @30c/@310
       1808 str @314        181a str @318
```
(The slow-path `stmia @2fc(3)` is present in both; the store-table
extractor misses the `add.w r6,r0,#764; stmia r6!,{r1,r2,r3}` spelling.)

`0x198..0x1ac` (the companion's target):

```
ORIG   0eca strd @198/@19c  0ece str @1a0  1032 strd @198/@19c
       1036 str @1a0        1058 str @1a4   1064 str @1a4
       1072 vstr @1a8       1086 str @1ac   114c strd @198/@19c
       1166 str @1ac        1176 str @1ac
FINAL  10d6 strd @198/@19c  10da str @1a0  10f8 strd @198/@19c
       1104 str @1a0        1108 str @1a8   113a strd @198/@19c
       113e str @1a0        115c vstr @1a8  117c str @1ac
       123e str @1ac        18a0 str @1ac
```
Baseline (before the swap) paired `strd @19c/@1a0` and stored `@198` late;
the swap fixes that.

## 4. Pool / code split

| build | size | code end | code | pool start | pool words | pool bytes |
|---|---:|---:|---:|---:|---:|---:|
| ORIGINAL | 7,072 | `0x182c` | 6,188 | `0x182c` | **221** | **884** |
| baseline | 7,072 | `0x18a8` | 6,312 | `0x18a8` | **190** | **760** |
| **final** | **7,072** | `0x18a8` | **6,312** | `0x18a8` | **190** | **760** |

The final variant keeps the baseline code/pool budget (the ascending reorder
moves bytes inside the tail; the companion pays back the 4 B).  The
remaining 124 B gap to the original is still the device-major duplication
(w9710): the original spends 124 B less code and 124 B more pool.
The original's final pool starts `3ea66666 3f266666` (0.325f / 0.65f); ours
keeps those two words in the mid-function pool at `0x8fc`.

## 5. Re-test: `0x13c..0x148` SLP group move on top

Moving `0x13c, 0x140, 0x144, 0x148` to immediately after `0x16c` (w9609's
group) does **not** compose with the ascending reorder:

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| ascending only | 7,068 | 5,801 | **2,139** |
| ascending + group | 7,068 | 5,890 | 2,264 |
| ascending + group + 1a8/198 swap | 7,068 | 5,863 | 2,274 |

The group cancels the companion's +4 (back to 7,068) and regresses both
metrics, so it is not landed.

## 6. Search evidence

* All 16 subsets of the four `0x2f4..0x318` adjacent swaps: only the
  three-swap ascending set is 7,068; the full set is 7,076; no subset is
  7,072.
* All 24 permutations of the `0x30c/0x310/0x314/0x318` group on top of the
  ascending reorder: sizes 7,052..7,100, none 7,072.
* All 720 permutations of the `0x2f4..0x308` six statements: many 7,072
  permutations, best aligned 2,187 (no better than baseline).
* Adjacent / distance<=4 swaps over all 229 constructor statements on top
  of the ascending reorder: 37 size-7,072 candidates; the `1a8/198` swap is
  the best aligned (2,148).
* All pairwise swaps of the 37 statements in lines 170..215: 12 size-7,072
  candidates; `1a8/198` is again the best (2,148; next is 2,206).
* `__builtin_expect(...,0)` on each of the nine `if (Globals::iPadHD)`
  guards: no size or alignment change.

## 7. Landed source change

Four statement moves, no expression edits:

* `field_0x2f4` before `field_0x2f8`
* `field_0x2fc` before `field_0x300`
* `field_0x304` before `choiceWindowTitleHeight` (`0x308`)
* `field_0x198` before `field_0x1a8`

## 8. Gate

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

Function size 7,072, `.text` size 0x1337c8 exact, `.rodata`/`.data`/`.bss`
sizes exact, file size 2,187,328 exact.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9810
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9810/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5800
python3 /dev/shm/w9810/align_diff.py _ZN6LayoutC1Ev   # 2148
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w9810/`: `mk.py` (statement moves), `measure.sh`,
`fdiff.py`, `align_diff.py`, `storeseq.py` (SLP-aware store table),
`pool.py` (literal-pool dump), `subsets.py`, `permgroup.py`, `search2.py`,
`search3.py`, `tryexpect.py`, `Layout.base.cpp`, `Layout.final.cpp`,
`orig.store`, `final.store`, `orig.pool`, `asc.pool`.

## 10. Open questions

* The remaining 124 B code/pool gap is the full device-major rewrite
  (w9710): reproduce the per-arm GOT reloads so the final pool grows from
  190 to 221 words while code shrinks from 6,312 to 6,188 B.
* The original's `0x2f4..0x318` block is device-major; ours is a flat
  ternary chain that happens to schedule into the same ascending store
  order.  A faithful device-major source is still the long-term shape.
