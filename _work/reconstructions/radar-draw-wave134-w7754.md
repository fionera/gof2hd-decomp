# Radar::draw wave 134 (w7754) — enemy-tail block-placement levers / literal-pool island audit (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w7754`, based on `master`
(`ec0acea1e`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official display 75.4). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave133-w7732.md`, `radar-draw-wave132-w7731.md`,
`radar-draw-wave131-w7712.md`, `radar-draw-body-wave122-w7210.md`.

This wave attacked the assignment's hypothesis directly: **the planet/ship/
asteroid draw-block order and the literal-pool layout**, and specifically the
open question left by wave 133 — the **enemy-tail block split**
(`0x12f3a6..0x12f420` contiguous in the original vs `0x12f3cc..0x12f3e0` +
`0x12f888..0x12f8a2` in ours). It adds four pieces of evidence the earlier
notes did not have:

1. a **fresh Ghidra 12.1.2 decompile** confirming the high-level block order is
   already identical (planet -> cold recompute -> planet-lock tail -> landmark
   -> enemies -> gas -> asteroids -> music);
2. a **literal-pool island proof**: the first pool position is a *consequence*
   of the first basic-block island's length, not an independent lever — the
   original's island ends at `0x12e33c` (`b.n 0x12e376`), ours at `0x12e30c`
   (`b.n 0x12e338`), which is exactly the 0x30-byte pool displacement;
3. a **block-placement lever sweep** for the enemy tail: `goto`, `continue`,
   empty-then/else, forward-goto, and `!(A||B||C||D)` / `!A&&!B&&!C&&!D` all
   canonicalise to the same IR and produce **byte-identical** output;
4. a **positive-OR + `__builtin_expect` matrix** for the first `otherLock`
   guard: none of the 7 spellings beats the baseline, and the `cbnz` chain only
   appears when the outer `expect(...,0)` is removed, which re-rolls the tail.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w7754
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.9 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra decompile (provider reachable with
`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`):

```sh
rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure 0x13e050 --format md > ghidra_draw.md
```

Scratch in `/dev/shm/w7754` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol difflib harness reproducing 75.354839
exactly), `drv.py` (variant driver), `runs.py`/`runs.txt` (normalised
instruction aligner with original addresses), `orig.dis`/`ours.dis` (full
Thumb disassembly), `ghidra_draw.md` (2019-line decompile).

## 2. Ghidra block order (re-confirmed, unchanged)

The decompiler statement order is exactly the baseline source's block order:

```
prologue (enabled/mission/turretMode/alienOrbit/campaign/arrays)
transform = CameraGetLocal(CameraGetCurrent()); cameraX/Y/Z; MatrixGetInverse
onPlanetCourse (prologue cold tail)
field_0x1a8 = onPlanetCourse
4x DrawImage2D(field_0x1c4, ...)          <- radar background quad
route block
otherLock guard #1
landmark guard (station 109/110/111)
landmark loop
planet-lock tail (inside landmark `if`)
otherLock recompute #1
planet block (campaign>=2 && planetTargets && !alienOrbit && !isDockingToPlanet)
planet loop
planet-lock tail
enemies block (lockedEnemy / scannerAvailable / enemy loop / enemy tail)
otherLock recompute #2
gas-cloud block
asteroids block
music tail
```

The planet/ship/asteroid block order is therefore **already exact**; the
residual is codegen, not source order.

## 3. Literal-pool island proof (new)

The first pool is at `0x12e340` (original) and `0x12e310` (ours), 5 words in
both, identical words in identical order (wave 129 result, re-confirmed):
`__stack_chk_guard`, `Globals::Canvas`, `Globals::status`,
`PaintCanvas::DrawImage2D`, `Globals::font`.

The pool is emitted at the end of the **first constant island**. The first
`ldr [pc,#N]` is at `0x12e064` in both. The island ends at the first
unconditional branch:

```
original: 0x12e2fc otherLock guard ... 0x12e33c b.n 0x12e376   -> pool 0x12e340
ours:     0x12e2fc otherLock guard ... 0x12e30c b.n 0x12e338   -> pool 0x12e310
```

So the 0x30-byte pool displacement is exactly the 0x30-byte difference in the
first island: the original's `cbnz` chain keeps the 109/110 station checks
inside the island (they fall through into `0x12e33c`), while ours' `cbz`+`itt`
form branches out at `0x12e30c` and places the station checks *after* the pool.
This is why every `ldr [pc,#N]` differs textually: the immediate is a function
of the island length, not a separate source choice. It also confirms wave 133's
"region-neutral" result: the landmark guard region re-aligns at `0x12e376`.

## 4. Enemy-tail block split (the one remaining large block-order delta)

Original (contiguous, body fall-through):

```
12f3a6 ldr r0,[r4,#0] ; blx Level::getPlayer
12f3ac blx PlayerEgo::isDockingToAsteroid
12f3b0 and r1,r6,#1 ; str [sp,#92]
12f3b6 ldr r1,[sp,#80] ; and ; str [sp,#104]
12f3c0 ldr r1,[sp,#88] ; and ; str [sp,#132]
12f3c8 bne.n 12f40e                  ; A -> otherLock recompute #2
12f3ca blx Level::getPlayer
12f3d0 blx PlayerEgo::isDockingToDockingPoint
12f3d4 orrs r0,[sp,#148] ; orrs r0,[sp,#144]
12f3dc bne.n 12f40e                  ; (B|turret|other) -> recompute
12f3de ldr r0,[r4,#8]                ; navWreck body ...
12f3f0 blx KIPlayer::isDead
12f3fe b.w 12fe74
12f40e ...                           ; otherLock recompute #2
```

Ours (body outlined after the gas-loop body):

```
12f3cc ldr r0,[r4,#0] ; blx Level::getPlayer
12f3d2 ldr r1,[sp,#80] ; and ; str [sp,#92]
12f3da blx PlayerEgo::isDockingToAsteroid
12f3e0 beq.w 12f888                  ; !A -> outlined body
12f3e4 ...                           ; skip path
12f888 ldr r2,[sp,#144] ; cmp ; it ne ; movne
12f890 ldr r0,[r4,#0] ; ldr r1,[sp,#152] ; orr.w r5,r1,r2
12f898 blx Level::getPlayer
12f89c blx PlayerEgo::isDockingToDockingPoint
12f8a0 cmp r5,#0 ; bne.w 12f3e4
12f8a6 ldr r5,[sp,#88] ; cmp r0,#0 ; bne.w 12f3e6
12f8ae ldr r0,[r4,#8]                ; navWreck body ...
```

The difference is **which side of the `if` is fall-through**. The original
keeps the condition continuation + navWreck inline and branches to the loop
exit; ours keeps the loop-exit/skip path inline and outlines the body.

**New result:** every source spelling of the condition produces the *same*
output (all 75.354839 / 9908 B). The measured spellings:

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | `!A && !(B \|\| turret \|\| other)` | 9908 | **75.354839** |
| `andchain` | `!A && !B && !turret && !other` | 9908 | 75.354839 (tie) |
| `orchain` | `!(A \|\| B \|\| turret \|\| other)` | 9908 | 75.354839 (tie) |
| `empty_then` | `if (A\|\|B\|\|turret\|\|other) { } else { body }` | 9908 | 75.354839 (tie) |
| `goto_else` | `if (cond) { goto tail_done; } else { body }` | 9908 | 75.354839 (tie) |
| `goto_block` | `if (cond) goto tail_done; { body }` | 9908 | 75.354839 (tie) |
| `hoistbool` | `bool canLock = ...; if (canLock) { body }` | 9900 | 67.288379 |
| `order_turret_first` | `!(turret \|\| other \|\| B)` | 9880 | 69.596438 |
| `order_other_first` | `!(B \|\| other \|\| turret)` | 9876 | 69.912394 |
| `use_notvars` | `!A && !B && notTurretMode && notOtherLock` | 9828 | 62.231821 |
| `use_notvars_or` | `!(B \|\| !notTurretMode \|\| !notOtherLock)` | 9828 | 62.231821 |
| `use_notvars_and` | `notTurretMode != 0 && notOtherLock != 0` | 9828 | 62.231821 |

The `hoistbool`/`order_*`/`use_notvars` forms are **shorter** and re-roll the
rest of the function; the tie forms are byte-identical. So the block placement
is a pure `MachineBlockPlacement` decision, not a source-order one.

## 5. First `otherLock` guard: positive-OR + expect matrix (new)

The original's guard is a `cbnz` chain (`lockedAsteroid != null`,
`lockedGasCloud != null`, `field_0x1c != null`), i.e. the positive-OR form.
The baseline's `__builtin_expect(..., 0)` makes clang invert it to
`cbz`+`itt eq`. Removing the expect restores the `cbnz` chain but re-rolls the
tail. Measured:

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | outer `expect(all-null,0)` + inner `expect(gas==null,1)` | 9908 | **75.354839** |
| `outer0_noinner` | outer `expect(all-null,0)`, no inner expect | 9908 | 75.326165 |
| `pos_outer1` | positive OR + `expect(any,1)` | 9908 | 75.326165 |
| `pos_ifelse` | positive OR, no expect | 9908 | 74.179211 |
| `pos_ternary` | ternary `(any) ? 1 : (field_0x14 != 0)` | 9908 | 74.179211 |
| `pos_init` | `int otherLock = 1; if (all null) ...` | 9908 | 74.179211 |
| `pos_outer0` | positive OR + `expect(any,0)` | 9872 | 73.440644 |
| `outer1` | outer `expect(all-null,1)` | 9872 | 73.440644 |
| `noouter_inner1` | no outer expect, inner `expect(gas==null,1)` | 9892 | 72.891566 |

The `cbnz` chain only appears without the outer expect, and that costs 1.2
points because the tail re-rolls. The baseline's expect is therefore doing real
work: it is what keeps the rest of the function in its current (locally optimal)
placement.

## 6. Other new sweeps (all ties or worse)

### 6.1 Comparison canonicalisation

All of the following are semantic-equivalent rewrites that clang canonicalises
to the same compare/branch, so they are exact ties (9908 B / 75.354839):
`campaign >= 2` -> `> 1`; `getType() != 172` -> `!= 0xac`; `slotCount <= 3` ->
`< 4`; `planetLockTimer >= 1` -> `> 0`; `stationLockTimer > 0` -> `>= 1`;
`gasLockTimer > 500` -> `>= 501`; `asteroidLockTimer > 500` -> `>= 501`;
`field_0x1bc >= 1` -> `> 0`; `field_0x1bc < 3` -> `<= 2`; `field_0x1bc < 5` ->
`<= 4`; `(current-136) > 15` -> `>= 16`; `i <= 1` -> `i < 2`.
`distance < bestDistance` -> `<=` is the only one that changes output
(75.268817).

### 6.2 `bool`/`int` type sweep

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| `tractor_bool_int` (`dead` -> `int`) | 9908 | 75.354839 (tie) |
| `navwreck_bool_int` | 9908 | 75.354839 (tie) |
| `hasdocknav_int` | 9908 | 75.354839 (tie) |
| `gashit_bool_int` | 9908 | 75.326165 |
| `enemyhit_bool_int` | 9924 | 71.422432 |
| `boss_bool_int` | 9904 | 73.493976 |
| `always_bool_int` | 9900 | 72.557739 |
| `asthit_bool_int` | 9908 | 70.326835 |
| `planethit_bool_int` | 9892 | 72.750753 |
| `marked_bool_int` | 9916 | 72.732491 |
| `notother_bool` | 9920 | 71.963287 |
| `notturret_bool` | 9844 | 73.435927 |
| `landmarkhit_int` | 9884 | 70.034443 |
| `wreck_bool_int` | 9912 | 72.925326 |

### 6.3 Loop-bound / size caching (iterator reuse)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| `planet_size_cache` | 9908 | 74.194011 |
| `enemy_size_cache` | 9864 | 63.982746 |
| `ast_size_cache` | 9908 | 74.233305 |
| `gas_size_cache` | 9912 | 70.296604 |
| `landmark_size_cache` | 9900 | 70.556193 |

### 6.4 Declaration hoisting / ordering

Moving `int otherLock;` or `bool landmarkHit;` to the top of the function is a
tie (9908 B / 75.354839). Moving `notOtherLock`/`notTurretMode` inside the
landmark `if` is 9900 B / 73.551348. A float `crossX`/`crossY` with the
bounds-first `inSight` condition is a tie (clang folds the floats back to int).

## 7. Root cause (unchanged, with the island proof)

The residual is the coupled register-allocation / block-placement fixed point:

1. the first `otherLock` guard's `cbz`+`itt` shape (ours) vs the original's
   `cbnz` chain, which is *also* the reason the first literal pool sits at
   `0x12e310` instead of `0x12e340` (island length, §3);
2. the invented post-loop recompute at `0x12e72c` (original has only the
   prologue cold tail at `0x12e6ee`), whose 72 B exactly compensate the
   original's +28 B planet-loop body and +44 B tail (wave 132);
3. the enemy-tail block split (§4), a pure `MachineBlockPlacement` decision;
4. `stations` in `r6` instead of `sl`+spill, and the `turretMode` slot swap;
5. the bottom-placed landmark loop header (`0x12eb48` vs `0x12e7d4`);
6. the ship `inSight` `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

The new evidence rules out the assignment's two named levers: the draw-block
order is already exact, and the literal-pool layout is a consequence of the
first-island length, not an independent source choice. The remaining delta is
entirely `MachineBlockPlacement` / register allocation inside
`0x12e2fc..0x12e376` and the enemy tail.

## 8. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 85,143 differing bytes (size/base
exact), `.ARM.extab` 1,416 (size exact 156,312), `.note.gnu.build-id` 20.
`.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`,
`.dynsym`, `.dynstr`, `.hash` are byte-identical.
