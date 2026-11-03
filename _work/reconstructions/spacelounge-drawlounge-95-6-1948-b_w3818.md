# SpaceLounge::drawLounge — 95.6%, 1948 B (w3818)

Status: research note (2026-11-03). Worktree `wt/w3818`, branch `wave88-w3818`,
base `41804126`. No `src/` change landed: the remaining diff is a greedy-RA
register permutation, and no tested source shape improves the official score.

Baseline (`cmake --build cmake-build-match --target check -j8`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
.text 0x1337c8 @ 0x69440
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.6`, `osize = usize = 1948`,
`byte = linked = false`. The object-level oracle
(`_work/reconstructions/w3692_oracle.py`) reports `95.4545` for the same build.

## Remaining diff

The instruction stream is identical except for one block and register names.
The original allocates the lounge-overview locals as

```text
r4 = this/layout   r5 = x        r6 = width
r8 = agent/top     r9 = pad      sl = pad*2      fp = lineHeight/top
```

while clang 7.0.2 greedy RA in our build allocates

```text
r4 = this/layout   r5 = pad copy/agent   r6 = width
r8 = top           r9 = x                sl = pad*2   fp = pad/lineHeight
```

Because `pad` is in `fp`, the allocator must copy it to `r5` before
`fp` is reloaded with `lineHeight` (`mov r5, fp` at the join); the original
keeps `pad` in `r9` and `lineHeight` in `fp`, so it needs no copy. The
`x`/`pad` swap also moves the `x - pad` / `top - pad` subtractions to
different registers. That permutation is the whole remaining diff.

The original clamp right arm is the IR-faithful

```text
addgt r1, r6, sl      ; width + pad*2
subgt r5, r0, r1      ; x = (w - inset) - (width + pad*2)
```

The current source writes the subtraction as `+ (-width - pad - pad)`; this
makes Reassociate emit a four-instruction negate tree, but keeps the single
`mov r5, fp` pad copy after the join. The literal `- (width + pad*2)` form
reproduces the original two-instruction tree but duplicates the pad copy into
both clamp arms, and measures lower (official 94.0 vs 95.6).

## Searches that did not move the RA

All of the following kept `pad = fp` and the same 64 differing instructions
(22 diff opcodes), or changed the size and lowered the score:

* clamp forms: `-(width+pad*2)`, `-(width+pad+pad)`, `-width-pad-pad`,
  `-(pad*2+width)`, `-(pad+pad+width)`;
* condition operand orders (`pad+x+width`, `x+pad+width`, `width+pad+x`,
  `x+width+pad`, `pad+width+x`, `width+x+pad`);
* first-arm `inset+pad` vs `pad+inset`;
* `top` as `pad+pad`, `pad*2`, `pad<<1`;
* declaration/statement permutations of `agent`, `topLeft`, `bottomRight`,
  `pad`, `SetColor`, `centerX`, `top`, `layout`, `x`, `lineHeight`;
* `x` as `int x = centerX`, no `x` (mutating `centerX`), `int x; x = centerX;`;
* `const`/`unsigned`/`long`/`volatile`/`register` on `pad`, `centerX`, `top`,
  `width`, `lineHeight`;
* `__builtin_expect` variants (`!=0`, `!!`, `==1`, `>0`, weight 2/100);
* `lineHeight` as a local vs `layout->field_0x30_rowHeight` at the call;
* `name`/`role` ternary vs if/else, name/role order, `String` ctor forms,
  commutative operand swaps in `getRace()+406`, `getType()+354`,
  `name+String("  ")`, `role+String("  ")`, `width += ...`.

Moving `lineHeight` before `x` does flip `pad` to `r5` (size 1948 with the
`-(width+pad*2)` clamp), but the official score falls to 95.2. Using
`layout->field_0x94` directly in the drawBox call flips `pad` to `r9`, but the
compiler then reloads the field (size 1972, 68.9%).

## Conclusion

The remaining 95.6 -> 100 gap is a greedy-RA colouring permutation rooted in
the `pad` vs `x` spill-weight order, not a wrong constant, field offset,
callee, block order or literal pool. No source-shape change in this pass
improved `byte_exact`, `linked_exact`, or the official fuzzy score, so the
tree is unchanged.
