# Hud::draw — wave-103 oracle pass: second-bar String-before-width order

**Status:** code change + analysis, 2026-11-04 (wave 103). Worktree `wt/wf315`, branch
`wave103-wf315`, base `master 1d2b4398e`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp:1091`), original size **9332 B** (Ghidra `Hud_draw` @ 0x173b90,
file 0x163b90).

Gate on entry and on exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, lint CLEAN, drift 0, **RATCHET PASS**.

Official per-function score: **92.1 % -> 93.2 %** at the unchanged 9332-byte size;
Hud::draw's `.ARM.extab` entry 476 -> 468 B (77 -> 76 LSDA rows), section
`0x26298 -> 0x26290`, `.rodata` base unchanged (`0x1c9e10`).

This note extends `hud-draw-gamma-store-dock-order-w4515.md`,
`hud-draw-dock-order-w4415.md` and `hud-draw-extab-lsda-w4273.md` with the wave-103
third-party Ghidra oracle
(`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Hud.c`,
`Hud::draw` at line 2411, dock block lines ~3550-3720).

---

## 0. TL;DR

1. **The original second bar constructs the `String` before querying the bar width.**
   The oracle (`Hud.c` ~line 3630) reads
   `getText(...) -> String(aAStack_54, pSVar20, false) -> GetImage2DWidth -> GetImage2DHeight`,
   and the original binary agrees: `StringC1ERKS0_b` at `0x165bc8`, `GetImage2DWidth`
   at `0x165bd4`, `GetImage2DHeight` at `0x165bf4`. HEAD had `halfW = GetImage2DWidth`
   *before* `String text`, which is wrong. Moving the `String` declaration first is
   worth **+0.9 pt** on its own (92.1 -> 93.0) and is landed here.
2. **The oracle's dock statement order is `P B T W H D`** (progress, barOffset,
   `String text`, barW, barH, dt/fade). That faithful order measures **94.1 %** but
   is 9324 B (loses size-exactness) and its `.ARM.extab` entry is 480 B (78 rows),
   which pushes `.rodata` to `0x1c9e20` and drops byte_exact to 4033. It is *not*
   landable without a separate extab shave.
3. **The landed shape is `P B W H T D` with `fontId`/`halfW`/`textY` inlined**
   (`F0H0Y0`): **93.2 %**, 9332 B, extab 468 B (76 rows), `.rodata` untouched.
   This is the best gate-safe shape that keeps the oracle's second-bar fix; the
   best gate-safe *tuned* shape without the fix is `W T P B H D` at 93.7 %.
4. **The extab budget is exactly 8 bytes of headroom** in the current build:
   `.ARM.extab` is `0x26290`, `.rodata` is 16-aligned at `0x1c9e10`, so the section
   may grow to `0x26298` (= Hud entry 476 B). Hud entry 480 B -> section `0x2629c`
   -> `.rodata 0x1c9e20` -> byte_exact 4033. Any future Hud::draw shape must keep
   the entry at **<= 476 B**.

---

## 1. Ground truth: the second bar

Original `Hud::draw` second bar, 0x165b52..0x165d3c:

```
165bbe  blx  GameText::getText
165bc2  mov  r1, r0
165bc4  add  r0, sp, #208          ; String text
165bc6  movs r2, #0
165bc8  blx  String::String(String const&, bool)
165bcc  ldr.w r1, [sl, #892]       ; image_0x37c
165bd0  ldr.w r0, [r9]             ; Globals::Canvas
165bd4  blx  PaintCanvas::GetImage2DWidth
165bd8  vmov s2, r0
165bdc  vmov.f32 s0, #96           ; 0.5
165be0  vcvt.f32.s32 s2, s2
165be4  vmul.f32 s0, s2, s0
165be8  vcvt.s32.f32 s18, s0       ; halfW = (int)(width * 0.5f)
165bec  ldr.w r1, [sl, #892]
165bf0  ldr.w r0, [r9]
165bf4  blx  PaintCanvas::GetImage2DHeight
165bf8  mov  r8, r0                ; barH
```

The oracle renders the same order:

```c
pSVar20 = (String *)GameText::getText(Globals::gameText,iVar8);
AbyssEngine::String::String((String *)aAStack_54,pSVar20,false);
uVar11 = AbyssEngine::PaintCanvas::GetImage2DWidth(Globals::Canvas,*(uint *)(this_01 + 0x31c));
fVar10 = (float)uVar11;
iVar9 = AbyssEngine::PaintCanvas::GetImage2DHeight(Globals::Canvas,*(uint *)(this_01 + 0x31c));
```

HEAD instead emitted `GetImage2DWidth` before `String::String` (the source had
`int halfW = ...; String text(...);`). The landed source is

```cpp
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

---

## 2. Ground truth: the dock statement order

The oracle's first-bar block (`Hud.c` ~line 3580) executes

```
frameH      = GetImage2DHeight(image_0x37c)
textH       = GetTextHeight(font)
transferred = getDockTransferedAmount()
total       = getDockTotalAmount()
getText(3205/3204)
String(" ")
progress    = transferred / total  (flipped when field_0x279)
barOffset   = (int)(textH + frameH * 2.5)
operator+   (*getText + String(" "))
~String(" ")
barW        = GetImage2DWidth(image_0x37c)
barH        = GetImage2DHeight(image_0x37c)
field_0x468 += dt; fadeIn; fade
```

i.e. the source order is **`P B T W H D`**. The original binary is instruction-identical
to this shape through 0x165932 (only the PC-relative literal offsets differ). The
oracle also confirms `textY = (float)(top * 2) + (float)barH * 2.5f` is computed
*after* the `GetTextWidth` call (0x165a24..0x165a38), so `fontId`/`halfW`/`textY`
are inline expressions in the `DrawString` call, not locals.

---

## 3. Measured shapes (official verifier, `autojunk=False`)

`score1.py` = `tools/verify/verify.py`'s scorer on `_ZN3Hud4drawExxP9PlayerEgobjj`;
`extab` = Hud::draw's `.ARM.extab` entry size; `rows` = LSDA call-site rows.

| shape | pct | size | extab | rows | gate |
|---|---|---|---|---|---|
| HEAD (`P H D T B W`, decls) | 92.1 | 9332 | 476 | 77 | PASS |
| HEAD + second-bar fix | 93.0 | 9332 | 480 | 78 | FAIL (extab) |
| `P B T W H D` + fix + `F0H0Y0` (faithful) | **94.1** | 9324 | 480 | 78 | FAIL (size) |
| `P B T W H D` + fix + `F0H1Y0` | 93.9 | 9332 | 488 | 79 | FAIL (extab) |
| `P B T W H D` + fix + `F0H0Y1` | 93.8 | 9332 | 488 | 79 | FAIL (extab) |
| `B D T P W H` + fix + `F0H0Y0` | 93.8 | 9332 | 480 | 78 | FAIL (extab) |
| `P B W H T D` + fix + `F0H0Y0` (landed) | **93.2** | 9332 | 468 | 76 | **PASS** |
| `P B H W T D` + fix + `F0H0Y0` | 93.2 | 9332 | 468 | 76 | PASS |
| `W T P B H D` (no fix) | 93.7 | 9332 | 468 | 76 | PASS |

`F0H0Y0` = `fontId`, `halfW`, `textY` all inlined into the `DrawString` and the two
conditional `DrawImage2D` calls (the faithful form).

A full 720-permutation sweep of `{P,B,T,W,H,D}` on top of the second-bar fix and
`F0H0Y0` confirms 93.2 is the best gate-safe score; every 93.8+ order lands at
extab 480 (78 rows).

---

## 4. Why the faithful shape is 4 B over the extab budget

`.ARM.extab` starts at `0x1a3b78`; `.rodata` is 16-aligned at `0x1c9e10`, so the
section may be at most `0x26298` bytes. The landed build is `0x26290` (8 B of
headroom); the faithful shape's Hud entry is 480 B, i.e. section `0x2629c` (+4
over) and `.rodata` moves to `0x1c9e20`, which rewrites every `.text` literal
that names a `.rodata` address (byte_exact 4500 -> 4033).

The extra row is a **second-bar LSDA split**:

```
original      : 1659a0 +412 lp=165fd0   (first bar, one row)
                165c70 +204 lp=165fd4   (second bar, one row)
P B T W H D   : 1659a8 +410 lp=165fd0   (first bar, one row)
                165c74  +40 lp=165fce   (second bar, split)
                165cbe +126 lp=165fd4
```

The split falls in the call-free `filled = progress * (float)halfW` block between
`DrawImage2D` and `DrawRegion2D` (0x165c9c..0x165cbe). Both landing pads branch to
the same `String::~String` cleanup (`0x165fd4`), so this is a machine-CFG
artefact, not a C++ scope. Moving `float filled` before `SetColor` merges the row
(476 B / 77 rows) but changes the size to 9356 B and drops the score to 83.2;
inlining `filled` into the `DrawRegion2D` arguments gives 476 B but 9356 B / 83.7.
No source placement tried so far keeps both 9332 B and 476 B.

The other extab deltas in the current build are `LevelScript::process` +4 and
`Generator::createMission` -4 (sum 0); shaving `LevelScript::process` by 4 B would
unblock the faithful 94.1 % shape, but that is a separate pass.

---

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8     # RATCHET PASS

python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj

arm-linux-androideabi-readelf -S -W cmake-build-match/libgof2hdaa.so | \
  grep -E 'ARM.extab|\.rodata'
python3 _work/reconstructions/gameresource_gen/lsda.py cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch (variant builders, 720-permutation sweep, extab/LSDA parsers) lived in
`/dev/shm/wf315`: `sweep2.py` (second-bar fix base), `sweep5.py` (full dock sweep),
`score1.py`, `extab.py`, `extab_cmp2.py`.
