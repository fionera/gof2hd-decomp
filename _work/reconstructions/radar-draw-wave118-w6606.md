# Radar::draw pass (wave118, w6606) — planet-tail store order / block-placement study

**Status:** research note, 2026-11-05. Worktree `wt/w6606`, branch
`wave118-w6606`, based on current `master`. **No source change landed**; this
commit contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale). Original 9908 B.

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 raw 74.029509 %
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals). Baseline instruction count: **3492 original / 3489 ours**
(2584 matched). Structural state re-measured this wave: **27 sections** on
both sides, exact file size **2,187,328 B**, `.text` **0x133808 (ours) vs
0x1337c8 (original)** — 0x40 B larger; `.ARM.extab` **0x26298 on both**
(the wave-117 4 B shortfall is gone); `.rodata` 0x41c27 vs 0x41b67
(the known 0xC0 B mission-case-table tail).

## 1. New finding: the planet-tail store order controls block placement

The baseline's planet tail (`if (!turretMode && !landmarkHit) { ... }`,
source lines 640-672) ends with

```cpp
} else if (this->field_0x14 == 0) {
    this->planetLockTimer = 0;
    this->field_0x18 = 0;
} else {
    this->planetLockTimer = 0;
}
```

Clang hoists the common `planetLockTimer = 0` store above the branch, so the
hot `!= 0` arm is empty and **falls through to `enemies:`**. MachineBlockPlacement
therefore lays `enemies:` out immediately after the tail, and the whole
`planet tail` block is pushed *after* the landmark loop (ours 0xb9c, original
0x6f8).

Writing the same logic with the store **duplicated** and the `== 0` arm cold
prevents the hoist:

```cpp
} else if (this->field_0x14 != 0) {
    this->planetLockTimer = 0;
} else {
    this->planetLockTimer = 0;
    this->field_0x18 = 0;
}
```

This is the **first source edit found that reproduces the original's
planet-tail / landmark block order without the invented recompute**. The
tail now ends with `b.n enemies`, MBP puts the planet tail right after the
cold autopilot block and the landmark block right after it:

```
original                           V2 (recompute deleted)
69e cold autopilot -> 16c          69e cold autopilot -> 16c
6f8 planet tail (0x50 B)           6ec planet tail (0x50 B)
758 landmark setup                 73e landmark setup
784 landmark loop header           764 landmark loop header
```

The same trick applied to the station tail
(`if (lockedStation == nullptr) { stationLockTimer = 0; field_0x28 = 0; } else { stationLockTimer = 0; }`)
is **score-neutral** (the compiler still hoists it in our TU; the original
does not).

## 2. Variants measured this wave

All are single-edit changes on the pristine baseline unless noted.
`usize` is the emitted function size; `raw %` is the official fuzzy score;
`n` is matched/3492.

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** | 9908 | **74.029509** | 2584 |
| `V2` tail store duplicated (`!= 0` hot arm) | 9760 | 68.918724 | 2387 |
| `V2R` V2 + recompute moved after the tail | 9956 | 70.273365 | 2455 |
| `V2R_S` V2R + `String label` un-merge of the generic-else DrawString | 9984 | 70.709382 | — |
| `V2R_SA` V2R_S + Ghidra asteroid `dead` form | 10004 | 69.924275 | — |
| `V2_S` V2 + `String label` un-merge | 9776 | 69.051053 | — |
| `V2E` V2 + recompute at function end | 9884 | 54.211508 | — |
| `V2P` V2 + recompute before the planet loop | 9876 | 63.576349 | — |
| `B2v` baseline + tail store duplicated (recompute kept) | 9920 | 72.745491 | — |
| `PT1` baseline + inverted `else if (field_0x14 != 0)` | 9908 | 74.029509 | 2584 |
| `ST1` station tail store duplicated | 9908 | 74.029509 | 2584 |
| `STPT` ST1 + PT1 | 9908 | 74.029509 | 2584 |
| `OL_orchain` `otherLock = a \|\| b \|\| c \|\| d` | 9904 | 70.549419 | 2459 |
| `OL_or2` same with `if (!otherLock)` | 9904 | 70.549419 | — |
| `OLC_V2` OL_orchain + V2 tail | 9904 | 70.549419 | — |
| `OLC_V2R` OL_orchain + V2R | 9928 | 70.758640 | 2467 |
| `OLC_V2R_S` OLC_V2R + `String label` | 9960 | 70.707939 | — |
| `OL_early` `otherLock = field_0x14 != 0` at top + `if (!(AND)) = 1` | 9844 | 70.169394 | — |
| `OL_early2` same with positive-OR body | 9928 | 69.754768 | — |
| `OL_bool` `bool otherLock` | 9880 | 67.853042 | — |
| `OL_char` `char otherLock` | 9908 | 74.029509 | 2584 |
| `RC_full` recompute written as the full `alienOrbit` if/else | 9920 | 72.222222 | — |
| `AS_EN` baseline + Ghidra asteroid + distance-first in-sight | 9916 | 67.105828 | — |
| `H1` outer expect 0→1 | 9912 | 70.915877 | — |
| `H2` inner expect 1→0 | 9900 | 71.692705 | — |
| `H3` both | 9912 | 70.915877 | — |
| `H4`/`H5` extra expects on the landmark on-screen chain | 9860 | 66.102667 | — |
| `EX1..EX3` move the single landmark expect to term 3/4/5 | 9908 | 74.029509 | 2584 |
| `OL_top`/`OL_top0`/`OL_top1` `otherLock` declared at function top | 9908 | 74.029509 | 2584 |
| `SLOT1`/`SLOT2` `planetHit` declared at the top (before/after turretMode) | 9908 | 74.029509 | 2584 |
| `C_pt`/`C_st`/`C_ptst`/`C_ptst_char`/`C_all` neutral-edit combinations | 9908 | 74.029509 | 2584 |
| `RC_nostore` recompute without `field_0x1a8 = onPlanetCourse` | 9876 | 65.719208 | — |
| `RC_short` recompute without the `goingToPlanet`/`isDockingToPlanet` terms | 9880 | 66.303100 | — |
| `V2R_pad` V2R + one extra call in the recompute | 9964 | 70.253255 | — |

## 3. Why the structural fix loses: `otherLock` / `status` register split

The original's allocation at the first `otherLock` chain is:

```
2c2 movs r5,#1        ; otherLock in r5
2c4 ldr.w r0,[r4,#312]
2c8 cbz r0,@0322
2ca ldr r6,[sp,#156]  ; status re-materialised into r6
```

i.e. `status` is spilled to `[sp,#156]` early, kept in **r5** only until the
chain, then re-materialised into **r6** so `otherLock` can take r5. The
baseline instead keeps `status` in r5 for the whole function and gives
`otherLock` r9, which forces `mov.w r9,#1` (4 B) and a duplicated set block.

`OL_orchain` (`otherLock = a || b || c || d`, a single OR expression — the
same shape as the second chain) **does** reproduce the original register
assignment (`otherLock` r5, `status` r6) and the second chain's `cbnz`
shape, but the allocator then keeps `status` live in r6 from the very top
instead of spilling it:

```
original 30: ldr r0,[r0,#0]; str r0,[sp,#156]; ldr r0,[r0,#0]; blx getMission
OL_orchain 4e: ldr r6,[r0,#0]; ldr r0,[r6,#0]; blx getMission
```

That costs 2 instructions in the prologue and re-colours the whole function;
score 70.55. The same split is why `V2`/`V2R` lose 4-6 points in the
300-600 region (planet loop body): without the recompute the allocator moves
`status` from r5 to fp and the loop counter from fp to sl.

## 4. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 % /
2584 matched / 3492-3489**. This wave's new results:

1. The planet-tail block placement is controlled by the
   `else if (field_0x14 == 0)` store order. Duplicating the
   `planetLockTimer = 0` store (V2) makes the tail end in `b enemies` and
   reproduces the original's `[cold autopilot][planet tail][landmark]`
   order for the first time without the invented recompute — tail at 0x6ec
   (original 0x6f8), landmark loop at 0x764 (original 0x784).
2. The fix is still a net loss because deleting the recompute changes the
   global register allocation (`status` r5→fp, loop counter fp→sl). Keeping
   the recompute after the tail (V2R) restores the tail at 0x6fa but the
   recompute is then placed cold after the landmark loop; 70.27.
3. The single-expression `otherLock = a || b || c || d` form is the only
   source edit found that gives the original's `otherLock`=r5 / `status`=r6
   assignment, but it also makes `status` live in r6 from the top, costing
   the prologue; 70.55.
4. The station-tail store duplication is score-neutral.

A byte-faithful pass would need (a) the V2 tail store order, (b) the
`otherLock` OR-chain, and (c) a way to keep `status` spilled to `[sp,#156]`
until 0x2ca — the last is not reachable by ordinary source edits at this
granularity with clang 7 -Oz.

## 5. Scratch

All scratch under `/dev/shm/w6606` (not committed): `Radar.base.cpp`
(pristine baseline source), `score.py`, `try.sh`, `regions.py`, `hunk.py`,
`dr.py`, `mm2.py` (50-instruction match map), `dump2.py`, `idx2addr.py`,
`base.so`/`v2.so`, `rea.json`/`rea.c` (Ghidra), and every variant listed
above (`V2`, `V2R`, `V2R_S`, `V2R_SA`, `V2_S`, `V2E`, `V2P`, `B2v`, `PT1`,
`ST1`, `STPT`, `OL_orchain`, `OL_or2`, `OLC_V2`, `OLC_V2R`, `OLC_V2R_S`,
`OL_early`, `OL_early2`, `OL_bool`, `OL_char`, `OL_top*`, `RC_full`,
`AS_EN`, `H1..H5`, `EX1..EX3`, `SLOT1`, `SLOT2`).
