# Radar::draw pass (wave119, w6913) — whole-block order map, the recompute anchor, and why the landmark loop rotates

**Status:** research note, 2026-11-05. Worktree `wt/w6913`, branch
`wave119-w6913`, based on current `master` (after `radar-draw-wave119-w6910.md`).
**No source change landed**; this commit contains this note only. The target
remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.426606 %** (official `verify.py` display 74.4). 2596 instructions match,
404 branch + 492 non-branch instructions are mismatched on the original side.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. No section/extab change (note-only commit).

## 1. Exact commands used

```sh
cd /home/debian/GalaxyOnFire2/wt/w6913
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS

NDK=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6913/orig.asm
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6913/ours.asm
```

The scoring harness imports `tools/verify/verify.py` and reproduces its
`disassemble`/`reconcile` exactly (identical 74.426606 % on the baseline).
Per-variant loop: edit `Radar.cpp`, `cmake --build cmake-build-match --target
link_so -j6` (≈3–5 s), score the single symbol, restore the baseline.

## 2. Whole-function block-order map (the core artefact)

Both binaries place the function at `0x12e050`, so function-relative offsets are
directly comparable. Address order of the major blocks:

| block | original | baseline ours | note |
|---|---:|---:|---|
| prologue / route | 12e050–12e2fc | 12e050–12e2fc | **identical, delta 0** |
| otherLock #1 | 12e304–12e312 (24 B) | 12e304–12e338 (40 B) | ours if-converts (see §3) |
| landmark guard | 12e314–12e372 | 12e338–12e376 | ours has no 109/110 skip block |
| otherLock #2 | 12e376–12e388 | 12e378–12e38a | identical code |
| planet section test | 12e38a–12e3d4 | 12e38c–12e3d4 | identical code |
| planet loop | 12e3d4–12e6e4 | 12e3d4–12e6d0 | identical body, delta 0 |
| prologue cold (`!alienOrbit` onPlanetCourse) | 12e6ee–12e746 | 12e6d2–12e72a | identical code |
| **invented recompute** | — | **12e72c–12e77e** | 33 extra instructions |
| landmark loop setup | 12e7a8–12e7d2 | 12e780–12e7ae | |
| literal pool (5 words) | 12e79c–12e7a6 | 12e7b0–12e7ba | same 5 words |
| landmark loop head | **12e7d4** (inline) | **12eb00** (after body) | loop rotated |
| landmark loop body | 12e7e0–12eb40 | 12e7bc–12eb40 | body itself matches |
| landmark tail (`if (!turretMode)`) | 12eb42–12ebbe | 12eb42–12eb84 | |
| planet-lock tail (`if (!turretMode && !landmarkHit)`) | **12e748–12e798** | **12ebd4–12ec34** | moved after the loop |
| enemies | 12ec24 | 12ec36 | |

The prologue is byte-identical through `12e2fc`. The first divergence is the
otherLock block; everything after it inherits a byte delta that changes sign
several times through the function.

## 3. First divergence: otherLock #1 (23 original insns vs 4 ours)

Original (`12e2fc`):

```
mov  r1, r4
ldr.w r0, [r1, #12]!     ; lockedAsteroid, r1 = &lockedAsteroid
str  r1, [sp, #124]
cbnz r0, 12e312          ; a != 0 -> shared set1
ldr  r0, [r4, #56]       ; lockedGasCloud
cbnz r0, 12e312
ldr  r0, [r4, #28]       ; field_0x1c
cmp  r0, #0
beq.w 12ebc2             ; all null -> otherLock = field_0x14 != 0
12e312: movs r5, #1      ; single shared set1, otherLock in r5
```

Ours (baseline):

```
cbz  r0, 12e324          ; a == 0 -> check b
mov.w r9, #1             ; set1 #1, otherLock in r9
ldr  r5, [sp, #156]      ; &Globals::status hoisted into set1
b.n  12e338
12e324: ... itt eq; ldreq; cmpeq ...   ; b/c if-converted
12e334: mov.w r9, #1     ; set1 #2
```

The original's shared `set1` is only `movs r5,#1`; ours' first `set1` also
contains the `ldr r5,[sp,#156]` status load, so it cannot tail-merge with the
b/c set1 and PHI elimination leaves two materialisations. This confirms
w6910 §3 (PHI elimination + the null-pointer 62.5 % heuristic) and adds the
precise reason the merge fails: **the hoisted status load in the first set1**.

## 4. New finding: deleting the recompute *fixes* otherLock (r5) and the planet-lock tail, but moves the landmark loop out of line

The 33-instruction block at `12e72c` is the invented second `onPlanetCourse`
recompute (w6817: the original has one `goingToPlanet` / `isDockingToPlanet`
call; we have two). Deleting it (`norec`) gives:

* otherLock #1 becomes the original's straight chain **with `movs r5,#1`**
  (`12e306`) — the register the original uses, not `r9`;
* the planet-lock tail moves inline to `12e764–12e7ae` (original `12e748`),
  and the enemies section follows at `12e7b0` (original `12ec24`);
* **but the whole landmark loop moves out of line to `12f45a`**, after the
  enemies section. The guard's `bne.w`/`ble.w` now target `12f45a`.

Measured: `9824 B / 63.746398 %` (reproduces w6817's number). The landmark
loop is one contiguous ~0x390-byte block; moving it costs more than the
otherLock + planet-tail gains.

The mechanism is a placement cycle: in the original the planet-lock tail's exit
is `b.n 12ec24` (a branch over the landmark loop), so the loop sits *between*
the planet-lock tail and the enemies. In `norec` the planet-lock tail's last
block (`12e7a4`: `ldr r1,[r4,#20]; movs r0,#0; str r0,[r4,#416]; cbnz r1;
str r0,[r4,#24]`) falls straight through into the enemies section, so
MachineBlockPlacement puts the enemies chain next and the landmark loop last.

`norec + Status *status` (the w6910 straight-chain trick) does **not** compose:
`9808 B / 63.783160 %`, loop still at `12f44a`.

## 5. The landmark-loop rotation is a pool-placement consequence

In the original the literal pool at `12e79c` is placed after the planet-lock
tail's `b.n 12ec24` water, so the loop setup (`12e7a8`) and head (`12e7d4`)
follow the pool and the body starts at `12e7e0`.

In the baseline the planet-lock tail is elsewhere, so the only water near the
loop is the setup block's own `b.n 12eb00` at `12e7ae`; the pool goes at
`12e7b0` and the body starts at `12e7bc`. The head is placed after the body at
`12eb00` (the latch at `12eafc` falls into it).

Consequence: the loop body matches instruction-for-instruction but with a
constant **+0x24 byte delta** for the first ~120 instructions (the whole
`onScreen` bounds test and hit test), then the delta grows to +0x28/+0x2c as
the stack-slot differences accumulate. Fixing the loop body offset requires
fixing the pool placement, which requires the planet-lock tail to be inline
before the loop, which requires the recompute to be gone — i.e. the `norec`
experiment — which then moves the loop out of line.

## 6. Guard: the missing 109/110 skip block

Original `12e326`:

```
cmp  r0, #109
beq.n 12e338            ; -> skip block
... 110 check ...
bne.n 12e354            ; -> 111 check
12e338: movs r1, #0     ; skip block (109 branch + 110 fall-through)
mov  r5, r6             ; r5 = &Globals::status (r6 during the guard)
b.n  12e376
12e354: ... 111 && campaign > 93 ...
12e372: movs r1, #0     ; join
ldr  r5, [sp, #156]
12e376: ...
```

Ours branches the 109 and 110 cases straight to the join (`beq.n 12e376`) and
has no skip block, because `r5` is already `&Globals::status` (loaded at
`12e30a`) instead of holding `otherLock`. The skip block is the water at
`12e33c` that w6910 identified as the reason the original's pool lands at
`12e340`. Reproducing it needs `otherLock` in `r5` *and* `&Globals::status`
loaded only at the guard — the two conditions the baseline cannot satisfy
simultaneously (the `Status *status` local gives the straight chain but loads
status before otherLock, so the 109/110 branches still merge into the join).

## 7. Variant sweep (all on the pristine baseline)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.426606** |
| `Status *status` before otherLock (w6910 form) | 9900 | 72.536928 |
| `Status *status` after otherLock | 9912 | 72.484953 |
| `Status *status` used in planet section only | 9836 | 69.285919 |
| recompute deleted (`norec`) | 9824 | 63.746398 |
| `norec` + `Status *status` | 9808 | 63.783160 |
| `norec` + `__builtin_expect(landmarks != nullptr, 1)` | 9740 | 64.643838 |
| `norec` + planet-lock tail `expect(...,1)` | 9820 | 62.332420 |
| `norec` + planet-lock tail `expect(...,0)` | 9848 | 52.285139 |
| faithful inSight (distance first, float crossX/Y) | 9864 | 66.685813 |
| faithful inSight + `Status *status` | 9868 | 66.657089 |
| inSight float compare (no int cast) | 9920 | 67.297762 |
| planet-lock tail `expect(...,1)` | 9904 | 71.676466 |
| planet-lock tail `expect(...,0)` | 9924 | 61.650694 |
| `bool otherLock` (if/else) | 9868 | 67.375377 |
| `uint8_t otherLock` | 9908 | 74.426606 (identical) |
| otherLock plain OR4 / double-bang | 9916 | 70.110489 |
| otherLock init-0 if/else | 9908 | 73.193807 |
| otherLock bitwise `|` of four tests | 10464 | 46.591223 |
| otherLock `expect` all three null 1 | 9908 | 74.426606 (identical) |
| otherLock outer expect 1 | 9900 | 71.014076 |
| otherLock expect a only | 9912 | 72.380679 |
| guard `!=` conjunctive form | 9908 | 74.426606 (identical) |
| guard `!=` form with `||` in the 111 arm | 9912 | 64.660361 |
| guard `skip` bool local | 9916 | 70.286533 |
| guard 110 before 109 | 9908 | 74.426606 (identical) |
| guard extra parens | 9908 | 74.426606 (identical) |
| guard `skip` if/else + `Status *status` | 9904 | 70.441514 |
| loop `__builtin_expect(i < size, 1)` | 10008 | 37.558685 |
| loop `__builtin_expect(i < size, 0)` | 9908 | 58.456251 |
| loop `(int) size` cast | 9908 | 74.426606 (identical) |
| loop size cached in local | 9908 | 72.500716 |
| landmark loop `continue` → positive `if` | 9908 | 74.426606 (identical) |
| `i <= 1` → `i < 2` | 9908 | 74.426606 (identical) |
| `i != 3` → `i < 3 || i > 3` | 9908 | 74.426606 (identical) |
| `offset` ternary → if/else | 9908 | 74.426606 (identical) |
| `landmarkHit = true` before field_0x28 store | 9908 | 74.426606 (identical) |
| `topOk` inlined | 9908 | 74.283257 |
| `size`/`left`/`top` reorder | 9908 | 74.369266 |
| planet tail `else if` merged into `else` | 9908 | 74.426606 (identical) |
| enemy condition locals (both calls first) | 9888 | 68.044196 |
| enemy condition `skip` bool local | 9908 | 74.426606 (identical) |
| enemy condition De Morgan | 9908 | 74.426606 (identical) |
| drop recompute `field_0x1a8` store | 9876 | 65.766413 |
| drop prologue `field_0x1a8` store | 9860 | 70.420917 |
| `landmarkHit` declared before `turretMode` | 9908 | 74.426606 (identical) |
| `turretMode` moved after otherLock | 9904 | 71.752223 |
| `notOtherLock`/`notTurretMode` as `bool` | 9920 | 71.311828 |
| swap `notOtherLock`/`notTurretMode` declarations | 9908 | 74.426606 (identical) |
| `notOtherLock = otherLock == 0` | 9908 | 74.426606 (identical) |
| `notTurretMode = turretMode == 0` | 9908 | 74.426606 (identical) |
| planet+landmark section swap (semantics changed) | 9832 | 55.632383 |

None beats the baseline.

## 8. Residual accounting (baseline, unchanged from w6910)

| masking | ratio |
|---|---:|
| none | 74.426606 |
| `[sp, #N]` → `[sp, #S]` | 76.089450 |
| `[sp, #N]` + `loc_XXXX` → wildcard | 84.805046 |

So ~1.7 points are pure stack-slot colouring and ~8.7 more are branch-target
offsets. Branch mismatches: 404 of 830 original branches.

The stack-slot rotation is tied to the frame: the original pushes
`{d8-d13}` (the inSight block caches `crosshairPos.x/y` in `s26`/`s24` across
the `operator-`/`VectorLength` calls), the baseline pushes `{d8-d11}`. The
faithful inSight shape restores `{d8-d13}` and moves `mode` from `[sp,#112]`
to `[sp,#104]` (original `[sp,#100]`), but re-lays-out the function and scores
66.7 %.

## 9. Conclusion

The function is a very strong `difflib` local optimum. The residual is not a
constant source-shape error but a **block-placement cycle**:

1. The invented recompute occupies the slot where the original has the
   planet-lock tail.
2. That pushes the planet-lock tail after the landmark loop.
3. That moves the literal pool in front of the loop body and rotates the loop
   head to `12eb00`.
4. The otherLock #1 if-conversion (status load hoisted into set1) adds 16 B
   at the front and removes the `12e33c` water that would put the pool at
   `12e340`.

Deleting the recompute breaks the cycle in the right direction for otherLock
(`r5`, straight chain) and the planet-lock tail, but lets the landmark loop
fall out of line. A future pass that beats 74.426606 % needs either

(a) a source shape that keeps the landmark loop inline *and* deletes the
    recompute (the ~0x390-byte block placement is the blocker), or
(b) a way to get `otherLock` into `r5` with `&Globals::status` loaded only at
    the guard (restores the `12e33c` skip block and the `12e340` pool).

The baseline remains the best measured shape at **74.426606 % / 9908 B**.
No source change is committed; the gate is unchanged (`RATCHET PASS`,
byte 4505, linked 4505, data 843/843, imports 207, lint CLEAN, drift 0).
