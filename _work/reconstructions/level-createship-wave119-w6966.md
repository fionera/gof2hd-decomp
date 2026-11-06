# `Level::createShip` — wave 119 (w6966): land the 74.4 % placement-new body behind a +8 B `HangarWindow` LSDA guard

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave119-w6966`, base master `d512ec24e`. Worktree left clean.

> **Superseded in part (wave 119 w6978 / commit `9e15e6e92`).** The
> `HangarWindow` `NoopGuard` was replaced by ternaries; `createShip` stays
> 74.4 %, `HangarWindow::render` is 91.7 %, and `.ARM.extab` is 4 B short
> (`Generator::createMission` -4). The placement-new body and its analysis
> below remain the current source shape.

**Status: landed.** `createShip` 70.3 → **74.4 %**, body stays 2764 B,
`.ARM.extab` stays size-exact `0x26298`, `.rodata` stays at `0x1c9e10`,
file size stays 2,187,328 B. The +8 B needed to pay for the body's
328 B LSDA comes from a real RAII guard in `HangarWindow::render`
(the w6922 plan), which turns its −4 record into +4 and cancels
`Generator::createMission`'s +4.

```
                                    before (HEAD)        after
createShip fuzzy (official)         70.3                 74.4
createShip size                     2764 B               2764 B
createShip insns (orig/ours)        1007 / 1005          1007 / 1007
createShip LSDA record              336 B / 57 rows      328 B / 54 rows
createShip LSDA ttype               0xff (cleanup)       0x00 (try/catch type table)
HangarWindow::render fuzzy          91.3                 89.7
HangarWindow::render LSDA record    944 B (-4)           952 B (+4)
Generator::createMission LSDA       176 B (+4)           176 B (+4)
.ARM.extab                          0x26298 (exact)      0x26298 (exact)
.rodata base                        0x1c9e10 (exact)     0x1c9e10 (exact)
file size                           2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact           4505 / 4505          4505 / 4505
```

`check`: **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0 missing 0
extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: imports 207`, lint
CLEAN, `drift` 0 fields.

## 1. What changed

`src/game/world/Level.cpp` — `createShip` body replaced with the wave-89
placement-new + `try` shape (the `e2fa39a15` body, 74.4 %). Relative to the
HEAD real-`new` body this is:

* `PlayerFighter` / `PlayerFixedObject` allocated with
  `(T *) ::operator new(sizeof(T))` + placement `new (p) T(...)`
  (the `// lint: operator_call` waiver);
* `player->setEmpData(empPoints, empTime)` wrapped in
  `try { ... } catch (...) { __builtin_trap(); }`
  (the `// lint: arm_builtin anchor` waiver — it is also the `.text`
  size anchor: without it the body is 2776 B);
* two-call `getLevel() > 20 ? 20 : getLevel()` ternaries for both
  `level` and `empPoints` (the original's shape);
* no `Globals::status == nullptr` early return;
* the tuned spawn slot-map: `wz = waypoint->x`, `wy = waypoint->y`,
  `wx = waypoint->z`; jitter call order `Y,Z,X`; then
  `x = jitterZ + jitterX - 20000`,
  `y = (float)(wy + jitterY - 20000)`,
  `z = (float)(wz + wx - 20000)`.

`src/game/ui/HangarWindow.cpp` — a real RAII guard between the two
`tab == 1` calls in `render()`:

```diff
                                     if (tab == 1) {
+                                        struct NoopGuard { ~NoopGuard() {} };
                                         right = item->item->getStationAmount();
+                                        NoopGuard guard;
                                         left = item->item->getAmount();
```

The user-provided (non-trivial) empty destructor splits the merged
18-byte cleanup row into two 4-byte rows, exactly the original's row
structure; it adds +8 B to `HangarWindow::render`'s LSDA and keeps
`ttype = 0xff` (no type table). Cost: `render` 91.3 → 89.7 (still not
byte-exact, so `byte_exact` is unchanged).

## 2. Why the +8 is needed

At HEAD `.ARM.extab` is **size-exact** (`0x26298`) only by cancellation:
`HangarWindow::render` −4 and `Generator::createMission` +4. The
placement-new body's LSDA is 328 B (−8 vs the original 336 B), so
without a compensating +8 the section becomes `0x26290`.

`0x1a3b78 + 0x26290 = 0x1c9e08`, which still rounds up to the same
16-byte `.rodata` base `0x1c9e10`, so the *gate* would still pass — but
the file size would shrink by 8 B. The guard restores the exact
`0x26298` size and the exact file size, i.e. it is the w6922 plan with
the current (post-w6924) cancelling pair.

Measured LSDA deltas on the placement-new body (per-function
`.ARM.extab.text.*` record length, no link needed):

| added construct | record | Δ |
|---|---:|---:|
| none (e2fa body) | 328 | 0 |
| `NoopGuard` around `setEmpData` inside the `try` | 328 | 0 (optimised into the existing cleanup) |
| `NoopGuard` before `hardCoreMode()` | 404 | +68 |
| `NoopGuard` before `new Player` | 396 | +60 |
| `NoopGuard` before `getShipGroup()` | 348 | +12 |
| `NoopGuard` before `addObject(fixed->geometry)` | 340 | +4 |
| `NoopGuard` before `setBV()` | 340 | +4 |
| `NoopGuard` before `ship->setLevel()` | 340 | +4 |
| `NoopGuard` before `getCurrentCampaignMission()` | 440 | +104 |

No single in-function guard gives exactly +8; the `HangarWindow` guard
does, at the smallest score cost (89.7, the w6922 minimum among 24
measured +8 positions).

## 3. Real-`new` shape: still a hard 70.3 % wall

The coordinator's hint asked for a real-`new` shape that keeps the
336 B LSDA and beats 70.3 %. Every variant measured this pass ties or
loses (official scorer, one-TU mini `.so`, 2764 B unless stated):

| variant | % | size | LSDA |
|---|---:|---:|---:|
| HEAD (real `new`, one-call clamps, null check, XZY jitter) | **70.3** | 2764 | 336 |
| HEAD + tuned spawn slot-map (`x=jZ+jX`, `z=wz+wx`) | 70.3 | 2764 | 336 |
| HEAD + `setShipGroup(..., group)` | 62.7 | 2760 | 336 |
| HEAD + jitter XYZ order | 70.2 | 2764 | 336 |
| HEAD + null check at function top | 58.0 | 2784 | — |
| HEAD + null check before the coord block | 62.3 | 2776 | — |
| HEAD + null check before `new Player` | 31.9 | 2776 | 340 |
| HEAD + `int x,y,z` + `float fx,fy,fz` before `setEmpData` | 57.5 | 2776 | — |
| e2fa body + `int x,y,z` (faithful spawn, casts at use) | 61.6 | 2752 | 328 |
| e2fa body + `int x,y,z` (tuned spawn, casts at use) | 61.0 | 2752 | 328 |
| e2fa body, no `try`, `int x,y,z` (real or placement ships) | 61.7 | 2748 | 316 |
| e2fa body + status null check | 62.5 | 2764 | 328 |
| e2fa body + one-call `level` clamp | 69.6 | 2772 | 328 |
| e2fa body + one-call `empPoints` clamp | 70.7 | 2756 | 328 |
| e2fa body + one-call both clamps | 71.9 | 2764 | 328 |
| e2fa body + `setShipGroup(..., hostile)` | 73.9 | 2764 | 328 |
| **e2fa body (landed)** | **74.4** | 2764 | 328 |
| e2fa body, real `new` both ships, no `try` | 64.6 | 2768 | 336 |
| e2fa body, real `new` fighter only | 66.1 | 2780 | 328 |
| e2fa body, real `new` fixed only | 62.7 | 2760 | 328 |

The real-`new` prologue is the blocker: Greedy RA spills `shipClass`
(`sp#60`) instead of splitting it `r5 -> sl -> r6`, which cascades
`type` `sl`↔`fp`, `status` `r6`↔`r4`, `nextInt` `r4`↔`r6`, `ship`
`fp` vs `r9`, and 3 delete stubs instead of 4. The original is real
`new` (57 cleanup-only sites, `ttype 0xff`) yet has the placement-new
prologue; the real-`new` cleanup landing pads keep the allocated
pointer live across each ctor call and change that first Greedy
decision. No source rewrite tried here moves it while keeping 2764 B
and the 336 B record (see also w6916).

The placement-new body matches the original's first **98 instructions
byte-for-byte** (prologue, hitpoint chain, empPoints clamp, `Player`
ctor); the residual is the coordinate block (original computes `int
x,y,z` before `setEmpData`, ours converts to float there), the
`BoundingAAB` argument-setup sequences, and the register permutation in
the capital-ship zoo (`x` in `r6` vs the original's `sl`).

## 4. LSDA shape of the landed body

Parsed with the record-length-aware parser (`lsda4.py`, scratch):

```
original : rec=336 lsda_off=+12 lps=0xb0 ttype=0xff cs=0x1 rows=57 table_end=+336
landed   : rec=328 lsda_off=+12 lps=0xb0 ttype=0x00 cs=0x1 rows=54 table_end=+322
           (6 B type table at +322..+328)
```

The `try/catch` adds the type table and re-partitions the call-site
rows (54 vs 57); the row starts also shift because the body layout
differs. That is why the record cannot be made 336 B without matching
the original's body, not just its EH structure.

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

Scratch helpers (not committed): `/dev/shm/w6966/` — `score.py`
(one-function official scorer), `tryvar.py` / `batch.py` (splice body,
rebuild, score), `lsda4.py` (record-length-aware LSDA parser),
`head_body.cpp` (HEAD real-`new` body), `body_e2fa.cpp` (landed body).

## 6. Doc note

`docs/VALIDATION.md` and `docs/IDENTICALITY.md` still say
"`Level::createShip` is exact at 336 B (real-`new` shape)" and describe
`HangarWindow::render` as −4. After this pass the records are
`createShip` 328 (−8), `HangarWindow::render` 952 (+4),
`createMission` 176 (+4); the section size, `.rodata` base and file
size are unchanged. The source pass commits in this area do not touch
docs (see `e2fa39a15`, `b76b59bc3`, `6e7f4f47f`, `8f10cc659`); the
sentence needs a one-line update at the next docs sweep.
