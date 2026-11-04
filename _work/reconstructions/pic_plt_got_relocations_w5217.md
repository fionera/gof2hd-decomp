# PIC / PLT / GOT / relocation flag audit (w5217)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5217`,
branch `wave104-w5217`, base commit `8c5fd3646`. Scratch is `/dev/shm/w5217`.
**No `src/`, build-flag, tool, baseline or allowlist change was landed.** This commit
contains only this note. The canonical build was left untouched.

## 0. Bottom line

| flag | verdict at this tree state | hard evidence |
|---|---|---|
| `-fPIC` (current) | **valid; exact** | `.got`/`.plt`/`.rel.dyn`/`.rel.plt`/`.ARM.attributes` byte-identical to the original; 0 text relocations; `GOT_use: GOT-indirect` |
| `-fpic` | **unobservable vs `-fPIC`** | whole linked `.so` identical except the 20-byte build-id; object-level probe identical SHA-256 |
| `-fPIE` / `-fpie` | **disproven** | shared link fails: gold rejects `R_ARM_REL32`; object probe emits 38 `R_ARM_REL32` vs 30 for `-fPIC` |
| `-fno-pic` (control) | **disproven** | 7,720 text relocations; `GOT_use: direct`; `.got` 17,164 -> 14,928; gate 4,500 -> 1,377 |
| `-fno-plt` | **unobservable** | whole linked `.so` identical except build-id; clang 7.0.2 ARM emits the same code under `-fPIC` |
| `-fno-semantic-interposition` | **not available in clang 7.0.2** | `clang++: error: unknown argument: '-fno-semantic-interposition'`; no `-mllvm` equivalent either |
| `-mno-movt` | **unobservable** | whole linked `.so` identical except build-id |
| `-mno-restrict-it` | **unobservable** | whole linked `.so` identical except build-id |
| `-mno-neg-immediates` | **unobservable** | whole linked `.so` identical except build-id |
| `-mlong-calls` | **disproven** | 65,824 text relocations; `.text` 1,259,464 -> 1,643,160; `.rel.dyn` 28,744 -> 554,048; gate 4,500 -> 1,345 |
| `-mno-long-calls` | **unobservable** | whole linked `.so` identical except build-id |
| `-fno-jump-tables` | **disproven** | `.text` 1,259,464 -> 1,263,848; `.rel.dyn` 28,744 -> 28,616; `.rodata` 269,159 -> 268,615; gate 4,500 -> 1,390 |

**No strict improvement exists.** Every unobservable variant ties the baseline at
`byte_exact 4500/4524`, `linked_exact 4500/4524`, `size-exact 4524/4524`, and no variant
raises the fuzzy percentage of any of the 24 non-byte-exact target functions (max delta
`+0.0`). The current `-fPIC` spelling is therefore kept.

## 1. Baseline (canonical build)

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5217
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
cmake --build cmake-build-match --target lint -j6
```

Raw output:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`size-exact` from `cmake-build-match/verify/report.json` is **4524 / 4524**.

Baseline ELF sections vs the original (sizes and SHA-256 of the exact section bytes,
computed with a direct ELF section-header reader, not `objcopy`):

| section | original size | base size | original SHA-256 | base same? |
|---|---:|---:|---|---|
| `.got` | 17,164 | 17,164 | `87e6b167273f1d99b6f7c40b1619909275a0ab8ed249f9349da6928da2f57576` | **yes** |
| `.plt` | 44,108 | 44,108 | `31d4766e6f3d52d827c0caa2b8d81e60b8c033cc1c6694c68111693c34d06a73` | **yes** |
| `.rel.dyn` | 28,744 | 28,744 | `4f55867c15475df09d0fc73442c2446c5dda4f49c6b8eee11a5823a273194792` | **yes** |
| `.rel.plt` | 29,392 | 29,392 | `43bf7c4eb1705aa4100655a5cce6077df7447fe90bb349cbc2c6ac2b78571995` | **yes** |
| `.ARM.attributes` | 56 | 56 | `b03afea6aa56c9ca191d5385abb498e64c7ecdf8e249a80e68704050256b1bc2` | **yes** |
| `.rodata` | 269,159 | 269,159 | `73c737319958f462dfea24f59883dea97853195cbb5607f50243f417389deb31` | **yes** |
| `.data` | 10,604 | 10,604 | `bf3a87118411e0a3883bfe8db2c4e2d48f9dc38346016de7fd1409d7003398d4` | **yes** |
| `.data.rel.ro` | 12,264 | 12,264 | `34ad551db3f4b09ae3e40f5107d6666f922a7de32bd0f0900e5d5669443a9025` | **yes** |
| `.dynsym` | 89,248 | 89,248 | `5a3b59e8436c8dc5b2ef63ac5ab0196eb41cde2ed6314b354e7a9fbd37a82d6c` | **yes** |
| `.dynstr` | 189,208 | 189,208 | `a42e036792f61aaa4c07ac027495052050ab7ddacbdc5204cc2aadefb69955b7` | **yes** |
| `.hash` | 38,716 | 38,716 | `fdc6fe8d74f1d4e2882daef8057dbea632a25ee5da3dfd8df2f500911c65cb1e` | **yes** |
| `.text` | 1,259,464 | 1,259,464 | `d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed` | no (24 non-exact bodies) |

Relocation-type counts and text relocations (readelf `-rW`; text range
`0x69440..0x19cc08`):

| binary | `R_ARM_ABS32` | `R_ARM_GLOB_DAT` | `R_ARM_JUMP_SLOT` | `R_ARM_RELATIVE` | relocs in `.text` |
|---|---:|---:|---:|---:|---:|
| original | 1,817 | 614 | 3,674 | 1,162 | **0** |
| base `-fPIC` | 1,817 | 614 | 3,674 | 1,162 | **0** |

`readelf -dW` on both: `FLAGS: BIND_NOW`, `FLAGS_1: NOW`, no `TEXTREL`.
`readelf -AW` on both: `Tag_ABI_PCS_GOT_use: GOT-indirect`.

## 2. Method

A scratch toolchain wrapper includes the repo toolchain and either replaces the app
`-fPIC` token or appends the candidate flag to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
# /dev/shm/w5217/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w5217/cmake/toolchain.cmake")
if(GOF2_PIC_FLAG)
  string(REPLACE "-fPIC" "${GOF2_PIC_FLAG}" _gof2_cxx "${CMAKE_CXX_FLAGS}")
  string(REPLACE "-fPIC" "${GOF2_PIC_FLAG}" _gof2_c "${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${_gof2_cxx}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS "${_gof2_c}" CACHE STRING "" FORCE)
endif()
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Each variant was configured and built in its own scratch build directory:

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w5217 -B /dev/shm/w5217/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5217/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_PIC_FLAG='<pic>' -DGOF2_EXTRA_FLAGS='<extra>'
cmake --build /dev/shm/w5217/build-<tag> --target link_so -j6
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5217/build-<tag>/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5217/build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

`size-exact` is `sum(osize == usize)` over the verify report rows. Section sizes and
SHA-256 values come from a direct ELF section-header reader (`/dev/shm/w5217/elfsec.py`).
Text relocations are the `readelf -rW` entries whose `r_offset` lies in `.text`.
`GOT_use` is `readelf -AW`'s `Tag_ABI_PCS_GOT_use`.

The variant list was exactly:

```text
base            (no change)
fpic            -fPIC -> -fpic
fpie            -fPIC -> -fpie
fPIE            -fPIC -> -fPIE
fnopic          -fPIC -> -fno-pic   (positive control)
fnoplt          -fno-plt
fnosemint       -fno-semantic-interposition
mnomovt         -mno-movt
mnorestrictit   -mno-restrict-it
mnonegimm       -mno-neg-immediates
mlongcalls      -mlong-calls
mnolongcalls    -mno-long-calls
fnojumptables   -fno-jump-tables
```

## 3. Raw results

### 3.1 Gate counts

| tag | byte_exact | linked_exact | size-exact | avg | missing | extra | wrong_type |
|---|---:|---:|---:|---:|---:|---:|---:|
| base | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 0 | 0 |
| `-fpic` | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 0 | 0 |
| `-fno-plt` | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 0 | 0 |
| `-mno-movt` | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 0 | 0 |
| `-mno-restrict-it` | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 0 | 0 |
| `-mno-neg-immediates` | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 0 | 0 |
| `-mno-long-calls` | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 0 | 0 |
| `-fno-pic` | 1,377 | 2,956 | 3,078/4,524 | 89.65 | 0 | 0 | 0 |
| `-mlong-calls` | 1,345 | 1,664 | 1,673/4,524 | 75.45 | 0 | 0 | 0 |
| `-fno-jump-tables` | 1,390 | 4,411 | 4,436/4,524 | 99.13 | 0 | 0 | 0 |
| `-fPIE` | build failure | — | — | — | — | — | — |
| `-fpie` | build failure | — | — | — | — | — | — |
| `-fno-semantic-interposition` | build failure | — | — | — | — | — | — |

The two `-fPIE`/`-fpie` builds fail at the gold link with, for example:

```text
ld.gold: error: .../ParticleSettings.cpp.obj: requires unsupported dynamic reloc
R_ARM_REL32; recompile with -fPIC
```

The `-fno-semantic-interposition` build fails at compile time:

```text
clang++: error: unknown argument: '-fno-semantic-interposition'
```

### 3.2 Section sizes, relocation counts, text relocations, GOT attribute

| tag | `.got` | `.plt` | `.rel.dyn` | `.rel.plt` | text relocs | `GOT_use` | sections same as base |
|---|---:|---:|---:|---:|---:|---|---:|
| base | 17,164 | 44,108 | 28,744 | 29,392 | 0 | GOT-indirect | 12/12 |
| `-fpic` | 17,164 | 44,108 | 28,744 | 29,392 | 0 | GOT-indirect | 12/12 |
| `-fno-plt` | 17,164 | 44,108 | 28,744 | 29,392 | 0 | GOT-indirect | 12/12 |
| `-mno-movt` | 17,164 | 44,108 | 28,744 | 29,392 | 0 | GOT-indirect | 12/12 |
| `-mno-restrict-it` | 17,164 | 44,108 | 28,744 | 29,392 | 0 | GOT-indirect | 12/12 |
| `-mno-neg-immediates` | 17,164 | 44,108 | 28,744 | 29,392 | 0 | GOT-indirect | 12/12 |
| `-mno-long-calls` | 17,164 | 44,108 | 28,744 | 29,392 | 0 | GOT-indirect | 12/12 |
| `-fno-pic` | 14,928 | 44,324 | 85,888 | 29,536 | 7,720 | **direct** | 2/12 |
| `-mlong-calls` | 16,572 | 44,264 | 554,048 | 29,496 | 65,824 | GOT-indirect | 4/12 |
| `-fno-jump-tables` | 17,164 | 44,108 | 28,616 | 29,392 | 0 | GOT-indirect | 3/12 |

Relocation-type counts:

| tag | `R_ARM_ABS32` | `R_ARM_GLOB_DAT` | `R_ARM_JUMP_SLOT` | `R_ARM_RELATIVE` |
|---|---:|---:|---:|---:|
| base / `-fpic` / `-fno-plt` / `-mno-movt` / `-mno-restrict-it` / `-mno-neg-immediates` / `-mno-long-calls` | 1,817 | 614 | 3,674 | 1,162 |
| `-fno-pic` | 5,897 | 37 | 3,692 | 4,802 |
| `-mlong-calls` | 67,570 | 453 | 3,687 | 1,233 |
| `-fno-jump-tables` | 1,817 | 614 | 3,674 | 1,146 |

Sections that differ from base for the non-neutral variants:

* `-mlong-calls`: `.text`, `.plt`, `.got`, `.rel.dyn`, `.rel.plt`, `.data`,
  `.data.rel.ro`, `.dynsym` (`.rodata`, `.dynstr`, `.hash`, `.ARM.attributes` same).
* `-fno-jump-tables`: `.text`, `.plt`, `.got`, `.rel.dyn`, `.rel.plt`, `.rodata`,
  `.data`, `.data.rel.ro`, `.dynsym` (`.dynstr`, `.hash`, `.ARM.attributes` same).
* `-fno-pic`: `.text`, `.plt`, `.got`, `.rel.dyn`, `.rel.plt`, `.ARM.attributes`,
  `.rodata`, `.data`, `.data.rel.ro`, `.dynsym` (`.dynstr`, `.hash` same).

### 3.3 Whole-file identity of the unobservable variants

`cmp -l base.so variant.so` for each unobservable variant:

| variant | differing bytes | file offsets | location |
|---|---:|---|---|
| `-fpic` | 19 | 476..495 | `.note.gnu.build-id` descriptor |
| `-fno-plt` | 19 | 476..494 | `.note.gnu.build-id` descriptor |
| `-mno-movt` | 20 | 476..495 | `.note.gnu.build-id` descriptor |
| `-mno-restrict-it` | 20 | 476..495 | `.note.gnu.build-id` descriptor |
| `-mno-neg-immediates` | 20 | 476..495 | `.note.gnu.build-id` descriptor |
| `-mno-long-calls` | 19 | 476..495 | `.note.gnu.build-id` descriptor |

`.note.gnu.build-id` starts at file offset 460 and is 36 bytes; its 20-byte descriptor
starts at 476. Every one of the six variants is therefore byte-identical to the base
except for the GNU build-id, which is a hash of the whole (unstripped) link output and
necessarily changes on every relink.

### 3.4 Target functions

There are 24 non-byte-exact functions in the baseline. The unobservable variants
(`-fpic`, `-fno-plt`, `-mno-movt`, `-mno-restrict-it`, `-mno-neg-immediates`,
`-mno-long-calls`) move **0 of 24** target percentages. The non-neutral variants move:

| target | base | `-mlong-calls` | `-fno-jump-tables` | `-fno-pic` |
|---|---:|---:|---:|---:|
| `_ZN6LayoutC1Ev` | 47.8 | 47.8 | 47.8 | 22.6 |
| `_ZN6LayoutC2Ev` | 47.8 | 47.8 | 47.8 | 22.6 |
| `_ZN11LevelScript7processEi` | 53.4 | 23.7 | 33.0 | 34.1 |
| `_ZN6Status16missionCompletedEbbx` | 60.4 | 22.4 | 31.2 | 54.2 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 66.4 | 32.5 | 38.5 | 62.9 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 74.0 | 25.8 | 74.0 | 59.1 |
| `_ZN5MGame10OnTouchEndEiiPv` | 74.8 | 27.0 | 74.8 | 43.5 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 77.5 | 39.0 | 64.4 | 57.6 |
| `_ZN7GlobalsC1Ev` | 80.5 | 80.5 | 80.5 | 13.2 |
| `_ZN7GlobalsC2Ev` | 80.5 | 80.5 | 80.5 | 13.2 |
| `_ZN12HangarWindow6renderEv` | 84.4 | 39.0 | 83.8 | 45.7 |
| `_ZN9RocketGun6updateEi` | 86.9 | 44.0 | 86.9 | 73.5 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 87.6 | 46.4 | 87.6 | 69.2 |
| `_ZN5Level11createSceneEv` | 88.3 | 33.0 | 69.1 | 61.3 |
| `_ZN3Gun22calcCharacterCollisionEv` | 90.0 | 47.5 | 90.0 | 74.7 |
| `_ZN5Level13createMissionEv` | 92.0 | 46.7 | 62.2 | 63.3 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 92.9 | 41.8 | 89.9 | 58.7 |
| `_ZN15MenuTouchWindowC1Ei` | 95.1 | 48.6 | 95.1 | 69.3 |
| `_ZN15MenuTouchWindowC2Ei` | 95.1 | 48.6 | 95.1 | 69.3 |
| `_ZN11SpaceLounge10drawLoungeEv` | 95.5 | 50.4 | 95.5 | 61.3 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 97.7 | 50.0 | 70.2 | 69.3 |
| `_ZN10ModStationC1Ev` | 98.4 | 55.7 | 98.4 | 57.0 |
| `_ZN10ModStationC2Ev` | 98.4 | 55.7 | 98.4 | 57.0 |
| `_ZN5MGame8OnUpdateEv` | 99.9 | 44.6 | 99.9 | 51.9 |

`-mlong-calls` moves 20 of 24 (all except `Layout` C1/C2 and `Globals` C1/C2);
`-fno-jump-tables` moves 9 of 24; `-fno-pic` moves all 24.

### 3.5 Target/TU exact counts

The 4,524 compared functions map to 192 app TUs; 242 of the compared functions do not
resolve to an app object symbol (aliases/vendored symbols), so the TU counts cover the
remaining 4,282 functions. A TU is "all-exact" only if every compared function it defines
is byte-exact.

| tag | compared functions | byte-exact | TUs with compared functions | all-exact TUs | TUs with a non-exact function |
|---|---:|---:|---:|---:|---:|
| base | 4,524 | 4,500 | 192 | **176** | 16 |
| `-fpic` | 4,524 | 4,500 | 192 | **176** | 16 |
| `-fno-plt` | 4,524 | 4,500 | 192 | **176** | 16 |
| `-mno-movt` | 4,524 | 4,500 | 192 | **176** | 16 |
| `-mno-restrict-it` | 4,524 | 4,500 | 192 | **176** | 16 |
| `-mno-neg-immediates` | 4,524 | 4,500 | 192 | **176** | 16 |
| `-mno-long-calls` | 4,524 | 4,500 | 192 | **176** | 16 |
| `-fno-pic` | 4,524 | 1,377 | 192 | 1 | 191 |
| `-mlong-calls` | 4,524 | 1,345 | 192 | 1 | 191 |
| `-fno-jump-tables` | 4,524 | 1,390 | 192 | 1 | 191 |

### 3.6 Object-level `-fPIE` / `-fpie` probe

The exact `ParticleSettings.cpp` compile command from
`cmake-build-match/compile_commands.json` was re-run with only the PIC token changed:

```sh
CLANG=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CLANG -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 <PIC FLAG> -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ ... -c src/engine/render/ParticleSettings.cpp -o probe.o
```

Object-level result:

| PIC flag | object SHA-256 | `R_ARM_GOT_PREL` | `R_ARM_REL32` | `R_ARM_ABS32` |
|---|---|---:|---:|---:|
| `-fPIC` | `8d01273eeb838b44e8e6b3f78e92cc3c250bca983e6fb43b0ea426851ee85075` | 9 | 30 | 600 |
| `-fpic` | `8d01273eeb838b44e8e6b3f78e92cc3c250bca983e6fb43b0ea426851ee85075` | 9 | 30 | 600 |
| `-fPIE` | `f584a163e43cb3a867cce58238fd042e80a99aa382ffaffe6253fbb61a8f8ddb` | 1 | 38 | 598 |
| `-fpie` | `f584a163e43cb3a867cce58238fd042e80a99aa382ffaffe6253fbb61a8f8ddb` | 1 | 38 | 598 |

All four objects still carry `Tag_ABI_PCS_GOT_use: GOT-indirect`; the distinguishing
feature is the 8 extra `R_ARM_REL32` relocations and the loss of 8 `R_ARM_GOT_PREL`
relocations. Gold cannot resolve those `R_ARM_REL32` relocations in a shared link, which
is why the full `-fPIE`/`-fpie` builds fail.

## 4. Proven, disproven, unobservable

### Positively proven

1. The original is PIC with the default visibility/semantic-interposition model:
   `.ARM.attributes` says `GOT_use: GOT-indirect`, there are zero text relocations,
   and `.got`/`.plt`/`.rel.dyn`/`.rel.plt` are byte-identical to the current `-fPIC`
   build.
2. `-fPIC` and `-fpic` produce identical object code and an identical linked `.so`
   except for the build-id. The binary cannot distinguish the spelling.
3. `-fno-plt` is a codegen no-op under `-fPIC` on this target; the whole `.so` is
   identical except the build-id.
4. `-mno-movt`, `-mno-restrict-it`, `-mno-neg-immediates` and `-mno-long-calls` are
   codegen-neutral on this tree; the whole `.so` is identical except the build-id.
5. `-fPIE`/`-fpie` are not the original PIC model: they emit extra `R_ARM_REL32`
   relocations and fail the shared link.
6. `-fno-pic` is not the original model: it flips `GOT_use` to `direct`, introduces
   7,720 text relocations, and collapses the gate to 1,377/2,956.
7. `-mlong-calls` and `-fno-jump-tables` are real codegen changes and both lower the
   gate; neither is the original flag.
8. `-fno-semantic-interposition` is not accepted by clang 7.0.2, so it cannot have
   been the original build flag.

### Disproven

* `-fPIE`, `-fpie` (shared-link failure; `R_ARM_REL32`).
* `-fno-pic` (text relocations; `GOT_use: direct`; gate collapse).
* `-mlong-calls` (65,824 text relocations; `.text` +383,696 B; gate 1,345).
* `-fno-jump-tables` (`.text` +4,384 B; `.rel.dyn` -128 B; gate 1,390).

### Unobservable / still unknown

* `-fPIC` vs `-fpic`: identical codegen on this target. The exact spelling used by the
  original cannot be recovered from the binary.
* `-fno-plt`: identical codegen under `-fPIC`; cannot be excluded, but also cannot be
  detected or used to improve the gate.
* `-mno-movt`, `-mno-restrict-it`, `-mno-neg-immediates`, `-mno-long-calls`: accepted but
  codegen-neutral on this tree; cannot be excluded.
* Whether the original build command line contained any of the unobservable flags. The
  stripped `.so` carries no command-line record; only the emitted code and ELF
  attributes are observable.

## 5. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Original is PIC, GOT-indirect, no text relocations | **very high** | original `.ARM.attributes` `GOT_use: GOT-indirect`; 0 relocations in `.text`; `.got`/`.plt`/`.rel.dyn`/`.rel.plt` byte-identical to base |
| `-fPIC` and `-fpic` are indistinguishable here | **very high** | whole `.so` differs only in the 20-byte build-id; `ParticleSettings.cpp` object SHA identical under both |
| `-fPIE`/`-fpie` are not the original | **very high** | gold link failure on `R_ARM_REL32`; object probe 38 vs 30 `R_ARM_REL32`, 1 vs 9 `GOT_PREL` |
| `-fno-pic` is not the original | **very high** | 7,720 text relocations; `GOT_use: direct`; `.got` 14,928 vs 17,164; gate 1,377/2,956 |
| `-fno-plt` is codegen-neutral here | **very high** | whole `.so` identical except build-id; all relocation/GOT/PLT sections byte-identical |
| `-mno-movt`, `-mno-restrict-it`, `-mno-neg-immediates`, `-mno-long-calls` are codegen-neutral here | **very high** | whole `.so` identical except build-id for each variant |
| `-mlong-calls` is not the original | **very high** | 65,824 text relocations; `.text` 1,643,160; `.rel.dyn` 554,048; gate 1,345/1,664 |
| `-fno-jump-tables` is not the original | **very high** | `.text` 1,263,848; `.rel.dyn` 28,616; `.rodata` 268,615; gate 1,390/4,411 |
| `-fno-semantic-interposition` was not the original flag | **very high** | clang 7.0.2: `unknown argument: '-fno-semantic-interposition'`; no `-mllvm` equivalent |
| No strict improvement exists at this tree state | **high** | every neutral variant ties 4500/4500/4524; no target fuzzy rises (max delta +0.0) |

## 6. Strict improvement?

**No.** The best variants (`-fpic`, `-fno-plt`, `-mno-movt`, `-mno-restrict-it`,
`-mno-neg-immediates`, `-mno-long-calls`) are exactly gate-neutral:

```text
before: byte 4500  linked 4500  size 4524/4524
after:  byte 4500  linked 4500  size 4524/4524
```

No variant raises `byte_exact` or `linked_exact`, and no variant raises any of the 24
target fuzzy percentages. The current `-fPIC` spelling is therefore kept, and no
`src/` or build-flag change is landed. The only committed change is this note.

## 7. Reproduction artifacts

Scratch (not tracked):

```text
/dev/shm/w5217/toolchain_extra.cmake   global PIC/extra-flag wrapper
/dev/shm/w5217/run_sweep.py            variant driver
/dev/shm/w5217/analyze.py              readelf/reloc/section analyzer
/dev/shm/w5217/elfsec.py               direct ELF section-header reader
/dev/shm/w5217/results/*.json          per-variant gate + report rows
/dev/shm/w5217/build-<tag>/            per-variant build tree and linked .so
/dev/shm/w5217/tmp/objprobe/           object-level -fPIC/-fpic/-fPIE/-fpie probe
```

The canonical `cmake-build-match` build was re-run after the sweep: `RATCHET PASS` at
`byte 4500 / linked 4500`, `size 4524/4524`, `data 843/843`, `imports 207`, lint
`CLEAN`, drift 0. `report.json` was not committed.
