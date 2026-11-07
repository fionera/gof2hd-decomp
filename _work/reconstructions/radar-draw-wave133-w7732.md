# Radar::draw wave 133 (w7732) — draw-block order / literal-pool layout audit (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w7732`, based on `master`
(`144fc16b0`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline official score **75.4 %** (unrounded `difflib` ratio 75.400000).
Whole-`.text` differing bytes **85,244** (`.text` size/base exact).
Gate at start and end: `RATCHET PASS`, byte **4506**, linked **4506**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave131-w7712.md`, `radar-draw-wave129-w7673.md`,
`radar-draw-body-wave122-w7210.md`.

This wave executed the assignment's NEW hypothesis directly: **the
planet/ship/asteroid draw-block order and the literal-pool layout**, using a
fresh Ghidra 12.1.2 decompile of `FUN_0013e050` plus the original Thumb
disassembly. It produced three new results the earlier notes did not have:

1. the exact high-level draw-block order is **already identical** to the
   baseline source (Ghidra statement order confirms it);
2. the first literal pool is **address-shifted but region-neutral** — the
   landmark-guard region re-aligns at `0x12e376`, so pool placement is a dead
   end, not the residual;
3. the real remaining block-order delta is the **enemy-tail block split**
   (`0x12f3a6..0x12f420` contiguous in the original vs
   `0x12f3cc..0x12f3e0` + `0x12f888..0x12f8a2` in ours), which is a
   `MachineBlockPlacement` decision, not a source-order one.

Every source variant below was built and scored; all land at or below the
baseline. The baseline is a strong local optimum.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w7732
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra decompile (provider reachable with `GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`):

```sh
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure 0x13e050 --format md
```

Scratch in `/dev/shm/w7732` (not committed): `Radar.base.cpp` (pristine
source), `score1.py` (single-symbol scorer importing `tools/verify/verify.py`,
reproduces 75.400000 exactly), `runs.py` (normalised instruction aligner with
original addresses), `e_*.py` / `batch1.py`..`batch9.py` (variant drivers),
`w7732_orig.dis` / `w7732_ours.dis` (full function objdump), `w7732_ghidra.md`
(2019-line decompile).

## 2. Exact draw-block order from Ghidra (new)

Ghidra's statement order for the whole function is:

```
prologue (enabled / mission / turretMode / alienOrbit / campaign / arrays)
transform = CameraGetLocal(CameraGetCurrent()); cameraX/Y/Z; MatrixGetInverse
onPlanetCourse (prologue cold tail; `beq.w 0x12e6ee` from 0x12e1b6)
field_0x1a8 = onPlanetCourse
4x DrawImage2D(field_0x1c4, ...)          <- radar background quad
route block (getWaypoint / update / calcDistance / DrawImage2D / DrawString)
otherLock guard #1                        <- `if (all null) otherLock = field_0x14 != 0; else 1`
landmark guard (station 109/110/111)
landmark loop                             <- outlined after the planet section
planet-lock tail (inside the landmark `if`)
otherLock recompute #1 (lockedAsteroid||lockedGasCloud||field_0x1c||lockedStation)
planet block (campaign>=2 && planetTargets && !alienOrbit && !isDockingToPlanet)
planet loop
planet-lock tail
enemies block (lockedEnemy / scannerAvailable / enemy loop / enemy lock tail)
otherLock recompute #2 (dockNavPtr&&!lockedEnemy || field_0x1c || lockedStation || field_0x14)
gas-cloud block (sparks + cloud loop + gas lock tail)
asteroids block (slot reset / otherLock recompute / asteroid loop / slot distance / lock tail)
music tail (field_0x1bc, switch on currentMusicEvent)
```

This is **exactly** the baseline source's block order. The Ghidra
decompile also confirms the source shape of the first `otherLock` guard is the
baseline's, not a positive-OR form:

```c
pRVar27 = this + 0xc;                       /* &lockedAsteroid */
if ((*(int *)pRVar27 == 0) && (*(int *)(this + 0x38) == 0) && (*(int *)(this + 0x1c) == 0)) {
    bVar42 = *(int *)(this + 0x14) != 0;    /* field_0x14 */
} else {
    bVar42 = true;
}
```

and the second recompute uses `this + 0x24` (lockedStation), i.e. the source
`lockedAsteroid || lockedGasCloud || field_0x1c || lockedStation`. So the
draw-block *order* and the high-level predicate shapes are pinned; the
residual is codegen.

## 3. Literal-pool layout is region-neutral (new)

The first pool is at `0x12e340` (original) and `0x12e310` (ours), 5 words in
both, **identical words in identical order** (wave 129 result, re-confirmed):
`__stack_chk_guard`, `Globals::Canvas`, `Globals::status`,
`PaintCanvas::DrawImage2D`, `Globals::font`.

New this wave: the pool sits *inside* the landmark-guard region, and that
region has the **same total size and the same end address in both binaries**:

```
0x12e2fc  otherLock guard start          (both)
0x12e310  ours pool   (5 words -> 0x12e324)
0x12e340  orig pool   (5 words -> 0x12e354)
0x12e376  landmark-guard region end / planet block start   (both)
0x12e3dc  planet-loop exit branch        (both)
```

So the 0x30-byte pool displacement is exactly compensated by the internal
split of the landmark guard (original: 109/110 checks before the pool, 111
check after; ours: all three checks after the pool). Everything from
`0x12e376` onward is **address-aligned**, which is why the planet loop, the
cold tail and the planet-lock tail all sit within a few bytes of the original.
The literal-pool position is therefore not a lever for the rest of the
function; only the ~0x7a-byte landmark-guard region itself differs.
(Measured: the baseline's first pool is 0x30 earlier, yet the planet loop exit
is at the same `0x12e3dc` in both.)

## 4. The real block-order delta: enemy-tail block split (new)

The original's enemy-tail condition/navWreck/otherLock block is **contiguous**
before the gas-cloud loop:

```
12f3a6  ldr r0,[r4,#0]
12f3a8  blx Level::getPlayer
12f3ac  blx PlayerEgo::isDockingToAsteroid
12f3b0  and.w r1,r6,#1 ; str [sp,#92]
12f3b6  ldr r1,[sp,#80] ; and.w ; str [sp,#104]
12f3c0  ldr r1,[sp,#88] ; and.w ; str [sp,#132]
12f3c8  bne.n 12f40e                  ; isDockingToAsteroid -> otherLock recompute
12f3ca  blx Level::getPlayer
12f3d0  blx PlayerEgo::isDockingToDockingPoint
12f3d4  orrs r0,[sp,#148] ; orrs r0,[sp,#144]
12f3dc  bne.n 12f40e                  ; turretMode||otherLock -> otherLock recompute
12f3de  ldr r0,[r4,#8]                ; navWreck: dockNavPtr ...
12f3f0  blx KIPlayer::isDead
12f3fe  b.w 12fe74
12f40e  ldr r0,[r4,#8]                ; otherLock recompute (dockNavPtr...)
```

Ours splits the same code into two blocks, the second outlined **after** the
gas-cloud loop:

```
12f3cc  blx Level::getPlayer
12f3d2  ldr r1,[sp,#80] ; and.w ; str [sp,#92]
12f3da  blx PlayerEgo::isDockingToAsteroid
12f3e0  beq.w 12f888                  ; !isDockingToAsteroid -> outlined block
12f3e4  ...                           ; docking path
12f888  ldr r2,[sp,#144] ; ... ; blx Level::getPlayer
12f89c  blx PlayerEgo::isDockingToDockingPoint
12f8a0  cmp r5,#0 ; bne.w 12f3e4
12f8ae  ldr r0,[r4,#8]                ; navWreck
12f8bc  blx KIPlayer::isDead
12f8ca  b.n 12fe74
```

This is the one remaining *large* block-order difference (25 normalised
instructions in the aligner's run table). It is a `MachineBlockPlacement`
decision: the callee is the same, the predicates are the same, and every
source spelling of the condition (`&&`, `!(A||B||C||D)`, `!A&&!B&&!C&&!D`,
nested `if`, hoisted `bool` locals) produces byte-identical output (all
75.400000 / 9908 B). It cannot be moved from source without re-rolling the
whole function.

## 5. Planet-lock tail store order is already correct (new)

Ghidra and the disassembly both store **`planetLockTimer` (0x1a0) before
`field_0x18` (0x18)** in the `field_0x14 == 0` arm:

```
orig 12ec1c  movs r0,#0 ; str.w r0,[r4,#416] ; str r0,[r4,#24]
ours 12ec20  movs r0,#0 ; str.w r0,[r4,#416] ; str r0,[r4,#24]
```

The baseline source writes `field_0x18` first; clang's scheduler emits
`planetLockTimer` first anyway, matching the original byte-for-byte. This
closes wave 122 §5.5: the proposed source swap is a no-op that only re-rolls
nearby blocks (measured 65.7 %).

## 6. Variant matrix (all single edits on the pristine baseline)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **75.400000** |
| `e_norecompute` | delete the invented post-loop `onPlanetCourse` recompute (Ghidra has only the prologue cold tail) | 9856 | 70.800000 |
| `recompute_before_loop` | move that recompute before the planet loop | 9996 | 70.700000 |
| `e_insight` | faithful distance-first enemy `inSight` (float `crossX/crossY`, `VectorLength` first) | 9896 | 68.800000 |
| `tail_rev` | swap the two enemy-tail `&&` operands | 9880 | 69.500000 |
| `tail_exp1` | `__builtin_expect(!isDockingToAsteroid(), 1)` | 9872 | 62.800000 |
| `tail_exp0` | `__builtin_expect(!isDockingToAsteroid(), 0)` | 9944 | 59.400000 |
| `tail_whole_exp1` | expect the whole tail condition likely | 9892 | 68.400000 |
| `tail_hoist_dp` | hoist `isDockingToDockingPoint()` into a `bool` | 9884 | 68.500000 |
| `tail_hoist_da` | hoist `isDockingToAsteroid()` into a `bool` | 9908 | 75.400000 (tie) |
| `tail_hoist_both` | hoist both | 9884 | 68.500000 |
| `tail_orchain` | `!(A || B || turretMode || otherLock)` | 9908 | 75.400000 (tie) |
| `tail_andchain` | `!A && !B && !turretMode && !otherLock` | 9908 | 75.400000 (tie) |
| `tail_nested` | nested `if (!A) { if (!(...)) { body } }` | 9908 | 75.400000 (tie) |
| `otherlock_pos` | `int otherLock = 1; if (all null) ...` | 9908 | 74.200000 |
| `otherlock_pos_exp` | same + outer `expect(...,0)` | 9908 | 75.300000 |
| `ol_ifelse` | `if (any) otherLock = 1; else otherLock = field_0x14 != 0;` | 9908 | 74.200000 |
| `ol_ifelse_exp` | same + `expect(any,1)` | 9908 | 75.300000 |
| `ol_expr` | single `int otherLock = a||b||c||d` | 9852 | 73.000000 |
| `ol_expr_bool` | same, `bool` | 9812 | 69.500000 |
| `lm_pos` | landmark guard as positive `!=109 && !=110 && !(...)` | 9908 | 75.400000 (tie) |
| `lm_hoist` | hoist `getStation()->getIndex()` once | 9984 | 39.800000 |
| `lm_locklabel` | landmark `i==0` arm assigns `this->lockLabel` (Ghidra) | 9904 | 71.900000 |
| `ast_cargo_first` | asteroid predicate `!hasCargo && (isDead||isDying)` (Ghidra) | 9912 | 73.500000 |
| `pl_swap` | swap `field_0x18`/`planetLockTimer` source order | 9904 | 65.700000 |
| `planet_hoist` | hoist `planetTargets->data()[i]` | 9908 | 75.400000 (tie) |
| `landmark_hoist` | hoist `landmarks->data()[i]` | 9808 | 68.300000 |
| `planet_for_int` / `_ne` | planet loop spelling | 9908 | 75.400000 (tie) |
| `enemy_for_int` | enemy loop spelling | 9908 | 75.400000 (tie) |
| `ast_for_uint` | asteroid loop spelling | 9908 | 75.400000 (tie) |
| `astslot_i` | asteroid slot loop `k` -> `i` | 9908 | 75.400000 (tie) |
| `gas_for_decl` | gas loop `for (uint32_t i...)` | 9908 | 75.300000 |
| `planetHit_before` | move `bool planetHit` before `stations` | 9908 | 75.400000 (tie) |
| `slotCount_uint` / `otherLock_uint` / `hasDockNav_int` / `candidateWreck_int` / `navWreck_int` / `onPlanetCourse_int` | type spelling | 9908 | 75.400000 (tie) |
| `landmark_before` / `notvars_before` / `ol_before_landmarkhit_only` | local declaration order | 9908 | 75.400000 (tie) |
| `marked_bool` | `int marked` -> `bool` | 9916 | 72.700000 |
| `alwaysEnemy_int` / `bossSeen_int` / `enemyHit_int` / `gasHit_int` / `asteroidHit_int` | `bool` -> `int` | 9900–9924 | 70.3–75.3 |
| `inCenter_bool` | `int inCenter` -> `bool` | 9912 | 73.000000 |
| `notOtherLock_bool` / `notTurretMode_bool` | `int` -> `bool` | 9920 / 9844 | 72.0 / 73.4 |
| `landmarkHit_int` / `missionBlocks_int` / `turretMode_int` / `alienOrbit_int` | `bool` -> `int` | 9884–9900 | 67.5–75.3 |

The faithful Ghidra shapes (`e_norecompute`, `e_insight`, `lm_locklabel`,
`ast_cargo_first`) are all **shorter and score lower**: they re-roll every
block after the edit. The baseline's extra post-loop recompute is the only
thing keeping the planet-lock tail in its original slot.

## 7. Root cause (unchanged, with the new evidence)

The residual is the coupled register-allocation / block-placement fixed point:

1. the first `otherLock` guard's `cbz`+`itt` shape (ours) vs the original's
   `cbnz` chain, with the cold `field_0x14 != 0` block at `0x1305fa` (ours)
   vs `0x12ebc2` (original);
2. the invented post-loop recompute at `0x12e72c` (original has only the
   prologue cold tail at `0x12e6ee`) — but deleting it moves the landmark
   loop body to `0x12f490` and costs 4.6 points;
3. the enemy-tail block split (§4);
4. `stations` in `r6` instead of `sl`+spill, and the `turretMode` slot swap;
5. the bottom-placed landmark loop header (`0x12eb48` vs `0x12e7d4`);
6. the ship `inSight` `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

The new evidence rules out the two levers the assignment named: the
draw-block order is already exact, and the literal-pool layout is
region-neutral (the function re-aligns at `0x12e376`). The remaining delta is
entirely `MachineBlockPlacement` / register allocation inside the
`0x12e2fc..0x12e376` guard region and the enemy tail.

## 8. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 85,244 differing bytes (size/base
exact), `.ARM.extab` 1,416 differing bytes (size exact 156,312),
`.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`,
`.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
