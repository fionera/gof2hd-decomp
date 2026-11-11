# Build provenance round 12 (w11720): the 306-row `.ARM.extab` census on the
# wave-171 base, the surviving per-TU knobs, and the remaining flag families

Status: **research note, no new landed code** (2026-11-11).
Worktree `wt/w11720`, branch `wave169-w11720`, base master `41ef4c8b6`
(wave 171).  Scratch `/home/debian/GalaxyOnFire2/scratch/w11720`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit: **RATCHET PASS**, `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.text` **81,661** differing bytes,
`.ARM.extab` **711**, `.rel.plt` **0** (byte-identical), build-id 20,
total **82,392**.

## 0. Bottom line

1. The wave-171 `.ARM.extab` residual is still **711 bytes in 11 records /
   306 differing row tuples**.  The full row-level enumeration is section 2.
   It decomposes into **96 lp-only rows, 117 body-only rows and 93 both**.
2. **Two records are pure landing-pad identity ties** and are the only
   cleanup-scope candidates left:
   * `Radar::Radar` (7 B): rows 1/3/11/13/15 and 19/21 use two adjacent
     identical `b.n` trampolines in the opposite order.  `.text` is already
     byte-identical (`raw 0`).
   * `SpaceLounge::drawLounge` (14 B): the original has **one** landing-pad
     thunk for rows 10/14/16/20; ours has **two** (a redundant 2-byte
     trampoline at fn+1868 plus the real pad at fn+1880).  This is a Clang
     cleanup-scope difference, not a body-layout shift.
3. **Nine records (690 B) are body-layout consequences** (start/length moves
   and downstream LP shifts).  They cannot be removed without fixing the
   corresponding `.text`.  `LevelScript::process` (187 B) and
   `Level::createMission` (159 B) are the largest.
4. **The surviving per-TU knobs are still worth their bytes on the
   wave-171 base**: `Radar.cpp -disable-postra-machine-licm` **2 B**,
   `Radar.cpp +cheap-predicable-cpsr` **4 B**, `Level.cpp
   -enable-earlycse-memssa=false` **1 B**, `Layout.cpp +prof-unpr` **1 B**.
   The `LevelScript.cpp -disable-postra-machine-sink` knob is **dead**
   (neutral).  All four leave `.ARM.extab` at 711.
5. **The ARM subtarget-feature family is exhausted**: the 16 features that
   were only swept on `LevelScript`/`Radar` in round 11 were swept on the
   other eight residual TUs (128 cells).  Every cell is neutral except the
   already-known `Level +use-misched`/`+vmlx-hazards`/`+neon-fpmovs`,
   `SpaceLounge +use-misched`/`+vmlx-hazards`, `Layout +use-misched`
   size/layout movers.  No new text or extab win.
6. **The post-RA knob family is exhausted**: `-enable-earlycse-memssa=false`,
   `-disable-postra-machine-sink`, `-disable-postra-machine-licm` on all ten
   residual TUs (27 cells) are neutral or worse.
7. **SpaceLounge branch-weight ratios 2:1..20:1** were link-measured: 7:1
   (the checked-in value) is best at 81,661/711; 3:1 and 10:1 are exactly
   neutral; every other ratio is text-worse (82,394/716 at 2:1/4:1/5:1/6:1/
   8:1/12:1, 82,397/720 at 20:1, 81,664/715 at 15:1).

## 1. Baseline (this worktree)

| metric | value |
|---|---:|
| `.text` differing bytes | 81,661 |
| `.ARM.extab` differing bytes | 711 |
| `.rel.plt` differing bytes | 0 |
| build-id differing bytes | 20 |
| total differing bytes | 82,392 |
| byte_exact / linked_exact | 4507 / 4507 |
| size_exact | 4524 / 4524 |
| data_exact | 843 / 843 |
| imports | 207 |

Per-function raw same-address diffs of the 11 extab-record owners:

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | 53,812 | 17,932 | 57,232 |
| `Radar::draw` | 7,061 | 1,561 | 9,908 |
| `Level::createMission` | 5,425 | 936 | 17,172 |
| `HangarWindow::render` | 2,282 | 401 | 9,700 |
| `Level::createShip` | 1,560 | 455 | 2,764 |
| `Hud::draw` | 558 | 230 | 9,332 |
| `MGame::OnTouchEnd` | 3,778 | 1,457 | 8,080 |
| `SpaceLounge::drawLounge` | 175 | 41 | 1,948 |
| `Generator::createMission` | 127 | 107 | 1,996 |
| `ModStation::ModStation` | 43 | 12 | 1,008 |
| `Radar::Radar` | **0** | **0** | 2,488 |

## 2. `.ARM.extab` row census (11 records, 711 bytes)

Row tuples are `(start, length, landing-pad, action)` relative to the
function start.  "diff" counts differing tuples; "lp-only" is the subset
where only the landing-pad identity differs; "body" is where only
start/length differ; "both" is both.

| record | owner | len | rows | diff | lp-only | body | both | lp-only delta (orig-ours) |
|---|---|---:|---:|---:|---:|---:|---:|---|
| 0x1a78c4 | `Generator::createMission` | 172 | 26 | 6 | 6 | 0 | 0 | `+2 x4, +20, -4` |
| 0x1a9ac4 | `Level::createMission` | 956 | 148 | 97 | 55 | 23 | 19 | `+4 x19, +6 x14, +8 x13, +18 x4, +10 x5` |
| 0x1ab09c | `Level::createShip` | 336 | 57 | 48 | 10 | 23 | 15 | `-6 x10` |
| 0x1ada6c | `ModStation::ModStation` | 112 | 15 | 1 | 0 | 1 | 0 | — |
| 0x1c1f08 | `Radar::Radar` | 256 | 39 | 7 | 7 | 0 | 0 | `-2 x5, +2 x2` (swap) |
| 0x1c203c | `Radar::draw` | 132 | 19 | 16 | 3 | 5 | 8 | `+8 x2, +2` |
| 0x1c2af8 | `LevelScript::process` | 348 | 46 | 46 | 0 | 19 | 27 | — |
| 0x1c30fc | `HangarWindow::render` | 948 | 156 | 48 | 4 | 27 | 17 | `+10, -2 x3` |
| 0x1c57cc | `Hud::draw` | 476 | 77 | 11 | 5 | 4 | 2 | `+2 x5` |
| 0x1c7138 | `SpaceLounge::drawLounge` | 168 | 25 | 10 | 6 | 2 | 2 | `-2 x5, +10` |
| 0x1c8034 | `MGame::OnTouchEnd` | 280 | 44 | 16 | 0 | 13 | 3 | — |
| **total** | | | | **306** | **96** | **117** | **93** | |

The row-tuple dumps are reproducible with
`_work/reconstructions/extab_lsda_decode.py` and
`_work/reconstructions/extab_record_map.py`.  The per-record lp-only maps
(the exact orig→ours pad-address sets) are:

```
Generator::createMission   {1912:[1910], 1910:[1908], 1906:[1904],
                            1932:[1912], 1904:[1902], 1888:[1892]}
Level::createMission       uniform -4/-6/-8/-10/-18 downstream shifts
Level::createShip          uniform -6 downstream shift
Radar::Radar               {2462:[2464], 2464:[2462]}          <- swap
Radar::draw                {9818:[9810], 9870:[9868], 9816:[9808]}
HangarWindow::render       {9544:[9534], 9512:[9514], 9514:[9516]}
Hud::draw                  {9270:[9268], 9272:[9270], 9274:[9272]}  (uniform +2)
SpaceLounge::drawLounge    {1868:[1870], 1874:[1876], 1878:[1868,1880]}
```

## 3. Per-record analysis

### 3.1 `Radar::Radar` (7 B, pure pad-identity tie; `.text` byte-identical)

`.text` is exact (`raw 0`, 2,488 B, 812 insns).  The four cleanup pads at
the function tail are:

```
12de20: b.n 12de2a        ; pad 2460, skips the exception save
12de22: b.n 12de28        ; pad 2462
12de24: b.n 12de28        ; pad 2464
12de26: b.n 12de28        ; pad 2466
12de28: mov r5, r0
12de2a: ldr r0, [sp, #16] ; this->field_0x18c String
12de2c: blx StringD1Ev
12de30: mov r0, r5
12de32: blx _Unwind_Resume
```

Pads 2462/2464/2466 are byte-identical `b.n 12de28` trampolines.  The LSDA
assigns rows 1/3/11/13/15 (labelStrings / `new Array` / `getLandmarks`) to
2462 in the original and 2464 in ours; rows 19/21 (playerGuns loop /
Image2DCreate) to 2464 in the original and 2462 in ours; row 20 (enemyGuns)
to 2466 in both; row 39 to 2460 in both.  The IR order is the original's
order (`%175` function-level cleanup before `%304` playerGuns cleanup), so
this is a `MachineBlockPlacement` frequency tie that our build resolves the
other way.  Round 11 already showed no branch-weight/BPI/MBP knob, no
`__builtin_expect`, and no scope/`{}` shape flips it without moving
`.text`; re-confirmed here: `{}` around the playerGuns block and around
labelStrings[1..3] are exactly neutral (81,661/711).

### 3.2 `SpaceLounge::drawLounge` (14 B, cleanup-scope difference)

The body is 175 raw bytes different, but the structural cause is visible in
the cleanup tail.  Original:

```
171482 (1866): b.n 17148e   ; one thunk for rows 10/14/16/20
171484 (1868): mov r4,r0    ; sp+44 cleanup
171486 (1870): add r0,sp,#44
171488 (1872): b.n 171498
17148a (1874): mov r4,r0
17148c (1876): b.n 171496
17148e (1878): mov r4,r0    ; sp+92 cleanup  <- rows 10/14/16/20
171490 (1880): add r0,sp,#92
171492: blx StringD1Ev
171496 (1886): add r0,sp,#104
171498: blx StringD1Ev
17149c: mov r0,r4
17149e: blx _Unwind_Resume
1714a2: nop                 ; literal-pool alignment
```

Ours has **two** identical thunks and one extra 2-byte instruction:

```
171482 (1866): b.n 171490   ; row 17
171484 (1868): b.n 171490   ; row 10  <- redundant thunk
171486 (1870): mov r4,r0    ; sp+44 cleanup
171488 (1872): add r0,sp,#44
17148a (1874): b.n 17149a
17148c (1876): mov r4,r0
17148e (1878): b.n 171498
171490 (1880): mov r4,r0    ; sp+92 cleanup  <- rows 14/16/20
171492: add r0,sp,#92
171494: blx StringD1Ev
171498: add r0,sp,#104
17149a: blx StringD1Ev
17149e: mov r0,r4
1714a0: blx _Unwind_Resume
```

The extra thunk is the landing-pad block for the first `GetTextWidth`
(IR `%134`, predecessors `%110`/`%114`), which branches to the same cleanup
(`%290`) as the later `String("")`/`SetColor`/`DrawRectangle` landing pad
(`%238`).  In the original those two landing-pad blocks are one; in ours
Clang emits two.  Removing the thunk would be size-exact (the original's
2-byte `nop` at fn+1898 absorbs it) and would fix rows 10/14/16/20
(14 extab bytes).

Shapes measured (all link-measured; the first four are exactly neutral at
81,661/711, the rest are size/footprint movers):

| shape | `.text` | extab |
|---|---:|---:|
| base | 81,661 | 711 |
| drop the `{}` around the `isKnown` if body | 81,661 | 711 |
| `width += expect ? ... : 0;` ternary | 81,661 | 711 |
| single-expression `GetTextWidth(name) + (expect ? ... : 0)` | 81,661 | 711 |
| `String emptyBox; drawBox(..., emptyBox)` | 246,465 | 12,277 |
| `String twoSpaces("  "); ... role + twoSpaces` | 245,893 | 12,284 |
| `GetTextWidth(..., role)` (drop the temp) | 202,966 | 11,766 |
| `if (agent->isKnown())` (drop the expect) | 243,629 | 1,417 |

The landing-pad split is not controlled by braces, ternaries or statement
form; it is a Clang cleanup-scope artifact around the `if` body's temporary
`String("  ")`.  No size-exact shape found that makes `%134` reuse `%238`.

### 3.3 Body-layout records (690 B)

* `LevelScript::process` (187 B, 46/46 rows): every row start and length
  moves because the 57,232-byte body is laid out differently.
* `Level::createMission` (159 B): 55 lp-only rows are uniform downstream
  shifts (-4/-6/-8/-10/-18); 23 body-only and 19 both.  Body layout.
* `HangarWindow::render` (93 B): the known row-39/40 cleanup-scope
  difference (`Status::getShip` in the name-only cleanup in the original,
  four-String cleanup in ours) plus rows 96-135 body shift.  Round 10's
  wave-160 shape (record 151 B) costs +549 raw; no cheaper shape found.
* `Level::createShip` (77 B): uniform -6 downstream LP shift; body layout.
* `Radar::draw` (88 B): rows 1-14 `otherLock` cold-block body shift.
* `MGame::OnTouchEnd` (42 B): rows 17-18 shift by 4; epilogue/tail
  placement.
* `Hud::draw` (35 B): uniform +2 LP shift downstream of the `float filled`
  VarDecl split.
* `Generator::createMission` (7 B): stack-frame/spill layout (frame is 8 B
  larger in ours), not a cleanup scope.
* `ModStation::ModStation` (2 B): row 3 `(390,30,922)` vs `(400,20,922)`;
  pre-RA CMOV copy and literal-pool order tie.

## 4. Surviving per-TU knobs on the wave-171 base

Each knob was removed/replaced in turn (CMakeLists property edited,
reconfigure, rebuild, whole-`.text` same-address diff):

| TU | knob | with | without | verdict |
|---|---|---:|---:|---|
| Radar | `-mllvm -disable-postra-machine-licm` | 81,661 | 81,663 | **worth 2 B** |
| Radar | `-Xclang -target-feature -Xclang +cheap-predicable-cpsr` | 81,661 | 81,665 | **worth 4 B** |
| Level | `-mllvm -enable-earlycse-memssa=false` | 81,661 | 81,662 | **worth 1 B** |
| Layout | `-Xclang -target-feature -Xclang +prof-unpr` | 81,661 | 81,662 | **worth 1 B** |
| LevelScript | `-mllvm -disable-postra-machine-sink` | 81,661 | 81,661 | **dead** |

The two Radar knobs are independent and additive (licm-only 81,665,
cpsr-only 81,663, both 81,661, neither 81,667).  All four knob settings
leave `.ARM.extab` at 711.  The wave-171 source changes did not absorb the
Radar/Level/Layout wins (unlike wave 169, where all four were dead).  The
LevelScript knob is still dead but harmless; removing it is not a byte win,
so it is left in place.

## 5. Flag sweeps

### 5.1 ARM subtarget features on the other eight TUs (128 cells)

Round 11 swept 32 `-Xclang -target-feature -Xclang <±feature>` cells on
`LevelScript.cpp` and `Radar.cpp` only.  The 16 non-neutral/representative
features (`±cheap-predicable-cpsr`, `±prof-unpr`, `±use-misched`,
`±vmlx-hazards`, `±avoid-partial-cpsr`, `±fuse-literals`, `±no-movt`,
`±no-neg-immediates`, `±lob`, `±ret-addr-stack`, `±slow-odd-reg`,
`±slow-fp-brcc`, `±fpao`, `±neon-fpmovs`, `±dont-widen-vmovs`,
`±prefer-vmovsr`) were swept on `Level`, `HangarWindow`, `MGame`, `Hud`,
`SpaceLounge`, `Generator`, `ModStation` and `Layout`.  Every cell is
exactly neutral at 81,661/711 except:

| TU | feature | `.text` | extab | verdict |
|---|---|---:|---:|---|
| Level | `+use-misched` | 930,652 | 125,419 | size/layout mover |
| Level | `+vmlx-hazards` | 922,030 | 2,036 | size/layout mover |
| Level | `+neon-fpmovs` | 91,039 | 939 | size/layout mover |
| Level | `+cheap-predicable-cpsr` | 883,997 | 1,908 | size/layout mover |
| SpaceLounge | `+use-misched` | 281,304 | 12,906 | size/layout mover |
| SpaceLounge | `+vmlx-hazards` | 251,944 | 2,069 | size/layout mover |
| SpaceLounge | `+cheap-predicable-cpsr` | 88,390 | 1,097 | size/layout mover |
| Layout | `+use-misched` | 771,594 | 99,012 | size/layout mover |

No size-exact text or extab win.  The ARM feature family is exhausted.

### 5.2 Post-RA / EarlyCSE knob family on all ten TUs (27 cells)

`-mllvm -enable-earlycse-memssa=false`, `-mllvm -disable-postra-machine-sink`
and `-mllvm -disable-postra-machine-licm` were swept on all ten residual
TUs (skipping the TU that already carries the same flag).  Neutral cells:
LevelScript/earlycse, HangarWindow/all three, Radar/earlycse, MGame/all
three, Hud/all three, Generator/all three, ModStation/all three,
Layout/sink.  Text-worse cells: Level/sink 81,899/722, Level/licm
907,130/2,039, Radar/sink 439,179/1,483, SpaceLounge/earlycse 81,685/711,
SpaceLounge/sink 271,938/2,534, Layout/earlycse 815,072/1,914,
Layout/licm 82,273/712, LevelScript/licm 357,511/711.  No win.

### 5.3 SpaceLounge branch-weight ratios (10 cells)

The checked-in `7:1` per-file weight is best.  3:1 and 10:1 are exactly
neutral; 2:1/4:1/5:1/6:1/8:1/12:1 are 82,394/716; 15:1 is 81,664/715;
20:1 is 82,397/720.  The extab does not move below 711.

### 5.4 Radar ARM-feature pairings (8 cells)

`+cheap-predicable-cpsr` (the round-11 win) was paired with each of
`+avoid-partial-cpsr`, `+no-movt`, `+fuse-literals`, `+prof-unpr`,
`+slow-odd-reg`, `+no-neg-immediates`, `+lob`, `+ret-addr-stack` on
`Radar.cpp`.  All are exactly neutral at 81,661/711 except `+prof-unpr`
(82,346/711, worse).  No pairing improves on the checked-in single feature.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11720
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 _work/reconstructions/extab_record_map.py
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch artifacts: `/home/debian/GalaxyOnFire2/scratch/w11720/{secdiff.py,
knob.py,sweep.py,variant.py,classify.py,feat_cells*.txt,sl_cells*.txt,
knob_cells*.txt,rows_base.txt,classify_base.txt}`.

## 7. Files changed

* `_work/reconstructions/build-provenance-round12-extab-flags.md` — this note.
