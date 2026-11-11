# Radar::draw wave 178 (w12208) — branch/landing-pad layout, music-tail select

**Status:** landed, 2026-11-11. Worktree `wt/w12208`, branch `wave178-w12208`,
base `master` `1b48f990f` (wave-177 merge). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave177-w12108.md` (the immediately preceding Radar pass)
and `radar-draw-wave175-w11907.md` (the `expect(field_0x1a8, 0)` landing-pad
site). This note is self-contained: every number below was measured on
`1b48f990f` in this worktree.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,636 | **6,586** |
| instruction-index alignment | 1,813 | **1,664** |
| instruction count | 3,539 ours / 3,532 orig | 3,539 / 3,532 |
| whole-`.so` `.text` differing bytes | 79,251 | **79,201** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 731 (size/base exact) | 731 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

Net: raw **-50 B**, aligned **-149 instructions**, size exactly 9,908,
`.rel.plt` still byte-identical.

## 1. Landed change (five cells, net size 0)

```diff
@@ landmarks visibility guard (line ~422)
-    if (__builtin_expect((this->landmarks != nullptr && // lint: arm_builtin
+    if ((this->landmarks != nullptr && // lint: arm_builtin
         !(Globals::status->getStation()->getIndex() == 109 ||
           Globals::status->getStation()->getIndex() == 110 ||
           (Globals::status->getStation()->getIndex() == 111 &&
-           Globals::status->getCurrentCampaignMission() > 93))), 0)) {
+           Globals::status->getCurrentCampaignMission() > 93)))) {

@@ planet-lock blip guard (line ~653)
-                    if (__builtin_expect((__builtin_expect(this->level->getPlayer()->isDockingToPlanet(), 1)), 0)) { // lint: arm_builtin anchor: Radar::draw
+                    if ((__builtin_expect(this->level->getPlayer()->isDockingToPlanet(), 1))) { // lint: arm_builtin anchor: Radar::draw

@@ enemy loop, wreck (line ~691)
-            bool wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
+            bool wreck = false;
+            if (enemy->hasCargo) {
+                wreck = enemy->isDead() || enemy->isDying();
+            }

@@ enemy-lock release (line ~941)
-                if (*(navWreck ? &this->field_0x1c : &this->lockedEnemy) == nullptr) {
+                if ((navWreck ? this->field_0x1c : this->lockedEnemy) == nullptr) {

@@ music tail, mission-complete jingle (line ~1332)
-                            Globals::sound->play(Globals::status->getCurrentCampaignMission() < 106 ? 2241 : 2242,
-                                                 nullptr, nullptr, 0.0f);
+                            if (Globals::status->getCurrentCampaignMission() > 105) {
+                                Globals::sound->play(2242, nullptr, nullptr, 0.0f);
+                            } else {
+                                Globals::sound->play(2241, nullptr, nullptr, 0.0f);
+                            }
```

All five are condition/arm/order reshapes; no condition semantics, side
effects or short-circuit order change. The `> 105` form is the same predicate
as the original `< 106` (both select 2241 for missions <= 105 and 2242 for
missions >= 106), but clang canonicalises the two spellings to different
condition codes.

## 2. Attribution (all size 9,908 unless noted)

Cells: `L` = landmarks `expect(...,0)` removed; `D` = inner
`expect(isDockingToPlanet,1)` removed (outer single `expect(...,1)` kept);
`W` = wreck independent-if; `N` = `navWreck` value form; `M` = music tail
`> 105` if/else.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,636 | 1,813 | 9,908 |
| `L` | 6,670 | 1,701 | 9,908 |
| `D` | 6,665 | 1,813 | 9,908 |
| `W` | 6,635 | 1,815 | 9,908 |
| `N` | 6,636 | 1,813 | 9,908 |
| `M` | 6,595 | 1,806 | **9,912** |
| `L+D` | 6,655 | 1,670 | 9,908 |
| `L+M` | 6,602 | 1,693 | 9,908 |
| `D+M` | 6,623 | 1,806 | **9,912** |
| `L+D+M` | 6,587 | 1,662 | 9,908 |
| `L+D+M+W` | **6,586** | 1,664 | 9,908 |
| **`L+D+M+W+N` (landed)** | **6,586** | **1,664** | **9,908** |

The win is a multi-cell interaction, not a single edit:

* `M` alone is **size-fatal** (+4 B, 9,912). `L` compensates exactly -4 B, so
  `L+M` and `L+D+M` are size-exact. `D+M` alone is still 9,912.
* `L` alone is raw-worse (+34) but aligned-better (-112): it moves the
  landmark loop's landing pad. `D` alone is raw-worse (+29). Together with
  `M` they unlock the music-tail match and drop raw by 49.
* `W` is -1 raw / +2 aligned on the final shape; kept because raw is the
  landing metric.
* `N` is exactly neutral on the final shape (same bytes, same size); kept as
  the more faithful value form of the original `ldreq/ldrne` select.

## 3. Variant tables (this session)

### 3.1 Music-tail spelling (on `L+D+W+N`)

| spelling | raw | aligned | size |
|---|---:|---:|---:|
| `mission < 106 ? 2241 : 2242` (base ternary) | 6,654 | 1,672 | 9,908 |
| named temp `int musicId = ... < 106 ? ...` | 6,648 | 1,670 | 9,908 |
| if/else `>= 106` first | 6,679 | 1,667 | **9,896** |
| if/else `<= 105` first | 6,590 | 1,668 | 9,908 |
| **if/else `> 105` first (landed)** | **6,586** | **1,664** | **9,908** |
| ternary `>= 106 ? 2242 : 2241` | 6,655 | 1,673 | 9,908 |
| ternary `> 105 ? 2242 : 2241` | 6,655 | 1,673 | 9,908 |

The `> 105` if/else makes clang emit the original's exact select at
`130654`:

```
orig 130654: cmp r1,#106 ; ite lt ; movwlt r1,#2241 ; movwge r1,#2242
ours 130654: cmp r1,#106 ; ite lt ; movwlt r1,#2241 ; movwge r1,#2242
```

(only the base register of the preceding `ldr r0,[r8]` vs `[r9]` differs).

### 3.2 Wreck computation / statement order (on the landed shape)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| independent-if (landed) | **6,586** | 1,664 | 9,908 |
| `hasCargo && (isDead || isDying)` | 6,587 | 1,662 | 9,908 |
| `(isDead || isDying) && hasCargo` | 9,812 | 3,388 | **10,244** |
| `update(); wreck; field_0x75 = 0;` | 6,589 | 1,656 | 9,908 |
| `update(); wreck-if; field_0x75 = 0;` | 6,588 | 1,658 | 9,908 |
| `wreck; update(); field_0x75 = 0;` | 6,588 | 1,673 | 9,908 |
| `update(); field_0x75 = 0; wreck;` | 8,531 | 1,679 | **9,904** |
| `field_0x75 = 0; update(); wreck;` | 8,531 | 1,679 | **9,904** |
| `int wreck` / `unsigned wreck` | 8,473 | 1,799 | **9,912** |
| `update(); int wreck = ...` | 8,593 | 1,653 | **9,916** |

The original computes `wreck` **after** `this->update(enemy)` and before
`enemy->field_0x75 = 0` (`12ed5e update`, `12ed66 hasCargo`, `12ed86 strb
field_0x75`), but moving our source to that order is raw-worse (+7) on the
current shape, so the base order is kept.

### 3.3 `bossSeen` first assignment (in the `continue` arm)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `bossSeen = shipGroup == 10;` (base) | **6,586** | 1,664 | 9,908 |
| `if (shipGroup == 10) bossSeen = true;` | 9,187 | 1,867 | **9,980** |
| `bossSeen = true;` | 8,507 | 1,703 | **9,900** |

### 3.4 `navWreck` select form (on `L+D+M+W`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| value form (landed) | **6,586** | 1,664 | 9,908 |
| pointer form `*(cond ? &a : &b)` | 6,586 | 1,664 | 9,908 |
| ternary of comparisons | 9,067 | 1,882 | **9,980** |

### 3.5 Whole-function expect re-sweep (on the landed shape)

All 26 `__builtin_expect` sites were removed and flipped 0<->1. Best raw
removal is `rm6_0` (`getCurrentCampaignMission() != 120` plain):
**6,591 / 1,630 / 9,908** (raw +5, aligned -34). Best raw flip is neutral.
No single-expect edit beats 6,586 raw.

## 4. Disassembly evidence

* **Planet-lock block.** Original `12ff98`:
  `ldr r0,[r4,#416]; ldr r2,[sp,#100]; ldr r1,[r4,#440]; add r0,r2;
  str r0,[r4,#416]; cmp r0,r1; ble.w 130486` — the lock body is fall-through.
  Ours `130050` is byte-identical modulo the `ble` target (ours `1303ee`).
* **Dock guard.** Original `130470` has the body inline:
  `ldrb.w r0,[r4,#424]; ldr r1,[r4,#24]; cmp r0,#0; str r1,[r4,#20];
  beq.n 130486; ldr r0,[r4,#0]; blx getPlayer; blx dockToPlanet`.
  Ours still emits the body out of line (`bne.w 1305bc` at `1303ce`).
  Making the guard plain is 4 B short (9,904) and raw-worse (6,786 on the
  final shape), so `expect(field_0x1a8, 0)` stays.
* **Asteroid `field_0x1a8` guard.** Original `12f0ee`:
  `ldrb.w r0,[r4,#424]; cbnz r0,12f15c` (load after the `field_0x1b0`
  check). Ours hoists the load to `12f0c6` and reuses it; residual.
* **`ite eq` select.** Original `12fe98` uses
  `ite eq; ldreq r0,[sp,#128]; ldrne r0,[sp,#96]`; ours uses
  `it eq; moveq r1,r0` after two unconditional loads. Residual (same 2 B
  net as the value/pointer forms above).
* **`campaign` in `sl`.** Original `12e0f8 mov sl,r0` / `12e38a cmp.w sl,#2`
  keeps `campaign` in `sl` until the landmark loop; ours spills it to
  `sp+144` and uses `sl` for the zero constant and the station pointer.
  Every size-exact campaign-in-`sl` shape measured in waves 162-177 is
  raw-worse; unchanged.

## 5. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12208/Radar.best3.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12208/` —
`Radar.base.cpp` (base source), `Radar.best3.cpp` (landed source),
`harness.py` (variant runner), `sbs.py` (side-by-side disasm),
`w12.json`/`w12.log` (attribution sweep), `w7.json`/`w7.log` (expect
re-sweep), `w5.json`/`w6.json` (music-tail sweep), `w8.json` (wreck/nav
sweep), `orig_full.dis` / `ours_full.dis` (full-function disassembly).
