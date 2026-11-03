# Section headers, `.bss` offset and build-id: the original was gold + GNU strip (w4239)

Status: research note + landed build change (2026-11-03), worktree `wt/w4239`, branch
`wave92-w4239`, base master `b379d00d`. Scratch is `/dev/shm/w4239`. Every number is measured
against `../_work/bins/android_2.0.16_libgof2hdaa.so` (the repo's ground truth) and the shipped
APK library `lib/armeabi-v7a/libgof2hdaa.so` (sha256 `6d38fd76…3b58`).

## TL;DR

The last 5 section-header bytes and the unaligned `.bss` file offset are **bfd's `strip`
normalization, not gold's**. The ground truth is not a pure gold artifact: it is the shipped APK
library (which *was* GNU-stripped) with `.shstrtab` reordered back to gold's Stringpool order
(w3566). The original link therefore was **gold without `-s` followed by GNU
`strip --strip-all`**, and its build-id is the SHA-1 of the **unstripped** link output, not of the
shipped file.

Landed: the link now runs gold without `-s`, strips with the NDK's
`arm-linux-androideabi-strip --strip-all`, and restores gold's `.shstrtab` order from the
unstripped output (`tools/reorder_shstrtab.py`). This makes **all 27 section headers byte-exact**
(including `.rel.plt` `SHF_INFO_LINK`/`sh_info=20`, `.init_array`/`.fini_array` `sh_entsize=4`
and `.bss` `sh_offset=0x21596c`) and reduces the whole-file differing-byte count against the
ground truth from **225,879 to 225,874** (and against the shipped APK from **226,167 to
226,162**). It also gives the build-id the original's semantics (hash of the unstripped link
output), which gold `-s` can never do.

Gate before and after: `byte_exact 4498/4524`, `linked_exact 4498/4524`, size-exact
`4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data_exact 843/843`,
imports `207`, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`,
lint `CLEAN`, drift `0`.

## 1. The exact header deltas

`readelf -SW` on the ground truth vs the old gold `-s` output differed in exactly 5 bytes:

| section | field | ground truth | old gold `-s` |
|---|---|---:|---:|
| `.rel.plt` | `sh_flags` | `0x42` (`A`+`SHF_INFO_LINK`) | `0x02` (`A`) |
| `.rel.plt` | `sh_info` | 20 (`.got`) | 0 |
| `.fini_array` | `sh_entsize` | 4 | 0 |
| `.init_array` | `sh_entsize` | 4 | 0 |
| `.bss` | `sh_offset` | `0x21596c` (end of `.data`) | `0x215970` (16-aligned) |

The ELF header and all 8 program headers are already identical. `.ARM.extab` is now size-exact
(`0x26298`), so these 5 bytes were the whole header tail.

## 2. They are bfd's bytes, not gold's

The shipped APK library has **exactly the same 5 values** as the ground truth. It differs from
the ground truth only by 288 bytes, all in `.shstrtab` and the `sh_name` fields (w3566 measured
this; reproduced here). The ground truth is therefore the APK library with `.shstrtab` reordered
to gold's Stringpool order — a repo reconstruction, not a different link.

`gold` 1.12 does not produce those values:

* `gold/arm.cc` never calls `Output_section::set_info_section` for `.rel.plt` (only i386,
  x86-64, s390, powerpc, tilegx and aarch64 do), so `sh_flags`/`sh_info` are `A`/0.
* `Output_section::set_entsize` takes the input sections' `sh_entsize`; clang 7 emits
  `.init_array`/`.fini_array` with `sh_entsize = 0`, so gold writes 0.
* `Output_segment::set_section_list_addresses` aligns every section's file offset to its
  `sh_addralign` (16 for `.bss`), giving `0x215970`.

bfd does all three differently: `assign_section_numbers` sets `SHF_INFO_LINK` and
`sh_info = target section index`, `elf_fake_sections` gives `SHT_INIT_ARRAY`/`SHT_FINI_ARRAY`
`sh_entsize = 4`, and NOBITS offsets are not aligned. The NDK r18b
`arm-linux-androideabi-strip --strip-all` (GNU binutils 2.27) on a **non-stripped** gold output
reproduces all five exactly. Host `arm-none-eabi-strip` 2.44 does the same.

## 3. The build-id proves the original link did not use `-s`

Gold computes the build-id as `SHA1(output file with the 20-byte build-id descriptor zeroed)`.
Measured on our own builds:

| file | stored build-id | SHA1(file with note zeroed) |
|---|---|---|
| `gold -s` output | `d4836d20…1812` | `d4836d20…1812` (match) |
| gold **without** `-s` | `0d6e4026…4031` | `0d6e4026…4031` (match) |
| that file after GNU strip | `0d6e4026…4031` | `392b0b42…f01f` (no match) |

The ground truth's stored build-id is `c011810d…d310`; the APK library carries the same value.
Neither is the hash of the shipped/stripped file:

```
SHA1(ground truth with note zeroed) = d7c05dc5e9804023d8786147addc48537b0bee6d
SHA1(APK lib with note zeroed)      = ca7d79401291dbc354dd5e1f4fc394397fa4f9e1
stored build-id                     = c011810da45777b83ed721b2a87d517efed0d310
```

So the original's build-id is the hash of an intermediate file that is neither the APK library
nor the hybrid ground truth. The only consistent candidate is the **unstripped gold link
output**: gold hashed it, then GNU strip removed `.symtab`/`.strtab` and rewrote the section
headers/`.shstrtab` without recomputing the note. This also explains why the ground truth's
build-id does not match its own bytes. A gold `-s` link hashes the already-stripped file, so it
can never reproduce the original build-id even if every section content byte matches.

## 4. No single NDK r18b tool gives bfd headers *and* gold `.shstrtab`

`strip`/`objcopy` normalize the headers but rewrite `.shstrtab` into section-header order
(259 differing bytes). `llvm-strip --strip-unneeded` leaves the gold headers untouched;
`llvm-strip --strip-all` drops `.comment`/`.note.gnu.gold-version` (25 sections). The original
combination (bfd headers + gold `.shstrtab`) is exactly the hybrid ground truth.

`tools/reorder_shstrtab.py` closes the gap deterministically: it reads the unstripped gold
output's `.shstrtab` (gold's Stringpool order, including suffix sharing), filters it to the 27
sections that survive the strip, and rewrites the stripped file's `.shstrtab` and `sh_name`
fields. On the current link this reproduces the gold `-s` `.shstrtab` byte-for-byte (279 bytes;
verified by `cmp`), with no hard-coded section names or offsets.

## 5. The landed change

`CMakeLists.txt`:

* gold link: `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`
  **without `-s`**, output to `libgof2hdaa.unstripped.so`;
* `${GOF2_NDK_STRIP} --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so`;
* `tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so`;
* remove the unstripped intermediate.

The link is still a single plain gold link; the post-link strip is the original's own pipeline
(not a linker trick, no `--version-script`, no section-ordering file, no manual data). The
unstripped intermediate is only used to recover gold's own `.shstrtab` bytes.

Measured after the change:

```
ours vs ground truth  225,874 differing bytes  (was 225,879)
ours vs APK library   226,162 differing bytes  (was 226,167)
section headers       0 differing bytes        (was 5)
.shstrtab             0 differing bytes
```

Remaining differing bytes are content only: `.text` 106,990, `.ARM.extab` 117,742,
`.ARM.exidx` 1,110, `.note.gnu.build-id` 20, `.rel.plt` 12.

## 6. What remains

* **`.note.gnu.build-id` (20 bytes).** Now semantically correct (hash of the unstripped link
  output), so it will become byte-identical exactly when every other byte of the unstripped
  link output is identical. It is the last section to converge.
* **`.rel.plt` (12 bytes).** The six-entry `PlayerEgo::deleteHackingGame` PLT-slot rotation in
  `LevelScript::process` (w4219 §4); a body block-order issue, not a link knob.
* **`.text` / `.ARM.exidx` / `.ARM.extab`.** Per-function body/register-allocation and EH
  records (the 26 non-linked functions).

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4239
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# expected: byte 4498 linked 4498, RATCHET PASS

# header diff (should print nothing):
python3 - <<'PY'
import struct
O=open('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
U=open('cmake-build-match/libgof2hdaa.so','rb').read()
shoff=struct.unpack_from('<I',O,0x20)[0]; entsz=struct.unpack_from('<H',O,0x2e)[0]
n=struct.unpack_from('<H',O,0x30)[0]
for i in range(n):
    b=shoff+i*entsz
    if struct.unpack_from('<IIIIIIIIII',O,b)!=struct.unpack_from('<IIIIIIIIII',U,b):
        print('sec',i)
PY

# build-id formula:
python3 - <<'PY'
import hashlib
for p in ('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so',
          'cmake-build-match/libgof2hdaa.so'):
    d=bytearray(open(p,'rb').read()); d[0x1cc+16:0x1cc+16+20]=b'\0'*20
    print(p, hashlib.sha1(d).hexdigest())
PY
```
