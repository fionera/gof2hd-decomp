# Radar::draw pass 8 — the scoped 80:1 branch weight is a real 1.4-point win (w4241)

**Status:** landed change + analysis note, 2026-11-03. Worktree `wt/w4241`, branch
`wave92-w4241`, base `b379d00d`, HEAD before this pass `eb472137`.

**Change:** restore the per-file
`-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` override for
`src/game/weapons/Radar.cpp` in `CMakeLists.txt` and correct the stale comment that
claimed the override had no effect.

**Result:** `_ZN5Radar4drawEP6PlayerP3Hudi` official verifier ratio
**0.724825 (72.4825 %) -> 0.739255 (73.9255 %)**, `usize 9908` unchanged.
Gate unchanged: `byte 4498 / linked 4498 / size 4524 / data 843 / imports 207 /
lint CLEAN / drift 0 / RATCHET PASS`.

## 1. The regression being repaired

Commit `666d7c5e` ("Branch-weight overrides pass 1") removed the Radar, SpaceLounge and
MenuTouchWindow per-file branch-weight overrides and wrote:

> with the default 2000:1 `__builtin_expect` weight the linked .so is byte-identical
> (byte_exact/linked_exact unchanged and the official per-function percentages unchanged)

The first half is true only in the sense that the **gate counts** are unchanged. The
function bytes are *not* identical, and the official percentage is *not* unchanged.
Measured with the official verifier API (`autojunk=False`):

| Radar.cpp flags | usize | ratio | pct | normalized stream (orig/ours) | function SHA-1 |
|---|---:|---:|---:|---:|---|
| default (no override) | 9908 | 0.724825 | 72.4825 % | 3492 / 3489 | `b549e88cda1362a3d408b11729377096fae3c024` |
| **80:1 (restored)** | 9908 | **0.739255** | **73.9255 %** | 3492 / 3488 | `414321e9e05febb673ee0413ffb4a80392be6ae4` |

Both builds produce the same `usize`, the same gate counts, and the same `.text` size
(`0x1337c8`); only `Radar::draw`'s bytes and its verifier ratio differ.

`80:1` is a broad plateau, not a magic value. From the pass-7 note
(`radar-draw-missing-124-w4223.md` section 4), re-measured with the same source:

| L:U | usize | ratio |
|---|---:|---:|
| none | 9908 | 0.724825 |
| 60:1 / 64:1 / 70:1 | 9908 | 0.738109 |
| 72:1 / 74:1 | 9908 | 0.739255 |
| **80:1** | 9908 | **0.739255** |
| 82:1 .. 96:1 / 110:1 | 9908 | 0.739255 |
| 104:1 / 120:1 | 9908 | 0.725398 / 0.725111 |
| 150:1 / 200:1 / 500:1 / 1000:1 / 2000:1 | 9908 | 0.738968 / 0.738968 / 0.738968 / 0.738968 / 0.724825 |

So the override is a genuine matching knob for this TU, and the old comment's
"percentages unchanged" claim was wrong.

## 2. What the 80:1 knob changes

Diffing the no-override and 80:1 bodies instruction-by-instruction (addresses and
branch targets normalized) gives ratio 0.9457: the two builds are the same function
with a different **block placement**, not different statements. The largest visible
effects are the placement of the duplicate autopilot block and of the first
`otherLock` cold block:

| block | no override | 80:1 | original |
|---|---|---|---|
| duplicate autopilot block | `0x12ea70` | `0x12e6d2` | `0x12e6ee` (single block) |
| first `otherLock` cold block | `0x13066e` | `0x1304d0` | `0x12ebc2` |
| second `otherLock` cold block | `0x1304d0` | `0x12eb8a` | `0x12ebce` |

The 80:1 layout is closer to the original in the early/planet region, which is why the
official ratio rises even though the instruction counts and the gate are unchanged.

## 3. Statement completeness (call multiset)

Counting `bl`/`blx` with a resolved `<sym>` in the raw objdump of both 9908-byte
bodies (current 80:1 build vs original):

| | original | ours (80:1) |
|---|---:|---:|
| total `bl`+`blx` | 345 | 352 |
| `PaintCanvas::DrawString` | 9 | 8 |
| `SolarSystem::getStations` | 2 | 3 |
| `SolarSystem::getStationEnumIndex` | 1 | 2 |
| `Level::getPlayer` | 29 | 32 |
| `Status::getSystem` | 5 | 7 |
| `KIPlayer::isDead` | 17 | 16 |
| `KIPlayer::isDying` | 13 | 12 |
| `PlayerEgo::goingToPlanet` | 1 | 2 |
| `PlayerEgo::isDockingToPlanet` | 4 | 5 |
| `PlayerEgo::getAutoPilotTarget` | 1 | 2 |

The ten positive deltas (`getStations +1`, `getStationEnumIndex +1`, `getPlayer +3`,
`getSystem +2`, `goingToPlanet +1`, `isDockingToPlanet +1`, `getAutoPilotTarget +1`)
are exactly the call set of the **second, campaign-block autopilot block** in
`Radar.cpp` (lines 612-620):

```cpp
        if (!alienOrbit) {
            int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
            SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
            int stationIndex = system->getStationEnumIndex(
                    ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
                            ->data()[this->planetDockIndex]);
            this->field_0x1a8 = this->level->getPlayer()->goingToPlanet() &&
                                !this->level->getPlayer()->isDockingToPlanet() &&
                                autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
        }
```

The original has only **one** autopilot block, and it is the **top-level** one, not the
campaign one. Evidence (original disassembly):

* the top-level `else` arm at `0x12e1a8..0x12e1b6` tests
  `goingToPlanet() && !isDockingToPlanet()` and then `beq.w 0x12e6ee`;
* `0x12e6ee..0x12e746` is the autopilot body: `getAutoPilotTarget` (`0x12e6f4`),
  `getStarSystem`/`getPlanetTargets`, `getSystem` x2, `getStations` (`0x12e71c`),
  `getStationEnumIndex` (`0x12e72a`), then `subs/clz/lsrs` for
  `autoPilotTarget == targets->data()[stationIndex]`; it contains **no**
  `goingToPlanet`/`isDockingToPlanet` call;
* the campaign block still has its own `getStations` (`0x12e3c2`) for the planet loop,
  but no second autopilot body.

So the campaign-block `if (!alienOrbit)` is the semantically wrong duplicate. Deleting
it (`del_campaign_ap`) makes the call multiset match the original's autopilot calls, but
it compiles to **9740 B / 0.622903** — the baseline's duplicate block is a codegen
crutch that keeps the register allocation closer to the original. Deleting the
top-level block instead (the pass-7 "A" variant) also gives 9740 B / 0.622903.

## 4. Residual differences (all codegen, not statements)

### 4.1 Stack-slot swap

The stack frame size is identical (`sub sp,#296`, `str r0,[sp,#292]`), and the set of
`[sp,#N]` offsets used is identical. Only the assignment of three slots is permuted:

| variable | original | ours (80:1) |
|---|---:|---:|
| `mode` (3rd argument) | `[sp,#100]` | `[sp,#112]` |
| `turretMode` | `[sp,#148]` | `[sp,#152]` |
| `landmarkHit` / `autoPilotTarget` (slot-shared) | `[sp,#152]` | `[sp,#148]` |
| `notTurretMode` | `[sp,#108]` | `[sp,#108]` (match) |
| `notOtherLock` | `[sp,#116]` | `[sp,#116]` (match) |

The landmark-loop preamble is otherwise instruction-for-instruction identical; only
`#148`/`#152` are exchanged.

### 4.2 VFP save set

```
original  12e058  vpush {d8-d13}
ours      12e058  vpush {d8-d11}
```

The original keeps `PlayerEgo::crosshairPos.x/y` in callee-saved `s26`/`s24` across
the two AEMath calls in the enemy in-sight test (`0x12ef4c..0x12ef76`); ours converts
them from `s0`/`s2`, so only `d8-d11` are saved. The "distance-first" source shape
reproduces the original `vpush` but is 9924 B / 0.711894.

### 4.3 Constant pool

The first PIC constant pool is five words, byte-identical in both:

```
000e1c9a 000e2166 000e20bc 000e2224 000e1fda
```

Only its placement differs: original `0x12e340`, ours `0x12e310`. clang places the
island after the first unconditional branch that follows the first `ldr rX,[pc,#N]`;
the `otherLock` block shape moves that branch, so every `[pc,#N]` operand downstream
differs by a constant.

### 4.4 Tail merges

* **Landmark distance `DrawString`.** The original keeps the `else` branch's distance
  `DrawString` out of line (`0x12e9e0`) and the `i == 0` branch's second one separate
  (`0x12eb36`); ours tail-merges them into one shared `blx` (9 vs 8 `DrawString`
  calls). The original's `else` tail is `blx; ldr r5,[sp,#156]; mov r9,r6; b ...`
  while the `i == 0` tail is `blx; ldr r5,[sp,#156]; ldr fp,[sp,#112]; b ...`, so the
  suffixes differ and clang does not merge them.
* **Music `play`.** The original has two separate play tails (`0x1302d2` and
  `0x130698`); the A+B+C shape shares one. The current 9908-byte baseline already has
  the correct `play` call count (7 vs 7), so this merge is not active in the landed
  shape.

## 5. Source variants measured this pass (all with the 80:1 flag)

Every variant is the baseline `Radar.cpp` plus the listed edit, measured with the
official verifier (`autojunk=False`). Only variants that stay at `usize 9908` are
gate-landable.

| variant | usize | ratio | note |
|---|---:|---:|---|
| baseline | 9908 | **0.739255** | |
| delete campaign-block `if (!alienOrbit)` autopilot block | 9740 | 0.622903 | semantically correct, size shift |
| `else` branch of landmark `i <= 1` + `continue;` | 9908 | 0.739255 | neutral (still merged) |
| `i == 0` branch + `continue;` | 9908 | 0.739255 | neutral |
| both branches + `continue;` | 9908 | 0.739255 | neutral |
| `otherLock` if/else without `__builtin_expect` | 9908 | 0.739255 | neutral |
| `otherLock` `\|\|` form without expect | 9908 | 0.739255 | neutral |
| `otherLock` `\|\|` form with `expect(...,1)` | 9908 | 0.739255 | neutral |
| `int otherLock = 1; if (all-null) otherLock = f14 != 0;` | 9908 | 0.739255 | neutral |
| same with `expect(...,0)` | 9908 | 0.739255 | neutral |
| `bool notTurretMode` | 9900 | 0.705731 | size shift |
| `bool notOtherLock` | 9900 | 0.719897 | size shift |
| both `bool` | 9900 | 0.719897 | size shift |
| `notTurretMode = turretMode ? 0 : 1` | 9908 | 0.739255 | neutral |
| `notOtherLock = otherLock ? 0 : 1` | 9908 | 0.739255 | neutral |
| swap `notTurretMode`/`notOtherLock` declaration order | 9908 | 0.739255 | neutral |
| `int m = mode;` local alias for all five `+= mode` | 9908 | 0.739255 | neutral |
| `x = x + mode` instead of `x += mode` | 9908 | 0.739255 | neutral |
| `-mllvm -disable-ssc` | 9908 | 0.678321 | worse |
| `-mllvm -no-stack-slot-sharing` | 9908 | 0.674212 | worse |
| `-mllvm -enable-deferred-spilling` | 9932 | 0.676214 | worse |

The pass-7 note's larger tables (17 `otherLock` spellings, 14 type spellings, 6 loop
variable spellings, 3 declaration orders, `!(...)` in-range rewrite, distance-first
in-sight, `Sprite *blip` locals, etc.) all report the same conclusion: no 9908-byte
source variant beats 0.739255.

## 6. Conclusion

* The 80:1 override is a real, gate-neutral improvement: **72.4825 % -> 73.9255 %**.
  The `666d7c5e` comment claiming the per-function percentages were unchanged is false
  for `Radar::draw`; the comment has been corrected and the override restored.
* The 124-byte deficit is not missing code: the call multiset is complete apart from
  the single tail-merged landmark `DrawString`, and every cold/error block exists.
* The remaining gap is block placement (first `otherLock` cold block, duplicate
  autopilot block) plus `StackSlotColoring` (`#148`/`#152` swap) and the VFP save set.
  All are invariant under every source spelling tried so far.
* The semantically correct single-autopilot source (`del_campaign_ap`) is 9740 B /
  0.622903; the baseline's duplicate block is a codegen crutch. Landing the correct
  source requires reproducing the original's cold-tail placement without it, which no
  current 9908-byte source shape reaches.

## 7. Evidence

Scratch (not committed): `/dev/shm/w4241/` — `Radar.base.cpp` (baseline),
`CMakeLists.80.txt`/`CMakeLists.final.txt` (80:1 CMakeLists),
`o_body.txt` (original body disassembly), `u_body80.txt` (80:1 body),
`u_body_noflag.txt` (no-override body), `u_delcamp.txt` (campaign-block deleted),
`u_nossc.txt`, `smdiff.py` (diff map), `one.py` (ratio), `batch*.py` (variant sweeps).
