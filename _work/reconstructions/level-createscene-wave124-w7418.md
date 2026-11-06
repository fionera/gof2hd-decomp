# `Level::createScene` — wave-124 w7418: natural parts loop + `rnd` reference + `cap` split + `geo` hoist

**Status:** source win landed, 2026-11-06. Worktree `wt/w7418`. Target
`_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`, `createScene` only).
Baseline (wave-123 landed source): **92.757306 % / 2252 B**, 783/791
normalized instructions. New source: **93.554563 % / 2252 B**, 783/784
instructions, first structural diff moved to normalized instruction 9.
Full gate stays green: **byte_exact 4506 / linked_exact 4506**, avg
**99.94 %**, data **843/843**, imports **207**, lint CLEAN, drift 0,
symbol parity 5370/5370, `.ARM.exidx` byte-identical, `.ARM.extab`
size/base exact.

## TL;DR

The winning shape is the natural parts-loop `Vector` construction combined
with an `AERandom *&rnd` reference and a 16-bit `cap` split of `capacity`:

* natural `Vector rot; rot.x = 0; rot.y = pi; rot.z = 0; g->setRotation(rot);`
  (replaces the wave-121/122/123 `ptrtoint/sub/inttoptr` trick),
* `AbyssEngine::AERandom *&rnd = Globals::rnd;` declared before `capacity`
  (all eight `nextInt` sites go through the reference),
* `unsigned short cap = (unsigned short) capacity;` used only by the two
  spot/retry `nextInt` calls (`shipCount` still uses `capacity + 1`),
* `AEGeometry *geo = ship->geometry;` hoisted before
  `setAlwaysFriend(true)` and reused for the final rotation.

The `cap` split is load-bearing for the improved RA basin (it moves
`&Globals::rnd` from `r9` into `r8` and `race` into `r5`), but it costs one
`uxth`; the natural parts form and the `geo` hoist offset the size back to
2252 B. The residual is still Greedy-RA stack-slot/register ordering, not
semantics: every constant, call and call order already matches the binary.

## 1. Score and gate

| metric | before | after |
|---|---|---|
| `createScene` fuzzy | 92.757306 % | **93.554563 %** |
| size | 2252 B | 2252 B |
| normalized insns | 783 / 791 | 783 / **784** |
| first structural diff | — | instruction 9 |
| `byte_exact` | 4506 | 4506 |
| `linked_exact` | 4506 | 4506 |
| avg | 99.94 % | 99.94 % |
| data exact | 843/843 | 843/843 |
| imports | 207 | 207 |

Section deltas (all size/base exact, no section moves): `.text`
**87,210** differing bytes (was 87,014; the whole delta is inside the
2252 B `createScene` body), `.ARM.exidx` **0** (byte-identical),
`.ARM.extab` **1,446** (size exact, 156,312 B), `.rodata` **0**,
`.rel.plt` **4**, `.bss` **0**, `.note.gnu.build-id` **20**. File size
exact (2,187,328 B). `docs/IDENTICALITY.md` and `docs/VALIDATION.md`
updated for the new `.text` delta and the 92.8 -> 93.6 score.

## 2. Exact source change (`src/game/world/Level.cpp`)

Parts loop (`missionPtr == 23`):

```cpp
Vector rot;
rot.x = 0.0f;
rot.y = 3.1415927f;
rot.z = 0.0f;
g->setRotation(rot);
```

instead of the previous address trick:

```cpp
Vector rot;
rot.y = 3.1415927f;
rot.z = 0.0f;
rot.x = 0.0f;
Vector *rp = (Vector *) ((char *) &rot - (size_t) g);
g->setRotation(*rp);
```

Ship loop preamble:

```cpp
AbyssEngine::AERandom *&rnd = Globals::rnd;
int capacity = kHangarCapacity[sceneRace];
unsigned short cap = (unsigned short) capacity;
int shipCount = rnd->nextInt(capacity + 1);
```

and the two spot/retry sites:

```cpp
int spot = rnd->nextInt(cap);
for (int tries = 100; tries != 0 && spotUsed[spot] != 0; tries--)
    spot = rnd->nextInt(cap);
```

Final rotation:

```cpp
AEGeometry *geo = ship->geometry;
ship->player->setAlwaysFriend(true);
ship->setToSleep();
geo->setRotation(0.0f, (float) rnd->nextInt(300) / 100.0f, 0.0f);
```

All other `Globals::rnd` uses go through the `rnd` reference.

## 3. Evidence

First structural diff (`/dev/shm/w7418/show_cand.txt`):

```
  str  r0, [sp, #36]      |   str  r0, [sp, #40]
```

then a stack-slot/register-allocation cascade. Later the candidate carries
one extra instruction:

```
  mov.w r9, #0            |   uxth r4, r4
```

i.e. the `unsigned short cap` split emits the `uxth` that the original does
not have, but its shortened `capacity` live range is what moves
`&Globals::rnd` into `r8` (matching the original) and `race` into `r5`.

## 4. Variants measured (all no-win unless noted)

| variant | pct | size |
|---|---|---|
| **nat_xyz + ref + geo_player + ushort cap (landed)** | **93.554563** | **2252** |
| nat_xyz + ref + inline + ushort cap | 89.923469 | 2256 |
| nat_xyz + ref + geo_sleep + ushort cap | 89.795918 | 2256 |
| nat_xyz + ref + angle + ushort cap | 90.932312 | 2252 |
| trick_xyz + local + angle + int | 90.978399 | 2252 |
| trick_xyz + local + inline + int | 88.310038 | 2252 |
| trick_yzx + local + inline + int (wave-123 baseline) | 92.757306 | 2252 |
| `int cap = capacity` (collapses the split) | 80.305927 | 2228 |

`AERandom *rnd` value pointer is worse; `AERandom **rndp` is equivalent to
the chosen reference form. `Status *&status` / `Status **statusp` forms did
not change the score.

## 5. Remaining root cause

`capacity` must stay in `r6` while `&Globals::rnd` stays in `r8`,
`&Globals::status` in `sl`, `race` in `r5`, `type` in `fp` and `i` in `r9`.
The landed source now matches the original on `&Globals::rnd`, `race`,
`type` and `&Globals::status`; the residual is stack-slot ordering (first
diff `[sp,#36]` vs `[sp,#40]`) plus the single extra `uxth`. Closing it
needs a source shape that splits `capacity`'s live range **without** the
`uxth` instruction (or otherwise keeps `capacity` in `r6` while the loop
values keep their original homes). This is an RA-puzzle problem, not a
semantic one.

## 6. Reproduction

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --show _ZN5Level11createSceneEv ...
cmake --build cmake-build-match --target check -j6   # byte 4506 linked 4506
```
