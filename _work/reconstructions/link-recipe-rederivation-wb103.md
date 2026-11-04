# Link recipe and layout re-derivation (wb103, wave 101)

Status: research note, 2026-11-04. Worktree `wt/wb103`, branch `wave101-wb103`,
base `3856949ab`. Scratch is `/dev/shm/wb103`. **No build-file, source or flag
change is landed**: the current CMake/gold recipe is independently re-derived and
confirmed. The note and the scripts in `_work/reconstructions/wb103/` are the
deliverable. The canonical gate is unchanged:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Everything below was measured against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 bytes) with this tree's NDK r18b (`clang 7.0.2`, `gold 1.12`,
GNU `strip` 2.27.0.20170315).

## 0. Bottom line

* The current link line is the original link line, modulo unobservable defaults
  and the reconstruction aids listed in §8.
* `tools/source_order.txt` is the original object order: five independent
  section witnesses (`.text`, `.rodata`, `.data`, `.data.rel.ro`, `.bss`), the
  `.ARM.exidx` entry order and the `.rel.plt` first-reference order all match.
* **No `--section-ordering-file` is needed.** The current plain input-order link
  reproduces all 4,524 exported function addresses. Every ordering-file variant
  tested is worse (`4500 -> 4316/1543/1345/1342`).
* `.rel.plt` and `.rel.dyn` are **entry-for-entry identical** to the original;
  `.dynsym`/`.dynstr`/`.hash` have the same symbol order, and `.dynsym` differs
  only in 661 `st_shndx` fields (+1) caused by the extra `.missioncase` section.
* The only remaining whole-file deltas are source-body/`-g`/reconstruction-aid
  deltas: `.text` 101,704 bytes, `.ARM.exidx` 1,637 bytes, `.ARM.extab` -4
  bytes, `.note.gnu.build-id` 20 bytes, plus the `.missioncase` header/dynsym
  side effects. No link flag fixes them.
* The one structural improvement candidate (removing `.missioncase`) is **not**
  a strict gate improvement: it raises section/file identity but drops
  `byte_exact` 4500 -> 4424 (or worse with an ordering file). Current design is
  kept.

## 1. Baseline and exact link command

```sh
cd /home/debian/GalaxyOnFire2/wt/wb103
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

The exact gold command is the `libgof2hdaa.so` rule in
`cmake-build-match/build.ninja`:

```text
<NDK>/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
  -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a
  -o <build>/libgof2hdaa.unstripped.so
  <NDK>/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o
  -L <NDK>/platforms/android-16/arch-arm/usr/lib
  <192 gof2 objects in tools/source_order.txt order>
  --whole-archive third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a --no-whole-archive
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
  <APK>/libfmodex.so <APK>/libfmodevent.so
  third_party/openssl/libcrypto_gof2.a
  -l m
  <NDK>/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a
  <NDK>/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a
  <NDK>/toolchains/.../armv7-a/thumb/libgcc.a
  -l dl -l c
  <NDK>/platforms/android-16/arch-arm/usr/lib/crtend_so.o
&& <NDK>/toolchains/.../bin/arm-linux-androideabi-strip --strip-all \
     -o <build>/libgof2hdaa.so <build>/libgof2hdaa.unstripped.so
&& python3 tools/reorder_shstrtab.py <build>/libgof2hdaa.unstripped.so <build>/libgof2hdaa.so
```

Extract it reproducibly with:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1 \
  > /dev/shm/wb103/link_cmd_line.txt
```

The relink helpers in `_work/reconstructions/wb103/` read that file and mutate
one token at a time (`relink.py`, `relink_mut.py`, `relink_replace.py`,
`relink_replace_order.py`, `relink_insert.py`). They always re-run the exact
GNU strip and `reorder_shstrtab.py` steps.

## 2. Toolchain and artifact fingerprints

| observable | value | implication |
|---|---|---|
| `.note.gnu.gold-version` | `gold 1.12` (GNU Binutils 2.27.0.20170315) | NDK r18b gold |
| `.note.android.ident` | `api=16`, `ndk="r18b"`, `build="5063045"` | API 16, r18b |
| `.comment` | clang 7.0.2 (`Android (4751641 based on r328903)`) + `GCC: (GNU) 4.9.x 20150123` | app TUs + libcrypto `sha256.o` |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, softfp, unaligned v6 | armeabi-v7a softfp |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | exact link-line order |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `DT_RELCOUNT` | 1162 | `-z combreloc` (default) |
| `.hash`, no `.gnu.hash` | SYSV hash only | gold default `--hash-style=sysv` |
| `PT_GNU_RELRO` | `0x20cba0..0x214000` | `-z relro` (default) |
| `PT_GNU_STACK` | `RW` | `-z noexecstack` (default) |

Archive `.comment` checks:

```sh
AR=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ar
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$AR p third_party/openssl/libcrypto_gof2.a sha256.o > /dev/shm/wb103/sha256.o
$RE -p .comment /dev/shm/wb103/sha256.o          # GCC 4.9.x 20150123
$AR p third_party/zlib/libz_gof2.a inflate.o > /dev/shm/wb103/inflate.o
$RE -p .comment /dev/shm/wb103/inflate.o         # clang 7.0.2
$AR p .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a _aeabi_ldivmod.o > /dev/shm/wb103/ldiv.o
$RE -p .comment /dev/shm/wb103/ldiv.o             # no .comment
```

The original `.comment` is `crtbegin_so.o`'s clang string + `sha256.o`'s GCC
string (deduplicated leading NUL); see
`_work/reconstructions/build-toolchain-runtime-provenance-w5019.md` §2.2 for the
byte decomposition.

`crtbegin_so.o` is required: removing it fails the link with
`requires unsupported dynamic reloc R_ARM_REL32`. `crtend_so.o` is
unobservable: removing it produces a byte-identical `.so`. The API-16
`crtbegin_so.o` is proven by the `.note.android.ident` API word; the API-21
`crtbegin_so.o` changes that word to `0x15`.

## 3. Section table: original vs current

`python3 _work/reconstructions/wb103/fingerprint.py ORIG OURS` prints this
table. `diffB` is the number of differing bytes when sizes match; `NOBITS`
means the section has no file bytes.

```text
section                 orig_size   our_size   orig_addr   our_addr    diffB
.note.android.ident            98         98        134        134        0
.note.gnu.build-id             24         24        1cc        1cc       20
.dynsym                     15ca0      15ca0        1f0        1f0      661
.dynstr                     2e318      2e318      15e90      15e90        0
.hash                        973c       973c      441a8      441a8        0
.gnu.version                 2b94       2b94      4d8e4      4d8e4        0
.gnu.version_d                 1c         1c      50478      50478        0
.gnu.version_r                 40         40      50494      50494        0
.rel.dyn                     7048       7048      504d4      504d4        0
.rel.plt                     72d0       72d0      5751c      5751c        0
.plt                         ac4c       ac4c      5e7ec      5e7ec        0
.text                      1337c8     1337c8      69440      69440   101704
.ARM.exidx                   6f70       6f70     19cc08     19cc08     1637
.ARM.extab                  26298      26294     1a3b78     1a3b78    size!
.rodata                     41b67      41b67     1c9e10     1c9e10        0
.fini_array                     8          8     20cba0     20cba0        0
.data.rel.ro                 2fe8       2fe8     20cbb0     20cbb0        0
.init_array                     c          c     20fb98     20fb98        0
.dynamic                      148        148     20fba4     20fba4        0
.got                         430c       430c     20fcf4     20fcf4        0
.data                        296c       296c     214000     214000        0
.bss                         69ac       69ac     216970     216970   NOBITS
.comment                      12f        12f          0          0        0
.note.gnu.gold-version         1c         1c          0          0        0
.ARM.attributes                38         38          0          0        0
.shstrtab                     117        124          0          0    size!
.missioncase           EXTRA size c0 addr 20b977
```

File size: original `2,187,328`, ours `2,187,380` (+52). Section count:
original 27, ours 28.

All section headers are identical except:

* `.ARM.extab` `sh_size` `0x26294` vs `0x26298` (-4);
* `.shstrtab` `sh_size` `0x124` vs `0x117`;
* `.rel.plt` `sh_info` 21 vs 20 (the extra `.missioncase` section shifts the
  `.got` section index from 20 to 21);
* the extra `.missioncase` section itself.

In particular `.text`, `.rodata`, `.data`, `.data.rel.ro`, `.got`, `.dynamic`,
`.init_array`, `.fini_array`, `.bss`, `.dynsym`, `.dynstr`, `.hash`,
`.gnu.version*`, `.rel.dyn`, `.rel.plt`, `.plt`, `.comment`,
`.note.android.ident`, `.note.gnu.gold-version` and `.ARM.attributes` are
size- and base-exact.

## 4. Dynamic section, program headers, relocations and symbols

* Program headers: 8 in both, same types/order/flags/alignments. `PT_LOAD`,
  `PT_DYNAMIC`, `PT_NOTE`, `PT_GNU_STACK`, `PT_ARM_EXIDX` and `PT_GNU_RELRO`
  addresses/sizes are exact. `.dynamic` is byte-identical (36 entries).
* `DT_NEEDED` is exactly the 10-entry original order above.
* `.rel.dyn`: 2,431 entries, 1,817 `R_ARM_ABS32` + 614 `R_ARM_GLOB_DAT`,
  **entry-for-entry identical**.
* `.rel.plt`: 3,674 `R_ARM_JUMP_SLOT` entries, **entry-for-entry identical**
  (first `__cxa_finalize@LIBC`, last `raise@LIBC`).
* `.dynsym`: 5,578 entries. `st_name`, `st_value`, `st_size` and `st_info` are
  identical for every entry; only 661 `st_shndx` values differ by +1. The +1 is
  exactly the extra `.missioncase` section index. Symbol order is identical.
* `.dynstr` and `.hash` are byte-identical; `.plt` and `.got` are byte-identical.

The `.rel.plt` order is first-reference order in the input object order, not
output-address order. `_work/reconstructions/w5018/relplt_witness.py` walks the
objects in `tools/source_order.txt` order and reproduces the original slot
sequence exactly (`differing slots []`).

## 5. Build-id algorithm (proven)

`--build-id` is SHA-1, not MD5/UUID: the note is `namesz=4 descsz=20 type=3`
(size `0x24`); `--build-id=md5`/`uuid` give `descsz=16` (size `0x20`) and
`--build-id=none` removes the note.

Gold computes the descriptor as `SHA1(unstripped output with the 20-byte
descriptor zeroed)`. Proof on our own unstripped gold output:

```sh
python3 _work/reconstructions/w5018/buildid_proof.py \
  /dev/shm/wb103/flagmatrix/base.unstripped.so \
  /dev/shm/wb103/flagmatrix/base.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
base.unstripped.so  build-id 577509f5b2e670e4510eb95a21dbcdb87dc8f487
                    sha1(zero desc) 577509f5b2e670e4510eb95a21dbcdb87dc8f487  match True
base.so (stripped)  build-id 577509f5b2e670e4510eb95a21dbcdb87dc8f487
                    sha1(zero desc) 2efe0566ac8eb01cde251c4bfc827a16dc3bbf56  match False
original (stripped) build-id c011810da45777b83ed721b2a87d517efed0d310
                    sha1(zero desc) d7c05dc5e9804023d8786147addc48537b0bee6d  match False
```

The original's build-id is therefore a hash of its **unstripped** gold output
(including `.symtab`/`.debug_*`), which no longer exists. A gold `-s` link cannot
reproduce it. A gold `-s` link followed by the bfd strip and `reorder_shstrtab`
is byte-identical to the current output **except the 20-byte build-id**.

## 6. Post-link strip normalization (proven)

The shipped `.so` has bfd-normalized section headers but gold's Stringpool
`.shstrtab` order. The current pipeline is the unique one that reproduces both:

```sh
python3 _work/reconstructions/w5018/strip_proof.py . cmake-build-match ORIG
```

| pipeline | `.shstrtab` | headers |
|---|---|---|
| gold `-s` | same | `.rel.plt` flags/info wrong, `.init_array`/`.fini_array` `sh_entsize=0`, `.bss` offset aligned |
| gold `-s` + bfd strip | same | `.rel.plt` info wrong, `.ARM.extab` size wrong |
| gold (no `-s`) + bfd strip | bfd section order | `.ARM.extab` size wrong |
| gold (no `-s`) + bfd strip + reorder | same | only `.ARM.extab` size and `.missioncase` side effects |

`strip --strip-all` and `strip --strip-unneeded` produce byte-identical output
(`cmp` equal). The NDK `llvm-strip` does **not** normalize `.rel.plt` flags/info,
`INIT_ARRAY`/`FINI_ARRAY` `sh_entsize` or the `.bss` offset; the host GNU
`strip` 2.44 cannot parse the ARM ELF at all. The NDK r18b GNU
`arm-linux-androideabi-strip` is required. `tools/reorder_shstrtab.py` is
required because bfd rewrites `.shstrtab` into section-header order.

## 7. Link order and the section-ordering file

`tools/source_order.txt` has 192 TUs. Independent witnesses:

```sh
ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
python3 _work/reconstructions/w5018/order_witness.py . "$ORIG" cmake-build-match
python3 _work/reconstructions/w5018/exidx_witness.py . "$ORIG" cmake-build-match
python3 _work/reconstructions/w5018/relplt_witness.py . "$ORIG" cmake-build-match/libgof2hdaa.so cmake-build-match
```

```text
.text: 190 objects  order_match=True  ratio=1.000000
.rodata: 78 objects  order_match=True  ratio=1.000000
.data: 59 objects  order_match=True  ratio=1.000000
.data.rel.ro: 78 objects  order_match=True  ratio=1.000000
.bss: 23 objects  order_match=True  ratio=1.000000
exidx entries mapped 2566  objects 190  object_order_matches_source_order True  objects_with_nonascending_addrs 0
first-reference object sequence matches source_order: True
.rel.plt slots 3674  differing slots []
```

The first and last exported functions are at identical addresses in both
binaries (`Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed` at
`0x69578`, `__cxa_demangle` at `0x194030`).

The original was linked **without** a `--section-ordering-file`; the current
build also has none. Every ordering-file variant tested is worse:

| variant | byte_exact | linked_exact |
|---|---:|---:|
| no ordering file (current) | **4500** | **4500** |
| `--section-ordering-file=/dev/null` | 1543 | 4500 |
| minimal file (`.text.startup`, `*`) | 4316 | 4500 |
| exported `.text.<sym>` sorted by original address | 1342 | 4500 |
| full unique `.text.*` order from the current map | 1345 | 4500 |
| old `gen_section_order.py` output | 1342 | 4500 |

The minimal file is the closest because it preserves gold's default `.text.*`
input order; it still loses 184 byte_exact. The reason is that gold's ordering
file groups same-named input sections (e.g. the many
`.text.__clang_call_terminate` copies) and disables the default
`.text.startup`-first sort unless explicitly listed. A section-ordering file is
therefore a reconstruction aid the original did not use and the current tree
does not need.

## 8. Link-flag A/B matrix

All rows are one mutation of the exact current link line, re-stripped and
re-`reorder_shstrtab`'d. `byte`/`linked` are the official verifier counts.
"identical" means `cmp` equal to the current `.so` (including build-id);
"artifact" means the gate is unchanged but the section/dynamic bytes differ.

| variant | byte | linked | verdict |
|---|---:|---:|---|
| current | **4500** | **4500** | baseline |
| `-z lazy` | 2932 | 4500 | changes `.text`/`.dynamic`/`.rel.plt`/`.plt`/`.got` |
| `-z norelro` | 2932 | 4500 | removes `PT_GNU_RELRO`, reorders RW sections |
| `-z execstack` | 4500 | 4500 | only `GNU_STACK` flags RW -> RWX |
| `-z noexecstack` | 4500 | 4500 | identical (default) |
| `-z nocombreloc` | 4489 | 4500 | drops `DT_RELCOUNT`, changes `.rel.dyn` |
| `-z max-page-size=4096` | 4500 | 4500 | identical (default) |
| `-z max-page-size=65536` | — | — | changes LOAD alignment |
| `-z common-page-size=4096/65536` | 4500 | 4500 | identical |
| `--spare-dynamic-tags=5` | 4500 | 4500 | identical (default with `-z now`) |
| `--spare-dynamic-tags=0/10` | — | — | changes `.dynamic` size |
| `--hash-style=sysv` | 4500 | 4500 | identical (default) |
| `--hash-style=gnu` | 1750 | 4500 | adds `.gnu.hash`, changes dynsym |
| `--hash-style=both` | 1750 | 4500 | adds `.gnu.hash` |
| `--sort-section=none` | 4500 | 4500 | identical (default) |
| `--sort-section=name` | 1342 | 4498 | changes `.text`/`.rodata`/dynsym |
| `--sort-section=alignment` | — | — | gold 1.12 link fail |
| `--gc-sections` | 2176 | 4500 | removes empty `.text` sentinels, shrinks exidx |
| `--icf=none` | 4500 | 4500 | identical (default) |
| `--icf=safe` / `--icf=all` | — | — | changes `.text`/exidx |
| `--rosegment` | 1720 | 4500 | adds a third LOAD |
| `-O0` / `-O1` | 4500 | 4500 | identical (gold default class) |
| `-O2` / `-O3` | 1720 | 4500 | changes `.text`/`.rodata`/dynsym |
| `--build-id=sha1` | 4500 | 4500 | identical (default) |
| `--build-id=md5` / `uuid` | 1969 | 4500 | note `0x20` |
| `--build-id=none` | 1750 | 4500 | no note |
| `--exclude-libs=libgcc.a` | 4500 | 4500 | current |
| no `--exclude-libs` | 1720 | 4492 | 3 extra exports (5373) |
| `--exclude-libs=ALL` | 1324 | 3432 | 4916 exports |
| `--exclude-libs=libc++abi.a` | — | — | 5071 exports |
| `--whole-archive` zlib+libzip | 4500 | 4500 | current |
| no whole-archive | 1338 | 4416 | 5281 exports, imports 197 |
| zlib whole / libzip normal | 1354 | 4463 | 5333 exports |
| no zlib / no libzip | 1342 / 1346 | 4453 / 4426 | wrong export/import sets |
| `--whole-archive` libcrypto | 4500 | 4500 | identical (all members already pulled) |
| no libcrypto | 1364 | 4477 | 5347 exports, imports 208 |
| no `libandroid_support.a` | 1503 | 4499 | exidx 3564 |
| no `libgcc.a` | 1345 | 4492 | imports 206, exidx 3564 |
| `libgcc.a` before `libc++abi.a` | 4500 | 4500 | identical (archive rescan) |
| `libandroid_support.a` before `libc++abi.a` | 1503 | 4499 | changes `.text`/exidx |
| swap FMOD after `libc++abi.a` | 4500 | 4500 | artifact: NEEDED/dynsym/rel.plt change |
| FMOD after `libgcc.a` | 4500 | 4500 | artifact: NEEDED/dynsym/rel.plt change |
| swap zlib/libzip | 4366 | 4500 | changes `.text`/`.rodata` |
| no `crtend_so.o` | 4500 | 4500 | identical |
| API-21 `crtbegin_so.o` | 4500 | 4500 | artifact: note API 21 |
| API-21 `crtend_so.o` | 4500 | 4500 | identical |
| add `libc++_static.a` before `libc++abi.a` | 4500 | 4500 | identical (0 members pulled) |
| add `libunwind.a` after `libandroid_support.a` | 4500 | 4500 | identical (0 members pulled) |
| `--as-needed` | 1750 | 4500 | drops 4 NEEDED |
| `--no-as-needed` | 4500 | 4500 | identical (default) |
| `--no-undefined` | 4500 | 4500 | identical |
| `--allow-multiple-definition` | 4500 | 4500 | identical (policy-banned) |
| `--unresolved-symbols=ignore-all` | 4500 | 4500 | identical |
| `--fix-cortex-a8` | 4500 | 4500 | identical |
| `--warn-shared-textrel` | 4500 | 4500 | identical |
| `-z text` / `-z notext` / `-z muldefs` | 4500 | 4500 | identical |
| `--no-copy-dt-needed-entries` / `--no-add-needed` | 4500 | 4500 | identical |
| `--no-allow-shlib-undefined` / `--no-keep-memory` | 4500 | 4500 | identical |
| `-s` on gold | 4500 | 4500 | artifact: only build-id differs |
| `-z separate-code` / `-z noseparate-code` | — | — | gold 1.12: unknown `-z` option |
| `--pack-dyn-relocs=none` / `--no-rosegment` | — | — | gold 1.12: unknown option |
| `--emit-relocs` | — | — | gold 1.12 link fail |

The `--sort-section=name`, `--gc-sections`, `--icf`, `--rosegment`, `-O2` and
`--hash-style=gnu` rows are the strongest negative evidence: each moves `.text`
and/or `.rodata`/`.ARM.exidx` away from the original. The original therefore did
not use them.

## 9. The `.missioncase` section (reconstruction aid)

`src/game/mission/Status.cpp` emits a 192-byte `kMissionCase` table in a
dedicated read-only section:

```cpp
#pragma clang section rodata=".missioncase"
static const unsigned char kMissionCase[192] = { ... };
#pragma clang section rodata=""
```

The table is a codegen device for `Status::missionCompleted` (a still-non-exact
function); it is **not in the original binary**. It sits at `0x20b977`, exactly
at the original `.rodata` end, so all original `.rodata` bytes and addresses
stay exact. Its cost is:

* 28 vs 27 section headers;
* `.shstrtab` `0x124` vs `0x117`;
* `.rel.plt` `sh_info` 21 vs 20;
* 661 `.dynsym` `st_shndx` values +1;
* file size +52 bytes.

Attempts to remove it by merging it into `.rodata` are not gate improvements:

| variant | byte_exact | sections | file size | `.rodata` | `.dynsym` diff | note |
|---|---:|---:|---:|---|---:|---|
| separate `.missioncase` (current) | **4500** | 28 | 2,187,380 | `0x41b67` exact | 661 shndx | best gate |
| rename to `.rodata.missioncase`, no order file | 4424 | 27 | 2,187,328 | `0x41c27` | 219 | table inserted mid-`.rodata` |
| rename + `*`/`.rodata.missioncase` order file | 1529 | 27 | 2,187,328 | `0x41c37` | 854 | table at `.rodata+0x41b77` (+0x10) |
| writable `.missioncase` (SHF_WRITE) | 4469 | 28 | — | `0x41b67` | — | `.bss` shifts to `0x216a30` |

The clean future fix is source-side: make `Status::missionCompleted` emit the
original switch/block layout without the permutation table (e.g. a switch form
whose MachineBlockPlacement order matches the original). When the table is gone,
the extra section, `.shstrtab`, `.rel.plt` `sh_info` and 661 `.dynsym`
`st_shndx` bytes should all become exact with no link-flag change. That is a
source pass, not a link pass, and is not attempted here.

## 10. Proven vs hypothesis

### Proven by artifact fingerprint or controlled A/B

* NDK r18b, clang 7.0.2, gold 1.12, GNU strip 2.27.0.20170315, API 16,
  armeabi-v7a/Thumb-2/NEON/softfp.
* `-shared -soname libgof2hdaa.so --build-id` (SHA-1) `-z now`
  `--exclude-libs=libgcc.a`; no `-s` on gold.
* `-z relro`, `-z noexecstack`, `--hash-style=sysv`, `-z combreloc`,
  `--no-as-needed`, `-z max-page-size=4096`, `--spare-dynamic-tags=5` are the
  defaults the original exhibits.
* `--whole-archive` around zlib+libzip is required; libcrypto whole-archive is
  neutral.
* Archive order is `... FMOD, libcrypto, libm, libc++abi, libandroid_support,
  libgcc, libdl, libc`.
* `crtbegin_so.o` is required and must be the API-16 one; `crtend_so.o` is
  unobservable.
* No `--section-ordering-file`, no `--gc-sections`, no `--icf`, no
  `--sort-section`, no `--rosegment`, no `-O2`, no `--emit-relocs`.
* `tools/source_order.txt` is the original object order (five section
  witnesses + exidx + rel.plt).
* `.rel.dyn` and `.rel.plt` are entry-for-entry identical; `.dynsym` order is
  identical and only 661 `st_shndx` values differ.
* Build-id = SHA-1(unstripped output, descriptor zeroed); GNU bfd strip
  normalization + `reorder_shstrtab.py` is the unique header pipeline.
* The extra `.missioncase` section is the sole cause of the `.shstrtab`,
  `.rel.plt` `sh_info` and `.dynsym` `st_shndx` deltas.

### Hypothesis / unobservable (cannot be distinguished from the binary)

* Whether the original build explicitly spelled the defaults `-z relro`,
  `-z noexecstack`, `--hash-style=sysv`, `--no-as-needed`, `-O0`/`-O1`,
  `--fix-cortex-a8`, `--no-undefined`, `--warn-shared-textrel`,
  `--fatal-warnings`, `-z text/notext/muldefs`, `--no-copy-dt-needed-entries`,
  `--no-add-needed`, `--no-allow-shlib-undefined`, `--no-keep-memory`,
  `--allow-multiple-definition`, `--unresolved-symbols=ignore-all`,
  `-z common-page-size=4096`, `--spare-dynamic-tags=5`, `--icf=none`,
  `--sort-section=none`.
* Whether `libc++_static.a`, `libunwind.a`, `libatomic.a` were present
  (0 members pulled; adding `libc++_static.a`/`libunwind.a` is byte-identical).
* Whether `--exclude-libs=libatomic.a`/`libunwind.a` were present.
* Whether the original used `--strip-all` or `--strip-unneeded` (identical).
* Whether the original used `-s` on gold (only the build-id differs; the
  build-id semantics prove no `-s`).
* Whether `crtend_so.o` was present (removal identical).
* Whether a future source pass can remove `.missioncase` and keep
  `byte_exact 4500` (not attempted; the table is currently load-bearing for
  `Status::missionCompleted`).

## 11. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/wb103
ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
NDK=.cache/ndk/android-ndk-r18b
OBJ=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump

# baseline
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# exact link command
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1 > /dev/shm/wb103/link_cmd_line.txt

# artifact fingerprints
python3 _work/reconstructions/wb103/fingerprint.py "$ORIG" cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/w5018/order_witness.py . "$ORIG" cmake-build-match
python3 _work/reconstructions/w5018/exidx_witness.py . "$ORIG" cmake-build-match
python3 _work/reconstructions/w5018/relplt_witness.py . "$ORIG" cmake-build-match/libgof2hdaa.so cmake-build-match
python3 _work/reconstructions/w5018/buildid_proof.py \
  /dev/shm/wb103/flagmatrix/base.unstripped.so \
  /dev/shm/wb103/flagmatrix/base.so "$ORIG"
python3 _work/reconstructions/w5018/strip_proof.py . cmake-build-match "$ORIG"

# link-flag A/B (w5018 script; writes its scratch under /dev/shm/w5018)
python3 _work/reconstructions/w5018/flag_matrix.py . cmake-build-match "$ORIG"

# one-off relinks with this pass's helpers (scratch /dev/shm/wb103)
/dev/shm/wb103/relink.py /dev/shm/wb103/variant.so -z lazy
python3 tools/verify/verify.py --build-dir /dev/shm/wb103/verify_variant \
  --original "$ORIG" --ours /dev/shm/wb103/variant.so --objdump "$OBJ" \
  --fail-on-wrong-type --top 0
```

The scripts committed with this note are:
`_work/reconstructions/wb103/fingerprint.py`,
`relink.py`, `relink_mut.py`, `relink_replace.py`,
`relink_replace_order.py`, `relink_insert.py`,
`run_verify_variants.sh`, and the old `gen_section_order.py` used for the
ordering-file experiment. The five artifact-witness scripts and
`flag_matrix.py`/`buildid_proof.py`/`strip_proof.py` are the tracked w5018
scripts, referenced above.
