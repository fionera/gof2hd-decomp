# Wave-92 documentation sync (w4269)

Date: 2026-11-03. Branch `wave92-w4269`, base `master b379d00d`.
Scope: **docs-only** — `AGENTS.md`, `docs/VALIDATION.md`, `docs/IDENTICALITY.md`.
No source file, build flag, tool, or baseline was changed.

This note records the evidence used to correct the current-state lines. Historical
sections in `docs/IDENTICALITY.md` are kept; the 2026-10-26 section is now marked
historical and a new 2026-11-03 section states the authoritative state.

## 1. Gate before the pass

`cmake --preset default` then (in this nested worktree, because the default
`../_work/bins/...` path does not resolve from `/home/debian/GalaxyOnFire2/wt/w4269`):

```
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Result:

```
compared 4524  avg 99.9%  byte_exact 4499  linked_exact 4499  fuzzy_perfect 4499
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`drift`:

```
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`lint`:

```
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`diff`:

```
original: 5370  ours: 5370  extra: 0  missing: 0 (wrong_type: 0  absent: 0)
SYMBOL PARITY ✓
```

## 2. ELF section evidence

Measured directly from the two files with the NDK `readelf` and a small Python
parser (section name, type, address, offset, size; then byte-compare every
non-NOBITS section body). Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Ours: `cmake-build-match/libgof2hdaa.so`.

Both files have **27 sections** and the same `(type, addr, offset, size)` for every
one. The section-header table itself is byte-identical:

```
shoff 2186248 == 2186248, entsize 40 == 40, num 27 == 27
section header table bytes identical: True  (1080 bytes)
```

Per-section content differences (bytes):

| section | size | base | differing bytes |
|---|---:|---:|---:|
| .text | 1,259,464 (0x1337c8) | 0x69440 | **107,135** |
| .ARM.exidx | 28,528 (0x6f70) | 0x19cc08 | **839** |
| .ARM.extab | 156,312 (0x26298) | 0x1a3b78 | **108,469** |
| .note.gnu.build-id | 36 | 0x1cc | **20** |
| .rodata | 269,159 (0x41b67) | 0x1c9e10 | 0 |
| .data | 10,604 (0x296c) | 0x214000 | 0 |
| .data.rel.ro | 12,264 (0x2fe8) | 0x20cbb0 | 0 |
| .bss | 27,052 (0x69ac) | 0x216970 | 0 (NOBITS; `sh_offset` 0x21596c) |
| .got | 17,164 (0x430c) | 0x20fcf4 | 0 |
| .rel.plt | 29,392 (0x72d0) | 0x5751c | **0 (byte-identical)** |
| .rel.dyn | 28,744 (0x7048) | 0x504d4 | 0 |
| .dynsym | 89,248 (0x15ca0) | 0x1f0 | 0 |
| .dynstr | 189,208 (0x2e318) | 0x15e90 | 0 |
| .hash | 38,716 (0x973c) | 0x441a8 | 0 |
| .gnu.version | 11,156 | 0x4d8e4 | 0 |
| .gnu.version_d | 28 | 0x50478 | 0 |
| .gnu.version_r | 64 | 0x50494 | 0 |
| .plt | 44,108 (0xac4c) | 0x5e7ec | 0 |
| .dynamic | 328 (0x148) | 0x20fba4 | 0 |
| .init_array | 12 | 0x20fb98 | 0 |
| .fini_array | 8 | 0x20cba0 | 0 |
| .comment | 303 | 0 | 0 |
| .note.android.ident | 152 | 0x134 | 0 |
| .note.gnu.gold-version | 28 | 0 | 0 |
| .ARM.attributes | 56 | 0 | 0 |
| .shstrtab | 279 (0x117) | 0 | 0 |

So the only differing section bytes are `.text`, `.ARM.exidx`, `.ARM.extab` and
`.note.gnu.build-id`. Every section is size- and base-exact; `.rel.plt` content is
byte-identical; all 27 section headers are exact.

`.ARM.exidx` structure was also re-derived from the original: 3566 entries
(28,528 / 8), of which 272 are `EXIDX_CANTUNWIND`, 856 are inline compact, and
2438 point into `.ARM.extab`. The 839 differing bytes are 822 entries whose `w1`
(unwind word) differs; no `w0` (prel31 function target) differs.

`.dynsym` has 5,578 entries (5,370 defined + 208 undefined); the byte comparison is
0, so the gold hash-map iteration order is now reproduced exactly. The undefined
import set is identical to the original's 207 names, including `memalign` (the
former `posix_memalign` delta is gone; `libandroid_support.a` is on the link line).

## 3. Build recipe evidence

From `CMakeLists.txt` / `cmake/toolchain.cmake`:

* CMake + NDK r18b, clang 7.0.2 (not ndk-build).
* Compile flags: `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
  -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
  -fstack-protector-strong -Oz -g -DANDROID -DNDEBUG -stdlib=libc++`;
  `-ffunction-sections -funwind-tables` are added by `target_compile_options`.
* Link: one plain gold link, no `-s`:
  `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`.
* Post-link: GNU `arm-linux-androideabi-strip --strip-all`, then
  `tools/reorder_shstrtab.py` restores gold's Stringpool `.shstrtab` order.
* Link order: `tools/source_order.txt` (192 `src/` TUs, checked in and read
  directly by `CMakeLists.txt`). No `--section-ordering-file`, no second pass,
  no `--unresolved-symbols` switch.
* `tools/gen_section_order.py` and `tools/gen_exidx_markers.py` do not exist;
  `tools/gen_link_order.py` is analysis-only.
* `tools/sodiff_allow.txt` is empty (comments only).

## 4. The 25 non-byte-exact functions

From `report.json` (all rows with `pct < 100`), sorted worst-first:

```
47.700  _ZN6LayoutC1Ev
47.700  _ZN6LayoutC2Ev
52.000  _ZN11LevelScript7processEi
57.700  _ZN6Status16missionCompletedEbbx
73.900  _ZN5Radar4drawEP6PlayerP3Hudi
74.100  _ZN5Level10createShipEiiiP8Waypointbb
74.300  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
78.400  _ZN5MGame10OnTouchEndEiiPv
78.800  _ZN12HangarWindow6renderEv
80.500  _ZN7GlobalsC1Ev
80.500  _ZN7GlobalsC2Ev
86.200  _ZN3Hud4drawExxP9PlayerEgobjj
86.900  _ZN9RocketGun6updateEi
87.300  _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
88.300  _ZN5Level11createSceneEv
90.000  _ZN3Gun22calcCharacterCollisionEv
90.700  _ZN5Level13createMissionEv
94.200  _ZN15MenuTouchWindowC1Ei
94.200  _ZN15MenuTouchWindowC2Ei
95.500  _ZN11SpaceLounge10drawLoungeEv
97.700  _ZN15MenuTouchWindow12OnTouchBeginEiiPv
97.900  _ZN15MenuTouchWindow10OnTouchEndEiiPv
98.400  _ZN10ModStationC1Ev
98.400  _ZN10ModStationC2Ev
99.900  _ZN5MGame8OnUpdateEv
```

Bucket counts: `{100: 4499, 90-99: 10, 80-89: 6, 70-79: 5, <60: 4}`.
`size_match` is 4524/4524 (no size mismatch). 16 non-exact functions are >=80%.

## 5. Documentation changes made

`AGENTS.md`:
* Added the CMake/NDK-r18b + gold-without-`-s` + GNU-strip + `reorder_shstrtab.py`
  build recipe, the exact flag set, and the nested-worktree configure override.
* Added a "Current state (2026-11-03, wave 92)" section with the gate counts and
  the section/content summary.
* Removed the stale `--section-ordering-file` / `--unresolved-symbols=ignore-all`
  "approved deviations"; the link order is `tools/source_order.txt`.
* Corrected the TU-organization bullet (checked-in source list, not a
  section-ordering file) and the shim-import pitfall (the ratchet pins 207 names).

`docs/VALIDATION.md`:
* `.dynsym` order is now byte-identical, not "5566/5578 positions".
* The size/base-exact section list now includes `.ARM.extab`, `.rel.plt`,
  `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`; `.rel.plt` content is byte-identical
  and all 27 headers are exact.
* Added a current ELF-section state block.
* Replaced "Known import delta: posix_memalign" with "Import set: identical"
  (both import `memalign`; 207 names).
* Updated the current-state dates and the pinned-import wording.

`docs/IDENTICALITY.md`:
* Added the 2026-11-03 authoritative update (gate, build recipe, section table,
  25-function list) and marked the 2026-10-26 update historical.
* Corrected "current differing bytes" to `.text` 107,135; `.ARM.extab` 108,469;
  `.ARM.exidx` 839; build-id 20; `.rel.plt` 0; everything else 0.
* `.dynsym`/`.dynstr` rows are now byte-identical; added `.rel.plt`, `.rel.dyn`
  and `.hash` rows.
* Updated the remaining-gaps worst list, the link-metadata item, the "Reality"
  count (27 -> 25), the pct buckets (10/6/5/4) and the near-miss count (16).
* Marked the shader `UpdateMeshData` and `ParticleSettings_str` leads resolved
  (no shader function is in the current 25; `.rodata` is byte-identical).

## 6. What remains

* 25 non-byte-exact functions (list above), all size-exact; these are body /
  register-allocation / block-order deltas, not missing code.
* The three remaining LSDA record deltas (`Generator::createMission` +4,
  `Level::createShip` -8, `LevelScript::process` +4, net 0) and the 839 raw
  `.ARM.exidx` bytes they cause.
* The 108,469 differing `.ARM.extab` address operands that follow the local
  `.text` layout.
* The GNU build-id (20 differing bytes), a content hash that converges last.

## 7. Gate after the pass

Docs-only change; `check` was rerun after the edits and is unchanged:

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
