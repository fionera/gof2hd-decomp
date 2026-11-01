# Local-function gap audit (w3306)

**Branch:** `research-w3306`, based on `master` `9b46ec31`
**Original:** `../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, `.dynsym` only)
**Ours:** `cmake-build-match/libgof2hdaa.so`
**Date:** 2026-11-01

## TL;DR

- Baseline gate at `9b46ec31`: `byte_exact 1458`, `linked_exact 4493`, size-exact
  `4522/4523`, `.text 0x1343d0` (original `.text 0x1337c8`). The task brief says
  `byte 1415`; the checked-in `tools/verify_baseline.json` and a fresh `check` both
  say **1458**. The numbers below are the measured ones.
- There are **66 delta-change points** between exported functions. The cumulative
  `ours - original` address delta ends at **+2464**.
- App-code drift is almost entirely **deliberate out-of-line layout anchors** added to
  our source (`mtwTextLayoutAnchor`, `modStationTouchEndPad`, `levelTextAnchor`,
  `is_equal`, `arabicFormOffset`, `ObjectGunProjectileMesh`,
  `starSystemLayoutAnchor`, `levelMissionPad`, `StarMap_GetScreenPosition`,
  `generatorMissionLayoutPad`). The original has no such local functions at those
  addresses.
- One helper, `__clang_call_terminate` (10 B), exists in both binaries but is placed
  in the wrong gap: ours after `AESoundRessource::D2Ev`, the original after
  `FileInterfaceAndroid::D2Ev`. Net final drift is 0, but it creates a `-10/+10`
  wiggle.
- The libzip/openssl region is not a local-body problem. The plain `.text` group
  (9804 B of libcrypto SHA/OPENSSL code) is inserted before `zip_fopen` by
  `PLAIN_TEXT_LEAD=0x180` in `tools/gen_section_order.py`. This creates the
  `+9804 / -10168 / +7544 / -7904 / +816` oscillation.
- The libc++abi local gap is 588 B smaller than the original's, and the
  `bad_typeid::what` gap is 96 B smaller. These come from the prebuilt libc++abi
  archive, not app source.
- **Safe fix found:** make `is_equal` in `src/game/mission/RecordHandler.cpp`
  `static` (or delete it). It is unused; clang drops it at `-Oz`. Measured:
  `byte_exact 1458 -> 1458`, `linked_exact 4493`, final delta `+2464 -> +2432`,
  `RATCHET PASS`. This is the only helper removal I found that costs no
  byte-exact functions.
- Removing the 2934-byte `mtwTextLayoutAnchor` reduces the final delta to
  **-480** but costs **59 byte_exact** (`1458 -> 1399`). The anchor is keeping
  later relocated literals at their byte-exact addresses.

## Method

### Build and gate

```
cmake --preset default
cmake --build cmake-build-match --target check -j32
```

Fresh baseline output:

```
compared 4523  avg 99.89%  byte_exact 1458  linked_exact 4493  fuzzy_perfect 4494
stub_zero_size 0  orig_zero_size 1  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 1458  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` confirms the only exported size mismatch is
`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`:
original 1996 B, ours 1992 B (`-4`). `getAppVersion` is the original's one
`orig_zero_size` symbol and is excluded from the 4523 scored functions.

### Delta walk

The walk reads `.dynsym` of both binaries, keeps every defined `STT_FUNC`
symbol in `.text`, matches by mangled name, sorts by **original** address, and
prints every point where `delta = ours_addr - orig_addr` changes. The gap is
`next_start - (prev_start + prev_size)` in each binary. The script used is:

```python
#!/usr/bin/env python3
import struct, sys

def load(path):
    d = open(path, 'rb').read()
    e_shoff = struct.unpack_from('<I', d, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', d, 0x2e)[0]
    e_shnum = struct.unpack_from('<H', d, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', d, 0x32)[0]
    secs = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        name, typ, flags, addr, offset, size, link, info, align, entsize = \
            struct.unpack_from('<IIIIIIIIII', d, off)
        secs.append(dict(name=name, addr=addr, offset=offset, size=size))
    shstr = secs[e_shstrndx]
    st = d[shstr['offset']:shstr['offset'] + shstr['size']]
    for s in secs:
        e = st.find(b'\0', s['name'])
        s['sname'] = st[s['name']:e].decode()
    return d, secs

def funcs(path):
    d, secs = load(path)
    byname = {s['sname']: s for s in secs}
    ds, st = byname['.dynsym'], byname['.dynstr']
    strtab = d[st['offset']:st['offset'] + st['size']]
    def gstr(n):
        e = strtab.find(b'\0', n)
        return strtab[n:e].decode('utf-8', 'replace')
    text = byname['.text']
    t0, t1 = text['addr'], text['addr'] + text['size']
    out = []
    for i in range(ds['size'] // 16):
        off = ds['offset'] + i * 16
        name, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, off)
        typ, bind = info & 0xf, info >> 4
        if typ != 2 or shndx == 0 or bind == 0:
            continue
        v = value & ~1
        if t0 <= v < t1:
            out.append(dict(name=gstr(name), value=v, size=size))
    return out

fo, fu = funcs(sys.argv[1]), funcs(sys.argv[2])
om = {s['name']: s for s in fo}
um = {s['name']: s for s in fu}
common = sorted([s for s in fo if s['name'] in um], key=lambda x: (x['value'], x['name']))
last = None
for i, s in enumerate(common):
    u = um[s['name']]
    delta = u['value'] - s['value']
    if last is None or delta != last:
        if i:
            ps, pu = common[i - 1], um[common[i - 1]['name']]
            go = s['value'] - (ps['value'] + ps['size'])
            gu = u['value'] - (pu['value'] + pu['size'])
            print(f"DELTA {last:+d} -> {delta:+d} at {s['name']}")
            print(f"   prev {ps['name']} orig {ps['value']:#x}+{ps['size']:#x} "
                  f"ours {pu['value']:#x}+{pu['size']:#x}")
            print(f"   gap_orig={go:#x} ({go}) gap_ours={gu:#x} ({gu}) "
                  f"gap_delta={gu-go:+d}")
        else:
            print(f"DELTA {delta:+d} at first {s['name']}")
        last = delta
```

Run:

```
python3 /tmp/gapwalk.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
```

### Full symtab for local-function ownership

The linked `.so` is stripped, so I relinked the exact same gold command without
the final `strip --strip-all` to `/tmp/libgof2hdaa_sym.so`. Its `.text` bytes are
identical to the stripped `.so` (verified byte-for-byte), and its `.symtab`
gives every local function address/size. To reproduce:

```
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/linkcmd.txt
sed 's#-o .../cmake-build-match/libgof2hdaa.so#-o /tmp/libgof2hdaa_sym.so#' \
  /tmp/linkcmd.txt > /tmp/linkcmd_sym.sh
bash /tmp/linkcmd_sym.sh
```

Object ownership:

```
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
find cmake-build-match/CMakeFiles/gof2.dir -name '*.obj' -print0 | \
  xargs -0 $NM -A -S > /tmp/obj_syms.txt
```

`cmake-build-match/section_order.txt` is the gold ordering file. Local
(`.text.<mangled>`) sections that are not in the original `.dynsym` are slotted
after the preceding listed section in their own object by
`tools/gen_section_order.py` (`before`/`after` maps). This is why a weak helper
such as `__clang_call_terminate` can be owned by one object but placed in
another object's gap.

Disassembly:

```
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x... --stop-address=0x... <so>
```

## Full delta walk

Every point where the cumulative delta changes. `gap_orig`/`gap_ours` are the
bytes between the previous exported function's end and the next exported
function's start (local code + alignment). Negative `gap_ours` means the two
exported functions are inverted in our link (plain-`.text` grouping; see below).

rows 66
| # | prev -> next | delta | gap_orig | gap_ours | gap_delta | cause |
|---|---|---|---|---|---|---|
| 0 | `_ZN11AbyssEngine13getAppVersionEv` -> `_ZN11AbyssEngine6Engine6InitGLEbii` | +0 -> +4 | 0 | 2 | +2 | our `getAppVersion` is a 2 B `bx lr`; original is a 0 B alias of `Engine::InitGL` (+2 B body, +2 B align) |
| 1 | `_ZN20FileInterfaceAndroidD2Ev` -> `_ZN20FileInterfaceAndroidD0Ev` | +4 -> -6 | 10 | 0 | -10 | original `__clang_call_terminate` (10 B) sits here; ours is placed after AESoundRessource::D2Ev |
| 2 | `_ZN20FileInterfaceAndroidD0Ev` -> `_ZN20FileInterfaceAndroid8OpenReadEN11AbyssEngine6StringEibiij` | -6 -> -8 | 2 | 0 | -2 | alignment |
| 3 | `_Z14ArrayAddCachedIjEvT_R5ArrayIS0_E` -> `_ZN8GameText13getRegionCodeEv` | -8 -> +8 | 2 | 18 | +16 | extra out-of-line `arabicFormOffset` (16 B); original inlines the arithmetic |
| 4 | `_ZN5ArrayIPN11AbyssEngine16AESoundInterfaceEED2Ev` -> `_ZN11AbyssEngine16AESoundRessource8SetSoundEPKNS_11AESoundInfoEi` | +8 -> +18 | 0 | 10 | +10 | our `__clang_call_terminate` (10 B) sits here; original has it after FileInterfaceAndroid::D2Ev |
| 5 | `_Z14ArraySetLengthIPN11AbyssEngine16AESoundInterfaceEEvjR5ArrayIT_E` -> `_ZN11AbyssEngine16AESoundRessource18initWithoutLoadingEi` | +18 -> +20 | 0 | 2 | +2 | alignment |
| 6 | `_ZN11AbyssEngine6AEMath14MatrixDebugOutERKNS0_6MatrixE` -> `_ZN11AbyssEngine10QuaternionC1Ev` | +20 -> +18 | 2 | 0 | -2 | alignment |
| 7 | `_ZN11AbyssEngine10QuaternionC2ERKNS_6AEMath6MatrixE` -> `_ZN11AbyssEngine10Quaternion3SetERKNS_6AEMath6MatrixE` | +18 -> +16 | 2 | 0 | -2 | alignment |
| 8 | `_Z8ArrayAddIiEvT_R5ArrayIS0_E` -> `_ZN8GameText7getTextEi` | +16 -> +56 | 2 | 42 | +40 | extra out-of-line `levelTextAnchor` (40 B); original Level.cpp has no such helper |
| 9 | `_ZN11AbyssEngine18ApplicationManager10CheckCrackEPKc` -> `_Z7glErrorv` | +56 -> +54 | 2 | 0 | -2 | alignment |
| 10 | `_Z7glErrorv` -> `_ZN11AbyssEngine6EngineC1Ev` | +54 -> +56 | 0 | 2 | +2 | alignment |
| 11 | `_ZN11AbyssEngine12ConfigReader25RegisterTokenReadFunctionENS_6StringEPFvPS0_PvES3_` -> `_ZN5ArrayIPN11AbyssEngine11TokenStructEED2Ev` | +56 -> +48 | 700 | 692 | -8 | `ReadTimer` 100 B vs 104 B and `ReadKeyMap` 476 B vs 480 B: each misses a 2 B `b.n` + 2 B `nop` |
| 12 | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` -> `_ZN6MTitleC1Ev` | +48 -> +44 | 0 | 0 | +0 | `Generator::createMission` body is 4 B short (exported size 1992 vs 1996) |
| 13 | `_ZN6MTitle10OnTouchEndEii` -> `_ZN6MTitle9OnReleaseEv` | +44 -> +48 | 0 | 4 | +4 | extra `generatorMissionLayoutPad` (4 B); original MTitle.cpp has no such helper |
| 14 | `_ZN8KIPlayer25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE` -> `_ZN8ListItemC1EiiPN11AbyssEngine6StringE` | +48 -> +46 | 2 | 0 | -2 | alignment |
| 15 | `_ZN8ListItem11getNumLinesEv` -> `_ZN8ListItem12checkCreditsEv` | +46 -> +44 | 2 | 0 | -2 | alignment |
| 16 | `_ZN9Explosion9translateERKN11AbyssEngine6AEMath6VectorE` -> `_ZN6StatusC1Ev` | +44 -> +42 | 2 | 0 | -2 | alignment |
| 17 | `_ZN6Status9isOnStackEP7Station` -> `_ZN6Status10setStationEP7Station` | +42 -> +40 | 2 | 0 | -2 | alignment |
| 18 | `_ZN5Level20setPlayerEngineColorEs` -> `_ZN5Level19initParticleSystemsEv` | +40 -> +48 | 4 | 12 | +8 | extra `levelMissionPad` (8 B); original Level.cpp has no such helper |
| 19 | `_ZN5Level25incNumDeliveredPassengersEi` -> `_ZN7StarMapC1EbP7Missionbi` | +48 -> +56 | 0 | 8 | +8 | extra `StarMap_GetScreenPosition` (8 B); original StarMap.cpp has no such helper |
| 20 | `_ZN13RecordHandler26readOptionsFileAsByteArrayEPPa` -> `_ZN13RecordHandler27writeByteArrayAsOptionsFileEPai` | +56 -> +88 | 0 | 32 | +32 | extra `is_equal` (32 B) in RecordHandler.cpp; original uses only libc++abi's copy |
| 21 | `_ZN10ModStation11OnTouchMoveEiiPv` -> `_ZN10ModStation10OnTouchEndEiiPv` | +88 -> +140 | 0 | 52 | +52 | extra `modStationTouchEndPad` (52 B); original ModStation.cpp has no such helper |
| 22 | `_ZN13PlayerFighter13setAIDisabledEb` -> `_ZN13PlayerFighter6updateEi` | +140 -> +136 | 6 | 2 | -4 | alignment (original 6 B vs ours 2 B) |
| 23 | `_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf` -> `_ZN4NodeC1Ei` | +136 -> +134 | 2 | 0 | -2 | alignment |
| 24 | `_ZN16SystemPathFinder15getJumpDistanceEP5ArrayIP11SolarSystemEii` -> `_ZN16SystemPathFinder13getSystemPathEP5ArrayIP11SolarSystemEii` | +134 -> +132 | 2 | 0 | -2 | alignment |
| 25 | `_ZN14PlayerCreature8calmDownEv` -> `_ZN14PlayerCreature5resetEv` | +132 -> +128 | 4 | 0 | -4 | alignment (original 4 B vs ours 0 B) |
| 26 | `_ZN13PlayerStation25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE` -> `_ZN6WantedC1EiN11AbyssEngine6StringEiibiiiiiiiii` | +128 -> +126 | 2 | 0 | -2 | alignment |
| 27 | `_ZN6Wanted18getCurrentLocationEv` -> `_ZN15MenuTouchWindowC1Ei` | +126 -> +128 | 0 | 2 | +2 | alignment |
| 28 | `_ZN15MenuTouchWindow23startSupernovaChallengeEv` -> `_ZN14PendingProductC1EP9BluePrint` | +128 -> +3064 | 0 | 2936 | +2936 | extra `mtwTextLayoutAnchor` (2934 B) + 2 B align; original MenuTouchWindow.cpp has no such helper |
| 29 | `_ZN14ListItemWindow10OnTouchEndEii` -> `_ZN10StarSystemC1Ei` | +3064 -> +3080 | 4 | 20 | +16 | extra `starSystemLayoutAnchor` (14 B) + 2 B align; original StarSystem.cpp has no such helper |
| 30 | `_ZN12MarqueeImage4drawEv` -> `_ZN9ObjectGunC1EiP3GunijP5Level` | +3080 -> +3096 | 4 | 20 | +16 | extra `ObjectGunProjectileMesh` (16 B); original ObjectGun.cpp inlines the table lookup |
| 31 | `_ZN11ModMainMenu17ShowLoadingScreenEv` -> `_ZN6GalaxyC1Ev` | +3096 -> +3094 | 2 | 0 | -2 | alignment |
| 32 | `_ZN6Galaxy9getSystemEi` -> `_ZN6Galaxy10getStationEi` | +3094 -> +3096 | 0 | 2 | +2 | alignment |
| 33 | `_ZN18ParticleSystemMesh12getQuadCountEv` -> `inflateReset` | +3096 -> +3094 | 2 | 0 | -2 | alignment |
| 34 | `inflatePrime` -> `inflateInit2_` | +3094 -> +3092 | 2 | 0 | -2 | alignment |
| 35 | `_zip_get_name` -> `zip_file_strerror` | +3092 -> +3090 | 2 | 0 | -2 | alignment |
| 36 | `zip_file_strerror` -> `zip_fopen_index` | +3090 -> +3088 | 2 | 0 | -2 | alignment |
| 37 | `zip_error_clear` -> `zip_get_archive_comment` | +3088 -> +3086 | 2 | 0 | -2 | alignment |
| 38 | `zip_file_error_get` -> `_zip_file_get_offset` | +3086 -> +3084 | 2 | 0 | -2 | alignment |
| 39 | `_zip_unchange` -> `_zip_new` | +3084 -> +3082 | 2 | 0 | -2 | alignment |
| 40 | `_zip_new` -> `zip_close` | +3082 -> +3080 | 2 | 0 | -2 | alignment |
| 41 | `zip_unchange_archive` -> `zip_source_free` | +3080 -> +3078 | 2 | 0 | -2 | alignment |
| 42 | `zip_source_filep` -> `_zip_source_file_or_p` | +3078 -> +3076 | 2 | 0 | -2 | alignment |
| 43 | `zip_strerror` -> `_zip_entry_new` | +3076 -> +3074 | 2 | 0 | -2 | alignment |
| 44 | `_zip_entry_new` -> `zip_get_archive_flag` | +3074 -> +3072 | 2 | 0 | -2 | alignment |
| 45 | `zip_set_archive_flag` -> `_zip_unchange_data` | +3072 -> +3070 | 2 | 0 | -2 | alignment |
| 46 | `_zip_unchange_data` -> `zip_fread` | +3070 -> +3072 | 0 | 2 | +2 | alignment |
| 47 | `zip_fread` -> `zip_rename` | +3072 -> +3070 | 2 | 0 | -2 | alignment |
| 48 | `zip_rename` -> `zip_source_zip` | +3070 -> +3072 | 0 | 2 | +2 | alignment |
| 49 | `zip_stat_index` -> `zip_source_function` | +3072 -> +3070 | 2 | 0 | -2 | alignment |
| 50 | `zip_source_function` -> `zip_replace` | +3070 -> +3068 | 2 | 0 | -2 | alignment |
| 51 | `_zip_replace` -> `zip_delete` | +3068 -> +3066 | 2 | 0 | -2 | alignment |
| 52 | `zip_get_file_comment` -> `_zip_mkstemp` | +3066 -> +3068 | 0 | 2 | +2 | alignment |
| 53 | `zip_error_get` -> `zip_fclose` | +3068 -> +3066 | 2 | 0 | -2 | alignment |
| 54 | `zip_fclose` -> `_zip_cdir_free` | +3066 -> +3064 | 2 | 0 | -2 | alignment |
| 55 | `zip_stat_init` -> `zip_fopen` | +3064 -> +12868 | 0 | 9804 | +9804 | plain `.text` group (9804 B) inserted here by `PLAIN_TEXT_LEAD=0x180`; original has zip_fopen immediately after zip_stat_init |
| 56 | `zip_fopen` -> `zip_unchange_all` | +12868 -> +12866 | 2 | 0 | -2 | alignment |
| 57 | `zip_unchange_all` -> `_zip_error_strerror` | +12866 -> +12864 | 2 | 0 | -2 | alignment |
| 58 | `_zip_error_strerror` -> `SHA224_Init` | +12864 -> +2696 | 0 | -10168 | -10168 | same plain `.text` group: SHA224_Init is inside the group, so its address is inverted (negative gap) |
| 59 | `SHA224_Final` -> `OPENSSL_atomic_add` | +2696 -> +10240 | 4 | 7548 | +7544 | plain `.text` tail: sha256_block_data_order_armv8 + OPENSSL_* order differs (original has OPENSSL_* before sha256_block_data_order_neon) |
| 60 | `OPENSSL_instrument_bus2` -> `sha256_block_data_order` | +10240 -> +2336 | 328 | -7576 | -7904 | same plain `.text` ordering: sha256_block_data_order is inside the group (negative gap) |
| 61 | `sha256_block_data_order_neon` -> `__cxa_end_cleanup` | +2336 -> +3152 | 576 | 1392 | +816 | plain `.text` tail: sha256_block_data_order_armv8 + OPENSSL_* occupy the gap |
| 62 | `__cxa_deleted_virtual` -> `__dynamic_cast` | +3152 -> +2564 | 2212 | 1624 | -588 | libc++abi local type_info/exception gap: original 2212 B, ours 1624 B (-588 B; prebuilt archive differs) |
| 63 | `__dynamic_cast` -> `_ZNSt13bad_exceptionD1Ev` | +2564 -> +2562 | 1300 | 1298 | -2 | alignment |
| 64 | `_ZNSt9exceptionD0Ev` -> `_ZNKSt9exception4whatEv` | +2562 -> +2560 | 2 | 0 | -2 | alignment |
| 65 | `_ZNKSt10bad_typeid4whatEv` -> `_ZSt14set_unexpectedPFvvE` | +2560 -> +2464 | 96 | 0 | -96 | original libc++abi 96 B local function placed here; ours places its (different) copy after set_unexpected |


## Gaps with identical size (no delta change)

These are non-trivial gaps (>= 8 B) where `gap_orig == gap_ours`, so they do not
change the cumulative delta. They are included for completeness: the local
functions are archive code (zlib, libzip, libc++abi) or the JNI helper, and
their sizes match exactly. The local-function list is from our `.symtab`.

| prev -> next | gap_orig | gap_ours | local function(s) in ours |
|---|---:|---:|---|
| `_ZN7_JNIEnv23CallStaticBooleanMethodEP7_jclassP10_jmethodIDz` -> `_ZN3NFC8getWidthEv` | 0x20 | 0x20 | `_ZL21clearPendingExceptionP7_JNIEnv@0x69d5c+32` |
| `inflate` -> `inflateEnd` | 0xb6 | 0xb6 | `updatewindow@0x188834+182` |
| `inflateSync` -> `inflateSyncPoint` | 0x4a | 0x4a | `syncsearch@0x188aa8+74` |
| `deflateBound` -> `deflateCopy` | 0x66 | 0x66 | `putShortMSB@0x18983c+32`; `flush_pending@0x18985c+70` |
| `deflateCopy` -> `get_crc_table` | 0xaf6 | 0xaf6 | `deflate_stored@0x1899ba+250`; `deflate_fast@0x189ab4+696`; `deflate_slow@0x189d6c+968`; `fill_window@0x18a134+282`; `longest_match@0x18a24e+406`; `longest_match_fast@0x18a3e4+202` |
| `crc32_combine` -> `inflateBackInit_` | 0x38 | 0x38 | `gf2_matrix_square@0x18a810+56` |
| `_tr_init` -> `_tr_stored_block` | 0x60 | 0x60 | `init_block@0x18b7b4+96` |
| `_tr_align` -> `_tr_flush_block` | 0x6c | 0x6c | `bi_flush@0x18ba20+108` |
| `_tr_flush_block` -> `_tr_tally` | 0x708 | 0x708 | `build_tree@0x18be50+912`; `compress_block@0x18c1e0+796`; `bi_windup@0x18c4fc+92` |
| `_tr_tally` -> `inflate_fast` | 0x518 | 0x518 | `pqdownheap@0x18c604+182`; `scan_tree@0x18c6ba+208`; `send_tree@0x18c78a+912` |
| `zip_source_buffer` -> `zip_error_clear` | 0x88 | 0x88 | `read_data@0x18d5d4+136` |
| `zip_close` -> `zip_add_dir` | 0x48 | 0x48 | `_zip_torrentzip_cmp@0x18e2b0+8`; `ch_set_error@0x18e2b8+64` |
| `_zip_source_file_or_p` -> `_zip_error_clear` | 0x138 | 0x138 | `read_file@0x18e5f0+312` |
| `zip_source_zip` -> `zip_stat_index` | 0xf0 | 0xf0 | `read_zip@0x18eb30+240` |
| `zip_open` -> `zip_error_get` | 0x238 | 0x238 | `_zip_allocate_new@0x18f53c+108`; `set_error@0x18f5a8+88`; `_zip_checkcons@0x18f600+372` |
| `_zip_dirent_write` -> `_zip_dirent_init` | 0x56 | 0x56 | `_zip_write4@0x18fb28+60`; `_zip_write2@0x18fb64+26` |
| `_zip_read4` -> `_zip_dirent_torrent_normalize` | 0xe4 | 0xe4 | `_zip_readstr@0x18fe94+106`; `_zip_readfpstr@0x18fefe+120` |
| `CRYPTO_memcmp` -> `_armv7_neon_probe` | 0x18 | 0x18 | alignment only |
| `sha256_block_data_order` -> `sha256_block_data_order_neon` | 0x18 | 0x18 | alignment only |
| `__cxa_end_cleanup` -> `__cxa_allocate_exception` | 0x10 | 0x10 | alignment only |
| `__cxa_throw` -> `__cxa_get_exception_ptr` | 0x2a | 0x2a | `_ZN10__cxxabiv1L22exception_cleanup_funcE...@0x192a38+24`; `_ZN10__cxxabiv1L12failed_throwEPNS_15__cxa_exceptionE@0x192a50+18` |
| `__cxa_begin_cleanup` -> `__cxa_begin_catch` | 0x5c | 0x5c | `__cxa_end_cleanup_impl@0x192ac0+92` |
| `__cxa_rethrow_primary_exception` -> `__cxa_uncaught_exception` | 0x24 | 0x24 | `_ZN10__cxxabiv1L27dependent_exception_cleanupE...@0x192de0+36` |
| `__cxa_get_globals_fast` -> `__cxa_guard_acquire` | 0x50 | 0x50 | `_ZN10__cxxabiv112_GLOBAL__N_110construct_Ev@0x192ea8+40`; `_ZN10__cxxabiv112_GLOBAL__N_19destruct_EPv@0x192ed0+40` |
| `_ZSt9terminatev` -> `_ZSt10unexpectedv` | 0x14 | 0x14 | `_ZSt12__unexpectedPFvvE@0x193084+20` |
| `_ZSt13get_terminatev` -> `_ZSt15set_new_handlerPFvvE` | 0x34 | 0x34 | `_ZSt11__terminatePFvvE@0x1930c4+52` |
| `__gxx_personality_v0` -> `__cxa_call_unexpected` | 0x3d8 | 0x3d8 | `continue_unwind` 20; `scan_eh_tab` 808; `_Unwind_GetGR` 56; `call_terminate` 28; `set_registers` 72 |
| `__cxa_call_unexpected` -> `__cxa_pure_virtual` | 0x1bc | 0x1bc | `readEncodedPointer` 168; `readULEB128` 46; `exception_spec_can_catch` 108; `readSLEB128` 66; `_Unwind_SetGR` 52 |
| `_ZSt13set_terminatePFvvE` -> `__cxa_demangle` | 0x10c | 0x10c | `demangling_terminate_handler` 240; `demangling_unexpected_handler` 28 |


## Local-function inventory

Sizes are from our `/tmp/libgof2hdaa_sym.so` and from the original disassembly.
"Original TU" is from `_work/original_layout/tu_mapping.md`.

| our local function | our size | original size | original TU | class | delta | our object | source file |
|---|---:|---:|---|---|---:|---|---|
| `getAppVersion` (exported) | 2 | 0 (alias of `Engine::InitGL`) | invented `AbyssEngine.cpp`; original `Engine.cpp` | (a) body/alias difference | +2 | `src/engine/core/AbyssEngine.cpp.obj` | `src/engine/core/AbyssEngine.cpp` |
| `__clang_call_terminate` | 10 | 10 (same code) | `FileInterfaceIPhone.mm` analog | (b) helper exists, wrong gap | net 0 (`-10` then `+10`) | `src/engine/file/FileInterfaceAndroid.cpp.obj` (weak; also `AESoundRessource.cpp.obj`) | compiler-generated in `src/engine/file/FileInterfaceAndroid.cpp` |
| `arabicFormOffset` | 16 | 0 (inlined) | `Svn_iPhone_AE2/Classes/GameText.cpp` | (b) extra helper | +16 | `src/engine/core/GameText.cpp.obj` | `src/engine/core/GameText.cpp` |
| `levelTextAnchor` | 40 | 0 | `Classes/Game/Level.cpp` | (b) extra helper | +40 | `src/game/world/Level.cpp.obj` | `src/game/world/Level.cpp` |
| `ReadTimer` | 100 | 104 | `Svn_iPhone_AE2/Classes/configreader.cpp` | (a) body difference | -4 | `src/engine/file/ConfigReader.cpp.obj` | `src/engine/file/ConfigReader.cpp` |
| `ReadKeyMap` | 476 | 480 | `Svn_iPhone_AE2/Classes/configreader.cpp` | (a) body difference | -4 | `src/engine/file/ConfigReader.cpp.obj` | `src/engine/file/ConfigReader.cpp` |
| `ReadVibration` | 116 | 116 | `Svn_iPhone_AE2/Classes/configreader.cpp` | same | 0 | `src/engine/file/ConfigReader.cpp.obj` | `src/engine/file/ConfigReader.cpp` |
| `Generator::createMission` (exported) | 1992 | 1996 | `Classes/Game/Generator.cpp` | (a) body difference | -4 | `src/game/mission/Generator.cpp.obj` | `src/game/mission/Generator.cpp` |
| `generatorMissionLayoutPad` | 4 | 0 | `Classes/Game/MTitle.cpp` | (b) extra helper | +4 | `src/game/menu/MTitle.cpp.obj` | `src/game/menu/MTitle.cpp` |
| `levelMissionPad` | 8 | 0 | `Classes/Game/Level.cpp` | (b) extra helper | +8 | `src/game/world/Level.cpp.obj` | `src/game/world/Level.cpp` |
| `StarMap_GetScreenPosition` | 8 | 0 | `Classes/Game/StarMap.cpp` | (b) extra helper | +8 | `src/game/world/StarMap.cpp.obj` | `src/game/world/StarMap.cpp` |
| `is_equal` (RecordHandler copy) | 32 | 0 here (libc++abi has its own 32 B copy at `0x193a8c`) | libc++abi `private_typeinfo.cpp` (archive) | (b) duplicate helper | +32 | `src/game/mission/RecordHandler.cpp.obj` | `src/game/mission/RecordHandler.cpp` |
| `modStationTouchEndPad` | 52 | 0 | `Classes/Game/ModStation.cpp` | (b) extra helper | +52 | `src/game/menu/ModStation.cpp.obj` | `src/game/menu/ModStation.cpp` |
| `mtwTextLayoutAnchor` | 2934 | 0 | `Classes/Game/MenuTouchWindow.cpp` | (b) extra helper | +2934 (+2 align) | `src/game/ui/MenuTouchWindow.cpp.obj` | `src/game/ui/MenuTouchWindow.cpp` |
| `starSystemLayoutAnchor` | 14 | 0 | `Classes/Game/StarSystem.cpp` | (b) extra helper | +14 (+2 align) | `src/game/world/StarSystem.cpp.obj` | `src/game/world/StarSystem.cpp` |
| `ObjectGunProjectileMesh` | 16 | 0 (inlined) | `Classes/Game/ObjectGun.cpp` | (b) extra helper | +16 | `src/game/weapons/ObjectGun.cpp.obj` | `src/game/weapons/ObjectGun.cpp` |
| libc++abi local gap (`__cxa_deleted_virtual` -> `__dynamic_cast`) | 1624 | 2212 | libc++abi archive | (a) library body/version difference | -588 | `libc++abi.a` (archive) | prebuilt libc++abi |
| libc++abi local gap (`bad_typeid::what` -> `set_unexpected`) | 0 | 96 | libc++abi archive | (b) helper placed elsewhere | -96 | `libc++abi.a` (archive) | prebuilt libc++abi |
| plain `.text` group (libcrypto SHA/OPENSSL + libgcc) | 9804 | ~9760 (`0x44` smaller) | libcrypto archive | (c) grouping/ordering, not body size | `+9804` at zip_fopen, `-10168` at SHA224_Init, net +92 | `libcrypto_gof2.a` / `libgcc.a` (archives) | `tools/gen_section_order.py` |

## Detailed gap analyses

### 1. `getAppVersion` -> `Engine::InitGL` (`+0 -> +4`)

Original:

```
0006db20 <_ZN11AbyssEngine13getAppVersionEv@@Base>:   ; size 0, same addr as InitGL
0006db20 <_ZN11AbyssEngine6Engine6InitGLEbii@@Base>:  ; size 408
```

Ours:

```
0006db20 <_ZN11AbyssEngine13getAppVersionEv>:
   6db20: 4770  bx lr
0006db24 <_ZN11AbyssEngine6Engine6InitGLEbii>:
```

`getAppVersion` is an empty `void` function in `src/engine/core/AbyssEngine.cpp`;
clang emits a 2-byte `bx lr`. The original symbol is a 0-size alias of
`Engine::InitGL` at the same address. This contributes `+2` body plus `+2`
alignment. Fixing it requires making the symbol a zero-size alias (the source
currently has no attribute-free way to do that; the lint bans
`__attribute__`).

### 2. `FileInterfaceAndroid::D2Ev` -> `D0Ev` (`-10`)

Original gap `0x6dfc4..0x6dfce` is exactly `__clang_call_terminate`:

```
0006dfc4: b580       push {r7, lr}
0006dfc6: f7f0 efac  blx  __cxa_begin_catch@plt
0006dfca: f7f0 efb0  blx  _ZSt9terminatev@plt
```

Ours has no local code there; the same 10-byte helper is placed at `0x7fb7c`,
between `Array<AESoundInterface>::~Array()` and
`AESoundRessource::SetSound`. The helper is a weak compiler-generated local
symbol emitted by many TUs. `tools/gen_section_order.py` slots
`.text.__clang_call_terminate` after the first object in CMake input order that
emits it (AESoundRessource), not after the original owner
(FileInterfaceAndroid). This is a **placement** issue, not a size difference:
the final drift is unchanged, but the `-10/+10` wiggle is visible in the walk.

### 3. `ArrayAddCached<unsigned>` -> `GameText::getRegionCode` (`+16`)

Original gap is 2 B alignment. Ours is 18 B: `arabicFormOffset` at `0x7efc4`
(16 B) plus 2 B alignment. The original inlines the `(code - 0x621) * 20`
arithmetic in `convertStringFromArabic`; our out-of-line hidden function is
extra. Making it `static` removes it, but measured `byte_exact 1458 -> 1392`
(`-66`).

### 4. `AESoundRessource` gap (`+10`)

This is the other half of item 2: our `__clang_call_terminate` at `0x7fb7c`
(10 B). The original has 0 B here.

### 5. `ArrayAdd<int>` -> `GameText::getText` (`+40`)

Original gap 2 B. Ours has `levelTextAnchor` at `0x81828` (40 B) plus 2 B
alignment. The original `Level.cpp` has no such helper; it is a deliberate
layout pad. Removing it costs `byte_exact 1458 -> 1390` (`-68`).

### 6. `ConfigReader::RegisterTokenReadFunction` -> `Array<TokenStruct>::~Array()` (`-8`)

Original local code is 700 B; ours 692 B. Boundaries from disassembly:

| function | original | ours | delta |
|---|---:|---:|---:|
| `ReadTimer` | `0x873a0..0x87408` = 104 | `0x873d8..0x8743c` = 100 | -4 |
| `ReadKeyMap` | `0x87408..0x875e8` = 480 | `0x8743c..0x87618` = 476 | -4 |
| `ReadVibration` | `0x875e8..0x8765c` = 116 | `0x87618..0x8768c` = 116 | 0 |

Both `ReadTimer` and `ReadKeyMap` are 4 B smaller in ours. The missing bytes
are one 2-byte `b.n` in the exception-handler sequence plus a 2-byte `nop`
before the literal pool:

```
; original ReadTimer tail
873ea: blx __stack_chk_fail
873ee: e7ff  b.n 873f0
873f0: mov r4, r0
...
873fa: blx _Unwind_Resume
873fe: bf00  nop
87400: <literal pool>

; ours
87422: blx __stack_chk_fail
87426: mov r4, r0
...
87430: blx _Unwind_Resume
87434: <literal pool>
```

`ReadKeyMap` has the same pattern (extra `b.n` in the landing-pad sequence and
a trailing `nop`). This is a **real body difference** in the exception-handler
layout, not an extra helper. Fixing it means matching the original's landing-pad
branch shape in `src/engine/file/ConfigReader.cpp`.

### 7. `Generator::createMission` -> `MTitle` ctor (`-4`)

The only exported size mismatch in `report.json`: original 1996 B, ours 1992 B.
The comment in `src/game/menu/MTitle.cpp` already records this ("the
reconstructed createMission body is 4 bytes smaller than the original's"). It is
a real body difference in `src/game/mission/Generator.cpp`.

### 8. `MTitle::OnTouchEnd` -> `MTitle::OnRelease` (`+4`)

Original gap 0. Ours has `generatorMissionLayoutPad` at `0x97a58`:

```
97a58: 2000  movs r0, #0
97a5a: 4770  bx lr
```

This is a pure extra helper. Removing it costs `byte_exact 1458 -> 1416`
(`-42`).

### 9. `Level::setPlayerEngineColor` -> `Level::initParticleSystems` (`+8`)

Original gap 4 B alignment. Ours has `levelMissionPad` at `0xbd71c` (8 B) plus
4 B alignment. Pure extra helper. Removing it costs `byte_exact 1458 -> 1428`
(`-30`).

### 10. `Level::incNumDeliveredPassengers` -> `StarMap` ctor (`+8`)

Original gap 0. Ours has `StarMap_GetScreenPosition` at `0xc67d4` (8 B):

```
c67d4: 2800  cmp r0, #0
c67d6: bf18  it ne
c67d8: 2001  movne r0, #1
c67da: 4770  bx lr
```

Pure extra helper. Removing it costs `byte_exact 1458 -> 1439` (`-19`).

### 11. `RecordHandler::readOptionsFileAsByteArray` -> `writeByteArrayAsOptionsFile` (`+32`)

Original gap 0. Ours has a second copy of libc++abi's `is_equal` at `0xcde10`
(32 B). The original has only the libc++abi copy at `0x193a8c`. This is a
duplicate helper. **This is the one safe removal found:** make it `static` (or
delete it) and clang drops it at `-Oz`; `byte_exact` stays 1458 and the final
delta drops by 32. See the experiment section.

### 12. `ModStation::OnTouchMove` -> `OnTouchEnd` (`+52`)

Original gap 0. Ours has `modStationTouchEndPad` at `0xd7658` (52 B). Pure extra
helper. Removing it costs `byte_exact 1458 -> 1453` (`-5`).

### 13. `PlayerFighter` / `PlayerCreature` gaps (`-4`, `-4`)

These are alignment, not local code:

- `PlayerFighter::setAIDisabled` ends at `0xdcfd2` in the original; `update`
  starts at `0xdcfd8` (8-byte aligned), so 6 B of padding. Ours ends at
  `0xdd05e`; `update` starts at `0xdd060`, so 2 B. Different alignment phase.
- `PlayerCreature::calmDown` ends at `0x11cc3c` in the original; `reset` starts
  at `0x11cc40`, so 4 B padding. Ours ends at `0x11ccbe`; `reset` starts at
  `0x11ccc0`, so 0 B.

These will move when the earlier anchors are fixed.

### 14. `MenuTouchWindow::startSupernovaChallenge` -> `PendingProduct` ctor (`+2936`)

Original gap 0. Ours has `mtwTextLayoutAnchor` at `0x12d448`, size `0xb76`
(2934 B), followed by 2 B alignment to `0x12dfc0`:

```
12d448 <_ZL19mtwTextLayoutAnchorv>:
   ... 2934 bytes of volatile stores ...
12dfbe: <end>
12dfc0 <_ZN14PendingProductC1EP9BluePrint>:
```

This is the single largest drift source. The anchor is kept alive by a
`reinterpret_cast<int>(mtwTextLayoutAnchor)` element in
`MenuTouchWindow_languageIds` in `src/game/ui/MenuTouchWindow.cpp`. Removing the
anchor and that array element reduces the final delta to **-480**, but costs
**59 byte_exact** (`1458 -> 1399`). The anchor is deliberately holding the
addresses of later functions at values that keep their relocated literal pools
byte-exact.

### 15. `ListItemWindow::OnTouchEnd` -> `StarSystem` ctor (`+16`)

Original gap 4 B. Ours has `starSystemLayoutAnchor` at `0x13475c` (14 B) plus
2 B alignment. Pure extra helper. Removing it costs `byte_exact 1458 -> 1428`
(`-30`).

### 16. `MarqueeImage::draw` -> `ObjectGun` ctor (`+16`)

Original gap 4 B. Ours has `ObjectGunProjectileMesh` at `0x160304` (16 B). The
original inlines the `MUZZLE_MESHES[item]` lookup. Pure extra helper. Removing
it costs `byte_exact 1458 -> 1428` (`-30`).

### 17. libzip/openssl: plain `.text` grouping (`+9804`, `-10168`, `+7544`, `-7904`, `+816`)

This is not a local-function body problem. `tools/gen_section_order.py` emits a
single `.text` entry for all plain-`.text` archive members (libcrypto's
SHA224/SHA256/OPENSSL functions and libgcc's `__aeabi_*`), and places it at
`plain_text_addr - PLAIN_TEXT_LEAD` (`0x180`). The earliest plain-`.text`
original symbol is `SHA224_Init` at `0x18f730`; `0x18f730 - 0x180 = 0x18f5b0`
is just before `zip_fopen` (`0x18f5c0`). Gold therefore inserts the whole
9804-byte plain-`.text` block between `zip_stat_init` and `zip_fopen`, while the
original has `zip_fopen` immediately after `zip_stat_init` and the plain
`.text` code later, interleaved with `.text.*` sections.

Consequences in the walk:

| point | gap_orig | gap_ours | gap_delta |
|---|---:|---:|---:|
| `zip_stat_init` -> `zip_fopen` | 0 | 9804 | +9804 |
| `_zip_error_strerror` -> `SHA224_Init` | 0 | -10168 | -10168 |
| `SHA224_Final` -> `OPENSSL_atomic_add` | 4 | 7548 | +7544 |
| `OPENSSL_instrument_bus2` -> `sha256_block_data_order` | 328 | -7576 | -7904 |
| `sha256_block_data_order_neon` -> `__cxa_end_cleanup` | 576 | 1392 | +816 |

The net contribution of this region is only `+92` B, but the intermediate
drift is ~10 KB. The fix belongs in `tools/gen_section_order.py` (adjust
`PLAIN_TEXT_LEAD` or stop grouping plain `.text` members), not in `src/`.

### 18. libc++abi local gaps (`-588`, `-96`)

- `__cxa_deleted_virtual` -> `__dynamic_cast`: original local gap 2212 B, ours
  1624 B. Both sides contain the same exported anchors and the type_info
  local functions, but the prebuilt libc++abi archive's local set/size differs.
- `bad_typeid::what` -> `set_unexpected`: original has a 96-byte local function
  at `0x193e64` (calls `vfprintf`, `fputc`, `vasprintf`, `__assert2`); ours
  places its (different) `demangling_terminate_handler` after
  `set_unexpected`, so the gap is 0.

These are library-archive differences, not app source.

## Drift budget

| category | bytes |
|---|---:|
| extra app local helpers (`getAppVersion`, `arabicFormOffset`, `levelTextAnchor`, `generatorMissionLayoutPad`, `levelMissionPad`, `StarMap_GetScreenPosition`, `is_equal`, `modStationTouchEndPad`, `mtwTextLayoutAnchor`, `starSystemLayoutAnchor`, `ObjectGunProjectileMesh`) | +3126 |
| app body shortfalls (`ConfigReader` -8, `createMission` -4) | -12 |
| `__clang_call_terminate` placement | 0 (net; `-10/+10`) |
| libc++abi local gaps | -684 |
| plain `.text` grouping net | +92 |
| alignment net | -58 |
| **final** | **+2464** |

## Ranking of fixes

Ranked by final address drift removed. "byte_exact effect" is the measured
one-at-a-time change from the 1458 baseline. The safe `is_equal` patch, where
present in the smaller-removal experiments, does not change `byte_exact`.

| rank | fix | drift removed | byte_exact effect | source file / function |
|---:|---|---:|---:|---|
| 1 | remove `mtwTextLayoutAnchor` (and its array element) | 2944 (final `+2464 -> -480`) | **-59** (1458 -> 1399) | `src/game/ui/MenuTouchWindow.cpp` |
| 2 | remove `modStationTouchEndPad` | 52 | -5 (1458 -> 1453) | `src/game/menu/ModStation.cpp` |
| 3 | remove `levelTextAnchor` | 40 | -68 (1458 -> 1390) | `src/game/world/Level.cpp` |
| 4 | remove the `RecordHandler` copy of `is_equal` | 32 | **0 (1458 -> 1458)** | `src/game/mission/RecordHandler.cpp` |
| 5 | remove `arabicFormOffset` (make it `static`/inline) | 16 | -66 (1458 -> 1392) | `src/engine/core/GameText.cpp` |
| 6 | remove `ObjectGunProjectileMesh` (inline the table lookup) | 16 | -30 (1458 -> 1428) | `src/game/weapons/ObjectGun.cpp` |
| 7 | remove `starSystemLayoutAnchor` | 16 (14 + 2 align) | -30 (1458 -> 1428) | `src/game/world/StarSystem.cpp` |
| 8 | move `__clang_call_terminate` to the `FileInterfaceAndroid::D2Ev` gap | 0 final (removes `-10/+10` wiggle) | not measured | `tools/gen_section_order.py` / `src/engine/file/FileInterfaceAndroid.cpp` |
| 9 | remove `levelMissionPad` | 8 | -30 (1458 -> 1428) | `src/game/world/Level.cpp` |
| 10 | remove `StarMap_GetScreenPosition` | 8 | -19 (1458 -> 1439) | `src/game/world/StarMap.cpp` |
| 11 | remove `generatorMissionLayoutPad` | 4 | -42 (1458 -> 1416) | `src/game/menu/MTitle.cpp` |
| 12 | make `getAppVersion` a 0-size alias of `Engine::InitGL` | 2 | not measured | `src/engine/core/AbyssEngine.cpp` |
| 13 | grow `ReadTimer`/`ReadKeyMap` by 2 B each (match landing-pad `b.n` + `nop`) | +8 (currently -8) | not measured | `src/engine/file/ConfigReader.cpp` |
| 14 | grow `Generator::createMission` by 4 B | +4 (currently -4) | not measured | `src/game/mission/Generator.cpp` |
| 15 | fix plain `.text` grouping (`PLAIN_TEXT_LEAD`) | removes ~10 KB oscillation; net +92 | not measured | `tools/gen_section_order.py` |
| 16 | libc++abi local gaps | -684 | not app source | prebuilt libc++abi archive |

The ranking is by **drift removed**, not by gate safety. Only rank 4 is
currently a free win. The anchors at ranks 1–3, 5–7, 9–11 are deliberately
holding later relocated literals byte-exact; removing them without first making
the earlier bodies size-exact trades address drift for `byte_exact`.

## Measured experiments

### Safe: remove the `RecordHandler` copy of `is_equal`

Patch:

```diff
-bool is_equal(const std::type_info *lhs, const std::type_info *rhs, bool compareNames) {
+static bool is_equal(const std::type_info *lhs, const std::type_info *rhs, bool compareNames) {
```

Build/check:

```
cmake --build cmake-build-match --target check -j32
```

Result:

```
compared 4523  avg 99.89%  byte_exact 1458  linked_exact 4493  fuzzy_perfect 4494
verify: byte 1458  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Delta walk after the patch: final delta `+2464 -> +2432`; the 32-byte jump at
`writeByteArrayAsOptionsFile` disappears (the delta stays `+56` there). The
symbol is no longer emitted.

This patch is safe but was reverted before committing the note (the deliverable
tree only contains this note).

### Large anchor: remove `mtwTextLayoutAnchor`

Patch:

```diff
-static void mtwTextLayoutAnchor();
...
-static const int MenuTouchWindow_languageIds[11] = {27, 28, 30, 29, 31, 32, 33, 35, 38, 39,
-                                                    reinterpret_cast<int>(mtwTextLayoutAnchor)};
+static const int MenuTouchWindow_languageIds[10] = {27, 28, 30, 29, 31, 32, 33, 35, 38, 39};
...
-static void mtwTextLayoutAnchor() {
-    ... 2934 bytes of volatile stores ...
-}
```

Result:

```
verify: byte 1399  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
RATCHET FAIL: byte_exact 1458 -> 1399
```

Delta walk after the patch: final delta `+2464 -> -480`; the
`PendingProduct` point changes from `+128 -> +3064` to `+128 -> +144`.

### Other one-at-a-time removals

Each was measured one-at-a-time from the 1458 baseline. The `is_equal` patch,
where present, does not change `byte_exact`.

| removal | byte_exact |
|---|---:|
| `arabicFormOffset` (`static`) | 1392 (-66) |
| `generatorMissionLayoutPad` | 1416 (-42) |
| `ObjectGunProjectileMesh` (`static`) | 1428 (-30) |
| `starSystemLayoutAnchor` | 1428 (-30) |
| `levelMissionPad` | 1428 (-30) |
| `StarMap_GetScreenPosition` | 1439 (-19) |
| `levelTextAnchor` | 1390 (-68) |
| `modStationTouchEndPad` | 1453 (-5) |

The `arabicFormOffset` removal also reduced the final delta by 32 (the helper's
16 B plus 16 B of downstream alignment) to `+2432`.

## Reproduction checklist

```
# 1. baseline
cmake --preset default
cmake --build cmake-build-match --target check -j32

# 2. delta walk
python3 /tmp/gapwalk.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so

# 3. full symtab for local functions
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/linkcmd.txt
sed 's#-o .../cmake-build-match/libgof2hdaa.so#-o /tmp/libgof2hdaa_sym.so#' \
  /tmp/linkcmd.txt > /tmp/linkcmd_sym.sh
bash /tmp/linkcmd_sym.sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nS /tmp/libgof2hdaa_sym.so

# 4. object ownership
find cmake-build-match/CMakeFiles/gof2.dir -name '*.obj' -print0 | \
  xargs -0 $NM -A -S > /tmp/obj_syms.txt

# 5. disassembly
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x... --stop-address=0x... \
    ../_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x... --stop-address=0x... \
    cmake-build-match/libgof2hdaa.so
```

## Notes / caveats

- `tools/verify_baseline.json` is the authoritative baseline for this tree; it
  says `byte_exact 1458`, not the 1415 in the task brief. No `--update-baseline`
  was run.
- The `linked_exact` gate masks calls into local code (`#X`), so a wrong local
  callee among these helpers is invisible to the gate. The disassembly above is
  the cross-check.
- The `mtwTextLayoutAnchor` and other anchor removals are not "free": the
  original's later functions have relocated literal-pool words whose bytes
  depend on the link address. The anchors keep those addresses at the values
  that make the current 1458 byte-exact functions byte-exact. The right order is
  to make the earlier bodies size-exact first, then remove the anchors and
  re-measure.
- The plain-`.text` grouping is the largest single source of intermediate
  address drift (~10 KB), but its net contribution to the final delta is only
  +92 B. Fixing it will move thousands of addresses and needs a full gate run.
