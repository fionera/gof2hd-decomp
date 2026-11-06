# MenuTouchWindow::OnTouchBegin wave 122 (w7219): oracle-shape re-sweep, no source win

Date: 2026-11-06. Worktree `wt/w7219`, base `29cd275f6`. Original binary:
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

Wave 121 concluded HEAD is a hard local optimum of the
`{ref,ptr} x {c1,top,lm,c3} x {T+(L+H), L+(T+H)}` space. This wave tested a
new hypothesis taken from the Unity-remake oracle
(`_work/oracles/.../native/game/MenuTouchWindow.c`, case 0xf): the original
source almost certainly has a **function-top `Layout` local** used by the
`choiceWindowOpen` check, case 1/2 and case 8's first `if`, while **case 15
uses `Globals::layout` directly** (the oracle decompiles case 15 as
`*(int *)(Globals::layout + 0x28)`, `*(int *)(Globals::layout + 0xc)`,
`*(int *)(Globals::layout + 0x20)`). Reproducing that exact shape gives
**1892 B / 165 diffs** (all of them branch-address shifts plus the same
case-8 `ip`/`r4` and case-15 register residual); the 96-cell variant of it
still bottoms out at **1896 B / 18 diffs**. No legal C++ shape beats
98.092643 %.

## 1. The oracle shape (new this wave)

The oracle at `MenuTouchWindow.c:4341` is:

```c
pLVar13 = Globals::layout;                       // function entry
...
if (*(char *)(param_1 + 0x170)) { ... }          // messageShowing
if (*Globals::layout != (Layout)0x0) { ... }     // choiceWindowOpen
switch (*(undefined4 *)(param_1 + 0x16c)) {
case 1: case 2:
    iVar12 = *(int *)(pLVar13 + 0xc);            // uses the local
    ...
case 8:
    iVar7 = *(int *)(pLVar13 + 0x28);            // first if uses the local
    ...
    iVar7 = *(int *)(Globals::layout + 0x28);    // second if uses the global
    pLVar13 = Globals::layout;                   // re-binds the local
case 0xf:
    iVar12 = Globals::w - *(int *)(Globals::layout + 0x28);
    iVar7  = GetImage2DWidth(Globals::Canvas, ...);
    if ((iVar12 - iVar7 < param_2) &&
        (iVar14 = *(int *)(Globals::layout + 0xc),    // leftMargin
         iVar12 = *(int *)(Globals::layout + 0x20),   // top
         iVar7  = GetImage2DHeight(Globals::Canvas, ...),
         (int)param_3 < iVar7 + iVar12 + iVar14)) {   // height + top + lm
```

The disassembly confirms the function-top local: `r6 = Globals::layout` is
loaded once at `0x129bfe` (right after the `messageShowing` check) and is
still live at case 1/2 (`0x129c5c`). Case 8's first `if` uses that `r6`
(`ldr r0,[r6,#40]` at `0x129fb6`); after the sound call the value is
re-loaded (`ldr r0,[sp,#16]; ldr r6,[r0,#0]` at `0x129ffa`). Case 15 does
**not** use `r6`: it re-loads `&Globals::layout` from `[sp,#16]`
(`0x12a1bc`, `0x12a212`, `0x12a23c`) and re-loads the value at each use.

Reproducing this source shape (function-top `Layout &layout`, used by the
`choiceWindowOpen` check, case 1/2 and case 8's first `if`; all-direct in
case 15) gives **1892 B / 165 diffs**. The diff is *only*:

* region A (`case 8`, 4th `y` condition): `ip` vs `r4` — unchanged;
* case 15: the address is kept in `fp` (`ldr.w r2,[fp]`) instead of being
  re-loaded from `[sp,#16]` (`ldr r2,[sp,#16]; ldr r2,[r2,#0]`), which is
  exactly one instruction (4 B) and shifts every branch target.

Everything else — the function-top `r6` local, case 1/2, case 8's two
`if`s, the case-15 first condition, the `scrollbarHit` store shape — is
byte-identical.

## 2. Full 96-cell re-sweep of the oracle shape

`{c1,top,lm,c3} x {TLH, LTH, HTL, HLT, TL_H, LH_T}` with the function-top
local + case-8 local (96 builds). The landscape is the same as wave 121:

| c1 top lm c3 | order | size | diffs |
|---|---|---:|---:|
| **LLGG / LGLG / GLGG / GGLG** | **TL_H or LH_T** | **1896** | **18** |
| LLGG / LGLG / GLGG / GGLG | TLH / LTH | 1896 | 23 |
| LLGG / LGLG / GLGG / GGLG | HTL / HLT | 1896 | 54 |
| LGGG / GGGG | any | 1892 | 164–165 |
| LLGL / LGLL / GLGL / GGLL | any | 1892 | 154–159 |
| LLLG / GLLG | any | 1892 | 163–164 |
| LLLL / GLLL | any | 1888 | 165–166 |
| LGGL / GGGL | any | 1888 | 157–162 |

So the function-top local does not perturb the case-15 RA decision; HEAD's
`LLGG`/`TL_H` remains the global optimum.

## 3. Other shapes measured this wave

All measurements are full gold link + `verify.py --show`; size is the
linked function size, diffs are structural `|` lines.

| variant | size | diffs |
|---|---:|---:|
| HEAD (`Layout &layout` in case 15, `T+(L+H)`) | 1896 | 18 |
| oracle shape, all-direct case 15 (`LTH`/`TLH`) | 1892 | 164 |
| oracle shape, `HTL`/`HLT` | 1892 | 165 |
| `H + (T+L)` / `H + (L+T)` with top/lm local | 1896 | 54 |
| `(T+L) + H` / `(L+T) + H` with top local, lm direct | 1896 | 23 |
| `int right = Globals::w - Globals::layout->buttonInsetX;` + all-direct | 1892 | 164 |
| `int inset = Globals::layout->buttonInsetX;` + all-direct | 1892 | 164 |
| `PaintCanvas &canvas = *Globals::Canvas;` local (oracle shape) | 1892 | 158 |
| nested `if` with a second `Layout &layout2` | 1896 | 163 |
| nested `if`, all-direct, `H+(T+L)` | 1896 | 112 |
| comma-operator second local (`lm=…,top=…,h=…`) | 1892 | 164 |
| static-inline helper taking `Layout *` / `Globals::layout` | 1892 | 164 |
| `bool hit; if (c1) { if (c2) hit=1; else hit=0; } else hit=0;` | 1896 | 163 |
| `scrollbarHit = c1 && c2;` / `bool hit = c1 && c2;` | 1896 | 163 |
| `(*(Layout *volatile *)&Globals::layout)->field` (both fields) | 1896 | 128 |
| `Layout *lp = *(Layout *volatile *)&Globals::layout;` + `lp->` | 1888 | 171 |
| `Layout *volatile lp; (lp = Globals::layout, …)` | 1940 | 768 |
| function-top `Layout *layout` used everywhere | 1876 | 734 |
| function-top `Layout &layout` used everywhere | 1876 | 734 |

The `volatile` variant reaches 1896 B but loads the layout value twice
(`ldr.w r0,[fp]; ldr r2,[r0,#12]` and `ldr.w r0,[fp]; ldr r6,[r0,#32]`),
so it is 128 diffs, not 18.

## 4. Root cause (re-confirmed with MIR)

Post-RA MIR of the oracle shape, case 15:

```
bb.37:  %508:gpr = t2LDRi12 %stack.0, 0      ; &Globals::layout -> r11 (fp)
bb.42:  %r2 = t2LDRi12 %r11, 0               ; value
        %r2 = t2LDRi12 %r2, 40               ; buttonInsetX
        tBL GetImage2DWidth
bb.44:  %r2 = t2LDRi12 %r11, 0               ; value again, address still in fp
        %r4 = t2LDRi12 %r2, 12               ; leftMargin
        %r6 = t2LDRi12 %r2, 32               ; top
        tBL GetImage2DHeight
```

The original instead has `ldr r4,[sp,#16]` at `0x12a1bc` and a **second**
`ldr r2,[sp,#16]` at `0x12a212`; the address is never kept across the
call. `%508` is a spilled stack reload, so the Greedy spiller keeps it in
the callee-saved `r11` across the `GetImage2DWidth` call (one reload
instead of two). In the original the same value is re-materialised from
`[sp,#16]` at each use, which frees `r11` for `Globals::Canvas`
(`0x12a1ec: ldr.w fp,[r1]`). The original's choice is 4 B *worse* in the
naive count but is what the binary does; ours is the better RA decision.

The `-mllvm` knobs re-checked this wave
(`-split-spill-mode=default`, `-enable-deferred-spilling=0`,
`-regalloc=greedy`, `-disable-spill-hoist`) all produce the same object;
`-split-spill-mode=aggressive`, `-consider-local-interval-cost` and
`-split-large-intervals` are rejected by clang 7.0.2.

## 5. Verdict

No source win. HEAD remains the global optimum at **98.092643 %**, 1896 B,
18 diffs. The residual is a Greedy-RA live-range-split / rematerialisation
tie-break for `&Globals::layout` across the first `GetImage2DWidth` call
(keep in `fp` vs re-materialise from `[sp,#16]`), plus the independent
case-8 `ip` vs `r4` tie-break for `this->listEntryWidth`. The oracle-shape
reconstruction confirms the source-level shape (function-top local + direct
case 15) but does not change the RA decision.

## 6. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w7219
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
`renamable $r11 = t2LDRi12 %stack.0, 0` in bb.37 and `%r2 = t2LDRi12
%r11, 0` in both bb.42 and bb.44.
