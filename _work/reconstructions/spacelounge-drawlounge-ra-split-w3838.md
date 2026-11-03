# SpaceLounge::drawLounge — clamp/lineHeight shape vs greedy-RA split (w3838)

Status: research note (2026-11-03). Worktree `wt/w3838`, branch `wave89-w3838`,
base `5710f6d0`. No `src/` change landed: every source shape tested either kept
the official score at 95.6 or lowered it.

Baseline (`cmake --build cmake-build-match --target check -j8`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
.text 0x1337c8
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.6`, `osize = usize = 1948`,
not byte/linked exact. Object-level oracle (`w3692_oracle.py`) reports `95.5`
for the same build.

## 1. The remaining diff

The instruction stream is identical except for the clamp right arm and a
register permutation. Original (overview block):

```text
r4 = this/layout  r5 = x  r6 = width  r8 = agent/top
r9 = pad  sl = pad*2  fp = lineHeight
...
1712dc: itt gt
1712de: addgt.w r1, r6, sl      ; width + pad*2
1712e2: subgt   r5, r0, r1      ; x = (w - inset) - (width + pad*2)
```

Ours:

```text
r5 = pad copy/top-pad  r9 = x  fp = pad/lineHeight
...
5a4: itttt gt
5a6: movgt   r1, #0
5a8: subgt.w r1, r1, fp, lsl #1 ; 0 - pad*2
5ac: subgt   r1, r1, r6         ; - width
5ae: addgt.w r9, r1, r0         ; x = (w - inset) + (-width - pad*2)
```

The current source writes the right arm as
`Globals::w - layout->buttonInsetX + (-width - pad - pad)`. This keeps a single
`mov r5, fp` pad copy at the join. The IR-faithful form
`Globals::w - layout->buttonInsetX - (width + pad * 2)` reproduces the
original's two-instruction `addgt/subgt` arm exactly, but the pad copy is then
duplicated into **both** clamp arms (`mov r5, fp` at 0x58c and 0x596), and the
official score falls to 93.8.

## 2. Why the faithful clamp does not fix the RA (new evidence)

The clamp form does not change the register allocation at all. Compiling the
base (`flat`) and the faithful (`-(width + pad*2)`) sources to LLVM IR and
running the project's instrumented LLVM 7 `llc -debug-only=regalloc` gives the
same intervals and the same eviction:

```text
x        %573 [6640r,6752B:0)[6768r,6832B:2)[6832B,7008B:0)[7040r,7088B:1)[7088B,10896r:3)
              weight:2.956381e-04
top-pad  %46  [7376r,8112r:0)  0@7376r  weight:6.251314e-04
```

Both forms produce `pad = fp`, `x = r9`, and the same `mov r5, fp` split copy.
The clamp tree only changes the four right-arm instructions, not the weights.

The chain is pure greedy-RA eviction ordering:

1. `%573` (x) is assigned `$r5` first (size 4204, hint-boosted priority).
2. `%46` (top - pad, weight 6.25e-4 > x's 2.96e-4) evicts `%573` from `$r5`
   (`should evict: %573 ... evicting $r5 interference: Cascade 2`).
3. `%573` is re-assigned `$r9` (evicting `%53`).
4. `%2` (pad) is then allocated: `$r9` is occupied by x, so it evicts `%355`
   from `$r11` (`$r11` total 0.4687 vs `$r9` 1.1133 in the split-candidate
   evaluation).
5. `%44` (lineHeight, weight 6.10e-4) evicts pad from `$r11`; pad is split,
   `%613 -> $r11`, `%612 = COPY %613 -> $r5`, giving the `mov r5, fp` copy.

The original instead kept x in `r5` and split it at the top-pad definition
(`mov r3, r5` / `mov r9, r3`), leaving `r9` free for pad. That requires
`weight(top-pad) <= weight(x)`, i.e. x's live interval must be much shorter
or top-pad's much longer than in our MIR. Both are identical between the flat
and faithful clamp forms.

## 3. Shapes measured (official oracle pct, size)

All measured against the pristine source; no change improves on 95.5.

* clamp: `-(width+pad*2)` 93.8; `-width-pad*2` 93.8; `-(pad*2+width)` 93.8;
  `-(width+pad+pad)` 95.3; `+(-width-pad-pad)` **95.5** (current);
  `-width-(pad+pad)` 93.8; `(w-inset)-(width+pad*2)` 93.8;
  `-width-pad-pad` 92.3; `-width+(-pad-pad)` 95.5.
* `lineHeight` before the clamp: 91.6/1952; before `x`: 95.0; first: 91.6.
* `layout` cached before `pad`: 95.5; `layout` after the clamp: 95.5;
  `layout` before `x`: 91.6/1952.
* `inset` local: 95.5; `inset` local + faithful clamp: 93.8.
* `centerX` used in the clamp condition: 95.5; `int x; x = centerX;`: 95.5.
* `textX = x` / `textX = x + 0` after `lineHeight`, used in the later
  DrawString/GetTextWidth: 95.5 (the copy coalesces).
* `boxX`/`boxY`/`boxW` locals used in both drawBox and DrawRectangle: 95.5.
* `p = pad` local for the box: 95.5; `lineHeight` via `Globals::layout`: 95.5.
* `__builtin_expect` variants: none 91.8/1952; `!=0` 95.5; `true` 95.5;
  weight 0 84.5/1952; on clamp1 weight 0 95.2; on clamp2 weight 1 91.5/1956;
  on the final `isKnown` 87.1.
* `String("  ") + role` 95.2; `role + String("  ", false)` 95.5;
  `String sp("  ")` local 91.7/1960; `known` local 94.2/1952;
  `roleWidth` accumulator 90.8/1952.
* `top` forms: `pad*2` 95.5; `top` before `centerX` 95.5; `pad2` locals 95.5.

Instrumented-`llc` probes (tools only, not source):

* `NO_HINT_575=1` (clear x's r3 copy hint): pad moves to `r11`, x to `r10`;
  still not the original.
* `NO_EVICT_46_573=1` (forbid top-pad evicting x): pad `r11`, x `r9`, pad copy
  duplicated in both arms; still not the original.
* `FORCE_PAD_PRIO` sweep (w3818): no value reproduces the original pattern.

## 4. Conclusion

The remaining 95.6 -> 100 gap is not a wrong constant, field offset, callee,
block order, or literal pool, and it is not the clamp/lineHeight source shape:
the faithful clamp produces the original's right arm but leaves the greedy-RA
eviction (`%46` top-pad evicting `%573` x from `r5`) unchanged, so pad still
lands in `fp` and needs the `r5` copy. The single most important root cause is
the greedy allocator's split/eviction ordering, rooted in the spill-weight
ratio between x's long live interval (4288 slots) and top-pad's short one
(736 slots). No source shape tested flips that ratio without changing the
instruction stream.
