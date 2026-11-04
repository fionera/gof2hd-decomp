# w5516: `Level::createMission` (92.028388 -> 92.094405 %, 17172 B) — wave-107:
# `setScaling` issued before the turret rotation/position restores the allocator

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w5516`, branch `wave107-w5516`,
base `f857b5171`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).

## Result

```text
_ZN5Level13createMissionEv   fuzzy 92.028388 -> 92.094405 %
                             size  17172 B (unchanged, size-exact)
                             orig 6055 insns / ours 6063 insns (unchanged)
                             difflib non-equal instructions 569 -> 563
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
```

ELF shape unchanged: 28 sections, `.text` 0x1337c8, `.ARM.extab` 0x26294
(the pre-existing 4-byte-short delta vs 0x26298), `.ARM.exidx` 0x6f70.
No section, extab or EH change was introduced.

## The landed change (`src/game/world/Level.cpp`, alienGuns loop)

Move the `PlayerTurret::setScaling(6.0f)` call from after
`gun->setPosition(...)` to directly after `gun->player->setMaxHitpoints(1000)`,
i.e. *before* the table select and both `MatrixRotateVector` calls:

```cpp
            (*this->enemies)[i] = this->createStaticObject(nullptr, 6772, true);
            KIPlayer *gun = (*this->enemies)[i];
            gun->player->setMaxHitpoints(1000);
            // `setScaling` is issued before the turret's rotation/position are
            // computed; this statement order reproduces the original's register
            // allocation around the table select.
            ((PlayerTurret *) gun)->setScaling(6.0f);
            // `i < 0` ... codegen anchor ...
            const float (*t)[6] = (bigAlienWave + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
            ...
```

The call is a `blx` with a single float argument, so its position in the source
is free (no data dependence on the rotation/position temporaries).  Clang's
scheduler does *not* move it back: the emitted loop body is 8 bytes shorter and
the `[sp,#200]`/`[sp,#212]` vector temporaries, the `t[1]`/`t[2]` spill slots
and the `r5`/`r6`/`r8`/`r9`/`sl`/`fp` assignment around the selector all move
closer to the original.  This is a real (if small) improvement, not a size
trick: the function size is unchanged.

## Measured table (official verifier normalisation, unrounded)

`pct` is `difflib.SequenceMatcher(autojunk=False).ratio()*100` over
`verify.py`'s reconciled disassembly.  All rows are one fresh
`cmake --build cmake-build-match --target link_so` + `score.py`.

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **baseline** | **17172** | **6063** | **92.028388** |
| **`setScaling` before the select (landed)** | **17172** | **6063** | **92.094405** |
| `setScaling` before `setMaxHitpoints` | 17172 | 6063 | 92.094405 (same IR) |
| `setScaling` after `setMaxHitpoints` | 17172 | 6063 | 92.094405 (same IR) |
| `setScaling` after the first vector | 17164 | 6063 | 90.774055 |
| `setScaling` after `setRotation` | 17164 | 6063 | 90.757551 |
| `setScaling` after the second vector | 17188 | 6066 | 90.421582 |
| landed + `gun->cargo` before `setScaling` | 17172 | 6063 | 92.077901 |
| landed + `gun->shipGroup` before `setScaling` | 17172 | 6063 | 92.077901 |
| landed + both before `setScaling` | 17172 | 6063 | 92.077901 |
| landed + `Vector v`/`Matrix rotation` decl swap | 17172 | 6063 | 91.962370 |
| landed + `Vector base` before `Vector v` | 17172 | 6063 | 91.929361 |
| landed + `KIPlayer *gun = createStaticObject(...)` | 17156 | 6060 | 89.591416 |
| landed + `(security == 3) && raid` (flips raid slot) | 17196 | 6065 | 87.095710 |
| landed + `campaign >= 32 && raid` (flips raid slot) | 17180 | 6066 | 88.755053 |
| landed + `alienGuns ? (bigAlienWave ? 8 : 7) : 0` | 17172 | 6063 | 92.044892 |
| landed + `alienGuns > 0` / `if (alienGuns)` | 17172 | 6063 | 92.094405 |
| landed + `guns = fortressGuns + alienGuns` | 17172 | 6063 | 92.094405 |
| landed + `unsigned alienGuns` / `unsigned guns` | 17172 | 6063 | 92.094405 |
| landed + `k == 0/1/2` convoy chain order | 17172 | 6063 | 91.945866 |
| landed + `switch (k)` convoy | 17172 | 6063 | 92.094405 |
| landed + `case 9` cargo/crateCaptured swap | 17172 | 6063 | 92.094405 |

`setScaling` in the *pirateFortress* loop (same move) is a regression:
`pf_sc0` 91.978874, `pf_sc1..3` 92.061396, so that loop stays as-is.

## Branchy-selector family (re-measured on top of the landed change)

| variant | size | ours insns | pct | cbz |
|---|---:|---:|---:|---:|
| landed (branchless `it ne; movne`) | 17172 | 6063 | **92.094405** | 34 |
| `dup-first` (first vector + setRotation in both arms) | 17156 | 6062 | 88.635801 | 35 |
| `dup-both` | 17156 | 6064 | 89.660863 | 35 |
| `cargo` only in one arm (shared store removed) | 17156 | 6061 | 90.706504 | 35 |
| `dup-first` + `setScaling` early | 17156 | 6062 | 88.487249 | 35 |
| `cargo`-in-one-arm + `setScaling` early | 17164 | 6062 | 87.166790 | 35 |

The original has 35 `cbz`; the landed baseline has 34.  Every shape that
produces the 35th `cbz` costs more than it gains (the loop body and the
following blocks shift), so the branchless anchor is still the best score.

## Proven vs hypothesis

Proven (measured this pass):

* `setScaling` position is a free scheduling lever worth **+0.066 pp** with no
  size change; it is the only size-preserving win found in ~110 variants.
* `sc_at0` is a plateau: every neighbouring statement move
  (`cargo`/`shipGroup`/`setMaxHitpoints` before it, declaration reorders) is
  either identical IR or 0.02-0.17 pp worse.
* The `-Oz` select-to-branch story from wave-105 holds: the landed anchor still
  lowers to `cmp.w r1, r2, asr #31; it ne; movne r6, r0`.
* `raid`/`globals` slot flip (`sp84` 2->17, `sp88` 17->2, matching the original)
  is reachable via `(security == 3) && raid` or `campaign >= 32 && raid`, but
  both cost 3-5 pp of block layout; the slot swap alone is not worth it.
* The convoy if-chain order is canonicalised by clang: writing the source as
  2,1,0 (current), 0,1,2, or `switch (k)` all give the same ascending
  `cmp #0 / #1 / #2` chain; only an explicit `switch` keeps size (92.094405).

Hypothesis (not landed):

* The original's `b2e04` `orrs` form is produced by
  `SelectionDAGBuilder::ShouldEmitAsBranches` returning false for
  `or (icmp ne (i-first, 0)), (icmp ne (alienWave, 0))` — i.e. both leaves must
  be `SETNE` against a null constant.  Our IR is
  `and (icmp ne (alienWave,0)), (icmp eq (i, first))`; CodeGenPrepare's
  `SinkCmpExpression` moves the `alienWave` compare into the branch block
  (`OptimizeCmpExpression` runs because ARM does not set
  `hasMultipleConditionRegisters()`), which lets `FindMergedConditions` split it
  into two branches.  Making `alienWave` a `bool` does suppress the split
  (emits `tst`) but changes the alienGuns select and costs ~3.9 pp.
* The `raid`/`globals` stack-slot swap is downstream of the epilogue frequency
  (wave-90: needs +0.0042, i.e. +0.74 %), which in turn is downstream of that
  same `b2e04` CFG difference.  No size-preserving spelling found.

## Remaining diff (landed)

563 non-equal instructions out of 6055/6063.  Largest clusters:

| offset | orig/ours | region |
|---|---:|---|
| `0x32ee` | 19 / 19 | alienGuns table select + first vector |
| `0x397c` | 12 / 12 | raid loop bound association / register roles |
| `0x2e2c` | 8 / 5 | merged station predicate (`orrs`) |
| `0x1440` | 8 / 8 | convoy offset if-chain order |
| `0x3e02` | 7 / 7 | extraAliens loop-bound sum association |
| `0x3c94` | 7 / 7 | extraAliens spawn jitter (register vs stack) |
| `0x3334` | 7 / 7 | alienGuns second vector `t[0..2]` |
| `0x2f54` | 6 / 1 | pirateFortress predicate tail |

Plus ~18 one-instruction `[sp,#84]`/`[sp,#88]` (`raid`/`globals`) swaps and ~6
`[sp,#40]/[sp,#44]/[sp,#48]` sum-slot rotations.

## Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5516
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# fast single-function score (official verifier normalisation, unrounded)
python3 /dev/shm/w5516/score.py
python3 /dev/shm/w5516/probe.py     # pct + size + insns + sp84/sp88 + cbz count
```

Scratch (not committed): `/dev/shm/w5516/` (`Level.base.cpp`, `score.py`,
`probe.py`, `runvar.py`, `norm.py`, `regdiff.py`, `addrdiff.py`, `dump.py`,
`emit.sh`, `v*.json`, `Level.ll`, `SDP.cpp`, `cgp_cm.txt`).

## Gate counts (before -> after)

```text
byte_exact      4500 -> 4500   (4500/4524)
linked_exact    4500 -> 4500   (4500/4524)
size_exact      4524 -> 4524
stub_zero_size     0 -> 0
missing            0 -> 0
wrong_type         0 -> 0
extra              0 -> 0
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
```
