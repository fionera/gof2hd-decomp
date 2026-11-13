# MenuTouchWindow::OnTouchBegin wave 191b (w13666): RA keep-vs-remat tie-break, no win

Date: 2026-11-13. Worktree `wt/w13666`, branch `wave191b-w13666`, base
`ad6d56523`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, function at `0x129bd4`, 1,896 B).
Baseline and best measured this wave: **raw 28 / aligned 16 / size
1896/1896 / 754/754 insns**. No source shape tested beat HEAD; `src/` is
unchanged and the gate is green.

Gate at HEAD and after this wave (`check`): `RATCHET PASS` — byte_exact
**4508/4524**, linked_exact 4508/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370 (0 extra / 0
missing), stub_zero_size 0. Section deltas unchanged: `.text` 71,210,
`.ARM.extab` 577 (section size 156,312), `.ARM.exidx` 0, `.rodata` 0,
`.note.gnu.build-id` 20; `.rel.plt` byte-identical.

## 1. Residual map at HEAD (28 bytes, 16 aligned lines)

All four regions are pure register-allocation differences; the instruction
streams are otherwise identical (754/754 instructions).

| region | offset(s) | original | ours |
|---|---|---|---|
| A (case 8, 1st `if`) | `0x3fe`,`0x404` | `ldr.w ip,[sl,#340]` / `add r3,ip` | `ldr.w r4,...` / `add r3,r4` |
| B (case 15 c1) | `0x60c`..`0x630` | `r3=heapBufB`, `r6=field_0x1e0`, `r5=w`, `r2=layout value` | `r2/r3/r4/r6` permutation |
| C (case 15 c2) | `0x63e`..`0x652` | fresh `Globals::layout` reload; `leftMargin` (off 12) **and** `top` (off 32) both from it | `leftMargin` from cached `Layout&` (`%29`), `top` fresh |
| D (case 15 c3) | `0x672`,`0x678` | `r5=buttonInsetX` | `r4=buttonInsetX` |

The `[sp,#16]` spill/reload pattern is identical to HEAD: 8 sites (entry
store at `0x129c14`, loads at `0x129d20`, `0x129f84`, `0x129ffa`,
`0x12a1bc`, `0x12a212`, `0x12a23c`, `0x12a2b6`). `fp` = `&Globals::Canvas`
in both (`ldr.w fp,[r1]` at `0x12a1ec`; deref `f8db 0000` at
`0x12a1f8/0x12a21c/0x12a242`).

## 2. Root cause re-confirmed with pre-RA MIR

Compiled the TU with the exact CMake command plus
`-mllvm -print-machineinstrs` and extracted the
`After Machine Instruction Scheduler` (pre-RA) and
`After Greedy Register Allocator` (post-RA) dumps.

**HEAD bb.42/44 pre-RA** (the `Layout &layout = *Globals::layout;` reference
is `%29`, live across the first call for the c2 `leftMargin`):

```
bb.42:
  %301 = [%298]              ; &Globals::w
  %29  = [%99]               ; Globals::layout value   <-- cached for c2 lm
  %306 = [%29, 40]           ; buttonInsetX
  %309 = %305 - %306         ; w - inset
  call GetImage2DWidth
bb.44:
  %319 = [%99]               ; fresh Globals::layout value
  %320 = [%29, 12]           ; leftMargin  <- cached %29
  %323 = [%319, 32]          ; top         <- fresh
```

**D (all-direct) bb.42/44 pre-RA** — instruction-for-instruction the
original's bb.42/44, with `%29` removed and `%320 = [%319, 12]` fresh:

```
bb.42:
  %301 = [%98]               ; Globals::layout value (short-lived)
  %306 = [%301, 40]          ; buttonInsetX
  call GetImage2DWidth
bb.44:
  %319 = [%98]               ; fresh Globals::layout value
  %322 = [%319, 12]          ; leftMargin  <- fresh
  %323 = [%319, 32]          ; top         <- fresh
```

D reproduces the original's instruction stream exactly but is **1892 B**:
Greedy-RA keeps the `&Globals::layout` address (`%98`) in `fp` from the
start of case 15 across both calls (`ldr.w fp,[sp,#16]` + `ldr.w r2,[fp]`
at bb.44/bb.47), saving one `ldr rX,[sp,#16]`. The original reloads the
address at every use (`ldr r4,[sp,#16]` at `0x5e8`, `ldr r2,[sp,#16]` at
`0x63e`, `ldr r0,[sp,#16]` at `0x668`) and gives `fp` to `&Globals::Canvas`.
The third state — address spilled **and** `fp = &Globals::Canvas` **and**
both c2 fields fresh — is not reachable by any source shape tested this
wave.

## 3. New measurements this wave

Every candidate was a full gold link measured with `tools/fn_diff.py`;
`src/` was restored after each. All sizes are 1896 unless noted.

### 3.1 case 15 — all-direct D and hoisted locals

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (lm cached, top fresh) | **28** | **16** | 1896 |
| D (all-direct) | 362 | 76 | 1892 |
| D + `int w = Globals::w` | 362 | 76 | 1892 |
| D + `int inset = Globals::layout->buttonInsetX` | 362 | 76 | 1892 |
| D + `int lm/top` locals | 320 | 33 | 1892 |
| D + `PaintCanvas *canvas` (both calls) | 316 | 40 | 1892 |
| D + `Layout *lp` (c2 lm+top) | 308 | 73 | 1892 |
| D + `Layout *lp` (c1+lm) | 28 | 16 | 1896 |
| D + `Layout *lp` (c1+top) | 29 | 15 | 1896 |
| D + `Layout *lp` (c1 only) | 362 | 76 | 1892 |
| D + `int w` + `Layout *lp` (c1+lm) | 28 | 16 | 1896 |
| D + `int w` + `Layout *lp` (c1+top) | 29 | 15 | 1896 |
| D + `w`/`canvas`/`inset` hoisted (all orders) | 316 | 40 | 1892 |
| D + `w`/`lp`/`canvas` (all orders) | 332 | 76 | 1892 |
| D + hoisted `width`/`height` call results | 354-386 | 97-112 | 1880-1892 |
| D + `height` + `lm`/`top` local | 840 | 158 | 1896 |
| D + `lp` + `height` local | 607 | 135 | 1896 |
| D + `fresh = Globals::layout` comma / pointer assignment | 362 | 76 | 1892 |
| D + `Globals::layout != 0 ? ...` | 358 | 90 | 1896 |
| D + `while` forms of the two pre-block loops | 362-364 | 76-77 | 1892 |

The 16-way `{c1,c2lm,c2top,c3}` cached/fresh mask grid was re-run with both
`Layout &` and `Layout *` locals; it reproduces the w13603/w13612 buckets
exactly (`02/03` = 28/16/1896, `04/05` = 29/15/1896, everything with both c2
fields fresh = 1892 or smaller).

### 3.2 case 15 — c2 association / c1 order

| c2 / c1 expression | raw | aligned | size |
|---|---:|---:|---:|
| `top + (lm + height)` (D) | 362 | 76 | 1892 |
| `top + lm + height` | 362 | 76 | 1892 |
| `lm + top + height` | 362 | 76 | 1892 |
| `(top + lm) + height` | 362 | 76 | 1892 |
| `lm + (top + height)` | 362 | 76 | 1892 |
| `height + top + lm` / `height + lm + top` | 369-370 | 111 | 1892 |
| `lm + height + top` / `top + height + lm` | 380 | 87 | 1892 |
| c1 `w - width - inset` / `(w - width) - inset` | 345 | 85 | 1884 |
| c1 `w - (inset + width)` | 365 | 85 | 1892 |
| c2 `y - top - lm < height` | 358 | 98 | 1892 |

### 3.3 case 8 — region A (ip vs r4)

| variant | raw | aligned | size | region A |
|---|---:|---:|---:|---|
| HEAD | **28** | **16** | 1896 | r4 |
| `(unsigned)` / `(unsigned int)` cast | 29 | 17 | 1896 | r4 |
| `(unsigned short)` / `(short)` / `(char)` / `(unsigned char)` | 29-30 | 16 | 1896 | r4 |
| `& 0xffff` / `| 0` / `^ 0` / `* 1` / `-(-x)` / `x - 0` / `0 + x` | 28-29 | 16 | 1896 | r4 |
| `<< 0` / `>> 0` / `/ 1` / `~(~x)` / comma / ternary / bool | 28 | 16 | 1896 | r4 |
| `volatile` read / reference / pointer cast | 28-29 | 16-17 | 1896 | r4 |
| `lw` first / `lw` middle / `lm + (lw + top)` | 33 | 17 | 1896 | r4 |
| `(lm + top) + lw` / `top + (lm + lw)` | 28 | 16 | 1896 | r4 |
| second-`if` 4th-condition variants (5 orders) | 28-34 | 16-18 | 1896 | r4 |
| second-`if` 1st-condition variants (5 orders) | 28 | 16 | 1896 | r4 |
| 23 of 24 first-`if` `&&` permutations | 44-1373 | 24-520 | 1896-1912 | r4/? |
| `__builtin_expect` on c2/c3 | 28 | 16 | 1896 | r4 |
| `__builtin_expect` on c0/c1 | 558 | 68 | 1896 | r4 |
| `!(&& chain)` | 1286 | 183 | 1892 | **ip** |

Only reordering case 8 in the switch (or inverting the whole first `if`)
flips region A to `ip`; every such reorder also changes the `tbh` jump
table and/or the literal-pool permutation, so raw rises to 54+.

### 3.4 earlier-code perturbations (vreg-number shifts)

Removing/renaming `previous`, changing loop-variable types (`unsigned int`
vs `int`), swapping case-8 declaration order, swapping `upButtonPressed`/
`downButtonPressed`, swapping the case-1/2 drag stores, changing `row`/
`listTopY`/`buttonInsetX` types: all leave D at **362/76/1892** (or worse).
The RA tie-break is insensitive to these vreg-number shifts.

### 3.5 RA knob sweep on D (single-TU object)

`-mllvm` knobs leave the object at **1892 B** (`0x764`) with the same
allocation: `-regalloc-csr-first-time-cost=0/1/100/1000`,
`-spp-rematerialization-threshold=0/1/10/1000`,
`-enable-deferred-spilling`, `-split-spill-mode=size/speed`,
`-remat-pic-stub-load=true/false`, `-stress-regalloc=13/14/15/16`.
`-stress-regalloc=8..12` changes the size (1928/1864/1832/1928/1916) but
never to 1896. So the keep-vs-remat decision is not exposed by the
available Greedy-RA cost knobs.

## 4. Conclusion

Every shape lands in one of two buckets:

* `%29` live across the first call -> 1896 B, `fp = &Globals::Canvas`,
  `%98` reloaded at all 8 sites; raw 28 (lm cached) or 29 (top cached).
* `%29` dead (all-direct) -> 1892 B, `%98` kept in `fp` from the case-15
  start, `&Globals::Canvas` in `r5`; raw 291-362.

The original's third combination (`%29` dead, `%98` reloaded, `fp =
&Globals::Canvas`) was not reachable by the ~150 new shapes measured this
wave. Region A is an independent Greedy-RA `ip`/`r4` tie-break; every
spelling that flips it also perturbs the jump table or literal pool. HEAD
is unchanged; the gate is green.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13666
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# raw=28 aligned=16 size=1896/1896 insn=754/754
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13666` (`sweep15*.py`,
`sweep8*.py`, `sweep_c3.py`, `sweep_fresh.py`, `sweep_loops2.py`,
`sweep_final.py`, `knob.sh`, `head.mir`, `d.mir`).
