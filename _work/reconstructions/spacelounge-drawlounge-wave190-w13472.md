# SpaceLounge::drawLounge — wave-190 raw win: DrawRectangle pad reload (w13472)

Status: **landed** (2026-11-12). Worktree `wt/w13472`, branch `wave190-w13472`,
base `2996b2315`.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* before: raw **175**, aligned **41**, size **1948/1948**, insn **741/741**
* after:  raw **138**, aligned **87**, size **1948/1948**, insn **739/741**
* `.text` diff **71,959 -> 71,922** (-37); `.ARM.extab` diff **578 -> 575** (-3);
  total text+extab+build-id **72,557 -> 72,517** (-40)
* gate: `RATCHET PASS` — byte 4507 / linked 4507, size 4524/4524, data 843/843,
  imports 207, stub/missing/extra 0, lint CLEAN (byte/linked counts unchanged;
  drawLounge is still not byte-exact, so the gate is neutral while `.text` shrinks)

Prior work: `spacelounge-drawlounge-wave190-w13464.md` and the notes it cites.
This note supersedes nothing; it adds one new source cell.

## 1. The change

`Globals::Canvas->DrawRectangle(x - pad, ...)` -> `DrawRectangle(x -
Globals::layout->field_0x94, ...)` (the second call only; the `drawBox` call and
the clamp still use the `int pad` local). Semantically identical (`pad` was
loaded from that field and nothing writes it), but the extra field load is a
second, short-lived pad vreg in the machine IR. That changes the greedy-RA
colouring of the whole clamp/join: the pad vreg no longer has to survive to the
DrawRectangle call, so its live range is shorter and the allocator picks a
different colouring.

## 2. New colouring (vs the wave-190 baseline)

```
value        original        HEAD (175)      landed (138)
x            r5 -> r3 -> r9  r9 (whole)      r9 (whole)
pad          r9              fp (r11)        sl (r10)
pad*2 (sl)   r10             r10             fp (r11)
lineHeight   fp (r11)        fp (r11)        sl (r10) after pad dies
top          r2 -> r8        r2 -> r8        r1 -> fp
```

The landed form is still not the original's `pad=r9, x=r5->r3->r9` split, but
the whole-function byte diff drops 175 -> 138 because the new colouring agrees
with the original on more of the early (pre-clamp) instructions. The aligned
metric rises (41 -> 87) because the instruction stream now differs in more
places, but the raw same-address byte metric — the `.text` diff the gate
measures — improves by 37 bytes. `.ARM.extab` also drops 3 bytes of content
diff (the LSDA call-site/landing-pad offsets move with the new join).

## 3. Cells measured (all size 1948 unless noted)

| variant | raw | aligned |
|---|---:|---:|
| HEAD (local `pad` in both calls) | 175 | 41 |
| **DrawRectangle `x - Globals::layout->field_0x94`** | **138** | **87** |
| same via the local `layout` pointer (`layout->field_0x94`) | 141 | 91 |
| same + `top - Globals::layout->field_0x94` | 310 | 90 |
| same + `Globals::layout->field_0x94 * 2 + width` | 523 (size 1956) | 82 |
| same + faithful right arm `-(width + pad + pad)` | 139 | 88 |
| same + `int above = top - pad` join local | 138 | 87 |
| `drawBox` x reload instead (rect keeps `pad`) | 1883 | 163 |
| `drawBox` top reload | 1873 | 168 |
| `drawBox` width reload | 1880 | 243 |
| rect lineHeight reload (`layout->field_0x30_rowHeight`) | 142 | 93 |
| `int p2 = Globals::layout->field_0x94;` before the rect | 1581 (size 1972) | 216 |
| `int above = top - pad;` after the clamp (no reload) | 265 | 50 |
| `int above = top - pad;` before the clamp | 1853 (size 1968) | 258 |

The reload must be *inline in the call* (no named local): a named `p2` local
makes the value long-lived and grows the body to 1972.

## 4. Reproduction

```bash
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13472` (`sweep.py`, `b1.py`..
`b16.py`, `runs.py`, `cc.sh`, `score.py`).

## 5. Follow-up cells (same session, after landing 138)

All measured with the real `link_so` build (`tools/fn_diff.py`), landed body as
the base, size 1948 unless noted. None beat 138; they are recorded so the next
worker does not re-run them.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **landed (rect x reload)** | **138** | **87** | **1948** |
| rect x + rect y both reload (`top - Globals::layout->field_0x94`) | 310 | 90 | 1948 |
| rect y reload only (rect x = `x - pad`) | 1412 (obj proxy) | – | 1964 |
| rect lineHeight via local `layout` | 142 | 93 | 1948 |
| `int above = top - pad;` after lineHeight (both calls use `above`) | 138 | 87 | 1948 |
| `int above` + rect y reload | 138 | 87 | 1948 |
| `int left = x - pad;` for both calls (no field reload) | 175 | 41 | 1948 |
| `int left` + `int above` locals | 175 | 41 | 1948 |
| `int boxW = pad * 2 + width;` | 517 | 79 | 1952 |
| `int pad` moved after `centerX` | 1872 | 208 | 1976 |
| `int x = centerX;` moved before `width` | 193 | 87 | 1952 |
| `int x = centerX;` moved right after `centerX` | 138 | 87 | 1948 |
| `int x = centerX + 0;` | 138 | 87 | 1948 |
| `int x = <recomputed centerX expression>` | 1087 | 158 | 1948 |
| `int top = ... (pad * 2)` | 138 | 87 | 1948 |
| `int pad2 = pad + pad; int top = ... pad2` (named local) | 138 | 87 | 1948 |
| named `pad2` used in the right arm too | 1553 | 167 | 1960 |
| right arm `-(width + pad + pad)` | 139 | 88 | 1948 |
| right arm `w - inset - width - pad - pad` | 481 | 100 | **1944** (fatal) |
| right arm `w - (inset + width + pad + pad)` | 533 | 90 | 1952 |
| right arm `w - inset + (-(width + pad + pad))` | 139 | 88 | 1948 |
| clamp condition `inset > x - pad` | 140 | 89 | 1948 |
| clamp condition `w - inset < pad + x + width` | 139 | 88 | 1948 |
| clamp condition `x - Globals::layout->field_0x94` (reload) | 545 | 91 | 1952 |
| then-arm `inset + Globals::layout->field_0x94` (reload) | 141 | 87 | 1948 |
| right arm both pads reloaded from `Globals::layout` | 159 | 89 | 1948 |
| drawBox width `Globals::layout->field_0x94 * 2 + width` | 1891 | 254 | 1980 |
| drawBox x reload (`x - Globals::layout->field_0x94`) | 1883 | 163 | 1948 |
| drawBox y reload (`top - Globals::layout->field_0x94`) | 1873 | 168 | 1948 |
| top computed with two field reloads | 1538 | 177 | 1964 |
| `__builtin_expect(this->mode == 0, 1)` | 1854 | 642 | 1968 |
| `__builtin_expect(this->hoverAgent < 0, 0)` | 977 | 281 | 1948 |
| plain `if (agent->isKnown())` (drop the expect anchor) | 1872 | 183 | 1976 |

### Why 138 is a local optimum

The residual 138 bytes (linked, same-address) break down as: early-block
Canvas/agentRects register roles 20, pad/pad2 load+mov 4, x `vmov` 1, clamp
condition 8, clamp right arm 33, join 40, epilogue/landing pads 36 (roughly).

Forcing the original's `pad=r9, x=r5` colouring (rect x+y reload) is possible
and does reproduce the original's `ldr.w r9,[r6,#148]`/`mov.w sl,r9,lsl#1`/
`vmov r5,s18` exactly, but the body then diverges in the join and in the
conversation-mode block, so raw rises to 310.  The original's right arm
(`addgt.w r1, r6, sl; subgt r5, r0, r1`) needs `pad2` as a single reused value;
every source spelling that makes clang emit that pair either grows the body
(`pad2` local -> 1960, `2 * pad` -> 1960) or shrinks it (`- width - pad - pad`
-> 1944), so none is gate-safe.  The +1 instruction in the landing-pad block
(ours has an extra branch-only pad at `0x171484`) is an EH call-site-table
difference and is not reachable from the mode-0 statement order.

Scratch: `scratch/w13472/landed_src.cpp`, `body_landed.txt`, `sweepL.py`,
`evalbody.py`, `scoreobj.py`, `runs.py`.
