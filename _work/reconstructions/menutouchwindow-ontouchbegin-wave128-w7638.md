# MenuTouchWindow::OnTouchBegin wave 128 (w7638): oracle-exact case-8 hit-test locals

Date: 2026-11-07. Worktree `wt/w7638`, branch `wave128-w7638`, base
`4bbe88776` + cherry-picked wave-127 `2f586feaf`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `Android (4751641 based on r328903)`, `-Oz`,
image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`). Baseline and best measured this wave:
**98.092643 %**, 1896 B, `byte=False`, `linked=False`, **18** differing
instruction lines (header excluded).

Gate at HEAD: `byte_exact 4506 / linked_exact 4506`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.

## TL;DR

Landed one **byte-identical source-faithfulness** change: the case-8 button
hit-test now matches the Unity-remake oracle exactly — `listTopY` and
`buttonInsetX` are cached in locals, the four comparisons use the oracle's
operand order, and both locals are re-loaded after the sound call. The
emitted code is bit-for-bit identical to wave-127 (same 18-line residual),
confirmed with `verify.py --show`.

The residual is still the same 18 lines in 4 regions. This wave additionally
reconstructed the oracle's **case-15** shape (all-direct `Globals::layout`
accesses, fresh value after the first `GetImage2DWidth` call) and measured
~30 more variants. The all-direct shape is the true source shape but the
Greedy RA then keeps `&Globals::layout` in `fp` and the function shrinks to
1892 B / 165 diffs. Every shape that keeps the value live across the call
stays at 1896 B / 18 diffs. The two RA decisions remain coupled.

## 1. The landed change (case 8)

Oracle (`Reference/decompiled/native/game/MenuTouchWindow.c:4554`):

```c
case 8:
  *(undefined2 *)(param_1 + 0x108) = 0;
  iVar12 = *(int *)(param_1 + 0x19c);       // listTopY
  iVar7  = *(int *)(pLVar13 + 0x28);        // buttonInsetX
  if ((((iVar7 + iVar12 < param_2) && (param_2 < iVar7 + iVar12 + *(int *)(param_1 + 0x158))) &&
      (*(int *)(pLVar13 + 0xc) + iVar7 < (int)param_3)) &&
     ((int)param_3 < *(int *)(pLVar13 + 0xc) + *(int *)(pLVar13 + 0x20) + *(int *)(param_1 + 0x154))) {
    *(undefined1 *)(param_1 + 0x108) = 1;
    in_s0 = (float)FModSound::play(...);
    iVar12 = *(int *)(param_1 + 0x19c);
    iVar7  = *(int *)(Globals::layout + 0x28);
    pLVar13 = Globals::layout;
  }
  if (((*(int *)(param_1 + 0x158) + iVar7 + iVar12 < param_2) &&
      (param_2 < (iVar12 - iVar7) + *(int *)(param_1 + 0x1a0))) && ...)
```

`src/game/ui/MenuTouchWindow.cpp` now reproduces that shape verbatim
(`case 8: { ... }`, `int listTopY = this->listTopY; int buttonInsetX =
layout->buttonInsetX;`, oracle operand order, re-load both locals after the
sound call, re-bind `layout = Globals::layout;`). The original disassembly
proves the locals are real: `r1 = listTopY` and `r0 = buttonInsetX` are held
across the whole first `if` (`0x129fb2`/`0x129fb6`) and re-loaded after the
sound call (`0x129ffc`/`0x12a002`).

The change is **bit-for-bit identical** to wave-127: same 1896 B, same 18
diff lines (case-8 `ip`/`r4`, case-15 regions B/C/D).

## 2. Case-15 oracle reconstruction (no source win)

Oracle (`MenuTouchWindow.c:4635`):

```c
case 0xf: goto LAB_00150d0c;
case 0x10: ...
LAB_00150d0c:
  ScrollTouchWindow::OnTouchBegin(...);
  /* scrollEntries 52/60 loop, scrollSlots loop */
  iVar12 = Globals::w - *(int *)(Globals::layout + 0x28);
  iVar7  = GetImage2DWidth(Globals::Canvas, heapBufB[field_0x1e0]);
  if ((iVar12 - iVar7 < param_2) &&
      (iVar14 = *(int *)(Globals::layout + 0xc),
       iVar12 = *(int *)(Globals::layout + 0x20),
       iVar7  = GetImage2DHeight(Globals::Canvas, heapBufB[field_0x1e0]),
       (int)param_3 < iVar7 + iVar12 + iVar14))
    uVar1 = 1;
  else
    uVar1 = 0;
  *(undefined1 *)(param_1 + 0x1d9) = uVar1;
  iVar12 = *(int *)(Globals::layout + 0x28);
  iVar7  = GetImage2DWidth(Globals::Canvas, **(uint **)(param_1 + 0x134));
  if (param_2 < iVar7 + iVar12) { ... }
```

The original disassembly confirms all-direct accesses: `&Globals::layout` is
spilled once to `[sp,#16]` at `0x129c14`, then re-loaded at `0x12a1bc`,
`0x12a212`, `0x12a23c`; the value is re-loaded after each call
(`0x12a218`, `0x12a240`). The value `%29` loaded before the first
`GetImage2DWidth` call dies at that call (`0x12a1fc`).

Reproducing this shape in C++ gives **1892 B / 165 diffs**: the Greedy RA
then keeps `%98` (`&Globals::layout`) in `fp` across both calls instead of
rematerialising it from `[sp,#16]`, and `Globals::Canvas` moves from `fp` to
`r5`. The function is 4 B shorter and every branch target shifts.

Post-RA MIR of the all-direct shape (`# *** IR Dump After Greedy Register
Allocator ***` / `After Virtual Register Rewriter`):

```
bb.42: renamable $r2  = t2LDRi12 renamable $r11, 0   ; value from fp
       renamable $r2  = t2LDRi12 killed $r2, 40      ; buttonInsetX
       tBL GetImage2DWidth
bb.44: renamable $r2  = t2LDRi12 renamable $r11, 0   ; value again (fp)
       renamable $r4  = t2LDRi12 $r2, 12             ; leftMargin
       renamable $r6  = t2LDRi12 killed $r2, 32      ; top
       tBL GetImage2DHeight
bb.47: renamable $r2  = t2LDRi12 killed $r11, 0      ; value again (fp)
```

`%98` is live across both calls in `r11` (fp). The original instead has
`ldr r4,[sp,#16]` / `ldr r2,[sp,#16]` / `ldr r0,[sp,#16]` (three 16-bit
rematerialisations) and `fp = Globals::Canvas`.

## 3. Variants measured this wave

All measurements are full gold link + `verify.py --show`; size is the linked
function size, diffs are structural `|` lines (header excluded).

| variant | size | diffs |
|---|---:|---:|
| wave-127 HEAD (`Layout &layout` in case 15) | 1896 | **18** |
| oracle-exact case-8 locals (landed) | 1896 | **18** |
| case-15 all-direct | 1892 | 165 |
| all-direct + `PaintCanvas *canvas` local | 1892 | 158 |
| all-direct + `PaintCanvas &canvas` reference | 1892 | 158 |
| all-direct + `right`/`width` locals (oracle) | 1892 | 165 |
| all-direct + `right`/`width`/`top`/`lm` locals | 1892 | 173 |
| all-direct + `height` local | 1904 | 389 |
| all-direct + all locals + `hit` local | 1900 | 389 |
| `lp` pointer for c1, direct c2/c3 | 1892 | 165 |
| direct c1, `lp` pointer for c3 | 1892 | 165 |
| `lp` pointer re-bound after first call | 1896 | 18 |
| `lp` pointer for all + re-bind | 1896 | 18 |
| `inset = layout.buttonInsetX` cached, direct rest | 1892 | 165 |
| `top`/`lm` locals + direct c1/c3 | 1888 | 97 |
| `canvas` hoisted before scrollSlots loop | 1888 | 174 |
| `canvas` reference hoisted before loop | 1888 | 174 |
| `right` hoisted before scrollSlots loop | 1892 | 193 |
| function-top `PaintCanvas *canvas` | 1896 | 730 |
| nested `if (c1) { Layout *lp = Globals::layout; if (c2) ... }` | 1896 | 163 |
| case reorder: ascending 1,2,3,4,6,13,7,8,9,11,12,14,16,15,17 | 1896 | 34 |
| case reorder: binary block order | 1896 | 34 |
| case reorder: 7 other permutations | 1896 | 18 |

`-mllvm` knobs re-checked on the all-direct shape:
`-enable-deferred-spilling=1`, `-disable-spill-hoist=1`,
`-sink-insts-to-avoid-spills=1`, `-regalloc-csr-first-time-cost=100/1000`,
`-spp-rematerialization-threshold=1/100`, `-split-spill-mode=size|speed`,
`-regalloc=pbqp|basic` — every one leaves the function at 1892 B (pbqp/basic
are much larger).

## 4. Root cause (re-confirmed)

The residual is one Greedy-RA live-range/rematerialisation tie-break for
`&Globals::layout` (`%98`) across the first `GetImage2DWidth` call, coupled
to the liveness of the cached layout value (`%29`):

* Keep `%29` live across the call (wave-127 HEAD): `%98` is rematerialised
  from `[sp,#16]`, `fp = Globals::Canvas`, function stays 1896 B, but `top`
  is read from the cached `%29` (18 diffs).
* Make `%29` dead at the call (the original's IR): `%98` is kept in `fp`,
  `Globals::Canvas` moves to `r5`, function shrinks to 1892 B, every branch
  target shifts (165 diffs).

The original source is the all-direct shape (proved by the fresh value
loads after both calls). clang 7.0.2's Greedy RA makes the opposite remat
choice on our TU. >630 cumulative shapes across waves 118-128 do not
separate the two decisions.

Region A (`case 8`, `ip` vs `r4` for `listEntryWidth`) is an independent
short-lived-value tie-break; the oracle-exact locals do not change it.

## 5. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w7638
TMPDIR=/dev/shm/w7638/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w7638/tmp cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Post-RA MIR: compile the TU from `cmake-build-match/compile_commands.json`
with `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN15MenuTouchWindow12OnTouchBeginEiiPv`; the
`# *** IR Dump After Greedy Register Allocator ***` and `After Virtual
Register Rewriter` sections show `%98` in `$r11` in bb.42/44/47 for the
all-direct shape.

Scratch (not tracked): `/dev/shm/w7638/` (`base.cpp`, `pre127.cpp`,
`sweep.py`, `reorder.py`, `measure.sh`, `show*.txt`, `mir_all.txt`).
