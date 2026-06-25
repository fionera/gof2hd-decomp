# ASM Validation: original `.so` ↔ our build

Continuously check whether our recovered C++ still assembles to the **same
instructions** as the original Android binary, function by function, and tune
compiler flags toward byte-exactness — without disturbing normal macOS
development.

## TL;DR

```bash
# configure the matching build and run the report (first configure downloads NDK r18b)
cmake --preset default
cmake --build cmake-build-match --target verify

# inspect one function (side-by-side disassembly diff)
cmake --build cmake-build-match --target verify-fn -- FN=_Z8ArrayAddIP14AEPakFileEntryEvT_R5ArrayIS2_E
```

CMake downloads NDK r18b itself (`cmake/DownloadNDK.cmake`) and runs its clang +
GNU binutils **natively** — on Apple Silicon the `darwin-x86_64` toolchain runs
under Rosetta 2 (`softwareupdate --install-rosetta`). No OrbStack needed.

## Why this exists / how it works

The original `libgof2hdaa.so` is ARMv7-A Thumb-2 + VFP/NEON, built with **NDK
r18b clang 7.0.2** for `armeabi-v7a`. We validate against it like this:

1. **Compile** each `src/*.cpp` with the *matching* toolchain. CMake compiles the
   `gof2` OBJECT library directly with the downloaded NDK clang (run natively under
   Rosetta), so Ninja tracks the source→object mapping and a deleted source can
   never leave a stale orphan in the link. The per-TU build is **resilient** — a TU
   that doesn't compile yet is skipped and reported, never aborting the run.
2. **Delink** the matching originals out of the stripped, fully-linked `.so` into a
   relocatable ARM `.o` carrying the real mangled symbol names (`delink.py`).
3. **Diff**: disassemble both sides with the NDK's `arm-linux-androideabi-objdump`, truncate to
   each symbol's real size, normalize away post-link absolute addresses, and
   compute a per-function fuzzy match plus a byte-exact flag (`asmdiff.py`).
4. **Auto-discovery**: any mangled symbol present in *both* our object and the
   `.so` symbol table is compared — coverage grows automatically as more TUs
   compile. No function list to maintain.

> **Why not [objdiff](https://github.com/encounter/objdiff)?** Its 32-bit ARM
> disassembler caps at ARMv6K (GBA/DS/3DS). This game is ARMv7-A Thumb-2 with
> VFP/NEON, which objdiff rejects outright, so we disassemble with GNU objdump
> (full ARMv7 support) and do our own normalize + match.

## Reading the report

```
 match  bytes  unit                               symbol
  100.0%   ==  engine/math/AEMath                 _ZN11AbyssEngine6AEMath...mlERKNS0_6VectorES3_
   67.5%       engine/math/AEMath                 _ZN11AbyssEngine6AEMath6MatrixmLERKS1_
...
N comparisons   avg 67.5%   100%-fuzzy 30   byte-exact 21
coverage: compared 2311/4524 original functions   missing 2213 (-> cmake-build-match/verify/missing.txt)
report -> cmake-build-match/verify/report.json
```

- **`L` (linked-exact)** is the real matching signal: the machine code is byte-identical to
  the original **after linking** — i.e. identical except inside relocation-covered fields (call
  targets, GOT/literal-pool addresses), which our unlinked `.o` leaves as placeholders while the
  delinked target carries the linker-resolved value. This is what to chase. (`asmdiff.linked_equal`
  masks the relocation byte-ranges, read from our object's `-r` table, before comparing.)
- **`==` (byte-exact)** is the rarer strict subset: raw bytes identical with *no* relocations at
  all — only leaf functions that never call out or load a global can reach it. A function can be a
  perfect match (`L`) yet not `==` purely because it makes a call; don't chase `==`, chase `L`.
- **`match`** is a fuzzy instruction-level score (0–100). Useful for "how close" and
  for comparing flag settings. It can read below 100% even for a byte-exact function
  when the original contains a literal pool (clang marks pools as data on our side;
  the delinked original has no such marker so objdump decodes the pool as code) — so
  trust `bytes ==` for "matched", `match` for "getting warmer".
- **`coverage` / `missing`** is the progress metric: how many of the original's
  `.text` functions we compared at all. *Missing* = original functions with no
  counterpart in our build — either not decompiled/compiled yet, or whose signature
  mangles differently (the `Array`-vs-`std::vector` gap accounts for a few hundred).
  The full list is written to `cmake-build-match/verify/missing.txt`.
- **`wrong type` (`missing_wrong_type`)** splits that *missing* pile into the two cases that
  look identical to an exact-name match but need very different fixes. We demangle every missing
  original and every symbol our build defines, strip the parameter list to a *qualified name*
  (`AEFile::Open`), and group by it. A missing original whose qualified name we **do** define is
  reported as *implemented under a different signature* — the body exists, only a parameter or
  qualifier type is off (`String` by-value vs `String&`, a global enum that should be nested,
  `void*` vs `char const*`), so the mangled name differs and the exact-name match skipped it.
  These are the cheap wins: retype our params to match the original and the function starts
  comparing (and usually matching). The rest are *genuinely absent* (never written). Both sides'
  full signatures, grouped by name, are written to
  `cmake-build-match/verify/missing_wrong_type.txt`; `report.json` carries the counts
  (`missing_wrong_type`, `missing_absent`) and the structured `wrong_type` list. Demangling uses
  `c++filt -n` (Itanium-aware on macOS — no OrbStack needed for this step).
- `report.json` has the full per-function data plus the coverage counts
  (`count`, `compared_unique`, `original_functions`, `missing`, `missing_wrong_type`,
  `missing_absent`) and the structured `wrong_type` pairings for scripting/CI.

## Tuning compiler flags

The canonical flag set lives in `cmake/toolchain.cmake` (the *one* source of truth —
it sets `CMAKE_CXX_FLAGS` for the single `-fPIC` `gof2` object library used by both
verify and relink). The biggest knob is the optimization level:

```bash
# A/B test opt levels — re-run picks up changed flags and rebuilds:
cmake --preset default -DGOF2_MATCH_OPT=-O2
cmake --build cmake-build-match --target verify
```

`GOF2_MATCH_OPT` flows through to the build via the `default` preset. Other flags
(fpu, stack protector, API level, exceptions/rtti) live in `cmake/toolchain.cmake`;
edit there and re-run `verify`. Known facts already baked in: `-mfpu=neon` (the original
uses NEON), `-D__ANDROID_API__=21` (needed for libc++ `<cmath>` to compile),
`-mthumb`, `-frtti`, **`-fPIC`** (the original .so has no `R_ARM_REL32`/`TEXTREL` and accesses
globals GOT-indirect — that is `-fPIC` codegen, not `-fpic`).

> **`-Oz` vs `-O2` — settled: `-Oz`.** A clean A/B on equal coverage (2326 functions compared in
> both) gave `-Oz` 468 byte-exact / 66.1% avg vs `-O2` 444 / 54.9%; 24 functions match only at
> `-Oz` and none only at `-O2`. The original was built `-Oz`. (`toolchain.cmake` defaults to it.)

## Files

| Path | Role |
|------|------|
| `cmake/DownloadNDK.cmake` | downloads NDK r18b + resolves clang/binutils tool paths |
| `cmake/toolchain.cmake` | the project toolchain (auto-applied) + canonical compile flags (`GOF2_MATCH_OPT`) |
| `CMakeLists.txt` (`gof2`) | CMake OBJECT library — compiles every TU via the toolchain; emits the tracked `match_objects.txt` (no stale orphans) |
| `third_party/match_libs.cmake` + `build_match_lib.cmake` | build the vendored libcrypto/zlib/libzip subsets with the NDK clang |
| `CMakeLists.txt` (`relink` target) | link the `gof2` objects + vendored archives → `libgof2hdaa.so` via NDK gold |
| `tools/symdiff.py` | whole-`.so` dynamic-symbol parity diff (the `relink` gate) |
| `tools/verify/delink.py` | extract original functions from the `.so` → `verify/target/*.o` |
| `tools/verify/asmdiff.py` | objdump-based normalize + per-symbol match |
| `tools/verify/verify.py` | orchestrator: table + `report.json`; `--show` one function |

Inputs (read-only): `_work/bins/android_2.0.16_libgof2hdaa.so`,
`_work/symbols/android_2.0.16.symbols.tsv`, `_work/symbols/android_thumb_map.tsv`.

## Known coverage gaps

Auto-discovery matches functions by **exact mangled name**, so anything whose signature mangles
differently than the original is invisible (not wrong — just not compared):

- **`Array<T>` — faithful hand-written container.** `src/engine/core/Array.h` is a single
  hand-written `Array<T>` template (layout `{size@0,data@4,capacity@8}`, realloc-based growth —
  bodies transcribed from the Android binary). It mangles as `5ArrayI...` like the original (so the
  container functions compare) **and** matches the original's element-access / iteration codegen.
- TUs that don't compile under the ARM toolchain yet are skipped (see the build summary); their
  functions simply aren't compared until they build. (Currently only `MGame.cpp` — a pre-existing
  `expected statement` parse error, unrelated to containers.)

## Direct CLI (without CMake)

```bash
python3 tools/verify/verify.py                 # build + diff everything
python3 tools/verify/verify.py --no-build      # reuse existing base objects
python3 tools/verify/verify.py --only AEMath   # filter by symbol regex
python3 tools/verify/verify.py --show <mangled-symbol>
```

Both the build and the diff fan out across `GOF2_VERIFY_JOBS` workers (default 8). The per-object
delink/objdump calls have a 90s timeout (`asmdiff.DISASM_TIMEOUT`): if a call wedges, that one unit
is skipped with a warning instead of hanging the whole run. (Run via the CMake `verify` target so
the `GOF2_NDK_*` tool paths are exported; a bare `python3 tools/verify/verify.py` needs them in the
environment.)
