# SpaceLounge::drawLounge — wave-119 direct-field / boxW re-check (w6953)

Status: research note (2026-11-06). Worktree `wt/w6953`, branch
`wave119-w6953`, base `db1106935`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4505 / linked 4505, data 843/843, imports 207,
  lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave119-w6941.md` (pad-weight/split),
`-wave119-w6927.md` (x-split/eviction), `-wave119-w6744.md` (eviction order),
`-wave117-w6517.md` (292-variant sweep), `-wave113-w6117.md` (exhaustive
source-shape sweep), `-w103-oracle.md` (Unity oracle). This note supersedes
nothing.

## 1. Residual (unchanged)

Only the clamp / join `0x1712b2..0x171330` differs:

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

The committed `short pad3` right arm is a scoring workaround: the faithful
`width + pad * 2` arm is the original 8-byte `itt gt; addgt.w r1,r6,sl;
subgt r5,r0,r1`, but without the x-split its join is 4 B short and scores
**94.0** (measured this wave) vs HEAD's 95.676.

## 2. New result: the direct-field shape reaches pad=r9 / x=r5

`faithful clamp + Globals::layout->field_0x94 in DrawRectangle` (all three
args) gives the target's clamp colouring exactly:

```text
170ebe: ldr.w r9, [r6, #148]   ; pad = r9
1712b2: vmov  r5, s18          ; x   = r5
1712bc: sub.w r1, r5, r9       ; x - pad
1712c4: add.w r5, r0, r9       ; x = inset + pad
1712d4: add.w r1, r9, r5       ; pad + x
1712dc: itt   gt
1712de: addgt.w r1, r6, sl     ; width + pad*2  (8-byte arm, sl = pad2)
1712e2: subgt r5, r0, r1
```

The DrawRectangle reload shortens pad's live range and the compiler recomputes
x-pad/top-pad directly into the argument registers (no split).  But the extra
live value flips **Globals::Canvas from `sl` to `fp`** for the whole function,
so the conversation branch (`0x170d64`, `ldr.w sl,[r0]` → `ldr.w fp,[r0]`)
also changes.  Measured **89.7 %**, 1944 B, 723 insns.  All 8 subsets of the
direct field were already measured in `-w4227.md`; the best is (x-pad, top-pad
direct) = 89.6 %.

## 3. New result: the boxW shape reaches pad=r9 / x=r5 but spills top

`faithful clamp + int boxW = width + pad * 2` after the clamp:

```text
170ec6: ldr.w r9, [r6, #148]   ; pad = r9
1712b2: vmov  r5, s18          ; x   = r5
1712dc: itt   gt
1712de: addgt.w r1, r6, fp     ; width + pad*2  (fp = pad2, not sl)
...
5d0: vmov r1, s16              ; top -> r1
5d4: add  r6, fp               ; boxW = width + pad2(fp)
5d6: str  r1, [sp, #20]        ; top SPILLED
5d8: sub.w sl, r1, r9          ; top - pad -> sl
5de: mov  r9, r5               ; x -> r9 (split copy present)
```

pad=r9 and x=r5 are correct (and the split copy appears), but `pad2` lands in
`fp` instead of `sl`, which pushes `lineHeight` to `r8` and forces `top` to the
stack: frame 120 → 128, **1968 B**, **72.6 %**.  The register rotation is
`pad2: fp (want sl)`, `lineHeight: r8 (want fp)`, `top: spill (want r8)`.
The reason `pad2` does not get `sl` is that `top - pad` (`%44`, hinted) is
selected before `pad2` (`%381`, unhinted) and takes `sl`.  This is the same
`-stress-regalloc=12` trade-off recorded in `-w4018.md §6`.

## 4. Measurements this wave (~40 variants, all <= 95.676047)

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6` (~1.2 s), then the official
`verify.py` internals (`/dev/shm/w6953/fastscore.py`, `autojunk=False`).

| variant | raw % | size | note |
|---|---:|---:|---|
| **HEAD (`short pad3`)** | **95.676047** | **1948** | 729/728 |
| faithful `width + pad*2` | 94.000000 | 1948 | 729/729, 8-byte arm |
| faithful + `unsigned pad` | 93.700000 | 1948 | 729/729 |
| faithful + `long pad` / `unsigned top` / `long top` / `const int top` | 94.000000 | 1948 | 729/729 |
| faithful + `unsigned x` | 93.700000 | 1948 | 729/729 |
| faithful + `int drawX = x;` before/after `lineHeight` | 94.000000 | 1948 | coalesced |
| faithful + `int boxY = top - pad;` (both calls) | 92.300000 | 1944 | pad=r5, x=r9 |
| faithful + `int boxY = top; boxY -= pad;` | 92.300000 | 1944 | pad=r5, x=r9 |
| HEAD + `int boxY = top - pad;` / `boxY -= pad` | 93.100000 | 1948 | — |
| faithful + `int pad2 = pad*2` + `boxW = width + pad2` | 72.600000 | 1968 | top spilled |
| faithful + `boxW` + `lineHeight` before `boxW` | 72.600000 | 1968 | top spilled |
| faithful + `boxW` + `float topF` | 79.300000 | 1968 | top spilled |
| HEAD + `boxW` (local / pad2) | 71.1–71.2 | 1964–1972 | top spilled |
| faithful + direct field in DrawRectangle (all 3) | 89.700000 | 1944 | Canvas sl→fp |
| HEAD + direct field in DrawRectangle (all 3) | 89.500000 | 1952 | Canvas sl→fp |
| faithful + direct field x-pad only | 89.900000 | 1940 | Canvas sl→fp |
| faithful + direct field boxW only | 72.100000 | 1972 | — |
| faithful + `int screenW`/`int inset` locals | 94.000000 | 1948 | rematerialised |
| faithful + `int textDy = layout->field_0x2c0` | 94.000000 | 1948 | rematerialised |
| faithful + `top + -pad` / `top - pad*1` | 94.000000 | 1948 | — |
| faithful + `(int)(top - (float)pad)` | 91.000000 | 1964 | — |

A 16-variant systematic sweep over {arm: head/faith/pad2} × {x decl:
init/inplace/uninit} × {boxW: none/local/pad2} × {lineHeight: after/before}
also left HEAD on top (95.7); the best alternative was faithful +
`lineHeight` before the clamp at 95.2.

## 5. Conclusion

The residual is still the single greedy-RA fact recorded in waves 110–119:
`pad` is a plain unhinted `t2LDRi12` (no COPY in the MIR), so it is selected
after every `0xE`-priority interval, by which time `x` occupies `r9`.  The
target needs `x` to keep `r5` and split at the join (`r5 -> r3 -> r9`) so
`pad` can take `r9`:

* the **boxW** shape reaches `pad=r9`/`x=r5` but rotates `pad2` to `fp` and
  spills `top` (1968 B, 72.6 %);
* the **direct-field** shape reaches `pad=r9`/`x=r5` but flips
  `Globals::Canvas` from `sl` to `fp` in the conversation branch (1944 B,
  89.7 %);
* every other shape is the HEAD allocation.

No source-level shape measured this wave (or in the ~1700 variants of waves
110–119) produces `pad=r9`, `x=r5 -> r3 -> r9`, `pad2=sl`, `lineHeight=fp`
and `top=r8` simultaneously.  The committed HEAD (`short pad3`,
**95.67604667124228 %**, 1948 B) remains the best measured shape.

## 6. Structural state (unchanged, re-verified)

`check` = RATCHET PASS: byte 4505 / linked 4505, data 843/843, imports 207,
lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact.  No
section/extab change from this pass.

## 7. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w6953/fastscore.py _ZN11SpaceLounge10drawLoungeEv
```
