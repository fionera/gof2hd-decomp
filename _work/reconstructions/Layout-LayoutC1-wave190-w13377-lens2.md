# `Layout::Layout` C1/C2 — wave 190 / w13377, second lens

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original VMA
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13377`, base master `1904beaf7`.

**Result: no size-exact raw win found; `src/` is left at the base.**

| metric | base |
|---|---:|
| raw same-address bytes | **5,256** |
| aligned (difflib) | **2,726** |
| size | **7,072/7,072** |
| instructions | **2,520/2,552** |
| whole-`.text` diff | 73,505 |
| `.ARM.extab` diff | 610 (size/base exact `0x1a3b78`, 156,312 B) |

Gate after the pass: `RATCHET PASS` — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.

## 1. Real-TU order searches (new)

The wave-189 note used a minimal-TU proxy for the large searches.  This pass
re-ran the searches with the **real `Layout.cpp` TU** (clang 7.0.2, exact
checked-in command; ~0.17 s/compile, 4 workers, `ulimit -v 12000000`):

* all 2-opt swaps within a 20-unit window (4,310 candidates): best **5,241**
  proxy (base proxy 5,241/5,242), i.e. neutral.  The two 5,241 candidates
  (`5↔6`, `5↔9`) were built and linked: both give real **raw 5,256**,
  size 7,072 — no win.
* 2,991 random block moves of length 1–4: best **5,242** proxy (neutral);
  next 5,243/5,245.  No candidate beats the base.
* exhaustive permutations of the first-block groups `{4,5,6,7}` (24) and
  `{0,1,2,3,4}` (120): base order is best in both sets.
* exhaustive permutation of the `0x34..0x44` SLP-chain units
  `{8,9,10,11,12,18}` (720): base order is best (5,242); next 5,246.

So the current order is a strong local optimum under the real TU, not just the
minimal proxy.

## 2. Literal-pool placement is the dominant residual (new)

The original end pool starts at **`0xd2854`**:

```
0xd2854  3ea66666 3f266666 42c80000 42480000   ; 0.325f, 0.65f, 100.0f, 50.0f
0xd2864  0013df2e 0013de3c 0013ddb4 0013dd96   ; addresses
0xd2874  438ca000 43c80000 0013dc9a 43480000   ; 281.25f, 400.0f, addr, 200.0f
...
```

Our end pool starts at **`0xd2908`** with integer constants; the four floats
are in a local island at **`0xd1918`**:

```
ours 0xd1918  00009642 0000c842 00004842 6666a63e  ; 75.0f, 100.0f, 50.0f, 0.325f
ours 0xd1924  6666263f 66666642                    ; 0.65f, 57.6f
ours 0xd2908  0b000000 0a000000 0e010000 16000000  ; integers
```

The original's `field_0x90` load is `addw r1, pc, #0xf08` from the end pool
(`0xd1948`); ours is `subw r1, pc, #0x58` from the island (`0xd197a`).  The
mid block is correspondingly ~0xc4 larger in ours and the tail ~0xc4 smaller,
which shifts a large run of instructions.  This is a constant-island pass
decision, not a wrong value or store order.

## 3. iOS source order and shape tests (new)

The true original source order is the iOS 1.1.4 order recorded in
`layout_ctor/ios_source_order.md`.  Re-applying it to the current tree:

* all simple units sorted by iOS order: size **6,788**, raw **6,045**;
* every unit (including `if`/`else` blocks, keyed by first offset) sorted by
  iOS order: size **6,880**, raw **5,945**;
* hybrid head/tail splits at k = 100/120/137/150/170: all size-fatal or
  raw ≥ 5,683.

The iOS order is structurally correct but shifts the code far enough that the
raw same-address metric gets worse; it does not by itself move the four floats
into the end pool.

Targeted expression shapes also lose: int-typed `field_0x20_top` (7,000/6,017),
int-typed `field_0x9c` (7,048/5,448), `const float` locals for 0.325/0.65 or
100/50 (5,246 proxy), removing the `volatile rd` reload (7,064/5,395),
removing the `t_field_0x2c8` or `v` temporaries (7,112/5,548, 7,064/5,382).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13377
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5256 aligned=2726 size=7072/7072 insn=2520/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13377/`
(`unit_score.py`, `sweep2real.py`, `rand_blocks.py`, `perm_block.py`,
`ios_order.py`, `ios_order_all.py`, `hybrid.py`, `sort_tail.py`,
`stores.py`, `decomp_layout.c`).
