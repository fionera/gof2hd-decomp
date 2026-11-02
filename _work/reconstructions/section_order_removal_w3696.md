# Section-ordering removal: per-TU emission order is necessary but not sufficient (w3696)

Status: research note, 2026-11-02. Worktree `wt/w3696`, branch `wave87-w3696`,
based on master `a1c57bff`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with this tree's NDK r18b
clang 7.0.2 / gold 1.12. This is a follow-up to
`_work/reconstructions/section_order_removal_w3687.md`; the new result is that
**sorting each object's own `.text.*` sections is not enough** to remove the
ordering file. The ordering file is also a per-symbol scatter map for weak
inline copies emitted by the invented `Array.cpp` / `ArrayInstantiations.cpp`
TUs.

## TL;DR

* Baseline (unchanged): **byte_exact 4496 / linked_exact 4496**, `.text`
  `0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift `0`.
* Reconfirmed the w3687 removal cost: omitting `--section-ordering-file` gives
  **byte 1527 / linked 4496**, only **307/4524** exported functions at their
  original address, exact-address prefix **47** (first mismatch
  `keyEventReleased @ 0x6a41c`).
* New controlled experiment: permute **every object's own `.text.*` section
  headers** into the per-object subsequence of `section_order.txt`, then relink
  with the same gold line minus the ordering file. Result: **byte 1728 /
  linked 4496**, **504/4524** addresses, exact-address prefix **268**. This
  proves per-object emission order is necessary, but not sufficient.
* The first remaining mismatch is
  `ArrayReleaseClasses<AELowLevelFile*>`: original `0x702e4`, ours `0x18690e`.
  It is defined in `ArrayInstantiations.cpp`, but the original placed it inside
  the `AEFile.cpp` TU. `ArrayInstantiations.cpp` alone defines 35 exported weak
  functions spanning `0x702e4..0x183d74` (`0x113a90`); `Array.cpp` defines 57
  spanning `0x6ffe8..0x1839d8`. One object cannot occupy all those positions.
* Targeted TU-membership experiment: move just that one explicit instantiation
  from `ArrayInstantiations.cpp` to `AEFile.cpp`. Gate stays **4496/4496**;
  reorder-all + no-order improves **1728 -> 1861**, addresses **504 -> 970**,
  prefix **268 -> 970**. The next mismatch is
  `ArrayReleaseClasses<ShaderBaseStruct*>` (`0x84240`), which belongs to
  `Engine.cpp`. Moving that one too regressed the gate **4496 -> 4494**
  (`linked 4495`), so it was not landed.
* Three source-order fixes (VirtualInput, GLES1Compat, FileInterfaceAndroid)
  are gate-neutral **4496/4496** but improve the no-order build
  **1527 -> 1557** and prefix **47 -> 222**. They show the per-TU source-order
  part is tractable; the weak-copy membership part is the harder blocker.
* **Conclusion: keep the ordering file for now.** Removing it requires both
  (a) per-TU source/emission order convergence and (b) TU-membership
  convergence for weak inline copies (`Array.cpp`, `ArrayInstantiations.cpp`,
  and the weak copies every TU emits). The ordering file is currently doing
  both jobs.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section fingerprint (original == baseline except the known `.ARM.extab` -8):

| section | original | baseline |
|---|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x26290 @ 0x1a3b78` |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` |

## 2. No-order baseline (reproduction of w3687)

The committed helper relinks the current marker objects with the exact gold
line from `CMakeLists.txt`, minus `--section-ordering-file`:

```
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3696/noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3696/verify_noorder \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3696/noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3696/noorder.so
```

Measured:

```
compared 4524  avg 99.9%  byte_exact 1527  linked_exact 4496  fuzzy_perfect 4496
/tmp/w3696/noorder.so: 307/4524 same; prefix 47;
  first (0x6a41c, keyEventReleased, 0x6a5e4)
```

This is the same result as w3687. `.text` size stays `0x1337c8`; only placement
changes.

## 3. New experiment: reorder every object's own sections

With `-ffunction-sections`, gold's input order is the section-header order of
each relocatable object. The committed `reorder_elf_sections.py` permutes only
the `.text.*` section-header entries of an ELF32 object into the order in which
`section_order.txt` lists those same sections, then rewrites all section-index
references (`e_shstrndx`, `sh_link`, REL/RELA/GROUP `sh_info`, symtab
`st_shndx`, GROUP payload indices). No source or linker flag is changed.

```
mkdir -p /tmp/w3696/reordered
python3 - <<'PY'
import os, subprocess
src = 'cmake-build-match/exidx_markers/link_order.rsp'
lines = [l.strip() for l in open(src) if l.strip()]
out = []
for i, p in enumerate(lines):
    dst = f'/tmp/w3696/reordered/{i:03d}_{os.path.basename(p)}'
    subprocess.run(['python3',
                    '_work/reconstructions/w3696/reorder_elf_sections.py',
                    p, dst, 'cmake-build-match/section_order.txt'], check=True)
    out.append(dst)
open('/tmp/w3696/reordered/link_order.rsp', 'w').write('\n'.join(out) + '\n')
PY
python3 _work/reconstructions/w3696/relink_noorder.py \
  /tmp/w3696/reordered/noorder.so /tmp/w3696/reordered/link_order.rsp
python3 tools/verify/verify.py \
  --build-dir /tmp/w3696/verify_reordered \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3696/reordered/noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3696/reordered/noorder.so
```

Measured:

| variant | byte_exact | linked_exact | funcs at orig addr | exact-address prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| ordering file (baseline) | **4496** | **4496** | 4524 / 4524 | 4524 | — |
| no ordering file | 1527 | 4496 | 307 / 4524 | 47 | `keyEventReleased` `0x6a41c` vs `0x6a5e4` |
| no ordering file + all objects' sections reordered | **1728** | **4496** | **504 / 4524** | **268** | `ArrayReleaseClasses<AELowLevelFile*>` `0x702e4` vs `0x18690e` |

The 201-point gain from sorting each object's own sections is real, but it
stops at the first weak inline copy that belongs to a different object in our
tree than in the original.

## 4. Why per-object order is not enough: weak-copy scatter

`ArrayInstantiations.cpp` and `Array.cpp` are invented TUs. The original had
no such TUs; it emitted weak template copies from the TUs that used them. Our
explicit instantiations put all those copies in two objects at the end of the
link order. The ordering file then scatters them to their original addresses;
a plain input-order link cannot.

Measured spans of exported weak functions defined by those objects (original
`.dynsym` addresses):

| object | exported weak funcs | original address span | range |
|---|---:|---:|---:|
| `ArrayInstantiations.cpp.obj` | 35 | `0x113a90` | `0x702e4..0x183d74` |
| `Array.cpp.obj` | 57 | `0x1139f0` | `0x6ffe8..0x1839d8` |

This is why the first mismatch after sorting every object is
`ArrayReleaseClasses<AELowLevelFile*>`: it belongs at `0x702e4` inside the
`AEFile.cpp` block, but our definition lives in the last object.

### 4.1 Controlled TU-membership move

`AEFile.cpp` already calls `ArrayReleaseClasses(*AEFile::file)`, so its
original weak copy belonged to that TU. The experiment moved the explicit
instantiation from `ArrayInstantiations.cpp` to `AEFile.cpp`:

```diff
--- a/src/engine/file/AEFile.cpp
+++ b/src/engine/file/AEFile.cpp
@@
 #include "engine/file/AENormalFile.h"
+#include "engine/core/ArrayReleaseClasses.h"
+
+template void ArrayReleaseClasses<AELowLevelFile *>(Array<AELowLevelFile *> &);
--- a/src/engine/core/ArrayInstantiations.cpp
+++ b/src/engine/core/ArrayInstantiations.cpp
@@
-template void ArrayReleaseClasses<AELowLevelFile *>(Array<AELowLevelFile *> &);
```

Result with the ordering file: **byte 4496 / linked 4496** (no gate change).
Then reorder all objects as in §3 and relink without the ordering file:

| variant | byte_exact | linked_exact | funcs at orig addr | exact-address prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| no order + all objects reordered | 1728 | 4496 | 504 / 4524 | 268 | `ArrayReleaseClasses<AELowLevelFile*>` |
| + move that one instantiation to `AEFile.cpp` | **1861** | **4496** | **970 / 4524** | **970** | `ArrayReleaseClasses<ShaderBaseStruct*>` `0x84240` vs `0x186914` |

This is causal: one weak-copy membership move fixes 466 addresses and lifts the
no-order byte count by 133. The next mismatch is the same class of problem:
`ArrayReleaseClasses<ShaderBaseStruct*>` belongs to `Engine.cpp` (original
`0x84240`, between `Engine::~Engine` and `Engine::AfterGLInit`), but our
definition is in `Array.cpp`.

Moving that second instantiation to `Engine.cpp` was tested and **regressed
the gate to byte 4494 / linked 4495**, so it was reverted. The weak-copy
membership campaign is therefore not free; each move must be checked under the
full gate.

## 5. Per-TU source-order experiments

Three source-order fixes were tested. All are gate-neutral with the ordering
file on (**4496/4496**), but they move the no-order frontier:

| experiment | gate byte/linked | no-order byte | exact-address prefix | next mismatch |
|---|---:|---:|---:|---|
| baseline | 4496 / 4496 | 1527 | 47 | `keyEventReleased` |
| `VirtualInput.cpp`: move `SendStoredKeyUpEvents` after `keyReleased`; mark `g_delayedKeyCount`/`g_delayedKeys` `[[gnu::used]]` | 4496 / 4496 | 1529 | 134 | `glShadeModel` |
| + `GLES1Compat.cpp`: reorder the `extern "C"` GL stubs to original address order | 4496 / 4496 | 1529 | 182 | `FileInterfaceAndroid::FileInterfaceAndroid()` |
| + `FileInterfaceAndroid.cpp`: move local `OpenAppend` after `FileInterfaceAndroid::Output` | 4496 / 4496 | 1557 | 222 | `EaseInOutMatrix::SetDuration` |

Details:

* `VirtualInput.cpp`: the original order is `keyEventReleased`,
  `keyReleased`, `SendStoredKeyUpEvents`. Our source had
  `SendStoredKeyUpEvents` before `keyPressed`, which made clang emit
  `keyReleased` early. Moving it after `keyReleased` fixes the order, but
  changes the `.bss` first-use order of `g_delayedKeyCount`/`g_delayedKeys`.
  Adding `[[gnu::used]]` to those two file statics restores the baseline `.bss`
  order and makes the gate neutral; without it w3687 measured 4496 -> 4491.
* `GLES1Compat.cpp`: the original `.text` order is
  `glShadeModel, glMatrixMode, glVertexPointer, glAlphaFunc, glLoadIdentity,
  glScalef, glFogf, glFogfv, glLoadMatrixf, glTexEnvi, glMultMatrixf,
  glTexCoordPointer, glNormalPointer, glColorPointer, glTexEnvf,
  glEnableClientState, glClientActiveTexture, glMaterialfv, glColor4f,
  glLightfv, glDisableClientState, glLightModelfv, glHint, glMaterialf`.
  Reordering the source stubs to that order is gate-neutral.
* `FileInterfaceAndroid.cpp`: `_Z10OpenAppendPtibj` is exported at
  `0x6e802`, after `FileInterfaceAndroid::Output`, but our source defined it
  before the constructors. Moving the definition after `Output` removes the
  no-order shift of the `FileInterfaceAndroid` ctor.
* The next mismatch, `EaseInOutMatrix::SetDuration`, is not a plain function
  order problem. The object interleaves weak inline `AEMath::Matrix` and
  `Quaternion` copies (`Matrix::Matrix`, `operator-`, `operator+`, `operator*`)
  between the `EaseInOutMatrix` methods; the original placed them at different
  first-use points. This is the same weak-copy issue as §4.

These source edits are **not landed in this commit** because the gate is
unchanged (4496/4496); they are recorded here as a reproducible convergence
path. The exact patches are in the diff snippets above and in the experiment
commands.

## 6. Proven vs hypothesis

**Proven by measurement:**

* Removing the ordering file at this HEAD costs **2969 byte_exact**
  (4496 -> 1527), with `linked_exact` unchanged at 4496 and `.text` size
  unchanged at `0x1337c8`.
* Sorting every object's own `.text.*` sections into the ordering-file
  subsequence improves the no-order build to **1728 / 4496**, 504/4524
  addresses, prefix 268. It is necessary but not sufficient.
* The first remaining blocker is weak-copy TU membership:
  `ArrayReleaseClasses<AELowLevelFile*>` is defined in
  `ArrayInstantiations.cpp` but belongs inside `AEFile.cpp`.
* Moving that one instantiation to `AEFile.cpp` is gate-neutral and improves
  the no-order build to **1861 / 4496**, 970/4524 addresses, prefix 970.
* `ArrayInstantiations.cpp` and `Array.cpp` define weak functions spanning
  `0x702e4..0x183d74` and `0x6ffe8..0x1839d8` respectively; no single object
  position can reproduce those addresses in a plain input-order link.
* Three source-order fixes are gate-neutral and move the no-order frontier
  47 -> 222; the next mismatch is a weak inline copy (`EaseInOutMatrix`).

**Hypothesis (not proven here):**

* If every explicit weak instantiation is moved to its original owner TU and
  every TU's source/first-use order is converged, the ordering file can be
  dropped and a plain input-order link will reproduce the original `.text`.
  The one-object experiment (§4.1) is a positive sample, but the second move
  (`ShaderBaseStruct` -> `Engine.cpp`) already costs 2 byte_exact, so the
  campaign must be done per symbol under the full gate.
* The `.ARM.exidx`/`.ARM.extab`/`.rodata` base shifts in the no-order link are
  consequences of gold's default empty-`.text`/marker placement, not of the
  function order itself. They need re-checking after the weak-copy convergence.

## 7. Reproduction scripts

Committed alongside this note:

* `_work/reconstructions/w3696/reorder_elf_sections.py` — permute an ELF32
  object's `.text.*` section headers and rewrite section-index references.
* `_work/reconstructions/w3696/relink_noorder.py` — exact gold relink with the
  ordering-file argument omitted.
* `_work/reconstructions/w3696/addr_match.py` — count exported functions at
  their original address and the exact-address prefix.

The all-object reorder loop is the `python3 - <<'PY'` block in §3. The weak-copy
span table is reproducible with:

```sh
python3 - <<'PY'
import os, sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_SYMTAB
orig = Elf32('../_work/bins/android_2.0.16_libgof2hdaa.so').functions()
for p in [l.strip() for l in open('cmake-build-match/exidx_markers/link_order.rsp') if l.strip()]:
    e = Elf32(p)
    a = [orig[s['name']]['value'] for s in e._symbols(SHT_SYMTAB)
         if s['shndx'] and (s['info'] & 0xf) == 2 and s['name'] in orig]
    if a and os.path.basename(p) in ('o191_Array.cpp.obj',
                                     'o192_ArrayInstantiations.cpp.obj'):
        print(os.path.basename(p), len(a), hex(min(a)), hex(max(a)))
PY
```
