# Literal-pool audit (w3390)

Scope: the functions that `report.json` scores at `pct == 100.0` but not `byte` — i.e. the
instruction stream and every inter-function address already match, and only literal-pool words
(and the branch immediates that reference the shifted PLT slots) still differ.

Starting point (branch `wave87-w3390`, HEAD `ddc2fd8f`): **byte_exact 3921**, 575 such functions.
Ending point: **byte_exact 4107**, 389 such functions (see the commit for the exact gate line).

## Method

`tools/verify/report.json` is produced by `cmake --build cmake-build-match --target verify`.
For every `pct == 100.0 && !byte` row the audit script disassembles the function body from both
`.so` files, separates instruction words from literal-pool words (the same `PCREL_LOAD` /
pool-byte logic `tools/verify/verify.py` uses), and classifies each differing pool word.

The important correction to a naive classification: **ARM PIC code stores a PC-relative delta,
not an address.**  The sequence

```
ldr rX, [pc, #N]      @ pool word = delta
add rX, pc            @ rX = address of the datum
```

means the raw pool word is `datum - (address of the add + 4)`.  Classifying the raw word by the
section its *value* falls in is therefore wrong (it usually lands in `.text` and looks like a
"wrong function pointer").  The audit resolves the `add rX, pc` that consumes each pool word and
classifies the **destination address** instead.  With that resolution:

* **all 600** differing PIC pool words in the ending tree point at the *same logical datum*
  (same string bytes, same GOT symbol, same global) — there are **no wrong function pointers**
  in this set;
* the only `.text`-classified pool words are two non-PIC words in the SHA-224/256 state code,
  which are deltas into `.bss`.

The coordinator's `Level::createCampaignMission` example (`0xb4ea8`: `0x148622` vs `0x1483fa`)
is a PIC delta to `.rodata` (`0x1fd45e` vs `0x1fd436`), not a function pointer.

### Commands

```sh
cmake --preset default
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target verify  -j8     # writes report.json
python3 /tmp/classify3.py                                # PIC-resolved classification
cmake --build cmake-build-match --target check   -j8     # ratchet
```

`/tmp/classify3.py` is the audit driver (it reuses `tools/verify/verify.py`'s pool/jump-table
scanner and `elf.Elf32`).  It writes `/tmp/classify3.json`, one record per target function with
`kind`, pool word, resolved `odest`/`udest`, destination section, and resolved symbol/string.

Section ranges in the original (image base 0x10000):

| section | range |
|---|---|
| `.text` | `0x69440..0x19cc08` |
| `.rodata` | `0x1c9e10..0x20b977` |
| `.data.rel.ro` | `0x20cbb0..0x20fb98` |
| `.got` | `0x20fcf4..0x214000` |
| `.data` | `0x214000..0x21696c` |
| `.bss` | `0x216970..0x21d31c` |

## Ranked classification (ending tree, 389 functions)

Word counts (a "run" is one contiguous differing word):

| class | words | functions |
|---|---:|---:|
| PIC pool -> `.rodata` (string/table offset) | 463 | 187 |
| instruction word (branch immediate, same resolved symbol) | 1153 | 181 |
| PIC pool -> `.got` (same slot, function base moved) | 72 | 52 |
| PIC pool -> `.bss` (same global, address moved) | 48 | 29 |
| PIC pool -> `.data` (same global) | 12 | 7 |
| PIC pool -> `.data.rel.ro` (same vtable/pointer table) | 5 | 5 |
| non-PIC `.text` word (delta into `.bss`) | 2 | 2 |

Ranked causes:

1. **`.rodata` chunk ordering (463 words / 187 funcs).**  `gold` concatenates the per-TU
   `.rodata.str1.1` chunks in link order (deduplicating mergeable strings).  The original's
   `.rodata` is the witness for its *link* order, and it disagrees with the `.text`-address
   heuristic in `tools/gen_link_order.py` for `LowResourceTable.cpp` vs `ResourceMaterial.cpp`:
   the original's low-resource strings sit at `0x1993e` and the portrait strings at `0x1b255`,
   but `loadPortraits` (ResourceMaterial) has the lower `.text` address.  Swapping the two
   objects' link positions fixes the whole 400-word block (see pass 2).
   Remaining `.rodata` deltas are a section-*group* ordering issue: our `.str1.8` group is
   emitted before the plain `.rodata`/`.rodata._ZTS` groups, the original's after them
   (`data/audio/FMOD_GOF2.fev` at `0x1d770` vs `0x316d8`), plus a handful of small table moves.
2. **PLT slot ordering (1153 instruction words / 181 funcs).**  `gold` assigns PLT slots in
   *input* (object) order, not output `.text` order.  Because the section-ordering file reorders
   `.text`, a function that is early in `.text` can still be late in its object.  The 29
   divergent slots rotate two groups: `FileRead::loadStaticCollision/loadStationCollision`
   (ours `0x64368/0x64374`, orig `0x643b0/0x643bc`) and `Standing::setStanding`
   (ours `0x66c78`, orig `0x66c00`).  These are real TU/emission-order issues, not literal
   values: the branch immediates point at the same symbol, just a different PLT slot.
3. **`.got` (72 words / 52 funcs).**  Same GOT slot address in both binaries; the function's
   own base address moved, so the PC-relative delta to the slot differs.  These disappear once
   the function's `.text` placement is exact (they are all in functions whose address differs
   by the `.ARM.extab` `-0xc` / `.bss` `+0x10` residue).
4. **`.bss` (48 / 29), `.data` (12 / 7), `.data.rel.ro` (5 / 5).**  Same globals/pointer tables;
   the destination address moved because our `.bss` starts at `0x216980` instead of `0x216970`
   (the known `.data`/`.bss` layout residue).
5. **Non-PIC `.text` words (2 / 2).**  SHA-224/256 state pointers stored as deltas into `.bss`.

## Fixes landed

### Pass 1 — GOT ordering (`8db58dfd`, byte 3921 -> 4105)

`Engine *GetEngine()` was defined in `src/engine/core/ApplicationManager.cpp`, so its `gEngine`
reference created the GOT slot ~139 slots too early and shifted the whole shader GOT block by
`+4`.  The original TU starts at `GetEngine` (`.text 0x93c88`), so the function was moved to
`src/platform/android/NdkHooks.cpp`.  Four more first-reference orderings were corrected:

* `opensubkeyfile()` before `F()` in `src/engine/file/BlowfishSubkeyFile.cpp` (orig
  `opensubkeyfile 0x6b2a0`, `F 0x6b2d4`);
* `GlowPPShader::GlowPPShader()` before `SetInActive()` in `GlowPPShader.cpp`;
* `BoundingAAB::BoundingAAB(...)` at the top of `BoundingAAB.cpp`;
* `ndk_resetNativeItemInformationList()` at the top of `LODManager.cpp`.

After pass 1 the GOT symbol sequence is identical (0 slot-address deltas over 767 slots).

### Pass 2 — link order from the `.rodata` witness (`tools/gen_link_order.py`)

The original's `.rodata.str1.1` chunk order is the authority for the link order (the `.text`
order is separately reproduced by `--section-ordering-file`).  `LowResourceTable.cpp` must be
linked before `ResourceMaterial.cpp` even though `loadPortraits` has the lower `.text` address.
`gen_link_order.py` now applies that single override.  `.text`, `.ARM.exidx`, `.data`, `.got`
and `.bss` sizes/addresses are unchanged; `.rodata`'s `LowResourceTable`/`ResourceMaterial`
blocks move to the original offsets (`0x1993e` / `0x1b255`).

Measured: **byte_exact 4105 -> 4107**, `.rodata` pool deltas 863 -> 463 words, targets
391 -> 389.  The 400 fixed words are spread over ~100 functions that still carry PLT-slot
instruction deltas, which is why only two functions flip to fully byte-exact.

## Remaining backlog (389 functions)

| class | words | functions |
|---|---:|---:|
| `.rodata` | 463 | 187 |
| instruction (PLT slots) | 1153 | 181 |
| `.got` | 72 | 52 |
| `.bss` | 48 | 29 |
| `.data` | 12 | 7 |
| `.data.rel.ro` | 5 | 5 |
| `.text` (non-PIC) | 2 | 2 |

Next steps, in value order:

1. **PLT slot order.**  Reorder the functions inside `Level.cpp` (and the other TUs that make
   the first reference) so the object's section order matches the original's PLT first-reference
   order.  `clang` emits a forward-referenced function immediately after the function that first
   references it, so the object's section order is *not* the `.cpp` order; the `.ll`/`.o`
   section order is the thing to inspect.  Fixing the 29 slots removes ~1150 instruction-word
   deltas.
2. **`.rodata` section-group order.**  Our `.rodata.str1.8` group lands before the plain
   `.rodata` / `.rodata._ZTS` groups; the original has it after.  This is a `gold` orphan-section
   ordering issue (the group order follows the first input section with each name), so it needs
   the object that first introduces `.str1.8` to be moved, or a per-object section rename.
3. **`.bss` base `+0x10`.**  Our `.bss` starts at `0x216980` vs `0x216970`.  The `.data` size is
   exact, so this is a `.data`→`.bss` alignment/padding difference; fixing it removes the 48
   `.bss` pool deltas and the 72 `.got` deltas in one go.
4. The libzip `.rodata` block (`No error` vs `TORRENTZIPPED-`) is a `zip_err_str.o` pull-order
   issue; moving the member in `third_party/build_match_lib.cmake` did **not** change the
   emitted order (gold pulls it on the first undefined `_zip_err_str` reference), so it is left
   as-is.

## Full classification table

Every `pct == 100.0 && !byte` function in the ending tree.  `size` is the function byte size;
`diff words` counts differing 4-byte words; `classes` is the PIC-resolved destination section
(`insn` = instruction word).

| # | function | size | diff words | classes |
|---:|---|---:|---:|---|
| 1 | `_ZN5Level19createRadioMessagesEi` | 12282 | 441 | insn:441 |
| 2 | `zip_close` | 2532 | 116 | insn:108, pool .got:1, pool .rodata:7 |
| 3 | `zip_open` | 1200 | 54 | insn:50, pool .got:1, pool .rodata:3 |
| 4 | `_ZN5Level21createCampaignMissionEv` | 29676 | 49 | insn:4, pool .rodata:45 |
| 5 | `inflate` | 4360 | 47 | insn:24, pool .got:1, pool .rodata:22 |
| 6 | `inflateBack` | 2568 | 22 | insn:6, pool .got:1, pool .rodata:15 |
| 7 | `__cxa_call_unexpected` | 408 | 21 | insn:18, pool .got:3 |
| 8 | `_zip_error_strerror` | 280 | 19 | insn:10, pool .got:5, pool .rodata:4 |
| 9 | `deflate` | 1732 | 17 | insn:14, pool .data.rel.ro:1, pool .got:2 |
| 10 | `zip_fopen_index` | 476 | 17 | insn:16, pool .rodata:1 |
| 11 | `_ZN14DialogueWindow11loadContentEv` | 3484 | 15 | pool .bss:2, pool .rodata:13 |
| 12 | `_ZN5Level18createRadioMessageEii` | 1592 | 15 | insn:9, pool .rodata:6 |
| 13 | `_ZN5Level9createGunEiiiiiiii` | 2012 | 12 | pool .rodata:12 |
| 14 | `_zip_dirent_read` | 724 | 12 | insn:8, pool .got:2, pool .rodata:2 |
| 15 | `_zip_dirent_write` | 344 | 12 | insn:9, pool .got:1, pool .rodata:2 |
| 16 | `zip_source_zip` | 324 | 12 | insn:11, pool .got:1 |
| 17 | `__cxa_guard_abort` | 88 | 11 | insn:6, pool .bss:2, pool .rodata:3 |
| 18 | `__cxa_guard_acquire` | 116 | 11 | insn:6, pool .bss:2, pool .rodata:3 |
| 19 | `__cxa_guard_release` | 88 | 11 | insn:6, pool .bss:2, pool .rodata:3 |
| 20 | `_zip_mkstemp` | 340 | 11 | insn:7, pool .data:3, pool .got:1 |
| 21 | `zip_error_to_str` | 172 | 11 | insn:4, pool .got:3, pool .rodata:4 |
| 22 | `__cxa_demangle` | 568 | 10 | insn:4, pool .data.rel.ro:1, pool .got:1, pool .rodata:4 |
| 23 | `_ZnwjSt11align_val_t` | 128 | 9 | insn:6, pool .got:3 |
| 24 | `_zip_cdir_write` | 196 | 9 | insn:8, pool .rodata:1 |
| 25 | `_zip_dirent_torrent_normalize` | 216 | 9 | insn:6, pool .bss:2, pool .got:1 |
| 26 | `_zip_filerange_crc` | 184 | 9 | insn:8, pool .got:1 |
| 27 | `zip_add_dir` | 156 | 9 | insn:9 |
| 28 | `_zip_free` | 132 | 8 | insn:8 |
| 29 | `_Z10keyPressedPN11AbyssEngine6EngineEi` | 708 | 7 | pool .bss:7 |
| 30 | `_ZN10ModStation10OnTouchEndEiiPv` | 5404 | 7 | insn:6, pool .rodata:1 |
| 31 | `_ZN10ModStation12OnInitializeEv` | 6396 | 7 | pool .data:2, pool .rodata:5 |
| 32 | `_ZN11TouchButton4initERKN11AbyssEngine6StringEjiiiiiihh` | 1816 | 7 | pool .rodata:7 |
| 33 | `_Znwj` | 68 | 7 | insn:5, pool .got:2 |
| 34 | `__cxa_get_globals` | 72 | 7 | insn:4, pool .bss:1, pool .rodata:2 |
| 35 | `_zip_file_get_offset` | 144 | 7 | insn:6, pool .got:1 |
| 36 | `_zip_source_file_or_p` | 152 | 7 | insn:7 |
| 37 | `compress2` | 136 | 7 | insn:5, pool .got:1, pool .rodata:1 |
| 38 | `deflateCopy` | 280 | 7 | insn:7 |
| 39 | `uncompress` | 144 | 7 | insn:5, pool .got:1, pool .rodata:1 |
| 40 | `SHA256_Update` | 272 | 6 | insn:6 |
| 41 | `_ZN11AbyssEngine11BloomShader4InitEPNS_6EngineE` | 520 | 6 | pool .rodata:6 |
| 42 | `_ZN13PlayerStationC1EP7Station` | 3440 | 6 | insn:6 |
| 43 | `_ZN13PlayerStationC2EP7Station` | 3440 | 6 | insn:6 |
| 44 | `_ZN5Level19initParticleSystemsEv` | 1872 | 6 | pool .rodata:6 |
| 45 | `__cxa_rethrow_primary_exception` | 120 | 6 | insn:6 |
| 46 | `_zip_file_fillbuf` | 158 | 6 | insn:6 |
| 47 | `_zip_unchange` | 156 | 6 | insn:6 |
| 48 | `deflateInit2_` | 416 | 6 | insn:3, pool .got:3 |
| 49 | `inflate_table` | 928 | 6 | insn:1, pool .got:1, pool .rodata:4 |
| 50 | `zip_fread` | 282 | 6 | insn:6 |
| 51 | `SHA224` | 100 | 5 | insn:4, pool .text:1 |
| 52 | `SHA256` | 104 | 5 | insn:4, pool .text:1 |
| 53 | `SHA256_Final` | 1048 | 5 | insn:5 |
| 54 | `_ZN10StarSystemC1Ei` | 2900 | 5 | pool .rodata:5 |
| 55 | `_ZN10StarSystemC2Ei` | 2900 | 5 | pool .rodata:5 |
| 56 | `_ZN11AbyssEngine12GlowPPShader4InitEPNS_6EngineE` | 432 | 5 | pool .rodata:5 |
| 57 | `_ZN7Globals12getShipGroupEiib` | 1856 | 5 | pool .rodata:5 |
| 58 | `__cxa_get_globals_fast` | 52 | 5 | insn:3, pool .bss:1, pool .rodata:1 |
| 59 | `_tr_flush_block` | 964 | 5 | insn:1, pool .rodata:4 |
| 60 | `_zip_cdir_new` | 98 | 5 | insn:5 |
| 61 | `_zip_name_locate` | 192 | 5 | insn:3, pool .got:2 |
| 62 | `adler32` | 516 | 5 | insn:5 |
| 63 | `deflateReset` | 192 | 5 | insn:4, pool .data.rel.ro:1 |
| 64 | `zip_rename` | 128 | 5 | insn:5 |
| 65 | `zip_source_buffer` | 120 | 5 | insn:5 |
| 66 | `_ZN10RepairBeam6updateEiP5RadarP5LevelP3Hud` | 1872 | 4 | pool .rodata:4 |
| 67 | `_ZN11AbyssEngine6Engine13SetPostEffectEjb` | 352 | 4 | pool .bss:1, pool .data:3 |
| 68 | `_ZN11SpaceLoungeC1Ev` | 1200 | 4 | pool .rodata:4 |
| 69 | `_ZN11SpaceLoungeC2Ev` | 1200 | 4 | pool .rodata:4 |
| 70 | `_ZN14ListItemWindow3setEP8ListItemjjjjb` | 6820 | 4 | pool .rodata:4 |
| 71 | `_ZN5Level15createAsteroidsEv` | 1588 | 4 | pool .rodata:4 |
| 72 | `_ZnajRKSt9nothrow_t` | 26 | 4 | insn:4 |
| 73 | `_ZnajSt11align_val_tRKSt9nothrow_t` | 26 | 4 | insn:4 |
| 74 | `_ZnwjRKSt9nothrow_t` | 26 | 4 | insn:4 |
| 75 | `_ZnwjSt11align_val_tRKSt9nothrow_t` | 26 | 4 | insn:4 |
| 76 | `__cxa_begin_catch` | 112 | 4 | insn:4 |
| 77 | `__cxa_rethrow` | 96 | 4 | insn:4 |
| 78 | `__cxa_throw` | 92 | 4 | insn:4 |
| 79 | `_zip_set_name` | 136 | 4 | insn:4 |
| 80 | `inflateSetDictionary` | 138 | 4 | insn:4 |
| 81 | `zip_fclose` | 110 | 4 | insn:4 |
| 82 | `_Z13simulateTouchPN11AbyssEngine6EngineE` | 1928 | 3 | pool .bss:3 |
| 83 | `_ZN10StarSystem9initLightEv` | 1476 | 3 | pool .rodata:3 |
| 84 | `_ZN11AbyssEngine13TextureShader4InitEPNS_6EngineE` | 72 | 3 | pool .rodata:3 |
| 85 | `_ZN11AbyssEngine21TextureVtxColorShader4InitEPNS_6EngineE` | 72 | 3 | pool .rodata:3 |
| 86 | `_ZN11AbyssEngine22TextureAlphaTestShader4InitEPNS_6EngineE` | 72 | 3 | pool .rodata:3 |
| 87 | `_ZN11SpaceLounge4initEv` | 2256 | 3 | pool .bss:1, pool .rodata:2 |
| 88 | `_ZN12ChoiceWindow4drawEv` | 784 | 3 | pool .rodata:3 |
| 89 | `_ZN12ChoiceWindow8setMedalEii` | 476 | 3 | pool .rodata:3 |
| 90 | `_ZN14DialogueWindow4initEv` | 652 | 3 | pool .rodata:3 |
| 91 | `_ZN14DialogueWindow51pickGermanGenericTextBecauseWeSaved100EurosWithThatEiP5Agent` | 184 | 3 | pool .rodata:3 |
| 92 | `_ZN5MGame12switchCameraEi` | 316 | 3 | insn:3 |
| 93 | `_ZN6GalaxyC1Ev` | 76 | 3 | insn:3 |
| 94 | `_ZN6GalaxyC2Ev` | 76 | 3 | insn:3 |
| 95 | `_ZN7StarMap14initStarSystemEv` | 2076 | 3 | pool .rodata:3 |
| 96 | `_ZN8GameText23convertStringFromArabicEN11AbyssEngine6StringE` | 728 | 3 | pool .rodata:3 |
| 97 | `_ZN8GameText7getTextEi` | 292 | 3 | pool .bss:3 |
| 98 | `_ZN9Generator12createAgentsEP7Station` | 1464 | 3 | pool .rodata:3 |
| 99 | `_ZN9Generator14getShipBuyListEP7Station` | 2244 | 3 | pool .rodata:3 |
| 100 | `_ZN9LensFlare8render2DEfffi` | 1568 | 3 | pool .rodata:3 |
| 101 | `_ZSt9terminatev` | 84 | 3 | insn:2, pool .got:1 |
| 102 | `__cxa_allocate_exception` | 42 | 3 | insn:3 |
| 103 | `__cxa_begin_cleanup` | 88 | 3 | insn:3 |
| 104 | `__cxa_current_primary_exception` | 96 | 3 | insn:3 |
| 105 | `__cxa_end_catch` | 136 | 3 | insn:3 |
| 106 | `__dynamic_cast` | 208 | 3 | insn:2, pool .got:1 |
| 107 | `__gxx_personality_v0` | 348 | 3 | insn:2, pool .got:1 |
| 108 | `_zip_entry_free` | 40 | 3 | insn:3 |
| 109 | `_zip_entry_new` | 118 | 3 | insn:3 |
| 110 | `_zip_memdup` | 56 | 3 | insn:3 |
| 111 | `_zip_new` | 74 | 3 | insn:3 |
| 112 | `_zip_replace` | 106 | 3 | insn:3 |
| 113 | `inflateCopy` | 206 | 3 | insn:3 |
| 114 | `inflateInit2_` | 160 | 3 | insn:1, pool .got:2 |
| 115 | `inflateSync` | 236 | 3 | insn:2, pool .got:1 |
| 116 | `inflate_fast` | 972 | 3 | pool .rodata:3 |
| 117 | `ndk23_handleAcceleration` | 276 | 3 | pool .bss:3 |
| 118 | `zip_set_archive_comment` | 80 | 3 | insn:3 |
| 119 | `zip_set_file_comment` | 116 | 3 | insn:3 |
| 120 | `_Z20keyReleasedWithDelayPN11AbyssEngine6EngineEi` | 36 | 2 | pool .bss:2 |
| 121 | `_Z21SendStoredKeyUpEventsPN11AbyssEngine6EngineE` | 60 | 2 | pool .bss:2 |
| 122 | `_ZN10ModStation10OnKeyPressExx` | 2516 | 2 | pool .bss:1, pool .data:1 |
| 123 | `_ZN10ModStation11checkMedalsEv` | 468 | 2 | pool .rodata:2 |
| 124 | `_ZN10ModStation21resetIdleCamForHangarEv` | 360 | 2 | pool .rodata:2 |
| 125 | `_ZN11AbyssEngine10BlurShader4InitEPNS_6EngineE` | 188 | 2 | pool .rodata:2 |
| 126 | `_ZN11AbyssEngine10BumpShader4InitEPNS_6EngineE` | 476 | 2 | pool .rodata:2 |
| 127 | `_ZN11AbyssEngine10GlowShader4InitEPNS_6EngineE` | 140 | 2 | pool .rodata:2 |
| 128 | `_ZN11AbyssEngine10MaskShader4InitEPNS_6EngineE` | 196 | 2 | pool .rodata:2 |
| 129 | `_ZN11AbyssEngine11BumpMapping4InitEPNS_6EngineE` | 216 | 2 | pool .rodata:2 |
| 130 | `_ZN11AbyssEngine11ColorMixAdd4InitEPNS_6EngineE` | 188 | 2 | pool .rodata:2 |
| 131 | `_ZN11AbyssEngine11CubeMapping4InitEPNS_6EngineE` | 312 | 2 | pool .rodata:2 |
| 132 | `_ZN11AbyssEngine11GreenShader4InitEPNS_6EngineE` | 288 | 2 | pool .rodata:2 |
| 133 | `_ZN11AbyssEngine11NoTexShader4InitEPNS_6EngineE` | 92 | 2 | pool .rodata:2 |
| 134 | `_ZN11AbyssEngine11PulseShader4InitEPNS_6EngineE` | 304 | 2 | pool .rodata:2 |
| 135 | `_ZN11AbyssEngine12BumpShaderV24InitEPNS_6EngineE` | 324 | 2 | pool .rodata:2 |
| 136 | `_ZN11AbyssEngine12BumpShaderV34InitEPNS_6EngineE` | 484 | 2 | pool .rodata:2 |
| 137 | `_ZN11AbyssEngine12BumpShaderV44InitEPNS_6EngineE` | 324 | 2 | pool .rodata:2 |
| 138 | `_ZN11AbyssEngine12EnergyShield4InitEPNS_6EngineE` | 340 | 2 | pool .rodata:2 |
| 139 | `_ZN11AbyssEngine12PostBWShader4InitEPNS_6EngineE` | 116 | 2 | pool .rodata:2 |
| 140 | `_ZN11AbyssEngine12SimpleShader4InitEPNS_6EngineE` | 92 | 2 | pool .rodata:2 |
| 141 | `_ZN11AbyssEngine13DrawFBOShader4InitEPNS_6EngineE` | 116 | 2 | pool .rodata:2 |
| 142 | `_ZN11AbyssEngine13GenericShader4InitEPNS_6EngineE` | 356 | 2 | pool .rodata:2 |
| 143 | `_ZN11AbyssEngine13SandboxShader4InitEPNS_6EngineE` | 264 | 2 | pool .rodata:2 |
| 144 | `_ZN11AbyssEngine13TexOnlyShader4InitEPNS_6EngineE` | 116 | 2 | pool .rodata:2 |
| 145 | `_ZN11AbyssEngine14GenericShader14InitEPNS_6EngineE` | 288 | 2 | pool .rodata:2 |
| 146 | `_ZN11AbyssEngine14GenericShader24InitEPNS_6EngineE` | 288 | 2 | pool .rodata:2 |
| 147 | `_ZN11AbyssEngine15BumpShaderCloak4InitEPNS_6EngineE` | 540 | 2 | pool .rodata:2 |
| 148 | `_ZN11AbyssEngine15SpecCubeMapping4InitEPNS_6EngineE` | 340 | 2 | pool .rodata:2 |
| 149 | `_ZN11AbyssEngine17BumpRimCubeShader4InitEPNS_6EngineE` | 620 | 2 | pool .rodata:2 |
| 150 | `_ZN11AbyssEngine17BumpShaderRefract4InitEPNS_6EngineE` | 292 | 2 | pool .rodata:2 |
| 151 | `_ZN11AbyssEngine17CubeNormalMapping4InitEPNS_6EngineE` | 376 | 2 | pool .rodata:2 |
| 152 | `_ZN11AbyssEngine17TextureConference4InitEPNS_6EngineE` | 156 | 2 | pool .rodata:2 |
| 153 | `_ZN11AbyssEngine17VertexColorShader4InitEPNS_6EngineE` | 304 | 2 | pool .rodata:2 |
| 154 | `_ZN11AbyssEngine18BumpShaderParticle4InitEPNS_6EngineE` | 388 | 2 | pool .rodata:2 |
| 155 | `_ZN11AbyssEngine18TextureLightShader4InitEPNS_6EngineE` | 392 | 2 | pool .rodata:2 |
| 156 | `_ZN11AbyssEngine19NoTexVtxColorShader4InitEPNS_6EngineE` | 108 | 2 | pool .rodata:2 |
| 157 | `_ZN11AbyssEngine20SpecCubeAlphaMapping4InitEPNS_6EngineE` | 340 | 2 | pool .rodata:2 |
| 158 | `_ZN11AbyssEngine21BumpRimCubeShader_new4InitEPNS_6EngineE` | 620 | 2 | pool .rodata:2 |
| 159 | `_ZN11AbyssEngine22SimpleRefractionShader4InitEPNS_6EngineE` | 340 | 2 | pool .rodata:2 |
| 160 | `_ZN11AbyssEngine29VertexColorAlphaTextureShader4InitEPNS_6EngineE` | 304 | 2 | pool .rodata:2 |
| 161 | `_ZN11AbyssEngine9DNSShader4InitEPNS_6EngineE` | 356 | 2 | pool .rodata:2 |
| 162 | `_ZN11SpaceLounge10OnTouchEndEii` | 7448 | 2 | pool .rodata:2 |
| 163 | `_ZN13AutoPilotListC1EP5Level` | 760 | 2 | pool .rodata:2 |
| 164 | `_ZN13AutoPilotListC2EP5Level` | 760 | 2 | pool .rodata:2 |
| 165 | `_ZN14DialogueWindow6lengthEv` | 104 | 2 | pool .rodata:2 |
| 166 | `_ZN17PlayerFixedObjectC1EiiP6PlayerP10AEGeometryfff` | 756 | 2 | pool .rodata:2 |
| 167 | `_ZN17PlayerFixedObjectC2EiiP6PlayerP10AEGeometryfff` | 756 | 2 | pool .rodata:2 |
| 168 | `_ZN5Level11createSpaceEv` | 3076 | 2 | pool .rodata:2 |
| 169 | `_ZN5Level17getBoundingVolumeEiP10AEGeometry` | 536 | 2 | insn:2 |
| 170 | `_ZN5MGame12OnTouchBeginEiiPv` | 2936 | 2 | pool .data:1, pool .rodata:1 |
| 171 | `_ZN5Radar12calcDistanceEffffff` | 624 | 2 | pool .rodata:2 |
| 172 | `_ZN5Radar15drawCurrentLockEP3Hud` | 1680 | 2 | pool .rodata:2 |
| 173 | `_ZN6GalaxyD1Ev` | 46 | 2 | insn:2 |
| 174 | `_ZN6GalaxyD2Ev` | 46 | 2 | insn:2 |
| 175 | `_ZN6Player14playShootSoundEiiPN11AbyssEngine6AEMath6VectorEf` | 188 | 2 | pool .rodata:2 |
| 176 | `_ZN6Player16calcWeaponSoundsEi` | 412 | 2 | pool .rodata:2 |
| 177 | `_ZN6Status26getGammaRayDamagePerSecondEii` | 88 | 2 | pool .rodata:2 |
| 178 | `_ZN7Globals18getDialogueSoundIdEiP5Agent` | 1724 | 2 | pool .rodata:2 |
| 179 | `_ZN7Globals26playMusicAndFadeOutCurrentEi` | 588 | 2 | pool .rodata:2 |
| 180 | `_ZN9FModSound4initEv` | 332 | 2 | pool .rodata:2 |
| 181 | `_ZN9Generator14getItemBuyListEP7Station` | 1840 | 2 | pool .rodata:2 |
| 182 | `_ZN9ObjectGunC1EiP3GunijP5Level` | 516 | 2 | pool .rodata:2 |
| 183 | `_ZN9ObjectGunC2EiP3GunijP5Level` | 516 | 2 | pool .rodata:2 |
| 184 | `_ZNSt10bad_typeidD0Ev` | 16 | 2 | insn:2 |
| 185 | `_ZNSt8bad_castD0Ev` | 16 | 2 | insn:2 |
| 186 | `__cxa_allocate_dependent_exception` | 30 | 2 | insn:2 |
| 187 | `__cxa_decrement_exception_refcount` | 64 | 2 | insn:2 |
| 188 | `__cxa_deleted_virtual` | 16 | 2 | insn:1, pool .rodata:1 |
| 189 | `__cxa_pure_virtual` | 16 | 2 | insn:1, pool .rodata:1 |
| 190 | `__cxa_uncaught_exceptions` | 22 | 2 | insn:2 |
| 191 | `_tr_tally` | 172 | 2 | pool .got:2 |
| 192 | `_zip_cdir_free` | 76 | 2 | insn:1, pool .got:1 |
| 193 | `_zip_cdir_grow` | 68 | 2 | insn:2 |
| 194 | `adler32_combine` | 120 | 2 | insn:2 |
| 195 | `crc32_combine` | 208 | 2 | insn:1, pool .got:1 |
| 196 | `deflateParams` | 148 | 2 | insn:1, pool .data.rel.ro:1 |
| 197 | `deflateSetDictionary` | 194 | 2 | insn:2 |
| 198 | `inflateBackInit_` | 148 | 2 | pool .got:2 |
| 199 | `zip_add` | 36 | 2 | insn:2 |
| 200 | `zip_delete` | 72 | 2 | insn:2 |
| 201 | `zip_error_get_sys_type` | 44 | 2 | pool .got:2 |
| 202 | `zip_fopen` | 34 | 2 | insn:2 |
| 203 | `zip_replace` | 48 | 2 | insn:2 |
| 204 | `zip_source_file` | 52 | 2 | insn:2 |
| 205 | `zip_source_filep` | 54 | 2 | insn:2 |
| 206 | `zip_source_function` | 58 | 2 | insn:2 |
| 207 | `zip_stat` | 52 | 2 | insn:2 |
| 208 | `zip_stat_index` | 202 | 2 | insn:2 |
| 209 | `zip_unchange_all` | 50 | 2 | insn:2 |
| 210 | `SHA224_Final` | 4 | 1 | insn:1 |
| 211 | `SHA224_Init` | 132 | 1 | insn:1 |
| 212 | `SHA224_Update` | 4 | 1 | insn:1 |
| 213 | `SHA256_Init` | 132 | 1 | insn:1 |
| 214 | `SHA256_Transform` | 8 | 1 | insn:1 |
| 215 | `_Z10MouseInputii` | 84 | 1 | pool .bss:1 |
| 216 | `_Z10SlowMotionv` | 12 | 1 | pool .bss:1 |
| 217 | `_Z11UseJoysticki` | 12 | 1 | pool .bss:1 |
| 218 | `_Z11keyReleasedPN11AbyssEngine6EngineEi` | 396 | 1 | pool .bss:1 |
| 219 | `_Z12CaptureMousei` | 12 | 1 | pool .bss:1 |
| 220 | `_Z12keyIsPressedv` | 52 | 1 | pool .bss:1 |
| 221 | `_Z14GetUseJoystickv` | 12 | 1 | pool .bss:1 |
| 222 | `_Z7SpeedUpv` | 12 | 1 | pool .bss:1 |
| 223 | `_Z9HideMousev` | 12 | 1 | pool .bss:1 |
| 224 | `_ZN10ModStation10checkHintsEv` | 2748 | 1 | pool .data:1 |
| 225 | `_ZN10StarSystem26switchSunForSupernovaIntroEv` | 292 | 1 | pool .rodata:1 |
| 226 | `_ZN11AbyssEngine6Engine12DoPostEffectEv` | 340 | 1 | pool .bss:1 |
| 227 | `_ZN11AbyssEngine6Engine13LightSetLightEj` | 160 | 1 | pool .rodata:1 |
| 228 | `_ZN11SolarSystem13getAttackRaceEv` | 24 | 1 | pool .rodata:1 |
| 229 | `_ZN11SolarSystem13hasPirateBaseEv` | 76 | 1 | pool .rodata:1 |
| 230 | `_ZN11SolarSystem18hasHiddenBlueprintEv` | 76 | 1 | pool .rodata:1 |
| 231 | `_ZN11SpaceLounge9startChatEv` | 14450 | 1 | pool .rodata:1 |
| 232 | `_ZN11TouchButton11setPositionEii` | 8 | 1 | insn:1 |
| 233 | `_ZN11TouchButton12setYPositionEi` | 12 | 1 | insn:1 |
| 234 | `_ZN11TouchButton7setTextERKN11AbyssEngine6StringE` | 248 | 1 | insn:1 |
| 235 | `_ZN12Achievements16checkForNewMedalEP9PlayerEgo` | 1168 | 1 | pool .rodata:1 |
| 236 | `_ZN12Achievements8getValueEii` | 28 | 1 | pool .rodata:1 |
| 237 | `_ZN12ChoiceWindow13setMiscButtonERKN11AbyssEngine6StringE` | 332 | 1 | insn:1 |
| 238 | `_ZN12ChoiceWindow6updateEi` | 6 | 1 | insn:1 |
| 239 | `_ZN12ChoiceWindow9setHeightEi` | 200 | 1 | insn:1 |
| 240 | `_ZN12HangarWindow10OnTouchEndEii` | 5828 | 1 | pool .rodata:1 |
| 241 | `_ZN12ImageFactory10createCharEbi` | 128 | 1 | pool .rodata:1 |
| 242 | `_ZN12ImageFactory9loadImageEiii` | 268 | 1 | pool .rodata:1 |
| 243 | `_ZN12ImageFactoryC1Ev` | 132 | 1 | pool .rodata:1 |
| 244 | `_ZN12ImageFactoryC2Ev` | 132 | 1 | pool .rodata:1 |
| 245 | `_ZN12StatusWindow6reInitEv` | 288 | 1 | pool .bss:1 |
| 246 | `_ZN13PlayerFighter6updateEi` | 10376 | 1 | pool .rodata:1 |
| 247 | `_ZN13PlayerFighterC1EiiP6PlayerP10AEGeometryfffb` | 1324 | 1 | pool .rodata:1 |
| 248 | `_ZN13PlayerFighterC2EiiP6PlayerP10AEGeometryfffb` | 1324 | 1 | pool .rodata:1 |
| 249 | `_ZN13RecordHandler16addHashToOptionsEv` | 256 | 1 | pool .rodata:1 |
| 250 | `_ZN13RecordHandler7addHashEi` | 276 | 1 | pool .rodata:1 |
| 251 | `_ZN13RecordHandler9checkHashEj` | 280 | 1 | pool .rodata:1 |
| 252 | `_ZN14DialogueWindow18hasSuccessDialogueEi` | 32 | 1 | pool .rodata:1 |
| 253 | `_ZN14DialogueWindow19hasBriefingDialogueEi` | 32 | 1 | pool .rodata:1 |
| 254 | `_ZN14DialogueWindowC1EPN11AbyssEngine6StringES2_Pi` | 376 | 1 | pool .rodata:1 |
| 255 | `_ZN14DialogueWindowC2EPN11AbyssEngine6StringES2_Pi` | 376 | 1 | pool .rodata:1 |
| 256 | `_ZN14ListItemWindow6updateEi` | 352 | 1 | pool .rodata:1 |
| 257 | `_ZN14MissionsWindow4initEv` | 2468 | 1 | pool .rodata:1 |
| 258 | `_ZN14MissionsWindow6updateEi` | 556 | 1 | pool .rodata:1 |
| 259 | `_ZN14PlayerAsteroid16getQualityStringEv` | 60 | 1 | pool .data.rel.ro:1 |
| 260 | `_ZN14PlayerCreature4rageEi` | 80 | 1 | pool .rodata:1 |
| 261 | `_ZN14PlayerCreature9getWeightEv` | 20 | 1 | pool .rodata:1 |
| 262 | `_ZN14PlayerCreatureC1EiiP6PlayerP10AEGeometryfff` | 204 | 1 | pool .rodata:1 |
| 263 | `_ZN14PlayerCreatureC2EiiP6PlayerP10AEGeometryfff` | 204 | 1 | pool .rodata:1 |
| 264 | `_ZN15MenuTouchWindow4drawEv` | 13052 | 1 | insn:1 |
| 265 | `_ZN3Gun8setIndexEi` | 272 | 1 | pool .rodata:1 |
| 266 | `_ZN3Gun9setOffsetEii` | 136 | 1 | pool .rodata:1 |
| 267 | `_ZN3Hud13hudEventMedalEii` | 448 | 1 | pool .rodata:1 |
| 268 | `_ZN3Hud20drawOrbitInformationEv` | 608 | 1 | pool .rodata:1 |
| 269 | `_ZN3Hud4initEv` | 3464 | 1 | pool .rodata:1 |
| 270 | `_ZN4Item27canBeInstalledMultipleTimesEv` | 16 | 1 | pool .rodata:1 |
| 271 | `_ZN4Item8isInListEPS_P5ArrayIS0_E` | 6 | 1 | insn:1 |
| 272 | `_ZN4Item8isInListEiP5ArrayIPS_E` | 8 | 1 | insn:1 |
| 273 | `_ZN4Ship11adjustPriceEv` | 192 | 1 | pool .rodata:1 |
| 274 | `_ZN4Ship8addCargoEP4Item` | 62 | 1 | insn:1 |
| 275 | `_ZN4Ship8addCargoEP5ArrayIP4ItemE` | 24 | 1 | insn:1 |
| 276 | `_ZN5Agent21getModPricePercentageEv` | 28 | 1 | pool .rodata:1 |
| 277 | `_ZN5Level10assignGunsEv` | 2948 | 1 | pool .rodata:1 |
| 278 | `_ZN5MGame12successCheckEv` | 1468 | 1 | insn:1 |
| 279 | `_ZN5MGame13gameOverCheckEv` | 696 | 1 | insn:1 |
| 280 | `_ZN5MGame14startJumpSceneEv` | 896 | 1 | insn:1 |
| 281 | `_ZN6AEFile11crc32_ccittERKN11AbyssEngine6StringE` | 68 | 1 | pool .rodata:1 |
| 282 | `_ZN6Player14stopShootSoundEii` | 80 | 1 | pool .rodata:1 |
| 283 | `_ZN6Status15checkForLevelUpEv` | 100 | 1 | pool .rodata:1 |
| 284 | `_ZN6Status19nextCampaignMissionEb` | 8492 | 1 | pool .rodata:1 |
| 285 | `_ZN7BeamGun8setEnemyEP6Player` | 6 | 1 | insn:1 |
| 286 | `_ZN7BeamGunC1EiP3GuniP5Level` | 224 | 1 | pool .rodata:1 |
| 287 | `_ZN7BeamGunC2EiP3GuniP5Level` | 224 | 1 | pool .rodata:1 |
| 288 | `_ZN7Globals21getRandomEnemyFighterEi` | 176 | 1 | pool .rodata:1 |
| 289 | `_ZN7StarMap10OnTouchEndEii` | 2972 | 1 | pool .rodata:1 |
| 290 | `_ZN7StarMap4initEbP7Missionbi` | 2220 | 1 | pool .rodata:1 |
| 291 | `_ZN7Station20stationHasPirateBaseEv` | 64 | 1 | pool .rodata:1 |
| 292 | `_ZN7Station21getPirateStationIndexEv` | 40 | 1 | pool .rodata:1 |
| 293 | `_ZN7Station23getHiddenBlueprintIndexEv` | 40 | 1 | pool .rodata:1 |
| 294 | `_ZN7Station25stationHasHiddenBlueprintEb` | 68 | 1 | pool .rodata:1 |
| 295 | `_ZN8CutScene17replacePlayerShipEii` | 288 | 1 | pool .rodata:1 |
| 296 | `_ZN8GameText17isNonArabicStringEPKtj` | 92 | 1 | pool .rodata:1 |
| 297 | `_ZN8Standing12getEnemyRaceEi` | 24 | 1 | pool .rodata:1 |
| 298 | `_ZN8Standing9applyKillEi` | 108 | 1 | pool .rodata:1 |
| 299 | `_ZN9FModSound13disableReverbEv` | 92 | 1 | pool .rodata:1 |
| 300 | `_ZN9FModSound13freeAllEventsEv` | 260 | 1 | pool .rodata:1 |
| 301 | `_ZN9Generator11getLootListEii` | 500 | 1 | pool .rodata:1 |
| 302 | `_ZN9Generator18isKaamoSpecialItemEi` | 36 | 1 | pool .rodata:1 |
| 303 | `_ZN9Generator20generateStationIndexEP5ArrayIP11SolarSystemEi` | 344 | 1 | pool .rodata:1 |
| 304 | `_ZN9ObjectGun6updateEi` | 1092 | 1 | pool .rodata:1 |
| 305 | `_ZN9PlayerEgo20approachDockingPointEP3HudiP5Radar` | 2532 | 1 | pool .rodata:1 |
| 306 | `_ZN9PlayerEgo6updateEiP5RadarP3HudP5RadioP11LevelScriptibi` | 8852 | 1 | pool .rodata:1 |
| 307 | `_ZN9SpriteGun6updateEi` | 6 | 1 | insn:1 |
| 308 | `_ZN9SpriteGun8setEnemyEP6Player` | 6 | 1 | insn:1 |
| 309 | `_ZNKSt10bad_typeid4whatEv` | 12 | 1 | pool .rodata:1 |
| 310 | `_ZNKSt13bad_exception4whatEv` | 12 | 1 | pool .rodata:1 |
| 311 | `_ZNKSt16bad_array_length4whatEv` | 12 | 1 | pool .rodata:1 |
| 312 | `_ZNKSt20bad_array_new_length4whatEv` | 12 | 1 | pool .rodata:1 |
| 313 | `_ZNKSt8bad_cast4whatEv` | 12 | 1 | pool .rodata:1 |
| 314 | `_ZNKSt9bad_alloc4whatEv` | 12 | 1 | pool .rodata:1 |
| 315 | `_ZNKSt9exception4whatEv` | 12 | 1 | pool .rodata:1 |
| 316 | `_ZNSt10bad_typeidC1Ev` | 16 | 1 | pool .got:1 |
| 317 | `_ZNSt10bad_typeidC2Ev` | 16 | 1 | pool .got:1 |
| 318 | `_ZNSt10bad_typeidD1Ev` | 4 | 1 | insn:1 |
| 319 | `_ZNSt10bad_typeidD2Ev` | 4 | 1 | insn:1 |
| 320 | `_ZNSt13bad_exceptionD0Ev` | 4 | 1 | insn:1 |
| 321 | `_ZNSt16bad_array_lengthC1Ev` | 16 | 1 | pool .got:1 |
| 322 | `_ZNSt16bad_array_lengthC2Ev` | 16 | 1 | pool .got:1 |
| 323 | `_ZNSt16bad_array_lengthD0Ev` | 4 | 1 | insn:1 |
| 324 | `_ZNSt20bad_array_new_lengthC1Ev` | 16 | 1 | pool .got:1 |
| 325 | `_ZNSt20bad_array_new_lengthC2Ev` | 16 | 1 | pool .got:1 |
| 326 | `_ZNSt20bad_array_new_lengthD0Ev` | 4 | 1 | insn:1 |
| 327 | `_ZNSt8bad_castC1Ev` | 16 | 1 | pool .got:1 |
| 328 | `_ZNSt8bad_castC2Ev` | 16 | 1 | pool .got:1 |
| 329 | `_ZNSt8bad_castD1Ev` | 4 | 1 | insn:1 |
| 330 | `_ZNSt8bad_castD2Ev` | 4 | 1 | insn:1 |
| 331 | `_ZNSt9bad_allocC1Ev` | 16 | 1 | pool .got:1 |
| 332 | `_ZNSt9bad_allocC2Ev` | 16 | 1 | pool .got:1 |
| 333 | `_ZNSt9bad_allocD0Ev` | 4 | 1 | insn:1 |
| 334 | `_ZNSt9exceptionD0Ev` | 4 | 1 | insn:1 |
| 335 | `_ZNSt9type_infoD0Ev` | 4 | 1 | insn:1 |
| 336 | `_ZSt10unexpectedv` | 24 | 1 | pool .got:1 |
| 337 | `_ZSt13get_terminatev` | 20 | 1 | pool .got:1 |
| 338 | `_ZSt13set_terminatePFvvE` | 48 | 1 | pool .got:1 |
| 339 | `_ZSt14get_unexpectedv` | 20 | 1 | pool .got:1 |
| 340 | `_ZSt14set_unexpectedPFvvE` | 48 | 1 | pool .got:1 |
| 341 | `_ZSt15get_new_handlerv` | 20 | 1 | pool .got:1 |
| 342 | `_ZSt15set_new_handlerPFvvE` | 36 | 1 | pool .got:1 |
| 343 | `_ZdaPv` | 4 | 1 | insn:1 |
| 344 | `_ZdaPvRKSt9nothrow_t` | 4 | 1 | insn:1 |
| 345 | `_ZdaPvSt11align_val_t` | 4 | 1 | insn:1 |
| 346 | `_ZdaPvSt11align_val_tRKSt9nothrow_t` | 4 | 1 | insn:1 |
| 347 | `_ZdaPvj` | 4 | 1 | insn:1 |
| 348 | `_ZdaPvjSt11align_val_t` | 6 | 1 | insn:1 |
| 349 | `_ZdlPv` | 4 | 1 | insn:1 |
| 350 | `_ZdlPvRKSt9nothrow_t` | 4 | 1 | insn:1 |
| 351 | `_ZdlPvSt11align_val_t` | 4 | 1 | insn:1 |
| 352 | `_ZdlPvSt11align_val_tRKSt9nothrow_t` | 4 | 1 | insn:1 |
| 353 | `_ZdlPvj` | 4 | 1 | insn:1 |
| 354 | `_ZdlPvjSt11align_val_t` | 6 | 1 | insn:1 |
| 355 | `_Znaj` | 4 | 1 | insn:1 |
| 356 | `_ZnajSt11align_val_t` | 4 | 1 | insn:1 |
| 357 | `__cxa_current_exception_type` | 56 | 1 | insn:1 |
| 358 | `__cxa_free_exception` | 16 | 1 | insn:1 |
| 359 | `__cxa_uncaught_exception` | 16 | 1 | insn:1 |
| 360 | `_tr_init` | 76 | 1 | pool .data:1 |
| 361 | `_zip_dirent_finalize` | 48 | 1 | pool .got:1 |
| 362 | `_zip_error_fini` | 18 | 1 | insn:1 |
| 363 | `_zip_error_get` | 34 | 1 | insn:1 |
| 364 | `_zip_get_name` | 90 | 1 | insn:1 |
| 365 | `_zip_unchange_data` | 44 | 1 | insn:1 |
| 366 | `compress` | 20 | 1 | insn:1 |
| 367 | `crc32` | 644 | 1 | pool .rodata:1 |
| 368 | `deflateBound` | 40 | 1 | insn:1 |
| 369 | `deflateInit_` | 32 | 1 | insn:1 |
| 370 | `get_crc_table` | 12 | 1 | pool .rodata:1 |
| 371 | `inflateInit_` | 10 | 1 | insn:1 |
| 372 | `ndk23_Init` | 296 | 1 | pool .bss:1 |
| 373 | `ndk23_InitWithZip` | 300 | 1 | pool .bss:1 |
| 374 | `zError` | 20 | 1 | pool .got:1 |
| 375 | `zcalloc` | 8 | 1 | insn:1 |
| 376 | `zcfree` | 6 | 1 | insn:1 |
| 377 | `zip_error_clear` | 6 | 1 | insn:1 |
| 378 | `zip_error_get` | 6 | 1 | insn:1 |
| 379 | `zip_file_error_clear` | 6 | 1 | insn:1 |
| 380 | `zip_file_error_get` | 6 | 1 | insn:1 |
| 381 | `zip_file_strerror` | 6 | 1 | insn:1 |
| 382 | `zip_get_file_comment` | 96 | 1 | insn:1 |
| 383 | `zip_get_name` | 8 | 1 | insn:1 |
| 384 | `zip_name_locate` | 8 | 1 | insn:1 |
| 385 | `zip_source_free` | 32 | 1 | insn:1 |
| 386 | `zip_strerror` | 6 | 1 | insn:1 |
| 387 | `zip_unchange` | 6 | 1 | insn:1 |
| 388 | `zip_unchange_archive` | 30 | 1 | insn:1 |
| 389 | `zlibVersion` | 12 | 1 | pool .rodata:1 |
