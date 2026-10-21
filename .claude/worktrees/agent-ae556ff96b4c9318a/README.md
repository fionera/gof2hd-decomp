# Galaxy on Fire 2 — decomp

A work-in-progress decomp of the *Abyss Engine* + *Galaxy on Fire 2* game
(Fishlabs, ~2012), recovered by decompiling the Android `libgof2hdaa.so`.

## Build
Open the folder in **CLion** (it reads `CMakePresets.json` and configures automatically), or from a shell:
```
cmake --preset debug
cmake --build cmake-build-debug --target gof2_host   # runnable placeholder today
cmake --build cmake-build-debug --target gof2        # the engine/game library (cleanup ongoing)
```
`compile_commands.json` is exported for clangd/CLion code intelligence. Requires a C++14 (gnu++14)
clang; no external deps yet (SDL2/GLES2 arrive with the platform layer).

### ASM validation (matching build)
To check whether our code still assembles to the **same instructions** as the original `.so`,
function by function, build with the matching NDK r18b toolchain (via OrbStack) and diff against the
binary:
```
bash tools/verify/setup.sh                              # one-time OrbStack provisioning
cmake --preset match
cmake --build cmake-build-match --target verify         # prints a per-function match-% table
```
This is independent of the native build above. See **[docs/VALIDATION.md](docs/VALIDATION.md)**.

## Ground truth
The original binary is loaded in Ghidra as `android_2.0.16_libgof2hdaa.so` (ARM32). It remains the
authoritative reference for function signatures, struct field types, and behavior during the port.
