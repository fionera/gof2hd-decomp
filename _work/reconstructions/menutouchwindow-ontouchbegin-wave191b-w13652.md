# MenuTouchWindow::OnTouchBegin wave 191b (w13652): case-order sweep, no win

Date: 2026-11-13. Worktree `wt/w13652`, branch `wave191b-w13652`, base
`8dc75802b`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, function at `0x129bd4`, 1,896 B).
Baseline and best measured this wave: **raw 28 / aligned 16 / size
1896/1896 / 754/754 insns**. No shape tested beat HEAD; `src/` is unchanged
and the gate is green.

Gate at HEAD and after this wave (`check`): `RATCHET PASS` — byte_exact
**4508/4524**, linked_exact 4508/4524, size 4524/4524, data 843/843, imports
207, lint CLEAN, drift 0, symbol parity 5370/5370 (0 extra / 0 missing),
stub_zero_size 0. Section deltas unchanged: `.text` 71,196, `.ARM.extab`
577, `.ARM.exidx` 0, `.rodata` 0; `.rel.plt` byte-identical.

## 1. Residual map at HEAD (28 bytes, 16 aligned lines)

All four regions are pure register-allocation differences; every opcode and
operand shape is otherwise identical (754/754 instructions, all four regions
are register-field-only).

| region | offset(s) | original | ours |
|---|---|---|---|
| A (case 8, 1st `if`) | `0x401`,`0x404` | `ldr.w ip,[sl,#340]` / `add r3,ip` | `ldr.w r4,...` / `add r3,r4` |
| B (case 15 c1) | `0x60f`..`0x630` | `r3=heapBufB`, `r6=field_0x1e0`, `r5=w`, `r2=layout value` | `r2/r3/r4/r6` permutation |
| C (case 15 c2) | `0x640`..`0x652` | fresh `Globals::layout` reload; `leftMargin` (off 12) **and** `top` (off 32) both from it | `leftMargin` from cached `Layout&` (`%29`), `top` fresh |
| D (case 15 c3) | `0x672`,`0x678` | `r5=buttonInsetX` | `r4=buttonInsetX` |

The `[sp,#16]` spill/reload pattern is identical to HEAD: 8 sites (entry
store at `0x129c14`, loads at `0x129d20`, `0x129f84`, `0x129ffa`, `0x12a1bc`,
`0x12a212`, `0x12a23c`, `0x12a2b6`). `fp` = `&Globals::Canvas` in both
(`ldr.w fp,[r1]` at `0x12a1ec`; deref `f8db 0000` at `0x12a1f8/0x12a21c/
0x12a242`). `listEntryWidth` (`[sl,#340]`) is loaded at four sites; only the
case-8 first-`if` site (`0x129fd2`) differs (case 1/2 sites at `0x129d4a`
(`ip`) and `0x129d9c` (`r1`) and the case-8 second-`if` site at `0x12a026`
(`r2`) already match).

## 2. New evidence this wave: post-RA MIR (`-mllvm -print-machineinstrs`)

The `After Virtual Register Rewriter` dump pins the exact assignments.

**HEAD (28):**

```
bb.42  liveins: $r4,$r8,$r9,$r10
  $r6  = t2LDRi12 killed $r4, 0        ; %29  layout value  -> r6 (CSR, live across call)
  $r2  = t2LDRi12 $r10, 312            ; heapBufB
  $r3  = t2LDRi12 $r10, 480            ; field_0x1e0
  $r11 = t2LDRi12 killed $r1, 0        ; %304 &Globals::Canvas -> fp
  $r4  = t2LDRi12 killed $r0, 0        ; Globals::w
  $r5  = t2LDRi12 $r6, 40              ; buttonInsetX
  $r4  = t2SUBrr $r4, $r5
bb.44  liveins: $r6,$r8,$r9,$r10,$r11
  $r2  = t2LDRi12 %stack.0, 0          ; fresh &Globals::layout reload
  $r2  = t2LDRi12 killed $r2, 0        ; fresh layout value
  $r4  = t2LDRi12 killed $r6, 12       ; leftMargin  <- cached %29 (region C)
  $r5  = t2LDRi12 killed $r2, 32       ; top         <- fresh
```

**D (all-direct, 1892):**

```
bb.42  liveins: $r8,$r9,$r10,$r11
  $r11 = t2LDRi12 %stack.0, 0          ; &Globals::layout reload kept in fp
  $r2  = t2LDRi12 $r11, 0              ; layout value
  $r5  = t2LDRi12 killed $r1, 0        ; &Globals::Canvas -> r5
  $r2  = t2LDRi12 killed $r2, 40       ; buttonInsetX
bb.44  liveins: $r5,$r8,$r9,$r10,$r11
  $r2  = t2LDRi12 $r11, 0              ; layout value (reuses fp)
  $r4  = t2LDRi12 $r2, 12              ; leftMargin  <- fresh
  $r6  = t2LDRi12 killed $r2, 32       ; top         <- fresh
```

So the difference is exactly whether the single `%99` (`&Globals::layout`)
reload is kept in `fp` across both calls (D) or re-materialised per use site
(HEAD/original). In D the RA spends `fp` on `%99` and demotes
`&Globals::Canvas` to `r5`; in HEAD/original `fp` holds `&Globals::Canvas`
and each `%99` use reloads from `[sp,#16]`. The original needs the third
combination: `%29` dead **and** `%99` re-materialised **and** `fp =
&Globals::Canvas` — i.e. 8 `[sp,#16]` sites at 1896 B with D's bb.44 IR.

## 3. Case-block order sweep (new angle, ~120 builds)

The prior waves only tried adjacent label swaps. This wave swept the full
case-body order. `case` blocks were moved as whole source ranges (a
half-open-range reorder; note the earlier `reorder.py` in w13612's scratch
duplicated the next `case` label and mis-compiled).

Ordering is *not* free: several reorderings are codegen no-ops, others change
the literal-pool permutation, and some change the register allocation.

| variant | raw | aligned | size | pool == orig | region A |
|---|---:|---:|---:|---|---|
| HEAD order | **28** | **16** | 1896 | yes | `r4` |
| swap 9/11, 12/14, 3/17, 6/13, 17/default | 28 | 16 | 1896 | yes | `r4` |
| `case 1`/`case 2` label swap, `case 6`/`case 13` label swap | 28 | 16 | 1896 | yes | `r4` |
| swap 1/2 and 7 | 28 | 16 | 1896 | yes | `r4` |
| swap 1/2 and 9/12/14/613/3/17 | 35 | 20 | 1896 | yes | `r4` |
| swap 1/2 and 11 | 56 | 35 | 1896 | yes | `r4` |
| swap 1/2 and 4 | 39 | 24 | 1896 | yes | `r4` |
| swap 1/2 and 8 (`sw07`) | 61 | 36 | 1896 | no | **`ip`** |
| swap 7 and 8 (`sw17`) | 58 | 36 | 1896 | no | **`ip`** |
| swap 4 and 8 / 16 and 8 / 15 and 8 | 290/1280/958 | — | 1888-1892 | no | `ip` |
| move case 8 to position 0 (`8first`) | 54 | 32 | 1896 | no | **`ip`** |
| move case 8 to position 1/2 | 54 | 32 | 1896 | no | **`ip`** |
| move case 8 to position 3 | 56 | 34 | 1896 | no | **`ip`** |
| move case 8 to position 4 | 959 | 512 | 1892 | no | `r4` |
| move case 8 to position 5/6 | 49 | 31 | 1896 | no | `r4` |
| move case 8 to position 7..12 (= current or later) | 28 | 16 | 1896 | yes | `r4` |
| move case 15 to front (`16,15,...`) | 290 | 31 | 1892 | no | `r4` |

**Key result:** region A can be flipped to `ip` (matching the original) by
moving case 8 earlier in the switch — `8first`/`pos8_0..3` and `sw07`/`sw17`
all give `ip`. But *every* such order also permutes the literal pool at
`0x12a318..0x12a33a`. The original's pool is byte-identical to the HEAD
order's pool:

```
orig/HEAD: 96600e00 0c600e00 a0620e00 5a630e00 8e600e00 925f0e00
           785e0e00 7c610e00 28610e00
pos8_5   : 96600e00 0c600e00 7c610e00 28610e00 a0620e00 5a630e00
           8e600e00 925f0e00 785e0e00
```

No case order in the sweep (all 91 pairwise swaps + all 14 move-to-position
placements for case 8) produced **both** a matching literal pool and region A
= `ip`. So region A is a global-IR-order/vreg-numbering tie-break: it is
reachable, but only by a reorder that also breaks the (byte-exact) literal
pool, and the two constraints were not satisfiable together in this sweep.

## 4. Other measurements this wave (all reverted; 1896 B unless noted)

### 4.1 case 15 — full 64-cell grid (decl × c1 × c2top × c2lm × c3)

Each of the four `Globals::layout->field` reads is either direct (fresh) or
via the `Layout &layout` / `Layout *layout` local (cached). Result: the
whole grid collapses to the previously known buckets; the best is HEAD
(c1 direct, c2 top direct, c2 lm cached, c3 direct) at 28/16/1896.

| decl | c1 | c2 top | c2 lm | c3 | raw | aligned | size |
|---|---|---|---|---|---|---|---|
| ref | direct | direct | **cached** | direct | **28** | **16** | 1896 |
| ref | cached | direct | cached | direct | 28 | 16 | 1896 |
| ref | direct | **cached** | direct | direct | 29 | 15 | 1896 |
| ref | cached | cached | direct | direct | 29 | 15 | 1896 |
| ref | direct | direct | direct | direct (=D) | 362 | 76 | 1892 |
| ref | direct | direct | direct | cached | 322 | 74 | 1888 |
| ref | direct | direct | cached | cached | 291 | 74 | 1892 |
| ref | direct | cached | cached | direct | 308 | 73 | 1892 |
| ref | * | * | * | * (remaining 56 cells) | 28-362 | 15-76 | 1888-1896 |
| `Layout *layout` (same cells) | | | | | same as `ref` (pointer/reference identical) | | |
| unused ref / `(void) layout` / second ref | direct | direct | **cached** | direct | 28 | 16 | 1896 |

### 4.2 case 15 — c2 grouping/order (fresh unless noted)

| c2 expression | raw | aligned | size |
|---|---:|---:|---:|
| `top + (lm + height)` (D) | 362 | 76 | 1892 |
| `lm + top + height` | 362 | 76 | 1892 |
| `(top + lm) + height` | 362 | 76 | 1892 |
| `lm + (top + height)` | 362 | 76 | 1892 |
| `height + top + lm` | 369 | 111 | 1892 |
| `height + lm + top` | 370 | 111 | 1892 |
| `top + height + lm` | 380 | 87 | 1892 |
| `int lm = Globals::layout->field_0xc_leftMargin;` before the `if` | 320 | 33 | 1892 |
| `int right = Globals::w - Globals::layout->buttonInsetX;` + `right - width < x` | 362 | 76 | 1892 |
| same, `x > right - width` | 362 | 76 | 1892 |
| `Layout *lp = Globals::layout;` used for c2 lm | 28 | 16 | 1896 |
| `Layout &layout = *Globals::layout;` used for c1 inset only | 362 | 76 | 1892 |
| `Layout &layout` used for c3 inset only | 322 | 74 | 1888 |
| `Layout &layout` used for c1 + c3 | 322 | 74 | 1888 |

### 4.3 case 8 — region A (all keep 1896 B unless noted)

| variant | raw | aligned |
|---|---:|---:|
| HEAD | **28** | **16** |
| `&&`-operand permutations of the first `if` (8 orders) | 28-898 | 16-469 |
| comparison polarity flips (`A < x`→`x > A`, `y < A`→`A > y`, all four) | 28 | 16 |
| `layout->field_0xc_leftMargin` → `Globals::layout->...` in the 1st if | 28 | 16 |
| `layout->field_0x20_top` → `Globals::layout->...` in the 1st if | 28 | 16 |
| `buttonInsetX` init from `Globals::layout->buttonInsetX` | 28 | 16 |
| `+ 0` on `listTopY` / `buttonInsetX` inits | 28 | 16 |
| `*(&this->listEntryWidth)` | 28 | 16 |
| `(unsigned) this->listEntryWidth` | 29 | 17 |
| `int lw = this->listEntryWidth;` at top of case 8, used in 1st if only | 62 | 26 |
| same, used in 2nd if only | 34 | 18 |
| same, used in both ifs | 794 | 107 (1892) |
| `unsigned int` locals for `listTopY`/`buttonInsetX` | 34 | 22 |
| `long` locals | 28 | 16 |
| `int listTopY = ..., buttonInsetX = ...;` (same declaration) | 28 | 16 |
| swap the two local declarations | 30 | 18 |
| `int extra = this->listEntryHeight;` reused in 2nd if | 1289 | 171 (1892) |
| remove `layout = Globals::layout;` from the 1st-if body | 30 | 18 |
| remove `listTopY = this->listTopY;` | 816 | 135 (1892) |
| remove `buttonInsetX = Globals::layout->buttonInsetX;` | 806 | 143 (1892) |
| swap the two `if` statements | 858 | 141 (1892) |
| move the three `optBtn*`/`sliders[0]` calls before/between the ifs | 932/940 | 120/140 (1936/1940) |

The case-8 block never uses `r4` in the original and `ip`/`r4` are both free
at `0x129fd2` in HEAD, so this is a pure allocator tie-break. It is
**coupled to the global IR order** (see §3), not reachable from case-8 source
spelling alone.

## 5. Conclusion

Every shape still lands in one of two buckets:

* `%29` (cached `Globals::layout` value) live across the first call -> 1896 B,
  `fp = &Globals::Canvas`, `%99` re-materialised at all 8 sites; raw 28
  (HEAD) or 29 (`top` cached). Region C keeps the cached `leftMargin`.
* `%29` dead -> 1892 B (or smaller), `%99` kept in `fp`, `&Globals::Canvas`
  in `r5`; raw 291-362.

The original's combination (`%29` dead, `%99` re-materialised, `fp =
&Globals::Canvas`, 1896 B) remains unreachable by source spelling. The new
finding is that **region A is not independent of the case-block order**: it
flips to `ip` for any order that moves case 8 earlier, but every such order
also permutes the literal pool, so no order in the sweep satisfied both.
Region A and region C therefore each need a different global-IR-order
perturbation, and the two have not been found together.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13652
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# raw=28 aligned=16 size=1896/1896 insn=754/754
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13652` (`harness.py`,
`sw.py` + `v1..v9.py` variant generators, `reorder2.py`, `search2.py`,
`chk.py`, `probe.py`, `mi_all.txt`/`mi_D.txt` full `-print-machineinstrs`
dumps, `greedy_fn.txt`/`vrewrite_fn.txt`/`vrewrite_D.txt` extracted
post-RA MIR, `RegAllocGreedy70.cpp`).
