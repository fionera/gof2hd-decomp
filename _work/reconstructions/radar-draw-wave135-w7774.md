# Radar::draw wave 135 (w7774) — declaration placement / local-caching sweep on the `stations`/`turretMode`/`otherLock` fixed point (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w7774`, based on `master`
(`bbffef482`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official 75.4). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave134-w7753.md`, `radar-draw-wave131-w7712.md`,
`radar-draw-wave129-w7673.md`.

This wave attacked the assignment's three named levers with a **new
declaration-placement / local-caching sweep** (the previous waves swept loop
forms, store order, literal pools and the `expect` spellings). The new evidence
is that the three levers are *one* coupled fixed point and that **scalar
declaration placement is a no-op**: moving `otherLock`, `landmarkHit`,
`notOtherLock`, `notTurretMode`, `planetHit`, `turretMode` or `onPlanetCourse`
around produces byte-identical code, because these are SSA values coloured into
spill slots, not source-order allocas. The only lever that moves the guard is
*whether `Globals::status` is loaded before or after the guard* — and that
change buys the original's `cbnz` chain at the cost of 12 B and 1.9 points.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w7774
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.9 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w7774` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol difflib harness reproducing 75.354839
exactly), `run.py`/`batch.py`..`batch8.py` (variant drivers), `orig.dis`/
`ours.dis` (full Thumb objdump with addresses), `sbs.py` (aligned side-by-side),
`regions.py`/`regions2.py` (difflib opcode map), `Radar.ll` (clang IR).

## 2. The exact original guard, and why ours differs

Original `12e2fc..12e318` (`objdump -d`, image base `0x10000`):

```
12e2fc mov r1, r4
12e2fe ldr.w r0, [r1, #12]!   ; lockedAsteroid, r1 = &field_0xc
12e302 str r1, [sp, #124]
12e304 cbnz r0, 12e312        ; -> else
12e306 ldr r0, [r4, #56]      ; lockedGasCloud
12e308 cbnz r0, 12e312
12e30a ldr r0, [r4, #28]      ; field_0x1c
12e30c cmp r0, #0
12e30e beq.w 12ebc2           ; -> then (cold)
12e312 movs r5, #1            ; else: otherLock = 1   (r5!)
12e314 ldr r0, [r4, #312]     ; join: landmarks
12e318 cbz r0, 12e372
12e31a ldr r6, [sp, #156]     ; status -> r6  (AFTER the join)
```

Ours `12e2fc..12e338`:

```
12e2fe ldr.w r0, [r1, #12]!
12e302 str r1, [sp, #124]
12e304 cbz r0, 12e324         ; -> check others
12e306 mov.w r9, #1           ; else: otherLock = 1   (r9!)
12e30a ldr r5, [sp, #156]     ; status -> r5  (INSIDE the else block)
12e30c b.n 12e338
...
12e324 ldr r0, [r4, #56]
12e326 ldr r5, [sp, #156]     ; status -> r5
12e328 cmp r0, #0
12e32a itt eq
12e32c ldreq r0, [r4, #28]
12e32e cmpeq r0, #0
12e330 beq.w 1305ee           ; -> then
12e334 mov.w r9, #1
```

So the `itt` is **downstream of the else block being non-trivial**: because
ours must reload `Globals::status` into `r5` in the else block (and in the
`then` path), the two remaining null tests are if-converted. The original's
else block is the 2-byte `movs r5,#1` because `otherLock` already lives in
`r5` and `status` is loaded *after* the join into `r6`.

The `movs r5,#1` vs `mov.w r9,#1` encoding is itself downstream: `r9` needs a
32-bit `mov.w`, `r5` a 16-bit `movs`.

## 3. New measurement: loading `status` before the guard reproduces the chain

Inserting `Status *status = Globals::status;` before the guard and using it in
the landmark guard (`status_before_guard`) produces the **exact original
`cbnz` chain**:

```
12e31a cbnz r0, 12e32a
12e31e ldr r0, [r4, #56]
12e320 cbnz r0, 12e32a
12e322 ldr r0, [r4, #28]
12e324 cmp r0, #0
12e326 beq.w 1305ee
12e32a mov.w r9, #1
```

but the score drops to **73.428981** (9896 B): `status` now occupies `r5`
(`ldr r5,[sp,#156]` before the guard) and `otherLock` still lands in `r9`, and
the load is hoisted onto the hot path. So the chain shape is *reachable* from
source, but only by moving the status load, which is the wrong trade.

## 4. Variant matrix (all single edits on the pristine baseline)

### 4.1 Scalar declaration placement — all byte-identical ties

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **75.354839** |
| `otherlock_decl_before_turret` | `int otherLock;` before `turretMode` | 9908 | 75.354839 (tie) |
| `otherlock_top0` | `int otherLock = 0;` at function scope | 9908 | 75.354839 (tie) |
| `otherlock_top_field` | `int otherLock = field_0x14 != 0;` at function scope | 9908 | 75.354839 (tie) |
| `otherlock_before_guard_field` | same, just before the guard | 9908 | 75.354839 (tie) |
| `landmarkHit_before_otherlock` | swap the two declarations | 9908 | 75.354839 (tie) |
| `nototherlock_before_landmark` | `int notOtherLock` before `landmarkHit` | 9908 | 75.354839 (tie) |
| `planetHit_before_turret` | `bool planetHit` before `turretMode` | 9908 | 75.354839 (tie) |
| `planetHit_bool_top` | `bool planetHit` at function scope | 9908 | 75.354839 (tie) |
| `planetHit_before_stations` | `bool planetHit` before the `stations` fetch | 9908 | 75.354839 (tie) |
| `onplanet_top` | `bool onPlanetCourse;` at function scope | 9908 | 75.354839 (tie) |
| `onplanet_before_turret` | `bool onPlanetCourse;` before `turretMode` | 9908 | 75.354839 (tie) |
| `alwaysenemy_before_missionblocks` | swap the two flags | 9908 | 75.354839 (tie) |
| `nototherlock_int_cast` | `(otherLock == 0) ? 1 : 0` | 9908 | 75.354839 (tie) |
| `turret_bool_explicit` | `isInTurretMode() != 0` | 9908 | 75.354839 (tie) |
| `canvas_local` | `PaintCanvas *canvas = Globals::Canvas;` | 9908 | 75.354839 (tie) |

### 4.2 Local caching that changes live ranges

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **75.354839** |
| `status_local` | `Status *status = Globals::status;` at function scope, all uses | 9908 | 75.096774 |
| `status_local_planet` | `Status *st` only for the `stations` fetch | 9908 | 75.354839 (tie) |
| `status_local_planet_all` | `Status *st` for the whole planet section | 9892 | 71.594661 |
| `status_before_guard` | `Status *status` before the guard, landmark guard uses it | 9896 | 73.428981 |
| `status_before_guard_all` | same + planet section uses it | 9852 | 58.826912 |
| `status_after_guard` | `Status *status` after the guard, landmark+planet uses | 9856 | 58.674716 |
| `status_before_landmark_only` | `Status *status` after `landmarkHit` | 9908 | 75.125448 |
| `level_local` | `Level *level = this->level;` for the 7 fetches | 9900 | 72.988094 |
| `ego_local` | `PlayerEgo *ego = this->level->getPlayer();` | 9860 | 69.804822 |
| `stations_const` | `Array<int> *const stations` | 9908 | 75.354839 (tie) |
| `stations_auto` | `auto *stations` | 9908 | 75.354839 (tie) |
| `stations_data_cache` | `int *stationData = stations->data();` | 9876 | 73.262493 |
| `stations_volatile` | `Array<int> *volatile stations` | 10184 | 51.063830 |
| `cr_stationdata` | `int stationData = stations->data()[i]; int stationId = stationData;` | 9908 | 75.354839 (tie) |
| `cr_freelance_local` | cache `stations->data()[i]` for the freelance test | 9908 | 75.354839 (tie) |

### 4.3 `otherLock` guard spellings (re-confirmed with the placement sweep)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** (`int`, nested expect 0) | unmodified | **9908** | **75.354839** |
| `ternary` | `cond ? 1 : (field_0x14 != 0)` | 9908 | 74.179211 |
| `ternary_exp` | same with `expect(cond,1)` | 9908 | 75.326165 |
| `init1_then` | `int otherLock = 1; if (all null) ...` | 9908 | 74.179211 |
| `init1_then_exp` | same with `expect(...,0)` | 9908 | 75.326165 |
| `or_if` | `if (a||b||c) 1 else field` | 9908 | 74.179211 |
| `or_if_exp1` | same with `expect(...,1)` | 9908 | 75.326165 |
| `or_bool` | `bool anyLock = a||b||c;` | 9908 | 74.179211 |
| `sbg_bool` | `status_before_guard` + `bool otherLock` | 9816 | 68.903932 |

### 4.4 `stations` reload / planet-section reorders

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **75.354839** |
| `camp_reload` | campaign `isTarget = stations->data()[i] == ...` | 9900 | 73.235800 |
| `cr_sid_and` | `stationId == stations->data()[i] && stations->data()[i] == ...` | 9916 | 72.120344 |
| `cr_cs_first` | `currentStation` before `stationId` + reload | 9900 | 73.264487 |
| `cr_sid_first` | reload + extra `stationId == stations->data()[i]` | 9888 | 72.012624 |
| `cr_marked_before` | `int marked` before the mission fetches + reload | 9900 | 73.235800 |
| `cr_freelance_first` | swap the two mission fetches + reload | 9868 | 71.153293 |
| `cr_warpgate_after` | `textOffset` before `warpGate` + reload | 9900 | 73.235800 |
| `cr_textoffset_first` | `textOffset` first + reload | 9900 | 73.235800 |
| `campaign_before_alien` | swap `campaign`/`alienOrbit` | 9908 | 75.268817 |
| `turret_after_campaign` | move `turretMode` after `campaign` | 9908 | 75.268817 |
| `mission_after_turret` | move the mission block after `turretMode` | 9908 | 75.125448 |
| `enemy_fetch_last` | move the `enemies` fetch last | 9908 | 75.240143 |

## 5. Why the campaign reload is the crux (and why it still loses)

The original's campaign `isTarget` really is
`stations->data()[i] == campaignMission->getTargetStation()`, not
`stationId == ...`:

```
12e474 ldr.w r0, [sl, #4]      ; stations->data()
12e478 ldr.w sl, [r0, fp, lsl #2]  ; scratch = stations->data()[i]
12e47c mov r0, r6
12e47e blx Mission::getTargetStation
12e484 cmp r8, r9              ; stationId != currentStation
12e48c cmp sl, r0              ; isTarget
12e48e ldr.w sl, [sp, #116]    ; restore stations
```

The baseline uses `stationId` (r8) and therefore never needs the reload. The
faithful `camp_reload` variant is built and dumped: it spills `stations` to
`[sp,#148]`, moves `stationId` to `r9`, `currentStation` to `r6` (spilled to
`[sp,#132]`), and uses `r8` for the scratch. That is a *different* allocation
from the original (`r8=stationId`, `r9=currentStation`, `sl=scratch`,
`stations` spilled to `[sp,#116]`), and it is 8 B shorter / 2.1 points lower.

The original's allocation is a genuine 3-way fixed point:

* the scratch can use `sl` only if `r8`/`r9`/`r6` are all live at the reload;
* `r8` is free in `camp_reload` because `stationId` moved to `r9`;
* `stationId` moved to `r9` because `currentStation` was spilled to free a
  register for the scratch;
* `currentStation` was spilled because the scratch took `r8`.

Every source spelling that removes one edge re-rolls the other.

## 6. Stack-slot layout (new measurements)

The scalar spill slots are shifted between the two binaries even though the
frame is 296 B in both:

| variable | original | ours |
|---|---|---|
| `mode` (arg) | `[sp,#100]` | `[sp,#112]` |
| `hud` (arg) | `[sp,#84]` | `[sp,#84]` |
| `player` (arg) | `[sp,#136]` | `[sp,#136]` |
| `turretMode` | `[sp,#148]` | `[sp,#152]` |
| `planetHit` | `[sp,#152]` | `[sp,#148]` |
| `otherLock` (planet section) | `[sp,#112]` | `[sp,#116]` |
| `stations` spill | `[sp,#116]` | none (`sl` backup) |
| `landmarkHit` | `[sp,#108]` | `[sp,#108]` |
| `Globals::status` | `[sp,#156]` | `[sp,#156]` |

`mode` sits 12 B higher in ours, so the stack-object order differs by one
12-byte object even though the total is exact. The `turretMode`/`planetHit`
swap is a `StackColoring` output: every declaration-order spelling tried
(§4.1) is byte-identical, confirming wave 134's finding.

## 7. Post-loop recompute (re-confirmed)

The original's planet-loop exit (`12e748`) goes **straight** to the planet-lock
tail; the `onPlanetCourse` recompute exists only once, in the cold initial
`else` block at `12e6ee` (reached from `12e1b6 beq.w 12e6ee`, jumping back to
`12e1bc`). Ours has an **invented post-loop recompute** at `12e72c`
(`12e72c..12e774`, 72 B) and the cold block at `12e6d2`. The original loop
body `12e3d4..12e746` is 882 B vs ours `12e3d4..12e72a` 854 B (+28 B), and
the original tail `12e748..12e79a` is 82 B vs ours `12e774..12e7c2` 78 B
(+4 B). Removing the recompute (`norecomp`, wave 134) re-rolls the allocation
to 9856 B / 70.76.

## 8. Root cause (unchanged, with the exact register mapping)

The residual is the coupled register-allocation / stack-coloring fixed point:

1. `otherLock` in `r5` (original) vs `r9` (baseline), with `status` in `r6`
   (original) vs `r5` (baseline). This drives the first guard's `cbnz` chain
   vs `cbz`+`itt`, the `movs r5,#1` vs `mov.w r9,#1` encoding, the first
   literal pool position (`12e340` vs `12e310`), and the `notOtherLock`
   materialization (`eor r0,r5,#1; str [sp,#116]` vs folded `cmp r9,#0`).
   The chain is reachable from source only by hoisting the `status` load
   (§3), which costs 12 B and 1.9 points.
2. `stations` in `sl`+spill `[sp,#116]` with `sl` as the campaign scratch
   (original) vs `r6`+`sl` backup with `r6` as the scratch (baseline). The
   scratch choice is circular with `stationId`/`currentStation` (§5).
3. `turretMode`/`planetHit` stack slots `[sp,#148]`/`[sp,#152]` swapped, and
   `mode` at `[sp,#100]` vs `[sp,#112]` — `StackColoring` outputs, not
   declaration-order functions.
4. the invented post-loop `onPlanetCourse` recompute (72 B) that compensates
   the original's +28 B loop body and +4 B tail (§7);
5. the enemy-tail block split and the ship `inSight` `s24`/`s26` live range
   (`vpush {d8-d13}` vs `{d8-d11}`).

Every source lever tried this wave either ties at 75.354839 or lands below it.
The baseline remains a strong local optimum.

## 9. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,022** differing bytes (size/base
exact), `.ARM.extab` **1,416** (size exact 156,312), `.note.gnu.build-id`
**20**. `.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`, `.rel.plt`,
`.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
