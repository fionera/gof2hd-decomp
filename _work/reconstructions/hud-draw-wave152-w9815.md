# Hud::draw — wave 152 (w9815): the 224-byte frame is the `sl` live range, not the `Globals::w` spill

**Status:** analysis-only, 2026-11-08 (wave 152). Worktree `wt/w9815`,
base `master 9d491d46a` (wave 151). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **1325** (no source landed) |
| aligned (instruction-index) diff | **249** | **249** |
| size | 9332 | 9332 |
| frame | 224 | 224 |

Gate: `check` green (`byte_exact 4507`, `linked_exact 4507`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).

---

## 0. TL;DR

1. **The w9715 premise is falsified.** The extra 8 bytes of frame (224 vs 216)
   are **not** the `Globals::w` spill slot.  `base + pivotY=0` already spills
   `Globals::w` to `[sp,#56]` and keeps `Globals::h` in `r9`, exactly like the
   original's `[sp,#60]` / `r9` (the w9715 note had w/h swapped: the original
   stores **w** at `[sp,#60]`, keeps **h** in `r9`; see the relocation check in
   §2).  The frame still drops to 216.
2. **The frame is the `sl` live range.** With the `pivotY = dividerOffset`
   anchor, `dividerOffset` stays live in `sl` through the armour block, so
   `this` cannot be coloured into `sl`: it stays in `r5`, is spilled to
   `[sp,#104]` and reloaded at the original's five sites.  Without the anchor
   the allocator emits `mov sl, r5` at `0x163faa` and uses `sl` for `this` in
   the armour-regen block; `dividerOffset` goes to `[sp,#88]`.  That single
   colouring change re-lays out the whole 224-byte frame (high slots shift by
   8, low slots by 4) and cascades into the dock2/field_0x4b8 regions.
3. **No source shape found that keeps the 224 frame with `pivotY = 0`.**  All
   new hypotheses (below) either fold the anchor away (9312 B, frame 216) or
   make the codegen worse.  The anchor remains the unique 1325/249 shape.

## 1. Measured variants (full build; base = 1325 / 249 / 9332 / 224)

| variant | size | raw | align | frame |
|---|---:|---:|---:|---:|
| **base (landed)** | **9332** | **1325** | **249** | **224** |
| base + `pivotY=0` | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=dividerOffset*0` | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=dividerOffset&0` | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=dividerOffset-dividerOffset` | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=(dividerOffset,0)` | 9312 | 8261 | 1009 | 216 |
| base + `pivotX=dividerOffset`, `pivotY=0` | 9336 | 7312 | 454 | 224 |
| base + `pivotY=0`, `const dividerOffset` | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=0`, `unsigned dividerOffset` | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=0`, `int zero=0` hoisted, `pivotX=pivotY=zero` | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=0`, `int globW=Globals::w` hoisted | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=0`, `int globH=Globals::h` hoisted | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=0`, `int divider2=dividerOffset` in gamma | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=0`, `PaintCanvas *canvas` in regen block | 9312 | 8261 | 1009 | 216 |
| base + `pivotY=0`, `PaintCanvas *canvas` for all shield calls | 9312 | 8275 | 1042 | 216 |
| base + `pivotY=0`, `PaintCanvas *canvas` for all armour calls | 9280 | 8845 | 2080 | 216 |
| base + `pivotY=0`, `PaintCanvas *canvas` for shield+armour+gamma | 9144 | 8764 | 2378 | 216 |
| base + `pivotY=0`, `PaintCanvas *canvas` for whole function | 8956 | 8658 | 2143 | 216 |
| base + `pivotY=0`, `PaintCanvas *canvas` in gamma block | 9372 | 8197 | 1183 | 216 |
| base + `pivotY=0`, `__builtin_expect(hasArmorRegen,1)` | 9320 | 8110 | 1051 | 216 |
| base + `pivotY=0`, `__builtin_expect(hasArmorRegen,0)` | 9336 | 8331 | 1414 | 216 |
| base + `pivotY=0`, `dividerOffset` declared after SetColor | 9308 | 8306 | 1055 | 216 |
| base + anchor moved to hull-bar `pivotY` | 9328 | 7456 | 1028 | 216 |
| base + anchor moved to shield-bar `pivotY` | 9324 | 8134 | 820 | 216 |
| base + anchor moved to gamma-bar `pivotY` | 9324 | 8091 | 1018 | 216 |
| base + `pivotY=0`, dead `(void) Globals::status;` before 4th block | 9244 | 8303 | 1361 | 216 |
| base + `pivotY=0`, `Status *volatile st = Globals::status;` | 9312 | 8250 | 924 | 216 |
| base + `pivotY=0`, nested `if (Globals::status) { if (4b8) ... }` | 9312 | 8261 | 1009 | 216 |

## 2. Why the anchor changes the frame

Original armour block (relocation-verified):

```
163f2e: ldr.w r8,[r0]          ; r8 = DrawImage2D fn ptr
163f36: ldr r0,[sp,#96]        ; reload dividerOffset
163f3c: adds r3,r0,r6          ; dividerOffset + armorY
163f46: mov r6,sl              ; r6 = armorBarY (sl now free)
163faa: blx DrawRegion2D        ; hull bar, pivotY=0
163fae: ldrb r0,[r5,#544]      ; this->hasArmorRegen (this still in r5)
163fb2: ldr.w sl,[sp,#96]      ; sl = dividerOffset
163ff4: strd r4,r4,[sp,#12]    ; pivotX=0, pivotY=0
```

`base + pivotY=0` instead:

```
163faa: mov sl,r5              ; sl = this
163fae: ldrb r0,[r5,#544]
163fbe: ldrh r0,[sl,#1094]     ; this via sl in the regen block
163ffe: strd fp,r9,[sp,#108]   ; ego/Canvas spill 8 B lower
```

The `Globals::w` spill is present in **both** shapes: original `str r0,[sp,#60]`
at `0x164336` (the literal at `0x164520` resolves through `.rel.dyn` to
`_ZN7Globals1wE` at GOT `0x210278`; the `0x16451c` literal is `_ZN7Globals1hE`
at `0x210280`), base `str r0,[sp,#56]` at `0x164324`.  So the spill is not the
frame lever.

Stack-slot census (`objdump` `[sp,#N]` operand histogram):

```
orig : 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 160 220
base : 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 160 220   (identical)
bp0  : 56 60 64 68 72 76 80 84 88 92  96 100 104 108 112 152 212      (one slot fewer)
```

The base's frame layout is byte-for-byte the original's; only the usage counts
of `[sp,#92]`/`[sp,#108]`/`[sp,#120]` differ (6/22/39 vs 10/23/38).

## 3. Negative results / new hypotheses tested

* `pivotY` expressions that fold to 0 (`*0`, `&0`, `-self`, `^self`, comma)
  all lose the anchor and reproduce `base+pivotY=0` exactly.
* `const`/`unsigned` `dividerOffset`, `int zero` locals, hoisted
  `Globals::w`/`Globals::h` locals, `int divider2` alias: all optimised away
  (identical code to `base+pivotY=0`).
* `PaintCanvas *canvas` locals (shield only, armour only, shield+armour+gamma,
  whole function, gamma only): all re-colour the receiver and regress
  (8956–9372 B, raw 8197–8845, align 1042–2378).
* `__builtin_expect(hasArmorRegen, 0/1)`: 9320–9336 B, raw 8110–8331,
  align 1051–1414.
* Moving the anchor to `pivotX` recovers the 224 frame (9336 B) but costs an
  extra instruction (`strd sl,r4` vs `strd r4,r4`) and the aligned diff jumps
  to 454; moving it to the hull/shield/gamma bar instead of the regen bar
  loses the frame (9324–9328 B, frame 216).

## 4. Open questions

1. The frame is a whole-function RA consequence of `sl = dividerOffset`.  A
   faithful `pivotY=0` shape needs a *different* value to occupy `sl` through
   the armour block.  The only source lever found so far is the anchor itself.
2. The original's dead `ldr.w r8,[sp,#108]` loads at `0x164176`/`0x164210`
   (`Globals::status`) suggest the original source *did* reference
   `Globals::status` in the field_0x4b8 region but the null test was optimised
   away after the earlier gamma-block dereference.  Our compiler keeps the
   test; forcing it to drop the test without changing the frame is still open.
3. `pivotX=dividerOffset` (9336/7312/454) is the only variant that keeps the
   224 frame with a zero `pivotY`; it is 1 instruction off but the aligned
   metric is 205 worse, so it was not landed.

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9815
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9815/measure.py
python3 /dev/shm/w9815/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w9815`: `measure.py` (size/raw/frame),
`align_diff.py`, `slots.py` (`[sp,#N]` histogram), `dis.py`, `diffins.py`,
`run.sh` + `v*.cpp` (variant sources).
