# Build-id and DWARF reconstruction feasibility (wave106, w5423)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5423`,
branch `wave106-w5423`, base `486f8a119`. Scratch: `/dev/shm/w5423` only.
**No source or build-flag change is landed.** The note is the deliverable.

## 0. Bottom line

* Gold's build-id is **proved** on this tree: it is the SHA-1 of the complete
  **unstripped** gold output with the 20-byte build-id descriptor zeroed. The hash
  includes the section-header table and `.shstrtab`; it is not the hash of the
  shipped stripped file.
* The original build-id `c011810da45777b83ed721b2a87d517efed0d310` is therefore the
  hash of an original **unstripped** link output that is not shipped. The shipped
  `.so` is GNU-stripped and contains no `.symtab`, `.strtab`, or `.debug_*`.
* On our build, 88.7% of the hashed bytes are absent from the shipped file:
  `.debug_*` is 11,048,187 B (57.1%) and `.symtab`+`.strtab` is 6,120,236 B
  (31.6%) of the 19,356,436-byte unstripped output. The shipped 27 sections plus
  headers are only 11.3%.
* The build-id is **all-or-nothing**: every debug flag, path spelling, TU order,
  and code byte changes it, with no partial credit. Thirty controlled rebuilds
  (debug levels, DWARF versions, column info, type units, profiling info,
  `comp_dir`, prefix maps, path spelling, TU order, TU count) all produced
  different build-ids; **none matched `c011810d...`**.
* Matching is feasible **in principle** only as a final checksum after every byte
  of the original unstripped output is reproduced: exact `.text`/EH/relocations,
  exact `.symtab`/`.strtab`, exact `.debug_*`, exact original build directory and
  source-path spelling, exact debug flags, exact TU order. It is **not feasible
  from the shipped stripped file alone** and is not feasible at the current tree
  state. The decisive unknown is the original unstripped link output (equivalently
  its exact `.symtab`/`.strtab`/`.debug_*` bytes and build paths).
* No strict gate improvement exists. The canonical gate is unchanged:
  `byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0`.

## 1. Baseline gate

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5423
mkdir -p /dev/shm/w5423/tmp
TMPDIR=/dev/shm/w5423/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5423/tmp cmake --build cmake-build-match --target check -j6
```

Result:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` from `cmake-build-match/verify/report.json` is **4524/4524**
(`sum(osize == usize)` over 4524 rows). Current stripped-section deltas measured
against the original:

| section | original | ours | differing bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,259,464 | **99,226** |
| `.ARM.exidx` | 28,528 | 28,528 | **829** |
| `.ARM.extab` | 156,312 | 156,312 | **108,294** |
| `.rel.plt` | 29,392 | 29,392 | 0 |
| `.rodata` | 269,159 | 269,159 | 0 |
| `.data` | 10,604 | 10,604 | 0 |
| `.dynsym` / `.dynstr` / `.hash` | exact | exact | 0 |
| `.note.gnu.build-id` | 36 | 36 | 20 |

## 2. Gold build-id algorithm, reproduced end-to-end

The `link_so` target deletes the unstripped output after stripping. To preserve it,
the exact gold command was extracted from Ninja and rerun with a scratch `-o`:

```sh
python3 - <<'PY'
import subprocess
cmd = subprocess.check_output(
    ['ninja','-C','cmake-build-match','-t','commands','libgof2hdaa.so'],
    text=True).strip().splitlines()[-1]
marker = (' && /home/debian/GalaxyOnFire2/wt/w5423/.cache/ndk/'
          'android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/'
          'linux-x86_64/bin/arm-linux-androideabi-strip')
link = cmd.split(marker)[0]
link = link.replace(
    '-o /home/debian/GalaxyOnFire2/wt/w5423/cmake-build-match/libgof2hdaa.unstripped.so',
    '-o /dev/shm/w5423/ours.unstripped.so')
subprocess.run(link, shell=True, check=True, executable='/bin/bash')
PY
readelf -n /dev/shm/w5423/ours.unstripped.so | grep -A2 'build ID'
```

Result:

```text
Build ID: 112db4ba4c5a951699b2c07f187a00e0ada9513e
```

The hash check parses the note by section name, zeroes the descriptor, and hashes
the whole file:

```python
import hashlib, struct
p = '/dev/shm/w5423/ours.unstripped.so'
d = bytearray(open(p, 'rb').read())
e_shoff = struct.unpack_from('<I', d, 0x20)[0]
e_shentsize = struct.unpack_from('<H', d, 0x2e)[0]
e_shnum = struct.unpack_from('<H', d, 0x30)[0]
e_shstrndx = struct.unpack_from('<H', d, 0x32)[0]
shoff = struct.unpack_from('<I', d, e_shoff + e_shstrndx*e_shentsize + 0x10)[0]
shsz = struct.unpack_from('<I', d, e_shoff + e_shstrndx*e_shentsize + 0x14)[0]
shstr = d[shoff:shoff+shsz]
for i in range(e_shnum):
    h = e_shoff + i*e_shentsize
    no = struct.unpack_from('<I', d, h)[0]
    name = shstr[no:shstr.index(0, no)].decode()
    if name == '.note.gnu.build-id':
        off = struct.unpack_from('<I', d, h + 0x10)[0]
        namesz, descsz, typ = struct.unpack_from('<III', d, off)
        desc = off + 12 + ((namesz + 3) & ~3)
        stored = bytes(d[desc:desc+20]).hex()
        z = bytearray(d)
        z[desc:desc+20] = b'\0'*20
        print('stored', stored)
        print('sha1 whole', hashlib.sha1(d).hexdigest())
        print('sha1 zeroed', hashlib.sha1(z).hexdigest())
```

Measured:

| file | size | stored build-id | SHA-1 whole | SHA-1 descriptor zeroed |
|---|---:|---|---|---|
| our unstripped gold output | 19,356,436 | `112db4ba4c5a951699b2c07f187a00e0ada9513e` | `dfdcdad5a89a05cbfa7af3064b8b2f57d1748de1` | **`112db4ba...` (match)** |
| our stripped `.so` | 2,187,328 | `112db4ba...` | `3d52266b7bed9ef0902e67dc56576f3d4f31c6dd` | `53f4df49e6594fbc246bd47e2d554083a4712b16` |
| original stripped `.so` | 2,187,328 | `c011810da45777b83ed721b2a87d517efed0d310` | `39528e8a46f5d75adfd851f71bdeb97c02b060f0` | `d7c05dc5e9804023d8786147addc48537b0bee6d` |
| APK copy (bfd `.shstrtab`) | 2,187,328 | `c011810d...` | `9c722c81172e24751568c6375a07e736915c2401` | `ca7d79401291dbc354dd5e1f4fc394397fa4f9e1` |

The original build-id is not any hash of the shipped stripped file. It is the hash
of a larger unstripped file. Repeated relinks of the same objects in the same build
directory are byte-identical (`ours.unstripped.so` and `ours2.unstripped.so` have
the same SHA-256 `935f3565...`), so the algorithm has no timestamp/randomness
component.

The hash is over the whole file, including the section-header table. On our
unstripped output, with the descriptor zeroed:

| input | SHA-1 |
|---|---|
| whole file | **`112db4ba4c5a951699b2c07f187a00e0ada9513e`** (stored build-id) |
| bytes before `e_shoff` only | `7a14016c3e12d72953dcbc0863b88e743c798735` |
| section-header table zeroed | `8979b97901142bcb5e342bb0875e5f78462e558d` |

### 2.1 Composition of the unstripped hash input

`readelf -SW /dev/shm/w5423/ours.unstripped.so` shows 40 sections:

| part | bytes | share of 19,356,436 |
|---|---:|---:|
| `.debug_*` (11 sections) | 11,048,187 | 57.1% |
| `.symtab` + `.strtab` | 6,120,236 | 31.6% |
| shipped sections + ELF/program headers + `.shstrtab` | 2,188,013 | 11.3% |

The original unstripped size is not shipped and cannot be recovered from the
stripped file. Our build is a reference of the same order (~19 MB), not the
original size.

## 3. Degrees of freedom that change the unstripped hash

All rebuilds below used a scratch wrapper around the tracked toolchain, replacing
only the `-g` token and appending the debug/path flags. The build directory was
`/dev/shm/w5423/build-same` for every row, so `DW_AT_comp_dir` and the compile
command were held constant except for the flag under test. Each row was built with
`link_so`, the unstripped output was preserved by rerunning the gold command, and
`check` was run against the original.

Wrapper:

```cmake
set(GOF2_DBG_FLAG "-g" CACHE STRING "debug-info flag under test")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra debug flags under test")
include("/home/debian/GalaxyOnFire2/wt/w5423/cmake/toolchain.cmake")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _w5423_cxx "${CMAKE_CXX_FLAGS}")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _w5423_c   "${CMAKE_C_FLAGS}")
if(NOT GOF2_EXTRA_FLAGS STREQUAL "")
  string(APPEND _w5423_cxx " ${GOF2_EXTRA_FLAGS}")
  string(APPEND _w5423_c   " ${GOF2_EXTRA_FLAGS}")
endif()
set(CMAKE_CXX_FLAGS "${_w5423_cxx}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${_w5423_c}"   CACHE STRING "" FORCE)
```

Per variant:

```sh
cmake -S . -B /dev/shm/w5423/build-same -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5423/toolchain_dbg.cmake \
  -DGOF2_DBG_FLAG='<flag>' -DGOF2_EXTRA_FLAGS='<extra>' -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5423/build-same --target link_so -j6
# preserve unstripped output by rerunning the gold command with -o /dev/shm/...
cmake --build /dev/shm/w5423/build-same --target check -j6
```

### 3.1 Debug level and DWARF version

| tag | flags | unstripped size | `.debug_*` | `.symtab`+`.strtab` | build-id | stripped zeroed SHA-1 | gate |
|---|---|---:|---:|---:|---|---|---|
| `g` | `-g` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58e57d995806321b7ed660e615ce3b0dd4` | `53f4df49...` | PASS 4500/4500 |
| `g2` | `-g2` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` | `53f4df49...` | PASS 4500/4500 |
| `g3` | `-g3` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` | `53f4df49...` | PASS 4500/4500 |
| `dwarf4` | `-g2 -gdwarf-4` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` | `53f4df49...` | PASS 4500/4500 |
| `nostandalone` | `-g2 -fno-standalone-debug` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` | `53f4df49...` | PASS 4500/4500 |
| `elimtypes` | `-g2 -feliminate-unused-debug-types` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` | `53f4df49...` | PASS 4500/4500 |
| `noelimtypes` | `-g2 -fno-eliminate-unused-debug-types` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` | `53f4df49...` | PASS 4500/4500 |
| `gcolumn` | `-g2 -gcolumn-info` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` | `53f4df49...` | PASS 4500/4500 |
| `dwarf2` | `-g2 -gdwarf-2` | 19,966,756 | 11,658,538 | 6,120,204 | `869b4ed9af6808084d8c084eb81632755add14a4` | `53f4df49...` | PASS 4500/4500 |
| `dwarf3` | `-g2 -gdwarf-3` | 19,773,284 | 11,465,032 | 6,120,236 | `f1f8c733d111992ff0fb44d1068a4fe332a645c5` | `53f4df49...` | PASS 4500/4500 |
| `nocolumn` | `-g2 -gno-column-info` | 18,823,836 | 10,515,586 | 6,120,236 | `ecd0ac6d860f537b0e3f9ff427243dae2f175d5b` | `53f4df49...` | PASS 4500/4500 |
| `dbgtypes` | `-g2 -fdebug-types-section` | 14,762,036 | 6,453,731 | 6,120,236 | `26b082474da2ae7b9158330bd322a6eec28a871d` | `53f4df49...` | PASS 4500/4500 |
| `profiling` | `-g2 -fdebug-info-for-profiling` | 19,563,404 | 11,255,153 | 6,120,236 | `3cddf826ac940e4d1ebccc1121b88e4f8f254465` | `53f4df49...` | PASS 4500/4500 |
| `g0` | `-g0` | 3,660,252 | 685,917 | 786,316 | `9798af827d4515f53ee1f5a89b0c6b05a1b4f424` | `c0fa361b...` | FAIL 2069/4247, size 4485/4524 |
| `g1` | `-g1` | 5,183,556 | 2,179,499 | 816,044 | `6a073ea43edf24ae5d3e891c4eebb4a6fc233153` | `c0fa361b...` | FAIL 2069/4247 |
| `gline` | `-gline-tables-only` | 5,183,556 | 2,179,499 | 816,044 | `6a073ea43edf24ae5d3e891c4eebb4a6fc233153` | `c0fa361b...` | FAIL 2069/4247 |

Key facts:

* `-g` == `-g2` == `-g3` == `-gdwarf-4` == `-fno-standalone-debug` ==
  `-feliminate-unused-debug-types` == `-fno-eliminate-unused-debug-types` ==
  `-gcolumn-info`: byte-identical unstripped output and same build-id in the same
  directory.
* `-g1` == `-gline-tables-only`: byte-identical unstripped output and same
  build-id. `-g0` has a different unstripped output (no `.debug_info`) but the same
  stripped codegen as `-g1`/`-gline` (`c0fa361b...` stripped zeroed SHA-1).
* `-g0`/`-g1`/`-gline` are disproven by codegen: byte 2069, linked 4247,
  size-exact 4485/4524 vs baseline 4500/4500, 4524/4524. The original must be
  `-g2`-class (`-debug-info-kind=limited`).
* `-gdwarf-2`, `-gdwarf-3`, `-gno-column-info`, `-fdebug-types-section`, and
  `-fdebug-info-for-profiling` are codegen-neutral (stripped zeroed SHA-1
  `53f4df49...`, gate PASS 4500/4500) but change the build-id. They are
  unobservable from the shipped `.so`.
* `-fdebug-types-section` adds a 41st section (`.debug_types`) and shrinks the
  unstripped file by 4,594,376 B; it is not a default and has no positive evidence.
* `-g2 -gdwarf-5` is unusable with clang 7.0.2 here. Compiling
  `src/engine/render/PaintCanvas.cpp` with the exact compile line and
  `-g2 -gdwarf-5` exits 254 with a clang frontend signal:
  `PLEASE submit a bug report ... Preprocessed source(s) ... /tmp/PaintCanvas-*.cpp`.
  This reproduces the wave-104 result (45 TUs crash).

Driver mapping (`clang++ -###`):

| flag | cc1 setting |
|---|---|
| `-g`, `-g2`, `-g3` | `-debug-info-kind=limited -dwarf-version=4 -dwarf-column-info` |
| `-g1`, `-gline-tables-only` | `-debug-info-kind=line-tables-only -dwarf-version=4 -dwarf-column-info` |
| `-g0` | no `-debug-info-kind`; `-dwarf-column-info` only |
| `-g2 -gdwarf-2/3/4` | `-debug-info-kind=limited -dwarf-version=2/3/4` |
| `-g2 -gno-column-info` | `-debug-info-kind=limited -dwarf-version=4` (no `-dwarf-column-info`) |
| `-g2 -fdebug-types-section` | adds `-generate-type-units` |
| `-g2 -fdebug-info-for-profiling` | adds `-fdebug-info-for-profiling` |

`-fdebug-compilation-dir` is cc1-only in clang 7.0.2; the driver rejects
`-fdebug-compilation-dir=/src`. It must be passed as
`-Xclang -fdebug-compilation-dir -Xclang /src`.

### 3.2 `comp_dir`, source-path spelling, prefix maps

All rows below have the **same stripped `.so`** after zeroing the build-id
(`53f4df49e6594fbc246bd47e2d554083a4712b16`) and pass the same gate
(4500/4500, 4524/4524, data 843, imports 207, lint CLEAN, drift 0). Only the
unstripped bytes and the build-id change.

| tag | `comp_dir` / prefix map | unstripped size | `.debug_*` | `.symtab`+`.strtab` | build-id |
|---|---|---:|---:|---:|---|
| baseline | build dir `/dev/shm/w5423/build-same` | 19,356,412 | 11,048,159 | 6,120,236 | `951bfa58...` |
| `compdir_hck` | comp_dir `/Users/hck/Documents/Dev/GalaxyOnFire2_merged` | 19,356,428 | 11,048,179 | 6,120,236 | `c44f6563a0765b907d498198464c9cac6c82d753` |
| `compdir_ntran` | comp_dir `/Users/n.tran/projects/gof2` | 19,356,412 | 11,048,161 | 6,120,236 | `ac11bdf01236b406a9f0f3e53837770376a25997` |
| `compdir_mhehmeyer` | comp_dir `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2` | 19,356,428 | 11,048,178 | 6,120,236 | `f6a133039d8308d1aa9906b3fe9103791466cf96` |
| `prefix_hck` | map tree root to `/Users/hck/Documents/Dev/GalaxyOnFire2_merged` | 18,444,220 | 10,336,286 | 5,919,916 | `2c253e9aca7893f09a8aa77d792c08f37e22511c` |
| `prefix_ntran` | map tree root to `/Users/n.tran/projects/gof2` | 18,403,916 | 10,295,984 | 5,919,916 | `c44880d1b2c6f0d3bb353ca6e949d36ca59d7a23` |
| `prefix_mhehmeyer` | map tree root to `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2` | 18,441,980 | 10,334,047 | 5,919,916 | `29b2a52f26dd19f541fdd94c2aee8d2da3960158` |
| `prefix_dot` | map tree root to `.` | 18,345,700 | 10,237,770 | 5,919,916 | `da9f32113ecc59399e81b1ff63a6f8fd764eecb7` |
| `prefix_src` | map tree root to `/src` | 18,352,420 | 10,244,487 | 5,919,916 | `65d2167637958e20f56ff8ce5f8e09d6ab088276` |
| `prefix_jni` | map tree root to `/jni` | 18,352,420 | 10,244,487 | 5,919,916 | `635dffec00cc6dccf96deab1ec9add83bbffaa1c` |
| `prefix_hck_compdir_hck` | prefix map + comp_dir hck | 18,444,236 | 10,336,306 | 5,919,916 | `7769a4fe7b29c75112c1492a6c5d49fa2a8a0491` |
| `prefix_ntran_compdir_ntran` | prefix map + comp_dir ntran | 18,403,916 | 10,295,986 | 5,919,916 | `3f77cd3b3156aac2a1b6f5e034d0809ba3fd01ec` |
| `prefix_hck_slash` | map tree root to `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/` (trailing slash) | 18,446,460 | 10,338,525 | 5,919,916 | `631481e1c5491db1bd99a70f98aa538b2619693e` |

The path strings are in the DWARF and `.symtab` file symbols, not in the stripped
`.so`. For example:

```sh
readelf --debug-dump=info /dev/shm/w5423/ours.unstripped.so 2>/dev/null |
  grep -m3 -E 'DW_AT_(producer|comp_dir|name)'
```

shows `DW_AT_name` as the absolute source path
(`/home/debian/GalaxyOnFire2/wt/w5423/src/platform/android/BackButton.c`) and
`DW_AT_comp_dir` as the build directory
(`/home/debian/GalaxyOnFire2/wt/w5423/cmake-build-match`). `readelf
--debug-dump=decodedline` shows the same absolute paths in the line table. A
prefix map rewrites both the DWARF strings and the ELF file symbols, which is why
it also changes `.symtab`/`.strtab` size.

The source-layout dumps give these plausible original roots:

* `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/` (mac FHD merged tree)
* `/Users/n.tran/projects/gof2/` (iOS HD 1.1.19)
* `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/` (iOS SD 1.0.1)

No Android build directory or Android source path survives in the APK or the
`.so`. The original Android tree was probably one of these macOS roots, but the
exact `comp_dir` and file spelling (absolute vs relative, trailing slash, `src/`
vs `Classes/`, `jni/`, etc.) are not observable. A trailing slash alone changes
the build-id (`prefix_hck` vs `prefix_hck_slash`) and the unstripped size by
2,240 B.

### 3.3 TU order and TU count

Two controlled source-list experiments (source list backed up and restored
afterwards):

| tag | change | unstripped size | build-id | stripped zeroed SHA-1 | gate |
|---|---|---:|---|---|---|
| `order_swap` | swap first two TUs (`BackButton.c`, `NFC.cpp`) in `tools/source_order.txt` | 19,356,412 | `0504dd5b6e9b1c7f1d66129886711ee640fc338a` | `29f07ea9...` | FAIL byte 3733/4500 |
| `extra_tu` | add `src/platform/android/EmptyProbe.c` (`int empty_probe_variable;`) at end of list | 19,356,928 | `3ec717c99833cec06404a470ef9d76c7fdd15ca6` | `246ba26b...` | FAIL byte 1750/4500, extra export |

The original TU membership and order are known from the source dumps and the
`.dynsym` reconstruction, but the current tree still has invented TUs and
different source paths/local symbols. Even a correct order with the wrong TU
contents/paths cannot match the hash.

## 4. What is proven vs still hypothesis

### Proven by command

* Gold `--build-id` = SHA-1 of the complete unstripped output with the 20-byte
  descriptor zeroed. The hash includes `.shstrtab` and the section header table.
* The original `c011810d...` is not the hash of the shipped stripped file
  (`d7c05d...` ground truth, `ca7d79...` APK copy). It must be the hash of a
  larger unstripped output.
* The original was linked without `-s` and then GNU-stripped (build-id + bfd
  section-header normalization + `.shstrtab` reorder).
* `-g2`-class debug info is required by codegen; `-g0`, `-g1`, and
  `-gline-tables-only` drop the gate to 2069/4247 and size-exact 4485/4524.
* `-g`, `-g2`, `-g3`, `-gdwarf-4`, `-fno-standalone-debug`,
  `-feliminate-unused-debug-types`, `-fno-eliminate-unused-debug-types`, and
  `-gcolumn-info` are indistinguishable on this tree (same unstripped bytes and
  same build-id in the same directory).
* `-gdwarf-2`, `-gdwarf-3`, `-gno-column-info`, `-fdebug-types-section`,
  `-fdebug-info-for-profiling`, `comp_dir`, `-fdebug-prefix-map`, path spelling,
  TU order, and TU count all change the build-id while leaving the stripped
  `.so` identical (for the codegen-neutral debug/path variants).
* `-g2 -gdwarf-5` crashes clang 7.0.2 on `PaintCanvas.cpp`.
* None of the 30 tested variants produced `c011810d...`.

### Still hypothesis / unobservable

* The exact original unstripped size (our build is 19,356,436 B; the original is
  not shipped).
* The exact original `.symtab`/`.strtab` and `.debug_*` bytes.
* The exact original `DW_AT_comp_dir`, `DW_AT_name` spelling, and build directory.
* The exact original DWARF version and debug flags among the codegen-neutral set
  (`-gdwarf-2/3/4`, `-gno-column-info`, `-fdebug-types-section`,
  `-fdebug-info-for-profiling`, `-fno-standalone-debug`, prefix maps).
* Whether the original used `-fdebug-types-section` (would add a 41st section) or
  `-gno-column-info` (would shrink debug info by ~533 KB).
* Whether a byte-identical unstripped output is achievable without the original
  source (the current tree still has 99,226 differing `.text` bytes, 829
  `.ARM.exidx`, 108,294 `.ARM.extab`, and different local symbols/TU paths).

## 5. Feasibility verdict

**In principle: yes, but only as a final whole-file checksum.** If every byte of
the original unstripped gold output is reproduced — shipped sections, `.symtab`,
`.strtab`, `.debug_*`, section headers, `.shstrtab`, and the exact original build
paths/flags — then the SHA-1 will be `c011810d...`. That would indeed be decisive
proof of the complete recipe.

**In practice, from the shipped `.so` alone: no.** The build-id is a one-way hash
over ~17 MB of data that is not in the shipped file. It has no partial credit and
cannot guide a search. The current tree is not byte-exact in `.text`/EH, and its
local symbol/TU/path set differs from the original. Even a perfect guess of
`comp_dir` and debug flags would not match until the code, `.symtab`, and
`.debug_*` are exact.

**Decisive unknown:** the original unstripped link output (equivalently its exact
`.symtab`/`.strtab`/`.debug_*` bytes and original build directory/source paths).
If that artifact or a full original source tree + build directory is ever found,
the build-id becomes a strong final check. Until then it is not a usable oracle
for the remaining 24 functions.

No source or flag change is justified by this pass. The canonical build remains
`byte 4500/4524, linked 4500/4524, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS`.

## 6. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w5423
mkdir -p /dev/shm/w5423/tmp
TMPDIR=/dev/shm/w5423/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5423/tmp cmake --build cmake-build-match --target check -j6

# preserve and hash our unstripped output (see §2)
python3 - <<'PY'
import subprocess
cmd = subprocess.check_output(
    ['ninja','-C','cmake-build-match','-t','commands','libgof2hdaa.so'],
    text=True).strip().splitlines()[-1]
marker = (' && /home/debian/GalaxyOnFire2/wt/w5423/.cache/ndk/'
          'android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/'
          'linux-x86_64/bin/arm-linux-androideabi-strip')
link = cmd.split(marker)[0].replace(
    '-o /home/debian/GalaxyOnFire2/wt/w5423/cmake-build-match/libgof2hdaa.unstripped.so',
    '-o /dev/shm/w5423/ours.unstripped.so')
subprocess.run(link, shell=True, check=True, executable='/bin/bash')
PY
readelf -n /dev/shm/w5423/ours.unstripped.so | grep -A2 'build ID'
readelf -SW /dev/shm/w5423/ours.unstripped.so | grep -E '\.debug|\.symtab|\.strtab'

# original stripped hashes
python3 - <<'PY'
import hashlib
p='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
d=bytearray(open(p,'rb').read())
d[0x1dc:0x1f0]=b'\0'*20
print(hashlib.sha1(d).hexdigest())   # d7c05dc5e9804023d8786147addc48537b0bee6d
PY

# variant rebuild (example: -gno-column-info)
cmake -S . -B /dev/shm/w5423/build-same -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5423/toolchain_dbg.cmake \
  -DGOF2_DBG_FLAG='-g2 -gno-column-info' -DGOF2_EXTRA_FLAGS='' \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5423/build-same --target link_so -j6
cmake --build /dev/shm/w5423/build-same --target check -j6
```
