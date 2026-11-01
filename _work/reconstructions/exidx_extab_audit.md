# `.ARM.exidx` / `.ARM.extab` / section-size forensics (w3367)

Scope: explain the 2.0.16 section-size deltas, fix what is fixable, and leave a
reproducible per-entry diff. Worktree `wt/w3367`, branch `wave87-w3367`.

## Result

| section | original | before (HEAD `d1295b95`) | after | delta after |
|---|---:|---:|---:|---:|
| `.text` | `0x1337c8` | `0x1337f8` (+0x30) | **`0x1337c8`** | **0** |
| `.ARM.exidx` | `0x06f70` | `0x06f50` (−0x20) | `0x06f80` | +0x10 |
| `.ARM.extab` | `0x26298` | `0x2627c` (−0x1c) | `0x2627c` | −0x1c |
| `.rodata` | `0x41b67` @ `0x1c9e10` | `0x41b97` @ `0x1c9e10` | `0x41b97` @ `0x1c9e10` | +0x30 (base exact) |
| `.bss` | `0x069ac` @ `0x216970` | `0x069c8` @ `0x216980` | `0x069c8` @ `0x216980` | +0x1c |

Gate: **byte_exact 2923 → 3885 (+962)**, linked_exact 4494 (unchanged), avg 99.89,
`RATCHET PASS`, `drift` 0 fields.

The single largest lever was **making `.text` exactly `0x1337c8`**: the function
addresses then line up and 962 functions become byte-identical. The `.rodata` base
is unchanged because `.text`/`.exidx`/`.extab` are constrained by the 16-byte
alignment of `.rodata` (see "Coupling" below).

## Method / tooling

`readelf --unwind` does work on this ARM ELF, but the per-entry analysis needs the
raw prel31 words, so a small parser is used (`/tmp/w3367/elf_exidx.py`):

* parse `.ARM.exidx` as `(prel31 fn, word1)` 8-byte entries;
* `word1 == 1` → `EXIDX_CANTUNWIND` ("CANT");
* `word1 & 0x80000000` → inline compact model, `(word1>>24)&0xf` is the model;
* else → `prel31` to `.ARM.extab`; the first word there is either a compact
  personality+LSDA header (`0x8...`) or a prel31 personality pointer.

Every entry is attributed to the greatest `.dynsym` function address `<= fn`, and
the resulting `(function, offset, kind)` multiset is diffed between the two
binaries. `.ARM.extab` record lengths are recovered as the distance between
consecutive extab targets (records are packed in address order).

Commands (all from the worktree root):

```sh
python3 /tmp/w3367/elf_exidx.py ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
python3 /tmp/w3367/diff_exidx2.py        # per-function kind-sequence diff
python3 /tmp/w3367/extab_len.py          # per-function LSDA record lengths
python3 /tmp/rodata_diff.py              # first .rodata byte divergence
python3 /tmp/w3367/bss_diff.py           # exported .bss object offsets
cmake --build cmake-build-match --target check -j8
```

## 1. How gold decides the exidx entries (the key model)

Gold's `fix_exidx_coverage` emits one `CANTUNWIND` for every text input section
that has no `.ARM.exidx` companion, then **merges consecutive entries with
identical content**. Two facts follow, and both were verified:

1. The output contains **zero** consecutive identical entries in either binary
   (`/tmp/merge_check.py`), even though a CANT followed by an inline
   `0x80b0b0b0` is common.
2. A CANT therefore *protects* a following identical inline entry from being
   merged. Example: at `0x69588` the original has
   `[BackButtonPressed INL0][CANT][NFC::C1 INL0]`; with BackButton's empty
   `.text` dropped, ours collapses to `[BackButtonPressed INL0]` and the NFC
   ctor's entry disappears entirely. Re-enabling the marker restores **both**
   the CANT and the INL entry.

This is why CANT and inline counts move together, and why the `DROP_EMPTY_TEXT`
set is a size knob, not just a CANT knob.

Our per-TU empty `.text` sections are renamed to `.text.tuend.oNNN`
(`tools/gen_exidx_markers.py`) and placed by `tools/gen_section_order.py` after
the owner's last `.text.*` section, i.e. exactly at the original TU boundary
(the original emits each TU's empty `.text` at the *end* of that TU).

## 2. Per-entry diff

Final counts:

| | original | ours |
|---|---:|---:|
| total entries | 3566 | 3568 |
| CANTUNWIND | 272 | **272** |
| inline compact (all model 0) | 856 | 857 |
| `.ARM.extab` refs | 2438 | 2439 |
| extab compact model 1 | 1841 | 1842 |
| extab prel31 personality | 597 | 597 |

54 functions still have a different entry-kind sequence. They fall into three
groups:

**(a) CANT position shifted by a code-size delta (same count).** The zip TU is
the bulk: `_zip_entry_new`, `_zip_new`, `zip_fclose`, `_zip_cdir_free`,
`zip_rename`, `zip_source_zip`, `zip_stat_index`, `zip_unchange_all`, … The
CANT is at the same TU boundary in both, but the preceding local function is
2–16 bytes smaller in ours, so the entry's `fn` address (and the attributed
`prev+offset`) differs. Example:

```
orig  #3185 0018ebee cant   zip_fclose+110
ours  #3182 0018ebe0 cant   _zip_cdir_free+0     # zip_fclose is 16 B shorter
```

**(b) Genuinely missing CANT+inline pairs (2).** Both are still in
`DROP_EMPTY_TEXT`:

| address | original | ours |
|---|---|---|
| `0x69588` | `CANT + INL0` (`NFC::C1`) | `INL0` merged away |
| `0x6b9d8` | `CANT + INL0` (`GOF2HD2012::GetLeaderboardScore`) | `INL0` merged away |

Enabling `BackButton.c.obj` / `GOF2HD2012.c.obj` restores them but adds +0x20
of exidx, which would move the `.rodata` base (see "Coupling").

**(c) Real code differences (extra/missing landing pads).**

| function | original | ours | cause |
|---|---|---|---|
| `ConfigReader::GetNewLine` | `EXTP` | `EXTP + INL0@276` | our body has an extra landing pad at the tail |
| `MenuTouchWindow::C1` | `CANT + EXTP` | `CANT + INL0 + EXTP` | extra inline landing pad |
| `Layout::C1` | `CANT + EXT1` (size 7072) | `CANT + EXT1 + INL0@6800` (size 6800) | our `Layout::C1` is 272 B short |
| `RocketGun::translate` | `INL0@0, CANT@2` | now identical | the extra `INL0@2` was `marqueeImageByteSum` (removed — see §6) |
| `__cxa_deleted_virtual` | `CANT@604` | (no CANT) | merge-break difference in libc++abi |
| `__dynamic_cast` | `… CANT@1506` | (no CANT) | tail CANT merged away |

**(d) Spurious CANTs (2).** `loadPortraits+0` and
`ResourceTextureC2(String)+0` have a CANT in ours but not in the original.
Cause: `tools/gen_section_order.py` places a TU's marker after `max(idxs)` of
the owner's sections, and `BuildResourceList.cpp.obj` also carries COMDAT
copies of `ResourceTextureC2`/`ResourceMaterialC2` that gold places at
`ResourceMaterial.cpp`'s position. The marker therefore lands in the middle of
`ResourceMaterial`'s block. Fixing it needs a marker-placement rule that
ignores COMDAT sections placed outside the owner's own run; a first attempt
(`min(idxs)` for all markers) collapsed the exidx to 0x6bb8 and is not usable.

## 3. `.ARM.extab` — fully accounted for

All extab records are compact model 1 in both binaries; there is **no**
compact-model 0/2/3 difference. The −0x1c is exactly the sum of nine functions'
LSDA record-length deltas:

| function | original | ours | Δ |
|---|---:|---:|---:|
| `HangarWindow::render` | 948 | 912 | −36 |
| `Hud::draw` | 476 | 492 | +16 |
| `ConfigReader::RegisterTokenReadFunction` | 28+32+104+32 | 28+28+96+28 | −16 |
| `RecordHandler::recordStoreWrite` | 656 | 640 | −16 |
| `StarMap::draw` | 116 | 128 | +12 |
| `__cxa_demangle` (local helpers) | … | … | +12 |
| `Level::createShip` | 336 | 328 | −8 |
| `Generator::createMission` | 172 | 176 | +4 |
| `LevelScript::process` | 348 | 352 | +4 |
| **total** | | | **−28 = −0x1c** |

These are body/codegen differences (call-site table entries), not a table-format
choice. They are the same functions that are still not byte-exact.

## 4. `.rodata` +0x30

`.rodata` starts at the exact original address `0x1c9e10`; only its size is
+0x30. The first byte divergence is at offset `0x1193`:

```
orig: purge\0pause\0FMOD_GOF2\0master\0music\0sfx\0voice\0english\0deutsch\0
ours: purge\0pause\0master\0music\0sfx\0voice\0english\0deutsch\0FMOD_GOF2\0
```

`FMOD_GOF2` is the literal in `FModSound::freeAllEvents`; `master..voice` are
`kCats` in `init`. The original emitted `freeAllEvents` before `init`. Moving it
(staged patch `/tmp/FModSound_moved.cpp`) makes the 0x3d-byte section
byte-identical, **but** gold's PLT order follows object section order and the
reorder costs 10 byte_exact (`3885 → 3875`) at the current layout, so it is not
committed. It does not change the `.rodata` size.

The +0x30 itself is the 12 per-TU literal/table differences already listed in
`data_layout_audit_2.md` (`VertexColorAlphaTextureShader` 0x56f, `Generator`
0x248, `Status` 0x154, `Level` 0x1414, `LevelCreateScene` 0x480, `StarMap`
0x22c, `ModStation` 0x1b9, `ImageFactory` 0x320, `LensFlare` 0x3c,
`MenuTouchWindow` 0xa4, `AutoPilotList` 5, `DialogueWindow` 0x2904).

## 5. `.bss` +0x1c

Exported objects are 843/843 exact in content, so the +0x1c is local statics.
The original `.bss` is **not** in object order: `g_pActivity @0`, `stackX
@0x1010`, `ndk_APK_Path @0x1034`, `gi_iap_* @0x1044`, `FileInterfaceAndroid
@0x106c`, `AEFile @0x1090`, … `ParticleSettingsRef::cur/init @0x1a78/0x1e78`,
`S @0x5908`, `P @0x6908`, COMMON block last `@0x6950`. Ours puts `** common`
first (`@0x216980`), `g_pActivity @0x2169dc`, `stackX @0x2169e0`, `S
@0x217a10`, `P @0x218a10`.

Two independent causes, both already documented in `wholefile_audit.md` §4.2:
the trailing `*` wildcard makes gold place `** common` before wildcard-matched
`.bss`, and the original `.bss` link order is not our `o001..o008` order.
Removing `*` alone restores `g_pActivity @0` and COMMON-last but shifts `.text`
by −12 and `.bss` to +0x28, so it is not a standalone win. Known local-static
deltas: `VirtualInput.cpp` `g_inputE.0..3` +16, `BlowfishSubkeyFile.cpp` +4.

## 6. `.text` +0x30 — the fix that mattered

Named-function size deltas (`.dynsym`) sum to −0x24; the rest was padding. The
two stale "layout anchors" were the cause:

1. `src/engine/render/MarqueeImage.cpp` had
   `[[gnu::used]] static int marqueeImageByteSum(int)` — a 16-byte out-of-line
   helper the original does not contain. Its comment claimed the preceding TUs
   were 16 B short, but after the earlier data-layout passes that was stale: it
   pushed the whole `MarqueeImage..Galaxy` block **+0x10** late (and its exidx
   entry was the extra `RocketGun::translate+2 INL0`). **Deleted.**
2. `src/engine/core/AbyssEngine.cpp`'s `levelTextAnchor()` pads `.text` to the
   original total. Its body was 0x34 B; the cumulative deficit before it is
   −0x24, so it overshot by 0x10. **Trimmed to 0x24 B** (one `layoutPad` store).

Measured effect of the two changes together (with `DROP_EMPTY_TEXT` re-tuned to
keep the `.rodata` base): `.text 0x1337f8 → 0x1337c8` (exact), byte_exact
`2923 → 3885`.

Remaining `.text` layout deltas (all local functions, all small):

| address | delta | region |
|---|---:|---|
| `0x8765c` | −0x8 | `ConfigReader` local helpers (two 4-byte shrinks) |
| `0xd2bc8` | +0x2 | `Layout` local helper |
| `0x175e08` | −0x2 | between `MarqueeImage` and `Galaxy` |
| `0x1869f4` | −0x2 | before `inflateReset` |
| `0x18c6c8`…`0x18f618` | −0x2 each | libzip local functions |
| `0x18fe40` | +0x4 | before `OPENSSL_atomic_add` |
| `0x193c10` | −0x2 | libc++abi |

They are the same codegen deltas that keep the extab records different.

## 7. Coupling: why the sizes cannot be fixed independently

`.rodata` is placed at `align16(.text_end + .ARM.exidx + .ARM.extab)`. With the
original values the sum is `0x1c09d0`; ours is currently `0x1c09c4`, i.e. only
**0xc bytes of alignment slack**. Therefore:

* `.text` ±8 B, `.exidx` ±8 B, `.extab` ±4 B changes move `.rodata` unless they
  are combined to land back in `(0x1c09c0, 0x1c09d0]`;
* the chosen `DROP_EMPTY_TEXT` set (`BackButton`, `main`, `GOF2HD2012` dropped;
  the other seven app TUs kept) plus the trimmed `levelTextAnchor` land the
  total at `0x1c09c4`, still inside the window, so the base stays `0x1c9e10`;
* fixing the extab (−0x1c → 0) or the two missing CANT+INL pairs (+0x20) alone
  moves `.rodata`; they must be landed together with a matching `.text` change.

## 8. Ranked remaining causes

1. **Real body differences** (extab +0x1c, exidx landing pads): `HangarWindow::render`,
   `Hud::draw`, `ConfigReader::RegisterTokenReadFunction`,
   `RecordHandler::recordStoreWrite`, `StarMap::draw`, `Level::createShip`,
   `Generator::createMission`, `LevelScript::process`, `__cxa_demangle`,
   `Layout::C1` (272 B short), `ConfigReader::GetNewLine`,
   `MenuTouchWindow::C1`. Fixing these fixes extab and the last 26 non-fuzzy
   functions at the same time.
2. **Marker placement for COMDAT-owning TUs** (`loadPortraits`,
   `ResourceTextureC2`): 2 spurious CANTs.
3. **Two missing CANT+INL pairs** (`NFC::C1`, `GetLeaderboardScore`): needs
   +0x20 exidx, must be paired with a −0x20 `.text`/extab change.
4. **`.rodata` literal order** (`FModSound` first; then the 12 sections above).
   The FModSound reorder is correct but currently costs 10 byte_exact via PLT
   order; it belongs to the PLT-order campaign.
5. **`.bss` local-static order/padding** (`** common` first; `VirtualInput`
   `g_inputE.0..3` +16; `BlowfishSubkeyFile` +4).
6. **libzip local-function 2-byte deltas** (the −0x2 cascade): vendored-code
   codegen; each shifts a CANT.

## 9. Reproduction

```sh
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check   -j8   # RATCHET PASS, byte 3885
cmake --build cmake-build-match --target drift   -j8   # 0 fields
```

Scratch parsers live under `/tmp/w3367/` (`elf_exidx.py`, `diff_exidx2.py`,
`extab_len.py`, `bss_diff.py`, `measure.sh`). They are analysis-only; the
committed changes are `src/engine/render/MarqueeImage.cpp`,
`src/engine/core/AbyssEngine.cpp`, `tools/gen_exidx_markers.py`, and the two
docs.

---

# w3376: close the exception-table delta

Scope: remove the last `+0x10` `.ARM.exidx` / `-0x1c` `.ARM.extab` delta without moving
`.text` (`0x1337c8`) or the `.rodata` base (`0x1c9e10`). Worktree `wt/w3376`, branch
`wave87-w3376`, HEAD `1fcdd328`.

## Result

| section | original | before (HEAD `1fcdd328`) | after | delta after |
|---|---:|---:|---:|---:|
| `.text` | `0x1337c8` | `0x1337c8` | **`0x1337c8`** | **0** |
| `.ARM.exidx` | `0x06f70` | `0x06f80` (+0x10) | **`0x06f70`** | **0** |
| `.ARM.extab` | `0x26298` | `0x2627c` (−0x1c) | **`0x2628c`** | **−0xc** |
| `.rodata` | `0x41b67` @ `0x1c9e10` | `0x41b97` @ `0x1c9e10` | `0x41b97` @ `0x1c9e10` | +0x30 (base exact) |
| `.bss` | `0x069ac` @ `0x216970` | `0x069c8` @ `0x216980` | `0x069c8` @ `0x216980` | +0x1c |
| `.rel.dyn` | `0x07048` | `0x07048` | `0x07048` | **0** |

Gate: **byte_exact 3907 → 3914 (+7)**, linked_exact 4494 (unchanged), avg 99.89,
`RATCHET PASS`, `drift` 0 fields. `.ARM.exidx` is now **exact** (3566 entries:
CANT 272, inline 856, extab 2438 — the same split as the original).

The +7 byte-exact are all in the `ConfigReader` TU: `GetNewLine`, `ParseFile`,
`ConfigReader::ConfigReader` (C1/C2), `ConfigReader::~ConfigReader` (D1/D2),
`Array<TokenStruct>::~Array`. They were already 100 % fuzzy but their literal pools
were 4–8 B out of place; restoring the original TU size made them byte-identical.

## Method / tooling

`_work/reconstructions/exidx_table.py` is the committed, self-contained parser (the
w3367 scratch parsers lived in `/tmp`). It decodes:

* `.ARM.exidx` as 8-byte `(prel31 fn, word1)` entries;
* `word1 == 1` → `EXIDX_CANTUNWIND`; `word1 & 0x80000000` → inline compact model;
  otherwise a prel31 to `.ARM.extab`;
* the LSDA with the real libc++abi layout
  (`sources/cxx-stl/llvm-libc++abi/src/cxa_personality.cpp`):
  `lpStartEncoding`+`lpStart`, `ttypeEncoding`+`classInfoOffset` (encoded with
  `ttypeEncoding`), `callSiteEncoding`+`callSiteTableLength` (ULEB128), then
  `(start, length, landingPad, action)` call-site rows.

```sh
# full per-entry table: entry#, exidx address, function+offset, kind, model, extab
python3 _work/reconstructions/exidx_table.py ../_work/bins/android_2.0.16_libgof2hdaa.so
python3 _work/reconstructions/exidx_table.py cmake-build-match/libgof2hdaa.so

# per-function kind-sequence + extab-record-length diff
python3 _work/reconstructions/exidx_table.py --diff \
    ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

Every extab target is used by exactly one exidx entry in both binaries (no shared
LSDA records), so record lengths are recovered as the distance between consecutive
extab targets in address order.

## 1. The `+0x10` was two artificial `.text` anchors

Both extra entries were **inline model-0** entries for hand-written layout anchors,
not real landing pads:

| anchor | TU | size | exidx entry | why it existed |
|---|---|---:|---|---|
| `levelMissionPad(int)` | `ConfigReader.cpp` | 8 B | `INL0` at `GetNewLine+275` (`0x878e0`) | padded `.text` after an earlier anchor trim |
| `mtwTextLayoutAnchor()` | `MenuTouchWindow.cpp` | 0 B | `INL0` at `Wanted::getCurrentLocation+3` (`0x12299c`) | kept one `.data.rel.ro` relocation so `.rel.dyn` stayed `0x7048` |

`levelMissionPad` was deleted; the `ConfigReader` functions below grew by exactly the
same 8 B, so `.text` stayed exact and the entry disappeared.

`mtwTextLayoutAnchor` was replaced by a plain `.data.rel.ro` reference to the symbol the
original stores at `0x20de20`:

```cpp
extern const char _ZTI15PlayerStaticFar[];
[[gnu::used]] static const void *const mtwPlayerStaticFarTypeInfoRef = _ZTI15PlayerStaticFar;
```

This keeps exactly one `.rel.dyn` entry (`.rel.dyn` stays `0x7048`, so `.rel.plt`/`.plt`
and every function address stay put) but has no `.ARM.exidx` companion. The original's
`0x20de20` slot is `R_ARM_ABS32 _ZTI15PlayerStaticFar`, so this is also the correct data.

Removing the two anchors alone gives `.exidx 0x6f78` and moves `.rodata` to `0x1c9e00`
(the `text+exidx+extab` sum falls to `0x1c9dfc`). The extab fix below puts the sum back
to `0x1c9e04`, inside the 16-byte alignment window.

## 2. `ConfigReader` landing-pad trampolines (`+0x10` extab, `+8` text)

The three `ConfigReader` token readers were 4/8/4 B short in the LSDA call-site table
(`ReadTimer` 28 vs 32, `ReadKeyMap` 96 vs 104, `ReadVibration` 28 vs 32). The original
emits an extra `b.n` trampoline landing pad for the *second* potentially-throwing call
in each function, i.e. one extra call-site row:

```
orig ReadTimer         ours (before)
... blx __stack_chk_fail   ... bl __stack_chk_fail
    b.n 873f0              mov r4, r0        <- shared cleanup
    mov r4, r0
```

The generated code is identical; only the EH cleanup grouping differs. Three
semantically-null locals (all optimised away) reproduce clang's grouping:

| function | change | .text | extab |
|---|---|---:|---:|
| `ReadTimer` | `int value = line.ValueOf(); engine->timerInterval = value;` | +4 | 28 → 32 |
| `ReadKeyMap` | `int dummy = 0; (void) dummy;` before the trailing `ArrayRemoveAll(keys)` | +4 | 96 → 104 |
| `ReadVibration` | `int on = line.Compare("ON"); if (on == 0) …` | +0 | 28 → 32 |

Net `+8` `.text` (cancelled by deleting `levelMissionPad`) and `+0x10` `.extab`.
The three functions' disassembly now matches the original instruction-for-instruction
(including the `b.n` + `nop`), and the four `ConfigReader` extab records are exactly
`32 / 104 / 32 / 12` as in the original.

## 3. Remaining `.ARM.extab` delta: `−0xc`

| function | original | ours | Δ | note |
|---|---:|---:|---:|---|
| `HangarWindow::render` | 948 | 912 | −36 | body/call-site layout |
| `RecordHandler::readWanted` | 656 | 640 | −16 | body/call-site layout |
| `Level::createShip` (mis-attributed as `ArrayAdd<KIPlayer>`) | 336 | 328 | −8 | body/call-site layout |
| `Hud::drawChallengeModeScore` | 476 | 492 | +16 | ours larger |
| `StarMap::drawOnScreenInfo` | 116 | 128 | +12 | ours larger |
| `__cxa_demangle` | 2144 | 2156 | +12 | vendored libc++abi |
| `Generator::createAgent` | 172 | 176 | +4 | ours larger |
| `LevelScript::~LevelScript` | 348 | 352 | +4 | ours larger |
| **total** | | | **−12 = −0xc** | |

These are the same functions that are still not byte-exact. Because the current
`text+exidx+extab` sum is `0x1c9e04`, the extab may grow by at most `+0xc` before
`.rodata` moves to `0x1c9e20`; fixing all eight lands exactly on the original
`0x1c9e10` boundary, but fixing any single large one (`HangarWindow` −36,
`readWanted` −16) must be paired with a compensating positive delta.

## 4. Per-function kind-sequence diff (16 functions, offsets ignored)

The total entry count and the CANT/INL/EXT split are now exact. The residual kind-sequence
differences are all *placement* differences caused by the remaining body-size deltas
(libzip's −2 B local functions, `Layout::C1`'s 272 B pad, libc++abi), plus the two
`DROP_EMPTY_TEXT` TUs:

```
SetGPIsLinked              orig CANT+EXT   ours EXT
BackButtonPressed          orig CANT+INL   ours (none)
isInMainMenu               orig CANT+INL   ours (none)
ArrayReleaseClasses<...>   orig EXT        ours CANT+EXT
loadLowTexturesAndMaterials orig EXT       ours CANT+EXT
ArrayAdd<ParticleSet>      orig EXT        ours CANT+EXT
GetTouch                   orig CANT+EXT   ours EXT
ResourceMaterial::C2       orig EXT        ours CANT+EXT
AEMath::VectorLerp         orig EXT        ours CANT+EXT
Layout::C2                 orig EXT        ours INL+EXT
bad_typeid::what           orig CANT+EXT+CANT+INL  ours CANT+EXT+INL
__cxa_deleted_virtual      orig 24 entries ours 23 (missing CANT@603)
__cxa_demangle             orig 239        ours 240 (tail)
ndk_autosave               orig INL        ours CANT+INL
ndk_resetNativeItem...     orig CANT+EXT   ours EXT
ndk_setNativeItem...       orig EXT        ours CANT+EXT
```

The libzip/`_zip_*` offset-only shifts (e.g. `_zip_fclose` CANT/EXT at +111 vs +109)
are omitted from this list; they are the −2 B local-function cascade documented in §6
of the w3367 section.

## 5. Measured experiments (negative results, for the record)

All were reverted; they are useful because they show why the simple fixes do not work.

| experiment | `.text` | `.exidx` | `.extab` | `.rodata` base | byte_exact |
|---|---:|---:|---:|---:|---:|
| HEAD | `0x1337c8` | `0x6f80` | `0x2627c` | `0x1c9e10` | 3907 |
| typeinfo ref only (no extab/text compensation) | `0x1337c8` | `0x6f78` | `0x2627c` | **`0x1c9e00`** | 1706 |
| `Level::createShip` `catch (int)` + `catch (...)` | `0x1337e8` | `0x6f80` | `0x26284` | `0x1c9e10` | 2323 |
| `Level::createShip` `catch (const std::exception&)` + `catch (...)` | `0x1337c8` | `0x6f78` | `0x26284` | `0x1c9e10` | 2876 |

The `std::exception` variant keeps `.text` exact but `.rel.dyn` shrinks by 8 (the
`_ZTIi` GLOB_DAT goes away), which moves `.rel.plt`/`.plt` and every PLT call site:
byte_exact collapses. **`.rel.dyn` must stay `0x7048`**; that is why the
`mtwTextLayoutAnchor` replacement is a `.data.rel.ro` reference rather than deleting the
relocation.

ConfigReader A/B variants that did **not** reproduce the original (all keep the 28-byte
record): braces, `else`, `return`, `String tmp`, `String line; line = …`,
`String(GetNewLine())`, `-fexceptions`, `-fno-rtti`, `-O0/-O1/-O2/-O3/-Os`, and
`-fno-exceptions` (12/16/12, too small). The `int` locals above are the only variants
that produce the original `b.n` trampoline.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3376
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check   -j8   # RATCHET PASS, byte 3914
cmake --build cmake-build-match --target drift   -j8   # 0 fields

NDK=.cache/ndk/android-ndk-r18b
RE=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -SW cmake-build-match/libgof2hdaa.so | grep -E '\.text |\.ARM.exidx|\.ARM.extab|\.rodata |\.bss |\.rel.dyn'

python3 _work/reconstructions/exidx_table.py --diff \
    ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

## 7. Ranked remaining causes

1. **`HangarWindow::render` (−36)** — largest single extab delta; fix with the body
   reconstruction campaign.
2. **`RecordHandler::readWanted` (−16)** — 4 call-site rows short; same class of
   clang EH-grouping difference as the ConfigReader readers (try a null local near the
   `AEFile::Read` block).
3. **`Level::createShip` (−8)** — the artificial `try { setEmpData } catch (...)` is not
   the original cleanup shape; the original LSDA has 57 call-site rows with
   `callSiteEncoding 0x01`, ours uses a different header/encoding.
4. **`Hud::drawChallengeModeScore` (+16), `StarMap::drawOnScreenInfo` (+12),
   `Generator::createAgent` (+4), `LevelScript::~LevelScript` (+4)** — ours is larger;
   fixing them needs a compensating negative delta to stay inside the `.rodata` window.
5. **`__cxa_demangle` (+12)** — vendored libc++abi codegen.
6. **`Layout::C1`** — 272 B short (the `layoutCtorPad` anchor supplies it); the pad's
   `INL0` entry is the `Layout::C2` kind-sequence difference. Reconstructing the real
   ctor body fixes `.text`, the exidx entry and the extab record together.
