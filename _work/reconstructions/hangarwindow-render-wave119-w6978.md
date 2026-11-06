# `HangarWindow::render` — wave 119 (w6978): drop the +8 guard, land the ternary LSDA

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B.  Worktree `/home/debian/GalaxyOnFire2/wt/w6978`,
branch `wave119-w6978`, base master `ffb4027ca` (wave-119 continuation state, which
includes the w6960 `createShip` placement-new shape and its `HangarWindow` `NoopGuard`).

Builds on `hangarwindow-render-wave119-w6933.md` (the ternary amount block is the
faithful shape: 948 B LSDA, 91.7 %), `-w6942.md` (the −4 census) and
`level-createship-wave119-w6960.md` (the +8 guard that compensated `createShip` −8).

## Result

```
                                    before (HEAD ffb4027ca)   landed
HangarWindow::render fuzzy          89.739705                 91.655191
HangarWindow::render size           9700 B                    9700 B
HangarWindow::render LSDA record    952 B (+4)                948 B (exact)
HangarWindow::render insns          3635                      3636
Level::createShip LSDA              328 (-8)                  328 (-8)
Generator::createMission LSDA       176 (+4)                  176 (+4)
.ARM.extab size                    0x26298 (exact)           0x26294 (4 B short)
.rodata base                       0x1c9e10                  0x1c9e10 (exact)
.text size / base                  0x1337c8 / 0x69440        same (exact)
.ARM.exidx size / base             0x6f70 / 0x19cc08         same (exact)
file size                          2,187,328                 2,187,328 (exact)
27 sections                        yes                       yes
byte_exact / linked_exact          4505 / 4505               4505 / 4505
data_exact / imports / lint/drift  843 / 207 / CLEAN / 0     same
```

`check`: **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0
avg 99.94`; `data: exact 843 of 843`; `sodiff: imports 207`; lint `CLEAN ✓`;
drift `0`; 27 sections.

The landed source is the w6933 ternary amount block verbatim.  It removes the
`NoopGuard` and the `volatile int pad3` anchor that w6960 added to keep `.ARM.extab`
size-exact after the `createShip` −8.  The result is **exactly the task's reference
structural state**: `.ARM.extab` is 4 B short (`0x26294`), which fits the 16-byte
`.rodata` alignment gap, so `.rodata` stays at `0x1c9e10`, every function address is
unchanged and the file size is still 2,187,328 B.

## 1. The change

```diff
-                                    int right, left;
-                                    if (tab == 1) {
-                                        struct NoopGuard { ~NoopGuard() {} };
-                                        right = item->item->getStationAmount();
-                                        NoopGuard guard;
-                                        left = item->item->getAmount();
-                                    } else {
-                                        right = item->item->getAmount();
-                                        left = this->bluePrint->getCurrentAmount(item->item->getIndex());
-                                    }
-                                    if (left > 0) {
-                                        // Codegen anchor: the dead store keeps the
-                                        // list-body register roles (this -> r6) and
-                                        // the 9700-byte body of the original. Storing
-                                        // `left` (already in r0) avoids a reload of y.
-                                        volatile int pad3 = left;
+                                    // Ternaries (rather than an if/else) keep the
+                                    // original's per-arm landing pads: clang emits a
+                                    // distinct cleanup row for each ?: arm even when
+                                    // the arm code is identical, which is what the
+                                    // 948-byte .ARM.extab record encodes.
+                                    int right = tab == 1 ? item->item->getStationAmount() : item->item->getAmount();
+                                    int left = tab == 1 ? item->item->getAmount() : this->bluePrint->getCurrentAmount(item->item->getIndex());
+                                    if (left > 0) {
```

The body stays at exactly 9700 B: the two `?:` arms add +4 B and deleting `pad3`
removes −4 B.  The `render` LSDA record becomes exactly the original's 948 B
(previously 952 with the guard, or 944 without it).

## 2. Why the guard is no longer needed

Measured at `be3034dc1` (the task's reference commit): `createShip` 336 (exact),
`Generator::createMission` 168 (−4) and `HangarWindow::render` 944 (−4), i.e.
`.ARM.extab` **`0x26290` (8 B short)**, `.rodata` still `0x1c9e10`.  The current
branch adds the w6960 `createShip` placement-new shape and its guard:
`createShip` 328 (−8), `Generator` 176 (+4, guard) and `HangarWindow` 952 (+4,
guard), net 0 (`0x26298`).

Removing the `HangarWindow` guard gives `0 + (−8) + (+4) = −4`, i.e. `0x26294`.
`0x1a3b78 + 0x26294 = 0x1c9e0c`, which aligns back to `0x1c9e10`, so the section
size change is invisible to every address and to the file size.  The landed
section is therefore **4 B closer to the original than the task's reference
commit** while the target function improves 91.3 → 91.7.

## 3. Measured variants (official single-function scorer, 9700 B unless noted)

All rows are the ternary amount block unless marked `if/else`.  "rec" is the
`HangarWindow` `.ARM.extab` record length.

| variant | % | rec | note |
|---|---:|---:|---|
| **HEAD: if/else + `pad3` + `NoopGuard`** | **89.739705** | **952** | before |
| **landed: ternary, no `pad3`, no guard** | **91.655191** | **948** | LSDA exact |
| if/else + `pad3`, no guard | 91.337281 | 944 | w6922 baseline |
| ternary, guard between the two `?:` | 91.7 | 948 | guard is EH-neutral here |
| ternary, guard before both `?:` | 91.7 | 948 | same |
| ternary, guard after both `?:` | 86.9 | 952 | RA cascade |
| ternary, guard in `buttons[23]` slot branch | 90.0 | 952 | text-neutral +4 |
| ternary, guard after `buttons[0]->setVisible` | 89.9 | 952 | text-neutral +4 |
| ternary, guard after `buttons[0]` block | 89.6 | 952 | text-neutral +4 |
| ternary, guard around `buttons[8]` | 90.0 | 956 | +8 |
| ternary, guard around `buttons[9]` | 90.0 | 960 | +12 |
| if/else + `pad3`, guard between calls (HEAD) | 89.7 | 952 | before |
| if/else, guard between calls, no `pad3` | 91.0 | 940 | |
| if/else, guard between calls, non-volatile pad | 89.3 | 952 | |
| if/else, `left` first then `right` | 68.3 | 952 | |
| ternary + `pad3` | 87.6 | 956 | 9704 B |

The 952 B variants that keep the body at 9700 B are all in the 89.6–90.0 range; none
beats the guard-free ternary.  The `t_ship_b0`/`vis_725` (+4) placements are the
exact-extab fallback if the section size must stay `0x26298`: **90.0 % at 952 B**.

## 4. Attempts to move the +8 to another function (all dead ends)

The +8 must come from splitting a merged call-site row with an empty-destructor
`NoopGuard`.  The only functions with a row-count difference vs the original are
`HangarWindow` (156/157) and `createShip` (57/61); every other record matches.

| candidate | rec delta | text | cost | note |
|---|---:|---:|---:|---|
| `Generator` guard between `nextInt` calls | +8 | +16 B | −10.2 | 84.7 → 74.5 |
| `Generator` guard before `getTecLevel` | 0 | 0 | 0 | row already split |
| `Generator` guard before `getName` | 0 | 0 | 0 | |
| `SpaceLounge::drawLounge` guard before `GetTextWidth` | +4 | +4 B | −1.3 | text shift breaks addresses |
| `SpaceLounge::drawLounge` guard before `DrawRectangle` | +4 | +36 B | −19.9 | |
| `ModStation::ModStation` (5 placements) | 0 | 0 | 0..−1.9 | no row split |
| `createShip` guard around jitter/`new Player` | +100/+60 | −28/−28 B | −39/−36 | |

No text-neutral +8/+4 placement outside `HangarWindow` was found, so the guard is
simply removed and the section is allowed to sit in the alignment window.

## 5. Section / gate impact

| section | before | after | original |
|---|---:|---:|---:|
| `.text` size / base | `0x1337c8` / `0x69440` | same | same |
| `.ARM.exidx` size / base | `0x6f70` / `0x19cc08` | same | same |
| `.ARM.extab` size / base | `0x26298` / `0x1a3b78` | **`0x26294`** / `0x1a3b78` | `0x26298` |
| `.rodata` base | `0x1c9e10` | **`0x1c9e10`** | `0x1c9e10` |
| file size | 2,187,328 | 2,187,328 | 2,187,328 |
| sections | 27 | 27 | 27 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | 4505 / 4505 | — |
| `data_exact` / imports | 843 / 207 | 843 / 207 | — |
| avg fuzzy | 99.94 | 99.94 | — |
| lint / drift | CLEAN / 0 | CLEAN / 0 | — |

Raw section-byte deltas (orig vs ours): `.text` 88,216 → **88,116**;
`.ARM.exidx` 863 → **1,660**; `.ARM.extab` 109,364 → **135,597**.  The exidx/extab
raw-byte growth is only the 4-byte record shift (the `HangarWindow` record shrinks
948→944 and every later extab record moves), exactly as documented in the w6942
note; `.text` improves.

`exidx_table.py --diff` after the change:

```
functions with differing extab record length:
  _ZN5Level10createShipEiiiP8Waypointbb: orig 336 ours 328 delta -8
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE: orig 172 ours 176 delta +4
  total delta -4 (-0x4)
```

`HangarWindow::render` is now **exact** (948 B).

## 6. Proven vs hypothesis

**Proven**

* The ternary amount block is the faithful shape and gives the original's 948-byte
  LSDA record exactly, at 9700 B, with no guard and no `pad3`.
* Removing the w6960 guard raises `render` 89.739705 → **91.655191** (+1.9155 raw
  points) and leaves `byte_exact`/`linked_exact` at 4505/4505.
* `.ARM.extab` becomes `0x26294` (4 B short) but `.rodata` stays at `0x1c9e10`, all
  27 section headers are unchanged and the file size is exact.
* The 952 B variants that keep the body at 9700 B top out at 90.0 % (`buttons[23]`
  guard), so the guard-free ternary is the best score.

**Hypothesis / open**

* A text-neutral +8/+4 `NoopGuard` placement outside `HangarWindow` that would keep
  `.ARM.extab` exactly `0x26298` was not found; the only row-count differences are in
  `HangarWindow` and `createShip`, and every `createShip`/`Generator`/`SpaceLounge`/
  `ModStation` placement either changes `.text` or costs >1.6 points.

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint -j6       # CLEAN
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w6978/` — `score1.py` / `rawscore.py`
(single-function official scorer), `measure.py` (edit + link + score + LSDA),
`rows.py` (LSDA row dump), `sweep_*.py` (the guard/pad placement sweeps).
