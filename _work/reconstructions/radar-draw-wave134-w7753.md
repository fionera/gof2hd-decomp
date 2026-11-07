# Radar::draw wave 134 (w7753) — `stations` scratch/spill fixed point, `turretMode` slot swap, first `otherLock` guard (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w7753`, based on `master`
(`ec0acea1e`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official 75.4). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave132-w7731.md`, `radar-draw-wave133-w7732.md`,
`radar-draw-wave131-w7712.md`, `radar-draw-wave129-w7674.md`.

This wave attacked the assignment's three named levers with a **new
symbol-resolved objdump** of the original (not just Ghidra pseudocode):
(1) the `stations` `r6` vs `sl`+spill allocation, (2) the
`turretMode`/`planetHit` stack-slot swap, and (3) the first `otherLock`
guard `cbz`+`itt` vs `cbnz` chain. It confirms the original's source shape
exactly, but shows the residual is a **circular register-allocation fixed
point**: the scratch register depends on where `stationId` lives, which
depends on whether `currentStation` is spilled, which depends on the scratch
register. Every source spelling that changes one edge re-rolls the other.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w7753
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w7753` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol difflib scorer reproducing 75.354839
exactly), `run.py`/`b1.py`..`b22.py` (variant drivers), `orig_addr.dis` /
`ours_addr.dis` (full Thumb objdump with addresses), `ghidra_draw.md`
(fresh Ghidra 12.1.2 decompile of `0x13e050`), `regions.py` (difflib
opcode map).

## 2. The original's planet-block source shape is now symbol-confirmed

`objdump -d` of the original at `0x12e3b8..0x12e492` (image base `0x10000`)
resolves the PLT calls:

```
12e3b8 ldr r0,[r5,#0] ; blx Status::getSystem
12e3be blx SolarSystem::getStations
12e3c6 mov sl,r0                 ; sl = stations
12e3c8 mov.w fp,#0               ; fp = i = 0
12e3ce str.w sl,[sp,#116]        ; spill stations
12e3d2 str r0,[sp,#152]          ; planetHit = 0
12e3fa ldr.w r0,[sl,#4]          ; stations->data()
12e3fe ldr.w r8,[r0,fp,lsl#2]    ; stationId = stations->data()[i]
12e40c mov r9,r0                 ; currentStation
...
12e474 ldr.w r0,[sl,#4]          ; stations->data()
12e478 ldr.w sl,[r0,fp,lsl#2]    ; stations->data()[i]  (scratch -> sl)
12e47c mov r0,r6                 ; campaignMission
12e47e blx Mission::getTargetStation
12e484 cmp r8,r9                 ; stationId != currentStation
12e48c cmp sl,r0                 ; isTarget = stations->data()[i] == getTargetStation()
12e48e ldr.w sl,[sp,#116]        ; restore stations
```

So the campaign `isTarget` is **`stations->data()[i] ==
campaignMission->getTargetStation()`**, not `stationId == ...`; the load is
hoisted above the call because it is the comparison's left operand. This
matches Ghidra and wave 132. The freelance `isTarget` at `0x12e4be` uses the
same `ldr [sl,#4]; ldr sl,[r0,fp,lsl#2]` scratch-and-restore shape.

Original register split: `r4=this`, `r5=freelance` (and `otherLock` in the
landmark region), `r6=campaignMission` (and `status` in the landmark region),
`r8=stationId`, `r9=currentStation`, `sl=stations` + scratch, spill
`[sp,#116]`, `fp=i`.

Baseline register split: `r6=stations` + scratch, `sl=stations` backup,
`r8=stationId`, `r9=currentStation`, `fp=i`. The baseline keeps the
`stations` backup in a **register** (`mov r6,sl` restores, 2 B) instead of a
**stack slot** (`ldr.w sl,[sp,#116]`, 4 B), which is exactly the +12 B of the
loop-body reloads the wave-132 accounting attributed to the `sl` allocation.

## 3. The circular fixed point (new)

The faithful `camp_reload` source (campaign `isTarget` via
`stations->data()[i]`) is built and dumped. Its loop body is:

```
0003a8 ldr r0,[sp,#148]          ; stations (spilled!)
0003ac ldr.w r9,[r0,fp,lsl#2]    ; stationId -> r9
0003ba mov r6,r0                 ; currentStation -> r6
0003ca str r6,[sp,#132]          ; currentStation spilled
000424 ldr r0,[sp,#148]
000426 ldr r0,[r0,#4]
000428 ldr.w r8,[r0,fp,lsl#2]    ; scratch -> r8
00042e blx Mission::getTargetStation
000438 cmp r9,r2                 ; stationId != currentStation
00043e cmp r8,r0                 ; isTarget
```

The allocator spills `stations` and `currentStation`, and uses `r8` for the
scratch. The original instead keeps `stations` in `sl`, spills it, and uses
`sl` for the scratch. The tie is circular:

* the scratch can use `sl` only if `r8`/`r9`/`r6` are all live at the load;
* `r8` is free in `camp_reload` because `stationId` was moved to `r9`;
* `stationId` moved to `r9` because `currentStation` was spilled to free a
  register for the scratch;
* `currentStation` was spilled because the scratch took `r8`.

Greedy allocation breaks the tie the other way from the original, and every
source spelling that removes one edge re-rolls the other. This is the same
coupled fixed point as waves 122/127/128/129/132, now with the exact
register-to-variable mapping for both binaries.

## 4. Variant matrix (all single edits on the pristine baseline)

### 4.1 `stations` allocation / local caching

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **75.354839** |
| `camp_reload` | campaign `isTarget = stations->data()[i] == getTargetStation()` | 9900 | 73.235800 |
| `sid_reload_local` | `int sid = stations->data()[i]; isTarget = sid == ...` | 9900 | 73.235800 |
| `sameStation` | `bool sameStation = stationId == currentStation;` after the reload | 9900 | 73.235800 |
| `stationData_local` | `int stationData = stations->data()[i];` reused for the campaign check | 9900 | 73.235800 |
| `reload_nested` | `if (stationId != currentStation) { isTarget = stations->data()[i] == ...; }` | 9904 | 73.512118 |
| `reload_combined` | `isTarget = stationId != currentStation && stations->data()[i] == ...` | 9904 | 73.512118 |
| `reload_combined2` | same, operands swapped | 9900 | 73.235800 |
| `isTarget_first_cmp` | `if (isTarget && stationId != currentStation)` | 9900 | 73.321859 |
| `cr_exp_if0` | `camp_reload` + `expect(stationId != currentStation && isTarget, 0)` | 9908 | 73.842626 |
| `cr_exp_if1` | same, `expect(...,1)` | 9928 | 73.524355 |
| `cr_exp_istarget` | `expect(stations->data()[i] == ..., 0)` | 9900 | 73.235800 |
| `stations_before_if` | declare `stations` before the planet `if` | 9892 | 70.403910 |
| `stations_top_null` | `stations = nullptr;` at function scope, assign inside | 9908 | 75.354839 (tie) |
| `stations_top_uninit` | `stations;` at function scope, assign inside | 9908 | 75.354839 (tie) |
| `stations_constptr` | `Array<int> *const stations` | 9908 | 75.354839 (tie) |
| `stations_cast_style` | local `SolarSystem *solarSystem` for `getStations` | 9908 | 75.354839 (tie) |
| `swap_sid_cs` | swap `stationId`/`currentStation` declaration order | 9908 | 75.326165 |
| `sid_uint` / `cs_uint` / `sid_const` | type spelling | 9908 | 75.354839 (tie) |
| `sid_vol` | `volatile int stationId` | 9920 | 73.537844 |
| `warpgate_after_textoffset` / `textoffset_first` | declaration order | 9908 | 75.354839 (tie) |
| `marked_before_mission` | move `int marked` before the mission fetches | 9908 | 75.354839 (tie) |
| `freelance_before_campaign` | swap the two mission fetches | 9852 | 71.976995 |
| `norecomp` | delete the invented post-loop `onPlanetCourse` recompute | 9856 | 70.762590 |
| `norecomp+camp_reload` | both | 9864 | 72.991650 |

### 4.2 `turretMode` / `planetHit` slot swap

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **75.354839** |
| `planethit_before_stations` | `bool planetHit` before the `stations` fetch | 9908 | 75.354839 (tie) |
| `ph_top` | `bool planetHit = false;` at function scope | 9908 | 75.354839 (tie) |
| `ph_before_if` | `bool planetHit = false;` just before the planet `if` | 9908 | 75.354839 (tie) |
| `planetHit_int` | `int planetHit` | 9892 | 72.750753 |
| `nv_swap` | swap `notOtherLock`/`notTurretMode` declarations | 9908 | 75.354839 (tie) |
| `ntm_early` | compute `notTurretMode` right after `turretMode` | 9908 | 75.354839 (tie) |
| `onplanet_int` / `onplanet_uint` / `onplanet_init` | `onPlanetCourse` type/init | 9908 | 75.354839 (tie) |
| `missionblocks_int` | `int missionBlocksPlanets` | 9900 | 72.440493 |
| `alwaysenemy_bool` | `bool alwaysEnemySeen` | 9904 | 72.555205 |

The slot assignment is a `StackColoring` output, not a declaration-order
function: moving `planetHit` to function scope produces byte-identical code.
The original's `[sp,#148]=turretMode`, `[sp,#152]=planetHit`; ours is the
reverse. The same slot `[sp,#152]` is reused for `landmarkHit` in the
landmark region, so the swap is entangled with the `otherLock`/`status`
register choice below.

### 4.3 First `otherLock` guard (`cbnz` chain vs `cbz`+`itt`)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **75.354839** |
| `init1_if` | `int otherLock = 1; if (expect(cond,0)) otherLock = field_0x14 != 0;` | 9908 | 75.354839 (tie) |
| `ifelse_expect_cond` | positive `a!=0 || b!=0 || c!=0` with `expect(...,1)` | 9908 | 75.354839 (tie) |
| `no_expect` | drop all `__builtin_expect` | 9908 | 74.179211 |
| `outer_only` | drop the inner `expect(lockedGasCloud==nullptr,1)` | 9908 | 75.326165 |
| `last_expect0` | move the inner expect to `field_0x1c==nullptr` | 9908 | 75.326165 |
| `nested_guard` | three nested `if`s | 9908 | 74.179211 |
| `nested_guard_exp` | nested `if`s with `expect(...,1)` each | 9872 | 73.440644 |
| `exp_each` | `expect` each operand, no outer expect | 9872 | 73.440644 |
| `exp_each_outer0` | `expect` each + outer `expect(...,0)` | 9912 | 73.376344 |
| `exp_a0_b1` | `expect(a==null,0)` + `expect(b==null,1)` | 9912 | 73.308486 |
| `exp_a1_b1_outer1` | `expect` each + outer `expect(...,1)` | 9872 | 73.440644 |
| `initfield_if` | `int otherLock = field_0x14 != 0; if (!cond) otherLock = 1;` | 9868 | 72.570443 |

The original's `cbnz` chain is:

```
0x2b4 cbnz r0, 0x2c2      ; lockedAsteroid != 0 -> else
0x2b6 ldr r0,[r4,#56]     ; lockedGasCloud
0x2b8 cbnz r0, 0x2c2
0x2ba ldr r0,[r4,#28]     ; field_0x1c
0x2bc cmp r0,#0
0x2be beq.w 0xb72         ; all null -> cold then
0x2c2 movs r5,#1          ; single else block
```

Ours duplicates the else block (`mov r9,#1; ldr r5,[sp,#156]; b join` at
`0x2b6`, and `mov r9,#1` at `0x2e4`) and uses `itt eq` for the
`b==null && c==null` pair. The duplication exists because the else block
must reload `Globals::status` into `r5`; the original's else block is just
`movs r5,#1` because the original's landmark guard reloads status into `r6`
(`ldr r6,[sp,#156]` at `0x2ca`). So the guard shape is downstream of the
`otherLock`-in-`r5` vs `otherLock`-in-`r9`/`status`-in-`r5` choice, not an
independent source lever.

## 5. `notOtherLock` materialization is also downstream

The original materializes `notOtherLock`:

```
0x772 eor r0,r0,#1 ; str r0,[sp,#108]   ; notTurretMode
0x778 eor r0,r5,#1 ; str r0,[sp,#116]   ; notOtherLock  (r5 = otherLock)
...
0x80c ldr r6,[sp,#116] ; ands r3,r6     ; use notOtherLock
```

Ours folds `notOtherLock` into `!otherLock` (`cmp.w r9,#0; bne`), because
`otherLock` lives in `r9` and is not clobbered. In the original `r5` is
reused for `Globals::status` at `0x77c`, so `notOtherLock` must be spilled.
Reordering the `&&` operands to `... && notOtherLock && notTurretMode`
(`swap_ntm_nol`) or `notOtherLock` first (`nol_first`) does not recover it
(9872 / 71.92, 9908 / 74.49).

## 6. Root cause (unchanged, with the exact register mapping)

The residual is the coupled register-allocation / stack-coloring fixed point:

1. `stations` in `sl`+spill `[sp,#116]` (original) vs `r6`+`sl` backup
   (baseline) — the scratch-register choice is circular with
   `stationId`/`currentStation` (§3);
2. `otherLock` in `r5` (original) vs `r9` (baseline), with `status` in
   `r6` (original) vs `r5` (baseline) — this drives both the first
   `otherLock` guard's `cbnz` chain and `notOtherLock` materialization;
3. `turretMode`/`planetHit` stack slots `[sp,#148]`/`[sp,#152]` swapped;
4. the invented post-loop `onPlanetCourse` recompute (72 B) that
   compensates the original's +28 B loop body and +44 B tail, and keeps
   `alienOrbit` live across the planet loop (removing it re-rolls the
   allocation: `norecomp` 9856);
5. the enemy-tail block split and the ship `inSight` `s24`/`s26` live range
   (waves 132/133).

Every source lever tried this wave either ties at 75.354839 or lands below
it. The baseline is a strong local optimum; the remaining delta is
`MachineBlockPlacement`/`StackColoring` inside the
`0x12e2fc..0x12e376` guard region and the planet block.

## 7. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,143** differing bytes (size/base
exact), `.ARM.extab` **1,416** (size exact 156,312), `.note.gnu.build-id`
**20**. `.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`, `.rel.plt`,
`.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
