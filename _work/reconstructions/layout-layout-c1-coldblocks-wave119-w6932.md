# Layout::Layout C1/C2 — wave 119 / worker w6932
## Cold outlined blocks: 4 iPad tails in the original vs 1 retinaDisplay tail in ours

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6932`, base = wave-119 master `99b118964`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

This pass attacks the task's cold-outlined-code angle: enumerate both cold
regions, identify the source construct that produces the original's four cold
blocks, and test whether that construct can be reproduced at exactly 7072 B.

## Result — no landable source win; note-only commit

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (single-object link) | 48.963317384 % | **48.963317384 %** |
| `verify` pct (1 dp, build-dir report) | 49.0 | 49.0 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the 27 section headers, file size 2,187,328 B,
`.text`, `.ARM.exidx`, `.ARM.extab` (`0x26290`, 4 B short) and `.rodata`
are unchanged.

## 1. Exact cold-region enumeration

The function is `0xd1028..0xd2bc8` (7072 B).  The last hot instruction is the
epilogue `pop {r4,r5,r6,r7,pc}`; everything after it is the cold region
(outlined tails + alignment pad + literal pool).

| body | `pop` | cold region after `pop` | cold blocks | pad | end pool | end-pool words |
|---|---:|---:|---:|---:|---:|---:|
| **original** | `0xd2800` | **966 B** | `0xd2802..0xd2852` = **80 B** | 2 B | `0xd2854..0xd2bc8` = 884 B | **221** |
| HEAD | `0xd28bc` | **778 B** | `0xd28be..0xd28ce` = **16 B** | 2 B | `0xd28d0..0xd2bc8` = 760 B | **190** |
| delta | | **+188 B = 47 words** | **+64 B = 16 words** | 0 | **+124 B = 31 words** | **+31** |

(The task brief's "46 words / 52 B cold" is the older w6720 census; the later
wave-119 Layout statement-order passes changed the HEAD cold block from 26 B
to 16 B, so the current split is 16 cold + 31 pool = 47 words.)

### Original: four cold blocks, all reached by `beq.w` from the "else" path

All four are the final `Globals::iPad ? A : B` arm of a nested ternary chain
in the non-iPadHD / non-n9 path.  They are if-converted (`cmp; it eq; moveq`)
but placed after the epilogue.

**Block A — `0xd2802`, 38 B** (`0x130`/`0x134`/`v138` chain's final else):

```
d2802: ldr   r6, [sp, #4]      ; r6 = &Globals::iPad (spilled at 0xd1880)
d2804: movs  r2, #108
d2806: ldrb  r1, [r6]          ; iPad
d2808: cmp   r1, #0
d280a: mov.w r1, #56
d280e: it    eq
d2810: moveq r2, #80
d2812: it    eq
d2814: moveq r1, #40
d2816: strd  r1, r2, [r0, #304] ; 0x130 = iPad?56:40, 0x134 = iPad?108:80
d281a: movs  r1, #47
d281c: ldrb  r2, [r6]          ; iPad again
d281e: cmp   r2, #0
d2820: it    eq
d2822: moveq r1, #35
d2824: b.w   0xd1cba           ; join: store 0x138 = v138
```

**Block B — `0xd2828`, 14 B** (`t_field_0x2c8`'s final `iPad ? 45 : 30`):

```
d2828: ldrb  r1, [r6]          ; iPad
d282a: movs  r6, #45
d282c: cmp   r1, #0
d282e: it    eq
d2830: moveq r6, #30
d2832: b.w   0xd1814           ; join: vmov/vcvt/vstr 0x2c8
```

**Block C — `0xd2836`, 14 B** (`field_0x2d0`'s final `iPad ? 45 : 30`):

```
d2836: ldrb  r1, [r3]          ; iPad
d2838: movs  r2, #45
d283a: cmp   r1, #0
d283c: it    eq
d283e: moveq r2, #30
d2840: b.w   0xd1860           ; join: vmov/vcvt/vstr 0x2d0
```

**Block D — `0xd2844`, 14 B** (`field_0x60`'s final `iPad ? 64 : 44`):

```
d2844: ldrb  r1, [r6]          ; iPad
d2846: movs  r6, #64
d2848: cmp   r1, #0
d284a: it    eq
d284c: moveq r6, #44
d284e: b.w   0xd16d6           ; join: vmov/vcvt/vstr 0x60
```

Branch sites into the four blocks (all `beq.w`, i.e. taken when the preceding
`Globals::retinaDisplay` test is false):

| block | branch site | condition | source line |
|---|---|---|---|
| A `0xd2802` | `0xd1caa` | `r8` = retinaDisplay == 0 (after iPadLarge false) | 116–138 (`v138` chain) |
| B `0xd2828` | `0xd180e` | `r2` = retinaDisplay == 0 | 48 (`t_field_0x2c8`) |
| C `0xd2836` | `0xd185a` | `r2` = retinaDisplay == 0 | 46 (`field_0x2d0`) |
| D `0xd2844` | `0xd1716` | `r1` = retinaDisplay == 0 | 37 (`field_0x60`) |

### HEAD: one cold block, `0xd28be`, 16 B

The final `retinaDisplay ? 232 : 116` arm of `field_0x1ac` (source line 179):

```
d28be: movs  r1, #232
d28c0: cmp.w r8, #0            ; r8 = &Globals::retinaDisplay
d28c4: it    eq
d28c6: moveq r1, #116
d28c8: str.w r1, [r0, #428]    ; 0x1ac
d28cc: b.n   0xd2198           ; join
```

Branch site: `0xd224e` `beq.w 0xd28be`, taken when `fp` (n9) is false.

## 2. The source construct

Both binaries compile the same flat per-field ternary list; clang 7.0.2
hoists the common `Globals::iPadHD` test into one big straight-line block
(`0xd1042..0xd1268` orig, `0xd1042..0xd1274` HEAD), then a `Globals::n9`
block, then a shared "else" region of `retinaDisplay` / `iPadLarge` / `iPad`
ternaries.  The cold blocks are not a separate source statement: they are the
**innermost `Globals::iPad ? A : B` of a ternary chain**, which clang
if-converts but places after the epilogue when the containing block is a
separate low-frequency chain.

The difference is *chain depth* and *tail merging*, not the expression kind:

* **`field_0x60`** (line 37):
  `iPadHD ? 64*1.40625f : (n9 ? 79 : (iPadLarge ? 128 : (retinaDisplay ? 88 : (iPad ? 64 : 44))))`.
  In the else path the chain is
  `iPadLarge ? 128 : (retinaDisplay ? 88 : (iPad ? 64 : 44))` — three
  conditions.  The original outlines the `iPad ? 64 : 44` tail (block D).
  In HEAD the `retinaDisplay` arm is **tail-merged with `field_0x5c`'s
  `retinaDisplay` arm** (both set `r6 = 88` and continue to the 0x60 store),
  so HEAD's chain collapses to `iPadLarge ? 128 : (iPad ? 64 : 44)` — two
  conditions — and the `iPad` tail is inlined (`0xd16da`).

* **`t_field_0x2c8`** (line 48) and **`field_0x2d0`** (line 46): same 5-arm
  shape.  The original outlines the final `iPad ? 45 : 30` (blocks B/C).
  HEAD keeps the `retinaDisplay ? 60 : (iPad ? 45 : 30)` chain inline
  (`0xd1802..0xd1826` for 0x2d0, `0xd1856..d188c` for 0x2c8).

* **`0x130`/`0x134`/`v138`** (lines 116–138): an explicit
  `if (iPadHD) … else if (n9) … else if (iPadLarge) … else if (retinaDisplay) … else { 0x130 = iPad?56:40; … }`.
  The original outlines the whole final `else` (block A).  HEAD inlines it
  as the fallthrough of the `retinaDisplay` test (`0xd1df6..0xd1e1e`).

* HEAD's one cold tail (`field_0x1ac`, line 179) is the mirror case: its chain
  is `iPadLarge ? 346 : (iPad ? 173 : (n9 ? 208 : (retinaDisplay ? 232 : 116)))`
  — four conditions — so the last `retinaDisplay` arm is low-frequency enough
  to be placed after the epilogue.

**Proven:** the four original cold blocks are exactly the final `iPad` arms of
the 0x60 / 0x2c8 / 0x2d0 / 0x130 chains, and HEAD's one cold block is the
final `retinaDisplay` arm of the 0x1ac chain.  The store traces (all six flag
configurations) are value-identical between the two binaries.

**Hypothesis:** the original source order keeps the 0x5c and 0x60
`retinaDisplay` arms unmerged (the original tests `retinaDisplay` twice, at
`0xd16aa` for 0x5c and `0xd170c` for 0x60), which deepens the 0x60 chain by
one level and lets clang outline the `iPad` tail.  Reproducing that from HEAD
requires changing the *block layout* (which successor is the fallthrough), not
just the expression.

## 3. Pool / SLP census

End pool (`pop`+2 .. `0xd2bc8`):

| | original | HEAD | delta |
|---|---:|---:|---:|
| end-pool bytes | 884 | 760 | +124 |
| end-pool words | 221 | 190 | +31 |
| `vld1.64` in the else region (`0xd1570..0xd2800` / `0xd1568..0xd28bc`) | **52** | **44** | **+8 groups** |
| `vld1.64` in the whole function | 86 | 77 | +9 |

The +8 else-region `vld1.64` groups are exactly the +32 pool words the task
brief refers to (8 × 16 B).  The remaining end-pool delta is −1 word: the
original has 8 fewer GOT-offset literals than HEAD (22 vs 23 in the w6720
census), so 32 − 1 = 31.

The 8 missing SLP windows (from the w6720 census, still the right set) are
the non-HD field runs `0xb0..0xbc`, `0xec..0xf8` (two branches),
`0x13c..0x148`, `0x140..0x14c`, `0x16c..0x178`, `0x180..0x18c`,
`0x2a0..0x2ac`.  In HEAD those fields are assigned far apart in source order
(e.g. 0xb0, 0xa0, 0xd0, 0xd4, 0xcc, 0xd8, 0xb8, 0xb4), so clang emits scalar
`str`/`strd` and the four constants never enter the pool.

## 4. Experiments (all real builds; exact linked metric)

Method: `/dev/shm/w6932/build.sh <src> <tag>` compiles only `Layout.cpp` with
the exact `compile_commands.json` flags, and `/dev/shm/w6932/score.sh <tag>`
gold-links that single object into a throwaway `.so`, GNU-strips it, and runs
the `tools/verify/verify.py` `Resolver` + `reconcile` +
`difflib(autojunk=False)` pipeline on `_ZN6LayoutC1Ev`.  It reproduces the
full-link number bit-for-bit (base 48.963317384 %).

| variant | change | size | ratio % |
|---|---|---:|---:|
| **HEAD (base)** | — | **7072** | **48.963317** |
| `swap5c60` | swap source lines 0x5c / 0x60 | 7072 | 48.644338 |
| `order2c8` | source order 0x2c8, 0x2cc, 0x2d0 (original store order) | 7072 | 48.923445 |
| `g_16c` | make 0x16c/0x170/0x174/0x178 adjacent | 7072 | 48.963317 (codegen no-op) |
| `40`/`41` | adjacent-statement swap sweep, all 239 candidates | 7072 | max = base |
| `0x1ac` move sweep | move line 179 to every statement position (239) | 7072 | max = base |
| `0x130` block move sweep | move lines 116–138 to every position (316) | 7072 | max = base |
| `60_if` | 0x60 as explicit 6-arm if/else | 7056 | 48.284118 |
| `60_rd_first` | 0x60 chain `retinaDisplay` before `iPadLarge` | 7064 | 47.746310 |
| `60_rd_only2` | drop 0x60 `iPadLarge` arm | 7040 | 44.595405 |
| `60_ipad_first` | 0x60 chain `iPad` before `iPadLarge` | 7000 | 42.350110 |
| `2d0_if` | 0x2d0 as explicit 6-arm if/else | 7064 | 48.534397 |
| `2c8_if` | 0x2c8 as explicit 6-arm if/else | 7064 | 47.695071 |
| `g_b0` | make 0xb0/0xb4/0xb8/0xbc adjacent | 7080 | 40.828520 |
| `g_2a0` | make 0x2a0..0x2ac adjacent | 7080 | 44.563919 |
| `g_180` | make 0x180..0x18c adjacent | 6956 | 47.408598 |
| `1ac_rd_first` | 0x1ac chain `retinaDisplay` before `n9` | 7064 | 47.624750 |
| `1ac_n9_rd` | 0x1ac chain `n9 ? (retinaDisplay ? 232 : 116) : 208` | 7056 | 48.043131 |
| `1ac_ipad_first` | 0x1ac chain `iPad` before `iPadLarge` | 7064 | 45.751895 |
| `reorder` | reorder every statement to the original's first-store order | 6736 | 33.150518 |
| `bigif` | extract the iPadHD arm of all 167 simple fields into one big `if/else` | 6264 | 13.330530 |

Notes on the two structural experiments:

* **`reorder`**: the original's first-store order was extracted from the
  disassembly (`0x14,0x18,0x1c,0x20, 0x4,0x8,0xc,0x10, 0x24…0x70, 0x2c8…0x2d4,
  0x7c…0x88, 0x74,0x78, 0x2c0,0x2c4,0x2d8, 0xac, 0x8c,0x90, 0xa4,0xa8,
  0xc0…0xcc, 0xb0…0xbc, 0xec…0xf8, 0x294…0x2b0, 0x108…0x114, 0x118…0x124,
  0xdc…0xe8, 0xfc…0x104, 0x2b4,0x2b8, 0x128,0x12c, 0x130,0x134, 0x140…0x1dc,
  0x138,0x13c, 0x1e0, …`).  Applying it to our expressions gives **6736 B**
  (hot 6020 B, cold+pool 716 B), i.e. 336 B *smaller* than the original.
  So the original order alone is not the missing 336 B; the original's
  expressions must add ~336 B (the 4 cold tails + 8 SLP groups + a larger
  hot body), and our current scrambled order is what keeps HEAD at 7072 B.
  Because any size change from 7072 B collapses whole-`.so` `byte_exact`
  (7052 B → 3221), the reorder is unlandable.

* **`bigif`**: extracting the `Globals::iPadHD` arm of all 167 simple fields
  into one `if (iPadHD) { … } else { … }` gives 6264 B / 13.3 %.  The flat
  ternary list is therefore the correct macro shape; clang's code hoisting
  already produces the same big straight-line iPadHD block.

## 5. Proven vs hypothesis

**Proven (measured):**

* Original cold region = **80 B in four blocks** (`0xd2802` 38 B, `0xd2828`
  14 B, `0xd2836` 14 B, `0xd2844` 14 B); HEAD = **16 B in one block**
  (`0xd28be`).  Delta +64 B.
* The four original cold blocks are the final `Globals::iPad ? A : B` arms of
  the `0x130`/`0x134`/`v138`, `t_field_0x2c8`, `field_0x2d0` and
  `field_0x60` chains.  HEAD's one cold block is the final
  `retinaDisplay ? 232 : 116` arm of `field_0x1ac`.
* End-pool delta = **+31 words** (221 vs 190); the else-region `vld1.64`
  count differs by **+8 groups** (52 vs 44), i.e. +32 words, minus one
  GOT-offset word.
* The current HEAD is a local optimum for every single adjacent swap (239),
  every single-statement move of `field_0x1ac` (239), and every move of the
  `0x130`/`0x134`/`v138` block (316) at 7072 B.
* Explicit if/else conversions of the cold chains shrink the body (7056 /
  7064 B) and score lower; condition reorders change the size and score
  lower; SLP-adjacency moves either overshoot (7080 B) or undershoot
  (6956 B) and score far lower.

**Hypothesis (not proven):**

* The original source has the 0x5c and 0x60 `retinaDisplay` arms unmerged
  (two separate `retinaDisplay` tests), which deepens the 0x60 chain by one
  level and lets clang place the `iPad` tail after the epilogue.  The same
  mechanism (chain depth + which successor is the fallthrough) explains the
  0x2c8/0x2d0/0x130 tails.  Reproducing it from HEAD requires a source
  ordering that keeps the chains unmerged *and* keeps the total at exactly
  7072 B; every ordering measured here lands below 7072 B.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6932
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6932/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6932/ours.dis
```

Scratch harnesses (not committed, `/dev/shm/w6932/`): `build.sh`,
`buildflag.sh`, `score.sh`, `score.py`, `align.py`, `align2.py`, `align3.py`,
`census.py`…`census4.py`, `stores2.py`, `reorder3.py`, `bigif.py`,
`variants.py`, `sweep_adj.py`, `sweep_move1.py`, `sweep_block.py`,
`Layout.reorder.cpp`, `Layout.bigif.cpp`.
