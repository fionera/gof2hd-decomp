# Per-TU `-fno-exceptions` / `-fno-rtti` archaeology (w5204)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5204`,
branch `wave104-w5204`, base commit `8c5fd3646`. Scratch: `/dev/shm/w5204`.
No source, build, tool, baseline or allowlist change was landed. **No strict improvement
exists**; this commit contains only this note.

## TL;DR

* Baseline `check` is green: **byte_exact 4500/4524, linked_exact 4500/4524,
  size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0**.
* I measured **5 global variants** and **192 app TUs x 5 per-TU variants = 960 per-TU
  full-link cells** (plus 192 baseline cells = 1152 cells total). Every cell was linked
  and compared against the original `.so`.
* **No cell improves any gate count.** `byte_exact` never exceeds 4500; `linked_exact`
  never exceeds 4500; `size_exact` never exceeds 4524; `data_exact` never exceeds 843.
  **No target function becomes exact in any cell** (`gained == 0` for all 1152 cells).
* Global `-fno-exceptions`: byte 4500 -> **1344**, linked 4500 -> **3757**,
  `.ARM.extab` 156,312 -> **30,732 B**. Global `-fno-rtti`: byte 4500 -> **1701**,
  data 843 -> **593/673**, **170 exported typeinfo symbols missing**.
* Global `-fno-exceptions -fno-rtti`: byte **1344**, linked **3757**, data **593/673**,
  170 missing.
* Global `-fexceptions` and `-frtti` are **byte-identical to the baseline except
  `.note.gnu.build-id`** (20 bytes): they are neutral and cannot be distinguished from
  clang's defaults.
* The original's EH/RTTI fingerprints are exactly the baseline's: 3566 `.ARM.exidx`
  entries (272 CANTUNWIND, 856 inline, 2438 extab), `.ARM.extab` 156,312 B, 176 `_ZTI`
  + 176 `_ZTS` + 99 `_ZTV` exports. `-fno-exceptions` shrinks `.ARM.extab` to 30,732 B;
  `-fno-rtti` removes 85 `_ZTI`/`_ZTS` pairs (170 symbols).
* **Conclusion:** the original used exceptions for every TU whose EH records are
  observable (148 TUs; their per-TU `.ARM.extab` shrink sums to exactly the global
  125,580 B), and RTTI for every TU whose typeinfo/vtables are observable (79 TUs).
  Per-TU `-fno-exceptions` is unobservable on the other 42 TUs; per-TU `-fno-rtti` is
  unobservable on the other 113 TUs. Neither is ever a gate improvement.

## 1. Baseline

The worktree is nested, so the original `.so` path must be passed explicitly (AGENTS.md):

```sh
cd /home/debian/GalaxyOnFire2/wt/w5204
export TMPDIR=/dev/shm/w5204/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Result:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`cmake-build-match/verify/report.json` summary: `byte_exact 4500`, `linked_exact 4500`,
`size` 4524/4524, `stub_zero_size 0`, `missing 0`, `extra 0`.

Hashes and section sizes (from `tools/verify/elf.py`; `.bss` size is the section-header
size, not the file-backed byte count):

| artifact | sha256 |
|---|---|
| original `_work/bins/android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` |
| baseline `cmake-build-match/libgof2hdaa.so` | `8346bbd8bf5bfaae6662f9b9b5030cb1458bb54564eb0d251748d013b87849c4` |

| section | original size | baseline size | baseline differing bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,259,464 | 100,113 |
| `.ARM.exidx` | 28,528 | 28,528 | 829 |
| `.ARM.extab` | 156,312 | 156,312 | 108,295 |
| `.rodata` | 269,159 | 269,159 | 0 |
| `.data.rel.ro` | 12,264 | 12,264 | 0 |
| `.data` | 10,604 | 10,604 | 0 |
| `.bss` | 27,052 | 27,052 | 0 (NOBITS) |

Note: `docs/VALIDATION.md` / `docs/IDENTICALITY.md` quote `.text` differing bytes
102,777 and `Generator::createMission` 77.5 %. This checked-out baseline is 100,113
differing `.text` bytes and `report.json` shows `Generator::createMission` 64.4 %.
All numbers in this note are from this worktree's actual baseline, not the docs.

## 2. Harness: per-TU `COMPILE_FLAGS`

The per-TU flag is applied with the **`COMPILE_FLAGS` source-file property** (not
`COMPILE_OPTIONS`) through a scratch `CMAKE_PROJECT_INCLUDE` file, so no repo file is
edited:

```cmake
# /dev/shm/w5204/project_include.cmake
file(STRINGS "${CMAKE_SOURCE_DIR}/tools/source_order.txt" _harness_srcs)
list(TRANSFORM _harness_srcs STRIP)
list(FILTER _harness_srcs INCLUDE REGEX "^src/")
if(DEFINED GOF2_TU_FLAGS AND NOT GOF2_TU_FLAGS STREQUAL "")
  if(DEFINED GOF2_TU_FLAG_SOURCES AND NOT GOF2_TU_FLAG_SOURCES STREQUAL "")
    foreach(_s IN LISTS GOF2_TU_FLAG_SOURCES)
      set_source_files_properties("${CMAKE_SOURCE_DIR}/${_s}"
        PROPERTIES COMPILE_FLAGS "${GOF2_TU_FLAGS}")
    endforeach()
  else()
    set_source_files_properties(${_harness_srcs}
      PROPERTIES COMPILE_FLAGS "${GOF2_TU_FLAGS}")
  endif()
endif()
```

A global variant is built with:

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w5204 -B /dev/shm/w5204/build-noexc -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/home/debian/GalaxyOnFire2/wt/w5204/cmake/toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DCMAKE_PROJECT_INCLUDE=/dev/shm/w5204/project_include.cmake \
  -DGOF2_TU_FLAGS='-fno-exceptions'
cmake --build /dev/shm/w5204/build-noexc --target link_so -j6
```

The per-TU object matrix used the exact command from
`cmake-build-match/compile_commands.json` with the variant flags appended after the
canonical flags and `-o` redirected to `/dev/shm/w5204/obj/<tu>.<variant>.o`:

```sh
python3 - <<'PY'
import json, shlex, subprocess
cc = json.load(open('cmake-build-match/compile_commands.json'))
e = [x for x in cc if x['file'].endswith('/Layout.cpp')][0]
cmd = shlex.split(e['command'])
cmd[cmd.index('-o') + 1] = '/dev/shm/w5204/obj/Layout.noexc.o'
cmd += ['-fno-exceptions']
subprocess.run(cmd, cwd='cmake-build-match', check=True)
PY
```

This is equivalent to `COMPILE_FLAGS`; I verified section-for-section identity on two TUs:

* `Layout.cpp` with `-fno-exceptions`: 141/141 `.text.*`/`.ARM.extab*`/`.ARM.exidx*`/
  `.rodata*`/`.data*` sections identical between the `COMPILE_FLAGS` build and the
  command-line object.
* `MGame.cpp` with `-fno-rtti`: 132/132 sections identical.

Per-TU full-link cells were produced by copying the canonical build directory to a
scratch directory, replacing exactly one object with its variant object, and running
the canonical gold link command:

```sh
cp -a cmake-build-match /dev/shm/w5204/pertu0
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1 \
  | sed 's|/home/debian/GalaxyOnFire2/wt/w5204/cmake-build-match|/dev/shm/w5204/pertu0|g' \
  > /dev/shm/w5204/pertu0_link.sh
cp /dev/shm/w5204/obj/Layout.noexc.o \
   /dev/shm/w5204/pertu0/CMakeFiles/gof2.dir/src/game/ui/Layout.cpp.obj
bash /dev/shm/w5204/pertu0_link.sh
```

The link is ~0.5 s; the canonical baseline object reproduces the canonical `.so`
byte-for-byte (`sha256 8346bbd8...`). Six worker directories were used, one per
thread, with a lock per directory.

Gate counts for per-TU cells were computed with a fast counter that compares every
common `.dynsym` function's size and raw bytes (`tools/verify/elf.py`), plus
`tools/datadiff.py` for data objects. It reproduces the official verifier on:

| build | fast byte | official byte | fast size | official linked |
|---|---:|---:|---:|---:|
| baseline | 4500 | 4500 | 4524 | 4500 |
| per-TU `Layout.cpp -fno-exceptions` | 1369 | 1369 | 4509 | 4485 |
| per-TU `MGame.cpp -fno-rtti` | 2929 | 2929 | 4524 | 4500 |

Official commands for the two per-TU spot checks:

```sh
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5204/verify-layout-noexc \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5204/pertu-layout-noexc.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
# byte 1369 linked 4485 avg 99.78

python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5204/verify-mgame-nortti \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5204/pertu-mgame-nortti.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
# byte 2929 linked 4500 avg 99.9
```

## 3. Global variant results

All five global builds compile every app TU with the variant flag. `-fexceptions` and
`-frtti` differ from the baseline only in `.note.gnu.build-id` (20 bytes; different
build directory changes the build-id input). `-fno-exceptions` and `-fno-rtti` change
real code/data.

| variant | byte | linked | size | data | imports | symbol parity | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` | `.data.rel.ro` | `.data` |
|---|---:|---:|---:|---:|---:|---|---:|---:|---:|---:|---:|---:|
| baseline | 4500 | 4500 | 4524/4524 | 843/843 | 207 | 5370/5370 | 1,259,464 | 28,528 | 156,312 | 269,159 | 12,264 | 10,604 |
| `-fno-exceptions` | **1344** | **3757** | **3772/4524** | 843/843 | 207 | 5370/5370 | 1,168,484 | 28,520 | **30,732** | 269,143 | 12,264 | 10,604 |
| `-fno-rtti` | **1701** | **4500** | 4524/4524 | **593/673** | 207 | **5200/5370 (170 missing)** | 1,259,464 | 28,528 | 156,312 | 266,318 | 11,292 | 10,604 |
| `-fno-exceptions -fno-rtti` | **1344** | **3757** | **3772/4524** | **593/673** | 207 | **5200/5370 (170 missing)** | 1,168,484 | 28,520 | **30,732** | 266,318 | 11,292 | 10,604 |
| `-fexceptions` | 4500 | 4500 | 4524/4524 | 843/843 | 207 | 5370/5370 | 1,259,464 | 28,528 | 156,312 | 269,159 | 12,264 | 10,604 |
| `-frtti` | 4500 | 4500 | 4524/4524 | 843/843 | 207 | 5370/5370 | 1,259,464 | 28,528 | 156,312 | 269,159 | 12,264 | 10,604 |

The global `-fno-exceptions` build's `.text` is 90,980 B smaller and its
`size_exact` is 3772/4524; the official `linked_exact` is 3757, not 4500.

Global `.so` sha256:

| variant | sha256 |
|---|---|
| baseline | `8346bbd8bf5bfaae6662f9b9b5030cb1458bb54564eb0d251748d013b87849c4` |
| `-fno-exceptions` | `3693be0b97a90f4d4b7ca75ea4a2868e770c2312e11b870bd593f5f8062abd42` |
| `-fno-rtti` | `b12842577995d9fb51e60288092cedbafc699c72ba442b87e0157cab16598d9c` |
| `-fno-exceptions -fno-rtti` | `c690ab86eac60ea25a6a4e28f69c268985ea894e996d366dd18704c442d6aad6` |
| `-fexceptions` | `1db31eddcf4b39fed03f825e114676a33b0082cc9391aa21e2408566348d176d` |
| `-frtti` | `530b2e752b62186f62d466ebcfdce45dd782c4e94b301915d089f29cd824e8f1` |

Global differing bytes vs the original:

| variant | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` | `.data.rel.ro` | `.data` |
|---|---:|---:|---:|---:|---:|---:|
| `-fno-exceptions` | 1,223,236 | 19,356 | 151,844 | 3,849 | 3,069 | 224 |
| `-fno-rtti` | 191,159 | 2,101 | 109,998 | 125,352 | 5,501 | 149 |
| `-fno-exceptions -fno-rtti` | 1,223,227 | 19,435 | 151,899 | 126,241 | 5,709 | 224 |
| `-fexceptions` | 100,113 | 829 | 108,295 | 0 | 0 | 0 |
| `-frtti` | 100,113 | 829 | 108,295 | 0 | 0 | 0 |

EH entry counts (parsed from `.ARM.exidx`; ARM EHABI prel31 targets):

| build | entries | CANTUNWIND | inline | extab |
|---|---:|---:|---:|---:|
| original | 3566 | 272 | 856 | 2438 |
| baseline | 3566 | 272 | 856 | 2438 |
| global `-fno-exceptions` | 3565 | 272 | 855 | 2438 |
| global `-fno-rtti` | 3566 | 272 | 856 | 2438 |
| global `-fexceptions` | 3566 | 272 | 856 | 2438 |
| global `-frtti` | 3566 | 272 | 856 | 2438 |

RTTI object counts (exported `.dynsym` objects):

| build | `_ZTI` | `_ZTS` | `_ZTV` | total objects |
|---|---:|---:|---:|---:|
| original | 176 | 176 | 99 | 843 |
| baseline | 176 | 176 | 99 | 843 |
| global `-fno-exceptions` | 176 | 176 | 99 | 843 |
| global `-fno-rtti` | **91** | **91** | 99 | 673 |
| global `-fexceptions` | 176 | 176 | 99 | 843 |
| global `-frtti` | 176 | 176 | 99 | 843 |

`-fno-rtti` keeps all 99 vtables but removes 85 `_ZTI`/`_ZTS` pairs (170 symbols),
shrinks `.rodata` by 2,841 B and `.data.rel.ro` by 972 B. The original exports all
176/176/99, so `-fno-rtti` is disproven wherever those objects are owned.

## 4. Per-TU matrix: all 192 app TUs

Every one of the 192 TUs was compiled with each of the five variants and linked into a
full `.so` with the other 191 TUs at baseline. Summary of the 192 cells per variant:

| variant | TUs with any object-section change | neutral TUs | byte min / max / mean | size min / max | data min / max | `.ARM.extab` min / max | `.ARM.exidx` min / max | total target gains |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| baseline | 0 | 192 | 4500 / 4500 / 4500 | 4524 / 4524 | 843 / 843 | 156,312 / 156,312 | 28,528 / 28,528 | 0 |
| `-fno-exceptions` | 150 | 42 | **1356 / 4500 / 3000.8** | 4499 / 4524 | 843 / 843 | **88,100 / 156,312** | **28,520 / 28,528** | **0** |
| `-fno-rtti` | 79 | 113 | **1734 / 4500 / 3425.4** | 4524 / 4524 | **835 / 843** | 156,312 / 156,312 | 28,528 / 28,528 | **0** |
| `-fno-exceptions -fno-rtti` | 155 | 37 | **1356 / 4500 / 2393.8** | 4499 / 4524 | **835 / 843** | **88,100 / 156,312** | **28,520 / 28,528** | **0** |
| `-fexceptions` | 0 | 192 | 4500 / 4500 / 4500 | 4524 / 4524 | 843 / 843 | 156,312 / 156,312 | 28,528 / 28,528 | 0 |
| `-frtti` | 0 | 192 | 4500 / 4500 / 4500 | 4524 / 4524 | 843 / 843 | 156,312 / 156,312 | 28,528 / 28,528 | 0 |

Key per-TU observations:

* `-fno-exceptions` changes `.ARM.extab` for **148 TUs**; the shrink per TU ranges from
  12 B (`PlayerStaticFar.cpp`) to 68,212 B (`GameResource.cpp`). **The sum of the 148
  per-TU `.ARM.extab` shrinks is exactly 125,580 B**, equal to the global
  `156,312 - 30,732` shrink. The other 42 TUs contribute 0.
* `-fno-exceptions` changes the `.ARM.exidx` **size** for only one TU:
  `IParticleSystem.cpp` 28,528 -> 28,520 (one entry removed). All other TUs keep
  28,528 B.
* `-fno-rtti` changes data sections for **79 TUs**. Per-TU `data_exact` drops by 2-6
  objects (max 6, `AEFile.cpp` 843 -> 835); the 79 TUs lose 166 data objects when
  applied individually. The global `-fno-rtti` build loses 170 exported typeinfo
  symbols (85 `_ZTI` + 85 `_ZTS`), so the global build is the authoritative count.
* `-fno-rtti` never changes `.text` size or `.ARM.extab` size; it changes `.rodata`
  and `.data.rel.ro` content (typeinfo/vtable RTTI slots).
* `-fno-exceptions -fno-rtti` is the union of the two effects; it is never better than
  either alone.
* `-fexceptions` and `-frtti` are neutral for all 192 TUs (no object-section change).

Neutral TU lists (per-TU flag has no observable effect; these are the only TUs where
the original could have used the flag without leaving a trace):

`-fno-exceptions` neutral (42):
`AERandom.cpp`, `Array.cpp`, `ArrayInstantiations.cpp`, `IApplicationModule.cpp`,
`NFC.cpp`, `BlowfishSubkeyFile.c`, `VirtualInput.cpp`, `AEMath.cpp`, `BoundingAAB.cpp`,
`BoundingSphere.cpp`, `EaseInOut.cpp`, `Matrix.cpp`, `Quaternion.cpp`, `Vector.cpp`,
`AEGeometry.cpp`, `AMeshMerger.cpp`, `Camera.cpp`, `Curve.cpp`, `Image2D.cpp`,
`ImageFont.cpp`, `ImagePart.cpp`, `LensFlare.cpp`, `MarqueeImage.cpp`, `Material.cpp`,
`Sparks.cpp`, `Sprite.cpp`, `SpriteSystem.cpp`, `Trail.cpp`, `DrawFBOShader.cpp`,
`HackingGame.cpp`, `Achievements.cpp`, `TargetFollowCamera.cpp`, `TouchSlider.cpp`,
`SentryGun.cpp`, `SpriteGun.cpp`, `NewsItem.cpp`, `SpacePoint.cpp`, `Standing.cpp`,
`BackButton.c`, `GOF2HD2012.c`, `NdkAutosave.cpp`, `main.c`.

`-fno-rtti` neutral (113): the complement of the 79 TUs that change data sections.
The 79 changed TUs are exactly the TUs that own exported `_ZTI`/`_ZTS`/`_ZTV` objects
or their RTTI-bearing vtables (e.g. `FileInterfaceAndroid.cpp`, `AEFile.cpp`,
`String.cpp`, `AESoundRessource.cpp`, all shader TUs, `MGame.cpp`, `ModStation.cpp`,
`RocketGun.cpp`).

## 5. Target functions (the 24 non-exact functions in 16 TUs)

The 16 TUs owning the 24 non-exact functions are:
`Globals.cpp`, `MGame.cpp`, `ModStation.cpp`, `SpaceLounge.cpp`, `Generator.cpp`,
`Status.cpp`, `PlayerEgo.cpp`, `HangarWindow.cpp`, `Hud.cpp`, `Layout.cpp`,
`MenuTouchWindow.cpp`, `Gun.cpp`, `Radar.cpp`, `RocketGun.cpp`, `Level.cpp`,
`LevelScript.cpp`.

Object-level target-section changes (`.text.<mangled>` in the variant object vs
baseline object) and linked-level size/exactness:

| target | TU | `-fno-exceptions` object | `-fno-rtti` object | `-fno-exc -fno-rtti` object | linked size under `-fno-exc` | linked bytes under `-fno-rtti` | exact after any variant? |
|---|---|---|---|---|---|---|---|
| `_ZN7GlobalsC1Ev` | `Globals.cpp` | same | same | same | 612 | same | no |
| `_ZN7GlobalsC2Ev` | `Globals.cpp` | same | same | same | 612 | same | no |
| `_ZN5MGame10OnTouchEndEiiPv` | `MGame.cpp` | 8080->7940 | same | 8080->7940 | 7940 | changed | no |
| `_ZN5MGame8OnUpdateEv` | `MGame.cpp` | 11440->10996 | same | 11440->10996 | 10996 | changed | no |
| `_ZN10ModStationC2Ev` | `ModStation.cpp` | 1008->960 | same | 1008->960 | 960 | changed | no |
| `_ZN10ModStationC1Ev` | `ModStation.cpp` | same (alias of C2) | same | same (alias) | 960 | changed | no |
| `_ZN11SpaceLounge10drawLoungeEv` | `SpaceLounge.cpp` | 1948->1880 | same | 1948->1880 | 1880 | same | no |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `Generator.cpp` | 1996->1892 | same | 1996->1892 | 1892 | same | no |
| `_ZN6Status16missionCompletedEbbx` | `Status.cpp` | same | same | same | 1432 | same | no |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `PlayerEgo.cpp` | same | same | same | 968 | same | no |
| `_ZN12HangarWindow6renderEv` | `HangarWindow.cpp` | 9700->9200 | same | 9700->9200 | 9200 | same | no |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `Hud.cpp` | 9332->9172 | same | 9332->9172 | 9172 | same | no |
| `_ZN6LayoutC1Ev` | `Layout.cpp` | same | same | same | 7072 | same | no |
| `_ZN6LayoutC2Ev` | `Layout.cpp` | same | same | same | 7072 | same | no |
| `_ZN15MenuTouchWindowC1Ei` | `MenuTouchWindow.cpp` | same (alias of C2) | same | same (alias) | 9248 | same | no |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `MenuTouchWindow.cpp` | same | same | same | 1896 | same | no |
| `_ZN15MenuTouchWindowC2Ei` | `MenuTouchWindow.cpp` | 9600->9248 | same | 9600->9248 | 9248 | same | no |
| `_ZN3Gun22calcCharacterCollisionEv` | `Gun.cpp` | same | same | same | 1984 | same | no |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | `Radar.cpp` | 9908->10404 | same | 9908->10404 | 10404 | same | no |
| `_ZN9RocketGun6updateEi` | `RocketGun.cpp` | same | same | same | 1908 | changed | no |
| `_ZN5Level11createSceneEv` | `Level.cpp` | 2252->2196 | same | 2252->2196 | 2196 | same | no |
| `_ZN5Level13createMissionEv` | `Level.cpp` | 17172->16884 | same | 17172->16884 | 16884 | same | no |
| `_ZN5Level10createShipEiiiP8Waypointbb` | `Level.cpp` | 2764->2600 | same | 2764->2600 | 2600 | same | no |
| `_ZN11LevelScript7processEi` | `LevelScript.cpp` | 57232->57172 | same | 57232->57172 | 57172 | same | no |

Notes:

* `-fno-exceptions` changes **13 unique target code bodies** (15 exported symbols
  because `ModStation` C1/C2 and `MenuTouchWindow` C1/C2 are aliases). Every changed
  target **shrinks**, so it cannot be byte-exact (the original has the baseline size).
  The 11 targets whose object section is unchanged still change in the linked `.so`
  because the rest of the TU's code shrinks and shifts call/literal-pool offsets; they
  remain non-exact.
* `-fno-rtti` changes the linked bytes of 5 targets (`MGame::OnUpdate`,
  `MGame::OnTouchEnd`, `ModStation` C1/C2, `RocketGun::update`) because their TU's
  `.rodata`/`.data.rel.ro` shrinks; none becomes exact. The other 19 targets are
  byte-identical to baseline under `-fno-rtti`.
* `-fexceptions` and `-frtti` leave all 24 targets byte-identical to baseline.
* **`gained == 0` for all 1152 cells**: no target function becomes exact under any
  variant.

Target EH records:

* All 24 targets keep the same `.ARM.exidx` kind under every variant. `Globals` C1/C2,
  `Layout` C1/C2 and `MenuTouchWindow` C1/C2 are CANTUNWIND; the other 18 are EXTAB.
* Target `.ARM.extab` record bytes: 11/24 already match the original in the baseline.
  No variant increases that count. `-fno-exceptions` makes 4 targets worse
  (`MGame::OnTouchEnd`, `MGame::OnUpdate`, `Gun::calcCharacterCollision`,
  `RocketGun::update`); `-fno-rtti`, `-fexceptions` and `-frtti` leave 11/24.

## 6. What is positively proven, disproven, and unobservable

### Positively proven (hard evidence)

1. **The original's `.ARM.extab` is the full `-fexceptions` form.** Original and
   baseline are both 156,312 B with 3566 exidx entries (272 CANT, 856 inline, 2438
   extab). Global `-fno-exceptions` is 30,732 B (3565 entries, 855 inline, 2438
   extab). The 148 TUs whose per-TU `.ARM.extab` shrinks account for exactly
   125,580 B, the entire global shrink. Therefore every TU that owns an EH record was
   compiled with exceptions enabled.
2. **The original's typeinfo set is the full `-frtti` form.** Original and baseline
   export 176 `_ZTI`, 176 `_ZTS`, 99 `_ZTV` (843 data objects). Global `-fno-rtti`
   exports 91/91/99 (673 objects, 170 symbols missing) and shrinks `.rodata` by
   2,841 B and `.data.rel.ro` by 972 B. Therefore every TU that owns an exported
   typeinfo object was compiled with RTTI enabled.
3. **Explicit `-fexceptions` / `-frtti` are neutral.** Applying either to all 192 TUs
   produces a `.so` identical to the baseline in every section except
   `.note.gnu.build-id` (20 bytes), with byte 4500, linked 4500, data 843/843, symbol
   parity 5370/5370.
4. **No strict improvement exists.** 1152 full-link cells; `byte_exact` max 4500,
   `linked_exact` max 4500, `size_exact` max 4524, `data_exact` max 843; no target
   gains an exact match; no section's differing-byte count is below baseline.

### Disproven (hard evidence)

1. **Global `-fno-exceptions`**: byte 4500 -> 1344, linked 4500 -> 3757,
   `.text` 1,259,464 -> 1,168,484, `.ARM.extab` 156,312 -> 30,732, `.ARM.exidx`
   28,528 -> 28,520. It changes 150 TUs and shrinks `.ARM.extab` for 148.
2. **Global `-fno-rtti`**: byte 4500 -> 1701, data 843 -> 593/673, 170 exported
   typeinfo symbols missing, `.rodata` 269,159 -> 266,318, `.data.rel.ro`
   12,264 -> 11,292. It changes 79 TUs.
3. **Global `-fno-exceptions -fno-rtti`**: byte 1344, linked 3757, data 593/673,
   170 missing; never better than either alone.
4. **Per-TU `-fno-exceptions` on any of the 150 non-neutral TUs**: byte_exact drops
   (range 1356-4500; mean 3000.8), `size_exact` drops (4499-4524), and no target
   improves. The only neutral cells are the 42 TUs with no observable EH code.
5. **Per-TU `-fno-rtti` on any of the 79 non-neutral TUs**: byte_exact drops
   (range 1734-4500; mean 3425.4), `data_exact` drops (835-843), and no target
   improves. The only neutral cells are the 113 TUs with no observable
   typeinfo/vtable effect.
6. **No target function becomes exact under any variant**: `gained == 0` for all
   1152 cells; `-fno-exceptions` only shrinks 13 target bodies (15 symbols), and
   `-fno-rtti` only shifts 5 target bodies via data layout.

### Unobservable / unknown

1. **Per-TU `-fno-exceptions` on the 42 neutral TUs.** Their object sections are
   identical to baseline; the original could have used the flag there without a
   trace. It cannot improve the gate either.
2. **Per-TU `-fno-rtti` on the 113 neutral TUs.** Same: identical object sections;
   the original could have used it there without a trace.
3. **Whether the original passed `-fexceptions` / `-frtti` explicitly or relied on
   clang's defaults.** Both produce identical code; the binary cannot distinguish
   them. The global build's explicit flags are neutral.
4. **The exact per-TU flag history for neutral TUs** (e.g. a C TU with
   `-fno-exceptions`, or a non-polymorphic C++ TU with `-fno-rtti`) is not
   recoverable from this binary.
5. **Why the original's `.ARM.extab` content differs from baseline in 108,295 bytes
   despite identical size** is a source/layout issue (LSDA/type-table offsets), not a
   flag issue; no tested flag reduces it.

## 7. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Original used exceptions for every TU with observable EH records | **very high** | original/base `.ARM.extab` 156,312 B vs global `-fno-exceptions` 30,732 B; 148 per-TU shrinks sum to exactly 125,580 B; original/base exidx 3566 entries (272/856/2438) identical |
| Original used RTTI for every TU with observable typeinfo/vtables | **very high** | original/base 176 `_ZTI` + 176 `_ZTS` + 99 `_ZTV`; global `-fno-rtti` 91/91/99 (170 missing); `.rodata` -2,841 B, `.data.rel.ro` -972 B |
| Explicit `-fexceptions` / `-frtti` are neutral vs defaults | **very high** | full-link section contents identical except 20-byte build-id; byte 4500/linked 4500/data 843/parity 5370 |
| Global `-fno-exceptions` is wrong | **very high** | byte 1344, linked 3757, `.text` -91, -980 B, `.ARM.extab` -125,580 B |
| Global `-fno-rtti` is wrong | **very high** | byte 1701, data 593/673, 170 missing, `.rodata` -2,841 B |
| No per-TU variant is a strict improvement | **very high** | 1152 full-link cells; max byte 4500, max linked 4500, max size 4524, max data 843; `gained == 0` everywhere |
| Per-TU `-fno-exceptions` neutral on 42 TUs | **high** | 42 TUs have byte-identical object sections under the flag; per-TU full-link cells give byte 4500, size 4524, data 843, `.ARM.extab` 156,312 |
| Per-TU `-fno-rtti` neutral on 113 TUs | **high** | 113 TUs have byte-identical object sections under the flag; per-TU full-link cells give byte 4500, size 4524, data 843 |
| No target becomes exact under any variant | **very high** | target table; `gained == 0` in all 1152 cells; 13 target bodies shrink under `-fno-exceptions`, 5 shift under `-fno-rtti` |
| Exact per-TU flag history of neutral TUs | **unobservable** | no code/data/EH difference; cannot be recovered from the binary |

## 8. Strict improvement?

**No.** There is no minimal code/build change to land. The baseline is already the
best of the tested variants:

```text
baseline:                     byte 4500  linked 4500  size 4524/4524  data 843/843
best per-TU -fno-exceptions:  byte 4500  linked 4500  size 4524/4524  data 843/843  (neutral TUs only)
best per-TU -fno-rtti:        byte 4500  linked 4500  size 4524/4524  data 843/843  (neutral TUs only)
global -fexceptions:          byte 4500  linked 4500  size 4524/4524  data 843/843
global -frtti:                byte 4500  linked 4500  size 4524/4524  data 843/843
```

No baseline update is needed. The experiments were reverted; `git status` is clean
apart from this note.

## Appendix A: all 192 app TUs, per-variant full-link counts

Each cell is `byte_exact / size_exact / data_exact / .ARM.extab_size`. Baseline is
`4500 / 4524 / 843 / 156312` for every TU. The per-TU link uses the variant object
for that TU and baseline objects for the other 191.

| TU | noexc B/S/D/E | nortti B/S/D/E | noexc+nortti B/S/D/E | exc B/S/D/E | rtti B/S/D/E |
|---|---:|---:|---:|---:|---:|
| `src/engine/audio/AESoundRessource.cpp` | 1953/4522/843/156296 | 1735/4524/838/156312 | 1499/4522/838/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/audio/FModSound.cpp` | 3999/4522/843/156296 | 4500/4524/843/156312 | 3999/4522/843/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/AERandom.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/ApplicationManager.cpp` | 1369/4516/843/155548 | 4500/4524/843/156312 | 1369/4516/843/155548 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/Array.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/ArrayInstantiations.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/GameText.cpp` | 1378/4521/843/156264 | 4500/4524/843/156312 | 1378/4521/843/156264 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/GameTextiPhone.cpp` | 1768/4521/843/155936 | 4500/4524/843/156312 | 1768/4521/843/155936 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/IApplicationModule.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/core/NFC.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/file/AEFile.cpp` | 1769/4512/843/155948 | 2904/4524/835/156312 | 1688/4512/835/155948 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/file/BlowfishSubkeyFile.c` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/file/ConfigReader.cpp` | 1591/4519/843/156012 | 4500/4524/843/156312 | 1591/4519/843/156012 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/file/FileInterfaceAndroid.cpp` | 1441/4518/843/156052 | 1734/4524/838/156312 | 1369/4518/838/156052 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/file/FileRead.cpp` | 2643/4507/843/155224 | 4500/4524/843/156312 | 2643/4507/843/155224 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/input/VirtualInput.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/AEMath.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/BoundingAAB.cpp` | 4500/4524/843/156312 | 1740/4524/840/156312 | 1740/4524/840/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/BoundingSphere.cpp` | 4500/4524/843/156312 | 1745/4524/840/156312 | 1745/4524/840/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/BoundingVolume.cpp` | 3217/4521/843/156280 | 2926/4524/840/156312 | 2438/4521/840/156280 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/EaseInOut.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/EaseInOutMatrix.cpp` | 1769/4518/843/156188 | 4500/4524/843/156312 | 1769/4518/843/156188 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/Matrix.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/Quaternion.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/Transform.cpp` | 1369/4516/843/156048 | 4500/4524/843/156312 | 1369/4516/843/156048 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/math/Vector.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/AEGeometry.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/AMeshMerger.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Camera.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Curve.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Engine.cpp` | 1378/4516/843/155712 | 4500/4524/843/156312 | 1378/4516/843/155712 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/FBOContainer.cpp` | 2028/4520/843/156280 | 4500/4524/843/156312 | 2028/4520/843/156280 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/GLES1Compat.cpp` | 1434/4522/843/156260 | 4500/4524/843/156312 | 1434/4522/843/156260 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/GameResource.cpp` | 1356/4521/843/88100 | 4500/4524/843/156312 | 1356/4521/843/88100 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/IParticleSystem.cpp` | 3108/4522/843/156272 | 2929/4524/840/156312 | 2206/4522/840/156272 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Image.cpp` | 1765/4523/843/156284 | 4500/4524/843/156312 | 1765/4523/843/156284 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Image2D.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ImageFactory.cpp` | 2593/4521/843/156252 | 4500/4524/843/156312 | 2593/4521/843/156252 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ImageFont.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ImagePart.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/LODManager.cpp` | 2377/4522/843/156296 | 4500/4524/843/156312 | 2377/4522/843/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/LensFlare.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/LodMeshMerger.cpp` | 3127/4522/843/156256 | 4500/4524/843/156312 | 3127/4522/843/156256 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/MarqueeImage.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Material.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Mesh.cpp` | 1421/4520/843/156244 | 4500/4524/843/156312 | 1421/4520/843/156244 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/MeshMerger.cpp` | 3687/4522/843/156288 | 4500/4524/843/156312 | 3687/4522/843/156288 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/MovingStars.cpp` | 3694/4522/843/156312 | 4500/4524/843/156312 | 3694/4522/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/PaintCanvas.cpp` | 1378/4505/843/155700 | 4500/4524/843/156312 | 1378/4505/843/155700 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ParticleSettings.cpp` | 1377/4521/843/155952 | 4500/4524/843/156312 | 1377/4521/843/155952 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ParticleSystemManager.cpp` | 3128/4515/843/156208 | 4500/4524/843/156312 | 3128/4515/843/156208 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ParticleSystemMesh.cpp` | 3936/4522/843/156292 | 1748/4524/840/156312 | 1704/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ParticleSystemSprite.cpp` | 3886/4522/843/156288 | 2929/4524/840/156312 | 2768/4522/840/156288 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/ShaderBaseStruct.cpp` | 2236/4523/843/156288 | 1735/4524/840/156312 | 2127/4523/840/156288 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/SimpleMeshMerger.cpp` | 3098/4522/843/156236 | 4500/4524/843/156312 | 3098/4522/843/156236 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Sparks.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Sprite.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/SpriteSystem.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/Trail.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/BloomShader.cpp` | 1620/4521/843/156220 | 1735/4524/840/156312 | 1369/4521/840/156220 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/BlurShader.cpp` | 2168/4522/843/156292 | 1735/4524/840/156312 | 1499/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/BumpMapping.cpp` | 3973/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/BumpRimCubeShader.cpp` | 3954/4522/843/156292 | 1735/4524/840/156312 | 1712/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/BumpShader.cpp` | 2184/4522/843/156292 | 1735/4524/840/156312 | 1501/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/ColorMixAdd.cpp` | 3988/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/CubeMapping.cpp` | 3963/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/CubeNormalMapping.cpp` | 3966/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/DNSShader.cpp` | 3948/4522/843/156292 | 1735/4524/840/156312 | 1712/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/DrawFBOShader.cpp` | 4500/4524/843/156312 | 1735/4524/840/156312 | 1735/4524/840/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/EnergyShield.cpp` | 4032/4522/843/156292 | 1735/4524/840/156312 | 1718/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/GenericShader.cpp` | 4030/4522/843/156292 | 1735/4524/840/156312 | 1720/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/GenericShader1.cpp` | 4033/4522/843/156292 | 1735/4524/840/156312 | 1720/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/GenericShader2.cpp` | 4020/4522/843/156292 | 1735/4524/840/156312 | 1718/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/GlowPPShader.cpp` | 1623/4521/843/156220 | 1735/4524/840/156312 | 1369/4521/840/156220 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/GlowShader.cpp` | 3985/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/GreenShader.cpp` | 4031/4522/843/156292 | 1735/4524/840/156312 | 1718/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/MaskShader.cpp` | 3976/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/NoTexShader.cpp` | 3960/4522/843/156292 | 1735/4524/840/156312 | 1712/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/NoTexVtxColorShader.cpp` | 4017/4522/843/156292 | 1735/4524/840/156312 | 1718/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/PostBWShader.cpp` | 4036/4522/843/156292 | 1735/4524/840/156312 | 1720/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/PulseShader.cpp` | 3991/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/SandboxShader.cpp` | 3979/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/SimpleRefractionShader.cpp` | 2194/4522/843/156292 | 1735/4524/840/156312 | 1501/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/SimpleShader.cpp` | 4029/4522/843/156292 | 1735/4524/840/156312 | 1719/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/SpecCubeAlphaMapping.cpp` | 3943/4522/843/156292 | 2905/4524/840/156312 | 2816/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/SpecCubeMapping.cpp` | 3994/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/TexOnlyShader.cpp` | 2181/4522/843/156292 | 1735/4524/840/156312 | 1501/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/TextureAlphaTestShader.cpp` | 4032/4522/843/156292 | 1735/4524/840/156312 | 1721/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/TextureLightShader.cpp` | 4014/4522/843/156292 | 1735/4524/840/156312 | 1718/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/TextureShader.cpp` | 3969/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/TextureVtxColorShader.cpp` | 2187/4522/843/156292 | 2909/4524/840/156312 | 1835/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/VertexColorAlphaTextureShader.cpp` | 3982/4522/843/156292 | 1735/4524/840/156312 | 1713/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/engine/render/shaders/VertexColorShader.cpp` | 3957/4522/843/156292 | 1735/4524/840/156312 | 1712/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/AutoPilotList.cpp` | 2693/4521/843/156180 | 4500/4524/843/156312 | 2693/4521/843/156180 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/BumpRimCubeShader_new.cpp` | 4026/4522/843/156292 | 2906/4524/840/156312 | 2870/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/BumpShaderCloak.cpp` | 2175/4522/843/156292 | 1735/4524/840/156312 | 1501/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/BumpShaderParticle.cpp` | 4028/4522/843/156292 | 1735/4524/840/156312 | 1718/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/BumpShaderRefract.cpp` | 3951/4522/843/156292 | 1735/4524/840/156312 | 1712/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/BumpShaderV2.cpp` | 4011/4522/843/156292 | 1735/4524/840/156312 | 1718/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/BumpShaderV3.cpp` | 2191/4522/843/156292 | 1735/4524/840/156312 | 1501/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/BumpShaderV4.cpp` | 2178/4522/843/156292 | 1735/4524/840/156312 | 1501/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/CheatCode.cpp` | 1586/4517/843/156244 | 4500/4524/843/156312 | 1586/4517/843/156244 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/CutScene.cpp` | 2052/4520/843/156144 | 4500/4524/843/156312 | 2052/4520/843/156144 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/GameInit.cpp` | 1497/4521/843/156060 | 4500/4524/843/156312 | 1497/4521/843/156060 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/Globals.cpp` | 1369/4508/843/153268 | 4500/4524/843/156312 | 1369/4508/843/153268 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/HangarList.cpp` | 1378/4516/843/155880 | 4500/4524/843/156312 | 1378/4516/843/155880 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/Radio.cpp` | 2890/4520/843/156208 | 4500/4524/843/156312 | 2890/4520/843/156208 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/RadioMessage.cpp` | 3775/4520/843/156312 | 4500/4524/843/156312 | 3775/4520/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/String.cpp` | 1852/4511/843/155864 | 1734/4524/840/156312 | 1659/4511/840/155864 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/core/TextureConference.cpp` | 2197/4522/843/156292 | 1735/4524/840/156312 | 1501/4522/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/menu/HackingGame.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/menu/MGame.cpp` | 1369/4507/843/154668 | 2929/4524/840/156312 | 1369/4507/840/154668 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/menu/MTitle.cpp` | 2385/4522/843/156296 | 1747/4524/840/156312 | 1543/4522/840/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/menu/MiningGame.cpp` | 1467/4521/843/156204 | 4500/4524/843/156312 | 1467/4521/843/156204 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/menu/ModMainMenu.cpp` | 3057/4521/843/156260 | 1748/4524/840/156312 | 1395/4521/840/156260 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/menu/ModStation.cpp` | 1484/4508/843/154396 | 1742/4524/840/156312 | 1484/4508/840/154396 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/menu/SpaceLounge.cpp` | 2143/4512/843/150528 | 4500/4524/843/156312 | 2143/4512/843/150528 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/Achievements.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/BluePrint.cpp` | 3055/4521/843/156248 | 4500/4524/843/156312 | 3055/4521/843/156248 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/Explosion.cpp` | 2273/4519/843/156068 | 4500/4524/843/156312 | 2273/4519/843/156068 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/GameRecord.cpp` | 2826/4521/843/156164 | 4500/4524/843/156312 | 2826/4521/843/156164 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/Generator.cpp` | 2026/4518/843/155932 | 4500/4524/843/156312 | 2026/4518/843/155932 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/Item.cpp` | 2500/4520/843/156208 | 4500/4524/843/156312 | 2500/4520/843/156208 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/Mission.cpp` | 1369/4514/843/156088 | 1750/4524/840/156312 | 1369/4514/840/156088 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/Objective.cpp` | 2041/4520/843/156260 | 4500/4524/843/156312 | 2041/4520/843/156260 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/PendingProduct.cpp` | 2767/4520/843/156268 | 4500/4524/843/156312 | 2767/4520/843/156268 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/RecordHandler.cpp` | 1369/4499/843/153396 | 4500/4524/843/156312 | 1369/4499/843/153396 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/mission/Status.cpp` | 1498/4508/843/153948 | 4500/4524/843/156312 | 1498/4508/843/153948 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/Agent.cpp` | 1378/4520/843/156232 | 4500/4524/843/156312 | 1378/4520/843/156232 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/KIPlayer.cpp` | 2192/4518/843/156200 | 1740/4524/840/156312 | 1369/4518/840/156200 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/Player.cpp` | 2121/4514/843/156144 | 4500/4524/843/156312 | 2121/4514/843/156144 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerAsteroid.cpp` | 2530/4521/843/156232 | 1743/4524/840/156312 | 1369/4521/840/156232 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerCreature.cpp` | 3170/4522/843/156296 | 1743/4524/840/156312 | 1624/4522/840/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerEgo.cpp` | 1369/4510/843/155852 | 4500/4524/843/156312 | 1369/4510/843/155852 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerFighter.cpp` | 2597/4516/843/156036 | 1742/4524/840/156312 | 1482/4516/840/156036 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerFixedObject.cpp` | 2805/4517/843/156132 | 1745/4524/840/156312 | 1369/4517/840/156132 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerGasCloud.cpp` | 1369/4519/843/156164 | 1748/4524/840/156312 | 1369/4519/840/156164 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerJumpgate.cpp` | 2181/4522/843/156264 | 1740/4524/840/156312 | 1369/4522/840/156264 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerJunk.cpp` | 3592/4523/843/156292 | 1746/4524/840/156312 | 1667/4523/840/156292 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerStatic.cpp` | 3060/4522/843/156296 | 1743/4524/840/156312 | 1614/4522/840/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerStaticFar.cpp` | 3461/4522/843/156300 | 1743/4524/840/156312 | 1639/4522/840/156300 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerStation.cpp` | 1369/4522/843/155572 | 1745/4524/840/156312 | 1369/4522/840/155572 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerTurret.cpp` | 2844/4521/843/156136 | 1746/4524/840/156312 | 1369/4521/840/156136 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/PlayerWormHole.cpp` | 2645/4522/843/156296 | 1740/4524/840/156312 | 1576/4522/840/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/Ship.cpp` | 3000/4515/843/156180 | 4500/4524/843/156312 | 3000/4515/843/156180 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ship/TargetFollowCamera.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/ChoiceWindow.cpp` | 2741/4518/843/156084 | 4500/4524/843/156312 | 2741/4518/843/156084 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/DialogueWindow.cpp` | 1369/4513/843/155668 | 4500/4524/843/156312 | 1369/4513/843/155668 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/HangarWindow.cpp` | 2058/4512/843/154052 | 4500/4524/843/156312 | 2058/4512/843/154052 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/Hud.cpp` | 2121/4510/843/154352 | 4500/4524/843/156312 | 2121/4510/843/154352 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/Layout.cpp` | 1369/4509/843/155772 | 4500/4524/843/156312 | 1369/4509/843/155772 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/ListItem.cpp` | 2208/4510/843/156180 | 4500/4524/843/156312 | 2208/4510/843/156180 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/ListItemWindow.cpp` | 2119/4518/843/155224 | 4500/4524/843/156312 | 2119/4518/843/155224 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/MenuTouchWindow.cpp` | 1369/4511/843/153888 | 4500/4524/843/156312 | 1369/4511/843/153888 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/MissionsWindow.cpp` | 1369/4518/843/155748 | 4500/4524/843/156312 | 1369/4518/843/155748 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/ScrollTouchBox.cpp` | 2720/4520/843/156240 | 4500/4524/843/156312 | 2720/4520/843/156240 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/ScrollTouchWindow.cpp` | 2990/4516/843/156196 | 4500/4524/843/156312 | 2990/4516/843/156196 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/StatusWindow.cpp` | 1369/4517/843/155220 | 4500/4524/843/156312 | 1369/4517/843/155220 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/TouchButton.cpp` | 1369/4509/843/156048 | 4500/4524/843/156312 | 1369/4509/843/156048 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/TouchSlider.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/ui/WantedWindow.cpp` | 1369/4516/843/155596 | 4500/4524/843/156312 | 1369/4516/843/155596 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/BeamGun.cpp` | 3820/4522/843/156280 | 1748/4524/840/156312 | 1691/4522/840/156280 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/BombGun.cpp` | 2752/4522/843/156216 | 1745/4524/840/156312 | 1369/4522/840/156216 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/Gun.cpp` | 1369/4520/843/156228 | 4500/4524/843/156312 | 1369/4520/843/156228 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/MineGun.cpp` | 2905/4522/843/156248 | 1749/4524/840/156312 | 1443/4522/840/156248 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/ObjectGun.cpp` | 2919/4521/843/156224 | 2930/4524/840/156312 | 2120/4521/840/156224 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/Radar.cpp` | 1369/4519/843/155652 | 4500/4524/843/156312 | 1369/4519/843/155652 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/RepairBeam.cpp` | 2390/4522/843/156264 | 4500/4524/843/156312 | 2390/4522/843/156264 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/RocketGun.cpp` | 1378/4521/843/156208 | 2932/4524/840/156312 | 1369/4521/840/156208 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/SentryGun.cpp` | 4500/4524/843/156312 | 1750/4524/840/156312 | 1750/4524/840/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/SpriteGun.cpp` | 4500/4524/843/156312 | 1748/4524/840/156312 | 1748/4524/840/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/weapons/TractorBeam.cpp` | 3395/4522/843/156296 | 4500/4524/843/156312 | 3395/4522/843/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/Galaxy.cpp` | 3063/4519/843/156264 | 4500/4524/843/156312 | 3063/4519/843/156264 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/Level.cpp` | 1491/4500/843/145860 | 4500/4524/843/156312 | 1491/4500/843/145860 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/LevelScript.cpp` | 2049/4521/843/155712 | 4500/4524/843/156312 | 2049/4521/843/155712 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/NewsItem.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/NewsTicker.cpp` | 2168/4521/843/154992 | 4500/4524/843/156312 | 2168/4521/843/154992 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/Route.cpp` | 1381/4517/843/156172 | 4500/4524/843/156312 | 1381/4517/843/156172 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/SolarSystem.cpp` | 3451/4522/843/156288 | 4500/4524/843/156312 | 3451/4522/843/156288 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/SpacePoint.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/Standing.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/StarMap.cpp` | 1879/4515/843/155640 | 4500/4524/843/156312 | 1879/4515/843/155640 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/StarSystem.cpp` | 1369/4522/843/156088 | 4500/4524/843/156312 | 1369/4522/843/156088 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/Station.cpp` | 2242/4513/843/156164 | 4500/4524/843/156312 | 2242/4513/843/156164 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/SystemPathFinder.cpp` | 2663/4519/843/156204 | 4500/4524/843/156312 | 2663/4519/843/156204 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/Wanted.cpp` | 3255/4522/843/156296 | 4500/4524/843/156312 | 3255/4522/843/156296 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/game/world/Waypoint.cpp` | 2918/4522/843/156284 | 1750/4524/840/156312 | 1437/4522/840/156284 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/platform/android/BackButton.c` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/platform/android/GOF2HD2012.c` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/platform/android/NdkAutosave.cpp` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/platform/android/NdkHooks.cpp` | 1992/4520/843/156216 | 4500/4524/843/156312 | 1992/4520/843/156216 | 4500/4524/843/156312 | 4500/4524/843/156312 |
| `src/platform/android/main.c` | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 | 4500/4524/843/156312 |
