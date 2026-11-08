# Hud::draw — wave 144 (w8915): full LSDA decode and the THBY rematerialization wall

**Status:** analysis-only, 2026-11-08 (wave 144). Worktree `wt/w8915`,
base `master f5093474f` (wave 143). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports **207**,
lint **CLEAN**, drift **0**, symbol parity 5370/5370.

Raw same-address diff (primary metric): **1325 -> 1325 B** (no source change
landed; function stays **9332 B**). `.ARM.extab` stays **156,312 B**
(`0x26298`), `.rodata` stays `0x1c9e10`, `.rel.plt` byte-identical.

---

## 0. TL;DR

1. Re-diffed the full Thumb listing after the wave-143 YBTH landing: the raw
   diff is **1325** in **117 contiguous runs**. The largest run is still
   **82 B at func+0x2025** (dock2 `barYOffset`/`halfW`/`text`/`barH` order),
   then 75 B at `0x1eaa` (dock1 register rotation), 65 B at `0x696` (damage
   4-byte shift), 52 B at `0x1dbc` (dock1 `mov r5,fp`), 50 B at `0x1fbb`
   (dock2 first-block tail), 48 B at `0x62f`, 48 B at `0x1f1e`, 46 B at
   `0x7b4`, 41 B at `0x217a`.
2. Decoded `Hud::draw`'s LSDA call-site table exactly (see §2). The original
   has **77 entries / 454 B**; the landed YBTH build also has **77 / 453**;
   the faithful THBY build has **78 / 459**. The extra THBY entry is the
   `filled` split pair (`0x20e4 len 0x2a` + `0x2130 len 0x7e`); the original
   merges it into `0x20e0 len 0xcc`.
3. THBY is the *faithful* asm order (`text, halfW, barH, barYOffset`) and it
   fixes the 82-byte run: `barYOffset` lands in `r5` (not `fp`) and
   `Globals::Canvas` stays in `fp`, matching the original. But THBY is
   **1352 raw / extab +4** because of the extra call-site entry.
4. **New wall:** every source shape that removes the split (declares
   `filled` before `SetColor`, or reuses `fade`/`progress`, or inlines the
   expression) merges the LSDA and gives THBY `extab 0x26298` exact — but
   the backend then **rematerializes `progress`** (duplicates the
   `getDriveChargeRate`/`getCloakRate` chain), growing the code to **9368 B**
   and the raw diff to ~8000. The IR is identical except for the
   `DrawRegion2D` unwind label; the rematerialization is a pure backend RA
   decision. No source spelling tried (const/volatile/`= 0.0f`/`*=`/inline/
   ternary) avoids it.
5. All type/order tie-breaks (`unsigned int barYOffset`, `const int
   barYOffset`, dock1 `barW`/`barH` swap, dock1 `unsigned short barH`,
   `unsigned short top`) measure exactly **1325** — the metric is quantized
   and none changes the differing bytes. `int barH` (dock1 or dock2) grows
   the function to 9324/9320 and is worse.

## 1. Contiguous same-address hunk census (base, 1325)

| region | func offset | raw diff |
|---|---|---:|
| regen `pivotY` trace | `0x467` | 1 |
| gamma body | `0x4c5`-`0x4f0` | 5 |
| damage indicators | `0x5e6`-`0x7e2` | 481 |
| literal pool (damage) | `0x974`-`0x990` | 8 |
| dock1 (docking transfer bar) | `0x1cd6`-`0x1f4e` | 326 |
| dock2 (jump-drive/cloak bar) | `0x1f50`-`0x21b8` | 495 |
| EH cleanup pad | `0x242c`-`0x243e` | 7 |
| literal pool (tail) | `0x245c`-`0x2460` | 2 |
| **total** | | **1325** |

Largest contiguous runs:

| raw | func offset | character |
|---:|---|---|
| 82 | `0x2025`-`0x2077` | dock2 `barYOffset` early (fp) / `halfW` before `text` / Canvas spilled |
| 75 | `0x1eaa`-`0x1ef4` | dock1 `vmov s0,fp` vs `r4`; `asrs` vs `mov.w asr` |
| 65 | `0x696`-`0x6d6` | damage block-2 4-byte shift |
| 52 | `0x1dbc`-`0x1def` | dock1 `mov r5,fp` / `barW`/`barH` register swap |
| 50 | `0x1fbb`-`0x1fec` | dock2 first-block `add.w` schedule |
| 48 | `0x62f`-`0x65e` | damage block-1 4-byte shift |
| 48 | `0x1f1e`-`0x1f4d` | dock1 `DrawImage2D` tail |
| 46 | `0x7b4`-`0x7e1` | damage block-4 rotation |
| 41 | `0x217a`-`0x21a3` | dock2 second-block tail: Canvas loaded from `[sp,#120]` vs `[fp]` |

The 82-byte and 41-byte runs are the same root cause: in the landed YBTH,
`int barYOffset` is declared first and is loaded early into `fp`; `fp` was
the original's `Globals::Canvas`, so Canvas is spilled to `[sp,#120]`. In
the original, `barYOffset` is loaded late into `r5` and Canvas stays in `fp`.

## 2. Exact LSDA call-site decode

`.ARM.exidx` entry `0x1a1e60` -> `.ARM.extab` `0x1c57cc` -> LSDA `0x1c57dc`
(`ff ff 01 c6 03` = LPStart omit, TType omit, call-site encoding 1,
uleb table length). Each entry is four ulebs `(start, length, landing-pad,
action)`.

| build | table len | entries | dock2 entries |
|---|---:|---:|---|
| original | 454 | **77** | `2044 4 2434` (width), `2064 4 2432` (height), `20e0 cc 2444` (merged draw) |
| YBTH (landed) | 453 | **77** | `2058 4 242a`, `20dc 2a 2436`, `2128 7e 243c` |
| THBY (faithful) | 459 | **78** | `2048 4 2434`, `2068 4 2432`, `20e4 2a 2440`, `2130 7e 2446` |
| THBY + merged split | 459-? | **77** | width, height, `20e4 cc 2444` — extab **0x26298 exact** |

The original's `GetImage2DWidth`/`GetImage2DHeight` are separate 4-byte
entries; the whole `SetColor`/`DrawImage2D`/`DrawRegion2D`/`GetTextWidth`/
`DrawString` sequence is one `0xcc` range. THBY reproduces the width/height
entries but splits the draw range at the `filled` VarDecl. YBTH has the
right count only because it merges the width/height entries instead.

## 3. The rematerialization wall (NEW)

`float filled = progress * (float) halfW;` is a scalar SSA value: it emits no
`llvm.lifetime.start` and no alloca. But `EmitAutoVarAlloca` pushes a
`NormalEHLifetimeMarker` non-EH scope at the VarDecl, and
`getInvokeDestImpl` caches the landing pad on `EHStack.begin()`. Declaring
`filled` between `DrawImage2D` and `DrawRegion2D` therefore splits the draw
range into two landing pads.

Moving the VarDecl before `SetColor` merges the pads (verified in the IR:
`DrawRegion2D` unwinds to `%2798` instead of `%2800`), and for THBY gives
`extab 0x26298` exact. But the backend then rematerializes `progress`:

```
; base YBTH: progress computed once into s16
2012: vmul.f32 s16, s0, s18
...
2122: vmul.f32 s2, s16, s0        ; filled = progress * halfW

; merged-split: progress recomputed
2016: vmul.f32 s0, s0, s18        ; for the <1.0 check
2024: ldrb.w r0,[sl,#0x277]       ; <- duplicate rate chain
202c: blx getDriveChargeRate
2034: blx getCloakRate
203c: vmul.f32 s16, s0, s18       ; progress again
```

The duplicate chain adds 36 bytes (9332 -> 9368), shifting every later
address and exploding the raw diff to ~8000. The IR is byte-identical
except for the unwind label, so this is a register-allocation/scheduling
decision, not a source-level IR difference.

## 4. Negative results (all measured, base 1325 unless noted)

| variant | raw | size | `.ARM.extab` |
|---|---:|---:|---:|
| base YBTH | **1325** | **9332** | **0x26298** |
| THBY (faithful) | 1352 | 9332 | 0x2629c |
| THBY + `float filled;` before `barH` | 8001 | 9368 | **0x26298** |
| THBY + `float filled;` after `barYOffset` | 8001 | 9368 | **0x26298** |
| THBY + `float filled = 0.0f;` after `barYOffset` | 8001 | 9368 | **0x26298** |
| THBY + `float filled = ...` after `barYOffset` | 8020 | 9372 | **0x26298** |
| THBY + `float filled = ...` after `halfW` | 8002 | 9368 | **0x26298** |
| THBY + `filled = (rate*1.05<1 ? rate*1.05 : 1)*halfW` | 7994 | 9372 | **0x26298** |
| THBY + `volatile float progress` | 8020 | 9380 | **0x26298** |
| THBY + non-phi `progress = rate*1.05; if (!(<1)) = 1` | 8021 | 9344 | **0x26298** |
| YBTH + `float filled;` before `barH`/`text`/`halfW` | 8001 | 9368 | 0x26294 |
| YBTH + inline `filled` (4x expression) | 8001 | 9368 | 0x26294 |
| YBTH + `progress *= (float) halfW` | 8001 | 9368 | 0x26294 |
| YBTH + reuse `fade` as `filled` | 8001 | 9368 | 0x26294 |
| YBTH + `float filled;` after `SetColor` | 3713 | 9336 | 0x26298 |
| YBTH + `float filled;` after `y` | 1442 | 9332 | 0x2629c |
| YBTH + `PaintCanvas *canvas` local | 8485 | 9356 | 0x26298 |
| dock2 `unsigned int barYOffset` | 1325 | 9332 | 0x26298 |
| dock2 `const int barYOffset` | 1325 | 9332 | 0x26298 |
| dock2 `int barH` / `short barH` | 1956 / 3501 | 9324 / 9320 | 0x26298 |
| dock1 `barW`/`barH` swap | 1325 | 9332 | 0x26298 |
| dock1 `unsigned short barH` | 1325 | 9332 | 0x26298 |
| dock1 `int barH` | 1956 | 9324 | 0x26298 |
| dock1 `int barYOffset` (first block) | 1325 | 9332 | 0x26298 |
| dock1 `unsigned short top` | 1325 | 9332 | 0x26298 |

## 5. Open questions for the next wave

1. The rematerialization is the only thing standing between THBY and a
   ~1300 raw diff with exact extab. A source shape that keeps `progress` in
   `s16` across the merged draw range would win. Candidates: a different
   `filled` declaration form that does not push a non-EH scope (e.g., a
   compound statement / helper local in an inner scope), or a `progress`
   spelling that the backend cannot rematerialize (e.g., a value with a
   single definition and no phi).
2. The damage 481 is still the 4-byte shift from the two dead
   `ldr.w r8,[sp,#108]` loads plus the `Globals::status &&` size anchor.
   A source shape that produces the dead reloads (e.g., forcing
   `sideImage`/`frontImage` to spill) would collapse the largest region.
3. Dock1's 52-byte `mov r5,fp` run is the same `barW`/`barH` register swap;
   the original uses `barW=r8, barH=fp`, ours `barW=r5, barH=r8`.

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8915
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8915/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w8915/lsda2.py                          # call-site decode
```

Scratch harness in `/dev/shm/w8915`: `fdiff.py`, `dump.py` (contiguous runs),
`seq2.py` (full side-by-side Thumb listing with PLT names), `regions.py`
(region sums), `lsda2.py` (4-uleb call-site decoder), `variant.py`
(single-file patch + build), `measure.sh`, `Hud.base.cpp` (wave-143 YBTH).
