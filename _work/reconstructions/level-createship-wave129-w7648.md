# `Level::createShip` — wave 129 (w7648): the guard is the sink trigger; the zero register is the 12-byte delta

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `fee528cbe` (wave-128 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7648`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.** The checked-in body stays the
best at **70.5415 %** (official rounded 70.5), 2764 B, 1007/1006 insns, exact
336 B / 57-row / `ttype 0xff` cleanup-only LSDA. Two new, reproducible findings
below correct wave-128 and explain the remaining 12-byte size delta.

## Result

```
                                   original        HEAD (fee528cbe)
createShip exact ratio              —               70.541480
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

All measurements below use the verifier's own `normalize`/`reconcile`/`score`
path (`/dev/shm/w7648/exact.py`, the unrounded ratio), not a raw difflib pass.

## 1. New finding: the status guard is what sinks the all-int coordinates

Wave 128 concluded that the faithful all-int spawn "still sinks below
`setEmpData`". That is true **only with the `if (Globals::status == nullptr)`
guard present**. The guard puts a branch immediately after the `setEmpData`
call; the pre-RA scheduler then sinks the pure coordinate arithmetic to the end
of that block (after the call, before the branch).

Remove the guard and the same all-int source keeps the coordinate block
**between the `Player` ctor and `setEmpData`**, which is exactly the original's
position:

```
ours, all-int coords declared before `new Player`, no status guard:
  c03b6  blx  Player ctor
  c03ba  ldr  r0,[sp,#40]    ; wz
  c03bc  movw r1,#20000
  c03c0  ldr  r2,[sp,#32]    ; jY
  c03c2  add  r0,r4          ; wz + jZ
  c03c4  subs r6,r0,r1       ; x -> r6
  c03c6  ldr  r0,[sp,#48]    ; wy
  c03c8  add  r0,r2
  c03ca  ldr  r2,[sp,#36]    ; jX
  c03cc  subs r4,r0,r1       ; y -> r4
  c03ce  ldr  r0,[sp,#44]    ; wx
  c03d0  add  r0,r2
  c03d2  mov  r2,r9          ; empTime
  c03d4  sub.w fp,r0,r1      ; z -> fp
  c03d8  mov  r0,r5
  c03da  mov  r1,sl
  c03dc  blx  setEmpData
```

So the placement is *not* a scheduler-strategy problem: `-mllvm
-misched-topdown` and `-mllvm -misched-botup` both leave the guarded body's
coordinates after the call. It is the post-call branch (the guard) that makes
the sink profitable. The original has no such branch, so the faithful source
must not have the guard.

The no-guard all-int body still scores only **61.7223 %** (2776 B, 1002 insns)
because its register assignment and size diverge elsewhere; the guard is
therefore a *compensating* hack in the checked-in body, not the original shape.

## 2. New finding: the 12-byte size delta is the zero register

The faithful no-guard all-int body is **2776 B** (+12 vs the original). The
extra bytes are six `movs r0,#0; str r0,[sp,#0]` pairs in the BoundingAAB
sequence that the original folds into `strd r6,r0,[sp]` with a persistent
`r6 = 0`:

```
orig  c04a2  movs r6,#0
      ...
      c04b8  str  r6,[sp,#0]
      c04dc  strd r2,r1,[sp,#8]
      c04e0  mov  r1,sl
      c04e2  strd r6,r0,[sp]        ; 0.0f and last extent in one store
ours  ...
      c04da  strd r0,r2,[sp,#4]
      c04de  movs r0,#0
      c04e0  str  r0,[sp,#0]        ; rematerialised every call
```

6 × 2 B = 12 B. The original can keep `r6 = 0` because its exception pointer is
`r8`; ours spends `r6` on the exception PHI. A named `float zero = 0.0f;`
(declared at the top of the branch or at function scope) is folded back to the
same rematerialised `movs r0,#0` and changes nothing.

## 3. The exception pointer is `r6` in every body tried (original `r8`)

Confirmed in both the checked-in body and the no-guard all-int body:

```
orig  c0bba  mov r8,r0 ; mov r0,sl   ; fighter
      c0bc0  mov r8,r0 ; mov r0,fp   ; volumes
      c0bc6  mov r8,r0 ; mov r0,r9   ; fixed
      c0bcc  mov r8,r0 ; mov r0,r5   ; player / BoundingAAB
      c0bd4  mov r0,r8
ours  c0bc6  mov r6,r0 ; mov r0,sl
      c0bc0  mov r6,r0 ; mov r0,fp
      c0bca  mov r6,r0 ; mov r0,r9
      c0bd8  mov r6,r0 ; mov r0,r5
      c0be0  mov r0,r6
```

The no-guard body still has the landing pads interleaved at RA time (the
exception PHI is live across the whole function), so `r6` is chosen even though
the final code order puts the pads at the end. This is the wave-126/127 root
cause, unchanged.

## 4. Sweeps run this pass (all regressions unless marked)

| variant | exact % | size | insns | note |
|---|---:|---:|---:|---|
| **HEAD: mixed `float z, float y, int x` + status guard** | **70.5415** | 2764 | 1007/1006 | best |
| 48-way coord form/order grid (int/float × 6 orders) | 57.2–70.5415 | 2764–2788 | — | only the three equivalent `{x int, y float, z float}` permutations tie 70.5415 |
| waypoint order × coord order (36 combos) | 70.38–70.5415 | 2764 | — | `wz,wy,wx` + `z,y,x` is the max |
| all-int coords, after `new Player`, guard | 61.5461 | 2772 | 1007/998 | sinks below `setEmpData` |
| all-int coords, before `new Player`, guard | 61.5461 | 2772 | 1007/998 | still sinks (wave-128) |
| all-int coords, before `new Player`, **no guard** | 61.7223 | 2776 | 1007/1002 | **correct placement** (finding 1) |
| mixed coords, before `new Player`, no guard | 58.3459 | 2752 | 1007/988 | |
| mixed coords, after `new Player`, no guard | 63.5589 | 2748 | 1007/988 | |
| two-call `getLevel()` MIN (level only) | 68.6508 | 2772 | 1007/1009 | faithful wave-125 shape |
| two-call `getLevel()` MIN (empPoints only) | 68.3532 | 2772 | 1007/1009 | |
| two-call `getLevel()` MIN (both) | 68.6478 | 2780 | 1007/1012 | |
| two-call MIN + all-int + no guard | 60.8135 | 2792 | 1007/1009 | faithful but +28 B |
| `PlayerFixedObject *ship` instead of `KIPlayer *ship` | 70.5415 | 2764 | 1007/1006 | coalesced identically |
| `KIPlayer *volatile ship` | 55.7361 | 2832 | 1007/1024 | forces the store, too much code |
| `ship` moved before/after the guard | 70.5415 | 2764 | 1007/1006 | no change |
| guard condition `!status` / `status != nullptr` | 70.5415 | 2764 | 1007/1006 | identical CFG |
| guard on `shipClass`/`race`/`mission`/`type`/`waypoint`/`player` | 59.2–63.6 | 2748–2788 | — | all worse |
| `Vector pos` replaced by direct `(float)x,y,z` casts | 70.2434 | 2764 | 1007/1006 | -0.3 |
| ctor args before `Vector pos` | 70.2434 | 2764 | 1007/1006 | -0.3 |
| `float zero = 0.0f;` named zero (base and no-guard) | 70.5415 / 61.7223 | 2764 / 2776 | — | folded away |

## 5. Remaining lever

The exact original source is now strongly constrained:

* all-int `x`/`y`/`z` computed **before** `new Player` (finding 1);
* **no** `Globals::status == nullptr` guard;
* two-call `MIN(getLevel(), 20)` for both `level` and `empPoints` (the binary
  calls `getLevel()` twice in each clamp);
* the `0.0f` BoundingAAB argument kept in a callee-saved zero register.

The blocker is still the RA-time landing-pad order: it must put the cleanup
pads at the end of the *IR block list* so the exception PHI is short-lived and
`r8` (freed by the dead `z`) is chosen, freeing `r6` for the zero. A plain
`try`/`catch` moves the pads but changes the LSDA to `ttype 0x00`; no source
shape tried in waves 119–129 both moves the pads and keeps the 336 B
cleanup-only record.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w7648/{exact.py,sweep*.py,diffnorm.py}`.
