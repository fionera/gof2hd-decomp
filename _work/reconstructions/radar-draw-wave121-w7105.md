# Radar::draw pass (wave121, w7105) — literal pool re-derived; block placement, not the pool, is the dominant residual

**Status:** research note, 2026-11-06. Worktree `wt/w7105`, branch
`wave121-w7105`, based on `master` `bd061a489`. **No source change landed**;
this commit contains this note only. The target remains the pristine
baseline (74.541284 %).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.541284 %** (official `verify.py` display 74.5), `M = 2600` matching
instructions.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. Whole-file size unchanged (note-only commit).

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7105
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS

NDK=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7105/orig.asm
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7105/ours.asm
```

Per-variant loop (≈1.4 s each): edit `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then score the single symbol with a
scratch `difflib` harness that mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` (identical 74.541284 % on the baseline). Harness:
`/dev/shm/w7105/score.py`; variant drivers `/dev/shm/w7105/runv*.py`.

## 2. The literal pool is already byte-identical in content and first-use order

The first constant island is the *same five words in the same order* in both
binaries:

| word | symbol | first use |
|---|---|---|
| `0x000e1c9a` | `__stack_chk_guard` | `12e064` |
| `0x000e2166` | `Globals::Canvas` | `12e084` |
| `0x000e20bc` | `Globals::status` | `12e09a` |
| `0x000e2224` | `PaintCanvas::DrawImage2D` | `12e1da` |
| `0x000e1fda` | `Globals::font` | `12e2b4` |

All eight pools have the same contents in the same order (45 pool words each);
only the **address of the first pool** differs:

* original: `12e340` (after the 109/110 guard skip block + `nop`)
* ours: `12e2e0` (right after the waypoint `b.n`)

So "re-derive the literal pool" is already done: there is no missing source
constant/table. The pool is misplaced, not wrong.

### 2.1 Why the pool moves: the `Status *status` load is hoisted over the otherLock chain

Original prologue tail:

```
12e2de: b.n   12e2fc            ; waypoint onScreen block skips scannerAvailable
12e2e0: ldrb  r0,[r4,#427]      ; scannerAvailable  <- branch target of cbz at 12e29a
12e2e4: cbz   r0,12e2fc
...
12e2fc: mov   r1,r4             ; otherLock chain
12e2fe: ldr.w r0,[r1,#12]!      ; lockedAsteroid
12e302: str   r1,[sp,#124]      ; &lockedAsteroid
12e304: cbnz  r0,12e312
12e306: ldr   r0,[r4,#56]       ; lockedGasCloud
12e308: cbnz  r0,12e312
12e30a: ldr   r0,[r4,#28]       ; field_0x1c
12e30c: cmp   r0,#0
12e30e: beq.w 12ebc2           ; then: otherLock = field_0x14 != 0
12e312: movs  r5,#1             ; else: otherLock = 1
12e314: ldr.w r0,[r4,#312]      ; landmarks
12e318: cbz   r0,12e372         ; skip landmark block
12e31a: ldr   r6,[sp,#156]      ; &Globals::status  <- AFTER chain + landmarks check
12e31c: ldr   r0,[r6,#0]        ; Globals::status
...
12e338: movs  r1,#0
12e33a: mov   r5,r6
12e33c: b.n   12e376
12e33e: nop
12e340: <first literal pool>
```

Ours (baseline) loads `status` at `12e310`, **before** the chain:

```
12e2de: b.n   12e310            ; waypoint block skips scannerAvailable AND the status load
12e2e0: <first literal pool>    ; 12e2e0 is not a branch target -> legal island slot
12e2f4: ldrb  r0,[r4,#427]      ; scannerAvailable (moved after the pool)
12e2f8: cbz   r0,12e310
...
12e310: ldr   r5,[sp,#156]      ; Status *status = Globals::status
12e312: mov   r1,r4             ; otherLock chain
...
12e31a: ldr   r6,[r5,#0]        ; status value in r6
```

The `Status *status` local is the w6957-tuned shape. Because its load is a
statement before the chain, the waypoint `b.n` targets `12e310`; that makes
`12e2e0` a cold gap and the island pass puts the pool there. In the original
the status load is sunk *after* the landmarks check, so `12e2e0` is the
scannerAvailable branch target and the first cold gap is `12e340`.

### 2.2 The pool position alone is only worth ~0.14 %

Moving the five-word island from `ours[222:227]` to the original slot and
leaving everything else untouched:

| simulated island position | ratio |
|---|---:|
| baseline (no move) | 74.541284 |
| move to `ours[257]` | **74.6846** |
| move to `ours[258]` | 74.6560 |
| move to `ours[259]` | 74.6273 |
| move to `ours[262]` | 74.5986 |

So the island is a *symptom* of the status load, not the dominant residual.
Fixing it in isolation cannot buy more than 0.15 points.

## 3. The dominant residual is block placement (planet-lock tail vs landmark loop)

The original code layout is:

```
12e376  planet block (outer condition, stations, planet loop 12e3d4..12e748)
12e748  planet-lock tail (if (!turretMode && !landmarkHit) { ... })
12e79c  second literal pool
12e7a8  landmark-loop setup + for-loop 12e7d4..12eb42
12eb42  landmark station-lock tail, then b.w 12e376 (back to planet block)
```

Ours is:

```
12e3d4  planet loop 12e3e4..12e748
12e7de  landmark-loop setup + for-loop ..12eb42
12eb46  planet-lock tail
```

i.e. the original treats the landmark loop as a **cold out-of-line block** and
keeps the planet block + planet-lock tail inline; ours treats the landmark loop
as hot and pushes the planet-lock tail after it. This is the
`DELETE orig[637:654] (17)` + `DELETE orig[673:692] (19)` pair in the diff —
36 normalized instructions that are simply in the wrong place.

Evidence from the call stream (original `Radar::update` call addresses):

| call | original | ours |
|---|---|---|
| waypoint update | `12e23c` | `12e23c` |
| planet-loop update | `12e3ee` | `12e3e4` |
| landmark-loop update | `12e80a` | `12e7de` |
| enemy-loop update | `12ed62` | `12ed72` |

and the first `PlayerEgo::isDockingToAsteroid` (planet-lock tail):

* original `12e774` (before the landmark loop)
* ours `12eb4e` (after the landmark loop)

### 3.1 `__builtin_expect` is not the lever

Marking the landmark guard cold aligns the block order but changes the whole
register allocation:

| variant | size | frame | ratio | first `isDockingToAsteroid` |
|---|---:|---:|---:|---:|
| baseline | 9908 | `sub sp,#296`, vpush d8-d11 | **74.541284** | `12eb4e` |
| `__builtin_expect(landmarks != nullptr, 0)` | 9936 | `sub sp,#304`, vpush d8-d11 | 56.742457 | `12e7c6` |
| same + `lockLabel` for the i==0 arm | 9928 | `sub sp,#304` | 56.779176 | `12e7c6` |
| `__builtin_expect(landmarks != nullptr, 1)` | 9812 | — | 67.328156 | `12ea86` |
| `__builtin_expect(campaign >= 2 && ..., 1)` | 9868 | — | 60.505892 | `12e76e` |
| whole guard `__builtin_expect(..., 0)` | 9904 | — | 72.169676 | `12eb68` |
| remove all `__builtin_expect` | 9892 | — | 70.746739 | `12eb7c` |
| remove landmark onScreen expects | 9904 | — | 71.414243 | `12eb74` |

`__builtin_expect(campaign >= 2 && ..., 1)` puts the tail at `12e76e`
(original `12e774`) and produces **no insert/delete blocks** — the block order
is fully aligned — but the register allocation is re-rolled and the score
falls to 60.5. So the block order can be reproduced, but not together with the
baseline's register allocation.

`__builtin_expect(landmarks != nullptr, 0)` likewise has no big
insert/delete blocks (block order aligned) and puts the tail at `12e7c6`, but
grows the frame to 304 B and keeps `&Globals::status` in `r6` for the whole
function, scoring 56.7.

## 4. Distance-first `inSight` reproduces the exact prologue but re-rolls the body

The original's enemy `inSight` block (`12ef4c`) is distance-first:

```
12ef4c: ldr  r0,[pc,#688]     ; &PlayerEgo::crosshairPos
12ef54: add  r5,sp,#232
12ef5a: vldr s26,[r0]         ; crosshair.x -> callee-saved s26
12ef5e: vldr s24,[r0,#4]      ; crosshair.y -> callee-saved s24
12ef62: mov  r0,r5
12ef64: ldrd r2,r1,[sp,#108]  ; &targetPos / &egoPos
12ef68: blx  AEMath::mi
12ef6c: mov  r0,r5
12ef6e: vcvt.s32.f32 s24,s24
12ef72: vcvt.s32.f32 s26,s26
12ef76: blx  AEMath::VectorLength
12ef7a: vmov s0,r0
12ef80: vcmpe.f32 s0,s20      ; distance < 60000.0f FIRST
12ef88: bpl.n 12efc0
12ef8a: vmov r1,s26           ; then x bounds
...
```

The two crosshair floats stay live across `mi`, which is the *only* reason the
original has `vpush {d8-d13}` (s24/s26 appear nowhere else in the function).
The baseline's bounds-first shape computes the ints before the call and uses
only `d8-d11`.

Distance-first source shape:

```cpp
int cx = (int) PlayerEgo::crosshairPos.x;
int cy = (int) PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
    this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
    inSight = true;
}
```

gives `vpush {d8-d13}`, `sub sp,#296`, `egoPos`/`targetPos` at `sp+108`/`sp+112`
(exactly the original) but scores **68.053364 % / 9896 B**, because the frame
slot assignment changes (`mode` `sp+104` vs original `sp+100`) and the body
re-rolls. Float locals (`float crossX/crossY` + `(int)crossX`) give the same
68.0; `float dist` gives 64.57; inline `(int)PlayerEgo::crosshairPos.x` gives
64.26. Combining distance-first with the direct-`Globals::status` guard gives
68.51 (9904 B), still far below baseline.

## 5. The `Status *status` local: the pool cause and the chain-shape coupling

The faithful guard is direct `Globals::status` (the original keeps
`&Globals::status` in `[sp,#156]` and reloads it at `12e31a`, after the
landmarks check). Removing the local moves the pool to `12e310` but flips the
otherLock chain from `cbnz` to `cbz` and duplicates the status load and
`mov otherLock,#1`:

```
12e304: cbz   r0,12e324       ; ours direct-Globals
12e306: mov.w r9,#1
12e30a: ldr   r5,[sp,#156]
12e30c: b.n   12e338
12e310: <pool>
12e324: ldr   r0,[r4,#56]
12e326: ldr   r5,[sp,#156]
...
```

Score 72.266017 % / 9912 B. Moving the `Status *status` declaration after the
chain instead:

| variant | size | ratio |
|---|---:|---:|
| baseline (`Status *status` before chain) | 9908 | **74.541284** |
| `Status *status;` before, `status = Globals::status;` after | 9920 | 73.284630 |
| `Status *status` declared after `notTurretMode` | 9920 | 73.284630 |
| `Status *status` declared just before the guard | 9920 | 73.284630 |
| `Status *&status = Globals::status` | 9912 | 72.266017 |
| direct `Globals::status`, no local | 9912 | 72.266017 |
| direct + `(status = Globals::status)` in the guard | 9920 | 73.284630 |
| `Status *const status` | 9908 | 74.541284 (tie) |
| `auto *status` | 9908 | 74.541284 (tie) |

The chain-shape flip is the blocker: the original needs the status load *after*
the chain **and** the `cbnz`/single-merge chain. No source spelling found this
wave gives both.

## 6. Variant sweep (all on the pristine baseline; no win)

`size` is emitted function size; `ratio` is the unrounded `difflib` score.

| variant | size | ratio |
|---|---:|---:|
| **baseline** | 9908 | **74.541284** |
| `Status *const status` / `auto *status` | 9908 | 74.541284 |
| `campaign` → `uint32_t` | 9908 | 74.541284 |
| `onPlanetCourse` → `int` | 9908 | 74.541284 |
| `field_0x14 != nullptr` | 9908 | 74.541284 |
| `int otherLock = 1; if (cond) ...` | 9908 | 74.541284 |
| declaration-order permutations with `Status` first | 9908 | 74.541284 |
| `whole_expect0_noinner` (drop inner expect) | 9908 | 74.512615 |
| `alienOrbit` → `int` | 9908 | 74.530601 |
| `alwaysEnemySeen` → `bool` | 9908 | 72.438011 |
| `turretMode` → `int`/`uint32_t` | 9888 | 66.676234 |
| `missionBlocksPlanets` → `int` | 9900 | 71.684588 |
| `landmarkHit` → `int` | 9896 | 70.891884 |
| `enemyHit` → `int` | 9924 | 70.610140 |
| `bossSeen` → `int` | 9904 | 72.709021 |
| `otherLock` → `bool` | 9860 | 68.169419 |
| OR-form `otherLock` (all four pointers) | 9892 | 71.541219 |
| `otherLock` OR-form + status after chain | 9892 | 71.721370 |
| landmark i==0 arm → `this->lockLabel` | 9904 | 71.614845 |
| landmark setup moved inside the `if` body | 9900 | 72.536928 |
| distance-first `inSight` (int locals) | 9896 | 68.053364 |
| distance-first + direct `Globals::status` | 9904 | 68.511615 |
| distance-first + `campaign` expect | 9864 | 51.522989 |
| `__builtin_expect(landmarks != nullptr, 0)` | 9936 | 56.742457 |
| `__builtin_expect(campaign >= 2 && ..., 1)` | 9868 | 60.505892 |
| strip all `__builtin_expect` | 9892 | 70.746739 |

No variant beats **74.541284 % / 9908 B**.

## 7. Remaining root cause

The function is a tuned local optimum whose 74.5 % is dominated by two coupled
compiler decisions that the source cannot independently steer:

1. **VFP callee-saved set** (`d8-d11` vs `d8-d13`). Only distance-first
   `inSight` restores `d8-d13`, and it re-rolls the frame slot assignment
   (`mode` `sp+104` vs `sp+100`).
2. **Cold-block placement** (planet-lock tail before the landmark loop in the
   original, after it in ours). `__builtin_expect` can force the order but
   changes the frame size / register allocation and costs 14–18 points.

The first literal pool at `12e2e0` vs `12e340` is a *consequence* of (1)/(2)'s
sibling, the `Status *status` load position; moving the island alone is worth
only 0.14 points. There is no missing source constant or table: the pool
contents and first-use order are already byte-identical.
