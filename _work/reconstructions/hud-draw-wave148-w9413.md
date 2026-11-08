# Hud::draw — wave 148 (w9413): the `Globals::status` null test is what swaps fp/r9

**Status:** landed pass, 2026-11-08 (wave 148). Worktree `wt/w9413`,
base `master cbe3b5f7f` (wave 147). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **6965** |
| aligned (instruction-index) diff | **249** | **949** |
| size | 9332 | **9332** |
| `.ARM.extab` | 0x26298 (156,312) | **0x26298 (156,312)** |
| dock2 LSDA (rows / table len) | 103 / **459** (split `4/2432 2a/243e 7e/2444`) | 103 / **454** (merged `4/2436 4/2434 ce/2446`) |
| dock2 window nonmatch (0x1f00-0x2200) | 141 | **95** |
| stack frame | 224 | **216** |

Gate (`check`, full): **RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`,
size-exact 4524/4524, `data_exact 843/843`, imports **207**, lint **CLEAN**,
drift **0**, all 27 section sizes exact (`.text` 0x1337c8, `.ARM.extab` 0x26298).

---

## 0. TL;DR

1. **The w9314 handoff's lever was right, but the cause is the `Globals::status`
   null-test anchor, not the dock2 declaration order.** Removing the anchor
   (`if (Globals::status && this->field_0x4b8 >= 1)` -> `if (this->field_0x4b8 >= 1)`)
   from the 9368 shape flips the allocator: the dock2 region now restores
   `r9 = Canvas`, `fp = ego` (the original's assignment; the 9368 shape had
   `fp = Canvas`, `r9 = ego`), and the function collapses from 9368 to **9332 B**.
2. **The extra spill slot disappears with the anchor.** The 9368 shape's frame
   was 224 with the extra `[sp,#80]` function-pointer spill; removing the anchor
   removes the spill, but also drops the frame to **216** (one slot below the
   original's 224). So the swap and the spill were both consequences of the same
   `Globals::status` live range.
3. **The dock2 LSDA is now structurally exact**: table length **454** (was 459),
   103 rows (same as original), and the tail is the original's
   `(4, 4, one merged 0xcc-shaped range)` instead of the base's split
   `(4, 0x2a, 0x7e)`. The pads are `+2` (`2436/2434/2446` vs `2434/2432/2444`)
   because the landing pad sits 2 B later in the dock1 region.
4. **Trade-off:** raw/align regress (1325->6965, 249->949) because the frame is
   8 B short, so every `[sp,#N]` access is shifted. The dock2 *window* itself
   improves (nonmatch 141->95). The base (frame 224, split LSDA) is still the
   best raw/align variant; this pass buys the merged LSDA + correct dock2
   registers at the cost of the frame.

## 1. Root cause: `Globals::status` anchor

The base source carries a deliberate codegen anchor (wave 117 w6516):

```c++
    // The null test is a codegen anchor: the original has no test here, but the extra
    // load/compare re-colours the shield/armour block and keeps the function at its
    // original 9332-byte size.
    if (Globals::status && this->field_0x4b8 >= 1) {
```

The original has **no** null test here (disasm `1642f2: ldr.w r0,[sl,#1208];
cmp r0,#1; blt.n`). Removing it in the 9368 shape:

| shape | size | raw | align | frame | dock2 regs | LSDA len |
|---|---:|---:|---:|---:|---|---:|
| 9368 shape (THBY, y-before, filled-inline) + anchor | 9368 | 8001 | 897 | 224 | fp=Canvas, r9=ego | 454 (merged) |
| **same, anchor removed (LANDED)** | **9332** | **6965** | **949** | **216** | **r9=Canvas, fp=ego** | **454 (merged)** |
| base (YBTH, y local, filled late) + anchor | 9332 | **1325** | **249** | **224** | r9=Canvas, fp=ego | 459 (split) |
| base, anchor removed | 9268 | 7456 | 636 | 224 | — | — |

So the anchor is exactly what kept the base's frame at 224 (matching the
original) *and* what forced the 9368 shape's fp/r9 swap. It is a single live
`Globals::status` value that the RA colours into the dock2 callee-saved pair.

## 2. The landed source shape

`THBY` declaration order (text, halfW, barH, barYOffset), `int y` before
`SetColor`, `filled` written inline at its three uses:

```c++
        if (show) {
            float progress = 1.0f;
            if (rate * 1.05f < 1.0f)
                progress = (this->field_0x277 ? ego->getDriveChargeRate() : ego->getCloakRate()) * 1.05f;
            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            int barYOffset = Globals::layout->field_0x218;
            this->field_0x464 += dt;
            float fade = (float) this->field_0x464 / 1000.0f;
            int fadeColor = 0xffffffff;
            if (fade < 1.0f)
                fadeColor = (int) (fade * 255.0f) - 256;
            int top = this->field_0x3e2;
            int y = barOffset + top * 2;
            Globals::Canvas->SetColor(fadeColor);
            Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, y, 0x11, 0x14);
            Globals::Canvas->DrawRegion2D(this->image_0x37c, (int) ((float) halfW - (progress * (float) halfW)), 0,
                                          (int) ((progress * (float) halfW) + (progress * (float) halfW)),
                                          barH, 0.0f, 0, 0, (int) ((float) (Globals::w >> 1) - (progress * (float) halfW)),
                                          y + barYOffset);
            Globals::Canvas->DrawString(..., (int) ((float) y + (float) barH * 2.5f), false);
        }
```

## 3. Negative results (full build, base = 1325 / 249 / 9332)

| variant | raw | align | size | frame |
|---|---:|---:|---:|---:|
| 9368 shape + `int top` before `barYOffset` | 8001 | 895 | 9368 | 224 |
| 9368 shape + `int top` before `barH` | 7998 | 897 | 9368 | 224 |
| 9368 shape + `int top` before `this->field_0x464 += dt` | 8001 | 895 | 9368 | 224 |
| 9368 shape + anchor reversed (`field_0x4b8 >= 1 && Globals::status`) | 8001 | — | 9368 | 224 |
| 9368 shape + anchor `Globals::status != 0 && ...` | 8001 | — | 9368 | 224 |
| 9368 shape, anchor removed + `__builtin_expect(show,1)` | 6966 | — | 9332 | 216 |
| 9368 shape, anchor removed + `__builtin_expect(rate*1.05<1,0)` | 7125 | — | 9336 | — |
| 9368 shape, anchor removed + `__builtin_expect(fade<1,0)` | 7114 | — | 9336 | — |
| base + `int top` first (TBYH) | 1372 | 249 | 9332 | 224 |

None of the `int top`/`barYOffset` reorderings change the fp/r9 assignment or
the frame; only the `Globals::status` live range does.

## 4. Open questions for the next wave

1. **Recover the last 4-byte frame slot.** The original is 224, the landed shape
   216. The original's stack-slot histogram matches the *base* exactly
   (`60..120`, `160`, `220`; 16 local slots) while the landed shape has 15
   (`56..112`, `152`, `212`). Adding one 4-byte spill (8-byte frame alignment)
   would shift every `[sp,#N]` by +8 and should collapse the raw diff. The
   original also swaps the `ego` (#116) and `Globals::globals` (#120) slots
   relative to the landed shape (#112 / #108), so the new slot must land
   *between* them.
2. **The faithful shape is the landed one** (no null test, merged LSDA,
   correct dock2 registers); the base is a metric-only overfit. If the frame can
   be recovered without re-introducing the anchor, the landed shape should win
   on all three metrics.
3. The `filled` inline form matches the original's `vmul.f32 s2,s16,s0` at
   `0x2122` (computed between DrawImage2D and DrawRegion2D) — the wave-146
   "computed at declaration" workaround is no longer needed.

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9413
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w9413/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w9413/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
```
