# TU membership convergence and interleaving (w3613)

Status: research note + one landed source cleanup (2026-11-02). Worktree `wt/w3613`,
branch `wave87-w3613`, based on master `4f8c8af5`. Every number below was measured
against `../_work/bins/android_2.0.16_libgof2hdaa.so` with this tree's NDK r18b
clang 7.0.2 / gold 1.12.

## TL;DR

* The older **"144 / 195 objects interleaved"** figure is real only when *every*
  exported symbol is counted. It is dominated by weak inline copies (`Array`,
  `String`, `Matrix` ctors) that every TU emits. Counting only strong (`T`)
  functions, **1 of 193** objects is interleaved: `NdkHooks.cpp` contains
  `NdkAutosave.cpp`'s `ndk_autosave` at `0x94770`.
* Eight source files in this tree are **empty invented TUs**: they emit no
  functions at all. Removing all eight changes the link from 203 to 195 objects,
  the marker count from 977 to 969, and the section-order file from 5885 to 5877
  names, yet leaves every ELF section byte-identical except the 19-byte
  `.note.gnu.build-id`. Gate: **byte 4496 / linked 4496**, unchanged.
* Merging `BSphere.cpp` into `Vector.cpp` (the SD dump attributes
  `BSphere::Merge(BSphere)` / `operator=` to `Vector.cpp`) **regresses
  byte_exact 4496 -> 4040** with `linked_exact` unchanged. The Android link wants
  the current separate `BSphere` object (or at least its current section order);
  dump attribution alone is not sufficient.
* The remaining TU-membership candidates are the known
  `NdkHooks`/`NdkAutosave` pair and the `GameResource` three-way split. Neither
  was landed here; both need a full A/B because the link order is load-bearing.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

`.text` is `0x1337c8 @ 0x69440`; `.ARM.exidx` is `0x6f70 @ 0x19cc08`;
`.ARM.extab` is `0x26290 @ 0x1a3b78` (original `0x26298`, the known -8);
`.rodata` is `0x41b67 @ 0x1c9e10` and byte-identical. The section fingerprint is
the same as `_work/reconstructions/tu_compile_link_order_w3560.md` §1 except for
the already-known `.ARM.extab` -8.

## 2. Interleaving measurement

The committed probe is `_work/reconstructions/w3613/tu_interleave.py`:

```
python3 _work/reconstructions/w3613/tu_interleave.py
```

It reads `cmake-build-match/link_order.rsp`, the object `.symtab`s and the
original `.dynsym`, and prints two metrics:

| metric | objects | interleaved |
|---|---:|---:|
| all exported functions (incl. weak `W`) | 195 | **144** |
| strong (`STB_GLOBAL`, `T`) functions only | 193 | **1** |

The single strong interleaving is:

```
STRONG NdkHooks.cpp.obj 0x93c88-0x94e38 foreign 1
  0x94770 NdkAutosave.cpp.obj ndk_autosave
```

This is the same `NdkHooks`/`NdkAutosave` pair already measured in
`tu_compile_link_order_w3560.md` E2 (merging them fixed the `.dynsym` gap but
cost 519 byte_exact because it changed the object's section/PLT order).

The dump-attribution check (HD app witness + SD engine witness) finds only two
original TUs represented by more than one of our objects:

```
Classes/Game/GameResource.cpp: 3 objects, 3 funcs
    BuildResourceList.cpp.obj
    LowResourceTable.cpp.obj
    ResourceMaterial.cpp.obj
Svn_iPhone_AE2/Classes/Vector.cpp: 2 objects, 20 funcs
    Vector.cpp.obj (18)
    BSphere.cpp.obj (2)
```

The 8 empty TUs removed in §4 are not in this list because they have no strong
function to attribute.

## 3. Experiment E1 — merge `BSphere.cpp` into `Vector.cpp`

The SD dump attributes `BSphere::Merge(BSphere const&)` and
`BSphere::operator=(BSphere const&)` to `Svn_iPhone_AE2/Classes/Vector.cpp`
(the current `Vector.cpp` already has `BSphere::Merge(Transform const&)`).
The Android `.text` addresses are:

```
0x7395d..0x73f31  Vector.cpp functions
0x73f31           BSphere::Merge(Transform const&)
0x74001           BSphere::Merge(BSphere const&)
0x74145           BSphere::operator=(BSphere const&)
0x74155           IApplicationModule::SetApplicationManager(...)
```

The merge was done by moving the two definitions from `BSphere.cpp` into
`Vector.cpp` in that exact order (after `Merge(Transform)`), then either
deleting `BSphere.cpp` or leaving it as an empty sentinel. Both variants were
measured; both give the same result.

```
# move the two functions, then force the custom link outputs to regenerate
python3 - <<'PY'
from pathlib import Path
v = Path('src/engine/math/Vector.cpp')
s = v.read_text()
bs = Path('src/engine/math/BSphere.cpp').read_text()
start = bs.index('        void BSphere::Merge(const BSphere &other)')
end = bs.index('        BSphere &BSphere::operator=(const BSphere &other)')
op_end = bs.index('\n\n\n    }\n}', end)
block = bs[start:op_end]
needle = '            Merge(sphere);\n        }\n'
assert needle in s
s = s.replace(needle, needle + '\n' + block.rstrip() + '\n', 1)
v.write_text(s)
PY
rm src/engine/math/BSphere.cpp
python3 - <<'PY'
import os
for p in ['cmake-build-match/link_order.rsp', 'cmake-build-match/section_order.txt',
          'cmake-build-match/libgof2hdaa.so', 'cmake-build-match/exidx_markers/stamp']:
    try: os.unlink(p)
    except FileNotFoundError: pass
PY
cmake --build cmake-build-match --target check -j8
```

Measured:

| variant | byte_exact | linked_exact | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` base | PLT same-index |
|---|---:|---:|---:|---:|---:|---:|---:|
| baseline | **4496** | **4496** | `0x1337c8` | `0x6f70` | `0x26290 @ 0x1a3b78` | `0x1c9e10` | 3668/3674 |
| BSphere -> Vector (delete BSphere) | 4040 | 4496 | `0x1337c8` | `0x6f60` | `0x26290 @ 0x1a3b68` | `0x1c9e00` | 3666/3674 |
| BSphere -> Vector (empty BSphere sentinel) | 4040 | 4496 | `0x1337c8` | `0x6f60` | `0x26290 @ 0x1a3b68` | `0x1c9e00` | 3666/3674 |

The `.text` differing-byte count rises from 119,626 to 124,344 even though the
size stays exact: moving the two functions into `Vector.cpp` changes the object's
local-section order and the resulting `.ARM.exidx`/`.ARM.extab`/`.rodata`
placement. The merge is therefore **not** a safe convergence at this HEAD.

## 4. Experiment E2 — remove the eight empty invented TUs

The following source files contain no function definitions (verified with the
object `.symtab`; `nm` reports zero `T`/`t` functions):

```
src/engine/audio/AESound.cpp
src/engine/core/AbyssEngine.cpp
src/engine/core/KeyCode.cpp
src/engine/core/KeyFrame.cpp
src/engine/render/ResourceMesh.cpp
src/engine/render/ResourceTexture.cpp
src/engine/render/ResourceTransform.cpp
src/game/weapons/AbstractGun.cpp
```

Their code is already emitted by the owning TU (`KeyFrame` by `Transform.cpp`,
`AbstractGun` by `BeamGun.cpp`, `Resource*` by `PaintCanvas.cpp`, etc.). The
cleanup is landed in this commit.

```
for f in src/engine/audio/AESound.cpp src/engine/core/AbyssEngine.cpp \
         src/engine/core/KeyCode.cpp src/engine/core/KeyFrame.cpp \
         src/engine/render/ResourceMesh.cpp src/engine/render/ResourceTexture.cpp \
         src/engine/render/ResourceTransform.cpp src/game/weapons/AbstractGun.cpp; do
    rm "$f"
done
python3 - <<'PY'
import os
for p in ['cmake-build-match/link_order.rsp', 'cmake-build-match/section_order.txt',
          'cmake-build-match/libgof2hdaa.so', 'cmake-build-match/exidx_markers/stamp']:
    try: os.unlink(p)
    except FileNotFoundError: pass
PY
cmake --build cmake-build-match --target check -j8
```

**Important:** deleting a source file does not invalidate the custom
`link_order.rsp` / `section_order.txt` outputs. Running `check` immediately can
silently reuse the old response file and report a false "unchanged" result.
Delete those outputs (or clean the build dir) before measuring.

Measured:

| variant | byte_exact | linked_exact | objects | markers | section-order names | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| baseline | **4496** | **4496** | 203 | 977 | 5885 | `0x1337c8` | `0x6f70` | `0x26290` | `0x41b67` |
| eight empty TUs removed | **4496** | **4496** | **195** | **969** | **5877** | `0x1337c8` | `0x6f70` | `0x26290` | `0x41b67` |

`lint` stays `CLEAN ✓` and `drift` stays 0. A byte comparison of the two link
outputs (`cmp`) shows only **19 differing bytes**, all inside
`.note.gnu.build-id`; every other section is byte-identical. The CANTUNWIND
fingerprint is unchanged: 3566 exidx entries, 272 CANTUNWIND, 194 duplicate
targets, 193 CANT+real pairs. Gold's output is unchanged; the empty objects'
zero-size markers were evidently collinear with existing marker addresses (the
exact gold coalescing rule was not instrumented).

## 5. Proven vs hypothesis

**Proven by measurement:**

* The 144/195 interleaving figure is an all-symbol metric; the strong-function
  metric is 1/193 (`NdkHooks`/`NdkAutosave`).
* Eight source TUs in the tree are empty and binary-invisible. Removing them is
  a strict TU-convergence improvement: 203 -> 195 objects, no gate change, no
  section content change except the build-id.
* Merging `BSphere.cpp` into `Vector.cpp` costs 456 byte_exact at this HEAD,
  despite the SD dump attributing those symbols to `Vector.cpp`.
* The `GameResource` original TU is split across three of our objects
  (`BuildResourceList`, `LowResourceTable`, `ResourceMaterial`); the `Vector`
  dump attribution is split across `Vector` + `BSphere`.

**Hypothesis (not proven here):**

* The `NdkHooks`/`NdkAutosave` merge would remove the last strong interleaving,
  but the earlier E2 measurement (byte -519) shows it must reproduce the
  original within-object section/PLT order at the same time.
* The `GameResource` three-way split is a real membership divergence, but the
  current link-order override (`LowResourceTable` before `ResourceMaterial`) is
  load-bearing for `.rodata`; a merge must be tested with the full gate and the
  `.rodata` string order checked.
* With all TU membership converged, the section-ordering file could be dropped
  and a plain input-order link would reproduce the original `.text`; this is the
  existing hypothesis in `ffunction_sections_and_order_file.md` §3, not newly
  proven here.

## 6. Reproduction

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3613/tu_interleave.py
```

The interleaving probe is committed at
`_work/reconstructions/w3613/tu_interleave.py`; it is self-contained and reads
only the original `.so`, `link_order.rsp`, and the object `.symtab`s.
