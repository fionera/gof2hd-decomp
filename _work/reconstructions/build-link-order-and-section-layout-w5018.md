# Link-flag and link-order archaeology: whole-`.so` deltas and the exact link recipe (w5018, wave 100)

Status: research note + reproducible scripts, 2026-11-04. Worktree `wt/w5018`, branch
`wave100-w5018`, base `44225192c`. Scratch: `/dev/shm/w5018`. **No `src/` or build-flag change is
landed**: the current CMake/gold recipe is independently re-derived and confirmed. The note is the
deliverable; the scripts under `_work/reconstructions/w5018/` are the reproducible evidence.

This note answers three questions from the shipped artifacts, not from the existing `CMakeLists.txt`:

1. **Is `tools/source_order.txt` the original object link order?** Yes — independently witnessed
   from five independent section-adjacency signals (`.text`, `.rodata`, `.data`, `.data.rel.ro`,
   `.bss`), plus `.ARM.exidx` entry order and `.rel.plt` first-reference order.
2. **Are the remaining whole-`.so` deltas link-order artifacts?** No. `.rel.plt` (12 bytes),
   `.ARM.extab` (-4 bytes), `.ARM.exidx` (1,637 bytes) and `.text` (101,683 bytes) are all
   **source-body** artifacts of the 24 non-byte-exact functions. The build-id (20 bytes) is
   unrecoverable because it is a SHA-1 of the original *unstripped* output.
3. **Which link flags did the original use?** The current recipe is exactly right. Every flag that
   is observable in the binary is proven by a positive A/B fingerprint; the rest are
   unobservable/neutral.

## 0. Bottom line

| delta | size | classification | cause |
|---|---:|---|---|
| `.text` | 101,683 differing bytes | **source-body** | 24 non-byte-exact functions |
| `.ARM.exidx` | 1,637 differing bytes | **source-body** | word0 (function prel31) identical in all 3566 entries; word1 (extab prel31) shifts with the LSDA deltas |
| `.ARM.extab` | `0x26298` vs `0x26294` (-4) | **source-body** | net of five per-function LSDA size deltas (+4 -8 +4 -8 +4) |
| `.rel.plt` | 12 differing bytes (6-slot rotation) | **source-body** | `LevelScript::process` body layout: `PlayerEgo::deleteHackingGame` is first-referenced 0x543c into the function instead of 0x61bc |
| `.note.gnu.build-id` | 20 differing bytes | **unrecoverable** | SHA-1 of the original unstripped gold output (includes `.symtab`/`.debug_*`) |
| everything else | 0 | — | 26 of 27 section headers are byte-identical; all other sections are byte-identical |

The object link order is **exactly right**. No link-order correction is needed.

## 1. Baseline (before and after this pass)

```sh
cd /home/debian/GalaxyOnFire2/wt/w5018
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section table (original vs ours; `diffB` is the number of differing bytes when the sizes match):

```text
section                 orig_size   our_size   orig_addr   our_addr    diffB
.note.android.ident            98         98        134        134        0
.note.gnu.build-id             24         24        1cc        1cc       20
.dynsym                     15ca0      15ca0        1f0        1f0        0
.dynstr                     2e318      2e318      15e90      15e90        0
.hash                        973c       973c      441a8      441a8        0
.gnu.version                 2b94       2b94      4d8e4      4d8e4        0
.gnu.version_d                 1c         1c      50478      50478        0
.gnu.version_r                 40         40      50494      50494        0
.rel.dyn                     7048       7048      504d4      504d4        0
.rel.plt                     72d0       72d0      5751c      5751c       12
.plt                         ac4c       ac4c      5e7ec      5e7ec        0
.text                      1337c8     1337c8      69440      69440   101683
.ARM.exidx                   6f70       6f70     19cc08     19cc08     1637
.ARM.extab                  26298      26294     1a3b78     1a3b78    size!
.rodata                     41b67      41b67     1c9e10     1c9e10        0
.fini_array                     8          8     20cba0     20cba0        0
.data.rel.ro                 2fe8       2fe8     20cbb0     20cbb0        0
.init_array                     c          c     20fb98     20fb98        0
.dynamic                      148        148     20fba4     20fba4        0
.got                         430c       430c     20fcf4     20fcf4        0
.data                        296c       296c     214000     214000        0
.bss                         69ac       69ac     216970     216970        0 (NOBITS)
.comment                      12f        12f          0          0        0
.note.gnu.gold-version         1c         1c          0          0        0
.ARM.attributes                38         38          0          0        0
.shstrtab                     117        117          0          0        0
```

`.comment` is byte-identical: `Android (4751641 based on r328903) clang version 7.0.2 ... GCC: (GNU)
4.9.x 20150123 (prerelease)`. `.note.android.ident` is byte-identical (`Android`, `r18b`,
`5063045`). `.note.gnu.gold-version` is byte-identical (`gold 1.12`). Program headers are
identical: `GNU_STACK` flags 6 (RW, no X), `GNU_RELRO` `0x20cba0..0x214000`, `PT_LOAD` split
identical. `.dynamic` is byte-identical: `DT_FLAGS=0x8` (`DF_BIND_NOW`), `DT_FLAGS_1=0x1`
(`DF_1_NOW`), `DT_RELCOUNT=0x48a`, and the same 10 `NEEDED` entries in the same order.

## 2. Independent link-order reconstruction

The build's `tools/source_order.txt` has 192 TUs. The original was linked without a
`--section-ordering-file`, so gold emits each object's sections in input order. Five independent
witnesses recover that order from the shipped `.so` alone:

### 2.1 Method

For each relocatable object (`cmake-build-match/CMakeFiles/gof2.dir/./<src>.obj`), take every
defined strong symbol that also appears in the original `.dynsym`, look up its original address,
and sort objects by the lowest such address in the section. The resulting object sequence must
equal `tools/source_order.txt` filtered to the objects with a witness in that section.

```sh
python3 _work/reconstructions/w5018/order_witness.py . \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match
```

```text
.text: 190 objects  order_match=True  ratio=1.000000
.rodata: 78 objects  order_match=True  ratio=1.000000
.data: 59 objects  order_match=True  ratio=1.000000
.data.rel.ro: 78 objects  order_match=True  ratio=1.000000
.bss: 23 objects  order_match=True  ratio=1.000000
```

`Array.cpp` and `ArrayInstantiations.cpp` have no exported strong function/data symbol, so they
have no witness; they are placed by their `.text` contribution in the same relative position.

For `.rodata`/`.data`/`.data.rel.ro`/`.bss` the witness is stronger than a sort: each object's
symbols form one **contiguous block**, and the blocks appear in exactly `source_order.txt` order
(verified by `data_adjacency_witness.py`; the committed `order_witness.py` is the reduced form).

### 2.2 `.ARM.exidx`

Each `.ARM.exidx` entry's word0 is a prel31 pointer to the function it covers. Map those function
addresses to the defining object and check the object sequence and per-object address monotonicity:

```sh
python3 _work/reconstructions/w5018/exidx_witness.py . \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match
```

```text
exidx entries mapped 2566  objects 190  object_order_matches_source_order True  objects_with_nonascending_addrs 0
```

The section has 3566 entries in both binaries: 272 `CANTUNWIND`, 856 inline, 2438 extab. The
object sequence is exact and every object's entries are in ascending function-address order.

### 2.3 `.rel.plt`

Gold assigns PLT slots in first-reference order. Walk the objects in `source_order.txt` order,
collecting `PC24`/`THM_CALL`/`CALL`/`JUMP24`/`THM_JUMP24` relocations in section order, and record
the first object that references each symbol:

```sh
python3 _work/reconstructions/w5018/relplt_witness.py . \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so cmake-build-match
```

```text
first-reference object sequence matches source_order: True
```

### 2.4 Conclusion

`tools/source_order.txt` is independently confirmed by five section witnesses plus the exidx and
rel.plt first-reference witnesses. It is also consistent with the original source-layout dumps
(`_work/original_layout/tu_mapping.md`): each object is a witnessed original TU, and the order is
the one recovered from the original `.dynsym` by `tools/gen_link_order.py`; the witnesses above
confirm that order without consulting the original `.so`'s symbol table. It is the original object
order. **No link-order change is needed.**

## 3. The whole-`.so` deltas are source-side

### 3.1 `.rel.plt`: a 6-slot rotation inside `LevelScript::process`

Exactly six `.rel.plt` slots differ (a single rotation); every other slot is identical:

| slot | original | ours |
|---:|---|---|
| 2831 | `StarSystem::switchPlanetForIntro` | `PlayerEgo::deleteHackingGame` |
| 2832 | `Level::switchSkyboxForIntro` | `StarSystem::switchPlanetForIntro` |
| 2833 | `ParticleSystemManager::reset` | `Level::switchSkyboxForIntro` |
| 2834 | `PlayerEgo::startSmokeEmission` | `ParticleSystemManager::reset` |
| 2835 | `PlayerStation::setVisible` | `PlayerEgo::startSmokeEmission` |
| 2836 | `PlayerEgo::deleteHackingGame` | `PlayerStation::setVisible` |

All six symbols are first-referenced in **one TU**, `src/game/world/LevelScript.cpp` (object #136
in `tools/source_order.txt`), and all six first references are inside
`LevelScript::process` (`0x137d18`, size `0xdf90` in both). The function is size-exact but only
54.8% byte-exact. The first-reference offsets (function-relative) are:

| symbol | original | ours |
|---|---:|---:|
| `StarSystem::switchPlanetForIntro` | `0x54d4` (`0x13d1ec`) | `0x61b2` (`0x13deca`) |
| `Level::switchSkyboxForIntro` | `0x54dc` (`0x13d1f4`) | `0x61ba` (`0x13ded2`) |
| `ParticleSystemManager::reset` | `0x54fa` (`0x13d212`) | `0x61d8` (`0x13def0`) |
| `PlayerEgo::startSmokeEmission` | `0x567e` (`0x13d396`) | `0x6464` (`0x13e17c`) |
| `PlayerStation::setVisible` | `0x5d06` (`0x13da1e`) | `0x677e` (`0x13e496`) |
| `PlayerEgo::deleteHackingGame` | `0x61bc` (`0x13ded4`) | `0x543c` (`0x13d154`) |

So the case-40 body (which contains the first five symbols) and the case-125 state>=4 hacking
block (which contains `deleteHackingGame`) are laid out in the opposite order. In the original the
hacking block is at `0x61bc`, **after** `PlayerStation::setVisible` at `0x5d06`; in ours it is at
`0x543c`, **before** `switchPlanetForIntro` at `0x61b2`.

**This is not link order.** It is the intra-function block layout of `LevelScript::process`. The
exact recipe for the LevelScript worker:

> Move the first reference of `PlayerEgo::deleteHackingGame` from function offset `0x543c` to
> `0x61bc` (after `PlayerStation::setVisible` at `0x5d06`). Equivalently, reproduce the original
> block layout of `LevelScript::process` so the case-125 state>=4 hacking block is emitted after
> the case-40 body. When that happens, the six-slot `.rel.plt` rotation disappears and `.rel.plt`
> becomes byte-identical.

**Tested negative result (important):** swapping the two contiguous source blocks
(`case 125` lines 3510-3637 and `case 40` lines 3638-3705) does **not** change the code at all.
The object's `.text._ZN11LevelScript7processEi` (57232 bytes) and
`.rel.text._ZN11LevelScript7processEi` (21096 bytes) are byte-identical before and after the swap;
only `.debug_line`/`.debug_info` change (and therefore the build-id). So the difference is a CFG
difference, not a source-order difference. The LevelScript worker must find the source shape that
moves the block (for example, a different `if`/`else if` nesting or a different `&&` chain), not
just reorder the `case` labels.

### 3.2 `.ARM.extab` -4: five per-function LSDA deltas

Parse the exidx→extab pointers, sort the unique extab record addresses, and take successive
differences as record sizes. Comparing the record owned by each function gives exactly five
differing records, and their deltas sum to the whole-section delta:

```sh
python3 _work/reconstructions/w5018/extab_delta.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

```text
function                                                       orig_off  orig_sz   our_sz  delta
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE    0x1a78c4      172      176     +4
_ZN5Level10createShipEiiiP8Waypointbb                          0x1ab09c      336      328     -8
_ZN11LevelScript7processEi                                     0x1c2af8      348      352     +4
_ZN12HangarWindow6renderEv                                     0x1c30fc      948      940     -8
_ZN3Hud4drawExxP9PlayerEgobjj                                  0x1c57cc      476      480     +4
net                                                                                           -4
.ARM.extab size original 0x26298  ours 0x26294  delta -4
```

All five functions are non-byte-exact (`Generator::createMission` 74.3%, `Level::createShip`
74.4%, `LevelScript::process` 54.8%, `HangarWindow::render` 80.8%, `Hud::draw` 91.7%). The -4 is
the net of the five LSDA size changes, not a link-order artifact.

### 3.3 `.ARM.exidx` 1,637 bytes: downstream of the LSDA deltas

In all 3566 exidx entries, **word0 (the prel31 to the function) is identical**; only word1 (the
prel31 to the extab record, or the inline bit) differs, in 1602 entries. The raw byte delta is
1637 because the small prel31 deltas change one or two bytes per entry. This is entirely explained
by the five LSDA records shifting the extab targets. Not link order.

### 3.4 `.note.gnu.build-id`: SHA-1 of the unstripped output

See §4. The original's 20-byte build-id `c011810da45777b83ed721b2a87d517efed0d310` is the SHA-1 of
the original *unstripped* gold output (which no longer exists), so it cannot be recovered from the
stripped artifact. It is expected to remain 20 differing bytes.

### 3.5 `.text`: the 24 non-byte-exact functions

The 101,683 differing `.text` bytes are the 24 non-byte-exact function bodies (the gate reports
byte_exact 4500/4524). This is source-body work, not build archaeology.

## 4. `--build-id` algorithm (proven)

Gold's `--build-id` (default = `--build-id=sha1`) writes a 4+4+4+4-byte note (`.note.gnu.build-id`,
size `0x24`) whose 20-byte descriptor is the SHA-1 of the whole output file **with the 20-byte
descriptor zeroed**, taken over the **unstripped** gold output (including `.symtab`, `.strtab`,
`.debug_*`). Proof:

```sh
python3 _work/reconstructions/w5018/buildid_proof.py \
  /dev/shm/w5018/variants/base.unstripped.so \
  /dev/shm/w5018/variants/swapped.unstripped.so \
  /dev/shm/w5018/base2.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
base.unstripped.so     build-id b2a066295b75e6e0506a5df3708a23416ab4654e
                       sha1(zero desc) b2a066295b75e6e0506a5df3708a23416ab4654e  match True
swapped.unstripped.so  build-id 20b0b26af0cda94bd41772f8ad592b5a51dd3b91
                       sha1(zero desc) 20b0b26af0cda94bd41772f8ad592b5a51dd3b91  match True
base2.so (stripped)    build-id b2a066295b75e6e0506a5df3708a23416ab4654e
                       sha1(zero desc) 34f404c0fc0005c8426742dc8ef79d37e755c4b4  match False
original (stripped)    build-id c011810da45777b83ed721b2a87d517efed0d310
                       sha1(zero desc) d7c05dc5e9804023d8786147addc48537b0bee6d  match False
```

The decisive experiment: swap the two `LevelScript.cpp` case blocks (which changes only the
debug line table, not the machine code). The stripped `.so` is byte-identical in every section
**except** `.note.gnu.build-id` (20 bytes):

```text
stripped section diffs base2 vs swapped2:
   .note.gnu.build-id 0x24 diffbytes 20
```

The unstripped outputs differ only in `.note.gnu.build-id`, `.debug_info` (310 bytes) and
`.debug_line` (462637 bytes). Both unstripped build-ids equal SHA-1(zeroed descriptor). This
proves the hash covers the debug sections, i.e. it is taken **before** the strip.

`--build-id=md5` and `--build-id=uuid` produce a 16-byte descriptor (note size `0x20`), and
`--build-id=none` removes the note; the original's note is `0x24` with `descsz=20`, so the
algorithm is SHA-1. `--build-id` and `--build-id=sha1` produce byte-identical output.

## 5. Post-link strip normalization (proven)

The shipped `.so` has a **bfd-normalized** section header table but **gold's Stringpool
`.shstrtab` order**. Neither `gold -s` alone nor `gold + bfd strip` (without reorder) reproduces
that combination:

```sh
python3 _work/reconstructions/w5018/strip_proof.py . cmake-build-match \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
gold -s                          shstrtab SAME  hdrs DIFF
    .rel.plt:flags=42/2,info=14/0
    .fini_array:entsize=4/0
    .init_array:entsize=4/0
    .bss:offset=21596c/215970
gold -s + bfd strip              shstrtab SAME  hdrs DIFF
    .ARM.extab:size=26298/26294
gold + bfd strip                 shstrtab DIFF  hdrs DIFF   (bfd rewrote .shstrtab into section-header order)
gold + bfd strip + reorder       shstrtab SAME  hdrs DIFF
    .ARM.extab:size=26298/26294
```

`gold -s` leaves the `.rel.plt` `SHF_INFO_LINK`/`sh_info`, the `INIT_ARRAY`/`FINI_ARRAY`
`sh_entsize=4`, and the unaligned NOBITS `.bss` `sh_offset` wrong. The NDK's
`arm-linux-androideabi-strip --strip-all` fixes all of them. It also rewrites `.shstrtab` into
section-header order when it removes the debug sections; `tools/reorder_shstrtab.py` restores
gold's Stringpool order from the unstripped output. The current pipeline
(`gold` no `-s` → `arm-linux-androideabi-strip --strip-all` → `reorder_shstrtab.py`) is the
unique one that reproduces every header except the known `.ARM.extab` size.

An interesting equivalence: `gold -s` → bfd strip and `gold` (no `-s`) → bfd strip → reorder
produce **byte-identical output except the build-id** (verified section-by-section). So `-s` on
the gold link is unobservable from the stripped artifact; only the build-id differs, and the
original's unstripped output is unavailable. The current no-`-s` choice is correct for the
build-id semantics and is kept.

## 6. Flag classification

The A/B harness relinks the exact current command with one mutation at a time, strips, reorders,
and compares every section, `NEEDED`, exports and imports against the original. A mutation that
produces byte-identical output is UNOBSERVABLE; one that changes output is a candidate original
flag (compare with the original).

```sh
python3 _work/reconstructions/w5018/flag_matrix.py . cmake-build-match \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

Base output sha256 prefix `045e6798e294988d`; original `d4c17be61af96272` (the difference is the
24 source-body/build-id deltas above).

### PROVEN (mutation changes the output; current value matches the original)

| flag | evidence |
|---|---|
| `--build-id` (= sha1) | 20-byte descriptor; `md5`/`uuid` → 16-byte note `0x20`, `none` → no note; `--build-id=sha1` == base |
| `-z now` | `-z lazy` changes `.text` 115075, `.dynamic` size, `.rel.plt` 7360, `.plt`, `.got`, `.data.rel.ro`; original has `DT_FLAGS=0x8`, `DT_FLAGS_1=0x1`, `.got` before `.data` in `PT_GNU_RELRO` |
| `-z relro` | `-z norelro` changes `.text` 115481, `.data.rel.ro`, `.got`, `.dynamic`; original has `PT_GNU_RELRO 0x20cba0..0x214000` |
| `-z noexecstack` | `-z execstack` changes only `PT_GNU_STACK` flags (RW → RWX); `-z noexecstack` == base (default); original `GNU_STACK` flags 6 |
| `--exclude-libs=libgcc.a` | removing it adds 3 exports (5373 vs 5370) and changes `.text` +0x10, dynsym/dynstr/hash/rel.plt/plt/got sizes; `ALL` (4916 exports) and `libc++abi.a` (5071) are wrong |
| `--whole-archive zlib libzip` | no whole-archive: exports 5281, imports 197, `.text 0x12c208`; zlib whole / libzip normal: 5333/193; no libzip: 5293/184; no zlib: 5318/215; libcrypto whole-archive is neutral (all its members are already pulled) |
| `-l` order: `log GLESv2 GLESv1_CM EGL android`, then FMOD, then `m`, `libc++abi.a`, `libandroid_support.a`, `libgcc.a`, `dl`, `c` | NEEDED order is exactly `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc`; moving libfmodex after libc++abi changes NEEDED (`.dynsym` 20518, `.rel.plt` 2954); moving libfmodex after libgcc changes NEEDED (`.dynsym` 33103, `.rel.plt` 4271); libandroid_support before libc++abi → `.text 0x133788`, exidx 3564; no libandroid_support → exidx 3564; no libgcc → imports 206, exidx 3564; no libcrypto → exports 5347, imports 208; swap zlib/libzip → `.text` 138622, `.data` 0x2978 |
| `--hash-style=sysv` | `gnu`/`both` add `.gnu.hash` and change dynsym/dynstr/rel.plt; `sysv` == base; original has `.hash` and no `.gnu.hash` |
| `-z combreloc` (default) | `-z nocombreloc` changes `.rel.dyn` 17753, dynsym 305, `.text` 101800 |
| `--no-as-needed` (default) | `--as-needed` drops `libGLESv1_CM`, `libEGL`, `libandroid`, `libdl` from NEEDED; `--no-as-needed` == base |
| `-soname libgof2hdaa.so` | `--no-soname` changes dynstr/dynamic |
| no `--gc-sections` | `--gc-sections` changes `.text 0x133794`, exidx `0x6690`, rodata/data/bss/dynsym |
| no `--sort-section` | `--sort-section=name` changes `.text 0x13380c`, rodata `0x41b50`, data.rel.ro `0x2fe0`, bss `0x69b4`, exidx `0x68b0`; `alignment` fails to link |
| no `--rosegment` | `--rosegment` changes section addresses/order massively |
| linker `-O1` (neutral) / `-O2` (wrong) | `-O1` == base; `-O2` changes `.text` 170626, rodata size, dynsym, `.plt`, `.got` |

### UNOBSERVABLE (mutation produces byte-identical output)

`--no-undefined`, `--allow-multiple-definition`, `--unresolved-symbols=ignore-all`,
`--fix-cortex-a8`, `--warn-shared-textrel`, `-z text`, `-z notext`, `-z muldefs`,
`--no-copy-dt-needed-entries`, `--no-add-needed`, `--no-allow-shlib-undefined`,
`--no-keep-memory`, `-O1`, `-z noexecstack` (default), `--hash-style=sysv` (default),
`--no-as-needed` (default), `libgcc.a` before `libc++abi.a` (the archive is rescanned; the pulled
members and their order are unchanged), `--whole-archive` around `libcrypto_gof2.a` (all its
members are already pulled by the normal scan), and `-s` on the gold link (with the bfd strip;
only the build-id changes).

### DISPROVEN (mutation changes output away from the original)

`-z lazy`, `-z norelro`, `-z nocombreloc`, `--as-needed`, `--exclude-libs=ALL`,
`--exclude-libs=libc++abi.a`, no `--exclude-libs`, no `--whole-archive`, zlib whole / libzip
normal, no libzip, no zlib, zlib/libzip swapped, FMOD/libc++abi swapped, FMOD after libc++abi,
FMOD after libgcc, libandroid_support before libc++abi, remove libgcc, remove libandroid_support,
remove libcrypto,
`--hash-style=gnu`, `--hash-style=both`, `--sort-section=name`, `--sort-section=alignment`
(link fail), `--gc-sections`, `--rosegment`, `-O2`, `--build-id=md5`, `--build-id=uuid`,
`--build-id=none`, `--no-soname`, `--emit-relocs` (link fail).

## 7. The exact link line

```text
<NDK>/arm-linux-androideabi/bin/ld.gold -shared -soname libgof2hdaa.so --build-id -z now
  --exclude-libs=libgcc.a -o libgof2hdaa.unstripped.so
  <NDK>/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o -L <NDK>/platforms/android-16/arch-arm/usr/lib
  <192 gof2 objects in tools/source_order.txt order>
  --whole-archive third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a --no-whole-archive
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
  <APK>/libfmodex.so <APK>/libfmodevent.so
  third_party/openssl/libcrypto_gof2.a
  -l m
  <NDK>/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a
  <NDK>/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a
  <NDK>/.../armv7-a/thumb/libgcc.a
  -l dl -l c
  <NDK>/platforms/android-16/arch-arm/usr/lib/crtend_so.o
&& <NDK>/arm-linux-androideabi-strip --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
&& python3 tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so
```

No `-s`, no `--section-ordering-file`, no `--version-script`, no second link pass, no
`--unresolved-symbols`, no `--allow-multiple-definition`, no `--emit-relocs`.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5018
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6

ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
python3 _work/reconstructions/w5018/order_witness.py . "$ORIG" cmake-build-match
python3 _work/reconstructions/w5018/exidx_witness.py . "$ORIG" cmake-build-match
python3 _work/reconstructions/w5018/relplt_witness.py . "$ORIG" cmake-build-match/libgof2hdaa.so cmake-build-match
python3 _work/reconstructions/w5018/extab_delta.py "$ORIG" cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/w5018/strip_proof.py . cmake-build-match "$ORIG"
python3 _work/reconstructions/w5018/flag_matrix.py . cmake-build-match "$ORIG"
```

The build-id proof needs the unstripped gold output. Reproduce it with:

```sh
cd cmake-build-match
ninja -t commands libgof2hdaa.so | tail -1 > /dev/shm/w5018/link_cmd.txt
# edit the -o path to .../base.unstripped.so, run the gold part, then:
python3 _work/reconstructions/w5018/buildid_proof.py /dev/shm/w5018/base.unstripped.so
```

## 9. Handoff to the LevelScript worker

The only actionable whole-`.so` fix is the `.rel.plt` rotation. It is source-side and belongs to
the `LevelScript::process` reconstruction:

- Target: first reference of `PlayerEgo::deleteHackingGame` at function offset `0x61bc`
  (absolute `0x13ded4`), after `PlayerStation::setVisible` at `0x5d06` (`0x13da1e`).
- Current: `0x543c` (`0x13d154`), before `StarSystem::switchPlanetForIntro` at `0x61b2`
  (`0x13deca`).
- The six-slot rotation disappears automatically when that block moves; no link/build change is
  needed.
- Swapping the `case 125` and `case 40` source blocks does **not** move it (the object `.text` is
  byte-identical); the worker must reproduce the original CFG/block layout, not just reorder the
  `case` labels.

When the function becomes byte-exact, `.rel.plt` should become byte-identical and the gate's
byte_exact/linked_exact counts should rise by one (the function is size-exact, so no address
shift is expected).

## 10. Files

- `_work/reconstructions/build-link-order-and-section-layout-w5018.md` — this note.
- `_work/reconstructions/w5018/order_witness.py` — `.text`/`.rodata`/`.data`/`.data.rel.ro`/`.bss`
  object-order witnesses.
- `_work/reconstructions/w5018/data_adjacency_witness.py` — per-object data-block contiguity and
  adjacency witness.
- `_work/reconstructions/w5018/exidx_witness.py` — `.ARM.exidx` object-order witness.
- `_work/reconstructions/w5018/relplt_witness.py` — `.rel.plt` first-reference witness + rotation
  table + call-site table.
- `_work/reconstructions/w5018/extab_delta.py` — per-function LSDA size deltas.
- `_work/reconstructions/w5018/buildid_proof.py` — build-id = SHA-1(unstripped, descriptor zeroed).
- `_work/reconstructions/w5018/strip_proof.py` — post-link strip normalization.
- `_work/reconstructions/w5018/flag_matrix.py` — link-flag A/B matrix.
