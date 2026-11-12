# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13391

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13391`, base master `bea73c49b` (wave-190 base,
raw **3,244**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave190-w13373.md` (case-183 switch, tail and
case-2/4 order landed). Scope this pass: the `0x1400-0x1a00` case-body
schedules and the `0x3200-0x3e00` callee-saved rotation.

## 1. Result — landed

```text
                                     size            raw    aligned  insn
base bea73c49b                       17172/17172     3244   1156     6210/6197
landed                               17172/17172     3217   1150     6210/6197
```

Raw **3,244 -> 3,217** (-27), aligned **1,156 -> 1,150** (-6), size exact
17,172 B. `.text` differing bytes **73,250 -> 73,223** (-27);
`.ARM.exidx` 0, `.ARM.extab` 606 unchanged, `.note.gnu.build-id` unchanged.
Total (text+extab+build-id) **73,875 -> 73,848** (-27). `.rel.plt`
byte-identical, 27/27 headers, file size 2,187,328 exact.

Gate: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
`RATCHET PASS`. No baseline update.

## 2. The change: `case 1` route heights were swapped

The `case 1` body builds a nine-int route:

```cpp
int coords[9] = {rnd->nextInt(100000) - 50000, 0, 75000,
                 rnd->nextInt(100000) - 50000, 0, 50000,
                 rnd->nextInt(100000) - 50000, 0, 100000};
```

The original stores the **second** waypoint height as `50000` (`coords[2]`)
and the **third** as `75000` (`coords[5]`).  Evidence at `0xb14c2..0xb14ea`:

```text
0xb14c2  movw  r4, #50000
0xb14c6  subs  r0, r0, r4
0xb14c8  str   r0, [sp, #312]      ; coords[0]
0xb14ca  mov.w r8, #0
0xb14d4  strd  r8, r4, [sp, #316]  ; coords[1]=0, coords[2]=50000
...
0xb14e4  ldr   r2, [pc, #276]      ; literal 75000 @ 0xb15fc
0xb14ea  str   r2, [sp, #332]      ; coords[5]=75000
```

The base had `coords[2]=75000`, `coords[5]=50000`, i.e. a real semantic
error.  Swapping the two constants is the whole patch; no other source
change is needed.  It removes the 27-byte `case 1` prologue difference and
leaves the function size untouched.

## 3. `case 2` escort loop: `setAlwaysFriend` before the asteroid read

The original calls `player->setAlwaysFriend(true)` immediately after the
`createShip` store (`0xb18da..0xb18ea`), then reads the asteroid position
(`0xb18ee..0xb1904`).  The base read the asteroid first and called
`setAlwaysFriend` after `setPosition`.  Moving the call ahead of the
`asteroidPos = ...` statement is the second win:

```text
                                     size            raw    aligned
before                               17172/17172     3217   1150
after                                17172/17172     3136   1144
```

Raw **3,217 -> 3,136** (-81), size exact.  `.text` 73,223 -> 73,142 (-81).

A second small win in the same loop: swap `setSpeed(0.0f)` before
`setRotationSpeed(0.0f)` (the original emits the inlined speed store at
`0xb193c` before the virtual rotation call at `0xb194e`).  Raw
**3,136 -> 3,132** (-4), size exact, `.text` 73,142 -> 73,138.

## 4. Residual after the landing

The remaining raw 3,217 is still dominated by the `0x3200-0x3e00`
callee-saved rotation (`this` r4-vs-r8, `status` r8-vs-sl, `fortressGuns`
fp-vs-r6 in the raid loop; `this` r4-vs-r8, `gun` r6-vs-r4, `i` fp-vs-sl in
the alien-turret loop) and by the `case 1`/`case 2` body schedules.  The
case-183 switch still lays `case 2` before `case 1` (MBP), and the literal
pool order follows the emitted store order; no source-order permutation
tried this wave moved MBP without changing size.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13391
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3217 aligned=1150 size=17172/17172`, `.text` diff 73,223,
`.ARM.extab` diff 606, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13391/` (`dump.py`, `align.py`,
`runs.py`, `measure.sh`, `Level.base.cpp`).

## 6. Current-master harvest (wave 190)

Rebased onto master `f79cb3ef5` (the w13369/w13387/w13389 harvest) and
re-measured on the current tree:

```text
                                     size            raw    aligned  insn
master before                        17172/17172     3244   1156     6210/6197
harvested                            17172/17172     3132   1145     6210/6197
```

Raw **3,244 -> 3,132** (-112), aligned **1,156 -> 1,145** (-9), size exact
17,172 B. `.text` **73,233 -> 73,121** (-112); `.ARM.extab` 606 and
`.note.gnu.build-id` 20 unchanged; total (text+extab+build-id)
**73,859 -> 73,747** (-112). `.rel.plt` byte-identical, 27/27 headers,
file size 2,187,328 exact. `check` -> **RATCHET PASS** (byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370).

## 7. Follow-up harvest: attackerCount before array allocation

`c8315bc48` moves the `int attackerCount = ...` computation ahead of
`this->enemies = new Array<KIPlayer *>();` in the case-1 pirate body. Raw
**3,132 -> 3,055** (-77), aligned **1,145 -> 1,128**, size exact 17,172 B.
`.text` **73,121 -> 73,044** (-77); `.ARM.extab` **606 -> 603** (-3);
build-id 20 unchanged; total (text+extab+build-id) **73,747 -> 73,667**
(-80). `check` -> RATCHET PASS.

## 8. Follow-up harvest: case-183 descending offset store order

`b5bacf3d8` reorders the `case 183` convoy `offset[0..2]` stores to the
descending original order (`offset[0]` before `offset[1]`) in cases 0 and 1.
Raw **3,054 -> 3,044** (-10), aligned **1,129 -> 1,116**, size exact
17,172 B. `.text` **73,035 -> 73,025** (-10); `.ARM.extab` 603 and
build-id 20 unchanged; total (text+extab+build-id) **73,658 -> 73,648**
(-10). `check` -> RATCHET PASS.

## 9. Follow-up harvest: case-183 default store order

`a237f185f` moves the `default:` arm of the `case 183` convoy switch ahead
of `case 2:` and reorders the default stores (`offset[0]`, `offset[1]`,
volatile `offset[2]`). Raw **3,044 -> 3,037** (-7), aligned 1,116
unchanged, size exact 17,172 B. `.text` **73,025 -> 73,018** (-7);
`.ARM.extab` 603 and build-id 20 unchanged; total (text+extab+build-id)
**73,648 -> 73,641** (-7). `check` -> RATCHET PASS.
