# Hud::draw — second-bar statement order under the `.ARM.extab` budget (w5015)

**Status:** code change + analysis, 2026-11-04 (wave 100). Worktree `wt/w5015`, branch
`wave100-w5015`, base `master 44225192c`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B**, Ghidra `Hud_draw` @ 0x173b90
(file 0x163b90).

Gate on entry and on exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports **207**, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26294` (4 B short of the original), lint **CLEAN**,
drift **0**, **RATCHET PASS**.

Official per-function score: **91.7 % -> 93.0 %** at the unchanged 9332-byte size.

This note extends `hud-draw-extab-lsda-w4273.md`, `hud-draw-dock-order-w4415.md` and
`hud-draw-gamma-store-dock-order-w4515.md`. It leaves the gamma-store question and the
`this`-spill allocation untouched; it is purely a statement-order pass in the two progress-bar
blocks.

---

## 0. TL;DR

1. The docking-transfer block now uses the **faithful original statement order**
   `P B T W H D` (`progress`, `barOffset`, `String text`, `barW`, `barH`, `dt/fade`), i.e. the
   order the original binary executes (see §2). This was previously gate-blocked at 92.3 %
   because it needs 79 LSDA rows (extab 0x2629c, `.rodata` +0x10).
2. Moving the **second (jump-drive / cloak) bar's** declarations into the order
   `W T H B D O Y` — `halfW`, `text`, `barH`, `barYOffset`, `dt/fade/fadeColor`, `top`, `y` —
   removes one LSDA row (`Hud::draw` 480 B / **78 rows**), so the faithful dock order fits the
   budget again: `.ARM.extab` stays **0x26294** and `.rodata` stays **0x1c9e10**.
3. Result: **91.7 -> 93.0 %** at 9332 B, `.text` differing bytes 101,675 -> **101,394**,
   every gate count unchanged.
4. This is a pure declaration/statement reorder. No anchors, no `volatile`, no
   `__builtin_*`, no padding locals, no new constructs; all moved variables are independent
   scalar declarations, so semantics are unchanged.
5. The fully faithful second-bar order `T W H D O Y` (text first, `top`/`barYOffset` before
   `SetColor`) scores 92.1 % but costs 79 rows and is still gate-blocked. `W T H B O Y D`
   scores 92.8 % at 78 rows but is not the original order.

---

## 1. The `.ARM.extab` budget (restated)

```
.ARM.exidx  0x19cc08  size 0x06f70
.ARM.extab  0x1a3b78  size 0x26298 (original)   ends 0x1c9e10
.rodata     0x1c9e10  size 0x41b67  (align 16)
```

`.ARM.extab`'s alignment is 4 and `.rodata`'s is 16; the original's extab already ends exactly
on the 16-aligned `.rodata` base, so there is **zero slack**. Any extab size `<= 0x26298` is
free; a single extra LSDA row (6–8 B) pushes `.rodata` to `0x1c9e20`, which changes every
`.text` literal-pool word naming a `.rodata` address and drops `byte_exact` 4500 -> 4040.

Hud::draw's extab entry is 476 B / 77 rows in the original and 480 B / **78 rows** here; the
whole section is 0x26294 (156,308 vs 156,312). The usable budget is therefore **at most 78
rows for Hud::draw**.

## 2. Ground truth: the original statement order

Disassembly of the original (`objdump -d`, file offset = VMA - 0x10000):

Docking-transfer block, 0x165880..0x1659c0:

```
frameH      = GetImage2DHeight(image_0x37c)
textH       = GetTextHeight(font)
transferred = getDockTransferedAmount()
total       = getDockTotalAmount()
getText(3205/3204); String(" ")          <- String text construction
progress    = transferred / total
barOffset   = (int)(textH + frameH*2.5)
operator+   (*getText + String(" ")); ~String
barW        = GetImage2DWidth(image_0x37c)
barH        = GetImage2DHeight(image_0x37c)
field_0x468 += dt; fadeIn; fade = 1.0f
```

i.e. source order `P B T W H D` (the compiler schedules `String(" ")` before `progress` and
`operator+` after `barOffset`; the declaration is what matters).

Second (jump-drive / cloak) bar, 0x165ba6..0x165c68:

```
getText(318/317); String ctor            <- String text
GetImage2DWidth(image_0x37c) * 0.5       <- int halfW
GetImage2DHeight(image_0x37c)            <- int barH
field_0x464 += dt; fade = /1000; fadeColor
top   = field_0x3e2                      (ldrh r6,[sl,#994])
barYOffset = layout->field_0x218         (ldr r5,[r1,#536])
SetColor(fadeColor)
DrawImage2D(image_0x378, w>>1, y, 0x11, 0x14)
DrawRegion2D(image_0x37c, ...)
GetTextWidth / DrawString
```

i.e. the faithful second-bar order is `T W H D O Y` (`top` then `barYOffset`, both before
`SetColor`). That order costs 79 rows in our build (see §4).

## 3. Landing-pad caching mechanism

Clang 7.0.2's ARM EH lowering caches the current landing pad **per innermost EH scope**. A
scalar local whose `lifetime.start` sits between two invokes splits the scope even when the
local is optimized away entirely, so it creates a second landing-pad MBB (a `b.n` to the same
cleanup block) and therefore an extra LSDA call-site row. This was verified with standalone
clang 7.0.2 micro-tests (`-Oz -fPIC -funwind-tables -fstack-protector-strong`): inserting a
dead `int x = 0;` between two calls adds a row; removing it merges the pads back.

Consequences:

* `String text`'s declaration position in the second bar controls whether its cleanup region
  merges with the `DrawRegion2D`/`DrawString` region. Placing it **after `barH`** (i.e. after
  the two `GetImage2D*` invokes and before the `field_0x464` arithmetic) keeps the row count at
  78; placing it first (`T W H ...`) costs 79.
* `float filled` must stay declared at its use site (`float filled = progress * (float) halfW;`
  immediately before `DrawRegion2D`). Hoisting the declaration earlier merges pads but changes
  register allocation (`vcvt.f32.s32 s18,s18` appears) and grows the function to 9360 B.
* `int top` / `int y` can be moved past the `fadeColor` computation without changing size; this
  is what lets `W T H B D O Y` keep 78 rows.

## 4. The landed source change

`src/game/ui/Hud.cpp`, `Hud::draw`. Two edits.

Docking-transfer block (now `P B T W H D`):

```diff
             barOffset = (int) ((float) textH + (float) frameH * 2.5f);
+            String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
+            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
             this->field_0x468 += dt;
             float fadeIn = (float) this->field_0x468 / 1000.0f;
             float fade = 1.0f;
-            String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
-            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
```

Second bar (now `W T H B D O Y`):

```cpp
            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            int barYOffset = Globals::layout->field_0x218;
            this->field_0x464 += dt;
            float fade = (float) this->field_0x464 / 1000.0f;
            int fadeColor = fade < 1.0f ? (int) (fade * 255.0f) - 256 : 0xffffffff;
            int top = this->field_0x3e2;
            int y = barOffset + top * 2;
            Globals::Canvas->SetColor(fadeColor);
            Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, y, 0x11, 0x14);
            float filled = progress * (float) halfW;
            Globals::Canvas->DrawRegion2D(this->image_0x37c, (int) ((float) halfW - filled), 0, (int) (filled + filled),
                                          barH, 0.0f, 0, 0, (int) ((float) (Globals::w >> 1) - filled), y + barYOffset);
            Globals::Canvas->DrawString(...);
```

The `DrawImage2D`/`DrawRegion2D`/`DrawString` tail is unchanged.

## 5. Search performed (official verifier, `autojunk=False`)

All builds are full `link_so` + `tools/verify` scoring of `_ZN3Hud4drawExxP9PlayerEgobjj`;
`.rodata` from `readelf -S -W`. A build is gate-compatible iff `usize == 9332` and
`.rodata == 0x1c9e10` (equivalently `.ARM.extab <= 0x26298`).

### 5.1 Dock block: 24 permutations of `{T,W,H,D}` on the landed second-bar shape

| order | size | extab rows | `.rodata` | score |
|---|---|---|---|---|
| **`T W H D`** (faithful) | **9332** | **78** | **0x1c9e10** | **93.0** (landed) |
| `T W D H` | 9332 | 78 | 0x1c9e10 | 92.6 |
| `H D T W` | 9332 | 77 | 0x1c9e10 | 90.8 |
| `H T W D` | 9328 | 77 | 0x1c9e10 | 90.9 |
| `W D T H` | 9320 | 77 | 0x1c9e10 | 89.3 |
| `D H T W` | 9324 | 77 | 0x1c9e10 | 90.5 |
| others | — | — | mostly 0x1c9e20 | <= 90.6 |

The faithful dock order is also the best dock order once the second bar is fixed.

### 5.2 Second bar: 144 combinations of `{T,W,H}` prefix x `{B,O,Y,D}` suffix

Gate-compatible best (all 9332 B, `.rodata 0x1c9e10`, 78 rows):

| order | score |
|---|---|
| **`W T H B D O Y`** | **93.0** (landed) |
| `W T H B O Y D` | 92.8 |
| `W T H B O D Y` | 92.8 |
| `W T H O B Y D` | 92.8 |
| `W T H O B D Y` | 92.8 |
| `W T H O Y B D` | 92.8 |
| `H T W B D O Y` | 92.8 |
| `W T H O Y D B` | 92.7 |
| `W T H O D B Y` | 92.7 |
| `W T H O D Y B` | 92.7 |
| `W T H D B O Y` | 92.7 |
| `W T H D O B Y` | 92.7 |
| `W T H D O Y B` | 92.7 |
| `H T W B O Y D` / `H T W B O D Y` / `H T W O B Y D` / `H T W O B D Y` / `H T W O Y B D` | 92.6 |
| `H T W O Y D B` / `H T W O D B Y` / `H T W O D Y B` / `H T W D B O Y` / `H T W D O B Y` / `H T W D O Y B` | 92.5 |

(`B` = `barYOffset`, `O` = `top`+`y`, `Y` = `y` alone; in the landed order `O` and `Y` are
adjacent, `top` then `y`.)

### 5.3 Blocked shapes (79+ rows, `.rodata 0x1c9e20`)

| shape | size | rows | score |
|---|---|---|---|
| `T W H D O Y` (fully faithful second bar) | 9332 | 79 | 92.1 |
| `T W H B D O Y` | 9332 | 79 | 92.5 |
| `T W H O Y B D` | 9332 | 79 | 92.3 |
| `T W H B O Y D` (T first, O/Y before D) | 9332 | 79 | 92.8 |
| `T W H B D SC O Y` (T first, `SetColor` before `top`/`y`) | 9336 | 80 | 92.5 |
| `W T H B D SC O Y` (T between `W`,`H`, `SetColor` before `top`/`y`) | 9336 | 79 | 92.4 |

### 5.4 Other measured variants

| variant | size | rows | score |
|---|---|---|---|
| HEAD (dock `P B H D T W`, second bar pristine) | 9332 | 78 | 91.7 |
| dock `P B T W H D` + second bar `W H T B D SC O Y` | 9332 | 78 | 92.1 |
| dock `P B T W H D` + second bar `W H T D SC O Y B` | 9332 | 78 | 91.8 |
| `float filled` hoisted before `DrawImage2D` | 9360 | 77 | 83.2 |
| `float filled` hoisted after `fadeColor` | 9360 | 77 | 83.4 |
| `SetColor` before `top`/`y` (i.e. `W T H B D SC O Y`) | 9336 | 79 | 92.4 |
| `SetColor` immediately after `barYOffset` | — | — | compile error (no `fadeColor`) |

## 6. Per-function `.ARM.extab` deltas (current build)

Parsing `.ARM.exidx` -> `.ARM.extab` and comparing each function's entry size with the
original, exactly **five** functions differ; the deltas sum to the section's -4 B:

| function | original | ours | delta |
|---|---|---|---|
| `Generator::createMission` | 172 | 176 | **+4** |
| `Level::createShip` | 336 | 328 | -8 |
| `LevelScript::process` | 348 | 352 | **+4** |
| `HangarWindow::render` | 948 | 940 | -8 |
| `Hud::draw` | 476 | 480 | **+4** |
| **sum** | | | **-4** |

Shaving either `Generator::createMission` or `LevelScript::process` by 4 B (or Hud::draw by 4 B)
would free a 79th Hud::draw row and unblock the fully faithful `T W H D O Y` second bar
(92.1 %) or the 92.8 % `W T H B O Y D` shape. All three are non-byte-exact targets in their own
right, so that is a separate pass.

## 7. Residual diffs of the landed shape

Unchanged from `hud-draw-dock-order-w4415.md` §5 except that the dock prologue now matches:

1. Prologue `vpush {d8-d12}` vs original `vpush {d8-d11}` (one extra callee-saved VFP
   register); unwind opcodes differ accordingly.
2. Gamma-block `&Globals::status` store placement (the `this`-spill tension documented in
   `hud-draw-gamma-store-dock-order-w4515.md` §1-2).
3. The two dead `ldr.w r8, [sp, #108]` reloads at 0x164176 / 0x164210.
4. Mission-panel `fp`/`r9` swaps, `add r3, r4` vs `sxtah r3, r3, r4`, `ldrd r5, r8` vs
   `ldrd r8, r5`, `add r2, r5` vs `add r2, r8`.
5. EH landing-pad tail ordering.

## 8. Reproduction

```
cmake --build cmake-build-match --target check -j6       # RATCHET PASS
cmake --build cmake-build-match --target drift -j6       # 0 drifted fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
arm-linux-androideabi-readelf -S -W cmake-build-match/libgof2hdaa.so | grep -E 'ARM.extab|\.rodata'
python3 _work/reconstructions/gameresource_gen/lsda.py cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch (variant builders, 24-perm dock sweep, 144-combination second-bar sweep, LSDA/extab
dumpers) lived in `/dev/shm/w5015`: `bm.sh`, `score_one.py`, `apply_final.py`,
`sweep_dock_final2.py`, `sweep_sec3.py`, `sweep_sec4.py`, `extab_sizes_cur.py`.
