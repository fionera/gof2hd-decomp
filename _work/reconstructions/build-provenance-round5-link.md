# Build provenance round 5 (w11020): link recipe, strip version, and the
# ground-truth `.shstrtab` normalization

Status: **research note only**, 2026-11-08. Worktree `wt/w11020`, base
master `b3d3a1f68` (wave 163). Scratch `/dev/shm/w11020`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). No `src/`, `tools/`,
`CMakeLists.txt`, baseline, allowlist or doc change is landed; this note is
the only commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` **84,652** differing bytes (size
1,259,464 B), `.ARM.extab` **725** (size 156,312 B), `.rel.plt` **0**
(byte-identical), `.note.gnu.build-id` **20**. All 27 section sizes exact;
file size 2,187,328 B.

## 0. Bottom line

1. **The original is GNU gold 1.12 from NDK r18b, then GNU bfd strip
   2.27.0.20170315 (also r18b).** The gold-version note is byte-identical
   (`"gold 1.12"`); the r13b gold 1.11 control emits `"gold 1.11"` and a
   different layout. The strip is pinned to binutils 2.27, not 2.25: the
   original's `.rel.plt` `sh_info` is **20** (the `.got` section index) and
   `.init_array`/`.fini_array` `sh_entsize` is **4**; r13b's strip 2.25
   gives `sh_info` **11** and `sh_entsize` **0**.
2. **Every linker-controlled byte is already exact.** `.dynsym`,
   `.dynstr`, `.hash`, `.gnu.version{,_d,_r}`, `.rel.dyn`, `.rel.plt`,
   `.plt`, `.ARM.exidx`, `.rodata`, `.data.rel.ro`, `.init_array`,
   `.fini_array`, `.dynamic`, `.got`, `.data`, `.bss`, `.comment`,
   `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`,
   `.shstrtab`, the ELF header, all 8 program headers and all 27 section
   headers are **0 differing bytes**. The only residual is `.text`
   (84,652), `.ARM.extab` (725) and the build-id descriptor (20).
3. **The shipped APK `.so` is exactly our gold link + r18b `strip
   --strip-all`** in every metadata section and header. The repo's
   `tools/reorder_shstrtab.py` is a **ground-truth normalization**, not an
   original post-link step: the APK copy and the ground-truth file differ
   by exactly 288 bytes (259 in `.shstrtab` + 29 `sh_name` fields), and
   `reorder_shstrtab.py` on the APK copy with our gold output reproduces
   the ground-truth SHA-256 exactly.
4. **No post-link step other than bfd `strip --strip-all` is visible.**
   `strip --strip-all`, `objcopy --strip-all` and `strip --strip-unneeded`
   are byte-identical on this file. There is no `.gnu_debuglink`, no
   `.symtab`/`.strtab`/`.debug_*`, no patchelf-style `.dynstr` rewrite, and
   `.comment`/`.note.gnu.gold-version`/`.ARM.attributes` are retained.
5. **The build-id is gold's SHA-1 of the *unstripped* link output with the
   20-byte descriptor zeroed.** It is not the SHA-1 of either shipped
   variant (ground truth `d7c05dc5…`, APK `ca7d7940…`, neither equals
   `c011810d…`). Verified on our r18b and r13b gold outputs.
6. **APK metadata:** four `lib/armeabi-v7a/*.so`; `libgof2hdaa.so` is
   deflated (method 8), no extra field, CRC `a7ae3617`, local-header offset
   3,578,408 (mod 4096 = 2600), DOS timestamp `1980-00-00 00:00:00`. The
   APK has 143 entries, no zip64/comment, a v2 signature block (no v3), and
   was built by Android Gradle 3.1.2 (version 2.0.16, 2018-10-22). The
   `.so` is compressed, so no zip alignment/compression trick affects its
   bytes.

## 1. Baseline and section-by-section comparison

`raw` = same-address differing bytes; `hdr` = section-header fields
(type/flags/addr/off/size/link/info/align/entsize). Comparison is
ground truth vs `cmake-build-match/libgof2hdaa.so`.

| section | size | diff | hdr |
|---|---:|---:|---|
| (null) | 0 | 0 | OK |
| `.note.android.ident` | 152 | **0** | OK |
| `.note.gnu.build-id` | 36 | **20** | OK |
| `.dynsym` | 89,248 | **0** | OK |
| `.dynstr` | 189,208 | **0** | OK |
| `.hash` | 38,716 | **0** | OK |
| `.gnu.version` | 11,156 | **0** | OK |
| `.gnu.version_d` | 28 | **0** | OK |
| `.gnu.version_r` | 64 | **0** | OK |
| `.rel.dyn` | 28,744 | **0** | OK |
| `.rel.plt` | 29,392 | **0** | OK |
| `.plt` | 44,108 | **0** | OK |
| `.text` | 1,259,464 | **84,652** | OK |
| `.ARM.exidx` | 28,528 | **0** | OK |
| `.ARM.extab` | 156,312 | **725** | OK |
| `.rodata` | 269,159 | **0** | OK |
| `.fini_array` | 8 | **0** | OK |
| `.data.rel.ro` | 12,264 | **0** | OK |
| `.init_array` | 12 | **0** | OK |
| `.dynamic` | 328 | **0** | OK |
| `.got` | 17,164 | **0** | OK |
| `.data` | 10,604 | **0** | OK |
| `.bss` | 27,052 | **0** | OK |
| `.comment` | 303 | **0** | OK |
| `.note.gnu.gold-version` | 28 | **0** | OK |
| `.ARM.attributes` | 56 | **0** | OK |
| `.shstrtab` | 279 | **0** | OK |

Raw header tables:

* ELF header `0x00..0x34`: **0** differing bytes.
* Program headers (8 × 32 B at `0x34`): **0** differing bytes.
* Section headers (27 × 40 B at `0x215c08`): **0** differing bytes.

Relocation and symbol counts (identical in both):

| item | count |
|---|---:|
| `.rel.dyn` `R_ARM_ABS32` | 1,817 |
| `.rel.dyn` `R_ARM_GLOB_DAT` | 614 |
| `.rel.dyn` `R_ARM_RELATIVE` | 1,162 |
| `.rel.plt` `R_ARM_JUMP_SLOT` | 3,674 |
| `.dynsym` entries | 5,580 |
| `.ARM.exidx` entries | 3,566 (856 with an extab record) |
| undefined imports | 207 |
| defined exports | 5,370 |

The 17 non-byte-exact bodies are the same wave-163 set:
`Layout::Layout` C1/C2, `LevelScript::process`, `Level::createShip`,
`Status::missionCompleted`, `Radar::draw`, `MGame::OnTouchEnd`,
`Level::createMission`, `Gun::calcCharacterCollision`,
`HangarWindow::render`, `SpaceLounge::drawLounge`, `Hud::draw`,
`Generator::createMission`, `MenuTouchWindow::OnTouchBegin`,
`ModStation::ModStation` C1/C2, `MGame::OnUpdate`.

### 1.1 `.ARM.extab` decomposition (current state)

Parsing `.ARM.exidx` (prel31 second word, bit31 = 0 and word != 1 means an
extab pointer; the base is the address of the second word) maps the 725
differing `.ARM.extab` bytes to 11 records:

| owner | extab diff bytes |
|---|---:|
| `LevelScript::process` | 185 |
| `Level::createMission` | 159 |
| `HangarWindow::render` | 151 |
| `Level::createShip` | 77 |
| `Radar::draw` | 50 |
| `Hud::draw` | 37 |
| `MGame::OnTouchEnd` | 30 |
| `SpaceLounge::drawLounge` | 14 |
| `Generator::createMission` | 13 |
| `Radar::Radar(Level*)` | 7 |
| `ModStation::ModStation` (C1) | 2 |
| **total** | **725** |

`Radar::Radar(Level*)` is the only byte-exact body owning an extab delta
(7 bytes); the other 718 bytes are downstream of non-exact bodies.

## 2. The ground-truth `.shstrtab` normalization

The APK copy and the ground-truth file are the same 2,187,328-byte ELF with
the same 27 sections, same section order, same offsets and same contents
except:

* `.shstrtab`: 259 differing bytes;
* the 26 `sh_name` fields: 29 differing bytes;
* total **288** differing bytes.

The APK `.shstrtab` is in **section-header order** (the bfd strip's
rewrite):

```
.shstrtab .note.android.ident .note.gnu.build-id .dynsym .dynstr .hash
.gnu.version .gnu.version_d .gnu.version_r .rel.dyn .rel.plt .text
.ARM.exidx .ARM.extab .rodata .fini_array .data.rel.ro .init_array
.dynamic .got .data .bss .comment .note.gnu.gold-version .ARM.attributes
```

The ground truth `.shstrtab` is in **gold Stringpool order**:

```
.init_array .fini_array .ARM.exidx .text .got .comment .note.android.ident
.rel.plt .bss .ARM.attributes .dynstr .gnu.version_r .data.rel.ro .rel.dyn
.gnu.version .note.gnu.gold-version .dynsym .hash .note.gnu.build-id
.gnu.version_d .dynamic .ARM.extab .shstrtab .rodata .data
```

Proof that the ground truth is the APK copy with the repo normalization
applied:

```sh
cp apk/lib/armeabi-v7a/libgof2hdaa.so reordered.so
python3 tools/reorder_shstrtab.py ours.unstripped.so reordered.so
sha256sum reordered.so /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
# both d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483
```

The reverse is also exact: our gold link + r18b strip **without**
`reorder_shstrtab.py` is byte-identical to the APK copy in every metadata
section and section header (only `.text`, `.ARM.extab` and the build-id
descriptor differ). So:

* the original build's post-link output is the APK copy (bfd `.shstrtab`
  order);
* the ground-truth file used by the gate is the APK copy plus a repo-side
  `.shstrtab` rewrite;
* the canonical build's `reorder_shstrtab.py` step exists only to match
  that rewritten ground truth.

If the project ever wants to match the pristine APK artifact instead of the
ground truth, dropping `reorder_shstrtab.py` is the only change needed
(288 bytes: 259 `.shstrtab` + 29 `sh_name`).

## 3. Linker identity: gold 1.12 from r18b

The original's `.note.gnu.gold-version` is 28 bytes:

```
04000000 09000000 04000000 474e5500 676f6c64 20312e31 32000000
```

i.e. `"GNU"` / `"gold 1.12"`. Our canonical build is byte-identical. The
local r18b linker reports:

```
GNU gold (GNU Binutils 2.27.0.20170315) 1.12
```

The local r13b linker reports:

```
GNU gold (binutils-2.25-0666073 2.25.51.20141117) 1.11
```

Control: linking the same objects with r13b gold 1.11 (and r18b strip)
changes the note to `"gold 1.11"` and the whole layout:

| observable | original / r18b gold 1.12 | r13b gold 1.11 |
|---|---:|---:|
| file size | 2,187,328 | 2,188,456 (+1,128) |
| `.text` diff | 84,652 | 98,718 |
| `.rodata` diff | 0 | 406 |
| `.data.rel.ro` diff | 0 | 1,603 |
| `.got` diff | 0 | 3,674 |
| `.rel.dyn` diff | 0 | 10,431 |
| `.rel.plt` diff | 0 | 10,745 |
| `.plt` diff | 0 | 11,025 |
| `.text` address | `0x69440` | `0x69420` |
| RW section order | `.fini_array .data.rel.ro .init_array .dynamic .got .data .bss` | `.dynamic .got .data .fini_array .data.rel.ro .init_array .bss` |

So the original is gold **1.12**, not 1.11. The NDK r18b package pins the
exact 1.12 build: its `source.properties` is
`Pkg.Revision = 18.1.5063045`, its clang is
`Android (4751641 based on r328903) clang version 7.0.2`, its
`platforms/android-16/arch-arm/usr/lib/crtbegin_so.o` carries the
byte-identical `.note.android.ident` (`API 16`, `r18b`, `5063045`), and its
gold is the only gold 1.12 in the r18b package. A different gold 1.12 build
that is behaviorally identical on this input cannot be distinguished from
the output alone; there is no positive evidence for one.

## 4. Post-link step: bfd strip 2.27 (r18b), not 2.25 (r13b)

The shipped APK `.so` is exactly reproduced (metadata-wise) by our gold link
followed by the r18b GNU strip:

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
```

Section-header evidence (original / APK copy vs the two local strips):

| header field | original / APK | r18b strip 2.27 | r13b strip 2.25 |
|---|---|---|---|
| `.rel.plt` flags | `AI` | `AI` | `AI` |
| `.rel.plt` `sh_info` | **20** (`.got` index) | **20** (`.got` index) | **11** (`.plt` index) |
| `.init_array` `sh_entsize` | **4** | **4** | **0** |
| `.fini_array` `sh_entsize` | **4** | **4** | **0** |
| `.bss` `sh_offset` | `0x21596c` | `0x21596c` | `0x21596c` |
| `.shstrtab` order | section-header | section-header | section-header |
| `.comment` retained | yes | yes | yes |
| `.note.gnu.gold-version` retained | yes | yes | yes |
| `.ARM.attributes` retained | yes | yes | yes |
| `.symtab`/`.strtab`/`.debug_*` | absent | absent | absent |

The `.rel.plt` `sh_info` is the sharpest discriminator. The r18b strip sets
it to the `.got` section index (20 in the r18b gold layout, 17 in the r13b
gold layout); the r13b strip sets it to 11 (the `.plt` section index). The
four combinations are:

| gold | strip | `.rel.plt` `sh_info` | init/fini entsize |
|---|---|---:|---:|
| r18b 1.12 | r18b 2.27 | **20** | **4** |
| r13b 1.11 | r18b 2.27 | 17 | 4 |
| r18b 1.12 | r13b 2.25 | 11 | 0 |
| r13b 1.11 | r13b 2.25 | 11 | 0 |

Only **r18b gold + r18b strip** gives the original's `(20, 4)`. The strip
is therefore binutils 2.27.0.20170315 (r18b), not 2.25 (r13b).

Tool-spelling controls:

* `strip --strip-all`, `objcopy --strip-all` and `strip --strip-unneeded`
  all produce the **same** 2,187,328-byte file on this input
  (`sha256 e9c7de5c…`). The exact spelling is unobservable; the bfd
  behavior is what matters.
* `strip --strip-all -R .comment` removes `.comment` (2,186,976 B) and is
  wrong.
* `llvm-strip`/`llvm-objcopy --strip-all` drop `.comment`,
  `.note.gnu.gold-version` and `.ARM.attributes` and leave `.rel.plt`
  `SHF_INFO_LINK`/`sh_info` and `.init_array`/`.fini_array` `sh_entsize`
  unnormalized (wave 8622); wrong.
* No `.gnu_debuglink`, no `.debug_*`, no `.symtab`/`.strtab`, no
  patchelf-style `.dynstr` rewrite, no extra section. There is no evidence
  of any post-link step other than the bfd strip.

### 4.1 Correction to the wave-8622 `.rel.plt` parenthetical

`_work/reconstructions/linker_provenance_w8622.md` says `-z lazy`/`-z relro`
"leave `.rel.plt` `sh_info` at 0x14 instead of 0x15". The measured values
are the opposite: the original (and `-z now`) is **0x14**, while
`-z lazy`/`-z relro` give **0x15**. The rest of that note's `-z now`
evidence (`.got` before `.data`, `DT_FLAGS=BIND_NOW`,
`DT_FLAGS_1=NOW`) is correct.

## 5. Exact gold invocation

The canonical link line (from `ninja -t commands libgof2hdaa.so`) is:

```sh
ld.gold -shared -soname libgof2hdaa.so --build-id -z now \
  --exclude-libs=libgcc.a -o libgof2hdaa.unstripped.so \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o \
  -L $NDK/platforms/android-16/arch-arm/usr/lib \
  <objects in tools/source_order.txt> \
  --whole-archive third_party/zlib/libz_gof2.a \
                  third_party/libzip/libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  _work/bins/apk_libs/libfmodex.so _work/bins/apk_libs/libfmodevent.so \
  third_party/openssl/libcrypto_gof2.a -l m \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

Every clause is pinned by the output:

| clause | evidence |
|---|---|
| `-shared` | ELF `DYN` |
| `-soname libgof2hdaa.so` | `DT_SONAME` |
| `--build-id` (SHA-1) | 36-byte `.note.gnu.build-id`, 20-byte descriptor |
| `-z now` | `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `.got` before `.data`, `.rel.plt` `sh_info=0x14` |
| `--exclude-libs=libgcc.a` | no exported `__aeabi_ldivmod`/`__aeabi_ldiv0`/`__aeabi_idiv0`; libc++abi symbols still exported |
| no `-s` | build-id is the SHA-1 of the unstripped output, not the shipped file |
| object order | all 4,524 function addresses match; `--no-text-reorder` changes `.text` by >1 MB |
| default `--merge-exidx-entries` | `.ARM.exidx` is 28,528 B; disabling grows it |
| default sysv hash | `.hash` only, no `.gnu.hash` |
| default 4 KiB pages | `PT_LOAD` align `0x1000`; `-z max-page-size=0x1000` neutral |
| no `--rosegment` | exactly 2 `PT_LOAD`s |
| no ICF / no `--gc-sections` | `--icf` shrinks `.text`; `--gc-sections` drops the 193 empty-`.text` sentinels |
| no `--version-script` | all 5,370 symbols exported |
| no `--allow-multiple-definition` | no duplicate strong definitions |
| no `--unresolved-symbols` | 207 undefined imports (shared-library default) |

Controls measured in this pass:

* `--build-id=sha1` is byte-identical to the default; `--build-id=md5`
  emits a 32-byte note (16-byte descriptor) and shifts `.rel.plt` by -4;
  `--build-id=none` removes the note and shifts the image by -0x24.
* `-z lazy` / `-z relro`: `.got` moves after `.data`, `DT_FLAGS` disappears,
  `.rel.plt` `sh_info` becomes 0x15, file size +780. Wrong.
* `--exclude-libs=ALL`: hides libc++abi symbols too
  (`__gxx_personality_v0` disappears), `.dynsym` 0x14040 vs 0x15ca0,
  `.dynstr` 0x2c61e vs 0x2e318, `.text` 0x133628 vs 0x1337c8. Wrong.
* no `--exclude-libs`: exports `__aeabi_ldivmod` (size 0) plus weak
  `__aeabi_ldiv0`/`__aeabi_idiv0`, `.dynsym` 0x15cd0, `.text` 0x1337d8.
  Wrong.
* r13b gold 1.11: see §3. Wrong.

`DT_NEEDED` order (identical): `liblog`, `libGLESv2`, `libGLESv1_CM`,
`libEGL`, `libandroid`, `libfmodex`, `libfmodevent`, `libm`, `libdl`,
`libc`. `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `DT_RELCOUNT=1162`,
`DT_PLTGOT=0x21068c`, `DT_JMPREL=0x5751c`, `DT_PLTRELSZ=29392`.

## 6. Build-id

The note is `namesz=4, descsz=20, type=3, name="GNU\0"`, descriptor:

```
original: c011810da45777b83ed721b2a87d517efed0d310
ours:     637ba398189449ed97045edc72dfea48412f62d6
```

The 16-byte note header is identical; only the 20-byte descriptor differs.
Gold's algorithm, verified end-to-end on our unstripped output:

```python
d = bytearray(open('libgof2hdaa.unstripped.so','rb').read())
# find the 20-byte descriptor in .note.gnu.build-id, zero it, SHA-1 the file
d[desc_off:desc_off+20] = b'\0'*20
assert hashlib.sha1(d).hexdigest() == build_id
```

This holds for both the r18b gold 1.12 and r13b gold 1.11 outputs. The
shipped file's own SHA-1 (with the descriptor zeroed) does **not** match:
ground truth `d7c05dc5…`, APK copy `ca7d7940…`, ours `3bf3ccad…`. So the
build-id was computed over the original **unstripped** link output, before
the strip. It is all-or-nothing: it cannot match until `.text`,
`.ARM.extab`, the debug info and the original build paths all match.

## 7. APK `lib/armeabi-v7a/` metadata

APK: `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`
(6,389,308 B, sha256 `9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19`).

| library | size | compiler / linker | build-id |
|---|---:|---|---|
| `libgof2hdaa.so` | 2,187,328 | clang 7.0.2 r18b / gold 1.12 | `c011810da45777b83ed721b2a87d517efed0d310` |
| `libcrashlytics.so` | 202,580 | GCC 4.9.x / gold 1.11 | `e3c978ad09d1fab2d6c86de96df95c6442868a1a` |
| `libfmodex.so` | 920,204 | GCC 4.4.3/4.6, ARMv5TE | none |
| `libfmodevent.so` | 370,660 | GCC 4.4.3, ARMv5TE | none |

`libgof2hdaa.so` zip entry:

| field | value |
|---|---|
| uncompressed size | 2,187,328 |
| CRC-32 | `a7ae3617` (matches the extracted file) |
| compression | deflated (method 8), normal |
| extra field | 0 bytes |
| local-header offset | 3,578,408 (`mod 4096 = 2600`) |
| DOS timestamp | `1980-00-00 00:00:00` |
| encoding software | 2.4, MS-DOS/FAT origin |
| encrypted | no |
| extended local header | no |

The `.so` is compressed, so the zip's alignment/compression does not touch
its bytes. The APK has 143 entries, no zip64, no archive comment, and a v2
signature block (`APK Sig Block 42`, one v2 signer + verity padding; no v3).
`META-INF/MANIFEST.MF` says `Created-By: Android Gradle 3.1.2`;
`assets/crashlytics-build.properties` says `version_name=2.0.16`,
`build_id=f527605b-309f-404d-8afc-b68027edc49e`, built
`Mon Oct 22 10:18:17 CEST 2018`; the signing cert subject is
`O=FISHLABS Entertainment GmbH`.

The ground-truth `.so` is the APK copy with the 288-byte `.shstrtab`
normalization described in §2; the APK copy is the pristine shipped
artifact.

## 8. Conclusions / open questions

* The link recipe is exact: gold 1.12 (r18b) + GNU bfd strip 2.27 (r18b).
  All linker-controlled bytes are already identical.
* The only residual is source shape: `.text` (84,652 bytes in 17 bodies),
  `.ARM.extab` (725 bytes in 11 records), and the build-id descriptor
  (20 bytes, downstream of the first two).
* The ground truth is a normalized artifact, not the pristine APK `.so`.
  The repo's `reorder_shstrtab.py` step is the only difference.
* The exact gold build (r18b vs another behaviorally identical gold 1.12)
  is not distinguishable from the output alone; the r18b package is pinned
  by the byte-identical `.note.android.ident`, clang `.comment`, and strip
  header normalization, so r18b is the supported conclusion.
* No post-link step other than bfd `strip --strip-all` is visible. The
  original build-id is the SHA-1 of the unstripped output, so it remains
  the last checksum after every other byte matches.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11020
TMPDIR=/dev/shm/w11020/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w11020/tmp cmake --build cmake-build-match --target check -j8

# section-by-section comparison (the script is reproduced in the scratch:
# /dev/shm/w11020/seccmp.py)
python3 /dev/shm/w11020/seccmp.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so

# extract the APK copy
mkdir -p /dev/shm/w11020/apk
cd /dev/shm/w11020/apk
unzip -o -q "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" 'lib/armeabi-v7a/*'

# ground-truth normalization proof
cp apk/lib/armeabi-v7a/libgof2hdaa.so /dev/shm/w11020/reordered.so
python3 tools/reorder_shstrtab.py /dev/shm/w11020/ours.unstripped.so \
  /dev/shm/w11020/reordered.so
sha256sum /dev/shm/w11020/reordered.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# strip-version controls
R18=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip
R13=.cache/ndk/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip
$R18 --strip-all -o /dev/shm/w11020/strip_r18b.so /dev/shm/w11020/ours.unstripped.so
$R13 --strip-all -o /dev/shm/w11020/strip_r13b.so /dev/shm/w11020/ours.unstripped.so
readelf -SW /dev/shm/w11020/strip_r18b.so | grep -E '\.rel\.plt|\.init_array|\.fini_array'
readelf -SW /dev/shm/w11020/strip_r13b.so | grep -E '\.rel\.plt|\.init_array|\.fini_array'

# build-id algorithm (inline)
python3 - <<'PY'
import hashlib
d = bytearray(open('/dev/shm/w11020/ours.unstripped.so','rb').read())
j = d.find(b'\x04\x00\x00\x00\x14\x00\x00\x00\x03\x00\x00\x00GNU\x00')
bid = bytes(d[j+16:j+36])
d[j+16:j+36] = b'\0'*20
print(bid.hex(), hashlib.sha1(d).hexdigest(), bid.hex() == hashlib.sha1(d).hexdigest())
PY
```

Scratch: `/dev/shm/w11020/` — `seccmp.py` (section comparison),
`extab2.py` (extab owner mapping), `runlink.py` (link variants),
`linkcmd.txt` (canonical link command), `ours.unstripped.so`,
`strip_r18b.so`, `strip_r13b.so`, `gold_r13b.so`, `excl_all.so`,
`excl_none.so`, `zlazy.so`, `zrelro.so`, `bidnone.so`, `bidmd5.so`.

## 10. Files changed

* `_work/reconstructions/build-provenance-round5-link.md` — this note.
* No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change.

## Appendix A. `seccmp.py` (section-by-section comparison)

```python
import struct, sys, hashlib

def sections(path):
    d = open(path, 'rb').read()
    shoff = struct.unpack_from('<I', d, 0x20)[0]
    shentsize = struct.unpack_from('<H', d, 0x2e)[0]
    shnum = struct.unpack_from('<H', d, 0x30)[0]
    shstrndx = struct.unpack_from('<H', d, 0x32)[0]
    shstr = shoff + shstrndx * shentsize
    stroff = struct.unpack_from('<I', d, shstr + 16)[0]
    strsize = struct.unpack_from('<I', d, shstr + 20)[0]
    tab = d[stroff:stroff + strsize]
    out = []
    for i in range(shnum):
        b = shoff + i * shentsize
        no = struct.unpack_from('<I', d, b)[0]
        name = tab[no:tab.index(b'\0', no)].decode()
        typ, flags, addr, off, size, link, info, align, entsize = \
            struct.unpack_from('<9I', d, b + 4)
        out.append(dict(i=i, name=name, type=typ, flags=flags, addr=addr,
                        off=off, size=size, link=link, info=info,
                        align=align, entsize=entsize))
    return d, out, shoff, shentsize, shnum

def cmp(a, b, label):
    da, sa, shoffa, sea, na = sections(a)
    db, sb, shoffb, seb, nb = sections(b)
    print('===', label)
    print('file sizes', len(da), len(db),
          'sha', hashlib.sha256(da).hexdigest()[:16],
          hashlib.sha256(db).hexdigest()[:16])
    print('sections', na, nb, 'shoff', hex(shoffa), hex(shoffb))
    print('section order equal:',
          [s['name'] for s in sa] == [s['name'] for s in sb])
    mb = {s['name']: s for s in sb}
    for s in sa:
        t = mb.get(s['name'])
        if t is None:
            print('  MISSING in B:', s['name']); continue
        if s['type'] == 8:  # SHT_NOBITS: no file bytes
            diff = 0
        else:
            x = da[s['off']:s['off'] + s['size']]
            y = db[t['off']:t['off'] + t['size']]
            diff = sum(1 for p, q in zip(x, y) if p != q) + abs(len(x) - len(y))
        hdr = all(s[k] == t[k] for k in
                  ('type', 'flags', 'addr', 'size', 'link', 'info', 'align', 'entsize'))
        print('  %-24s size %8d/%8d diff %7d hdr %s' %
              (s['name'] or '<null>', s['size'], t['size'], diff,
               'OK' if hdr else 'DIFF'))
    raw_a = da[shoffa:shoffa + sea * na]
    raw_b = db[shoffb:shoffb + seb * nb]
    print('raw section-header table diff bytes:',
          sum(1 for p, q in zip(raw_a, raw_b) if p != q),
          'len', len(raw_a), len(raw_b))

if __name__ == '__main__':
    cmp(sys.argv[1], sys.argv[2], sys.argv[3] if len(sys.argv) > 3 else '')
```

## Appendix B. `extab2.py` (`.ARM.extab` owner mapping)

```python
import struct, bisect
from collections import Counter

A = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
B = '/home/debian/GalaxyOnFire2/wt/w11020/cmake-build-match/libgof2hdaa.so'

def secs(path):
    d = open(path, 'rb').read()
    shoff = struct.unpack_from('<I', d, 0x20)[0]
    se = struct.unpack_from('<H', d, 0x2e)[0]
    n = struct.unpack_from('<H', d, 0x30)[0]
    si = struct.unpack_from('<H', d, 0x32)[0]
    b = shoff + si * se
    so = struct.unpack_from('<I', d, b + 16)[0]
    ss = struct.unpack_from('<I', d, b + 20)[0]
    tab = d[so:so + ss]
    out = {}
    for i in range(n):
        h = shoff + i * se
        no = struct.unpack_from('<I', d, h)[0]
        name = tab[no:tab.index(b'\0', no)].decode()
        typ, flags, addr, off, size, link, info, align, entsize = \
            struct.unpack_from('<9I', d, h + 4)
        out[name] = (addr, off, size, link, info, entsize)
    return d, out

def dynnames(path):
    d, s = secs(path)
    doff = s['.dynsym'][1]; dsize = s['.dynsym'][2]
    soff = s['.dynstr'][1]; ssize = s['.dynstr'][2]
    st = d[soff:soff + ssize]
    names = {}
    for i in range(0, dsize, 16):
        nameoff, val, size, info, other, shndx = \
            struct.unpack_from('<IIIBBH', d, doff + i)
        name = st[nameoff:st.index(b'\0', nameoff)].decode()
        if val:
            names.setdefault(val & ~1, name)
    return names

da, sa = secs(A)
db, sb = secs(B)
ea_off = sa['.ARM.exidx'][1]; ea_size = sa['.ARM.exidx'][2]
ta_off = sa['.ARM.extab'][1]; ta_size = sa['.ARM.extab'][2]
ea_addr = sa['.ARM.exidx'][0]; ta_addr = sa['.ARM.extab'][0]
names = dynnames(A)

def prel31(x, at):
    v = x & 0x7fffffff
    if v & 0x40000000:
        v -= 0x80000000
    return at + v

entries = []
for i in range(0, ea_size, 8):
    w0, w1 = struct.unpack_from('<II', da, ea_off + i)
    fn = prel31(w0, ea_addr + i)
    # EHABI: bit31 == 0 and word != 1 -> prel31 to .ARM.extab, base = 2nd word
    extab = prel31(w1, ea_addr + i + 4) if ((w1 & 0x80000000) == 0 and w1 != 1) else None
    entries.append((fn, extab))
extab_entries = sorted((e[1], e[0]) for e in entries if e[1] is not None)
addrs = [x[0] for x in extab_entries]
x = da[ta_off:ta_off + ta_size]
y = db[ta_off:ta_off + ta_size]
diffs = [i for i, (p, q) in enumerate(zip(x, y)) if p != q]
c = Counter()
for i in diffs:
    addr = ta_addr + i
    j = bisect.bisect_right(addrs, addr) - 1
    if j >= 0:
        c[extab_entries[j][1]] += 1
print('extab diff bytes', len(diffs))
for fn, n in c.most_common():
    print('  %-60s %#x diff %d' % (names.get(fn, '?'), fn, n))
```

## Appendix C. `runlink.py` (link-variant helper)

```python
import subprocess, sys

cmd = open('/dev/shm/w11020/linkcmd.txt').read().strip()
marker = (' && /home/debian/GalaxyOnFire2/wt/w11020/.cache/ndk/'
          'android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/'
          'linux-x86_64/bin/arm-linux-androideabi-strip')
link = cmd[:cmd.find(marker)]
link = link.replace(
    '-o /home/debian/GalaxyOnFire2/wt/w11020/cmake-build-match/libgof2hdaa.unstripped.so',
    '-o OUT')
name = sys.argv[1]
for s in sys.argv[2:]:
    a, b = s.split('=>', 1)
    link = link.replace(a, b)
link = link.replace('OUT', '/dev/shm/w11020/%s.unstripped.so' % name)
subprocess.run(link, shell=True, check=True, executable='/bin/bash')
R18 = ('/home/debian/GalaxyOnFire2/wt/w11020/.cache/ndk/android-ndk-r18b/'
       'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
       'arm-linux-androideabi-strip')
subprocess.run([R18, '--strip-all', '-o', '/dev/shm/w11020/%s.so' % name,
                '/dev/shm/w11020/%s.unstripped.so' % name], check=True)
```
