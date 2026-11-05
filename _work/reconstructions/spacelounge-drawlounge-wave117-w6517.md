# SpaceLounge::drawLounge — wave-117 source-shape sweep (w6517)

Status: research note (2026-11-05). Worktree `wt/w6517`, branch
`wave117-w6517`, base `c22678d64`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4501 / linked 4501, data 843/843, imports 207,
  lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave116-w6417.md` (boxW IR-order finding),
`-wave115-w6318.md` (branch-weight source shape), `-wave114-w6218.md`
(eviction weight), `-wave113-w6117.md` (exhaustive source-shape sweep),
`-wave112-w6015.md` (the 7:1 flag), `-wave110-w5818.md` (RA trace). This note
supersedes nothing.

## 1. The residual is one RA coin-flip

The complete byte residual is the clamp / join region
`0x1712b2..0x171330`; every other instruction matches. The only two values
whose physical registers differ are `pad` and `x`:

```text
value        original        ours (HEAD)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole interval)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

The two are coupled. `pad` is enqueued unhinted at priority `(1<<29)+6384`;
`x` (`%608`, size 4228, hint `$r3`) is enqueued at `(1<<31)|(1<<30)|4228` and
is therefore selected long before `pad`. `%46` (`top - pad`, weight
6.251309e-04) then evicts `x` from `r5` (weight 2.946140e-04, cascade 2),
`x` is re-queued and immediately re-selected into `r9` (which is still free),
and `pad` (cascade 7) therefore has to take `r11`/`fp`; it is then evicted at
cascade 10 by `lineHeight` (`%44`) and `RS_Split` re-picks `r11`.

Original allocation is the mirror image: `x` keeps `r5` through the clamp and
is split at the join (`mov r3, r5` / `mov r9, r3`), which frees `r5` for
`top - pad` and leaves `r9` for `pad`. The original's 8-byte right arm
(`itt gt; addgt.w r1, r6, sl; subgt r5, r0, r1`) then falls out for free,
because `sl` already holds `pad*2`.

The committed `short pad3 = pad;` hack exists only because our `pad` is in
`fp`: with `pad` in `r9` the plain `width + pad * 2` would emit the original
8-byte arm (verified: the faithful variant's right arm *is*
`itt gt; addgt.w r1, r6, sl; subgt.w r9, r0, r1`, 8 B — it is the extra
`mov r5, fp` copies at the branch targets that cost the 1.7 points).

## 2. `pad -> r9` is source-reachable, but only with a spill

Wave-116 found that a `boxW` local flips the IR order to add-before-subs and
gets **`pad -> r9` and `x -> r5`** — exactly the original. Reproduced here:

```cpp
int boxW = width + pad * 2;      // after the clamp, before lineHeight
int lineHeight = layout->field_0x30_rowHeight;
layout->drawBox(2, x - pad, top - pad, boxW, lineHeight, String(""));
```

with the faithful `width + pad * 2` clamp. Result: `pad = r9`, `x = r5`,
`pad2 = fp(r11)`, `lineHeight = r8`, and **`top` is spilled to `[sp,#20]`**
(frame 120 -> 128, every `sp`-relative access shifts by 8) -> 72.6 %.

The reason is a three-way register rotation. In the original `pad2` lands in
`r10` (`sl`), so the join's `add sl, r6` computes `boxW` in place in `r10`,
leaving `r11` for `lineHeight` and `r8` for `top`. In the `boxW` shape `pad2`
lands in `r11` instead (something — `Globals::Canvas` — occupies `r10` across
`[688,7536)` in that IR), so `boxW` is computed in `r6`, `lineHeight` takes
`r8`, and `top` (materialised at the join while `pad2` is still live) has no
callee-saved register left.

## 3. Measurements this wave (292 variants, all <= 95.676047)

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6` (~1.2 s), then the official
`verify.py` internals (`/dev/shm/w6517/fastscore.py`, `autojunk=False`).
Scratch generators `/dev/shm/w6517/gen117{b..o}.py`, results
`/dev/shm/w6517/sweep117b.log` + `r117{b..o}.json`.

### 3.1 The `boxW` family (the only shape that reaches `pad -> r9`)

| variant | raw % | size | frame | pad | x | top |
|---|---:|---:|---:|---|---|---|
| HEAD (`short pad3`) | **95.676047** | 1948 | 120 | fp | r9 | r8 |
| faithful `width + pad*2` | 93.964335 | 1948 | 120 | fp | r9 | r8 |
| faithful + `boxW = width + pad*2` | 72.602740 | 1968 | 128 | **r9** | **r5** | spill |
| faithful + `boxW = pad*2 + width` | 72.602740 | 1968 | 128 | r9 | r5 | spill |
| faithful + `boxW = width + pad + pad` | 72.889499 | 1960 | 128 | r9 | r5 | spill |
| faithful + `boxW = pad + pad + width` | 72.752231 | 1960 | 128 | r9 | r5 | spill |
| faithful + `boxW` + `boxX`/`boxY` locals | 72.602740 | 1968 | 128 | r9 | r5 | spill |
| faithful + `boxW` + `pad2` local | 72.602740 | 1968 | 128 | r9 | r5 | spill |
| faithful + `pad2 += width` (in-place) | 72.602740 | 1968 | 128 | r9 | r5 | spill |
| faithful + `width += pad2` (in-place) | 72.602740 | 1968 | 128 | r9 | r5 | spill |
| `boxW` before the clamp | 89.759450 | 1940 | 120 | fp | r9 | r8 |
| `boxW` before `x` | 89.759450 | 1940 | 120 | fp | r9 | r8 |
| HEAD clamp + `boxW = width + pad3*2` | 71.819425 | 1972 | 128 | r9 | r5 | spill |
| HEAD clamp + `boxW = width + pad*2` | 71.086808 | 1964 | 128 | r9 | r5 | spill |
| `boxW` + `const` / `unsigned` / `short` | 72.6 / 72.6 / 72.5 | 1968 | 128 | r9 | r5 | spill |
| `boxW` + `top` as `short`/`unsigned`/`long` | 72.5 / 72.6 / 72.6 | 1968 | 128 | r9 | r5 | spill |
| `boxW` + `top` declared after clamp/boxW | 80.384088 | 1968 | 128 | r9 | r5 | spill |
| `boxW` + `float topF` converted at uses | 85.871056 | 1948 | 120 | fp | r9 | — |
| `boxW` + `float padF` | 72.602740 | 1968 | 128 | r9 | r5 | spill |
| `boxW` + DrawString y recomputed | 72.602740 | 1968 | 128 | r9 | r5 | spill |

Every `boxW` shape that keeps the add-before-subs IR order spills `top`; every
shape that avoids the spill (`boxW` before the clamp) loses the `pad -> r9`
flip again. No source shape measured produces both.

### 3.2 `pad2` / clamp / declaration permutations (no `boxW`)

All of these are byte-for-byte the HEAD allocation (95.676047, 1948 B):

```text
pad2 local after pad / before centerX / after top (top uses pad2)
pad2 = pad*2 / pad+pad, used in top + clamp + drawBox
faithful `width + pad*2` (93.964335)   faithful `width + pad+pad` (95.473251)
`int pad3 = (short)pad` / `long pad3 = (short)pad` / `short pad3 = (short)pad`
`width + pad3*2u` / `width + (int)(short)(pad*2)` / `width + (int)(short)(pad+pad)`
`width + pad3 + pad3` (92.392049)      `width + (pad3 << 1)` (95.676047)
`unsigned short pad3` (91.718001)
inset local / screenW local / both (95.676047, also with boxW -> 71.1)
`const int pad` / `const int top` / `const int centerX`
`x` declared before `layout` / before `pad3` / `pad3` before `layout`
`lineHeight` before `x` (91.643836)    `lineHeight` before clamp (95.198903)
explicit `else { x = centerX; }` / `x = centerX` after clamp
`result-var` with explicit else (71.692940)
condition operands inverted (`inset > x-pad`) (95.401510 / 95.538778)
`!(x - pad >= inset)` (95.676047)
right-edge condition first (90.884167)
two separate `if`s (92.107069 / 92.244338)
ternary clamp (92.107069 / 95.676047 / 92.107069)
`clamp-field-direct` (reload `Globals::layout->field_0x94`) (91.220850)
`pad-top-only` (`pad2` in clamp, `pad` in top) (83.906465)
helper `static inline int clampX(...)` (95.610425)
```

### 3.3 Attempts to break the eviction by shortening `x`

`shouldEvict(%46, %608)` is `A.weight > B.weight`; `%46` is 6.251309e-04 and
`x` is 2.946140e-04. To stop the eviction `x` must reach ~6.25e-04, i.e.
`Size < ~1930` (its interval is 4228). Introducing `int textX = x;` after
`drawBox`/`DrawRectangle` and using it in the `DrawString` x arguments does
**not** help — the copy is coalesced away, and when it is not, the extra live
value grows the frame to 128:

| variant | raw % | size |
|---|---:|---:|
| `textX = x` after `drawBox` (both DrawStrings) | 78.189300 | 1968 |
| `textX = x` after `DrawRectangle` (both / 1st / 2nd) | 80.631868 | 1964 |
| `const int textX = x` after `DrawRectangle` | 80.631868 | 1964 |
| `int x2 = x + 0` | 78.189300 | 1968 |
| `float topF` (no `boxW`) | 85.871056 | 1948 |
| `float topF` + `boxW` | 85.871056 | 1948 |

### 3.4 `boxY` early (lower `%46`'s weight instead of raising `x`'s)

`int boxY = top - pad;` computed right after `top` would give `%46` a long
live range and drop its weight below `x`'s. It changes the schedule and the
frame:

| variant | raw % | size |
|---|---:|---:|
| `boxY` early, used in drawBox + DrawRectangle | 71.086808 | 1964 |
| `boxY` early, drawBox only | 71.086808 | 1964 |
| `boxY` after clamp, both | 89.042040 | 1932 |
| `boxY` after clamp, drawBox only | 83.413627 | 1952 |
| `boxX`/`boxY` early | 70.418668 | 1952 |

## 4. Compiler-instrumentation experiments (tools only, not landed)

* Patched clang 7.0.2 at `/tmp/llvm_android_build` (source `/tmp/llvm_android2`,
  original `RegAllocGreedy.cpp` saved at
  `/dev/shm/w6517/RegAllocGreedy.cpp.orig`) adding a hidden
  `-mllvm -w6517-nohint` that disables the `Prio |= 1<<30` hint boost. Result:
  `pad -> r8`, `x -> sl`, `r9 -> width`; official-style score ~94.4. Not the
  original allocation.
* `-regalloc-csr-first-time-cost=1/2/4`, `-split-spill-mode=default/size/block`,
  `-enable-deferred-spilling`, `-enable-local-reassign`,
  `-exhaustive-register-search`: none move `pad` to `r9`.
* The patched clang is functionally identical to stock for every other flag
  (this is what `/dev/shm/w6517/cc7.sh` uses); do **not** leave
  `-w6517-nohint` in `CMakeLists.txt` — NDK clang-7 does not know it.

## 5. Conclusion

`pad -> r9` (and therefore the original 8-byte clamp arm and the byte-exact
function) is source-reachable only through the `boxW`/add-before-subs IR
order, and that order always spills `top` because `pad2` is pushed from `r10`
to `r11` by the extra live value. 292 variants measured this wave (plus the
~300 from waves 106–116) do not produce both `pad -> r9` and an unspilled
`top`. HEAD (`short pad3`, 95.676047 %) remains the best measured shape.

## 6. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w6517/fastscore.py                     # raw score of the link
python3 /dev/shm/w6517/diffops.py                       # per-instruction residual
python3 /dev/shm/w6517/sweep/run2.py variants.json out.json   # variant sweep
```

Per-variant raw score (`official verify.py` internals, `autojunk=False`):
`/dev/shm/w6517/fastscore.py`. Scratch variant sources and generators are in
`/dev/shm/w6517/` (scratch only, not tracked).
