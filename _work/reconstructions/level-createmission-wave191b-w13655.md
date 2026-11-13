# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13655

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13655`, base master `fda02cb7e`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13650.md` (the semantic alienGuns
interleave, raw 2,626 -> 2,636), `...w13638.md`, `...w13625.md`,
`...w13615.md`.

```text
                                     size            raw    aligned  insn
base fda02cb7e (w13650 landed)       17172/17172     2636    841     6206/6197
this pass (landed)                   17172/17172     2635   1022     6206/6197
```

Landed: move `gun->cargo = nullptr;` from after the second
`MatrixRotateVector` to **between `setRotation` and the second
`MatrixRotateVector`**.  The store is unobservable (nothing between the two
points reads `cargo`), so the semantic interleave is preserved:
`rotate1 -> setRotation(rotate1) -> cargo -> rotate2 -> setPosition(rotate2)`.

Net: raw **2636 -> 2635 (-1)**, aligned **841 -> 1022 (+181)**, size exact
17,172/17,172, `.ARM.extab` unchanged (577 differing bytes, size/base exact
at `0x26298`), `.text` diff 71,201 -> 71,200, `.ARM.exidx` 0, `.rodata` 0.
`check` green: byte_exact 4508, linked_exact 4508, data_exact 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370.

The +10 raw cost of the semantic interleave is **not fully recovered**; the
best raw found is 2,635.  The aligned metric regresses because the cargo
store is now interleaved with the second rotation's setup; raw is the
primary gate metric (w13638 §3d rejected a +5 raw / -4 aligned trade for the
same reason), so the -1 raw is landed and the aligned trade is recorded here.

## 1. Why the +10 exists

The w13650 semantic fix moves the 8-byte `setRotation` block (load
`gun->geometry`, `mov r1`, `blx`) from after the second rotation to between
the two rotations.  The second rotation block is 38 B and starts at
`0xb3312` (ours) vs `0xb330c` (binary); the base (wrong-semantics) source
had it at `0xb330a`.  The byte-at-offset diff in `0xb330c..0xb3358` is 66
for the semantic order vs 56 for the base order, hence +10 raw.  The
instruction content is otherwise identical.  A byte-shift simulation of the
second rotation block (keeping the tail fixed) gives:

| block shift | raw |
|---|---:|
| -8 | 2,628 |
| -4 | **2,621** |
| 0 (current) | 2,636 |
| +4 | 2,637 |

So a 4-byte earlier second-rotation block would be worth ~15 raw, but the
block position is set by the first-rotation setup, which is already the
compact `ldrd`/`strd` form.  The only way to shorten it by 4 B is to get the
`&v`/`&rotation` pointer into a low register (`add.w fp,sp,#imm` 4 B ->
`add r4,sp,#imm` 2 B twice); the greedy allocator keeps `fp` because the
loop's callee-saved pressure is already r4/r5/r6/r8/r9/sl/fp.

## 2. Measured variants (all size 17,172 unless noted)

Starting from the w13650 tree (`2,636 / 841 / 17,172`).  `C` = cargo,
`P` = setPosition, `S` = setScaling, `G` = shipGroup; the letters are the
emitted source order after the second rotation.

### 2a. Tail permutations (`{C,P,S,G}` after rotate2)

All 24 permutations, gun-before and gun-after (48 builds).  Best:

| variant | raw | aligned |
|---|---:|---:|
| **`CPSG` / `GPSC` (gun before, landed w13650)** | **2,636** | **841** |
| `CPSG` / `GPSC` (gun after store) | 2,637 | 837 |
| `CPGS` | 2,639 | 843 |
| `CSPG` | 2,640 | 844 |
| `PSCG` | 2,642 | 1,022 |
| `PSGC` | 2,644 | 1,022 |

No permutation beats 2,636.

### 2b. Cargo before the second rotation (the landed shape)

`S4` = cargo between `setRotation` and rotate2.  All 6 tail orders of
`{P,S,G}` and both gun placements:

| variant | raw | aligned |
|---|---:|---:|
| **`S4` + `PSG` (landed)** | **2,635** | **1,022** |
| `S4` + `PSG`, gun after | 2,636 | 1,018 |
| `S4` + `SPG` | 2,637 | 867 |
| `S4` + `PGS` | 2,638 | 1,022 |
| `S4` + `GPS` | 2,639 | 845 |
| `S4` + `GSP` | 2,641 | 867 |

A full 120-permutation sweep of `{rotate1,setRotation,rotate2,C,S,G}` with
`P` after rotate2 found the same minimum: `rotate1,setRotation,C,rotate2,
P,S,G` = **2,635 / 1,022**.

### 2c. Cargo before the first rotation / between rotate1 and setRotation

| variant | raw | aligned |
|---|---:|---:|
| cargo before rotate1 (`S2`) + `PSG` | 2,640 | 1,022 |
| cargo between rotate1 and setRotation (`S3`) + `PSG` | 2,637 | 1,005 |
| cargo before rotate1 + `SPG` | 2,642 | 844 |
| cargo between rotate1 and setRotation + `SPG` | 2,639 | 844 |

### 2d. Gun placement (binary reloads the element after the store)

| variant | raw | aligned |
|---|---:|---:|
| `KIPlayer *gun = (*this->enemies)[i];` before store (current) | 2,636 | 841 |
| `gun` after `createStaticObject` (binary shape) | 2,637 | 837 |
| gun after store + cargo `S4` | 2,636 | 1,018 |
| no `gun` local, `(*this->enemies)[i]` everywhere | 4,509 | 942 (size 17,220) |
| `setMaxHitpoints` via `(*this->enemies)[i]`, then `gun` | 4,251 | 865 (size 17,184) |

### 2e. Loop forms

| variant | raw | aligned |
|---|---:|---:|
| `for (...; i++, k++)` (current) | 2,636 | 841 |
| `while (i < ...) { ...; i++; k++; }` | 2,636 | 841 |
| `for (...; i++) { ...; k++; }` | 2,636 | 841 |
| `for (int i = ..., k = 0; ...; i++, k++)` | 2,636 | 841 |
| `++i, ++k` / `i += 1, k += 1` | 2,636 | 841 |
| `k++` at body start | 2,939 | 837 |
| `int k = i - start;` in body | 16,730 | 2,392 (size 17,348) |
| `for (int k = 0; k < alienGuns; k++) { int i = ...; }` | 5,628 | 1,483 (size 17,192) |
| `do { } while` | 13,446 | 1,206 (size 17,188) |

### 2f. Temporary lifetimes / vector construction

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `Vector v` outside loop (current) | 2,636 | 841 | 17,172 |
| `Vector v` inside loop | 3,919 | 938 | 17,176 |
| `Matrix rotation` inside loop | 4,290 | 889 | 17,184 |
| `Vector p = {t[k][3],...}; v = R(p)` | 2,950 | 863 | 17,172 |
| `Vector p(t[k][3],...)` | 2,950 | 863 | 17,172 |
| `Vector p; p.x=...; ...` | 2,950 | 863 | 17,172 |
| `const float *row = t[k];` + `row[i]` | 4,031 | 941 | 17,168 |
| `float f0..f5 = t[k][0..5];` | 2,636 | 841 | 17,172 (SROA folds) |
| `Vector a; a.x=...;` elemwise first only | 2,948 | 865 | 17,172 |
| `Vector a,b` elemwise both + plain select | 3,900 | 982 | 17,176 |
| direct `setRotation(MatrixRotateVector(...))` | 3,854 | 855 | 17,168 |
| `Vector vr = R1; v = R2; setRotation(vr)` | 2,926 | 857 | 17,172 |
| `v = R1; Vector w = R2; setRotation(v); setPosition(base+w)` | 4,322 | 852 | 17,164 |

### 2g. `t` selection (the crutch)

The `(bigAlienWave + (i < 0) + (i < 0))` crutch is still required for size
17,172 (plain `bigAlienWave ? ... : ...` = 17,168).  Crutch variants:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| 3-term crutch (current) | 2,636 | 841 | 17,172 |
| 3-term + `+ 0` / `+ (i<0) - (i<0)` / `* 0` | 2,636 | 841 | 17,172 |
| 4-term crutch | 2,946 | 843 | 17,172 |
| 2-term crutch | 4,036 | 870 | 17,168 |
| plain ternary / `if/else` / `switch` | 4,371 | 874 | 17,168 |

### 2h. Declaration order

All 24 permutations of `{Matrix rotation, Vector v, int k, Vector base}`:
`R,V,K,B` / `R,V,B,K` / `R,K,V,B` / `K,R,V,B` = 2,636 / 841; every
permutation with `base` before `v` = 2,664 / 852; every permutation with
`rotation` after `v` = 3,951 / 855 / 17,168.

### 2i. Other blocks / flags

* pirateFortress tail permutations (24) and `t += 6` before/after: best
  2,636 / 841.
* pirateFortress `v = R(...)` instead of `Vector p; v = p`: 2,967 / 845 /
  17,172.
* raid `rnd`/`globals` non-volatile, declaration reorders: 2,636-2,638.
* case-2 loot `GCSR` order: 2,641 / 837 (with the w13650 tree); with the
  landed cargo `S4`: 2,640 / 1,018.
* case-2 `Array<KIPlayer *> *const enemies` alias: neutral 2,636 / 841.
* `if (alienGuns)` / `> 0` / `!!alienGuns` / `>= 1`: neutral.
* `alienGuns`/`fortressGuns` `if/else` vs ternary: neutral.
* per-TU `-mllvm` flags: `-avoid-speculation=false` neutral;
  `-disable-postra-machine-sink` 2,644 / 845; `-misched-postra` 3,506 / 912;
  `-hoist-cheap-insts` 3,318 / 893; `-regalloc=basic/pbqp`,
  `-stress-regalloc=9/10/11`, `-disable-postra-machine-licm`,
  `-enable-deferred-spilling`, `-sink-insts-to-avoid-spills`,
  `-disable-machine-licm` all change size.

## 3. Conclusion

The +10 is a byte-alignment artifact of the 8-byte `setRotation` block
moving before the 38-byte second-rotation block.  The only source-level
lever that moves the block position without changing the function size is
the `cargo = nullptr` placement; the best is `S4` (cargo between
`setRotation` and rotate2), which recovers 1 raw byte at the cost of the
aligned metric.  The 4-byte-earlier alignment that would recover ~15 raw
needs a low-register `&v`/`&rotation` pointer, which the greedy allocator
does not choose for this loop.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13655
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2635 aligned=1022 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13655/`
(`measure.py`, `sweep1.py`..`sweep17.py`, `flagtest.py`, `shift.py`,
`scan_shift.py`, `regdiff.py`, `regraw.py`, `Level.cur.cpp`, `Level.base.cpp`,
`base.so`, `cur.so`).
