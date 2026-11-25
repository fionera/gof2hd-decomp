# TU-merge A/B: BSphere → Mesh.cpp, ParticleSettingsRef → ParticleSettings.cpp,
# AENormalFile/AEPakFile → AEFile.cpp

Worker: w15098 (wave193). Base revision: `bac641cc9` (2026-11-25).
Repo: `/home/debian/GalaxyOnFire2/wt/w15098`.
Original binary: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; all addresses below are real ELF addresses).

## Question

The source-layout dump says these classes belonged to TUs that currently hold more than one
class:

| class / group | dump-declared original TU | current tree file |
|---|---|---|
| `AEMath::BSphere` | `Svn_iPhone_AE2/Classes/Mesh.cpp` (`_work/original_layout/tu_mapping.md`, row `engine/math/BSphere.cpp`) | `src/engine/math/Vector.cpp` |
| `ParticleSettingsRef` | `Classes/Game/ParticleSystems/ParticleSettings.cpp` | `src/engine/render/ParticleSettings.cpp` |
| `AENormalFile`, `AEPakFile` (+ `ArrayAdd<AEPakFileEntry>`, `AEPakFile::Write`) | `Svn_iPhone_AE2/Classes/AEFile.cpp` | `src/engine/file/AEFile.cpp` |

The second and third merges are already present in the tree. The first is a mismatch: the
Android binary evidence (authority #1) puts BSphere at the tail of the `Vector.cpp` TU, not in
the `Mesh.cpp` TU. This note records a controlled A/B move of BSphere into `Mesh.cpp` and a
static address audit of the two already-merged groups.

## Baseline (before experiment)

```sh
cd /home/debian/GalaxyOnFire2/wt/w15098
cmake --build cmake-build-match --target check -j4
```

Measured (`/tmp/w15098-final-baseline-check.log`):

```text
byte_exact    4508/4524
linked_exact  4508/4524
size-exact    4524/4524
data_exact    843/843
imports       207
RATCHET PASS
```

`lint` and `drift` were also run explicitly:

```sh
cmake --build cmake-build-match --target lint  -j4   # CLEAN ✓
cmake --build cmake-build-match --target drift -j4   # DRIFT: 0 field(s) across 0 class(es)
```

## Experiment A: move BSphere out of Vector.cpp into Mesh.cpp

Edit: deleted `src/engine/math/Vector.cpp` lines 212–313 (the `MatrixTransformVector` /
`MatrixRotateVector` forward declarations, `CopyVector`, and the three `BSphere` definitions)
and appended the same block to the end of `src/engine/render/Mesh.cpp` inside
`namespace AbyssEngine { namespace AEMath { ... } }`. No other change.

Reproducible edit:

```sh
cd /home/debian/GalaxyOnFire2/wt/w15098
python3 - <<'PY'
from pathlib import Path
v = Path('src/engine/math/Vector.cpp')
m = Path('src/engine/render/Mesh.cpp')
lines = v.read_text().splitlines(keepends=True)
block = ''.join(lines[211:313])                 # 1-based lines 212..313
v.write_text(''.join(lines[:211] + lines[313:]))
m.write_text(m.read_text().rstrip('\n') + '\n\nnamespace AbyssEngine {\nnamespace AEMath {\n'
             + block + '}\n}\n')
PY
cmake --build cmake-build-match --target check -j4
```

Measured (`/tmp/w15098-ab-bsphere-mesh-rerun.log`; the first run in
`/tmp/w15098-ab-bsphere-mesh-check.log` gave the identical result):

```text
verify: byte 3458  linked 4508  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4508 -> 3458
RATCHET FAIL
```

Link-visible effect (`readelf -sW cmake-build-match/libgof2hdaa.so`):

| symbol | before (Vector.cpp) | after (Mesh.cpp) |
|---|---|---|
| `BSphere::Merge(Transform const&)` | 0x00073f31, 208 B | 0x0006dc15, 208 B |
| `BSphere::Merge(BSphere const&)` | 0x00074001, 324 B | 0x0006d061, 324 B |
| `BSphere::operator=(BSphere const&)` | 0x00074145, 16 B | 0x0006bbed, 16 B |

The function bytes themselves are unchanged (`linked_exact` stayed at 4508), but the section
moved from the `Vector.cpp` region into the `Mesh.cpp` region. Because the build uses
`-ffunction-sections` and a fixed TU link order, relocating this block shifts the placement of
all later functions, which changes literal-pool/PC-relative branch offsets for the ~1050
functions after the old position; `byte_exact` drops by exactly 1050 (4508 → 3458).
This is a **strict regression**, so BSphere must stay in `src/engine/math/Vector.cpp`.

### Original binary confirms the Vector.cpp placement

`readelf -sW` on the original, sorted by address, gives the neighborhood:

```text
0x00073eed  66  AEMath::VectorLerp(Vector const&, Vector const&, float)      # Vector.cpp
0x00073f31 208  AEMath::BSphere::Merge(Transform const&)                    # <-- BSphere
0x00074001 324  AEMath::BSphere::Merge(BSphere const&)
0x00074145  16  AEMath::BSphere::operator=(BSphere const&)
0x00074155   8  IApplicationModule::SetApplicationManager(...)              # next TU
```

`tools/source_order.txt` has `src/engine/math/Vector.cpp` (line 30) immediately before
`src/engine/core/IApplicationModule.cpp` (line 31); `src/engine/render/Mesh.cpp` is line 18.
So the original's BSphere is emitted after `VectorLerp` and immediately before the
`IApplicationModule` TU, exactly where the current tree emits it. The dump's "Mesh.cpp"
owner for BSphere describes the SD/iOS source tree, but the Android 2.0.16 binary (ground
truth #1) clearly places the emitted symbols in the `Vector.cpp` TU region. Do not move it.

## Experiment B (static audit): ParticleSettingsRef already merged into ParticleSettings.cpp

No source change; audit of the current build against the original:

```sh
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -sW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -Ei 'ParticleSettingsRef'
$RE -sW cmake-build-match/libgof2hdaa.so            | grep -Ei 'ParticleSettingsRef'
```

Original and ours are identical, including address and size:

| symbol | address | size |
|---|---|---|
| `ParticleSettingsRef::cur` | 0x002183e8 | 7680 |
| `ParticleSettingsRef::init` | 0x0021a1e8 | 7680 |
| `ParticleSettingsRef::assertInit` | 0x0021bfec | 4 |
| `ParticleSettingsRef::initialize()` (weak, header-inline) | 0x000e3965 | 48 |

Object membership confirms the statics are emitted by the intended TU:

```sh
$RE -sW cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/ParticleSettings.cpp.obj \
  | grep 'ParticleSettingsRef'
# ParticleSettingsRef::assertInit / ::cur / ::init, all in ParticleSettings.cpp.obj
```

Verdict: **positive / already done**. Splitting the statics into their own TU would change
`.bss`/`.data` emission order and addresses (they are currently address-exact), so there is
nothing to gain.

## Experiment C (static audit): AENormalFile/AEPakFile already merged into AEFile.cpp

No split `.cpp` files exist; `src/engine/file/AEFile.cpp` defines AEFile, AEPakFile,
AENormalFile and AELowLevelFile as the dump says. Current original-vs-ours symbol audit:

```sh
$RE -sW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -Ei 'AENormalFile|AEPakFile'
$RE -sW cmake-build-match/libgof2hdaa.so            | grep -Ei 'AENormalFile|AEPakFile'
```

Every function, typeinfo and vtable is address- and size-identical. Representative rows:

| symbol | original | ours |
|---|---|---|
| `AEPakFile::AEPakFile(FileInterface*, int, int)` | 0x00070011, 36 B | same |
| `AEPakFile::Read(unsigned, void*)` | 0x00070069, 58 B | same |
| `AEPakFile::Write(unsigned, void*)` (weak) | 0x00071309, 4 B | same |
| `AENormalFile::AENormalFile(FileInterface*)` | 0x000700f9, 20 B | same |
| `AENormalFile::D1/D2` | 0x0007010d, 36 B | same |
| `ArrayAdd<AEPakFileEntry>` (weak) | 0x000705b5, 36 B | same |
| `typeinfo for AEPakFile` | 0x0020cc6c, 12 B | same |
| `typeinfo for AENormalFile` | 0x0020cc78, 12 B | same |
| `vtable for AEPakFile` | 0x0020cc24, 36 B | same |
| `vtable for AENormalFile` | 0x0020cc48, 36 B | same |

Object membership confirms all of these are emitted by
`CMakeFiles/gof2.dir/src/engine/file/AEFile.cpp.obj`. Verdict: **positive / already done**;
resplitting would move `.rodata` (typeinfo/vtable/string literals) and code addresses away from
the original.

## Summary table

| merge | baseline byte/linked/data | after forced move | verdict |
|---|---|---|---|
| BSphere → `Mesh.cpp` | 4508 / 4508 / 843 | **3458 / 4508 / 843** | **negative**; keep BSphere in `Vector.cpp` |
| ParticleSettingsRef → `ParticleSettings.cpp` | 4508 / 4508 / 843 | n/a (already merged) | **positive**; symbols address/size-exact |
| AENormalFile/AEPakFile → `AEFile.cpp` | 4508 / 4508 / 843 | n/a (already merged) | **positive**; symbols/typeinfo/vtables address-exact |

## Proven vs hypothesis

**Proven by controlled A/B rebuild (full `check` gate):**
- Moving the BSphere block from `Vector.cpp` to `Mesh.cpp` is a strict regression:
  `byte_exact` 4508 → 3458, `linked_exact`/`data` unchanged. The block must stay in
  `Vector.cpp` on the Android binary.
- The original binary's BSphere addresses (0x73f31/0x74001/0x74145) sit between the
  Vector TU's `VectorLerp` and the next TU `IApplicationModule.cpp`, matching current output.

**Proven by original-vs-ours `readelf -sW` and object-member audit:**
- `ParticleSettingsRef` statics/`initialize()` and the AENormalFile/AEPakFile function,
  typeinfo and vtable set are already emitted by the dump-declared TUs at the original
  addresses and sizes; data is `843/843` exact in the same build.

**Still hypothesis (not A/B-tested because already correct and cheap to leave alone):**
- Forcing ParticleSettingsRef or AENormalFile/AEPakFile into fresh split TUs would lower
  `byte_exact`/`data_exact` by shifting later `.text`/`.rodata` placement. The BSphere A/B
  demonstrates the mechanism, and both groups are currently address-exact, so there is no
  positive split to make.

## Final state

After the A/B, the tree was reverted (`git checkout -- src/engine/math/Vector.cpp
src/engine/render/Mesh.cpp`) and `check` re-run:

```text
byte_exact   4508/4524
linked_exact 4508/4524
data_exact   843/843
imports      207
RATCHET PASS
lint CLEAN, drift 0
```

No source change was committed for this study; only this note.
