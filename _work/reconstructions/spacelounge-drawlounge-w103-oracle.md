# SpaceLounge::drawLounge — wave-103 oracle cross-check (wf317)

Status: research note (2026-11-04). Worktree `wt/wf317`, branch
`wave103-wf317`, base `1d2b4398e`. **No `src/` change landed**: every source
shape tested keeps the official score at 95.4733 (83 structural `|` lines,
1948 B). The tree is clean.

Gate (`cmake --build cmake-build-match --target check -j8`) before and after:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11SpaceLounge10drawLoungeEv`: `osize = usize = 1948`, `byte = linked =
false`, official `pct 95.5` (raw `difflib` ratio **95.4733**), 83 structural
`|` lines in `tools/verify/verify.py --show`.

Prior work (all exhaustive): `spacelounge-drawlounge-w5016.md` (newest),
`-w4818.md`, `-w4519.md`, `-w4246.md`, `-w4227.md`, `-w4118.md`, `-w4018.md`,
`-w3838.md`, `-w3818.md`, `branch-weight-overrides_w4218.md`. This note only
records the wave-103 oracle cross-check and re-measurement.

## 1. Wave-103 oracle

New oracle: `_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/
native/game/SpaceLounge.c` (Ghidra 12.1.4 of the KiritoJPK NDK-r14/clang-3.8
-O2 mod rebuild, image base 0x10000). `SpaceLounge::drawLounge` is at
`@0x0019f53c` (1878 B). Its mode-0 name-plate block is:

```c
iVar7  = *(int *)(Globals::layout + 0x94);          /* pad */
...
iVar12 = iVar7 * 2;                                  /* pad2, a real local */
fVar20 = (float)VectorSignedToFloat(iVar12, ...);
iVar22 = (int)(fVar23 - fVar20);                     /* top */
iVar11 = (int)(fVar24 + (fVar21 - fVar24) * 0.5);    /* centerX */
...
iVar10 = *(int *)(Globals::layout + 0x28);           /* buttonInsetX */
if (iVar11 - iVar7 < iVar10) {
    iVar11 = iVar10 + iVar7;
}
else if (Globals::w - iVar10 < iVar11 + iVar7 + iVar16) {
    iVar11 = (Globals::w - iVar10) - (iVar16 + iVar12);
}
uVar15 = *(undefined4 *)(Globals::layout + 0x30);    /* lineHeight */
Layout::drawBox(pLVar3, 2, iVar11 - iVar7, iVar22 - iVar7,
                iVar12 + iVar16, uVar15, String(""));
...
PaintCanvas::DrawRectangle(Globals::Canvas, iVar11 - iVar7, iVar22 - iVar7,
                           iVar12 + iVar16, *(int *)(Globals::layout + 0x30));
```

The **draw order is identical to our source** (name -> role -> width ->
isKnown width add -> clamp -> drawBox -> SetColor/DrawRectangle ->
SetColor/DrawString(name) -> isKnown -> SetColor/DrawString(role)). The only
source-shape difference the oracle exposes is the `pad2 = pad * 2` local and
`centerX` modified in place (both already known from Ghidra). No new draw-order
or body-order evidence.

## 2. Measured table (official linked verifier)

`tools/verify/verify.py` on the linked `.so`; raw pct from a copy of
`score()` with `round(..., 4)` (the committed `--show` path does not rewrite
`report.json`, so a `--show`-only run reports the *previous* full run's pct).

| variant | source shape | raw pct | size | `|` lines |
|---|---|---:|---:|---:|
| **committed base** | flat `+(-width-pad-pad)`, `int x=centerX` | **95.4733** | 1948 | **83** |
| `pad2` + tuned flat | `int pad2=pad*2; top=...pad2`, flat arm | 95.4733 | 1948 | 83 |
| `inset` local + flat | `int inset=...; if (x-pad<inset)` | 95.4733 | 1948 | 83 |
| oracle-faithful | `pad2`, `top` before `centerX`, `centerX` in place, faithful `-(width+pad2)` arm | 93.5528 | 1948 | 178 |
| direct field in DrawRectangle | `x - Globals::layout->field_0x94` etc. | 89.2857 | 1952 | 243 |
| min/max macros | `SPACELOUNGE_MAX/MIN` | 89.7119 | 1940 | 215 |
| helper / lambda / struct copy of pad | inlined clamp | 95.4733 | 1948 | 83 |
| `VectorSignedToFloat(pad+pad,0)` | helper cast | 95.4733 | 1948 | 83 |

The oracle-faithful form is semantically right (and produces the original's
2-instruction clamp arm `itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`) but scores
lower because it allocates `pad = fp`, so the compiler needs a `mov r5,fp` pad
copy in both clamp arms. The committed flat form is a scoring artifact: it
keeps the same wrong `pad=fp, x=r9` allocation but its 4-instruction arm has a
smaller diff.

## 3. Why the residual is greedy-RA, not source shape

The residual is one contiguous region: `pad` is loaded into `fp` (r11) instead
of `r9`, `x` is materialised whole in `r9` instead of `r5 -> r3 -> r9`, and the
clamp arm / post-`drawBox` agent/top restore / tail branch offsets follow.
Register map:

```text
value        original        ours (base)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
x-pad        r6 -> r2        r6 -> r2
top-pad      r5 -> r3        r5 -> r3
```

The w5016 RA trace (instrumented LLVM 7.0.2, same revision as NDK r18b) is
reproduced here:

```text
select %609 x       size=4220 w=2.946139e-04 hint=$r3 -> $r5
select %47  boxW    size=768  w=6.080038e-04 hint=$r3 -> $r6
select %46  top-pad size=736  w=6.251306e-04 hint=$r3 -> $r5   (evicts x)
select %609 x       size=4220 w=2.946139e-04 hint=$r3 -> $r9   (r9 free -> whole)
select %45  x-pad   size=704  w=6.432504e-04 hint=$r2 -> $r6
select %47  boxW    size=768  w=6.080038e-04 hint=$r3 -> $r10
select %2   pad     size=6400 w=2.154168e-04 no hint -> cannot evict x in r9
     -> split; the load part lands in $r11 (fp)
```

`pad` is a plain `t2LDRi12` with **no COPY anywhere** in the MIR, so it is an
unhinted `0xA` interval and is selected after every `0xE` (physical-hint)
interval. By then `x` occupies `r9`. The target needs `pad` in `r9` *before*
`x` re-enqueues, which requires the whole `r9/r10/r11` trio to be live at that
point; no source shape tested (this wave or w5016's ~100 shapes, or w4519's
384-variant sweep) produces it without also changing the reuse structure or the
body size.

## 4. New shapes tried this wave (all <= 95.4733)

* oracle-faithful (`pad2`, `top` before `centerX`, `centerX` in place,
  `int inset`, `Globals::w - inset < centerX + pad + width`,
  `(Globals::w - inset) - (width + pad2)`, `pad2 + width`): 93.5528 / 178.
* oracle-faithful with `int x = centerX` separate: 93.5528 / 178.
* `pad2` local + tuned flat arm: 95.4733 / 83 (score-neutral).
* `inset` local + tuned flat arm: 95.4733 / 83.
* `pad2` + `inset` + flat arm (`+(-width-pad2)`): 178.
* inlined helper `clampPlateX(centerX, pad, width, inset)`: 83 (same code).
* lambda capturing `[pad, width, inset]`: 83 (same code).
* local `struct PadBox { int pad; } pb = {pad};`: 83 (same code).
* `AbyssEngine::AEMath::VectorSignedToFloat(pad + pad, 0)` for `top`: 83.
* direct `Globals::layout->field_0x94` in `DrawRectangle` (target colouring
  `pad=r9, x=r5`): 89.2857 / 243 / **1952** (recompute + reload).
* `SPACELOUNGE_MIN/MAX` clamp: 215.
* ternary clamp: 83 (canonicalised to the base).
* `int pad2 = pad + pad` / `pad << 1` / `2 * pad`: all 83.
* `int top` before `int centerX`: 83.

## 5. Conclusion

1. The wave-103 oracle confirms the draw order and the `pad2`/`centerX`
   source shape; it exposes no new body order.
2. The committed flat-clamp source is still the highest-scoring shape
   (95.4733 / 83 lines / 1948 B). The semantically faithful oracle shape scores
   93.5528 / 178 lines at the same size.
3. The residual is the greedy-RA `pad=fp, x=r9` allocation, blocked by `pad`
   having no physical copy hint. Byte-exactness would require reproducing the
   original's `x = r5 -> r3 -> r9` split and `pad = r9` whole; the target
   colouring is reachable only via a `field_0x94` reload in `DrawRectangle`,
   which also forces the recompute and changes the body size (1952).
4. No `src/` change is landed: the base is not improved by any tested shape.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS

# per-function diff (83 structural lines on the committed base)
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11SpaceLounge10drawLoungeEv
```

Scratch (not committed, `/dev/shm/wf317`): `wf317_sl_orig.cpp` (committed
base), `wf317_sweep*.py` (source-shape generators), `wf317_fast.sh` /
`wf317_testp.sh` (build + `--show` / full-verify harness), `wf317_verify_*.log`,
`wf317_orig_dis.txt` / `wf317_ours_dis.txt` (full disassembly).
