# AGENTS.md — working on the GoF2 decomp

Guidelines for any agent or human making changes in this repo. Read this before touching `src/`.
The project is a **1:1 byte-matching decompilation** of the Android `libgof2hdaa.so` (Galaxy on Fire 2 HD
2.0.16, armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`) targeting authentic original C++ source code.
Correctness is defined by the binary, not by "looks right".

## Ground truth, in order of authority

1. **The original binary** — `android_2.0.16_libgof2hdaa.so` (configured via `GOF2_ORIGINAL_SO` or
   `../_work/bins/android_2.0.16_libgof2hdaa.so`). Use Ghidra or ARM/Thumb disassembly for exact
   struct layouts and behavior.
2. **Original source-layout dumps** — `_work/original_layout/` (`tu_mapping.md`,
   `original_layout_notes.md`, `original_layout.json`). Authority on which functions and statics
   belong in which TU.
3. **DeepOpen** (`../DeepOpen/`) — J2ME v1.0.4 Java logic reference only; always re-verify
   constants and thresholds against the v2 Android binary.

## Build & Validation CLI

```bash
cmake --preset default                                     # configure once (fetches NDK r18b)
cmake --build cmake-build-match --target link_so           # compile + gold-link libgof2hdaa.so

# Unified validation CLI (stdlib-only Python 3, auto-discovers CMakeCache.txt / env vars):
python3 tools/validate.py status                           # fast byte-first comparison + ranked diff table
python3 tools/validate.py diff <mangled_or_substr>         # side-by-side disasm + differing byte hunks
python3 tools/validate.py check                            # full regression gate vs tools/baseline.json
python3 tools/validate.py check --update-baseline          # atomically ratchet tools/baseline.json upward
python3 tools/validate.py layout [ClassName ...]           # NDK clang++ AAPCS field_0xNN offset check
python3 tools/validate.py lint                             # source-authenticity linter + debt audit
```

CMake convenience targets wrap `tools/validate.py`: `verify`, `verify-fn` (`FN=<sym>`), `lint`,
`drift`, and `check`. Reports default to `<build-dir>/report.json` (never the repo root).

Set `GOF2_ORIGINAL_SO` and `GOF2_FMOD_LIBDIR` via environment variables or `-DGOF2_ORIGINAL_SO=...`
when configuring a build directory outside the default relative path.

## Parallel Codex Harness

```bash
python3 tools/harness.py --workers 4 --max-tasks 16
python3 tools/harness.py --targets _ZN... --workers 2
python3 tools/harness.py --dry-run --workers 2 --max-tasks 4
```

`tools/harness.py` groups non-exact symbols by TU (preventing file collisions), creates an isolated
`git worktree` per worker under `.harness/wt/` on branch `decomp/<run-id>-w<idx>-<slug>`, symlinks
`.cache` when present, renders `tools/agent_prompt.md`, runs `codex exec`, and gates each worker
with `tools/validate.py check`. Improved/matched branches are kept; unimproved worktrees are cleaned
up automatically.

## Regression Gate (`tools/validate.py check`)

`tools/validate.py check` (also wired into `tools/hooks/pre-push` via
`git config core.hooksPath tools/hooks`) checks:

- **Freshness**: built `libgof2hdaa.so` must be newer than `src/`, `third_party/`, `CMakeLists.txt`,
  and `tools/source_order.txt`.
- **No regressions** against `tools/baseline.json`:
  - `byte_exact` (4,508/4,524) and `linked_exact` (4,508/4,524) must never decrease.
  - `data.exact` (843/843) must never decrease.
  - `stub_zero_size`, `missing`, `wrong_type`, `extra` must stay at 0 (no undocumented extra exports).
  - Undefined imports must remain identical to the pinned 207-symbol set.
- **Lint**: 0 hard source-authenticity violations.
- **Layout**: 0 drifted `field_0xNN` offsets under NDK ARM AAPCS (when NDK `clang++` is available).

Never run `--update-baseline` to absorb a regression.

## Source Authenticity Rules (Hard)

- Target authentic original C++ source code.
- **Forbidden**:
  - Inline assembly (`asm`, `__asm`, `__asm__`, `asm volatile`).
  - Manual jump tables / computed gotos (`&&label`, `goto *`).
  - Compiler attributes or linker hacks (`__attribute__`, `__declspec`, custom linker scripts,
    `--version-script`, `--section-ordering-file`, `--allow-multiple-definition`).
  - `union` hacks or `void*` pointer-arithmetic casts.
  - Gratuitous `extern "C"` (only allowed with same-line `// lint: extern_c` at genuine native ABI
    boundaries: JNI `Java_*`, `ndk*`, GL/libc C ABI).
  - Explicit template instantiations without a same-line `// lint: explicit_inst` waiver.
  - New per-file `-mllvm` / `-target-feature` overrides in `CMakeLists.txt`.

## Correctness & TU Organization Pitfalls

- **Never leave a non-void function without a `return`** (UB under `-Oz` collapses the symbol to size 0).
- **Never call through an unassigned file-static function pointer** (same UB body deletion).
- **Namespace-scope globals become exported `.dynsym` symbols** — keep module state `static` or in an
  anonymous namespace unless cross-TU linkage is required by the original.
- **TU membership matters**: `.rodata`/`.data` layout, `.init_array` entries, and `-Oz` same-TU
  inlining follow TU ownership. Keep `tools/source_order.txt` in original TU link order and consult
  `_work/original_layout/tu_mapping.md` before adding or moving code.
