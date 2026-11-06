# Build-flags / build-provenance research A (wave 123, w7319)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7319`,
base `1c8c9b20e` ("report: wave-122 final snapshot"). Scratch work is under
`/dev/shm/w7319`. **No `src/`, `tools/`, baseline, allowlist or build-system change is
landed.** This note is the only commit.

Canonical gate at the base (re-measured after this pass, unchanged):

```text
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

Original: `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B. Canonical build: `sha256
2c0a0424b3a41a24b4cc32923b45b6201af629edae5fc5d553d910a85badf93a`, same size.

## 0. Bottom line

* **Wave 120's claim is independently confirmed.** Compiling all 192 app TUs with the
  stock NDK r18b `android.toolchain.cmake` (Release) produces **0 differing function
  blobs and 0 symbol-set differences** versus the canonical `cmake/toolchain.cmake`
  objects; relinking with the full CMake/ndk-build default link flag set produces the
  **same `.so` SHA-256**.
* **The app TUs were built with CMake, not ndk-build.** The two discriminating
  defaults are `-fdata-sections` (ndk-build adds it, CMake does not) and
  `-fno-exceptions -fno-rtti` (ndk-build's `GLOBAL_CXXFLAGS` defaults, CMake does not).
  The original has no `-fdata-sections` and has exceptions + RTTI on.
* **No new global or per-TU flag improves any of the 18 non-exact functions.** A
  259-candidate `-f`/`-mllvm` screen on the 13 TUs that own the 18 targets, followed by
  per-TU link-and-score for every clean candidate, found **zero positive deltas**. The
  cleanest global candidate found (`-mllvm -tail-dup-placement-penalty=0`) changes
  exactly one function in the whole tree (`MGame::OnTouchEnd`) and makes it 0.015 raw
  points *worse* (81.715006 -> 81.700288).
* The residual is therefore **source shape**, not a missing compiler/linker flag. The
  EH/LSDA shape is already correct except for one 8-byte scope in
  `Level::createShip`.

## 1. Independent re-test of the wave-120 stock-CMake equivalence

Scratch project: `/dev/shm/w7319/stock` (an OBJECT library listing exactly the 192
paths in `tools/source_order.txt`, with the same include dirs and the two existing
per-file branch-weight overrides for `StarMap.cpp` / `SpaceLounge.cpp`). Configured
with the **stock** NDK r18b toolchain:

```sh
NDK=$PWD/.cache/ndk/android-ndk-r18b
cmake -S /dev/shm/w7319/stock -B /dev/shm/w7319/stock/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON \
  -DANDROID_CPP_FEATURES="rtti exceptions" \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build /dev/shm/w7319/stock/build -j8
```

The emitted C++ line is the stock r18b Release line:

```text
clang++ --target=armv7-none-linux-androideabi16 --gcc-toolchain=... --sysroot=...
  -isystem .../libc++/include -isystem .../support/include -isystem .../libc++abi/include
  -isystem .../arm-linux-androideabi
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -fPIC
```

Comparison against `cmake-build-match/CMakeFiles/gof2.dir/<TU>.obj`, using
`tools/flag_fingerprint.py`'s `object_functions()` (defined `STT_FUNC` bytes per ET_REL
object):

```text
TUs compared                     192
functions compared             5085
TUs with differing function blobs   0
functions with differing bytes      0
TUs with differing symbol sets      0
```

(Full object bytes differ for all 192 TUs, but only in debug-info offsets/paths and
symbol-table placement: the scratch build directory path is a different length, which
shifts the `.debug_*` sections by 0x20 bytes. Every `.text`/`.rodata`/`.data` section
size is identical and every function blob is identical; the linked `.so` is identical.)

Link equivalence: the canonical gold command was re-run with the full CMake/ndk-build
default link flag set added after `-shared`:

```text
--no-undefined --fix-cortex-a8
--exclude-libs=libatomic.a --exclude-libs=libunwind.a
-z noexecstack -z relro --warn-shared-textrel --fatal-warnings
--hash-style=sysv --no-as-needed
-z max-page-size=4096 -z common-page-size=4096 --spare-dynamic-tags=5
```

then stripped and `.shstrtab`-normalised exactly like the canonical link. Result:

```text
variant  2c0a0424b3a41a24b4cc32923b45b6201af629edae5fc5d553d910a85badf93a
current  2c0a0424b3a41a24b4cc32923b45b6201af629edae5fc5d553d910a85badf93a
```

So the current recipe is codegen-equivalent to stock CMake Release for this tree.

## 2. Provenance re-derived from the shipped artifacts

All of the following were re-measured directly from
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` and the APK
(`Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`).

### 2.1 Toolchain / ABI

| observable | value | conclusion |
|---|---|---|
| `.note.android.ident` | API `16`, `r18b`, build `5063045` | NDK r18b, API 16 |
| NDK `source.properties` | `Pkg.Revision = 18.1.5063045` | build number matches exactly |
| `.comment` | `clang version 7.0.2 (4751641 based on r328903)` | NDK r18b clang |
| `.comment` | `GCC: (GNU) 4.9.x 20150123 (prerelease)` | libcrypto GCC 4.9 |
| `.note.gnu.gold-version` | `gold 1.12` | NDK r18b gold |
| ELF header | `ELF32 DYN ARM EABI5`, `e_flags 0x5000200` | armeabi-v7a **softfp** |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -fPIC`; no `-fshort-enums`, no `-mno-unaligned-access` |
| `.ARM.attributes` vs ours | **byte-identical** | no ABI flag difference |

### 2.2 ISA mode per function

`Elf32.functions()` over the original: **4501 Thumb / 23 ARM**. The 23 ARM functions
are exactly the OpenSSL libcrypto subset (`SHA224/SHA256*`, `sha256_block_data_order*`,
`OPENSSL_*`, `CRYPTO_memcmp`, `_armv7_*`/`_armv8_*_probe`). So:

* app TUs, zlib and libzip: **Thumb** (`-mthumb`);
* libcrypto: **ARM** GCC (`-marm`, GCC default).

### 2.3 Exceptions / RTTI / statics / stack protector / visibility

| observable | value | conclusion |
|---|---|---|
| `__gxx_personality_v0`, `__cxa_throw`, `__cxa_begin_catch`, `__cxa_end_catch` | defined in `.text` (from libc++abi) | exceptions **ON** |
| `__dynamic_cast` + 451 defined `_ZTI`/`_ZTV`/`_ZTS` symbols | defined | RTTI **ON** |
| `__cxa_guard_acquire/release/abort` | defined and referenced | `-fthreadsafe-statics` (default) **ON** |
| `__stack_chk_fail@LIBC` | 787 `bl` sites | `-fstack-protector-strong` **ON** |
| defined dynsym visibility | 4895 `GLOBAL DEFAULT`, 475 `WEAK DEFAULT`, 0 `HIDDEN`/`PROTECTED` | `-fvisibility=default`, no `-fvisibility-inlines-hidden` |
| `.ARM.exidx` | 3566 entries: 272 CANTUNWIND, 856 inline, 2438 extab | `-funwind-tables` **ON**; kind sequence identical to ours |

### 2.4 Link shape

| observable | value | conclusion |
|---|---|---|
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `DT_NEEDED` order | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-line order |
| hash section | `.hash` only, no `.gnu.hash` | gold Android default |
| `.init_array` / `.fini_array` | 12 B (3 entries) / 8 B (2 entries) | 3 static initialisers, 2 finalisers |
| `.comment` / `.note.android.ident` / `.note.gnu.gold-version` / `.ARM.attributes` | byte-identical to ours | toolchain/link identity |

### 2.5 CMake vs ndk-build (the decisive discriminator)

The NDK r18b defaults differ in exactly two codegen-visible ways:

```text
$NDK/build/core/default-build-commands.mk (ndk-build)
  GLOBAL_CFLAGS   = -target ... -fdata-sections -ffunction-sections \
                    -fstack-protector-strong -funwind-tables -no-canonical-prefixes -g
  GLOBAL_CXXFLAGS = $(GLOBAL_CFLAGS) -fno-exceptions -fno-rtti

$NDK/build/cmake/android.toolchain.cmake (CMake)
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes
  ... -Oz -DNDEBUG ... -fPIC
```

The original has **no `-fdata-sections`** (adding it drops the gate to
4314/4501/4524; `-fno-data-sections` is the canonical 4506) and has **exceptions +
RTTI on** (ndk-build would need `APP_CPPFLAGS := -fexceptions -frtti` to override its
defaults). The single explanation that needs no overrides is the **CMake** toolchain.
This is consistent with the APK's `Created-By: Android Gradle 3.1.2` and the
`externalNativeBuild` era, but the exact CMake version is not recoverable from the
stripped ELF.

### 2.6 LTO / `-Oz` / `-g`

* **No LTO.** All 4,524 functions survive as separate symbols at their exact original
  addresses and `.text` is in plain TU/function input order. Any cross-TU LTO would
  merge/reorder/inline them. Per-TU compilation is proven.
* **`-Oz`** is uniquely best (`-O0/-O1/-O2/-O3/-Os` are 61/1033/1168/1167/1263 byte
  exact in the wave-119 matrix).
* **`-g` (g2)** is load-bearing at clang 7 `-Oz`: `-g0`/`-g1`/`-gline-tables-only`
  drop to 2071/4247/4485; `-g2`/`-g3`/`-gdwarf-2/3/4` are neutral.

### 2.7 Vendored libraries

* zlib 1.2.3 and libzip 0.9.3: clang, **Thumb**, `-Oz -g -fstack-protector-strong`
  (plus `-mllvm -align-all-functions=2` on `zip_file_strerror`, a matching knob).
* libcrypto 1.1.0l: real GCC 4.9 (NDK r13b), **ARM**, `-O3` (`Configure
  android-armeabi` CFLAGS + `-march=armv7-a`).

## 3. New flag screen (259 candidates)

Two object-level screens were run with
`tools/flag_fingerprint.py --mode llvm-screen` on the 13 TUs that own the 18 non-exact
functions. A flag that does not change a target's object bytes cannot change its linked
instruction stream, so this is a sound pre-filter.

* Screen 1: 219 `-f*` / `-mllvm` candidates (`/dev/shm/w7319/newflags.txt`).
* Screen 2: 40 more candidates (`/dev/shm/w7319/newflags2.txt`), including the
  `-mno-movt`, `-pre-RA-sched=*`, `-ffast-math`, `-menable-unsafe-fp-math`,
  `-funsafe-math-optimizations`, `-fno-honor-nans`, `-mllvm -tail-merge-threshold`,
  `-mllvm -tail-dup-size`, and `-mllvm -inline-threshold=2..9` families.

Results:

* Most candidates are **neutral** for all 18 targets (they do not change a single
  target object byte).
* Only a handful change any target. The cleanest global candidate found is
  `-mllvm -tail-dup-placement-penalty=0`: a full 192-TU object diff shows it changes
  **exactly one function in the whole tree**, `MGame::OnTouchEnd`, and **zero exact
  functions**. Link-and-score: 81.715006 -> 81.700288 (slightly worse; official 81.7
  unchanged, so gate-neutral).
* `-mllvm -inline-threshold=10` changes only `HangarWindow::render` in the target TUs
  but **79 exact functions** across the tree (the known global gate 1354). Per-TU on
  `HangarWindow.cpp` it changes only `HangarWindow::render`, but scores 78.976 vs the
  94.043 baseline (worse). `inline-threshold=6/7/8/9` are the same 78.976.
* `-mllvm -enable-machine-outliner` crashes clang 7.0.2 on this tree;
  `-fno-semantic-interposition` and `-menable-unsafe-fp-math` are unknown to clang 7.

### 3.1 Per-TU clean candidates (target changes, 0 exact changes in that TU)

Every such cell was compiled, swapped into the canonical link, and scored against the
original. Baseline raw scores are from the canonical `.so`.

| TU | target | flag | baseline | variant | size |
|---|---|---|---:|---:|---:|
| `LevelScript.cpp` | `LevelScript::process` | `-mllvm -disable-postra-machine-sink` | 60.065662 | 60.065662 | 57232 |
| `LevelScript.cpp` | `LevelScript::process` | `-mllvm -disable-ifcvt-diamond` | 60.065662 | 60.020890 | 57232 |
| `LevelScript.cpp` | `LevelScript::process` | `-mllvm -disable-constant-hoisting` | 60.065662 | 60.056210 | 57232 |
| `Radar.cpp` | `Radar::draw` | `-mllvm -disable-postra-machine-sink` | 74.541284 | 74.541284 | 9908 |
| `Radar.cpp` | `Radar::draw` | `-mllvm -disable-lsr` | 74.541284 | 74.322580 | 9908 |
| `Layout.cpp` | `Layout::Layout` | `-fno-unwind-tables` | 48.744520 | 48.744520 | 7072 |
| `Level.cpp` | `Level::createMission` | `-mllvm -enable-earlycse-memssa=0` | 92.343234 | 92.343234 | 17172 |
| `Generator.cpp` | `Generator::createMission` | `-mllvm -disable-postra-machine-sink` | 89.944904 | 89.807160 | 1996 |
| `HangarWindow.cpp` | `HangarWindow::render` | `-mllvm -inline-threshold=10` | 94.043023 | 78.975630 | 9700 |
| `MGame.cpp` | `MGame::OnTouchEnd` | `-mllvm -tail-dup-placement-penalty=0` | 81.715006 | 81.700288 | 8080 |

No cell is positive. Cells that also change an exact function in the same TU are
net-negative unless the target becomes exact, and none does.

## 4. Current section / EH state (for the next worker)

27 sections, names identical. Differing sections:

| section | original | ours | differing bytes |
|---|---:|---:|---:|
| `.note.gnu.build-id` | 36 | 36 | 20 |
| `.text` | 1,259,464 | 1,259,464 | 86,947 |
| `.ARM.exidx` | 28,528 | 28,528 | 1,327 |
| `.ARM.extab` | 156,312 | **156,304** | size -8 |
| `.rel.plt` | 29,392 | 29,392 | 6 |

`.rodata`, `.data`, `.bss`, `.got`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`,
`.gnu.version*`, `.data.rel.ro`, `.init_array`, `.fini_array`, `.dynamic`, `.plt`,
`.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes` and
`.shstrtab` are byte-identical.

`exidx_table.py --diff` reports **3566 entries on both sides** with an **identical kind
sequence** (272 CANTUNWIND, 856 inline, 2438 extab). The only extab-record difference
is `Level::createShip`: original 336 B, ours 328 B (-8). So the EH/LSDA shape is a
single missing cleanup scope, not a flag.

The `.rel.plt` 6-byte difference is exactly three adjacent relocation entries
(3329-3331) whose order is a rotation of the first-reference order of
`MenuTouchWindow::inCinematicMode`, `MGame::setCinematicMode` and
`Hud::resetAnalogStick`; it is a consequence of the source-shape residual in the
non-exact functions that call them, not a separate flag problem.

## 5. The 18 non-exact functions (wave-123 baseline)

| function | pct | size |
|---|---:|---:|
| `_ZN6LayoutC1Ev` | 48.7 | 7072 |
| `_ZN6LayoutC2Ev` | 48.7 | 7072 |
| `_ZN11LevelScript7processEi` | 60.3 | 57232 |
| `_ZN6Status16missionCompletedEbbx` | 71.0 | 1432 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 73.9 | 2764 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 74.5 | 9908 |
| `_ZN5MGame10OnTouchEndEiiPv` | 81.7 | 8080 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 89.9 | 1996 |
| `_ZN5Level13createMissionEv` | 92.3 | 17172 |
| `_ZN5Level11createSceneEv` | 92.8 | 2252 |
| `_ZN3Gun22calcCharacterCollisionEv` | 93.4 | 1984 |
| `_ZN12HangarWindow6renderEv` | 94.0 | 9700 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 95.3 | 9332 |
| `_ZN11SpaceLounge10drawLoungeEv` | 95.7 | 1948 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 98.1 | 1896 |
| `_ZN10ModStationC1Ev` | 99.2 | 1008 |
| `_ZN10ModStationC2Ev` | 99.2 | 1008 |
| `_ZN5MGame8OnUpdateEv` | 99.9 | 11440 |

All 18 are size-exact. The diffs are register allocation, instruction scheduling,
block order, switch/jump-table case order, and literal-pool placement — the signature
of source shape, not of a global flag.

## 6. Proven vs hypothesis

**Proven by this pass**

1. The current compile recipe is equivalent to stock NDK r18b CMake Release for all
   192 app TUs: 0 differing function blobs, 0 symbol-set differences.
2. Relinking with the full CMake/ndk-build default link flag set gives the identical
   `.so` hash `2c0a0424...`.
3. NDK r18b / clang 7.0.2 / gold 1.12 / API 16 / armeabi-v7a softfp / Thumb-2 /
   VFPv3 / NEONv1.
4. Exceptions, RTTI, threadsafe statics, `-fstack-protector-strong`,
   `-funwind-tables`, default visibility, `-fPIC`, `-Oz`, `-g` are all on; no
   `-fdata-sections`.
5. The app TUs were built with the CMake toolchain, not ndk-build (the
   `-fdata-sections` + `-fno-exceptions -fno-rtti` discriminator).
6. No LTO.
7. 4501/4524 functions are Thumb; the 23 ARM functions are exactly the libcrypto
   subset.
8. No new global or per-TU flag improves any of the 18 targets.

**Not settled**

1. The exact CMake version and original build directory are not recoverable from the
   stripped ELF; they only affect `.note.gnu.build-id` and debug-info paths.
2. The original's full flag list beyond the observable set is not recoverable; the
   neutral flags cannot be excluded as used-but-neutral.
3. The 18 residuals are source-shape work (switch case order, register allocation,
   one 8-byte LSDA scope), not build-flag work.

## 7. Reproduction

```sh
# stock-CMake object equivalence
cmake --build /dev/shm/w7319/stock/build -j8
python3 - <<'PY'
import sys
from pathlib import Path
sys.path.insert(0, 'tools')
import flag_fingerprint as ff
ROOT = Path('.').resolve()
canon = ROOT/'cmake-build-match/CMakeFiles/gof2.dir'
stock = Path('/dev/shm/w7319/stock/build/CMakeFiles/gof2_stock.dir/home/debian/GalaxyOnFire2/wt/w7319')
for s in [l.strip() for l in open('tools/source_order.txt') if l.strip().startswith('src/')]:
    a = ff.object_functions(canon/(s+'.obj'))
    b = ff.object_functions(stock/(s+'.o'))
    assert a == b, s
print('192/192 TUs identical')
PY

# new flag screen
python3 tools/flag_fingerprint.py --mode llvm-screen \
  --llvm-candidates /dev/shm/w7319/newflags.txt --jobs 8 \
  --workdir /dev/shm/w7319/ff

# per-TU link-and-score harness
python3 /dev/shm/w7319/batch_pertu.py
```

Scratch artifacts (not tracked): `/dev/shm/w7319/{stock,ff,ff2,fullscreen}`,
`newflags.txt`, `newflags2.txt`, `fullscreen.py`, `batch_pertu.py`,
`sweep_penalty.py`, `test_pertu.py`, `scorefn2.py`, `cases.json`.

## 8. Gate state

No source or build-flag change was landed. `tools/verify_baseline.json` was not
touched; `report.json` was not committed. The final `check` is unchanged:

```text
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

The worktree is clean.
