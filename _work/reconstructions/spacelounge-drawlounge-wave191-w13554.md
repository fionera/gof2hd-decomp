# SpaceLounge::drawLounge — wave-191 source-shape sweep (w13554)

Status: research note (2026-11-13). Worktree `wt/w13554`, branch
`wave191b-w13554`, base `817cf1059`. **No `src/` change landed**: every
size-exact shape tested keeps the committed raw/aligned score. The tree is
clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**
* after: unchanged (note only)

Prior work: `spacelounge-drawlounge-wave190-w13472.md` (landed the 175 -> 138
`DrawRectangle` pad reload) and the notes it cites. This note supersedes
nothing; it records ~120 new measured cells on the *post-w13472* base.

## 1. Residual is still one register assignment

`tools/fn_diff.py` gives 138/87/1948/739. Byte runs (36 runs, 138 B):

| offset run | len | what |
|---|---:|---|
| 0x02f..0x262 (16 runs) | 19 | conversation-mode block: `sl`/`fp` role swap |
| 0x189, 0x19c, 0x1a5 | 4 | `ldr.w pad` / `mov.w pad2, pad, lsl #1` reg fields |
| 0x21b..0x262 | 7 | width-add block reg fields |
| 0x57d..0x5a5 | 8 | clamp condition reg fields |
| 0x5a6..0x5c7 | 33 | clamp right arm (4 insns vs the original's 2) |
| 0x5c8..0x5e0 | 20 | join (`x` whole in r9 vs `r5 -> r3 -> r9`) |
| 0x5f6..0x60d | 20 | `DrawRectangle` arg setup |
| 0x720..0x76c | 36 | EH cleanup landing pads (one extra bare `b.n`) |
| 0x78c | 1 | literal-pool branch offset |

```
value        original        HEAD (138)
pad          r9              sl (r10)
pad*2 (sl)   r10             fp (r11)
x            r5 -> r3 -> r9  r9 (whole)
top          r2 -> r8        r1 -> fp
```

Getting `pad` into `r9` would fix **every** run above at once (the whole
residual is downstream of that one colouring). It is a greedy-RA fact, not a
source-shape fact: `%610` (x, `COPY %385:spr`) carries the physical-hint bit
(`prio 0xE000108C`) because `VRM->hasKnownPreference(%610)` is true, while
`%2` (pad, a bare `t2LDRi12`) is `0xA0001900`. x is therefore always selected
before pad; `%45` (`top - pad`, prio `0xE...`) then evicts x from r5 (r5 has
`CostPerUse=0`, r9 has 1), and the re-enqueued x takes the free r9 whole.

## 2. Instrumented-compiler evidence (new)

`/tmp/llvm_android_702b/build2/bin/clang++` has a `NOHINT=<rawvreg>` knob
(`raNoHint`) in addition to the `HINTVREG`/`PRIOVREG`/`NOEVICT` knobs. With
`NOHINT=2147484258` (`%610`, x) the ENQ priority drops to `0xA000108C` and
**pad is selected before x** -- but pad still lands in `r10`, because `r9` is
then held by `%368` (the GOT-relative `&Globals::layout` address temp,
w=1.25e-4). The resulting object is a different colouring (width moves to r5,
etc.) that still measures **138/87/1948**. So "pad selected first" alone is
not sufficient; `%368` must also be out of r9, and no source shape tested
moves it.

`HINTVREG=2147483650:75` (pad -> r9) sets pad's bit30 (`prio 0xE0001900`) and
makes pad the first callee-saved selection, but it then takes `$r5`, then
`$r8`, then spills -- still 138/87/1948.

## 3. New cells measured this session (all size 1948 unless noted)

Base = 138/87/1948. Every row was built with the real `link_so` target and
scored with `tools/fn_diff.py`.

| variant | raw | aligned |
|---|---:|---:|
| base | **138** | **87** |
| `int textX = x;` used in DrawString (also `x+0`, `x*1`, `-(-x)`, `x\|0`) | 138 | 87 |
| `int textX` used in DrawString + DrawRectangle | 138 | 87 |
| `int left = x - pad; int above = top - pad;` used in drawBox (also in rect) | 138 | 87 |
| `int above = top - pad;` used in drawBox | 138 | 87 |
| `int pad2 = pad + pad;` used in top only / top+box | 138 | 87 |
| `int pad2 = pad + pad;` used in top+box+arm | 1553 | 167 (1960) |
| `int pad2 = pad + pad;` used in top+arm | 1553 | 167 (1960) |
| `int pad2 = pad;` used in arm / top / all | 138 | 87 |
| `int pad = field + 0` / `(int)(float)field` | 138 / 1881 | 87 / 246 (1984) |
| top before centerX / after pad / before centerX | 138 | 87 |
| `float centerXf; int centerX = (int)centerXf;` | 138 | 87 |
| `int x = centerX + 0` / `* 1` / `long x` | 138 | 87 |
| `unsigned x` | 139 | 88 |
| `int x = centerX;` moved after centerX / before width / before name | 138 | 87 |
| `int lineHeight` before centerX | 606 | 88 (1952) |
| `Layout *layout` const | 138 | 87 |
| `layout->drawBox` via `Globals::layout` | 138 | 87 |
| `lineHeight` via `Globals::layout` / inline in the call | 138 | 87 |
| `DrawString` top via local `layout` | 1875 | 157 (1976) |
| `DrawRectangle` x via local `layout` | 141 | 91 |
| `DrawRectangle` x via `(field)` / `+ -field` / `field*1` / `-field + x` | 138 | 87 |
| `DrawRectangle` x+y reload | 310 | 90 |
| clamp `int inset` / `int gw` / both | 138 | 87 |
| clamp `centerX` in place (no `x`) | 138 | 87 |
| clamp `centerX` in place + `-(width+pad+pad)` | 139 | 88 |
| clamp explicit `else x = centerX` / ternary phi | 138 | 87 |
| clamp cond uses `centerX` instead of `x` | 138 | 87 |
| `__builtin_expect(isKnown(), 2/3/4/7/8/16)` | 138 | 87 |
| `__builtin_expect(isKnown() != 0, 1)` | 138 | 87 |
| `__builtin_expect(isKnown(), 0)` / `!=0,0` / `==0,0` / `!isKnown(),0` | 1846 | 225 (1972) |
| plain `if (isKnown())` | 1872 | 183 (1976) |
| `__builtin_expect(hoverAgent < 0, 0)` | 977 | 281 |
| `__builtin_expect(mode == 0, 1)` | 1854 | 642 (1968) |
| `Vector` load order reversed | 138 | 87 |
| `Agent *agent` moved after the Vector loads / after pad | 138 | 87 |
| `long long pad` (any cast combination) | 1893-1902 | 203-259 (2000+) |
| `short pad` / `unsigned pad` | 1884 / 140 | 196 / 89 (1980) |
| top via `pad << 1` / `pad * 2` | 138 | 87 |
| box width via `pad + pad` | 138 | 87 |
| box width via `pad << 1` | 1869 | 192 (1976) |
| arm `-(pad << 1)` / `-(width + pad*2)` / any `pad*2` family | 1553 | 167 (1960) |
| arm `-(width + pad + pad)` | 139 | 88 |
| arm `+ (-width - pad - pad)` (committed) | **138** | **87** |
| arm `+ (-width - (pad + pad))` | 1553 | 167 (1960) |
| 7 arm forms x 6 condition sum orders (42 cells) | min 138 (flat/negpp) | 87 |
| `String(agent->isKnown() ? "  " : "")` | 1874 | 199 (1968) |
| `(agent->isKnown() ? name + String("  ") : name)` | 1892 | 376 (2004) |

## 4. Why the faithful arm cannot be used at 1948

The original arm is `itt gt; addgt.w r1, r6, sl; subgt r5, r0, r1`
(`x = (w - inset) - (width + pad2)`, pad2 = sl). Every source spelling that
makes clang reuse the existing `%380` pad2 vreg (`pad*2`, `pad<<1`,
`(pad+pad)`, named `pad2`) makes `%380` live across the clamp, and at -Oz it
is then spilled to `[sp,#16]` (`ldrgt r1, [sp,#16]`), growing the body to
1960. Only the two-independent-subtraction spelling `-width - pad - pad`
keeps `%380` dead in the arm (clang folds it to `0 - (pad<<1) - width`), and
that is what the committed source uses.

## 5. Conclusion

The committed flat clamp + `DrawRectangle` pad reload remains the best
size-exact shape: **raw 138 / aligned 87 / 1948 B / 739 insns**. The whole
138-byte residual is the single `pad = sl` (vs `r9`) colouring; it is blocked
by x's physical-hint priority bit (`%610` is a `COPY` from a vreg with a known
phys) plus the `%368` GOT-address temp occupying r9 once x is de-hinted. No
source shape tested (this note's ~120 cells plus the ~250 cells in the cited
wave-190 notes) moves pad into r9 at body size 1948.

## 6. Reproduction

```bash
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13554` (`base.cpp`,
`harness.py` + `v14.py`..`v34.py`, `gen2.py`/`run2.py`, `gen3.py`/`run3.py`,
`cc.sh`, `showdiff.py`, `ra2.log`, `hints.log`, `orig.asm`, `ours.asm`).
The instrumented clang is `/tmp/llvm_android_702b/build2/bin/clang++`
(`RA_TRACE`, `NOHINT`, `HINTVREG`, `PRIOVREG`, `NOEVICT`; vreg numbers are
`0x80000000 + index`).
