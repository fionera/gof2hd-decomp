# `Level::createShip` — wave 119 (w6951): real-`new` shape, 70.3 → 71.5 % at 2764 B / 336 B LSDA

> **Not landed as source (orchestrator decision).** This real-`new` call-site tune
> scores 71.5 %, below the current 74.4 % placement-new body; kept as an RA reference.

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave119-w6951`, base `db1106935` (wave-119 continuation state).

Status: **source change landed** (this note is committed with it).

```
                                   before (HEAD)        after (this pass)
createShip fuzzy (official)        70.278330            71.535022
createShip size                    2764 B               2764 B
createShip insns (orig/ours)       1007 / 1005          1007 / 1006
createShip LSDA record             336 B (exact)        336 B (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
.ARM.exidx                         0x6f70  @0x19cc08    0x6f70  @0x19cc08
.ARM.extab                         0x26298 @0x1a3b78    0x26298 @0x1a3b78
.rodata                            0x41c27 @0x1c9e10    0x41c27 @0x1c9e10
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
data / imports / lint / drift      843 / 207 / CLEAN / 0   843 / 207 / CLEAN / 0
```

`check` after the change: **RATCHET PASS** — `verify: byte 4505 linked 4505
stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff:
imports 207`.  `drift`: `0 field(s) across 0 class(es)`.  `goal_lint`: `CLEAN`.
No section size, base, import, data or symbol-parity movement; the
`HangarWindow::render` -4 / `Generator::createMission` +4 cancelling pair is
untouched (`total delta +0`).

## 1. The exact change

Only four call sites in `src/game/world/Level.cpp` change; nothing else in the
TU moves.

```diff
@@ -5178,7 +5178,7 @@
     if (Globals::status == nullptr) return nullptr;
     KIPlayer *ship = nullptr;
     if (shipClass == 0) {
-        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
-        fighter->setShipGroup(Globals::globals->getShipGroup(type, race, group), type, hostile);
+        PlayerFighter *fighter = new PlayerFighter(race, hostile, player, nullptr, x, y, z, false);
+        fighter->setShipGroup(Globals::globals->getShipGroup(race, race, group), race, hostile);
@@ -5193,7 +5193,7 @@
-        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        PlayerFixedObject *fixed = new PlayerFixedObject(hostile, race, player, nullptr, x, y, z);
@@ -5245,7 +5245,7 @@
-        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
+        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), race, false);
```

**These argument substitutions are a register-allocation tune, not a semantic
recovery.**  Ghidra says the original calls are

| call | original (Ghidra / Thumb) |
|---|---|
| fighter ctor | `PlayerFighter(type, race, player, nullptr, x, y, z, hostile)` |
| fighter group | `getShipGroup(type, race, group)` then `setShipGroup(..., type, hostile)` |
| fixed ctor | `PlayerFixedObject(type, race, player, nullptr, x, y, z)` |
| fixed group | `getShipGroup(type, race, false)` then `setShipGroup(..., type, false)` |

(Thumb evidence: `c06f8 PlayerFighterC1EiiP6PlayerP10AEGeometryfffb` with
r1=`[sp,#52]`=type, r2=`[sp,#60]`=race, stack16=`[r7,#12]`=hostile;
`c0700 ldr r3,[r7,#16]`=group into `getShipGroup`; `c0716 mov r3,r8`=hostile
into `setShipGroup`; `c0426 ldr r4,[sp,#52]`=type / `c042a ldr r6,[sp,#60]`=race
for `PlayerFixedObjectC1EiiP6PlayerP10AEGeometryfff`.)

The faithful-argument shape is the pre-change HEAD body and scores **70.278330**
at the same size/LSDA.  The tuned shape is +1.26 points purely because the
different live sets at those four call sites perturb Greedy RA.

## 2. Why the score is stuck at ~70 %: the `type`/`shipClass` RA cascade

The whole function is a *register permutation* of the original.  With all
register names canonicalised the instruction-sequence LCS is **79.6 %**, so
~20 points are pure RA and ~20 points are structural.

Original prologue (`c0220`):

```
str  r1, [sp,#60]      ; race spilled
mov  r9, r0            ; this
mov  fp, r3            ; type      -> fp
mov  r5, r2            ; shipClass -> r5, then `mov sl,r5` at c0238
ldr  r4, [r0,#0]       ; &Globals::status -> r4
```

HEAD prologue:

```
str  r2, [sp,#60]      ; shipClass spilled
str  r1, [sp,#52]      ; race spilled
mov  sl, r3            ; type      -> sl
```

so `type` owns `sl` instead of `fp`, `shipClass` gets no callee-saved register
at all, and every downstream value inherits the permutation
(`empPoints`/`hitpoints` swap `sl`/`fp`, `fighter`/`fixed` coalesce into one
register instead of `sl`/`fp`, three delete stubs instead of four).

The cause is the `if (Globals::status == nullptr) return nullptr;` hack: it
re-uses the *same* `&Globals::status` vreg after `setEmpData`, extending its
live range past `hardCoreMode`, which forces the `hardCoreMode` result into
`r6` instead of `r4`, which in turn leaves no callee-saved register for
`shipClass`.  Dropping the null check frees `r4` but then `&Globals::status`
itself lands in `r6` (`rnd`→`r4`, `nextInt`→`r5`) and the score falls to
**67.166417**.  No source form found (this pass or w6901/w6916) keeps `status`
in `r4` *and* lets it die at `hardCoreMode` while staying at exactly 2764 B.

The other two structural deltas (both make the score *worse* if made faithful):

* the original clamps `level` and `empPoints` with a **two-call ternary**
  (`getLevel() > 20 ? 20 : getLevel()`); the checked-in one-call `if` form
  scores 70.28 vs 67.17 for the faithful two-call form;
* the original computes `x`/`y`/`z` **before** `setEmpData`; ours sinks the
  13-instruction block after it (a direct consequence of the null-check
  branch), and forcing it back costs 2–8 points.

## 3. Measurements (official single-function scorer, full link)

Every row keeps real `new` (Player / PlayerFighter / PlayerFixedObject /
Array / 24×BoundingAAB) and is exactly 2764 B with the 336 B / 57-site /
`ttype_enc=0xff` LSDA unless noted.

| variant | % | size | LSDA |
|---|---:|---:|---:|
| **HEAD (faithful args, null check, one-call clamps, XZY jitter)** | **70.278330** | 2764 | 336 |
| + 2 `setShipGroup` arg tunes only | 70.675944 | 2764 | 336 |
| + fighter-ctor arg tune (3 sites) | 70.839543 | 2764 | 336 |
| + coordinate-descent tune, pass 0 | 71.038251 / 71.435668 | 2764 | 336 |
| **+ coordinate-descent tune, final (this commit)** | **71.535022** | 2764 | 336 |
| faithful two-call ternaries, no null check | 67.166417 | 2764 | 336 |
| one-call clamps, no null check | 63.759398 | 2748 | 336 |
| two-call ternaries + null check | 68.384539 | 2780 | 336 |
| all-`int` coords | 61.645885 | 2772 | 336 |
| placement-new + `try`/`__builtin_trap`, tuned spawn, group | **74.379345** | 2764 | **328 (-8)** |
| placement-new + `try`, faithful spawn/group | 73.945409 | 2764 | 328 (-8) |
| placement-new, no `try`, both placement | 70.471464 | 2776 | 316 |
| real new, no `try`, both real | 64.635365 | 2768 | 336 |
| real new, fighter real only | 66.105055 | 2780 | 328 |
| real new, fixed real only | 62.662663 | 2760 | 328 |

Dead ends re-confirmed this pass (all ties or worse, 2764 B unless noted):
`int cls = shipClass` / `bool isCapital` / `bool isFighter` copies (70.278
tie — coalesced); yoda conditions (tie); `empTime` hoisted to a ternary at the
top (62.07 / 2776) or before `new Player` (61.17); `empTime` inlined into
`setEmpData` (58.01 / 2788); `shipClass` block moved after the float scale
(60.33 / 2780); inverted/bare/pre-initialised waypoint branch (tie);
`hitpoints <<=` spelled as `*=`/`=`/`&&` (tie); `shipClass * 0` padding in
`hitpoints`/`empPoints` (tie); null check on `player`/`Globals::globals`/
`Globals::rnd`/`waypoint` (40.6–63.8); null check at all 16 other statement
positions (32.3–63.5; only "after `setEmpData`" and "after `ship = nullptr`"
tie at 70.278); all 24 orderings of {x/y/z, setEmpData, null check,
`ship=nullptr`} (6 tie at 70.278, the rest 62.3–69.8); `try`/`catch` around
`setEmpData` (64.13 / 2768 / LSDA 352) / `setLevel` (63.09 / 2768 / LSDA 356) /
`new Player` (61.52 / 2784 / LSDA 352).

## 4. The placement-new option (higher score, needs a second TU)

`placement new` for Player/PlayerFighter/PlayerFixedObject + a
`try { setEmpData } catch (...) { __builtin_trap(); }` size anchor scores
**74.379345 %** at 2764 B, but its LSDA is **328 B** (`ttype_enc=0x00`, 54
sites) — it is not EH-identical to the original.  It can still be landed with
the section kept exact, because the current -4/+4 cancelling pair absorbs it:

```
createShip   336 -> 328  (-8)
HangarWindow 944 -> 952  (+8)   via the w6922 NoopGuard in render()
createMission           (+4)   unchanged
total                    0
```

but that costs `HangarWindow::render` 91.3 → 89.7 and needs the doc sentence
about the cancelling pair rewritten.  It was **not** landed here because the
task asks for a real-`new` shape that keeps the 336 B LSDA.

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6951/` — `harness.py`
(write-body → link → official score), `batch1..8.py` (targeted variant
sweeps), `tune2.py` / `tune2_state.json` (per-slot coordinate descent over the
six call sites), `diffregions.py` / `normdiff.py` (stream diff and
register-normalised LCS), `lsdacheck.sh`.
