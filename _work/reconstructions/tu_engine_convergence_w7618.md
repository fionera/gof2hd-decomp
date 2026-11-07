# Engine TU convergence: `ShaderBaseStruct.cpp` -> `Shader/BaseShader.cpp` (w7618)

Status: landed (2026-11-07, worktree `wt/w7618`, branch `wave127-w7618`).
Baseline commit `bf365a964` (byte 4506 / linked 4506).

## TL;DR

The invented `src/engine/core/AbyssEngine.cpp` mega-TU was already dismantled before this
pass: `Image.cpp`, `Image2D.cpp`, `ImageFont.cpp`, `SpriteSystem.cpp`, `Material.cpp`,
`Mesh.cpp` and `Camera.cpp` are separate TUs and every original SD-engine symbol is emitted
by exactly one of our objects. The only remaining engine-TU divergence was the **file name**
of the shader base class: the SD engine dump attributes
`ShaderBaseStruct::{ShaderBaseStruct,Update,UseShader}`, its vtable/typeinfo and
`shaderIndexIntern` to `Svn_iPhone_AE2/Classes/Shader/BaseShader.cpp`, while our tree called
the file `engine/render/ShaderBaseStruct.cpp` and `tu_mapping.md` wrongly recorded it as a
split of `Engine.cpp`.

This pass renames the file to `src/engine/render/shaders/BaseShader.cpp` (the `Shader/`
subfolder is our `shaders/`), keeps its `tools/source_order.txt` slot unchanged (line 71,
between `GenericShader2.cpp` and `GenericShader.cpp`), and corrects the mapping table
(adds the four missing Image/SpriteSystem rows and the BaseShader row).

## Evidence

### 1. The class belongs to the shader block, not `Engine.cpp`

The SD 1.0.1 engine dump (`original_layout.json`, `tu_symbols.gof2sd_1.0.1_ios`):

```
Svn_iPhone_AE2/Classes/Shader/BaseShader.cpp
    _AbyssEngine::ShaderBaseStruct::ShaderBaseStruct()
    _AbyssEngine::ShaderBaseStruct::Update()
    _AbyssEngine::ShaderBaseStruct::UseShader()
    _typeinfo name for AbyssEngine::ShaderBaseStruct
    _vtable for AbyssEngine::ShaderBaseStruct
    _typeinfo for AbyssEngine::ShaderBaseStruct
    _AbyssEngine::ShaderBaseStruct::shaderIndexIntern
```

`Engine.cpp` only emits the inline `ShaderRegister`/`GetProgramObject`/`Array<ShaderBaseStruct*>`
helpers, not the class methods.

The original `.text` confirms the placement. Exported shader neighbours (original `.dynsym`):

```
0x8e0d0  GenericShader2::GenericShader2()
0x8e750  GenericShader::GenericShader()
0x8eba0  GenericShader1::GenericShader1()
```

An unstripped reproduction of our link (same object list as `link_target`) puts the
`ShaderBaseStruct` methods at:

```
0x8e424  ShaderBaseStruct::ShaderBaseStruct()
0x8e4e8  ShaderBaseStruct::~ShaderBaseStruct()
0x8e4ea  ShaderBaseStruct::DeleteShader()
0x8e4f0  ShaderBaseStruct::ES2LoadShader(...)
0x8e584  ShaderBaseStruct::ES2LoadProgram(...)
0x8e674  ShaderBaseStruct::LoadBindShader(...)
0x8e744  ShaderBaseStruct::UseShader(bool)
0x8e74a  ShaderBaseStruct::Update()
```

i.e. entirely inside the `[0x8e0d0, 0x8e750)` window between GenericShader2 and
GenericShader. Merging the file into `Engine.cpp` (whose functions start at `0x6db20`)
would move it out of that window, so the old `tu_mapping.md` row was wrong.

### 2. No engine TU is still split

The committed probe `_work/reconstructions/w3613/tu_interleave.py` (run with a
`link_order.rsp` generated from `tools/source_order.txt`) reports:

```
objects linked: 192
objects with strong exported funcs: 190; range contains another object's strong func: 0
original TUs represented by >1 object (strong funcs): (none)
objects with no strong exported funcs: Array.cpp, ArrayInstantiations.cpp
```

The dump-attribution check over every SD-engine TU (strong symbols) likewise finds no
original TU represented by more than one of our objects. `Image.cpp`, `Image2D.cpp`,
`ImageFont.cpp`, `SpriteSystem.cpp`, `Material.cpp`, `Mesh.cpp` and `Camera.cpp` each own
their witnessed symbols; `GameData` is already emitted by `GameInit.cpp`, `Node` by
`SystemPathFinder.cpp`, `AENormalFile`/`AEPakFile` by `AEFile.cpp`, `ParticleSettingsRef`
by `ParticleSettings.cpp`, and `BSphere` by `Vector.cpp` (addresses `0x73f30/0x74000/0x74144`,
matching the original `0x73f31/0x74001/0x74145`).

## Change

* `git mv src/engine/render/ShaderBaseStruct.cpp src/engine/render/shaders/BaseShader.cpp`
* `tools/source_order.txt`: same line 71, path updated.
* `_work/original_layout/tu_mapping.md`: BaseShader row corrected; missing
  `Image.cpp`/`Image2D.cpp`/`ImageFont.cpp`/`SpriteSystem.cpp` rows added; stale
  "Original TUs with NO our-side file" section refreshed.

## Gate

```
before: byte 4506 / linked 4506 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0
after:  byte 4506 / linked 4506 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0
```

Whole-file `cmp` before/after: exactly **20 bytes** differ, all in the
`.note.gnu.build-id` descriptor (offset 477..496). The link is built with `-g`, so the
source path is embedded in the unstripped link output and gold hashes that output for the
build-id; the rename therefore changes only the build-id, which is the documented
irreducible delta. `.text`, `.rodata`, `.data`, `.bss`, `.ARM.exidx` and `.ARM.extab` are
unchanged.
