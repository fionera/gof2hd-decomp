# Cross-artifact mining (wave 102, wm104): the KiritoJPK 7z APK set, the mod rebuild, and the iOS 1.1.19 source order

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/wm104`,
branch `wave102-wm104`, base `53d9ee4d5`. Scratch: `/dev/shm/wm104` only.
**No source or build-flag change is landed.** Every experiment below was measured with the
official verifier and reverted; the note is the deliverable. The canonical gate is unchanged
(scratch-build baseline: `byte 4500`, `linked 4500`, `size 4524/4524`, `data 843/843`,
`imports 207`, `RATCHET PASS`).

## 0. Bottom line

* The two KiritoJPK 7z archives were extracted. `GOF2FHD_APK.7z` contains **14 APKs** and
  `GOF2FHD_OBB.7z` contains the **1.6 GB game-data OBB**. The OBB is a plain zip of 2,460
  game-data files; it has **no** `.so`, `.o`, `.a`, `.d`, build log, project file or source
  path.
* The 14 APKs carry **four distinct `libgof2hdaa.so` files**:
  * **Stock 2.0.16** (`Save fix/GOF2_2.0.16_For_Save_Fix.apk`): the original NDK r18b /
    clang 7.0.2 / gold 1.12 lib, byte-identical to the ground truth except for a **4-byte
    patch** at VMA `0xcd7b0` that turns `RecordHandler::checkHash(unsigned int)` into
    `return 1` (`f0 b5 03 af` -> `01 20 70 47`). This is a second copy of the original.
  * **A full source rebuild** (`GOF2_2.0.16_{Android,Original}_Price*.apk`,
    `Experimental/Post-processing FX (Bloom)/*`, `Experimental/New system/*`): 2,469,656 B,
    **NDK r14 / clang 3.8.275480 / gold 1.11**, `-O2` (`Tag_ABI_optimization_goals:
    Aggressive Speed`), **system libstdc++** (`DT_NEEDED libstdc++.so`), no
    `.note.android.ident`, 26 sections. All three variants share the same build id
    `1dbb7d45...` and differ only by small post-link patches.
* The source rebuild is a **second compilation of the same game source family**:
  4,245 exported functions are common to it and the ground truth, and their `.text` order is
  **99.32 % LIS-identical** (4,216/4,245; the 9 adjacent descents are all STL/template
  aliases). All **24 non-exact functions are present and in the same relative order**.
* Its `.rodata` string set is **95.9 % Jaccard-identical** to the original. The only game-text
  differences are the four localization strings
  `Galaxy on Fire 2 HD`/`Datenschutzb.`/`Nutzungsb.`/`Terms of S.` (original) vs
  `Galaxy on Fire 2 FHD`/`Datenschutzrichtlinie`/`Nutzungsbestimmungen`/`Terms of Service`
  (rebuild). The rebuild also lacks the three Android `DeviceInfo` JNI strings. The code is
  the same family; treat the rebuild as a **code oracle with a localization/DeviceInfo
  caveat**, not as a byte oracle.
* The rebuild's author (KiritoJPK) also publishes a **full Ghidra decompilation of this exact
  rebuild** in the `Galaxy-on-Fire-2-Unity-Remake` GitHub repo
  (`Reference/decompiled/native/game/*.c`, `_FUNCTIONS.tsv`, `INDEX.md`). This is the closest
  thing to debug info in any shipped artifact and is a directly usable second decompilation
  for the 24 functions. The repo's `Reference/README.md` explicitly says the code came from
  `GOF2_2.0.16_Android_Price.apk` (the mod rebuild).
* The **iOS 1.1.19 source-layout dump** is a reliable source-order witness: per-TU LIS against
  the Android address order is 0.83–1.00, and **within each TU all 24 target functions are in
  the same relative order** in iOS 1.1.19 and in the Android 2.0.16 binary (the global order
  differs because the iOS and Android link orders differ). The few per-TU inversions are
  template instantiations / inlined helpers, not the target functions.
* No shipped artifact contains DWARF, `.symtab`, `.d`, `.o`, `.a`, build logs, or a project
  file. The only compiler evidence is `.comment`, `.note.gnu.gold-version`,
  `.ARM.attributes`, `DT_NEEDED`, and the `ndk-r14-release` libc++abi path inside the rebuild.
* Three controlled A/B experiments were run and reverted because none was a strict
  improvement:
  1. Hoisting `Globals::Globals`'s `v54/v58` initializers to the top of the constructor:
     `byte 4500 -> 4500`, `Globals::Globals` 80.5 -> 80.5 %. Neutral.
  2. Removing the `.missioncase` rank table and switching directly on `m->getType()` with the
     inverted original type labels: `byte 4500 -> 3043`, `linked 4500 -> 4500`,
     `data 843 -> 843`. The 192-byte table is **load-bearing for `.rodata` layout**, not just
     for codegen. Reverted.
  3. Making the `Globals::Globals` `resWidth` default an early direct store (the Unity
     decompilation's shape): `byte 4500 -> 2547`, `Globals::Globals` 80.5 -> 29.8 %,
     size 612 -> 628. Reverted.

## 1. Extraction commands

```sh
mkdir -p /dev/shm/wm104/apk7z /dev/shm/wm104/obb7z
7z x -y -o/dev/shm/wm104/apk7z \
  "/home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_APK.7z"
7z x -y -o/dev/shm/wm104/obb7z \
  "/home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_OBB.7z"

# exact lib extraction (unzip -p avoids the duplicate-name/space pitfalls of a shell loop)
unzip -p "/dev/shm/wm104/apk7z/Save fix/GOF2_2.0.16_For_Save_Fix.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/wm104/libs_clean/stock_savefix.so
unzip -p "/dev/shm/wm104/apk7z/GOF2_2.0.16_Android_Price.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/wm104/libs_clean/mod_main.so
unzip -p "/dev/shm/wm104/apk7z/Experimental/Post-processing FX (Bloom)/GOF2_2.0.16_Android_Price.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/wm104/libs_clean/mod_bloom.so
unzip -p "/dev/shm/wm104/apk7z/Experimental/New system/GOF2_2.0.16_Original_Price.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/wm104/libs_clean/mod_newsystem.so
```

The 7z APK archive has 27 files / 5 folders; the OBB archive has one 1,625,421,767-byte
`main.47947006...obb`. The 14 APKs are:

```text
GOF2_2.0.16_Android_Price.apk
GOF2_2.0.16_Android_Price_30fps.apk
GOF2_2.0.16_Android_Price_For_Emulator.apk
GOF2_2.0.16_Original_Price.apk
GOF2_2.0.16_Original_Price_30fps.apk
GOF2_2.0.16_Original_Price_For_Emulator.apk
Experimental/Post-processing FX (Bloom)/{same six}
Experimental/New system/GOF2_2.0.16_Original_Price.apk
Save fix/GOF2_2.0.16_For_Save_Fix.apk
```

## 2. The four distinct libs

```sh
sha256sum /dev/shm/wm104/libs_clean/*.so
```

| lib | sha256 | size | build id | compiler / linker |
|---|---|---:|---|---|
| stock Save fix | `7ce001c543b751a47a1f7ce5185ab98eb66c800f0c524fd94072d1d1f8541930` | 2,187,328 | `c011810d...` | clang 7.0.2, gold 1.12, NDK r18b |
| mod main | `9a166d19b9fde9552e2b061183909fa2f8d86467b69713403a33a4612f402fad` | 2,469,656 | `1dbb7d45...` | clang 3.8.275480, gold 1.11, NDK r14 |
| mod Bloom | `b125c91836802e563f08aaee3c2d2e4c6ff748988c223125084308112da5d95f` | 2,469,656 | `1dbb7d45...` | same base rebuild |
| mod New system | `9183a68b3a76ce83880df2c044b694f21969c9fb5df190f2b1abd26e5b9c3187` | 2,469,656 | `1dbb7d45...` | same base rebuild |

The three mod libs differ from each other by only 18–284 bytes (mostly `.shstrtab`/section
names plus a few post-link code patches). They are one base rebuild with small mods.

### 2.1 The stock Save fix lib is the original + one 4-byte patch

```sh
cmp -l /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
       /dev/shm/wm104/libs_clean/stock_savefix.so
# 292 lines total; the 4 in .text are:
# 841649 360 1
# 841650 265 40
# 841651 3 160
# 841652 257 107
# the other 288 are .shstrtab/sh_name (raw bfd order, same as the APKPure APK lib)
```

File offset `0xcd7b0` is VMA `0xcd7b0` (`.text` is file-offset == VMA). The ground truth has
`f0 b5 03 af` (`push {r4,r5,r6,r7,lr}; add r7,sp,#12`); the Save fix lib has
`01 20 70 47` (`movs r0,#1; bx lr`). The containing symbol is
`_ZN13RecordHandler9checkHashEj` at `0xcd7b1` (Thumb), size 280. `cmp -l` reports **292**
differing bytes: those 4 in `.text` plus 288 in `.shstrtab`/`sh_name`. The 288 are exactly the
raw-bfd-strip vs `tools/reorder_shstrtab.py` difference that the APKPure APK lib also has
relative to the ground truth (wave-100 §1.2), so the Save fix lib is a raw bfd-strip artifact.
Every checked section other than `.text`/`.shstrtab` is byte-identical:

```sh
for sec in .text .rodata .data .ARM.exidx .ARM.extab .dynsym .dynstr; do
  # NOTE: llvm-objcopy with no output operand rewrites the input in place (and
  # canonicalizes .shstrtab); pass /dev/null to keep the input untouched.
  llvm-objcopy --dump-section $sec=orig.sec /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so /dev/null
  llvm-objcopy --dump-section $sec=stock.sec /dev/shm/wm104/libs_clean/stock_savefix.so /dev/null
  echo -n "$sec "; cmp -s orig.sec stock.sec && echo SAME || echo DIFF
done
```

Result: `.rodata`, `.data`, `.ARM.exidx`, `.ARM.extab`, `.dynsym`, `.dynstr` are all `SAME`;
`.text` differs only in those 4 bytes. This is a second, independently shipped copy of the
original 2.0.16 code.

### 2.2 The mod rebuild's ELF metadata

```sh
readelf -hW /dev/shm/wm104/libs_clean/mod_main.so
readelf -nW /dev/shm/wm104/libs_clean/mod_main.so
readelf -p .comment /dev/shm/wm104/libs_clean/mod_main.so
readelf -dW /dev/shm/wm104/libs_clean/mod_main.so | grep NEEDED
readelf -AW /dev/shm/wm104/libs_clean/mod_main.so
```

| observable | original 2.0.16 | mod rebuild |
|---|---|---|
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123` | `Android clang version 3.8.275480 (based on LLVM 3.8.275480)` + `GCC: (GNU) 4.9.x 20150123` |
| `.note.android.ident` | present, `api=16`, `ndk="r18b"`, `build="5063045"` | **absent** |
| `.note.gnu.gold-version` | `gold 1.12` | `gold 1.11` |
| `.ARM.attributes` | no `Tag_ABI_optimization_goals`, no `Tag_Virtualization_use` | `Tag_ABI_optimization_goals: Aggressive Speed`, `Tag_Virtualization_use: TrustZone` |
| `DT_NEEDED` | `... libm.so libdl.so libc.so` | `... libm.so libstdc++.so libdl.so libc.so` |
| sections | 27 | 26 |
| `.init_array` | `0xc` (3 entries) | `0x10` (4 entries) |
| `libc++abi` path in `.rodata` | `/Volumes/.../ndk-release-r18/.../abort_message.cpp` | `/Volumes/.../ndk-r14-release/.../abort_message.cpp` |

The `libstdc++.so` NEEDED and the 4-entry `.init_array` are consistent with an `ndk-build`
`APP_STL := system` / `-O2` build, not with the original CMake `c++_static` `-Oz` build
(the latter inference is a hypothesis, not a proven build-system fact).

### 2.3 Section sizes

```sh
readelf -SW <lib> | grep -E '\.text|\.rodata|\.data|\.bss|\.ARM|\.init_array'
```

| section | original | stock Save fix | mod rebuild |
|---|---:|---:|---:|
| `.text` | `0x1337c8` | `0x1337c8` | `0x1682c8` |
| `.rodata` | `0x41b67` | `0x41b67` | `0x427d4` |
| `.data` | `0x296c` | `0x296c` | `0x2944` |
| `.bss` | `0x69ac` | `0x69ac` | `0x6854` |
| `.ARM.exidx` | `0x6f70` | `0x6f70` | `0x5fb8` |
| `.ARM.extab` | `0x26298` | `0x26298` | `0x3f590` |
| `.init_array` | `0xc` | `0xc` | `0x10` |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | `0x200c` |
| `.got` | `0x430c` | `0x430c` | `0x3bc0` |

The rebuild's `.text` is 17.1 % larger (clang 3.8 at `-O2` vs clang 7 at `-Oz`), so its
function sizes are **not** a size oracle. Its function **order** and **call graph** are the
useful signal.

## 3. The mod rebuild is a full source rebuild, not a patch

```sh
python3 - <<'PY'
import subprocess,bisect
def syms(path):
    out=subprocess.check_output(['readelf','--dyn-syms','-W',path],text=True)
    d={}
    for line in out.splitlines():
        p=line.split()
        if len(p)>=8 and p[3]=='FUNC' and p[6]!='UND':
            try: d[p[7]]=int(p[1],16)
            except: pass
    return d
O=syms('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
M=syms('/dev/shm/wm104/libs_clean/mod_main.so')
common=set(O)&set(M)
oo=sorted(common,key=lambda n:O[n]); mo=sorted(common,key=lambda n:M[n])
pos={n:i for i,n in enumerate(mo)}; seq=[pos[n] for n in oo]
tails=[]
for x in seq:
    i=bisect.bisect_left(tails,x)
    if i==len(tails): tails.append(x)
    else: tails[i]=x
print('orig',len(O),'mod',len(M),'common',len(common),'LIS',len(tails),len(tails)/len(seq))
PY
```

Measured: **original 4,524 functions, mod 4,249, common 4,245, LIS 4,216/4,245 = 99.32 %**.
The 9 adjacent descents are all STL/template aliases:

```text
Array<Transform*>::~Array -> Array<Mesh*>::~Array
Array<Mesh*>::~Array -> Transform::Transform(Transform*)
Array<int>::~Array -> GameText::~GameText
std::terminate -> std::unexpected
__dynamic_cast -> std::exception::~exception
std::bad_exception::~bad_exception -> std::bad_array_new_length::~bad_array_new_length
std::exception::~exception -> std::bad_array_length::~bad_array_length
std::bad_exception::what -> std::bad_alloc::bad_alloc
std::bad_array_length::what -> operator new(unsigned int)
```

In the sampled set, the 279 original-only functions are mostly `D0` deleting destructors, `C2`
constructors, template instantiations and libc++-specific symbols (e.g.
`_ZN11AbyssEngine10BlurShaderD0Ev`, `_ZN11AbyssEngine6AEMath6MatrixC2Ev`,
`_ZN11AbyssEngine11PaintCanvas12FindResourceEt`). Every one of the **24 non-exact functions
is present in the rebuild and in the same relative order**:

```text
orig order == mod order for all 24: Generator::createMission, PlayerEgo::calcCollision,
Status::missionCompleted, Level::createMission, Level::createScene, Level::createShip,
Layout::Layout C1/C2, ModStation::ModStation C1/C2, Globals::Globals C1/C2,
MenuTouchWindow::MenuTouchWindow C1/C2, MenuTouchWindow::OnTouchBegin, Radar::draw,
LevelScript::process, HangarWindow::render, Gun::calcCharacterCollision,
RocketGun::update, Hud::draw, SpaceLounge::drawLounge, MGame::OnTouchEnd, MGame::OnUpdate.
```

### 3.1 String-set comparison

```sh
llvm-objcopy --dump-section .rodata=orig.rodata \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so /dev/null
llvm-objcopy --dump-section .rodata=mod.rodata /dev/shm/wm104/libs_clean/mod_main.so /dev/null
python3 - <<'PY'
import re
def strs(p):
    d=open(p,'rb').read()
    return set(m.group().decode('latin1') for m in re.finditer(rb'[\x20-\x7e]{6,}',d))
a=strs('orig.rodata'); b=strs('mod.rodata')
print(len(a),len(b),len(a&b),len(a&b)/len(a|b))
PY
```

Measured: original 3,171 strings, mod 3,085, common 3,063, **Jaccard 0.9593**. The only
game-text differences are:

| original | mod rebuild |
|---|---|
| `Galaxy on Fire 2 HD` | `Galaxy on Fire 2 FHD` |
| `Datenschutzb.` | `Datenschutzrichtlinie` |
| `Nutzungsb.` | `Nutzungsbestimmungen` |
| `Terms of S.` | `Terms of Service` |

Most of the rest are libc++ vs libstdc++ demangler/exception/printf strings
(`N12_GLOBAL__N_1...`, `typeid(`, `objc_object<`, `void abort_message(...)`, etc.). The
notable non-STL exceptions are three Android JNI/device-info strings that exist only in the
original: `getDisplayWidth`, `getDisplayHeight` and `net/fishlabs/ow_utils/DeviceInfo`
(`strings -a` finds 0 occurrences in the rebuild). So the rebuild is the **same code family
with a few source differences**, not a byte-for-byte source match; treat it as a code oracle
with a localization/DeviceInfo caveat.

## 4. The 24 non-exact functions in the rebuild

The rebuild's function sizes are `-O2` sizes, so they are not directly comparable; they are
listed to show that every target is present and that `RocketGun::update` is the one function
where the rebuild is slightly *smaller* than the original.

```text
function                                                       pct   orig    mod   ours mod/orig
_ZN6LayoutC1Ev                                                47.8   7072  10324   7072    1.460
_ZN6LayoutC2Ev                                                47.8   7072  10324   7072    1.460
_ZN11LevelScript7processEi                                    53.2  57232  59076  57232    1.032
_ZN5Radar4drawEP6PlayerP3Hudi                                 73.9   9908  11648   9908    1.176
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE   74.3   1996   2300   1996    1.152
_ZN5Level10createShipEiiiP8Waypointbb                         74.4   2764   3112   2764    1.126
_ZN5MGame10OnTouchEndEiiPv                                    74.7   8080   9784   8080    1.211
_ZN7GlobalsC1Ev                                               80.5    612    620    612    1.013
_ZN7GlobalsC2Ev                                               80.5    612    620    612    1.013
_ZN12HangarWindow6renderEv                                    83.1   9700  12016   9700    1.239
_ZN9RocketGun6updateEi                                        86.9   1908   1904   1908    0.998
_ZN6Status16missionCompletedEbbx                              87.0   1432   1508   1432    1.053
_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE              87.6    968   1112    968    1.149
_ZN5Level11createSceneEv                                      88.3   2252   2656   2252    1.179
_ZN3Gun22calcCharacterCollisionEv                             90.0   1984   2024   1984    1.020
_ZN5Level13createMissionEv                                    91.1  17172  20120  17172    1.172
_ZN3Hud4drawExxP9PlayerEgobjj                                 93.0   9332  11548   9332    1.237
_ZN15MenuTouchWindowC1Ei                                      95.1   9600  11988   9600    1.249
_ZN15MenuTouchWindowC2Ei                                      95.1   9600  11988   9600    1.249
_ZN11SpaceLounge10drawLoungeEv                                95.5   1948   2132   1948    1.094
_ZN15MenuTouchWindow12OnTouchBeginEiiPv                       97.7   1896   2120   1896    1.118
_ZN10ModStationC1Ev                                           98.4   1008   1076   1008    1.067
_ZN10ModStationC2Ev                                           98.4   1008   1076   1008    1.067
_ZN5MGame8OnUpdateEv                                          99.9  11440  13148  11440    1.149
```

(`pct`/`ours` are from `/dev/shm/wm104/build/verify/report.json`, i.e. the official verifier
on the scratch build; `orig`/`mod` are the ELF symbol sizes. The `pct` values match the
canonical wave-101 report to within the usual 0.1–1.6-point scratch-build noise; the canonical
gate is still `byte 4500`.)

## 5. iOS 1.1.19 source order for the 24 functions

The 1.1.19 dump (`source file list dump/gof2hd_1.1.19_ios.cpp`, parsed in
`_work/original_layout/original_layout.json`) lists the original TU and the symbol order
within each TU. Comparing the demangled iOS order with the Android 2.0.16 address order gives
a per-TU LIS of:

| iOS TU | matched | LIS | ratio |
|---|---:|---:|---:|
| `Classes/Game/Generator.cpp` | 11/17 | 11/11 | 1.000 |
| `Classes/Game/Globals.cpp` | 36/173 | 33/36 | 0.917 |
| `Classes/Game/Gun.cpp` | 24/25 | 22/24 | 0.917 |
| `Classes/Game/HangarWindow.cpp` | 30/32 | 25/30 | 0.833 |
| `Classes/Game/Hud.cpp` | 39/43 | 38/39 | 0.974 |
| `Classes/Game/Layout.cpp` | 43/43 | 43/43 | 1.000 |
| `Classes/Game/Level.cpp` | 94/149 | 91/94 | 0.968 |
| `Classes/Game/LevelScript.cpp` | 18/18 | 16/18 | 0.889 |
| `Classes/Game/MenuTouchWindow.cpp` | 31/32 | 28/31 | 0.903 |
| `Classes/Game/MGame.cpp` | 41/52 | 41/41 | 1.000 |
| `Classes/Game/ModStation.cpp` | 32/107 | 32/32 | 1.000 |
| `Classes/Game/PlayerEgo.cpp` | 153/158 | 151/153 | 0.987 |
| `Classes/Game/Radar.cpp` | 16/18 | 16/16 | 1.000 |
| `Classes/Game/RocketGun.cpp` | 11/14 | 11/11 | 1.000 |
| `Classes/Game/SpaceLounge.cpp` | 26/32 | 23/26 | 0.885 |
| `Classes/Game/Status.cpp` | 112/117 | 110/112 | 0.982 |

The target functions' exact iOS indices and Android addresses:

```text
Generator::createMission      ios 10  android 0x971b9
Globals::Globals              ios  0/1 android 0xe31e1
Gun::calcCharacterCollision   ios 21  android 0x152f91
HangarWindow::render          ios 12  android 0x1490d9
Hud::draw                     ios 27  android 0x163b91
Layout::Layout                ios  0/1 android 0xd1029
Level::createMission          ios 13  android 0xaffd9
Level::createScene            ios 14  android 0xb42ed
Level::createShip             ios 33  android 0xc0215
LevelScript::process          ios  5  android 0x137d19
MenuTouchWindow::MenuTouchWindow ios 0/3 android 0x12299d
MenuTouchWindow::OnTouchBegin ios 18  android 0x129bd5
MGame::OnTouchEnd(void*)      ios 19  android 0x17a145
MGame::OnUpdate               ios 27  android 0x17c8d9
MGame::OnTouchEnd(int,int)    ios 41  android 0x180ec7
ModStation::ModStation        ios  0/1 android 0xd52d1
PlayerEgo::calcCollision      ios 87  android 0x9f0cd
Radar::draw                   ios  6  android 0x12e051
RocketGun::update             ios  8  android 0x15ecd9
SpaceLounge::drawLounge       ios 16  android 0x170d39
Status::missionCompleted      ios 53  android 0xabf39
```

Within each TU, all 24 target functions are in the same relative order in the iOS dump as in
the Android binary; the *global* order is not monotonic because the iOS and Android link
orders differ (e.g. `Status::missionCompleted` is iOS 53 while `Level::createMission` is iOS
13). The per-TU LIS shortfalls appear to be caused by template instantiations and inlined
helpers (e.g. the `ArrayAdd`/`ArrayReleaseClasses`/`ArraySetLength` symbols that iOS emits in
a different place or not at all), not by the target functions. **The iOS 1.1.19 dump therefore
confirms the target functions' TU membership and within-TU relative order; it does not reveal
anything new about their bodies.**

## 6. The Unity-remake decompilation (additional artifact)

The `KiritoJPK/Galaxy-on-Fire-2-Unity-Remake` GitHub repo contains a full Ghidra
decompilation of the mod rebuild:

```text
Reference/decompiled/native/game/*.c          # one file per class, 4,028 functions
Reference/decompiled/native/_ALL_FUNCTIONS.c  # all functions in one file
Reference/decompiled/native/_FUNCTIONS.tsv    # name, address, size
Reference/decompiled/native/INDEX.md
Reference/decompiled/native/CLASS_HIERARCHY.txt
```

The repo's `Reference/README.md` says the code came from
`Reference/binaries/GOF2_2.0.16_Android_Price.apk`, i.e. exactly the mod rebuild analysed
above (addresses use Ghidra image base 0x10000; real = ghidra - 0x10000). The relevant files
for the 24 functions are:

```text
Generator.c Globals.c Gun.c HangarWindow.c Hud.c Layout.c Level.c LevelScript.c
MenuTouchWindow.c MGame.c ModStation.c PlayerEgo.c Radar.c RocketGun.c SpaceLounge.c Status.c
```

This is the only decompilation of the rebuild in the artifacts. It is **pseudocode, not
compilable**, and it inherits the rebuild's localization-version caveat, but it is a useful
second opinion for the 24 bodies. Two concrete leads:

* `Status::missionCompleted` is decompiled as a **direct `switch (m->getType())`** with the
  case bodies in the order `0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e`, then the
  empty `0x9f/0xa1/0xad..` group, then `0xa0`, `0xa2`, ... This is the source order the
  `.missioncase` rank table is trying to reproduce.
* `Globals::Globals` is decompiled with the `options._84_4_ = 0x247` store early (before the
  bulk of the zero stores), matching the original's early `movw r6,#583`; our source currently
  materializes `v54` later. Both a declaration-only hoist (neutral, §7.1) and the literal
  early-store shape (regression, §7.3) were tested; the original's early `movw` is a
  scheduler/register-allocation artifact, not proof of an early source store.

The repo's `Reference/README.md` also lists `Reference/binaries/libgof2hdaa.so` ("the original
32-bit ARM game library") and `Reference/binaries/GOF2_2.0.16_Android_Price.apk`. In the GitHub
tree the `.so` is a Git-LFS pointer and was not downloaded here, so its bytes were not
independently checked. The repo ships `Reference/research/*.md` notes, but no
DWARF/symtab/`.d`/`.o`/build logs.

## 7. Controlled A/B experiments (all reverted)

The scratch build is a full copy of the canonical recipe:

```sh
cmake -S . -B /dev/shm/wm104/build -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz
cmake --build /dev/shm/wm104/build --target check -j8
```

Baseline (reproduced before and after every experiment):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

### 7.1 `Globals::Globals` initializer hoist (neutral)

Moved `int v54 = 0x247; int v58 = 0x201;` from the bottom of the constructor to the top
(immediately after `GameSettingsRecord *settings = ...`), leaving the conditional overrides
in place. This was intended to reproduce the original's early `movw r6,#583`.

```sh
cp src/game/core/Globals.cpp /dev/shm/wm104/Globals.cpp.orig
# ... edit ...
cmake --build /dev/shm/wm104/build --target check -j8
```

Result: `byte 4500 -> 4500`, `linked 4500 -> 4500`, `Globals::Globals` C1/C2
`80.5 -> 80.5 %`, size `612 -> 612`. **Not a strict improvement; reverted** with
`cp /dev/shm/wm104/Globals.cpp.orig src/game/core/Globals.cpp`.

### 7.2 `Status::missionCompleted` rank-table removal (regression)

Removed the `#pragma clang section rodata=".missioncase"` / `kMissionCase[192]` table and
rewrote `switch (kMissionCase[m->getType()])` as a direct `switch (m->getType())`, replacing
each dense case label with the original type labels that map to it (inverted from the table;
the `case N: {` form was handled explicitly). This is the decompiled-C source shape.

```sh
cp src/game/mission/Status.cpp /dev/shm/wm104/Status.cpp.orig
# ... edit ...
cmake --build /dev/shm/wm104/build --target check -j8
```

Result:

```text
verify: byte 3043  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET FAIL: byte_exact 4500 -> 3043
```

`linked_exact` stayed 4500 (code addresses are fine), but `.rodata` layout shifted and
byte_exact collapsed. The 192-byte `.missioncase` table is **load-bearing for `.rodata`
layout**, not just a codegen device. **Not an improvement; reverted** with
`cp /dev/shm/wm104/Status.cpp.orig src/game/mission/Status.cpp`. The canonical gate was
re-measured after the revert: `byte 4500`, `linked 4500`, `RATCHET PASS`.

### 7.3 `Globals::Globals` early direct `resWidth` store (regression)

The Unity decompilation's `Globals::Globals` shows `options._84_4_ = 0x247;` early, before the
bulk of the zero stores, and the conditional overrides later. I tested that exact source
shape: `settings->resWidth = 0x247;` immediately after the `settings` local, with
`if (...) settings->resWidth = ...;` and the `resHeight` block left at the bottom.

```sh
cp src/game/core/Globals.cpp /dev/shm/wm104/Globals.cpp.wm104base
# ... move the resWidth default store to the top; make the overrides direct stores ...
cmake --build /dev/shm/wm104/build --target check -j8
```

Result:

```text
verify: byte 2547  linked 4500  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET FAIL: byte_exact 4500 -> 2547
```

`Globals::Globals` C1/C2 went `80.5 -> 29.8 %`, size `612 -> 628`; `linked_exact` stayed
4500. The early store changes the TU's literal-pool/`.rodata` layout, which drops ~1,950
other functions' byte matches. **Not an improvement; reverted** with
`cp /dev/shm/wm104/Globals.cpp.wm104base src/game/core/Globals.cpp`. A declaration-only early
move (7.1) is neutral; a real early store (7.3) is a large regression. The original's early
`movw r6,#583` is therefore a scheduler/register-allocation artifact, not evidence for an
early source store.

## 8. Other inventory items

* `/home/debian/GalaxyOnFire2/_bg/libgof2hdaa.so` — **our own older build**, not an original:
  `sha256 c372f2403be494513a5b9fe3c45ac4205621c2bc5959bce37d8cfbafc47d6d31`, 2,163,472 B,
  build id `20316cb9...`, clang 7.0.2 / gold 1.12, `.text 0x12ff10`. It predates the
  `.tmp/w696` build (`.text 0x1333c0`, build id `b66af4b5...`) and is not its base.
* `/home/debian/GalaxyOnFire2/.tmp/w696/{base,head1,candidate1,delta192}.so` — **our own
  builds** (same build id `b66af4b5...` for base/head1; candidate1/delta192 are experiments).
  None is an original.
* `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` — the ground-truth 2.0.16 APK
  (already known). Its `lib/armeabi-v7a/libgof2hdaa.so` is the same ELF as the ground truth
  except `.shstrtab`/`sh_name` (the wave-100 correction).
* `Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa` — iOS 1.1.4, no 1.1.19 binary. The
  1.1.19 evidence is the source-layout dump only.
* `Original Windows/Steam` — the Windows build; no `.pdb`, `.lib`, `.exp`, `.map`, `.sln` or
  `.vcxproj`.
* `source file list dump/` — the 1.1.19 dump is the only 1.1.19 artifact; see §5.
* `DeepOpen/extras/gof2-1.0.1-ios-symbols/` — a demangled symbol dump ripped from the iOS
  1.0.1 binary (`ripped_symbols.hpp`, 2,868 lines). All 24 target classes/functions are
  present, but the signatures are older (`Status::missionCompleted(bool, long long)` vs the
  Android 3-arg form; `Level::createShip(int,int,int,Waypoint*,bool)` vs the Android 6-arg
  form) and the file is a flat string-table order, not a per-TU source order. It corroborates
  the functions' existence in the iOS lineage, not the 2.0.16 order.
* OBB: `main.47947006...obb` is a zip with 2,460 game-data entries (`.aem`, `.aei`, etc.),
  4,418,825,533 bytes uncompressed. `unzip -l` and `strings` show no `.so`/`.o`/`.a`/`.d`/
  build log/source path.

## 9. Proven vs hypothesis

**Proven (commands above):**

1. The 7z APK set contains four distinct libs: one stock 2.0.16 copy and one base source
   rebuild in three post-link variants.
2. The stock copy is the original except for a 4-byte `RecordHandler::checkHash` patch at
   VMA `0xcd7b0` (`f0 b5 03 af` -> `01 20 70 47`); the other 288 differing bytes are the
   raw-bfd `.shstrtab`/`sh_name` order that the APKPure APK lib also has.
3. The rebuild is NDK r14 / clang 3.8.275480 / gold 1.11, `-O2`, system libstdc++, no
   `.note.android.ident`, 26 sections, `.init_array` 0x10.
4. The rebuild is a full source rebuild: 4,245 common functions, 99.32 % order LIS, all 24
   non-exact functions present and in the same relative order.
5. The rebuild's `.rodata` is 95.9 % Jaccard-identical; the only game-text differences are
   the four localization strings listed in §3.1, and the only non-STL code strings missing from
   the rebuild are the three `DeviceInfo` JNI strings.
6. The iOS 1.1.19 dump confirms the 24 target functions' TU membership and within-TU
   relative order; the iOS 1.0.1 symbol dump corroborates their existence (with older
   signatures).
7. No shipped artifact contains DWARF, `.symtab`, `.d`, `.o`, `.a`, build logs or a project
   file.
8. The `.missioncase` rank table is load-bearing for `.rodata` layout: removing it drops
   byte_exact 4500 -> 3043 while linked_exact stays 4500.
9. Hoisting `Globals::Globals`'s `v54/v58` initializers is codegen-neutral.
10. Making the `Globals::Globals` `resWidth` default an early direct store (the Unity
    decompilation's shape) regresses byte_exact 4500 -> 2547, `Globals::Globals`
    80.5 -> 29.8 %, size 612 -> 628, while linked_exact stays 4500.

**Hypothesis / lead (not proven):**

1. The rebuild's source revision is the same game code with a **different localization
   table** (full German/English strings instead of the Android abbreviations) and a few
   source-level differences (the `DeviceInfo` JNI strings are gone). The code evidence (order,
   strings, call graph) is consistent with this, but the rebuild's exact source commit is not
   in the artifacts.
2. The Unity repo's decompiled `Status::missionCompleted` direct-switch case order is the
   original source order. It is the right lead for replacing the `.missioncase` table, but a
   naive removal regresses `.rodata`; a compensating `.rodata` layout change is required.
3. The original's early `movw r6,#583` in `Globals::Globals` is a scheduler/register-allocation
   artifact, not evidence of an early source store: a declaration-only hoist is neutral (7.1)
   and a real early store is a large regression (7.3). Reproducing it would need a source shape
   that keeps the constant live without moving the store.
4. The rebuild's `-O2` code is a useful cross-compiler oracle for the 24 bodies (same source,
   different optimizer), but it is not a byte/size oracle and its localization strings must
   not be copied into the Android tree.

## 10. Reproduce

```sh
# 1. extract the archives
7z x -y -o/dev/shm/wm104/apk7z \
  "/home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_APK.7z"
7z x -y -o/dev/shm/wm104/obb7z \
  "/home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_OBB.7z"

# 2. exact libs
mkdir -p /dev/shm/wm104/libs_clean
unzip -p "/dev/shm/wm104/apk7z/Save fix/GOF2_2.0.16_For_Save_Fix.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/wm104/libs_clean/stock_savefix.so
unzip -p "/dev/shm/wm104/apk7z/GOF2_2.0.16_Android_Price.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/wm104/libs_clean/mod_main.so
sha256sum /dev/shm/wm104/libs_clean/*.so

# 3. stock-vs-original: 4 code bytes + 288 raw-bfd .shstrtab/sh_name bytes
cmp -l /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
       /dev/shm/wm104/libs_clean/stock_savefix.so

# 4. rebuild metadata
readelf -nW /dev/shm/wm104/libs_clean/mod_main.so
readelf -p .comment /dev/shm/wm104/libs_clean/mod_main.so
readelf -dW /dev/shm/wm104/libs_clean/mod_main.so | grep NEEDED
readelf -AW /dev/shm/wm104/libs_clean/mod_main.so

# 5. scratch build + official gate
cmake -S . -B /dev/shm/wm104/build -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz
cmake --build /dev/shm/wm104/build --target check -j8

# 6. iOS 1.1.19 order / Unity decompilation
python3 - <<'PY'
import json
j=json.load(open('_work/original_layout/original_layout.json'))
print(j['tu_symbols']['gof2hd_1.1.19_ios']['Classes/Game/Status.cpp']['symbols'])
PY
# GitHub: https://github.com/KiritoJPK/Galaxy-on-Fire-2-Unity-Remake
#   Reference/decompiled/native/game/*.c
```
