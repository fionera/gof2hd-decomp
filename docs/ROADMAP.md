# GOF2 decomp — status & roadmap

A full matching decomp of a commercial engine+game is a long, incremental effort (thousands of
functions). This tracks what's done and the path to full coverage. Detailed RE log: `DECOMP_NOTES.md`.

## ★ MILESTONE: whole game disassembled AND reassembled
- **Disassembly COMPLETE** — every function in the target (3,126 across 286 classes / the whole
  named universe) has written, compiling C++. `0` functions missing.
- **Reassembly WORKS** — `make reassemble` (`work/reassemble.sh`) compiles all + auto-stubs the
  1,983 not-yet-recovered data globals/vtables + links → `build/full/libgof2hdaa.so`
  (1.86 MB vs 2.14 MB target; 7,225 functions; valid ELF DYN shared object).
- **Byte-exact subset: 1,672+** functions reproduce the target instruction-for-instruction
  (`matches.tsv`, gated by the parallel `make verify`, 0 regressed). Driving the rest to byte-exact —
  plus giving the 1,983 stubs their real values — is the remaining **finetuning** phase (deferred).
- Toolchain pinned: NDK r18b clang 7.0.2, **`-Oz -fstack-protector`** (`-strong` is net-negative),
  gold-vs-lld noted for byte-identical relink. Build+gate parallelized across all cores.

## Done — infrastructure & tooling (the hard setup is complete)
- **Ghidra project**: 3 binaries imported, disassembled, demangled. Android Thumb coverage
  recovered (undisassembled funcs 3780→221; 7450 functions). Types imported + validated.
- **Asset tools** (`tools/`), all validated against real data:
  - `.aei` textures — 1354 files parsed, 0 errors; uncompressed→PNG. (GPU-compressed minority: TODO decode)
  - `.bin` tables — `names` + `items` schema-validated (byte-exact), `.lang` (3385 strings). Generic dumper for the rest.
  - FMOD `.fev/.fsb` — documented (stock tools).
- **Matching pipeline**: NDK r18b clang 7.0.2 running (Rosetta), `make`/diff harness, verified flags.
- **Matched code**: `src/engine/array.cpp` — `ArrayAdd`, `ArrayRemoveAll`, and `ArraySetLength`
  reproduce the target **byte-exactly** (`ArraySetLength` was the last `-O2` near-miss at 95.8%; it
  matches at the correct `-Oz`). One template covers the whole `Array<T*>` family.

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
3. ✅ **Containers** — `src/engine/array.cpp`: `ArrayAdd`+`ArrayRemoveAll`+`ArraySetLength` byte-exact
   (at `-Oz`), covering the whole `Array<T*>` family. `src/engine/string.cpp`:
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

## Coverage progression — 147 → **1104** byte-exact functions (1103 exact + 1 near, 0 regressed, 98 classes)
Two toolchain breakthroughs drove the scale-up: **(1) the target is `-Oz`** (size), not `-O2` — lifted the
false "ceiling"; **(2) the target uses `-fstack-protector`** (canaries on funcs with on-stack buffers) — adding
it (0 regressions) unlocked the canary-gated `Create*`/`Draw*` family. Core game layer now largely recovered:
PlayerEgo 112, KIPlayer 38, Player 62, Ship 66, Status 87, Level 51, Item, Mission, Agent, Station, Wanted,
Hud, TouchButton, Layout, ListItem, PaintCanvas 61, Transform, ApplicationManager, Engine, ~30 GL shaders, etc.
Repeatable pipeline per class: `build_pkg.py <Class>` (symbol-table ⋈ candidates, any namespace) → Ghidra batch
work-item extractor → codex or Claude subagents (`run_codex_class.sh` / Agent) → `collect_class.sh` → `make verify`.

### Earlier milestones
Driven by parallel waves of subagents (one per class), each building a header and matching its
methods via `try.sh`/`gofdiff`. All genuine C++, 0 asm-cheats, integrity intact, committed per wave:
- **Wave 0 — 8 Claude team-members → 376.** Status 81, Player 58, Ship 43, AEMath 40, AEFile 32,
  Level 36, Quaternion 15, Item 0 (+ carryover).
- **`-Oz` discovery → 392.** Switched the global flag (see below); 16 previously-divergent functions
  flipped to byte-exact.
- **Wave 1 — Claude authoring (existing classes' un-attempted methods) at `-Oz` → 428 (+`Release`).**
  Ship +20 (66 total), Level +9 (51), Status +4 (87), Player +2 (62).
- **Wave 2 — codex on 4 NEW classes at `-Oz` → 486.** ApplicationManager 42/63, Transform 11/24,
  AERandom 5/7, ConfigReader 0/5. Packages built by `work/build_pkg.py` (joins symbol table +
  `candidates.tsv`) + a Ghidra batch work-item extractor; codex workers via `work/run_codex_class.sh`
  (direct authoring, opaque-struct + byte-offset-cast field access).
- **Wave 3 — codex (Engine, AESoundRessource) → 533.** Engine 35/75, AESoundRessource 12/25.
- **Wave 4 — codex (10 shader classes) → 564.** ShaderBaseStruct 5/9, DrawFBOShader 5/6, PostBWShader
  4/7, TextureShader 4/7, GlowPPShader/BloomShader/TextureVtxColorShader 3 each, rest 1-2. Formulaic
  GL uniform setup; residuals are register-scheduling around `glUniform*`/`glGetUniformLocation`.

Per-class yield correlates with code shape: accessor/logic-heavy classes 50-90% first-pass at `-Oz`;
GL/scheduling- and parse-heavy classes 15-50% (the residuals are register-allocation/scheduling ties →
decomp-permuter, not more LLM re-tries). **44 classes now have byte-exact methods.**

New-class scaling recipe (repeatable): `build_pkg.py <Class>` → batch-extract work-items in Ghidra →
`run_codex_class.sh <Class>` → `collect_class.sh <Class>` → `make verify`. Tractable classes
(accessor/logic-heavy) yield well (ApplicationManager 67%); parsing/exception-tail classes need permuter.

### The "toolchain ceiling" was the WRONG OPT LEVEL — RESOLVED → 392 functions
Earlier waves concluded control-flow/FP-heavy functions "frequently cannot be byte-matched" with
clang 7.0.2 and blamed an inherent toolchain ceiling. **That was a misdiagnosis.** Toolchain
archaeology settled it:
- **Compiler identity is exact**: `.comment` + `.note.android.ident` ⇒ NDK **r18b** clang **7.0.2**
  build 5063045; linker **gold 1.12**; `.ARM.attributes` = v7 + VFPv3 + NEONv1 (the `-march=armv7-a`
  default — no explicit `-mfpu` needed). So the compiler/version/arch were never the problem.
- **The target is built `-Oz` (size), not `-O2`.** An opt-level sweep over the full regression set +
  133 divergent authored functions is decisive:

  | level | of 376 known: exact | divergent set: exact | divergent mean |
  |-------|---------------------|----------------------|----------------|
  | `-O2` | 375 (ArraySetLength 95.8%) | 0 | 53.3% |
  | `-Os` | **376** | 7 | 57.4% |
  | `-Oz` | **376** | **16** | **64.0%** |
  | `-O1` | 315 | — | — |

  `-Oz` keeps **every** known match exact AND flips **16** previously-"unmatchable" functions to
  byte-exact — including FP `AEMath::InvSqrt` (12/12) and 134-byte control-flow `Player::shoot`
  (39/39), the exact categories the "ceiling" claimed were impossible. 9 of the 16 match *only* at
  `-Oz`. The `movw/movt`-vs-literal-pool, predicated-epilogue and if-conversion "divergences" were
  just `-O2` vs size-opt codegen.
- The earlier `-fstack-protector-strong` regression was a red herring: the target uses plain
  `-fstack-protector` (canaries only on functions with arrays, e.g. `MatrixIdentity`), not `-strong`.

**Global flag switched `-O2 → -Oz`** (Makefile/try.sh/permuter/flags.sh), re-blessed, verified
**392 exact / 0 regressed** on a clean build. This lifts the supposed ceiling: FP and control-flow
functions are matchable at the right opt level.

### To scale further (now much higher-yield at `-Oz`)
Re-run waves at `-Oz` — functions prior waves abandoned as "unmatchable" are now reachable, and the
~117 still-divergent authored functions sit at a higher base % (mean 64%), so most need only small
source fixes or a decomp-permuter pass (now also `-Oz`). Keep prioritizing leaf/accessor functions
from `candidates.tsv`; expand to non-leaf as callees land. The hardened `make verify` gate
(clean build + integrity + asm-guard) keeps every batch honest.

## How to contribute a function (the loop)
Decompile in Ghidra (types applied) → write C++ in `src/<subsystem>/` → `make asm` →
`build/extract_target.sh <elf_vaddr> <n>` → diff → iterate source/flags to exact. Log matches here.
