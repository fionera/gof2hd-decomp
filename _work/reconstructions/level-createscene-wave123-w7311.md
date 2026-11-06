# `Level::createScene` — wave-123 w7311: `&Globals::rnd` / `&Globals::status` census, no source win

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w7311`. Target
`_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`). No source change
landed. Baseline and best remains the landed source: **92.757306 % /
2252 B**, 783/791 instructions, gate **byte 4506 / linked 4506**, lint
CLEAN, drift 0.

This note corrects one reading of the wave-121/122 census and records
the exact reason the two remaining basins cannot be bridged from source.
It also records a *new* near-miss shape (constructor temporary) that
reproduces the original's parts-loop byte-for-byte but loses the RA.

## TL;DR

* The original keeps **two** address-of-global values in callee-saved
  registers for the whole body:
  * `sl` = `&Globals::status` (loaded at `b433e`, reloaded `[sl]` at
    `b4342`, `b4652`, `b474c`, `b4772`, `b4780`),
  * `r8` = `&Globals::rnd` (loaded at `b492c`, reloaded `[r8]` at all
    eight `nextInt` sites `b4936 b49ae b49be b49ca b49f6 b4a46 b4a56
    b4ad8`).
  Neither is a local copy; both are GOT-loaded *addresses* whose value
  is re-loaded after every call. Wave-122 said "`sl` = status"; it is
  really `&Globals::status`.
* The landed source's local `AERandom *rnd` value in `r5` is
  **load-bearing for the early allocation** (`&Globals::status` ends up
  in `sl`), but it is exactly what pushes `race` into `r6` and hence
  evicts `capacity`. Removing it (`AGT`) fixes `&Globals::rnd`'s home
  but spills `&Globals::status` and rotates `{capacity, &rnd, i}`.
* `g->setRotation(Vector(0.0f, 3.1415927f, 0.0f))` reproduces the
  original's parts loop **exactly** (8 instructions, `add r1, sp, #56`,
  three separate stores in `y,x,z` order) but shifts the whole RA from
  instruction 460 on: 85.0 % / 2248 B.
* ~90 new variants measured. **None beat 92.757306 % at 2252 B.**

## 1. The two reachable ship-loop basins, exactly

Target map (original, from `orig_cs_full.txt`):

| value | original |
|---|---|
| `spotUsed` | r4 |
| `race` | r5 |
| `capacity` | r6 |
| `&Globals::rnd` | r8 |
| `i` | r9 |
| `&Globals::status` | sl |
| `type` | fp |
| `&Globals::status` spill | `[sp,#40]` (only around the parts loop) |
| `spots` | `[sp,#44]` |
| `sceneRace` | `[sp,#52]` |
| `this` | `[sp,#48]` |

| variant | capacity | &rnd | spotUsed | race | type | i | &status |
|---|---|---|---|---|---|---|---|
| **landed** (trick + local `rnd`) | fp | `[sp,#44]` | r4 | r6 | r8 | r9 | sl |
| **AGT** (all-global + trick) | r8 | r9 | r4 | r5 | fp | `[sp,#44]` | `[sp,#40]` |
| **AGN** (all-global + natural) | r6→r8 | r8→r9 | r4 | r5 | fp | r6 | `[sp,#40]` |
| **geo2** (AGT + `geo` before `setAlwaysFriend`) | sl | r8 | r4 | r5 | fp | r9 | r6/`[sp,#40]` |

`AGT` is a **pure 3-way rotation**: `{r6,r8,r9}` holds
`{capacity,&rnd,i}` in the original and `{i,capacity,&rnd}` in `AGT`,
plus `&Globals::status` is spilled instead of living in `sl`.
`geo2` is the dual: `&rnd`, `spotUsed`, `race`, `type`, `i` are all
correct; only `capacity` (sl) and `&Globals::status` (r6) are
transposed.

## 2. Why `capacity` leaves `r6` (greedy-RA trace)

`llc -debug-only=regalloc` on the createScene-only module
(`/dev/shm/w7311/v/map.sh`). `capacity` is **always assigned `r6`
first** (it is the earliest long-lived interval) and then evicted by a
hotter loop value:

* **landed**: `%46` (`race`, w = 2.7946e-2) evicts `%27`
  (`capacity`, w = 2.4608e-2) from `r6`; `capacity` → `r11` (fp).
  `%34` (the local `rnd` value, w = 2.9758e-2) is what had earlier
  evicted `%3` (`&Globals::status`) from `r5` and pushed it to `r10`
  (`sl`). So the local `rnd` both *creates* the `sl` home for
  `&Globals::status` and *destroys* the `r6` home for `capacity`.
* **AGT / AGN**: `%492` (`ship->geometry` load, interval
  `[11392,11600)`, w = 2.5740e-2) evicts `%27` (`capacity`,
  w = 2.4466e-2) from `r6`; `capacity` → `r8`, which evicts `%556`
  (`&Globals::rnd`) → `r9`.
* **geo2**: `%562` (w = 4.1391e-1) evicts `%27` from `r6`; `capacity`
  → `r10` (`sl`), `&Globals::status` → `r6`.

`weight = UseDefFreq / (Size + 25*NumInstr)`; `shouldEvict` is a plain
`A.weight > B.weight` (no hysteresis). `capacity` loses by 3–5 % in
every basin.

## 3. New near-miss: constructor temporary

`g->setRotation(Vector(0.0f, 3.1415927f, 0.0f));` (and
`Vector rot(0.0f, 3.1415927f, 0.0f); g->setRotation(rot);`) emits the
**original's parts loop verbatim**:

```
b4860: ldr   r0, [pc, #804]
b4862: add   r1, sp, #56
b4864: str   r0, [sp, #60]     ; rot.y = pi
b4866: movs  r4, #0            ; (original: mov.w sl, #0)
b4868: mov   r0, r5
b486a: str   r4, [sp, #56]     ; (original: str.w sl, [sp, #56])
b486c: str   r6, [sp, #44]     ; extra spill
b486e: str   r4, [sp, #64]     ; (original: str.w sl, [sp, #64])
b4870: blx   setRotation
```

i.e. the exact store order `y, x, z`, the early `add r1, sp, #56`, and
no `strd`. The only residual is the zero register (`r4` instead of
`sl`) and one extra `str r6, [sp, #44]` spill. But the absence of the
`ptrtoint/sub/inttoptr` trio changes the greedy order, and the first
real divergence moves to instruction 460 (`add.w r9` vs `add.w r8`,
the `kHangarMeshes` base). Result: **85.0 % / 2248 B**.

The trick form
(`Vector *rp = (Vector *) ((char *) &rot - (size_t) g);`) is the only
source shape found that reproduces the *early* allocation; it pays for
it with the `subs r1, r0, r5` + `strd` in the parts loop.

## 4. Variants measured this session (all no-win)

Each compiled with the repo's exact NDK r18b `-Oz` flags; score is the
unrounded `difflib` ratio.

| variant | pct | size |
|---|---|---|
| **landed (trick + local `rnd`, `y,z,x`)** | **92.757306** | **2252** |
| `t_yxz` / `t_xzy` / `t_zxy` | 92.630241 | 2252 |
| `t_xyz` | 88.310038 | 2252 |
| `t_zyx` | 88.182973 | 2252 |
| `geo` before `setAlwaysFriend` / before `setToSleep` / inline ptr | 92.757306 | 2252 |
| `landed` + `geo` before `player` | 88.9 | 2248 |
| `landed` + `geo` + `angle` | 88.9 | 2248 |
| `Vector(0,pi,0)` temp / `Vector rot(0,pi,0)` / `{}` + `rot.y` | 85.0 / 85.0 / 87.9 | 2248 / 2248 / 2252 |
| `nat_yxz` (natural `y,x,z`) | 84.9 | 2248 |
| `AGT` + `geo` before player | 79.8 | 2228 |
| `AGT` + `angle` temp | 79.5 | 2224 |
| `AGT` (all-global + trick) | 75.4 | 2232 |
| `AGN` (all-global + natural) | 75.0 | 2228 |
| `AERandom *&rnd` / `AERandom **rndp` (all sites) | 75.4 | 2232 |
| `AERandom *volatile rnd` / `*const volatile` | 77.6 | 2240 |
| `const AERandom *rnd` / `volatile AERandom *rnd` / `auto` | 92.757306 | 2252 |
| `rnd` at all 8 sites | 86.6 | 2240 |
| `rnd` declared at function top | 79.8 | 2228 |
| `rnd` declared before `capacity` / `shipCount` | 70.8 | 2236 |
| `rnd` declared before `spotUsed` / zero loop | 76.6 / 72.9 | 2228 / 2224 |
| `rnd` declared before `spots` / before ship loop | 92.757306 | 2252 |
| `Status *status = Globals::status` local | 74.4 | 2228 |
| `PaintCanvas *canvas = Globals::Canvas` local | 65.9 | 2220 |
| `Globals *globals = Globals::globals` local | 74.1 | 2252 |
| `Level *self = this` local | 92.757306 | 2252 |
| `spots` before `spotUsed` / `capacity` / `shipCount` | 88.2 / 90.7 / 90.7 | 2252 |
| `ownHangar` after `capacity` | 90.7 | 2252 |
| `capacity` before `ownHangar` | 82.1 | 2276 |
| `int cap2 = capacity` for the retry loop | 92.757306 | 2252 |
| `ownHangar`/`spots`/`spotUsed`/`shipCount`/`race`/`type`/`spot`/`tries`/`i`/`k` type swaps (bool↔int, `unsigned`, `char*`, `bool*`, …) | 92.757306 | 2252 |
| `capacity` `const int` | 92.757306 | 2252 |
| `capacity` `unsigned` / `size_t` | 92.6 | 2252 |
| `mesh` unsigned | 75.2 | 2244 |
| `sceneRace` unsigned | 92.5 | 2252 |
| `j` unsigned | 92.6 | 2252 |
| ternary / switch rewrites of the `race = 8` and `pick` selects | 92.757306 | 2252 |
| `while` / `do` rewrites of the `tries` retry loop | 92.757306 | 2252 |
| `rp` declared before the stores / between stores | 92.757306 | 2252 |
| `(char*)`, `(size_t)`, `(intptr_t)` subtraction forms | 92.757306 | 2252 |
| `float rot[3]` + cast | 85.2 / 92.6 | 2260 / 2252 |

## 5. Remaining root cause

`capacity` must stay in `r6` while `&Globals::rnd` stays in `r8`,
`&Globals::status` in `sl`, `race` in `r5`, `type` in `fp` and `i` in
`r9`. That is all seven callee-saved registers. The landed source
already uses all seven, but with the wrong occupancy (`r5` = the
`Globals::rnd` *value*, `fp` = `capacity`, `r8` = `type`).

A landable fix needs **one** of:

1. a no-instruction live-range split of `capacity` so its spill weight
   beats `race`/`ship->geometry` in `r6`, without adding the `uxth`
   that the `unsigned short cap` split costs (+4 B, 2256 B); or
2. a source shape that keeps the *early* allocation of the landed
   source (which is byte-exact through instruction 478) while making
   the eight `Globals::rnd` sites reload through a register-held
   `&Globals::rnd` instead of a register-held value; or
3. a source shape that reproduces the original's parts loop
   (`add r1, sp, #56`, three separate `y,x,z` stores, zero in `sl`)
   *and* keeps the landed's early allocation — the constructor
   temporary does the former but loses the latter from instruction 460.

All three are RA-puzzle problems, not source-semantics problems: every
constant, call and call order in the function already matches the
binary.

## 6. Reproduction

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --show _ZN5Level11createSceneEv ...
cmake --build cmake-build-match --target check -j6   # byte 4506 linked 4506
# RA trace for the createScene-only module:
bash /dev/shm/w7311/v/map.sh   # llc -debug-only=regalloc -> ra_cs.txt
```
