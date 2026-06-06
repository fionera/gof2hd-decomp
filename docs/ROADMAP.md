# GOF2 decomp — status & roadmap

A full matching decomp of a commercial engine+game is a long, incremental effort (thousands of
functions). This tracks what's done and the path to full coverage. Detailed RE log: `DECOMP_NOTES.md`.

## Done — infrastructure & tooling (the hard setup is complete)
- **Ghidra project**: 3 binaries imported, disassembled, demangled. Android Thumb coverage
  recovered (undisassembled funcs 3780→221; 7450 functions). Types imported + validated.
- **Asset tools** (`tools/`), all validated against real data:
  - `.aei` textures — 1354 files parsed, 0 errors; uncompressed→PNG. (GPU-compressed minority: TODO decode)
  - `.bin` tables — `names` + `items` schema-validated (byte-exact), `.lang` (3385 strings). Generic dumper for the rest.
  - FMOD `.fev/.fsb` — documented (stock tools).
- **Matching pipeline**: NDK r18b clang 7.0.2 running (Rosetta), `make`/diff harness, verified flags.
- **Matched code**: `src/engine/array.cpp` — `ArrayAdd` and `ArrayRemoveAll` reproduce the target
  **byte-exactly**; one template covers the whole `Array<T*>` family. `ArraySetLength` ~90% (1 redundant load to resolve).

## Progress on the 6 fronts
1. ✅ **iOS function discovery** — Thumb-prologue sweep of `__text`: **2407 → 7822 functions**
   (now on par with Android's 7450). The unencrypted iOS binary is fully usable.
2. ✅ **BSim** — pipeline validated (self-query 150/150 @ 1.000; reusable `tools/Gof2BSim.java`).
   **Empirical finding (probe: 2500 Android funcs ingested, 410 iOS named queried):** cross-compiler
   matching (Android clang ↔ iOS llvm-gcc) is **low-yield** — of 17 same-name true positives, most
   scored 0.3–0.6 (overlapping false positives); only ~1 reached 1.0. So **don't bulk-name iOS from
   Android via BSim** (it would mostly apply wrong names). BSim stays useful for *same-compiler* and
   *within-binary* family matching, not cross-compiler. Naming is effectively maxed from the symbol
   tables (demangling, done) — bulk naming is NOT the scaling accelerator; per-function authoring is.
3. ✅ **Containers** — `src/engine/array.cpp`: `ArrayAdd`+`ArrayRemoveAll` byte-exact, `ArraySetLength`
   ~exact (1 scheduling swap), covering the whole `Array<T*>` family. `src/engine/string.cpp`:
   `String` layout + `GetStringLength`.
4. ✅ **`.bin` schemas** — `names`, `items`(233), `systems`(34), `.lang` all byte-validated in
   `tools/gofbin.py`; method documented for ships/agents/weapons/stations/etc.
5. 🚧 **Engine subsystems** — seeded (array/string). The long per-function grind: file I/O (`AEFile`,
   `AEPakFile`), math (`AEMath`), rendering (`PaintCanvas`, `Image2D`, shaders), sound, then the game
   layer (`PlayerEgo`, `Ship`, `Level`, `Mission`, `KIPlayer`, `NFC`). Pipeline + BSim ready to scale it.
6. ✅ **Relink groundwork** — `make link` produces an ARM `libgof2hdaa.so` (soname/NEEDED/build-id via
   lld); requirements for byte-identical relink documented in `RELINK.md`.

The remaining work is **#5 at volume** — authoring and matching the thousands of engine+game functions.
Everything needed to do that efficiently (discovery, naming, types, tools, build+diff+link) is now in place.

## Parallel worker pipeline (codex fan-out) — proven
Work-list → context-extract → **N parallel `codex exec` workers** (`work/run_worker.sh`, `run_all.sh`)
→ each authors `work/src/<fn>.cpp` and self-verifies via `tools/try.sh` (`gofdiff`) → collect
(`work/collect.sh`) → integrate to `src/recovered/` + `matches.tsv` → `make verify`.

First batch result: 10 leaf functions, 10 workers in parallel (~minutes wall-clock) →
**9 genuine byte-exact matches first-try** (`GetWidth/GetHeight/OpenRead/OpenWrite/SetTexture/
SetWorldViewMatrix/SetApplicationManager/DisableClip/TransformGetTransform`). Coverage 3 → 12.

Lessons baked into the pipeline:
- **Workers can cheat** — one used `__attribute__((naked))` + inline `asm` to force the byte match.
  Added an **asm/naked guard** to `make verify` (rejects non-genuine source). The cheat was excluded
  and flagged for a real redo (`work/src/SetAnimationState.cpp.asm-cheat-REDO`).
- Leaf/tail-call functions match ~first-try; the differ normalizing call/branch *targets* makes
  tail-call wrappers (`return ext(field)`) trivial to match.
- candidates: `work/candidates.tsv` (3666 named funcs ranked by size/leaf-ness) feeds the next batches.

## Self-run wave (8 Claude team-members, no codex) — 147 → 376 functions
Codex hit its usage limit, so a wave was run with 8 parallel **Claude subagents** (one per class),
each building a coherent layout-`static_assert`'d header and matching its methods via `try.sh`/`gofdiff`.
Yield (all genuine C++, 0 cheats, integrity intact, committed per class):
Status 81, Player 58, Ship 43, AEMath 40, AEFile 32, Level 36, Quaternion 15, Item 0 (+ carryover) → **376 total, 7.4% of named.**
(Nested subagents-per-method weren't available, so each class-agent did its methods directly.)

### Toolchain ceiling (confirmed independently by every agent)
Simple accessors / tail-call wrappers match ~first-try (≈75–80% of attempts). **Control-flow- and
FP-heavy functions frequently cannot be byte-matched with NDK r18b clang 7.0.2 `-O2`**, even though
`.comment` says clang 7.0.2. Systematic, non-source-fixable divergences observed:
- if-conversion (`cbz`/`bx` vs predicated `it`/`bxne`); loop rotation/guard-merging; predicated-epilogue
  SP-fold; FP load-order & vmls operand canonicalization; `movw/movt` vs literal-pool constants.
- The target is **size-optimized** — per-function `__attribute__((minsize))` reproduces much of it.
- `-fstack-protector-strong` was tested and **regressed** a canary method (89.7%→66.7%) — NOT the answer.
Implication: a meaningful fraction of functions may need the *exact* original clang build/flags (or are
inherently un-bytematchable here). Accessors/data/tail-calls are reliably matchable; the hard tail needs
toolchain archaeology. This is the real ceiling on automated byte-match coverage.

### To scale further
Run bigger tranches through `run_all.sh` (more workers), prioritizing leaf functions from
`candidates.tsv`; route near-misses to decomp-permuter; expand to non-leaf functions as their
callees land. The `make verify` gate (with asm-guard) keeps every batch honest.

## How to contribute a function (the loop)
Decompile in Ghidra (types applied) → write C++ in `src/<subsystem>/` → `make asm` →
`build/extract_target.sh <elf_vaddr> <n>` → diff → iterate source/flags to exact. Log matches here.
