# Hud::draw — wave-114: the dock order / second-bar LSDA merge, measured — w6217

**Status:** analysis-only pass, 2026-11-05 (wave 114). Worktree `wt/w6217`,
branch `wave114-w6217`, base `master d8557f7e2` (wave-113 harvest). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

**No source change was landed.** The landed wave-112 shape (`T P B H W D` dock
order, `short frameH` / `short transferred`) is still the best fuzzy score at
**94.9 %, 9332 B**. Every faithful / merged variant measured below is a
regression on the fuzzy metric and/or on the section state.

Gate on entry and exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
**RATCHET PASS**. Sections unchanged: `.text 0x1337c8`, `.ARM.extab 0x26298`,
file size **2,187,328 B**, 27 sections.

This note extends `hud-draw-wave113-w6116.md`, `hud-draw-wave112-w6014.md`,
`hud-draw-wave111-w5916.md`, `hud-draw-wave109-w5717.md` and
`hud-draw-oracle-dock-w103-wf315.md`.

---

## 0. TL;DR

1. **The second-bar LSDA merge is real and gives an EXACT Hud record.** Moving
   the `float filled` computation out from between `DrawImage2D` and
   `DrawRegion2D` (hoist the declaration above `SetColor`, or inline the
   expression) makes clang emit one `landingpad` for `SetColor` +
   `DrawImage2D` + `DrawRegion2D`, and the Hud LSDA becomes **476 B / 454 B
   call-site table — byte-for-byte the original's row count and table length**.
2. **But it costs 11 fuzzy points and 24 bytes.** The merge removes one
   landing-pad `MachineBasicBlock`, which re-colours the whole stack frame
   (`ego #116 -> #120`, `&Canvas #120 -> #116`, the four int params
   `#64..#76 -> #60..#72`) and grows the function to **9356 B**. Score
   **83.8 %**, `.text 0x1337e8`.
3. **`.ARM.extab` then becomes `0x26294`** because Generator's `-4` is no
   longer cancelled. Hud is exact; Generator::createMission is still
   `168` vs `172` (it is only 82.6 % itself, so its LSDA delta is a symptom of
   its own code layout, not a Hud problem).
4. **No dock-block shape adds the missing +4.** A full 120-permutation sweep
   of the dock statement order *with the merge* yields call-site table lengths
   of only **454** (the exact value) or less — never 458. Moving the
   `dt/fade` block through all six positions likewise gives 448/454. So the
   "dock order needs +4 extab" hypothesis does not hold for any statement
   permutation of this source.
5. **The faithful dock order is nearly byte-identical, but 8 B short.** With
   `int frameH` / `int transferred` and `P B T W H`, the dock block from
   `0x165880` to `0x16594e` matches the original instruction-for-instruction
   (including the deferred `operator+` at `0x165926`); only the
   `frameH`/font-GOT spill slots are swapped and `barW`/`barH` land in
   `r6`/`r8` instead of `r8`/`fp`. That 8 B deficit re-colours every later
   branch target and scores **93.6 %**.

---

## 1. Baseline (landed, wave 112)

```c++
short frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int   textH  = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
short transferred = ego->getDockTransferedAmount();
int   total  = ego->getDockTotalAmount();
String text  = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
float progress = (float) transferred / (float) total;
if (this->field_0x279) progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
```

| metric | value |
|---|---:|
| size | **9332 B** |
| fuzzy | **94.9 %** |
| Hud LSDA record / table | **480 / 459** (orig 476 / 454) |
| `.ARM.extab` | **0x26298** (exact) |
| `.text` | **0x1337c8** (exact) |

The 78-vs-77 call-site row delta is the second bar:

```
orig : (8416, 204, 9284, 0)                         ; SetColor+DrawImage2D+DrawRegion2D+GetTextWidth+DrawString
ours : (8420,  42, 9280, 0) (8496, 126, 9286, 0)    ; split after DrawImage2D
```

The split is caused by the `float filled = progress * (float) halfW;`
statement sitting between the two `invoke`s: clang caches a *different*
`landingpad` on the innermost EH scope for the `DrawRegion2D` invoke.  The
extra landing pad is the extra 2-byte `b.n` stub at `0x165fd4`, which is why
the final cleanup lands at `0x165fd6` (ours) instead of `0x165fd4` (orig) and
why ~40 trailing `b.n loc_XXXX` instructions differ by 2.

---

## 2. The merge: Hud LSDA becomes exact, everything else regresses

Three real-C++ forms all merge the row:

```c++
// (a) hoist the declaration above SetColor, assign after DrawImage2D
float filled;
...
Globals::Canvas->SetColor(fadeColor);
Globals::Canvas->DrawImage2D(...);
filled = progress * (float) halfW;

// (b) initialise before SetColor
float filled = progress * (float) halfW;
Globals::Canvas->SetColor(fadeColor);

// (c) inline `progress * (float) halfW` in all three uses
```

| shape | size | fuzzy | Hud rec/table | `.ARM.extab` | `.text` |
|---|---:|---:|---:|---:|---:|
| landed (no merge) | 9332 | **94.9** | 480 / 459 | **0x26298** | **0x1337c8** |
| (a) `float filled;` hoisted, assigned after DrawImage2D | 9356 | 84.3 | **476 / 454** | 0x26294 | 0x1337e8 |
| (b) initialise before `SetColor` | 9356 | 83.8 | **476 / 454** | 0x26294 | 0x1337e8 |
| (c) inline, no local | 9356 | 84.3 | **476 / 454** | 0x26294 | 0x1337e8 |

The merged IR is *identical* to the baseline except that the second-bar
`landingpad` blocks are folded (`%2788` absorbs `%2757/%2772/%2777`, `%2790`
disappears).  That single MBB removal re-orders the spill-slot creation and
therefore the whole frame:

```
orig / landed :  ego #116, &Canvas #120, params #68 #64 #72 #76
merged        :  ego #120, &Canvas #116, params #64 #60 #68 #72
```

and the function grows by 24 B (9356).  The second-bar body itself becomes
almost byte-identical to the original (`vcvt.f32.s32 s0,s18` /
`vmul.f32 s2,s16,s0` land after `DrawImage2D` exactly as in the original), but
the global re-colour costs far more than it gains.

### 2.1 Dock-order sweep *with* the merge (120 permutations)

Every permutation of `{T,P,B,W,H}` (with `dt/fade` last) was built with form
(a).  Result: **call-site table is 454 for every permutation that keeps the
merge** (a few orders drop to 448/442 when the merge is lost).  Best rows:

| dock order | size | fuzzy | Hud rec/table |
|---|---:|---:|---:|
| `P B T W H` | 9348 | **83.9** | 476 / 454 |
| `P B T H W` | 9348 | 83.9 | 476 / 454 |
| `B P T W H` | 9348 | 83.9 | 476 / 454 |
| `B P T H W` | 9348 | 83.9 | 476 / 454 |
| `T P B H W` | 9348 | 83.7 | 476 / 454 |

Moving the `dt/fade`/`top`/`barYOffset` block through all six positions with
the merge gives 448 or 454, never 458.  **No dock statement order adds the
+4 that would keep `.ARM.extab` at `0x26298`.**

---

## 3. Faithful dock order (no merge)

```c++
int frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int textH  = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
int transferred = ego->getDockTransferedAmount();
int total  = ego->getDockTotalAmount();
float progress = (float) transferred / (float) total;
if (this->field_0x279) progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

| metric | value |
|---|---:|
| size | 9324 B (-8) |
| fuzzy | 93.6 % |
| Hud LSDA | 480 / 459 |
| `.ARM.extab` | 0x26298 |

Disassembly comparison of `0x165880..0x16594e` is instruction-identical to the
original, including `mov r5,fp; mov fp,r9; mov r9,r0; mov r0,r5`, the deferred
`operator+` at `0x165926`, `GetImage2DWidth` at `0x16593e` and
`GetImage2DHeight` at `0x16594e`.  The only deltas are:

1. `frameH` / `Globals::font`-GOT spill slots swapped (`#96`/`#92`);
2. `mov r6,r0` (barW) where the original has `mov r8,r0`;
3. the missing `mov r5,fp` (Canvas save) before `GetImage2DHeight`, so
   `barH` goes to `r8` instead of `fp` and Canvas stays in `fp`.

Those three deltas account for the whole 8 B deficit; the missing 2-byte
`mov r5,fp` plus the `r6`-vs-`r8` / `r8`-vs-`fp` encoding differences
re-colour every downstream branch target (the 8 B shift alone costs ~45
instruction matches).

### 3.1 Type cross-product on the faithful order

`frameH`, `transferred`, `barW`, `barH` each `int` or `short`
(16 combinations, both `P B T H W` and `P B T W H`):

| best rows | size | fuzzy |
|---|---:|---:|
| `int/int/int/short` (`P B T H W` or `P B T W H`) | 9332 | **94.9** |
| `int/int/short/short` | 9332 | 94.9 |
| `short/short/int/int` | 9324 | 93.5 |
| `int/int/int/int` | 9324 | 93.6 |

`int/int/int/short` is size-exact but puts `barW` in `r5` and `barH` in `r8`
(still not `r8`/`fp`), so the dock block is not closer to the original than
the landed shape and the fuzzy score is unchanged at 94.9.

Other declaration forms tried on the faithful order (all 9324 B, 93.6 % unless
noted): `int barW, barH;` then assign W/H or H/W (9324, rec 476/453),
`const int`, `long`, `unsigned int` (9320), `auto`, `(int)` cast, separate
`int barW; int barH;`, `PaintCanvas *canvas = Globals::Canvas;` local (9324,
93.5-93.6).

---

## 4. Other second-bar forms (no merge)

| shape | size | fuzzy | Hud rec |
|---|---:|---:|---:|
| landed `float filled = progress * (float) halfW;` | 9332 | **94.9** | 480 |
| `float filled(progress * (float) halfW);` | 9332 | 94.9 | 480 |
| `auto` / `const auto` / `(float)` cast / extra parens | 9332 | 94.9 | 480 |
| `float filled = 0.0f; filled = progress * (float) halfW;` | 9332 | 94.9 | 480 |
| `volatile float filled` | 9352 | 93.4 | 480 |
| `double filled` | 9344 | 93.4 | 480 |
| `float halfW` instead of `int halfW` | 9324 | 93.4 | 480 |
| `int fadeColor; if/else` (both arms) | 9332 | 94.9 | 480 |
| `fadeColor` ternary | 9332 | 94.6 | 480 |
| `int y = top * 2 + barOffset;` / `barOffset + (top << 1)` | 9332 | 94.9 | 480 |
| `String text` moved after `barH`/`top` | 9316 | 90.0 | 468 |
| `String text` moved before `if (show)` | 9336 | 93.7 | 488 |
| `int y` after `SetColor` | 9324 | 90.6 | 488 |

---

## 5. Proven vs hypothesis

**Proven**

* The original's second bar has **one** landing pad for `SetColor`,
  `DrawImage2D`, `DrawRegion2D`, `GetTextWidth`, `DrawString`
  (`(8416,204,9284,0)`); ours has two because the `filled` statement sits
  between the two invokes.
* Hoisting / inlining `filled` makes the Hud LSDA **exactly** 476 B record /
  454 B table (77 rows, same as the original).
* That merge always costs 24 B and re-colours the frame; the best merged fuzzy
  score is **83.9 %**.
* No statement permutation of the dock block (120 orders, plus all 6 positions
  of the `dt/fade` block) produces a merged table length other than 454.
* The faithful dock order (`P B T W H`, `int`/`int`) reproduces the original
  dock block almost instruction-for-instruction but is 8 B short and scores
  93.6 %.

**Hypothesis (not landed)**

The remaining Hud LSDA +4 and the dock-block `barW -> r8` / `barH -> fp`
allocation are the same clang-7 `-Oz` stack-slot / register-allocation
tie-break.  A source shape that (i) merges the second-bar row *and* (ii)
keeps the landed frame colouring would score well above 95 %, but it must
also restore the +4 that Generator::createMission is missing.  Since
Generator::createMission is itself only 82.6 % and its LSDA delta is a
symptom of its own code layout, fixing the Hud merge in isolation regresses
`.ARM.extab` to `0x26294` and `.text` to `0x1337e8`.

---

## 6. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch harness (outside the repo): `/dev/shm/w6217/` — `Hud_baseline.cpp`,
`vtest.py` (dock-block/statement replacement + build + LSDA dump),
`sweep_merge.py` / `sweep_merge_results.json` (120-permutation merge sweep),
`lsdadump.py` (per-row LSDA dump), `align.py` (aligned instruction diff),
`landmarks.py` (call-sequence / address-delta table), `mkir.sh` (IR dump).

---

## 7. Follow-up sweeps (same pass, w6217)

These extend §2–4; every build used the same `link_so` + `score.py` +
`exidx_table.py` harness. **None beat the landed 94.9 % / 9332 B.**

### 7.1 Declaration-order sweep, no merge (120 permutations, `int` types)

All of `{T,P,B,W,H}` with `int frameH` / `int transferred` / `int barW` /
`int barH`:

| best orders | size | fuzzy | Hud rec/table |
|---|---:|---:|---:|
| `P B T H W`, `B P T H W` | 9324 | **93.8** | 480 / 459 |
| `P W T H B`, `B T H W P` | 9332 | 93.8 | 476 / 453 |
| `T P B H W`, `T B P H W`, `P B T W H`, `B P T W H` | 9324 | 93.6 | 480 / 459 |

The rest fall to 85–93 %. So the `int`-typed declaration order alone cannot
recover the landed score.

### 7.2 Type cross-product *with* the merge (`filled` hoisted)

Same 4 orders × `{int,short}` for `frameH`/`transferred`/`barW`/`barH` (64
builds). **Every** one gives Hud record **476 / 454** and `.ARM.extab`
**0x26294**; sizes 9348–9368, fuzzy **83.1–84.2 %**. Best:
`P B T W H`, `int/int/short/int` → 9352 B, 84.2 %. The merge is therefore
independent of the dock types/order; it always re-colours the frame.

### 7.3 Second-bar statement order and types

With the faithful `P B T W H` dock block (`int`/`int`), varying
`top`/`barYOffset` (`int`, `short`, `unsigned short`, `unsigned int`), their
declaration order, and `y = barOffset + top*2` vs `top*2 + barOffset`: all
**9324 B / 93.6 %**. Moving the `dt`/`fade`/`fadeColor`/`y` statements through
targeted orders: 93.4–93.6 %. `fadeColor` as `unsigned int` / `-1`, `fade` as
`double` / `* 0.001f`: 93.3–94.9 % on the landed block, never above 94.9 %.

### 7.4 `String` construction forms

`(*getText(...)) + String(" ")`, `String(" ") + *getText(...)`,
`String text; text = ...`, `... + " "`, `String(" ", false)`: 93.0–93.6 %
(faithful dock block). The landed form is already the best.

### 7.5 Where the landed 94.9 % actually differs (measured)

Aligning the landed build against the original instruction-by-instruction
(`align4.py`-style, address-tagged) shows the **jump/cloak bar (the second
bar, `0x165b50`–`0x165d4c`) is almost identical**; only three register
choices differ:

| value | original | landed |
|---|---|---|
| `barYOffset` | `r5` | `fp` |
| `fadeColor` phi | `r1` | `r2` (then `mov r1,r2`) |
| `&Globals::Canvas` | `fp` | `r5` (and `ldr r0,[sp,#120]` reload) |

Everything else in that bar (barH→`r8`, top→`r6`, `barOffset`→`r4`,
`y = barOffset + top*2`→`r9`, `halfW`→`s18`, `progress`→`s16`) matches.
Those three choices are a cascade from the **docking bar's** `mov r5,r9`
(landed) vs `mov fp,r9` (original) — i.e. which callee-saved register holds
`&Globals::Canvas` across the docking bar. The resulting encoding deltas
(`ldr.w r0,[fp]` 4 B vs `ldr r0,[r5]` 2 B, `mov r9,fp; ldr.w fp,…` 6 B vs
`ldrd fp,r9,…` 4 B, and the extra `mov r1,r2`) are what shift every tail
branch target by 2 B and cost most of the remaining 5.1 %.

Two smaller independent deltas also remain and are *not* dock-related:

* `0x164010` (`Status *st = Globals::status;` block): the original keeps
  `&Globals::status` in `r6` and spills it after `getIndex`; ours spills it
  immediately and reuses `r0`. Same size, different schedule.
* `0x164e76`: `ldrd r5,r8,[r1,#500]` (original) vs `ldrd r8,r5,[r1,#500]`
  (ours) — a pure two-register swap for `layout->field_0x1f4` /
  `layout->field_0x1f8` in the `getType()==184` HUD branch
  (`Hud.cpp:1352`); the arithmetic is identical.

The faithful `P B T W H` variant fixes the docking bar's `&Canvas` register
(`mov fp,r9`) and is instruction-identical from `0x165880` to `0x16593e`
(except the `frameH`/font-GOT spill slots `#96`/`#92`), but it then picks
`barW→r6` (orig `r8`), omits `mov r5,fp`, and lands `barH→r8` (orig `fp`),
so it is 8 B short and scores 93.6 %. No source form tried (declaration order,
types, casts, `String` forms, `fadeColor` forms, statement order) makes clang
choose `barW→r8`/`barH→fp` while keeping the landed frame.

### 7.6 Artifacts

Scratch harnesses (outside the repo, `/dev/shm/w6217/`): `vtest.py`,
`score.py`, `align2.py`/`align3.py`/`align4.py` (address-tagged alignment),
`batch1.py`–`batch8.py` + `batch*.json` (the sweeps above),
`build_faithful.py` / `faithful.so` (the 9324 B faithful build).
