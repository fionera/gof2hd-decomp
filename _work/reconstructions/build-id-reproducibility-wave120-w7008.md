# Build-id and reproducibility as a build-provenance constraint (wave 120, w7008)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7008`,
branch `wave120-w7008`, base `ef7fb9cde`. Scratch: `/dev/shm/w7008`.
**No `src/`, `tools/`, baseline, allowlist, or CMake change is landed.** The note is the
only deliverable.

Canonical gate, measured before writing this note:

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
size-exact 4524/4524
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Original: `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B, build-id `c011810da45777b83ed721b2a87d517efed0d310`.
Canonical build: `sha256 83d9bc6be57b8e72c4889aa5536274e2764cb26ba88ef9c4dfd545100f66b260`,
2,187,328 B, build-id `287186a09650e8d72cbc363c7945cf800cdd65d6`.

## 0. Bottom line

* **Gold's build-id algorithm is re-proved on the current build**: gold's default
  `--build-id` is the SHA-1 of the complete **unstripped** gold output with the 20-byte
  build-id descriptor zeroed. `--build-id=sha1` is identical; `--build-id=md5` is a
  different algorithm. GNU `strip --strip-all` preserves the note, so the shipped
  stripped file carries a build-id that is a hash of bytes it no longer contains.
* The original `c011810d...` is therefore a 160-bit commitment to an original
  **unstripped** gold output that is not shipped. It is not the SHA-1 of the shipped
  stripped file (`d7c05dc5e9804023d8786147addc48537b0bee6d` with the descriptor zeroed).
* On our build, 88.7% of the hashed bytes are absent from the shipped `.so`: 57.07%
  `.debug_*` and 31.63% `.symtab`+`.strtab`; only 11.30% is the shipped image plus
  headers. The original unstripped size is not shipped, but is necessarily of the same
  order (~19 MB).
* **Build-id is sensitive to**: source-path spelling, build-directory spelling, `-g`
  level, DWARF version, `-fdebug-prefix-map`/`-fdebug-compilation-dir`, and link
  order. Each of those changes the unstripped hash even when the stripped `.so` is
  byte-identical apart from the note.
* **Build-id is not sensitive to**: the compiler executable's path (a symlinked NDK
  compiler produces the same hash), archive file/member timestamps, or
  `TZ`/`LC_ALL`/`SOURCE_DATE_EPOCH` on this tree.
* **CMake version itself is not hashed.** CMake 3.6.4111459, 3.20.6, 3.22.6 and
  3.31.6 produce the same build-id when their effective compile/link commands are the
  same. CMake 3.6/3.10 can still change the build-id indirectly: they ignore the
  source-file `COMPILE_OPTIONS` property used by the current tree, silently dropping
  the two per-file branch-weight flags. With `COMPILE_FLAGS` (or otherwise identical
  commands), the build-id is identical across those versions.
* **The original build-id cannot constrain the original build directory, CMake
  version, or host by itself.** It is a one-way whole-file hash with no partial
  credit; the app's `DW_AT_comp_dir`, source paths, `.symtab`, and `.debug_*` are
  absent from the shipped file. A small candidate-path brute force is feasible only
  after every other byte of the original unstripped output is already exact.
* **The build-id is not recoverable independently of the remaining 19 functions.**
  Those functions' `.text`, symbol sizes/order, line tables, and EH records are all
  in the hash. The 20-byte value is known from the shipped note and can be copied
  synthetically, but a genuine gold build-id will only equal `c011810d...` when the
  whole unstripped output matches. The build-id is a final checksum, not an oracle
  that can solve the remaining functions.

## 1. Baseline and current residual

The current tree has **19 non-byte-exact functions**, all size-exact:

```text
49.0  Layout::Layout C1/C2 (7072 B)
60.0  LevelScript::process (57232 B)
74.4  Level::createShip (2764 B)
74.5  Radar::draw (9908 B)
81.6  MGame::OnTouchEnd (8080 B)
84.7  Generator::createMission (1996 B)
89.7  HangarWindow::render (9700 B)
92.3  Level::createMission (17172 B)
92.5  Status::missionCompleted (1432 B)
92.8  Level::createScene (2252 B)
93.4  Gun::calcCharacterCollision (1984 B)
95.5  PlayerEgo::calcCollision (968 B)
95.7  SpaceLounge::drawLounge (1948 B)
96.0  Hud::draw (9332 B)
98.1  MenuTouchWindow::OnTouchBegin (1896 B)
99.2  ModStation C1/C2 (1008 B)
99.9  MGame::OnUpdate (11440 B)
```

Current stripped-section deltas against the original (both files 2,187,328 B):

| section | original | ours | differing bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,259,464 | **88,216** |
| `.ARM.exidx` | 28,528 | 28,528 | **863** |
| `.ARM.extab` | 156,312 | 156,312 | **109,364** |
| `.rodata` | 269,159 | 269,351 | **192** (appended mission table) |
| `.rel.plt` | 29,392 | 29,392 | **6** |
| `.note.gnu.build-id` | 36 | 36 | **20** |

The `.rodata` prefix is exact; the 192 B `Status::missionCompleted` rank table is the
only extra `.rodata` input. `.bss` is NOBITS; a naive byte compare reports a 2-byte
file-padding artifact, not a real section-content difference.

The current unstripped gold output is 19,367,052 B (40 sections):

| part | bytes | share |
|---|---:|---:|
| `.debug_*` (11 sections) | 11,053,346 | 57.07% |
| `.symtab` + `.strtab` | 6,125,689 | 31.63% |
| shipped sections + ELF/program headers + `.shstrtab` | 2,188,017 | 11.30% |

## 2. Gold build-id algorithm re-proved on this build

The build deletes the unstripped output after stripping. Re-running the exact gold
command with a scratch `-o` preserves it. The note is at file offset `0x1cc`; its
descriptor is at `0x1dc` (12-byte note header + 4-byte `"GNU\0"` name, padded to 4).

```python
import hashlib, struct
p = '/dev/shm/w7008/out/base.unstripped.so'
d = bytearray(open(p, 'rb').read())
# locate .note.gnu.build-id by section name, zero descsz bytes at desc, SHA-1 whole file
...
```

Measured:

| file | size | stored build-id | SHA-1 whole | SHA-1 descriptor zeroed |
|---|---:|---|---|---|
| our unstripped gold output | 19,367,052 | `287186a09650e8d72cbc363c7945cf800cdd65d6` | `43364bb517d66aa692fd1e027ebbcf5db8839ed2` | **`287186a09650e8d72cbc363c7945cf800cdd65d6` (match)** |
| our stripped `.so` | 2,187,328 | `287186a09650e8d72cbc363c7945cf800cdd65d6` | `6f375eb279a552bc16744e14f2d16da545dfab95` | `49da34618dbc2adcd4c381395aedd5d0231155f2` |
| original stripped `.so` | 2,187,328 | `c011810da45777b83ed721b2a87d517efed0d310` | `39528e8a46f5d75adfd851f71bdeb97c02b060f0` | `d7c05dc5e9804023d8786147addc48537b0bee6d` |

The original build-id is not any hash of the shipped stripped file: the stripped
file's own descriptor-zeroed SHA-1 is `d7c05d...`, not `c011810d...`. The original
must therefore be the hash of a larger unstripped gold output.

`--build-id` defaults to SHA-1 and is identical to `--build-id=sha1`; `--build-id=md5`
uses a different algorithm:

| link flag | stored build-id | SHA-1 descriptor zeroed |
|---|---|---|
| `--build-id` (default) | `287186a0...` | `287186a0...` |
| `--build-id=sha1` | `287186a0...` | `287186a0...` |
| `--build-id=md5` | `2e8a8cd04f53d071b335281392881ff2` | `c71cd0ab...` |

The hash covers the whole file, including the section-header table and `.shstrtab`.
GNU `strip --strip-all` preserves the note but rewrites section headers and
`.shstrtab`; `tools/reorder_shstrtab.py` restores gold's Stringpool order in the
stripped file. Neither post-link step changes the stored build-id, because the hash
was already taken over the unstripped file.

## 3. Controlled sensitivity experiments

All variant builds used the same NDK r18b toolchain, the same source tree except where
noted, and the same link inputs. `stripped-zeroed` is the SHA-1 of the final stripped
`.so` after zeroing its 20-byte build-id descriptor; when it is `49da3461...`, the
stripped `.so` is byte-identical to the canonical build apart from the build-id.

### 3.1 Source path

Same build directory `/dev/shm/w7008/build-cmake36`, same CMake 3.31.6, same flags;
only the source-tree path changed from `/dev/shm/w7008/compat-src` to
`/dev/shm/w7008/csrc2`.

| source path | unstripped size | build-id | stripped-zeroed |
|---|---:|---|---|
| `/dev/shm/w7008/compat-src` | 19,344,500 | `c2c3e95b75742ecb40f5265b7a7cd5ecb8da0364` | `49da3461...` |
| `/dev/shm/w7008/csrc2` | 19,333,236 | `af83221b2497fdc5fa0b94106e3187e5e6049048` | `49da3461...` |

The source path is embedded in `DW_AT_name`, the line-table file table, and `.symtab`
file symbols. A shorter path shrinks the unstripped file and changes the hash while
leaving the stripped image identical.

### 3.2 Build directory

Same source `/dev/shm/w7008/compat-src`, same CMake 3.31.6, same flags; only the build
directory changed from `/dev/shm/w7008/build-cmake36` to `/dev/shm/w7008/build-b`.

| build directory | unstripped size | build-id | stripped-zeroed |
|---|---:|---|---|
| `/dev/shm/w7008/build-cmake36` | 19,344,500 | `c2c3e95b75742ecb40f5265b7a7cd5ecb8da0364` | `49da3461...` |
| `/dev/shm/w7008/build-b` | 19,344,492 | `a60f81eba2b6625cc08f3a6c4d249d6b5ee4a194` | `49da3461...` |

`DW_AT_comp_dir` and the line-table directory table embed the build directory. The
build directory is therefore a real build-provenance input, but it is not recoverable
from the shipped file.

### 3.3 CMake version

Two controlled sets, both with the same source path and build directory within each
set. The first set uses the current tree's source-file `COMPILE_OPTIONS` property;
CMake 3.6/3.10 do not support it, so they silently drop the two per-file branch-weight
flags. The second set uses `COMPILE_FLAGS`, which all versions support, so the
effective commands are identical.

| CMake | source property | unstripped size | build-id | stripped-zeroed |
|---|---|---:|---|---|
| 3.6.4111459 (`3.6.0-rc2`) | `COMPILE_OPTIONS` (dropped) | 19,344,508 | `d6df008d57405595944c39dfcf525b409471f924` | `6ef06b7d...` |
| 3.10.2 | `COMPILE_OPTIONS` (dropped) | 19,344,508 | `d6df008d57405595944c39dfcf525b409471f924` | `6ef06b7d...` |
| 3.20.6 | `COMPILE_OPTIONS` | 19,344,500 | `c2c3e95b75742ecb40f5265b7a7cd5ecb8da0364` | `49da3461...` |
| 3.25.3 | `COMPILE_OPTIONS` | 19,344,500 | `c2c3e95b75742ecb40f5265b7a7cd5ecb8da0364` | `49da3461...` |
| 3.31.6 | `COMPILE_OPTIONS` | 19,344,500 | `c2c3e95b75742ecb40f5265b7a7cd5ecb8da0364` | `49da3461...` |
| 3.6.4111459 | `COMPILE_FLAGS` | 19,349,004 | `069d65333778cfbdd5e7a57feb00aa1034d9a164` | `49da3461...` |
| 3.20.6 | `COMPILE_FLAGS` | 19,349,004 | `069d65333778cfbdd5e7a57feb00aa1034d9a164` | `49da3461...` |
| 3.22.6 | `COMPILE_FLAGS` | 19,349,004 | `069d65333778cfbdd5e7a57feb00aa1034d9a164` | `49da3461...` |
| 3.31.6 | `COMPILE_FLAGS` | 19,349,004 | `069d65333778cfbdd5e7a57feb00aa1034d9a164` | `49da3461...` |

**Conclusion:** the CMake version string is not embedded. CMake only matters through
the effective command line it generates. The 3.6/3.10 `COMPILE_OPTIONS` gap is a real
trap for the current tree, but it is a property-support difference, not a CMake-version
fingerprint. The original's likely AGP-3.1.2-era CMake versions (3.6.4111459 and
3.10.2) are both in the tested set; `c011810d...` cannot distinguish them.

### 3.4 Compiler path

Same source, build directory, CMake 3.31.6, and flags; the NDK was reached through a
symlink `/dev/shm/w7008/ndk-alt` instead of the real `.cache/ndk/...` path.

| compiler path | unstripped size | build-id | stripped-zeroed |
|---|---:|---|---|
| real NDK path | 19,344,500 | `c2c3e95b75742ecb40f5265b7a7cd5ecb8da0364` | `49da3461...` |
| symlinked NDK path | 19,344,500 | `c2c3e95b75742ecb40f5265b7a7cd5ecb8da0364` | `49da3461...` |

Clang does not embed its own executable path; `DW_AT_producer` carries the version
string, not the path. A different compiler path is unobservable in the build-id.

### 3.5 `-g` level and DWARF version

Same source, build directory `/dev/shm/w7008/build-g`, CMake 3.31.6. The tracked
toolchain's `-g` token was replaced.

| debug flag | unstripped size | build-id | stripped-zeroed |
|---|---:|---|---|
| `-g` | 19,344,492 | `107111c7d55609da23cf239ba8abf9ab4aec7503` | `49da3461...` |
| `-g2` | 19,344,492 | `107111c7d55609da23cf239ba8abf9ab4aec7503` | `49da3461...` |
| `-g3` | 19,344,492 | `107111c7d55609da23cf239ba8abf9ab4aec7503` | `49da3461...` |
| `-g0` | 3,660,252 | `8ba62e6ee1e59652c56b0de7cbf92e746fe701ae` | `1eb88a41...` |
| `-g1` | 5,177,428 | `90c5e04f004574da2dad794aa50be95d0036cf40` | `1eb88a41...` |
| `-g -gdwarf-2` | 19,955,484 | `fad0c2474cf094b3f138e302e164053c80287694` | `49da3461...` |

`-g`/`-g2`/`-g3` are identical; `-g0`/`-g1` change codegen at `-Oz` (PostRA sinking)
and therefore also the stripped image. `-gdwarf-2` is codegen-neutral but changes the
build-id. The original must be `-g2`-class from the gate, but the build-id cannot
distinguish `-g2` from `-g3` or DWARF 2/3/4 among codegen-neutral choices.

### 3.6 Debug-prefix-map and compilation-dir variants

Same source, build directory `/dev/shm/w7008/build-pm`, CMake 3.31.6. All rows have
the same stripped image (`49da3461...`) and differ only in the unstripped hash.

| variant | unstripped size | build-id |
|---|---:|---|
| baseline | 19,344,492 | `9b97108cf494aa63ad9dbeb3bc66b16ec31bf97c` |
| map source root to `/src` | 18,352,476 | `03900be46a1290c0a5b4efc80ee5fdea060c813a` |
| map source root to `.` | 18,345,748 | `3eecc08a7575388c7b563b0b0f971194986e56dc` |
| map source root to `/Users/hck/Documents/Dev/GalaxyOnFire2_merged` | 18,444,356 | `69d8071186ce574a43e6c3d37a71f35f41669b52` |
| `-Xclang -fdebug-compilation-dir -Xclang /src` | 19,344,476 | `68d690beb1273a1c973beb7a17528f0426f5e675` |

Prefix maps rewrite both DWARF strings and ELF file symbols, so they also change
`.symtab`/`.strtab` sizes. A trailing slash, a different root spelling, or `comp_dir`
alone changes the hash. This is why the original build directory cannot be guessed
from the shipped file.

### 3.7 Environment variables

Same source, build directory `/dev/shm/w7008/build-env`, CMake 3.31.6. The second row
was built with `TZ=America/New_York LC_ALL=C SOURCE_DATE_EPOCH=0`; the first row used
the ambient environment.

| environment | unstripped size | build-id | stripped-zeroed |
|---|---:|---|---|
| ambient | 19,344,492 | `9b97108cf494aa63ad9dbeb3bc66b16ec31bf97c` | `49da3461...` |
| `TZ` + `LC_ALL` + `SOURCE_DATE_EPOCH` | 19,344,492 | `9b97108cf494aa63ad9dbeb3bc66b16ec31bf97c` | `49da3461...` |

There are no `__DATE__`/`__TIME__`/`__TIMESTAMP__` uses in `src/` or the vendored
sources, and clang 7.0.2 does not consume `SOURCE_DATE_EPOCH` for these inputs. The
build is environment-insensitive on this tree.

### 3.8 Link order

Same canonical objects, same link command, only the first two app object arguments
(`BackButton.c.obj` and `NFC.cpp.obj`) swapped.

| link order | unstripped size | build-id |
|---|---:|---|
| canonical | 19,367,052 | `287186a09650e8d72cbc363c7945cf800cdd65d6` |
| first two TUs swapped | 19,367,052 | `8f03f13b8daede2c64196d4d7103715c5c1a516b` |

The swap changes `.text` layout, `.symtab` order, and the dynamic-symbol first-mention
order. Link order is a first-class build-provenance input.

### 3.9 Archive timestamps

Same canonical objects, same link command; the three archives were copied to scratch.
In one variant the archive file mtimes were set to `now`; in the other every `ar`
member-header timestamp was patched to `now`.

| archive variant | unstripped size | build-id |
|---|---:|---|
| canonical archives | 19,367,052 | `287186a09650e8d72cbc363c7945cf800cdd65d6` |
| archive file mtimes changed | 19,367,052 | `287186a09650e8d72cbc363c7945cf800cdd65d6` |
| archive member-header mtimes changed | 19,367,052 | `287186a09650e8d72cbc363c7945cf800cdd65d6` |

Gold hashes the link output, not the input archives. Archive timestamps are
unobservable in the build-id.

## 4. Small brute force over likely Android Gradle paths

The APK survives only `Created-By: Android Gradle 3.1.2`; no `Android.mk`,
`Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`, or `*.ninja` survives.
AGP 3.1.2-era `externalNativeBuild` normally used the SDK CMake 3.6.4111459 or
3.10.2, with a build directory under `app/.externalNativeBuild/cmake/<buildType>/<abi>`
or `app/build/intermediates/cmake/<buildType>/obj/<abi>`.

A six-candidate sweep was run by rebuilding with
`-Xclang -fdebug-compilation-dir -Xclang <candidate>` while holding the source path,
build directory, CMake 3.31.6, and all other flags constant:

| candidate `comp_dir` | build-id |
|---|---|
| `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/app/.externalNativeBuild/cmake/release/armeabi-v7a` | `754bcaaf6df677c9504365822785944aa19ed501` |
| `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/app/build/intermediates/cmake/release/obj/armeabi-v7a` | `64f018bb49fb93ce8a0b1fb56e4b94691a2ce7f4` |
| `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/.externalNativeBuild/cmake/release/armeabi-v7a` | `e4026c5bbf1a6fa160f308b3b750db8bc0f930c3` |
| `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/app/.cxx/cmake/release/armeabi-v7a` | `8f57f7766da0710cbe99acf4fda14c61a9eb15d9` |
| `/Users/n.tran/projects/gof2/app/.externalNativeBuild/cmake/release/armeabi-v7a` | `3336330c8213b53974a05e1b717828e76ecd0f80` |
| `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/app/.externalNativeBuild/cmake/release/armeabi-v7a` | `92a0821f8888016e671778851df23e88d6dcf5f5` |

All six are distinct and none is `c011810d...`. **This is not evidence for or against
any candidate**: the current tree still has 19 non-exact functions, a 192-byte extra
`.rodata` table, and different source paths/TU contents, so every candidate is far
from the original unstripped output. The sweep only demonstrates that the candidate
space is small enough to test *after* the rest of the file is exact.

## 5. What must match for a future whole-`.so` hash match

Two different targets must be kept distinct:

### 5.1 Final stripped `.so` SHA-256

To make `sha256(libgof2hdaa.so)` equal
`d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`, the final
2,187,328-byte stripped file must match byte-for-byte:

* all 27 section headers and `.shstrtab` bytes/order;
* exact `.text` for all 4,524 functions, including the 19 non-exact bodies;
* exact `.ARM.exidx` and `.ARM.extab` bytes (not just sizes);
* exact `.rodata` (the current 192-byte mission table must be removed or otherwise
  made to match the original's `.rodata`);
* exact `.rel.plt` (currently 6 bytes differ);
* exact `.note.gnu.build-id` descriptor `c011810da45777b83ed721b2a87d517efed0d310`;
* exact file size and all other section bytes.

### 5.2 Genuine gold build-id

To make the build-id itself the genuine SHA-1 of the unstripped gold output, the
**entire unstripped file** must match the original's:

* all of §5.1, plus exact `.symtab`/`.strtab` (names, order, sizes, local symbols);
* exact `.debug_*` (DWARF version, producer, `DW_AT_comp_dir`, source paths, line
  tables, variable locations, pubnames/pubtypes, ranges, aranges);
* exact section-header table, program headers, file offsets, and padding;
* exact original source-tree path and build-directory path;
* exact debug flags (`-g2`-class, DWARF version, column-info choice, prefix maps);
* exact TU membership, object link order, archive order, and toolchain archive bytes
  (NDK r18b, darwin `libc++abi.a`);
* exact effective CMake compile/link commands.

Archive timestamps, `TZ`, `LC_ALL`, and `SOURCE_DATE_EPOCH` do **not** need to match.
CMake version does not need to match if it generates the same effective commands.
Compiler executable path does not need to match.

If only the final `.so` SHA-256 is required, the 20-byte build-id descriptor can be
copied from the original after every other stripped byte matches. That yields a
matching file hash but not a gold-verified build-id; it should be documented as a
synthetic note if ever done. For genuine provenance, the unstripped output must match.

## 6. Is the build-id recoverable independently of the remaining 19 functions?

**No, not genuinely.** The 19 functions are in `.text`, and their sizes/addresses are
also in `.symtab`; their line tables are in `.debug_line`; their EH records are in
`.ARM.exidx`/`.ARM.extab`. All of those bytes are in the SHA-1 input. The build-id is
all-or-nothing: a one-byte change anywhere in the ~19 MB unstripped file flips the
whole 160-bit value with no partial credit.

The 20-byte value is already known from the shipped note, so it can be copied
synthetically without solving the 19 functions. But that does not make the build-id
"recoverable" in the provenance sense: the copied value will not be the SHA-1 of the
new unstripped file. The 19 functions are necessary but not sufficient; the debug
info, symbol table, TU paths, and source/build paths must also match.

## 7. Proven / disproven / unobservable

### Proven by command

* Gold default `--build-id` = SHA-1 of the complete unstripped output with the 20-byte
  descriptor zeroed; `--build-id=sha1` is identical.
* The original `c011810d...` is not the SHA-1 of the shipped stripped file
  (`d7c05d...`); it is the hash of a larger unstripped output.
* Build-id changes with source path, build directory, `-g` level, DWARF version,
  `-fdebug-prefix-map`, `-fdebug-compilation-dir`, and link order.
* Build-id does not change with compiler executable path, archive file/member
  timestamps, or `TZ`/`LC_ALL`/`SOURCE_DATE_EPOCH` on this tree.
* CMake version itself is not hashed: 3.6.4111459, 3.20.6, 3.22.6 and 3.31.6 produce
  the same build-id when their effective commands are identical.
* CMake 3.6/3.10 ignore source `COMPILE_OPTIONS`, which changes the effective commands
  and therefore the build-id for the current tree.
* `-g`/`-g2`/`-g3` are identical; `-g0`/`-g1` change codegen and the stripped image.
* The original must be `-g2`-class from the gate, but the build-id cannot distinguish
  `-g2` from `-g3` or DWARF 2/3/4 among codegen-neutral choices.

### Disproven

* Build-id is not a hash of the shipped stripped file.
* Build-id is not sensitive to archive timestamps.
* Build-id is not sensitive to the compiler executable path.
* Build-id is not sensitive to `TZ`/`LC_ALL`/`SOURCE_DATE_EPOCH` here.
* CMake version is not directly embedded in the output.

### Unobservable from the shipped `.so`

* The original unstripped size and all `.debug_*` bytes.
* The original `.symtab`/`.strtab` names, order, and sizes.
* The original `DW_AT_comp_dir` and source-path spelling.
* The original build directory and source-tree root.
* The original CMake version (only effective commands matter).
* The original host beyond the already-known darwin `libc++abi` path; the build-id is
  a content hash, not a host fingerprint.
* The exact debug-flag choice among codegen-neutral variants.

## 8. Reproduction commands

Baseline:

```sh
cd /home/debian/GalaxyOnFire2/wt/w7008
mkdir -p /dev/shm/w7008/tmp
TMPDIR=/dev/shm/w7008/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w7008/tmp cmake --build cmake-build-match --target check -j8
```

For the variant builds, `/dev/shm/w7008/compat-src` is a copy of the worktree's
`CMakeLists.txt` with `src/`, `third_party/`, `tools/`, `cmake/`, and `.cache/`
symlinked back to the worktree, plus `cmake-real/{toolchain,DownloadNDK}.cmake`
copies of the tracked files. The only edits are (a) `cmake_minimum_required(VERSION
3.6)`, (b) explicit `${CMAKE_BINARY_DIR}/CMakeFiles/gof2.dir/./<src>.obj` object
paths instead of `$<TARGET_OBJECTS:gof2>` so CMake 3.6 can link, and (c)
`include_guard(GLOBAL)` commented out in the `cmake-real/` copies for CMake 3.6.
Because `src/` is a symlink, changing the *spelling* of the source root (e.g.
`/dev/shm/w7008/csrc2` as another symlink to the same tree) is what exercises
`DW_AT_name`/`comp_dir`; the compiled bytes are otherwise identical.

Preserve and hash the unstripped output:

```python
import subprocess, re, hashlib, struct
build = '/home/debian/GalaxyOnFire2/wt/w7008/cmake-build-match'
out = '/dev/shm/w7008/out/base.unstripped.so'
cmd = subprocess.check_output(['ninja','-C',build,'-t','commands','libgof2hdaa.so'],
                              text=True).strip().splitlines()[-1]
link = next(p for p in cmd.split(' && ') if 'ld.gold' in p)
link = re.sub(r'-o\s+\S*libgof2hdaa\.unstripped\.so', '-o '+out, link, count=1)
subprocess.run(link, shell=True, check=True, executable='/bin/bash')
d = bytearray(open(out,'rb').read())
# locate .note.gnu.build-id, zero descsz bytes at desc, SHA-1 whole file
```

Variant wrapper (replace only the `-g` token and append extra flags):

```cmake
# /dev/shm/w7008/toolchain-var.cmake
set(GOF2_DBG_FLAG "-g" CACHE STRING "debug flag")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra flags")
include("/dev/shm/w7008/compat-src/cmake-real/toolchain.cmake")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _cxx "${CMAKE_CXX_FLAGS}")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _c "${CMAKE_C_FLAGS}")
if(NOT GOF2_EXTRA_FLAGS STREQUAL "")
  string(APPEND _cxx " ${GOF2_EXTRA_FLAGS}")
  string(APPEND _c " ${GOF2_EXTRA_FLAGS}")
endif()
set(CMAKE_CXX_FLAGS "${_cxx}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${_c}"   CACHE STRING "" FORCE)
```

Relink variants (link-order swap, archive timestamps) use the same gold command with
only the output path, object order, or archive paths changed. The canonical objects
are in `cmake-build-match/CMakeFiles/gof2.dir/`; the three archives are
`third_party/{zlib,libzip,openssl}/*.a`.

## 9. Conclusion

The build-id is a strong **final** provenance check and a weak **search** signal. It
proves that the original was linked with gold `--build-id` without `-s` and then
stripped, and it commits to the exact original unstripped output. It cannot, by
itself, tell us the original build directory, source path, CMake version, or host.
Those remain unobservable until every other byte of the unstripped output is
reproduced; at that point a small candidate sweep over plausible paths is feasible and
the build-id becomes decisive.
