# Hud::draw — wave 146 (w9213): merged-LSDA dock2 at 9,332 B with the +4 spill slot removed

**Status:** landed pass, 2026-11-08 (wave 146). Worktree `wt/w9213`,
base `master 31297fadc` (wave 145). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **2224** |
| size | 9332 | **9332** |
| `.ARM.extab` | 0x26298 (156,312) | **0x26298 (156,312)** |
| dock2 LSDA rows | `205e 4 2432` / `20e4 2a 243e` / `2130 7e 2444` | **`204c 4 2436` / `2058 4 2434` / `20ec c2 2446`** |
| LSDA entry count | 78 (0..77) | **77 (0..76)** |

Gate (`check`, full): **RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`,
size-exact 4524/4524, `data_exact 843/843`, sodiff 0 extra / 0 missing,
imports **207**, lint **CLEAN**, drift **0**.

---

## 0. TL;DR

1. **The wave-145 handoff's "merged LSDA" candidate (`ydi`) is not actually
   merged.** `ydi` (`filled` after `halfW`, `int y` after `DrawImage2D`) has the
   *same* dock2 call-site structure as the landed base: one 4-byte entry plus
   two ranges (`2060 4 2432` / `20e8 28 243e` / `2136 76 2444`). It is only a
   raw-diff reshuffle (1325 -> 1903), not a structural win.
2. **The original dock2 LSDA is `(4, 4, 0xcc)`**: `GetImage2DWidth` (4 B pad
   `2434`), `GetImage2DHeight` (4 B pad `2432`), then **one** merged range
   `20e0 cc 2444` covering `SetColor`/`DrawImage2D`/`DrawRegion2D`/
   `GetTextWidth`/`DrawString`. The landed base splits the last group in two
   and folds `GetImage2DHeight` into it (78 entries).
3. **The +4 spill slot is the `int y` local.** Inlining `y`
   (`barOffset + top*2` at its three uses; clang CSEs it into `r9` exactly like
   the original's `add.w r9, r4, r6, lsl #1` at `0x20e8`) removes the extra
   `[sp,#80]` slot and restores the `(4, 4, 0xcc)` structure. With `filled`
   computed at its declaration (early, before `this->field_0x464 += dt`) this
   lands at **9332 B / extab 0x26298 / raw 2224**.
4. **The 4-byte epilogue pair is a separate RA artifact, not the LSDA.**
   `filled` declared early but *assigned* late (the faithful shape, matching
   the original's `vmul.f32 s2,s16,s0` at `0x2122`) with inline `y` gives
   **9328 B** (4 short). The missing 4 B is the dock1 `barOffset` phi being
   allocated to `r9` instead of `r4` (`vmov r9,s16` vs `vmov r4,s16` +
   `ldrd fp,r9,[sp,#116]`), i.e. `Globals::Canvas` is spilled to `[sp,#120]`
   in our build but stays in `r9` in the original. Not solved this wave.

---

## 1. LSDA structure (decoded with `lsda.py`)

Original (77 entries, extab `0x1c57cc`, len 454):

| # | start | len | pad |
|---|---:|---:|---:|
| 71 | `2044` | 4 | `2434` |
| 72 | `2064` | 4 | `2432` |
| 73 | `20e0` | `cc` | `2444` |

Landed base (78 entries, len 459, extab `0x26298` — one extra entry is bought
back by one fewer entry elsewhere):

| # | start | len | pad |
|---|---:|---:|---:|
| 71 | `205e` | 4 | `2432` |
| 72 | `20e4` | `2a` | `243e` |
| 73 | `2130` | `7e` | `2444` |
| 74 | `21ae` | `e0` | 0 |

New (77 entries, len 448, extab `0x26298`):

| # | start | len | pad |
|---|---:|---:|---:|
| 71 | `204c` | 4 | `2436` |
| 72 | `2058` | 4 | `2434` |
| 73 | `20ec` | `c2` | `2446` |

Same `(4, 4, one-big-range)` shape as the original. The pads are `+2` because
the function body is 4 B shorter in the dock1 region.

## 2. The source shape that lands

```c++
        if (show) {
            float progress = 1.0f;
            if (rate * 1.05f < 1.0f)
                progress = (this->field_0x277 ? ego->getDriveChargeRate() : ego->getCloakRate()) * 1.05f;
            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            int barYOffset = Globals::layout->field_0x218;
            float filled = progress * (float) halfW;
            this->field_0x464 += dt;
            float fade = (float) this->field_0x464 / 1000.0f;
            int fadeColor = 0xffffffff;
            if (fade < 1.0f)
                fadeColor = (int) (fade * 255.0f) - 256;
            int top = this->field_0x3e2;
            Globals::Canvas->SetColor(fadeColor);
            Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, barOffset + top * 2, 0x11, 0x14);
            Globals::Canvas->DrawRegion2D(this->image_0x37c, (int) ((float) halfW - filled), 0, (int) (filled + filled),
                                          barH, 0.0f, 0, 0, (int) ((float) (Globals::w >> 1) - filled), barOffset + top * 2 + barYOffset);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, text,
                (Globals::w >> 1) -
                    (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1),
                (int) ((float) (barOffset + top * 2) + (float) barH * 2.5f), false);
        }
```

`barOffset` is the outer `int barOffset = 0;` (dock1 phi: `0` or
`(int)(textH + frameH*2.5f)`), matching the original's `r4` phi at `0x1fc0` /
`0x1fae`.

## 3. Measured variants (full build, raw same-address diff)

All with `THBY` declaration order (text, halfW, barH, barYOffset) unless noted.

| shape | raw | size | extab | dock2 LSDA |
|---|---:|---:|---:|---|
| landed base (`YBTH`, `y` local, `filled` after DrawImage2D) | **1325** | 9332 | 0x26298 | 4 / 0x2a / 0x7e |
| `ydi` (YBTH, `filled` after halfW, `y` after DrawImage2D) | 1903 | 9332 | 0x26298 | 4 / 0x28 / 0x76 |
| THBY, `filled` after DrawImage2D, `y` local | 1352 | 9332 | 0x2629c | 4 / 4 / 0x2a / 0x7e |
| THBY, `filled` after DrawImage2D, `y` inline | 1897 | 9332 | 0x2629c | 4 / 4 / 0x28 / 0x7e |
| THBY, `filled` before `y`, `y` local | 8005 | 9368 | 0x26298 | 4 / 4 / 0xc8 |
| THBY, `filled` before `y`, `y` inline | 2185 | **9328** | 0x26298 | 4 / 4 / 0xc2 |
| THBY, `float filled;` early + assign late, `y` inline | 2159 | **9328** | 0x26298 | 4 / 4 / 0xc6 |
| **THBY, `filled` at declaration, `y` inline (LANDED)** | **2224** | **9332** | **0x26298** | **4 / 4 / 0xc2** |

The `9328` shapes are 4 B short of the mandatory size; the 4 B is the dock1
`barOffset` phi (`r4` vs `r9`) described in §0.4. The landed shape buys the
4 B back by computing `filled` at its declaration, which changes the
`progress`/`halfW` live ranges but not the LSDA structure.

## 4. Stack-slot histogram (whole function)

`[sp,#80]` has **3** accesses in both the original and the landed shape
(no extra slot). Remaining differences are `[sp,#120]` 42 vs 38 and
`[sp,#108]` 22 vs 23 — the `Globals::Canvas` spill described in §0.4.

## 5. Caveats (METRIC-ONLY)

- **Raw diff rises 1325 -> 2224.** This is *not* a byte-match win; it is a
  structural (LSDA) win. `byte_exact` is unchanged at 4507 because `Hud::draw`
  was already non-exact.
- `filled` is computed at its declaration (early). The original computes it
  late (`vmul.f32 s2,s16,s0` at `0x2122`, between `DrawImage2D` and
  `DrawRegion2D`). The faithful late-assignment shape is the 9328 B variant.
  If a later wave finds the missing 4 B, prefer `float filled;` + late
  assignment.
- `int y` is inlined in the source; clang CSEs it into `r9`, which matches the
  original's single `add.w r9,r4,r6,lsl #1`.

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9213
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w9213/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w9213/lsda.py cmake-build-match/libgof2hdaa.so 163b90
```

Scratch harness in `/dev/shm/w9213`: `fdiff.py`, `region.py`, `dis2.py`,
`nd.py`, `lsda.py`, `runvar.py`, `sweepA..D.py`, `Hud.base.cpp`,
`B2.so` (landed build), `inl2.so` (9328 B merged-LSDA shape).
