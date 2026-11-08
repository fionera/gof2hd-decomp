# `Level::createShip` — wave 147 (w9315): the case-3 `this` reload is a
# `status`-in-r6 artefact; the `LODManager *lm` local removes the reload but
# shifts the whole allocation (2198 / 2760).  No landable (size-exact, better)
# source shape found; note-only.

Status: **note-only** (no `src/` change landed; master body kept).
Gate on the unchanged tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.
Worktree `wt/w9315`, base master `9c9744ca5` (wave 146).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`.  Metrics: raw same-address diff bytes
(`/dev/shm/w9315/fdiff.py`) and the committed instruction-index alignment
(`_work/reconstructions/align_diff.py`); baseline raw **1978**, aligned **486**.

## 0. Baselines reproduced

```
master (float z, float y, int x)   raw 1978  size 2764  aligned 486
iif    (int x, int y, float z)     raw 1647  size 2768  aligned 495
iii    (int x, int y, int z)       raw 2334  size 2792
iff    (int x, float y, float z)   raw 1981  size 2764  (size-exact, 3 worse)
fif    (float x, int y, float z)   raw 1982  size 2764
ffi    (float x, float y, int z)   raw 1984  size 2764
```

## 1. The capital-block difference: `fixed`/`volumes` swap + `this` in `sl`

Re-checked against a fresh `iif` build (wave-145's reading was right):

| value | original | `iif` |
|---|---|---|
| `fixed` | `r9` | `fp` |
| `volumes` | `fp` | `r9` |
| `type` | `r4` (reloaded per case end) | `r4` (reloaded per case end) |
| `race` | `r6` (reloaded at merge) | `r6` (reloaded at merge) |
| `this` | **memory `[sp,#56]`**, loaded lazily at `c071e`/`c0b5e` | **`sl`**, loaded at `c03e2`, reloaded at `c06aa`/`c0a7a`/`c0b36` |

The two are one consequence: the first `new` result (`fixed`) gets `fp` in
`iif` and `r9` in the original, and `this` takes the register the allocator
frees at `c03e2`.  `sl` is clobbered by `vmov sl,s20` (pos.x) in every race
case, so `iif` must reload `this` at the case ends.  The case-3 end is the one
that costs the +4: `ldr.w sl,[sp,#52]` before `ldr r4,[sp,#60]` (type).

## 2. New root cause: `status` is in `r6`, not `r4`

Entry maps (fresh disassembly, `iif`):

| value | original | `iif` |
|---|---|---|
| `this` | `r9` | `r9` |
| `type` | **`fp`** | `[sp,#60]` |
| `shipClass` | `r5` -> `sl` -> `r6` | `sl` -> `[sp,#48]` |
| `status` | **`r4`** | **`r6`** |
| `rnd` | `r5` | `r4` |
| `nextInt` | `r6` | `r5` |
| `jitterX` | `[sp,#28]` | **`fp`** |

The decisive difference is the `r4/r6` rotation of `status`/`nextInt`.
In the original `nextInt` dies at `c0282`, so `r6` is free at `c02b4` and
`shipClass` is moved `sl -> r6`; `sl` is then free for `empPoints` at
`c032e`; at `c03e2` all seven callee-saved registers are live
(`r4,r8,fp` coords, `r5` player, `r6` shipClass, `r9` empTime, `sl` empPoints)
so `this` is forced to memory.  In `iif`, `status` owns `r6` for the whole
function, `shipClass` is spilled at `c0330`, `empPoints` takes `fp`, and `sl`
is free at `c03e2`, so the allocator puts `this` in `sl`.

So the mission's "keep `this` in a register" is backwards: the original keeps
`this` **out** of a register (in memory) and reloads it lazily.  The fix is to
make the allocator spill `this`, which requires `r6` to be free at `c02b4`.

## 3. New hypotheses measured this wave

All full-link, one function, real TU.  New levers (not in waves 144-146):

| variant | raw | size |
|---|---:|---:|
| `iif` baseline | 1647 | 2768 |
| **`iif` + `LODManager *lm = lodManager;` before the `if`** | **2198** | **2760** |
| `iif` + `lm` + `int mp = missionPtr` | 2447 | 2804 |
| `iif` + `lm` + `AbyssEngine::AERandom *rnd` | 2311 | 2760 |
| `iif` + `lm` + `Status *st` | 2361 | 2772 |
| `iif` + `lm` + `Level *self` | 2198 | 2760 |
| `iif` + `lm` + `__builtin_expect(waypoint != nullptr,1)` | 2324 | 2768 |
| `iif` + `Status *st = Globals::status` (all uses via `st`) | 2457 | 2768 |
| `iif` + `AbyssEngine::AERandom *rnd = Globals::rnd` | 1661 | 2768 |
| `iif` + `__builtin_expect(waypoint != nullptr, 1/0)` | 2323 | **2764** |
| `iif` + `__builtin_expect(mission == 49..56, 0)` | 2005 | **2764** |
| `iif` + full profile expect set (fighter hot, capital cold, 0x9a cold, t14 cold, missionPtr hot) | 2556 | 2760 |
| `iif` + `__builtin_expect(shipClass == 0, 1)` | 2457 | 2744 |
| `iif` + `__builtin_expect(shipClass == 0, 0)` | 2366 | 2716 |
| `iif` + `__builtin_expect(shipClass == 1, 1)` | 2368 | 2744 |
| `iif` + `__builtin_expect(shipClass == 1, 0)` | 2472 | 2760 |
| `iif` + `__builtin_expect(mission == 0x9a, 0)` | 2410 | 2772 |
| `iif` + `__builtin_expect(type == 14, 0)` | 2527 | 2768 |
| `iif` + `__builtin_expect(type == 14, 1)` | 2553 | 2800 |
| `iif` + `Level *self = this` at top (lodManager+missionPtr+setLevel) | 1647 | 2768 |
| `iif` + `self` only in capital arm / only at setLevel | 1647 | 2768 |
| `iif` + `this->` explicit in capital arm | 1647 | 2768 |
| `iif` + `int x, y; float z;` declared first / at top, assigned later | 1647 | 2768 |
| `iif` + `Vector pos;` field-wise assignment | 1647 | 2768 |
| `iif` + `(float)` casts at the ctor calls instead of `pos` | 1647 | 2768 |
| `iff` / `fif` / `ffi` (size-exact type vectors) | 1981 / 1982 / 1984 | 2764 |

### 3.1 The `lm` local is the first shape that actually spills `this`
`LODManager *lm = lodManager;` immediately before `if (shipClass == 0)`
makes the case blocks use `r9` for pos.x (not `sl`) and `sl` for `volumes`;
`this` stays in memory and **there is no case-end `this` reload** (the case-3
block ends with `b.n c0b2a`, exactly like the original).  But the extra live
`lm` pointer re-rolls the entire entry map (`type -> sl`, `shipClass -> fp`,
`status -> r6`) and the body shrinks to 2760.  Raw 2198 > master 1978, so it
is not landable.  This is the structural proof that removing the reload is
possible, but not by an alias that leaves the rest of the map intact.

### 3.2 `__builtin_expect` (branch weights) changes size, not the map
The wave-146 `!prof` finding applies: expects change block layout / size
(`waypoint` and `mission==49..56` expects both land exactly at 2764), but
none of them moves `status` out of `r6`, so the `this` reload survives and the
raw stays > 1978.  A source producer of the original's branch weights is still
missing.

## 4. Conclusion / open item

The residual is the `r4`/`r6` rotation of `status`/`nextInt` at entry.
Every source lever tried (locals, aliases, coordinate types/order, declaration
order, expects) either coalesces away or re-rolls the whole map.  The only
shape that spills `this` (`lm` before the `if`) overshoots to 2760 and 2198.
A source shape that puts `status` in `r4` (so `nextInt` gets `r6`, dies early,
and `shipClass` can move `sl -> r6`) would reproduce the original entry map and
make `iif` land at 2764; not found.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9315/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w9315/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
```
Scratch `/dev/shm/w9315/`: `fdiff.py`, `align_diff.py`, `try.py` (replace body
+ rebuild + score), `sbs.py` (side-by-side aligned disassembly), `gen*.py`
(variant generators), `iif_full.asm`, `orig_*.asm`, `pre_ra.txt`/`iif_ra.txt`
(`-mllvm -print-before-all` / `-print-after-all` for the target function).
