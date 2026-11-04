# Build-flag research wave 104: 20 independent audits

Status: consolidated research note, 2026-11-04. Base `8c5fd3646`. Twenty
`codex exec -p dsv41` agents worked in isolated worktrees (`wt/w5201`..`wt/w5220`),
one build-flag question each. Every note below is self-contained and was committed
separately; this file only synthesizes them.

Canonical gate before and after the wave (note-only changes):

```text
byte_exact 4500/4524   linked_exact 4500/4524   size-exact 4524/4524
data_exact 843/843     imports 207              lint CLEAN   drift 0
RATCHET PASS
```

## Bottom line

The build recipe is no longer a guess. It is positively fingerprinted by the shipped
ELF and by controlled A/B rebuilds. The current `cmake/toolchain.cmake` +
`CMakeLists.txt` recipe is **correct for every observable compile/link flag**; no
tested alternative improves any gate count. The remaining 24 non-exact functions are
source-shape residuals, not a missing build flag.

The original was built with:

* **NDK r18b**, clang **7.0.2**, gold **1.12**, API **16**, armeabi-v7a,
  Thumb-2, NEONv1, softfp, static libc++.
* **CMake with the NDK r18b `android.toolchain.cmake` defaults**, not vanilla
  ndk-build. The two decisive binary discriminators are:
  * **no `-fdata-sections`** (ndk-build adds it; adding it gives byte 4311 and
    changes `.rodata`/`.data`), and
  * **FMOD before the libc++/libgcc archives** (CMake's `target_link_libraries`
    order; ndk-build's STL-before-user-libs order gives byte 1345).
* Effective compile flags: `-Oz -g -std=c++11 -fPIC -frtti -fexceptions
  -fstack-protector-strong -funwind-tables -ffunction-sections
  -DANDROID -DNDEBUG -stdlib=libc++`, default visibility, frame pointers kept,
  no `-fdata-sections`.
* Effective link flags: one plain gold link, `-shared -soname libgof2hdaa.so
  --build-id -z now --exclude-libs=libgcc.a`, zlib/libzip `--whole-archive`,
  no `-s`, followed by GNU `strip --strip-all` and the `.shstrtab` normalization.
* Packaging: Android Gradle Plugin **3.1.2**, release, 2018-10-22; the exact
  native build driver (`externalNativeBuild` vs standalone CMake) is not in the
  APK and is unobservable.

## What each audit established

| agent | topic | result |
|---|---|---|
| w5201 | independent ELF re-derivation | Confirms the whole compile-flag set from `.ARM.attributes`, canaries, RTTI/EH objects, exidx duplicates, `new[]` guards, PIC relocations and A/B. No improvement. |
| w5202 | build-system archaeology | CMake r18b defaults, not ndk-build; no `-fdata-sections`; FMOD-before-STL; macOS host; no surviving build files. |
| w5203 | gold link flags | `-z now`, `--build-id`, `--hash-style=sysv`, `--exclude-libs=libgcc.a`, whole-archive zlib/libzip and GNU strip are required; `-z norelro`, `--hash-style=gnu`, `--gc-sections`, `--icf`, `-s` are disproven. |
| w5204 | per-TU exceptions/RTTI | 960 per-TU cells; original uses exceptions/RTTI on all observable TUs; no per-TU exception/RTTI flag improves anything. |
| w5205 | per-TU ISA/ARM mode | All 192 app TUs are Thumb; the 23 ARM functions are the vendored libcrypto SHA subset and are already exact. No per-TU ISA flag improves. |
| w5206 | per-TU stack/unwind/frame pointer | Uniform `-fstack-protector-strong`, `-funwind-tables`, keep-frame-pointer; 0 canary/frame-pointer/exidx mismatches; no per-TU improvement. |
| w5207 | per-TU C++ semantics | 4,032 compiles; default visibility, thread-safe statics, `__cxa_atexit`, `-fcommon`, strict aliasing and constructor elision confirmed; no improvement. |
| w5208 | per-TU data/rodata | `-fdata-sections`, `-fwritable-strings`, `-fno-zero-initialized-in-bss` are worse; `-fno-common` fails on the duplicate `g_android_back_button_pressed`; no improvement. |
| w5209 | FP/math/vectorization | 22 global flags; 10 are exact no-ops, 12 worse; no target improves. |
| w5210 | `-mllvm` backend screen | 823 hidden options, 231 code-changing flags, 189 full-link variants; no variant beats 4500 or creates a new exact function. |
| w5211 | debug info/build-id | `-g`/`-g2` is load-bearing; `-g0` drops to 2069. Build-id is SHA-1 of the unstripped gold output with the descriptor zeroed; reproducing it needs the original debug info/symbol tables/paths. |
| w5212 | vendored libraries | zlib 1.2.3 / libzip 0.9.3 / OpenSSL 1.1.0-line; vendored `.text` is already byte-identical; exact flags confirmed; no improvement. |
| w5213 | cross-artifact evidence | iOS/Windows artifacts confirm ABI/source/TU facts but not Android flags; no build scripts or source leak. |
| w5214 | compiler-generated patterns | All canaries, frame pointers, EH personalities, RTTI, static init, PLT/GOT and `__aeabi_*` patterns match; flags confirmed. |
| w5215 | optimization level | `-Oz` uniquely matches; `-Os` 1263, `-O2` 1168, `-O3` 1167, `-O1` 1033, `-O0` 61; no per-TU alternative improves. |
| w5216 | visibility/export | All 5,370 defined exports are DEFAULT; hidden/protected/inlines-hidden are disproven; `.dynsym`/`.dynstr`/`.hash` are byte-identical. |
| w5217 | PIC/PLT/GOT/relocations | `-fpic` vs `-fPIC` unobservable; `-fPIE`, `-fno-pic`, `-mlong-calls`, `-fno-jump-tables` disproven; no improvement. |
| w5218 | stack protector | `-fstack-protector-strong` positively identified (678 canary functions / 787 call sites); basic/all/none are decisively wrong. |
| w5219 | exceptions/RTTI/unwind | Exceptions, RTTI and `-funwind-tables` are on; `-fno-exceptions` drops to 1344, `-fno-rtti` to 1701, `-fno-unwind-tables` to 2878. |
| w5220 | metadata/macros | `-DANDROID`, `-DNDEBUG`, `-fident`, `-gcc-toolchain`, `-Wa,--noexecstack` etc. are codegen-neutral; `_FORTIFY_SOURCE=2` at API 16 is inert; no improvement. |

## Current residual (measured at this commit)

Same-size section content differences vs the original:

| section | size | differing bytes | note |
|---|---:|---:|---|
| `.text` | 1,259,464 | **100,113** | 24 non-exact function bodies |
| `.ARM.exidx` | 28,528 | **829** | 812 word-1 (EHABI/LSDA offset) words; 0 prel31 words |
| `.ARM.extab` | 156,312 | **108,295** | size/base exact, LSDA content shifted |
| `.rel.plt` | 29,392 | **0** | byte-identical |
| `.note.gnu.build-id` | 36 | **20** | SHA-1 of the original unstripped link output |

All other sections, including `.rodata`, `.data`, `.data.rel.ro`, `.bss`, `.got`,
`.plt`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.dynamic`, `.init_array`,
`.fini_array`, `.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
`.ARM.attributes` and `.shstrtab`, are byte-identical.

## What still prevents 100%

1. **24 source-shape functions** (the whole `.text` delta): `Layout::Layout` C1/C2,
   `LevelScript::process`, `Status::missionCompleted`, `Generator::createMission`,
   `Level::createShip`, `Radar::draw`, `MGame::OnTouchEnd`, `HangarWindow::render`,
   `Globals::Globals` C1/C2, `RocketGun::update`, `PlayerEgo::calcCollision`,
   `Level::createScene`, `Gun::calcCharacterCollision`, `Level::createMission`,
   `Hud::draw`, `MenuTouchWindow` C1/C2, `SpaceLounge::drawLounge`,
   `MenuTouchWindow::OnTouchBegin`, `ModStation::ModStation` C1/C2 and
   `MGame::OnUpdate`. All are size-exact; the deltas are register allocation,
   instruction scheduling, block order and EH scope grouping.
2. **EH LSDA grouping** follows those five bodies (`Generator::createMission`,
   `Level::createShip`, `LevelScript::process`, `HangarWindow::render`, `Hud::draw`).
   `.ARM.exidx` addresses are exact; only the word-1 LSDA offsets differ.
3. **Build-id** requires a byte-identical *unstripped* gold output: exact debug info,
   symbol tables, section headers and original source/build paths. The original
   unstripped `.so` is not shipped, so this converges last even after `.text` is exact.

Flags are not the blocker. The next productive work is worst-first source-shape
reconstruction, not more flag search.

## Per-agent notes

`_work/reconstructions/independent_compile_flag_rederivation_w5201.md`,
`build_system_archaeology_w5202.md`, `linker_flag_audit_w5203.md`,
`per_tu_exceptions_rtti_w5204.md`, `per_tu_isa_arm_mode_w5205.md`,
`per_tu_stack_unwind_w5206.md`, `per_tu_cxx_semantics_w5207.md`,
`per_tu_data_rodata_w5208.md`, `fp_math_vectorization_w5209.md`,
`mllvm_backend_screen_w5210.md`, `debug_info_build_id_w5211.md`,
`vendored_library_flags_w5212.md`, `cross_artifact_build_evidence_w5213.md`,
`compiler_generated_pattern_audit_w5214.md`, `optimization_level_per_tu_w5215.md`,
`visibility_symbol_export_w5216.md`, `pic_plt_got_relocations_w5217.md`,
`stack_protector_fingerprint_w5218.md`, `exceptions_rtti_fingerprint_w5219.md`,
`build_metadata_macros_w5220.md`.
