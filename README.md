# Galaxy on Fire 2 — matching decompilation

A work-in-progress **matching decompilation** of the *Abyss Engine* + *Galaxy on Fire 2* game
(Fishlabs, ~2012). Goal: C++ source that recompiles to byte-identical binaries, plus tooling
for all the game's asset formats.

See `docs/DECOMP_NOTES.md` for the full reverse-engineering log and rationale.

## Layout
```
include/   gof2_types.h        — recovered engine/game structs (32 structs, validated)
src/       engine/  game/      — recovered C++, organized by subsystem
tools/     aei.py gofbin.py    — asset tools (textures, data tables, strings)  [see tools/README.md]
build/     flags.sh build_fn.sh extract_target.sh — per-function byte-match harness  [see build/README.md]
docs/      DECOMP_NOTES.md unified_symbols.tsv
Makefile                       — builds src/ with the exact original toolchain
```

## Target & toolchain
Primary matching target: **Android `libgof2hdaa.so` 2.0.16** (armeabi-v7a, Thumb-2), built with
**NDK r18b / clang 7.0.2** — exactly reproducible. iOS 1.1.4 HD (unencrypted armv7) and iOS 1.0.5
are co-references; Windows `GOF2.exe` is reference-only (content-cut; e.g. `items.bin` has 196
records vs the full 233). The recovered C++ is shared across all platforms.

## Build (in the OrbStack guest where the NDK lives)
```
make NDK=/opt/android-ndk-r18b        # compile src/ to objects
make asm NDK=/opt/android-ndk-r18b    # emit .s for diffing against the target
```

## Matching workflow (per function)
1. In Ghidra, read the decompiled function + apply recovered types.
2. Author the C++ in `src/<subsystem>/`.
3. `make asm` → compare to the target via `build/extract_target.sh <elf_vaddr> <n>`.
4. Iterate source/flags until the disassembly matches (see `build/README.md`).

Proven: `src/engine/array.cpp::ArrayAdd` matches the target **15/15 instructions**.

## Status (high level)
- ✅ Ghidra project: 3 binaries imported, disassembled (Android Thumb coverage recovered
  3780→221), demangled (Android 5054 / iOS 1252+209 symbols), Android types imported.
- ✅ Asset tooling: `.aei` textures (1354 files, →PNG), `.bin` tables (names/items, schema-
  validated), `.lang` strings (3385). FMOD via stock tools (see tools/README.md).
- ✅ Byte-match harness + first matched functions (`Array<T>`: ArrayAdd/ArrayRemoveAll 100%, ArraySetLength 95.8%).
- ✅ iOS function discovery (2407→7822); BSim pipeline (cross-compiler found low-yield — see ROADMAP).
- ✅ **Automation pipeline** (`make verify`/`coverage`; `tools/{gofdiff.py,Gof2ExtractCtx.java,coverage.py,verify.sh,permuter/}`):
  extract context → author → diff (MATCH %) → permute → register in `matches.tsv` → CI-gate.
- 🚧 The long road: authoring matched source across the engine + game at volume. `make coverage`
  tracks progress (currently 3 functions). Everything needed to scale it is in place.
