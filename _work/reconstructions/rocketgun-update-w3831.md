# `RocketGun::update(int)` — pass 2 (w3831)

**Status:** research note, 2026-11-03. Worktree `wt/w3831`, branch `wave89-w3831`,
base `5710f6d0`. No source change landed: every faithful source-shape variant tried
this pass scores <= 86.9 %. Tree clean, gate unchanged (byte 4498 / linked 4498,
size-exact 4522/4524, `.text 0x1337c8`, lint `CLEAN ✓`, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**. This pass re-derived the
real-clang post-RA allocation and swept ~40 more source shapes; none moved the
score. See `rocketgun-update-86-9-1908-b_w3811.md` for the full spill-weight
analysis this note builds on.

## 1. Real-clang post-RA state (confirmed this pass)

`clang++ -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi`
(the NDK clang accepts `-print-after-all`; it does **not** accept `-print-after=greedy`
or `-debug-only=regalloc` — those need the self-built llc). The Virtual Register
Rewriter dump gives the true final assignment:

| value | original | ours |
| --- | --- | --- |
| `elapsed` | r4 until launch block, then `[sp,#16]` | r5 until loop preheader, then `[sp,#12]` |
| `this` | sl, spilled `[sp,#20]` at launch block | sl, spilled `[sp,#20]` at launch block |
| `dt` | `[sp,#24]` | `[sp,#24]` |
| `i*60` trail cursor | r4 (`adds r4,#60`) | r9 (`add.w r9,r9,#60`) |
| `i*12` | fp | fp |
| `i` | r8 | r8 |
| `Sinf` result `s` | `[sp,#8]` | r6 |
| `Cosf` result `c` | `[sp,#12]` | `[sp,#8]` |
| `trailMatrices` local | none (5 reloads of `[sl,#216]`) | `[sp,#16]` |

Frame slots: original `guard 4, s 8, c 12, elapsed 16, this 20, dt 24`;
ours `guard 4, c 8, elapsed 12, trailMatrices 16, this 20, dt 24`.
Because `elapsed`/`this` are not adjacent in ours, the two launch-block stores
stay separate (`str r5,[sp,#12]` + `str.w sl,[sp,#20]`) instead of the original's
`strd r4, sl, [sp,#16]`.

The loop body is otherwise structurally identical: the original also recomputes
every stack address inside the loop (`add.w sl,sp,#116`, `add r5,sp,#128`,
`add r6,sp,#40`, `add r0,sp,#144`), and the RA rematerializes our hoisted
`t2ADDri %stack.N,0` vregs in exactly the same way. The difference is only which
scratch register each rematerialized address uses (`sl`/`r6`/`r5`/`r4` vs
`r6`/`r5`/`r4`/`r9`).

## 2. Allocation cascade (self-built LLVM 7.0.0 llc, `-debug-only=regalloc`)

Vreg numbers and weights match the w3811 measurements (`%29` elapsed 9.400839e-04,
`%251` 8.566814e-03, `%252` 9.612001e-03, `%255` 1.007480e-02, `%256` 3.753831e-03,
`%384` cursor 1.489900e-02, `%385` i*12 3.281973e-02, `%386` i 2.805497e-02):

1. `%29` elapsed -> r4, `%28` this -> r5, `%247` dt -> r6.
2. `%251` (`&stack.4`) evicts `%29` -> r4; `%29` -> r8.
3. `%252` (`&stack.3`) evicts `%251` -> r4; `%251` -> r9. **`%252` keeps r4.**
4. `%255` -> r6 (evicts dt), `%256` -> r11, `%386` (i) evicts `%29` from r8.
5. `%384` (cursor) runs `tryEvict` with candidates r4 (`%252`), r9 (`%251`),
   r11 (`%284`); `tryEvict` keeps the **last** evictable register, so it takes
   r11 (or r9 in the real clang build) and never r4.
6. `%252` is only evicted from r4 much later (cascade 16) and then rematerialized.
   `%29` is finally re-assigned to r5 (real build) after `this` moves to sl.

So the cursor cannot get r4 while `%252` holds it, and `%252` is heavier than
elapsed. The original's cursor is in r4 because in the original's allocation
`%252`/`%251` did not own r4 at that point (or the cursor was allocated first).

## 3. Why removing the `trailMatrices` local does not fix it

Removing the local is structurally right (the original has no local) but changes
the *whole* allocation: `this` moves from sl to fp, `elapsed` is spilled at the
first call (`str r1,[sp,#16]` before `Gun::update`), and the loop collapses
(1880 B / 57.6 %). The local is what currently makes `this` land in sl (and thus
be spilled at the launch block); without it the allocator keeps `this` in fp and
never creates the `[sp,#20]` slot.

Likewise, forcing `s` to spill (the other prerequisite for the original's
`s 8, c 12, elapsed 16` layout) is not reachable: `s` is live across only the
Cosf/VectorCross/VectorNormalize/operator* calls and r6 is free for it in our
allocation. Declaring `s2 = s + s` / `c2 = c + c` early (to lengthen the live
ranges) is folded back by the compiler and changes nothing.

## 4. Variants measured this pass (all reverted, <= 86.9)

Each was built with the full `link_so` and scored with the official verifier.

| variant | size | score |
| --- | --- | --- |
| baseline | 1908 | **86.9** |
| `Matrix &trail = (*this->trailMatrices)[i]` | 1908 | 85.3 |
| `Matrix *trail = &(*this->trailMatrices)[i]` | 1908 | 85.3 |
| direct `(*this->trailMatrices)[i]` at both sites | 1884 | 63.5 |
| direct `this->trailMatrices->data()[i]` at both sites | 1884 | 63.5 |
| `Matrix &trail` hoisted to top of `if (trailMatrices)` | 1908 | 68.0 |
| `Matrix &trail` hoisted to top of loop body | 1920 | 46.3 |
| second `ahead` via `this->gun->` | 1908 | 86.9 |
| no `gun = this->gun` before second `ahead` | 1908 | 86.9 |
| first `MatrixSetTranslation` via `(*this->trailMatrices)[i]` | 1892 | 66.3 |
| `heading` computed before first `MatrixSetTranslation` | 1832 | 59.3 |
| `ss = s+s`, `cc = c+c` locals | 1908 | 86.9 |
| `s2`/`c2` early (all placements) | 1908 | 86.9 |
| inline `VectorNormalize(VectorCross(...))` | 1828 | 60.0 |
| `const Vector &pos/vel` bindings (first/second/cluster) | 1908 | 86.9 |
| `phase` as `uint32_t` local | 1908 | 86.9 |
| `total` as `uint32_t` | 1908 | 86.7 |
| `int i` / `size_t i` loop | 1908 | 86.9 |
| `Matrix *trailData` before loop, `trailData[i]` | 1896 | 49.0 |
| `trailData` + `(*trailMatrices)[i]` mix | 1880 | 48.6 |
| `(*trailMatrices)[i]` for both (no local) | 1908 | 84.0 |
| `trailMatrices` local as `*const` | 1908 | 86.9 |
| local moved before `dt` / before anchor | 1908 | 86.9 |
| `dt` after anchor | 1908 | 86.9 |
| heading via member `gun->` | 1908 | 86.9 |
| no `unused` anchor | 1908 | 85.8 |
| no `padx` anchor | 1908 | 86.7 |
| no both anchors | 1908 | 85.6 |
| `volatile float padx` | 1908 | 86.9 |
| `volatile char padx` | 1908 | 86.0 (635 insns) |
| `volatile int padx = elapsed` | 1904 | 72.6 |
| `volatile int padx = particleSystem` | 1912 | 80.8 |
| two padx anchors | 1908 | 76.9 |
| `volatile float unused = dt` | 1912 | 84.6 |
| `volatile float unused = (float) elapsed` | 1912 | 84.6 |
| anchor before `dt` | 1908 | 86.9 |
| two `unused` anchors / char variant | 1912 | 75.6 |
| loop-body `volatile int pad = elapsed` (1..4x) | 1908..1916 | 62.7 / 45.6 / 42.9 / 56.9 |

## 5. Remaining root cause

One register permutation, driven by the greedy allocator's eviction order:

* `elapsed` is evicted from r4 at the loop preheader by the hoisted
  `t2ADDri %stack.N,0` address vregs (`%251`/`%252`, weights ~9e-3 vs elapsed's
  9.4e-4) and finally lands in r5 instead of r4.
* `%252` (`&stack.3`) then owns r4 through the loop, so the strength-reduced
  `i*60` cursor (`%384`) takes r9 (real clang) instead of the original's r4.
* Because `elapsed` is in r5 and `this` stays in sl, the launch-block spill is
  `str r5,[sp,#12]` + `str.w sl,[sp,#20]` rather than `strd r4,sl,[sp,#16]`.
* The missing `s` spill (ours keeps it in r6) and the extra `trailMatrices`
  stack slot (`[sp,#16]`) are the other two visible consequences; both are
  downstream of the same allocation choice.

A source shape that fixes this must make the cursor's live range/priority beat
`%252` (so it is allocated r4) while keeping `this` in sl so it is spilled at
`[sp,#20]`. Every faithful variant tried so far either keeps the current
permutation or collapses the loop allocation entirely.
