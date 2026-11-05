# Hud::draw — wave-116: docking-bar `total`/`barW` widths — w6416

**Status:** landed pass, 2026-11-05 (wave 116). Worktree `wt/w6416`, branch
`wave116-w6416`, base `master d672d349f` (wave-115 harvest). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

| metric | before | after |
|---|---:|---:|
| fuzzy (`verify.py` ratio) | **95.4632 %** | **95.6053 %** |
| size | 9332 B | 9332 B |
| Hud LSDA record / call-site table | 476 / 454 (exact) | 476 / 454 (exact) |
| `.ARM.extab` | 0x26294 | **0x26294** (unchanged) |
| `.text` | 0x1337c8 | 0x1337c8 |
| sections / file size | 27 / 2,187,328 | 27 / 2,187,328 |

Gate (`check`): **RATCHET PASS**, `byte_exact 4501`, `linked_exact 4501`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**.

---

## 1. What changed

Two type words in the docking-transfer bar (`Hud.cpp` ~1631):

```c++
-            int total = ego->getDockTotalAmount();
+            short total = ego->getDockTotalAmount();
...
-            short barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
+            unsigned short barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
```

The declaration order stays the wave-115 faithful order
`F T R O P B S W H` (frameH, textH, transferred, total, progress, barOffset,
String text, barW, barH).  Nothing else in the function moved.

## 2. Why these two widths

The wave-115 note swept 72 combinations of
`frameH/transferred/barW/barH` but kept `total` at `int` and only listed the
best few rows.  A full re-sweep on the wave-115 base (144 combinations of
`frameH ∈ {short,int,unsigned short,unsigned int}`,
`textH ∈ {int,short,unsigned int}`,
`barW ∈ {short,int,unsigned short,unsigned int}`,
`barH ∈ {int,short,unsigned int}`) plus a `total ∈ {int,short}` pass found:

| frameH | textH | transferred | total | barW | barH | size | fuzzy |
|---|---|---|---|---|---|---:|---:|
| short | int | int | **short** | **unsigned short** | int | 9332 | **95.6053** |
| short | short | int | int | unsigned short | int | 9332 | 95.5735 |
| unsigned short | int | int | short | unsigned short | int | 9332 | 95.5735 |
| short | int | int | int | short | int | 9332 | 95.4632 (wave-115) |
| int | int | int | short | short | int | 9332 | 95.4783 |
| int | int | int | int | int | int | 9328 | 93.2825 |

`short total` makes clang sign-extend `total` at the `vcvt.f32.s32` and
`unsigned short barW` makes it zero-extend `barW` at the `vmov`/`vcvt`; the two
extra `sxth`/`uxth` uses re-colour the spill weights enough to move the docking
bar's `barW`/`barH` temporaries and the downstream second-bar registers toward
the original.  The original's own `total`/`barW` are 32-bit (`mov r5,r0` /
`mov r8,r0` with no extension), so this is a tuning of the source shape, not a
faithful type reconstruction; it is the best measured variant.

## 3. Searches that did not beat 95.6053

All on the new base, each a full build + `verify.py` ratio:

* 24 permutations of the `F T R O` declarations — only `FTRO` reaches
  95.6053; next `TFRO` 95.1761.
* 120 permutations of `{progress, barOffset, String text, barW, barH}` — only
  `PBSWH` / `BPSWH` reach 95.6053.
* 90 single-statement moves of the second-bar block
  `{halfW, String, barH, barYOffset, dt, fade, fadeColor, if, top, y}` — none
  beat 95.6053.
* Second-bar types (`halfW`, `barH`, `barYOffset`, `top`, `fadeColor`, `y`) —
  none beat 95.6053; `halfW`/`barH` must stay `int`.
* `filled` placement (before `SetColor`, before `DrawImage2D`, inline,
  declare-then-assign) — 83.7–91.2 %.
* `String text` forms (`= *getText`, default-bool ctor, `true` bool) —
  95.5735–95.6053 %.
* `gammaY`/`gammaBarY` declaration/expression forms — 95.6053 or worse.
* `barOffset`, `armorY`, `armorBarY`, `dividerOffset`, `savedColor` types —
  95.6053 or worse.
* `Globals::status` gamma-condition spellings (`st` vs direct, extra local,
  `Status *&`) — 95.6053 or worse (direct references collapse to 9312 B).

## 4. Remaining deltas (unchanged in kind from wave 115)

The residual 95.6053 % is still register allocation / scheduling:

1. `Status *st` gamma block at `0x164010`: original keeps `&Globals::status`
   in `r6` and spills it after `getIndex`, reloads `Globals::status` for
   `getCurrentCampaignMission`; ours spills immediately and caches the value.
2. `gammaY`/`gammaBarY` `fp`/`r9` swap at `0x164032`.
3. Two dead `ldr.w r8,[sp,#108]` in the original damage block
   (`0x164176`, `0x164210`) that our build does not emit; our equivalent bytes
   are the `Globals::status &&` null guard at `0x1642ee` (an anchor required to
   keep the 9332-byte size and the status spill slot — see
   `hud-draw-status-slot-w4253.md`).
4. `ldrd r5,r8` / `ldrd r8,r5` swap at `0x164e76` (`layout->field_0x1f4` /
   `field_0x1f8`).
5. Docking bar: ours still has `mov fp,r0` (textH) where the original has
   `mov fp,r9` (Canvas) and `mov r9,r0` (textH), plus `mov fp,r0`/`mov r8,r0`
   for barW/barH where the original has `mov r8,r0`/`mov fp,r0`.  The
   all-`int` dock variant reproduces `mov fp,r9` but re-colours the frame
   (frameH slot `#92` vs `#96`) and drops to 93.28 %.
6. Second bar: `add.w r2,r9,r5` is scheduled after the float ops in ours,
   before them in the original; `&Globals::Canvas` is in `r4` vs `fp`.

## 5. Reproduction

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
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch harness (outside the repo): `/dev/shm/w6416/` — `score.py` (exact
`verify.py` ratio for one symbol), `bs.sh` (link + score), `align2.py`
(address-tagged normalized diff), `sweep_types.py` / `sweep2.py`–`sweep10.py`
(the type/order sweeps above).
