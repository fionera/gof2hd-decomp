# MenuTouchWindow::OnTouchBegin wave 127 (w7615): final RA tie-break audit, no source win

Date: 2026-11-07. Worktree `wt/w7615`, branch `wave127-w7615`, base
`bf365a964`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `Android (4751641 based on r328903)`, `-Oz`,
image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`). Baseline and best measured this wave:
**98.092643 %**, 1896 B, 734/734 normalised instructions, `byte=False`,
`linked=False`, **18** differing instruction lines (header excluded).

Gate at HEAD: `byte_exact 4506 / linked_exact 4506`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.

## TL;DR

Only a **byte-identical source-faithfulness refactor** landed: `OnTouchBegin`
now declares the function-top `Layout *layout = Globals::layout;` local shown
by the oracle and re-binds it after the case-8 sound call (the compiler
already CSE'd this; the emitted code is bit-for-bit identical to HEAD,
verified by an identical `verify.py --show` diff). The residual is the same
18 lines in 4 regions as waves 118/119/121/122/123. This wave re-derived the exact post-RA MIR of the
original's case-15 block and confirmed the root cause is a **single Greedy-RA
rematerialisation decision for `&Globals::layout`** that is coupled to the
liveness of the cached `Globals::layout` value. Every source shape that makes
the value dead at the first `GetImage2DWidth` call (the original's IR) makes
the RA keep `&Globals::layout` in `fp` and shortens the function to 1892 B;
every shape that keeps the value live across the call keeps the address
rematerialised and the function at 1896 B but reads `top` from the cached
value. The two decisions cannot be separated by any legal C++ shape tested
(>600 cumulative across waves 118-127).

## 1. The residual (18 instruction lines)

`tools/verify/verify.py --show` diff (orig | ours), function at `0x129bd4`,
1896 B both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

Pure register choice for a value that dies in the next instruction. `ip`
(r12) is free on our side too; Greedy RA still picks the callee-saved `r4`.
All 12 parenthesizations/permutations of
`leftMargin + field_0x20_top + listEntryWidth` were re-measured: every one
allocates `r4`. `y - lm - top < w`, `int entryW = ...`, casts and volatile
reads are all neutral or worse (19-113 lines).

### Region B — `case 15`, scrollbar `if`, first condition

```
ldr.w  r3, [sl, #312]        |  ldr.w  r2, [sl, #312]     ; heapBufB
ldr.w  r6, [sl, #480]        |  ldr.w  r3, [sl, #480]     ; field_0x1e0
ldr    r2, [r4, #0]          |  ldr    r6, [r4, #0]       ; Globals::layout value
ldr.w  r1, [r3, r6, lsl #2]  |  ldr.w  r1, [r2, r3, lsl #2]
ldr    r5, [r0, #0]          |  ldr    r4, [r0, #0]       ; Globals::w
ldr    r2, [r2, #40]         |  ldr    r5, [r6, #40]      ; buttonInsetX
subs   r5, r5, r2            |  subs   r4, r4, r5
subs   r0, r5, r0            |  subs   r0, r4, r0
```

### Region C — `case 15`, second condition tail

```
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]      ; top (cached %29)
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]      ; leftMargin (fresh)
adds   r1, r6, r5            |  adds   r1, r5, r4
```

### Third `if`

```
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]      ; buttonInsetX
add    r0, r5                |  add    r0, r4
```

## 2. Original vs HEAD, case 15, side by side

Original (`0x12a1bc`-`0x12a24e`):

```
12a1bc  ldr    r4, [sp, #16]      ; &Globals::layout (hoisted before scrollSlots loop)
12a1f0  ldr    r2, [r4, #0]       ; %29 = Globals::layout
12a1fc  ldr    r2, [r2, #40]      ; buttonInsetX  (r2 = %29 dies here)
12a200  blx    GetImage2DWidth
12a212  ldr    r2, [sp, #16]      ; address REMAT
12a218  ldr    r2, [r2, #0]       ; fresh value
12a21e  ldr    r5, [r2, #12]      ; leftMargin  (fresh)
12a220  ldr    r6, [r2, #32]      ; top         (fresh)
12a222  blx    GetImage2DHeight
12a23c  ldr    r0, [sp, #16]      ; address REMAT
12a240  ldr    r2, [r0, #0]       ; fresh value
12a246  ldr    r5, [r2, #40]      ; buttonInsetX (fresh)
```

HEAD (`0x12a1bc`-`0x12a24e`):

```
12a1bc  ldr    r4, [sp, #16]      ; &Globals::layout
12a1f0  ldr    r6, [r4, #0]       ; %29 = Globals::layout
12a1fc  ldr    r5, [r6, #40]      ; buttonInsetX  (%29 STAYS LIVE)
12a200  blx    GetImage2DWidth
12a212  ldr    r2, [sp, #16]      ; address REMAT
12a214  ldr    r4, [r6, #32]      ; top         (cached %29)
12a218  ldr    r2, [r2, #0]       ; fresh value
12a21c  ldr    r5, [r2, #12]      ; leftMargin  (fresh)
12a23c  ldr    r0, [sp, #16]      ; address REMAT
12a240  ldr    r2, [r0, #0]       ; fresh value
12a246  ldr    r4, [r2, #40]      ; buttonInsetX (fresh)
```

So HEAD already has the **correct instruction count and size** (3 address
loads + 3 value loads, all 16-bit). The only difference is that `top` is
read from the cached `%29` (r6) instead of the fresh value (r2), which is
exactly the liveness difference.

## 3. Post-RA MIR evidence (exact)

HEAD (`-mllvm -print-after-all`, `# *** IR Dump After Greedy Register
Allocator ***`):

```
bb.37:  %508 = t2LDRi12 %stack.0, 0        ; &Globals::layout
bb.42:  %29  = t2LDRi12 %508, 0            ; Globals::layout value
        %306 = t2LDRi12 %29, 40            ; buttonInsetX
        tBL GetImage2DWidth
bb.44:  %511 = t2LDRi12 %stack.0, 0        ; address REMAT
        %319 = t2LDRi12 %511, 0            ; fresh value
        %320 = t2LDRi12 %29, 32            ; top  <-- %29 LIVE ACROSS CALL
        %323 = t2LDRi12 %319, 12           ; leftMargin (fresh)
        tBL GetImage2DHeight
bb.47:  %510 = t2LDRi12 %stack.0, 0        ; address REMAT
        %335 = t2LDRi12 %510, 0            ; fresh value
```

All-direct source (`Globals::layout->field_0x20_top`, no reference local):

```
bb.37:  %508 = t2LDRi12 %stack.0, 0        ; &Globals::layout -> fp
bb.42:  %301 = t2LDRi12 %508, 0            ; value
        %306 = t2LDRi12 %301, 40
        tBL GetImage2DWidth
bb.44:  %319 = t2LDRi12 %508, 0            ; value again (address still in fp)
        %323 = t2LDRi12 %319, 12           ; leftMargin
        %320 = t2LDRi12 %319, 32           ; top  <-- both fresh
        tBL GetImage2DHeight
```

The pre-RA IR is otherwise identical (modulo virtual register numbering).
The single difference is `%320 = t2LDRi12 %29, 32` (HEAD) vs
`%320 = t2LDRi12 %319, 32` (all-direct). Because `%29` is live across the
first call, Greedy RA splits the address interval and rematerialises it
(3 x `ldr rX,[sp,#16]`, 16-bit). When `%29` is dead, the RA keeps the
address in `fp` (1 x `ldr.w fp,[sp,#16]`, 32-bit), and the function shrinks
to 1892 B.

## 4. Variants measured this wave (all reverted)

All measurements are full gold link + `verify.py --show`; diffs are
structural `|` lines (header excluded).

| variant | size | diffs |
|---|---:|---:|
| **HEAD (`Layout &layout`, top cached)** | **1896** | **18** |
| all-direct (`Globals::layout->` everywhere) | 1892 | 165 |
| oracle shape: function-top `Layout *layout`, reassigned after case-8 sound | 1896 | 18 |
| oracle shape + `right`/`width` locals, `H+T+L` | 1892 | 166 |
| oracle shape + `right`/`width` locals, `T+(L+H)` | 1892 | 165 |
| oracle shape + `right`/`width` locals, `(T+L)+H` | 1892 | 165 |
| `int right` local + direct rest | 1892 | 165 |
| pointer local for c1 only, direct c2/c3 | 1892 | 165 |
| reference local for c1 only, direct c2/c3 | 1892 | 165 |
| `lm` cached / `top` fresh (`L G L G`) | 1896 | 18 (different region) |
| c1 cached / c2 fresh / c3 cached | 1888 | 158 |
| `bool hit = c1 && c2; scrollbarHit = hit;` | 1896 | 164 |
| `scrollbarHit = (c1 && c2) ? 1 : 0;` | 1896 | 164 |
| nested `if` after c1 (`Layout &layout` inside) | 1896 | 164 |
| nested `if` after c1 (direct) | 1892 | 165 |
| `y - top - lm < H` | 1892 | 169 |
| `H + T + L` direct | 1892 | 166 |
| `PaintCanvas *canvas` at function top + direct case 15 | 1896 | 730 |
| `PaintCanvas *canvas` before c1 | 1892 | 158 |
| `*(Layout *volatile *)&Globals::layout` both fields | 1896 | 133 |
| `*(Layout *volatile *)&Globals::layout` c2 only | 1896 | 127 |
| case-8 4th condition: 12 permutations | 1896 | 18-20 |
| case-8 `y - lm - top < w` | 1896 | 113 |

`-mllvm` knobs re-checked on the all-direct shape (manual TU compile):
`spp-rematerialization-threshold=0..1000`, `regalloc-csr-first-time-cost=0..1000`,
`enable-deferred-spilling=0/1`, `disable-spill-hoist=1`,
`sink-insts-to-avoid-spills=0/1`, `split-spill-mode=default`,
`regalloc=greedy` — **every one produces the same 1892-byte object**.

## 5. Root cause and verdict

The residual is a Greedy-RA live-range/rematerialisation tie-break:

* Keep `%29` live across the first call (HEAD): the address interval is
  split and rematerialised from `[sp,#16]`; the function stays 1896 B, but
  `top` is read from the cached `%29` (18 diffs).
* Make `%29` dead at the call (the original's IR): the RA keeps
  `&Globals::layout` in `fp`; the function shrinks to 1892 B and every
  branch target shifts (164-165 diffs).

The original binary proves the source-level shape is all-direct (fresh
`leftMargin` and `top` from one reloaded value), but clang 7.0.2's Greedy RA
makes the opposite remat choice on our TU. No legal C++ shape found in
waves 118-127 (>600 shapes) separates the two decisions. HEAD remains the
global optimum at **98.092643 %**, 1896 B, 18 diffs.

## 6. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w7615
TMPDIR=/dev/shm/w7615/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w7615/tmp cmake --build cmake-build-match --target check -j8
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
`%320 = t2LDRi12 %29, 32` in bb.44 (cached `top`) and
`%511 = t2LDRi12 %stack.0, 0` (address remat).

Scratch (not tracked): `/dev/shm/w7615/` (`base.cpp`, `try.sh`,
`variant.py`, `batch*.py`, `mir.sh`, `show.txt`, `mir.txt`, `gra.txt`).
