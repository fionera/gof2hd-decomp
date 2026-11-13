# AGENTS.md — working on the GoF2 decomp

Guidelines for any agent (or human) making changes in this repo. Read this before touching `src/`.
The project is a **byte-matching decompilation** of the Android `libgof2hdaa.so` (Galaxy on Fire 2 HD
2.0.16, armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`) with the end goal of a byte-identical `.so` and a
portable, readable C++ tree. Correctness is defined by the binary, not by "looks right".

## Ground truth, in order of authority

1. **The original binary** — `../_work/bins/android_2.0.16_libgof2hdaa.so` (outside the repo).
   Loaded in Ghidra (MCP) as `android_2.0.16_libgof2hdaa.so`, image base 0x10000. Use
   `get_struct_layout` for field offsets and `decompile_function` / manual Thumb disasm for behavior.
   Ghidra pseudocode can be broken (e.g. `operator new` marked no-return drops case bodies) — fall
   back to disassembly when a function looks truncated.
2. **Original source-layout dumps** — `../source file list dump/` (iOS/mac/windows/MeeGo builds with
   symbol info: source path → object file → exact symbols per TU). Parsed and analyzed into
   `_work/original_layout/` (`tu_mapping.md`, `original_layout_notes.md`, `original_layout.json`).
   This is the authority on **which functions/statics belong in which TU** and on original file names.
3. **DeepOpen** (`../DeepOpen/`) — readable Java recreation of the J2ME v1.0.4 game; a logic oracle
   only. v2 constants/thresholds frequently differ (e.g. raid thresholds 90/65/35/10 vs Java's
   80/60/35/10) — always re-verify constants against the binary.
4. Sibling ports (`../gof2-vita`, `../KaamoClubModApi`, `../gof2edit`, `../FMOD-GOF2-Recreated`) —
   reference for platform/runtime/data-format behavior.

## Build & gates

```
cmake --preset default                                  # once; downloads NDK r18b
cmake --build cmake-build-match --target link_so        # compile + gold-link libgof2hdaa.so
cmake --build cmake-build-match --target diff           # sodiff parity (tools/sodiff_allow.txt applied)
cmake --build cmake-build-match --target verify         # per-function ASM match -> report.json (auto-synced to repo root)
FN=<mangled> cmake --build cmake-build-match --target verify-fn   # one function, side by side
cmake --build cmake-build-match --target lint           # goal_lint structural invariants
cmake --build cmake-build-match --target drift          # field_0xNN offset check (NDK clang, AAPCS)
cmake --build cmake-build-match --target check          # tools/ratchet.py: the enforced gate
```

The original `.so` path is configured in `CMakeLists.txt` (`GOF2_ORIGINAL_SO`, default
`../_work/bins/...` relative to the repo; a nested worktree must pass
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` and, if
the cache already holds the default, `-DGOF2_FMOD_LIBDIR=.../apk_libs`).

**Build recipe (2026-11-03).** The build is **CMake + NDK r18b**, not ndk-build. It compiles
every TU with clang 7.0.2 and the original's flags:
`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
-std=c++11 -fPIC -frtti -fstack-protector-strong -ffunction-sections -funwind-tables -Oz -g
-DANDROID -DNDEBUG -stdlib=libc++`. The link is a single plain **gold** link *without* `-s`
(`-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`), followed by the
original's post-link GNU `arm-linux-androideabi-strip --strip-all` and
`tools/reorder_shstrtab.py` (restores gold's Stringpool `.shstrtab` order). There is no
`--section-ordering-file`, no second link pass, and no `--unresolved-symbols` switch:
`tools/source_order.txt` is the checked-in object/TU link order, and gold's default
shared-library undefined-import handling is used. `tools/sodiff_allow.txt` is empty.

## The no-degradation gate (mandatory for every change)

The gate is machine-checked: `tools/ratchet.py` (the `check` target, and the pre-push hook installed
via `git config core.hooksPath tools/hooks`) compares a fresh build against
`tools/verify_baseline.json` + `tools/sodiff_allow.txt` and fails on any regression:

- `byte_exact` must **never decrease**. This is the primary hard count.
- `linked_exact` may dip by 1 on a single link (section-address transient) but must recover; a
  persistent drop means revert.
- `stub_zero_size` / `missing` / `wrong_type` / verify `extra` must not grow; defined exports
  beyond the allowlist and **new undefined imports** (a typo'd callee becomes a silent import —
  the ratchet is the only thing that catches it) are hard failures.
- `lint` must stay `CLEAN ✓`. Never `--update-baseline` (either tool) to absorb a regression you
  caused.
- Improvements are locked in the same commit with `python3 tools/ratchet.py --update-baseline`
  (it refuses if undocumented exports exist). State the real before/after counts in the commit
  message — the history is machine-checked later.
- If a pass changes any number that `docs/VALIDATION.md` or `docs/IDENTICALITY.md` states inline
  (counts, the irreducible-delta list), **update the doc in the same commit**. Stale doc numbers
  have already caused silent regressions to be misread as fine.

Known gate limitations (do not lean on them):

- `linked_exact` compares intra-function branch offsets literally and cross-function calls by
  resolved symbol name (PLT-aware). Calls into **local (non-exported) code** still mask (`#X`) —
  a wrong callee among local helpers is invisible; cross-check reconstructions against Ghidra.
- The `drift` target is real now (NDK clang, AAPCS, forced layout completion). The former
  **74-field backlog across 8 classes** (PlayerFighter, Radar, GameData, ParticleSystemMesh,
  PlayerAsteroid, IParticleSystem, Hud markers, PlayerTurret) has been reconciled; a clean run
  now reports **0 drifted fields**. Keep it at zero and re-check against Ghidra
  `get_struct_layout` when changing a struct.
- Lint waivers are same-line only, every criterion — a waiver on a neighbouring line waives
  nothing.

## Current state (2026-11-13, wave 191b)

`check` is green: byte_exact **4508/4524**, linked_exact **4508/4524**, size-exact
**4524/4524**, data_exact **843/843**, imports **207** (identical to the original), lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing). 16 functions are not
byte-exact. All **27 original ELF section headers are byte-identical** (no extra section);
`.text`, `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.dynsym`, `.dynstr`, `.hash` and
`.rel.dyn` are size- and base-exact, and `.rel.plt` content is byte-identical. `.ARM.extab` is
size- and base-exact at `0x26298` (wave-102 LSDA-scope pass). The only
differing section bytes are `.text` (74,735), `.ARM.extab` (561) and
`.note.gnu.build-id` (20); `.ARM.exidx` and `.rodata` are now byte-identical.

## Source rules (hard)

- No `asm` in any spelling (`asm`, `__asm`, `__asm__`, `asm volatile`), no `__attribute__`, no
  `__declspec`, no explicit template instantiation outside `ArrayInstantiations.cpp`, no include
  cycles. The lint scanner has known blind spots for some spellings — the rule is the rule, not the
  regex.
- `extern "C"` only with a same-line `// lint: extern_c` waiver, and only when the symbol is
  exported unmangled by the original (JNI `Java_*`, `ndk*`, Blowfish, GL/libc ABI).
- One class per header. `void*` and `union` only with same-line waivers at the documented
  ABI-mandated sites.
- No linker tricks: single plain gold link, no `--version-script`, no second pass, no
  `--section-ordering-file`, no `--unresolved-symbols` switch. The link order is the checked-in
  plain source list `tools/source_order.txt` (gold emits symbols in input order). Undefined
  app-provided imports (FMOD, GLES, …) stay unresolved exactly as in the original; the import set
  is pinned by the ratchet (207 names), so a typo'd callee is a hard failure. Duplicate strong
  definitions are hard link errors — the flag that used to mask them
  (`--allow-multiple-definition`) hid a real triplicate of the Vector/Matrix operators for weeks.
  Do not add link flags to make a symbol problem go away — reshape the C++.

## Correctness pitfalls (each of these has already produced real bugs)

- **Never leave a non-void function without a `return`.** Falling off the end is UB; clang at
  `-Oz` deletes the entire body, the symbol becomes size 0, and it silently aliases the next
  function's address (e.g. `MTitle::OnKeyPress`). If you must stub, return a value and mark
  `// TODO(decomp)`.
- **Never call through a file-static function pointer that is never assigned** (`static Fn* f =
  nullptr; ... f(...)`). Same UB body-deletion (killed `MeshMerger::render`, `PlayerEgo::stopBoost`,
  `ImageFactory::reload`, `SpriteGun::update`, …). 12 such stubs exist; when you touch one,
  reconstruct the real body from Ghidra instead of preserving the stub.
- **Namespace-scope globals become exported dynsym symbols.** The original exports almost none.
  New module state must be file-`static` / anonymous-namespace unless cross-TU opacity is required
  for codegen matching; if a global genuinely must be exported, document it in
  `docs/VALIDATION.md`'s irreducible-delta list in the same commit.
- **Constants from DeepOpen/Java are v1** — verify every threshold/table against the v2 binary's
  literal pools before use.
- Undefined shim imports (`Mat_assign`, `MeshCreate`, …) resolve to nothing at link time. The
  ratchet pins the 207-name import set, so a typo'd callee is a hard failure; when calling engine
  internals, copy the exact shim name from an existing use.

## TU organization policy (from the original source dumps)

The original layout (see `_work/original_layout/original_layout_notes.md`): a flat engine tree
(`AE2/Classes/`, one `Shader/` subfolder) + a flat game tree (`Classes/Game/`, only
`ParticleSystems/` and `MeshMerger/` subfolders) + platform glue. Our `src/engine|game|platform`
folder taxonomy is fine — **folder paths are free, TU membership is not**:

- `-ffunction-sections` + the checked-in `tools/source_order.txt` link order reproduce the
  original `.text` order regardless of file layout, but **`.rodata`/`.data` layout, string-literal
  pools, `__GLOBAL__sub_I_*`
  initializers (original has 3 `.init_array` entries; we have the same 3), per-TU statics, and -Oz
  same-TU inlining all follow TU membership**. Byte-identicality of data requires the original's
  TU ownership and link order.
- Before adding or moving a function/class, check `_work/original_layout/tu_mapping.md` for its
  original TU. New code goes in the original TU. Do not invent new TUs when a witnessed original
  TU exists.
- Known divergences to migrate toward (one merge per pass, under the full gate): dismantle the
  invented `engine/core/AbyssEngine.cpp` mega-TU into the original `Image/Image2D/ImageFont/
  SpriteSystem/Material/Mesh/Camera` engine TUs (this is also the work-breakdown for the ~1260
  unimplemented engine functions); fold `AbstractGun→BeamGun.cpp`, `KeyFrame→Transform.cpp`,
  `Node→SystemPathFinder.cpp`, `BSphere→Mesh.cpp`, `AENormalFile/AEPakFile→AEFile.cpp`,
  `ParticleSettingsRef→ParticleSettings.cpp`, `Resource*→PaintCanvas.cpp`-shaped TU; recreate
  `GameInit.cpp` and `GameResource.cpp`.
- Files with **no** dump witness (the GLES2 shader zoo, `SpriteGun`, `PlayerCreature`,
  `VirtualInput`, `NFC`, …) are Android-2.0.x era: restructure them only from binary evidence
  (string/rodata adjacency), never by analogy.

## Struct reconstruction methodology

1. Get the authoritative layout from Ghidra (`get_struct_layout`); never guess from decompiled
   member accesses of a single method.
2. Reorder/repad the header **atomically** (the whole struct, not a few fields), renaming
   `field_0xNN` placeholders so the name always equals the real original offset.
3. Rebuild + relink + verify: keep only if byte_exact/linked_exact rise and parity holds; else
   revert. Cross-file `field_0xNN` renames must be receiver-checked (the same placeholder name is
   reused by unrelated classes).

## Analysis artifacts & process

- Durable analysis goes in `_work/reconstructions/` (tracked). An analysis must be
  **self-contained**: inline the reconstructed body or commit it alongside — never reference a
  session/agent transcript as the only copy of the work.
- Pass commits: `<Function/Area> pass N: <what>` with honest before/after gate counts.
- Push after landing passes; local `master` has drifted hundreds of commits ahead of the remotes
  before.
