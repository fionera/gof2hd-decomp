# Hud::draw — wave-103 oracle pass: dock order, `top*2` fix, extab-exact shape

**Status:** code change + analysis, 2026-11-04 (wave 103). Worktree `wt/wf315`, branch
`wave103-wf315`, base `master 1d2b4398e`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp:1627`), original size **9332 B** (Ghidra `Hud_draw` @ 0x173b90,
file 0x163b90).

Gate on entry and on exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, **`.ARM.extab 0x26298` (exact)**, lint CLEAN, drift 0,
**RATCHET PASS**.

Official per-function score: **92.1 % -> 92.9 %** at the unchanged 9332-byte size and
the unchanged 476-byte / 77-row `.ARM.extab` record. `.rodata` base unchanged
(`0x1c9e10`). The only remaining extab length deltas are the known cancelling
`Generator::createMission` -4 / `LevelScript::process` +4 pair.

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
   *before* `String text`, which is wrong; the landed source now has the `String` first.
2. **The first bar's `DrawRegion2D` `posY` is `barYOffset + top * 2`, not
   `barYOffset + top`.** The original computes `add.w r2, r4, r6, lsl #1` at
   `0x1659ce` (`r4 = layout->0x218`, `r6 = field_0x3e2`), i.e. `layout->0x218 +
   field_0x3e2 * 2`. HEAD/landed had `barYOffset + top`, which emitted a 2-byte
   `adds r3, r4, r5`. The fix is a real behavioural correction; it is landed here.
3. **The oracle's dock statement order is `P B T W H D`** (progress, barOffset,
   `String text`, barW, barH, dt/fade). That faithful order measures **93.5 %** with
   the `top*2` fix, but is 9324 B (loses size-exactness) and its `.ARM.extab` entry
   is 480 B (78 rows). It is *not* landable without a separate extab shave.
4. **The landed shape is `B H T W P D` with `fontId`/`halfW`/`textY` inlined
   (`F0H0Y0`): 92.9 %, 9332 B, extab 476 B (77 rows), `.rodata` untouched.** A full
   720-permutation sweep with the `top*2` fix confirms 92.9 is the best score that
   keeps 9332 B + 476 B; `P`-first orders cap at 92.1 %.
5. **The faithful `P B T W H D` shape is one LSDA row over budget because of a
   second-bar CFG artefact**, not a C++ scope (see §4). No source placement tried
   keeps both 9332 B and 476 B for that order.

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

The landed source is

```cpp
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

---

## 2. Ground truth: the dock statement order and `posY`

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
to this shape through `0x165932` (only the PC-relative literal offsets differ).

The `DrawRegion2D` `posY` is proven by the original `0x1659ce`:

```
165986  ldrh.w r6, [sl, #994]      ; r6 = field_0x3e2 (top)
16598a  ldr.w  r4, [r1, #536]      ; r4 = layout->field_0x218 (barYOffset)
...
1659ce  add.w  r2, r4, r6, lsl #1  ; posY = barYOffset + top * 2
1659e6  str    r2, [sp, #24]       ; 10th DrawRegion2D argument
```

The `DrawImage2D` y is `top * 2` (`0x1659a6`, `mov.w r9, r6, lsl #1`), so both the
bar image and the fill region are anchored at `top * 2`. HEAD's `barYOffset + top`
was a decomp bug; it emitted `adds r3, r4, r5` (2 B) instead of `add.w r2, r4, r6,
lsl #1` (4 B).

---

## 3. Measured shapes (official verifier)

`score1.py` = `tools/verify/verify.py`'s scorer on `_ZN3Hud4drawExxP9PlayerEgobjj`;
`extab` = Hud::draw's `.ARM.extab` entry size; `rows` = LSDA call-site rows.
All rows below use the second-bar `String`-before-width fix and the `top*2` fix.

| shape | pct | size | extab | rows | gate |
|---|---|---|---|---|---|
| HEAD (`P H D T B W`, decls) | 92.1 | 9332 | 476 | 77 | PASS |
| `P B T W H D` + `F0H0Y0` (faithful) | **93.5** | 9324 | 480 | 78 | FAIL (size+extab) |
| `P B T H W D` + `F0H0Y0` | 93.6 | 9324 | 480 | 78 | FAIL (size+extab) |
| `P B W H T D` + `F0H0Y0` (previous landed) | 92.4 | 9336 | 468 | 76 | FAIL (size+extab) |
| `P B W D T H` + `F0H0Y0` | 92.1 | 9332 | 476 | 77 | PASS |
| `H D T W P B` + `F0H0Y0` | 92.7 | 9332 | 476 | 77 | PASS |
| **`B H T W P D` + `F0H0Y0` (landed)** | **92.9** | **9332** | **476** | **77** | **PASS** |
| `H T W P D B` + `F0H0Y0` | 92.9 | 9332 | 476 | 77 | PASS |

`F0H0Y0` = `fontId`, `halfW`, `textY` all inlined into the `DrawString` and the two
conditional `DrawImage2D` calls (the faithful form). A full 720-permutation sweep
(`/dev/shm/wf315/sweep6.out`, reproducible with `sweep6.py`) shows the maximum
9332/476 score is **92.9**; `P`-first orders cap at 92.1. A follow-up sweep of the
top 28 permutations x all 8 `F/H/Y` combinations (`sweep7.out`) found **no**
9332/476 variant with locals — the locals always move the size or the extab record.

---

## 4. Why the faithful shape is one LSDA row over budget

`.ARM.extab` starts at `0x1a3b78`; `.rodata` is 16-aligned at `0x1c9e10`, so the
section may be at most `0x26298` bytes. The landed build is exactly `0x26298`
(Hud entry 476 B). The faithful `P B T W H D` shape's Hud entry is 480 B, i.e.
section `0x2629c` (+4 over) and `.rodata` moves to `0x1c9e20`, which rewrites every
`.text` literal that names a `.rodata` address (byte_exact 4500 -> 4033).

The extra row is a **second-bar LSDA split**:

```
original      : 1659a0 +412 lp=165fd0   (first bar, one row)
                165c70 +204 lp=165fd4   (second bar, one row)
P B T W H D   : 1659a8 +410 lp=165fd0   (first bar, one row)
                165c74  +40 lp=165fce   (second bar, split)
                165cbe +126 lp=165fd4
```

The split falls in the call-free `filled = progress * (float)halfW` block between
`DrawImage2D` and `DrawRegion2D` (0x165c9c..0x165cbe). The `-Oz` IR has two
`landingpad` blocks (`%2778` for `SetColor`/`DrawImage2D`, `%2780` for
`DrawRegion2D`/`GetTextWidth`/`DrawString`) that both `br` the same
`String::~String` cleanup; at `-O0` IRGen emits a single landing pad, so this is a
machine-CFG artefact, not a C++ scope. Moving `float filled` before `SetColor`
merges the row (476 B / 77 rows) but changes the size to 9348 B and drops the score
to 84.0; inlining `filled` into the `DrawRegion2D` arguments gives 476 B but
9348 B / 84.4. No source placement tried so far keeps both 9332 B and 476 B for the
faithful order, so the tuned `B H T W P D` shape is landed instead.

The other extab deltas in the current build are `LevelScript::process` +4 and
`Generator::createMission` -4 (sum 0); shaving `LevelScript::process` by 4 B would
unblock the faithful 93.5 % shape, but that is a separate pass.

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
