# Build metadata and preprocessor macros (w5220)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5220`,
branch `wave104-w5220`, base `8c5fd3646`. Every number below is from a command run in
this worktree against `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
or from a controlled scratch rebuild of this tree. **No source, build-flag, baseline, tool,
or allowlist change was landed: no tested variant is a strict improvement.** This commit
contains only this note.

## 0. Bottom line

* Canonical baseline is green: **byte_exact 4500/4524, linked_exact 4500/4524,
  size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0**.
  `.text` is `0x1337c8` at `0x69440`; the original and our build have the same 27
  section headers and the same `.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
  `.ARM.attributes`, and `.shstrtab` bytes.
* The tested flags split into three classes:
  1. **Codegen/metadata-neutral** (full stripped `.so` byte-identical to a scratch
     baseline built in the same directory, including build-id): `-DANDROID` removal,
     `-DNDEBUG` removal, `-D_FORTIFY_SOURCE=2` at API 16, `-DDEBUG`, `-D_DEBUG`,
     `-fident`, `-fno-ident`, `-gcc-toolchain`, `-Wa,--noexecstack`, `-Wformat`,
     `-Werror=format-security`, `-Qn`.
  2. **Driver/debug-metadata-only** (final stripped content identical except
     `.note.gnu.build-id`): `-D__ANDROID_API__=21`, `-grecord-gcc-switches`,
     `-no-canonical-prefixes`, and the metadata bundle. The build-id changes because
     the original build-id is the SHA-1 of the *unstripped* gold output; the debug
     sections differ but are removed by `strip --strip-all`.
  3. **Real regression**: `-D__ANDROID_API__=21 -D_FORTIFY_SOURCE=2` together.
     At API 21 bionic's FORTIFY inline overloads become active; the gate collapses to
     **byte 1372, linked 4461, size 4500/4524**, imports rise 207 -> 210
     (`__memset_chk`, `__strcpy_chk`, `__strlen_chk`), and 3,128 functions change.
     `-D_FORTIFY_SOURCE=2` alone is inert at API 16 because the bionic string/stdio
     FORTIFY overloads are gated on `__ANDROID_API__ >= 17` (`J_MR1`).
* `-frecord-gcc-switches` is **not accepted by clang 7.0.2**:
  `error: unknown argument '-frecord-gcc-switches', did you mean '-grecord-gcc-switches'?`
  The nearest accepted flag, `-grecord-gcc-switches`, is debug-info-only here.
* No strict improvement exists. The current `-DANDROID -DNDEBUG -D__ANDROID_API__=16`
  set is consistent with the original, but the three macros are codegen-neutral on this
  tree; they cannot be distinguished from an original build that omitted them.

## 1. Baseline and exact commands

Canonical configure/build (the worktree's default original path is wrong for a nested
worktree, so the absolute path is required):

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

Raw output:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Original:

```text
size 2,187,328
sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483
build-id c011810da45777b83ed721b2a87d517efed0d310
```

Canonical `.so`:

```text
size 2,187,328
sha256 e467f853436ac657ebea56eb2b9fc8e163987739a977351ea4be70e1d9611476
build-id a7d63c00757c04f2... (full bytes in §5)
```

Artifact caveat: the tracked repo-root `report.json` at the base commit is stale
relative to `cmake-build-match/verify/report.json` because `ratchet.py` runs
`verify.py` directly and does not perform the `verify` target's copy step. The gate
counts are the same, but the fresh report has `Generator::createMission` 77.5,
`HangarWindow::render` 84.4, and `Hud::draw` 92.9 (the repo-root snapshot has 64.4,
80.2, 92.1). All A/B comparisons in this note use the fresh
`cmake-build-match/verify/report.json`.

The scratch A/B used a wrapper that includes the canonical toolchain, optionally
removes exact tokens, optionally overrides `-D__ANDROID_API__=16`, and appends extras
to both C and C++ flags:

```cmake
# /dev/shm/w5220/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w5220/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
set(GOF2_REMOVE_FLAGS "" CACHE STRING "semicolon-separated exact tokens to remove")
set(GOF2_API_MACRO "" CACHE STRING "override -D__ANDROID_API__ value")
set(_w5220_cxx "${CMAKE_CXX_FLAGS}")
set(_w5220_c   "${CMAKE_C_FLAGS}")
if(GOF2_API_MACRO)
  string(REPLACE "-D__ANDROID_API__=16" "-D__ANDROID_API__=${GOF2_API_MACRO}" _w5220_cxx "${_w5220_cxx}")
  string(REPLACE "-D__ANDROID_API__=16" "-D__ANDROID_API__=${GOF2_API_MACRO}" _w5220_c   "${_w5220_c}")
endif()
foreach(_tok IN LISTS GOF2_REMOVE_FLAGS)
  string(REPLACE " ${_tok}" "" _w5220_cxx "${_w5220_cxx}")
  string(REPLACE " ${_tok}" "" _w5220_c   "${_w5220_c}")
endforeach()
if(GOF2_EXTRA_FLAGS)
  set(_w5220_cxx "${_w5220_cxx} ${GOF2_EXTRA_FLAGS}")
  set(_w5220_c   "${_w5220_c} ${GOF2_EXTRA_FLAGS}")
endif()
set(CMAKE_CXX_FLAGS "${_w5220_cxx}" CACHE STRING "matching ARM compile flags" FORCE)
set(CMAKE_C_FLAGS   "${_w5220_c}"   CACHE STRING "matching ARM C compile flags" FORCE)
```

Each variant was configured in the same scratch build directory
(`/dev/shm/w5220/build`), linked with `link_so -j6`, verified with
`tools/ratchet.py --build-dir /dev/shm/w5220/build`, and its `.so` was copied to
`/dev/shm/w5220/variants/<name>.so`. The scratch baseline (no extras, no removals,
API 16) was built in the same directory to remove the build-directory path effect on
debug info/build-id.

Because the scratch build directory is `/dev/shm/w5220/build` rather than
`cmake-build-match`, the scratch baseline build-id is `c1b00cd0...`, not the canonical
`a7d63c00...`. Its stripped content sections are otherwise identical to the canonical
build. All variant comparisons below are against the **scratch baseline**, not the
canonical build-id.

## 2. Source macro audit

`grep -RInE '^\s*#\s*(if|ifdef|ifndef|elif)' src ...` shows that `src/` has no
`ANDROID`, `NDEBUG`, `DEBUG`, `_DEBUG`, `_FORTIFY_SOURCE`, or `__ANDROID_API__`
conditionals. The only non-include-guard conditionals in `src/` are:

| macro | sites | effect |
|---|---|---|
| `__SIZEOF_POINTER__` | many headers/`.cpp` | layout/ABI static checks and 32-bit branches; compiler builtin, already 4 |
| `__cplusplus` | a few C/C++ boundary headers | C++ declarations only |
| `GOF2_MATCH` | `Transform.h`, `KeyFrame.h` | static_asserts only; **not defined by the build** (see `tools/drift_scan.py`) |
| `GOF2_ENUM_BlendMode` | `PaintCanvas.h`, particle headers | include guard for a shared enum |
| `GOF2_ARRAY_INSTANTIATIONS` | `Array.cpp` | explicit-instantiation TU switch |

The tested macros are therefore mostly observable only through NDK/libc++ headers or
vendored code:

| macro | live conditionals outside `src/` | consequence |
|---|---|---|
| `ANDROID` | NDK `KHR/khrplatform.h`, `EGL/eglplatform.h`; vendored GL uses `__ANDROID__` | no code change on this tree (full `.so` identical) |
| `NDEBUG` | NDK `assert.h`; no `assert(` in `src/` | no code change |
| `DEBUG` | zlib `deflate.c`, `trees.c`, `zutil.c`, `deflate.h`, `zutil.h`; OpenSSL `cryptlib.c` | app-only neutral; zlib object code changes (below) |
| `_DEBUG` | OpenSSL comments/`BN_DEBUG[_DEBUG]`; no zlib use | app-only neutral; zlib objects identical |
| `_FORTIFY_SOURCE` | NDK `sys/cdefs.h` + `bits/fortify/*` | inert at API 16; active at API >= 17 |
| `__ANDROID_API__` | NDK feature/`__INTRODUCED_IN` gates, including FORTIFY | API 21 alone debug-info-only; API 21 + FORTIFY changes code |

## 3. Object-level A/B (representative TUs)

`Level.cpp` was compiled with the exact `compile_commands.json` command, replacing only
`-o` and appending the tested flag. `main.c` was used for the C `-fident`/`-fno-ident`/
`-Qn` check. Raw object hashes (SHA-256 prefix) and sizes:

| variant | `Level.cpp.obj` sha256 | size | `.text.*` aggregate sha256 | note |
|---|---|---:|---|---|
| base | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | |
| no `-DANDROID` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| no `-DNDEBUG` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-D__ANDROID_API__=21` | `3512bac7e564a0aa` | 1,017,700 | `b63a4f2c8d6aa9d0` | debug sections only |
| `-D_FORTIFY_SOURCE=2` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical at API 16 |
| `-DDEBUG` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-D_DEBUG` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-fident` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-fno-ident` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical; `.comment` unchanged |
| `-frecord-gcc-switches` | — | — | — | clang 7.0.2 rejects the flag |
| `-grecord-gcc-switches` | `3d3099fd0e250b40` | 1,019,760 | `b63a4f2c8d6aa9d0` | `.debug_str` only |
| `-no-canonical-prefixes` | `fec963ed9ee1c4e5` | 1,018,252 | `b63a4f2c8d6aa9d0` | `.debug_line` only |
| `-gcc-toolchain ...` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-Wa,--noexecstack` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-Wformat` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-Werror=format-security` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical |
| `-Qn` | `cd891b7ac30598f3` | 1,018,252 | `b63a4f2c8d6aa9d0` | identical; driver warns "argument unused" |
| metadata bundle | `fec963ed9ee1c4e5` | 1,018,252 | `b63a4f2c8d6aa9d0` | same as `-no-canonical-prefixes` |

`main.c` object sha256 was `ad00784c1500cb9d` for base, `-fident`, `-fno-ident`, and
`-Qn`; `.comment` was the same clang string in all four.

Object-level section deltas:

* `-D__ANDROID_API__=21`: `.debug_abbrev`, `.debug_info`, `.debug_line`, `.debug_pubnames`,
  `.debug_pubtypes`, `.debug_str`, and debug relocations differ; `.text.*`, `.comment`,
  `.ARM.attributes`, and `.note.GNU-stack` are identical.
* `-grecord-gcc-switches`: only `.debug_str` and `.symtab` differ.
* `-no-canonical-prefixes`: only `.debug_line` and `.rel.debug_line` differ.
* `-fno-ident` / `-fident` / `-Qn`: full object identical.

## 4. Full-link A/B results

All variants used the scratch baseline build directory. `size` is `osize == usize` over
the 4,524 compared functions. `section diffs` is the number of sections whose
(name, size, address, SHA-256) differ from the scratch baseline.

| variant | byte | linked | size | data | imports | `.so` sha256 | section diffs |
|---|---:|---:|---:|---:|---:|---|---:|
| scratch baseline | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| no `-DANDROID` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| no `-DNDEBUG` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-D__ANDROID_API__=21` | 4500 | 4500 | 4524/4524 | 843 | 207 | `03ab7a03f6337272...` | 1 (build-id) |
| `-D_FORTIFY_SOURCE=2` (API 16) | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-DDEBUG` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-D_DEBUG` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-fident` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-fno-ident` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-frecord-gcc-switches` | — | — | — | — | — | build failed | — |
| `-grecord-gcc-switches` | 4500 | 4500 | 4524/4524 | 843 | 207 | `b023ea3bdb225dec...` | 1 (build-id) |
| `-no-canonical-prefixes` | 4500 | 4500 | 4524/4524 | 843 | 207 | `7bad3ba10e3f78eb...` | 1 (build-id) |
| `-gcc-toolchain ...` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-Wa,--noexecstack` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-Wformat` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-Werror=format-security` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| `-Qn` | 4500 | 4500 | 4524/4524 | 843 | 207 | `477145864baf22ce...` | 0 |
| metadata bundle | 4500 | 4500 | 4524/4524 | 843 | 207 | `7bad3ba10e3f78eb...` | 1 (build-id) |
| API 21 + FORTIFY 2 | **1372** | **4461** | **4500/4524** | 843 | **210** | `194b1c4150ab7c99...` | many (see §6) |

`lint` stayed `CLEAN ✓` for every successful variant. `drift` is source/header-only and
was re-run on the canonical tree: `=== DRIFT: 0 field(s) across 0 class(es) ===`.

Full build-id values for the metadata-observable variants:

```text
scratch baseline          c1b00cd0eed0f182e5d5b3ce7652d0f2187e776ac857964c6af13aa503886215
-D__ANDROID_API__=21      64bdd753a2ea934ca38aea0445d707039ad0c0f54d43016a04ac819acffea93d
-grecord-gcc-switches     49c0c6a808bf42c5965a08b052e0fc54a777959e53dc6366308370b44c585806
-no-canonical-prefixes    5ab54624e1124e65432234539d45b686b63d146c32b53b0ba7990a5f17e7ad02
metadata bundle           5ab54624e1124e65432234539d45b686b63d146c32b53b0ba7990a5f17e7ad02
API 21 + FORTIFY 2       c1d29a6d17f8720c606f1b42b3ef0ff6db8f8164f4bba62062b9a317e7e56fb0
```

## 5. `.comment`, notes, `.ARM.attributes`, sections

The original and canonical baseline have byte-identical metadata sections:

| section | original SHA-256 | canonical ours SHA-256 | status |
|---|---|---|---|
| `.comment` | `d4f577f46aa5f132...` | `d4f577f46aa5f132...` | exact |
| `.note.android.ident` | `522bf4171215935b...` | `522bf4171215935b...` | exact (API 16, r18b, 5063045) |
| `.note.gnu.gold-version` | `47f758e9dbe91045...` | `47f758e9dbe91045...` | exact (gold 1.12) |
| `.ARM.attributes` | `b03afea6aa56c9ca...` | `b03afea6aa56c9ca...` | exact |
| `.shstrtab` | `c34e875ec1fd5096...` | `c34e875ec1fd5096...` | exact |
| `.note.gnu.build-id` | `375800e6e941c456...` | `a7d63c00757c04f2...` | differs (expected) |

`.comment` is the NDK r18b clang 7.0.2 string plus the GCC 4.9.x string from the
crt/libgcc objects. `-fident` and `-fno-ident` do not change it: clang 7.0.2's
`-###` output shows neither flag reaching `-cc1`, and the app object `.comment`
is unchanged. `-Qn` is accepted but emits `argument unused during compilation:
'-Qn' [-Wunused-command-line-argument]` and does not change the object.

For every neutral variant the 27 section headers are byte-identical to the scratch
baseline, and all section contents are identical. For `-D__ANDROID_API__=21`,
`-grecord-gcc-switches`, `-no-canonical-prefixes`, and the metadata bundle, the only
differing section is `.note.gnu.build-id`; `.text`, `.rodata`, `.data`, `.bss`,
`.ARM.exidx`, `.ARM.extab`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.rel.plt`,
`.plt`, `.got`, `.dynamic`, `.init_array`, `.fini_array`, `.comment`, and
`.ARM.attributes` are byte-identical to the scratch baseline.

Canonical baseline section facts (ours vs original):

```text
.text        ours 0x1337c8 @ 0x69440   original 0x1337c8 @ 0x69440   content differs
.rodata      ours 0x41b67  @ 0x1c9e10  original 0x41b67  @ 0x1c9e10  exact
.data        ours 0x296c   @ 0x214000  original 0x296c   @ 0x214000  exact
.bss         ours 0x69ac   @ 0x216970  original 0x69ac   @ 0x216970  exact
.ARM.exidx   ours 0x6f70   @ 0x19cc08  original 0x6f70   @ 0x19cc08  content differs
.ARM.extab   ours 0x26298  @ 0x1a3b78  original 0x26298  @ 0x1a3b78  content differs
```

## 6. The API 21 + FORTIFY interaction (the only real regression)

`-D_FORTIFY_SOURCE=2` alone is inert because the original target is API 16. The bionic
FORTIFY inline overloads in `bits/fortify/string.h` and `bits/fortify/stdio.h` are
gated on `__ANDROID_API__ >= __ANDROID_API_J_MR1__`, and
`__ANDROID_API_J_MR1__ == 17` (`android/api-level.h`). A minimal probe confirms it:

```sh
# probe.c: void probe(const char *s) { char d[8]; memset(d,0,sizeof d); strcpy(d,s); (void)strlen(d); }
clang --target=armv7a-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -Oz -fPIC -ffunction-sections \
  --sysroot=$NDK/sysroot -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -D__ANDROID_API__=<api> [-D_FORTIFY_SOURCE=2] -c probe.c -o probe.o
```

Relocation results:

| API | FORTIFY | call target |
|---:|---:|---|
| 16 | off | `strcpy` |
| 16 | 2 | `strcpy` (inert) |
| 17 | off | `strcpy` |
| 17 | 2 | `__strcpy_chk`, `__strlen_chk` |
| 21 | off | `strcpy` |
| 21 | 2 | `__strcpy_chk`, `__strlen_chk` |

Full-link API 21 + FORTIFY 2:

```text
verify: byte 1372  linked 4461  stubs 0  missing 0  extra 0  avg 99.7
size-exact: 4500 / 4524
data: exact 843 of 843
imports: 210  (new: __memset_chk, __strcpy_chk, __strlen_chk)
changed functions: 3128
```

Section changes vs scratch baseline:

```text
.text        0x1337c8 -> 0x133838  (+0x70)
.ARM.exidx   0x6f70   -> 0x6f78    (+8)
.ARM.extab   0x26298  -> 0x262a0   (+8)
.dynsym      0x15ca0  -> 0x15cd0   (+0x30)
.dynstr      0x2e318  -> 0x2e33f   (+0x27)
.hash        0x973c   -> 0x9748    (+0xc)
.rel.plt     0x72d0   -> 0x72e8    (+0x18)
.plt         0xac4c   -> 0xac70    (+0x24)
.got         0x430c   -> 0x4318    (+0xc)
.rodata      same size, content changed
.data        same size, content changed
.bss         same size, content changed
```

Representative changed functions (original `100.0%` -> API21+FORTIFY):

```text
PaintCanvas::MeshChangeShaderAnimValue(Mesh*, float, unsigned)   46 -> 84 B   19.6%
ArrayAddCached<Matrix>                                          184 -> 172 B  29.2%
PaintCanvas::Initialize(bool)                                   256 -> 252 B  41.4%
PaintCanvas::SetGameOrientation(LandscapeMode)                  704 -> 712 B  43.7%
LodMeshMerger::LodMeshMerger(int,int,PaintCanvas*,unsigned short) 428 -> 428 B 48.7%
ArrayAddCached<Mesh*>                                            68 -> 58 B   49.1%
ArrayAddCached<unsigned>                                         70 -> 60 B   50.9%
ArrayRemoveAll<T>                                                34 -> 40 B   75.0%
ndk23_setRootDirectory                                           40 -> 36 B   75.9%
MeshCreate(Engine*, unsigned short, unsigned short, Mesh**)     308 -> 312 B  90.0%
```

The `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, and `.ARM.attributes`
sections remain byte-identical even in this variant; the regression is code/data, not
toolchain metadata.

## 7. zlib `DEBUG` probe (why `-DDEBUG` is not a global original flag)

The app TUs have no `DEBUG` conditionals, so `-DDEBUG` is neutral when applied only to
`src/`. The vendored zlib 1.2.3 does have live `#ifdef DEBUG` code. Compiling the exact
zlib units with the `build_match_lib.cmake` flags plus `-DDEBUG` gives:

| unit | base object | `-DDEBUG` object | `-D_DEBUG` object |
|---|---|---|---|
| `deflate.c` | 45,764 B `8364b0dabc397ad3` | 49,760 B `1577eec98e27e4ea` | identical to base |
| `trees.c` | 48,612 B `0e709c23c81f5345` | 44,244 B `f4e01be7837d52e8` | identical to base |
| `zutil.c` | 6,704 B `9618b0adc97db7d3` | 7,624 B `a293168671c95aa3` | identical to base |

All 41 zlib functions in the canonical verify report are `byte=True, linked=True,
100.0%`. Therefore the original zlib was **not** built with `-DDEBUG`; `-D_DEBUG` has
no zlib effect. `-D_FORTIFY_SOURCE=2` at API 16 also leaves these zlib objects identical.

## 8. Proven, disproven, unknown

### Positively proven

1. Canonical baseline is 4500/4500/4524, data 843, imports 207, lint CLEAN, drift 0.
2. Removing `-DANDROID` or `-DNDEBUG` leaves the full stripped `.so` byte-identical to
   the scratch baseline (including build-id).
3. `-DDEBUG` and `-D_DEBUG` are app-neutral; zlib `DEBUG` is live and would change
   zlib, while `_DEBUG` does not.
4. `-D_FORTIFY_SOURCE=2` is inert at API 16; it is active at API >= 17 and is a severe
   regression at API 21 (1372/4461, 3 new imports).
5. `-D__ANDROID_API__=21` alone is code-neutral; only debug info and the resulting
   build-id change.
6. `-fident` and `-fno-ident` are no-ops for clang 7.0.2 here; `.comment` is unchanged.
7. `-frecord-gcc-switches` is rejected by clang 7.0.2; `-grecord-gcc-switches` is
   debug-info-only.
8. `-no-canonical-prefixes` is driver/debug-line-only; final content is identical
   except build-id.
9. `-gcc-toolchain`, `-Wa,--noexecstack`, `-Wformat`, `-Werror=format-security`, and
   `-Qn` are codegen-neutral on this tree.
10. `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`,
    and `.shstrtab` are byte-identical to the original.

### Disproven

1. `-D_FORTIFY_SOURCE=2` is not a strict improvement at API 16 and is a large
   regression at API 21.
2. `-D__ANDROID_API__=21` is not a codegen improvement.
3. `-DDEBUG`/`-D_DEBUG` are not app codegen knobs; `-DDEBUG` would break zlib.
4. `-frecord-gcc-switches` is not usable with clang 7.0.2.
5. `-fno-ident` does not remove the clang `.comment` string.
6. `-Qn`, `-Wformat`, `-Werror=format-security`, `-gcc-toolchain`, and
   `-Wa,--noexecstack` are not missing codegen flags.
7. `-DANDROID`/`-DNDEBUG` are not codegen knobs on this tree.

### Unobservable / unknown

1. Any neutral flag could have been on the original command line; a stripped `.so`
   cannot distinguish it. `-DANDROID`, `-DNDEBUG`, `-DDEBUG`, `-D_DEBUG`, `-fident`,
   `-fno-ident`, `-Qn`, `-Wformat`, `-Werror=format-security`, `-gcc-toolchain`, and
   `-Wa,--noexecstack` are all in this class.
2. The original build directory is not recoverable. Debug-info-only flags change the
   unstripped output and therefore the build-id, but the stripped content is identical.
3. Only the `-D__ANDROID_API__` macro was overridden; a full API 21 target/crtbegin
   would also change `.note.android.ident` and was not tested because the original note
   is API 16.
4. Vendored libs have separate flag lists in `third_party/build_match_lib.cmake`; the
   full-link A/B above covers app TUs, with direct zlib probes for `DEBUG`/`_DEBUG`/
   `FORTIFY`.
5. `-frecord-gcc-switches` cannot be tested further with this compiler; only the clang
   spelling `-grecord-gcc-switches` is available.

## 9. Confidence table

| Claim | Confidence | Hard evidence |
|---|---|---|
| Baseline 4500/4500/4524, data 843, imports 207, lint CLEAN, drift 0 | high | `check` and `drift` output in §1 |
| `-DANDROID` removal neutral | high | full `.so` sha `477145864baf22ce...`, 0 section diffs |
| `-DNDEBUG` removal neutral | high | full `.so` sha `477145864baf22ce...`, 0 section diffs |
| `-D_FORTIFY_SOURCE=2` inert at API 16 | high | full `.so` sha `477145864baf22ce...`, probe `strcpy` |
| `-D_FORTIFY_SOURCE=2` active at API >= 17 | high | probe `__strcpy_chk`/`__strlen_chk`; API21 full link 1372/4461 |
| `-D__ANDROID_API__=21` alone code-neutral | high | full `.so` only build-id differs; `.text` hash identical |
| `-DDEBUG` app-neutral | high | full `.so` sha `477145864baf22ce...`; no `DEBUG` in `src/` |
| `-DDEBUG` would change zlib | high | zlib object hashes/sizes in §7 |
| `-D_DEBUG` app/zlib-neutral | high | full `.so` identical; zlib objects identical |
| `-fident`/`-fno-ident` no-op | high | object and full `.so` identical; `.comment` unchanged |
| `-frecord-gcc-switches` unsupported | high | clang error text |
| `-grecord-gcc-switches` debug-only | high | object `.debug_str` only; full `.so` only build-id |
| `-no-canonical-prefixes` debug-line-only | high | object `.debug_line` only; full `.so` only build-id |
| `-gcc-toolchain`/`-Wa,--noexecstack`/`-Wformat`/`-Werror=format-security`/`-Qn` neutral | high | full `.so` sha `477145864baf22ce...` |
| `.comment`/notes/`.ARM.attributes` exact | high | section SHA-256 in §5 |
| Original command line recoverable | low | stripped `.so`; neutral flags unobservable |

## 10. Strict improvement

**None.** No tested variant raises `byte_exact`, `linked_exact`, `size-exact`,
`data_exact`, or lowers imports. The neutral variants are exactly neutral; the
debug-info-only variants differ only in build-id; API21+FORTIFY is a large regression.
No source or build change is landed, and no baseline update is warranted.

## 11. Reproduction summary

```sh
# canonical baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6

# one scratch variant (repeat with each flag list)
cmake -S /home/debian/GalaxyOnFire2/wt/w5220 -B /dev/shm/w5220/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5220/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flags>' \
  -DGOF2_REMOVE_FLAGS='<tokens>' -DGOF2_API_MACRO='<16|21|>'
cmake --build /dev/shm/w5220/build --target link_so -j6
python3 tools/ratchet.py --build-dir /dev/shm/w5220/build
python3 /dev/shm/w5220/elfmeta.py /dev/shm/w5220/build/libgof2hdaa.so

# API 21 + FORTIFY interaction
# same configure with -DGOF2_API_MACRO=21 -DGOF2_EXTRA_FLAGS='-D_FORTIFY_SOURCE=2'
```

The scratch helper `/dev/shm/w5220/elfmeta.py` is a small ELF32 parser that prints
each section's name/type/flags/address/offset/size/entsize and SHA-256; it was used for
the section tables above. The zlib probe used the exact `build_match_lib.cmake` zlib
flags plus `-DDEBUG`, `-D_DEBUG`, or `-D_FORTIFY_SOURCE=2`.

## Appendix A. Scratch ELF section dumper

The section tables were produced with this self-contained ELF32 LE parser
(`/dev/shm/w5220/elfmeta.py`):

```python
#!/usr/bin/env python3
import hashlib, json, struct, sys
from pathlib import Path

def read(path):
    d = Path(path).read_bytes()
    assert d[:4] == b'\x7fELF' and d[4] == 1 and d[5] == 1, path
    e_shoff = struct.unpack_from('<I', d, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', d, 0x2e)[0]
    e_shnum = struct.unpack_from('<H', d, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', d, 0x32)[0]
    sh = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        vals = struct.unpack_from('<IIIIIIIIII', d, off)
        sh.append(dict(i=i, name_off=vals[0], type=vals[1], flags=vals[2],
                       addr=vals[3], off=vals[4], size=vals[5], link=vals[6],
                       info=vals[7], addralign=vals[8], entsize=vals[9]))
    strtab = sh[e_shstrndx]
    def cstr(n):
        e = d.index(b'\0', strtab['off'] + n)
        return d[strtab['off'] + n:e].decode()
    for s in sh:
        s['name'] = cstr(s['name_off'])
        b = d[s['off']:s['off'] + s['size']]
        s['sha256'] = hashlib.sha256(b).hexdigest()
    return dict(path=str(path), size=len(d), sha256=hashlib.sha256(d).hexdigest(),
                shoff=e_shoff, shnum=e_shnum, sections=sh)

out = {p: read(p) for p in sys.argv[1:]}
print(json.dumps(out, indent=1))
```

## Appendix B. Exact zlib probe command

The zlib object probe used the same flags as `third_party/build_match_lib.cmake`
for the zlib branch, with the tested macro appended:

```sh
NDK=/home/debian/GalaxyOnFire2/wt/w5220/.cache/ndk/android-ndk-r18b
CLANG=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
SRC=/home/debian/GalaxyOnFire2/wt/w5220/third_party/zlib/zlib-1.2.3
for u in deflate trees zutil; do
  "$CLANG" --target=armv7a-linux-androideabi16 -mfpu=neon -mfloat-abi=softfp \
    --sysroot=$NDK/platforms/android-16/arch-arm \
    -isystem $NDK/sysroot/usr/include \
    -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
    -fPIC -ffunction-sections -funwind-tables -Wno-everything \
    -Oz -g -fstack-protector-strong -I. -mthumb \
    -DDEBUG -c "$SRC/$u.c" -o "/dev/shm/w5220/zlib_ab/$u-DEBUG.o"
done
```

The `-D_DEBUG` and `-D_FORTIFY_SOURCE=2` probes were the same command with the
corresponding macro substituted.
