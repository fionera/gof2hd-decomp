# SpaceLounge::drawLounge — wave-122 pad-weight / draw-call shape re-check (w7217)

Status: research note (2026-11-06). Worktree `wt/w7217`, base `29cd275f6`.
**No `src/` change landed**: the committed HEAD is still the best measured shape.
The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 728/729 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.93 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior work: `spacelounge-drawlounge-wave121-w7117.md` (RA order),
`-wave119-w6953.md` (direct-field / boxW), `-wave119-w6927.md` (x split),
`-wave119-w6744.md` (eviction order), `-wave117-w6517.md` (292-variant sweep),
`-wave113-w6117.md` (exhaustive source-shape sweep). This note supersedes
nothing.

## 1. Residual (re-verified, unchanged)

Only the clamp / join `0x1712b2..0x171330` differs (plus the tail registers
and EH selector offsets that follow from it):

```text
value        original        HEAD
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
top          r2 -> r8        r2 -> r8
agent        r8 (spilled)    r8 (spilled)
```

Exact aligned diff (HEAD vs original) has ~20 replace blocks, all in
`0x1712b2..0x171330` plus tail EH selector offsets.

## 2. New measurement: clamp-region byte length

`vmov X,s18` -> first `bl` (String ctor):

* **original: 62 B** (0x1712b2..0x1712f0)
* **HEAD: 74 B** (0x1712b2..0x1712fc)
* **faithful (`width + pad*2`, no `pad3`): 72 B**

So the residual is not only RA: our clamp is 10-12 B longer. The extra bytes
are `str.w r8,[sp,#12]` (4 B, misplaced -- the original has it *inside* the
join at 0x171300), `mov r5,fp` (2 B), and `subgt.w r9` (4 B) vs the original
`subgt r5` (2 B).

Original clamp/join (authoritative, objdump):

```text
1712b2: vmov r5, s18
1712bc: sub.w r1, r5, r9          ; x - pad
1712c4: add.w r5, r0, r9          ; x = inset + pad
1712d4: add.w r1, r9, r5          ; pad + x
1712d8: add r1, r6                ; + width
1712dc: itt gt
1712de: addgt.w r1, r6, sl        ; width + pad2   (8-byte arm)
1712e2: subgt r5, r0, r1          ; x = right - ...
1712e4: ldr r1,[pc,#476]; ldr.w fp,[r4,#48]
1712f4: vmov r2, s16
1712f8: mov r3, r5                ; x copy #1
1712fa: add sl, r6                ; boxW
1712fc: mov r1, r9                ; pad
1712fe: subs r6, r3, r1           ; x - pad
171300: str.w r8,[sp,#12]         ; spill agent
171304: sub.w r5, r2, r9          ; top - pad
171308: mov r9, r3                ; x copy #2
17130a: mov r8, r2
17130c: strd sl, fp, [sp]
```

HEAD instead: `ittt gt; movgt.w r1,fp,lsl#16; addgt.w r1,r6,r1,asr#15;
subgt.w r9,r0,r1`, `str.w r8,[sp,#12]` before the String ctor, `mov r5,fp`,
and no `mov r3,r5`/`mov r9,r3`.

## 3. Root cause (confirmed with instrumented LLVM 7.0.2)

Scratch compiler: `/tmp/llvm_android_702b/build2/bin/clang++` with
`RA_TRACE` / `VRW` / `HINTVREG` / `PRIOVREG` / `NOEVICT` / `NOHINT` hooks.

RA order (HEAD, `RA_TRACE=1`):

1. `%608` (x, hint `$r3`, w=2.946e-4) -> `$r5`
2. `%47` (boxW) -> `$r6`
3. `%46` (top-pad, hint `$r3 $r2`, w=6.251e-4) evicts x from `$r5` -> `$r5`
4. `%608` re-selects -> `$r9` (whole)
5. `%2` (pad, **unhinted**, w=2.159e-4) -> `$r11` (fp)

The original needs **pad -> r9 before step 3**, so x splits `r5 -> r3 -> r9`.
`%2` is a plain `t2LDRi12` with no COPY => unhinted => selected after every
`0xE`-priority interval. Only the drawBox/DrawRectangle direct-field shapes
reach pad=r9/x=r5, but they rotate other callee-saved values.

### Scratch knobs (diagnostic only, not source-expressible)

| knob | result |
|---|---|
| `VRW=2147483650:1.5..2.0` on faithful | **93.617**, 1944 B, **pad=r9, x=r5** |
| `VRW=…:2.2` | 91.695, 1936 B, pad=r6, x=r5 |
| `VRW=…:2.5..3.0` | 84.283, 1948 B, pad=fp, x=r5 |
| `HINTVREG=…:75` (pad->r9) | 78.99, 1980 B, pad=r5, x=sl |
| `HINTVREG=…:76/77` (pad->r10/r11) | 93.34 / 94.03, 1948 B, pad=r8, x=r9 |
| `NOHINT=608` | 95.676 (unchanged) |

`VRW=2` reaches the target registers but the join loses the split copies and
the `str.w r8,[sp,#12]` placement: top lands in `r1` (want `r2`), lineHeight
in `r8` (want `fp`), agent on the stack (want `r8`), 1944 B (4 B short).
No VRW value in `1.5..3.0` gives the exact original.

## 4. Variants measured this session (~90, all <= 95.6760)

Harness: `/dev/shm/w7217/harness.py` (single-TU NDK r18b compile, pad/x/insn
readout).

| family | result |
|---|---|
| **HEAD** | **95.6760**, 1948 B, 728/729, pad=fp, x=r9 |
| `int padB = pad` used in box / clamp / DrawRectangle | 95.676 (coalesced) |
| `int boxX/boxY` locals, drawBox only / DrawRect only / both | 95.676 (coalesced) |
| named `pad2 = pad*2` used everywhere | 95.676 |
| `top` via `float topF` / `double` round-trip | 95.676 |
| `pad * 2` vs `pad + pad` vs `pad << 1` (mixed forms) | 95.676 |
| `const int &pad` | 76.733, 1972 B, x=r5 but whole function rotates |
| `int pad = *(&...)` / pointer / `pad+0` / `(int)` cast | 95.676 |
| `unsigned pad` | 95.4015 |
| `long pad` | 95.676 |
| pad load moved after SetColor / after centerX / after top | 93.068 |
| named `String empty("")` before drawBox | 73.58, 1992 B (adds dtor) |
| `drawBox`/`lineHeight` via `Globals::layout` | 95.676 |
| `centerX` used after join (`xr + (centerX-centerX)`, `if(centerX==x){}`) | 95.676 (folded) |
| uninit `x` + 3-way `else x = centerX` | 95.676 |
| `x` declared before name construction | 95.676 |
| `DrawRect` x-pad direct field | 92.784, pad=sl, x=r9 |
| `DrawRect` both x-pad/top-pad direct field | **91.684**, 1948 B, pad=r9, x=r5 |
| `DrawRect` boxW direct field | 72.192, pad=r6, x=r2 |
| `drawBox` top-pad / x-pad direct field | 81.51 / 81.89 |

The `DrawRect both direct field` shape is the closest to the target registers
(pad=r9, x=r5) but it flips `Globals::Canvas` from `sl` to `fp` for the whole
function (the conversation branch), and recomputes x-pad/top-pad directly into
the call argument registers (no split copies), so it scores 91.684.

## 5. Why every other shape fails

`pad` has no COPY in the MIR (`%2:rgpr = t2LDRi12 ...`), so it is a plain
unhinted `0xA` interval. The greedy allocator selects all hinted `0xE`
intervals first; `%608` (x, hint `$r3`, size 4220) is selected before
`pad` (size 6384) and, after `%46` evicts it from `r5`, moves whole to the
free `r9`. Every source-level way of adding pad uses is folded (`pad+pad`,
`pad*2`, `pad^pad`, `pad-pad`, `pad|0`, `pad*3-pad`, ...), so pad's weight
stays 2.159e-4 and its split still prefers `r11` (0.469) over `r9` (1.113).

Even when pad reaches `r9` (scratch `VRW`), `%46` evicts it because pad
(2.159e-4) is cheaper than x (2.946e-4); the original must have pad's weight
above x's, which no code-neutral source expression achieves.

## 6. Conclusion

The committed HEAD (`short pad3`, **95.67604667124228 %**, 1948 B) remains the
best measured shape. The residual is a single greedy-RA split-choice:
`pad` is unhinted and is allocated after `x`, so it can never take `r9`
before `x`'s re-selection unless its weight is inflated or its live range is
shortened; both alternatives rotate other callee-saved values and score lower.
No `src/` change landed.

## 7. Structural state (unchanged, re-verified)

`check` = RATCHET PASS: byte 4506 / linked 4506, avg 99.93 %, data 843/843,
imports 207, lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact.
