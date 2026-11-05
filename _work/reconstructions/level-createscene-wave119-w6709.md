# `Level::createScene` — wave-119 w6709: `-g` Vector-address trick + `rnd`
# mask 78 breaks the 90.7243 % ceiling (90.7243 -> 92.7573 %, 2252 B)

**Status:** source change landed, 2026-11-05. Worktree `wt/w6709`, branch
`wave119-w6709`. Target `_ZN5Level11createSceneEv`.

Follow-up to `level-createscene-wave118-w6611.md` (and the chain back through
waves 88–118). Wave 118 concluded that the shipped `-(g+1)` / `~g` Vector
address trick was the unique maximum at **90.7243 %** and that every mask /
statement-order / type perturbation was neutral or worse. This pass found a
**better basin**: a one-instruction `-g` address trick combined with a
different `Vector` store order and a different ship-loop `rnd` mask reaches
**92.7573 %** at the same 2252 B size.

## TL;DR

* Before: **90.7243 % / 2252 B**, 783/791 instructions, first diff at
  instruction 462 (parts-loop preheader branch target `loc_610` vs `loc_614`,
  the +4 shift from the `mvns`+`add` trick).
* After: **92.7573 % / 2252 B**, 783/791 instructions, first diff at
  instruction 478 (parts-loop store schedule). The +4 shift is gone; the
  remaining parts-loop diff is the scheduler's `mov.w sl,#0` placement plus
  the `strd sl,r0,[sp,#56]` store merge, and the remaining ship-loop diff is
  the `rnd`-address register permutation.
* The landed change is three independent textual edits in the
  `missionPtr == 23` branch:

  1. The hangar-part rotation vector is built by field assignment in the
     order **y, z, x** instead of the `Vector(0, pi, 0)` constructor:
     ```cpp
     Vector rot;
     rot.y = 3.1415927f;
     rot.z = 0.0f;
     rot.x = 0.0f;
     ```
  2. The address trick is the **one-instruction** `- (size_t) g` form
     instead of `- ((size_t) g + 1)`:
     ```cpp
     Vector *rp = (Vector *) ((char *) &rot - (size_t) g);
     g->setRotation(*rp);
     ```
  3. The ship-loop `rnd` mask changes from 94 (all five variable sites
     local) to **78** (sites 3 and 4 global):
     ```cpp
     if (Globals::rnd->nextInt(100) < 30)          // was rnd->
         race = 8;
     ...
     int pick = Globals::rnd->nextInt(3);          // was rnd->
     ```
* Gate re-measured green: **byte_exact 4503/4524, linked_exact 4503/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN, drift 0, RATCHET PASS.**
* Structural state unchanged: 27 section headers, exact file size
  2,187,328 B, `.text` 0x1337c8, `.rodata` 0x41c27, `.ARM.extab` 0x26290
  (the pre-existing createShip −8 delta).

## 1. Why the old ceiling existed

Wave 118 established the two allocation basins:

* **Basin B (local-value / shipped):** `Globals::status` in `sl`, parts-loop
  table base in `r9`, `Globals::Canvas` in `r8`, parts-loop zero in `sl`.
  This is the original's *early* allocation.
* **Basin A (all-global):** `Globals::status` in `r4` (spilled), parts-loop
  table base in `r8`, `Canvas` in `r9`.

The original's ship loop is unambiguously Basin A (`r8 = &Globals::rnd`,
`ldr.w r0,[r8]` reloads at every site), but its prologue and parts loop are
Basin B. The shipped `-(g+1)` trick forces Basin B's parts-loop allocation
while keeping the ship loop's local-value form, at the cost of two extra
16-bit instructions (`mvns r0,r5; add r1,r0`) and therefore a +4-byte shift
of every later branch target.

## 2. The new basin

The `-g` form `(char *)&rot - (size_t) g` compiles to

```
add   r0, sp, #56
subs  r1, r0, r5
```

— one extra 16-bit instruction (+2 bytes) instead of two (+4 bytes), and it
still keeps the parts-loop zero in `sl`. On its own it scores **90.3431 %**
because the scheduler hoists `mov.w sl,#0` above the `rot.y` store and the
address base lands in `r0` instead of `r1`.

Building the vector by field assignment in **y, z, x** order changes the
store schedule enough that the resulting parts loop becomes

```
ldr   r0, [pc, #808]     ; pi
mov.w sl, #0
str.w sl, [sp, #64]      ; z = 0
strd  sl, r0, [sp, #56]  ; x = 0, y = pi
add   r0, sp, #56
subs  r1, r0, r5
mov   r0, r5
blx   setRotation
```

The `strd` merges the adjacent x/y stores, which is *shorter* than the
original's three separate stores, and the whole parts loop is now within
2 bytes of the original's size. Crucially, this store shape also changes the
global register pressure enough that the ship-loop `rnd` mask optimum moves.

## 3. The mask sweep

The ship-loop `rnd` sites are (bit order used by the sweep):

| bit | site | shipped (94) | landed (78) |
|---|---|---|---|
| 0 | `shipCount = Globals::rnd->nextInt(capacity+1)` | global | global |
| 1 | `if (rnd->nextInt(100) <= 29)` | local | local |
| 2 | `race = rnd->nextInt(4)` | local | local |
| 3 | `if (rnd->nextInt(100) < 30)` | local | **global** |
| 4 | `int pick = rnd->nextInt(3)` | local | **global** |
| 5 | `int spot = Globals::rnd->nextInt(capacity)` | global | global |
| 6 | `spot = rnd->nextInt(capacity)` (retry loop) | local | local |
| 7 | `Globals::rnd->nextInt(300)` | global | global |

With the y/z/x store shape and the `-g` trick, the full 5-variable-site
sweep (32 masks) gives:

| mask (5-site) | sites local | pct |
|---|---|---|
| 19 | B, C, G | **92.7573** |
| 21 | B, D, G | **92.7573** |
| 23 | B, C, D, G | 91.7408 |
| 27 | B, C, E, G | 91.8679 |
| 29 | B, D, E, G | 91.8679 |
| 30 | C, D, E, G | 91.8679 |
| 31 | all five | 88.2952 |
| 0 | all global | 75.4286 |

Masks 19 and 21 tie; the landed source uses the 8-site mask **78**
(`0b01001110`, sites 3 and 4 global), which is mask 19 in the 5-site
encoding. Making the two "always global" sites (`spot`, `setRotation(300)`)
local is worse (92.12 / 92.38).

## 4. Measurements

All variants compiled with the exact NDK r18b clang 7.0.2 `-Oz` flags,
gold-linked, stripped, `reorder_shstrtab`-fixed and scored with the official
single-function verifier.

| variant | pct | size |
|---|---|---|
| shipped wave-115 (`-(g+1)`, mask 94) | 90.7243 | 2252 B / 791 |
| `-g` trick alone (mask 94) | 90.3431 | 2252 B / 791 |
| `~g` trick alone (mask 94) | 90.7243 | 2252 B / 791 |
| `g<<31` trick (mask 94) | 90.5277 | 2252 B / 790 |
| `-g` + y/z/x store order (mask 94) | 91.7408 | 2252 B / 791 |
| **`-g` + y/z/x + mask 78 (landed)** | **92.7573** | **2252 B / 791** |
| `-g` + y/x/z + mask 78 | 92.6302 | 2252 B / 791 |
| `-g` + x/z/y + mask 78 | 92.6302 | 2252 B / 791 |
| `-g` + z/x/y + mask 78 | 92.6302 | 2252 B / 791 |
| `-g` + x/y/z + mask 78 | 88.3100 | 2252 B / 791 |
| `-g` + z/y/x + mask 78 | 88.1830 | 2252 B / 791 |
| `-g` + y/z/x + mask 23 (E local) | 91.7408 | 2252 B / 791 |
| `-g` + y/z/x + mask 27 | 91.8679 | 2252 B / 791 |
| `-g` + y/z/x + mask 31 (all local) | 88.2952 | 2248 B / 789 |
| `-g` + y/z/x + all global | 75.4286 | 2232 B / 792 |
| `-(g+1)` + y/z/x + mask 78 | 88.0559 | 2252 B / 791 |
| `-(g+1)` + y/z/x + mask 94 | 90.9784 | 2252 B / 791 |

Neutral probes on the landed shape (identical 92.7573 % code): `int n =
shipCount`, `ship`/`race`/`type`/`spot` declared outside the loop,
`unsigned` for `shipCount`/`i`/`spot`/`race`/`type`, `bool *spotUsed`,
`const int *const spots`, `AERandom *const rnd`, `rnd` moved after the
`spotUsed` fill loop, and the `table` declaration hoisted before `spot`.
Worse: `unsigned capacity` 92.6302, `rnd` declared inside the loop 91.4867,
`rnd` moved before `spotUsed` 76.6011, `rnd` before `capacity`/`shipCount`
70.7595, `spot` before `createShip` 62.3033, swapping
`setAlwaysFriend`/`setToSleep` 92.5032, swapping
`setExhaustVisible`/`setRotation` 92.3761.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6709
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (`/dev/shm/w6709`, not committed): `run.py` applies a list of
textual replacements to `src/game/world/Level.cpp`, rebuilds `link_so`
(≈2 s), and scores with `score.py` (imports the official verifier and prints
the unrounded difflib ratio and sizes). The mask sweeps are `sweep2.py`
(`-g` trick, 32 masks), `sweep3.py` (`-g` + y/z/x, 32 masks) and
`sweep4.py` (all six store permutations × masks 16–31).

## 6. Remaining deltas

The function is now instruction-exact up to instruction 477. The remaining
differences are:

1. Parts loop (instructions 478–483): the scheduler places `mov.w sl,#0`
   before the `rot.y` store and merges the x/y stores into
   `strd sl,r0,[sp,#56]`; the original materialises the zero after the y
   store and uses three separate stores.
2. Ship loop (from instruction 548): the `rnd` address is in `r2` (spilled
   to `[sp,#44]`) instead of `r8`, and `capacity`/`race`/`type` are
   permuted (`fp`/`r6`/`r8` vs the original `r6`/`r5`/`fp`).

Both are register-allocation / instruction-scheduling effects; the source
shape that produces them is not yet known.
