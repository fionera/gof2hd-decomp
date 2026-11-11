# Hud::draw — wave 174 (w11917): dock type/order shape (raw 223 -> 168)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`, 77 LSDA rows (`.ARM.extab` 0x26298).

| metric | before (wave 173) | after |
|---|---:|---:|
| function raw bytes | 223 | **168** |
| aligned | 183 | **146** |
| size | 9,332 | 9,332 |
| whole-`.text` diff | 80,897 | **80,842** |
| whole-`.ARM.extab` diff | 675 | **678** |
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
| prologue | 1 | 0 |
| damage | 25 | 25 |
| mission | 0 | 0 |
| dock1 | 0 | 0 |
| dock2 | 197 | **142** |
| rate | 0 | 0 |
| show | 0 | 1 |
| tail | 0 | 0 |
| sum | 223 | **168** |

## Landed source shape

Four independent changes, all in `Hud::draw`:

1. **Armor-regen `DrawRegion2D` pivotY is 0, not `dividerOffset`.**  The old
   `dividerOffset` pivotY was a codegen anchor from wave 135.  With the wave-173
   show block landed it is no longer needed; the original stores `r4,r4` (0,0)
   in `strd r4,r4,[sp,#12]` at `0x163ff4`.  Changing the argument to `0` makes
   the whole prologue byte-identical (prologue 1 -> 0, `.text` -1).

2. **`short frameH`, `short textH`.**  Both are `int` in the original (no
   `sxth`), but the `short` form re-colours the callee-saved tie in the dock
   block: the extra `sxth` instructions shift the allocator from
   `barW=r5, barH=r8, Canvas=fp` to a closer match of the original's
   `barW=r8, barH=fp, Canvas=r5`.  This is the same class of measured shape
   anchor as wave 168's `short barH`; it is size-exact (9,332 B) and the
   instruction count is 3227 vs the original's 3226 (one `sxth`).

3. **`int barH` / `int barW`, `barH` declared first.**  The faithful dock types
   (the original has no `sxth` for `barH` and uses signed `barW/2` rounding).
   Declaring `barH` before `barW` makes the compiler emit
   `GetImage2DHeight` before `GetImage2DWidth`; the original calls Width first,
   but the reversed order is a measured raw win (the register allocation lands
   closer).  Both calls are pure getters.

4. **`top`/`barYOffset` before the fade math.**  `int top = this->field_0x3e2;`
   and `unsigned int barYOffset = Globals::layout->field_0x218;` are moved
   above `this->field_0x468 += dt;` / `fadeIn` / `fade`.  This reproduces the
   original's `ldrh r6,[sl,#0x3e2]` / `ldr r4,[r1,#536]` schedule in the dock
   block (`0x165984`/`0x16598a`).

The landed dock block:

```cpp
short frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
short textH = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
int transferred = ego->getDockTransferedAmount();
int total = ego->getDockTotalAmount();
float progress = (float) transferred / (float) total;
if (this->field_0x279)
    progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
int top = this->field_0x3e2;
unsigned int barYOffset = Globals::layout->field_0x218;
this->field_0x468 += dt;
float fadeIn = (float) this->field_0x468 / 1000.0f;
float fade = 1.0f;
if (fadeIn < 1.0f)
    fade = fadeIn;
```

## Measurements

The type sweep (frameH/textH × barW/barH, 288 combinations, all with the
`dividerOffset` removal) found exactly one size-exact family below the 223
baseline:

| frameH | textH | barW | barH | raw | aligned | size |
|---|---|---|---|---:|---:|---:|
| short | short | int | int | **181** | 163 | 9,332 |
| char | char | int | int | 181 | 166 | 9,332 |
| short | short | long | long | 181 | 163 | 9,332 |
| short | short | int | int, `barH` first | **176** | 149 | 9,332 |
| short | short | int | int, `barH` first, `top` before fade | **168** | **146** | 9,332 |

The last row is landed.  Other variants measured on the landed types:
`top`/`barYOffset` swapped 191 raw, fade ternary neutral (181), progress
ternary neutral (181), `barOffset` operand order neutral (181).

## Remaining residual (168 raw)

* **Damage block 25 raw.**  The `Status * volatile` anchor is still the
  unbraced form; the original's `ldr.w r8,[sp,#108]` sits after
  `lsls r0,r1,#30` (`0x164176`) while ours is hoisted before it (`0x164174`),
  and the `&0x24`/`&0x18` stores share one unconditional `mov r0,#300`.
  Every braced/volatile re-placement measured either +4 B (9,336) or a
  whole-function re-colour.  The anchor is kept.
* **Dock block 142 raw.**  The remaining differences are register names
  (`barW`/`barH`/`Canvas`) and the `Globals::w` pointer in `fp` vs stack
  `#96`; the `short frameH`/`short textH` shape is the closest size-exact
  colouring found.
* **Show block 1 raw.**  A single branch-offset byte from the one extra
  `sxth`; the show block is otherwise exact.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
# raw=168 aligned=146 size=9332/9332
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY
```
