# Galaxy on Fire 2 — Decompilation Project Notes

Goal: a **matching decompilation** of the full *Abyss Engine* + *Galaxy on Fire 2* game
(C++), recompilable back to byte-identical binaries.

_Last updated: 2026-06-06_

---

## 1. Source inventory & triage

| Source | File | Format / Arch | Encrypted | Named funcs | Toolchain | Verdict |
|---|---|---|---|---|---|---|
| **iOS 1.1.4 HD** | `Original IPA/...HD_1.1.4_ios_4.3.ipa` → `GalaxyOnFire2 HD` | Mach-O **armv7** | **No** (`cryptid 0`) | 664 (653 C++ methods, 27 AE classes) | Xcode SDK 6.0, **llvm-gcc/clang + libstdc++** | **Co-primary** (unencrypted; iOS platform layer + statically-linked FMOD) |
| **Android 2.0.16 HD** | `Original APK/...2.0.16_APKPure.apk` → `lib/armeabi-v7a/libgof2hdaa.so` | ELF **ARMv7** | No | **4524** (4075 C++ methods, 133 AE classes) | **NDK r18, clang 7.0.2 (LLVM 7.0.2) / GCC 4.9** | **Primary** (richest symbols; reproducible toolchain) |
| iOS 1.0.5 | `Original IPA/...1.0.5_ios_3.0.ipa` → `GalaxyOnFire2` | Mach-O universal armv6+**armv7** | No | 162 (155 methods, 25 AE classes) | older Apple GCC | Secondary cross-ref (matches DeepOpen 1.0.1 era) |
| Windows Steam | `Original Windows/.../GOF2.exe` | PE32 **x86** | No | 0 (stripped) | MSVC (vc10) | **Reference only** — cut content/features; ModApi offsets validate struct layouts |
| KiritoJPK Android | `KiritoJPK_.../GOF2FHD_APK.7z` + OBB | (7z, not yet extracted) | — | — | — | Modded/repacked Android — assets/OBB reference only |
| DeepOpen | `DeepOpen/code/GoF2_jsr/` | **Java (J2ME)** | — | — | — | **Discard for code** (wrong language + flagged incorrect). Keep `extras/gof2-1.0.1-ios-symbols` as a naming reference |
| KaamoClubModApi | `KaamoClubModApi/` | C++/Rust mod loader (PC) | — | — | — | **Reference only** — targets GOF2.exe **v1.0.3**; hardcoded offsets & struct semantics (`Globals_appManager`, `Globals_status`, valkyrie flags) for cross-checking |

### Why these targets
- **Both iOS binaries are NOT FairPlay-encrypted** (`LC_ENCRYPTION_INFO cryptid 0`) → `__TEXT` is fully readable. This is what makes iOS usable at all.
- **Android `.so` has by far the richest symbol table**: 4524 exported functions (no `-fvisibility=hidden`), demangling to **133 `AbyssEngine` classes** + the full game class hierarchy. It is the best map of the engine+game structure.
- **The recovered C++ source is shared across all platforms** (Abyss Engine + game). We name/structure from the richest binary (Android), cross-validate against iOS, and pick a matching-build target per platform.

### Code architecture (from symbols)
- Engine namespace: **`AbyssEngine`** (~1001 methods) — `Engine`, `Mesh`, `Transform`, `Camera`, `Material`, `String`, `Array<T>`, `Image2D`, `SpriteSystem`, `AEMath::Matrix`, file I/O (`AEFile`, `AEPakFileEntry`, `AELowLevelFile`), sound (`AESoundInterface`), shaders…
- Game lives in top-level classes (Android-only symbols, 4392 unique): **`PlayerEgo`** (player ship, 168), `Status`, `Player`, `Ship`, `Level`/`LevelScript`, **`KIPlayer`** (enemy AI; "KI" = German for AI), `Item`, `Mission`, `Hud`, `Agent`, `Radio`/`RadioMessage`, weapons (`BombGun`, `MineGun`), `Achievements`, and the **`NFC::`** namespace (store / DLC, e.g. `NFC::iap_buy_dlc_valkyrie()`).
- iOS-only delta (538): mostly **FMOD** (392, statically linked on iOS; a separate `libfmodex.so` on Android) + template instantiations + a few file classes.

---

## 2. Ghidra project

- **Project:** `GalaxyOnFire2` at repo root (`/Users/fionera/Downloads/GalaxyOnFire2`), Ghidra 12.1.2.
- **Imported programs** (all auto-analyzed):
  - `/primary/ios_1.1.4_HD` — `ARM:LE:32:v8`, image base 0x1000, **2407 functions**, 3619 symbols.
  - `/reference/android_2.0.16_libgof2hdaa.so` — `ARM:LE:32:v8`.
  - `/reference/ios_1.0.5_armv7` — `ARM:LE:32:v8` (armv7 slice extracted via `lipo -thin`).
- GhidraMCP bridge connected over UDS (pid varies); HTTP plugin on `127.0.0.1:8089`.

### Extracted working files (`_work/`)
- `_work/bins/` — the raw binaries pulled from the IPA/APK/EXE for import.
- `_work/symbols/` — demangled symbol maps (the cross-platform naming source of truth):
  - `ios_1.1.4_HD.symbols.tsv`, `ios_1.0.5.symbols.tsv`, `android_2.0.16.symbols.tsv` — `addr \t mangled \t demangled`.
  - `unified_symbols.tsv` — **5057 unique** symbols keyed by normalized mangled name, with presence flags + per-binary addresses. Overlap: iOS-HD∩Android = 126; iOS-HD-only = 538; Android-only = 4392.

> Demangling note: Apple `c++filt` strips one leading `_` → use it as-is for Mach-O (`__Z…`) but pass `--no-strip-underscore` for ELF (`_Z…`).

---

## 3. Demangling — DONE

Ghidra's stock **Demangler GNU** analyzer no-ops on these binaries (iOS Mach-O symbols carry a
double leading underscore `__Z` that the bundled native demangler rejects; the analyzer also
failed to apply on the Android ELF `_Z`). Fixed with a script that normalizes the leading
underscore (`__Z`→`_Z`) and applies the Java `GnuDemangler` directly, creating **namespaces,
classes, and function signatures** while **preserving** the original mangled symbol.

Script: `_work/scripts/DemangleAbyssEngine.java` (copy to `~/ghidra_scripts`, run per program).
Applied & saved to all three programs:

| Program | Symbols demangled & applied |
|---|---|
| `android_2.0.16_libgof2hdaa.so` | 5054 |
| `ios_1.1.4_HD` | 1252 |
| `ios_1.0.5_armv7` | 209 |

Result: full class tree recovered — e.g. `AbyssEngine`, `AEMath`, `Array<AbyssEngine::Mesh*>`,
and (Android) the game classes `PlayerEgo`, `Ship`, `KIPlayer`, `Mission`, `NFC`, etc.

> Enablement note: needs `GHIDRA_MCP_ALLOW_SCRIPTS=1` **and** the `~/ghidra_scripts` bundle
> enabled once (Ghidra **Window → Bundle Manager** → enable the row), else scripts fail with a
> `GhidraPlaceholderBundle` cast error. Both are done in the current session.

---

## 4. Matching-decomp strategy (forward plan)

1. **Demangle** all three programs in Ghidra (script, see §3).
2. **Port names across binaries** by code similarity (BSim / function matching) so iOS-unnamed
   functions inherit Android's names and vice-versa.
3. **Recover types/structs** from vtables + the rich class symbols; cross-check struct layouts
   against KaamoClubModApi offsets and the Windows build.
4. **Pick the first byte-match target = Android** (NDK r18 clang 7.0.2 is downloadable/pinnable;
   ELF is simpler than Mach-O). Reproduce iOS (armv7) and Windows (MSVC) builds as later goals
   from the same shared C++ source.
5. Build a recompile harness that diffs produced object code against the target `.so`/binary.

---

## 5. Progress log (decomp phase)

### Thumb/ARM disassembly coverage (both ARM binaries were severely under-disassembled)
Ghidra created Function objects from the symbol tables but left most of them with **no
disassembled bytes** (it didn't apply the per-symbol Thumb bit). Fixed by extracting the
authoritative ARM/Thumb map from each binary and force-disassembling in the correct mode,
then recomputing function bodies from flow.

- **Android**: ARM/Thumb bit from ELF dynsym `st_value` bit0 (macOS `nm` strips it — use
  `greadelf -sW`). Map: `_work/symbols/android_thumb_map.tsv` (4501 Thumb, 23 ARM).
  Result: noInstrAtEntry **3780 → 221**; functions **5298 → ~7450**. Bodies fixed for 1861+.
- **iOS HD**: ARM/Thumb from Mach-O `n_desc & N_ARM_THUMB_DEF` (parsed in Python).
  Map: `_work/symbols/ios_hd_thumb_map.tsv` (663 Thumb code syms, only **1** ARM code sym —
  iOS is effectively all-Thumb). Disassembled 781 functions, fixed 859 bodies; verified no
  ARM functions were mis-decoded.
- Reusable script: `_work/scripts/FixAndroidThumb.java` (map-driven, applies to either).
- **Image-base note**: Android Ghidra addr = `(elf_vaddr & ~1) + 0x10000`; iOS Ghidra addr =
  Mach-O `n_value` as-is (no rebase).

### Cross-binary name porting — findings
Outcome: **low yield in current state, by design.** Both binaries are already fully demangled
from their own symbol tables, so the cross-port target is small:
- iOS HD: 966 discovered functions, **919 named / 47 unnamed**; the 47 are FMOD/stub helpers,
  only 1 references a usable string → string-anchor porting nets ≈1 function.
- The 126 functions shared by name between iOS HD and Android are already identically named
  (shared Itanium mangling) — no porting needed.
- The real iOS gap is **function discovery** (966 vs Android's ~7450), not naming: thousands
  of non-symboled Thumb functions in iOS aren't turned into functions yet.

Reliable signals & tooling built:
- Android string→function map works via the reference model (`_work/symbols/strref_android.tsv`,
  1878 pairs) because clang/ELF materializes data refs.
- iOS string refs are **decompiler-only** (ARM literal-pool loads aren't stored as references,
  even with the ARM Constant Reference Analyzer options on) — must extract via the decompiler.
- **Recommendation for exhaustive porting:** (1) run aggressive Thumb function discovery on iOS
  to close the discovery gap, then (2) use **BSim** (bundled: `Ghidra/Features/BSim`, H2 local
  DB) for cross-compiler (llvm-gcc↔clang) code matching — the proper tool for this. String
  anchors are a high-precision supplement, not a substitute.

### Type/struct recovery
- Imported the **32 reverse-engineered struct layouts** from KaamoClubModApi into the Android
  program's type manager (`_work/types/gof2_types.h`, preprocessed: `AEArray<T>` template
  flattened to a concrete `AEArray{size; void* data; size2}`, stdint typedefs added).
  Includes `AEString`, `AEArray`, `SingleItem`(72B), `ShipInfo`(116B), `SingleSystem`,
  `SingleAgent`, `Globals_status`(504B), `Globals_appManager`(96B), `PlayerEgo`, `MGame`(200B),
  `LevelScript`, `TargetFollowCamera`, etc.
- **Cross-check PASSED**: decompiling Android `ArrayAdd<AEPakFileEntry*>` shows accesses at
  off 0 (size, ++), off 4 (data, realloc'd), off 8 (size2) — an exact match to the RE'd
  `AEArray{size@0; data@4; size2@8}`. Confirms the Windows-derived layouts apply byte-for-byte
  to the 32-bit ARM builds (same source, same pointer size/alignment).
- The library is reusable for all three programs (identical 32-bit layout). ModApi global
  offsets (e.g. `GLOBALS_STATUS=0x60AD6C`) are **Windows-specific addresses** — the *layouts*
  transfer, the *addresses* must be re-found per platform.
- C++ class recovery is well-seeded: **186 named vtables + 364 typeinfo (RTTI)** symbols in
  Android → enables automated class-hierarchy + vftable struct synthesis as a follow-on.

### Recompile + byte-match harness — PoC matched a function byte-exactly
- Stood up the **exact original toolchain**: NDK r18b, clang 7.0.2 — `--version` prints the
  same string as the target `.comment`: `Android (4751641 based on r328903) clang version 7.0.2`.
- Runs the x86_64 toolchain on OrbStack's arm64 Ubuntu via Rosetta (needed amd64 multiarch:
  pinned `ports.ubuntu.com` to `arch=arm64`, added `archive.ubuntu.com` for `arch=amd64`,
  installed `libc6:amd64` etc. for `/lib64/ld-linux-x86-64.so.2`). Toolchain at
  `/opt/android-ndk-r18b/...prebuilt/linux-x86_64` in the guest.
- **Result: `ArrayAdd<T*>` recompiled to a 15/15 instruction-identical match** with the target
  (`_work/build/arrayadd_target.txt` vs `arrayadd_compiled.txt`; the only nominal diff is
  `bl realloc` vs the target's `blx` — a link-time veneer, not codegen).
- Matching flags: `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti`
  (keep the frame pointer). Two convergence iterations showed how source structure drives the
  match: (1) reuse the `(size+1)` temp → matches the arithmetic; (2) don't keep it live across
  the `realloc` call (target reloads from memory) → matches register allocation/prologue exactly.
  NB: `ArrayAdd` matches at `-O2` AND `-Oz` (opt-invariant), which is why early waves wrongly
  fixed `-O2`; see the opt-level finding below.

### Toolchain archaeology — the target is `-Oz`, not `-O2` (lifted the "ceiling")
- Compiler identity pinned exactly: `.comment` + `.note.android.ident` ⇒ NDK **r18b** clang
  **7.0.2** build 5063045. Linker **gold 1.12** (`.note.gnu.gold-version`) — *not* lld (relink-only
  concern, see `RELINK.md`). `.ARM.attributes`: CPU v7, **VFPv3 + NEONv1**, strict IEEE FP — and the
  bare `-march=armv7-a` default already emits exactly these, so **no `-mfpu`/`-mfloat-abi` needed**.
- **Opt level was the real divergence.** Sweep over the 376-fn regression set + 133 divergent
  authored functions: `-O2` left 0 of the divergent set exact (mean 53%); **`-Oz` keeps all 376
  exact and flips 16 divergent → byte-exact (mean 64%)**, incl. FP `InvSqrt` (12/12) and ctrl-flow
  `Player::shoot` (39/39). 9 match *only* at `-Oz`. `-Os` is intermediate (7 flips); `-O1`/`-O3`
  worse. ⇒ global flag switched to **`-Oz`**; coverage 376 → **392**.
- The `movw/movt`-vs-literal-pool / predicated-epilogue / if-conversion "non-source-fixable
  divergences" prior agents catalogued were simply `-O2` vs size-opt codegen. Stack canaries on
  some functions (e.g. `MatrixIdentity`) ⇒ plain `-fstack-protector` (NOT `-strong`, which
  over-guards and regressed a test).
- Reproduce: `build/probe/sweep.py` (full set) and `build/probe/divsweep.py` (divergent set).
- Harness: `_work/build/{flags.sh,build_fn.sh,extract_target.sh,README.md,sample_arrayadd.cpp}`.
- This proves the matching-decompilation approach is viable: recovered C++ + the exact toolchain
  reproduce the target's machine code. The remaining work is volume — author source per function
  and iterate flags/source against the diff, then solve whole-`.so` relink (link order, version
  script, build-id).

## External reference repos (added to project dir; cross-validation, not game source)
- **gof2-vita/** — Android `.so` loader (FalsoJNI/AFakeNative) for the original ARMv6 non-HD binary.
  `loader/patch/*.c` resolves real game symbols by mangled name and confirms signatures + field
  names — notably the full `Globals::` touch-button int fields (`fire_x/y`, `boost_x/y`,
  `autopilot_x/y`, `turret_view_x/y`, `pause_x/y`, `action_menu_x/y`, `fast_forward_x/y`,
  `sec_fire_x/y`) and `ApplicationManager::OnTouchBegin/OnTouchEnd/GetCurrentApplicationModule`.
  Useful to fix header field names/signatures for Phase-2 matching. Cg shaders are SHA1-named.
- **gof2edit/** (Rust) — authoritative RE of the `.bin` tables + save format (`src/data/`,
  `src/data/save/`, `src/bin_io/`). Supersedes `tools/gofbin.py`; gives exact field semantics for
  Item/Ship/Station/Agent/System/Wanted/Engine/Faction data classes.
- **FMOD-GOF2-Recreated/** — recreated FMOD Designer project for the PC audio events (sound assets).
