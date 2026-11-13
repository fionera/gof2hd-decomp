# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13691

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13691`, base master `b3fd7e6a2`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13650.md` (the semantic alienGuns
interleave, raw 2,626 -> 2,636), `...w13655.md`, `...w13671.md`, `...w13685.md`
and the w13680 sweep.  Task: recover the +10 raw cost of the interleave while
keeping `aligned <= 841`.  **No such shape was found; note-only, `src/`
unchanged.**

```text
                                     size            raw    aligned  insn
base b3fd7e6a2 (w13650 landed)       17172/17172     2636    841     6206/6197
semantic base (both rots first)      17172/17172     2626    845     6206/6197
best raw found (cargo c2)            17172/17172     2635   1022     6206/6197
best aligned at raw < 2636           17172/17172     2635   1022     6206/6197
```

All size-exact candidates below have `.ARM.extab` diff **577** (size/base
exact at `0x1a3b78`), `.ARM.exidx` 0, `.rodata` 0.  `check` on the unchanged
tree: byte_exact 4508/4524, linked_exact 4508/4524, size 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. The +10 is the 8-byte `setRotation` block shift

The w13650 interleave inserts the 8-byte `setRotation` block between the two
`MatrixRotateVector` calls.  The binary's block sits at `0xb3308`; ours at
`0xb330e`.  The second rotation therefore starts at `0xb3312` (ours) vs
`0xb330c` (binary), and the whole post-rotation code is +8.  The semantic
base (both rotations first, `setRotation` after) reproduces the binary's
`0xb332c vldr s0,[sp,#372]` exactly, which is where its raw 2,626 comes from.

The code before the second rotation is:

| span element | binary | ours (crutch) | delta |
|---|---|---:|---:|
| `t` select | 14 B (`cbz` diamond) | 22 B (`it ne`/`movne` cmov) | **+8** |
| `&v` / `&rotation` materialisation | 2+2 B (low `r5`/`r4`) | 4+4 B (high `r9`/`fp`) | **+4** |
| `Vector{...}` field loads | 24 B (5 separate `ldr`) | 20 B (`ldrd`/`strd`) | **-4** |
| `&v` for `Vector::operator=` | 2 B (`r4`) | 4 B (`fp`) | **+2** |
| **net before rotate2** | | | **+10** |

The `(bigAlienWave + (i < 0) + (i < 0))` crutch is size-load-bearing: the
plain ternary is hoisted by LICM and gives 17,168.  The crutch forces the
select into the loop but changes it from a branch to a cmov and pushes
`gun`/`&v`/`&rotation` into high registers.  Every shape that removes the
crutch changes the function size or the whole-function colouring.

## 2. Measured variants

Baseline on top of w13650 is `2636 / 841 / 17,172`.  `C` = cargo,
`P` = setPosition, `S` = setScaling, `G` = shipGroup.

### 2a. `cargo` placement × tail order × `gun` placement

Full 2×4×6 sweep (gun before/after store; cargo after setRotation / after
rotate2 / after setScaling / after shipGroup; all PSG orders).  Best rows:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **cargo after rotate2, gun before (checked in)** | **2,636** | **841** | 17,172 |
| cargo after setRotation, gun before (`c2`) | **2,635** | 1,022 | 17,172 |
| cargo after setRotation, gun after | 2,636 | 1,018 | 17,172 |
| cargo after rotate2, gun after | 2,637 | **837** | 17,172 |
| cargo after setScaling, gun before | 2,642 | 1,022 | 17,172 |
| cargo after setScaling, gun after | 2,643 | 1,018 | 17,172 |
| cargo after shipGroup, gun before | 2,644 | 1,022 | 17,172 |
| cargo after shipGroup, gun after | 2,645 | 1,018 | 17,172 |

The `c2` -1 is the only raw improvement in the whole space; it is the shape
independently landed by w13685 (`695502470`).  It cannot be combined with the
gun-after aligned win (2,636/1,018), and every raw-2,635 row is aligned 1,022.

### 2b. `t`-select spelling

| expression | raw | aligned | size |
|---|---:|---:|---:|
| `(bigAlienWave + (i<0) + (i<0))` (checked in) | **2,636** | **841** | 17,172 |
| `(bigAlienWave + (i<0))` (two-term) | 4,074 | 892 | 17,168 |
| `(bigAlienWave - (i<0))` / `^ (i<0)` | 3,871 | 865 | 17,168 |
| `(bigAlienWave + (i<0)+(i<0)+(i<0))` | 2,946 | 843 | 17,172 |
| `(bigAlienWave - (i<0)*4)` | 2,955 | **836** | 17,172 |
| plain `bigAlienWave ? ... : ...` | 4,371 | 874 | 17,168 |
| `if/else`, `switch`, pointer-array, `__builtin_expect` | 4,371 | 874 | 17,168 |
| pointer crutch `(...) + (i<0)` / `+ 2*(i<0)` | 4,371 | 915 | 17,164 |
| pointer crutch `- (i<0) - (i<0)` | 4,306 | 907 | 17,188 |
| `*(volatile bool *)&bigAlienWave` | 16,697 | 4,222 | 17,288 |

The two-term crutch is the interesting one: it produces an 18-byte in-loop
select (`cmp.w r1,r2,lsr#31; it ne; movne`) and is 4 B short (17,168).  It
only lands at 17,172 when combined with gun-after + cargo-after-setScaling
(`2,670 / 835 / 17,172`) or cargo-after-shipGroup (`2,673 / 832 / 17,172`),
both raw-worse than the checked-in shape.  The aligned metric drops below
841 there, but raw rises by 34-37.

### 2c. temporary lifetimes / vector construction

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `Vector v` outside loop (checked in) | **2,636** | **841** | 17,172 |
| `Vector v` / `Vector *pv` / `Vector v[1]` / `Vector &vr` aliases | 2,636 | 841 | 17,172 |
| `Vector a = {t[k][3],...}` for one vector | 4,384 | 1,056 | 17,196 |
| named `Vector` for the second vector only | 4,486 | 909 | 17,176 |
| `const float *row = t[k]` | 4,031 | 941 | 17,168 |
| `const float *row` + two-term crutch | 4,074 | 892 | 17,168 |
| `float f0..f5 = t[k][0..5]` | 2,636 | 841 | 17,172 |
| `gun->geometry->setRotation(MatrixRotateVector(...))` direct | 3,854 | 855 | 17,168 |
| struct `{ Matrix rotation; Vector v; }` | 4,570 | 1,037 | 17,216 |
| `Vector tmp` outside loop + copy | 16,809 | 3,135 | 17,420 |
| `const bool *baw = &bigAlienWave; *baw ? ...` | 10,372 | 961 | 17,168 |

### 2d. fillers tried to recover the 4 B of the two-term crutch

`shipGroup + (i<0)`, `| (i<0)`, `^ (i<0)`, `+ (i>>31)` all add 12 B
(17,180).  A second `cargo = nullptr` store adds 8 B (17,176).  `v.x += 0.0f`,
`base.x += 0.0f` add 36 B.  `k += 0`, `k += (i & 0)`, `shipGroup + 0`,
`if (i & 0x40000000) {}` fold away (17,168).  No 4-B filler was found.

### 2e. loop form / declaration order

`for`/`while`/`do`, `i++,k++` vs `++i,++k`, `k` as `short`/`size_t`/`long
long`, `k = i - start`, declaration permutations of
`{rotation, v, k, base}`: all neutral at `2,636 / 841` or size-breaking
(17,176-17,348).  `base` before `v` = 2,664/852; `rotation` after `v` =
3,951/855/17,168.

## 3. Conclusion

The +10 is a greedy-RA live-range artifact: `gun` occupies `r4`, forcing
`&v`, `&rotation` and the reloaded `this` into high registers and the
`MatrixRotateVector` return slot into `r9`.  The checked-in crutch is the
only 4-B size filler that keeps the select in-loop, but it is a cmov where
the binary has a `cbz` diamond.  The only raw win in the measured space is
the w13685 `cargo` move (`2,636 -> 2,635`), which costs +181 aligned and so
fails the task's `aligned <= 841` target.  `src/` is left unchanged.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13691
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2636 aligned=841 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13691/`
(`measure.py`, `bucket.py`, `side.py`, `try_*.py`, `sweep_*.py`,
`Level.base.cpp`, `base_sem.so`, `c2.so`, `c5.so`, `c6.so`, `ga.so`, `t5.so`,
`plain.so`, `v1.so`).
