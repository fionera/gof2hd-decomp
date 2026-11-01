# Layout / anchor audit v2 — current HEAD, plus the `.ARM.exidx` missing-entry lead

Analysis-only report. No `src/`, `CMakeLists.txt`, or gate changes were made for this pass.
The old `_work/reconstructions/layout_anchor_audit.md` is left untouched; this file supersedes
its numbers.

## 0. Provenance, build, and commands

- Worktree: `/home/debian/GalaxyOnFire2/wt/w3304`
- Branch: `research-w3304`
- HEAD at analysis time: `81db17d10cf707aad018b28831730d147a0e1959`
  (`tools: worker launcher/remover with workdir confinement and cleanup`)
- Original: `../_work/bins/android_2.0.16_libgof2hdaa.so`
  - sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`
- Our linked artifact: `cmake-build-match/libgof2hdaa.so`
  - sha256 `2cec02203642cbe72bcb55ddc0e8b604ae258582327cb65729ed113d8e536515`

Build and gate (exactly the task's recipe):

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j32
```

Measured gate at this HEAD:

```text
byte_exact 1415   linked_exact 4493   fuzzy_perfect 4494   avg 99.89
stub_zero_size 0  missing 0  extra 0  size-exact 4522/4523
data exact 843/843   sodiff extra 0 missing 0 imports 207
RATCHET PASS
```

This matches the task's baseline (byte 1415, linked 4493, size-exact 4522/4523,
`.text` 0x1343d0). `verify/report.json` has 4523 compared rows; the 4524th common dynsym
function is `AbyssEngine::getAppVersion` (original size 0), which the report excludes.

All measurements below are reproducible with the scripts in `/tmp` listed in §8. Every
address is the real ELF address (Ghidra address − 0x10000).

## 1. Exact current section deltas (original vs ours)

`readelf -SW` on both binaries; addresses and sizes in bytes.

| section | orig addr | orig size | ours addr | ours size | Δaddr | Δsize |
|---|---:|---:|---:|---:|---:|---:|
| `.text` | `0x69440` | `0x1337c8` | `0x69440` | `0x1343d0` | 0 | **`+0xc08` (+3080)** |
| `.ARM.exidx` | `0x19cc08` | `0x6f70` | `0x19d810` | `0x66c8` | `+0xc08` | **`-0x8a8` (-2216, -277 entries)** |
| `.ARM.extab` | `0x1a3b78` | `0x26298` | `0x1a3ed8` | `0x262a4` | `+0x360` | **`+0xc` (+12)** |
| `.rodata` | `0x1c9e10` | `0x41b67` | `0x1ca180` | `0x41747` | `+0x370` | `-0x420` (-1056) |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` | `0x20ceb0` | `0x3008` | `+0x300` | `+0x20` (+32) |
| `.data` | `0x214000` | `0x296c` | `0x210000` | `0x296c` | `-0x4000` | 0 |
| `.bss` | `0x216970` | `0x69ac` | `0x216c80` | `0x69d8` | `+0x310` | `+0x2c` (+44) |
| `.got` | `0x20fcf4` | `0x430c` | `0x212974` | `0x430c` | `+0x2c80` | 0 |
| `.dynamic` | `0x20fba4` | `0x148` | `0x20fec4` | `0x138` | `+0x320` | `-0x10` |
| `.init_array` | `0x20fb98` | `0xc` | `0x20feb8` | `0xc` | `+0x320` | 0 |
| `.fini_array` | `0x20cba0` | `0x8` | `0x20cea0` | `0x8` | `+0x300` | 0 |
| `.aezdata` | absent | — | `0x21296c` | `0x8` | — | **`+0x8` (extra section)** |
| `.comment` | — | `0x12f` | — | `0x12f` | — | 0 |

Section count: original 27, ours 28 (the extra `.aezdata`).

Key end addresses:

| item | original | ours |
|---|---:|---:|
| `.text` start | `0x69440` | `0x69440` |
| `.text` end | `0x19cc08` | `0x19d810` |
| `.ARM.exidx` start/end | `0x19cc08` / `0x1a3b78` | `0x19d810` / `0x1a3ed8` |
| `.ARM.extab` start/end | `0x1a3b78` / `0x1c9e10` | `0x1a3ed8` / `0x1ca17c` |
| `.rodata` start/end | `0x1c9e10` / `0x20b977` | `0x1ca180` / `0x20b8c7` |
| `.fini_array` | `0x20cba0` | `0x20cea0` |
| `.got` | `0x20fcf4` | `0x212974` |
| `.data` | `0x214000` | `0x210000` |
| `.bss` | `0x216970` | `0x216c80` |

### 1.1 Corrections to the old audit

The old audit's section table and invariants are stale at this HEAD:

- **`.got` ordering.** Original order is `.got` then `.data` then `.bss`.
  Ours is `.data` then `.aezdata` then `.got` then `.bss`. The old audit's
  "`.got` before `.data`" statement described the original but its address table
  implied ours also had `.got` before `.data`; it does not.
- **`.got - ctor` is no longer `0x7c0c0`.** Measured now:

  | ctor | orig addr | ours addr | orig `.got-ctor` | ours `.got-ctor` |
  |---|---:|---:|---:|---:|
  | `_ZNSt9bad_allocC1Ev` | `0x193c34` | `0x194634` | `0x7c0c0` | **`0x7e340`** |
  | `_ZNSt20bad_array_new_lengthC1Ev` | `0x193c54` | `0x194654` | `0x7c0a0` | `0x7e320` |
  | `_ZNSt16bad_array_lengthC1Ev` | `0x193c74` | `0x194674` | `0x7c080` | `0x7e300` |
  | `_ZNSt8bad_castC1Ev` | `0x193e04` | `0x194804` | `0x7bef0` | `0x7e170` |
  | `_ZNSt10bad_typeidC1Ev` | `0x193e34` | `0x194834` | `0x7bec0` | `0x7e140` |

  The five ctors are `linked: true` but **`byte: false`** in `verify/report.json`
  (`pct 100.0`). Their literal is `.got + 0x976 - (ctor + 4)`: original
  `0x0007ca32`, ours `0x0007ecb2`. The old audit's "all ten C1/C2 ctors `byte: true`"
  is stale.
- **`.ARM.extab` is `+12`, not `-32`.** `docs/IDENTICALITY.md:37` still says
  `156,280 (-32)`; measured ours is `0x262a4 = 156,324`, delta `+12`.
- **`.rodata` and `.bss` doc numbers are stale.** `docs/IDENTICALITY.md:36,40`
  say `.rodata` 267,831 and `.bss` 27,064; measured are `0x41747 = 268,103` and
  `0x69d8 = 27,096`.
- **`kMissionOrder` is not emitted.** `src/game/mission/Status.cpp:2050` declares
  `static volatile unsigned char kMissionOrder[190];` but never references it; the
  object has no `.bss` section at all. It contributes 0 bytes today.
- **`Transform.cpp` 72-byte `.data` block and `RocketGun.cpp` `kRocketTrailSets[4]`
  are gone.** `nm -S` on both objects shows no such `.data` symbol. The old audit's
  items 18 and 19 are stale.
- **`HangarWindow` has 3 `description = description;` lines, not 6** (current
  `src/game/ui/HangarWindow.cpp:969-971`).

## 2. Decomposing the `.text` `+0xc08` (3080 B)

### 2.1 Method

`/tmp/delta_full.py` reads both `.dynsym` tables through `tools/verify/elf.py`,
sorts the 4524 common functions by original address, and prints:

- every function whose `usize != osize`;
- every inter-function gap delta `gd = gap_ours - gap_orig`;
- every start-address delta transition.

The 4523-row `verify/report.json` excludes `getAppVersion` (original size 0); the
script keeps it so the address arithmetic is complete.

### 2.2 Function-body deltas — only two

| function | orig size | ours size | delta |
|---|---:|---:|---:|
| `_ZN11AbyssEngine13getAppVersionEv` | 0 | 2 | **+2** |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 1992 | **-4** |

Net function-body delta: **-2 B**. Every other one of the 4523 compared functions is
size-exact. This is the big change since the old audit, which had 22 size mismatches
netting -1,230 B.

### 2.3 Region sums

`/tmp/region_sum.py` sums `gd` over the regions between the anchor and the archive
boundaries:

| region | `sum(gd)` |
|---|---:|
| prologue, first function → `MenuTouchWindow::startSupernovaChallenge` | **`+0x82` (+130)** |
| anchor gap after `startSupernovaChallenge` | **`+0xb78` (+2936)** |
| `PendingProduct` → `StarSystem` | 0 |
| `StarSystem` gap | `+0x10` (+16) |
| `StarSystem` → `MarqueeImage` | 0 |
| `ObjectGun` gap | `+0x10` (+16) |
| `ObjectGun` → `zip_stat_init` | `-0x20` (-32) |
| `zip_stat_init` → `zip_fopen` | `+0x264c` (+9804) |
| `zip_fopen` → `_zip_error_strerror` | `-0x4` (-4) |
| `_zip_error_strerror` → `SHA224_Init` | `-0x27b8` (-10168) |
| `SHA224` → `OPENSSL_instrument_bus2` | `+0x1d78` (+7544) |
| `OPENSSL_instrument_bus2` gap | `-0x1ee0` (-7904) |
| `sha256_block_data_order` → `_neon` | 0 |
| `sha256_block_data_order_neon` gap | `+0x330` (+816) |
| `__cxa_end_cleanup` → `__cxa_deleted_virtual` | 0 |
| `__cxa_deleted_virtual` gap | `-0x24c` (-588) |
| `__dynamic_cast` → last dynsym function | `-0x64` (-100) |
| tail after last dynsym function (`__cxa_demangle`) | **`+0x268` (+616)** |

The cumulative delta at the last dynsym function is `+0x9a0` (2464); adding the tail
`+0x268` gives the observed `+0xc08` (3080).

### 2.4 The clean arithmetic

```text
mtwTextLayoutAnchor body                     +2936
prologue gaps                                +130
post-anchor non-archive gaps                   +0   (StarSystem +16, ObjectGun +16, ObjectGun..zip -32)
archive-member placement                     -600   (zlib/libzip/OpenSSL/libc++abi)
tail after __cxa_demangle                     +616
function-body size delta                        -2   (Generator -4, getAppVersion +2)
                                             -----
                                             +3080 = +0xc08
```

The task's "remaining ~144 B" is therefore `130 - 600 + 616 - 2 = 144`. It is **not**
a single missing body; it is the net of the small prologue anchors/helpers, a
cross-archive placement artifact, and the local libc++abi demangler tail.

### 2.5 Every place the start delta changes

The full transition list from `/tmp/delta_full.py` follows. `sd` is the start-address
delta at that function; `gd` is the gap delta after the previous function. `+2`/`-2`
entries are 2-byte Thumb alignment; they are listed for completeness.

| # | orig addr | function | sd | gd | owning TU / cause |
|---:|---:|---|---:|---:|---|
| 0 | `0x69578` | `Java_..._BackButtonPressed` | 0 | — | NFC.cpp |
| 162 | `0x6db20` | `Engine::InitGL` | +4 | +4 | GLES1Compat.cpp |
| 193 | `0x6dfce` | `FileInterfaceAndroid::D0Ev` | -6 | -0xa / -0x2 | FileInterfaceAndroid.cpp |
| 194 | `0x6dfe0` | `FileInterfaceAndroid::OpenRead` | -8 | — | FileInterfaceAndroid.cpp |
| 754 | `0x7efcc` | `GameText::getRegionCode` | +8 | **+0x10** | `arabicFormOffset` (16 B, GameText.cpp) |
| 778 | `0x7fb74` | `AESoundRessource::SetSound` | +0x12 | +0xa | AESoundRessource.cpp |
| 780 | `0x7fbd0` | `AESoundRessource::initWithoutLoading` | +0x14 | +0x2 | AESoundRessource.cpp |
| 846 | `0x81008` | `Quaternion::C2` | +0x12 | -0x2 | Quaternion.cpp |
| 857 | `0x8103c` | `Quaternion::Set` | +0x10 | -0x2 | Quaternion.cpp |
| 883 | `0x81818` | `GameText::getText` | +0x38 | **+0x28** | `levelTextAnchor` (40 B, Level.cpp, section-order line 889) |
| 963 | `0x83ee8` | `glError` | +0x36 | -0x2 / +0x2 | ApplicationManager.cpp / Engine.cpp |
| 964 | `0x83eec` | `Engine::C1` | +0x38 | — | Engine.cpp |
| 1061 | `0x8765c` | `Array<TokenStruct>::D2` | +0x30 | -0x8 | ConfigReader.cpp |
| 1488 | `0x97984` | `MTitle::C1` | +0x2c | — | MTitle.cpp |
| 1497 | `0x97a2c` | `MTitle::OnRelease` | +0x30 | **+0x4** | `generatorMissionLayoutPad` (4 B, MTitle.cpp) |
| 1996 | `0xa63e0` | `ListItem::C2` | +0x2e | -0x2 | KIPlayer.cpp |
| 2046 | `0xa6834` | `ListItem::checkCredits` | +0x2c | -0x2 | ListItem.cpp |
| 2125 | `0xa8ac0` | `Status::C1` | +0x2a | -0x2 | Explosion.cpp |
| 2149 | `0xa90b0` | `Status::setStation` | +0x28 | -0x2 | Status.cpp |
| 2290 | `0xbd6f8` | `Level::initParticleSystems` | +0x30 | **+0x8** | `levelMissionPad` (8 B, Level.cpp) |
| 2372 | `0xc67a4` | `StarMap::C1` | +0x38 | **+0x8** | `StarMap_GetScreenPosition` (8 B, StarMap.cpp) |
| 2417 | `0xcddd8` | `RecordHandler::writeByteArrayAsOptionsFile` | +0x58 | **+0x20** | `is_equal` (32 B, RecordHandler.cpp) |
| 2527 | `0xd7600` | `ModStation::OnTouchEnd` | +0x8c | **+0x34** | `modStationTouchEndPad` (52 B, ModStation.cpp) |
| 2582 | `0xdcfd8` | `PlayerFighter::update` | +0x88 | -0x4 | PlayerFighter.cpp |
| 2761 | `0x11b710` | `Node::C1` | +0x86 | -0x2 | ResourceMaterial.cpp |
| 2772 | `0x11b7a8` | `SystemPathFinder::getSystemPath` | +0x84 | -0x2 | SystemPathFinder.cpp |
| 2843 | `0x11cc40` | `PlayerCreature::reset` | +0x80 | -0x4 | PlayerCreature.cpp |
| 2994 | `0x122890` | `Wanted::C1` | +0x7e | -0x2 | PlayerStation.cpp |
| 3025 | `0x12299c` | `MenuTouchWindow::C2` | +0x80 | +0x2 | Wanted.cpp |
| **3066** | **`0x12d3c8`** | **`PendingProduct::C1`** | **`+0xbf8`** | **`+0xb78`** | **`mtwTextLayoutAnchor` (2936 B, MenuTouchWindow.cpp)** |
| 3113 | `0x133b68` | `StarSystem::C2` | +0xc08 | **+0x10** | `starSystemLayoutAnchor` (14 B + 2 pad, StarSystem.cpp) |
| 3671 | `0x15f700` | `ObjectGun::C1` | +0xc18 | **+0x10** | `ObjectGunProjectileMesh` (16 B, ObjectGun.cpp) |
| 3996 | `0x175e08` | `Galaxy::C2` | +0xc16 | -0x2 | ModMainMenu.cpp |
| 4003 | `0x175ea8` | `Galaxy::getStation` | +0xc18 | +0x2 | Galaxy.cpp |
| 4282 | `0x1869f4` | `inflateReset` | +0xc16 | -0x2 | ParticleSystemMesh.cpp |
| 4284 | `0x186a6c` | `inflateInit2_` | +0xc14 | -0x2 | zlib |
| 4334 | `0x18c6c8` | `zip_file_strerror` | +0xc12 | -0x2 | libzip |
| 4335 | `0x18c6d0` | `zip_fopen_index` | +0xc10 | -0x2 | libzip |
| 4339 | `0x18ca54` | `zip_get_archive_comment` | +0xc0e | -0x2 | libzip |
| 4343 | `0x18cb44` | `_zip_file_get_offset` | +0xc0c | -0x2 | libzip |
| 4346 | `0x18cc78` | `_zip_new` | +0xc0a | -0x2 | libzip |
| 4347 | `0x18ccc4` | `zip_close` | +0xc08 | -0x2 | libzip |
| 4354 | `0x18d8f0` | `zip_source_free` | +0xc06 | -0x2 | libzip |
| 4357 | `0x18d954` | `_zip_source_file_or_p` | +0xc04 | -0x2 | libzip |
| 4365 | `0x18db84` | `_zip_entry_new` | +0xc02 | -0x2 | libzip |
| 4366 | `0x18dbfc` | `zip_get_archive_flag` | +0xc00 | -0x2 | libzip |
| 4368 | `0x18dc24` | `_zip_unchange_data` | +0xbfe | +0x2 | libzip |
| 4369 | `0x18dc50` | `zip_fread` | +0xc00 | -0x2 | libzip |
| 4370 | `0x18dd6c` | `zip_rename` | +0xbfe | +0x2 | libzip |
| 4371 | `0x18ddec` | `zip_source_zip` | +0xc00 | -0x2 | libzip |
| 4373 | `0x18e0ec` | `zip_source_function` | +0xbfe | -0x2 | libzip |
| 4374 | `0x18e128` | `zip_replace` | +0xbfc | -0x2 | libzip |
| 4376 | `0x18e1c4` | `zip_delete` | +0xbfa | +0x2 | libzip |
| 4380 | `0x18e314` | `_zip_mkstemp` | +0xbfc | -0x2 | libzip |
| 4384 | `0x18eb80` | `zip_fclose` | +0xbfa | -0x2 | libzip |
| 4385 | `0x18ebf0` | `_zip_cdir_free` | +0xbf8 | -0x2 | libzip |
| 4400 | `0x18f5c0` | `zip_fopen` | +0x3244 | **+0x264c** | archive member placement |
| 4402 | `0x18f618` | `_zip_error_strerror` | +0x3240 | -0x4 | libzip |
| 4403 | `0x18f730` | `SHA224_Init` | +0xa88 | **-0x27b8** | libzip → OpenSSL boundary |
| 4412 | `0x18fe40` | `OPENSSL_atomic_add` | +0x2800 | **+0x1d78** | OpenSSL internal |
| 4424 | `0x1900e0` | `sha256_block_data_order` | +0x920 | **-0x1ee0** | OpenSSL → libc++abi boundary |
| 4426 | `0x191d20` | `__cxa_end_cleanup` | +0xc50 | **+0x330** | libc++abi |
| 4459 | `0x19362c` | `__dynamic_cast` | +0xa04 | **-0x24c** | libc++abi |
| 4460 | `0x193c10` | `_ZNSt9bad_allocD2Ev` | +0xa02 | -0x2 | libc++abi |
| 4471 | `0x193c18` | `_ZNKSt9exception4whatEv` | +0xa00 | -0x2 | libc++abi |
| 4521 | `0x193ec4` | `_ZSt14set_unexpectedPFvvE` | +0x9a0 | **-0x60** | libc++abi |
| — | tail | after `__cxa_demangle` | +0x9a0 | **+0x268** | local libc++abi demangler + padding |

The `+0x264c` / `-0x4` / `-0x27b8` / `+0x1d78` / `-0x1ee0` / `+0x330` / `-0x24c` /
`-0x64` chain nets to `-0x258` (-600). It is **archive member placement**, not a source delta: the
same archive members are present, but gold places the zlib/libzip/OpenSSL/libc++abi
input sections in a different order than the original link. The `.text` end is
therefore not a function-by-function source difference in that region.

## 3. Artificial anchor / pad inventory in `src/`

`nm -S` on the `.obj` files; linked addresses from the unstripped relink
`/tmp/test_exidxorder.so` (`nm -nS`). "Load-bearing" means removing it changes the
current `.text`/`.data.rel.ro`/`.rodata` layout or the gate.

| # | site | symbol | obj size | linked addr | owning TU | load-bearing? |
|---:|---|---|---:|---:|---|---|
| 1 | `src/game/ui/MenuTouchWindow.cpp:277,281,3855-3940` | `_ZL19mtwTextLayoutAnchorv` (`t`) | **`0xb76` (2934)** | `0x12d448`..`0x12dfbe` (linked 0xb78) | MenuTouchWindow.cpp | **yes** — kept alive by a `R_ARM_ABS32` relocation in `.data.rel.ro` (`MenuTouchWindow_languageIds[11]`, `R_ARM_ABS32` at `.rel.data.rel.ro+0x28`); self-call keeps it out of line |
| 2 | `src/game/world/StarSystem.cpp:129` | `_ZL22starSystemLayoutAnchori` (`t`) | **`0xe` (14)** | `0x13475c`..`0x13476a` | StarSystem.cpp | **yes** — `[[gnu::used]]`; sits between `StarSystem::C1` and `C2` in `section_order.txt:3130-3132`; creates the `+0x10` gap |
| 3 | `src/game/world/StarMap.cpp:41` | `_ZL25StarMap_GetScreenPosition...` (`t`) | **`0x8`** | `0xc67d4`..`0xc67dc` | StarMap.cpp | **yes** — `[[gnu::used]]`; immediately before `StarMap::C1`, creates the `+0x8` gap |
| 4 | `src/game/menu/MTitle.cpp:106` | `_ZL25generatorMissionLayoutPadv` (`t`) | **`0x4`** | `0x97a58`..`0x97a5c` | MTitle.cpp | **yes** — `[[gnu::used]]`; immediately before `MTitle::OnRelease`, creates the `+0x4` gap |
| 5 | `src/game/menu/ModStation.cpp:1109` | `_Z21modStationTouchEndPadi` (`T`, hidden) | **`0x34` (52)** | `0xd7658`..`0xd768c` | ModStation.cpp | **yes** — recursive, hidden external linkage; creates the `+0x34` gap before `OnTouchEnd` |
| 6 | `src/game/world/Level.cpp:718` | `_Z15levelMissionPadi` (`T`, hidden) | **`0x8`** | `0xbd71c`..`0xbd724` | Level.cpp | **yes** — after `setPlayerEngineColor`; creates the `+0x8` gap |
| 7 | `src/game/world/Level.cpp:7191` | `_ZL15levelTextAnchorv` (`t`) | **`0x28` (40)** | `0x81828`..`0x81850` | Level.cpp | **yes** — section-order line 889 places it between `ArrayAdd<int>` and `GameText::getText`; creates the `+0x28` gap. Also has a 4-byte `.bss` `layoutPad` at `0x218fa4` |
| 8 | `src/engine/core/GameText.cpp:59` | `_Z16arabicFormOffsetj` (`T`, hidden) | **`0x10` (16)** | `0x7efc4`..`0x7efd4` | GameText.cpp | **yes** — real helper, not dead; creates the `+0x10` gap before `GameText::getRegionCode` |
| 9 | `src/game/mission/RecordHandler.cpp:1415` | `_Z8is_equalPKSt9type_infoS1_b` (`T`, hidden) | **`0x20` (32)** | `0xcde10`..`0xcde30` | RecordHandler.cpp | **yes** — real libc++abi helper; creates the `+0x20` gap |
| 10 | `src/game/weapons/ObjectGun.cpp` | `_Z23ObjectGunProjectileMeshi` (`t`) | **`0x10` (16)** | `0x160304`..`0x160314` | ObjectGun.cpp | **yes** — real helper used by ctor/update; creates the `+0x10` gap before `ObjectGun::C1` |
| 11 | `src/game/world/LevelScript.cpp:44` | `_ZL12s_processPad` | **`0x20` (32) `.bss`** | — | LevelScript.cpp | **yes** — volatile `int[8]` written in `LevelScript::process`; codegen/register-allocation anchor |
| 12 | `src/game/mission/Item.cpp:17` | `_ZL30canBeInstalledMultipleTimesPad` | **`0x108` (264) `.rodata`** | — | Item.cpp | **yes** — `[[gnu::used]]`; holds the `canBeInstalledMultipleTimes` literal distance exact |
| 13 | `src/engine/file/AEFile.cpp:8` | `_ZL12pakExtension` | **`0x8` `.aezdata`** | `0x21296c` | AEFile.cpp | **yes for current layout** — sole `.aezdata` content; original has no `.aezdata` |
| 14 | `src/game/mission/Status.cpp:2050` | `_ZL12kMissionOrder` | **`0xbe` (190) declared, 0 emitted** | — | Status.cpp | **no** — never referenced; object has no `.bss`; stale comment |
| 15 | `src/game/mission/Status.cpp:2053` | `_ZL12kMissionCase` | **`0xbe` (190) `.rodata`** | — | Status.cpp | real table, not an anchor |
| 16 | `src/game/ui/HangarWindow.cpp:960,964,1064` | `volatile int pad0/pad1/textYbarrier`; `description = description;` ×3 at `:969-971` | in-function stores | — | HangarWindow.cpp | codegen anchors; `render` is size-exact 9700 |
| 17 | `src/game/weapons/RocketGun.cpp:232,289` | `volatile int padx`, `volatile int unused` | in-function stores | — | RocketGun.cpp | codegen anchors; `update` is size-exact 1908 |
| 18 | `src/game/weapons/Gun.cpp:699` | `volatile uint8_t hitFlag` read | in-function | — | Gun.cpp | codegen anchor in `calcCharacterCollision` |

Observed totals of the separate-section `.text` anchors/helpers in the table:
`0xb76 + 0xe + 0x8 + 0x4 + 0x34 + 0x8 + 0x28 + 0x10 + 0x20 + 0x10 = 0xc34`
(3124 B) in object bytes; the linked gaps sum to `0xb78 + 0x10 + 0x8 + 0x4 + 0x34 +
0x8 + 0x28 + 0x10 + 0x20 + 0x10 = 0xc38` (3128 B) because of alignment. The
MenuTouchWindow anchor alone is `0xb78 / 0xc08 = 95.3%` of the `.text` delta.

Data anchors: `canBeInstalledMultipleTimesPad` 264 B `.rodata`, `pakExtension` 8 B
`.aezdata`, `s_processPad` 32 B `.bss`, `levelTextAnchor::layoutPad` 4 B `.bss`.
`kMissionOrder` is currently 0 B.

## 4. NEW LEAD: the `.ARM.exidx` missing unwind entries

### 4.1 Measured tables

Both `.ARM.exidx` tables are arrays of 8-byte entries: `word1 = prel31(function)`,
`word2 = prel31(unwind)`; `word2 == 1` is `EXIDX_CANTUNWIND`; `word2 & 0x80000000`
is a compact inline unwind record; otherwise `word2` is a prel31 to `.ARM.extab`.

| | original | ours | delta |
|---|---:|---:|---:|
| `.ARM.exidx` address | `0x19cc08` | `0x19d810` | `+0xc08` |
| `.ARM.exidx` size | `0x6f70` | `0x66c8` | `-0x8a8` |
| entries | **3566** | **3289** | **-277** |
| `CANTUNWIND` entries | **272** | **6** | -266 |
| real entries | 3294 | 3283 | -11 |
| compact inline | 856 | 842 | -14 |
| `.ARM.extab` references | 2438 | 2441 | +3 |
| unique function addresses | 3372 | 3288 | -84 |
| duplicate addresses | **194** | **1** | -193 |

The task's "267 functions" is a miscount: `0x6f70 - 0x66c8 = 0x8a8 = 2216 B = 277`
entries. The task's "3299 entries" is also off; `0x66c8 / 8 = 3289`.

The deficit splits exactly as:

```text
266 extra CANTUNWIND entries in original
+ 11 extra real entries in original
= 277
```

### 4.2 Classification of the 272 original CANTUNWIND entries

`/tmp/cant_classify.py` / `/tmp/cant_table.py` classify each CANT entry by its
position relative to the preceding exported function and by the simulated end of the
owning object's `.text`:

| class | count | meaning |
|---|---:|---|
| `tu-end` | **172** | at the end of an object's `.text` (one per TU; alignment padding follows the last function) |
| `gap0-alias` | **81** | at `gap == 0` after a non-last function; a zero-size alias at the start of the next function (C1/C2, D1/D2, or a TU boundary with no padding) |
| `gap>0` | **18** | inside archive members (zlib/libzip/libc++abi), at an alignment gap after the preceding function |
| `no-prev` | **1** | `0x69534`, before the first exported function (crtbegin) |

The 172 `tu-end` entries are the key structural finding: **the original emits one
end-of-TU `CANTUNWIND` entry per object; our build emits none.** For 166 of our 193
objects, the object's last-function end maps exactly to an original CANT entry. Our
own `.ARM.exidx` has a CANT entry at the end of **0** of 193 objects (the 6 CANT
entries we do have are at `0x69534`, `0x7fb7c`, `0x1901b8`, `0x192804`, `0x192970`,
and the `0x19d810` end-of-`.text` sentinel).

The 81 `gap0-alias` entries are mostly the CANT half of a CANT+real pair at the same
address. The 18 `gap>0` entries are all archive members:

```text
0x18989a deflateCopy            gap 0xaf4
0x189c34 crc32_combine          gap 0x38
0x18bf06 _tr_tally              gap 0x516
0x18ca4c zip_source_buffer      gap 0x88
0x18d6f0 zip_close              gap 0x48
0x18db24 _zip_source_file_or_p  gap 0x138
0x18e020 zip_source_zip         gap 0xf0
0x18eb78 zip_open               gap 0x238
0x1922a8 __cxa_get_globals_fast gap 0x50
0x192d68 __cxa_call_unexpected  gap 0x1bc
0x192fd4 __cxa_deleted_virtual  gap 0x24c
0x193c0e __dynamic_cast         gap 0x512
0x193ec4 _ZNKSt10bad_typeid4whatEv gap 0x60
0x194030 _ZSt13set_terminatePFvvE gap 0x10c
0x19ae14 __cxa_demangle         gap 0x6bac
0x19ae52 __cxa_demangle         gap 0x6bea
0x19cc08 __cxa_demangle         gap 0x89a0 (end-of-.text sentinel)
```

### 4.3 The 194 duplicate addresses

Original has 194 addresses with more than one exidx entry; ours has 1.

| pattern | count | notes |
|---|---:|---|
| `CANT` + `real` at same address | **193** | the CANT half is the alias/sentinel |
| `real` + `real` at same address | 1 | `0x6db20`: `getAppVersion` (size 0) and `Engine::InitGL` share the address; original keeps both real entries, ours keeps only `InitGL` |

Of the 193 CANT+real pairs:

| category | count |
|---|---:|
| C1/C2 constructor aliases | **114** |
| D1/D2 destructor aliases | **1** |
| single exported name | **74** |
| local address | **4** |
| multi-name other | **1** |

Examples of the 74 single-name pairs: `_Z24IsInGameSubMenuNotActivei` `0x69e5c`,
`opensubkeyfile` `0x6b2a0`, `JNI_OnLoad` `0x6b550`, `glShadeModel` `0x6dac0`,
`_Z4logiPc` `0x6ddf0`, `ImageRelease` `0x6ee94`, `CameraSetPerspective` `0x7130c`,
`String::Reverse` `0x722e8`, `AEMath::Vector::operator float*` `0x7395c`,
`IApplicationModule::SetApplicationManager` `0x74154`, `CurveCreate` `0x7415c`,
`AESoundRessource::getSoundInfo` `0x7fa30`, and so on. Most are the end-of-TU
sentinel coinciding with the next function's start.

### 4.4 The 17 unique exported functions with a real original entry but no ours entry

`/tmp/exidx_cmp.py` compares the common exported functions. There are 17 unique
addresses (25 names because of C1/C2 pairs) where the original has a real entry and
ours has none:

| orig addr | function(s) | orig size | ours addr | TU | orig pattern |
|---:|---|---:|---:|---|---|
| `0x69588` | `NFC::C1/C2` | 2 | `0x69588` | `src/engine/core/NFC.cpp` | C,R |
| `0x6b9d8` | `Java_..._GetLeaderboardScore` | 16 | `0x6b9d8` | `src/platform/android/main.c` | C,R |
| `0x74154` | `IApplicationModule::SetApplicationManager` | 8 | `0x7414c` | `src/engine/core/IApplicationModule.cpp` | C,R |
| `0x8012c` | `AEMath::Matrix::operator float*` | 2 | `0x80140` | `src/engine/math/Matrix.cpp` | C,R |
| `0x81008` | `Quaternion::C1/C2` | 2 | `0x8101a` | `src/engine/math/Quaternion.cpp` | R |
| `0x83ee8` | `glError` | 4 | `0x83f1e` | `src/engine/render/Engine.cpp` | R |
| `0x88490` | `DrawFBOShader::C1/C2` | 48 | `0x884c0` | `src/engine/render/shaders/DrawFBOShader.cpp` | R |
| `0x9539c` | `Generator::C1/C2` | 2 | `0x953cc` | `src/game/mission/Generator.cpp` | C,R |
| `0x97bfc` | `Objective::C1/C2` | 18 | `0x97c2c` | `src/game/mission/Objective.cpp` | C,R |
| `0x11d9f4` | `HangarList::C1/C2` | 10 | `0x11da74` | `src/game/core/HangarList.cpp` | C,R |
| `0x1757a0` | `ModMainMenu::C1/C2` | 36 | `0x1763b8` | `src/game/menu/ModMainMenu.cpp` | C,R |
| `0x1869f4` | `inflateReset` | 64 | `0x18760a` | zlib | R |
| `0x18ca54` | `zip_get_archive_comment` | 44 | `0x18d662` | libzip | R |
| `0x18db7c` | `zip_strerror` | 6 | `0x18e780` | libzip | C,R |
| `0x18dc10` | `zip_set_archive_flag` | 18 | `0x18e810` | libzip | C,R |
| `0x193dfc` | `std::type_info::D1/D2` | 2 | `0x1947fc` | libc++abi | C,R |
| `0x193ec4` | `_ZSt14set_unexpectedPFvvE` | 48 | `0x194864` | libc++abi | C,R |

12 of the 17 have a CANT+real pair in the original; 5 have only a real entry
(`Quaternion::C1/C2`, `glError`, `DrawFBOShader::C1/C2`, `inflateReset`,
`zip_get_archive_comment`). Ours has **one** entry the original lacks:
`__cxa_end_cleanup` at `0x192970` (size 0, CANTUNWIND).

These 17 are **functions we emit but whose exidx entry gold absorbed into a run of
adjacent entries with identical unwind info** (`--merge-exidx-entries` keeps the
first entry of the run and drops the rest). They are not missing functions: all 17
are `linked: true` in `verify/report.json` and their bodies are present. Example:
`Quaternion::C1()` at `0x8101a` is a 2-byte `bx lr`; the preceding `MatrixDebugOut`
and the following `Quaternion::C1(float,float,float,float)` are the same unwind shape,
and our table has no entry anywhere in `0x80ff0..0x8102a`.

### 4.5 Are any local functions missing? No

- Our local function set (3954 local symbols across the object files) covers the
  original local addresses at the same relative offsets; the simulation in
  `/tmp/simulate3.py` reproduces our `.text` layout from `section_order.txt` +
  `link_order.rsp` + object sections with **0 mismatches over 3938 mapped symbols**.
- Original exidx entries at local addresses: 406 (82 CANT, 324 real).
  Ours: 333 (3 CANT, 330 real). Ours has **6 more real local entries** than the
  original, not fewer.
- The 277-entry deficit is therefore `266 CANT + 11 real`. The 11 real are the 17
exported functions above minus 6: ours has 330 real local entries vs the original's
324, so 6 extra real local entries exactly compensate 6 of the 17. No local function
body is missing from `.text`.

### 4.6 The section-ordering experiment (the actionable part)

The deficit is a **link-order artifact**, not a source-code deficit. Relinking the
same objects with the same gold 1.12 but different `--section-ordering-file`
arguments changes the exidx dramatically:

| link | `.text` size | exidx size | entries | CANT | compact | extab | unique | dups |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| original | `0x1337c8` | `0x6f70` | **3566** | **272** | 856 | 2438 | 3372 | 194 |
| ordered (`section_order.txt`, shipped) | `0x1343d0` | `0x66c8` | **3289** | **6** | 842 | 2441 | 3288 | 1 |
| no ordering file | `0x13442c` | `0x7018` | **3587** | **275** | 871 | 2441 | 3390 | 197 |
| object-order ordering file | `0x134400` | `0x6958` | **3371** | **82** | 848 | 2441 | 3319 | 52 |
| ordered + `.ARM.exidx.*` listed | `0x1343d0` | `0x66c8` | **3289** | **6** | 842 | 2441 | 3288 | 1 |
| ordered + `--no-merge-exidx-entries` | `0x1343d0` | `0x8dc0` | **4536** | **6** | 2089 | 2441 | 4535 | 1 |
| ordered, gold 1.11 (r13b) | `0x1343d0` | `0x66c8` | **3289** | **6** | 842 | 2441 | 3288 | 1 |
| bfd (no ordering file) | `0x1326b8` | `0x66a8` | **3285** | **6** | 838 | 2441 | 3285 | 0 |

Key observations:

- The **no-ordering-file** gold link reproduces the original's exidx structure almost
  exactly: 3587 vs 3566 entries, 275 vs 272 CANT, 871 vs 856 compact, 2441 vs 2438
  extab, 197 vs 194 dups. Its `.text` layout is wrong (size `0x13442c`, function
  addresses shifted), so it is not a drop-in fix, but it proves the CANT entries are
  present in the objects and are being **dropped by the ordered link**, not missing.
- Adding the `.ARM.exidx.*` sections to the ordering file does **not** restore them
  (still 3289/6). So the effect is not simply "the exidx sections are not ordered".
- `--no-merge-exidx-entries` does **not** restore them either (4536/6); it only
  expands the compact entries. So the CANT drop is not the `--merge-exidx-entries`
  pass.
- Gold 1.11 (r13b) gives the same 3289/6, so it is not a gold-version difference.
- The object-order ordering file gives an intermediate 3371/82, showing the CANT
  count is a sensitive function of the exact `.text.*` order.

**Inference:** gold's `--section-ordering-file` reorders the `.text.*` input sections
but leaves the `.ARM.exidx.text.*` input sections in object order. When the two
orders disagree, gold's exidx writer drops the per-TU trailing CANT entry (or
associates it with the wrong section). The original link either used no ordering file
(and the original `.text` order happened to be the object order) or used a linker
that preserves the per-TU sentinel. This is the single highest-value lead for the
`.ARM.exidx` gap; it is a **build/link** investigation, not a `src/` reconstruction.

### 4.7 `.ARM.extab` `+12 B`

The `.ARM.extab` difference is **not** a missing-function issue. The first byte
difference is at offset `0x244`, but that is a prel31 shift artifact: the record at
`0x244` belongs to `Mesh::Mesh(Mesh*)` at `0x6ba90` and its first word differs by
exactly the `0x360` base shift (`0x7febada4` vs `0x7febaa44`). A word-level diff of
the two extab sections shows many records changed (the functions are not byte-exact,
so their unwind opcodes differ); the `+12` is the net length difference. The common
suffix is 586 bytes. Do not read the `+12` as "12 missing bytes of unwind info".

### 4.8 TU mapping of the CANT entries

The 172 `tu-end` entries are spread across essentially every object. The
`/tmp/cant_table.py` per-TU summary shows one `tu-end` per TU for the engine, game,
and platform TUs, plus 63 `gap0-alias` + 17 `gap>0` in the archives. Representative
engine TUs from the source-layout dumps:

```text
Camera.cpp (CameraIsSphereinViewFrustum end)
Layout.cpp (OnTouchEnd end)
Level.cpp (incNumDeliveredPassengers end)
TexOnlyShader.cpp (D0Ev+0x10)
Array.cpp (4x)
SystemPathFinder.cpp
GamePadHandler.mm (IsInGameSubMenuNotActive / IsDialogNotVisible)
GameInit.cpp
GameResource.cpp (BuildResourceList)
PaintCanvas.cpp
ImageFont.cpp
Image.cpp
Material.cpp
SpriteSystem.cpp
ApplicationManager.cpp
Math16_iPhone.cpp (Sqrtf)
Mesh.cpp
BlowfishSubkeyFile
NFC
main.c
zlib / libzip / libc++abi archives
```

The full per-TU list is in `/tmp/cant_table.out`; the script is deterministic and
can be re-run from the note's commands.

## 5. Ranked fix list with current expected byte effects

The old audit's ranking was based on 22 size-mismatched functions netting -1,230 B.
At this HEAD all functions are size-exact except `Generator::createMission` (-4), so
the ranking changes substantially.

### Rank 1 — `.ARM.exidx` link-order investigation (266 CANT + 11 real entries)

**Expected effect:** up to `+0x8a8` (2216 B) of `.ARM.exidx` size and 277 entries;
zero `.text` change. The no-order link proves 275 CANT entries are available in the
objects. This is the largest single section delta after `.text` and is entirely a
link/build question.

**Action:** compare the original's `.text` order with the object order for the first
few TUs; try a gold link with the `.text.*` order that matches the original but with
the `.ARM.exidx.text.*` sections in the same order (the interleaved test above did
not work, so inspect gold's `--section-ordering-file` handling of `.ARM.exidx`).
Do not add source anchors for this.

**Risk:** low for `src/` (no source change); medium for the gate because `.text`
addresses must stay fixed.

### Rank 2 — `MenuTouchWindow` 0xb78 anchor (95% of the `.text` delta)

**Expected effect:** removing the anchor body alone gives `.text = 0x1343d0 -
0xb78 = 0x133858`, which is 0x90 (144 B) **short** of the original `0x1337c8` —
exactly the "remaining 144" in §2. Removing the anchor also removes the
`.data.rel.ro` relocation and changes the `.got`/ctor layout, so it must be a
coordinated pass.

**Action:** recover the real MenuTouchWindow code that the anchor is standing in for;
the anchor is `0xb76` bytes of volatile stores and a self-call, not a real body.
Pair it with the 144 B of non-anchor net (prologue +130, archive -600, tail +616,
size -2) and re-check the ten exception ctors.

**Risk:** high; this is the page-boundary anchor.

### Rank 3 — small prologue anchors/helpers (`+0x82` = 130 B)

The prologue +130 B is the net of the positive contributors below and the small
negative/2-byte alignment gaps (FileInterfaceAndroid -12, ConfigReader -8,
PlayerFighter/PlayerCreature -4 each, and many -2 cases); the positive entries alone
sum to 204, and the negative/alignment cases net it down to +130.

| gap | bytes | cause |
|---|---:|---|
| `ArrayAddCached<unsigned int>` → `GameText::getRegionCode` | +16 | `arabicFormOffset` helper |
| `ArrayAdd<int>` → `GameText::getText` | +40 | `levelTextAnchor` (Level.cpp) |
| `Level::setPlayerEngineColor` → `initParticleSystems` | +8 | `levelMissionPad` |
| `Level::incNumDeliveredPassengers` → `StarMap::C1` | +8 | `StarMap_GetScreenPosition` |
| `RecordHandler::readOptionsFileAsByteArray` → `writeByteArrayAsOptionsFile` | +32 | `is_equal` helper |
| `ModStation::OnTouchMove` → `OnTouchEnd` | +52 | `modStationTouchEndPad` |
| `MTitle::OnTouchEnd` → `OnRelease` | +4 | `generatorMissionLayoutPad` |
| `AESoundRessource` | +12 | two alignment gaps |
| `ListItemWindow::OnTouchEnd` → `StarSystem::C1` | +16 | `starSystemLayoutAnchor` |
| `MarqueeImage::draw` → `ObjectGun::C1` | +16 | `ObjectGunProjectileMesh` helper |
| 2-byte alignment and small negative gaps | remainder | Thumb alignment |

**Expected effect:** each removal is exact; the 2-byte alignment cases are noise.
Do them as one batch after Rank 2, with a full gate.

**Risk:** medium; several are real helpers, not dead code (`arabicFormOffset`,
`is_equal`, `ObjectGunProjectileMesh`).

### Rank 4 — the `+0x268` (616 B) tail after `__cxa_demangle`

The tail is local libc++abi demangler code + padding. It is not covered by
`local_funcs.json` (which stops at `0x187604`, the start of the zlib archive).
**Expected effect:** up to -616 B of `.text` if the local demangler code is made
byte-identical or the archive member selection is corrected. **Risk:** medium; this
is archive-local code, not a game TU.

### Rank 5 — data anchors (`.rodata`, `.aezdata`, `.bss`)

- `canBeInstalledMultipleTimesPad` 264 B `.rodata`: load-bearing for the
  `canBeInstalledMultipleTimes` literal distance. Removing it moves `.rodata` -264
  and may break the `Item` literal.
- `pakExtension` 8 B `.aezdata`: the only `.aezdata` content; the original has no
  `.aezdata`. Removing it removes the extra section and moves `.got`/`.data`.
- `s_processPad` 32 B `.bss`: codegen anchor in `LevelScript::process`.
- `kMissionOrder` 190 B declared but 0 emitted: stale; either wire it up or delete
  the declaration (no byte effect today).

**Expected effect:** `.rodata` -264, `.aezdata` -8 (section removed), `.bss` -32;
`.got`/`.data` addresses move. **Risk:** high; page-quantized `.got`.

## 6. Observed vs inferred

**Observed**

- All section addresses/sizes in §1; all gate counts.
- The two function-body deltas (`getAppVersion +2`, `Generator::createMission -4`).
- Every `gd` transition in §2.5 and the region sums in §2.3.
- All anchor object sizes and linked addresses in §3 (`nm -S` on the `.obj` files and
  `nm -nS` on the unstripped relink).
- The exidx counts in §4.1, the 172/81/18/1 classification, the 194 duplicate
  addresses, the 17-function list, and the link-variant table in §4.6.
- The `.ARM.extab` first-difference offset `0x244` and the `0x360` base-shift
  explanation.

**Inferred**

- The exact gold mechanism that drops the per-TU CANT entries when
  `--section-ordering-file` reorders `.text.*` but not `.ARM.exidx.text.*` (§4.6).
  The link variants are measured; the "gold associates exidx with text order"
  explanation is the most consistent hypothesis, not proven from gold source.
- That the original link used no ordering file or a linker that preserves the
  per-TU sentinel. The original's `.text` order is the `section_order.txt` order, so
  a no-order link is not directly usable; the original may have used a different
  input-object order that happened to match.
- The "remaining 144 B" decomposition is exact arithmetic; the attribution of the
  archive `-600` to member placement (rather than a source delta) is inferred from
  the cancellation pattern.
- The ranking and expected byte effects in §5 are recommendations, not measured
  results.

## 7. Verification recipe

After any change:

```sh
cmake --build cmake-build-match --target link_so -j4
cmake --build cmake-build-match --target verify -j4
FN=_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE cmake --build cmake-build-match --target verify-fn
readelf -SW cmake-build-match/libgof2hdaa.so | rg '\.text|\.ARM.exidx|\.ARM.extab|\.rodata|\.got|\.data|\.bss|\.aezdata'
python3 /tmp/exidx.py            # entry/cant counts
python3 /tmp/exidx_cmp.py        # exported-entry diff
python3 /tmp/cant_classify.py    # 172/81/18/1 classification
python3 /tmp/delta_full.py       # .text delta transitions
python3 /tmp/gap_table.py        # per-gap + per-TU sums
```

Check:

- `byte_exact` does not decrease; `linked_exact` does not persistently decrease.
- `.text` is still `0x1343d0` (or the coordinated new page point).
- `.ARM.exidx` entry count and CANT count move toward 3566/272.
- No new undefined imports; no new exported globals; lint stays clean; drift 0.

## 8. Scripts and artifacts used

All are in `/tmp` and reproducible from this note:

| script | purpose |
|---|---|
| `/tmp/delta_full.py` | per-function size/gap/start-delta transitions |
| `/tmp/region_sum.py` | region sums of `gd` |
| `/tmp/gap_table.py` | per-gap table + by-TU sums |
| `/tmp/exidx.py` | raw exidx counts |
| `/tmp/exidx_cmp.py` | exported-entry diff |
| `/tmp/exidx_types.py` | entry-type transition counts |
| `/tmp/exidx_map.py`, `/tmp/exidx_map2.py` | address-mapped exidx comparison |
| `/tmp/cant_classify.py` | 172/81/18/1 CANT classification |
| `/tmp/cant_attr.py`, `/tmp/cant_table.py` | per-TU CANT attribution |
| `/tmp/cant_map.py` | CANT local-address mapping |
| `/tmp/build_objmap.py` | symbol → object map (`/tmp/objmap.json`) |
| `/tmp/simulate3.py` | section-order layout simulation (`/tmp/sim_assigned3.json`) |
| `/tmp/linkcmd.txt` | exact gold link command |
| `/tmp/link_exidxorder.sh`, `/tmp/link_noorder2.sh`, `/tmp/link_objorder.sh`, `/tmp/link_gold11.sh`, `/tmp/link_bfd.sh` | link variants in §4.6 |
| `/tmp/test_exidxorder.so`, `/tmp/test_noorder2.so`, `/tmp/test_objorder.so`, `/tmp/test_gold11.so`, `/tmp/test_bfd.so`, `/tmp/test_nomerge.so` | unstripped relink artifacts |

## 9. Top findings

1. **`.text` is now almost entirely one anchor.** All 4523 compared functions are
   size-exact except `Generator::createMission` (-4); `getAppVersion` is +2. The
   `+0xc08` is `+0xb78` MenuTouchWindow anchor + `+0x82` prologue gaps - `600`
   archive placement + `+0x268` tail - `2` body delta. The old audit's 22
   size-mismatch story is obsolete.
2. **The `.ARM.exidx` deficit is 277 entries, not 267.** It is `266 CANT + 11 real`.
   The original's 272 CANT entries decompose into 172 per-TU end-of-text sentinels,
   81 gap-0 aliases, 18 archive padding entries, and 1 pre-first-function entry; it
   also has 193 CANT+real alias pairs. Our ordered link emits 6 CANT and 1 duplicate.
3. **The exidx deficit is a link-order artifact.** A no-ordering-file gold link of
   the same objects gives 3587 entries / 275 CANT, almost exactly the original's
   3566 / 272. Adding `.ARM.exidx.*` to the ordering file or using
   `--no-merge-exidx-entries` does not restore them; the CANT count is a sensitive
   function of the exact `.text.*` order.
4. **Only 17 unique exported functions genuinely lack a real exidx entry in ours**
   (25 names), and all are `linked: true` bodies. They are gold-merged entries, not
   missing functions.
5. **No local function is missing.** Our local set covers the original local
   addresses; ours has 330 real local entries vs the original's 324.
6. **The `.ARM.extab` `+12` is not 12 missing bytes.** It is the net length of
   different unwind records for non-byte-exact functions; the first byte difference
   at `0x244` is a prel31 base-shift artifact.
7. **Stale anchors to clean up:** `kMissionOrder` (declared, 0 emitted),
   `Transform.cpp` 72-byte `.data` block (gone), `RocketGun::kRocketTrailSets`
   (gone), `HangarWindow` six self-assignments (now 3).
