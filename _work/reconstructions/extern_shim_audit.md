# Extern / decompiler-shim audit

Branch: `wave39a-w915` (analysis-only; no `src/`, build-config, or gate changes).
Target: `../_work/bins/android_2.0.16_libgof2hdaa.so` (ELF 32-bit ARM, NDK r18b clang 7.0.2,
`-Oz`, image base 0x10000 in Ghidra).

This report inventories the remaining file-scope `extern`/free-function declarations in `src/`,
separates genuine ABI from decompiler shims, and gives a bounded replacement plan for each live
shim. Addresses below are **ELF virtual addresses from `arm-linux-androideabi-nm -D`**; add
`0x10000` for the Ghidra address used by the project (`Ghidra VA = nm value + 0x10000`).

## Method / evidence

Commands used (no full gate was run):

```sh
cmake --preset default
cmake --build cmake-build-match --target link_so -j$(nproc)

NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D --undefined-only cmake-build-match/libgof2hdaa.so | awk '{print $NF}' | sort -u
$NM -D --undefined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | awk '{print $NF}' | sort -u
$NM -D --defined-only  ../_work/bins/android_2.0.16_libgof2hdaa.so | awk '{print $NF}' | sort -u
$NM -D --defined-only  cmake-build-match/libgof2hdaa.so | awk '{print $NF}' | sort -u

# AST inventory: declarations with no body in the main file, and definitions anywhere in src/
/usr/lib/llvm-19/bin/clang-query -p cmake-build-match \
  -c 'set output dump' \
  -c 'match functionDecl(isExpansionInMainFile(), unless(hasBody(compoundStmt()))).bind("f")' \
  -c 'match functionDecl(isExpansionInMainFile(), hasBody(compoundStmt())).bind("def")' \
  -c 'quit' src/game/ui/MenuTouchWindow.cpp

# call-reference check: does a call bind to the file-scope void* shim or to the typed overload?
/usr/lib/llvm-19/bin/clang-query -p cmake-build-match \
  -c 'set output dump' \
  -c 'match callExpr(callee(functionDecl(hasName("MatrixSetTranslation")))).bind("c")' \
  -c 'quit' src/game/world/StarMap.cpp

readelf -S ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -S cmake-build-match/libgof2hdaa.so
```

Machine-checked baseline facts (from the current build / `report.json` / `tools/verify_baseline.json`):

- Original undefined imports: **207**; ours: **207**; extra: **0**; missing: **0**.
- Original defined exports: **5370**; ours: **5370**; extra: **0**; missing: **0**.
- `.text`: original `0x1337c8`, ours `0x1343c0`.
- `.init_array`: original `0xc` (3 entries), ours `0xc` (3 entries).
- `.data`: original `0x296c`, ours `0x2978`.
- `.aezdata`: original absent, ours `0x58` (invented section; see below).
- Current verify baseline: `byte_exact 1352`, `linked_exact 4468`, `stub_zero_size 0`, `missing 0`,
  `wrong_type 0`, `extra 0`, `compared 4523`.

## Classification key

- **(a) Real external ABI** — keep the declaration; the unmangled/C name is in the original
  `.dynsym` or is a genuine libc/FMOD/GLES/JNI boundary. See §1 and §4.
- **(b) Decompiler shim** — a C-style free function or `void*` wrapper standing in for a real C++
  function. If it has call sites, replace it with a direct typed call (§3); if it has zero call
  sites and no definition, delete it (§2).
- **(c) Duplicate/obsolete** — a second declaration of the same name, an overload superseded by a
  typed method, or a redundant local libc declaration. Delete one copy/consolidate; see §2.4.

**Observation vs inference.** Symbol presence/absence, addresses, section sizes, source line
numbers, and `DeclRefExpr` counts are observations from `nm`, `readelf`, `grep`, and clang-query.
The mapping of a dead shim name to a particular original overload in §2.3 is an inference from the
shim name and the original `.dynsym`; where several overloads exist (notably `ChoiceWindow::set`,
`Status::replaceHash`, and `String::operator[]`) the exact intended overload is not recoverable from
the dead declaration alone. `_mtw_buildMenu` and `_mtw_steerFromTouchId` are inferred local helpers
with no exported symbol.

## Bottom line

1. **103 dead shim names remain** (106 declaration lines), all with **zero free-function
   `DeclRefExpr` call references** and **no matching symbol in either the original or our `.dynsym`**.
   They are pure source clutter and can be deleted mechanically: **62 unique names / 65 declaration
   lines in `.cpp` files** plus **41 unique names in headers** (39 `String_*` in `String.h`, 2
   `Trail_*` in `Trail.h`). Adding the genuine libc `sinf` gives 104 unique names / 107 lines.
2. **Five `void*` Matrix declarations are dead/misleading.** `MatrixSetTranslation(void*,...)` in
   `Globals.cpp:332`, `StarMap.cpp:60`, `SpaceLounge.cpp:63` and `MatrixSetRotation(void*,...)` in
   `StarMap.cpp:103`, `SpaceLounge.cpp:67` are not the functions the calls bind to. Clang AST shows
   every call binds to the typed `AbyssEngine::AEMath` overloads
   (`MatrixSetTranslation(Matrix&,float,float,float)` @ `0x80e48`,
   `MatrixSetRotation(Matrix&,float,float,float)` @ `0x80918`). The `void*` declarations can be
   removed with no codegen change.
3. **18 live shim wrappers remain** in `SpaceLounge.cpp`, `StarMap.cpp`, `StarSystem.cpp`,
   `RocketGun.cpp`, `MGame.cpp`, `MenuTouchWindow.cpp`, and `Globals.cpp`. They are `static inline`
   C-style wrappers around real typed methods. Each has a concrete direct-call replacement and a
   real callee address in the table below. No inline asm, no `__attribute__`, no `__declspec`, and no
   source-level computed-goto/jump table exist in `src/`; the manual link/data mechanisms are the
   ten `[[gnu::used]]` anchors, the invented 0x58-byte `.aezdata` section, the hand-shaped
   `.init_array`, and the dispatch tables in §5.2.

## 1. Genuine external ABI (keep)

All `extern "C"` sites are genuine. There are **43 textual `extern "C"` sites**, each with a
same-line waiver (`grep -RIn 'extern "C"' src | wc -l` = 43; `grep -RIn 'lint: extern_c' src |
wc -l` = 43). The blocks expand to many unmangled declarations (e.g. `GLES1Compat.cpp` alone has 24
non-implicit `extern "C"` functions, and the C TU `main.c` has 78), and every unmangled name was
verified against the original `.dynsym`/import set:

| Group | Files / examples | Evidence |
|---|---|---|
| JNI entry points | `main.c` (78 C functions, including `JNI_OnLoad` and the `Java_*` wrappers), `RecordHandler.cpp:115,121` (`Java_*_SetOrigamiSuperClub`, `..._ReleaseOrigamiSuperClub`), `NFC.cpp:5`, `NFC.h:61` | exported unmangled in original |
| NDK / platform | `NdkHooks.cpp` (37 `extern "C"` functions: `ndk23_*`, `ndk_*`, `ExitFunction`), `NdkHooks.h:29`, `ApkLoader.cpp:3,17` (`loadAPK`, `loadAPKAndZip`), `LODManager.cpp:48`/`LODManager.h:21` (`ndk_resetNativeItemInformationList`) | exported unmangled in original |
| Blowfish C ABI | `BlowfishSubkeyFile.cpp:5,8,11,14,16,21,41,63` (`S`, `P`, `SubkeyFile`, `F`, `opensubkeyfile`, `Blowfish_encipher`, `Blowfish_decipher`, `InitializeBlowfish`), `VirtualInput.cpp:794,797`/`VirtualInput.h:8` | exported unmangled in original |
| GLES1 compatibility | `GLES1Compat.cpp:14`, `Mesh.cpp:21` (`glVertexPointer`, `glTexCoordPointer`, `glNormalPointer`, `glColorPointer`), `Engine.cpp:5` (`glClientActiveTexture`) | defined locally, original exports the same names |
| libc | `PulseShader.cpp:10` `sinf` | original import `sinf` |

`sinf` is the only no-body declaration in the dead list that is genuine ABI. It is a redundant
local declaration (the file does not include `<cmath>`/`<math.h>`); keeping it is correct, but
`#include <cmath>` plus `std::sinf`/`::sinf` is the cleaner replacement.

## 2. Dead decompiler shims (delete)

These are file-scope free-function declarations with **no definition anywhere in `src/`**, **zero
`DeclRefExpr` call references**, and **no symbol in either binary**. They are safe to delete; no
call site or symbol parity is affected. Counts: **62 unique names / 65 declaration lines in `.cpp`**
plus **41 unique names / 41 lines in headers** = **103 unique names / 106 lines**. The three
duplicate `.cpp` names are `_mtw_ChoiceWindow_set` (two overloads), `_mtw_ArrayReleaseClasses_Str`
(two identical declarations), and `Status_replaceHash` (two signatures).

### 2.1 `.cpp` declarations

| file:line | declaration |
|---|---|
| `src/engine/core/AbyssEngine.cpp:71` | `AE_SpriteSystem_pushMatrix` |
| `src/engine/render/AEGeometry.cpp:5` | `Transform_GetTransform` |
| `src/game/core/String.cpp:23` | `String_concat` |
| `src/game/core/String.cpp:25` | `String_assign_op` |
| `src/game/menu/MiningGame.cpp:44` | `MiningGame_Status_getShip` |
| `src/game/menu/MiningGame.cpp:46` | `MiningGame_Ship_getFirstEquipmentOfSort` |
| `src/game/menu/MiningGame.cpp:52` | `MiningGame_Layout_getPulseValue` |
| `src/game/menu/MiningGame.cpp:54` | `MiningGame_Status_getShip_render` |
| `src/game/menu/MiningGame.cpp:58` | `MiningGame_Status_getCurrentCampaignMission_render` |
| `src/game/menu/ModStation.cpp:228` | `ChoiceWindow_setFee_frag` |
| `src/game/menu/ModStation.cpp:267` | `ChoiceWindow_setNotice_frag` |
| `src/game/menu/ModStation.cpp:269` | `ChoiceWindow_set6_frag` |
| `src/game/menu/ModStation.cpp:271` | `Status_holder_frag` |
| `src/game/menu/SpaceLounge.cpp:116` | `Station_getAgents` |
| `src/game/menu/SpaceLounge.cpp:122` | `ArrayRemove_AgentPtr` |
| `src/game/ui/Hud.cpp:23` | `Status_replaceHash` |
| `src/game/ui/Hud.cpp:27` | `Image2DCreate` |
| `src/game/ui/Hud.cpp:31` | `Hud_loadImages` |
| `src/game/ui/Hud.cpp:33` | `Hud_buildQuickMenu` |
| `src/game/ui/Layout.cpp:13` | `TouchButton_footerAnim` |
| `src/game/ui/Layout.cpp:15` | `Globals_drawLines` |
| `src/game/ui/MenuTouchWindow.cpp:88` | `_mtw_ChoiceWindow_set` (4-arg overload) |
| `src/game/ui/MenuTouchWindow.cpp:112` | `_mtw_Array_StrArr_ctor` |
| `src/game/ui/MenuTouchWindow.cpp:116` | `_mtw_ArraySetLength_StrArr` |
| `src/game/ui/MenuTouchWindow.cpp:120` | `_mtw_ArraySetLength_Str` |
| `src/game/ui/MenuTouchWindow.cpp:122` | `_mtw_Globals_longToTimeStringNoSeconds` |
| `src/game/ui/MenuTouchWindow.cpp:126` | `_mtw_Layout_formatCredits` |
| `src/game/ui/MenuTouchWindow.cpp:128` | `_mtw_TouchButton_ctor7` |
| `src/game/ui/MenuTouchWindow.cpp:141` | `_mtw_ChoiceWindow_OnTouchBegin` |
| `src/game/ui/MenuTouchWindow.cpp:145` | `_mtw_Layout_OnTouchBegin` |
| `src/game/ui/MenuTouchWindow.cpp:149` | `_mtw_TouchButton_OnTouchBegin` |
| `src/game/ui/MenuTouchWindow.cpp:153` | `_mtw_TouchButton_OnTouchBeginXY` |
| `src/game/ui/MenuTouchWindow.cpp:161` | `_mtw_TouchSlider_OnTouchBegin` |
| `src/game/ui/MenuTouchWindow.cpp:165` | `_mtw_ScrollTouchWindow_OnTouchBegin` |
| `src/game/ui/MenuTouchWindow.cpp:169` | `_mtw_MissionsWindow_OnTouchBegin` |
| `src/game/ui/MenuTouchWindow.cpp:173` | `_mtw_FModSound_play` |
| `src/game/ui/MenuTouchWindow.cpp:177` | `_mtw_idiv` |
| `src/game/ui/MenuTouchWindow.cpp:186` | `_mtw_ChoiceWindow_set` (3-arg overload) |
| `src/game/ui/MenuTouchWindow.cpp:202` | `_mtw_RecordHandler_readAllPreviewRecords` |
| `src/game/ui/MenuTouchWindow.cpp:216` | `_mtw_ScrollTouchWindow_update` |
| `src/game/ui/MenuTouchWindow.cpp:218` | `_mtw_MissionsWindow_update` |
| `src/game/ui/MenuTouchWindow.cpp:227` | `_mtw_ChoiceWindow_OnTouchMove` |
| `src/game/ui/MenuTouchWindow.cpp:231` | `_mtw_TouchButton_OnTouchMove` |
| `src/game/ui/MenuTouchWindow.cpp:235` | `_mtw_TouchButton_OnTouchMoveXY` |
| `src/game/ui/MenuTouchWindow.cpp:239` | `_mtw_TouchSlider_OnTouchMove` |
| `src/game/ui/MenuTouchWindow.cpp:243` | `_mtw_TouchSlider_getValue` |
| `src/game/ui/MenuTouchWindow.cpp:245` | `_mtw_ScrollTouchWindow_OnTouchMove` |
| `src/game/ui/MenuTouchWindow.cpp:249` | `_mtw_MissionsWindow_OnTouchMove` |
| `src/game/ui/MenuTouchWindow.cpp:251` | `_mtw_FModSound_setVolume` |
| `src/game/ui/MenuTouchWindow.cpp:263` | `_mtw_ImageFactory_drawShip` |
| `src/game/ui/MenuTouchWindow.cpp:278` | `_mtw_ArrayReleaseClasses_TB` |
| `src/game/ui/MenuTouchWindow.cpp:280` | `_mtw_ArrayReleaseClasses_Str` |
| `src/game/ui/MenuTouchWindow.cpp:290` | `_mtw_ArrayReleaseClasses_Str` (duplicate of line 280) |
| `src/game/ui/MenuTouchWindow.cpp:292` | `_mtw_ArrayReleaseClasses_StrArr` |
| `src/game/ui/MenuTouchWindow.cpp:296` | `_mtw_String_op_plus` |
| `src/game/ui/MenuTouchWindow.cpp:300` | `_mtw_TouchButton_ctorFull` |
| `src/game/ui/MenuTouchWindow.cpp:314` | `_mtw_steerFromTouchId` |
| `src/game/ui/MenuTouchWindow.cpp:318` | `_mtw_Layout_OnTouchMove` |
| `src/game/ui/MenuTouchWindow.cpp:320` | `_mtw_buildMenu` |
| `src/game/ui/StatusWindow.cpp:90` | `Status_replaceHash` (4-arg overload) |
| `src/game/world/StarMap.cpp:49` | `Station_getIndex` |
| `src/game/world/StarMap.cpp:73` | `FileRead_ctor` |
| `src/game/world/StarMap.cpp:75` | `FileRead_dtor` |
| `src/game/world/StarMap.cpp:91` | `Station_getTextureIndex` |
| `src/game/world/StarMap.cpp:107` | `SystemPathFinder_ctor` |

### 2.2 Header declarations

| file:line | declaration |
|---|---|
| `src/game/core/String.h:5-73` | 39 `String_*` declarations: `String_ctor`, `String_ctor_char`, `String_ctor_wchar`, `String_ctor_copy`, `String_ctor_int`, `String_ctor_longlong`, `String_ctor_float`, `String_ctor_charval`, `String_dtor`, `String_dtor_del`, `String_Set_char`, `String_Set_wchar`, `String_Set_longlong`, `String_Set_float`, `String_assign`, `String_addAssign_str`, `String_addAssign_char`, `String_addAssign_int`, `String_addAssign_longlong`, `String_addAssign_float`, `String_SubString`, `String_IndexOf_from`, `String_IndexOf`, `String_index`, `String_index_const`, `String_ReplaceString`, `String_ReplaceChar`, `String_Reverse`, `String_Trim`, `String_ToUpperCase`, `String_ToLowerCase`, `String_ConvertFromUTF8`, `String_Split`, `String_SplitTags`, `String_GetAEChar`, `String_ValueOf`, `String_StrLen_char`, `String_StrLen_wchar`, `String_PrintOut` |
| `src/engine/render/Trail.h:55,57` | `Trail_renderTransform`, `Trail_transformSetColor` |

### 2.3 Intended original callees for dead shims

The shim names themselves are absent from both binaries; the table gives the real original
function the shim was standing in for, where recoverable.

#### `_mtw_*` (36 unique names, all dead)

| declaration | intended original callee (ELF vaddr) |
|---|---|
| `_mtw_ArrayReleaseClasses_Str` | `ArrayReleaseClasses<String*>` @ `0x784e8` |
| `_mtw_ArrayReleaseClasses_StrArr` | `ArrayReleaseClasses<Array<String>*>` @ `0x125a70` |
| `_mtw_ArrayReleaseClasses_TB` | `ArrayReleaseClasses<TouchButton*>` @ `0xd923c` |
| `_mtw_ArraySetLength_Str` | `ArraySetLength<String*>` @ `0x7c68c` |
| `_mtw_ArraySetLength_StrArr` | `ArraySetLength<Array<String>*>` @ `0x125af2` |
| `_mtw_Array_StrArr_ctor` | `Array<Array<String>*>::Array()` @ `0x125ad6` |
| `_mtw_Array_Str_dtor` | `Array<String*>::~Array()` @ `0x73352` |
| `_mtw_ChoiceWindow_OnTouchBegin` | `ChoiceWindow::OnTouchBegin(int,int)` @ `0x1474cc` |
| `_mtw_ChoiceWindow_OnTouchMove` | `ChoiceWindow::OnTouchMove(int,int)` @ `0x147518` |
| `_mtw_ChoiceWindow_set` | `ChoiceWindow::set(String const&,String const&,bool)` @ `0x1468f4`; `set(String const&,bool)` @ `0x146d5c` |
| `_mtw_FModSound_play` | `FModSound::play(int,Vector*,Vector*,float)` @ `0x932f4` |
| `_mtw_FModSound_setVolume` | `FModSound::setVolume(int,float)` @ `0x92fb8` |
| `_mtw_Globals_longToTimeStringNoSeconds` | `Globals::longToTimeStringNoSeconds(long long,String&)` @ `0xe3c68` |
| `_mtw_ImageFactory_drawShip` | `ImageFactory::drawShip(int,int,int)` @ `0x11cad0` |
| `_mtw_Layout_OnTouchBegin` | `Layout::OnTouchBegin(int,int)` @ `0xd51a8` |
| `_mtw_Layout_formatCredits` | `Layout::formatCredits(int)` @ `0xd34d8` |
| `_mtw_MissionsWindow_OnTouchBegin` | `MissionsWindow::OnTouchBegin(int,int)` @ `0x150bdc` |
| `_mtw_MissionsWindow_OnTouchMove` | `MissionsWindow::OnTouchMove(int,int)` @ `0x150ca8` |
| `_mtw_MissionsWindow_update` | `MissionsWindow::update(int)` @ `0x1509b0` |
| `_mtw_RecordHandler_readAllPreviewRecords` | `RecordHandler::readAllPreviewRecords()` @ `0xcbb48` |
| `_mtw_ScrollTouchWindow_OnTouchBegin` | `ScrollTouchWindow::OnTouchBegin(int,int)` @ `0x174552` |
| `_mtw_ScrollTouchWindow_OnTouchMove` | `ScrollTouchWindow::OnTouchMove(int,int)` @ `0x174558` |
| `_mtw_ScrollTouchWindow_update` | `ScrollTouchWindow::update(int)` @ `0x17454c` |
| `_mtw_String_op_plus` | `AbyssEngine::String::operator+` (inline; no exported symbol) |
| `_mtw_TouchButton_OnTouchBegin` | `TouchButton::OnTouchBegin(int,int)` @ `0x1699d8` |
| `_mtw_TouchButton_OnTouchBeginXY` | `TouchButton::OnTouchBegin(int,int)` @ `0x1699d8` |
| `_mtw_TouchButton_OnTouchMove` | `TouchButton::OnTouchMove(int,int)` @ `0x169a28` |
| `_mtw_TouchButton_OnTouchMoveXY` | `TouchButton::OnTouchMove(int,int)` @ `0x169a28` |
| `_mtw_TouchButton_ctor7` | `TouchButton::TouchButton(String const&,int,int,int,char)` @ `0x1684f0` |
| `_mtw_TouchButton_ctorFull` | `TouchButton::TouchButton(String const&,int,int,int,int,char,char)` @ `0x168e88` |
| `_mtw_TouchSlider_OnTouchBegin` | `TouchSlider::OnTouchBegin(int,int)` @ `0xa242c` |
| `_mtw_TouchSlider_OnTouchMove` | `TouchSlider::OnTouchMove(int,int)` @ `0xa2494` |
| `_mtw_TouchSlider_getValue` | `TouchSlider::getValue()` @ `0xa23c0` |
| `_mtw_buildMenu` | `MenuTouchWindow` local helper (no exported symbol) |
| `_mtw_idiv` | `__aeabi_idiv` (import) |
| `_mtw_steerFromTouchId` | `MenuTouchWindow` local helper (no exported symbol) |

#### `String_*` (39 unique names, all dead)

| declaration | intended original callee (ELF vaddr) |
|---|---|
| `String_ctor` | `String::String()` @ `0x72398` |
| `String_ctor_char` | `String::String(char const*,bool)` @ `0x72488` |
| `String_ctor_wchar` | `String::String(uint16_t const*,bool)` @ `0x723b0` |
| `String_ctor_copy` | `String::String(String const&,bool)` @ `0x72564` |
| `String_ctor_int` | `String::String(int)` @ `0x737b4` (weak) |
| `String_ctor_longlong` | `String::String(long long)` @ `0x72904` (weak) |
| `String_ctor_float` | `String::String(float)` @ `0x729bc` (weak) |
| `String_ctor_charval` | `String::String(char)` @ `0x6e484` (weak) |
| `String_dtor` | `String::~String()` @ `0x72594` |
| `String_dtor_del` | `String::~String()` deleting @ `0x725b8` |
| `String_Set_char` | `String::Set(char const*)` @ `0x724b4` |
| `String_Set_wchar` | `String::Set(uint16_t const*)` @ `0x723dc` |
| `String_Set_longlong` | `String::Set(long long)` @ `0x72a0c` |
| `String_Set_float` | `String::Set(float)` @ `0x72cb0` |
| `String_assign` | `String::operator=(String const&)` @ `0x727a8` |
| `String_addAssign_str` | `String::operator+=(String const&)` @ `0x727d8` |
| `String_addAssign_char` | `String::operator+=(char const&)` @ `0x72850` |
| `String_addAssign_int` | `String::operator+=(int const&)` @ `0x72928` |
| `String_addAssign_longlong` | `String::operator+=(long long const&)` @ `0x7289c` |
| `String_addAssign_float` | `String::operator+=(float const&)` @ `0x72958` |
| `String_SubString` | `String::SubString(uint,uint)` @ `0x72ed8` |
| `String_IndexOf_from` | `String::IndexOf(uint,String const&)` @ `0x73004` |
| `String_IndexOf` | `String::IndexOf(String const&)` @ `0x73068` |
| `String_index` | `String::operator[](int)` @ `0x72378` |
| `String_index_const` | `String::operator[](int) const` @ `0x72788` |
| `String_ReplaceString` | `String::ReplaceString(String,String)` @ `0x73530` |
| `String_ReplaceChar` | `String::ReplaceChar(char,char)` @ `0x73070` |
| `String_Reverse` | `String::Reverse()` @ `0x722e8` |
| `String_Trim` | `String::Trim()` @ `0x731a8` |
| `String_ToUpperCase` | `String::ToUpperCase()` @ `0x730a4` |
| `String_ToLowerCase` | `String::ToLowerCase()` @ `0x73128` |
| `String_ConvertFromUTF8` | `String::ConvertFromUTF8()` @ `0x7270c` |
| `String_Split` | `String::Split(String)` @ `0x7322c` |
| `String_SplitTags` | `String::SplitTags(String)` @ `0x73368` |
| `String_GetAEChar` | `String::operator uint16_t*()` @ `0x725c8` |
| `String_ValueOf` | `String::ValueOf()` @ `0x727bc` |
| `String_StrLen_char` | `String::StrLen(char const*)` @ `0x729e0` |
| `String_StrLen_wchar` | `String::StrLen(uint16_t const*)` @ `0x729f0` |
| `String_PrintOut` | `String::PrintOut()` @ `0x73644` |

#### Other dead shims (28 unique names)

| declaration | intended original callee (ELF vaddr) |
|---|---|
| `AE_SpriteSystem_pushMatrix` | no exported symbol; no call site |
| `Transform_GetTransform` | `PaintCanvas::TransformGetTransform(unsigned)` @ `0x7b67c` |
| `Trail_renderTransform` | no exported symbol; no call site |
| `Trail_transformSetColor` | no exported symbol; no call site |
| `String_concat` | `String::operator+` (inline; no exported symbol) |
| `String_assign_op` | `String::operator=(String const&)` @ `0x727a8` |
| `MiningGame_Status_getShip` | `Status::getShip()` @ `0xa9658` |
| `MiningGame_Ship_getFirstEquipmentOfSort` | `Ship::getFirstEquipmentOfSort(int)` @ `0x174ce4` |
| `MiningGame_Layout_getPulseValue` | `Layout::getPulseValue(float)` @ `0xd4bf0` |
| `MiningGame_Status_getShip_render` | `Status::getShip()` @ `0xa9658` |
| `MiningGame_Status_getCurrentCampaignMission_render` | `Status::getCurrentCampaignMission()` @ `0xa966c` |
| `ChoiceWindow_setFee_frag` | `ChoiceWindow::set` overload family @ `0x1468c8`/`0x1468f4`/`0x146954`/`0x1469b0`/`0x146d5c` |
| `ChoiceWindow_setNotice_frag` | same `ChoiceWindow::set` family |
| `ChoiceWindow_set6_frag` | `ChoiceWindow::set(String,String,bool,String,String,String,int,int)` @ `0x1469b0` |
| `Status_holder_frag` | no exported symbol; no call site |
| `Station_getAgents` | `Station::getAgents()` @ `0xa6c70` |
| `ArrayRemove_AgentPtr` | `ArrayRemove<Agent*>` @ `0x16a850` |
| `Status_replaceHash` | `Status::replaceHash(String,String)` @ `0xac83c`; `replaceHash(String,String,String)` @ `0xac8d4` |
| `Image2DCreate` | `PaintCanvas::Image2DCreate(uint16_t,unsigned&)` @ `0x79c78` |
| `Hud_loadImages` | no exported symbol; no call site |
| `Hud_buildQuickMenu` | no exported symbol; no call site |
| `TouchButton_footerAnim` | no exported symbol; no call site |
| `Globals_drawLines` | `Globals::drawLines(uint16_t,Array<String*>*,int,int)` @ `0xe4688` |
| `Station_getIndex` | `Station::getIndex()` @ `0xa6bda` |
| `FileRead_ctor` | `FileRead::FileRead()` @ `0x11f944` |
| `FileRead_dtor` | `FileRead::~FileRead()` @ `0x11f946` |
| `Station_getTextureIndex` | `Station::getTextureIndex()` @ `0xa6c0c` |
| `SystemPathFinder_ctor` | `SystemPathFinder::SystemPathFinder()` @ `0x11b77c` |

### 2.4 Duplicate / obsolete declarations

- `_mtw_ChoiceWindow_set` is declared twice (`MenuTouchWindow.cpp:88` 4-arg, `:186` 3-arg); both
  are dead.
- `_mtw_ArrayReleaseClasses_Str` is declared twice, identically (`MenuTouchWindow.cpp:280,290`).
- `_mtw_Array_Str_dtor` has a `static inline` definition at `MenuTouchWindow.cpp:100` **and** a
  redundant declaration at `:108`; the latter is duplicate.
- `Status_replaceHash` is declared with two different signatures in `Hud.cpp:23` and
  `StatusWindow.cpp:90`; both are dead (calls are `Globals::status->replaceHash(...)`).
- `MatrixSetTranslation` is declared three times (`Globals.cpp:332`, `StarMap.cpp:60`,
  `SpaceLounge.cpp:63`); `MatrixSetRotation` twice (`StarMap.cpp:103`, `SpaceLounge.cpp:67`). All
  are dead; see §3.
- `sinf` (`PulseShader.cpp:10`) is a redundant local declaration of a genuine libc import.

## 3. Live shim wrappers (replace, do not simply delete)

These are `static inline` wrappers (or one helper) that are actually called. They are not
`extern` declarations, but they are the remaining decompiler shims in the call graph. Each can be
replaced by a direct typed call; the real callee is already defined in `src/`.

### 3.1 AEMath / Vector wrappers

| wrapper | file:line | call sites | real callee (ELF vaddr) | replacement |
|---|---|---|---|---|
| `MatrixGetRight(void*,void*)` | `SpaceLounge.cpp:95` | `SpaceLounge.cpp:1449` | `AEMath::MatrixGetRight(Matrix const&)` @ `0x80704` | `AbyssEngine::AEMath::MatrixGetRight(matrix)` |
| `MatrixGetPosition(void*,void*)` | `SpaceLounge.cpp:97` | `SpaceLounge.cpp:1462` | `AEMath::MatrixGetPosition(Matrix const&)` @ `0x8072e` | `AbyssEngine::AEMath::MatrixGetPosition(matrix)` |
| `MatrixGetUp(void*,void*)` | `SpaceLounge.cpp:99` | `SpaceLounge.cpp:1462` | `AEMath::MatrixGetUp(Matrix const&)` @ `0x80712` | `AbyssEngine::AEMath::MatrixGetUp(matrix)` |
| `MatrixGetLookAt(void*,void*,void*,void*)` | `SpaceLounge.cpp:101` | `SpaceLounge.cpp:1462` | `AEMath::MatrixGetLookAt(Vector const&,Vector const&,Vector const&)` @ `0x80f18` | `AbyssEngine::AEMath::MatrixGetLookAt(from,to,up)` |
| `MatrixGetDir(void*,void*)` | `SpaceLounge.cpp:105` | `SpaceLounge.cpp:1466,1467` | `AEMath::MatrixGetDir(Matrix const&)` @ `0x80720` | `AbyssEngine::AEMath::MatrixGetDir(matrix)` |
| `StarMap_MatrixGetPosition(const Matrix&)` | `StarMap.cpp:50` | `StarMap.cpp:1134,1155,1158` | `AEMath::MatrixGetPosition(Matrix const&)` @ `0x8072e` | direct namespaced call |
| `VectorNormalize(Vector*,Vector*)` | `StarMap.cpp:63` | `StarMap.cpp:493,1162` | `AEMath::VectorNormalize(Vector const&)` @ `0x73e3c` | `*out = AbyssEngine::AEMath::VectorNormalize(*value)` |
| `MatrixGetUp(Vector*,const Matrix*)` | `StarSystem.cpp:28` | `StarSystem.cpp:627,643` | `AEMath::MatrixGetUp(Matrix const&)` @ `0x80712` | direct namespaced call |
| `MatrixGetLookAt(Matrix*,const Vector*,const Vector*,const Vector*)` | `StarSystem.cpp:30` | `StarSystem.cpp:626,644` | `AEMath::MatrixGetLookAt(...)` @ `0x80f18` | direct namespaced call |
| `MatrixGetDir(Vector&,const Matrix&)` | `RocketGun.cpp:13` | `RocketGun.cpp:194` | `AEMath::MatrixGetDir(Matrix const&)` @ `0x80720` | direct namespaced call |
| `VectorCross(Vector&,const Vector&,const Vector&)` | `RocketGun.cpp:15` | `RocketGun.cpp:216,218,321` | `AEMath::VectorCross(...)` @ `0x73dfc` | direct namespaced call |
| `VectorNormalize(Vector&,const Vector&)` | `RocketGun.cpp:17` | `RocketGun.cpp:217,219,323,332` | `AEMath::VectorNormalize(...)` @ `0x73e3c` | direct namespaced call |

Dead AEMath wrappers/declarations to delete: `src/game/mission/Explosion.cpp:13`
(`MatrixGetPosition` wrapper, unused), `src/game/menu/SpaceLounge.cpp:55` (`MatrixMultiply`
declaration, unused), and the dead
`MatrixSetTranslation`/`MatrixSetRotation` `void*` declarations listed in §2.4. The AST proof that
the `void*` declarations are not the bound callees:

```text
StarMap.cpp:258 CallExpr -> DeclRefExpr 'Matrix (Matrix &, float, float, float)' Function
  'MatrixSetTranslation' 'Matrix (Matrix &, float, float, float)'
SpaceLounge.cpp:257 CallExpr -> DeclRefExpr 'Matrix (Matrix &, float, float, float)' Function
  'MatrixSetRotation' 'Matrix (Matrix &, float, float, float)'
```

The real typed overloads are `MatrixSetTranslation(Matrix&,float,float,float)` @ `0x80e48` and
`MatrixSetRotation(Matrix&,float,float,float)` @ `0x80918`.

### 3.2 Menu / camera / sound wrappers

| wrapper | file:line | call sites | real callee (ELF vaddr) | replacement |
|---|---|---|---|---|
| `TFC_setLookAtCam` | `MGame.cpp:61` | `MGame.cpp:2166,2196` | `TargetFollowCamera::setLookAtCam(bool)` @ `0x15b49e` | `camera->setLookAtCam(v)` |
| `TFC_setActive` | `MGame.cpp:68` | `MGame.cpp:2165` | `TargetFollowCamera::setActive(bool)` @ `0x15a99a` | `camera->setActive(v)` |
| `TFC_setPosition` | `MGame.cpp:70` | `MGame.cpp:2236` | `TargetFollowCamera::setPosition(float,float,float)` @ `0x15a9d0` | `camera->setPosition(x,y,z)` |
| `_mtw_GameText_getText` | `MenuTouchWindow.cpp:86` | `MenuTouchWindow.cpp:1204,1219,1220,3830,3831` | `GameText::getText(int)` @ `0x81818` | `gt->getText(id)` |
| `_mtw_GameRecord_dtor` | `MenuTouchWindow.cpp:182` | `MenuTouchWindow.cpp:1193` | `GameRecord::~GameRecord()` @ `0x1559d4` | `delete rec;` (or explicit dtor + `operator delete`, whichever matches the original schedule) |
| `nextInt_71ad0` | `Globals.cpp:330` | `Globals.cpp:1140,1156,1166,1176` | `AERandom::nextInt(int)` @ `0x71ad0` | `Globals::rnd->nextInt(bound)` |

Dead inline wrappers to delete while touching these files:

- `MenuTouchWindow.cpp`: `_mtw_Array_TB_dtor` (96), `_mtw_Array_TS_dtor` (98), `_mtw_Array_Str_dtor`
  (100), `_mtw_TouchButton_dtor` (102), `_mtw_ChoiceWindow_dtor` (104),
  `_mtw_ScrollTouchWindow_dtor` (106), `_mtw_Array_StrArr_dtor` (110), `_mtw_Array_Str_ctor` (114),
  `_mtw_Status_resetGame` (132), `_mtw_RecordHandler_dtor` (180), `_mtw_GameRecord_load` (184),
  `_mtw_AppMgr_GetApplicationModule` (190), `_mtw_AppMgr_GetEngine` (204),
  `_mtw_Array_TB_ctor` (255). All have zero call references.
- `MiningGame.cpp`: `MiningGame_FModSound_setParamValue` (31), `MiningGame_FModSound_play` (33),
  `MiningGame_FModSound_stop` (36), `MiningGame_Achievements_hasMedal` (38),
  `MiningGame_Achievements_getValue` (40), `MiningGame_Hud_hudEventMedal` (42),
  `MiningGame_Item_getAttribute` (48), `MiningGame_Status_getCurrentCampaignMission` (50),
  `MiningGame_Ship_getFreeSpace` (56). All have zero call references; the real methods are
  `FModSound::setParamValue(int,int,float)` @ `0x92f20`, `FModSound::play` @ `0x932f4`,
  `FModSound::stop(int)` @ `0x93568`, `Achievements::hasMedal` @ `0x157008`,
  `Achievements::getValue` @ `0x157018`, `Hud::hudEventMedal` @ `0x1625a0`,
  `Item::getAttribute` @ `0xe01b4`, `Status::getCurrentCampaignMission` @ `0xa966c`,
  `Ship::getFreeSpace` @ `0x174c7c`.

## 4. `extern` variables (52 non-`extern "C"` declarations, all genuine)

All 52 file-scope `extern` object declarations outside `extern "C"` blocks have a real definition;
all but `hrwhs_country_code` have a real use, and that one is an exported original global that must
stay. None is a decompiler shim. (The four `extern "C"` object declarations — `S`, `P`,
`SubkeyFile` in `BlowfishSubkeyFile.cpp` and `S` in `VirtualInput.cpp` — are covered in §1.) They
fall into three groups:

| Group | Declarations | Definitions | Original symbol evidence |
|---|---|---|---|
| Platform/JNI state | `NdkHooks.h:7-26` (`g_pEnv`, `gi_iap_buy_*`, `hrwhs_country_code`, `gb_android_offerwallCreditAmount`), `NFC.h:59`, `ModStation.cpp:10`, `MenuTouchWindow.cpp:266-269`, `Globals.cpp:335` | `main.c` / `NdkHooks.cpp` | exported `g_pEnv`, `gi_iap_buy_*`, `hrwhs_country_code`, `gb_android_offerwallCreditAmount`, `g_android_*` |
| Engine globals | `AbyssEngine.h:38-66` (28 flags/counters), `Engine.h:25` (`PostEffectFlag`), `Engine.h:421` (`gEngine`) | `AbyssEngine.cpp:41-69`, `Engine.cpp:10,21` | exported `_ZN11AbyssEngine*` / `gEngine` |
| Zip handles | `FileInterfaceAndroid.h:13,14` (`APKArchive`, `ZIPArchive`) | `FileInterfaceAndroid.cpp:314,316` | exported `APKArchive`, `ZIPArchive` |

`hrwhs_country_code` is unused in our source but is an exported original global, so it must stay.
`SwapCounter` exists both as `AbyssEngine::SwapCounter` (`AbyssEngine.h:43` /
`AbyssEngine.cpp:46`) and `Engine::SwapCounter` (`Engine.h:392` / `Engine.cpp:1777`); these are
distinct mangled symbols, not a duplicate.

## 5. Inline asm, attributes, jump tables, manual link/data

### 5.1 Inline asm / compiler attributes

- **No inline asm** in `src/` (no `asm`, `__asm`, `__asm__`, `asm volatile`).
- **No `__attribute__` or `__declspec`** in `src/`.
- **7 `[[gnu::used]]` sites** (C++11 GNU attribute spelling; lint does not flag it, but it is the
  only attribute mechanism in the tree):

| file:line | symbol | purpose |
|---|---|---|
| `src/game/world/StarMap.cpp:55` | `StarMap_GetScreenPosition` | layout/anchor |
| `src/game/world/StarSystem.cpp:133` | `starSystemLayoutAnchor` | layout anchor |
| `src/game/mission/Generator.cpp:53` | `generatorShipListLayoutAnchor` | layout anchor |
| `src/game/mission/RecordHandler.cpp:1417` | `mixRecordStoreSlot` | layout anchor |
| `src/game/weapons/RocketGun.cpp:40` | `kRocketTrailSets[4]` | data-table anchor |
| `src/engine/file/AEFile.cpp:8` | `pakExtension[8]` | `.aezdata` anchor |
| `src/engine/math/Transform.cpp:16` | `g_transform_matrix_flag[80]` | `.aezdata` anchor |

### 5.2 Manually linked data / sections

- **`.aezdata` is an invented section** created with `#pragma clang section data=".aezdata"`,
  holding `pakExtension[8]` (`AEFile.cpp:7-9`) and `g_transform_matrix_flag[80]`
  (`Transform.cpp:15-17`). Our `.so` has
  `.aezdata` size `0x58`; the original has no `.aezdata` section. This is a deliberate
  address-layout anchor (comments in `Transform.cpp:8-14` explain the `.got`/`.data` distance).
  It is not a shim, but it is a real deviation from the original section list. `docs/IDENTICALITY.md:40`
  mentions the `.aezdata` anchor in the `.data` delta table, but the section itself is not listed
  there; consider adding it explicitly.
- **`.init_array` is hand-shaped** (3 entries in both binaries):
  `Engine::tv_modes`/`renderer`/`vendor` (`Engine.cpp:1791-1798`), the touch buffer
  `touches = realloc(touches, sizeof(Touch))` (`LODManager.cpp:12`), and
  `ParticleSettingsRef::init`/`::cur` (`ParticleSettings.cpp:702-704`).
- **`#pragma pack`** appears at `Globals.cpp:7-89` and `Globals.cpp:337-348` for the
  `GameSettingsRecord`/`HintsBuffer` overlays; these are layout pragmas, not link data.
- **Manual dispatch/data tables** (not jump tables):
  - `Level.cpp:175-186` `PlayerEgoVtable` overlay (`slot[7]` + `placeAtStation` at `0x1c`);
  - `ObjectGun.cpp:15` `MUZZLE_MESHES[233]`;
  - `RocketGun.cpp:40` `kRocketTrailSets[4]`;
  - `MiningGame.cpp:20-29` `LAYER_SPEEDS[7]`, `LAYER_DIAMETERS[49]`;
  - function-pointer typedefs/slots in `StarMap.cpp:81-95`, `Sparks.cpp:99`,
    `MiningGame.cpp:140`, `MenuTouchWindow.cpp:307`, `LevelScript.cpp:55-61`.
- **No source-level manual jump table**: no `goto *`, no label-address array (`&&label`), no
  `__label__`. All `switch` statements are ordinary C++ and are compiled to `tbb/tbh` by clang.

## 6. Actionable next steps (bounded, one worker each)

1. **Mechanical dead-declaration deletion (lowest risk).** Delete the 65 `.cpp` declaration lines in
   §2.1 and the 41 header declarations in §2.2 (103 unique names / 106 lines). No call sites, no
   definitions, no symbol-table entries. Expected gate effect: none (`byte_exact`/`linked_exact`
   unchanged); lint `void_ptr` waivers drop. Suggested commit: `extern shim cleanup: drop 103 dead
   shim names (106 lines)`.
2. **Delete the dead `void*` Matrix declarations.** Remove `Globals.cpp:332`, `StarMap.cpp:60,103`,
   `SpaceLounge.cpp:63,67`. AST proves calls bind to the typed overloads; expected codegen-neutral.
   Do this separately from step 1 so a codegen surprise is easy to bisect.
3. **Replace live wrappers with direct typed calls.** Start with `SpaceLounge.cpp:95-105` and
   `StarMap.cpp:50,63` (AEMath wrappers), then `MGame.cpp:61-70` (TFC), then
   `MenuTouchWindow.cpp:86,182` and `Globals.cpp:330`. Use the addresses in §3; run `verify-fn` on
   the containing functions and the full `check` gate after each file.
4. **Delete dead inline wrappers** in `MenuTouchWindow.cpp` and `MiningGame.cpp` (§3.2) while
   touching those files; they are zero-reference and cannot affect codegen.
5. **Keep `sinf`** but replace the local `extern "C"` declaration with `<cmath>`/`<math.h>` if the
   include is acceptable for the TU; otherwise leave it as genuine ABI.
6. **Audit `.aezdata` separately.** It is the only invented section and is not a shim; confirm it is
   documented in `docs/IDENTICALITY.md` and keep its 0x58-byte layout intact.

## Appendix A — real cross-TU declarations (not shims)

The remaining 118 no-body `.cpp` declarations (63 unique names), excluding the 27 `main.c`
prototypes and the 5 same-TU forwards listed below, comprise 5 dead `void*` Matrix declarations
(`MatrixSetTranslation` ×3, `MatrixSetRotation` ×2; 2 names), one duplicate `_mtw_Array_Str_dtor`
declaration (`MenuTouchWindow.cpp:108`, duplicate of the inline definition at line 100, already
noted in §2.4), and 112 real cross-TU declarations (60 unique names). The list below covers all of
them so the audit is exhaustive.

`ApplicationManager.cpp`: `TextureCreateFromFileIntern`; `BSphere.cpp`: `MatrixRotateVector`,
`MatrixTransformVector`; `BlowfishSubkeyFile.cpp`: `F`; `BuildResourceList.cpp`:
`loadLowTexturesAndMaterials`, `loadPortraits`; `ConfigReader.cpp`: `ReadKeyMap`, `ReadTimer`,
`ReadVibration`; `Engine.cpp`: `MeshCreate`, `MeshRelease`, `esMatrixMultiply`,
`glClientActiveTexture`; `Image.cpp`: `ImageFontRelease`, `ImageRelease`, `MeshCreate`,
`MeshRelease`, `TextureCreateFromFileIntern`; `Image2D.cpp`: `MeshRelease`; `ImageFont.cpp`:
`ImageFontDrawString`, `ImageFontGetHeight`, `ImageFontGetWidth`, `ImageFontGetYOffset`, `MeshDraw`,
`MeshRelease`; `MGame.cpp`: `MatrixRotateVector`, `VectorLength`, `operator-`; `Material.cpp`:
`MeshDraw`; `Mesh.cpp`: `MeshConvertToVBO`, `MeshConvertToVBOIntern`, `MeshReadData`, `MeshRelease`,
`MeshReleaseIntern`, `TransformConvertToVBO`, `TransformRelease`, `glColorPointer`,
`glNormalPointer`, `glTexCoordPointer`, `glVertexPointer`; `ModMainMenu.cpp`: `Sinf`;
`MovingStars.cpp`: `MatrixTransformVector`; `PaintCanvas.cpp`: `CameraIsPointinViewFrustum`,
`CameraIsSphereinViewFrustum`, `CameraSetPerspective`, `Image2DRelease`, `ImageCreateFontFromFile`,
`ImageCreateRegionFromFile`, `ImageFontCheckString`, `ImageFontDrawString`, `ImageFontGetHeight`,
`ImageFontGetSpacing`, `ImageFontGetWidth`, `ImageFontGetYOffset`, `ImageFontRelease`,
`ImageFontSetSpacing`, `ImageFontSetYOffset`, `MaterialDraw`, `MeshConvertToVBO`, `MeshCreate`,
`MeshCreateFromFile`, `MeshDraw`, `MeshIntersect`, `MeshRelease`, `SpriteSystemCreate`,
`SpriteSystemDraw`, `SpriteSystemRelease`, `SpriteSystemSetAllSize`, `SpriteSystemSetAllUv`,
`SpriteSystemSetRGBA`, `SpriteSystemSetUv`, `TextureCreateFromFile`,
`TextureCreateFromFileIntern`; `Player.cpp`: `VectorLength`, `operator*`, `operator-`;
`PlayerEgo.cpp`: `operator*`; `PlayerFighter.cpp`: `VectorLength`, `operator*`, `operator-`;
`PlayerFixedObject.cpp`: `VectorLength`; `PlayerGasCloud.cpp`: `VectorLength`;
`PlayerTurret.cpp`: `MatrixGetDir`, `MatrixInverseTransformVector`, `MatrixRotateVector`,
`VectorLength`, `VectorNormalize`, `operator*`, `operator+`, `operator-`; `RocketGun.cpp`:
`operator*`, `operator+`, `operator-`; `SpaceLounge.cpp`: `MatrixMultiply`, `operator*`,
`operator+`, `operator-`; `SpriteSystem.cpp`: `MeshCreate`, `MeshDraw`, `MeshRelease`;
`StarSystem.cpp`: `operator*`, `operator+`; `Transform.cpp`: `CameraIsSphereinViewFrustum`;
`main.c`: `ndk23_*`, `ndk_*` platform entry points.

`MatrixSetTranslation`/`MatrixSetRotation` in `Globals.cpp`, `StarMap.cpp`, and `SpaceLounge.cpp`
are counted in this appendix by name, but their `void*` declarations are dead shims (§2.4/§3.1).

Same-TU static forward declarations (real functions defined later in the same file; not shims):
`GameInit.cpp:23 cheatFunction`, `ModStation.cpp:50 modStationTouchEndPad`,
`MenuTouchWindow.cpp:326 mtwTextLayoutAnchor`, `Level.cpp:189 levelSpawnFar`, and
`Level.cpp:1601 levelTextAnchor`.

## Appendix B — command outputs used for the counts

```text
$ nm -D --undefined-only cmake-build-match/libgof2hdaa.so | ... | wc -l
207
$ nm -D --undefined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | ... | wc -l
207
$ nm -D --defined-only cmake-build-match/libgof2hdaa.so | ... | wc -l
5370
$ nm -D --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | ... | wc -l
5370
```

`clang-query` no-body/definition matcher totals for the `.cpp` inventory (199 TUs, main file,
non-implicit; operators included):

```text
total no-body declarations:       216 declarations / 158 unique names
  §2 dead .cpp:                    65 declarations /  62 unique names
  genuine libc sinf:                1 declaration  /   1 unique name
  same-TU static forwards:          5 declarations /   5 unique names
  main.c platform prototypes:      27 declarations /  27 unique names
  remaining unclassified:         118 declarations /  63 unique names
    (5 Matrix void* + 1 duplicate _mtw_Array_Str_dtor + 112 real cross-TU / 60 names)
header no-body free FunctionDecls:  41 unique names (39 String_*, 2 Trail_*)
```

All 103 dead names (excluding `sinf`) were checked against both `.dynsym` sets and are absent from
both; all had `DeclRefExpr` count 0. `sinf` had 30 references and is an import in both binaries.
The 103 dead names are 62 `.cpp` + 41 header names. The 5 Matrix `void*` declarations are separate
(2 names, 5 lines) and are counted in the 118/63 remaining-unclassified group, together with the
one duplicate `_mtw_Array_Str_dtor` line and 112/60 real cross-TU declarations.
