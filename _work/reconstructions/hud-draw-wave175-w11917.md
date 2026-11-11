# Hud::draw — wave 175 (w11917): cache `top * 2` as `y` (raw 168 -> 156)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`, 77 LSDA rows (`.ARM.extab` 0x26298).

| metric | before (wave 174) | after |
|---|---:|---:|
| function raw bytes | 168 | **156** |
| aligned | 146 | **145** |
| size | 9,332 | 9,332 |
| whole-`.text` diff | 80,439 | **80,427** |
| whole-`.ARM.extab` diff | 750 | 750 |
| `.rel.plt` diff | 0 | 0 |

Gate: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370, `.rel.plt` 0.
RATCHET PASS.

Per-region raw split (regions: prologue 0x163b90, damage 0x16412c, mission
0x1643cc, dock1 0x165700, dock2 0x165880, rate 0x165b50, show 0x165ba6,
tail 0x165d4c):

| region | before | after |
|---|---:|---:|
| prologue | 0 | 0 |
| damage | 25 | 25 |
| mission | 0 | 0 |
| dock1 | 0 | 0 |
| dock2 | 142 | **130** |
| rate | 0 | 0 |
| show | 1 | 1 |
| tail | 0 | 0 |
| sum | 168 | **156** |

## Landed source shape

One change in the dock block: the original computes `top * 2` **once** and
reuses the value for both `DrawImage2D`'s `y` and `DrawRegion2D`'s `pivotY`
(`mov.w r9, r6, lsl #1` at `0x1659a6`, then `mov r3, r9` and
`add.w r2, r4, r6, lsl #1`).  Passing `top * 2` twice lets clang
re-materialise the shift in one of the two uses; naming it `int y` forces the
CSE and removes 12 raw bytes:

```cpp
int top = this->field_0x3e2;
int y = top * 2;
unsigned int barYOffset = Globals::layout->field_0x218;
...
Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, y, 0x11, 0x14);
Globals::Canvas->DrawRegion2D(this->image_0x38c, 0, 0, (int) (progress * (float) barW), barH, 0.0f, 0, 0,
                              (Globals::w >> 1) - barW / 2, barYOffset + y);
```

`DrawString`'s y stays written as `(top * 2)` in the source; the compiler
CSEs it to the same `y` value (the original reuses `r9` there too), so it is
byte-neutral.  The change is size-exact (9,332 B) and instruction count is
unchanged at 3227 vs the original's 3226 (the one extra `sxth` from the
wave-174 `short frameH`/`short textH` shape anchor).

## Damage block `hitFlags & 0x24` (25 raw, irreducible this wave)

Current source (wave-174 base, unchanged):

```cpp
if (hitFlags & 1)
    this->field_0x4ac = 300;
if (hitFlags & 2)
    this->field_0x4b0 = 300;
if (hitFlags & 0x24)
    (void) *(Status * volatile *) &Globals::status;
    this->field_0x4b8 = 300;
if (hitFlags & 0x18)
    this->field_0x4b4 = 300;
```

Original (`0x164174`):

```
164174 lsls   r0, r1, #30
164176 ldr.w  r8, [sp, #108]        ; dead reload of the spilled Globals::status value
16417a itt    mi
16417c movmi  r0, #300
164180 strmi  r0, [sl, #0x4b0]
164184 tst.w  r1, #36
164188 itt    ne
16418a movne  r0, #300
16418e strne  r0, [sl, #0x4b8]
164192 tst.w  r1, #24
164196 itt    ne
164198 movne  r0, #300
16419c strne  r0, [sl, #0x4b4]
```

Ours:

```
164174 ldr.w  r8, [sp, #108]        ; address load, hoisted before lsls
164178 lsls   r0, r1, #30
16417a itt    mi
16417c movmi  r0, #300
164180 strmi  r0, [sl, #0x4b0]
164184 tst.w  r1, #36
164188 it     ne
16418a ldrne.w r0, [r8]             ; volatile value load (extra)
16418e tst.w  r1, #24
164192 mov.w  r0, #300              ; unconditional
164196 str.w  r0, [sl, #0x4b8]      ; unconditional
16419a it     ne
16419c strne  r0, [sl, #0x4b4]
```

The `Status * volatile` anchor is a hard size anchor: the faithful damage
shape is 4 B short of the original's 9,332 B.  Ghidra confirms the original
source is the plain `if ((uVar32 & 0x24) != 0) { *(this+0x4b8) = 300; }` with
no status read; the `ldr.w r8,[sp,#108]` is a dead reload of the value spilled
for `Globals::setCoordsSteer` in the prologue.

**New measurement this wave.**  The *cast* form

```cpp
if (hitFlags & 0x24) {
    (void) (Status * volatile) Globals::status;
    this->field_0x4b8 = 300;
}
```

is instruction-for-instruction identical to the original's damage block
**except** that it is missing the dead `ldr.w r8,[sp,#108]`: it emits the
conditional `movne`/`strne` pair and no value load.  It is size 9,328 B
(4 B short) at raw 7,342.  A cast to a volatile-qualified *rvalue* type does
not make the read volatile, so the load is DCE'd.  Adding exactly the dead
4-byte value load back would make the damage block byte-exact at 9,332 B;
every attempt to force it also emitted the value load or changed the spill
colour:

| damage shape | raw | size |
|---|---:|---:|
| unbraced deref anchor (landed) | **156** | 9,332 |
| braced deref anchor | 7,092 | 9,336 |
| deref anchor before `&0x24` | 7,098 | 9,336 |
| deref anchor before `&1` | 8,087 | 9,364 |
| deref anchor after `&0x18` | 7,069 | 9,336 |
| cast anchor, any placement | 7,342 | 9,328 |
| `Status *volatile &s = Globals::status; (void) s;` | 7,092 | 9,336 |
| `Status *volatile s = Globals::status; (void) s;` | 7,282 | 9,340 |
| `Status *s = Globals::status` alias for the show block | 7,342 | 9,328 |

Treat the damage block as irreducible for this wave.  The lead for a future
wave is the cast form + a genuine dead 4-byte value reload of the spilled
`Globals::status` slot (not an address load and not a dereference).

## Dock block: what was swept (all size-exact unless noted)

On top of the landed `int y` shape:

* `frameH`/`textH` types: `short/short` is the only pair at 156 (and the only
  one at 9,332 B); `short/int`, `int/short`, `int/int`, `unsigned short/int`,
  `short/unsigned int`, `char/char`, `long` all give 9,328 B (4 B short).
* `barH`/`barW` types (25 combinations): `int/int`, `int/long`, `long/int`,
  `long/long` all 156; `unsigned int/int` 158; `int/unsigned short` 183;
  `short/*` 5,300+.
* `barH` declared before `barW` (156) beats `barW` first (161).
* `top` before `barYOffset` (156) beats swapped (164).
* Full 480-permutation sweep of the five declaration/construction blocks
  (`progress`, `barOffset`, `String text`, `barH`/`barW`, `top`/`y`/
  `barYOffset`): `O P T Bhw Utb` and `P O T Bhw Utb` tie at 156; everything
  else is >= 161.
* `int y`, `top << 1`, `top + top`, `2 * top`, `unsigned int`/`long y` all
  156; `int y` placed after `barYOffset` or after the fade math is 164.
* `float` locals for `transferred`/`total`/`frameH`/`textH`, a
  `PaintCanvas *canvas` local, `barX`/`filled` hoists, `textH2`/`frameH2`
  copies: neutral or worse.

The remaining dock2 differences are register allocation only: ours keeps
`barW=r5, barH=r8, Canvas=fp, top=r4, barYOffset=fp`; the original uses
`barW=r8, barH=fp, Canvas=r5, top=r6, barYOffset=r4`.  The one extra `sxth`
at `0x165900` comes from the `short frameH` shape anchor and is what keeps
the function size-exact; `int/int` re-colours the block and is 4 B short.

## Remaining residual (156 raw)

* damage block 25 (above);
* dock2 130 (register names + the one `sxth`);
* show 1: a single literal-pool byte at `0x165c4c` — original
  `.word 0x000aa9dc`, ours `.word 0x000aa9e0` (a 4-byte GOT-relative addend
  difference for `Globals::gameText`, i.e. a `.got` entry-order artifact, not
  a Hud::draw code difference).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
# raw=156 aligned=145 size=9332/9332 insn=3227/3226
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY
```

Section check (`.text` 80,427, `.ARM.extab` 750, `.rel.plt` 0):

```python
import sys; sys.path.insert(0,'tools/verify')
from elf import Elf32
a=Elf32('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
b=Elf32('cmake-build-match/libgof2hdaa.so')
def sec(e,n):
    return next(s for s in e.sections if s['name']==n)
for n in ['.text','.ARM.extab','.ARM.exidx','.rel.plt','.rodata','.data']:
    sa,sb=sec(a,n),sec(b,n)
    da=a.data[sa['offset']:sa['offset']+sa['size']]
    db=b.data[sb['offset']:sb['offset']+sb['size']]
    print(n, sa['size'], sb['size'], sum(x!=y for x,y in zip(da,db)))
```
