# ndk-build Android.mk reconstruction (w3420)

Status: research note, 2026-11-01. Worktree `wt/w3420`, branch `wave87-w3420`,
base `d00e0030` (current master). Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled rebuild of this tree.

## 0. Bottom line

The original native build is an **NDK r18b ndk-build** build. The reconstructed module shape is:

```make
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := c++_static
APP_OPTIM := release

LOCAL_MODULE := gof2hdaa
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L<apk-libs> -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
```

This is not just inferred from the NDK source. An actual r18b `ndk-build` probe with these
variables emits exactly the original's `DT_NEEDED` order and exactly the original's
`--whole-archive zlib libzip --no-whole-archive crypto` link order. A full r18b `ndk-build`
build of the current 204-source tree also compiles and links successfully.

The compile-flag set is the r18b default set. The only authentic default the tree deliberately
does **not** pass is `-fdata-sections`: at the current tree state it costs **-71 byte_exact**
(4182 -> 4111), so it is documented here but not landed. The full ndk-build link-flag set is
byte-identical to the current link (same sha256).

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

Measured:

```text
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`size-exact` from `cmake-build-match/verify/report.json` is **4524 / 4524**. Lint is
`CLEAN`. Every ELF section is size- and base-exact:

| section | address | size |
|---|---:|---:|
| `.text` | `0x69440` | `0x1337c8` |
| `.ARM.exidx` | `0x19cc08` | `0x6f70` |
| `.ARM.extab` | `0x1a3b78` | `0x26298` |
| `.rodata` | `0x1c9e10` | `0x41b67` |
| `.fini_array` | `0x20cba0` | `0x8` |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` |
| `.init_array` | `0x20fb98` | `0xc` |
| `.dynamic` | `0x20fba4` | `0x148` |
| `.got` | `0x20fcf4` | `0x430c` |
| `.data` | `0x214000` | `0x296c` |
| `.bss` | `0x216970` | `0x69ac` |

The baseline `.so` sha256 is
`c6130f568f1a1cdb019c64b61398b7271a3df66281991f7c16031a50c5028a99`.

## 2. What an actual r18b ndk-build emits

A scratch project was created under `/tmp/w3420/ndkprobe` with `jni/Android.mk` and
`jni/Application.mk` containing the reconstructed variables above. `ndk-build V=1` was run
with the repo's NDK r18b:

```sh
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/tmp/w3420/ndkprobe \
  APP_BUILD_SCRIPT=/tmp/w3420/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3420/ndkprobe/jni/Application.mk V=1 -j8
```

The emitted compile command is the canonical r18b command:

```text
clang++ -target armv7-none-linux-androideabi16
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables
  -no-canonical-prefixes -g
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument
  -fno-exceptions -fno-rtti
  -gcc-toolchain .../arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -Oz -DNDEBUG -mfpu=neon
  ... -std=c++11 ... -frtti -fexceptions ...
```

The emitted shared-library link command contains, in this order:

```text
-Wl,-soname,libgof2hdaa.so -shared --sysroot=.../platforms/android-16/arch-arm
<objects>
-Wl,--whole-archive <zlib.a> <libzip.a> -Wl,--no-whole-archive <libcrypto.a>
<libc++_static.a> <libc++abi.a> <libandroid_support.a> <libunwind.a>
-lgcc -Wl,--exclude-libs,libgcc.a -latomic -Wl,--exclude-libs,libatomic.a
-target armv7-none-linux-androideabi16 -no-canonical-prefixes
-gcc-toolchain ... -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a
-Wl,--build-id -nostdlib++ -Wl,--no-undefined -Wl,-z,noexecstack
-Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings
-L... -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid ... -lfmodex -lfmodevent -lm -ldl
-ldl -lc -lm
```

The important observable is the resulting `DT_NEEDED` order. With
`LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -L<apk-libs> -lfmodex -lfmodevent -lm -ldl`
the probe's `readelf -d` is exactly the original's:

```text
liblog.so libGLESv2.so libGLESv1_CM.so libEGL.so libandroid.so
libfmodex.so libfmodevent.so libm.so libdl.so libc.so
```

This proves the `LOCAL_LDLIBS` line (including `-lm -ldl` before ndk-build's own
`-lc -lm`) reproduces the original NEEDED order. It also proves the FMOD libraries can be
listed in `LOCAL_LDLIBS`; ndk-build only warns that they are non-system libraries. The
binary cannot distinguish `LOCAL_LDLIBS` from `LOCAL_SHARED_LIBRARIES` for those two, but
the NEEDED order is exact either way.

With the prebuilt static-library modules:

```make
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
```

the emitted link line is exactly:

```text
-Wl,--whole-archive libz_gof2.a libzip_gof2.a -Wl,--no-whole-archive libcrypto_gof2.a
```

This is the same order the current CMake link uses, and it is the order required to keep the
zlib/libzip exported symbol set and the original `.text`/PLT blocks.

## 3. Controlled compile-flag A/B at the current tree

### 3.1 Full ndk-build compile flags (minus `-fdata-sections`) are neutral

A scratch wrapper toolchain appended the authentic r18b flags after the repo's canonical set:

```sh
cmake -S . -B /tmp/w3420/build-ndkflags-1 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3420/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  '-DGOF2_EXTRA_FLAGS=-DNDEBUG -fpic -fexceptions -no-canonical-prefixes -Wno-invalid-command-line-argument -Wno-unused-command-line-argument -gcc-toolchain /home/debian/GalaxyOnFire2/wt/w3420/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 -mfpu=vfpv3-d16 -mfpu=neon'
cmake --build /tmp/w3420/build-ndkflags-1 --target check -j8
```

Result: **byte 4182, linked 4495, size 4524/4524**, `RATCHET PASS`. Every code/data section
(`.text`, `.rodata`, `.data`, `.ARM.exidx`, `.ARM.extab`, `.got`, `.dynamic`) is byte-identical
to the baseline. The two `.so` files differ in exactly 20 bytes, all inside
`.note.gnu.build-id` (offsets 477..496). So `-DNDEBUG`, `-fpic`, `-fexceptions`,
`-no-canonical-prefixes`, the two `-Wno-*` flags, `-gcc-toolchain`, and the
`-mfpu=vfpv3-d16 -mfpu=neon` ordering are gate-neutral at this tree state.

A single-TU ndk-build compile of `src/engine/math/Quaternion.cpp` with the same module
variables produced an object whose `.text.*`, `.rodata`, `.data`, `.ARM.exidx`, `.ARM.extab`
and relocation sections are byte-identical to the CMake object; only `.debug_line`,
`.debug_str` and `.symtab` differ (paths and symbol ordering).

### 3.2 `-fdata-sections` is authentic but currently negative

```sh
cmake -S . -B /tmp/w3420/build-fdata-1 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3420/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS=-fdata-sections
cmake --build /tmp/w3420/build-fdata-1 --target check -j8
```

Result:

| build | byte | linked | size | `.text` | `.rodata` | `.data` |
|---|---:|---:|---:|---:|---:|---:|
| baseline | **4182** | **4495** | 4524/4524 | `0x1337c8` | `0x41b67` | `0x296c` |
| app TUs `-fdata-sections` | **4111** | 4495 | 4524/4524 | `0x1337c8` | `0x41a27` | `0x2962` |

`-fdata-sections` is an r18b `GLOBAL_CFLAGS` default and is certainly in the original build,
but at this tree state it moves `.rodata` by `-0x140` and `.data` by `-0xa` and costs 71
byte-exact functions. It is therefore **not landed**. This is a data/TU-layout interaction,
not evidence against ndk-build. (The vendored zlib/libzip are built by a separate script and
were not included in this app-TU-only A/B.)

## 4. Controlled link-flag A/B at the current tree

The current link command was extracted with:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1
```

It was re-run with the full ndk-build link set inserted after `--build-id`:

```text
-z relro -z noexecstack --no-undefined --warn-shared-textrel --fatal-warnings --fix-cortex-a8
```

The relinked file is **byte-identical** to the baseline:

```text
c6130f568f1a1cdb019c64b61398b7271a3df66281991f7c16031a50c5028a99  baseline
c6130f568f1a1cdb019c64b61398b7271a3df66281991f7c16031a50c5028a99  ndk-build link flags
```

So `-z relro`, `-z noexecstack`, `--no-undefined`, `--warn-shared-textrel`,
`--fatal-warnings` and `--fix-cortex-a8` are exact no-ops here. `-z now` is already in the
tree and is the only link flag that materially changed the original layout. This confirms the
reconstructed ndk-build link line without adding linker tricks.

## 5. Full ndk-build build of the current tree

A complete scratch ndk-build project was generated with the current 204 source files in the
recovered `link_order.rsp` order, the reconstructed module variables, and the prebuilt
zlib/libzip/libcrypto archives:

```sh
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/tmp/w3420/ndkfull \
  APP_BUILD_SCRIPT=/tmp/w3420/ndkfull/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3420/ndkfull/jni/Application.mk -j8
```

It compiles and links successfully. Verification of the resulting
`/tmp/w3420/ndkfull/libs/armeabi-v7a/libgof2hdaa.so`:

```text
compared 4524  avg 99.69%  byte_exact 1345  linked_exact 3843  fuzzy_perfect 3863
stub_zero_size 0  orig_zero_size 0  missing 0  wrong_type 0  absent 0  extra 0
```

Its `.text` is `0x135f80` (original `0x1337c8`), `.rodata` `0x41eb7`, `.data` `0x2960`,
`.bss` `0x69b8`. The `DT_NEEDED` order is exact and `DT_FLAGS=BIND_NOW` / `DT_FLAGS_1=NOW`
are present. This is **not** a gate improvement: it lacks the CMake tree's section-ordering
file and it links the full `c++_static` runtime (`libc++_static.a`, `libunwind.a`, ...), so
function addresses and local runtime code differ. It is evidence that the reconstructed
Android.mk is buildable and has the right module shape, not that it is the best matching
build for this tree.

## 6. Recovering the original `LOCAL_SRC_FILES` order

The original was linked without a section-ordering file, so its `.text` is input-object order.
The source dumps in `_work/original_layout/original_layout.json` give the symbols of each
original TU. Matching those symbols to the original's strong `T` text symbols and sorting each
TU by its lowest `.text` address recovers the original object order.

Command (self-contained; run from the repo root):

```python
import json, subprocess, os
NM = '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
out = subprocess.check_output([NM, '-D', '--defined-only', '-n',
    '../_work/bins/android_2.0.16_libgof2hdaa.so'], text=True)
rows = []
for ln in out.splitlines():
    p = ln.split()
    if len(p) >= 3:
        rows.append((int(p[0], 16), p[1], p[2]))
dem = subprocess.run(['llvm-cxxfilt'], input='\n'.join(n for _, _, n in rows),
                     text=True, capture_output=True).stdout.splitlines()
m = {}
for (a, t, n), d in zip(rows, dem):
    for k in (d, d.lstrip('_'), n):
        m.setdefault(k, []).append((a, t))
d = json.load(open('_work/original_layout/original_layout.json'))
recs = []
for dump, tus in d['tu_symbols'].items():
    for tu, v in tus.items():
        addrs = []
        for s in v['symbols']:
            for c in (s, s.lstrip('_'), s[1:] if s.startswith('_') else s):
                for a, t in m.get(c, []):
                    if t == 'T' and 0x69440 <= a < 0x19cc08:
                        addrs.append(a)
        if addrs:
            recs.append((min(addrs), dump, tu, len(addrs), len(v['symbols'])))
```

**Result:** 154 original TUs have at least one strong text symbol in the Android binary.
Sorting them by that address gives the table in Appendix B. The order is the original link
order for those TUs: engine TUs first (`Mesh`, `Engine_iPhone`, `EaseInOutMatrix`, `Image`,
`AEFile`, `Camera`, ...), then game TUs (`FModSound`, `LODManager`, `Generator`, `MTitle`,
...), then zlib (`inflate`, `compress`, `deflate`, ...).

Cross-check against the current `link_order.rsp`: 130 of the 154 original TU basenames also
occur in the current link order. With basename matching, their relative order has a longest
increasing subsequence of **129 / 130**; the only ambiguity is duplicate basenames such as
`Matrix.cpp`/`Vector.cpp`. This is strong independent evidence that `tools/gen_link_order.py`
recovered the original object order, not just an address heuristic.

The 24 unmatched TUs are the expected ones: iOS glue (`GamePadHandler.mm`,
`Engine_iPhone.mm`, `AppController.mm`, ...), Android-only 2.0.x code, and the
libzip/libcrypto archives (not in the source dumps). The exact original `LOCAL_SRC_FILES`
line therefore cannot be recovered verbatim, but its **order** is recovered for 154 TUs and
the remaining Android-only TUs are placed by the same address evidence in `link_order.rsp`.

## 7. What is proven vs still hypothesis

**Proven by commands in this note**

* Baseline is byte 4182 / linked 4495 / size 4524/4524 / lint CLEAN / drift 0, with every ELF
  section size- and base-exact.
* Actual r18b ndk-build emits the reconstructed compile command and link command.
* `LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -L<apk-libs> -lfmodex -lfmodevent -lm -ldl`
  reproduces the original `DT_NEEDED` order exactly.
* `LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip` + `LOCAL_STATIC_LIBRARIES := crypto`
  reproduces the original `--whole-archive zlib libzip --no-whole-archive crypto` order.
* The full ndk-build compile flag set (minus `-fdata-sections`) is gate-neutral and leaves
  every code/data section byte-identical; only the build-id changes.
* `-fdata-sections` is authentic but currently costs 71 byte-exact functions.
* The full ndk-build link flag set is byte-identical to the current link.
* A complete ndk-build build of the current 204-source tree succeeds.
* The original object order is recovered for 154 TUs and agrees with `link_order.rsp` at
  129/130 common basenames.

**Hypothesis / limits**

* The exact original `Android.mk`/`Application.mk` text is not in the APK or the source dumps.
  The module block above is a reconstruction, not a recovered file.
* The exact original `LOCAL_SRC_FILES` list cannot be recovered verbatim because the original
  TU split differs from this tree (the `AbyssEngine.cpp` merge, split `AEFile`/`Resource*`/
  `ParticleSettingsRef`, and Android-only 2.0.x TUs). Appendix B gives the recovered order,
  not a verbatim source list.
* `APP_PLATFORM := android-16` is indistinguishable from omitting it (r18b's minimum/default
  is 16). `APP_OPTIM := release` is indistinguishable from omitting it (release is default).
* FMOD in `LOCAL_LDLIBS` vs `LOCAL_SHARED_LIBRARIES` cannot be distinguished from the binary;
  both produce the same NEEDED order. The probe uses `LOCAL_LDLIBS`.
* The vendored zlib/libzip are built by this repo's separate script, not by ndk-build. Their
  `-fdata-sections` behavior was not included in the app-TU A/B.
* The full ndk-build build is not a gate improvement because it lacks the section-ordering
  file and links the full `c++_static` runtime.

## Appendix A — buildable Android.mk for the current tree

This is the exact file used for the full ndk-build build in §5 (paths are relative to a
scratch `jni/` directory containing symlinks to `src/` and `third_party/`):

```make
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := zlib
LOCAL_SRC_FILES := third_party/zlib/libz_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libzip
LOCAL_SRC_FILES := third_party/libzip/libzip_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := third_party/openssl/libcrypto_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := gof2hdaa
LOCAL_SRC_FILES := \
    src/platform/android/BackButton.c \
    src/engine/core/NFC.cpp \
    src/engine/input/VirtualInput.cpp \
    src/engine/file/BlowfishSubkeyFile.cpp \
    src/platform/android/main.c \
    src/platform/android/GOF2HD2012.c \
    src/engine/render/Mesh.cpp \
    src/engine/render/GLES1Compat.cpp \
    src/engine/file/FileInterfaceAndroid.cpp \
    src/engine/math/EaseInOutMatrix.cpp \
    src/engine/render/Image.cpp \
    src/engine/file/AEFile.cpp \
    src/engine/render/Camera.cpp \
    src/engine/math/EaseInOut.cpp \
    src/engine/core/AERandom.cpp \
    src/engine/render/Image2D.cpp \
    src/engine/render/ImageFont.cpp \
    src/game/core/String.cpp \
    src/engine/math/Vector.cpp \
    src/engine/math/BSphere.cpp \
    src/engine/core/IApplicationModule.cpp \
    src/engine/render/Curve.cpp \
    src/game/core/CheatCode.cpp \
    src/engine/math/Transform.cpp \
    src/engine/render/PaintCanvas.cpp \
    src/engine/core/GameTextiPhone.cpp \
    src/engine/render/FBOContainer.cpp \
    src/engine/audio/AESoundRessource.cpp \
    src/engine/math/Matrix.cpp \
    src/engine/math/Quaternion.cpp \
    src/engine/core/GameTextInit.cpp \
    src/engine/core/ApplicationManager.cpp \
    src/engine/render/Engine.cpp \
    src/engine/math/AEMath.cpp \
    src/engine/render/SpriteSystem.cpp \
    src/engine/render/Material.cpp \
    src/engine/file/ConfigReader.cpp \
    src/engine/render/shaders/SpecCubeAlphaMapping.cpp \
    src/game/core/BumpShaderParticle.cpp \
    src/engine/render/shaders/GreenShader.cpp \
    src/engine/render/shaders/DrawFBOShader.cpp \
    src/game/core/BumpRimCubeShader_new.cpp \
    src/engine/render/shaders/SimpleShader.cpp \
    src/engine/render/shaders/TextureAlphaTestShader.cpp \
    src/engine/render/shaders/BloomShader.cpp \
    src/engine/render/shaders/EnergyShield.cpp \
    src/engine/render/shaders/BlurShader.cpp \
    src/game/core/BumpShaderCloak.cpp \
    src/game/core/BumpShaderV4.cpp \
    src/engine/render/shaders/TexOnlyShader.cpp \
    src/engine/render/shaders/BumpShader.cpp \
    src/engine/render/shaders/TextureVtxColorShader.cpp \
    src/game/core/BumpShaderV3.cpp \
    src/engine/render/shaders/SimpleRefractionShader.cpp \
    src/game/core/TextureConference.cpp \
    src/engine/render/shaders/GlowPPShader.cpp \
    src/game/core/BumpShaderV2.cpp \
    src/engine/render/shaders/TextureLightShader.cpp \
    src/engine/render/shaders/NoTexVtxColorShader.cpp \
    src/engine/render/shaders/GenericShader2.cpp \
    src/engine/render/ShaderBaseStruct.cpp \
    src/engine/render/shaders/GenericShader.cpp \
    src/engine/render/shaders/GenericShader1.cpp \
    src/engine/render/shaders/PostBWShader.cpp \
    src/engine/render/shaders/DNSShader.cpp \
    src/game/core/BumpShaderRefract.cpp \
    src/engine/render/shaders/BumpRimCubeShader.cpp \
    src/engine/render/shaders/VertexColorShader.cpp \
    src/engine/render/shaders/NoTexShader.cpp \
    src/engine/render/shaders/CubeMapping.cpp \
    src/engine/render/shaders/CubeNormalMapping.cpp \
    src/engine/render/shaders/TextureShader.cpp \
    src/engine/render/shaders/BumpMapping.cpp \
    src/engine/render/shaders/MaskShader.cpp \
    src/engine/render/shaders/SandboxShader.cpp \
    src/engine/render/shaders/VertexColorAlphaTextureShader.cpp \
    src/engine/render/shaders/GlowShader.cpp \
    src/engine/render/shaders/ColorMixAdd.cpp \
    src/engine/render/shaders/PulseShader.cpp \
    src/engine/render/shaders/SpecCubeMapping.cpp \
    src/engine/audio/FModSound.cpp \
    src/platform/android/NdkHooks.cpp \
    src/platform/android/NdkAutosave.cpp \
    src/engine/render/LODManager.cpp \
    src/game/mission/Generator.cpp \
    src/game/menu/MTitle.cpp \
    src/game/mission/Objective.cpp \
    src/engine/math/BoundingAAB.cpp \
    src/game/core/CutScene.cpp \
    src/game/ship/PlayerEgo.cpp \
    src/game/ui/TouchSlider.cpp \
    src/game/ship/Player.cpp \
    src/engine/render/AEGeometry.cpp \
    src/game/ship/PlayerJumpgate.cpp \
    src/game/ship/PlayerWormHole.cpp \
    src/game/ship/KIPlayer.cpp \
    src/game/ui/ListItem.cpp \
    src/game/world/Station.cpp \
    src/game/weapons/RepairBeam.cpp \
    src/engine/render/ImagePart.cpp \
    src/game/mission/Explosion.cpp \
    src/game/mission/Status.cpp \
    src/game/world/Level.cpp \
    src/game/world/StarMap.cpp \
    src/game/mission/RecordHandler.cpp \
    src/game/core/GameInit.cpp \
    src/engine/render/Sprite.cpp \
    src/game/ui/Layout.cpp \
    src/game/menu/ModStation.cpp \
    src/game/ship/PlayerFighter.cpp \
    src/game/mission/Item.cpp \
    src/game/ship/PlayerStatic.cpp \
    src/game/world/NewsItem.cpp \
    src/game/ui/WantedWindow.cpp \
    src/game/ship/PlayerAsteroid.cpp \
    src/game/core/Globals.cpp \
    src/engine/render/BuildResourceList.cpp \
    src/engine/render/LowResourceTable.cpp \
    src/engine/render/ResourceMaterial.cpp \
    src/game/world/SystemPathFinder.cpp \
    src/game/world/Route.cpp \
    src/game/ship/PlayerStaticFar.cpp \
    src/engine/render/ImageFactory.cpp \
    src/game/ship/PlayerCreature.cpp \
    src/engine/render/LensFlare.cpp \
    src/game/world/Standing.cpp \
    src/game/core/HangarList.cpp \
    src/game/menu/MiningGame.cpp \
    src/engine/math/BoundingVolume.cpp \
    src/engine/file/FileRead.cpp \
    src/game/ship/PlayerStation.cpp \
    src/game/world/Wanted.cpp \
    src/game/ui/MenuTouchWindow.cpp \
    src/game/mission/PendingProduct.cpp \
    src/game/weapons/Radar.cpp \
    src/game/core/AutoPilotList.cpp \
    src/game/ui/ListItemWindow.cpp \
    src/game/world/StarSystem.cpp \
    src/game/ui/ScrollTouchBox.cpp \
    src/game/world/LevelScript.cpp \
    src/engine/render/MovingStars.cpp \
    src/game/ui/ChoiceWindow.cpp \
    src/game/weapons/BombGun.cpp \
    src/game/ui/HangarWindow.cpp \
    src/game/menu/HackingGame.cpp \
    src/game/ui/MissionsWindow.cpp \
    src/game/weapons/TractorBeam.cpp \
    src/game/core/RadioMessage.cpp \
    src/engine/math/BoundingSphere.cpp \
    src/game/weapons/Gun.cpp \
    src/game/ship/PlayerFixedObject.cpp \
    src/game/core/Radio.cpp \
    src/game/world/SolarSystem.cpp \
    src/game/mission/GameRecord.cpp \
    src/game/weapons/MineGun.cpp \
    src/game/mission/Achievements.cpp \
    src/game/ship/PlayerTurret.cpp \
    src/game/world/Waypoint.cpp \
    src/game/ui/StatusWindow.cpp \
    src/game/ship/TargetFollowCamera.cpp \
    src/game/mission/Mission.cpp \
    src/engine/render/Trail.cpp \
    src/game/weapons/SentryGun.cpp \
    src/game/world/NewsTicker.cpp \
    src/engine/render/Sparks.cpp \
    src/game/ship/PlayerJunk.cpp \
    src/game/weapons/RocketGun.cpp \
    src/engine/render/MarqueeImage.cpp \
    src/game/weapons/ObjectGun.cpp \
    src/game/world/SpacePoint.cpp \
    src/game/ui/Hud.cpp \
    src/game/ui/DialogueWindow.cpp \
    src/game/ui/TouchButton.cpp \
    src/game/weapons/SpriteGun.cpp \
    src/game/menu/SpaceLounge.cpp \
    src/engine/render/MeshMerger.cpp \
    src/game/ui/ScrollTouchWindow.cpp \
    src/game/ship/Ship.cpp \
    src/game/menu/ModMainMenu.cpp \
    src/game/world/Galaxy.cpp \
    src/game/ship/PlayerGasCloud.cpp \
    src/game/mission/BluePrint.cpp \
    src/game/ship/Agent.cpp \
    src/game/weapons/BeamGun.cpp \
    src/game/menu/MGame.cpp \
    src/engine/render/SimpleMeshMerger.cpp \
    src/engine/render/AMeshMerger.cpp \
    src/engine/render/LodMeshMerger.cpp \
    src/engine/render/IParticleSystem.cpp \
    src/engine/render/ParticleSystemSprite.cpp \
    src/engine/render/ParticleSystemManager.cpp \
    src/engine/render/ParticleSettings.cpp \
    src/engine/render/ParticleSystemMesh.cpp \
    src/engine/core/Array.cpp \
    src/engine/core/ArrayInstantiations.cpp \
    src/engine/core/GameText.cpp \
    src/engine/audio/AESound.cpp \
    src/engine/core/AbyssEngine.cpp \
    src/engine/core/KeyCode.cpp \
    src/engine/core/KeyFrame.cpp \
    src/engine/render/ResourceMesh.cpp \
    src/engine/render/ResourceTexture.cpp \
    src/engine/render/ResourceTransform.cpp \
    src/game/weapons/AbstractGun.cpp

LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/third_party/fmod/inc \
    $(LOCAL_PATH)/third_party/gl \
    $(LOCAL_PATH)/third_party/jni \
    $(LOCAL_PATH)/third_party/libzip \
    $(LOCAL_PATH)/third_party/crypto
LOCAL_CFLAGS := -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L/home/debian/GalaxyOnFire2/wt/w3420/../_work/bins/apk_libs \
                -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
include $(BUILD_SHARED_LIBRARY)
```

`Application.mk`:

```make
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := c++_static
APP_OPTIM := release
```

## Appendix B — recovered original TU order (154 TUs)

`min .text` is the lowest strong `T` symbol address of a symbol listed for that TU in the
source dumps. `matched/total` is the number of dump symbols that matched a strong Android
text symbol over the dump's total symbol count. Dump provenance is `gof2hd` (HD iOS 1.1.19),
`gof2sd` (SD iOS 1.0.1 engine), or `gof3d` (GoF1 1.1.4, used only where the GoF2 dumps lack
the TU).

| # | min `.text` | matched/total | dump | original TU |
|---:|---:|---:|---|---|
|   0 | `0x00069e5c` | 9/12 | gof2hd | `Classes/GamePadHandler.mm` |
|   1 | `0x0006ba90` | 7/12 | gof2sd | `Classes/Mesh.cpp` |
|   2 | `0x0006daf2` | 7/21 | gof2sd | `Classes/Engine_iPhone.mm` |
|   3 | `0x0006ddb4` | 14/17 | gof3d | `Classes/Engine.mm` |
|   4 | `0x0006e918` | 7/10 | gof2sd | `Classes/EaseInOutMatrix.cpp` |
|   5 | `0x0006ee94` | 5/15 | gof2sd | `Classes/Image.cpp` |
|   6 | `0x00070010` | 46/65 | gof3d | `Classes/AEFile.cpp` |
|   7 | `0x0007130c` | 2/2 | gof2sd | `Classes/Camera.cpp` |
|   8 | `0x00071780` | 8/8 | gof2sd | `Classes/EaseInOut.cpp` |
|   9 | `0x000719e0` | 7/7 | gof2sd | `Classes/Random.cpp` |
|  10 | `0x00071b64` | 1/1 | gof2sd | `Classes/Image2D.cpp` |
|  11 | `0x00071bc0` | 6/6 | gof2sd | `Classes/ImageFont.cpp` |
|  12 | `0x00072378` | 30/37 | gof2sd | `Classes/AEString.mm` |
|  13 | `0x00072378` | 29/40 | gof3d | `Classes/AEString.cpp` |
|  14 | `0x0007396c` | 20/20 | gof2sd | `Classes/Vector.cpp` |
|  15 | `0x00074154` | 1/1 | gof2sd | `Classes/IApplicationModule.cpp` |
|  16 | `0x00074378` | 8/11 | gof3d | `Classes/CheatCode.cpp` |
|  17 | `0x000746a0` | 16/29 | gof2sd | `Classes/Transform.cpp` |
|  18 | `0x00077674` | 96/169 | gof2sd | `Classes/PaintCanvas.cpp` |
|  19 | `0x0007f2b8` | 3/3 | gof2sd | `Classes/GameText_iPhone.mm` |
|  20 | `0x0007f2b8` | 7/7 | gof3d | `Classes/gametext.mm` |
|  21 | `0x0007fa30` | 21/23 | gof3d | `Classes/AESoundRessource.cpp` |
|  22 | `0x00080130` | 22/23 | gof2sd | `Classes/Matrix.cpp` |
|  23 | `0x00081008` | 12/13 | gof2sd | `Classes/Quaternion.cpp` |
|  24 | `0x0008165c` | 5/6 | gof2sd | `Classes/GameText.cpp` |
|  25 | `0x0008193c` | 29/47 | gof2sd | `Classes/ApplicationManager.cpp` |
|  26 | `0x0008193c` | 48/54 | gof3d | `Classes/ApplicationManager.mm` |
|  27 | `0x00083eec` | 34/60 | gof2sd | `Classes/Engine.cpp` |
|  28 | `0x00086a28` | 6/6 | gof2sd | `Classes/Math16_iPhone.cpp` |
|  29 | `0x00086adc` | 7/8 | gof2sd | `Classes/SpriteSystem.cpp` |
|  30 | `0x000870b8` | 1/5 | gof2sd | `Classes/Material.cpp` |
|  31 | `0x00087258` | 7/13 | gof3d | `Classes/configreader.cpp` |
|  32 | `0x00088f50` | 3/9 | gof2sd | `Classes/Shader/SimpleShader.cpp` |
|  33 | `0x000890c0` | 3/9 | gof2sd | `Classes/Shader/TextureAlphaTestShader.cpp` |
|  34 | `0x0008bd1c` | 3/9 | gof2sd | `Classes/Shader/TextureVtxColorShader.cpp` |
|  35 | `0x0008dadc` | 3/9 | gof2sd | `Classes/Shader/TextureLightShader.cpp` |
|  36 | `0x0008df2c` | 3/9 | gof2sd | `Classes/Shader/NoTexVtxColorShader.cpp` |
|  37 | `0x0008e424` | 2/7 | gof2sd | `Classes/Shader/BaseShader.cpp` |
|  38 | `0x000905c8` | 3/9 | gof2sd | `Classes/Shader/NoTexShader.cpp` |
|  39 | `0x00090ee8` | 3/9 | gof2sd | `Classes/Shader/TextureShader.cpp` |
|  40 | `0x00092b70` | 35/39 | gof2hd | `Classes/Game/FModSound.mm` |
|  41 | `0x0009518c` | 8/9 | gof2hd | `Classes/Game/LODManager.cpp` |
|  42 | `0x0009539c` | 10/17 | gof2hd | `Classes/Game/Generator.cpp` |
|  43 | `0x00097984` | 12/22 | gof2hd | `Classes/Game/MTitle.cpp` |
|  44 | `0x00097bfc` | 6/7 | gof2hd | `Classes/Game/Objective.cpp` |
|  45 | `0x00098104` | 8/11 | gof2hd | `Classes/Game/BoundingAAB.cpp` |
|  46 | `0x000983e4` | 14/18 | gof2hd | `Classes/Game/CutScene.cpp` |
|  47 | `0x000998cc` | 151/158 | gof2hd | `Classes/Game/PlayerEgo.cpp` |
|  48 | `0x000a228c` | 14/14 | gof2hd | `Classes/Game/TouchSlider.cpp` |
|  49 | `0x000a2570` | 91/95 | gof2hd | `Classes/Game/Player.cpp` |
|  50 | `0x000a4398` | 43/43 | gof2hd | `Classes/Game/AEGeometry.cpp` |
|  51 | `0x000a4fa4` | 10/13 | gof2hd | `Classes/Game/PlayerJumpgate.cpp` |
|  52 | `0x000a5254` | 12/15 | gof2hd | `Classes/Game/PlayerWormHole.cpp` |
|  53 | `0x000a57c8` | 45/60 | gof2hd | `Classes/Game/KIPlayer.cpp` |
|  54 | `0x000a64fa` | 21/21 | gof2hd | `Classes/Game/ListItem.cpp` |
|  55 | `0x000a692c` | 33/36 | gof2hd | `Classes/Game/Station.cpp` |
|  56 | `0x000a7100` | 6/8 | gof2hd | `Classes/Game/RepairBeam.cpp` |
|  57 | `0x000a7a74` | 3/3 | gof2hd | `Classes/Game/ImagePart.cpp` |
|  58 | `0x000a7af4` | 16/17 | gof2hd | `Classes/Game/Explosion.cpp` |
|  59 | `0x000a8ac0` | 108/117 | gof2hd | `Classes/Game/Status.cpp` |
|  60 | `0x000adac8` | 82/149 | gof2hd | `Classes/Game/Level.cpp` |
|  61 | `0x000c67a4` | 20/25 | gof2hd | `Classes/Game/StarMap.cpp` |
|  62 | `0x000cba2c` | 24/28 | gof2hd | `Classes/Game/RecordHandler.cpp` |
|  63 | `0x000d03e4` | 3/6 | gof2hd | `Classes/Game/GameInit.cpp` |
|  64 | `0x000d0d18` | 14/14 | gof2hd | `Classes/Game/Sprite.cpp` |
|  65 | `0x000d1028` | 43/43 | gof2hd | `Classes/Game/Layout.cpp` |
|  66 | `0x000d52d0` | 26/107 | gof2hd | `Classes/Game/ModStation.cpp` |
|  67 | `0x000dc2f8` | 29/34 | gof2hd | `Classes/Game/PlayerFighter.cpp` |
|  68 | `0x000e0090` | 43/43 | gof2hd | `Classes/Game/Item.cpp` |
|  69 | `0x000e08cc` | 11/14 | gof2hd | `Classes/Game/PlayerStatic.cpp` |
|  70 | `0x000e09c8` | 3/3 | gof2hd | `Classes/Game/NewsItem.cpp` |
|  71 | `0x000e0a44` | 14/14 | gof2hd | `Classes/Game/WantedWindow.cpp` |
|  72 | `0x000e2648` | 22/31 | gof2hd | `Classes/Game/PlayerAsteroid.cpp` |
|  73 | `0x000e31e0` | 33/173 | gof2hd | `Classes/Game/Globals.cpp` |
|  74 | `0x000e8f54` | 3/4 | gof2hd | `Classes/Game/GameResource.cpp` |
|  75 | `0x0011b710` | 7/10 | gof2hd | `Classes/Game/SystemPathFinder.cpp` |
|  76 | `0x0011bb74` | 22/23 | gof2hd | `Classes/Game/Route.cpp` |
|  77 | `0x0011c254` | 10/13 | gof2hd | `Classes/Game/PlayerStaticFar.cpp` |
|  78 | `0x0011c54c` | 11/18 | gof2hd | `Classes/Game/ImageFactory.cpp` |
|  79 | `0x0011d00c` | 5/5 | gof2hd | `Classes/Game/LensFlare.cpp` |
|  80 | `0x0011d6a8` | 19/19 | gof2hd | `Classes/Game/Standing.cpp` |
|  81 | `0x0011d9f4` | 17/21 | gof2hd | `Classes/Game/HangarList.cpp` |
|  82 | `0x0011e7a8` | 15/17 | gof2hd | `Classes/Game/MiningGame.cpp` |
|  83 | `0x0011f6dc` | 10/14 | gof2hd | `Classes/Game/BoundingVolume.cpp` |
|  84 | `0x0011f944` | 17/25 | gof2hd | `Classes/Game/FileRead.cpp` |
|  85 | `0x00121818` | 15/18 | gof2hd | `Classes/Game/PlayerStation.cpp` |
|  86 | `0x00122890` | 30/30 | gof2hd | `Classes/Game/Wanted.cpp` |
|  87 | `0x0012299c` | 28/32 | gof2hd | `Classes/Game/MenuTouchWindow.cpp` |
|  88 | `0x0012d3c8` | 5/5 | gof2hd | `Classes/Game/PendingProduct.cpp` |
|  89 | `0x0012d484` | 16/18 | gof2hd | `Classes/Game/Radar.cpp` |
|  90 | `0x00131528` | 12/18 | gof2hd | `Classes/Game/ListItemWindow.cpp` |
|  91 | `0x00133b68` | 16/25 | gof2hd | `Classes/Game/StarSystem.cpp` |
|  92 | `0x00135598` | 15/15 | gof2hd | `Classes/Game/ScrollTouchBox.cpp` |
|  93 | `0x00135adc` | 17/18 | gof2hd | `Classes/Game/LevelScript.cpp` |
|  94 | `0x00146050` | 2/2 | gof2hd | `Classes/Game/MovingStars.cpp` |
|  95 | `0x001467ec` | 21/27 | gof2hd | `Classes/Game/ChoiceWindow.cpp` |
|  96 | `0x001475cc` | 8/13 | gof2hd | `Classes/Game/BombGun.cpp` |
|  97 | `0x00147d20` | 25/32 | gof2hd | `Classes/Game/HangarWindow.cpp` |
|  98 | `0x0014ecc8` | 12/12 | gof2hd | `Classes/Game/HackingGame.cpp` |
|  99 | `0x0014fa10` | 12/13 | gof2hd | `Classes/Game/MissionsWindow.cpp` |
| 100 | `0x001511b4` | 6/6 | gof2hd | `Classes/Game/TractorBeam.cpp` |
| 101 | `0x0015158c` | 12/12 | gof2hd | `Classes/Game/RadioMessage.cpp` |
| 102 | `0x00151d28` | 8/11 | gof2hd | `Classes/Game/BoundingSphere.cpp` |
| 103 | `0x00151f20` | 22/25 | gof2hd | `Classes/Game/Gun.cpp` |
| 104 | `0x00153a7c` | 25/30 | gof2hd | `Classes/Game/PlayerFixedObject.cpp` |
| 105 | `0x00154f7c` | 10/74 | gof2hd | `Classes/Game/Radio.cpp` |
| 106 | `0x001555dc` | 26/27 | gof2hd | `Classes/Game/SolarSystem.cpp` |
| 107 | `0x00155900` | 5/5 | gof2hd | `Classes/Game/GameRecord.cpp` |
| 108 | `0x00156478` | 8/14 | gof2hd | `Classes/Game/MineGun.cpp` |
| 109 | `0x001568d0` | 22/23 | gof2hd | `Classes/Game/Achievements.cpp` |
| 110 | `0x00157040` | 19/22 | gof2hd | `Classes/Game/PlayerTurret.cpp` |
| 111 | `0x00157c54` | 8/11 | gof2hd | `Classes/Game/Waypoint.cpp` |
| 112 | `0x00157d80` | 12/13 | gof2hd | `Classes/Game/StatusWindow.cpp` |
| 113 | `0x0015a4c8` | 37/37 | gof2hd | `Classes/Game/TargetFollowCamera.cpp` |
| 114 | `0x0015b64c` | 45/49 | gof2hd | `Classes/Game/Mission.cpp` |
| 115 | `0x0015bf8c` | 6/6 | gof2hd | `Classes/Game/Trail.cpp` |
| 116 | `0x0015c380` | 5/8 | gof2hd | `Classes/Game/SentryGun.cpp` |
| 117 | `0x0015c468` | 9/10 | gof2hd | `Classes/Game/NewsTicker.cpp` |
| 118 | `0x0015e468` | 5/5 | gof2hd | `Classes/Game/Sparks.cpp` |
| 119 | `0x0015e720` | 7/10 | gof2hd | `Classes/Game/PlayerJunk.cpp` |
| 120 | `0x0015e8f8` | 10/14 | gof2hd | `Classes/Game/RocketGun.cpp` |
| 121 | `0x0015f59c` | 6/6 | gof2hd | `Classes/Game/MarqueeImage.cpp` |
| 122 | `0x0015f700` | 11/15 | gof2hd | `Classes/Game/ObjectGun.cpp` |
| 123 | `0x001603ac` | 5/5 | gof2hd | `Classes/Game/SpacePoint.cpp` |
| 124 | `0x001603fc` | 38/43 | gof2hd | `Classes/Game/Hud.cpp` |
| 125 | `0x00166bfc` | 23/100 | gof2hd | `Classes/Game/DialogueWindow.cpp` |
| 126 | `0x001684f0` | 42/50 | gof2hd | `Classes/Game/TouchButton.cpp` |
| 127 | `0x00169ad0` | 23/32 | gof2hd | `Classes/Game/SpaceLounge.cpp` |
| 128 | `0x00174128` | 14/14 | gof2hd | `Classes/Game/ScrollTouchWindow.cpp` |
| 129 | `0x0017457c` | 81/85 | gof2hd | `Classes/Game/Ship.cpp` |
| 130 | `0x001757a0` | 13/22 | gof2hd | `Classes/Game/ModMainMenu.cpp` |
| 131 | `0x00175e08` | 16/17 | gof2hd | `Classes/Game/Galaxy.cpp` |
| 132 | `0x001763cc` | 14/17 | gof2hd | `Classes/Game/PlayerGasCloud.cpp` |
| 133 | `0x00176f20` | 24/25 | gof2hd | `Classes/Game/BluePrint.cpp` |
| 134 | `0x001773c4` | 46/46 | gof2hd | `Classes/Game/Agent.cpp` |
| 135 | `0x00177754` | 9/21 | gof2hd | `Classes/Game/BeamGun.cpp` |
| 136 | `0x00177b30` | 36/52 | gof2hd | `Classes/Game/MGame.cpp` |
| 137 | `0x001811f0` | 1/1 | gof2hd | `Classes/Game/MeshMerger/AMeshMerger.cpp` |
| 138 | `0x00181400` | 3/3 | gof2hd | `Classes/Game/MeshMerger/LodMeshMerger.cpp` |
| 139 | `0x001819c0` | 12/16 | gof2hd | `Classes/Game/ParticleSystems/IParticleSystem.cpp` |
| 140 | `0x00182b88` | 13/18 | gof2hd | `Classes/Game/ParticleSystems/ParticleSystemSprite.cpp` |
| 141 | `0x0018357c` | 22/32 | gof2hd | `Classes/Game/ParticleSystems/ParticleSystemManager.cpp` |
| 142 | `0x00183f4c` | 3/13 | gof2hd | `Classes/Game/ParticleSystems/ParticleSettings.cpp` |
| 143 | `0x00185490` | 20/25 | gof2hd | `Classes/Game/ParticleSystems/ParticleSystemMesh.cpp` |
| 144 | `0x001869f4` | 5/9 | gof2sd | `Classes/zlib/inflate.c` |
| 145 | `0x00187fcc` | 3/3 | gof2sd | `Classes/zlib/compress.c` |
| 146 | `0x00188074` | 5/14 | gof2sd | `Classes/zlib/deflate.c` |
| 147 | `0x001898a8` | 1/2 | gof2sd | `Classes/zlib/crc32.c` |
| 148 | `0x0018a714` | 2/3 | gof2sd | `Classes/zlib/zutil.c` |
| 149 | `0x0018a724` | 1/5 | gof2sd | `Classes/zlib/inftrees.c` |
| 150 | `0x0018aac4` | 1/1 | gof2sd | `Classes/zlib/uncompr.c` |
| 151 | `0x0018ab54` | 4/25 | gof2sd | `Classes/zlib/trees.c` |
| 152 | `0x0018bf08` | 1/1 | gof2sd | `Classes/zlib/inffast.c` |
| 153 | `0x0018c2d4` | 1/1 | gof2sd | `Classes/zlib/adler32.c` |

## Reproduction commands (summary)

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# authentic ndk-build compile flags (minus -fdata-sections)
cmake -S . -B /tmp/w3420/build-ndkflags-1 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3420/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  '-DGOF2_EXTRA_FLAGS=-DNDEBUG -fpic -fexceptions -no-canonical-prefixes -Wno-invalid-command-line-argument -Wno-unused-command-line-argument -gcc-toolchain /home/debian/GalaxyOnFire2/wt/w3420/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 -mfpu=vfpv3-d16 -mfpu=neon'
cmake --build /tmp/w3420/build-ndkflags-1 --target check -j8

# -fdata-sections A/B
cmake -S . -B /tmp/w3420/build-fdata-1 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3420/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS=-fdata-sections
cmake --build /tmp/w3420/build-fdata-1 --target check -j8

# full ndk-build link flags (relink current objects)
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1
# insert -z relro -z noexecstack --no-undefined --warn-shared-textrel --fatal-warnings
# --fix-cortex-a8 after --build-id, retarget -o, and run the resulting gold command

# actual ndk-build probe
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/tmp/w3420/ndkprobe \
  APP_BUILD_SCRIPT=/tmp/w3420/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3420/ndkprobe/jni/Application.mk V=1 -j8

# full ndk-build build of the current tree
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/tmp/w3420/ndkfull \
  APP_BUILD_SCRIPT=/tmp/w3420/ndkfull/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3420/ndkfull/jni/Application.mk -j8
```
