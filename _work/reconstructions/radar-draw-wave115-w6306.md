# Radar::draw pass (wave115, w6306) — register-allocation / block-placement sweep

**Status:** research note, 2026-11-05. Worktree `wt/w6306`, branch
`wave115-w6306`, based on current `master`. **No source change landed**; this
commit contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale). Original 9908 B.

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 raw 74.029509 %
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals). Baseline instruction count: **3492 original / 3489 ours**.
Structural state re-measured this wave and unchanged: 27 sections, exact file
size **2,187,328 B**, `.text` **1,259,464 B** on both sides, `.ARM.extab`
**156,312 B (0x26298)** on both sides.

## 1. What the original actually does (new evidence this wave)

Ghidra decompilation (`rea decompile`) and a full call-sequence alignment were
used to pin the remaining structural deltas. The call multisets differ only by:

```
  9   8  DrawString
  2   3  SolarSystem::getStations
  1   2  SolarSystem::getStationEnumIndex
 29  32  Level::getPlayer
  5   7  Status::getSystem
 17  16  KIPlayer::isDead
 13  12  KIPlayer::isDying
  1   2  PlayerEgo::goingToPlanet
  4   5  PlayerEgo::isDockingToPlanet
  1   2  PlayerEgo::getAutoPilotTarget
```

The `getStations/getStationEnumIndex/getPlayer/getSystem/goingToPlanet/
isDockingToPlanet/getAutoPilotTarget` deltas are exactly the semantically
redundant post-planet-loop `onPlanetCourse` recompute at source lines 615–626
(10 calls, ~152 B). The remaining deltas are one extra `DrawString` and one
extra `isDead`+`isDying` pair in the original.

### 1a. First `otherLock` chain — root cause is the register, not the source

Original `0x2fc..0x312`:

```
2fc mov  r1, r4
2fe ldr.w r0, [r1, #12]!      ; &lockedAsteroid, load lockedAsteroid
302 str  r1, [sp, #124]
304 cbnz r0, 312              ; a != null -> set
306 ldr  r0, [r4, #56]        ; lockedGasCloud
308 cbnz r0, 312              ; b != null -> set
30a ldr  r0, [r4, #28]        ; field_0x1c
30c cmp  r0, #0
30e beq.w ebc2                ; c == 0 -> cold: otherLock = field_0x14 != 0
312 movs r5, #1               ; single shared set block (2 B)
314 ldr  r0, [r4, #312]       ; landmark guard
```

Ours:

```
2fc mov  r1, r4
2fe ldr.w r0, [r1, #12]!
302 str  r1, [sp, #124]
304 cbz  r0, 324              ; a == null -> test b
306 mov.w r9, #1              ; duplicated set block (4 B, high reg)
30a ldr  r5, [sp, #156]       ; Globals::status hoisted into r5 here
30c b.n  338
324 ldr  r0, [r4, #56]
326 ldr  r5, [sp, #156]       ; status loaded again
328 cmp  r0, #0
32a itt  eq
32c ldreq r0, [r4, #28]
32e cmpeq r0, #0
330 beq.w 1305fa              ; cold (r9)
334 mov.w r9, #1
338 ldr  r0, [r4, #312]
```

The original keeps `otherLock` in **r5** and `Globals::status` in **r6**; ours
keeps `otherLock` in **r9** and `status` in **r5**. Because `mov.w r9,#1` is
4 B, clang duplicates the set block instead of branching to a shared 2 B
`movs r5,#1`. The root cause is that clang **hoists the `Globals::status` load
above the chain** in our TU; the original loads it only after the `landmarks`
null check. All wave-114 hint/term permutations (see
`radar-draw-wave114-w6206.md`) plus the new forms below still produce `cbz` +
duplicated set; no source spelling tried so far moves `otherLock` to r5.

### 1b. Asteroid loop — original computes the dead predicate twice

Ghidra renders the loop entry condition as

```c
if ((asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying())) ||
    (!asteroid->isDead() && !asteroid->isDying())) {
    update(asteroid);
    ...
}
```

i.e. `!(dead && !hasCargo)`. The original therefore emits **two** dead
predicate evaluations before `update` (isDead 0x12faae, isDying 0x12fab6,
isDead 0x12fabe, isDying 0x12faca) plus the `wreck` evaluation after
`update` (isDead 0x12fae4, isDying 0x12fb0e). Ours computes `dead` once
(isDead 0x12fad8, isDying 0x12fae0) plus `wreck` (isDead 0x12fb00,
isDying 0x12fb2a) — exactly one isDead+isDying pair short. Writing the
Ghidra form restores the pair (call multiset then matches) but the block
layout gets worse (see §2).

### 1c. Landmark generic-else `DrawString` is tail-merged in ours

Original has **9** `DrawString` calls; ours **8**. The original's generic
`else` (i != 0 || alienOrbit) ends with its own `blx DrawString` at 0x12e9e0
and then `ldr r5,[sp,#156]; mov r9,r6; b 12eb3c`. Ours branches from the
generic else straight into the i0 block's final `DrawString` at 0x12eaf2
(`b.n 12eaf2`), sharing the call. The merge is enabled by block placement:
ours puts the `(i & 0x7fffffff) == 1 || == 3` else-if block at 0x12e8ac
(immediately after the onScreen-false branch, before the loop body), while the
original puts it at 0x12e9ea, between the generic else and the i0 block.

### 1d. Prologue: original saves six d-registers

Original `vpush {d8-d13}`; ours `vpush {d8-d11}`. The original keeps
`PlayerEgo::crosshairPos.x/y` in **s26/s24** across the
`AEMath::operator-` call in the enemy in-sight block and converts them to int
between `operator-` and `VectorLength`. Our baseline converts to int before
the hit-box and never keeps a float live across a call.

## 2. Variants measured this wave (all dead ends)

All variants are single-edit changes on the pristine baseline unless noted.
`usize` is the emitted function size; `raw %` is the official fuzzy score.

### 2a. First `otherLock` chain forms

| variant | usize | raw % |
|---|---:|---:|
| baseline (nested negative AND, outer expect 0, inner expect 1) | 9908 | **74.029509** |
| plain positive OR `if (a||b||c) x=1; else x=d!=0;` | 9908 | 72.797593 |
| `bool anyLock = a||b||c; int x = anyLock ? 1 : d!=0;` | 9908 | 72.797593 |
| same, `if (anyLock) x=1; else x=d!=0;` | 9908 | 72.797593 |
| same, `bool anyLock = !(a==0&&b==0&&c==0)` | 9908 | 72.797593 |
| `int x = 1; if (expect(AND,0)) x = d!=0;` | 9908 | 74.000859 |
| `int x = d!=0; if (expect(OR,1)) x = 1;` | 9928 | 69.754768 |
| `int x = d!=0; if (expect(OR,0)) x = 1;` | 9928 | 69.544021 |
| chain computed before the mission block (live range extended) | 9844 | 64.627316 |
| `int otherLock;` declaration moved to function top | 9908 | 74.029509 |

### 2b. Asteroid `dead` predicate (Ghidra form)

```cpp
if (!(asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying()) ||
      (!asteroid->isDead() && !asteroid->isDying()))) {
    continue;
}
```

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| Ghidra form (restores the missing isDead+isDying pair) | 9928 | 71.465369 |

The call multiset then matches exactly (isDead 17/17, isDying 13/13), but the
extra pair shifts every later block and loses ~2.5 points.

### 2c. Enemy in-sight block (distance-first / float crosshair)

| variant | usize | raw % |
|---|---:|---:|
| baseline (hit-box first, `int cx/cy`) | 9908 | **74.029509** |
| distance first, `int cx/cy` | 9948 | 63.375431 |
| distance first, `float cx/cy`, inline `(int)` casts | 9948 | 63.318025 |
| distance first, no locals, inline `(int)crosshairPos.x` | 9928 | 62.715270 |
| distance first in a nested `if`, float locals | 9984 | 62.029483 |
| `bool inSight = dist<60000; if (inSight && hitbox) inSight=true;` | 9804 | 67.386920 |
| hit-box first, `float cx/cy`, inline casts | 9872 | 70.422535 |

The distance-first float form (F2) *does* restore `vpush {d8-d13}` and matches
the prologue, but the in-sight block's instruction order still differs and the
global alignment loss dominates.

### 2d. Landmark `i <= 1` block / else-if / tail-merge

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `if (i != 0 || alienOrbit) {generic} else {i0}` | 9912 | 72.289847 |
| `if ((i != 0) | alienOrbit) {generic} else {i0}` | 9904 | 72.206304 |
| `if (!(i == 0 && !alienOrbit)) {generic} else {i0}` | 9912 | 72.289847 |
| `if (i == 0) { if (!alienOrbit) i0 else generic } else generic` | 10016 | 71.064254 |
| `if (i != 0) generic else if (alienOrbit) generic else i0` | 9964 | 71.306087 |
| generic else + `continue;` | 9908 | 74.029509 |
| generic else via `String &label` / `String *label` | 9908 | 74.029509 |
| `int x/y` locals for the generic-else DrawString coords | 9908 | 74.029509 |
| `String label = this->lockLabel;` (breaks the merge, adds copy ctor/dtor) | 9928 | 73.195876 |
| else-if `i == 1 || i == 3` (mask optimized away) | 9908 | 74.029509 |
| else-if `expect(...,1)` / reversed terms | 9908 | 74.029509 |
| else-if no expect | 9912 | 72.242910 |
| else-if split into two expects | 9912 | 72.242910 |

### 2e. Guard / status caching / misc

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `Status *status = Globals::status;` before the guard | 9912 | 72.032078 |
| nested `if (landmarks) { if (!(...)) { ... } }` | 9908 | 74.029509 |
| `bool skipLandmarks = ...; if (landmarks && !skipLandmarks)` | 9916 | 70.150322 |
| `bool notOtherLock/notTurretMode` | 9900 | 72.180020 |
| swap `notTurretMode`/`notOtherLock` declarations | 9908 | 74.029509 |
| hit-box `size * 2` → `size + size` | 9908 | 74.029509 |
| hit-box `size * 2` → `(size << 1)` | 9908 | 74.029509 |
| enemy docking guard OR reordered (`turretMode || otherLock || isDockingToDockingPoint`) | 9884 | 68.684550 |
| enemy docking guard OR reordered (`otherLock || turretMode || ...`) | 9876 | 68.503372 |
| enemy docking guard with `int t/o` locals | 9888 | 67.364797 |
| landmark hit-box condition with bitwise `&` | 9908 | 70.890067 |
| landmark hit-box terms reordered | 9924 | 68.882521 |

### 2f. Removing the redundant recompute (block-placement anchor)

| variant | usize | raw % |
|---|---:|---:|
| baseline (recompute present) | 9908 | **74.029509** |
| recompute removed (`V_nodup`) | 9756 | 62.314026 |

Removing the 10-call recompute is semantically correct and matches the
original's call multiset, but it **destroys block placement**: without the
recompute as an anchor, clang sinks the landmark loop from 0x12e7b0 to
0x12f474 (after the gas/asteroid loops), producing a single 411-instruction
alignment gap. The recompute is therefore load-bearing for the fuzzy score
even though it is not in the original.

## 3. Conclusion

No variant beat the pristine baseline **74.029509 % / 9908 B**. The function
is at a strong source-level local optimum for this compiler. The remaining
deltas are dominated by two things that are not reachable by ordinary source
edits at this granularity:

1. **Register allocation of `otherLock`** (r9 vs the original's r5), caused by
   clang hoisting the `Globals::status` load above the first chain. This
   changes the branch polarity and duplicates the set block.
2. **Block placement** — the landmark loop is anchored after the planet loop
   only by the redundant recompute; the else-if block and the generic-else
   `DrawString` tail-merge follow from that placement.

Scratch artifacts (not committed) live in `/dev/shm/w6306/`:
`Radar.base.cpp` (pristine copy), `score.py`, `try.sh`, `show.py`, `diff.py`,
`cnt.py`, `gen_lm.py`, and the `V_*`/`A_*`/`F_*`/`I_*`/`L_*`/`E_*`/`S_*`/
`N_*`/`T_*`/`K_*`/`Q_*`/`R_*`/`B_*`/`P_*`/`C_*`/`H_*` variant files.
