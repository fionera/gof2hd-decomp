# Galaxy on Fire 2 — native port

A work-in-progress **native, runnable port** of the *Abyss Engine* + *Galaxy on Fire 2* game
(Fishlabs, ~2012), recovered by decompiling the Android `libgof2hdaa.so` and rebuilding it as a
modern 64-bit C++ program that runs on macOS.

> This project began as a *byte-matching* decompilation (recompile to a binary-identical Android
> `.so`). That work is preserved in git history; the tree was since collapsed into this single
> CMake source tree focused on getting the game **running** natively. To browse the matching-era
> sources/tooling: `git log` for the pre-restructure commits, or
> `git checkout <commit> -- src/recovered tools matches.tsv`.

See `docs/DECOMP_NOTES.md` for the full reverse-engineering log.

## Layout (single CMake tree)
```
CMakeLists.txt        CMake project: library `gof2` + executable `gof2_host`
CMakePresets.json     debug/release presets (CLion auto-configures from these)
include/gof2/         foundation headers + one real-struct header per class
    common.h            shared types: Array<T> == std::vector<T>, String (UTF-16 game class), aliases
    math.h              3D math: AEMath::Vector / Matrix, Quaternion
    fwd.h               forward declarations of every class
    <Class>.h           real struct (named fields; no byte offsets)
src/<Class>.cpp       one .cpp per class (188), the engine + game logic
host/main.cpp         macOS entry point (Milestone 2 placeholder)
tools/                rewrite_body.py, reconcile.py, build.sh, classmap/fieldmap.json
docs/                 DECOMP_NOTES.md + reverse-engineering notes
assets_ref/           reference assets (for Milestone 3 asset mounting)
```

## Build
Open the folder in **CLion** (it reads `CMakePresets.json` and configures automatically), or from a shell:
```
cmake --preset debug
cmake --build cmake-build-debug --target gof2_host   # runnable placeholder today
cmake --build cmake-build-debug --target gof2        # the engine/game library (cleanup ongoing)
```
`compile_commands.json` is exported for clangd/CLion code intelligence. Requires a C++14 (gnu++14)
clang; no external deps yet (SDL2/GLES2 arrive with the platform layer).

## Roadmap
- **M1 — compile/link**: every TU in `src/` compiles; resolve undefined symbols into the host. *(in progress: the byte-offset → real-struct cleanup is being applied class by class; most TUs compile.)*
- **M2 — first frame**: real structs + entry point + window/GL (GLES2→OpenGL or ANGLE) + input.
- **M3 — main menu**: asset mounting (`.aei` textures, `.bin` tables, shaders, sounds).
- **M4 — playable**: fix logic bugs in coverage-mode functions → in-game.

## Ground truth
The original binary is loaded in Ghidra as `android_2.0.16_libgof2hdaa.so` (ARM32). It remains the
authoritative reference for function signatures, struct field types, and behavior during the port.
