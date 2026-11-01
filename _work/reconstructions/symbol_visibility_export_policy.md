# Symbol visibility / export policy (w3427)

Status: research note, 2026-11-01. Worktree `wt/w3427`, branch `wave87-w3427`, based on
`d00e0030`. All numbers below are from this worktree and the original
`../_work/bins/android_2.0.16_libgof2hdaa.so` (sha256
`d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).

## TL;DR

The original was built with **default ELF visibility**, not `-fvisibility=hidden`,
`-fvisibility=inlines-hidden`, or `-fvisibility=protected`:

* every named `.dynsym` entry in the original has `st_other == 0` (`STV_DEFAULT`);
* the original exports 5370 defined symbols: 4062 `GLOBAL FUNC`, 462 `WEAK FUNC`,
  830 `GLOBAL OBJECT`, 13 `WEAK OBJECT`, and 3 `GLOBAL NOTYPE` ABS symbols;
* the current tree already matches that policy, but before this pass it had exactly one
  binding mismatch: `PaintCanvas::DrawMesh(Mesh*, Matrix&, Matrix&, unsigned, Matrix&)` was
  `GLOBAL` in our link and `WEAK` in the original. Declaring that overload `inline` in
  `PaintCanvas.h` reproduces the original `WEAK` binding with no gate change.

The gate (`tools/ratchet.py`) does **not** compare symbol binding/visibility, so this mismatch
was invisible to `check`. `tools/sodiff.py` checks names and types, not `st_info`/`st_other`.

## Baseline (before the source change)

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8
```

Measured:

| item | value |
|---|---|
| `byte_exact` | **4182** |
| `linked_exact` | **4495** |
| `.text` | **0x1337c8 @ 0x69440** (exact) |
| `.rodata` | **0x41b67 @ 0x1c9e10** (exact) |
| `.bss` | **0x69ac @ 0x216970** (exact) |
| `data_exact` | **843 / 843** |
| `sodiff` | original 5370, ours 5370, extra 0, missing 0 |
| undefined imports | **207** |
| lint | `CLEAN ✓` |
| drift | `0 field(s) across 0 class(es)` |

The check line was:

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## Original `.dynsym` shape

Parsing the original `.dynsym` directly (ELF32_Sym: `st_info` at +12, `st_other` at +13)
gives:

| binding | type | section | count |
|---|---|---|---:|
| GLOBAL | FUNC | defined | 4062 |
| WEAK | FUNC | defined | 462 |
| GLOBAL | OBJECT | defined | 830 |
| WEAK | OBJECT | defined | 13 |
| GLOBAL | NOTYPE | ABS | 3 |
| GLOBAL | FUNC | UND | 205 |
| GLOBAL | OBJECT | UND | 2 |
| LOCAL | NOTYPE | UND | 1 (the null symbol) |

All 5577 named entries have `st_other == 0` (`STV_DEFAULT`). There are no
`STV_HIDDEN`, `STV_INTERNAL`, or `STV_PROTECTED` entries, and no defined `STB_LOCAL`
entries in `.dynsym` (the original is stripped; local symbols are absent entirely).

The 3 `GLOBAL NOTYPE ABS` symbols are the linker-provided `_edata`, `_end`,
`__bss_start`.

Reproduction of the counts:

```sh
readelf -sW ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | awk 'NR>3 && $1 ~ /^[0-9]+:/ {print $5, $6, $4, $7}' | sort | uniq -c
readelf -sW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep DrawMesh
```

The 462 weak functions are consistent with the normal C++ `weak_odr` result of
inline/template definitions. The original exports them; hiding inline functions is therefore
wrong.

## Controlled A/B: visibility flags

Each variant was configured in a scratch build directory under `/tmp` with
`GOF2_MATCH_OPT` (the only supported way to add a compile flag without editing the tree):

```
cmake -S . -B /tmp/w3427-vis-hidden-1 -DGOF2_MATCH_OPT='-Oz -fvisibility=hidden'
cmake --build /tmp/w3427-vis-hidden-1 --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /tmp/w3427-vis-hidden-1/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3427-vis-hidden-1/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 tools/sodiff.py --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3427-vis-hidden-1/libgof2hdaa.so --allow tools/sodiff_allow.txt
python3 tools/ratchet.py --build-dir /tmp/w3427-vis-hidden-1 --no-rebuild
```

The same commands were used with `-fvisibility-inlines-hidden` and
`-fvisibility=protected`.

| variant | byte | linked | compared | missing | data exact | `.text` | sodiff defined exports | verdict |
|---|---:|---:|---:|---:|---:|---|---:|---|
| baseline (default) | **4182** | **4495** | 4524 | 0 | 843/843 | 0x1337c8 @ 0x69440 | 5370 | correct |
| `-fvisibility=hidden` | 57 | 243 | 244 | 4280 | 297/341 | 0x130f9c @ 0x10b80 | 588 | catastrophic: hides 4782 exports |
| `-fvisibility-inlines-hidden` | 1278 | 4203 | 4384 | 140 | 772/843 | 0x1337a8 @ 0x667e0 | 5230 | wrong: original exports inline functions |
| `-fvisibility=protected` | 1345 | 2383 | 4524 | 0 | 633/843 | 0x130f98 @ 0x58880 | 5370 | wrong: changes visibility and call codegen |

`-fvisibility=hidden` is not a subtle near miss: it leaves only 244 compared functions and
removes 4782 of the original's 5370 defined exports. `-fvisibility-inlines-hidden` removes
140 inline exports (`AbstractGun::isAutoTurret`, `ModStation::OnTouchEnd`, ...). Protected
visibility keeps the names but marks 3838 `GLOBAL FUNC` and 442 `WEAK FUNC` as
`STV_PROTECTED`, and the codegen/PLT shape collapses (`linked_exact` 2383).

Section sizes/addresses for the same variants:

| variant | `.text` | `.rodata` | `.bss` |
|---|---|---|---|
| original | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | 0x69ac @ 0x216970 |
| baseline (default) | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | 0x69ac @ 0x216970 |
| `-fvisibility=hidden` | 0x130f9c @ 0x10b80 | 0x41b67 @ 0x16ed10 | 0x68fc @ 0x1b8a70 |
| `-fvisibility-inlines-hidden` | 0x1337a8 @ 0x667e0 | 0x41b67 @ 0x1c7190 | 0x69ac @ 0x213970 |
| `-fvisibility=protected` | 0x130f98 @ 0x58880 | 0x41b67 @ 0x1b6a10 | 0x68fc @ 0x200a70 |

The baseline is the compiler default (`-fvisibility=default`); no explicit visibility flag
is present in `cmake/toolchain.cmake`.

## The one binding mismatch, and its fix

Before the fix, comparing every common `.dynsym` name by `(st_info, st_other, st_shndx,
st_size)` found exactly two non-value differences:

```
_ZN11AbyssEngine11PaintCanvas8DrawMeshEPNS_4MeshERNS_6AEMath6MatrixES5_jS5_
    original: st_info=0x22 (WEAK FUNC), st_other=0, size=1296, value=0x7e979
    ours:     st_info=0x12 (GLOBAL FUNC), st_other=0, size=1296, value=0x7e979

_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
    size 1996 (original) vs 1992 (ours)   # unrelated body-size issue
```

The `PaintCanvas::DrawMesh` overload is defined out of line in
`src/engine/render/PaintCanvas.cpp`; the header declaration in
`src/engine/render/PaintCanvas.h` was not `inline`. The original symbol is `WEAK`, which is
the normal binding for an inline definition. Declaring the header overload `inline`:

```c++
inline void DrawMesh(Mesh *mesh, Matrix &worldMatrix,
                     Matrix &viewMatrix, unsigned int color,
                     Matrix &uvMatrix);
```

changes the emitted binding to `WEAK` without changing its address, size, or code. A scratch
build of that change measured:

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

and the `.dynsym` binding counts became exactly the original's:

```
4062 GLOBAL FUNC / 462 WEAK FUNC / 830 GLOBAL OBJECT / 13 WEAK OBJECT / 3 GLOBAL NOTYPE
```

The only remaining non-value `.dynsym` difference is the unrelated
`Generator::createMission` size.

A definition-only `inline` (adding `inline` to the `.cpp` definition but not the header) also
produced the `WEAK` binding and avoided `-Wundefined-inline` warnings, but it is
standard-ill-formed (an inline declaration must be reachable in every TU that declares the
function). The header form is the standard-correct one and is the one committed.

## Conclusion / policy

1. **Default visibility is the original policy.** All 5370 defined exports are
   `STV_DEFAULT`; no export is hidden/protected/internal. Keep global definitions at default
   visibility.
2. **Do not add `-fvisibility=hidden` or `-fvisibility-inlines-hidden`.** The original exports
   both ordinary functions and inline/template functions. Hiding either changes the export
   set and the PLT/call codegen.
3. **Do not add `-fvisibility=protected`.** It keeps names but changes visibility and
   interposition/call codegen.
4. **Internal helpers should use internal linkage (`static` / anonymous namespace), not
   visibility attributes.** The original's non-exported helpers are absent from `.dynsym`;
   the stripped binary cannot distinguish `static` from hidden, but the exported set is
   unambiguously default-visible.
5. **Weak is expected for inline/template definitions.** 462 functions and 13 objects are
   `WEAK` in the original. Do not force them strong. Conversely, a function the original
   exports `WEAK` should be declared inline (or otherwise weak) in source; the
   `PaintCanvas::DrawMesh` overload was the last such mismatch.
6. **Gate gap:** `tools/sodiff.py` and `tools/ratchet.py` compare names/types, not
   `st_info`/`st_other`. A future symbol-parity check should compare binding and visibility
   too; otherwise a single strong/weak flip is invisible.

## Proven vs still hypothesis

**Proven by measurement**

* The original's named `.dynsym` entries are all `STV_DEFAULT`; the defined-export counts are
  4062 GLOBAL FUNC, 462 WEAK FUNC, 830 GLOBAL OBJECT, 13 WEAK OBJECT, 3 GLOBAL NOTYPE.
* `-fvisibility=hidden`, `-fvisibility-inlines-hidden`, and `-fvisibility=protected` each
  regress the gate; the measured numbers are in the table above.
* The baseline tree had exactly one binding mismatch, the 5-argument
  `PaintCanvas::DrawMesh`; declaring it `inline` in the header changes it to `WEAK` and
  leaves byte/linked/data/imports unchanged.

**Still hypothesis / not claimed**

* The original source's exact spelling of the `inline` declaration (header vs definition)
  cannot be recovered from the stripped binary. Both produce `WEAK`; the header form is
  standard-correct and is what this tree uses.
* The original's non-exported helpers could have been `static`, anonymous-namespace, or
  hidden-visibility globals; the stripped `.dynsym` cannot distinguish them. Only the
  exported set is proven.
* The `Generator::createMission` 4-byte size difference is a separate body/TU issue, not a
  visibility issue.
