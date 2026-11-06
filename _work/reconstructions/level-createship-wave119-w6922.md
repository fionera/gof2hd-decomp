# `Level::createShip` — wave 119 (w6922): LSDA census + the +8 B that lets the placement-new shape land

Target: `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).  Branch `wave119-w6922`,
base = master `298979092` (wave-119 continuation state).

Status: **research note.**  The best measured variant is the w6829
placement-new shape (72.1 %) **plus** a real RAII cleanup guard in
`HangarWindow::render` that adds exactly +8 B to `.ARM.extab`.  With both, the
section stays at `0x26290`, `.rodata` stays at `0x1c9e10`, `.text` stays
`0x1337c8`, and the gate is unchanged (`byte 4505 / linked 4505`).
The source change is recorded here (exact patch in §6) and was reverted before
committing so the docs are not left stale; see §7.

```
                                    before (HEAD)        best variant
createShip fuzzy (official)         70.3                 72.1
createShip size                     2764 B               2764 B
createShip LSDA record              336 B (exact)        328 B (-8)
HangarWindow::render fuzzy          91.3                 89.7
HangarWindow::render LSDA record    944 B (-4)           952 B (+4)
Generator::createMission LSDA       168 B (-4)           168 B (-4)
.text                               0x1337c8 (exact)     0x1337c8 (exact)
.ARM.exidx                          0x6f70 (exact)       0x6f70 (exact)
.ARM.extab                          0x26290 (8 B short)  0x26290 (8 B short)
.rodata base                        0x1c9e10 (exact)     0x1c9e10 (exact)
file size                           2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact           4505 / 4505          4505 / 4505
```

`check` on the best variant: **RATCHET PASS** — `verify: byte 4505 linked 4505
stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff:
imports 207`, lint CLEAN, drift 0, 27 sections.

---

## 1. Full `.ARM.extab` LSDA census (the requested first step)

Command (the committed parser; `--diff` prints every function whose record
length differs):

```
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

At HEAD this reports **exactly two** functions with a different LSDA size:

| function | original | ours | Δ | rows orig/ours |
|---|---:|---:|---:|---:|
| `_ZN12HangarWindow6renderEv` | 948 | 944 | **-4** | 156 / 156 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 172 | 168 | **-4** | 26 / 25 |
| **total** | | | **-8** | |

All other 2436 `EXT` records are byte-length identical.  A second pass
(`/dev/shm/w6922/lsdacmp2.py`) comparing the parsed LSDA header
(`lps_enc`, `ttype_enc`, `cs_enc`) and the call-site row count for every
function finds **no other difference**: there is no function where the
original has a catch handler (`ttype != 0xff`) or a landing pad that our
source lacks.  The section's whole -8 B is these two records.

Consequence for the coordinator's hypothesis ("a real try/catch / landing pad
in another function that the original has"): the only missing landing-pad
structure in the whole binary is `createMission`'s cold `pad=0` gap row; the
`HangarWindow` delta is *row encoding*, not a missing row (§3).

## 2. `Generator::createMission` — the missing `pad=0` row

Original `cslen = 151`, 26 rows; ours `cslen = 147`, 25 rows.  The extra
original row is

```
orig [7]  start=0x3c8 len=0x20 lp=0x0 act=0     (the cold getRoutes()==nullptr block)
```

ours has no gap there; its null block sits at `0x3e2` inside a cleanup row
(`0x3e2,0x36,lp=0x768`).  This is the w6780 finding; nothing new.

Adding the gap row to ours costs 5 B (`start 2 + len 1 + pad0 1 + action0 1`)
and gives `cslen = 152` → **176 B**, i.e. +8 over HEAD but +4 over the
original.  Exact 172 needs `cslen` 148..151, which requires the original's
different try-range partition (the shared `type` store chained with the else
arm), not a source spelling.  The w6780/w6835 sweeps already exhausted this;
re-confirmed here:

* inverting the `getOffer()==5` condition and swapping arms: cslen 147, 168 B
  (clang canonicalises the branch polarity back);
* hoisting the null block out of the else arm (real flag, `goto`, and
  `__builtin_expect(nullRoutes,0)`): cslen 147, 168 B, code identical;
* moving the null block after `getName()`: cslen 171/29 → 192 B, score
  83.6 → 48.1 (dead end).

## 3. `HangarWindow::render` — same row count, four fewer long rows

Both tables have 156 rows and `ttype=0xff`; the -4 is pure uleb128 encoding.
Row-byte distribution (`/dev/shm/w6922/rowlens.py`):

| row size | original | ours |
|---|---:|---:|
| 5 B (`lp=0`, start≥0x80) | 20 | 21 |
| 6 B | 125 | 127 |
| 7 B (a start/len/lp ≥ 0x80) | **11** | **8** |
| total | 927 | 923 |

The original has **18 rows with `len >= 0x80`**; ours has **14**.  The four
extra 7-byte rows are exactly the +4.  They are the alternating
cleanup/gap rows of the capital-ship `BoundingAAB` construction sequence and
of the item loop; the difference is *invoke grouping* (which calls clang
merges into one call-site range) and therefore a body-layout consequence, not
a missing landing pad.  Example (original vs ours at the same code):

```
orig [16] (1406, 4, 9418, 0)   [17] (1416, 4, 9588, 0)
ours [16] (1408,18, 9586, 0)
```

i.e. the original keeps the `getStationAmount()` and `getAmount()` invokes in
two 4-byte cleanup rows (two distinct landing-pad stubs); ours merges them
into one 18-byte row.

## 4. Sink sweep: the only reachable deltas are 0, +8, +12, +16

A semantically-null local (`int padS = 0; (void) padS;`) inserted after every
call statement in the function, compiled one TU at a time and read from the
per-function `.ARM.extab.text.*` section (no link needed):

* `HangarWindow::render`: 148 candidate statements → deltas
  `{0: 114, +8: 24, +12: 9, +16: 1}`.  **No +4.**
* `Generator::createMission`: 46 candidate statements → `{0: 35, +8: 5}`.
  **No +4.**

A +4 split would need a row with `len >= 0x80` split into a cleanup row plus
a `pad=0` row (or the initial `pad=0` row split at offset ≥ 0x80); none of the
reachable split points has that shape.  All non-zero deltas also add a new
landing-pad stub, so they move the function's own code stream.

## 5. The createShip placement-new shape and its LSDA

The w6829 source shape (placement `new`, two-call ternaries, `try/trap` around
`setEmpData`, no status null check, faithful spawn, `hostile`) measures
**72.1 %** on current master (XYZ jitter order; the w6833 XZY order gives
70.2 % for this shape).  Its LSDA is **328 B / 54 rows**, but the header is
`ttype_enc = 0x00` — the `try/catch` adds a type table.  The original is
**336 B / 57 rows, `ttype_enc = 0xff`** (cleanup-only, real `new`).

Without the `try/catch` the shape is 316 B (`ttype=0xff`) and `.text` grows by
0x20, so the `try/trap` is also the `.text`-size anchor.  The shape is
therefore *not* EH-identical to the original; it is a register-allocation win
(the exception-pointer vreg goes to r8 instead of r6, reproducing the
original's callee-saved permutation).  That is why the +8 must be found
elsewhere.

## 6. The +8: a real RAII cleanup guard in `HangarWindow::render`

Inserting an empty-destructor guard between the two `tab == 1` calls splits
the merged 18-byte cleanup row into two 4-byte rows — the original's row
structure:

```diff
--- a/src/game/ui/HangarWindow.cpp
+++ b/src/game/ui/HangarWindow.cpp
@@ -569,7 +569,9 @@ void HangarWindow::render() {
                                     int right, left;
                                     if (tab == 1) {
+                                        struct NoopGuard { ~NoopGuard() {} };
                                         right = item->item->getStationAmount();
+                                        NoopGuard guard;
                                         left = item->item->getAmount();
                                     } else {
```

Resulting rows:

```
orig [16] (1406, 4, 9418, 0)   [17] (1416, 4, 9588, 0)
ours [16] (1408, 4, 9564, 0)   [17] (1422, 4, 9332, 0)
```

The row *structure* now matches the original (two 4-byte cleanup rows); the
landing-pad addresses differ because the cleanup blocks sit at different
addresses.  `ttype_enc` stays `0xff` (no type table).  Cost: render
91.3 → 89.7, size stays 9700 B.  This is the least-damaging +8 of the 24
measured +8 positions; the best alternatives were 89.7 at source lines 572,
737 and 796, the worst 64.9 (line 602, function shrinks to 9660 B).

The same +8 can be obtained with the repo's usual `int padS = 0; (void) padS;`
sink (identical 89.7 / 9700 B); the guard is preferred because it is a real
C++ object with a destructor and keeps `ttype=0xff`.

Full patch (also re-applies the w6829 createShip shape):

```diff
--- a/src/game/world/Level.cpp
+++ b/src/game/world/Level.cpp
@@ -5179,10 +5179,9 @@
     int jitterX = Globals::rnd->nextInt(40000);
-    int jitterZ = Globals::rnd->nextInt(40000);
     int jitterY = Globals::rnd->nextInt(40000);
-    int level = Globals::status->getLevel();
-    if (level > 20) level = 20;
+    int jitterZ = Globals::rnd->nextInt(40000);
+    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
@@ -5198,9 +5197,7 @@
-    int empPoints = Globals::status->getLevel();
-    if (empPoints > 20) empPoints = 20;
-    empPoints = empPoints * 5 + 40;
+    int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
@@ -5218,12 +5215,16 @@
-    player->setEmpData(empPoints, empTime);
+    try {
+        player->setEmpData(empPoints, empTime);
+    } catch (...) {
+        __builtin_trap(); // lint: arm_builtin anchor: Level::createShip; removal grows body 2764 -> 2772
+    }
 
-    if (Globals::status == nullptr) return nullptr;
     KIPlayer *ship = nullptr;
     if (shipClass == 0) {
-        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
+        PlayerFighter *fighter = (PlayerFighter *) ::operator new(sizeof(PlayerFighter)); // lint: operator_call
+        new (fighter) PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
@@ -5238,7 +5239,8 @@
-        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
+        new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
```

## 7. What is proven vs hypothesis

**Proven**

* The `.ARM.extab` -8 B is exactly `HangarWindow::render` -4 and
  `Generator::createMission` -4; no other function's LSDA differs in size,
  row count, `ttype_enc` or `lps_enc`.
* `HangarWindow`'s -4 is row *encoding* (original 18 rows with `len>=0x80`,
  ours 14), not a missing landing pad.
* `createMission`'s -4 is one missing `pad=0` gap row for the cold
  `getRoutes()==nullptr` block; adding it costs +8 (176 B, +4 over original).
* A plain statement sink can only produce 0/+8/+12/+16 in either function;
  there is no +4 split reachable from the current body.
* The RAII guard adds exactly +8 to `HangarWindow::render` (944 → 952),
  keeps `.text` at `0x1337c8`, keeps `ttype=0xff`, and restores the
  original's two-row cleanup structure.
* With guard + placement-new shape: `.ARM.extab` stays `0x26290`,
  `.rodata` stays `0x1c9e10`, file size exact, gate `byte 4505 / linked 4505`.

**Hypothesis / open**

* The original's four extra long rows in `HangarWindow` are an invoke-grouping
  consequence of its body (register allocation / block order); matching them
  without a guard requires matching the 89.7 % body, not a source spelling.
* The placement-new createShip shape is not EH-identical to the original
  (`ttype=0x00` vs `0xff`, 54 vs 57 rows); it is a register-allocation win.
  A shape that keeps the real-`new` 336 B cleanup structure *and* the
  placement-new RA was not found.

**Doc impact if landed:** `docs/VALIDATION.md` and `docs/IDENTICALITY.md`
currently say "`Level::createShip` is exact at 336 B (real-`new` shape)" and
explain the -8 as "createMission -4 plus 4 B tail alignment".  Landing this
makes createShip 328 B and HangarWindow +4; the sentence must be updated in
the same commit (the section size and `.rodata` base do not change).

## 8. Reproduction

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

Scratch helpers (not committed): `/dev/shm/w6922/` —
`lsda.py` (dump one record's call-site table), `rowlens.py` (per-row byte
lengths), `lsdacmp2.py` (row-count/header census), `measure.py` (per-function
record sizes + section sizes), `score1.py` (one-function official fuzzy
score), `sweep_hw.py` / `sweep_gen.py` (sink delta sweeps),
`sweep_hw_score.py` (delta + score for the 24 `+8` positions), `best.patch`.
