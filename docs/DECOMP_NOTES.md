# Galaxy on Fire 2 — Decompilation Project Notes

Goal: a **matching decompilation** of the full *Abyss Engine* + *Galaxy on Fire 2* game
(C++), recompilable back to byte-identical binaries.

---

## 1. Source inventory & triage

| Source | File | Format / Arch | Encrypted | Named funcs | Toolchain | Verdict |
|---|---|---|---|---|---|---|
| **iOS 1.1.4 HD** | `Original IPA/...HD_1.1.4_ios_4.3.ipa` → `GalaxyOnFire2 HD` | Mach-O **armv7** | **No** (`cryptid 0`) | 664 (653 C++ methods, 27 AE classes) | Xcode SDK 6.0, **llvm-gcc/clang + libstdc++** | **Co-primary** (unencrypted; iOS platform layer + statically-linked FMOD) |
| **Android 2.0.16 HD** | `Original APK/...2.0.16_APKPure.apk` → `lib/armeabi-v7a/libgof2hdaa.so` | ELF **ARMv7** | No | **4524** (4075 C++ methods, 133 AE classes) | **NDK r18, clang 7.0.2 (LLVM 7.0.2) / GCC 4.9** | **Primary** (richest symbols; reproducible toolchain) |
| iOS 1.0.5 | `Original IPA/...1.0.5_ios_3.0.ipa` → `GalaxyOnFire2` | Mach-O universal armv6+**armv7** | No | 162 (155 methods, 25 AE classes) | older Apple GCC | Secondary cross-ref (matches DeepOpen 1.0.1 era) |
| Windows Steam | `Original Windows/.../GOF2.exe` | PE32 **x86** | No | 0 (stripped) | MSVC (vc10) | **Reference only** — cut content/features; ModApi offsets validate struct layouts |
| KiritoJPK Android | `KiritoJPK_.../GOF2FHD_APK.7z` + OBB | (7z, not yet extracted) | — | — | — | Modded/repacked Android — assets/OBB reference only |
| DeepOpen | `DeepOpen/code/GoF2_jsr/` | **Java (J2ME)**, v1 | — | — | — | **Logic reference (not byte-match)** — deobfuscated/refactored Java decomp of the *same codebase lineage*: classes map ~1:1 to the C++ (`AEMath`/`AEMatrix`/`AEVector3D`, `AECamera`, `TargetFollowCamera`; `PlayerEgo`, `KIPlayer`, `Status`, `Ship`, `Mission`, `LevelScript`, `Globals`, `Item`, `Agent`, `Radio`…). Use to understand **what a murky decompiled function does** (algorithms, control flow, field semantics). Wrong language + earlier version ⇒ never for codegen/matching. Also `extras/gof2-1.0.1-ios-symbols` (naming ref) |

### Why these targets
- **Both iOS binaries are NOT FairPlay-encrypted** (`LC_ENCRYPTION_INFO cryptid 0`) → `__TEXT` is fully readable. This is what makes iOS usable at all.
- **Android `.so` has by far the richest symbol table**: 4524 exported functions (no `-fvisibility=hidden`), demangling to **133 `AbyssEngine` classes** + the full game class hierarchy. It is the best map of the engine+game structure.
- **The recovered C++ source is shared across all platforms** (Abyss Engine + game). We name/structure from the richest binary (Android), cross-validate against iOS, and pick a matching-build target per platform.

### Code architecture (from symbols)
- Engine namespace: **`AbyssEngine`** (~1001 methods) — `Engine`, `Mesh`, `Transform`, `Camera`, `Material`, `String`, `Array<T>`, `Image2D`, `SpriteSystem`, `AEMath::Matrix`, file I/O (`AEFile`, `AEPakFileEntry`, `AELowLevelFile`), sound (`AESoundInterface`), shaders…
- Game lives in top-level classes (Android-only symbols, 4392 unique): **`PlayerEgo`** (player ship, 168), `Status`, `Player`, `Ship`, `Level`/`LevelScript`, **`KIPlayer`** (enemy AI; "KI" = German for AI), `Item`, `Mission`, `Hud`, `Agent`, `Radio`/`RadioMessage`, weapons (`BombGun`, `MineGun`), `Achievements`, and the **`NFC::`** namespace (store / DLC, e.g. `NFC::iap_buy_dlc_valkyrie()`).
- iOS-only delta (538): mostly **FMOD** (392, statically linked on iOS; a separate `libfmodex.so` on Android) + template instantiations + a few file classes.