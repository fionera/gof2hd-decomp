# Linker provenance: gold 1.12, default flags, and the section-layout matrix (w8622)

Status: **note-only research commit**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8622`, base `e18e0c2a4` ("Wave 140 docs:
.text 86,472, .ARM.extab 1,411, semantic wins"). Scratch: `/dev/shm/w8622`.
No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is
landed. This note is the only commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas of the canonical build (full byte comparison, all 27 section
headers):

| section | size | base | differing bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | `0x69440` | **86,472** |
| `.ARM.extab` | 156,312 | `0x1a3b78` | **1,411** |
| `.note.gnu.build-id` | 36 | `0x1cc` | **20** |
| `.plt`, `.rel.plt`, `.hash`, `.dynsym`, `.dynstr`, `.got`, `.ARM.exidx`, `.rodata`, `.data`, `.bss`, `.shstrtab`, `.comment`, `.note.gnu.gold-version`, `.ARM.attributes` | exact | exact | **0** |

This pass re-confirms the linker recipe and adds a 30-cell linker-flag /
linker-identity matrix. **No variant raises `byte_exact` above 4507, and no
variant improves any section size/base.** The current recipe is exactly the
original's.

---

## 0. Bottom line

1. **The original is GNU gold 1.12, and the binary says so.** Its
   `.note.gnu.gold-version` section is byte-identical to ours:
   `04000000 09000000 04000000 474e5500 676f6c64 20312e31 32000000`
   = `"gold 1.12"`. Only gold emits this section; bfd and lld do not. The
   NDK r18b `ld.gold --version` is `GNU gold (GNU Binutils 2.27.0.20170315)
   1.12`, i.e. the same linker build.
2. **Every linker-controlled section is already byte-identical**, not merely
   size/base-exact: `.plt` (0 of 44,108 bytes), `.rel.plt` (0 of 29,392),
   `.hash` (0 of 38,716), `.dynsym` (0 of 89,248), `.dynstr` (0 of 189,208),
   `.got` (0 of 17,164), `.ARM.exidx` (0 of 28,528), `.shstrtab` (0 of 279),
   `.comment` (0 of 303), `.note.gnu.gold-version` (0 of 28) and
   `.ARM.attributes` (0 of 56). All 27 section headers and all 8 program
   headers are identical.
3. **The only remaining differences are compiler output**: `.text`
   (86,472 bytes in 17 bodies) and `.ARM.extab` (1,411 bytes), plus the
   build-id hash that is a consequence of those (20 bytes). There is no
   linker-side residual left to find.
4. **The 1,411 `.ARM.extab` bytes are 99.5 % downstream of the 17 body
   residuals.** With the correct owner mapping (see §4), 10 of the 11
   differing LSDA records belong to non-byte-exact bodies; the 11th
   (`Radar::Radar` C2, 7 bytes) is the only byte-exact body with an LSDA
   delta. This corrects the w7620 note, whose owner mapping was off by one
   because `.dynsym` `st_value` carries the Thumb bit while `.ARM.exidx`
   function addresses are even.
5. **The gold defaults are load-bearing and already reproduced**:
   `--text-reorder` on, `--merge-exidx-entries` on, `--hash-style=sysv`,
   `--build-id` (= sha1), `-z now`, `-z noexecstack`, 4 KiB max page size,
   no `--rosegment`, no ICF, no `--gc-sections`, no `--sort-section`.
   Every departure from those defaults changes bytes.

---

## 1. Discriminating evidence for gold 1.12

### 1.1 `.note.gnu.gold-version` (the identity proof)

```
0000 04000000 09000000 04000000 474e5500  ............GNU.
0010 676f6c64 20312e31 32000000           gold 1.12...
```

Byte-identical in the original and our build. The note is emitted by gold
only; GNU bfd and lld do not emit it (the bfd/lld variants below have no such
section). This alone pins the linker family and version.

### 1.2 Program headers are identical

`readelf -lW` on the original and our build produces the same 8 headers:

```
PHDR      0x000034 0x00000034 0x00000034 0x00100 0x00100 R   0x4
LOAD      0x000000 0x00000000 0x00000000 0x20b977 0x20b977 R E 0x1000
LOAD      0x20bba0 0x0020cba0 0x0020cba0 0x09dcc 0x1077c RW  0x1000
DYNAMIC   0x20eba4 0x0020fba4 0x0020fba4 0x00148 0x00148 RW  0x4
NOTE      0x000134 0x00000134 0x00000134 0x000bc 0x000bc R   0x4
GNU_STACK 0x000000 0x00000000 0x00000000 0x00000 0x00000 RW  0x10
EXIDX     0x19cc08 0x0019cc08 0x0019cc08 0x06f70 0x06f70 R   0x4
GNU_RELRO 0x20bba0 0x0020cba0 0x0020cba0 0x07460 0x07460 RW  0x10
```

Two `PT_LOAD`s (R E and RW), one `PT_GNU_RELRO` covering
`.fini_array .data.rel.ro .init_array .dynamic .got`, `PT_GNU_STACK` RW
(non-executable), 4 KiB page alignment. This rules out `--rosegment`,
`-z execstack`, `-z norelro`, and any non-default page size.

### 1.3 Linker-controlled section content is exact

| section | size | original base | ours base | differing bytes |
|---|---:|---:|---:|---:|
| `.plt` | 44,108 | `0x5e7ec` | `0x5e7ec` | **0** |
| `.rel.plt` | 29,392 | `0x5751c` | `0x5751c` | **0** |
| `.hash` | 38,716 | `0x441a8` | `0x441a8` | **0** |
| `.dynsym` | 89,248 | `0x1f0` | `0x1f0` | **0** |
| `.dynstr` | 189,208 | `0x15e90` | `0x15e90` | **0** |
| `.got` | 17,164 | `0x20fcf4` | `0x20fcf4` | **0** |
| `.ARM.exidx` | 28,528 | `0x19cc08` | `0x19cc08` | **0** |
| `.shstrtab` | 279 | 0 | 0 | **0** |
| `.comment` | 303 | 0 | 0 | **0** |
| `.note.gnu.gold-version` | 28 | 0 | 0 | **0** |
| `.ARM.attributes` | 56 | 0 | 0 | **0** |

Because `.plt`, `.rel.plt`, `.hash`, `.dynsym`, `.dynstr`, `.got` and
`.ARM.exidx` are *byte-identical*, the PLT layout, PLT entry order, sysv hash
bucket order, symbol insertion order, GOT slot order and exidx
sorting/merging are all exactly reproduced. There is no room for a different
linker algorithm to hide.

---

## 2. Matrix: 30 linker/strip variants

All variants use the canonical objects (`cmake-build-match/CMakeFiles/gof2.dir`)
and the canonical link line, with only the linker or flags changed. `byte` is
`byte_exact` from `tools/verify/verify.py`; `size` and `diff` are the section
size and same-address differing bytes vs the original. A dash means
unchanged/exact. All variants keep the 4,524 function symbols and sizes
(`size 4524/4524`).

| variant | byte | `.text` size/diff | `.ARM.extab` diff | `.ARM.exidx` size/diff | `.plt` diff | `.rel.plt` diff | `.hash` diff | `.dynsym` diff | `.got` diff | extra |
|---|---:|---|---:|---|---:|---:|---:|---:|---:|---|
| **canonical** | **4507** | `0x1337c8` / 86,472 | 1,411 | `0x6f70` / 0 | 0 | 0 | 0 | 0 | 0 | — |
| gold `-O1` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | — |
| gold `-O2` | 1720 | `0x1337c8` / 156,266 | 2,019 | same / 0 | 7,086 | 0 | 0 | 24,132 | 3,674 | — |
| gold `-O3` | 1720 | same as `-O2` | 2,019 | same | 7,086 | 0 | 0 | 24,132 | 3,674 | — |
| `--icf=safe` | 1347 | `0x130964` / 1,207,898 | 2,703 | `0x59a0` / 21,541 | 3,676 | 7,348 | 0 | 10,681 | 0 | — |
| `--icf=all` | 1347 | `0x12f99c` / 1,208,007 | 2,748 | `0x5760` / 21,845 | 3,676 | 7,348 | 0 | 10,923 | 0 | — |
| `--gc-sections` | 2177 | `0x133794` / 947,182 | 2,619 | `0x6690` / 19,461 | 0 | 0 | 0 | 3,211 | 0 | — |
| `--hash-style=sysv` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | — |
| `--hash-style=both` | 1750 | `0x1337c8` / 140,325 | 2,014 | same / 0 | 10,121 | 10,837 | 13,489 | 45,558 | 11,022 | `.gnu.hash` |
| `--hash-style=gnu` | 1750 | `0x1337c8` / 146,785 | 2,020 | same / 0 | 8,129 | 10,837 | missing | 41,774 | 7,348 | `.gnu.hash` |
| `-z lazy` (instead of `-z now`) | 2932 | `0x1337c8` / 100,031 | 1,411 | same / 0 | 11,024 | 7,348 | 0 | 1,328 | 0 | — |
| `-z relro` (instead of `-z now`) | 2932 | same as `-z lazy` | 1,411 | same | 11,024 | 7,348 | 0 | 1,328 | 0 | — |
| `-z norelro` | 2932 | `0x1337c8` / 99,773 | 1,411 | same / 0 | 11,024 | 7,348 | 0 | 7,211 | 3,674 | — |
| `--rosegment` | 1720 | `0x1337c8` / 166,543 | 3,202 | same / 10,698 | 14,699 | 3,674 | 0 | 19,126 | 11,022 | 3rd `PT_LOAD` |
| `--build-id=sha1` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | — |
| `--build-id=md5` | 1971 | `0x1337c8` / 133,688 | 2,019 | same / 0 | 3,675 | 0 | 0 | 0 | 3,674 | note 32 B |
| `--build-id=none` | 1750 | `0x1337c8` / 140,742 | 2,019 | same / 0 | 4,163 | 0 | 0 | 10,669 | 3,674 | no note |
| `--sort-section=name` | 1342 | `0x13380c` / 1,230,342 | 146,114 | `0x68b0` / 21,222 | 0 | 0 | 0 | 14,060 | 0 | — |
| `--sort-section=name --no-text-reorder` | 1342 | same as `--sort-section=name` | 146,114 | same | 0 | 0 | 0 | 14,060 | 0 | — |
| `--no-text-reorder` | 1543 | `0x1337c8` / 1,103,407 | 1,411 | same / 11,425 | 0 | 0 | 0 | 6,362 | 0 | — |
| `--merge-exidx-entries` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | — |
| `--no-merge-exidx-entries` | 2878 | `0x1337c8` / 103,603 | 2,688 | `0x95c0` / 30,012 | 3,675 | 3,674 | 0 | 1,290 | 0 | — |
| `-z execstack` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | `GNU_STACK` RWE |
| `-z noexecstack` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | — |
| `-z max-page-size=0x1000` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | — |
| strip `--strip-unneeded` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | — |
| strip `--strip-all -R .comment` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | no `.comment` |
| llvm-objcopy `--strip-all` | 4507 | same | 1,411 | same | 0 | 0 | 0 | 0 | 0 | no `.comment`/gold-version/attributes, `.rel.plt` flags/info + init/fini entsize wrong |
| **bfd** `arm-linux-androideabi-ld.bfd` | 1345 | `0x131a58` / 262,032 | 3,202 | `0x6698` / 24,489 | 32,556 | 15,933 | 12,886 | 45,694 | 10,809 | `.plt` entsize 4, no gold-version |
| **lld** `ld.lld` (LLD 7.0.2) | 1345 | `0x13303c` / 1,161,017 | 6,373 | `0x66a0` / 24,277 | 41,524 | 16,519 | 20,370 | 45,673 | 14,708 | `.gnu.hash`, `.got.plt` |

Notes on the matrix:

* **The neutral variants are byte-identical whole files.** `-O1`, `--hash-style=sysv`, `--build-id=sha1`, `--merge-exidx-entries`, `-z noexecstack`, `-z max-page-size=0x1000` and `strip --strip-unneeded` all produce the same 2,187,328-byte file as the canonical build (sha256 `08f510621e4d0319e5d94b0d8e3fd143312a80ae64ddb48bc6536d6be8348919`). `-z execstack` changes only `PT_GNU_STACK` (RWE), not a section byte.
* `-O1` is exactly neutral; `-O2`/`-O3` enable gold's string merging and ICF
  pass, shrinking `.dynstr` by `0xe4` and `.rodata` by `0x200` and changing
  `.plt`/`.got`. The original's `.dynstr`/`.rodata` sizes are the `-O1`/`-O0`
  ones, so the original did not use `-O2`.
* `--icf=safe` and `--icf=all` fold code: `.text` shrinks by `0xa64` /
  `0x3e2c`, `.ARM.exidx` shrinks, and exported symbols collapse onto shared
  addresses. The original has no folding.
* `--gc-sections` removes 0x34 bytes of `.text`, 0x8e0 of `.ARM.exidx`,
  0x10 of `.rodata` and 0x10 of `.bss` (the 193 empty-`.text` CANTUNWIND
  sentinels are kept in the original, which `--gc-sections` would drop).
* `--hash-style=sysv` is the default and neutral; `both`/`gnu` add a
  `.gnu.hash` section (the original has none) and shift every later section.
* `-z now` is required: `-z lazy`/`-z relro` leave `.rel.plt` `sh_info` at
  0x14 instead of 0x15, change `.plt` by 11,024 bytes, and put `.got` after
  `.data` instead of before it. `-z norelro` additionally shifts the image
  base by 0x20.
* `--build-id` is sha1 (36-byte note); `md5` makes a 32-byte note and shifts
  the image by 4; `none` removes the note and shifts by 0x24.
* `--sort-section=name` reorders `.text` (and `.rodata`/`.data.rel.ro`);
  `--no-text-reorder` alone also changes `.text` by 1.1 MB, proving gold's
  default `--text-reorder` is load-bearing and already correct.
* `--merge-exidx-entries` is the default and neutral; disabling it grows
  `.ARM.exidx` from 0x6f70 to 0x95c0.
* `--rosegment` creates a third `PT_LOAD` (R) between the R E and RW loads;
  the original has only two `PT_LOAD`s.
* `-z execstack` changes `GNU_STACK` to RWE (original RW) while leaving all
  section bytes unchanged; `-z noexecstack` is the default.
* `--strip-all` and `--strip-unneeded` produce identical files here (both
  remove `.symtab`/`.strtab` and keep `.comment`). Removing `.comment`
  (`-R .comment`) is wrong: the original has the 303-byte `.comment`.
  llvm-objcopy `--strip-all` is wrong: it drops `.comment`,
  `.note.gnu.gold-version` and `.ARM.attributes`, leaves `.rel.plt`
  `SHF_INFO_LINK`/`sh_info` unnormalized, and leaves `.init_array`/
  `.fini_array` `sh_entsize=0`. The GNU bfd strip is required for the
  bfd-normalized headers.
* bfd and lld are not close: bfd puts `.rodata` before `.ARM.extab`, gives
  `.plt` `sh_entsize=4`, and has different `.dynsym`/`.hash`/`.got` sizes;
  lld emits `.gnu.hash` and `.got.plt`, puts `.ARM.exidx` first, marks
  `.rodata` `SHF_MERGE|SHF_STRINGS`, and has a different `.text` size. Both
  score `byte_exact 1345` (the same 4,524 function sizes, but shifted text).

---

## 3. The recipe is exactly right

The canonical CMake link line is:

```
ld.gold -shared -soname libgof2hdaa.so --build-id -z now
        --exclude-libs=libgcc.a -o <unstripped>
        crtbegin_so.o -L <ndk-lib> <objects in tools/source_order.txt>
        --whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
        -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
        libfmodex.so libfmodevent.so libcrypto_gof2.a -l m
        libc++abi.a libandroid_support.a libgcc.a -l dl -l c crtend_so.o
arm-linux-androideabi-strip --strip-all -o <so> <unstripped>
python3 tools/reorder_shstrtab.py <unstripped> <so>
```

Every clause is now independently pinned:

| clause | evidence |
|---|---|
| gold 1.12 | `.note.gnu.gold-version` = `"gold 1.12"`, byte-identical |
| `-shared -soname` | ELF type DYN, `DT_SONAME=libgof2hdaa.so`, 8 PH exact |
| `--build-id` (sha1) | 36-byte note; `sha1` neutral, `md5`/`none` wrong |
| `-z now` | `.rel.plt` `sh_info=0x15`, `.got` before `.data`, `DT_FLAGS=BIND_NOW`; `lazy`/`relro` wrong |
| `--exclude-libs=libgcc.a` | `__aeabi_ldivmod` defined but not exported; import set 207 |
| object order | every function at its original address; `--sort-section=name`/`--no-text-reorder` wrong |
| default `--text-reorder` | `--no-text-reorder` changes `.text` by 1.1 MB |
| default `--merge-exidx-entries` | `--no-merge-exidx-entries` grows `.ARM.exidx` by 0x2650 |
| default sysv hash | `--hash-style=both/gnu` add `.gnu.hash` |
| default 4 KiB pages | `-z max-page-size=0x1000` neutral, PH align 0x1000 |
| no rosegment | 2 `PT_LOAD`s; `--rosegment` adds a third |
| no ICF / no gc | `--icf` shrinks `.text` by 0xa64+, `--gc-sections` by 0x34 |
| GNU strip `--strip-all` | bfd-normalized headers, `.comment` kept, `.shstrtab` restored by reorder |
| `reorder_shstrtab.py` | `.shstrtab` byte-identical (0 of 279) |

---

## 4. `.ARM.extab`: corrected owner mapping (new)

`exidx_raw_audit.py` reports 11 differing EXT records / 1,411 bytes. The
owner of a record is the function at the `.ARM.exidx` entry address. The
previous note mapped owners from `.dynsym` values without clearing the Thumb
bit (`st_value & 1`), which shifted every owner by one function. With the
Thumb bit cleared, the 11 records map exactly as follows:

| owner | body byte-exact? | extab diff |
|---|---|---:|
| `Generator::createMission` | no | 24 |
| `Level::createMission` | no | 138 |
| `Level::createShip` | no | 70 |
| `ModStation::ModStation` (C2) | no | 2 |
| **`Radar::Radar` (C2)** | **yes** | **7** |
| `Radar::draw` | no | 38 |
| `LevelScript::process` | no | 279 |
| `HangarWindow::render` | no | 762 |
| `Hud::draw` | no | 37 |
| `SpaceLounge::drawLounge` | no | 14 |
| `MGame::OnTouchEnd` | no | 40 |
| **total** | | **1,411** |

So **1,404 of the 1,411 bytes are downstream of the 17 non-byte-exact
bodies** and will close automatically when those bodies close. The only
independent EH-shape residual is `Radar::Radar` C2 (7 bytes): its LSDA has
two cleanup landing pads (offsets 2462 and 2464) assigned to the opposite
call sites (rows 1/3/11/13/15 vs 19/21), while the body itself is
byte-exact. This is a source-level cleanup-scope ordering issue, not a
linker artifact.

The six non-byte-exact bodies with no extab record at all are
`MenuTouchWindow::OnTouchBegin`, `Gun::calcCharacterCollision`,
`MGame::OnUpdate`, `Layout::Layout` C1/C2 and `Status::missionCompleted`
(no EH in those functions).

---

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8622
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# section-by-section byte comparison
python3 /dev/shm/w8622/sec.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so

# one variant: extract the canonical link command from build.ninja, substitute
# the linker / insert flags, strip + reorder, then score
python3 /dev/shm/w8622/runvar.sh <name> "<extra flags>" [<ld-path>] [<strip-command>]
python3 /dev/shm/w8622/summarize.py <name> ...
```

Scratch (not committed): `/dev/shm/w8622/` — `linkcmd.txt`, `runvar.sh`,
`sec.py`, `score.py`, `summarize.py`, `out/` (one `.so` + `.res` per
variant), `verify_*/`.

---

## 6. What would falsify this note

* A linker/strip variant with `byte_exact >= 4508` and no section size/base
  regression. None of the 30 cells above does; the best non-canonical cells
  are 4507 and are byte-identical to the canonical output.
* A section size/base change that improves `.text` or `.ARM.extab`.
  `--icf`/`--gc-sections` shrink `.text`; every other departure grows the
  diff.
* A byte-exact body other than `Radar::Radar` C2 owning a differing
  `.ARM.extab` record (the corrected owner map above). The current 7-byte
  residual is a cleanup-scope ordering issue, not a linker flag.
