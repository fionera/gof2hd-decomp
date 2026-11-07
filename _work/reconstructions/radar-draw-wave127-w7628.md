# Radar::draw pass (wave 127, w7628) — single-recompute layout + Ghidra cross-check, no source win

**Status:** research note, 2026-11-07. Worktree `wt/w7628`, based on current
`master` (`4bbe88776`). **No source change landed**; this commit contains this
note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`
(`sha256 878a70b21ffaf509ff68130767f6630cc7a0895958fe1eed12ba0b7413bb522c`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.268817 %** (official display 75.3). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, data 843/843, imports 207,
lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave126-w7606.md`, `radar-draw-planet-loop-wave125-w7512.md`,
`radar-draw-wave125-w7511.md`, `radar-draw-body-wave122-w7210.md`. This wave
adds two things the earlier notes did not have: (a) a **Ghidra decompile of the
original** (`FUN_0013e050`, image base 0x10000), and (b) a measurement of the
**single-recompute** body, which reproduces the original's planet-lock tail and
landmark-setup *addresses* but re-rolls the planet loop's register split.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7628
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra (the project's image base is **0x10000**, so the ELF vaddr `0x12e050`
is Ghidra `0x13e050`):

```sh
cd /home/debian/GalaxyOnFire2/_work/bins
/tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless . \
  android_2.0.16_libgof2hdaa.so_ghidra -process android_2.0.16_libgof2hdaa.so \
  -noanalysis -scriptPath /home/debian/ghidra_scripts -postScript DecompRadar.java
```

`DecompRadar.java` decompiles `toAddr(0x13e050)`; output saved in
`/dev/shm/w7628/ghidra_radar.cpp` (1864 lines). The decompile is coherent for
this function (unlike the `operator new` cases): every call site is visible.

## 2. Ghidra confirms the logic is complete

Whole-function call-multiset diff (original vs current baseline) is exactly the
invented duplicate recompute plus the local `String` label:

```
_ZN11AbyssEngine6StringaSERKS0_@plt       6 / 5   (String::operator=)
_ZN11SolarSystem11getStationsEv@plt       2 / 3
_ZN11SolarSystem19getStationEnumIndexEi@plt 1 / 2
_ZN5Level9getPlayerEv@plt                29 / 30
_ZN6Status9getSystemEv@plt                5 / 7
_ZN9PlayerEgo18getAutoPilotTargetEv@plt   1 / 2
```

Every other callee (including `isDead` 17 / `isDying` 13) matches exactly. So
the 9-instruction deficit is **not missing logic**; it is codegen (3 separate
bool loads vs one `ldrd`, the `marked` bool canonicalisation, the `stations`
rematerialisation, and the rotated landmark loop).

The Ghidra decompile shows the original's planet section is exactly the source
shape already in `Radar.cpp`:

* `stationId = stations->data()[i]` (kept in `r8`);
* the campaign `isTarget` is `stations->data()[i] == campaignMission->getTargetStation()`
  (a fresh load into `sl`, not `stationId`);
* one `marked` int, one `textOffset`, one `planetHit`;
* one recompute total (the prologue-else block), placed at `0x12e6ee` after the
  planet loop but reached only from `0x12e1b6`; the planet-loop exit
  (`bcs.w 0x12e748`) goes straight to the planet-lock tail.

**Execution order vs layout order (important).** The source order is landmark
block then planet section. The binary lays the planet section out first
(`0x12e376`–`0x12e748`) and the landmark loop second (`0x12e7a8`–`0x12ebbe`),
and the landmark tail ends with `b.w 0x12e376`. So the *execution* order is
guard → landmark loop → planet section → planet-lock tail → enemies. That is
why `r5` at the landmark setup (`0x12e7c8 eor r0,r5,#1`) is still
`otherLock` (loaded by the guard, `0x12e312`/`0x12ebc2`), not the
`missionBlocksPlanets` value stored at `[sp,#112]` later by the planet section.

## 3. The single-recompute body: right block addresses, wrong registers

Removing the invented post-loop recompute **and** making the campaign
`isTarget` use `stations->data()[i]` gives a 9864 B body. Its block addresses
now line up with the original exactly:

| block | original | single-recompute variant |
|---|---|---|
| planet-lock tail | `12e748` | `12e748` |
| landmark setup | `12e7a8` | `12e7a8` |
| landmark loop head | `12e7d4` | `12e7d0` (rotated) |
| enemies | `12ec24` | `12ec28` |

but the score drops to **72.934063 %** (3454 insns, 38 short) because the
planet-loop register split re-rolls:

| value | original | variant |
|---|---|---|
| `i` (planet loop) | `fp` | `sl` |
| `stations` | `sl`, spilled to `[sp,#116]` | `[sp,#148]` |
| `stationId` | `r8` | `r6` |
| `currentStation` | `r9` | `r8` |
| `autoPilotTarget` (recompute) | spilled to `[sp,#152]` | kept in `fp` |

The root cause is the recompute's preservation strategy. In the original the
recompute rotates `fp`→`r9`, `r8`→`fp`, `sl`→`r8`, `sl`=planetTargets, and
spills `autoPilotTarget` to `[sp,#152]`; `fp` is therefore free to be the
planet-loop index. In the variant the recompute spills `fp` to `[sp,#108]` and
keeps `autoPilotTarget` in `fp`, so the loop index is pushed to `sl` and
`stations` to the stack. The two decisions are mutually dependent: the loop
index assignment changes the recompute's register pressure, which changes the
index assignment.

## 4. Variants measured this wave (all single edits on the pristine baseline unless noted)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.268817** |
| remove recompute only (`rec_none`) | 9856 | 70.76 |
| campaign `isTarget` → `stations->data()[i]` (`camp_reload`) | 9904 | 73.444221 |
| `rec_none` + `camp_reload` (single-recompute body) | 9864 | 72.934063 |
| `rec_none` + `camp_reload` + faithful `this->lockLabel` | 9868 | 70.750647 |
| faithful `this->lockLabel` alone | 9904 | 72.628260 |
| `notOtherLock = !missionBlocksPlanets` | 9856 | 72.789996 |
| `rec_none`+`camp_reload`+`notOtherLock=!missionBlocksPlanets` | 9808 | 71.556454 |
| `int missionBlocksPlanets` | 9900 | 72.354459 |
| `int turretMode` | 9888 | 67.374767 |
| `int alienOrbit` | 9908 | 75.258028 |
| `uint32_t campaign` | 9908 | 75.268817 (tie) |
| `bool landmarkHit` / `bool planetHit` / `int otherLock` | 9908 | 75.268817 (tie) |
| `bool otherLock` | 9812 | 69.077810 |
| `bool notOtherLock` | 9920 | 71.934605 |
| `bool notTurretMode` | 9844 | 73.446490 |
| `bool notOtherLock`+`bool notTurretMode` | 9920 | 71.934605 |
| `int landmarkHit` | 9884 | 71.010333 |
| `int planetHit` | 9896 | 72.844642 |
| `planetHit` declared before `stations` / after `for` / before `if` | 9908 | 75.268817 (tie) |
| single-recompute + `int i` / `unsigned i` / `const stations` | 9864 | 72.934063 (tie) |
| single-recompute + `bool notOtherLock` / `bool notTurretMode` | 9876 / 9808 | 70.19 / 70.54 |

The baseline is a strong local optimum: every structural change that moves the
planet-lock tail or the landmark setup into the original slot also re-rolls the
planet loop's `fp`/`sl` split and loses 2–5 points.

## 5. Remaining root cause

The function is a coupled fixed point:

1. The original's planet-loop index is `fp` and `stations` is `sl` (spilled);
   the variant's is `sl` and `stations` on the stack.
2. The recompute's `autoPilotTarget` spill/register decision is the other half
   of the same allocation; it cannot be steered independently from source
   without changing the loop index.
3. The landmark loop is bottom-rotated in ours and top-tested in the original;
   the second literal pool therefore lands after the setup (`0x12e7f4`) instead
   of before it (`0x12e79c`), which alone mismatches every `ldr [pc,#…]` and
   every branch past the pool.
4. `marked` is canonicalised (`marked = marked != 0`) in the original's
   `==125` path because the original reloads it from `[sp,#132]` across
   `getCurrentCampaignMission()`; ours keeps it in a register.
5. The first literal pool is at `0x12e340` (original) vs `0x12e310` (ours),
   the wave-125 water-block fixed point.

A real win needs a source shape that (a) keeps the single recompute, (b) puts
the planet-loop index in `fp` and `stations` in `sl`+spill, and (c) un-rotates
the landmark loop. That combination was not reachable from the spellings tried
this wave; the baseline (with the duplicate recompute) remains the best
`difflib` score at 75.268817 %.

## 6. Artifacts

Scratch in `/dev/shm/w7628` (not committed): `Radar.base.cpp` (pristine
source), `Radar.variant.cpp` (single-recompute body), `score.py`, `try.sh`,
`ops.py`, `ops.txt`/`ops2.txt` (difflib opcode dumps), `orig.dis`/`ours.dis`/
`ours2.dis`, `ghidra_radar.cpp` (Ghidra decompile of `FUN_0013e050`),
`batch.py`–`batch4.py`. The analysis is self-contained in this note.
