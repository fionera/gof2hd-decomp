# SpaceLounge::drawLounge — wave-107 (w5519)

Status: landed pass (2026-11-04). Worktree `wt/w5519`, branch
`wave107-w5519`, base `f857b5171`.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, still not byte-exact).

* before: raw **95.473251%**, 1948 B, 729/729 insns
* after:  raw **95.538778%**, 1948 B, 729/728 insns

The landed change is a real (if small) fuzzy improvement, not a byte match.
All ELF sections remain size- and base-exact; no `.ARM.extab` or section
delta was introduced.

Prior work: `spacelounge-drawlounge-modstation-ctor-w5320.md` (w5320),
`SpaceLounge-drawLounge-wave106-w5418.md` (w5418),
`spacelounge-drawlounge-w5016.md`, `-w4818.md`, `-w4519.md`, `-w4246.md`,
`-w4227.md`, `-w4118.md`.

## 1. Landed source change

In the mode-0 name-plate clamp, the right-edge arm now folds the padding
through a 16-bit sign-extending intermediate:

```cpp
Layout *layout = Globals::layout;
// The original right-edge clamp folds the layout padding through a
// 16-bit sign-extend (mov.w r1, pad, lsl #16; add.w r1, width,
// r1, asr #15) rather than reusing the int pad*2 kept in sl.
short pad3 = pad;
int x = centerX;
if (x - pad < layout->buttonInsetX) {
    x = layout->buttonInsetX + pad;
} else if (pad + x + width > Globals::w - layout->buttonInsetX) {
    x = Globals::w - layout->buttonInsetX - (width + pad3 * 2);
}
```

The base source used the flat form
`x = Globals::w - layout->buttonInsetX + (-width - pad - pad);` and scored
95.473251. The faithful int form `-(width + pad * 2)` alone scores
93.964335; the `short` intermediate recovers and slightly beats the flat
form.

Equivalent one-line spelling (also measured 95.538778):

```cpp
x = Globals::w - layout->buttonInsetX - (width + (short) pad * 2);
```

The local `short pad3` form is the one gate-checked and committed.

## 2. Build / measurement commands

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Per-variant measurement: copy the variant over `src/game/menu/SpaceLounge.cpp`,
`cmake --build cmake-build-match --target link_so -j6`, then run the official
`tools/verify/verify.py` score on the linked `.so`, then
`git checkout -- src/game/menu/SpaceLounge.cpp`. Raw (unrounded) ratio is
`difflib.SequenceMatcher(..., autojunk=False).ratio()*100` over the normalised
instruction stream, exactly as `tools/verify/verify.py:score`.

Scratch harness (not committed) in `/dev/shm/w5519`: `base.cpp`,
`measure.py`, `measvar.py`, `fastcheck.py`, `sweep3.py`/`sweep4.py`,
`t4.py`..`t13.py`, `base_ra.txt` (greedy trace), `vrwf.dis` (instrumented
VRW build).

## 3. Gate result (with the landed change)

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN ✓ (no new soft violations; hard invariants 0)
drift:  0 field(s) across 0 class(es)
```

`tools/verify_baseline.json` was not updated (no count changed).

## 4. Residual diff

`osize = usize = 1948`, 729 vs 728 instructions. The residual is confined to
the mode-0 name-plate clamp / join / post-`drawBox` restore plus the tail
branch offsets that follow from the clamp's 2-byte size difference. Register
map:

```text
value        original        ours (landed)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

Original clamp right arm (2 insns) vs landed form (4 insns):

```text
orig:   itt gt
        addgt.w r1, r6, sl        ; width + pad*2
        subgt   r5, r0, r1        ; x = (w - inset) - (width + pad*2)
landed: ittt gt
        movgt.w r1, fp, lsl #16   ; (short) pad
        addgt.w r1, r6, r1, asr #15 ; width + pad*2
        subgt.w r9, r0, r1
```

The landed form removes one instruction from the base's 4-instruction flat
clamp arm (`movgt #0; subgt.w r1, r1, fp, lsl #1; subgt.w r1, r1, r6;
addgt.w r9, r1, r0`), which is where the score gain comes from.

## 5. Measured variants (official linked verifier)

| variant | raw % | size |
|---|---:|---:|
| base (flat clamp) | 95.473251 | 1948 |
| **short pad3 in clamp RHS (landed)** | **95.538778** | **1948** |
| inline `(short) pad * 2` | 95.538778 | 1948 |
| `int pad3 = (short) pad` | 95.538778 | 1948 |
| `signed char pad3` | 95.538778 | 1948 |
| `short pad3` + `int pad2 = pad3*2` | 95.473251 | 1948 |
| `unsigned short pad3` | 91.718001 | 1956 |
| `char` / `unsigned char pad3` | 95.198903 | 1948 |
| `bool pad3` | 91.581109 | 1956 |
| short `width2` in cond + faithful clamp | 95.401510 | 1948 |
| short `top2` / `x2` copies | 95.198903 | 1948 |
| short `lineHeight2` | 92.117889 | 1952 |
| short `centerX2` | 91.980809 | 1952 |
| short `pad3` in top + RHS | 88.461538 | 1948 |
| short `pad3` in box + RHS | 71.819425 | 1972 |
| short `pad3` in top only | 81.973955 | 1968 |
| short `pad3` in box only | 72.614962 | 1964 |
| `pad2` in top+clamp | 88.461538 | 1948 |
| `pad2` in clamp only | 95.473251 | 1948 |

Other wave-107 source shapes (all <= 95.473251 with the flat clamp):

| variant | raw % | size | pad | x |
|---|---:|---:|---|---|
| faithful `-(width + pad*2)` | 93.964335 | 1948 | fp | r9 |
| faithful + `pad2` local (top/box/clamp) | 93.964335 | 1948 | fp | r9 |
| plain `if (agent->isKnown())` + faithful | 93.689986 | 1948 | r5 | r9 |
| plain `if` + flat | 92.054795 | 1952 | r5 | r9 |
| reversed condition `inset > x - pad` + flat | 95.198903 | 1948 | fp | r9 |
| reversed condition + faithful | 93.827160 | 1948 | fp | r9 |
| `volatile int pad` | 66.030014 | 1980 | r1 | r5 |
| `volatile int pad` + faithful | 66.165414 | 1976 | r1 | r5 |
| direct `Globals::layout->field_0x94` in clamp (3 loads) | 90.697674 | 1960 | r9 | r9 |
| direct field in top + late `pad` (2 loads) | 69.856459 | 1976 | r0 | r5 |
| `boxX`/`boxY` locals | 95.473251 | 1948 | fp | r9 |
| `drawX` before drawBox | 95.473251 | 1948 | fp | r9 |
| `drawX` before DrawString | 70.758621 | 1956 | fp | r9 |
| `int pad2 = pad * 2;` in every position/use | 93.96-95.47 | 1948 | fp | r9 |
| `const`/`register`/`unsigned`/`long` pad | 95.473251 | 1948 | fp | r9 |
| pad load moved after SetColor / after centerX | 95.473251 | 1948 | fp | r9 |
| top before centerX | 95.473251 | 1948 | fp | r9 |
| `__builtin_expect(isKnown()!=0/==1/(bool)/?:,1)` | 95.473251 | 1948 | fp | r9 |
| plain `isKnown()!=0` / `==1` / `!!` | 92.054795 | 1952 | r5 | r9 |

## 6. New evidence: the exact greedy-allocator condition (weight, not priority)

The blocker is the `RAGreedy::shouldEvict` predicate
(`lib/CodeGen/RegAllocGreedy.cpp`):

```cpp
if (A.weight > B.weight) return true;   // A evicts B
```

`pad` can only take `r9` from `x` if **pad's spill weight exceeds x's**.  The
instrumented clang 7.0.2 (`/tmp/llvm_android_702b/build2`) prints the exact
numbers for this function:

```text
pad  %2   size 6400  weight 2.154168e-04   (no COPY hint -> prio 0xA0001900)
x    %609 size 4220  weight 2.946139e-04   (hint $r3     -> prio 0xE0000000+4220)
```

`normalizeSpillWeight` is `UseDefFreq / (Size + 25*SlotIndex::InstrDist)`
(`CalcSpillWeights.h`), so the *numerator* is actually higher for pad
(`1.344` vs `1.273`); pad loses **only because its live interval is 2180 slot
indices longer**.  To flip the eviction, pad's size must drop below about
`6400 * 2.154/2.946 = 4680` (or x's weight must drop below 2.154e-4).

This explains all the earlier negative results: no clamp algebra, `pad2`
local, declaration order, `drawX`/`boxX` copy, cast, or `volatile` placement
changes pad's *live range* (it is one load at `0x170ebe` used in the top, the
clamp and both box calls), so pad's size stays 6400 and its weight stays
2.154e-4.

The landed `short pad3` variant does **not** change pad/x allocation (still
`pad=fp`, `x=r9`); it only changes the clamp RHS codegen, which is a legal
source-level spelling of the original's 16-bit fold.

## 7. Instrumented confirmation: VRW=pad:1.45 reproduces the clamp exactly

`VRW=2147483650:1.45` (scratch compiler only) raises pad's weight to
`3.12e-4 > x's 2.946e-4`, so pad evicts x from `r9` and x is re-selected into
`r5` and split.  The resulting clamp is **byte-identical to the original**:

```text
57a: vmov r5, s18                 ; x = r5
588: sub.w r1, r5, r9             ; x - pad
590: add.w r5, r0, r9             ; x = inset + pad
5a0: add.w r1, r9, r5             ; pad + x
5a4: add r1, r6                   ; + width
5a8: itt gt
5aa: addgt.w r1, r6, sl           ; width + pad*2
5ae: subgt r5, r0, r1             ; x = right - (...)
```

but the cascade also moves `lineHeight` to `r8` (original `fp`) and `top` to
`r1` (original `r2 -> r8`), so the linked score is only ~95.7 and the body
does not stay byte-identical elsewhere.  Weight alone is therefore necessary
but not sufficient.

## 8. What is proven vs hypothesis

Proven (measured, reproducible):

1. The `short pad3` clamp RHS is a strict improvement over the committed
   base: 95.473251 -> 95.538778 at the same 1948 B.
2. The faithful int form `-(width + pad * 2)` alone is worse (93.964335);
   the 16-bit fold is what recovers the original's `lsl #16 / asr #15`
   shape.
3. The greedy allocator's eviction rule is a strict weight comparison
   (`A.weight > B.weight`), and pad's weight is 2.154168e-04 vs x's
   2.946139e-04.  pad's *use-density numerator* is higher (1.344 vs 1.273);
   it loses purely on interval size (6400 vs 4220).
4. Raising pad's weight in the allocator (VRW=1.45) reproduces the original
   2-instruction clamp exactly (`pad=r9`, `x=r5`), but perturbs
   `lineHeight`/`top` in the same cascade.
5. Every other source-level clamp algebra, `pad2`/`boxX`/`drawX` local,
   declaration order, cast, and branch-probability spelling tested
   canonicalises to the same MIR and therefore the same 6400-slot pad
   interval.

Hypothesis (not proven):

1. The original source must reduce pad's live range below ~4680 slot indices
   (or reduce x's weight below 2.154e-4) *without* adding a second load or
   extra instructions.  The single load at `0x170ebe` and pad's uses in top +
   clamp + both box calls make this hard: the interval necessarily spans the
   name/role/GetTextWidth block.
2. A source construct that lets the compiler rematerialize or re-load pad
   after the width block while keeping one `ldr [layout,#148]` in the final
   code would be the most likely route; none was found.

## 9. Conclusion

Landed the `short pad3` clamp spelling: raw **95.473251 -> 95.538778**
(1948 B, 729/728 insns), gate `byte 4500 / linked 4500 / data 843 /
imports 207 / lint CLEAN / drift 0`, `RATCHET PASS`.  The function is still
not byte-exact; the remaining gap is the pad/x allocation (pad=fp vs r9,
x=r9 vs r5) described above.
