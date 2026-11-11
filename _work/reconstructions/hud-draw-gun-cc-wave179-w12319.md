# Wave 179 (w12319): `Hud::draw` image-id local (raw 148 -> 147);
# `Gun::calcCharacterCollision` left at the base (458)

Worker: w12319. Branch `wave179-w12319`, base master `64014ce6f`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w12319` (not tracked).

## 0. Result

| target | symbol | VMA | size | before raw/aligned | after raw/aligned |
|---|---|---|---:|---:|---:|
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | `0x152f90` | 1984 | 458 / 63 | **458 / 63** (no change) |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | 148 / 147 | **147 / 156** |

Landed: the `Hud::draw` change only. `Gun::calcCharacterCollision` is left at
the base (no measured win this wave).

Gate after the change: `check` **RATCHET PASS** — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370.  Whole-`.text` differing bytes **77,936 -> 77,935**.
`.rel.plt` byte-identical (0).  `.rodata`, `.data`, `.data.rel.ro`, `.got`,
`.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.ARM.exidx`, `.init_array` all 0;
`.ARM.extab` content diff 700 -> 703 (+3) at the unchanged 156,312-byte /
base-exact section (not a ratchet metric; the section size and VMA are
identical).

The aligned metric for `Hud::draw` moves 147 -> 156: the raw win comes from
loading `image_0x37c` once for the two `GetImage2D` queries, which lands one
more byte of the shipped dock-transfer block.  Raw is the tracked worker
metric; the function is still not byte-exact either way, so the ratchet's
`byte_exact` count is unchanged at 4507.

## 1. `Hud::draw` — landed change

In the docking-transfer bar block (`src/game/ui/Hud.cpp`, ~line 1660):

```cpp
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
```

->

```cpp
unsigned int img = this->image_0x37c;
int barH = Globals::Canvas->GetImage2DHeight(img);
int barW = Globals::Canvas->GetImage2DWidth(img);
```

Semantically identical (the field is not written between the two reads; the
two `GetImage2D*` queries are const getters).  The local makes clang load
`image_0x37c` once into a register and pass it to both calls; the shipped code
reloads the field, but the single-load form happens to land one more byte of
the surrounding register shuffle.  This is the same `img` local that wave 178
(w12219, `hud-draw-wave178-w12219.md`) measured at 147 on its branch; it was
not in this worker's base (master `64014ce6f` still had the two-load form).

Region split (`fn_diff` byte groups): damage block `0x164188-0x16419c` 19
(unchanged), dock-transfer block `0x1658a2-0x165a28` 129 -> **128**,
everything else 0.  Total 148 -> **147**.

### Remaining `Hud::draw` residual (all in the dock-transfer block)

The 128-byte dock residual is the cyclic register permutation documented in
`gun-hud-wave177-w12119.md` / `hud-draw-wave178-w12219.md`.  Shipped:
`textH=r9`, `&Canvas=fp`, `total=r5`, `text=r6`, `barW=r8`, `barH=fp`,
`top=r6`, `barYOffset=r4`; ours: `textH=fp`, `&Canvas=r6`, `total=r9`,
`text=r5`, `barH=r8`, `barW=r5`, `top=r4`, `barYOffset=fp`.  The dock block is
a 4-cycle `r9 -> fp -> r6 -> r5 -> r9`.

Also stuck: the `getText(reversed ? 3204 : 3205)` select.  Shipped emits
`movw r1,#3205; cmp r8,#0; it eq; movweq r1,#3204` (default 3205, `eq`).
Every spelling tried (`reversed ? 3205 : 3204`, `!reversed ? 3204 : 3205`,
`reversed == 0 ? 3204 : 3205`, `0 == reversed ? ...`, `int`/`unsigned char`
`reversed`) makes clang emit the `it ne` form with default 3204, which costs
1-2 raw bytes relative to the base's `it ne` + default 3205.

### Measured `Hud::draw` variants this wave (all 9,332 B unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base (`short frameH`, `short textH`, two field loads) | 148 | 147 |
| **`img` local, height then width (landed)** | **147** | 156 |
| `img` + `int textH` | 1,706 | 162 |
| `img` + `int frameH` | 1,748 | 83 |
| `img` + `int frameH` + `int textH` | 1,514 | 178 |
| `img` + width first | 152 | 169 |
| `img` + `reversed ? 3205 : 3204` | 148 | 156 |
| `img` + `!reversed ? 3204 : 3205` | 148 | 156 |
| `img` + `int reversed` / `unsigned char reversed` | 147 | 156 |
| `img` + `PaintCanvas *canvas` local | 416 | 193 |
| `img` + `String *textPtr` / `const String &` | 147 | 156 |
| `img` + `barOffset` operand swap / temp | 147 | 156 |
| `img` + `long barH/barW` | 147 | 156 |
| `img` + `unsigned int barH/barW` | 1,567 | 160 (9,328 B) |
| `img` + `short barH/barW` | 1,650 | 173 (9,340 B) |

Best aligned is `int frameH` + `img` (83) but it is 9,328 B (4 short: the
shipped `mov r5,fp` save and the pool-alignment `nop` are gone because the
Canvas pointer lives in `r6` instead of `fp`).  `short frameH, int textH,
height-first, no img` is even better at aligned 63 but also 9,328 B, and its
2-byte shift over ~0x6b4 bytes makes raw 1,697.  Raw rewards same-size
instructions, so the base + `img` form (147) stays the best raw.

## 2. `Gun::calcCharacterCollision` — no win (458 stands)

Residual is exactly the `r4`/`r5` slot/index swap documented in
`gun-calccharactercollision-w7610.md`, `-wave131-w7701.md`,
`-wave132-w7714.md`, `-wave133-w7739.md`, `-wave176-w12017.md` and
`-wave178-w12219.md`:

| value | shipped | ours |
|---|---|---|
| `i*3` | **r4** | r5 |
| scatter `playerPosition` sret | **sp+44** | sp+104 |
| scatter `delta` (`operator-`) sret | **sp+104** | sp+44 |
| `Matrix view` alloca | sp+104 | sp+104 |
| k-loop `Player::getPosition` sret | sp+104 (rematerialised) | sp+104 (hoisted) |

Root cause unchanged: StackColoring merges the scatter `playerPosition` sret
into the `Matrix view` alloca slot (both sp+104) and the `operator-` sret takes
sp+44; shipped keeps `playerPosition` at sp+44 and `delta` at sp+104.  Early
Machine LICM also hoists the k-loop `t2ADDri` stack adds into the i-loop
preheader, so greedy RA gives the `i*3` index `r5` instead of `r4`, and the
k-loop is top-tested instead of the shipped rotated form
(`movs r5,#0; b.n <cond-at-bottom>`).

Variants measured this wave (all 1,984 B unless noted):

| variant | raw | size |
|---|---:|---:|
| base | **458** | 1,984 |
| `for (k=0;;++k) { if (k>=size) break; ... }` | 458 | 1,984 |
| `while (k < size)` / `for (k=0; k<size; k=k+1)` | 458 | 1,984 |
| `if (!__builtin_expect(diff>0,0)) continue;` | 458 | 1,984 |
| `expect(diff<=0,1) {} else {...}` / `if(...){}else{}` | 458 | 1,984 |
| `diff >= 1` / `int diff = magnitude; diff -= distance;` | 458 | 1,984 |
| `Vector delta = enemyPosition - playerPosition;` (named) | 458 | 1,984 |
| `playerPosition - enemyPosition` | 460 | 1,984 |
| `(void) playerPosition;` dummy use | 458 | 1,984 |
| `expect(k < size, 1)` on the loop condition | 1,799 | 1,948 |
| `expect(k < size, 0)` | 1,714 | 1,944 |
| `while (expect(k<size,1))` | 1,870 | 1,896 |
| `do { ... } while (k < size)` | 1,032 | 1,988 |
| `do { ... } while (++k < size)` | 1,032 | 1,988 |
| `do` without `expect` | 1,872 | 1,912 |
| reverse `for (k=size; k-- > 0;)` | 1,021 | 1,988 |
| `diff != 0` | 1,042 | 1,980 |
| `Vector delta;` declared before `playerPosition` | 1,645 | 1,964 |

The shipped k-loop is rotated; the base is top-tested.  A `do-while` rotates
but costs 4 bytes because it re-loads `enemies` at the loop head (the shipped
reuses the bottom test's `ldr r0,[r9,#180]` as the next body's `[r0,#4]`
base).  Every `expect` spelling that rotates the loop moves `this` out of r9
and shrinks the body below 1,984 B.  No source shape found that keeps the
1,984 B frame and the shipped rotation + slot assignment; the function is left
at the base.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj _ZN3Gun22calcCharacterCollisionEv
# Hud raw=147 aligned=156 size=9332/9332
# Gun raw=458 aligned=63 size=1984/1984
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY 5370/5370
cmake --build cmake-build-match --target drift -j2   # DRIFT 0
cmake --build cmake-build-match --target lint -j2    # CLEAN
```

Section diff (same helper as wave 178): compare `objdump -h` size/file-off and
count differing bytes at the same offset.  With the landed change: `.text`
77,935, `.ARM.extab` 703, `.rel.plt` 0, every other alloc section 0.

## 4. Files changed

`src/game/ui/Hud.cpp` (the `img` local) and this note.  `Gun.cpp` is untouched.
Gate: byte 4507 / linked 4507, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` identical.
