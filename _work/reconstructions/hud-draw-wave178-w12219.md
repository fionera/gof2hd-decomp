# Wave 178 (w12219): `Hud::draw` image-id local (raw 148 -> 147);
# `Gun::calcCharacterCollision` left at the base (458)

Worker: w12219. Branch `wave178-w12219`, base master `1b48f990f`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w12219` (not tracked).

## 0. Result

| target | symbol | VMA | size | before raw/aligned | after raw/aligned |
|---|---|---|---:|---:|---:|
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | `0x152f90` | 1984 | 458 / 63 | **458 / 63** (no change) |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | 148 / 147 | **147 / 156** |

Landed: the `Hud::draw` change only. `Gun::calcCharacterCollision` is left at
the base (no measured win this wave).

Gate after the change: `check` **RATCHET PASS** - byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370.  Whole-`.text` differing bytes **79,251 -> 79,250**.
`.rel.plt` byte-identical (0).  `.rodata`, `.data`, `.data.rel.ro`, `.bss`,
`.got`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash`, `.ARM.exidx`,
`.init_array` all 0.

`.ARM.extab` content diff moves **731 -> 734** (+3) at the unchanged
156,312-byte / base-exact section (the section size and VMA are identical;
only 3 more of its bytes differ, in an LSDA call-site record elsewhere in the
image).  This is not a ratchet metric; `check` passes with it.  `.text` and
`.rel.plt` are the tracked byte sections and both improve/hold.

The aligned metric for `Hud::draw` moves 147 -> 156: the raw win comes from
shortening the dock-transfer block by one byte, which re-orders a handful of
downstream instructions.  Raw is the tracked worker metric; the function is
still not byte-exact either way, so the ratchet's `byte_exact` count is
unchanged at 4507.

## 1. `Hud::draw` - landed change

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

Semantically identical (the field is not written between the two reads).  The
local makes clang load `image_0x37c` once into a register and pass it to both
calls; the shipped code reloads the field, but the single-load form happens to
land one more byte of the surrounding register shuffle.

Region split (`fn_diff` byte groups):

| region | before | after |
|---|---:|---:|
| damage block `0x164188-0x16419c` | 19 | 19 |
| dock-transfer block `0x1658a2-0x165a28` | 129 | **128** |
| everything else | 0 | 0 |
| **total** | **148** | **147** |

### Measured variants (all 9,332 B unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base (both calls read `this->image_0x37c`) | 148 | 147 |
| **`unsigned int img` local, height then width (landed)** | **147** | 156 |
| `int img` local | 147 | 156 |
| `const unsigned int img` local | 147 | 156 |
| width first, no local | 153 | 161 |
| width first, `img` local | 152 | 169 |
| `const unsigned int &img` | 148 | 147 |
| `img` only for the height call | 148 | 147 |
| `img` declared after both calls | 148 | 147 |
| `img` declared before `String text` | 94,928 | - (frame shifts) |

The remaining 128-byte dock residual is the same cyclic register permutation
documented in `gun-hud-wave177-w12119.md`: shipped `textH=r9`, `&Canvas=fp`,
`total=r5`, `text=r6`, `barW=r8`, `barH=fp`; ours `textH=fp`, `&Canvas=r6`,
`total=r9`, `text=r5`, `barH=r8`, `barW=r5`.

### Damage block (19 raw, unchanged)

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

Shipped is `tst; itt ne; movne; strne` for both blocks.  The unbraced
`Status **volatile p = &Globals::status;` anchor is load-bearing for the whole
function's register allocation: removing it drops the frame to 9,328 B and
raw to 7,345; the cast form `(void) *(Status *volatile *) &Globals::status;`
gives 153 raw at 9,332 B.  The current form (147) is the best measured.

## 2. Open lead: `int frameH` / `int textH`

The shipped `frameH` and `textH` are **not** truncated: the shipped stores the
full `GetImage2DHeight` result (`str r0,[sp,#96]` at `0x16588c`) and feeds
`textH` straight to `vmov s4,r9` / `vcvt.f32.s32 s4,s4` with no `sxth`.
Our source declares both `short`, which emits two `sxth` instructions that the
shipped does not have.  Wave 105 (`hud-draw-short-frameh-w5318.md`) chose
`short` as a size/RA lever; with the current wave-177/178 source it is no
longer size-neutral.

Measured on top of the landed `img` change:

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| `short frameH`, `short textH` (landed) | 147 | 156 | 9,332 | 3,228 |
| `int frameH`, `short textH` | 1,748 | **83** | 9,328 | 3,226 |
| `short frameH`, `int textH` | 1,706 | 162 | 9,328 | 3,227 |
| `int frameH`, `int textH` | 1,514 | 178 | 9,328 | 3,221 |
| `int`/`int`, height-then-width, no `img` local | 1,500 | 175 | 9,328 | 3,220 |
| `int`/`int`, width-then-height, no `img` local | 1,506 | 183 | 9,328 | 3,220 |

`int frameH` alone reproduces the shipped instruction *count* (3,226) and drops
the aligned diff to 83, but the frame is 4 bytes short, so `raw` is dominated
by a 4-byte shift.  The missing bytes are exactly:

1. `mov r5, fp` in the `GetImage2D` block (`0x16594c`): the shipped saves the
   canvas pointer out of `fp` before the second call overwrites `fp` with
   `barH`.  Ours keeps the canvas in `r6`/`fp` and assigns `barH` to `r8`, so
   the save is dead and is not emitted.
2. the pool-alignment `nop` at `0x165c46` that the shipped needs because its
   code reaches the literal pool at `0x165c48`; ours reaches it at `0x165c44`.

`int frameH` + `int textH` gives the shipped early-dock register assignment
(`mov fp,r9` canvas->fp, `mov r9,r0` textH->r9, `mov r5,r0` total->r5) but
loses 5 instructions overall and still ends 4 bytes short.  If a future pass
can recover the `mov r5,fp` + `nop` pair (i.e. force `barH` into `fp` while
`Canvas` is live there), the aligned metric is 83 and the raw should collapse
from ~1.7 kB to a few hundred bytes.  That is the cheapest remaining `Hud::draw`
lever; it was not landed here because every `int` shape is 9,328 B.

## 3. `Gun::calcCharacterCollision` - no win (458 stands)

Residual is exactly the `r4`/`r5` slot/index swap documented in
`gun-calccharactercollision-w7610.md`, `-wave131-w7701.md`,
`-wave132-w7714.md`, `-wave133-w7739.md` and `-wave176-w12017.md`:

| value | shipped | ours |
|---|---|---|
| `i*3` | **r4** | r5 |
| scatter `playerPosition` sret | **sp+44** | sp+104 |
| scatter `delta` (`operator-`) sret | **sp+104** | sp+44 |
| `Matrix view` alloca | sp+104 | sp+104 |
| k-loop `Player::getPosition` sret | sp+104 (rematerialised) | sp+104 (hoisted) |

Root cause: StackColoring merges the scatter `playerPosition` sret into the
`Matrix view` alloca slot (both sp+104) and the `operator-` sret takes sp+44;
shipped keeps `playerPosition` at sp+44 and `delta` at sp+104.  Early Machine
LICM also hoists the k-loop `t2ADDri` stack adds into the i-loop preheader,
so greedy RA gives the `i*3` index `r5` instead of the shipped `r4`, and the
k-loop is top-tested instead of the shipped rotated form
(`movs r5,#0; b.n <cond-at-bottom>`).

Variants measured this wave (all 1,984 B unless noted):

| variant | raw | size |
|---|---:|---:|
| base | **458** | 1,984 |
| `Matrix view; view = ...;` (assignment instead of copy-init) | 1,888 | 2,056 |
| `const Matrix *viewp = ...; Matrix view = *viewp;` | 458 | 1,984 |
| `Vector playerPosition(...)` copy-init | 458 | 1,984 |
| named `Vector delta = enemyPosition - playerPosition;` | 458 | 1,984 |
| `playerPosition` hoisted to the i-loop body (to extend its life) | 1,568 | 1,964 |
| `Matrix view` hoisted to the i-loop body | 1,875 | 2,024 |

No source shape found that keeps the 1,984 B frame and moves the slot/index
assignment; the function is left at the base.

## 4. Reproduction

```
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj _ZN3Gun22calcCharacterCollisionEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
```

Section diff helper (same one used for the numbers above): compare
`objdump -h` section `size`/`File off` and count differing bytes at the same
offset.  With the landed change: `.text` 79,250, `.ARM.extab` 734,
`.rel.plt` 0, every other alloc section 0.

## 5. Files changed

`src/game/ui/Hud.cpp` (the three-line `img` local) and this note.
Gate: byte 4507 / linked 4507, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` identical.
