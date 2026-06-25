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
cmake --build cmake-build-match --target verify         # per-function match-% table vs the .so
cmake --build cmake-build-match --target relink         # whole-.so symbol-parity gate
```
`compile_commands.json` is exported for clangd/CLion code intelligence. Requires Apple Silicon with
Rosetta 2 (the NDK r18b toolchain is x86_64). See **[docs/VALIDATION.md](docs/VALIDATION.md)**.

## Ground truth
The original binary is loaded in Ghidra as `android_2.0.16_libgof2hdaa.so` (ARM32). It remains the
authoritative reference for function signatures, struct field types, and behavior during the port.
