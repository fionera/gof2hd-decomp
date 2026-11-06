# SpaceLounge::drawLounge — wave-121 pad/x allocation re-check (w7117)

Status: research note (2026-11-06). Worktree `wt/w7117`, base `bd061a489`.
**No `src/` change landed**: the committed HEAD is still the best measured shape.
The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4505 / linked 4505, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0,
  symbol parity unchanged (27 sections, file size 2,187,328 B exact)

Prior work: `spacelounge-drawlounge-wave119-w6953.md` (best summary),
`-w6941.md`, `-w6927.md`, `-w6744.md`, `-wave118-w6616.md`,
`-wave117-w6517.md`, `-wave113-w6117.md`, `-w103-oracle.md`.
This note supersedes nothing.

## 1. Residual (unchanged)

Only the clamp / join `0x1712b2..0x171330` differs:

```text
value        original        HEAD
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

The committed `short pad3` right arm is a scoring workaround: it reproduces
the original 12-byte arm `ittt gt; movgt.w r1,pad,lsl #16; addgt.w
r1,width,r1,asr #15; subgt r5,r0,r1` but with `pad` in `fp`.  The faithful
`width + pad * 2` arm is the original 8-byte `itt gt; addgt.w r1,r6,sl;
subgt r5,r0,r1`, but without the x-split its join is 4 B short and scores
**94.0** (measured again this wave, 729/729 insns).

## 2. Exact stock-RA trace (new this wave)

Final pass interval list (`regalloc.log`, drawLounge final pass):

```text
%2   pad        [496r,6064B:0)[6752B,7568r:0)  w=2.159249e-04  unhinted
%608 x          [6784r,6896B:0)[6912r,6976B:2)
                [6976B,7168B:0)[7216r,7264B:1)[7264B,11072r:3)
                                               w=2.946140e-04  hint $r3
%44  lineHeight [7312r,7696r:0)               w=5.978897e-04  hinted
%45  x-pad      [7568r,8272r:0)               w=6.432506e-04  hinted
%46  top-pad    [7552r,8288r:0)               w=6.251309e-04  hinted
%47  boxW       [7536r,8304r:0)               w=6.080040e-04  hinted
```

Allocation order (same log):

```text
selectOrSplit %608 (x)  hints: $r3 -> missed -> assigning %608 to $r5
selectOrSplit %47  (boxW) -> $r6
selectOrSplit %46  (top-pad) hints $r3 $r2 -> missed
    $r9 is available at cost 1
    should evict: %608 ... weight 2.946140e-04
    evicting $r5 -> unassigning %608 from $r5 -> assigning %46 to $r5
selectOrSplit %608 (x)  hints: $r3 -> missed
    should evict: %53 ... weight 2.882066e-04
    assigning %608 to $r9          <-- x takes r9 whole
...
selectOrSplit %2 (pad, unhinted) -> evicts %365 -> assigning %2 to $r11
selectOrSplit %44 (lineHeight) -> evicts %2 from $r11 -> %44 to $r11
selectOrSplit %2 (round 2) -> RS_Split
    $r5  total = 0.6445256243
    $r11 total = 0.4687455428      <-- chosen
    $r9  total = 1.113270972       (x occupies r9)
    Split for $r11 in 15 bundles
```

So `pad` is an unhinted `0xA0000000+size` interval and is selected after every
hinted interval; by then `x` already holds `r9`, and the split analysis prices
`r9` at 1.113 (vs `r11` 0.469).  The original needs `pad -> r9` **before**
`%46` evicts `x` from `r5`, so that `x` splits `r5 -> r3 -> r9` instead of
moving whole to `r9`.

## 3. Variants measured this wave (~80, all <= 95.676047)

Harness: `fast2.compile_and_check` (single-TU NDK r18b compile, pad/x/insn
readout) plus full `rebuild.sh` + official `verify.py` score for the only
pad=r9/x=r5 hit.

| family | result |
|---|---|
| HEAD (`short pad3`) | **fp/r9**, 728 insns, **95.676047 %** |
| all 12 valid source orders of `{pad,SetColor,centerX,top}` | fp/r9 (or fp/r9 with a different pad base); no change |
| `Layout *layout` hoisted above `pad` / `pad` via `layout->field_0x94` | x=sl, 732 insns, worse |
| `int pad2 = pad*2` / `pad+pad` / `pad<<1` / copy+`*=2` / copy+`+=` | fp/r9, 727-729 insns |
| exact oracle shape (`pad2` real local, `centerX` in place, cond `Globals::w - inset < x + pad + width`, rhs `(w-inset)-(width+pad2)`) | fp/r9, 729 insns (94.0 %) |
| `__builtin_expect` on either/both clamp conditions (18 combos) | only `(unlikely, likely)` + `pad3` moves pad to **r5** (730 insns); no pad=r9/x=r5 |
| `pad3` cast forms: `(short)pad*2` inline, `int pad3=(short)pad`, `static_cast`, `&0xffff`, `pad3+pad3`, `pad3<<1` | fp/r9, 727-730 insns |
| `pad3` in condition / left arm / `top` | fp/r9, 727-729 insns |
| `int drawX = x;` before/after clamp, `x+0`, `x*1`, `x|0`, `x&-1`, `long`, `unsigned` | coalesced, fp/r9, 728 insns |
| `centerX` modified in place (no `int x`) | fp/r9, 728-729 insns |
| weight-boost expressions `pad2+(pad-pad)`, `+(pad^pad)`, `+(pad&0)`, `+(pad*0)`, `(pad2+pad)-pad`, ... | folded, fp/r9, 729 insns |
| **`drawBox`/`DrawRectangle` use `pad3` (short) for x/y** | **pad=r9, x=r5**, but 725 insns, 1960 B, **81.8 %** |

The last row is the only source shape found this wave that reaches
`pad=r9` + `x=r5` with the 16-bit sign-extend arm, but it shortens pad's live
range: the join becomes `sxth.w r1,r9` + `subs r2,r5,r1` + `sub.w r3,sl,r1`
with `top` in `sl`, and the original's `mov r3,r5` / `mov r9,r3` split copies
disappear (725 insns, 81.8 %).  It is therefore not a win.

## 4. Why every other shape fails

`pad` has no COPY in the MIR (`%2:rgpr = t2LDRi12 ...`), so it is a plain
unhinted `0xA` interval.  The greedy allocator selects all hinted `0xE`
intervals first; `%608` (x, hint `$r3`, size 4220) is selected before `pad`
(size 6384) and, after `%46` evicts it from `r5`, moves whole to the free `r9`.
Every source-level way of adding pad uses is folded (`pad+pad`, `pad*2`,
`pad^pad`, ...), so pad's weight stays 2.159e-4 and its split still prefers
`r11` (0.469) over `r9` (1.113).  Only two non-source mechanisms reach the
target structure:

* `-w6927-boostpad` (scratch compiler, weight x2): pad=r9, x=r5 split, but
  `lineHeight -> r8` and `top -> fp` (r8/r11 swapped), **95.1 %**;
* shortening pad's live range (`drawBox` with `pad3`): pad=r9, x=r5, but the
  join loses the split copies (**81.8 %**).

## 5. Conclusion

The committed HEAD (`short pad3`, **95.67604667124228 %**, 1948 B) remains the
best measured shape.  The residual is a single greedy-RA split-choice:
`pad` is unhinted and is allocated after `x`, so it can never take `r9`
before `x`'s re-selection unless its weight is inflated or its live range is
shortened; both alternatives rotate other callee-saved values and score lower.
No `src/` change landed.

## 6. Structural state (unchanged, re-verified)

`check` = RATCHET PASS: byte 4505 / linked 4505, avg 99.94 %, data 843/843,
imports 207, lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact.
