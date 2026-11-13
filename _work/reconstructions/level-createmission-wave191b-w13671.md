# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13671

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13671`, base master `2064dfd40`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13650.md` (landed the faithful
`alienGuns` interleave, raw +10 / aligned -4), `...w13638.md`, `...w13625.md`.

```text
                                     size            raw    aligned  insn
base 2064dfd40 (w13650)              17172/17172     2636    841     6206/6197
this pass (landed, cargo c2)         17172/17172     2635   1022     6206/6197
```

Landed: move `gun->cargo = nullptr;` from between the second
`MatrixRotateVector` and `setPosition` to between `setRotation` and the second
`MatrixRotateVector` (all three statements are independent; `setRotation` and
`MatrixRotateVector` do not read `cargo`, so this is semantically neutral and
keeps the w13650 interleave).  Raw **2636 -> 2635 (-1)**, aligned
**841 -> 1022 (+181)**, size exact 17,172/17,172, `.ARM.extab` unchanged
(577 differing bytes, size/base exact at `0x1a3b78`), `.text` diff
71,227 -> 71,226, `.ARM.exidx` 0, `.rodata` 0.  `check` green: byte_exact
4508, linked_exact 4508, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

## 1. What the +10 actually is

The w13650 interleave is semantically right, but it shifts the second
rotation's two `blx` call sites (and their `mov` operands) by +8 relative to
the original, so those bytes stop matching.  Comparing the span from the
`setMaxHitpoints` return to the second rotation start:

| span element | original | ours | delta |
|---|---|---:|---:|
| crutch table select | 14 B (`cbz` diamond) | 18 B (`it ne`/`movne`) | **+4** |
| `&temp` / `&rotation` address setup | `add r5,sp,#212` + `add r4,sp,#312` (2+2) | `add.w r9,sp,#212` + `add.w fp,sp,#312` (4+4, hoisted) | **+6** |
| `Vector{...}` field loads | 6 separate `ldr` (14 B) | `ldrd`/`ldrd`/`strd` (12 B) | **-2** |
| `&v` for `Vector::operator=` | `add r4,sp,#372` (2 B) | `add.w fp,sp,#372` (4 B) | **+2** |
| `t + k*24` | `add.w r0,r1,r9` (4 B) | `adds r0,r5,r6` (2 B) | **-2** |
| table base save | `mov sl,r1` (2 B) | — | **-2** |
| **net** | 70 B | 80 B | **+10** |

The crutch select is size-load-bearing (plain `bigAlienWave ? ... : ...` is
17,168 B and re-allocates the whole function: raw 4,371).  The hoisted 4-byte
address adds are the greedy-RA choice of high registers (`r9`, `fp`) for
loop-invariant stack addresses; the original uses low registers (`r5`, `r4`)
and recomputes `&rotation` inside the loop.  Both are global-RA effects, not
source order.

## 2. Measured shapes (all size 17,172 and `.ARM.extab` 577 unless noted)

Baseline on top of w13650 is `2636 / 841`.  Every group below was rebuilt and
measured with `tools/fn_diff.py` + section headers.

### 2a. `cargo = nullptr` placement (the only raw win)

`c0` = before the first rotation, `c1` = after the first rotation, `c2` =
after `setRotation`, `c3` = after the second rotation (checked in before this
pass), `c4` = after `setPosition`, `c5` = after `setScaling`, `c6` = last.
Tail order `PSG` = `setPosition; setScaling; shipGroup` (checked in).

| variant | raw | aligned |
|---|---:|---:|
| `c0` | 2,640 | 1,022 |
| `c1` | 2,637 | 1,005 |
| **`c2` (landed)** | **2,635** | **1,022** |
| `c3` (base) | 2,636 | 841 |
| `c4`/`c5`/`c6` (= w13650 PSGC/PSCG/PSGC perms) | 2,636–2,642 | 841–1,022 |

All 24 permutations of `{setPosition,setScaling,cargo,shipGroup}` were
re-measured on top of the interleave; the best raw is `c2` + `PSG` = 2,635.
Every variant with `cargo` scheduled after `setScaling` scores aligned 1,022;
every variant with `cargo` before `setScaling` scores 841–847.  The landed
`c2` is the former, hence the aligned jump.

### 2b. Loop form (all neutral, 2,636 / 841)

`for (int i = A, k = 0; ...)`, `int i = A; for (; ...)`, `++i/++k`,
`i++, k += 1`, `i++, k = k + 1`, `k++` in the body, `while` loop,
`i <= B-1`, `i != B` (2,638), `B > i` (2,638).  `int end = B;` hoisted
changes the size (17,184) and the whole function.

### 2c. Temporary lifetimes (all size-breaking unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `gun->geometry->setRotation(MatrixRotateVector(...))` | 3,854 | 855 | 17,168 |
| `Vector r = first; setRotation(r); v = second;` | 3,838 | 882 | 17,168 |
| `Vector r; r = first; ...` | 4,367 | 1,030 | 17,192 |
| `Vector p = second; setPosition(p)` | 3,863 | 874 | 17,168 |
| `Vector pos = base + v; setPosition(pos)` | 4,457 | 889 | 17,140 |
| `Vector v2` outside / inside the loop | 4,417 / 3,838 | 1,034 / 882 | 17,192 / 17,168 |
| `Vector v` inside the loop | 3,919 | 938 | 17,176 |
| `const Vector &r = v; setRotation(r)` | 2,636 | 841 | 17,172 |
| `AEGeometry *geom = gun->geometry` | 2,636 | 841 | 17,172 |
| `Matrix *rot = &rotation` / `Matrix &rot = rotation` | 2,636 | 841 | 17,172 |

### 2d. `gun` lifetime / store order / setup

| variant | raw | aligned |
|---|---:|---:|
| `gun` after `createStaticObject` store (w13638 §3g) | 2,637 | 837 |
| `gun` after store + `cargo` c2 | 2,636 | 1,018 |
| `KIPlayer *gun;` declared before the loop | 2,636 | 841 |
| `KIPlayer *gun = createStaticObject(...); enemies[i] = gun;` | 4,370 | 857 (17,156) |
| `Array<KIPlayer*> *enemies = this->enemies;` alias | 4,339 | 922 (17,164) |
| `Vector base` before `int k` / `Vector base(...)` / `center` local | 2,636 | 841 |
| `Vector v;` (no init) / `Vector v = Vector{...}` / `Vector v = {}` | 2,636 | 841 |
| `setMaxHitpoints` moved after the rotations | 2,816–2,820 | 915–921 |
| `t` declared before `setMaxHitpoints` | 2,636 | 841 |

### 2e. Crutch expression (size-load-bearing)

| expression | raw | aligned | size |
|---|---:|---:|---:|
| `(bigAlienWave + (i < 0) + (i < 0))` (checked in) | **2,636** | **841** | 17,172 |
| `bigAlienWave` (plain) | 4,371 | 874 | 17,168 |
| `(bigAlienWave + (i < 0))` | 4,074 | 892 | 17,168 |
| `(bigAlienWave - (i < 0))` / `^` | 3,871 | 865 | 17,168 |
| `(bigAlienWave \| (i < 0))` | 4,038 | 869 | 17,168 |
| `(bigAlienWave + (i < 0)*2)` / `2*(i<0)` | 4,036 | 870 | 17,168 |
| `(bigAlienWave + !!(i < 0))` | 4,074 | 892 | 17,168 |
| `(bigAlienWave + (i<0) + (i<0)) != 0` | 2,636 | 841 | 17,172 |
| `(bigAlienWave + (bool)(i<0) + (bool)(i<0))` | 2,636 | 841 | 17,172 |
| `(bigAlienWave + (i<0) + (i<0) + (i<0))` | 2,946 | 843 | 17,172 |
| crutch on `k` / `alienGuns` / `localRace` / `friendCount` | 3,902–4,502 | 874–954 | 17,168–17,196 |

Only the exact `i`-based two-comparison form keeps size 17,172; any other
opaque condition re-allocates the function.

### 2f. `pirateFortress` (checked-in order is already best)

`PSGC` (`setPosition; setScaling; shipGroup; cargo`) is the checked-in order
and measures 2,636 / 841.  All 24 permutations were re-measured on top of the
interleave: best alternatives 2,642 (`PSCG`) and 2,651 (`PCSG`).  Dropping
the `Vector p` rotate temp (`v = MatrixRotateVector(...)` directly) is
16,656 / 2,240 / 17,316; `setRotation(MatrixRotateVector(...))` is
2,967 / 845 / 17,172.

### 2g. Case-2 loot store order (all 24 permutations)

Checked-in `GSCR` (crateCaptured, setSpeed, cargo, setRotationSpeed) is the
best raw at 2,636 / 841.  Next: `GCRS` 2,637 / 845, `GCRS`/`CRGS` 2,639 /
845, `CGSR` 2,641 / 837, `GCSR` 2,644 / 838.

### 2h. Case-1 `enemyRace` front-end-object shapes (hoist blocker)

`int enemyRaceBox[N]` with `[1]`/`[N-1]`, `struct {int pad; int race;}`,
`union {long long; struct{int pad; int race;}}`, `long long`/`double` with
`((int*)&x)[1]` are all SROA-promoted and **neutral** (2,636 / 841 / 17,172):
the hoist is not blocked.  `Vector raceVec` (`.x`/`.y`/`.z`) blocks the hoist
but re-allocates the whole function: 16,078 / 1,635 / **17,140**.  An
out-of-line `RaceBox()` ctor gives 16,519 / 2,346 / **17,240**.

## 3. Why no larger win

The +10 is the sum of the crutch select (+4, size-load-bearing) and the
greedy-RA choice to hoist the two stack-address adds into high registers
(+6).  Both are global-RA effects; every source shape that removes them
changes the frame or the whole-function colouring and leaves size 17,172.
The only source-order degree of freedom that moves raw at all is the
`cargo = nullptr` store, and its best position (`c2`) recovers exactly 1 of
the 10 bytes.  The aligned metric jumps because the cargo store now sits
ahead of the second rotation's argument setup, which desynchronises the
instruction-stream alignment for the rest of the loop; the raw byte count is
the primary metric.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13671
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2635 aligned=1022 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13671/`
(`measure.py`, `sweep1.py`..`sweep24.py`, `adiff.py`, `base.so`, `cur.so`,
`C2.so`).
