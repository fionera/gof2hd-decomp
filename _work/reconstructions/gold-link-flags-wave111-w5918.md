# Gold 1.12 link-command archaeology and link-flag A/B (w5918, wave 111)

Status: research note, 2026-11-04. Worktree `wt/w5918`, branch `wave111-w5918`, base
`8eea060cc` (wave-110 harvest). Scratch: `/dev/shm/w5918`. **No `src/` or build-flag change is
landed**: the current CMake/gold recipe is independently re-derived from the shipped ELF and
confirmed to be the unique minimal recipe that reproduces every observable. No candidate link
flag improves the gate.

## 0. Bottom line

* Baseline is green: `byte_exact 4500/4524`, `linked_exact 4500/4524`, `size_exact 4524/4524`,
  `data_exact 843/843`, imports `207`, `lint CLEAN`, `drift 0`, file size `2,187,328` B.
* The checked-in link command, relinked from the exact `ninja` command line, is byte-identical to
  the checked-in `.so` (sha256 `47d00b82a5e928af...`). The original is
  `d4c17be61af96272...`.
* **No tested link flag improves `byte_exact` or `linked_exact`.** The best measured variant is
  the current recipe, `4500/4500`. The only flag that lowers `linked_exact` is `--icf=all`
  (`4499`); `--icf=safe` keeps `linked_exact 4500` but drops `byte_exact` to `1347`.
* The remaining whole-`.so` deltas are source-body/artifact deltas, not link-flag deltas:
  `.text` has 95,999 differing bytes at the exact original size; `.ARM.exidx` has 1,683
  differing bytes at the exact original size; `.ARM.extab` is 4 B short; `.rodata` is 0xc0 B
  long because of the decomp-only `Status::missionCompleted` rank table appended at `0x20b977`;
  `.note.gnu.build-id` is 20 bytes (SHA-1 of the original unstripped link output).
* `.rel.plt`, `.dynsym`, `.dynstr`, `.hash`, `.plt`, `.got`, `.rel.dyn`, `.data`,
  `.data.rel.ro`, `.dynamic`, `.bss` and every other named section are byte-identical to the
  original. The wave-100 `.rel.plt` 6-slot rotation is no longer present in the current tree.

## 1. Baseline and method

```sh
cd /home/debian/GalaxyOnFire2/wt/w5918
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

The A/B harness extracts the *exact* gold + strip + reorder command from Ninja, relinks the
current objects with one mutation at a time, strips and reorders exactly as the build does, and
compares every section against the original. It then runs the project verifier on the variant
`.so` to obtain the gate counts. The harness is reproduced in §6.

Baseline section table (`diff bytes` is the number of differing bytes when the sizes match):

| section | orig size | our size | orig addr | our addr | diff bytes |
|---|---:|---:|---:|---:|---:|
| `.note.android.ident` | 98 | 98 | 134 | 134 | 0 |
| `.note.gnu.build-id` | 24 | 24 | 1cc | 1cc | 20 |
| `.dynsym` | 15ca0 | 15ca0 | 1f0 | 1f0 | 0 |
| `.dynstr` | 2e318 | 2e318 | 15e90 | 15e90 | 0 |
| `.hash` | 973c | 973c | 441a8 | 441a8 | 0 |
| `.gnu.version` | 2b94 | 2b94 | 4d8e4 | 4d8e4 | 0 |
| `.gnu.version_d` | 1c | 1c | 50478 | 50478 | 0 |
| `.gnu.version_r` | 40 | 40 | 50494 | 50494 | 0 |
| `.rel.dyn` | 7048 | 7048 | 504d4 | 504d4 | 0 |
| `.rel.plt` | 72d0 | 72d0 | 5751c | 5751c | 0 |
| `.plt` | ac4c | ac4c | 5e7ec | 5e7ec | 0 |
| `.text` | 1337c8 | 1337c8 | 69440 | 69440 | 95999 |
| `.ARM.exidx` | 6f70 | 6f70 | 19cc08 | 19cc08 | 1683 |
| `.ARM.extab` | 26298 | 26294 | 1a3b78 | 1a3b78 | size! |
| `.rodata` | 41b67 | 41c27 | 1c9e10 | 1c9e10 | size! |
| `.fini_array` | 8 | 8 | 20cba0 | 20cba0 | 0 |
| `.data.rel.ro` | 2fe8 | 2fe8 | 20cbb0 | 20cbb0 | 0 |
| `.init_array` | c | c | 20fb98 | 20fb98 | 0 |
| `.dynamic` | 148 | 148 | 20fba4 | 20fba4 | 0 |
| `.got` | 430c | 430c | 20fcf4 | 20fcf4 | 0 |
| `.data` | 296c | 296c | 214000 | 214000 | 0 |
| `.bss` | 69ac | 69ac | 216970 | 216970 | 0 (NOBITS) |
| `.comment` | 12f | 12f | 0 | 0 | 0 |
| `.note.gnu.gold-version` | 1c | 1c | 0 | 0 | 0 |
| `.ARM.attributes` | 38 | 38 | 0 | 0 | 0 |
| `.shstrtab` | 117 | 117 | 0 | 0 | 0 |

The two size deltas are expected and documented:

* `.ARM.extab` is 4 B short (`0x26294` vs `0x26298`). This is the net of the five per-function
  LSDA size deltas identified in
  `_work/reconstructions/build-link-order-and-section-layout-w5018.md` §3.2; it is a source-body
  artifact, not a link-flag artifact.
* `.rodata` is `0xc0` B long (`0x41c27` vs `0x41b67`). The first `0x41b67` bytes are
  byte-identical to the original; the extra `0xc0` bytes are the decomp-only
  `kMissionCase[192]` table in `tools/mission_case_table.cpp`, linked last so it lands at
  `0x20b977` (the first byte past the original `.rodata` image). It shifts no original byte.
  The first `PT_LOAD` file/mem size consequently grows by `0xc0` (`0x20ba37` vs `0x20b977`);
  all other program headers are identical.

## 2. The exact gold 1.12 link command from ELF evidence

The original's observable ELF fixes the toolchain and every link flag that leaves a trace:

| observable in the original | value | flag/recipe it proves |
|---|---|---|
| `.note.gnu.gold-version` | `gold 1.12` | gold 1.12 linker |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2 ...` + `GCC: (GNU) 4.9.x 20150123` | NDK r18b clang 7.0.2; GCC 4.9.x runtime archives |
| `.note.android.ident` | API 16, `r18b`, build `5063045` | NDK r18b, `android-16` |
| ELF header flags | `0x5000200` (EABI5, soft-float) | armeabi-v7a softfp |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp` |
| `DT_NEEDED` order | `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc` | exact `-l`/FMOD/`-l m`/`-l dl -l c` order |
| `DT_SONAME` | `libgof2hdaa.so` | `-soname libgof2hdaa.so` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` (implies RELRO) |
| `PT_GNU_RELRO` | `0x20cba0..0x214000` (`.fini_array .data.rel.ro .init_array .dynamic .got`) | `-z now` / `-z relro`; `.got` before `.data` |
| `PT_GNU_STACK` | `RW` (flags 6) | `-z noexecstack` (default) |
| `.hash` present, no `.gnu.hash` | sysv hash only | `--hash-style=sysv` (default) |
| `.note.gnu.build-id` | 0x24-byte note, 20-byte SHA-1 descriptor | `--build-id` / `--build-id=sha1` |
| `.dynsym` | `0x15ca0` = 5578 entries (5370 exports + 207 imports + null) | `--exclude-libs=libgcc.a`; zlib/libzip whole-archive |
| `.rel.plt` / `.plt` | `0x72d0` / `0xac4c` | PLT slot order follows first reference; no ICF/GC |
| `.ARM.exidx` | `0x6f70` = 3566 entries (272 CANTUNWIND, 856 inline, 2438 extab) | `-ffunction-sections -funwind-tables`; no GC/ICF |
| `.text` | `0x1337c8` | plain object input order; no `--sort-section`, no ICF, no `--gc-sections` |
| `.rodata` | `0x41b67` | no `--sort-section`; no `--gc-sections` |
| 27 section headers / 8 program headers | — | plain gold shared link, no version script, no section-ordering file |

The exact link line (paths abbreviated; `$OBJS` is the 192 objects in
`tools/source_order.txt` order, and `$MISSION` is
`CMakeFiles/mission_table.dir/./tools/mission_case_table.cpp.obj`):

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold \
  -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  -o libgof2hdaa.unstripped.so \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o \
  -L $NDK/platforms/android-16/arch-arm/usr/lib \
  $OBJS \
  --whole-archive third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  $APK_LIBS/libfmodex.so $APK_LIBS/libfmodevent.so \
  third_party/openssl/libcrypto_gof2.a \
  -l m \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a \
  $MISSION \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
python3 tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so
```

`$MISSION` is a decomp-only input; it is placed after `libgcc.a` and before `-l dl -l c` so it
lands at `0x20b977` without moving any original `.rodata` byte. The original link did not have
this object.

The exact command line is obtainable with:

```sh
cd cmake-build-match
ninja -t commands libgof2hdaa.so | tail -1
```

Relinking that command verbatim reproduces the checked-in `.so`
(`sha256 47d00b82a5e928afb13846b1d732e168e85738d5434ad4ac89b8fc7b77e60ad1`).

## 3. Measured flag matrix

Notation: `size (N)` = section size in hex and N differing bytes; `size (!)` = size differs
from the original; `-` = section absent. `gate` is `byte_exact/linked_exact` from
`tools/verify/verify.py`. The original `.text` size is `0x1337c8`; `.ARM.exidx` is `0x6f70`;
`.ARM.extab` is `0x26298`; `.rodata` is `0x41b67`; `.dynsym` is `0x15ca0`; `.plt` is `0xac4c`;
`.got` is `0x430c`; `.rel.dyn` is `0x7048`; `.rel.plt` is `0x72d0`; `.dynamic` is `0x148`;
build-id note is `0x24` with 20 differing bytes.

| variant | .text | .ARM.exidx | .ARM.extab | .rodata | .dynsym | .plt | .got | .rel.dyn | .rel.plt | .dynamic | build-id | gate byte/linked |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| `icf_none` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `icf_safe` | 130964 (!) | 59a0 (!) | 26294 (!) | 41c27 (!) | 15ca0 (10681) | ac4c (3676) | 430c (0) | 7048 (4012) | 72d0 (7348) | 148 (4) | 24 (20) | 1347/4500 |
| `icf_all` | 12f99c (!) | 5760 (!) | 26294 (!) | 41c27 (!) | 15ca0 (10923) | ac4c (3676) | 430c (0) | 7048 (4086) | 72d0 (7348) | 148 (4) | 24 (20) | 1347/4499 |
| `gc_sections` | 133794 (!) | 6690 (!) | 26294 (!) | 41b57 (!) | 15ca0 (3211) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 2176/4500 |
| `icf_safe_gc` | 130950 (!) | 59a0 (!) | 26294 (!) | 41b57 (!) | 15ca0 (10702) | ac4c (3676) | 430c (0) | 7048 (4012) | 72d0 (7348) | 148 (4) | 24 (20) | 1347/4500 |
| `icf_all_gc` | 12f988 (!) | 5760 (!) | 26294 (!) | 41b57 (!) | 15ca0 (10948) | ac4c (3676) | 430c (0) | 7048 (4086) | 72d0 (7348) | 148 (4) | 24 (20) | 1347/4499 |
| `z_relro_only` | 1337c8 (109458) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (1328) | ac4c (11024) | 430c (0) | 7048 (7280) | 72d0 (7348) | 138 (!) | 24 (20) | 2932/4500 |
| `z_norelro_only` | 1337c8 (109199) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (7211) | ac4c (11024) | 430c (3674) | 7048 (8929) | 72d0 (7348) | 138 (!) | 24 (20) | 2932/4500 |
| `z_lazy_only` | 1337c8 (109458) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (1328) | ac4c (11024) | 430c (0) | 7048 (7280) | 72d0 (7348) | 138 (!) | 24 (20) | 2932/4500 |
| `z_now_then_relro` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `z_now_then_norelro` | 1337c8 (109862) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (7266) | ac4c (11025) | 430c (3674) | 7048 (10431) | 72d0 (10745) | 148 (18) | 24 (20) | 2932/4500 |
| `hash_sysv` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `hash_gnu` | 1337c8 (155793) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (41774) | ac4c (8129) | 430c (7348) | 7048 (12885) | 72d0 (10837) | 148 (18) | 24 (20) | 1750/4500 |
| `hash_both` | 1337c8 (149428) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (45558) | ac4c (10121) | 430c (11022) | 7048 (14416) | 72d0 (10837) | 150 (!) | 24 (20) | 1750/4500 |
| `sort_none` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `sort_name` | 13380c (!) | 68b0 (!) | 26294 (!) | 41c10 (!) | 15ca0 (14061) | ac4c (0) | 430c (0) | 7048 (4036) | 72d0 (0) | 148 (2) | 24 (20) | 1342/4498 |
| `no_exclude` | 1337d8 (!) | 6f70 (5457) | 26294 (!) | 41c27 (!) | 15cd0 (!) | ac64 (!) | 4314 (!) | 7048 (2614) | 72e0 (!) | 148 (26) | 24 (19) | 1720/4492 |
| `exclude_all` | 133628 (!) | 6f70 (8838) | 26294 (!) | 41c27 (!) | 14040 (!) | a628 (!) | 4100 (!) | 7048 (13573) | 6eb8 (!) | 148 (47) | 24 (20) | 1324/3432 |
| `exclude_cxxabi` | 133728 (!) | 6f70 (7480) | 26294 (!) | 41c27 (!) | 149f0 (!) | a9e8 (!) | 4240 (!) | 7048 (13595) | 7138 (!) | 148 (47) | 24 (20) | 1364/3576 |
| `exclude_two` | 13371c (!) | 6f70 (7654) | 26294 (!) | 41c27 (!) | 149c0 (!) | a9d0 (!) | 4238 (!) | 7048 (13609) | 7128 (!) | 148 (46) | 24 (20) | 1364/3582 |
| `exclude_atomic` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `buildid_sha1` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `buildid_tree` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `buildid_md5` | 1337c8 (142829) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (3675) | 430c (3674) | 7048 (0) | 72d0 (0) | 148 (8) | 20 (!) | 1969/4500 |
| `buildid_uuid` | 1337c8 (142829) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (3675) | 430c (3674) | 7048 (0) | 72d0 (0) | 148 (8) | 20 (!) | 1969/4500 |
| `buildid_none` | 1337c8 (149801) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (10669) | ac4c (4163) | 430c (3674) | 7048 (0) | 72d0 (0) | 148 (9) | - (-) | 1750/4500 |
| `buildid_0x` | 1337c8 (144130) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (3919) | 430c (3674) | 7048 (0) | 72d0 (0) | 148 (8) | 14 (!) | 1969/4500 |
| `no_soname` | 1337c8 (149793) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (11933) | ac4c (4285) | 430c (7348) | 7048 (2996) | 72d0 (0) | 140 (!) | 24 (20) | 1750/4500 |
| `soname_other` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (5648) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (10) | 24 (20) | 4500/4500 |
| `no_undefined` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `allow_shlib_undef` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `no_allow_shlib_undef` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `O1` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `O2` | 1337c8 (165276) | 6f70 (1683) | 26294 (!) | 41a27 (!) | 15ca0 (24132) | ac4c (7086) | 430c (3674) | 7048 (0) | 72d0 (0) | 148 (46) | 24 (20) | 1720/4500 |
| `as_needed` | 1337c8 (151120) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (5624) | ac4c (4407) | 430c (3674) | 7048 (3177) | 72d0 (0) | 128 (!) | 24 (20) | 1750/4500 |
| `no_as_needed` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `no_whole` | 12c208 (!) | 6af0 (!) | 25f40 (!) | 3dcd7 (!) | 15670 (!) | a9e8 (!) | 4228 (!) | 6f50 (!) | 7138 (!) | 148 (48) | 24 (19) | 1338/4416 |
| `whole_zlib_only` | 131db8 (!) | 6d10 (!) | 2615c (!) | 41be7 (!) | 15970 (!) | aaa8 (!) | 427c (!) | 7040 (!) | 71b8 (!) | 148 (45) | 24 (20) | 1354/4463 |
| `whole_libzip_only` | 12dc38 (!) | 6d50 (!) | 26078 (!) | 3dd27 (!) | 159e0 (!) | abb0 (!) | 42c4 (!) | 6f58 (!) | 7268 (!) | 148 (46) | 24 (20) | 1342/4453 |
| `whole_zlib_libzip_crypto` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `whole_all_static` | 139f80 (!) | 75b8 (!) | 26784 (!) | 42f78 (!) | 16430 (!) | adf0 (!) | 43d4 (!) | 72c8 (!) | 73e8 (!) | 148 (45) | 24 (19) | 1344/4500 |
| `whole_zlib_libzip_cxxabi` | 134208 (!) | 70b8 (!) | 2647c (!) | 41d5c (!) | 16130 (!) | ad6c (!) | 4394 (!) | 7298 (!) | 7390 (!) | 148 (44) | 24 (20) | 1344/4500 |
| `whole_around_objects` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `whole_around_shared` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `whole_after_libs` | 1337c8 (177650) | 6f70 (4121) | 26294 (!) | 41c21 (!) | 15ca0 (29280) | ac4c (0) | 430c (0) | 7048 (6293) | 72d0 (3592) | 148 (0) | 24 (20) | 4306/4500 |
| `no_text_reorder` | 1337c8 (1103591) | 6f70 (11559) | 26294 (!) | 41c27 (!) | 15ca0 (6362) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 1543/4500 |
| `text_reorder` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `rosegment` | 1337c8 (175512) | 6f70 (12381) | 26294 (!) | 41c27 (!) | 15ca0 (19126) | ac4c (14699) | 430c (11022) | 7048 (4471) | 72d0 (3674) | 148 (28) | 24 (20) | 1720/4500 |
| `merge_exidx` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `no_merge_exidx` | 1337c8 (113071) | 95c0 (!) | 26294 (!) | 41c27 (!) | 15ca0 (1290) | ac4c (3675) | 430c (0) | 7048 (6354) | 72d0 (3674) | 148 (4) | 24 (20) | 2878/4500 |
| `no_gnu_unique` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `gnu_unique` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `z_execstack` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `z_noexecstack` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `z_text` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `z_notext` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `allow_multiple` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `unresolved_ignore` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `fix_cortex` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `warn_textrel` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `no_copy_dt_needed` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `no_add_needed` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |
| `no_keep_memory` | 1337c8 (95999) | 6f70 (1683) | 26294 (!) | 41c27 (!) | 15ca0 (0) | ac4c (0) | 430c (0) | 7048 (0) | 72d0 (0) | 148 (0) | 24 (20) | 4500/4500 |

### 3.1 ICF and GC

* `--icf=none` is the default and is byte-identical to the base.
* `--icf=safe` folds ctors/dtors/address-not-taken functions: `.text` shrinks to `0x130964`
  (`-0x2e64`), `.ARM.exidx` to `0x59a0`, and `byte_exact` collapses to `1347` (`linked_exact`
  stays `4500` because the surviving functions still match structurally).
* `--icf=all` is stronger: `.text 0x12f99c`, `.ARM.exidx 0x5760`, `byte_exact 1347`,
  `linked_exact 4499` (one function is lost). `--icf-iterations=1/3` does not change the result.
* `--gc-sections` removes unused sections: `.text 0x133794` (`-0x34`), `.ARM.exidx 0x6690`,
  `.rodata 0x41b57`, `.bss 0x699c`; `byte_exact 2176`, `linked_exact 4500`.
* `--icf=safe --gc-sections` and `--icf=all --gc-sections` are combinations of the above.
* `--gc-keep-exported` is not supported by this gold 1.12 (`use --help`).

Conclusion: the original used neither ICF nor GC. The exact `.text`/`.ARM.exidx` sizes and the
full export set (`5370`) are only reproduced with both off.

### 3.2 RELRO / BIND_NOW

* Base `-z now`: `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `.dynamic 0x148`, `.text` diff
  95,999, gate `4500/4500`.
* `-z relro` alone (replace `now` with `relro`) and `-z lazy` are identical to each other:
  `.dynamic 0x138` (no `BIND_NOW`/`NOW`), `.text` diff 109,458, gate `2932/4500`.
* `-z norelro` alone: `.dynamic 0x138`, `.text` diff 109,199, `.got` diff 3,674,
  gate `2932/4500`.
* `-z now -z relro` (either order) is byte-identical to the base.
* `-z now -z norelro` (or `-z norelro -z now`): `.dynamic` diff 18, `.text` diff 109,862,
  gate `2932/4500`.
* `-z bind-now` is not accepted by this gold (use `-z now`).

Conclusion: `-z now` is proven; `-z relro` is redundant/unobservable when `-z now` is present.
`-z lazy`, `-z relro` alone and `-z norelro` are positively excluded.

### 3.3 Hash style and section sorting

* `--hash-style=sysv` is the default and byte-identical.
* `--hash-style=gnu` adds `.gnu.hash`, changes `.dynsym`/`.dynstr`/`.plt`/`.got`/`.rel.*`,
  `.text` diff 155,793, gate `1750/4500`.
* `--hash-style=both` is similar (`.dynamic` size `0x150`), gate `1750/4500`.
* `--sort-section=none` is the default and byte-identical.
* `--sort-section=name`: `.text 0x13380c`, `.ARM.exidx 0x68b0`, `.rodata 0x41c10`,
  `.data.rel.ro 0x2fe0`, `.bss 0x69b4`, gate `1342/4498`.
* `--sort-section=alignment` is not accepted by this gold (only `none`/`name`).

Conclusion: sysv hash and no section sorting are proven; gnu/both and name are excluded.

### 3.4 `--exclude-libs`

* `--exclude-libs=libgcc.a` is required: removing it exports 3 extra libgcc symbols
  (`5373` vs `5370`), changes `.dynsym`/`.dynstr`/`.hash`/`.plt`/`.got`/`.rel.*` and `.text`
  (`0x1337d8`), gate `1720/4492`.
* `--exclude-libs=ALL` hides too much: exports `4916`, 242 missing functions, gate `1324/3432`.
* `--exclude-libs=libc++abi.a`: exports `5071`, 98 missing, gate `1364/3576`.
* `--exclude-libs=libgcc.a,libc++abi.a`: exports `5068`, 98 missing, gate `1364/3582`.
* `--exclude-libs=libatomic.a` (and `libgcc.a,libatomic.a`) is byte-identical: no libatomic
  member is pulled.

Conclusion: `--exclude-libs=libgcc.a` is proven; `libatomic.a` is unobservable.

### 3.5 Build-id and SONAME

* `--build-id` and `--build-id=sha1` are byte-identical (20-byte SHA-1 descriptor).
* `--build-id=tree` is also byte-identical at this file size (gold's tree hash falls back to
  SHA-1 below its minimum file size), so it is unobservable.
* `--build-id=md5`/`uuid`: 16-byte descriptor, note size `0x20`, `.text` diff 142,829,
  gate `1969/4500`.
* `--build-id=none`: no note, `.text` diff 149,801, gate `1750/4500`.
* `--build-id=0x12345678`: 20-byte note but different descriptor (`0x14` note size), gate
  `1969/4500`.
* No `-soname`: `.dynstr`/`.dynamic` change, `.text` diff 149,793, gate `1750/4500`.
* `-soname libother.so`: only `.dynstr`/`.dynamic` differ; gate stays `4500/4500`, but
  `DT_SONAME` is wrong.

Conclusion: `--build-id` (SHA-1) and `-soname libgof2hdaa.so` are proven. md5/uuid/none/0x and
a different SONAME are excluded.

### 3.6 Undefined symbols, `-O1`, `--as-needed`

* `--no-undefined`, `--allow-shlib-undefined`, `--no-allow-shlib-undefined` are all
  byte-identical. The original may have had `--no-undefined` (the CMake default link line does),
  but it leaves no trace.
* `-O0` and `-O1` (`-Wl,-O1`) are byte-identical. `-O2`/`-O3` change `.text` (diff 165,276),
  `.rodata` (`0x41a27`), `.dynsym`, `.plt`, `.got`, `.data.rel.ro`, `.dynamic`; gate
  `1720/4500`.
* `--as-needed` drops `libGLESv1_CM`, `libEGL`, `libandroid`, `libdl` from `DT_NEEDED`
  (`.dynamic 0x128`), changes `.text` (diff 151,120), gate `1750/4500`.
* `--no-as-needed` is the default and byte-identical.

Conclusion: `-O1` and the undefined-symbol flags are unobservable; `-O2`/`-O3` and
`--as-needed` are excluded.

### 3.7 `--whole-archive` placement

* Current `--whole-archive zlib libzip --no-whole-archive` is required:
  * no whole-archive: `.text 0x12c208`, exports `5281`, imports `197`, 84 missing,
    gate `1338/4416`;
  * zlib only: `.text 0x131db8`, exports `5333`, imports `193`, 37 missing,
    gate `1354/4463`;
  * libzip only: `.text 0x12dc38`, exports `5318`, imports `215`, 47 missing,
    gate `1342/4453`.
* Adding `libcrypto_gof2.a` inside the same whole-archive block
  (`--whole-archive zlib libzip crypto --no-whole-archive`) is byte-identical: every crypto
  member is already pulled by the normal scan.
* Wrapping the app objects or the FMOD shared libraries in `--whole-archive` is byte-identical
  (they are not archives).
* Putting the whole-archive block after the shared/static libraries changes `.text` (diff
  177,650), `.dynsym`, `.rel.dyn`, `.rel.plt`, `.data.rel.ro`, gate `4306/4500`.
* `--whole-archive` around `libc++abi.a` or all static archives adds exports
  (`5440`/`5474`, extra 43/77), changes `.text`, gate `1344/4500`.

Conclusion: zlib and libzip must be whole-archived at their current position. Crypto/objects/
shared-lib placement is unobservable; cxxabi/all-static whole-archive is excluded.

### 3.8 Other gold knobs (not in the requested list, measured for completeness)

* `--text-reorder` is the default and byte-identical; `--no-text-reorder` changes the `.text`
  order (diff 1,103,591), gate `1543/4500`.
* `--merge-exidx-entries` is the default and byte-identical; `--no-merge-exidx-entries` grows
  `.ARM.exidx` to `0x95c0` (4792 entries), gate `2878/4500`.
* `--rosegment` changes section addresses/`.text` (diff 175,512), gate `1720/4500`.
  `--no-rosegment` is not accepted (no rosegment is the default).
* `--gnu-unique`/`--no-gnu-unique` are byte-identical.
* `-z execstack` changes only `PT_GNU_STACK` (RW → RWX); gate stays `4500/4500` but the
  program header is wrong. `-z noexecstack`, `-z text`, `-z notext` are byte-identical.
* `--allow-multiple-definition`, `--unresolved-symbols=ignore-all`, `--fix-cortex-a8`,
  `--warn-shared-textrel`, `--no-copy-dt-needed-entries`, `--no-add-needed`,
  `--no-keep-memory` are byte-identical.
* `--emit-relocs` fails inside gold 1.12 (`internal error in relocate_relocs`).

## 4. Classification

### Positively excluded (mutation changes the ELF away from the original)

`--icf=safe`, `--icf=all` (and with `--gc-sections`), `--gc-sections`, `-z relro` alone,
`-z lazy`, `-z norelro` (alone or with `-z now`), `--hash-style=gnu`, `--hash-style=both`,
`--sort-section=name`, no `--exclude-libs`, `--exclude-libs=ALL`,
`--exclude-libs=libc++abi.a`, `--exclude-libs=libgcc.a,libc++abi.a`, `--build-id=md5`,
`--build-id=uuid`, `--build-id=none`, `--build-id=0x...`, no `-soname`,
`-soname libother.so`, `--as-needed`, no `--whole-archive`, zlib-only whole-archive,
libzip-only whole-archive, whole-archive after the libraries, whole-archive around
`libc++abi.a`, whole-archive around all static archives, `-O2`, `-O3`, `--no-text-reorder`,
`--rosegment`, `--no-merge-exidx-entries`, `-z execstack`.

### Neutral / unobservable (byte-identical output)

`--icf=none`, `-z relro` with `-z now`, `--hash-style=sysv`, `--sort-section=none`,
`--exclude-libs=libatomic.a`, `--exclude-libs=libgcc.a,libatomic.a`, `--build-id=sha1`,
`--build-id=tree`, `--no-undefined`, `--allow-shlib-undefined`,
`--no-allow-shlib-undefined`, `-O0`, `-O1`/`-Wl,-O1`, `--no-as-needed`, crypto inside the
zlib+libzip whole-archive block, whole-archive around objects, whole-archive around shared
libraries, `--text-reorder`, `--merge-exidx-entries`, `--gnu-unique`, `--no-gnu-unique`,
`-z noexecstack`, `-z text`, `-z notext`, `--allow-multiple-definition`,
`--unresolved-symbols=ignore-all`, `--fix-cortex-a8`, `--warn-shared-textrel`,
`--no-copy-dt-needed-entries`, `--no-add-needed`, `--no-keep-memory`.

### Link failures / unsupported in gold 1.12

`--gc-keep-exported`, `-z bind-now`, `--sort-section=alignment`, `--no-rosegment`,
`--emit-relocs`.

### Gate improvement

**None.** The best measured variant is the current recipe (`4500/4500`). Every non-base variant
either keeps `linked_exact 4500` but reduces `byte_exact`, or changes only non-code bytes while
staying `4500/4500` (and is excluded by ELF evidence). `--icf=all` is the only variant that
reduces `linked_exact` (`4499`).

## 5. Proven vs hypothesis

### Proven

* The original was linked with gold 1.12 from NDK r18b clang 7.0.2, armeabi-v7a softfp,
  API 16.
* `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`.
* sysv hash, no `.gnu.hash`; no section sorting; no ICF; no GC; no `--rosegment`.
* zlib and libzip whole-archived at their current position; crypto normal.
* FMOD before libc++abi; the full `DT_NEEDED` order.
* The remaining `.text`/`.ARM.exidx`/`.ARM.extab`/`.rodata`/build-id deltas are not link-flag
  deltas.

### Unobservable / hypothesis

* Whether the original explicitly passed `--no-undefined`, `-O1`, `-z relro`,
  `--exclude-libs=libatomic.a`, `--text-reorder`, `--merge-exidx-entries`, or
  `--build-id=tree`; each is byte-identical to the current recipe.
* Whether the original link line had `-s`; the shipped APK is GNU-stripped and the build-id is
  hashed before the strip, so `-s` is unobservable except through the build-id.
* The exact original link command text, object order beyond the recovered TU order, and the
  original unstripped build-id preimage.
* The exact host OS/NDK package (darwin vs linux) is covered by
  `_work/reconstructions/build_provenance_w3901.md`; this note only re-confirms that the
  *link flags* are the CMake/NDK r18b defaults.

## 6. Reproduction

The harness used for every row:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5918
mkdir -p /dev/shm/w5918
# 1. extract the exact gold/strip/reorder command
cd cmake-build-match
ninja -t commands libgof2hdaa.so | tail -1 > /dev/shm/w5918/link_cmd.txt
# 2. relink the base and every mutation, strip + reorder, compare sections
python3 /dev/shm/w5918/link_matrix.py
python3 /dev/shm/w5918/link_matrix2.py
python3 /dev/shm/w5918/link_matrix3.py
# 3. verify each variant (parallel)
/dev/shm/w5918/run_verify_all.sh
/dev/shm/w5918/run_verify2.sh
/dev/shm/w5918/run_verify3.sh
```

The base relink check is:

```sh
sha256sum cmake-build-match/libgof2hdaa.so /dev/shm/w5918/variants/base.so
# 47d00b82a5e928afb13846b1d732e168e85738d5434ad4ac89b8fc7b77e60ad1  both
```

The full CSV of all 80+ variants (section sizes, diff bytes, gate counts) is reproduced by the
scripts; the curated table in §3 is the subset that answers the requested flag list. The scripts
are scratch-only (`/dev/shm/w5918`); the note is the committed deliverable.

## 7. Files

* `_work/reconstructions/gold-link-flags-wave111-w5918.md` — this note.
* `_work/reconstructions/build-link-order-and-section-layout-w5018.md` — link-order and
  section-delta archaeology (wave 100).
* `_work/reconstructions/build_provenance_w3901.md` — toolchain/host provenance (wave 89).
