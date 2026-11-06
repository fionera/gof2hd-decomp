# MenuTouchWindow::OnTouchBegin wave 119 (w6945): RA tie-break sweep, no source win

Date: 2026-11-06. Worktree `wt/w6945`, branch `wave119-w6945`, base
`7a031742d`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, `bool MenuTouchWindow::OnTouchBegin(int,
int, void*)`). Baseline and best measured: **98.092643 %**, 1896 B,
734/734 normalised instructions, `byte=False`, `linked=False`, **18**
differing instruction lines (header excluded).

Gate at commit: `byte_exact 4505 / linked_exact 4505`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27
sections, `.so` exactly 2,187,328 B. No section/extab change.

## TL;DR

Same 18 residual lines in 3 regions as waves 118 / 119-w6716 / w6746 /
w6908. This wave re-derived the residual from the post-RA MIR and ran a new
~70-variant sweep (const/volatile references, pointer re-binding, hoisted
`int right`, alternate `+` associations and operand orders, deref/array
spellings, and case-8 expression re-associations). **No legal C++ variant
beats 98.092643 %.** The new, sharper result is that the original's IR is
*exactly* HEAD's IR with the `Globals::layout` **value** (`%29`) spilled /
rematerialised across the first `GetImage2DWidth` call instead of kept in a
callee-saved register; every shape that makes `%29` dead across the call
also makes the spiller keep `&Globals::layout` in `fp` and drop the two
address rematerialisations, costing 4 B (1892 B) and cascading every branch
target.

## 1. The residual (unchanged)

`tools/verify/verify.py --show` diff (orig | ours), function at `0x129bd4`,
1896 B both sides, 734 instructions both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

Both encodings are the 16-bit `ADD (register) T1` (`0x4463` / `0x4423`), so
this is a pure register choice, not an instruction-shape difference. The
post-RA MIR operand is `%214:rgpr` (`rGPR` = r0-r12+lr, so `ip` is legal).
The identical expression in the *second* `case 8` `if` uses `r2` in both
builds, and the same expression in `case 3` uses `ip` in both builds.

### Region B — `case 15`, scrollbar `if`, first condition

```
ldr.w  r3, [sl, #312]        |  ldr.w  r2, [sl, #312]     ; this->heapBufB
ldr.w  r6, [sl, #480]        |  ldr.w  r3, [sl, #480]     ; this->field_0x1e0
ldr    r2, [r4, #0]          |  ldr    r6, [r4, #0]       ; Globals::layout
ldr.w  r1, [r3, r6, lsl #2]  |  ldr.w  r1, [r2, r3, lsl #2] ; heapBufB[idx]
ldr    r5, [r0, #0]          |  ldr    r4, [r0, #0]       ; Globals::w
ldr    r2, [r2, #40]         |  ldr    r5, [r6, #40]      ; layout->buttonInsetX
subs   r5, r5, r2            |  subs   r4, r4, r5
subs   r0, r5, r0            |  subs   r0, r4, r0
```

Instruction sequence identical; a 5-register permutation. The original's
`%29` (layout value) lives in caller-saved `r2` and dies at the
`buttonInsetX` load; ours lives in callee-saved `r6` and survives the call.

### Region C — `case 15`, scrollbar `if`, second condition tail

```
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]      ; layout->field_0x20_top (cached)
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]      ; layout->field_0xc_leftMargin
adds   r1, r6, r5            |  adds   r1, r5, r4
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]      ; third if: layout->buttonInsetX
add    r0, r5                |  add    r0, r4
```

## 2. HEAD source shape

```cpp
{
    Layout &layout = *Globals::layout;
    if (x > Globals::w - layout.buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
        y < layout.field_0x20_top + (Globals::layout->field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])))
        this->scrollbarHit = 1;
    else
        this->scrollbarHit = 0;
    if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
        this->dragLastX = y;
        this->dragStartX = y;
        this->dragVelocity = 0;
        this->dragging = 1;
    }
}
```

## 3. Binary facts (re-confirmed with objdump + MIR)

- Literal pool: `0x129f54` → GOT `Globals::layout`; `0x12a318` → GOT
  `Globals::w`; `0x12a31c` → GOT `Globals::Canvas`.
- Prologue `push {r4,r5,r6,r7,lr}` + `stmdb sp!,{r3,r4,r5,r6,r7,r8,r9,sl,fp}`;
  `sl = this`, `r8 = x`, `r9 = y`.
- `&Globals::layout` is computed once at `0x129bf8..129bfc` and spilled to
  `[sp,#16]` at `0x129c14`. Every later use reloads it.
- `0x12a1ba: movs r5,#0; 0x12a1bc: ldr r4,[sp,#16]` — the address is
  hoisted into the `scrollSlots` loop preheader and held in `r4` across the
  loop's `TouchButton::OnTouchBegin` calls until `0x12a1f0`.
- `fp = &Globals::Canvas` (`ldr.w fp,[r1]` at `0x12a1ec`), live across all
  three calls; the Canvas *value* is reloaded `ldr.w r0,[fp]` each time.
- Region C (`0x12a20a..12a228`) loads `&Globals::layout` **fresh**
  (`ldr r2,[sp,#16]; ldr r2,[r2,#0]`) and reads both `leftMargin`
  (`[r2,#12]`) and `top` (`[r2,#32]`) from that one fresh value, then
  `adds r1,r6,r5` (top+lm), `add r0,r1` (+H).
- Third `if` (`0x12a23c..12a24c`) reloads `&Globals::layout` fresh again.
- The only values live across the first call in the original are `r5`
  (`w - buttonInsetX`), `fp` (`&Globals::Canvas`), `r8`, `r9`, `sl`.

### Post-RA MIR of HEAD (the exact residual mechanism)

```
bb.42:  %29:gpr  = t2LDRi12 %508:gpr, 0        ; Globals::layout value
        %306:rgpr= t2LDRi12 %29:gpr, 40        ; buttonInsetX   <- %29 dies here in orig
        ...
        tBL GetImage2DWidth
bb.44:  %511:gpr = t2LDRi12 %stack.0, 0        ; address rematerialised
        %319:gpr = t2LDRi12 %511:gpr, 0        ; fresh layout value
        %320:rgpr= t2LDRi12 %29:gpr, 32        ; top  <- %29 LIVE ACROSS THE CALL (ours)
        %323:gprnopc = t2LDRi12 %319:gpr, 12   ; leftMargin
        %325 = t2ADDrr %323, %320              ; lm + top
```

In the original `%320` is `t2LDRi12 %319, 32`, so `%29` is dead at the
call. That single difference explains Region B (the allocator can then put
`%29` in caller-saved `r2` and reuse it for `%306`) and Region C (the `top`
load moves after the call).

## 4. Variants measured this wave

All measurements are full gold link + per-function `verify.py` (`score.py`
uses `difflib.SequenceMatcher(autojunk=False)` over the reconciled
mnemonics). Every variant reverted; HEAD restored.

| variant | size | pct | diffs |
|---|---:|---:|---:|
| HEAD (`Layout &layout`, `top` cached, `lm`/`c3` fresh) | 1896 | **98.092643** | **18** |
| `const Layout &layout` (same body) | 1896 | 98.092643 | 18 |
| `int right = Globals::w - layout.buttonInsetX;` then `x > right - W()` | 1896 | 98.092643 | 18 |
| `int right = Globals::w - Globals::layout->buttonInsetX;` | 1896 | 98.092643 | 18 |
| `(*Globals::layout).field_0xc_leftMargin` (deref spelling) | 1896 | 98.092643 | 18 |
| `(*Globals::layout).buttonInsetX` for c1 / for c3 | 1896 | 98.092643 | 18 |
| `*(this->heapBufB + this->field_0x1e0)` / `*this->heapBufA` | 1896 | 98.092643 | 18 |
| `lm + (top + H)` (LGLG, `top` fresh, `lm` cached) | 1896 | 97.956403 | 18 |
| `Globals::layout->top + (layout.lm + H)` (LGLG, c1/c3 G) | 1896 | 97.956403 | 18 |
| `top + lm + H` (flat, `top` cached) | 1896 | 97.138965 | 23 |
| `(top + lm) + H` (explicit grouping, `top` cached) | 1896 | 97.138965 | 23 |
| `top + (H + lm)` / `H + (top + lm)` / `H + (lm + top)` | 1896 | 94.414169 | 53–54 |
| `lm + (H + top)` | 1892 | 90.661213 | 78 |
| all-direct (`Globals::layout->` everywhere, paren) | 1892 | 90.313779 | 156 |
| all-direct flat / `(top+lm)+H` / LGGG (`layout` for c1 only) | 1892 | 90.313779 | 156 |
| `top` fresh + `lm` fresh + `c3` cached (LGGL) | 1888 | 90.102389 | 148 |
| `Globals::layout->top + (layout.lm + H)` with c3=L (LGLL) | 1892 | 90.723056 | 146 |
| both fields cached (`layout.top + (layout.lm + H)`) | 1892 | 90.859482 | 155 |
| `volatile Layout &layout` | 1892 | 89.631651 | 164 |
| `int topv = layout.field_0x20_top;` hoist | 1872 | 89.650446 | 164 |
| `int lmv = Globals::layout->field_0xc_leftMargin;` hoist | 1872 | 90.061686 | 163 |
| `const Layout *layout = Globals::layout;` (all `layout->`) | 1888 | 90.573770 | 156 |
| third-if operand swap `W() + buttonInsetX` | 1908 | 89.613035 | 150 |
| `x > Globals::w - (buttonInsetX + W())` | 1900 | 88.888889 | 166 |
| case-8 first-if 4th cond re-associations (`t_lm_w`, `lm_tw`, `t_lmw`, `lt_w`) | 1896 | 98.092643 | 18 |
| case-8 first-if `w_lm_t` / `w_lt` | 1896 | 97.956403 | 19 |

### Why `%29` dead costs 4 B

With `top`/`lm` both fresh (`%29` dead), the spiller keeps `%508`
(`&Globals::layout`) in `fp` from bb.41 through bb.47 instead of
rematerialising it:

```
ours (1892 B):  bb.41  ldr.w fp,[sp,#16]
                bb.44  ldr.w r2,[fp]           ; 1 insn
                bb.47  ldr.w r2,[fp]           ; 1 insn
orig (1896 B):  bb.41  ldr   r4,[sp,#16]
                bb.44  ldr r2,[sp,#16]; ldr r2,[r2,#0]   ; 2 insns
                bb.47  ldr r2,[sp,#16]; ldr r2,[r2,#0]   ; 2 insns
```

That is −2 instructions (732 vs 734) and +4 B at the preheader
(`ldr r4,[sp,#16]` 2 B → `ldr.w fp,[sp,#16]` 4 B), net −4 B, which then
shifts every later branch target and pool entry (the 156-line "cascade").

## 5. Verdict

The residual is a Greedy-RA live-range-splitting tie-break, not a source
shape difference. The original's IR is HEAD's IR with `%29` spilled /
rematerialised across the first call; every legal source shape that makes
`%29` dead also drops the two address rematerialisations (4 B, 1892 B), and
every shape that keeps the address rematerialised also keeps exactly one of
`top`/`lm` cached in `r6` (18 diffs, 1896 B). The 1896-B family is exactly
{LLGG, LGLG, GLGG, GGLG} (`top` cached, `lm` fresh, `c3` fresh; `c1` free).
`-mllvm` knobs (`split-spill-mode`, `regalloc-csr-first-time-cost`,
`enable-deferred-spilling`, `sink-insts-to-avoid-spills`,
`disable-spill-hoist`, `spp-rematerialization-threshold`) do not change the
`[sp,#16]` pattern. Region A (`ip` vs `r4`) is an independent RA tie-break
in a block whose code is otherwise identical.

Cumulative across waves 118/119 (>300 shapes): best remains
**98.092643 %**, 1896 B, 18 diffs. The next lever would have to perturb
global liveness from *outside* case 15 while keeping the 700+ already
matching instructions, or a different clang-7 scheduling outcome.

## 6. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w6945
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
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
`%320:rgpr = t2LDRi12 %29:gpr, 32` in bb.44 (the cached `top`).
