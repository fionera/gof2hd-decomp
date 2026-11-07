# Build-flag / PGO provenance, wave 136 (w8119)

Status: **note-only research commit**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8119`, base `2e51d765d` ("Wave 135 docs:
authoritative .text 84,960 / .ARM.extab 1,404 after case-125 switch").
Scratch: `/dev/shm/w8119`. No `src/`, `tools/`, `CMakeLists.txt`, baseline,
allowlist or doc change is landed. This note is the only commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check`):

```text
verify: byte 4506/4524  linked 4506/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas (fresh measurement, canonical build):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **84,960** |
| `.ARM.extab` | 156,312 | **1,404** |
| `.note.gnu.build-id` | 36 | **20** |
| `.rodata`, `.data`, `.bss`, `.ARM.exidx`, `.rel.plt`, `.dynsym`, `.dynstr` | exact | **0** |

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B, `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.

---

## 0. Bottom line

1. **The original was not built with whole-program `-fprofile-use`.** Of the
   **1,973** functions in the original that contain at least one conditional
   branch, **1,957 (99.19%) are byte-exact and linked-exact against our
   no-profile build**. Only the 16 known residual branch functions differ.
   A whole-program PGO build would attach `!prof` weights to every branch and
   perturb block placement/inlining in a large fraction of those 1,973
   functions. The original's branch layout is the *heuristic* layout in
   99.19% of branch-containing functions.
2. **A synthetic whole-tree `.profdata` is a large regression.** A uniform
   profile (all counters = 1) built from the instrumented tree's own
   `__profd__` records gives `byte_exact 1250/4524`, `linked_exact 3276`,
   `.text` 1,220,251 differing bytes.
3. **The Status 4:3:2:1 `.profdata` is also a regression.** The w7781 counter
   map, applied as a one-function text profile, reproduces the switch weights
   but gives `Status::missionCompleted` **17.9%** (1432 -> 1416 B) and
   `byte_exact 1421/4524` — exactly the w7781 result. `-fprofile-instr-use`
   marks every *unprofiled* function cold, so a partial profile cannot be
   scoped to one function.
4. **No build flag raises `byte_exact`.** A 60-variant full-gate matrix (below)
   finds no flag that beats the canonical 4506; every codegen-relevant flag is
   negative, and the neutral flags leave `.text` byte-identical. The canonical
   flag set is the original's.
5. **New confirmations from the APK/NDK re-mine.** The APK's
   `lib/armeabi-v7a/libgof2hdaa.so` is the same build as the ground truth
   (2,187,328 B, same `.comment`/`.note`/build-id; only the 288-byte
   `.shstrtab` differs, which `tools/reorder_shstrtab.py` restores). The NDK
   r18b CMake toolchain's compile flags are exactly
   `-g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
   -no-canonical-prefixes` plus `-Oz`; ndk-build would additionally add
   `-fdata-sections -fno-exceptions -fno-rtti`, all three disproven. The
   original keeps frame pointers at `-Oz -g` (clang 7 default), so
   `-fno-omit-frame-pointer` is a no-op and `-fomit-frame-pointer` is a
   regression.

**Verdict: not PGO, and no missing flag. The 18 residual functions are
source shape, not build configuration.**

---

## 1. Method

### 1.1 Branch-function census (the whole-binary `!prof` inspection)

The shipped `.so` is stripped, so the `!prof` metadata itself is not in the
binary. The observable is the *block layout* the profile produces. Because
the project already reproduces the original's function order and sizes, the
per-function byte comparison is a direct test of "same branch layout as the
heuristic build". I disassembled the original with the NDK objdump and counted
every function that contains a conditional branch (`b<cond>`, `cbz`, `cbnz`,
`tbz`, `tbnz`), then joined that set to `report.json`:

```text
functions in report:            4524
with ≥1 conditional branch:     1973
byte_exact + linked_exact:      1957   (99.19 %)
linked-residual:                  16   (the 18 residual symbols; C1/C2 aliases counted once)
```

The 16 residual branch functions are exactly the known list:

```text
Layout::Layout C1/C2
LevelScript::process
Level::createShip / createMission / createScene
Status::missionCompleted
Radar::draw
MGame::OnTouchEnd / OnUpdate
Gun::calcCharacterCollision
HangarWindow::render
SpaceLounge::drawLounge
Hud::draw
Generator::createMission
MenuTouchWindow::OnTouchBegin
ModStation::ModStation C1/C2
```

Every one of those TUs has 96–99% of its *other* functions byte-exact
(e.g. `Status.cpp` 146/147, `Level.cpp` 128/131, `MGame.cpp` 46/48,
`Layout.cpp` 51/53). A per-TU `-fprofile-use` would perturb the whole TU, not
one function per TU.

### 1.2 Synthetic profiles

The instrumented tree was built with
`-fprofile-instr-generate -mllvm -enable-name-compression=false`; the
`__llvm_prf_data` records (40-byte records: `NameRef` = MD5(name)[:8],
`FuncHash` @ +8, `NumCounters` @ +28) and the plain `__llvm_prf_names` blob
were parsed to emit a text profile for all 4,166 profiled functions. The text
format is the LLVM 7 `llvm-profdata merge -text` format:

```text
<name>
# Func Hash:
<hash>
# Num Counters:
<n>
# Counter Values:
<c0>
<c1>
...
```

`llvm-profdata merge` produced a valid 616,440-byte `.profdata`. The same
machinery produced the Status-only 4:3:2:1 profile from the w7781 counter map
(cases 0/8/11/13/14 -> counter 3; cases 167/174/150/151/152/153/154/155/158/
162/166/170/184/189 -> counter 2; cases 156/157/160/163/164/165/168/169/171/
172 -> counter 1; everything else 1).

### 1.3 Flag matrix

A scratch build (`/dev/shm/w8119/build`) uses a compiler launcher that appends
`$GOF2_EXTRA_FLAGS` after the canonical flags; each cell deletes all `.obj`s,
relinks the whole tree, and scores with a byte-only fast scorer that reproduces
`verify.py`'s `byte_exact` exactly (canonical 4506, 18 bad). Section diffs are
computed against the original with a section-header parser.

---

## 2. PGO measurements

| build | byte_exact | linked_exact | avg | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|---:|---:|---:|
| canonical (no profile) | **4506** | **4506** | 99.94 | **84,960** | **1,404** |
| uniform whole-tree profile (all counters 1) | 1250 | 3276 | 89.82 | 1,220,251 | 137,436 |
| Status-only 4:3:2:1 profile | 1421 | 4506 | 99.92 | 1,142,751 | 1,404 |

The Status-only profile reproduces the w7781 result exactly:
`Status::missionCompleted` **17.9%**, size **1432 -> 1416 B**. The 4:3:2:1
switch weights are correct, but `-fprofile-instr-use` treats every function not
in the profile as cold; all of `Status.cpp`'s other 146 functions change (and
the whole gate collapses to 1421). Even the `.ARM.extab` is unchanged for the
Status-only profile (1,404), i.e. the switch weights do not recover the
original's EH layout.

**Interpretation.** The 1,957/1,973 census is the decisive evidence. A
whole-program `-fprofile-use` build would have `!prof` weights on every branch
of all 1,973 functions; the probability that a real training profile reproduces
the static heuristic block order in 1,957 of them (and differs in exactly the
16 known source-shape residuals) is negligible. The original is a plain
`-Oz -g` build with heuristic branch weights. The Status 4:3:2:1 switch is a
source-shape effect (the w7781 open question), not a global PGO flag.

---

## 3. Full-gate flag matrix

All cells are a full link of the whole tree with only the named flag appended
to every compile. Canonical is `byte_exact 4506`, `.text` diff 84,960.

### 3.1 Neutral (byte_exact 4506, all sections byte-identical)

| flag | note |
|---|---|
| `-fno-omit-frame-pointer` | already the `-Oz -g` default (clang 7 keeps `r7`) |
| `-fmerge-all-constants` | `.rodata` still exact |
| `-fno-asynchronous-unwind-tables` | exceptions still emit sync tables |
| `-fno-standalone-debug` | debug info is stripped |
| `-fno-limit-debug-info` | debug info is stripped |
| `-ffp-contract=off` / `-ffp-contract=fast` | VFPv3 has no FMA |
| `-fno-trapping-math` | no trapping FP in the tree |
| `-fno-delete-null-pointer-checks` | no effect |
| `-fvectorize` / `-fno-vectorize` | loop vectorizer is off at `-Oz` |
| `-fno-merge-constants` | `.rodata` still exact |
| `-g3` / `-gdwarf-4` | debug info is stripped |
| `-fno-sized-deallocation` | no effect |
| `-fdebug-prefix-map=...` | debug info is stripped |
| `-no-canonical-prefixes` | NDK CMake toolchain flag; codegen-neutral |
| `-fno-unique-section-names` | no effect |
| `-fno-plt` | no effect |
| `-mno-movt` | no effect |
| `-fno-aligned-allocation` / `-fno-aligned-new` | no effect |
| `-fno-ident` | `.comment` still emitted |
| `-fno-signed-char` / `-funsigned-char` / `-fshort-wchar` | no effect |
| `-fno-lax-vector-conversions` | no effect |
| `-fno-operator-names` / `-fno-access-control` | no effect |
| `-fno-sanitize=all` | no effect |

### 3.2 Negative (byte_exact < 4506)

| flag | byte_exact | `.text` diff |
|---|---:|---:|
| `-fomit-frame-pointer` | 1298 | 1,224,750 |
| `-fno-unwind-tables` | 2878 | 100,908 |
| `-gline-tables-only` / `-g1` | 2071 | 804,727 |
| `-fno-math-errno` | 1356 | 1,150,704 |
| `-fno-strict-aliasing` | 1371 | 1,193,927 |
| `-fwrapv` | 1418 | 1,097,492 |
| `-fvisibility=hidden` | 57 | 1,225,448 |
| `-fno-exceptions` | 1344 | 1,223,195 |
| `-fno-threadsafe-statics` | 1368 | 1,100,877 |
| `-fno-rtti` | 1701 | 176,812 |
| `-fno-stack-protector` | 1369 | 1,223,560 |
| `-mno-unaligned-access` | 1367 | 1,188,903 |
| `-fno-builtin` | 1350 | 1,209,176 |
| `-fno-inline` / `-fno-inline-functions` | 1219 | 1,260,922 |
| `-fno-slp-vectorize` | 1484 | 1,190,071 |
| `-fshort-enums` | 2870 | 851,600 |
| `-fno-jump-tables` | 1390 | 1,215,637 |
| `-fno-optimize-sibling-calls` | 1316 | 1,212,288 |
| `-fno-use-cxa-atexit` | 1356 | 1,184,219 |
| `-fno-elide-constructors` | 1370 | 1,242,261 |
| `-fstack-protector` | 1369 | 1,223,592 |
| `-fstack-protector-all` | 83 | 1,341,894 |
| `-fno-zero-initialized-in-bss` | 1522 | 1,157,025 |
| `-fno-builtin-memcpy` | 1359 | 1,204,145 |
| `-fno-builtin-memset` | 1351 | 1,207,105 |
| `-ffreestanding` | 1350 | 1,209,176 |
| `-fno-common` | **link error** (duplicate `g_android_back_button_pressed`) | — |
| `-gdwarf-5` | build error (clang 7) | — |
| `-mvectorize-with-neon-quad` | unsupported by clang 7 | — |

The `-fno-common` link error is a useful positive: the original was compiled
with the clang 7 default `-fcommon` (tentative definitions in
`BackButton.c`/`GOF2HD2012.c` merge).

**No cell beats 4506.** The canonical flag set is the original's, and the
remaining 18 functions cannot be recovered by any flag.

---

## 4. APK / NDK re-mine (new confirmations)

### 4.1 APK

`/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`
contains exactly four `lib/armeabi-v7a/*.so`: `libgof2hdaa.so`,
`libcrashlytics.so`, `libfmodex.so`, `libfmodevent.so`. The APK's
`libgof2hdaa.so` is 2,187,328 B, `sha256 6d38fd76...`; it differs from the
ground truth in **288 bytes**, all in `.shstrtab` (bfd section-header order vs
gold Stringpool order), which `tools/reorder_shstrtab.py` restores. `.comment`
is exactly the NDK r18b clang 7.0.2 string plus the OpenSSL GCC 4.9.x string;
`.note.android.ident` decodes to API 16 / `r18b` / `5063045`;
`.note.gnu.gold-version` is `gold 1.12`; build-id `c011810d...`. No
`Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.mk`, `*.gradle`,
`*.ninja`, `compile_commands.json`, `*.o`, `*.a`, `*.d`, `*.profdata` or
`*.profraw` is in the APK. The OBB is game content only.

### 4.2 NDK r18b CMake vs ndk-build

`build/cmake/android.toolchain.cmake`:

```cmake
list(APPEND ANDROID_COMPILER_FLAGS
  -g -DANDROID -ffunction-sections -funwind-tables
  -fstack-protector-strong -no-canonical-prefixes)
# release: -Oz for armeabi + thumb
```

`build/core/default-build-commands.mk`:

```make
GLOBAL_CFLAGS  = ... -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables ...
GLOBAL_CXXFLAGS = $(GLOBAL_CFLAGS) -fno-exceptions -fno-rtti
```

So ndk-build would add `-fdata-sections`, `-fno-exceptions` and `-fno-rtti`;
all three are disproven by the full-gate matrix (4315 / 1344 / 1701). The CMake
toolchain does not add them, which is why the canonical build matches. The
`-no-canonical-prefixes` flag is codegen-neutral (tested). The original keeps
frame pointers: clang 7 at `-Oz -g` emits `push {r4, r6, r7, lr}; add r7, sp,
#8` for a simple function, so `-fno-omit-frame-pointer` is already the default
and `-fomit-frame-pointer` is a regression.

### 4.3 What is still unobservable

The exact CMake version/generator, host OS, build directory and whether the
build used `CMakeLists.txt` or `externalNativeBuild` are not embedded in the
stripped `.so`. They are codegen-neutral for this tree.

---

## 5. Reproduction

```sh
# canonical gate
cd /home/debian/GalaxyOnFire2/wt/w8119
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# branch-function census
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --no-show-raw-insn /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w8119/orig.dis
python3 - <<'PY'
import re, json
blocks, cur = {}, None
for line in open('/dev/shm/w8119/orig.dis'):
    m = re.match(r'^[0-9a-f]+ <(.+)>:$', line)
    if m:
        cur = m.group(1).replace('@@Base', '').replace('@plt', '')
        blocks.setdefault(cur, [])
        continue
    if cur is not None:
        blocks[cur].append(line)
cond = re.compile(r'\b(b(eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le)|cbz|cbnz|tbz|tbnz)\b')
r = json.load(open('report.json'))
status = {x['name']: x for x in r['rows']}
nb = be = le = 0
bad = []
for name, lines in blocks.items():
    if name not in status:
        continue
    if any(cond.search(l) for l in lines):
        nb += 1
        be += status[name]['byte']
        le += status[name]['linked']
        if not status[name]['linked']:
            bad.append(name)
print(f'branch functions {nb}, byte_exact {be}, linked_exact {le}, residual {len(bad)}')
print(bad)
PY

# synthetic PGO
cmake -S . -B /dev/shm/w8119/build_instr -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w8119/launch.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w8119/launch.sh
GOF2_EXTRA_FLAGS="-fprofile-instr-generate -mllvm -enable-name-compression=false" \
  cmake --build /dev/shm/w8119/build_instr --target gof2 -j6
python3 /dev/shm/w8119/mkprof.py       # writes alluniform.proftext from __llvm_prf_data
llvm-profdata merge /dev/shm/w8119/alluniform3.proftext -o /dev/shm/w8119/alluniform.profdata
GOF2_EXTRA_FLAGS="-fprofile-instr-use=/dev/shm/w8119/alluniform.profdata" \
  cmake --build /dev/shm/w8119/build --target link_so -j6

# flag matrix (one cell)
GOF2_EXTRA_FLAGS="-fno-omit-frame-pointer" \
  cmake --build /dev/shm/w8119/build --target link_so -j6
python3 /dev/shm/w8119/score.py /dev/shm/w8119/build/libgof2hdaa.so
```

Scratch (not committed): `/dev/shm/w8119/` — `launch.sh`, `matrix.sh`,
`score.py`, `sec.py`, `mkprof.py`, `orig.dis`, `alluniform*.proftext`,
`alluniform.profdata`, `status4321.proftext`, `status4321.profdata`,
`uniform.so`, `status4321.so`, `matrix1.txt`, `matrix2.txt`, `matrix3.txt`.

---

## 6. What would falsify this note

* A whole-program `-fprofile-use` build whose `.text` matches the original
  better than 4506 while keeping the 1,957 branch-function matches. The
  uniform profile drops to 1250 and the Status-only profile to 1421.
* A flag that raises `byte_exact` above 4506 without changing `.text` size.
  None of the 60+ tested cells does.
* Evidence that the original's `!prof` weights are empirical rather than
  heuristic. The 1,957/1,973 heuristic-layout match is the current evidence
  against it; the Status 4:3:2:1 switch is the one known exception and is
  being pursued as a source-shape question (w7781 open questions: a PHI of
  `__builtin_expect` values, or a GNU case-range + `__builtin_expect`).
