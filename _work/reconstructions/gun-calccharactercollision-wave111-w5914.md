# `Gun::calcCharacterCollision` wave 111 (w5914): the stack-slot swap is a
# post-RA coloring-order artifact; the shipped map needs *more* pressure than
# any `F P H L K` source shape produces

Worker: w5914. Branch: `wave111-w5914`, base master `8eea060cc`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w5914/`.

## 0. Result

**No code change landed.** HEAD (`src/game/weapons/Gun.cpp`, `int dmg = this->damage` +
four `enemy->damage(...)` call sites + tail `F H K L P`) is still the best measured shape:
**90.0 %**, 1984/1984 B, 585/650 normalized instructions (official verifier).

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `8eea060cc`) | **4500 / 4500** | **90.0** | **1984** | **585/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **585/650** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(no code change; 27 sections, `.ARM.extab` still `0x26294`, file size still 2,187,328 B).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. New this pass: the exact stack map of both builds

Wave 110 labelled the two swapped slots `expl`/`td`; that label was wrong.  The
`0x152fe4` store is **`explosive`**, and `explodeOnContact` is the `orrs r1,r5` store at
`0x15300c`.  Full map (verified against the post-PEI MIR frame objects *and* the asm
loads/stores):

| off | shipped | HEAD | shipped store / load | HEAD store / load |
|---|---|---|---|---|
| `sp+16` | `explodeOnContact` | `explodeOnContact` | `0x15300c` / `0x15326e` | same |
| `sp+20` | **`explosive`** | **`td`** | `0x152fe4`,`0x1535d4` / `0x1531aa`,`0x153354` | `0x153010` / per-use |
| `sp+24` | **`td`** | **`explosive`** | `0x153010` / `0x15319c`,`0x153266`,`0x1533f6` | `0x152fe4` / `0x1531aa`,`0x153354` |
| `sp+28` | `e` | `e` | `0x153018` / `0x153508` | same |
| `sp+32` | `scatter` | `scatter` | `0x152fd4` / `0x1530c2`,`0x15341e` | same |
| `sp+36` | `mine` | `mine` | `0x15300e` / `0x1530b8`,`0x153196`,`0x153260`,`0x153510` | same |
| `sp+40` | `i` | `i` | `0x153074` / (many) | same |

So exactly **two** 4-byte slots are exchanged, and nothing else in the frame differs.

### Why: post-RA `StackSlotColoring` order

The 4-byte frame objects do not exist before register allocation (`Live Stack Slot
Analysis` shows only `fi#0..fi#11`); greedy regalloc creates the spill slots
`fi#13..fi#20` and `StackSlotColoring` renumbers them.  `PrologEpilogInserter` then
assigns offsets in **reverse new-index order** (highest index gets the lowest offset).
HEAD's post-coloring order is

```
fi13=i  fi14=mine  fi15=scatter  fi16=e  fi17=explosive  fi18=td  fi19=eoc  fi20=guard
```

which yields `i=40, mine=36, scatter=32, e=28, explosive=24, td=20, eoc=16`.  The shipped
build needs `td` *before* `explosive` in that list (`… e, td, explosive, eoc`).  The
pre-coloring spill order is identical in both builds (`14=scatter, 15=mine, 16=eoc,
17=e, 18=explosive, 19=i, 20=td`), so the reorder is a pure consequence of the live
intervals the allocator produced, not of declaration order.

## 2. New this pass: the shipped build uses *all* callee-saved registers

The shipped map is `r9=this, r8=enemy, r6=td, r4=i*3, fp=i, sl=sp+192`, with `r5` free.
That is six callee-saved values live through the tail.  Every source shape that produces
the shipped tail order `F P H L K` (so the shared `1` constant can go to caller-saved
`r1`) **lowers** the pressure enough that the allocator demotes `this` to `r4` and uses
`r9` as a scratch:

```
tree + F P H L K : 64.8 %, 1912 B, this=r4, i3=r5   (mov r4,r0 ; mov r9,r4 ; str r9,[…,#216])
phi  + F P H L K : 61.8 %, 1904 B, this=r4, i3=r5
```

`F P H L K` frees `fp`, and the allocator then simply needs one fewer callee-saved
register.  Conversely every shape that keeps `this=r9, i3=r4` (i.e. HEAD's `F H K L P`)
keeps the shared `1` in `fp` across the `H` and `K` calls and therefore has no `fp` for
`i`.  This is a sharper statement of wave 110's anti-correlation: it is not just that
`fp` is occupied, it is that the *whole* callee-saved budget differs by one value.

## 3. Measured tables (official verifier; `link_so` + `tools/verify/verify.py` scoring)

### 3.1 Five-knob power set on HEAD (32 builds)

Knobs: `m` = `expect(mine,0)` on the **radius** `if (mine)`; `k` = `expect(ki->field_0x71,0)`;
`d` = `expect(distance < magnitude,0)`; `p` = phi damage form; `t` = tail `F P H L K`.

| combo | pct | size | this | i3 | join_fp | join_td | expl | td |
|---|---:|---:|---|---|---|---|---|---|
| `none` (= HEAD) | **90.0** | 1984 | r9 | r4 | F | F | 24 | 20 |
| `mkdpt` (= wave-109 `faithful2`) | 87.6 | 1984 | r9 | r6 | F | **T** | **20** | **24** |
| `mt` | 85.5 | 1984 | r9 | r4 | F | F | 24 | – |
| `kdpt` | 85.5 | 1984 | r9 | r5 | F | T | 20 | 24 |
| `dt` | 84.8 | 1980 | r9 | r5 | F | T | 20 | 24 |
| `p` | 84.1 | 1980 | r9 | r4 | F | F | 24 | 20 |
| `mdpt` | 83.5 | 1980 | r9 | r6 | F | T | 20 | 24 |
| `dpt` | 83.3 | 1976 | r9 | r5 | F | T | 20 | 24 |
| `kd` | 69.4 | 1948 | r9 | r5 | F | F | 24 | – |
| `d` | 69.3 | 1944 | r9 | r5 | F | F | 24 | – |
| `t` alone | 64.8 | 1912 | r4 | r5 | F | F | 24 | – |

`expl = 20 ∧ td = 24` only ever appears together with `i3 ∈ {r5, r6}`.  `i3 = r4` only
ever appears with `expl = 24 ∧ td = 20`.  **No combination in this 32-point space breaks
the anti-correlation.**

### 3.2 Single-site `__builtin_expect` on `tree + F P H L K` (48 builds)

Best is `expect(radius-mine, 0)`:

| variant | pct | size | this | i3 | join_td | expl | td |
|---|---:|---:|---|---|---|---|---|
| `FPHLK` + `expect(radius-mine,0)` | **85.5** | 1984 | **r9** | **r4** | F | **20** | 28 |
| `FPHLK` + `expect(distance,0)` | 84.8 | 1980 | r9 | r5 | T | 20 | 24 |
| `FPHLK` (no expect) | 64.8 | 1912 | r4 | r5 | F | 24 | – |
| all other 45 sites | ≤ 64.8 | | | | | | |

The 85.5 row is the closest hybrid found in three waves: it has `this=r9`, `i3=r4` **and**
`explosive` at `sp+20`, but `td` lands at `sp+28`, `e` at `sp+24`, and `enemy`/`td` are
swapped (`r6`/`r8`).  Stacking a second expect on top of it (46 more builds) never beats
85.5.

### 3.3 `direct` damage form + `F P H L K` (16 builds)

| combo | pct | size | i3 | join_td | expl | td |
|---|---:|---:|---|---|---|---|
| `md` | 83.9 | 2000 | r6 | T | 20 | 24 |
| `d` | 83.7 | 1996 | r5 | T | 20 | 24 |
| `mkd` | 83.0 | 2000 | r6 | T | 20 | 24 |
| `none` | 64.1 | 1920 | r5 | F | 24 | – |

### 3.4 Random search

400 random samples over `{tree, phi, direct} × 120 tails × 10 expect sites × 2 values`
(seeds 7 and 12345) never exceeded **85.3 %**; the best random hit was
`tree + F H P L K` at 85.3 %, 1976 B.

## 4. New variants measured this pass (all ≤ 90.0)

All of the following are **codegen-identical** to HEAD (90.0 / 1984 / 585, same signature):

* `int` instead of `bool` for `mine`, `scatter`, `explosive`, `explodeOnContact` (each
  alone and all together);
* `for (int i …)`, `for (int e …)`, both;
* `explosive` declared-and-initialised in place (`int` and `const bool`), forward
  declaration removed;
* `explodeOnContact` declared after `bomb` / after `mine`+`scatter`; `explodeOnContact = bomb || mine;`
* `Vector *td` → `Vector &td` (with `td = …`, `td -= …`, `VectorLength(td)`);
* `td` declared at the top of the function, or at the top of the outer loop body;
* `td` declared after the `explodeOnContact` block;
* `while`-loop rewrite of the outer loop; `this->enemies->count` / `enemies->data()[e]`;
* `float r = radius;` / `const float r`; `(void)`-style `const` on the radius local;
* `radius = enemy->radius; if (useCustomRadius) radius = customRadius;`
* `if (mine && !enemy->enemyFlagsLo) continue;` (two separate `if (mine && …)`);
* `enemy->isActive() == 0`, `getHitpoints() <= 0`;
* `= true` / `(uint8_t) 1` for `field_54[0]`, `field_54[1]`, `level->field_30`.

Not identical, all worse:

| variant | pct | note |
|---|---:|---|
| `bool scatter` declared before `bool mine` | 88.8 | 577 matched, same sig |
| `int outer` instead of `bool outer` | 84.0 | 1980 B |
| `int dmg` hoisted above `empDamage` / `explodeOnContact` | 85.0 | 1980 B |
| `int dmg` hoisted above `ki` | 63.9 | this=r4 |
| `int dmg` hoisted above `radius` | 80.3 | 1992 B |
| `if (mine) {…} if (!mine && scatter) {…}` | 66.3 | i3=r1 |
| `if (mine) {…} if (scatter) {…}` (split) | 60.4 | expl=20, td gone |
| `td` declared before `weaponType` / before `explosive` | 66.2 / 67.1 | this=r8 |
| `explosive` assigned before `mine`/`scatter` | 51.5 | this=r4 |
| `Vector &position` / `&velocity` | 50.2 | this=r8 |

## 5. Why the join reloads appear in the shipped build

`0x15318c` (`blx isAsteroid`) is reached from **both** the `outerCollide` path and the
bounding-box path (`0x15324e: beq 0x15318c`), and `fp` holds `radius` up to that point
(`0x1530b4`/`0x15315c`, consumed by the box compare at `0x1531c8`).  After the call the
shipped build reloads `i` into `fp` and `td` into `r6` at **both** joins
(`0x153198`/`0x15319c` and `0x153262`/`0x153266`) because both values are then live and
hot in the tail.  HEAD instead reloads `i` per use (`0x153330`, `0x153366`, `0x153372`,
`0x153384`, …) and `td` per use (`0x15336a`, `0x153376`, `0x1533f8`), because `fp` is
already committed to the shared `1` constant.  Fixing the tail order alone removes the
`fp` conflict but also removes the pressure that keeps `this` in `r9`, so the reloads do
not move into place.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984
```

Scratch harnesses (`/dev/shm/w5914/`): `eval.py` (splice body + `link_so` + score + 5-field
signature in one process, ~0.85 s/build), `batch.py` (score a list of bodies),
`sweep_tail.py` (all 120 tail permutations of a body), `randsearch.py` (randomised
`{damage form × tail × expect subset}` search), `body_head.cpp` / `body_f2.cpp` /
`body_fphlk.cpp` (drop-in bodies), `paa.txt` (`-mllvm -print-after-all` dump; the
`Greedy Register Allocator`, `Stack Slot Coloring` and `Prologue/Epilogue Insertion`
sections are the ones that matter).

## 7. Files changed

None under `src/` (note only).
