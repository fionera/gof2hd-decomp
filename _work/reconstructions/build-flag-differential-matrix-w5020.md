# Build-flag differential matrix and reproducibility harness (w5020, wave 100)

Status: research note, 2026-11-04. Worktree `wt/w5020`, branch `wave100-w5020`,
base `44225192c`. Scratch is `/dev/shm/w5020`. This note **independently re-derives**
the shipped `libgof2hdaa.so` build recipe from the artifacts, then measures a
differential flag matrix with the official verifier. It extends
`_work/reconstructions/build_archaeology_w4306.md` (wave 93) with evidence wave 93
did not have: a positive proof of gold's build-id algorithm, the current
`.rel.plt`/`.ARM.extab` residuals, a full 192-TU optimization-level sweep, a
vendored-library sweep, and a layout-free branch-weight-default sweep.

No codegen change was landed. The only build-file change is a default-off
`GOF2_NO_PER_FILE_WEIGHTS` option in `CMakeLists.txt` used by the harness to test
global `-mllvm` branch weights without duplicating the per-file overrides. The
canonical gate is unchanged:

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0
data_exact 843/843  imports 207  lint CLEAN  drift 0
RATCHET PASS
```

## 0. Bottom line

* The main recipe is confirmed, with new positive fingerprints:
  NDK r18b clang 7.0.2, API 16, armeabi-v7a/Thumb-2/NEON/softfp, `-Oz -g`,
  `-std=c++11 -fPIC -frtti -fstack-protector-strong -ffunction-sections
  -funwind-tables`, `-DANDROID -DNDEBUG`, `-stdlib=libc++`, single gold link
  `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`
  (no `-s`), followed by GNU `strip --strip-all`.
* **New proof of the link recipe:** gold's build-id is exactly
  `SHA1(unstripped output with the 20-byte build-id descriptor zeroed)`.
  Re-linking our objects with the canonical gold command reproduces our own
  build-id `ab4c71d770be30ff42bb5f832c346834829d6e4d` from that hash. The
  original's `c011810d...` is therefore a hash of an **unstripped** gold output;
  a gold `-s` link cannot reproduce it.
* **New per-TU result:** `-Oz` is positively distinguished from `-O2` for
  **184/192** TUs, from `-O1` for **191/192**, and from `-O0` for **192/192**.
  The remaining TUs are small and optimization-level-insensitive (their exact
  functions are byte-identical at `-Oz` and `-O2`). There is no evidence for a
  per-TU optimization level other than `-Oz`.
* **New vendored-library result:** zlib/libzip sampled TUs are `-Oz` (every
  sampled file changes at `-O1` and `-O2`; all but `zip_fopen_index.c` also at
  `-Os`). libcrypto's `sha256.c` changes
  at `-O1` and `-Os` but is identical at `-O2`/`-O3`; the GCC `-O3` in the
  OpenSSL `android-armeabi` recipe is consistent but not distinguishable from
  `-O2` on this subset.
* **New branch-weight result:** global `-mllvm -likely-branch-weight=64/500/1000`
  and `-unlikely-branch-weight=0/2` change **zero** exact functions across the
  190 TUs without per-file overrides. They only perturb the non-exact
  `Generator::createMission` and `SpaceLounge::drawLounge`. The original's
  global branch-weight defaults are unobservable on matched code; the
  StarMap/Radar per-file overrides are matching knobs, not global flags.
* `-mllvm -enable-unsafe-fp-math` is **not a clang 7.0.2 option** (unknown
  argument). The driver spelling `-Xclang -menable-unsafe-fp-math` is accepted
  and is **disproven** (byte_exact 1426).
* The 24 non-exact functions are not explained by any flag tested here. The
  ranked remaining hypotheses are source shape and the unobservable flags in
  §7.

## 1. Method and baseline

```sh
cd /home/debian/GalaxyOnFire2/wt/w5020
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Baseline measured before and after this pass:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` is 4524/4524. The original is
`sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 bytes. Our stripped link is the same size but a different hash.

The A/B harness is `tools/flag_fingerprint.py`. It writes a scratch toolchain
that includes the real `cmake/toolchain.cmake` and appends `GOF2_EXTRA_FLAGS` to
both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
include("<repo>/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Each variant is a fresh `/dev/shm/w5020/flag_fingerprint/build-<tag>`. The
measurement is the official `tools/verify/verify.py` with the same
`--fail-on-wrong-type --top 0` flags the `check` target uses. For variants with
thousands of changed functions the harness short-circuits only the difflib fuzzy
score and the pool-reconciliation `SequenceMatcher`; `byte_exact`,
`linked_exact`, `size-exact`, `missing`, `wrong_type` and `extra` are unchanged.
The canonical `cmake-build-match` is never modified and `--update-baseline` is
never run.

Reproduce the whole matrix:

```sh
python3 tools/flag_fingerprint.py --mode variants --jobs 8
python3 tools/flag_fingerprint.py --mode per-tu-all --jobs 48
python3 tools/flag_fingerprint.py --mode branch-weights --jobs 32
```

The script also supports `--mode per-tu` (all TUs containing the 24 non-exact
functions plus one representative exact TU per directory, and the vendored
libraries) and `--mode all`. Results are written to
`/dev/shm/w5020/flag_fingerprint/results.json`.

## 2. Independent artifact re-derivation

### 2.1 APK provenance

```sh
python3 - <<'PY'
import zipfile
p="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
z=zipfile.ZipFile(p)
print(z.read("META-INF/MANIFEST.MF").decode().splitlines()[:3])
i=z.getinfo("lib/armeabi-v7a/libgof2hdaa.so")
print(i.compress_type, i.file_size)
PY
```

```text
['Manifest-Version: 1.0', 'Built-By: Generated-by-ADT', 'Created-By: Android Gradle 3.1.2']
8 2187328
```

The APK names **Android Gradle Plugin 3.1.2** and the native library is
**deflated** (method 8), so the platform extracts it at install. The
`minSdkVersion=16` / `targetSdkVersion=27` values are the wave-93 AXML parse;
`minSdkVersion=16` is the native compile/link API and matches
`.note.android.ident`.

### 2.2 ELF identity

```sh
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW $O
readelf -nW $O
readelf -p .comment $O
readelf -AW $O
readelf -dW $O
readelf -SW $O
```

| observable | value | flag implication |
|---|---|---|
| ELF header | `ELF32 DYN ARM EABI5`, flags `0x5000200` (soft-float ABI) | armeabi-v7a **softfp** |
| `.note.android.ident` | `api=16`, `ndk="r18b"`, `build="5063045"` | NDK r18b, API 16 |
| `.comment` | clang 7.0.2 (`Android (4751641 based on r328903)`) + `GCC: (GNU) 4.9.x 20150123` | clang 7.0.2 app TUs + GCC 4.9 libcrypto |
| `.note.gnu.gold-version` | `gold 1.12` | NDK gold linker |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, unaligned v6 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -fPIC`; no `-fshort-enums`, no `-mno-unaligned-access` |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-line order; FMOD before `m/dl/c` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `DT_RELCOUNT` | 1162 | `-z combreloc` (gold default) |
| `.hash`, no `.gnu.hash` | SYSV hash only | gold Android default `--hash-style=sysv` |
| `.text` align | 32 | 32-byte-aligned input (libcrypto SHA asm), not a flag |
| `.bss` `sh_offset` | `0x21596c` (unaligned end of `.data`) | bfd strip normalization |

### 2.3 Build-id algorithm (new positive proof)

Gold's `--build-id` note is a 20-byte SHA-1. Re-linking our objects with the
canonical gold command but keeping the unstripped output:

```sh
ninja -C cmake-build-match -t commands link_so   # take the ld.gold line
# replace -o .../libgof2hdaa.unstripped.so with -o /dev/shm/w5020/ours_unstripped.so
# run the ld.gold command, then:
python3 - <<'PY'
# read .note.gnu.build-id, zero the 20-byte descriptor, sha1 the whole file
PY
```

Result:

```text
note: namesz=4 descsz=20 type=3 name="GNU"
descriptor                       ab4c71d770be30ff42bb5f832c346834829d6e4d
SHA1(file with descriptor zeroed) ab4c71d770be30ff42bb5f832c346834829d6e4d
SHA1(file as-is)                  09f661b5b328fe78bcd1abcd417826d722cfa3a6
```

The descriptor equals the zeroed-file hash exactly. The original's note has the
same shape (`namesz=4 descsz=20 type=3`, descriptor `c011810da45777b83ed721b2a87d517efed0d310`).
This proves the original was linked with `--build-id` **without** `-s`, and that
the build-id is a hash of the unstripped gold output.

### 2.4 Current section state (base 44225192c)

All 27 section headers are present; **26/27 are byte-identical**. The only
differing section header is `.ARM.extab` (one byte: `sh_size` `0x26298` vs
`0x26294`). Section content:

| section | original | ours | differing bytes | note |
|---|---:|---:|---:|---|
| `.text` | `0x1337c8` | `0x1337c8` | **101,683** | 24 non-exact functions |
| `.ARM.exidx` | `0x6f70` | `0x6f70` | **1,637** | shifted LSDA ranges |
| `.ARM.extab` | `0x26298` | `0x26294` | **134,548** (size differs) | 4-byte size residual |
| `.rel.plt` | `0x72d0` | `0x72d0` | **12** | 6-entry rotation, see below |
| `.note.gnu.build-id` | `0x24` | `0x24` | **20** | hash of unstripped output |
| all other 22 sections | — | — | **0** | `.rodata`, `.data`, `.data.rel.ro`, `.plt`, `.got`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.dynamic`, `.init_array`, `.fini_array`, `.comment`, `.ARM.attributes`, `.shstrtab`, ... |

This is 101,683 + 1,637 + 12 + 20 = **103,352** differing bytes in same-size
sections, plus the 134,548-byte `.ARM.extab` body. `docs/IDENTICALITY.md` records
`.text` 101,675 from wave 92; the 8-byte difference is from post-wave-92 source
passes at this base.

### 2.5 `.rel.plt` is not byte-identical at this base

Wave 93's note says `.rel.plt` content is byte-identical. At base `44225192c`
it is **not**: 12 bytes / 3 relocation entries differ, a six-entry rotation at
`.rel.plt` offsets `0x5878..0x58a0`:

| slot | original | ours |
|---|---|---|
| 1 | `StarSystem::switchPlanetForIntro` | `PlayerEgo::deleteHackingGame` |
| 2 | `Level::switchSkyboxForIntro` | `StarSystem::switchPlanetForIntro` |
| 3 | `ParticleSystemManager::reset` | `Level::switchSkyboxForIntro` |
| 4 | `PlayerEgo::startSmokeEmission` | `ParticleSystemManager::reset` |
| 5 | `PlayerStation::setVisible` | `PlayerEgo::startSmokeEmission` |
| 6 | `PlayerEgo::deleteHackingGame` | `PlayerStation::setVisible` |

The cause is the still-non-exact `LevelScript::process`: in our
`LevelScript.cpp.obj` the first relocation for `deleteHackingGame` is at
`0x543c`, before the `0x61b2`/`0x61ba`/`0x61d8` references to the other five;
the original's PLT first-reference order puts `deleteHackingGame` last. This is
a source-shape residual of one of the 24 non-exact functions, not a flag
difference. `docs/IDENTICALITY.md` already records the 12-byte rotation.

## 3. Whole-.so differential matrix

All variants are one extra flag on top of the canonical build. `byte`/`linked`
are the official verifier counts; `size` is `sum(osize == usize)`; `.text`,
`.rodata`, `.ARM.exidx`, `.ARM.extab` are section sizes; `diff` is the sum of
differing bytes in same-size sections; `chg24` is how many of the 24 non-exact
functions have different raw bytes from the canonical build; `new exact` is how
many previously non-exact functions became byte-exact (none did).

| variant | flags | byte | linked | size | .text | .rodata | exidx | extab | diff | chg24 | new exact |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| baseline | — | **4500** | **4500** | 4524/4524 | 0x1337c8 | 0x41b67 | 0x6f70 | 0x26294 | 103352 | 0 | 0 |
| fno_vectorize | `-fno-vectorize` | **4500** | **4500** | 4524/4524 | 0x1337c8 | 0x41b67 | 0x6f70 | 0x26294 | 103352 | 0 | 0 |
| fno_trapping_math | `-fno-trapping-math` | **4500** | **4500** | 4524/4524 | 0x1337c8 | 0x41b67 | 0x6f70 | 0x26294 | 103352 | 0 | 0 |
| fno_delete_null_pointer_checks | `-fno-delete-null-pointer-checks` | **4500** | **4500** | 4524/4524 | 0x1337c8 | 0x41b67 | 0x6f70 | 0x26294 | 103352 | 0 | 0 |
| inline_threshold_1 | `-mllvm -inline-threshold=1` | **4500** | **4500** | 4524/4524 | 0x1337c8 | 0x41b67 | 0x6f70 | 0x26294 | 103352 | 0 | 0 |
| inline_threshold_5 | `-mllvm -inline-threshold=5` | **4500** | **4500** | 4524/4524 | 0x1337c8 | 0x41b67 | 0x6f70 | 0x26294 | 103352 | 0 | 0 |
| fno_slp_vectorize | `-fno-slp-vectorize` | 1484 | 4383 | 4408/4524 | 0x132ea8 | 0x41b67 | 0x6f70 | 0x26284 | 18112 | 24 | 0 |
| fno_unroll_loops | `-fno-unroll-loops` | 2883 | 4490 | 4520/4524 | 0x1337a8 | 0x41b67 | 0x6f70 | 0x262a0 | 7399 | 14 | 0 |
| inline_threshold_0 | `-mllvm -inline-threshold=0` | 1356 | 4448 | 4498/4524 | 0x133688 | 0x41b67 | 0x6ff0 | 0x26358 | 12485 | 24 | 0 |
| inline_threshold_10 | `-mllvm -inline-threshold=10` | 1354 | 4414 | 4462/4519 | 0x1368f8 | 0x41b67 | 0x6f40 | 0x25264 | 12577 | 24 | 0 |
| inline_threshold_25 | `-mllvm -inline-threshold=25` | 1352 | 3825 | 3865/4398 | 0x139bc8 | 0x41b67 | 0x6b70 | 0x24d34 | 2334 | 24 | 0 |
| inline_threshold_50 | `-mllvm -inline-threshold=50` | 1350 | 3708 | 3746/4390 | 0x1475f8 | 0x43057 | 0x6b10 | 0x24f60 | 3357 | 24 | 0 |
| inline_threshold_100 | `-mllvm -inline-threshold=100` | 1350 | 3517 | 3553/4318 | 0x14e208 | 0x43057 | 0x6938 | 0x256dc | 3272 | 24 | 0 |
| unsafe_fp_math | `-mllvm -enable-unsafe-fp-math` | BUILD FAIL | | | | | | | | | |
| menable_unsafe_fp_math | `-Xclang -menable-unsafe-fp-math` | 1426 | 4367 | 4431/4524 | 0x133368 | 0x41b67 | 0x6f70 | 0x26290 | 17309 | 24 | 0 |
| ffast_math | `-ffast-math` | 1335 | 4222 | 4369/4524 | 0x132998 | 0x41b67 | 0x6f58 | 0x2627c | 2180 | 24 | 0 |
| fno_math_errno | `-fno-math-errno` | 1356 | 4486 | 4514/4524 | 0x133638 | 0x41b67 | 0x6f58 | 0x2626c | 1855 | 24 | 0 |
| freciprocal_math | `-freciprocal-math` | 1561 | 4402 | 4483/4524 | 0x133808 | 0x41b67 | 0x6f70 | 0x2629c | 11253 | 24 | 0 |
| fwrapv | `-fwrapv` | 1398 | 4461 | 4500/4524 | 0x1338c8 | 0x41b67 | 0x6f70 | 0x26294 | 13682 | 24 | 0 |
| fno_strict_overflow | `-fno-strict-overflow` | 1398 | 4461 | 4500/4524 | 0x1338c8 | 0x41b67 | 0x6f70 | 0x26294 | 13682 | 24 | 0 |
| fno_builtin | `-fno-builtin` | 1350 | 3985 | 4119/4524 | 0x1347b8 | 0x41b67 | 0x6fb0 | 0x26378 | 410618 | 24 | 0 |
| O1 | `-O1` | 1033 | 1781 | 2676/4524 | 0x143f28 | 0x41bc7 | 0x7840 | 0x2627c | 89 | 24 | 0 |
| O2 | `-O2` | 1168 | 2157 | 2411/4296 | 0x1632e8 | 0x42d77 | 0x6880 | 0x250b4 | 81 | 24 | 0 |
| O3 | `-O3` | 1167 | 2117 | 2247/4295 | 0x171638 | 0x42c97 | 0x6838 | 0x25470 | 299 | 24 | 0 |
| Os | `-Os` | 1263 | 2648 | 2883/4356 | 0x14bdc8 | 0x43057 | 0x6a30 | 0x24770 | 3326 | 24 | 0 |

Notes:

* The five neutral variants have **byte-identical `.text`** to the canonical
  build (101,683 differing bytes vs the original, exactly the baseline value).
  They differ only in the build-id note, which hashes the unstripped output
  (debug info records the flag).
* `-fno-unroll-loops` changes 14 of the 24 non-exact functions and makes 1,617
  previously exact functions non-exact; it is a real codegen difference, not a
  layout artifact.
* `-fno-vectorize` is neutral because the loop vectorizer does not fire on this
  `-Oz` code; `-fno-slp-vectorize` is much worse, so SLP vectorization is on.
* `-mllvm -inline-threshold=1` and `=5` are both neutral; `=0` and `>=10` are
  worse. Clang's `-Oz` sets the inline threshold to 5; 1 is in the same
  no-change plateau. The default is therefore bracketed to `[1,5]`, consistent
  with `-Oz`'s 5.
* `-mllvm -enable-unsafe-fp-math` is unknown to clang 7.0.2. The accepted
  spelling is `-Xclang -menable-unsafe-fp-math`, and it is worse (1426).

## 4. Per-TU optimization-level sweep

The harness compiles each TU with the canonical command plus a trailing
`-O0/-O1/-O2/-O3/-Os` (last `-O` wins) and compares every object-file function
byte with the canonical `-Oz` object. A TU whose **exact** functions change at
another level is positive proof that that TU was compiled at `-Oz`.

```sh
python3 tools/flag_fingerprint.py --mode per-tu-all --jobs 48
```

| level | TUs | TUs with no exact-function change | changed exact functions |
|---|---:|---:|---:|
| `-O0` | 192 | **0** | 4,997 |
| `-O1` | 192 | **1** (`IApplicationModule.cpp`) | 3,201 |
| `-O2` | 192 | **8** | 2,117 |
| `-O3` | 192 | **7** | 2,156 |
| `-Os` | 192 | **9** | 1,732 |

The eight `-O2`-insensitive TUs are all small, level-insensitive TUs:
`IApplicationModule.cpp`, `ImagePart.cpp`, `PendingProduct.cpp`, `SpriteGun.cpp`,
`SpacePoint.cpp`, `BackButton.c`, `GOF2HD2012.c`, `NdkAutosave.cpp`. Their exact
functions are byte-identical at `-Oz` and `-O2`, so the binary cannot distinguish
those two levels for them. `IApplicationModule.cpp` (one function) is also
insensitive at `-O1`; it changes at `-O0`. There is no TU where a non-`-Oz`
level is required.

The 24 non-exact functions are all in TUs where `-Oz` is positively
distinguished from `-O2` (the `-O2`-insensitive list contains none of the 24).
Per-TU optimization level is therefore not the cause of the residual.

### 4.1 Vendored libraries

`third_party/build_match_lib.cmake` builds zlib/libzip with clang `-Oz` and
libcrypto with GCC 4.9 `-O3`. Object-level A/B:

| vendored TU | `-O1` changed | `-O2` changed | `-Os` changed |
|---|---:|---:|---:|
| zlib/inflate.c | 12 | 11 | 4 |
| zlib/deflate.c | 20 | 17 | 9 |
| zlib/crc32.c | 3 | 2 | 2 |
| zlib/adler32.c | 2 | 2 | 2 |
| zlib/trees.c | 13 | 8 | 9 |
| zlib/inftrees.c | 1 | 1 | 1 |
| libzip/zip_open.c | 4 | 2 | 2 |
| libzip/zip_close.c | 2 | 1 | 1 |
| libzip/zip_dirent.c | 13 | 7 | 8 |
| libzip/zip_fopen_index.c | 2 | 2 | 0 |
| libcrypto/sha256.c | 9 | **0** | 6 |

Every sampled zlib/libzip file changes at `-O1` and `-O2` (all but
`zip_fopen_index.c` also at `-Os`), so `-Oz` is positively proven for them.
libcrypto's `sha256.c` changes at `-O1` and `-Os`
but is identical at `-O2` and `-O3`; the OpenSSL `android-armeabi` recipe's
`-O3` is consistent but not distinguishable from `-O2` on this subset. The two
libcrypto `.S` files are assembly and unaffected by `-O`.

## 5. Global branch-weight defaults (layout-free)

The per-file overrides in `CMakeLists.txt` (`StarMap.cpp` 7:1, `Radar.cpp` 80:1)
make a global `-mllvm -likely-branch-weight=...` duplicate the option on the
command line, which clang 7 rejects ("may only occur zero or one times"). The
harness therefore compiles every TU **except StarMap/Radar** with the global
variant and compares object bytes; this isolates the global default without
layout shifts.

```sh
python3 tools/flag_fingerprint.py --mode branch-weights --jobs 32
```

| variant | TUs | changed TUs | changed exact | changed non-exact |
|---|---:|---:|---:|---:|
| default | 190 | 0 | 0 | 0 |
| `-likely-branch-weight=64` | 190 | 2 | **0** | 2 |
| `-likely-branch-weight=500` | 190 | 1 | **0** | 1 |
| `-likely-branch-weight=1000` | 190 | 0 | **0** | 0 |
| `-unlikely-branch-weight=0` | 190 | 1 | **0** | 1 |
| `-unlikely-branch-weight=2` | 190 | 0 | **0** | 0 |

The only non-exact functions affected are `Generator::createMission` (likely
64/500) and `SpaceLounge::drawLounge` (likely 64, unlikely 0). **No exact
function changes for any global branch-weight variant.** The original's global
branch-weight defaults are therefore unobservable on matched code; the
per-file StarMap/Radar overrides are matching knobs for non-exact/percentage
residuals, not evidence of a global non-default.

The whole-.so control (`GOF2_NO_PER_FILE_WEIGHTS=ON`, no extra flags) drops
byte_exact to 2404 because removing the two per-file overrides shifts `.text`
by `+0x20` and changes PIC pool words in many later functions. That is a layout
transient, not a code difference; the branch-weight variants should be compared
to the control (2404), not to the 4500 baseline. The control's `linked_exact`
is 4496, and all branch variants also have `linked_exact` 4496.

## 6. Classification table

### PROVEN (the binary requires the canonical setting)

| setting | positive evidence |
|---|---|
| NDK r18b clang 7.0.2 | `.note.android.ident` build 5063045; `.comment` clang 7.0.2 |
| API 16 | `.note.android.ident` api=16; APK `minSdkVersion=16` |
| armeabi-v7a / Thumb-2 / NEON / softfp | ELF `e_flags` soft-float; `.ARM.attributes` ARM v7, Thumb-2, VFPv3, NEONv1; `-marm` worse, `-mfpu=vfpv3-d16` build fail, `-mfloat-abi=hard` link fail |
| `-Oz` | global `-O1/-O2/-O3/-Os` all much worse; per-TU `-Oz` distinguished from `-O2` for 184/192 TUs, from `-O1` for 191/192, from `-O0` for 192/192 |
| `-g` | wave 93: `-g0` drops byte_exact 4499 -> 2069; debug info pins PostRA sinking |
| `-std=c++11` | wave 93: signed `new[]` count guard; `-std=c++14` worse |
| `-fPIC` | `.ARM.attributes` `Tag_ABI_PCS_GOT_use: GOT-indirect`; no `R_ARM_REL32`/`DT_TEXTREL` |
| `-frtti` | 99 vtables, 176 typeinfo; `-fno-rtti` worse (1701) |
| `-fstack-protector-strong` | 787 canaries exactly; basic 88, all 3984 |
| `-funwind-tables` | 3566 `.ARM.exidx` entries, 0x26298 `.ARM.extab`; removing drops to 2878 |
| `-ffunction-sections` | 272 CANTUNWIND sentinels (193 empty-`.text` pairs); removing gives 52 |
| `-fno-threadsafe-statics` disproven | `-fno-threadsafe-statics` drops to 1368 |
| `-fno-use-cxa-atexit` disproven | `-fno-use-cxa-atexit` drops to 1356 |
| `-fno-exceptions` disproven | build fail (`try` in `Level.cpp`) |
| `-fno-jump-tables` disproven | drops to 1363; 401 `tbb` + 84 `tbh` |
| `-fno-slp-vectorize` disproven | drops to 1484 |
| `-fno-unroll-loops` disproven | drops to 2883 |
| `-fmath-errno` (default) | the original imports `__errno`; `-fno-math-errno` drops byte_exact to 1356 |
| `-freciprocal-math` disproven | drops to 1561 |
| `-ffast-math` disproven | drops to 1335 |
| `-fno-builtin` disproven | drops to 1350 |
| `-fwrapv` / `-fno-strict-overflow` disproven | both drop to 1398; strict overflow is on |
| `-fno-strict-aliasing` disproven | wave 93: drops to 1371 |
| `-fdata-sections` disproven | wave 93: `.rodata` 0x41b67 -> 0x41a07; byte 4311 |
| `-fvisibility=hidden` disproven | wave 93: byte 57 |
| `-fno-common` disproven | link fail (duplicate `g_android_back_button_pressed`) |
| `-fshort-enums` disproven | wave 93: byte 2869; `.ARM.attributes` enum int |
| `-fsigned-char` disproven | wave 93: byte 4438; ARM default unsigned char |
| `-fomit-frame-pointer` disproven | wave 93: byte 1298; 2473 frame-pointer prologues |
| `-fno-inline` disproven | wave 93: byte 1219 |
| `-mno-unaligned-access` disproven | wave 93: byte 1391; `.ARM.attributes` unaligned v6 |
| `-marm` disproven | wave 93: byte 57 |
| `-Os` disproven | byte 1263 |
| `-O1/-O2/-O3` disproven | byte 1033/1168/1167 |

### UNOBSERVABLE (the alternative produces identical code sections)

| alternative | evidence |
|---|---|
| `-fno-vectorize` | byte 4500, linked 4500, size 4524/4524, `.text` byte-identical to baseline |
| `-fno-trapping-math` | byte 4500, `.text` byte-identical |
| `-fno-delete-null-pointer-checks` | byte 4500, `.text` byte-identical |
| `-fno-sized-deallocation` | `_ZdlPvj`/`_ZdaPvj` defined but 0 call sites; flag neutral |
| `-mllvm -inline-threshold=1` | byte 4500, `.text` byte-identical; same plateau as default 5 |
| `-mllvm -inline-threshold=5` | byte 4500, `.text` byte-identical; `-Oz` default |
| `-mllvm -likely-branch-weight=64/500/1000` | 0 exact functions change across 190 TUs; only non-exact functions |
| `-mllvm -unlikely-branch-weight=0/2` | 0 exact functions change across 190 TUs; only non-exact functions |
| `-fPIC` vs `-fpic` | wave 93: byte-identical; both GOT-indirect, no REL32 |
| `-std=c++11` vs `gnu++11` | wave 93: byte-identical |
| `-DNDEBUG` / `-DANDROID` | wave 93: codegen-neutral on this source |
| `-fmerge-constants`, `-fno-plt` | wave 93: unobservable |

### DISPROVEN / UNSUPPORTED (the alternative is not the original)

| alternative | evidence |
|---|---|
| `-mllvm -enable-unsafe-fp-math` | clang 7.0.2: unknown argument (build fail) |
| `-Xclang -menable-unsafe-fp-math` | byte 1426, linked 4367, size 4431/4524 |
| `-fno-slp-vectorize` | byte 1484 |
| `-fno-unroll-loops` | byte 2883 |
| `-mllvm -inline-threshold=0/10/25/50/100` | byte 1356/1354/1352/1350/1350 |
| `-ffast-math` | byte 1335 |
| `-fno-math-errno` | byte 1356 |
| `-freciprocal-math` | byte 1561 |
| `-fwrapv` | byte 1398 |
| `-fno-strict-overflow` | byte 1398 |
| `-fno-builtin` | byte 1350; `.text` 0x1347b8 |
| `-O1/-O2/-O3/-Os` | byte 1033/1168/1167/1263 |

## 7. Ranked remaining flag hypotheses

The 24 non-exact functions are **not** explained by any flag tested here. In
order of remaining plausibility:

1. **Source shape of the 24 functions** (by far the most likely). The per-TU
   `-Oz` proof, the vendored `-Oz` proof, and the branch-weight sweep leave no
   flag family that changes an exact function without changing the whole
   binary. The `.rel.plt` 6-entry rotation is a direct symptom of
   `LevelScript::process`'s call order.
2. **Unobservable codegen-neutral flags** — `-fPIC` vs `-fpic`,
   `-std=c++11` vs `gnu++11`, `-DNDEBUG`, `-DANDROID`,
   `-fno-delete-null-pointer-checks`, `-fno-trapping-math`,
   `-fno-vectorize`, `-fmerge-constants`, `-fno-plt`. None can change the
   remaining bodies.
3. **`-mllvm -inline-threshold` in the 1..5 plateau** — unobservable; the
   `-Oz` default 5 is the natural choice.
4. **Global `-mllvm` branch-weight defaults** — unobservable on every exact
   function; only `Generator::createMission` and `SpaceLounge::drawLounge`
   change. A global non-default is not needed.
5. **Vendored libcrypto `-O2` vs `-O3`** — `sha256.c` is identical at both; the
   OpenSSL `android-armeabi` recipe uses `-O3`, so keep `-O3`.
6. **`-mllvm -enable-unsafe-fp-math`** — not a valid clang 7.0.2 option; the
   accepted `-Xclang -menable-unsafe-fp-math` is worse.

## 8. Build-file change

`CMakeLists.txt` gains a default-off option so the harness can test global
branch weights without duplicating the per-file `-mllvm` options:

```cmake
option(GOF2_NO_PER_FILE_WEIGHTS "Omit per-file branch-weight overrides (flag A/B harness)" OFF)
if(NOT GOF2_NO_PER_FILE_WEIGHTS)
  set_source_files_properties(... StarMap.cpp ...)
  set_source_files_properties(... Radar.cpp ...)
endif()
```

Default behavior is unchanged: `cmake --build cmake-build-match --target check`
still reports byte_exact 4500, linked_exact 4500, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

## 9. Gate counts

| state | byte_exact | linked_exact | size-exact | data | imports | lint | drift |
|---|---:|---:|---:|---:|---:|---|---:|
| before | 4500 | 4500 | 4524/4524 | 843/843 | 207 | CLEAN | 0 |
| after | 4500 | 4500 | 4524/4524 | 843/843 | 207 | CLEAN | 0 |

No gate improvement was available: every tested alternative is neutral or
worse. The note and harness are the deliverable; the only build-file change is
the default-off option.
