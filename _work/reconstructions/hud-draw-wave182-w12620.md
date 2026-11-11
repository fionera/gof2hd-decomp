# Wave 182 (w12620): `Hud::draw` dock-transfer register rotation
# (raw 148 -> 32); `Gun::calcCharacterCollision` left at 458; build audit

Worker: w12620. Branch `wave182-w12620`, base master `57e085059`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w12620`.

## 0. Result

| target | symbol | VMA | size | before raw/aligned | after raw/aligned |
|---|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | 148 / 147 | **32 / 21** |
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | `0x152f90` | 1984 | 458 / 63 | **458 / 63** (no change) |

Landed: the `Hud::draw` change only. `Gun::calcCharacterCollision` is left at
the base (no measured win this wave).

Gate after the change: `check` **RATCHET PASS** — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370.  Whole-`.text` differing bytes **75,365 -> 75,249**
(-116).  `.rel.plt` byte-identical (0).  `.ARM.extab` content diff
**655 -> 651** (-4) at the unchanged 156,312-byte / base-exact section
(`0x1a3b78`).  `.rodata`, `.data`, `.data.rel.ro`, `.got`, `.dynsym`,
`.dynstr`, `.hash`, `.rel.dyn`, `.ARM.exidx`, `.init_array` all 0.
`.note.gnu.build-id` is the only other differing section (20 B).

## 1. `Hud::draw` — landed change

In the docking-transfer bar block (`src/game/ui/Hud.cpp`, ~line 1645):

```cpp
short frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
short textH = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
...
bool reversed = this->field_0x279;
float progress = (float) transferred / (float) total;
if (reversed)
    progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
String text = *Globals::gameText->getText(reversed ? 3204 : 3205) + String(" ");
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
int top = this->field_0x3e2;
int y = top * 2;
unsigned int barYOffset = Globals::layout->field_0x218;
...
    (Globals::w >> 1) - barW / 2, barYOffset + y);
```

becomes:

```cpp
int frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int textH = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
...
bool forward = this->field_0x279 == 0;
float progress = (float) transferred / (float) total;
if (!forward)
    progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
String text = *Globals::gameText->getText(forward ? 3204 : 3205) + String(" ");
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int top = this->field_0x3e2;
int y = top * 2;
unsigned int barYOffset = Globals::layout->field_0x218;
int y2 = barYOffset + y;
...
    (Globals::w >> 1) - barW / 2, y2);
```

Semantics against the binary:

* `GetImage2DHeight`/`GetTextHeight` are declared `int` in
  `PaintCanvas.h`; the old `short` locals inserted two `sxth` instructions the
  original does not have.  The original stores the full `int` results
  (`str r0,[sp,#96]` / `mov r9,r0`).
* The original tests `field_0x279` with `cmp r8,#0` and emits
  `movw r1,#3205; it eq; movweq r1,#3204`, i.e. text id 3204 when the field is
  zero.  `forward = field_0x279 == 0` is that predicate; the old
  `reversed ? 3204 : 3205` had the opposite polarity.
* The original calls `GetImage2DWidth` before `GetImage2DHeight`
  (`0x16593e` / `0x16594e`).
* The original computes `barYOffset + y` once into a register and passes it as
  the last `DrawRegion2D` argument; the named `y2` local is the same value.

### Why it wins

The four changes together reproduce the original's dock-transfer register
rotation.  Before, the block was a 4-cycle `r9 -> fp -> r6 -> r5 -> r9`
(`textH=fp`, `&Canvas=r6`, `total=r9`, `text=r5`, `barW=r5`, `barH=r8`,
`top=r4`, `barYOffset=fp`).  The `int` locals and the `forward` predicate move
`textH` to `r9` and `&Canvas` to `fp`; the width-first `barW`/`barH` order and
the `y2` local then move `barW` to `r8`, `barH` to `fp`, `&Canvas` to `r6`,
`top` to `r5`, and `barYOffset` to `r4`.  The residual is now only the
`&Canvas`/`top` pair (`r6`/`r5` vs the shipped `r5`/`r6`) plus the damage
block's 20-byte anchor residual.

Per-region raw split (base -> this pass):

| region | before | after |
|---|---:|---:|
| damage block `0x164188-0x16419b` | 20 | 20 |
| dock-transfer block `0x1658a2-0x165a27` | 128 | **12** |
| everything else | 0 | 0 |
| **total** | **148** | **32** |

The 12 remaining dock bytes are all one-byte register-operand differences:
`0x16594c` (`mov r6,fp` vs `mov r5,fp`), `0x165987`, `0x16599e`, `0x1659a8`,
`0x1659bd`, `0x1659c1`, `0x1659d0`, `0x165a14`, `0x165a18`, `0x165a1c-1e`,
`0x165a48-4a`.

## 2. `Gun::calcCharacterCollision` — no win (458 stands)

Residual is the `r4`/`r5` slot/index swap and the rotated k-loop documented in
`gun-calccharactercollision-w7610.md`, `-wave131-w7701.md`,
`-wave132-w7714.md`, `-wave133-w7739.md`, `-wave176-w12017.md`,
`-wave178-w12219.md` and `-wave179-w12319.md`.  The StackColoring merge of the
scatter `playerPosition` sret with the `Matrix view` alloca is real: the
first 228-byte residual region starts at `0x153198` with an extra
`add r4,sp,#104` where the original has `ldr.w fp,[sp,#40]`.

Measured this wave (all 1,984 B unless noted):

| variant | raw | size |
|---|---:|---:|
| base | **458** | 1,984 |
| `Vector delta = enemyPosition - playerPosition;` | 458 | 1,984 |
| `Vector delta = enemyPosition - getPlayer()->getPosition();` | 462 | 1,984 |
| `Vector delta; delta = enemyPosition - playerPosition;` | 1,644 | 1,964 |
| `playerPosition = enemyPosition;` after the distance | 1,654 | 1,964 |

Per-TU flag sweep on `Gun.cpp` (reusing the `tools/per_tu_flags.cmake` A/B
build):

| flags | raw | size |
|---|---:|---:|
| `-mllvm -disable-ssc` | 1,138 | 1,980 |
| `-mllvm -no-stack-coloring` | 1,810 | 1,948 |
| `-mllvm -no-stack-slot-sharing` | 458 | 1,984 |
| `-mllvm -stackcoloring-lifetime-start-on-first-use` | 458 | 1,984 |
| `-mllvm -disable-postra-machine-licm` | 458 | 1,984 |
| `-mllvm -disable-postra-machine-sink` | 513 | 1,984 |
| `-mllvm -enable-earlycse-memssa=false` | 458 | 1,984 |
| `-Xclang -target-feature -Xclang +cheap-predicable-cpsr` | 458 | 1,984 |
| `-Xclang -target-feature -Xclang +prof-unpr` | 458 | 1,984 |
| `-mllvm -no-stack-slot-sharing` + each of the above | 458-513 | 1,984 |

No flag is a win.  The function is left untouched.

## 3. Build-provenance audit

Re-verified against the original with `objdump -h` + a same-offset byte
compare of every section:

* all **27** section headers are present, with identical names, sizes, VMAs
  and file offsets;
* `.comment` (303 B), `.note.android.ident` (152 B), `.note.gnu.gold-version`
  (28 B) and `.ARM.attributes` (56 B) are **byte-identical** (diff 0);
* `.dynsym`, `.dynstr`, `.hash`, `.gnu.version*`, `.rel.dyn`, `.rel.plt`,
  `.plt`, `.rodata`, `.fini_array`, `.data.rel.ro`, `.init_array`,
  `.dynamic`, `.got`, `.data`, `.bss`, `.ARM.exidx` are all diff 0;
* the only differing sections are `.text` (75,249), `.ARM.extab` (651) and
  `.note.gnu.build-id` (20).  File size is exact (2,187,328 B).

The compile/link recipe is therefore still the original one: NDK r18b clang
7.0.2, `-Oz -g -fstack-protector-strong -ffunction-sections -funwind-tables
-mthumb -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti`, plain gold
`-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`,
post-link GNU `strip --strip-all`, then `tools/reorder_shstrtab.py`.  No
remaining flag lever was found for the residual functions; the wins are
source-shape-driven.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj _ZN3Gun22calcCharacterCollisionEv
# Hud raw=32 aligned=21 size=9332/9332
# Gun raw=458 aligned=63 size=1984/1984
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY 5370/5370
cmake --build cmake-build-match --target drift -j2   # DRIFT 0
cmake --build cmake-build-match --target lint -j2    # CLEAN
```

## 5. Files changed

`src/game/ui/Hud.cpp` (the dock-transfer block above) and this note.
`Gun.cpp` is untouched.  Gate: byte 4507 / linked 4507, size-exact 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` identical.
