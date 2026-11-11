# Hud::draw — wave 173 (w11818): show-block order + inline `filled` (raw 548 -> 223)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`, 77 LSDA rows (`.ARM.extab` 0x26298).

| metric | before | after |
|---|---:|---:|
| function raw bytes | 548 | **223** |
| aligned | 220 | **183** |
| size | 9,332 | 9,332 |
| `.ARM.extab` | 0x26298 (77 rows) | 0x26298 (77 rows) |
| whole-`.text` diff | 81,503 | **81,178** |
| whole-`.ARM.extab` diff | 711 | **677** |

Gate: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370, `.rel.plt` 0.
RATCHET PASS.

Per-region raw split (regions: prologue 0x163b90, damage 0x16412c, mission
0x1643cc, dock1 0x165700, dock2 0x165880, rate 0x165b50, show 0x165ba6,
tail 0x165d4c):

| region | before | after |
|---|---:|---:|
| prologue | 6 | 1 |
| damage | 25 | 25 |
| mission | 0 | 0 |
| dock1 | 0 | 0 |
| dock2 | 197 | 197 |
| rate | 0 | 0 |
| show | 311 | **0** |
| tail | 9 | 0 |
| sum | 548 | **223** |

The entire show block now matches the original instruction-for-instruction;
only the damage block (25) and the dock block (197) remain.

## Show block: the landed shape

```cpp
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int top = this->field_0x3e2;
int barYOffset = Globals::layout->field_0x218;
this->field_0x464 += dt;
float fade = (float) this->field_0x464 / 1000.0f;
int fadeColor = fade < 1.0f ? (int) (fade * 255.0f) - 256 : 0xffffffff;
int y = barOffset + top * 2;
int y2 = y + barYOffset;
Globals::Canvas->SetColor(fadeColor);
Globals::Canvas->DrawImage2D(this->image_0x378, Globals::w >> 1, y, 0x11, 0x14);
Globals::Canvas->DrawRegion2D(this->image_0x37c,
    (int) ((float) halfW - progress * (float) halfW), 0,
    (int) (progress * (float) halfW + progress * (float) halfW),
    barH, 0.0f, 0, 0,
    (int) ((float) (Globals::w >> 1) - progress * (float) halfW), y2);
Globals::Canvas->DrawString(...);
```

Four independent anchors are involved; each was isolated by measurement:

1. **Declaration order `text`, `halfW`, `barH`.**  The original's call order is
   `getText`/`String::String`, `GetImage2DWidth`, `GetImage2DHeight`; the old
   `barH, text, halfW` order emitted GetImage2DHeight first.  The 120-order
   sweep in the wave-172 note found `T W H D Y` best but 78 LSDA rows; the
   remaining problem was not the order itself.
2. **`top`/`barYOffset` before the fade math.**  The original reloads
   `top` (`ldrh r6,[sl,#994]`) and `barYOffset` (`ldr r5,[r1,#536]`) at
   0x165c10/0x165c16, interleaved with the fade division.  Declaring them
   before `this->field_0x464 += dt` reproduces that schedule.
3. **`fadeColor` must be a ternary.**  With `int fadeColor = 0xffffffff; if
   (fade < 1.0f) fadeColor = ...;` clang puts the phi in r2 and emits
   `mov r1,r2` before the `SetColor` call (2 extra bytes and a shift of the
   whole tail).  The ternary form `fade < 1.0f ? ... : 0xffffffff` produces
   the original's `sub.w r1,r0,#256` / `mov.w r1,#0xffffffff` pair.  This was
   the single largest fix after the declaration order.
4. **`filled` must be inlined.**  See the LSDA section below.

`y2 = y + barYOffset` is scheduled before `SetColor` in the original
(`add.w r2,r9,r5` at 0x165ca0); computing it as a local before `SetColor`
reproduces that.  Passing `y + barYOffset` directly to `DrawRegion2D` leaves
the add at 0x165cc4 and costs 39 raw bytes.

## LSDA: why `filled` cannot be a local

The original has **77** call-site rows and the show block is a *single* row
`(start 8416, len 204, lp 9284)`.  A local `float filled = progress * (float)
halfW;` between `DrawImage2D` and `DrawRegion2D` makes clang open a second
cleanup region: the LSDA splits into `(8420,42,9280)` and `(8496,126,9286)`,
the epilogue grows a third `b.n` pad at 0x165fd0, and the whole `.ARM.extab`
grows to **0x2629c (78 rows)**, which shifts `.ARM.exidx`/`.dynsym`/`.data*`
and is gate-fatal.  Inlining the expression keeps all five show-block
invokes on one landingpad block:

```
invoke SetColor      unwind %2804
invoke DrawImage2D   unwind %2804
(filled = progress * (float)halfW, CSE'd to one vmul)
invoke DrawRegion2D  unwind %2804
invoke GetTextWidth  unwind %2804
invoke DrawString    unwind %2804
```

The compiler still computes `filled` once (`vmul.f32 s2,s16,s0`) and reuses
it four times, so the inlined source is byte-identical to the original's
single-`filled` form.

## Damage block `hitFlags & 0x24` (still 25 raw)

Original (0x164184, 28 bytes):

```
tst.w r1,#36 ; itt ne ; movne.w r0,#300 ; strne.w r0,[sl,#0x4b8]
tst.w r1,#24 ; itt ne ; movne.w r0,#300 ; strne.w r0,[sl,#0x4b4]
```

The landed unbraced form

```cpp
if (hitFlags & 0x24)
    (void) *(Status * volatile *) &Globals::status;
    this->field_0x4b8 = 300;
if (hitFlags & 0x18)
    this->field_0x4b4 = 300;
```

keeps `0x4b8` unconditional (`mov r0,#300; str r0,[sl,#0x4b8]`) and is the
only shape that stays at 9,332 bytes.  Every re-placement of the
`Status * volatile` anchor was re-measured on top of the new show block:

| variant | raw | size | extab |
|---|---:|---:|---|
| unbraced anchor (landed) | **223** | 9,332 | 0x26298 |
| braced anchor + conditional store | 7,108 | 9,336 | 0x26298 |
| anchor before the `&0x24` if | 7,114 | 9,336 | 0x26298 |
| anchor between the two ifs | 7,100 | 9,336 | 0x26298 |
| anchor after the `&0x18` if | 7,085 | 9,336 | 0x26298 |
| anchor before `&1` | 8,057 | 9,360 | 0x26298 |
| anchor between `&1`/`&2` | 8,058 | 9,360 | 0x26298 |
| `(void) &Globals::status;` | 7,488 | 9,320 | 0x26298 |
| `(void) Globals::status;` | 7,488 | 9,320 | 0x26298 |

The original's `ldr.w r8,[sp,#108]` at 0x164176 is a dead spill reload of
`&Globals::status` (r8 is overwritten with `barH` at 0x165bf8 and reloaded
again at 0x165d48).  The anchor reproduces it, but the volatile load inside
the `if` also emits `ldrne r0,[r8]`, which is the +4 bytes.  Removing the
volatile load (address-only forms) drops the function to 9,320.  The 25
damage raw bytes are therefore irreducible with the current anchor; the
damage block is byte-different but size-exact.

## Dock block (197 raw, unchanged)

The dock block (`frameH/textH/transferred/total/barW/barH`) is the same 197
raw as before this pass.  Its remaining differences are register allocation
(`barW` r8 vs r5, `barH` fp vs r8) and the `frameH`/`textH` stack-slot order
(sp+0x60 vs sp+0x5c).  Ternary forms of `fade`/`progress` and reordering
`frameH`/`textH` were measured neutral (223 raw in every case), so the block
was left untouched.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
# raw=223 aligned=183 size=9332/9332
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY
```

LSDA row dump (needs the parser in `_work/reconstructions/extab_census.py`):

```python
import importlib.util
spec=importlib.util.spec_from_file_location('ec','_work/reconstructions/extab_census.py')
ec=importlib.util.module_from_spec(spec); spec.loader.exec_module(ec)
data,secs,entries=ec.parse('cmake-build-match/libgof2hdaa.so')
for r in entries:
    if r['fn']==0x163b90:
        d=ec.decode_lsda(data,secs,r['extab'])
        print(len(d['rows']))   # 77
```
