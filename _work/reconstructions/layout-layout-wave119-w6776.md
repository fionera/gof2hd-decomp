# Layout::Layout C1/C2 — wave 119 / worker w6776
## SLP-group / cold-block reconstruction sweep: no 7072-B source win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6776`, base `49f2ada5e` (wave-119 harvest).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — note only, no source change landed

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.624950 % | **48.624950 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B, `.text`/`.rodata`/`.ARM.extab`
unchanged (no source change).

## 1. Confirmed: the 8 missing SLP groups are source-order constructs

The wave-119 w6720 census is reproduced exactly: original 81 `vst1.32` stores
(50 unique 16-B SLP blocks), HEAD 70 stores (42 blocks).  The 8 net missing
four-int groups are the runs `0xb0..bc`, `0xc0..cc`, `0xec..f8`, `0x13c..148`,
`0x140..14c`, `0x16c..178`, `0x180..18c`, `0x2a0..2ac`.

The grouping is produced purely by **source adjacency**: clang 7.0.2 `-Oz`
SLP-vectorises four consecutive int stores that live in the same basic block,
reordering them into address order.  Two concrete demonstrations (real
`link_so` builds):

* Moving `field_0xb0` immediately before `field_0xb8` on HEAD produces **both**
  the `0xb0` and the `0xc0` groups (the old `0xb4`/`0xc4` windows shift by one
  field and become the original windows).  Body 7072 → **7040** B, ratio
  48.594778 → **42.6 %**.
* Reordering the whole `0xa0..0x12c` block to the recovered iOS/default order
  (`a0 a4 b0 b8 b4 bc c0 c4 c8 cc d0 d4 d8 dc e0 e4 e8 ec f0 f4 f8 fc 100 104
  294 298 29c 2a0 2a4 2a8 2ac 2b0 2b4 2b8 108 10c 110 114 118 11c 120 124 128
  12c`) produces the original groups at `0xb0`, `0xc0`, `0xec`, `0x294`,
  `0x2a4`, `0x108`, `0x118`.  Body 7072 → **7000** B, ratio **43.1 %**.

So the constructs are real, but each group removes ~16 B of hot scalar stores
and adds 16 B of pool; the net body shrinks and the fuzzy alignment (dominated
by the shifted literal pool) gets worse.

## 2. The body-size wall is exact and non-additive

`byte_exact` is 4505 only while the ctor is exactly 7072 B.  A 7076-B ctor
re-measured this wave collapses `byte_exact` to **2454** (`RATCHET FAIL`), so
the 7072-B wall is hard.  Because literal-pool padding is 4-aligned, every
candidate edit moves the body in 4-B quanta and the size effect of two edits is
**not** the sum of their individual effects (pool island placement changes).

## 3. Real source bugs found (but each changes the body size)

### 3a. `field_0x138` has one round-trip too many

Original (`d1ccc`/`d1cf0`):
```
vcvt.f32.s32 s2, s0     ; s0 = v138
vcvt.s32.f32 s2, s2
vstr s2, [r0, #312]     ; 0x138
```
HEAD (`d1dc6..d1dca`) emits **four** conversions (two round-trips), i.e. the
source is `(int)(float)((int)(float)v138)` instead of `(int)(float)v138`.
Removing the inner `(int)(float)` is the faithful fix; it removes 8 B
(7072 → **7064**, ratio 48.644338).  It needs an +8 B compensation elsewhere to
stay at 7072, which was not found.

### 3b. `field_0x2d4` / `field_0x2d8` are a `strd` pair in the original

Original (`d18b6`): `strd r4, r6, [r0, #724]` (0x2d4/0x2d8).
HEAD emits two separate `str.w`.  Assigning both through locals
(`int v2d4; ...; this->field_0x2d4 = v2d4; this->field_0x2d8 = v2d8;`) makes
clang emit `strd r6, r5, [r0, #724]`, matching the original store shape, but
body 7072 → **7068** B (ratio 48.245614 on the 7076 base; 48.146672 on HEAD).

## 4. Cold-block outlining

The original outlines four `else` tails after `pop` (80 B total):
`field_0x130/0x134/v138` (38 B), `t_field_0x2c8` (14 B), `field_0x2d0` (14 B),
`field_0x60` (14 B).  HEAD outlines one different tail (`field_0x1ac`,
28 B).  The original's four tails are all the innermost `Globals::iPad ? A : B`
of a nested ternary; clang outlines them because the preceding
`retinaDisplay` test makes the `!retinaDisplay` arm cold.

Attempts to force the outline from source:
* `field_0x60` as a statement-level `if/else-if` chain: body **7032** B,
  ratio 47.3 %, and it changes the store from `vstr` to plain `str` (the
  original is `vstr`), so the shape is wrong.
* Hoisting `iPad ? 64 : 44` into a local `v60`: body **7016** B, ratio 32.7 %.
* Hoisting `t_field_0x2c8`'s `iPad ? 45 : 30` into a local: body **7208** B,
  ratio 21.9 % — it outlines many unrelated blocks.
* Reordering `0x2c4..0x2d8` to the iOS order `2c8 2cc 2d0 2d4 2d8`: body
  **7064** B, ratio 47.0 %.

The outline decision is a whole-function block-placement effect; no local
source shape reproduced exactly the original's four tails at 7072 B.

## 5. Measured variant table (all real `link_so` builds, 7072-B required)

| variant | body | ratio % |
|---|---:|---:|
| HEAD (landed) | **7072** | **48.624950** |
| `field_0x138` inner-cast removal | 7064 | 48.644338 |
| `field_0x2d0` outer-cast removal | 7072 | 48.624950 (codegen no-op) |
| `field_0x2d4/0x2d8` strd shape | 7068 | 48.146672 |
| `field_0x150` float arms | 7072 | 48.275862 |
| `field_0x2c8` round-trip removal | 7068 | 47.537388 |
| `field_0xa8` temp removal | 7068 | 48.624950 |
| `field_0x10c` outer cast removal | 7060 | 48.405104 |
| `field_0x160` outer cast removal | 7068 | 48.664807 |
| `field_0x1a8` before `field_0x1a0` | 7076 | 48.685259 |
| HEAD + `0x1a8`/`0x1a0` swap + `0x138` fix | 7068 | 48.704663 |
| `0xb0` before `0xb8` (2 SLP groups) | 7040 | 42.6 |
| `a0..12c` iOS order (7 SLP groups) | 7000 | 43.1 |
| `0x94`/`0x1f4` swap | 7064 | 48.7 |
| `0x130` chain as written | 7072 | 48.624950 |

A full adjacent-swap sweep (202 pairs) on HEAD found no 7072-B improvement
(consistent with w6750).  A ±2-statement move sweep (300 moves) likewise found
none.  The best 7072-B variant remains HEAD at **48.624950 %**.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6776
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so
```

Scratch harnesses (not committed): `/dev/shm/w6776/{score.py,dests.py,
opcodes.py,sweep_adj.py,sweep_m2.py,combo.py,struct_map.py,curorder.py}`
plus `Layout.base.cpp` / `Layout.base2.cpp` and the variant disassemblies.
