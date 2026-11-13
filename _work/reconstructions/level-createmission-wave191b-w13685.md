# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13685

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13685`, base master `004b2758f`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13650.md` (the semantic alienGuns
interleave, raw 2,626 -> 2,636), `...w13638.md`, `...w13625.md`,
`...w13615.md`.  Also re-measured the w13655 / w13671 cargo placement on this
base.

```text
                                     size            raw    aligned  insn
base 004b2758f (w13650 landed)       17172/17172     2636    841     6206/6197
this pass (landed)                   17172/17172     2635   1022     6206/6197
```

Landed: move `gun->cargo = nullptr;` from between the second
`MatrixRotateVector` and `setPosition` to **between `setRotation` and the
second `MatrixRotateVector`**.  The store is unobservable, so the w13650
semantic interleave is preserved:
`rotate1 -> setRotation(rotate1) -> cargo -> rotate2 -> setPosition(rotate2)`.
This is the same shape independently found by w13655 (`e54f00460`) and
w13671 (`7a77baa21`).

Net: raw **2636 -> 2635 (-1)**, aligned **841 -> 1022 (+181)**, size exact
17,172/17,172, `.ARM.extab` unchanged (577 differing bytes, size/base exact
at `0x1a3b78`), `.text` diff 71,247 -> 71,246, `.ARM.exidx` 0, `.rodata` 0.
`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. The +10 is a greedy-RA live-range artifact, not a source-order cost

The w13650 interleave is semantically required and the checked-in source now
matches the binary's call sequence exactly
(`b32f6 rotate1 -> b3308 setRotation -> b3320 rotate2`).  The +10 comes from
the *registers* the greedy allocator picks in the loop:

| value | binary | ours |
|---|---|---|
| `gun` | `r6` | `r4` |
| `this` (reload) | `r4` (`ldr r4,[sp,#124]`, 2 B) | `r8` (`ldr.w r8,[sp,#124]`, 4 B) |
| `v` pointer | `r4` (`add r4,sp,#372`, 2 B) | `fp` (`add.w fp,sp,#372`, 4 B) |
| `&rotation` | `r4` (`add r4,sp,#312`, 2 B) | `fp` (`add.w fp,sp,#312`, 4 B) |
| MatrixRotateVector return temp | `r5` (hoisted in preheader) | `r9` (`add.w r9,sp,#212`, 4 B in body) |
| `t[k][1]` | kept in `r8` across the first call | `r8` reused, then `this` reloaded |
| `k*24` | `r9` | `r6` |
| `i` | `fp` | `sl` |

Because `gun` takes `r4`, every `&v` / `&rotation` / `this` pointer lands in a
high register and needs a 4-byte `add.w`/`ldr.w` instead of a 2-byte low-reg
form.  The second-rotation block therefore sits 8 B later than the binary's
(`0xb3328` vs `0xb3320`).  A byte-shift simulation of the block (w13655 §1)
puts a 4-B-earlier block at raw ~2,621, but no source shape reaches it.

## 2. Measured variants (all size 17,172 unless noted)

Starting from the w13650 tree (`2,636 / 841 / 17,172`).

### 2a. `cargo` placement (every gap in the loop body)

| cargo position | raw | aligned |
|---|---:|---:|
| before `gun` (illegal, after decl) | 2,638 | 1,023 |
| after `createStaticObject` | 2,637 | 1,011 |
| after `setMaxHitpoints` | 2,640 | 1,022 |
| after the `t` select | 2,637 | 1,005 |
| **after `setRotation` (landed)** | **2,635** | **1,022** |
| after `rotate2` (w13650) | 2,636 | 841 |
| after `setPosition` | 2,638 | 1,022 |
| after `setScaling` (natural) | 2,642 | 1,022 |
| after `shipGroup` | 2,644 | 1,022 |

`shipGroup` must stay last: moving it earlier costs 2,968..4,184 raw.

### 2b. `gun` placement / type

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `gun` before store (current) | 2,636 | 841 | 17,172 |
| `gun` after store (binary reload shape) | 2,637 | 837 | 17,172 |
| `gun` after store + landed cargo | 2,636 | 1,018 | 17,172 |
| `KIPlayer *&gun` reference | 4,304 | 860 | 17,188 |
| `auto *gun` | 2,636 | 841 | 17,172 |

### 2c. Loop forms

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `while` / `k` in `for` init / `i++,k++` swapped / `k` as `size_t` | 2,636 | 841 | 17,172 |
| hoisted `end` / `start` | 5,609 | 1,521 | 17,184 |
| `j` loop with `i = start + j` | 13,301 | 1,895 | 17,176 |
| `j` loop, `t[j]` | 16,649 | 2,368 | 17,272 |
| pointer walk over `enemies` | 13,142 | 1,895 | 17,152 |
| `k = i - start` in body | 12,153 | 1,268 | 17,168 |
| `k` as `short` / `long long` | 4,553 / 4,511 | 941 / 952 | 17,208 |
| `k` volatile | 4,605 | 960 | 17,240 |

### 2d. Temporary lifetimes / row shapes

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `v` uninitialised / `Vector v(0,0,0)` / `const` t / `const` gun | 2,636 | 841 | 17,172 |
| row scalars `r0..r5` / `float row[6]` / named `t1` | 2,636 | 841 | 17,172 |
| `Vector r1 = rot1; v = rot2; setRotation(r1)` | 2,926 | 857 | 17,172 |
| both rotations first (`r1`,`r2`) | 4,470 | 936 | 17,140 |
| `Vector r2 = rot2; v = rot1; setRotation(v)` | 3,952 | 875 | 17,168 |
| `v` inside loop | 3,919 | 938 | 17,176 |
| row pointer `const float *row = t[k]` | 12,716 | 1,265 | 17,176 |
| copy `Vector r1 = v` after rot1 | 4,785 | 1,616 | 17,196 |

### 2e. `t` select / crutch

| crutch expression | raw | aligned | size |
|---|---:|---:|---:|
| **`(bigAlienWave + (i<0) + (i<0))` (current)** | **2,636** | **841** | 17,172 |
| `+ (i<0)` (one term) | 4,074 | 892 | 17,168 |
| `- (i<0)` / `^ (i<0)` | 3,871 | 865 | 17,168 |
| `\| (i<0)` / `!=0 \| (i<0)` | 4,038 | 869 | 17,168 |
| three `+ (i<0)` terms | 2,946 | 843 | 17,172 |
| `bigAlienWave` plain (hoisted) | 4,371 | 874 | 17,168 |
| `if (__builtin_expect(...,0)) t = ...; else ...` | 2,636 | 841 | 17,172 |
| `(i >> 31)` / `(int)(i<0)` | 4,074 | 892 | 17,168 |

The two-term crutch is load-bearing for the exact 17,172 size; the plain
ternary is hoisted by LICM and loses 4 B.

### 2f. Other blocks (with the landed cargo placement)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `pirateFortress` all 24 tail permutations, best `PSGC` (current) | 2,635 | 1,022 | 17,172 |
| next best `PSCG` | 2,641 | 1,026 | 17,172 |
| `raid` rnd as plain pointer / const ref / volatile ptr / no local | 2,636 | 841 | 17,172 |
| `if (alienGuns)` / `if (alienGuns > 0)` | 2,636 | 841 | 17,172 |
| case-1 `enemyRace` in `int[2]`/`int[3]`/struct at nonzero offset | 2,636 | 841 | 17,172 |
| case-1 `enemyRace` via `Vector.y` / `float[2]` (blocks hoist) | 16,078 | 1,635 | 17,140 |

## 3. Conclusion

The +10 raw cost of the semantic interleave is **not recoverable with source
shapes** on this reconstruction.  It is a greedy register-allocation artifact:
`gun` occupies `r4`, forcing `&v`, `&rotation` and the reloaded `this` into
high registers (`fp`/`r8`) and the MatrixRotateVector return slot into `r9`.
The only landed raw win is the cargo placement above (2,636 -> 2,635); the
secondary aligned metric regresses to 1,022 because the cargo store now
precedes the second rotation's argument setup.  Raw is the primary gate
metric, so the -1 is landed.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13685
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2635 aligned=1022 size=17172/17172 insn=6206/6197`.
Scratch: `/home/debian/GalaxyOnFire2/scratch/w13685/`.
