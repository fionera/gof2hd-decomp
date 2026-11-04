# Compiler-generated pattern audit (w5214, wave 104)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5214`,
branch `wave104-w5214`, base `8c5fd3646`. Scratch is `/dev/shm/w5214`.
No `src/`, build-flag, tool, baseline or allowlist change is landed: **no tested
flag is a strict improvement**. This commit contains only this note.

The goal is an inventory of compiler-generated patterns in the shipped original
and a mapping of each pattern to the compile/link flag(s) it proves or
constrains, with the current build's status.

## 0. Bottom line

* Baseline gate (canonical `cmake-build-match`, before and after this pass):

  ```text
  verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  RATCHET PASS
  size-exact 4524/4524; lint CLEAN; drift 0
  ```

* Every compiler-generated pattern that is observable on the shipped binary is
  reproduced **exactly** by the current recipe: stack canaries, frame pointers,
  EH tables/personalities, RTTI/typeinfo/vtables, static init/destructors,
  `__cxa_atexit`/`__dso_handle`, PLT/GOT/PIC relocations, the `__cxa_*` and
  `__aeabi_*` runtime, `memcpy`/`memset` lowering, and libc++/exceptions.
* All 24 non-byte-exact functions are application functions. Every runtime
  support function (`__cxa_*`, `__gxx_personality_v0`, `_Unwind_*`, libgcc
  helpers, zlib/libzip/libcrypto) is byte-exact.
* No tested flag is a strict improvement. Two full-link A/B variants run for
  this note are both worse: `-fno-stack-protector` 1369/3780 and
  `-fno-slp-vectorize` 1484/4386 (baseline 4500/4500).
* The remaining `.text`/`.ARM.exidx`/`.ARM.extab` differences are source-shape
  residues in the 24 non-exact functions (plus 16 LSDA records of byte-exact
  functions), not compile-flag differences.
* The inline differing-byte counts in `docs/VALIDATION.md` and
  `docs/IDENTICALITY.md` are stale for this binary: measured `.text` 100,113
  (docs say 102,777) and `.ARM.exidx` 829 (docs say 842). See §7.

## 1. Baseline and artifact identity

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5214
export TMPDIR=/dev/shm/w5214/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target lint  -j6
cmake --build cmake-build-match --target drift -j6
```

`check` output:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint`: `CLEAN ✓ (no new soft violations; hard invariants 0)`.
`drift`: `=== DRIFT: 0 field(s) across 0 class(es) ===`.
`report.json`: `size-exact 4524/4524`, `byte 4500`, `linked 4500`.

Artifact hashes and sizes:

```sh
sha256sum /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
          cmake-build-match/libgof2hdaa.so
stat -c '%n %s' /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
                  cmake-build-match/libgof2hdaa.so
```

| file | sha256 | size |
|---|---|---:|
| original | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |
| ours | `d7a6fe3101e1c4ed74d1ce4672e9e8ad44a447bdbca7f4a20fb1ba9d15940317` | 2,187,328 |

The hash difference is the `.note.gnu.build-id` (the original's build-id is the
SHA-1 of its unstripped gold output; the build directory is embedded in DWARF).

Section table, measured directly from the two ELF files (script parses
`sh_name/sh_type/sh_addr/sh_offset/sh_size` and compares section bytes):

```text
section                 orig size  ours size  size=  bytes=  differing bytes
.text                    0x1337c8   0x1337c8   True   False  100113
.ARM.exidx               0x6f70     0x6f70     True   False  829
.ARM.extab               0x26298    0x26298    True   False  108295
.note.gnu.build-id       0x24       0x24       True   False  20
.rodata                  0x41b67    0x41b67    True   True   0
.data                    0x296c     0x296c     True   True   0
.data.rel.ro             0x2fe8     0x2fe8     True   True   0
.bss                     0x69ac     0x69ac     True   True   0
.got                     0x430c     0x430c     True   True   0
.plt                     0xac4c     0xac4c     True   True   0
.rel.dyn                 0x7048     0x7048     True   True   0
.rel.plt                 0x72d0     0x72d0     True   True   0
.dynsym                  0x15ca0    0x15ca0    True   True   0
.dynstr                  0x2e318    0x2e318    True   True   0
.hash                    0x973c     0x973c     True   True   0
.init_array              0xc        0xc        True   True   0
.fini_array              0x8        0x8        True   True   0
.dynamic                 0x148      0x148      True   True   0
.gnu.version             0x2b94     0x2b94     True   True   0
.gnu.version_r           0x40       0x40       True   True   0
.gnu.version_d           0x1c       0x1c       True   True   0
.comment                 0x12f      0x12f      True   True   0
.note.android.ident      0x98       0x98       True   True   0
.note.gnu.gold-version   0x1c       0x1c       True   True   0
.ARM.attributes          0x38       0x38       True   True   0
.shstrtab                0x117      0x117      True   True   0
```

All 27 original section headers are present and size-exact. Only `.text`,
`.ARM.exidx`, `.ARM.extab` and `.note.gnu.build-id` differ in content.

Symbol counts (`readelf --dyn-syms -W`):

| observable | original | ours |
|---|---:|---:|
| defined `FUNC` | 4524 | 4524 |
| defined `OBJECT` | 843 | 843 |
| defined symbols total | 5370 | 5370 |
| undefined imports (excl. null) | 207 | 207 |
| `_ZTV` | 99 | 99 |
| `_ZTI` | 176 | 176 |
| `_ZTS` | 176 | 176 |
| `__cxa_*` names (defined+UND) | 32 | 32 |
| `__aeabi_*` names (defined+UND) | 21 | 21 |

The undefined-import name sets are identical (`diff` of the sorted `UND` names
is empty).

## 2. Pattern inventory and flag mapping

All counts below are from the NDK r18b `objdump -d -M force-thumb` (Thumb code)
plus `objdump -d` (ARM-mode vendored libcrypto) and the ELF parsers. The
scripts are inlined in §8.

### 2.1 Stack canaries — proves `-fstack-protector-strong`

Commands:

```sh
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d -M force-thumb <so> > /dev/shm/w5214/out/{orig,ours}.dis
grep -c '__stack_chk_fail@plt' ...
```

Raw results:

| observable | original | ours |
|---|---:|---:|
| `__stack_chk_fail` call sites | **787** | **787** |
| functions containing a `__stack_chk_fail` call | **757** | **757** |
| canary-presence set differences | — | **0** |
| `__stack_chk_guard` import | `UND OBJECT GLOBAL` | identical |
| `__stack_chk_fail` import | `UND FUNC GLOBAL` | identical |

The guard is the **global** guard, not a TLS guard: the prologue loads
`__stack_chk_guard` through the GOT (`ldr rA,[pc]` / `add rA,pc` /
`ldr rB,[rA]` / `ldr rC,[rB]`), and the import is `UND OBJECT`, so clang's
default `-mstack-protector-guard=global` is correct.

The prior canary table (`stack_frame_unwind_w4305_canary.csv`, 756 data rows)
uses a stricter decoded-canary-slot pattern; the extra function in the 757
count is `BuildResourceList` (55,992 B), whose canary slot the older classifier
did not decode. Both sides agree on all 757.

Object-level A/B (`src/engine/file/BlowfishSubkeyFile.c`, exact
`compile_commands.json` command with one flag changed):

| variant | `InitializeBlowfish` | `opensubkeyfile` | `F` | `Blowfish_encipher` | `Blowfish_decipher` |
|---|---:|---:|---:|---:|---:|
| base (`-fstack-protector-strong`) | **416** | **52** | **60** | **80** | **80** |
| `-fstack-protector` (basic) | 372 | 52 | 60 | 80 | 80 |
| `-fno-stack-protector` | 372 | 52 | 60 | 80 | 80 |
| `-fstack-protector-all` | 416 | 80 | 96 | 112 | 112 |

The original dynsym sizes are 416 / 52 / 60 / 80 / 80. Only **strong** matches:
basic/none drop `InitializeBlowfish` to 372; all grows `opensubkeyfile` to 80
and `F`/encipher/decipher. `-fstack-protector-explicit` does not exist in clang
7.0.2.

### 2.2 Frame pointers — proves the default keep setting

Raw prologue census (whole `.text`):

| pattern | original | ours |
|---|---:|---:|
| `add r7, sp, #N` | **2476** | **2476** |
| `mov r7, sp` | **220** | **220** |
| `push {..., r7, lr}` | **2690** | **2690** |
| functions with a frame pointer in the first 14 instructions | **2705** | **2705** |
| frame-pointer set differences | — | **0** |

Object-level A/B (`BlowfishSubkeyFile.c`):

| variant | `Blowfish_encipher` | `Blowfish_decipher` | `InitializeBlowfish` |
|---|---:|---:|---:|
| base | **80** | **80** | **416** |
| `-fomit-frame-pointer` | 72 | 72 | 420 |

The original matches the base. `-fno-omit-frame-pointer`,
`-mno-omit-leaf-frame-pointer` and `-Xclang -mdisable-fp-elim` are byte-identical
to the default on this tree (prior wave-93 measurement); the default is already
"keep". `-momit-leaf-frame-pointer` and `-fomit-frame-pointer` are disproven.

### 2.3 EH tables and personalities — proves `-funwind-tables` and `-fexceptions`

ELF EH structure (direct parser of `.ARM.exidx` prel31 entries and
`.ARM.extab` records):

| observable | original | ours |
|---|---:|---:|
| `.ARM.exidx` size | `0x6f70` | `0x6f70` |
| exidx entries | **3566** | **3566** |
| `CANTUNWIND` (`w1 == 1`) | **272** | **272** |
| CANT entries sharing a target with a real entry | **193** | **193** |
| duplicate target addresses | **194** | **194** |
| inline compact entries | **856** | **856** |
| extab-referencing entries | **2438** | **2438** |
| `.ARM.extab` size | `0x26298` | `0x26298` |
| extab records | **2438** | **2438** |
| compact personality index 0 (`__aeabi_unwind_cpp_pr0`) | **856** | **856** |
| compact personality index 1 (`__aeabi_unwind_cpp_pr1`) | **1841** | **1841** |
| explicit personality pointer (`__gxx_personality_v0@plt`) | **597** | **597** |

The 193 CANT+real pairs are gold's `fix_exidx_coverage()` sentinel for the empty
plain `.text` section every `-ffunction-sections` object carries; this is the
positive proof of `-ffunction-sections` (an object built without it has one
monolithic `.text` and no such sentinels).

Personality routines:

| symbol | role | original | ours |
|---|---|---|---:|
| `__aeabi_unwind_cpp_pr0` | compact personality index 0 | imported | identical |
| `__aeabi_unwind_cpp_pr1` | compact personality index 1 | imported | identical |
| `__aeabi_unwind_cpp_pr2` | compact personality index 2 | **absent** | absent |
| `__gxx_personality_v0` | C++ personality, 348 B, byte-exact | defined | identical |
| `_Unwind_Resume` | 540 call sites | imported | identical |
| `_Unwind_RaiseException` | 3 call sites | imported | identical |
| `_Unwind_DeleteException`, `_Unwind_GetLanguageSpecificData`, `_Unwind_GetRegionStart`, `_Unwind_VRS_Get`, `_Unwind_VRS_Set` | 1 each | imported | identical |
| `__gnu_unwind_frame` | 1 call site | imported | identical |

The explicit personality pointer words in `.ARM.extab` all resolve to
`__gxx_personality_v0@plt` (0x5eb60 in the original, byte-identical `.plt`).

Object-level A/B (`src/engine/render/Engine.cpp`):

| variant | `.ARM.exidx` total | `.ARM.extab` total |
|---|---:|---:|
| base | 640 B / 80 sections | **1168 B / 47 sections** |
| `-fno-unwind-tables` appended | 640 B / 80 sections | 1120 B / 43 sections |

`-fno-exceptions` (`src/game/world/Level.cpp`) changes 23 function sizes
(`Level::init` 1568→1464, `createRadioMessages` 12282→10120, `createShip`
2764→2600, …) and shrinks the TU's `.ARM.extab` from 11548 B to 1096 B. The
original matches the base, so **exceptions are on**.

### 2.4 RTTI / typeinfo / vtables — proves `-frtti`

`readelf --dyn-syms -W` section placement and sizes:

| observable | original | ours |
|---|---:|---:|
| `_ZTV` (vtables) | **99** in `.data.rel.ro`, 5364 B | identical |
| `_ZTI` (typeinfo) | **176** in `.data.rel.ro`, 2156 B | identical |
| `_ZTS` (type names) | **176** in `.rodata`, 2820 B | identical |
| `.rodata` bytes | — | **0 differing** |
| `.data.rel.ro` bytes | — | **0 differing** |

Object-level A/B (`src/engine/file/FileInterfaceAndroid.cpp`):

| variant | `_ZTI`/`_ZTS`/`_ZTV` symbols | `.text` changes |
|---|---:|---:|
| base | **8** | — |
| `-fno-rtti` | **2** | 0 |

`-fno-rtti` leaves `.text` unchanged but removes 6 typeinfo/vtable objects from
this TU (and changes `.rodata`/`.data.rel.ro` globally). The original's
`.rodata` and `.data.rel.ro` are byte-identical to the base, so **RTTI is on**.

### 2.5 Static init/destructors, `__cxa_atexit`, `__dso_handle`

`.init_array` and `.fini_array` are byte-identical:

```text
.init_array [0x69441, 0x694c1, 0x694dd]   (3 entries)
.fini_array [0x69548, 0x69534]             (2 entries)
```

* The three `.init_array` targets are the three original static initializers:
  `Engine.cpp` (`tv_modes` as `Array<int>`, `renderer`, `vendor`), the touch
  buffer (`realloc(touches, sizeof(Touch))`), and `ParticleSettings.cpp`
  (`ParticleSettingsRef::init`, `::cur`). They are local (non-exported)
  functions; the disassembly is byte-identical.
* The two `.fini_array` targets are crtbegin's ARM-mode `__on_dlclose` and
  `__on_dlclose_late` (they call `__cxa_finalize`), also byte-identical.

`__cxa_atexit` usage:

| site | form | original | ours |
|---|---|---|---:|
| `Engine.cpp` `__GLOBAL__sub_I` (0x69440) | 2× indirect `blx r4`, `r4 = GOT[__cxa_atexit]` | 2 | 2 |
| `ParticleSettings.cpp` init (0x694dc) | direct `blx __cxa_atexit@plt` (0x69500) | 1 | 1 |
| `GameText::getText` (0x818b2) | direct `blx __cxa_atexit@plt` | 1 | 1 |
| direct `__cxa_atexit@plt` call sites counted by grep | — | **2** | **2** |
| `atexit@plt` call sites | — | **0** | **0** |

The indirect Engine calls use the GOT slot at `0x210000`:

```text
00210000  00000115 R_ARM_GLOB_DAT  00000000  __cxa_atexit@LIBC
```

`__dso_handle` is a local hidden 4-byte object from `crtbegin_so.o`:

```text
crtbegin_so.o: 15: 00000000 4 OBJECT GLOBAL HIDDEN 2 __dso_handle
linked .data:  0x214000: 00402100  (R_ARM_RELATIVE -> value 0x214000)
```

The third argument to `__cxa_atexit` is `0x214000` (the address of
`__dso_handle`) at both the Engine and GameText call sites. `__dso_handle` is
not exported (hidden, stripped), which is why it does not appear in `.dynsym`.

Object-level A/B (`src/engine/render/Engine.cpp`):

| variant | `.text.startup` size | relocation |
|---|---:|---|
| base | **128** | `R_ARM_GOT_PREL __cxa_atexit` |
| `-fno-use-cxa-atexit` | 140 | `R_ARM_GOT_PREL atexit` |

The original init function is 128 B, so `__cxa_atexit` is proven.

### 2.6 PLT / GOT / PIC / relocations

`readelf -dW` and `readelf -rW`:

| observable | original | ours |
|---|---:|---:|
| `R_ARM_JUMP_SLOT` | **3674** | **3674** |
| `R_ARM_ABS32` | **1817** | **1817** |
| `R_ARM_GLOB_DAT` | **614** | **614** |
| `R_ARM_RELATIVE` | **1162** | **1162** |
| `R_ARM_REL32` | **0** | **0** |
| `DT_RELCOUNT` | **1162** | **1162** |
| `DT_FLAGS` | `BIND_NOW` | identical |
| `DT_FLAGS_1` | `NOW` | identical |
| `DT_TEXTREL` | absent | absent |
| `.plt` / `.got` bytes | — | **0 differing** |
| `.rel.plt` / `.rel.dyn` bytes | — | **0 differing** |

This proves PIC codegen (`-fPIC`/`-fpic`; the exact spelling is unobservable on
this source, both are byte-identical) and `-z now` / `-z combreloc` link
settings. It also proves the single plain gold link: `.rel.plt` is
byte-identical, so the PLT order and the GOT layout are identical.

### 2.7 `__cxa_*` runtime

Defined (from libc++abi, all byte-exact) and undefined:

| symbol | original size | byte-exact |
|---|---:|---|
| `__cxa_allocate_exception` | 42 | yes |
| `__cxa_throw` | 92 | yes |
| `__cxa_begin_catch` | 112 | yes |
| `__cxa_end_catch` | 136 | yes |
| `__cxa_guard_acquire` | 116 | yes |
| `__cxa_pure_virtual` | 16 | yes |
| `__cxa_deleted_virtual` | 16 | yes |
| `__cxa_demangle` | 568 | yes |
| `__gxx_personality_v0` | 348 | yes |

Call-site census (identical on both sides):

| call | original | ours |
|---|---:|---:|
| `__cxa_guard_acquire` | 2 | 2 |
| `__cxa_guard_release` | 1 | 1 |
| `__cxa_guard_abort` | 1 | 1 |
| `__cxa_atexit` (direct PLT) | 2 | 2 |
| `__cxa_throw` | 3 | 3 |
| `__cxa_begin_catch` | 12 | 12 |
| `__cxa_end_catch` | 10 | 10 |
| `__cxa_allocate_exception` | 3 | 3 |
| `__cxa_allocate_dependent_exception` | 1 | 1 |
| `__cxa_rethrow` | 1 | 1 |
| `__cxa_get_globals` | 6 | 6 |
| `__cxa_get_globals_fast` | 8 | 8 |
| `__cxa_uncaught_exceptions` | 1 | 1 |
| `__cxa_call_unexpected` | 7 | 7 |
| `__cxa_begin_cleanup` | 1 | 1 |
| `__cxa_decrement_exception_refcount` | 1 | 1 |
| `__cxa_demangle` | 1 | 1 |

The thread-safe-statics fast path is present in the byte-exact
`GameText::getText` (292 B): `ldrb; dmb ish; tst #1; bne` → construct →
`__cxa_atexit` → `__cxa_guard_release`. Object-level A/B:

| variant | `GameText::getText` |
|---|---:|
| base | **292** |
| `-fno-threadsafe-statics` | 260 |
| `-fno-use-cxa-atexit` | 288 |

The original is 292 B, so thread-safe statics and `__cxa_atexit` are proven.

### 2.8 `__aeabi_*` runtime

Call-site census (ARM+Thumb, identical on both sides):

| helper | original | ours |
|---|---:|---:|
| `__aeabi_idiv` | 68 | 68 |
| `__aeabi_uidiv` | 29 | 29 |
| `__aeabi_idivmod` | 26 | 26 |
| `__aeabi_uidivmod` | 12 | 12 |
| `__aeabi_uldivmod` | 3 | 3 |
| `__aeabi_memcpy` | 124 | 124 |
| `__aeabi_memcpy4` | 17 | 17 |
| `__aeabi_memcpy8` | 1 | 1 |
| `__aeabi_memclr` | 10 | 10 |
| `__aeabi_memclr4` | 74 | 74 |
| `__aeabi_memclr8` | 12 | 12 |
| `__aeabi_memmove` | 1 | 1 |
| `__aeabi_memmove4` | 5 | 5 |
| `__aeabi_memset4` | 1 | 1 |
| `__aeabi_f2lz` | 16 | 16 |
| `__aeabi_f2ulz` | 1 | 1 |
| `__aeabi_l2d` | 3 | 3 |
| `__aeabi_l2f` | 59 | 59 |
| `__aeabi_ul2f` | 2 | 2 |
| `__aeabi_ldivmod` | 0 calls | 0 calls |
| `__aeabi_unwind_cpp_pr0` | personality index 0 | identical |
| `__aeabi_unwind_cpp_pr1` | personality index 1 | identical |

`__aeabi_ldivmod` is **not exported** and not undefined: it is the local
libgcc thunk from `_aeabi_ldivmod.o` (size 0, tail-calls
`__gnu_ldivmod_helper`), hidden by `--exclude-libs=libgcc.a`. This is the
expected result of the original's link line.

### 2.9 `memcpy`/`memset` inlining

Direct `memcpy`/`memset` calls (ARM-mode vendored libcrypto) vs the EABI
helpers:

| call | original | ours |
|---|---:|---:|
| `memcpy@plt` | 3 | 3 |
| `memset@plt` | 5 | 5 |
| `__aeabi_memcpy` (Thumb) | 124 | 124 |
| `__aeabi_memclr4` | 74 | 74 |
| `__aeabi_memclr8` | 12 | 12 |
| `__aeabi_memclr` | 10 | 10 |

The presence of both direct libc calls and EABI helpers, with identical counts,
proves normal builtin lowering (`-fno-builtin` absent). Object-level A/B
(`src/game/world/Level.cpp`): `-fno-builtin` changes **30** function sizes
(`Level::Level` 264→304, `Matrix::Matrix` 64→104, `createMission` 17172→17300,
…). The original matches the base.

### 2.10 libc++ / exceptions / sized delete

* `-stdlib=libc++` is proven by code-byte identity of the libc++abi runtime:
  `__cxa_allocate_exception` (42 B), `__cxa_guard_acquire` (116 B),
  `__gxx_personality_v0` (348 B), `__cxa_throw` (92 B), etc. are byte-exact.
  No `std::__ndk1`/`std::__cxx11`/`std::__1` names appear in `.dynsym`.
* `-fno-sized-deallocation` (the C++11 default) is proven by the call census:
  `_ZdlPv` 1057, `_ZdaPv` 249, `_ZdlPvj`/`_ZdaPvj` **0**.
  Object-level `-fsized-deallocation` on `Engine.cpp` changes
  `Engine::~Engine` 228→236 and `Engine::ShaderInit` 884→1006.
* `-fexceptions` is proven by the EH structure and by the fact that
  `-fno-exceptions` changes 23 `Level.cpp` function sizes and shrinks its
  `.ARM.extab` from 11548 B to 1096 B.

### 2.11 Toolchain / ISA / notes

| observable | value |
|---|---|
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123` |
| `.note.android.ident` | `api=16`, `ndk="r18b"`, `build="5063045"` |
| `.note.gnu.gold-version` | `gold 1.12` |
| `.ARM.attributes` | ARM v7/A, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, unaligned v6 |
| ELF flags | `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) |

All of these sections are byte-identical between the two binaries.

## 3. Controlled A/B experiments

Object-level A/B uses the exact `compile_commands.json` command with one flag
changed; the linked full-build A/B uses a scratch toolchain that appends
`GOF2_EXTRA_FLAGS` to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`.

| variant | TU / scope | observed change | original matches? |
|---|---|---|---|
| `-fstack-protector` | `BlowfishSubkeyFile.c` | `InitializeBlowfish` 416→372 | no (basic) |
| `-fno-stack-protector` | `BlowfishSubkeyFile.c` | 416→372 | no (none) |
| `-fstack-protector-all` | `BlowfishSubkeyFile.c` | `opensubkeyfile` 52→80, `F` 60→96, enc/dec 80→112 | no (all) |
| `-fomit-frame-pointer` | `BlowfishSubkeyFile.c` | enc/dec 80→72, `InitializeBlowfish` 416→420 | no |
| `-g0` | `BlowfishSubkeyFile.c` | `InitializeBlowfish` bytes change | no |
| `-fno-unwind-tables` | `Engine.cpp` | `.ARM.extab` 1168→1120 B, 47→43 sections | no |
| `-fno-exceptions` | `Level.cpp` | 23 sizes change, `.ARM.extab` 11548→1096 B | no |
| `-fno-rtti` | `FileInterfaceAndroid.cpp` | `_ZTI`/`_ZTS`/`_ZTV` 8→2 | no |
| `-fno-threadsafe-statics` | `GameText.cpp` | `getText` 292→260 | no |
| `-fno-use-cxa-atexit` | `GameText.cpp` | `getText` 292→288 | no |
| `-fno-use-cxa-atexit` | `Engine.cpp` | `.text.startup` 128→140, `__cxa_atexit`→`atexit` | no |
| `-fsized-deallocation` | `Engine.cpp` | `~Engine` 228→236, `ShaderInit` 884→1006 | no |
| `-fno-strict-aliasing` | `Engine.cpp` | `RenderMesh` 1040→1032 | no |
| `-std=c++14` | `PaintCanvas.cpp` | `Material::Material` 276→272 | no |
| `-fno-builtin` | `Level.cpp` | 30 sizes change | no |
| `-fno-merge-constants` | `Level.cpp` | neutral | consistent |
| `-fdata-sections` | `Level.cpp` | sections 624→722, `.rodata*` 4→86, `.bss*` 1→17 | no |
| `-fno-common` | `BackButton.c` | `g_android_back_button_pressed` `COM`→strong | source-dependent |
| `-Os` / `-O2` / `-O3` | `Engine.cpp` | `SetTextures` 312→328/336/360 | no |

Full-link A/B (official verifier, same original):

| variant | byte_exact | linked_exact | size-exact |
|---|---:|---:|---:|
| base `-Oz` | **4500** | **4500** | **4524/4524** |
| `-fno-stack-protector` | 1369 | 3780 | — |
| `-fno-slp-vectorize` | 1484 | 4386 | — |

Both variants are strictly worse. The prior 81-variant global matrix and
277-cell per-TU matrix (`worst_function_flag_sensitivity_w3751.md`) also found
no flag that raises any of the non-exact functions' fuzzy match.

## 4. Proven / disproven / unobservable

### Positively proven (positive fingerprint in the shipped binary + A/B)

* NDK r18b clang 7.0.2, API 16, ARMv7-A, Thumb-2, NEON, softfp, gold 1.12,
  libc++.
* `-Oz`, `-g`, `-std=c++11` (vs c++14), `-fPIC` (PIC codegen), default
  visibility, `-fstack-protector-strong`, frame pointers kept,
  `-funwind-tables`, `-fexceptions`, `-frtti`, `-ffunction-sections`,
  no `-fdata-sections`, strict aliasing on, math errno on, SLP vectorization
  on, loop unrolling on, jump tables on, unaligned access on,
  zero-init-in-bss on, unsigned `char`, int enums, no sized delete,
  thread-safe statics, `__cxa_atexit`, normal builtins.
* `-z now`, `-z combreloc`, no text relocations, single plain gold link.

### Disproven (the alternative moves away from the original)

* `-fstack-protector` (basic), `-fno-stack-protector`,
  `-fstack-protector-all`.
* `-fomit-frame-pointer`, `-momit-leaf-frame-pointer`.
* `-fno-unwind-tables`, `-fno-exceptions`.
* `-fno-rtti`.
* `-fno-threadsafe-statics`, `-fno-use-cxa-atexit`.
* `-fsized-deallocation`.
* `-fno-strict-aliasing`, `-fno-builtin`, `-fno-merge-constants` (neutral),
  `-fdata-sections`.
* `-Os`, `-O2`, `-O3`, `-g0`, `-g1`, `-std=c++14`.
* `-marm`, `-mfpu=vfpv3-d16`, `-mfloat-abi=hard`, `-mvisibility=hidden`,
  `-fvisibility-inlines-hidden`.

### Unobservable / not settled

* Exact `-fPIC` vs `-fpic` spelling (byte-identical on this source).
* Exact `-std=c++11` vs `-std=gnu++11` spelling (byte-identical).
* `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16` as macros (the API level
  itself is proven by `.note.android.ident`; the macros are unused by the
  source and codegen-neutral).
* `-fmerge-constants` (neutral on this source).
* `-fcommon` vs a single-definition `-fno-common` original source: the current
  source has two tentative definitions of `g_android_back_button_pressed`
  (`BackButton.c`, `GOF2HD2012.c`), so it requires `-fcommon`; the shipped
  binary cannot distinguish `-fcommon` from a single-definition original.
* Exact `-target` vendor/OS spelling.
* The build-system file (`Android.mk`/`Application.mk` vs custom
  Makefile/CMake) is not in the APK.

## 5. Confidence table

`PROVEN` = positive binary fingerprint plus a controlled A/B that moves away
when the flag changes. `CONSISTENT` = the binary is consistent with the flag but
the flag is not uniquely observable. `UNOBSERVABLE` = no code/data difference on
this source.

| claim | confidence | hard evidence |
|---|---|---|
| NDK r18b clang 7.0.2 | **PROVEN** | `.comment` exact string; `.note.android.ident` build 5063045 |
| API 16 | **PROVEN** | `.note.android.ident` api=16; APK `minSdkVersion=16` |
| ARMv7-A / Thumb-2 / NEON / softfp | **PROVEN** | `.ARM.attributes`; ELF flags `0x5000200`; NEON instructions |
| `-Oz` | **PROVEN** | `SetTextures` 312 vs 328/336/360 for `-Os`/`-O2`/`-O3` |
| `-g` | **PROVEN** | `-g0` changes `InitializeBlowfish` bytes (object A/B) |
| `-std=c++11` | **PROVEN** | `Material::Material` 276 vs 272 under `-std=c++14` |
| `-fPIC` (PIC) | **PROVEN** | 0 `R_ARM_REL32`, no `DT_TEXTREL`, 614 `GLOB_DAT`, `.got` exact |
| `-fstack-protector-strong` | **PROVEN** | 787 canary calls; 416/52/60/80/80 anchors; basic/all differ |
| frame pointers kept | **PROVEN** | 2476 `add r7,sp` + 220 `mov r7,sp` + 2690 `push {...,r7,lr}`; omit changes |
| `-funwind-tables` | **PROVEN** | 3566 exidx / 272 CANT / 193 CANT+real / 2438 extab; object A/B extab 1168→1120 |
| `-ffunction-sections` | **PROVEN** | 193 CANT+real empty-`.text` sentinels |
| `-fexceptions` | **PROVEN** | `_Unwind_Resume` 540, `__gxx_personality_v0` 348 B exact, extab 156312 B; `-fno-exceptions` changes 23 Level sizes |
| `-frtti` | **PROVEN** | 176 `_ZTI`/`_ZTS`, 99 `_ZTV`; `.rodata`/`.data.rel.ro` exact; `-fno-rtti` 8→2 |
| `-fno-threadsafe-statics` absent | **PROVEN** | 2 `__cxa_guard_acquire`; `getText` 292 vs 260 |
| `-fno-use-cxa-atexit` absent | **PROVEN** | 2 direct + 2 indirect `__cxa_atexit`; 0 `atexit`; init 128 vs 140 |
| `__dso_handle` local | **PROVEN** | crtbegin hidden 4-byte OBJECT; `.data` 0x214000 self-pointer; third arg 0x214000 |
| `-fno-sized-deallocation` | **PROVEN** | 1057 `_ZdlPv`, 249 `_ZdaPv`, 0 `_ZdlPvj`/`_ZdaPvj` |
| normal builtins / memcpy lowering | **PROVEN** | 3 `memcpy` + 5 `memset` + 124/74/12/10 EABI calls; `-fno-builtin` changes 30 sizes |
| `-stdlib=libc++` | **PROVEN** | `__cxa_*`/`__gxx_personality_v0` byte-exact; no libstdc++ names |
| `-z now` / `-z combreloc` | **PROVEN** | `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `DT_RELCOUNT=1162` |
| `-fdata-sections` absent | **PROVEN** | `.rodata`/`.data` exact; `-fdata-sections` 624→722 sections |
| `-fno-common` vs `-fcommon` | **CONSISTENT** | current source has two `COM` definitions; binary cannot distinguish |
| `-fPIC` vs `-fpic` | **UNOBSERVABLE** | byte-identical on this source |
| `-std=c++11` vs `gnu++11` | **UNOBSERVABLE** | byte-identical |
| `-DNDEBUG`/`-DANDROID`/API macro | **UNOBSERVABLE** | codegen-neutral; API level proven by note |
| `-fmerge-constants` | **UNOBSERVABLE** | neutral; `.rodata` identical |

## 6. Strict improvement?

**No.** The current recipe is already the best tested:

* Baseline: byte 4500, linked 4500, size 4524/4524.
* `-fno-stack-protector`: byte 1369, linked 3780.
* `-fno-slp-vectorize`: byte 1484, linked 4386.
* The prior 81-variant global matrix and 277-cell per-TU matrix found no flag
  that raises any non-exact function's fuzzy match; every flag that changes a
  target lowers it (`worst_function_flag_sensitivity_w3751.md`).
* All 24 non-byte-exact functions are application functions; every runtime
  support function is byte-exact. The remaining diffs are source-shape
  (register allocation, scheduling, 16/32-bit encoding, basic-block order,
  local vectorization/conditional execution), not flag-driven.

Therefore this commit lands **the note only**; no `src/`, build-flag, tool,
baseline or allowlist change is made.

## 7. Residuals and stale doc numbers

The only non-identical section bytes are:

| section | differing bytes | cause |
|---|---:|---|
| `.text` | **100,113** | 24 non-exact application functions (20 distinct addresses) |
| `.ARM.exidx` | **829** | 812 entries' word1 (LSDA pointer) shifted by the two extab record-size deltas |
| `.ARM.extab` | **108,295** | 2 record-size deltas + 16 semantic LSDA records + the resulting tail shift |
| `.note.gnu.build-id` | **20** | build-id hash |

The `.text` functions with differing bytes (per-function differing bytes /
function size):

```text
1306/1996   Generator::createMission
 602/968    PlayerEgo::calcCollision
 871/1432   Status::missionCompleted
5932/17172  Level::createMission
 454/2252   Level::createScene
2065/2764   Level::createShip
5876/7072   Layout::Layout C1/C2
  39/1008   ModStation::ModStation C1/C2
 405/612    Globals::Globals C1/C2
4482/9600   MenuTouchWindow::MenuTouchWindow C1/C2
  71/1896   MenuTouchWindow::OnTouchBegin
7610/9908   Radar::draw
54743/57232 LevelScript::process
6935/9700   HangarWindow::render
 668/1984   Gun::calcCharacterCollision
 972/1908   RocketGun::update
2400/9332   Hud::draw
 178/1948   SpaceLounge::drawLounge
4463/8080   MGame::OnTouchEnd
  41/11440  MGame::OnUpdate
```

EH residuals:

* `.ARM.exidx`: 3566 entries, 272 CANT, 193 CANT+real, 194 duplicate targets,
  856 inline, 2438 extab refs — all identical. 812 entries have a differing
  `word1` (LSDA pointer) because two extab records change size.
* `.ARM.extab`: 2438 records. Record-size differences: exactly two —
  `Generator::createMission` 172→168 (−4) and `LevelScript::process` 348→352
  (+4), net 0, so the section size is exact. 16 records have a semantic
  (post-personality-word) LSDA content difference; the remaining 253 records
  that differ only do so in the first (prel31 personality) word because they
  are shifted by 4 bytes.
* The 16 semantic LSDA records include byte-exact functions
  (`PaintCanvas::DrawStringColor`, `PlayerJumpgate::C2`,
  `RecordHandler::recordStoreRead`, `RecordHandler::readOptionsFileAsByteArray`,
  `Radar::C2`, `StatusWindow::D2`) as well as non-exact functions. These are
  source-shape residues, not flags.

Stale inline numbers in the docs (measured for this binary):

| doc claim | measured |
|---|---|
| `docs/VALIDATION.md` / `docs/IDENTICALITY.md` `.text` 102,777 | **100,113** |
| `docs/VALIDATION.md` / `docs/IDENTICALITY.md` `.ARM.exidx` 842 | **829** |

The `.ARM.extab` size (`0x26298`) and the "24 non-byte-exact functions" count
are still correct. The two stale byte counts should be corrected in a docs pass
(the AGENTS.md rule about updating inline doc numbers); this note does not
change them.

## 8. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w5214
export TMPDIR=/dev/shm/w5214/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target lint  -j6
cmake --build cmake-build-match --target drift -j6

# whole-.text census
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d -M force-thumb /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w5214/out/orig.dis
$OBJ -d -M force-thumb cmake-build-match/libgof2hdaa.so > /dev/shm/w5214/out/ours.dis
$OBJ -d /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w5214/out/orig.auto.dis
$OBJ -d cmake-build-match/libgof2hdaa.so > /dev/shm/w5214/out/ours.auto.dis

# section bytes / hashes
OBJCOPY=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
for s in .text .ARM.exidx .ARM.extab .rodata .data .data.rel.ro .got .plt \
         .rel.plt .rel.dyn .dynsym .dynstr .hash .init_array .fini_array \
         .note.gnu.build-id .comment .ARM.attributes .note.android.ident \
         .note.gnu.gold-version .shstrtab; do
  $OBJCOPY -O binary --only-section=$s <orig> /dev/shm/w5214/out/o.bin
  $OBJCOPY -O binary --only-section=$s <ours> /dev/shm/w5214/out/b.bin
  echo "$s $(cmp -l /dev/shm/w5214/out/o.bin /dev/shm/w5214/out/b.bin | wc -l)"
done

# relocation / dynamic / symbol counts
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -rW <so> | awk '/R_ARM_/{print $3}' | sort | uniq -c
$RE -dW <so> | grep -E 'NEEDED|FLAGS|RELCOUNT|TEXTREL|SONAME'
$RE --dyn-syms -W <so> | awk '$4=="FUNC"&&$7!="UND"{n++} END{print n}'

# object-level A/B (exact compile_commands.json command, one flag changed)
python3 /dev/shm/w5214/compile_tu.py src/engine/file/BlowfishSubkeyFile.c \
  /dev/shm/w5214/obj/Blowfish.sp1.o '-fstack-protector' ''
python3 /dev/shm/w5214/objstat.py /dev/shm/w5214/obj/Blowfish.base.o \
  /dev/shm/w5214/obj/Blowfish.sp1.o

# full-link A/B
cmake -S . -B /dev/shm/w5214/build-nosp -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5214/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fno-stack-protector'
cmake --build /dev/shm/w5214/build-nosp --target link_so -j6
python3 tools/verify/verify.py --build-dir /dev/shm/w5214/verify-nosp \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5214/build-nosp/libgof2hdaa.so \
  --objdump $OBJ --fail-on-wrong-type --top 0
```

Scratch scripts (not tracked): `/dev/shm/w5214/compile_tu.py`,
`objstat.py`, `census.py`, `census2.py`, `eh.py`, `eh2.py`, `recsizes.py`,
`recsizes2.py`, `extab_perfunc.py`, `extab_normalized.py`, `diffmap.py`,
`funcpat.py`, `toolchain_extra.cmake`.

## 9. Files touched

* `_work/reconstructions/compiler_generated_pattern_audit_w5214.md` (this note).
* No `src/`, build-flag, tool, baseline or allowlist change.
* No `--update-baseline` was run.
