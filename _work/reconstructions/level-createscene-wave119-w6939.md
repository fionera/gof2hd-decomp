# `Level::createScene` — wave-119 w6939: no source win over 92.8000 %

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w6939`, branch
`wave119-w6939` (base master `98f531de9`). Target
`_ZN5Level11createSceneEv`, `src/game/world/Level.cpp`. No source change
landed; this note records the measurements so the next pass does not repeat
them.

Continuation of `level-createscene-wave119-w6903.md` (the most complete
prior note: full measurement tables, MIR section, reproduction commands),
`…-w6820.md`, `…-w6735.md`, `…-w6709.md` (which landed the `-g`
Vector-address trick, the y/z/x store order and the ship-loop `rnd` mask,
90.7243 -> 92.7573 %).

## TL;DR

* Baseline and best: **92.8000 % / 2252 B**, 791/783 instructions, first
  diff at normalized instruction 478 (parts-loop store schedule).
* The function is instruction-exact up to instruction 477 and from 484 to
  547. The only two differing regions are (a) the parts-loop store
  schedule (478-483) and (b) the ship-loop register allocation (548
  onward).
* ~70 new source variants were measured this session. **None beat
  92.8000 %.** The two structural facts below are new and close off the
  remaining search space:
  1. Any parts-loop address form that *folds back to `&rot`* (e.g.
     `rp + g`, `&rot - g + g`, a `Vector &rr = rot` alias, a `float *`
     alias, `Vector rot(0,pi,0)`) reverts to the 84.9 % basin. The
     `subs r1, r0, r5` is load-bearing, not incidental.
  2. The ship-loop `rnd` local must be used at **all three** of A, B and R
     (the two `nextInt` calls inside the race roll and the *nested retry
     loop*). Any proper subset collapses: A+B = 68.5, A+R = 86.7,
     B+R = 88.3, R only = 66.5, A only = 68.6, B only = 68.2, C only
     = 75.4. The nested-loop use at R is what gives the value its spill
     weight.
* Gate re-measured green after restoring the landed source:
  **byte_exact 4505/4524, linked_exact 4505/4524, stub_zero_size 0,
  missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
  lint CLEAN, drift 0, RATCHET PASS.** 27 sections, file size
  2,187,328 B.

## 1. The two remaining diff regions

### 1.1 Parts loop (instructions 478-483)

Original (`g->setRotation(rot)` — natural address, three separate stores,
zero materialised after the y store):

```
ldr   r0, [pc, #808]      ; pi
add   r1, sp, #56         ; &rot
str   r0, [sp, #60]       ; rot.y = pi
mov.w sl, #0              ; zero
mov   r0, r5              ; g
str.w sl, [sp, #56]       ; rot.x = 0
str.w sl, [sp, #64]       ; rot.z = 0
blx   setRotation
```

Landed (`- (size_t) g` trick — `subs` address, `strd` merge):

```
ldr   r0, [pc, #808]      ; pi
mov.w sl, #0              ; zero
str.w sl, [sp, #64]       ; rot.z = 0
strd  sl, r0, [sp, #56]   ; rot.x = 0, rot.y = pi (merged)
add   r0, sp, #56
subs  r1, r0, r5
mov   r0, r5
blx   setRotation
```

Both regions are 26 bytes / 8 instructions. The original's store order is
y, x, z; because y(60) then x(56) is a *decreasing* offset pair the ARM
load/store optimiser cannot merge them, and x(56)/z(64) are not adjacent,
so all three stores stay separate. In the landed build the scheduler puts
x and y adjacent, so they merge into `strd`.

### 1.2 Ship loop (instructions 548 onward)

Original (all eight sites use `Globals::rnd->`; the GOT-derived address of
`Globals::rnd` is hoisted into **r8** and the pointer value is reloaded
with `ldr.w r0,[r8]` at every site):

```
ldr   r0, [pc, #632]      ; GOT
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr.w r8, [r0]            ; r8 = &Globals::rnd
ldr   r0, [sp, #52]       ; sceneRace
ldr.w r6, [r1, r0, lsl#2] ; capacity -> r6
ldr.w r0, [r8]            ; Globals::rnd
adds  r1, r6, #1
blx   nextInt
```

Landed (local `AERandom *rnd = Globals::rnd;` used at A, B and R; the value
is cached in r5 and the address is spilled):

```
ldr   r0, [pc, #632]
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr   r2, [r0, #0]        ; r2 = &Globals::rnd
ldr   r0, [sp, #52]
str   r2, [sp, #44]       ; spill &Globals::rnd
ldr.w fp, [r1, r0, lsl#2] ; capacity -> fp
ldr   r0, [r2, #0]        ; Globals::rnd
add.w r1, fp, #1
blx   nextInt
```

Register map in the ship loop:

| value | original | landed |
|---|---|---|
| `spotUsed` | r4 | r4 |
| `race` | r5 | r6 |
| `capacity` | r6 | fp |
| `&Globals::rnd` | **r8** | spilled `[sp,#44]` |
| `i` | r9 | r9 |
| `status` | sl | sl |
| `type` | fp | r8 |
| `Globals::rnd` value | reloaded `[r8]` at every site | cached in r5 |
| `this` | spilled `[sp,#48]` | spilled `[sp,#48]` |

The landed build already matches the original's `this`-spilled /
`status`-in-`sl` assignment (and the whole prologue, mission-2 and
mission-4 branches are instruction-exact). The only mismatch is the extra
loop-live `rnd` value.

## 2. The central basin conflict (re-confirmed)

* **Local-value basin (landed, 92.8000 %):** `Globals::status` in `sl`,
  Canvas in `r8`, table base in `r9`, parts-loop zero in `sl`; ship loop
  caches `rnd` in r5 and spills the address.
* **All-global basin (75.4000 %, 2232 B, fd 6):** removing the local `rnd`
  entirely gives the original's `ldr rX,[addr]` ship-loop shape, but the
  whole function collapses to a different allocation: `Globals::status` in
  `r4` (spilled), `this` kept in `sl`, `&Globals::rnd` in `r9`, `capacity`
  in `r8`, `i` spilled across the retry loop, and the parts-loop preheader
  `r8`/`r9` swap. First diff at instruction 6 (the `this` spill store is
  scheduled before the first `beq` instead of after the second `cmp`).

The register budget in both builds is the same seven callee-saved
registers (`r4,r5,r6,r8,r9,sl,fp`; `r7` is the frame pointer). The landed
shape has **eight** loop-live values, so exactly one must spill, and the
allocator picks `&Globals::rnd`. The original has seven, so nothing spills.
This is a pure assignment/priority difference; no source shape found in
six sessions bridges it.

## 3. Measurements this session (all 2252 B unless noted)

Scored with the official single-function verifier (unrounded difflib
ratio). `pct` is the `--show` structural ratio; `fd` is the first differing
normalized instruction index. Note that `fd` 4/5/6/28 is the basin change;
`fd` 477/478 is the parts-loop store schedule; `fd` 133 is the mission-4
`r9`/`sl` swap.

### 3.1 Parts loop: any form that folds back to `&rot` reverts the basin

All of these keep the landed ship loop (`rnd` local at A, B, R).

| variant | pct | size | fd |
|---|---|---|---|
| **landed `- (size_t) g`, y,z,x** | **92.8000** | 2252 | 478 |
| `- (size_t) g`, y,x,z | 92.6000 | 2252 | 477 |
| `- (size_t) g`, x,z,y | 92.6000 | 2252 | 477 |
| `- (size_t) g`, `rp` declared before the stores, y,x,z | 92.6000 | 2252 | 477 |
| `- (size_t) g`, `rot.x = rot.z = 0.0f` (y first) | 92.8000 | 2252 | 478 |
| `- (size_t) g`, `Vector rot = Vector();` then y,z,x | 92.8000 | 2252 | 478 |
| `- (size_t) g` via `AEGeometry *const cg = g` | 92.8000 | 2252 | 478 |
| `- (size_t) g` inlined into the call (`setRotation(*(Vector *)…)`) | 92.8000 | 2252 | 478 |
| `const float pi = 3.1415927f;` local, then `rot.y = pi` | 92.8000 | 2252 | 478 |
| `float *f = (float *)&rot; f[0]=0; f[2]=0;` + trick | 92.6000 | 2252 | 477 |
| `float *f = (float *)&rot;` then `(char *)f - (size_t)g` | 92.6000 | 2252 | 477 |
| `float *f`, f[0]=0, y=pi, f[2]=0 | 92.6000 | 2252 | 477 |
| `float *f`, f[0]=0, y=pi, z=0 | 92.6000 | 2252 | 477 |
| `- ((size_t) g + 1)` (the pre-bfe6d18d2 form) | 88.1000 | 2252 | 28 |
| `float *f`, f[0]=0, y=pi, f[2]=0 (x,y,z order) | 88.3000 | 2252 | 28 |
| **plain `g->setRotation(rot)`, y,x,z** | 84.9000 | 2248 | 28 |
| plain, y,z,x | 84.9000 | 2248 | 28 |
| plain `Vector rot(0.0f, 3.1415927f, 0.0f)` | 85.0000 | 2248 | 28 |
| plain `Vector rot = {0.0f, 3.1415927f, 0.0f}` | 85.0000 | 2248 | 28 |
| plain `setRotation(Vector(0,pi,0))` temporary | 85.0000 | 2248 | 28 |
| plain + `Vector &rr = rot` alias | 84.9000 | 2248 | 28 |
| plain + `setRotation(*(Vector *)&rot)` | 84.9000 | 2248 | 28 |
| plain + `const Vector &rr = rot` | 84.9000 | 2248 | 28 |
| plain + `AEGeometry *gg = g` | 84.9000 | 2248 | 28 |
| plain + `if (g == nullptr) continue;` | 84.9000 | 2248 | 28 |
| plain + `Vector *rp = &rot` | 84.9000 | 2248 | 28 |
| `rp = (Vector *)((char *)&rot - (size_t)g); rp = (Vector *)((char *)rp + (size_t)g);` | 84.9000 | 2248 | 28 |
| `setRotation(*(Vector *)((char *)&rot - (size_t)g + (size_t)g))` | 84.9000 | 2248 | 28 |
| `Vector rot;` declared before `AEGeometry *g = new …` | 79.0000 | 2260 | 5 |
| `Vector rot;` declared before the `if (mesh < 0) continue;` | 79.0000 | 2260 | 5 |
| `Vector rot;` declared before the `for (int k = 0; k < 4; k++)` loop | 84.3000 | 2264 | 28 |
| `Vector rot;` wrapped in a nested `{ … }` block | 92.8000 | 2252 | 478 |
| plain parts + landed ship loop | 84.9000 | 2248 | 28 |
| plain parts + all-global ship loop | 74.7000 | 2224 | 6 |
| plain parts + all-global ship loop, no local at all | 74.7000 | 2224 | 6 |

### 3.2 Ship loop: the `rnd` local must be used at A, B *and* R

All of these keep the landed parts loop (trick, y,z,x).

| local `rnd` sites | pct | size | fd |
|---|---|---|---|
| **A, B, R (landed)** | **92.8000** | 2252 | 478 |
| A, B, C, R | 91.7000 | 2252 | 28 |
| A, B, D, R | 91.9000 | 2252 | 28 |
| A, B, E, R | 92.1000 | 2252 | 28 |
| A, B, F, R | 92.4000 | 2252 | 28 |
| A, R | 86.7000 | 2260 | 28 |
| B, R | 88.3000 | 2252 | 28 |
| A, B (R global) | 68.5000 | 2232 | 4 |
| A, B, C (R global) | 68.5000 | 2232 | 4 |
| A only | 68.6000 | 2232 | 4 |
| B only | 68.2000 | 2232 | 4 |
| C only | 75.4000 | 2232 | 6 |
| R only | 66.5000 | 2236 | 4 |
| none (all-global) | 75.4000 | 2232 | 6 |

The `R` site is the `spot = rnd->nextInt(capacity);` inside the *nested*
retry loop. Its use inside a nested loop is what gives the value enough
spill weight to take r5 and thereby pin the prologue basin.

### 3.3 Ship loop: declaration position / form / statement probes

| variant | pct | size | fd |
|---|---|---|---|
| `spots` declared before `rnd` | 92.8000 | 2252 | 478 |
| `AERandom *const rnd` | 92.8000 | 2252 | 478 |
| second alias `AERandom *rnd2 = Globals::rnd;` used at R | 92.8000 | 2252 | 478 |
| `rnd = Globals::rnd;` at loop top | 91.5000 | 2252 | 133 |
| `rnd = Globals::rnd;` at loop bottom | 91.5000 | 2252 | 133 |
| `rnd = Globals::rnd;` just before the retry loop | 80.1000 | 2240 | 6 |
| `rnd` declared before `spotUsed` | 76.6000 | 2228 | 6 |
| `rnd` declared before `shipCount` | 70.8000 | 2236 | 6 |
| `rnd` declared at the top of `createScene` | 60.6000 | 2204 | 5 |
| all-global + `(void) rnd;` after the loop | 75.4000 | 2232 | 6 |
| all-global + `if (rnd == nullptr) return;` after the loop | 69.6000 | 2240 | 4 |
| all-global + `if (rnd->nextInt(1) < 0) return;` after the loop | 67.4000 | 2248 | 4 |
| all-global + `(void) Globals::status;` after the loop | 75.4000 | 2232 | 6 |
| all-global + `if (capacity == -7) return;` after the loop | 75.4000 | 2232 | 6 |

Neutral (all **92.8000 % / 2252 B / fd 478**): `int race = (int) sceneRace;`,
`getRandomEnemyFighter((int) race)`, `nextInt((int) capacity)`,
`ArrayAdd<KIPlayer *>(ship, *this->enemies)`, `setExhaustVisible(0)`,
`setAlwaysFriend(1)`, `spotUsed[spot] = (unsigned char) 1`,
`*(kHangarCapacity + sceneRace)`, `(sceneRace == 7 ? … : …)[0]`,
`unsigned tries`, `while` retry loop, `unsigned spot`,
`i < (int) shipCount`, `const int capacity`, `int i = 0;` hoisted before the
loop, `int type;` declared before `int race`, `unsigned char *const
spotUsed`, `KIPlayer *const ship`, `const int *const spots`,
`AERandom *const rnd`.

### 3.4 Per-file `-mllvm` register-allocator knobs

Temporarily added as `set_source_files_properties(Level.cpp COMPILE_OPTIONS
"-mllvm;<flag>")`, scored, then reverted (`CMakeLists.txt` is unchanged in
the landed tree):

| flag | pct | size | fd |
|---|---|---|---|
| `-enable-deferred-spilling` | 90.9000 | 2252 | 152 |
| `-disable-spill-hoist` | 92.8000 | 2252 | 478 |
| `-disable-spill-fusing` | 92.8000 | 2252 | 478 |
| `-regalloc-csr-first-time-cost=0` | 92.8000 | 2252 | 478 |
| `-regalloc-csr-first-time-cost=100` | 92.8000 | 2252 | 478 |
| `-regalloc-csr-first-time-cost=1000` | 92.8000 | 2252 | 478 |

(`-split-spill-mode=size` 92.6, `-sink-insts-to-avoid-spills` 93.1 and
`-regalloc=fast` 17.7 were already measured in w6820/w6903. The 93.1 flag
is not usable: per-file flags change the codegen of the other byte-exact
`Level.cpp` functions and collapse the global `byte_exact` count.)

## 4. The landed source shape (for reference)

```cpp
bool ownHangar = false;
if (Globals::status->getStation()->getIndex() == 108)
    ownHangar = Globals::status->field_114 == 3;
int capacity = kHangarCapacity[sceneRace];
int shipCount = Globals::rnd->nextInt(capacity + 1);
if (ownHangar) {
    Array<Ship *> *ships = Globals::status->voidStation->getShips();
    shipCount = ships != nullptr ? (capacity <= ships->size() ? capacity : ships->size()) : 0;
}
unsigned char *spotUsed = new unsigned char[capacity];
for (int i = 0; i < capacity; i++) spotUsed[i] = 0;
AbyssEngine::AERandom *rnd = Globals::rnd;                       // basin pin
const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];
for (int i = 0; i < shipCount; i++) {
    int race = sceneRace;
    if (rnd->nextInt(100) <= 29) {                               // A
        race = rnd->nextInt(4);                                  // B
        if (Globals::rnd->nextInt(100) < 30) race = 8;           // C
    }
    int type = Globals::globals->getRandomEnemyFighter(race);
    if (Globals::status->getStation()->getIndex() == 100) {
        int pick = Globals::rnd->nextInt(3);                     // D
        type = pick == 0 ? 37 : pick == 1 ? 38 : 40;
    }
    if (ownHangar) type = (*Globals::status->voidStation->getShips())[i]->getIndex();
    KIPlayer *ship = this->createShip(0, 0, type, nullptr, false, false);
    int spot = Globals::rnd->nextInt(capacity);                  // E
    for (int tries = 100; tries != 0 && spotUsed[spot] != 0; tries--)
        spot = rnd->nextInt(capacity);                           // R (nested loop)
    spotUsed[spot] = 1;
    const int *table;
    switch (sceneRace) { case 0: … case 1: … case 3: … case 8: … default: table = spots; }
    ship->setPosition((float) table[spot * 3],
                      (float) (table[spot * 3 + 1] + kShipHangarHeights[type]),
                      (float) table[spot * 3 + 2]);
    ship->player->setAlwaysFriend(true);
    ship->setToSleep();
    ship->geometry->setRotation(0.0f, (float) Globals::rnd->nextInt(300) / 100.0f, 0.0f);  // F
    ((PlayerFighter *) ship)->setExhaustVisible(false);
    ArrayAdd(ship, *this->enemies);
}
delete[] spotUsed;
return;
```
and in the parts loop:
```cpp
Vector rot;
rot.y = 3.1415927f;
rot.z = 0.0f;
rot.x = 0.0f;
Vector *rp = (Vector *) ((char *) &rot - (size_t) g);   // basin pin (semantically wrong)
g->setRotation(*rp);
```

## 5. Conclusion

The landed wave-119 shape is a genuine local maximum for clang 7.0.2 at
`-Oz`. Both remaining regions are pinned by the same global allocation
basin, and the two levers that pin the basin (`subs` in the parts loop, a
loop-live `rnd` value used inside the nested retry loop) are exactly the
two things that prevent the parts-loop and ship-loop code from matching.
The all-global source is the semantically correct shape and reproduces the
original ship loop, but loses the prologue basin (75.4 %).

The next pass should look for a source feature that pins the prologue
basin **without** adding an eighth loop-live value and **without** a
`subs` in the parts loop — i.e. something that changes the greedy
allocator's priority ordering globally while leaving both regions'
instruction shapes natural. Candidates not yet tried: a witnessed original
local in the mission-2/mission-4 branches that is live across a loop, or a
different `Vector rot` scope that changes the frame-object order.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6939
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (not committed): `/dev/shm/w6939/` — `Level.base.cpp`
(the landed source), `score.py` (official verifier import, unrounded ratio
+ size + first-diff index), `norm.py` / `norm2.py` (per-instruction diff,
`norm2` ignores branch-target labels), `runv.py` + `t1.py` … `t16.py`
(this session's variant batches), `orig.asm` / `ours.asm` (resolved
disassembly).
