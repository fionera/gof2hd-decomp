# `Level::createScene` — wave-127 w7634: the wave-123 source is 4/5 on the
# ship-loop register map; the wave-124 `cap` split trades that for score

**Status:** dead-end sweep, 2026-11-07. Worktree `wt/w7634`, base
`4bbe88776`. Target `_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`,
`createScene` only). **No source change landed** — the wave-124 landed source
remains the best at **93.554563 % / 2252 B**, 783 original / 784 ours
normalized instructions. Full gate re-measured green: **byte_exact 4506 /
linked_exact 4506**, avg 99.94 %, data 843/843, imports 207, lint CLEAN,
drift 0, symbol parity 5370/5370.

Follow-up to `level-createscene-wave126-w7611.md` (which followed
`…-wave125-w7517.md`, `…-wave124-w7418.md`, `…-wave123-w7311.md`).

## TL;DR

The wave-126 note framed the residual as a closed cycle rooted at
`ownHangar`. This pass re-checked the *pre-wave-124* source (commit
`897519820^`, the wave-123 landed shape) and found something the earlier
notes did not record:

* The **wave-123 source** (`ptrtoint/sub/inttoptr` Vector trick, cached
  `AERandom *rnd` value, **no** `cap` split, **no** `geo` hoist) has the
  ship-loop map **`spotUsed=r4, ownHangar=r4, &status=sl, &rnd=r8`** —
  i.e. **four of the five** target values are already correct. Only
  `capacity` is wrong (`fp`/`r11` instead of `r6`).
* The **wave-124 source** (natural Vector, `AERandom *&rnd`, `unsigned
  short cap`, `geo` hoist) raises the fuzzy score to 93.554563 % but its
  map is `capacity=r4, spotUsed=r6, ownHangar=r5, &status=sl` — three of
  the five target values are wrong, plus the extra `uxth`.

So the wave-124 `cap` split is load-bearing for the *score* basin but it
is exactly what destroys the wave-123 register map. Reverting the four
wave-124 changes one at a time shows the `cap` split is the dominant
lever; reverting it alone drops the function to 80.305927 % / 2228 B.

## 1. Exact maps

Original (`0xb42ec`, verified against the raw disassembly):

| value | original |
|---|---|
| `spotUsed` | r4 |
| `race` | r5 |
| `capacity` | r6 |
| `&Globals::rnd` | r8 |
| `i` | r9 |
| `&Globals::status` | sl (spilled `[sp,#40]` around parts/ship loops) |
| `type` | fp |
| `ownHangar` | r4 (then `[sp,#24]`) |
| `spots` / `hangarPart` | `[sp,#44]` |

| source | pct | size | insns | cap | spot | own | status | rnd |
|---|---|---|---|---|---|---|---|---|
| **wave-124 (landed)** | **93.554563** | 2252 | 784 | r4 | r6 | r5 | sl | r8 |
| wave-123 (all four reverted) | 92.757306 | 2252 | 791 | fp | r4 | r4 | sl | r8 |
| revert `cap` only | 80.305927 | 2228 | 786 | sl | r4 | r4 | r4 | r8 |
| revert `geo` only | 89.923469 | 2256 | 785 | r4 | r6 | r5 | sl | r8 |
| revert Vector only | 93.044033 | 2252 | 784 | r4 | r6 | r5 | sl | r8 |
| revert `rnd` only | 74.412698 | 2256 | 792 | r4 | r9 | r5 | fp | r8 |

## 2. The full 16-combo revert matrix

Starting from the landed wave-124 source, revert each of
`V` = natural Vector → `ptrtoint/sub/inttoptr` trick, `R` = `AERandom
*&rnd` → cached `AERandom *rnd` value after the zero loop, `C` = remove
the `unsigned short cap` split, `G` = `geo` hoist → inline
`ship->geometry->setRotation`. Every row is 2252 B unless noted.

| revert | pct | size | cap | spot | own | status |
|---|---|---|---|---|---|---|
| — (landed) | 93.554563 | 2252 | r4 | r6 | r5 | sl |
| C | 80.305927 | 2228 | sl | r4 | r4 | r4 |
| G | 89.923469 | 2256 | r4 | r6 | r5 | sl |
| GC | 75.031847 | 2228 | r8 | r4 | r4 | r4 |
| R | 74.412698 | 2256 | r4 | r9 | r5 | fp |
| RC | 83.100381 | 2244 | fp | r4 | r4 | sl |
| RG | 73.730964 | 2260 | r4 | r9 | r5 | fp |
| RGC | 83.153210 | 2244 | fp | r4 | r4 | sl |
| V | 93.044033 | 2252 | r4 | r6 | r5 | sl |
| VC | 79.796048 | 2228 | sl | r4 | r4 | r4 |
| VG | 89.413265 | 2256 | r4 | r6 | r5 | sl |
| VGC | 75.428571 | 2232 | r8 | r4 | r5 | r4 |
| VR | 74.412698 | 2240 | r4 | r9 | — | r5 |
| VRC | 86.314449 | 2244 | fp | r4 | r4 | sl |
| VRG | 73.857868 | 2244 | r4 | r9 | — | r5 |
| VRGC | 86.513995 | 2248 | fp | r4 | r4 | sl |

The `RGC`/`VRC`/`VRGC` rows are the interesting ones: they keep
`spotUsed=r4, ownHangar=r4, &status=sl` (and `&rnd=r8` for `VRC`), i.e.
the wave-123 map, but `capacity` lands in `fp`. `VRC` (trick Vector +
cached `rnd` + no `cap` + `geo` hoist) is the best of these at
86.314449 %.

## 3. Why freeing `r5` does not fix `capacity`

The wave-123 note's own RA trace says: in the wave-123 source the cached
`AERandom *rnd` **value** occupies `r5`; `race` therefore takes `r6`
and evicts `capacity` to `fp`. Freeing `r5` should let `race` take `r5`
and leave `capacity` in `r6`.

But changing the cached value to the `AERandom *&rnd` **reference** in
the wave-123 source (so the address goes to `r8` and the value is
reloaded after every call) does not just free `r5`: the extra
reloads push the whole function into the 2232 B basin
(`cap=r8, spot=r4, own=r5, status=r4`, 75.428571 %). The wave-124 source
needs the `cap` split **and** the natural Vector **and** the `geo` hoist
to climb back to 2252 B / 93.55 %. So `r5`-freeing and the 2252 B basin
are coupled through the same greedy-RA queue.

## 4. Additional variants measured this pass (all 93.554563 %, identical code)

Every one of these produced byte-identical code to the landed source
(`cap=r4 spot=r6 own=r5 status=sl rnd=r8`):

* `ownHangar` as `int`, as `bool` ternary, as `&&` single expression,
  computed after `capacity` (latter 86.8 %, 2276 B).
* `shipCount` as `unsigned`, `if/else` instead of the ternary,
  `capacity < size` / `size < capacity` reversed (93.43 %), `std::min`
  (93.43 %), early `if/else` (87.09 %, 2264 B).
* `spotUsed` as `bool*`, `char*`, `(unsigned char*) new char[]`.
* `rnd` reference declared at the top of the block, after `capacity`,
  as `AERandom **rndp = &Globals::rnd; AERandom *&rnd = *rndp;`.
* `capacity` as `const int`, `unsigned int` (93.43 %),
  `kHangarCapacity[sceneRace] + 0`, `*(kHangarCapacity + sceneRace)`,
  via a `const int *capTable` local, `* 1`.
* `spots` as an `if/else` (92.27 %, 2248 B).
* `geo` before `setToSleep` (93.30 %), before `setPosition` (89.80 %,
  2256 B), after `setToSleep` (89.92 %, 2256 B), inline (89.92 %).
* `g->setRotation(Vector(0, pi, 0))` temporary (identical).
* `plainHangar` 3-compare form (85.62 %, 2264 B), `Vector rot; rot =
  Vector(...)` (81.75 %, 2264 B) — both move the `&status` spill slot to
  `[sp,#36]` (the original slot) but lose 8-12 points elsewhere.
* `missionPtr` dispatch as separate `if`s instead of `else if`
  (identical).

## 5. Root cause / next angle

The residual is still the Greedy-RA permutation of
`{capacity, spotUsed, ownHangar}` over `{r4, r6, r5}` plus the `uxth`.
The new datum is that the **wave-123 source already has
`{spotUsed, ownHangar, &status, &rnd}` correct**; only `capacity` is
misplaced (`fp` instead of `r6`). A shape that keeps the wave-123
allocation of `race`/`rnd`/`status` while keeping `capacity`'s live
range long enough to stay in `r6` (i.e. no `uxth`, no `cap` split, no
`geo` `ldrd`, but still 2252 B) would close the function. No such shape
was found across the 16-combo revert matrix and the ~40 extra variants
above.

## 6. Reproduction

Scratch is `/dev/shm/w7634`:

* `Level.cpp.base` — pristine landed `src/game/world/Level.cpp`;
* `Level.cpp.w123` — the wave-123 source (`897519820^`);
* `fast.py` — unrounded single-function `difflib` score + size/insns;
* `sig.py` — extracts the `capacity/spotUsed/ownHangar/&status/&rnd`
  register map from the preheader;
* `mk.py`, `mkw.py`, `reorder.py`, `revert.py`, `combo2.py` — variant
  runners;
* `mi.txt`, `sched.txt`, `greedy.txt`, `vr.txt` — `-mllvm
  -print-machineinstrs` dumps of the landed source
  (`-mllvm -filter-print-funcs=_ZN5Level11createSceneEv`).

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump $OBJDUMP --show _ZN5Level11createSceneEv
cmake --build cmake-build-match --target check -j6   # byte 4506 linked 4506
```
