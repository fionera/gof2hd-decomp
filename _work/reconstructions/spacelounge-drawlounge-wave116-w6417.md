# SpaceLounge::drawLounge — wave-116 IR-order / register probe (w6417)

Status: research note (2026-11-05). Worktree `wt/w6417`, branch
`wave116-w6417`, base `d672d349f`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `byte 4501 / linked 4501`, data 843/843, imports 207, lint CLEAN,
  drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave115-w6318.md` (branch-weight source
shape), `-wave114-w6218.md` (eviction weight), `-wave113-w6117.md`
(exhaustive source-shape sweep), `-wave112-w6015.md` (the 7:1 flag),
`-wave110-w5818.md` (RA trace). This note supersedes nothing.

## 1. The original clamp is the "faithful" `width + pad * 2`

The committed HEAD carries a `short pad3 = pad;` hack so the right clamp arm
emits the 16-bit sign-extend:

```text
HEAD right arm:  ittt gt; movgt.w r1, fp, lsl #16
                 addgt.w r1, r6, r1, asr #15
                 subgt.w r9, r0, r1                       (10 B)
original:        itt gt; addgt.w r1, r6, sl
                 subgt r5, r0, r1                         (8 B)
```

`sl` is the `pad * 2` value already materialised for `top` (`mov.w sl, r9,
lsl #1`), so the original source is the plain

```cpp
x = Globals::w - layout->buttonInsetX - (width + pad * 2);
```

and the compiler CSEs the right-arm `width + pad*2` with the `drawBox` 4th
argument.  The `short pad3` form only exists because our `pad` lands in `fp`
and the plain form then needs a second `add` at the join.  Measured:

| variant | raw % | size | frame | pad reg |
|---|---:|---:|---:|---|
| HEAD (`short pad3`) | **95.676047** | 1948 | 120 | fp |
| faithful `width + pad*2` | 93.964335 | 1948 | 120 | fp |
| faithful `width + pad + pad` | 95.473251 | 1948 | 120 | fp |
| `w - (inset + width + pad*2)` | 94.924554 | 1948 | 120 | fp |
| `w - inset - width - pad*2` | 93.964335 | 1948 | 120 | fp |

## 2. New finding: the IR order at the join is add-before-subs

The original join block computes the `drawBox` width *before* the two
subtractions:

```text
1712f4: vmov r2, s16      ; top
1712f8: mov  r3, r5       ; x copy (split)
1712fa: add  sl, r6       ; width + pad*2     <-- add FIRST
1712fc: mov  r1, r9       ; pad copy
1712fe: subs r6, r3, r1   ; x - pad
171300: str.w r8, [sp,#12]
171304: sub.w r5, r2, r9  ; top - pad
171308: mov  r9, r3       ; x -> r9
17130a: mov  r8, r2       ; top -> r8
```

Our HEAD IR has the opposite order (`sub x-pad`, `sub top-pad`, `add
width+pad2`), because the `drawBox` arguments are evaluated left-to-right and
`width + pad*2` is the 4th argument.  The only source shape that flips the IR
order is a separate local computed before the call:

```cpp
int boxW = width + pad * 2;   // after the clamp, before lineHeight
int lineHeight = layout->field_0x30_rowHeight;
layout->drawBox(2, x - pad, top - pad, boxW, lineHeight, String(""));
```

With that local the IR becomes `%bw = add; %bx = sub; %by = sub`, matching the
original's schedule, and **`pad` moves to `r9` and `x` to `r5`** — the two
registers the original uses.  This is the first source shape found (waves
106–115 missed it) that reaches `pad -> r9` without a compiler knob.

## 3. Why it still loses: the extra live range spills `top`

The `boxW` local is live across the `String("")` constructor in the IR (its
`add` is emitted before the ctor), so the greedy allocator keeps it in a
callee-saved register.  At the join every callee-saved register is then taken
(`r4 layout, r5 x, r6 width, r8 lineHeight, r9 pad, r10 top-pad, r11 pad2`)
and `top` is spilled to `[sp,#20]`; the stack frame grows 120 -> 128 and every
`sp`-relative access shifts by 8.  The original instead keeps `lineHeight` in
`r11` and `pad2` in `r10`, so `r8` is free for `top` after spilling the agent
(`str.w r8, [sp,#12]`).

Measured (all with the 7:1 per-file branch weight):

| variant | raw % | size | frame | pad | x | notes |
|---|---:|---:|---:|---|---|---|
| HEAD (`short pad3`) | **95.676047** | 1948 | 120 | fp | r9 | best |
| faithful clamp | 93.964335 | 1948 | 120 | fp | r9 | right arm matches |
| faithful + `boxW` local | 72.602740 | 1968 | 128 | **r9** | **r5** | top spilled |
| faithful + `boxW` + `boxX`/`boxY` | 72.602740 | 1968 | 128 | r9 | r5 | same |
| faithful + `boxW` + `boxY` early | 72.602740 | 1968 | 128 | r9 | r5 | same |
| faithful + `boxW` + `top` late | 82.223747 | 1948 | 120 | fp | r9 | top dies early |
| faithful + `boxW` + no `lineHeight` local | 72.602740 | 1968 | 128 | r9 | r5 | same |
| faithful + explicit `String empty("")` | 71.621622 | 2016 | 136 | r9 | r5 | worse |
| pad3 + `boxW = width + pad3*2` | 81.587953 | 1976 | 120 | fp | r9 | early alloc shifts |
| pad3 + `boxW` before clamp | 95.676047 | 1948 | 120 | fp | r9 | IR order unchanged |
| faithful + `boxW` before clamp | 89.759450 | 1948 | 120 | fp | r9 | one add only |
| faithful + `pad2 = pad*2` local | 93.964335 | 1948 | 120 | fp | r9 | same as faithful |
| faithful + `pad2` local + `boxW` | 72.602740 | 1968 | 128 | r9 | r5 | same |
| faithful + `top` two conversions | 70.628415 | 1968 | 128 | r5 | r5 | no pad2 interval |
| faithful + `boxW = width+pad+pad` | 70.853242 | 1968 | 128 | r5 | r5 | — |

The branch-weight ratio is irrelevant to this family: `1:1`, `3:1`, `5:3`,
`7:1`, `31:1` all score 72.602740 on the `boxW` shape.

## 4. Other shapes re-measured this wave (all <= 95.676047)

```text
variant                                             raw         size
`unsigned width`                                    95.676047   1948
`unsigned pad`                                      95.676047   1948
`unsigned pad` + `unsigned x`                       95.676047   1948
conditions use `centerX` (left/right/both)          95.676047   1948
`int x2 = x` after clamp, used in all 3 calls       95.676047   1948
`boxX`/`boxY`/`boxW` locals after clamp             95.676047   1948
`int boxW` before clamp + drawBox uses it           95.676047   1948
`short pad`                                         95.676047   1948
`int pad2 = pad*2` used for top/clamp/drawBox       93.964335   1948
`lineHeight` before clamp                           95.198903   1948
`top` before `centerX`                              95.676047   1948
pad loaded after SetColor / after centerX           92.93 / 95.68  1948
```

(The `95.676047` rows are byte-for-byte the HEAD allocation; the source edits
are coalesced/CSE'd away.)

## 5. Conclusion

`pad -> r9` is source-reachable (via the `boxW` local + faithful clamp), but
the same IR change forces `top` out of `r8` and adds an 8-byte stack slot, so
the official score drops from 95.7 to 72.6.  The remaining blocker is the
allocator's choice of `r11` for `pad2` (original: `r10`) in that shape, which
pushes `lineHeight` into `r8`.  Closing the function needs a source shape that
gets the add-before-subs IR order *and* keeps `pad2` in `r10`; none of the
~70 variants measured this wave does.

## 6. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN11SpaceLounge10drawLoungeEv
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w6417/score.py [path/to/libgof2hdaa.so]
```

Variant sources from this wave are under `/dev/shm/w6417/v*` (scratch only).
