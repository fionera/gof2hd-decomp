# `Gun::calcCharacterCollision` + `SpaceLounge::drawLounge` wave 186 (w13020):
# both residuals remain RA/hoist plateaus; no size-exact raw win

Worker: w13020. Branch `wave186-w13020`, base master `2d02567f6`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w13020`.

## 0. Result

**No `src/` change landed.**  Every size-exact shape measured this wave is
equal to or worse than the committed body.  The tree is clean apart from
this note.

| symbol | raw | aligned | size | pct |
|---|---:|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | **458** | 63 | 1984/1984 | 93.4 |
| `_ZN11SpaceLounge10drawLoungeEv` | **175** | 41 | 1948/1948 | 95.7 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, `.rel.plt` byte-identical, 27/27 headers, file size 2,187,328
exact.  `.text` diff 74,402, `.ARM.exidx` 0, `.ARM.extab` diff 647
(size/base exact at `0x1a3b78`, 156,312 B), `.rodata` 0, build-id 20,
total 75,069.

## 1. `Gun::calcCharacterCollision` — 458/63/1984

### Root cause, now confirmed in machine IR

`-mllvm -print-machineinstrs` on `Gun.cpp` shows **Early Machine Loop
Invariant Code Motion** hoisting the address of the scatter-block
`playerPosition` alloca out of the `i` loop into the function entry block:

```
# After Remove dead machine instructions (pre-LICM), bb.18:
  %114:gprnopc = t2ADDri %stack.10, 0   ; &playerPosition
  $r2 = COPY %114                        ; operator-(enemyPosition, playerPosition)

# After Early Machine Loop Invariant Code Motion, entry block:
  %308:gprnopc = t2ADDri %stack.10, 0   ; hoisted
```

The greedy allocator then keeps `%308` live across the whole `i` loop body
in a callee-saved register, which is why `i*3` (`add.w r4/r5, fp, fp, lsl#1`)
lands in `r5` instead of the shipped `r4`.  The shipped body never hoists
the address: it rematerialises `add r4, sp, #104` at each use site, leaving
`r4` free for `i*3` and emitting the 4-byte `ldr.w fp, [sp, #40]` (radius/i
reload) where we emit the 2-byte `add r4, sp, #104`.  That 2-byte
substitution shifts the collision-check block (function offsets `0x223`,
`0x2c2`) and the `r4`/`r5` swap cascades through the pp/delta slots, the
view block (`0x43e`) and the rotated `k` loop.  The `k` loop itself is also
laid out differently (ours tests at the top, the shipped rotates the test
to the bottom), but that is downstream of the same register pressure.

### New sweeps this wave (all full link + `tools/fn_diff.py`, `-j2`)

| variant | raw | size |
|---|---:|---:|
| committed (`stmt` shape, named `playerPosition`) | **458** | 1984 |
| `expr` inline (`VectorLength(enemyPosition - getPlayer()->getPosition())`) | 462 | 1984 |
| `Vector playerPosition(<temporary>)` copy-ctor | 458 | 1984 |
| named `Vector delta = enemyPosition - playerPosition;` | 458 | 1984 |
| `playerPosition` decl + assign split | 1822 | 2000 |
| `playerPosition` declared at `e`-loop body top | 1770 | 1964 |
| `playerPosition` declared at function top | 1797 | 1960 |
| `k`-loop `Vector ep(enemy->getPosition()); *td = ep; *td -= position;` | 458 | 1984 |
| `k`-loop `Vector ep; ep = ...;` | 1768 | 1972 |
| `Matrix view` assign instead of copy-init | 1888 | 2056 |
| `Matrix view` in explicit `{ }` scope | 458 | 1984 |
| `const Vector &position/velocity` | 1797 | 1920 |
| `enemyPosition` declared before `position`/`velocity` | 541 | 1984 |
| swapped subtraction operands (`playerPosition - enemyPosition`) | 460 | 1984 |

No shape that removes the LICM hoist without changing the frame keeps size
1984; every named-`playerPosition`/`view` relocation either grows the frame
or re-hoists the same `%stack.10`.  The wave-185 note's conclusion stands.

## 2. `SpaceLounge::drawLounge` — 175/41/1948

### Root cause, now confirmed in machine IR

`-mllvm -print-machineinstrs` on `SpaceLounge.cpp` gives the post-RA
assignment at the right-edge clamp (`bb.39`):

```
liveins: $r6, $r8, $r10, $r11, $s16, $s18     ; r5 is FREE
$r9  = COPY killed $s18                        ; x   -> r9
$r1  = t2SUBrr $r9, $r11                       ; x - pad
...
bb.40: $r5 = COPY $r11                         ; pad copied to r5
bb.41: $r5 = COPY $r11                         ; pad copied to r5
bb.42: $r9 = t2SUBrr $r0, $r1                  ; x = (w-inset) - (width+pad2)
bb.43: $r11 = t2LDRi12 $r4, 48                 ; fp reused for lineHeight
bb.44: $r5 = t2SUBrr $r2, $r1                  ; top - pad
```

`pad` is loaded once at function offset `0x189` into `$r11` (`fp`) and `x`
takes `$r9`; the shipped is the opposite colour (`pad`=`r9`, `x`=`r5`).
Because `fp` is needed for `lineHeight` after the clamp, the allocator
inserts `mov r5, fp` in *both* clamp branches (`$r5 = COPY $r11`), which is
the 2-byte growth that shifts the whole downstream block and the exception
landing pad.  The shipped has no such copy: `sub.w r5, r2, r9` uses `pad`
in `r9` directly.

The clamp *shape* is independently fixable: writing
`+ (-width - (pad + pad))` produces exactly the shipped IR

```
%174 = add i32 %width, %pad2
%175 = sub i32 %(w-inset), %174
```

and the shipped 2-instruction clamp `addgt.w r1, r6, sl; subgt r5, r0, r1`.
But `pad` still lands in `fp`, so the two `mov r5, fp` copies remain and the
raw score is 445 (the copy is a net loss of 4 B against a 6 B clamp saving,
and the block re-shifts).  No source-level knob found this wave changes
`pad`'s colour.

### New sweeps this wave (all full link + `tools/fn_diff.py`, `-j2`)

| variant | raw | pad reg | size |
|---|---:|---|---:|
| committed (flat `+ (-width - pad - pad)`) | **175** | fp | 1948 |
| `- (width + pad + pad)` | 176 | fp | 1948 |
| `- width - pad - pad` (flat minus) | 468 | fp | 1944 |
| `+ (-width - (pad + pad))` (2-insn clamp) | 445 | fp | 1948 |
| `+ (-(pad + pad) - width)` | 445 | fp | 1948 |
| `- (width + (pad + pad))` | 445 | fp | 1948 |
| `- width - (pad + pad)` | 445 | fp | 1948 |
| `- ((pad + pad) + width)` | 445 | fp | 1948 |
| `- width - 2 * pad` | 445 | fp | 1948 |
| `+ (-width - pad * 2)` | 445 | fp | 1948 |
| local `int pad2 = pad + pad;` | 445 | fp | 1948 |
| 6 condition-operand permutations with the 2-insn clamp | 445 | fp | 1948 |
| `int`/`long`/`int32_t x` | 445 | fp | 1948 |
| `unsigned int x` | 446 | fp | 1948 |
| `short x` | 543 | fp | 1952 |
| `int x(centerX)` / `int x{centerX}` / `auto x` | 175 | fp | 1948 |
| `int x = (int) centerX` / `centerX + 0` / split decl+assign | 175 | fp | 1948 |
| `const int centerX` | 175 | fp | 1948 |
| `x` declared after `centerX` | 445 | fp | 1948 |
| `x` declared after `pad` / after `SetColor` | build fail | — | — |
| `centerX` in place (no `x` local) | 175 | fp | 1948 |
| `register`/`const`/`(int)`-cast `pad` | 175 | fp | 1948 |
| `unsigned pad` | 177 | fp | 1948 |
| `Layout *layout = Globals::layout;` moved above `pad` | 1860 | r6 | 1972 |
| 24 permutations of `{pad, SetColor, centerX, top}` | 175 (pad first) / 254 (SetColor first) | fp | 1948 |
| inline `layout->field_0x30_rowHeight` (base clamp) | 175 | fp | 1948 |
| inline `layout->field_0x30_rowHeight` (2-insn clamp) | 445 | fp | 1948 |
| `lineHeight` hoisted before the clamp (both clamps) | 175 / 445 | fp | 1948 |

`pad` is `fp` in **every** size-1948 variant measured (~70 this wave, ~200
across waves 185–186).  The colour is a global greedy-RA decision driven by
the whole (huge) function, not by the mode-0 declaration order, the clamp
association, or the `x` spelling.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13020
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv _ZN11SpaceLounge10drawLoungeEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=458/175`, `aligned=63/41`, sizes `1984/1948`,
`RATCHET PASS`, byte 4507 / linked 4507, data 843/843, imports 207,
lint CLEAN, drift 0.

Machine-IR evidence (scratch, not tracked):

```sh
# Gun: shows Early Machine LICM hoisting %308 = t2ADDri %stack.10, 0
clang++ <Gun.cpp flags> -mllvm -print-machineinstrs > gun_mi.txt
# SpaceLounge: "After Virtual Register Rewriter" shows $r11 = pad, $r9 = x
clang++ <SpaceLounge.cpp flags> -mllvm -print-machineinstrs > sl_mi.txt
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13020/`
(`runs.py`, `sweep*.py`, `gsweep*.py`, `gun_mi.txt`, `sl_mi.txt`,
`sl_vrr.txt`, `sl_vrr_nw.txt`, `gun_before_licm.txt`, `gun_after_licm.txt`).

## 4. Files changed

`_work/reconstructions/gun-spacelounge-wave186-w13020.md` only.
