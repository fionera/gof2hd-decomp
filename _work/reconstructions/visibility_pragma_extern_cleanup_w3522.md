# Visibility pragmas / extern cleanup (w3522)

Status: research note + landed gate-neutral source cleanup. Worktree `wt/w3522`,
branch `wave87-w3522`, based on master `16e5c9a4`. All numbers are from this
worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

## TL;DR

There are exactly **three `#pragma GCC visibility` blocks** and **eight ad-hoc
`extern` declarations in `.cpp` files**. All eight `extern`s can be removed or
moved to headers, and all three pragma blocks can be removed, with **no gate
change**:

```
before: byte 4495  linked 4496
after:  byte 4495  linked 4496
```

The hidden visibility itself is **load-bearing** and cannot simply be dropped:

* removing the hidden declaration on the `s_processPad` extern makes LevelScript's
  accesses GOT-indirect (`R_ARM_GOT_PREL` instead of `R_ARM_REL32`) and collapses
  `byte_exact` 4495 -> 1566;
* making the VirtualInput delayed-key globals `static` without reordering the
  functions changes clang's `.bss` first-use order and loses 5 bytes
  (`byte_exact` 4495 -> 4490).

The landed cleanup therefore keeps hidden visibility where codegen needs it, but
spells it with same-line `[[gnu::visibility("hidden")]]` attributes and moves the
shared declaration into a header. VirtualInput becomes fully `static` by moving
`SendStoredKeyUpEvents` before `keyPressed`, which reproduces the original `.bss`
order without any visibility attribute.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8
```

```
verify: byte 4495  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
original: 5370  ours: 5370  extra: 0  missing: 0 (wrong_type: 0  absent: 0)
```

Section sizes/bases at baseline (all exact except `.ARM.extab`, which is the
pre-existing -8 residue):

```
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x6f70   @ 0x19cc08
.ARM.extab   0x26290  @ 0x1a3b78   (original 0x26298)
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

## 2. Inventory

```sh
grep -rn "visibility" src | grep -v AEGeometry
grep -rn "^extern \|^    extern \|^  extern " src --include=*.cpp | grep -v 'extern "C"'
```

Three visibility pragma blocks:

| file | construct |
|---|---|
| `src/game/ship/TargetFollowCamera.cpp` | `#pragma GCC visibility push(hidden)` around `volatile int s_processPad[4];` |
| `src/game/world/LevelScript.cpp` | `#pragma GCC visibility push(hidden)` around `extern volatile int s_processPad[4];` |
| `src/engine/input/VirtualInput.cpp` | `#pragma GCC visibility push(hidden)` around `g_delayedKeyCount` / `g_delayedKeys` |

One redundant hidden attribute:

```c++
// src/game/ui/StatusWindow.cpp
[[gnu::visibility("hidden")]] static inline int statusWindowMedalRowsHeight(...)
```

Eight ad-hoc `extern` declarations in `.cpp` files:

| file | declaration |
|---|---|
| `src/game/ship/TargetFollowCamera.h` / `LevelScript.cpp` | `extern volatile int s_processPad[4];` |
| `src/game/menu/ModStation.cpp` | `extern int g_android_current_achievements[3];` |
| `src/game/ui/MenuTouchWindow.cpp` | `g_android_link_game_gp`, `g_android_gp_is_linked`, `g_android_show_achievements`, `g_android_show_leaderboards` |
| `src/game/core/Globals.cpp` | `extern int g_android_leaderboard_scores[8];` |
| `src/engine/core/NFC.cpp` | `extern const char *deviceInfoPath;` |

## 3. Original-binary evidence

### 3.1 Original visibility policy

Every named `.dynsym` entry in the original is `STV_DEFAULT`:

```sh
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -sW ../_work/bins/android_2.0.16_libgof2hdaa.so |
  awk 'NR>3 && $1 ~ /^[0-9]+:/ && $8 != "" {print $6}' | sort | uniq -c
```

```
   5577 DEFAULT
```

So the original did not use `-fvisibility=hidden`; the hidden pragmas in this
tree are only a way to keep local layout anchors out of `.dynsym`.

### 3.2 The 16-byte `s_processPad` chunk is unreferenced in the original

Original `.dynsym` around the chunk:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D -S --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so |
  sort | awk '$1>="00218380" && $1<="002183c0"'
```

```
00218380 00000001 B _ZN7Globals19iap_hack_dlc1BoughtE
00218381 00000001 B _ZN7Globals19iap_hack_dlc2BoughtE
00218382 00000001 B _ZN7Globals19iap_hack_dlc3BoughtE
00218383 00000001 B _ZN5Radar10drawTargetE
00218384 00000004 B _ZN10StarSystem16orbitPlanetIndexE
002183b0 00000004 B _ZN7Mission5emptyE
```

There is a 0x28-byte hole `0x218388..0x2183b0`. Our unstripped link fills it with
`CHAR_BRENT[20]`, `g_sw_charDef`, and the 16-byte `s_processPad`:

```sh
/tmp/relink_map.sh   # exact gold command from build.ninja + -Map, no strip
$NM -n -S /tmp/ours_unstripped.so | awk '$1>="00218380" && $1<="002183b8"'
```

```
00218388 00000014 b _ZL10CHAR_BRENT
0021839c 00000004 b _ZL12g_sw_charDef
002183a0 00000010 b s_processPad
002183b0 00000004 B _ZN7Mission5emptyE
```

A scan of the original's entire `.text` for PIC literal-pool references to
`0x2183a0..0x2183b0` finds **zero**, and no 4-byte pointer to `0x2183a0` exists
anywhere in the file:

```python
# /tmp/scan_all.py (capstone; skipdata=True)
import capstone, struct
TEXT_BASE=0x69440
data=open('/tmp/orig_text.bin','rb').read()   # objcopy -O binary --only-section=.text
md=capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB); md.skipdata=True
insns=list(md.disasm(data,TEXT_BASE))
hits=0
for idx,i in enumerate(insns):
    if i.mnemonic=='add' and 'pc' in i.op_str:
        for j in range(max(0,idx-5),idx):
            p=insns[j]
            if p.mnemonic.startswith('ldr') and '[pc' in p.op_str:
                imm=int(p.op_str.split('#')[1].split(']')[0],0)
                pool=p.address+4+imm
                w=struct.unpack_from('<I',data,pool-TEXT_BASE)[0]
                tgt=(w+i.address+4)&0xffffffff
                if 0x2183a0<=tgt<0x2183b0: hits+=1
                break
print(hits)   # 0
```

```sh
python3 - <<'PY'
import struct
data=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
print(data.count(struct.pack('<I',0x2183a0)))   # 0
PY
```

So `s_processPad` is a **layout anchor for an unreferenced 16-byte original
`.bss` chunk**, not a reconstruction of a real referenced global. Its address
(and therefore the TU it lives in) is load-bearing, but its name/value are not.

### 3.3 Why the hidden declaration is load-bearing

With the hidden declaration, LevelScript's stores to `s_processPad` are direct
PC-relative accesses:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -dr cmake-build-match/CMakeFiles/gof2.dir/src/game/world/LevelScript.cpp.obj |
  grep -A1 s_processPad
```

```
404: R_ARM_REL32  s_processPad
c2f4: R_ARM_REL32 s_processPad
```

Removing the pragma (plain `extern`) makes it default-visible and GOT-indirect:

```
404: R_ARM_GOT_PREL  s_processPad
c2f8: R_ARM_GOT_PREL s_processPad
```

That inserts a new GOT slot and shifts the PLT/GOT layout, which is why the
full gate collapses (see E2 below).

## 4. Controlled A/B experiments

Each row is a full `cmake --build cmake-build-match --target check -j8` after
editing only the listed source. No `--update-baseline` was used.

| # | source change | byte | linked | verdict |
|---|---|---:|---:|---|
| E0 | baseline | **4495** | **4496** | — |
| E1 | drop redundant `[[gnu::visibility("hidden")]]` from `static inline statusWindowMedalRowsHeight` | 4495 | 4496 | neutral |
| E2 | `LevelScript.cpp`: plain `extern volatile int s_processPad[4];` (no pragma) | **1566** | 4496 | catastrophic: GOT-indirect access |
| E3 | `LevelScript.cpp`: `[[gnu::visibility("hidden")]] extern ...` instead of pragma | 4495 | 4496 | neutral |
| E4 | `TargetFollowCamera.cpp`: `[[gnu::visibility("hidden")]] volatile int s_processPad[4];` instead of pragma | 4495 | 4496 | neutral |
| E5 | `VirtualInput.cpp`: `[[gnu::visibility("hidden")]]` on both delayed-key globals instead of pragma | 4495 | 4496 | neutral |
| E6 | `VirtualInput.cpp`: `static` delayed-key globals, no function reorder | **4490** | 4496 | -5: `.bss` first-use order changes |
| E7 | E6 + move `SendStoredKeyUpEvents` before `keyPressed` | 4495 | 4496 | neutral; exact original `.bss` order |
| E8 | `LevelScript.cpp`: `static volatile int s_processPad[4];` and remove the TargetFollowCamera definition | **4494** | 4496 | -1: 16-byte chunk moves out of 0x2183a0 |
| E9 | `TargetFollowCamera.cpp`: `[[gnu::used]] static` filler + LevelScript local stack `volatile int s_processPad[4]` | **3685** | 4496 | -810: `LevelScript::process` shrinks 0xdf90 -> 0xdf80 |
| E10 | `NFC.cpp`: move `deviceInfoPath` definition before first use, drop redundant `extern` | 4495 | 4496 | neutral |
| E11 | move the six `g_android_*` externs into `src/platform/android/GOF2HD2012.h` | 4495 | 4496 | neutral |
| E12 | **all of E1+E3+E4+E7+E10+E11 combined** | **4495** | **4496** | **landed** |

Notes on the mechanism:

* E6/E7: clang 7.0.2 emits internal-linkage globals in first-use order, not
  declaration order. With `SendStoredKeyUpEvents` after `keyPressed`, the speed
  flags are first-used first and the delayed-key pair lands after them. Moving
  `SendStoredKeyUpEvents` before `keyPressed` makes the delayed-key pair first,
  reproducing the original order `g_delayedKeyCount, g_delayedKeys, g_speedUp,
  g_slowMotion, stackX, ...`.
* E8: the original's 16-byte hole is at `0x2183a0`; a file-static in
  `LevelScript.cpp` lands in a different TU's `.bss` slot and loses one
  byte-exact function.
* E9: the stack array changes `LevelScript::process`'s frame and shrinks the
  body by 0x10, shifting every later `.text` function.

## 5. Landed change

The landed patch removes all three pragma blocks and all eight ad-hoc `.cpp`
`extern` declarations:

* `TargetFollowCamera.cpp` / `.h`: the 16-byte anchor is declared once in the
  header with `[[gnu::visibility("hidden")]]`; `LevelScript.cpp` no longer has
  its own `extern`.
* `VirtualInput.cpp`: delayed-key globals are `static`; `SendStoredKeyUpEvents`
  is moved before `keyPressed` to preserve the original `.bss` order.
* `StatusWindow.cpp`: redundant hidden attribute removed from the `static inline`
  helper.
* `NFC.cpp`: `deviceInfoPath` is defined once, before first use.
* `src/platform/android/GOF2HD2012.h` (new): declares the six `g_android_*`
  counters with C linkage; `ModStation.cpp`, `MenuTouchWindow.cpp` and
  `Globals.cpp` include it instead of declaring them locally.

Final gate:

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8
```

```
verify: byte 4495  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
original: 5370  ours: 5370  extra: 0  missing: 0 (wrong_type: 0  absent: 0)
```

All section sizes/bases are unchanged from the baseline table. The final
`.bss` order for VirtualInput is exactly the original's:

```
0x216974 g_delayedKeyCount
0x216978 g_delayedKeys
0x217978 g_speedUp
0x21797c g_slowMotion
0x217980 stackX
0x217984 stackY
0x217988 wheelStack
0x21798c g_simEdgesInit
0x217990 g_simTouchActive
0x217994 g_input
```

## 6. Proven vs still hypothesis

**Proven by measurement**

* The tree had exactly three `#pragma GCC visibility` blocks and eight ad-hoc
  `.cpp` `extern` declarations; all can be removed or moved to headers with no
  gate change (E12: 4495/4496 before and after).
* The original's named `.dynsym` entries are all `STV_DEFAULT` (5577/5577).
* The original has no code or pointer reference to `0x2183a0..0x2183b0`; the
  16-byte `s_processPad` chunk is an unreferenced layout anchor.
* Removing hidden visibility from the `s_processPad` extern changes the object
  relocation from `R_ARM_REL32` to `R_ARM_GOT_PREL` and collapses `byte_exact`
  to 1566.
* Making the VirtualInput delayed-key globals `static` without reordering loses
  5 bytes; moving `SendStoredKeyUpEvents` before `keyPressed` restores the
  original `.bss` order and the gate.
* Making `s_processPad` a `static` in `LevelScript.cpp` loses 1 byte; a stack
  array loses 810 bytes because `LevelScript::process` shrinks by 0x10.

**Still hypothesis / not claimed**

* The original source spelling of the 16-byte `0x2183a0` object cannot be
  recovered from the stripped binary; it is unreferenced, so any 16-byte
  unreferenced object in TargetFollowCamera's TU is observationally equivalent.
* The original may have used `static` plus a different function order rather
  than hidden external linkage for the VirtualInput delayed-key globals; E7 is
  the smallest source-only shape that reproduces every observed `.bss` address.
* No strict `byte_exact` improvement was found in this pass; the cleanup is
  gate-neutral by construction.
