# MenuTouchWindow::OnTouchBegin wave 123 (w7316): MIR-driven RA re-sweep, no source win

Date: 2026-11-06. Worktree `wt/w7316`, base `1c8c9b20e`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, `bool MenuTouchWindow::OnTouchBegin(int,
int, void*)`). Baseline and best measured this wave: **98.092643 %**,
1896 B, `byte=False`, `linked=False`, **18** differing instruction lines
(header excluded).

Gate at HEAD: `byte_exact 4506 / linked_exact 4506`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.

## TL;DR

The residual is the same 18 lines in 4 regions as waves 118/119/121/122.
This wave re-derived the exact post-RA MIR and tested a new family of
shapes aimed at the single RA decision that controls the whole residual:
whether `&Globals::layout` is rematerialised from `[sp,#16]` or kept in
`fp` across the first `GetImage2DWidth` call. Every shape that makes the
cached layout **value** (`%29`) dead at the call also makes Greedy RA keep
`&Globals::layout` in `fp` and shortens the function by 4 B (1892 B,
164 diffs). Every shape that keeps `%29` live across the call keeps the
address rematerialised and the function at 1896 B, but leaves exactly one
of `top`/`lm` cached (18 diffs). No legal C++ shape separates the two
decisions. HEAD remains the global optimum.

## 1. The residual (unchanged)

`tools/verify/verify.py --show` diff (orig | ours), function at `0x129bd4`,
1896 B both sides, 734 instructions both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

### Region B — `case 15`, scrollbar `if`, first condition

```
ldr.w  r3, [sl, #312]        |  ldr.w  r2, [sl, #312]     ; heapBufB
ldr.w  r6, [sl, #480]        |  ldr.w  r3, [sl, #480]     ; field_0x1e0
ldr    r2, [r4, #0]          |  ldr    r6, [r4, #0]       ; Globals::layout
ldr.w  r1, [r3, r6, lsl #2]  |  ldr.w  r1, [r2, r3, lsl #2]
ldr    r5, [r0, #0]          |  ldr    r4, [r0, #0]       ; Globals::w
ldr    r2, [r2, #40]         |  ldr    r5, [r6, #40]      ; buttonInsetX
subs   r5, r5, r2            |  subs   r4, r4, r5
subs   r0, r5, r0            |  subs   r0, r4, r0
```

### Region C — `case 15`, second condition tail

```
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]      ; top (cached)
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]      ; leftMargin
adds   r1, r6, r5            |  adds   r1, r5, r4
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]      ; third if: buttonInsetX
add    r0, r5                |  add    r0, r4
```

## 2. Exact post-RA MIR evidence (HEAD)

Compiled the TU with
`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
and inspected `# *** IR Dump After Greedy Register Allocator ***`.

HEAD, `case 15`:

```
bb.104:
  %r4 = tLDRspi $sp, 4          ; &Globals::layout (rematerialisable)
bb.107 (c1):
  %r6 = tLDRi %r4, 0            ; Globals::layout value  (%29)
  %r5 = tLDRi %r6, 40           ; buttonInsetX
  tBL GetImage2DWidth
bb.108 (c2):
  %r2 = tLDRspi $sp, 4          ; &Globals::layout reloaded
  %r4 = tLDRi %r6, 32           ; top from %29  <-- value live across call
  %r2 = tLDRi %r2, 0            ; fresh Globals::layout value
  %r5 = tLDRi %r2, 12           ; leftMargin from fresh value
  tBL GetImage2DHeight
bb.111 (c3):
  %r2 = tLDRspi $sp, 4          ; &Globals::layout reloaded
  %r4 = tLDRi %r2, 40           ; buttonInsetX
```

The original instead has `%29` dead at the first call: it reloads
`&Globals::layout` at `0x12a212` and reads **both** `leftMargin`
(`[r2,#12]`) and `top` (`[r2,#32]`) from that one fresh value. That is the
`top`-fresh family (`L G L G` in wave-121 order). Compiling it gives
1896 B / 18 diffs but merely swaps the cached field (`lm` becomes the
cached one, `top` fresh). Making **both** fields fresh (`L G G G`) gives
1892 B / 164 diffs: the RA then keeps `&Globals::layout` in `fp` instead
of rematerialising it from `[sp,#16]`.

## 3. Variants measured this wave

All measurements are full gold link + per-function `verify.py --show`.
Every variant reverted.

| variant | size | diffs |
|---|---:|---:|
| HEAD (`Layout &layout`, top cached) | 1896 | **18** |
| `lm + (top + H)` (lfirst) | 1896 | 18 |
| `top + lm + H` / `lm + top + H` (flat) | 1896 | 23 |
| third `if` operand swapped (`W + inset`) | 1896 | 22 |
| third `if` via `int inset = Globals::layout->buttonInsetX` | 1896 | 18 |
| `Globals::w + (-layout.buttonInsetX)` | 1896 | 18 |
| top fresh, lm cached (`L G L G`) | 1896 | 18 |
| both top/lm fresh (`L G G G`) | 1892 | 164 |
| hoisted `int right` + fresh `lm`/`top` locals | 1892 | 78 |
| hoisted `int right` + `Layout *layout` after call | 1892 | 172 |
| hoisted `int right` + pointer local for c2+c3 | 1896 | 114 |
| `Layout *layout = Globals::layout` before c1, direct c2/c3 | 1892 | 164 |
| same, pointer reused for c2 | 1892 | 163 |
| `Layout **slot = &Globals::layout` + `(*slot)->` | 1892 | 164 |
| `Layout *&slot` / `Layout *const &slot` | 1892 | 164 |
| `Layout *volatile &slot` | 1896 | 135 |
| `*(Layout *volatile *)&Globals::layout` for c2 fields | 1896 | 126 |
| `Layout *volatile *p = &Globals::layout` | 1896 | 135 |
| `Layout **volatile p = &Globals::layout` | 1932 | 762 |
| `Layout *volatile layout` local | 1928 | 761 |
| `volatile Layout *layout` local | 1888 | 174 |
| `PaintCanvas *canvas` local before c1 (all variants) | 1892 | 157–212 |
| `PaintCanvas *canvas` at top of `case 15` | 1892 | 212 |
| `Layout &layout` at top of `case 15` (before loops) | 1920 | 684 |
| `int top = layout.field_0x20_top` hoisted before `if` | 1892 | 78 |
| `int top` + `int lm` hoisted before `if` | 1888 | 96 |
| `bool xok` / `bool hit` split | 1892 | 167–172 |
| comma-operator assignment of `Layout *` in c2 | 1892 | 164 |
| `(unsigned)` / `(long)` cast on `listEntryWidth` | 1896 | 18–19 |
| `*(volatile int *)&this->listEntryWidth` | 1896 | 18 |
| `int entryW = this->listEntryWidth` before case-8 `if` | 1896 | 20 |
| `int top = Globals::layout->field_0x20_top` before case-8 `if` | 1896 | 20 |
| `(void) this->listEntryWidth;` no-op before case-8 `if` | 1896 | 18 |

### 3.1 Case-8 association sweep

All 12 parenthesizations/permutations of
`leftMargin + field_0x20_top + listEntryWidth` were built. **Every one
allocates `r4`** for the `listEntryWidth` load; none produces `ip`.
Region A is therefore independent of the expression tree.

## 4. Root cause (re-confirmed)

The residual is a single Greedy-RA live-range/rematerialisation
tie-break for the `Globals::layout` **value** (`%29`) across the first
`GetImage2DWidth` call, plus the independent case-8 `ip` vs `r4`
tie-break for `this->listEntryWidth`.

* Keep `%29` live across the call (HEAD): the address
  `&Globals::layout` stays rematerialised from `[sp,#16]`, the function
  stays 1896 B, and exactly one of `top`/`lm` is read from the cached
  `%29` (18 diffs).
* Make `%29` dead at the call: the RA keeps `&Globals::layout` in `fp`
  (`ldr.w r2,[fp]` instead of `ldr r2,[sp,#16]; ldr r2,[r2,#0]`),
  shortening the function by 4 B (1892 B) and shifting every branch
  target (164+ diffs).

No legal C++ shape found in waves 118–123 (>500 shapes) separates the two
decisions. The original source almost certainly uses direct
`Globals::layout` accesses in `case 15` (the oracle shape), but clang
7.0.2's Greedy RA makes the opposite remat choice on our TU. This is a
compiler tie-break, not a source-shape difference.

## 5. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w7316
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Post-RA MIR: compile the TU from `cmake-build-match/compile_commands.json`
with `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN15MenuTouchWindow12OnTouchBeginEiiPv`; the
`# *** IR Dump After Greedy Register Allocator ***` section shows
`%r6 = tLDRi %r4, 0` in bb.107 and `%r4 = tLDRi %r6, 32` in bb.108
(cached `top`), while `%r2 = tLDRspi $sp, 4` is rematerialised in bb.108
and bb.111.
