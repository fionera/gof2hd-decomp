# `Level::createScene` — wave-119 w6934: the `-sink-insts-to-avoid-spills` ctor basin

**Status:** no landable source win over **92.7573 %** (2252 B). Worktree
`wt/w6934`, branch `wave119-w6934` (base master `59184fd11`). Target
`_ZN5Level11createSceneEv`, `src/game/world/Level.cpp`. This note records a
new compiler-flag basin (93.2656 %) that cannot be landed as-is, plus the
source measurements taken while looking for a flag-free equivalent.

Continuation of `level-createscene-wave119-w6903.md` (the most complete
dead-end sweep), `…-w6820.md` (measurement tables, MIR), `…-w6709.md`
(landed `-g` trick + y/z/x store order + ship-loop `rnd` mask) and
`Level-createScene-flag-free.md`.

## TL;DR

* Baseline and landed best: **92.7573 % / 2252 B**, 791/783 instructions,
  first structural diff at normalized instruction 478 (parts-loop store
  schedule). Gate green: byte 4505, linked 4505, data 843/843, imports
  207, lint CLEAN, drift 0, RATCHET PASS.
* **New:** the plain `Vector rot(0.0f, 3.1415927f, 0.0f)` constructor form
  plus a per-file `-mllvm -sink-insts-to-avoid-spills` scores
  **93.2656 % / 2252 B**, first diff **548** — the whole parts loop and the
  hangar/capacity block become instruction-exact. The flag is TU-wide and
  changes 12 other `Level.cpp` functions (+100 B `.text`), so `check` fails
  (`byte_exact 4505 -> 2357`, `linked_exact 4505 -> 4491`). Not landable.
* The ctor form **without** the flag gives the right parts-loop *store
  schedule* but the wrong register allocation (84.9524 %). Adding the
  landed `-g` trick to it gives the right RA and a parts loop that is
  instruction-exact except for one extra 2-byte `subs` (88.3100 %, +2 byte
  shift of every later branch target).
* The landed y/z/x field form + `-g` trick remains the unique maximum.
  ~45 new variants measured; none beat 92.7573 %.

## 1. The flag basin (new)

Add to `CMakeLists.txt` (next to the existing `SpaceLounge.cpp`
`COMPILE_OPTIONS` block):

```cmake
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/world/Level.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-sink-insts-to-avoid-spills")
```

and use the plain constructor form for the hangar-part rotation:

```cpp
AEGeometry *g = new AEGeometry(mesh, Globals::Canvas, false);
Vector rot(0.0f, 3.1415927f, 0.0f);
g->setRotation(rot);
```

Measured with the official single-function verifier:
`score 93.2656 %  size 2252B (orig 2252)  firstdiff 548  insns 791/783`.
The parts loop is then byte-identical to the original:

```
ldr   r0, [pc, #808]      ; pi
add   r1, sp, #56         ; &rot
str   r0, [sp, #60]       ; rot.y = pi
mov.w sl, #0
mov   r0, r5
str.w sl, [sp, #56]       ; rot.x = 0
str.w sl, [sp, #64]       ; rot.z = 0
blx   setRotation
```

and the diff moves to instruction 548 (ship-loop `rnd` register allocation,
unchanged from the landed build).

### 1.1 Why it cannot be landed

The flag is per-file, so all of `Level.cpp` is recompiled. Twelve other
functions change size (+100 B total), `.text` grows `0x1337c8 -> 0x133848`
and the `.rodata` base shifts `0x1c9e10 -> 0x1c9e90`:

| function | size delta |
|---|---|
| `Level::getBoundingVolume(int, AEGeometry*)` | +16 |
| `Level::createWingmen()` | -8 |
| `Level::createGasClouds()` | -12 |
| `Level::updateOrbit(int)` | +4 |
| `Level::updateMissionOrbit(int)` | -4 |
| `Level::createAsteroids()` | +20 |
| `Level::initParticleSystems()` | +8 |
| `Level::assignGuns()` | +20 |
| `Level::createSpace()` | +32 |
| `Level::createStaticObject(Waypoint*, int, bool)` | +8 |
| `Level::createMission()` | +12 |
| `Level::createCampaignMission()` | +4 |

Eleven of those twelve are currently byte-exact (`createMission` is not).
`check` with the flag reports **byte_exact 4505 -> 2357** (the `.rodata`
shift changes literal-pool words in ~2100 functions) and **linked_exact
4505 -> 4491**. Even if `.text` were compensated back to the original size
so `.rodata` did not move, the eleven exact functions would still differ:
`4505 - 11 = 4494 < 4503`. Re-tuning them is not a small job — e.g.
`createWingmen` under the flag has **211 differing instructions of 236**
(a completely different allocation), so the flag basin is not reachable by
"fix the other eleven" within a wave.

`-mllvm -machine-sink-bfi=true` leaves the result unchanged (93.2656 %);
`-mllvm -machine-sink-split=false` destroys it (42.7665 %).

### 1.2 MIR evidence

Pre-RA (Eliminate PHIs) MIR is identical between the flag and no-flag ctor
builds apart from debug-location `!N` numbering. The difference is made
during/after register allocation: with the flag the `%545 = t2ADDri
%stack.1, 0` materialisation of `&rot` is sunk into the loop body and the
`t2MOVi 192/304/-1` constants are re-ordered, which flips greedy RA to the
original's allocation (parts-loop zero in `sl`, `hangarPart` in `r4`,
`k` in `r6`). Without the flag the allocator instead puts the zero in
`r4`, `hangarPart` in `r6` and spills `k` to `[sp,#44]`.

## 2. The ctor form without the flag

`Vector rot(0.0f, 3.1415927f, 0.0f); g->setRotation(rot);` alone:

```
score 84.9524 %  size 2248B  firstdiff 28  insns 792/783
```

Parts loop (20 B, original is 24 B):

```
ldr   r0, [pc, #804]
add   r1, sp, #56
str   r0, [sp, #60]       ; y
movs  r4, #0              ; zero in r4, not sl
mov   r0, r5
str   r4, [sp, #56]       ; x
str   r6, [sp, #44]       ; k spilled
str   r4, [sp, #64]       ; z
blx   setRotation
```

The **store schedule is right** (y, x, z, three separate stores) but the
zero lands in `r4` and `k` is spilled. Adding the landed `-g` trick:

```cpp
Vector rot(0.0f, 3.1415927f, 0.0f);
Vector *rp = (Vector *) ((char *) &rot - (size_t) g);
g->setRotation(*rp);
```

gives **88.3100 % / 2252 B / firstdiff 28**. That build is
instruction-identical to the original for the whole function *except* the
parts loop, where it emits

```
ldr   r0, [pc, #808]
mov.w sl, #0
str   r0, [sp, #60]       ; y
add   r0, sp, #56
subs  r1, r0, r5          ; <-- extra 2-byte instruction
mov   r0, r5
str.w sl, [sp, #56]       ; x
str.w sl, [sp, #64]       ; z
blx   setRotation
```

i.e. 26 B instead of 24 B. The +2 bytes shift every later branch target
(`bne.w loc_840` becomes `loc_842`), which is what costs the ~4.4 points
versus the landed form. **If a source shape could fold `(char *)&rot -
(size_t)g` back to `&rot` while keeping the RA effect, this would be the
93.2656 % basin without any flag.**

Fold-back attempts that all reverted to the plain ctor basin (84.9524 %):

```cpp
(char *) &rot - (size_t) g + (size_t) g
(char *) &rot - ((size_t) g - (size_t) g)
(char *) &rot - (size_t) g * 0
(char *) &rot - (size_t) (g - g)
Vector *rp = ...; g->setRotation(rot);          // rp dead
Vector *rp = ...; rp = (char *) rp + (size_t) g; g->setRotation(*rp);
```

i.e. the RA effect needs the subtraction to survive to the backend.

## 3. Measurements (all 2252 B unless noted)

Scored with the official single-function verifier (unrounded difflib
ratio). `fd` is the first differing normalized instruction index.

### 3.1 Landed trick, field-assignment permutations

| variant | pct | fd |
|---|---|---|
| **y, z, x (landed)** | **92.7573** | **478** |
| y, x, z | 92.6302 | 477 |
| x, z, y | 92.6302 | 477 |
| z, x, y | 92.6302 | 477 |
| x, y, z | 88.3100 | 28 |
| z, y, x | 88.1830 | 28 |
| same six without the trick | 84.88–84.95 | 28 |

### 3.2 32-mask `rnd` sweep (bit0=B `nextInt(100)<=29`, bit1=C `race=nextInt(4)`,
bit2=D `nextInt(100)<30`, bit3=E `pick=nextInt(3)`, bit4=G retry)

* landed field y/z/x form: best **92.7573** at masks **19** and **21**
  (B, C, G local); everything else ≤ 91.87.
* ctor + trick form: best **91.2325** at mask 23 (fd 462).
* ctor + flag form: best **93.2656** at masks 19 and 21.

### 3.3 Trick operand / address spellings (landed field form)

| trick operand | pct | fd |
|---|---|---|
| `g` (landed) | **92.7573** | 478 |
| `mesh` | 91.4975 | 456 |
| `Globals::Canvas` | 87.0006 | 28 (+8 B) |
| `this` | 86.8020 | 28 |
| `sceneRace` | 86.5482 | 28 |
| `k` | 86.4590 | 28 |

### 3.4 ctor-form variants (no flag)

| variant | pct | size | fd |
|---|---|---|---|
| `Vector rot(0.0f, 3.1415927f, 0.0f)` | 84.9524 | 2248 | 28 |
| `Vector rot(0, 3.1415927f, 0)` | 84.9524 | 2248 | 28 |
| `Vector rot = Vector(...)` | 84.9524 | 2248 | 28 |
| `float zero = 0.0f; Vector rot(zero, pi, zero)` | 84.9524 | 2248 | 28 |
| ctor declared before `new AEGeometry` | 85.4061 | 2256 | 28 |
| `if (g != nullptr) g->setRotation(rot)` | 84.9524 | 2248 | 28 |
| `float rot[3]; rot[1]=pi; rot[0]=0; rot[2]=0; setRotation(*(Vector*)rot)` | 84.8793 | 2248 | 28 |
| float-array + `-g` trick | 92.6302 | 2252 | 477 |

### 3.5 Retry loop

Ghidra shows the retry as `tries = -100; while (tries != 0 && spotUsed[spot])
{ spot = nextInt(capacity); tries++; }`, and the original emits
`mvn.w r5,#99` + `adds r5,#1`. The landed source spells it
`for (int tries = 100; tries != 0 && ...; tries--)` and clang canonicalises
it to the same `mvn.w`/`adds` pair, so the source spelling is irrelevant.
Spelling it literally as `tries = -100; tries++` scores **92.0635 %**
(fd 28) — it perturbs the allocation without changing the instruction
sequence.

### 3.6 Other probes (all ≤ 92.7573)

`AERandom **rndp = &Globals::rnd` with `(*rndp)->` at B/C/G, B/G or G only
= 75.4286 (all-global basin). `Status *status = Globals::status;` at the
function top, all uses rewritten: all-global rnd 66.3246 (2196 B), landed
rnd 70.8440 (2208 B). ctor + flag + all-global `rnd` = 75.9365 (2232 B).

## 4. Ghidra observations (v2 binary, `rea decompile`)

* Hangar-part rotation call is `AEGeometry::setRotation(const Vector&)`
  with a stack `Vector`; Ghidra prints it as `setRotation((Vector *)pAVar23)`.
* `plainHangar` is `(int)sceneRace < 4 && sceneRace != 1` (the complement
  of the landed `sceneRace == 1 || sceneRace > 3`; same code).
* Outer parts loop counter is unsigned (`bcc`), inner child loop is signed
  (`blt`) — both already match.
* `capacity` is read as `uint` from `kHangarCapacity[sceneRace]`;
  `spotUsed` is `unsigned char *`.
* Ship loop: `race` r5, `type` fp, `capacity` r6, `spotUsed` r4, `i` r9,
  `&Globals::rnd` **r8** (value reloaded with `ldr.w r0,[r8]` at every
  site). The landed build is a cyclic permutation of that assignment
  (`capacity` fp, `race` r6, `type` r8, `&Globals::rnd` spilled) — the
  central basin conflict documented in `…-w6903.md` §3 is unchanged.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6934
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (not committed): `/dev/shm/w6934/` — `Level.base.cpp`
(landed source), `score.py` (official verifier import, unrounded ratio +
size + first-diff), `mk.py`/`build.sh` (single variant build+score),
`sweep3.py`/`sweep4.py` (32-mask `rnd` sweeps), `flagcombo.sh` (per-file
flag trials), `orig.asm`, `show*.txt`, `mir_cs_*.txt`.
