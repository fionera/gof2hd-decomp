# SpaceLounge::drawLounge — wave-119 hint / literal-pool study (w6714)

Status: research note (2026-11-05). Worktree `wt/w6714`, branch
`wave119-w6714`, base `47a1396bd`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4503 / linked 4503, data 843/843, imports 207,
  lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave118-w6616.md` (the scratch-compiler
knobs and the `pad -> r9` + 8-byte-arm reproduction),
`-wave117-w6517.md` (the 292-variant sweep and the `boxW` IR-order finding),
`-wave116-w6417.md`, `-wave115-w6318.md`, `-wave114-w6218.md`,
`-wave113-w6117.md`, `-wave112-w6015.md`, `-wave110-w5818.md`. This note
supersedes nothing.

## 1. The residual is still only the clamp / join

Every instruction outside `0x1712b2..0x171330` is byte-identical. Only `pad`
and `x` differ:

```text
value        original        HEAD (committed)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole interval)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad2 (sl)    r10             r10
lineHeight   fp (r11)        fp (r11)
```

## 2. New result this wave: the literal-pool word is already correct

The only differing pool word in the whole 1948-byte body is at `0x1714c4`:

```text
orig  1714c4: 84 91 05 00   -> 0x00059184
ours  1714c4: 78 91 05 00   -> 0x00059178
```

This is the `pc`-relative offset used by `ldr r1,[pc,#476]` (orig
`0x1712e4`, ours `0x1712f0`) for the `String("")` empty literal. Resolving
both against their own instruction addresses gives the **same** `.rodata`
target in both binaries:

```text
orig  (0x1712e4 + 4) + 0x59184 = 0x1ca46c
ours  (0x1712f0 + 4) + 0x59178 = 0x1ca46c
```

So the pool word is not a separate `.rodata`/string-layout defect: it differs
only because the loading instruction is 12 B later. It will resolve by itself
when the clamp/join matches. No `.rodata` change is needed.

## 3. Why `pad` is in `fp`: it has no allocation hint

Re-running the `-mllvm -debug-only=regalloc` trace on the committed HEAD
confirms and sharpens wave-118's facts:

* `pad` = `%2`, size 6384, enqueued at `0xA0000000 + 6384` (no `1<<30` hint
  bit), selected at `RS_Assign Cascade 0`, evicts `%365` from `r11` (`fp`).
* `x` = `%608`, size 4220, enqueued at `0xE0000000 + 4220` (hint bit set),
  selected long before `pad`, takes `r5`, is evicted by `%46`
  (`top - pad`, weight 6.251309e-04), is re-queued and immediately re-selected
  into the still-free `r9`.
* `r9` is taken by **no other interval** before `pad` is selected. If `pad`
  were popped before `x`, `r9` would be free and `pad` would take it.

`x`'s hint is `$r3`, produced by `CalcSpillWeights`' `copyHint()` from the
first `DrawString` argument copy `$r3 = COPY %43` (the join copy `%43 = COPY
%608` is coalesced into `%608`, so the hint lands on `%608`). `pad` has **no
COPY at all** in the IR (only `t2LSUBrr` / `t2ADDrr` / `t2LSLri` /
`vcvt`-operand uses), so it cannot acquire a hint from source in the obvious
way. `pad` therefore needs either (a) a physical-register COPY in the IR that
does not change the generated code, or (b) `x` to lose its hint without
disturbing the rest of the allocation. Neither has been found.

## 4. The `boxW` shape re-examined (pad -> r9, but top spills)

The wave-116/117 `boxW` shape

```cpp
int boxW = width + pad * 2;
int lineHeight = layout->field_0x30_rowHeight;
layout->drawBox(2, x - pad, top - pad, boxW, lineHeight, String(""));
```

does give `pad -> r9` and `x -> r5` from stock clang. A fresh trace shows why
it still loses:

* `pad` `%2` is size 6352 (not 6384) and is re-enqueued at `RS_Split`
  (`prio = 6352`); `splitAroundRegion` then picks `r9`.
* `pad2` `%381` gets `fp` (`r11`) instead of `sl` (`r10`) because the
  stack-guard GOT pointer (`%616`, live `[48,6064)`) occupies `r10` across
  `pad2`'s whole early range in this IR.
* `boxW` `%487` gets `r6`, `lineHeight` gets `r8`, and `top` has no
  callee-saved register left: it is spilled to `[sp,#20]`, the frame grows
  120 -> 128, and every `sp`-relative access shifts by 8 -> **72.6 %**.

The original instead reuses `sl` in place (`add sl, r6` at `0x1712fa`), so
`pad2`/`boxW` need one register, `r11` is free for `lineHeight`, and `r8` is
free for `top` after the agent spill. Every attempt to make `boxW` reuse
`pad2`'s register from source failed (see 5).

## 5. Measurements this wave

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6` (~1.1 s), then the official
`verify.py` internals. All sizes are ours (orig is 1948).

### 5.1 Source variants on the faithful base

| variant | raw % | size | note |
|---|---:|---:|---|
| faithful `width + pad * 2` | 93.964335 | 1948 | pad fp, x r9 |
| `pad2 = pad * 2` local (before SetColor / at top / all uses) | 93.964335 | 1948 | coalesced |
| `padc = pad` copy (clamp / top / all) | 93.964335 | 1948 | coalesced |
| `const int pad`, `const int pad2` | 93.964335 | 1948 | — |
| `boxX = x - pad`, `boxY = top - pad` (after clamp) | 93.964335 | 1948 | — |
| `boxY` only | 92.307692 | 1944 | — |
| `drawX = x` used in both DrawStrings | 93.964335 | 1948 | coalesced |
| `x2 = x` used in both DrawStrings | 93.964335 | 1948 | coalesced |
| `boxW = width + pad*2` after clamp | 72.602740 | 1968 | pad r9, x r5, top spilled |
| `boxW = pad*2 + width` | 72.602740 | 1968 | same |
| `boxW = width + pad + pad` | 72.752231 | 1960 | same |
| `boxW = (pad<<1) + width` / `width + (pad<<1)` | 72.8 | 1960 | same |
| `boxW = width + 2*pad` | 72.602740 | 1968 | same |
| `pad2` local + `boxW = width + pad2` | 72.602740 | 1968 | same |
| `pad2` local + `pad2 += width` in place | 72.602740 | 1968 | same |
| `boxW` after `lineHeight` | 72.602740 | 1968 | same |
| `boxW` before clamp | 89.759450 | 1940 | pad fp |
| `boxW` declared but unused | 95.676047 | 1948 | DCE, identical to HEAD |
| `boxW` before clamp + pad3 arm | 95.676047 | 1948 | identical to HEAD |

### 5.2 Source variants on the committed HEAD (`short pad3`)

| variant | raw % | size | pad | note |
|---|---:|---:|---|---|
| HEAD (`short pad3`) | **95.676047** | 1948 | fp | best |
| `short pad3 = (short) pad` | 95.676047 | 1948 | fp | identical |
| `width + 2 * pad3` | 95.676047 | 1948 | fp | identical |
| `width + (pad3 << 1)` | 95.676047 | 1948 | fp | identical |
| `unsigned short pad3` | 91.7 | 1956 | — | — |
| `width + pad3 + pad3` | 92.4 | 1952 | — | — |
| `pad3` in the else-if condition | 95.401510 | 1948 | fp | — |
| `pad3` in `x - pad` | 90.9 | 1956 | — | — |
| `pad3` in `top - pad` | 91.2 | 1952 | — | — |
| `pad3` in drawBox width | 71.8 | 1972 | — | — |
| `pad3` in DrawRectangle width | 71.8 | 1980 | — | — |
| `pad3` in both widths | 72.2 | 1968 | — | — |
| `pad3` from `Globals::layout->field_0x94` | 92.4 | 1952 | — | — |
| `top` from `Globals::layout->field_0x94 + ...` | 79.9 | 1972 | — | — |
| `int x; x = centerX;` | 95.676047 | 1948 | fp | identical |
| `pad3` declared after `int x` | 95.676047 | 1948 | fp | identical |
| `pad3` declared before `top` | 95.676047 | 1948 | fp | identical |
| `xm = x - pad` local for drawBox | 95.676047 | 1948 | fp | identical |
| `ym = top - pad` local for drawBox | 93.1 | 1948 | — | — |
| `x` initialised in if/else (3-way phi) | 95.676047 | 1948 | fp | identical |
| `x` initialised in if/else, `centerX` in conditions | 95.676047 | 1948 | fp | identical |
| else-if commuted (`x + width + pad`, `width + pad + x`, `x + pad + width`) | 95.676047 | 1948 | fp | identical |
| `w - inset - width - pad3*2` | 95.676047 | 1948 | fp | identical |
| `w - (inset + width + pad3*2)` | 91.7 | 1952 | — | — |
| `inset = layout->buttonInsetX` cached | 95.676047 | 1948 | fp | identical |
| `pad` declared just before `top` / after `centerX` | 93.1 | 1948 | — | — |
| inline identity helper `pad = id(field_0x94)` | 95.676047 | 1948 | fp | coalesced |

### 5.3 Scratch-compiler runs (not landable)

`-mllvm` knobs on `/tmp/llvm_android_build/bin/clang++` (`patched2.sh`).

| source | flags | raw % | size | note |
|---|---|---:|---:|---|
| HEAD | `-w6616-hintpad` | 78.3 | 1972 | pad r8, x sl |
| HEAD | `-w6616-hintpad -w6616-boostx` | 92.0 | 1948 | — |
| HEAD | `-w6616-boostx` | 81.4 | 1968 | — |
| HEAD | `-w6616-forcepad9 -w6616-boostx -w6616-nohint46` | 83.7 | 1948 | — |
| HEAD | `-w6616-hintpad -w6616-nohintx` | 91.8 | 1948 | pad r8, x fp |
| faithful | (none) | 93.964335 | 1948 | pad fp, x r9 |
| faithful | `-w6616-forcepad9` | 83.3 | 1960 | pad r5, x sl |
| faithful | `-w6616-forcepad9 -w6616-lock46` | 83.3 | 1960 | same |
| faithful | `-w6616-forcepad9 -w6616-nohint46` | 69.8 | 1960 | — |
| faithful | `-w6616-forcepad9 -w6616-boostx` | 81.4 | 1944 | pad r9, x r5, no split |
| faithful | `-w6616-lock -w6616-lock46 -w6616-boostx` | 80.8 | 1972 | — |
| faithful | `-w6616-boostx -w6616-forcepad9 -w6616-nohint46` | 93.617021 | 1944 | exact clamp + 8 B arm, join 4 B short (wave 118) |

The `forcepad9`/`lock` knobs are unreliable because their size windows
(6300..6450 and 4150..4300) also match other intervals after splitting; e.g.
on the faithful source `forcepad9` alone puts `pad` in `r5`, not `r9`.

## 6. Structural state (unchanged, re-verified this wave)

27 sections; file size **2,187,328 B** exact.

```text
.text        orig 0x69440 0x1337c8   ours same
.ARM.exidx   orig 0x19cc08 0x6f70    ours same
.ARM.extab   orig 0x1a3b78 0x26298   ours 0x26290  (8 B short, createShip)
.rodata      orig 0x1c9e10 0x41b67   ours 0x41c27  (192 B longer)
.data        orig 0x214000 0x296c    ours same
.bss         orig 0x216970 0x69ac    ours same
.got         orig 0x20fcf4 0x430c    ours same
.data.rel.ro orig 0x20cbb0 0x2fe8    ours same
```

## 7. Conclusion

The clamp is fully understood and the pool word is already correct. The last
missing piece is still `pad -> r9`, which requires `pad` to be enqueued before
`x`. Since `pad` has no COPY in the IR it has no hint, and `x`'s `$r3` hint
(from the first `DrawString` argument copy) always wins the queue. All
source-level attempts to give `pad` a hint (copies, helpers, const, type
changes, declaration reordering) coalesce away or perturb the rest of the
allocation; all `boxW` shapes that put `pad` in `r9` spill `top` because
`pad2`/`boxW` no longer share `sl`. HEAD (`short pad3`, **95.676047 %**)
remains the best measured shape.
