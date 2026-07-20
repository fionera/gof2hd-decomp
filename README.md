# Galaxy on Fire 2 — decomp

A work-in-progress decomp of the *Abyss Engine* + *Galaxy on Fire 2* game
(Fishlabs, ~2012), recovered by decompiling the Android `libgof2hdaa.so`.

## Build
This project **is** the byte-matching build: it compiles every TU with the original NDK r18b
toolchain and validates the result against `libgof2hdaa.so`, function by function. CMake downloads
the NDK itself (`cmake/DownloadNDK.cmake`) and runs its clang/binutils natively — on Apple Silicon
the `darwin-x86_64` toolchain runs under **Rosetta 2** (`softwareupdate --install-rosetta` if
missing). Open the folder in **CLion** (it reads `CMakePresets.json` and configures automatically),
or from a shell:
```
cmake --preset default                                  # first run downloads NDK r18b (~500MB)
cmake --build cmake-build-match --target gof2           # compile every TU to ARM objects
cmake --build cmake-build-match --target link_so        # link libgof2hdaa.so (gold) the original's shape
cmake --build cmake-build-match --target diff           # whole-.so symbol-parity gate (tools/sodiff.py + allowlist)
cmake --build cmake-build-match --target verify         # per-function ASM match-% table vs the .so
cmake --build cmake-build-match --target lint           # structural invariants (tools/goal_lint.py)
cmake --build cmake-build-match --target drift          # field_0xNN offset check (NDK clang, AAPCS)
cmake --build cmake-build-match --target check          # no-degradation ratchet (tools/ratchet.py); also the pre-push hook
```
Single-function ASM diff: `FN=<mangled> cmake --build cmake-build-match --target verify-fn`.
`lint` is a ratchet: hard rules (no asm/`__attribute__`/extern-C, no explicit template instantiation,
no include cycles) must stay 0; the `void*`/`union`/one-class counts may only shrink — lock progress
with `python3 tools/goal_lint.py src --update-baseline`.
`compile_commands.json` is exported for clangd/CLion code intelligence. Requires Apple Silicon with
Rosetta 2 (the NDK r18b toolchain is x86_64). See **[docs/VALIDATION.md](docs/VALIDATION.md)**.

## Ground truth
The original binary is loaded in Ghidra as `android_2.0.16_libgof2hdaa.so` (ARM32). It remains the
authoritative reference for function signatures, struct field types, and behavior during the port.

## Credits & thanks
This decomp stands on the shoulders of the wider Galaxy on Fire 2 / Abyss Engine community. A huge
**thank you** to everyone whose work made it possible — go star their repos:

- **[DeepOpen](https://github.com/BaalNetbek/DeepOpen)** by **BaalNetbek** — a decompiled, deobfuscated
  recreation of the J2ME *GoF2 v1.0.4* and the common *Abyss Engine*. An invaluable logic oracle:
  when the C++ disassembly is murky, the readable Java reveals what a function is actually doing.
- **[KaamoClubModApi](https://github.com/1337Skid/KaamoClubModApi)** by **1337Skid** — a modding API
  for Galaxy on Fire 2, and a great reference for how the game's data and systems fit together.
- **[FMOD-GOF2-Recreated](https://github.com/KroeteTroete/FMOD-GOF2-Recreated)** by **KroeteTroete** —
  a replica of GoF2's FMOD Designer project, indispensable for understanding the audio/event layer.
- **[gof2-vita](https://github.com/v-atamanenko/gof2-vita)** by **Vladimir Atamanenko (v-atamanenko)** —
  a native PS Vita port of GoF2, a terrific reference for the engine's platform/runtime behavior.
- **[gof2edit](https://github.com/Ravernstal/gof2edit)** by **Ravernstal** — a CLI tool to unpack/repack
  the game's BIN and save files and patch binaries; great for understanding the on-disk data formats.
- **KiritoJPK** — for the *Galaxy on Fire 2 Full HD* Android packaging used while cross-referencing assets.

And of course **Fishlabs** (now Deep Silver Fishlabs) for creating *Galaxy on Fire 2* and the *Abyss
Engine* in the first place. This project is a non-commercial, educational reverse-engineering effort;
all original game code, assets, and trademarks remain the property of their respective owners.
