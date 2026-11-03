# Radar::draw pass 11 — working Ghidra decompilation, the full call-multiset
# decomposition, and a 60-variant source sweep (w4406)

**Status:** research note, 2026-11-03. Worktree `wt/w4406`, branch
`wave94-w4406`, base `f262bd4b`. **No source change landed**: no variant that
keeps `Radar::draw` at 9908 bytes beats the baseline verifier ratio
(0.739255). This commit contains this note only.

## 0. Baseline (measured before and after this pass)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN5Radar4drawEP6PlayerP3Hudi`:

```
osize 9908  usize 9908  byte False linked False
official ratio 0.739255 (73.9255 %)  normalized stream 3492 (orig) / 3488 (ours)
```

`Radar.cpp` still carries the scoped `-mllvm -likely-branch-weight=80
-mllvm -unlikely-branch-weight=1` override (`CMakeLists.txt:86-87`).

## 1. New this pass: a *working* headless Ghidra decompilation

Earlier notes recorded Ghidra pseudocode as truncated for this function. That
was a project/image-base problem, not a Ghidra limitation. The shipped Ghidra
project uses the wrong image base for this `.so`. A fresh headless import
works:

```
# /dev/shm/w4406/ghidra_decomp5.py (scratch, not committed)
#   - import android_2.0.16_libgof2hdaa.so
#   - set TMode (Thumb)
#   - disassemble 0x13e050..0x140703   (Ghidra VA = file VA + 0x10000)
#   - CreateFunctionCmd / setBody
#   - decompile
```

Output: `/dev/shm/w4406/ghidra_radar5.txt`, 1676 lines, function body
`0013e050..00140703` (file VA `0x12e050..0x130703`, 9908 bytes). The
decompilation is complete and readable; it is the primary new evidence in
this pass. **The decompilation confirms that the current `Radar.cpp` control
flow is structurally correct except for the duplicate campaign autopilot
block** (see §3).

## 2. The call multiset decomposes exactly into three source differences

`calls.py` (official verifier API + `SequenceMatcher`, `autojunk=False`)
gives orig 342 calls vs ours 349:

| symbol | orig | ours | delta |
|---|---:|---:|---:|
| `PaintCanvas::DrawString` | 9 | 8 | **-1** |
| `SolarSystem::getStations` | 2 | 3 | +1 |
| `SolarSystem::getStationEnumIndex` | 1 | 2 | +1 |
| `Level::getPlayer` | 29 | 32 | +3 |
| `Status::getSystem` | 5 | 7 | +2 |
| `KIPlayer::isDead` | 17 | 16 | **-1** |
| `KIPlayer::isDying` | 13 | 12 | **-1** |
| `PlayerEgo::goingToPlanet` | 1 | 2 | +1 |
| `PlayerEgo::isDockingToPlanet` | 4 | 5 | +1 |
| `PlayerEgo::getAutoPilotTarget` | 1 | 2 | +1 |

The **+1/+3/+2/+1/+1/+1** group is *exactly* the call set of the duplicate
`if (!alienOrbit) { ... }` autopilot block after the planet loop
(`getAutoPilotTarget`, `getSystem` ×2, `getStations`, `getStationEnumIndex`,
`getPlayer` ×3, `goingToPlanet`, `isDockingToPlanet`) — already known from
w3804/w4202. The **-1/-1/-1** group is *new* and is **not** part of that
block:

* **`DrawString -1`** — the landmark loop's `else` branch
  (`DrawString(radarFont(), lockLabel, screenX + offset,
  screenY + layout->field_0x4, false)`) is **tail-merged** into the
  `i == 0 && !alienOrbit` branch's identical call in our build. The original
  keeps two separate `blx` sites:
  * original `0x12e9e0` (else branch)
  * original `0x12eb36` (TecLevel branch, `screenY + field_0x4*2`)
  Ours emits one shared `blx` at `0x12eaf2` and a `b.n` from the else branch.
* **`isDead -1` / `isDying -1`** — the asteroid dead predicate. The original
  evaluates `isDead()||isDying()` **twice** on the `hasCargo` path:
  ```
  ; original 0x12faa6
  ldrb.w r0,[r6,#0x4c]      ; hasCargo
  cbz    r0, 0x12fabc
  blx    isDead
  cbnz   r0, 0x12fad4       ; -> update
  blx    isDying
  cbnz   r0, 0x12fad4
  0x12fabc:
  blx    isDead
  bne.w  continue
  blx    isDying
  bne.w  continue
  0x12fad4: update(...)
  ```
  i.e. `if (!(hasCargo && (isDead||isDying)) && (isDead||isDying)) continue;`
  (the w4105 `AST` shape). Our source uses a `bool dead = isDead()||isDying();`
  local, which evaluates the pair once (2 calls instead of 4).

So the residual is **not** a missing statement — it is three codegen/placement
decisions (duplicate block, one tail merge, one common-subexpression
evaluation) that happen to cancel to the same 9908-byte size.

## 3. The original campaign block (Ghidra, authoritative)

`ghidra_radar5.txt` lines 387-560. The original is:

```c
if (campaign >= 2 && planetTargets != 0 && !alienOrbit) {
    if (!player->isDockingToPlanet()) {
        stations = status->getSystem()->getStations();
        planetHit = false;
        for (i = 0; i < planetTargets->size(); ++i) { ... }
        if (!turretMode && !landmarkHit) { ... planet-lock tail ... }
    }
}
```

There is **no** `if (!alienOrbit) { getAutoPilotTarget(); ... }` block after
the loop. The only `getAutoPilotTarget` call in the whole function is the
top-level one at `0x12e6f4` (the `field_0x1a8` computation before the 4
`DrawImage2D` calls). The `!isDockingToPlanet()` test is inside the outer `if`
in the decompilation, but it compiles to the same `bne.w enemies` as the
other outer-condition failures, so the C++ nesting is ambiguous and does not
matter.

Machine layout of the original (function-relative offsets, base `0x12e050`):

```
0x000  prologue / init / mission block / turretMode / alienOrbit / campaign /
       level array fetches / camera matrix / field_0x1a8 top-level block
0x16c  DrawImage2D x4
0x2ac  route block
0x2ac  otherLock #1 (hot: cbnz/cbnz/cmp/beq; cold at 0xb72)
0x2c2  landmark condition (station 109/110/111&&campaign>93)
0x326  otherLock #2 (hot: cbnz/cbnz/cmp/beq; cold at 0xb7e)
0x33a  campaign block preamble (getSystem/getStations, planetHit=false)
0x38c  planet loop body
0x69e  top-level autopilot block (cold, reached from 0x166)
0x6f8  planet-lock tail (reached from the loop's bcs.w)
0x758  landmark loop (cold, reached from 0x310/0x31e)
0xaf2  cold tail: otherLock#1-cold (0xb72), otherLock#2-cold (0xb7e),
       missionBlocksPlanets (0xb8a), planetLockTimer=0/field_0x18=0 (0xbcc)
0xbd4  enemies:
```

In our build the planet-lock tail is instead emitted **after** the duplicate
block's cold half (ours `0x12ebec`), and the `otherLock#1` cold block is
emitted at `0x1304d0` (function-relative `0x2480`), not in the cold tail.

## 4. Stack-slot map (original vs ours)

| variable | original | ours |
|---|---|---|
| `hud` (r2) | `[sp,#84]` | `[sp,#84]` |
| `mode` (r3) | **`[sp,#100]`** | **`[sp,#112]`** |
| `player` (r1) | `[sp,#136]` | `[sp,#136]` |
| `turretMode` | **`[sp,#148]`** | **`[sp,#152]`** |
| `landmarkHit` / `planetHit` | **`[sp,#152]`** | **`[sp,#148]`** |
| `notTurretMode` / `landmarkHit` | `[sp,#108]` | `[sp,#108]` |
| `missionBlocksPlanets` | **`[sp,#112]`** | **`[sp,#116]`** |
| `notOtherLock` / `stations` | `[sp,#116]` | `[sp,#116]` |
| `&lockLabel` | `[sp,#104]` | `[sp,#100]` |
| `otherLock` | `[sp,#144]` | `[sp,#144]` |
| `&lockedAsteroid` | `[sp,#124]` | `[sp,#124]` |

So it is a **three-way rotation**, not a single swap:

```
original:  #100=mode   #104=&lockLabel  #108=notTurretMode  #112=mbp    #116=notOtherLock
ours:      #100=&lockLabel  #104=?      #108=notTurretMode  #112=mode   #116=mbp
```

The *use counts* are identical: both builds touch the `mode` slot exactly six
times (one `str r3` in the prologue + five `ldr` at the five `+= mode`
sites):

```
orig  [sp,#100]: 12e05e(str) 12f85e 12fe3c 12febc 12ff9c 1303c2
ours  [sp,#112]: 12e05e(str) 12f84a 12fe48 12feba 12ff96 1303c2
```

So this is pure `StackSlotColoring` / spill-slot-index ordering, not a
missing or extra use. Declaration-order and type sweeps (§7) do not move it.

## 5. The other structural residuals, measured

* **`vpush {d8-d13}` vs `{d8-d11}`** (orig vs ours). The original's enemy
  in-sight test is distance-first and keeps `crosshairPos.x/.y` in `s26`/`s24`
  across the `AEMath::operator-` call:
  ```
  ; original 0x12ef4c
  ldr   r0,[pc]            ; &PlayerEgo::crosshairPos
  movs  r6,#0
  strb.w r6,[r8,#0x73]     ; enemy->field_0x73 = 0
  ldr   r0,[r0,#0]
  vldr  s26,[r0]           ; crosshairPos.x
  vldr  s24,[r0,#4]        ; crosshairPos.y
  blx   AEMath::operator-
  vcvt.s32.f32 s24,s24     ; cy
  vcvt.s32.f32 s26,s26     ; cx
  blx   AEMath::VectorLength
  vcmpe.f32 s0,s20         ; < 60000.0f
  bpl   0x12efc0           ; -> false
  ; then x-pair, then y-pair bounds checks
  ```
  i.e. `if (VectorLength(egoPos-targetPos) < 60000.0f &&
  screenX > cx-size && screenX < cx+size &&
  screenY > cy-size && screenY < cy+size)`.
  Our source tests the four screen bounds first, interleaved
  (`x>`, `y>`, `x<`, `y<`), then `VectorLength`.
* **First `otherLock` chain**: original `cbnz/cbnz/cmp/beq` (22 B), ours
  `cbz` + `itt eq/ldreq/cmpeq/beq` (52 B region incl. the `mov.w r9,#1` hot
  arm). The *second* `otherLock` chain has the original's
  `cbnz/cbnz/cmp/beq` shape in our build too. The IR of the two chains is
  identical (`-emit-llvm -S`), so this is a global
  MachineBlockPlacement/IfConverter decision (already established by
  w4271).
* **Planet-lock blip**: original `ldr.w r5,[r4,#244]; mov r0,r5; ...;
  vldr s0,[r4,#420]; vldr s2,[r4,#440]`; ours
  `mov r5,r4; ldr.w r0,[r5,#244]!; ...; vldr s0,[r5,#176];
  vldr s2,[r5,#196]` — the compiler chose `r5 = &this->blipSprite` as a base
  in our build. Same instruction count, different registers/offsets.

## 6. `delcamp` (duplicate block removed) diverges completely

Removing the duplicate `if (!alienOrbit)` block gives **9740 B / 0.622903**
(normalized stream 3492/3424, i.e. 68 instructions short). Its diff against
the original is *not* local: the whole register allocation and stack layout
change (e.g. `mode` moves to `[sp,#104]`, the landmark loop index moves from
`sl` to `r5`, the planet tail moves to `0x708`, ...). The duplicate block is
therefore **load-bearing for the entire function's allocation**, not just for
its own bytes. This is why every "semantically correct" single-autopilot
variant scores far below the baseline.

## 7. Variant sweep (all measured with the official verifier, `autojunk=False`)

Baseline: **9908 / 0.739255** (3492/3488).

### 7.1 Declaration order / type changes (all `usize 9908` unless noted)

| variant | usize | ratio |
|---|---:|---:|
| `landmarkHit` declared before `otherLock` | 9908 | 0.739255 |
| `otherLock` declared at function top | 9908 | 0.739255 |
| `int m = mode;` local + `+= m` everywhere | 9908 | 0.739255 |
| `bool hasDockNav` → `int` | 9908 | 0.739255 |
| `bool candidateWreck` → `int` | 9908 | 0.739255 |
| `bool navWreck` → `int` | 9908 | 0.739255 |
| `int marked = false` → `= 0` | 9908 | 0.739255 |
| `bool inSight` → `int` | 9908 | 0.739255 |
| `bool ambient` → `int` | 9908 | 0.739255 |
| `bool topOk` → `int` | 9908 | 0.738682 |
| `bool gasHit` → `int` | 9908 | 0.738968 |
| `bool bossSeen` → `int` | 9908 | 0.722740 |
| `bool planetHit` → `int` | 9896 | 0.713016 |
| `bool enemyHit` → `int` | 9928 | 0.699485 |
| `bool wreck` → `int` | 9920 | 0.718294 |
| `bool alwaysEnemy` → `int` | 9904 | 0.715391 |
| `bool asteroidHit` → `int` | 9908 | 0.688154 |
| `bool missionBlocksPlanets` → `int` | 9904 | 0.726830 |
| `bool turretMode` → `int` | 9888 | 0.660643 |
| `bool alienOrbit` → `int` | 9904 | 0.721880 |
| `turretMode`/`alienOrbit`/`campaign` decl permutations (6) | 9908 | 0.739255 best (current order), 0.719484 worst |

### 7.2 `otherLock` condition permutations

* First chain (`lockedAsteroid && lockedGasCloud && field_0x1c`), 6 orders:
  `ABC`/`ACB` neutral 0.739255, `BAC`/`BCA` 9896/0.708632,
  `CAB`/`CBA` 9932/0.663134.
* Second chain (`lockedAsteroid || lockedGasCloud || field_0x1c ||
  lockedStation`), all 24 orders: **current `ABCD` is the best**
  (0.739255); next best 0.738968; `C`-first orders collapse to
  9940/0.664758.
* Third + fourth chains (post-enemies and asteroid `otherLock`), all 24
  orders: **current order is the best** (0.739255); next best 0.738682.

### 7.3 Blip local-pointer shapes

| variant | usize | ratio |
|---|---:|---:|
| enemy-lock: drop `Sprite *blip`, use `this->blipSprite` | 9908 | 0.738501 |
| gas-lock: drop `Sprite *blip` | 9928 | 0.639782 |
| gas-lock: keep `blip` for the second `getRawFrameCount` | 9900 | 0.699986 |
| planet-lock: add `Sprite *blip` local | 9896 | 0.720390 |
| station-lock: add `Sprite *blip` local | 9904 | 0.730113 |
| asteroid-lock: add `Sprite *blip` local | 9904 | 0.721490 |

### 7.4 Asteroid dead-predicate shapes

| variant | usize | ratio |
|---|---:|---:|
| baseline (`bool dead = ...; if (dead && !hasCargo)`) | 9908 | 0.739255 |
| `if (!hasCargo && dead)` | 9908 | 0.737717 |
| `if (!hasCargo && (isDead()||isDying()))` | 9912 | 0.719565 |
| `if (!(hasCargo && ...)) { if (...) continue; }` (AST) | 9928 | 0.712466 |
| `if (!hasCargo) { if (isDead()||isDying()) continue; }` | 9912 | 0.719565 |

### 7.5 Enemy in-sight shapes

| variant | usize | ratio |
|---|---:|---:|
| baseline (screen-first, interleaved) | 9908 | 0.739255 |
| distance-first, interleaved (`v_insight_distfirst`) | 9924 | 0.711894 |
| distance-first, x-pair/y-pair (`v_insight_distfirst_int`) | 9952 | 0.631941 |

### 7.6 Tail-merge knobs (per-file `-mllvm` on top of the 80:1 override)

| flag | usize | ratio |
|---|---:|---:|
| (none) | 9908 | 0.739255 |
| `-tail-merge-size=2` / `=3` / `=4` | 9908 | 0.739255 |
| `-tail-merge-size=5` / `=8` | 9920 | 0.727741 |
| `-disable-early-taildup` | 9912 | 0.720057 |
| `-tail-merge-threshold=3` | 9952 | 0.696883 |
| `-tail-merge-threshold=2` | 10148 | 0.584628 |
| `-tail-merge-threshold=0` | 10444 | 0.549646 |

Note `-tail-merge-threshold=0` removes 217 merged instructions, so tail
merging is doing a great deal of work in this function; disabling it is not
viable. `-tail-merge-size=2..4` is a plateau at the baseline.

### 7.7 Positive-OR cold-tail lever (re-measured)

| variant | usize | ratio |
|---|---:|---:|
| baseline | 9908 | **0.739255** |
| positive OR, no `expect` | 9904 | 0.721697 |
| positive OR + `onScreen` plain | 9892 | 0.708698 |
| positive OR + `-disable-early-taildup` | 9908 | 0.724276 |

The positive-OR spelling does move the `otherLock#1` cold block into the cold
tail (w4271), but it costs 4 bytes and the pool shift dominates.

## 8. Conclusion

* The function's **statement set is now fully understood**: the only call-set
  differences are the duplicate campaign autopilot block (+7 calls), one
  tail-merged landmark `DrawString` (-1), and one common-subexpression
  asteroid dead predicate (-2). Everything else in the 342-call set matches
  exactly.
* The remaining 26 % of the stream is **block placement + register/stack-slot
  allocation**, not source statements:
  1. `otherLock#1` cold block at `0x2480` (ours) vs `0xb72` (orig);
  2. the planet-lock tail after the duplicate block's cold half (ours) vs
     immediately after the planet loop (orig);
  3. `mode`/`&lockLabel`/`missionBlocksPlanets` stack-slot rotation;
  4. `vpush {d8-d11}` vs `{d8-d13}` (needs `s24`/`s26` crosshair floats);
  5. `cbz+itt` vs `cbnz` lowering of the first `otherLock` chain;
  6. `r5 = &blipSprite` write-back base at the planet-lock blip.
* The duplicate autopilot block is a **global** allocation crutch: removing it
  (`delcamp`) drops the body to 9740 and re-randomizes the whole register
  assignment, so it cannot be traded for the missing DrawString/isDead/isDying
  bytes locally.

### What remains

1. Find a source shape that reproduces the original's `mode` spill slot
   (`[sp,#100]`) while keeping the duplicate block. The use counts are
   identical (6 each), so this is spill-slot creation order, i.e. a register
   allocator artifact — probably not reachable from C++ at all.
2. If a 4-byte-shrinking change can be found that is *also* neutral for the
   stream (e.g. a cold-block reorder), combine it with the positive-OR form
   (9904 / 0.721697) to reach 9908 with the original's cold-tail order.
3. Reproduce the original's `s24`/`s26` crosshair floats **without**
   reordering the screen tests (the distance-first reorder costs 16 B of
   `vpush` and a large layout shift).

## 9. Evidence / scratch (all under `/dev/shm/w4406`, not committed)

* `ghidra_decomp5.py` — working headless Ghidra decompile (see §1).
* `ghidra_radar5.txt` — full 1676-line decompilation of `Radar::draw`.
* `measure.py` — official-verifier ratio for one symbol.
* `dumpnorm.py`, `show.py`, `addrlist.py`, `calls.py`, `callsites.py`,
  `callalign.py`, `diff.py` — normalised-stream and call-set diff tools.
* `sweep.py` … `sweep8.py` — the variant sweeps in §7.
* `try.sh`, `tryflag2.sh` — build-and-measure harnesses (the latter patches
  `CMakeLists.txt` and restores it, avoiding the `tryflag.sh` stale-object
  bug).
* `delcamp.cpp`, `mode_local.cpp`, `v_insight_distfirst*.cpp`,
  `c_AST.cpp`, `c_*.cpp`, `m_*.cpp`, `var_*.cpp` — all variant sources.
* `Radar.base.cpp` — the landed baseline source.
* `orignorm.txt`, `oursnorm.txt`, `dc_norm.txt`, `ins_norm.txt`,
  `posor_norm.txt`, `oaddr.txt`, `uaddr.txt`, `diff.txt` — dumps.
