# SpaceLounge::drawLounge — wave-191b diff-cluster map + fresh shape sweep (w13620)

Status: research note (2026-11-13). Worktree `wt/w13620`, branch
`wave191b-w13620`, base `7301219a1`. **No `src/` change landed** — every
size-exact shape tested keeps or worsens the committed raw score. Tree is
clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**,
  `.ARM.extab` diff **577 B**
* after: unchanged (note only)

Prior work read first: `spacelounge-drawlounge-wave191b-w13566.md` and
`spacelounge-drawlounge-wave191-w13554.md` (and the wave-190 notes they cite).
This note adds a fresh contiguous byte-cluster map and ~180 new measured
cells; it supersedes nothing.

## 1. Fresh contiguous diff-cluster map (138 bytes, 36 runs)

Built with `tools/fn_diff.py`'s byte extraction + a contiguous-run clusterer
(`scratch/w13620/clusters.py`). Top clusters:

| offset | len | first differing insn (orig | ours) |
|---|---:|---|
| 0x5a6 | 33 | `addgt.w r1, r6, sl` | `movgt r1, #0` (4-insn arm vs 2) |
| 0x74d | 31 | `mov r4, r0` | `b.n 171490` (extra EH landing-pad branch) |
| 0x5c8 | 18 | `str.w r8, [sp, #12]` | `add r6, fp` (join / x split) |
| 0x600 | 10 | `ldr r0, [sp, #24]` | `ldr.w r1, [r1, #148]` (DrawRectangle args) |
| 0x5f6 | 9 | `mov fp, r8` | `ldr r0, [sp, #20]` (DrawRectangle args) |
| 0x58e | 3 | `add.w r5, r0, r9` | `add.w r9, r0, sl` (left clamp) |
| 0x2f | 2 | `ldr.w sl, [r0]` | `ldr.w fp, [r0]` |
| 0x19c | 2 | `mov.w sl, r9, lsl #1` | `mov.w fp, sl, lsl #1` |
| 0x23b | 2 | `mla fp, r2, r1, r3` | `mla sl, r2, r1, r3` |
| 0x5de | 2 | `mov r2, r6` | `movs r1, #2` |
| 0x49 | 1 | `ldrd fp, r8, [r4, #112]` | `ldrd sl, r8, [r4, #112]` |
| 0x60/0x6e/0x88/0x92 | 1 each | `sl` | `fp` (conversation branch) |
| 0x189/0x1a5/0x21b | 1 each | `r9`/`sl` | `sl`/`fp` |
| 0x57d/0x584/0x586/0x59c/0x59e/0x5a4 | 1 each | clamp reg fields |
| 0x60c | 1 | `mov r3, sl` | `mov r3, r6` |
| 0x720/0x726/0x73c/0x748/0x74a/0x78c | 1 each | branch targets / pool offset |

The map confirms the w13554 conclusion: **every cluster above 0x57d is
downstream of the single `pad = sl` (r10) vs `r9` colouring**, plus the
independent 2-byte EH landing-pad shift at 0x74c.

## 2. New evidence: the EH cluster is downstream of the code shift

Decoded the LSDA call-site table for drawLounge from `.ARM.extab`
(`scratch/w13620/lsda.py`). Both files have 25 call sites; the landing-pad
addresses are the same *function-relative* values in the original and ours
(e.g. entry 1 lp=0x74c in both). What differs is the *code at* those
addresses: ours has an extra bare `b.n` at 0x74c, so the sp+44 cleanup and
every later landing pad is shifted 2 bytes earlier. The call-site ranges
also differ from entry 14 on (e.g. orig `start=1460 len=8` vs ours
`start=1470 len=8`; orig `start=1522 len=44` vs ours `start=1524 len=38`).
So the 31-byte 0x74d cluster is a consequence of the same instruction-layout
difference, not an independently fixable EH-table bug.

## 3. Measured cells (all real link, `tools/fn_diff.py`)

Base = **138 / 87 / 1948 / 739** (raw/aligned/size/insn), `.ARM.extab` diff
577. Every row below was built with the real `link_so` target and scored with
`tools/fn_diff.py`; `.ARM.extab` diff is shown where it changed.

### 3.1 `left`/`above` source shape (the closest structural match)

The original disasm computes `x - pad` and `top - pad` once and reuses them
for both drawBox and DrawRectangle (`subs r6, r3, r1` / `sub.w r5, r2, r9`),
which is the `left/above` shape. Measured:

| variant | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| base | **138** | **87** | 1948 | 739 | 577 |
| `int left=x-pad; int above=top-pad;` used in drawBox+rect | 175 | **41** | 1948 | **741** | 580 |
| same, `-(width+pad+pad)` arm | 176 | 42 | 1948 | 741 | 580 |
| same, `left/above` in drawBox only | 138 | 87 | 1948 | 739 | 577 |
| same, `left/above` in rect only | 1664 | 223 | 1984 | 739 | 12221 |
| same, `above` only (both) | 138 | 87 | 1948 | 739 | 577 |
| same, declared before clamp | 1850 | 287 | 1956 | 741 | 1299 |
| same + `width += pad*2` in place | 175 | 41 | 1948 | 741 | 580 |
| same + `pad2` local (top+box+arm) | 445 | 54 | 1948 | 741 | 592 |

The `left/above` shape is almost certainly the original source shape
(741/741 instructions, aligned 41), but it moves `pad` from `sl` to `fp`
(r11) and `x` from `r9` whole to `r5 -> r9`, so raw rises 138 -> 175.

### 3.2 `pad2` / faithful-arm spellings (all size-exact unless noted)

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| `-(width+pad+pad)` arm | 139 | 88 | 1948 | 739 |
| `(w-inset)-(width+pad+pad)` | 139 | 88 | 1948 | 739 |
| `right`/`inset` locals | 138 | 87 | 1948 | 739 |
| `pad2 = pad+pad` used in arm only | 1553 | 167 | 1960 | 735 |
| `pad2` used in top+arm | 1553 | 167 | 1960 | 735 |
| `pad2` used in top+box+arm | 445 | 54 | 1948 | 741 |
| `pad2` declared before clamp / before x / after x / before width / before name / before top / inside else | 1553 | 167 | 1960 | 735 |
| `-(width+(pad<<1))`, `-(width+2*pad)` | 1553 | 167 | 1960 | 735 |
| `width += pad*2` in place (base shape) | 517 | 79 | 1952 | 741 |
| `width += pad+pad` in place (base shape) | 517 | 79 | 1952 | 741 |
| `x = w-inset-width-pad-pad` sequential | 468 | 64 | 1944 | 740 |
| arm reloads `layout->field_0x94` | 159 | 89 | 1948 | 739 |

Every spelling that makes clang keep `pad2` live to the arm either spills it
to `[sp,#16]` (1960 B) or changes the whole colouring. The 2-instruction arm
is reachable (`addgt.w r1, r6, sl; subgt.w r9, r0, r1`) only with a different
colouring that scores worse overall.

### 3.3 `layout` declaration / `Globals::layout` use

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| `Layout *layout` before pad | 943 | 161 | 1948 | 738 |
| `layout` at branch top | 943 | 161 | 1948 | 738 |
| `layout` at function start | 1129 | 188 | 1944 | 737 |
| `layout` before width | 1475 | 166 | 1972 | 745 |
| `layout` before name | 1531 | 182 | 1952 | 740 |
| replace later `Globals::layout` with local `layout` | 1825 | 200 | 1960 | 734 |
| no local `layout` at all | 138 | 87 | 1948 | 739 |
| clamp uses `Globals::layout` | 138 | 87 | 1948 | 739 |
| drawBox uses `Globals::layout` | 138 | 87 | 1948 | 739 |
| `layout0` for pad only | 138 | 87 | 1948 | 739 |

The `%368` GOT temp is CSE'd; none of these move it out of the way.

### 3.4 Branch hints / condition forms

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `__builtin_expect(isKnown(),1)` (committed) | 138 | 87 | 1948 |
| `__builtin_expect(isKnown(),2/3/7/8/16)` | 138 | 87 | 1948 |
| plain `if (isKnown())` | 1872 | 183 | 1976 |
| `__builtin_expect(isKnown(),0)` | 1846 | 225 | 1972 |
| `__builtin_expect(x-pad<inset,0)` | 1863 | 176 | 1976 |
| `__builtin_expect(x-pad<inset,1..16)` | 541 | 113 | 1952 |
| `__builtin_expect(right-cond,0)` | 513 | 90 | 1952 |
| `__builtin_expect(right-cond,1..16)` | 1872 | 183 | 1976 |
| both clamp hints | 528-544 | 93-117 | 1956 |
| swap clamp conditions | 1871 | 202 | 1976 |
| nested `else { if }` | 138 | 87 | 1948 |
| bool locals for clamp | 138 | 87 | 1948 |
| `__builtin_expect(mode==0,0/1/...)` | 1653-1854 | 468-642 | 1940-1968 |
| `__builtin_expect(hoverAgent<0,0)` | 977 | 281 | 1948 |
| `__builtin_expect(hoverAgent<0,1/...)` | 1502 | 380 | 1944 |
| `__builtin_expect(mission!=0,0)` | 965 | 390 | 1948 |
| `__builtin_expect(name-cond,0)` | 1435 | 168 | 1952 |
| `if (hoverAgent >= 0) { body }` | 138 | 87 | 1948 |
| `if (hoverAgent == -1)` | 142 | 88 | 1948 |
| `if (hoverAgent <= -1)` | 138 | 87 | 1948 |
| `if (!mode)` / `if (0==mode)` | 138 | 87 | 1948 |

### 3.5 Store/teardown order, String temporaries, misc

| variant | raw | aligned | size |
|---|---:|---:|---:|
| reorder `{pad, SetColor, centerX, top}` (6 valid orders) | 138 or 1872 | 87 or 208 | 1948/1976 |
| `textY = top + field_0x2c0` local | 1858-1862 | 226-369 | 1964-1976 |
| SetColor before drawBox | 1888 | 218 | 1980 |
| SetColor(white) before DrawRectangle | 559 | 136 | 1952 |
| named `String spaces("  ")` | 920-1740 | 291-328 | 1960-2004 |
| named `String empty("")` / default | 1752-1753 | 362-368 | 2008-2016 |
| `role + String(" ") + String(" ")` | 1954 | 257 | 2056 |
| `String("  ") + role` | 140 | 89 | 1948 |
| `role + String("  ", false)` | 138 | 87 | 1948 |
| `role + String("  ", true)` | 139 | 88 | 1948 |
| `name + String(isKnown() ? "  " : "")` | 1874 | 199 | 1968 |
| `isKnown() ? name+String("  ") : name` | 1887 | 340 | 2004 |
| cache `offer` / `mission` locals | 1354-1893 | 213-580 | 1944-1972 |
| `(topLeft.x+bottomRight.x)*0.5f` | 145 | 92 | 1948 |
| `bottomRight.x-(bottomRight.x-topLeft.x)*0.5f` | 141 | 90 | 1948 |
| `topLeft.x-(topLeft.x-bottomRight.x)*0.5f` | 141 | 89 | 1948 |
| `(topLeft.x+bottomRight.x)/2.0f` | 145 | 92 | 1948 |
| `bottomRight.y - (float)pad*2.0f` | 1854 | 182 | 1972 |
| `bottomRight.y - (float)pad - (float)pad` | 1854 | 182 | 1972 |
| `bottomRight.y - (double)(pad+pad)` | 1483 | 153 | 1952 |
| `float centerXf/topf` locals | 138 | 87 | 1948 |
| `int pad2 = pad*2` for top | 138 | 87 | 1948 |
| `pad` reloads in clamp / arm / draw / top | 159-1853 | 89-183 | 1948-1964 |
| `SetColor(-1)` / hex-case variants | 138 | 87 | 1948 |

## 4. Instrumented-clang probe (L1 shape)

`/tmp/llvm_android_702b/build2/bin/clang++` (RA_TRACE/NOHINT/HINTVREG/
PRIOVREG/SIZEVREG/FORCESPLIT) was used only as an oracle; no source change
depends on it.

* L1 base: `pad -> r11 (tryEvict)`, `%47 (width+pad2) -> r10`, `x2 -> r9`.
* L1 + `NOHINT=%609`: `%47 -> r9`, `pad -> r10`, `x2 -> r11` — still not the
  original triple.
* L1 + `HINTVREG=2147483650:75`: `pad -> r9` confirmed, but the final body is
  still 175/41/1948 (x2 moves to r11 and the DrawString block recolours).
* L1 + `SIZEVREG`/`PRIOVREG` variants: unchanged 175/41/1948.

So `pad -> r9` alone is necessary but not sufficient in the L1 shape either;
the original's `pad r9 / %47 r10 / x2 r9` triple needs pad to be selected
before `%47` *and* x2 to reuse r9 after pad dies.

## 5. Conclusion

The committed flat clamp + `DrawRectangle` pad reload remains the best raw
shape: **raw 138 / aligned 87 / 1948 B / 739 insns / extab 577**. The
`left/above` shape is the best aligned shape (**41**) and the structural
match (741/741 insns), but it costs raw 138 -> 175. No tested size-exact
source shape improves raw below 138, and no shape improves both raw and
aligned. The residual remains the coupled RA fact described in w13554/w13566:
`pad` must win `r9`, `%47` must take `sl`, and `x` must split at the
`x - pad` use; no source-only spelling found here achieves all three.

## 6. Reproduction

```bash
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13620` (`harness.py`,
`sweep1.py`..`sweep26.py`, `clusters.py`, `sbs2.py`, `lsda.py`,
`cc_base.sh`, `icompile.sh`, `ra_base.log`, `ra_L1.log`, `ra_L1_nohint.log`,
`ra_hint.log`).
