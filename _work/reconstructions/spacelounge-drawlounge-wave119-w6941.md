# SpaceLounge::drawLounge — wave-119 pad-weight / split study (w6941)

Status: research note (2026-11-06). Worktree `wt/w6941`, branch
`wave119-w6941`, base `98f531de9`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4505 / linked 4505, data 843/843, imports 207,
  lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave119-w6744.md` (eviction order),
`-wave119-w6714.md` (literal pool), `-wave118-w6616.md` (scratch knobs),
`-wave117-w6517.md` (292-variant sweep), `-wave113-w6117.md` (exhaustive
source-shape sweep), `-w103-oracle.md` (Unity oracle). This note supersedes
nothing.

## 1. Residual, unchanged

Only the clamp / join `0x1712b2..0x171330` differs. Original vs HEAD:

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

## 2. New result: the split is reachable, but only by changing pad's spill weight

A fresh look at the *whole* allocation (not just the clamp) shows the target
structure is exactly what the greedy allocator produces when `pad` is **not**
evicted from its first callee-saved register.  The existing scratch LLVM
7.0.2 at `/tmp/llvm_android_build` carries a `-w6927-boostpad` knob
(`CalcSpillWeights.cpp`: `weight *= 2` for size 6300..6450).  On the committed
HEAD source it gives:

```text
1712e0  ittt gt
1712e2  movgt.w r1, r9, lsl #16     ; short pad3 arm (HEAD's form)
1712e6  addgt.w r1, r6, r1, asr #15
1712ea  subgt   r5, r0, r1
...
1712fc  vmov    r1, s16             ; top
171300  mov     r2, r5              ; x -> r2   (split copy)
171302  add     sl, r6              ; boxW
171304  sub.w   r5, r1, r9          ; top - pad     <- %46 in r5 (original!)
171308  mov     fp, r1              ; top -> fp
17130a  mov     r1, r9              ; pad
17130c  subs    r6, r2, r1          ; x - pad
17130e  mov     r9, r2              ; x -> r9       (split copy)
171310  strd    sl, r8, [sp]
```

That is the original's **structure**: `pad -> r9`, `x` split at the join
(`r5 -> r2 -> r9`), `%46 (top - pad) -> r5`, `%45 (x - pad) -> r6`,
`%47 (boxW) -> r10`.  Only the two *other* callee-saved values are swapped:
`lineHeight -> r8` (original `fp`) and `top -> r1 -> fp` (original
`r2 -> r8`).  Measured: **95.1 %**, 1948 B, 729/729 insns — lower than HEAD's
95.7 % because HEAD's 3-instruction arm happens to align better in the
difflib metric.

So the residual is not "the split is unreachable"; it is a **three-way
callee-saved rotation** (`r8`/`r9`/`r11` for `top`/`pad`/`lineHeight`) that
the greedy allocator only gets right when `pad`'s spill weight is inflated
past `lineHeight`'s.  No source shape changes that weight (see below).

## 3. Source sweep this wave (~110 variants, all <= 95.676047)

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6` (~1.4 s), then the official
`verify.py` internals (`/dev/shm/w6941/fastscore.py`, `autojunk=False`).
The full block from `int pad = Globals::layout->field_0x94;` through the
`drawBox(...)` call was replaced for each variant.

| family | best | notes |
|---|---:|---|
| HEAD (`short pad3`) | **95.676047** | 1948 B, 729/728 |
| faithful `width + pad*2` | 93.964335 | 729/729, 2-insn arm |
| `pad2`/`pad+pad`/`pad<<1`/`2*pad` local | 93.964335 | coalesced |
| `boxW` local (faithful) | 72.602740 | pad r9, top spilled |
| `boxX`/`boxY`/`boxW` locals | 94.0 / 72.6 | coalesced / spilled |
| oracle-faithful (`centerX` in place) | 93.6 | `itt lt` arm |
| `pad` declared late (2 loads) | 70.0 | 1976 B |
| `const int pad`, `const int &padref` | 95.676047 | coalesced |
| `int x(centerX)`, `int x; x=centerX` | 95.676047 | identical |
| nested `if` / 3-way `else` / ternary | 95.676047 | identical |
| `two_if` (not else-if) | 92.1 | 729/728 |
| `pad + pad + pad - pad`, `pad*2 + (pad^pad)`, … | 95.676047 | folded |
| `SetColor((unsigned) pad)` (diagnostic) | 78.6 | pad r9, rest rotates, 1968 B |
| `pad_late` (field used in `top`) | 70.0 | 1976 B, 2 loads |

Second independent sweep (11 variants, w6941 re-run) targeting the
`lineHeight` / `pad2` interval shape — no improvement either:

| variant | raw % | size | insns |
|---|---:|---:|---|
| HEAD | **95.676047** | 1948 | 729/728 |
| `lineHeight` from `Globals::layout` | 95.676047 | 1948 | 729/728 |
| `lineHeight` inlined in `drawBox` (both spellings) | 95.676047 | 1948 | 729/728 |
| right arm `width + pad3*2` without parens | 95.676047 | 1948 | 729/728 |
| right arm `width + pad*2` without parens | 93.8 | 1948 | 729/729 |
| `lineHeight` declared before the clamp | 91.6 | 1952 | 729/731 |
| `pad2 = pad*2` local, `top`/clamp/`drawBox` use it | 94.0 | 1948 | 729/729 |
| same, `short pad3` arm | 94.0 | 1948 | 729/729 |
| `pad2` declared after `top` | 94.0 | 1948 | 729/729 |
| `pad2 = pad + pad` local | 72.8 | 1960 | 729/728 |

Also re-measured with the scratch compiler (not landable):

| flags | raw % | size | note |
|---|---:|---:|---|
| `-w6927-boostpad` | 95.1 | 1948 | **original structure**, lineHeight/top swapped |
| `-w6927-boostpad -w6616-lock46` | 95.1 | 1948 | same |
| `-w6927-boostpad -w6616-forcex5` | 80.0 | 1956 | — |
| `-w6927-boostpad -w6927-padprio` | 77.2 | 1952 | — |
| `-w6616-forcepad9 -w6616-forcex5 -w6616-nohint46 -w6616-lock46` | 93.6 | 1944 | pad r9, x r5, %46 r9 |
| `-w6616-lock -w6616-lock46` (faithful) | 94.0 | 1948 | — |
| `-join-liveintervals=false` | 50.2 | 2016 | too broad |
| `-enable-deferred-spilling` | 83.8 | 1960 | — |

A targeted scratch-LLVM patch that skips only the `%43 = COPY %608` phi
coalescing (the copy at raw slot index 7280) gives **86.8 %**, 1944 B: the
split alone is not enough, the rest of the rotation rotates with it.

## 4. Conclusion

`pad` has no COPY in the MIR, so it is an unhinted `0xA` interval and is
selected after every hinted `0xE` interval; by then `x` occupies `r9`.  The
original needs `pad -> r9`, `x` split at the join and `%46 -> r5`, which the
greedy allocator only does when `pad`'s spill weight is inflated (the
`-w6927-boostpad` trace above).  Every source-level way of adding pad uses
(`pad+pad`, `pad*2 + (pad^pad)`, …) is folded away and leaves the weight
unchanged; every source shape that reaches `pad -> r9` (`boxW`, `SetColor(pad)`)
rotates the remaining callee-saved values.  The committed HEAD
(`short pad3`, **95.67604667124228 %**) remains the best measured shape.

## 5. Structural state (unchanged, re-verified)

27 sections; file size **2,187,328 B** exact; `.ARM.extab` 0x26298 (size- and
base-exact).  No section/extab change from this pass.

## 6. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w6941/fastscore.py
```
