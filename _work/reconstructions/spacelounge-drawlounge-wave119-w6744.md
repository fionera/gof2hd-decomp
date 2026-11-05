# SpaceLounge::drawLounge — wave-119 eviction-order study (w6744)

Status: research note (2026-11-05). Worktree `wt/w6744`, branch
`wave119-w6744`, base `b5cbf56bd`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.67604667124228 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4505 / linked 4505, data 843/843, imports 207,
  lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave119-w6714.md` (literal-pool study),
`-wave118-w6616.md` (scratch-compiler knobs, `pad -> r9` + 8-byte arm),
`-wave117-w6517.md` (292-variant sweep, `boxW` IR-order finding),
`-wave116-w6417.md`, `-wave115-w6318.md`, `-wave114-w6218.md`,
`-wave113-w6117.md`, `-wave112-w6015.md`, `-wave110-w5818.md`. This note
supersedes nothing.

## 1. The residual, restated

Only the clamp / join `0x1712b2..0x171330` differs (plus the tail registers
that follow from it). The original allocates `pad -> r9`, `x -> r5` (split at
the join into `r5 -> r3 -> r9`); HEAD allocates `pad -> fp`, `x -> r9`
(whole interval), which forces `mov r5, fp` copies at both clamp branch
targets and puts `top` in `r8` instead of `fp` after the join.

## 2. New result: `%45`, not `%46`, is the interval that evicts `x`

The wave-118 note says `%46` (`top - pad`) evicts `x` from `r5`. A fresh
`-mllvm -debug-only=regalloc` trace of the committed HEAD shows the evictor is
actually **`%45` (`x - pad`)**, the other drawBox argument:

```text
selectOrSplit GPRnopc:%45 [7568r,8272r:0) weight:6.432506e-04
should evict: %608 [6784r,11072r) weight:2.946140e-04
unassigning %608 from $r5
assigning %45 to $r5
```

`%46` (`top - pad`, weight 6.251309e-04) pops later and takes `r9`, because
`x` has already moved there. The exact queue order for the clamp values is

| value | vreg | size | priority | hint |
|---|---|---:|---:|---|
| agent | `%1` | 8784 | `0xE0000000+8784` | yes |
| `x` | `%608` | 4220 | `0xE0000000+4220` | `$r3` |
| width | `%607` | 1968 | `0xE0000000+1968` | `$r0` |
| `pad2+width` | `%47` | 768 | `0xE0000000+768` | yes |
| `top-pad` | `%46` | 736 | `0xE0000000+736` | `$r3` |
| `x-pad` | `%45` | 704 | `0xE0000000+704` | `$r2 $r1` |
| `pad` | `%2` | 6384 | `0xA0000000+6384` | **none** |
| `pad2` | `%381` | 6160 | `0xA0000000+6160` | none |

Because `%45`/`%46` carry the `1<<30` hint bit and `pad` does not, `pad`
always pops after the eviction. `pad` then evicts `%365` from `r11`
(`2.159249e-04 > 2.145276e-04`) and is itself evicted from `r11` by
`lineHeight` (`%44`, `5.978897e-04`), which is why HEAD ends with
`pad -> fp` and the `mov r5, fp` split copies.

For the original allocation the order must instead be

1. `x -> r5`;
2. `pad -> r9` (r9 still free);
3. `%45`/`%46` evict `x` from `r5`;
4. `x` re-selected: r9 is occupied by `pad`, so it splits at the join
   (`r5 -> r3 -> r9`).

Step 2 before step 3 is unreachable from the stock priority queue: any
hinted interval beats any unhinted one, so `pad` cannot be popped between
`x` and `%45`/`%46` unless it too is hinted — and then it pops *before* `x`
and takes `r8` (`-w6616-hintpad`: pad=`r8`, x=`sl`, 78.3 %, 1972 B).

## 3. Scratch-compiler confirmation of the ordering argument

A scratch LLVM 7.0.2 at `/tmp/llvm_android_build` (source `/tmp/llvm_android2`)
was extended with one new hidden knob, `-w6744-nohint45` (clear the `1<<30`
bit for the size 600..800 interval, i.e. `%45`), alongside the existing
wave-118 knobs. Measurements on the committed HEAD source:

| flags | raw % | size | pad | x | note |
|---|---:|---:|---|---|---|
| (none) | 95.676047 | 1948 | fp | r9 | committed HEAD |
| `-w6616-hintpad` | 78.3 | 1972 | r8 | sl | pad pops before x |
| `-w6744-nohint45 -w6616-nohint46` | 80.1 | 1968 | r5 | r8 | pad pops before %45/%46, but `%102` now grabs r9 first |
| `-w6744-nohint45 -w6616-nohint46 -w6616-forcepad9` | 68.9 | 1940 | r9 | r5 | r9 forced; `top` still spills |
| `-w6616-boostx -w6616-forcepad9 -w6616-nohint46` (wave 118) | 93.617021 | 1944 | r9 | r5 | exact clamp + 8 B arm, join 4 B short |

The `nohint45+nohint46` run is the informative one: with `%45`/`%46`
unhinted, `pad` does pop before them as predicted, but `%102` (a local
interval with `$r9 is available at cost 1`) has already taken `r9`, so `pad`
takes `r10` (`sl`) instead and the rest of the allocation rotates anyway.
The queue is a whack-a-mole: every source-visible way of moving `pad` ahead
of the eviction lets some other interval take `r9` first.

## 4. Source sweep (576 variants, no improvement)

Harness: generate the mode==0 clamp block, compile the TU with the exact NDK
r18b command line, gold-link the full `.so` with the checked-in link command
and score with the official `verify.py` internals (`/dev/shm/w6744/gen9.py`,
`linkvar.sh`, `scorevar.py`, `rawscore.py`).

The sweep crosses:

* clamp RHS: `(width + pad*2)`, `(width + pad3*2)`,
  `(width + pad3 + pad3)`, `(width + pad + pad)`, `(pad*2 + width)`,
  `(pad3*2 + width)`, `width - ... - pad*2`, `width - ... - pad3*2`;
* condition operand order: `pad+x+width`, `x+pad+width`, `pad+width+x`,
  `width+pad+x`;
* `x` declaration: `int x = centerX;`, `int x; x = centerX;`,
  `int x(centerX);`;
* `short pad3` absent / `= pad` / `= (short) pad`;
* `lineHeight` before vs after the clamp.

Result: the maximum raw score over all 576 is **95.67604667124228** — byte
identical in score to the committed HEAD. Bucket counts: 95.7 (36), 95.5
(12), 95.2 (72), 94.0 (72), 93.1 (12), 92.4 (24), 91.7 (12), 91.6 (36),
91.4 (12). No variant reaches `pad -> r9` except the `boxW` family, which
stays at 72.6 % / 1968 B because `pad2` lands in `fp` and `top` spills.

Additional one-off probes, all `pad = fp`, `x = r9`, 1948 B, 95.676047:

* `top` as `(float)(pad * 2)`, `(float)(pad << 1)`, `(float)(2 * pad)`,
  `(float)bottomRight.y - ...`, `... + 0.0f`;
* `int pad = layoutMetric(Globals::layout, 0x94);`,
  `*(int *)((char *) Globals::layout + 0x94)`,
  `__builtin_annotation(field, "pad")`;
* `Layout *layout` hoisted before `pad`, `int inset` local,
  `Globals::layout` inline in the clamp;
* 12 permutations of `pad` / `SetColor` / `centerX` / `top` (the scheduler
  normalises them all back to the same machine order);
* `__builtin_expect` on either clamp condition (8 combinations).

`layout->field_0x94` inline in the drawBox width gives `pad = r8`, `x = r2`,
1960 B; `pad + pad + width` in the drawBox gives `pad = fp`, `x = r6`,
1960 B. Both are worse.

## 5. Conclusion

The clamp and the join are fully explained, and the literal pool is already
correct (wave 119/w6714). The remaining gap is a single greedy-RA ordering
constraint: `pad` must occupy `r9` before `%45` evicts `x` from `r5`, which
requires `pad` to outrank `%45`/`%46` in the priority queue while still
losing to `x`. No stock-clang source shape found in 576 measured variants
plus the earlier ~900 achieves it; every shape that reaches `pad -> r9`
(`boxW` and friends) rotates `pad2` into `fp` and spills `top`, and every
shape that avoids the spill leaves `pad` in `fp`. The committed HEAD
(`short pad3`, **95.67604667124228 %**) remains the best measured shape.

## 6. Structural state (unchanged, re-verified)

27 sections; file size **2,187,328 B** exact; `.ARM.extab` 0x26290 (8 B short,
createShip only). No section/extab change from this pass.
