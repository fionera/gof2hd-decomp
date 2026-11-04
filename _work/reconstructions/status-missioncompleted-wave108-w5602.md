# `Status::missionCompleted` — wave 108: rank table in `.jcr`
# (28 sections, 87.0%, 1432 B) (w5602)

Status: **landed source change + research note**. Worktree `wt/w5602`, branch
`wave108-w5602`, base master `8f98ec4d6`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Target:
`_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`), original
`0xabf38`, size **1432 (0x598)**.

## 0. Result

The 192-byte rank table `kMissionCase[192]` moved from `.fini_array` (wave
107) to a new read-only `.jcr` section. Gold places `.jcr` in the
zero-filled gap between `.rodata` and `.fini_array` (ORDER_READONLY,
immediately after `.rodata`), so the table no longer touches the RELRO
segment:

* `DT_FINI_ARRAY` is back to `0x20cba0` / `8` bytes — the 48 table words are
  no longer called as exit destructors.
* `.dynamic` and `.rel.dyn` are byte-identical to the original again (wave
  107 had a 3-byte `.dynamic` and 4-byte `.rel.dyn` delta).
* `.fini_array`, `.data.rel.ro`, `.rodata`, `.data`, `.got`, `.init_array`,
  `.plt`, `.dynstr`, `.hash` are byte-identical.
* The target stays at **87.0%, 1432 B**; `byte_exact`/`linked_exact` stay
  **4500/4500**.

Cost: a 28th section header (`.jcr`), `.shstrtab` 0x117 -> 0x11c, file size
0x216040 -> 0x21606c (+44 B), and 661 `.dynsym` bytes differ because the
section indices after `.jcr` shift by one (`.dynsym` size is still exact).

| metric | wave-107 HEAD (`.fini_array`) | this pass (`.jcr`) | original |
|---|---:|---:|---:|
| section headers | 27 | **28** | 27 |
| file size | 0x216040 | **0x21606c** | 0x216040 |
| `.shstrtab` size | 0x117 | **0x11c** | 0x117 |
| `.jcr` | — | **0xc0 @ 0x20b977** | — |
| `.fini_array` size / addr | 0xc8 / 0x20cae0 | **8 / 0x20cba0** | 8 / 0x20cba0 |
| `DT_FINI_ARRAYSZ` | 200 | **8** | 8 |
| `.dynamic` | 3 B diff | **exact** | — |
| `.rel.dyn` | 4 B diff | **exact** | — |
| `.data.rel.ro` | exact | **exact** | — |
| `.rodata` / `.dynsym` size | exact | **exact** | — |
| `.ARM.extab` | 0x26298 | **0x26298** | 0x26298 |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** | — |
| `missionCompleted` fuzzy | 87.0% | **87.0%** | — |

The only program-header delta is the RO `LOAD` FileSiz/MemSiz
`0x20b977 -> 0x20ba37` (the `.jcr` bytes fit in the existing gap; the RW
`LOAD`, `GNU_RELRO`, `DYNAMIC`, `EXIDX` and `NOTE` headers are unchanged).

## 1. Gate

```
cmake --build cmake-build-match --target check -j6
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

`report.json` row: `pct 87.0`, `osize 1432`, `usize 1432`, `byte false`,
`linked false`.

## 2. Why `.jcr`

Gold's `Layout::default_section_order` (binutils 2.27 `gold/layout.cc`) puts
read-only PROGBITS sections in `ORDER_READONLY`, after `.text`/`.fini` and
before the RELRO group. `.rodata` and any unknown read-only section are both
`ORDER_READONLY`, ordered by creation; `.rodata` is created first, so a
read-only `.jcr` input lands immediately after `.rodata` at `0x20b977` — the
first byte of the zero-filled `.rodata`..`.fini_array` gap. `.fini_array`
stays at `0x20cba0` because the gap is 0x1229 bytes and `.jcr` is only 0xc0.

The table must stay read-only for this placement: a writable `.jcr` is
recognised as RELRO by `make_output_section` and would be placed before
`.fini_array` (moving the RELRO group). The array is `static const`, so its
input section has `SHF_ALLOC` only.

`.jcr` is the shortest gold-recognised read-only section name that is not
otherwise emitted (4 chars -> `.shstrtab` +5 bytes). `.eh_frame`,
`.gcc_except_table`, `.tm_clone_table`, `.data.rel.ro.local`, `.rel.ro`,
`.sdata`, `.lrodata` were all measured; they behave the same but cost more
`.shstrtab` bytes (`.tm_clone_table` +0x38 file, `.data.rel.ro.local` +0x3c).

## 3. Measured variants (full link + official verifier)

All rows are `Status.cpp`-only edits, rebuilt with the canonical
`cmake-build-match` build dir and scored with `tools/verify/verify.py`.

| variant | pct | usize | sections | file | byte_exact | notes |
|---|---:|---:|---:|---:|---:|---|
| wave-107 HEAD: table in `.fini_array` | 87.0 | 1432 | 27 | 0x216040 | 4500 | `.dynamic`/`.rel.dyn` deltas, destructor hazard |
| **table in `.jcr`** | **87.0** | **1432** | **28** | **0x21606c** | **4500** | **landed; `.dynamic`/`.rel.dyn` exact** |
| table in `.rodata` (`#pragma ... ".rodata.gap"`) | 87.0 | 1432 | 27 | 0x216040 | 4424 | `.rodata` +0xc0, content shifts |
| table in `.note.gnu.build-id` | 87.0 | 1432 | 27 | 0x216040 | 2932 | note +0xc0, shifts `.dynsym`..`.text` |
| writable table in `.rodata` | 87.0 | 1432 | 27 | 0x216040 | 4424 | merges into `.rodata` |
| `.jcr` + cache `int missionType` | 87.0 | 1432 | 28 | — | 4500 | identical codegen |
| `.jcr` + `(int)` cast on table index | 87.0 | 1432 | 28 | — | 4500 | identical codegen |
| `.jcr` + `unsigned` index | 87.0 | 1432 | 28 | — | 4500 | identical codegen |
| `.jcr` + split `case 0:` / `case 11:` | 87.0 | 1432 | 28 | — | 4500 | clang merges the identical bodies |
| contiguous ranks 0..27 (one 28-entry TBH) | 69.9 | 1388 | 27 | — | 2326 | no `sub #150`, no low switch; -44 B |

The contiguous-rank experiment relabels every case to 0..27 in the original
body order (splitting `case 0`/`case 11`). It removes both the `sub #150`
and the low `sub #8` dispatch, shrinking the function to 1388 B; the body
order is right but the dispatch no longer resembles the original's
`sub #150; cmp #39; tbh` + low TBH, so the score drops to 69.9 and the
`.text` size change collapses `byte_exact` to 2326.

## 4. Residual score (unchanged from wave 107)

The 13% residual is the same as wave 107: the rank table adds
`ldr/add/ldrb` before the `sub #150; cmp #31; tbh`, and the 32-entry table
puts the body region 10 bytes earlier than the original's 40-entry TBH.
The body code itself is mostly instruction-identical; the mismatch is
mostly `loc_*` branch targets shifted by the table-size difference plus the
case-159 null anchor (`cmp r0,#0; beq` — a deliberate 4-byte size anchor).
No source-only variant found this pass beats 87.0 at 1432 B.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5602
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target lint -j6    # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields

RD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RD -SW /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /tmp/o.txt
$RD -SW cmake-build-match/libgof2hdaa.so > /tmp/m.txt
diff /tmp/o.txt /tmp/m.txt   # .jcr inserted; .fini_array/.data.rel.ro exact
```

Scratch (not committed): `/dev/shm/w5602/` — `Status.jcr.cpp` (landed),
`Status.contig.cpp`, `Status.split011.cpp`, `trysec.sh`, `score.sh`,
`runvar.sh`, `orig.dis`, `ours.dis`.

## 6. Proven vs hypothesis

**Proven (command-backed):**

* `.jcr` placement gives `.fini_array` 8 B @ 0x20cba0, byte-identical
  `.dynamic`/`.rel.dyn`/`.rodata`/`.data.rel.ro`, no exit-destructor calls,
  `byte_exact` 4500, `linked_exact` 4500, lint CLEAN, drift 0, target
  87.0% / 1432 B.
* The cost is one extra section header and 661 `.dynsym` bytes of
  `st_shndx` index shift (size exact).
* `.rodata` and `.note.gnu.build-id` placements keep 27 sections but move
  data and collapse `byte_exact` (4424 / 2932).
* A single contiguous 0..27 rank table is 1388 B and scores 69.9.

**Hypothesis / open:**

* The 87.0% ceiling is set by the 10-byte body-region shift and the extra
  table-load instructions; aligning the bodies would require a 10-byte
  dispatch anchor plus a matching 10-byte body shrink, and the task forbids
  padding anchors.
* If a future pass can keep `.dynsym` byte-identical with a 28th section,
  the section would have to be created after `.bss` (no allocated PROGBITS
  order exists after `ORDER_BSS` in gold 2.27).
