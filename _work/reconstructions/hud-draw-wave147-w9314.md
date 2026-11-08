# Hud::draw — wave 147 (w9314): the dock2 LSDA/RA tension is a 3-way register rotation

**Status:** analysis-only, 2026-11-08 (wave 147). Worktree `wt/w9314`,
base `master 9c9744ca5`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

Entry and exit gate (`check`, full): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports **207**,
lint **CLEAN**, drift **0**. **No source change landed** (the base is still the
best measured variant on every metric); this note records the sweep and the
new root cause.

| metric | value |
|---|---:|
| raw same-address diff | **1325** (unchanged) |
| aligned (instruction-index) diff | **249** (unchanged) |
| size | **9332** |
| `.ARM.extab` | **0x26298 (156,312)** exact |

---

## 0. TL;DR

1. **Built a real LSDA decoder** (`/dev/shm/w9314/lsda.py`) that walks
   `.ARM.exidx` -> extab -> LSDA (the extab entry is 4 words here; the LSDA
   sits at `extab+16`, header `ff ff 01`, table length uleb). It reproduces the
   wave-145 numbers exactly (original: 77 entries / 454 B, dock2 tail
   `4/2434`, `4/2432`, `cc/2444`), so the dock2 signature is now a
   first-class sweep metric alongside raw/align.
2. **The w9213 shape's LSDA was right but its register allocation is a
   3-way rotation.** Original dock2 registers: `r4=barOffset`, `r6=top`,
   `r9=Canvas`, `fp=ego`(->`Canvas`), `r5=barYOffset`, `r8=barH`.
   Our merged shapes: `r4=top`, `r6=Canvas`, `r9=barOffset`, `fp=barYOffset`,
   `r8=barH`. i.e. `ours(r4)=orig(r6)`, `ours(r6)=orig(r9)`,
   `ours(r9)=orig(r4)` — a cyclic shift of the same three values, not a spill
   of one value.
3. **The 4 missing bytes are one `ldr` of `Globals::Canvas`, not an epilogue
   instruction.** Per-call-segment size census of the 9328 B shape vs the
   original: the `GetImage2DWidth` setup is 4 instructions (16 B) in ours vs
   3 (12 B) in the original, because ours reloads `Canvas` from `[sp,#120]`
   (`ldr r4,[sp,#120]`) where the original still has it in `r9`. The original's
   `ldrd fp,r9,[sp,#116]` after the dock1 `String` dtor is exactly that
   reload (8 B vs our 4 B `ldr.w fp`), and it is what makes the function
   9332 B.
4. **The merge and the epilogue are mutually exclusive in every shape tried.**
   If `y` is materialised before `SetColor` (which is what makes the allocator
   put `Canvas` in `r9` and emit `ldrd fp,r9`), the merged EH scope costs
   **+36 B** (9368). If `y` is left inline (computed at its first use after
   `SetColor`, matching the original's `add.w r9,r4,r6,lsl #1` at `0x20e8`),
   the EH scope merges at **9328 B** but `Canvas` is reloaded per use.
   This is a real IR ordering difference (`zext/shl/add` before vs after the
   `SetColor` invoke), not just debug info — verified by diffing `-emit-llvm`.
5. **New negative results** (all full-build measured): `__builtin_expect` on
   the dock2 branches, `PaintCanvas *canvas` locals, `float filled` declared
   at every position, `filled` reused from `progress`/`fade`, `int`/`unsigned`/
   `short` variants of `top`/`halfW`/`barH`/`barYOffset`, and `int y;` +
   late assignment. None reaches 9332 B *and* merged LSDA *and* `ldrd fp,r9`.

## 1. LSDA signature metric (new)

`python3 /dev/shm/w9314/lsda.py <so> _ZN3Hud4drawExxP9PlayerEgobjj` prints
`extab=… lsda=… len=… entries=…` plus the tail entries. The dock2 tail
(entries with `start >= 0x2000`) is the signature:

| build | signature |
|---|---|
| **original** | `77\|4/2434 4/2432 cc/2444 e2/0 42/2346 1a4/0` |
| base (HEAD) | `77\|4/2432 2a/243e 7e/2444 e0/0 42/2346 1a4/0` |
| w9213 (`THBY, y inline, filled early`) | `77\|4/2436 4/2434 c2/2446 104/0 3e/2320 184/0` |

Notes on the decoder: the `.ARM.exidx` word1 is a prel31 to the extab entry
relative to *itself*; the 16-byte extab entry here is
`[personality prel31][…][…][…]` and the LSDA is at `extab+16`. The first
`parse_lsda` candidate that parses cleanly (LPStart `0xff`, TType `0xff`,
call-site encoding `1`, table consumed exactly) is the right one.

## 2. The 9328 B shape in detail (`THBY`, `y` inline, `filled` inline)

Source (dock2):

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
            Globals::Canvas->SetColor(fadeColor);
            Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, barOffset + top * 2, 0x11, 0x14);
            Globals::Canvas->DrawRegion2D(this->image_0x37c, (int) ((float) halfW - (progress * (float) halfW)), 0,
                                          (int) ((progress * (float) halfW) + (progress * (float) halfW)),
                                          barH, 0.0f, 0, 0, (int) ((float) (Globals::w >> 1) - (progress * (float) halfW)),
                                          barOffset + top * 2 + barYOffset);
            Globals::Canvas->DrawString(..., (int) ((float) (barOffset + top * 2) + (float) barH * 2.5f), false);
        }
```

Measured: `size=9328 raw=2159 align=422 extab=0x26298 epi=ldr.w`,
LSDA `4/2432 4/2430 c6/2442` (merged, pads `-2` because the body is 4 B short
and the landing pad moved by 2).

Per-call-segment census (`callseg.py`, instructions between consecutive
`blx` calls, 4 B each):

| segment (call reached) | orig | ours | delta |
|---|---:|---:|---:|
| `GetImage2DWidth` setup | 3 | 4 | **+4** |
| `SetColor` setup | 43 | 44 | +4 |
| `DrawImage2D` setup | 14 | 13 | -4 |

The `+4` is `ldr r4,[sp,#120]` (reload of `Globals::Canvas`); the `-4` is the
missing `mov fp,r9`. Net 0 in the dock2 region; the other -4 that makes the
function 9328 comes from the dock1 region.

## 3. Why `y` position flips the allocation

`int y = barOffset + top * 2;` placed **before** `SetColor` emits
`zext/shl/add` *before* the `SetColor` invoke in the IR; the expression
written inline (or `int y;` + late assignment) leaves the `zext/shl/add`
after the invoke (clang sinks it to the first use). Verified with
`clang -S -emit-llvm`; the two IRs differ only in that ordering (plus debug
metadata). Consequences:

| shape | size | raw | align | LSDA | `ldrd fp,r9` |
|---|---:|---:|---:|---|---|
| base (`YBTH`, `y` local, `filled` late) | 9332 | **1325** | **249** | `4/2432 2a/243e 7e/2444` | yes |
| `THBY`, `y` before, `filled` late | 9332 | 1352 | 230 | `4/2434 4/2432 2a/2440 7e/2446` | yes |
| `THBY`, `y` before, `filled` inline | **9368** | 8001 | 897 | `4/245a 4/2458 cc/246a` | yes |
| `THBY`, `y` inline, `filled` inline | **9328** | 2159 | 422 | `4/2432 4/2430 c6/2442` | no |
| `THBY`, `y` inline, `filled` early (w9213) | 9332 | 2224 | 460 | `4/2436 4/2434 c2/2446` | no |
| `THBY`, `y` inline, `filled` inline, `__builtin_expect(fade<1,1)` | 9332 | 2112 | 435 | `4/2434 4/2432 c6/2444` | no |

The `9368` shapes are the merged-LSDA + correct-epilogue combination: they
have the original's `r9=Canvas`/`fp=ego` (well, `fp=Canvas`/`r9=ego` — a
swap) and `ldrd fp,r9`, but an extra 4-byte spill slot shifts every
`[sp,#N>=84]` access and the RA picks wider encodings (+36 B). The extra slot
is inserted below `[sp,#88]`; the first differing instruction in the whole
function is `str r0,[sp,#92]` vs `str r0,[sp,#88]` at func+0x18.

## 4. Negative results (all full-build measured, base = 1325/249/9332)

| variant | raw | align | size | extab |
|---|---:|---:|---:|---|
| `THBY` `y` before `filled` late | 1352 | **230** | 9332 | 0x2629c |
| `THBY` `y` before `filled` inline | 8001 | 897 | 9368 | 0x26298 |
| `THBY` `y` before, `filled` declared at top / reused from `progress` / reused from `fade` | 8001 | 897 | 9368 | 0x26298 |
| `THBY` `y` inline `filled` inline | 2159 | 422 | 9328 | 0x26298 |
| `THBY` `y` inline `filled` early (w9213) | 2224 | 460 | 9332 | 0x26298 |
| `THBY` `y` after SetColor (`int y;` + late assign) | 2159 | 422 | 9328 | 0x26298 |
| `THBY` `y` after SetColor (initialised) | 2226 | 436 | 9336 | 0x262a0 |
| `THBY` `y` after DrawRegion2D | 2129 | 431 | 9332 | 0x2629c |
| `THBY` `y` inline, `PaintCanvas *canvas` local | 8766 | 1387 | 9404 | 0x26298 |
| `THBY` `y` before, `PaintCanvas *canvas` local | 8768 | 1383 | 9400 | 0x26298 |
| `THBY` `y` inline, `unsigned short/unsigned int/short top` | 2159 | 422/423/422 | 9328 | 0x26298 |
| `THBY` `y` inline, `unsigned int barYOffset` | 2159 | 422 | 9328 | 0x26298 |
| `THBY` `y` inline, `unsigned int halfW` | 2159 | 424 | 9328 | 0x26298 |
| `THBY` `y` inline, `short barH` | 8054 | 1023 | 9380 | 0x26298 |
| `THBY` `y` inline, `__builtin_expect(fade<1,1)` | 2112 | 435 | 9332 | 0x26298 |
| `THBY` `y` inline, `__builtin_expect(rate*1.05<1,0)` | 2298 | 455 | 9336 | 0x26298 |
| base + `__builtin_expect(rate*1.05<1,1)` | **1325** | **249** | 9332 | 0x26298 |
| base + `__builtin_expect(rate*1.05<1,0)` | 3764 | 351 | 9340 | 0x26298 |
| base + `__builtin_expect(fade<1,1)` | 1911 | 307 | 9332 | 0x26298 |
| base + `__builtin_expect(show,1)` | **1325** | **249** | 9332 | 0x26298 (`4/2434 2a/2440 7e/2444`) |
| base + `float filled;` hoisted to the outer block | 8001 | 925 | 9368 | 0x26294 |
| `BTHY` `y` before `filled` late | 1328 | 247 | 9332 | 0x26298 |
| `BTYH` `y` before `filled` late | 1326 | 249 | 9332 | 0x26298 |
| `BYTH` `y` before `filled` late | 1327 | 257 | 9332 | 0x26298 |

`base + __builtin_expect(show,1)` is interesting: it changes the first dock2
LSDA pad from `2432` to `2434` (the original's value) at *identical* raw/align
and identical section sizes. It is a free, exact-pad improvement to the LSDA
signature (`4/2434 2a/2440 7e/2444` vs the original `4/2434 4/2432 cc/2444`)
but it does not change the instruction stream, so it is **METRIC-ONLY** and
was not landed (it also has no effect on `byte_exact`).

## 5. Open questions for the next wave

1. **Swap `fp`/`r9` in the 9368 shape.** `THBY y-before filled-inline` has
   `fp=Canvas`, `r9=ego`; the original has `r9=Canvas`, `fp=ego`. If that
   swap can be forced, the extra spill slot may disappear and the shape
   collapses to 9332 with a merged LSDA. Candidate levers: the number of
   `ego` uses in the dock2 region (the original calls `getDriveChargeRate` /
   `getCloakRate` through `fp`), or declaring `int top` before/after
   `barYOffset` (the original's load order is `top` then `barYOffset`).
2. **`y` as a `short`/`unsigned short`** (the original's `top` is
   `unsigned short`-ish: `ldrh.w r6,[sl,#994]`). Not tried for `y`.
3. **`filled` as `int`** (i.e. `(int)(progress * halfW)`) — not tried.
4. The dock1 region is where the extra 4-byte slot is inserted in the 9368
   shapes; the dock1 source is identical in all variants, so the slot is a
   pure RA consequence of the dock2 live ranges. A dock1 source change that
   shortens `text`'s live range across the dtor might remove it.

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9314
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w9314/full.py <variant.cpp>            # size/raw/align/extab/LSDA/epi in one shot
python3 /dev/shm/w9314/lsda.py cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w9314/callseg.py _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w9314`: `fdiff.py`, `dis.py`, `diso.py`,
`lsda.py` (exidx/extab/LSDA decoder), `sphist.py` (stack-slot histogram),
`firstdiff.py`, `callseg.py` (per-call segment census), `gen.py`/`gen2.py`
(source-shape generator), `sweep.py`/`sweep2.py`, `full.py` (one-shot
measurement), `cc.sh` (single-TU clang), `Hud.base.cpp` (HEAD source).
