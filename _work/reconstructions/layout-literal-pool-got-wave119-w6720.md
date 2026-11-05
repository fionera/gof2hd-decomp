# Layout::Layout C1/C2 — wave 119 / worker w6720
## Literal-pool / GOT hypothesis: the 46 missing words are SLP groups + outlined cold blocks, not GOT slots

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6720`, base `47a1396bd` (wave-118 harvest).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — hypothesis tested, no landable source improvement; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.594777756 % | **48.594777756 %** |
| `byte_exact` / `linked_exact` | 4503 / 4503 | **4503 / 4503** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the 27 section headers, file size 2,187,328 B,
`.text` and `.ARM.exidx` are unchanged.  `.ARM.extab` remains the wave-118
state: ours `0x26290`, original `0x26298` (8 B short, pre-existing; not
touched here).

## 1. Exact pool census (reproduces the 966/241 vs 782/195 numbers)

The function is `0xd1028..0xd2bc8`.  The `pop {r4,r5,r6,r7,pc}` is the last
hot instruction; everything after it is the "pool region" used by the task's
count (cold outlined blocks + alignment padding + literal pool):

| body | `pop` | region after `pop` | cold blocks | pad | end pool | end-pool words |
|---|---:|---:|---:|---:|---:|---:|
| **original** | `0xd2800` | **966 B** | `0xd2802..0xd2852` = 80 B | 2 B | `0xd2854..0xd2bc8` = 884 B | **221** |
| HEAD | `0xd28b8` | **782 B** | `0xd28ba..0xd28d4` = 26 B | 2 B | `0xd28d8..0xd2bc8` = 752 B | **188** |
| delta | | **+184 B = 46 words** | **+52 B = 13 words** | 0 | **+132 B = 33 words** | **+33** |

`966/4 = 241.5` and `782/4 = 195.5`, i.e. the task's "241 words / 195 words"
is the byte count divided by four (the region is not 4-aligned because it
starts at `pop+2`).

### End-pool word classification

Every word in `0xd2854..0xd2bc8` was classified by the instruction that
references it (`ldr rX,[pc,#imm]`, `add rX,pc,#imm`/`addw`/`subw` feeding a
`vld1.64`, `vldr`, or `vld1.64` feeding a `vst1.32`):

| kind | original | HEAD | delta |
|---|---:|---:|---:|
| `vld1.64` SLP group words (4 per group) | **200** (50 groups) | **168** (42 groups) | **+32 = +8 groups** |
| `vldr` scalar float words | **12** | **10** | **+2** |
| `ldr` GOT-offset words | **8** | **9** | **−1** |
| unreferenced (tail padding) | 1 | 1 | 0 |
| **total** | **221** | **188** | **+33** |

The `+32` vld1 words are eight unique 16-byte (four-int) SLP constant blocks.
The `+2` vldr words are one extra `100.0f` and one extra `50.0f` (the
`field_0x9c` float arms; see §4).  The `−1` ldr word is one fewer GOT-offset
literal in the original.

## 2. GOT hypothesis — disproved

Hypothesis: "the original loads globals through a local `Globals *` or
per-field literals (GOT-slot addresses) rather than direct `Globals::x`
accesses."

Both binaries use exactly the same idiom for every `Globals::<flag>` read:

```
ldr  rX, [pc, #imm]        ; pool word = GOT-relative offset
add  rX, pc                ; rX = address of the GOT slot
ldr  rX, [rX]              ; rX = &Globals::flag
ldrb rX, [rX]              ; flag value
```

Counting every `ldr rX,[pc]` whose pool word is a GOT-relative offset
(`0x0013xxxx`) and whose next `add rX,pc` resolves into `.got`
(`0x20fcf4..0x214000`):

| | original | HEAD |
|---|---:|---:|
| GOT-offset literals (islands + end pool) | **22** | **23** |
| resolved flag symbols | iPadHD, retinaDisplay, n9, iPad, iPadLarge, iPadLargePossible | same six |

The original has **one fewer**, not more.  There is no local `Globals *`
cache and no extra per-field GOT literal.  The 46-word deficit is entirely
SLP grouping + cold-block outlining.

## 3. The 33 missing end-pool words

### 3a. 32 words = 8 four-int SLP groups

The original's end pool contains **50** unique 16-byte SLP blocks; ours
contains **42** (net **+8**).  Comparing exact 4-tuples, 31 original blocks
have no exact match in HEAD and 23 HEAD blocks have no exact match in the
original; the 23 are the shifted-window variants of the same field runs
(e.g. `0x204`→`0x208`).  The net 8-block deficit is the following eight
windows, where the original emits a 4-int `vld1.64 {dA-dB}` +
`vst1.32 {dA-dB}` group and ours does not (destination offsets from the
linear disassembly):

| # | original pool addr | destination window | 4 pool words (hex) | decoded ints | source construct |
|---:|---|---|---|---|---|
| 1 | `0xd2980` | `0xb0..0xbc` | `18 dc c8 d2` | 24,220,200,210 | four adjacent `this->field_0xb0/b4/b8/bc = <ternary>` assignments |
| 2 | `0xd28b0` | `0xec..0xf8` | `3c 78 12c 258` | 60,120,300,600 | adjacent `ringNearMidThreshold(0xec)`, `field_0xf0`, `ringMidFarThreshold(0xf4)`, `field_0xf8` (n9 arm) |
| 3 | `0xd2960` | `0xec..0xf8` | `fe 2 1 ff` | −2,2,1,−1 | same four fields, other branch |
| 4 | `0xd2920` | `0x13c..0x148` | `0d 0e 0a 168` | 13,14,10,360 | `vbsl` input for the `0x13c..0x148` vectorised ternary |
| 5 | `0xd2910` | `0x140..0x14c` | `13 13 13 f3` | 19,19,19,243 | adjacent `field_0x140/144/148/14c` (n9 arm) |
| 6 | `0xd29e0` | `0x16c..0x178` | `106 f8 54 78` | 262,248,84,120 | adjacent `field_0x16c/170/174/178` |
| 7 | `0xd29f0` | `0x180..0x18c` | `1e 15a 1e 1be` | 30,346,30,446 | adjacent `field_0x180/184/188/18c` |
| 8 | `0xd2990` | `0x2a0..0x2ac` | `50 a0 190 320` | 80,160,400,800 | adjacent `field_0x2a0/2a4/2a8/2ac` |

The 23 HEAD-only tuples are the shifted-window variants of the same fields:
the original groups `0x204,0x214,0x224,0x23c,0x24c,0x25c,0x26c`, HEAD groups
`0x208,0x218,0x228,0x240,0x250,0x260,0x270` (one field later).  HEAD also has
an extra `0xdc` group and two extra `0x16c` groups.  Several SLP blocks are
shared between windows (e.g. the `0x108` block is reused at `0x2a4`), so the
net unique-block deficit is exactly 8.

**Source shape that emits each:** four consecutive integer field assignments
whose ternaries are adjacent in the source.  clang 7.0.2 at `-Oz` SLP-folds
four adjacent `int` stores into one `vld1.64 {dA-dB}` (16 B of literal-pool
constants) + one `vst1.32 {dA-dB}`.  In HEAD these fields are assigned far
apart (e.g. `field_0xb0` line 71, `field_0xb4` line 84, `field_0xb8` line 77,
`field_0xbc` line 82), so clang emits scalar `str.w`/`strd` instead and the
four constants never enter the pool.

### 3b. 2 words = extra scalar `100.0f` / `50.0f`

The original end pool has two copies each of `100.0f` (`0x42c80000`) and
`50.0f` (`0x42480000`); HEAD has one copy each plus `82.0f`/`41.0f`.
These are the `field_0x9c` float arms:

```cpp
this->field_0x9c = (int)(float)(Globals::iPadHD ? 70 : (Globals::n9 ? 75 :
                   (Globals::retinaDisplay ? 100 : 50)));
```

The original materialises the `100.0f`/`50.0f` pair in two separate branch
tails; HEAD shares one pair and instead materialises `82.0f`/`41.0f` for
`field_0xac`.  This is a type/arm-sharing difference, not a wrong value.

### 3c. −1 word = one fewer GOT-offset literal

See §2: original 22 GOT-offset literals, HEAD 23.

## 4. The 13 cold-block words = 52 B of outlined `else` tails

The original outlines **four** `else` tails after the `pop`; HEAD outlines
**one** different tail:

| original block | size | source construct |
|---|---:|---|
| `0xd2802` | 38 B | `field_0x130/0x134/v138` chain (lines 117–137): `else { 0x130 = iPad ? 56 : 40; 0x134 = iPad ? 108 : 80; v138 = iPad ? 47 : 35; }` |
| `0xd2828` | 14 B | `t_field_0x2c8` chain (line 48): `else { 45 : 30 }` (`iPad ? 45 : 30`) |
| `0xd2836` | 14 B | `field_0x2d0` chain (line 46): `else { 45 : 30 }` (`iPad ? 45 : 30`) |
| `0xd2844` | 14 B | `field_0x60` chain (line 37): `else { 64 : 44 }` (`iPad ? 64 : 44`) |
| **total** | **80 B** | |

HEAD's one outlined tail is `0xd28ba` (28 B):
`field_0x1ac`/`field_0x1a0` chain (lines 189–190): `232 : 116` and `14 : 7`.

The original's four tails all end in a `Globals::iPad ? A : B` arm and are
outlined because clang decided the `iPad` test was cold; HEAD inlines those
four and outlines a different chain.  The 52 B difference is exactly the
13-word cold-block component of the 46.

## 5. Adjacency experiments (all real `link_so` builds, unrounded ratio)

Each experiment moves the named source statements so the four fields are
adjacent in address order, then rebuilds and scores `_ZN6LayoutC1Ev`.

| variant | change | size | score % |
|---|---|---:|---:|
| HEAD | — | **7072** | **48.594778** |
| A | `0xec,0xf0,0xf4,0xf8` fully adjacent at `0xf0` | 6920 | 38.336347 |
| B | `0xec` before `0xf0`, `0xf4` before `0xf8` | 6976 | 42.384106 |
| C | `0xec` before `0xf0` only | 6976 | 42.504515 |
| D | `0xf4` before `0xf8` only | 7064 | 43.403747 |
| E | `0x140,0x144` before `0x148` | 7036 | 46.270746 |
| G | `0x204` before `0x208` | 7024 | 45.303425 |

Every adjacency variant reproduces the correct SLP grouping but shrinks the
body below the mandatory 7072 B and loses more `difflib` alignment than it
gains.  A 7076-B ctor collapses `byte_exact` 4503 → 2454 (re-measured in
wave 117/118), so the 7072-B wall is hard.

## 6. Proven vs hypothesis

**Proven (measured):**

* The 46-word gap is **33 end-pool words + 13 cold-block words**:
  32 words = 8 four-int SLP groups, 2 words = extra `100.0f`/`50.0f`,
  −1 word = one fewer GOT-offset literal; 52 B = four outlined `else` tails
  vs one.
* Both binaries use the identical GOT idiom for every `Globals::<flag>` read.
  The original has **22** GOT-offset literals, HEAD **23** — the GOT
  hypothesis is disproved.
* The original has **50** unique 16-byte SLP pool blocks, HEAD **42**.
  The destinations are the non-HD field runs `0xb0`, `0xec`, `0x13c`,
  `0x140`, `0x16c`, `0x180`, `0x2a0`, plus the shifted `0x204`/`0x208`
  window.
* The per-configuration store traces (six flag configurations) are
  **value-identical** between original and HEAD; only grouping/order differs.

**Hypothesis (not proven):**

* The original source assigns those four-field runs consecutively in address
  order (the pool blocks and `vst1.32` destinations are consistent with that),
  and the exact original statement order is what lets clang outline the four
  `iPad`-tail `else` blocks.  Reproducing it from HEAD needs a source shape
  that both restores the 8 SLP groups and keeps the total code+pool at
  exactly 7072 B; every adjacency variant measured here lands below 7072 B.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6720
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4503

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6720/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6720/ours.dis
```

Pool census (self-contained): parse each `objdump` line, take the `; (dXXXX <...>)`
or `; dXXXX <...>` annotation, read 4 bytes for `ldr`/`vldr`/`adr` and 16 bytes
for `vld1.64 {dA-dB}` (two 64-bit D registers = 16 B), and classify by the
referencing mnemonic.  `pop` is the last `pop {...,pc}` before the pool.
GOT slots resolve with `tools/verify/elf.py` `Elf32.got_map()`; the slot
address is `(add_pc + 4) + pool_word` for the 16-bit `add rX,pc` idiom.

Scratch harnesses (not committed): `/dev/shm/w6720/{parse.py,groups.py,
poolcensus.py,classify2.py,got.py,emu.py,score.py,trace_seq.py}`,
`Layout.base.cpp`, `orig.dis`, `ours.dis`, `orig.census.txt`,
`ours.census.txt`, `orig.groups.txt`, `ours.groups.txt`.
