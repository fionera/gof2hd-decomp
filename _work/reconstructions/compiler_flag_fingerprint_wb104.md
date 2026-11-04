# Compiler-flag fingerprint scan of the whole `.text` (wb104, wave 101)

**Status:** research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/wb104`,
branch `wave101-wb104`, base `3856949ab`. Scratch is `/dev/shm/wb104`. The canonical
`cmake-build-match` was used only as the baseline; every variant was built in a scratch
directory. No source or build-flag change is landed: **no tested flag is a strict
improvement**. The gate is unchanged.

**Baseline (measured before and after this pass):**

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size-exact 4524/4524; lint CLEAN; drift 0
```

Section sizes: `.text 0x1337c8`, `.rodata 0x41b67`, `.data 0x296c`, `.bss 0x69ac`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26294` (original `0x26298`), `.rel.plt 0x72d0`.

## 0. Bottom line

* The current compile recipe is confirmed by an independent **whole-`.text` instruction
  census** and by **object- and full-link A/B builds**. Every instruction-selection and
  runtime-support fingerprint that is observable on the 4500 byte-exact functions is
  reproduced exactly.
* **All** whole-`.text` pattern deltas are confined to the 24 non-exact functions. The
  4500 exact functions have zero pattern deltas (not merely equal gate counts: identical
  per-function instruction-pattern vectors).
* The only non-reproduced artifact-level patterns are the known source-shape residuals:
  24 non-exact function bodies, the 4-byte `.ARM.extab` deficit (fully accounted for by
  five non-exact functions' LSDA records), `.ARM.exidx` address content, `.dynsym`
  `st_shndx` values shifted by the extra `.missioncase` section, the build-id, and
  `.shstrtab`. None is a compile-flag difference.
* No missing compile flag was found. The remaining 24 functions are source-shape/TU-shape
  work, not flag work.

## 1. Method and baseline

```sh
cd /home/debian/GalaxyOnFire2/wt/wb104
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Three independent measurement layers were used:

1. **Whole-`.text` disassembly census.** Both `.so`s were disassembled with the NDK
   `arm-linux-androideabi-objdump -d -M force-thumb` (the stripped original has no
   `$t`/`$d` mapping symbols, so `-M force-thumb` is required). The census counts
   instruction mnemonics, 16/32-bit encoding, frame-pointer prologues, stack-canary
   calls, runtime-helper calls, jump tables, NEON/VFP, literal-pool loads and their
   alignment, and function-start alignment/gaps. Instructions are then attributed to
   exported `.dynsym` functions by `[st_value, st_value+st_size)`.
2. **Artifact fingerprints.** ELF sections, `.ARM.exidx`/`.ARM.extab` structure,
   relocation types, `.dynsym` binding/visibility, `.init_array`/`.fini_array`, notes,
   `.ARM.attributes`, and common symbols are compared directly on the two `.so`s.
3. **Controlled A/B rebuilds.** Object-level A/B uses the exact
   `compile_commands.json` command for a representative TU with one flag changed and
   compares every object-file function's raw bytes with the canonical `-Oz` object.
   Full-link A/B uses `tools/flag_fingerprint.py` (scratch wrapper appending
   `GOF2_EXTRA_FLAGS` to both C and C++ flags) and the official verifier. The canonical
   build is never modified and `--update-baseline` is never run.

The scratch scripts are under `/dev/shm/wb104/`:
`scan2.py` (whole-`.text` census), `perfunc.py` (per-function attribution),
`artifact_fp.py` (sections/relocs/dynsym), `extab_records.py` (EH record sizes),
`ab_tu.py` / `ab_tu2.py` (object A/B), `ab_sections.py` (object section/symbol A/B),
`run_extra_variants.py` / `run_seq_variants.py` / `run_stack_variants.py` /
`run_unwind_exc.py` / `run_inline_variants.py` (full-link A/B), and
`scan_flag_anchor.py` (all-TU anchor search).

## 2. Whole-`.text` instruction census

Command (per `.so`):

```sh
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d -M force-thumb /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/wb104/orig.dis
$OBJ -d -M force-thumb cmake-build-match/libgof2hdaa.so > /dev/shm/wb104/ours.dis
python3 /dev/shm/wb104/scan2.py
python3 /dev/shm/wb104/perfunc.py
```

`.text` size is identical (`0x1337c8`). The census counts are:

| pattern | original | ours | delta | attribution |
|---|---:|---:|---:|---|
| decoded instructions | 468303 | 468130 | -173 | 24 non-exact functions |
| Thumb-2 16-bit / 32-bit | 307409 / 160894 | 307067 / 161063 | -342 / +169 | 24 non-exact functions |
| `add r7, sp, #N` (frame pointer) | 2476 | 2476 | 0 | exact |
| `mov r7, sp` (frame pointer) | 220 | 220 | 0 | exact |
| `push {..., r7, lr}` | 2690 | 2690 | 0 | exact |
| `push {..., lr}` | 2718 | 2718 | 0 | exact |
| `__stack_chk_fail` call sites | 787 | 787 | 0 | exact |
| `__cxa_guard_acquire` calls | 2 | 2 | 0 | exact |
| `__cxa_atexit` calls | 2 | 2 | 0 | exact |
| `atexit` calls | 0 | 0 | 0 | exact |
| `_Unwind_Resume` calls | 541 | 541 | 0 | exact |
| `__cxa_throw` calls | 14 | 14 | 0 | exact |
| `__cxa_begin_catch` calls | 24 | 25 | +1 | `Level::createShip` (non-exact) |
| `__cxa_end_catch` calls | 25 | 25 | 0 | exact |
| `__gxx_personality_v0` refs | 112 | 112 | 0 | exact |
| `__aeabi_idiv` calls | 100 | 100 | 0 | exact |
| `__aeabi_uidiv` calls | 43 | 43 | 0 | exact |
| `__aeabi_idivmod` calls | 29 | 29 | 0 | exact |
| `__aeabi_uidivmod` calls | 13 | 13 | 0 | exact |
| `__aeabi_ldivmod` calls | 0 | 0 | 0 | exact |
| `memcpy` refs | 154 | 154 | 0 | exact |
| `memset` refs | 7 | 7 | 0 | exact |
| `memmove` refs | 12 | 12 | 0 | exact |
| `__aeabi_memcpy` refs | 151 | 151 | 0 | exact |
| `__aeabi_memset` refs | 4 | 4 | 0 | exact |
| `__aeabi_memclr` refs | 101 | 101 | 0 | exact |
| `__errno` refs | 31 | 31 | 0 | exact |
| `_ZdlPv` calls | 1058 | 1058 | 0 | exact |
| `_ZdaPv` calls | 252 | 252 | 0 | exact |
| `_ZdlPvj` / `_ZdaPvj` calls | 0 / 0 | 0 / 0 | 0 | exact |
| `tbb`/`tbh` (jump tables) | 152 | 152 | 0 | exact |
| `cbz`/`cbnz` | 5620 | 5608 | -12 | 24 non-exact functions |
| `it` blocks | 4003 | 4025 | +22 | 24 non-exact functions |
| `movw` / `movt` | 8888 / 9 | 8892 / 9 | +4 / 0 | 24 non-exact functions |
| `ldrd` / `strd` | 2051 / 4600 | 2042 / 4570 | -9 / -30 | 24 non-exact functions |
| NEON SIMD (`vld1`/`vst1`/`vadd`/...) | 16 | 12 | -4 | `Layout::Layout` (non-exact) |
| VFP scalar | 22603 | 22568 | -35 | 24 non-exact functions |
| `ldr rX,[pc,#imm]` literal loads | 14595 | 14591 | -4 | 24 non-exact functions |
| `add rX, pc` PIC anchors | 11866 | 11867 | +1 | 24 non-exact functions |
| literal-pool targets 4-byte aligned | 14595 | 14591 | -4 | all aligned on both sides |
| literal-pool targets unaligned | 0 | 0 | 0 | exact |
| `bkpt` / `udf` / `svc` | 56 / 56 / 36 | 53 / 56 / 36 | -3 / 0 / 0 | `LevelScript::process` (non-exact) |
| `dmb`/`dsb`/`isb` | 21 | 21 | 0 | exact |
| `ldrex`/`strex` | 17 | 17 | 0 | exact |

**Attribution result:** the per-function census assigns every instruction to an exported
function. The 4500 byte-exact functions have **zero** pattern deltas; the unassigned
local/padding regions also have zero pattern deltas. Every delta in the table above is
inside the 24 non-exact functions. The per-function deltas are:

| non-exact function | pattern deltas (ours - original) |
|---|---|
| `Layout::Layout` (C1/C2) | -4 NEON SIMD, -30 `strd`, -9 `ldrd`, +22 `it`, -12 `cbz`, +4 `movw`, -4 `ldr_pc`, -8 decoded bytes |
| `LevelScript::process` | -3 `bkpt`, -326 16-bit, +163 32-bit, -7 VFP, +4 `it`, +2 `cbz`, -10 `ldrd`, -3 `strd`, -2 `ldr_pc`, +1 `movw`, -1 `movt`, +1 `svc`, -1 `udf` |
| `HangarWindow::render` | +20 16-bit, -10 32-bit, -3 `ldrd`, -1 `strd`, +4 VFP, +1 `movw`, -1 `add_pc`, -1 `ldr_pc`, +3 `String::operator=` calls |
| `Hud::draw` | -10 16-bit, +5 32-bit, +1 `cbz`, +1 `it`, +1 `ldrd`, -1 `strd` |
| `Level::createShip` | +4 `ldrd`, -6 `strd`, +1 `movt`, +1 `udf`, -4 VFP, +1 `__cxa_begin_catch`, +1 `setWreckedMeshId` |
| `Level::createScene` | +16 16-bit, -8 32-bit, +1 `ldrd` |
| `Level::createMission` | +16 16-bit, -8 32-bit, +1 `add_pc`, -2 `cbz`, -1 `it`, +1 `ldr_pc`, +2 `ldrd`, -1 `movw` |
| `MGame::OnTouchEnd` | -2 16-bit, +1 32-bit, +3 `cbz`, -1 `getCampaignMission` |
| `MGame::OnUpdate` | +2 16-bit, -1 32-bit |
| `Radar::draw` | -8 16-bit, +4 32-bit, -8 `cbz`, +2 `it`, -1 `ldrd`, +1 `movw`, call-count deltas |
| `Status::missionCompleted` | -2 16-bit, +1 32-bit, +1 `add_pc`, +1 `ldr_pc` |
| `Globals::Globals` (C1/C2) | +4 16-bit, -2 32-bit |
| `Generator::createMission` | +2 16-bit, -1 32-bit, -3 `add_pc`, +2 `cbz`, -1 `strd`, -1 VFP |
| `PlayerEgo::calcCollision` | +4 16-bit, -2 32-bit, +1 `cbz` |
| `RocketGun::update` | +2 16-bit, -1 32-bit, -1 `strd` |
| `ModStation::ModStation` (C1/C2) | no instruction-pattern delta |
| `MenuTouchWindow::MenuTouchWindow` (C2) | +4 16-bit, -2 32-bit (C1 has no delta) |
| `MenuTouchWindow::OnTouchBegin` | no instruction-pattern delta |
| `SpaceLounge::drawLounge` | no instruction-pattern delta |
| `Gun::calcCharacterCollision` | no instruction-pattern delta |

The `bkpt` delta is exactly the 3 extra `bkpt` instructions in `LevelScript::process`;
the `neon_simd` delta is exactly the 4 extra NEON instructions in `Layout::Layout`.
Neither is a flag-level pattern: `-fno-slp-vectorize` and `-fstack-protector-*` change
thousands of exact functions, while these deltas are local to one non-exact function.

## 3. Artifact-level fingerprints

Command:

```sh
python3 /dev/shm/wb104/artifact_fp.py
python3 /dev/shm/wb104/extab_records.py
```

### 3.1 Sections

| section | original | ours | byte-identical? |
|---|---:|---:|---|
| `.text` | `0x1337c8` | `0x1337c8` | no (24 non-exact functions) |
| `.rodata` | `0x41b67` | `0x41b67` | **yes** |
| `.data` | `0x296c` | `0x296c` | **yes** |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | **yes** |
| `.bss` | `0x69ac` | `0x69ac` | **yes** (NOBITS) |
| `.got` | `0x430c` | `0x430c` | **yes** |
| `.plt` | `0xac4c` | `0xac4c` | **yes** |
| `.rel.dyn` | `0x7048` | `0x7048` | **yes** |
| `.rel.plt` | `0x72d0` | `0x72d0` | **yes** |
| `.dynstr` | `0x2e318` | `0x2e318` | **yes** |
| `.hash` | `0x973c` | `0x973c` | **yes** |
| `.comment` | `0x12f` | `0x12f` | **yes** |
| `.ARM.attributes` | `0x38` | `0x38` | **yes** |
| `.init_array` | `0xc` | `0xc` | **yes** |
| `.fini_array` | `0x8` | `0x8` | **yes** |
| `.ARM.exidx` | `0x6f70` | `0x6f70` | content differs (addresses) |
| `.ARM.extab` | `0x26298` | `0x26294` | **4 bytes short** |
| `.dynsym` | `0x15ca0` | `0x15ca0` | `st_shndx` differs (661 entries) |
| `.note.gnu.build-id` | `0x24` | `0x24` | differs (hash) |
| `.shstrtab` | `0x117` | `0x124` | extra `.missioncase` name |

The extra `.missioncase` section is `0xc0` bytes but is placed inside the existing
padding between `.rodata` and `.fini_array`, so it adds no file bytes. The `.shstrtab`
grows by 13 bytes (the `.missioncase` name), and the file gains one 40-byte section
header. The `.ARM.extab` is 4 bytes shorter, but the following `.rodata` is 16-byte
aligned, so those 4 bytes are absorbed by padding. The file-size delta is `+52` =
`+13` (`.shstrtab`) `+40` (extra header) `-1` (the original has a 1-byte pad before the
section-header table; ours has none).

### 3.2 `.ARM.exidx` / `.ARM.extab`

| observable | original | ours |
|---|---:|---:|
| `.ARM.exidx` entries | 3566 | 3566 |
| CANTUNWIND (`w1 == 1`) | 272 | 272 |
| duplicate targets | 194 | 194 |
| CANT sharing a target with a real entry | 193 | 193 |
| inline compact entries | 856 | 856 |
| extab-referencing entries | 2438 | 2438 |
| functions with an extab record | 2210 | 2210 |
| distinct extab records | 2438 | 2438 |
| `.ARM.extab` size | `0x26298` | `0x26294` |

`readelf --unwind` produces 2438 blocks on both sides. After normalizing the extab
address on the first line, **all 2438 unwind blocks are textually identical** (same
opcodes, same order). The 4-byte size deficit is fully accounted for by five non-exact
functions' LSDA record sizes:

| function | original record size | ours | delta |
|---|---:|---:|---:|
| `LevelScript::process` | 348 | 352 | +4 |
| `HangarWindow::render` | 948 | 940 | -8 |
| `Hud::draw` | 476 | 480 | +4 |
| `Level::createShip` | 336 | 328 | -8 |
| `Generator::createMission` | 172 | 176 | +4 |
| **net** | | | **-4** |

These are landing-pad/LSDA differences inside the 24 non-exact functions, not a flag
difference. `-fno-unwind-tables` is a real regression (see §4).

### 3.3 Relocations and dynamic symbols

| observable | original | ours |
|---|---:|---:|
| `R_ARM_ABS32` | 1817 | 1817 |
| `R_ARM_GLOB_DAT` | 614 | 614 |
| `R_ARM_RELATIVE` | 1162 | 1162 |
| `R_ARM_JUMP_SLOT` | 3674 | 3674 |
| `R_ARM_REL32` | 0 | 0 |
| defined `.dynsym` entries | 5370 | 5370 |
| functions / objects / other | 4524 / 843 / 3 | 4524 / 843 / 3 |
| binding (GLOBAL / WEAK) | 4895 / 475 | 4895 / 475 |
| visibility (DEFAULT / HIDDEN / PROTECTED) | 5370 / 0 / 0 | 5370 / 0 / 0 |
| `_ZTI` / `_ZTS` / `_ZTV` | 176 / 176 / 99 | 176 / 176 / 99 |

The 661 `.dynsym` entries that differ differ **only in `st_shndx`** (the extra
`.missioncase` section is inserted before `.fini_array`, so it shifts the section index
of every symbol in `.fini_array`/`.data.rel.ro`/`.init_array`/`.dynamic`/`.got`/`.data`/
`.bss` by one). All `st_value`, `st_size`, `st_info` and `st_other` fields are identical.
`.rel.plt` is byte-identical (3674/3674 entries, same order).

### 3.4 Notes, attributes, static init

| observable | original | ours |
|---|---|---|
| `.note.android.ident` | `api=16`, `ndk="r18b"`, `build="5063045"` | identical |
| `.note.gnu.gold-version` | `gold 1.12` | identical |
| `.comment` | clang 7.0.2 + GCC 4.9.x | identical |
| `.ARM.attributes` | ARM v7/A, Thumb-2, VFPv3, NEONv1, GOT-indirect, enum int, unaligned v6 | identical |
| `.init_array` | 3 entries: `0x69441`, `0x694c1`, `0x694dd` | identical |
| `.fini_array` | 2 entries: `0x69548`, `0x69534` | identical |

### 3.5 Common symbols

`g_android_back_button_pressed` is a tentative definition in both
`src/platform/android/BackButton.c` and `src/platform/android/GOF2HD2012.c`:

```sh
READELF=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$READELF -sW /dev/shm/wb104/objsec/src_platform_android_BackButton.c.base.o | grep g_android_back_button_pressed
# 322: 00000004 4 OBJECT GLOBAL DEFAULT COM g_android_back_button_pressed
$READELF -sW /dev/shm/wb104/objsec/src_platform_android_BackButton.c.fno_common.o | grep g_android_back_button_pressed
# 322: 00000000 4 OBJECT GLOBAL DEFAULT   7 g_android_back_button_pressed
```

With the default `-fcommon`, both objects emit `SHN_COMMON` and gold merges them; with
`-fno-common` the link fails with a duplicate definition. The linked `.bss` is exact
(`0x69ac`), so the current source's common-symbol handling is consistent with the
original. This is a source-shape dependency, not positive proof that the original used
`-fcommon` (a single-definition original source could have used `-fno-common`).

## 4. A/B builds

### 4.1 Object-level A/B

The exact `compile_commands.json` command is used with one flag changed; every
object-file function's raw bytes are compared with the canonical `-Oz` object. A
variant that changes an **exact** function is observable (and, if the original is
byte-exact, disproven); a variant that changes only non-exact functions is unobservable
on matched code.

Representative TUs: `Engine.cpp`, `Level.cpp`, `Layout.cpp`,
`BlowfishSubkeyFile.c`, `PaintCanvas.cpp`, `StarMap.cpp`, `Radar.cpp`,
`GameText.cpp`, `ParticleSettings.cpp`. Summary (changed exact functions across the
representative TUs):

| variant | changed exact functions | verdict |
|---|---:|---|
| base | 0 | — |
| `-g0` (from `-g`) | 55 | `-g` proven |
| `-fno-jump-tables` | 17 | jump tables proven |
| `-fno-stack-protector` | 118 | strong protector proven |
| `-fstack-protector` (basic) | 118 | basic disproven |
| `-fstack-protector-all` | 442 | all disproven |
| `-fomit-frame-pointer` | 410 | frame pointer proven |
| `-fno-unwind-tables` | 28 | unwind tables proven |
| `-fsized-deallocation` | 67 | sized delete disproven |
| `-fno-slp-vectorize` | 36 | SLP proven |
| `-fno-unroll-loops` | 1 | unrolling proven |
| `-mno-unaligned-access` | 17 | unaligned access proven |
| `-marm` (from `-mthumb`) | 560 | Thumb proven |
| `-mfpu=vfpv3-d16` | 55 | NEON proven |
| `-mfloat-abi=hard` | 120 | softfp proven |
| `-std=c++14` | 1 (`Material::Material`) | c++11 proven |
| `-Os` / `-O2` | 233 / 285 | `-Oz` proven |
| `-ffast-math` | 49 | fast-math disproven |
| `-fno-builtin` | 140 | builtins proven |
| `-fno-strict-aliasing` | 20 | strict aliasing proven |
| `-fwrapv` | 5 | strict overflow proven |
| `-fshort-enums` | 2 | int enums proven |
| `-fvisibility=hidden` | 58 | default visibility proven |
| `-fvisibility-inlines-hidden` | 2 | inlines-visible proven |
| `-mllvm -inline-threshold=0` | 4 | default inlining proven |
| `-mllvm -inline-threshold=1` | 0 | unobservable plateau |
| `-fno-threadsafe-statics` | 1 (`GameText::getText`) | guard variables proven |
| `-fno-use-cxa-atexit` | 1 (`GameText::getText`) | `__cxa_atexit` proven |
| `-fno-sized-deallocation` | 0 | default/unobservable |
| `-fno-common` | 0 | codegen-neutral; link fails on this source |
| `-fmerge-constants` / `-fno-merge-constants` | 0 | unobservable |
| `-falign-functions=4` | 0 | clang 7.0.2 ignores it |
| `-mno-movt` | 0 | no app-TU anchor; libc++abi is prebuilt |
| `-mno-neg-immediates` | 0 | unobservable |
| `-mcrc` | 0 | unobservable |
| `-mstack-alignment=16` | 0 | unobservable |
| `-fno-vectorize` | 0 | loop vectorizer does not fire |
| `-fno-rtti` | 0 code; data changes | RTTI proven by `.rodata`/`.data.rel.ro` |
| `-fdata-sections` | 0 code; section layout changes | absent |
| `-fsigned-char` | 4 (`FileInterfaceAndroid.cpp`, `String.cpp`) | unsigned char proven |

Object section A/B confirm the data-side flags:

| TU / variant | `.text` | `.rodata` | `.data` | `.bss` | sections | common |
|---|---:|---:|---:|---:|---:|---|
| `Globals.cpp` base | 24504 | 14469 | 15 | 1863 | 302 | — |
| `Globals.cpp` `-fno-zero-initialized-in-bss` | 24504 | 14469 | 1875 | 0 | 301 | — |
| `Globals.cpp` `-fdata-sections` | 24504 | 14469 | 15 | 1833 | 463 | — |
| `Globals.cpp` `-fno-merge-constants` | 24504 | 14469 | 15 | 1863 | 302 | — |
| `Globals.cpp` `-falign-functions=16` | 24504 | 14469 | 15 | 1863 | 302 | — |
| `Level.cpp` base / `-fno-merge-constants` | 102458 | 6490 | 68 | 64 | 625 | — |
| `BackButton.c` base | 16 | 0 | 0 | 0 | 26 | `COM` |
| `BackButton.c` `-fno-common` | 16 | 0 | 0 | 4 | 27 | strong |
| `GOF2HD2012.c` base | 184 | 0 | 0 | 40 | 69 | `COM` |
| `GOF2HD2012.c` `-fno-common` | 184 | 0 | 0 | 104 | 69 | strong |

`-fno-zero-initialized-in-bss` moves 1860 bytes from `.bss` to `.data` in `Globals.cpp`;
the original's `.bss`/`.data` are exact, so zero-init-in-bss is on.
`-fno-merge-constants` is neutral on both `.rodata` and code; the original's `.rodata`
is byte-identical, so constant merging is consistent (but the flag itself is
unobservable).

### 4.2 Full-link A/B

`tools/flag_fingerprint.py` was run with `--workdir /dev/shm/wb104/flag_fingerprint`
(29 default variants), plus extra sequential/parallel drivers for flags not in its
default list. All counts are the official verifier's `byte_exact` / `linked_exact` /
size-exact. Baseline: **4500 / 4500 / 4524**, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26294`.

| variant | byte | linked | size | `.text` | `.ARM.exidx` | `.ARM.extab` | verdict |
|---|---:|---:|---:|---:|---:|---:|---|
| base `-Oz` | **4500** | **4500** | **4524/4524** | `0x1337c8` | `0x6f70` | `0x26294` | correct |
| `-fno-vectorize` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral |
| `-fno-trapping-math` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral |
| `-fno-delete-null-pointer-checks` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral |
| `-mllvm -inline-threshold=1` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral |
| `-fno-merge-constants` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral |
| `-falign-functions=4` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral (ignored) |
| `-mno-movt` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral |
| `-fno-sized-deallocation` | 4500 | 4500 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | neutral/default |
| `-mllvm -inline-threshold=0` | 1356 | 4448 | 4498/4524 | `0x133688` | `0x6ff0` | `0x26358` | worse |
| `-mllvm -inline-threshold=10` | 1354 | 4414 | 4462/4519 | `0x1368f8` | `0x6f40` | `0x25264` | worse |
| `-fno-slp-vectorize` | 1484 | 4383 | 4408/4524 | `0x132ec8` | `0x6f70` | `0x26284` | worse |
| `-fno-unroll-loops` | 2883 | 4490 | 4520/4524 | `0x1337a8` | `0x6f70` | `0x262a0` | worse |
| `-fno-jump-tables` | 1363 | 4408 | 4437/4524 | `0x134828` | `0x6f70` | `0x262b4` | worse |
| `-fomit-frame-pointer` | 1298 | 1920 | 2163/4524 | `0x12dfa8` | `0x6868` | `0x214f0` | worse |
| `-fno-stack-protector` | 1377 | 3779 | 3789/4524 | `0x12e128` | `0x6f18` | `0x2607c` | worse |
| `-fstack-protector` (basic) | 1377 | 3783 | 3793/4524 | `0x12e1a8` | `0x6f18` | `0x2607c` | worse |
| `-fstack-protector-all` | 83 | 960 | 981/4524 | `0x14f728` | `0x7230` | `0x270b8` | worse |
| `-fno-unwind-tables` (target option removed) | 2878 | 4498 | 4524/4524 | `0x1337c8` | `0x6678` | `0x25568` | worse |
| `-fno-rtti` | 1701 | 4497 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | worse (data) |
| `-fno-threadsafe-statics` | 1368 | 4492 | 4520/4521 | `0x133684` | `0x6f70` | `0x26258` | worse |
| `-fno-use-cxa-atexit` | 1356 | 4497 | 4523/4524 | `0x133818` | `0x6f70` | `0x26288` | worse |
| `-fno-zero-initialized-in-bss` | 1522 | 4487 | 4516/4524 | `0x133768` | `0x6f70` | `0x26294` | worse |
| `-fsigned-char` | 4442 | 4496 | 4522/4524 | `0x1337c8` | `0x6f70` | `0x26294` | worse |
| `-fno-strict-aliasing` | 1371 | 4349 | 4411/4524 | `0x133ce8` | `0x6f70` | `0x2629c` | worse |
| `-fdata-sections` | 4311 | 4498 | 4524/4524 | `0x1337c8` | `0x6f70` | `0x26294` | worse (data) |
| `-fvisibility-inlines-hidden` | 1278 | 4201 | 4381/4383 | `0x1337a8` | `0x6f70` | `0x26290` | worse |
| `-std=c++14` (C++ only) | 1512 | 4484 | 4514/4524 | `0x133788` | `0x6f70` | `0x26294` | worse |
| `-g0` | 2069 | 4244 | 4485/4524 | `0x1337c8` | `0x6f70` | `0x26294` | worse |
| `-fno-inline` | 1219 | 3509 | 3571/4524 | `0x13c048` | `0x7858` | `0x26998` | worse |
| `-fno-elide-constructors` | 1370 | 4211 | 4234/4524 | `0x13b568` | `0x6fc8` | `0x28048` | worse |
| `-fno-optimize-sibling-calls` | 1316 | 3587 | 3686/4524 | `0x1315b8` | `0x72e8` | `0x262a0` | worse |
| `-marm` | 57 | 235 | 342/4524 | `0x1a3618` | `0x6eb8` | `0x25b1c` | worse |
| `-O1` | 1033 | 1781 | 2676/4524 | `0x143fc8` | `0x7840` | `0x26278` | worse |
| `-O2` | 1168 | 2157 | 2411/4296 | `0x163208` | `0x6880` | `0x250b0` | worse |
| `-O3` | 1167 | 2117 | 2247/4295 | `0x1717b8` | `0x6838` | `0x25470` | worse |
| `-Os` | 1263 | 2648 | 2883/4356 | `0x14bf08` | `0x6a30` | `0x24770` | worse |
| `-ffast-math` | 1335 | 4222 | 4369/4524 | `0x132998` | `0x6f58` | `0x2627c` | worse |
| `-fno-math-errno` | 1356 | 4486 | 4514/4524 | `0x133638` | `0x6f58` | `0x2626c` | worse |
| `-freciprocal-math` | 1530 | 4402 | 4483/4524 | `0x133828` | `0x6f70` | `0x2629c` | worse |
| `-fwrapv` | 1398 | 4461 | 4500/4524 | `0x1338c8` | `0x6f70` | `0x26294` | worse |
| `-fno-strict-overflow` | 1398 | 4461 | 4500/4524 | `0x1338c8` | `0x6f70` | `0x26294` | worse |
| `-fno-builtin` | 1350 | 3985 | 4118/4524 | `0x1347b8` | `0x6fb0` | `0x26378` | worse |
| `-fno-common` | — | — | — | — | — | — | **link fail** (`g_android_back_button_pressed`) |
| `-fno-exceptions` | — | — | — | — | — | — | **build fail** (`Level.cpp:5214: try`) |
| `-mfloat-abi=hard` | — | — | — | — | — | — | **link fail** (VFP args vs softfp FMOD) |
| `-mfpu=vfpv3-d16` | — | — | — | — | — | — | **build fail** (NEON intrinsics) |

`-mllvm -inline-threshold=5` and the parallel `-falign-functions=4` run hit the shared
vendored-libcrypto build race (`third_party/openssl/_obj_gof2`); the sequential
`-falign-functions=4` build is neutral, and `=5` is `-Oz`'s default. `-fno-unwind-tables`
appended to `CMAKE_CXX_FLAGS` is neutral because the target's `-funwind-tables` comes
later on the command line; the sequential build that removes the target option is the
valid row above.

## 5. Positive/negative evidence table

`PROVEN` = a positive fingerprint in the shipped binary plus an A/B rebuild that moves
away from it. `UNOBSERVABLE` = no code/data difference on this source (cannot be
excluded). `DISPROVEN` = the alternative is worse or fails.

| setting | status | positive evidence | A/B evidence |
|---|---|---|---|
| NDK r18b clang 7.0.2 | **PROVEN** | `.note.android.ident` build 5063045; `.comment` clang 7.0.2 | — |
| API 16 | **PROVEN** | `.note.android.ident` api=16; APK `minSdkVersion=16` | — |
| `-march=armv7-a` | **PROVEN** | `.ARM.attributes` ARM v7/A; `movw`/`movt`; Thumb-2 | `-mcpu=cortex-a9` much worse (prior wave) |
| `-mthumb` | **PROVEN** | all 4524 exported functions at even addresses; Thumb-2 | `-marm` 57 / 235 / 342 |
| `-mfpu=neon` | **PROVEN** | NEONv1; 16 NEON SIMD instructions | `-mfpu=vfpv3-d16` build fail; 55 exact changes |
| `-mfloat-abi=softfp` | **PROVEN** | ELF flags `0x5000200`; no `Tag_ABI_VFP_args` | `-mfloat-abi=hard` link fail; 120 exact changes |
| `-Oz` | **PROVEN** | 4500 vs 1033/1168/1167/1263 for `-O1/-O2/-O3/-Os` | per-TU `-Oz` distinguished from `-O2` for 184/192 TUs |
| `-g` | **PROVEN** | `-g0` 2069; `InitializeBlowfish` entry-copy schedule | 55 exact changes in object A/B |
| `-std=c++11` | **PROVEN** | `Material::Material` 276 vs 272 under `-std=c++14` | `-std=c++14` 1512 |
| `-fPIC` | **PROVEN** (PIC); `-fpic` spelling **UNOBSERVABLE** | no `R_ARM_REL32`/`DT_TEXTREL`; 614 `GLOB_DAT`; `.got` exact | `-fpic` byte-identical (prior wave) |
| default visibility | **PROVEN** | 5370 DEFAULT; 4895 GLOBAL + 475 WEAK | `-fvisibility=hidden` 57; `-fvisibility-inlines-hidden` 1278 |
| `-fstack-protector-strong` | **PROVEN** | 787 canary calls; basic/all differ | basic 1377/3783; all 83/960; none 1377/3779 |
| frame pointers kept | **PROVEN** | 2476 `add r7,sp` + 220 `mov r7,sp`; 2690 `push {...,r7,lr}` | `-fomit-frame-pointer` 1298 |
| `-funwind-tables` | **PROVEN** | 3566 EXIDX, 272 CANT, 193 CANT+real, 2210 functions with extab | `-fno-unwind-tables` 2878; `.ARM.exidx` 0x6678 |
| `-ffunction-sections` | **PROVEN** | 193 CANT+real pairs; object 394 vs 92 sections | — |
| `-frtti` | **PROVEN** | 176 `_ZTI`/`_ZTS`, 99 `_ZTV`; `.rodata`/`.data.rel.ro` exact | `-fno-rtti` 1701; `.rodata` 0x4104e |
| `-fexceptions` | **PROVEN** | `_Unwind_Resume` 541, `__cxa_throw` 14, `__gxx_personality_v0` 112 | `-fno-exceptions` build fail |
| `-fno-sized-deallocation` | **PROVEN** (default) | 1058 `_ZdlPv`, 252 `_ZdaPv`, 0 `_ZdlPvj`/`_ZdaPvj` | `-fsized-deallocation` 1347 (prior); neutral `-fno-sized-deallocation` |
| thread-safe statics | **PROVEN** | 2 `__cxa_guard_acquire` calls | `-fno-threadsafe-statics` 1368; `GameText::getText` changes |
| `__cxa_atexit` | **PROVEN** | 2 `__cxa_atexit`, 0 `atexit` | `-fno-use-cxa-atexit` 1356; `GameText::getText` changes |
| strict aliasing | **PROVEN** | `Engine::RenderMesh` 1040 vs 1032 under `-fno-strict-aliasing` | `-fno-strict-aliasing` 1371; 20 exact changes |
| `-fmath-errno` (default) | **PROVEN** | 31 `__errno` refs | `-fno-math-errno` 1356 |
| zero-init in `.bss` | **PROVEN** | `.bss` exact `0x69ac` | `-fno-zero-initialized-in-bss` 1522; `Globals.cpp` 1863→0 |
| unsigned `char` | **PROVEN** | — | `-fsigned-char` 4442; `FileInterfaceAndroid.cpp`/`String.cpp` anchors |
| int-sized enums | **PROVEN** | `.ARM.attributes` enum int | `-fshort-enums` 2869 (prior); 2 exact anchors |
| no `-fdata-sections` | **PROVEN** | `.rodata` exact `0x41b67`; `.data` exact | `-fdata-sections` 4311; object 302→463 sections |
| SLP vectorization | **PROVEN** | 16 NEON SIMD instructions | `-fno-slp-vectorize` 1484 |
| loop unrolling | **PROVEN** | — | `-fno-unroll-loops` 2883 |
| jump tables | **PROVEN** | 152 `tbb`/`tbh` | `-fno-jump-tables` 1363 |
| unaligned access | **PROVEN** | `.ARM.attributes` unaligned v6 | `-mno-unaligned-access` 1404 (prior); 17 exact anchors |
| `-stdlib=libc++` | **PROVEN** | `__cxa_*`/`__cxa_guard_acquire` bytes match NDK r18b `libc++abi.a`; no libstdc++ symbols | — |
| `-fmerge-constants` | **UNOBSERVABLE** | `.rodata` byte-identical | `-fno-merge-constants` 4500; object `.rodata` identical |
| `-falign-functions=4` | **UNOBSERVABLE** | function-start alignment/gap distributions identical | clang 7.0.2 warns "not supported"; 4500 |
| `-mno-movt` | **UNOBSERVABLE** | no real code `movt` in app TUs (only libc++abi `std::terminate` and literal-pool words) | 4500 |
| `-mno-neg-immediates`, `-mcrc`, `-mstack-alignment=16` | **UNOBSERVABLE** | no pattern difference | 4500 |
| `-fno-vectorize` | **UNOBSERVABLE** | loop vectorizer does not fire at `-Oz` | 4500 |
| `-fno-trapping-math`, `-fno-delete-null-pointer-checks` | **UNOBSERVABLE** | — | 4500 |
| `-fno-plt` | **UNOBSERVABLE** | — | prior wave: 4500 |
| `-fPIC` vs `-fpic`, `-std=c++11` vs `gnu++11` | **UNOBSERVABLE** | byte-identical | prior wave |
| `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16` | **UNOBSERVABLE** as macros | API 16 proven by note | prior wave: 4500 |
| `-fcommon` | **CONSISTENT** (source-dependent) | `.bss` exact; current source needs it | `-fno-common` link fail (duplicate tentative definition) |
| `-fno-inline` | **DISPROVEN** | — | 1219 / 3509 / 3571 in this pass; object A/B changes many exact functions |
| `-fno-elide-constructors` | **DISPROVEN** | — | 1370 / 4211 / 4234 in this pass |
| `-fno-optimize-sibling-calls` | **DISPROVEN** | — | 1316 / 3587 / 3686 in this pass |
| `-fvisibility=protected` | **DISPROVEN** | — | 1345 (prior) |
| `-mexecute-only` | **DISPROVEN** | — | 1299 (prior) |
| `-mstackrealign` | **DISPROVEN** | — | 80 (prior) |
| `-momit-leaf-frame-pointer` | **DISPROVEN** | — | 1390 (prior) |
| `-mthread-model single` | **DISPROVEN** | — | 2085 (prior) |
| `-mlong-calls` | **DISPROVEN** | — | 1345 (prior) |
| `-mno-implicit-float` | **DISPROVEN** | — | 1357 (prior) |
| `-mcpu=cortex-a9` | **DISPROVEN** | — | 1230 (prior) |
| `-fno-honor-nans` | **DISPROVEN** | — | 1429 (prior) |
| `-fno-signed-zeros` | **DISPROVEN** | — | 2355 (prior) |
| `-fno-zero-initialized-in-bss` | **DISPROVEN** | — | 1522 |
| `-fno-threadsafe-statics` | **DISPROVEN** | — | 1368 |
| `-fno-use-cxa-atexit` | **DISPROVEN** | — | 1356 |
| `-fno-rtti` | **DISPROVEN** | — | 1701 |
| `-fno-exceptions` | **DISPROVEN** | — | build fail |
| `-fsized-deallocation` | **DISPROVEN** | — | 1347 (prior) |
| `-fdata-sections` | **DISPROVEN** | — | 4311 |
| `-fshort-enums` | **DISPROVEN** | — | 2869 (prior) |
| `-fsigned-char` | **DISPROVEN** | — | 4442 |
| `-fno-builtin` | **DISPROVEN** | — | 1350 |
| `-fno-strict-aliasing` | **DISPROVEN** | — | 1371 |
| `-fwrapv` / `-fno-strict-overflow` | **DISPROVEN** | — | 1398 |
| `-ffast-math` | **DISPROVEN** | — | 1335 |
| `-fno-math-errno` | **DISPROVEN** | — | 1356 |
| `-freciprocal-math` | **DISPROVEN** | — | 1530 |
| `-fno-slp-vectorize` | **DISPROVEN** | — | 1484 |
| `-fno-unroll-loops` | **DISPROVEN** | — | 2883 |
| `-fno-jump-tables` | **DISPROVEN** | — | 1363 |
| `-fno-stack-protector` / basic / all | **DISPROVEN** | — | 1377 / 1377 / 83 |
| `-fomit-frame-pointer` | **DISPROVEN** | — | 1298 |
| `-fno-unwind-tables` | **DISPROVEN** | — | 2878 |
| `-marm` | **DISPROVEN** | — | 57 |
| `-mfloat-abi=hard` | **DISPROVEN** | — | link fail |
| `-mfpu=vfpv3-d16` | **DISPROVEN** | — | build fail |
| `-std=c++14` | **DISPROVEN** | — | 1512 |
| `-g0` | **DISPROVEN** | — | 2069 |
| `-O1/-O2/-O3/-Os` | **DISPROVEN** | — | 1033/1168/1167/1263 |
| `-fvisibility=hidden` | **DISPROVEN** | — | 57 |
| `-fvisibility-inlines-hidden` | **DISPROVEN** | — | 1278 |
| `-mllvm -inline-threshold=0/10/25/50/100` | **DISPROVEN** | — | 1356/1354/1352/1350/1350 |

## 6. Patterns in the original that the current build does not reproduce

These are the only non-reproduced patterns found. None is a compile-flag difference.

1. **24 non-exact function bodies.** Their local instruction-pattern deltas are listed in
   §2. The 4500 exact functions have zero pattern deltas. The deltas are register
   allocation, scheduling, 16/32-bit encoding, basic-block order, and local
   vectorization/conditional-execution differences.
2. **`.ARM.extab` is 4 bytes short.** All 2438 unwind blocks have identical opcode text;
   the deficit is exactly the net of five non-exact functions' LSDA record sizes
   (`LevelScript::process` +4, `HangarWindow::render` -8, `Hud::draw` +4,
   `Level::createShip` -8, `Generator::createMission` +4).
3. **`.ARM.exidx` content differs** (address words and LSDA ranges) but the structure is
   identical: 3566 entries, 272 CANT, 194 duplicate targets, 193 CANT+real, 856 inline
   compact, 2438 extab references.
4. **`.dynsym` `st_shndx` differs for 661 entries** because the build adds the extra
   `.missioncase` section, shifting `.data`/`.bss` section indices by one. All
   `st_value`/`st_size`/`st_info`/`st_other` fields are identical.
5. **`.note.gnu.build-id` differs** (the original's is the SHA-1 of its unstripped gold
   output; expected).
6. **`.shstrtab` differs** because of the extra `.missioncase` section name; the file is
   `+52` bytes (`+13` name, `+40` header, `-1` pad; the `.missioncase` content and the
   4-byte `.ARM.extab` deficit are absorbed by existing/alignment padding).
7. **No flag-level mismatch.** Every flag-family fingerprint that is observable on the
   exact functions is reproduced: stack protector, frame pointer, unwind tables,
   exceptions/RTTI, static init/atexit, guard variables, jump tables, division helpers,
   memcpy/memset inlining, NEON/VFP, Thumb, literal-pool alignment, function alignment,
   constant merging, common symbols, visibility, and inlining.

## 7. Proven vs still hypothesis

**Proven by the measurements above:**

* The shipped binary is NDK r18b clang 7.0.2, API 16, armeabi-v7a, Thumb-2, NEON,
  softfp, gold 1.12, libc++.
* The current compile recipe is correct: `-Oz -g -std=c++11 -fPIC -frtti
  -fstack-protector-strong -ffunction-sections -funwind-tables -DANDROID -DNDEBUG
  -stdlib=libc++`, default visibility, frame pointers, strict aliasing, math errno,
  SLP vectorization, loop unrolling, jump tables, unaligned access, zero-init-in-bss,
  unsigned char, int enums, no sized delete, thread-safe statics, `__cxa_atexit`.
* No tested flag is a strict improvement over the current recipe.
* All whole-`.text` pattern deltas are confined to the 24 non-exact functions.
* The 4-byte `.ARM.extab` deficit is fully accounted for by five non-exact functions'
  LSDA records.

**Still hypothesis / not settled:**

* Exact `-fPIC` vs `-fpic` spelling; exact `-std=c++11` vs `-std=gnu++11` spelling.
* `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16` as macros (API 16 itself is proven).
* `-fmerge-constants` (neutral on this source).
* `-falign-functions=4` (ignored by clang 7.0.2; the default alignment is correct).
* `-mno-movt` (no app-TU anchor; libc++abi is prebuilt).
* `-mno-neg-immediates`, `-mcrc`, `-mstack-alignment=16`, `-fno-vectorize`,
  `-fno-trapping-math`, `-fno-delete-null-pointer-checks`, `-fno-plt` (neutral).
* `-fcommon` vs a single-definition `-fno-common` original source (the current source
  requires `-fcommon`, but the shipped binary cannot distinguish the two).
* The remaining 24 non-exact functions are source-shape/TU-shape work, not flag work.

## 8. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/wb104
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# whole-.text census
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d -M force-thumb /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/wb104/orig.dis
$OBJ -d -M force-thumb cmake-build-match/libgof2hdaa.so > /dev/shm/wb104/ours.dis
python3 /dev/shm/wb104/scan2.py
python3 /dev/shm/wb104/perfunc.py
python3 /dev/shm/wb104/artifact_fp.py
python3 /dev/shm/wb104/extab_records.py

# full-link default matrix (29 variants)
python3 tools/flag_fingerprint.py --mode variants \
  --workdir /dev/shm/wb104/flag_fingerprint --jobs 8

# extra full-link variants (sequential/parallel scratch drivers)
python3 /dev/shm/wb104/run_extra_variants.py
python3 /dev/shm/wb104/run_seq_variants.py
python3 /dev/shm/wb104/run_stack_variants.py
python3 /dev/shm/wb104/run_unwind_exc.py
python3 /dev/shm/wb104/run_inline_variants.py

# object-level A/B
python3 /dev/shm/wb104/ab_tu.py
python3 /dev/shm/wb104/ab_tu2.py
python3 /dev/shm/wb104/ab_sections.py
python3 /dev/shm/wb104/scan_flag_anchor.py
```

No `--update-baseline` was run. `tools/verify_baseline.json` is unchanged.

## 9. Gate

| state | byte_exact | linked_exact | size-exact | data | imports | lint | drift |
|---|---:|---:|---:|---:|---:|---|---:|
| before | 4500 | 4500 | 4524/4524 | 843/843 | 207 | CLEAN | 0 |
| after | 4500 | 4500 | 4524/4524 | 843/843 | 207 | CLEAN | 0 |

`RATCHET PASS`. No source or build-flag change was landed; this commit contains only this
note.
