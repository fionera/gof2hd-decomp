# Wave 177 (w12119): `Hud::draw` reverse-flag anchor (raw 150 -> 148);
# `Gun::calcCharacterCollision` still the `r4`/`r5` LICM-hoist swap (458)

Worker: w12119. Branch `wave177-w12119`, base master `9445ac5fb`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w12119` (not tracked).

## 0. Result

| target | symbol | VMA | size | before raw/aligned | after raw/aligned |
|---|---|---|---:|---:|---:|
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | `0x152f90` | 1984 | 458 / 63 | **458 / 63** (no change) |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | 150 / 143 | **148 / 147** |

Landed: the `Hud::draw` change only. `Gun::calcCharacterCollision` is left at
the base (no measured win this wave).

Gate after the change: `check` **RATCHET PASS** — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370.  Whole-`.text` differing bytes **79,591 -> 79,589**.
`.rel.plt` byte-identical (0).  `.ARM.extab` 1,321 and `.ARM.exidx` 0
unchanged; `.rodata`, `.data`, `.data.rel.ro`, `.got`, `.dynsym`, `.dynstr`,
`.hash`, `.rel.dyn` all 0.

The aligned metric for `Hud::draw` moves 143 -> 147: the raw win comes from
matching the shipped `r8` flag and the `ldrb`/`movw` order, which reorders a
few instructions and costs 4 aligned-instruction mismatches.  Raw is the
tracked worker metric; the function is still not byte-exact either way, so
the ratchet's `byte_exact` count is unchanged at 4507.

## 1. `Hud::draw` — landed change

In the docking-transfer bar block (`src/game/ui/Hud.cpp`, ~line 1649):

```cpp
int total = ego->getDockTotalAmount();
// Codegen anchor: reading the reverse flag into a named bool after the
// dock-amount calls makes clang keep it in r8 (the shipped register)
// and lets the getText select reuse it; the original reloads the
// field once for both uses.  Semantically identical (no writes to
// field_0x279 in between), raw diff 150 -> 148 at the same 9,332 B.
bool reversed = this->field_0x279;
float progress = (float) transferred / (float) total;
if (reversed)
    progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
String text = *Globals::gameText->getText(reversed ? 3204 : 3205) + String(" ");
```

`reversed` is a plain `bool` copy of `this->field_0x279`; the two uses are
the same two reads the original performs.  The ternary is written with the
arms swapped (`reversed ? 3204 : 3205`) because that is the spelling that
makes clang emit the shipped `movw r1,#3205; it ne; movwne r1,#3204`
sequence with the flag in `r8`.

### Why it moves the bytes

Shipped dock2 head (`0x1658a0`):

```
1658a0  mov   r5, fp          ; r5 = Canvas
1658a2  mov   fp, r9          ; fp = &Globals::Canvas
1658a4  mov   r9, r0          ; r9 = textH
1658a6  mov   r0, r5
1658a8  blx   getDockTransferedAmount   -> r4
1658ae  mov   r0, r5
1658b0  blx   getDockTotalAmount        -> r5
1658b6  ldr   r0, [pc]        ; gameText
1658b8  ldrb.w r8, [sl, #633] ; flag -> r8
1658bc  movw  r1, #3205
1658c2  cmp.w r8, #0
1658ca  it    eq
1658cc  movweq r1, #3204
1658d0  blx   getText
1658d6  mov   r6, r0          ; text -> r6
```

Before this wave (raw 150) ours kept the flag in `r9`, `total` in `r8`,
`text` in `r5`, `&Globals::Canvas` in `r6` and `textH` in `fp`; the flag
load and the `movw` were in the shipped order but the register names were
permuted.  With the `reversed` local the flag lands in `r8` and the
`ldrb`/`movw` order matches.  Remaining dock2 residual (about 128 B) is the
rest of the same cyclic register permutation: shipped `textH=r9`,
`&Canvas=fp`, `total=r5`, `text=r6`; ours `textH=fp`, `&Canvas=r6`,
`total=r9`, `text=r5`.

### Damage block (19 raw, irreducible this wave)

```
164184  tst.w  r1, #36
164188  mov.w  r0, #300
16418c  it     ne
16418e  strne.w r8, [sp, #0xd0]   ; dead volatile address store (the anchor)
164192  str.w  r0, [sl, #0x4b8]   ; unconditional
164196  tst.w  r1, #24
16419a  it     ne
16419c  strne.w r0, [sl, #0x4b4]
```

Shipped is `tst; itt ne; movne; strne` for both blocks (28 B total).  The
current unbraced `Status **volatile p = &Globals::status;` anchor is what
keeps the *whole function's* register allocation identical to the shipped
one; the dead address store (4 B) is compensated by the shared unconditional
`mov r0,#300` / last-block `strne` (4 B shorter than the shipped last block).
Every other anchor placement re-colours the function:

| damage shape | raw | size |
|---|---:|---:|
| unbraced address-store anchor (landed) | **148** | 9,332 |
| braced address-store anchor | 7,087 | 9,336 |
| anchor after the `if` | 7,120 | 9,336 |
| anchor before the `if` | 7,130 | 9,336 |
| cast form `(void)(Status *volatile) Globals::status` | 7,336 | 9,328 |
| `volatile int/long/short x = (int)(uintptr_t)&Globals::status` | 148 | 9,332 |
| `Status **const volatile` / `Status *volatile *volatile` | 148 | 9,332 |
| `unsigned int` / `short` / `unsigned char hitFlags` | 148 | 9,332 |

The cast form is the only shape that emits the shipped damage block
byte-for-byte, but it loses the dead `ldr.w r8,[sp,#108]` reload and is
9,328 B; every attempt to add the 4-byte reload back re-colours the whole
function (the reload is an RA artifact of the address being live in the
damage block).  The wave-175 lead ("cast form + a genuine dead 4-byte value
reload of the spilled slot") is still open.

### Show block (1 raw, not code)

`0x165c4c`: `.word 0x000aa9dc` (orig) vs `0x000aa9e0` (ours), the
GOT-relative addend for `Globals::gameText` — a `.got` entry-order artifact.

### `Hud::draw` variants measured this wave

All size 9,332 unless noted; base raw 150 / aligned 143.

| variant | raw | aligned |
|---|---:|---:|
| **`bool reversed` after `total`, inverted ternary (landed)** | **148** | 147 |
| `bool reversed` after `total`, shipped ternary | 149 | 147 |
| `bool reversed` after `textH` / after `transferred` | 154 | 149 |
| `bool reversed` before `frameH` | 181 | 150 |
| `bool reversed` after `frameH` | 173 | 150 |
| `const bool &reversed` after `total` | 148 | 147 |
| `const unsigned char &reversed`, shipped ternary | 150 | 143 |
| `const unsigned char &reversed`, inverted ternary | 151 | 143 |
| `volatile bool reversed` | 1,817 | 195 (9,348 B) |
| `int` / `short` / `unsigned char` reversed | 148-149 | 147 |
| `reversed == 0 ? 3204 : 3205` / `!reversed ? ...` / `reversed+3204` | 148-149 | 147 |
| `long transferred/total` | 148 | 147 |
| `progress` via `/=` or ternary | 148 | 147 |
| `int barYOffset` / `int y = top + top` | 148 | 147 |
| `const String *t = getText(...)` / `+ " "` | 148 | 147 |
| `PaintCanvas *&canvas` reference in the dock block | 150 | 143 |
| `PaintCanvas *canvas` value cache | 3,302 | 368 (9,308 B) |
| `barH/barW` before `String text` | 1,819 | 270 (9,344 B) |
| `barH/barW` before `barOffset` | 1,819 | 270 (9,344 B) |
| `top/y/barYOffset` before `barH/barW` | 213 | 152 |
| `top` before `String text` | 3,299 | 253 (9,344 B) |
| `y/barYOffset` after the fade math | 167 | 146 |
| `int frameH` / `int textH` | 1,748 / 1,697 | 145 / 63 (9,328 B) |
| `frameH`/`textH` declaration swap | 197 | 164 |
| `barOffset` before the dock calls | 148 | 147 |
| `String text` via `+=` | 483 | 111 (9,328 B) |
| `barOffset` in `double` | 1,559 | 188 (9,336 B) |

## 2. `Gun::calcCharacterCollision` — no win (458 stands)

Residual is exactly the `r4`/`r5` swap documented in
`gun-calccharactercollision-w7610.md`, `-wave131-w7701.md`,
`-wave132-w7714.md`, `-wave133-w7739.md` and `-wave176-w12017.md`:

| value | shipped | ours |
|---|---|---|
| `i*3` | **r4** | r5 |
| `pp` (getPlayer()->getPosition sret) | **r4 / sp+44** | r4 / sp+104 |
| `delta` (operator- sret) | **r5 / sp+104** | r5 / sp+44 |

Root cause: StackColoring merges the k-loop `Player::getPosition` sret
(`fi#12`) into the `Matrix view` alloca (`fi#10`); Early Machine LICM hoists
`t2ADDri %stack.10` before `t2ADDri %stack.11` into the i-loop preheader, so
greedy RA gives fi#10=`r4`, fi#11=`r5`.  The shipped has no hoisted stack
adds: it rematerialises `add r5,sp,#104` in the view block (`0x1533c8`) and
`add r4,sp,#104` in the k-loop (`0x153444`), and its k-loop is rotated
(`movs r5,#0; b.n 0x1534fc`, condition at the bottom) while ours is
top-tested.  `-mllvm -no-stack-coloring` flips `i*3` to `r4` but moves the
slots to `sp+112`/`sp+52` and drops the size to 1,948 B.

Variants measured this wave (all 1,984 B unless noted):

| variant | raw | size |
|---|---:|---:|
| base `diff = magnitude - distance; diff > 0` | **458** | 1,984 |
| `while (k < enemies->size())` form | 458 | 1,984 |
| cache `unsigned n = enemies->size()` before the k-loop | 1,713 | 1,960 |
| cache `int n = (int) enemies->size()` | 1,713 | 1,960 |
| cache `Array<Player *> *es = enemies` | 1,738 | 1,952 |

The remaining levers (prevent the LICM hoist, change the dominator-tree
block order, keep `%stack.10` live in a callee-saved register) are unchanged
from the wave-133/176 open questions; no source shape found this wave.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv _ZN3Hud4drawExxP9PlayerEgobjj
# Gun raw=458 aligned=63 size=1984/1984
# Hud raw=148 aligned=147 size=9332/9332
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY 5370/5370
cmake --build cmake-build-match --target drift -j2   # DRIFT 0
```

## 4. Files changed

`src/game/ui/Hud.cpp` (the `reversed` anchor above) and this note.  `Gun.cpp`
is untouched.  Gate: byte 4507 / linked 4507, size-exact 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370, `.text`
79,589 differing bytes, `.rel.plt` identical.
