# Extab census wave 146 (w9219): LSDA row / landing-pad root causes

Status: **analysis + decoder, no landed `src/` change** (Radar::Radar scope
probes measured; the base shape is best). Worktree `wt/w9219`, base `31297fadc` (wave 145).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. `.ARM.extab` **1,370** differing bytes at exact 156,312 B;
`.text` 85,651; `.ARM.exidx` byte-identical; `.rodata`/`.data`/`.bss` exact.

## 0. What was built

`_work/reconstructions/extab_census.py` (new, committed):

```sh
python3 _work/reconstructions/extab_census.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so \
  .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```

For each of the 11 differing `.ARM.extab` owners it:

* decodes both LSDA records (generic personality, LPStart enc `0xff`, so all
  `start`/`len`/`lp` values are function-relative);
* disassembles both functions and annotates every row with the `bl`/`blx`
  targets inside its `[start, start+len)` range;
* greedily aligns the two row sequences on the call-target list and classifies
  each difference as `LP-REMAP` (same range, different landing pad), `SHIFT`
  (same calls, range moved), `SPLIT->k` / `MERGE<-k` (row-boundary change) or
  `ROW` (unmatched);
* prints the histogram of `ours_lp - orig_lp` for same-range rows. A single
  delta is an **address shift** (the LP code moved with the body); mixed
  `+d`/`-d` deltas are an **identity/order swap** (the same call site points at
  a different landing-pad block).

## 1. Census summary

`bodyraw` is the raw same-address `.text` diff bytes for the function (the
primary metric; `fdiff.py`). `lp-deltas` is the same-range LP delta histogram.

| extab | fn | owner | bodyraw | rows | lp-deltas | root cause |
|---:|---|---|---:|---|---|---|
| 762 | 0x1490d8 | `HangarWindow::render` | 1719 | 156/156 | {-4:4, -2:2} | body layout + `iconX`/`iconY` VarDecl split (rows 125/126) + borderX row 19/20 |
| 158 | 0xaffd8 | `Level::createMission` | 5592 | 148/148 | {-18:5, -10:16, -6:34} | body layout: 55 LP address shifts, 40 call-site shifts |
| 150 | 0x137d18 | `LevelScript::process` | 54895 | 46/46 | {} | body layout only: 25 SHIFT + 21 ROW, no LP identity change |
| 93 | 0xc0214 | `Level::createShip` | 1978 | 57/57 | {-2:1, +6:3} | body layout: `Array<BoundingVolume>` LP -2, `BoundingAAB` LP +6 |
| 86 | 0x12e050 | `Radar::draw` | 8373 | 19/19 | {-2:1} | body layout: `String::operator=` LP -2, 11 SHIFT |
| 37 | 0x163b90 | `Hud::draw` | 1325 | 77/77 | {-2:5} | `float filled` VarDecl split (rows 72/73) + 5 LP -2 address shifts |
| 37 | 0x17a144 | `MGame::OnTouchEnd` | 4180 | 44/44 | {} | body layout only: 11 SHIFT + 5 ROW |
| 24 | 0x971b8 | `Generator::createMission` | 445 | 26/26 | {-8:1, -2:2} | body layout (the `bonus`/`rem` RA of w145 §3) |
| 14 | 0x170d38 | `SpaceLounge::drawLounge` | 175 | 25/25 | {-10:1, +2:5} | **LP identity swap** among identical cleanup trampolines |
| 7 | 0x12d484 | `Radar::Radar` | **0** | 39/39 | {-2:2, +2:5} | **LP identity swap** among identical cleanup trampolines |
| 2 | 0xd52d0 | `ModStation::ModStation` | 11 | 15/15 | {} | RA of the `MatrixSetRotation` 4th arg (`r3` vs `r4`+`mov`) |
| **1,370** | | | | | | |

**The three root-cause families:**

1. **Body-layout only** (ModStation 2, LevelScript 150, MGame 37,
   Level::createMission 158, Radar::draw 86, Level::createShip 93,
   Generator 24 = **550 B**). The row *structure* is identical; the call sites
   and/or landing-pad code moved because the body codegen differs. These will
   only fall when the body is reconstructed byte-exactly; there is no separate
   EH source construct to fix.
2. **LP identity/order swap among identical trampolines** (Radar::Radar 7,
   SpaceLounge 14 = **21 B**). The body is byte-exact (Radar) or near-exact
   (SpaceLounge, 175 raw). The same call site points at a *different*
   landing-pad block, but every such block is the same `b.n` trampoline to the
   same cleanup. This is a pure EH-scope-ordering difference, not codegen.
3. **VarDecl lifetime split** (HangarWindow rows 125/126, Hud rows 72/73 =
   the known `iconX`/`iconY` and `filled` splits). A local `VarDecl` between
   two invokes pushes a `CallLifetimeEnd`, which starts a new landing pad for
   the enclosing cleanup scope and splits one call-site row into two.

## 2. `Radar::Radar` (7 B, body byte-exact): the LP swap is scope order

`Radar::Radar` is the cleanest witness because `bodyraw == 0`: the two
binaries have byte-identical `.text`, so the only difference is the LSDA.

| row | orig | ours | call(s) in range | IR unwind dest |
|---:|---|---|---|---|
| 1 | `(68,310,2462)` | `(68,310,2464)` | `MatrixC2`, `Image2DCreate`, `GetImage2DWidth`, `GetImage2DHeight`, `new` | `%175` |
| 3 | `(388,14,2462)` | `(388,14,2464)` | `ArraySetLength`, `new` | `%175` |
| 11 | `(610,64,2462)` | `(610,64,2464)` | `inAlienOrbit`, `dlc1Won`, `inEmptyOrbit`, `getLandmarks`, `String::operator`, `Set` | `%175` |
| 13 | `(710,6,2462)` | `(710,6,2464)` | `new` (labelStrings[1]) | `%175` |
| 15 | `(736,6,2462)` | `(736,6,2464)` | `new` (labelStrings[2]) | `%175` |
| 19 | `(892,6,2464)` | `(892,6,2462)` | `getEnemyGuns` | `%372` |
| 21 | `(998,624,2464)` | `(998,624,2462)` | 39 `Image2DCreate`/`GetImage2D*` | `%372` |

LP `2462` = `0x99e` = `0x12de22`, LP `2464` = `0x9a0` = `0x12de24`. Both are
`b.n 0x12de28`, and `0x12de28` is `mov r5,r0; ldr r0,[sp,#16]; blx
StringD1Ev; mov r0,r5; blx _Unwind_Resume` (destroy the member `String` at
`this+0x18c`). So the two landing pads are **identical trampolines to the same
cleanup**; only the row->trampoline assignment is swapped.

The IR shows why there are two: every top-level cleanup scope gets its own
`landingpad` block that branches to the common cleanup `%786`
(`%175` for the early `labelStrings` block, `%300`/`%304` for the `playerGuns`
block, `%372`/`%376` for the `enemyGuns` block, `%638` for the
`Image2DCreate` block). All of these are `cleanup; br %786`. The backend emits
one `b.n` trampoline per block; because the trampolines are identical, the
*order* of the blocks is invisible in `.text` and only the LSDA records it.

### 2.1 Scope-structure probes (all keep `bodyraw == 0`)

Base = landed `Radar.cpp`, extab diff **7**:

| variant | extab diff | rows |
|---|---:|---|
| base | **7** | 39/39 |
| remove `{...}` around `labelStrings->data()[0]` | 7 | 39/39 |
| add `{...}` around `labelStrings->data()[1..3]` | 7 | 39/39 |
| add `{...}` around the whole `Image2DCreate` block | 7 | 39/39 |
| add `{...}` around the `playerGuns` block | 7 | 39/39 |
| add `{...}` around only the `if (enemyGuns != nullptr)` | 7 | 39/39 |
| add `{...}` around only the `for` loop | 7 | 39/39 |
| `if (enemyGuns != nullptr)` without braces | 7 | 39/39 |
| **add `{...}` around the whole `enemyGuns` block (decl+if)** | **13** | 39/39 |
| nest `enemyGuns` inside the `playerGuns` scope | 28 | 39/39 |
| one `{...}` around both gun blocks | 30 | 39/39 |
| move the `inAlienOrbit` block after `labelStrings[1..3]` | 102 | 39/**40** |

Conclusion: the LP order is controlled by the **cleanup-scope nesting around
the `enemyGuns` block**, but no tried spelling beats the landed shape. The
original's scope structure is *not* the landed one (the swap proves it), yet
the swap is a pure label tie: both trampolines are identical, so the only
observable is the 7 LSDA bytes. This is a **METRIC-ONLY** target: a fix would
have to reproduce the original's scope nesting without changing `.text`, and
the one scope change that does move the LP order (wrapping `enemyGuns`) makes
the diff worse.

## 3. `ModStation::ModStation` (2 B): pure RA artifact

Row 3: orig `(390,30,922)` vs ours `(398,22,922)`; both end at 420. The call
is `MatrixSetRotation` at `0xd5470` (offset `0x1a0`). The row starts at the
first instruction of the invoke's protected region:

```
orig 0xd5456: f850 3028   ldr.w r3, [r0, r8, lsl #2]   ; 4th arg loaded into r3
ours 0xd545a: f850 4028   ldr.w r4, [r0, r8, lsl #2]   ; loaded into r4
ours 0xd546c: 4623        mov   r3, r4                ; then moved into r3
```

So the 2 bytes are the 8-byte earlier start of the invoke range because the
original loads the argument directly into `r3` and ours loads into `r4` then
moves. This is a register-allocation difference, not an EH construct. Probes
(local `farYaw`, local `zero`, swapped `yaw`/table args) all leave the row at
`(398,22,922)` and the extab at 2 bytes.

## 4. The two VarDecl splits (confirmation at wave 146)

* **`HangarWindow::render` rows 125/126**: orig `(6824,210,9654)`; ours
  `(6826,96,9654)` + `(7004,30,9600)`. The free-credits `int iconX`/`int
  iconY` declarations sit between the `DrawImage2D(tabIcons[count])` and
  `DrawImage2D(scrollHintImageA)` invokes. `EmitAutoVarAlloca` emits a
  `CallLifetimeEnd`, which starts a new landing pad for the enclosing cleanup
  scope. The second LP (`9600`) is a `b.n` trampoline to the same cleanup
  (`9654`), so it is a pure label split.
* **`Hud::draw` rows 72/73**: orig `(8292,4,9266)` + `(8416,204,9284)`; ours
  `(8420,42,9278)` + `(8496,126,9284)`. The `float filled = progress * halfW`
  VarDecl between `DrawImage2D` and `DrawRegion2D` splits the merged
  `SetColor..DrawString` row. (Wave 145 w9015 showed the merge costs +36 B of
  body, so this is blocked on the body RA, not on the EH source shape.)

## 5. Full table

`O:` = original call targets in the row range, `U:` = ours when different.
`+N` means N further calls elided. Rows with identical `(start,len,lp)` are
omitted. `SAME` rows are not printed.

```
====================================================================================================
fn 0x971b8  Generator::createMission   reclen 172  diffbytes 24  bodyraw 445  rows 26/26  lp-deltas {-8: 1, -2: 2}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  10  (1272,4,1912)                (1272,4,1910)                LP-REMAP   O:<_ZN6Status8getLevelEv@plt>
  11  (1320,20,1914)               (1318,20,1914)               SHIFT      O:<_ZN6Status11getStandingEv@plt>,<_ZN5Agent7getRaceEv@plt>,<_ZN8Standing15getMissionBonusEi@plt>
  12  (1340,100,0)                 (1338,98,0)                  SHIFT      O:<__aeabi_idivmod@plt>,<__aeabi_idivmod@plt>,<__aeabi_idivmod@plt>,<__aeabi_idivmod@plt>
  13  (1440,8,1910)                (1436,8,1908)                SHIFT      O:<_Znwj@plt>
  14  (1448,10,1906)               (1444,10,1904)               SHIFT      O:<_ZN11AbyssEngine6StringC1ERKS0_b@plt>
  15  (1458,42,1916)               (1454,44,1916)               SHIFT      O:<_ZN5Agent13getImagePartsEv@plt>,<_ZN7MissionC1EiN11AbyssEngine6StringEPiiiii@plt>
  16  (1500,24,0)                  (1498,24,0)                  SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<__aeabi_idiv@plt>
  17  (1524,78,1932)               (1522,78,1912)               SHIFT      O:<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_ZN7Globals13getRandomNameEib@plt>
  18  (1602,8,1882)                (1600,8,1880)                SHIFT      O:<_ZN7Mission13setTargetNameEN11AbyssEngine6StringE@plt>
  19  (1610,34,0)                  (1608,34,0)                  SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<__aeabi_idivmod@plt>,<__aeabi_idivmod@plt>
  20  (1644,30,1932)               (1642,30,1932)               SHIFT      O:<_ZN7Mission8setCostsEi@plt>,<_ZN7Mission18setProductionGoodsEii@plt>,<_ZN7Mission8setBonusEi@plt>
  21  (1686,6,1940)                (1684,6,1940)                SHIFT      O:<_ZN11SolarSystem17stationIsInSystemEi@plt>
  22  (1724,4,1880)                (1722,4,1936)                SHIFT      O:<_ZN4Item11getMaxPriceEv@plt>
  23  (1758,6,1904)                (1758,6,1896)                LP-REMAP   O:<_ZN11SolarSystem7getNameEv@plt>
  24  (1764,8,1888)                (1764,8,1886)                LP-REMAP   O:<_ZN7Mission19setTargetSystemNameERKN11AbyssEngine6StringE@plt>
  summary: {'LP-REMAP': 3, 'SHIFT': 12}
====================================================================================================
fn 0xaffd8  Level::createMission   reclen 956  diffbytes 158  bodyraw 5592  rows 148/148  lp-deltas {-18: 5, -10: 16, -6: 34}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  1   (228,4,17100)                (228,4,17094)                LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  3   (860,8,17060)                (860,8,17054)                LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  5   (882,4,17058)                (882,4,17052)                LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  7   (1108,12,17056)              (1108,12,17050)              LP-REMAP   O:<_ZN9ObjectiveC1EiiP5Level@plt>
  9   (1136,12,17054)              (1136,12,17048)              LP-REMAP   O:<_ZN9ObjectiveC1EiiP5Level@plt>
  11  (1326,8,17098)               (1326,8,17092)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  13  (2836,10,17092)              (2836,10,17086)              LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  15  (2872,4,17090)               (2872,4,17084)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  17  (2970,8,17088)               (2970,8,17082)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  19  (3982,4,17076)               (3982,4,17070)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  21  (4234,20,17080)              (4234,20,17074)              LP-REMAP   O:<_ZN8GameText7getTextEi@plt>,<_ZN11AbyssEngine6StringC1ERKS0_b@plt>
  22  (4258,22,17066)              (4258,22,17060)              LP-REMAP   O:<_ZN7MissionC1EiN11AbyssEngine6StringEPiiiii@plt>
  24  (4318,14,17064)              (4318,14,17058)              LP-REMAP   O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  26  (4378,8,16980)               (4378,8,16974)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  28  (4400,4,16978)               (4400,4,16972)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  30  (4652,8,16972)               (4652,8,16966)               LP-REMAP   O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  32  (4780,8,16970)               (4780,8,16964)               LP-REMAP   O:<_ZN8WaypointC1EiiiP5Route@plt>
  34  (4996,4,16968)               (4996,4,16962)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  36  (5040,16,16962)              (5040,16,16956)              LP-REMAP   O:<_ZN5RouteC1EPiP5ArrayIP8KIPlayerES0_i@plt>
  37  (5056,250,0)                 (5056,252,0)                 SHIFT      O:<_ZN5Route7setLoopEb@plt>,<_ZN5Route11getWaypointEi@plt>,<_ZN5Level10createShipEiiiP8Waypointbb@plt>,<_ZN5Route5cloneEv@plt>,+7
  38  (5306,10,16960)              (5308,10,16954)              SHIFT      O:<_ZN9ObjectiveC1EiiP5Level@plt>
  39  (5316,98,0)                  (5318,98,0)                  SHIFT      O:r6,r6,r6,<_Znwj@plt>
  40  (5414,8,17052)               (5416,8,17042)               SHIFT      O:<_ZN5RouteC1EPii@plt>
  41  (5422,106,0)                 (5424,106,0)                 SHIFT      O:<_ZN6Status10getMissionEv@plt>,<_ZN7Mission13getDifficultyEv@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_Znwj@plt>
  42  (5528,4,17050)               (5530,4,17040)               SHIFT      O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  43  (5532,334,0)                 (5534,334,0)                 SHIFT      O:<_Z14ArraySetLengthIP8KIPlayerEvjR5ArrayIT_E@plt>,<_ZN7Globals21getRandomEnemyFighterEi@plt>,<_ZN5Level10createShipEiiiP8Waypointbb@plt>,<_ZN6Player14setAlwaysEnemyEb@plt>,+7
  44  (5866,10,17048)              (5868,10,17038)              SHIFT      O:<_ZN9ObjectiveC1EiiP5Level@plt>
  45  (5876,94,0)                  (5878,94,0)                  SHIFT      O:<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_Znwj@plt>
  46  (5970,8,17046)               (5972,8,17036)               SHIFT      O:<_ZN5RouteC1EPii@plt>
  47  (5978,56,0)                  (5980,56,0)                  SHIFT      O:<_ZN7Mission23getProductionGoodAmountEv@plt>,<_ZN7Mission13getDifficultyEv@plt>,<_Znwj@plt>
  48  (6034,4,17044)               (6036,4,17034)               SHIFT      O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  49  (6038,556,0)                 (6040,552,0)                 SHIFT      O:<_Z14ArraySetLengthIP8KIPlayerEvjR5ArrayIT_E@plt>,<_ZN7Globals21getRandomEnemyFighterEi@plt>,<_ZN5Level10createShipEiiiP8Waypointbb@plt>,r6,+19
  50  (6594,12,17042)              (6592,12,17032)              SHIFT      O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  51  (6606,10,0)                  (6604,10,0)                  SHIFT      O:<_Znwj@plt>
  52  (6616,12,17040)              (6614,12,17030)              SHIFT      O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  53  (6628,168,0)                 (6626,170,0)                 SHIFT      O:<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_ZN5Level11createRouteEi@plt>,r5,+4
  54  (6796,8,17038)               (6796,8,17028)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  56  (6818,4,17036)               (6818,4,17026)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  58  (6968,10,17034)              (6968,10,17024)              LP-REMAP   O:<_ZN9ObjectiveC1EiiP5Level@plt>
  60  (7056,8,17028)               (7056,8,17018)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  62  (7120,4,17026)               (7120,4,17016)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  64  (7326,12,17024)              (7326,12,17014)              LP-REMAP   O:<_ZN9ObjectiveC1EiiP5Level@plt>
  66  (7414,8,17022)               (7414,8,17012)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  68  (7518,4,17020)               (7518,4,17010)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  70  (7770,8,17018)               (7770,8,17008)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  72  (7902,4,17016)               (7902,4,17006)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  74  (8468,10,17014)              (8468,10,17004)              LP-REMAP   O:<_ZN9ObjectiveC1EiiP5Level@plt>
  76  (8592,4,17012)               (8592,4,16994)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  78  (8920,8,17010)               (8920,8,16992)               LP-REMAP   O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  80  (9062,16,17008)              (9062,16,16990)              LP-REMAP   O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  82  (9088,12,17006)              (9088,12,16988)              LP-REMAP   O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  84  (9180,8,17004)               (9180,8,16986)               LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  86  (9320,4,16994)               (9320,4,16984)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  88  (9702,4,16992)               (9702,4,16982)               LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  90  (9740,16,16990)              (9740,16,16980)              LP-REMAP   O:<_ZN5RouteC1EPiP5ArrayIP8KIPlayerES0_i@plt>
  92  (9970,32,17078)              (9970,32,17072)              LP-REMAP   O:<_ZN7Mission23getProductionGoodAmountEv@plt>,<_ZN7Mission22getProductionGoodIndexEv@plt>,<_ZN9ObjectiveC1EiiiP5Level@plt>
  94  (10098,8,16958)              (10098,8,16952)              LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  96  (10164,4,16956)              (10164,4,16950)              LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  98  (10192,16,16954)             (10192,16,16948)             LP-REMAP   O:<_ZN8WaypointC1EiiiP5Route@plt>
  100 (10298,8,16948)              (10298,8,16942)              LP-REMAP   O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  102 (10322,18,16946)             (10322,18,16940)             LP-REMAP   O:<_ZN8WaypointC1EiiiP5Route@plt>
  104 (10420,8,16940)              (10420,8,16934)              LP-REMAP   O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  106 (10470,4,16938)              (10470,4,16932)              LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  108 (10514,16,16936)             (10514,16,16930)             LP-REMAP   O:<_ZN5RouteC1EPiP5ArrayIP8KIPlayerES0_i@plt>
  110 (10722,32,17062)             (10722,32,17056)             LP-REMAP   O:<_ZN7Mission23getProductionGoodAmountEv@plt>,<_ZN7Mission22getProductionGoodIndexEv@plt>,<_ZN9ObjectiveC1EiiiP5Level@plt>
  112 (10864,4,17082)              (10864,4,17076)              LP-REMAP   O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  114 (11284,8,17124)              (11284,8,17118)              LP-REMAP   O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  116 (11344,4,17118)              (11344,4,17112)              LP-REMAP   O:<_ZN5ArrayIiEC2Ev@plt>
  118 (11702,8,17134)              (11702,8,17128)              LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  119 (11710,240,0)                (11710,232,0)                SHIFT      O:<_ZN8KIPlayer8setRouteEP5Route@plt>,<_ZN8KIPlayer9setJumperEb@plt>,<_end@@Base+0x156ad4>,r5,+3
  120 (11950,20,17114)             (11942,20,17108)             SHIFT      O:<_ZN8WaypointC1EiiiP5Route@plt>
  121 (11970,264,0)                (11962,258,0)                SHIFT      O:<_ZN5Level18createStaticObjectEP8Waypointib@plt>,<_ZN8KIPlayer14setSpacePointsEP5ArrayIP10SpacePointE@plt>,r2,<_ZN17PlayerFixedObject9setMovingEb@plt>,+7
  122 (12234,20,17112)             (12220,20,17106)             SHIFT      O:<_ZN8WaypointC1EiiiP5Route@plt>
  123 (12254,132,0)                (12240,132,0)                SHIFT      O:<_ZN5Level18createStaticObjectEP8Waypointib@plt>,<_ZN8KIPlayer14setSpacePointsEP5ArrayIP10SpacePointE@plt>,r2,<_ZN17PlayerFixedObject9setMovingEb@plt>,+3
  124 (12386,8,17106)              (12372,8,17100)              SHIFT      O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  125 (12394,1768,0)               (12380,1782,0)               ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN5ArrayIiED2Ev@plt>,<_ZdlPv@plt>,<_ZN5Level10createShipEiiiP8Waypointbb@plt>,+54 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN5ArrayIiED2Ev@plt>,<_ZdlPv@plt>,<_ZN5Level10createShipEiiiP8Waypointbb@plt>,+54
  126 (14162,12,16988)             (14162,12,16978)             LP-REMAP   O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  128 (14184,12,16986)             (14184,12,16976)             LP-REMAP   O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  130 (15170,8,17116)              (15170,8,17110)              LP-REMAP   O:<_ZN5RouteC1EPii@plt>
  131 (15178,674,0)                (15178,678,0)                SHIFT      O:<_ZN5Route11getWaypointEv@plt>,<_ZN5Level18createStaticObjectEP8Waypointib@plt>,<_ZN5Route11getWaypointEv@plt>,r2,+23
  132 (15852,8,16934)              (15856,8,16926)              SHIFT      O:<_ZN5RouteC1EPii@plt>
  133 (15860,190,0)                (15864,194,0)                SHIFT      O:<_ZN5Route11getWaypointEv@plt>,<_ZN5Level10createShipEiiiP8Waypointbb@plt>,<_ZN6Player14setAlwaysEnemyEb@plt>,<_ZN5RouteD1Ev@plt>,+2
  134 (16050,16,16932)             (16058,16,16924)             SHIFT      O:<_ZN8WaypointC1EiiiP5Route@plt>
  135 (16066,152,0)                (16074,152,0)                SHIFT      O:<_ZN5Level18createStaticObjectEP8Waypointib@plt>,<_ZN17PlayerFixedObject14setDockingTypeEi@plt>,<_ZN17PlayerFixedObject9setMovingEb@plt>,r2,+3
  136 (16218,8,16926)              (16226,8,16918)              SHIFT      O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  137 (16226,260,0)                (16234,260,0)                SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN6Status10getStationEv@plt>,<_ZN7Station23getHiddenBlueprintIndexEv@plt>,<_ZN5Level10createShipEiiiP8Waypointbb@plt>,+11
  138 (16486,4,16920)              (16494,4,16914)              SHIFT      O:<_ZN9GeneratorC1Ev@plt>
  139 (16490,30,0)                 (16498,30,0)                 SHIFT      O:<_ZN9Generator11getLootListEii@plt>,<_ZN9GeneratorD1Ev@plt>,<_ZdlPv@plt>,<_Znwj@plt>
  140 (16520,4,16922)              (16528,4,16916)              SHIFT      O:<_ZN8FileReadC1Ev@plt>
  141 (16524,130,0)                (16532,124,0)                SHIFT      O:<_ZN8FileRead15loadSpacePointsEii@plt>,<_ZN8FileReadD1Ev@plt>,<_ZdlPv@plt>,<_ZN8KIPlayer14setSpacePointsEP5ArrayIP10SpacePointE@plt>,+3
  142 (16654,8,16924)              (16656,8,16928)              SHIFT      O:<_ZN5RouteC1EPii@plt>
  143 (16662,90,0)                 (16664,90,0)                 SHIFT      O:<_ZN6Status10getMissionEv@plt>,<_ZN7Mission13getDifficultyEv@plt>,<_Znwj@plt>
  144 (16752,4,16984)              (16754,4,17046)              SHIFT      O:<_ZN5ArrayIP8KIPlayerEC2Ev@plt>
  145 (16756,142,0)                (16758,134,0)                SHIFT      O:<_Z14ArraySetLengthIP8KIPlayerEvjR5ArrayIT_E@plt>,<_ZN7Globals21getRandomEnemyFighterEi@plt>,<_ZN5Route6lengthEv@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,+4
  146 (16898,14,16982)             (16892,14,17044)             SHIFT      O:<_ZN9ObjectiveC1EiiiP5Level@plt>
  147 (16912,260,0)                (16906,266,0)                SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZdlPv@plt>,<_Unwind_Resume@plt>
  summary: {'LP-REMAP': 55, 'SHIFT': 40, 'ROW': 1}
====================================================================================================
fn 0xc0214  Level::createShip   reclen 336  diffbytes 93  bodyraw 1978  rows 57/57  lp-deltas {-2: 1, 6: 3}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  2   (436,94,0)                   (436,98,0)                   SHIFT      O:<_ZN6Player10setEmpDataEii@plt>,<_Znwj@plt>
  3   (530,32,2482)                (534,28,2480)                SHIFT      O:<_ZN17PlayerFixedObjectC1EiiP6PlayerP10AEGeometryfff@plt>
  5   (570,4,2476)                 (570,4,2474)                 LP-REMAP   O:<_ZN5ArrayIP14BoundingVolumeEC2Ev@plt>
  6   (574,78,0)                   (574,82,0)                   SHIFT      O:<_Z14ArraySetLengthIP14BoundingVolumeEvjR5ArrayIT_E@plt>,<_Znwj@plt>
  7   (652,30,2448)                (656,34,2452)                SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  8   (682,20,0)                   (690,20,0)                   SHIFT      O:<_Znwj@plt>
  9   (702,28,2446)                (710,28,2450)                SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  10  (730,20,0)                   (738,20,0)                   SHIFT      O:<_Znwj@plt>
  11  (750,30,2444)                (758,34,2448)                SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  12  (780,20,0)                   (792,20,0)                   SHIFT      O:<_Znwj@plt>
  13  (800,28,2442)                (812,28,2446)                SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  14  (828,20,0)                   (840,20,0)                   SHIFT      O:<_Znwj@plt>
  15  (848,30,2440)                (860,34,2444)                SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  16  (878,20,0)                   (894,20,0)                   SHIFT      O:<_Znwj@plt>
  17  (898,28,2438)                (914,28,2442)                SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  18  (926,20,0)                   (942,20,0)                   SHIFT      O:<_Znwj@plt>
  19  (946,30,2436)                (962,34,2440)                SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  20  (976,20,0)                   (996,20,0)                   SHIFT      O:<_Znwj@plt>
  21  (996,30,2434)                (1016,28,2438)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  22  (1026,18,0)                  (1044,18,0)                  SHIFT      O:<_Znwj@plt>
  23  (1044,30,2432)               (1062,34,2436)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  24  (1074,20,0)                  (1096,20,0)                  SHIFT      O:<_Znwj@plt>
  25  (1094,28,2430)               (1116,28,2434)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  26  (1122,14,0)                  (1144,14,0)                  SHIFT      O:<_Znwj@plt>
  27  (1136,36,2428)               (1158,36,2426)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  28  (1172,52,0)                  (1194,46,0)                  ROW        O:<_Znwj@plt> U:<_ZN17PlayerFixedObject16setWreckedMeshIdEi@plt>,<_Znwj@plt>
  29  (1224,32,2470)               (1240,34,2482)               SHIFT      O:<_ZN13PlayerFighterC1EiiP6PlayerP10AEGeometryfffb@plt>
  30  (1256,354,0)                 (1274,348,0)                 ROW        O:<_ZN7Globals12getShipGroupEiib@plt>,r5,<_ZN10LODManager9addObjectEP10AEGeometry@plt>,<_ZN5ArrayIiED2Ev@plt>,+3 U:<_ZN7Globals12getShipGroupEiib@plt>,r6,<_ZN5ArrayIiED2Ev@plt>,<_ZdlPv@plt>,+3
  31  (1610,30,2468)               (1622,30,2472)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  32  (1640,20,0)                  (1652,20,0)                  SHIFT      O:<_Znwj@plt>
  33  (1660,28,2466)               (1672,28,2470)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  34  (1688,20,0)                  (1700,20,0)                  SHIFT      O:<_Znwj@plt>
  35  (1708,30,2464)               (1720,30,2468)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  36  (1738,20,0)                  (1750,20,0)                  SHIFT      O:<_Znwj@plt>
  37  (1758,28,2462)               (1770,28,2466)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  38  (1786,20,0)                  (1798,20,0)                  SHIFT      O:<_Znwj@plt>
  39  (1806,30,2460)               (1818,30,2464)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  40  (1836,48,0)                  (1848,46,0)                  SHIFT      O:<_Z14ArraySetLengthIP14BoundingVolumeEvjR5ArrayIT_E@plt>,<_Znwj@plt>
  41  (1884,30,2458)               (1894,30,2462)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  42  (1914,20,0)                  (1924,20,0)                  SHIFT      O:<_Znwj@plt>
  43  (1934,28,2456)               (1944,28,2460)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  44  (1962,20,0)                  (1972,20,0)                  SHIFT      O:<_Znwj@plt>
  45  (1982,28,2454)               (1992,28,2458)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  46  (2010,46,0)                  (2020,44,0)                  SHIFT      O:<_Z14ArraySetLengthIP14BoundingVolumeEvjR5ArrayIT_E@plt>,<_Znwj@plt>
  47  (2056,30,2452)               (2064,30,2456)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  48  (2086,20,0)                  (2094,20,0)                  SHIFT      O:<_Znwj@plt>
  49  (2106,28,2450)               (2114,28,2454)               SHIFT      O:<_ZN11BoundingAABC1Efffffffff@plt>
  50  (2134,54,0)                  (2142,46,0)                  ROW        O:<_ZN17PlayerFixedObject16setWreckedMeshIdEi@plt>,<_Z14ArraySetLengthIP14BoundingVolumeEvjR5ArrayIT_E@plt>,<_Znwj@plt> U:<_Z14ArraySetLengthIP14BoundingVolumeEvjR5ArrayIT_E@plt>,<_Znwj@plt>
  51  (2188,30,2426)               (2188,30,2432)               LP-REMAP   O:<_ZN11BoundingAABC1Efffffffff@plt>
  53  (2238,28,2424)               (2238,28,2430)               LP-REMAP   O:<_ZN11BoundingAABC1Efffffffff@plt>
  55  (2286,30,2422)               (2286,30,2428)               LP-REMAP   O:<_ZN11BoundingAABC1Efffffffff@plt>
  summary: {'SHIFT': 44, 'LP-REMAP': 4, 'ROW': 3}
====================================================================================================
fn 0xd52d0  ModStation::ModStation   reclen 112  diffbytes 2  bodyraw 11  rows 15/15  lp-deltas {}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  3   (390,30,922)                 (398,22,922)                 SHIFT      O:<_ZN11AbyssEngine6AEMath17MatrixSetRotationERNS0_6MatrixEfffNS0_13RotationOrderE@plt>
  summary: {'SHIFT': 1}
====================================================================================================
fn 0x12d484  Radar::Radar   reclen 256  diffbytes 7  bodyraw 0  rows 39/39  lp-deltas {-2: 2, 2: 5}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  1   (68,310,2462)                (68,310,2464)                LP-REMAP   O:<_ZN11AbyssEngine6AEMath6MatrixC2Ev@plt>,<_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj@plt>,<_ZN11AbyssEngine11PaintCanvas15GetImage2DWidthEj@plt>,<_ZN11AbyssEngine11PaintCanvas16GetImage2DHeightEj@plt>,+1
  3   (388,14,2462)                (388,14,2464)                LP-REMAP   O:<_Z14ArraySetLengthIPN11AbyssEngine6StringEEvjR5ArrayIT_E@plt>,<_Znwj@plt>
  11  (610,64,2462)                (610,64,2464)                LP-REMAP   O:<_ZN6Status12inAlienOrbitEv@plt>,<_ZN6Status7dlc1WonEv@plt>,<_ZN6Status12inEmptyOrbitEv@plt>,<_ZN5Level12getLandmarksEv@plt>,+3
  13  (710,6,2462)                 (710,6,2464)                 LP-REMAP   O:<_Znwj@plt>
  15  (736,6,2462)                 (736,6,2464)                 LP-REMAP   O:<_Znwj@plt>
  19  (892,6,2464)                 (892,6,2462)                 LP-REMAP   O:<_ZN5Level12getEnemyGunsEv@plt>
  21  (998,624,2464)               (998,624,2462)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj@plt>,<_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj@plt>,<_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj@plt>,<_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj@plt>,+35
  summary: {'LP-REMAP': 7}
====================================================================================================
fn 0x12e050  Radar::draw   reclen 132  diffbytes 86  bodyraw 8373  rows 19/19  lp-deltas {-2: 1}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  1   (568,8,9818)                 (568,8,9816)                 LP-REMAP   O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  2   (576,1818,0)                 (576,1838,0)                 ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,+66 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,+67
  3   (2394,10,9856)               (2414,10,9854)               SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  4   (2404,158,0)                 (2424,84,0)                  ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,<_ZN8GameText7getTextEi@plt>,+1 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN8GameText7getTextEi@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  5   (2562,10,9852)               (2508,10,9850)               SHIFT      O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  6   (2576,16,9858)               (2522,16,9856)               SHIFT      O:<_ZN6Status10getStationEv@plt>,<_ZN7Station11getTecLevelEv@plt>,<_ZN11AbyssEngine6StringC2Ei@plt>
  7   (2592,12,9842)               (2538,12,9840)               SHIFT      O:<_ZN11AbyssEngine6StringC1ERKS0_b@plt>
  8   (2604,16,9832)               (2550,16,9830)               SHIFT      O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  9   (2638,20,9822)               (2584,20,9820)               SHIFT      O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  10  (2658,78,0)                  (2604,78,0)                  SHIFT      O:r5,r5,r5,r5,+2
  11  (2736,14,9820)               (2682,10,9818)               SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  12  (2750,1256,0)                (2692,1294,0)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN8KIPlayer6isDeadEv@plt>,<_ZN5Level9getPlayerEv@plt>,+28 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN8KIPlayer6isDeadEv@plt>,<_ZN5Level9getPlayerEv@plt>,+29
  13  (4006,8,9872)                (3986,8,9874)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  14  (4014,1796,0)                (3994,1748,0)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN5Level9getPlayerEv@plt>,<_ZN9PlayerEgo11isAutoPilotEv@plt>,+51 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN5Level9getPlayerEv@plt>,<_ZN9PlayerEgo11isAutoPilotEv@plt>,+48
  15  (5810,8,9870)                (5742,8,9868)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  16  (5818,788,0)                 (5750,858,0)                 ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN5Level9getPlayerEv@plt>,<_ZN9PlayerEgo11isAutoPilotEv@plt>,+22 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN5Level9getPlayerEv@plt>,<_ZN9PlayerEgo11isAutoPilotEv@plt>,+25
  17  (6606,8,9816)                (6608,8,9814)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  18  (6614,3294,0)                (6616,3292,0)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN8KIPlayer6isDeadEv@plt>,<_ZN8KIPlayer7isDyingEv@plt>,+107 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,<_ZN8KIPlayer6isDeadEv@plt>,<_ZN8KIPlayer7isDyingEv@plt>,+108
  summary: {'LP-REMAP': 1, 'ROW': 6, 'SHIFT': 11}
====================================================================================================
fn 0x137d18  LevelScript::process   reclen 348  diffbytes 150  bodyraw 54895  rows 46/46  lp-deltas {}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  0   (0,5832,0)                   (0,5674,0)                   ROW        O:<_ZN5Level11getMessagesEv@plt>,<_ZN5Level9getPlayerEv@plt>,<_ZN5Level10getEnemiesEv@plt>,<_ZN6Status25getCurrentCampaignMissionEv@plt>,+247 U:<_ZN5Level10getEnemiesEv@plt>,<_ZN5Level9getPlayerEv@plt>,<_ZN5Level11getMessagesEv@plt>,<_ZN6Status25getCurrentCampaignMissionEv@plt>,+239
  1   (5832,6,57116)               (5674,6,57094)               SHIFT      O:<_ZN9ExplosionC1Ei@plt>
  2   (5838,748,0)                 (5680,790,0)                 SHIFT      O:<_ZN9Explosion14addFireStreaksEv@plt>,<_ZN9Explosion10setScalingEf@plt>,<_ZN12RadioMessage6isOverEv@plt>,<_ZN9PlayerEgo13setTurretModeEb@plt>,+47
  3   (6586,8,57080)               (6470,8,57080)               SHIFT      O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  4   (6594,1632,0)                (6478,1202,0)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN9PlayerEgo23getHackingGameDockIndexEv@plt>,<_ZN5Level16getDockingTargetEi@plt>,<_ZN17PlayerFixedObject14setDockingTypeEi@plt>,+91 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN9PlayerEgo23getHackingGameDockIndexEv@plt>,<_ZN5Level16getDockingTargetEi@plt>,<_ZN17PlayerFixedObject14setDockingTypeEi@plt>,+72
  5   (8226,8,57106)               (7680,8,57096)               SHIFT      O:<_ZN9ObjectiveC1EiiP5Level@plt>
  6   (8234,664,0)                 (7688,684,0)                 ROW        O:<_ZN10LODManager11forceUpdateEib@plt>,<_ZN12RadioMessage11isTriggeredEv@plt>,<_ZN12RadioMessage11isTriggeredEv@plt>,<_ZN18TargetFollowCamera23setRotationAroundTargetEb@plt>,+31 U:<_ZN10LODManager11forceUpdateEib@plt>,<_ZN12RadioMessage11isTriggeredEv@plt>,<_ZN12RadioMessage11isTriggeredEv@plt>,<_ZN6Status11getStandingEv@plt>,+33
  7   (8898,8,57104)               (8372,8,57106)               SHIFT      O:<_ZN5RouteC1EPii@plt>
  8   (8906,718,0)                 (8380,772,0)                 ROW        O:<_ZN5Level14setPlayerRouteEP5Route@plt>,<_ZN9PlayerEgo8setRouteEP5Route@plt>,<_ZN12RadioMessage11isTriggeredEv@plt>,<_ZN6Status18getCampaignMissionEv@plt>,+45 U:<_ZN5Level14setPlayerRouteEP5Route@plt>,<_ZN9PlayerEgo8setRouteEP5Route@plt>,<_ZN9PlayerEgo8getRouteEv@plt>,<_ZN9PlayerEgo8getRouteEv@plt>,+46
  9   (9624,8,57074)               (9152,8,57072)               SHIFT      O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  10  (9632,310,0)                 (9160,324,0)                 SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN10AEGeometry9isVisibleEv@plt>,<_ZN10AEGeometry11getPositionEv@plt>,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,+8
  11  (9942,6,57114)               (9484,6,57108)               SHIFT      O:<_ZN10AEGeometryC1EPN11AbyssEngine11PaintCanvasE@plt>
  12  (9948,2214,0)                (9490,1790,0)                ROW        O:<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,<_ZN12RadioMessage6isOverEv@plt>,<_ZN9PlayerEgo17isInRocketControlEv@plt>,+145 U:<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,<_ZN12RadioMessage6isOverEv@plt>,<_ZN9PlayerEgo17isInRocketControlEv@plt>,+99
  13  (12162,8,57098)              (11280,8,57098)              SHIFT      O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  14  (12170,2054,0)               (11288,3890,0)               ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,r5,r1,<_ZN8KIPlayer10setVisibleEb@plt>,+104 U:<_ZN11AbyssEngine6StringD1Ev@plt>,r5,r1,<_ZN8KIPlayer10setVisibleEb@plt>,+156
  15  (14224,14,57162)             (15178,10,57162)             SHIFT      O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  16  (14242,8,57152)              (15190,10,57152)             SHIFT      O:<_ZN11AbyssEngine6StringC2Ei@plt>
  17  (14250,12,57142)             (15200,14,57142)             SHIFT      O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  18  (14262,8,57132)              (15214,8,57132)              SHIFT      O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  19  (14270,5718,0)               (15222,7952,0)               ROW        O:r4,r4,r4,r4,+275 U:fp,fp,fp,fp,+429
  20  (19988,6,57086)              (23174,8,57116)              ROW        O:<_ZN10AEGeometryC1EPN11AbyssEngine11PaintCanvasE@plt> U:<_ZN5RouteC1EPii@plt>
  21  (19994,4984,0)               (23182,2248,0)               ROW        O:r2,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN11AbyssEngine6AEMath6VectoraSERKS1_@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,+234 U:<_ZN8KIPlayer8setRouteEP5Route@plt>,<_ZN9PlayerEgo13setTurretModeEb@plt>,<_ZN11LevelScript11resetCameraEP5Level@plt>,<_ZN9PlayerEgo15setFreeLookModeEb@plt>,+123
  22  (24978,8,57058)              (25430,8,57058)              SHIFT      O:<_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  23  (24986,1132,0)               (25438,508,0)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN9PlayerEgo23getHackingGameDockIndexEv@plt>,<_ZN5Level16getDockingTargetEi@plt>,<_ZN9PlayerEgo17deleteHackingGameEv@plt>,+57 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN9PlayerEgo23getHackingGameDockIndexEv@plt>,<_ZN5Level16getDockingTargetEi@plt>,<_ZN9PlayerEgo17deleteHackingGameEv@plt>,+30
  24  (26118,12,57096)             (25946,12,57104)             SHIFT      O:<_ZN10AEGeometryC1EtPN11AbyssEngine11PaintCanvasEb@plt>
  25  (26130,2794,0)               (25958,3552,0)               ROW        O:<_ZN10AEGeometry11getPositionEv@plt>,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN10AEGeometry12getDirectionEv@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,+141 U:<_ZN10AEGeometry11getPositionEv@plt>,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN10AEGeometry12getDirectionEv@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,+190
  26  (28924,8,57108)              (29510,10,57088)             SHIFT      O:<_ZN5RouteC1EPii@plt>
  27  (28932,328,0)                (29520,354,0)                ROW        O:<_ZN8KIPlayer9isWingManEv@plt>,r2,<_ZN6Player10setEnemiesEP5ArrayIPS_E@plt>,<_ZN5Route5cloneEv@plt>,+13 U:<_ZN5Level10getEnemiesEv@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,r6,+12
  28  (29260,8,57064)              (29874,6,57078)              ROW        O:<_ZN5RouteC1EPii@plt> U:<_ZN10AEGeometryC1EPN11AbyssEngine11PaintCanvasE@plt>
  29  (29268,552,0)                (29880,1346,0)               ROW        O:<_ZN8KIPlayer8setRouteEP5Route@plt>,<_ZN9PlayerEgo13setTurretModeEb@plt>,<_ZN11LevelScript11resetCameraEP5Level@plt>,<_ZN9PlayerEgo15setFreeLookModeEb@plt>,+36 U:r2,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN11AbyssEngine6AEMath6VectoraSERKS1_@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,+59
  30  (29820,10,57090)             (31226,8,57110)              SHIFT      O:<_ZN5RouteC1EPii@plt>
  31  (29830,6658,0)               (31234,5192,0)               ROW        O:<_ZN5Level10getEnemiesEv@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,<_ZN11AbyssEngine8AERandom7nextIntEi@plt>,r6,+330 U:<_ZN8KIPlayer9isWingManEv@plt>,r2,<_ZN6Player10setEnemiesEP5ArrayIPS_E@plt>,<_ZN5Route5cloneEv@plt>,+245
  32  (36488,4,57122)              (36426,4,57122)              SHIFT      O:<_ZN5ArrayIP6PlayerEC2Ev@plt>
  33  (36492,2626,0)               (36430,1686,0)               ROW        O:<_Z8ArrayAddIP6PlayerEvT_R5ArrayIS2_E@plt>,<_Z8ArrayAddIP6PlayerEvT_R5ArrayIS2_E@plt>,<_ZN6Player10setEnemiesEP5ArrayIPS_E@plt>,r2,+136 U:<_Z8ArrayAddIP6PlayerEvT_R5ArrayIS2_E@plt>,<_Z8ArrayAddIP6PlayerEvT_R5ArrayIS2_E@plt>,<_ZN6Player10setEnemiesEP5ArrayIPS_E@plt>,r2,+93
  34  (39118,6,57088)              (38116,6,57086)              SHIFT      O:<_ZN10AEGeometryC1EPN11AbyssEngine11PaintCanvasE@plt>
  35  (39124,2500,0)               (38122,3322,0)               ROW        O:<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN11AbyssEngine11PaintCanvas16CameraGetCurrentEv@plt>,<_ZN10AEGeometry9getMatrixEv@plt>,+110 U:<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN11AbyssEngine11PaintCanvas16CameraGetCurrentEv@plt>,<_ZN10AEGeometry9getMatrixEv@plt>,+163
  36  (41624,8,57068)              (41444,8,57066)              SHIFT      O:<_ZN5RouteC1EPii@plt>
  37  (41632,8,0)                  (41452,8,0)                  SHIFT      O:<_Znwj@plt>
  38  (41640,8,57066)              (41460,8,57064)              SHIFT      O:<_ZN5RouteC1EPii@plt>
  39  (41648,5556,0)               (41468,4078,0)               ROW        O:<_ZN8KIPlayer8setRouteEP5Route@plt>,<_ZN8KIPlayer8setRouteEP5Route@plt>,<_ZN13PlayerFighter13setAIDisabledEb@plt>,<_ZN10AEGeometry12getDirectionEv@plt>,+282 U:<_ZN8KIPlayer8setRouteEP5Route@plt>,<_ZN8KIPlayer8setRouteEP5Route@plt>,<_ZN13PlayerFighter13setAIDisabledEb@plt>,<_ZN10AEGeometry12getDirectionEv@plt>,+211
  40  (47204,8,57056)              (45546,8,57056)              SHIFT      O:<_ZN9ObjectiveC1EiiP5Level@plt>
  41  (47212,2470,0)               (45554,3978,0)               ROW        O:<_ZN9PlayerEgo22isDockedToDockingPointEv@plt>,<_ZN9PlayerEgo10hackingWonEv@plt>,<_ZN9ObjectiveD1Ev@plt>,<_ZdlPv@plt>,+114 U:<_ZN9PlayerEgo22isDockedToDockingPointEv@plt>,<_ZN9PlayerEgo10hackingWonEv@plt>,<_ZN9ObjectiveD1Ev@plt>,<_ZdlPv@plt>,+189
  42  (49682,16,57050)             (49532,16,57054)             SHIFT      O:<_ZN10AEGeometryC1EtPN11AbyssEngine11PaintCanvasEb@plt>
  43  (49698,4756,0)               (49548,4882,0)               ROW        O:<_ZN10AEGeometry11getPositionEv@plt>,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN10AEGeometry12getDirectionEv@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,+233 U:<_ZN10AEGeometry11getPositionEv@plt>,<_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>,<_ZN10AEGeometry12getDirectionEv@plt>,<_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>,+237
  44  (54454,8,57048)              (54430,8,57052)              SHIFT      O:<_ZN9ExplosionC1Ei@plt>
  45  (54462,2770,0)               (54438,2794,0)               ROW        O:<_ZN9Explosion14addFireStreaksEv@plt>,<_ZN9Explosion10setScalingEf@plt>,r2,<_ZN11AbyssEngine6AEMathplERKNS0_6VectorES3_@plt>,+112 U:<_ZN9Explosion14addFireStreaksEv@plt>,<_ZN9Explosion10setScalingEf@plt>,r2,<_ZN11AbyssEngine6AEMathplERKNS0_6VectorES3_@plt>,+113
  summary: {'ROW': 21, 'SHIFT': 25}
====================================================================================================
fn 0x1490d8  HangarWindow::render   reclen 948  diffbytes 762  bodyraw 1719  rows 156/156  lp-deltas {-4: 4, -2: 2}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  3   (750,20,9428)                (750,20,9424)                LP-REMAP   O:<_ZN6Layout7drawBoxEiiiiiN11AbyssEngine6StringE@plt>
  5   (806,20,9412)                (806,20,9408)                LP-REMAP   O:<_ZN6Layout7drawBoxEiiiiiN11AbyssEngine6StringE@plt>
  10  (1166,18,9470)               (1166,18,9468)               LP-REMAP   O:<_ZN4Ship8getPriceEv@plt>,<_ZN6Layout13formatCreditsEi@plt>
  11  (1194,34,9438)               (1194,34,9434)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  16  (1406,4,9418)                (1406,4,9414)                LP-REMAP   O:<_ZN4Item16getStationAmountEv@plt>
  17  (1416,4,9588)                (1416,4,9586)                LP-REMAP   O:<_ZN4Item9getAmountEv@plt>
  19  (1530,144,9410)              (1530,228,9446)              MERGE<-2   O:<_ZN9BluePrint17getCompletionRateEv@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjii@plt>,<_ZN11AbyssEngine11PaintCanvas12DrawRegion2DEjiiiifiiii@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,+1
  24  (1808,14,9258)               (1812,14,9258)               SHIFT      O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  25  (1826,10,9248)               (1830,10,9248)               SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  26  (1836,14,9238)               (1840,10,9238)               SHIFT      O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  30  (2120,18,9440)               (2120,18,9436)               LP-REMAP   O:<_ZN4Ship8getPriceEv@plt>,<_ZN6Layout13formatCreditsEi@plt>
  31  (2148,34,9380)               (2148,34,9378)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  37  (2412,30,9454)               (2412,30,9452)               LP-REMAP   O:<_ZN4Ship8getPriceEv@plt>,<_ZN6Status7getShipEv@plt>,<_ZN4Ship8getPriceEv@plt>,<_ZN6Layout13formatCreditsEi@plt>
  39  (2508,150,9664)              (2508,118,9664)              ROW        O:<_ZN4Ship8getIndexEv@plt>,<_ZN12ImageFactory8drawShipEiii@plt>,<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,+2 U:<_ZN4Ship8getIndexEv@plt>,<_ZN12ImageFactory8drawShipEiii@plt>,<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>,+1
  40  (2696,140,9562)              (2654,182,9512)              ROW        O:<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton10setVisibleEb@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton8getWidthEv@plt>,+4 U:<_ZN6Status7getShipEv@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton10setVisibleEb@plt>,<_ZN11TouchButton4drawEv@plt>,+5
  45  (2992,4,9418)                (2992,4,9414)                LP-REMAP   O:<_ZN4Item9getAmountEv@plt>
  46  (3008,154,9588)              (3008,154,9586)              LP-REMAP   O:<_ZN4Item8getIndexEv@plt>,<_ZN9BluePrint16getCurrentAmountEi@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+5
  48  (3406,4,9512)                (3406,4,9510)                LP-REMAP   O:<_ZN4Item16getStationAmountEv@plt>
  49  (3416,4,9600)                (3416,4,9598)                LP-REMAP   O:<_ZN4Item9getAmountEv@plt>
  51  (3644,4,9512)                (3644,4,9510)                LP-REMAP   O:<_ZN4Item9getAmountEv@plt>
  52  (3660,22,9600)               (3660,22,9598)               LP-REMAP   O:<_ZN4Item8getIndexEv@plt>,<_ZN9BluePrint16getCurrentAmountEi@plt>,<_ZN4Item18getBlueprintAmountEv@plt>
  53  (3686,12,9510)               (3686,12,9508)               LP-REMAP   O:<_ZN11AbyssEngine6StringC2Ei@plt>
  54  (3704,8,9504)                (3704,8,9502)                LP-REMAP   O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  55  (3712,10,9494)               (3712,10,9492)               LP-REMAP   O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  56  (3740,8,9452)                (3740,8,9450)                LP-REMAP   O:<_ZN11AbyssEngine6StringC2Ei@plt>
  57  (3756,8,9450)                (3756,8,9448)                LP-REMAP   O:<_ZN11AbyssEngine6StringC2Ei@plt>
  58  (3776,12,9446)               (3776,12,9442)               LP-REMAP   O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  59  (3788,10,9442)               (3788,10,9438)               LP-REMAP   O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  60  (3806,20,9514)               (3806,20,9526)               LP-REMAP   O:<_ZN4Item8getIndexEv@plt>,<_ZN9BluePrint14getTotalAmountEi@plt>,<_ZN11AbyssEngine6StringC2Ei@plt>
  61  (3826,10,9434)               (3826,10,9430)               LP-REMAP   O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  62  (3850,8,9472)                (3850,8,9470)                LP-REMAP   O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  63  (3858,10,9460)               (3858,10,9458)               LP-REMAP   O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  65  (3912,98,9590)               (3912,98,9588)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>,<_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringE@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  66  (4086,58,9456)               (4086,58,9454)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringE@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  68  (4248,18,9458)               (4248,18,9456)               LP-REMAP   O:<_ZN4Item14getSinglePriceEv@plt>,<_ZN6Layout13formatCreditsEi@plt>
  69  (4276,34,9420)               (4276,34,9416)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  71  (4512,12,9408)               (4512,12,9406)               LP-REMAP   O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  72  (4528,16,9422)               (4528,16,9418)               LP-REMAP   O:<_ZN4Item9getAmountEv@plt>,<_ZN11AbyssEngine6StringC2Ei@plt>
  73  (4544,12,9370)               (4544,12,9368)               LP-REMAP   O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  74  (4560,10,9360)               (4560,10,9358)               LP-REMAP   O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  80  (5074,18,9382)               (5074,18,9380)               LP-REMAP   O:<_ZN8GameText7getTextEi@plt>,<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  101 (6306,8,9586)                (6310,8,9584)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  102 (6314,8,9574)                (6318,8,9572)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  103 (6336,8,9560)                (6340,8,9560)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  104 (6344,8,9542)                (6348,8,9524)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  105 (6354,10,9584)               (6358,10,9582)               SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  106 (6364,8,9572)                (6368,8,9570)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  107 (6378,8,9558)                (6382,8,9558)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  108 (6386,8,9540)                (6390,8,9522)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  109 (6404,8,9582)                (6408,8,9580)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  110 (6412,8,9570)                (6416,8,9568)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  111 (6434,8,9556)                (6438,8,9556)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  112 (6442,8,9538)                (6446,8,9520)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  113 (6460,8,9580)                (6464,8,9578)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  114 (6468,8,9568)                (6472,8,9566)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  115 (6490,8,9554)                (6494,8,9554)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  116 (6498,8,9536)                (6502,8,9518)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  117 (6516,8,9578)                (6520,8,9576)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  118 (6524,8,9566)                (6528,8,9564)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  119 (6546,8,9552)                (6550,8,9552)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  120 (6554,8,9534)                (6558,8,9516)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  121 (6632,8,9576)                (6636,8,9574)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  122 (6640,8,9564)                (6644,8,9562)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  123 (6662,8,9550)                (6666,8,9550)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  124 (6670,8,9532)                (6674,8,9514)                SHIFT      O:<_ZN11AbyssEngine6StringaSERKS0_@plt>
  125 (6678,146,0)                 (6682,144,0)                 SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<__aeabi_uidiv@plt>
  126 (6824,210,9654)              (6826,96,9654)               ROW        O:<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton19replaceTextKeepSizeERKN11AbyssEngine6StringE@plt>,<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt>,<_ZN11TouchButton4drawEv@plt>,+2 U:<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton19replaceTextKeepSizeERKN11AbyssEngine6StringE@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt>,+1
  127 (7034,390,0)                 (7004,30,9600)               ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton10setVisibleEb@plt>,<_ZN11TouchButton11setPositionEii@plt>,+8 U:<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>
  128 (7424,8,9644)                (7034,392,0)                 ROW        O:<_ZN6Layout13formatCreditsEi@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton10setVisibleEb@plt>,<_ZN11TouchButton11setPositionEii@plt>,+8
  129 (7432,10,9634)               (7426,8,9644)                ROW        O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt> U:<_ZN6Layout13formatCreditsEi@plt>
  130 (7446,10,9624)               (7434,10,9634)               ROW        O:<_ZN11AbyssEngine6StringC1EPKcb@plt> U:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  131 (7456,12,9614)               (7448,8,9624)                ROW        O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt> U:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  132 (7474,20,9604)               (7456,12,9614)               ROW        O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt> U:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  133 (7494,52,0)                  (7476,20,9604)               ROW        O:r4,r4,r4,r4,+2 U:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  134 (7546,20,9602)               (7496,52,0)                  ROW        O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt> U:r4,r4,r4,r4,+2
  135 (7566,156,0)                 (7548,20,9602)               ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton10setVisibleEb@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt> U:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  136 (7722,8,9208)                (7568,154,0)                 ROW        O:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton10setVisibleEb@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  137 (7730,42,0)                  (7722,8,9208)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt> U:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt>
  138 (7772,8,9206)                (7730,42,0)                  ROW        O:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  139 (7780,108,0)                 (7772,8,9206)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1 U:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt>
  140 (7888,8,9204)                (7780,108,0)                 ROW        O:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1
  141 (7896,44,0)                  (7888,8,9204)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt> U:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt>
  142 (7940,8,9202)                (7896,44,0)                  ROW        O:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  143 (7948,100,0)                 (7940,8,9202)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1 U:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt>
  144 (8048,8,9200)                (7948,100,0)                 ROW        O:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1
  145 (8056,32,0)                  (8048,8,9200)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt> U:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt>
  146 (8088,8,9198)                (8056,32,0)                  ROW        O:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  147 (8096,94,0)                  (8088,8,9198)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1 U:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt>
  148 (8190,8,9196)                (8096,94,0)                  ROW        O:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1
  149 (8198,32,0)                  (8190,8,9196)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt> U:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt>
  150 (8230,8,9194)                (8198,32,0)                  ROW        O:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  151 (8238,98,0)                  (8230,8,9194)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1 U:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt>
  152 (8336,8,9192)                (8238,98,0)                  ROW        O:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+1
  153 (8344,32,0)                  (8336,8,9192)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt> U:<_ZN11TouchButton7setTextERKN11AbyssEngine6StringE@plt>
  154 (8376,8,9190)                (8344,32,0)                  ROW        O:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt> U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  155 (8384,1316,0)                (8376,8,9190)                ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+41 U:<_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE@plt>
  156 -                            (8384,1316,0)                EXTRA-OURS U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN11TouchButton11setPositionEiih@plt>,<_ZN11TouchButton4drawEv@plt>,<_ZN11TouchButton10setVisibleEb@plt>,+41
  summary: {'LP-REMAP': 35, 'MERGE<-2': 1, 'SHIFT': 28, 'ROW': 32, 'EXTRA-OURS': 1}
====================================================================================================
fn 0x163b90  Hud::draw   reclen 476  diffbytes 37  bodyraw 1325  rows 77/77  lp-deltas {-2: 5}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  6   (2640,10,9270)               (2640,10,9268)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjii@plt>,<_ZN9PlayerEgo16hasVolatileGoodsEv@plt>
  9   (2816,16,9270)               (2816,16,9268)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  18  (3238,56,9272)               (3238,56,9270)               LP-REMAP   O:<_ZN6Status7getShipEv@plt>,<_ZN4Ship14getCurrentLoadEv@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjii@plt>,<_ZN9PlayerEgo16hasVolatileGoodsEv@plt>
  21  (3464,18,9272)               (3464,18,9270)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  57  (6500,12,9274)               (6500,12,9272)               LP-REMAP   O:<_ZN4Item9getAmountEv@plt>,<_ZN11AbyssEngine6StringC2Ei@plt>
  68  (7614,4,9056)                (7612,4,9056)                SHIFT      O:<_ZN11AbyssEngine11PaintCanvas16GetImage2DHeightEj@plt>
  69  (7696,412,9280)              (7696,416,9280)              SHIFT      O:<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,<_ZN11AbyssEngine11PaintCanvas12DrawRegion2DEjiiiifiiii@plt>,<_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringE@plt>,+13
  70  (8108,152,0)                 (8112,174,0)                 ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN9PlayerEgo18getDriveChargeRateEv@plt>,<_ZN9PlayerEgo12getCloakRateEv@plt>,<_ZN9PlayerEgo18getDriveChargeRateEv@plt>,+3 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN9PlayerEgo18getDriveChargeRateEv@plt>,<_ZN9PlayerEgo12getCloakRateEv@plt>,<_ZN9PlayerEgo18getDriveChargeRateEv@plt>,+4
  71  (8260,4,9268)                (8286,4,9266)                SHIFT      O:<_ZN11AbyssEngine11PaintCanvas15GetImage2DWidthEj@plt>
  72  (8292,4,9266)                (8420,42,9278)               ROW        O:<_ZN11AbyssEngine11PaintCanvas16GetImage2DHeightEj@plt> U:<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>
  73  (8416,204,9284)              (8496,126,9284)              ROW        O:<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>,<_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh@plt>,<_ZN11AbyssEngine11PaintCanvas12DrawRegion2DEjiiiifiiii@plt>,<_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringE@plt>,+1 U:<_ZN11AbyssEngine11PaintCanvas12DrawRegion2DEjiiiifiiii@plt>,<_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringE@plt>,<_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib@plt>
  74  (8620,226,0)                 (8622,224,0)                 SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN9PlayerEgo8isMiningEv@plt>,<_ZN6Status25getCurrentCampaignMissionEv@plt>,<_ZN9PlayerEgo19isDockingToAsteroidEv@plt>,+4
  summary: {'LP-REMAP': 5, 'SHIFT': 4, 'ROW': 3}
====================================================================================================
fn 0x170d38  SpaceLounge::drawLounge   reclen 168  diffbytes 14  bodyraw 175  rows 25/25  lp-deltas {-10: 1, 2: 5}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  1   (86,18,1868)                 (86,18,1870)                 LP-REMAP   O:<_ZN6Layout7drawBoxEiiiiiN11AbyssEngine6StringE@plt>
  5   (862,202,1874)               (862,202,1876)               LP-REMAP   O:<_ZN5Agent10getMissionEv@plt>,<_ZN5Agent10getMissionEv@plt>,<_ZN7Mission7getTypeEv@plt>,<_ZN8GameText7getTextEi@plt>,+7
  7   (1170,36,1874)               (1170,36,1876)               LP-REMAP   O:<_ZN5Agent8getOfferEv@plt>,<_ZN8GameText7getTextEi@plt>,<_ZN11AbyssEngine6StringC1ERKS0_b@plt>
  9   (1308,8,1874)                (1308,8,1876)                LP-REMAP   O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  10  (1330,14,1878)               (1330,14,1868)               LP-REMAP   O:<_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringE@plt>,<_ZN5Agent7isKnownEv@plt>
  14  (1460,8,1878)                (1472,8,1880)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  15  (1492,18,1854)               (1496,18,1854)               SHIFT      O:<_ZN6Layout7drawBoxEiiiiiN11AbyssEngine6StringE@plt>
  16  (1522,44,1878)               (1526,38,1880)               SHIFT      O:<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>,<_ZN11AbyssEngine11PaintCanvas13DrawRectangleEiiii@plt>,<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>
  17  (1574,34,1866)               (1572,36,1866)               SHIFT      O:<_ZN5Agent7isKnownEv@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>,<_ZN11AbyssEngine6StringC1EPKcb@plt>
  20  (1658,18,1878)               (1658,18,1880)               LP-REMAP   O:<_ZN5Agent7isKnownEv@plt>,<_ZN11AbyssEngine11PaintCanvas8SetColorEj@plt>
  summary: {'LP-REMAP': 6, 'SHIFT': 4}
====================================================================================================
fn 0x17a144  MGame::OnTouchEnd   reclen 280  diffbytes 37  bodyraw 4180  rows 44/44  lp-deltas {}
  #   orig (start,len,lp)          ours (start,len,lp)          class      calls
  17  (1188,10,7972)               (1192,10,7972)               SHIFT      O:<_ZN11AbyssEngineplERKNS_6StringES2_@plt>
  18  (1202,12,7968)               (1206,8,7968)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  30  (1542,2330,0)                (1542,2226,0)                ROW        O:<_ZN11LevelScript18canSkipCutsceneNowEv@plt>,<_ZN15MenuTouchWindow20setSkipButtonVisibleEb@plt>,<_ZN5MGame11pauseSoundsEv@plt>,<_ZN9FModSound17IsCategoryEnabledEi@plt>,+136 U:<_ZN11LevelScript18canSkipCutsceneNowEv@plt>,<_ZN15MenuTouchWindow20setSkipButtonVisibleEb@plt>,<_ZN5MGame11pauseSoundsEv@plt>,<_ZN9FModSound17IsCategoryEnabledEi@plt>,+131
  31  (3872,16,7890)               (3768,16,7890)               SHIFT      O:<_ZN8GameText7getTextEi@plt>,<_ZN12ChoiceWindow3setERKN11AbyssEngine6StringE@plt>
  32  (3888,172,0)                 (3784,238,0)                 ROW        O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN6Status10getMissionEv@plt>,<_ZN7Mission7getTypeEv@plt>,<_ZN6Status10getMissionEv@plt>,+13 U:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN6Status10getMissionEv@plt>,<_ZN7Mission7getTypeEv@plt>,<_ZN6Status10getMissionEv@plt>,+16
  33  (4060,4,7858)                (4022,4,7858)                SHIFT      O:<_ZN8FileReadC1Ev@plt>
  34  (4064,60,0)                  (4026,60,0)                  SHIFT      O:<_ZN7Mission17isCampaignMissionEv@plt>,<_ZN6Status25getCurrentCampaignMissionEv@plt>,<_ZN7Mission17isCampaignMissionEv@plt>,<_ZN6Status25getCurrentCampaignMissionEv@plt>,+1
  35  (4124,4,7856)                (4086,4,7856)                SHIFT      O:<_ZN8FileReadC1Ev@plt>
  36  (4128,420,0)                 (4090,366,0)                 ROW        O:<_ZN8FileRead11loadStationEi@plt>,<_ZN6Status10setStationEP7Station@plt>,<_ZN8FileReadD1Ev@plt>,<_ZdlPv@plt>,+26 U:<_ZN8FileRead11loadStationEi@plt>,<_ZN6Status10setStationEP7Station@plt>,<_ZN8FileReadD1Ev@plt>,<_ZdlPv@plt>,+23
  37  (4548,8,7840)                (4456,8,7840)                SHIFT      O:<_ZN11AbyssEngine6StringC1EPKcb@plt>
  38  (4564,46,7846)               (4472,46,7846)               SHIFT      O:<_ZN12ImageFactory10createCharEbi@plt>,<_ZN6Status10getStationEv@plt>,<_ZN7Station8getIndexEv@plt>,<_ZN7MissionC1EiN11AbyssEngine6StringEPiiiii@plt>
  39  (4610,314,0)                 (4518,312,0)                 SHIFT      O:<_ZN11AbyssEngine6StringD1Ev@plt>,<_ZN6Status10setMissionEP7Mission@plt>,<_ZN6Status19setFreelanceMissionEP7Mission@plt>,<_ZN11AbyssEngine18ApplicationManager27SetCurrentApplicationModuleEj@plt>,+17
  40  (4924,4,7844)                (4830,4,7844)                SHIFT      O:<_ZN8FileReadC1Ev@plt>
  41  (4928,236,0)                 (4834,442,0)                 ROW        O:<_ZN8FileRead11loadStationEi@plt>,<_ZN6Status13departStationEP7Station@plt>,<_ZN8FileReadD1Ev@plt>,<_ZdlPv@plt>,+12 U:<_ZN8FileRead11loadStationEi@plt>,<_ZN6Status13departStationEP7Station@plt>,<_ZN8FileReadD1Ev@plt>,<_ZdlPv@plt>,+21
  42  (5164,4,7842)                (5276,4,7842)                SHIFT      O:<_ZN8FileReadC1Ev@plt>
  43  (5168,2912,0)                (5280,2800,0)                ROW        O:<_ZN8FileRead11loadStationEi@plt>,<_ZN6Status10setStationEP7Station@plt>,<_ZN8FileReadD1Ev@plt>,<_ZdlPv@plt>,+158 U:<_ZN8FileRead11loadStationEi@plt>,<_ZN6Status10setStationEP7Station@plt>,<_ZN8FileReadD1Ev@plt>,<_ZdlPv@plt>,+151
  summary: {'SHIFT': 11, 'ROW': 5}
```

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9219
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 _work/reconstructions/extab_census.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so \
  .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 /dev/shm/fdiff.py _ZN5RadarC2EP5Level              # body raw diff
```

Scratch (not committed), `/dev/shm/w9219/`: `extab_census2.py`,
`census_final.txt`, `Radar.base.cpp`, `ModStation.base.cpp`, `measure.sh`,
`try_radar*.py`, `try_ms.py`, `Radar.ll`, `Radar.s`.

## 7. Open questions for the next wave

1. **`Radar::Radar` (7 B)**: the original's `enemyGuns` cleanup scope is
   nested differently from the landed source. The swap is between the
   `labelStrings` outer cleanup (`%175`) and the `enemyGuns` cleanup (`%372`).
   A source shape that moves `%372`'s trampoline after `%175`'s *without*
   changing `.text` would zero the 7 bytes. All brace probes tried were
   neutral or worse; the next lead is the *order of the `if` blocks* or a
   different temporary-lifetime marker in the `enemyGuns` block.
2. **`SpaceLounge::drawLounge` (14 B)**: same LP-swap family; the body is 175
   raw and the pad/x register swap (w145 §2) dominates. Once the body is
   exact the LP swap should be re-measured.
3. **The 550 B body-layout family** (ModStation, LevelScript, MGame,
   Level::createMission, Radar::draw, Level::createShip, Generator) has no
   separate EH construct: it will fall with the body.
4. **HangarWindow 762 / Hud 37**: the VarDecl splits are known; the blocker is
   the body RA (+4 section bytes / +36 B body), not the EH table.

## 8. Files changed

`_work/reconstructions/extab_census.py` (new decoder),
`_work/reconstructions/extab-census-wave146-w9219.md` (this note).
Gate unchanged: byte 4507 / linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.
