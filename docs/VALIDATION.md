# Validation

How the decompiled `libgof2hdaa.so` is validated against the original Android 2.0.16 release
(`android_2.0.16_libgof2hdaa.so`, ELF 32-bit ARM, `armeabi-v7a` softfp, stripped).

## Build & Link Pipeline

The build uses **CMake + NDK r18b** (`cmake/toolchain.cmake`):
- Every TU in `src/` is compiled with NDK r18b clang 7.0.2 using the original flags:
  `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -ffunction-sections -funwind-tables -Oz -g -DANDROID -DNDEBUG -stdlib=libc++`.
- Vendored static libraries (`third_party/match_libs.cmake`) reproduce the original statically linked
  archives (OpenSSL 1.1.0l libcrypto subset built with GCC 4.9 / NDK r13b, zlib 1.2.3, libzip 0.9.3).
- `link_so` performs a single plain **gold** link (`-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`)
  in `tools/source_order.txt` input order, followed by GNU `arm-linux-androideabi-strip --strip-all`
  and `tools/reorder_shstrtab.py` (which restores gold's Stringpool `.shstrtab` order).
- No `--version-script`, no `--section-ordering-file`, and no `--allow-multiple-definition`.

## Validation CLI (`tools/validate.py`)

`tools/validate.py` is a self-contained Python 3 (stdlib-only) CLI with zero hardcoded machine paths.
It resolves `GOF2_ORIGINAL_SO`, `GOF2_FMOD_LIBDIR`, build dir, and NDK tools from CLI flags,
environment variables, `CMakeCache.txt`, or `.cache/ndk`.

| Subcommand | CMake Target | Purpose |
|---|---|---|
| `validate.py status` | `verify` | Pure-Python ELF32 comparison of `.dynsym` functions, data objects, imports, and raw ELF sections. Compares raw bytes in Python first (never invokes `objdump` for byte-exact functions). Writes JSON report to `<build-dir>/report.json`. |
| `validate.py diff <sym...>` | `verify-fn` | Side-by-side disassembly (via NDK `objdump` / `llvm-objdump`, or hex/word fallback) and differing byte hunks for one or more symbols (exact mangled name or substring). |
| `validate.py check` | `check` | Full regression gate against `tools/baseline.json`: build freshness (`src/`, `third_party/`, `CMakeLists.txt`, `tools/source_order.txt`), function/data/import parity, section parity report, source `lint`, and NDK `layout` check. Supports `--update-baseline` for atomic upward-only ratcheting. |
| `validate.py layout [Class...]` | `drift` | Verifies every `field_0xNN` offset matches ARM AAPCS layout using NDK `clang++ -fdump-record-layouts` (refuses host compiler). |
| `validate.py lint` | `lint` | Enforces source-authenticity hard rules (no inline asm, no computed gotos / manual jump tables, no `__attribute__`/`__declspec`, no `union`, no `void*` pointer-arithmetic casts, no unwaived `extern "C"` or explicit template instantiations) and reports a non-blocking technical-debt audit (`--strict` to fail on debt). |

Install the pre-push gate with:
```bash
git config core.hooksPath tools/hooks
```

## Parallel Codex Runner (`tools/harness.py`)

Run parallel Codex agents in isolated `git worktree` checkouts (one TU per worker, no file
collisions):

```bash
python3 tools/harness.py --workers 4 --max-tasks 16
python3 tools/harness.py --targets _ZN... --workers 2
python3 tools/harness.py --dry-run --workers 2 --max-tasks 4
```

Each worker runs on `decomp/<run-id>-w<idx>-<slug>` under `.harness/wt/`, streams logs to
`.harness/logs/<run-id>/`, and is gated by `tools/validate.py check` on exit. Branches that match
or improve the target with zero regressions are kept; unimproved/failed worktrees and temporary
branches are cleaned up automatically.

## Baseline & Current Parity (`tools/baseline.json`)

- **Functions**: `4,508 / 4,524` byte-exact, `4,508 / 4,524` linked-exact, `0` stubs, `0` missing,
  `0` wrong-type, `0` extra.
- **Data objects**: `843 / 843` byte-exact.
- **Undefined imports**: `207` (identical to the original).
- **Sections**: all 27 original sections present; `.rodata`, `.data`, `.data.rel.ro`, `.dynsym`,
  `.dynstr`, `.hash`, `.rel.dyn`, `.dynamic`, and `.shstrtab` are byte-identical. Remaining byte
  diffs are confined to `.text` (16 functions), `.ARM.extab`, `.ARM.exidx`, `.rel.plt`, and
  `.note.gnu.build-id`.
