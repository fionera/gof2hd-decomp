# Hud::draw — wave 145 (w9015): the merged-LSDA wall is a +4 spill slot, not `progress` rematerialisation

**Status:** analysis-only, 2026-11-08 (wave 145). Worktree `wt/w9015`,
base `master eb13ada9b` (wave 144). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports **207**,
lint **CLEAN**, drift **0**, symbol parity 5370/5370.

Raw same-address diff (primary metric): **1325 -> 1325 B** (no source change
landed; function stays **9332 B**). `.ARM.extab` stays **156,312 B**
(`0x26298`), `.rodata` stays `0x41b67`, `.rel.plt` byte-identical.

---

## 0. TL;DR

1. **The wave-144 premise is wrong.** `progress` is *already* kept in `s16`
   across the merged draw range in every merged source shape. The wave-144
   note read the source's own `progress` phi (`1.0f` / `rate*1.05f` recompute)
   as backend "rematerialisation". In `THBY + merged` the dock2 instruction
   stream is **identical instruction-for-instruction to the original**
   (`vmul.f32 s2, s16, s0` for `filled`, `s16` held across `SetColor`/
   `DrawImage2D`).
2. **The real blocker is a +4 stack slot.** Every source shape that merges the
   `filled` landing pads (declaring `filled` before `SetColor`, or inlining it)
   inserts one extra 4-byte spill slot at `[sp,#80]` and swaps slots
   `[sp,#116]`/`[sp,#120]`. That shifts every `[sp,#N]` access from `N>=80`
   and rotates the callee-saved registers (`sl`/`fp`/`r8`/`r6`/`r9`) in the
   **damage** region. Result: **9368 B / 8001 raw** even though the dock2
   region and the LSDA structure are perfect. The IR diff between the split
   and merged builds is *only* the landing-pad block (verified with
   `-emit-llvm`); the +4 is a pure backend RA consequence of the merge.
3. **NEW lead: drop the `int y` local.** Inlining `y` (`barOffset + top*2`) at
   its two uses keeps the original's 224-byte frame *and* the merged LSDA:
   **9328 B, `.ARM.extab` 0x26298 exact, raw 2159**. It is 4 bytes short of
   9332 because the dock1→dock2 epilogue then restores only `fp`
   (`ldr.w fp,[sp,#116]`) where the original restores `fp,r9` + `r8`
   (`ldrd fp,r9,[sp,#116]` / `ldr.w r8,[sp,#108]`). Adding any local back
   before `SetColor` re-inserts the +4 slot (8001 again).
4. **NEW lead: declare `y` after `DrawRegion2D`.** `int y = barOffset + top*2;`
   placed between `DrawRegion2D` and `DrawString` gives **9332 B / 2129 raw**
   but splits the LSDA at `y` (78 entries, `.ARM.extab` 0x2629c). So the
   merged range and the 4-byte epilogue are mutually exclusive in every shape
   tried.
5. All **24 dock2 local permutations** with an inlined `filled` give **9368 B**
   (raw 7997–8025). The merge always costs 36 bytes regardless of call order.

## 1. Exact LSDA structure (decoded)

Original `Hud::draw` LSDA (`.ARM.exidx` `0x1a1e60` -> extab `0x1c57cc`, LSDA
`0x1c57dc`, header `ff ff 01 c6 03`, **77 entries / 454 B**), dock2 tail:

| # | start | len | pad | meaning |
|---|---:|---:|---:|---|
| 71 | `2044` | 4 | `2434` | `GetImage2DWidth` (halfW) |
| 72 | `2064` | 4 | `2432` | `GetImage2DHeight` (barH) |
| 73 | `20e0` | `cc` | `2444` | **merged** SetColor/DrawImage2D/DrawRegion2D/GetTextWidth/DrawString |

The faithful `THBY` (text, halfW, barH, barYOffset) reproduces entries 71/72
but splits 73 into `20e4 2a 2440` + `2130 7e 2446` (**78 / 459**, extab
`0x2629c`). `THBY + merged` gives exactly `206e 4 245a`, `208e 4 2458`,
`2108 cc 246a` (**77 / 448**, extab `0x26298` exact). Structure and content
match; only the addresses differ, because the function is 36 B longer.

## 2. The +4 spill slot (new root cause)

`THBY + merged` (9368) vs original (9332), normalised disassembly:

| func off | original | merged build |
|---|---|---|
| `0x3a` | `str r0,[sp,#80]` | `str r0,[sp,#84]` |
| `0x90` | `str r0,[sp,#120]` | `str r0,[sp,#116]` |
| `0x120` | `str r0,[sp,#120]` | `str r0,[sp,#116]` |
| `0x1a6` | `str r0,[sp,#84]` | `str r0,[sp,#88]` |
| `0x37a` | `ldrh.w sl,[r1]` | `ldrh.w r8,[r1]` |
| `0x39e` | `ldr.w r8,[r0]` | `ldr.w fp,[r0]` |
| `0x3c8` | `blx r8` | `str.w fp,[sp,#80]` |
| `0x3ca` | `mov r0,fp` | `blx fp` |
| `0x3e0` | — | `mov fp,r8` |
| `0x410` | `strd r3,sl,[sp,#20]` | `str r3,[sp,#20]` + `str.w r8,[sp,#24]` |
| `0x478` | `strd fp,r9,[sp,#116]` | `strd r9,r6,[sp,#116]` |

The extra slot holds a spilled function pointer (`fp`) in the damage region.
Instruction count actually *drops* 3226 -> 3220 while the size grows 36 B:
the RA picks wider encodings throughout. The dock2 region itself is
instruction-identical to the original, so the "damage" 481-byte region and the
frame layout are what pay for the merge.

## 3. Negative results (all measured; base = landed YBTH, 1325 / 9332)

| variant | raw | size | `.ARM.extab` |
|---|---:|---:|---:|
| base YBTH (split) | **1325** | **9332** | **0x26298** |
| THBY (split) | 1352 | 9332 | 0x2629c |
| THBY + `float filled;` before `{String,halfW,barH,barYOffset,dt,fade,fadeColor,top,y,SetColor}` (all 10 positions) | 8001 | 9368 | 0x26298 |
| THBY + `float filled = ...` before `y` | 8005 | 9368 | 0x26298 |
| THBY + inline `filled` (3 uses) | 8001 | 9368 | 0x26298 |
| THBY + inline `filled`, all 24 dock2 orders | 7997–8025 | 9368 | 0x26294–0x26298 |
| THBY + `fadeColor` decl/assign, `top` first, `fade` decl/assign, `y` decl/assign, `barOffset` decl/assign | 8001 | 9368 | 0x26298 |
| THBY + `fadeColor` ternary | 8015 | 9368 | 0x26298 |
| THBY + `filled` declared at top of `if (show)` | 8001 | 9368 | 0x26298 |
| **THBY + inline `y` (no `y` local), inline `filled`** | **2159** | **9328** | **0x26298** |
| THBY + inline `y`, `float filled;` before `SetColor` | 2159 | 9328 | 0x26298 |
| YBTH + inline `y`, inline `filled` | 2162 | 9328 | 0x26294 |
| YBTH + inline `y`, `float filled;` before `SetColor` | 2162 | 9328 | 0x26294 |
| **THBY + `int y` declared after `DrawRegion2D`** | **2129** | **9332** | 0x2629c |
| THBY + inline `top`, `y` local, inline `filled` | 8119 | 9428 | 0x26298 |
| THBY + `int y2` before `SetColor` (used in DrawString) | 8001 | 9368 | 0x26298 |

## 4. Open questions for the next wave

1. **The 4-byte epilogue.** The original restores `ldrd fp,r9,[sp,#116]` +
   `ldr.w r8,[sp,#108]` after the dock1 `String` dtor; `inline_y` restores only
   `ldr.w fp,[sp,#116]`. A source shape that keeps `r9`/`r8` live across that
   dtor while still omitting the `y` local would add the missing 4 B and align
   the tail (tail diff 595 -> ~7, dock1 524 -> ~326). Candidate: make the
   dock1 `String text` construction use a second live value across the dtor.
2. **Damage-region RA.** The +4 slot is a spill of a function pointer in the
   damage block. If the damage region's dead `ldr.w r8,[sp,#108]` reloads are
   reproduced (wave-142 open question), the spill may disappear and the merged
   build may return to 9332.
3. The faithful `THBY` order is still the correct source order; the landed
   YBTH is METRIC-ONLY (wave-143 note). Any future merged-LSDA landing should
   re-test both orders.

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9015
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w9015/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w9015/lsda.py cmake-build-match/libgof2hdaa.so 163b90
```

Scratch harness in `/dev/shm/w9015`: `fdiff.py`, `dis.py`, `lsda.py`
(LPStart/TType/call-site decoder), `norm2.py` (normalised side-by-side diff),
`firstdiff.py`, `runvar.py`, `batch.py`, `sweep24.py`, `sweep_fy.py`,
`sweep_inl.py`, `sweep_v2.py`, `sweep_ybth.py`, `sweep_final.py`,
`Hud.base.cpp` (landed wave-144 YBTH source).
