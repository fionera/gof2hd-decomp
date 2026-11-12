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
