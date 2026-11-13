# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13702

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13702`, base master `21aadcc74`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13680.md` (the landed store reorder,
raw 2,636 -> 2,626 at aligned 841 -> 864), `...w13691.md`, `...w13685.md`,
`...w13671.md`, `...w13650.md`.

Task: recover the aligned **+23** while keeping the semantic alienGuns
interleave and the **2,626** raw.  **No such shape was found; note-only,
`src/` unchanged.**

```text
                                     size            raw    aligned  insn
base 21aadcc74 (w13680 landed)       17172/17172     2626    864     6206/6197
```

All size-exact candidates below have `.ARM.extab` diff **577** (size/base
exact at `0x1a3b78`), `.ARM.exidx` 0, `.rodata` 0, `.rel.plt` 0.  The current
tree is byte-for-byte the checked-in one: `.text` diff **71,339**, raw 2,626,
aligned 864.

## 1. The +23 is entirely inside the alienGuns loop body

Local (difflib on the loop span `0xb329a..0xb3398` only) aligned mismatch
counts, original 91 instructions vs ours 89:

| shape | local aligned | global aligned | raw |
|---|---:|---:|---:|
| landed w13680 (`T C R1 SR R2 S1 P SC G`) | **82** | **864** | **2,626** |
| w13650 (`S1 T R1 SR R2 C P SC G`) | 60 | 841 | 2,636 |
| pre-w13650 (`S1 T R1 R2 SR C P SC G`, non-semantic) | 63 | 845 | 2,626 |
| gun-after (`STORE GUN C T R1 SR R2 S1 P SC G`) | 82 | 863 | 2,627 |
| gun-after + `S1` early (`STORE GUN S1 T R1 SR R2 C P SC G`) | 56 | 837 | 2,637 |

The global delta is the local delta (22-23); the loop setup (`0xb323c..0xb329a`)
scores 6 for every shape and the loop tail 1.  So the +23 is the price of
moving `setMaxHitpoints` from before the table select (w13650) to after the
second rotation (w13680).

## 2. Why 2,626 forces the bad instruction order

The binary's loop body is:

```text
createStaticObject; enemies[i]=ret; gun=enemies[i]; setMaxHitpoints(1000);
t = bigAlienWave ? station : outpost;             // cbz diamond, 14 B
v = MatrixRotateVector(rotation, {t[3],t[4],t[5]});
setRotation(v);
v = MatrixRotateVector(rotation, {t[0],t[1],t[2]});
setPosition(base+v); setScaling(6.0f); shipGroup=localRace; cargo=nullptr;
```

`setMaxHitpoints` is *before* the select in the binary.  But the two-term
`(bigAlienWave + (i<0) + (i<0))` crutch — the only spelling that keeps the
select in-loop at size 17,172 — compiles to a 14-B `add.w/cmp.w/it ne/movne`
cmov, 10 B longer than the binary's 14-B `cbz` diamond once the two table
addresses are counted.  The w13680 reorder pays for that by moving
`setMaxHitpoints` after the second rotation and `cargo` ahead of the select,
which recovers the 10 raw bytes but desynchronises the loop's instruction
stream (local aligned 60 -> 82).

## 3. Store-order sweep (semantic core `T R1 SR R2 P`, gun-before)

`S1` = setMaxHitpoints, `C` = cargo.  Gaps: 0 = before `T`, 1 = `T/R1`,
2 = `R1/SR`, 3 = `SR/R2`, 4 = `R2/P`, 5 = after `P` (before `SC,G`).

| `S1` gap | `C` gap | raw | aligned |
|---|---:|---:|---:|
| **4** | **0** | **2,626** | **864** |
| 4 | 1 | 2,626 | 864 |
| 4 | 2 | 2,637 | 863 |
| 4 | 3 | 2,635 | 865 |
| 4 | 4 | 2,637 | 865 |
| 4 | 5 | 2,639 | 864 |
| 0 | 0 | 2,640 | 1,022 |
| 0 | 1 | 2,640 | 1,022 |
| 0 | 2 | 2,637 | 1,005 |
| 0 | 3 | 2,635 | 1,022 |
| 0 | 4 (w13650) | 2,636 | **841** |
| 0 | 5 | 2,638 | 1,022 |
| 5 | 0 | 2,635 | 1,013 |
| 5 | 4 | 2,648 | 862 |
| 2/3 (between rotations) | any | 2,803-2,820 | 915-1,057 |

Same-gap `S1,C` orders (both orders, all six gaps): best 2,637/864.  Full
`SC`/`G` placement sweep on top of `S1=4,C=0` (30 permutations): best raw
**2,627 / 866** (`SC` at `SR/R2`, `G` last), never 2,626 below 864.

## 4. Gun placement / temporary lifetimes

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `gun` before store (landed) | **2,626** | **864** | 17,172 |
| `gun` after store (`GA`) | 2,627 | 863 | 17,172 |
| `gun` after store + `S1` early | 2,637 | 837 | 17,172 |
| `gun` after store + `S1=4,C=0` + `SC`/`G` perms | 2,628-2,659 | 865-879 | 17,172 |
| `KIPlayer *const gun` / `auto *gun` / `PlayerTurret *turret` temp | 2,626 | 864 | 17,172 |
| `Vector *pv` / `Matrix *pr` / `Vector *pbase` aliases | 2,626 | 864 | 17,172 |
| `Vector v` uninit / ctor / `Vector::{}` / `const` | 2,626 | 864 | 17,172 |
| `Vector v` / `Vector v2` declared outside (separate second temp) | 4,376-4,429 | 914-966 | 17,176 |
| `Vector r2` copy for setPosition | 2,630 | 873 | 17,172 |
| `Player *player` / `AEGeometry *geom` temps | 4,292 / 4,388 | 883-892 | 17,176-17,188 |
| named `Vector a = {t[3],t[4],t[5]}` before first rotation | 2,946 | 880 | 17,172 |
| `float t0..t5 = t[k][0..5]` (named row scalars) | 2,626 | 864 | 17,172 |
| `const float *t` flat `t[k*6+3]` indexing | 2,626 | 864 | 17,172 |
| `float bx/by/bz = base.*` hoisted | 2,626 | 864 | 17,172 |
| `const Vector &base` / `Vector base0; base=base0` | 2,626 / 4,245 | 864 / 877 | 17,172 / 17,164 |

## 5. `t`-select spelling (all on the landed store order)

| expression | raw | aligned | size |
|---|---:|---:|---:|
| `(bigAlienWave + (i<0) + (i<0))` (landed) | **2,626** | **864** | 17,172 |
| `... != 0` / `... > 0` / `... >= 1` / `(bool)` / `(int)` | 2,626 | 864 | 17,172 |
| `(bigAlienWave + (i >> 31) + (i >> 31))` | 2,626 | 864 | 17,172 |
| `(bigAlienWave + (int)(i<0) + (int)(i<0))` | 2,626 | 864 | 17,172 |
| `(bigAlienWave + !!(i<0) + !!(i<0))` | 2,626 | 864 | 17,172 |
| `(bigAlienWave + (0 > i) + (0 > i))` | 2,626 | 864 | 17,172 |
| `(bigAlienWave + (i<0)*1 + (i<0)*1)` | 2,626 | 864 | 17,172 |
| `bool sel = ...; sel ? A : B` | 2,626 | 864 | 17,172 |
| `if/else`, `goto`, `switch`, `__builtin_expect`, `t=A; if(...) t=B` | 2,626 | 864 | 17,172 |
| `(bigAlienWave + (i<0) + (i<0)) == 1` | 2,628 | 864 | 17,172 |
| `(bigAlienWave + (i<0) + (i<0)) < 1` | 2,628 | 864 | 17,172 |
| `(i<0) == bigAlienWave` / `!=` | 3,862-3,864 | 887 | **17,168** |
| `(i<0) ? A : (bigAlienWave ? A : B)` (nested, blocks LICM) | 4,045 | 900 | **17,168** |
| one-term `(bigAlienWave + (i<0))` | 4,067 | 914 | **17,168** |
| one-term + duplicate `cargo` filler | 2,952 | 887 | 17,172 |
| plain `bigAlienWave` (LICM-hoisted) | 4,373 | 913 | **17,168** |
| `(bigAlienWave \|\| (i<0))` | 4,045 | 900 | 17,168 |
| `(bigAlienWave && !(i<0))` | 4,052 | 892 | 17,168 |
| `volatile bool` copies / `volatile bool *` | 4,293-16,705 | 939-4,238 | 17,184-17,304 |

Every 17,172 spelling produces the *identical* instruction stream
(`add.w r0,r0,sl,lsr#31; cmp.w r0,sl,asr#31; it ne; movne`), so no spelling
can move aligned.  The compiler never emits the binary's `cbz` diamond for a
loop-variant condition at `-Oz`; only the hoisted plain condition gives a
branch, and that costs the 4 B that the `ldrd`/`strd` pair saves.

## 6. Loop form / declaration order

`for`, `while`, `++i`, `i++`, `k` in the header, `size_t k`, `k` after
`base`: all neutral at **2,626 / 864**.  `do/while`, `while(true)+break`,
`goto` loop: 13,274 / 1,193 / **17,184**.  Hoisting `start` is neutral;
hoisting the bound is 5,639 / 1,515 / **17,192**.  Declaration permutations
other than the checked-in one are neutral or size-breaking
(`base` first: 2,701/887; `rotation` last: 3,952/885/**17,168**).

## 7. Cross-checks

* Pre-w13650 non-semantic shape (`S1 T R1 R2 SR C P SC G`): **2,626 / 845**
  (`.text` 71,345).  It is 19 aligned better than the landed shape but calls
  `setRotation` with the `t[0..2]` vector (the w13650 semantic bug).
* w13650 semantic shape (`S1 T R1 SR R2 C P SC G`): **2,636 / 841**
  (`.text` 71,349).  Recovering the 10 raw bytes is exactly the w13680
  store reorder; there is no third lever.
* `pirateFortress` `gun` after store (a separate semantic fix): 2,632 / 861
  (`.text` 71,345); combined with `GA`: 2,633 / 860.  Raw +6/+7, not
  landable under the 2,626 constraint.
* `bigAlienWave` retyped `int` (to try to force `cbz`): 12,464 / 986 /
  **17,156**.

## 8. Conclusion

The +23 is the S1-position cost, and raw 2,626 is only reachable with the
`gun`-before / `cargo`-first / `setMaxHitpoints`-after-rotation2 layout.  All
2,626 shapes share one instruction stream (local aligned 82, global 864);
every semantic shape that aligns better costs raw (w13650: 2,636/841;
gun-after: 2,627/863).  No compensating shape with **raw 2,626 and aligned
< 864** exists in the measured space.  `src/` is left unchanged.

## 9. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13702
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=864 size=17172/17172 insn=6206/6197`,
`.text` diff 71,339, `.ARM.extab` 577, `.ARM.exidx` 0, `.rodata` 0.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13702/`
(`harness2.py`, `sweep1.py`..`sweep51.py`, `localalign.py`, `regions.py`,
`perinsn.py`, `bytediff2.py`, `secdiff.py`, `A_w13650.so`, `B_w13680.so`,
`PRE.so`, `GA.so`, `F2.so`, `EQ.so`, `N1.so`, `Level.base.cpp`).
