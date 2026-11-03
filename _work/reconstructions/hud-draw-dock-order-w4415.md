# Hud::draw — dock-block statement order under the `.ARM.extab` budget (w4415)

**Status:** code change + analysis, 2026-11-03. Worktree `wt/w4415`, branch `wave94-w4415`,
base `master f262bd4b`. Target `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp:1091`),
original size **9332 B**, Ghidra `Hud_draw` @ 0x173b90 (file 0x163b90).

Gate on entry and on exit (full `check`): **byte_exact 4499 / linked_exact 4499**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports **207**, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, lint **CLEAN**, drift **0**, **RATCHET PASS**.

Official per-function score: **89.1 % -> 91.7 %** at the unchanged 9332-byte size.

This note extends `hud-draw-extab-lsda-w4273.md` (which established the `.ARM.extab` trap and the
"faithful" `P B T W H D` dock order at 90.7 %), `hud-draw-status-slot-w4253.md`,
`hud-draw-status-slot-w4233.md`, `hud-draw-st-local-w4215.md`, `hud-draw-slot-coloring-w4114.md`,
`hud-draw-slot-rotation-w4014.md` and `hud-draw-this-spill-w3910.md`.

---

## 0. TL;DR

1. The w4273 note's "dock reorder" (`String text = ...` moved to just after `barOffset`, i.e. the
   `P B T W H D` order that the *original binary* actually executes) is the faithful source shape
   and scores **92.3 %** at 9332 B — but it grows Hud::draw's LSDA call-site table from 77 to 79
   rows, `.ARM.extab` 0x26290 -> 0x2629c, which pushes `.rodata` from 0x1c9e10 to 0x1c9e20 and
   drops `byte_exact` 4499 -> 4040. **Gate-blocked.**
2. The budget is exactly **one LSDA row**: 77 rows -> 0x26290, 78 rows -> 0x26294, 79 rows ->
   0x2629c. `.rodata` is 16-byte aligned and `.ARM.extab` ends at 0x1c9e10 in the original, so
   **any extab size <= 0x26298 is free** (0x26294 and 0x26298 both round to 0x1c9e10).
3. Two extra *type* changes in the two `shift` locals of the mission panel (`short -> int` in the
   `type == 12` branch, `int -> short` in the final `else`) are worth **+1.6 pt** on top of the
   reorder and shave 8 B off the LSDA — but not enough to fit 79 rows.
4. A 720-permutation sweep of the six dock-block statements, on top of the shift changes, finds a
   **78-row / 9332-byte / extab 0x26294** shape that scores **91.7 %**:
   `P B H D T W` (`progress`, `barOffset`, `barH`, `dt/fade`, `text`, `barW`). That is the change
   landed here: **89.1 -> 91.7**, size-exact preserved, `.rodata` untouched.
5. Two strictly better shapes are measured and documented but remain blocked by the extab budget:
   * `P B T W H D` (faithful) = **92.3 %**, extab 0x2629c (**+4 B over budget**);
   * `P B T W H D` with `unsigned short top` moved between `SetColor` and `DrawImage2D`
     = **93.4 %**, extab 0x262a0 (**+8 B over budget**).

---

## 1. The `.ARM.extab` budget, restated with exact numbers

```
.ARM.exidx  0x19cc08  size 0x06f70
.ARM.extab  0x1a3b78  size 0x26298 (original)   ends 0x1c9e10
.rodata     0x1c9e10  size 0x41b67  (align 16)
```

`.ARM.extab`'s section alignment is 4, `.rodata`'s is 16, and `.rodata` is the very next section.
Because `0x1a3b78 + 0x26298 = 0x1c9e10` is already 16-aligned, the original leaves **zero slack**.

Measured (`arm-linux-androideabi-readelf -S -W`), per Hud::draw source shape:

| shape | `.ARM.extab` | `.rodata` | `byte_exact` |
|---|---|---|---|
| original | 0x26298 | 0x1c9e10 | — |
| baseline (HEAD) | 0x26290 | 0x1c9e10 | 4499 |
| landed (`P B H D T W`) | **0x26294** | **0x1c9e10** | **4499** |
| `P B T W H D` (faithful) | 0x2629c | 0x1c9e20 | **4040** (measured, RATCHET FAIL) |
| `P B T W H D` + `top` moved | 0x262a0 | 0x1c9e20 | 4040 |

So the usable budget is `extab <= 0x26298`, i.e. **Hud::draw's extab entry <= 484 bytes**
(original 476). Hud::draw's entry is `4 (personality prel31) + 16 (unwind opcodes) + 3 (LSDA
header) + uleb(cs_table_len) + call-site table`, rounded up to a multiple of 4. The call-site
table is ~6 B per row, so **one row is worth 6-8 B of section size** and the budget is
"at most 78 rows".

Hud::draw's extab entry, byte-for-byte:

```
original : 9493e97f c94f9702 f0800083 b0b0b0ab ffff01 c603 00f813 0000 f813 08c047 00 ...
landed   : 9c93e97f c9519702 f0800084 b0b0b0ab ffff01 cb03 00f413 0000 f413 08bc47 00 ...
           ^personality  ^unwind opcodes (differ: landed pushes d8-d12, original d8-d11)
                                                    ^cs_table_len: 454 (orig) vs 459 (landed)
```

---

## 2. LSDA row accounting (the whole function, not just the dock block)

`python3 _work/reconstructions/gameresource_gen/lsda.py <so> _ZN3Hud4drawExxP9PlayerEgobjj`

| shape | rows | dock-block rows | extab |
|---|---|---|---|
| original | **77** | 4 (`165926 +10`, `16593e +4`, `16594e +4`, `1659a0 +412`) | 0x26298 |
| baseline | **77** | 6 (`16595c +10`, `1659a6 +112`, `165a36 +272`, `165c78 +40`, `165cc2 +126`, ...) | 0x26290 |
| `P B T W H D` | **79** | 6 | 0x2629c |
| landed `P B H D T W` | **78** | 5 | 0x26294 |

The baseline coincidentally has the same *count* as the original but a different split: it loses
two rows at the head of the dock block (its `barW`/`barH` calls come before `getText`, so the
`getText`/`String(" ")`/`operator+` triple collapses into a single row) and gains two rows from
the two `String text` cleanup regions being split. The reorder restores the original's
`getText`/`String(" ")`/`operator+` structure: 0x165880..0x165936 becomes **instruction-identical
to the original** (only the three PC-relative constant-pool offsets differ, `ldr r0,[pc,#956]`
vs `[pc,#952]` etc., because the pool sits 4 B lower) but exposes both splits, hence 79.

Landing-pad stubs for the two dock `String text` cleanups (both `b.n` to the same
`add r0, sp, #208; bl StringD1Ev; bl _Unwind_Resume` block):

```
P B T W H D   : 1659a0 +112 lp=165f70 / 165a30 +272 lp=165fca   (2 rows, orig 1)
                165c74  +40 lp=165fd2 / 165cbe +126 lp=165fd6   (2 rows, orig 1)
landed        : 1659a6 +108 lp=165f6e / 165a32 +272 lp=165fc8   (2 rows)
                165c74  +40 lp=165fd0 / 165cbe +126 lp=165fd4   (2 rows)
```

The splits are a machine-CFG artefact (two landing-pad MBBs that both do the same cleanup); the
IR has a *single* `landingpad` block `%2634` with 17 predecessors, so this cannot be steered from
the C++ scope structure directly.

---

## 3. The landed source change

`src/game/ui/Hud.cpp`, docking-transfer block (`Hud::draw`, ~line 1632). Three edits:

```diff
@@ mission panel, `type == 12` branch (line ~1338)
-            short shift = 0;
+            int shift = 0;
@@ final `else` branch (line ~1385)
-        int shift = 0;
+        short shift = 0;
@@ docking-transfer block (line ~1638)
             barOffset = (int) ((float) textH + (float) frameH * 2.5f);
-            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
             this->field_0x468 += dt;
             float fadeIn = (float) this->field_0x468 / 1000.0f;
             float fade = 1.0f;
             String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
+            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             if (fadeIn < 1.0f)
                 fade = fadeIn;
```

i.e. the statement order becomes `progress / barOffset / barH / dt+fadeIn+fade / text / barW`,
followed by `if (fadeIn < 1.0f) fade = fadeIn;`. No new constructs, no anchors, no `volatile`,
no `__builtin_*`, no padding locals; `barH` and `barW` are pure getters so the reorder is
semantics-preserving.

Resulting dock-block code (ours, 0x16594e..0x165a58) — the `getText`/`String(" ")`/`operator+`
triple is byte-identical to the original at 0x1658d0..0x165932; the divergence starts at the
`GetImage2DWidth`/`GetImage2DHeight` pair:

```
16594e: add  r0, sp, #208        ; operator+ result slot
165950: mov  r1, r8
165952: mov  r2, fp
165954: blx  operator+
165958: add  r0, sp, #196
16595a: blx  ~String             ; String(" ") temp
16595e: ldr.w r1, [sl, #892]     ; image_0x37c
165962: ldr.w r0, [r9]
165966: blx  GetImage2DWidth     ; barW
...
```

---

## 4. Search performed (all measured with the official verifier, `autojunk=False`)

`score1.py` = `tools/verify/verify.py`'s scorer on `_ZN3Hud4drawExxP9PlayerEgobjj`; `extab` from
`readelf -S -W`.

### 4.1 720 permutations of `{T,P,B,W,H,D}` on top of the shift changes

`T` = `String text = ...`, `P` = progress+flip, `B` = barOffset, `W` = barW, `H` = barH,
`D` = `field_0x468 += dt; fadeIn; fade = 1.0f;`.

| order | size | extab | score |
|---|---|---|---|
| `P B T W H D` | 9332 | 0x2629c | **92.3** (blocked) |
| `P B T H W D` | 9332 | 0x2629c | 92.1 (blocked) |
| `P B H D T W` | 9332 | **0x26294** | **91.7** (landed) |
| `P H B D T W` / `P B H D T W` / `B P H D T W` | 9332 | 0x26294 | 91.7 |
| `P D H T W B` / `D P H T W B` | 9332 | 0x26294 | 91.5 |
| `H T P B W D` / `H T B P W D` | 9328 | 0x26294 | 91.7 |
| `B W H D T` (baseline order) | 9332 | 0x26290 | 89.1 |

Every order that puts `T` before `W`/`H` and `P` before `B` reproduces the original's dock-block
prologue but costs 2 LSDA rows.

### 4.2 `unsigned short top` / `int barYOffset` placement

`top` must precede `DrawImage2D`; `barYOffset` must precede `DrawRegion2D`. Sweeping both over the
six statement slots:

| order | top slot | extab | score |
|---|---|---|---|
| `P B T W H D` | before `DrawImage2D` (after `SetColor`) | 0x262a0 | **93.4** (blocked) |
| `P B T W H D` | unchanged | 0x2629c | 92.3 (blocked) |
| `P B H D T W` | unchanged | 0x26294 | 91.7 (landed) |
| `P B H D T W` | before `DrawImage2D` | 0x2629c | 91.3 |

### 4.3 `fontId` / `halfW` / `textY` placement (6 slots x 3 decls, 216 combos)

Moving the three declarations earlier merges one of the two dock-block splits (78 rows) but also
shrinks the function to 9320 B, so it loses `size-exact`. Best: `91.6 % / 9320 B / 0x26294`.

### 4.4 Other shapes measured and rejected

| variant | size | extab | score |
|---|---|---|---|
| `String text` after `total` (`T` before `P`, "decl_top") | 9356 | 0x262a4 | 86.8 |
| `String space(" "); String text = *getText() + space;` | 9324 | 0x2629c | 87.4 |
| `text += String(" ")` | 9332 | 0x262a0 | 88.7 |
| `String text(*getText(), false)` in the dock block | 9364 | 0x262a8 | 86.7 |
| second-bar `String text` as copy-init / no-`false` / hoisted pointer | 9332 | unchanged | unchanged |
| `float fade = fadeIn < 1.0f ? fadeIn : 1.0f;` | 9332 | unchanged | unchanged |
| `progress = field_0x279 ? 1.0f - progress : progress;` | 9332 | unchanged | unchanged |
| `int top` / `short barYOffset` | 9332 | unchanged | unchanged |
| shift types `(short,short)` / `(int,int)` | 9336 / 9328 | 0x2629c | 88.4 / 88.3 |

The two `shift` type changes are load-bearing for the reorder: on the baseline order they are
neutral-to-negative (86.8), but combined with `P B T W H D` they are +1.6 pt and -8 B of extab.

---

## 5. Residual diffs of the landed shape (`realaddr.py`, real vs branch-target only)

Still open, in rough order of size:

1. **Prologue**: ours `vpush {d8-d12}`, original `vpush {d8-d11}` (one extra callee-saved VFP
   register). This is also why the extab unwind opcodes differ (`c9519702 f0800084` vs
   `c94f9702 f0800083`). The faithful `P B T W H D` order *does* get `vpush {d8-d11}`.
2. **Gamma block store placement** (`&Globals::status` in r6, reload, spill after) — see
   `hud-draw-extab-lsda-w4273.md` §2; unchanged.
3. **The two dead `ldr.w r8, [sp, #108]` reloads** at 0x164176 / 0x164210 — unchanged.
4. Mission-panel `fp`/`r9` swaps, `add r3, r4` vs `sxtah r3, r3, r4`, `ldrd r5, r8` vs
   `ldrd r8, r5`, `add r2, r5` vs `add r2, r8` — unchanged.
5. Dock-block register allocation: original `barW -> r8`, `barH -> fp`; ours `barW -> fp`,
   `barH -> [sp,#96]`. Downstream of (1).
6. EH landing-pad tail ordering.

---

## 6. What would unblock the better shapes

The faithful `P B T W H D` (92.3 %) and the `top`-moved variant (93.4 %) need **4 B and 8 B**
respectively of `.ARM.extab`. Two routes, both outside this pass:

* **Remove one LSDA row from Hud::draw.** The two dock-block splits are the only rows we have that
  the original does not; both are machine-CFG landing-pad duplications, not C++ scopes, so a
  source shape that merges one has not been found (the `fontId/halfW/textY` moves merge one but
  cost 12 B of code).
* **Shrink another function's extab entry.** On the landed build exactly **5** functions have an
  extab entry size different from the original (computed from the sorted extab-entry addresses,
  the sum of the deltas reproduces the -4 B section delta exactly):

  | function | original | ours | delta |
  |---|---|---|---|
  | `Generator::createMission` (0x971b8) | 172 | 176 | **+4** |
  | `Level::createShip` (0xc0214) | 336 | 328 | -8 |
  | `LevelScript::process` (0x137d18) | 348 | 352 | **+4** |
  | `HangarWindow::render` (0x1490d8) | 948 | 940 | -8 |
  | `Hud::draw` (0x163b90) | 476 | 480 | **+4** |

  Fixing **either** of the two +4 entries would bring the faithful `P B T W H D` shape
  (0x2629c) down to 0x26298 and unblock its 92.3 %. Both functions are far from byte-exact
  (`Generator::createMission` 74.3 %, `LevelScript::process` 52.0 %) and their LSDA tables are
  280 vs 279 and 331 vs 324 bytes, i.e. the delta is a real code difference, so this is a
  separate pass. (`_zip_get_name`, `_zip_name_locate`, `_zip_file_get_offset`, `_zip_entry_new`,
  `zip_fclose`, `zip_error_to_str` all match on the landed build.)

---

## 7. Reproduction

```
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j8     # RATCHET PASS
cmake --build cmake-build-match --target drift -j8     # 0 drifted fields
arm-linux-androideabi-readelf -S -W cmake-build-match/libgof2hdaa.so | grep -E 'ARM.extab|\.rodata'
python3 _work/reconstructions/gameresource_gen/lsda.py cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch (variant builders, permutation sweeps, LSDA/IR dumpers) lived in `/dev/shm/w4415`:
`buildvar.py`, `sweep_perm2.py`, `sweep_fhy.py`, `sweep_tb.py`, `sweep_tb3.py`, `sweep_shift.py`,
`sweep_2nd.py`, `sweep_combo.py`, `score1.py`, `realaddr.py`, `extab_raw.py`, `emit_ir.sh`.
