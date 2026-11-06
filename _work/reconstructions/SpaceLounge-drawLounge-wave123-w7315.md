# SpaceLounge::drawLounge — wave-123 pad-weight / draw-call shape re-check (w7315)

Status: research note (2026-11-06). Worktree `wt/w7315`, base `1c8c9b20e`.
**No `src/` change landed**: the committed HEAD is still the best measured shape.
The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior work: `spacelounge-drawlounge-wave122-w7217.md` (pad weight x1.5),
`-wave121-w7117.md` (RA order), `-wave119-w6953.md` (direct field / boxW),
`-wave119-w6927.md` (x split), `-wave119-w6744.md` (eviction order),
`-wave117-w6517.md` (292-variant sweep), `-wave113-w6117.md` (source-shape
sweep). This note supersedes nothing.

## 1. Residual (re-verified, unchanged)

Only the clamp / join `0x1712b2..0x171330` differs; every instruction before
`0x1712b2` is identical except the register that holds `pad`:

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

The original right arm is the faithful 8-byte
`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`; HEAD uses the `short pad3`
sign-extend workaround (`ittt gt; movgt.w r1,fp,lsl#16; addgt.w r1,r6,
r1,asr#15; subgt.w r9,r0,r1`).

## 2. New: exact weight threshold for `pad -> r9` (instrumented LLVM 7.0.2)

On the **faithful** source (`width + pad * 2`, no `pad3`), scaling only
`pad`'s spill weight (`VRW=2147483650:F`, `%2 = 0x80000002`) gives a sharp
transition:

| `F` | pad | pad2 | x | lineHeight | arm |
|---|---|---|---|---|---|
| 1.00–1.05 | fp | sl | r9 | fp | `itt` (8 B) |
| 1.10–1.40 | fp | sl | r9 | r8 | `itt` |
| **1.42–2.00** | **r9** | **sl** | **r5** | **r8** | `itt` |

So `pad -> r9` and the `r5 -> r2 -> r9` x split appear exactly when
`pad.weight > x.weight`, i.e. `2.078e-4 * 1.42 = 2.95e-4 > 2.946e-4`.
No `F` in `1.0..2.0` yields the original's `lineHeight -> fp` together with
`pad -> r9`: from `F = 1.10` upward `lineHeight` always lands in `r8`.

## 3. Why the target is a weight-order impossibility for this MIR

The faithful MIR weights (measured from `HINTDBG`/`select` traces) are:

```text
x         2.946140e-04   (hinted $r3, size 4220)
agent     2.267318e-04   (hinted,      size 8768)
pad       2.077779e-04   (unhinted,    size 6368)
lineHeight 5.978897e-04  (unhinted,    size 384)
```

The original needs three things at once:

1. `pad -> r9` and `x` split: requires `pad.weight > x.weight`.
2. `lineHeight -> fp` (not `r8`): at `%44`'s selection `fp` holds `pad` and
   `r8` holds `agent`; `shouldEvict` picks the cheaper evictee, so this
   requires `pad.weight < agent.weight`.
3. Therefore `agent > pad > x`.

But the MIR has `x (2.946e-4) > agent (2.267e-4)`, so (1)+(2) cannot hold
simultaneously. This is the precise form of the wave-121/122 "tension".

A 125-point grid over `VRW(pad) x VRW(x) x VRW(agent)` (pad 1.3–1.8,
x 0.7–1.3, agent 1.0–1.8) found **no** combination with
`pad=r9, pad2=sl, x=r5, lineHeight=fp`. Raising `agent` enough to flip
`lineHeight` to `fp` (factor > ~1.30) always rotates `pad` out of `r9`
(`pad -> r8` or `fp`, `x -> r2`). The allocator is non-monotonic here.

## 4. Draw-call shapes re-measured this wave (all <= 95.676047)

| variant | raw % | size | regs |
|---|---:|---:|---|
| **HEAD (`short pad3`)** | **95.676047** | **1948** | pad=fp, x=r9, lh=fp |
| faithful `width + pad*2` | 93.964335 | 1948 | pad=fp, x=r9, lh=fp, 729/729 |
| faithful + `pad2` local used everywhere | 93.964335 | 1948 | pad=fp, x=r9, lh=fp |
| `int boxW = width + pad*2` after clamp | 72.6 | 1968 | pad=r9, x=r5, pad2=fp, lh=r8, top spilled |
| `int boxW` before clamp | — | — | pad=fp, x=r9, boxW precomputed in sl |
| direct `layout->field_0x30_rowHeight` in both calls | 95.676047 | 1948 | identical code to HEAD (CSE) |
| `const int &pad` | 76.268861 | 1972 | pad=r3, pad2=r8, x=r5, lh=fp |
| `long long pad` | 78.178110 | 2008 | pad=r9, x=r5, pad2=r0, lh=r8 |
| `short pad` | — | — | pad=fp, pad2=r0, x=r5, lh=r8 |
| mixed `pad+pad` / `pad*2` / `pad<<1` forms | 95.676047 | 1948 | all CSE to one `MOVsi`, pad=fp |
| `centerX` used directly in clamp conds | 95.676047 | 1948 | pad=fp, x=r9 |
| `unsigned`/`long`/`double` width/centerX/top | 95.676047 | 1948 | pad=fp, x=r9 |
| `__builtin_expect` on name/DrawString isKnown (18 combos) | 95.676047 | 1948 | pad=fp, x=r9 |

The `boxW`-local shape is the only source shape that reaches `pad=r9` +
`x=r5`, but it rotates `pad2 -> fp`, `lineHeight -> r8` and spills `top`
(1968 B) because `Globals::Canvas` stays live in `sl` across the clamp in
that IR order (wave-117 §2). The faithful shape keeps `pad2 -> sl` but
`pad -> fp`.

## 5. Conclusion

The residual is a single greedy-RA split choice driven by the fixed MIR
weights. The faithful MIR has `x > agent`, so the two conditions the original
satisfies (`pad > x` for the r9 split, `pad < agent` for `lineHeight -> fp`)
are mutually exclusive under any monotone weight scaling. No source shape
found in waves 113–123 reproduces the original `pad=r9 / x=r5 / pad2=sl /
lineHeight=fp` together. The committed HEAD (`short pad3`,
**95.67604667124228 %**, 1948 B) remains the best measured shape.

## 6. Structural state (re-verified)

`check` = RATCHET PASS: byte 4506 / linked 4506, avg 99.94 %, data 843/843,
imports 207, lint CLEAN, 27 sections, file size 2,187,328 B exact.
