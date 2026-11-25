# Build-flags research: validating the CMake/NDK r18b flag set (wave192)

Status: research note, 2026-11-25. Worktree `wt/w14240`, branch `wave192-w14240`,
base master `8b830af3a`. Research only: **no source, build-flag, or baseline change**
is landed. This commit contains this note only. Scratch work:
`/home/debian/GalaxyOnFire2/scratch/w14240/`.

Ground truth: `_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`, 2,187,328 B).
Ours: `cmake-build-match/libgof2hdaa.so`
(sha256 `51153a50…b7d6a9ca`, 2,187,328 B).

## 0. Bottom line

The flags documented in `AGENTS.md` / `cmake/toolchain.cmake` / `CMakeLists.txt` are
**correct and independently corroborated by the ELF artifact**. Specifically:

* Toolchain = **NDK r18b (18.1.5063045), clang 7.0.2, GNU gold 1.12**, API 16,
  armeabi-v7a, **softfp**, Thumb-2, VFPv3, NEON. All proven from
  `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`,
  `e_flags`.
* Compile flags = the CMake r18b toolchain defaults, with **`-fdata-sections` OFF**
  (`-ffunction-sections -funwind-tables` on), `-Oz -g -std=c++11 -fPIC -frtti
  -fstack-protector-strong -mthumb -mfpu=neon -mfloat-abi=softfp`. Every data/ABI
  section is byte-identical, which is the sensitive test for these.
* Link = single plain gold `-shared -soname libgof2hdaa.so --build-id -z now
  --exclude-libs=libgcc.a`, then GNU `arm-linux-androideabi-strip --strip-all`,
  then `tools/reorder_shstrtab.py`. ELF header, all 8 program headers, the dynamic
  section (incl. `DT_NEEDED` order), and **all 27 section headers are byte-identical**.
* The residual is **pure per-TU codegen + source shape**, not a global flag:
  `.text` 78,809 diff bytes, `.ARM.extab` 556, `.note.gnu.build-id` 20, and a
  **4-byte `.rel.plt` swap that is a source/PLT-reference-order artifact** —
  *not* a flag difference.
* No global flag change is expected to raise `byte_exact`; any global change would
  regress the 4,508 already-exact functions. Remaining levers are per-TU
  instruction-selection/scheduling knobs (list in §9).

This supersedes `build-flags-evidence-wave192.md`, whose "rel.plt byte-identical",
"`.text` 76,877", "extab 597", and "15 residual" figures are stale.

## 1. Method and artifacts

Reproduced with the checked-in `cmake-build-match` build (already configured/built;
not reconfigured) and read-only tools on both `.so` files:

```sh
O=../_work/bins/android_2.0.16_libgof2hdaa.so
U=cmake-build-match/libgof2hdaa.so
readelf -h/-lW/-SW/-dW/-A/-p .comment/-x .note.android.ident   "$O" "$U"
python3 scratch/w14240/diffsec.py  "$O" "$U"     # per-section raw-byte diff
python3 scratch/w14240/relplt.py   "$O" "$U"     # .rel.plt entry-by-entry diff
```

`diffsec.py` walks both section-header tables, asserts each header's
name/type/addr/offset/size match, then does a raw byte `memcmp` of every section's
file range (skipping `SHT_NOBITS`). `relplt.py` decodes each `Elf32_Rel` and resolves
the symbol name through `.dynsym`/`.dynstr`, so the `.rel.plt` residual is identified
by name, not just bytes.

## 2. ELF-level evidence (all identical unless noted)

| observable | original | ours | verdict |
|---|---|---|---|
| ELF header | `e_flags 0x5000200`, EABI v5, soft-float | same | identical |
| program headers | 8 PHDRs | 8 PHDRs | `readelf -lW` diff empty |
| section headers | 27 sections, `e_shstrndx 26`, `e_shoff 0x215c08` | same | `readelf -SW` diff empty (name/type/flags/addr/off/size/link/info/align/entsize) |
| dynamic section | 36 entries | same | `readelf -dW` diff empty |
| `DT_NEEDED` order | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | same | identical |
| `DT_FLAGS`/bind | `BIND_NOW`, SONAME `libgof2hdaa.so` | same | identical |
| `.note.gnu.gold-version` | `GNU` / `gold 1.12` | same | identical |
| `.note.android.ident` | `Android\0`, desc word `0x10` (API 16), `r18b`, `5063045` | same | identical |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2 (…00310037…) (…1d739ffb…) (based on LLVM 7.0.2svn)` + `GCC: (GNU) 4.9.x 20150123 (prerelease)` | same | **byte-identical** |
| `.ARM.attributes` | 56 B, 18 tags (below) | same | **byte-identical** |

`.ARM.attributes` (identical on both sides) pins the ABI flags:

```text
Tag_CPU_name "ARM v7"        Tag_CPU_arch v7      Tag_CPU_arch_profile Application
Tag_ARM_ISA_use Yes          Tag_THUMB_ISA_use Thumb-2
Tag_FP_arch VFPv3            Tag_Advanced_SIMD_arch NEONv1
Tag_ABI_PCS_GOT_use GOT-indirect   Tag_ABI_PCS_wchar_t 4
Tag_ABI_FP_denormal Needed   Tag_ABI_FP_exceptions Needed
Tag_ABI_FP_number_model IEEE 754   Tag_ABI_align_needed 8-byte
Tag_ABI_enum_size int        Tag_ABI_HardFP_use Deprecated
Tag_CPU_unaligned_access v6  Tag_ABI_FP_16bit_format IEEE 754
```

Consequences that *rule flags in/out*:

* `Tag_ABI_HardFP_use: Deprecated` + `Tag_FP_arch VFPv3` ⇒ **softfp**
  (`-mfloat-abi=softfp`), matching the documented flag.
* `Tag_CPU_unaligned_access: v6` ⇒ unaligned access **enabled**;
  `-mno-unaligned-access` is ruled out.
* `Tag_CPU_arch v7`, `Thumb-2`, `NEONv1`, `VFPv3` ⇒ `-march=armv7-a -mthumb
  -mfpu=neon` correct (`-mfpu=vfpv3-d16` alone would not emit NEON and would
  change the tag).
* `wchar_t 4`, `enum int`, `8-byte` alignment ⇒ standard AAPCS defaults; no
  `-fshort-wchar` / `-fshort-enums` / `-mabi=` override.

`.note.android.ident` decodes to exactly NDK r18b release **18.1.5063045** and
**API level 16** (`desc` first word `0x10`). This is the authoritative build-tool
stamp.

## 3. Per-section raw-byte residual

`diffsec.py` (2026-11-25), file total difference **79,389 B**:

| section | addr | size (dec / hex) | diff bytes |
|---|---|---:|---:|
| `.note.gnu.build-id` | `0x1cc` | 36 / `0x24` | 20 |
| `.rel.plt` | `0x5751c` | 29,392 / `0x72d0` | 4 |
| `.text` | `0x69440` | **1,259,464 / `0x1337c8`** | 78,809 |
| `.ARM.extab` | `0x1a3b78` | 156,312 / `0x26298` | 556 |
| `.ARM.exidx` | `0x19cc08` | 28,528 / `0x6f70` | 0 |

Every other section is a 0-byte diff, including `.rodata` (269,159),
`.data` (10,604), `.data.rel.ro` (12,264), `.dynsym` (89,248), `.dynstr` (189,208),
`.hash` (38,716), `.rel.dyn` (28,744), `.plt` (44,108), `.dynamic` (328),
`.got` (17,164), `.init_array` (12), `.fini_array` (8), `.comment` (303),
`.note.android.ident` (152), `.gnu.version*`, `.note.gnu.gold-version`,
`.ARM.attributes`, `.shstrtab`; `.bss` is `SHT_NOBITS`.

The `.ARM.extab` entries are 4-byte-aligned records, so the **aligned** and **raw**
residual are both **556 B** — this is a net `.text`+`.ARM.extab` trade, not an
alignment artifact.

> Note: older notes/docs state `.text` size `1,258,440`; the correct decimal for
> `0x1337c8` is **1,259,464** (verified `readelf -SW`). Older docs also state
> `.text` diff `78,810`; the measured value is **78,809**.

## 4. `.rel.plt` residual is a source-order artifact, not flags

`relplt.py`: 3,674 relocation entries, **exactly 2 differ**. Both are
`R_ARM_JUMP_SLOT` (type 22) and share the same r_offset pair
(`0x2132e4` / `0x2132e8`); only the target symbol is swapped:

| index | original | ours |
|---|---|---|
| 2835 | `_ZN13PlayerStation10setVisibleEb` | `_ZN9PlayerEgo17deleteHackingGameEv` |
| 2836 | `_ZN9PlayerEgo17deleteHackingGameEv` | `_ZN13PlayerStation10setVisibleEb` |

This is the order in which the two functions were first referenced (PLT slot
allocation follows first-call discovery order). It is fixed by TU/link source order,
not by any compile/link flag, and it is a 4-byte difference. `docs/IDENTICALITY.md`
and `docs/VALIDATION.md` currently claim `.rel.plt` is byte-identical; that is
stale (see §8).

## 5. Flag-by-flag validation

| documented flag | evidence it is correct |
|---|---|
| `-target armv7-none-linux-androideabi16` | `.note.android.ident` API 16; `e_flags` EABI v5; `.ARM.attributes` v7 App profile |
| `-march=armv7-a -mthumb` | `Tag_CPU_arch v7`, `Tag_THUMB_ISA_use Thumb-2` |
| `-mfpu=neon` | `Tag_Advanced_SIMD_arch NEONv1`, `Tag_FP_arch VFPv3` |
| `-mfloat-abi=softfp` | `e_flags` soft-float + `Tag_ABI_HardFP_use Deprecated` |
| `-std=c++11` | signed `new T[n]` guard in binary; required by toolchain; libc++ static |
| `-fPIC` | GOT-indirect access (`Tag_ABI_PCS_GOT_use`), no `DT_TEXTREL`, `.rel.dyn` matches |
| `-frtti` | RTTI/`typeinfo` symbols present and defined; `.rodata` byte-identical |
| `-fstack-protector-strong` | stack-canary codegen; `.rodata`/`.data` byte-identical (canary global) |
| `-ffunction-sections` | 193 empty-`.text` CANTUNWIND sentinels in `.ARM.exidx` (see CMakeLists note) |
| `-funwind-tables` | `.ARM.exidx` byte-identical (28,528 B) and `.ARM.extab` size/base exact |
| `-Oz` | `-Oz`-specific size; every function size-exact |
| `-g` | debug info is a codegen knob at clang 7 `-Oz` (PostRA sink); prior A/B measured linked 2779→2856 |
| `-DANDROID -DNDEBUG` | r18b CMake Release compile line; behavior matches binary |
| `-stdlib=libc++` (static) | no `libc++_shared.so` NEEDED; `__cxa_*` defined; no `std::__ndk1` imports |
| **no `-fdata-sections`** | `.rodata`/`.data`/`.data.rel.ro` byte-identical; ndk-build default would set it ON (see §6) |
| link: `gold -shared --build-id -z now --exclude-libs=libgcc.a` | gold 1.12 note, `BIND_NOW`, ELF/PHDR/dyn identical, build-id is SHA-1 over unstripped output |
| strip: GNU `arm-linux-androideabi-strip --strip-all` | `.shstrtab`/section offsets match bfd-normalized layout; w3901 proved `strip == APK .so` |
| `reorder_shstrtab.py` | `.shstrtab` diff 0; 27 section headers identical |

`-fPIC` vs `-fpic` is not distinguishable here (GOT fits the small model; both
byte-identical on this source). `-fPIC` is CMake's spelling, consistent with the
CMake provenance.

## 6. CMake vs ndk-build (reconciliation with w3901)

`_work/reconstructions/build_provenance_w3901.md` established the shipped library
was produced by the **NDK r18b CMake toolchain on a macOS host**, not ndk-build,
from two independent fingerprints that a flag-set comparison reproduces:

| observable | original | CMake r18b default | ndk-build r18b default |
|---|---|---|---|
| `-fdata-sections` | **off** (data layout) | **off** | **on** |
| link order | FMOD before libc++abi | **FMOD before libc++abi** | STL archives before FMOD |
| `_Unwind_*` | **UND** | **UND** | defined locally |
| `__gnu_Unwind_Find_exidx` | **absent** | **absent** | **UND** |
| `DT_NEEDED` order | log…fmodex, fmodevent, m, dl, c | **same** | differs via STL order |

Confidence: CMake 90%, ndk-build 2% (w3901 §1). The current tree's shorter link
line (plain gold, explicit `--exclude-libs=libgcc.a`, FMOD before libc++ archives,
no `-s`) was A/B-confirmed byte-equivalent to the full CMake default link set
(w3901 §6.1 canonical sha `88aff115…`, gate 4498/4498 at that time). The current
tree deliberately omits `-fdata-sections` (wave-141/w3420 A/B: enabling it costs
−71 `byte_exact`; `_work/reconstructions/android_mk_reconstruction.md`), which is
exactly what the CMake provenance predicts.

The current tree's per-TU knobs are also consistent with "per-TU codegen, not a
global flag wrong": StarMap branch-weight 7:1; LevelScript
`-disable-postra-machine-sink`; Radar `-disable-postra-machine-licm
+cheap-predicable-cpsr`; Level `-enable-earlycse-memssa=false`; Layout
`+prof-unpr`; StatusMissionTable `-marm -fno-function-sections
-fno-unwind-tables`.

## 7. Residual interpretation

Since the ELF/PHDR/dynamic/section-header layer, `.ARM.attributes`, `.comment`,
`.ARM.exidx`, `.rodata`, `.data`, `.data.rel.ro`, `.dynsym`/`.dynstr`/`.hash`,
`.rel.dyn`, `.got`, `.plt` and the Android/gold/NDK notes are all **byte-identical**,
the only possible remaining discrepancy is code generation and source shape:
the 16 non-`byte_exact` functions (all size-exact). The `.text` 78,809 +
`.ARM.extab` 556 residual is consistent with `-Oz` per-TU scheduling/register
allocation differences on those functions. The 4-byte `.rel.plt` swap is source
ordering. A wrong global flag would have perturbed a data/ABI/reloc section; none
is perturbed.

## 8. Stale documentation to correct (not edited here — flagged only)

Per AGENTS.md, doc numbers should be updated *in the same commit as the pass that
changes them*. This research pass changes no gate number, so the following are
left as findings for the owning pass:

* `docs/IDENTICALITY.md:33,37` — `.text` delta stated as **78,810**; measured **78,809**.
* `docs/VALIDATION.md:114` — `.text` delta **78,810**; measured **78,809**.
* `docs/IDENTICALITY.md:415-416` etc. and `docs/VALIDATION.md:52/99/115` — claim
  `.rel.plt` is **byte-identical**; measured **4 bytes / 2 entries swapped**
  (`PlayerStation::setVisible` ↔ `PlayerEgo::deleteHackingGame`, §4).
* `_work/reconstructions/build-flags-evidence-wave192.md` — stale: says rel.plt
  identical, `.text` 76,877, extab 597, 15 residual functions. Superseded by this note.
* Older notes quoting `.text` size `1,258,440` — correct is **1,259,464** (`0x1337c8`).

## 9. Candidate per-TU flags still worth trying (future work)

All are instruction-selection/scheduling switches that must be applied per-TU via
`tools/per_tu_flags.cmake` + `tools/flag_fingerprint.py` (override `--workdir` to
scratch; default is `/dev/shm`). Global application is expected to regress the
4,508 exact functions.

```
-fno-strict-aliasing   -fwrapv   -fno-delete-null-pointer-checks
-fno-sized-deallocation   -fno-threadsafe-statics   -fvisibility=hidden
-fomit-frame-pointer / -fno-omit-frame-pointer
-fno-math-errno   -fno-exceptions   -fno-rtti
-fno-inline-functions   -fno-vectorize   -fno-slp-vectorize
-fno-jump-tables   -fno-common   -fno-builtin
```

These do not change the ABI/attribute/data artifacts already proven identical, so
any win would be local codegen only.

## 10. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w14240
python3 scratch/w14240/diffsec.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
python3 scratch/w14240/relplt.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
readelf -SW/-lW/-dW/-A -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
```

No gate number changed by this research; `check` remains green at byte 4508/4524,
linked 4508/4524, size 4524/4524, data 843/843, imports 207, stubs/missing/extra 0,
lint CLEAN, drift 0, parity 5370/5370.
