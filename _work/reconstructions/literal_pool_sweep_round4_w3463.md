# Literal-pool sweep round 4 (w3463)

Status: research note. Branch `wave87-w3463`, based on current `master` (`3d321579`).
All numbers below were measured in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`,
image base 0x10000). No source or flag change is landed by this note: every experiment below
was neutral or worse on the `check` gate, so the tree is left at the baseline.

Scope: the `report.json` rows scored `pct == 100.0` but not `byte` — the instruction stream and
every inter-function address already match, and only literal-pool words (plus branch
immediates that reference shifted PLT slots) still differ. This is the round-4 pass over
`literal_pool_audit.md`, `literal_pool_sweep_w3436.md`, `literal_pool_sweep_w3437.md`,
`literal_pool_sweep_w3449.md` and `plt-dynsym-order.md`.

## Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target diff -j8
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/w3463_base_classify.json
```

| metric | value |
|---|---|
| byte_exact | **4408** |
| linked_exact | 4496 |
| compared | 4524 |
| avg_match | 99.89 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 |
| data exact | 843 / 843 |
| imports | 207 |
| lint | `CLEAN ✓` |
| drift | `0 field(s) across 0 class(es)` |
| symbol parity | `original: 5370  ours: 5370  extra: 0  missing: 0` |

The task brief quoted `byte 4182 / linked 4495`; the checked-out tree is already at
`byte 4408 / linked 4496`. The numbers below are the measured ones.

Section sizes/bases (ours vs original, `readelf -SW`):

| section | ours | original | delta |
|---|---|---|---|
| `.text` | `0x1337c8 @ 0x69440` | same | 0 |
| `.ARM.exidx` | `0x6f78 @ 0x19cc08` | `0x6f70` | **+8** |
| `.ARM.extab` | `0x2628c @ 0x1a3b80` | `0x26298 @ 0x1a3b78` | **-12** |
| `.rodata` | `0x41b67 @ 0x1c9e10` | same | 0 |
| `.data.rel.ro` | `0x2fe8 @ 0x20cbb0` | same | 0 |
| `.got` | `0x430c @ 0x20fcf4` | same | 0 |
| `.data` | `0x296c @ 0x214000` | same | 0 |
| `.bss` | `0x69ac @ 0x216970` | same | 0 |

The exidx/extab +8/-12 is the known libc++abi placement residue documented in
`docs/IDENTICALITY.md`; `.rodata`'s base is still exact because the net -4 is absorbed by its
16-byte alignment.

## Classification of the 88 functions

`literal_pool_sweep_w3436.py` (reuses `tools/verify/verify.py`'s pool/jump-table scanner and
`elf.Elf32`) prints:

```
pct100 non-byte functions: 88  (same address 87, moved 1)
runs by kind: {'pool': 175, 'insn': 492}
functions by kind: {'pool': 66, 'insn': 24}
pool runs by (orig section, ours section):
  ('.rodata', '.rodata')                   148
  ('.bss', '.bss')                          27
pool functions by (orig section, ours section):
  ('.rodata', '.rodata')                    51
  ('.bss', '.bss')                          15
```

Every pool run is "pic-same": the resolved destination is the same logical datum in both
binaries, only its address differs. The only moved function is
`_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf` (`addr_delta +0x2`, 3 instruction runs);
the other 87 are at the same address.

### Instruction runs (492 runs / 24 functions)

The runs are dominated by six PLT slots whose relative order differs:

| symbol | original PLT | ours PLT |
|---|---|---|
| `Array<RadioMessage*>::Array()` | `0x64380` | `0x64398` |
| `ArraySetLength<RadioMessage*>` | `0x6438c` | `0x643a4` |
| `RadioMessage::RadioMessage(int,int,int,int)` | `0x64398` | `0x643b0` |
| `RadioMessage::RadioMessage(int,int,int,int,int)` | `0x643a4` | `0x643bc` |
| `FileRead::loadStaticCollision(int)` | `0x643b0` | `0x64368` |
| `FileRead::loadStationCollision(int)` | `0x643bc` | `0x64374` |

`Level::createRadioMessages` alone accounts for 441 of the 492 instruction runs (307 calls to
the 4-arg `RadioMessage` ctor, 65 to `Array<RadioMessage*>`, 65 to `ArraySetLength`, 4 to the
5-arg `RadioMessage` ctor). The `FileRead::load*Collision` pair is six PLT slots too early in
ours (`0x64368`/`0x64374` vs `0x643b0`/`0x643bc`); the `RadioMessage` group is two slots too
late (`0x64398` vs `0x64380`). The two groups rotate around two other PLT entries.

Cause (measured): `gold` assigns PLT slots in object emission order. Our `Level.cpp.obj` emits
`Level::getBoundingVolume` at section 162, before `Level::createRadioMessages` at section 190:

```
$ objdump -h cmake-build-match/CMakeFiles/gof2.dir/./src/game/world/Level.cpp.obj \
    | grep -E '^ *[0-9]+ \.text\._ZN5Level'
162 _ZN5Level17getBoundingVolumeEiP10AEGeometry
190 _ZN5Level19createRadioMessagesEi
```

`getBoundingVolume` first references `FileRead::loadStaticCollision/loadStationCollision`;
`createRadioMessages` first references the `Array<RadioMessage*>` ctor. The original's PLT
order therefore requires `createRadioMessages` to be emitted before `getBoundingVolume`.

Two controlled source moves were tried (see the A/B table): moving the
`createRadioMessages` definition before `createShip` was neutral; moving it before
`createStaticObject` overshot by nine PLT slots (byte 4406). clang's emission order here is a
call-graph worklist, not source order, so the original position is between the two. This is
still a hypothesis, not a landed fix.

### `.rodata` runs (148 runs / 51 functions)

All 148 runs resolve to the same datum at a different address. The deltas are local shifts, not
wrong data. They fall into three groups:

1. **`Level.cpp` plain `.rodata` (the first divergence).** The first differing byte is
   `0x1fb7f8`. The original has `jumpgateLodMeshes` (16-bit table `15016,15020,15017,15021,
   15018,15022,15019,15023`) at `0x1fb7f8`, then `asteroidMeshes` (`16900,16921,16904,18836`)
   at `0x1fb808`. Ours has the two tables swapped. Our source defines `createAsteroids` before
   `createSpace`; the original's `.rodata` order is `createSpace`'s table first. Swapping the
   two function definitions in `Level.cpp` fixes this pair and moves the first divergence to
   `0x1fc450`, but is byte-neutral (see A/B table).
2. **`Level.cpp` `g_ips_shipTypeMap` / `g_ips_skyColors`.** At `0x1fc3d0` both binaries have
   the same 32-int `g_ips_shipTypeMap` prefix. The original continues with 32 more ints
   (`0x1fc450..0x1fc4d0`) and then `g_ips_skyColors` (20 ints, 0x50 bytes) at `0x1fc4d0`.
   Ours ends the table at `0x1fc450` and starts `g_ips_skyColors` (19 ints, 0x4c bytes) there.
   The missing 0x80 + 0x4 bytes are the first content divergence after the table swap. Adding
   the 32 entries alone was already measured in `literal_pool_sweep_w3437.md` as making
   `.rodata` `+0x80` and byte 4232 (worse); the 0x84 bytes must be recovered elsewhere in the
   same block first.
3. **Cumulative shifts in later TUs.** After `Level.cpp`, the deltas are cumulative local
   shifts: `+96` (ModStation), `+112` (PlayerFighter/Globals/ImageFactory), `+16`
   (ListItemWindow/StarSystem/ChoiceWindow/HangarWindow/MissionsWindow/Gun/SolarSystem/
   Achievements/ObjectGun), `+24` (Hud). They are downstream of the `Level.cpp` block and of
   the merge-section ordering. One concrete root: `PlayerCreature.cpp`'s
   `kCreatureTypeFactor[4]` is emitted in `.rodata.str4.4` at `0x1fe160` in ours, while the
   original has the same 16 bytes in the plain `.rodata` block at `0x201dd0` (next to the
   `PlayerCreature` typeinfo at `0x201de0`). Forcing it into a non-merge section with
   `#pragma clang section` moved it to `0x201e10`/`0x201e04` (delta `+64`/`+52`), still not
   the original address; the merge-group order is the remaining cause.

### `.bss` runs (27 runs / 15 functions)

Five of the 15 functions are the VirtualInput block (`SlowMotion`, `SpeedUp`, `keyPressed`,
`keyReleasedWithDelay`, `SendStoredKeyUpEvents`); the other ten reference different globals in
other TUs. The VirtualInput layout is measurable from the PIC destinations:

| symbol | original | ours |
|---|---|---|
| `g_pActivity` | `0x216970` | `0x216970` |
| `g_delayedKeyCount` | `0x216974` | `0x216988` |
| `g_delayedKeys[0x400]` | `0x216978` (ends `0x217978`) | `0x21698c` (ends `0x21798c`) |
| `g_speedUp` | `0x217978` | `0x216980` |
| `g_slowMotion` | `0x21797c` | `0x216984` |
| `stackX` | `0x217980` | `0x216974` |
| `stackY` | `0x217984` | `0x216978` |
| `wheelStack` | `0x217988` | `0x21697c` |
| `g_simEdgesInit` | `0x21798c` | `0x21798c` |
| `g_simTouchActive` | `0x217990` | `0x217990` |
| `g_input` | `0x217994` | `0x217994` |

`g_input`, `g_simEdgesInit` and `g_simTouchActive` already match. The original order is
`g_delayedKeyCount, g_delayedKeys, g_speedUp, g_slowMotion, stackX, stackY, wheelStack`; ours
is `stackX, stackY, wheelStack, g_speedUp, g_slowMotion, g_delayedKeyCount, g_delayedKeys`.
The three exported globals `stackX/stackY/wheelStack` are emitted first by clang in ours.
Moving their declarations after the anonymous namespace did not change the `.o` `.bss` order
(measured), consistent with `literal_pool_sweep_w3437.md`'s note. Moving
`SendStoredKeyUpEvents`/`keyReleasedWithDelay` before `keyPressed` reordered the internal
scalars to `g_delayedKeyCount, g_delayedKeys, g_speedUp, g_slowMotion` (closer to the
original) but cost 14 byte_exact (4394) because the function move also changed the PLT order.

The other ten `.bss` functions are separate globals:

| function | original dest | ours dest | delta |
|---|---|---|---|
| `ModStation::OnKeyPress` | `0x217bf8` | `0x217be8` | -0x10 |
| `Engine::DoPostEffect` | `0x217b1d` | `0x217aed` | -0x30 |
| `Engine::SetPostEffect` | `0x217b1d` | `0x217aed` | -0x30 |
| `SpaceLounge::init` | `0x2183d4` | `0x2183c8` | -0xc |
| `StatusWindow::reInit` | `0x21839c` | `0x2183a0` | +0x4 |
| `DialogueWindow::loadContent` | `0x2183c0` | `0x2183b4` | -0xc |
| `GameText::getText` | `0x217a30` | `0x217a00` | -0x30 |
| `ndk23_Init` / `ndk23_InitWithZip` / `ndk23_handleAcceleration` | `0x217b60` | `0x217b50` | -0x10 |

These are other TUs' regular `.bss` ordering, not the VirtualInput block.

## Controlled A/B experiments

All rows are full `cmake --build cmake-build-match --target check -j8` rebuilds of this tree.
`pool`/`insn` are the classifier's run counts. No experiment used `--update-baseline`.

| # | experiment | byte | linked | pool | insn | funcs | verdict |
|---|---|---:|---:|---:|---:|---:|---|
| 0 | baseline | 4408 | 4496 | 175 | 492 | 88 | — |
| 1 | swap `createSpace`/`createAsteroids` definitions | 4408 | 4496 | 173 | 492 | 88 | neutral; first `.rodata` diff `0x1fb7f8 -> 0x1fc450` |
| 2 | #1 + move `createRadioMessages` before `createShip` | 4408 | 4496 | 173 | 492 | 88 | neutral |
| 3 | #1 + move `createRadioMessages` before `createStaticObject` | **4406** | 4496 | 173 | 542 | 90 | worse (-2) |
| 4 | move `SendStoredKeyUpEvents`/`keyReleasedWithDelay` before `keyPressed` | **4394** | 4496 | 190 | 524 | 102 | worse (-14) |
| 5 | `alignas(16)` on `kCreatureTypeFactor` | 4408 | 4496 | 175 | 492 | 88 | neutral; delta -15472 -> +64 |
| 6 | `#pragma clang section rodata=".rodata.kCreatureTypeFactor"` | 4408 | 4496 | 175 | 492 | 88 | neutral; delta -> +52 |
| 7 | `#pragma clang section rodata=".rodata"` | 4408 | 4496 | 175 | 492 | 88 | neutral; delta -> +64 |

Experiment 1 is the only one that is a strict improvement in the literal-pool metric
(148 -> 146 `.rodata` runs, first divergence moved forward) without any gate regression. It is
not a strict `byte_exact` improvement, so it is documented here but not landed. Experiment 1's
two fixed runs are `Level::createSpace` (2 -> 1) and `Level::createAsteroids` (4 -> 3); both
functions still have other diffs.

## Proven vs still hypothesis

Proven (command-backed):

- The baseline is byte 4408 / linked 4496 / lint CLEAN / drift 0 / symbol parity 0 extra,
  0 missing.
- The 88 `pct==100 && !byte` rows split into 492 instruction runs (24 functions) and 175 pool
  runs (66 functions); the pool runs are `.rodata` 148/51 and `.bss` 27/15.
- The `RadioMessage`/`Array<RadioMessage>` PLT group is two slots too late and the
  `FileRead::load*Collision` pair six slots too early; `Level::createRadioMessages` accounts
  for 441 of the 492 instruction runs.
- The first `.rodata` divergence is the `createSpace`/`createAsteroids` local-table swap at
  `0x1fb7f8`; swapping the two function definitions fixes it and moves the first divergence to
  `0x1fc450`.
- The original's `g_ips_shipTypeMap` is 64 ints and `g_ips_skyColors` is 20 ints; ours are 32
  and 19, and the missing 0x84 bytes are the first content divergence after the table swap.
- The VirtualInput `.bss` order is `g_delayedKeyCount, g_delayedKeys, g_speedUp, g_slowMotion,
  stackX, stackY, wheelStack, g_simEdgesInit, g_simTouchActive, g_input`; ours puts
  `stackX/stackY/wheelStack` first and `g_delayedKeyCount/g_delayedKeys` after `g_slowMotion`.
  `g_input` already matches.
- `kCreatureTypeFactor` is in `.rodata.str4.4` at `0x1fe160` in ours and in plain `.rodata` at
  `0x201dd0` in the original; `alignas(16)`/`#pragma clang section` moves it to
  `0x201e10`/`0x201e04`, not to `0x201dd0`.

Hypothesis (not closed):

- The original's `Level::createRadioMessages` was emitted before `Level::getBoundingVolume`
  (the PLT order proves it), but the exact source/TU shape that produces that worklist position
  is not known. Moving the definition before `createStaticObject` overshoots by nine slots;
  before `createShip` is neutral.
- The original's VirtualInput `stackX/stackY/wheelStack` are exported globals but appear after
  the internal scalars in `.bss`. clang emits exported globals first in this TU; reproducing
  the original order likely needs a different TU split or section placement, not a declaration
  reorder.
- The remaining `.rodata` shifts are the `Level.cpp` block (missing 0x84 bytes) plus the
  merge-group ordering of `.rodata.str4.*`/`.rodata.cst*`; fixing the first content
  divergence should collapse the `+16/+96/+112` cascade.
- The other ten `.bss` functions are separate TUs' regular `.bss` ordering and need their own
  source/TU pass.

## Reproduction commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target diff -j8

# classification (pct==100 && !byte)
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/w3463_base_classify.json

# .rodata content diff (stripped .so is enough for the raw diff)
python3 _work/reconstructions/rodata_content_diff_map.py

# unstripped relink with -Map for per-symbol addresses (scratch only)
# the exact gold command is the COMMAND of the libgof2hdaa.so rule in
# cmake-build-match/build.ninja, with `-Map=/tmp/w3463.map` and without the strip
/tmp/w3463_relink_map.sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -S --defined-only /tmp/w3463_unstripped.so | grep -E 'shipTypeMap|skyColors|jumpgate|asteroidMeshes|kCreatureTypeFactor'

# PLT slot comparison
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E '<(_ZN5ArrayIP12RadioMessageEC2Ev|_Z14ArraySetLengthIP12RadioMessageEvjR5ArrayIT_E|_ZN12RadioMessageC1Eiiii|_ZN8FileRead19loadStaticCollisionEi|_ZN8FileRead20loadStationCollisionEi)@plt>:'
$OD -d -j .plt cmake-build-match/libgof2hdaa.so | grep -E '<(_ZN5ArrayIP12RadioMessageEC2Ev|_Z14ArraySetLengthIP12RadioMessageEvjR5ArrayIT_E|_ZN12RadioMessageC1Eiiii|_ZN8FileRead19loadStaticCollisionEi|_ZN8FileRead20loadStationCollisionEi)@plt>:'
```

`/tmp/w3463_relink_map.sh` is the plain gold link of the current build with `-Map` and
without `--strip-all`; it is scratch only, not part of the tree.

## Full classification table (88 functions)

| # | function | size | insn | .rodata | .bss | other |
|---:|---|---:|---:|---:|---:|---|
| 1 | `_ZN5Level21createCampaignMissionEv` | 29676 | 0 | 45 | 0 |  |
| 2 | `_ZN11SpaceLounge9startChatEv` | 14450 | 0 | 1 | 0 |  |
| 3 | `_ZN5Level19createRadioMessagesEi` | 12282 | 441 | 0 | 0 |  |
| 4 | `_ZN13PlayerFighter6updateEi` | 10376 | 0 | 1 | 0 |  |
| 5 | `_ZN14ListItemWindow3setEP8ListItemjjjjb` | 6820 | 0 | 4 | 0 |  |
| 6 | `_ZN10ModStation12OnInitializeEv` | 6396 | 0 | 5 | 0 |  |
| 7 | `_ZN12HangarWindow10OnTouchEndEii` | 5828 | 0 | 1 | 0 |  |
| 8 | `_ZN10ModStation10OnTouchEndEiiPv` | 5404 | 6 | 1 | 0 |  |
| 9 | `_ZN14DialogueWindow11loadContentEv` | 3484 | 0 | 0 | 2 |  |
| 10 | `_ZN3Hud4initEv` | 3464 | 0 | 1 | 0 |  |
| 11 | `_ZN13PlayerStationC1EP7Station` | 3440 | 6 | 0 | 0 |  |
| 12 | `_ZN13PlayerStationC2EP7Station` | 3440 | 6 | 0 | 0 |  |
| 13 | `_ZN5Level11createSpaceEv` | 3076 | 0 | 2 | 0 |  |
| 14 | `_ZN7StarMap10OnTouchEndEii` | 2972 | 0 | 1 | 0 |  |
| 15 | `_ZN5Level10assignGunsEv` | 2948 | 0 | 1 | 0 |  |
| 16 | `_ZN10StarSystemC1Ei` | 2900 | 0 | 5 | 0 |  |
| 17 | `_ZN10StarSystemC2Ei` | 2900 | 0 | 5 | 0 |  |
| 18 | `_ZN10ModStation10OnKeyPressExx` | 2516 | 0 | 0 | 1 |  |
| 19 | `_ZN14MissionsWindow4initEv` | 2468 | 0 | 1 | 0 |  |
| 20 | `_ZN11SpaceLounge4initEv` | 2256 | 0 | 0 | 1 |  |
| 21 | `_ZN7StarMap4initEbP7Missionbi` | 2220 | 0 | 1 | 0 |  |
| 22 | `_ZN7StarMap14initStarSystemEv` | 2076 | 0 | 3 | 0 |  |
| 23 | `_ZN5Level9createGunEiiiiiiii` | 2012 | 0 | 12 | 0 |  |
| 24 | `_ZN5Level19initParticleSystemsEv` | 1872 | 0 | 5 | 0 |  |
| 25 | `_ZN7Globals12getShipGroupEiib` | 1856 | 0 | 5 | 0 |  |
| 26 | `_ZN7Globals18getDialogueSoundIdEiP5Agent` | 1724 | 0 | 2 | 0 |  |
| 27 | `_ZN5Level18createRadioMessageEii` | 1592 | 9 | 6 | 0 |  |
| 28 | `_ZN5Level15createAsteroidsEv` | 1588 | 0 | 4 | 0 |  |
| 29 | `_ZN9LensFlare8render2DEfffi` | 1568 | 0 | 3 | 0 |  |
| 30 | `_ZN10StarSystem9initLightEv` | 1476 | 0 | 3 | 0 |  |
| 31 | `_ZN5MGame12successCheckEv` | 1468 | 1 | 0 | 0 |  |
| 32 | `_ZN13PlayerFighterC1EiiP6PlayerP10AEGeometryfffb` | 1324 | 0 | 1 | 0 |  |
| 33 | `_ZN13PlayerFighterC2EiiP6PlayerP10AEGeometryfffb` | 1324 | 0 | 1 | 0 |  |
| 34 | `_ZN12Achievements16checkForNewMedalEP9PlayerEgo` | 1168 | 0 | 1 | 0 |  |
| 35 | `_ZN9ObjectGun6updateEi` | 1092 | 0 | 1 | 0 |  |
| 36 | `_ZN5MGame14startJumpSceneEv` | 896 | 1 | 0 | 0 |  |
| 37 | `_ZN17PlayerFixedObjectC1EiiP6PlayerP10AEGeometryfff` | 756 | 0 | 1 | 0 |  |
| 38 | `_ZN17PlayerFixedObjectC2EiiP6PlayerP10AEGeometryfff` | 756 | 0 | 1 | 0 |  |
| 39 | `_Z10keyPressedPN11AbyssEngine6EngineEi` | 708 | 0 | 0 | 6 |  |
| 40 | `_ZN5MGame13gameOverCheckEv` | 696 | 1 | 0 | 0 |  |
| 41 | `_ZN14MissionsWindow6updateEi` | 556 | 0 | 1 | 0 |  |
| 42 | `_ZN5Level17getBoundingVolumeEiP10AEGeometry` | 536 | 2 | 0 | 0 |  |
| 43 | `_ZN9ObjectGunC1EiP3GunijP5Level` | 516 | 0 | 2 | 0 |  |
| 44 | `_ZN9ObjectGunC2EiP3GunijP5Level` | 516 | 0 | 2 | 0 |  |
| 45 | `_ZN12ChoiceWindow8setMedalEii` | 476 | 0 | 1 | 0 |  |
| 46 | `_ZN10ModStation21resetIdleCamForHangarEv` | 360 | 0 | 2 | 0 |  |
| 47 | `_ZN11AbyssEngine6Engine13SetPostEffectEjb` | 352 | 0 | 0 | 1 |  |
| 48 | `_ZN14ListItemWindow6updateEi` | 352 | 0 | 1 | 0 |  |
| 49 | `_ZN11AbyssEngine6Engine12DoPostEffectEv` | 340 | 0 | 0 | 1 |  |
| 50 | `_ZN12ChoiceWindow13setMiscButtonERKN11AbyssEngine6StringE` | 332 | 1 | 0 | 0 |  |
| 51 | `_ZN5MGame12switchCameraEi` | 316 | 3 | 0 | 0 |  |
| 52 | `ndk23_InitWithZip` | 300 | 0 | 0 | 1 |  |
| 53 | `ndk23_Init` | 296 | 0 | 0 | 1 |  |
| 54 | `_ZN10StarSystem26switchSunForSupernovaIntroEv` | 292 | 0 | 1 | 0 |  |
| 55 | `_ZN8GameText7getTextEi` | 292 | 0 | 0 | 3 |  |
| 56 | `_ZN12StatusWindow6reInitEv` | 288 | 0 | 0 | 1 |  |
| 57 | `ndk23_handleAcceleration` | 276 | 0 | 0 | 3 |  |
| 58 | `_ZN3Gun8setIndexEi` | 272 | 0 | 1 | 0 |  |
| 59 | `_ZN12ImageFactory9loadImageEiii` | 268 | 0 | 1 | 0 |  |
| 60 | `_ZN11TouchButton7setTextERKN11AbyssEngine6StringE` | 248 | 1 | 0 | 0 |  |
| 61 | `_ZN14PlayerCreatureC1EiiP6PlayerP10AEGeometryfff` | 204 | 0 | 1 | 0 |  |
| 62 | `_ZN14PlayerCreatureC2EiiP6PlayerP10AEGeometryfff` | 204 | 0 | 1 | 0 |  |
| 63 | `_ZN12ChoiceWindow9setHeightEi` | 200 | 1 | 0 | 0 |  |
| 64 | `_ZN7Globals21getRandomEnemyFighterEi` | 176 | 0 | 1 | 0 |  |
| 65 | `_ZN3Gun9setOffsetEii` | 136 | 0 | 1 | 0 |  |
| 66 | `_ZN12ImageFactoryC1Ev` | 132 | 0 | 1 | 0 |  |
| 67 | `_ZN12ImageFactoryC2Ev` | 132 | 0 | 1 | 0 |  |
| 68 | `_ZN12ImageFactory10createCharEbi` | 128 | 0 | 1 | 0 |  |
| 69 | `_ZN14PlayerCreature4rageEi` | 80 | 0 | 1 | 0 |  |
| 70 | `_ZN11SolarSystem18hasHiddenBlueprintEv` | 76 | 0 | 1 | 0 |  |
| 71 | `_ZN4Ship8addCargoEP4Item` | 62 | 1 | 0 | 0 |  |
| 72 | `_Z21SendStoredKeyUpEventsPN11AbyssEngine6EngineE` | 60 | 0 | 0 | 2 |  |
| 73 | `_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf` | 60 | 3 | 0 | 0 |  |
| 74 | `_Z20keyReleasedWithDelayPN11AbyssEngine6EngineEi` | 36 | 0 | 0 | 2 |  |
| 75 | `_ZN12Achievements8getValueEii` | 28 | 0 | 1 | 0 |  |
| 76 | `_ZN4Ship8addCargoEP5ArrayIP4ItemE` | 24 | 1 | 0 | 0 |  |
| 77 | `_ZN14PlayerCreature9getWeightEv` | 20 | 0 | 1 | 0 |  |
| 78 | `_ZN4Item27canBeInstalledMultipleTimesEv` | 16 | 0 | 1 | 0 |  |
| 79 | `_Z10SlowMotionv` | 12 | 0 | 0 | 1 |  |
| 80 | `_Z7SpeedUpv` | 12 | 0 | 0 | 1 |  |
| 81 | `_ZN11TouchButton12setYPositionEi` | 12 | 1 | 0 | 0 |  |
| 82 | `_ZN11TouchButton11setPositionEii` | 8 | 1 | 0 | 0 |  |
| 83 | `_ZN4Item8isInListEiP5ArrayIPS_E` | 8 | 1 | 0 | 0 |  |
| 84 | `_ZN12ChoiceWindow6updateEi` | 6 | 1 | 0 | 0 |  |
| 85 | `_ZN4Item8isInListEPS_P5ArrayIS0_E` | 6 | 1 | 0 | 0 |  |
| 86 | `_ZN7BeamGun8setEnemyEP6Player` | 6 | 1 | 0 | 0 |  |
| 87 | `_ZN9SpriteGun6updateEi` | 6 | 1 | 0 | 0 |  |
| 88 | `_ZN9SpriteGun8setEnemyEP6Player` | 6 | 1 | 0 | 0 |  |
