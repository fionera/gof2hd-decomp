# Engine mega-TU backlog batch — Image/Image2D/ImageFont/SpriteSystem/Material/Mesh/Camera (w7619)

## Finding: the named backlog is already reconstructed

The assignment named the former `engine/core/AbyssEngine.cpp` mega-TU backlog:
`Image`, `Image2D`, `ImageFont`, `SpriteSystem`, `Material`, `Mesh`, `Camera`.
In this worktree that mega-TU was already dismantled (w3613) and every exported
function in those seven TUs is already **byte- and link-exact** against the
Android 2.0.16 binary.  There is no unimplemented function left in those TUs to
reconstruct.

Evidence (fresh `cmake --build cmake-build-match --target check -j8`):

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0 absent 0)  extra 0
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

All of the following `report.json` rows are `pct 100.0, byte true, linked true`
(selected addresses/sizes from the original dynsym):

| TU | function | address | size |
|---|---|---|---|
| Mesh.cpp | `Mesh::Mesh(Mesh*)` C1/C2 | 0x6ba90 | 284 |
| Mesh.cpp | `MeshConvertToVBO` | 0x6bbac | 64 |
| Mesh.cpp | `MeshRelease` | 0x6c362 | 30 |
| Mesh.cpp | `MeshCreate` | 0x6c4d4 | 308 |
| Mesh.cpp | `MeshConvertToVBOIntern` | 0x6d510 | 720 |
| Mesh.cpp | `MeshCreateFromFile` | 0x6d074 | 824 |
| Mesh.cpp | `MeshDraw` | 0x6d3ac | 356 |
| Mesh.cpp | `MeshReadData` | 0x6c608 | 2632 |
| Image.cpp | `ImageCreateRegionFromFile` | 0x6eeb8 | 644 |
| Image.cpp | `ImageCreateFontFromFile` | 0x6f13c | 920 |
| Image.cpp | `ImageCreateFromFile` | 0x6f4d4 | 760 |
| Image.cpp | `TextureCreateFromFileIntern` | 0x6f7f4 | 2036 |
| Image.cpp | `TextureCreateFromFile` | 0x6f7cc | 40 |
| Image.cpp | `ImageRelease` | 0x6ee94 | 34 |
| Camera.cpp | `CameraSetPerspective` 6-arg | 0x7130c | 180 |
| Camera.cpp | `CameraSetPerspective` 5-arg | 0x713c0 | 140 |
| Camera.cpp | `CameraIsPointinViewFrustum` | 0x7144c | 384 |
| Camera.cpp | `CameraIsSphereinViewFrustum` | 0x715cc | 436 |
| Image2D.cpp | `Image2DRelease` | 0x71b64 | 28 |
| ImageFont.cpp | `ImageFontCheckString` | 0x72130 | 14 |
| ImageFont.cpp | `ImageFontGetWidth` (2 overloads) | 0x7213e/0x721e6 | 124/128 |
| ImageFont.cpp | `ImageFontGetHeight` | 0x721ba | 32 |
| ImageFont.cpp | `ImageFontGetYOffset` | 0x721da | 12 |
| ImageFont.cpp | `ImageFontSetSpacing` / `GetSpacing` | 0x72266/0x7226c | 6/12 |
| ImageFont.cpp | `ImageFontDrawString` (2 overloads) | 0x71b80/0x71bc0 | 58/1392 |
| ImageFont.cpp | `ImageFontRelease` | 0x7227e | 106 |
| SpriteSystem.cpp | `SpriteSystemCreate` | 0x86adc | 400 |
| SpriteSystem.cpp | `SpriteSystemSetAllUv` | 0x86c6c | 102 |
| SpriteSystem.cpp | `SpriteSystemSetUv` | 0x86cd4 | 112 |
| SpriteSystem.cpp | `SpriteSystemSetRGBA` | 0x86d44 | 116 |
| SpriteSystem.cpp | `SpriteSystemSetAllSize` | 0x86db8 | 34 |
| SpriteSystem.cpp | `SpriteSystemDraw` | 0x86ddc | 648 |
| SpriteSystem.cpp | `SpriteSystemRelease` | 0x87064 | 84 |
| Material.cpp | `MaterialDraw` | 0x870b8 | 416 |

A whole-`.text` byte-diff aggregation (original vs ours, 0x69440..0x1cc208)
attributes **zero differing bytes** to any symbol in these seven TUs.  The only
differing `.text` bytes are in the 18 known non-exact game-side functions
(`LevelScript::process`, `Radar::draw`, `Layout::Layout`, ...).  So the engine
backlog is not a source gap in this tree.

## What was changed (readability/fidelity batch, gate unchanged)

Rather than invent a function that already exists, this pass reconstructed the
expressions of seven engine functions from the original disassembly into
human-style C++ while preserving byte identity:

1. `CameraSetPerspective` (6-arg, 0x7130c): replaced the `float *f = (float*)cam`
   offset punning with the real `Camera` fields (`position[0..2]`,
   `frustumTanHalfFov`, `field_0x4c`, `frustumAspect`, `frustumInvCosX/Y`).
2. `CameraSetPerspective` (5-arg, 0x713c0): same field reconstruction.
3. `ImageFontGetHeight` (0x721ba): named the glyph-height load and the original
   `cmp #24; subeq #5` rule (`height == 24 -> height -= 5`).
4. `ImageFontGetWidth` (count overload, 0x7213e): removed the redundant
   `(float*)` cast on `Mesh::positions` (already `float*`).
5. `ImageFontGetWidth` (start/count overload, 0x721e6): same redundant-cast
   cleanup.

The original disassembly confirms the field offsets used: `position` at 0x0,
`frustumTanHalfFov` 0x48, `field_0x4c` 0x4c, `frustumAspect` 0x50,
`frustumInvCosX` 0x54, `frustumInvCosY` 0x58; and `ImageFontGetHeight` is
`vldr s0,[r0,#28]; vcvt.s32.f32; cmp r0,#24; it eq; subeq r0,#5`.

## Gate before/after

```
before: byte_exact 4506/4524  linked_exact 4506/4524  size 4524/4524
        data 843/843  imports 207  lint CLEAN  drift 0
after:  byte_exact 4506/4524  linked_exact 4506/4524  size 4524/4524
        data 843/843  imports 207  lint CLEAN  drift 0
```

`check` and `drift` both pass; no baseline update was needed because the counts
are unchanged.  The changed functions remain `pct 100.0, byte true, linked true`
with identical sizes (180, 140, 32, 124, 128 bytes).
