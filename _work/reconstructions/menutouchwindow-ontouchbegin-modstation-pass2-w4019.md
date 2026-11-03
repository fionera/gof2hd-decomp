# MenuTouchWindow::OnTouchBegin + ModStation C1/C2 pass 2 — w4019

Date: 2026-11-03. Branch `wave90-w4019` (base master `360c7467`).
Gate before/after: **byte_exact 4498 / linked_exact 4498 / size-exact 4524**,
`.text` `0x1337c8`, `.rodata` `0x41b67`, `.ARM.exidx` `0x6f70`,
`.ARM.extab` `0x26290` (-8), data_exact 843/843, imports 207, lint CLEAN,
drift 0. **No code change landed**; this note records the second source-shape
search and why the residual is not reachable from the C++ source.

Prior note: `_work/reconstructions/menutouchwindow-ontouchbegin-97-7-modstation-c1-c2-98-6_w3819.md`
(hereafter "w3819"). This pass re-verified every claim in it with the official
verifier and added the experiments listed below. All measurements use
`tools/verify/verify.py --show <mangled>` (`autojunk=False`) against
`../_work/bins/android_2.0.16_libgof2hdaa.so`; scratch lived in `/dev/shm/w4019`.

## Current state (re-measured)

| symbol | size orig/ours | fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | 97.7% | 27 |
| `_ZN10ModStationC1Ev` | 1008 / 1008 | 98.6% | 16 |
| `_ZN10ModStationC2Ev` | 1008 / 1008 | 98.6% | 16 |

Both are size-exact; there is no missing code, wrong constant, wrong field
offset or wrong callee.

## MenuTouchWindow::OnTouchBegin

### The two residual regions

Only two regions differ (addresses are the original VMA; ours is at the same
VMA because the function is size-exact and the section order is pinned):

1. **Case 8** (`upButtonPressed` / `downButtonPressed`), at `0x129fd2`:
   original `ldr.w ip,[sl,#340]` / `add r3,ip`; ours `ldr.w r4,[sl,#340]` /
   `add r3,r4`. `r4` is unused in the original's case-8 block; the original
   allocator still picks `ip` (r12) for the short-lived `listEntryWidth`
   temporary. This is a register-choice tie-break, not a source difference.

2. **Case 15/16** (`scrollbarHit` + drag init), `0x12a1da..0x12a24e`.
   Original:

```
12a1da  ldr   r0,[pc,#316]        ; GOT &Globals::w
12a1dc  ldr   r1,[pc,#316]        ; GOT &Globals::Canvas
12a1de  add   r0,pc
12a1e0  ldr.w r3,[sl,#312]        ; this->heapBufB
12a1e4  add   r1,pc
12a1e6  ldr.w r6,[sl,#480]        ; this->field_0x1e0
12a1ea  ldr   r0,[r0,#0]          ; &Globals::w
12a1ec  ldr.w fp,[r1]             ; &Globals::Canvas
12a1f0  ldr   r2,[r4,#0]          ; Globals::layout   (r4 = &Globals::layout, from [sp,#16])
12a1f2  ldr.w r1,[r3,r6,lsl#2]    ; heapBufB[field_0x1e0]
12a1f6  ldr   r5,[r0,#0]          ; Globals::w
12a1f8  ldr.w r0,[fp]             ; Globals::Canvas
12a1fc  ldr   r2,[r2,#40]         ; buttonInsetX
12a1fe  subs  r5,r5,r2            ; w - buttonInsetX
12a200  blx   GetImage2DWidth
12a204  subs  r0,r5,r0
12a206  cmp   r0,r8               ; x
12a208  bge.n 12a232
12a20a  ldr.w r0,[sl,#312]
12a20e  ldr.w r1,[sl,#480]
12a212  ldr   r2,[sp,#16]         ; &Globals::layout (reloaded from the spill)
12a214  ldr.w r1,[r0,r1,lsl#2]
12a218  ldr   r2,[r2,#0]          ; Globals::layout
12a21a  ldr.w r0,[fp]             ; Globals::Canvas
12a21e  ldr   r5,[r2,#12]         ; leftMargin
12a220  ldr   r6,[r2,#32]         ; top
12a222  blx   GetImage2DHeight
12a226  adds  r1,r6,r5            ; top + leftMargin
12a228  add   r0,r1               ; height + top + leftMargin
12a22a  cmp   r0,r9               ; y
12a22c  ble.n 12a232
12a22e  movs  r0,#1
12a230  b.n   12a234
12a232  movs  r0,#0
12a234  ldr.w r1,[sl,#308]        ; this->heapBufA
12a238  strb.w r0,[sl,#473]       ; scrollbarHit
12a23c  ldr   r0,[sp,#16]         ; &Globals::layout (reloaded again)
12a23e  ldr   r1,[r1,#0]
12a240  ldr   r2,[r0,#0]          ; Globals::layout
12a242  ldr.w r0,[fp]             ; Globals::Canvas
12a246  ldr   r5,[r2,#40]         ; buttonInsetX
12a248  blx   GetImage2DWidth
12a24c  add   r0,r5
12a24e  cmp   r0,r8
```

   Current source (the best measured shape, unchanged from w3819):

```cpp
{
    Layout &layout = *Globals::layout;
    int top = Globals::layout->field_0x20_top;
    if (x > Globals::w - layout.buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
        y < Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]) + top + layout.field_0xc_leftMargin)
        this->scrollbarHit = 1;
    else
        this->scrollbarHit = 0;
    if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
        ...
    }
}
```

   Ours keeps `Globals::layout`'s **value** in `r6` and spills `top` to
   `[sp,#12]`; the original keeps `&Globals::layout` in `[sp,#16]`, reloads it
   for every use, and keeps `&Globals::Canvas` in `fp`. The original's
   `r4 = &Globals::layout` is used for exactly one access
   (`12a1f0`) and is then rematerialised from the stack twice
   (`12a212`, `12a23c`).

### Why the faithful direct-global shape is 4 bytes short

The faithful source is direct `Globals::layout` access (Ghidra confirms three
separate `*(int *)(*piVar3 + …)` loads):

```cpp
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(...) &&
    y < Globals::Canvas->GetImage2DHeight(...) + Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin)
    this->scrollbarHit = 1;
else
    this->scrollbarHit = 0;
```

It compiles to **1892 B / 166 diff lines**. The 4-byte shortfall is exactly
the `ldr r2,[sp,#16]` that the original executes before the second
`Globals::layout` access: with direct globals clang 7.0.2 keeps
`&Globals::layout` in `fp` across the `GetImage2DWidth` call, so the second
access is one `ldr r2,[fp]` instead of `ldr r2,[sp,#16]; ldr r2,[r2,#0]`.
The original instead keeps `&Globals::Canvas` in `fp` and treats the layout
address as a rematerialisable spill. This is a register-allocation choice, not
a source-semantics difference.

### Source-shape sweep (all measured)

The block was replaced by each variant below and the whole `.so` re-linked and
verified. "diffs" is the number of `|` structural-diff lines in
`verify.py --show`; lower is better.

| variant | size | diffs | note |
|---|---:|---:|---|
| **current** (`Layout&` + hoisted `top`) | 1896 | **27** | best |
| direct `Globals::layout` (top+left) | 1892 | 166 | faithful, 4 B short |
| direct `Globals::layout` (left+top) | 1892 | 166 | |
| direct, parenthesised `w - (inset + width)` | 1892 | 173 | |
| direct, `w - width - inset` | 1880 | 171 | |
| `Layout *layout = Globals::layout` + hoisted `top` | 1896 | 27 | same codegen as current |
| `Layout *const layout` + hoisted `top` | 1896 | 27 | |
| `const Layout &layout` + hoisted `top` | 1896 | 27 | |
| `int top` only, direct layout fields | 1896 | 67 | no cache, but fp/Canvas swapped |
| `Layout **layout = &Globals::layout` | 1892 | 166 | |
| `Layout *&layout = Globals::layout` | 1892 | 166 | |
| `Layout *const &layout` | 1892 | 166 | |
| `Layout *volatile &layout` | 1896 | 138 | forces reload, but also volatile field loads |
| `Layout *volatile layout` | 1928 | 763 | local volatile pointer |
| `volatile Layout &layout` | 1884 | 174 | volatile field loads |
| `bool hit = cond; scrollbarHit = hit` | 1892 | 165 | |
| `scrollbarHit = cond1 && cond2` | 1892 | 165 | |
| nested `if` (cond1) { if (cond2) … } | 1896 | 113 | `ite` form |
| `if (!(cond1 && cond2)) 0 else 1` | 1892 | 165 | |
| local `int w = Globals::w` | 1892 | 166 | |
| `PaintCanvas *&canvas` reference | 1892 | 166 | |
| `PaintCanvas **canvas = &Globals::Canvas` | 1892 | 166 | |
| local `Layout&` + direct second `if` (current) | 1896 | 27 | |
| local `Layout&` + local second `if` | 1892 | 161 | |
| `ref_nohoist` (`layout.field_0x20_top` inline) | 1892 | 165 | |
| `ptr_nohoist` | 1892 | 165 | |

A 4×4 matrix of {local `layout` vs direct `Globals::layout` for `buttonInsetX`}
× {top from local vs direct} × {leftMargin local vs direct} × {second `if`
local vs direct} was also measured. The 27-diff cells are exactly the four
with the **second `if` direct and `top` hoisted**; the choice of local vs
direct for `buttonInsetX`/`top`/`leftMargin` does not change the 27-line
result (the current shape is one of them). Every cell with the second `if`
local is 161+ diffs; every cell that removes the `top` hoist is 1892 B.

Case 8's last condition was also re-ordered
(`left+top+width`, `top+left+width`, `width+left+top`,
`left+width+top`, `left+(top+width)`, `top+width+left`): 27 or 30 diffs,
never better. A local `listEntryWidth`/`listTopY` copy did not change the
`ip` vs `r4` choice.

### Conclusion for OnTouchBegin

The function is source-shape complete. The remaining 27 lines are two
independent allocator tie-breaks: `ip` vs `r4` for a short-lived case-8
temporary, and `fp`/`r6`/`[sp,#12]` vs `fp`/`r4`/`[sp,#16]` for the case-15
layout address. No plain C++ shape tried (including every combination of
local/direct, pointer/reference, volatile, const, declaration order,
parenthesisation and control-flow form) reproduces the original's exact
register assignment while keeping the 1896-byte size.

## ModStation::ModStation (C1/C2)

### The residual

The only difference is 16 instruction lines in the camera setup after
`MatrixSetTranslation(nearKey, …)`, at original VMA
`0xd5436..0xd5470`. The original:

```
d5436  ldr   r0,[pc,#632]      ; &gHangarCamPitch
d5438  ldr   r3,[pc,#632]      ; &gHangarCamYawIPad
d543a  ldr   r4,[pc,#636]      ; &gHangarCamYaw
d543c  add   r0,pc
d543e  add   r3,pc
d5440  ldrb.w r1,[sl]          ; Globals::iPad
d5444  add   r4,pc
d5446  ldr.w r2,[r0,r8,lsl#2]  ; pitchBits
d544a  mov   r0,r3
d544c  cmp   r1,#0
d544e  str   r3,[sp,#180]      ; save yawIPad pointer
d5450  str   r4,[sp,#176]      ; save yaw pointer
d5452  it    eq
d5454  moveq r0,r4
d5456  ldr.w r3,[r0,r8,lsl#2]  ; yawBits
d545a  movs  r0,#2
d545c  str.w r8,[sp,#184]      ; spill race
d5460  movs  r1,#0
d5462  add.w r8,sp,#256
d5466  strd  r1,r0,[sp]
d546a  add   r1,sp,#320
d546c  mov   r0,r8
d546e  mov   r4,r2             ; save pitchBits
d5470  blx   MatrixSetRotation
```

Ours is identical except that the yaw load, the `race` spill and the
`pitchBits` save are scheduled in a different order:

```
d5456  str.w r8,[sp,#184]
d545a  mov   r4,r2
d545c  ldr.w r3,[r0,r8,lsl#2]
d5460  movs  r0,#2
d5462  movs  r1,#0
d5464  add.w r8,sp,#256
d5468  strd  r1,r0,[sp]
d546c  add   r1,sp,#320
d546e  mov   r0,r8
d5470  blx   MatrixSetRotation
```

The first diff block (`d543a`..`d5446`) is the same scheduler effect on the
three table-address loads: the original issues all three `ldr rX,[pc]`
before the first `add rX,pc`; ours interleaves `add r0,pc` / `ldrb r1,[sl]`
before `ldr r4,[pc]`. Values and semantics are identical; only the post-RA
schedule differs. The rest of the 1008-byte function is byte-identical.

### Source-shape sweep (all measured)

The camera block was replaced by each variant below; the whole `.so` was
re-linked and verified.

| variant | size | diffs | note |
|---|---:|---:|---|
| **current** (`const float *pitchTable`, `const float *yawTable`, signal fence, volatile int loads) | 1008 | **16** | best |
| `const volatile int *` table pointers | 1008 | 16 | same codegen |
| direct `gHangarCamPitch[race]` + direct conditional yaw | 1000 | 261 | |
| direct pitch + local `yawTable` | 1008 | 16 | same codegen |
| local `pitchTable` + direct conditional yaw | 1000 | 261 | |
| no fence | 1008 | 18 | |
| fence after both loads | 1008 | 16 | |
| fence between pitch and yaw loads | 1008 | 16 | |
| yaw load before fence | 1000 | 65 | |
| pitch load before fence | 1008 | 18 | |
| `__atomic_thread_fence` | 1008 | 244 | |
| `__sync_synchronize` | 1008 | 244 | |
| `__ATOMIC_ACQUIRE` / `__ATOMIC_RELEASE` signal fence | 1008 | 16 | |
| `__builtin_memcpy` into `int` bits | 1008 | 258 | |
| `volatile int` locals | 992 | 277 | |
| yaw declared/loaded first | 1000 | 65 | |
| `int yawBits; int pitchBits;` declarations, pitch assigned first | 1008 | 16 | |
| `int yawBits, pitchBits;` pitch assigned first | 1008 | 16 | |
| `int pitchBits = …; int yawBits; yawBits = …;` | 1008 | 16 | |
| `race` copy used for yaw + second rotation | 1008 | 16 | |
| `pitchBits` copy used for second rotation | 1008 | 16 | |
| second rotation uses `pitchTable[race]` | 1008 | 16 | compiler CSEs it back |
| second rotation uses `yawTable[race]` | 984 | 271 | |
| second rotation uses both tables | 984 | 271 | |

Per-TU driver/backend flags were also tested on `ModStation.cpp` (a temporary
`set_source_files_properties` entry, reverted): `-fno-schedule-insns2`,
`-fno-schedule-insns`, both, `-mllvm -enable-post-misched=false`,
`-mllvm -enable-misched=false` all leave the function at **1008 B / 16
diffs**. This agrees with the w3902 knob sweep (no global or per-TU knob
raises any of the 26 targets).

### Conclusion for ModStation

The values, field offsets, callee and call arguments are exact. The residual
is a post-RA scheduler ordering of three independent instructions (the yaw
load, the `race` spill and the `pitchBits` save) plus the table-address load
interleave. No source shape or scheduling flag tried changes that ordering
without changing the function's size or register allocation elsewhere.

## What was proved

* Both functions are size-exact and semantically complete; the residual is
  entirely clang 7.0.2 register allocation / post-RA scheduling.
* The current `MenuTouchWindow` case-15 shape (local `Layout&`, hoisted
  `top`, direct second `if`) is a strict local optimum: the faithful direct
  shape is 4 bytes shorter because clang keeps `&Globals::layout` in `fp`
  across the `GetImage2DWidth` call.
* The current `ModStation` volatile-int + signal-fence shape is a strict local
  optimum; the 16-line residual is the same schedule under every variant
  tried, including per-TU scheduling flags.
* No code change is justified: `byte_exact` would only rise on a full
  function match, and no variant produced one.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4019
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# and --show _ZN10ModStationC1Ev / _ZN10ModStationC2Ev
```

Gate after (unchanged): `verify: byte 4498 linked 4498 stubs 0 missing 0
extra 0 avg 99.9`, `data: exact 843 of 843`, `sodiff: extra-beyond-allow 0
missing 0 allowed 0 imports 207`, `RATCHET PASS`.
