# Data content round 2 (w3451): `ModStation` `.data` order and the `.data` / `.data.rel.ro` map

Status: research note + one landed source change. Branch `wave87-w3451`, based on
master `cfbd140e` (`docs: campaign notes for dynsym/data wave`). Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2,
`-Oz`, image base 0x10000).

## TL;DR

`clang 7.0.2 -Oz` emits namespace-scope `.data` globals in **first-use order**, and
a namespace-scope `static int *const T[] = {A, B}` initializer does **not** count
as a use of `A`/`B` for that ordering. `ModStation.cpp`'s `gStoryFaces` was only
referenced by the `gStoryFaceTable` initializer, so `gPirateFace` (used in
functions) was emitted first. Adding a function-level `(void) gStoryFaces;` to
`ModStation::ModStation()` forces the original order:

```
0x2151e0  gStoryFaces  (1200 B)   <- original and now ours
0x215690  gPirateFace  (40 B)     <- original and now ours
```

Measured with the full gate:

| | byte_exact | linked_exact | `.rodata` diff | `.data.rel.ro` diff | `.data` diff |
|---|---:|---:|---:|---:|---:|
| baseline (HEAD `cfbd140e`) | 4254 | 4496 | 13266 B | 744 B | 260 B |
| **after `(void) gStoryFaces;`** | **4255** | **4496** | 13266 B | **670 B** | **3 B** |

Every ELF section stays size- and base-exact. The only verify row that changes is
`_ZN10ModStation10checkHintsEv` (`byte False -> True`): its literal pool contains
a PC-relative reference to `gPirateFace`, so the corrected address flips it.
`lint` is `CLEAN ✓` and `drift` is `0 field(s) across 0 class(es)`.

The remaining `.data` difference is 3 bytes of libc++abi handler pointers; the
remaining `.data.rel.ro` bytes are 646 B of libc++abi `.text` pointers, 18 B of
the `Level::createScene` switch table (downstream of the `.rodata` Level block),
and 6 B of `.bss` pointers. The dominant open blocker is still `.rodata`
(13266 B, first at `0x1fc390`, the `Status.cpp` `kMissionCase` table).

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

Baseline at HEAD `cfbd140e` (the change stashed with
`git stash push -- src/game/menu/ModStation.cpp`):

```
verify: byte 4254  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes/bases (both binaries; unchanged by this pass):

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
.ARM.exidx   0x6f70   @ 0x19cc08
.ARM.extab   0x26298  @ 0x1a3b78
.dynsym      0x15ca0  @ 0x1f0
.dynstr      0x2e318  @ 0x15e90
.dynamic     0x148    @ 0x20fba4
```

## 2. Measurement method

The gate's `data: exact 843 of 843` is relocation-normalized, not a raw byte
check, so the raw section bytes were compared directly. The scratch relink adds
`-Map` and drops the final `strip`:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/w3451/link_cmd.txt
# edit: add -Map=/tmp/w3451/data.map, replace -o with /tmp/w3451/unstripped.so,
# drop the trailing '&& ... arm-linux-androideabi-strip --strip-all'
```

The resulting script used here is `/tmp/w3451/relink_map.sh`; its gold command is
the build's own command with only those two edits. Raw section diff:

```sh
python3 - <<'PY'
import re, subprocess
orig='../_work/bins/android_2.0.16_libgof2hdaa.so'
ours='cmake-build-match/libgof2hdaa.so'
def secs(p):
    out=subprocess.check_output(['readelf','-S','-W',p],text=True)
    d={}
    for line in out.splitlines():
        m=re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+([0-9a-fA-F]+)',line)
        if m: d[m.group(1)]=(int(m.group(3),16),int(m.group(4),16),int(m.group(5),16))
    return d
O=secs(orig); U=secs(ours)
for name in ['.rodata','.data.rel.ro','.data']:
    a=O[name]; b=U[name]
    with open(orig,'rb') as f: f.seek(a[1]); od=f.read(a[2])
    with open(ours,'rb') as f: f.seek(b[1]); ud=f.read(b[2])
    dif=[i for i in range(len(od)) if od[i]!=ud[i]]
    print(name, len(dif), hex(a[0]+dif[0]) if dif else '-')
PY
```

`.rodata` ownership/placement was attributed with the committed scripts:

```sh
python3 _work/reconstructions/rodata_content_diff_map.py \
  --unstripped /tmp/w3451/unstripped.so --map /tmp/w3451/data.map
python3 _work/reconstructions/rodata_content_map_round2.py \
  --map /tmp/w3451/data.map \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so
```

## 3. Landed fix

`src/game/menu/ModStation.cpp`, first line of `ModStation::ModStation()`:

```cpp
ModStation::ModStation() : idleBox(nullptr), field_0x108(0), field_0x10c(0) {
    (void) gStoryFaces; // keep the story-face array before gPirateFace in .data
    this->dt = 0;
    ...
```

This is a real function-level use of `gStoryFaces`; it emits no instruction and
does not change `.text` size (`0x1337c8` before and after). It only changes the
order in which clang emits the two `.data` globals.

### Why the order was wrong

`gStoryFaces` is a `static int[60][5]`; `gPirateFace` is a `static int[10]`.
Before the fix the only reference to `gStoryFaces` was inside the
`gStoryFaceTable` initializer:

```cpp
static int *const gStoryFaceTable[63] = {
    gPirateGreetingFaceB, gStoryFaces[0], gStoryFaces[1], ...
```

clang does not treat a namespace-scope initializer as a use for `.data` emission
order, while `gPirateFace` is used directly in `ModStation::update` /
`checkHints` / `OnUpdate`, so `gPirateFace` was emitted first:

```
baseline: 0x2151e0 gPirateFace (40 B)   0x215208 gStoryFaces (1200 B)
original: 0x2151e0 gStoryFaces (1200 B) 0x215690 gPirateFace (40 B)
```

The original addresses are proven by content: the original `.data` bytes at
`0x2151e0..0x2156b8` (1200 B) and `0x215690..0x2156b8` (40 B) are byte-identical
to our `gStoryFaces` and `gPirateFace` after the fix (0 differing bytes in both
ranges), and `ModStation::checkHints`'s literal pool (see §4) resolves to
`gPirateFace = 0x215690` in the original.

### Scratch clang confirmation

Three minimal TUs compiled with the exact toolchain flags
(`-target armv7-none-linux-androideabi21 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g`),
then `nm -n`:

| TU | source shape | `.data` order |
|---|---|---|
| `order.cpp` | `A`, `B`, `T[]={A,B}`; only `f(){return B[0];}` | `B`, `T`, `A` |
| `order2.cpp` | same + `g(){return A[0];}` | `A`, `T`, `B` |
| `order3.cpp` | same + `g(){return A[0];}`, `T` after `g` | `A`, `T`, `B` |

So a function-level use of `A` is what moves it ahead of `B`; the table
initializer alone does not.

## 4. The `checkHints` flip (why +1 byte_exact)

`ModStation::checkHints` has a literal pool at `0xdb808` (11th word of the
function's pool). The object relocation is:

```
00000ab4  0019a903 R_ARM_REL32  000004b0  _ZL11gPirateFace
```

The word content is `0x290`, and the code is:

```
db570: 4ba5   ldr r3, [pc, #660]   ; loads 0xdb808
db574: 447b   add r3, pc            ; pc = 0xdb578
```

so the target is `gPirateFace + 0x290 - (0xdb808 - 0xdb578)` =
`gPirateFace + 0x290 - 0x290` = `gPirateFace`. The stored REL32 value is
`gPirateFace - 0x290`:

| state | `gPirateFace` | stored word | verify |
|---|---:|---:|---|
| original | 0x215690 | `0x0013a118` | — |
| baseline | 0x2151e0 | `0x00139c68` | `checkHints` byte False |
| after fix | 0x215690 | `0x0013a118` | `checkHints` byte True |

This is the only changed row in the verify report (`byte False -> True`,
`pct 100.0`); no other function's bytes change.

## 5. Measured data map after the fix

### 5.1 `.data` — 3 differing bytes (was 260)

| run | bytes | word | original | ours | delta |
|---|---:|---|---:|---:|---:|
| `0x216960` | 1 | `__cxa_terminate_handler` | `0x00193f25` | `0x00193f01` | `-0x24` |
| `0x216964` | 2 | `__cxa_unexpected_handler` | `0x00194015` | `0x00193ff1` | `-0x24` |

Both are libc++abi `.text` pointers; the vendored libc++abi layout is 0x24 bytes
shorter than the original's. This is not app data.

### 5.2 `.data.rel.ro` — 670 differing bytes (was 744)

611 differing runs, classified by the word delta:

| runs | bytes | class |
|---:|---:|---|
| 536 | 581 | libc++abi `.text` pointers, delta `-0x24` |
| 63 | 65 | libc++abi `.text` pointers, delta `-0x20` |
| 9 | 18 | `Level::createScene` switch table, delta `+0x3cc` |
| 2 | 4 | `.bss` pointers (`+0x10a4`, `+0x1094`) |
| 1 | 2 | `gStoryFaceTable[0]` -> `gPirateGreetingFaceB`, delta `+0x1090` |

The `gStoryFaceTable` region (`0x20da34..0x20db30`, 63 pointers) dropped from
**76 differing bytes / 63 differing entries** to **2 differing bytes / 1
differing entry** (the leading `.bss` pointer). That is the 74-byte
`.data.rel.ro` improvement.

The 9 `Level::createScene` table entries at `0x20d9c0..0x20d9e1` point into
`Level.cpp` `.rodata`; the original targets are `0x1fd740/0x1fd758/0x1fd7c4/
0x1fd7e8` (`kShipHangarHeights`, `kHangarMeshes`, ...), ours are `+0x3cc`
(`0x1fdb0c/...`). This is the same `Level.cpp` `.rodata` block that owns the
4684-byte `.rodata` divergence in §5.3.

The two `.bss` pointers are `CHAR_BRENT` (`0x21942c` vs `0x218388`) and
`g_dw_playerFace` (`0x219454` vs `0x2183c0`); `gPirateGreetingFaceB` is
`0x218c88` vs `0x217bf8`. `.bss` is size-exact but its internal order is not;
this is a link-order/TU-order issue, not data content (see
`_work/reconstructions/bss_audit.md` and `data_content_map_w3443.md`).

### 5.3 `.rodata` — 13266 differing bytes (unchanged)

```
differing bytes 13266 / 269159 (4.93%)
runs 5150  first VMA 0x1fc390  last VMA 0x20377f
word multiset: common 7364  only-orig 56  only-ours 56
NOT-EXACT _ZL12kMissionCase  ours 0x1fc390 size 190
```

Top owners (from `rodata_content_map_round2.py`):

| bytes | size | owner | ours | first original occurrence |
|---:|---:|---|---:|---:|
| 4684 | 6132 | `o102_Level.cpp.obj` | `0x1fc490` | NONE |
| 4519 | 14288 | `o115_Globals.cpp.obj` | `0x1fe310` | `0x1fe2a0` |
| 808 | 1272 | `o137_StarSystem.cpp.obj` | `0x202270` | `0x202260` |
| 454 | 1532 | `o149_Gun.cpp.obj` | `0x202b14` | `0x202b04` |
| 343 | 800 | `o122_ImageFactory.cpp.obj` | `0x201af4` | NONE |
| 302 | 413 | `o108_ModStation.cpp.obj` | `0x1fdfd0` | `0x1fdf40` |
| 276 | 804 | `o136_ListItemWindow.cpp.obj` | `0x201f44` | `0x201f34` |
| 268 | 540 | `o155_Achievements.cpp.obj` | `0x203144` | `0x203134` |
| 247 | 556 | `o103_StarMap.cpp.obj` | `0x1fdda4` | NONE |
| 224 | 340 | `o101_Status.cpp.obj` | `0x1fc330` | NONE |

The first divergence is still the 190-byte `static const unsigned char
kMissionCase[190]` in `src/game/mission/Status.cpp`. The original
`Status::missionCompleted` uses in-text `tbh` tables (40-entry types 150..189 at
`0xabfda`, 7-entry types 8..14 at `0xac046`) and has no verbatim copy of that
table anywhere in `.rodata`. Removing it alone shrinks `.rodata` by `0xc0` and
drops byte_exact to 2942 (`rodata_content_map_round2.md` §5.1); the 0xc0 bytes
of missing Level/duplicate-table content must be recovered in the same change.

## 6. Negative experiment (reverted)

Replacing the four `gPirateFace` uses in `ModStation.cpp` with
`gStoryFaces[59] + 5` (same address) removes the standalone `gPirateFace`
symbol. It regresses:

```
verify: byte 2459  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
```

clang emits `add.w r3, r0, #0x4b0` instead of a direct literal load, growing
`.text` by 0x20. `gPirateFace` must stay a standalone `int[10]` symbol; the
`(void) gStoryFaces;` use is the only way found to force the order without
changing codegen.

## 7. Proven vs hypothesis

**Proven (commands in §1–§6):**

1. Baseline byte_exact 4254 / linked 4496; after the fix 4255 / 4496. `.text`,
   `.rodata`, `.data.rel.ro`, `.got`, `.data`, `.bss`, `.ARM.exidx`,
   `.ARM.extab`, `.dynsym`, `.dynstr`, `.dynamic` are all size- and base-exact
   before and after.
2. The original `.data` has `gStoryFaces` at `0x2151e0` and `gPirateFace` at
   `0x215690`; the 1200 B and 40 B ranges are byte-identical to our post-fix
   content.
3. `.data` differing bytes 260 -> 3; `.data.rel.ro` 744 -> 670; `.rodata`
   unchanged at 13266. The `gStoryFaceTable` region drops from 76 to 2
   differing bytes.
4. The only verify row that changes is `ModStation::checkHints` (byte False ->
   True), because its literal pool is a PC-relative `gPirateFace` reference.
5. The `gStoryFaces[59] + 5` substitution regresses to byte 2459 / linked 4493
   with `.text +0x20`.
6. A function-level `(void) A;` forces `.data` emission order; a namespace-scope
   `static int *const T[] = {A,B}` initializer does not (scratch TUs in §3).

**Hypothesis (consistent with the evidence, not yet proven):**

1. The remaining `.data` 3 bytes and `.data.rel.ro` 646 bytes are entirely
   vendored libc++abi `.text` layout (`-0x20`/`-0x24`), not app data. Fixing
   them requires matching the original libc++abi archive/layout, not source.
2. The `.data.rel.ro` 18-byte `Level::createScene` switch-table delta and the
   6-byte `.bss` pointer deltas are downstream of the `.rodata`/`.bss` order;
   they should fall out of the `.rodata` Level block and `.bss` TU-order fixes.
3. The top `.rodata` lever is still `kMissionCase`: removing it and recovering
   the original 0xc0 bytes of Level/duplicate-table content in one pass should
   move `Level.cpp` from `0x1fc490` to `0x1fc3d0` and align the `Level` /
   `Globals` blocks. `Globals.cpp` is currently a pure `-0x70` shift.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3451

cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# baseline (4254): stash the one-line change, rebuild, check, restore
git stash push -- src/game/menu/ModStation.cpp
cmake --build cmake-build-match --target check -j8
git stash pop
cmake --build cmake-build-match --target check -j8   # 4255

# scratch -Map relink (see §2)
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/w3451/link_cmd.txt
# edit the command: -Map=/tmp/w3451/data.map -o /tmp/w3451/unstripped.so, drop strip
bash /tmp/w3451/relink_map.sh

python3 _work/reconstructions/rodata_content_diff_map.py \
  --unstripped /tmp/w3451/unstripped.so --map /tmp/w3451/data.map
python3 _work/reconstructions/rodata_content_map_round2.py \
  --map /tmp/w3451/data.map \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so

# the checkHints literal-pool evidence
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStation10checkHintsEv

# lock the landed gain
python3 tools/ratchet.py --build-dir cmake-build-match --update-baseline
```
