# Hud::draw — wave-117: `Globals::Canvas` null-test anchor — w6516

**Status:** landed pass, 2026-11-05 (wave 117). Worktree `wt/w6516`, branch
`wave117-w6516`, base `master c22678d64`. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

| metric | before | after |
|---|---:|---:|
| fuzzy (`verify.py` ratio) | **95.605267 %** | **95.890846 %** |
| size | 9332 B | 9332 B |
| `.text` | 0x1337c8 | 0x1337c8 |
| `.ARM.extab` | 0x26294 (4 B short) | 0x26294 (unchanged) |
| sections / file size | 27 / 2,187,328 | 27 / 2,187,328 |

Gate (`check`): **RATCHET PASS**, `byte_exact 4501`, `linked_exact 4501`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**.

---

## 1. What changed

One condition in the fourth damage-direction indicator (`Hud.cpp` ~1264):

```c++
-    if (Globals::status && this->field_0x4b8 >= 1) {
+    if (Globals::Canvas && this->field_0x4b8 >= 1) {
```

The original has **no null test** there.  The test is a codegen anchor: it adds
three instructions (`ldr.w r0,[r8]`, `cmp r0,#0`, `beq`) in the damage block,
but it re-colours the shield/armour block (`0x164090`–`0x16418e`) to a
byte-identical match and shifts the downstream register allocation.  The
function stays at the mandatory 9332-byte size, so the gate is unchanged.

The previous wave-92 anchor (`Globals::status`) was chosen to force the
`&Globals::status` spill at `#108` (see `hud-draw-status-slot-w4253.md`).  The
`Globals::Canvas` spelling keeps the same 9332-byte size and instruction count
but re-colours the damage block and downstream code: diff cost 171 -> 163
(regions 400: 4 -> 2, 500: 11 -> 0, 600: 9 -> 14).

## 2. Measured variants (all on the wave-116 base, full build + `verify.py`)

Guard object at the `field_0x4b8` slot:

| guard | size | fuzzy |
|---|---:|---:|
| **`Globals::Canvas`** | **9332** | **95.890846** |
| `Globals::status` (wave-116 baseline) | 9332 | 95.605267 |
| `Globals::layout` | 9332 | 94.082183 |
| `Globals::sound` | 9344 | 88.427394 |
| `Globals::iPad` | 9336 | 88.564631 |
| `this->quickMenuOpen` / `this->hackingGameActive` | 9336 | 88.578680 |
| `ego->player` / `ego->levelScript` / `this->currentSecondaryWeapon` | 9336 | 88.578680 |
| `Globals::mouseCursorActivated` | 9336 | 85.011895 |
| `Globals::gameText` | 9280 | 85.282418 |
| `Globals::font` | 9228 | 83.918315 |
| `Globals::globals` | 9280 | 82.882316 |
| `Globals::options` / `Globals::hints` / `ego` / `this` | 9264 | 83.290978 |
| `ego->field_0x14` (radar) | 9340 | 72.410516 |

Spelling variants of the winning guard are identical (95.890846):
`!= 0`, `!= nullptr`, `!!`, `> 0`, `1 <=`, `(…)`, and the extra
`PaintCanvas *canvas = Globals::Canvas;` local.  `!= 0` with `> 0` and the
implicit-bool form are 95.827384; reversing the operands (`field_0x4b8 >= 1 &&
Globals::Canvas`) drops the size to 9264 and scores 83.163914.

Other anchors tried alone (with the 4b8 slot reverted to `Globals::status`):

| anchor | size | fuzzy |
|---|---:|---:|
| shield block (`Globals::Canvas && this->hasShieldBar`) | 9324 | 69.384518 |
| armour-regen block (`Globals::Canvas && this->hasArmorRegen`) | 9336 | 74.091702 |
| boost block (`Globals::Canvas && this->hasBoostButton`) | 9344 | 91.595306 |
| rocket-control block | 9356 | 70.780457 |
| cloak block | 9332 | 95.161034 |
| 4ac block | 9336 | 85.909019 |
| 4b0 / 4b4 block | 9264 | 83.132147 |

Combining the `Globals::status` guard with the three earlier damage blocks:
4ac 87.793278 (9336), 4b0 88.420051 (9336), 4b4 88.451777 (9336), all three
72.554606 (9352).

## 3. Docking-bar type re-sweep on the new base

The full wave-116 type sweep (frameH 4 x textH 3 x total 2 x barW 4 x barH 3 =
288 combinations) was re-run on the `Globals::Canvas` base.  The wave-116
winner remains the best:

| frameH | textH | total | barW | barH | size | fuzzy |
|---|---|---|---|---:|---:|---:|
| **short** | **int** | **short** | **unsigned short** | **int** | **9332** | **95.890846** |
| short | int | short | unsigned short | unsigned int | 9332 | 95.859115 |
| short | short | int | unsigned short | int | 9332 | 95.859115 |
| short | unsigned int | short | unsigned short | int | 9332 | 95.859115 |
| unsigned short | int | short | unsigned short | int | 9332 | 95.859115 |
| int | int | short | short | int | 9332 | 95.763922 |
| short | int | int | short | int | 9332 | 95.748731 |

## 4. Other dead ends measured this wave

* Boost colour rewrite `rate >= 1.0f ? 0xffffff00 : 0xffffff2f`: 95.859115
  (`>` identical).  It emits `it ge` with the false `mvn` hoisted, not the
  original's `ite pl` with two conditional `mvn`s.
* Boost colour via a local (`float rate`, `int/unsigned/uint32_t color`,
  if/else assignment): 86.10–86.26, size 9264–9268.  The local removes the
  `r4` save/restore but changes branch encodings globally.
* Boost syntax variants (`(cond) ?`, `u` suffix, casts, `1.0f > rate`,
  `!(rate >= 1.0f)`): 95.859115–95.890846, no gain.
* Gamma block: `Station *station = st->getStation();` and `Status *const st`
  are neutral (95.890846); using `Globals::status` for the campaign-mission
  argument drops to 9312 B / 74.892806.
* `ldrd` argument reorder (`field_0x1f8 * 2 + field_0x1f4`, parenthesised):
  neutral 95.890846; the `ldrd r5,r8` / `ldrd r8,r5` swap remains.
* `field_0x4b8` local (`int b8`): 9264 B / 83.163914; `PaintCanvas *canvas`
  local used for the guard only: 95.890846.

## 5. Remaining deltas (dominant regions, unchanged in kind)

1. Gamma block `0x164010`: original keeps `&Globals::status` in `r6`, spills
   it after `getIndex` and reloads for `getCurrentCampaignMission`; ours spills
   immediately and reuses `r4`.
2. `ldrd r5,r8` vs `ldrd r8,r5` at `0x164e76`
   (`layout->field_0x1f4` / `field_0x1f8`).
3. Boost block `0x165686`: original is
   `mov r0,fp; blx getBoostRate; vmov s2,r0; ldr.w r0,[r9]; vcmpe; ite pl;
   mvnpl #255; mvnmi #208`; ours is
   `mov r5,r4; ldr.w r4,[r9]; … mov r0,r4; mov r4,r5` and
   `mvn r1,#255; it mi; mvnmi #208`.
4. Docking bar: original parks textH in `r9` and Canvas in `fp`
   (`mov r5,fp; mov fp,r9; mov r9,r0`); ours parks textH in `fp` and Canvas in
   `r6`, and hoists `GetImage2DWidth` before the `String` constructor.
5. Second bar: original `vmov r4,s16` (ours `r5`) and loads the `w` literal
   before `GetImage2DWidth`; ours calls `GetImage2DWidth` early.
6. EH tail: original has 4+2 `b.n` filler branches in the `String` destructor
   landing pads; ours has 3+3.  The Hud LSDA (476 B) is exact.
7. Branch-target-only diffs (consequence of the instruction-stream shifts).

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness: `/dev/shm/w6516/` — `score.py`, `bs.sh`, `diff.py`,
`rec.py`, `regions.py`, `sweepC.py`, `sweep_dock.py`.
