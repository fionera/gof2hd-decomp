# `Level::createShip` — wave 119 (w6937): placement-new shape lands at 72.1 % with a real +8 LSDA compensation in `Level::createMission`

Target: `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).  Branch
`wave119-w6937`, base = current master `98f531de9`.

Status: **source change landed.**  The w6829 placement-new shape (70.2/70.3 %)
is re-landed, and the +8 B `.ARM.extab` shortfall it creates is compensated by
a real empty-destructor RAII guard in `Level::createMission` (cost **0.4**
points, not the 1.6-point `HangarWindow` guard).  `.ARM.extab` stays
size-exact `0x26298`, `.text` stays `0x1337c8`, `.rodata` stays `0x1c9e10`,
file size stays 2,187,328 B, and the gate is unchanged at
`byte 4505 / linked 4505`.

```
                                    before (HEAD)        after (landed)
createShip fuzzy (official)         70.2 (task 70.3)     72.1
createShip size                     2764 B               2764 B
createShip LSDA record              336 B (exact)        328 B (-8)
Level::createMission fuzzy          92.3                 91.9   (-0.4)
Level::createMission LSDA record    956 B (exact)        964 B (+8)
Generator::createMission LSDA       172 B (orig)         176 B (+4)
HangarWindow::render LSDA           948 B (orig)         944 B (-4)
.text                               0x1337c8 (exact)     0x1337c8 (exact)
.ARM.exidx                          0x6f70 (exact)       0x6f70 (exact)
.ARM.extab                          0x26298 (exact)      0x26298 (exact)
.rodata base                        0x1c9e10 (exact)     0x1c9e10 (exact)
file size                           2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact           4505 / 4505          4505 / 4505
```

`check` on the landed state: **RATCHET PASS** — `verify: byte 4505 linked 4505
stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff:
imports 207`, lint CLEAN, drift 0, 27 sections.

---

## 1. Why the +8 is *not* needed to keep `.rodata` in place

The w6922 note concluded that the placement-new createShip shape (328 B, -8)
had to be compensated because `.ARM.extab` would cross the 16-byte alignment
boundary and move `.rodata`.  That was measured when
`Generator::createMission` was still 168 B (-4).  Since commit `09a378ee1`
Generator is **176 B (+4)**, so:

```
HEAD:   .ARM.extab = 0x26298  (HangarWindow -4, Generator +4, createShip 336)
        end = 0x1a3b78 + 0x26298 = 0x1c9e10   -> .rodata 0x1c9e10
createShip -8 only:
        .ARM.extab = 0x26290
        end = 0x1a3b78 + 0x26290 = 0x1c9e08   -> rounds up to 0x1c9e10
```

So the placement-new shape alone already keeps `.rodata` (and the file size)
exact; the +8 is required only to keep the **section size** exact
(`0x26298`), which is the documented invariant and the 27-section parity.

Measured directly (build with only the createShip patch):

```
.ARM.extab 0x26290   .rodata 0x1c9e10   file 2,187,328
byte 4505 linked 4505   createShip 72.1 %
```

## 2. The landed source change

`src/game/world/Level.cpp`, two independent pieces:

**(a) createShip: the w6829 placement-new shape** (faithful spawn X,Y,Z,
two-call ternaries, `try/trap` around `setEmpData`, placement `new` for both
ship ctors, no status null check):

```cpp
-    int jitterZ = Globals::rnd->nextInt(40000);
     int jitterY = Globals::rnd->nextInt(40000);
-    int level = Globals::status->getLevel();
-    if (level > 20) level = 20;
+    int jitterZ = Globals::rnd->nextInt(40000);
+    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
...
-    int empPoints = Globals::status->getLevel();
-    if (empPoints > 20) empPoints = 20;
-    empPoints = empPoints * 5 + 40;
+    int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
...
-    player->setEmpData(empPoints, empTime);
+    try {
+        player->setEmpData(empPoints, empTime);
+    } catch (...) {
+        __builtin_trap(); // lint: arm_builtin anchor: Level::createShip; removal grows body 2764 -> 2772
+    }
...
-    if (Globals::status == nullptr) return nullptr;
...
-        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
+        PlayerFighter *fighter = (PlayerFighter *) ::operator new(sizeof(PlayerFighter)); // lint: operator_call
+        new (fighter) PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
...
-        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
+        new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
```

**(b) the +8 compensation: a real RAII guard in `Level::createMission`**
(inside the `case4Tail` attacker spawn block, after the first
`setPosition`; empty destructor, no behaviour change):

```cpp
                     (*this->enemies)[attackerCount]->setPosition(-2500.0f, -300.0f, 27000.0f);
+                    struct NoopGuard { ~NoopGuard() {} };
+                    NoopGuard guard;
                     (*this->enemies)[attackerCount + 1]->setPosition(6500.0f, 3000.0f, 24000.0f);
```

The guard makes clang emit one extra cleanup landing pad / call-site row for
the following `setPosition` calls.  It adds exactly **+8 B** to
`Level::createMission`'s LSDA record (956 -> 964) and **0 B** to `.text`
(the landing pad fits in existing padding, function size stays 17172 B).

## 3. The +8 search: measured candidates

All candidates are real empty-destructor guards inserted at a block start or
after a call statement, compiled one TU at a time and measured from the
per-function `.ARM.extab.text.*` section.  "cost" is the function's own fuzzy
score delta.

| function | insertion | extab Δ | text Δ | score cost | byte_exact |
|---|---|---:|---:|---:|---|
| **Level::createMission** | after first attacker `setPosition` (line 2375) | **+8** | **0** | **0.4** (92.3 -> 91.9) | **kept** |
| loadingScreen (GameInit.cpp) | start of `if (getLanguage()==9)` RTL block | +8 | 0 | 0.4 (100 -> 99.6) | loses 1 (4504) |
| HangarWindow::render | between the two `tab==1` amount calls | +8 | 0 | 1.6 (91.2 -> 89.6) | kept |
| StarSystem::StarSystem(int) | start of `mission==0 && mode==3` block | +8 | 0 | 15.0 (100 -> 85.0) | loses 1 |
| Radar::Radar(Level*) | start of label-strings block | +4 | 0 | 3.5 (100 -> 96.5) | loses 1 |
| BeamGun::BeamGun(...) | start of secondary-mesh `else` block | +4 | 0 | 3.6 (100 -> 96.4) | loses 1 |
| Hud::draw(...) | start of event-queue block | +8 | +4 | 2.4 (96.0 -> 94.4) | kept, but `.text` +0x20 |
| MGame::OnUpdate() | after `pauseOpen = 1` | +8 | +4 | 2.6 (99.9 -> 97.3) | kept, but `.text` +0x20 |
| PlayerFixedObject ctor | start of wrecked-mesh block | +8 | +4 | n/m | layout shift |

The winner is `Level::createMission`: it is a non-byte-exact function
(92.3 %), so the guard does not cost a `byte_exact`; it adds exactly +8 to
the LSDA with **no** `.text` change; and its own score cost is 0.4 points,
inside the requested <=0.5 budget.  `loadingScreen` is equally cheap but
would drop `byte_exact` 4505 -> 4504 (RATCHET FAIL without a baseline
update), so it was rejected.

### 3.1 Why the w6922 `HangarWindow` guard was replaced

The w6922 note already measured the `HangarWindow::render` guard: +8 B LSDA,
0 B `.text`, cost 1.6 points (91.2 -> 89.6).  It works, but it is 4x the
requested budget.  The `Level::createMission` guard is the same mechanism
(empty-destructor cleanup split of a merged call-site range) at 0.4 points
and in the same TU as the target function.

## 4. `.ARM.extab` record census after landing

```
$ python3 _work/reconstructions/exidx_table.py --diff \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
entries: orig 3566 ours 3566
  CANT orig 272 ours 272
  INL  orig 856 ours 856
  EXT  orig 2438 ours 2438

functions with differing extab record length:
  _ZN12HangarWindow6renderEv: orig 948 ours 944 delta -4
  _ZN5Level10createShipEiiiP8Waypointbb: orig 336 ours 328 delta -8
  _ZN5Level13createMissionEv: orig 956 ours 964 delta +8
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE: orig 172 ours 176 delta +4
  total delta +0 (0x0)
```

The four records now cancel exactly; the section size is unchanged.

## 5. Proven vs hypothesis

**Proven**

* With `Generator::createMission` at 176 B (+4), the placement-new createShip
  shape (328 B, -8) does **not** move `.rodata`: `0x1a3b78 + 0x26290 =
  0x1c9e08`, which rounds up to `0x1c9e10`.  The +8 is only needed for
  section-size parity.
* A real empty-destructor guard in `Level::createMission` adds exactly +8 B
  to its LSDA record and 0 B to `.text`; its score cost is 0.4 points
  (92.3 -> 91.9).
* The landed state is `byte 4505 / linked 4505`, `data 843/843`,
  `imports 207`, lint CLEAN, drift 0, 27 sections, file size 2,187,328 B,
  `.text`/`.ARM.exidx`/`.ARM.extab`/`.rodata` base all exact.
* The w6829 placement-new createShip shape reproduces the original's
  callee-saved permutation (exception pointer in r8, `this` r9 spilled at
  `0xc0332`, `type` fp spilled at `0xc0336`) and is 1007 insns, 2764 B.

**Hypothesis / open**

* The exact original createShip EH shape is still not reproduced: the
  placement-new shape is 328 B / `ttype_enc=0x00` (the `try/trap` adds a
  type table) vs the original 336 B / `ttype_enc=0xff`.  The 72.1 % is a
  register-allocation win, not an EH-structural match.
* The `Level::createMission` guard is a compensation device, not an original
  source feature; the original's 956 B record has no such row.  It is
  semantically inert (empty destructor) and is documented here.

**Doc impact:** `docs/VALIDATION.md` and `docs/IDENTICALITY.md` currently
say `.ARM.extab` is size-exact with "HangarWindow -4 / Generator +4" and
"createShip is exact at 336 B (real-`new` shape)".  The landed state changes
that to "HangarWindow -4 / createShip -8 / createMission +8 / Generator +4,
createShip 328 B (placement-new shape)".  The section size and `.rodata`
base are unchanged.  The docs were left untouched per this worker's
instruction ("do not commit docs unless the gate requires it"); the numbers
above are the authoritative update.

## 6. Reproduction

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

Scratch helpers (not committed): `/dev/shm/w6937/` — `cc1.py` (single-TU
compile), `extab.py` (per-function `.ARM.extab.text.*` size), `dumprows.py`
(LSDA call-site rows), `locate.py` (robust function→source line), and the
block/statement sweep scripts `sweep_blocks3.py` / `sweep_stmts3.py`.
