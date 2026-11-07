# Radar::draw wave 136 (w8110) — new live-range / nesting probes on the `stations`/`turretMode`/`otherLock` fixed point (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w8110`, based on `master`
(`2e51d765d`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline official fuzzy **75.4** (`tools/verify/verify.py --show`, exact
`75.4000`). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave135-w7774.md`, `radar-draw-wave134-w7753.md`,
`radar-draw-wave131-w7712.md`.

This wave did **not** re-run the wave-135 declaration-placement sweep. It
attacked the same three levers from the *IR/block-structure* side, with a new
set of hypotheses the earlier waves had not tried:

1. the landmark guard as **nested `if`s** rather than one `&&` condition
   (the original IR is a nested then-block, not a flattened `&&`);
2. `Status *status` **declared before** the guard but **assigned after** it
   (a live range that starts at the join, unlike `status_before_guard`);
3. hoisting `this->planetTargets` / `this->level->getPlayer()` / `this->enemies`
   into locals across the planet section, to steal `r6` from `stations`;
4. reordering the `inSight` crosshair/length condition (the largest single
   diff block, 36 instructions at `0x12ef88`);
5. `turretMode`/`alienOrbit`/`planetHit`/`landmarkHit` type and
   function-scope placement, to move the `[sp,#148]`/`[sp,#152]` slots.

Every variant was built and scored with the real verifier normalisation. All
tie at 75.4000 or lose; the baseline remains a strong local optimum.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w8110
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w8110` (not committed): `Radar.base.cpp` (pristine
source), `vscore.py` (single-symbol scorer importing
`tools/verify/verify.py`, reproduces 75.4000 exactly), `guard.py` (dumps the
`0x12e2fc..0x12e344` guard region), `addrblocks.py` (difflib opcode map with
addresses), `gen*.py` (variant generators), `orig.fn`/`ours.fn` (full Thumb
objdump with addresses).

## 2. Where the residual actually is (new address map)

`addrblocks.py` over the exact verifier-normalised streams gives the top
non-equal blocks (orig address / ours address / instruction count):

| # | orig | ours | n | region |
|---|---|---|---:|---|
| 1 | `0x12ef88` | `0x12efcc` | 36 | enemy `inSight` crosshair/length check |
| 2 | `0x12f3ba` | `0x12f3e0` | 33 | enemy-tail flag materialisation |
| 3 | `0x12ef58` | `0x12ef64` | 33 | enemy `inSight` vector-length prologue |
| 4 | `0x12f89a` | `0x12f886` | 27 | asteroid/gas tail |
| 5 | **`0x12e304`** | `0x12e304` | **27** | **first `otherLock` guard** |
| 6 | `0x12e748` | `0x12e72c` | 22 | planet-tail vs invented post-loop recompute |
| 7 | `0x12e354` | `0x12e324` | 22 | landmark station-index checks |
| 8 | `0x12e7a8` | `0x12e800` | 19 | landmark setup |

The three named levers account for blocks 5–8; the single largest block is the
enemy `inSight` region, which this wave also probed (§6).

## 3. The guard is a block-placement consequence, not a source spelling

Original (`0x12e2fc..`):

```
12e2fc mov r1, r4
12e2fe ldr.w r0, [r1, #12]!
12e302 str r1, [sp, #124]
12e304 cbnz r0, 12e312        ; lockedAsteroid != 0 -> else (fall-through)
12e306 ldr r0, [r4, #56]
12e308 cbnz r0, 12e312
12e30a ldr r0, [r4, #28]
12e30c cmp r0, #0
12e30e beq.w 12ebc2           ; all null -> cold then
12e312 movs r5, #1            ; else: otherLock = 1  (r5, 2 B)
12e314 ldr.w r0, [r4, #312]   ; join: landmarks
12e318 cbz r0, 12e372
12e31a ldr r6, [sp, #156]     ; status GOT -> r6, AFTER the join
```

Ours:

```
12e2fc mov r1, r4
12e2fe ldr.w r0, [r1, #12]!
12e302 str r1, [sp, #124]
12e304 cbz r0, 12e324         ; lockedAsteroid == 0 -> check others
12e306 mov.w r9, #1           ; else: otherLock = 1  (r9, 4 B)
12e30a ldr r5, [sp, #156]     ; status GOT -> r5, INSIDE the else
12e30c b.n 12e338
12e324 ldr r0, [r4, #56]
12e326 ldr r5, [sp, #156]     ; status GOT -> r5, in the other pred
12e328 cmp r0, #0
12e32a itt eq
12e32c ldreq r0, [r4, #28]
12e32e cmpeq r0, #0
12e330 beq.w 1305fa
12e334 mov.w r9, #1
```

The `itt` is downstream of the **status GOT reload being sunk into both
predecessors**: the else block is 6 B (not 2 B), so it cannot be the
fall-through, so clang predicates the `b && c` pair. `otherLock` lands in `r9`
(32-bit `mov.w`) instead of `r5` (16-bit `movs`).

The original's `r5` is then **clobbered by the status GOT load**, which is why
`notOtherLock` is materialised at the landmark setup (`eor r0,r5,#1;
str r0,[sp,#116]` at `0x12e7c8`). Ours keeps `otherLock` in `r9` (never
clobbered) and folds `!otherLock` into `cmp r9,#0` at the loop use
(`0x12e874`). So the guard shape, the `notOtherLock` materialisation and the
`otherLock` register are one chain.

## 4. New variant matrix (all single edits on the pristine baseline)

### 4.1 Landmark guard as nested `if`s (new)

The original IR (`Radar.ll`, block 268/274) is `if (landmarks != null) { load
status; if (!(station checks)) { loop } }`, not a flattened `&&`. Rewriting the
source that way is byte-identical:

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | one `&&` condition | **9908** | **75.4000** |
| `n2_nested_direct` | `if (landmarks) { if (!(checks)) { ... } }` | 9908 | 75.4000 (tie) |
| `n3_nested_positive` | nested, positive `!=109 && !=110 && !(...)` | 9908 | 75.4000 (tie) |
| `n1_nested_status` | nested + `Status *status` inside the outer if | 9908 | 70.5000 |

So block structure is not the lever; the flattened `&&` already lowers to the
same IR.

### 4.2 `Status *status` live-range placements (new)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | no local | **9908** | **75.4000** |
| `z1_status_decl_before` | `Status *status;` before guard, `status = Globals::status;` after guard | 9908 | 75.1000 |
| `z2_status_mixed` | local after guard, first check still `Globals::status` | 9908 | 75.1000 |
| `b1_status_after_guard` | `Status *status = Globals::status;` right after guard | 9908 | 75.1000 |
| `s1_status_before_guard` | local before guard (wave-135 `status_before_guard`) | 9896 | 73.4000 |
| `p4_status_planet` | local only for the planet section | 9908 | 75.4000 (tie) |

`b1` is the interesting one: it **does** move the status GOT load to `r6`
after the landmark check (matching the original's `ldr r6,[sp,#156]`), but the
guard is still `cbz`+`itt` and `otherLock` is still `r9`, so the net is
−0.3 points. The GOT-load placement and the `otherLock` register are
independent in `b1` but coupled in the original.

### 4.3 Forcing `stations` out of `r6` (new)

The original keeps `stations` in `sl` and spills it to `[sp,#116]`
(`mov sl,r0; str sl,[sp,#116]`), reusing `sl` as the campaign/freelance
scratch. Ours keeps `stations` in `r6` and backs it up in `sl`
(`mov r6,r0; mov sl,r6`). To steal `r6` from `stations` I hoisted locals that
are live across the planet section:

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | — | **9908** | **75.4000** |
| `r1_pt_local` | `Array<KIPlayer*> *planetTargets = this->planetTargets;` before the planet `if`, used in the loop | 9908 | 67.8000 |
| `r2_ego_local` | `PlayerEgo *ego = this->level->getPlayer();` before the planet `if` | 9908 | 73.2000 |
| `r3_both` | both | 9908 | 67.7000 |
| `y4_enemies_local` | `enemies` local before the guard | 9908 | 75.4000 (tie) |
| `y5_level_local` | `Level *level` local before the guard | 9908 | 75.4000 (tie) |
| `p1_system_local` | `SolarSystem *system` local, reused for `getStations`/`getWarpGateEnumIndex` | 9908 | 70.8000 |
| `p3_stationdata` | `int *stationData = stations->data();` used everywhere | 9908 | 71.4000 |
| `p5_stations_const` | `Array<int> *const stations` | 9908 | 75.4000 (tie) |

No local steals `r6` without also re-rolling the loop; `stations` stays in
`r6`. The `camp_reload` control (campaign `isTarget` via `stations->data()[i]`)
is 73.2000, confirming wave 134's circular fixed point.

### 4.4 `turretMode` / `planetHit` slot swap (new type/scope spellings)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | `[sp,#148]=planetHit`, `[sp,#152]=turretMode` | **9908** | **75.4000** |
| `x1_swap_tm_ao` | swap `turretMode`/`alienOrbit` declarations | 9908 | 75.2000 |
| `x3_tm_after_campaign` | `turretMode` after `campaign` | 9908 | 75.3000 |
| `x4_ph_func` | `planetHit` at function scope | 9908 | 75.4000 (tie) |
| `cc_both_bools_top` | `planetHit`+`landmarkHit` at function scope | 9908 | 75.4000 (tie) |
| `dd_ph_lh_top` | `planetHit` before `landmarkHit` at function scope | 9908 | 75.4000 (tie) |
| `x2_tm_int` | `int turretMode` | 9908 | 67.5000 |
| `x5_ao_int` | `int alienOrbit` | 9908 | 75.3000 |

The `[sp,#148]`/`[sp,#152]` swap is a `StackColoring` output; every
declaration/scope spelling is a tie or a loss. Adding the two bools at
function scope does **not** shift the slots.

### 4.5 `inSight` region (largest single diff, new)

Original order at `0x12ef58`: `AEMath::mi` -> `VectorLength` -> compare
`s20` (60000) -> `screenX > cx-f` -> `screenX < cx+f` -> `screenY > cy-f` ->
`screenY < cy+f`. Ours does the screen checks first and the length last.

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | screen checks then length | **9908** | **75.4000** |
| `q1_len_first` | length first, original screen order | 9908 | 65.4000 |
| `q2_len_first_xxyy` | length first, x>,x<,y>,y< | 9908 | 68.8000 |
| `q3_xxyy` | screen x>,x<,y>,y< then length | 9908 | 72.2000 |
| `w1_nested_len` | nested `if (length) { if (screen) ... }` | 9908 | 65.6000 |
| `w3_lenand_xxyy` | `bool inSight = length && x>,x<,y>,y<` | 9908 | 65.7000 |
| `u1_no_deadstore` | drop the `enemy->field_0x73 = 0;` | 9908 | 64.4000 |

The original's machine order is **not** recoverable by source order: putting
the length first re-rolls the whole enemy loop and loses 7–10 points. Note the
`field_0x73 = 0` store is *not* dead in either binary (both keep two stores),
so `u1` is a real regression.

## 5. Root cause (unchanged, with the exact register mapping)

1. `otherLock` in `r5` (original) vs `r9` (baseline), with the status GOT in
   `r6` vs `r5`. This drives the first guard's `cbnz` chain vs `cbz`+`itt`,
   the `movs r5,#1` vs `mov.w r9,#1` encoding, the first literal-pool position
   (`0x12e340` vs `0x12e310`), and the `notOtherLock` materialisation
   (`eor r0,r5,#1; str [sp,#116]` vs folded `cmp r9,#0`).
2. `stations` in `sl`+spill `[sp,#116]` with `sl` as the scratch (original)
   vs `r6`+`sl` backup with `r6` as the scratch (baseline). The scratch choice
   is circular with `stationId`/`currentStation`.
3. `turretMode`/`planetHit` slots `[sp,#148]`/`[sp,#152]` swapped, and `mode`
   at `[sp,#100]` vs `[sp,#112]` — `StackColoring` outputs.
4. the invented post-loop `onPlanetCourse` recompute (72 B) that compensates
   the original's +28 B loop body and +4 B tail.
5. the enemy `inSight` block (36+33 instructions) and the ship `inSight`
   `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

Every new live-range/nesting/type lever tried this wave either ties at
75.4000 or lands below it. The baseline remains a strong local optimum; the
residual is `MachineBlockPlacement`/`StackColoring`/greedy register allocation
inside the `0x12e2fc..0x12e376` guard region, the planet block and the enemy
`inSight` block.

## 6. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **84,960** differing bytes (size/base
exact), `.ARM.extab` **1,404** (size exact 156,312), `.note.gnu.build-id`
**20**. `.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`, `.rel.plt`,
`.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
