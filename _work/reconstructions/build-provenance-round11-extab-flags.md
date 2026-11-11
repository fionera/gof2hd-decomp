# Build provenance round 11 (w11620): the 306-row `.ARM.extab` census on the
# wave-170 base, the surviving per-TU knobs, and the open flag families

Status: **one landed `CMakeLists.txt` change** (2026-11-11).
Worktree `wt/w11620`, branch `wave171-w11620`, base master `50716a763`
(wave 170).  Scratch `/dev/shm/w11620`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry: **RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact
4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  `.text` **81,825** differing bytes, `.ARM.extab` **711**,
`.rel.plt` **0** (byte-identical), build-id 20, total **82,556**.

Gate on exit: **RATCHET PASS** with the same counts; `.text` **81,821**
(-4), `.ARM.extab` **711** (unchanged), `.rel.plt` 0, total **82,552**.

## 0. Bottom line

1. **Landed:** `Radar.cpp` `-Xclang -target-feature -Xclang +cheap-predicable-cpsr`.
   Size-exact (9,908 B), every non-target function and every alloc section
   unchanged, `.text` 81,825 -> **81,821** (-4), `.ARM.extab` 711 (neutral).
   `Radar::draw` raw 7,095 -> **7,091**, aligned 1,595 -> **1,592**.
   Found by the round-11 ARM subtarget-feature sweep (64 cells).
2. The wave-170 `.ARM.extab` residual is **711 bytes in 11 records / 306
   differing row tuples** (down from 1,292 / 313 at wave 169: the HangarWindow
   LSDA fix removed 581 bytes and 7 rows).  The full per-record census and
   row-level classification are in sections 2-3.
3. **Eight records (695 B) are body-layout consequences**: the row geometry
   moves because the non-byte-exact bodies are laid out differently.  They
   cannot be fixed without fixing the corresponding `.text`, and the two
   largest (`LevelScript::process` 187 B, `Level::createMission` 159 B) are
   pure body-layout records.  `Generator::createMission` (7 B, counted
   separately) is a stack-frame/spill-layout consequence of the same kind.
4. **One record is a pure landing-pad identity tie** (`Radar::Radar`, 7 B):
   the `.text` is already byte-identical (`raw 0`), only the call-site→pad
   assignment differs.  It is a machine-block-placement tie among three
   identical `b.n` trampolines; no source scope, `__builtin_expect`, loop
   reorder, or block-frequency/BPI/MBP knob flips it.
5. **One record is a scheduling tie** (`ModStation::ModStation`, 2 B): the
   pre-RA CMOV copy sits after the `cmp` in ours and before it in the
   original.  No source shape tested moves it.
6. **The surviving per-TU knobs are still worth their bytes** on the wave-170
   base: `Radar.cpp -disable-postra-machine-licm` **2 B**, `Level.cpp
   -enable-earlycse-memssa=false` **1 B**, `Layout.cpp +prof-unpr` **1 B**.
   The `LevelScript.cpp -disable-postra-machine-sink` knob is now **dead**
   (neutral after the wave-170 LevelScript changes) but harmless.
7. **70 link-measured block-frequency/if-conversion/ARM/branch-weight cells
   found no size-exact text win.**  The only extab movers are `LevelScript`
   10:1 branch weights (extab 705, -6, but `.text` +276,281) and `Radar`
   80:1 (extab 710, -1, `.text` +13,957).  The win came from the separate
   64-cell ARM subtarget-feature sweep (`+cheap-predicable-cpsr`).

## 1. Baseline (this worktree)

| metric | value |
|---|---:|
| `.text` differing bytes | 81,825 |
| `.ARM.extab` differing bytes | 711 |
| `.rel.plt` differing bytes | 0 |
| build-id differing bytes | 20 |
| total differing bytes | 82,556 |
| byte_exact / linked_exact | 4507 / 4507 |
| size_exact | 4524 / 4524 |
| data_exact | 843 / 843 |
| imports | 207 |

Per-function raw same-address diffs of the 11 extab-record owners (entry
baseline; `Radar::draw` is 7,091 after the landed flag):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | 53,933 | 17,900 | 57,232 |
| `Radar::draw` | 7,095 | 1,595 | 9,908 |
| `Level::createMission` | 5,441 | 778 | 17,172 |
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
function start.  "diff rows" counts tuples that differ; "lp-only" is the
subset where only the landing-pad identity differs (start/length identical);
"body-only" is the subset where only start/length differ; "both" is both.

| record | owner | len | rows | diff | lp-only | body-only | both |
|---|---|---:|---:|---:|---:|---:|---:|
| 0x137d18 | `LevelScript::process` | 348 | 46 | 46 | 0 | 19 | 27 |
| 0xaffd8 | `Level::createMission` | 956 | 148 | 97 | 55 | 23 | 19 |
| 0x1490d8 | `HangarWindow::render` | 948 | 156 | 48 | 4 | 27 | 17 |
| 0xc0214 | `Level::createShip` | 336 | 57 | 48 | 10 | 23 | 15 |
| 0x12e050 | `Radar::draw` | 132 | 19 | 16 | 3 | 5 | 8 |
| 0x17a144 | `MGame::OnTouchEnd` | 280 | 44 | 16 | 0 | 13 | 3 |
| 0x163b90 | `Hud::draw` | 476 | 77 | 11 | 5 | 4 | 2 |
| 0x170d38 | `SpaceLounge::drawLounge` | 168 | 25 | 10 | 6 | 2 | 2 |
| 0x971b8 | `Generator::createMission` | 172 | 26 | 6 | 6 | 0 | 0 |
| 0x12d484 | `Radar::Radar` | 256 | 39 | 7 | 7 | 0 | 0 |
| 0xd52d0 | `ModStation::ModStation` | 112 | 15 | 1 | 0 | 1 | 0 |
| **total** | | | | **306** | **96** | **117** | **93** |

The row-tuple dumps are reproducible with the decoder in
`_work/reconstructions/extab_lsda_decode.py` and `extab_record_map.py`.

## 3. Per-record analysis

### 3.1 `Radar::Radar` (7 B, pure pad identity; `.text` byte-identical)

The `.text` is already exact (`raw 0`, `aligned 0`, 2,488 B, 812 insns).
The four cleanup pads at the function tail are:

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

The IR has four landing-pad blocks that all `landingpad; br %786`
(`%175` function-level, `%304` playerGuns loop, `%372` Image2DCreate
calls, `%376` enemyGuns loop); `%786` destroys the member String at
`this+0x18c`.  The row assignment is:

| rows | region | orig LP | ours LP |
|---|---|---:|---:|
| 1,3,11,13,15 | labelStrings / `new Array` / `getLandmarks` | 2462 | 2464 |
| 19,21 | playerGuns loop + Image2DCreate | 2464 | 2462 |
| 20 | enemyGuns loop | 2466 | 2466 |
| 39 | (different pad) | 2460 | 2460 |

So `%175` and `%304` are swapped in the final block order; the enemyGuns
pad is third in both.  This is a `MachineBlockPlacement` tie among identical
cold blocks, not a cleanup-scope difference.

Source shapes measured (all size-exact 2,488 B):

| shape | `.text` | extab | raw | aligned |
|---|---:|---:|---:|---:|
| base | 81,825 | 711 | 0 | 0 |
| drop `{ }` around labelStrings[0] | 81,825 | 711 | 0 | 0 |
| wrap the `inAlienOrbit` `if` in `{ }` | 81,825 | 711 | 0 | 0 |
| `__builtin_expect(playerGuns != nullptr, 1)` | 81,882 | 733 | 57 | 21 |
| `__builtin_expect(enemyGuns != nullptr, 1)` | 81,868 | 729 | 43 | 17 |
| both null-check expects | 81,950 | 733 | 125 | 88 |
| expect on both loop conditions | 81,878 | 733 | 53 | 19 |
| swap the two loops | 83,501 | 765 | 1,676 | 122 |
| move the `inAlienOrbit` block after labelStrings[1..3] | 86,721 | 31,622 | 139 | 50 |

No shape reduces the 7-byte record without moving `.text`.  This confirms
the wave-147/148/round-9 conclusion: the tie is a block-frequency /
block-placement decision, and the branch-weight / BPI / MBP knob families
do not break it.

### 3.2 `Generator::createMission` (7 B, frame-layout consequence)

All six differing rows are lp-only, but the LPs are not a uniform shift:
rows 10/13/14/23 are ours = orig - 2, row 17 is orig 1932 vs ours 1912
(-20), row 24 is orig 1888 vs ours 1892 (+4).  The body is 127 raw bytes
different because the stack frame is **8 bytes larger** in ours
(`sub sp,#96` vs `sub sp,#104`): the three String slots are at
`sp+44/56/80` (orig) and `sp+48/60/88` (ours), ours has one extra stack
access (63 vs 62), and the `amount`/`itemId` spill slot is `sp+16` in the
original but `sp+84` in ours.  One float-to-int conversion is also selected
differently (`it ne; subne r5,r6,r0` in the original vs `nrme f3,#1.0;
subs r6,r5,r0` in ours).  This is a register-allocation/instruction-selection
difference, not a cleanup-scope difference.  Source shapes tested (all
size-exact 1,996 B): `bool selected` -> `int` (raw 131), `uint32_t type` ->
`int` (127), swap `agentName`/`difficulty` declarations (127), move
`itemId`/`amount` into the type branches (1,085).  Base is best.

### 3.3 `SpaceLounge::drawLounge` (14 B)

Six lp-only rows: rows 1/5/7/9 are ours = orig + 2 (identical adjacent
`b.n` pads 1868/1870 and 1874/1876), row 10 is a swap (orig 1878, ours
1868).  Rows 14-17 are the body shift in the conversation panel.  The
right-edge clamp form is the wave-106 scoring artifact; the faithful forms
measured worse:

| clamp form | `.text` | extab | raw | size |
|---|---:|---:|---:|---:|
| base `+ (-width - pad - pad)` | 81,825 | 711 | 175 | 1,948 |
| `- (width + pad + pad)` | 81,826 | 711 | 176 | 1,948 |
| `- (pad + pad + width)` | 82,095 | 723 | 445 | 1,948 |
| `- width - pad - pad` | 83,436 | 730 | 468 | 1,944 |

### 3.4 `Hud::draw` (35 B)

Five lp-only rows (6/9/18/21/57) are uniformly ours = orig - 2, downstream
of the `float filled` VarDecl split / body shift documented in
`hud-draw-extab-lsda-w4273.md`.  The tail rows 68-74 are the dock-block
body difference.  The known dock-block reorder grows the LSDA call-site
table by 12 bytes (77 -> 79 entries) and shifts `.rodata`, so it fails the
gate.

### 3.5 `Level::createShip` (77 B)

Ten lp-only rows (31/33/35/37/39/41/43/45/47/49) are all ours = orig + 6:
a uniform downstream shift of the tail pad table, not a pad-identity swap.
Rows 0-30 are the body shift (the prologue alone differs: `str r1,[sp,#60]`
+ `mov fp,r3` vs `strd r1,r2,[sp,#56]` + `mov sl,r3`).  Body layout.

### 3.6 `Level::createMission` (159 B)

55 lp-only rows with deltas -4/-6/-8/-10/-18, plus 23 body-only and 19 both.
The LPs track the body shift (first half -4, tail -6/-8).  Body layout.

### 3.7 Body-layout records

* `LevelScript::process` (187 B, 46/46 rows): every row start and length
  moves because the 57,232-byte body is laid out differently.  Largest
  single record; pure body-layout consequence.
* `Radar::draw` (88 B): rows 1-14 shift by 2-30 bytes (the `otherLock`
  cold-block region); 3 lp-only rows with -8/-2.
* `HangarWindow::render` (93 B): rows 40-44 are the ship-branch cleanup
  split; rows 96-135 are the credit-loop body shift.  The wave-170 w11514
  shape already reproduces the original row-39 split (record 676 -> 93).
* `MGame::OnTouchEnd` (42 B): rows 17-18 shift by 4; rows 30-43 are the
  epilogue/tail 4-cycle placement.
* `ModStation::ModStation` (2 B): row 3 is `(390,30,922)` orig vs
  `(400,20,922)` ours.  The pre-RA CMOV copy (`mov r0,r3`) sits before the
  `cmp` in the original and after it in ours; the literal-pool order is
  also swapped (`Pitch,YawIPad,Yaw` vs `Pitch,Yaw,YawIPad`).  Both are
  selection/scheduling ties (see `modstation-extab-lp-wave149-w9520.md`).

## 4. Landed change: `Radar.cpp +cheap-predicable-cpsr`

`CMakeLists.txt`, Radar.cpp property:

```cmake
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/weapons/Radar.cpp"
    PROPERTIES COMPILE_OPTIONS
      "-mllvm;-disable-postra-machine-licm;-Xclang;-target-feature;-Xclang;+cheap-predicable-cpsr")
```

| metric | before | after |
|---|---:|---:|
| `Radar::draw` raw | 7,095 | **7,091** |
| `Radar::draw` aligned | 1,595 | **1,592** |
| `Radar::draw` size | 9,908 | 9,908 |
| `Radar::Radar` raw / size | 0 / 2,488 | 0 / 2,488 |
| whole `.text` diff | 81,825 | **81,821** |
| whole `.ARM.extab` diff | 711 | 711 |
| `.rel.plt` / `.rodata` / `.data` / `.bss` | 0 | 0 |
| gate byte_exact / linked_exact | 4507 / 4507 | 4507 / 4507 |

The ARM `cheap-predicable-cpsr` subtarget feature makes the predication
cost model prefer a conditional move in one `Radar::draw` block, moving
four bytes into agreement without changing any instruction count or size.
It is the only non-neutral cell in the 64-cell subtarget sweep (32 features
x {`LevelScript`, `Radar`}); `Radar +prof-unpr` is text-worse (94,924),
`Radar +use-misched` / `+vmlx-hazards` are size movers, and every other
feature is exactly neutral at 81,825 / 711.

## 5. Surviving per-TU knobs on the wave-170 base

Each knob was removed in turn (CMakeLists property commented out,
reconfigure, rebuild, whole-`.text` same-address diff):

| TU | knob | with | without | verdict |
|---|---|---:|---:|---|
| Radar | `-mllvm -disable-postra-machine-licm` | 81,825 | 81,827 | **worth 2 B** |
| Level | `-mllvm -enable-earlycse-memssa=false` | 81,825 | 81,826 | **worth 1 B** |
| Layout | `-Xclang -target-feature -Xclang +prof-unpr` | 81,825 | 81,826 | **worth 1 B** |
| LevelScript | `-mllvm -disable-postra-machine-sink` | 81,825 | 81,825 | **dead** |

All four knob settings leave `.ARM.extab` at 711.  The wave-170 LevelScript
rewrites absorbed the 2-byte win the knob used to provide; removing it is
neutral but not a byte win, so it is left in place.

Combination checks (Radar + a second knob, Level + a second knob, Layout +
a second knob) were all neutral or worse:

| base TU | added knob | `.text` | extab |
|---|---|---:|---:|
| Radar | `-enable-earlycse-memssa=false` | 81,825 | 711 |
| Radar | `-disable-ifcvt-triangle-rev` | 81,825 | 711 |
| Radar | `-machine-sink-split-probability-threshold=0` | 95,619 | 711 |
| Level | `-disable-ifcvt-triangle-rev` | 81,837 | 711 |
| Level | `-machine-sink-split-probability-threshold=0` | 818,904 | 1,490 |
| Layout | `-disable-postra-machine-sink` | 81,825 | 711 |
| Layout | `-disable-ifcvt-triangle-rev` | 81,825 | 711 |
| Layout | `-machine-sink-split-probability-threshold=0` | 81,825 | 711 |

The per-file 7:1 branch-weight overrides on `StarMap.cpp`/`SpaceLounge.cpp`
are also still essential: with `GOF2_NO_PER_FILE_WEIGHTS=ON` the whole
`.text` diff jumps to 801,081 and `.ARM.extab` to 1,434 (the `.text` grows
32 B, so every later function shifts).

## 6. Flag sweeps

### 6.1 Block-frequency / if-conversion / ARM / branch-weight (70 cells)

Ten residual TUs x seven flags.  Baseline 81,825 / 711.  Every cell was
link-measured with the real gold link; the closest cells:

| TU | flag | `.text` | extab |
|---|---|---:|---:|
| LevelScript | `-mllvm -disable-ifcvt-triangle-rev` | 81,825 | 711 |
| LevelScript | `-mllvm -arm-use-mulops` | 81,825 | 711 |
| LevelScript | `-mllvm -use-mbpi` | 81,825 | 711 |
| LevelScript | `-mllvm -tail-dup-placement` | 81,825 | 711 |
| LevelScript | `-mllvm -machine-sink-split-probability-threshold=0` | 81,826 | 711 |
| LevelScript | `-mllvm -likely-branch-weight=10 -mllvm -unlikely-branch-weight=1` | 358,106 | **705** |
| LevelScript | `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 82,425 | 711 |
| Level | `-mllvm -disable-ifcvt-triangle-rev` | 81,837 | 711 |
| Level | `-mllvm -machine-sink-split-probability-threshold=0` | 818,904 | 1,490 |
| Radar | `-mllvm -machine-sink-split-probability-threshold=0` | 95,619 | 711 |
| Radar | `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 95,782 | **710** |
| HangarWindow | `-mllvm -disable-ifcvt-triangle-rev` | 81,882 | 711 |
| SpaceLounge | `-mllvm -disable-ifcvt-triangle-rev` | 82,591 | 740 |
| Generator | `-mllvm -disable-ifcvt-triangle-rev` | 81,827 | 711 |
| Generator | `-mllvm -machine-sink-split-probability-threshold=0` | 987,313 | 1,402 |
| ModStation | `-mllvm -machine-sink-split-probability-threshold=0` | 82,505 | 729 |

All other cells (Layout, Hud, MGame, and the remaining flags on the TUs
above) are exactly neutral at 81,825 / 711.  The two extab movers are not
text-neutral.  This extends the round-9 result (6,565 object-level cells,
no text win) to the wave-170 base.

### 6.2 ARM subtarget features (64 cells)

32 `-Xclang -target-feature -Xclang <±feature>` cells on each of
`LevelScript.cpp` and `Radar.cpp` (`±prof-unpr`, `±use-misched`,
`±avoid-partial-cpsr`, `±ret-addr-stack`, `±lob`, `±fuse-literals`,
`±no-movt`, `±prefer-vmovsr`, `±slow-odd-reg`, `±slow-fp-brcc`, `±fpao`,
`±neon-fpmovs`, `±dont-widen-vmovs`, `±vmlx-hazards`, `±no-neg-immediates`,
`±cheap-predicable-cpsr`).  Non-neutral cells:

| TU | feature | `.text` | extab | verdict |
|---|---|---:|---:|---|
| Radar | `+cheap-predicable-cpsr` | **81,821** | 711 | **landed (-4)** |
| Radar | `+prof-unpr` | 94,924 | 711 | worse |
| Radar | `+use-misched` | 453,027 (size +192) | 1,965 | size mover |
| Radar | `+vmlx-hazards` | 436,942 (size +32) | 1,396 | size mover |
| LevelScript | `+use-misched` | 424,213 (size -64) | 1,695 | size mover |
| LevelScript | `+vmlx-hazards` | 409,132 (size +32) | 1,397 | size mover |

All other 58 cells are exactly neutral at 81,825 / 711.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11620
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

Scratch artifacts: `/dev/shm/w11620/{measure.py,rows.py,classify.py,
sbs.py,knob.py,sweep.py,sweep_focus.py,sweep_feat.py,runexp.py}`.

## 8. Files changed

* `CMakeLists.txt` — add `+cheap-predicable-cpsr` to the Radar.cpp property.
* `_work/reconstructions/build-provenance-round11-extab-flags.md` — this note.
