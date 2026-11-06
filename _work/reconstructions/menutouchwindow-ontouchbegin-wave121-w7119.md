# MenuTouchWindow::OnTouchBegin wave 121 (w7119): full shape re-sweep, no source win

Date: 2026-11-06. Worktree `wt/w7119`, base `bd061a489`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, `bool MenuTouchWindow::OnTouchBegin(int,
int, void*)`). Baseline and best measured this wave: **98.092643 %**,
1896 B, 734/734 normalised instructions, `byte=False`, `linked=False`,
**18** differing instruction lines (header excluded).

Gate at HEAD: `byte_exact 4505 / linked_exact 4505`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.

## TL;DR

The residual is the same 18 lines in 4 regions as waves 118/119. This wave
re-ran the complete `{ref,ptr} x {c1,top,lm,c3} x {T+(L+H), L+(T+H)}`
96-cell shape sweep, plus new first-condition, case-8, `PaintCanvas`-local,
`volatile`-pointer, whole-function-local and `-mllvm` experiments. **No
legal C++ variant beats 98.092643 %.** The absolute best family is HEAD's
(`top` cached, `lm` fresh, c3 fresh: `LLGG`/`GLGG`); the `top`-fresh family
(`LGLG`/`GGLG`) drops to 97.956403 %, and all-fresh (`GGGG`) collapses to
90.313779 % / 1892 B because clang promotes `&Globals::layout` into `fp`.

## 1. The residual (18 lines, 4 regions)

`tools/verify/verify.py --show` diff (orig | ours), function at `0x129bd4`,
1896 B both sides, 734 instructions both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

Both are the 16-bit `ADD (register) T1`; pure register choice. The same
expression in `case 3` (iPad branch) uses `ip` in both builds, and the
second `case 8` `if` uses `r2` in both. Post-RA MIR (ours): bb.71
`liveins: $r0,$r1,$r2,$r3,$r6,$r8,$r9,$r10`, `$r4 = ldr $r10, 340` —
`ip` is free but the greedy allocator picks `r4`.

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

Instruction sequence identical; 5-register permutation. The original's
`%29` (layout value) lives in caller-saved `r2` and dies at the
`buttonInsetX` load; ours lives in callee-saved `r6` and survives the call.

### Region C — `case 15`, second condition tail

```
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]      ; top (cached)
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]      ; leftMargin
adds   r1, r6, r5            |  adds   r1, r5, r4
```

### Third `if`

```
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]      ; buttonInsetX
add    r0, r5                |  add    r0, r4
```

## 2. Root cause (re-confirmed)

Post-RA MIR of HEAD, bb.42/bb.44:

```
bb.42:  %29:gpr  = t2LDRi12 %508:gpr, 0        ; Globals::layout value
        %306:rgpr= t2LDRi12 %29:gpr, 40        ; buttonInsetX  <- %29 dies here in orig
        tBL GetImage2DWidth
bb.44:  %511:gpr = t2LDRi12 %stack.0, 0        ; address rematerialised
        %319:gpr = t2LDRi12 %511:gpr, 0        ; fresh layout value
        %320:rgpr= t2LDRi12 %29:gpr, 32        ; top  <- %29 LIVE ACROSS CALL (ours)
        %323:gprnopc = t2LDRi12 %319:gpr, 12   ; leftMargin
```

The original's `%320` is `t2LDRi12 %319, 32` (fresh), i.e. `%29` is dead at
the call. Every source shape that makes `%29` dead also makes the spiller
keep `&Globals::layout` in `fp` (dropping the two `ldr r?, [sp,#16]`
rematerialisations, −4 B, 1892 B), which cascades every later branch target
(156 diff lines). Every shape that keeps the address rematerialised also
keeps exactly one of `top`/`lm` cached in `r6` (18 diffs, 1896 B). This is
a Greedy-RA rematerialisation tie-break, not a source shape difference.

## 3. Variants measured this wave

All measurements are full gold link + per-function `verify.py`
(`difflib.SequenceMatcher(autojunk=False)` over the reconciled mnemonics;
`raw` is the unrounded ratio).

### 3.1 Complete `{ref,ptr} x {c1,top,lm,c3} x {T+(L+H), L+(T+H)}` sweep (128 cells)

`L` = cached local (`layout.field` / `layout->field`), `G` = direct
(`Globals::layout->field`). The landscape is exactly:

| c1 top lm c3 | assoc | size | raw % | diffs |
|---|---|---:|---:|---:|
| **L L G G / G L G G** | **T+(L+H) or L+(T+H)** | **1896** | **98.092643** | **18** |
| L G L G / G G L G | T+(L+H) or L+(T+H) | 1896 | 97.956403 | 18 |
| L L G L / L L L G / G L G L / G L L G | either | 1892 | 90.859482 | 146–155 |
| L G L L / G G L L | either | 1892 | 90.723056 | 146 |
| L L L L / G L L L | either | 1888 | 90.573770 | 156 |
| L G G G / G G G G | either | 1892 | 90.313779 | 156 |
| L G G L / G G G L | either | 1888 | 90.102389 | 148 |

`ref` vs `ptr` local makes no difference in any cell. `T_LH` vs `L_TH`
makes no difference in any cell. HEAD is the global optimum of this space.

### 3.2 Case-8 (Region A) targeted sweep

| variant | size | raw % | diffs |
|---|---:|---:|---:|
| HEAD | 1896 | 98.092643 | 18 |
| 4th-cond re-associations `lm+t+w`, `t+lm+w`, `(lm+t)+w`, `lm+(t+w)`, `t+(lm+w)`, `(t+lm)+w`, `w+(lm+t)` | 1896 | 98.092643 | 18 |
| `w+(lm+t)` / `w+(t+lm)` (width first) | 1896 | 97.956403 | 19 |
| `Layout &layout = *Globals::layout;` local around the whole `case 8` block | 1892 | 88.3 | 338 |
| `Layout &layout` local around the first `if` only | 1896 | 98.092643 | 18 (optimised away) |
| `case 8: { ... }` extra braces | 1896 | 98.092643 | 18 |
| `upButtonPressed = downButtonPressed = 0;` chained | 1896 | 98.092643 | 18 |
| `int entryW = this->listEntryWidth;` (prior wave) | 1896 | — | 20 |
| `{ } + entryW` (prior wave) | 1896 | — | 34 |

Region A never moves: `%214` always gets `r4`, never `ip`.

### 3.3 Case-15 first-condition / spelling sweep

| variant | size | raw % | diffs |
|---|---:|---:|---:|
| HEAD | 1896 | 98.092643 | 18 |
| `int imageId = this->heapBufB[this->field_0x1e0];` | 1896 | 98.092643 | 18 |
| `int right = Globals::w - layout.buttonInsetX;` | 1896 | 98.092643 | 18 |
| `int right = Globals::w - Globals::layout->buttonInsetX;` | 1896 | 98.092643 | 18 |
| `int w = Globals::w;` | 1896 | 98.092643 | 18 |
| `(Globals::w - lm) - W()` explicit grouping | 1896 | 98.092643 | 18 |
| `W() < x` reversed compare | 1896 | 98.092643 | 18 |
| `Globals::w - (lm + W())` | 1896 | 97.138965 | 39 |
| `Globals::w - W() - lm` (width before inset) | 1892 | 89.6 | 162 |
| `lm + (top + H)` (top cached, lm fresh) | 1896 | 98.092643 | 18 |
| `(*Globals::layout).field` deref spelling | 1892 | 90.313779 | 156 |
| `PaintCanvas *canvas = Globals::Canvas;` local | 1892 | 89.8 | 154 |
| `PaintCanvas &canvas = *Globals::Canvas;` ref | 1892 | 89.8 | 154 |
| `(*(decltype(Globals::layout) volatile *)&Globals::layout)->...` (both fields) | 1896 | 95.8 | 120 |
| same, `top` only | 1892 | 89.8 | 159 |
| `this->scrollbarHit = cond1 && cond2;` | 1892 | 89.4 | 160 |
| `bool hit = cond1 && cond2; scrollbarHit = hit;` | 1892 | 89.4 | 160 |
| `Layout *lp = Globals::layout;` at function top, all `lp->` | 1900 | 44.8 | 694 |
| `Layout &layout = *Globals::layout;` in `case 1/2`, all `layout.` | 1896 | 98.092643 | 18 (optimised away) |

### 3.4 `-mllvm` knobs (manual TU compile, not usable in-tree)

`spp-rematerialization-threshold=0,1,2,4,8,16,32,64,128,1000`,
`split-spill-mode=default`, `enable-deferred-spilling=0/1`,
`regalloc-csr-first-time-cost=0/100`, `disable-spill-hoist=1`,
`sink-insts-to-avoid-spills=0`: **byte-identical object** every time.
(`-split-spill-mode=aggressive`, `-consider-local-interval-cost`,
`-split-large-intervals` are not accepted by this clang 7.0.2 build.)

## 4. Verdict

HEAD is a hard local optimum. The residual is a Greedy-RA
live-range/rematerialisation tie-break for the `Globals::layout` **value**
across the first `GetImage2DWidth` call, plus an independent `ip` vs `r4`
tie-break for `this->listEntryWidth` in `case 8`. Making `%29` dead at the
call (the original's IR) forces `&Globals::layout` into `fp` and costs 4 B;
keeping the address rematerialised forces one of `top`/`lm` to stay cached
in `r6`. No legal C++ shape separates the two decisions.

Cumulative across waves 118/119/121 (>450 shapes): best remains
**98.092643 %**, 1896 B, 18 diffs. The next lever would have to perturb
global liveness from *outside* case 15 while keeping the 700+ already
matching instructions.

## 5. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w7119
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Post-RA MIR: compile the TU from `cmake-build-match/compile_commands.json`
with `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN15MenuTouchWindow12OnTouchBeginEiiPv`; the section
`# *** IR Dump After Greedy Register Allocator ***` shows
`%320:rgpr = t2LDRi12 %29:gpr, 32` in bb.44 (cached `top`).
