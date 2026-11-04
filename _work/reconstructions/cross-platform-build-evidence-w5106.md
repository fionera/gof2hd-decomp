# Cross-platform build evidence: iOS 1.0.5/1.1.4 and Windows vs Android 2.0.16 (w5106, wave 101)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/wb106`,
branch `wave101-wb106`, base `3856949ab`. Scratch is `/dev/shm/wb106`.

This pass mines the shipped iOS and Windows artifacts and the original source-layout
dumps for build-system evidence, and checks whether any of the 24 non-exact Android
functions has a closer cross-platform sibling that reveals the real source shape.
No source change was landed. The canonical gate is unchanged:

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0
data_exact 843/843  imports 207  lint CLEAN  drift 0
RATCHET PASS
```

## 0. Bottom line

* **The source-layout dumps contain no compiler or assembler flags.** They record
  only Xcode build-configuration paths, architecture, source paths and per-TU
  symbol lists. The only build metadata they carry is
  `Release-iphoneos/arm64` (HD 1.1.19), `Distribute-iphoneos/armv6` (SD 1.0.1 and
  3D 1.1.4), and the Xcode DerivedData root. The flag recipe in
  `cmake/toolchain.cmake` cannot be confirmed or refuted from the dumps.
* **TU membership is confirmed for all 24 non-exact functions.** Every one is
  assigned by the `gof2hd_1.1.19_ios` dump to exactly the TU our repo uses.
* **Emitted function order is confirmed for their TUs.** The dump's symbol order
  is the object section order, and our `-ffunction-sections` objects reproduce it
  (e.g. `Level.cpp`: ctor, dtor, `setInitStreamOut`, `init`, `createSpace`,
  `createPlayer`, `createAsteroids`, `createGasClouds`, ...; the Android symbol
  addresses match exactly).
* **The iOS 1.1.4 HD binary is a usable codegen sibling for 3 of the 24.** Its
  Itanium vtables name `MGame::OnUpdate`, `MGame::OnTouchEnd(int,int,void*)` and
  `RocketGun::update(int)`. All three have the same source structure as Android
  2.0.16 (same prologue, same field accesses, same branch/loop order). The other
  21 are either non-polymorphic or (ModStation) have a constructor target, and
  their names are stripped (`nlocalsym=1`).
* **The iOS 1.0.5 SD binary is an unoptimized (-O0-like) source-shape oracle.** It
  has the same `MGame`/`RocketGun` source structure, including the two separate
  `beq` branches for `hasGeometry && delayActive` that the Android
  `__builtin_expect` anchor is compensating for.
* **The Windows Steam exe is packed/encrypted.** `.text` entropy is exactly
  8.000 and the entry point is a decompressor stub in `.bind`; the on-disk
  `.text` does not disassemble. It is useful for RTTI/metadata only, not codegen.
* **Shared vs Android-specific flags.** iOS and Android share Thumb-2, NEON and
  the softfp float ABI (proved by `Vector::Vector(float,float,float)` taking its
  floats in r1-r3 on both). They differ in compiler (Apple clang 4.x vs clang
  7.0.2), optimization (`-Os` vs `-Oz`), STL (libstdc++.6 vs libc++), exceptions
  (SJLj vs EHABI), stack protector (111 vs 787 canaries), and symbol visibility
  (hidden game symbols vs default).
* **The 24 residuals are not explained by a cross-platform source-shape
  difference.** The iOS siblings confirm the source structure; the remaining
  differences are compiler/regalloc. This is consistent with the wave-100
  conclusion that the residual is source shape of the *individual* functions
  and codegen, not a build flag.

## 1. Artifacts

```sh
cd /home/debian/GalaxyOnFire2/_work/bins
sha256sum ios_1.1.4_HD ios_1.0.5_armv7 ios_1.0.5_GalaxyOnFire2 win_GOF2.exe
file ios_1.1.4_HD ios_1.0.5_armv7 ios_1.0.5_GalaxyOnFire2 win_GOF2.exe
```

| artifact | sha256 | format | build metadata |
|---|---|---|---|
| `ios_1.1.4_HD` | `74ece5be...4958` | Mach-O armv7, PIE, Thumb-2 | `LC_VERSION_MIN_IPHONEOS 4.3`, SDK 6.0, UUID `F0BB0EED-...`, `cryptid 0`; links `libstdc++.6`, `libz.1`, OpenGLES, OpenAL; SJLj exceptions |
| `ios_1.0.5_armv7` | `b8e8000f...4661` | Mach-O armv7, non-PIE, Thumb-2 | IPA name iOS 3.0, UUID `84309E35-...`, `cryptid 0`; links `libstdc++.6`; unoptimized (-O0-like) code |
| `ios_1.0.5_GalaxyOnFire2` | `8420bb7a...80e3` | universal armv6+armv7 | same 1.0.5 app |
| `win_GOF2.exe` | `1234131b...1fb9` | PE32 i386 | MSVC 2010 (`MSVCR100.dll`, linker 10.0), timestamp 2012-09-05, image base 0x400000; imports `fmod_event.dll`, `steam_api.dll`; packed |

The original APK/IPA are:
`Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`,
`Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa`,
`Original IPA/Galaxy_on_Fire_2__1.0.5_ios_3.0.ipa`.
The IPAs contain no dSYM; the extracted binaries are the only shipped code.

### 1.1 The Windows exe is packed

```sh
python3 - <<'PY'
import math, struct
from collections import Counter
b=open('/home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]
n=struct.unpack_from('<H',b,pe+6)[0]; optsz=struct.unpack_from('<H',b,pe+20)[0]
so=pe+24+optsz
for i in range(n):
    o=so+i*40; nm=b[o:o+8].rstrip(b'\0').decode()
    vsz,va,rsz,raw=struct.unpack_from('<IIII',b,o+8)
    d=b[raw:raw+rsz]; c=Counter(d)
    H=-sum(v/len(d)*math.log2(v/len(d)) for v in c.values())
    print(f'{nm:8s} size {rsz:8d} entropy {H:.3f}')
PY
```

```text
.text    size  1225728 entropy 8.000
.rdata   size   257024 entropy 5.367
.data    size   648704 entropy 3.953
.rsrc    size   221184 entropy 7.443
.reloc   size    88576 entropy 6.457
.bind    size   376832 entropy 7.996
```

`.text` is perfectly uniform (8.000 bits/byte), and the PE entry point
(`0x65d2ee`) is a `rep movsl`/decrypt stub in `.bind`. The on-disk `.text`
therefore does not disassemble (`objdump -d --start-address=0x48f4fb` yields
data). The MSVC RTTI names (`.?AVMGame@@`, `.?AVModStation@@`, ...) and the
vtable/COL structure are still present in `.rdata`/`.data`, so the exe is useful
for class inventory and metadata, not for instruction-level comparison.

## 2. Dump mining

```sh
cd "/home/debian/GalaxyOnFire2/source file list dump"
grep -nE 'clang|gcc|GCC|CFLAG|OPTIM|ARCHS|SDKROOT|DEPLOYMENT|LLVM|OTHER_|DEBUG|Release|Distribute|armv|arm64|i386' \
  *.cpp "source file lists dump.txt"
```

The only hits are the object paths
(`.../Release-iphoneos/.../Objects-normal/arm64/...` and
`.../Distribute-iphoneos/.../Objects-normal/armv6/...`) and the words
`Release`/`Distribute`. There are **no** `-O*`, `-f*`, `-m*`, `ARCHS`,
`SDKROOT`, `IPHONEOS_DEPLOYMENT_TARGET` or compiler-version strings. The dumps
are symbol/contributor lists, not build logs.

### 2.1 TU membership of the 24 non-exact functions

```sh
python3 /dev/shm/wb106/check_tu.py gof2hd_1.1.19_ios.cpp
```

Every one of the 20 unique functions (24 with ctor/dtor duplicates) resolves to
the expected TU:

| function | dump TU | our TU |
|---|---|---|
| `Layout::Layout()` | `Layout.cpp` | `src/game/ui/Layout.cpp` |
| `LevelScript::process(int)` | `LevelScript.cpp` | `src/game/world/LevelScript.cpp` |
| `Radar::draw(Player*, Hud*, int)` | `Radar.cpp` | `src/game/weapons/Radar.cpp` |
| `Generator::createMission(Agent*, Array<SolarSystem*>*)` | `Generator.cpp` | `src/game/mission/Generator.cpp` |
| `Level::createShip(int,int,int,Waypoint*,bool,bool)` | `Level.cpp` | `src/game/world/Level.cpp` |
| `MGame::OnTouchEnd(int,int,void*)` | `MGame.cpp` | `src/game/menu/MGame.cpp` |
| `Globals::Globals()` | `Globals.cpp` | `src/game/core/Globals.cpp` |
| `HangarWindow::render()` | `HangarWindow.cpp` | `src/game/ui/HangarWindow.cpp` |
| `Status::missionCompleted(bool,bool,long long)` | `Status.cpp` | `src/game/mission/Status.cpp` |
| `RocketGun::update(int)` | `RocketGun.cpp` | `src/game/weapons/RocketGun.cpp` |
| `PlayerEgo::calcCollision(Array<KIPlayer*>*)` | `PlayerEgo.cpp` | `src/game/ship/PlayerEgo.cpp` |
| `Level::createScene()` | `Level.cpp` | `src/game/world/Level.cpp` |
| `Gun::calcCharacterCollision()` | `Gun.cpp` | `src/game/weapons/Gun.cpp` |
| `Level::createMission()` | `Level.cpp` | `src/game/world/Level.cpp` |
| `Hud::draw(long long,long long,PlayerEgo*,bool,unsigned,unsigned)` | `Hud.cpp` | `src/game/ui/Hud.cpp` |
| `MenuTouchWindow::MenuTouchWindow(int)` | `MenuTouchWindow.cpp` | `src/game/ui/MenuTouchWindow.cpp` |
| `SpaceLounge::drawLounge()` | `SpaceLounge.cpp` | `src/game/menu/SpaceLounge.cpp` |
| `MenuTouchWindow::OnTouchBegin(int,int,void*)` | `MenuTouchWindow.cpp` | `src/game/ui/MenuTouchWindow.cpp` |
| `ModStation::ModStation()` | `ModStation.cpp` | `src/game/menu/ModStation.cpp` |
| `MGame::OnUpdate()` | `MGame.cpp` | `src/game/menu/MGame.cpp` |

No TU move is indicated.

### 2.2 Emitted function order

The dump's symbol order is the object section order. `cmp_order.py` compares it
to our `-ffunction-sections` object section order for every TU. For `Level.cpp`
the first symbols are identical:

```text
dump: Level::Level(int), Level::~Level(), Level::setInitStreamOut(), Level::init(),
      Level::createSpace(), Level::createPlayer(), Level::createAsteroids(),
      Level::createGasClouds(), Level::createMission(), Level::createScene(), ...
obj : Level::Level(int), Level::~Level(), Level::setInitStreamOut(), Level::init(),
      Level::createSpace(), Level::createPlayer(), Level::createAsteroids(),
      Level::createGasClouds(), Level::createMission(), Level::createScene(), ...
```

The extra object entries are local template instantiations (`Array<T>::Array()`,
`ArraySetLength<T>`) that the dump omits. The Android addresses confirm the
order: `_ZN5LevelC1Ei` = `0xadac9`, `_ZN5Level13createMissionEv` = `0xaffd9`,
`_ZN5Level11createSceneEv` = `0xb42ed` in both the original and our build.

## 3. Cross-platform codegen of the 24

The iOS 1.1.4 HD binary exports only 664 `T` symbols (engine + FMOD) and has
`nlocalsym=1`; game symbols are stripped. Three of the 24 are still locatable
through their Itanium vtables:

```sh
# Itanium vtable -> typeinfo -> name, then slot order from IApplicationModule
python3 /dev/shm/wb106/macho_vt.py ios_1.1.4_HD MGame ModStation RocketGun
```

| function | iOS 1.1.4 vtable slot | iOS address | iOS size | Android size |
|---|---|---|---:|---:|---:|
| `MGame::OnUpdate()` | `MGame` slot 12 | `0x167f4d` | 17,076 | 11,440 |
| `MGame::OnTouchEnd(int,int,void*)` | `MGame` slot 11 | `0x1648ad` | 11,164 | 8,080 |
| `RocketGun::update(int)` | `RocketGun` slot 4 | `0x1ac0d9` | 1,988 | 1,908 |

(`MGame` vtable `0x445408`, `ModStation` `0x445518`, `RocketGun` `0x445d18`; the
slot order is the `IApplicationModule`/`AbstractGun` declaration order.)

### 3.1 `RocketGun::update(int)`

```sh
llvm-objdump -d --triple=thumbv7-none-eabi --start-address=0x1ac0d8 \
  --stop-address=0x1ac180 ios_1.1.4_HD
llvm-objdump -d --triple=thumbv7-none-eabi --start-address=0x15ecd8 \
  --stop-address=0x15ed60 android_2.0.16_libgof2hdaa.so
llvm-objdump -d --triple=thumbv7-none-eabi --start-address=0xaf0f8 \
  --stop-address=0xaf1c0 ios_1.0.5_armv7
```

All three builds have the same prologue and the same two separate conditional
branches for `hasGeometry && delayActive`:

```text
iOS 1.1.4:  ldrb r0,[r6,#0x1c]; tst.w r0,#1; beq ...
            ldr r0,[r6,#8]; ldrb r0,[r0,#0xa9]; tst.w r0,#1; beq ...
Android:    ldrb r0,[r10,#0x1c]; cmp r0,#0; beq ...
            ldr r0,[r10,#8]; ldrb r0,[r0,#0xa9]; cmp r0,#0; beq ...
iOS 1.0.5:  ldrb r3,[r5,#0x1c]; cmp r3,#0; beq ...
            ldr r0,[r5,#8]; ldrb r3,[r0,#0x94]; cmp r3,#0; beq ...
```

They also all zero three words at entry (a local `Array` object):
iOS 1.1.4 `str r0,[sp,#0x264/0x268/0x26c]`, Android
`strd r0,r0,[sp,#144]; str r0,[sp,#0x98]`, iOS 1.0.5
`str r3,[sp,#0x124/0x128/0x12c]`.

The Android source currently carries
`__builtin_expect(this->hasGeometry != 0 && this->gun->delayActive != 0, 1)`
with the comment that the plain form lowers the raw match 86.9 -> 84.6. The
cross-platform evidence confirms the **natural source is the plain `&&`** and
that the original compiler emitted two branches; the anchor is a codegen
workaround, not a semantic difference.

### 3.2 `MGame::OnUpdate()`

```sh
llvm-objdump -d --triple=thumbv7-none-eabi --start-address=0x167f4c \
  --stop-address=0x168060 ios_1.1.4_HD
llvm-objdump -d --triple=thumbv7-none-eabi --start-address=0x17c8d8 \
  --stop-address=0x17c9f0 android_2.0.16_libgof2hdaa.so
```

Both start with `ldr r0,[this,#8]` (the `ApplicationManager` at the Itanium
`IApplicationModule` offset 8), call `GetElapsedTimeMillis`, and clamp with the
same literal `0x96` (150):

```text
iOS:      cmp r4,#0x96; bgt ...
Android:  cmp r0,#0x96; bgt ...
iOS 1.0.5: cmp r0,#0x96; ble ...
```

This is the same `delta = clamp(elapsed, 0, 150)` source shape.

### 3.3 `MGame::OnTouchEnd(int,int,void*)`

```sh
llvm-objdump -d --triple=thumbv7-none-eabi --start-address=0x1648ac \
  --stop-address=0x164980 ios_1.1.4_HD
llvm-objdump -d --triple=thumbv7-none-eabi --start-address=0x17a144 \
  --stop-address=0x17a220 android_2.0.16_libgof2hdaa.so
```

Both start with the same three statements in the same order:

```text
iOS:      ldr r4,[r11,#0xb8]; cmp r4,r5; itt eq; moveq r0,#0; streq r0,[r11,#0xb8]
          ldr r0,[r11,#0x54]; bl <isAutoPilot>; str r0,[sp,#0x20]
          mov r0,#0x3f800000; str r0,[r11,#0x15c]
Android:  ldr r0,[r11,#0xc0]; cmp r0,r3; itt eq; moveq r0,#0; streq r0,[r11,#0xc0]
          ldr r0,[r10,#88]; bl <isAutoPilot>; mov r8,r0
          mov r1,#0x3f800000; str r1,[r10,#0x110]
```

i.e. `if (activeTouchId == touchId) activeTouchId = nullptr;`,
`bool wasAutoPilot = player->isAutoPilot();`, `flFastForwardWeight = 1.0f;`.
The field offsets differ (`activeTouchId` 0xb8 vs 0xc0, `player` 0x54 vs 0x58,
`flFastForwardWeight` 0x15c vs 0x110) because the `MGame` struct grew between
1.1.4 and 2.0.16; the **statement order is identical**.

The iOS 1.0.5 -O0 build shows the same sequence with its own offsets
(`0x154` for `flFastForwardWeight`, `0xe8` for the camera, `0x59` for
`pauseOpen`, `0xc1` for `choiceWindowOpen`), confirming that the source shape is
stable across all three builds.

### 3.4 The other 21

`Layout`, `LevelScript`, `Radar`, `Generator`, `Level`, `Globals`,
`HangarWindow`, `Status`, `PlayerEgo`, `Gun`, `Hud`, `MenuTouchWindow`,
`SpaceLounge` are not polymorphic in the original (no RTTI/vtable in either iOS
binary), and the iOS game symbols are stripped. `ModStation` is polymorphic
(`IApplicationModule`), but its target `ModStation::ModStation()` is a
constructor, so it is not in the vtable either. These cannot be located by name
without a full function-start/order reconstruction. The Windows exe is packed,
so it cannot supply the code either. This is the main limitation of the
cross-platform sibling check.

## 4. Per-platform flags: proven vs hypothesis

### PROVEN from the artifacts

| property | iOS 1.1.4 HD | Android 2.0.16 | evidence |
|---|---|---|---|
| ISA / mode | ARMv7 Thumb-2 | ARMv7 Thumb-2 | odd vtable entries; Thumb disassembly; `.ARM.attributes` |
| float ABI | **softfp** | **softfp** | `Vector::Vector(float,float,float)` takes x,y,z in r1,r2,r3 then `vmov sN,rN` in both |
| NEON/VFP | VFPv3+NEON | VFPv3+NEON | NEON instructions; `-mfpu=neon` |
| STL | `libstdc++.6` | `libc++` | `LC_LOAD_DYLIB`; Android `__cxa_*` bytes match `libc++abi.a` |
| exceptions | SJLj (`__gxx_personality_sj0`) | EHABI (`_Unwind_Resume`, `.ARM.exidx`) | imports/sections |
| stack protector | 111 `__stack_chk_fail` calls | 787 | stub call counts |
| thread-safe statics | 16 `__cxa_guard_acquire` | 2 | stub call counts |
| visibility | game symbols hidden/stripped | default (4524 exported) | `nlocalsym=1`; Android dynsym |
| PIE | 1.1.4 yes, 1.0.5 no | yes (ET_DYN) | Mach-O flags; ELF type |

### HYPOTHESIS (consistent with the artifacts, not directly encoded)

| property | iOS 1.1.4 HD | Android 2.0.16 | evidence |
|---|---|---|---|
| optimization | `-Os`-like (size) | `-Oz` | iOS code is size-optimized but larger than Android; no debug info/flags in the binary |
| compiler version | Apple clang ~4.x (Xcode 4.5/4.6 era) | clang 7.0.2 | SJLj personality, `tst` bool idiom, 2012 build date; Android `.comment` is explicit |
| stack-protector mode | basic (`-fstack-protector`) | `-fstack-protector-strong` | 111 vs 787 canaries; the exact mode is not encoded |

### Android-specific (not shared with iOS)

* `-Oz` (iOS uses `-Os`-like size optimization).
* `-fstack-protector-strong` (iOS has far fewer canaries; the exact iOS
  protector mode is not encoded in the binary).
* EHABI exceptions + `-funwind-tables` (iOS uses SJLj).
* libc++ (iOS uses libstdc++.6).
* default visibility for the game TUs (iOS hides them).
* `-ffunction-sections` (the iOS build does not need it; its `__text` is one
  section).

### Shared / not Android-specific

* Thumb-2, NEON, softfp, ARMv7-A.
* `-fPIC`/PIE (1.1.4 is PIE).
* RTTI (both have typeinfo/vtables).
* frame pointers (both keep `r7`).

## 5. A/B measurements

Two cross-platform-motivated flags were tested in scratch builds with the
official verifier. The canonical `cmake-build-match` was not touched and
`--update-baseline` was never run.

```sh
cat > /dev/shm/wb106/flag_fingerprint/toolchain_extra.cmake <<'WRAP'
include("/home/debian/GalaxyOnFire2/wt/wb106/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
WRAP

cmake -S . -B /dev/shm/wb106/flag_fingerprint/build-TAG -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/wb106/flag_fingerprint/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_EXTRA_FLAGS='FLAG'
cmake --build /dev/shm/wb106/flag_fingerprint/build-TAG --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/wb106/flag_fingerprint/verify-TAG \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/wb106/flag_fingerprint/build-TAG/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

| variant | byte_exact | linked_exact | size-exact | `.text` | `.rodata` | `.ARM.exidx` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|---:|
| baseline `-Oz` | **4500** | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` |
| `-fno-stack-protector` | 1377 | 3781 | 3789/4524 | `0x12e128` | `0x41b67` | `0x6f18` | `0x2607c` |
| `-fsjlj-exceptions` | 1345 | 3762 | 3773/4524 | `0x1d0208` | `0x41b67` | `0x1160` | `0x11a0` |

`-fno-stack-protector` removes the 787 canaries and drops `byte_exact` from
4500 to 1377; `-fsjlj-exceptions` replaces EHABI with SJLj and changes `.text`
from `0x1337c8` to `0x1d0208`. Both confirm that the Android-specific settings
are load-bearing.

### 5.1 `-fsjlj-exceptions` counts

The full `verify.py` difflib on this variant is very slow because almost every
function changes; the run was stopped after 11 minutes. The tool's own fast mode
(`verify_fast.py`, which short-circuits only the fuzzy score and pool
reconciliation) reports:

```text
compared 4524  avg 83.16%  byte_exact 1345  linked_exact 3762  fuzzy_perfect 3762
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
```

`byte_exact` and `size-exact` (1345 and 3773/4524) are independent of the fast
mode; `linked_exact` is the fast-mode value (it can only under-count, never
over-count). This is a research variant, not a landed change.

## 6. Conclusion

* The dumps are **not** a flag source. They are a reliable source for TU
  membership and emitted function order; both are already correct for the 24
  non-exact functions.
* The iOS 1.1.4 HD and 1.0.5 SD siblings **confirm the source shape** of the
  locatable non-exact functions (`RocketGun::update`, `MGame::OnUpdate`,
  `MGame::OnTouchEnd`). Their residuals are compiler/regalloc differences, not
  a different source structure.
* The Windows exe is packed and cannot be used for codegen comparison without
  unpacking; its RTTI is still useful for class inventory.
* The Android recipe's Android-specific flags (`-Oz`,
  `-fstack-protector-strong`, EHABI/`-funwind-tables`, libc++, default
  visibility) are confirmed by the cross-platform contrast and by the A/B
  measurements. No flag change is warranted.
* The remaining 21 non-exact functions have no directly locatable iOS/Windows
  sibling; resolving them still requires Android-side source-shape work.

## 7. Reproduction scripts

The scratch scripts used in this pass are self-contained here:

* `/dev/shm/wb106/parse_dump2.py` -- parse a symbol dump into TU blocks.
* `/dev/shm/wb106/check_tu.py` -- map the 24 functions to dump TUs.
* `/dev/shm/wb106/cmp_order.py` -- compare dump symbol order to our object
  section order.
* `/dev/shm/wb106/macho_vt.py` -- Itanium vtable/RTTI locator for Mach-O.
* `/dev/shm/wb106/macho_fstarts.py` -- `LC_FUNCTION_STARTS` decoder.
* `/dev/shm/wb106/pe_rtti2.py` / `pe_vt.py` -- MSVC RTTI/vtable locator.

They are scratch-only; the durable evidence is the commands and tables above.
