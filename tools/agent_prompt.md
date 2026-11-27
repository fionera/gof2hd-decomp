# GOF2HD Decompilation Task

Target TU: `{{TARGET_TU}}`
Branch: `{{BRANCH_NAME}}`

## Target Symbol(s)

{{TARGET_SYMBOLS_TABLE}}

## Workflow

1. Inspect the current binary diff for the target symbol(s):
   ```bash
   {{DIFF_CMD}}
   ```
2. Read the relevant `.cpp` / `.h` files under `src/` and any reference notes in `_work/original_layout/`.
3. Edit the C++ source to match the original ARM/Thumb machine code (NDK r18b clang 7.0.2, `-Oz`).
4. Rebuild and verify:
   ```bash
   {{BUILD_CMD}}
   {{CHECK_CMD}}
   ```
5. If you improve the target with zero regressions (`validate.py check` passes), commit your changes cleanly on `{{BRANCH_NAME}}` with the before/after byte-diff counts in the commit message. If an approach does not improve the diff or causes any regression, revert it.

## Strict Authenticity Rules

- Target **authentic original C++ source code** — clean, idiomatic code that naturally compiles to the original instructions.
- **FORBIDDEN**:
  - Inline assembly (`asm`, `__asm`, `__asm__`)
  - Manual jump tables / computed gotos (`&&label`, `goto *`)
  - Compiler attributes or linker hacks (`__attribute__`, `__declspec`, custom linker scripts)
  - Gratuitous `extern "C"` or weird `extern` workarounds
  - `void*` pointer-arithmetic casts or `union` hacks
  - Adding new per-file `-mllvm` / `-target-feature` flags in `CMakeLists.txt`
- **NEVER** modify `tools/baseline.json` to hide a regression.
- Only edit files belonging to this task (`{{TARGET_TU}}` and its directly related headers).
