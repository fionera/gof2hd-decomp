# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13502

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13502`, base master `015c180a3`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13493.md` (case-2 coords pair `0_2`)
and the w13495 case-2 coords note. Fresh lens: the **case-2 convoy loot
block**.

```text
                                     size            raw    aligned  insn
base 015c180a3                       17172/17172     2964   1124     6210/6197
landed                               17172/17172     2725    871     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,964 | **2,725** (-239) |
| aligned | 1,124 | **871** (-253) |
| insn | 6210/6197 | 6206/6197 |
| `.text` diff | 71,803 | **71,564** (-239) |
| `.ARM.extab` diff | 582 | **577** (-5) |
| `.rodata` diff | 485 | 485 |
| build-id diff | 20 | 20 |

`.rel.plt` byte-identical (not listed by `secdiff.py`), 27/27 headers,
`.ARM.exidx` 0, file size 2,187,328 exact. `check`: **RATCHET PASS**
(byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).

## 2. The change: case-2 convoy loot order and non-volatile `offset`

The original's second (convoy-ship) loop, `0xb18ac..0xb1990`, emits this
order after `setPosition`:

```
0xb1930  ldr  r0,[r5,#248]      ; this->enemies
0xb1934  ldr  r0,[r0,#4]        ; data
0xb1936  add  r0,r6             ; + (8 + goods*4)
0xb1938  ldr  r1,[r0,r8,lsl#2]  ; ship
0xb193c  str.w r9,[r1,#80]      ; ship->cargo = nullptr      (0x50)
0xb1940  strb.w r9,[r1,#76]     ; ship->crateCaptured() = 0  (0x4c)
0xb1944  ...                    ; reload ship
0xb194a  ldr  r2,[r1,#28]       ; ship->setSpeed(0)          (vtable +0x1c)
0xb1950  ...                    ; reload ship
0xb195e  blx  setRotationSpeed  ; ship->setRotationSpeed(0)
0xb196e  ...                    ; player->setHitpoints(max*3)
```

The source had `crateCaptured` first, then `setSpeed`, then `cargo`, then
`setRotationSpeed`, and used a `KIPlayer *convoyShip` local for both
`cargo` and `crateCaptured`.  Reordering to the binary's
`cargo -> crateCaptured -> setSpeed -> setRotationSpeed` and dropping the
local (each statement re-indexes `(*enemies)[convoyEnd + j]`, as the binary
does) is one half of the patch.

The other half is `volatile int offset` -> `int offset`.  With the reordered
block the volatile spill is no longer needed for size; non-volatile lets
clang keep `offset` in `r9` and `convoyEnd` in `fp` across the loop exactly
as the original does (`0xb17f8 ldr r9,[sp,#120]`, `0xb1886 cmp r8,fp`).
`volatile` with the new order is size-fatal (17,180 B).

## 3. Measurements (all raw/aligned; size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,964 | 1,124 | 17,172 |
| **cargo-first + drop local + non-volatile offset (landed)** | **2,725** | **871** | **17,172** |
| cargo-first + drop local, `volatile offset` | 10,399 | 959 | 17,180 |
| cargo-first + keep `convoyShip` local + non-volatile offset | 10,368 | 866 | 17,164 |
| cargo-first + drop `enemies` local + non-volatile offset | 10,028 | 873 | 17,180 |
| cargo-first + drop both locals + non-volatile offset | 10,056 | 895 | 17,180 |
| cargo-first + keep local, `volatile offset` | 10,410 | 957 | 17,180 |
| `crateCaptured`-first + non-volatile offset | 10,368 | 866 | 17,164 |

The landed shape is the only size-exact combination; it also lowers the
instruction-alignment diff by 253.

## 4. Residual after this pass

Case 2's first (escort) loop still differs: the source order is
`getRoute()->reachWaypoint(0)`, `setRoute(clone)`, `setAlwaysEnemy(true)`,
while the binary (`0xb183a..0xb187e`) is `setAlwaysEnemy(true)`,
`setRoute(clone)`, `getRoute()->reachWaypoint(0)`.  Applying that order alone
is size-exact but raw **2,993** / aligned **1,116** on the pre-pass base
(raw +29); it is not landed here.  The case-1 attacker loop still hoists
`enemyRace` into `r5` where the binary reloads `ldr r5,[sp,#116]` at
`0xb1608`.  Both remain for a future pass.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13502
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=2725 aligned=871 size=17172/17172`, `.text` diff 71,564,
`.ARM.extab` diff 577, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13502/`
(`bytediff.py`, `sbs.py`, `aligndist.py`, `secdiff.py`, `run_variants.py`,
`orig_createMission.s`, `ours_createMission.s`).
