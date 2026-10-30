> **Current frontier (2026-10-30, wave 87):** **every exported function is size-exact**
> (4523/4523). Gate: `byte_exact 1412`, `linked_exact 4485`, avg 99.88%,
> `.text 0x1343d0`, lint CLEAN, drift 0. Wave 87 attacked the worst-match queue: Gun::calcCharacterCollision
> 74.9 -> 77.2%, Layout::Layout 45.4 -> 45.5% (7072 B), Radar::draw 71.40 -> 71.43%,
> createCampaignMission 96.9 -> 97.0%, LevelScript::process 51.6 -> 51.9%, MeshReadData
> 77.1 -> 78.1%, MenuTouchWindow::draw 95.1 -> 96.2%, drawChallengeModeScore 81.5 -> 83.8%,
> StarMap::drawOnScreenInfo 98.2 -> 99.8%, SpaceLounge::startChat 97.4 -> 99.1%,
> PlayerEgo::update 98.6 -> 98.9%, MGame::OnUpdate 99.7 -> 99.8%,
> ModStation::OnKeyPress 99.7 -> 99.9%, MenuTouchWindow::OnTouchEnd 84.9%. The remaining work is fuzzy/register-allocation matching
> for the 38 non-linked-exact functions; the detailed table below is the wave-69 snapshot and is
> retained for address-order history.
>

> **Current frontier (2026-10-30, wave 84):** **every exported function is now size-exact**
> (4523/4523). `Layout::Layout` landed its 7072 B schedule with a 280 B `Item.cpp` `.rodata` pad
> (43.1%), and `Generator::createMission` landed a pool-loaded `0.5f` cost factor at 1996 B (73.6%)
> with the `MTitle` anchor removed. Gate: `byte_exact 1412`, `linked_exact 4485`, avg 99.87%,
> `.text 0x1343d0`, lint CLEAN, drift 0. The remaining work is fuzzy/register-allocation matching
> for the non-byte-exact functions; the detailed table below is the wave-69 snapshot and is
> retained for address-order history.
>
Generated from the current `wave69-w1744` build (commit `4bd072cb` + this pass) by
walking the common `.dynsym` function symbols of both binaries in original-address order.
Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so` (image base `0x10000`).

## Snapshot

| item | original | ours | delta |
|---|---:|---:|---:|
| `.text` start | `0x69440` | `0x69440` | `0` |
| `.text` size | `0x1337c8` (1259464 B) | `0x1343d0` (1262544 B) | `+0xc08` (+3080 B) |
| `.text` end | `0x1ccc08` | `0x1cd810` | `+0xc08` |
| exported functions (common) | 4524 | 4524 | 0 |
| address or size differs | — | — | **4373** |
| address differs | — | — | 4372 |
| size differs | — | — | 14 |
| address exact | — | — | 152 (3.36%) |
| address and size both exact | — | — | 151 |

Gate at generation time: `byte_exact 1361`, `linked_exact 4485`, `.text 0x1343d0`, lint CLEAN, drift 0.

The fleet's `0%` figure predates commit `4bd072cb` (the `.text.startup`/crtbegin ordering fix);
the current build measures 152/4524 = 3.36% of exports at their original address. The map below
is the queue for raising that number.

`Δaddr = ours - orig`; `Δsize = ours - orig`. Addresses are `st_value & ~1`.

## First divergence (the frontier)

- First **size** divergence: `AbyssEngine::MeshReadData(AbyssEngine::Engine*, unsigned int const&, unsigned int, AbyssEngine::Mesh**, AbyssEngine::Material*)` at `0x06c608` — original 2632 B, ours 2624 B (`-8` B), match 76.5%.
- First **address** divergence: `void ArrayAdd<AbyssEngine::Mesh*>(AbyssEngine::Mesh*, Array<AbyssEngine::Mesh*>&)` at `0x06d050` — ours `0x06d048` (`-8` B).

The first 127 exports are address- and size-exact. `MeshReadData` is the 128th: its address still
matches, but its body is 8 B short, so the next 34 exports run at `-8` B. Fixing that one body
therefore re-aligns those 34 exports (152 -> 186 address-exact exports, 3.36% -> 4.11%) until the
next size difference (`getAppVersion`/`Engine::InitGL`).

## Root causes: exported size differences (14)

These are the only exported functions whose own body length differs. Each one changes the address
delta of every function after it. `drift after` is the address delta of the next exported function.

| # | orig addr | function | orig B | ours B | ΔB | match | drift after |
|---:|---|---|---:|---:|---:|---:|---:|
| 1 | `0x06c608` | `_ZN11AbyssEngine12MeshReadDataEPNS_6EngineERKjjPPNS_4MeshEPNS_8MaterialE`<br>AbyssEngine::MeshReadData(AbyssEngine::Engine*, unsigned int const&, unsigned int, AbyssEngine::Mesh**, AbyssEngine::Material*) | 2632 | 2624 | -8 | 76.5% | -8 |
| 2 | `0x06db20` | `_ZN11AbyssEngine13getAppVersionEv`<br>AbyssEngine::getAppVersion() | 0 | 2 | +2 | ?% | -4 |
| 3 | `0x0971b8` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`<br>Generator::createMission(Agent*, Array<SolarSystem*>*) | 1996 | 1992 | -4 | 73.8% | +44 |
| 4 | `0x0affd8` | `_ZN5Level13createMissionEv`<br>Level::createMission() | 17172 | 17176 | +4 | 80.4% | +36 |
| 5 | `0x0c0214` | `_ZN5Level10createShipEiiiP8Waypointbb`<br>Level::createShip(int, int, int, Waypoint*, bool, bool) | 2764 | 2788 | +24 | 71.8% | +64 |
| 6 | `0x0c90f4` | `_ZN7StarMap4drawEv`<br>StarMap::draw() | 2644 | 2648 | +4 | 92.9% | +76 |
| 7 | `0x0d1028` | `_ZN6LayoutC1Ev`<br>Layout::Layout() | 7072 | 6636 | -436 | 46.0% | -332 |
| 8 | `0x0d1028` | `_ZN6LayoutC2Ev`<br>Layout::Layout() | 7072 | 6636 | -436 | 46.0% | -332 |
| 9 | `0x121818` | `_ZN13PlayerStationC1EP7Station`<br>PlayerStation::PlayerStation(Station*) | 3440 | 3444 | +4 | 94.4% | -284 |
| 10 | `0x121818` | `_ZN13PlayerStationC2EP7Station`<br>PlayerStation::PlayerStation(Station*) | 3440 | 3444 | +4 | 94.4% | -284 |
| 11 | `0x12aabc` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv`<br>MenuTouchWindow::OnTouchEnd(int, int, void*) | 8132 | 8128 | -4 | 80.9% | -288 |
| 12 | `0x12e050` | `_ZN5Radar4drawEP6PlayerP3Hudi`<br>Radar::draw(Player*, Hud*, int) | 9908 | 9932 | +24 | 69.3% | +3072 |
| 13 | `0x137d18` | `_ZN11LevelScript7processEi`<br>LevelScript::process(int) | 57232 | 57252 | +20 | 50.4% | +3108 |
| 14 | `0x152f90` | `_ZN3Gun22calcCharacterCollisionEv`<br>Gun::calcCharacterCollision() | 1984 | 1964 | -20 | 68.9% | +3088 |

## Drift bands (87 contiguous runs of equal address delta)

Each row is a maximal run of consecutive exports that share one address delta. `transition` is the
change from the previous band; `prev size Δ` is the previous export's own size difference and
`local gap Δ` is the size difference of the non-exported code between the two exports.

| delta | start addr | count | first function | transition | prev size Δ | local gap Δ |
|---:|---|---:|---|---:|---:|---:|
| 0 | `0x069578` | 128 | `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed` | — | — | — |
| -8 | `0x06d050` | 33 | `_Z8ArrayAddIPN11AbyssEngine4MeshEEvT_R5ArrayIS3_E` | -8 | -8 | 0 |
| -4 | `0x06db20` | 1 | `_ZN11AbyssEngine6Engine6InitGLEbii` | +4 | 0 | +4 |
| -8 | `0x06db20` | 1 | `_ZN11AbyssEngine13getAppVersionEv` | -4 | 0 | -4 |
| -4 | `0x06dcb8` | 30 | `_ZN11AbyssEngine6Engine25SetFrameBufferScaleFactorEfi` | +4 | +2 | +2 |
| -14 | `0x06dfce` | 1 | `_ZN20FileInterfaceAndroidD0Ev` | -10 | 0 | -10 |
| -16 | `0x06dfe0` | 560 | `_ZN20FileInterfaceAndroid8OpenReadEN11AbyssEngine6StringEibiij` | -2 | 0 | -2 |
| 0 | `0x07efcc` | 24 | `_ZN8GameText13getRegionCodeEv` | +16 | 0 | +16 |
| +10 | `0x07fb74` | 2 | `_ZN11AbyssEngine16AESoundRessource8SetSoundEPKNS_11AESoundInfoEi` | +10 | 0 | +10 |
| +12 | `0x07fbd0` | 66 | `_ZN11AbyssEngine16AESoundRessource18initWithoutLoadingEi` | +2 | 0 | +2 |
| +10 | `0x081008` | 11 | `_ZN11AbyssEngine10QuaternionC1Ev` | -2 | 0 | -2 |
| +8 | `0x08103c` | 26 | `_ZN11AbyssEngine10Quaternion3SetERKNS_6AEMath6MatrixE` | -2 | 0 | -2 |
| +52 | `0x081818` | 80 | `_ZN8GameText7getTextEi` | +44 | 0 | +44 |
| +50 | `0x083ee8` | 1 | `_Z7glErrorv` | -2 | 0 | -2 |
| +52 | `0x083eec` | 8 | `_ZN11AbyssEngine6EngineC1Ev` | +2 | 0 | +2 |
| +56 | `0x084348` | 89 | `_ZN11AbyssEngine6Engine15ResetLightParamEv` | +4 | 0 | +4 |
| +48 | `0x08765c` | 427 | `_ZN5ArrayIPN11AbyssEngine11TokenStructEED2Ev` | -8 | 0 | -8 |
| +44 | `0x097984` | 157 | `_ZN6MTitleC2Ev` | -4 | -4 | 0 |
| +40 | `0x09bdf0` | 351 | `_ZN9PlayerEgo10handleShipEi` | -4 | 0 | -4 |
| +38 | `0x0a63e0` | 50 | `_ZN8ListItemC2EiiPN11AbyssEngine6StringE` | -2 | 0 | -2 |
| +36 | `0x0a6834` | 79 | `_ZN8ListItem12checkCreditsEv` | -2 | 0 | -2 |
| +34 | `0x0a8ac0` | 24 | `_ZN6StatusC1Ev` | -2 | 0 | -2 |
| +32 | `0x0a90b0` | 131 | `_ZN6Status10setStationEP7Station` | -2 | 0 | -2 |
| +36 | `0x0b42ec` | 1 | `_ZN5Level11createSceneEv` | +4 | +4 | 0 |
| +40 | `0x0b4bb8` | 20 | `_ZN5Level21createCampaignMissionEv` | +4 | 0 | +4 |
| +64 | `0x0c0ce0` | 71 | `_ZN5Level11createRouteEi` | +24 | +24 | 0 |
| +72 | `0x0c67a4` | 18 | `_ZN7StarMapC1EbP7Missionbi` | +8 | 0 | +8 |
| +76 | `0x0c9b48` | 2 | `_ZN7StarMap14missionChangedEv` | +4 | +4 | 0 |
| +72 | `0x0c9b90` | 25 | `_ZN7StarMap6updateEi` | -4 | 0 | -4 |
| +104 | `0x0cddd8` | 49 | `_ZN13RecordHandler27writeByteArrayAsOptionsFileEPai` | +32 | 0 | +32 |
| -332 | `0x0d2bc8` | 48 | `_ZN6LayoutD2Ev` | -436 | -436 | 0 |
| -328 | `0x0d52d0` | 13 | `_ZN10ModStationC1Ev` | +4 | 0 | +4 |
| -276 | `0x0d7600` | 55 | `_ZN10ModStation10OnTouchEndEiiPv` | +52 | 0 | +52 |
| -280 | `0x0dcfd8` | 179 | `_ZN13PlayerFighter6updateEi` | -4 | 0 | -4 |
| -282 | `0x11b710` | 11 | `_ZN4NodeC2Ei` | -2 | 0 | -2 |
| -284 | `0x11b7a8` | 71 | `_ZN16SystemPathFinder13getSystemPathEP5ArrayIP11SolarSystemEii` | -2 | 0 | -2 |
| -288 | `0x11cc40` | 136 | `_ZN14PlayerCreature5resetEv` | -4 | 0 | -4 |
| -284 | `0x122588` | 15 | `_ZN13PlayerStationD2Ev` | +4 | +4 | 0 |
| -286 | `0x122890` | 31 | `_ZN6WantedC2EiN11AbyssEngine6StringEiibiiiiiiiii` | -2 | 0 | -2 |
| -284 | `0x12299c` | 30 | `_ZN15MenuTouchWindowC1Ei` | +2 | 0 | +2 |
| -288 | `0x12ca80` | 11 | `_ZN15MenuTouchWindow13startValkyrieEv` | -4 | -4 | 0 |
| +3048 | `0x12d3c8` | 14 | `_ZN14PendingProductC2EP9BluePrint` | +3336 | 0 | +3336 |
| +3072 | `0x130704` | 33 | `_ZN5Radar12calcDistanceEffffff` | +24 | +24 | 0 |
| +3088 | `0x133b68` | 48 | `_ZN10StarSystemC2Ei` | +16 | 0 | +16 |
| +3108 | `0x145ca8` | 173 | `_ZN11LevelScript11resetCameraEP5Level` | +20 | +20 | 0 |
| +3088 | `0x153750` | 157 | `_ZN3Gun18calcLevelCollisionEv` | -20 | -20 | 0 |
| +3106 | `0x158164` | 2 | `_ZN12StatusWindowD2Ev` | +18 | 0 | +18 |
| +3104 | `0x1581d0` | 178 | `_ZN12StatusWindow25getRelativeScrollStartPosEv` | -2 | 0 | -2 |
| +3120 | `0x15f700` | 71 | `_ZN9ObjectGunC1EiP3GunijP5Level` | +16 | 0 | +16 |
| +3118 | `0x166bfc` | 2 | `_ZN14DialogueWindowC2EP7MissionP5Leveli` | -2 | 0 | -2 |
| +3116 | `0x166c58` | 77 | `_ZN14DialogueWindow4initEv` | -2 | 0 | -2 |
| +3120 | `0x169ad0` | 175 | `_ZN11SpaceLoungeC1Ev` | +4 | 0 | +4 |
| +3118 | `0x175e08` | 7 | `_ZN6GalaxyC2Ev` | -2 | 0 | -2 |
| +3120 | `0x175ea8` | 279 | `_ZN6Galaxy10getStationEi` | +2 | 0 | +2 |
| +3118 | `0x1869f4` | 2 | `inflateReset` | -2 | 0 | -2 |
| +3116 | `0x186a6c` | 50 | `inflateInit2_` | -2 | 0 | -2 |
| +3114 | `0x18c6c8` | 1 | `zip_file_strerror` | -2 | 0 | -2 |
| +3112 | `0x18c6d0` | 4 | `zip_fopen_index` | -2 | 0 | -2 |
| +3110 | `0x18ca54` | 4 | `zip_get_archive_comment` | -2 | 0 | -2 |
| +3108 | `0x18cb44` | 3 | `_zip_file_get_offset` | -2 | 0 | -2 |
| +3106 | `0x18cc78` | 1 | `_zip_new` | -2 | 0 | -2 |
| +3104 | `0x18ccc4` | 7 | `zip_close` | -2 | 0 | -2 |
| +3102 | `0x18d8f0` | 3 | `zip_source_free` | -2 | 0 | -2 |
| +3100 | `0x18d954` | 8 | `_zip_source_file_or_p` | -2 | 0 | -2 |
| +3098 | `0x18db84` | 1 | `_zip_entry_new` | -2 | 0 | -2 |
| +3096 | `0x18dbfc` | 2 | `zip_get_archive_flag` | -2 | 0 | -2 |
| +3094 | `0x18dc24` | 1 | `_zip_unchange_data` | -2 | 0 | -2 |
| +3096 | `0x18dc50` | 1 | `zip_fread` | +2 | 0 | +2 |
| +3094 | `0x18dd6c` | 1 | `zip_rename` | -2 | 0 | -2 |
| +3096 | `0x18ddec` | 2 | `zip_source_zip` | +2 | 0 | +2 |
| +3094 | `0x18e0ec` | 1 | `zip_source_function` | -2 | 0 | -2 |
| +3092 | `0x18e128` | 2 | `zip_replace` | -2 | 0 | -2 |
| +3090 | `0x18e1c4` | 4 | `zip_delete` | -2 | 0 | -2 |
| +3092 | `0x18e314` | 4 | `_zip_mkstemp` | +2 | 0 | +2 |
| +3090 | `0x18eb80` | 1 | `zip_fclose` | -2 | 0 | -2 |
| +3088 | `0x18ebf0` | 15 | `_zip_cdir_free` | -2 | 0 | -2 |
| +12868 | `0x18f5c0` | 1 | `zip_fopen` | +9780 | 0 | +9780 |
| +12866 | `0x18f5e4` | 1 | `zip_unchange_all` | -2 | 0 | -2 |
| +12864 | `0x18f618` | 1 | `_zip_error_strerror` | -2 | 0 | -2 |
| +2720 | `0x18f730` | 9 | `SHA224_Init` | -10144 | 0 | -10144 |
| +10240 | `0x18fe40` | 12 | `OPENSSL_atomic_add` | +7520 | 0 | +7520 |
| +2336 | `0x1900e0` | 2 | `sha256_block_data_order` | -7904 | 0 | -7904 |
| +3152 | `0x191d20` | 33 | `__cxa_end_cleanup` | +816 | 0 | +816 |
| +2564 | `0x19362c` | 1 | `__dynamic_cast` | -588 | 0 | -588 |
| +2562 | `0x193c10` | 11 | `_ZNSt9exceptionD1Ev` | -2 | 0 | -2 |
| +2560 | `0x193c18` | 50 | `_ZNKSt9exception4whatEv` | -2 | 0 | -2 |
| +2464 | `0x193ec4` | 3 | `_ZSt14set_unexpectedPFvvE` | -96 | 0 | -96 |

## Largest local-code gaps

Differences between consecutive exports that are not explained by an exported size difference,
i.e. non-exported (local/static) code or archive-member ordering. Top 25 by magnitude.

| Δgap | at orig addr | between | note |
|---:|---|---|---|
| -10144 | `0x18f730` | `_zip_error_strerror` → `SHA224_Init` | plain `.text` archive block (zip/libcrypto/sha) ordering |
| +9780 | `0x18f5c0` | `zip_stat_init` → `zip_fopen` | plain `.text` archive block (zip/libcrypto/sha) ordering |
| -7904 | `0x1900e0` | `OPENSSL_instrument_bus2` → `sha256_block_data_order` | plain `.text` archive block (zip/libcrypto/sha) ordering |
| +7520 | `0x18fe40` | `SHA224_Final` → `OPENSSL_atomic_add` | plain `.text` archive block (zip/libcrypto/sha) ordering |
| +3336 | `0x12d3c8` | `_ZN15MenuTouchWindow23startSupernovaChallengeEv` → `_ZN14PendingProductC2EP9BluePrint` |  |
| +816 | `0x191d20` | `sha256_block_data_order_neon` → `__cxa_end_cleanup` | plain `.text` compiler-runtime block (libc++abi/libgcc) ordering |
| -588 | `0x19362c` | `__cxa_deleted_virtual` → `__dynamic_cast` | plain `.text` compiler-runtime block (libc++abi/libgcc) ordering |
| -96 | `0x193ec4` | `_ZNKSt10bad_typeid4whatEv` → `_ZSt14set_unexpectedPFvvE` | plain `.text` compiler-runtime block (libc++abi/libgcc) ordering |
| +52 | `0x0d7600` | `_ZN10ModStation11OnTouchMoveEiiPv` → `_ZN10ModStation10OnTouchEndEiiPv` |  |
| +44 | `0x081818` | `_Z8ArrayAddIiEvT_R5ArrayIS0_E` → `_ZN8GameText7getTextEi` |  |
| +32 | `0x0cddd8` | `_ZN13RecordHandler26readOptionsFileAsByteArrayEPPa` → `_ZN13RecordHandler27writeByteArrayAsOptionsFileEPai` |  |
| +18 | `0x158164` | `_ZN12StatusWindow6reInitEv` → `_ZN12StatusWindowD2Ev` |  |
| +16 | `0x07efcc` | `_Z14ArrayAddCachedIjEvT_R5ArrayIS0_E` → `_ZN8GameText13getRegionCodeEv` |  |
| +16 | `0x133b68` | `_ZN14ListItemWindow10OnTouchEndEii` → `_ZN10StarSystemC2Ei` |  |
| +16 | `0x15f700` | `_ZN12MarqueeImage4drawEv` → `_ZN9ObjectGunC1EiP3GunijP5Level` |  |
| -10 | `0x06dfce` | `_ZN20FileInterfaceAndroidD2Ev` → `_ZN20FileInterfaceAndroidD0Ev` |  |
| +10 | `0x07fb74` | `_ZN5ArrayIPN11AbyssEngine16AESoundInterfaceEED2Ev` → `_ZN11AbyssEngine16AESoundRessource8SetSoundEPKNS_11AESoundInfoEi` |  |
| -8 | `0x08765c` | `_ZN11AbyssEngine12ConfigReader25RegisterTokenReadFunctionENS_6StringEPFvPS0_PvES3_` → `_ZN5ArrayIPN11AbyssEngine11TokenStructEED2Ev` |  |
| +8 | `0x0c67a4` | `_ZN5Level25incNumDeliveredPassengersEi` → `_ZN7StarMapC1EbP7Missionbi` |  |
| +4 | `0x06db20` | `_ZN11AbyssEngine6Engine17initFileInterfaceEv` → `_ZN11AbyssEngine6Engine6InitGLEbii` |  |
| +4 | `0x084348` | `_ZN11AbyssEngine6Engine11AfterGLInitEv` → `_ZN11AbyssEngine6Engine15ResetLightParamEv` |  |
| -4 | `0x09bdf0` | `_ZN9PlayerEgo4rollEi` → `_ZN9PlayerEgo10handleShipEi` |  |
| +4 | `0x0b4bb8` | `_ZN5Level11createSceneEv` → `_ZN5Level21createCampaignMissionEv` |  |
| -4 | `0x0c9b90` | `_ZN7StarMap24askForJumpIntoAlienWorldEv` → `_ZN7StarMap6updateEi` |  |
| +4 | `0x0d52d0` | `_ZN6Layout10OnTouchEndEii` → `_ZN10ModStationC1Ev` |  |

## Priority queue (read top to bottom)

1. **`MeshReadData` (-8 B, 76.5%)** at `0x6c608` — the first size divergence. Reconstruct the
   missing 8 B and the next 34 exports snap to delta 0 (152 -> 186 address-exact, 3.36% -> 4.11%).
2. **`getAppVersion` / `Engine::InitGL` region** (`0x6db20`–`0x6dcb8`) — original exports
   `getAppVersion` with size 0; ours emits a 2 B body. The following `FileInterfaceAndroid` block
   runs at `-16` until `0x7efcc`, where local code is 16 B larger and the delta returns to 0.
3. **`GameText`/`Engine` region** (`0x7efcc`–`0xcddd8`) — local code is cumulatively larger,
   pushing the delta from `0` to `+104`. The `GameText::getText` gap alone is `+44`.
4. **`Layout::Layout()` C1/C2 (-436 B each, same address `0xd1028`, 46.0%)** — the largest exported
   size deficit; changes the delta from `+104` to `-332`.
5. **`PendingProduct` region** (`0x12ca80`→`0x12d3c8`) — local code is `+3336` B larger, the largest
   single local gap; delta jumps `-288` → `+3048`.
6. **plain `.text` archive block** (`0x18c000`–`0x191d20`) — `zip_fopen`/`SHA224_Init`/
   `sha256_block_data_order`/`OPENSSL_atomic_add` appear in archive order, not original address
   order; spikes of `+12868`, `+10240`, `+2336`. Needs per-object section splitting or link-order
   changes, not a source edit.
7. **tail** (`0x193ec4` onward) — settles at `+2464`; remaining drift is local libc++abi/libgcc code.

## Full list (4373 entries)

Every exported function whose address or size differs, in original address order.
`match` is the verify fuzzy percentage; `B`/`L` mark byte-exact / linked-exact (from the report).

| # | orig addr | ours addr | Δaddr | orig B | ours B | ΔB | match | B | L | function |
|---:|---|---:|---:|---:|---:|---:|---:|:-:|:-:|---|
| 1 | `0x06c608` | `0x06c608` | 0 | 2632 | 2624 | -8 | 76.5% |  |  | `_ZN11AbyssEngine12MeshReadDataEPNS_6EngineERKjjPPNS_4MeshEPNS_8MaterialE`<br>AbyssEngine::MeshReadData(AbyssEngine::Engine*, unsigned int const&, unsigned int, AbyssEngine::Mesh**, AbyssEngine::Material*) |
| 2 | `0x06d050` | `0x06d048` | -8 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine4MeshEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::Mesh*>(AbyssEngine::Mesh*, Array<AbyssEngine::Mesh*>&) |
| 3 | `0x06d074` | `0x06d06c` | -8 | 824 | 824 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18MeshCreateFromFileEPNS_6EngineEPKcPPNS_4MeshEPNS_8MaterialE`<br>AbyssEngine::MeshCreateFromFile(AbyssEngine::Engine*, char const*, AbyssEngine::Mesh**, AbyssEngine::Material*) |
| 4 | `0x06d3ac` | `0x06d3a4` | -8 | 356 | 356 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8MeshDrawEPNS_6EngineEPNS_4MeshE`<br>AbyssEngine::MeshDraw(AbyssEngine::Engine*, AbyssEngine::Mesh*) |
| 5 | `0x06d510` | `0x06d508` | -8 | 720 | 720 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22MeshConvertToVBOInternEPNS_4MeshE`<br>AbyssEngine::MeshConvertToVBOIntern(AbyssEngine::Mesh*) |
| 6 | `0x06d7e0` | `0x06d7d8` | -8 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TransformConvertToVBOEPNS_9TransformE`<br>AbyssEngine::TransformConvertToVBO(AbyssEngine::Transform*) |
| 7 | `0x06d818` | `0x06d810` | -8 | 680 | 680 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13MeshIntersectEffPNS_4MeshE`<br>AbyssEngine::MeshIntersect(float, float, AbyssEngine::Mesh*) |
| 8 | `0x06dac0` | `0x06dab8` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glShadeModel`<br>glShadeModel |
| 9 | `0x06dac2` | `0x06daba` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glMatrixMode`<br>glMatrixMode |
| 10 | `0x06dac4` | `0x06dabc` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glVertexPointer`<br>glVertexPointer |
| 11 | `0x06dac6` | `0x06dabe` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glAlphaFunc`<br>glAlphaFunc |
| 12 | `0x06dac8` | `0x06dac0` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glLoadIdentity`<br>glLoadIdentity |
| 13 | `0x06daca` | `0x06dac2` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glScalef`<br>glScalef |
| 14 | `0x06dacc` | `0x06dac4` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glFogf`<br>glFogf |
| 15 | `0x06dace` | `0x06dac6` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glFogfv`<br>glFogfv |
| 16 | `0x06dad0` | `0x06dac8` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glLoadMatrixf`<br>glLoadMatrixf |
| 17 | `0x06dad2` | `0x06daca` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glTexEnvi`<br>glTexEnvi |
| 18 | `0x06dad4` | `0x06dacc` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glMultMatrixf`<br>glMultMatrixf |
| 19 | `0x06dad6` | `0x06dace` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glTexCoordPointer`<br>glTexCoordPointer |
| 20 | `0x06dad8` | `0x06dad0` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glNormalPointer`<br>glNormalPointer |
| 21 | `0x06dada` | `0x06dad2` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glColorPointer`<br>glColorPointer |
| 22 | `0x06dadc` | `0x06dad4` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glTexEnvf`<br>glTexEnvf |
| 23 | `0x06dade` | `0x06dad6` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glEnableClientState`<br>glEnableClientState |
| 24 | `0x06dae0` | `0x06dad8` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glClientActiveTexture`<br>glClientActiveTexture |
| 25 | `0x06dae2` | `0x06dada` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glMaterialfv`<br>glMaterialfv |
| 26 | `0x06dae4` | `0x06dadc` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glColor4f`<br>glColor4f |
| 27 | `0x06dae6` | `0x06dade` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glLightfv`<br>glLightfv |
| 28 | `0x06dae8` | `0x06dae0` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glDisableClientState`<br>glDisableClientState |
| 29 | `0x06daea` | `0x06dae2` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glLightModelfv`<br>glLightModelfv |
| 30 | `0x06daec` | `0x06dae4` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glHint`<br>glHint |
| 31 | `0x06daee` | `0x06dae6` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `glMaterialf`<br>glMaterialf |
| 32 | `0x06daf0` | `0x06dae8` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `_Z13AELabelObjectjjPKc`<br>AELabelObject(unsigned int, unsigned int, char const*) |
| 33 | `0x06daf2` | `0x06daea` | -8 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine20SetScreenOrientationENS_13LandscapeModeE`<br>AbyssEngine::Engine::SetScreenOrientation(AbyssEngine::LandscapeMode) |
| 34 | `0x06daf4` | `0x06daec` | -8 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine17initFileInterfaceEv`<br>AbyssEngine::Engine::initFileInterface() |
| 35 | `0x06db20` | `0x06db1c` | -4 | 408 | 408 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine6InitGLEbii`<br>AbyssEngine::Engine::InitGL(bool, int, int) |
| 36 | `0x06db20` | `0x06db18` | -8 | 0 | 2 | +2 | ?% |  |  | `_ZN11AbyssEngine13getAppVersionEv`<br>AbyssEngine::getAppVersion() |
| 37 | `0x06dcb8` | `0x06dcb4` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine25SetFrameBufferScaleFactorEfi`<br>AbyssEngine::Engine::SetFrameBufferScaleFactor(float, int) |
| 38 | `0x06dcba` | `0x06dcb6` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine9ReleaseGLEv`<br>AbyssEngine::Engine::ReleaseGL() |
| 39 | `0x06dcbc` | `0x06dcb8` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine9PreUpdateEv`<br>AbyssEngine::Engine::PreUpdate() |
| 40 | `0x06dcbe` | `0x06dcba` | -4 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine18ActivateTextureFBOEv`<br>AbyssEngine::Engine::ActivateTextureFBO() |
| 41 | `0x06dccc` | `0x06dcc8` | -4 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine25ActivateRender2TextureFBOEv`<br>AbyssEngine::Engine::ActivateRender2TextureFBO() |
| 42 | `0x06dcda` | `0x06dcd6` | -4 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine27DeactivateRender2TextureFBOEv`<br>AbyssEngine::Engine::DeactivateRender2TextureFBO() |
| 43 | `0x06dce8` | `0x06dce4` | -4 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine18ActivateRefractFBOEv`<br>AbyssEngine::Engine::ActivateRefractFBO() |
| 44 | `0x06dcf6` | `0x06dcf2` | -4 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine22ActivateRender2FracFBOEv`<br>AbyssEngine::Engine::ActivateRender2FracFBO() |
| 45 | `0x06dd04` | `0x06dd00` | -4 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine24DeactivateRender2FracFBOEv`<br>AbyssEngine::Engine::DeactivateRender2FracFBO() |
| 46 | `0x06dd12` | `0x06dd0e` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine19ActivateFrameBufferEi`<br>AbyssEngine::Engine::ActivateFrameBuffer(int) |
| 47 | `0x06dd14` | `0x06dd10` | -4 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine18ActivateViewBufferEv`<br>AbyssEngine::Engine::ActivateViewBuffer() |
| 48 | `0x06dd38` | `0x06dd34` | -4 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine7CopyFBOEv`<br>AbyssEngine::Engine::CopyFBO() |
| 49 | `0x06ddb4` | `0x06ddb0` | -4 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine10SwapBufferEv`<br>AbyssEngine::Engine::SwapBuffer() |
| 50 | `0x06ddce` | `0x06ddca` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine15GrabFrameBufferEv`<br>AbyssEngine::Engine::GrabFrameBuffer() |
| 51 | `0x06ddd0` | `0x06ddcc` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine22SaveImageToPhotosAlbumEv`<br>AbyssEngine::Engine::SaveImageToPhotosAlbum() |
| 52 | `0x06ddd2` | `0x06ddce` | -4 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine16GetJPEGImageDataEf`<br>AbyssEngine::Engine::GetJPEGImageData(float) |
| 53 | `0x06ddd6` | `0x06ddd2` | -4 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine13GetDeviceInfoEv`<br>AbyssEngine::Engine::GetDeviceInfo() |
| 54 | `0x06ddf0` | `0x06ddec` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_Z4logiPc`<br>logi(char*) |
| 55 | `0x06ddf2` | `0x06ddee` | -4 | 2 | 2 | 0 | 100.0% | Y | Y | `_Z4logePc`<br>loge(char*) |
| 56 | `0x06ddf4` | `0x06ddf0` | -4 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC1Ev`<br>FileInterfaceAndroid::FileInterfaceAndroid() |
| 57 | `0x06ddf4` | `0x06ddf0` | -4 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC2Ev`<br>FileInterfaceAndroid::FileInterfaceAndroid() |
| 58 | `0x06de14` | `0x06de10` | -4 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid10OpenAppendEN11AbyssEngine6StringEibj`<br>FileInterfaceAndroid::OpenAppend(AbyssEngine::String, int, bool, unsigned int) |
| 59 | `0x06de18` | `0x06de14` | -4 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC2EP8zip_filebiii`<br>FileInterfaceAndroid::FileInterfaceAndroid(zip_file*, bool, int, int, int) |
| 60 | `0x06de18` | `0x06de14` | -4 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC1EP8zip_filebiii`<br>FileInterfaceAndroid::FileInterfaceAndroid(zip_file*, bool, int, int, int) |
| 61 | `0x06de68` | `0x06de64` | -4 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC2EP7__sFILEb`<br>FileInterfaceAndroid::FileInterfaceAndroid(__sFILE*, bool) |
| 62 | `0x06de68` | `0x06de64` | -4 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC1EP7__sFILEb`<br>FileInterfaceAndroid::FileInterfaceAndroid(__sFILE*, bool) |
| 63 | `0x06de9c` | `0x06de98` | -4 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC1EP8_jobjectb`<br>FileInterfaceAndroid::FileInterfaceAndroid(_jobject*, bool) |
| 64 | `0x06de9c` | `0x06de98` | -4 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidC2EP8_jobjectb`<br>FileInterfaceAndroid::FileInterfaceAndroid(_jobject*, bool) |
| 65 | `0x06df88` | `0x06df84` | -4 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidD1Ev`<br>FileInterfaceAndroid::~FileInterfaceAndroid() |
| 66 | `0x06df88` | `0x06df84` | -4 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidD2Ev`<br>FileInterfaceAndroid::~FileInterfaceAndroid() |
| 67 | `0x06dfce` | `0x06dfc0` | -14 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroidD0Ev`<br>FileInterfaceAndroid::~FileInterfaceAndroid() |
| 68 | `0x06dfe0` | `0x06dfd0` | -16 | 552 | 552 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid8OpenReadEN11AbyssEngine6StringEibiij`<br>FileInterfaceAndroid::OpenRead(AbyssEngine::String, int, bool, int, int, unsigned int) |
| 69 | `0x06e208` | `0x06e1f8` | -16 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid9OpenWriteEN11AbyssEngine6StringEibj`<br>FileInterfaceAndroid::OpenWrite(AbyssEngine::String, int, bool, unsigned int) |
| 70 | `0x06e2e4` | `0x06e2d4` | -16 | 360 | 360 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid4ReadEjPv`<br>FileInterfaceAndroid::Read(unsigned int, void*) |
| 71 | `0x06e44c` | `0x06e43c` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN7_JNIEnv13CallIntMethodEP8_jobjectP10_jmethodIDz`<br>_JNIEnv::CallIntMethod(_jobject*, _jmethodID*, ...) |
| 72 | `0x06e484` | `0x06e474` | -16 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2Ec`<br>AbyssEngine::String::String(char) |
| 73 | `0x06e4ac` | `0x06e49c` | -16 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid5WriteEjPKv`<br>FileInterfaceAndroid::Write(unsigned int, void const*) |
| 74 | `0x06e558` | `0x06e548` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN7_JNIEnv14CallVoidMethodEP8_jobjectP10_jmethodIDz`<br>_JNIEnv::CallVoidMethod(_jobject*, _jmethodID*, ...) |
| 75 | `0x06e590` | `0x06e580` | -16 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid4SeekEj`<br>FileInterfaceAndroid::Seek(unsigned int) |
| 76 | `0x06e5e0` | `0x06e5d0` | -16 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid11GetFileSizeEv`<br>FileInterfaceAndroid::GetFileSize() |
| 77 | `0x06e606` | `0x06e5f6` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid13GetAppRootDirEv`<br>FileInterfaceAndroid::GetAppRootDir() |
| 78 | `0x06e60a` | `0x06e5fa` | -16 | 38 | 38 | 0 | 100.0% |  | Y | `_Z15GetStringLengthN11AbyssEngine6StringE`<br>GetStringLength(AbyssEngine::String) |
| 79 | `0x06e630` | `0x06e620` | -16 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid9FileExistEN11AbyssEngine6StringE`<br>FileInterfaceAndroid::FileExist(AbyssEngine::String) |
| 80 | `0x06e764` | `0x06e754` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid10FileDeleteEN11AbyssEngine6StringE`<br>FileInterfaceAndroid::FileDelete(AbyssEngine::String) |
| 81 | `0x06e794` | `0x06e784` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid18GetDeviceFreeSpaceEv`<br>FileInterfaceAndroid::GetDeviceFreeSpace() |
| 82 | `0x06e798` | `0x06e788` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid12FileEnumInitEPcb`<br>FileInterfaceAndroid::FileEnumInit(char*, bool) |
| 83 | `0x06e79c` | `0x06e78c` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid15FileGetNextEnumERN11AbyssEngine6StringE`<br>FileInterfaceAndroid::FileGetNextEnum(AbyssEngine::String&) |
| 84 | `0x06e7a0` | `0x06e790` | -16 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid5CloseEv`<br>FileInterfaceAndroid::Close() |
| 85 | `0x06e7f4` | `0x06e7e4` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid13SetAppRootDirEPv`<br>FileInterfaceAndroid::SetAppRootDir(void*) |
| 86 | `0x06e7fa` | `0x06e7ea` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid15SetZipDirectoryEPv`<br>FileInterfaceAndroid::SetZipDirectory(void*) |
| 87 | `0x06e800` | `0x06e7f0` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid6OutputEPc`<br>FileInterfaceAndroid::Output(char*) |
| 88 | `0x06e802` | `0x06e7f2` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_Z10OpenAppendPtibj`<br>OpenAppend(unsigned short*, int, bool, unsigned int) |
| 89 | `0x06e806` | `0x06e7f6` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid16SetSaveDirectoryEN11AbyssEngine6StringE`<br>FileInterfaceAndroid::SetSaveDirectory(AbyssEngine::String) |
| 90 | `0x06e808` | `0x06e7f8` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN20FileInterfaceAndroid18ResetSaveDirectoryEv`<br>FileInterfaceAndroid::ResetSaveDirectory() |
| 91 | `0x06e80c` | `0x06e7fc` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN20FileInterfaceAndroid12GetDirPreFixEv`<br>FileInterfaceAndroid::GetDirPreFix() |
| 92 | `0x06e820` | `0x06e810` | -16 | 178 | 178 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrixC1Ev`<br>AbyssEngine::EaseInOutMatrix::EaseInOutMatrix() |
| 93 | `0x06e820` | `0x06e810` | -16 | 178 | 178 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrixC2Ev`<br>AbyssEngine::EaseInOutMatrix::EaseInOutMatrix() |
| 94 | `0x06e8d8` | `0x06e8c8` | -16 | 64 | 64 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6MatrixC2Ev`<br>AbyssEngine::AEMath::Matrix::Matrix() |
| 95 | `0x06e918` | `0x06e908` | -16 | 444 | 444 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix8SetRangeENS_6AEMath6MatrixES2_`<br>AbyssEngine::EaseInOutMatrix::SetRange(AbyssEngine::AEMath::Matrix, AbyssEngine::AEMath::Matrix) |
| 96 | `0x06ead4` | `0x06eac4` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine15EaseInOutMatrix11SetDurationEi`<br>AbyssEngine::EaseInOutMatrix::SetDuration(int) |
| 97 | `0x06eae2` | `0x06ead2` | -16 | 238 | 238 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrixC1ENS_6AEMath6MatrixES2_i`<br>AbyssEngine::EaseInOutMatrix::EaseInOutMatrix(AbyssEngine::AEMath::Matrix, AbyssEngine::AEMath::Matrix, int) |
| 98 | `0x06eae2` | `0x06ead2` | -16 | 238 | 238 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrixC2ENS_6AEMath6MatrixES2_i`<br>AbyssEngine::EaseInOutMatrix::EaseInOutMatrix(AbyssEngine::AEMath::Matrix, AbyssEngine::AEMath::Matrix, int) |
| 99 | `0x06ebd0` | `0x06ebc0` | -16 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN11AbyssEnginemiERKNS_10QuaternionES2_`<br>AbyssEngine::operator-(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&) |
| 100 | `0x06ec1c` | `0x06ec0c` | -16 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix18UpdateCurrentValueEv`<br>AbyssEngine::EaseInOutMatrix::UpdateCurrentValue() |
| 101 | `0x06ed08` | `0x06ecf8` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix8IncreaseEf`<br>AbyssEngine::EaseInOutMatrix::Increase(float) |
| 102 | `0x06ed34` | `0x06ed24` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix8DecreaseEf`<br>AbyssEngine::EaseInOutMatrix::Decrease(float) |
| 103 | `0x06ed60` | `0x06ed50` | -16 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix6RunOutEf`<br>AbyssEngine::EaseInOutMatrix::RunOut(float) |
| 104 | `0x06edcc` | `0x06edbc` | -16 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix8GetValueEv`<br>AbyssEngine::EaseInOutMatrix::GetValue() |
| 105 | `0x06eddc` | `0x06edcc` | -16 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix13SetToMaxValueEv`<br>AbyssEngine::EaseInOutMatrix::SetToMaxValue() |
| 106 | `0x06ede8` | `0x06edd8` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix13SetToMinValueEv`<br>AbyssEngine::EaseInOutMatrix::SetToMinValue() |
| 107 | `0x06edf2` | `0x06ede2` | -16 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix11GetMinValueEv`<br>AbyssEngine::EaseInOutMatrix::GetMinValue() |
| 108 | `0x06edfe` | `0x06edee` | -16 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrix11GetMaxValueEv`<br>AbyssEngine::EaseInOutMatrix::GetMaxValue() |
| 109 | `0x06ee0c` | `0x06edfc` | -16 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKNS_10QuaternionES2_`<br>AbyssEngine::operator+(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&) |
| 110 | `0x06ee56` | `0x06ee46` | -16 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEnginemlERKNS_10QuaternionEf`<br>AbyssEngine::operator*(AbyssEngine::Quaternion const&, float) |
| 111 | `0x06ee94` | `0x06ee84` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ImageReleaseEPPNS_5ImageE`<br>AbyssEngine::ImageRelease(AbyssEngine::Image**) |
| 112 | `0x06eeb8` | `0x06eea8` | -16 | 644 | 644 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine25ImageCreateRegionFromFileEPNS_6EngineEPKctPNS_7Image2DE`<br>AbyssEngine::ImageCreateRegionFromFile(AbyssEngine::Engine*, char const*, unsigned short, AbyssEngine::Image2D*) |
| 113 | `0x06f13c` | `0x06f12c` | -16 | 920 | 920 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine23ImageCreateFontFromFileEPNS_6EngineEPKctPPNS_9ImageFontE`<br>AbyssEngine::ImageCreateFontFromFile(AbyssEngine::Engine*, char const*, unsigned short, AbyssEngine::ImageFont**) |
| 114 | `0x06f4d4` | `0x06f4c4` | -16 | 760 | 760 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19ImageCreateFromFileEPNS_6EngineEPKcPPNS_5ImageE`<br>AbyssEngine::ImageCreateFromFile(AbyssEngine::Engine*, char const*, AbyssEngine::Image**) |
| 115 | `0x06f7cc` | `0x06f7bc` | -16 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureCreateFromFileEPNS_6EngineEPKcPFvPNS_5ImageEPvES6_Pjbf`<br>AbyssEngine::TextureCreateFromFile(AbyssEngine::Engine*, char const*, void (*)(AbyssEngine::Image*, void*), void*, unsigned int*, bool, float) |
| 116 | `0x06f7f4` | `0x06f7e4` | -16 | 2036 | 2036 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine27TextureCreateFromFileInternEPNS_6EngineEPKcPFvPNS_5ImageEPvES6_PjfPNS_15AELoadedTextureEb`<br>AbyssEngine::TextureCreateFromFileIntern(AbyssEngine::Engine*, char const*, void (*)(AbyssEngine::Image*, void*), void*, unsigned int*, float, AbyssEngine::AELoadedTexture*, bool) |
| 117 | `0x06ffe8` | `0x06ffd8` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine15AELoadedTextureEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::AELoadedTexture*>(AbyssEngine::AELoadedTexture*, Array<AbyssEngine::AELoadedTexture*>&) |
| 118 | `0x07000c` | `0x06fffc` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine25GenerateCompressedTextureEPNS_5ImageE`<br>AbyssEngine::GenerateCompressedTexture(AbyssEngine::Image*) |
| 119 | `0x070010` | `0x070000` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN9AEPakFileC1EP13FileInterfaceii`<br>AEPakFile::AEPakFile(FileInterface*, int, int) |
| 120 | `0x070010` | `0x070000` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN9AEPakFileC2EP13FileInterfaceii`<br>AEPakFile::AEPakFile(FileInterface*, int, int) |
| 121 | `0x070034` | `0x070024` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN9AEPakFileD2Ev`<br>AEPakFile::~AEPakFile() |
| 122 | `0x070034` | `0x070024` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN9AEPakFileD1Ev`<br>AEPakFile::~AEPakFile() |
| 123 | `0x070058` | `0x070048` | -16 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN9AEPakFileD0Ev`<br>AEPakFile::~AEPakFile() |
| 124 | `0x070068` | `0x070058` | -16 | 58 | 58 | 0 | 100.0% | Y | Y | `_ZN9AEPakFile4ReadEjPv`<br>AEPakFile::Read(unsigned int, void*) |
| 125 | `0x0700a2` | `0x070092` | -16 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN9AEPakFile4SkipEj`<br>AEPakFile::Skip(unsigned int) |
| 126 | `0x0700d0` | `0x0700c0` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9AEPakFile11GetFileSizeEv`<br>AEPakFile::GetFileSize() |
| 127 | `0x0700d4` | `0x0700c4` | -16 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN9AEPakFile7ReleaseEv`<br>AEPakFile::Release() |
| 128 | `0x0700f8` | `0x0700e8` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN12AENormalFileC1EP13FileInterface`<br>AENormalFile::AENormalFile(FileInterface*) |
| 129 | `0x0700f8` | `0x0700e8` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN12AENormalFileC2EP13FileInterface`<br>AENormalFile::AENormalFile(FileInterface*) |
| 130 | `0x07010c` | `0x0700fc` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN12AENormalFileD2Ev`<br>AENormalFile::~AENormalFile() |
| 131 | `0x07010c` | `0x0700fc` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN12AENormalFileD1Ev`<br>AENormalFile::~AENormalFile() |
| 132 | `0x070130` | `0x070120` | -16 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN12AENormalFileD0Ev`<br>AENormalFile::~AENormalFile() |
| 133 | `0x070140` | `0x070130` | -16 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN12AENormalFile7ReleaseEv`<br>AENormalFile::Release() |
| 134 | `0x070162` | `0x070152` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN12AENormalFile4ReadEjPv`<br>AENormalFile::Read(unsigned int, void*) |
| 135 | `0x070170` | `0x070160` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN12AENormalFile5WriteEjPv`<br>AENormalFile::Write(unsigned int, void*) |
| 136 | `0x07017e` | `0x07016e` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN12AENormalFile4SkipEj`<br>AENormalFile::Skip(unsigned int) |
| 137 | `0x07018c` | `0x07017c` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN12AENormalFile11GetFileSizeEv`<br>AENormalFile::GetFileSize() |
| 138 | `0x07019c` | `0x07018c` | -16 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN6AEFile12SetInterfaceEP13FileInterface`<br>AEFile::SetInterface(FileInterface*) |
| 139 | `0x07021c` | `0x07020c` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14AEPakFileEntryEC2Ev`<br>Array<AEPakFileEntry*>::Array() |
| 140 | `0x070238` | `0x070228` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14AELowLevelFileEC2Ev`<br>Array<AELowLevelFile*>::Array() |
| 141 | `0x070254` | `0x070244` | -16 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN6AEFile7ReleaseEv`<br>AEFile::Release() |
| 142 | `0x0702e4` | `0x0702d4` | -16 | 74 | 74 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP14AELowLevelFileEvR5ArrayIT_E`<br>void ArrayReleaseClasses<AELowLevelFile*>(Array<AELowLevelFile*>&) |
| 143 | `0x07032e` | `0x07031e` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14AELowLevelFileED2Ev`<br>Array<AELowLevelFile*>::~Array() |
| 144 | `0x070344` | `0x070334` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14AEPakFileEntryED2Ev`<br>Array<AEPakFileEntry*>::~Array() |
| 145 | `0x07035c` | `0x07034c` | -16 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN6AEFile20sortPakFileEntryListEv`<br>AEFile::sortPakFileEntryList() |
| 146 | `0x0703bc` | `0x0703ac` | -16 | 372 | 372 | 0 | 100.0% |  | Y | `_ZN6AEFile22collectFilesInPakFilesERN11AbyssEngine6StringE`<br>AEFile::collectFilesInPakFiles(AbyssEngine::String&) |
| 147 | `0x070530` | `0x070520` | -16 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6AEFile8OpenReadERN11AbyssEngine6StringEPj`<br>AEFile::OpenRead(AbyssEngine::String&, unsigned int*) |
| 148 | `0x070538` | `0x070528` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadEjPvj`<br>AEFile::Read(unsigned int, void*, unsigned int) |
| 149 | `0x070570` | `0x070560` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN6AEFile11crc32_ccittERKN11AbyssEngine6StringE`<br>AEFile::crc32_ccitt(AbyssEngine::String const&) |
| 150 | `0x0705b4` | `0x0705a4` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP14AEPakFileEntryEvT_R5ArrayIS2_E`<br>void ArrayAdd<AEPakFileEntry*>(AEPakFileEntry*, Array<AEPakFileEntry*>&) |
| 151 | `0x0705d8` | `0x0705c8` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN6AEFile4SkipEjj`<br>AEFile::Skip(unsigned int, unsigned int) |
| 152 | `0x070610` | `0x070600` | -16 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN6AEFile5CloseEj`<br>AEFile::Close(unsigned int) |
| 153 | `0x070658` | `0x070648` | -16 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN6AEFile15RegisterPakFileEN11AbyssEngine6StringE`<br>AEFile::RegisterPakFile(AbyssEngine::String) |
| 154 | `0x070668` | `0x070658` | -16 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN6AEFile15collectPakFilesERKN11AbyssEngine6StringE`<br>AEFile::collectPakFiles(AbyssEngine::String const&) |
| 155 | `0x07075c` | `0x07074c` | -16 | 360 | 360 | 0 | 100.0% |  | Y | `_ZN6AEFile11findPakFileERKN11AbyssEngine6StringE`<br>AEFile::findPakFile(AbyssEngine::String const&) |
| 156 | `0x0708c4` | `0x0708b4` | -16 | 516 | 516 | 0 | 100.0% |  | Y | `_ZN6AEFile4OpenERN11AbyssEngine6StringENS_12FileOpenTypeEPj`<br>AEFile::Open(AbyssEngine::String&, AEFile::FileOpenType, unsigned int*) |
| 157 | `0x070ac8` | `0x070ab8` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP14AELowLevelFileEvT_R5ArrayIS2_E`<br>void ArrayAdd<AELowLevelFile*>(AELowLevelFile*, Array<AELowLevelFile*>&) |
| 158 | `0x070aec` | `0x070adc` | -16 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6AEFile8OpenReadEPKcPj`<br>AEFile::OpenRead(char const*, unsigned int*) |
| 159 | `0x070b38` | `0x070b28` | -16 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6AEFile9OpenWriteERN11AbyssEngine6StringEPj`<br>AEFile::OpenWrite(AbyssEngine::String&, unsigned int*) |
| 160 | `0x070b40` | `0x070b30` | -16 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6AEFile9OpenWriteEPKcPj`<br>AEFile::OpenWrite(char const*, unsigned int*) |
| 161 | `0x070b8c` | `0x070b7c` | -16 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6AEFile10OpenAppendERN11AbyssEngine6StringEPj`<br>AEFile::OpenAppend(AbyssEngine::String&, unsigned int*) |
| 162 | `0x070b94` | `0x070b84` | -16 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6AEFile10OpenAppendEPKcPj`<br>AEFile::OpenAppend(char const*, unsigned int*) |
| 163 | `0x070be0` | `0x070bd0` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERcj`<br>AEFile::Read(char&, unsigned int) |
| 164 | `0x070bea` | `0x070bda` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERjj`<br>AEFile::Read(unsigned int&, unsigned int) |
| 165 | `0x070bf4` | `0x070be4` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERij`<br>AEFile::Read(int&, unsigned int) |
| 166 | `0x070bfe` | `0x070bee` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERtj`<br>AEFile::Read(unsigned short&, unsigned int) |
| 167 | `0x070c08` | `0x070bf8` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERsj`<br>AEFile::Read(short&, unsigned int) |
| 168 | `0x070c12` | `0x070c02` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERhj`<br>AEFile::Read(unsigned char&, unsigned int) |
| 169 | `0x070c1c` | `0x070c0c` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERaj`<br>AEFile::Read(signed char&, unsigned int) |
| 170 | `0x070c26` | `0x070c16` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERxj`<br>AEFile::Read(long long&, unsigned int) |
| 171 | `0x070c30` | `0x070c20` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERbj`<br>AEFile::Read(bool&, unsigned int) |
| 172 | `0x070c3a` | `0x070c2a` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERfj`<br>AEFile::Read(float&, unsigned int) |
| 173 | `0x070c44` | `0x070c34` | -16 | 168 | 168 | 0 | 100.0% |  | Y | `_ZN6AEFile4ReadERN11AbyssEngine6StringEjb`<br>AEFile::Read(AbyssEngine::String&, unsigned int, bool) |
| 174 | `0x070cec` | `0x070cdc` | -16 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN6AEFile12ReadSwitchedERij`<br>AEFile::ReadSwitched(int&, unsigned int) |
| 175 | `0x070d06` | `0x070cf6` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN6AEFile12ReadSwitchedERtj`<br>AEFile::ReadSwitched(unsigned short&, unsigned int) |
| 176 | `0x070d22` | `0x070d12` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN6AEFile12ReadSwitchedERsj`<br>AEFile::ReadSwitched(short&, unsigned int) |
| 177 | `0x070d40` | `0x070d30` | -16 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN6AEFile12ReadSwitchedERN11AbyssEngine6StringEj`<br>AEFile::ReadSwitched(AbyssEngine::String&, unsigned int) |
| 178 | `0x070db4` | `0x070da4` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEjPvj`<br>AEFile::Write(unsigned int, void*, unsigned int) |
| 179 | `0x070dec` | `0x070ddc` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEcj`<br>AEFile::Write(char, unsigned int) |
| 180 | `0x070e20` | `0x070e10` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEjj`<br>AEFile::Write(unsigned int, unsigned int) |
| 181 | `0x070e50` | `0x070e40` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEij`<br>AEFile::Write(int, unsigned int) |
| 182 | `0x070e80` | `0x070e70` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEtj`<br>AEFile::Write(unsigned short, unsigned int) |
| 183 | `0x070eb4` | `0x070ea4` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEsj`<br>AEFile::Write(short, unsigned int) |
| 184 | `0x070ee8` | `0x070ed8` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEhj`<br>AEFile::Write(unsigned char, unsigned int) |
| 185 | `0x070f1c` | `0x070f0c` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEaj`<br>AEFile::Write(signed char, unsigned int) |
| 186 | `0x070f50` | `0x070f40` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteExj`<br>AEFile::Write(long long, unsigned int) |
| 187 | `0x070f80` | `0x070f70` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEbj`<br>AEFile::Write(bool, unsigned int) |
| 188 | `0x070fb4` | `0x070fa4` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteEfj`<br>AEFile::Write(float, unsigned int) |
| 189 | `0x070fe4` | `0x070fd4` | -16 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN6AEFile5WriteERKN11AbyssEngine6StringEjb`<br>AEFile::Write(AbyssEngine::String const&, unsigned int, bool) |
| 190 | `0x071074` | `0x071064` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6AEFile11GetFileSizeEj`<br>AEFile::GetFileSize(unsigned int) |
| 191 | `0x0710a8` | `0x071098` | -16 | 272 | 272 | 0 | 100.0% |  | Y | `_ZN6AEFile9FileExistERKN11AbyssEngine6StringE`<br>AEFile::FileExist(AbyssEngine::String const&) |
| 192 | `0x0711b8` | `0x0711a8` | -16 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN6AEFile10FileDeleteERKN11AbyssEngine6StringE`<br>AEFile::FileDelete(AbyssEngine::String const&) |
| 193 | `0x071228` | `0x071218` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN6AEFile18GetDeviceFreeSpaceEv`<br>AEFile::GetDeviceFreeSpace() |
| 194 | `0x071240` | `0x071230` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN6AEFile13GetAppRootDirEv`<br>AEFile::GetAppRootDir() |
| 195 | `0x071258` | `0x071248` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN6AEFile13SetAppRootDirEPv`<br>AEFile::SetAppRootDir(void*) |
| 196 | `0x071270` | `0x071260` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN6AEFile15SetZipDirectoryEPv`<br>AEFile::SetZipDirectory(void*) |
| 197 | `0x071288` | `0x071278` | -16 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN6AEFile16SetSaveDirectoryEN11AbyssEngine6StringE`<br>AEFile::SetSaveDirectory(AbyssEngine::String) |
| 198 | `0x0712f0` | `0x0712e0` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN6AEFile18ResetSaveDirectoryEv`<br>AEFile::ResetSaveDirectory() |
| 199 | `0x071308` | `0x0712f8` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9AEPakFile5WriteEjPv`<br>AEPakFile::Write(unsigned int, void*) |
| 200 | `0x07130c` | `0x0712fc` | -16 | 180 | 180 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20CameraSetPerspectiveEfffffPNS_6CameraE`<br>AbyssEngine::CameraSetPerspective(float, float, float, float, float, AbyssEngine::Camera*) |
| 201 | `0x0713c0` | `0x0713b0` | -16 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20CameraSetPerspectiveEffffPNS_6CameraE`<br>AbyssEngine::CameraSetPerspective(float, float, float, float, AbyssEngine::Camera*) |
| 202 | `0x07144c` | `0x07143c` | -16 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine26CameraIsPointinViewFrustumERKNS_6AEMath6VectorEPNS0_6MatrixEPNS_6CameraE`<br>AbyssEngine::CameraIsPointinViewFrustum(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Matrix*, AbyssEngine::Camera*) |
| 203 | `0x0715cc` | `0x0715bc` | -16 | 436 | 436 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine27CameraIsSphereinViewFrustumERKNS_6AEMath6VectorEfPNS0_6MatrixEPNS_6CameraE`<br>AbyssEngine::CameraIsSphereinViewFrustum(AbyssEngine::AEMath::Vector const&, float, AbyssEngine::AEMath::Matrix*, AbyssEngine::Camera*) |
| 204 | `0x071780` | `0x071770` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOutC2Ev`<br>AbyssEngine::EaseInOut::EaseInOut() |
| 205 | `0x071780` | `0x071770` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOutC1Ev`<br>AbyssEngine::EaseInOut::EaseInOut() |
| 206 | `0x0717a8` | `0x071798` | -16 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOut18UpdateCurrentValueEv`<br>AbyssEngine::EaseInOut::UpdateCurrentValue() |
| 207 | `0x071810` | `0x071800` | -16 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOutC2Eff`<br>AbyssEngine::EaseInOut::EaseInOut(float, float) |
| 208 | `0x071810` | `0x071800` | -16 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOutC1Eff`<br>AbyssEngine::EaseInOut::EaseInOut(float, float) |
| 209 | `0x071820` | `0x071810` | -16 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOut8SetRangeEff`<br>AbyssEngine::EaseInOut::SetRange(float, float) |
| 210 | `0x071840` | `0x071830` | -16 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOut8IncreaseEf`<br>AbyssEngine::EaseInOut::Increase(float) |
| 211 | `0x0718a8` | `0x071898` | -16 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOut8DecreaseEf`<br>AbyssEngine::EaseInOut::Decrease(float) |
| 212 | `0x071910` | `0x071900` | -16 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOut6RunOutEf`<br>AbyssEngine::EaseInOut::RunOut(float) |
| 213 | `0x0719ac` | `0x07199c` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9EaseInOut8GetValueEv`<br>AbyssEngine::EaseInOut::GetValue() |
| 214 | `0x0719b0` | `0x0719a0` | -16 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOut13SetToMaxValueEv`<br>AbyssEngine::EaseInOut::SetToMaxValue() |
| 215 | `0x0719bc` | `0x0719ac` | -16 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9EaseInOut13SetToMinValueEv`<br>AbyssEngine::EaseInOut::SetToMinValue() |
| 216 | `0x0719c8` | `0x0719b8` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9EaseInOut11GetMinValueEv`<br>AbyssEngine::EaseInOut::GetMinValue() |
| 217 | `0x0719cc` | `0x0719bc` | -16 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9EaseInOut11GetMaxValueEv`<br>AbyssEngine::EaseInOut::GetMaxValue() |
| 218 | `0x0719e0` | `0x0719d0` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8AERandomC1Ev`<br>AbyssEngine::AERandom::AERandom() |
| 219 | `0x0719e0` | `0x0719d0` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8AERandomC2Ev`<br>AbyssEngine::AERandom::AERandom() |
| 220 | `0x071a04` | `0x0719f4` | -16 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine8AERandom7setSeedEx`<br>AbyssEngine::AERandom::setSeed(long long) |
| 221 | `0x071a18` | `0x071a08` | -16 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine8AERandomC2Ex`<br>AbyssEngine::AERandom::AERandom(long long) |
| 222 | `0x071a18` | `0x071a08` | -16 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine8AERandomC1Ex`<br>AbyssEngine::AERandom::AERandom(long long) |
| 223 | `0x071a2c` | `0x071a1c` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine8AERandomD2Ev`<br>AbyssEngine::AERandom::~AERandom() |
| 224 | `0x071a2c` | `0x071a1c` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine8AERandomD1Ev`<br>AbyssEngine::AERandom::~AERandom() |
| 225 | `0x071a30` | `0x071a20` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8AERandom5resetEv`<br>AbyssEngine::AERandom::reset() |
| 226 | `0x071a54` | `0x071a44` | -16 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine8AERandom4nextEi`<br>AbyssEngine::AERandom::next(int) |
| 227 | `0x071aa4` | `0x071a94` | -16 | 44 | 44 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine8AERandom7nextIntEv`<br>AbyssEngine::AERandom::nextInt() |
| 228 | `0x071ad0` | `0x071ac0` | -16 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8AERandom7nextIntEi`<br>AbyssEngine::AERandom::nextInt(int) |
| 229 | `0x071b64` | `0x071b54` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14Image2DReleaseEPNS_6EngineEPPNS_7Image2DE`<br>AbyssEngine::Image2DRelease(AbyssEngine::Engine*, AbyssEngine::Image2D**) |
| 230 | `0x071b80` | `0x071b70` | -16 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19ImageFontDrawStringEPNS_9ImageFontEPKtiiPNS_11PaintCanvasEPNS_6EngineEb`<br>AbyssEngine::ImageFontDrawString(AbyssEngine::ImageFont*, unsigned short const*, int, int, AbyssEngine::PaintCanvas*, AbyssEngine::Engine*, bool) |
| 231 | `0x071bc0` | `0x071bb0` | -16 | 1392 | 1392 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19ImageFontDrawStringEPNS_9ImageFontEPKtjiiPNS_11PaintCanvasEPNS_6EngineEb`<br>AbyssEngine::ImageFontDrawString(AbyssEngine::ImageFont*, unsigned short const*, unsigned int, int, int, AbyssEngine::PaintCanvas*, AbyssEngine::Engine*, bool) |
| 232 | `0x072130` | `0x072120` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine20ImageFontCheckStringEPNS_9ImageFontEPKtj`<br>AbyssEngine::ImageFontCheckString(AbyssEngine::ImageFont*, unsigned short const*, unsigned int) |
| 233 | `0x07213e` | `0x07212e` | -16 | 124 | 124 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine17ImageFontGetWidthEPNS_9ImageFontEPKtj`<br>AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int) |
| 234 | `0x0721ba` | `0x0721aa` | -16 | 32 | 32 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ImageFontGetHeightEPNS_9ImageFontE`<br>AbyssEngine::ImageFontGetHeight(AbyssEngine::ImageFont*) |
| 235 | `0x0721da` | `0x0721ca` | -16 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine19ImageFontGetYOffsetEPNS_9ImageFontE`<br>AbyssEngine::ImageFontGetYOffset(AbyssEngine::ImageFont*) |
| 236 | `0x0721e6` | `0x0721d6` | -16 | 128 | 128 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine17ImageFontGetWidthEPNS_9ImageFontEPKtjj`<br>AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int, unsigned int) |
| 237 | `0x072266` | `0x072256` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine19ImageFontSetSpacingEPNS_9ImageFontEs`<br>AbyssEngine::ImageFontSetSpacing(AbyssEngine::ImageFont*, short) |
| 238 | `0x07226c` | `0x07225c` | -16 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine19ImageFontGetSpacingEPNS_9ImageFontE`<br>AbyssEngine::ImageFontGetSpacing(AbyssEngine::ImageFont*) |
| 239 | `0x072278` | `0x072268` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine19ImageFontSetYOffsetEPNS_9ImageFontEs`<br>AbyssEngine::ImageFontSetYOffset(AbyssEngine::ImageFont*, short) |
| 240 | `0x07227e` | `0x07226e` | -16 | 106 | 106 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ImageFontReleaseEPNS_6EngineEPPNS_9ImageFontE`<br>AbyssEngine::ImageFontRelease(AbyssEngine::Engine*, AbyssEngine::ImageFont**) |
| 241 | `0x0722e8` | `0x0722d8` | -16 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String7ReverseEv`<br>AbyssEngine::String::Reverse() |
| 242 | `0x072378` | `0x072368` | -16 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringixEi`<br>AbyssEngine::String::operator[](int) |
| 243 | `0x072398` | `0x072388` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC1Ev`<br>AbyssEngine::String::String() |
| 244 | `0x072398` | `0x072388` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2Ev`<br>AbyssEngine::String::String() |
| 245 | `0x0723b0` | `0x0723a0` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC1EPKtb`<br>AbyssEngine::String::String(unsigned short const*, bool) |
| 246 | `0x0723b0` | `0x0723a0` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2EPKtb`<br>AbyssEngine::String::String(unsigned short const*, bool) |
| 247 | `0x0723dc` | `0x0723cc` | -16 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String3SetEPKt`<br>AbyssEngine::String::Set(unsigned short const*) |
| 248 | `0x072488` | `0x072478` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2EPKcb`<br>AbyssEngine::String::String(char const*, bool) |
| 249 | `0x072488` | `0x072478` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC1EPKcb`<br>AbyssEngine::String::String(char const*, bool) |
| 250 | `0x0724b4` | `0x0724a4` | -16 | 176 | 176 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String3SetEPKc`<br>AbyssEngine::String::Set(char const*) |
| 251 | `0x072564` | `0x072554` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2ERKS0_b`<br>AbyssEngine::String::String(AbyssEngine::String const&, bool) |
| 252 | `0x072564` | `0x072554` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC1ERKS0_b`<br>AbyssEngine::String::String(AbyssEngine::String const&, bool) |
| 253 | `0x072594` | `0x072584` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringD2Ev`<br>AbyssEngine::String::~String() |
| 254 | `0x072594` | `0x072584` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringD1Ev`<br>AbyssEngine::String::~String() |
| 255 | `0x0725b8` | `0x0725a8` | -16 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringD0Ev`<br>AbyssEngine::String::~String() |
| 256 | `0x0725c8` | `0x0725b8` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6StringcvPtEv`<br>AbyssEngine::String::operator unsigned short*() |
| 257 | `0x0725cc` | `0x0725bc` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine6StringcvPKtEv`<br>AbyssEngine::String::operator unsigned short const*() const |
| 258 | `0x0725d0` | `0x0725c0` | -16 | 316 | 316 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String16getWCharFromUtf8EPci`<br>AbyssEngine::String::getWCharFromUtf8(char*, int) |
| 259 | `0x07270c` | `0x0726fc` | -16 | 70 | 70 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String15ConvertFromUTF8Ev`<br>AbyssEngine::String::ConvertFromUTF8() |
| 260 | `0x072752` | `0x072742` | -16 | 50 | 50 | 0 | 100.0% |  | Y | `_ZNK11AbyssEngine6String9GetAECharEv`<br>AbyssEngine::String::GetAEChar() const |
| 261 | `0x072784` | `0x072774` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine6String10GetAEWCharEv`<br>AbyssEngine::String::GetAEWChar() const |
| 262 | `0x072788` | `0x072778` | -16 | 32 | 32 | 0 | 100.0% |  | Y | `_ZNK11AbyssEngine6StringixEi`<br>AbyssEngine::String::operator[](int) const |
| 263 | `0x0727a8` | `0x072798` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringaSERKS0_`<br>AbyssEngine::String::operator=(AbyssEngine::String const&) |
| 264 | `0x0727bc` | `0x0727ac` | -16 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String7ValueOfEv`<br>AbyssEngine::String::ValueOf() |
| 265 | `0x0727d8` | `0x0727c8` | -16 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringpLERKS0_`<br>AbyssEngine::String::operator+=(AbyssEngine::String const&) |
| 266 | `0x072850` | `0x072840` | -16 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringpLERKc`<br>AbyssEngine::String::operator+=(char const&) |
| 267 | `0x07289c` | `0x07288c` | -16 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringpLERKx`<br>AbyssEngine::String::operator+=(long long const&) |
| 268 | `0x072904` | `0x0728f4` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2Ex`<br>AbyssEngine::String::String(long long) |
| 269 | `0x072928` | `0x072918` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringpLERKi`<br>AbyssEngine::String::operator+=(int const&) |
| 270 | `0x072958` | `0x072948` | -16 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringpLERKf`<br>AbyssEngine::String::operator+=(float const&) |
| 271 | `0x0729bc` | `0x0729ac` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2Ef`<br>AbyssEngine::String::String(float) |
| 272 | `0x0729e0` | `0x0729d0` | -16 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String6StrLenEPKc`<br>AbyssEngine::String::StrLen(char const*) |
| 273 | `0x0729f0` | `0x0729e0` | -16 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String6StrLenEPKt`<br>AbyssEngine::String::StrLen(unsigned short const*) |
| 274 | `0x072a0c` | `0x0729fc` | -16 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String3SetEx`<br>AbyssEngine::String::Set(long long) |
| 275 | `0x072adc` | `0x072acc` | -16 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine4MODFEfPf`<br>AbyssEngine::MODF(float, float*) |
| 276 | `0x072af8` | `0x072ae8` | -16 | 440 | 440 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18computeFloatStringEfiPiS0_i`<br>AbyssEngine::computeFloatString(float, int, int*, int*, int) |
| 277 | `0x072cb0` | `0x072ca0` | -16 | 460 | 460 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String3SetEf`<br>AbyssEngine::String::Set(float) |
| 278 | `0x072e7c` | `0x072e6c` | -16 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKNS_6StringES2_`<br>AbyssEngine::operator+(AbyssEngine::String const&, AbyssEngine::String const&) |
| 279 | `0x072ed8` | `0x072ec8` | -16 | 168 | 168 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String9SubStringEjj`<br>AbyssEngine::String::SubString(unsigned int, unsigned int) |
| 280 | `0x072f80` | `0x072f70` | -16 | 64 | 64 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String7CompareERKS0_`<br>AbyssEngine::String::Compare(AbyssEngine::String const&) |
| 281 | `0x072fc0` | `0x072fb0` | -16 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String7CompareEPKc`<br>AbyssEngine::String::Compare(char const*) |
| 282 | `0x073004` | `0x072ff4` | -16 | 100 | 100 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String7IndexOfEjRKS0_`<br>AbyssEngine::String::IndexOf(unsigned int, AbyssEngine::String const&) |
| 283 | `0x073068` | `0x073058` | -16 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String7IndexOfERKS0_`<br>AbyssEngine::String::IndexOf(AbyssEngine::String const&) |
| 284 | `0x073070` | `0x073060` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String11ReplaceCharEcc`<br>AbyssEngine::String::ReplaceChar(char, char) |
| 285 | `0x0730a4` | `0x073094` | -16 | 130 | 130 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String11ToUpperCaseEv`<br>AbyssEngine::String::ToUpperCase() |
| 286 | `0x073128` | `0x073118` | -16 | 126 | 126 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String11ToLowerCaseEv`<br>AbyssEngine::String::ToLowerCase() |
| 287 | `0x0731a8` | `0x073198` | -16 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String4TrimEv`<br>AbyssEngine::String::Trim() |
| 288 | `0x07322c` | `0x07321c` | -16 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String5SplitES0_`<br>AbyssEngine::String::Split(AbyssEngine::String) |
| 289 | `0x0732f0` | `0x0732e0` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine6StringEEC2Ev`<br>Array<AbyssEngine::String*>::Array() |
| 290 | `0x07330c` | `0x0732fc` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine6StringEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::String*>(AbyssEngine::String*, Array<AbyssEngine::String*>&) |
| 291 | `0x073330` | `0x073320` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine6StringEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::String*>(Array<AbyssEngine::String*>&) |
| 292 | `0x073352` | `0x073342` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine6StringEED2Ev`<br>Array<AbyssEngine::String*>::~Array() |
| 293 | `0x073368` | `0x073358` | -16 | 456 | 456 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String9SplitTagsES0_`<br>AbyssEngine::String::SplitTags(AbyssEngine::String) |
| 294 | `0x073530` | `0x073520` | -16 | 260 | 260 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String13ReplaceStringES0_S0_`<br>AbyssEngine::String::ReplaceString(AbyssEngine::String, AbyssEngine::String) |
| 295 | `0x073634` | `0x073624` | -16 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6String15GetStringLengthEPKc`<br>AbyssEngine::String::GetStringLength(char const*) |
| 296 | `0x073644` | `0x073634` | -16 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6String8PrintOutEv`<br>AbyssEngine::String::PrintOut() |
| 297 | `0x073654` | `0x073644` | -16 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEnginepsERKNS_6StringE`<br>AbyssEngine::operator+(AbyssEngine::String const&) |
| 298 | `0x073660` | `0x073650` | -16 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKNS_6StringERKx`<br>AbyssEngine::operator+(AbyssEngine::String const&, long long const&) |
| 299 | `0x0736dc` | `0x0736cc` | -16 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKxRKNS_6StringE`<br>AbyssEngine::operator+(long long const&, AbyssEngine::String const&) |
| 300 | `0x07373c` | `0x07372c` | -16 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKNS_6StringERKi`<br>AbyssEngine::operator+(AbyssEngine::String const&, int const&) |
| 301 | `0x0737b4` | `0x0737a4` | -16 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6StringC2Ei`<br>AbyssEngine::String::String(int) |
| 302 | `0x0737dc` | `0x0737cc` | -16 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKiRKNS_6StringE`<br>AbyssEngine::operator+(int const&, AbyssEngine::String const&) |
| 303 | `0x073838` | `0x073828` | -16 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKNS_6StringERKf`<br>AbyssEngine::operator+(AbyssEngine::String const&, float const&) |
| 304 | `0x0738b0` | `0x0738a0` | -16 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineplERKfRKNS_6StringE`<br>AbyssEngine::operator+(float const&, AbyssEngine::String const&) |
| 305 | `0x07390c` | `0x0738fc` | -16 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11AbyssEngineeqERKNS_6StringES2_`<br>AbyssEngine::operator==(AbyssEngine::String const&, AbyssEngine::String const&) |
| 306 | `0x07395c` | `0x07394c` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectorcvPfEv`<br>AbyssEngine::AEMath::Vector::operator float*() |
| 307 | `0x07395e` | `0x07394e` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine6AEMath6VectorcvPKfEv`<br>AbyssEngine::AEMath::Vector::operator float const*() const |
| 308 | `0x073960` | `0x073950` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectorixEi`<br>AbyssEngine::AEMath::Vector::operator[](int) |
| 309 | `0x073966` | `0x073956` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine6AEMath6VectorixEi`<br>AbyssEngine::AEMath::Vector::operator[](int) const |
| 310 | `0x07396c` | `0x07395c` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectoraSERKS1_`<br>AbyssEngine::AEMath::Vector::operator=(AbyssEngine::AEMath::Vector const&) |
| 311 | `0x07397a` | `0x07396a` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectorpLERKS1_`<br>AbyssEngine::AEMath::Vector::operator+=(AbyssEngine::AEMath::Vector const&) |
| 312 | `0x0739ac` | `0x07399c` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectormIERKS1_`<br>AbyssEngine::AEMath::Vector::operator-=(AbyssEngine::AEMath::Vector const&) |
| 313 | `0x0739de` | `0x0739ce` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectormLEf`<br>AbyssEngine::AEMath::Vector::operator*=(float) |
| 314 | `0x073a08` | `0x0739f8` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectormLERKS1_`<br>AbyssEngine::AEMath::Vector::operator*=(AbyssEngine::AEMath::Vector const&) |
| 315 | `0x073a3a` | `0x073a2a` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectordVEf`<br>AbyssEngine::AEMath::Vector::operator/=(float) |
| 316 | `0x073a64` | `0x073a54` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6VectordVERKS1_`<br>AbyssEngine::AEMath::Vector::operator/=(AbyssEngine::AEMath::Vector const&) |
| 317 | `0x073a96` | `0x073a86` | -16 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathpsERKNS0_6VectorE`<br>AbyssEngine::AEMath::operator+(AbyssEngine::AEMath::Vector const&) |
| 318 | `0x073aa4` | `0x073a94` | -16 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathngERKNS0_6VectorE`<br>AbyssEngine::AEMath::operator-(AbyssEngine::AEMath::Vector const&) |
| 319 | `0x073aca` | `0x073aba` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathplERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator+(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 320 | `0x073afc` | `0x073aec` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathmiERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator-(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 321 | `0x073b2e` | `0x073b1e` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathmlERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator*(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 322 | `0x073b60` | `0x073b50` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathmlERKNS0_6VectorEf`<br>AbyssEngine::AEMath::operator*(AbyssEngine::AEMath::Vector const&, float) |
| 323 | `0x073b8a` | `0x073b7a` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathmlEfRKNS0_6VectorE`<br>AbyssEngine::AEMath::operator*(float, AbyssEngine::AEMath::Vector const&) |
| 324 | `0x073bb4` | `0x073ba4` | -16 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathdvERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator/(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 325 | `0x073be6` | `0x073bd6` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathdvERKNS0_6VectorEf`<br>AbyssEngine::AEMath::operator/(AbyssEngine::AEMath::Vector const&, float) |
| 326 | `0x073c10` | `0x073c00` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathdvEfRKNS0_6VectorE`<br>AbyssEngine::AEMath::operator/(float, AbyssEngine::AEMath::Vector const&) |
| 327 | `0x073c3a` | `0x073c2a` | -16 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMatheqERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator==(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 328 | `0x073c7e` | `0x073c6e` | -16 | 64 | 64 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathneERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator!=(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 329 | `0x073cbe` | `0x073cae` | -16 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathltERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator<(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 330 | `0x073d02` | `0x073cf2` | -16 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathgtERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator>(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 331 | `0x073d46` | `0x073d36` | -16 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathleERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator<=(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 332 | `0x073d8a` | `0x073d7a` | -16 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathgeERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::operator>=(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 333 | `0x073dce` | `0x073dbe` | -16 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath13VectorIsEqualERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::VectorIsEqual(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 334 | `0x073dd2` | `0x073dc2` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath9VectorDotERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::VectorDot(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 335 | `0x073dfc` | `0x073dec` | -16 | 62 | 62 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath11VectorCrossERKNS0_6VectorES3_`<br>AbyssEngine::AEMath::VectorCross(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 336 | `0x073e3c` | `0x073e2c` | -16 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath15VectorNormalizeERKNS0_6VectorE`<br>AbyssEngine::AEMath::VectorNormalize(AbyssEngine::AEMath::Vector const&) |
| 337 | `0x073eb8` | `0x073ea8` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath12VectorLengthERKNS0_6VectorE`<br>AbyssEngine::AEMath::VectorLength(AbyssEngine::AEMath::Vector const&) |
| 338 | `0x073eec` | `0x073edc` | -16 | 66 | 66 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath10VectorLerpERKNS0_6VectorES3_f`<br>AbyssEngine::AEMath::VectorLerp(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, float) |
| 339 | `0x073f30` | `0x073f20` | -16 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath7BSphere5MergeERKNS_9TransformE`<br>AbyssEngine::AEMath::BSphere::Merge(AbyssEngine::Transform const&) |
| 340 | `0x074000` | `0x073ff0` | -16 | 324 | 324 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath7BSphere5MergeERKS1_`<br>AbyssEngine::AEMath::BSphere::Merge(AbyssEngine::AEMath::BSphere const&) |
| 341 | `0x074144` | `0x074134` | -16 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath7BSphereaSERKS1_`<br>AbyssEngine::AEMath::BSphere::operator=(AbyssEngine::AEMath::BSphere const&) |
| 342 | `0x074154` | `0x074144` | -16 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18IApplicationModule21SetApplicationManagerEPNS_18ApplicationManagerE`<br>AbyssEngine::IApplicationModule::SetApplicationManager(AbyssEngine::ApplicationManager*) |
| 343 | `0x07415c` | `0x07414c` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11CurveCreateEPPvtPPNS_5CurveE`<br>AbyssEngine::CurveCreate(void**, unsigned short, AbyssEngine::Curve**) |
| 344 | `0x074194` | `0x074184` | -16 | 398 | 398 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13CurveGetValueEyPNS_5CurveE`<br>AbyssEngine::CurveGetValue(unsigned long long, AbyssEngine::Curve*) |
| 345 | `0x074322` | `0x074312` | -16 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12CurveReleaseEPPNS_5CurveE`<br>AbyssEngine::CurveRelease(AbyssEngine::Curve**) |
| 346 | `0x074378` | `0x074368` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9CheatCodeC2Ev`<br>AbyssEngine::CheatCode::CheatCode() |
| 347 | `0x074378` | `0x074368` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9CheatCodeC1Ev`<br>AbyssEngine::CheatCode::CheatCode() |
| 348 | `0x0743a4` | `0x074394` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayItEC2Ev`<br>Array<unsigned short>::Array() |
| 349 | `0x0743c0` | `0x0743b0` | -16 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9CheatCodeD1Ev`<br>AbyssEngine::CheatCode::~CheatCode() |
| 350 | `0x0743c0` | `0x0743b0` | -16 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9CheatCodeD2Ev`<br>AbyssEngine::CheatCode::~CheatCode() |
| 351 | `0x0743da` | `0x0743ca` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayItED2Ev`<br>Array<unsigned short>::~Array() |
| 352 | `0x0743f0` | `0x0743e0` | -16 | 88 | 88 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9CheatCode6UpdateEt`<br>AbyssEngine::CheatCode::Update(unsigned short) |
| 353 | `0x074448` | `0x074438` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12CheatHandlerC1EPNS_7KeyCodeE`<br>AbyssEngine::CheatHandler::CheatHandler(AbyssEngine::KeyCode*) |
| 354 | `0x074448` | `0x074438` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12CheatHandlerC2EPNS_7KeyCodeE`<br>AbyssEngine::CheatHandler::CheatHandler(AbyssEngine::KeyCode*) |
| 355 | `0x074480` | `0x074470` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine9CheatCodeEEC2Ev`<br>Array<AbyssEngine::CheatCode*>::Array() |
| 356 | `0x07449c` | `0x07448c` | -16 | 98 | 98 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12CheatHandlerD2Ev`<br>AbyssEngine::CheatHandler::~CheatHandler() |
| 357 | `0x07449c` | `0x07448c` | -16 | 98 | 98 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12CheatHandlerD1Ev`<br>AbyssEngine::CheatHandler::~CheatHandler() |
| 358 | `0x0744fe` | `0x0744ee` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine9CheatCodeEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::CheatCode*>(Array<AbyssEngine::CheatCode*>&) |
| 359 | `0x074520` | `0x074510` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine9CheatCodeEED2Ev`<br>Array<AbyssEngine::CheatCode*>::~Array() |
| 360 | `0x074538` | `0x074528` | -16 | 214 | 214 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12CheatHandler12AddCheatCodeERKNS_6StringEi`<br>AbyssEngine::CheatHandler::AddCheatCode(AbyssEngine::String const&, int) |
| 361 | `0x07460e` | `0x0745fe` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine9CheatCodeEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::CheatCode*>(AbyssEngine::CheatCode*, Array<AbyssEngine::CheatCode*>&) |
| 362 | `0x074632` | `0x074622` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddItEvT_R5ArrayIS0_E`<br>void ArrayAdd<unsigned short>(unsigned short, Array<unsigned short>&) |
| 363 | `0x074656` | `0x074646` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine12CheatHandler12SetCheatFuncEPFviPvES1_`<br>AbyssEngine::CheatHandler::SetCheatFunc(void (*)(int, void*), void*) |
| 364 | `0x07465c` | `0x07464c` | -16 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12CheatHandler6UpdateEt`<br>AbyssEngine::CheatHandler::Update(unsigned short) |
| 365 | `0x0746a0` | `0x074690` | -16 | 480 | 480 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9TransformC1Ev`<br>AbyssEngine::Transform::Transform() |
| 366 | `0x0746a0` | `0x074690` | -16 | 480 | 480 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9TransformC2Ev`<br>AbyssEngine::Transform::Transform() |
| 367 | `0x074880` | `0x074870` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine4MeshEEC2Ev`<br>Array<AbyssEngine::Mesh*>::Array() |
| 368 | `0x07489c` | `0x07488c` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine9TransformEEC2Ev`<br>Array<AbyssEngine::Transform*>::Array() |
| 369 | `0x0748b8` | `0x0748a8` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine8KeyFrameEEC2Ev`<br>Array<AbyssEngine::KeyFrame*>::Array() |
| 370 | `0x0748d4` | `0x0748c4` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine8KeyFrameEED2Ev`<br>Array<AbyssEngine::KeyFrame*>::~Array() |
| 371 | `0x0748ea` | `0x0748da` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine9TransformEED2Ev`<br>Array<AbyssEngine::Transform*>::~Array() |
| 372 | `0x074900` | `0x0748f0` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine4MeshEED2Ev`<br>Array<AbyssEngine::Mesh*>::~Array() |
| 373 | `0x074918` | `0x074908` | -16 | 596 | 596 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9TransformC2EPS0_`<br>AbyssEngine::Transform::Transform(AbyssEngine::Transform*) |
| 374 | `0x074918` | `0x074908` | -16 | 596 | 596 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9TransformC1EPS0_`<br>AbyssEngine::Transform::Transform(AbyssEngine::Transform*) |
| 375 | `0x074b6c` | `0x074b5c` | -16 | 10 | 10 | 0 | 100.0% |  | Y | `_Z8ArraySetIPN11AbyssEngine8KeyFrameEEvRK5ArrayIT_ERS5_`<br>void ArraySet<AbyssEngine::KeyFrame*>(Array<AbyssEngine::KeyFrame*> const&, Array<AbyssEngine::KeyFrame*>&) |
| 376 | `0x074b76` | `0x074b66` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine9TransformEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::Transform*>(AbyssEngine::Transform*, Array<AbyssEngine::Transform*>&) |
| 377 | `0x074b9a` | `0x074b8a` | -16 | 162 | 162 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform20CollectAnimationDataEv`<br>AbyssEngine::Transform::CollectAnimationData() |
| 378 | `0x074c3c` | `0x074c2c` | -16 | 32 | 32 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9Transform17SetAnimationStateENS_13AnimationModeEPv`<br>AbyssEngine::Transform::SetAnimationState(AbyssEngine::AnimationMode, void*) |
| 379 | `0x074c5c` | `0x074c4c` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9Transform17SetAnimationSpeedEf`<br>AbyssEngine::Transform::SetAnimationSpeed(float) |
| 380 | `0x074c62` | `0x074c52` | -16 | 102 | 102 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform24InitAnimationRangeInTimeEv`<br>AbyssEngine::Transform::InitAnimationRangeInTime() |
| 381 | `0x074cc8` | `0x074cb8` | -16 | 456 | 456 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform6UpdateExb`<br>AbyssEngine::Transform::Update(long long, bool) |
| 382 | `0x074e90` | `0x074e80` | -16 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform23SetAnimationRangeInTimeExx`<br>AbyssEngine::Transform::SetAnimationRangeInTime(long long, long long) |
| 383 | `0x074fb4` | `0x074fa4` | -16 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform28SetAnimationRangeInKeyFramesEii`<br>AbyssEngine::Transform::SetAnimationRangeInKeyFrames(int, int) |
| 384 | `0x075004` | `0x074ff4` | -16 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9Transform18SetAnimationLengthEx`<br>AbyssEngine::Transform::SetAnimationLength(long long) |
| 385 | `0x07500e` | `0x074ffe` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform26PauseAnimationWithKeyFrameEi`<br>AbyssEngine::Transform::PauseAnimationWithKeyFrame(int) |
| 386 | `0x075048` | `0x075038` | -16 | 2112 | 2112 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform12InternUpdateExb`<br>AbyssEngine::Transform::InternUpdate(long long, bool) |
| 387 | `0x075888` | `0x075878` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform27PauseAnimationWithTimeStampEx`<br>AbyssEngine::Transform::PauseAnimationWithTimeStamp(long long) |
| 388 | `0x0758a0` | `0x075890` | -16 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform11AddKeyFrameERKNS_6AEMath6VectorES4_S4_S4_S4_S4_i`<br>AbyssEngine::Transform::AddKeyFrame(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, int) |
| 389 | `0x075934` | `0x075924` | -16 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8KeyFrameC2Ev`<br>AbyssEngine::KeyFrame::KeyFrame() |
| 390 | `0x0759a0` | `0x075990` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine8KeyFrameEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::KeyFrame*>(AbyssEngine::KeyFrame*, Array<AbyssEngine::KeyFrame*>&) |
| 391 | `0x0759c4` | `0x0759b4` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform14InsertKeyFrameEPNS_8KeyFrameEi`<br>AbyssEngine::Transform::InsertKeyFrame(AbyssEngine::KeyFrame*, int) |
| 392 | `0x075a08` | `0x0759f8` | -16 | 1288 | 1288 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform15UpdateKeyFramesEPNS_8KeyFrameEi`<br>AbyssEngine::Transform::UpdateKeyFrames(AbyssEngine::KeyFrame*, int) |
| 393 | `0x075f10` | `0x075f00` | -16 | 1276 | 1276 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform14InsertKeyFrameEPKfxi`<br>AbyssEngine::Transform::InsertKeyFrame(float const*, long long, int) |
| 394 | `0x07640c` | `0x0763fc` | -16 | 4076 | 4076 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform18InsertKeyFrame_oldEPKfxi`<br>AbyssEngine::Transform::InsertKeyFrame_old(float const*, long long, int) |
| 395 | `0x0773f8` | `0x0773e8` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9Transform8DebugOutEi`<br>AbyssEngine::Transform::DebugOut(int) |
| 396 | `0x0773fa` | `0x0773ea` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9Transform9IsRunningEv`<br>AbyssEngine::Transform::IsRunning() |
| 397 | `0x077424` | `0x077414` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9Transform23SetCurrentAnimationTimeEx`<br>AbyssEngine::Transform::SetCurrentAnimationTime(long long) |
| 398 | `0x07742a` | `0x07741a` | -16 | 78 | 78 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform10SetVFCFlagEb`<br>AbyssEngine::Transform::SetVFCFlag(bool) |
| 399 | `0x077478` | `0x077468` | -16 | 304 | 304 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9Transform10InCameraVFEPNS_6AEMath6MatrixEPNS_6CameraE`<br>AbyssEngine::Transform::InCameraVF(AbyssEngine::AEMath::Matrix*, AbyssEngine::Camera*) |
| 400 | `0x0775a8` | `0x077598` | -16 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9TransformD2Ev`<br>AbyssEngine::Transform::~Transform() |
| 401 | `0x0775a8` | `0x077598` | -16 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9TransformD1Ev`<br>AbyssEngine::Transform::~Transform() |
| 402 | `0x0775e8` | `0x0775d8` | -16 | 72 | 72 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIPN11AbyssEngine8KeyFrameEEvR5ArrayIT_E`<br>void ArrayReleaseClasses<AbyssEngine::KeyFrame*>(Array<AbyssEngine::KeyFrame*>&) |
| 403 | `0x077630` | `0x077620` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine9Transform10SetVisibleEb`<br>AbyssEngine::Transform::SetVisible(bool) |
| 404 | `0x077636` | `0x077626` | -16 | 60 | 60 | 0 | 100.0% |  | Y | `_Z8ArraySetIPN11AbyssEngine8KeyFrameEEvPKT_jR5ArrayIS3_E`<br>void ArraySet<AbyssEngine::KeyFrame*>(AbyssEngine::KeyFrame* const*, unsigned int, Array<AbyssEngine::KeyFrame*>&) |
| 405 | `0x077674` | `0x077664` | -16 | 520 | 520 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvasC2EPNS_6EngineE`<br>AbyssEngine::PaintCanvas::PaintCanvas(AbyssEngine::Engine*) |
| 406 | `0x077674` | `0x077664` | -16 | 520 | 520 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvasC1EPNS_6EngineE`<br>AbyssEngine::PaintCanvas::PaintCanvas(AbyssEngine::Engine*) |
| 407 | `0x07787c` | `0x07786c` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine15AELoadedTextureEEC2Ev`<br>Array<AbyssEngine::AELoadedTexture*>::Array() |
| 408 | `0x077898` | `0x077888` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine8ResourceEEC2Ev`<br>Array<AbyssEngine::Resource*>::Array() |
| 409 | `0x0778b4` | `0x0778a4` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine9ImageFontEEC2Ev`<br>Array<AbyssEngine::ImageFont*>::Array() |
| 410 | `0x0778d0` | `0x0778c0` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine7Image2DEEC2Ev`<br>Array<AbyssEngine::Image2D*>::Array() |
| 411 | `0x0778ec` | `0x0778dc` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine6CameraEEC2Ev`<br>Array<AbyssEngine::Camera*>::Array() |
| 412 | `0x077908` | `0x0778f8` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine8MaterialEEC2Ev`<br>Array<AbyssEngine::Material*>::Array() |
| 413 | `0x077924` | `0x077914` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine12SpriteSystemEEC2Ev`<br>Array<AbyssEngine::SpriteSystem*>::Array() |
| 414 | `0x077940` | `0x077930` | -16 | 78 | 78 | 0 | 100.0% |  | Y | `_ZN5ArrayIN11AbyssEngine6AEMath6MatrixEEC2Ev`<br>Array<AbyssEngine::AEMath::Matrix>::Array() |
| 415 | `0x07798e` | `0x07797e` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIjEC2Ev`<br>Array<unsigned int>::Array() |
| 416 | `0x0779aa` | `0x07799a` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIN11AbyssEngine6AEMath6MatrixEED2Ev`<br>Array<AbyssEngine::AEMath::Matrix>::~Array() |
| 417 | `0x0779c0` | `0x0779b0` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIjED2Ev`<br>Array<unsigned int>::~Array() |
| 418 | `0x0779d6` | `0x0779c6` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine12SpriteSystemEED2Ev`<br>Array<AbyssEngine::SpriteSystem*>::~Array() |
| 419 | `0x0779ec` | `0x0779dc` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine8MaterialEED2Ev`<br>Array<AbyssEngine::Material*>::~Array() |
| 420 | `0x077a02` | `0x0779f2` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine6CameraEED2Ev`<br>Array<AbyssEngine::Camera*>::~Array() |
| 421 | `0x077a18` | `0x077a08` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine7Image2DEED2Ev`<br>Array<AbyssEngine::Image2D*>::~Array() |
| 422 | `0x077a2e` | `0x077a1e` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine9ImageFontEED2Ev`<br>Array<AbyssEngine::ImageFont*>::~Array() |
| 423 | `0x077a44` | `0x077a34` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine8ResourceEED2Ev`<br>Array<AbyssEngine::Resource*>::~Array() |
| 424 | `0x077a5a` | `0x077a4a` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine15AELoadedTextureEED2Ev`<br>Array<AbyssEngine::AELoadedTexture*>::~Array() |
| 425 | `0x077a70` | `0x077a60` | -16 | 532 | 532 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvasD1Ev`<br>AbyssEngine::PaintCanvas::~PaintCanvas() |
| 426 | `0x077a70` | `0x077a60` | -16 | 532 | 532 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvasD2Ev`<br>AbyssEngine::PaintCanvas::~PaintCanvas() |
| 427 | `0x077c84` | `0x077c74` | -16 | 568 | 568 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas19ReleaseAllResourcesEv`<br>AbyssEngine::PaintCanvas::ReleaseAllResources() |
| 428 | `0x077ebc` | `0x077eac` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15ResourceTextureD2Ev`<br>AbyssEngine::ResourceTexture::~ResourceTexture() |
| 429 | `0x077ed0` | `0x077ec0` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ResourceMeshD2Ev`<br>AbyssEngine::ResourceMesh::~ResourceMesh() |
| 430 | `0x077ee4` | `0x077ed4` | -16 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17ResourceTransformD2Ev`<br>AbyssEngine::ResourceTransform::~ResourceTransform() |
| 431 | `0x077f04` | `0x077ef4` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIPN11AbyssEngine4MeshEEvR5ArrayIT_E`<br>void ArrayRelease<AbyssEngine::Mesh*>(Array<AbyssEngine::Mesh*>&) |
| 432 | `0x077f18` | `0x077f08` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIN11AbyssEngine6AEMath6MatrixEEvR5ArrayIT_E`<br>void ArrayRelease<AbyssEngine::AEMath::Matrix>(Array<AbyssEngine::AEMath::Matrix>&) |
| 433 | `0x077f2c` | `0x077f1c` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIjEvR5ArrayIT_E`<br>void ArrayRelease<unsigned int>(Array<unsigned int>&) |
| 434 | `0x077f40` | `0x077f30` | -16 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas15SetResourceListEPKPNS_8ResourceEj`<br>AbyssEngine::PaintCanvas::SetResourceList(AbyssEngine::Resource* const*, unsigned int) |
| 435 | `0x077f4e` | `0x077f3e` | -16 | 54 | 54 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine8ResourceEEvPKT_jR5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::Resource*>(AbyssEngine::Resource* const*, unsigned int, Array<AbyssEngine::Resource*>&) |
| 436 | `0x077f84` | `0x077f74` | -16 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11AddResourceEPNS_8ResourceE`<br>AbyssEngine::PaintCanvas::AddResource(AbyssEngine::Resource*) |
| 437 | `0x077f90` | `0x077f80` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine8ResourceEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::Resource*>(AbyssEngine::Resource*, Array<AbyssEngine::Resource*>&) |
| 438 | `0x077fb4` | `0x077fa4` | -16 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17GetMeshResourceIdERNS_6StringEt`<br>AbyssEngine::PaintCanvas::GetMeshResourceId(AbyssEngine::String&, unsigned short) |
| 439 | `0x07800a` | `0x077ffa` | -16 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17GetMeshResourceIdERNS_6StringE`<br>AbyssEngine::PaintCanvas::GetMeshResourceId(AbyssEngine::String&) |
| 440 | `0x078056` | `0x078046` | -16 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20GetTextureResourceIdERNS_6StringE`<br>AbyssEngine::PaintCanvas::GetTextureResourceId(AbyssEngine::String&) |
| 441 | `0x0780a4` | `0x078094` | -16 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16GetReverseStringENS_6StringE`<br>AbyssEngine::PaintCanvas::GetReverseString(AbyssEngine::String) |
| 442 | `0x078104` | `0x0780f4` | -16 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16GetReverseStringENS_6StringEb`<br>AbyssEngine::PaintCanvas::GetReverseString(AbyssEngine::String, bool) |
| 443 | `0x078194` | `0x078184` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11CheckStringEjRKNS_6StringE`<br>AbyssEngine::PaintCanvas::CheckString(unsigned int, AbyssEngine::String const&) |
| 444 | `0x0781c0` | `0x0781b0` | -16 | 82 | 82 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10DrawStringEjPKtiib`<br>AbyssEngine::PaintCanvas::DrawString(unsigned int, unsigned short const*, int, int, bool) |
| 445 | `0x078212` | `0x078202` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10SetTextureEjj`<br>AbyssEngine::PaintCanvas::SetTexture(unsigned int, unsigned int) |
| 446 | `0x078218` | `0x078208` | -16 | 90 | 90 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib`<br>AbyssEngine::PaintCanvas::DrawString(unsigned int, AbyssEngine::String const&, int, int, bool) |
| 447 | `0x078274` | `0x078264` | -16 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas15DrawStringColorEjRKNS_6StringEiib`<br>AbyssEngine::PaintCanvas::DrawStringColor(unsigned int, AbyssEngine::String const&, int, int, bool) |
| 448 | `0x0783d4` | `0x0783c4` | -16 | 88 | 88 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas8GetColorEv`<br>AbyssEngine::PaintCanvas::GetColor() |
| 449 | `0x07842c` | `0x07841c` | -16 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringE`<br>AbyssEngine::PaintCanvas::GetTextWidth(unsigned int, AbyssEngine::String const&) |
| 450 | `0x078460` | `0x078450` | -16 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas8SetColorEj`<br>AbyssEngine::PaintCanvas::SetColor(unsigned int) |
| 451 | `0x0784e8` | `0x0784d8` | -16 | 74 | 74 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIPN11AbyssEngine6StringEEvR5ArrayIT_E`<br>void ArrayReleaseClasses<AbyssEngine::String*>(Array<AbyssEngine::String*>&) |
| 452 | `0x078538` | `0x078528` | -16 | 728 | 728 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12DrawRegion2DEjiiiifiiii`<br>AbyssEngine::PaintCanvas::DrawRegion2D(unsigned int, int, int, int, int, float, int, int, int, int) |
| 453 | `0x078810` | `0x078800` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas18SetWorldViewMatrixERKNS_6AEMath6MatrixE`<br>AbyssEngine::PaintCanvas::SetWorldViewMatrix(AbyssEngine::AEMath::Matrix const&) |
| 454 | `0x078818` | `0x078808` | -16 | 584 | 584 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12DrawRegion2DEjfiiiiff`<br>AbyssEngine::PaintCanvas::DrawRegion2D(unsigned int, float, int, int, int, int, float, float) |
| 455 | `0x078a60` | `0x078a50` | -16 | 208 | 208 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas14RestoreImage2DEPNS_7Image2DE`<br>AbyssEngine::PaintCanvas::RestoreImage2D(AbyssEngine::Image2D*) |
| 456 | `0x078b30` | `0x078b20` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13MeshSet2DMaskEjii`<br>AbyssEngine::PaintCanvas::MeshSet2DMask(unsigned int, int, int) |
| 457 | `0x078b60` | `0x078b50` | -16 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas15MeshClear2DMaskEv`<br>AbyssEngine::PaintCanvas::MeshClear2DMask() |
| 458 | `0x078b68` | `0x078b58` | -16 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjii`<br>AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int) |
| 459 | `0x078c48` | `0x078c38` | -16 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiih`<br>AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int, unsigned char) |
| 460 | `0x078d9c` | `0x078d8c` | -16 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas15GetImage2DWidthEj`<br>AbyssEngine::PaintCanvas::GetImage2DWidth(unsigned int) |
| 461 | `0x078db4` | `0x078da4` | -16 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas16GetImage2DHeightEj`<br>AbyssEngine::PaintCanvas::GetImage2DHeight(unsigned int) |
| 462 | `0x078dd0` | `0x078dc0` | -16 | 484 | 484 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiihh`<br>AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int, unsigned char, unsigned char) |
| 463 | `0x078fb4` | `0x078fa4` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas8GetWidthEv`<br>AbyssEngine::PaintCanvas::GetWidth() |
| 464 | `0x078fba` | `0x078faa` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas9GetHeightEv`<br>AbyssEngine::PaintCanvas::GetHeight() |
| 465 | `0x078fc0` | `0x078fb0` | -16 | 708 | 708 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiiiihhh`<br>AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int, int, int, unsigned char, unsigned char, unsigned char) |
| 466 | `0x079284` | `0x079274` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas22DrawFrameBufferTextureEiiiiii`<br>AbyssEngine::PaintCanvas::DrawFrameBufferTexture(int, int, int, int, int, int) |
| 467 | `0x079288` | `0x079278` | -16 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13FillRectangleEiiii`<br>AbyssEngine::PaintCanvas::FillRectangle(int, int, int, int) |
| 468 | `0x079368` | `0x079358` | -16 | 364 | 364 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13DrawRectangleEiiii`<br>AbyssEngine::PaintCanvas::DrawRectangle(int, int, int, int) |
| 469 | `0x0794d8` | `0x0794c8` | -16 | 436 | 436 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas8DrawLineEiiii`<br>AbyssEngine::PaintCanvas::DrawLine(int, int, int, int) |
| 470 | `0x07968c` | `0x07967c` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas8DrawMeshEj`<br>AbyssEngine::PaintCanvas::DrawMesh(unsigned int) |
| 471 | `0x0796a0` | `0x079690` | -16 | 580 | 580 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13DrawTransformEjPKNS_6AEMath6MatrixE`<br>AbyssEngine::PaintCanvas::DrawTransform(unsigned int, AbyssEngine::AEMath::Matrix const*) |
| 472 | `0x0798e4` | `0x0798d4` | -16 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13DrawTransformEPNS_9TransformERKNS_6AEMath6MatrixERS4_`<br>AbyssEngine::PaintCanvas::DrawTransform(AbyssEngine::Transform*, AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix&) |
| 473 | `0x0799b8` | `0x0799a8` | -16 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas19TextureCreateGlobalENS_6StringEj`<br>AbyssEngine::PaintCanvas::TextureCreateGlobal(AbyssEngine::String, unsigned int) |
| 474 | `0x079a34` | `0x079a24` | -16 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13TextureCreateEtPFvPNS_5ImageEPvES3_Rjb`<br>AbyssEngine::PaintCanvas::TextureCreate(unsigned short, void (*)(AbyssEngine::Image*, void*), void*, unsigned int&, bool) |
| 475 | `0x079ad0` | `0x079ac0` | -16 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas12FindResourceEt`<br>AbyssEngine::PaintCanvas::FindResource(unsigned short) |
| 476 | `0x079b00` | `0x079af0` | -16 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13TextureCreateEtRjb`<br>AbyssEngine::PaintCanvas::TextureCreate(unsigned short, unsigned int&, bool) |
| 477 | `0x079b14` | `0x079b04` | -16 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10FontCreateEtRjb`<br>AbyssEngine::PaintCanvas::FontCreate(unsigned short, unsigned int&, bool) |
| 478 | `0x079bf4` | `0x079be4` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine9ImageFontEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::ImageFont*>(AbyssEngine::ImageFont*, Array<AbyssEngine::ImageFont*>&) |
| 479 | `0x079c18` | `0x079c08` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14FontSetSpacingEjs`<br>AbyssEngine::PaintCanvas::FontSetSpacing(unsigned int, short) |
| 480 | `0x079c30` | `0x079c20` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14FontGetSpacingEj`<br>AbyssEngine::PaintCanvas::FontGetSpacing(unsigned int) |
| 481 | `0x079c48` | `0x079c38` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14FontSetYOffsetEjs`<br>AbyssEngine::PaintCanvas::FontSetYOffset(unsigned int, short) |
| 482 | `0x079c60` | `0x079c50` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14FontGetYOffsetEj`<br>AbyssEngine::PaintCanvas::FontGetYOffset(unsigned int) |
| 483 | `0x079c78` | `0x079c68` | -16 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj`<br>AbyssEngine::PaintCanvas::Image2DCreate(unsigned short, unsigned int&) |
| 484 | `0x079d38` | `0x079d28` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine7Image2DEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::Image2D*>(AbyssEngine::Image2D*, Array<AbyssEngine::Image2D*>&) |
| 485 | `0x079d5c` | `0x079d4c` | -16 | 240 | 240 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10MeshCreateEtRjb`<br>AbyssEngine::PaintCanvas::MeshCreate(unsigned short, unsigned int&, bool) |
| 486 | `0x079e4c` | `0x079e3c` | -16 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14MaterialCreateEtRj`<br>AbyssEngine::PaintCanvas::MaterialCreate(unsigned short, unsigned int&) |
| 487 | `0x079f2c` | `0x079f1c` | -16 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10MeshCreateEttatRj`<br>AbyssEngine::PaintCanvas::MeshCreate(unsigned short, unsigned short, signed char, unsigned short, unsigned int&) |
| 488 | `0x079fb4` | `0x079fa4` | -16 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10MeshCreateEttaRj`<br>AbyssEngine::PaintCanvas::MeshCreate(unsigned short, unsigned short, signed char, unsigned int&) |
| 489 | `0x07a00c` | `0x079ffc` | -16 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas20MeshSetTriangleCountEjt`<br>AbyssEngine::PaintCanvas::MeshSetTriangleCount(unsigned int, unsigned short) |
| 490 | `0x07a02a` | `0x07a01a` | -16 | 58 | 58 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas12MeshSetPointEjtfff`<br>AbyssEngine::PaintCanvas::MeshSetPoint(unsigned int, unsigned short, float, float, float) |
| 491 | `0x07a064` | `0x07a054` | -16 | 82 | 82 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas18MeshTranslatePointEjtfff`<br>AbyssEngine::PaintCanvas::MeshTranslatePoint(unsigned int, unsigned short, float, float, float) |
| 492 | `0x07a0b8` | `0x07a0a8` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas9MeshSetUvEjtff`<br>AbyssEngine::PaintCanvas::MeshSetUv(unsigned int, unsigned short, float, float) |
| 493 | `0x07a100` | `0x07a0f0` | -16 | 136 | 136 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas12MeshSetColorEjtj`<br>AbyssEngine::PaintCanvas::MeshSetColor(unsigned int, unsigned short, unsigned int) |
| 494 | `0x07a188` | `0x07a178` | -16 | 62 | 62 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas12MeshSetColorEjtffff`<br>AbyssEngine::PaintCanvas::MeshSetColor(unsigned int, unsigned short, float, float, float, float) |
| 495 | `0x07a1c6` | `0x07a1b6` | -16 | 60 | 60 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas15MeshSetTriangleEjtttt`<br>AbyssEngine::PaintCanvas::MeshSetTriangle(unsigned int, unsigned short, unsigned short, unsigned short, unsigned short) |
| 496 | `0x07a202` | `0x07a1f2` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13MeshSetNormalEjtRKNS_6AEMath6VectorE`<br>AbyssEngine::PaintCanvas::MeshSetNormal(unsigned int, unsigned short, AbyssEngine::AEMath::Vector const&) |
| 497 | `0x07a232` | `0x07a222` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14MeshSetTangentEjtRKNS_6AEMath6VectorE`<br>AbyssEngine::PaintCanvas::MeshSetTangent(unsigned int, unsigned short, AbyssEngine::AEMath::Vector const&) |
| 498 | `0x07a262` | `0x07a252` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16MeshSetBiTangentEjtRKNS_6AEMath6VectorE`<br>AbyssEngine::PaintCanvas::MeshSetBiTangent(unsigned int, unsigned short, AbyssEngine::AEMath::Vector const&) |
| 499 | `0x07a292` | `0x07a282` | -16 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16MeshConvertToVBOEj`<br>AbyssEngine::PaintCanvas::MeshConvertToVBO(unsigned int) |
| 500 | `0x07a2a4` | `0x07a294` | -16 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas25MeshChangeShaderAnimValueEPNS_4MeshEfj`<br>AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(AbyssEngine::Mesh*, float, unsigned int) |
| 501 | `0x07a2d2` | `0x07a2c2` | -16 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas25MeshChangeShaderAnimValueEPNS_9TransformEfj`<br>AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(AbyssEngine::Transform*, float, unsigned int) |
| 502 | `0x07a322` | `0x07a312` | -16 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas24MeshChangeMaterialInternEPNS_9TransformEPNS_8MaterialE`<br>AbyssEngine::PaintCanvas::MeshChangeMaterialIntern(AbyssEngine::Transform*, AbyssEngine::Material*) |
| 503 | `0x07a372` | `0x07a362` | -16 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas24MeshChangeMaterialInternEPNS_4MeshEPNS_8MaterialE`<br>AbyssEngine::PaintCanvas::MeshChangeMaterialIntern(AbyssEngine::Mesh*, AbyssEngine::Material*) |
| 504 | `0x07a380` | `0x07a370` | -16 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17MeshCloneMaterialEjRj`<br>AbyssEngine::PaintCanvas::MeshCloneMaterial(unsigned int, unsigned int&) |
| 505 | `0x07a3d8` | `0x07a3c8` | -16 | 276 | 276 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8MaterialC2EPS0_`<br>AbyssEngine::Material::Material(AbyssEngine::Material*) |
| 506 | `0x07a4ec` | `0x07a4dc` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine8MaterialEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::Material*>(AbyssEngine::Material*, Array<AbyssEngine::Material*>&) |
| 507 | `0x07a510` | `0x07a500` | -16 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas26MeshChangeResourceMaterialEjt`<br>AbyssEngine::PaintCanvas::MeshChangeResourceMaterial(unsigned int, unsigned short) |
| 508 | `0x07a540` | `0x07a530` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas18MeshChangeMaterialEjt`<br>AbyssEngine::PaintCanvas::MeshChangeMaterial(unsigned int, unsigned short) |
| 509 | `0x07a562` | `0x07a552` | -16 | 44 | 44 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas26MeshResourceChangeMaterialEtt`<br>AbyssEngine::PaintCanvas::MeshResourceChangeMaterial(unsigned short, unsigned short) |
| 510 | `0x07a58e` | `0x07a57e` | -16 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas29MeshResourceChangeAllMaterialEtt`<br>AbyssEngine::PaintCanvas::MeshResourceChangeAllMaterial(unsigned short, unsigned short) |
| 511 | `0x07a5bc` | `0x07a5ac` | -16 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas14MeshGetPointerEj`<br>AbyssEngine::PaintCanvas::MeshGetPointer(unsigned int) |
| 512 | `0x07a5cc` | `0x07a5bc` | -16 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas15MeshGetTriCountEPNS_4MeshE`<br>AbyssEngine::PaintCanvas::MeshGetTriCount(AbyssEngine::Mesh*) |
| 513 | `0x07a5f2` | `0x07a5e2` | -16 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20TransformGetTriCountEPNS_9TransformE`<br>AbyssEngine::PaintCanvas::TransformGetTriCount(AbyssEngine::Transform*) |
| 514 | `0x07a648` | `0x07a638` | -16 | 176 | 176 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas18SpriteSystemCreateEtbtRj`<br>AbyssEngine::PaintCanvas::SpriteSystemCreate(unsigned short, bool, unsigned short, unsigned int&) |
| 515 | `0x07a6f8` | `0x07a6e8` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine12SpriteSystemEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::SpriteSystem*>(AbyssEngine::SpriteSystem*, Array<AbyssEngine::SpriteSystem*>&) |
| 516 | `0x07a71c` | `0x07a70c` | -16 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas18SpriteSystemCreateEtbRj`<br>AbyssEngine::PaintCanvas::SpriteSystemCreate(unsigned short, bool, unsigned int&) |
| 517 | `0x07a7a0` | `0x07a790` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas22SpriteSystemSetAllSizeEjs`<br>AbyssEngine::PaintCanvas::SpriteSystemSetAllSize(unsigned int, short) |
| 518 | `0x07a7bc` | `0x07a7ac` | -16 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20SpriteSystemSetAllUvEjffff`<br>AbyssEngine::PaintCanvas::SpriteSystemSetAllUv(unsigned int, float, float, float, float) |
| 519 | `0x07a7f8` | `0x07a7e8` | -16 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17SpriteSystemSetUvEjtffff`<br>AbyssEngine::PaintCanvas::SpriteSystemSetUv(unsigned int, unsigned short, float, float, float, float) |
| 520 | `0x07a840` | `0x07a830` | -16 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas19SpriteSystemSetRGBAEjtffff`<br>AbyssEngine::PaintCanvas::SpriteSystemSetRGBA(unsigned int, unsigned short, float, float, float, float) |
| 521 | `0x07a888` | `0x07a878` | -16 | 42 | 42 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas19SpriteSystemSetSizeEjts`<br>AbyssEngine::PaintCanvas::SpriteSystemSetSize(unsigned int, unsigned short, short) |
| 522 | `0x07a8b2` | `0x07a8a2` | -16 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas19SpriteSystemAddSizeEjts`<br>AbyssEngine::PaintCanvas::SpriteSystemAddSize(unsigned int, unsigned short, short) |
| 523 | `0x07a8e6` | `0x07a8d6` | -16 | 188 | 188 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas23SpriteSystemGetPositionEjtRKNS_6AEMath6MatrixERNS1_6VectorE`<br>AbyssEngine::PaintCanvas::SpriteSystemGetPosition(unsigned int, unsigned short, AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector&) |
| 524 | `0x07a9a2` | `0x07a992` | -16 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas23SpriteSystemGetPositionEjtRNS_6AEMath6VectorE`<br>AbyssEngine::PaintCanvas::SpriteSystemGetPosition(unsigned int, unsigned short, AbyssEngine::AEMath::Vector&) |
| 525 | `0x07a9d6` | `0x07a9c6` | -16 | 62 | 62 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas23SpriteSystemSetPositionEjtfff`<br>AbyssEngine::PaintCanvas::SpriteSystemSetPosition(unsigned int, unsigned short, float, float, float) |
| 526 | `0x07aa14` | `0x07aa04` | -16 | 86 | 86 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas23SpriteSystemAddPositionEjtfff`<br>AbyssEngine::PaintCanvas::SpriteSystemAddPosition(unsigned int, unsigned short, float, float, float) |
| 527 | `0x07aa70` | `0x07aa60` | -16 | 428 | 428 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16DrawSpriteSystemEj`<br>AbyssEngine::PaintCanvas::DrawSpriteSystem(unsigned int) |
| 528 | `0x07ac20` | `0x07ac10` | -16 | 396 | 396 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16DrawSpriteSystemEjNS_6AEMath6MatrixE`<br>AbyssEngine::PaintCanvas::DrawSpriteSystem(unsigned int, AbyssEngine::AEMath::Matrix) |
| 529 | `0x07adb0` | `0x07ada0` | -16 | 436 | 436 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16DrawSpriteSystemEjNS_6AEMath6MatrixES2_`<br>AbyssEngine::PaintCanvas::DrawSpriteSystem(unsigned int, AbyssEngine::AEMath::Matrix, AbyssEngine::AEMath::Matrix) |
| 530 | `0x07af64` | `0x07af54` | -16 | 268 | 268 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas15TransformCreateEtRj`<br>AbyssEngine::PaintCanvas::TransformCreate(unsigned short, unsigned int&) |
| 531 | `0x07b070` | `0x07b060` | -16 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas15TransformCreateERj`<br>AbyssEngine::PaintCanvas::TransformCreate(unsigned int&) |
| 532 | `0x07b0b0` | `0x07b0a0` | -16 | 200 | 200 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16TransformAddMeshEjtb`<br>AbyssEngine::PaintCanvas::TransformAddMesh(unsigned int, unsigned short, bool) |
| 533 | `0x07b178` | `0x07b168` | -16 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas19TransformRemoveMeshEjt`<br>AbyssEngine::PaintCanvas::TransformRemoveMesh(unsigned int, unsigned short) |
| 534 | `0x07b1a4` | `0x07b194` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas21TransformRemoveMeshIdEjj`<br>AbyssEngine::PaintCanvas::TransformRemoveMeshId(unsigned int, unsigned int) |
| 535 | `0x07b1c6` | `0x07b1b6` | -16 | 194 | 194 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas18TransformAddMeshIdEjj`<br>AbyssEngine::PaintCanvas::TransformAddMeshId(unsigned int, unsigned int) |
| 536 | `0x07b288` | `0x07b278` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_Z11ArrayRemoveIPN11AbyssEngine4MeshEEvT_R5ArrayIS3_E`<br>void ArrayRemove<AbyssEngine::Mesh*>(AbyssEngine::Mesh*, Array<AbyssEngine::Mesh*>&) |
| 537 | `0x07b2cc` | `0x07b2bc` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17TransformSetLocalEjRKNS_6AEMath6MatrixE`<br>AbyssEngine::PaintCanvas::TransformSetLocal(unsigned int, AbyssEngine::AEMath::Matrix const&) |
| 538 | `0x07b2ee` | `0x07b2de` | -16 | 94 | 94 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17TransformAddChildEjj`<br>AbyssEngine::PaintCanvas::TransformAddChild(unsigned int, unsigned int) |
| 539 | `0x07b34c` | `0x07b33c` | -16 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20TransformRemoveChildEjj`<br>AbyssEngine::PaintCanvas::TransformRemoveChild(unsigned int, unsigned int) |
| 540 | `0x07b388` | `0x07b378` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_Z11ArrayRemoveIPN11AbyssEngine9TransformEEvT_R5ArrayIS3_E`<br>void ArrayRemove<AbyssEngine::Transform*>(AbyssEngine::Transform*, Array<AbyssEngine::Transform*>&) |
| 541 | `0x07b3cc` | `0x07b3bc` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17TransformGetLocalEj`<br>AbyssEngine::PaintCanvas::TransformGetLocal(unsigned int) |
| 542 | `0x07b410` | `0x07b400` | -16 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas17TransformSetColorEjj`<br>AbyssEngine::PaintCanvas::TransformSetColor(unsigned int, unsigned int) |
| 543 | `0x07b424` | `0x07b414` | -16 | 364 | 364 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas33TransformGet2DPickedTextureRegionEPNS_9TransformEiiii`<br>AbyssEngine::PaintCanvas::TransformGet2DPickedTextureRegion(AbyssEngine::Transform*, int, int, int, int) |
| 544 | `0x07b590` | `0x07b580` | -16 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas33TransformGet2DPickedTextureRegionEjiiii`<br>AbyssEngine::PaintCanvas::TransformGet2DPickedTextureRegion(unsigned int, int, int, int, int) |
| 545 | `0x07b67c` | `0x07b66c` | -16 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas21TransformGetTransformEj`<br>AbyssEngine::PaintCanvas::TransformGetTransform(unsigned int) |
| 546 | `0x07b690` | `0x07b680` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20TransformGetTriCountEj`<br>AbyssEngine::PaintCanvas::TransformGetTriCount(unsigned int) |
| 547 | `0x07b6a8` | `0x07b698` | -16 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12CameraCreateERj`<br>AbyssEngine::PaintCanvas::CameraCreate(unsigned int&) |
| 548 | `0x07b724` | `0x07b714` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6CameraC2Efffff`<br>AbyssEngine::Camera::Camera(float, float, float, float, float) |
| 549 | `0x07b75c` | `0x07b74c` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine6CameraEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::Camera*>(AbyssEngine::Camera*, Array<AbyssEngine::Camera*>&) |
| 550 | `0x07b780` | `0x07b770` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14CameraSetLocalEjRKNS_6AEMath6MatrixE`<br>AbyssEngine::PaintCanvas::CameraSetLocal(unsigned int, AbyssEngine::AEMath::Matrix const&) |
| 551 | `0x07b7a4` | `0x07b794` | -16 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14CameraGetLocalEj`<br>AbyssEngine::PaintCanvas::CameraGetLocal(unsigned int) |
| 552 | `0x07b7ec` | `0x07b7dc` | -16 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas16CameraSetCurrentEj`<br>AbyssEngine::PaintCanvas::CameraSetCurrent(unsigned int) |
| 553 | `0x07b808` | `0x07b7f8` | -16 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas21SetProjectionMatrix3dEfff`<br>AbyssEngine::PaintCanvas::SetProjectionMatrix3d(float, float, float) |
| 554 | `0x07b968` | `0x07b958` | -16 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas16CameraGetCurrentEv`<br>AbyssEngine::PaintCanvas::CameraGetCurrent() |
| 555 | `0x07b97a` | `0x07b96a` | -16 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas23CameraGetCurrentFactor1Ev`<br>AbyssEngine::PaintCanvas::CameraGetCurrentFactor1() |
| 556 | `0x07b99e` | `0x07b98e` | -16 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20CameraSetPerspectiveEjfff`<br>AbyssEngine::PaintCanvas::CameraSetPerspective(unsigned int, float, float, float) |
| 557 | `0x07ba1e` | `0x07ba0e` | -16 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20CameraSetPerspectiveEjff`<br>AbyssEngine::PaintCanvas::CameraSetPerspective(unsigned int, float, float) |
| 558 | `0x07ba98` | `0x07ba88` | -16 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas26CameraIsPointinViewFrustumERKNS_6AEMath6VectorE`<br>AbyssEngine::PaintCanvas::CameraIsPointinViewFrustum(AbyssEngine::AEMath::Vector const&) |
| 559 | `0x07bbb8` | `0x07bba8` | -16 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas27CameraIsSphereinViewFrustumERKNS_6AEMath6VectorEf`<br>AbyssEngine::PaintCanvas::CameraIsSphereinViewFrustum(AbyssEngine::AEMath::Vector const&, float) |
| 560 | `0x07bce8` | `0x07bcd8` | -16 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17GetScreenPositionERKNS_6AEMath6VectorERS2_`<br>AbyssEngine::PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector&) |
| 561 | `0x07bd58` | `0x07bd48` | -16 | 604 | 604 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17GetScreenPositionERNS_6AEMath6MatrixERKNS1_6VectorERS4_`<br>AbyssEngine::PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Matrix&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector&) |
| 562 | `0x07bfb4` | `0x07bfa4` | -16 | 392 | 392 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17GetScreenPositionERKNS_6AEMath6MatrixERNS1_6VectorE`<br>AbyssEngine::PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector&) |
| 563 | `0x07c13c` | `0x07c12c` | -16 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14MaterialCreateERjNS_9BlendModeEjt`<br>AbyssEngine::PaintCanvas::MaterialCreate(unsigned int&, AbyssEngine::BlendMode, unsigned int, unsigned short) |
| 564 | `0x07c188` | `0x07c178` | -16 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8MaterialC2Ev`<br>AbyssEngine::Material::Material() |
| 565 | `0x07c220` | `0x07c210` | -16 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas14MaterialChangeEjNS_9BlendModeEj`<br>AbyssEngine::PaintCanvas::MaterialChange(unsigned int, AbyssEngine::BlendMode, unsigned int) |
| 566 | `0x07c238` | `0x07c228` | -16 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas29MaterialResourceChangeTextureEjji`<br>AbyssEngine::PaintCanvas::MaterialResourceChangeTexture(unsigned int, unsigned int, int) |
| 567 | `0x07c274` | `0x07c264` | -16 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas19MaterialGetMaterialEj`<br>AbyssEngine::PaintCanvas::MaterialGetMaterial(unsigned int) |
| 568 | `0x07c288` | `0x07c278` | -16 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas17ChangeCubeTextureEj`<br>AbyssEngine::PaintCanvas::ChangeCubeTexture(unsigned int) |
| 569 | `0x07c2f0` | `0x07c2e0` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12HasVibrationEv`<br>AbyssEngine::PaintCanvas::HasVibration() |
| 570 | `0x07c2f6` | `0x07c2e6` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas7VibrateEt`<br>AbyssEngine::PaintCanvas::Vibrate(unsigned short) |
| 571 | `0x07c2fc` | `0x07c2ec` | -16 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12GetTextWidthEjRKNS_6StringEjj`<br>AbyssEngine::PaintCanvas::GetTextWidth(unsigned int, AbyssEngine::String const&, unsigned int, unsigned int) |
| 572 | `0x07c33c` | `0x07c32c` | -16 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13GetTextHeightEj`<br>AbyssEngine::PaintCanvas::GetTextHeight(unsigned int) |
| 573 | `0x07c354` | `0x07c344` | -16 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas7GetLineEjNS_6StringEiPS1_`<br>AbyssEngine::PaintCanvas::GetLine(unsigned int, AbyssEngine::String, int, AbyssEngine::String*) |
| 574 | `0x07c488` | `0x07c478` | -16 | 516 | 516 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12GetLineArrayEjRKNS_6StringEiP5ArrayIPS1_E`<br>AbyssEngine::PaintCanvas::GetLineArray(unsigned int, AbyssEngine::String const&, int, Array<AbyssEngine::String*>*) |
| 575 | `0x07c68c` | `0x07c67c` | -16 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIPN11AbyssEngine6StringEEvjR5ArrayIT_E`<br>void ArraySetLength<AbyssEngine::String*>(unsigned int, Array<AbyssEngine::String*>&) |
| 576 | `0x07c6c0` | `0x07c6b0` | -16 | 102 | 102 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13DrawTextLinesEjP5ArrayIPNS_6StringEEiib`<br>AbyssEngine::PaintCanvas::DrawTextLines(unsigned int, Array<AbyssEngine::String*>*, int, int, bool) |
| 577 | `0x07c726` | `0x07c716` | -16 | 102 | 102 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13DrawTextLinesEjP5ArrayIPNS_6StringEEiijb`<br>AbyssEngine::PaintCanvas::DrawTextLines(unsigned int, Array<AbyssEngine::String*>*, int, int, unsigned int, bool) |
| 578 | `0x07c78c` | `0x07c77c` | -16 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13DrawTextLinesEjP5ArrayIPNS_6StringEEii`<br>AbyssEngine::PaintCanvas::DrawTextLines(unsigned int, Array<AbyssEngine::String*>*, int, int) |
| 579 | `0x07c7a4` | `0x07c794` | -16 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas14ResourceLoadedEjNS_12ResourceTypeE`<br>AbyssEngine::PaintCanvas::ResourceLoaded(unsigned int, AbyssEngine::ResourceType) |
| 580 | `0x07c7f4` | `0x07c7e4` | -16 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas27ReleaseSpriteSystemResourceEj`<br>AbyssEngine::PaintCanvas::ReleaseSpriteSystemResource(unsigned int) |
| 581 | `0x07c80e` | `0x07c7fe` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine9ImageFontEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::ImageFont*>(Array<AbyssEngine::ImageFont*>&) |
| 582 | `0x07c830` | `0x07c820` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine7Image2DEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::Image2D*>(Array<AbyssEngine::Image2D*>&) |
| 583 | `0x07c852` | `0x07c842` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine4MeshEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::Mesh*>(Array<AbyssEngine::Mesh*>&) |
| 584 | `0x07c874` | `0x07c864` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine9TransformEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::Transform*>(Array<AbyssEngine::Transform*>&) |
| 585 | `0x07c896` | `0x07c886` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine6CameraEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::Camera*>(Array<AbyssEngine::Camera*>&) |
| 586 | `0x07c8b8` | `0x07c8a8` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine8MaterialD2Ev`<br>AbyssEngine::Material::~Material() |
| 587 | `0x07c8f0` | `0x07c8e0` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine8MaterialEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::Material*>(Array<AbyssEngine::Material*>&) |
| 588 | `0x07c912` | `0x07c902` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine12SpriteSystemEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::SpriteSystem*>(Array<AbyssEngine::SpriteSystem*>&) |
| 589 | `0x07c938` | `0x07c928` | -16 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas8SetColorEhhhh`<br>AbyssEngine::PaintCanvas::SetColor(unsigned char, unsigned char, unsigned char, unsigned char) |
| 590 | `0x07c9b8` | `0x07c9a8` | -16 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas18SetProjOrthoMatrixEv`<br>AbyssEngine::PaintCanvas::SetProjOrthoMatrix() |
| 591 | `0x07ca84` | `0x07ca74` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11ClearBufferEj`<br>AbyssEngine::PaintCanvas::ClearBuffer(unsigned int) |
| 592 | `0x07caa6` | `0x07ca96` | -16 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10ClearDepthEv`<br>AbyssEngine::PaintCanvas::ClearDepth() |
| 593 | `0x07caae` | `0x07ca9e` | -16 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas10SwapBufferEv`<br>AbyssEngine::PaintCanvas::SwapBuffer() |
| 594 | `0x07cab0` | `0x07caa0` | -16 | 134 | 134 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10EnableClipEiiii`<br>AbyssEngine::PaintCanvas::EnableClip(int, int, int, int) |
| 595 | `0x07cb36` | `0x07cb26` | -16 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas11DisableClipEv`<br>AbyssEngine::PaintCanvas::DisableClip() |
| 596 | `0x07cb3e` | `0x07cb2e` | -16 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas13SetShaderModeEi`<br>AbyssEngine::PaintCanvas::SetShaderMode(int) |
| 597 | `0x07cb48` | `0x07cb38` | -16 | 764 | 764 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12SetBlendModeENS_9BlendModeE`<br>AbyssEngine::PaintCanvas::SetBlendMode(AbyssEngine::BlendMode) |
| 598 | `0x07ce44` | `0x07ce34` | -16 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas7Begin3dEv`<br>AbyssEngine::PaintCanvas::Begin3d() |
| 599 | `0x07cee0` | `0x07ced0` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas9FogEnableEbNS_7FogModeE`<br>AbyssEngine::PaintCanvas::FogEnable(bool, AbyssEngine::FogMode) |
| 600 | `0x07cf28` | `0x07cf18` | -16 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas15FogSetParameterENS_7FogModeEfffj`<br>AbyssEngine::PaintCanvas::FogSetParameter(AbyssEngine::FogMode, float, float, float, unsigned int) |
| 601 | `0x07d088` | `0x07d078` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13StartDraw2FBOEv`<br>AbyssEngine::PaintCanvas::StartDraw2FBO() |
| 602 | `0x07d08e` | `0x07d07e` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12StopDraw2FBOEv`<br>AbyssEngine::PaintCanvas::StopDraw2FBO() |
| 603 | `0x07d0b4` | `0x07d0a4` | -16 | 408 | 408 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas15ResetPersMatrixEv`<br>AbyssEngine::PaintCanvas::ResetPersMatrix() |
| 604 | `0x07d24c` | `0x07d23c` | -16 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13SetMatForGlowEPNS_8MaterialE`<br>AbyssEngine::PaintCanvas::SetMatForGlow(AbyssEngine::Material*) |
| 605 | `0x07d3cc` | `0x07d3bc` | -16 | 148 | 148 | 0 | 100.0% |  | Y | `_Z8ArrayAddIN11AbyssEngine6AEMath6MatrixEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::AEMath::Matrix>(AbyssEngine::AEMath::Matrix, Array<AbyssEngine::AEMath::Matrix>&) |
| 606 | `0x07d460` | `0x07d450` | -16 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIjEvT_R5ArrayIS0_E`<br>void ArrayAdd<unsigned int>(unsigned int, Array<unsigned int>&) |
| 607 | `0x07d484` | `0x07d474` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas20RemoveAllMatsForGlowEv`<br>AbyssEngine::PaintCanvas::RemoveAllMatsForGlow() |
| 608 | `0x07d4bc` | `0x07d4ac` | -16 | 38 | 38 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIN11AbyssEngine6AEMath6MatrixEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::AEMath::Matrix>(Array<AbyssEngine::AEMath::Matrix>&) |
| 609 | `0x07d4e2` | `0x07d4d2` | -16 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIjEvR5ArrayIT_E`<br>void ArrayRemoveAll<unsigned int>(Array<unsigned int>&) |
| 610 | `0x07d504` | `0x07d4f4` | -16 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas19CheckNUseRefractFBOEb`<br>AbyssEngine::PaintCanvas::CheckNUseRefractFBO(bool) |
| 611 | `0x07d53c` | `0x07d52c` | -16 | 3236 | 3236 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas5End3dEv`<br>AbyssEngine::PaintCanvas::End3d() |
| 612 | `0x07e1e0` | `0x07e1d0` | -16 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas7BeginBGEv`<br>AbyssEngine::PaintCanvas::BeginBG() |
| 613 | `0x07e28c` | `0x07e27c` | -16 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas5EndBGEv`<br>AbyssEngine::PaintCanvas::EndBG() |
| 614 | `0x07e2a0` | `0x07e290` | -16 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas7Begin2dEv`<br>AbyssEngine::PaintCanvas::Begin2d() |
| 615 | `0x07e378` | `0x07e368` | -16 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas5End2dEv`<br>AbyssEngine::PaintCanvas::End2d() |
| 616 | `0x07e3fc` | `0x07e3ec` | -16 | 704 | 704 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas18SetGameOrientationENS_13LandscapeModeE`<br>AbyssEngine::PaintCanvas::SetGameOrientation(AbyssEngine::LandscapeMode) |
| 617 | `0x07e6bc` | `0x07e6ac` | -16 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas10InitializeEb`<br>AbyssEngine::PaintCanvas::Initialize(bool) |
| 618 | `0x07e7bc` | `0x07e7ac` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas12GetGravValueEv`<br>AbyssEngine::PaintCanvas::GetGravValue() |
| 619 | `0x07e7c2` | `0x07e7b2` | -16 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas13GetAccelValueEv`<br>AbyssEngine::PaintCanvas::GetAccelValue() |
| 620 | `0x07e7c8` | `0x07e7b8` | -16 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas7SuspendEv`<br>AbyssEngine::PaintCanvas::Suspend() |
| 621 | `0x07e828` | `0x07e818` | -16 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas6ResumeEv`<br>AbyssEngine::PaintCanvas::Resume() |
| 622 | `0x07e8e4` | `0x07e8d4` | -16 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas14ReloadTexturesEv`<br>AbyssEngine::PaintCanvas::ReloadTextures() |
| 623 | `0x07e974` | `0x07e964` | -16 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine11PaintCanvas13WarmUpTextureEv`<br>AbyssEngine::PaintCanvas::WarmUpTexture() |
| 624 | `0x07e978` | `0x07e968` | -16 | 1296 | 1296 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PaintCanvas8DrawMeshEPNS_4MeshERNS_6AEMath6MatrixES5_jS5_`<br>AbyssEngine::PaintCanvas::DrawMesh(AbyssEngine::Mesh*, AbyssEngine::AEMath::Matrix&, AbyssEngine::AEMath::Matrix&, unsigned int, AbyssEngine::AEMath::Matrix&) |
| 625 | `0x07ee88` | `0x07ee78` | -16 | 68 | 68 | 0 | 100.0% |  | Y | `_Z14ArrayAddCachedIPN11AbyssEngine4MeshEEvT_R5ArrayIS3_E`<br>void ArrayAddCached<AbyssEngine::Mesh*>(AbyssEngine::Mesh*, Array<AbyssEngine::Mesh*>&) |
| 626 | `0x07eecc` | `0x07eebc` | -16 | 184 | 184 | 0 | 100.0% |  | Y | `_Z14ArrayAddCachedIN11AbyssEngine6AEMath6MatrixEEvT_R5ArrayIS3_E`<br>void ArrayAddCached<AbyssEngine::AEMath::Matrix>(AbyssEngine::AEMath::Matrix, Array<AbyssEngine::AEMath::Matrix>&) |
| 627 | `0x07ef84` | `0x07ef74` | -16 | 70 | 70 | 0 | 100.0% |  | Y | `_Z14ArrayAddCachedIjEvT_R5ArrayIS0_E`<br>void ArrayAddCached<unsigned int>(unsigned int, Array<unsigned int>&) |
| 628 | `0x07fb74` | `0x07fb7e` | +10 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource8SetSoundEPKNS_11AESoundInfoEi`<br>AbyssEngine::AESoundRessource::SetSound(AbyssEngine::AESoundInfo const*, int) |
| 629 | `0x07fb9c` | `0x07fba6` | +10 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIPN11AbyssEngine16AESoundInterfaceEEvjR5ArrayIT_E`<br>void ArraySetLength<AbyssEngine::AESoundInterface*>(unsigned int, Array<AbyssEngine::AESoundInterface*>&) |
| 630 | `0x07fbd0` | `0x07fbdc` | +12 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource18initWithoutLoadingEi`<br>AbyssEngine::AESoundRessource::initWithoutLoading(int) |
| 631 | `0x07fc38` | `0x07fc44` | +12 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource4initEi`<br>AbyssEngine::AESoundRessource::init(int) |
| 632 | `0x07fcc4` | `0x07fcd0` | +12 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource9setVolumeEii`<br>AbyssEngine::AESoundRessource::setVolume(int, int) |
| 633 | `0x07fd14` | `0x07fd20` | +12 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource9setVolumeEi`<br>AbyssEngine::AESoundRessource::setVolume(int) |
| 634 | `0x07fd42` | `0x07fd4e` | +12 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource14setSoundVolumeEi`<br>AbyssEngine::AESoundRessource::setSoundVolume(int) |
| 635 | `0x07fd70` | `0x07fd7c` | +12 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource14setMusicVolumeEi`<br>AbyssEngine::AESoundRessource::setMusicVolume(int) |
| 636 | `0x07fda0` | `0x07fdac` | +12 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource9isPlayingEi`<br>AbyssEngine::AESoundRessource::isPlaying(int) |
| 637 | `0x07fde8` | `0x07fdf4` | +12 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource7releaseEi`<br>AbyssEngine::AESoundRessource::release(int) |
| 638 | `0x07fe2c` | `0x07fe38` | +12 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource9playMusicEi`<br>AbyssEngine::AESoundRessource::playMusic(int) |
| 639 | `0x07fe88` | `0x07fe94` | +12 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource13playMusicLoopEi`<br>AbyssEngine::AESoundRessource::playMusicLoop(int) |
| 640 | `0x07fee4` | `0x07fef0` | +12 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource4playEi`<br>AbyssEngine::AESoundRessource::play(int) |
| 641 | `0x07feec` | `0x07fef8` | +12 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource4playEif`<br>AbyssEngine::AESoundRessource::play(int, float) |
| 642 | `0x07ff54` | `0x07ff60` | +12 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource8playLoopEi`<br>AbyssEngine::AESoundRessource::playLoop(int) |
| 643 | `0x07ffb0` | `0x07ffbc` | +12 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource4stopEi`<br>AbyssEngine::AESoundRessource::stop(int) |
| 644 | `0x07fff4` | `0x080000` | +12 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource4stopEv`<br>AbyssEngine::AESoundRessource::stop() |
| 645 | `0x080016` | `0x080022` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource12checkLoopingEv`<br>AbyssEngine::AESoundRessource::checkLooping() |
| 646 | `0x080018` | `0x080024` | +12 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource6supendEv`<br>AbyssEngine::AESoundRessource::supend() |
| 647 | `0x08003c` | `0x080048` | +12 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource5pauseEi`<br>AbyssEngine::AESoundRessource::pause(int) |
| 648 | `0x080080` | `0x08008c` | +12 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource5pauseEv`<br>AbyssEngine::AESoundRessource::pause() |
| 649 | `0x0800a4` | `0x0800b0` | +12 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16AESoundRessource6resumeEi`<br>AbyssEngine::AESoundRessource::resume(int) |
| 650 | `0x0800e8` | `0x0800f4` | +12 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16AESoundRessource6resumeEv`<br>AbyssEngine::AESoundRessource::resume() |
| 651 | `0x08010a` | `0x080116` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound9loadSoundEPKc`<br>AbyssEngine::AESound::loadSound(char const*) |
| 652 | `0x08010c` | `0x080118` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound11unloadSoundEv`<br>AbyssEngine::AESound::unloadSound() |
| 653 | `0x08010e` | `0x08011a` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound4playEv`<br>AbyssEngine::AESound::play() |
| 654 | `0x080110` | `0x08011c` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound4playEf`<br>AbyssEngine::AESound::play(float) |
| 655 | `0x080112` | `0x08011e` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound8playLoopEv`<br>AbyssEngine::AESound::playLoop() |
| 656 | `0x080114` | `0x080120` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound5pauseEv`<br>AbyssEngine::AESound::pause() |
| 657 | `0x080116` | `0x080122` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound6resumeEv`<br>AbyssEngine::AESound::resume() |
| 658 | `0x080118` | `0x080124` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound4stopEv`<br>AbyssEngine::AESound::stop() |
| 659 | `0x08011a` | `0x080126` | +12 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound9isPlayingEv`<br>AbyssEngine::AESound::isPlaying() |
| 660 | `0x08011e` | `0x08012a` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound7setGainEi`<br>AbyssEngine::AESound::setGain(int) |
| 661 | `0x080120` | `0x08012c` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound9setVolumeEi`<br>AbyssEngine::AESound::setVolume(int) |
| 662 | `0x080122` | `0x08012e` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound14setSoundVolumeEi`<br>AbyssEngine::AESound::setSoundVolume(int) |
| 663 | `0x080124` | `0x080130` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound14setMusicVolumeEi`<br>AbyssEngine::AESound::setMusicVolume(int) |
| 664 | `0x080126` | `0x080132` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound7releaseEv`<br>AbyssEngine::AESound::release() |
| 665 | `0x080128` | `0x080134` | +12 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine7AESound6loadedEv`<br>AbyssEngine::AESound::loaded() |
| 666 | `0x08012c` | `0x080138` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath6MatrixcvPfEv`<br>AbyssEngine::AEMath::Matrix::operator float*() |
| 667 | `0x08012e` | `0x08013a` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine6AEMath6MatrixcvPKfEv`<br>AbyssEngine::AEMath::Matrix::operator float const*() const |
| 668 | `0x080130` | `0x08013c` | +12 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath6MatrixaSERKS1_`<br>AbyssEngine::AEMath::Matrix::operator=(AbyssEngine::AEMath::Matrix const&) |
| 669 | `0x08016a` | `0x080176` | +12 | 350 | 350 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath6MatrixmLERKS1_`<br>AbyssEngine::AEMath::Matrix::operator*=(AbyssEngine::AEMath::Matrix const&) |
| 670 | `0x0802c8` | `0x0802d4` | +12 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMathmlERKNS0_6MatrixES3_`<br>AbyssEngine::AEMath::operator*(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&) |
| 671 | `0x080448` | `0x080454` | +12 | 228 | 228 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMatheqERKNS0_6MatrixES3_`<br>AbyssEngine::AEMath::operator==(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&) |
| 672 | `0x08052c` | `0x080538` | +12 | 226 | 226 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathneERKNS0_6MatrixES3_`<br>AbyssEngine::AEMath::operator!=(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&) |
| 673 | `0x08060e` | `0x08061a` | +12 | 122 | 122 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMathmlERKNS0_6MatrixERKNS0_6VectorE`<br>AbyssEngine::AEMath::operator*(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector const&) |
| 674 | `0x080688` | `0x080694` | +12 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath14MatrixMultiplyERKNS0_6MatrixES3_`<br>AbyssEngine::AEMath::MatrixMultiply(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&) |
| 675 | `0x080694` | `0x0806a0` | +12 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath14MatrixIdentityERNS0_6MatrixE`<br>AbyssEngine::AEMath::MatrixIdentity(AbyssEngine::AEMath::Matrix&) |
| 676 | `0x080700` | `0x08070c` | +12 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath13MatrixIsEqualERKNS0_6MatrixES3_`<br>AbyssEngine::AEMath::MatrixIsEqual(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&) |
| 677 | `0x080704` | `0x080710` | +12 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath14MatrixGetRightERKNS0_6MatrixE`<br>AbyssEngine::AEMath::MatrixGetRight(AbyssEngine::AEMath::Matrix const&) |
| 678 | `0x080712` | `0x08071e` | +12 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath11MatrixGetUpERKNS0_6MatrixE`<br>AbyssEngine::AEMath::MatrixGetUp(AbyssEngine::AEMath::Matrix const&) |
| 679 | `0x080720` | `0x08072c` | +12 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath12MatrixGetDirERKNS0_6MatrixE`<br>AbyssEngine::AEMath::MatrixGetDir(AbyssEngine::AEMath::Matrix const&) |
| 680 | `0x08072e` | `0x08073a` | +12 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath17MatrixGetPositionERKNS0_6MatrixE`<br>AbyssEngine::AEMath::MatrixGetPosition(AbyssEngine::AEMath::Matrix const&) |
| 681 | `0x08073c` | `0x080748` | +12 | 122 | 122 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath21MatrixTransformVectorERKNS0_6MatrixERKNS0_6VectorE`<br>AbyssEngine::AEMath::MatrixTransformVector(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector const&) |
| 682 | `0x0807b6` | `0x0807c2` | +12 | 98 | 98 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath18MatrixRotateVectorERKNS0_6MatrixERKNS0_6VectorE`<br>AbyssEngine::AEMath::MatrixRotateVector(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector const&) |
| 683 | `0x080818` | `0x080824` | +12 | 158 | 158 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath28MatrixInverseTransformVectorERKNS0_6MatrixERKNS0_6VectorE`<br>AbyssEngine::AEMath::MatrixInverseTransformVector(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector const&) |
| 684 | `0x0808b6` | `0x0808c2` | +12 | 98 | 98 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath25MatrixInverseRotateVectorERKNS0_6MatrixERKNS0_6VectorE`<br>AbyssEngine::AEMath::MatrixInverseRotateVector(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector const&) |
| 685 | `0x080918` | `0x080924` | +12 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath17MatrixSetRotationERNS0_6MatrixEfff`<br>AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, float, float, float) |
| 686 | `0x0809f0` | `0x0809fc` | +12 | 724 | 724 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath17MatrixSetRotationERNS0_6MatrixEfffNS0_13RotationOrderE`<br>AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, float, float, float, AbyssEngine::AEMath::RotationOrder) |
| 687 | `0x080cc4` | `0x080cd0` | +12 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath17MatrixSetRotationERNS0_6MatrixERKNS0_6VectorE`<br>AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, AbyssEngine::AEMath::Vector const&) |
| 688 | `0x080d40` | `0x080d4c` | +12 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath17MatrixSetRotationERNS0_6MatrixERKNS0_6VectorES5_S5_`<br>AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 689 | `0x080d84` | `0x080d90` | +12 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath16MatrixSetScalingERNS0_6MatrixEfff`<br>AbyssEngine::AEMath::MatrixSetScaling(AbyssEngine::AEMath::Matrix&, float, float, float) |
| 690 | `0x080e48` | `0x080e54` | +12 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath20MatrixSetTranslationERNS0_6MatrixEfff`<br>AbyssEngine::AEMath::MatrixSetTranslation(AbyssEngine::AEMath::Matrix&, float, float, float) |
| 691 | `0x080e60` | `0x080e6c` | +12 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath20MatrixSetTranslationERNS0_6MatrixERKNS0_6VectorE`<br>AbyssEngine::AEMath::MatrixSetTranslation(AbyssEngine::AEMath::Matrix&, AbyssEngine::AEMath::Vector const&) |
| 692 | `0x080e78` | `0x080e84` | +12 | 158 | 158 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath16MatrixGetInverseERKNS0_6MatrixE`<br>AbyssEngine::AEMath::MatrixGetInverse(AbyssEngine::AEMath::Matrix const&) |
| 693 | `0x080f18` | `0x080f24` | +12 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath15MatrixGetLookAtERKNS0_6VectorES3_S3_`<br>AbyssEngine::AEMath::MatrixGetLookAt(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 694 | `0x080fc4` | `0x080fd0` | +12 | 64 | 64 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath11MatrixGetGLERKNS0_6MatrixEPf`<br>AbyssEngine::AEMath::MatrixGetGL(AbyssEngine::AEMath::Matrix const&, float*) |
| 695 | `0x081004` | `0x081010` | +12 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath14MatrixDebugOutERKNS0_6MatrixE`<br>AbyssEngine::AEMath::MatrixDebugOut(AbyssEngine::AEMath::Matrix const&) |
| 696 | `0x081008` | `0x081012` | +10 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionC1Ev`<br>AbyssEngine::Quaternion::Quaternion() |
| 697 | `0x081008` | `0x081012` | +10 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionC2Ev`<br>AbyssEngine::Quaternion::Quaternion() |
| 698 | `0x08100a` | `0x081014` | +10 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionC1Effff`<br>AbyssEngine::Quaternion::Quaternion(float, float, float, float) |
| 699 | `0x08100a` | `0x081014` | +10 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionC2Effff`<br>AbyssEngine::Quaternion::Quaternion(float, float, float, float) |
| 700 | `0x081018` | `0x081022` | +10 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10QuaternionC1ENS_6AEMath6VectorE`<br>AbyssEngine::Quaternion::Quaternion(AbyssEngine::AEMath::Vector) |
| 701 | `0x081018` | `0x081022` | +10 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10QuaternionC2ENS_6AEMath6VectorE`<br>AbyssEngine::Quaternion::Quaternion(AbyssEngine::AEMath::Vector) |
| 702 | `0x081026` | `0x081030` | +10 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10Quaternion3SetENS_6AEMath6VectorE`<br>AbyssEngine::Quaternion::Set(AbyssEngine::AEMath::Vector) |
| 703 | `0x08102a` | `0x081034` | +10 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionC2EPS0_`<br>AbyssEngine::Quaternion::Quaternion(AbyssEngine::Quaternion*) |
| 704 | `0x08102a` | `0x081034` | +10 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionC1EPS0_`<br>AbyssEngine::Quaternion::Quaternion(AbyssEngine::Quaternion*) |
| 705 | `0x08102c` | `0x081036` | +10 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10QuaternionC1ERKNS_6AEMath6MatrixE`<br>AbyssEngine::Quaternion::Quaternion(AbyssEngine::AEMath::Matrix const&) |
| 706 | `0x08102c` | `0x081036` | +10 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10QuaternionC2ERKNS_6AEMath6MatrixE`<br>AbyssEngine::Quaternion::Quaternion(AbyssEngine::AEMath::Matrix const&) |
| 707 | `0x08103c` | `0x081044` | +8 | 532 | 532 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10Quaternion3SetERKNS_6AEMath6MatrixE`<br>AbyssEngine::Quaternion::Set(AbyssEngine::AEMath::Matrix const&) |
| 708 | `0x081250` | `0x081258` | +8 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionD2Ev`<br>AbyssEngine::Quaternion::~Quaternion() |
| 709 | `0x081250` | `0x081258` | +8 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionD1Ev`<br>AbyssEngine::Quaternion::~Quaternion() |
| 710 | `0x081254` | `0x08125c` | +8 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10Quaternion3SetEfff`<br>AbyssEngine::Quaternion::Set(float, float, float) |
| 711 | `0x081328` | `0x081330` | +8 | 74 | 74 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine10Quaternion7InverseEv`<br>AbyssEngine::Quaternion::Inverse() const |
| 712 | `0x081372` | `0x08137a` | +8 | 54 | 54 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10Quaternion3DotERKS0_S2_`<br>AbyssEngine::Quaternion::Dot(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&) |
| 713 | `0x0813a8` | `0x0813b0` | +8 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10Quaternion4LerpERKS0_S2_f`<br>AbyssEngine::Quaternion::Lerp(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&, float) |
| 714 | `0x081410` | `0x081418` | +8 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10Quaternion10NormalizedEv`<br>AbyssEngine::Quaternion::Normalized() |
| 715 | `0x08145c` | `0x081464` | +8 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10Quaternion4LerpEPKfS2_f`<br>AbyssEngine::Quaternion::Lerp(float const*, float const*, float) |
| 716 | `0x0814d4` | `0x0814dc` | +8 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternioncvPfEv`<br>AbyssEngine::Quaternion::operator float*() |
| 717 | `0x0814d6` | `0x0814de` | +8 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine10QuaternioncvPKfEv`<br>AbyssEngine::Quaternion::operator float const*() const |
| 718 | `0x0814d8` | `0x0814e0` | +8 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10QuaternionixEi`<br>AbyssEngine::Quaternion::operator[](int) |
| 719 | `0x0814de` | `0x0814e6` | +8 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine10QuaternionixEi`<br>AbyssEngine::Quaternion::operator[](int) const |
| 720 | `0x0814e4` | `0x0814ec` | +8 | 60 | 60 | 0 | 100.0% |  | Y | `_ZNK11AbyssEngine10Quaternion6LengthEv`<br>AbyssEngine::Quaternion::Length() const |
| 721 | `0x081520` | `0x081528` | +8 | 314 | 314 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine10Quaternion7ConvertERNS_6AEMath6MatrixE`<br>AbyssEngine::Quaternion::Convert(AbyssEngine::AEMath::Matrix&) |
| 722 | `0x08165c` | `0x081664` | +8 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN8GameTextC1Ev`<br>GameText::GameText() |
| 723 | `0x08165c` | `0x081664` | +8 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN8GameTextC2Ev`<br>GameText::GameText() |
| 724 | `0x0816f4` | `0x0816fc` | +8 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIiEC2Ev`<br>Array<int>::Array() |
| 725 | `0x081710` | `0x081718` | +8 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIiED2Ev`<br>Array<int>::~Array() |
| 726 | `0x081726` | `0x08172e` | +8 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN8GameTextD2Ev`<br>GameText::~GameText() |
| 727 | `0x081726` | `0x08172e` | +8 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN8GameTextD1Ev`<br>GameText::~GameText() |
| 728 | `0x08174e` | `0x081756` | +8 | 66 | 66 | 0 | 100.0% | Y | Y | `_ZN8GameText7releaseEv`<br>GameText::release() |
| 729 | `0x081790` | `0x081798` | +8 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN8GameText11getLanguageEv`<br>GameText::getLanguage() |
| 730 | `0x0817a0` | `0x0817a8` | +8 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN8GameText18setSubstituteArrayEPij`<br>GameText::setSubstituteArray(int*, unsigned int) |
| 731 | `0x0817d0` | `0x0817d8` | +8 | 34 | 34 | 0 | 100.0% |  | Y | `_Z14ArrayRemoveAllIiEvR5ArrayIT_E`<br>void ArrayRemoveAll<int>(Array<int>&) |
| 732 | `0x0817f2` | `0x0817fa` | +8 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIiEvT_R5ArrayIS0_E`<br>void ArrayAdd<int>(int, Array<int>&) |
| 733 | `0x081818` | `0x08184c` | +52 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN8GameText7getTextEi`<br>GameText::getText(int) |
| 734 | `0x08193c` | `0x081970` | +52 | 1668 | 1668 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManagerC1EPNS_6EngineE`<br>AbyssEngine::ApplicationManager::ApplicationManager(AbyssEngine::Engine*) |
| 735 | `0x08193c` | `0x081970` | +52 | 1668 | 1668 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManagerC2EPNS_6EngineE`<br>AbyssEngine::ApplicationManager::ApplicationManager(AbyssEngine::Engine*) |
| 736 | `0x081fc0` | `0x081ff4` | +52 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine18IApplicationModuleEEC2Ev`<br>Array<AbyssEngine::IApplicationModule*>::Array() |
| 737 | `0x081fdc` | `0x082010` | +52 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN5ArrayIxEC2Ev`<br>Array<long long>::Array() |
| 738 | `0x081ffa` | `0x08202e` | +52 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine7KeyCodeaSERKS0_`<br>AbyssEngine::KeyCode::operator=(AbyssEngine::KeyCode const&) |
| 739 | `0x08200c` | `0x082040` | +52 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIxED2Ev`<br>Array<long long>::~Array() |
| 740 | `0x082022` | `0x082056` | +52 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine18IApplicationModuleEED2Ev`<br>Array<AbyssEngine::IApplicationModule*>::~Array() |
| 741 | `0x082038` | `0x08206c` | +52 | 252 | 252 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManagerD1Ev`<br>AbyssEngine::ApplicationManager::~ApplicationManager() |
| 742 | `0x082038` | `0x08206c` | +52 | 252 | 252 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManagerD2Ev`<br>AbyssEngine::ApplicationManager::~ApplicationManager() |
| 743 | `0x082134` | `0x082168` | +52 | 20 | 20 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIPN11AbyssEngine18IApplicationModuleEEvR5ArrayIT_E`<br>void ArrayRelease<AbyssEngine::IApplicationModule*>(Array<AbyssEngine::IApplicationModule*>&) |
| 744 | `0x082148` | `0x08217c` | +52 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager8SoundSetEPKNS_11AESoundInfoEi`<br>AbyssEngine::ApplicationManager::SoundSet(AbyssEngine::AESoundInfo const*, int) |
| 745 | `0x08217c` | `0x0821b0` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager12SoundReleaseEi`<br>AbyssEngine::ApplicationManager::SoundRelease(int) |
| 746 | `0x08218a` | `0x0821be` | +52 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager14SoundPlayMusicEi`<br>AbyssEngine::ApplicationManager::SoundPlayMusic(int) |
| 747 | `0x08219e` | `0x0821d2` | +52 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager18SoundPlayMusicLoopEi`<br>AbyssEngine::ApplicationManager::SoundPlayMusicLoop(int) |
| 748 | `0x0821b2` | `0x0821e6` | +52 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager9SoundPlayEi`<br>AbyssEngine::ApplicationManager::SoundPlay(int) |
| 749 | `0x0821c6` | `0x0821fa` | +52 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager9SoundPlayEif`<br>AbyssEngine::ApplicationManager::SoundPlay(int, float) |
| 750 | `0x0821da` | `0x08220e` | +52 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager13SoundPlayLoopEi`<br>AbyssEngine::ApplicationManager::SoundPlayLoop(int) |
| 751 | `0x0821ee` | `0x082222` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager9SoundStopEi`<br>AbyssEngine::ApplicationManager::SoundStop(int) |
| 752 | `0x0821fc` | `0x082230` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager15SoundStopSoundsEv`<br>AbyssEngine::ApplicationManager::SoundStopSounds() |
| 753 | `0x08220a` | `0x08223e` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager16SoundPauseSoundsEv`<br>AbyssEngine::ApplicationManager::SoundPauseSounds() |
| 754 | `0x082218` | `0x08224c` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager10SoundPauseEi`<br>AbyssEngine::ApplicationManager::SoundPause(int) |
| 755 | `0x082226` | `0x08225a` | +52 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager11SoundResumeEi`<br>AbyssEngine::ApplicationManager::SoundResume(int) |
| 756 | `0x082240` | `0x082274` | +52 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager17SoundResumeSoundsEv`<br>AbyssEngine::ApplicationManager::SoundResumeSounds() |
| 757 | `0x08225a` | `0x08228e` | +52 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager11SoundEnableEb`<br>AbyssEngine::ApplicationManager::SoundEnable(bool) |
| 758 | `0x082264` | `0x082298` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager13SoundFxEnableEb`<br>AbyssEngine::ApplicationManager::SoundFxEnable(bool) |
| 759 | `0x08226a` | `0x08229e` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager16SoundMusicEnableEb`<br>AbyssEngine::ApplicationManager::SoundMusicEnable(bool) |
| 760 | `0x082270` | `0x0822a4` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager14SoundSetVolumeEii`<br>AbyssEngine::ApplicationManager::SoundSetVolume(int, int) |
| 761 | `0x08227e` | `0x0822b2` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager16SoundSetFXVolumeEi`<br>AbyssEngine::ApplicationManager::SoundSetFXVolume(int) |
| 762 | `0x08228c` | `0x0822c0` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager19SoundSetMusicVolumeEi`<br>AbyssEngine::ApplicationManager::SoundSetMusicVolume(int) |
| 763 | `0x08229a` | `0x0822ce` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager14SoundIsPlayingEi`<br>AbyssEngine::ApplicationManager::SoundIsPlaying(int) |
| 764 | `0x0822a8` | `0x0822dc` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager11SoundResumeEv`<br>AbyssEngine::ApplicationManager::SoundResume() |
| 765 | `0x0822b6` | `0x0822ea` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager13VibrateEnableEb`<br>AbyssEngine::ApplicationManager::VibrateEnable(bool) |
| 766 | `0x0822bc` | `0x0822f0` | +52 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager16VibrateSupportedEv`<br>AbyssEngine::ApplicationManager::VibrateSupported() |
| 767 | `0x0822c4` | `0x0822f8` | +52 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager7VibrateEt`<br>AbyssEngine::ApplicationManager::Vibrate(unsigned short) |
| 768 | `0x0822d4` | `0x082308` | +52 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager25RegisterApplicationModuleEjPNS_18IApplicationModuleE`<br>AbyssEngine::ApplicationManager::RegisterApplicationModule(unsigned int, AbyssEngine::IApplicationModule*) |
| 769 | `0x08230e` | `0x082342` | +52 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine18IApplicationModuleEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::IApplicationModule*>(AbyssEngine::IApplicationModule*, Array<AbyssEngine::IApplicationModule*>&) |
| 770 | `0x082334` | `0x082368` | +52 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager21EnablePerformanceTestEi`<br>AbyssEngine::ApplicationManager::EnablePerformanceTest(int) |
| 771 | `0x0823b0` | `0x0823e4` | +52 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager27SetCurrentApplicationModuleEj`<br>AbyssEngine::ApplicationManager::SetCurrentApplicationModule(unsigned int) |
| 772 | `0x082440` | `0x082474` | +52 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager20SetApplicationModuleEPNS_18IApplicationModuleE`<br>AbyssEngine::ApplicationManager::SetApplicationModule(AbyssEngine::IApplicationModule*) |
| 773 | `0x08244e` | `0x082482` | +52 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZNK11AbyssEngine18ApplicationManager27GetCurrentApplicationModuleEv`<br>AbyssEngine::ApplicationManager::GetCurrentApplicationModule() const |
| 774 | `0x082452` | `0x082486` | +52 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager20GetApplicationModuleEj`<br>AbyssEngine::ApplicationManager::GetApplicationModule(unsigned int) |
| 775 | `0x082476` | `0x0824aa` | +52 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager4QuitEv`<br>AbyssEngine::ApplicationManager::Quit() |
| 776 | `0x082480` | `0x0824b4` | +52 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager27GetApplicationVersionStringEv`<br>AbyssEngine::ApplicationManager::GetApplicationVersionString() |
| 777 | `0x082494` | `0x0824c8` | +52 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager18SetApplicationDataEPv`<br>AbyssEngine::ApplicationManager::SetApplicationData(void*) |
| 778 | `0x082498` | `0x0824cc` | +52 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager18GetApplicationDataEv`<br>AbyssEngine::ApplicationManager::GetApplicationData() |
| 779 | `0x08249c` | `0x0824d0` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager9GetEngineEv`<br>AbyssEngine::ApplicationManager::GetEngine() |
| 780 | `0x0824a2` | `0x0824d6` | +52 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager7SuspendEv`<br>AbyssEngine::ApplicationManager::Suspend() |
| 781 | `0x0824d0` | `0x082504` | +52 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager6ResumeEb`<br>AbyssEngine::ApplicationManager::Resume(bool) |
| 782 | `0x082502` | `0x082536` | +52 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager15SetExitCallbackEPFvvE`<br>AbyssEngine::ApplicationManager::SetExitCallback(void (*)()) |
| 783 | `0x082506` | `0x08253a` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager18SetLoadingCallbackEPFvPNS_11PaintCanvasEiPvES3_`<br>AbyssEngine::ApplicationManager::SetLoadingCallback(void (*)(AbyssEngine::PaintCanvas*, int, void*), void*) |
| 784 | `0x08250c` | `0x082540` | +52 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager19LoadingCallbackShowEiPv`<br>AbyssEngine::ApplicationManager::LoadingCallbackShow(int, void*) |
| 785 | `0x082516` | `0x08254a` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager17SetResumeCallbackEPFbPNS_11PaintCanvasEPvES3_`<br>AbyssEngine::ApplicationManager::SetResumeCallback(bool (*)(AbyssEngine::PaintCanvas*, void*), void*) |
| 786 | `0x08251c` | `0x082550` | +52 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager16CheatSetCallbackEPFviPvES1_`<br>AbyssEngine::ApplicationManager::CheatSetCallback(void (*)(int, void*), void*) |
| 787 | `0x082528` | `0x08255c` | +52 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager12CheatAddCodeERKNS_6StringEi`<br>AbyssEngine::ApplicationManager::CheatAddCode(AbyssEngine::String const&, int) |
| 788 | `0x082534` | `0x082568` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager11CheatEnableEb`<br>AbyssEngine::ApplicationManager::CheatEnable(bool) |
| 789 | `0x08253a` | `0x08256e` | +52 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager11CheatUpdateEt`<br>AbyssEngine::ApplicationManager::CheatUpdate(unsigned short) |
| 790 | `0x08254c` | `0x082580` | +52 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager14ConfigReadFileENS_6StringE`<br>AbyssEngine::ApplicationManager::ConfigReadFile(AbyssEngine::String) |
| 791 | `0x0825a4` | `0x0825d8` | +52 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager31ConfigRegisterTokenReadFunctionENS_6StringEPFvPNS_12ConfigReaderEPvES4_`<br>AbyssEngine::ApplicationManager::ConfigRegisterTokenReadFunction(AbyssEngine::String, void (*)(AbyssEngine::ConfigReader*, void*), void*) |
| 792 | `0x082600` | `0x082634` | +52 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager20ConfigRegisterActionExx`<br>AbyssEngine::ApplicationManager::ConfigRegisterAction(long long, long long) |
| 793 | `0x082620` | `0x082654` | +52 | 52 | 52 | 0 | 100.0% |  | Y | `_Z8ArrayAddIxEvT_R5ArrayIS0_E`<br>void ArrayAdd<long long>(long long, Array<long long>&) |
| 794 | `0x082654` | `0x082688` | +52 | 146 | 146 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager22ConfigGetKeysForActionEx`<br>AbyssEngine::ApplicationManager::ConfigGetKeysForAction(long long) |
| 795 | `0x0826e8` | `0x08271c` | +52 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager25CheckForOrientationChangeEv`<br>AbyssEngine::ApplicationManager::CheckForOrientationChange() |
| 796 | `0x0827dc` | `0x082810` | +52 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager20GetElapsedTimeMillisEv`<br>AbyssEngine::ApplicationManager::GetElapsedTimeMillis() |
| 797 | `0x0827f0` | `0x082824` | +52 | 4576 | 4576 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager8OnUpdateEx`<br>AbyssEngine::ApplicationManager::OnUpdate(long long) |
| 798 | `0x0839d0` | `0x083a04` | +52 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStruct13GetShaderNameEv`<br>AbyssEngine::ShaderBaseStruct::GetShaderName() |
| 799 | `0x0839de` | `0x083a12` | +52 | 90 | 90 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager18ConvertTouchCoordsERiS1_`<br>AbyssEngine::ApplicationManager::ConvertTouchCoords(int&, int&) |
| 800 | `0x083a38` | `0x083a6c` | +52 | 376 | 376 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager12OnTouchBeginEiiPv`<br>AbyssEngine::ApplicationManager::OnTouchBegin(int, int, void*) |
| 801 | `0x083bb0` | `0x083be4` | +52 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager11OnTouchMoveEiiPv`<br>AbyssEngine::ApplicationManager::OnTouchMove(int, int, void*) |
| 802 | `0x083c18` | `0x083c4c` | +52 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager10OnTouchEndEiiPv`<br>AbyssEngine::ApplicationManager::OnTouchEnd(int, int, void*) |
| 803 | `0x083c90` | `0x083cc4` | +52 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager10OnTouchEndEv`<br>AbyssEngine::ApplicationManager::OnTouchEnd() |
| 804 | `0x083ca6` | `0x083cda` | +52 | 242 | 242 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager10OnKeyPressEi`<br>AbyssEngine::ApplicationManager::OnKeyPress(int) |
| 805 | `0x083d98` | `0x083dcc` | +52 | 236 | 236 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager12OnKeyReleaseEi`<br>AbyssEngine::ApplicationManager::OnKeyRelease(int) |
| 806 | `0x083e84` | `0x083eb8` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager11GetKeyStateEv`<br>AbyssEngine::ApplicationManager::GetKeyState() |
| 807 | `0x083e8a` | `0x083ebe` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager14GetActionStateEv`<br>AbyssEngine::ApplicationManager::GetActionState() |
| 808 | `0x083e90` | `0x083ec4` | +52 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager13ResetKeyStateEv`<br>AbyssEngine::ApplicationManager::ResetKeyState() |
| 809 | `0x083e98` | `0x083ecc` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager20GetCurrentTimeMillisEv`<br>AbyssEngine::ApplicationManager::GetCurrentTimeMillis() |
| 810 | `0x083e9e` | `0x083ed2` | +52 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager19GetSystemTimeMillisEv`<br>AbyssEngine::ApplicationManager::GetSystemTimeMillis() |
| 811 | `0x083ea4` | `0x083ed8` | +52 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18ApplicationManager17KeyCodeSetMappingEP5ArrayIPNS_7KeyCodeEE`<br>AbyssEngine::ApplicationManager::KeyCodeSetMapping(Array<AbyssEngine::KeyCode*>*) |
| 812 | `0x083ee2` | `0x083f16` | +52 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18ApplicationManager10CheckCrackEPKc`<br>AbyssEngine::ApplicationManager::CheckCrack(char const*) |
| 813 | `0x083ee8` | `0x083f1a` | +50 | 4 | 4 | 0 | 100.0% |  | Y | `_Z7glErrorv`<br>glError() |
| 814 | `0x083eec` | `0x083f20` | +52 | 572 | 572 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6EngineC1Ev`<br>AbyssEngine::Engine::Engine() |
| 815 | `0x083eec` | `0x083f20` | +52 | 572 | 572 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6EngineC2Ev`<br>AbyssEngine::Engine::Engine() |
| 816 | `0x084128` | `0x08415c` | +52 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine16ShaderBaseStructEEC2Ev`<br>Array<AbyssEngine::ShaderBaseStruct*>::Array() |
| 817 | `0x084144` | `0x084178` | +52 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine16ShaderBaseStructEED2Ev`<br>Array<AbyssEngine::ShaderBaseStruct*>::~Array() |
| 818 | `0x08415c` | `0x084190` | +52 | 228 | 228 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6EngineD2Ev`<br>AbyssEngine::Engine::~Engine() |
| 819 | `0x08415c` | `0x084190` | +52 | 228 | 228 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6EngineD1Ev`<br>AbyssEngine::Engine::~Engine() |
| 820 | `0x084240` | `0x084274` | +52 | 74 | 74 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIPN11AbyssEngine16ShaderBaseStructEEvR5ArrayIT_E`<br>void ArrayReleaseClasses<AbyssEngine::ShaderBaseStruct*>(Array<AbyssEngine::ShaderBaseStruct*>&) |
| 821 | `0x08428c` | `0x0842c0` | +52 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine11AfterGLInitEv`<br>AbyssEngine::Engine::AfterGLInit() |
| 822 | `0x084348` | `0x084380` | +56 | 480 | 480 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine15ResetLightParamEv`<br>AbyssEngine::Engine::ResetLightParam() |
| 823 | `0x084528` | `0x084560` | +56 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine10InitializeEPFvPS0_E`<br>AbyssEngine::Engine::Initialize(void (*)(AbyssEngine::Engine*)) |
| 824 | `0x08452e` | `0x084566` | +56 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine7ReleaseEv`<br>AbyssEngine::Engine::Release() |
| 825 | `0x084530` | `0x084568` | +56 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine15GetDisplayWidthEv`<br>AbyssEngine::Engine::GetDisplayWidth() |
| 826 | `0x084536` | `0x08456e` | +56 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine16GetDisplayHeightEv`<br>AbyssEngine::Engine::GetDisplayHeight() |
| 827 | `0x08453c` | `0x084574` | +56 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine12HasVibrationEv`<br>AbyssEngine::Engine::HasVibration() |
| 828 | `0x084552` | `0x08458a` | +56 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine7VibrateEt`<br>AbyssEngine::Engine::Vibrate(unsigned short) |
| 829 | `0x084558` | `0x084590` | +56 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine11ClearBufferEj`<br>AbyssEngine::Engine::ClearBuffer(unsigned int) |
| 830 | `0x0845d8` | `0x084610` | +56 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine15SetOnDestroyAppEPFvPS0_E`<br>AbyssEngine::Engine::SetOnDestroyApp(void (*)(AbyssEngine::Engine*)) |
| 831 | `0x0845de` | `0x084616` | +56 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine7SuspendEv`<br>AbyssEngine::Engine::Suspend() |
| 832 | `0x0845ee` | `0x084626` | +56 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine6ResumeEv`<br>AbyssEngine::Engine::Resume() |
| 833 | `0x084618` | `0x084650` | +56 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16esMatrixMultiplyEPNS_8ESMatrixES1_S1_`<br>AbyssEngine::esMatrixMultiply(AbyssEngine::ESMatrix*, AbyssEngine::ESMatrix*, AbyssEngine::ESMatrix*) |
| 834 | `0x0846ac` | `0x0846e4` | +56 | 884 | 884 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine10ShaderInitEv`<br>AbyssEngine::Engine::ShaderInit() |
| 835 | `0x084a20` | `0x084a58` | +56 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14ShaderRegisterEPNS_16ShaderBaseStructE`<br>AbyssEngine::Engine::ShaderRegister(AbyssEngine::ShaderBaseStruct*) |
| 836 | `0x084aa0` | `0x084ad8` | +56 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine21SetFrameBufferTextureEii`<br>AbyssEngine::Engine::SetFrameBufferTexture(int, int) |
| 837 | `0x084b04` | `0x084b3c` | +56 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine10SetAddDataEPvi`<br>AbyssEngine::Engine::SetAddData(void*, int) |
| 838 | `0x084b0c` | `0x084b44` | +56 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14SetTexturesExtEjz`<br>AbyssEngine::Engine::SetTexturesExt(unsigned int, ...) |
| 839 | `0x084c44` | `0x084c7c` | +56 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine11SetTexturesEjj`<br>AbyssEngine::Engine::SetTextures(unsigned int, unsigned int) |
| 840 | `0x084d7c` | `0x084db4` | +56 | 640 | 640 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine8GlEnableEjb`<br>AbyssEngine::Engine::GlEnable(unsigned int, bool) |
| 841 | `0x084ffc` | `0x085034` | +56 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14SetTextureSlotEjj`<br>AbyssEngine::Engine::SetTextureSlot(unsigned int, unsigned int) |
| 842 | `0x0850c0` | `0x0850f8` | +56 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14SetEyePositionEfff`<br>AbyssEngine::Engine::SetEyePosition(float, float, float) |
| 843 | `0x0850f0` | `0x085128` | +56 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine11SetUVMatrixERKNS_6AEMath6MatrixE`<br>AbyssEngine::Engine::SetUVMatrix(AbyssEngine::AEMath::Matrix const&) |
| 844 | `0x085184` | `0x0851bc` | +56 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine13ResetUVMatrixEv`<br>AbyssEngine::Engine::ResetUVMatrix() |
| 845 | `0x0851f4` | `0x08522c` | +56 | 408 | 408 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14SetModelMatrixERKNS_6AEMath6MatrixE`<br>AbyssEngine::Engine::SetModelMatrix(AbyssEngine::AEMath::Matrix const&) |
| 846 | `0x08538c` | `0x0853c4` | +56 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine12ShaderUpdateEv`<br>AbyssEngine::Engine::ShaderUpdate() |
| 847 | `0x0853b0` | `0x0853e8` | +56 | 200 | 200 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine18SetWorldViewMatrixERKNS_6AEMath6MatrixE`<br>AbyssEngine::Engine::SetWorldViewMatrix(AbyssEngine::AEMath::Matrix const&) |
| 848 | `0x085478` | `0x0854b0` | +56 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14SetPerspMatrixEPf`<br>AbyssEngine::Engine::SetPerspMatrix(float*) |
| 849 | `0x0854e8` | `0x085520` | +56 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14SetOrthoMatrixEPfS1_b`<br>AbyssEngine::Engine::SetOrthoMatrix(float*, float*, bool) |
| 850 | `0x0855c8` | `0x085600` | +56 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine8SetColorEffff`<br>AbyssEngine::Engine::SetColor(float, float, float, float) |
| 851 | `0x0856b4` | `0x0856ec` | +56 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine26LightSetMaterialColorAlphaEf`<br>AbyssEngine::Engine::LightSetMaterialColorAlpha(float) |
| 852 | `0x08571c` | `0x085754` | +56 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine18LightSetLightCountEi`<br>AbyssEngine::Engine::LightSetLightCount(int) |
| 853 | `0x08572c` | `0x085764` | +56 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine11LightEnableEb`<br>AbyssEngine::Engine::LightEnable(bool) |
| 854 | `0x085788` | `0x0857c0` | +56 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine13LightSetLightEj`<br>AbyssEngine::Engine::LightSetLight(unsigned int) |
| 855 | `0x085828` | `0x085860` | +56 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine23LightSetParticleAmbientEfff`<br>AbyssEngine::Engine::LightSetParticleAmbient(float, float, float) |
| 856 | `0x085830` | `0x085868` | +56 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine31LightSetGlobalSceneColorAmbientEfff`<br>AbyssEngine::Engine::LightSetGlobalSceneColorAmbient(float, float, float) |
| 857 | `0x0858d0` | `0x085908` | +56 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine22LightSetLightDirectionEfffj`<br>AbyssEngine::Engine::LightSetLightDirection(float, float, float, unsigned int) |
| 858 | `0x08595c` | `0x085994` | +56 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine21LightSetLightPositionEfffj`<br>AbyssEngine::Engine::LightSetLightPosition(float, float, float, unsigned int) |
| 859 | `0x0859e0` | `0x085a18` | +56 | 180 | 180 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine25LightSetLightColorDiffuseEfffj`<br>AbyssEngine::Engine::LightSetLightColorDiffuse(float, float, float, unsigned int) |
| 860 | `0x085a94` | `0x085acc` | +56 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine25LightSetLightColorAmbientEfffj`<br>AbyssEngine::Engine::LightSetLightColorAmbient(float, float, float, unsigned int) |
| 861 | `0x085b60` | `0x085b98` | +56 | 180 | 180 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine26LightSetLightColorSpecularEfffj`<br>AbyssEngine::Engine::LightSetLightColorSpecular(float, float, float, unsigned int) |
| 862 | `0x085c14` | `0x085c4c` | +56 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine28LightSetMaterialColorDiffuseEfff`<br>AbyssEngine::Engine::LightSetMaterialColorDiffuse(float, float, float) |
| 863 | `0x085ca0` | `0x085cd8` | +56 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine28LightSetMaterialColorAmbientEfff`<br>AbyssEngine::Engine::LightSetMaterialColorAmbient(float, float, float) |
| 864 | `0x085d44` | `0x085d7c` | +56 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine29LightSetMaterialColorSpecularEfff`<br>AbyssEngine::Engine::LightSetMaterialColorSpecular(float, float, float) |
| 865 | `0x085dd0` | `0x085e08` | +56 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine16LightSetRimColorEfff`<br>AbyssEngine::Engine::LightSetRimColor(float, float, float) |
| 866 | `0x085e04` | `0x085e3c` | +56 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine30LightSetMaterialColorShininessEf`<br>AbyssEngine::Engine::LightSetMaterialColorShininess(float) |
| 867 | `0x085e28` | `0x085e60` | +56 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine13SetFXMaterialENS_9BlendModeE`<br>AbyssEngine::SetFXMaterial(AbyssEngine::BlendMode) |
| 868 | `0x085e2c` | `0x085e64` | +56 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine10DrawLine2DEPfib`<br>AbyssEngine::Engine::DrawLine2D(float*, int, bool) |
| 869 | `0x085e60` | `0x085e98` | +56 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine15ShaderSetActiveEiPNS_4MeshE`<br>AbyssEngine::Engine::ShaderSetActive(int, AbyssEngine::Mesh*) |
| 870 | `0x085f24` | `0x085f5c` | +56 | 1040 | 1040 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine10RenderMeshEPNS_4MeshE`<br>AbyssEngine::Engine::RenderMesh(AbyssEngine::Mesh*) |
| 871 | `0x086334` | `0x08636c` | +56 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine17ShaderSetInActiveEv`<br>AbyssEngine::Engine::ShaderSetInActive() |
| 872 | `0x086350` | `0x086388` | +56 | 258 | 258 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine13AEClientStateEjb`<br>AbyssEngine::Engine::AEClientState(unsigned int, bool) |
| 873 | `0x086452` | `0x08648a` | +56 | 110 | 110 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine20IsExtensionSupportedEPKc`<br>AbyssEngine::Engine::IsExtensionSupported(char const*) |
| 874 | `0x0864c0` | `0x0864f8` | +56 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine12GetGravValueEv`<br>AbyssEngine::Engine::GetGravValue() |
| 875 | `0x086510` | `0x086548` | +56 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine13GetAccelValueEv`<br>AbyssEngine::Engine::GetAccelValue() |
| 876 | `0x086560` | `0x086598` | +56 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine12SetGravValueEddd`<br>AbyssEngine::Engine::SetGravValue(double, double, double) |
| 877 | `0x086582` | `0x0865ba` | +56 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine13SetAccelValueEddd`<br>AbyssEngine::Engine::SetAccelValue(double, double, double) |
| 878 | `0x0865a4` | `0x0865dc` | +56 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine16ShaderBaseStructEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::ShaderBaseStruct*>(AbyssEngine::ShaderBaseStruct*, Array<AbyssEngine::ShaderBaseStruct*>&) |
| 879 | `0x0865c8` | `0x086600` | +56 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine13ReloadShadersEv`<br>AbyssEngine::Engine::ReloadShaders() |
| 880 | `0x0865fc` | `0x086634` | +56 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine13SetPostEffectEjb`<br>AbyssEngine::Engine::SetPostEffect(unsigned int, bool) |
| 881 | `0x08675c` | `0x086794` | +56 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine12DoPostEffectEv`<br>AbyssEngine::Engine::DoPostEffect() |
| 882 | `0x0868b0` | `0x0868e8` | +56 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine12DrawCloakFBOEPNS_12FBOContainerE`<br>AbyssEngine::Engine::DrawCloakFBO(AbyssEngine::FBOContainer*) |
| 883 | `0x0868dc` | `0x086914` | +56 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine21IsPostEffectActivatedEv`<br>AbyssEngine::Engine::IsPostEffectActivated() |
| 884 | `0x0868e8` | `0x086920` | +56 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6Engine18IsRefractActivatedEv`<br>AbyssEngine::Engine::IsRefractActivated() |
| 885 | `0x0868f0` | `0x086928` | +56 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine8DrawQuadEiiii`<br>AbyssEngine::Engine::DrawQuad(int, int, int, int) |
| 886 | `0x086988` | `0x0869c0` | +56 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine13GlowBeginGlowEi`<br>AbyssEngine::Engine::GlowBeginGlow(int) |
| 887 | `0x0869c8` | `0x086a00` | +56 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine14GlowEnableGlowEv`<br>AbyssEngine::Engine::GlowEnableGlow() |
| 888 | `0x0869f4` | `0x086a2c` | +56 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6Engine11GlowEndGlowEv`<br>AbyssEngine::Engine::GlowEndGlow() |
| 889 | `0x086a28` | `0x086a60` | +56 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath5SqrtfEf`<br>AbyssEngine::AEMath::Sqrtf(float) |
| 890 | `0x086a44` | `0x086a7c` | +56 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath4SinfEf`<br>AbyssEngine::AEMath::Sinf(float) |
| 891 | `0x086a48` | `0x086a80` | +56 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath4CosfEf`<br>AbyssEngine::AEMath::Cosf(float) |
| 892 | `0x086a4c` | `0x086a84` | +56 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath5ACosfEf`<br>AbyssEngine::AEMath::ACosf(float) |
| 893 | `0x086a50` | `0x086a88` | +56 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath5ATanfEf`<br>AbyssEngine::AEMath::ATanf(float) |
| 894 | `0x086a54` | `0x086a8c` | +56 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath4AbsfEf`<br>AbyssEngine::AEMath::Absf(float) |
| 895 | `0x086a70` | `0x086aa8` | +56 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath3MaxEff`<br>AbyssEngine::AEMath::Max(float, float) |
| 896 | `0x086a8c` | `0x086ac4` | +56 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath3MinEff`<br>AbyssEngine::AEMath::Min(float, float) |
| 897 | `0x086aa8` | `0x086ae0` | +56 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine6AEMath3PowEff`<br>AbyssEngine::AEMath::Pow(float, float) |
| 898 | `0x086aac` | `0x086ae4` | +56 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine6AEMath7InvSqrtEf`<br>AbyssEngine::AEMath::InvSqrt(float) |
| 899 | `0x086adc` | `0x086b14` | +56 | 400 | 400 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18SpriteSystemCreateEPNS_6EngineEtbPPNS_12SpriteSystemE`<br>AbyssEngine::SpriteSystemCreate(AbyssEngine::Engine*, unsigned short, bool, AbyssEngine::SpriteSystem**) |
| 900 | `0x086c6c` | `0x086ca4` | +56 | 102 | 102 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine20SpriteSystemSetAllUvEffffPNS_12SpriteSystemE`<br>AbyssEngine::SpriteSystemSetAllUv(float, float, float, float, AbyssEngine::SpriteSystem*) |
| 901 | `0x086cd4` | `0x086d0c` | +56 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17SpriteSystemSetUvEtffffPNS_12SpriteSystemE`<br>AbyssEngine::SpriteSystemSetUv(unsigned short, float, float, float, float, AbyssEngine::SpriteSystem*) |
| 902 | `0x086d44` | `0x086d7c` | +56 | 116 | 116 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine19SpriteSystemSetRGBAEtffffPNS_12SpriteSystemE`<br>AbyssEngine::SpriteSystemSetRGBA(unsigned short, float, float, float, float, AbyssEngine::SpriteSystem*) |
| 903 | `0x086db8` | `0x086df0` | +56 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine22SpriteSystemSetAllSizeEsPNS_12SpriteSystemE`<br>AbyssEngine::SpriteSystemSetAllSize(short, AbyssEngine::SpriteSystem*) |
| 904 | `0x086ddc` | `0x086e14` | +56 | 648 | 648 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16SpriteSystemDrawEPNS_6EngineERKNS_6AEMath6MatrixES5_PNS_12SpriteSystemE`<br>AbyssEngine::SpriteSystemDraw(AbyssEngine::Engine*, AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&, AbyssEngine::SpriteSystem*) |
| 905 | `0x087064` | `0x08709c` | +56 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19SpriteSystemReleaseEPNS_6EngineEPPNS_12SpriteSystemE`<br>AbyssEngine::SpriteSystemRelease(AbyssEngine::Engine*, AbyssEngine::SpriteSystem**) |
| 906 | `0x0870b8` | `0x0870f0` | +56 | 416 | 416 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12MaterialDrawEPNS_11PaintCanvasEPNS_6EngineEPNS_8MaterialEb`<br>AbyssEngine::MaterialDraw(AbyssEngine::PaintCanvas*, AbyssEngine::Engine*, AbyssEngine::Material*, bool) |
| 907 | `0x087258` | `0x087290` | +56 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ConfigReaderC1EPNS_6EngineE`<br>AbyssEngine::ConfigReader::ConfigReader(AbyssEngine::Engine*) |
| 908 | `0x087258` | `0x087290` | +56 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ConfigReaderC2EPNS_6EngineE`<br>AbyssEngine::ConfigReader::ConfigReader(AbyssEngine::Engine*) |
| 909 | `0x087330` | `0x087368` | +56 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine11TokenStructEEC2Ev`<br>Array<AbyssEngine::TokenStruct*>::Array() |
| 910 | `0x08734c` | `0x087384` | +56 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ConfigReader25RegisterTokenReadFunctionENS_6StringEPFvPS0_PvES3_`<br>AbyssEngine::ConfigReader::RegisterTokenReadFunction(AbyssEngine::String, void (*)(AbyssEngine::ConfigReader*, void*), void*) |
| 911 | `0x08765c` | `0x08768c` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine11TokenStructEED2Ev`<br>Array<AbyssEngine::TokenStruct*>::~Array() |
| 912 | `0x087672` | `0x0876a2` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ConfigReaderD2Ev`<br>AbyssEngine::ConfigReader::~ConfigReader() |
| 913 | `0x087672` | `0x0876a2` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ConfigReaderD1Ev`<br>AbyssEngine::ConfigReader::~ConfigReader() |
| 914 | `0x0876bc` | `0x0876ec` | +48 | 280 | 280 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ConfigReader9ParseFileENS_6StringE`<br>AbyssEngine::ConfigReader::ParseFile(AbyssEngine::String) |
| 915 | `0x0877d4` | `0x087804` | +48 | 276 | 276 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ConfigReader10GetNewLineEv`<br>AbyssEngine::ConfigReader::GetNewLine() |
| 916 | `0x0878e8` | `0x087918` | +48 | 36 | 36 | 0 | 100.0% | Y | Y | `_Z8ArrayAddIPN11AbyssEngine11TokenStructEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::TokenStruct*>(AbyssEngine::TokenStruct*, Array<AbyssEngine::TokenStruct*>&) |
| 917 | `0x08790c` | `0x08793c` | +48 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN5ArrayIPN11AbyssEngine7KeyCodeEEC2Ev`<br>Array<AbyssEngine::KeyCode*>::Array() |
| 918 | `0x087928` | `0x087958` | +48 | 36 | 36 | 0 | 100.0% | Y | Y | `_Z8ArrayAddIPN11AbyssEngine7KeyCodeEEvT_R5ArrayIS3_E`<br>void ArrayAdd<AbyssEngine::KeyCode*>(AbyssEngine::KeyCode*, Array<AbyssEngine::KeyCode*>&) |
| 919 | `0x08794c` | `0x08797c` | +48 | 34 | 34 | 0 | 100.0% | Y | Y | `_Z14ArrayRemoveAllIPN11AbyssEngine7KeyCodeEEvR5ArrayIT_E`<br>void ArrayRemoveAll<AbyssEngine::KeyCode*>(Array<AbyssEngine::KeyCode*>&) |
| 920 | `0x08796e` | `0x08799e` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine7KeyCodeEED2Ev`<br>Array<AbyssEngine::KeyCode*>::~Array() |
| 921 | `0x087984` | `0x0879b4` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20SpecCubeAlphaMappingC2Ev`<br>AbyssEngine::SpecCubeAlphaMapping::SpecCubeAlphaMapping() |
| 922 | `0x087984` | `0x0879b4` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20SpecCubeAlphaMappingC1Ev`<br>AbyssEngine::SpecCubeAlphaMapping::SpecCubeAlphaMapping() |
| 923 | `0x087a10` | `0x087a40` | +48 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20SpecCubeAlphaMapping4InitEPNS_6EngineE`<br>AbyssEngine::SpecCubeAlphaMapping::Init(AbyssEngine::Engine*) |
| 924 | `0x087b64` | `0x087b94` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20SpecCubeAlphaMapping11SetInActiveEv`<br>AbyssEngine::SpecCubeAlphaMapping::SetInActive() |
| 925 | `0x087b88` | `0x087bb8` | +48 | 404 | 404 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20SpecCubeAlphaMapping14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::SpecCubeAlphaMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 926 | `0x087d1c` | `0x087d4c` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine20SpecCubeAlphaMappingD0Ev`<br>AbyssEngine::SpecCubeAlphaMapping::~SpecCubeAlphaMapping() |
| 927 | `0x087d2c` | `0x087d5c` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ShaderBaseStruct12RenderEffectEPNS_12FBOContainerEPNS_6EngineE`<br>AbyssEngine::ShaderBaseStruct::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) |
| 928 | `0x087d2e` | `0x087d5e` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ShaderBaseStruct12RenderEffectEPNS_12FBOContainerEPNS_6EngineEfNS_6AEMath6VectorE`<br>AbyssEngine::ShaderBaseStruct::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*, float, AbyssEngine::AEMath::Vector) |
| 929 | `0x087d30` | `0x087d60` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ShaderBaseStruct12RenderEffectEPNS_12FBOContainerERS2_PNS_6EngineE`<br>AbyssEngine::ShaderBaseStruct::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&, AbyssEngine::Engine*) |
| 930 | `0x087d32` | `0x087d62` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ShaderBaseStruct12RenderEffectEPNS_12FBOContainerERS2_PNS_6EngineEfNS_6AEMath6VectorE`<br>AbyssEngine::ShaderBaseStruct::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&, AbyssEngine::Engine*, float, AbyssEngine::AEMath::Vector) |
| 931 | `0x087d34` | `0x087d64` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18BumpShaderParticleC2Ev`<br>AbyssEngine::BumpShaderParticle::BumpShaderParticle() |
| 932 | `0x087d34` | `0x087d64` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18BumpShaderParticleC1Ev`<br>AbyssEngine::BumpShaderParticle::BumpShaderParticle() |
| 933 | `0x087dc0` | `0x087df0` | +48 | 388 | 388 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18BumpShaderParticle4InitEPNS_6EngineE`<br>AbyssEngine::BumpShaderParticle::Init(AbyssEngine::Engine*) |
| 934 | `0x087f44` | `0x087f74` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18BumpShaderParticle11SetInActiveEv`<br>AbyssEngine::BumpShaderParticle::SetInActive() |
| 935 | `0x087f8c` | `0x087fbc` | +48 | 632 | 632 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18BumpShaderParticle14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpShaderParticle::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 936 | `0x088204` | `0x088234` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18BumpShaderParticleD0Ev`<br>AbyssEngine::BumpShaderParticle::~BumpShaderParticle() |
| 937 | `0x088214` | `0x088244` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11GreenShaderC2Ev`<br>AbyssEngine::GreenShader::GreenShader() |
| 938 | `0x088214` | `0x088244` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11GreenShaderC1Ev`<br>AbyssEngine::GreenShader::GreenShader() |
| 939 | `0x0882a0` | `0x0882d0` | +48 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11GreenShader4InitEPNS_6EngineE`<br>AbyssEngine::GreenShader::Init(AbyssEngine::Engine*) |
| 940 | `0x0883c0` | `0x0883f0` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11GreenShader11SetInActiveEv`<br>AbyssEngine::GreenShader::SetInActive() |
| 941 | `0x0883fe` | `0x08842e` | +48 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11GreenShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::GreenShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 942 | `0x08847e` | `0x0884ae` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11GreenShaderD0Ev`<br>AbyssEngine::GreenShader::~GreenShader() |
| 943 | `0x088490` | `0x0884c0` | +48 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShaderC1Ev`<br>AbyssEngine::DrawFBOShader::DrawFBOShader() |
| 944 | `0x088490` | `0x0884c0` | +48 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShaderC2Ev`<br>AbyssEngine::DrawFBOShader::DrawFBOShader() |
| 945 | `0x0884c0` | `0x0884f0` | +48 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShaderD2Ev`<br>AbyssEngine::DrawFBOShader::~DrawFBOShader() |
| 946 | `0x0884c0` | `0x0884f0` | +48 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShaderD1Ev`<br>AbyssEngine::DrawFBOShader::~DrawFBOShader() |
| 947 | `0x0884c4` | `0x0884f4` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShaderD0Ev`<br>AbyssEngine::DrawFBOShader::~DrawFBOShader() |
| 948 | `0x0884d4` | `0x088504` | +48 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShader4InitEPNS_6EngineE`<br>AbyssEngine::DrawFBOShader::Init(AbyssEngine::Engine*) |
| 949 | `0x088548` | `0x088578` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShader11SetInActiveEv`<br>AbyssEngine::DrawFBOShader::SetInActive() |
| 950 | `0x088560` | `0x088590` | +48 | 552 | 552 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShader12RenderEffectEPNS_12FBOContainerEPNS_6EngineE`<br>AbyssEngine::DrawFBOShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) |
| 951 | `0x088788` | `0x0887b8` | +48 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13DrawFBOShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::DrawFBOShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 952 | `0x088820` | `0x088850` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21BumpRimCubeShader_newC1Ev`<br>AbyssEngine::BumpRimCubeShader_new::BumpRimCubeShader_new() |
| 953 | `0x088820` | `0x088850` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21BumpRimCubeShader_newC2Ev`<br>AbyssEngine::BumpRimCubeShader_new::BumpRimCubeShader_new() |
| 954 | `0x0888ac` | `0x0888dc` | +48 | 620 | 620 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21BumpRimCubeShader_new4InitEPNS_6EngineE`<br>AbyssEngine::BumpRimCubeShader_new::Init(AbyssEngine::Engine*) |
| 955 | `0x088b18` | `0x088b48` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21BumpRimCubeShader_new11SetInActiveEv`<br>AbyssEngine::BumpRimCubeShader_new::SetInActive() |
| 956 | `0x088b58` | `0x088b88` | +48 | 1000 | 1000 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21BumpRimCubeShader_new14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpRimCubeShader_new::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 957 | `0x088f40` | `0x088f70` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21BumpRimCubeShader_newD0Ev`<br>AbyssEngine::BumpRimCubeShader_new::~BumpRimCubeShader_new() |
| 958 | `0x088f50` | `0x088f80` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12SimpleShaderC1Ev`<br>AbyssEngine::SimpleShader::SimpleShader() |
| 959 | `0x088f50` | `0x088f80` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12SimpleShaderC2Ev`<br>AbyssEngine::SimpleShader::SimpleShader() |
| 960 | `0x088fdc` | `0x08900c` | +48 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12SimpleShader4InitEPNS_6EngineE`<br>AbyssEngine::SimpleShader::Init(AbyssEngine::Engine*) |
| 961 | `0x089038` | `0x089068` | +48 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12SimpleShader11SetInActiveEv`<br>AbyssEngine::SimpleShader::SetInActive() |
| 962 | `0x08903e` | `0x08906e` | +48 | 114 | 114 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12SimpleShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::SimpleShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 963 | `0x0890b0` | `0x0890e0` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12SimpleShaderD0Ev`<br>AbyssEngine::SimpleShader::~SimpleShader() |
| 964 | `0x0890c0` | `0x0890f0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShaderC2Ev`<br>AbyssEngine::TextureAlphaTestShader::TextureAlphaTestShader() |
| 965 | `0x0890c0` | `0x0890f0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShaderC1Ev`<br>AbyssEngine::TextureAlphaTestShader::TextureAlphaTestShader() |
| 966 | `0x08914c` | `0x08917c` | +48 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShader9UseShaderEb`<br>AbyssEngine::TextureAlphaTestShader::UseShader(bool) |
| 967 | `0x08916c` | `0x08919c` | +48 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShader23ConnectShaderComponentsEji`<br>AbyssEngine::TextureAlphaTestShader::ConnectShaderComponents(unsigned int, int) |
| 968 | `0x089230` | `0x089260` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShader4InitEPNS_6EngineE`<br>AbyssEngine::TextureAlphaTestShader::Init(AbyssEngine::Engine*) |
| 969 | `0x089278` | `0x0892a8` | +48 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShader11SetInActiveEv`<br>AbyssEngine::TextureAlphaTestShader::SetInActive() |
| 970 | `0x0892a4` | `0x0892d4` | +48 | 296 | 296 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::TextureAlphaTestShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 971 | `0x0893cc` | `0x0893fc` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22TextureAlphaTestShaderD0Ev`<br>AbyssEngine::TextureAlphaTestShader::~TextureAlphaTestShader() |
| 972 | `0x0893dc` | `0x08940c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShaderC1Ev`<br>AbyssEngine::BloomShader::BloomShader() |
| 973 | `0x0893dc` | `0x08940c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShaderC2Ev`<br>AbyssEngine::BloomShader::BloomShader() |
| 974 | `0x089468` | `0x089498` | +48 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShaderD1Ev`<br>AbyssEngine::BloomShader::~BloomShader() |
| 975 | `0x089468` | `0x089498` | +48 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShaderD2Ev`<br>AbyssEngine::BloomShader::~BloomShader() |
| 976 | `0x08947c` | `0x0894ac` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShaderD0Ev`<br>AbyssEngine::BloomShader::~BloomShader() |
| 977 | `0x08948c` | `0x0894bc` | +48 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShader12InternalInitEPNS_6EngineE`<br>AbyssEngine::BloomShader::InternalInit(AbyssEngine::Engine*) |
| 978 | `0x0895ec` | `0x08961c` | +48 | 520 | 520 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShader4InitEPNS_6EngineE`<br>AbyssEngine::BloomShader::Init(AbyssEngine::Engine*) |
| 979 | `0x0897f4` | `0x089824` | +48 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShader11SetInActiveEv`<br>AbyssEngine::BloomShader::SetInActive() |
| 980 | `0x089820` | `0x089850` | +48 | 1208 | 1208 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShader12RenderEffectEPNS_12FBOContainerEPNS_6EngineE`<br>AbyssEngine::BloomShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) |
| 981 | `0x089cd8` | `0x089d08` | +48 | 1152 | 1152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShader12RenderEffectEPNS_12FBOContainerERS2_PNS_6EngineE`<br>AbyssEngine::BloomShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&, AbyssEngine::Engine*) |
| 982 | `0x08a158` | `0x08a188` | +48 | 166 | 166 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BloomShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BloomShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 983 | `0x08a200` | `0x08a230` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12EnergyShieldC1Ev`<br>AbyssEngine::EnergyShield::EnergyShield() |
| 984 | `0x08a200` | `0x08a230` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12EnergyShieldC2Ev`<br>AbyssEngine::EnergyShield::EnergyShield() |
| 985 | `0x08a28c` | `0x08a2bc` | +48 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12EnergyShield4InitEPNS_6EngineE`<br>AbyssEngine::EnergyShield::Init(AbyssEngine::Engine*) |
| 986 | `0x08a3e0` | `0x08a410` | +48 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12EnergyShield11SetInActiveEv`<br>AbyssEngine::EnergyShield::SetInActive() |
| 987 | `0x08a40c` | `0x08a43c` | +48 | 452 | 452 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12EnergyShield14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::EnergyShield::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 988 | `0x08a5d0` | `0x08a600` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12EnergyShieldD0Ev`<br>AbyssEngine::EnergyShield::~EnergyShield() |
| 989 | `0x08a5e0` | `0x08a610` | +48 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShaderC1Ev`<br>AbyssEngine::BlurShader::BlurShader() |
| 990 | `0x08a5e0` | `0x08a610` | +48 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShaderC2Ev`<br>AbyssEngine::BlurShader::BlurShader() |
| 991 | `0x08a678` | `0x08a6a8` | +48 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShaderD2Ev`<br>AbyssEngine::BlurShader::~BlurShader() |
| 992 | `0x08a678` | `0x08a6a8` | +48 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShaderD1Ev`<br>AbyssEngine::BlurShader::~BlurShader() |
| 993 | `0x08a67c` | `0x08a6ac` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShaderD0Ev`<br>AbyssEngine::BlurShader::~BlurShader() |
| 994 | `0x08a68c` | `0x08a6bc` | +48 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShader4InitEPNS_6EngineE`<br>AbyssEngine::BlurShader::Init(AbyssEngine::Engine*) |
| 995 | `0x08a748` | `0x08a778` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShader11SetInActiveEv`<br>AbyssEngine::BlurShader::SetInActive() |
| 996 | `0x08a760` | `0x08a790` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShader12RenderEffectEPNS_12FBOContainerEPNS_6EngineEfNS_6AEMath6VectorE`<br>AbyssEngine::BlurShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*, float, AbyssEngine::AEMath::Vector) |
| 997 | `0x08a7a8` | `0x08a7d8` | +48 | 784 | 784 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShader12RenderEffectEPNS_12FBOContainerERS2_PNS_6EngineEfNS_6AEMath6VectorE`<br>AbyssEngine::BlurShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&, AbyssEngine::Engine*, float, AbyssEngine::AEMath::Vector) |
| 998 | `0x08aab8` | `0x08aae8` | +48 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BlurShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BlurShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 999 | `0x08abf0` | `0x08ac20` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15BumpShaderCloakC1Ev`<br>AbyssEngine::BumpShaderCloak::BumpShaderCloak() |
| 1000 | `0x08abf0` | `0x08ac20` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15BumpShaderCloakC2Ev`<br>AbyssEngine::BumpShaderCloak::BumpShaderCloak() |
| 1001 | `0x08ac7c` | `0x08acac` | +48 | 540 | 540 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15BumpShaderCloak4InitEPNS_6EngineE`<br>AbyssEngine::BumpShaderCloak::Init(AbyssEngine::Engine*) |
| 1002 | `0x08ae98` | `0x08aec8` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15BumpShaderCloak11SetInActiveEv`<br>AbyssEngine::BumpShaderCloak::SetInActive() |
| 1003 | `0x08aed8` | `0x08af08` | +48 | 808 | 808 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15BumpShaderCloak14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpShaderCloak::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1004 | `0x08b200` | `0x08b230` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15BumpShaderCloakD0Ev`<br>AbyssEngine::BumpShaderCloak::~BumpShaderCloak() |
| 1005 | `0x08b210` | `0x08b240` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV4C2Ev`<br>AbyssEngine::BumpShaderV4::BumpShaderV4() |
| 1006 | `0x08b210` | `0x08b240` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV4C1Ev`<br>AbyssEngine::BumpShaderV4::BumpShaderV4() |
| 1007 | `0x08b29c` | `0x08b2cc` | +48 | 324 | 324 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV44InitEPNS_6EngineE`<br>AbyssEngine::BumpShaderV4::Init(AbyssEngine::Engine*) |
| 1008 | `0x08b3e0` | `0x08b410` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV411SetInActiveEv`<br>AbyssEngine::BumpShaderV4::SetInActive() |
| 1009 | `0x08b420` | `0x08b450` | +48 | 516 | 516 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV414UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpShaderV4::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1010 | `0x08b624` | `0x08b654` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV4D0Ev`<br>AbyssEngine::BumpShaderV4::~BumpShaderV4() |
| 1011 | `0x08b634` | `0x08b664` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TexOnlyShaderC1Ev`<br>AbyssEngine::TexOnlyShader::TexOnlyShader() |
| 1012 | `0x08b634` | `0x08b664` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TexOnlyShaderC2Ev`<br>AbyssEngine::TexOnlyShader::TexOnlyShader() |
| 1013 | `0x08b6c0` | `0x08b6f0` | +48 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TexOnlyShader4InitEPNS_6EngineE`<br>AbyssEngine::TexOnlyShader::Init(AbyssEngine::Engine*) |
| 1014 | `0x08b734` | `0x08b764` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TexOnlyShader11SetInActiveEv`<br>AbyssEngine::TexOnlyShader::SetInActive() |
| 1015 | `0x08b74a` | `0x08b77a` | +48 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TexOnlyShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::TexOnlyShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1016 | `0x08b7e2` | `0x08b812` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TexOnlyShaderD0Ev`<br>AbyssEngine::TexOnlyShader::~TexOnlyShader() |
| 1017 | `0x08b7f4` | `0x08b824` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BumpShaderC2Ev`<br>AbyssEngine::BumpShader::BumpShader() |
| 1018 | `0x08b7f4` | `0x08b824` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BumpShaderC1Ev`<br>AbyssEngine::BumpShader::BumpShader() |
| 1019 | `0x08b880` | `0x08b8b0` | +48 | 476 | 476 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BumpShader4InitEPNS_6EngineE`<br>AbyssEngine::BumpShader::Init(AbyssEngine::Engine*) |
| 1020 | `0x08ba5c` | `0x08ba8c` | +48 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BumpShader11SetInActiveEv`<br>AbyssEngine::BumpShader::SetInActive() |
| 1021 | `0x08ba88` | `0x08bab8` | +48 | 644 | 644 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BumpShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1022 | `0x08bd0c` | `0x08bd3c` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10BumpShaderD0Ev`<br>AbyssEngine::BumpShader::~BumpShader() |
| 1023 | `0x08bd1c` | `0x08bd4c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShaderC1Ev`<br>AbyssEngine::TextureVtxColorShader::TextureVtxColorShader() |
| 1024 | `0x08bd1c` | `0x08bd4c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShaderC2Ev`<br>AbyssEngine::TextureVtxColorShader::TextureVtxColorShader() |
| 1025 | `0x08bda8` | `0x08bdd8` | +48 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShader9UseShaderEb`<br>AbyssEngine::TextureVtxColorShader::UseShader(bool) |
| 1026 | `0x08bdc8` | `0x08bdf8` | +48 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShader23ConnectShaderComponentsEji`<br>AbyssEngine::TextureVtxColorShader::ConnectShaderComponents(unsigned int, int) |
| 1027 | `0x08bed0` | `0x08bf00` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShader4InitEPNS_6EngineE`<br>AbyssEngine::TextureVtxColorShader::Init(AbyssEngine::Engine*) |
| 1028 | `0x08bf18` | `0x08bf48` | +48 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShader11SetInActiveEv`<br>AbyssEngine::TextureVtxColorShader::SetInActive() |
| 1029 | `0x08bf4c` | `0x08bf7c` | +48 | 448 | 448 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::TextureVtxColorShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1030 | `0x08c10c` | `0x08c13c` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine21TextureVtxColorShaderD0Ev`<br>AbyssEngine::TextureVtxColorShader::~TextureVtxColorShader() |
| 1031 | `0x08c11c` | `0x08c14c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV3C2Ev`<br>AbyssEngine::BumpShaderV3::BumpShaderV3() |
| 1032 | `0x08c11c` | `0x08c14c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV3C1Ev`<br>AbyssEngine::BumpShaderV3::BumpShaderV3() |
| 1033 | `0x08c1a8` | `0x08c1d8` | +48 | 484 | 484 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV34InitEPNS_6EngineE`<br>AbyssEngine::BumpShaderV3::Init(AbyssEngine::Engine*) |
| 1034 | `0x08c38c` | `0x08c3bc` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV311SetInActiveEv`<br>AbyssEngine::BumpShaderV3::SetInActive() |
| 1035 | `0x08c3cc` | `0x08c3fc` | +48 | 740 | 740 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV314UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpShaderV3::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1036 | `0x08c6b0` | `0x08c6e0` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV3D0Ev`<br>AbyssEngine::BumpShaderV3::~BumpShaderV3() |
| 1037 | `0x08c6c0` | `0x08c6f0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22SimpleRefractionShaderC2Ev`<br>AbyssEngine::SimpleRefractionShader::SimpleRefractionShader() |
| 1038 | `0x08c6c0` | `0x08c6f0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22SimpleRefractionShaderC1Ev`<br>AbyssEngine::SimpleRefractionShader::SimpleRefractionShader() |
| 1039 | `0x08c74c` | `0x08c77c` | +48 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22SimpleRefractionShader4InitEPNS_6EngineE`<br>AbyssEngine::SimpleRefractionShader::Init(AbyssEngine::Engine*) |
| 1040 | `0x08c8a0` | `0x08c8d0` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22SimpleRefractionShader11SetInActiveEv`<br>AbyssEngine::SimpleRefractionShader::SetInActive() |
| 1041 | `0x08c8e0` | `0x08c910` | +48 | 536 | 536 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22SimpleRefractionShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::SimpleRefractionShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1042 | `0x08caf8` | `0x08cb28` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine22SimpleRefractionShaderD0Ev`<br>AbyssEngine::SimpleRefractionShader::~SimpleRefractionShader() |
| 1043 | `0x08cb08` | `0x08cb38` | +48 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17TextureConferenceC1Ev`<br>AbyssEngine::TextureConference::TextureConference() |
| 1044 | `0x08cb08` | `0x08cb38` | +48 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17TextureConferenceC2Ev`<br>AbyssEngine::TextureConference::TextureConference() |
| 1045 | `0x08cb98` | `0x08cbc8` | +48 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17TextureConference4InitEPNS_6EngineE`<br>AbyssEngine::TextureConference::Init(AbyssEngine::Engine*) |
| 1046 | `0x08cc34` | `0x08cc64` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17TextureConference11SetInActiveEv`<br>AbyssEngine::TextureConference::SetInActive() |
| 1047 | `0x08cc4a` | `0x08cc7a` | +48 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17TextureConference14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::TextureConference::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1048 | `0x08cd0a` | `0x08cd3a` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17TextureConferenceD0Ev`<br>AbyssEngine::TextureConference::~TextureConference() |
| 1049 | `0x08cd1c` | `0x08cd4c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShaderC1Ev`<br>AbyssEngine::GlowPPShader::GlowPPShader() |
| 1050 | `0x08cd1c` | `0x08cd4c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShaderC2Ev`<br>AbyssEngine::GlowPPShader::GlowPPShader() |
| 1051 | `0x08cda8` | `0x08cdd8` | +48 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShaderD1Ev`<br>AbyssEngine::GlowPPShader::~GlowPPShader() |
| 1052 | `0x08cda8` | `0x08cdd8` | +48 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShaderD2Ev`<br>AbyssEngine::GlowPPShader::~GlowPPShader() |
| 1053 | `0x08cdac` | `0x08cddc` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShaderD0Ev`<br>AbyssEngine::GlowPPShader::~GlowPPShader() |
| 1054 | `0x08cdbc` | `0x08cdec` | +48 | 356 | 356 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShader12InternalInitEPNS_6EngineE`<br>AbyssEngine::GlowPPShader::InternalInit(AbyssEngine::Engine*) |
| 1055 | `0x08cf20` | `0x08cf50` | +48 | 432 | 432 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShader4InitEPNS_6EngineE`<br>AbyssEngine::GlowPPShader::Init(AbyssEngine::Engine*) |
| 1056 | `0x08d0d0` | `0x08d100` | +48 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShader11SetInActiveEv`<br>AbyssEngine::GlowPPShader::SetInActive() |
| 1057 | `0x08d0ec` | `0x08d11c` | +48 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShader12RenderEffectEPNS_12FBOContainerEPNS_6EngineE`<br>AbyssEngine::GlowPPShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) |
| 1058 | `0x08d120` | `0x08d150` | +48 | 1264 | 1264 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShader12RenderEffectEPNS_12FBOContainerERS2_PNS_6EngineE`<br>AbyssEngine::GlowPPShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&, AbyssEngine::Engine*) |
| 1059 | `0x08d610` | `0x08d640` | +48 | 166 | 166 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12GlowPPShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::GlowPPShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1060 | `0x08d6b8` | `0x08d6e8` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV2C1Ev`<br>AbyssEngine::BumpShaderV2::BumpShaderV2() |
| 1061 | `0x08d6b8` | `0x08d6e8` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV2C2Ev`<br>AbyssEngine::BumpShaderV2::BumpShaderV2() |
| 1062 | `0x08d744` | `0x08d774` | +48 | 324 | 324 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV24InitEPNS_6EngineE`<br>AbyssEngine::BumpShaderV2::Init(AbyssEngine::Engine*) |
| 1063 | `0x08d888` | `0x08d8b8` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV211SetInActiveEv`<br>AbyssEngine::BumpShaderV2::SetInActive() |
| 1064 | `0x08d8c8` | `0x08d8f8` | +48 | 516 | 516 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV214UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpShaderV2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1065 | `0x08dacc` | `0x08dafc` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12BumpShaderV2D0Ev`<br>AbyssEngine::BumpShaderV2::~BumpShaderV2() |
| 1066 | `0x08dadc` | `0x08db0c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18TextureLightShaderC2Ev`<br>AbyssEngine::TextureLightShader::TextureLightShader() |
| 1067 | `0x08dadc` | `0x08db0c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18TextureLightShaderC1Ev`<br>AbyssEngine::TextureLightShader::TextureLightShader() |
| 1068 | `0x08db68` | `0x08db98` | +48 | 392 | 392 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18TextureLightShader4InitEPNS_6EngineE`<br>AbyssEngine::TextureLightShader::Init(AbyssEngine::Engine*) |
| 1069 | `0x08dcf0` | `0x08dd20` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18TextureLightShader11SetInActiveEv`<br>AbyssEngine::TextureLightShader::SetInActive() |
| 1070 | `0x08dd14` | `0x08dd44` | +48 | 520 | 520 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18TextureLightShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::TextureLightShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1071 | `0x08df1c` | `0x08df4c` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine18TextureLightShaderD0Ev`<br>AbyssEngine::TextureLightShader::~TextureLightShader() |
| 1072 | `0x08df2c` | `0x08df5c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19NoTexVtxColorShaderC2Ev`<br>AbyssEngine::NoTexVtxColorShader::NoTexVtxColorShader() |
| 1073 | `0x08df2c` | `0x08df5c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19NoTexVtxColorShaderC1Ev`<br>AbyssEngine::NoTexVtxColorShader::NoTexVtxColorShader() |
| 1074 | `0x08dfb8` | `0x08dfe8` | +48 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19NoTexVtxColorShader4InitEPNS_6EngineE`<br>AbyssEngine::NoTexVtxColorShader::Init(AbyssEngine::Engine*) |
| 1075 | `0x08e024` | `0x08e054` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19NoTexVtxColorShader11SetInActiveEv`<br>AbyssEngine::NoTexVtxColorShader::SetInActive() |
| 1076 | `0x08e03a` | `0x08e06a` | +48 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19NoTexVtxColorShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::NoTexVtxColorShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1077 | `0x08e0be` | `0x08e0ee` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine19NoTexVtxColorShaderD0Ev`<br>AbyssEngine::NoTexVtxColorShader::~NoTexVtxColorShader() |
| 1078 | `0x08e0d0` | `0x08e100` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader2C1Ev`<br>AbyssEngine::GenericShader2::GenericShader2() |
| 1079 | `0x08e0d0` | `0x08e100` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader2C2Ev`<br>AbyssEngine::GenericShader2::GenericShader2() |
| 1080 | `0x08e15c` | `0x08e18c` | +48 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader24InitEPNS_6EngineE`<br>AbyssEngine::GenericShader2::Init(AbyssEngine::Engine*) |
| 1081 | `0x08e27c` | `0x08e2ac` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader211SetInActiveEv`<br>AbyssEngine::GenericShader2::SetInActive() |
| 1082 | `0x08e2ba` | `0x08e2ea` | +48 | 344 | 344 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader214UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::GenericShader2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1083 | `0x08e412` | `0x08e442` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader2D0Ev`<br>AbyssEngine::GenericShader2::~GenericShader2() |
| 1084 | `0x08e424` | `0x08e454` | +48 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStructC2Ev`<br>AbyssEngine::ShaderBaseStruct::ShaderBaseStruct() |
| 1085 | `0x08e4c4` | `0x08e4f4` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStructD1Ev`<br>AbyssEngine::ShaderBaseStruct::~ShaderBaseStruct() |
| 1086 | `0x08e4c4` | `0x08e4f4` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStructD2Ev`<br>AbyssEngine::ShaderBaseStruct::~ShaderBaseStruct() |
| 1087 | `0x08e4e8` | `0x08e518` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ShaderBaseStructD0Ev`<br>AbyssEngine::ShaderBaseStruct::~ShaderBaseStruct() |
| 1088 | `0x08e4ea` | `0x08e51a` | +48 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStruct12DeleteShaderEv`<br>AbyssEngine::ShaderBaseStruct::DeleteShader() |
| 1089 | `0x08e4f0` | `0x08e520` | +48 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStruct13ES2LoadShaderEjPKc`<br>AbyssEngine::ShaderBaseStruct::ES2LoadShader(unsigned int, char const*) |
| 1090 | `0x08e584` | `0x08e5b4` | +48 | 240 | 240 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStruct14ES2LoadProgramEPKcS2_`<br>AbyssEngine::ShaderBaseStruct::ES2LoadProgram(char const*, char const*) |
| 1091 | `0x08e674` | `0x08e6a4` | +48 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStruct14LoadBindShaderEPKcS2_`<br>AbyssEngine::ShaderBaseStruct::LoadBindShader(char const*, char const*) |
| 1092 | `0x08e744` | `0x08e774` | +48 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine16ShaderBaseStruct9UseShaderEb`<br>AbyssEngine::ShaderBaseStruct::UseShader(bool) |
| 1093 | `0x08e74a` | `0x08e77a` | +48 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ShaderBaseStruct6UpdateEv`<br>AbyssEngine::ShaderBaseStruct::Update() |
| 1094 | `0x08e750` | `0x08e780` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13GenericShaderC1Ev`<br>AbyssEngine::GenericShader::GenericShader() |
| 1095 | `0x08e750` | `0x08e780` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13GenericShaderC2Ev`<br>AbyssEngine::GenericShader::GenericShader() |
| 1096 | `0x08e7dc` | `0x08e80c` | +48 | 356 | 356 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13GenericShader4InitEPNS_6EngineE`<br>AbyssEngine::GenericShader::Init(AbyssEngine::Engine*) |
| 1097 | `0x08e940` | `0x08e970` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13GenericShader11SetInActiveEv`<br>AbyssEngine::GenericShader::SetInActive() |
| 1098 | `0x08e980` | `0x08e9b0` | +48 | 528 | 528 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13GenericShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::GenericShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1099 | `0x08eb90` | `0x08ebc0` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13GenericShaderD0Ev`<br>AbyssEngine::GenericShader::~GenericShader() |
| 1100 | `0x08eba0` | `0x08ebd0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader1C1Ev`<br>AbyssEngine::GenericShader1::GenericShader1() |
| 1101 | `0x08eba0` | `0x08ebd0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader1C2Ev`<br>AbyssEngine::GenericShader1::GenericShader1() |
| 1102 | `0x08ec2c` | `0x08ec5c` | +48 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader14InitEPNS_6EngineE`<br>AbyssEngine::GenericShader1::Init(AbyssEngine::Engine*) |
| 1103 | `0x08ed4c` | `0x08ed7c` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader111SetInActiveEv`<br>AbyssEngine::GenericShader1::SetInActive() |
| 1104 | `0x08ed8a` | `0x08edba` | +48 | 344 | 344 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader114UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::GenericShader1::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1105 | `0x08eee2` | `0x08ef12` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine14GenericShader1D0Ev`<br>AbyssEngine::GenericShader1::~GenericShader1() |
| 1106 | `0x08eef4` | `0x08ef24` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShaderC2Ev`<br>AbyssEngine::PostBWShader::PostBWShader() |
| 1107 | `0x08eef4` | `0x08ef24` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShaderC1Ev`<br>AbyssEngine::PostBWShader::PostBWShader() |
| 1108 | `0x08ef80` | `0x08efb0` | +48 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShaderD2Ev`<br>AbyssEngine::PostBWShader::~PostBWShader() |
| 1109 | `0x08ef80` | `0x08efb0` | +48 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShaderD1Ev`<br>AbyssEngine::PostBWShader::~PostBWShader() |
| 1110 | `0x08ef94` | `0x08efc4` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShaderD0Ev`<br>AbyssEngine::PostBWShader::~PostBWShader() |
| 1111 | `0x08efa4` | `0x08efd4` | +48 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShader4InitEPNS_6EngineE`<br>AbyssEngine::PostBWShader::Init(AbyssEngine::Engine*) |
| 1112 | `0x08f018` | `0x08f048` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShader11SetInActiveEv`<br>AbyssEngine::PostBWShader::SetInActive() |
| 1113 | `0x08f030` | `0x08f060` | +48 | 524 | 524 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShader12RenderEffectEPNS_12FBOContainerEPNS_6EngineE`<br>AbyssEngine::PostBWShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) |
| 1114 | `0x08f23c` | `0x08f26c` | +48 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12PostBWShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::PostBWShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1115 | `0x08f2d4` | `0x08f304` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9DNSShaderC1Ev`<br>AbyssEngine::DNSShader::DNSShader() |
| 1116 | `0x08f2d4` | `0x08f304` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9DNSShaderC2Ev`<br>AbyssEngine::DNSShader::DNSShader() |
| 1117 | `0x08f360` | `0x08f390` | +48 | 356 | 356 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9DNSShader4InitEPNS_6EngineE`<br>AbyssEngine::DNSShader::Init(AbyssEngine::Engine*) |
| 1118 | `0x08f4c4` | `0x08f4f4` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9DNSShader11SetInActiveEv`<br>AbyssEngine::DNSShader::SetInActive() |
| 1119 | `0x08f504` | `0x08f534` | +48 | 588 | 588 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9DNSShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::DNSShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1120 | `0x08f750` | `0x08f780` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine9DNSShaderD0Ev`<br>AbyssEngine::DNSShader::~DNSShader() |
| 1121 | `0x08f760` | `0x08f790` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpShaderRefractC2Ev`<br>AbyssEngine::BumpShaderRefract::BumpShaderRefract() |
| 1122 | `0x08f760` | `0x08f790` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpShaderRefractC1Ev`<br>AbyssEngine::BumpShaderRefract::BumpShaderRefract() |
| 1123 | `0x08f7ec` | `0x08f81c` | +48 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpShaderRefract4InitEPNS_6EngineE`<br>AbyssEngine::BumpShaderRefract::Init(AbyssEngine::Engine*) |
| 1124 | `0x08f910` | `0x08f940` | +48 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpShaderRefract11SetInActiveEv`<br>AbyssEngine::BumpShaderRefract::SetInActive() |
| 1125 | `0x08f93a` | `0x08f96a` | +48 | 392 | 392 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpShaderRefract14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpShaderRefract::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1126 | `0x08fac2` | `0x08faf2` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpShaderRefractD0Ev`<br>AbyssEngine::BumpShaderRefract::~BumpShaderRefract() |
| 1127 | `0x08fad4` | `0x08fb04` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpRimCubeShaderC2Ev`<br>AbyssEngine::BumpRimCubeShader::BumpRimCubeShader() |
| 1128 | `0x08fad4` | `0x08fb04` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpRimCubeShaderC1Ev`<br>AbyssEngine::BumpRimCubeShader::BumpRimCubeShader() |
| 1129 | `0x08fb60` | `0x08fb90` | +48 | 620 | 620 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpRimCubeShader4InitEPNS_6EngineE`<br>AbyssEngine::BumpRimCubeShader::Init(AbyssEngine::Engine*) |
| 1130 | `0x08fdcc` | `0x08fdfc` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpRimCubeShader11SetInActiveEv`<br>AbyssEngine::BumpRimCubeShader::SetInActive() |
| 1131 | `0x08fe0c` | `0x08fe3c` | +48 | 852 | 852 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpRimCubeShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpRimCubeShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1132 | `0x090160` | `0x090190` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17BumpRimCubeShaderD0Ev`<br>AbyssEngine::BumpRimCubeShader::~BumpRimCubeShader() |
| 1133 | `0x090170` | `0x0901a0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17VertexColorShaderC1Ev`<br>AbyssEngine::VertexColorShader::VertexColorShader() |
| 1134 | `0x090170` | `0x0901a0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17VertexColorShaderC2Ev`<br>AbyssEngine::VertexColorShader::VertexColorShader() |
| 1135 | `0x0901fc` | `0x09022c` | +48 | 304 | 304 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17VertexColorShader4InitEPNS_6EngineE`<br>AbyssEngine::VertexColorShader::Init(AbyssEngine::Engine*) |
| 1136 | `0x09032c` | `0x09035c` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17VertexColorShader11SetInActiveEv`<br>AbyssEngine::VertexColorShader::SetInActive() |
| 1137 | `0x090374` | `0x0903a4` | +48 | 580 | 580 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17VertexColorShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::VertexColorShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1138 | `0x0905b8` | `0x0905e8` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17VertexColorShaderD0Ev`<br>AbyssEngine::VertexColorShader::~VertexColorShader() |
| 1139 | `0x0905c8` | `0x0905f8` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11NoTexShaderC1Ev`<br>AbyssEngine::NoTexShader::NoTexShader() |
| 1140 | `0x0905c8` | `0x0905f8` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11NoTexShaderC2Ev`<br>AbyssEngine::NoTexShader::NoTexShader() |
| 1141 | `0x090654` | `0x090684` | +48 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11NoTexShader4InitEPNS_6EngineE`<br>AbyssEngine::NoTexShader::Init(AbyssEngine::Engine*) |
| 1142 | `0x0906b0` | `0x0906e0` | +48 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11NoTexShader11SetInActiveEv`<br>AbyssEngine::NoTexShader::SetInActive() |
| 1143 | `0x0906b6` | `0x0906e6` | +48 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11NoTexShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::NoTexShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1144 | `0x09073a` | `0x09076a` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11NoTexShaderD0Ev`<br>AbyssEngine::NoTexShader::~NoTexShader() |
| 1145 | `0x09074c` | `0x09077c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11CubeMappingC2Ev`<br>AbyssEngine::CubeMapping::CubeMapping() |
| 1146 | `0x09074c` | `0x09077c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11CubeMappingC1Ev`<br>AbyssEngine::CubeMapping::CubeMapping() |
| 1147 | `0x0907d8` | `0x090808` | +48 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11CubeMapping4InitEPNS_6EngineE`<br>AbyssEngine::CubeMapping::Init(AbyssEngine::Engine*) |
| 1148 | `0x090910` | `0x090940` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11CubeMapping11SetInActiveEv`<br>AbyssEngine::CubeMapping::SetInActive() |
| 1149 | `0x090934` | `0x090964` | +48 | 380 | 380 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11CubeMapping14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::CubeMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1150 | `0x090ab0` | `0x090ae0` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11CubeMappingD0Ev`<br>AbyssEngine::CubeMapping::~CubeMapping() |
| 1151 | `0x090ac0` | `0x090af0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17CubeNormalMappingC2Ev`<br>AbyssEngine::CubeNormalMapping::CubeNormalMapping() |
| 1152 | `0x090ac0` | `0x090af0` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17CubeNormalMappingC1Ev`<br>AbyssEngine::CubeNormalMapping::CubeNormalMapping() |
| 1153 | `0x090b4c` | `0x090b7c` | +48 | 376 | 376 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17CubeNormalMapping4InitEPNS_6EngineE`<br>AbyssEngine::CubeNormalMapping::Init(AbyssEngine::Engine*) |
| 1154 | `0x090cc4` | `0x090cf4` | +48 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17CubeNormalMapping11SetInActiveEv`<br>AbyssEngine::CubeNormalMapping::SetInActive() |
| 1155 | `0x090cf0` | `0x090d20` | +48 | 488 | 488 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17CubeNormalMapping14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::CubeNormalMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1156 | `0x090ed8` | `0x090f08` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine17CubeNormalMappingD0Ev`<br>AbyssEngine::CubeNormalMapping::~CubeNormalMapping() |
| 1157 | `0x090ee8` | `0x090f18` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShaderC1Ev`<br>AbyssEngine::TextureShader::TextureShader() |
| 1158 | `0x090ee8` | `0x090f18` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShaderC2Ev`<br>AbyssEngine::TextureShader::TextureShader() |
| 1159 | `0x090f74` | `0x090fa4` | +48 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShader9UseShaderEb`<br>AbyssEngine::TextureShader::UseShader(bool) |
| 1160 | `0x090f94` | `0x090fc4` | +48 | 240 | 240 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShader23ConnectShaderComponentsEji`<br>AbyssEngine::TextureShader::ConnectShaderComponents(unsigned int, int) |
| 1161 | `0x091084` | `0x0910b4` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShader4InitEPNS_6EngineE`<br>AbyssEngine::TextureShader::Init(AbyssEngine::Engine*) |
| 1162 | `0x0910cc` | `0x0910fc` | +48 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShader11SetInActiveEv`<br>AbyssEngine::TextureShader::SetInActive() |
| 1163 | `0x0910f8` | `0x091128` | +48 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::TextureShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1164 | `0x091258` | `0x091288` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13TextureShaderD0Ev`<br>AbyssEngine::TextureShader::~TextureShader() |
| 1165 | `0x091268` | `0x091298` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BumpMappingC2Ev`<br>AbyssEngine::BumpMapping::BumpMapping() |
| 1166 | `0x091268` | `0x091298` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BumpMappingC1Ev`<br>AbyssEngine::BumpMapping::BumpMapping() |
| 1167 | `0x0912f4` | `0x091324` | +48 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BumpMapping4InitEPNS_6EngineE`<br>AbyssEngine::BumpMapping::Init(AbyssEngine::Engine*) |
| 1168 | `0x0913cc` | `0x0913fc` | +48 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BumpMapping11SetInActiveEv`<br>AbyssEngine::BumpMapping::SetInActive() |
| 1169 | `0x0913f8` | `0x091428` | +48 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BumpMapping14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::BumpMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1170 | `0x0914d0` | `0x091500` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11BumpMappingD0Ev`<br>AbyssEngine::BumpMapping::~BumpMapping() |
| 1171 | `0x0914e0` | `0x091510` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10MaskShaderC1Ev`<br>AbyssEngine::MaskShader::MaskShader() |
| 1172 | `0x0914e0` | `0x091510` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10MaskShaderC2Ev`<br>AbyssEngine::MaskShader::MaskShader() |
| 1173 | `0x09156c` | `0x09159c` | +48 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10MaskShader4InitEPNS_6EngineE`<br>AbyssEngine::MaskShader::Init(AbyssEngine::Engine*) |
| 1174 | `0x091630` | `0x091660` | +48 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10MaskShader11SetInActiveEv`<br>AbyssEngine::MaskShader::SetInActive() |
| 1175 | `0x09165a` | `0x09168a` | +48 | 290 | 290 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10MaskShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::MaskShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1176 | `0x09177c` | `0x0917ac` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10MaskShaderD0Ev`<br>AbyssEngine::MaskShader::~MaskShader() |
| 1177 | `0x09178c` | `0x0917bc` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13SandboxShaderC1Ev`<br>AbyssEngine::SandboxShader::SandboxShader() |
| 1178 | `0x09178c` | `0x0917bc` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13SandboxShaderC2Ev`<br>AbyssEngine::SandboxShader::SandboxShader() |
| 1179 | `0x091818` | `0x091848` | +48 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13SandboxShader4InitEPNS_6EngineE`<br>AbyssEngine::SandboxShader::Init(AbyssEngine::Engine*) |
| 1180 | `0x091920` | `0x091950` | +48 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13SandboxShader11SetInActiveEv`<br>AbyssEngine::SandboxShader::SetInActive() |
| 1181 | `0x09194c` | `0x09197c` | +48 | 388 | 388 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13SandboxShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::SandboxShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1182 | `0x091ad0` | `0x091b00` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine13SandboxShaderD0Ev`<br>AbyssEngine::SandboxShader::~SandboxShader() |
| 1183 | `0x091ae0` | `0x091b10` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine29VertexColorAlphaTextureShaderC2Ev`<br>AbyssEngine::VertexColorAlphaTextureShader::VertexColorAlphaTextureShader() |
| 1184 | `0x091ae0` | `0x091b10` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine29VertexColorAlphaTextureShaderC1Ev`<br>AbyssEngine::VertexColorAlphaTextureShader::VertexColorAlphaTextureShader() |
| 1185 | `0x091b6c` | `0x091b9c` | +48 | 304 | 304 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine29VertexColorAlphaTextureShader4InitEPNS_6EngineE`<br>AbyssEngine::VertexColorAlphaTextureShader::Init(AbyssEngine::Engine*) |
| 1186 | `0x091c9c` | `0x091ccc` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine29VertexColorAlphaTextureShader11SetInActiveEv`<br>AbyssEngine::VertexColorAlphaTextureShader::SetInActive() |
| 1187 | `0x091ce4` | `0x091d14` | +48 | 580 | 580 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine29VertexColorAlphaTextureShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::VertexColorAlphaTextureShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1188 | `0x091f28` | `0x091f58` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine29VertexColorAlphaTextureShaderD0Ev`<br>AbyssEngine::VertexColorAlphaTextureShader::~VertexColorAlphaTextureShader() |
| 1189 | `0x091f38` | `0x091f68` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10GlowShaderC2Ev`<br>AbyssEngine::GlowShader::GlowShader() |
| 1190 | `0x091f38` | `0x091f68` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10GlowShaderC1Ev`<br>AbyssEngine::GlowShader::GlowShader() |
| 1191 | `0x091fc4` | `0x091ff4` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10GlowShader4InitEPNS_6EngineE`<br>AbyssEngine::GlowShader::Init(AbyssEngine::Engine*) |
| 1192 | `0x092050` | `0x092080` | +48 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10GlowShader11SetInActiveEv`<br>AbyssEngine::GlowShader::SetInActive() |
| 1193 | `0x092070` | `0x0920a0` | +48 | 186 | 186 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10GlowShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::GlowShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1194 | `0x09212a` | `0x09215a` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine10GlowShaderD0Ev`<br>AbyssEngine::GlowShader::~GlowShader() |
| 1195 | `0x09213c` | `0x09216c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11ColorMixAddC1Ev`<br>AbyssEngine::ColorMixAdd::ColorMixAdd() |
| 1196 | `0x09213c` | `0x09216c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11ColorMixAddC2Ev`<br>AbyssEngine::ColorMixAdd::ColorMixAdd() |
| 1197 | `0x0921c8` | `0x0921f8` | +48 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11ColorMixAdd4InitEPNS_6EngineE`<br>AbyssEngine::ColorMixAdd::Init(AbyssEngine::Engine*) |
| 1198 | `0x092284` | `0x0922b4` | +48 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11ColorMixAdd11SetInActiveEv`<br>AbyssEngine::ColorMixAdd::SetInActive() |
| 1199 | `0x0922a2` | `0x0922d2` | +48 | 230 | 230 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11ColorMixAdd14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::ColorMixAdd::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1200 | `0x092388` | `0x0923b8` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11ColorMixAddD0Ev`<br>AbyssEngine::ColorMixAdd::~ColorMixAdd() |
| 1201 | `0x092398` | `0x0923c8` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PulseShaderC2Ev`<br>AbyssEngine::PulseShader::PulseShader() |
| 1202 | `0x092398` | `0x0923c8` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PulseShaderC1Ev`<br>AbyssEngine::PulseShader::PulseShader() |
| 1203 | `0x092424` | `0x092454` | +48 | 304 | 304 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PulseShader4InitEPNS_6EngineE`<br>AbyssEngine::PulseShader::Init(AbyssEngine::Engine*) |
| 1204 | `0x092554` | `0x092584` | +48 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PulseShader11SetInActiveEv`<br>AbyssEngine::PulseShader::SetInActive() |
| 1205 | `0x092594` | `0x0925c4` | +48 | 572 | 572 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PulseShader14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::PulseShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1206 | `0x0927d0` | `0x092800` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine11PulseShaderD0Ev`<br>AbyssEngine::PulseShader::~PulseShader() |
| 1207 | `0x0927e0` | `0x092810` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15SpecCubeMappingC1Ev`<br>AbyssEngine::SpecCubeMapping::SpecCubeMapping() |
| 1208 | `0x0927e0` | `0x092810` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15SpecCubeMappingC2Ev`<br>AbyssEngine::SpecCubeMapping::SpecCubeMapping() |
| 1209 | `0x09286c` | `0x09289c` | +48 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15SpecCubeMapping4InitEPNS_6EngineE`<br>AbyssEngine::SpecCubeMapping::Init(AbyssEngine::Engine*) |
| 1210 | `0x0929c0` | `0x0929f0` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15SpecCubeMapping11SetInActiveEv`<br>AbyssEngine::SpecCubeMapping::SetInActive() |
| 1211 | `0x0929e4` | `0x092a14` | +48 | 380 | 380 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15SpecCubeMapping14UpdateMeshDataEPNS_4MeshEPNS_6EngineE`<br>AbyssEngine::SpecCubeMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) |
| 1212 | `0x092b60` | `0x092b90` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15SpecCubeMappingD0Ev`<br>AbyssEngine::SpecCubeMapping::~SpecCubeMapping() |
| 1213 | `0x092b70` | `0x092ba0` | +48 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN9FModSoundC2Ev`<br>FModSound::FModSound() |
| 1214 | `0x092b70` | `0x092ba0` | +48 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN9FModSoundC1Ev`<br>FModSound::FModSound() |
| 1215 | `0x092bd4` | `0x092c04` | +48 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN9FModSoundD1Ev`<br>FModSound::~FModSound() |
| 1216 | `0x092bd4` | `0x092c04` | +48 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN9FModSoundD2Ev`<br>FModSound::~FModSound() |
| 1217 | `0x092be6` | `0x092c16` | +48 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN9FModSound7releaseEv`<br>FModSound::release() |
| 1218 | `0x092c30` | `0x092c60` | +48 | 332 | 332 | 0 | 100.0% |  | Y | `_ZN9FModSound4initEv`<br>FModSound::init() |
| 1219 | `0x092d7c` | `0x092dac` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9FModSound8ERRCHECKE11FMOD_RESULT`<br>FModSound::ERRCHECK(FMOD_RESULT) |
| 1220 | `0x092d80` | `0x092db0` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN9FModSound16setAudioLanguageEi`<br>FModSound::setAudioLanguage(int) |
| 1221 | `0x092da4` | `0x092dd4` | +48 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9FModSound18isHeadsetPluggedInEv`<br>FModSound::isHeadsetPluggedIn() |
| 1222 | `0x092da8` | `0x092dd8` | +48 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9FModSound24tryToStopMusicForBGMusicEv`<br>FModSound::tryToStopMusicForBGMusic() |
| 1223 | `0x092dac` | `0x092ddc` | +48 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN9FModSound20stopAllSoundFXEventsEv`<br>FModSound::stopAllSoundFXEvents() |
| 1224 | `0x092de0` | `0x092e10` | +48 | 260 | 260 | 0 | 100.0% |  | Y | `_ZN9FModSound13freeAllEventsEv`<br>FModSound::freeAllEvents() |
| 1225 | `0x092ee4` | `0x092f14` | +48 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN9FModSound8getParamEPKci`<br>FModSound::getParam(char const*, int) |
| 1226 | `0x092f20` | `0x092f50` | +48 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9FModSound13setParamValueEiif`<br>FModSound::setParamValue(int, int, float) |
| 1227 | `0x092f64` | `0x092f94` | +48 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9FModSound13setParamValueEPKcif`<br>FModSound::setParamValue(char const*, int, float) |
| 1228 | `0x092fa8` | `0x092fd8` | +48 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN9FModSound18setMusicParamValueEif`<br>FModSound::setMusicParamValue(int, float) |
| 1229 | `0x092fb8` | `0x092fe8` | +48 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN9FModSound9setVolumeEif`<br>FModSound::setVolume(int, float) |
| 1230 | `0x092fd6` | `0x093006` | +48 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN9FModSound14setSoundVolumeEif`<br>FModSound::setSoundVolume(int, float) |
| 1231 | `0x092ff0` | `0x093020` | +48 | 500 | 500 | 0 | 100.0% |  | Y | `_ZN9FModSound9updateAllEPN11AbyssEngine6AEMath6VectorES3_S3_S3_`<br>FModSound::updateAll(AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*) |
| 1232 | `0x0931e4` | `0x093214` | +48 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9FModSound9isPlayingEi`<br>FModSound::isPlaying(int) |
| 1233 | `0x093228` | `0x093258` | +48 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9FModSound15isChannelActiveEi`<br>FModSound::isChannelActive(int) |
| 1234 | `0x09326c` | `0x09329c` | +48 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN9FModSound17IsCategoryEnabledEi`<br>FModSound::IsCategoryEnabled(int) |
| 1235 | `0x093288` | `0x0932b8` | +48 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN9FModSound14enableCategoryEib`<br>FModSound::enableCategory(int, bool) |
| 1236 | `0x0932f4` | `0x093324` | +48 | 560 | 560 | 0 | 100.0% |  | Y | `_ZN9FModSound4playEiPN11AbyssEngine6AEMath6VectorES3_f`<br>FModSound::play(int, AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*, float) |
| 1237 | `0x093524` | `0x093554` | +48 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN9FModSound23playMusicFadeOutCurrentEi`<br>FModSound::playMusicFadeOutCurrent(int) |
| 1238 | `0x093550` | `0x093580` | +48 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN9FModSound10fadeOutNowEv`<br>FModSound::fadeOutNow() |
| 1239 | `0x093568` | `0x093598` | +48 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN9FModSound4stopEi`<br>FModSound::stop(int) |
| 1240 | `0x09357c` | `0x0935ac` | +48 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN9FModSound4stopEPN4FMOD5EventE`<br>FModSound::stop(FMOD::Event*) |
| 1241 | `0x093588` | `0x0935b8` | +48 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN9FModSound23updateEvent3DAttributesEiPN11AbyssEngine6AEMath6VectorES3_b`<br>FModSound::updateEvent3DAttributes(int, AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*, bool) |
| 1242 | `0x0935a8` | `0x0935d8` | +48 | 584 | 584 | 0 | 100.0% |  | Y | `_ZN9FModSound23updateEvent3DAttributesEPN4FMOD5EventEiPN11AbyssEngine6AEMath6VectorES6_b`<br>FModSound::updateEvent3DAttributes(FMOD::Event*, int, AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*, bool) |
| 1243 | `0x0937f0` | `0x093820` | +48 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN9FModSound5pauseEPN4FMOD5EventE`<br>FModSound::pause(FMOD::Event*) |
| 1244 | `0x09384c` | `0x09387c` | +48 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN9FModSound6resumeEPN4FMOD5EventE`<br>FModSound::resume(FMOD::Event*) |
| 1245 | `0x093874` | `0x0938a4` | +48 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9FModSound13setParamValueEPN4FMOD5EventEif`<br>FModSound::setParamValue(FMOD::Event*, int, float) |
| 1246 | `0x0938b8` | `0x0938e8` | +48 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN9FModSound12enableReverbEi`<br>FModSound::enableReverb(int) |
| 1247 | `0x09392c` | `0x09395c` | +48 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN9FModSound13disableReverbEv`<br>FModSound::disableReverb() |
| 1248 | `0x093988` | `0x0939b8` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN9FModSound18getPlayingProgressEi`<br>FModSound::getPlayingProgress(int) |
| 1249 | `0x093a14` | `0x093a44` | +48 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN9FModSound19getEventPauseLengthEi`<br>FModSound::getEventPauseLength(int) |
| 1250 | `0x093a74` | `0x093aa4` | +48 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN9FModSound12setDownPitchEb`<br>FModSound::setDownPitch(bool) |
| 1251 | `0x093ad8` | `0x093b08` | +48 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN9FModSound14promptMusicCueEi`<br>FModSound::promptMusicCue(int) |
| 1252 | `0x093ae8` | `0x093b18` | +48 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN9FModSound5pauseEi`<br>FModSound::pause(int) |
| 1253 | `0x093b00` | `0x093b30` | +48 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN9FModSound6resumeEi`<br>FModSound::resume(int) |
| 1254 | `0x093b18` | `0x093b48` | +48 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN9FModSound8pauseAllEv`<br>FModSound::pauseAll() |
| 1255 | `0x093b4c` | `0x093b7c` | +48 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN9FModSound7stopAllEv`<br>FModSound::stopAll() |
| 1256 | `0x093b80` | `0x093bb0` | +48 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN9FModSound9resumeAllEv`<br>FModSound::resumeAll() |
| 1257 | `0x093bb4` | `0x093be4` | +48 | 70 | 70 | 0 | 100.0% |  | Y | `_ZN9FModSound15pauseAllPlayingEv`<br>FModSound::pauseAllPlaying() |
| 1258 | `0x093bfc` | `0x093c2c` | +48 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN9FModSound28pauseAllPlayingSoundFXEventsEv`<br>FModSound::pauseAllPlayingSoundFXEvents() |
| 1259 | `0x093c88` | `0x093cb8` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_Z9GetEnginev`<br>GetEngine() |
| 1260 | `0x093c98` | `0x093cc8` | +48 | 20 | 20 | 0 | 100.0% |  | Y | `ExitFunction`<br>ExitFunction |
| 1261 | `0x093cac` | `0x093cdc` | +48 | 4 | 4 | 0 | 100.0% | Y | Y | `ndk23_getScreenshotFlag`<br>ndk23_getScreenshotFlag |
| 1262 | `0x093cb0` | `0x093ce0` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `ndk23_resetScreenshotFlag`<br>ndk23_resetScreenshotFlag |
| 1263 | `0x093cb4` | `0x093ce4` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `ndk23_getExitFlag`<br>ndk23_getExitFlag |
| 1264 | `0x093cc4` | `0x093cf4` | +48 | 72 | 72 | 0 | 100.0% |  | Y | `loadAPK`<br>loadAPK |
| 1265 | `0x093d0c` | `0x093d3c` | +48 | 68 | 68 | 0 | 100.0% |  | Y | `loadAPKAndZip`<br>loadAPKAndZip |
| 1266 | `0x093d50` | `0x093d80` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `ndk23_setDisplayHeightAndWidth`<br>ndk23_setDisplayHeightAndWidth |
| 1267 | `0x093d52` | `0x093d82` | +48 | 4 | 4 | 0 | 100.0% | Y | Y | `ndk23_getCurrentFiredStatus`<br>ndk23_getCurrentFiredStatus |
| 1268 | `0x093d58` | `0x093d88` | +48 | 296 | 296 | 0 | 100.0% |  | Y | `ndk23_Init`<br>ndk23_Init |
| 1269 | `0x093e80` | `0x093eb0` | +48 | 300 | 300 | 0 | 100.0% |  | Y | `ndk23_InitWithZip`<br>ndk23_InitWithZip |
| 1270 | `0x093fac` | `0x093fdc` | +48 | 64 | 64 | 0 | 100.0% |  | Y | `ndk23_resize`<br>ndk23_resize |
| 1271 | `0x093fec` | `0x09401c` | +48 | 40 | 40 | 0 | 100.0% |  | Y | `ndk23_ndkDone`<br>ndk23_ndkDone |
| 1272 | `0x094014` | `0x094044` | +48 | 4 | 4 | 0 | 100.0% |  | Y | `ndk23_handleTouchScreenEvent`<br>ndk23_handleTouchScreenEvent |
| 1273 | `0x094018` | `0x094048` | +48 | 276 | 276 | 0 | 100.0% |  | Y | `ndk23_handleAcceleration`<br>ndk23_handleAcceleration |
| 1274 | `0x09412c` | `0x09415c` | +48 | 340 | 340 | 0 | 100.0% |  | Y | `ndk23_newrender`<br>ndk23_newrender |
| 1275 | `0x094280` | `0x0942b0` | +48 | 60 | 60 | 0 | 100.0% |  | Y | `ndk_checkPlaytimeAndSpendOfferwallCredits`<br>ndk_checkPlaytimeAndSpendOfferwallCredits |
| 1276 | `0x0942bc` | `0x0942ec` | +48 | 156 | 156 | 0 | 100.0% |  | Y | `ndk23_handleTouchPadEvent`<br>ndk23_handleTouchPadEvent |
| 1277 | `0x094358` | `0x094388` | +48 | 40 | 40 | 0 | 100.0% |  | Y | `ndk23_setRootDirectory`<br>ndk23_setRootDirectory |
| 1278 | `0x094380` | `0x0943b0` | +48 | 40 | 40 | 0 | 100.0% |  | Y | `ndk23_setZipDirectory`<br>ndk23_setZipDirectory |
| 1279 | `0x0943a8` | `0x0943d8` | +48 | 20 | 20 | 0 | 100.0% |  | Y | `ndk23_setCountryCode`<br>ndk23_setCountryCode |
| 1280 | `0x0943bc` | `0x0943ec` | +48 | 52 | 52 | 0 | 100.0% |  | Y | `ndk23_sendingPauseSignal`<br>ndk23_sendingPauseSignal |
| 1281 | `0x0943f0` | `0x094420` | +48 | 32 | 32 | 0 | 100.0% |  | Y | `ndk23_sendingResumeSignal`<br>ndk23_sendingResumeSignal |
| 1282 | `0x094410` | `0x094440` | +48 | 276 | 276 | 0 | 100.0% |  | Y | `ndk_iapBoughtPremium`<br>ndk_iapBoughtPremium |
| 1283 | `0x094524` | `0x094554` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `ndk_getDLC_1_BOUGHT`<br>ndk_getDLC_1_BOUGHT |
| 1284 | `0x094548` | `0x094578` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `ndk_getDLC_2_BOUGHT`<br>ndk_getDLC_2_BOUGHT |
| 1285 | `0x09456c` | `0x09459c` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `ndk_getDLC_3_BOUGHT`<br>ndk_getDLC_3_BOUGHT |
| 1286 | `0x094590` | `0x0945c0` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `ndk_getDLC_4_BOUGHT`<br>ndk_getDLC_4_BOUGHT |
| 1287 | `0x0945b4` | `0x0945e4` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `ndk_getDLC_5_BOUGHT`<br>ndk_getDLC_5_BOUGHT |
| 1288 | `0x0945d8` | `0x094608` | +48 | 84 | 84 | 0 | 100.0% |  | Y | `ndk_iapBoughtConsumable`<br>ndk_iapBoughtConsumable |
| 1289 | `0x09462c` | `0x09465c` | +48 | 220 | 220 | 0 | 100.0% |  | Y | `setBaughtCredits`<br>setBaughtCredits |
| 1290 | `0x094708` | `0x094738` | +48 | 104 | 104 | 0 | 100.0% |  | Y | `checkFirstCreditPackBoughtWriteAction`<br>checkFirstCreditPackBoughtWriteAction |
| 1291 | `0x094770` | `0x0947a0` | +48 | 60 | 60 | 0 | 100.0% |  | Y | `ndk_autosave`<br>ndk_autosave |
| 1292 | `0x0947ac` | `0x0947dc` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `ndk_getCurrentApplicationModule`<br>ndk_getCurrentApplicationModule |
| 1293 | `0x0947d0` | `0x094800` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `ndk_getLogoShown`<br>ndk_getLogoShown |
| 1294 | `0x0947e0` | `0x094810` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `ndk_isInMainMenu`<br>ndk_isInMainMenu |
| 1295 | `0x0947f0` | `0x094820` | +48 | 40 | 40 | 0 | 100.0% |  | Y | `pConstToNonConst`<br>pConstToNonConst |
| 1296 | `0x094818` | `0x094848` | +48 | 16 | 16 | 0 | 100.0% | Y | Y | `getStringUTFChars`<br>getStringUTFChars |
| 1297 | `0x094828` | `0x094858` | +48 | 48 | 48 | 0 | 100.0% |  | Y | `releaseStringUTFChars`<br>releaseStringUTFChars |
| 1298 | `0x094858` | `0x094888` | +48 | 1504 | 1504 | 0 | 100.0% |  | Y | `ndk_setNativeItemInformationList`<br>ndk_setNativeItemInformationList |
| 1299 | `0x094e38` | `0x094e68` | +48 | 676 | 676 | 0 | 100.0% |  | Y | `ndk_resetNativeItemInformationList`<br>ndk_resetNativeItemInformationList |
| 1300 | `0x0950dc` | `0x09510c` | +48 | 116 | 116 | 0 | 100.0% |  | Y | `_Z8AddTouchiiii`<br>AddTouch(int, int, int, int) |
| 1301 | `0x095150` | `0x095180` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_Z13GetTouchCountv`<br>GetTouchCount() |
| 1302 | `0x095160` | `0x095190` | +48 | 16 | 16 | 0 | 100.0% |  | Y | `_Z13RemoveTouchesv`<br>RemoveTouches() |
| 1303 | `0x095170` | `0x0951a0` | +48 | 28 | 28 | 0 | 100.0% |  | Y | `_Z8GetTouchi`<br>GetTouch(int) |
| 1304 | `0x09518c` | `0x0951bc` | +48 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN10LODManagerC2Ev`<br>LODManager::LODManager() |
| 1305 | `0x09518c` | `0x0951bc` | +48 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN10LODManagerC1Ev`<br>LODManager::LODManager() |
| 1306 | `0x0951c0` | `0x0951f0` | +48 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN5ArrayIP10AEGeometryEC2Ev`<br>Array<AEGeometry*>::Array() |
| 1307 | `0x0951dc` | `0x09520c` | +48 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10LODManagerD1Ev`<br>LODManager::~LODManager() |
| 1308 | `0x0951dc` | `0x09520c` | +48 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10LODManagerD2Ev`<br>LODManager::~LODManager() |
| 1309 | `0x0951f6` | `0x095226` | +48 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP10AEGeometryED2Ev`<br>Array<AEGeometry*>::~Array() |
| 1310 | `0x09520c` | `0x09523c` | +48 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN10LODManager9addObjectEP10AEGeometry`<br>LODManager::addObject(AEGeometry*) |
| 1311 | `0x09522a` | `0x09525a` | +48 | 36 | 36 | 0 | 100.0% | Y | Y | `_Z8ArrayAddIP10AEGeometryEvT_R5ArrayIS2_E`<br>void ArrayAdd<AEGeometry*>(AEGeometry*, Array<AEGeometry*>&) |
| 1312 | `0x09524e` | `0x09527e` | +48 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN10LODManager12removeObjectEP10AEGeometry`<br>LODManager::removeObject(AEGeometry*) |
| 1313 | `0x09527e` | `0x0952ae` | +48 | 68 | 68 | 0 | 100.0% | Y | Y | `_Z11ArrayRemoveIP10AEGeometryEvT_R5ArrayIS2_E`<br>void ArrayRemove<AEGeometry*>(AEGeometry*, Array<AEGeometry*>&) |
| 1314 | `0x0952c4` | `0x0952f4` | +48 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN10LODManager11forceUpdateEib`<br>LODManager::forceUpdate(int, bool) |
| 1315 | `0x095384` | `0x0953b4` | +48 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN10LODManager6updateEi`<br>LODManager::update(int) |
| 1316 | `0x09539c` | `0x0953cc` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9GeneratorC1Ev`<br>Generator::Generator() |
| 1317 | `0x09539c` | `0x0953cc` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9GeneratorC2Ev`<br>Generator::Generator() |
| 1318 | `0x09539e` | `0x0953ce` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9GeneratorD1Ev`<br>Generator::~Generator() |
| 1319 | `0x09539e` | `0x0953ce` | +48 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9GeneratorD2Ev`<br>Generator::~Generator() |
| 1320 | `0x0953a0` | `0x0953d0` | +48 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN9Generator18computerTradeGoodsEP7Station`<br>Generator::computerTradeGoods(Station*) |
| 1321 | `0x095404` | `0x095434` | +48 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN9Generator18isKaamoSpecialItemEi`<br>Generator::isKaamoSpecialItem(int) |
| 1322 | `0x095428` | `0x095458` | +48 | 1840 | 1840 | 0 | 100.0% |  | Y | `_ZN9Generator14getItemBuyListEP7Station`<br>Generator::getItemBuyList(Station*) |
| 1323 | `0x095b58` | `0x095b88` | +48 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN5ArrayIP4ItemEC2Ev`<br>Array<Item*>::Array() |
| 1324 | `0x095b74` | `0x095ba4` | +48 | 52 | 52 | 0 | 100.0% | Y | Y | `_Z14ArraySetLengthIP4ItemEvjR5ArrayIT_E`<br>void ArraySetLength<Item*>(unsigned int, Array<Item*>&) |
| 1325 | `0x095ba8` | `0x095bd8` | +48 | 36 | 36 | 0 | 100.0% | Y | Y | `_Z8ArrayAddIP4ItemEvT_R5ArrayIS2_E`<br>void ArrayAdd<Item*>(Item*, Array<Item*>&) |
| 1326 | `0x095bcc` | `0x095bfc` | +48 | 2244 | 2244 | 0 | 100.0% |  | Y | `_ZN9Generator14getShipBuyListEP7Station`<br>Generator::getShipBuyList(Station*) |
| 1327 | `0x096490` | `0x0964c0` | +48 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN5ArrayIP4ShipEC2Ev`<br>Array<Ship*>::Array() |
| 1328 | `0x0964ac` | `0x0964dc` | +48 | 36 | 36 | 0 | 100.0% | Y | Y | `_Z8ArrayAddIP4ShipEvT_R5ArrayIS2_E`<br>void ArrayAdd<Ship*>(Ship*, Array<Ship*>&) |
| 1329 | `0x0964d0` | `0x096500` | +48 | 52 | 52 | 0 | 100.0% | Y | Y | `_Z14ArraySetLengthIP4ShipEvjR5ArrayIT_E`<br>void ArraySetLength<Ship*>(unsigned int, Array<Ship*>&) |
| 1330 | `0x096504` | `0x096534` | +48 | 500 | 500 | 0 | 100.0% |  | Y | `_ZN9Generator11getLootListEii`<br>Generator::getLootList(int, int) |
| 1331 | `0x0966f8` | `0x096728` | +48 | 52 | 52 | 0 | 100.0% | Y | Y | `_Z14ArraySetLengthIiEvjR5ArrayIT_E`<br>void ArraySetLength<int>(unsigned int, Array<int>&) |
| 1332 | `0x09672c` | `0x09675c` | +48 | 344 | 344 | 0 | 100.0% |  | Y | `_ZN9Generator20generateStationIndexEP5ArrayIP11SolarSystemEi`<br>Generator::generateStationIndex(Array<SolarSystem*>*, int) |
| 1333 | `0x096884` | `0x0968b4` | +48 | 1464 | 1464 | 0 | 100.0% |  | Y | `_ZN9Generator12createAgentsEP7Station`<br>Generator::createAgents(Station*) |
| 1334 | `0x096e3c` | `0x096e6c` | +48 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN5ArrayIP5AgentEC2Ev`<br>Array<Agent*>::Array() |
| 1335 | `0x096e58` | `0x096e88` | +48 | 52 | 52 | 0 | 100.0% | Y | Y | `_Z14ArraySetLengthIP5AgentEvjR5ArrayIT_E`<br>void ArraySetLength<Agent*>(unsigned int, Array<Agent*>&) |
| 1336 | `0x096e8c` | `0x096ebc` | +48 | 812 | 812 | 0 | 100.0% |  | Y | `_ZN9Generator11createAgentEP7Station`<br>Generator::createAgent(Station*) |
| 1337 | `0x0971b8` | `0x0971e8` | +48 | 1996 | 1992 | -4 | 73.8% |  |  | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`<br>Generator::createMission(Agent*, Array<SolarSystem*>*) |
| 1338 | `0x097984` | `0x0979b0` | +44 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN6MTitleC2Ev`<br>MTitle::MTitle() |
| 1339 | `0x097984` | `0x0979b0` | +44 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN6MTitleC1Ev`<br>MTitle::MTitle() |
| 1340 | `0x097998` | `0x0979c4` | +44 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN6MTitleD2Ev`<br>MTitle::~MTitle() |
| 1341 | `0x097998` | `0x0979c4` | +44 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN6MTitleD1Ev`<br>MTitle::~MTitle() |
| 1342 | `0x0979bc` | `0x0979e8` | +44 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN6MTitleD0Ev`<br>MTitle::~MTitle() |
| 1343 | `0x0979cc` | `0x0979f8` | +44 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN6MTitle12OnInitializeEv`<br>MTitle::OnInitialize() |
| 1344 | `0x097a20` | `0x097a4c` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN6MTitle12OnTouchBeginEii`<br>MTitle::OnTouchBegin(int, int) |
| 1345 | `0x097a22` | `0x097a4e` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN6MTitle11OnTouchMoveEii`<br>MTitle::OnTouchMove(int, int) |
| 1346 | `0x097a24` | `0x097a50` | +44 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN6MTitle10OnTouchEndEii`<br>MTitle::OnTouchEnd(int, int) |
| 1347 | `0x097a2c` | `0x097a58` | +44 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN6MTitle9OnReleaseEv`<br>MTitle::OnRelease() |
| 1348 | `0x097a88` | `0x097ab4` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN6MTitle10OnKeyPressExx`<br>MTitle::OnKeyPress(long long, long long) |
| 1349 | `0x097a8a` | `0x097ab6` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN6MTitle8OnUpdateEv`<br>MTitle::OnUpdate() |
| 1350 | `0x097a8c` | `0x097ab8` | +44 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN6MTitle10OnRender2DEv`<br>MTitle::OnRender2D() |
| 1351 | `0x097bc4` | `0x097bf0` | +44 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN6MTitle10OnRender3DEv`<br>MTitle::OnRender3D() |
| 1352 | `0x097bec` | `0x097c18` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN6MTitle12OnKeyReleaseExx`<br>MTitle::OnKeyRelease(long long, long long) |
| 1353 | `0x097bee` | `0x097c1a` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18IApplicationModule12OnTouchBeginEiiPv`<br>AbyssEngine::IApplicationModule::OnTouchBegin(int, int, void*) |
| 1354 | `0x097bf0` | `0x097c1c` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18IApplicationModule11OnTouchMoveEiiPv`<br>AbyssEngine::IApplicationModule::OnTouchMove(int, int, void*) |
| 1355 | `0x097bf2` | `0x097c1e` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18IApplicationModule10OnTouchEndEiiPv`<br>AbyssEngine::IApplicationModule::OnTouchEnd(int, int, void*) |
| 1356 | `0x097bf4` | `0x097c20` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18IApplicationModule9OnSuspendEv`<br>AbyssEngine::IApplicationModule::OnSuspend() |
| 1357 | `0x097bf6` | `0x097c22` | +44 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine18IApplicationModule8OnResumeEv`<br>AbyssEngine::IApplicationModule::OnResume() |
| 1358 | `0x097bf8` | `0x097c24` | +44 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6MTitle17ShowLoadingScreenEv`<br>MTitle::ShowLoadingScreen() |
| 1359 | `0x097bfc` | `0x097c28` | +44 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN9ObjectiveC2EiiP5Level`<br>Objective::Objective(int, int, Level*) |
| 1360 | `0x097bfc` | `0x097c28` | +44 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN9ObjectiveC1EiiP5Level`<br>Objective::Objective(int, int, Level*) |
| 1361 | `0x097c0e` | `0x097c3a` | +44 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN9ObjectiveC2EiiiP5Level`<br>Objective::Objective(int, int, int, Level*) |
| 1362 | `0x097c0e` | `0x097c3a` | +44 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN9ObjectiveC1EiiiP5Level`<br>Objective::Objective(int, int, int, Level*) |
| 1363 | `0x097c2a` | `0x097c56` | +44 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN9ObjectiveD1Ev`<br>Objective::~Objective() |
| 1364 | `0x097c2a` | `0x097c56` | +44 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN9ObjectiveD2Ev`<br>Objective::~Objective() |
| 1365 | `0x097c5c` | `0x097c88` | +44 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP9ObjectiveEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Objective*>(Array<Objective*>&) |
| 1366 | `0x097cac` | `0x097cd8` | +44 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9ObjectiveED2Ev`<br>Array<Objective*>::~Array() |
| 1367 | `0x097cc2` | `0x097cee` | +44 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN9Objective12addObjectiveEPS_`<br>Objective::addObjective(Objective*) |
| 1368 | `0x097cfe` | `0x097d2a` | +44 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9ObjectiveEC2Ev`<br>Array<Objective*>::Array() |
| 1369 | `0x097d1a` | `0x097d46` | +44 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP9ObjectiveEvT_R5ArrayIS2_E`<br>void ArrayAdd<Objective*>(Objective*, Array<Objective*>&) |
| 1370 | `0x097d40` | `0x097d6c` | +44 | 884 | 884 | 0 | 100.0% |  | Y | `_ZN9Objective8achievedEi`<br>Objective::achieved(int) |
| 1371 | `0x0980b4` | `0x0980e0` | +44 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN9Objective12getCalcValueEv`<br>Objective::getCalcValue() |
| 1372 | `0x0980c0` | `0x0980ec` | +44 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN9Objective19isSurvivalObjectiveEv`<br>Objective::isSurvivalObjective() |
| 1373 | `0x0980cc` | `0x0980f8` | +44 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN9Objective15setAchievedTextEPN11AbyssEngine6StringE`<br>Objective::setAchievedText(AbyssEngine::String*) |
| 1374 | `0x0980fe` | `0x09812a` | +44 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9Objective15getAchievedTextEv`<br>Objective::getAchievedText() |
| 1375 | `0x098104` | `0x098130` | +44 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11BoundingAABC2Efffffffff`<br>BoundingAAB::BoundingAAB(float, float, float, float, float, float, float, float, float) |
| 1376 | `0x098104` | `0x098130` | +44 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11BoundingAABC1Efffffffff`<br>BoundingAAB::BoundingAAB(float, float, float, float, float, float, float, float, float) |
| 1377 | `0x098198` | `0x0981c4` | +44 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11BoundingAABD2Ev`<br>BoundingAAB::~BoundingAAB() |
| 1378 | `0x098198` | `0x0981c4` | +44 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11BoundingAABD1Ev`<br>BoundingAAB::~BoundingAAB() |
| 1379 | `0x09819c` | `0x0981c8` | +44 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN11BoundingAAB7collideEfff`<br>BoundingAAB::collide(float, float, float) |
| 1380 | `0x0981d2` | `0x0981fe` | +44 | 158 | 158 | 0 | 100.0% | Y | Y | `_ZN11BoundingAAB12outerCollideEfff`<br>BoundingAAB::outerCollide(float, float, float) |
| 1381 | `0x098270` | `0x09829c` | +44 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN11BoundingAAB18getCollisionNormalERKN11AbyssEngine6AEMath6VectorE`<br>BoundingAAB::getCollisionNormal(AbyssEngine::AEMath::Vector const&) |
| 1382 | `0x09827a` | `0x0982a6` | +44 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN11BoundingAAB6updateEfff`<br>BoundingAAB::update(float, float, float) |
| 1383 | `0x098280` | `0x0982ac` | +44 | 356 | 356 | 0 | 100.0% |  | Y | `_ZN11BoundingAAB25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE`<br>BoundingAAB::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) |
| 1384 | `0x0983e4` | `0x098410` | +44 | 60 | 60 | 0 | 100.0% | Y | Y | `_ZN8CutSceneC1Ei`<br>CutScene::CutScene(int) |
| 1385 | `0x0983e4` | `0x098410` | +44 | 60 | 60 | 0 | 100.0% | Y | Y | `_ZN8CutSceneC2Ei`<br>CutScene::CutScene(int) |
| 1386 | `0x098420` | `0x09844c` | +44 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN8CutSceneD1Ev`<br>CutScene::~CutScene() |
| 1387 | `0x098420` | `0x09844c` | +44 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN8CutSceneD2Ev`<br>CutScene::~CutScene() |
| 1388 | `0x0984c4` | `0x0984f0` | +44 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP10AEGeometryEvR5ArrayIT_E`<br>void ArrayReleaseClasses<AEGeometry*>(Array<AEGeometry*>&) |
| 1389 | `0x098514` | `0x098540` | +44 | 276 | 276 | 0 | 100.0% |  | Y | `_ZN8CutScene11resetCameraEv`<br>CutScene::resetCamera() |
| 1390 | `0x098628` | `0x098654` | +44 | 1088 | 1088 | 0 | 100.0% |  | Y | `_ZN8CutScene10initializeEv`<br>CutScene::initialize() |
| 1391 | `0x098a68` | `0x098a94` | +44 | 1048 | 1048 | 0 | 100.0% |  | Y | `_ZN8CutScene14checkForTurretEv`<br>CutScene::checkForTurret() |
| 1392 | `0x098e80` | `0x098eac` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8CutScene13isInitializedEv`<br>CutScene::isInitialized() |
| 1393 | `0x098e86` | `0x098eb2` | +44 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN8CutScene6updateEi`<br>CutScene::update(int) |
| 1394 | `0x098e8c` | `0x098eb8` | +44 | 2008 | 2008 | 0 | 100.0% |  | Y | `_ZN8CutScene7processEi`<br>CutScene::process(int) |
| 1395 | `0x099664` | `0x099690` | +44 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN8CutScene6updateEv`<br>CutScene::update() |
| 1396 | `0x099668` | `0x099694` | +44 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN8CutScene17replacePlayerShipEii`<br>CutScene::replacePlayerShip(int, int) |
| 1397 | `0x099788` | `0x0997b4` | +44 | 72 | 72 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIPN11AbyssEngine6AEMath6VectorEEvR5ArrayIT_E`<br>void ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(Array<AbyssEngine::AEMath::Vector*>&) |
| 1398 | `0x0997d0` | `0x0997fc` | +44 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine6AEMath6VectorEED2Ev`<br>Array<AbyssEngine::AEMath::Vector*>::~Array() |
| 1399 | `0x0997e6` | `0x099812` | +44 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP5ArrayIPN11AbyssEngine6AEMath6VectorEEEvRS0_IT_E`<br>void ArrayReleaseClasses<Array<AbyssEngine::AEMath::Vector*>*>(Array<Array<AbyssEngine::AEMath::Vector*>*>&) |
| 1400 | `0x099836` | `0x099862` | +44 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IPN11AbyssEngine6AEMath6VectorEEED2Ev`<br>Array<Array<AbyssEngine::AEMath::Vector*>*>::~Array() |
| 1401 | `0x09984c` | `0x099878` | +44 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8CutScene8render2DEv`<br>CutScene::render2D() |
| 1402 | `0x099852` | `0x09987e` | +44 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN8CutScene8renderBGEv`<br>CutScene::renderBG() |
| 1403 | `0x09985c` | `0x099888` | +44 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN8CutScene8render3DEv`<br>CutScene::render3D() |
| 1404 | `0x0998cc` | `0x0998f8` | +44 | 2008 | 2008 | 0 | 100.0% |  | Y | `_ZN9PlayerEgoC2EP6Player`<br>PlayerEgo::PlayerEgo(Player*) |
| 1405 | `0x0998cc` | `0x0998f8` | +44 | 2008 | 2008 | 0 | 100.0% |  | Y | `_ZN9PlayerEgoC1EP6Player`<br>PlayerEgo::PlayerEgo(Player*) |
| 1406 | `0x09a0a4` | `0x09a0d0` | +44 | 338 | 338 | 0 | 100.0% |  | Y | `_ZN9PlayerEgoD2Ev`<br>PlayerEgo::~PlayerEgo() |
| 1407 | `0x09a0a4` | `0x09a0d0` | +44 | 338 | 338 | 0 | 100.0% |  | Y | `_ZN9PlayerEgoD1Ev`<br>PlayerEgo::~PlayerEgo() |
| 1408 | `0x09a1f6` | `0x09a222` | +44 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15EaseInOutMatrixD2Ev`<br>AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix() |
| 1409 | `0x09a20e` | `0x09a23a` | +44 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP10RepairBeamEvR5ArrayIT_E`<br>void ArrayReleaseClasses<RepairBeam*>(Array<RepairBeam*>&) |
| 1410 | `0x09a25e` | `0x09a28a` | +44 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP10RepairBeamED2Ev`<br>Array<RepairBeam*>::~Array() |
| 1411 | `0x09a274` | `0x09a2a0` | +44 | 600 | 600 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo7setShipEii`<br>PlayerEgo::setShip(int, int) |
| 1412 | `0x09a4cc` | `0x09a4f8` | +44 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP10RepairBeamEC2Ev`<br>Array<RepairBeam*>::Array() |
| 1413 | `0x09a4e8` | `0x09a514` | +44 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP10RepairBeamEvT_R5ArrayIS2_E`<br>void ArrayAdd<RepairBeam*>(RepairBeam*, Array<RepairBeam*>&) |
| 1414 | `0x09a50c` | `0x09a538` | +44 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo17setExhaustVisibleEb`<br>PlayerEgo::setExhaustVisible(bool) |
| 1415 | `0x09a550` | `0x09a57c` | +44 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo15PlayEngineSoundEv`<br>PlayerEgo::PlayEngineSound() |
| 1416 | `0x09a5dc` | `0x09a608` | +44 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16PauseEngineSoundEv`<br>PlayerEgo::PauseEngineSound() |
| 1417 | `0x09a5e2` | `0x09a60e` | +44 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo17ResumeEngineSoundEv`<br>PlayerEgo::ResumeEngineSound() |
| 1418 | `0x09a5ec` | `0x09a618` | +44 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo15StopEngineSoundEv`<br>PlayerEgo::StopEngineSound() |
| 1419 | `0x09a684` | `0x09a6b0` | +44 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo22isDockedToDockingPointEv`<br>PlayerEgo::isDockedToDockingPoint() |
| 1420 | `0x09a69e` | `0x09a6ca` | +44 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo13resetMovementEv`<br>PlayerEgo::resetMovement() |
| 1421 | `0x09a6b2` | `0x09a6de` | +44 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo11resetLastHPEv`<br>PlayerEgo::resetLastHP() |
| 1422 | `0x09a6c4` | `0x09a6f0` | +44 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14startJumpDriveEv`<br>PlayerEgo::startJumpDrive() |
| 1423 | `0x09a708` | `0x09a734` | +44 | 680 | 680 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14toggleCloakingEv`<br>PlayerEgo::toggleCloaking() |
| 1424 | `0x09a9b0` | `0x09a9dc` | +44 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo8hasCloakEv`<br>PlayerEgo::hasCloak() |
| 1425 | `0x09a9bc` | `0x09a9e8` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo16hasVolatileGoodsEv`<br>PlayerEgo::hasVolatileGoods() |
| 1426 | `0x09a9c4` | `0x09a9f0` | +44 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo16getVolatileForceEv`<br>PlayerEgo::getVolatileForce() |
| 1427 | `0x09a9f8` | `0x09aa24` | +44 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo20addNukeVolatileForceEf`<br>PlayerEgo::addNukeVolatileForce(float) |
| 1428 | `0x09aa10` | `0x09aa3c` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo14isInTurretModeEv`<br>PlayerEgo::isInTurretMode() |
| 1429 | `0x09aa18` | `0x09aa44` | +44 | 376 | 376 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16setDockingCameraEv`<br>PlayerEgo::setDockingCamera() |
| 1430 | `0x09ab90` | `0x09abbc` | +44 | 584 | 584 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo13setTurretModeEb`<br>PlayerEgo::setTurretMode(bool) |
| 1431 | `0x09add8` | `0x09ae04` | +44 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo8isMiningEv`<br>PlayerEgo::isMining() |
| 1432 | `0x09ade4` | `0x09ae10` | +44 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo17isInRocketControlEv`<br>PlayerEgo::isInRocketControl() |
| 1433 | `0x09adf0` | `0x09ae1c` | +44 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12stopShootingEi`<br>PlayerEgo::stopShooting(int) |
| 1434 | `0x09ae34` | `0x09ae60` | +44 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo9stopBoostEv`<br>PlayerEgo::stopBoost() |
| 1435 | `0x09ae80` | `0x09aeac` | +44 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo16isBoostRefreshedEv`<br>PlayerEgo::isBoostRefreshed() |
| 1436 | `0x09aea4` | `0x09aed0` | +44 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo5boostEv`<br>PlayerEgo::boost() |
| 1437 | `0x09af00` | `0x09af2c` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo13getBoostSpeedEv`<br>PlayerEgo::getBoostSpeed() |
| 1438 | `0x09af06` | `0x09af32` | +44 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo10forceBoostEv`<br>PlayerEgo::forceBoost() |
| 1439 | `0x09af24` | `0x09af50` | +44 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo22getCurrentMiningAmountEv`<br>PlayerEgo::getCurrentMiningAmount() |
| 1440 | `0x09af32` | `0x09af5e` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo8boostingEv`<br>PlayerEgo::boosting() |
| 1441 | `0x09af38` | `0x09af64` | +44 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo12readyToBoostEv`<br>PlayerEgo::readyToBoost() |
| 1442 | `0x09af48` | `0x09af74` | +44 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo18getBoostPercentageEv`<br>PlayerEgo::getBoostPercentage() |
| 1443 | `0x09afa0` | `0x09afcc` | +44 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo12getBoostRateEv`<br>PlayerEgo::getBoostRate() |
| 1444 | `0x09afd0` | `0x09affc` | +44 | 62 | 62 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo12getCloakRateEv`<br>PlayerEgo::getCloakRate() |
| 1445 | `0x09b00e` | `0x09b03a` | +44 | 44 | 44 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo18getDriveChargeRateEv`<br>PlayerEgo::getDriveChargeRate() |
| 1446 | `0x09b03a` | `0x09b066` | +44 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo10driveReadyEv`<br>PlayerEgo::driveReady() |
| 1447 | `0x09b048` | `0x09b074` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo15isChargingDriveEv`<br>PlayerEgo::isChargingDrive() |
| 1448 | `0x09b04e` | `0x09b07a` | +44 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo18resetChargingDriveEv`<br>PlayerEgo::resetChargingDrive() |
| 1449 | `0x09b056` | `0x09b082` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo15isChargingCloakEv`<br>PlayerEgo::isChargingCloak() |
| 1450 | `0x09b05c` | `0x09b088` | +44 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo17isRechargingCloakEv`<br>PlayerEgo::isRechargingCloak() |
| 1451 | `0x09b06a` | `0x09b096` | +44 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo20getCloakRechargeRateEv`<br>PlayerEgo::getCloakRechargeRate() |
| 1452 | `0x09b08c` | `0x09b0b8` | +44 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo13readyForCloakEv`<br>PlayerEgo::readyForCloak() |
| 1453 | `0x09b0aa` | `0x09b0d6` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo9isCloakedEv`<br>PlayerEgo::isCloaked() |
| 1454 | `0x09b0b0` | `0x09b0dc` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo10setCollideEb`<br>PlayerEgo::setCollide(bool) |
| 1455 | `0x09b0b8` | `0x09b0e4` | +44 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo8setLevelEP5Level`<br>PlayerEgo::setLevel(Level*) |
| 1456 | `0x09b150` | `0x09b17c` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo8setRouteEP5Route`<br>PlayerEgo::setRoute(Route*) |
| 1457 | `0x09b156` | `0x09b182` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo8getRouteEv`<br>PlayerEgo::getRoute() |
| 1458 | `0x09b15c` | `0x09b188` | +44 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo11removeRouteEv`<br>PlayerEgo::removeRoute() |
| 1459 | `0x09b178` | `0x09b1a4` | +44 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo14levelCollisionEv`<br>PlayerEgo::levelCollision() |
| 1460 | `0x09b17c` | `0x09b1a8` | +44 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo6isDeadEv`<br>PlayerEgo::isDead() |
| 1461 | `0x09b190` | `0x09b1bc` | +44 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12getHitpointsEv`<br>PlayerEgo::getHitpoints() |
| 1462 | `0x09b198` | `0x09b1c4` | +44 | 260 | 260 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo5shootEii`<br>PlayerEgo::shoot(int, int) |
| 1463 | `0x09b29c` | `0x09b2c8` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo13hasAutoTurretEv`<br>PlayerEgo::hasAutoTurret() |
| 1464 | `0x09b2a2` | `0x09b2ce` | +44 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo13setAutoTurretEb`<br>PlayerEgo::setAutoTurret(bool) |
| 1465 | `0x09b2b4` | `0x09b2e0` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo19autoTurretIsEnabledEv`<br>PlayerEgo::autoTurretIsEnabled() |
| 1466 | `0x09b2bc` | `0x09b2e8` | +44 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo17setTurretPositionEN11AbyssEngine6AEMath6VectorE`<br>PlayerEgo::setTurretPosition(AbyssEngine::AEMath::Vector) |
| 1467 | `0x09b2ec` | `0x09b318` | +44 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo17getTurretPositionEv`<br>PlayerEgo::getTurretPosition() |
| 1468 | `0x09b340` | `0x09b36c` | +44 | 1080 | 1080 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14checkForTurretEv`<br>PlayerEgo::checkForTurret() |
| 1469 | `0x09b778` | `0x09b7a4` | +44 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo6addGunEP3Guni`<br>PlayerEgo::addGun(Gun*, int) |
| 1470 | `0x09b78e` | `0x09b7ba` | +44 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo6addGunEP5ArrayIP3GunEi`<br>PlayerEgo::addGun(Array<Gun*>*, int) |
| 1471 | `0x09b7a4` | `0x09b7d0` | +44 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo17getHullDamageRateEv`<br>PlayerEgo::getHullDamageRate() |
| 1472 | `0x09b7aa` | `0x09b7d6` | +44 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo19getShieldDamageRateEv`<br>PlayerEgo::getShieldDamageRate() |
| 1473 | `0x09b7b0` | `0x09b7dc` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo30getCurrentSecondaryWeaponIndexEv`<br>PlayerEgo::getCurrentSecondaryWeaponIndex() |
| 1474 | `0x09b7b8` | `0x09b7e4` | +44 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo30setCurrentSecondaryWeaponIndexEi`<br>PlayerEgo::setCurrentSecondaryWeaponIndex(int) |
| 1475 | `0x09b7d0` | `0x09b7fc` | +44 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo11GetUpVectorEv`<br>PlayerEgo::GetUpVector() |
| 1476 | `0x09b7dc` | `0x09b808` | +44 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12GetDirVectorEv`<br>PlayerEgo::GetDirVector() |
| 1477 | `0x09b7e8` | `0x09b814` | +44 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo11getPositionEv`<br>PlayerEgo::getPosition() |
| 1478 | `0x09b7f4` | `0x09b820` | +44 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo9setActiveEb`<br>PlayerEgo::setActive(bool) |
| 1479 | `0x09b7fa` | `0x09b826` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo21setComputerControlledEb`<br>PlayerEgo::setComputerControlled(bool) |
| 1480 | `0x09b800` | `0x09b82c` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo9setFreezeEb`<br>PlayerEgo::setFreeze(bool) |
| 1481 | `0x09b808` | `0x09b834` | +44 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12setAutoPilotEP8KIPlayer`<br>PlayerEgo::setAutoPilot(KIPlayer*) |
| 1482 | `0x09b86c` | `0x09b898` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo9setThrustEf`<br>PlayerEgo::setThrust(float) |
| 1483 | `0x09b872` | `0x09b89e` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo18getAutoPilotTargetEv`<br>PlayerEgo::getAutoPilotTarget() |
| 1484 | `0x09b878` | `0x09b8a4` | +44 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo13goingToPlanetEv`<br>PlayerEgo::goingToPlanet() |
| 1485 | `0x09b8a4` | `0x09b8d0` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo11isAutoPilotEv`<br>PlayerEgo::isAutoPilot() |
| 1486 | `0x09b8aa` | `0x09b8d6` | +44 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo13goingToStreamEv`<br>PlayerEgo::goingToStream() |
| 1487 | `0x09b8c8` | `0x09b8f4` | +44 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14goingToStationEv`<br>PlayerEgo::goingToStation() |
| 1488 | `0x09b8f6` | `0x09b922` | +44 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo15goingToWaypointEv`<br>PlayerEgo::goingToWaypoint() |
| 1489 | `0x09b8fc` | `0x09b928` | +44 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo15goingToWormholeEv`<br>PlayerEgo::goingToWormhole() |
| 1490 | `0x09b91c` | `0x09b948` | +44 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo11setPositionEfff`<br>PlayerEgo::setPosition(float, float, float) |
| 1491 | `0x09b948` | `0x09b974` | +44 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo11setPositionEN11AbyssEngine6AEMath6VectorE`<br>PlayerEgo::setPosition(AbyssEngine::AEMath::Vector) |
| 1492 | `0x09b94c` | `0x09b978` | +44 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12isInWormholeEv`<br>PlayerEgo::isInWormhole() |
| 1493 | `0x09b96c` | `0x09b998` | +44 | 652 | 652 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16handleTurretViewEi`<br>PlayerEgo::handleTurretView(int) |
| 1494 | `0x09bbf8` | `0x09bc24` | +44 | 500 | 500 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo4rollEi`<br>PlayerEgo::roll(int) |
| 1495 | `0x09bdf0` | `0x09be18` | +40 | 896 | 896 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo10handleShipEi`<br>PlayerEgo::handleShip(int) |
| 1496 | `0x09c170` | `0x09c198` | +40 | 136 | 136 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo21getCloakingPercentageEv`<br>PlayerEgo::getCloakingPercentage() |
| 1497 | `0x09c1f8` | `0x09c220` | +40 | 1244 | 1244 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14updateManeuverEv`<br>PlayerEgo::updateManeuver() |
| 1498 | `0x09c6d4` | `0x09c6fc` | +40 | 932 | 932 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14moveToPositionEN11AbyssEngine6AEMath6VectorEbf`<br>PlayerEgo::moveToPosition(AbyssEngine::AEMath::Vector, bool, float) |
| 1499 | `0x09ca78` | `0x09caa0` | +40 | 956 | 956 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16handleAutoTurretEi`<br>PlayerEgo::handleAutoTurret(int) |
| 1500 | `0x09ce38` | `0x09ce60` | +40 | 8852 | 8852 | 0 | 96.9% |  |  | `_ZN9PlayerEgo6updateEiP5RadarP3HudP5RadioP11LevelScriptibi`<br>PlayerEgo::update(int, Radar*, Hud*, Radio*, LevelScript*, int, bool, int) |
| 1501 | `0x09f0cc` | `0x09f0f4` | +40 | 968 | 968 | 0 | 83.2% |  |  | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`<br>PlayerEgo::calcCollision(Array<KIPlayer*>*) |
| 1502 | `0x09f494` | `0x09f4bc` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo17isDockingToPlanetEv`<br>PlayerEgo::isDockingToPlanet() |
| 1503 | `0x09f49a` | `0x09f4c2` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo17isDockingToStreamEv`<br>PlayerEgo::isDockingToStream() |
| 1504 | `0x09f4a0` | `0x09f4c8` | +40 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14dockToAsteroidEP8KIPlayerP5Radar`<br>PlayerEgo::dockToAsteroid(KIPlayer*, Radar*) |
| 1505 | `0x09f570` | `0x09f598` | +40 | 1156 | 1156 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16approachAsteroidEP3HudiP5Radar`<br>PlayerEgo::approachAsteroid(Hud*, int, Radar*) |
| 1506 | `0x09f9f4` | `0x09fa1c` | +40 | 708 | 708 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo18dockToDockingPointEP8KIPlayerP5Radar`<br>PlayerEgo::dockToDockingPoint(KIPlayer*, Radar*) |
| 1507 | `0x09fcb8` | `0x09fce0` | +40 | 2532 | 2532 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo20approachDockingPointEP3HudiP5Radar`<br>PlayerEgo::approachDockingPoint(Hud*, int, Radar*) |
| 1508 | `0x0a069c` | `0x0a06c4` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo23isDockingToDockingPointEv`<br>PlayerEgo::isDockingToDockingPoint() |
| 1509 | `0x0a06b2` | `0x0a06da` | +40 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo20isLandingOrTakingOffEv`<br>PlayerEgo::isLandingOrTakingOff() |
| 1510 | `0x0a06ce` | `0x0a06f6` | +40 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN16ParticleSettingsUt_aSERKS0_`<br>ParticleSettings::{unnamed type#1}::operator=({unnamed type#1} const&) |
| 1511 | `0x0a06ec` | `0x0a0714` | +40 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo9hitCameraEv`<br>PlayerEgo::hitCamera() |
| 1512 | `0x0a0704` | `0x0a072c` | +40 | 584 | 584 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo5rightEif`<br>PlayerEgo::right(int, float) |
| 1513 | `0x0a094c` | `0x0a0974` | +40 | 580 | 580 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo4leftEif`<br>PlayerEgo::left(int, float) |
| 1514 | `0x0a0b90` | `0x0a0bb8` | +40 | 620 | 620 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo4downEif`<br>PlayerEgo::down(int, float) |
| 1515 | `0x0a0dfc` | `0x0a0e24` | +40 | 624 | 624 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo2upEif`<br>PlayerEgo::up(int, float) |
| 1516 | `0x0a106c` | `0x0a1094` | +40 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo25tryToStartEmergencySystemEv`<br>PlayerEgo::tryToStartEmergencySystem() |
| 1517 | `0x0a10f8` | `0x0a1120` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo21emergencySystemActiveEv`<br>PlayerEgo::emergencySystemActive() |
| 1518 | `0x0a1106` | `0x0a112e` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo19collidesWithStationEv`<br>PlayerEgo::collidesWithStation() |
| 1519 | `0x0a110c` | `0x0a1134` | +40 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo9isHackingEv`<br>PlayerEgo::isHacking() |
| 1520 | `0x0a1118` | `0x0a1140` | +40 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo10hackingWonEv`<br>PlayerEgo::hackingWon() |
| 1521 | `0x0a1126` | `0x0a114e` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo17deleteHackingGameEv`<br>PlayerEgo::deleteHackingGame() |
| 1522 | `0x0a1142` | `0x0a116a` | +40 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo23getHackingGameDockIndexEv`<br>PlayerEgo::getHackingGameDockIndex() |
| 1523 | `0x0a1152` | `0x0a117a` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo20isInDockingProcedureEv`<br>PlayerEgo::isInDockingProcedure() |
| 1524 | `0x0a1168` | `0x0a1190` | +40 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12turnVerticalEif`<br>PlayerEgo::turnVertical(int, float) |
| 1525 | `0x0a1194` | `0x0a11bc` | +40 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14turnHorizontalEif`<br>PlayerEgo::turnHorizontal(int, float) |
| 1526 | `0x0a11ae` | `0x0a11d6` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo16getRocketBankingEv`<br>PlayerEgo::getRocketBanking() |
| 1527 | `0x0a11b4` | `0x0a11dc` | +40 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo6strafeEib`<br>PlayerEgo::strafe(int, bool) |
| 1528 | `0x0a128c` | `0x0a12b4` | +40 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16hackingRotateRCWEv`<br>PlayerEgo::hackingRotateRCW() |
| 1529 | `0x0a12b8` | `0x0a12e0` | +40 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16hackingRotateLCWEv`<br>PlayerEgo::hackingRotateLCW() |
| 1530 | `0x0a12e4` | `0x0a130c` | +40 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14hackingShuffleEv`<br>PlayerEgo::hackingShuffle() |
| 1531 | `0x0a12f2` | `0x0a131a` | +40 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo14alignToHorizonEv`<br>PlayerEgo::alignToHorizon() |
| 1532 | `0x0a12fc` | `0x0a1324` | +40 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo6rotateEfff`<br>PlayerEgo::rotate(float, float, float) |
| 1533 | `0x0a137c` | `0x0a13a4` | +40 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo11setRotationEfff`<br>PlayerEgo::setRotation(float, float, float) |
| 1534 | `0x0a13e0` | `0x0a1408` | +40 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo7explodeEv`<br>PlayerEgo::explode() |
| 1535 | `0x0a14a4` | `0x0a14cc` | +40 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo6reviveEv`<br>PlayerEgo::revive() |
| 1536 | `0x0a1598` | `0x0a15c0` | +40 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo18startSmokeEmissionEv`<br>PlayerEgo::startSmokeEmission() |
| 1537 | `0x0a15c6` | `0x0a15ee` | +40 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12endExplosionEv`<br>PlayerEgo::endExplosion() |
| 1538 | `0x0a15d4` | `0x0a15fc` | +40 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo14explosionEndedEv`<br>PlayerEgo::explosionEnded() |
| 1539 | `0x0a15ee` | `0x0a1616` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo22aboutToReachAutoTargetEv`<br>PlayerEgo::aboutToReachAutoTarget() |
| 1540 | `0x0a15f4` | `0x0a161c` | +40 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo13resetGunDelayEv`<br>PlayerEgo::resetGunDelay() |
| 1541 | `0x0a15fc` | `0x0a1624` | +40 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14refillGunDelayEv`<br>PlayerEgo::refillGunDelay() |
| 1542 | `0x0a1604` | `0x0a162c` | +40 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo18isDockedToAsteroidEv`<br>PlayerEgo::isDockedToAsteroid() |
| 1543 | `0x0a161e` | `0x0a1646` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo19isDockingToAsteroidEv`<br>PlayerEgo::isDockingToAsteroid() |
| 1544 | `0x0a1634` | `0x0a165c` | +40 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo21isDockedToMiningPlantEv`<br>PlayerEgo::isDockedToMiningPlant() |
| 1545 | `0x0a167c` | `0x0a16a4` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo16isDockedToStreamEv`<br>PlayerEgo::isDockedToStream() |
| 1546 | `0x0a1684` | `0x0a16ac` | +40 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12dockToPlanetEv`<br>PlayerEgo::dockToPlanet() |
| 1547 | `0x0a16f4` | `0x0a171c` | +40 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo14stopPlanetDockEv`<br>PlayerEgo::stopPlanetDock() |
| 1548 | `0x0a1720` | `0x0a1748` | +40 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo16isDockedToPlanetEv`<br>PlayerEgo::isDockedToPlanet() |
| 1549 | `0x0a1732` | `0x0a175a` | +40 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12dockToStreamEb`<br>PlayerEgo::dockToStream(bool) |
| 1550 | `0x0a1772` | `0x0a179a` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo25shouldSwitchToStandardCamEv`<br>PlayerEgo::shouldSwitchToStandardCam() |
| 1551 | `0x0a1788` | `0x0a17b0` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo25shouldSwitchToFreeLookCamEv`<br>PlayerEgo::shouldSwitchToFreeLookCam() |
| 1552 | `0x0a179e` | `0x0a17c6` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo15setDockingStateEi`<br>PlayerEgo::setDockingState(int) |
| 1553 | `0x0a17b4` | `0x0a17dc` | +40 | 524 | 524 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo10stopMiningEv`<br>PlayerEgo::stopMining() |
| 1554 | `0x0a19c0` | `0x0a19e8` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo14lostMiningGameEv`<br>PlayerEgo::lostMiningGame() |
| 1555 | `0x0a19c6` | `0x0a19ee` | +40 | 82 | 82 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo16setRocketControlEP3GunP10AEGeometry`<br>PlayerEgo::setRocketControl(Gun*, AEGeometry*) |
| 1556 | `0x0a1a18` | `0x0a1a40` | +40 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo13killLiberatorEv`<br>PlayerEgo::killLiberator() |
| 1557 | `0x0a1abc` | `0x0a1ae4` | +40 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12initManeuverEi`<br>PlayerEgo::initManeuver(int) |
| 1558 | `0x0a1b78` | `0x0a1ba0` | +40 | 222 | 222 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo6renderEb`<br>PlayerEgo::render(bool) |
| 1559 | `0x0a1c56` | `0x0a1c7e` | +40 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo32hideShipForFirstPersonCameraViewEb`<br>PlayerEgo::hideShipForFirstPersonCameraView(bool) |
| 1560 | `0x0a1c88` | `0x0a1cb0` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo10setVisibleEb`<br>PlayerEgo::setVisible(bool) |
| 1561 | `0x0a1c98` | `0x0a1cc0` | +40 | 620 | 620 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo4drawEb`<br>PlayerEgo::draw(bool) |
| 1562 | `0x0a1f04` | `0x0a1f2c` | +40 | 496 | 496 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo12drawThrottleEv`<br>PlayerEgo::drawThrottle() |
| 1563 | `0x0a20f4` | `0x0a211c` | +40 | 58 | 58 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo15throttleChangedEv`<br>PlayerEgo::throttleChanged() |
| 1564 | `0x0a212e` | `0x0a2156` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo11getHandlingEv`<br>PlayerEgo::getHandling() |
| 1565 | `0x0a2134` | `0x0a215c` | +40 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo5shakeEi`<br>PlayerEgo::shake(int) |
| 1566 | `0x0a21e0` | `0x0a2208` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo6getHUDEv`<br>PlayerEgo::getHUD() |
| 1567 | `0x0a21e6` | `0x0a220e` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo16isInFreeLookModeEv`<br>PlayerEgo::isInFreeLookMode() |
| 1568 | `0x0a21ec` | `0x0a2214` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo15setFreeLookModeEb`<br>PlayerEgo::setFreeLookMode(bool) |
| 1569 | `0x0a21f2` | `0x0a221a` | +40 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo21setTargetFollowCameraEP18TargetFollowCamera`<br>PlayerEgo::setTargetFollowCamera(TargetFollowCamera*) |
| 1570 | `0x0a2202` | `0x0a222a` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo21getTargetFollowCameraEv`<br>PlayerEgo::getTargetFollowCamera() |
| 1571 | `0x0a2208` | `0x0a2230` | +40 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9PlayerEgo19pitchAllPrimaryGunsEf`<br>PlayerEgo::pitchAllPrimaryGuns(float) |
| 1572 | `0x0a220e` | `0x0a2236` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo8setSpeedEf`<br>PlayerEgo::setSpeed(float) |
| 1573 | `0x0a2214` | `0x0a223c` | +40 | 96 | 96 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo12changeThrustEf`<br>PlayerEgo::changeThrust(float) |
| 1574 | `0x0a2274` | `0x0a229c` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo9getThrustEv`<br>PlayerEgo::getThrust() |
| 1575 | `0x0a227a` | `0x0a22a2` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo8getSpeedEv`<br>PlayerEgo::getSpeed() |
| 1576 | `0x0a2280` | `0x0a22a8` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo18getDockTotalAmountEv`<br>PlayerEgo::getDockTotalAmount() |
| 1577 | `0x0a2286` | `0x0a22ae` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9PlayerEgo23getDockTransferedAmountEv`<br>PlayerEgo::getDockTransferedAmount() |
| 1578 | `0x0a228c` | `0x0a22b4` | +40 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN11TouchSliderC2Eiiif`<br>TouchSlider::TouchSlider(int, int, int, float) |
| 1579 | `0x0a228c` | `0x0a22b4` | +40 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN11TouchSliderC1Eiiif`<br>TouchSlider::TouchSlider(int, int, int, float) |
| 1580 | `0x0a232c` | `0x0a2354` | +40 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider11setPositionEii`<br>TouchSlider::setPosition(int, int) |
| 1581 | `0x0a2370` | `0x0a2398` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11TouchSliderD1Ev`<br>TouchSlider::~TouchSlider() |
| 1582 | `0x0a2370` | `0x0a2398` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11TouchSliderD2Ev`<br>TouchSlider::~TouchSlider() |
| 1583 | `0x0a2372` | `0x0a239a` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider8getWidthEv`<br>TouchSlider::getWidth() |
| 1584 | `0x0a2378` | `0x0a23a0` | +40 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11TouchSlider4drawEv`<br>TouchSlider::draw() |
| 1585 | `0x0a23c0` | `0x0a23e8` | +40 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider8getValueEv`<br>TouchSlider::getValue() |
| 1586 | `0x0a23ee` | `0x0a2416` | +40 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider8setValueEf`<br>TouchSlider::setValue(float) |
| 1587 | `0x0a2422` | `0x0a244a` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider13setFixedScaleEi`<br>TouchSlider::setFixedScale(int) |
| 1588 | `0x0a2426` | `0x0a244e` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider18setHalfTransparentEb`<br>TouchSlider::setHalfTransparent(bool) |
| 1589 | `0x0a242c` | `0x0a2454` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11TouchSlider12OnTouchBeginEii`<br>TouchSlider::OnTouchBegin(int, int) |
| 1590 | `0x0a2448` | `0x0a2470` | +40 | 76 | 76 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider13touchedInsideEii`<br>TouchSlider::touchedInside(int, int) |
| 1591 | `0x0a2494` | `0x0a24bc` | +40 | 72 | 72 | 0 | 100.0% | Y | Y | `_ZN11TouchSlider11OnTouchMoveEii`<br>TouchSlider::OnTouchMove(int, int) |
| 1592 | `0x0a24dc` | `0x0a2504` | +40 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11TouchSlider10OnTouchEndEii`<br>TouchSlider::OnTouchEnd(int, int) |
| 1593 | `0x0a2570` | `0x0a2598` | +40 | 448 | 448 | 0 | 100.0% |  | Y | `_ZN6PlayerC2Eiiiii`<br>Player::Player(int, int, int, int, int) |
| 1594 | `0x0a2570` | `0x0a2598` | +40 | 448 | 448 | 0 | 100.0% |  | Y | `_ZN6PlayerC1Eiiiii`<br>Player::Player(int, int, int, int, int) |
| 1595 | `0x0a2730` | `0x0a2758` | +40 | 136 | 136 | 0 | 100.0% | Y | Y | `_ZN6Player16updateDamageRateEv`<br>Player::updateDamageRate() |
| 1596 | `0x0a27b8` | `0x0a27e0` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IP3GunEEC2Ev`<br>Array<Array<Gun*>*>::Array() |
| 1597 | `0x0a27d4` | `0x0a27fc` | +40 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP5ArrayIP3GunEEvjRS0_IT_E`<br>void ArraySetLength<Array<Gun*>*>(unsigned int, Array<Array<Gun*>*>&) |
| 1598 | `0x0a2808` | `0x0a2830` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP3GunEC2Ev`<br>Array<Gun*>::Array() |
| 1599 | `0x0a2824` | `0x0a284c` | +40 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP3GunEvjR5ArrayIT_E`<br>void ArraySetLength<Gun*>(unsigned int, Array<Gun*>&) |
| 1600 | `0x0a2858` | `0x0a2880` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN6Player11getPositionEv`<br>Player::getPosition() |
| 1601 | `0x0a2864` | `0x0a288c` | +40 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN6PlayerD1Ev`<br>Player::~Player() |
| 1602 | `0x0a2864` | `0x0a288c` | +40 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN6PlayerD2Ev`<br>Player::~Player() |
| 1603 | `0x0a28e4` | `0x0a290c` | +40 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP3GunEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Gun*>(Array<Gun*>&) |
| 1604 | `0x0a2934` | `0x0a295c` | +40 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP3GunED2Ev`<br>Array<Gun*>::~Array() |
| 1605 | `0x0a294a` | `0x0a2972` | +40 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP5ArrayIP3GunEEvRS0_IT_E`<br>void ArrayReleaseClasses<Array<Gun*>*>(Array<Array<Gun*>*>&) |
| 1606 | `0x0a299a` | `0x0a29c2` | +40 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IP3GunEED2Ev`<br>Array<Array<Gun*>*>::~Array() |
| 1607 | `0x0a29b0` | `0x0a29d8` | +40 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP6PlayerED2Ev`<br>Array<Player*>::~Array() |
| 1608 | `0x0a29c6` | `0x0a29ee` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player18setShootingEnabledEb`<br>Player::setShootingEnabled(bool) |
| 1609 | `0x0a29cc` | `0x0a29f4` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN6Player10isAsteroidEv`<br>Player::isAsteroid() |
| 1610 | `0x0a29e2` | `0x0a2a0a` | +40 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN6Player10isGasCloudEv`<br>Player::isGasCloud() |
| 1611 | `0x0a29f8` | `0x0a2a20` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player14setNeverAttackEb`<br>Player::setNeverAttack(bool) |
| 1612 | `0x0a29fe` | `0x0a2a26` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player15doesNeverAttackEv`<br>Player::doesNeverAttack() |
| 1613 | `0x0a2a04` | `0x0a2a2c` | +40 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN6Player14setAlwaysEnemyEb`<br>Player::setAlwaysEnemy(bool) |
| 1614 | `0x0a2a14` | `0x0a2a3c` | +40 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN6Player15setAlwaysFriendEb`<br>Player::setAlwaysFriend(bool) |
| 1615 | `0x0a2a28` | `0x0a2a50` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player14isAlwaysFriendEv`<br>Player::isAlwaysFriend() |
| 1616 | `0x0a2a2e` | `0x0a2a56` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player13isAlwaysEnemyEv`<br>Player::isAlwaysEnemy() |
| 1617 | `0x0a2a34` | `0x0a2a5c` | +40 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN6Player10setEmpDataEii`<br>Player::setEmpData(int, int) |
| 1618 | `0x0a2a56` | `0x0a2a7e` | +40 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN6Player17setPlayShootSoundEbi`<br>Player::setPlayShootSound(bool, int) |
| 1619 | `0x0a2a60` | `0x0a2a88` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Player9setRadiusEi`<br>Player::setRadius(int) |
| 1620 | `0x0a2a64` | `0x0a2a8c` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player11setKIPlayerEP8KIPlayer`<br>Player::setKIPlayer(KIPlayer*) |
| 1621 | `0x0a2a6a` | `0x0a2a92` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player11getKIPlayerEv`<br>Player::getKIPlayer() |
| 1622 | `0x0a2a70` | `0x0a2a98` | +40 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN6Player5resetEv`<br>Player::reset() |
| 1623 | `0x0a2acc` | `0x0a2af4` | +40 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN6Player10setEnemiesEP5ArrayIPS_E`<br>Player::setEnemies(Array<Player*>*) |
| 1624 | `0x0a2b50` | `0x0a2b78` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP6PlayerEC2Ev`<br>Array<Player*>::Array() |
| 1625 | `0x0a2b6c` | `0x0a2b94` | +40 | 10 | 10 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP6PlayerEvRK5ArrayIT_ERS4_`<br>void ArrayAdd<Player*>(Array<Player*> const&, Array<Player*>&) |
| 1626 | `0x0a2b76` | `0x0a2b9e` | +40 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN6Player8setEnemyEPS_`<br>Player::setEnemy(Player*) |
| 1627 | `0x0a2bbe` | `0x0a2be6` | +40 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP6PlayerEvT_R5ArrayIS2_E`<br>void ArrayAdd<Player*>(Player*, Array<Player*>&) |
| 1628 | `0x0a2be2` | `0x0a2c0a` | +40 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN6Player10addEnemiesEP5ArrayIPS_E`<br>Player::addEnemies(Array<Player*>*) |
| 1629 | `0x0a2c6a` | `0x0a2c92` | +40 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN6Player8addEnemyEPS_`<br>Player::addEnemy(Player*) |
| 1630 | `0x0a2cd2` | `0x0a2cfa` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Player10getEnemiesEv`<br>Player::getEnemies() |
| 1631 | `0x0a2cd6` | `0x0a2cfe` | +40 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN6Player8getEnemyEi`<br>Player::getEnemy(int) |
| 1632 | `0x0a2ce0` | `0x0a2d08` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Player9getRadiusEv`<br>Player::getRadius() |
| 1633 | `0x0a2ce4` | `0x0a2d0c` | +40 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN6Player12setHitpointsEi`<br>Player::setHitpoints(int) |
| 1634 | `0x0a2cf6` | `0x0a2d1e` | +40 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN6Player11setShieldHPEi`<br>Player::setShieldHP(int) |
| 1635 | `0x0a2d1c` | `0x0a2d44` | +40 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN6Player10setArmorHPEi`<br>Player::setArmorHP(int) |
| 1636 | `0x0a2d30` | `0x0a2d58` | +40 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6Player10setGammaHPEi`<br>Player::setGammaHP(int) |
| 1637 | `0x0a2d64` | `0x0a2d8c` | +40 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Player15setMaxEmpPointsEi`<br>Player::setMaxEmpPoints(int) |
| 1638 | `0x0a2d6c` | `0x0a2d94` | +40 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6Player15setMaxHitpointsEi`<br>Player::setMaxHitpoints(int) |
| 1639 | `0x0a2d76` | `0x0a2d9e` | +40 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN6Player14setMaxShieldHPEi`<br>Player::setMaxShieldHP(int) |
| 1640 | `0x0a2d8a` | `0x0a2db2` | +40 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Player13setMaxArmorHPEi`<br>Player::setMaxArmorHP(int) |
| 1641 | `0x0a2d92` | `0x0a2dba` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Player12getEmpPointsEv`<br>Player::getEmpPoints() |
| 1642 | `0x0a2d96` | `0x0a2dbe` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player15getMaxEmpPointsEv`<br>Player::getMaxEmpPoints() |
| 1643 | `0x0a2d9c` | `0x0a2dc4` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player14getMaxShieldHPEv`<br>Player::getMaxShieldHP() |
| 1644 | `0x0a2da2` | `0x0a2dca` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN6Player11getShieldHPEv`<br>Player::getShieldHP() |
| 1645 | `0x0a2db0` | `0x0a2dd8` | +40 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN6Player13getCombinedHPEv`<br>Player::getCombinedHP() |
| 1646 | `0x0a2dd6` | `0x0a2dfe` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player10getArmorHPEv`<br>Player::getArmorHP() |
| 1647 | `0x0a2ddc` | `0x0a2e04` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player13getMaxArmorHPEv`<br>Player::getMaxArmorHP() |
| 1648 | `0x0a2de2` | `0x0a2e0a` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN6Player10getGammaHPEv`<br>Player::getGammaHP() |
| 1649 | `0x0a2df0` | `0x0a2e18` | +40 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN6Player16regenerateShieldEf`<br>Player::regenerateShield(float) |
| 1650 | `0x0a2e1a` | `0x0a2e42` | +40 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN6Player16regenerateShieldEv`<br>Player::regenerateShield() |
| 1651 | `0x0a2e44` | `0x0a2e6c` | +40 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN6Player15regenerateArmorEv`<br>Player::regenerateArmor() |
| 1652 | `0x0a2e58` | `0x0a2e80` | +40 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN6Player14regenerateHullEv`<br>Player::regenerateHull() |
| 1653 | `0x0a2e6c` | `0x0a2e94` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Player12getHitpointsEv`<br>Player::getHitpoints() |
| 1654 | `0x0a2e70` | `0x0a2e98` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player15getMaxHitpointsEv`<br>Player::getMaxHitpoints() |
| 1655 | `0x0a2e76` | `0x0a2e9e` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player13getDamageRateEv`<br>Player::getDamageRate() |
| 1656 | `0x0a2e7c` | `0x0a2ea4` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player16getEmpDamageRateEv`<br>Player::getEmpDamageRate() |
| 1657 | `0x0a2e82` | `0x0a2eaa` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player19getShieldDamageRateEv`<br>Player::getShieldDamageRate() |
| 1658 | `0x0a2e88` | `0x0a2eb0` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player18getArmorDamageRateEv`<br>Player::getArmorDamageRate() |
| 1659 | `0x0a2e8e` | `0x0a2eb6` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player13setVulnerableEb`<br>Player::setVulnerable(bool) |
| 1660 | `0x0a2e94` | `0x0a2ebc` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player12setHitVectorEfff`<br>Player::setHitVector(float, float, float) |
| 1661 | `0x0a2e9a` | `0x0a2ec2` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player12setBombForceEf`<br>Player::setBombForce(float) |
| 1662 | `0x0a2ea0` | `0x0a2ec8` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player11setEmpForceEf`<br>Player::setEmpForce(float) |
| 1663 | `0x0a2ea6` | `0x0a2ece` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player12getBombForceEv`<br>Player::getBombForce() |
| 1664 | `0x0a2eac` | `0x0a2ed4` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player11getEmpForceEv`<br>Player::getEmpForce() |
| 1665 | `0x0a2eb2` | `0x0a2eda` | +40 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN6Player12getHitVectorEv`<br>Player::getHitVector() |
| 1666 | `0x0a2ec2` | `0x0a2eea` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Player15getGunRegenRateEi`<br>Player::getGunRegenRate(int) |
| 1667 | `0x0a2ec6` | `0x0a2eee` | +40 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN6Player10damageHullEi`<br>Player::damageHull(int) |
| 1668 | `0x0a2f04` | `0x0a2f2c` | +40 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Player10damageShipEi`<br>Player::damageShip(int) |
| 1669 | `0x0a2f10` | `0x0a2f38` | +40 | 82 | 82 | 0 | 100.0% |  | Y | `_ZN6Player12damageShieldEi`<br>Player::damageShield(int) |
| 1670 | `0x0a2f62` | `0x0a2f8a` | +40 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN6Player11damageGammaEf`<br>Player::damageGamma(float) |
| 1671 | `0x0a2f96` | `0x0a2fbe` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN6Player23resetDamageDoneByPlayerEv`<br>Player::resetDamageDoneByPlayer() |
| 1672 | `0x0a2fa4` | `0x0a2fcc` | +40 | 540 | 540 | 0 | 100.0% |  | Y | `_ZN6Player9damageEmpEib`<br>Player::damageEmp(int, bool) |
| 1673 | `0x0a31c0` | `0x0a31e8` | +40 | 1236 | 1236 | 0 | 100.0% |  | Y | `_ZN6Player6damageEibi`<br>Player::damage(int, bool, int) |
| 1674 | `0x0a3694` | `0x0a36bc` | +40 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6Player6damageEi`<br>Player::damage(int) |
| 1675 | `0x0a369e` | `0x0a36c6` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player11turnedEnemyEv`<br>Player::turnedEnemy() |
| 1676 | `0x0a36a4` | `0x0a36cc` | +40 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN6Player9turnEnemyEv`<br>Player::turnEnemy() |
| 1677 | `0x0a36ac` | `0x0a36d4` | +40 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN6Player12gunAvailableEi`<br>Player::gunAvailable(int) |
| 1678 | `0x0a36ce` | `0x0a36f6` | +40 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Player6isDeadEv`<br>Player::isDead() |
| 1679 | `0x0a36da` | `0x0a3702` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player9isDamagedEv`<br>Player::isDamaged() |
| 1680 | `0x0a36e0` | `0x0a3708` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player9setActiveEb`<br>Player::setActive(bool) |
| 1681 | `0x0a36e6` | `0x0a370e` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player8isActiveEv`<br>Player::isActive() |
| 1682 | `0x0a36ec` | `0x0a3714` | +40 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN6Player6addGunEP3Guni`<br>Player::addGun(Gun*, int) |
| 1683 | `0x0a3750` | `0x0a3778` | +40 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP3GunEvT_R5ArrayIS2_E`<br>void ArrayAdd<Gun*>(Gun*, Array<Gun*>&) |
| 1684 | `0x0a3774` | `0x0a379c` | +40 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN6Player16calcWeaponSoundsEi`<br>Player::calcWeaponSounds(int) |
| 1685 | `0x0a3910` | `0x0a3938` | +40 | 118 | 118 | 0 | 100.0% |  | Y | `_ZN6Player6addGunEP5ArrayIP3GunEi`<br>Player::addGun(Array<Gun*>*, int) |
| 1686 | `0x0a3986` | `0x0a39ae` | +40 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN6Player13removeAllGunsEv`<br>Player::removeAllGuns() |
| 1687 | `0x0a39a6` | `0x0a39ce` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Player11getGunSlotsEv`<br>Player::getGunSlots() |
| 1688 | `0x0a39aa` | `0x0a39d2` | +40 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN6Player13resetGunDelayEi`<br>Player::resetGunDelay(int) |
| 1689 | `0x0a39dc` | `0x0a3a04` | +40 | 54 | 54 | 0 | 100.0% | Y | Y | `_ZN6Player14refillGunDelayEi`<br>Player::refillGunDelay(int) |
| 1690 | `0x0a3a14` | `0x0a3a3c` | +40 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN6Player14stopShootSoundEii`<br>Player::stopShootSound(int, int) |
| 1691 | `0x0a3a64` | `0x0a3a8c` | +40 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN6Player14playShootSoundEiiPN11AbyssEngine6AEMath6VectorEf`<br>Player::playShootSound(int, int, AbyssEngine::AEMath::Vector*, float) |
| 1692 | `0x0a3b20` | `0x0a3b48` | +40 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN6Player5shootEixbN11AbyssEngine6AEMath6MatrixE`<br>Player::shoot(int, long long, bool, AbyssEngine::AEMath::Matrix) |
| 1693 | `0x0a3cbc` | `0x0a3ce4` | +40 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6Player12stopShootingEi`<br>Player::stopShooting(int) |
| 1694 | `0x0a3d08` | `0x0a3d30` | +40 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN6Player12stopShootingEii`<br>Player::stopShooting(int, int) |
| 1695 | `0x0a3d5c` | `0x0a3d84` | +40 | 468 | 468 | 0 | 100.0% |  | Y | `_ZN6Player5shootEiixbN11AbyssEngine6AEMath6MatrixE`<br>Player::shoot(int, int, long long, bool, AbyssEngine::AEMath::Matrix) |
| 1696 | `0x0a3f30` | `0x0a3f58` | +40 | 110 | 110 | 0 | 100.0% |  | Y | `_ZN6Player5shootEixb`<br>Player::shoot(int, long long, bool) |
| 1697 | `0x0a3f9e` | `0x0a3fc6` | +40 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN6Player5shootEiixb`<br>Player::shoot(int, int, long long, bool) |
| 1698 | `0x0a4014` | `0x0a403c` | +40 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN6Player15PlayEngineSoundEiPN11AbyssEngine6AEMath6VectorE`<br>Player::PlayEngineSound(int, AbyssEngine::AEMath::Vector*) |
| 1699 | `0x0a4088` | `0x0a40b0` | +40 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN6Player16PauseEngineSoundEv`<br>Player::PauseEngineSound() |
| 1700 | `0x0a40ac` | `0x0a40d4` | +40 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6Player17ResumeEngineSoundEb`<br>Player::ResumeEngineSound(bool) |
| 1701 | `0x0a40e0` | `0x0a4108` | +40 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN6Player15StopEngineSoundEv`<br>Player::StopEngineSound() |
| 1702 | `0x0a4108` | `0x0a4130` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Player14GetEngineEventEv`<br>Player::GetEngineEvent() |
| 1703 | `0x0a4110` | `0x0a4138` | +40 | 460 | 460 | 0 | 100.0% |  | Y | `_ZN6Player6updateEib`<br>Player::update(int, bool) |
| 1704 | `0x0a42dc` | `0x0a4304` | +40 | 94 | 94 | 0 | 100.0% |  | Y | `_ZN6Player4healEf`<br>Player::heal(float) |
| 1705 | `0x0a433a` | `0x0a4362` | +40 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN6Player19pitchAllPrimaryGunsEf`<br>Player::pitchAllPrimaryGuns(float) |
| 1706 | `0x0a4360` | `0x0a4388` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN6Player11replaceGunsEiiiiib`<br>Player::replaceGuns(int, int, int, int, int, bool) |
| 1707 | `0x0a4362` | `0x0a438a` | +40 | 54 | 54 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP6PlayerEvPKT_jR5ArrayIS2_E`<br>void ArrayAdd<Player*>(Player* const*, unsigned int, Array<Player*>&) |
| 1708 | `0x0a4398` | `0x0a43c0` | +40 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN10AEGeometryC2EtPN11AbyssEngine11PaintCanvasEb`<br>AEGeometry::AEGeometry(unsigned short, AbyssEngine::PaintCanvas*, bool) |
| 1709 | `0x0a4398` | `0x0a43c0` | +40 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN10AEGeometryC1EtPN11AbyssEngine11PaintCanvasEb`<br>AEGeometry::AEGeometry(unsigned short, AbyssEngine::PaintCanvas*, bool) |
| 1710 | `0x0a4468` | `0x0a4490` | +40 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN10AEGeometryC1EPN11AbyssEngine11PaintCanvasE`<br>AEGeometry::AEGeometry(AbyssEngine::PaintCanvas*) |
| 1711 | `0x0a4468` | `0x0a4490` | +40 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN10AEGeometryC2EPN11AbyssEngine11PaintCanvasE`<br>AEGeometry::AEGeometry(AbyssEngine::PaintCanvas*) |
| 1712 | `0x0a44f8` | `0x0a4520` | +40 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN10AEGeometryD2Ev`<br>AEGeometry::~AEGeometry() |
| 1713 | `0x0a44f8` | `0x0a4520` | +40 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN10AEGeometryD1Ev`<br>AEGeometry::~AEGeometry() |
| 1714 | `0x0a453a` | `0x0a4562` | +40 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN10AEGeometry8addChildEj`<br>AEGeometry::addChild(unsigned int) |
| 1715 | `0x0a4558` | `0x0a4580` | +40 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10AEGeometry7setMeshEt`<br>AEGeometry::setMesh(unsigned short) |
| 1716 | `0x0a4572` | `0x0a459a` | +40 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN10AEGeometry17getParentPositionEv`<br>AEGeometry::getParentPosition() |
| 1717 | `0x0a4592` | `0x0a45ba` | +40 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10AEGeometry11getPositionEv`<br>AEGeometry::getPosition() |
| 1718 | `0x0a45ac` | `0x0a45d4` | +40 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10AEGeometry12getDirectionEv`<br>AEGeometry::getDirection() |
| 1719 | `0x0a45c6` | `0x0a45ee` | +40 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10AEGeometry11getUpVectorEv`<br>AEGeometry::getUpVector() |
| 1720 | `0x0a45e0` | `0x0a4608` | +40 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10AEGeometry14getRightVectorEv`<br>AEGeometry::getRightVector() |
| 1721 | `0x0a45fa` | `0x0a4622` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry11getRotationEv`<br>AEGeometry::getRotation() |
| 1722 | `0x0a4608` | `0x0a4630` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry10getScalingEv`<br>AEGeometry::getScaling() |
| 1723 | `0x0a4616` | `0x0a463e` | +40 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN10AEGeometry9getMatrixEv`<br>AEGeometry::getMatrix() |
| 1724 | `0x0a4624` | `0x0a464c` | +40 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN10AEGeometry9setMatrixERKN11AbyssEngine6AEMath6MatrixE`<br>AEGeometry::setMatrix(AbyssEngine::AEMath::Matrix const&) |
| 1725 | `0x0a462e` | `0x0a4656` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry18getReferenceMatrixEv`<br>AEGeometry::getReferenceMatrix() |
| 1726 | `0x0a4632` | `0x0a465a` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN10AEGeometry11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>AEGeometry::setPosition(AbyssEngine::AEMath::Vector const&) |
| 1727 | `0x0a4640` | `0x0a4668` | +40 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN10AEGeometry11setPositionEfff`<br>AEGeometry::setPosition(float, float, float) |
| 1728 | `0x0a4688` | `0x0a46b0` | +40 | 240 | 240 | 0 | 100.0% |  | Y | `_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_`<br>AEGeometry::setDirection(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 1729 | `0x0a4778` | `0x0a47a0` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN10AEGeometry11setRotationERKN11AbyssEngine6AEMath6VectorE`<br>AEGeometry::setRotation(AbyssEngine::AEMath::Vector const&) |
| 1730 | `0x0a4784` | `0x0a47ac` | +40 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN10AEGeometry11setRotationEfff`<br>AEGeometry::setRotation(float, float, float) |
| 1731 | `0x0a47f8` | `0x0a4820` | +40 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN10AEGeometry10setScalingEf`<br>AEGeometry::setScaling(float) |
| 1732 | `0x0a4800` | `0x0a4828` | +40 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN10AEGeometry10setScalingEfff`<br>AEGeometry::setScaling(float, float, float) |
| 1733 | `0x0a4874` | `0x0a489c` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN10AEGeometry10setScalingERKN11AbyssEngine6AEMath6VectorE`<br>AEGeometry::setScaling(AbyssEngine::AEMath::Vector const&) |
| 1734 | `0x0a4880` | `0x0a48a8` | +40 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN10AEGeometry6rotateEfff`<br>AEGeometry::rotate(float, float, float) |
| 1735 | `0x0a4914` | `0x0a493c` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN10AEGeometry6rotateERKN11AbyssEngine6AEMath6VectorE`<br>AEGeometry::rotate(AbyssEngine::AEMath::Vector const&) |
| 1736 | `0x0a4920` | `0x0a4948` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN10AEGeometry9translateERKN11AbyssEngine6AEMath6VectorE`<br>AEGeometry::translate(AbyssEngine::AEMath::Vector const&) |
| 1737 | `0x0a492c` | `0x0a4954` | +40 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN10AEGeometry9translateEfff`<br>AEGeometry::translate(float, float, float) |
| 1738 | `0x0a49b8` | `0x0a49e0` | +40 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN10AEGeometry11moveForwardEf`<br>AEGeometry::moveForward(float) |
| 1739 | `0x0a4a40` | `0x0a4a68` | +40 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry10setVisibleEb`<br>AEGeometry::setVisible(bool) |
| 1740 | `0x0a4a4a` | `0x0a4a72` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry9isVisibleEv`<br>AEGeometry::isVisible() |
| 1741 | `0x0a4a50` | `0x0a4a78` | +40 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN10AEGeometry6renderEv`<br>AEGeometry::render() |
| 1742 | `0x0a4a62` | `0x0a4a8a` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry5getIDEv`<br>AEGeometry::getID() |
| 1743 | `0x0a4a66` | `0x0a4a8e` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry16setRotationOrderEN11AbyssEngine6AEMath13RotationOrderE`<br>AEGeometry::setRotationOrder(AbyssEngine::AEMath::RotationOrder) |
| 1744 | `0x0a4a6a` | `0x0a4a92` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN10AEGeometry21updateReferenceMatrixEv`<br>AEGeometry::updateReferenceMatrix() |
| 1745 | `0x0a4a86` | `0x0a4aae` | +40 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry25setLodLastVisibleDistanceEy`<br>AEGeometry::setLodLastVisibleDistance(unsigned long long) |
| 1746 | `0x0a4a98` | `0x0a4ac0` | +40 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN10AEGeometry17setLodChildMeshesEPt`<br>AEGeometry::setLodChildMeshes(unsigned short*) |
| 1747 | `0x0a4b3c` | `0x0a4b64` | +40 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN10AEGeometry20setLodChildTransformEj`<br>AEGeometry::setLodChildTransform(unsigned int) |
| 1748 | `0x0a4b88` | `0x0a4bb0` | +40 | 252 | 252 | 0 | 100.0% |  | Y | `_ZN10AEGeometry23setLodMeshesWithMeshIdsEPtPjPii`<br>AEGeometry::setLodMeshesWithMeshIds(unsigned short*, unsigned int*, int*, int) |
| 1749 | `0x0a4c84` | `0x0a4cac` | +40 | 252 | 252 | 0 | 100.0% |  | Y | `_ZN10AEGeometry12setLodMeshesEPtPii`<br>AEGeometry::setLodMeshes(unsigned short*, int*, int) |
| 1750 | `0x0a4d80` | `0x0a4da8` | +40 | 532 | 532 | 0 | 100.0% |  | Y | `_ZN10AEGeometry9updateLodERKN11AbyssEngine6AEMath6VectorEf`<br>AEGeometry::updateLod(AbyssEngine::AEMath::Vector const&, float) |
| 1751 | `0x0a4f94` | `0x0a4fbc` | +40 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry6hasLodEv`<br>AEGeometry::hasLod() |
| 1752 | `0x0a4f9e` | `0x0a4fc6` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN10AEGeometry24DEBUG_setMeshMergerIndexEiP13LodMeshMerger`<br>AEGeometry::DEBUG_setMeshMergerIndex(int, LodMeshMerger*) |
| 1753 | `0x0a4fa4` | `0x0a4fcc` | +40 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgateC2EiP10AEGeometryfffb`<br>PlayerJumpgate::PlayerJumpgate(int, AEGeometry*, float, float, float, bool) |
| 1754 | `0x0a4fa4` | `0x0a4fcc` | +40 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgateC1EiP10AEGeometryfffb`<br>PlayerJumpgate::PlayerJumpgate(int, AEGeometry*, float, float, float, bool) |
| 1755 | `0x0a50ac` | `0x0a50d4` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14BoundingVolumeEC2Ev`<br>Array<BoundingVolume*>::Array() |
| 1756 | `0x0a50c8` | `0x0a50f0` | +40 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP14BoundingVolumeEvjR5ArrayIT_E`<br>void ArraySetLength<BoundingVolume*>(unsigned int, Array<BoundingVolume*>&) |
| 1757 | `0x0a50fc` | `0x0a5124` | +40 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgateD1Ev`<br>PlayerJumpgate::~PlayerJumpgate() |
| 1758 | `0x0a50fc` | `0x0a5124` | +40 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgateD2Ev`<br>PlayerJumpgate::~PlayerJumpgate() |
| 1759 | `0x0a5100` | `0x0a5128` | +40 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgateD0Ev`<br>PlayerJumpgate::~PlayerJumpgate() |
| 1760 | `0x0a5110` | `0x0a5138` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerJumpgate22addJumpAnimationHandleEj`<br>PlayerJumpgate::addJumpAnimationHandle(unsigned int) |
| 1761 | `0x0a5118` | `0x0a5140` | +40 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgate8activateEv`<br>PlayerJumpgate::activate() |
| 1762 | `0x0a5164` | `0x0a518c` | +40 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgate10timeToJumpEv`<br>PlayerJumpgate::timeToJump() |
| 1763 | `0x0a5190` | `0x0a51b8` | +40 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgate14animationEndedEv`<br>PlayerJumpgate::animationEnded() |
| 1764 | `0x0a51c0` | `0x0a51e8` | +40 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgate11setPositionEfff`<br>PlayerJumpgate::setPosition(float, float, float) |
| 1765 | `0x0a51f0` | `0x0a5218` | +40 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN14PlayerJumpgate6updateEi`<br>PlayerJumpgate::update(int) |
| 1766 | `0x0a5220` | `0x0a5248` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8initPushERKN11AbyssEngine6AEMath6VectorEi`<br>KIPlayer::initPush(AbyssEngine::AEMath::Vector const&, int) |
| 1767 | `0x0a5222` | `0x0a524a` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer4pushEi`<br>KIPlayer::push(int) |
| 1768 | `0x0a5224` | `0x0a524c` | +40 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer12outerCollideERKN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::outerCollide(AbyssEngine::AEMath::Vector const&) |
| 1769 | `0x0a5240` | `0x0a5268` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8getSpeedEv`<br>KIPlayer::getSpeed() |
| 1770 | `0x0a5244` | `0x0a526c` | +40 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer18getCollisionNormalERKN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::getCollisionNormal(AbyssEngine::AEMath::Vector const&) |
| 1771 | `0x0a524e` | `0x0a5276` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8setStateEi`<br>KIPlayer::setState(int) |
| 1772 | `0x0a5254` | `0x0a527c` | +40 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHoleC2EiP10AEGeometryfffb`<br>PlayerWormHole::PlayerWormHole(int, AEGeometry*, float, float, float, bool) |
| 1773 | `0x0a5254` | `0x0a527c` | +40 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHoleC1EiP10AEGeometryfffb`<br>PlayerWormHole::PlayerWormHole(int, AEGeometry*, float, float, float, bool) |
| 1774 | `0x0a52ec` | `0x0a5314` | +40 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN14PlayerWormHole5resetEb`<br>PlayerWormHole::reset(bool) |
| 1775 | `0x0a52fe` | `0x0a5326` | +40 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHoleD1Ev`<br>PlayerWormHole::~PlayerWormHole() |
| 1776 | `0x0a52fe` | `0x0a5326` | +40 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHoleD2Ev`<br>PlayerWormHole::~PlayerWormHole() |
| 1777 | `0x0a5302` | `0x0a532a` | +40 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHoleD0Ev`<br>PlayerWormHole::~PlayerWormHole() |
| 1778 | `0x0a5314` | `0x0a533c` | +40 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN14PlayerWormHole4openEv`<br>PlayerWormHole::open() |
| 1779 | `0x0a5324` | `0x0a534c` | +40 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN14PlayerWormHole11isShrinkingEv`<br>PlayerWormHole::isShrinking() |
| 1780 | `0x0a5336` | `0x0a535e` | +40 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHole11setPositionEfff`<br>PlayerWormHole::setPosition(float, float, float) |
| 1781 | `0x0a536a` | `0x0a5392` | +40 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHole6renderEv`<br>PlayerWormHole::render() |
| 1782 | `0x0a5378` | `0x0a53a0` | +40 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN14PlayerWormHole15freeMissionLockEv`<br>PlayerWormHole::freeMissionLock() |
| 1783 | `0x0a5380` | `0x0a53a8` | +40 | 1056 | 1056 | 0 | 100.0% |  | Y | `_ZN14PlayerWormHole6updateEi`<br>PlayerWormHole::update(int) |
| 1784 | `0x0a57a0` | `0x0a57c8` | +40 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN14PlayerWormHole11getPositionEv`<br>PlayerWormHole::getPosition() |
| 1785 | `0x0a57c8` | `0x0a57f0` | +40 | 556 | 556 | 0 | 100.0% |  | Y | `_ZN8KIPlayerC1EiiP6PlayerP10AEGeometryfffb`<br>KIPlayer::KIPlayer(int, int, Player*, AEGeometry*, float, float, float, bool) |
| 1786 | `0x0a57c8` | `0x0a57f0` | +40 | 556 | 556 | 0 | 100.0% |  | Y | `_ZN8KIPlayerC2EiiP6PlayerP10AEGeometryfffb`<br>KIPlayer::KIPlayer(int, int, Player*, AEGeometry*, float, float, float, bool) |
| 1787 | `0x0a59f4` | `0x0a5a1c` | +40 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN8KIPlayerD1Ev`<br>KIPlayer::~KIPlayer() |
| 1788 | `0x0a59f4` | `0x0a5a1c` | +40 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN8KIPlayerD2Ev`<br>KIPlayer::~KIPlayer() |
| 1789 | `0x0a5ab0` | `0x0a5ad8` | +40 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP10SpacePointEvR5ArrayIT_E`<br>void ArrayReleaseClasses<SpacePoint*>(Array<SpacePoint*>&) |
| 1790 | `0x0a5b00` | `0x0a5b28` | +40 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP10SpacePointED2Ev`<br>Array<SpacePoint*>::~Array() |
| 1791 | `0x0a5b16` | `0x0a5b3e` | +40 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN8KIPlayerD0Ev`<br>KIPlayer::~KIPlayer() |
| 1792 | `0x0a5b26` | `0x0a5b4e` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer9isVisibleEv`<br>KIPlayer::isVisible() |
| 1793 | `0x0a5b2c` | `0x0a5b54` | +40 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN8KIPlayer10setVisibleEb`<br>KIPlayer::setVisible(bool) |
| 1794 | `0x0a5b3e` | `0x0a5b66` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer14setSpacePointsEP5ArrayIP10SpacePointE`<br>KIPlayer::setSpacePoints(Array<SpacePoint*>*) |
| 1795 | `0x0a5b44` | `0x0a5b6c` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer14getSpacePointsEv`<br>KIPlayer::getSpacePoints() |
| 1796 | `0x0a5b4c` | `0x0a5b74` | +40 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN8KIPlayer25getNearestNavigationPointERKN11AbyssEngine6AEMath6VectorEP10SpacePoint`<br>KIPlayer::getNearestNavigationPoint(AbyssEngine::AEMath::Vector const&, SpacePoint*) |
| 1797 | `0x0a5c54` | `0x0a5c7c` | +40 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN8KIPlayer22getNearestDockingPointERKN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::getNearestDockingPoint(AbyssEngine::AEMath::Vector const&) |
| 1798 | `0x0a5d40` | `0x0a5d68` | +40 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN8KIPlayer12setShipGroupEP10AEGeometryib`<br>KIPlayer::setShipGroup(AEGeometry*, int, bool) |
| 1799 | `0x0a5dd8` | `0x0a5e00` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer9setJumperEb`<br>KIPlayer::setJumper(bool) |
| 1800 | `0x0a5dde` | `0x0a5e06` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8isJumperEv`<br>KIPlayer::isJumper() |
| 1801 | `0x0a5de4` | `0x0a5e0c` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer18setInitialRotationEN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::setInitialRotation(AbyssEngine::AEMath::Vector) |
| 1802 | `0x0a5de6` | `0x0a5e0e` | +40 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN8KIPlayer5resetEv`<br>KIPlayer::reset() |
| 1803 | `0x0a5e2a` | `0x0a5e52` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN8KIPlayer10setToSleepEv`<br>KIPlayer::setToSleep() |
| 1804 | `0x0a5e46` | `0x0a5e6e` | +40 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8KIPlayer9setActiveEb`<br>KIPlayer::setActive(bool) |
| 1805 | `0x0a5e4c` | `0x0a5e74` | +40 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN8KIPlayer13setInitActiveEb`<br>KIPlayer::setInitActive(bool) |
| 1806 | `0x0a5e60` | `0x0a5e88` | +40 | 684 | 684 | 0 | 100.0% |  | Y | `_ZN8KIPlayer12captureCrateEP3Hud`<br>KIPlayer::captureCrate(Hud*) |
| 1807 | `0x0a610c` | `0x0a6134` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer6isDeadEv`<br>KIPlayer::isDead() |
| 1808 | `0x0a611a` | `0x0a6142` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer7isDyingEv`<br>KIPlayer::isDying() |
| 1809 | `0x0a6128` | `0x0a6150` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer9isWingManEv`<br>KIPlayer::isWingMan() |
| 1810 | `0x0a612e` | `0x0a6156` | +40 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer10setWingmanEbi`<br>KIPlayer::setWingman(bool, int) |
| 1811 | `0x0a613a` | `0x0a6162` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer17setWingmanCommandEiPS_`<br>KIPlayer::setWingmanCommand(int, KIPlayer*) |
| 1812 | `0x0a6140` | `0x0a6168` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8setSpeedEf`<br>KIPlayer::setSpeed(float) |
| 1813 | `0x0a6144` | `0x0a616c` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer16setRotationSpeedEf`<br>KIPlayer::setRotationSpeed(float) |
| 1814 | `0x0a614a` | `0x0a6172` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8setLevelEP5Level`<br>KIPlayer::setLevel(Level*) |
| 1815 | `0x0a614e` | `0x0a6176` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer15enableExplosionEv`<br>KIPlayer::enableExplosion() |
| 1816 | `0x0a6150` | `0x0a6178` | +40 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN8KIPlayer8setRouteEP5Route`<br>KIPlayer::setRoute(Route*) |
| 1817 | `0x0a6174` | `0x0a619c` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8getRouteEv`<br>KIPlayer::getRoute() |
| 1818 | `0x0a6178` | `0x0a61a0` | +40 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8KIPlayer10setEnemiesEP5ArrayIP6PlayerE`<br>KIPlayer::setEnemies(Array<Player*>*) |
| 1819 | `0x0a617e` | `0x0a61a6` | +40 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN8KIPlayer16PauseEngineSoundEv`<br>KIPlayer::PauseEngineSound() |
| 1820 | `0x0a6192` | `0x0a61ba` | +40 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN8KIPlayer15PlayEngineSoundEv`<br>KIPlayer::PlayEngineSound() |
| 1821 | `0x0a61a8` | `0x0a61d0` | +40 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN8KIPlayer17ResumeEngineSoundEv`<br>KIPlayer::ResumeEngineSound() |
| 1822 | `0x0a61be` | `0x0a61e6` | +40 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN8KIPlayer15StopEngineSoundEv`<br>KIPlayer::StopEngineSound() |
| 1823 | `0x0a61d2` | `0x0a61fa` | +40 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8KIPlayer6addGunEP3Guni`<br>KIPlayer::addGun(Gun*, int) |
| 1824 | `0x0a61d8` | `0x0a6200` | +40 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8KIPlayer6addGunEP5ArrayIP3GunEi`<br>KIPlayer::addGun(Array<Gun*>*, int) |
| 1825 | `0x0a61de` | `0x0a6206` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer7getTypeEv`<br>KIPlayer::getType() |
| 1826 | `0x0a61e4` | `0x0a620c` | +40 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer7isEnemyEv`<br>KIPlayer::isEnemy() |
| 1827 | `0x0a61ec` | `0x0a6214` | +40 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN8KIPlayer11getPositionEv`<br>KIPlayer::getPosition() |
| 1828 | `0x0a61fa` | `0x0a6222` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer8isDockedEv`<br>KIPlayer::isDocked() |
| 1829 | `0x0a6208` | `0x0a6230` | +40 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN8KIPlayer7setDeadEv`<br>KIPlayer::setDead() |
| 1830 | `0x0a6214` | `0x0a623c` | +40 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN8KIPlayer5awakeEv`<br>KIPlayer::awake() |
| 1831 | `0x0a6224` | `0x0a624c` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer13setJumpSphereEj`<br>KIPlayer::setJumpSphere(unsigned int) |
| 1832 | `0x0a622c` | `0x0a6254` | +40 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN8KIPlayer11createCrateEi`<br>KIPlayer::createCrate(int) |
| 1833 | `0x0a6304` | `0x0a632c` | +40 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer14cargoAvailableEv`<br>KIPlayer::cargoAvailable() |
| 1834 | `0x0a6328` | `0x0a6350` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer14levelCollisionEPN11AbyssEngine6AEMath6VectorEx`<br>KIPlayer::levelCollision(AbyssEngine::AEMath::Vector*, long long) |
| 1835 | `0x0a632c` | `0x0a6354` | +40 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8KIPlayer6renderEv`<br>KIPlayer::render() |
| 1836 | `0x0a6332` | `0x0a635a` | +40 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN8KIPlayer9translateERKN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::translate(AbyssEngine::AEMath::Vector const&) |
| 1837 | `0x0a6350` | `0x0a6378` | +40 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer4jumpEv`<br>KIPlayer::jump() |
| 1838 | `0x0a635e` | `0x0a6386` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer6reviveEv`<br>KIPlayer::revive() |
| 1839 | `0x0a6360` | `0x0a6388` | +40 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN8KIPlayer11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::setPosition(AbyssEngine::AEMath::Vector const&) |
| 1840 | `0x0a6394` | `0x0a63bc` | +40 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN8KIPlayer11setPositionEfff`<br>KIPlayer::setPosition(float, float, float) |
| 1841 | `0x0a63c0` | `0x0a63e8` | +40 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer6updateEi`<br>KIPlayer::update(int) |
| 1842 | `0x0a63c2` | `0x0a63ea` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer7collideEfff`<br>KIPlayer::collide(float, float, float) |
| 1843 | `0x0a63c6` | `0x0a63ee` | +40 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer12outerCollideEfff`<br>KIPlayer::outerCollide(float, float, float) |
| 1844 | `0x0a63ca` | `0x0a63f2` | +40 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer19getProjectionVectorERKN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::getProjectionVector(AbyssEngine::AEMath::Vector const&) |
| 1845 | `0x0a63d4` | `0x0a63fc` | +40 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8KIPlayer25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE`<br>KIPlayer::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) |
| 1846 | `0x0a63e0` | `0x0a6406` | +38 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EiiPN11AbyssEngine6StringE`<br>ListItem::ListItem(int, int, AbyssEngine::String*) |
| 1847 | `0x0a63e0` | `0x0a6406` | +38 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EiiPN11AbyssEngine6StringE`<br>ListItem::ListItem(int, int, AbyssEngine::String*) |
| 1848 | `0x0a6428` | `0x0a644e` | +38 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN8ListItem4initEv`<br>ListItem::init() |
| 1849 | `0x0a645a` | `0x0a6480` | +38 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EPS_`<br>ListItem::ListItem(ListItem*) |
| 1850 | `0x0a645a` | `0x0a6480` | +38 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EPS_`<br>ListItem::ListItem(ListItem*) |
| 1851 | `0x0a64fa` | `0x0a6520` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EP9BluePrint`<br>ListItem::ListItem(BluePrint*) |
| 1852 | `0x0a64fa` | `0x0a6520` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EP9BluePrint`<br>ListItem::ListItem(BluePrint*) |
| 1853 | `0x0a6512` | `0x0a6538` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EP4Ship`<br>ListItem::ListItem(Ship*) |
| 1854 | `0x0a6512` | `0x0a6538` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EP4Ship`<br>ListItem::ListItem(Ship*) |
| 1855 | `0x0a652a` | `0x0a6550` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EP4Item`<br>ListItem::ListItem(Item*) |
| 1856 | `0x0a652a` | `0x0a6550` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EP4Item`<br>ListItem::ListItem(Item*) |
| 1857 | `0x0a6542` | `0x0a6568` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EP14PendingProduct`<br>ListItem::ListItem(PendingProduct*) |
| 1858 | `0x0a6542` | `0x0a6568` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EP14PendingProduct`<br>ListItem::ListItem(PendingProduct*) |
| 1859 | `0x0a655a` | `0x0a6580` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EP5Agent`<br>ListItem::ListItem(Agent*) |
| 1860 | `0x0a655a` | `0x0a6580` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EP5Agent`<br>ListItem::ListItem(Agent*) |
| 1861 | `0x0a6572` | `0x0a6598` | +38 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EPN11AbyssEngine6StringEbi`<br>ListItem::ListItem(AbyssEngine::String*, bool, int) |
| 1862 | `0x0a6572` | `0x0a6598` | +38 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EPN11AbyssEngine6StringEbi`<br>ListItem::ListItem(AbyssEngine::String*, bool, int) |
| 1863 | `0x0a65bc` | `0x0a65e2` | +38 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EPN11AbyssEngine6StringE`<br>ListItem::ListItem(AbyssEngine::String*) |
| 1864 | `0x0a65bc` | `0x0a65e2` | +38 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EPN11AbyssEngine6StringE`<br>ListItem::ListItem(AbyssEngine::String*) |
| 1865 | `0x0a65fc` | `0x0a6622` | +38 | 70 | 70 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EPN11AbyssEngine6StringEi`<br>ListItem::ListItem(AbyssEngine::String*, int) |
| 1866 | `0x0a65fc` | `0x0a6622` | +38 | 70 | 70 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EPN11AbyssEngine6StringEi`<br>ListItem::ListItem(AbyssEngine::String*, int) |
| 1867 | `0x0a6642` | `0x0a6668` | +38 | 70 | 70 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EPN11AbyssEngine6StringEb`<br>ListItem::ListItem(AbyssEngine::String*, bool) |
| 1868 | `0x0a6642` | `0x0a6668` | +38 | 70 | 70 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EPN11AbyssEngine6StringEb`<br>ListItem::ListItem(AbyssEngine::String*, bool) |
| 1869 | `0x0a6688` | `0x0a66ae` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1Ei`<br>ListItem::ListItem(int) |
| 1870 | `0x0a6688` | `0x0a66ae` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2Ei`<br>ListItem::ListItem(int) |
| 1871 | `0x0a66a0` | `0x0a66c6` | +38 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EPN11AbyssEngine6StringES2_`<br>ListItem::ListItem(AbyssEngine::String*, AbyssEngine::String*) |
| 1872 | `0x0a66a0` | `0x0a66c6` | +38 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EPN11AbyssEngine6StringES2_`<br>ListItem::ListItem(AbyssEngine::String*, AbyssEngine::String*) |
| 1873 | `0x0a66f4` | `0x0a671a` | +38 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN8ListItemC2Eii`<br>ListItem::ListItem(int, int) |
| 1874 | `0x0a66f4` | `0x0a671a` | +38 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN8ListItemC1Eii`<br>ListItem::ListItem(int, int) |
| 1875 | `0x0a6718` | `0x0a673e` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EP7Mission`<br>ListItem::ListItem(Mission*) |
| 1876 | `0x0a6718` | `0x0a673e` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EP7Mission`<br>ListItem::ListItem(Mission*) |
| 1877 | `0x0a6730` | `0x0a6756` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC1EP5ArrayIPN11AbyssEngine6StringEE`<br>ListItem::ListItem(Array<AbyssEngine::String*>*) |
| 1878 | `0x0a6730` | `0x0a6756` | +38 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8ListItemC2EP5ArrayIPN11AbyssEngine6StringEE`<br>ListItem::ListItem(Array<AbyssEngine::String*>*) |
| 1879 | `0x0a6748` | `0x0a676e` | +38 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN8ListItemD1Ev`<br>ListItem::~ListItem() |
| 1880 | `0x0a6748` | `0x0a676e` | +38 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN8ListItemD2Ev`<br>ListItem::~ListItem() |
| 1881 | `0x0a676e` | `0x0a6794` | +38 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8ListItem12isSelectableEv`<br>ListItem::isSelectable() |
| 1882 | `0x0a6774` | `0x0a679a` | +38 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN8ListItem7isImageEv`<br>ListItem::isImage() |
| 1883 | `0x0a6782` | `0x0a67a8` | +38 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8ListItem6isShipEv`<br>ListItem::isShip() |
| 1884 | `0x0a678c` | `0x0a67b2` | +38 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8ListItem6isItemEv`<br>ListItem::isItem() |
| 1885 | `0x0a6796` | `0x0a67bc` | +38 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8ListItem7isCargoEv`<br>ListItem::isCargo() |
| 1886 | `0x0a67a0` | `0x0a67c6` | +38 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN8ListItem5isTabEv`<br>ListItem::isTab() |
| 1887 | `0x0a67be` | `0x0a67e4` | +38 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8ListItem9isMissionEv`<br>ListItem::isMission() |
| 1888 | `0x0a67c8` | `0x0a67ee` | +38 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN8ListItem6isSlotEv`<br>ListItem::isSlot() |
| 1889 | `0x0a67d6` | `0x0a67fc` | +38 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8ListItem12isTextButtonEv`<br>ListItem::isTextButton() |
| 1890 | `0x0a67dc` | `0x0a6802` | +38 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN8ListItem12isSellButtonEv`<br>ListItem::isSellButton() |
| 1891 | `0x0a67f0` | `0x0a6816` | +38 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN8ListItem19isMoveToCargoButtonEv`<br>ListItem::isMoveToCargoButton() |
| 1892 | `0x0a6808` | `0x0a682e` | +38 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8ListItem11isBluePrintEv`<br>ListItem::isBluePrint() |
| 1893 | `0x0a6812` | `0x0a6838` | +38 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8ListItem16isPendingProductEv`<br>ListItem::isPendingProduct() |
| 1894 | `0x0a681c` | `0x0a6842` | +38 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN8ListItem6isTextEv`<br>ListItem::isText() |
| 1895 | `0x0a6822` | `0x0a6848` | +38 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN8ListItem11getNumLinesEv`<br>ListItem::getNumLines() |
| 1896 | `0x0a6834` | `0x0a6858` | +36 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN8ListItem12checkCreditsEv`<br>ListItem::checkCredits() |
| 1897 | `0x0a6870` | `0x0a6894` | +36 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN8ListItem9checkSlotEv`<br>ListItem::checkSlot() |
| 1898 | `0x0a68b0` | `0x0a68d4` | +36 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN8ListItem9checkSortEv`<br>ListItem::checkSort() |
| 1899 | `0x0a68e4` | `0x0a6908` | +36 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN8ListItem8getPriceEv`<br>ListItem::getPrice() |
| 1900 | `0x0a68fc` | `0x0a6920` | +36 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN8ListItem8getIndexEv`<br>ListItem::getIndex() |
| 1901 | `0x0a692c` | `0x0a6950` | +36 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN7StationC2EN11AbyssEngine6StringEiiii`<br>Station::Station(AbyssEngine::String, int, int, int, int) |
| 1902 | `0x0a692c` | `0x0a6950` | +36 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN7StationC1EN11AbyssEngine6StringEiiii`<br>Station::Station(AbyssEngine::String, int, int, int, int) |
| 1903 | `0x0a6978` | `0x0a699c` | +36 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN7StationC1Ev`<br>Station::Station() |
| 1904 | `0x0a6978` | `0x0a699c` | +36 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN7StationC2Ev`<br>Station::Station() |
| 1905 | `0x0a69f4` | `0x0a6a18` | +36 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN7StationD2Ev`<br>Station::~Station() |
| 1906 | `0x0a69f4` | `0x0a6a18` | +36 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN7StationD1Ev`<br>Station::~Station() |
| 1907 | `0x0a6ae8` | `0x0a6b0c` | +36 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP4ShipEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Ship*>(Array<Ship*>&) |
| 1908 | `0x0a6b38` | `0x0a6b5c` | +36 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP4ShipED2Ev`<br>Array<Ship*>::~Array() |
| 1909 | `0x0a6b4e` | `0x0a6b72` | +36 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP4ItemEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Item*>(Array<Item*>&) |
| 1910 | `0x0a6b9e` | `0x0a6bc2` | +36 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP4ItemED2Ev`<br>Array<Item*>::~Array() |
| 1911 | `0x0a6bb4` | `0x0a6bd8` | +36 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP5AgentED2Ev`<br>Array<Agent*>::~Array() |
| 1912 | `0x0a6bca` | `0x0a6bee` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station9getSystemEv`<br>Station::getSystem() |
| 1913 | `0x0a6bce` | `0x0a6bf2` | +36 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN7Station7getNameEv`<br>Station::getName() |
| 1914 | `0x0a6bda` | `0x0a6bfe` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station8getIndexEv`<br>Station::getIndex() |
| 1915 | `0x0a6bde` | `0x0a6c02` | +36 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7Station18setAttackedFriendsEb`<br>Station::setAttackedFriends(bool) |
| 1916 | `0x0a6be4` | `0x0a6c08` | +36 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7Station18hasAttackedFriendsEv`<br>Station::hasAttackedFriends() |
| 1917 | `0x0a6bec` | `0x0a6c10` | +36 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN7Station18isAttackedByAliensEv`<br>Station::isAttackedByAliens() |
| 1918 | `0x0a6c08` | `0x0a6c2c` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station8isPlanetEv`<br>Station::isPlanet() |
| 1919 | `0x0a6c0c` | `0x0a6c30` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station15getTextureIndexEv`<br>Station::getTextureIndex() |
| 1920 | `0x0a6c10` | `0x0a6c34` | +36 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN7Station12isDiscoveredEv`<br>Station::isDiscovered() |
| 1921 | `0x0a6c2c` | `0x0a6c50` | +36 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN7Station5visitEv`<br>Station::visit() |
| 1922 | `0x0a6c64` | `0x0a6c88` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station11getTecLevelEv`<br>Station::getTecLevel() |
| 1923 | `0x0a6c68` | `0x0a6c8c` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station8getShipsEv`<br>Station::getShips() |
| 1924 | `0x0a6c6c` | `0x0a6c90` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station8getItemsEv`<br>Station::getItems() |
| 1925 | `0x0a6c70` | `0x0a6c94` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Station9getAgentsEv`<br>Station::getAgents() |
| 1926 | `0x0a6c74` | `0x0a6c98` | +36 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN7Station11removeShipsEv`<br>Station::removeShips() |
| 1927 | `0x0a6c94` | `0x0a6cb8` | +36 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN7Station10removeShipEP4Ship`<br>Station::removeShip(Ship*) |
| 1928 | `0x0a6ca2` | `0x0a6cc6` | +36 | 68 | 68 | 0 | 100.0% |  | Y | `_Z11ArrayRemoveIP4ShipEvT_R5ArrayIS2_E`<br>void ArrayRemove<Ship*>(Ship*, Array<Ship*>&) |
| 1929 | `0x0a6ce6` | `0x0a6d0a` | +36 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN7Station8setShipsEP5ArrayIP4ShipEb`<br>Station::setShips(Array<Ship*>*, bool) |
| 1930 | `0x0a6d5e` | `0x0a6d82` | +36 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN7Station8setItemsEP5ArrayIP4ItemEb`<br>Station::setItems(Array<Item*>*, bool) |
| 1931 | `0x0a6dce` | `0x0a6df2` | +36 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN7Station9setAgentsEP5ArrayIP5AgentE`<br>Station::setAgents(Array<Agent*>*) |
| 1932 | `0x0a6df2` | `0x0a6e16` | +36 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP5AgentEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Agent*>(Array<Agent*>&) |
| 1933 | `0x0a6e42` | `0x0a6e66` | +36 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN7Station7hasItemEi`<br>Station::hasItem(int) |
| 1934 | `0x0a6e7c` | `0x0a6ea0` | +36 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN7Station7hasShipEi`<br>Station::hasShip(int) |
| 1935 | `0x0a6eb6` | `0x0a6eda` | +36 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN7Station7addItemEP4Item`<br>Station::addItem(Item*) |
| 1936 | `0x0a6f36` | `0x0a6f5a` | +36 | 110 | 110 | 0 | 100.0% |  | Y | `_ZN7Station7addShipEP4Ship`<br>Station::addShip(Ship*) |
| 1937 | `0x0a6fa4` | `0x0a6fc8` | +36 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN7Station20stationHasPirateBaseEv`<br>Station::stationHasPirateBase() |
| 1938 | `0x0a6fe4` | `0x0a7008` | +36 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN7Station21getPirateStationIndexEv`<br>Station::getPirateStationIndex() |
| 1939 | `0x0a700c` | `0x0a7030` | +36 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN7Station25stationHasHiddenBlueprintEb`<br>Station::stationHasHiddenBlueprint(bool) |
| 1940 | `0x0a7050` | `0x0a7074` | +36 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN7Station23getHiddenBlueprintIndexEv`<br>Station::getHiddenBlueprintIndex() |
| 1941 | `0x0a7078` | `0x0a709c` | +36 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN7Station5cloneEv`<br>Station::clone() |
| 1942 | `0x0a70ec` | `0x0a7110` | +36 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN7Station6equalsEPS_`<br>Station::equals(Station*) |
| 1943 | `0x0a7100` | `0x0a7124` | +36 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN10RepairBeamC1Eii`<br>RepairBeam::RepairBeam(int, int) |
| 1944 | `0x0a7100` | `0x0a7124` | +36 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN10RepairBeamC2Eii`<br>RepairBeam::RepairBeam(int, int) |
| 1945 | `0x0a721c` | `0x0a7240` | +36 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP10AEGeometryEvjR5ArrayIT_E`<br>void ArraySetLength<AEGeometry*>(unsigned int, Array<AEGeometry*>&) |
| 1946 | `0x0a7250` | `0x0a7274` | +36 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIfEC2Ev`<br>Array<float>::Array() |
| 1947 | `0x0a726c` | `0x0a7290` | +36 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIfEvjR5ArrayIT_E`<br>void ArraySetLength<float>(unsigned int, Array<float>&) |
| 1948 | `0x0a72a0` | `0x0a72c4` | +36 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN10RepairBeamD2Ev`<br>RepairBeam::~RepairBeam() |
| 1949 | `0x0a72a0` | `0x0a72c4` | +36 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN10RepairBeamD1Ev`<br>RepairBeam::~RepairBeam() |
| 1950 | `0x0a72e0` | `0x0a7304` | +36 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIfED2Ev`<br>Array<float>::~Array() |
| 1951 | `0x0a72f8` | `0x0a731c` | +36 | 1872 | 1872 | 0 | 100.0% |  | Y | `_ZN10RepairBeam6updateEiP5RadarP5LevelP3Hud`<br>RepairBeam::update(int, Radar*, Level*, Hud*) |
| 1952 | `0x0a7a48` | `0x0a7a6c` | +36 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN10RepairBeam6renderEv`<br>RepairBeam::render() |
| 1953 | `0x0a7a74` | `0x0a7a98` | +36 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN9ImagePartC1Ejii`<br>ImagePart::ImagePart(unsigned int, int, int) |
| 1954 | `0x0a7a74` | `0x0a7a98` | +36 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN9ImagePartC2Ejii`<br>ImagePart::ImagePart(unsigned int, int, int) |
| 1955 | `0x0a7aa8` | `0x0a7acc` | +36 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9ImagePartD2Ev`<br>ImagePart::~ImagePart() |
| 1956 | `0x0a7aa8` | `0x0a7acc` | +36 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9ImagePartD1Ev`<br>ImagePart::~ImagePart() |
| 1957 | `0x0a7aac` | `0x0a7ad0` | +36 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN9ImagePart4drawEiib`<br>ImagePart::draw(int, int, bool) |
| 1958 | `0x0a7af4` | `0x0a7b18` | +36 | 924 | 924 | 0 | 100.0% |  | Y | `_ZN9ExplosionC2Ei`<br>Explosion::Explosion(int) |
| 1959 | `0x0a7af4` | `0x0a7b18` | +36 | 924 | 924 | 0 | 100.0% |  | Y | `_ZN9ExplosionC1Ei`<br>Explosion::Explosion(int) |
| 1960 | `0x0a7e90` | `0x0a7eb4` | +36 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN9Explosion10setScalingEf`<br>Explosion::setScaling(float) |
| 1961 | `0x0a7fb0` | `0x0a7fd4` | +36 | 296 | 296 | 0 | 100.0% |  | Y | `_ZN9Explosion5resetEv`<br>Explosion::reset() |
| 1962 | `0x0a80d8` | `0x0a80fc` | +36 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9ExplosionD2Ev`<br>Explosion::~Explosion() |
| 1963 | `0x0a80d8` | `0x0a80fc` | +36 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN9ExplosionD1Ev`<br>Explosion::~Explosion() |
| 1964 | `0x0a811c` | `0x0a8140` | +36 | 392 | 392 | 0 | 100.0% |  | Y | `_ZN9Explosion14addFireStreaksEv`<br>Explosion::addFireStreaks() |
| 1965 | `0x0a82a4` | `0x0a82c8` | +36 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9Explosion14setWeaponIndexEi`<br>Explosion::setWeaponIndex(int) |
| 1966 | `0x0a82a8` | `0x0a82cc` | +36 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN9Explosion9playSoundEPN11AbyssEngine6AEMath6VectorE`<br>Explosion::playSound(AbyssEngine::AEMath::Vector*) |
| 1967 | `0x0a83b0` | `0x0a83d4` | +36 | 396 | 396 | 0 | 100.0% |  | Y | `_ZN9Explosion5startERKN11AbyssEngine6AEMath6VectorES4_`<br>Explosion::start(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 1968 | `0x0a853c` | `0x0a8560` | +36 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN9Explosion5startERKN11AbyssEngine6AEMath6MatrixE`<br>Explosion::start(AbyssEngine::AEMath::Matrix const&) |
| 1969 | `0x0a8668` | `0x0a868c` | +36 | 440 | 440 | 0 | 100.0% |  | Y | `_ZN9Explosion6updateEiP18TargetFollowCamera`<br>Explosion::update(int, TargetFollowCamera*) |
| 1970 | `0x0a8820` | `0x0a8844` | +36 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN9Explosion6updateEiRKN11AbyssEngine6AEMath6VectorE`<br>Explosion::update(int, AbyssEngine::AEMath::Vector const&) |
| 1971 | `0x0a8900` | `0x0a8924` | +36 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN9Explosion11peakReachedEv`<br>Explosion::peakReached() |
| 1972 | `0x0a8916` | `0x0a893a` | +36 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9Explosion9isPlayingEv`<br>Explosion::isPlaying() |
| 1973 | `0x0a891c` | `0x0a8940` | +36 | 388 | 388 | 0 | 100.0% |  | Y | `_ZN9Explosion6renderEv`<br>Explosion::render() |
| 1974 | `0x0a8aa0` | `0x0a8ac4` | +36 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN9Explosion9translateERKN11AbyssEngine6AEMath6VectorE`<br>Explosion::translate(AbyssEngine::AEMath::Vector const&) |
| 1975 | `0x0a8ac0` | `0x0a8ae2` | +34 | 490 | 490 | 0 | 100.0% |  | Y | `_ZN6StatusC1Ev`<br>Status::Status() |
| 1976 | `0x0a8ac0` | `0x0a8ae2` | +34 | 490 | 490 | 0 | 100.0% |  | Y | `_ZN6StatusC2Ev`<br>Status::Status() |
| 1977 | `0x0a8caa` | `0x0a8ccc` | +34 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP7MissionEC2Ev`<br>Array<Mission*>::Array() |
| 1978 | `0x0a8cc6` | `0x0a8ce8` | +34 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP7StationEC2Ev`<br>Array<Station*>::Array() |
| 1979 | `0x0a8ce2` | `0x0a8d04` | +34 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIbEC2Ev`<br>Array<bool>::Array() |
| 1980 | `0x0a8cfe` | `0x0a8d20` | +34 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP7MissionEvjR5ArrayIT_E`<br>void ArraySetLength<Mission*>(unsigned int, Array<Mission*>&) |
| 1981 | `0x0a8d32` | `0x0a8d54` | +34 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP7StationEvjR5ArrayIT_E`<br>void ArraySetLength<Station*>(unsigned int, Array<Station*>&) |
| 1982 | `0x0a8d66` | `0x0a8d88` | +34 | 48 | 48 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIbEvjR5ArrayIT_E`<br>void ArraySetLength<bool>(unsigned int, Array<bool>&) |
| 1983 | `0x0a8d96` | `0x0a8db8` | +34 | 146 | 146 | 0 | 100.0% |  | Y | `_ZN6StatusD1Ev`<br>Status::~Status() |
| 1984 | `0x0a8d96` | `0x0a8db8` | +34 | 146 | 146 | 0 | 100.0% |  | Y | `_ZN6StatusD2Ev`<br>Status::~Status() |
| 1985 | `0x0a8e28` | `0x0a8e4a` | +34 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP7StationEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Station*>(Array<Station*>&) |
| 1986 | `0x0a8e78` | `0x0a8e9a` | +34 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP7StationED2Ev`<br>Array<Station*>::~Array() |
| 1987 | `0x0a8e8e` | `0x0a8eb0` | +34 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP6WantedEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Wanted*>(Array<Wanted*>&) |
| 1988 | `0x0a8ede` | `0x0a8f00` | +34 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP6WantedED2Ev`<br>Array<Wanted*>::~Array() |
| 1989 | `0x0a8ef4` | `0x0a8f16` | +34 | 158 | 158 | 0 | 100.0% |  | Y | `_ZN6Status17addPendingProductEP9BluePrint`<br>Status::addPendingProduct(BluePrint*) |
| 1990 | `0x0a8f92` | `0x0a8fb4` | +34 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14PendingProductEC2Ev`<br>Array<PendingProduct*>::Array() |
| 1991 | `0x0a8fae` | `0x0a8fd0` | +34 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP14PendingProductEvT_R5ArrayIS2_E`<br>void ArrayAdd<PendingProduct*>(PendingProduct*, Array<PendingProduct*>&) |
| 1992 | `0x0a8fd2` | `0x0a8ff4` | +34 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status18getPendingProductsEv`<br>Status::getPendingProducts() |
| 1993 | `0x0a8fd6` | `0x0a8ff8` | +34 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status21getSystemVisibilitiesEv`<br>Status::getSystemVisibilities() |
| 1994 | `0x0a8fda` | `0x0a8ffc` | +34 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN6Status19setSystemVisibilityEib`<br>Status::setSystemVisibility(int, bool) |
| 1995 | `0x0a8fe2` | `0x0a9004` | +34 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status15getStationStackEv`<br>Status::getStationStack() |
| 1996 | `0x0a8fe8` | `0x0a900a` | +34 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status15setStationStackEP5ArrayIP7StationE`<br>Status::setStationStack(Array<Station*>*) |
| 1997 | `0x0a8fee` | `0x0a9010` | +34 | 130 | 130 | 0 | 100.0% |  | Y | `_ZN6Status17addStationToStackEP7Station`<br>Status::addStationToStack(Station*) |
| 1998 | `0x0a9070` | `0x0a9092` | +34 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN6Status9isOnStackEP7Station`<br>Status::isOnStack(Station*) |
| 1999 | `0x0a90b0` | `0x0a90d0` | +32 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN6Status10setStationEP7Station`<br>Status::setStation(Station*) |
| 2000 | `0x0a924c` | `0x0a926c` | +32 | 1008 | 1008 | 0 | 100.0% |  | Y | `_ZN6Status13departStationEP7Station`<br>Status::departStation(Station*) |
| 2001 | `0x0a963c` | `0x0a965c` | +32 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN6Status12inAlienOrbitEv`<br>Status::inAlienOrbit() |
| 2002 | `0x0a9646` | `0x0a9666` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status10getStationEv`<br>Status::getStation() |
| 2003 | `0x0a964c` | `0x0a966c` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status14getPlayingTimeEv`<br>Status::getPlayingTime() |
| 2004 | `0x0a9652` | `0x0a9672` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status10setMissionEP7Mission`<br>Status::setMission(Mission*) |
| 2005 | `0x0a9658` | `0x0a9678` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status7getShipEv`<br>Status::getShip() |
| 2006 | `0x0a965e` | `0x0a967e` | +32 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN6Status7gameWonEv`<br>Status::gameWon() |
| 2007 | `0x0a966c` | `0x0a968c` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status25getCurrentCampaignMissionEv`<br>Status::getCurrentCampaignMission() |
| 2008 | `0x0a9672` | `0x0a9692` | +32 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN6Status18getCampaignMissionEv`<br>Status::getCampaignMission() |
| 2009 | `0x0a967c` | `0x0a969c` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status10getMissionEv`<br>Status::getMission() |
| 2010 | `0x0a9684` | `0x0a96a4` | +32 | 860 | 860 | 0 | 100.0% |  | Y | `_ZN6Status10moveWantedEv`<br>Status::moveWanted() |
| 2011 | `0x0a99e0` | `0x0a9a00` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status13getPassengersEv`<br>Status::getPassengers() |
| 2012 | `0x0a99e4` | `0x0a9a04` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status13setPassengersEi`<br>Status::setPassengers(int) |
| 2013 | `0x0a99e8` | `0x0a9a08` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status11getMissionsEv`<br>Status::getMissions() |
| 2014 | `0x0a99ee` | `0x0a9a0e` | +32 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN6Status19getFreelanceMissionEv`<br>Status::getFreelanceMission() |
| 2015 | `0x0a99f8` | `0x0a9a18` | +32 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN6Status19setFreelanceMissionEP7Mission`<br>Status::setFreelanceMission(Mission*) |
| 2016 | `0x0a9a04` | `0x0a9a24` | +32 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN6Status18setCampaignMissionEP7Mission`<br>Status::setCampaignMission(Mission*) |
| 2017 | `0x0a9a48` | `0x0a9a68` | +32 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN6Status19getNumberOfMissionsEv`<br>Status::getNumberOfMissions() |
| 2018 | `0x0a9a76` | `0x0a9a96` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status14getMaxMissionsEv`<br>Status::getMaxMissions() |
| 2019 | `0x0a9a7a` | `0x0a9a9a` | +32 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Status15incMissionCountEv`<br>Status::incMissionCount() |
| 2020 | `0x0a9a86` | `0x0a9aa6` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status25setCurrentCampaignMissionEi`<br>Status::setCurrentCampaignMission(int) |
| 2021 | `0x0a9a8c` | `0x0a9aac` | +32 | 8492 | 8492 | 0 | 100.0% |  | Y | `_ZN6Status19nextCampaignMissionEb`<br>Status::nextCampaignMission(bool) |
| 2022 | `0x0abbb8` | `0x0abbd8` | +32 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN6Status13changeCreditsEi`<br>Status::changeCredits(int) |
| 2023 | `0x0abbdc` | `0x0abbfc` | +32 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN6Status7setShipEP4Ship`<br>Status::setShip(Ship*) |
| 2024 | `0x0abbfe` | `0x0abc1e` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status18setStationsVisitedEi`<br>Status::setStationsVisited(int) |
| 2025 | `0x0abc04` | `0x0abc24` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status18getStationsVisitedEv`<br>Status::getStationsVisited() |
| 2026 | `0x0abc0a` | `0x0abc2a` | +32 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN6Status7dlc1WonEv`<br>Status::dlc1Won() |
| 2027 | `0x0abc18` | `0x0abc38` | +32 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN6Status12inEmptyOrbitEv`<br>Status::inEmptyOrbit() |
| 2028 | `0x0abc9c` | `0x0abcbc` | +32 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6Status17inPlanetRingOrbitEv`<br>Status::inPlanetRingOrbit() |
| 2029 | `0x0abcd0` | `0x0abcf0` | +32 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN6Status18orbitHasPlanetRingEi`<br>Status::orbitHasPlanetRing(int) |
| 2030 | `0x0abcf2` | `0x0abd12` | +32 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN6Status12inStormOrbitEv`<br>Status::inStormOrbit() |
| 2031 | `0x0abd34` | `0x0abd54` | +32 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN6Status17inSupernovaSystemEv`<br>Status::inSupernovaSystem() |
| 2032 | `0x0abd60` | `0x0abd80` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status9getSystemEv`<br>Status::getSystem() |
| 2033 | `0x0abd66` | `0x0abd86` | +32 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN6Status16inFogSkyboxOrbitEv`<br>Status::inFogSkyboxOrbit() |
| 2034 | `0x0abd98` | `0x0abdb8` | +32 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN6Status16inSupernovaOrbitEv`<br>Status::inSupernovaOrbit() |
| 2035 | `0x0abdba` | `0x0abdda` | +32 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN6Status18inDeepScienceOrbitEv`<br>Status::inDeepScienceOrbit() |
| 2036 | `0x0abdec` | `0x0abe0c` | +32 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN6Status19inBlackMarketSystemEv`<br>Status::inBlackMarketSystem() |
| 2037 | `0x0abe0e` | `0x0abe2e` | +32 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN6Status17inPirateLootOrbitEv`<br>Status::inPirateLootOrbit() |
| 2038 | `0x0abe40` | `0x0abe60` | +32 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN6Status12hardCoreModeEv`<br>Status::hardCoreMode() |
| 2039 | `0x0abe64` | `0x0abe84` | +32 | 130 | 130 | 0 | 100.0% |  | Y | `_ZN6Status23getWantedInCurrentOrbitEv`<br>Status::getWantedInCurrentOrbit() |
| 2040 | `0x0abee6` | `0x0abf06` | +32 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN6Status13missionFailedEbx`<br>Status::missionFailed(bool, long long) |
| 2041 | `0x0abf38` | `0x0abf58` | +32 | 1432 | 1432 | 0 | 86.3% |  |  | `_ZN6Status16missionCompletedEbbx`<br>Status::missionCompleted(bool, bool, long long) |
| 2042 | `0x0ac4d0` | `0x0ac4f0` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status15setJumpgateUsedEi`<br>Status::setJumpgateUsed(int) |
| 2043 | `0x0ac4d6` | `0x0ac4f6` | +32 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Status12jumpgateUsedEv`<br>Status::jumpgateUsed() |
| 2044 | `0x0ac4e2` | `0x0ac502` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status15getJumpgateUsedEv`<br>Status::getJumpgateUsed() |
| 2045 | `0x0ac4e8` | `0x0ac508` | +32 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Status13crateCapturedEi`<br>Status::crateCaptured(int) |
| 2046 | `0x0ac4f4` | `0x0ac514` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status17setCapturedCratesEi`<br>Status::setCapturedCrates(int) |
| 2047 | `0x0ac4fa` | `0x0ac51a` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status17getCapturedCratesEv`<br>Status::getCapturedCrates() |
| 2048 | `0x0ac500` | `0x0ac520` | +32 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Status18incEquipmentBoughtEv`<br>Status::incEquipmentBought() |
| 2049 | `0x0ac50c` | `0x0ac52c` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status18setBoughtEquipmentEi`<br>Status::setBoughtEquipment(int) |
| 2050 | `0x0ac512` | `0x0ac532` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status18getBoughtEquipmentEv`<br>Status::getBoughtEquipment() |
| 2051 | `0x0ac518` | `0x0ac538` | +32 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN6Status13removeMissionEP7Mission`<br>Status::removeMission(Mission*) |
| 2052 | `0x0ac5b4` | `0x0ac5d4` | +32 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Status12visitStationEv`<br>Status::visitStation() |
| 2053 | `0x0ac5c0` | `0x0ac5e0` | +32 | 20 | 20 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIiEvR5ArrayIT_E`<br>void ArrayRelease<int>(Array<int>&) |
| 2054 | `0x0ac5d4` | `0x0ac5f4` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status14getPlanetNamesEv`<br>Status::getPlanetNames() |
| 2055 | `0x0ac5da` | `0x0ac5fa` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status17getPlanetTexturesEv`<br>Status::getPlanetTextures() |
| 2056 | `0x0ac5e0` | `0x0ac600` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status10getCreditsEv`<br>Status::getCredits() |
| 2057 | `0x0ac5e6` | `0x0ac606` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status9setRatingEi`<br>Status::setRating(int) |
| 2058 | `0x0ac5ec` | `0x0ac60c` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status14setPlayingTimeEx`<br>Status::setPlayingTime(long long) |
| 2059 | `0x0ac5f2` | `0x0ac612` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status8setKillsEi`<br>Status::setKills(int) |
| 2060 | `0x0ac5f8` | `0x0ac618` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status15setMissionCountEi`<br>Status::setMissionCount(int) |
| 2061 | `0x0ac5fe` | `0x0ac61e` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status8setLevelEi`<br>Status::setLevel(int) |
| 2062 | `0x0ac604` | `0x0ac624` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status9setLastXPEi`<br>Status::setLastXP(int) |
| 2063 | `0x0ac60a` | `0x0ac62a` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status16setGoodsProducedEi`<br>Status::setGoodsProduced(int) |
| 2064 | `0x0ac610` | `0x0ac630` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status16getGoodsProducedEv`<br>Status::getGoodsProduced() |
| 2065 | `0x0ac616` | `0x0ac636` | +32 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Status16incGoodsProducedEi`<br>Status::incGoodsProduced(int) |
| 2066 | `0x0ac622` | `0x0ac642` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status10setCreditsEi`<br>Status::setCredits(int) |
| 2067 | `0x0ac628` | `0x0ac648` | +32 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN6Status15checkForLevelUpEv`<br>Status::checkForLevelUp() |
| 2068 | `0x0ac68c` | `0x0ac6ac` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status9getRatingEv`<br>Status::getRating() |
| 2069 | `0x0ac692` | `0x0ac6b2` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status9getLastXPEv`<br>Status::getLastXP() |
| 2070 | `0x0ac698` | `0x0ac6b8` | +32 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN6Status12changeRatingEi`<br>Status::changeRating(int) |
| 2071 | `0x0ac6bc` | `0x0ac6dc` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status8getKillsEv`<br>Status::getKills() |
| 2072 | `0x0ac6c4` | `0x0ac6e4` | +32 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN6Status8incKillsEv`<br>Status::incKills() |
| 2073 | `0x0ac6e0` | `0x0ac700` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status14setPirateKillsEi`<br>Status::setPirateKills(int) |
| 2074 | `0x0ac6e6` | `0x0ac706` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status14getPirateKillsEv`<br>Status::getPirateKills() |
| 2075 | `0x0ac6ec` | `0x0ac70c` | +32 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN6Status14incPirateKillsEv`<br>Status::incPirateKills() |
| 2076 | `0x0ac708` | `0x0ac728` | +32 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Status8addKillsEi`<br>Status::addKills(int) |
| 2077 | `0x0ac714` | `0x0ac734` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status15getMissionCountEv`<br>Status::getMissionCount() |
| 2078 | `0x0ac71a` | `0x0ac73a` | +32 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Status8getLevelEv`<br>Status::getLevel() |
| 2079 | `0x0ac720` | `0x0ac740` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status11getStandingEv`<br>Status::getStanding() |
| 2080 | `0x0ac724` | `0x0ac744` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status13getBluePrintsEv`<br>Status::getBluePrints() |
| 2081 | `0x0ac728` | `0x0ac748` | +32 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN6Status15unlockBluePrintEi`<br>Status::unlockBluePrint(int) |
| 2082 | `0x0ac762` | `0x0ac782` | +32 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN6Status19isBlueprintUnlockedEi`<br>Status::isBlueprintUnlocked(int) |
| 2083 | `0x0ac7a6` | `0x0ac7c6` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status9getAgentsEv`<br>Status::getAgents() |
| 2084 | `0x0ac7aa` | `0x0ac7ca` | +32 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN6Status14incPlayingTimeEx`<br>Status::incPlayingTime(long long) |
| 2085 | `0x0ac7ba` | `0x0ac7da` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status10getWingmenEv`<br>Status::getWingmen() |
| 2086 | `0x0ac7be` | `0x0ac7de` | +32 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN6Status10setWingmenEP5ArrayIPN11AbyssEngine6StringEE`<br>Status::setWingmen(Array<AbyssEngine::String*>*) |
| 2087 | `0x0ac83c` | `0x0ac85c` | +32 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN6Status11replaceHashEN11AbyssEngine6StringES1_`<br>Status::replaceHash(AbyssEngine::String, AbyssEngine::String) |
| 2088 | `0x0ac8d4` | `0x0ac8f4` | +32 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN6Status11replaceHashEN11AbyssEngine6StringES1_S1_`<br>Status::replaceHash(AbyssEngine::String, AbyssEngine::String, AbyssEngine::String) |
| 2089 | `0x0ac9f4` | `0x0aca14` | +32 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN6Status14stringHasTokenEN11AbyssEngine6StringES1_`<br>Status::stringHasToken(AbyssEngine::String, AbyssEngine::String) |
| 2090 | `0x0aca10` | `0x0aca30` | +32 | 708 | 708 | 0 | 100.0% |  | Y | `_ZN6Status15calcCargoPricesEv`<br>Status::calcCargoPrices() |
| 2091 | `0x0accd4` | `0x0accf4` | +32 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN6Status10loadAgentsEv`<br>Status::loadAgents() |
| 2092 | `0x0acd0a` | `0x0acd2a` | +32 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN6Status10loadAgentsEP5ArrayIiE`<br>Status::loadAgents(Array<int>*) |
| 2093 | `0x0acd0c` | `0x0acd2c` | +32 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN6Status20getCollectedBountiesEi`<br>Status::getCollectedBounties(int) |
| 2094 | `0x0acd1a` | `0x0acd3a` | +32 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN6Status20incCollectedBountiesEi`<br>Status::incCollectedBounties(int) |
| 2095 | `0x0acd2c` | `0x0acd4c` | +32 | 72 | 72 | 0 | 100.0% | Y | Y | `_ZN6Status25isFreighterMissionStationEi`<br>Status::isFreighterMissionStation(int) |
| 2096 | `0x0acd74` | `0x0acd94` | +32 | 74 | 74 | 0 | 100.0% | Y | Y | `_ZN6Status29getFreighterMissionStationBitEi`<br>Status::getFreighterMissionStationBit(int) |
| 2097 | `0x0acdc0` | `0x0acde0` | +32 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN6Status26getGammaRayDamagePerSecondEii`<br>Status::getGammaRayDamagePerSecond(int, int) |
| 2098 | `0x0ace18` | `0x0ace38` | +32 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN6Status10loadWantedEv`<br>Status::loadWanted() |
| 2099 | `0x0ace4e` | `0x0ace6e` | +32 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Status9getWantedEv`<br>Status::getWanted() |
| 2100 | `0x0ace52` | `0x0ace72` | +32 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN6Status17isStorylineWantedEi`<br>Status::isStorylineWanted(int) |
| 2101 | `0x0ace5c` | `0x0ace7c` | +32 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN6Status21wantedBoardAccessibleEv`<br>Status::wantedBoardAccessible() |
| 2102 | `0x0acedc` | `0x0acefc` | +32 | 960 | 960 | 0 | 100.0% |  | Y | `_ZN6Status17activateNewWantedEv`<br>Status::activateNewWanted() |
| 2103 | `0x0ad29c` | `0x0ad2bc` | +32 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP11SolarSystemEvR5ArrayIT_E`<br>void ArrayReleaseClasses<SolarSystem*>(Array<SolarSystem*>&) |
| 2104 | `0x0ad2ec` | `0x0ad30c` | +32 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11SolarSystemED2Ev`<br>Array<SolarSystem*>::~Array() |
| 2105 | `0x0ad304` | `0x0ad324` | +32 | 1684 | 1684 | 0 | 100.0% |  | Y | `_ZN6Status9resetGameEv`<br>Status::resetGame() |
| 2106 | `0x0ad998` | `0x0ad9b8` | +32 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN5ArrayIbED2Ev`<br>Array<bool>::~Array() |
| 2107 | `0x0ad9ac` | `0x0ad9cc` | +32 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP9BluePrintEvR5ArrayIT_E`<br>void ArrayReleaseClasses<BluePrint*>(Array<BluePrint*>&) |
| 2108 | `0x0ad9fc` | `0x0ada1c` | +32 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9BluePrintED2Ev`<br>Array<BluePrint*>::~Array() |
| 2109 | `0x0ada12` | `0x0ada32` | +32 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9BluePrintEC2Ev`<br>Array<BluePrint*>::Array() |
| 2110 | `0x0ada2e` | `0x0ada4e` | +32 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP9BluePrintEvjR5ArrayIT_E`<br>void ArraySetLength<BluePrint*>(unsigned int, Array<BluePrint*>&) |
| 2111 | `0x0ada62` | `0x0ada82` | +32 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP14PendingProductEvR5ArrayIT_E`<br>void ArrayReleaseClasses<PendingProduct*>(Array<PendingProduct*>&) |
| 2112 | `0x0adab2` | `0x0adad2` | +32 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14PendingProductED2Ev`<br>Array<PendingProduct*>::~Array() |
| 2113 | `0x0adac8` | `0x0adae8` | +32 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN5LevelC2Ei`<br>Level::Level(int) |
| 2114 | `0x0adac8` | `0x0adae8` | +32 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN5LevelC1Ei`<br>Level::Level(int) |
| 2115 | `0x0adbd0` | `0x0adbf0` | +32 | 652 | 652 | 0 | 100.0% |  | Y | `_ZN5LevelD1Ev`<br>Level::~Level() |
| 2116 | `0x0adbd0` | `0x0adbf0` | +32 | 652 | 652 | 0 | 100.0% |  | Y | `_ZN5LevelD2Ev`<br>Level::~Level() |
| 2117 | `0x0ade5c` | `0x0ade7c` | +32 | 74 | 74 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP11AbstractGunEvR5ArrayIT_E`<br>void ArrayReleaseClasses<AbstractGun*>(Array<AbstractGun*>&) |
| 2118 | `0x0adea6` | `0x0adec6` | +32 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11AbstractGunED2Ev`<br>Array<AbstractGun*>::~Array() |
| 2119 | `0x0adebc` | `0x0adedc` | +32 | 74 | 74 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP8KIPlayerEvR5ArrayIT_E`<br>void ArrayReleaseClasses<KIPlayer*>(Array<KIPlayer*>&) |
| 2120 | `0x0adf06` | `0x0adf26` | +32 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8KIPlayerED2Ev`<br>Array<KIPlayer*>::~Array() |
| 2121 | `0x0adf1c` | `0x0adf3c` | +32 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP12RadioMessageEvR5ArrayIT_E`<br>void ArrayReleaseClasses<RadioMessage*>(Array<RadioMessage*>&) |
| 2122 | `0x0adf6c` | `0x0adf8c` | +32 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP12RadioMessageED2Ev`<br>Array<RadioMessage*>::~Array() |
| 2123 | `0x0adf84` | `0x0adfa4` | +32 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN5Level16setInitStreamOutEv`<br>Level::setInitStreamOut() |
| 2124 | `0x0adf94` | `0x0adfb4` | +32 | 1568 | 1568 | 0 | 100.0% |  | Y | `_ZN5Level4initEv`<br>Level::init() |
| 2125 | `0x0ae5b8` | `0x0ae5d8` | +32 | 3076 | 3076 | 0 | 100.0% |  | Y | `_ZN5Level11createSpaceEv`<br>Level::createSpace() |
| 2126 | `0x0af1bc` | `0x0af1dc` | +32 | 1340 | 1340 | 0 | 100.0% |  | Y | `_ZN5Level12createPlayerEv`<br>Level::createPlayer() |
| 2127 | `0x0af6f8` | `0x0af718` | +32 | 1588 | 1588 | 0 | 100.0% |  | Y | `_ZN5Level15createAsteroidsEv`<br>Level::createAsteroids() |
| 2128 | `0x0afd2c` | `0x0afd4c` | +32 | 684 | 684 | 0 | 100.0% |  | Y | `_ZN5Level15createGasCloudsEv`<br>Level::createGasClouds() |
| 2129 | `0x0affd8` | `0x0afff8` | +32 | 17172 | 17176 | +4 | 80.4% |  |  | `_ZN5Level13createMissionEv`<br>Level::createMission() |
| 2130 | `0x0b42ec` | `0x0b4310` | +36 | 2252 | 2252 | 0 | 87.2% |  |  | `_ZN5Level11createSceneEv`<br>Level::createScene() |
| 2131 | `0x0b4bb8` | `0x0b4be0` | +40 | 29676 | 29676 | 0 | 91.6% |  |  | `_ZN5Level21createCampaignMissionEv`<br>Level::createCampaignMission() |
| 2132 | `0x0bbfa4` | `0x0bbfcc` | +40 | 592 | 592 | 0 | 100.0% |  | Y | `_ZN5Level19createStaticObjectsEv`<br>Level::createStaticObjects() |
| 2133 | `0x0bc1f4` | `0x0bc21c` | +40 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN5Level16createSentryGunsEv`<br>Level::createSentryGuns() |
| 2134 | `0x0bc2dc` | `0x0bc304` | +40 | 272 | 272 | 0 | 100.0% |  | Y | `_ZN5Level20createFighterTurretsEv`<br>Level::createFighterTurrets() |
| 2135 | `0x0bc3ec` | `0x0bc414` | +40 | 660 | 660 | 0 | 100.0% |  | Y | `_ZN5Level13createWingmenEv`<br>Level::createWingmen() |
| 2136 | `0x0bc680` | `0x0bc6a8` | +40 | 2948 | 2948 | 0 | 100.0% |  | Y | `_ZN5Level10assignGunsEv`<br>Level::assignGuns() |
| 2137 | `0x0bd204` | `0x0bd22c` | +40 | 1160 | 1160 | 0 | 100.0% |  | Y | `_ZN5Level14connectPlayersEv`<br>Level::connectPlayers() |
| 2138 | `0x0bd68c` | `0x0bd6b4` | +40 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN5Level21enableParticleEffectsEbb`<br>Level::enableParticleEffects(bool, bool) |
| 2139 | `0x0bd6b8` | `0x0bd6e0` | +40 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN5Level20setPlayerEngineColorEs`<br>Level::setPlayerEngineColor(short) |
| 2140 | `0x0bd6f8` | `0x0bd720` | +40 | 1872 | 1872 | 0 | 100.0% |  | Y | `_ZN5Level19initParticleSystemsEv`<br>Level::initParticleSystems() |
| 2141 | `0x0bde48` | `0x0bde70` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level13getStarSystemEv`<br>Level::getStarSystem() |
| 2142 | `0x0bde4e` | `0x0bde76` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8KIPlayerEC2Ev`<br>Array<KIPlayer*>::Array() |
| 2143 | `0x0bde6a` | `0x0bde92` | +40 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP8KIPlayerEvjR5ArrayIT_E`<br>void ArraySetLength<KIPlayer*>(unsigned int, Array<KIPlayer*>&) |
| 2144 | `0x0bde9e` | `0x0bdec6` | +40 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level12getGasCloudsEv`<br>Level::getGasClouds() |
| 2145 | `0x0bdea4` | `0x0bdecc` | +40 | 2012 | 2012 | 0 | 100.0% |  | Y | `_ZN5Level9createGunEiiiiiiii`<br>Level::createGun(int, int, int, int, int, int, int, int) |
| 2146 | `0x0be680` | `0x0be6a8` | +40 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11AbstractGunEC2Ev`<br>Array<AbstractGun*>::Array() |
| 2147 | `0x0be69c` | `0x0be6c4` | +40 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP11AbstractGunEvT_R5ArrayIS2_E`<br>void ArrayAdd<AbstractGun*>(AbstractGun*, Array<AbstractGun*>&) |
| 2148 | `0x0be6c0` | `0x0be6e8` | +40 | 6960 | 6960 | 0 | 100.0% |  | Y | `_ZN5Level18createStaticObjectEP8Waypointib`<br>Level::createStaticObject(Waypoint*, int, bool) |
| 2149 | `0x0c01f0` | `0x0c0218` | +40 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP8KIPlayerEvT_R5ArrayIS2_E`<br>void ArrayAdd<KIPlayer*>(KIPlayer*, Array<KIPlayer*>&) |
| 2150 | `0x0c0214` | `0x0c023c` | +40 | 2764 | 2788 | +24 | 71.8% |  |  | `_ZN5Level10createShipEiiiP8Waypointbb`<br>Level::createShip(int, int, int, Waypoint*, bool, bool) |
| 2151 | `0x0c0ce0` | `0x0c0d20` | +64 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN5Level11createRouteEi`<br>Level::createRoute(int) |
| 2152 | `0x0c0dc8` | `0x0c0e08` | +64 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5Level14setPlayerRouteEP5Route`<br>Level::setPlayerRoute(Route*) |
| 2153 | `0x0c0de4` | `0x0c0e24` | +64 | 12282 | 12282 | 0 | 100.0% |  | Y | `_ZN5Level19createRadioMessagesEi`<br>Level::createRadioMessages(int) |
| 2154 | `0x0c3dde` | `0x0c3e1e` | +64 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP11AbstractGunEvjR5ArrayIT_E`<br>void ArraySetLength<AbstractGun*>(unsigned int, Array<AbstractGun*>&) |
| 2155 | `0x0c3e12` | `0x0c3e52` | +64 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP6PlayerEvjR5ArrayIT_E`<br>void ArraySetLength<Player*>(unsigned int, Array<Player*>&) |
| 2156 | `0x0c3e48` | `0x0c3e88` | +64 | 536 | 536 | 0 | 100.0% |  | Y | `_ZN5Level17getBoundingVolumeEiP10AEGeometry`<br>Level::getBoundingVolume(int, AEGeometry*) |
| 2157 | `0x0c4060` | `0x0c40a0` | +64 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP12RadioMessageEC2Ev`<br>Array<RadioMessage*>::Array() |
| 2158 | `0x0c407c` | `0x0c40bc` | +64 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP12RadioMessageEvjR5ArrayIT_E`<br>void ArraySetLength<RadioMessage*>(unsigned int, Array<RadioMessage*>&) |
| 2159 | `0x0c40b0` | `0x0c40f0` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level9getPlayerEv`<br>Level::getPlayer() |
| 2160 | `0x0c40b6` | `0x0c40f6` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level10getEnemiesEv`<br>Level::getEnemies() |
| 2161 | `0x0c40bc` | `0x0c40fc` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level12getLandmarksEv`<br>Level::getLandmarks() |
| 2162 | `0x0c40c2` | `0x0c4102` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level12getAsteroidsEv`<br>Level::getAsteroids() |
| 2163 | `0x0c40c8` | `0x0c4108` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level19getAsteroidWaypointEv`<br>Level::getAsteroidWaypoint() |
| 2164 | `0x0c40ce` | `0x0c410e` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level14getPlayerRouteEv`<br>Level::getPlayerRoute() |
| 2165 | `0x0c40d4` | `0x0c4114` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level13getEnemyRouteEv`<br>Level::getEnemyRoute() |
| 2166 | `0x0c40da` | `0x0c411a` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level14getFriendRouteEv`<br>Level::getFriendRoute() |
| 2167 | `0x0c40e0` | `0x0c4120` | +64 | 336 | 336 | 0 | 100.0% |  | Y | `_ZN5Level11flashScreenEi`<br>Level::flashScreen(int) |
| 2168 | `0x0c4230` | `0x0c4270` | +64 | 504 | 504 | 0 | 100.0% |  | Y | `_ZN5Level9enemyDiedEib`<br>Level::enemyDied(int, bool) |
| 2169 | `0x0c4428` | `0x0c4468` | +64 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN5Level8junkDiedEv`<br>Level::junkDied() |
| 2170 | `0x0c444c` | `0x0c448c` | +64 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN5Level10applyKillsEv`<br>Level::applyKills() |
| 2171 | `0x0c4478` | `0x0c44b8` | +64 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN5Level10friendDiedEv`<br>Level::friendDied() |
| 2172 | `0x0c4484` | `0x0c44c4` | +64 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN5Level11wingmanDiedERKN11AbyssEngine6StringE`<br>Level::wingmanDied(AbyssEngine::String const&) |
| 2173 | `0x0c44ec` | `0x0c452c` | +64 | 68 | 68 | 0 | 100.0% |  | Y | `_Z11ArrayRemoveIPN11AbyssEngine6StringEEvT_R5ArrayIS3_E`<br>void ArrayRemove<AbyssEngine::String*>(AbyssEngine::String*, Array<AbyssEngine::String*>&) |
| 2174 | `0x0c4530` | `0x0c4570` | +64 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN5Level12asteroidDiedEv`<br>Level::asteroidDied() |
| 2175 | `0x0c453c` | `0x0c457c` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level16getAsteroidsLeftEv`<br>Level::getAsteroidsLeft() |
| 2176 | `0x0c4542` | `0x0c4582` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level14getEnemiesLeftEv`<br>Level::getEnemiesLeft() |
| 2177 | `0x0c4548` | `0x0c4588` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level14getFriendsLeftEv`<br>Level::getFriendsLeft() |
| 2178 | `0x0c454e` | `0x0c458e` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level11getMessagesEv`<br>Level::getMessages() |
| 2179 | `0x0c4554` | `0x0c4594` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level12getTimeLimitEv`<br>Level::getTimeLimit() |
| 2180 | `0x0c455a` | `0x0c459a` | +64 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN5Level7collideEN11AbyssEngine6AEMath6VectorEb`<br>Level::collide(AbyssEngine::AEMath::Vector, bool) |
| 2181 | `0x0c456e` | `0x0c45ae` | +64 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN5Level23isInAsteroidCenterRangeEN11AbyssEngine6AEMath6VectorE`<br>Level::isInAsteroidCenterRange(AbyssEngine::AEMath::Vector) |
| 2182 | `0x0c457c` | `0x0c45bc` | +64 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN5Level13collideStreamEN11AbyssEngine6AEMath6VectorE`<br>Level::collideStream(AbyssEngine::AEMath::Vector) |
| 2183 | `0x0c4594` | `0x0c45d4` | +64 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN5Level14collideStationEN11AbyssEngine6AEMath6VectorE`<br>Level::collideStation(AbyssEngine::AEMath::Vector) |
| 2184 | `0x0c45e8` | `0x0c4628` | +64 | 1320 | 1320 | 0 | 100.0% |  | Y | `_ZN5Level8renderBGEi`<br>Level::renderBG(int) |
| 2185 | `0x0c4b10` | `0x0c4b50` | +64 | 400 | 400 | 0 | 100.0% |  | Y | `_ZN5Level6renderEi`<br>Level::render(int) |
| 2186 | `0x0c4ca0` | `0x0c4ce0` | +64 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN5Level8render2DEv`<br>Level::render2D() |
| 2187 | `0x0c4ca8` | `0x0c4ce8` | +64 | 214 | 214 | 0 | 100.0% | Y | Y | `_ZN5Level11renderPauseEx`<br>Level::renderPause(long long) |
| 2188 | `0x0c4d80` | `0x0c4dc0` | +64 | 1160 | 1160 | 0 | 100.0% |  | Y | `_ZN5Level18updateMissionOrbitEi`<br>Level::updateMissionOrbit(int) |
| 2189 | `0x0c5208` | `0x0c5248` | +64 | 968 | 968 | 0 | 100.0% |  | Y | `_ZN5Level11updateOrbitEi`<br>Level::updateOrbit(int) |
| 2190 | `0x0c55d0` | `0x0c5610` | +64 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN5Level15alarmAllFriendsEib`<br>Level::alarmAllFriends(int, bool) |
| 2191 | `0x0c566c` | `0x0c56ac` | +64 | 1592 | 1592 | 0 | 100.0% |  | Y | `_ZN5Level18createRadioMessageEii`<br>Level::createRadioMessage(int, int) |
| 2192 | `0x0c5ca4` | `0x0c5ce4` | +64 | 688 | 688 | 0 | 100.0% |  | Y | `_ZN5Level20updateAlienAttackersEi`<br>Level::updateAlienAttackers(int) |
| 2193 | `0x0c5f54` | `0x0c5f94` | +64 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5Level21updateAsteroidClusterEv`<br>Level::updateAsteroidCluster() |
| 2194 | `0x0c5f58` | `0x0c5f98` | +64 | 760 | 760 | 0 | 100.0% |  | Y | `_ZN5Level6updateExb`<br>Level::update(long long, bool) |
| 2195 | `0x0c6250` | `0x0c6290` | +64 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN5Level14checkObjectiveEi`<br>Level::checkObjective(int) |
| 2196 | `0x0c625c` | `0x0c629c` | +64 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN5Level16stealFriendCargoEv`<br>Level::stealFriendCargo() |
| 2197 | `0x0c6264` | `0x0c62a4` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level20friendCargoWasStolenEv`<br>Level::friendCargoWasStolen() |
| 2198 | `0x0c626a` | `0x0c62aa` | +64 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN5Level16removeObjectivesEv`<br>Level::removeObjectives() |
| 2199 | `0x0c6272` | `0x0c62b2` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level13getPlayerGunsEv`<br>Level::getPlayerGuns() |
| 2200 | `0x0c6278` | `0x0c62b8` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level12getEnemyGunsEv`<br>Level::getEnemyGuns() |
| 2201 | `0x0c627e` | `0x0c62be` | +64 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN5Level13checkGameOverEi`<br>Level::checkGameOver(int) |
| 2202 | `0x0c628a` | `0x0c62ca` | +64 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP12RadioMessageEvT_R5ArrayIS2_E`<br>void ArrayAdd<RadioMessage*>(RadioMessage*, Array<RadioMessage*>&) |
| 2203 | `0x0c62b0` | `0x0c62f0` | +64 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN5Level19pirateStationActionEb`<br>Level::pirateStationAction(bool) |
| 2204 | `0x0c6320` | `0x0c6360` | +64 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN5Level13uncoverWantedEi`<br>Level::uncoverWanted(int) |
| 2205 | `0x0c6388` | `0x0c63c8` | +64 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN5Level12attackWantedEi`<br>Level::attackWanted(int) |
| 2206 | `0x0c63f4` | `0x0c6434` | +64 | 252 | 252 | 0 | 100.0% |  | Y | `_ZN5Level16almostKillWantedEi`<br>Level::almostKillWanted(int) |
| 2207 | `0x0c64f0` | `0x0c6530` | +64 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5Level10killWantedEi`<br>Level::killWanted(int) |
| 2208 | `0x0c6506` | `0x0c6546` | +64 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5Level17friendTurnedEnemyEi`<br>Level::friendTurnedEnemy(int) |
| 2209 | `0x0c651c` | `0x0c655c` | +64 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN5Level9enableFogEb`<br>Level::enableFog(bool) |
| 2210 | `0x0c6528` | `0x0c6568` | +64 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN5Level17enableMovingStarsEb`<br>Level::enableMovingStars(bool) |
| 2211 | `0x0c653a` | `0x0c657a` | +64 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN5Level5resetEv`<br>Level::reset() |
| 2212 | `0x0c663c` | `0x0c667c` | +64 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN5Level20switchSkyboxForIntroEv`<br>Level::switchSkyboxForIntro() |
| 2213 | `0x0c668c` | `0x0c66cc` | +64 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN5Level32switchSkyboxForSupernovaReversalEv`<br>Level::switchSkyboxForSupernovaReversal() |
| 2214 | `0x0c66f8` | `0x0c6738` | +64 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN5Level20getNumDockingTargetsEv`<br>Level::getNumDockingTargets() |
| 2215 | `0x0c6726` | `0x0c6766` | +64 | 54 | 54 | 0 | 100.0% | Y | Y | `_ZN5Level16getDockingTargetEi`<br>Level::getDockingTarget(int) |
| 2216 | `0x0c675c` | `0x0c679c` | +64 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN5Level14hasMiningPlantEv`<br>Level::hasMiningPlant() |
| 2217 | `0x0c676a` | `0x0c67aa` | +64 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN5Level14getMiningPlantEv`<br>Level::getMiningPlant() |
| 2218 | `0x0c6780` | `0x0c67c0` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level18getNumDeliveredOreEv`<br>Level::getNumDeliveredOre() |
| 2219 | `0x0c6786` | `0x0c67c6` | +64 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN5Level18incNumDeliveredOreEi`<br>Level::incNumDeliveredOre(int) |
| 2220 | `0x0c6792` | `0x0c67d2` | +64 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Level25getNumDeliveredPassengersEv`<br>Level::getNumDeliveredPassengers() |
| 2221 | `0x0c6798` | `0x0c67d8` | +64 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN5Level25incNumDeliveredPassengersEi`<br>Level::incNumDeliveredPassengers(int) |
| 2222 | `0x0c67a4` | `0x0c67ec` | +72 | 1020 | 1020 | 0 | 100.0% |  | Y | `_ZN7StarMapC1EbP7Missionbi`<br>StarMap::StarMap(bool, Mission*, bool, int) |
| 2223 | `0x0c67a4` | `0x0c67ec` | +72 | 1020 | 1020 | 0 | 100.0% |  | Y | `_ZN7StarMapC2EbP7Missionbi`<br>StarMap::StarMap(bool, Mission*, bool, int) |
| 2224 | `0x0c6ba0` | `0x0c6be8` | +72 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPN11AbyssEngine6AEMath6VectorEEC2Ev`<br>Array<AbyssEngine::AEMath::Vector*>::Array() |
| 2225 | `0x0c6bbc` | `0x0c6c04` | +72 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIPN11AbyssEngine6AEMath6VectorEEvjR5ArrayIT_E`<br>void ArraySetLength<AbyssEngine::AEMath::Vector*>(unsigned int, Array<AbyssEngine::AEMath::Vector*>&) |
| 2226 | `0x0c6bf0` | `0x0c6c38` | +72 | 2220 | 2220 | 0 | 100.0% |  | Y | `_ZN7StarMap4initEbP7Missionbi`<br>StarMap::init(bool, Mission*, bool, int) |
| 2227 | `0x0c749c` | `0x0c74e4` | +72 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN7StarMapD2Ev`<br>StarMap::~StarMap() |
| 2228 | `0x0c749c` | `0x0c74e4` | +72 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN7StarMapD1Ev`<br>StarMap::~StarMap() |
| 2229 | `0x0c7580` | `0x0c75c8` | +72 | 2076 | 2076 | 0 | 100.0% |  | Y | `_ZN7StarMap14initStarSystemEv`<br>StarMap::initStarSystem() |
| 2230 | `0x0c7d9c` | `0x0c7de4` | +72 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN7StarMap10initLightsEv`<br>StarMap::initLights() |
| 2231 | `0x0c7dd8` | `0x0c7e20` | +72 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN7StarMap8setStartEii`<br>StarMap::setStart(int, int) |
| 2232 | `0x0c7e0c` | `0x0c7e54` | +72 | 564 | 564 | 0 | 100.0% |  | Y | `_ZN7StarMap6departEb`<br>StarMap::depart(bool) |
| 2233 | `0x0c8040` | `0x0c8088` | +72 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN7StarMap14isInPlanetModeEv`<br>StarMap::isInPlanetMode() |
| 2234 | `0x0c804c` | `0x0c8094` | +72 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN7StarMap14setJumpMapModeEbb`<br>StarMap::setJumpMapMode(bool, bool) |
| 2235 | `0x0c8056` | `0x0c809e` | +72 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN7StarMap8renderBGEv`<br>StarMap::renderBG() |
| 2236 | `0x0c8058` | `0x0c80a0` | +72 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN7StarMap6renderEv`<br>StarMap::render() |
| 2237 | `0x0c80e8` | `0x0c8130` | +72 | 620 | 620 | 0 | 100.0% |  | Y | `_ZN7StarMap7drawKeyEv`<br>StarMap::drawKey() |
| 2238 | `0x0c8354` | `0x0c839c` | +72 | 3488 | 3488 | 0 | 98.1% |  |  | `_ZN7StarMap16drawOnScreenInfoEib`<br>StarMap::drawOnScreenInfo(int, bool) |
| 2239 | `0x0c90f4` | `0x0c913c` | +72 | 2644 | 2648 | +4 | 92.9% |  |  | `_ZN7StarMap4drawEv`<br>StarMap::draw() |
| 2240 | `0x0c9b48` | `0x0c9b94` | +76 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7StarMap14missionChangedEv`<br>StarMap::missionChanged() |
| 2241 | `0x0c9b50` | `0x0c9b9c` | +76 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN7StarMap24askForJumpIntoAlienWorldEv`<br>StarMap::askForJumpIntoAlienWorld() |
| 2242 | `0x0c9b90` | `0x0c9bd8` | +72 | 3132 | 3132 | 0 | 100.0% |  | Y | `_ZN7StarMap6updateEi`<br>StarMap::update(int) |
| 2243 | `0x0ca7cc` | `0x0ca814` | +72 | 18 | 18 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIbEvR5ArrayIT_E`<br>void ArrayRelease<bool>(Array<bool>&) |
| 2244 | `0x0ca7e0` | `0x0ca828` | +72 | 812 | 812 | 0 | 100.0% |  | Y | `_ZN7StarMap12OnTouchBeginEii`<br>StarMap::OnTouchBegin(int, int) |
| 2245 | `0x0cab0c` | `0x0cab54` | +72 | 900 | 900 | 0 | 100.0% |  | Y | `_ZN7StarMap11OnTouchMoveEii`<br>StarMap::OnTouchMove(int, int) |
| 2246 | `0x0cae90` | `0x0caed8` | +72 | 2972 | 2972 | 0 | 100.0% |  | Y | `_ZN7StarMap10OnTouchEndEii`<br>StarMap::OnTouchEnd(int, int) |
| 2247 | `0x0cba2c` | `0x0cba74` | +72 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN13RecordHandlerC2Ev`<br>RecordHandler::RecordHandler() |
| 2248 | `0x0cba2c` | `0x0cba74` | +72 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN13RecordHandlerC1Ev`<br>RecordHandler::RecordHandler() |
| 2249 | `0x0cbb08` | `0x0cbb50` | +72 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN13RecordHandlerD2Ev`<br>RecordHandler::~RecordHandler() |
| 2250 | `0x0cbb08` | `0x0cbb50` | +72 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN13RecordHandlerD1Ev`<br>RecordHandler::~RecordHandler() |
| 2251 | `0x0cbb30` | `0x0cbb78` | +72 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN13RecordHandler15notEnoughMemoryEv`<br>RecordHandler::notEnoughMemory() |
| 2252 | `0x0cbb48` | `0x0cbb90` | +72 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN13RecordHandler21readAllPreviewRecordsEv`<br>RecordHandler::readAllPreviewRecords() |
| 2253 | `0x0cbba0` | `0x0cbbe8` | +72 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP10GameRecordEC2Ev`<br>Array<GameRecord*>::Array() |
| 2254 | `0x0cbbbc` | `0x0cbc04` | +72 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP10GameRecordEvjR5ArrayIT_E`<br>void ArraySetLength<GameRecord*>(unsigned int, Array<GameRecord*>&) |
| 2255 | `0x0cbbf0` | `0x0cbc38` | +72 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN13RecordHandler22recordStoreReadPreviewEi`<br>RecordHandler::recordStoreReadPreview(int) |
| 2256 | `0x0cbcdc` | `0x0cbd24` | +72 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN13RecordHandler14readAllRecordsEv`<br>RecordHandler::readAllRecords() |
| 2257 | `0x0cbd34` | `0x0cbd7c` | +72 | 5416 | 5416 | 0 | 100.0% |  | Y | `_ZN13RecordHandler15recordStoreReadEi`<br>RecordHandler::recordStoreRead(int) |
| 2258 | `0x0cd25c` | `0x0cd2a4` | +72 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN13RecordHandler10readRecordEi`<br>RecordHandler::readRecord(int) |
| 2259 | `0x0cd260` | `0x0cd2a8` | +72 | 496 | 496 | 0 | 100.0% |  | Y | `_ZN13RecordHandler19loadResolutionValueEf`<br>RecordHandler::loadResolutionValue(float) |
| 2260 | `0x0cd450` | `0x0cd498` | +72 | 864 | 864 | 0 | 100.0% |  | Y | `_ZN13RecordHandler11loadOptionsEv`<br>RecordHandler::loadOptions() |
| 2261 | `0x0cd7b0` | `0x0cd7f8` | +72 | 280 | 280 | 0 | 100.0% |  | Y | `_ZN13RecordHandler9checkHashEj`<br>RecordHandler::checkHash(unsigned int) |
| 2262 | `0x0cd8c8` | `0x0cd910` | +72 | 564 | 564 | 0 | 100.0% |  | Y | `_ZN13RecordHandler11saveOptionsEv`<br>RecordHandler::saveOptions() |
| 2263 | `0x0cdafc` | `0x0cdb44` | +72 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN13RecordHandler16addHashToOptionsEv`<br>RecordHandler::addHashToOptions() |
| 2264 | `0x0cdbfc` | `0x0cdc44` | +72 | 180 | 180 | 0 | 100.0% |  | Y | `_ZN13RecordHandler21readRecordAsByteArrayEPPaib`<br>RecordHandler::readRecordAsByteArray(signed char**, int, bool) |
| 2265 | `0x0cdcb0` | `0x0cdcf8` | +72 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN13RecordHandler22writeByteArrayAsRecordEPaiib`<br>RecordHandler::writeByteArrayAsRecord(signed char*, int, int, bool) |
| 2266 | `0x0cdd4c` | `0x0cdd94` | +72 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN13RecordHandler26readOptionsFileAsByteArrayEPPa`<br>RecordHandler::readOptionsFileAsByteArray(signed char**) |
| 2267 | `0x0cddd8` | `0x0cde40` | +104 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN13RecordHandler27writeByteArrayAsOptionsFileEPai`<br>RecordHandler::writeByteArrayAsOptionsFile(signed char*, int) |
| 2268 | `0x0cde50` | `0x0cdeb8` | +104 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIhEC2Ev`<br>Array<unsigned char>::Array() |
| 2269 | `0x0cde6c` | `0x0cded4` | +104 | 50 | 50 | 0 | 100.0% |  | Y | `_Z8ArrayAddIhEvPKT_jR5ArrayIS0_E`<br>void ArrayAdd<unsigned char>(unsigned char const*, unsigned int, Array<unsigned char>&) |
| 2270 | `0x0cdea0` | `0x0cdf08` | +104 | 276 | 276 | 0 | 100.0% |  | Y | `_ZN13RecordHandler7addHashEi`<br>RecordHandler::addHash(int) |
| 2271 | `0x0cdfb4` | `0x0ce01c` | +104 | 668 | 668 | 0 | 100.0% |  | Y | `_ZN13RecordHandler11readMissionEj`<br>RecordHandler::readMission(unsigned int) |
| 2272 | `0x0ce250` | `0x0ce2b8` | +104 | 1012 | 1012 | 0 | 100.0% |  | Y | `_ZN13RecordHandler9readAgentEj`<br>RecordHandler::readAgent(unsigned int) |
| 2273 | `0x0ce644` | `0x0ce6ac` | +104 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP14PendingProductEvjR5ArrayIT_E`<br>void ArraySetLength<PendingProduct*>(unsigned int, Array<PendingProduct*>&) |
| 2274 | `0x0ce678` | `0x0ce6e0` | +104 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP6WantedEC2Ev`<br>Array<Wanted*>::Array() |
| 2275 | `0x0ce694` | `0x0ce6fc` | +104 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP6WantedEvjR5ArrayIT_E`<br>void ArraySetLength<Wanted*>(unsigned int, Array<Wanted*>&) |
| 2276 | `0x0ce6c8` | `0x0ce730` | +104 | 484 | 484 | 0 | 100.0% |  | Y | `_ZN13RecordHandler10readWantedEj`<br>RecordHandler::readWanted(unsigned int) |
| 2277 | `0x0ce8ac` | `0x0ce914` | +104 | 3988 | 3988 | 0 | 100.0% |  | Y | `_ZN13RecordHandler16recordStoreWriteEi`<br>RecordHandler::recordStoreWrite(int) |
| 2278 | `0x0cf840` | `0x0cf8a8` | +104 | 432 | 432 | 0 | 100.0% |  | Y | `_ZN13RecordHandler12writeMissionEP7Missionj`<br>RecordHandler::writeMission(Mission*, unsigned int) |
| 2279 | `0x0cf9f0` | `0x0cfa58` | +104 | 592 | 592 | 0 | 100.0% |  | Y | `_ZN13RecordHandler10writeAgentEP5Agentj`<br>RecordHandler::writeAgent(Agent*, unsigned int) |
| 2280 | `0x0cfc40` | `0x0cfca8` | +104 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN13RecordHandler11writeWantedEP6Wantedj`<br>RecordHandler::writeWanted(Wanted*, unsigned int) |
| 2281 | `0x0cfd78` | `0x0cfde0` | +104 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN13RecordHandler23recordStoreWritePreviewEi`<br>RecordHandler::recordStoreWritePreview(int) |
| 2282 | `0x0cfea4` | `0x0cff0c` | +104 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN13RecordHandler23recordStoreWritePreviewEP10GameRecordi`<br>RecordHandler::recordStoreWritePreview(GameRecord*, int) |
| 2283 | `0x0cffa4` | `0x0d000c` | +104 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN13RecordHandler21recoverSDVersionSavesEv`<br>RecordHandler::recoverSDVersionSaves() |
| 2284 | `0x0cffa8` | `0x0d0010` | +104 | 536 | 536 | 0 | 100.0% |  | Y | `_ZN13RecordHandler21convertSDVersionSavesEv`<br>RecordHandler::convertSDVersionSaves() |
| 2285 | `0x0d01c0` | `0x0d0228` | +104 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPaEC2Ev`<br>Array<signed char*>::Array() |
| 2286 | `0x0d01dc` | `0x0d0244` | +104 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIPaEvjR5ArrayIT_E`<br>void ArraySetLength<signed char*>(unsigned int, Array<signed char*>&) |
| 2287 | `0x0d0210` | `0x0d0278` | +104 | 72 | 72 | 0 | 100.0% |  | Y | `_Z18ArrayReleaseArraysIPaEvR5ArrayIT_E`<br>void ArrayReleaseArrays<signed char*>(Array<signed char*>&) |
| 2288 | `0x0d0258` | `0x0d02c0` | +104 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN13RecordHandler36changeSaveDirectoryToBackupDirectoryEv`<br>RecordHandler::changeSaveDirectoryToBackupDirectory() |
| 2289 | `0x0d0390` | `0x0d03f8` | +104 | 60 | 60 | 0 | 100.0% |  | Y | `Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetOrigamiSuperClub`<br>Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetOrigamiSuperClub |
| 2290 | `0x0d03cc` | `0x0d0434` | +104 | 24 | 24 | 0 | 100.0% |  | Y | `Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ReleaseOrigamiSuperClub`<br>Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ReleaseOrigamiSuperClub |
| 2291 | `0x0d03e4` | `0x0d044c` | +104 | 736 | 736 | 0 | 100.0% |  | Y | `_Z13loadingScreenPN11AbyssEngine11PaintCanvasEiPv`<br>loadingScreen(AbyssEngine::PaintCanvas*, int, void*) |
| 2292 | `0x0d06c4` | `0x0d072c` | +104 | 48 | 48 | 0 | 100.0% |  | Y | `_Z20OnDestroyApplicationPN11AbyssEngine6EngineE`<br>OnDestroyApplication(AbyssEngine::Engine*) |
| 2293 | `0x0d06f4` | `0x0d075c` | +104 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN8GameDataD2Ev`<br>GameData::~GameData() |
| 2294 | `0x0d0738` | `0x0d07a0` | +104 | 1332 | 1332 | 0 | 100.0% |  | Y | `_Z19OnCreateApplicationPN11AbyssEngine6EngineE`<br>OnCreateApplication(AbyssEngine::Engine*) |
| 2295 | `0x0d0c6c` | `0x0d0cd4` | +104 | 168 | 168 | 0 | 100.0% |  | Y | `_ZN8GameDataC2Ev`<br>GameData::GameData() |
| 2296 | `0x0d0d18` | `0x0d0d80` | +104 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN6SpriteC1Ejii`<br>Sprite::Sprite(unsigned int, int, int) |
| 2297 | `0x0d0d18` | `0x0d0d80` | +104 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN6SpriteC2Ejii`<br>Sprite::Sprite(unsigned int, int, int) |
| 2298 | `0x0d0d88` | `0x0d0df0` | +104 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN6Sprite8setFrameEi`<br>Sprite::setFrame(int) |
| 2299 | `0x0d0dd0` | `0x0d0e38` | +104 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN6SpriteC2EPjiii`<br>Sprite::Sprite(unsigned int*, int, int, int) |
| 2300 | `0x0d0dd0` | `0x0d0e38` | +104 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN6SpriteC1EPjiii`<br>Sprite::Sprite(unsigned int*, int, int, int) |
| 2301 | `0x0d0e30` | `0x0d0e98` | +104 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN6SpriteD1Ev`<br>Sprite::~Sprite() |
| 2302 | `0x0d0e30` | `0x0d0e98` | +104 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN6SpriteD2Ev`<br>Sprite::~Sprite() |
| 2303 | `0x0d0e46` | `0x0d0eae` | +104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Sprite20defineReferencePixelEii`<br>Sprite::defineReferencePixel(int, int) |
| 2304 | `0x0d0e4c` | `0x0d0eb4` | +104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Sprite19setRefPixelPositionEii`<br>Sprite::setRefPixelPosition(int, int) |
| 2305 | `0x0d0e52` | `0x0d0eba` | +104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Sprite11setPositionEii`<br>Sprite::setPosition(int, int) |
| 2306 | `0x0d0e58` | `0x0d0ec0` | +104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Sprite8getFrameEv`<br>Sprite::getFrame() |
| 2307 | `0x0d0e5c` | `0x0d0ec4` | +104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Sprite16getRawFrameCountEv`<br>Sprite::getRawFrameCount() |
| 2308 | `0x0d0e60` | `0x0d0ec8` | +104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Sprite13getFrameWidthEv`<br>Sprite::getFrameWidth() |
| 2309 | `0x0d0e64` | `0x0d0ecc` | +104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Sprite14getFrameHeightEv`<br>Sprite::getFrameHeight() |
| 2310 | `0x0d0e68` | `0x0d0ed0` | +104 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN6Sprite4drawEff`<br>Sprite::draw(float, float) |
| 2311 | `0x0d0f8c` | `0x0d0ff4` | +104 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN6Sprite10drawRegionEiiii`<br>Sprite::drawRegion(int, int, int, int) |
| 2312 | `0x0d1018` | `0x0d1080` | +104 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Sprite9nextFrameEv`<br>Sprite::nextFrame() |
| 2313 | `0x0d1020` | `0x0d1088` | +104 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Sprite9prevFrameEv`<br>Sprite::prevFrame() |
| 2314 | `0x0d1028` | `0x0d1090` | +104 | 7072 | 6636 | -436 | 46.0% |  |  | `_ZN6LayoutC1Ev`<br>Layout::Layout() |
| 2315 | `0x0d1028` | `0x0d1090` | +104 | 7072 | 6636 | -436 | 46.0% |  |  | `_ZN6LayoutC2Ev`<br>Layout::Layout() |
| 2316 | `0x0d2bc8` | `0x0d2a7c` | -332 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN6LayoutD2Ev`<br>Layout::~Layout() |
| 2317 | `0x0d2bc8` | `0x0d2a7c` | -332 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN6LayoutD1Ev`<br>Layout::~Layout() |
| 2318 | `0x0d2c20` | `0x0d2ad4` | -332 | 1064 | 1064 | 0 | 100.0% |  | Y | `_ZN6Layout6reloadEv`<br>Layout::reload() |
| 2319 | `0x0d3048` | `0x0d2efc` | -332 | 240 | 240 | 0 | 100.0% |  | Y | `_ZN6Layout21resetWindowDimensionsEv`<br>Layout::resetWindowDimensions() |
| 2320 | `0x0d3138` | `0x0d2fec` | -332 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Layout12setDrawColorEi`<br>Layout::setDrawColor(int) |
| 2321 | `0x0d3140` | `0x0d2ff4` | -332 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN6Layout8drawMaskEv`<br>Layout::drawMask() |
| 2322 | `0x0d3168` | `0x0d301c` | -332 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN6Layout8drawMaskEiiii`<br>Layout::drawMask(int, int, int, int) |
| 2323 | `0x0d31b0` | `0x0d3064` | -332 | 260 | 260 | 0 | 100.0% |  | Y | `_ZN6Layout9tagStringEN11AbyssEngine6StringE`<br>Layout::tagString(AbyssEngine::String) |
| 2324 | `0x0d32b4` | `0x0d3168` | -332 | 548 | 548 | 0 | 100.0% |  | Y | `_ZN6Layout12formatNumberEi`<br>Layout::formatNumber(int) |
| 2325 | `0x0d34d8` | `0x0d338c` | -332 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN6Layout13formatCreditsEi`<br>Layout::formatCredits(int) |
| 2326 | `0x0d3544` | `0x0d33f8` | -332 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN6Layout6drawBGEv`<br>Layout::drawBG() |
| 2327 | `0x0d3574` | `0x0d3428` | -332 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN6Layout13drawBGPatternEjiiii`<br>Layout::drawBGPattern(unsigned int, int, int, int, int) |
| 2328 | `0x0d36f4` | `0x0d35a8` | -332 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN6Layout10drawWindowEN11AbyssEngine6StringEb`<br>Layout::drawWindow(AbyssEngine::String, bool) |
| 2329 | `0x0d3780` | `0x0d3634` | -332 | 252 | 252 | 0 | 100.0% |  | Y | `_ZN6Layout10drawWindowEN11AbyssEngine6StringEiiiib`<br>Layout::drawWindow(AbyssEngine::String, int, int, int, int, bool) |
| 2330 | `0x0d387c` | `0x0d3730` | -332 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN6Layout10drawWindowEN11AbyssEngine6StringE`<br>Layout::drawWindow(AbyssEngine::String) |
| 2331 | `0x0d38d4` | `0x0d3788` | -332 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN6Layout10drawWindowEN11AbyssEngine6StringEiiii`<br>Layout::drawWindow(AbyssEngine::String, int, int, int, int) |
| 2332 | `0x0d393c` | `0x0d37f0` | -332 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN6Layout12drawBGBorderEjjiiii`<br>Layout::drawBGBorder(unsigned int, unsigned int, int, int, int, int) |
| 2333 | `0x0d395c` | `0x0d3810` | -332 | 684 | 684 | 0 | 100.0% |  | Y | `_ZN6Layout12drawBGBorderEjjiiiiii`<br>Layout::drawBGBorder(unsigned int, unsigned int, int, int, int, int, int, int) |
| 2334 | `0x0d3c08` | `0x0d3abc` | -332 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6Layout24getFooterTransitionWidthEv`<br>Layout::getFooterTransitionWidth() |
| 2335 | `0x0d3c3c` | `0x0d3af0` | -332 | 888 | 888 | 0 | 100.0% |  | Y | `_ZN6Layout10drawFooterEbb`<br>Layout::drawFooter(bool, bool) |
| 2336 | `0x0d3fb4` | `0x0d3e68` | -332 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Layout10drawFooterEv`<br>Layout::drawFooter() |
| 2337 | `0x0d3fbc` | `0x0d3e70` | -332 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN6Layout15drawEmptyFooterEb`<br>Layout::drawEmptyFooter(bool) |
| 2338 | `0x0d4090` | `0x0d3f44` | -332 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Layout22drawFooterNoBackButtonEv`<br>Layout::drawFooterNoBackButton() |
| 2339 | `0x0d4098` | `0x0d3f4c` | -332 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Layout17drawFooterStationEv`<br>Layout::drawFooterStation() |
| 2340 | `0x0d40a0` | `0x0d3f54` | -332 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN6Layout10drawHeaderEv`<br>Layout::drawHeader() |
| 2341 | `0x0d4100` | `0x0d3fb4` | -332 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN6Layout10drawHeaderEN11AbyssEngine6StringEb`<br>Layout::drawHeader(AbyssEngine::String, bool) |
| 2342 | `0x0d4200` | `0x0d40b4` | -332 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN6Layout10drawHeaderEN11AbyssEngine6StringE`<br>Layout::drawHeader(AbyssEngine::String) |
| 2343 | `0x0d4258` | `0x0d410c` | -332 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN6Layout13drawScrollBarEiiiii`<br>Layout::drawScrollBar(int, int, int, int, int) |
| 2344 | `0x0d4384` | `0x0d4238` | -332 | 1388 | 1388 | 0 | 100.0% |  | Y | `_ZN6Layout7drawBoxEiiiiiN11AbyssEngine6StringEh`<br>Layout::drawBox(int, int, int, int, int, AbyssEngine::String, unsigned char) |
| 2345 | `0x0d48f0` | `0x0d47a4` | -332 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN6Layout7drawBoxEiiiiiN11AbyssEngine6StringE`<br>Layout::drawBox(int, int, int, int, int, AbyssEngine::String) |
| 2346 | `0x0d495c` | `0x0d4810` | -332 | 184 | 184 | 0 | 100.0% |  | Y | `_ZN6Layout7initTipEv`<br>Layout::initTip() |
| 2347 | `0x0d4a14` | `0x0d48c8` | -332 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN6Layout7drawTipEv`<br>Layout::drawTip() |
| 2348 | `0x0d4b40` | `0x0d49f4` | -332 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN6Layout14initHelpWindowEN11AbyssEngine6StringE`<br>Layout::initHelpWindow(AbyssEngine::String) |
| 2349 | `0x0d4bc0` | `0x0d4a74` | -332 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN6Layout14drawHelpWindowEv`<br>Layout::drawHelpWindow() |
| 2350 | `0x0d4bc8` | `0x0d4a7c` | -332 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Layout11helpPressedEv`<br>Layout::helpPressed() |
| 2351 | `0x0d4bd0` | `0x0d4a84` | -332 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN6Layout19getHelpButtonOffsetEv`<br>Layout::getHelpButtonOffset() |
| 2352 | `0x0d4bf0` | `0x0d4aa4` | -332 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN6Layout13getPulseValueEf`<br>Layout::getPulseValue(float) |
| 2353 | `0x0d4c60` | `0x0d4b14` | -332 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN6Layout19setWindowDimensionsEiiii`<br>Layout::setWindowDimensions(int, int, int, int) |
| 2354 | `0x0d4d40` | `0x0d4bf4` | -332 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN6Layout24showMissionRewardMessageEib`<br>Layout::showMissionRewardMessage(int, bool) |
| 2355 | `0x0d4d74` | `0x0d4c28` | -332 | 672 | 672 | 0 | 100.0% |  | Y | `_ZN6Layout24drawMissionRewardMessageEb`<br>Layout::drawMissionRewardMessage(bool) |
| 2356 | `0x0d5014` | `0x0d4ec8` | -332 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN6Layout9startFadeEbii`<br>Layout::startFade(bool, int, int) |
| 2357 | `0x0d5032` | `0x0d4ee6` | -332 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Layout16enableFillScreenEb`<br>Layout::enableFillScreen(bool) |
| 2358 | `0x0d5038` | `0x0d4eec` | -332 | 276 | 276 | 0 | 100.0% |  | Y | `_ZN6Layout8drawFadeEv`<br>Layout::drawFade() |
| 2359 | `0x0d514c` | `0x0d5000` | -332 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Layout8isFadingEv`<br>Layout::isFading() |
| 2360 | `0x0d5152` | `0x0d5006` | -332 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN6Layout6updateEi`<br>Layout::update(int) |
| 2361 | `0x0d51a8` | `0x0d505c` | -332 | 94 | 94 | 0 | 100.0% |  | Y | `_ZN6Layout12OnTouchBeginEii`<br>Layout::OnTouchBegin(int, int) |
| 2362 | `0x0d5206` | `0x0d50ba` | -332 | 94 | 94 | 0 | 100.0% |  | Y | `_ZN6Layout11OnTouchMoveEii`<br>Layout::OnTouchMove(int, int) |
| 2363 | `0x0d5264` | `0x0d5118` | -332 | 106 | 106 | 0 | 100.0% |  | Y | `_ZN6Layout10OnTouchEndEii`<br>Layout::OnTouchEnd(int, int) |
| 2364 | `0x0d52d0` | `0x0d5188` | -328 | 1008 | 1008 | 0 | 98.6% |  |  | `_ZN10ModStationC1Ev`<br>ModStation::ModStation() |
| 2365 | `0x0d52d0` | `0x0d5188` | -328 | 1008 | 1008 | 0 | 98.6% |  |  | `_ZN10ModStationC2Ev`<br>ModStation::ModStation() |
| 2366 | `0x0d56c0` | `0x0d5578` | -328 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN10ModStationD1Ev`<br>ModStation::~ModStation() |
| 2367 | `0x0d56c0` | `0x0d5578` | -328 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN10ModStationD2Ev`<br>ModStation::~ModStation() |
| 2368 | `0x0d56f8` | `0x0d55b0` | -328 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN10ModStationD0Ev`<br>ModStation::~ModStation() |
| 2369 | `0x0d5708` | `0x0d55c0` | -328 | 6396 | 6396 | 0 | 99.5% |  |  | `_ZN10ModStation12OnInitializeEv`<br>ModStation::OnInitialize() |
| 2370 | `0x0d7004` | `0x0d6ebc` | -328 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11TouchButtonEC2Ev`<br>Array<TouchButton*>::Array() |
| 2371 | `0x0d7020` | `0x0d6ed8` | -328 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP11TouchButtonEvjR5ArrayIT_E`<br>void ArraySetLength<TouchButton*>(unsigned int, Array<TouchButton*>&) |
| 2372 | `0x0d7054` | `0x0d6f0c` | -328 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN10ModStation8autosaveEv`<br>ModStation::autosave() |
| 2373 | `0x0d70bc` | `0x0d6f74` | -328 | 324 | 324 | 0 | 100.0% |  | Y | `_ZN10ModStation10resetLightEv`<br>ModStation::resetLight() |
| 2374 | `0x0d7200` | `0x0d70b8` | -328 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN10ModStation12enterStationEv`<br>ModStation::enterStation() |
| 2375 | `0x0d72ac` | `0x0d7164` | -328 | 468 | 468 | 0 | 100.0% |  | Y | `_ZN10ModStation12OnTouchBeginEiiPv`<br>ModStation::OnTouchBegin(int, int, void*) |
| 2376 | `0x0d7480` | `0x0d7338` | -328 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN10ModStation11OnTouchMoveEiiPv`<br>ModStation::OnTouchMove(int, int, void*) |
| 2377 | `0x0d7600` | `0x0d74ec` | -276 | 5404 | 5404 | 0 | 100.0% |  | Y | `_ZN10ModStation10OnTouchEndEiiPv`<br>ModStation::OnTouchEnd(int, int, void*) |
| 2378 | `0x0d8b1c` | `0x0d8a08` | -276 | 468 | 468 | 0 | 100.0% |  | Y | `_ZN10ModStation11checkMedalsEv`<br>ModStation::checkMedals() |
| 2379 | `0x0d8cf0` | `0x0d8bdc` | -276 | 360 | 360 | 0 | 100.0% |  | Y | `_ZN10ModStation21resetIdleCamForHangarEv`<br>ModStation::resetIdleCamForHangar() |
| 2380 | `0x0d8e58` | `0x0d8d44` | -276 | 504 | 504 | 0 | 100.0% |  | Y | `_ZN10ModStation12leaveStationEv`<br>ModStation::leaveStation() |
| 2381 | `0x0d9050` | `0x0d8f3c` | -276 | 492 | 492 | 0 | 100.0% |  | Y | `_ZN10ModStation9OnReleaseEv`<br>ModStation::OnRelease() |
| 2382 | `0x0d923c` | `0x0d9128` | -276 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP11TouchButtonEvR5ArrayIT_E`<br>void ArrayReleaseClasses<TouchButton*>(Array<TouchButton*>&) |
| 2383 | `0x0d928c` | `0x0d9178` | -276 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11TouchButtonED2Ev`<br>Array<TouchButton*>::~Array() |
| 2384 | `0x0d92a4` | `0x0d9190` | -276 | 2516 | 2516 | 0 | 99.7% |  |  | `_ZN10ModStation10OnKeyPressExx`<br>ModStation::OnKeyPress(long long, long long) |
| 2385 | `0x0d9c78` | `0x0d9b64` | -276 | 3652 | 3652 | 0 | 100.0% |  | Y | `_ZN10ModStation8OnUpdateEv`<br>ModStation::OnUpdate() |
| 2386 | `0x0daabc` | `0x0da9a8` | -276 | 664 | 664 | 0 | 100.0% |  | Y | `_ZN10ModStation20checkPendingProductsEv`<br>ModStation::checkPendingProducts() |
| 2387 | `0x0dad54` | `0x0dac40` | -276 | 2748 | 2748 | 0 | 100.0% |  | Y | `_ZN10ModStation10checkHintsEv`<br>ModStation::checkHints() |
| 2388 | `0x0db810` | `0x0db6fc` | -276 | 1816 | 1816 | 0 | 100.0% |  | Y | `_ZN10ModStation10OnRender2DEv`<br>ModStation::OnRender2D() |
| 2389 | `0x0dbf28` | `0x0dbe14` | -276 | 196 | 196 | 0 | 100.0% |  | Y | `_ZN10ModStation10OnRender3DEv`<br>ModStation::OnRender3D() |
| 2390 | `0x0dbfec` | `0x0dbed8` | -276 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN10ModStation13setGameLoadedEv`<br>ModStation::setGameLoaded() |
| 2391 | `0x0dbff8` | `0x0dbee4` | -276 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN10ModStation14addAchievementEii`<br>ModStation::addAchievement(int, int) |
| 2392 | `0x0dc078` | `0x0dbf64` | -276 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPiEC2Ev`<br>Array<int*>::Array() |
| 2393 | `0x0dc094` | `0x0dbf80` | -276 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIPiEvjR5ArrayIT_E`<br>void ArraySetLength<int*>(unsigned int, Array<int*>&) |
| 2394 | `0x0dc0c8` | `0x0dbfb4` | -276 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN10ModStation13showMapWindowEv`<br>ModStation::showMapWindow() |
| 2395 | `0x0dc0d8` | `0x0dbfc4` | -276 | 228 | 228 | 0 | 100.0% |  | Y | `_ZN10ModStation14showCBSMessageEv`<br>ModStation::showCBSMessage() |
| 2396 | `0x0dc1bc` | `0x0dc0a8` | -276 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN10ModStation11showDlcMenuEv`<br>ModStation::showDlcMenu() |
| 2397 | `0x0dc29c` | `0x0dc188` | -276 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN10ModStation8OnResumeEv`<br>ModStation::OnResume() |
| 2398 | `0x0dc2d0` | `0x0dc1bc` | -276 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN10ModStation9OnSuspendEv`<br>ModStation::OnSuspend() |
| 2399 | `0x0dc2ec` | `0x0dc1d8` | -276 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN10ModStation12OnKeyReleaseExx`<br>ModStation::OnKeyRelease(long long, long long) |
| 2400 | `0x0dc2ee` | `0x0dc1da` | -276 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN10ModStation12OnTouchBeginEii`<br>ModStation::OnTouchBegin(int, int) |
| 2401 | `0x0dc2f0` | `0x0dc1dc` | -276 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN10ModStation11OnTouchMoveEii`<br>ModStation::OnTouchMove(int, int) |
| 2402 | `0x0dc2f2` | `0x0dc1de` | -276 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN10ModStation10OnTouchEndEii`<br>ModStation::OnTouchEnd(int, int) |
| 2403 | `0x0dc2f4` | `0x0dc1e0` | -276 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10ModStation17ShowLoadingScreenEv`<br>ModStation::ShowLoadingScreen() |
| 2404 | `0x0dc2f8` | `0x0dc1e4` | -276 | 1324 | 1324 | 0 | 100.0% |  | Y | `_ZN13PlayerFighterC2EiiP6PlayerP10AEGeometryfffb`<br>PlayerFighter::PlayerFighter(int, int, Player*, AEGeometry*, float, float, float, bool) |
| 2405 | `0x0dc2f8` | `0x0dc1e4` | -276 | 1324 | 1324 | 0 | 100.0% |  | Y | `_ZN13PlayerFighterC1EiiP6PlayerP10AEGeometryfffb`<br>PlayerFighter::PlayerFighter(int, int, Player*, AEGeometry*, float, float, float, bool) |
| 2406 | `0x0dc824` | `0x0dc710` | -276 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN13PlayerFighterD1Ev`<br>PlayerFighter::~PlayerFighter() |
| 2407 | `0x0dc824` | `0x0dc710` | -276 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN13PlayerFighterD2Ev`<br>PlayerFighter::~PlayerFighter() |
| 2408 | `0x0dc8bc` | `0x0dc7a8` | -276 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP14BoundingVolumeEvR5ArrayIT_E`<br>void ArrayReleaseClasses<BoundingVolume*>(Array<BoundingVolume*>&) |
| 2409 | `0x0dc90c` | `0x0dc7f8` | -276 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP14BoundingVolumeED2Ev`<br>Array<BoundingVolume*>::~Array() |
| 2410 | `0x0dc922` | `0x0dc80e` | -276 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN13PlayerFighterD0Ev`<br>PlayerFighter::~PlayerFighter() |
| 2411 | `0x0dc934` | `0x0dc820` | -276 | 252 | 252 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter5resetEv`<br>PlayerFighter::reset() |
| 2412 | `0x0dca30` | `0x0dc91c` | -276 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter8setLevelEP5Level`<br>PlayerFighter::setLevel(Level*) |
| 2413 | `0x0dcb18` | `0x0dca04` | -276 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter15setMissionCrateEb`<br>PlayerFighter::setMissionCrate(bool) |
| 2414 | `0x0dcb78` | `0x0dca64` | -276 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter23hasMissionCrateCapturedEv`<br>PlayerFighter::hasMissionCrateCaptured() |
| 2415 | `0x0dcb7e` | `0x0dca6a` | -276 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter19hasMissionCrateLostEv`<br>PlayerFighter::hasMissionCrateLost() |
| 2416 | `0x0dcb84` | `0x0dca70` | -276 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter12setShipGroupEP10AEGeometryib`<br>PlayerFighter::setShipGroup(AEGeometry*, int, bool) |
| 2417 | `0x0dcb88` | `0x0dca74` | -276 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter16hasCrateCapturedEv`<br>PlayerFighter::hasCrateCaptured() |
| 2418 | `0x0dcb94` | `0x0dca80` | -276 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter12hasCrateLostEv`<br>PlayerFighter::hasCrateLost() |
| 2419 | `0x0dcb9a` | `0x0dca86` | -276 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>PlayerFighter::setPosition(AbyssEngine::AEMath::Vector const&) |
| 2420 | `0x0dcbb8` | `0x0dcaa4` | -276 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter11setPositionEfff`<br>PlayerFighter::setPosition(float, float, float) |
| 2421 | `0x0dcc2c` | `0x0dcb18` | -276 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter17setWingmanCommandEiP8KIPlayer`<br>PlayerFighter::setWingmanCommand(int, KIPlayer*) |
| 2422 | `0x0dcccc` | `0x0dcbb8` | -276 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter8setSpeedEf`<br>PlayerFighter::setSpeed(float) |
| 2423 | `0x0dccdc` | `0x0dcbc8` | -276 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter9setRotateEi`<br>PlayerFighter::setRotate(int) |
| 2424 | `0x0dccf4` | `0x0dcbe0` | -276 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter13setShootErrorEi`<br>PlayerFighter::setShootError(int) |
| 2425 | `0x0dcd02` | `0x0dcbee` | -276 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter12setBoostProbEi`<br>PlayerFighter::setBoostProb(int) |
| 2426 | `0x0dcd08` | `0x0dcbf4` | -276 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter11removeTrailEv`<br>PlayerFighter::removeTrail() |
| 2427 | `0x0dcd24` | `0x0dcc10` | -276 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter17setExhaustVisibleEb`<br>PlayerFighter::setExhaustVisible(bool) |
| 2428 | `0x0dcd58` | `0x0dcc44` | -276 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter5cloakEib`<br>PlayerFighter::cloak(int, bool) |
| 2429 | `0x0dcd9c` | `0x0dcc88` | -276 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter19setCloakingPossibleEb`<br>PlayerFighter::setCloakingPossible(bool) |
| 2430 | `0x0dcdbc` | `0x0dcca8` | -276 | 528 | 528 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter14handleCloakingEv`<br>PlayerFighter::handleCloaking() |
| 2431 | `0x0dcfcc` | `0x0dceb8` | -276 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter13setAIDisabledEb`<br>PlayerFighter::setAIDisabled(bool) |
| 2432 | `0x0dcfd8` | `0x0dcec0` | -280 | 10376 | 10376 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter6updateEi`<br>PlayerFighter::update(int) |
| 2433 | `0x0df860` | `0x0df748` | -280 | 496 | 496 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter4rollEi`<br>PlayerFighter::roll(int) |
| 2434 | `0x0dfa50` | `0x0df938` | -280 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter6renderEv`<br>PlayerFighter::render() |
| 2435 | `0x0dfb84` | `0x0dfa6c` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter5setBVEP5ArrayIP14BoundingVolumeE`<br>PlayerFighter::setBV(Array<BoundingVolume*>*) |
| 2436 | `0x0dfb8a` | `0x0dfa72` | -280 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter5setBVEP14BoundingVolume`<br>PlayerFighter::setBV(BoundingVolume*) |
| 2437 | `0x0dfbc4` | `0x0dfaac` | -280 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP14BoundingVolumeEvT_R5ArrayIS2_E`<br>void ArrayAdd<BoundingVolume*>(BoundingVolume*, Array<BoundingVolume*>&) |
| 2438 | `0x0dfbe8` | `0x0dfad0` | -280 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter7collideEfff`<br>PlayerFighter::collide(float, float, float) |
| 2439 | `0x0dfc38` | `0x0dfb20` | -280 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN13PlayerFighter12outerCollideEfff`<br>PlayerFighter::outerCollide(float, float, float) |
| 2440 | `0x0dfc88` | `0x0dfb70` | -280 | 320 | 320 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter8initPushERKN11AbyssEngine6AEMath6VectorEi`<br>PlayerFighter::initPush(AbyssEngine::AEMath::Vector const&, int) |
| 2441 | `0x0dfdc8` | `0x0dfcb0` | -280 | 356 | 356 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter4pushEi`<br>PlayerFighter::push(int) |
| 2442 | `0x0dff2c` | `0x0dfe14` | -280 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter5awakeEv`<br>PlayerFighter::awake() |
| 2443 | `0x0dff5c` | `0x0dfe44` | -280 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN13PlayerFighter6reviveEv`<br>PlayerFighter::revive() |
| 2444 | `0x0e0090` | `0x0dff78` | -280 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN4ItemC1EP5ArrayIiES2_S2_`<br>Item::Item(Array<int>*, Array<int>*, Array<int>*) |
| 2445 | `0x0e0090` | `0x0dff78` | -280 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN4ItemC2EP5ArrayIiES2_S2_`<br>Item::Item(Array<int>*, Array<int>*, Array<int>*) |
| 2446 | `0x0e00a6` | `0x0dff8e` | -280 | 82 | 82 | 0 | 100.0% | Y | Y | `_ZN4Item4initEv`<br>Item::init() |
| 2447 | `0x0e00f8` | `0x0dffe0` | -280 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN4ItemD2Ev`<br>Item::~Item() |
| 2448 | `0x0e00f8` | `0x0dffe0` | -280 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN4ItemD1Ev`<br>Item::~Item() |
| 2449 | `0x0e00fa` | `0x0dffe2` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN4Item13setUnsaleableEb`<br>Item::setUnsaleable(bool) |
| 2450 | `0x0e0100` | `0x0dffe8` | -280 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN4Item27canBeInstalledMultipleTimesEv`<br>Item::canBeInstalledMultipleTimes() |
| 2451 | `0x0e0110` | `0x0dfff8` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item8getIndexEv`<br>Item::getIndex() |
| 2452 | `0x0e0114` | `0x0dfffc` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item7getTypeEv`<br>Item::getType() |
| 2453 | `0x0e0118` | `0x0e0000` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item11getTecLevelEv`<br>Item::getTecLevel() |
| 2454 | `0x0e011c` | `0x0e0004` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item7getSortEv`<br>Item::getSort() |
| 2455 | `0x0e0120` | `0x0e0008` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item14getSinglePriceEv`<br>Item::getSinglePrice() |
| 2456 | `0x0e0124` | `0x0e000c` | -280 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN4Item13getTotalPriceEv`<br>Item::getTotalPrice() |
| 2457 | `0x0e012c` | `0x0e0014` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item11getMaxPriceEv`<br>Item::getMaxPrice() |
| 2458 | `0x0e0130` | `0x0e0018` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item11getMinPriceEv`<br>Item::getMinPrice() |
| 2459 | `0x0e0134` | `0x0e001c` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item17getMaxPriceSystemEv`<br>Item::getMaxPriceSystem() |
| 2460 | `0x0e0138` | `0x0e0020` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item17getMinPriceSystemEv`<br>Item::getMinPriceSystem() |
| 2461 | `0x0e013c` | `0x0e0024` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item8setPriceEi`<br>Item::setPrice(int) |
| 2462 | `0x0e0140` | `0x0e0028` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item11setMinPriceEi`<br>Item::setMinPrice(int) |
| 2463 | `0x0e0144` | `0x0e002c` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item11setMaxPriceEi`<br>Item::setMaxPrice(int) |
| 2464 | `0x0e0148` | `0x0e0030` | -280 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN4Item12getPriceRateEv`<br>Item::getPriceRate() |
| 2465 | `0x0e016c` | `0x0e0054` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item9setAmountEi`<br>Item::setAmount(int) |
| 2466 | `0x0e0170` | `0x0e0058` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item12getOccurenceEv`<br>Item::getOccurence() |
| 2467 | `0x0e0174` | `0x0e005c` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item9getAmountEv`<br>Item::getAmount() |
| 2468 | `0x0e0178` | `0x0e0060` | -280 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN4Item12changeAmountEi`<br>Item::changeAmount(int) |
| 2469 | `0x0e0180` | `0x0e0068` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item21getMissingIngredientsEv`<br>Item::getMissingIngredients() |
| 2470 | `0x0e0184` | `0x0e006c` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item21setMissingIngredientsEi`<br>Item::setMissingIngredients(int) |
| 2471 | `0x0e0188` | `0x0e0070` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item16setStationAmountEi`<br>Item::setStationAmount(int) |
| 2472 | `0x0e018c` | `0x0e0074` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item16getStationAmountEv`<br>Item::getStationAmount() |
| 2473 | `0x0e0190` | `0x0e0078` | -280 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN4Item19changeStationAmountEi`<br>Item::changeStationAmount(int) |
| 2474 | `0x0e0198` | `0x0e0080` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item18setBlueprintAmountEi`<br>Item::setBlueprintAmount(int) |
| 2475 | `0x0e019c` | `0x0e0084` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item18getBlueprintAmountEv`<br>Item::getBlueprintAmount() |
| 2476 | `0x0e01a0` | `0x0e0088` | -280 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN4Item21changeBlueprintAmountEi`<br>Item::changeBlueprintAmount(int) |
| 2477 | `0x0e01a8` | `0x0e0090` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item14getIngredientsEv`<br>Item::getIngredients() |
| 2478 | `0x0e01ac` | `0x0e0094` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item13getQuantitiesEv`<br>Item::getQuantities() |
| 2479 | `0x0e01b0` | `0x0e0098` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Item13getAttributesEv`<br>Item::getAttributes() |
| 2480 | `0x0e01b4` | `0x0e009c` | -280 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN4Item12getAttributeEi`<br>Item::getAttribute(int) |
| 2481 | `0x0e01e8` | `0x0e00d0` | -280 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN4Item11transactionEbib`<br>Item::transaction(bool, int, bool) |
| 2482 | `0x0e023c` | `0x0e0124` | -280 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN4Item20transactionBlueprintEbi`<br>Item::transactionBlueprint(bool, int) |
| 2483 | `0x0e026e` | `0x0e0156` | -280 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN4Item8isInListEiiP5ArrayIPS_E`<br>Item::isInList(int, int, Array<Item*>*) |
| 2484 | `0x0e02a0` | `0x0e0188` | -280 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN4Item8isInListEiP5ArrayIPS_E`<br>Item::isInList(int, Array<Item*>*) |
| 2485 | `0x0e02a8` | `0x0e0190` | -280 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN4Item8isInListEPS_P5ArrayIS0_E`<br>Item::isInList(Item*, Array<Item*>*) |
| 2486 | `0x0e02ae` | `0x0e0196` | -280 | 138 | 138 | 0 | 100.0% |  | Y | `_ZN4Item9fabricateEPS_P5ArrayIS0_Ei`<br>Item::fabricate(Item*, Array<Item*>*, int) |
| 2487 | `0x0e0338` | `0x0e0220` | -280 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN4Item8makeItemEi`<br>Item::makeItem(int) |
| 2488 | `0x0e034a` | `0x0e0232` | -280 | 338 | 338 | 0 | 100.0% |  | Y | `_ZN4Item12combineItemsEP5ArrayIPS_ES3_`<br>Item::combineItems(Array<Item*>*, Array<Item*>*) |
| 2489 | `0x0e049c` | `0x0e0384` | -280 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN4Item12extractItemsEP5ArrayIPS_Eb`<br>Item::extractItems(Array<Item*>*, bool) |
| 2490 | `0x0e0504` | `0x0e03ec` | -280 | 130 | 130 | 0 | 100.0% |  | Y | `_ZN4Item17combineDuplicatesEP5ArrayIPS_E`<br>Item::combineDuplicates(Array<Item*>*) |
| 2491 | `0x0e0586` | `0x0e046e` | -280 | 68 | 68 | 0 | 100.0% |  | Y | `_Z11ArrayRemoveIP4ItemEvT_R5ArrayIS2_E`<br>void ArrayRemove<Item*>(Item*, Array<Item*>&) |
| 2492 | `0x0e05ca` | `0x0e04b2` | -280 | 476 | 476 | 0 | 100.0% |  | Y | `_ZN4Item8mixItemsEP5ArrayIPS_ES3_`<br>Item::mixItems(Array<Item*>*, Array<Item*>*) |
| 2493 | `0x0e07a6` | `0x0e068e` | -280 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN4Item6equalsEPS_`<br>Item::equals(Item*) |
| 2494 | `0x0e07ba` | `0x0e06a2` | -280 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN4Item8isWeaponEv`<br>Item::isWeapon() |
| 2495 | `0x0e07c6` | `0x0e06ae` | -280 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN4Item8makeItemEii`<br>Item::makeItem(int, int) |
| 2496 | `0x0e07d8` | `0x0e06c0` | -280 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN4Item5cloneEv`<br>Item::clone() |
| 2497 | `0x0e0810` | `0x0e06f8` | -280 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN4Item8makeItemEv`<br>Item::makeItem() |
| 2498 | `0x0e0824` | `0x0e070c` | -280 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN4Item12checkCreditsEv`<br>Item::checkCredits() |
| 2499 | `0x0e0848` | `0x0e0730` | -280 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN4Item11adjustPriceEP7Station`<br>Item::adjustPrice(Station*) |
| 2500 | `0x0e088c` | `0x0e0774` | -280 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN4Item15checkCargoSpaceEv`<br>Item::checkCargoSpace() |
| 2501 | `0x0e08c4` | `0x0e07ac` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN4Item12isUnsaleableEv`<br>Item::isUnsaleable() |
| 2502 | `0x0e08cc` | `0x0e07b4` | -280 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN12PlayerStaticC2EiP10AEGeometryfff`<br>PlayerStatic::PlayerStatic(int, AEGeometry*, float, float, float) |
| 2503 | `0x0e08cc` | `0x0e07b4` | -280 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN12PlayerStaticC1EiP10AEGeometryfff`<br>PlayerStatic::PlayerStatic(int, AEGeometry*, float, float, float) |
| 2504 | `0x0e096c` | `0x0e0854` | -280 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN12PlayerStaticD1Ev`<br>PlayerStatic::~PlayerStatic() |
| 2505 | `0x0e096c` | `0x0e0854` | -280 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN12PlayerStaticD2Ev`<br>PlayerStatic::~PlayerStatic() |
| 2506 | `0x0e0970` | `0x0e0858` | -280 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN12PlayerStaticD0Ev`<br>PlayerStatic::~PlayerStatic() |
| 2507 | `0x0e0980` | `0x0e0868` | -280 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN12PlayerStatic6updateEi`<br>PlayerStatic::update(int) |
| 2508 | `0x0e0982` | `0x0e086a` | -280 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN12PlayerStatic11getPositionEv`<br>PlayerStatic::getPosition() |
| 2509 | `0x0e09b8` | `0x0e08a0` | -280 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN12PlayerStatic6renderEv`<br>PlayerStatic::render() |
| 2510 | `0x0e09be` | `0x0e08a6` | -280 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN12PlayerStatic9translateERKN11AbyssEngine6AEMath6VectorE`<br>PlayerStatic::translate(AbyssEngine::AEMath::Vector const&) |
| 2511 | `0x0e09c0` | `0x0e08a8` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12PlayerStatic7collideEfff`<br>PlayerStatic::collide(float, float, float) |
| 2512 | `0x0e09c4` | `0x0e08ac` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12PlayerStatic12outerCollideEfff`<br>PlayerStatic::outerCollide(float, float, float) |
| 2513 | `0x0e09c8` | `0x0e08b0` | -280 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN8NewsItemC1EibPbiii`<br>NewsItem::NewsItem(int, bool, bool*, int, int, int) |
| 2514 | `0x0e09c8` | `0x0e08b0` | -280 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN8NewsItemC2EibPbiii`<br>NewsItem::NewsItem(int, bool, bool*, int, int, int) |
| 2515 | `0x0e09e6` | `0x0e08ce` | -280 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN8NewsItemD1Ev`<br>NewsItem::~NewsItem() |
| 2516 | `0x0e09e6` | `0x0e08ce` | -280 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN8NewsItemD2Ev`<br>NewsItem::~NewsItem() |
| 2517 | `0x0e09fa` | `0x0e08e2` | -280 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN8NewsItem5cloneEv`<br>NewsItem::clone() |
| 2518 | `0x0e0a44` | `0x0e092c` | -280 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN12WantedWindowC2Ev`<br>WantedWindow::WantedWindow() |
| 2519 | `0x0e0a44` | `0x0e092c` | -280 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN12WantedWindowC1Ev`<br>WantedWindow::WantedWindow() |
| 2520 | `0x0e0b38` | `0x0e0a20` | -280 | 908 | 908 | 0 | 100.0% |  | Y | `_ZN12WantedWindow4initEv`<br>WantedWindow::init() |
| 2521 | `0x0e0ec4` | `0x0e0dac` | -280 | 228 | 228 | 0 | 100.0% |  | Y | `_ZN12WantedWindowD2Ev`<br>WantedWindow::~WantedWindow() |
| 2522 | `0x0e0ec4` | `0x0e0dac` | -280 | 228 | 228 | 0 | 100.0% |  | Y | `_ZN12WantedWindowD1Ev`<br>WantedWindow::~WantedWindow() |
| 2523 | `0x0e0fa8` | `0x0e0e90` | -280 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP9ImagePartEvR5ArrayIT_E`<br>void ArrayReleaseClasses<ImagePart*>(Array<ImagePart*>&) |
| 2524 | `0x0e0ff8` | `0x0e0ee0` | -280 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9ImagePartED2Ev`<br>Array<ImagePart*>::~Array() |
| 2525 | `0x0e100e` | `0x0e0ef6` | -280 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP6WantedEvT_R5ArrayIS2_E`<br>void ArrayAdd<Wanted*>(Wanted*, Array<Wanted*>&) |
| 2526 | `0x0e1034` | `0x0e0f1c` | -280 | 1904 | 1904 | 0 | 100.0% |  | Y | `_ZN12WantedWindow12selectWantedEi`<br>WantedWindow::selectWanted(int) |
| 2527 | `0x0e17a4` | `0x0e168c` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12WantedWindow15setHangarUpdateEb`<br>WantedWindow::setHangarUpdate(bool) |
| 2528 | `0x0e17a8` | `0x0e1690` | -280 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN12WantedWindow25getRelativeScrollStartPosEv`<br>WantedWindow::getRelativeScrollStartPos() |
| 2529 | `0x0e17d8` | `0x0e16c0` | -280 | 84 | 84 | 0 | 100.0% | Y | Y | `_ZN12WantedWindow23getRelativeScrollHeightEv`<br>WantedWindow::getRelativeScrollHeight() |
| 2530 | `0x0e182c` | `0x0e1714` | -280 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN12WantedWindow8render3DEv`<br>WantedWindow::render3D() |
| 2531 | `0x0e1838` | `0x0e1720` | -280 | 1928 | 1928 | 0 | 100.0% |  | Y | `_ZN12WantedWindow4drawEv`<br>WantedWindow::draw() |
| 2532 | `0x0e1fc0` | `0x0e1ea8` | -280 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN12WantedWindow19getWantedAtPositionEii`<br>WantedWindow::getWantedAtPosition(int, int) |
| 2533 | `0x0e2024` | `0x0e1f0c` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12WantedWindow17hangarNeedsUpdateEv`<br>WantedWindow::hangarNeedsUpdate() |
| 2534 | `0x0e2028` | `0x0e1f10` | -280 | 218 | 218 | 0 | 100.0% |  | Y | `_ZN12WantedWindow6updateEi`<br>WantedWindow::update(int) |
| 2535 | `0x0e2104` | `0x0e1fec` | -280 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN12WantedWindow12OnTouchBeginEii`<br>WantedWindow::OnTouchBegin(int, int) |
| 2536 | `0x0e21a0` | `0x0e2088` | -280 | 268 | 268 | 0 | 100.0% |  | Y | `_ZN12WantedWindow11OnTouchMoveEii`<br>WantedWindow::OnTouchMove(int, int) |
| 2537 | `0x0e22ac` | `0x0e2194` | -280 | 924 | 924 | 0 | 100.0% |  | Y | `_ZN12WantedWindow10OnTouchEndEii`<br>WantedWindow::OnTouchEnd(int, int) |
| 2538 | `0x0e2648` | `0x0e2530` | -280 | 824 | 824 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroidC2EiP10AEGeometryiiRKN11AbyssEngine6AEMath6VectorEfi`<br>PlayerAsteroid::PlayerAsteroid(int, AEGeometry*, int, int, AbyssEngine::AEMath::Vector const&, float, int) |
| 2539 | `0x0e2648` | `0x0e2530` | -280 | 824 | 824 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroidC1EiP10AEGeometryiiRKN11AbyssEngine6AEMath6VectorEfi`<br>PlayerAsteroid::PlayerAsteroid(int, AEGeometry*, int, int, AbyssEngine::AEMath::Vector const&, float, int) |
| 2540 | `0x0e2980` | `0x0e2868` | -280 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroidD1Ev`<br>PlayerAsteroid::~PlayerAsteroid() |
| 2541 | `0x0e2980` | `0x0e2868` | -280 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroidD2Ev`<br>PlayerAsteroid::~PlayerAsteroid() |
| 2542 | `0x0e29b4` | `0x0e289c` | -280 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroidD0Ev`<br>PlayerAsteroid::~PlayerAsteroid() |
| 2543 | `0x0e29c4` | `0x0e28ac` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid16setAsteroidIndexEi`<br>PlayerAsteroid::setAsteroidIndex(int) |
| 2544 | `0x0e29ca` | `0x0e28b2` | -280 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid11getPositionEv`<br>PlayerAsteroid::getPosition() |
| 2545 | `0x0e29d6` | `0x0e28be` | -280 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>PlayerAsteroid::setPosition(AbyssEngine::AEMath::Vector const&) |
| 2546 | `0x0e29dc` | `0x0e28c4` | -280 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid9translateERKN11AbyssEngine6AEMath6VectorE`<br>PlayerAsteroid::translate(AbyssEngine::AEMath::Vector const&) |
| 2547 | `0x0e29e2` | `0x0e28ca` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid9isMinableEv`<br>PlayerAsteroid::isMinable() |
| 2548 | `0x0e29e8` | `0x0e28d0` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid10getQualityEv`<br>PlayerAsteroid::getQuality() |
| 2549 | `0x0e29ee` | `0x0e28d6` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid10getScalingEv`<br>PlayerAsteroid::getScaling() |
| 2550 | `0x0e29f4` | `0x0e28dc` | -280 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid20getQualityFrameIndexEv`<br>PlayerAsteroid::getQualityFrameIndex() |
| 2551 | `0x0e2a00` | `0x0e28e8` | -280 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid16getQualityStringEv`<br>PlayerAsteroid::getQualityString() |
| 2552 | `0x0e2a3c` | `0x0e2924` | -280 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid17setAsteroidCenterEN11AbyssEngine6AEMath6VectorE`<br>PlayerAsteroid::setAsteroidCenter(AbyssEngine::AEMath::Vector) |
| 2553 | `0x0e2a90` | `0x0e2978` | -280 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid18setRotationEnabledEb`<br>PlayerAsteroid::setRotationEnabled(bool) |
| 2554 | `0x0e2a98` | `0x0e2980` | -280 | 320 | 320 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid8initPushERKN11AbyssEngine6AEMath6VectorEi`<br>PlayerAsteroid::initPush(AbyssEngine::AEMath::Vector const&, int) |
| 2555 | `0x0e2bd8` | `0x0e2ac0` | -280 | 328 | 328 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid4pushEi`<br>PlayerAsteroid::push(int) |
| 2556 | `0x0e2d20` | `0x0e2c08` | -280 | 792 | 792 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid6updateEi`<br>PlayerAsteroid::update(int) |
| 2557 | `0x0e3038` | `0x0e2f20` | -280 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid6renderEv`<br>PlayerAsteroid::render() |
| 2558 | `0x0e306e` | `0x0e2f56` | -280 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid12outerCollideEfff`<br>PlayerAsteroid::outerCollide(float, float, float) |
| 2559 | `0x0e3078` | `0x0e2f60` | -280 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN14PlayerAsteroid12outerCollideEN11AbyssEngine6AEMath6VectorE`<br>PlayerAsteroid::outerCollide(AbyssEngine::AEMath::Vector) |
| 2560 | `0x0e3084` | `0x0e2f6c` | -280 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid19getProjectionVectorERKN11AbyssEngine6AEMath6VectorE`<br>PlayerAsteroid::getProjectionVector(AbyssEngine::AEMath::Vector const&) |
| 2561 | `0x0e3104` | `0x0e2fec` | -280 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN14PlayerAsteroid7collideEfff`<br>PlayerAsteroid::collide(float, float, float) |
| 2562 | `0x0e31e0` | `0x0e30c8` | -280 | 612 | 612 | 0 | 80.1% |  |  | `_ZN7GlobalsC2Ev`<br>Globals::Globals() |
| 2563 | `0x0e31e0` | `0x0e30c8` | -280 | 612 | 612 | 0 | 80.1% |  |  | `_ZN7GlobalsC1Ev`<br>Globals::Globals() |
| 2564 | `0x0e3444` | `0x0e332c` | -280 | 468 | 468 | 0 | 100.0% |  | Y | `_ZN7GlobalsD1Ev`<br>Globals::~Globals() |
| 2565 | `0x0e3444` | `0x0e332c` | -280 | 468 | 468 | 0 | 100.0% |  | Y | `_ZN7GlobalsD2Ev`<br>Globals::~Globals() |
| 2566 | `0x0e3618` | `0x0e3500` | -280 | 800 | 800 | 0 | 100.0% |  | Y | `_ZN7Globals4initEPN11AbyssEngine18ApplicationManagerEPNS0_6EngineE`<br>Globals::init(AbyssEngine::ApplicationManager*, AbyssEngine::Engine*) |
| 2567 | `0x0e3938` | `0x0e3820` | -280 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN7Globals10resetHintsEv`<br>Globals::resetHints() |
| 2568 | `0x0e3964` | `0x0e384c` | -280 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN19ParticleSettingsRef10initializeEv`<br>ParticleSettingsRef::initialize() |
| 2569 | `0x0e3994` | `0x0e387c` | -280 | 724 | 724 | 0 | 100.0% |  | Y | `_ZN7Globals16longToTimeStringExRN11AbyssEngine6StringE`<br>Globals::longToTimeString(long long, AbyssEngine::String&) |
| 2570 | `0x0e3c68` | `0x0e3b50` | -280 | 416 | 416 | 0 | 100.0% |  | Y | `_ZN7Globals25longToTimeStringNoSecondsExRN11AbyssEngine6StringE`<br>Globals::longToTimeStringNoSeconds(long long, AbyssEngine::String&) |
| 2571 | `0x0e3e08` | `0x0e3cf0` | -280 | 684 | 684 | 0 | 100.0% |  | Y | `_ZN7Globals15createBillBoardEiiffffi`<br>Globals::createBillBoard(int, int, float, float, float, float, int) |
| 2572 | `0x0e40b4` | `0x0e3f9c` | -280 | 380 | 380 | 0 | 100.0% |  | Y | `_ZN7Globals13getRandomNameEib`<br>Globals::getRandomName(int, bool) |
| 2573 | `0x0e4230` | `0x0e4118` | -280 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN7Globals7getLineEjN11AbyssEngine6StringEiPS1_`<br>Globals::getLine(unsigned int, AbyssEngine::String, int, AbyssEngine::String*) |
| 2574 | `0x0e4384` | `0x0e426c` | -280 | 520 | 520 | 0 | 100.0% |  | Y | `_ZN7Globals12getLineArrayEjRKN11AbyssEngine6StringEiP5ArrayIPS1_E`<br>Globals::getLineArray(unsigned int, AbyssEngine::String const&, int, Array<AbyssEngine::String*>*) |
| 2575 | `0x0e458c` | `0x0e4474` | -280 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN7Globals9drawLinesEjP5ArrayIPN11AbyssEngine6StringEEiib`<br>Globals::drawLines(unsigned int, Array<AbyssEngine::String*>*, int, int, bool) |
| 2576 | `0x0e4608` | `0x0e44f0` | -280 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN7Globals9drawLinesEjP5ArrayIPN11AbyssEngine6StringEEiijb`<br>Globals::drawLines(unsigned int, Array<AbyssEngine::String*>*, int, int, unsigned int, bool) |
| 2577 | `0x0e4688` | `0x0e4570` | -280 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN7Globals9drawLinesEjP5ArrayIPN11AbyssEngine6StringEEii`<br>Globals::drawLines(unsigned int, Array<AbyssEngine::String*>*, int, int) |
| 2578 | `0x0e469c` | `0x0e4584` | -280 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN7Globals16getBoundedStringERKN11AbyssEngine6StringEi`<br>Globals::getBoundedString(AbyssEngine::String const&, int) |
| 2579 | `0x0e479c` | `0x0e4684` | -280 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN7Globals16releaseResourcesEv`<br>Globals::releaseResources() |
| 2580 | `0x0e47c4` | `0x0e46ac` | -280 | 536 | 536 | 0 | 100.0% |  | Y | `_ZN7Globals8loadFontEi`<br>Globals::loadFont(int) |
| 2581 | `0x0e49dc` | `0x0e48c4` | -280 | 176 | 176 | 0 | 100.0% |  | Y | `_ZN7Globals21getRandomEnemyFighterEi`<br>Globals::getRandomEnemyFighter(int) |
| 2582 | `0x0e4a8c` | `0x0e4974` | -280 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN7Globals19getRandomPlanetNameEv`<br>Globals::getRandomPlanetName() |
| 2583 | `0x0e4af0` | `0x0e49d8` | -280 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN7Globals16getRandomStationEv`<br>Globals::getRandomStation() |
| 2584 | `0x0e4b38` | `0x0e4a20` | -280 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN7Globals24getRandomSystemForDrinksEv`<br>Globals::getRandomSystemForDrinks() |
| 2585 | `0x0e4b68` | `0x0e4a50` | -280 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN7Globals11getItemNameEi`<br>Globals::getItemName(int) |
| 2586 | `0x0e4b90` | `0x0e4a78` | -280 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN7Globals18reportLeaderboardsEv`<br>Globals::reportLeaderboards() |
| 2587 | `0x0e4bb4` | `0x0e4a9c` | -280 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN7Globals29reportSupernovaChallengeScoreEv`<br>Globals::reportSupernovaChallengeScore() |
| 2588 | `0x0e4bb8` | `0x0e4aa0` | -280 | 604 | 604 | 0 | 100.0% |  | Y | `_ZN7Globals17getWreckCollisionEiP10AEGeometry`<br>Globals::getWreckCollision(int, AEGeometry*) |
| 2589 | `0x0e4e14` | `0x0e4cfc` | -280 | 1856 | 1856 | 0 | 100.0% |  | Y | `_ZN7Globals12getShipGroupEiib`<br>Globals::getShipGroup(int, int, bool) |
| 2590 | `0x0e5554` | `0x0e543c` | -280 | 78 | 78 | 0 | 100.0% |  | Y | `_ZN7Globals25startNewSoundResourceListEv`<br>Globals::startNewSoundResourceList() |
| 2591 | `0x0e55a2` | `0x0e548a` | -280 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Globals20getSoundResourceListEv`<br>Globals::getSoundResourceList() |
| 2592 | `0x0e55a6` | `0x0e548e` | -280 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN7Globals22addSoundResourceToListEi`<br>Globals::addSoundResourceToList(int) |
| 2593 | `0x0e55cc` | `0x0e54b4` | -280 | 588 | 588 | 0 | 100.0% |  | Y | `_ZN7Globals26playMusicAndFadeOutCurrentEi`<br>Globals::playMusicAndFadeOutCurrent(int) |
| 2594 | `0x0e5818` | `0x0e5700` | -280 | 1724 | 1724 | 0 | 100.0% |  | Y | `_ZN7Globals18getDialogueSoundIdEiP5Agent`<br>Globals::getDialogueSoundId(int, Agent*) |
| 2595 | `0x0e5ed4` | `0x0e5dbc` | -280 | 9518 | 9518 | 0 | 100.0% |  | Y | `_ZN7Globals19getAgentMissionTextEP5Agent`<br>Globals::getAgentMissionText(Agent*) |
| 2596 | `0x0e8404` | `0x0e82ec` | -280 | 864 | 864 | 0 | 100.0% |  | Y | `_ZN7Globals14setCoordsSteerEiiiiRtS0_S0_S0_S0_S0_S0_S0_S0_S0_`<br>Globals::setCoordsSteer(int, int, int, int, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&) |
| 2597 | `0x0e8764` | `0x0e864c` | -280 | 1208 | 1208 | 0 | 100.0% |  | Y | `_ZN7Globals13setCoordsFireEiijjRjRtS1_S1_S1_S1_S1_S1_S1_S1_S1_S1_S1_`<br>Globals::setCoordsFire(int, int, unsigned int, unsigned int, unsigned int&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&, unsigned short&) |
| 2598 | `0x0e8c1c` | `0x0e8b04` | -280 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN7Globals26getKeyBindingReplaceStringEi`<br>Globals::getKeyBindingReplaceString(int) |
| 2599 | `0x0e8c68` | `0x0e8b50` | -280 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN7Globals16getKeyActionNameEi`<br>Globals::getKeyActionName(int) |
| 2600 | `0x0e8c72` | `0x0e8b5a` | -280 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN7Globals23replaceKeyBindingTokensERKN11AbyssEngine6StringE`<br>Globals::replaceKeyBindingTokens(AbyssEngine::String const&) |
| 2601 | `0x0e8c80` | `0x0e8b68` | -280 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN7Globals4sqrtEf`<br>Globals::sqrt(float) |
| 2602 | `0x0e8c88` | `0x0e8b70` | -280 | 716 | 716 | 0 | 100.0% |  | Y | `_ZN7Globals26getInAppPurchaseArrayIndexEiP5ArrayIPN11AbyssEngine6StringEE`<br>Globals::getInAppPurchaseArrayIndex(int, Array<AbyssEngine::String*>*) |
| 2603 | `0x0e8f54` | `0x0e8e3c` | -280 | 171316 | 171316 | 0 | 100.0% |  | Y | `_Z17BuildResourceListPN11AbyssEngine6EngineE`<br>BuildResourceList(AbyssEngine::Engine*) |
| 2604 | `0x112c88` | `0x112b70` | -280 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15ResourceTextureC2EPKcf`<br>AbyssEngine::ResourceTexture::ResourceTexture(char const*, float) |
| 2605 | `0x112cbc` | `0x112ba4` | -280 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ResourceMaterialC2EttNS_9BlendModeE`<br>AbyssEngine::ResourceMaterial::ResourceMaterial(unsigned short, unsigned short, AbyssEngine::BlendMode) |
| 2606 | `0x112cf0` | `0x112bd8` | -280 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine12ResourceMeshC2EPKctb`<br>AbyssEngine::ResourceMesh::ResourceMesh(char const*, unsigned short, bool) |
| 2607 | `0x112d20` | `0x112c08` | -280 | 44 | 44 | 0 | 100.0% | Y | Y | `_ZN11AbyssEngine16ResourceMaterialC2EtNS_9BlendModeE`<br>AbyssEngine::ResourceMaterial::ResourceMaterial(unsigned short, AbyssEngine::BlendMode) |
| 2608 | `0x112d4c` | `0x112c34` | -280 | 21562 | 21562 | 0 | 100.0% |  | Y | `_Z13loadPortraitsPN11AbyssEngine6EngineE`<br>loadPortraits(AbyssEngine::Engine*) |
| 2609 | `0x118188` | `0x118070` | -280 | 13642 | 13642 | 0 | 100.0% |  | Y | `_Z27loadLowTexturesAndMaterialsPN11AbyssEngine6EngineE`<br>loadLowTexturesAndMaterials(AbyssEngine::Engine*) |
| 2610 | `0x11b6d2` | `0x11b5ba` | -280 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf`<br>AbyssEngine::ResourceTexture::ResourceTexture(AbyssEngine::String const&, float) |
| 2611 | `0x11b710` | `0x11b5f6` | -282 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN4NodeC2Ei`<br>Node::Node(int) |
| 2612 | `0x11b710` | `0x11b5f6` | -282 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN4NodeC1Ei`<br>Node::Node(int) |
| 2613 | `0x11b73c` | `0x11b622` | -282 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP4NodeEC2Ev`<br>Array<Node*>::Array() |
| 2614 | `0x11b758` | `0x11b63e` | -282 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN4NodeD2Ev`<br>Node::~Node() |
| 2615 | `0x11b758` | `0x11b63e` | -282 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN4NodeD1Ev`<br>Node::~Node() |
| 2616 | `0x11b768` | `0x11b64e` | -282 | 20 | 20 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIP4NodeEvR5ArrayIT_E`<br>void ArrayRelease<Node*>(Array<Node*>&) |
| 2617 | `0x11b77c` | `0x11b662` | -282 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN16SystemPathFinderC2Ev`<br>SystemPathFinder::SystemPathFinder() |
| 2618 | `0x11b77c` | `0x11b662` | -282 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN16SystemPathFinderC1Ev`<br>SystemPathFinder::SystemPathFinder() |
| 2619 | `0x11b77e` | `0x11b664` | -282 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN16SystemPathFinderD2Ev`<br>SystemPathFinder::~SystemPathFinder() |
| 2620 | `0x11b77e` | `0x11b664` | -282 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN16SystemPathFinderD1Ev`<br>SystemPathFinder::~SystemPathFinder() |
| 2621 | `0x11b780` | `0x11b666` | -282 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN16SystemPathFinder15getJumpDistanceEP5ArrayIP11SolarSystemEii`<br>SystemPathFinder::getJumpDistance(Array<SolarSystem*>*, int, int) |
| 2622 | `0x11b7a8` | `0x11b68c` | -284 | 360 | 360 | 0 | 100.0% |  | Y | `_ZN16SystemPathFinder13getSystemPathEP5ArrayIP11SolarSystemEii`<br>SystemPathFinder::getSystemPath(Array<SolarSystem*>*, int, int) |
| 2623 | `0x11b910` | `0x11b7f4` | -284 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP4NodeEvjR5ArrayIT_E`<br>void ArraySetLength<Node*>(unsigned int, Array<Node*>&) |
| 2624 | `0x11b944` | `0x11b828` | -284 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP4NodeEvT_R5ArrayIS2_E`<br>void ArrayAdd<Node*>(Node*, Array<Node*>&) |
| 2625 | `0x11b968` | `0x11b84c` | -284 | 200 | 200 | 0 | 100.0% |  | Y | `_ZN16SystemPathFinder6searchEP4NodeS1_`<br>SystemPathFinder::search(Node*, Node*) |
| 2626 | `0x11ba30` | `0x11b914` | -284 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP4NodeEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Node*>(Array<Node*>&) |
| 2627 | `0x11ba80` | `0x11b964` | -284 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP4NodeED2Ev`<br>Array<Node*>::~Array() |
| 2628 | `0x11ba96` | `0x11b97a` | -284 | 122 | 122 | 0 | 100.0% |  | Y | `_ZN16SystemPathFinder13constructPathEP4Node`<br>SystemPathFinder::constructPath(Node*) |
| 2629 | `0x11bb10` | `0x11b9f4` | -284 | 68 | 68 | 0 | 100.0% |  | Y | `_Z11ArrayRemoveIP4NodeEvT_R5ArrayIS2_E`<br>void ArrayRemove<Node*>(Node*, Array<Node*>&) |
| 2630 | `0x11bb54` | `0x11ba38` | -284 | 32 | 32 | 0 | 100.0% | Y | Y | `_ZN16SystemPathFinder8containsEP5ArrayIP4NodeES2_`<br>SystemPathFinder::contains(Array<Node*>*, Node*) |
| 2631 | `0x11bb74` | `0x11ba58` | -284 | 162 | 162 | 0 | 100.0% |  | Y | `_ZN5RouteC1EPii`<br>Route::Route(int*, int) |
| 2632 | `0x11bb74` | `0x11ba58` | -284 | 162 | 162 | 0 | 100.0% |  | Y | `_ZN5RouteC2EPii`<br>Route::Route(int*, int) |
| 2633 | `0x11bc16` | `0x11bafa` | -284 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8WaypointEC2Ev`<br>Array<Waypoint*>::Array() |
| 2634 | `0x11bc32` | `0x11bb16` | -284 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP8WaypointEvT_R5ArrayIS2_E`<br>void ArrayAdd<Waypoint*>(Waypoint*, Array<Waypoint*>&) |
| 2635 | `0x11bc56` | `0x11bb3a` | -284 | 154 | 154 | 0 | 100.0% |  | Y | `_ZN5RouteC2EPiP5ArrayIP8KIPlayerES0_i`<br>Route::Route(int*, Array<KIPlayer*>*, int*, int) |
| 2636 | `0x11bc56` | `0x11bb3a` | -284 | 154 | 154 | 0 | 100.0% |  | Y | `_ZN5RouteC1EPiP5ArrayIP8KIPlayerES0_i`<br>Route::Route(int*, Array<KIPlayer*>*, int*, int) |
| 2637 | `0x11bcf0` | `0x11bbd4` | -284 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN5RouteD2Ev`<br>Route::~Route() |
| 2638 | `0x11bcf0` | `0x11bbd4` | -284 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN5RouteD1Ev`<br>Route::~Route() |
| 2639 | `0x11bd34` | `0x11bc18` | -284 | 74 | 74 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP8WaypointEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Waypoint*>(Array<Waypoint*>&) |
| 2640 | `0x11bd7e` | `0x11bc62` | -284 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8WaypointED2Ev`<br>Array<Waypoint*>::~Array() |
| 2641 | `0x11bd94` | `0x11bc78` | -284 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN5Route5resetEv`<br>Route::reset() |
| 2642 | `0x11bdb8` | `0x11bc9c` | -284 | 44 | 44 | 0 | 100.0% | Y | Y | `_ZN5Route12setNewCoordsEN11AbyssEngine6AEMath6VectorE`<br>Route::setNewCoords(AbyssEngine::AEMath::Vector) |
| 2643 | `0x11bde4` | `0x11bcc8` | -284 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Route7setLoopEb`<br>Route::setLoop(bool) |
| 2644 | `0x11bde8` | `0x11bccc` | -284 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN5Route16getDockingTargetEv`<br>Route::getDockingTarget() |
| 2645 | `0x11be00` | `0x11bce4` | -284 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN5Route16getDockingTargetEi`<br>Route::getDockingTarget(int) |
| 2646 | `0x11be16` | `0x11bcfa` | -284 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN5Route14getDockingTimeEv`<br>Route::getDockingTime() |
| 2647 | `0x11be1c` | `0x11bd00` | -284 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN5Route14getDockingTimeEi`<br>Route::getDockingTime(int) |
| 2648 | `0x11be34` | `0x11bd18` | -284 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN5Route11getWaypointEi`<br>Route::getWaypoint(int) |
| 2649 | `0x11bed8` | `0x11bdbc` | -284 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN5Route11getWaypointEv`<br>Route::getWaypoint() |
| 2650 | `0x11bede` | `0x11bdc2` | -284 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN5Route15getLastWaypointEv`<br>Route::getLastWaypoint() |
| 2651 | `0x11bee8` | `0x11bdcc` | -284 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN5Route15waypointDefinedEv`<br>Route::waypointDefined() |
| 2652 | `0x11bef2` | `0x11bdd6` | -284 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Route10getCurrentEv`<br>Route::getCurrent() |
| 2653 | `0x11bef6` | `0x11bdda` | -284 | 102 | 102 | 0 | 100.0% |  | Y | `_ZN5Route13reachWaypointEi`<br>Route::reachWaypoint(int) |
| 2654 | `0x11bf5c` | `0x11be40` | -284 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Route6lengthEv`<br>Route::length() |
| 2655 | `0x11bf62` | `0x11be46` | -284 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN5Route6updateERKN11AbyssEngine6AEMath6VectorE`<br>Route::update(AbyssEngine::AEMath::Vector const&) |
| 2656 | `0x11bf70` | `0x11be54` | -284 | 260 | 260 | 0 | 100.0% |  | Y | `_ZN5Route6updateEfff`<br>Route::update(float, float, float) |
| 2657 | `0x11c074` | `0x11bf58` | -284 | 66 | 66 | 0 | 100.0% |  | Y | `_ZN5Route13getExactCloneEv`<br>Route::getExactClone() |
| 2658 | `0x11c0b6` | `0x11bf9a` | -284 | 320 | 320 | 0 | 100.0% |  | Y | `_ZN5Route5cloneEv`<br>Route::clone() |
| 2659 | `0x11c1f6` | `0x11c0da` | -284 | 94 | 94 | 0 | 100.0% | Y | Y | `_ZN5Route9translateERKN11AbyssEngine6AEMath6VectorE`<br>Route::translate(AbyssEngine::AEMath::Vector const&) |
| 2660 | `0x11c254` | `0x11c138` | -284 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFarC1EiP10AEGeometryfff`<br>PlayerStaticFar::PlayerStaticFar(int, AEGeometry*, float, float, float) |
| 2661 | `0x11c254` | `0x11c138` | -284 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFarC2EiP10AEGeometryfff`<br>PlayerStaticFar::PlayerStaticFar(int, AEGeometry*, float, float, float) |
| 2662 | `0x11c2c4` | `0x11c1a8` | -284 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFarD2Ev`<br>PlayerStaticFar::~PlayerStaticFar() |
| 2663 | `0x11c2c4` | `0x11c1a8` | -284 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFarD1Ev`<br>PlayerStaticFar::~PlayerStaticFar() |
| 2664 | `0x11c2c8` | `0x11c1ac` | -284 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFarD0Ev`<br>PlayerStaticFar::~PlayerStaticFar() |
| 2665 | `0x11c2d8` | `0x11c1bc` | -284 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN15PlayerStaticFar12setYRotationEi`<br>PlayerStaticFar::setYRotation(int) |
| 2666 | `0x11c2da` | `0x11c1be` | -284 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFar6renderEv`<br>PlayerStaticFar::render() |
| 2667 | `0x11c2e0` | `0x11c1c4` | -284 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFar6updateEi`<br>PlayerStaticFar::update(int) |
| 2668 | `0x11c460` | `0x11c344` | -284 | 142 | 142 | 0 | 100.0% | Y | Y | `_ZN15PlayerStaticFar7collideEfff`<br>PlayerStaticFar::collide(float, float, float) |
| 2669 | `0x11c4ee` | `0x11c3d2` | -284 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN15PlayerStaticFar15getInitPositionEN11AbyssEngine6AEMath6VectorE`<br>PlayerStaticFar::getInitPosition(AbyssEngine::AEMath::Vector) |
| 2670 | `0x11c4fc` | `0x11c3e0` | -284 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN15PlayerStaticFar12outerCollideEfff`<br>PlayerStaticFar::outerCollide(float, float, float) |
| 2671 | `0x11c506` | `0x11c3ea` | -284 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN15PlayerStaticFar12outerCollideEN11AbyssEngine6AEMath6VectorE`<br>PlayerStaticFar::outerCollide(AbyssEngine::AEMath::Vector) |
| 2672 | `0x11c510` | `0x11c3f4` | -284 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFar19getProjectionVectorERKN11AbyssEngine6AEMath6VectorE`<br>PlayerStaticFar::getProjectionVector(AbyssEngine::AEMath::Vector const&) |
| 2673 | `0x11c52e` | `0x11c412` | -284 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN15PlayerStaticFar25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE`<br>PlayerStaticFar::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) |
| 2674 | `0x11c54c` | `0x11c430` | -284 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN12ImageFactoryC2Ev`<br>ImageFactory::ImageFactory() |
| 2675 | `0x11c54c` | `0x11c430` | -284 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN12ImageFactoryC1Ev`<br>ImageFactory::ImageFactory() |
| 2676 | `0x11c5d0` | `0x11c4b4` | -284 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN12ImageFactory6reloadEv`<br>ImageFactory::reload() |
| 2677 | `0x11c6b0` | `0x11c594` | -284 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN12ImageFactoryD2Ev`<br>ImageFactory::~ImageFactory() |
| 2678 | `0x11c6b0` | `0x11c594` | -284 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN12ImageFactoryD1Ev`<br>ImageFactory::~ImageFactory() |
| 2679 | `0x11c6cc` | `0x11c5b0` | -284 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN12ImageFactory10createCharEi`<br>ImageFactory::createChar(int) |
| 2680 | `0x11c6f4` | `0x11c5d8` | -284 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN12ImageFactory10createCharEbi`<br>ImageFactory::createChar(bool, int) |
| 2681 | `0x11c774` | `0x11c658` | -284 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN12ImageFactory8loadCharEPi`<br>ImageFactory::loadChar(int*) |
| 2682 | `0x11c7e4` | `0x11c6c8` | -284 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9ImagePartEC2Ev`<br>Array<ImagePart*>::Array() |
| 2683 | `0x11c800` | `0x11c6e4` | -284 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP9ImagePartEvjR5ArrayIT_E`<br>void ArraySetLength<ImagePart*>(unsigned int, Array<ImagePart*>&) |
| 2684 | `0x11c834` | `0x11c718` | -284 | 268 | 268 | 0 | 100.0% |  | Y | `_ZN12ImageFactory9loadImageEiii`<br>ImageFactory::loadImage(int, int, int) |
| 2685 | `0x11c940` | `0x11c824` | -284 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN12ImageFactory8drawCharEP5ArrayIP9ImagePartEiib`<br>ImageFactory::drawChar(Array<ImagePart*>*, int, int, bool) |
| 2686 | `0x11c9b4` | `0x11c898` | -284 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN12ImageFactory8drawItemEiiii`<br>ImageFactory::drawItem(int, int, int, int) |
| 2687 | `0x11ca50` | `0x11c934` | -284 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN12ImageFactory14getItemImageIdEi`<br>ImageFactory::getItemImageId(int) |
| 2688 | `0x11ca60` | `0x11c944` | -284 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN12ImageFactory8drawItemEiii`<br>ImageFactory::drawItem(int, int, int) |
| 2689 | `0x11cad0` | `0x11c9b4` | -284 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN12ImageFactory8drawShipEiii`<br>ImageFactory::drawShip(int, int, int) |
| 2690 | `0x11cb5c` | `0x11ca40` | -284 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN14PlayerCreatureC2EiiP6PlayerP10AEGeometryfff`<br>PlayerCreature::PlayerCreature(int, int, Player*, AEGeometry*, float, float, float) |
| 2691 | `0x11cb5c` | `0x11ca40` | -284 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN14PlayerCreatureC1EiiP6PlayerP10AEGeometryfff`<br>PlayerCreature::PlayerCreature(int, int, Player*, AEGeometry*, float, float, float) |
| 2692 | `0x11cc28` | `0x11cb0c` | -284 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature8calmDownEv`<br>PlayerCreature::calmDown() |
| 2693 | `0x11cc40` | `0x11cb20` | -288 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN14PlayerCreature5resetEv`<br>PlayerCreature::reset() |
| 2694 | `0x11ccd4` | `0x11cbb4` | -288 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PlayerCreatureD2Ev`<br>PlayerCreature::~PlayerCreature() |
| 2695 | `0x11ccd4` | `0x11cbb4` | -288 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PlayerCreatureD1Ev`<br>PlayerCreature::~PlayerCreature() |
| 2696 | `0x11ccd8` | `0x11cbb8` | -288 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN14PlayerCreatureD0Ev`<br>PlayerCreature::~PlayerCreature() |
| 2697 | `0x11cce8` | `0x11cbc8` | -288 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN14PlayerCreature4rageEi`<br>PlayerCreature::rage(int) |
| 2698 | `0x11cd38` | `0x11cc18` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature8isHookedEv`<br>PlayerCreature::isHooked() |
| 2699 | `0x11cd3e` | `0x11cc1e` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature8isCaughtEv`<br>PlayerCreature::isCaught() |
| 2700 | `0x11cd44` | `0x11cc24` | -288 | 10 | 10 | 0 | 100.0% |  | Y | `_ZN14PlayerCreature4hookEi`<br>PlayerCreature::hook(int) |
| 2701 | `0x11cd4e` | `0x11cc2e` | -288 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature6unhookEv`<br>PlayerCreature::unhook() |
| 2702 | `0x11cd68` | `0x11cc48` | -288 | 592 | 592 | 0 | 100.0% |  | Y | `_ZN14PlayerCreature6updateEi`<br>PlayerCreature::update(int) |
| 2703 | `0x11cfb8` | `0x11ce98` | -288 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN14PlayerCreature6renderEv`<br>PlayerCreature::render() |
| 2704 | `0x11cfdc` | `0x11cebc` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature7collideEfff`<br>PlayerCreature::collide(float, float, float) |
| 2705 | `0x11cfe0` | `0x11cec0` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature12outerCollideEfff`<br>PlayerCreature::outerCollide(float, float, float) |
| 2706 | `0x11cfe4` | `0x11cec4` | -288 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN14PlayerCreature9getWeightEv`<br>PlayerCreature::getWeight() |
| 2707 | `0x11cff8` | `0x11ced8` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature12getItemIndexEv`<br>PlayerCreature::getItemIndex() |
| 2708 | `0x11cffe` | `0x11cede` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature15getMaxEnduranceEv`<br>PlayerCreature::getMaxEndurance() |
| 2709 | `0x11d004` | `0x11cee4` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerCreature12getEnduranceEv`<br>PlayerCreature::getEndurance() |
| 2710 | `0x11d00c` | `0x11ceec` | -288 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN9LensFlareC1EPN11AbyssEngine11PaintCanvasE`<br>LensFlare::LensFlare(AbyssEngine::PaintCanvas*) |
| 2711 | `0x11d00c` | `0x11ceec` | -288 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN9LensFlareC2EPN11AbyssEngine11PaintCanvasE`<br>LensFlare::LensFlare(AbyssEngine::PaintCanvas*) |
| 2712 | `0x11d06c` | `0x11cf4c` | -288 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN9LensFlareD1Ev`<br>LensFlare::~LensFlare() |
| 2713 | `0x11d06c` | `0x11cf4c` | -288 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN9LensFlareD2Ev`<br>LensFlare::~LensFlare() |
| 2714 | `0x11d082` | `0x11cf62` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9LensFlare6updateEi`<br>LensFlare::update(int) |
| 2715 | `0x11d088` | `0x11cf68` | -288 | 1568 | 1568 | 0 | 100.0% |  | Y | `_ZN9LensFlare8render2DEfffi`<br>LensFlare::render2D(float, float, float, int) |
| 2716 | `0x11d6a8` | `0x11d588` | -288 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN8StandingC2Ev`<br>Standing::Standing() |
| 2717 | `0x11d6a8` | `0x11d588` | -288 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN8StandingC1Ev`<br>Standing::Standing() |
| 2718 | `0x11d6c8` | `0x11d5a8` | -288 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN8StandingD2Ev`<br>Standing::~Standing() |
| 2719 | `0x11d6c8` | `0x11d5a8` | -288 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN8StandingD1Ev`<br>Standing::~Standing() |
| 2720 | `0x11d6de` | `0x11d5be` | -288 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN8Standing11setStandingEii`<br>Standing::setStanding(int, int) |
| 2721 | `0x11d6e6` | `0x11d5c6` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8Standing12setStandingsEPi`<br>Standing::setStandings(int*) |
| 2722 | `0x11d6ea` | `0x11d5ca` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8Standing12getStandingsEv`<br>Standing::getStandings() |
| 2723 | `0x11d6ee` | `0x11d5ce` | -288 | 56 | 56 | 0 | 100.0% | Y | Y | `_ZN8Standing11getStandingEi`<br>Standing::getStanding(int) |
| 2724 | `0x11d728` | `0x11d608` | -288 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN8Standing15getStandingRateEi`<br>Standing::getStandingRate(int) |
| 2725 | `0x11d74c` | `0x11d62c` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN8Standing22setPlayerSignatureRaceEi`<br>Standing::setPlayerSignatureRace(int) |
| 2726 | `0x11d750` | `0x11d630` | -288 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN8Standing12rehabilitateEi`<br>Standing::rehabilitate(int) |
| 2727 | `0x11d780` | `0x11d660` | -288 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN8Standing17isEnemyWithAnyoneEv`<br>Standing::isEnemyWithAnyone() |
| 2728 | `0x11d7a2` | `0x11d682` | -288 | 102 | 102 | 0 | 100.0% | Y | Y | `_ZN8Standing7isEnemyEi`<br>Standing::isEnemy(int) |
| 2729 | `0x11d808` | `0x11d6e8` | -288 | 102 | 102 | 0 | 100.0% | Y | Y | `_ZN8Standing8isFriendEi`<br>Standing::isFriend(int) |
| 2730 | `0x11d86e` | `0x11d74e` | -288 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN8Standing9isNeutralEi`<br>Standing::isNeutral(int) |
| 2731 | `0x11d890` | `0x11d770` | -288 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN8Standing12getEnemyRaceEi`<br>Standing::getEnemyRace(int) |
| 2732 | `0x11d8a8` | `0x11d788` | -288 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN8Standing11applyPointsEii`<br>Standing::applyPoints(int, int) |
| 2733 | `0x11d8d0` | `0x11d7b0` | -288 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN8Standing9applyKillEi`<br>Standing::applyKill(int) |
| 2734 | `0x11d93c` | `0x11d81c` | -288 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN8Standing11applyDelictEii`<br>Standing::applyDelict(int, int) |
| 2735 | `0x11d98c` | `0x11d86c` | -288 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8Standing15applyStealCargoEi`<br>Standing::applyStealCargo(int) |
| 2736 | `0x11d992` | `0x11d872` | -288 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN8Standing21applyMissionCompletedEi`<br>Standing::applyMissionCompleted(int) |
| 2737 | `0x11d99a` | `0x11d87a` | -288 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8Standing12applyDisableEi`<br>Standing::applyDisable(int) |
| 2738 | `0x11d9a0` | `0x11d880` | -288 | 84 | 84 | 0 | 100.0% | Y | Y | `_ZN8Standing15getMissionBonusEi`<br>Standing::getMissionBonus(int) |
| 2739 | `0x11d9f4` | `0x11d8d4` | -288 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN10HangarListC1Ev`<br>HangarList::HangarList() |
| 2740 | `0x11d9f4` | `0x11d8d4` | -288 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN10HangarListC2Ev`<br>HangarList::HangarList() |
| 2741 | `0x11d9fe` | `0x11d8de` | -288 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN10HangarListD2Ev`<br>HangarList::~HangarList() |
| 2742 | `0x11d9fe` | `0x11d8de` | -288 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN10HangarListD1Ev`<br>HangarList::~HangarList() |
| 2743 | `0x11da10` | `0x11d8f0` | -288 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN10HangarList7releaseEv`<br>HangarList::release() |
| 2744 | `0x11da7c` | `0x11d95c` | -288 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP8ListItemEvR5ArrayIT_E`<br>void ArrayReleaseClasses<ListItem*>(Array<ListItem*>&) |
| 2745 | `0x11dacc` | `0x11d9ac` | -288 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8ListItemED2Ev`<br>Array<ListItem*>::~Array() |
| 2746 | `0x11dae2` | `0x11d9c2` | -288 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP5ArrayIP8ListItemEEvRS0_IT_E`<br>void ArrayReleaseClasses<Array<ListItem*>*>(Array<Array<ListItem*>*>&) |
| 2747 | `0x11db32` | `0x11da12` | -288 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IP8ListItemEED2Ev`<br>Array<Array<ListItem*>*>::~Array() |
| 2748 | `0x11db48` | `0x11da28` | -288 | 672 | 672 | 0 | 100.0% |  | Y | `_ZN10HangarList11initShipTabEP4Ship`<br>HangarList::initShipTab(Ship*) |
| 2749 | `0x11dde8` | `0x11dcc8` | -288 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8ListItemEC2Ev`<br>Array<ListItem*>::Array() |
| 2750 | `0x11de04` | `0x11dce4` | -288 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP8ListItemEvjR5ArrayIT_E`<br>void ArraySetLength<ListItem*>(unsigned int, Array<ListItem*>&) |
| 2751 | `0x11de38` | `0x11dd18` | -288 | 592 | 592 | 0 | 100.0% |  | Y | `_ZN10HangarList11initShopTabEP5ArrayIP4ItemEPS0_IP4ShipE`<br>HangarList::initShopTab(Array<Item*>*, Array<Ship*>*) |
| 2752 | `0x11e088` | `0x11df68` | -288 | 428 | 428 | 0 | 100.0% |  | Y | `_ZN10HangarList16initBlueprintTabEP5ArrayIP9BluePrintE`<br>HangarList::initBlueprintTab(Array<BluePrint*>*) |
| 2753 | `0x11e234` | `0x11e114` | -288 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN10HangarList4initEP4ShipP5ArrayIP4ItemEPS2_IS1_EPS2_IP9BluePrintE`<br>HangarList::init(Ship*, Array<Item*>*, Array<Ship*>*, Array<BluePrint*>*) |
| 2754 | `0x11e290` | `0x11e170` | -288 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IP8ListItemEEC2Ev`<br>Array<Array<ListItem*>*>::Array() |
| 2755 | `0x11e2ac` | `0x11e18c` | -288 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP5ArrayIP8ListItemEEvjRS0_IT_E`<br>void ArraySetLength<Array<ListItem*>*>(unsigned int, Array<Array<ListItem*>*>&) |
| 2756 | `0x11e2e0` | `0x11e1c0` | -288 | 712 | 712 | 0 | 100.0% |  | Y | `_ZN10HangarList11fillBuyListEP8ListItem`<br>HangarList::fillBuyList(ListItem*) |
| 2757 | `0x11e5a8` | `0x11e488` | -288 | 376 | 376 | 0 | 100.0% |  | Y | `_ZN10HangarList19fillIngredientsListEP9BluePrintb`<br>HangarList::fillIngredientsList(BluePrint*, bool) |
| 2758 | `0x11e720` | `0x11e600` | -288 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP8ListItemEvT_R5ArrayIS2_E`<br>void ArrayAdd<ListItem*>(ListItem*, Array<ListItem*>&) |
| 2759 | `0x11e744` | `0x11e624` | -288 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN10HangarList14getCurrentItemEv`<br>HangarList::getCurrentItem() |
| 2760 | `0x11e74a` | `0x11e62a` | -288 | 40 | 40 | 0 | 100.0% | Y | Y | `_ZN10HangarList16getCurrentItemAtEi`<br>HangarList::getCurrentItemAt(int) |
| 2761 | `0x11e772` | `0x11e652` | -288 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN10HangarList18getCurrentTabItemsEv`<br>HangarList::getCurrentTabItems() |
| 2762 | `0x11e77e` | `0x11e65e` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10HangarList8getItemsEv`<br>HangarList::getItems() |
| 2763 | `0x11e782` | `0x11e662` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10HangarList13setCurrentTabEib`<br>HangarList::setCurrentTab(int, bool) |
| 2764 | `0x11e786` | `0x11e666` | -288 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN10HangarList16getCurrentLengthEv`<br>HangarList::getCurrentLength() |
| 2765 | `0x11e79a` | `0x11e67a` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10HangarList13getCurrentTabEv`<br>HangarList::getCurrentTab() |
| 2766 | `0x11e79e` | `0x11e67e` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10HangarList19getCurrentItemIndexEv`<br>HangarList::getCurrentItemIndex() |
| 2767 | `0x11e7a2` | `0x11e682` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10HangarList19setCurrentItemIndexEi`<br>HangarList::setCurrentItemIndex(int) |
| 2768 | `0x11e7a8` | `0x11e688` | -288 | 972 | 972 | 0 | 100.0% |  | Y | `_ZN10MiningGameC1EiiP3Hud`<br>MiningGame::MiningGame(int, int, Hud*) |
| 2769 | `0x11e7a8` | `0x11e688` | -288 | 972 | 972 | 0 | 100.0% |  | Y | `_ZN10MiningGameC2EiiP3Hud`<br>MiningGame::MiningGame(int, int, Hud*) |
| 2770 | `0x11eb74` | `0x11ea54` | -288 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN10MiningGameD1Ev`<br>MiningGame::~MiningGame() |
| 2771 | `0x11eb74` | `0x11ea54` | -288 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN10MiningGameD2Ev`<br>MiningGame::~MiningGame() |
| 2772 | `0x11eb92` | `0x11ea72` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN10MiningGame7gameWonEv`<br>MiningGame::gameWon() |
| 2773 | `0x11eb98` | `0x11ea78` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN10MiningGame8gameLostEv`<br>MiningGame::gameLost() |
| 2774 | `0x11eb9e` | `0x11ea7e` | -288 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN10MiningGame12getOreAmountEv`<br>MiningGame::getOreAmount() |
| 2775 | `0x11ebac` | `0x11ea8c` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN10MiningGame7gotCoreEv`<br>MiningGame::gotCore() |
| 2776 | `0x11ebb2` | `0x11ea92` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10MiningGame15getAsteroidTypeEv`<br>MiningGame::getAsteroidType() |
| 2777 | `0x11ebb6` | `0x11ea96` | -288 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN10MiningGame2upEf`<br>MiningGame::up(float) |
| 2778 | `0x11ebc8` | `0x11eaa8` | -288 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN10MiningGame4downEf`<br>MiningGame::down(float) |
| 2779 | `0x11ebda` | `0x11eaba` | -288 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN10MiningGame4leftEf`<br>MiningGame::left(float) |
| 2780 | `0x11ebec` | `0x11eacc` | -288 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN10MiningGame5rightEf`<br>MiningGame::right(float) |
| 2781 | `0x11ec00` | `0x11eae0` | -288 | 1184 | 1184 | 0 | 100.0% |  | Y | `_ZN10MiningGame6updateEi`<br>MiningGame::update(int) |
| 2782 | `0x11f0a0` | `0x11ef80` | -288 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN10MiningGame16isInCurrentLayerEv`<br>MiningGame::isInCurrentLayer() |
| 2783 | `0x11f12c` | `0x11f00c` | -288 | 1456 | 1456 | 0 | 100.0% |  | Y | `_ZN10MiningGame8render2DEv`<br>MiningGame::render2D() |
| 2784 | `0x11f6dc` | `0x11f5bc` | -288 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN14BoundingVolumeC2Effffff`<br>BoundingVolume::BoundingVolume(float, float, float, float, float, float) |
| 2785 | `0x11f718` | `0x11f5f8` | -288 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN14BoundingVolumeD2Ev`<br>BoundingVolume::~BoundingVolume() |
| 2786 | `0x11f718` | `0x11f5f8` | -288 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN14BoundingVolumeD1Ev`<br>BoundingVolume::~BoundingVolume() |
| 2787 | `0x11f74c` | `0x11f62c` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN14BoundingVolume10setVolumesEP5ArrayIPS_E`<br>BoundingVolume::setVolumes(Array<BoundingVolume*>*) |
| 2788 | `0x11f750` | `0x11f630` | -288 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN14BoundingVolume9setVolumeEPS_`<br>BoundingVolume::setVolume(BoundingVolume*) |
| 2789 | `0x11f788` | `0x11f668` | -288 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN14BoundingVolume19getProjectionVectorERKN11AbyssEngine6AEMath6VectorE`<br>BoundingVolume::getProjectionVector(AbyssEngine::AEMath::Vector const&) |
| 2790 | `0x11f7d8` | `0x11f6b8` | -288 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN14BoundingVolume18getCollisionNormalERKN11AbyssEngine6AEMath6VectorE`<br>BoundingVolume::getCollisionNormal(AbyssEngine::AEMath::Vector const&) |
| 2791 | `0x11f7e4` | `0x11f6c4` | -288 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN14BoundingVolume6updateEfff`<br>BoundingVolume::update(float, float, float) |
| 2792 | `0x11f870` | `0x11f750` | -288 | 60 | 60 | 0 | 100.0% | Y | Y | `_ZN14BoundingVolume7collideEfff`<br>BoundingVolume::collide(float, float, float) |
| 2793 | `0x11f8ac` | `0x11f78c` | -288 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN14BoundingVolume12outerCollideEfff`<br>BoundingVolume::outerCollide(float, float, float) |
| 2794 | `0x11f8b0` | `0x11f790` | -288 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN14BoundingVolume31staticProjectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorEP5ArrayIPS_E`<br>BoundingVolume::staticProjectCollisionOnSurface(AbyssEngine::AEMath::Vector const&, Array<BoundingVolume*>*) |
| 2795 | `0x11f944` | `0x11f824` | -288 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8FileReadC2Ev`<br>FileRead::FileRead() |
| 2796 | `0x11f944` | `0x11f824` | -288 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8FileReadC1Ev`<br>FileRead::FileRead() |
| 2797 | `0x11f946` | `0x11f826` | -288 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8FileReadD2Ev`<br>FileRead::~FileRead() |
| 2798 | `0x11f946` | `0x11f826` | -288 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN8FileReadD1Ev`<br>FileRead::~FileRead() |
| 2799 | `0x11f948` | `0x11f828` | -288 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN8FileRead11loadStationEi`<br>FileRead::loadStation(int) |
| 2800 | `0x11f970` | `0x11f850` | -288 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN8FileRead18loadStationsBinaryEPsi`<br>FileRead::loadStationsBinary(short*, int) |
| 2801 | `0x11fad0` | `0x11f9b0` | -288 | 396 | 396 | 0 | 100.0% |  | Y | `_ZN8FileRead13loadShipPartsEi`<br>FileRead::loadShipParts(int) |
| 2802 | `0x11fc5c` | `0x11fb3c` | -288 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN8FileRead20loadStationCollisionEi`<br>FileRead::loadStationCollision(int) |
| 2803 | `0x11fd78` | `0x11fc58` | -288 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN8FileRead18loadWreckCollisionEi`<br>FileRead::loadWreckCollision(int) |
| 2804 | `0x11fe94` | `0x11fd74` | -288 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN8FileRead19loadStaticCollisionEi`<br>FileRead::loadStaticCollision(int) |
| 2805 | `0x11ffb0` | `0x11fe90` | -288 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN8FileRead16loadStationPartsEii`<br>FileRead::loadStationParts(int, int) |
| 2806 | `0x12014c` | `0x12002c` | -288 | 364 | 364 | 0 | 100.0% |  | Y | `_ZN8FileRead18loadStationsBinaryEP11SolarSystem`<br>FileRead::loadStationsBinary(SolarSystem*) |
| 2807 | `0x1202b8` | `0x120198` | -288 | 472 | 472 | 0 | 100.0% |  | Y | `_ZN8FileRead10loadAgentsEv`<br>FileRead::loadAgents() |
| 2808 | `0x120490` | `0x120370` | -288 | 544 | 544 | 0 | 100.0% |  | Y | `_ZN8FileRead10loadWantedEv`<br>FileRead::loadWanted() |
| 2809 | `0x1206b0` | `0x120590` | -288 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN8FileRead18loadStationsBinaryEv`<br>FileRead::loadStationsBinary() |
| 2810 | `0x1207b0` | `0x120690` | -288 | 48 | 48 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIaEvjR5ArrayIT_E`<br>void ArraySetLength<signed char>(unsigned int, Array<signed char>&) |
| 2811 | `0x1207e0` | `0x1206c0` | -288 | 716 | 716 | 0 | 100.0% |  | Y | `_ZN8FileRead17loadSystemsBinaryEv`<br>FileRead::loadSystemsBinary() |
| 2812 | `0x120aac` | `0x12098c` | -288 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11SolarSystemEC2Ev`<br>Array<SolarSystem*>::Array() |
| 2813 | `0x120ac8` | `0x1209a8` | -288 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP11SolarSystemEvjR5ArrayIT_E`<br>void ArraySetLength<SolarSystem*>(unsigned int, Array<SolarSystem*>&) |
| 2814 | `0x120afc` | `0x1209dc` | -288 | 428 | 428 | 0 | 100.0% |  | Y | `_ZN8FileRead15loadItemsBinaryEv`<br>FileRead::loadItemsBinary() |
| 2815 | `0x120ca8` | `0x120b88` | -288 | 296 | 296 | 0 | 100.0% |  | Y | `_ZN8FileRead15loadShipsBinaryEv`<br>FileRead::loadShipsBinary() |
| 2816 | `0x120dd0` | `0x120cb0` | -288 | 884 | 884 | 0 | 100.0% |  | Y | `_ZN8FileRead15loadNamesBinaryEibb`<br>FileRead::loadNamesBinary(int, bool, bool) |
| 2817 | `0x121144` | `0x121024` | -288 | 336 | 336 | 0 | 100.0% |  | Y | `_ZN8FileRead10loadTickerEv`<br>FileRead::loadTicker() |
| 2818 | `0x121294` | `0x121174` | -288 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8NewsItemEC2Ev`<br>Array<NewsItem*>::Array() |
| 2819 | `0x1212b0` | `0x121190` | -288 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP8NewsItemEvjR5ArrayIT_E`<br>void ArraySetLength<NewsItem*>(unsigned int, Array<NewsItem*>&) |
| 2820 | `0x1212e4` | `0x1211c4` | -288 | 500 | 500 | 0 | 100.0% |  | Y | `_ZN8FileRead19loadWeaponPositionsEi`<br>FileRead::loadWeaponPositions(int) |
| 2821 | `0x1214d8` | `0x1213b8` | -288 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IPN11AbyssEngine6AEMath6VectorEEEC2Ev`<br>Array<Array<AbyssEngine::AEMath::Vector*>*>::Array() |
| 2822 | `0x1214f4` | `0x1213d4` | -288 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP5ArrayIPN11AbyssEngine6AEMath6VectorEEEvjRS0_IT_E`<br>void ArraySetLength<Array<AbyssEngine::AEMath::Vector*>*>(unsigned int, Array<Array<AbyssEngine::AEMath::Vector*>*>&) |
| 2823 | `0x121528` | `0x121408` | -288 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIPN11AbyssEngine6AEMath6VectorEEvT_R5ArrayIS4_E`<br>void ArrayAdd<AbyssEngine::AEMath::Vector*>(AbyssEngine::AEMath::Vector*, Array<AbyssEngine::AEMath::Vector*>&) |
| 2824 | `0x121550` | `0x121430` | -288 | 648 | 648 | 0 | 100.0% |  | Y | `_ZN8FileRead15loadSpacePointsEii`<br>FileRead::loadSpacePoints(int, int) |
| 2825 | `0x1217d8` | `0x1216b8` | -288 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP10SpacePointEC2Ev`<br>Array<SpacePoint*>::Array() |
| 2826 | `0x1217f4` | `0x1216d4` | -288 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP10SpacePointEvT_R5ArrayIS2_E`<br>void ArrayAdd<SpacePoint*>(SpacePoint*, Array<SpacePoint*>&) |
| 2827 | `0x121818` | `0x1216f8` | -288 | 3440 | 3444 | +4 | 94.4% |  |  | `_ZN13PlayerStationC1EP7Station`<br>PlayerStation::PlayerStation(Station*) |
| 2828 | `0x121818` | `0x1216f8` | -288 | 3440 | 3444 | +4 | 94.4% |  |  | `_ZN13PlayerStationC2EP7Station`<br>PlayerStation::PlayerStation(Station*) |
| 2829 | `0x122588` | `0x12246c` | -284 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN13PlayerStationD2Ev`<br>PlayerStation::~PlayerStation() |
| 2830 | `0x122588` | `0x12246c` | -284 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN13PlayerStationD1Ev`<br>PlayerStation::~PlayerStation() |
| 2831 | `0x1225ec` | `0x1224d0` | -284 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN13PlayerStationD0Ev`<br>PlayerStation::~PlayerStation() |
| 2832 | `0x1225fc` | `0x1224e0` | -284 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN13PlayerStation6updateEi`<br>PlayerStation::update(int) |
| 2833 | `0x1226e8` | `0x1225cc` | -284 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN13PlayerStation6renderEv`<br>PlayerStation::render() |
| 2834 | `0x1226f8` | `0x1225dc` | -284 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN13PlayerStation11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>PlayerStation::setPosition(AbyssEngine::AEMath::Vector const&) |
| 2835 | `0x12274c` | `0x122630` | -284 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN13PlayerStation11getPositionEv`<br>PlayerStation::getPosition() |
| 2836 | `0x12275a` | `0x12263e` | -284 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerStation7getRootEv`<br>PlayerStation::getRoot() |
| 2837 | `0x122760` | `0x122644` | -284 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN13PlayerStation10setVisibleEb`<br>PlayerStation::setVisible(bool) |
| 2838 | `0x122766` | `0x12264a` | -284 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN13PlayerStation9translateEfff`<br>PlayerStation::translate(float, float, float) |
| 2839 | `0x12276e` | `0x122652` | -284 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN13PlayerStation7collideEfff`<br>PlayerStation::collide(float, float, float) |
| 2840 | `0x122778` | `0x12265c` | -284 | 196 | 196 | 0 | 100.0% | Y | Y | `_ZN13PlayerStation12outerCollideEfff`<br>PlayerStation::outerCollide(float, float, float) |
| 2841 | `0x12283c` | `0x122720` | -284 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN13PlayerStation12outerCollideERKN11AbyssEngine6AEMath6VectorE`<br>PlayerStation::outerCollide(AbyssEngine::AEMath::Vector const&) |
| 2842 | `0x122858` | `0x12273c` | -284 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN13PlayerStation19getProjectionVectorERKN11AbyssEngine6AEMath6VectorE`<br>PlayerStation::getProjectionVector(AbyssEngine::AEMath::Vector const&) |
| 2843 | `0x12287c` | `0x122760` | -284 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN13PlayerStation25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE`<br>PlayerStation::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) |
| 2844 | `0x122890` | `0x122772` | -286 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN6WantedC2EiN11AbyssEngine6StringEiibiiiiiiiii`<br>Wanted::Wanted(int, AbyssEngine::String, int, int, bool, int, int, int, int, int, int, int, int, int) |
| 2845 | `0x122890` | `0x122772` | -286 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN6WantedC1EiN11AbyssEngine6StringEiibiiiiiiiii`<br>Wanted::Wanted(int, AbyssEngine::String, int, int, bool, int, int, int, int, int, int, int, int, int) |
| 2846 | `0x122904` | `0x1227e6` | -286 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN6WantedD2Ev`<br>Wanted::~Wanted() |
| 2847 | `0x122904` | `0x1227e6` | -286 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN6WantedD1Ev`<br>Wanted::~Wanted() |
| 2848 | `0x122920` | `0x122802` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted8getIndexEv`<br>Wanted::getIndex() |
| 2849 | `0x122924` | `0x122806` | -286 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN6Wanted7getNameEv`<br>Wanted::getName() |
| 2850 | `0x122930` | `0x122812` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted13getNumWingmenEv`<br>Wanted::getNumWingmen() |
| 2851 | `0x122934` | `0x122816` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted8getBoardEv`<br>Wanted::getBoard() |
| 2852 | `0x122938` | `0x12281a` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted7getRaceEv`<br>Wanted::getRace() |
| 2853 | `0x12293c` | `0x12281e` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted6isMaleEv`<br>Wanted::isMale() |
| 2854 | `0x122940` | `0x122822` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted7getShipEv`<br>Wanted::getShip() |
| 2855 | `0x122944` | `0x122826` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted9getWeaponEv`<br>Wanted::getWeapon() |
| 2856 | `0x122948` | `0x12282a` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted12getHitpointsEv`<br>Wanted::getHitpoints() |
| 2857 | `0x12294c` | `0x12282e` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted7getLootEv`<br>Wanted::getLoot() |
| 2858 | `0x122950` | `0x122832` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted13getLootAmountEv`<br>Wanted::getLootAmount() |
| 2859 | `0x122954` | `0x122836` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted9getRewardEv`<br>Wanted::getReward() |
| 2860 | `0x122958` | `0x12283a` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted19getRequiredBountiesEv`<br>Wanted::getRequiredBounties() |
| 2861 | `0x12295c` | `0x12283e` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted18getRequiredMissionEv`<br>Wanted::getRequiredMission() |
| 2862 | `0x122960` | `0x122842` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted18setRequiredMissionEi`<br>Wanted::setRequiredMission(int) |
| 2863 | `0x122964` | `0x122846` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted13setImagePartsEPi`<br>Wanted::setImageParts(int*) |
| 2864 | `0x122968` | `0x12284a` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted13getImagePartsEv`<br>Wanted::getImageParts() |
| 2865 | `0x12296c` | `0x12284e` | -286 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Wanted13setTerminatedEb`<br>Wanted::setTerminated(bool) |
| 2866 | `0x122972` | `0x122854` | -286 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Wanted9setActiveEb`<br>Wanted::setActive(bool) |
| 2867 | `0x122978` | `0x12285a` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted12setTravelsToEi`<br>Wanted::setTravelsTo(int) |
| 2868 | `0x12297c` | `0x12285e` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted18setCurrentLocationEi`<br>Wanted::setCurrentLocation(int) |
| 2869 | `0x122980` | `0x122862` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted11setLastSeenEi`<br>Wanted::setLastSeen(int) |
| 2870 | `0x122984` | `0x122866` | -286 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Wanted8isActiveEv`<br>Wanted::isActive() |
| 2871 | `0x12298a` | `0x12286c` | -286 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN6Wanted12isTerminatedEv`<br>Wanted::isTerminated() |
| 2872 | `0x122990` | `0x122872` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted11getLastSeenEv`<br>Wanted::getLastSeen() |
| 2873 | `0x122994` | `0x122876` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted12getTravelsToEv`<br>Wanted::getTravelsTo() |
| 2874 | `0x122998` | `0x12287a` | -286 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Wanted18getCurrentLocationEv`<br>Wanted::getCurrentLocation() |
| 2875 | `0x12299c` | `0x122880` | -284 | 9600 | 9600 | 0 | 84.5% |  |  | `_ZN15MenuTouchWindowC1Ei`<br>MenuTouchWindow::MenuTouchWindow(int) |
| 2876 | `0x12299c` | `0x122880` | -284 | 9600 | 9600 | 0 | 84.5% |  |  | `_ZN15MenuTouchWindowC2Ei`<br>MenuTouchWindow::MenuTouchWindow(int) |
| 2877 | `0x124f1c` | `0x124e00` | -284 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow18loadPreviewRecordsEv`<br>MenuTouchWindow::loadPreviewRecords() |
| 2878 | `0x124fd8` | `0x124ebc` | -284 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow9addButtonEiN11AbyssEngine6StringEiP5ArrayIP11TouchButtonEi`<br>MenuTouchWindow::addButton(int, AbyssEngine::String, int, Array<TouchButton*>*, int) |
| 2879 | `0x12510c` | `0x124ff0` | -284 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP11TouchButtonEvT_R5ArrayIS2_E`<br>void ArrayAdd<TouchButton*>(TouchButton*, Array<TouchButton*>&) |
| 2880 | `0x125130` | `0x125014` | -284 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11TouchSliderEC2Ev`<br>Array<TouchSlider*>::Array() |
| 2881 | `0x12514c` | `0x125030` | -284 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP11TouchSliderEvT_R5ArrayIS2_E`<br>void ArrayAdd<TouchSlider*>(TouchSlider*, Array<TouchSlider*>&) |
| 2882 | `0x125170` | `0x125054` | -284 | 538 | 538 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindowD2Ev`<br>MenuTouchWindow::~MenuTouchWindow() |
| 2883 | `0x125170` | `0x125054` | -284 | 538 | 538 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindowD1Ev`<br>MenuTouchWindow::~MenuTouchWindow() |
| 2884 | `0x12538a` | `0x12526e` | -284 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP11TouchSliderEvR5ArrayIT_E`<br>void ArrayReleaseClasses<TouchSlider*>(Array<TouchSlider*>&) |
| 2885 | `0x1253da` | `0x1252be` | -284 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP11TouchSliderED2Ev`<br>Array<TouchSlider*>::~Array() |
| 2886 | `0x1253f0` | `0x1252d4` | -284 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP10GameRecordED2Ev`<br>Array<GameRecord*>::~Array() |
| 2887 | `0x125406` | `0x1252ea` | -284 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN15MenuTouchWindow17isInMissionScreenEv`<br>MenuTouchWindow::isInMissionScreen() |
| 2888 | `0x125414` | `0x1252f8` | -284 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow15setCutsceneModeEb`<br>MenuTouchWindow::setCutsceneMode(bool) |
| 2889 | `0x125454` | `0x125338` | -284 | 1564 | 1564 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow19createRecordButtonsEb`<br>MenuTouchWindow::createRecordButtons(bool) |
| 2890 | `0x125a70` | `0x125954` | -284 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP5ArrayIPN11AbyssEngine6StringEEEvRS0_IT_E`<br>void ArrayReleaseClasses<Array<AbyssEngine::String*>*>(Array<Array<AbyssEngine::String*>*>&) |
| 2891 | `0x125ac0` | `0x1259a4` | -284 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IPN11AbyssEngine6StringEEED2Ev`<br>Array<Array<AbyssEngine::String*>*>::~Array() |
| 2892 | `0x125ad6` | `0x1259ba` | -284 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IPN11AbyssEngine6StringEEEC2Ev`<br>Array<Array<AbyssEngine::String*>*>::Array() |
| 2893 | `0x125af2` | `0x1259d6` | -284 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP5ArrayIPN11AbyssEngine6StringEEEvjRS0_IT_E`<br>void ArraySetLength<Array<AbyssEngine::String*>*>(unsigned int, Array<Array<AbyssEngine::String*>*>&) |
| 2894 | `0x125b28` | `0x125a0c` | -284 | 184 | 184 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow8saveGameEi`<br>MenuTouchWindow::saveGame(int) |
| 2895 | `0x125be0` | `0x125ac4` | -284 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow20showSupernovaMessageEv`<br>MenuTouchWindow::showSupernovaMessage() |
| 2896 | `0x125c2c` | `0x125b10` | -284 | 328 | 328 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow8loadGameEi`<br>MenuTouchWindow::loadGame(int) |
| 2897 | `0x125d74` | `0x125c58` | -284 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN15MenuTouchWindow25getRelativeScrollStartPosEv`<br>MenuTouchWindow::getRelativeScrollStartPos() |
| 2898 | `0x125da4` | `0x125c88` | -284 | 84 | 84 | 0 | 100.0% | Y | Y | `_ZN15MenuTouchWindow23getRelativeScrollHeightEv`<br>MenuTouchWindow::getRelativeScrollHeight() |
| 2899 | `0x125df8` | `0x125cdc` | -284 | 1188 | 1188 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow16drawLoadSaveMenuEb`<br>MenuTouchWindow::drawLoadSaveMenu(bool) |
| 2900 | `0x12629c` | `0x126180` | -284 | 13052 | 13052 | 0 | 92.7% |  |  | `_ZN15MenuTouchWindow4drawEv`<br>MenuTouchWindow::draw() |
| 2901 | `0x129598` | `0x12947c` | -284 | 1596 | 1596 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow6updateEi`<br>MenuTouchWindow::update(int) |
| 2902 | `0x129bd4` | `0x129ab8` | -284 | 1896 | 1896 | 0 | 97.2% |  |  | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`<br>MenuTouchWindow::OnTouchBegin(int, int, void*) |
| 2903 | `0x12a33c` | `0x12a220` | -284 | 1920 | 1920 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow11OnTouchMoveEiiPv`<br>MenuTouchWindow::OnTouchMove(int, int, void*) |
| 2904 | `0x12aabc` | `0x12a9a0` | -284 | 8132 | 8128 | -4 | 80.9% |  |  | `_ZN15MenuTouchWindow10OnTouchEndEiiPv`<br>MenuTouchWindow::OnTouchEnd(int, int, void*) |
| 2905 | `0x12ca80` | `0x12c960` | -288 | 612 | 612 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow13startValkyrieEv`<br>MenuTouchWindow::startValkyrie() |
| 2906 | `0x12cce4` | `0x12cbc4` | -288 | 672 | 672 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow14startSupernovaEv`<br>MenuTouchWindow::startSupernova() |
| 2907 | `0x12cf84` | `0x12ce64` | -288 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow9startGOF2Ev`<br>MenuTouchWindow::startGOF2() |
| 2908 | `0x12cfe4` | `0x12cec4` | -288 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow8render3DEv`<br>MenuTouchWindow::render3D() |
| 2909 | `0x12cffa` | `0x12ceda` | -288 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN15MenuTouchWindow16isShowingMessageEv`<br>MenuTouchWindow::isShowingMessage() |
| 2910 | `0x12d000` | `0x12cee0` | -288 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN15MenuTouchWindow15inCinematicModeEv`<br>MenuTouchWindow::inCinematicMode() |
| 2911 | `0x12d00e` | `0x12ceee` | -288 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN15MenuTouchWindow18isMakingScreenshotEv`<br>MenuTouchWindow::isMakingScreenshot() |
| 2912 | `0x12d01e` | `0x12cefe` | -288 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN15MenuTouchWindow11hideMessageEv`<br>MenuTouchWindow::hideMessage() |
| 2913 | `0x12d026` | `0x12cf06` | -288 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow20setSkipButtonVisibleEb`<br>MenuTouchWindow::setSkipButtonVisible(bool) |
| 2914 | `0x12d064` | `0x12cf44` | -288 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow11callDlcMenuEv`<br>MenuTouchWindow::callDlcMenu() |
| 2915 | `0x12d0cc` | `0x12cfac` | -288 | 764 | 764 | 0 | 100.0% |  | Y | `_ZN15MenuTouchWindow23startSupernovaChallengeEv`<br>MenuTouchWindow::startSupernovaChallenge() |
| 2916 | `0x12d3c8` | `0x12dfb0` | +3048 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN14PendingProductC2EP9BluePrint`<br>PendingProduct::PendingProduct(BluePrint*) |
| 2917 | `0x12d3c8` | `0x12dfb0` | +3048 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN14PendingProductC1EP9BluePrint`<br>PendingProduct::PendingProduct(BluePrint*) |
| 2918 | `0x12d448` | `0x12e030` | +3048 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN14PendingProductC2EiN11AbyssEngine6StringEii`<br>PendingProduct::PendingProduct(int, AbyssEngine::String, int, int) |
| 2919 | `0x12d448` | `0x12e030` | +3048 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN14PendingProductC1EiN11AbyssEngine6StringEii`<br>PendingProduct::PendingProduct(int, AbyssEngine::String, int, int) |
| 2920 | `0x12d480` | `0x12e068` | +3048 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PendingProductD1Ev`<br>PendingProduct::~PendingProduct() |
| 2921 | `0x12d480` | `0x12e068` | +3048 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14PendingProductD2Ev`<br>PendingProduct::~PendingProduct() |
| 2922 | `0x12d484` | `0x12e06c` | +3048 | 2488 | 2488 | 0 | 100.0% |  | Y | `_ZN5RadarC2EP5Level`<br>Radar::Radar(Level*) |
| 2923 | `0x12d484` | `0x12e06c` | +3048 | 2488 | 2488 | 0 | 100.0% |  | Y | `_ZN5RadarC1EP5Level`<br>Radar::Radar(Level*) |
| 2924 | `0x12de3c` | `0x12ea24` | +3048 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN5RadarD2Ev`<br>Radar::~Radar() |
| 2925 | `0x12de3c` | `0x12ea24` | +3048 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN5RadarD1Ev`<br>Radar::~Radar() |
| 2926 | `0x12de60` | `0x12ea48` | +3048 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN5Radar6updateEN11AbyssEngine6AEMath6VectorE`<br>Radar::update(AbyssEngine::AEMath::Vector) |
| 2927 | `0x12df48` | `0x12eb30` | +3048 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN5Radar17elipsoidIntersectEiiN11AbyssEngine6AEMath6VectorE`<br>Radar::elipsoidIntersect(int, int, AbyssEngine::AEMath::Vector) |
| 2928 | `0x12e01c` | `0x12ec04` | +3048 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN5Radar6updateEP8KIPlayer`<br>Radar::update(KIPlayer*) |
| 2929 | `0x12e050` | `0x12ec38` | +3048 | 9908 | 9932 | +24 | 69.3% |  |  | `_ZN5Radar4drawEP6PlayerP3Hudi`<br>Radar::draw(Player*, Hud*, int) |
| 2930 | `0x130704` | `0x131304` | +3072 | 624 | 624 | 0 | 100.0% |  | Y | `_ZN5Radar12calcDistanceEffffff`<br>Radar::calcDistance(float, float, float, float, float, float) |
| 2931 | `0x130974` | `0x131574` | +3072 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN5Radar18getPlanetDockIndexEv`<br>Radar::getPlanetDockIndex() |
| 2932 | `0x130998` | `0x131598` | +3072 | 1680 | 1680 | 0 | 96.0% |  |  | `_ZN5Radar15drawCurrentLockEP3Hud`<br>Radar::drawCurrentLock(Hud*) |
| 2933 | `0x131028` | `0x131c28` | +3072 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Radar17getLockedGasCloudEv`<br>Radar::getLockedGasCloud() |
| 2934 | `0x13102c` | `0x131c2c` | +3072 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Radar17getLockedAsteroidEv`<br>Radar::getLockedAsteroid() |
| 2935 | `0x131030` | `0x131c30` | +3072 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Radar14unlockAsteroidEv`<br>Radar::unlockAsteroid() |
| 2936 | `0x131036` | `0x131c36` | +3072 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Radar14getLockedEnemyEv`<br>Radar::getLockedEnemy() |
| 2937 | `0x13103a` | `0x131c3a` | +3072 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN5Radar13stationLockedEv`<br>Radar::stationLocked() |
| 2938 | `0x13104e` | `0x131c4e` | +3072 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN5Radar19getTurretScopeWidthEv`<br>Radar::getTurretScopeWidth() |
| 2939 | `0x131056` | `0x131c56` | +3072 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Radar15isPlasmaInRangeEv`<br>Radar::isPlasmaInRange() |
| 2940 | `0x13105c` | `0x131c5c` | +3072 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Radar10hasScannerEv`<br>Radar::hasScanner() |
| 2941 | `0x131064` | `0x131c64` | +3072 | 760 | 760 | 0 | 100.0% |  | Y | `_ZN13AutoPilotListC1EP5Level`<br>AutoPilotList::AutoPilotList(Level*) |
| 2942 | `0x131064` | `0x131c64` | +3072 | 760 | 760 | 0 | 100.0% |  | Y | `_ZN13AutoPilotListC2EP5Level`<br>AutoPilotList::AutoPilotList(Level*) |
| 2943 | `0x13135c` | `0x131f5c` | +3072 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN13AutoPilotList4downEv`<br>AutoPilotList::down() |
| 2944 | `0x131378` | `0x131f78` | +3072 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN13AutoPilotListD2Ev`<br>AutoPilotList::~AutoPilotList() |
| 2945 | `0x131378` | `0x131f78` | +3072 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN13AutoPilotListD1Ev`<br>AutoPilotList::~AutoPilotList() |
| 2946 | `0x131398` | `0x131f98` | +3072 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN13AutoPilotList4fireEv`<br>AutoPilotList::fire() |
| 2947 | `0x13139c` | `0x131f9c` | +3072 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN13AutoPilotList15getTargetStringEv`<br>AutoPilotList::getTargetString() |
| 2948 | `0x1313cc` | `0x131fcc` | +3072 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN13AutoPilotList2upEv`<br>AutoPilotList::up() |
| 2949 | `0x1313e8` | `0x131fe8` | +3072 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN13AutoPilotList4drawEv`<br>AutoPilotList::draw() |
| 2950 | `0x1314d0` | `0x1320d0` | +3072 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN13AutoPilotList5touchEii`<br>AutoPilotList::touch(int, int) |
| 2951 | `0x131528` | `0x132128` | +3072 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN14ListItemWindowC2Ev`<br>ListItemWindow::ListItemWindow() |
| 2952 | `0x131528` | `0x132128` | +3072 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN14ListItemWindowC1Ev`<br>ListItemWindow::ListItemWindow() |
| 2953 | `0x1315b4` | `0x1321b4` | +3072 | 162 | 162 | 0 | 100.0% |  | Y | `_ZN14ListItemWindowD1Ev`<br>ListItemWindow::~ListItemWindow() |
| 2954 | `0x1315b4` | `0x1321b4` | +3072 | 162 | 162 | 0 | 100.0% |  | Y | `_ZN14ListItemWindowD2Ev`<br>ListItemWindow::~ListItemWindow() |
| 2955 | `0x131658` | `0x132258` | +3072 | 6820 | 6820 | 0 | 100.0% |  | Y | `_ZN14ListItemWindow3setEP8ListItemjjjjb`<br>ListItemWindow::set(ListItem*, unsigned int, unsigned int, unsigned int, unsigned int, bool) |
| 2956 | `0x1330fc` | `0x133cfc` | +3072 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14ListItemWindow11shows3DShipEv`<br>ListItemWindow::shows3DShip() |
| 2957 | `0x133104` | `0x133d04` | +3072 | 1892 | 1892 | 0 | 100.0% |  | Y | `_ZN14ListItemWindow4drawEv`<br>ListItemWindow::draw() |
| 2958 | `0x133868` | `0x134468` | +3072 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN14ListItemWindow6updateEi`<br>ListItemWindow::update(int) |
| 2959 | `0x1339c8` | `0x1345c8` | +3072 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN14ListItemWindow6renderEv`<br>ListItemWindow::render() |
| 2960 | `0x133a74` | `0x134674` | +3072 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN14ListItemWindow12OnTouchBeginEii`<br>ListItemWindow::OnTouchBegin(int, int) |
| 2961 | `0x133ad4` | `0x1346d4` | +3072 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN14ListItemWindow11OnTouchMoveEii`<br>ListItemWindow::OnTouchMove(int, int) |
| 2962 | `0x133b04` | `0x134704` | +3072 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN14ListItemWindow10OnTouchEndEii`<br>ListItemWindow::OnTouchEnd(int, int) |
| 2963 | `0x133b68` | `0x134778` | +3088 | 2900 | 2900 | 0 | 100.0% |  | Y | `_ZN10StarSystemC2Ei`<br>StarSystem::StarSystem(int) |
| 2964 | `0x133b68` | `0x134778` | +3088 | 2900 | 2900 | 0 | 100.0% |  | Y | `_ZN10StarSystemC1Ei`<br>StarSystem::StarSystem(int) |
| 2965 | `0x1346bc` | `0x1352cc` | +3088 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIjEvjR5ArrayIT_E`<br>void ArraySetLength<unsigned int>(unsigned int, Array<unsigned int>&) |
| 2966 | `0x1346f0` | `0x135300` | +3088 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN5ArrayIN11AbyssEngine6AEMath6VectorEEC2Ev`<br>Array<AbyssEngine::AEMath::Vector>::Array() |
| 2967 | `0x134710` | `0x135320` | +3088 | 60 | 60 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIN11AbyssEngine6AEMath6VectorEEvjR5ArrayIT_E`<br>void ArraySetLength<AbyssEngine::AEMath::Vector>(unsigned int, Array<AbyssEngine::AEMath::Vector>&) |
| 2968 | `0x13474c` | `0x13535c` | +3088 | 1476 | 1476 | 0 | 100.0% |  | Y | `_ZN10StarSystem9initLightEv`<br>StarSystem::initLight() |
| 2969 | `0x134d10` | `0x135920` | +3088 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN10StarSystemD2Ev`<br>StarSystem::~StarSystem() |
| 2970 | `0x134d10` | `0x135920` | +3088 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN10StarSystemD1Ev`<br>StarSystem::~StarSystem() |
| 2971 | `0x134d94` | `0x1359a4` | +3088 | 20 | 20 | 0 | 100.0% |  | Y | `_Z12ArrayReleaseIN11AbyssEngine6AEMath6VectorEEvR5ArrayIT_E`<br>void ArrayRelease<AbyssEngine::AEMath::Vector>(Array<AbyssEngine::AEMath::Vector>&) |
| 2972 | `0x134da8` | `0x1359b8` | +3088 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIN11AbyssEngine6AEMath6VectorEED2Ev`<br>Array<AbyssEngine::AEMath::Vector>::~Array() |
| 2973 | `0x134dbe` | `0x1359ce` | +3088 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN10StarSystem17getLightDirectionEv`<br>StarSystem::getLightDirection() |
| 2974 | `0x134dcc` | `0x1359dc` | +3088 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN10StarSystem20switchPlanetForIntroEv`<br>StarSystem::switchPlanetForIntro() |
| 2975 | `0x134e4c` | `0x135a5c` | +3088 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN10StarSystem29switchSunForSupernovaReversalEv`<br>StarSystem::switchSunForSupernovaReversal() |
| 2976 | `0x134ea0` | `0x135ab0` | +3088 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN10StarSystem26switchSunForSupernovaIntroEv`<br>StarSystem::switchSunForSupernovaIntro() |
| 2977 | `0x134fc4` | `0x135bd4` | +3088 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN10StarSystem30switchSunForSupernovaExpansionEv`<br>StarSystem::switchSunForSupernovaExpansion() |
| 2978 | `0x134ffc` | `0x135c0c` | +3088 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN10StarSystem28scaleSunDuringSupernovaIntroEi`<br>StarSystem::scaleSunDuringSupernovaIntro(int) |
| 2979 | `0x135064` | `0x135c74` | +3088 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN10StarSystem15updateSupernovaEi`<br>StarSystem::updateSupernova(int) |
| 2980 | `0x1350b8` | `0x135cc8` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10StarSystem10getPlanetsEv`<br>StarSystem::getPlanets() |
| 2981 | `0x1350bc` | `0x135ccc` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10StarSystem17getStationIndicesEv`<br>StarSystem::getStationIndices() |
| 2982 | `0x1350c0` | `0x135cd0` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10StarSystem16getPlanetTargetsEv`<br>StarSystem::getPlanetTargets() |
| 2983 | `0x1350c4` | `0x135cd4` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10StarSystem20getPlanetScaleFactorEv`<br>StarSystem::getPlanetScaleFactor() |
| 2984 | `0x1350c8` | `0x135cd8` | +3088 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN10StarSystem15renderSunStreakEv`<br>StarSystem::renderSunStreak() |
| 2985 | `0x135104` | `0x135d14` | +3088 | 996 | 996 | 0 | 100.0% |  | Y | `_ZN10StarSystem6renderEv`<br>StarSystem::render() |
| 2986 | `0x1354e8` | `0x1360f8` | +3088 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN10StarSystem8render2DEv`<br>StarSystem::render2D() |
| 2987 | `0x135594` | `0x1361a4` | +3088 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN10StarSystem6rotateEiii`<br>StarSystem::rotate(int, int, int) |
| 2988 | `0x135598` | `0x1361a8` | +3088 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBoxC1Eiiii`<br>ScrollTouchBox::ScrollTouchBox(int, int, int, int) |
| 2989 | `0x135598` | `0x1361a8` | +3088 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBoxC2Eiiii`<br>ScrollTouchBox::ScrollTouchBox(int, int, int, int) |
| 2990 | `0x1355d8` | `0x1361e8` | +3088 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBoxD1Ev`<br>ScrollTouchBox::~ScrollTouchBox() |
| 2991 | `0x1355d8` | `0x1361e8` | +3088 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBoxD2Ev`<br>ScrollTouchBox::~ScrollTouchBox() |
| 2992 | `0x135600` | `0x136210` | +3088 | 268 | 268 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBox7setTextEN11AbyssEngine6StringEi`<br>ScrollTouchBox::setText(AbyssEngine::String, int) |
| 2993 | `0x13570c` | `0x13631c` | +3088 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBox7setTextEN11AbyssEngine6StringE`<br>ScrollTouchBox::setText(AbyssEngine::String) |
| 2994 | `0x13576c` | `0x13637c` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox11setPositionEii`<br>ScrollTouchBox::setPosition(int, int) |
| 2995 | `0x135772` | `0x136382` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox12setYPositionEi`<br>ScrollTouchBox::setYPosition(int) |
| 2996 | `0x135778` | `0x136388` | +3088 | 408 | 408 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBox4drawEv`<br>ScrollTouchBox::draw() |
| 2997 | `0x135910` | `0x136520` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox15setTextCenteredEb`<br>ScrollTouchBox::setTextCentered(bool) |
| 2998 | `0x135918` | `0x136528` | +3088 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox25getRelativeScrollStartPosEv`<br>ScrollTouchBox::getRelativeScrollStartPos() |
| 2999 | `0x135948` | `0x136558` | +3088 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox23getRelativeScrollHeightEv`<br>ScrollTouchBox::getRelativeScrollHeight() |
| 3000 | `0x135998` | `0x1365a8` | +3088 | 138 | 138 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox6updateEi`<br>ScrollTouchBox::update(int) |
| 3001 | `0x135a22` | `0x136632` | +3088 | 50 | 50 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox13touchIsInsideEii`<br>ScrollTouchBox::touchIsInside(int, int) |
| 3002 | `0x135a54` | `0x136664` | +3088 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN14ScrollTouchBox12OnTouchBeginEii`<br>ScrollTouchBox::OnTouchBegin(int, int) |
| 3003 | `0x135a72` | `0x136682` | +3088 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox11OnTouchMoveEii`<br>ScrollTouchBox::OnTouchMove(int, int) |
| 3004 | `0x135a98` | `0x1366a8` | +3088 | 68 | 68 | 0 | 100.0% | Y | Y | `_ZN14ScrollTouchBox10OnTouchEndEii`<br>ScrollTouchBox::OnTouchEnd(int, int) |
| 3005 | `0x135adc` | `0x1366ec` | +3088 | 8332 | 8332 | 0 | 100.0% |  | Y | `_ZN11LevelScriptC1EP5LevelP3HudP5RadarP18TargetFollowCamera`<br>LevelScript::LevelScript(Level*, Hud*, Radar*, TargetFollowCamera*) |
| 3006 | `0x135adc` | `0x1366ec` | +3088 | 8332 | 8332 | 0 | 100.0% |  | Y | `_ZN11LevelScriptC2EP5LevelP3HudP5RadarP18TargetFollowCamera`<br>LevelScript::LevelScript(Level*, Hud*, Radar*, TargetFollowCamera*) |
| 3007 | `0x137b68` | `0x138778` | +3088 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN11LevelScript31setAutoPilotToProgrammedStationEv`<br>LevelScript::setAutoPilotToProgrammedStation() |
| 3008 | `0x137c3c` | `0x13884c` | +3088 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN11LevelScriptD1Ev`<br>LevelScript::~LevelScript() |
| 3009 | `0x137c3c` | `0x13884c` | +3088 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN11LevelScriptD2Ev`<br>LevelScript::~LevelScript() |
| 3010 | `0x137d18` | `0x138928` | +3088 | 57232 | 57252 | +20 | 50.4% |  |  | `_ZN11LevelScript7processEi`<br>LevelScript::process(int) |
| 3011 | `0x145ca8` | `0x1468cc` | +3108 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN11LevelScript11resetCameraEP5Level`<br>LevelScript::resetCamera(Level*) |
| 3012 | `0x145d24` | `0x146948` | +3108 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN11LevelScript10lookBehindEv`<br>LevelScript::lookBehind() |
| 3013 | `0x145d84` | `0x1469a8` | +3108 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN11LevelScript8render3DEv`<br>LevelScript::render3D() |
| 3014 | `0x145e08` | `0x146a2c` | +3108 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN11LevelScript12skipSequenceEv`<br>LevelScript::skipSequence() |
| 3015 | `0x145e44` | `0x146a68` | +3108 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11LevelScript18canSkipCutsceneNowEv`<br>LevelScript::canSkipCutsceneNow() |
| 3016 | `0x145e94` | `0x146ab8` | +3108 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN11LevelScript12skipCutsceneEv`<br>LevelScript::skipCutscene() |
| 3017 | `0x146030` | `0x146c54` | +3108 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN11LevelScript22resetStartSequenceOverEv`<br>LevelScript::resetStartSequenceOver() |
| 3018 | `0x14603a` | `0x146c5e` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11LevelScript17startSequenceOverEv`<br>LevelScript::startSequenceOver() |
| 3019 | `0x146040` | `0x146c64` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11LevelScript13startSequenceEv`<br>LevelScript::startSequence() |
| 3020 | `0x146046` | `0x146c6a` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11LevelScript8getEventEv`<br>LevelScript::getEvent() |
| 3021 | `0x14604a` | `0x146c6e` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11LevelScript8setEventEi`<br>LevelScript::setEvent(int) |
| 3022 | `0x146050` | `0x146c74` | +3108 | 320 | 320 | 0 | 100.0% |  | Y | `_ZN11MovingStarsC2Ev`<br>MovingStars::MovingStars() |
| 3023 | `0x146050` | `0x146c74` | +3108 | 320 | 320 | 0 | 100.0% |  | Y | `_ZN11MovingStarsC1Ev`<br>MovingStars::MovingStars() |
| 3024 | `0x146190` | `0x146db4` | +3108 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN11MovingStarsD2Ev`<br>MovingStars::~MovingStars() |
| 3025 | `0x146190` | `0x146db4` | +3108 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN11MovingStarsD1Ev`<br>MovingStars::~MovingStars() |
| 3026 | `0x1461c8` | `0x146dec` | +3108 | 1356 | 1356 | 0 | 100.0% |  | Y | `_ZN11MovingStars6updateEiN11AbyssEngine6AEMath6MatrixEbf`<br>MovingStars::update(int, AbyssEngine::AEMath::Matrix, bool, float) |
| 3027 | `0x146714` | `0x147338` | +3108 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11MovingStars6renderEv`<br>MovingStars::render() |
| 3028 | `0x14675c` | `0x147380` | +3108 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11MovingStars9translateERKN11AbyssEngine6AEMath6VectorE`<br>MovingStars::translate(AbyssEngine::AEMath::Vector const&) |
| 3029 | `0x1467ec` | `0x147410` | +3108 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindowC1Ev`<br>ChoiceWindow::ChoiceWindow() |
| 3030 | `0x1467ec` | `0x147410` | +3108 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindowC2Ev`<br>ChoiceWindow::ChoiceWindow() |
| 3031 | `0x146870` | `0x147494` | +3108 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindowD1Ev`<br>ChoiceWindow::~ChoiceWindow() |
| 3032 | `0x146870` | `0x147494` | +3108 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindowD2Ev`<br>ChoiceWindow::~ChoiceWindow() |
| 3033 | `0x1468c8` | `0x1474ec` | +3108 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow3setERKN11AbyssEngine6StringE`<br>ChoiceWindow::set(AbyssEngine::String const&) |
| 3034 | `0x1468f4` | `0x147518` | +3108 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow3setERKN11AbyssEngine6StringES3_b`<br>ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&, bool) |
| 3035 | `0x146954` | `0x147578` | +3108 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow3setERKN11AbyssEngine6StringES3_`<br>ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&) |
| 3036 | `0x1469b0` | `0x1475d4` | +3108 | 940 | 940 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow3setERKN11AbyssEngine6StringES3_bS3_S3_S3_ii`<br>ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&, bool, AbyssEngine::String const&, AbyssEngine::String const&, AbyssEngine::String const&, int, int) |
| 3037 | `0x146d5c` | `0x147980` | +3108 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow3setERKN11AbyssEngine6StringEb`<br>ChoiceWindow::set(AbyssEngine::String const&, bool) |
| 3038 | `0x146d94` | `0x1479b8` | +3108 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12ChoiceWindow13removeButtonsEv`<br>ChoiceWindow::removeButtons() |
| 3039 | `0x146d9c` | `0x1479c0` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12ChoiceWindow9hasChoiceEv`<br>ChoiceWindow::hasChoice() |
| 3040 | `0x146da4` | `0x1479c8` | +3108 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow8setWidthEi`<br>ChoiceWindow::setWidth(int) |
| 3041 | `0x146dc4` | `0x1479e8` | +3108 | 200 | 200 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow9setHeightEi`<br>ChoiceWindow::setHeight(int) |
| 3042 | `0x146e8c` | `0x147ab0` | +3108 | 476 | 476 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow8setMedalEii`<br>ChoiceWindow::setMedal(int, int) |
| 3043 | `0x147068` | `0x147c8c` | +3108 | 332 | 332 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow13setMiscButtonERKN11AbyssEngine6StringE`<br>ChoiceWindow::setMiscButton(AbyssEngine::String const&) |
| 3044 | `0x1471b4` | `0x147dd8` | +3108 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow6updateEi`<br>ChoiceWindow::update(int) |
| 3045 | `0x1471bc` | `0x147de0` | +3108 | 784 | 784 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow4drawEv`<br>ChoiceWindow::draw() |
| 3046 | `0x1474cc` | `0x1480f0` | +3108 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow12OnTouchBeginEii`<br>ChoiceWindow::OnTouchBegin(int, int) |
| 3047 | `0x147518` | `0x14813c` | +3108 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow11OnTouchMoveEii`<br>ChoiceWindow::OnTouchMove(int, int) |
| 3048 | `0x147564` | `0x148188` | +3108 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN12ChoiceWindow10OnTouchEndEii`<br>ChoiceWindow::OnTouchEnd(int, int) |
| 3049 | `0x1475c4` | `0x1481e8` | +3108 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN12ChoiceWindow4leftEv`<br>ChoiceWindow::left() |
| 3050 | `0x1475c6` | `0x1481ea` | +3108 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN12ChoiceWindow5rightEv`<br>ChoiceWindow::right() |
| 3051 | `0x1475c8` | `0x1481ec` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12ChoiceWindow4fireEv`<br>ChoiceWindow::fire() |
| 3052 | `0x1475cc` | `0x1481f0` | +3108 | 600 | 600 | 0 | 100.0% |  | Y | `_ZN7BombGunC2EP3GunjiibP5Level`<br>BombGun::BombGun(Gun*, unsigned int, int, int, bool, Level*) |
| 3053 | `0x1475cc` | `0x1481f0` | +3108 | 600 | 600 | 0 | 100.0% |  | Y | `_ZN7BombGunC1EP3GunjiibP5Level`<br>BombGun::BombGun(Gun*, unsigned int, int, int, bool, Level*) |
| 3054 | `0x147824` | `0x148448` | +3108 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN7BombGunD2Ev`<br>BombGun::~BombGun() |
| 3055 | `0x147824` | `0x148448` | +3108 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN7BombGunD1Ev`<br>BombGun::~BombGun() |
| 3056 | `0x147858` | `0x14847c` | +3108 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN7BombGunD0Ev`<br>BombGun::~BombGun() |
| 3057 | `0x147868` | `0x14848c` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7BombGun9setPlayerEP9PlayerEgo`<br>BombGun::setPlayer(PlayerEgo*) |
| 3058 | `0x147870` | `0x148494` | +3108 | 1152 | 1152 | 0 | 100.0% |  | Y | `_ZN7BombGun6updateEi`<br>BombGun::update(int) |
| 3059 | `0x147cf0` | `0x148914` | +3108 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN7BombGun6renderEv`<br>BombGun::render() |
| 3060 | `0x147d10` | `0x148934` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9RocketGun11isRocketGunEv`<br>RocketGun::isRocketGun() |
| 3061 | `0x147d14` | `0x148938` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7BombGun9isBombGunEv`<br>BombGun::isBombGun() |
| 3062 | `0x147d18` | `0x14893c` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbstractGun9isMineGunEv`<br>AbstractGun::isMineGun() |
| 3063 | `0x147d1c` | `0x148940` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbstractGun12isAutoTurretEv`<br>AbstractGun::isAutoTurret() |
| 3064 | `0x147d20` | `0x148944` | +3108 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN12HangarWindowC2Ev`<br>HangarWindow::HangarWindow() |
| 3065 | `0x147d20` | `0x148944` | +3108 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN12HangarWindowC1Ev`<br>HangarWindow::HangarWindow() |
| 3066 | `0x147d9c` | `0x1489c0` | +3108 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN12HangarWindowD1Ev`<br>HangarWindow::~HangarWindow() |
| 3067 | `0x147d9c` | `0x1489c0` | +3108 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN12HangarWindowD2Ev`<br>HangarWindow::~HangarWindow() |
| 3068 | `0x147e24` | `0x148a48` | +3108 | 3052 | 3052 | 0 | 100.0% |  | Y | `_ZN12HangarWindow10initializeEv`<br>HangarWindow::initialize() |
| 3069 | `0x148a10` | `0x149634` | +3108 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN12HangarWindow27refreshCurrentContentHeightEv`<br>HangarWindow::refreshCurrentContentHeight() |
| 3070 | `0x148a40` | `0x149664` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12HangarWindow13isInitializedEv`<br>HangarWindow::isInitialized() |
| 3071 | `0x148a44` | `0x149668` | +3108 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN12HangarWindow13getCurrentTabEv`<br>HangarWindow::getCurrentTab() |
| 3072 | `0x148a4a` | `0x14966e` | +3108 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN12HangarWindow8listModeEv`<br>HangarWindow::listMode() |
| 3073 | `0x148a64` | `0x149688` | +3108 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN12HangarWindow15isInSpecialModeEv`<br>HangarWindow::isInSpecialMode() |
| 3074 | `0x148a7a` | `0x14969e` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12HangarWindow14getCurrentItemEv`<br>HangarWindow::getCurrentItem() |
| 3075 | `0x148a7e` | `0x1496a2` | +3108 | 488 | 488 | 0 | 100.0% |  | Y | `_ZN12HangarWindow6updateEi`<br>HangarWindow::update(int) |
| 3076 | `0x148c68` | `0x14988c` | +3108 | 932 | 932 | 0 | 100.0% |  | Y | `_ZN12HangarWindow11transactionEb`<br>HangarWindow::transaction(bool) |
| 3077 | `0x14900c` | `0x149c30` | +3108 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN12HangarWindow37refreshCargoAvailabilityForBlueprintsEv`<br>HangarWindow::refreshCargoAvailabilityForBlueprints() |
| 3078 | `0x1490c8` | `0x149cec` | +3108 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN12HangarWindow8render3DEv`<br>HangarWindow::render3D() |
| 3079 | `0x1490d8` | `0x149cfc` | +3108 | 9700 | 9700 | 0 | 77.0% |  |  | `_ZN12HangarWindow6renderEv`<br>HangarWindow::render() |
| 3080 | `0x14b6bc` | `0x14c2e0` | +3108 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN12HangarWindow25getRelativeScrollStartPosEv`<br>HangarWindow::getRelativeScrollStartPos() |
| 3081 | `0x14b6ec` | `0x14c310` | +3108 | 84 | 84 | 0 | 100.0% | Y | Y | `_ZN12HangarWindow23getRelativeScrollHeightEv`<br>HangarWindow::getRelativeScrollHeight() |
| 3082 | `0x14b740` | `0x14c364` | +3108 | 1196 | 1196 | 0 | 100.0% |  | Y | `_ZN12HangarWindow12OnTouchBeginEii`<br>HangarWindow::OnTouchBegin(int, int) |
| 3083 | `0x14bbec` | `0x14c810` | +3108 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN12HangarWindow13highlightItemEP8ListItem`<br>HangarWindow::highlightItem(ListItem*) |
| 3084 | `0x14bc50` | `0x14c874` | +3108 | 584 | 584 | 0 | 100.0% |  | Y | `_ZN12HangarWindow25autoEquipSecondaryWeaponsEi`<br>HangarWindow::autoEquipSecondaryWeapons(int) |
| 3085 | `0x14be98` | `0x14cabc` | +3108 | 344 | 344 | 0 | 100.0% |  | Y | `_ZN12HangarWindow11OnTouchMoveEii`<br>HangarWindow::OnTouchMove(int, int) |
| 3086 | `0x14bff0` | `0x14cc14` | +3108 | 1872 | 1872 | 0 | 100.0% |  | Y | `_ZN12HangarWindow11setSellModeEb`<br>HangarWindow::setSellMode(bool) |
| 3087 | `0x14c740` | `0x14d364` | +3108 | 5828 | 5828 | 0 | 100.0% |  | Y | `_ZN12HangarWindow10OnTouchEndEii`<br>HangarWindow::OnTouchEnd(int, int) |
| 3088 | `0x14de04` | `0x14ea28` | +3108 | 404 | 404 | 0 | 100.0% |  | Y | `_ZN12HangarWindow11demountItemEP4Itemi`<br>HangarWindow::demountItem(Item*, int) |
| 3089 | `0x14df98` | `0x14ebbc` | +3108 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN12HangarWindow9mountItemEP4Item`<br>HangarWindow::mountItem(Item*) |
| 3090 | `0x14e118` | `0x14ed3c` | +3108 | 532 | 532 | 0 | 100.0% |  | Y | `_ZN12HangarWindow20showCreditsBuyWindowEv`<br>HangarWindow::showCreditsBuyWindow() |
| 3091 | `0x14e32c` | `0x14ef50` | +3108 | 452 | 452 | 0 | 100.0% |  | Y | `_ZN12HangarWindow21showFreeCreditsWindowEv`<br>HangarWindow::showFreeCreditsWindow() |
| 3092 | `0x14e4f0` | `0x14f114` | +3108 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN12HangarWindow12readyToCloseEv`<br>HangarWindow::readyToClose() |
| 3093 | `0x14e550` | `0x14f174` | +3108 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN12HangarWindow24currentItemIsHighlightedEv`<br>HangarWindow::currentItemIsHighlighted() |
| 3094 | `0x14e570` | `0x14f194` | +3108 | 1872 | 1872 | 0 | 100.0% |  | Y | `_ZN12HangarWindow10selectItemEP8ListItem`<br>HangarWindow::selectItem(ListItem*) |
| 3095 | `0x14ecc0` | `0x14f8e4` | +3108 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12HangarWindow11hideMessageEv`<br>HangarWindow::hideMessage() |
| 3096 | `0x14ecc8` | `0x14f8ec` | +3108 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN11HackingGameC1Eiiiii`<br>HackingGame::HackingGame(int, int, int, int, int) |
| 3097 | `0x14ecc8` | `0x14f8ec` | +3108 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN11HackingGameC2Eiiiii`<br>HackingGame::HackingGame(int, int, int, int, int) |
| 3098 | `0x14ed9c` | `0x14f9c0` | +3108 | 420 | 420 | 0 | 100.0% |  | Y | `_ZN11HackingGame6reInitEv`<br>HackingGame::reInit() |
| 3099 | `0x14ef40` | `0x14fb64` | +3108 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11HackingGameD2Ev`<br>HackingGame::~HackingGame() |
| 3100 | `0x14ef40` | `0x14fb64` | +3108 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11HackingGameD1Ev`<br>HackingGame::~HackingGame() |
| 3101 | `0x14ef42` | `0x14fb66` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11HackingGame13getRewardItemEv`<br>HackingGame::getRewardItem() |
| 3102 | `0x14ef48` | `0x14fb6c` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11HackingGame15getRewardAmountEv`<br>HackingGame::getRewardAmount() |
| 3103 | `0x14ef50` | `0x14fb74` | +3108 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN11HackingGame13rotateRightCWEb`<br>HackingGame::rotateRightCW(bool) |
| 3104 | `0x14ef8c` | `0x14fbb0` | +3108 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN11HackingGame12rotateLeftCWEb`<br>HackingGame::rotateLeftCW(bool) |
| 3105 | `0x14efc8` | `0x14fbec` | +3108 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN11HackingGame16solvableInNStepsEiiiiPi`<br>HackingGame::solvableInNSteps(int, int, int, int, int*) |
| 3106 | `0x14f0a4` | `0x14fcc8` | +3108 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11HackingGame7gameWonEPi`<br>HackingGame::gameWon(int*) |
| 3107 | `0x14f0c6` | `0x14fcea` | +3108 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11HackingGame12rotateLeftCWEPi`<br>HackingGame::rotateLeftCW(int*) |
| 3108 | `0x14f0e8` | `0x14fd0c` | +3108 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11HackingGame13rotateRightCWEPi`<br>HackingGame::rotateRightCW(int*) |
| 3109 | `0x14f10a` | `0x14fd2e` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11HackingGame15getDockingIndexEv`<br>HackingGame::getDockingIndex() |
| 3110 | `0x14f110` | `0x14fd34` | +3108 | 40 | 40 | 0 | 100.0% | Y | Y | `_ZN11HackingGame7gameWonEv`<br>HackingGame::gameWon() |
| 3111 | `0x14f138` | `0x14fd5c` | +3108 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN11HackingGame6updateEi`<br>HackingGame::update(int) |
| 3112 | `0x14f1cc` | `0x14fdf0` | +3108 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN11HackingGame10isRotatingEv`<br>HackingGame::isRotating() |
| 3113 | `0x14f1e4` | `0x14fe08` | +3108 | 2090 | 2090 | 0 | 100.0% |  | Y | `_ZN11HackingGame8render2DEv`<br>HackingGame::render2D() |
| 3114 | `0x14fa10` | `0x150634` | +3108 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN14MissionsWindowC1Ev`<br>MissionsWindow::MissionsWindow() |
| 3115 | `0x14fa10` | `0x150634` | +3108 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN14MissionsWindowC2Ev`<br>MissionsWindow::MissionsWindow() |
| 3116 | `0x14fa68` | `0x15068c` | +3108 | 2468 | 2468 | 0 | 100.0% |  | Y | `_ZN14MissionsWindow4initEv`<br>MissionsWindow::init() |
| 3117 | `0x15040c` | `0x151030` | +3108 | 166 | 166 | 0 | 100.0% |  | Y | `_ZN14MissionsWindowD1Ev`<br>MissionsWindow::~MissionsWindow() |
| 3118 | `0x15040c` | `0x151030` | +3108 | 166 | 166 | 0 | 100.0% |  | Y | `_ZN14MissionsWindowD2Ev`<br>MissionsWindow::~MissionsWindow() |
| 3119 | `0x1504b2` | `0x1510d6` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14MissionsWindow15setHangarUpdateEb`<br>MissionsWindow::setHangarUpdate(bool) |
| 3120 | `0x1504b8` | `0x1510dc` | +3108 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN14MissionsWindow8render3DEv`<br>MissionsWindow::render3D() |
| 3121 | `0x1504dc` | `0x151100` | +3108 | 1228 | 1228 | 0 | 100.0% |  | Y | `_ZN14MissionsWindow4drawEv`<br>MissionsWindow::draw() |
| 3122 | `0x1509a8` | `0x1515cc` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14MissionsWindow17hangarNeedsUpdateEv`<br>MissionsWindow::hangarNeedsUpdate() |
| 3123 | `0x1509b0` | `0x1515d4` | +3108 | 556 | 556 | 0 | 100.0% |  | Y | `_ZN14MissionsWindow6updateEi`<br>MissionsWindow::update(int) |
| 3124 | `0x150bdc` | `0x151800` | +3108 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN14MissionsWindow12OnTouchBeginEii`<br>MissionsWindow::OnTouchBegin(int, int) |
| 3125 | `0x150ca8` | `0x1518cc` | +3108 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN14MissionsWindow11OnTouchMoveEii`<br>MissionsWindow::OnTouchMove(int, int) |
| 3126 | `0x150d74` | `0x151998` | +3108 | 1088 | 1088 | 0 | 100.0% |  | Y | `_ZN14MissionsWindow10OnTouchEndEii`<br>MissionsWindow::OnTouchEnd(int, int) |
| 3127 | `0x1511b4` | `0x151dd8` | +3108 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11TractorBeamC1EP10AEGeometryi`<br>TractorBeam::TractorBeam(AEGeometry*, int) |
| 3128 | `0x1511b4` | `0x151dd8` | +3108 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN11TractorBeamC2EP10AEGeometryi`<br>TractorBeam::TractorBeam(AEGeometry*, int) |
| 3129 | `0x151210` | `0x151e34` | +3108 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11TractorBeamD2Ev`<br>TractorBeam::~TractorBeam() |
| 3130 | `0x151210` | `0x151e34` | +3108 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN11TractorBeamD1Ev`<br>TractorBeam::~TractorBeam() |
| 3131 | `0x15122c` | `0x151e50` | +3108 | 852 | 852 | 0 | 100.0% |  | Y | `_ZN11TractorBeam6updateEiP5RadarP5LevelP3Hud`<br>TractorBeam::update(int, Radar*, Level*, Hud*) |
| 3132 | `0x151580` | `0x1521a4` | +3108 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11TractorBeam6renderEv`<br>TractorBeam::render() |
| 3133 | `0x15158c` | `0x1521b0` | +3108 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN12RadioMessageC2Eiiii`<br>RadioMessage::RadioMessage(int, int, int, int) |
| 3134 | `0x15158c` | `0x1521b0` | +3108 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN12RadioMessageC1Eiiii`<br>RadioMessage::RadioMessage(int, int, int, int) |
| 3135 | `0x1515bc` | `0x1521e0` | +3108 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage5resetEv`<br>RadioMessage::reset() |
| 3136 | `0x1515c4` | `0x1521e8` | +3108 | 102 | 102 | 0 | 100.0% |  | Y | `_ZN12RadioMessageC1Eiiiii`<br>RadioMessage::RadioMessage(int, int, int, int, int) |
| 3137 | `0x1515c4` | `0x1521e8` | +3108 | 102 | 102 | 0 | 100.0% |  | Y | `_ZN12RadioMessageC2Eiiiii`<br>RadioMessage::RadioMessage(int, int, int, int, int) |
| 3138 | `0x15162a` | `0x15224e` | +3108 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN12RadioMessageC2EiiP9Objective`<br>RadioMessage::RadioMessage(int, int, Objective*) |
| 3139 | `0x15162a` | `0x15224e` | +3108 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN12RadioMessageC1EiiP9Objective`<br>RadioMessage::RadioMessage(int, int, Objective*) |
| 3140 | `0x151650` | `0x152274` | +3108 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN12RadioMessageD1Ev`<br>RadioMessage::~RadioMessage() |
| 3141 | `0x151650` | `0x152274` | +3108 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN12RadioMessageD2Ev`<br>RadioMessage::~RadioMessage() |
| 3142 | `0x151666` | `0x15228a` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage10getSoundIDEv`<br>RadioMessage::getSoundID() |
| 3143 | `0x15166a` | `0x15228e` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage9getTextIDEv`<br>RadioMessage::getTextID() |
| 3144 | `0x15166e` | `0x152292` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage10getImageIDEv`<br>RadioMessage::getImageID() |
| 3145 | `0x151672` | `0x152296` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage8setRadioEP5Radio`<br>RadioMessage::setRadio(Radio*) |
| 3146 | `0x151676` | `0x15229a` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage11isTriggeredEv`<br>RadioMessage::isTriggered() |
| 3147 | `0x15167c` | `0x1522a0` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage6isOverEv`<br>RadioMessage::isOver() |
| 3148 | `0x151682` | `0x1522a6` | +3108 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage6finishEv`<br>RadioMessage::finish() |
| 3149 | `0x15168a` | `0x1522ae` | +3108 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12RadioMessage7triggerEv`<br>RadioMessage::trigger() |
| 3150 | `0x151694` | `0x1522b8` | +3108 | 1684 | 1684 | 0 | 100.0% |  | Y | `_ZN12RadioMessage9triggeredExP9PlayerEgoP11LevelScript`<br>RadioMessage::triggered(long long, PlayerEgo*, LevelScript*) |
| 3151 | `0x151d28` | `0x15294c` | +3108 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN14BoundingSphereC1Efffffff`<br>BoundingSphere::BoundingSphere(float, float, float, float, float, float, float) |
| 3152 | `0x151d28` | `0x15294c` | +3108 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN14BoundingSphereC2Efffffff`<br>BoundingSphere::BoundingSphere(float, float, float, float, float, float, float) |
| 3153 | `0x151d64` | `0x152988` | +3108 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14BoundingSphereD2Ev`<br>BoundingSphere::~BoundingSphere() |
| 3154 | `0x151d64` | `0x152988` | +3108 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14BoundingSphereD1Ev`<br>BoundingSphere::~BoundingSphere() |
| 3155 | `0x151d68` | `0x15298c` | +3108 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN14BoundingSphere7collideEfff`<br>BoundingSphere::collide(float, float, float) |
| 3156 | `0x151da0` | `0x1529c4` | +3108 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN14BoundingSphere12outerCollideEfff`<br>BoundingSphere::outerCollide(float, float, float) |
| 3157 | `0x151e10` | `0x152a34` | +3108 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN14BoundingSphere18getCollisionNormalERKN11AbyssEngine6AEMath6VectorE`<br>BoundingSphere::getCollisionNormal(AbyssEngine::AEMath::Vector const&) |
| 3158 | `0x151e70` | `0x152a94` | +3108 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN14BoundingSphere6updateEfff`<br>BoundingSphere::update(float, float, float) |
| 3159 | `0x151e74` | `0x152a98` | +3108 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN14BoundingSphere25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE`<br>BoundingSphere::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) |
| 3160 | `0x151f20` | `0x152b44` | +3108 | 532 | 532 | 0 | 100.0% |  | Y | `_ZN3GunC1EiiiiiifN11AbyssEngine6AEMath6VectorES2_`<br>Gun::Gun(int, int, int, int, int, int, float, AbyssEngine::AEMath::Vector, AbyssEngine::AEMath::Vector) |
| 3161 | `0x151f20` | `0x152b44` | +3108 | 532 | 532 | 0 | 100.0% |  | Y | `_ZN3GunC2EiiiiiifN11AbyssEngine6AEMath6VectorES2_`<br>Gun::Gun(int, int, int, int, int, int, float, AbyssEngine::AEMath::Vector, AbyssEngine::AEMath::Vector) |
| 3162 | `0x152134` | `0x152d58` | +3108 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN3GunD2Ev`<br>Gun::~Gun() |
| 3163 | `0x152134` | `0x152d58` | +3108 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN3GunD1Ev`<br>Gun::~Gun() |
| 3164 | `0x1521ac` | `0x152dd0` | +3108 | 272 | 272 | 0 | 100.0% |  | Y | `_ZN3Gun8setIndexEi`<br>Gun::setIndex(int) |
| 3165 | `0x1522bc` | `0x152ee0` | +3108 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN3Gun27setErrorMagnitudePercentageEi`<br>Gun::setErrorMagnitudePercentage(int) |
| 3166 | `0x1522ca` | `0x152eee` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun12setFriendGunEb`<br>Gun::setFriendGun(bool) |
| 3167 | `0x1522d0` | `0x152ef4` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun12setMagnitudeEi`<br>Gun::setMagnitude(int) |
| 3168 | `0x1522d6` | `0x152efa` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun12getMagnitudeEv`<br>Gun::getMagnitude() |
| 3169 | `0x1522dc` | `0x152f00` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun17setLevelCollisionEb`<br>Gun::setLevelCollision(bool) |
| 3170 | `0x1522e4` | `0x152f08` | +3108 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN3Gun9setOffsetEPN11AbyssEngine6AEMath6VectorE`<br>Gun::setOffset(AbyssEngine::AEMath::Vector*) |
| 3171 | `0x152328` | `0x152f4c` | +3108 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN3Gun9setOffsetEii`<br>Gun::setOffset(int, int) |
| 3172 | `0x1523b0` | `0x152fd4` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun9setImpactEP6Sparks`<br>Gun::setImpact(Sparks*) |
| 3173 | `0x1523b6` | `0x152fda` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun10setEnemiesEP5ArrayIP6PlayerE`<br>Gun::setEnemies(Array<Player*>*) |
| 3174 | `0x1523bc` | `0x152fe0` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun10getEnemiesEv`<br>Gun::getEnemies() |
| 3175 | `0x1523c2` | `0x152fe6` | +3108 | 78 | 78 | 0 | 100.0% |  | Y | `_ZN3Gun8setEnemyEP6Player`<br>Gun::setEnemy(Player*) |
| 3176 | `0x152410` | `0x153034` | +3108 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN3Gun16removeAllEnemiesEv`<br>Gun::removeAllEnemies() |
| 3177 | `0x152418` | `0x15303c` | +3108 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN3Gun8setLevelEP5Level`<br>Gun::setLevel(Level*) |
| 3178 | `0x15241c` | `0x153040` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun12setPlayerGunEb`<br>Gun::setPlayerGun(bool) |
| 3179 | `0x152422` | `0x153046` | +3108 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Gun11isPlayerGunEv`<br>Gun::isPlayerGun() |
| 3180 | `0x152428` | `0x15304c` | +3108 | 1788 | 1788 | 0 | 100.0% |  | Y | `_ZN3Gun7shootAtEN11AbyssEngine6AEMath6MatrixEiP6Playerb`<br>Gun::shootAt(AbyssEngine::AEMath::Matrix, int, Player*, bool) |
| 3181 | `0x152b24` | `0x153748` | +3108 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN3Gun5shootEN11AbyssEngine6AEMath6MatrixEib`<br>Gun::shoot(AbyssEngine::AEMath::Matrix, int, bool) |
| 3182 | `0x152b7c` | `0x1537a0` | +3108 | 1044 | 1044 | 0 | 100.0% |  | Y | `_ZN3Gun6igniteEv`<br>Gun::ignite() |
| 3183 | `0x152f90` | `0x153bb4` | +3108 | 1984 | 1964 | -20 | 68.9% |  |  | `_ZN3Gun22calcCharacterCollisionEv`<br>Gun::calcCharacterCollision() |
| 3184 | `0x153750` | `0x154360` | +3088 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN3Gun18calcLevelCollisionEv`<br>Gun::calcLevelCollision() |
| 3185 | `0x153754` | `0x154364` | +3088 | 520 | 520 | 0 | 100.0% |  | Y | `_ZN3Gun6updateEi`<br>Gun::update(int) |
| 3186 | `0x15395c` | `0x15456c` | +3088 | 244 | 244 | 0 | 100.0% |  | Y | `_ZN3Gun6renderEv`<br>Gun::render() |
| 3187 | `0x153a50` | `0x154660` | +3088 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN3Gun9translateERKN11AbyssEngine6AEMath6VectorE`<br>Gun::translate(AbyssEngine::AEMath::Vector const&) |
| 3188 | `0x153a7c` | `0x15468c` | +3088 | 756 | 756 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObjectC1EiiP6PlayerP10AEGeometryfff`<br>PlayerFixedObject::PlayerFixedObject(int, int, Player*, AEGeometry*, float, float, float) |
| 3189 | `0x153a7c` | `0x15468c` | +3088 | 756 | 756 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObjectC2EiiP6PlayerP10AEGeometryfff`<br>PlayerFixedObject::PlayerFixedObject(int, int, Player*, AEGeometry*, float, float, float) |
| 3190 | `0x153d70` | `0x154980` | +3088 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObjectD2Ev`<br>PlayerFixedObject::~PlayerFixedObject() |
| 3191 | `0x153d70` | `0x154980` | +3088 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObjectD1Ev`<br>PlayerFixedObject::~PlayerFixedObject() |
| 3192 | `0x153e14` | `0x154a24` | +3088 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObjectD0Ev`<br>PlayerFixedObject::~PlayerFixedObject() |
| 3193 | `0x153e24` | `0x154a34` | +3088 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject5resetEv`<br>PlayerFixedObject::reset() |
| 3194 | `0x153ee4` | `0x154af4` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject9setMovingEb`<br>PlayerFixedObject::setMoving(bool) |
| 3195 | `0x153eec` | `0x154afc` | +3088 | 200 | 200 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject11setPositionEfff`<br>PlayerFixedObject::setPosition(float, float, float) |
| 3196 | `0x153fb4` | `0x154bc4` | +3088 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>PlayerFixedObject::setPosition(AbyssEngine::AEMath::Vector const&) |
| 3197 | `0x153fd0` | `0x154be0` | +3088 | 70 | 70 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject9translateERKN11AbyssEngine6AEMath6VectorE`<br>PlayerFixedObject::translate(AbyssEngine::AEMath::Vector const&) |
| 3198 | `0x154016` | `0x154c26` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject5setBVEP5ArrayIP14BoundingVolumeE`<br>PlayerFixedObject::setBV(Array<BoundingVolume*>*) |
| 3199 | `0x15401c` | `0x154c2c` | +3088 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject5setBVEP14BoundingVolume`<br>PlayerFixedObject::setBV(BoundingVolume*) |
| 3200 | `0x154056` | `0x154c66` | +3088 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject11getPositionEv`<br>PlayerFixedObject::getPosition() |
| 3201 | `0x15407c` | `0x154c8c` | +3088 | 184 | 184 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject16setWreckedMeshIdEi`<br>PlayerFixedObject::setWreckedMeshId(int) |
| 3202 | `0x154134` | `0x154d44` | +3088 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject11moveForwardEi`<br>PlayerFixedObject::moveForward(int) |
| 3203 | `0x1541e0` | `0x154df0` | +3088 | 2768 | 2768 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject6updateEi`<br>PlayerFixedObject::update(int) |
| 3204 | `0x154cb0` | `0x1558c0` | +3088 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject17setExhaustVisibleEb`<br>PlayerFixedObject::setExhaustVisible(bool) |
| 3205 | `0x154cdc` | `0x1558ec` | +3088 | 110 | 110 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject6renderEv`<br>PlayerFixedObject::render() |
| 3206 | `0x154d4a` | `0x15595a` | +3088 | 140 | 140 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject7collideEfff`<br>PlayerFixedObject::collide(float, float, float) |
| 3207 | `0x154dd6` | `0x1559e6` | +3088 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject19getProjectionVectorERKN11AbyssEngine6AEMath6VectorE`<br>PlayerFixedObject::getProjectionVector(AbyssEngine::AEMath::Vector const&) |
| 3208 | `0x154e16` | `0x155a26` | +3088 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE`<br>PlayerFixedObject::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) |
| 3209 | `0x154e50` | `0x155a60` | +3088 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject12outerCollideEN11AbyssEngine6AEMath6VectorE`<br>PlayerFixedObject::outerCollide(AbyssEngine::AEMath::Vector) |
| 3210 | `0x154e5a` | `0x155a6a` | +3088 | 144 | 144 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject12outerCollideEfff`<br>PlayerFixedObject::outerCollide(float, float, float) |
| 3211 | `0x154eea` | `0x155afa` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject14setDockingTypeEi`<br>PlayerFixedObject::setDockingType(int) |
| 3212 | `0x154ef0` | `0x155b00` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject14getDockingTypeEv`<br>PlayerFixedObject::getDockingType() |
| 3213 | `0x154ef6` | `0x155b06` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject14setTransportIDEi`<br>PlayerFixedObject::setTransportID(int) |
| 3214 | `0x154efc` | `0x155b0c` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject14getTransportIDEv`<br>PlayerFixedObject::getTransportID() |
| 3215 | `0x154f02` | `0x155b12` | +3088 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject7getNameEv`<br>PlayerFixedObject::getName() |
| 3216 | `0x154f12` | `0x155b22` | +3088 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE`<br>PlayerFixedObject::setName(AbyssEngine::String) |
| 3217 | `0x154f1a` | `0x155b2a` | +3088 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN17PlayerFixedObject8hideShipEv`<br>PlayerFixedObject::hideShip() |
| 3218 | `0x154f24` | `0x155b34` | +3088 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN17PlayerFixedObject20setDeadButSelectableEv`<br>PlayerFixedObject::setDeadButSelectable() |
| 3219 | `0x154f7c` | `0x155b8c` | +3088 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN5RadioC2Ev`<br>Radio::Radio() |
| 3220 | `0x154f7c` | `0x155b8c` | +3088 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN5RadioC1Ev`<br>Radio::Radio() |
| 3221 | `0x154fd4` | `0x155be4` | +3088 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN5RadioD2Ev`<br>Radio::~Radio() |
| 3222 | `0x154fd4` | `0x155be4` | +3088 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN5RadioD1Ev`<br>Radio::~Radio() |
| 3223 | `0x15501c` | `0x155c2c` | +3088 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN5Radio11setMessagesEP5ArrayIP12RadioMessageE`<br>Radio::setMessages(Array<RadioMessage*>*) |
| 3224 | `0x15504c` | `0x155c5c` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Radio17setCurrentMessageEP12RadioMessage`<br>Radio::setCurrentMessage(RadioMessage*) |
| 3225 | `0x155050` | `0x155c60` | +3088 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN5Radio10getMessageEi`<br>Radio::getMessage(int) |
| 3226 | `0x15505a` | `0x155c6a` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Radio16lastMessageShownEv`<br>Radio::lastMessageShown() |
| 3227 | `0x155060` | `0x155c70` | +3088 | 792 | 792 | 0 | 100.0% |  | Y | `_ZN5Radio6updateElP9PlayerEgoP11LevelScript`<br>Radio::update(long, PlayerEgo*, LevelScript*) |
| 3228 | `0x155378` | `0x155f88` | +3088 | 600 | 600 | 0 | 100.0% |  | Y | `_ZN5Radio4drawExP9PlayerEgoP11LevelScript`<br>Radio::draw(long long, PlayerEgo*, LevelScript*) |
| 3229 | `0x1555d0` | `0x1561e0` | +3088 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN5Radio16isShowingMessageEv`<br>Radio::isShowingMessage() |
| 3230 | `0x1555dc` | `0x1561ec` | +3088 | 184 | 184 | 0 | 100.0% |  | Y | `_ZN11SolarSystemC1EiN11AbyssEngine6StringEibiiiiiiPiP5ArrayIiES5_S5_`<br>SolarSystem::SolarSystem(int, AbyssEngine::String, int, bool, int, int, int, int, int, int, int*, Array<int>*, Array<int>*, Array<int>*) |
| 3231 | `0x1555dc` | `0x1561ec` | +3088 | 184 | 184 | 0 | 100.0% |  | Y | `_ZN11SolarSystemC2EiN11AbyssEngine6StringEibiiiiiiPiP5ArrayIiES5_S5_`<br>SolarSystem::SolarSystem(int, AbyssEngine::String, int, bool, int, int, int, int, int, int, int*, Array<int>*, Array<int>*, Array<int>*) |
| 3232 | `0x155694` | `0x1562a4` | +3088 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN11SolarSystemD2Ev`<br>SolarSystem::~SolarSystem() |
| 3233 | `0x155694` | `0x1562a4` | +3088 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN11SolarSystemD1Ev`<br>SolarSystem::~SolarSystem() |
| 3234 | `0x1556ec` | `0x1562fc` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem8getIndexEv`<br>SolarSystem::getIndex() |
| 3235 | `0x1556f0` | `0x156300` | +3088 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11SolarSystem7getNameEv`<br>SolarSystem::getName() |
| 3236 | `0x1556fe` | `0x15630e` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem16getSecurityLevelEv`<br>SolarSystem::getSecurityLevel() |
| 3237 | `0x155702` | `0x156312` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem7getRaceEv`<br>SolarSystem::getRace() |
| 3238 | `0x155708` | `0x156318` | +3088 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11SolarSystem13getAttackRaceEv`<br>SolarSystem::getAttackRace() |
| 3239 | `0x155720` | `0x156330` | +3088 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN11SolarSystem13hasPirateBaseEv`<br>SolarSystem::hasPirateBase() |
| 3240 | `0x15576c` | `0x15637c` | +3088 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem17stationIsInSystemEi`<br>SolarSystem::stationIsInSystem(int) |
| 3241 | `0x155790` | `0x1563a0` | +3088 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN11SolarSystem18hasHiddenBlueprintEv`<br>SolarSystem::hasHiddenBlueprint() |
| 3242 | `0x1557dc` | `0x1563ec` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem4getXEv`<br>SolarSystem::getX() |
| 3243 | `0x1557e0` | `0x1563f0` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem4getYEv`<br>SolarSystem::getY() |
| 3244 | `0x1557e4` | `0x1563f4` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem4getZEv`<br>SolarSystem::getZ() |
| 3245 | `0x1557e8` | `0x1563f8` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem15getTextureIndexEv`<br>SolarSystem::getTextureIndex() |
| 3246 | `0x1557ec` | `0x1563fc` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem16getWarpGateIndexEv`<br>SolarSystem::getWarpGateIndex() |
| 3247 | `0x1557f0` | `0x156400` | +3088 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11SolarSystem20getWarpGateEnumIndexEv`<br>SolarSystem::getWarpGateEnumIndex() |
| 3248 | `0x1557f6` | `0x156406` | +3088 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem19getStationEnumIndexEi`<br>SolarSystem::getStationEnumIndex(int) |
| 3249 | `0x155818` | `0x156428` | +3088 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11SolarSystem23currentOrbitHasWarpGateEv`<br>SolarSystem::currentOrbitHasWarpGate() |
| 3250 | `0x15583c` | `0x15644c` | +3088 | 30 | 30 | 0 | 100.0% |  | Y | `_ZN11SolarSystem17stationIsInSystemEP7Station`<br>SolarSystem::stationIsInSystem(Station*) |
| 3251 | `0x15585c` | `0x15646c` | +3088 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN11SolarSystem17isFullyDiscoveredEv`<br>SolarSystem::isFullyDiscovered() |
| 3252 | `0x15589c` | `0x1564ac` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem9setCoordsEii`<br>SolarSystem::setCoords(int, int) |
| 3253 | `0x1558a2` | `0x1564b2` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem11getStationsEv`<br>SolarSystem::getStations() |
| 3254 | `0x1558a6` | `0x1564b6` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem17getForbiddenGoodsEv`<br>SolarSystem::getForbiddenGoods() |
| 3255 | `0x1558aa` | `0x1564ba` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem9getRoutesEv`<br>SolarSystem::getRoutes() |
| 3256 | `0x1558ae` | `0x1564be` | +3088 | 40 | 40 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem22systemIsInSystemRoutesEi`<br>SolarSystem::systemIsInSystemRoutes(int) |
| 3257 | `0x1558d6` | `0x1564e6` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem9isVisibleEv`<br>SolarSystem::isVisible() |
| 3258 | `0x1558dc` | `0x1564ec` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem10setVisibleEb`<br>SolarSystem::setVisible(bool) |
| 3259 | `0x1558e2` | `0x1564f2` | +3088 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN11SolarSystem10hasNoOwnerEv`<br>SolarSystem::hasNoOwner() |
| 3260 | `0x155900` | `0x156510` | +3088 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN10GameRecordC1Ev`<br>GameRecord::GameRecord() |
| 3261 | `0x155900` | `0x156510` | +3088 | 212 | 212 | 0 | 100.0% |  | Y | `_ZN10GameRecordC2Ev`<br>GameRecord::GameRecord() |
| 3262 | `0x1559d4` | `0x1565e4` | +3088 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10GameRecordD1Ev`<br>GameRecord::~GameRecord() |
| 3263 | `0x1559d4` | `0x1565e4` | +3088 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN10GameRecordD2Ev`<br>GameRecord::~GameRecord() |
| 3264 | `0x1559f0` | `0x156600` | +3088 | 2696 | 2696 | 0 | 100.0% |  | Y | `_ZN10GameRecord4loadEv`<br>GameRecord::load() |
| 3265 | `0x156478` | `0x157088` | +3088 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN7MineGunC2EP3GuniiiP5Level`<br>MineGun::MineGun(Gun*, int, int, int, Level*) |
| 3266 | `0x156478` | `0x157088` | +3088 | 284 | 284 | 0 | 100.0% |  | Y | `_ZN7MineGunC1EP3GuniiiP5Level`<br>MineGun::MineGun(Gun*, int, int, int, Level*) |
| 3267 | `0x156594` | `0x1571a4` | +3088 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9ExplosionEC2Ev`<br>Array<Explosion*>::Array() |
| 3268 | `0x1565b0` | `0x1571c0` | +3088 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP9ExplosionEvjR5ArrayIT_E`<br>void ArraySetLength<Explosion*>(unsigned int, Array<Explosion*>&) |
| 3269 | `0x1565e4` | `0x1571f4` | +3088 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN7MineGunD2Ev`<br>MineGun::~MineGun() |
| 3270 | `0x1565e4` | `0x1571f4` | +3088 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN7MineGunD1Ev`<br>MineGun::~MineGun() |
| 3271 | `0x156640` | `0x157250` | +3088 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP9ExplosionEvR5ArrayIT_E`<br>void ArrayReleaseClasses<Explosion*>(Array<Explosion*>&) |
| 3272 | `0x156690` | `0x1572a0` | +3088 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP9ExplosionED2Ev`<br>Array<Explosion*>::~Array() |
| 3273 | `0x1566a6` | `0x1572b6` | +3088 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN7MineGunD0Ev`<br>MineGun::~MineGun() |
| 3274 | `0x1566b6` | `0x1572c6` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7MineGun9setPlayerEP9PlayerEgo`<br>MineGun::setPlayer(PlayerEgo*) |
| 3275 | `0x1566bc` | `0x1572cc` | +3088 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN7MineGun6renderEv`<br>MineGun::render() |
| 3276 | `0x1566ec` | `0x1572fc` | +3088 | 472 | 472 | 0 | 100.0% |  | Y | `_ZN7MineGun6updateEi`<br>MineGun::update(int) |
| 3277 | `0x1568c4` | `0x1574d4` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbstractGun11isRocketGunEv`<br>AbstractGun::isRocketGun() |
| 3278 | `0x1568c8` | `0x1574d8` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11AbstractGun9isBombGunEv`<br>AbstractGun::isBombGun() |
| 3279 | `0x1568cc` | `0x1574dc` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7MineGun9isMineGunEv`<br>MineGun::isMineGun() |
| 3280 | `0x1568d0` | `0x1574e0` | +3088 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN12AchievementsC2Ev`<br>Achievements::Achievements() |
| 3281 | `0x1568d0` | `0x1574e0` | +3088 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN12AchievementsC1Ev`<br>Achievements::Achievements() |
| 3282 | `0x1568f2` | `0x157502` | +3088 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN12AchievementsD1Ev`<br>Achievements::~Achievements() |
| 3283 | `0x1568f2` | `0x157502` | +3088 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN12AchievementsD2Ev`<br>Achievements::~Achievements() |
| 3284 | `0x156914` | `0x157524` | +3088 | 1168 | 1168 | 0 | 100.0% |  | Y | `_ZN12Achievements16checkForNewMedalEP9PlayerEgo`<br>Achievements::checkForNewMedal(PlayerEgo*) |
| 3285 | `0x156da4` | `0x1579b4` | +3088 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN12Achievements28initCheckEquipmentAndWeaponsEv`<br>Achievements::initCheckEquipmentAndWeapons() |
| 3286 | `0x156e48` | `0x157a58` | +3088 | 126 | 126 | 0 | 100.0% | Y | Y | `_ZN12Achievements11countMedalsEv`<br>Achievements::countMedals() |
| 3287 | `0x156ec6` | `0x157ad6` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12Achievements12gotAllMedalsEv`<br>Achievements::gotAllMedals() |
| 3288 | `0x156ecc` | `0x157adc` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12Achievements16gotAllGoldMedalsEv`<br>Achievements::gotAllGoldMedals() |
| 3289 | `0x156ed2` | `0x157ae2` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12Achievements21gotAllSupernovaMedalsEv`<br>Achievements::gotAllSupernovaMedals() |
| 3290 | `0x156ed8` | `0x157ae8` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12Achievements9getMedalsEv`<br>Achievements::getMedals() |
| 3291 | `0x156edc` | `0x157aec` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12Achievements12getNewMedalsEv`<br>Achievements::getNewMedals() |
| 3292 | `0x156ee0` | `0x157af0` | +3088 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12Achievements8incKillsEv`<br>Achievements::incKills() |
| 3293 | `0x156ee8` | `0x157af8` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12Achievements8getKillsEv`<br>Achievements::getKills() |
| 3294 | `0x156eec` | `0x157afc` | +3088 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12Achievements14incPirateKillsEv`<br>Achievements::incPirateKills() |
| 3295 | `0x156ef4` | `0x157b04` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12Achievements14getPirateKillsEv`<br>Achievements::getPirateKills() |
| 3296 | `0x156ef8` | `0x157b08` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12Achievements16resetPirateKillsEv`<br>Achievements::resetPirateKills() |
| 3297 | `0x156efe` | `0x157b0e` | +3088 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12Achievements10incCatchesEv`<br>Achievements::incCatches() |
| 3298 | `0x156f06` | `0x157b16` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12Achievements10getCatchesEv`<br>Achievements::getCatches() |
| 3299 | `0x156f0a` | `0x157b1a` | +3088 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN12Achievements13updateCreditsEi`<br>Achievements::updateCredits(int) |
| 3300 | `0x156f14` | `0x157b24` | +3088 | 56 | 56 | 0 | 100.0% | Y | Y | `_ZN12Achievements9setMedalsEPii`<br>Achievements::setMedals(int*, int) |
| 3301 | `0x156f4c` | `0x157b5c` | +3088 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN12Achievements8setMedalEii`<br>Achievements::setMedal(int, int) |
| 3302 | `0x156f54` | `0x157b64` | +3088 | 58 | 58 | 0 | 100.0% | Y | Y | `_ZN12Achievements4initEv`<br>Achievements::init() |
| 3303 | `0x156f8e` | `0x157b9e` | +3088 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN12Achievements14resetNewMedalsEv`<br>Achievements::resetNewMedals() |
| 3304 | `0x156fb0` | `0x157bc0` | +3088 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN12Achievements14applyNewMedalsEv`<br>Achievements::applyNewMedals() |
| 3305 | `0x157008` | `0x157c18` | +3088 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN12Achievements8hasMedalEii`<br>Achievements::hasMedal(int, int) |
| 3306 | `0x157018` | `0x157c28` | +3088 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN12Achievements8getValueEii`<br>Achievements::getValue(int, int) |
| 3307 | `0x157034` | `0x157c44` | +3088 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN12Achievements12isEliteMedalEi`<br>Achievements::isEliteMedal(int) |
| 3308 | `0x157040` | `0x157c50` | +3088 | 768 | 768 | 0 | 100.0% |  | Y | `_ZN12PlayerTurretC1EiP6PlayerP10AEGeometryfff`<br>PlayerTurret::PlayerTurret(int, Player*, AEGeometry*, float, float, float) |
| 3309 | `0x157040` | `0x157c50` | +3088 | 768 | 768 | 0 | 100.0% |  | Y | `_ZN12PlayerTurretC2EiP6PlayerP10AEGeometryfff`<br>PlayerTurret::PlayerTurret(int, Player*, AEGeometry*, float, float, float) |
| 3310 | `0x157340` | `0x157f50` | +3088 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN12PlayerTurretD2Ev`<br>PlayerTurret::~PlayerTurret() |
| 3311 | `0x157340` | `0x157f50` | +3088 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN12PlayerTurretD1Ev`<br>PlayerTurret::~PlayerTurret() |
| 3312 | `0x1573ac` | `0x157fbc` | +3088 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN12PlayerTurretD0Ev`<br>PlayerTurret::~PlayerTurret() |
| 3313 | `0x1573bc` | `0x157fcc` | +3088 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret8setLevelEP5Level`<br>PlayerTurret::setLevel(Level*) |
| 3314 | `0x1573f0` | `0x158000` | +3088 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret10setScalingEf`<br>PlayerTurret::setScaling(float) |
| 3315 | `0x1573fc` | `0x15800c` | +3088 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>PlayerTurret::setPosition(AbyssEngine::AEMath::Vector const&) |
| 3316 | `0x157418` | `0x158028` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12PlayerTurret14setTurretRangeEi`<br>PlayerTurret::setTurretRange(int) |
| 3317 | `0x15741e` | `0x15802e` | +3088 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret7setHostEP8KIPlayerRKN11AbyssEngine6AEMath6VectorE`<br>PlayerTurret::setHost(KIPlayer*, AbyssEngine::AEMath::Vector const&) |
| 3318 | `0x15742c` | `0x15803c` | +3088 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12PlayerTurret7getHostEv`<br>PlayerTurret::getHost() |
| 3319 | `0x157432` | `0x158042` | +3088 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret5resetEv`<br>PlayerTurret::reset() |
| 3320 | `0x157450` | `0x158060` | +3088 | 764 | 764 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret6updateEi`<br>PlayerTurret::update(int) |
| 3321 | `0x15774c` | `0x15835c` | +3088 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret15handleSentryGunEi`<br>PlayerTurret::handleSentryGun(int) |
| 3322 | `0x157782` | `0x158392` | +3088 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret12handleTurretEi`<br>PlayerTurret::handleTurret(int) |
| 3323 | `0x1577b8` | `0x1583c8` | +3088 | 336 | 336 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret9pickEnemyEv`<br>PlayerTurret::pickEnemy() |
| 3324 | `0x157908` | `0x158518` | +3088 | 708 | 708 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret14handleRotationEiP10AEGeometryS1_`<br>PlayerTurret::handleRotation(int, AEGeometry*, AEGeometry*) |
| 3325 | `0x157bcc` | `0x1587dc` | +3088 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret6renderEv`<br>PlayerTurret::render() |
| 3326 | `0x157c00` | `0x158810` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12PlayerTurret7collideEfff`<br>PlayerTurret::collide(float, float, float) |
| 3327 | `0x157c04` | `0x158814` | +3088 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12PlayerTurret12outerCollideEfff`<br>PlayerTurret::outerCollide(float, float, float) |
| 3328 | `0x157c08` | `0x158818` | +3088 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN12PlayerTurret6reviveEv`<br>PlayerTurret::revive() |
| 3329 | `0x157c54` | `0x158864` | +3088 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN8WaypointC1EiiiP5Route`<br>Waypoint::Waypoint(int, int, int, Route*) |
| 3330 | `0x157c54` | `0x158864` | +3088 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN8WaypointC2EiiiP5Route`<br>Waypoint::Waypoint(int, int, int, Route*) |
| 3331 | `0x157d24` | `0x158934` | +3088 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN8WaypointD1Ev`<br>Waypoint::~Waypoint() |
| 3332 | `0x157d24` | `0x158934` | +3088 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN8WaypointD2Ev`<br>Waypoint::~Waypoint() |
| 3333 | `0x157d28` | `0x158938` | +3088 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN8WaypointD0Ev`<br>Waypoint::~Waypoint() |
| 3334 | `0x157d38` | `0x158948` | +3088 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN8Waypoint11getPositionEv`<br>Waypoint::getPosition() |
| 3335 | `0x157d5e` | `0x15896e` | +3088 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN8Waypoint9setActiveEb`<br>Waypoint::setActive(bool) |
| 3336 | `0x157d64` | `0x158974` | +3088 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN8Waypoint7reachedEv`<br>Waypoint::reached() |
| 3337 | `0x157d6e` | `0x15897e` | +3088 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN8Waypoint5resetEv`<br>Waypoint::reset() |
| 3338 | `0x157d80` | `0x158990` | +3088 | 708 | 708 | 0 | 100.0% |  | Y | `_ZN12StatusWindowC2Ev`<br>StatusWindow::StatusWindow() |
| 3339 | `0x157d80` | `0x158990` | +3088 | 708 | 708 | 0 | 100.0% |  | Y | `_ZN12StatusWindowC1Ev`<br>StatusWindow::StatusWindow() |
| 3340 | `0x158044` | `0x158c54` | +3088 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN12StatusWindow6reInitEv`<br>StatusWindow::reInit() |
| 3341 | `0x158164` | `0x158d86` | +3106 | 106 | 106 | 0 | 100.0% |  | Y | `_ZN12StatusWindowD2Ev`<br>StatusWindow::~StatusWindow() |
| 3342 | `0x158164` | `0x158d86` | +3106 | 106 | 106 | 0 | 100.0% |  | Y | `_ZN12StatusWindowD1Ev`<br>StatusWindow::~StatusWindow() |
| 3343 | `0x1581d0` | `0x158df0` | +3104 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN12StatusWindow25getRelativeScrollStartPosEv`<br>StatusWindow::getRelativeScrollStartPos() |
| 3344 | `0x158200` | `0x158e20` | +3104 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN12StatusWindow23getRelativeScrollHeightEv`<br>StatusWindow::getRelativeScrollHeight() |
| 3345 | `0x158250` | `0x158e70` | +3104 | 202 | 202 | 0 | 100.0% |  | Y | `_ZN12StatusWindow6updateEi`<br>StatusWindow::update(int) |
| 3346 | `0x15831c` | `0x158f3c` | +3104 | 6280 | 6280 | 0 | 100.0% |  | Y | `_ZN12StatusWindow4drawEv`<br>StatusWindow::draw() |
| 3347 | `0x159ba4` | `0x15a7c4` | +3104 | 1132 | 1132 | 0 | 100.0% |  | Y | `_ZN12StatusWindow16getMedalHintTextEi`<br>StatusWindow::getMedalHintText(int) |
| 3348 | `0x15a010` | `0x15ac30` | +3104 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN12StatusWindow12OnTouchBeginEii`<br>StatusWindow::OnTouchBegin(int, int) |
| 3349 | `0x15a0bc` | `0x15acdc` | +3104 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN12StatusWindow11OnTouchMoveEii`<br>StatusWindow::OnTouchMove(int, int) |
| 3350 | `0x15a1c4` | `0x15ade4` | +3104 | 768 | 768 | 0 | 100.0% |  | Y | `_ZN12StatusWindow10OnTouchEndEii`<br>StatusWindow::OnTouchEnd(int, int) |
| 3351 | `0x15a4c8` | `0x15b0e8` | +3104 | 444 | 444 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCameraC1EjP10AEGeometryN11AbyssEngine6AEMath6VectorES4_`<br>TargetFollowCamera::TargetFollowCamera(unsigned int, AEGeometry*, AbyssEngine::AEMath::Vector, AbyssEngine::AEMath::Vector) |
| 3352 | `0x15a4c8` | `0x15b0e8` | +3104 | 444 | 444 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCameraC2EjP10AEGeometryN11AbyssEngine6AEMath6VectorES4_`<br>TargetFollowCamera::TargetFollowCamera(unsigned int, AEGeometry*, AbyssEngine::AEMath::Vector, AbyssEngine::AEMath::Vector) |
| 3353 | `0x15a688` | `0x15b2a8` | +3104 | 784 | 784 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera55aproximateCooefficientsForAproximationOfDampingFunktionEfRdS0_S0_S0_S0_`<br>TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(float, double&, double&, double&, double&, double&) |
| 3354 | `0x15a998` | `0x15b5b8` | +3104 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCameraD2Ev`<br>TargetFollowCamera::~TargetFollowCamera() |
| 3355 | `0x15a998` | `0x15b5b8` | +3104 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCameraD1Ev`<br>TargetFollowCamera::~TargetFollowCamera() |
| 3356 | `0x15a99a` | `0x15b5ba` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera9setActiveEb`<br>TargetFollowCamera::setActive(bool) |
| 3357 | `0x15a9a0` | `0x15b5c0` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera9setTargetEP10AEGeometry`<br>TargetFollowCamera::setTarget(AEGeometry*) |
| 3358 | `0x15a9a4` | `0x15b5c4` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera9getTargetEv`<br>TargetFollowCamera::getTarget() |
| 3359 | `0x15a9a8` | `0x15b5c8` | +3104 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera15setTargetOffsetERKN11AbyssEngine6AEMath6VectorE`<br>TargetFollowCamera::setTargetOffset(AbyssEngine::AEMath::Vector const&) |
| 3360 | `0x15a9ae` | `0x15b5ce` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera15getTargetOffsetEv`<br>TargetFollowCamera::getTargetOffset() |
| 3361 | `0x15a9b2` | `0x15b5d2` | +3104 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera12setCamOffsetERKN11AbyssEngine6AEMath6VectorE`<br>TargetFollowCamera::setCamOffset(AbyssEngine::AEMath::Vector const&) |
| 3362 | `0x15a9cc` | `0x15b5ec` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera12getCamOffsetEv`<br>TargetFollowCamera::getCamOffset() |
| 3363 | `0x15a9d0` | `0x15b5f0` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera11setPositionEfff`<br>TargetFollowCamera::setPosition(float, float, float) |
| 3364 | `0x15a9d6` | `0x15b5f6` | +3104 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>TargetFollowCamera::setPosition(AbyssEngine::AEMath::Vector const&) |
| 3365 | `0x15a9e4` | `0x15b604` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera11getPositionEv`<br>TargetFollowCamera::getPosition() |
| 3366 | `0x15a9e8` | `0x15b608` | +3104 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera9translateEfff`<br>TargetFollowCamera::translate(float, float, float) |
| 3367 | `0x15aa48` | `0x15b668` | +3104 | 2104 | 2104 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera6updateEi`<br>TargetFollowCamera::update(int) |
| 3368 | `0x15b280` | `0x15bea0` | +3104 | 86 | 86 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera17translateNoUpdateEfff`<br>TargetFollowCamera::translateNoUpdate(float, float, float) |
| 3369 | `0x15b2d8` | `0x15bef8` | +3104 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera20enableFirstPersonCamEb`<br>TargetFollowCamera::enableFirstPersonCam(bool) |
| 3370 | `0x15b32c` | `0x15bf4c` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera25hideShipForFirstPersonCamEv`<br>TargetFollowCamera::hideShipForFirstPersonCam() |
| 3371 | `0x15b332` | `0x15bf52` | +3104 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera20setFirstPersonMatrixERN11AbyssEngine6AEMath6MatrixE`<br>TargetFollowCamera::setFirstPersonMatrix(AbyssEngine::AEMath::Matrix&) |
| 3372 | `0x15b338` | `0x15bf58` | +3104 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera18setBoostPercentageEfi`<br>TargetFollowCamera::setBoostPercentage(float, int) |
| 3373 | `0x15b34a` | `0x15bf6a` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera19setRumblePercentageEfi`<br>TargetFollowCamera::setRumblePercentage(float, int) |
| 3374 | `0x15b350` | `0x15bf70` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera8setFixedEb`<br>TargetFollowCamera::setFixed(bool) |
| 3375 | `0x15b358` | `0x15bf78` | +3104 | 124 | 124 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera8setLocalEN11AbyssEngine6AEMath6MatrixE`<br>TargetFollowCamera::setLocal(AbyssEngine::AEMath::Matrix) |
| 3376 | `0x15b3d4` | `0x15bff4` | +3104 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera8getLocalEv`<br>TargetFollowCamera::getLocal() |
| 3377 | `0x15b3e4` | `0x15c004` | +3104 | 128 | 128 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera9setLockedEb`<br>TargetFollowCamera::setLocked(bool) |
| 3378 | `0x15b464` | `0x15c084` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera23setRotationAroundTargetEb`<br>TargetFollowCamera::setRotationAroundTarget(bool) |
| 3379 | `0x15b46c` | `0x15c08c` | +3104 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera18rotateAroundTargetEfff`<br>TargetFollowCamera::rotateAroundTarget(float, float, float) |
| 3380 | `0x15b498` | `0x15c0b8` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera10zoomTargetEf`<br>TargetFollowCamera::zoomTarget(float) |
| 3381 | `0x15b49e` | `0x15c0be` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera12setLookAtCamEb`<br>TargetFollowCamera::setLookAtCam(bool) |
| 3382 | `0x15b4a4` | `0x15c0c4` | +3104 | 102 | 102 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera20calculateCoefficentsEf`<br>TargetFollowCamera::calculateCoefficents(float) |
| 3383 | `0x15b50a` | `0x15c12a` | +3104 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera18setFastForwardModeEb`<br>TargetFollowCamera::setFastForwardMode(bool) |
| 3384 | `0x15b574` | `0x15c194` | +3104 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera15setShipHandlingEf`<br>TargetFollowCamera::setShipHandling(float) |
| 3385 | `0x15b5c8` | `0x15c1e8` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera19isInFastForwardModeEv`<br>TargetFollowCamera::isInFastForwardMode() |
| 3386 | `0x15b5ce` | `0x15c1ee` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera14isInLookAtModeEv`<br>TargetFollowCamera::isInLookAtMode() |
| 3387 | `0x15b5d4` | `0x15c1f4` | +3104 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN18TargetFollowCamera17resetShipHandlingEv`<br>TargetFollowCamera::resetShipHandling() |
| 3388 | `0x15b5ec` | `0x15c20c` | +3104 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera3hitEv`<br>TargetFollowCamera::hit() |
| 3389 | `0x15b60e` | `0x15c22e` | +3104 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera8hitSmallEv`<br>TargetFollowCamera::hitSmall() |
| 3390 | `0x15b62c` | `0x15c24c` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera18useTargetsUpVectorEb`<br>TargetFollowCamera::useTargetsUpVector(bool) |
| 3391 | `0x15b632` | `0x15c252` | +3104 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera4rollEf`<br>TargetFollowCamera::roll(float) |
| 3392 | `0x15b644` | `0x15c264` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18TargetFollowCamera7setRollEf`<br>TargetFollowCamera::setRoll(float) |
| 3393 | `0x15b64c` | `0x15c26c` | +3104 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN7MissionC1EiN11AbyssEngine6StringEPiiiii`<br>Mission::Mission(int, AbyssEngine::String, int*, int, int, int, int) |
| 3394 | `0x15b64c` | `0x15c26c` | +3104 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN7MissionC2EiN11AbyssEngine6StringEPiiiii`<br>Mission::Mission(int, AbyssEngine::String, int*, int, int, int, int) |
| 3395 | `0x15b770` | `0x15c390` | +3104 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN7MissionC2Eiii`<br>Mission::Mission(int, int, int) |
| 3396 | `0x15b770` | `0x15c390` | +3104 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN7MissionC1Eiii`<br>Mission::Mission(int, int, int) |
| 3397 | `0x15b89c` | `0x15c4bc` | +3104 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN7MissionC1Ev`<br>Mission::Mission() |
| 3398 | `0x15b89c` | `0x15c4bc` | +3104 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN7MissionC2Ev`<br>Mission::Mission() |
| 3399 | `0x15b978` | `0x15c598` | +3104 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN7MissionC2Ei`<br>Mission::Mission(int) |
| 3400 | `0x15b978` | `0x15c598` | +3104 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN7MissionC1Ei`<br>Mission::Mission(int) |
| 3401 | `0x15ba54` | `0x15c674` | +3104 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN7MissionD1Ev`<br>Mission::~Mission() |
| 3402 | `0x15ba54` | `0x15c674` | +3104 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN7MissionD2Ev`<br>Mission::~Mission() |
| 3403 | `0x15ba98` | `0x15c6b8` | +3104 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN7MissionD0Ev`<br>Mission::~Mission() |
| 3404 | `0x15baa8` | `0x15c6c8` | +3104 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN7Mission7isEmptyEv`<br>Mission::isEmpty() |
| 3405 | `0x15bab4` | `0x15c6d4` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7Mission10setVisibleEb`<br>Mission::setVisible(bool) |
| 3406 | `0x15baba` | `0x15c6da` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7Mission9isVisibleEv`<br>Mission::isVisible() |
| 3407 | `0x15bac0` | `0x15c6e0` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission9setFailedEb`<br>Mission::setFailed(bool) |
| 3408 | `0x15bac4` | `0x15c6e4` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission9hasFailedEv`<br>Mission::hasFailed() |
| 3409 | `0x15bac8` | `0x15c6e8` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission6setWonEb`<br>Mission::setWon(bool) |
| 3410 | `0x15bacc` | `0x15c6ec` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission6hasWonEv`<br>Mission::hasWon() |
| 3411 | `0x15bad0` | `0x15c6f0` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7Mission18setProductionGoodsEii`<br>Mission::setProductionGoods(int, int) |
| 3412 | `0x15bad6` | `0x15c6f6` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission22getProductionGoodIndexEv`<br>Mission::getProductionGoodIndex() |
| 3413 | `0x15bada` | `0x15c6fa` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission23getProductionGoodAmountEv`<br>Mission::getProductionGoodAmount() |
| 3414 | `0x15bade` | `0x15c6fe` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission14setStatusValueEi`<br>Mission::setStatusValue(int) |
| 3415 | `0x15bae2` | `0x15c702` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission14getStatusValueEv`<br>Mission::getStatusValue() |
| 3416 | `0x15bae6` | `0x15c706` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission7getTypeEv`<br>Mission::getType() |
| 3417 | `0x15baec` | `0x15c70c` | +3104 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN7Mission7getNameEv`<br>Mission::getName() |
| 3418 | `0x15bb2c` | `0x15c74c` | +3104 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN7Mission14getDescriptionEv`<br>Mission::getDescription() |
| 3419 | `0x15bb40` | `0x15c760` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission16isOutsideMissionEv`<br>Mission::isOutsideMission() |
| 3420 | `0x15bb44` | `0x15c764` | +3104 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN7Mission13getClientNameEv`<br>Mission::getClientName() |
| 3421 | `0x15bb52` | `0x15c772` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission14getClientImageEv`<br>Mission::getClientImage() |
| 3422 | `0x15bb56` | `0x15c776` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission13getClientRaceEv`<br>Mission::getClientRace() |
| 3423 | `0x15bb5a` | `0x15c77a` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission9getRewardEv`<br>Mission::getReward() |
| 3424 | `0x15bb5e` | `0x15c77e` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission9setRewardEi`<br>Mission::setReward(int) |
| 3425 | `0x15bb62` | `0x15c782` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission8getCostsEv`<br>Mission::getCosts() |
| 3426 | `0x15bb66` | `0x15c786` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission8setCostsEi`<br>Mission::setCosts(int) |
| 3427 | `0x15bb6a` | `0x15c78a` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission8getBonusEv`<br>Mission::getBonus() |
| 3428 | `0x15bb6e` | `0x15c78e` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission8setBonusEi`<br>Mission::setBonus(int) |
| 3429 | `0x15bb72` | `0x15c792` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7Mission23setInstantActionMissionEb`<br>Mission::setInstantActionMission(bool) |
| 3430 | `0x15bb78` | `0x15c798` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN7Mission22isInstantActionMissionEv`<br>Mission::isInstantActionMission() |
| 3431 | `0x15bb7e` | `0x15c79e` | +3104 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN7Mission17isCampaignMissionEv`<br>Mission::isCampaignMission() |
| 3432 | `0x15bb88` | `0x15c7a8` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission18setCampaignMissionEb`<br>Mission::setCampaignMission(bool) |
| 3433 | `0x15bb8c` | `0x15c7ac` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission16getTargetStationEv`<br>Mission::getTargetStation() |
| 3434 | `0x15bb90` | `0x15c7b0` | +3104 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN7Mission20getTargetStationNameEv`<br>Mission::getTargetStationName() |
| 3435 | `0x15bb9e` | `0x15c7be` | +3104 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN7Mission19getTargetSystemNameEv`<br>Mission::getTargetSystemName() |
| 3436 | `0x15bbac` | `0x15c7cc` | +3104 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN7Mission19setTargetSystemNameERKN11AbyssEngine6StringE`<br>Mission::setTargetSystemName(AbyssEngine::String const&) |
| 3437 | `0x15bbb4` | `0x15c7d4` | +3104 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN7Mission16setTargetStationEi`<br>Mission::setTargetStation(int) |
| 3438 | `0x15bc1c` | `0x15c83c` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission13setDifficultyEi`<br>Mission::setDifficulty(int) |
| 3439 | `0x15bc20` | `0x15c840` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission13getDifficultyEv`<br>Mission::getDifficulty() |
| 3440 | `0x15bc24` | `0x15c844` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission11getDistanceEv`<br>Mission::getDistance() |
| 3441 | `0x15bc28` | `0x15c848` | +3104 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN7Mission12calcDistanceEv`<br>Mission::calcDistance() |
| 3442 | `0x15bcac` | `0x15c8cc` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission8getAgentEv`<br>Mission::getAgent() |
| 3443 | `0x15bcb0` | `0x15c8d0` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission8setAgentEP5Agent`<br>Mission::setAgent(Agent*) |
| 3444 | `0x15bcb4` | `0x15c8d4` | +3104 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN7Mission13getTargetNameEv`<br>Mission::getTargetName() |
| 3445 | `0x15bcc2` | `0x15c8e2` | +3104 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN7Mission13setTargetNameEN11AbyssEngine6StringE`<br>Mission::setTargetName(AbyssEngine::String) |
| 3446 | `0x15bcc8` | `0x15c8e8` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN7Mission7setTypeEi`<br>Mission::setType(int) |
| 3447 | `0x15bccc` | `0x15c8ec` | +3104 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN7Mission5cloneEv`<br>Mission::clone() |
| 3448 | `0x15bd58` | `0x15c978` | +3104 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN5TrailC1Eii`<br>Trail::Trail(int, int) |
| 3449 | `0x15bd58` | `0x15c978` | +3104 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN5TrailC2Eii`<br>Trail::Trail(int, int) |
| 3450 | `0x15bef4` | `0x15cb14` | +3104 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN5Trail10changeTypeEi`<br>Trail::changeType(int) |
| 3451 | `0x15bf8c` | `0x15cbac` | +3104 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN5TrailD2Ev`<br>Trail::~Trail() |
| 3452 | `0x15bf8c` | `0x15cbac` | +3104 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN5TrailD1Ev`<br>Trail::~Trail() |
| 3453 | `0x15bfac` | `0x15cbcc` | +3104 | 472 | 472 | 0 | 100.0% |  | Y | `_ZN5Trail6updateEffffff`<br>Trail::update(float, float, float, float, float, float) |
| 3454 | `0x15c184` | `0x15cda4` | +3104 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN5Trail8setWidthEi`<br>Trail::setWidth(int) |
| 3455 | `0x15c1fc` | `0x15ce1c` | +3104 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN5Trail6updateERKN11AbyssEngine6AEMath6VectorES4_`<br>Trail::update(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 3456 | `0x15c228` | `0x15ce48` | +3104 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN5Trail6updateERKN11AbyssEngine6AEMath6MatrixES4_RKNS1_6VectorE`<br>Trail::update(AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Matrix const&, AbyssEngine::AEMath::Vector const&) |
| 3457 | `0x15c290` | `0x15ceb0` | +3104 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN5Trail5resetEN11AbyssEngine6AEMath6VectorE`<br>Trail::reset(AbyssEngine::AEMath::Vector) |
| 3458 | `0x15c308` | `0x15cf28` | +3104 | 98 | 98 | 0 | 100.0% | Y | Y | `_ZN5Trail9translateERKN11AbyssEngine6AEMath6VectorE`<br>Trail::translate(AbyssEngine::AEMath::Vector const&) |
| 3459 | `0x15c36c` | `0x15cf8c` | +3104 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN5Trail6renderEv`<br>Trail::render() |
| 3460 | `0x15c380` | `0x15cfa0` | +3104 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN9SentryGunC1EP3GuniiiP5Level`<br>SentryGun::SentryGun(Gun*, int, int, int, Level*) |
| 3461 | `0x15c380` | `0x15cfa0` | +3104 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN9SentryGunC2EP3GuniiiP5Level`<br>SentryGun::SentryGun(Gun*, int, int, int, Level*) |
| 3462 | `0x15c3bc` | `0x15cfdc` | +3104 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN9SentryGunD1Ev`<br>SentryGun::~SentryGun() |
| 3463 | `0x15c3bc` | `0x15cfdc` | +3104 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN9SentryGunD2Ev`<br>SentryGun::~SentryGun() |
| 3464 | `0x15c3c0` | `0x15cfe0` | +3104 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN9SentryGunD0Ev`<br>SentryGun::~SentryGun() |
| 3465 | `0x15c3d0` | `0x15cff0` | +3104 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9SentryGun6renderEv`<br>SentryGun::render() |
| 3466 | `0x15c3d2` | `0x15cff2` | +3104 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN9SentryGun6updateEi`<br>SentryGun::update(int) |
| 3467 | `0x15c468` | `0x15d088` | +3104 | 972 | 972 | 0 | 100.0% |  | Y | `_ZN10NewsTickerC2Eiiiii`<br>NewsTicker::NewsTicker(int, int, int, int, int) |
| 3468 | `0x15c468` | `0x15d088` | +3104 | 972 | 972 | 0 | 100.0% |  | Y | `_ZN10NewsTickerC1Eiiiii`<br>NewsTicker::NewsTicker(int, int, int, int, int) |
| 3469 | `0x15c834` | `0x15d454` | +3104 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP8NewsItemEvT_R5ArrayIS2_E`<br>void ArrayAdd<NewsItem*>(NewsItem*, Array<NewsItem*>&) |
| 3470 | `0x15c858` | `0x15d478` | +3104 | 6164 | 6164 | 0 | 100.0% |  | Y | `_ZN10NewsTicker13replaceTokensEN11AbyssEngine6StringE`<br>NewsTicker::replaceTokens(AbyssEngine::String) |
| 3471 | `0x15e06c` | `0x15ec8c` | +3104 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP8NewsItemEvR5ArrayIT_E`<br>void ArrayReleaseClasses<NewsItem*>(Array<NewsItem*>&) |
| 3472 | `0x15e0bc` | `0x15ecdc` | +3104 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP8NewsItemED2Ev`<br>Array<NewsItem*>::~Array() |
| 3473 | `0x15e0d2` | `0x15ecf2` | +3104 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN10NewsTickerD1Ev`<br>NewsTicker::~NewsTicker() |
| 3474 | `0x15e0d2` | `0x15ecf2` | +3104 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN10NewsTickerD2Ev`<br>NewsTicker::~NewsTicker() |
| 3475 | `0x15e0e4` | `0x15ed04` | +3104 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN10NewsTicker6updateEi`<br>NewsTicker::update(int) |
| 3476 | `0x15e174` | `0x15ed94` | +3104 | 320 | 320 | 0 | 100.0% |  | Y | `_ZN10NewsTicker4drawEv`<br>NewsTicker::draw() |
| 3477 | `0x15e2b4` | `0x15eed4` | +3104 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN10NewsTicker9getHeightEv`<br>NewsTicker::getHeight() |
| 3478 | `0x15e2d8` | `0x15eef8` | +3104 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN10NewsTicker12OnTouchBeginEii`<br>NewsTicker::OnTouchBegin(int, int) |
| 3479 | `0x15e328` | `0x15ef48` | +3104 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN10NewsTicker11OnTouchMoveEii`<br>NewsTicker::OnTouchMove(int, int) |
| 3480 | `0x15e374` | `0x15ef94` | +3104 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN10NewsTicker10OnTouchEndEii`<br>NewsTicker::OnTouchEnd(int, int) |
| 3481 | `0x15e38c` | `0x15efac` | +3104 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN6SparksC1Ei`<br>Sparks::Sparks(int) |
| 3482 | `0x15e38c` | `0x15efac` | +3104 | 220 | 220 | 0 | 100.0% |  | Y | `_ZN6SparksC2Ei`<br>Sparks::Sparks(int) |
| 3483 | `0x15e468` | `0x15f088` | +3104 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN6Sparks8isRocketEv`<br>Sparks::isRocket() |
| 3484 | `0x15e474` | `0x15f094` | +3104 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN6SparksD2Ev`<br>Sparks::~Sparks() |
| 3485 | `0x15e474` | `0x15f094` | +3104 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN6SparksD1Ev`<br>Sparks::~Sparks() |
| 3486 | `0x15e48c` | `0x15f0ac` | +3104 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN6Sparks7explodeEiii`<br>Sparks::explode(int, int, int) |
| 3487 | `0x15e594` | `0x15f1b4` | +3104 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN6Sparks7explodeERKN11AbyssEngine6AEMath6VectorE`<br>Sparks::explode(AbyssEngine::AEMath::Vector const&) |
| 3488 | `0x15e5c0` | `0x15f1e0` | +3104 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN6Sparks6renderEv`<br>Sparks::render() |
| 3489 | `0x15e660` | `0x15f280` | +3104 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN6Sparks6updateEi`<br>Sparks::update(int) |
| 3490 | `0x15e6d4` | `0x15f2f4` | +3104 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6Sparks9translateERKN11AbyssEngine6AEMath6VectorE`<br>Sparks::translate(AbyssEngine::AEMath::Vector const&) |
| 3491 | `0x15e720` | `0x15f340` | +3104 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN10PlayerJunkC2EiP6PlayerP10AEGeometryfff`<br>PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float) |
| 3492 | `0x15e720` | `0x15f340` | +3104 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN10PlayerJunkC1EiP6PlayerP10AEGeometryfff`<br>PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float) |
| 3493 | `0x15e768` | `0x15f388` | +3104 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN10PlayerJunkD1Ev`<br>PlayerJunk::~PlayerJunk() |
| 3494 | `0x15e768` | `0x15f388` | +3104 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN10PlayerJunkD2Ev`<br>PlayerJunk::~PlayerJunk() |
| 3495 | `0x15e76c` | `0x15f38c` | +3104 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN10PlayerJunkD0Ev`<br>PlayerJunk::~PlayerJunk() |
| 3496 | `0x15e77c` | `0x15f39c` | +3104 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN10PlayerJunk5resetEv`<br>PlayerJunk::reset() |
| 3497 | `0x15e798` | `0x15f3b8` | +3104 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN10PlayerJunk6updateEi`<br>PlayerJunk::update(int) |
| 3498 | `0x15e8cc` | `0x15f4ec` | +3104 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN10PlayerJunk6renderEv`<br>PlayerJunk::render() |
| 3499 | `0x15e8f0` | `0x15f510` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10PlayerJunk7collideEfff`<br>PlayerJunk::collide(float, float, float) |
| 3500 | `0x15e8f4` | `0x15f514` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10PlayerJunk12outerCollideEfff`<br>PlayerJunk::outerCollide(float, float, float) |
| 3501 | `0x15e8f8` | `0x15f518` | +3104 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN9RocketGunC2EiP3GuniijibP5Level`<br>RocketGun::RocketGun(int, Gun*, int, int, unsigned int, int, bool, Level*) |
| 3502 | `0x15e8f8` | `0x15f518` | +3104 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN9RocketGunC1EiP3GuniijibP5Level`<br>RocketGun::RocketGun(int, Gun*, int, int, unsigned int, int, bool, Level*) |
| 3503 | `0x15e9d0` | `0x15f5f0` | +3104 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN9RocketGunD2Ev`<br>RocketGun::~RocketGun() |
| 3504 | `0x15e9d0` | `0x15f5f0` | +3104 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN9RocketGunD1Ev`<br>RocketGun::~RocketGun() |
| 3505 | `0x15ea28` | `0x15f648` | +3104 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN9RocketGunD0Ev`<br>RocketGun::~RocketGun() |
| 3506 | `0x15ea38` | `0x15f658` | +3104 | 608 | 608 | 0 | 100.0% |  | Y | `_ZN9RocketGun8setRadarEP5Radar`<br>RocketGun::setRadar(Radar*) |
| 3507 | `0x15ec98` | `0x15f8b8` | +3104 | 60 | 60 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIN11AbyssEngine6AEMath6MatrixEEvjR5ArrayIT_E`<br>void ArraySetLength<AbyssEngine::AEMath::Matrix>(unsigned int, Array<AbyssEngine::AEMath::Matrix>&) |
| 3508 | `0x15ecd4` | `0x15f8f4` | +3104 | 4 | 4 | 0 | 100.0% |  | Y | `_ZN9RocketGun6renderEv`<br>RocketGun::render() |
| 3509 | `0x15ecd8` | `0x15f8f8` | +3104 | 1908 | 1908 | 0 | 85.7% |  |  | `_ZN9RocketGun6updateEi`<br>RocketGun::update(int) |
| 3510 | `0x15f44c` | `0x16006c` | +3104 | 332 | 332 | 0 | 100.0% |  | Y | `_ZN9RocketGun9seekEnemyEii`<br>RocketGun::seekEnemy(int, int) |
| 3511 | `0x15f598` | `0x1601b8` | +3104 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9RocketGun9translateERKN11AbyssEngine6AEMath6VectorE`<br>RocketGun::translate(AbyssEngine::AEMath::Vector const&) |
| 3512 | `0x15f59c` | `0x1601bc` | +3104 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN12MarqueeImageC1Etiiif`<br>MarqueeImage::MarqueeImage(unsigned short, int, int, int, float) |
| 3513 | `0x15f59c` | `0x1601bc` | +3104 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN12MarqueeImageC2Etiiif`<br>MarqueeImage::MarqueeImage(unsigned short, int, int, int, float) |
| 3514 | `0x15f5f0` | `0x160210` | +3104 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN12MarqueeImageD2Ev`<br>MarqueeImage::~MarqueeImage() |
| 3515 | `0x15f5f0` | `0x160210` | +3104 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN12MarqueeImageD1Ev`<br>MarqueeImage::~MarqueeImage() |
| 3516 | `0x15f5f2` | `0x160212` | +3104 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN12MarqueeImage11setPositionEii`<br>MarqueeImage::setPosition(int, int) |
| 3517 | `0x15f5f8` | `0x160218` | +3104 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN12MarqueeImage8setSpeedEf`<br>MarqueeImage::setSpeed(float) |
| 3518 | `0x15f5fc` | `0x16021c` | +3104 | 88 | 88 | 0 | 100.0% | Y | Y | `_ZN12MarqueeImage6updateEi`<br>MarqueeImage::update(int) |
| 3519 | `0x15f654` | `0x160274` | +3104 | 160 | 160 | 0 | 100.0% |  | Y | `_ZN12MarqueeImage4drawEii`<br>MarqueeImage::draw(int, int) |
| 3520 | `0x15f6f4` | `0x160314` | +3104 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN12MarqueeImage4drawEv`<br>MarqueeImage::draw() |
| 3521 | `0x15f700` | `0x160330` | +3120 | 516 | 516 | 0 | 100.0% |  | Y | `_ZN9ObjectGunC1EiP3GunijP5Level`<br>ObjectGun::ObjectGun(int, Gun*, int, unsigned int, Level*) |
| 3522 | `0x15f700` | `0x160330` | +3120 | 516 | 516 | 0 | 100.0% |  | Y | `_ZN9ObjectGunC2EiP3GunijP5Level`<br>ObjectGun::ObjectGun(int, Gun*, int, unsigned int, Level*) |
| 3523 | `0x15f904` | `0x160534` | +3120 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN9ObjectGunD2Ev`<br>ObjectGun::~ObjectGun() |
| 3524 | `0x15f904` | `0x160534` | +3120 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN9ObjectGunD1Ev`<br>ObjectGun::~ObjectGun() |
| 3525 | `0x15f94c` | `0x16057c` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN9ObjectGunD0Ev`<br>ObjectGun::~ObjectGun() |
| 3526 | `0x15f95c` | `0x16058c` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9ObjectGun10setEnemiesEP5ArrayIP6PlayerE`<br>ObjectGun::setEnemies(Array<Player*>*) |
| 3527 | `0x15f962` | `0x160592` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9ObjectGun8setEnemyEP6Player`<br>ObjectGun::setEnemy(Player*) |
| 3528 | `0x15f964` | `0x160594` | +3120 | 1092 | 1092 | 0 | 100.0% |  | Y | `_ZN9ObjectGun6updateEi`<br>ObjectGun::update(int) |
| 3529 | `0x15fda8` | `0x1609d8` | +3120 | 1456 | 1456 | 0 | 100.0% |  | Y | `_ZN9ObjectGun6renderEv`<br>ObjectGun::render() |
| 3530 | `0x160358` | `0x160f88` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9ObjectGun9translateERKN11AbyssEngine6AEMath6VectorE`<br>ObjectGun::translate(AbyssEngine::AEMath::Vector const&) |
| 3531 | `0x16035a` | `0x160f8a` | +3120 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN9ObjectGun10setScalingEiii`<br>ObjectGun::setScaling(int, int, int) |
| 3532 | `0x160374` | `0x160fa4` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN9ObjectGun10replaceGunEji`<br>ObjectGun::replaceGun(unsigned int, int) |
| 3533 | `0x1603ac` | `0x160fdc` | +3120 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN10SpacePointC2EiRKN11AbyssEngine6AEMath6VectorES4_i`<br>SpacePoint::SpacePoint(int, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, int) |
| 3534 | `0x1603ac` | `0x160fdc` | +3120 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN10SpacePointC1EiRKN11AbyssEngine6AEMath6VectorES4_i`<br>SpacePoint::SpacePoint(int, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&, int) |
| 3535 | `0x1603e6` | `0x161016` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10SpacePoint6isFreeEv`<br>SpacePoint::isFree() |
| 3536 | `0x1603ea` | `0x16101a` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN10SpacePoint4takeEv`<br>SpacePoint::take() |
| 3537 | `0x1603f0` | `0x161020` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN10SpacePoint8giveFreeEv`<br>SpacePoint::giveFree() |
| 3538 | `0x1603f6` | `0x161026` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN10SpacePoint8getIndexEv`<br>SpacePoint::getIndex() |
| 3539 | `0x1603fa` | `0x16102a` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN10SpacePointD2Ev`<br>SpacePoint::~SpacePoint() |
| 3540 | `0x1603fa` | `0x16102a` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN10SpacePointD1Ev`<br>SpacePoint::~SpacePoint() |
| 3541 | `0x1603fc` | `0x16102c` | +3120 | 230 | 230 | 0 | 100.0% |  | Y | `_ZN3HudC2Ev`<br>Hud::Hud() |
| 3542 | `0x1603fc` | `0x16102c` | +3120 | 230 | 230 | 0 | 100.0% |  | Y | `_ZN3HudC1Ev`<br>Hud::Hud() |
| 3543 | `0x1604e4` | `0x161114` | +3120 | 3464 | 3464 | 0 | 100.0% |  | Y | `_ZN3Hud4initEv`<br>Hud::init() |
| 3544 | `0x16126c` | `0x161e9c` | +3120 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN3HudD2Ev`<br>Hud::~Hud() |
| 3545 | `0x16126c` | `0x161e9c` | +3120 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN3HudD1Ev`<br>Hud::~Hud() |
| 3546 | `0x161354` | `0x161f84` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Hud18setAutofireEnabledEb`<br>Hud::setAutofireEnabled(bool) |
| 3547 | `0x16135a` | `0x161f8a` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPvEC2Ev`<br>Array<void*>::Array() |
| 3548 | `0x161376` | `0x161fa6` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIPvEvjR5ArrayIT_E`<br>void ArraySetLength<void*>(unsigned int, Array<void*>&) |
| 3549 | `0x1613aa` | `0x161fda` | +3120 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN3Hud12closeHudMenuEv`<br>Hud::closeHudMenu() |
| 3550 | `0x1613d0` | `0x162000` | +3120 | 112 | 112 | 0 | 100.0% |  | Y | `_ZN3Hud23checkIfQuickMenuIsEmptyEv`<br>Hud::checkIfQuickMenuIsEmpty() |
| 3551 | `0x161440` | `0x162070` | +3120 | 52 | 52 | 0 | 100.0% | Y | Y | `_ZN3Hud14releaseAllKeysEv`<br>Hud::releaseAllKeys() |
| 3552 | `0x161474` | `0x1620a4` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN3Hud10setVisibleEb`<br>Hud::setVisible(bool) |
| 3553 | `0x161478` | `0x1620a8` | +3120 | 332 | 332 | 0 | 100.0% |  | Y | `_ZN3Hud27updateSecondaryWeaponStringEv`<br>Hud::updateSecondaryWeaponString() |
| 3554 | `0x1615c8` | `0x1621f8` | +3120 | 3364 | 3364 | 0 | 100.0% |  | Y | `_ZN3Hud11initHudMenuEiP5Level`<br>Hud::initHudMenu(int, Level*) |
| 3555 | `0x1622ec` | `0x162f1c` | +3120 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN3Hud25setCurrentSecondaryWeaponEP4Item`<br>Hud::setCurrentSecondaryWeapon(Item*) |
| 3556 | `0x1622f4` | `0x162f24` | +3120 | 404 | 404 | 0 | 100.0% |  | Y | `_ZN3Hud14drawEventQueueEv`<br>Hud::drawEventQueue() |
| 3557 | `0x162488` | `0x1630b8` | +3120 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN3Hud15addToEventQueueEP8ListItem`<br>Hud::addToEventQueue(ListItem*) |
| 3558 | `0x1624b8` | `0x1630e8` | +3120 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN3Hud15setTimeExtenderEbbbb`<br>Hud::setTimeExtender(bool, bool, bool, bool) |
| 3559 | `0x1624dc` | `0x16310c` | +3120 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN3Hud11updateQueueEi`<br>Hud::updateQueue(int) |
| 3560 | `0x162550` | `0x163180` | +3120 | 78 | 78 | 0 | 100.0% |  | Y | `_ZN3Hud10clearQueueEv`<br>Hud::clearQueue() |
| 3561 | `0x1625a0` | `0x1631d0` | +3120 | 448 | 448 | 0 | 100.0% |  | Y | `_ZN3Hud13hudEventMedalEii`<br>Hud::hudEventMedal(int, int) |
| 3562 | `0x162760` | `0x163390` | +3120 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN3Hud20sameHudEventAsBeforeEN11AbyssEngine6StringE`<br>Hud::sameHudEventAsBefore(AbyssEngine::String) |
| 3563 | `0x1627a0` | `0x1633d0` | +3120 | 2656 | 2656 | 0 | 100.0% |  | Y | `_ZN3Hud8hudEventEiP9PlayerEgoi`<br>Hud::hudEvent(int, PlayerEgo*, int) |
| 3564 | `0x163200` | `0x163e30` | +3120 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN3Hud29sameHudEventAsBeforeAggregateEN11AbyssEngine6StringE`<br>Hud::sameHudEventAsBeforeAggregate(AbyssEngine::String) |
| 3565 | `0x16323e` | `0x163e6e` | +3120 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN3Hud9playerHitEv`<br>Hud::playerHit() |
| 3566 | `0x16324c` | `0x163e7c` | +3120 | 1164 | 1164 | 0 | 100.0% |  | Y | `_ZN3Hud10catchCargoEiibbbbb`<br>Hud::catchCargo(int, int, bool, bool, bool, bool, bool) |
| 3567 | `0x1636d8` | `0x164308` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Hud9cargoFullEv`<br>Hud::cargoFull() |
| 3568 | `0x1636de` | `0x16430e` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Hud20setHackingGameActiveEb`<br>Hud::setHackingGameActive(bool) |
| 3569 | `0x1636e4` | `0x164314` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Hud19isHackingGameActiveEv`<br>Hud::isHackingGameActive() |
| 3570 | `0x1636ea` | `0x16431a` | +3120 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN3Hud11firePressedEv`<br>Hud::firePressed() |
| 3571 | `0x1636f4` | `0x164324` | +3120 | 176 | 176 | 0 | 100.0% |  | Y | `_ZN3Hud15drawEventStringEN11AbyssEngine6StringEb`<br>Hud::drawEventString(AbyssEngine::String, bool) |
| 3572 | `0x1637a4` | `0x1643d4` | +3120 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN3Hud15drawPauseButtonEv`<br>Hud::drawPauseButton() |
| 3573 | `0x1637e0` | `0x164410` | +3120 | 944 | 944 | 0 | 81.5% |  |  | `_ZN3Hud22drawChallengeModeScoreEi`<br>Hud::drawChallengeModeScore(int) |
| 3574 | `0x163b90` | `0x1647c0` | +3120 | 9332 | 9332 | 0 | 87.3% |  |  | `_ZN3Hud4drawExxP9PlayerEgobjj`<br>Hud::draw(long long, long long, PlayerEgo*, bool, unsigned int, unsigned int) |
| 3575 | `0x166004` | `0x166c34` | +3120 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN3Hud16resetAnalogStickEv`<br>Hud::resetAnalogStick() |
| 3576 | `0x16600e` | `0x166c3e` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_Z21drawControlsInterfacexxP9PlayerEgobjj`<br>drawControlsInterface(long long, long long, PlayerEgo*, bool, unsigned int, unsigned int) |
| 3577 | `0x166010` | `0x166c40` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN3Hud13drawBigNumberEiiib`<br>Hud::drawBigNumber(int, int, int, bool) |
| 3578 | `0x166012` | `0x166c42` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN3Hud14drawTitleImageEb`<br>Hud::drawTitleImage(bool) |
| 3579 | `0x166014` | `0x166c44` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN3Hud11drawCreditsEv`<br>Hud::drawCredits() |
| 3580 | `0x166018` | `0x166c48` | +3120 | 608 | 608 | 0 | 100.0% |  | Y | `_ZN3Hud20drawOrbitInformationEv`<br>Hud::drawOrbitInformation() |
| 3581 | `0x166278` | `0x166ea8` | +3120 | 660 | 660 | 0 | 100.0% |  | Y | `_ZN3Hud8drawMenuEi`<br>Hud::drawMenu(int) |
| 3582 | `0x16650c` | `0x16713c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN3Hud9hudActionEiP5LevelP5Radar`<br>Hud::hudAction(int, Level*, Radar*) |
| 3583 | `0x166510` | `0x167140` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Hud15jumpMapSelectedEv`<br>Hud::jumpMapSelected() |
| 3584 | `0x166516` | `0x167146` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Hud18setJumpMapSelectedEb`<br>Hud::setJumpMapSelected(bool) |
| 3585 | `0x16651c` | `0x16714c` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN3Hud21enableFireForTutorialEb`<br>Hud::enableFireForTutorial(bool) |
| 3586 | `0x166522` | `0x167152` | +3120 | 40 | 40 | 0 | 100.0% | Y | Y | `_ZN3Hud10getAnalogXEv`<br>Hud::getAnalogX() |
| 3587 | `0x16654a` | `0x16717a` | +3120 | 40 | 40 | 0 | 100.0% | Y | Y | `_ZN3Hud10getAnalogYEv`<br>Hud::getAnalogY() |
| 3588 | `0x166574` | `0x1671a4` | +3120 | 1160 | 1160 | 0 | 100.0% |  | Y | `_ZN3Hud14touchedElementEjj`<br>Hud::touchedElement(unsigned int, unsigned int) |
| 3589 | `0x1669fc` | `0x16762c` | +3120 | 186 | 186 | 0 | 100.0% |  | Y | `_ZN3Hud10touchBeginEjjPv`<br>Hud::touchBegin(unsigned int, unsigned int, void*) |
| 3590 | `0x166ab8` | `0x1676e8` | +3120 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN3Hud9touchMoveEjjPv`<br>Hud::touchMove(unsigned int, unsigned int, void*) |
| 3591 | `0x166b84` | `0x1677b4` | +3120 | 118 | 118 | 0 | 100.0% |  | Y | `_ZN3Hud8touchEndEjjPv`<br>Hud::touchEnd(unsigned int, unsigned int, void*) |
| 3592 | `0x166bfc` | `0x16782a` | +3118 | 90 | 90 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowC2EP7MissionP5Leveli`<br>DialogueWindow::DialogueWindow(Mission*, Level*, int) |
| 3593 | `0x166bfc` | `0x16782a` | +3118 | 90 | 90 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowC1EP7MissionP5Leveli`<br>DialogueWindow::DialogueWindow(Mission*, Level*, int) |
| 3594 | `0x166c58` | `0x167884` | +3116 | 652 | 652 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow4initEv`<br>DialogueWindow::init() |
| 3595 | `0x166ee4` | `0x167b10` | +3116 | 120 | 120 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow3setEP7Missionii`<br>DialogueWindow::set(Mission*, int, int) |
| 3596 | `0x166f5c` | `0x167b88` | +3116 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowC1Ev`<br>DialogueWindow::DialogueWindow() |
| 3597 | `0x166f5c` | `0x167b88` | +3116 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowC2Ev`<br>DialogueWindow::DialogueWindow() |
| 3598 | `0x166fa0` | `0x167bcc` | +3116 | 376 | 376 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowC2EPN11AbyssEngine6StringES2_Pi`<br>DialogueWindow::DialogueWindow(AbyssEngine::String*, AbyssEngine::String*, int*) |
| 3599 | `0x166fa0` | `0x167bcc` | +3116 | 376 | 376 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowC1EPN11AbyssEngine6StringES2_Pi`<br>DialogueWindow::DialogueWindow(AbyssEngine::String*, AbyssEngine::String*, int*) |
| 3600 | `0x167118` | `0x167d44` | +3116 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowD1Ev`<br>DialogueWindow::~DialogueWindow() |
| 3601 | `0x167118` | `0x167d44` | +3116 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN14DialogueWindowD2Ev`<br>DialogueWindow::~DialogueWindow() |
| 3602 | `0x1671b4` | `0x167de0` | +3116 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow19hasBriefingDialogueEi`<br>DialogueWindow::hasBriefingDialogue(int) |
| 3603 | `0x1671d4` | `0x167e00` | +3116 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow18hasSuccessDialogueEi`<br>DialogueWindow::hasSuccessDialogue(int) |
| 3604 | `0x1671f4` | `0x167e20` | +3116 | 3484 | 3484 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow11loadContentEv`<br>DialogueWindow::loadContent() |
| 3605 | `0x167f90` | `0x168bbc` | +3116 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN14DialogueWindow7getModeEv`<br>DialogueWindow::getMode() |
| 3606 | `0x167f94` | `0x168bc0` | +3116 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow6updateEi`<br>DialogueWindow::update(int) |
| 3607 | `0x168008` | `0x168c34` | +3116 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow10isLastPageEv`<br>DialogueWindow::isLastPage() |
| 3608 | `0x16801e` | `0x168c4a` | +3116 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow8nextPageEv`<br>DialogueWindow::nextPage() |
| 3609 | `0x168044` | `0x168c70` | +3116 | 104 | 104 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow6lengthEv`<br>DialogueWindow::length() |
| 3610 | `0x1680ac` | `0x168cd8` | +3116 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN14DialogueWindow11isFirstPageEv`<br>DialogueWindow::isFirstPage() |
| 3611 | `0x1680b6` | `0x168ce2` | +3116 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN14DialogueWindow8hasLevelEv`<br>DialogueWindow::hasLevel() |
| 3612 | `0x1680c0` | `0x168cec` | +3116 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN14DialogueWindow8setLevelEP5Level`<br>DialogueWindow::setLevel(Level*) |
| 3613 | `0x1680c4` | `0x168cf0` | +3116 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow12previousPageEv`<br>DialogueWindow::previousPage() |
| 3614 | `0x1680e0` | `0x168d0c` | +3116 | 184 | 184 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow51pickGermanGenericTextBecauseWeSaved100EurosWithThatEiP5Agent`<br>DialogueWindow::pickGermanGenericTextBecauseWeSaved100EurosWithThat(int, Agent*) |
| 3615 | `0x168198` | `0x168dc4` | +3116 | 384 | 384 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow4drawEv`<br>DialogueWindow::draw() |
| 3616 | `0x168318` | `0x168f44` | +3116 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow12OnTouchBeginEii`<br>DialogueWindow::OnTouchBegin(int, int) |
| 3617 | `0x168370` | `0x168f9c` | +3116 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow11OnTouchMoveEii`<br>DialogueWindow::OnTouchMove(int, int) |
| 3618 | `0x1683c8` | `0x168ff4` | +3116 | 296 | 296 | 0 | 100.0% |  | Y | `_ZN14DialogueWindow10OnTouchEndEii`<br>DialogueWindow::OnTouchEnd(int, int) |
| 3619 | `0x1684f0` | `0x16911c` | +3116 | 168 | 168 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC1ERKN11AbyssEngine6StringEiiih`<br>TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, unsigned char) |
| 3620 | `0x1684f0` | `0x16911c` | +3116 | 168 | 168 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC2ERKN11AbyssEngine6StringEiiih`<br>TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, unsigned char) |
| 3621 | `0x168598` | `0x1691c4` | +3116 | 1816 | 1816 | 0 | 100.0% |  | Y | `_ZN11TouchButton4initERKN11AbyssEngine6StringEjiiiiiihh`<br>TouchButton::init(AbyssEngine::String const&, unsigned int, int, int, int, int, int, int, unsigned char, unsigned char) |
| 3622 | `0x168cb0` | `0x1698dc` | +3116 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC1Ejiiih`<br>TouchButton::TouchButton(unsigned int, int, int, int, unsigned char) |
| 3623 | `0x168cb0` | `0x1698dc` | +3116 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC2Ejiiih`<br>TouchButton::TouchButton(unsigned int, int, int, int, unsigned char) |
| 3624 | `0x168d9c` | `0x1699c8` | +3116 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC2Ejjiiih`<br>TouchButton::TouchButton(unsigned int, unsigned int, int, int, int, unsigned char) |
| 3625 | `0x168d9c` | `0x1699c8` | +3116 | 236 | 236 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC1Ejjiiih`<br>TouchButton::TouchButton(unsigned int, unsigned int, int, int, int, unsigned char) |
| 3626 | `0x168e88` | `0x169ab4` | +3116 | 168 | 168 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC1ERKN11AbyssEngine6StringEiiiihh`<br>TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, int, unsigned char, unsigned char) |
| 3627 | `0x168e88` | `0x169ab4` | +3116 | 168 | 168 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC2ERKN11AbyssEngine6StringEiiiihh`<br>TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, int, unsigned char, unsigned char) |
| 3628 | `0x168f30` | `0x169b5c` | +3116 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC2ERKN11AbyssEngine6StringEiiiihhji`<br>TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, int, unsigned char, unsigned char, unsigned int, int) |
| 3629 | `0x168f30` | `0x169b5c` | +3116 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC1ERKN11AbyssEngine6StringEiiiihhji`<br>TouchButton::TouchButton(AbyssEngine::String const&, int, int, int, int, unsigned char, unsigned char, unsigned int, int) |
| 3630 | `0x168ffc` | `0x169c28` | +3116 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC1Ejiiiihh`<br>TouchButton::TouchButton(unsigned int, int, int, int, int, unsigned char, unsigned char) |
| 3631 | `0x168ffc` | `0x169c28` | +3116 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC2Ejiiiihh`<br>TouchButton::TouchButton(unsigned int, int, int, int, int, unsigned char, unsigned char) |
| 3632 | `0x1690e4` | `0x169d10` | +3116 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC1EiiRKN11AbyssEngine6StringEiih`<br>TouchButton::TouchButton(int, int, AbyssEngine::String const&, int, int, unsigned char) |
| 3633 | `0x1690e4` | `0x169d10` | +3116 | 164 | 164 | 0 | 100.0% |  | Y | `_ZN11TouchButtonC2EiiRKN11AbyssEngine6StringEiih`<br>TouchButton::TouchButton(int, int, AbyssEngine::String const&, int, int, unsigned char) |
| 3634 | `0x169188` | `0x169db4` | +3116 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN11TouchButtonD1Ev`<br>TouchButton::~TouchButton() |
| 3635 | `0x169188` | `0x169db4` | +3116 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN11TouchButtonD2Ev`<br>TouchButton::~TouchButton() |
| 3636 | `0x1691b0` | `0x169ddc` | +3116 | 248 | 248 | 0 | 100.0% |  | Y | `_ZN11TouchButton7setTextERKN11AbyssEngine6StringE`<br>TouchButton::setText(AbyssEngine::String const&) |
| 3637 | `0x1692a8` | `0x169ed4` | +3116 | 80 | 80 | 0 | 100.0% | Y | Y | `_ZN11TouchButton11setPositionEiih`<br>TouchButton::setPosition(int, int, unsigned char) |
| 3638 | `0x1692f8` | `0x169f24` | +3116 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11TouchButton13setNumberTextERKN11AbyssEngine6StringE`<br>TouchButton::setNumberText(AbyssEngine::String const&) |
| 3639 | `0x1692fe` | `0x169f2a` | +3116 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11TouchButton21setGamePadButtonImageEj`<br>TouchButton::setGamePadButtonImage(unsigned int) |
| 3640 | `0x169304` | `0x169f30` | +3116 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN11TouchButton19replaceTextKeepSizeERKN11AbyssEngine6StringE`<br>TouchButton::replaceTextKeepSize(AbyssEngine::String const&) |
| 3641 | `0x16934c` | `0x169f78` | +3116 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN11TouchButton12setSplitTextERKN11AbyssEngine6StringE`<br>TouchButton::setSplitText(AbyssEngine::String const&) |
| 3642 | `0x169352` | `0x169f7e` | +3116 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN11TouchButton7getTextEv`<br>TouchButton::getText() |
| 3643 | `0x169360` | `0x169f8c` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton8getWidthEv`<br>TouchButton::getWidth() |
| 3644 | `0x169366` | `0x169f92` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton9getHeightEv`<br>TouchButton::getHeight() |
| 3645 | `0x16936c` | `0x169f98` | +3116 | 8 | 8 | 0 | 100.0% |  | Y | `_ZN11TouchButton11setPositionEii`<br>TouchButton::setPosition(int, int) |
| 3646 | `0x169374` | `0x169fa0` | +3116 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11TouchButton12setYPositionEi`<br>TouchButton::setYPosition(int) |
| 3647 | `0x169380` | `0x169fac` | +3116 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN11TouchButton11getPositionEv`<br>TouchButton::getPosition() |
| 3648 | `0x16939e` | `0x169fca` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton18setHalfTransparentEb`<br>TouchButton::setHalfTransparent(bool) |
| 3649 | `0x1693a4` | `0x169fd0` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton10setVisibleEb`<br>TouchButton::setVisible(bool) |
| 3650 | `0x1693aa` | `0x169fd6` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton9isVisibleEv`<br>TouchButton::isVisible() |
| 3651 | `0x1693b0` | `0x169fdc` | +3116 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN11TouchButton9translateEii`<br>TouchButton::translate(int, int) |
| 3652 | `0x1693be` | `0x169fea` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton16setAlwaysPressedEb`<br>TouchButton::setAlwaysPressed(bool) |
| 3653 | `0x1693c4` | `0x169ff0` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton12setTextColorEi`<br>TouchButton::setTextColor(int) |
| 3654 | `0x1693ca` | `0x169ff6` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton16setPressProgressEf`<br>TouchButton::setPressProgress(float) |
| 3655 | `0x1693d0` | `0x169ffc` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton25setPressProgressHighlightEb`<br>TouchButton::setPressProgressHighlight(bool) |
| 3656 | `0x1693d8` | `0x16a004` | +3116 | 1376 | 1376 | 0 | 100.0% |  | Y | `_ZN11TouchButton4drawEv`<br>TouchButton::draw() |
| 3657 | `0x169938` | `0x16a564` | +3116 | 144 | 144 | 0 | 100.0% |  | Y | `_ZN11TouchButton13touchedInsideEii`<br>TouchButton::touchedInside(int, int) |
| 3658 | `0x1699c8` | `0x16a5f4` | +3116 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN11TouchButton10resetTouchEv`<br>TouchButton::resetTouch() |
| 3659 | `0x1699d0` | `0x16a5fc` | +3116 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11TouchButton9isTouchedEv`<br>TouchButton::isTouched() |
| 3660 | `0x1699d8` | `0x16a604` | +3116 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11TouchButton12OnTouchBeginEii`<br>TouchButton::OnTouchBegin(int, int) |
| 3661 | `0x169a28` | `0x16a654` | +3116 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN11TouchButton11OnTouchMoveEii`<br>TouchButton::OnTouchMove(int, int) |
| 3662 | `0x169a54` | `0x16a680` | +3116 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN11TouchButton10OnTouchEndEii`<br>TouchButton::OnTouchEnd(int, int) |
| 3663 | `0x169aa4` | `0x16a6d0` | +3116 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN9SpriteGunC2EP3Guni`<br>SpriteGun::SpriteGun(Gun*, int) |
| 3664 | `0x169ab8` | `0x16a6e4` | +3116 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9SpriteGunD2Ev`<br>SpriteGun::~SpriteGun() |
| 3665 | `0x169ab8` | `0x16a6e4` | +3116 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9SpriteGunD1Ev`<br>SpriteGun::~SpriteGun() |
| 3666 | `0x169aba` | `0x16a6e6` | +3116 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9SpriteGunD0Ev`<br>SpriteGun::~SpriteGun() |
| 3667 | `0x169abc` | `0x16a6e8` | +3116 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN9SpriteGun6renderEv`<br>SpriteGun::render() |
| 3668 | `0x169abe` | `0x16a6ea` | +3116 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9SpriteGun6updateEi`<br>SpriteGun::update(int) |
| 3669 | `0x169ac4` | `0x16a6f0` | +3116 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9SpriteGun10setEnemiesEP5ArrayIP6PlayerE`<br>SpriteGun::setEnemies(Array<Player*>*) |
| 3670 | `0x169aca` | `0x16a6f6` | +3116 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN9SpriteGun8setEnemyEP6Player`<br>SpriteGun::setEnemy(Player*) |
| 3671 | `0x169ad0` | `0x16a700` | +3120 | 1200 | 1200 | 0 | 100.0% |  | Y | `_ZN11SpaceLoungeC1Ev`<br>SpaceLounge::SpaceLounge() |
| 3672 | `0x169ad0` | `0x16a700` | +3120 | 1200 | 1200 | 0 | 100.0% |  | Y | `_ZN11SpaceLoungeC2Ev`<br>SpaceLounge::SpaceLounge() |
| 3673 | `0x169f80` | `0x16abb0` | +3120 | 2256 | 2256 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge4initEv`<br>SpaceLounge::init() |
| 3674 | `0x16a850` | `0x16b480` | +3120 | 68 | 68 | 0 | 100.0% |  | Y | `_Z11ArrayRemoveIP5AgentEvT_R5ArrayIS2_E`<br>void ArrayRemove<Agent*>(Agent*, Array<Agent*>&) |
| 3675 | `0x16a894` | `0x16b4c4` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge13introFinishedEv`<br>SpaceLounge::introFinished() |
| 3676 | `0x16a89a` | `0x16b4ca` | +3120 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN11SpaceLoungeD1Ev`<br>SpaceLounge::~SpaceLounge() |
| 3677 | `0x16a89a` | `0x16b4ca` | +3120 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN11SpaceLoungeD2Ev`<br>SpaceLounge::~SpaceLounge() |
| 3678 | `0x16a9be` | `0x16b5ee` | +3120 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP5ArrayIP9ImagePartEEvRS0_IT_E`<br>void ArrayReleaseClasses<Array<ImagePart*>*>(Array<Array<ImagePart*>*>&) |
| 3679 | `0x16aa0e` | `0x16b63e` | +3120 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IP9ImagePartEED2Ev`<br>Array<Array<ImagePart*>*>::~Array() |
| 3680 | `0x16aa24` | `0x16b654` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIPS_IP9ImagePartEEC2Ev`<br>Array<Array<ImagePart*>*>::Array() |
| 3681 | `0x16aa40` | `0x16b670` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIP5ArrayIP9ImagePartEEvjRS0_IT_E`<br>void ArraySetLength<Array<ImagePart*>*>(unsigned int, Array<Array<ImagePart*>*>&) |
| 3682 | `0x16aa74` | `0x16b6a4` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge15setHangarUpdateEb`<br>SpaceLounge::setHangarUpdate(bool) |
| 3683 | `0x16aa7c` | `0x16b6ac` | +3120 | 14450 | 14450 | 0 | 97.4% |  |  | `_ZN11SpaceLounge9startChatEv`<br>SpaceLounge::startChat() |
| 3684 | `0x16e2ee` | `0x16ef1e` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge7refreshEv`<br>SpaceLounge::refresh() |
| 3685 | `0x16e2f0` | `0x16ef20` | +3120 | 8580 | 8580 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge10onKeyPressEi`<br>SpaceLounge::onKeyPress(int) |
| 3686 | `0x170478` | `0x1710a8` | +3120 | 704 | 704 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge21updateScreenPositionsEv`<br>SpaceLounge::updateScreenPositions() |
| 3687 | `0x170738` | `0x171368` | +3120 | 1204 | 1204 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge6updateEi`<br>SpaceLounge::update(int) |
| 3688 | `0x170bec` | `0x17181c` | +3120 | 332 | 332 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge4drawEv`<br>SpaceLounge::draw() |
| 3689 | `0x170d38` | `0x171968` | +3120 | 1948 | 1948 | 0 | 94.9% |  |  | `_ZN11SpaceLounge10drawLoungeEv`<br>SpaceLounge::drawLounge() |
| 3690 | `0x1714d4` | `0x172104` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge10draw3DShipEv`<br>SpaceLounge::draw3DShip() |
| 3691 | `0x1714e0` | `0x172110` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge17checkLocationModeEv`<br>SpaceLounge::checkLocationMode() |
| 3692 | `0x1714e4` | `0x172114` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge7mapModeEv`<br>SpaceLounge::mapMode() |
| 3693 | `0x1714ea` | `0x17211a` | +3120 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge8listModeEv`<br>SpaceLounge::listMode() |
| 3694 | `0x1714f8` | `0x172128` | +3120 | 644 | 644 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge10getSoundIdEP5Agent`<br>SpaceLounge::getSoundId(Agent*) |
| 3695 | `0x17177c` | `0x1723ac` | +3120 | 116 | 116 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge23getSpecificSoundForRaceEiib`<br>SpaceLounge::getSpecificSoundForRace(int, int, bool) |
| 3696 | `0x1717f0` | `0x172420` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN11SpaceLounge17hangarNeedsUpdateEv`<br>SpaceLounge::hangarNeedsUpdate() |
| 3697 | `0x1717f8` | `0x172428` | +3120 | 304 | 304 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge12OnTouchBeginEii`<br>SpaceLounge::OnTouchBegin(int, int) |
| 3698 | `0x171928` | `0x172558` | +3120 | 288 | 288 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge11OnTouchMoveEii`<br>SpaceLounge::OnTouchMove(int, int) |
| 3699 | `0x171a48` | `0x172678` | +3120 | 7448 | 7448 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge10OnTouchEndEii`<br>SpaceLounge::OnTouchEnd(int, int) |
| 3700 | `0x173760` | `0x174390` | +3120 | 50 | 50 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge10OnRender3DEv`<br>SpaceLounge::OnRender3D() |
| 3701 | `0x173792` | `0x1743c2` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11SpaceLounge10OnRenderBGEv`<br>SpaceLounge::OnRenderBG() |
| 3702 | `0x1737a0` | `0x1743d0` | +3120 | 700 | 700 | 0 | 100.0% |  | Y | `_ZN10MeshMergerC1ERK5ArrayItES0_IN11AbyssEngine6AEMath6MatrixEEPNS4_11PaintCanvasEt`<br>MeshMerger::MeshMerger(Array<unsigned short> const&, Array<AbyssEngine::AEMath::Matrix>, AbyssEngine::PaintCanvas*, unsigned short) |
| 3703 | `0x1737a0` | `0x1743d0` | +3120 | 700 | 700 | 0 | 100.0% |  | Y | `_ZN10MeshMergerC2ERK5ArrayItES0_IN11AbyssEngine6AEMath6MatrixEEPNS4_11PaintCanvasEt`<br>MeshMerger::MeshMerger(Array<unsigned short> const&, Array<AbyssEngine::AEMath::Matrix>, AbyssEngine::PaintCanvas*, unsigned short) |
| 3704 | `0x173a60` | `0x174690` | +3120 | 408 | 408 | 0 | 100.0% |  | Y | `_ZN10MeshMergerC2EiiPN11AbyssEngine11PaintCanvasEt`<br>MeshMerger::MeshMerger(int, int, AbyssEngine::PaintCanvas*, unsigned short) |
| 3705 | `0x173a60` | `0x174690` | +3120 | 408 | 408 | 0 | 100.0% |  | Y | `_ZN10MeshMergerC1EiiPN11AbyssEngine11PaintCanvasEt`<br>MeshMerger::MeshMerger(int, int, AbyssEngine::PaintCanvas*, unsigned short) |
| 3706 | `0x173bf8` | `0x174828` | +3120 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN10MeshMerger7setMeshEiat`<br>MeshMerger::setMesh(int, signed char, unsigned short) |
| 3707 | `0x173c4c` | `0x17487c` | +3120 | 40 | 40 | 0 | 100.0% | Y | Y | `_ZN10MeshMerger6setLodEia`<br>MeshMerger::setLod(int, signed char) |
| 3708 | `0x173c74` | `0x1748a4` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN10MeshMerger9setMatrixEiRKN11AbyssEngine6AEMath6MatrixE`<br>MeshMerger::setMatrix(int, AbyssEngine::AEMath::Matrix const&) |
| 3709 | `0x173c84` | `0x1748b4` | +3120 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN10MeshMerger10setEnabledEib`<br>MeshMerger::setEnabled(int, bool) |
| 3710 | `0x173ca6` | `0x1748d6` | +3120 | 232 | 232 | 0 | 100.0% |  | Y | `_ZN10MeshMerger4initEv`<br>MeshMerger::init() |
| 3711 | `0x173d90` | `0x1749c0` | +3120 | 412 | 412 | 0 | 100.0% |  | Y | `_ZN10MeshMerger13transformMeshEPN11AbyssEngine4MeshERKNS0_6AEMath6MatrixE`<br>MeshMerger::transformMesh(AbyssEngine::Mesh*, AbyssEngine::AEMath::Matrix const&) |
| 3712 | `0x173f2c` | `0x174b5c` | +3120 | 320 | 320 | 0 | 100.0% |  | Y | `_ZN10MeshMerger6updateEv`<br>MeshMerger::update() |
| 3713 | `0x17406c` | `0x174c9c` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN10MeshMerger6renderEv`<br>MeshMerger::render() |
| 3714 | `0x174078` | `0x174ca8` | +3120 | 176 | 176 | 0 | 100.0% |  | Y | `_ZN10MeshMergerD1Ev`<br>MeshMerger::~MeshMerger() |
| 3715 | `0x174078` | `0x174ca8` | +3120 | 176 | 176 | 0 | 100.0% |  | Y | `_ZN10MeshMergerD2Ev`<br>MeshMerger::~MeshMerger() |
| 3716 | `0x174128` | `0x174d58` | +3120 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindowC1Eiiiib`<br>ScrollTouchWindow::ScrollTouchWindow(int, int, int, int, bool) |
| 3717 | `0x174128` | `0x174d58` | +3120 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindowC2Eiiiib`<br>ScrollTouchWindow::ScrollTouchWindow(int, int, int, int, bool) |
| 3718 | `0x1741c0` | `0x174df0` | +3120 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindowC1Eiiii`<br>ScrollTouchWindow::ScrollTouchWindow(int, int, int, int) |
| 3719 | `0x1741c0` | `0x174df0` | +3120 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindowC2Eiiii`<br>ScrollTouchWindow::ScrollTouchWindow(int, int, int, int) |
| 3720 | `0x174244` | `0x174e74` | +3120 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindowD2Ev`<br>ScrollTouchWindow::~ScrollTouchWindow() |
| 3721 | `0x174244` | `0x174e74` | +3120 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindowD1Ev`<br>ScrollTouchWindow::~ScrollTouchWindow() |
| 3722 | `0x174264` | `0x174e94` | +3120 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow7setTextEN11AbyssEngine6StringES1_`<br>ScrollTouchWindow::setText(AbyssEngine::String, AbyssEngine::String) |
| 3723 | `0x1742c4` | `0x174ef4` | +3120 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow7setTextEN11AbyssEngine6StringES1_i`<br>ScrollTouchWindow::setText(AbyssEngine::String, AbyssEngine::String, int) |
| 3724 | `0x174328` | `0x174f58` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow12setYPositionEi`<br>ScrollTouchWindow::setYPosition(int) |
| 3725 | `0x174330` | `0x174f60` | +3120 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow4drawEv`<br>ScrollTouchWindow::draw() |
| 3726 | `0x174484` | `0x1750b4` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow15setTextCenteredEb`<br>ScrollTouchWindow::setTextCentered(bool) |
| 3727 | `0x17448c` | `0x1750bc` | +3120 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow10drawTextBGEv`<br>ScrollTouchWindow::drawTextBG() |
| 3728 | `0x17454c` | `0x17517c` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow6updateEi`<br>ScrollTouchWindow::update(int) |
| 3729 | `0x174552` | `0x175182` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow12OnTouchBeginEii`<br>ScrollTouchWindow::OnTouchBegin(int, int) |
| 3730 | `0x174558` | `0x175188` | +3120 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow11OnTouchMoveEii`<br>ScrollTouchWindow::OnTouchMove(int, int) |
| 3731 | `0x17456a` | `0x17519a` | +3120 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN17ScrollTouchWindow10OnTouchEndEii`<br>ScrollTouchWindow::OnTouchEnd(int, int) |
| 3732 | `0x17457c` | `0x1751ac` | +3120 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN4ShipC1Eiiiiiiiif`<br>Ship::Ship(int, int, int, int, int, int, int, int, float) |
| 3733 | `0x17457c` | `0x1751ac` | +3120 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN4ShipC2Eiiiiiiiif`<br>Ship::Ship(int, int, int, int, int, int, int, int, float) |
| 3734 | `0x174608` | `0x175238` | +3120 | 924 | 924 | 0 | 100.0% |  | Y | `_ZN4Ship12refreshValueEv`<br>Ship::refreshValue() |
| 3735 | `0x1749a4` | `0x1755d4` | +3120 | 94 | 94 | 0 | 100.0% |  | Y | `_ZN4ShipD2Ev`<br>Ship::~Ship() |
| 3736 | `0x1749a4` | `0x1755d4` | +3120 | 94 | 94 | 0 | 100.0% |  | Y | `_ZN4ShipD1Ev`<br>Ship::~Ship() |
| 3737 | `0x174a02` | `0x175632` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship7setRaceEi`<br>Ship::setRace(int) |
| 3738 | `0x174a06` | `0x175636` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship7getRaceEv`<br>Ship::getRace() |
| 3739 | `0x174a0a` | `0x17563a` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship8getIndexEv`<br>Ship::getIndex() |
| 3740 | `0x174a0e` | `0x17563e` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship8getPriceEv`<br>Ship::getPrice() |
| 3741 | `0x174a12` | `0x175642` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship20setCurrentWeaponSlotEi`<br>Ship::setCurrentWeaponSlot(int) |
| 3742 | `0x174a16` | `0x175646` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship20getCurrentWeaponSlotEv`<br>Ship::getCurrentWeaponSlot() |
| 3743 | `0x174a1c` | `0x17564c` | +3120 | 76 | 76 | 0 | 100.0% | Y | Y | `_ZN4Ship18getHandlingForShopEv`<br>Ship::getHandlingForShop() |
| 3744 | `0x174a68` | `0x175698` | +3120 | 76 | 76 | 0 | 100.0% | Y | Y | `_ZN4Ship11getHandlingEv`<br>Ship::getHandling() |
| 3745 | `0x174ab4` | `0x1756e4` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship9getBaseHPEv`<br>Ship::getBaseHP() |
| 3746 | `0x174ab8` | `0x1756e8` | +3120 | 66 | 66 | 0 | 100.0% | Y | Y | `_ZN4Ship13getCombinedHPEv`<br>Ship::getCombinedHP() |
| 3747 | `0x174afa` | `0x17572a` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship13getMaxArmorHPEv`<br>Ship::getMaxArmorHP() |
| 3748 | `0x174afe` | `0x17572e` | +3120 | 58 | 58 | 0 | 100.0% | Y | Y | `_ZN4Ship8getMaxHPEv`<br>Ship::getMaxHP() |
| 3749 | `0x174b38` | `0x175768` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship14getMaxShieldHPEv`<br>Ship::getMaxShieldHP() |
| 3750 | `0x174b3c` | `0x17576c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship14getShieldRegenEv`<br>Ship::getShieldRegen() |
| 3751 | `0x174b40` | `0x175770` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship8getValueEv`<br>Ship::getValue() |
| 3752 | `0x174b44` | `0x175774` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship12getRadarTypeEv`<br>Ship::getRadarType() |
| 3753 | `0x174b48` | `0x175778` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship13getBoostSpeedEv`<br>Ship::getBoostSpeed() |
| 3754 | `0x174b4c` | `0x17577c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship12getBoostTimeEv`<br>Ship::getBoostTime() |
| 3755 | `0x174b50` | `0x175780` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship13getBoostDelayEv`<br>Ship::getBoostDelay() |
| 3756 | `0x174b54` | `0x175784` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship10getAgilityEv`<br>Ship::getAgility() |
| 3757 | `0x174b58` | `0x175788` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship17getFireRateFactorEv`<br>Ship::getFireRateFactor() |
| 3758 | `0x174b5c` | `0x17578c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship15getDamageFactorEv`<br>Ship::getDamageFactor() |
| 3759 | `0x174b60` | `0x175790` | +3120 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN4Ship19hasSecondaryWeaponsEv`<br>Ship::hasSecondaryWeapons() |
| 3760 | `0x174b9a` | `0x1757ca` | +3120 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN4Ship10hasBoosterEv`<br>Ship::hasBooster() |
| 3761 | `0x174ba6` | `0x1757d6` | +3120 | 38 | 38 | 0 | 100.0% | Y | Y | `_ZN4Ship12hasJumpDriveEv`<br>Ship::hasJumpDrive() |
| 3762 | `0x174bcc` | `0x1757fc` | +3120 | 28 | 28 | 0 | 100.0% | Y | Y | `_ZN4Ship22hasJumpDriveIntegratedEv`<br>Ship::hasJumpDriveIntegrated() |
| 3763 | `0x174be8` | `0x175818` | +3120 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN4Ship8hasCloakEv`<br>Ship::hasCloak() |
| 3764 | `0x174c0a` | `0x17583a` | +3120 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN4Ship18hasCloakIntegratedEv`<br>Ship::hasCloakIntegrated() |
| 3765 | `0x174c22` | `0x175852` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN4Ship18hasEmergencySystemEv`<br>Ship::hasEmergencySystem() |
| 3766 | `0x174c28` | `0x175858` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship13getRepairTypeEv`<br>Ship::getRepairType() |
| 3767 | `0x174c2c` | `0x17585c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship16getSignatureRaceEv`<br>Ship::getSignatureRace() |
| 3768 | `0x174c30` | `0x175860` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship12getFirePowerEv`<br>Ship::getFirePower() |
| 3769 | `0x174c34` | `0x175864` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship11getBaseLoadEv`<br>Ship::getBaseLoad() |
| 3770 | `0x174c38` | `0x175868` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship12getCargoPlusEv`<br>Ship::getCargoPlus() |
| 3771 | `0x174c3c` | `0x17586c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship14getCompressionEv`<br>Ship::getCompression() |
| 3772 | `0x174c40` | `0x175870` | +3120 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN4Ship10getMaxLoadEv`<br>Ship::getMaxLoad() |
| 3773 | `0x174c48` | `0x175878` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship14getCurrentLoadEv`<br>Ship::getCurrentLoad() |
| 3774 | `0x174c4c` | `0x17587c` | +3120 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN4Ship10changeLoadEi`<br>Ship::changeLoad(int) |
| 3775 | `0x174c7c` | `0x1758ac` | +3120 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN4Ship12getFreeSpaceEv`<br>Ship::getFreeSpace() |
| 3776 | `0x174c88` | `0x1758b8` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship12getEquipmentEv`<br>Ship::getEquipment() |
| 3777 | `0x174c8c` | `0x1758bc` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship8getCargoEv`<br>Ship::getCargo() |
| 3778 | `0x174c90` | `0x1758c0` | +3120 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN4Ship12replaceCargoEP5ArrayIP4ItemE`<br>Ship::replaceCargo(Array<Item*>*) |
| 3779 | `0x174ce4` | `0x175914` | +3120 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN4Ship23getFirstEquipmentOfSortEi`<br>Ship::getFirstEquipmentOfSort(int) |
| 3780 | `0x174d44` | `0x175974` | +3120 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN4Ship12hasEquipmentEii`<br>Ship::hasEquipment(int, int) |
| 3781 | `0x174d90` | `0x1759c0` | +3120 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN4Ship16hasVolatileGoodsEv`<br>Ship::hasVolatileGoods() |
| 3782 | `0x174db2` | `0x1759e2` | +3120 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN4Ship8hasCargoEii`<br>Ship::hasCargo(int, int) |
| 3783 | `0x174dfe` | `0x175a2e` | +3120 | 58 | 58 | 0 | 100.0% |  | Y | `_ZN4Ship12hasCargoTypeEi`<br>Ship::hasCargoType(int) |
| 3784 | `0x174e38` | `0x175a68` | +3120 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN4Ship11removeCargoEP4Item`<br>Ship::removeCargo(Item*) |
| 3785 | `0x174e64` | `0x175a94` | +3120 | 118 | 118 | 0 | 100.0% |  | Y | `_ZN4Ship11removeCargoEii`<br>Ship::removeCargo(int, int) |
| 3786 | `0x174eda` | `0x175b0a` | +3120 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN4Ship14removeAllCargoEv`<br>Ship::removeAllCargo() |
| 3787 | `0x174ef4` | `0x175b24` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN4Ship11removeCargoEi`<br>Ship::removeCargo(int) |
| 3788 | `0x174f00` | `0x175b30` | +3120 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN4Ship8setCargoEP5ArrayIP4ItemE`<br>Ship::setCargo(Array<Item*>*) |
| 3789 | `0x174f58` | `0x175b88` | +3120 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN4Ship8getCargoEi`<br>Ship::getCargo(int) |
| 3790 | `0x174f96` | `0x175bc6` | +3120 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN4Ship14spaceAvailableEi`<br>Ship::spaceAvailable(int) |
| 3791 | `0x174faa` | `0x175bda` | +3120 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN4Ship8addCargoEP4Item`<br>Ship::addCargo(Item*) |
| 3792 | `0x174fe8` | `0x175c18` | +3120 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN4Ship8addCargoEP5ArrayIP4ItemE`<br>Ship::addCargo(Array<Item*>*) |
| 3793 | `0x175000` | `0x175c30` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN4Ship16replaceEquipmentEP5ArrayIP4ItemE`<br>Ship::replaceEquipment(Array<Item*>*) |
| 3794 | `0x175034` | `0x175c64` | +3120 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN4Ship15removeEquipmentEP4Item`<br>Ship::removeEquipment(Item*) |
| 3795 | `0x175072` | `0x175ca2` | +3120 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN4Ship12setEquipmentEP5ArrayIP4ItemE`<br>Ship::setEquipment(Array<Item*>*) |
| 3796 | `0x17509c` | `0x175ccc` | +3120 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN4Ship12setEquipmentEP4Item`<br>Ship::setEquipment(Item*) |
| 3797 | `0x1750cc` | `0x175cfc` | +3120 | 142 | 142 | 0 | 100.0% |  | Y | `_ZN4Ship12getEquipmentEi`<br>Ship::getEquipment(int) |
| 3798 | `0x17515a` | `0x175d8a` | +3120 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN4Ship10getSlotPosEP4Item`<br>Ship::getSlotPos(Item*) |
| 3799 | `0x1751ae` | `0x175dde` | +3120 | 86 | 86 | 0 | 100.0% |  | Y | `_ZN4Ship12addEquipmentEP4Item`<br>Ship::addEquipment(Item*) |
| 3800 | `0x175204` | `0x175e34` | +3120 | 108 | 108 | 0 | 100.0% |  | Y | `_ZN4Ship12setEquipmentEP4Itemi`<br>Ship::setEquipment(Item*, int) |
| 3801 | `0x175270` | `0x175ea0` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN4Ship13getCargoValueEv`<br>Ship::getCargoValue() |
| 3802 | `0x1752a4` | `0x175ed4` | +3120 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN4Ship17getEquipmentValueEv`<br>Ship::getEquipmentValue() |
| 3803 | `0x1752d4` | `0x175f04` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship16getMaxPassengersEv`<br>Ship::getMaxPassengers() |
| 3804 | `0x1752d8` | `0x175f08` | +3120 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN4Ship12freeAllSlotsEv`<br>Ship::freeAllSlots() |
| 3805 | `0x1752fa` | `0x175f2a` | +3120 | 74 | 74 | 0 | 100.0% |  | Y | `_ZN4Ship8freeSlotEP4Itemi`<br>Ship::freeSlot(Item*, int) |
| 3806 | `0x175344` | `0x175f74` | +3120 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN4Ship8freeSlotEP4Item`<br>Ship::freeSlot(Item*) |
| 3807 | `0x175388` | `0x175fb8` | +3120 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN4Ship8getSlotsEi`<br>Ship::getSlots(int) |
| 3808 | `0x175390` | `0x175fc0` | +3120 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN4Ship12getSlotTypesEv`<br>Ship::getSlotTypes() |
| 3809 | `0x1753aa` | `0x175fda` | +3120 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN4Ship12getUsedSlotsEi`<br>Ship::getUsedSlots(int) |
| 3810 | `0x1753e0` | `0x176010` | +3120 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN4Ship13slotAvailableEi`<br>Ship::slotAvailable(int) |
| 3811 | `0x17541e` | `0x17604e` | +3120 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN4Ship12getFreeSlotsEi`<br>Ship::getFreeSlots(int) |
| 3812 | `0x175430` | `0x176060` | +3120 | 18 | 18 | 0 | 100.0% |  | Y | `_ZN4Ship8makeShipEi`<br>Ship::makeShip(int) |
| 3813 | `0x175444` | `0x176074` | +3120 | 132 | 132 | 0 | 100.0% |  | Y | `_ZN4Ship5cloneEv`<br>Ship::clone() |
| 3814 | `0x1754c8` | `0x1760f8` | +3120 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN4Ship12priceDeclineEv`<br>Ship::priceDecline() |
| 3815 | `0x1754f8` | `0x176128` | +3120 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN4Ship11adjustPriceEv`<br>Ship::adjustPrice() |
| 3816 | `0x1755b8` | `0x1761e8` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship8setPriceEi`<br>Ship::setPrice(int) |
| 3817 | `0x1755bc` | `0x1761ec` | +3120 | 202 | 202 | 0 | 100.0% |  | Y | `_ZN4Ship6addModEi`<br>Ship::addMod(int) |
| 3818 | `0x175686` | `0x1762b6` | +3120 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN4Ship6equalsEPS_`<br>Ship::equals(Ship*) |
| 3819 | `0x175694` | `0x1762c4` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship22getNumAddedDeviceSlotsEv`<br>Ship::getNumAddedDeviceSlots() |
| 3820 | `0x175698` | `0x1762c8` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship19getUnmoddedHandlingEv`<br>Ship::getUnmoddedHandling() |
| 3821 | `0x17569c` | `0x1762cc` | +3120 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN4Ship13getModdedLoadEv`<br>Ship::getModdedLoad() |
| 3822 | `0x1756ca` | `0x1762fa` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN4Ship7getModsEv`<br>Ship::getMods() |
| 3823 | `0x1756ce` | `0x1762fe` | +3120 | 174 | 174 | 0 | 100.0% |  | Y | `_ZN4Ship7setModsEP5ArrayIiE`<br>Ship::setMods(Array<int>*) |
| 3824 | `0x17577c` | `0x1763ac` | +3120 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN4Ship15hasModInstalledEi`<br>Ship::hasModInstalled(int) |
| 3825 | `0x1757a0` | `0x1763d0` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11ModMainMenuC1Ev`<br>ModMainMenu::ModMainMenu() |
| 3826 | `0x1757a0` | `0x1763d0` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11ModMainMenuC2Ev`<br>ModMainMenu::ModMainMenu() |
| 3827 | `0x1757c4` | `0x1763f4` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11ModMainMenuD1Ev`<br>ModMainMenu::~ModMainMenu() |
| 3828 | `0x1757c4` | `0x1763f4` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN11ModMainMenuD2Ev`<br>ModMainMenu::~ModMainMenu() |
| 3829 | `0x1757e8` | `0x176418` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11ModMainMenuD0Ev`<br>ModMainMenu::~ModMainMenu() |
| 3830 | `0x1757f8` | `0x176428` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu8OnResumeEv`<br>ModMainMenu::OnResume() |
| 3831 | `0x17582c` | `0x17645c` | +3120 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu9OnSuspendEv`<br>ModMainMenu::OnSuspend() |
| 3832 | `0x175844` | `0x176474` | +3120 | 568 | 568 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu12OnInitializeEv`<br>ModMainMenu::OnInitialize() |
| 3833 | `0x175a7c` | `0x1766ac` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu12OnTouchBeginEiiPv`<br>ModMainMenu::OnTouchBegin(int, int, void*) |
| 3834 | `0x175a98` | `0x1766c8` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu11OnTouchMoveEiiPv`<br>ModMainMenu::OnTouchMove(int, int, void*) |
| 3835 | `0x175ab4` | `0x1766e4` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu10OnTouchEndEiiPv`<br>ModMainMenu::OnTouchEnd(int, int, void*) |
| 3836 | `0x175aec` | `0x17671c` | +3120 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu9OnReleaseEv`<br>ModMainMenu::OnRelease() |
| 3837 | `0x175b78` | `0x1767a8` | +3120 | 136 | 136 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu8OnUpdateEv`<br>ModMainMenu::OnUpdate() |
| 3838 | `0x175c00` | `0x176830` | +3120 | 452 | 452 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu10OnRender2DEv`<br>ModMainMenu::OnRender2D() |
| 3839 | `0x175dc4` | `0x1769f4` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN11ModMainMenu10OnRender3DEv`<br>ModMainMenu::OnRender3D() |
| 3840 | `0x175df8` | `0x176a28` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11ModMainMenu10OnKeyPressExx`<br>ModMainMenu::OnKeyPress(long long, long long) |
| 3841 | `0x175dfa` | `0x176a2a` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11ModMainMenu12OnKeyReleaseExx`<br>ModMainMenu::OnKeyRelease(long long, long long) |
| 3842 | `0x175dfc` | `0x176a2c` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11ModMainMenu12OnTouchBeginEii`<br>ModMainMenu::OnTouchBegin(int, int) |
| 3843 | `0x175dfe` | `0x176a2e` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11ModMainMenu11OnTouchMoveEii`<br>ModMainMenu::OnTouchMove(int, int) |
| 3844 | `0x175e00` | `0x176a30` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN11ModMainMenu10OnTouchEndEii`<br>ModMainMenu::OnTouchEnd(int, int) |
| 3845 | `0x175e02` | `0x176a32` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN11ModMainMenu17ShowLoadingScreenEv`<br>ModMainMenu::ShowLoadingScreen() |
| 3846 | `0x175e08` | `0x176a36` | +3118 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6GalaxyC2Ev`<br>Galaxy::Galaxy() |
| 3847 | `0x175e08` | `0x176a36` | +3118 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6GalaxyC1Ev`<br>Galaxy::Galaxy() |
| 3848 | `0x175e54` | `0x176a82` | +3118 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN6GalaxyD2Ev`<br>Galaxy::~Galaxy() |
| 3849 | `0x175e54` | `0x176a82` | +3118 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN6GalaxyD1Ev`<br>Galaxy::~Galaxy() |
| 3850 | `0x175e82` | `0x176ab0` | +3118 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN6Galaxy5resetEv`<br>Galaxy::reset() |
| 3851 | `0x175e94` | `0x176ac2` | +3118 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Galaxy10getSystemsEv`<br>Galaxy::getSystems() |
| 3852 | `0x175e98` | `0x176ac6` | +3118 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN6Galaxy9getSystemEi`<br>Galaxy::getSystem(int) |
| 3853 | `0x175ea8` | `0x176ad8` | +3120 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN6Galaxy10getStationEi`<br>Galaxy::getStation(int) |
| 3854 | `0x175ef4` | `0x176b24` | +3120 | 46 | 46 | 0 | 100.0% | Y | Y | `_ZN6Galaxy10setVisitedEPbi`<br>Galaxy::setVisited(bool*, int) |
| 3855 | `0x175f22` | `0x176b52` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN6Galaxy10getVisitedEv`<br>Galaxy::getVisited() |
| 3856 | `0x175f26` | `0x176b56` | +3120 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN6Galaxy12visitStationEi`<br>Galaxy::visitStation(int) |
| 3857 | `0x175f2e` | `0x176b5e` | +3120 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN6Galaxy18invDistancePercentEiiii`<br>Galaxy::invDistancePercent(int, int, int, int) |
| 3858 | `0x175f44` | `0x176b74` | +3120 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN6Galaxy15distancePercentEiiii`<br>Galaxy::distancePercent(int, int, int, int) |
| 3859 | `0x175f80` | `0x176bb0` | +3120 | 280 | 280 | 0 | 100.0% |  | Y | `_ZN6Galaxy8distanceEP11SolarSystemS1_`<br>Galaxy::distance(SolarSystem*, SolarSystem*) |
| 3860 | `0x176098` | `0x176cc8` | +3120 | 432 | 432 | 0 | 100.0% |  | Y | `_ZN6Galaxy24getAsteroidProbabilitiesEP7Station`<br>Galaxy::getAsteroidProbabilities(Station*) |
| 3861 | `0x176248` | `0x176e78` | +3120 | 388 | 388 | 0 | 100.0% |  | Y | `_ZN6Galaxy22getPlasmaProbabilitiesEP7Station`<br>Galaxy::getPlasmaProbabilities(Station*) |
| 3862 | `0x1763cc` | `0x176ffc` | +3120 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloudC1EiP21ParticleSystemManagerP10AEGeometryRKN11AbyssEngine6AEMath6VectorE`<br>PlayerGasCloud::PlayerGasCloud(int, ParticleSystemManager*, AEGeometry*, AbyssEngine::AEMath::Vector const&) |
| 3863 | `0x1763cc` | `0x176ffc` | +3120 | 340 | 340 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloudC2EiP21ParticleSystemManagerP10AEGeometryRKN11AbyssEngine6AEMath6VectorE`<br>PlayerGasCloud::PlayerGasCloud(int, ParticleSystemManager*, AEGeometry*, AbyssEngine::AEMath::Vector const&) |
| 3864 | `0x176520` | `0x177150` | +3120 | 200 | 200 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloudD1Ev`<br>PlayerGasCloud::~PlayerGasCloud() |
| 3865 | `0x176520` | `0x177150` | +3120 | 200 | 200 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloudD2Ev`<br>PlayerGasCloud::~PlayerGasCloud() |
| 3866 | `0x1765e8` | `0x177218` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloudD0Ev`<br>PlayerGasCloud::~PlayerGasCloud() |
| 3867 | `0x1765f8` | `0x177228` | +3120 | 22 | 22 | 0 | 100.0% | Y | Y | `_ZN14PlayerGasCloud15setSparkInSightEib`<br>PlayerGasCloud::setSparkInSight(int, bool) |
| 3868 | `0x176610` | `0x177240` | +3120 | 48 | 48 | 0 | 100.0% | Y | Y | `_ZN14PlayerGasCloud12isSparkAliveEi`<br>PlayerGasCloud::isSparkAlive(int) |
| 3869 | `0x176640` | `0x177270` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerGasCloud9getSparksEv`<br>PlayerGasCloud::getSparks() |
| 3870 | `0x176646` | `0x177276` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloud11getPositionEv`<br>PlayerGasCloud::getPosition() |
| 3871 | `0x176652` | `0x177282` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloud11setPositionERKN11AbyssEngine6AEMath6VectorE`<br>PlayerGasCloud::setPosition(AbyssEngine::AEMath::Vector const&) |
| 3872 | `0x176658` | `0x177288` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloud9translateERKN11AbyssEngine6AEMath6VectorE`<br>PlayerGasCloud::translate(AbyssEngine::AEMath::Vector const&) |
| 3873 | `0x176660` | `0x177290` | +3120 | 776 | 776 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloud7explodeEiN11AbyssEngine6AEMath6VectorEf`<br>PlayerGasCloud::explode(int, AbyssEngine::AEMath::Vector, float) |
| 3874 | `0x176968` | `0x177598` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIfEvT_R5ArrayIS0_E`<br>void ArrayAdd<float>(float, Array<float>&) |
| 3875 | `0x17698c` | `0x1775bc` | +3120 | 32 | 32 | 0 | 100.0% |  | Y | `_Z8ArrayAddIbEvT_R5ArrayIS0_E`<br>void ArrayAdd<bool>(bool, Array<bool>&) |
| 3876 | `0x1769ac` | `0x1775dc` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN14PlayerGasCloud11hasExplodedEv`<br>PlayerGasCloud::hasExploded() |
| 3877 | `0x1769b4` | `0x1775e4` | +3120 | 1124 | 1124 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloud6updateEi`<br>PlayerGasCloud::update(int) |
| 3878 | `0x176e18` | `0x177a48` | +3120 | 264 | 264 | 0 | 100.0% |  | Y | `_ZN14PlayerGasCloud6renderEv`<br>PlayerGasCloud::render() |
| 3879 | `0x176f20` | `0x177b50` | +3120 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN9BluePrintC2Ei`<br>BluePrint::BluePrint(int) |
| 3880 | `0x176f20` | `0x177b50` | +3120 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN9BluePrintC1Ei`<br>BluePrint::BluePrint(int) |
| 3881 | `0x176fdc` | `0x177c0c` | +3120 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN9BluePrintD2Ev`<br>BluePrint::~BluePrint() |
| 3882 | `0x176fdc` | `0x177c0c` | +3120 | 42 | 42 | 0 | 100.0% |  | Y | `_ZN9BluePrintD1Ev`<br>BluePrint::~BluePrint() |
| 3883 | `0x177008` | `0x177c38` | +3120 | 300 | 300 | 0 | 100.0% |  | Y | `_ZN9BluePrint7addItemEP4Itemii`<br>BluePrint::addItem(Item*, int, int) |
| 3884 | `0x177134` | `0x177d64` | +3120 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN9BluePrint17getIngredientListEv`<br>BluePrint::getIngredientList() |
| 3885 | `0x17714c` | `0x177d7c` | +3120 | 24 | 24 | 0 | 100.0% | Y | Y | `_ZN9BluePrint8completeEv`<br>BluePrint::complete() |
| 3886 | `0x177164` | `0x177d94` | +3120 | 32 | 32 | 0 | 100.0% | Y | Y | `_ZN9BluePrint11isCompletedEv`<br>BluePrint::isCompleted() |
| 3887 | `0x177184` | `0x177db4` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9BluePrint8getIndexEv`<br>BluePrint::getIndex() |
| 3888 | `0x177188` | `0x177db8` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9BluePrint13getMoneySpentEv`<br>BluePrint::getMoneySpent() |
| 3889 | `0x17718c` | `0x177dbc` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9BluePrint13setMoneySpentEi`<br>BluePrint::setMoneySpent(int) |
| 3890 | `0x177190` | `0x177dc0` | +3120 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN9BluePrint15getQuantityListEv`<br>BluePrint::getQuantityList() |
| 3891 | `0x1771a8` | `0x177dd8` | +3120 | 62 | 62 | 0 | 100.0% |  | Y | `_ZN9BluePrint14getTotalAmountEi`<br>BluePrint::getTotalAmount(int) |
| 3892 | `0x1771e6` | `0x177e16` | +3120 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN9BluePrint16getCurrentAmountEi`<br>BluePrint::getCurrentAmount(int) |
| 3893 | `0x177208` | `0x177e38` | +3120 | 48 | 48 | 0 | 100.0% |  | Y | `_ZN9BluePrint18getRemainingAmountEi`<br>BluePrint::getRemainingAmount(int) |
| 3894 | `0x177238` | `0x177e68` | +3120 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN9BluePrint17getCompletionRateEv`<br>BluePrint::getCompletionRate() |
| 3895 | `0x177298` | `0x177ec8` | +3120 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN9BluePrint19getIngredientsValueEv`<br>BluePrint::getIngredientsValue() |
| 3896 | `0x1772f0` | `0x177f20` | +3120 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN9BluePrint22getAutoCompletionPriceEv`<br>BluePrint::getAutoCompletionPrice() |
| 3897 | `0x177340` | `0x177f70` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9BluePrint15getBaseQuantityEv`<br>BluePrint::getBaseQuantity() |
| 3898 | `0x177344` | `0x177f74` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9BluePrint15getStationIndexEv`<br>BluePrint::getStationIndex() |
| 3899 | `0x177348` | `0x177f78` | +3120 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN9BluePrint14getStationNameEv`<br>BluePrint::getStationName() |
| 3900 | `0x177356` | `0x177f86` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9BluePrint11getQuantityEv`<br>BluePrint::getQuantity() |
| 3901 | `0x17735a` | `0x177f8a` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN9BluePrint10isUnlockedEv`<br>BluePrint::isUnlocked() |
| 3902 | `0x17735e` | `0x177f8e` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9BluePrint6unlockEv`<br>BluePrint::unlock() |
| 3903 | `0x177364` | `0x177f94` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN9BluePrint4lockEv`<br>BluePrint::lock() |
| 3904 | `0x17736a` | `0x177f9a` | +3120 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN9BluePrint7isEmptyEv`<br>BluePrint::isEmpty() |
| 3905 | `0x177374` | `0x177fa4` | +3120 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN9BluePrint5resetEv`<br>BluePrint::reset() |
| 3906 | `0x1773c4` | `0x177ff4` | +3120 | 242 | 242 | 0 | 100.0% |  | Y | `_ZN5AgentC1EiN11AbyssEngine6StringEiiibiiii`<br>Agent::Agent(int, AbyssEngine::String, int, int, int, bool, int, int, int, int) |
| 3907 | `0x1773c4` | `0x177ff4` | +3120 | 242 | 242 | 0 | 100.0% |  | Y | `_ZN5AgentC2EiN11AbyssEngine6StringEiiibiiii`<br>Agent::Agent(int, AbyssEngine::String, int, int, int, bool, int, int, int, int) |
| 3908 | `0x1774b8` | `0x1780e8` | +3120 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN5AgentD1Ev`<br>Agent::~Agent() |
| 3909 | `0x1774b8` | `0x1780e8` | +3120 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN5AgentD2Ev`<br>Agent::~Agent() |
| 3910 | `0x177500` | `0x178130` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent8getIndexEv`<br>Agent::getIndex() |
| 3911 | `0x177504` | `0x178134` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN5Agent7getNameEv`<br>Agent::getName() |
| 3912 | `0x177510` | `0x178140` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent7getTypeEv`<br>Agent::getType() |
| 3913 | `0x177514` | `0x178144` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent8getOfferEv`<br>Agent::getOffer() |
| 3914 | `0x177518` | `0x178148` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent7getRaceEv`<br>Agent::getRace() |
| 3915 | `0x17751c` | `0x17814c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent9getSystemEv`<br>Agent::getSystem() |
| 3916 | `0x177520` | `0x178150` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent10getStationEv`<br>Agent::getStation() |
| 3917 | `0x177524` | `0x178154` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent6isMaleEv`<br>Agent::isMale() |
| 3918 | `0x17752a` | `0x17815a` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent8getEventEv`<br>Agent::getEvent() |
| 3919 | `0x17752e` | `0x17815e` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent8setEventEi`<br>Agent::setEvent(int) |
| 3920 | `0x177532` | `0x178162` | +3120 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN5Agent9nextEventEv`<br>Agent::nextEvent() |
| 3921 | `0x17753a` | `0x17816a` | +3120 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN5Agent12isStoryAgentEv`<br>Agent::isStoryAgent() |
| 3922 | `0x177544` | `0x178174` | +3120 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN5Agent14isGenericAgentEv`<br>Agent::isGenericAgent() |
| 3923 | `0x177550` | `0x178180` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent13setImagePartsEPi`<br>Agent::setImageParts(int*) |
| 3924 | `0x177556` | `0x178186` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent13getImagePartsEv`<br>Agent::getImageParts() |
| 3925 | `0x17755c` | `0x17818c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent8setOfferEi`<br>Agent::setOffer(int) |
| 3926 | `0x177560` | `0x178190` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent10setMissionEP7Mission`<br>Agent::setMission(Mission*) |
| 3927 | `0x177566` | `0x178196` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent10getMissionEv`<br>Agent::getMission() |
| 3928 | `0x17756c` | `0x17819c` | +3120 | 12 | 12 | 0 | 100.0% | Y | Y | `_ZN5Agent7isKnownEv`<br>Agent::isKnown() |
| 3929 | `0x177578` | `0x1781a8` | +3120 | 204 | 204 | 0 | 100.0% |  | Y | `_ZN5Agent21setWingmanFriendNamesEP5ArrayIPN11AbyssEngine6StringEE`<br>Agent::setWingmanFriendNames(Array<AbyssEngine::String*>*) |
| 3930 | `0x177644` | `0x178274` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent18getSellSystemIndexEv`<br>Agent::getSellSystemIndex() |
| 3931 | `0x177648` | `0x178278` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent21getSellBlueprintIndexEv`<br>Agent::getSellBlueprintIndex() |
| 3932 | `0x17764c` | `0x17827c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent8setCostsEi`<br>Agent::setCosts(int) |
| 3933 | `0x177650` | `0x178280` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent8getCostsEv`<br>Agent::getCosts() |
| 3934 | `0x177654` | `0x178284` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent22getWingmanFriendsCountEv`<br>Agent::getWingmanFriendsCount() |
| 3935 | `0x177658` | `0x178288` | +3120 | 24 | 24 | 0 | 100.0% |  | Y | `_ZN5Agent14getWingmanNameEi`<br>Agent::getWingmanName(int) |
| 3936 | `0x177670` | `0x1782a0` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent15getWingmanNamesEv`<br>Agent::getWingmanNames() |
| 3937 | `0x177676` | `0x1782a6` | +3120 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN5Agent16getMissionStringEv`<br>Agent::getMissionString() |
| 3938 | `0x177684` | `0x1782b4` | +3120 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN5Agent16setMissionStringEN11AbyssEngine6StringE`<br>Agent::setMissionString(AbyssEngine::String) |
| 3939 | `0x1776dc` | `0x17830c` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent16getSellItemIndexEv`<br>Agent::getSellItemIndex() |
| 3940 | `0x1776e0` | `0x178310` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent19getSellItemQuantityEv`<br>Agent::getSellItemQuantity() |
| 3941 | `0x1776e4` | `0x178314` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent16getSellItemPriceEv`<br>Agent::getSellItemPrice() |
| 3942 | `0x1776e8` | `0x178318` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5Agent16setSellItemPriceEi`<br>Agent::setSellItemPrice(int) |
| 3943 | `0x1776ec` | `0x17831c` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent15setSellItemDataEiii`<br>Agent::setSellItemData(int, int, int) |
| 3944 | `0x1776f2` | `0x178322` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent16hasAcceptedOfferEv`<br>Agent::hasAcceptedOffer() |
| 3945 | `0x1776f8` | `0x178328` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent16setOfferAcceptedEb`<br>Agent::setOfferAccepted(bool) |
| 3946 | `0x1776fe` | `0x17832e` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent9hasRewardEv`<br>Agent::hasReward() |
| 3947 | `0x177704` | `0x178334` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent20giveRewardAtNextChatEb`<br>Agent::giveRewardAtNextChat(bool) |
| 3948 | `0x17770a` | `0x17833a` | +3120 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN5Agent14getStationNameEv`<br>Agent::getStationName() |
| 3949 | `0x177718` | `0x178348` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN5Agent14setStationNameEN11AbyssEngine6StringE`<br>Agent::setStationName(AbyssEngine::String) |
| 3950 | `0x17771e` | `0x17834e` | +3120 | 14 | 14 | 0 | 100.0% |  | Y | `_ZN5Agent13getSystemNameEv`<br>Agent::getSystemName() |
| 3951 | `0x17772c` | `0x17835c` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN5Agent13setSystemNameEN11AbyssEngine6StringE`<br>Agent::setSystemName(AbyssEngine::String) |
| 3952 | `0x177732` | `0x178362` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN5Agent15getSellModIndexEv`<br>Agent::getSellModIndex() |
| 3953 | `0x177738` | `0x178368` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5Agent21getModPricePercentageEv`<br>Agent::getModPricePercentage() |
| 3954 | `0x177754` | `0x178384` | +3120 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN7BeamGunC1EiP3GuniP5Level`<br>BeamGun::BeamGun(int, Gun*, int, Level*) |
| 3955 | `0x177754` | `0x178384` | +3120 | 224 | 224 | 0 | 100.0% |  | Y | `_ZN7BeamGunC2EiP3GuniP5Level`<br>BeamGun::BeamGun(int, Gun*, int, Level*) |
| 3956 | `0x177834` | `0x178464` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN7BeamGunD1Ev`<br>BeamGun::~BeamGun() |
| 3957 | `0x177834` | `0x178464` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN7BeamGunD2Ev`<br>BeamGun::~BeamGun() |
| 3958 | `0x17786c` | `0x17849c` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN7BeamGunD0Ev`<br>BeamGun::~BeamGun() |
| 3959 | `0x17787c` | `0x1784ac` | +3120 | 40 | 40 | 0 | 100.0% |  | Y | `_ZN7BeamGun6renderEv`<br>BeamGun::render() |
| 3960 | `0x1778a4` | `0x1784d4` | +3120 | 636 | 636 | 0 | 100.0% |  | Y | `_ZN7BeamGun6updateEi`<br>BeamGun::update(int) |
| 3961 | `0x177b20` | `0x178750` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN7BeamGun10setEnemiesEP5ArrayIP6PlayerE`<br>BeamGun::setEnemies(Array<Player*>*) |
| 3962 | `0x177b26` | `0x178756` | +3120 | 6 | 6 | 0 | 100.0% |  | Y | `_ZN7BeamGun8setEnemyEP6Player`<br>BeamGun::setEnemy(Player*) |
| 3963 | `0x177b2c` | `0x17875c` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN7BeamGun9translateERKN11AbyssEngine6AEMath6VectorE`<br>BeamGun::translate(AbyssEngine::AEMath::Vector const&) |
| 3964 | `0x177b2e` | `0x17875e` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN7BeamGun10replaceGunEji`<br>BeamGun::replaceGun(unsigned int, int) |
| 3965 | `0x177b30` | `0x178760` | +3120 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN5MGameC2Ev`<br>MGame::MGame() |
| 3966 | `0x177b30` | `0x178760` | +3120 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN5MGameC1Ev`<br>MGame::MGame() |
| 3967 | `0x177c08` | `0x178838` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN5MGameD2Ev`<br>MGame::~MGame() |
| 3968 | `0x177c08` | `0x178838` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN5MGameD1Ev`<br>MGame::~MGame() |
| 3969 | `0x177c40` | `0x178870` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN5MGameD0Ev`<br>MGame::~MGame() |
| 3970 | `0x177c50` | `0x178880` | +3120 | 2080 | 2080 | 0 | 100.0% |  | Y | `_ZN5MGame12OnInitializeEv`<br>MGame::OnInitialize() |
| 3971 | `0x178470` | `0x1790a0` | +3120 | 676 | 676 | 0 | 100.0% |  | Y | `_ZN5MGame5resetEv`<br>MGame::reset() |
| 3972 | `0x178714` | `0x179344` | +3120 | 564 | 564 | 0 | 100.0% |  | Y | `_ZN5MGame19handleAccelerometerEv`<br>MGame::handleAccelerometer() |
| 3973 | `0x178948` | `0x179578` | +3120 | 18 | 18 | 0 | 100.0% | Y | Y | `_ZN5MGame18maneuverTouchBeginEiiPv`<br>MGame::maneuverTouchBegin(int, int, void*) |
| 3974 | `0x17895c` | `0x17958c` | +3120 | 88 | 88 | 0 | 100.0% |  | Y | `_ZN5MGame17maneuverTouchMoveEiiPv`<br>MGame::maneuverTouchMove(int, int, void*) |
| 3975 | `0x1789b4` | `0x1795e4` | +3120 | 116 | 116 | 0 | 100.0% |  | Y | `_ZN5MGame16maneuverTouchEndEiiPv`<br>MGame::maneuverTouchEnd(int, int, void*) |
| 3976 | `0x178a28` | `0x179658` | +3120 | 208 | 208 | 0 | 100.0% |  | Y | `_ZN5MGame17freeCamTouchBeginEiiPv`<br>MGame::freeCamTouchBegin(int, int, void*) |
| 3977 | `0x178af8` | `0x179728` | +3120 | 568 | 568 | 0 | 100.0% |  | Y | `_ZN5MGame16freeCamTouchMoveEiiPv`<br>MGame::freeCamTouchMove(int, int, void*) |
| 3978 | `0x178d30` | `0x179960` | +3120 | 172 | 172 | 0 | 100.0% | Y | Y | `_ZN5MGame15freeCamTouchEndEiiPv`<br>MGame::freeCamTouchEnd(int, int, void*) |
| 3979 | `0x178ddc` | `0x179a0c` | +3120 | 72 | 72 | 0 | 100.0% |  | Y | `_ZN5MGame11pauseSoundsEv`<br>MGame::pauseSounds() |
| 3980 | `0x178e24` | `0x179a54` | +3120 | 64 | 64 | 0 | 100.0% |  | Y | `_ZN5MGame12resumeSoundsEv`<br>MGame::resumeSounds() |
| 3981 | `0x178e64` | `0x179a94` | +3120 | 2936 | 2936 | 0 | 100.0% |  | Y | `_ZN5MGame12OnTouchBeginEiiPv`<br>MGame::OnTouchBegin(int, int, void*) |
| 3982 | `0x1799dc` | `0x17a60c` | +3120 | 544 | 544 | 0 | 100.0% |  | Y | `_ZN5MGame11OnTouchMoveEiiPv`<br>MGame::OnTouchMove(int, int, void*) |
| 3983 | `0x179bfc` | `0x17a82c` | +3120 | 352 | 352 | 0 | 100.0% |  | Y | `_ZN5MGame8useCloakEv`<br>MGame::useCloak() |
| 3984 | `0x179d5c` | `0x17a98c` | +3120 | 596 | 596 | 0 | 100.0% |  | Y | `_ZN5MGame14UseKhadorDriveEv`<br>MGame::UseKhadorDrive() |
| 3985 | `0x179fb0` | `0x17abe0` | +3120 | 404 | 404 | 0 | 100.0% |  | Y | `_ZN5MGame22startChargingJumpDriveEv`<br>MGame::startChargingJumpDrive() |
| 3986 | `0x17a144` | `0x17ad74` | +3120 | 8080 | 8080 | 0 | 78.2% |  |  | `_ZN5MGame10OnTouchEndEiiPv`<br>MGame::OnTouchEnd(int, int, void*) |
| 3987 | `0x17c0d4` | `0x17cd04` | +3120 | 896 | 896 | 0 | 100.0% |  | Y | `_ZN5MGame14startJumpSceneEv`<br>MGame::startJumpScene() |
| 3988 | `0x17c454` | `0x17d084` | +3120 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN5MGame16setCinematicModeEb`<br>MGame::setCinematicMode(bool) |
| 3989 | `0x17c4e8` | `0x17d118` | +3120 | 316 | 316 | 0 | 100.0% |  | Y | `_ZN5MGame12switchCameraEi`<br>MGame::switchCamera(int) |
| 3990 | `0x17c624` | `0x17d254` | +3120 | 152 | 152 | 0 | 100.0% |  | Y | `_ZN5MGame9nextCamIdEi`<br>MGame::nextCamId(int) |
| 3991 | `0x17c6bc` | `0x17d2ec` | +3120 | 536 | 536 | 0 | 100.0% |  | Y | `_ZN5MGame9OnReleaseEv`<br>MGame::OnRelease() |
| 3992 | `0x17c8d4` | `0x17d504` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5MGame10OnKeyPressExx`<br>MGame::OnKeyPress(long long, long long) |
| 3993 | `0x17c8d8` | `0x17d508` | +3120 | 11440 | 11440 | 0 | 99.7% |  |  | `_ZN5MGame8OnUpdateEv`<br>MGame::OnUpdate() |
| 3994 | `0x17f588` | `0x1801b8` | +3120 | 920 | 920 | 0 | 100.0% |  | Y | `_ZN5MGame15updateJumpSceneEv`<br>MGame::updateJumpScene() |
| 3995 | `0x17f920` | `0x180550` | +3120 | 1300 | 1300 | 0 | 100.0% |  | Y | `_ZN5MGame9dockEventEii`<br>MGame::dockEvent(int, int) |
| 3996 | `0x17fe34` | `0x180a64` | +3120 | 292 | 292 | 0 | 100.0% |  | Y | `_ZN5MGame13dialogueEventEv`<br>MGame::dialogueEvent() |
| 3997 | `0x17ff58` | `0x180b88` | +3120 | 1468 | 1468 | 0 | 100.0% |  | Y | `_ZN5MGame12successCheckEv`<br>MGame::successCheck() |
| 3998 | `0x180514` | `0x181144` | +3120 | 696 | 696 | 0 | 100.0% |  | Y | `_ZN5MGame13gameOverCheckEv`<br>MGame::gameOverCheck() |
| 3999 | `0x1807cc` | `0x1813fc` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_ZN5MGame8OnResumeEv`<br>MGame::OnResume() |
| 4000 | `0x180800` | `0x181430` | +3120 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN5MGame9OnSuspendEv`<br>MGame::OnSuspend() |
| 4001 | `0x1808c0` | `0x1814f0` | +3120 | 1292 | 1292 | 0 | 100.0% |  | Y | `_ZN5MGame10OnRender2DEv`<br>MGame::OnRender2D() |
| 4002 | `0x180dcc` | `0x1819fc` | +3120 | 240 | 240 | 0 | 100.0% |  | Y | `_ZN5MGame10OnRender3DEv`<br>MGame::OnRender3D() |
| 4003 | `0x180ebc` | `0x181aec` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5MGame5pauseEv`<br>MGame::pause() |
| 4004 | `0x180ebe` | `0x181aee` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5MGame14showLiteScreenEv`<br>MGame::showLiteScreen() |
| 4005 | `0x180ec0` | `0x181af0` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5MGame12OnKeyReleaseExx`<br>MGame::OnKeyRelease(long long, long long) |
| 4006 | `0x180ec2` | `0x181af2` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5MGame12OnTouchBeginEii`<br>MGame::OnTouchBegin(int, int) |
| 4007 | `0x180ec4` | `0x181af4` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5MGame11OnTouchMoveEii`<br>MGame::OnTouchMove(int, int) |
| 4008 | `0x180ec6` | `0x181af6` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN5MGame10OnTouchEndEii`<br>MGame::OnTouchEnd(int, int) |
| 4009 | `0x180ec8` | `0x181af8` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN5MGame17ShowLoadingScreenEv`<br>MGame::ShowLoadingScreen() |
| 4010 | `0x180ecc` | `0x181afc` | +3120 | 740 | 740 | 0 | 100.0% |  | Y | `_ZN16SimpleMeshMergerC2ERK5ArrayItES0_IN11AbyssEngine6AEMath6MatrixEEPNS4_11PaintCanvasEt`<br>SimpleMeshMerger::SimpleMeshMerger(Array<unsigned short> const&, Array<AbyssEngine::AEMath::Matrix>, AbyssEngine::PaintCanvas*, unsigned short) |
| 4011 | `0x180ecc` | `0x181afc` | +3120 | 740 | 740 | 0 | 100.0% |  | Y | `_ZN16SimpleMeshMergerC1ERK5ArrayItES0_IN11AbyssEngine6AEMath6MatrixEEPNS4_11PaintCanvasEt`<br>SimpleMeshMerger::SimpleMeshMerger(Array<unsigned short> const&, Array<AbyssEngine::AEMath::Matrix>, AbyssEngine::PaintCanvas*, unsigned short) |
| 4012 | `0x1811b0` | `0x181de0` | +3120 | 52 | 52 | 0 | 100.0% |  | Y | `_Z14ArraySetLengthIPN11AbyssEngine4MeshEEvjR5ArrayIT_E`<br>void ArraySetLength<AbyssEngine::Mesh*>(unsigned int, Array<AbyssEngine::Mesh*>&) |
| 4013 | `0x1811e4` | `0x181e14` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN11AMeshMerger6renderEv`<br>AMeshMerger::render() |
| 4014 | `0x1811f0` | `0x181e20` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AMeshMergerD2Ev`<br>AMeshMerger::~AMeshMerger() |
| 4015 | `0x1811f0` | `0x181e20` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN11AMeshMergerD1Ev`<br>AMeshMerger::~AMeshMerger() |
| 4016 | `0x181200` | `0x181e30` | +3120 | 428 | 428 | 0 | 100.0% |  | Y | `_ZN13LodMeshMergerC2EiiPN11AbyssEngine11PaintCanvasEt`<br>LodMeshMerger::LodMeshMerger(int, int, AbyssEngine::PaintCanvas*, unsigned short) |
| 4017 | `0x181200` | `0x181e30` | +3120 | 428 | 428 | 0 | 100.0% |  | Y | `_ZN13LodMeshMergerC1EiiPN11AbyssEngine11PaintCanvasEt`<br>LodMeshMerger::LodMeshMerger(int, int, AbyssEngine::PaintCanvas*, unsigned short) |
| 4018 | `0x1813ac` | `0x181fdc` | +3120 | 84 | 84 | 0 | 100.0% |  | Y | `_ZN13LodMeshMerger7setMeshEiat`<br>LodMeshMerger::setMesh(int, signed char, unsigned short) |
| 4019 | `0x181400` | `0x182030` | +3120 | 40 | 40 | 0 | 100.0% | Y | Y | `_ZN13LodMeshMerger6setLodEia`<br>LodMeshMerger::setLod(int, signed char) |
| 4020 | `0x181428` | `0x182058` | +3120 | 16 | 16 | 0 | 100.0% |  | Y | `_ZN13LodMeshMerger9setMatrixEiRKN11AbyssEngine6AEMath6MatrixE`<br>LodMeshMerger::setMatrix(int, AbyssEngine::AEMath::Matrix const&) |
| 4021 | `0x181438` | `0x182068` | +3120 | 34 | 34 | 0 | 100.0% | Y | Y | `_ZN13LodMeshMerger10setEnabledEib`<br>LodMeshMerger::setEnabled(int, bool) |
| 4022 | `0x18145a` | `0x18208a` | +3120 | 246 | 246 | 0 | 100.0% |  | Y | `_ZN13LodMeshMerger4initEv`<br>LodMeshMerger::init() |
| 4023 | `0x181550` | `0x182180` | +3120 | 508 | 508 | 0 | 100.0% |  | Y | `_ZN13LodMeshMerger13transformMeshEPN11AbyssEngine4MeshERKNS0_6AEMath6MatrixE`<br>LodMeshMerger::transformMesh(AbyssEngine::Mesh*, AbyssEngine::AEMath::Matrix const&) |
| 4024 | `0x18174c` | `0x18237c` | +3120 | 454 | 454 | 0 | 100.0% |  | Y | `_ZN13LodMeshMerger6updateEv`<br>LodMeshMerger::update() |
| 4025 | `0x181914` | `0x182544` | +3120 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN13LodMeshMergerD2Ev`<br>LodMeshMerger::~LodMeshMerger() |
| 4026 | `0x181914` | `0x182544` | +3120 | 172 | 172 | 0 | 100.0% |  | Y | `_ZN13LodMeshMergerD1Ev`<br>LodMeshMerger::~LodMeshMerger() |
| 4027 | `0x1819c0` | `0x1825f0` | +3120 | 280 | 280 | 0 | 100.0% |  | Y | `_ZN15IParticleSystemC2EPN11AbyssEngine11PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEbb`<br>IParticleSystem::IParticleSystem(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix const*, Array<ParticleSettings::ParticleSet> const&, bool, bool) |
| 4028 | `0x181ad8` | `0x182708` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIN16ParticleSettings11ParticleSetEEC2Ev`<br>Array<ParticleSettings::ParticleSet>::Array() |
| 4029 | `0x181af4` | `0x182724` | +3120 | 10 | 10 | 0 | 100.0% |  | Y | `_Z8ArraySetIN16ParticleSettings11ParticleSetEEvRK5ArrayIT_ERS4_`<br>void ArraySet<ParticleSettings::ParticleSet>(Array<ParticleSettings::ParticleSet> const&, Array<ParticleSettings::ParticleSet>&) |
| 4030 | `0x181afe` | `0x18272e` | +3120 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIN16ParticleSettings11ParticleSetEED2Ev`<br>Array<ParticleSettings::ParticleSet>::~Array() |
| 4031 | `0x181b14` | `0x182744` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem16getParticleCountEv`<br>IParticleSystem::getParticleCount() |
| 4032 | `0x181b18` | `0x182748` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem19setParticleSetIndexEh`<br>IParticleSystem::setParticleSetIndex(unsigned char) |
| 4033 | `0x181b1e` | `0x18274e` | +3120 | 20 | 20 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem14setParticleSetEN16ParticleSettings11ParticleSetE`<br>IParticleSystem::setParticleSet(ParticleSettings::ParticleSet) |
| 4034 | `0x181b34` | `0x182764` | +3120 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN15IParticleSystem20resetEmitterVelocityEv`<br>IParticleSystem::resetEmitterVelocity() |
| 4035 | `0x181b90` | `0x1827c0` | +3120 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN15IParticleSystem19calcEmitterVelocityEi`<br>IParticleSystem::calcEmitterVelocity(int) |
| 4036 | `0x181c1c` | `0x18284c` | +3120 | 312 | 312 | 0 | 100.0% |  | Y | `_ZN15IParticleSystem16interpolateColorEiRfS0_S0_S0_`<br>IParticleSystem::interpolateColor(int, float&, float&, float&, float&) |
| 4037 | `0x181d54` | `0x182984` | +3120 | 66 | 66 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem6updateEi`<br>IParticleSystem::update(int) |
| 4038 | `0x181d96` | `0x1829c6` | +3120 | 14 | 14 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem10enableEmitEb`<br>IParticleSystem::enableEmit(bool) |
| 4039 | `0x181da4` | `0x1829d4` | +3120 | 26 | 26 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem12enableRenderEb`<br>IParticleSystem::enableRender(bool) |
| 4040 | `0x181dbe` | `0x1829ee` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem12enableUpdateEb`<br>IParticleSystem::enableUpdate(bool) |
| 4041 | `0x181dc4` | `0x1829f4` | +3120 | 2368 | 2368 | 0 | 100.0% |  | Y | `_ZN15IParticleSystem4emitEi`<br>IParticleSystem::emit(int) |
| 4042 | `0x182704` | `0x183334` | +3120 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN15IParticleSystem9rotateUVsEPfiS0_`<br>IParticleSystem::rotateUVs(float*, int, float*) |
| 4043 | `0x182790` | `0x1833c0` | +3120 | 944 | 944 | 0 | 100.0% |  | Y | `_ZN15IParticleSystem10emitManualEN11AbyssEngine6AEMath6VectorEiPKS2_f`<br>IParticleSystem::emitManual(AbyssEngine::AEMath::Vector, int, AbyssEngine::AEMath::Vector const*, float) |
| 4044 | `0x182b40` | `0x183770` | +3120 | 10 | 10 | 0 | 100.0% | Y | Y | `_ZN15IParticleSystem9setMatrixEPKN11AbyssEngine6AEMath6MatrixE`<br>IParticleSystem::setMatrix(AbyssEngine::AEMath::Matrix const*) |
| 4045 | `0x182b4a` | `0x18377a` | +3120 | 60 | 60 | 0 | 100.0% |  | Y | `_Z8ArraySetIN16ParticleSettings11ParticleSetEEvPKT_jR5ArrayIS2_E`<br>void ArraySet<ParticleSettings::ParticleSet>(ParticleSettings::ParticleSet const*, unsigned int, Array<ParticleSettings::ParticleSet>&) |
| 4046 | `0x182b88` | `0x1837b8` | +3120 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSpriteC1EPN11AbyssEngine11PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEbb`<br>ParticleSystemSprite::ParticleSystemSprite(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix const*, Array<ParticleSettings::ParticleSet> const&, bool, bool) |
| 4047 | `0x182b88` | `0x1837b8` | +3120 | 148 | 148 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSpriteC2EPN11AbyssEngine11PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEbb`<br>ParticleSystemSprite::ParticleSystemSprite(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix const*, Array<ParticleSettings::ParticleSet> const&, bool, bool) |
| 4048 | `0x182c1c` | `0x18384c` | +3120 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN15IParticleSystemD2Ev`<br>IParticleSystem::~IParticleSystem() |
| 4049 | `0x182c48` | `0x183878` | +3120 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN20ParticleSystemSprite4initEjt`<br>ParticleSystemSprite::init(unsigned int, unsigned short) |
| 4050 | `0x182c58` | `0x183888` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSpriteD2Ev`<br>ParticleSystemSprite::~ParticleSystemSprite() |
| 4051 | `0x182c58` | `0x183888` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSpriteD1Ev`<br>ParticleSystemSprite::~ParticleSystemSprite() |
| 4052 | `0x182c7c` | `0x1838ac` | +3120 | 240 | 240 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite11setParticleERKN11AbyssEngine6AEMath6VectorEfjffffbffS4_`<br>ParticleSystemSprite::setParticle(AbyssEngine::AEMath::Vector const&, float, unsigned int, float, float, float, float, bool, float, float, AbyssEngine::AEMath::Vector const&) |
| 4053 | `0x182d6c` | `0x18399c` | +3120 | 700 | 700 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite22updateAreaExitParticleEif`<br>ParticleSystemSprite::updateAreaExitParticle(int, float) |
| 4054 | `0x183028` | `0x183c58` | +3120 | 140 | 140 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite8setAlphaEijf`<br>ParticleSystemSprite::setAlpha(int, unsigned int, float) |
| 4055 | `0x1830b4` | `0x183ce4` | +3120 | 680 | 680 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite12updateSingleEif`<br>ParticleSystemSprite::updateSingle(int, float) |
| 4056 | `0x18335c` | `0x183f8c` | +3120 | 92 | 92 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite5resetEv`<br>ParticleSystemSprite::reset() |
| 4057 | `0x1833b8` | `0x183fe8` | +3120 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite7releaseEv`<br>ParticleSystemSprite::release() |
| 4058 | `0x1833e0` | `0x184010` | +3120 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite6renderEPN11AbyssEngine11PaintCanvasEjjNS0_9BlendModeE`<br>ParticleSystemSprite::render(AbyssEngine::PaintCanvas*, unsigned int, unsigned int, AbyssEngine::BlendMode) |
| 4059 | `0x18347c` | `0x1840ac` | +3120 | 250 | 250 | 0 | 100.0% |  | Y | `_ZN20ParticleSystemSprite6renderEPN11AbyssEngine11PaintCanvasEj`<br>ParticleSystemSprite::render(AbyssEngine::PaintCanvas*, unsigned int) |
| 4060 | `0x183576` | `0x1841a6` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN20ParticleSystemSprite6enableEb`<br>ParticleSystemSprite::enable(bool) |
| 4061 | `0x183578` | `0x1841a8` | +3120 | 4 | 4 | 0 | 100.0% | Y | Y | `_ZN20ParticleSystemSprite12getQuadCountEv`<br>ParticleSystemSprite::getQuadCount() |
| 4062 | `0x18357c` | `0x1841ac` | +3120 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManagerC2EPN11AbyssEngine11PaintCanvasEN16ParticleSettings9CameraSetEtbtb`<br>ParticleSystemManager::ParticleSystemManager(AbyssEngine::PaintCanvas*, ParticleSettings::CameraSet, unsigned short, bool, unsigned short, bool) |
| 4063 | `0x18357c` | `0x1841ac` | +3120 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManagerC1EPN11AbyssEngine11PaintCanvasEN16ParticleSettings9CameraSetEtbtb`<br>ParticleSystemManager::ParticleSystemManager(AbyssEngine::PaintCanvas*, ParticleSettings::CameraSet, unsigned short, bool, unsigned short, bool) |
| 4064 | `0x1835e0` | `0x184210` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP20ParticleSystemSpriteEC2Ev`<br>Array<ParticleSystemSprite*>::Array() |
| 4065 | `0x1835fc` | `0x18422c` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN5ArrayIP18ParticleSystemMeshEC2Ev`<br>Array<ParticleSystemMesh*>::Array() |
| 4066 | `0x183618` | `0x184248` | +3120 | 30 | 30 | 0 | 100.0% | Y | Y | `_ZN21ParticleSystemManager9constructEv`<br>ParticleSystemManager::construct() |
| 4067 | `0x183636` | `0x184266` | +3120 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP18ParticleSystemMeshED2Ev`<br>Array<ParticleSystemMesh*>::~Array() |
| 4068 | `0x18364c` | `0x18427c` | +3120 | 22 | 22 | 0 | 100.0% |  | Y | `_ZN5ArrayIP20ParticleSystemSpriteED2Ev`<br>Array<ParticleSystemSprite*>::~Array() |
| 4069 | `0x183662` | `0x184292` | +3120 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManagerC2EPN11AbyssEngine11PaintCanvasEN16ParticleSettings9CameraSetEtNS0_9BlendModeEbtS5_b`<br>ParticleSystemManager::ParticleSystemManager(AbyssEngine::PaintCanvas*, ParticleSettings::CameraSet, unsigned short, AbyssEngine::BlendMode, bool, unsigned short, AbyssEngine::BlendMode, bool) |
| 4070 | `0x183662` | `0x184292` | +3120 | 100 | 100 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManagerC1EPN11AbyssEngine11PaintCanvasEN16ParticleSettings9CameraSetEtNS0_9BlendModeEbtS5_b`<br>ParticleSystemManager::ParticleSystemManager(AbyssEngine::PaintCanvas*, ParticleSettings::CameraSet, unsigned short, AbyssEngine::BlendMode, bool, unsigned short, AbyssEngine::BlendMode, bool) |
| 4071 | `0x1836c6` | `0x1842f6` | +3120 | 34 | 34 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager4initEv`<br>ParticleSystemManager::init() |
| 4072 | `0x1836e8` | `0x184318` | +3120 | 192 | 192 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager11initSpritesEv`<br>ParticleSystemManager::initSprites() |
| 4073 | `0x1837a8` | `0x1843d8` | +3120 | 190 | 190 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager8initMeshEv`<br>ParticleSystemManager::initMesh() |
| 4074 | `0x183866` | `0x184496` | +3120 | 188 | 188 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager6updateEx`<br>ParticleSystemManager::update(long long) |
| 4075 | `0x183924` | `0x184554` | +3120 | 180 | 180 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager9addSystemEPKN11AbyssEngine6AEMath6MatrixEN16ParticleSettings11ParticleSetEb`<br>ParticleSystemManager::addSystem(AbyssEngine::AEMath::Matrix const*, ParticleSettings::ParticleSet, bool) |
| 4076 | `0x1839d8` | `0x184608` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIN16ParticleSettings11ParticleSetEEvT_R5ArrayIS2_E`<br>void ArrayAdd<ParticleSettings::ParticleSet>(ParticleSettings::ParticleSet, Array<ParticleSettings::ParticleSet>&) |
| 4077 | `0x1839fc` | `0x18462c` | +3120 | 90 | 90 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager15addSpriteSystemEPKN11AbyssEngine6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEb`<br>ParticleSystemManager::addSpriteSystem(AbyssEngine::AEMath::Matrix const*, Array<ParticleSettings::ParticleSet> const&, bool) |
| 4078 | `0x183a56` | `0x184686` | +3120 | 96 | 96 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager13addMeshSystemEPKN11AbyssEngine6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEb`<br>ParticleSystemManager::addMeshSystem(AbyssEngine::AEMath::Matrix const*, Array<ParticleSettings::ParticleSet> const&, bool) |
| 4079 | `0x183ab8` | `0x1846e8` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager18enableSystemUpdateEib`<br>ParticleSystemManager::enableSystemUpdate(int, bool) |
| 4080 | `0x183adc` | `0x18470c` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP20ParticleSystemSpriteEvT_R5ArrayIS2_E`<br>void ArrayAdd<ParticleSystemSprite*>(ParticleSystemSprite*, Array<ParticleSystemSprite*>&) |
| 4081 | `0x183b00` | `0x184730` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_Z8ArrayAddIP18ParticleSystemMeshEvT_R5ArrayIS2_E`<br>void ArrayAdd<ParticleSystemMesh*>(ParticleSystemMesh*, Array<ParticleSystemMesh*>&) |
| 4082 | `0x183b24` | `0x184754` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager21setParticleSetByIndexEih`<br>ParticleSystemManager::setParticleSetByIndex(int, unsigned char) |
| 4083 | `0x183b48` | `0x184778` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager19setParticleSetBySetEiN16ParticleSettings11ParticleSetE`<br>ParticleSystemManager::setParticleSetBySet(int, ParticleSettings::ParticleSet) |
| 4084 | `0x183b6c` | `0x18479c` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager16enableSystemEmitEib`<br>ParticleSystemManager::enableSystemEmit(int, bool) |
| 4085 | `0x183b90` | `0x1847c0` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager18enableSystemRenderEib`<br>ParticleSystemManager::enableSystemRender(int, bool) |
| 4086 | `0x183bb4` | `0x1847e4` | +3120 | 36 | 36 | 0 | 100.0% | Y | Y | `_ZN21ParticleSystemManager11resetSystemEi`<br>ParticleSystemManager::resetSystem(int) |
| 4087 | `0x183bd8` | `0x184808` | +3120 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManagerD2Ev`<br>ParticleSystemManager::~ParticleSystemManager() |
| 4088 | `0x183bd8` | `0x184808` | +3120 | 54 | 54 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManagerD1Ev`<br>ParticleSystemManager::~ParticleSystemManager() |
| 4089 | `0x183c0e` | `0x18483e` | +3120 | 26 | 26 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager7releaseEv`<br>ParticleSystemManager::release() |
| 4090 | `0x183c28` | `0x184858` | +3120 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager12cameraToggleEN16ParticleSettings9CameraSetE`<br>ParticleSystemManager::cameraToggle(ParticleSettings::CameraSet) |
| 4091 | `0x183c48` | `0x184878` | +3120 | 32 | 32 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager14releaseSpritesEv`<br>ParticleSystemManager::releaseSprites() |
| 4092 | `0x183c68` | `0x184898` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager8render3dEv`<br>ParticleSystemManager::render3d() |
| 4093 | `0x183c8c` | `0x1848bc` | +3120 | 46 | 46 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager12renderMeshesEv`<br>ParticleSystemManager::renderMeshes() |
| 4094 | `0x183cba` | `0x1848ea` | +3120 | 44 | 44 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager13renderSpritesEv`<br>ParticleSystemManager::renderSprites() |
| 4095 | `0x183ce6` | `0x184916` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN21ParticleSystemManager12renderPost3dEv`<br>ParticleSystemManager::renderPost3d() |
| 4096 | `0x183ce8` | `0x184918` | +3120 | 60 | 60 | 0 | 100.0% | Y | Y | `_ZN21ParticleSystemManager5resetEv`<br>ParticleSystemManager::reset() |
| 4097 | `0x183d24` | `0x184954` | +3120 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP18ParticleSystemMeshEvR5ArrayIT_E`<br>void ArrayReleaseClasses<ParticleSystemMesh*>(Array<ParticleSystemMesh*>&) |
| 4098 | `0x183d74` | `0x1849a4` | +3120 | 80 | 80 | 0 | 100.0% |  | Y | `_Z19ArrayReleaseClassesIP20ParticleSystemSpriteEvR5ArrayIT_E`<br>void ArrayReleaseClasses<ParticleSystemSprite*>(Array<ParticleSystemSprite*>&) |
| 4099 | `0x183dc4` | `0x1849f4` | +3120 | 60 | 60 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager10emitManualEiRKN11AbyssEngine6AEMath6VectorEiS4_f`<br>ParticleSystemManager::emitManual(int, AbyssEngine::AEMath::Vector const&, int, AbyssEngine::AEMath::Vector const&, float) |
| 4100 | `0x183e00` | `0x184a30` | +3120 | 68 | 68 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager10emitManualEiRKN11AbyssEngine6AEMath6VectorEif`<br>ParticleSystemManager::emitManual(int, AbyssEngine::AEMath::Vector const&, int, float) |
| 4101 | `0x183e44` | `0x184a74` | +3120 | 36 | 36 | 0 | 100.0% |  | Y | `_ZN21ParticleSystemManager15systemSetMatrixEiPKN11AbyssEngine6AEMath6MatrixE`<br>ParticleSystemManager::systemSetMatrix(int, AbyssEngine::AEMath::Matrix const*) |
| 4102 | `0x183e68` | `0x184a98` | +3120 | 228 | 228 | 0 | 100.0% |  | Y | `_ZN16ParticleSettings11multiplyAllEf`<br>ParticleSettings::multiplyAll(float) |
| 4103 | `0x183f4c` | `0x184b7c` | +3120 | 156 | 156 | 0 | 100.0% |  | Y | `_ZN16ParticleSettings11InterpolateENS_11ParticleSetES0_fS0_`<br>ParticleSettings::Interpolate(ParticleSettings::ParticleSet, ParticleSettings::ParticleSet, float, ParticleSettings::ParticleSet) |
| 4104 | `0x183fe8` | `0x184c18` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN16ParticleSettingsC1Ev`<br>ParticleSettings::ParticleSettings() |
| 4105 | `0x183fe8` | `0x184c18` | +3120 | 56 | 56 | 0 | 100.0% |  | Y | `_ZN16ParticleSettingsC2Ev`<br>ParticleSettings::ParticleSettings() |
| 4106 | `0x184020` | `0x184c50` | +3120 | 5204 | 5204 | 0 | 100.0% |  | Y | `_ZN16ParticleSettings4initEv`<br>ParticleSettings::init() |
| 4107 | `0x185474` | `0x1860a4` | +3120 | 28 | 28 | 0 | 100.0% |  | Y | `_ZN16ParticleSettingsD2Ev`<br>ParticleSettings::~ParticleSettings() |
| 4108 | `0x185490` | `0x1860c0` | +3120 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMeshC1EPN11AbyssEngine11PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEbb`<br>ParticleSystemMesh::ParticleSystemMesh(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix const*, Array<ParticleSettings::ParticleSet> const&, bool, bool) |
| 4109 | `0x185490` | `0x1860c0` | +3120 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMeshC2EPN11AbyssEngine11PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEbb`<br>ParticleSystemMesh::ParticleSystemMesh(AbyssEngine::PaintCanvas*, AbyssEngine::AEMath::Matrix const*, Array<ParticleSettings::ParticleSet> const&, bool, bool) |
| 4110 | `0x1855c4` | `0x1861f4` | +3120 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMeshD2Ev`<br>ParticleSystemMesh::~ParticleSystemMesh() |
| 4111 | `0x1855c4` | `0x1861f4` | +3120 | 20 | 20 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMeshD1Ev`<br>ParticleSystemMesh::~ParticleSystemMesh() |
| 4112 | `0x1855d8` | `0x186208` | +3120 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN18ParticleSystemMesh15startNewSectionEv`<br>ParticleSystemMesh::startNewSection() |
| 4113 | `0x1855e0` | `0x186210` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18ParticleSystemMesh20wasNewSectionStartedEv`<br>ParticleSystemMesh::wasNewSectionStarted() |
| 4114 | `0x1855e6` | `0x186216` | +3120 | 16 | 16 | 0 | 100.0% | Y | Y | `_ZN18ParticleSystemMesh5incIdEv`<br>ParticleSystemMesh::incId() |
| 4115 | `0x1855f6` | `0x186226` | +3120 | 8 | 8 | 0 | 100.0% | Y | Y | `_ZN18ParticleSystemMesh9getPrevIdEi`<br>ParticleSystemMesh::getPrevId(int) |
| 4116 | `0x1855fe` | `0x18622e` | +3120 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh26finishCurrentTrailParticleEN16ParticleSettings11ParticleSetEiRKN11AbyssEngine6AEMath6VectorES6_`<br>ParticleSystemMesh::finishCurrentTrailParticle(ParticleSettings::ParticleSet, int, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) |
| 4117 | `0x18564c` | `0x18627c` | +3120 | 1716 | 1716 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh9emitTrailEi`<br>ParticleSystemMesh::emitTrail(int) |
| 4118 | `0x185d00` | `0x186930` | +3120 | 908 | 908 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh11setParticleERKN11AbyssEngine6AEMath6VectorEfjffffbffS4_b`<br>ParticleSystemMesh::setParticle(AbyssEngine::AEMath::Vector const&, float, unsigned int, float, float, float, float, bool, float, float, AbyssEngine::AEMath::Vector const&, bool) |
| 4119 | `0x18608c` | `0x186cbc` | +3120 | 256 | 256 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh11setQuadEdgeERKN11AbyssEngine6AEMath6VectorEiS4_`<br>ParticleSystemMesh::setQuadEdge(AbyssEngine::AEMath::Vector const&, int, AbyssEngine::AEMath::Vector const&) |
| 4120 | `0x18618c` | `0x186dbc` | +3120 | 38 | 38 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh4emitEi`<br>ParticleSystemMesh::emit(int) |
| 4121 | `0x1861b2` | `0x186de2` | +3120 | 12 | 12 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh6renderEPN11AbyssEngine11PaintCanvasEj`<br>ParticleSystemMesh::render(AbyssEngine::PaintCanvas*, unsigned int) |
| 4122 | `0x1861be` | `0x186dee` | +3120 | 76 | 76 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh6renderEPN11AbyssEngine11PaintCanvasEjjNS0_9BlendModeE`<br>ParticleSystemMesh::render(AbyssEngine::PaintCanvas*, unsigned int, unsigned int, AbyssEngine::BlendMode) |
| 4123 | `0x18620a` | `0x186e3a` | +3120 | 94 | 94 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh5resetEv`<br>ParticleSystemMesh::reset() |
| 4124 | `0x186268` | `0x186e98` | +3120 | 308 | 308 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh4initEjt`<br>ParticleSystemMesh::init(unsigned int, unsigned short) |
| 4125 | `0x18639c` | `0x186fcc` | +3120 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZN18ParticleSystemMesh7releaseEv`<br>ParticleSystemMesh::release() |
| 4126 | `0x1863a0` | `0x186fd0` | +3120 | 572 | 572 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh17updateSingleColorEi`<br>ParticleSystemMesh::updateSingleColor(int) |
| 4127 | `0x1865dc` | `0x18720c` | +3120 | 216 | 216 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh16updateUsualEdgesEii`<br>ParticleSystemMesh::updateUsualEdges(int, int) |
| 4128 | `0x1866b4` | `0x1872e4` | +3120 | 364 | 364 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh16updateTrailEdgesEii`<br>ParticleSystemMesh::updateTrailEdges(int, int) |
| 4129 | `0x186820` | `0x187450` | +3120 | 80 | 80 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh11setParticleERKN11AbyssEngine6AEMath6VectorEfjffffbffS4_`<br>ParticleSystemMesh::setParticle(AbyssEngine::AEMath::Vector const&, float, unsigned int, float, float, float, float, bool, float, float, AbyssEngine::AEMath::Vector const&) |
| 4130 | `0x186870` | `0x1874a0` | +3120 | 380 | 380 | 0 | 100.0% |  | Y | `_ZN18ParticleSystemMesh12updateSingleEif`<br>ParticleSystemMesh::updateSingle(int, float) |
| 4131 | `0x1869ec` | `0x18761c` | +3120 | 6 | 6 | 0 | 100.0% | Y | Y | `_ZN18ParticleSystemMesh12getQuadCountEv`<br>ParticleSystemMesh::getQuadCount() |
| 4132 | `0x1869f4` | `0x187622` | +3118 | 64 | 64 | 0 | 100.0% | Y | Y | `inflateReset`<br>inflateReset |
| 4133 | `0x186a34` | `0x187662` | +3118 | 54 | 54 | 0 | 100.0% | Y | Y | `inflatePrime`<br>inflatePrime |
| 4134 | `0x186a6c` | `0x187698` | +3116 | 160 | 160 | 0 | 100.0% |  | Y | `inflateInit2_`<br>inflateInit2_ |
| 4135 | `0x186b0c` | `0x187738` | +3116 | 10 | 10 | 0 | 100.0% |  | Y | `inflateInit_`<br>inflateInit_ |
| 4136 | `0x186b18` | `0x187744` | +3116 | 4360 | 4360 | 0 | 100.0% |  | Y | `inflate`<br>inflate |
| 4137 | `0x187cd6` | `0x188902` | +3116 | 46 | 46 | 0 | 100.0% | Y | Y | `inflateEnd`<br>inflateEnd |
| 4138 | `0x187d04` | `0x188930` | +3116 | 138 | 138 | 0 | 100.0% |  | Y | `inflateSetDictionary`<br>inflateSetDictionary |
| 4139 | `0x187d8e` | `0x1889ba` | +3116 | 26 | 26 | 0 | 100.0% | Y | Y | `inflateGetHeader`<br>inflateGetHeader |
| 4140 | `0x187da8` | `0x1889d4` | +3116 | 236 | 236 | 0 | 100.0% |  | Y | `inflateSync`<br>inflateSync |
| 4141 | `0x187ede` | `0x188b0a` | +3116 | 30 | 30 | 0 | 100.0% | Y | Y | `inflateSyncPoint`<br>inflateSyncPoint |
| 4142 | `0x187efc` | `0x188b28` | +3116 | 206 | 206 | 0 | 100.0% |  | Y | `inflateCopy`<br>inflateCopy |
| 4143 | `0x187fcc` | `0x188bf8` | +3116 | 136 | 136 | 0 | 100.0% |  | Y | `compress2`<br>compress2 |
| 4144 | `0x188054` | `0x188c80` | +3116 | 20 | 20 | 0 | 100.0% |  | Y | `compress`<br>compress |
| 4145 | `0x188068` | `0x188c94` | +3116 | 12 | 12 | 0 | 100.0% | Y | Y | `compressBound`<br>compressBound |
| 4146 | `0x188074` | `0x188ca0` | +3116 | 32 | 32 | 0 | 100.0% |  | Y | `deflateInit_`<br>deflateInit_ |
| 4147 | `0x188094` | `0x188cc0` | +3116 | 416 | 416 | 0 | 100.0% |  | Y | `deflateInit2_`<br>deflateInit2_ |
| 4148 | `0x188234` | `0x188e60` | +3116 | 152 | 152 | 0 | 100.0% | Y | Y | `deflateEnd`<br>deflateEnd |
| 4149 | `0x1882cc` | `0x188ef8` | +3116 | 192 | 192 | 0 | 100.0% |  | Y | `deflateReset`<br>deflateReset |
| 4150 | `0x18838c` | `0x188fb8` | +3116 | 194 | 194 | 0 | 100.0% |  | Y | `deflateSetDictionary`<br>deflateSetDictionary |
| 4151 | `0x18844e` | `0x18907a` | +3116 | 24 | 24 | 0 | 100.0% | Y | Y | `deflateSetHeader`<br>deflateSetHeader |
| 4152 | `0x188466` | `0x189092` | +3116 | 38 | 38 | 0 | 100.0% | Y | Y | `deflatePrime`<br>deflatePrime |
| 4153 | `0x18848c` | `0x1890b8` | +3116 | 148 | 148 | 0 | 100.0% |  | Y | `deflateParams`<br>deflateParams |
| 4154 | `0x188520` | `0x18914c` | +3116 | 1732 | 1732 | 0 | 100.0% |  | Y | `deflate`<br>deflate |
| 4155 | `0x188be4` | `0x189810` | +3116 | 28 | 28 | 0 | 100.0% | Y | Y | `deflateTune`<br>deflateTune |
| 4156 | `0x188c00` | `0x18982c` | +3116 | 40 | 40 | 0 | 100.0% |  | Y | `deflateBound`<br>deflateBound |
| 4157 | `0x188c8e` | `0x1898ba` | +3116 | 280 | 280 | 0 | 100.0% |  | Y | `deflateCopy`<br>deflateCopy |
| 4158 | `0x18989c` | `0x18a4c8` | +3116 | 12 | 12 | 0 | 100.0% |  | Y | `get_crc_table`<br>get_crc_table |
| 4159 | `0x1898a8` | `0x18a4d4` | +3116 | 644 | 644 | 0 | 100.0% |  | Y | `crc32`<br>crc32 |
| 4160 | `0x189b2c` | `0x18a758` | +3116 | 208 | 208 | 0 | 100.0% |  | Y | `crc32_combine`<br>crc32_combine |
| 4161 | `0x189c34` | `0x18a860` | +3116 | 148 | 148 | 0 | 100.0% |  | Y | `inflateBackInit_`<br>inflateBackInit_ |
| 4162 | `0x189cc8` | `0x18a8f4` | +3116 | 2568 | 2568 | 0 | 100.0% |  | Y | `inflateBack`<br>inflateBack |
| 4163 | `0x18a6d0` | `0x18b2fc` | +3116 | 32 | 32 | 0 | 100.0% | Y | Y | `inflateBackEnd`<br>inflateBackEnd |
| 4164 | `0x18a6f0` | `0x18b31c` | +3116 | 12 | 12 | 0 | 100.0% |  | Y | `zlibVersion`<br>zlibVersion |
| 4165 | `0x18a6fc` | `0x18b328` | +3116 | 4 | 4 | 0 | 100.0% | Y | Y | `zlibCompileFlags`<br>zlibCompileFlags |
| 4166 | `0x18a700` | `0x18b32c` | +3116 | 20 | 20 | 0 | 100.0% |  | Y | `zError`<br>zError |
| 4167 | `0x18a714` | `0x18b340` | +3116 | 8 | 8 | 0 | 100.0% |  | Y | `zcalloc`<br>zcalloc |
| 4168 | `0x18a71c` | `0x18b348` | +3116 | 6 | 6 | 0 | 100.0% |  | Y | `zcfree`<br>zcfree |
| 4169 | `0x18a724` | `0x18b350` | +3116 | 928 | 928 | 0 | 100.0% |  | Y | `inflate_table`<br>inflate_table |
| 4170 | `0x18aac4` | `0x18b6f0` | +3116 | 144 | 144 | 0 | 100.0% |  | Y | `uncompress`<br>uncompress |
| 4171 | `0x18ab54` | `0x18b780` | +3116 | 76 | 76 | 0 | 100.0% |  | Y | `_tr_init`<br>_tr_init |
| 4172 | `0x18ac00` | `0x18b82c` | +3116 | 248 | 248 | 0 | 100.0% | Y | Y | `_tr_stored_block`<br>_tr_stored_block |
| 4173 | `0x18acf8` | `0x18b924` | +3116 | 276 | 276 | 0 | 100.0% | Y | Y | `_tr_align`<br>_tr_align |
| 4174 | `0x18ae78` | `0x18baa4` | +3116 | 964 | 964 | 0 | 100.0% |  | Y | `_tr_flush_block`<br>_tr_flush_block |
| 4175 | `0x18b944` | `0x18c570` | +3116 | 172 | 172 | 0 | 100.0% |  | Y | `_tr_tally`<br>_tr_tally |
| 4176 | `0x18bf08` | `0x18cb34` | +3116 | 972 | 972 | 0 | 100.0% |  | Y | `inflate_fast`<br>inflate_fast |
| 4177 | `0x18c2d4` | `0x18cf00` | +3116 | 516 | 516 | 0 | 100.0% |  | Y | `adler32`<br>adler32 |
| 4178 | `0x18c4d8` | `0x18d104` | +3116 | 120 | 120 | 0 | 100.0% |  | Y | `adler32_combine`<br>adler32_combine |
| 4179 | `0x18c550` | `0x18d17c` | +3116 | 56 | 56 | 0 | 100.0% |  | Y | `_zip_memdup`<br>_zip_memdup |
| 4180 | `0x18c588` | `0x18d1b4` | +3116 | 36 | 36 | 0 | 100.0% |  | Y | `zip_add`<br>zip_add |
| 4181 | `0x18c5ac` | `0x18d1d8` | +3116 | 184 | 184 | 0 | 100.0% |  | Y | `_zip_filerange_crc`<br>_zip_filerange_crc |
| 4182 | `0x18c664` | `0x18d290` | +3116 | 8 | 8 | 0 | 100.0% |  | Y | `zip_get_name`<br>zip_get_name |
| 4183 | `0x18c66c` | `0x18d298` | +3116 | 90 | 90 | 0 | 100.0% |  | Y | `_zip_get_name`<br>_zip_get_name |
| 4184 | `0x18c6c8` | `0x18d2f2` | +3114 | 6 | 6 | 0 | 100.0% |  | Y | `zip_file_strerror`<br>zip_file_strerror |
| 4185 | `0x18c6d0` | `0x18d2f8` | +3112 | 476 | 476 | 0 | 100.0% |  | Y | `zip_fopen_index`<br>zip_fopen_index |
| 4186 | `0x18c8ac` | `0x18d4d4` | +3112 | 158 | 158 | 0 | 100.0% |  | Y | `_zip_file_fillbuf`<br>_zip_file_fillbuf |
| 4187 | `0x18c94c` | `0x18d574` | +3112 | 120 | 120 | 0 | 100.0% |  | Y | `zip_source_buffer`<br>zip_source_buffer |
| 4188 | `0x18ca4c` | `0x18d674` | +3112 | 6 | 6 | 0 | 100.0% |  | Y | `zip_error_clear`<br>zip_error_clear |
| 4189 | `0x18ca54` | `0x18d67a` | +3110 | 44 | 44 | 0 | 100.0% | Y | Y | `zip_get_archive_comment`<br>zip_get_archive_comment |
| 4190 | `0x18ca80` | `0x18d6a6` | +3110 | 52 | 52 | 0 | 100.0% |  | Y | `zip_source_file`<br>zip_source_file |
| 4191 | `0x18cab4` | `0x18d6da` | +3110 | 136 | 136 | 0 | 100.0% |  | Y | `_zip_set_name`<br>_zip_set_name |
| 4192 | `0x18cb3c` | `0x18d762` | +3110 | 6 | 6 | 0 | 100.0% |  | Y | `zip_file_error_get`<br>zip_file_error_get |
| 4193 | `0x18cb44` | `0x18d768` | +3108 | 144 | 144 | 0 | 100.0% |  | Y | `_zip_file_get_offset`<br>_zip_file_get_offset |
| 4194 | `0x18cbd4` | `0x18d7f8` | +3108 | 6 | 6 | 0 | 100.0% |  | Y | `zip_unchange`<br>zip_unchange |
| 4195 | `0x18cbda` | `0x18d7fe` | +3108 | 156 | 156 | 0 | 100.0% |  | Y | `_zip_unchange`<br>_zip_unchange |
| 4196 | `0x18cc78` | `0x18d89a` | +3106 | 74 | 74 | 0 | 100.0% |  | Y | `_zip_new`<br>_zip_new |
| 4197 | `0x18ccc4` | `0x18d8e4` | +3104 | 2532 | 2532 | 0 | 100.0% |  | Y | `zip_close`<br>zip_close |
| 4198 | `0x18d6f0` | `0x18e310` | +3104 | 156 | 156 | 0 | 100.0% |  | Y | `zip_add_dir`<br>zip_add_dir |
| 4199 | `0x18d78c` | `0x18e3ac` | +3104 | 44 | 44 | 0 | 100.0% |  | Y | `zip_error_get_sys_type`<br>zip_error_get_sys_type |
| 4200 | `0x18d7b8` | `0x18e3d8` | +3104 | 80 | 80 | 0 | 100.0% |  | Y | `zip_set_archive_comment`<br>zip_set_archive_comment |
| 4201 | `0x18d808` | `0x18e428` | +3104 | 8 | 8 | 0 | 100.0% |  | Y | `zip_name_locate`<br>zip_name_locate |
| 4202 | `0x18d810` | `0x18e430` | +3104 | 192 | 192 | 0 | 100.0% |  | Y | `_zip_name_locate`<br>_zip_name_locate |
| 4203 | `0x18d8d0` | `0x18e4f0` | +3104 | 30 | 30 | 0 | 100.0% |  | Y | `zip_unchange_archive`<br>zip_unchange_archive |
| 4204 | `0x18d8f0` | `0x18e50e` | +3102 | 32 | 32 | 0 | 100.0% |  | Y | `zip_source_free`<br>zip_source_free |
| 4205 | `0x18d910` | `0x18e52e` | +3102 | 12 | 12 | 0 | 100.0% | Y | Y | `zip_get_num_files`<br>zip_get_num_files |
| 4206 | `0x18d91c` | `0x18e53a` | +3102 | 54 | 54 | 0 | 100.0% |  | Y | `zip_source_filep`<br>zip_source_filep |
| 4207 | `0x18d954` | `0x18e570` | +3100 | 152 | 152 | 0 | 100.0% |  | Y | `_zip_source_file_or_p`<br>_zip_source_file_or_p |
| 4208 | `0x18db24` | `0x18e740` | +3100 | 8 | 8 | 0 | 100.0% | Y | Y | `_zip_error_clear`<br>_zip_error_clear |
| 4209 | `0x18db2c` | `0x18e748` | +3100 | 10 | 10 | 0 | 100.0% | Y | Y | `_zip_error_copy`<br>_zip_error_copy |
| 4210 | `0x18db36` | `0x18e752` | +3100 | 18 | 18 | 0 | 100.0% |  | Y | `_zip_error_fini`<br>_zip_error_fini |
| 4211 | `0x18db48` | `0x18e764` | +3100 | 34 | 34 | 0 | 100.0% |  | Y | `_zip_error_get`<br>_zip_error_get |
| 4212 | `0x18db6a` | `0x18e786` | +3100 | 10 | 10 | 0 | 100.0% | Y | Y | `_zip_error_init`<br>_zip_error_init |
| 4213 | `0x18db74` | `0x18e790` | +3100 | 8 | 8 | 0 | 100.0% | Y | Y | `_zip_error_set`<br>_zip_error_set |
| 4214 | `0x18db7c` | `0x18e798` | +3100 | 6 | 6 | 0 | 100.0% |  | Y | `zip_strerror`<br>zip_strerror |
| 4215 | `0x18db84` | `0x18e79e` | +3098 | 118 | 118 | 0 | 100.0% |  | Y | `_zip_entry_new`<br>_zip_entry_new |
| 4216 | `0x18dbfc` | `0x18e814` | +3096 | 20 | 20 | 0 | 100.0% | Y | Y | `zip_get_archive_flag`<br>zip_get_archive_flag |
| 4217 | `0x18dc10` | `0x18e828` | +3096 | 18 | 18 | 0 | 100.0% | Y | Y | `zip_set_archive_flag`<br>zip_set_archive_flag |
| 4218 | `0x18dc24` | `0x18e83a` | +3094 | 44 | 44 | 0 | 100.0% |  | Y | `_zip_unchange_data`<br>_zip_unchange_data |
| 4219 | `0x18dc50` | `0x18e868` | +3096 | 282 | 282 | 0 | 100.0% |  | Y | `zip_fread`<br>zip_fread |
| 4220 | `0x18dd6c` | `0x18e982` | +3094 | 128 | 128 | 0 | 100.0% |  | Y | `zip_rename`<br>zip_rename |
| 4221 | `0x18ddec` | `0x18ea04` | +3096 | 324 | 324 | 0 | 100.0% |  | Y | `zip_source_zip`<br>zip_source_zip |
| 4222 | `0x18e020` | `0x18ec38` | +3096 | 202 | 202 | 0 | 100.0% |  | Y | `zip_stat_index`<br>zip_stat_index |
| 4223 | `0x18e0ec` | `0x18ed02` | +3094 | 58 | 58 | 0 | 100.0% |  | Y | `zip_source_function`<br>zip_source_function |
| 4224 | `0x18e128` | `0x18ed3c` | +3092 | 48 | 48 | 0 | 100.0% |  | Y | `zip_replace`<br>zip_replace |
| 4225 | `0x18e158` | `0x18ed6c` | +3092 | 106 | 106 | 0 | 100.0% |  | Y | `_zip_replace`<br>_zip_replace |
| 4226 | `0x18e1c4` | `0x18edd6` | +3090 | 72 | 72 | 0 | 100.0% |  | Y | `zip_delete`<br>zip_delete |
| 4227 | `0x18e20c` | `0x18ee1e` | +3090 | 116 | 116 | 0 | 100.0% |  | Y | `zip_set_file_comment`<br>zip_set_file_comment |
| 4228 | `0x18e280` | `0x18ee92` | +3090 | 52 | 52 | 0 | 100.0% |  | Y | `zip_stat`<br>zip_stat |
| 4229 | `0x18e2b4` | `0x18eec6` | +3090 | 96 | 96 | 0 | 100.0% |  | Y | `zip_get_file_comment`<br>zip_get_file_comment |
| 4230 | `0x18e314` | `0x18ef28` | +3092 | 340 | 340 | 0 | 100.0% |  | Y | `_zip_mkstemp`<br>_zip_mkstemp |
| 4231 | `0x18e468` | `0x18f07c` | +3092 | 40 | 40 | 0 | 100.0% |  | Y | `_zip_entry_free`<br>_zip_entry_free |
| 4232 | `0x18e490` | `0x18f0a4` | +3092 | 1200 | 1200 | 0 | 100.0% |  | Y | `zip_open`<br>zip_open |
| 4233 | `0x18eb78` | `0x18f78c` | +3092 | 6 | 6 | 0 | 100.0% |  | Y | `zip_error_get`<br>zip_error_get |
| 4234 | `0x18eb80` | `0x18f792` | +3090 | 110 | 110 | 0 | 100.0% |  | Y | `zip_fclose`<br>zip_fclose |
| 4235 | `0x18ebf0` | `0x18f800` | +3088 | 76 | 76 | 0 | 100.0% |  | Y | `_zip_cdir_free`<br>_zip_cdir_free |
| 4236 | `0x18ec3c` | `0x18f84c` | +3088 | 48 | 48 | 0 | 100.0% |  | Y | `_zip_dirent_finalize`<br>_zip_dirent_finalize |
| 4237 | `0x18ec6c` | `0x18f87c` | +3088 | 68 | 68 | 0 | 100.0% |  | Y | `_zip_cdir_grow`<br>_zip_cdir_grow |
| 4238 | `0x18ecb0` | `0x18f8c0` | +3088 | 98 | 98 | 0 | 100.0% |  | Y | `_zip_cdir_new`<br>_zip_cdir_new |
| 4239 | `0x18ed14` | `0x18f924` | +3088 | 196 | 196 | 0 | 100.0% |  | Y | `_zip_cdir_write`<br>_zip_cdir_write |
| 4240 | `0x18edd8` | `0x18f9e8` | +3088 | 344 | 344 | 0 | 100.0% |  | Y | `_zip_dirent_write`<br>_zip_dirent_write |
| 4241 | `0x18ef86` | `0x18fb96` | +3088 | 42 | 42 | 0 | 100.0% | Y | Y | `_zip_dirent_init`<br>_zip_dirent_init |
| 4242 | `0x18efb0` | `0x18fbc0` | +3088 | 724 | 724 | 0 | 100.0% |  | Y | `_zip_dirent_read`<br>_zip_dirent_read |
| 4243 | `0x18f284` | `0x18fe94` | +3088 | 12 | 12 | 0 | 100.0% | Y | Y | `_zip_read2`<br>_zip_read2 |
| 4244 | `0x18f290` | `0x18fea0` | +3088 | 12 | 12 | 0 | 100.0% | Y | Y | `_zip_read4`<br>_zip_read4 |
| 4245 | `0x18f380` | `0x18ff90` | +3088 | 216 | 216 | 0 | 100.0% |  | Y | `_zip_dirent_torrent_normalize`<br>_zip_dirent_torrent_normalize |
| 4246 | `0x18f458` | `0x190068` | +3088 | 132 | 132 | 0 | 100.0% |  | Y | `_zip_free`<br>_zip_free |
| 4247 | `0x18f4dc` | `0x1900ec` | +3088 | 6 | 6 | 0 | 100.0% |  | Y | `zip_file_error_clear`<br>zip_file_error_clear |
| 4248 | `0x18f4e4` | `0x1900f4` | +3088 | 172 | 172 | 0 | 100.0% |  | Y | `zip_error_to_str`<br>zip_error_to_str |
| 4249 | `0x18f590` | `0x1901a0` | +3088 | 48 | 48 | 0 | 100.0% | Y | Y | `zip_stat_init`<br>zip_stat_init |
| 4250 | `0x18f5c0` | `0x192804` | +12868 | 34 | 34 | 0 | 100.0% |  | Y | `zip_fopen`<br>zip_fopen |
| 4251 | `0x18f5e4` | `0x192826` | +12866 | 50 | 50 | 0 | 100.0% |  | Y | `zip_unchange_all`<br>zip_unchange_all |
| 4252 | `0x18f618` | `0x192858` | +12864 | 280 | 280 | 0 | 100.0% |  | Y | `_zip_error_strerror`<br>_zip_error_strerror |
| 4253 | `0x18f730` | `0x1901d0` | +2720 | 132 | 132 | 0 | 100.0% |  | Y | `SHA224_Init`<br>SHA224_Init |
| 4254 | `0x18f7b4` | `0x190254` | +2720 | 132 | 132 | 0 | 100.0% |  | Y | `SHA256_Init`<br>SHA256_Init |
| 4255 | `0x18f838` | `0x1902d8` | +2720 | 272 | 272 | 0 | 100.0% |  | Y | `SHA256_Update`<br>SHA256_Update |
| 4256 | `0x18f948` | `0x1903e8` | +2720 | 4 | 4 | 0 | 100.0% |  | Y | `SHA224_Update`<br>SHA224_Update |
| 4257 | `0x18f94c` | `0x1903ec` | +2720 | 8 | 8 | 0 | 100.0% |  | Y | `SHA256_Transform`<br>SHA256_Transform |
| 4258 | `0x18f954` | `0x1903f4` | +2720 | 1048 | 1048 | 0 | 100.0% |  | Y | `SHA256_Final`<br>SHA256_Final |
| 4259 | `0x18fd6c` | `0x19080c` | +2720 | 100 | 100 | 0 | 100.0% |  | Y | `SHA224`<br>SHA224 |
| 4260 | `0x18fdd0` | `0x190870` | +2720 | 104 | 104 | 0 | 100.0% |  | Y | `SHA256`<br>SHA256 |
| 4261 | `0x18fe38` | `0x1908d8` | +2720 | 4 | 4 | 0 | 100.0% |  | Y | `SHA224_Final`<br>SHA224_Final |
| 4262 | `0x18fe40` | `0x192640` | +10240 | 28 | 28 | 0 | 100.0% | Y | Y | `OPENSSL_atomic_add`<br>OPENSSL_atomic_add |
| 4263 | `0x18fe5c` | `0x19265c` | +10240 | 84 | 84 | 0 | 100.0% | Y | Y | `OPENSSL_cleanse`<br>OPENSSL_cleanse |
| 4264 | `0x18feb0` | `0x1926b0` | +10240 | 56 | 56 | 0 | 100.0% | Y | Y | `CRYPTO_memcmp`<br>CRYPTO_memcmp |
| 4265 | `0x18ff00` | `0x192700` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `_armv7_neon_probe`<br>_armv7_neon_probe |
| 4266 | `0x18ff08` | `0x192708` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `_armv7_tick`<br>_armv7_tick |
| 4267 | `0x18ff10` | `0x192710` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `_armv8_aes_probe`<br>_armv8_aes_probe |
| 4268 | `0x18ff18` | `0x192718` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `_armv8_sha1_probe`<br>_armv8_sha1_probe |
| 4269 | `0x18ff20` | `0x192720` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `_armv8_sha256_probe`<br>_armv8_sha256_probe |
| 4270 | `0x18ff28` | `0x192728` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `_armv8_pmull_probe`<br>_armv8_pmull_probe |
| 4271 | `0x18ff30` | `0x192730` | +10240 | 88 | 88 | 0 | 100.0% | Y | Y | `OPENSSL_wipe_cpu`<br>OPENSSL_wipe_cpu |
| 4272 | `0x18ff88` | `0x192788` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `OPENSSL_instrument_bus`<br>OPENSSL_instrument_bus |
| 4273 | `0x18ff90` | `0x192790` | +10240 | 8 | 8 | 0 | 100.0% | Y | Y | `OPENSSL_instrument_bus2`<br>OPENSSL_instrument_bus2 |
| 4274 | `0x1900e0` | `0x190a00` | +2336 | 3576 | 3576 | 0 | 100.0% | Y | Y | `sha256_block_data_order`<br>sha256_block_data_order |
| 4275 | `0x190ef0` | `0x191810` | +2336 | 3056 | 3056 | 0 | 100.0% | Y | Y | `sha256_block_data_order_neon`<br>sha256_block_data_order_neon |
| 4276 | `0x191d20` | `0x192970` | +3152 | 0 | 0 | 0 | 100.0% | Y | Y | `__cxa_end_cleanup`<br>__cxa_end_cleanup |
| 4277 | `0x191d30` | `0x192980` | +3152 | 42 | 42 | 0 | 100.0% |  | Y | `__cxa_allocate_exception`<br>__cxa_allocate_exception |
| 4278 | `0x191d5a` | `0x1929aa` | +3152 | 16 | 16 | 0 | 100.0% |  | Y | `__cxa_free_exception`<br>__cxa_free_exception |
| 4279 | `0x191d6a` | `0x1929ba` | +3152 | 30 | 30 | 0 | 100.0% |  | Y | `__cxa_allocate_dependent_exception`<br>__cxa_allocate_dependent_exception |
| 4280 | `0x191d88` | `0x1929d8` | +3152 | 4 | 4 | 0 | 100.0% |  | Y | `__cxa_free_dependent_exception`<br>__cxa_free_dependent_exception |
| 4281 | `0x191d8c` | `0x1929dc` | +3152 | 92 | 92 | 0 | 100.0% |  | Y | `__cxa_throw`<br>__cxa_throw |
| 4282 | `0x191e12` | `0x192a62` | +3152 | 4 | 4 | 0 | 100.0% | Y | Y | `__cxa_get_exception_ptr`<br>__cxa_get_exception_ptr |
| 4283 | `0x191e18` | `0x192a68` | +3152 | 88 | 88 | 0 | 100.0% |  | Y | `__cxa_begin_cleanup`<br>__cxa_begin_cleanup |
| 4284 | `0x191ecc` | `0x192b1c` | +3152 | 112 | 112 | 0 | 100.0% |  | Y | `__cxa_begin_catch`<br>__cxa_begin_catch |
| 4285 | `0x191f3c` | `0x192b8c` | +3152 | 136 | 136 | 0 | 100.0% |  | Y | `__cxa_end_catch`<br>__cxa_end_catch |
| 4286 | `0x191fc4` | `0x192c14` | +3152 | 64 | 64 | 0 | 100.0% |  | Y | `__cxa_decrement_exception_refcount`<br>__cxa_decrement_exception_refcount |
| 4287 | `0x192004` | `0x192c54` | +3152 | 56 | 56 | 0 | 100.0% |  | Y | `__cxa_current_exception_type`<br>__cxa_current_exception_type |
| 4288 | `0x19203c` | `0x192c8c` | +3152 | 96 | 96 | 0 | 100.0% |  | Y | `__cxa_rethrow`<br>__cxa_rethrow |
| 4289 | `0x19209c` | `0x192cec` | +3152 | 28 | 28 | 0 | 100.0% | Y | Y | `__cxa_increment_exception_refcount`<br>__cxa_increment_exception_refcount |
| 4290 | `0x1920b8` | `0x192d08` | +3152 | 96 | 96 | 0 | 100.0% |  | Y | `__cxa_current_primary_exception`<br>__cxa_current_primary_exception |
| 4291 | `0x192118` | `0x192d68` | +3152 | 120 | 120 | 0 | 100.0% |  | Y | `__cxa_rethrow_primary_exception`<br>__cxa_rethrow_primary_exception |
| 4292 | `0x1921b4` | `0x192e04` | +3152 | 16 | 16 | 0 | 100.0% |  | Y | `__cxa_uncaught_exception`<br>__cxa_uncaught_exception |
| 4293 | `0x1921c4` | `0x192e14` | +3152 | 22 | 22 | 0 | 100.0% |  | Y | `__cxa_uncaught_exceptions`<br>__cxa_uncaught_exceptions |
| 4294 | `0x1921dc` | `0x192e2c` | +3152 | 72 | 72 | 0 | 100.0% |  | Y | `__cxa_get_globals`<br>__cxa_get_globals |
| 4295 | `0x192224` | `0x192e74` | +3152 | 52 | 52 | 0 | 100.0% |  | Y | `__cxa_get_globals_fast`<br>__cxa_get_globals_fast |
| 4296 | `0x1922a8` | `0x192ef8` | +3152 | 116 | 116 | 0 | 100.0% |  | Y | `__cxa_guard_acquire`<br>__cxa_guard_acquire |
| 4297 | `0x19231c` | `0x192f6c` | +3152 | 88 | 88 | 0 | 100.0% |  | Y | `__cxa_guard_release`<br>__cxa_guard_release |
| 4298 | `0x192374` | `0x192fc4` | +3152 | 88 | 88 | 0 | 100.0% |  | Y | `__cxa_guard_abort`<br>__cxa_guard_abort |
| 4299 | `0x1923cc` | `0x19301c` | +3152 | 20 | 20 | 0 | 100.0% |  | Y | `_ZSt14get_unexpectedv`<br>std::get_unexpected() |
| 4300 | `0x1923e0` | `0x193030` | +3152 | 84 | 84 | 0 | 100.0% |  | Y | `_ZSt9terminatev`<br>std::terminate() |
| 4301 | `0x192448` | `0x193098` | +3152 | 24 | 24 | 0 | 100.0% |  | Y | `_ZSt10unexpectedv`<br>std::unexpected() |
| 4302 | `0x192460` | `0x1930b0` | +3152 | 20 | 20 | 0 | 100.0% |  | Y | `_ZSt13get_terminatev`<br>std::get_terminate() |
| 4303 | `0x1924a8` | `0x1930f8` | +3152 | 36 | 36 | 0 | 100.0% |  | Y | `_ZSt15set_new_handlerPFvvE`<br>std::set_new_handler(void (*)()) |
| 4304 | `0x1924cc` | `0x19311c` | +3152 | 20 | 20 | 0 | 100.0% |  | Y | `_ZSt15get_new_handlerv`<br>std::get_new_handler() |
| 4305 | `0x1924e0` | `0x193130` | +3152 | 348 | 348 | 0 | 100.0% |  | Y | `__gxx_personality_v0`<br>__gxx_personality_v0 |
| 4306 | `0x192a14` | `0x193664` | +3152 | 408 | 408 | 0 | 100.0% |  | Y | `__cxa_call_unexpected`<br>__cxa_call_unexpected |
| 4307 | `0x192d68` | `0x1939b8` | +3152 | 16 | 16 | 0 | 100.0% |  | Y | `__cxa_pure_virtual`<br>__cxa_pure_virtual |
| 4308 | `0x192d78` | `0x1939c8` | +3152 | 16 | 16 | 0 | 100.0% |  | Y | `__cxa_deleted_virtual`<br>__cxa_deleted_virtual |
| 4309 | `0x19362c` | `0x194030` | +2564 | 208 | 208 | 0 | 100.0% |  | Y | `__dynamic_cast`<br>__dynamic_cast |
| 4310 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt9exceptionD1Ev`<br>std::exception::~exception() |
| 4311 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt16bad_array_lengthD2Ev`<br>std::bad_array_length::~bad_array_length() |
| 4312 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt9bad_allocD2Ev`<br>std::bad_alloc::~bad_alloc() |
| 4313 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt16bad_array_lengthD1Ev`<br>std::bad_array_length::~bad_array_length() |
| 4314 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt20bad_array_new_lengthD2Ev`<br>std::bad_array_new_length::~bad_array_new_length() |
| 4315 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt9bad_allocD1Ev`<br>std::bad_alloc::~bad_alloc() |
| 4316 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt20bad_array_new_lengthD1Ev`<br>std::bad_array_new_length::~bad_array_new_length() |
| 4317 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt13bad_exceptionD2Ev`<br>std::bad_exception::~bad_exception() |
| 4318 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt9exceptionD2Ev`<br>std::exception::~exception() |
| 4319 | `0x193c10` | `0x194612` | +2562 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt13bad_exceptionD1Ev`<br>std::bad_exception::~bad_exception() |
| 4320 | `0x193c12` | `0x194614` | +2562 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt9exceptionD0Ev`<br>std::exception::~exception() |
| 4321 | `0x193c18` | `0x194618` | +2560 | 12 | 12 | 0 | 100.0% |  | Y | `_ZNKSt9exception4whatEv`<br>std::exception::what() const |
| 4322 | `0x193c24` | `0x194624` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt13bad_exceptionD0Ev`<br>std::bad_exception::~bad_exception() |
| 4323 | `0x193c28` | `0x194628` | +2560 | 12 | 12 | 0 | 100.0% |  | Y | `_ZNKSt13bad_exception4whatEv`<br>std::bad_exception::what() const |
| 4324 | `0x193c34` | `0x194634` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt9bad_allocC2Ev`<br>std::bad_alloc::bad_alloc() |
| 4325 | `0x193c34` | `0x194634` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt9bad_allocC1Ev`<br>std::bad_alloc::bad_alloc() |
| 4326 | `0x193c44` | `0x194644` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt9bad_allocD0Ev`<br>std::bad_alloc::~bad_alloc() |
| 4327 | `0x193c48` | `0x194648` | +2560 | 12 | 12 | 0 | 100.0% |  | Y | `_ZNKSt9bad_alloc4whatEv`<br>std::bad_alloc::what() const |
| 4328 | `0x193c54` | `0x194654` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt20bad_array_new_lengthC2Ev`<br>std::bad_array_new_length::bad_array_new_length() |
| 4329 | `0x193c54` | `0x194654` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt20bad_array_new_lengthC1Ev`<br>std::bad_array_new_length::bad_array_new_length() |
| 4330 | `0x193c64` | `0x194664` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt20bad_array_new_lengthD0Ev`<br>std::bad_array_new_length::~bad_array_new_length() |
| 4331 | `0x193c68` | `0x194668` | +2560 | 12 | 12 | 0 | 100.0% |  | Y | `_ZNKSt20bad_array_new_length4whatEv`<br>std::bad_array_new_length::what() const |
| 4332 | `0x193c74` | `0x194674` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt16bad_array_lengthC2Ev`<br>std::bad_array_length::bad_array_length() |
| 4333 | `0x193c74` | `0x194674` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt16bad_array_lengthC1Ev`<br>std::bad_array_length::bad_array_length() |
| 4334 | `0x193c84` | `0x194684` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt16bad_array_lengthD0Ev`<br>std::bad_array_length::~bad_array_length() |
| 4335 | `0x193c88` | `0x194688` | +2560 | 12 | 12 | 0 | 100.0% |  | Y | `_ZNKSt16bad_array_length4whatEv`<br>std::bad_array_length::what() const |
| 4336 | `0x193c94` | `0x194694` | +2560 | 68 | 68 | 0 | 100.0% |  | Y | `_Znwj`<br>operator new(unsigned int) |
| 4337 | `0x193cd8` | `0x1946d8` | +2560 | 26 | 26 | 0 | 100.0% |  | Y | `_ZnwjRKSt9nothrow_t`<br>operator new(unsigned int, std::nothrow_t const&) |
| 4338 | `0x193cf2` | `0x1946f2` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_Znaj`<br>operator new[](unsigned int) |
| 4339 | `0x193cf6` | `0x1946f6` | +2560 | 26 | 26 | 0 | 100.0% |  | Y | `_ZnajRKSt9nothrow_t`<br>operator new[](unsigned int, std::nothrow_t const&) |
| 4340 | `0x193d10` | `0x194710` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdlPv`<br>operator delete(void*) |
| 4341 | `0x193d14` | `0x194714` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdlPvRKSt9nothrow_t`<br>operator delete(void*, std::nothrow_t const&) |
| 4342 | `0x193d18` | `0x194718` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdlPvj`<br>operator delete(void*, unsigned int) |
| 4343 | `0x193d1c` | `0x19471c` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdaPv`<br>operator delete[](void*) |
| 4344 | `0x193d20` | `0x194720` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdaPvRKSt9nothrow_t`<br>operator delete[](void*, std::nothrow_t const&) |
| 4345 | `0x193d24` | `0x194724` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdaPvj`<br>operator delete[](void*, unsigned int) |
| 4346 | `0x193d28` | `0x194728` | +2560 | 128 | 128 | 0 | 100.0% |  | Y | `_ZnwjSt11align_val_t`<br>operator new(unsigned int, std::align_val_t) |
| 4347 | `0x193da8` | `0x1947a8` | +2560 | 26 | 26 | 0 | 100.0% |  | Y | `_ZnwjSt11align_val_tRKSt9nothrow_t`<br>operator new(unsigned int, std::align_val_t, std::nothrow_t const&) |
| 4348 | `0x193dc2` | `0x1947c2` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZnajSt11align_val_t`<br>operator new[](unsigned int, std::align_val_t) |
| 4349 | `0x193dc6` | `0x1947c6` | +2560 | 26 | 26 | 0 | 100.0% |  | Y | `_ZnajSt11align_val_tRKSt9nothrow_t`<br>operator new[](unsigned int, std::align_val_t, std::nothrow_t const&) |
| 4350 | `0x193de0` | `0x1947e0` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdlPvSt11align_val_t`<br>operator delete(void*, std::align_val_t) |
| 4351 | `0x193de4` | `0x1947e4` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdlPvSt11align_val_tRKSt9nothrow_t`<br>operator delete(void*, std::align_val_t, std::nothrow_t const&) |
| 4352 | `0x193de8` | `0x1947e8` | +2560 | 6 | 6 | 0 | 100.0% |  | Y | `_ZdlPvjSt11align_val_t`<br>operator delete(void*, unsigned int, std::align_val_t) |
| 4353 | `0x193dee` | `0x1947ee` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdaPvSt11align_val_t`<br>operator delete[](void*, std::align_val_t) |
| 4354 | `0x193df2` | `0x1947f2` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZdaPvSt11align_val_tRKSt9nothrow_t`<br>operator delete[](void*, std::align_val_t, std::nothrow_t const&) |
| 4355 | `0x193df6` | `0x1947f6` | +2560 | 6 | 6 | 0 | 100.0% |  | Y | `_ZdaPvjSt11align_val_t`<br>operator delete[](void*, unsigned int, std::align_val_t) |
| 4356 | `0x193dfc` | `0x1947fc` | +2560 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt9type_infoD1Ev`<br>std::type_info::~type_info() |
| 4357 | `0x193dfc` | `0x1947fc` | +2560 | 2 | 2 | 0 | 100.0% | Y | Y | `_ZNSt9type_infoD2Ev`<br>std::type_info::~type_info() |
| 4358 | `0x193dfe` | `0x1947fe` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt9type_infoD0Ev`<br>std::type_info::~type_info() |
| 4359 | `0x193e04` | `0x194804` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt8bad_castC2Ev`<br>std::bad_cast::bad_cast() |
| 4360 | `0x193e04` | `0x194804` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt8bad_castC1Ev`<br>std::bad_cast::bad_cast() |
| 4361 | `0x193e14` | `0x194814` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt8bad_castD1Ev`<br>std::bad_cast::~bad_cast() |
| 4362 | `0x193e14` | `0x194814` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt8bad_castD2Ev`<br>std::bad_cast::~bad_cast() |
| 4363 | `0x193e18` | `0x194818` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt8bad_castD0Ev`<br>std::bad_cast::~bad_cast() |
| 4364 | `0x193e28` | `0x194828` | +2560 | 12 | 12 | 0 | 100.0% |  | Y | `_ZNKSt8bad_cast4whatEv`<br>std::bad_cast::what() const |
| 4365 | `0x193e34` | `0x194834` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt10bad_typeidC1Ev`<br>std::bad_typeid::bad_typeid() |
| 4366 | `0x193e34` | `0x194834` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt10bad_typeidC2Ev`<br>std::bad_typeid::bad_typeid() |
| 4367 | `0x193e44` | `0x194844` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt10bad_typeidD2Ev`<br>std::bad_typeid::~bad_typeid() |
| 4368 | `0x193e44` | `0x194844` | +2560 | 4 | 4 | 0 | 100.0% |  | Y | `_ZNSt10bad_typeidD1Ev`<br>std::bad_typeid::~bad_typeid() |
| 4369 | `0x193e48` | `0x194848` | +2560 | 16 | 16 | 0 | 100.0% |  | Y | `_ZNSt10bad_typeidD0Ev`<br>std::bad_typeid::~bad_typeid() |
| 4370 | `0x193e58` | `0x194858` | +2560 | 12 | 12 | 0 | 100.0% |  | Y | `_ZNKSt10bad_typeid4whatEv`<br>std::bad_typeid::what() const |
| 4371 | `0x193ec4` | `0x194864` | +2464 | 48 | 48 | 0 | 100.0% |  | Y | `_ZSt14set_unexpectedPFvvE`<br>std::set_unexpected(void (*)()) |
| 4372 | `0x193ef4` | `0x194894` | +2464 | 48 | 48 | 0 | 100.0% |  | Y | `_ZSt13set_terminatePFvvE`<br>std::set_terminate(void (*)()) |
| 4373 | `0x194030` | `0x1949d0` | +2464 | 568 | 568 | 0 | 100.0% |  | Y | `__cxa_demangle`<br>__cxa_demangle |

