# Binary identicality: gap analysis & roadmap

Goal: produce a `libgof2hdaa.so` byte-identical to `../_work/bins/android_2.0.16_libgof2hdaa.so`
(same sha256). This document measures the gap and tracks progress. Measured against the original
(2,187,328 bytes) with our stripped link output.

## Update 2026-10-26

Current: **byte_exact 1352, linked_exact 4467, avg 99.77%**. libcrypto is OpenSSL 1.1.0l via
NDK r13b GCC 4.9 -O3 ARM (adds the original's genuine `memcpy`/`memset`/`mkstemp` imports). Adds `-g` (authentic ndk-build
default; keeps entry-block argument copies from PostRA sinking), the app flags for vendored
zlib/libzip (`-Oz -g -fstack-protector-strong -ffunction-sections`, was `-O2`), and source-order
placement of local functions in the section-ordering file. See docs/VALIDATION.md.

## Update 2026-07-26 — historical (superseded by the update above)

The verify metric was rebuilt (v2: symbol-aware call targets, PLT-aware, literal-pool words
compared as data, size-checked linked_exact, autojunk off — see docs/VALIDATION.md) and the
vendored zlib/libzip/SHA ISA modes fixed (131 functions were compared in the wrong ARM/Thumb
mode). Current: **compared 4523, byte_exact 1352, linked_exact 4467, avg 99.77%, stub_zero_size
0, missing 0, sodiff extra 52 (all allowlisted), undefined imports 601**. Historical figures in the sections below
(923/1636, 870/1569, 35/0/0, extra 9–35) are kept as a record of past states — do not quote them
as current. The no-degradation gate is now machine-checked: `tools/ratchet.py` vs
`tools/verify_baseline.json` (cmake `check` target + pre-push hook). Two new prerequisites for
byte-identicality were established from the original source-layout dumps
(`_work/original_layout/`): original **TU membership** (which functions/statics/literals compile
together — .rodata/.data order, `.init_array` (orig 3 entries vs our 1), string pools and -Oz
same-TU inlining all follow it) and original object **link order**. The `drift` target (fixed to
the real AAPCS ABI) reports 0 drifted fields; the former 74-field backlog has been reconciled.

## Section-size deltas (stripped ours vs original)

| section | orig | ours | delta | meaning |
|---|---|---|---|---|
| **.text** | 1,259,464 | 1,262,528 | **+3,064** | the remaining 56 non-linked exported functions plus the documented MenuTouchWindow 0x1000 layout anchor; local-code gaps are bracketed by the same neighbours. |
| **.rodata** | 269,159 | 268,007 | **−1,152** | anonymous const/string data still differs; exported objects are exact; the shader GLSL cluster is now exact. |
| .ARM.extab | 156,312 | 151,764 | −4,548 | smaller EH tables in the remaining non-exact functions. |
| .dynsym | 89,248 | 89,248 | 0 | same symbol count; order is still converging. |
| .dynstr | 189,208 | 189,208 | 0 | same string table size. |
| .data | 10,604 | 10,616 | **+12** | anonymous initialised-data layout still differs; the Transform pre-.got anchor now lives in .aezdata and the DialogueWindow last face row is 4 ints. |
| .bss | 27,052 | 27,280 | **+228** | anonymous zero-initialised-data layout still differs. |

## The four real gaps (in priority/size order)

1. **56 non-linked exported functions.** 4467 of 4523 compared functions are linked-exact; the
   remaining 56 are the long tail (20 in 90–99%, 13 in 80–89%, 9 in 70–79%, 11 in 60–69%,
   3 below 50%). The `.text` total is +3,064 B because of the documented
   MenuTouchWindow layout anchor; the remaining function bodies still net smaller, so this is a
   finishing campaign, not a missing-code campaign.
2. **Anonymous data layout.** `.rodata` is 1,152 B smaller, `.data` 12 B larger, and `.bss` 228 B larger than the original;
   exported data objects already match 843/843. The residual is local const/string/initialised data
   and literal pools owned by the remaining non-exact TUs.
3. **EH tables.** `.ARM.extab` is 4,548 B smaller, following the remaining function-size/EH-layout
   differences.
4. **Link metadata.** `.dynsym`/`.dynstr` sizes now match exactly; dynamic-symbol order and the GNU
   build-id are content hashes that converge only when everything else does.

## Done
- **Pass 1: strip** the link output (`strip --strip-all`) -> section set matches the original
  (no .symtab/.strtab/.debug_*). Parity unaffected (35/0/0); verify/sodiff read .dynsym+.text.

## Reality
Same-hash == completing the decompile to byte-perfection: make the remaining 56 functions exact,
recover the residual anonymous const/initialised data, and match the exact link layout. Progress is
tracked by: byte_exact/linked_exact function counts (verify) and the section-size deltas above.

## Next levers (tractable, bounded first)
- Pin the .rodata null source (largest zero runs -> owning TU) and fix oversize/placeholder const.
- Reduce the handful of extra dynsym symbols (restrict libcrypto exports; fold the ctor aliases).
- Byte-exactness campaign: verify.py worst-first, fix near-miss (80-99%) high-value functions first.
- Implement missing engine functions in batches (math/Vector/Matrix shims are the densest cluster).

## Concrete leads found (for the byte-exactness campaign)

The verify report (cmake-build-match/verify/report.json) has per-function rows: 1352 byte_exact,
4467 linked_exact, 4499 size_match of 4523; pct buckets {100:4467, 90-99:22, 80-89:11, 70-79:10,
60-69:10, <50:3}. 24 functions size-MISMATCH (need restructuring); 25 size-match >=80%
not-linked (the highest-value near-misses).

Systematic clusters (one cause fixes many):
1. **Shader `UpdateMeshData` cluster** (~15 fns at 86-94%, GenericShader/VertexColorShader/BumpShaderV2/4/
   CubeMapping/EnergyShield/...). Diff via direct disasm: the only real (non-relocation) deltas are
   field-offset drift. In `GenericShader`, fields from `uSpecularColor` onward are +4 in the original
   (orig 0x5c/0x60/0x68/.. vs ours 0x58/0x5c/0x64/..) -> our decomp is missing a 4-byte uniform-location
   field between uDiffuseColor and uSpecularColor (or has them mis-ordered). Plus a 1-byte Mesh field
   drift (orig [r4,#9] vs ours [r4,#10]). Fixing the shared base/layout flips the whole cluster.
   NOTE: this is layout drift that field_0xNN scanning can't see (semantic-named fields) -- a class of
   drift beyond [[drift-campaign]]. A generalized ASM-offset-diff drift detector would surface all of it.
2. **ParticleSettings_str[401401]={0}** -- 392KB zero array (the .rodata bloat), a mis-sized decomp
   artifact; the real particle-name string table is a few KB. Needs the original's data recovered
   (the `buf` results are dead in our decomp, so it's correctness-neutral but blocks .rodata identicality).

## PaintCanvas layout reconstruction (in progress — concrete data)

offdrift + per-method disasm give the original offsets (vs ours) for PaintCanvas fields:
- `engine`        orig 0x34  ours 0x3c   (GetWidth, SetWorldViewMatrix: `ldr [r0,#0x34]`)
- `meshCount`     orig 0x24  ours 0x30   (MeshSetUv: `ldr.w ip,[r0,#0x24]`)
- `meshes`        orig 0x28  ours 0x34   (MeshSetUv: `ldr [r0,#0x28]`)
Deltas: meshCount/meshes are +0xc (our struct has 12 extra bytes before 0x24); engine is +8 (so our
struct is ALSO missing 4 bytes between `meshes` and `engine` — orig has 2 fields there [0x2c,0x30],
we have 1 [`gameOrientation`]). Root: `::Array<AELoadedTexture*> cubeTextures` (12B, count/data_/
capacity_) is MIS-PLACED at 0x20 in ours; the original has it elsewhere. All early fields
(culledCount, quad2dMesh, field_0xc/14/1c, mask2dImage, gameOrientation, cubeTextures) ARE used, so
this is a RE-ORDER, not a deletion.

### Safe methodology for any struct (no-degradation gated)
1. Disassemble every method of the struct (ours+orig), align 1:1 (size-match), map each our-offset to
   its orig-offset; translate offsets->field names via the source. This yields the COMPLETE original
   field order (a full layout reconstructor tool over offdrift is the next tooling step).
2. Reorder/repad the header to match, atomically (the whole struct, not a few fields).
3. Rebuild + relink + verify: keep ONLY if byte_exact/linked_exact counts RISE and parity stays
   35/0/0; otherwise revert. The verify counts are the hard no-degradation gate.

This is the engine for the byte-exactness campaign; each reconstructed struct flips many functions.
Dozens of structs drift (offdrift delta histogram: -8:14, +4:12, +8:11, +32:9, ... across 300 fns).

## GROUND-TRUTH unblock: Ghidra has the original loaded

The original (android_2.0.16_libgof2hdaa.so, 8096 fns, image_base 0x10000) is OPEN in Ghidra (MCP).
`get_struct_layout <Struct>` returns the real field offsets; `decompile_function <addr>` shows real
field usage. This is the authoritative source for the byte-exactness campaign — reconstruct each
struct from Ghidra's layout, not from guesses. The verify byte_exact/linked_exact count remains the
no-degradation gate when applying.

### PaintCanvas — definitive layout (Ghidra get_struct_layout, size 524):
0x0 gravityRotationEnabled(=initialized), 0x4 culledCount, 0x8 field_0x8(=quad2dMesh), 0xc field_0xc,
0x10 field_0x10, 0x14 field_0x14, [0x18 gap], 0x1c field_0x1c, 0x20 field_0x20(int), 0x24
field_0x24(=meshCount), 0x28 field_0x28(=meshes), [0x2c gap=mask2dImage], 0x30 orientation
(=gameOrientation), 0x34 engine, 0x38.. matrices, ... 0x150 images, 0x164 cameras, ...
DECOMP BUG: `ChangeCubeTexture` (decompiled) reads cube count@field_0x10 / data@field_0x14 -> the
cube-texture Array IS field_0x10/0x14/0x18 (count/data/capacity @ 0x10). Our decomp instead made a
SEPARATE `cubeTextures` Array at 0x20, displacing field_0x20/meshCount/meshes by +0xc (root of the
~10 PaintCanvas near-misses, e.g. engine 0x3c-should-be-0x34). FIX: map `cubeTextures`->0x10 (merge
with field_0x10/0x14/0x18 uses), add the standalone `field_0x20` int, shift meshCount/meshes/
gameOrientation/engine back by 0xc. Verify-gate each step.
