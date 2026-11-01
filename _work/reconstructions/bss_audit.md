# .bss size audit (w3377)

## Result

| metric | before | after | original |
|---|---:|---:|---:|
| `.bss` size | `0x69c8` (27080) | **`0x69ac` (27052)** | `0x69ac` (27052) |
| `.text` size | `0x1337c8` | `0x1337c8` | `0x1337c8` |
| `.rodata` size | `0x41b97` | `0x41b97` | `0x41b67` |
| `.ARM.exidx` | `0x6f80` | `0x6f80` | `0x6f70` |
| `.ARM.extab` | `0x2627c` | `0x2627c` | `0x26298` |
| `byte_exact` | 3907 | 3907 | — |
| `linked_exact` | 4494 | 4494 | — |
| exported objects | 843/843 | 843/843 | 843 |

The +0x1c delta was three independent local-object issues:

1. `s_processPad` in `LevelScript.cpp` was `int[8]` (0x20) but the original's
   StarSystem→Mission local chunk is 0x28 and already contains `CHAR_BRENT` (0x14) +
   `g_sw_charDef` (4), so the pad must be `int[4]` (0x10). **−0x10**.
2. `SpaceLounge_silhouetteChar` in `SpaceLounge.cpp` was `int[8]` (0x20) but
   `ImageFactory::loadChar` reads exactly five ints (race + four columns). The original
   Hud→ParticleSettings local chunk is 0x28 and contains `g_dw_playerFace` (0x14), so the
   silhouette descriptor must be `int[5]` (0x14). **−0x0c**.
3. The remaining 4-byte alignment residual came from the invented
   `levelTextAnchor::layoutPad` static living in the plain `.bss` before libzip. Moving it
   to its own `.bss.anchor` input section and ordering that section after the archive
   `.bss` restores the original tail size. **+0x04**.

Net: `0x69c8 - 0x10 - 0x0c + 0x04 = 0x69ac`.

## Method

`.bss` has no file content, so the comparison was done on symbol maps:

```sh
# exported .bss symbols (original is stripped; .dynsym is the only witness)
arm-linux-androideabi-nm -D -S --size-sort \
    ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E ' [bB] ' > /tmp/orig_dyn_bss.txt
arm-linux-androideabi-nm -D -S --size-sort \
    cmake-build-match/libgof2hdaa.so | grep -E ' [bB] ' > /tmp/our_dyn_bss.txt

# local .bss symbols: relink the same objects without the final strip step, then nm
# (see the exact link command below)
arm-linux-androideabi-nm -S --size-sort /tmp/gof2_unstripped.so | grep -E ' [bB] '

# per-object .bss chunks / padding
#   add -Map=/tmp/gof2.map to the gold link line
```

The original's exported `.bss` symbols are 302 entries and their sizes sum to `0x55fb`
in both binaries. The non-exported remainder is `0x13b1` in the original and `0x13cd`
at HEAD — exactly the +0x1c. Because the exported set is identical, the delta has to be
in file-static / anonymous-namespace objects or alignment padding.

The original `.bss` layout was reconstructed from the exported-symbol addresses and the
gaps between them. The two decisive gaps are:

| original gap | between | size | contents inferred |
|---|---|---:|---|
| `0x218388..0x2183b0` | `StarSystem::orbitPlanetIndex` → `Mission::empty` | `0x28` | `s_processPad` + `CHAR_BRENT` + `g_sw_charDef` |
| `0x2183c0..0x2183e8` | `Hud::wingmanCommand` → `ParticleSettingsRef::cur` | `0x28` | `g_dw_playerFace` + `SpaceLounge_silhouetteChar` |

Our local symbols in those regions were:

| symbol | TU | HEAD size | reconstructed original |
|---|---|---:|---:|
| `s_processPad` | `LevelScript.cpp` | `0x20` | `0x10` (4 ints) |
| `CHAR_BRENT` | `Radio.cpp` (from `CharacterImages.h`) | `0x14` | `0x14` |
| `g_sw_charDef` | `StatusWindow.cpp` | `0x04` | `0x04` |
| `g_dw_playerFace` | `DialogueWindow.cpp` | `0x14` | `0x14` |
| `SpaceLounge_silhouetteChar` | `SpaceLounge.cpp` | `0x20` | `0x14` (5 ints) |

`0x28 - 0x14 - 0x04 = 0x10` for the first region and `0x28 - 0x14 = 0x14` for the second.
`ImageFactory::loadChar(int *desc)` reads `desc[0..4]` only, confirming the silhouette
descriptor is five ints, not eight.

## Local `.bss` symbol table (final build)

Addresses are from the unstripped relink (`/tmp/gof2_final.so`). Compiler-merged
`.L_MergedGlobals` aliases are omitted; they alias other entries and are not separate
storage.

| address | size | symbol | object |
|---|---:|---|---|
| `0x002169b0` | `0x4` | `OPENSSL_armcap_P` | libcrypto (common) |
| `0x002169ec` | `0x1` | `g_speedUp` | `VirtualInput.cpp` |
| `0x002169f0` | `0x1` | `g_slowMotion` | `VirtualInput.cpp` |
| `0x002169f4` | `0x4` | `g_delayedKeyCount` | `VirtualInput.cpp` |
| `0x002169f8` | `0x1000` | `g_delayedKeys` | `VirtualInput.cpp` |
| `0x002179f8` | `0x1` | `g_simEdgesInit` | `VirtualInput.cpp` |
| `0x002179fc` | `0x1` | `g_simTouchActive` | `VirtualInput.cpp` |
| `0x00217a00` | `0x4` | `g_input.0` | `VirtualInput.cpp` |
| `0x00217a04` | `0x4` | `g_input.1` | `VirtualInput.cpp` |
| `0x00217a08` | `0x4` | `g_input.2` | `VirtualInput.cpp` |
| `0x00217a0c` | `0x4` | `g_input.3` | `VirtualInput.cpp` |
| `0x00218ab0` | `0x4` | `privacyPolicy` guard | `GameTextInit.cpp` |
| `0x00218ab4` | `0x4` | `termsOfService` guard | `GameTextInit.cpp` |
| `0x00218ab8` | `0xc` | `privacyPolicy` String | `GameTextInit.cpp` |
| `0x00218ac4` | `0xc` | `termsOfService` String | `GameTextInit.cpp` |
| `0x00218b9d` | `0x1` | `g_Engine_postEffectFlag` | `Engine.cpp` |
| `0x00218c00` | `0x8` | `gGravity[0]` | `NdkHooks.cpp` |
| `0x00218c08` | `0x8` | `gGravity[1]` | `NdkHooks.cpp` |
| `0x00218c10` | `0x8` | `gGravity[2]` | `NdkHooks.cpp` |
| `0x00218c18` | `0x8` | `gNegGravity[0]` | `NdkHooks.cpp` |
| `0x00218c20` | `0x8` | `gNegGravity[1]` | `NdkHooks.cpp` |
| `0x00218c28` | `0x8` | `gNegGravity[2]` | `NdkHooks.cpp` |
| `0x00218c98` | `0x14` | `gPirateGreetingFaceB` | `ModStation.cpp` |
| `0x0021942c` | `0x10` | `s_processPad` | `LevelScript.cpp` |
| `0x0021943c` | `0x14` | `CHAR_BRENT` | `Radio.cpp` |
| `0x00219450` | `0x4` | `g_sw_charDef` | `StatusWindow.cpp` |
| `0x00219464` | `0x14` | `g_dw_playerFace` | `DialogueWindow.cpp` |
| `0x00219478` | `0x14` | `SpaceLounge_silhouetteChar` | `SpaceLounge.cpp` |
| `0x0021d0a0` | `0x2c` | `_zip_dirent_torrent_normalize.torrenttime` | libzip |
| `0x0021d0cc` | `0x4` | `_zip_dirent_torrent_normalize.last_mod` | libzip |
| `0x0021d0d0` | `0x1c` | `m.6561` | libcrypto |
| `0x0021d0ec` | `0x20` | `m.6568` | libcrypto |
| `0x0021d10c` | `0x4` | `__cxxabiv1::key` | libc++abi |
| `0x0021d110` | `0x4` | `__cxxabiv1::flag` | libc++abi |
| `0x0021d114` | `0x4` | `__cxxabiv1::guard_mut` | libc++abi |
| `0x0021d118` | `0x4` | `__cxxabiv1::guard_cv` | libc++abi |
| `0x0021d120` | `0x200` | `fallback_malloc::heap` | libc++abi |
| `0x0021d320` | `0x4` | `fallback_malloc::heap_mutex` | libc++abi |
| `0x0021d324` | `0x4` | `fallback_malloc::freelist` | libc++abi |
| `0x0021d328` | `0x4` | `levelTextAnchor::layoutPad` | `AbyssEngine.cpp` (`.bss.anchor`) |

## Ranked causes

1. **`s_processPad[8]` → `[4]`** (`src/game/world/LevelScript.cpp`). The array is only
   written at indices 0..3; the 8-element size was an over-allocation in the layout
   anchor. The original StarSystem→Mission gap is `0x28`, and `CHAR_BRENT` + `g_sw_charDef`
   already account for `0x18`. Saving: **0x10**.
2. **`SpaceLounge_silhouetteChar[8]` → `[5]`** (`src/game/menu/SpaceLounge.cpp`).
   `ImageFactory::loadChar` reads five ints. The original Hud→ParticleSettings gap is
   `0x28`, and `g_dw_playerFace` accounts for `0x14`. Saving: **0x0c**.
3. **`levelTextAnchor::layoutPad` section placement** (`src/engine/core/AbyssEngine.cpp`,
   `tools/gen_section_order.py`). After the two size fixes the raw local sum was correct,
   but the 4-byte anchor sat before libzip and changed the libzip alignment padding
   (`0x08` instead of the original `0x00`), leaving `.bss` at `0x69a8`. Placing the pad in
   `.bss.anchor` and ordering it after the archive `.bss` restores `0x04`.

The remaining local-size mismatches that are not part of this delta (GameTextInit is
`0x0c` smaller than the original PaintCanvas gap, NdkHooks is `0x08` smaller than the
original gap, and the tail padding differs because our Blowfish/common chunks are at the
front rather than the original's tail) cancel against each other in the total and are
left for a future `.bss` layout pass.

## Exact commands

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target link_so -j8
llvm-readobj -S cmake-build-match/libgof2hdaa.so | grep -A8 'Name: .bss'

# unstripped relink for local symbols / map
cd cmake-build-match
ld.gold -shared -soname libgof2hdaa.so --build-id -z now \
  --unresolved-symbols=ignore-all --exclude-libs=libgcc.a \
  --section-ordering-file=section_order.txt -Tdata 0x20cb80 \
  -Map=/tmp/gof2.map -o /tmp/gof2.so \
  crtbegin_so_renamed.o -L "$NDK/platforms/android-16/arch-arm/usr/lib" \
  @exidx_markers/link_order.rsp \
  --whole-archive exidx_markers/archives/libz_gof2.a \
                 exidx_markers/archives/libzip_gof2.a --no-whole-archive \
  exidx_markers/archives/libcrypto_gof2.a \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  ../_work/bins/apk_libs/libfmodex.so ../_work/bins/apk_libs/libfmodevent.so \
  exidx_markers/archives/libc++abi.a -l m \
  exidx_markers/archives/libandroid_support.a \
  exidx_markers/archives/libgcc.a -l dl -l c \
  "$NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o"
arm-linux-androideabi-nm -S --size-sort /tmp/gof2.so | grep -E ' [bB] '

# gate
cmake --build cmake-build-match --target check -j8
```

## Measured effects

```
before: .bss 0x69c8  .text 0x1337c8  .rodata 0x41b97  byte_exact 3907  linked_exact 4494
after:  .bss 0x69ac  .text 0x1337c8  .rodata 0x41b97  byte_exact 3907  linked_exact 4494
```

`check` result after the change:

```
verify: byte 3907  linked 4494  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
