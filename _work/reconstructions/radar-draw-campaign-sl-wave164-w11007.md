# Radar::draw wave 164 (w11007) — campaign / space-lounge (`sl`) block map

**Status:** source unchanged (base is still the best measured size-exact variant);
note landed 2026-11-08. Worktree `wt/w11007`, base master `b3d3a1f68` (wave 163).
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`.

Read with `radar-draw-campaign-sl-wave163-w10909.md`,
`radar-draw-campaign-sl-wave162-w10809.md`, `wave163-merge-tests.md`.

Baseline (wave-163 HEAD): original 9,908 B; ours 9,908 B; raw same-address diff
**8,124**; instruction-index alignment **1,320**; `.text` 84,652; `.ARM.extab`
725; `.rel.plt` byte-identical; gate byte_exact 4507/4524, linked 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

All addresses below are the **original** `libgof2hdaa.so` (image base 0x10000).

## 0. Correction to the wave-163 handoff

The w10909 note claimed the exit-of-loop test at `12e748` is
`lockedStation + otherLock` and that ours tests `planetHit + otherLock`
("slot assignment swapped").  That is a misread.  Slot `sp+108` is reused:
it holds `notTurretMode` inside the landmark block (`12e7c6`) and is then
overwritten with **`landmarkHit`** at `12e3a8` (`str r1,[sp,#108]`, where
`r1` is the `landmarkHit & 1` value produced at `12eb42/12eb44`).  Slot
`sp+148` is `turretMode` (written once at `12e0e8` from `isInTurretMode`).
So `12e748` is `if (turretMode | landmarkHit) goto 12ec24`, i.e. exactly the
source `if (!turretMode && !landmarkHit)`.  There is **no** exit-chain
divergence — only the slot numbers differ (ours: `sp+152` turretMode,
`sp+112` landmarkHit).

## 1. Original campaign block map (`12e390`–`12e798`)

The block is entered from the `otherLock` recompute at `12e376`–`12e38e`
(`campaign` is in `sl`; `cmp.w sl,#2` at `12e38a`).  `fp` is the loop index,
`r8` = `stationId`, `r9` = `currentStation`, `sl` = `stations`/`stationId`/
`status`/`planetTargets` scratch.

```
addr    block                                                        terminator
12e390  campaign guard tail: if (campaign < 2) skip                  blt.w 12ec24
12e394  if (planetTargets == 0 || alienOrbit) skip                   bne.w 12ec24
12e3a6  if (isDockingToPlanet()) skip                                bne.w 12ec24
12e3b8  stations = getSystem()->getStations();                        fallthrough
        [sp+112] = missionBlocksPlanets; i = 0;
        [sp+116] = stations; planetHit = 0 ([sp+152])
12e3d4  LOOP HEAD: if (i >= planetTargets->size()) -> EXIT           bcs.w 12e748
12e3e0  if (data()[i] == null) -> CONTINUE                           beq.w 12e4ec
12e3ec  update(data()[i]); if (!onScreen) -> CONTINUE                beq.n 12e4ec
12e3fa  stationId = stations->data()[i];                             fallthrough
        currentStation = getStation()->getIndex();
        warpGate = getSystem()->getWarpGateEnumIndex();
12e418  textOffset = 10;                                             beq.n 12e438
        if (stationId != currentStation && i == warpGate) {
            DrawImage2D(image_0xbc, x+10, y-10); textOffset = 24; }
12e438  [sp+128] = textOffset;                                       fallthrough
        campaignMission = getCampaignMission();
        freelance = getFreelanceMission();
12e44a  if (campaignMission == null) -> ELSE                          cbz r6,12e4a8
12e44c  if (campaignMission->isEmpty()) -> ELSE                       cbnz r0,12e4a8
12e454  if (!campaignMission->isVisible()) -> ELSE                    cbz r0,12e4a8
12e45c  if (getCurrentCampaignMission() < 148) -> BODY1               blt.n 12e474
12e468  if (getCurrentCampaignMission() < 152) -> ELSE                blt.n 12e4a8
12e474  BODY1 (fall-through, inline):                                 beq.w 12e600
        stationId = stations->data()[i];
        isTarget = (stationId == campaignMission->getTargetStation());
        marked = 0;                       [sp+132], duplicated init
        if (stationId == currentStation) -> 12e600
12e48c  if (stationId != isTarget) -> MISSION CHAIN                   bne.w 12e620
12e496  if (getCurrentCampaignMission() != 120) -> DRAW_C8            bne.w 12e606
12e4a4  marked = 0; -> MARK_STORE                                     b.n 12e61e
12e4a8  ELSE: marked = 0;                                             fallthrough
12e4aa  MARK_STORE: [sp+132] = marked;                                fallthrough
12e4ac  FREELANCE: if (freelance == null) -> HIT                      cbz r5,12e4dc
12e4ae  if (freelance->isEmpty()) -> HIT                              cbnz r0,12e4dc
12e4b6  if (!freelance->isVisible()) -> HIT                           cbz r0,12e4dc
12e4be  isTarget = stationId == (type==14 ? getAgent()->getStation()  bne.n 12e564
                                              : getTargetStation());
12e4cc  if (freelance->getType() != 14) -> 12e564                     bne.n 12e564
12e4d0  getAgent()->getStation(); -> 12e56a                           b.n 12e56a
12e564  getTargetStation();                                           fallthrough
12e56a  if (stationId == currentStation) -> 12e5fa                    beq.n 12e5fa
12e570  if (!isTarget) -> 12e4de                                      bne.n 12e4de
12e578  DrawImage2D(image_0xc4, x+textOffset, y-10); marked = 1;      b.n 12e4de
12e4dc  status; if (!(otherLock|planetHit|turretMode)) -> HIT_TEST    beq.n 12e4f2
12e4de  OR-chain: [sp+144]|[sp+152]|[sp+148]                          fallthrough
12e4ec  CONTINUE: ++i; -> LOOP HEAD                                   b.n 12e3d4
12e4f2  HIT_TEST: size = (i==orbitPlanetIndex)?0x128:0x124;           ble.n 12e4ec
        left/top = crosshair - size/2; bounds checks
12e55c  if (i == orbitPlanetIndex) -> 12e560                          bne.n 12e594
12e560  planetHit = 0; -> 12e5b2                                      b.n 12e5b2
12e594  if (field_0x18 != data()[i]) planetLockTimer = 0;             fallthrough
        planetDockIndex = i; field_0x18 = data()[i]; planetHit = 1;
12e5b2  [sp+152] = planetHit; if (stationId == currentStation)        beq.n 12e4ec
        -> CONTINUE
12e5b8  DrawString(planetNames[i], x+textOffset+(marked?14:0), y-10)  b.n 12e4ec
12e5fa  -> 12e4de                                                     b.n 12e4de
12e600  -> MISSION CHAIN                                              b.n 12e620
12e606  DRAW_C8: DrawImage2D(image_0xc8, x+textOffset, y-10);         fallthrough
        marked = 1;
12e61e  [sp+132] = marked;                                            fallthrough
12e620  MISSION CHAIN: cur==116 && (stationId-90)<=4 &&               bne.n 12e658
        (getStatusValue() & (1<<(stationId-90))) == 0
12e658  cur==120 && stationId==93                                     bne.n 12e66e
12e66e  cur==125 && isFreighterMissionStation(stationId) &&           bne.n 12e6e4
        (getStatusValue() & (1<<getFreighterMissionStationBit())) == 0
12e6c6  DRAW_C8: DrawImage2D(image_0xc8,...); marked = 1; -> 12e4aa   b.n 12e4aa
12e6e4  -> 12e4ac                                                     b.n 12e4ac
12e6ea  -> 12e4ac                                                     b.n 12e4ac
12e748  EXIT: if (turretMode | landmarkHit) -> 12ec24                 bne.w 12ec24
12e754  if (field_0x14 != 0) { if (field_0x14 == field_0x18)          cbz r0,12e768
            lockConfirmed = 1; field_0x14 = 0; }
12e768  if (planetHit && !isDockingToAsteroid() &&                    beq.n 12e78a
            !isDockingToDockingPoint()) { ... }
12e78a  if (field_0x14 == 0) planetLockTimer = 0; else field_0x14=0;  fallthrough
12e79a  nop + literal pool (12e79c..12e7a6)
12e7a8  landmark body (out of line, logically precedes this block)
```

### Key layout facts

* **BODY1 is the fall-through of the campaign-mission guard** (`12e472 blt.n
  12e4a8`, fall-through to `12e474`).  The `marked = 0` else is placed after
  body1 (`12e4a8`) and falls into the freelance block.
* **MISSION CHAIN is out of line** at `12e620`, after the whole freelance +
  hit-test region.  This is a single `if (guard) { body1; missionChain }`
  with the small body1 inline and the large mission chain sunk.
* `marked = 0` is **duplicated**: `12e486` (inside body1) and `12e4a8`
  (else).  The compiler sank the `bool marked = false;` initialiser into
  both paths.
* The `marked` value is re-normalised to 0/1 at `12e67a`–`12e684`
  (`r1 = marked; if (r1) r1 = 1; [sp+132] = r1`) just before the `cur==125`
  test — i.e. `marked = marked != 0`.

## 2. Exact condition chains (original)

### 2.1 Campaign-mission guard (`12e44a`–`12e472`)

```cpp
if (campaignMission != nullptr &&
    !campaignMission->isEmpty() &&
    campaignMission->isVisible() &&
    (Globals::status->getCurrentCampaignMission() < 148 ||
     Globals::status->getCurrentCampaignMission() >= 152)) { ... }
```

The range tail is evaluated as two separate `getCurrentCampaignMission()`
calls (`12e460` and `12e46c`); nothing is cached.  The body is the
fall-through (hot), so the guard is **not** marked cold.

### 2.2 Landmark inner hit-test (`12e84c`–`12e868`)

```cpp
if ((!landmarkHit && notOtherLock) &&
    (i != 3 && notTurretMode)) { ... }
```

Witnessed lowering:

```
12e84c  subs.w  r2, sl, #3        ; r2 = i - 3
12e850  mov.w   r3, #1
12e854  it ne
12e856  movne   r2, #1            ; r2 = (i != 3)
12e858  ldr     r6, [sp,#152]     ; landmarkHit
12e85a  bics    r3, r6            ; r3 = 1 & ~landmarkHit
12e85c  ldr     r6, [sp,#116]     ; notOtherLock
12e85e  ands    r3, r6            ; r3 = !landmarkHit & notOtherLock
12e860  cmp     r3, #1
12e862  bne     12e8d0
12e864  ldr     r3, [sp,#108]     ; notTurretMode
12e866  ands    r2, r3            ; r2 = (i != 3) & notTurretMode
12e868  beq     12e8d0
```

The current source is `!landmarkHit && i != 3 && notTurretMode &&
notOtherLock`, which clang reassociates to
`(!landmarkHit && i != 3) && (notTurretMode && notOtherLock)` — a different
grouping (see §4).

### 2.3 Exit chain (`12e748`)

```cpp
if (!turretMode && !landmarkHit) { ... }
```

lowered as `if (turretMode | landmarkHit) goto 12ec24`, with `turretMode` in
`sp+148` and `landmarkHit` in `sp+108` (slot reused from `notTurretMode`).

## 3. `sl` (r10) lifecycle in the original

`sl` is the heavily-reused callee-saved scratch register.  `campaign` only
owns it for the first 0x292 bytes:

| addr | instruction | value |
|---|---|---|
| `12e0f8` | `mov sl, r0` | `campaign` = getCurrentCampaignMission() |
| `12e38a` | `cmp.w sl, #2` | `campaign >= 2` (only use) |
| `12e3c6` | `mov sl, r0` | `stations` = getSystem()->getStations() |
| `12e3ce` | `str.w sl, [sp,#116]` | save `stations` |
| `12e3fa`/`12e474`/`12e4be`/`12e62c`/`12e644`/`12e662` | `ldr.w r0,[sl,#4]` | `stations->data()` |
| `12e478`/`12e4c2` | `ldr.w sl,[r0,fp,lsl#2]` | `stationId` |
| `12e48e`/`12e572`/`12e5fa`/`12e600`/`12e6e4` | `ldr.w sl,[sp,#116]` | restore `stations` |
| `12e66e` | `ldr.w sl,[sp,#156]` | `status` |
| `12e6b8`/`12e6ea` | `mov sl, r6` | `stations` |
| `12e70c` | `mov sl, r0` | `planetTargets` |
| `12e732` | `mov sl, r8` | restore |
| `12e7ba` | `str.w sl, [sp,#144]` | save `otherLock` |
| `12e7be` | `mov.w sl, #0` | landmark loop index `i` |

The zero / `missionBlocksPlanets` constant is in **`fp`**, not `sl`
(`mov.w fp,#0` at `12e07a` and `12e0d8`; `subs.w fp,r0,#172` at `12ebe0`;
`str.w fp,[sp,#112]` at `12e3ba`).  In our base it is `sl`
(`mov.w sl,#0` / `subs.w sl,r0,#172` / `str.w sl,[sp,#116]`) and `campaign`
is spilled to `sp+144` (`str r0,[sp,#144]` at `12e0f8`, reloaded at
`12e38c`).

## 4. Structural differences vs the current source (base)

| # | original | current base |
|---|---|---|
| 1 | `campaign` in `sl`; zero/mbp in `fp` | `campaign` in `[sp,#144]`; zero/mbp in `sl` |
| 2 | campaign-mission **body1 inline** (fall-through), `marked=0` else after it, mission chain out of line | body1 + mission chain both out of line (`12e5c6`/`12e606`), `marked=0` else inline (`12e476`) |
| 3 | landmark inner test grouped `(!landmarkHit && notOtherLock) && (i != 3 && notTurretMode)` | grouped `(!landmarkHit && i != 3) && (notTurretMode && notOtherLock)` |
| 4 | exit chain in `sp+148`/`sp+108` | `sp+152`/`sp+112` (same logic) |

Difference 2 is a direct consequence of `__builtin_expect(guard, 0)` in the
source: the expect makes the guard cold, so clang lays out the else first and
outlines body1+mission chain.  The original lays body1 out as fall-through.

## 5. Measured variants (all `fn_diff` on the real link)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **base (HEAD)** | **8,124** | **1,320** | 9,908 |
| guard `expect(...,0)` removed (`noexp`) | 8,691 | 1,392 | 9,968 (+60) |
| guard `expect(...,1)` | 9,351 | 2,055 | 9,952 (+44) |
| `expect` on range tail only | 8,124 | 1,320 | 9,908 (== base) |
| `expect(campaignMission != nullptr,0)` | 9,258 | 2,903 | 9,948 |
| `expect(!isEmpty(),0)` | 9,150 | 2,881 | 9,896 |
| `expect(isVisible(),0)` | 9,171 | 2,877 | 9,892 |
| `noexp` + faithful landmark grouping | 8,794 | 1,378 | 9,972 (+64) |
| faithful landmark grouping (`(!lh&&!ol)&&(i!=3&&!tm)`) | 8,494 | 1,313 | 9,912 (+4) |
| `(i != 3 && !lh && !tm && !ol)` | 8,540 | 1,306 | 9,904 (−4) |
| `(!lh && i != 3) && (!tm && !ol)` | 8,124 | 1,320 | 9,908 (== base) |
| faithful grouping + `expect(!turretMode && !landmarkHit, 1)` on exit | 8,374 | 1,419 | **9,908** |
| w10909 shape (`landmarks` 0 + route `onScreen` 1) | 9,611 | 3,605 | 9,908 |
| route `expect(...,1)` only | 8,124 | 1,320 | 9,908 (== base) |
| `landmarks` `expect(...,0)` only | 9,600 | 3,580 | 9,900 |
| `int marked` | 8,513 | 1,380 | 9,912 |
| `int planetHit` | 9,188 | 1,983 | 9,952 |
| `int landmarkHit` | 8,798 | 1,479 | 9,928 |
| `int otherLockFlag` | 9,163 | 1,436 | 9,852 |
| `bool notOtherLock` | 8,916 | 1,607 | 9,844 |
| `bool notTurretMode` | 9,558 | 1,699 | 9,840 |
| `int`/`char`/`short`/`long` `missionBlocksPlanets` | 8,246 | 1,366 | 9,904 |
| `volatile bool missionBlocksPlanets` | 9,546 | 2,730 | 9,828 |
| hit-test operand permutations (6) | 8,124 or 9,056 | 1,320/1,864 | 9,908/9,904 |
| exit `!landmarkHit` first | 8,124 | 1,320 | 9,908 (== base) |

Best size-exact raw remains the base **8,124 / 1,320 / 9,908**.

## 6. Why the `sl` flip is hard

The greedy allocator enqueues live ranges by size
(`Prio = (1<<29) + Size`).  In the base the coalesced zero /
`missionBlocksPlanets` vreg is the larger range and wins `sl`; `campaign`
(2 uses over 0x292 bytes) is the smallest remaining range and spills to
`sp+144`.  `campaign`'s use is fixed at `12e38a`, so hoisting its definition
cannot extend it past the zero vreg's span.  The only shapes that flip it
change the block layout (and therefore the size), which is why w10909's
`landmarks`-cold shape reached `sl` but cost +1,487 raw.

The original reaches `sl` for `campaign` because its zero vreg got `fp`
instead — a different allocator tie-break, not a different source range.

## 7. Gate

`check` green on the unchanged base: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data **843/843**, imports **207**,
lint CLEAN, drift 0, parity 5370/5370.  No source change was landed (the
tree is clean at `b3d3a1f68` + this note).

## 8. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# scratch harnesses: /dev/shm/w11007/{b1..b18}.py, blockdiff.py, cfg.py, idiff.py
```
