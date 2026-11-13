# SpaceLounge::drawLounge — wave-191b RA-priority experiments (w13566)

Status: research note (2026-11-13). Worktree `wt/w13566`, branch
`wave191b-w13566`, base `113a6f7a0`. **No `src/` change landed** — every
size-exact shape tested keeps or worsens the committed score. Tree is clean
apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**
* after: unchanged (note only)

Prior work: `spacelounge-drawlounge-wave191-w13554.md` (and the wave-190 note
it cites). This note records the follow-up experiments with two new knobs
added to the scratch clang (`SIZEVREG`, `FORCESPLIT`) and a new conclusion:
**moving `pad` to `r9` is necessary but not sufficient.**

## 1. The original also splits `x` (new)

Instruction-level diff of the committed base vs the original shows the
original carries **two extra instructions** in the overview branch that the
committed source does not produce:

```
1712f8: mov  r3, r5      ; x -> r3        (split copy #1)
1712fc: mov  r1, r9      ; pad -> r1
1712fe: subs r6, r3, r1  ; x - pad
171304: sub.w r5, r2, r9 ; top - pad
171308: mov  r9, r3      ; x -> r9        (split copy #2)
```

i.e. the original's live ranges are

| value | original | committed base |
|---|---|---|
| pad | **r9** | sl (r10) |
| pad2 | **r10** | fp (r11) |
| x | **r5 -> r3 -> r9** | r9 (whole) |
| top | r2 -> r8 | r1 -> fp |

`x` is split at the `x - pad` use and again before the calls; `pad` and `x2`
share `r9` because pad dies at `sub.w r5, r2, r9` (171304) and `x2` is born at
`mov r9, r3` (171308). `x1` and `%45` (`top - pad`) share `r5` for the same
reason. That perfect packing is why the original's whole function matches
once `pad` is in `r9`; the split is not optional.

## 2. `pad -> r9` alone is not enough (new)

Instrumented clang at `/tmp/llvm_android_702b/build2/bin/clang++` gained two
knobs (both in `lib/CodeGen/RegAllocGreedy.cpp`):

* `SIZEVREG=<rawvreg>:<size>` — override the live-range size used for the
  enqueue priority (`Prio = 0xA0000000 | Size` for unhinted ranges).
* `FORCESPLIT=<rawvreg>` — skip `tryAssign`/`tryEvict` for that vreg and go
  straight to `trySplit` (used to probe what a split of `x` would look like).

vreg numbers: `%2`=2147483650 (pad), `%45`=2147483693 (top-pad),
`%364`=2147484012 (&Globals::Canvas GOT), `%368`=2147484016
(&Globals::layout GOT), `%380`=2147484028 (pad*2), `%610`=2147484258 (x).

Measured cells (real link, `tools/fn_diff.py`):

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | **138** | **87** | 1948 | 739 |
| `SIZEVREG=%368:6000` | 138 | 87 | 1948 | 739 |
| `SIZEVREG=%368:6000,%364:6000` | 138 | 87 | 1948 | 739 |
| `SIZEVREG=%368:6000,%364:6000 NOHINT=%610` | **152** | 107 | 1948 | 739 |
| `SIZEVREG=%368:6000,%364:6000 NOHINTCO=%610` | **152** | 107 | 1948 | 739 |
| `SIZEVREG=... FORCESPLIT=%610 NOHINT=%610` | 272 | 126 | 1948 | 739 |
| `HINTVREG=%2:75` (pad -> r9) | 1872 | 222 | 1984 | 738 |
| `HINTVREG=%2:75 NOHINTCO=%610` | 1861 | 198 | 1968 | 741 |
| `HINTVREG=%2:75,%380:76 SIZEVREG=...` | 1849 | 167 | 1964 | 740 |
| `PRIOVREG=%2 NOHINT=%610` | 287 | 136 | 1948 | 739 |
| `PRIOVREG=%2 NOHINTCO=%610` | 287 | 136 | 1948 | 739 |

Key results:

* With `x` hinted (base), the GOT temps' priority is irrelevant: `x` is
  selected first, evicted from `r5` by `%45`, and the re-enqueued `x` takes
  `r9` whole; `pad` -> `r10`. Lowering `%368`/`%364` below `pad` changes
  nothing (138/87).
* De-hinting `x` (`NOHINT`/`NOHINTCO`) and lowering both GOT temps below
  `pad` gives **`pad -> r9` and `pad2 -> r10`** — the original's register
  pair — but `x` then takes `r10` (evicting `%368`) instead of being split.
  Score **152/107**, i.e. worse than the base.
* `HINTVREG=%2:75` also lands `pad` in `r9` first, but `pad` is later
  evicted/spilled and the body grows (1968-1984).
* `FORCESPLIT=%610` does split `x`, but the split point chosen by
  `trySplit` is not the `x - pad` use, and the result (272/126) is worse.

So the residual is **two** coupled RA facts, not one: `pad` must win `r9`
*and* `x` must be split at the `x - pad` use so that `x1`/`%45` share `r5`
and `pad`/`x2` share `r9`. Every shape that produces the first (de-hinting
`x`) loses the second.

## 3. Source shapes re-tested (all size-exact unless noted)

With the corrected base (the wave-191 note's `faithful arm` row is the
`-(width + pad + pad)` spelling, which still emits the 4-instruction `itttt`
arm — it is 139/88, not 2 instructions):

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | **138** | **87** | 1948 | 739 |
| `-(width + pad + pad)` | 139 | 88 | 1948 | 739 |
| `int left = x - pad; int above = top - pad;` reused in drawBox+rect | 175 | **41** | 1948 | **741** |
| same + `-(width + pad + pad)` | 176 | 42 | 1948 | 741 |
| `int left/above` + `int pad2 = pad + pad` | 1851 | 249 | 1964 | 743 |
| DrawString top via local `layout` (both) | 1104 | 188 | 1948 | 736 |
| DrawString top #1 via local `layout` | 1875 | 157 | 1976 | 741 |
| DrawRectangle x via local `layout` | 141 | 91 | 1948 | 739 |
| `layout->drawBox` via `Globals::layout` | 138 | 87 | 1948 | 739 |
| 12 valid orders of `{pad, SetColor, centerX, top}` | 138 or 1872 | 87 or 208 | 1948/1976 | 739/744 |

The `left/above` shape is the closest structural match (741/741
instructions, aligned 41) and is almost certainly the original's source
shape, but it moves `pad` to `fp` (r11) and `x` to `r9` and therefore scores
raw 175. It is **not** a raw win.

## 4. Conclusion

The committed flat clamp + `DrawRectangle` pad reload remains the best
size-exact shape: **raw 138 / aligned 87 / 1948 B / 739 insns**. The
remaining gap needs a source shape that (a) de-hints `x` relative to `pad`
and (b) still makes the allocator split `x` at the `x - pad` use. No shape
tested (this note's ~40 cells plus the ~400 cells in the two cited notes)
does both. `SIZEVREG`/`FORCESPLIT` are available in the scratch clang for
the next attempt.

## 5. Reproduction

```bash
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13566` (`ilink.sh`,
`ilink_t.sh`, `sweep.py`, `sdiff.py`, `v30.py`..`v34.py`, `gen4.py`,
`gen5.py`, `ra_*.log`). Instrumented clang:
`/tmp/llvm_android_702b/build2/bin/clang++` (`RA_TRACE`, `NOHINT`,
`NOHINTCO`, `HINTVREG`, `PRIOVREG`, `NOEVICT`, `SIZEVREG`, `FORCESPLIT`;
vreg numbers are `0x80000000 + index`).
