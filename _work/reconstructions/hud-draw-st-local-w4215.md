# Hud::draw — the `st` local, the shield `this` spill, and the stack-slot-coloring rotation (w4215)

**Status:** research note, 2026-11-03. Worktree `wt/w4215`, branch `wave92-w4215`,
base `b379d00d`. **No source change landed**: no tested faithful variant is
byte-exact, and the only variants that reproduce the original's shield-block
shape are gate-neutral or worse on the fuzzy metric. This commit is the note
only.

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` — original **9332 B**, ours **9332 B @
88.8 %** fuzzy. Gate on entry/exit (full `check`): **byte_exact 4498 /
linked_exact 4498**, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, lint CLEAN, drift 0, **RATCHET PASS**.

This note extends `hud-draw-slot-coloring-w4114.md`,
`hud-draw-slot-rotation-w4014.md`, `hud-draw-this-spill-w3910.md` and
`hud-draw-this-spill_w3834.md`. The new results are:

1. The clean source (HEAD minus the shield `SetColor(savedColor)` anchor and the
   `pad_` anchor) is **9240 B / 68.1 %**. Adding one faithful local,
   `Status *st = Globals::status;` immediately before the gamma `if` and using
   `st` for the three gamma calls (`st` variant), forces the original's
   **branch-local `this` spill** and reproduces the original's shield block
   byte-for-byte **except for the stack-slot base**:
   `ldrb.w r0,[r5,#543]; str r5,[sp,#108]; cmp r0,#0; str.w sl,[sp,#100]; beq`
   vs the original `...[sp,#104] ... [sp,#96] ...`.
2. The `st` variant is **9264 B / 83.3 %**; its low group (`#64` timeLeft lo,
   `#68` hi, `#72` fireImageIndex, `#76` secondaryKind, `#80` mining, `#84`
   savedColor, `#88` guard) is **exactly the original's**. Only the four
   high-group slots are cyclically rotated:
   `st`: div `#100`, layout `#104`, this `#108`, status `#96`;
   original: div `#96`, layout `#100`, this `#104`, status `#108`.
3. MIR shows the rotation is a **stack-slot-coloring permutation**, not a
   source-semantics difference. In the `st` build the GRA spill slots are
   `%stack.49` (layout), `%stack.59` (this), `%stack.63` (div) and `%stack.50`
   (status address); after Stack Slot Coloring the four values are assigned
   colours `%stack.48/%stack.47/%stack.49/%stack.50` and the frame layout maps
   descending colour index to ascending offset, producing the rotation.
4. The original's colouring is what you get when the status spill slot is
   merged with the early **iPad `Globals::globals` slot** (the earliest-starting
   of the four reused slots). In the `st` build the status slot starts at the
   gamma block (start `9360B`) and is therefore coloured last (highest colour,
   lowest offset); the three shield-block values take the three reused iPad
   slots.
5. A `top` variant (`Status *gstatus = Globals::status;` at the top, gamma
   uses it) is **9332 B / 70.8 %**: it hoists the status load to the function
   top and spills the *value* to `#96`, and in this worktree it does **not**
   reproduce the exact high-group assignment claimed in the w4114 note
   (measured here: layout `#108`, div `#104`, status `#96`, `this` stays in
   `r5`). It is not the original's source shape.
6. No faithful variant raises the fuzzy score above the HEAD 88.8 %; HEAD's
   88.8 % is partly an artifact of the two anchors (they reproduce the frame
   size and keep `dividerOffset`/`status` at `#96`/`#108`).

## 1. Ground truth: original shield block (image base 0x10000)

```
163de6: ldr   r0, [pc, #264]        ; &Globals::layout
163de8: add   r0, pc
163dea: ldr   r0, [r0, #0]
163dec: str   r0, [sp, #100]        ; &Globals::layout -> #100
163dee: ldr   r1, [r0, #0]          ; Globals::layout
163df0: ldr.w r0, [r9]              ; Globals::Canvas
163df4: ldr.w sl, [r1, #488]        ; dividerOffset -> sl
163df8: mov.w r1, #0xffffffff
163dfc: blx   PaintCanvas::SetColor
163e00: ldrb.w r0, [r5, #543]       ; this->hasShieldBar
163e04: str   r5, [sp, #104]        ; *** spill this ***
163e06: cmp   r0, #0
163e08: str.w sl, [sp, #96]         ; spill dividerOffset
163e0c: beq.n 163efc
163e0e: ldr.w r0, [fp]              ; ego->player
163e12: ldr.w r6, [r9]              ; Canvas
163e16: blx   Player::getShieldHP
163e1a: cmp   r0, #2
163e1c: blt.n 163e2e
163e1e: ldr   r0, [sp, #104]        ; *** reload this ***
163e20: ldrb.w r0, [r0, #580]       ; shieldHitFlash
163e24: cbz   r0, 163e2e
163e26: ldr   r5, [sp, #104]        ; *** reload this ***
163e28: add.w r0, r5, #680          ; &shieldFrameHitImage
163e2c: b.n   163e34
163e2e: ldr   r5, [sp, #104]        ; *** reload this ***
163e30: add.w r0, r5, #676          ; &shieldFrameImage
```

The merge block (`0x163efc`) uses `r5` directly (`addw r0,r5,#1090`), so the
spill is a branch-local split; `this` is reloaded later at `0x164040` (into
`r6`), `0x1640ec` (into `r5`) and `0x16413e` (into `sl`).

Original slot map (frame 224):

```
#60  damage scratch   #64  timeLeft lo   #68  timeLeft hi   #72  fireImageIndex
#76  secondaryKind    #80  mining        #84  savedColor    #88  &guard
#92  iPad             #96  dividerOffset #100 &Globals::layout
#104 this             #108 &Globals::status
#112 dt               #116 ego           #120 &Globals::Canvas
#160 String temp      #220 canary
```

## 2. The `st` variant: clean + `Status *st` at gamma

Source shape (exactly the clean source plus one local):

```cpp
    int dividerOffset = Globals::layout->field_0x1e8;
    Globals::Canvas->SetColor(0xffffffff);
    ...
    Status *st = Globals::status;
    if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                       st->getCurrentCampaignMission()) > 0.0f) {
        ...
    }
```

Measured with the official verifier, `autojunk=False`:
**size 9264 B, fuzzy 83.3 %** (HEAD 9332 B, 88.8 %).

Its shield block is the original's shape with the slot base shifted by +4:

```
163de6: ldr   r0, [pc, #264]
163de8: add   r0, pc
163dea: ldr   r0, [r0, #0]
163dec: str   r0, [sp, #104]        ; layout -> #104  (orig #100)
163dee: ldr   r1, [r0, #0]
163df0: ldr.w r0, [r9]
163df4: ldr.w sl, [r1, #488]
163df8: mov.w r1, #0xffffffff
163dfc: blx   PaintCanvas::SetColor
163e00: ldrb.w r0, [r5, #543]
163e04: str   r5, [sp, #108]        ; *** this -> #108 (orig #104) ***
163e06: cmp   r0, #0
163e08: str.w sl, [sp, #100]        ; div -> #100  (orig #96)
163e0c: beq.n 163efc
163e0e: ldr.w r0, [fp]
163e12: ldr.w r6, [r9]
163e16: blx   Player::getShieldHP
163e1a: cmp   r0, #2
163e1c: blt.n 163e2e
163e1e: ldr   r0, [sp, #108]        ; reload this
163e20: ldrb.w r0, [r0, #580]
163e24: cbz   r0, 163e2e
163e26: ldr   r5, [sp, #108]        ; reload this
163e28: add.w r0, r5, #680
163e2c: b.n   163e34
163e2e: ldr   r5, [sp, #108]        ; reload this
163e30: add.w r0, r5, #676
```

The instruction sequence, registers (`r5` = `this`, `sl` = `dividerOffset`,
`r6` = canvas) and branch shape are identical to the original; only the four
high-group offsets differ. The low group is exact:

```
163bba: str r1, [sp, #68]   ; timeLeft hi
163bbe: str r1, [sp, #64]   ; timeLeft lo
163bc2: str r1, [sp, #72]   ; fireImageIndex
163bc6: str r1, [sp, #76]   ; secondaryKind
163bca: str r0, [sp, #80]   ; mining
163ba8: str r0, [sp, #88]   ; &guard
```

Gamma block of the `st` variant (status address spilled to `#96`, value kept in
`r4` across the three calls):

```
164008: strd  fp, r9, [sp, #116]
16400c: ldr   r0, [pc, #276]        ; &Globals::status
16400e: add   r0, pc
164010: ldr   r0, [r0, #0]
164012: str   r0, [sp, #96]         ; &Globals::status -> #96 (orig #108)
164014: ldr   r4, [r0, #0]           ; Globals::status
164016: mov   r0, r4
164018: blx   Status::getStation
16401c: blx   Station::getIndex
164020: mov   r5, r0
164022: mov   r0, r4
164024: blx   Status::getCurrentCampaignMission
164028: mov   r2, r0
16402a: mov   r0, r4
16402c: mov   r1, r5
16402e: blx   Status::getGammaRayDamagePerSecond
```

## 3. MIR evidence: the rotation is stack-slot colouring

`-mllvm -print-machineinstrs` on the `st` build (Hud.cpp only). After the
Greedy Register Allocator, block `bb.28` (the shield block) contains the
original's branch-local split:

```
%2925:gprnopc = COPY %2924:gprnopc          ; this copy
t2STRi12 %2925:gprnopc, %stack.59           ; *** spill this ***
t2STRi12 %2919:gprnopc, %stack.63           ; spill dividerOffset
```

and the layout pointer is `%stack.49`; the gamma block spills the status
address to `%stack.50`:

```
t2STRi12 %2843:gpr, %stack.49               ; &Globals::layout
...
t2STRi12 %2871:gpr, %stack.50               ; &Globals::status
```

After **Stack Slot Coloring** the four slots are renumbered (colours) and the
iPad temporaries are merged into three of them:

```
bb.7 (iPad):   %stack.47 = width(this->image_0x31c)
               %stack.48 = width(this->image_0x310)
               %stack.49 = width(this->fuelGaugeBarImage)
bb.28:         $r5  -> %stack.47            ; this reuses width1
               $r10 -> %stack.49            ; div reuses width3
               (layout reuses %stack.48 = width2)
bb.37:         $r0  -> %stack.50            ; status gets the new slot
```

The frame layout maps descending colour index to ascending offset, so
`%stack.50 -> #96`, `%stack.49 -> #100`, `%stack.48 -> #104`,
`%stack.47 -> #108`, i.e. status `#96`, div `#100`, layout `#104`, this `#108`.

The original needs the *other* merge: status must reuse the earliest iPad slot
(`Globals::globals`, stored at `#108` at `0x163c2c`, live until the
`setCoordsFire` call), so that the four values are coloured
`div > layout > this > status` in colour index and land at
`#96/#100/#104/#108`. In the `st` build the status spill slot starts at the
gamma block (`9360B`), later than the three shield-block slots, so it is
coloured last (highest colour, lowest offset).

The `top` variant (`Status *gstatus = Globals::status;` before `int dt`)
creates the status slot before the iPad block, which is why it reaches the
correct rotation — but it also hoists the load and spills the value:

```
163b9e: str   r2, [sp, #116]        ; dt
163ba2: ldr   r0, [pc, #816]        ; &__stack_chk_guard
163ba4: ldr   r1, [pc, #816]        ; &Globals::status
163ba6: add   r0, pc
163ba8: add   r1, pc
163baa: ldr   r0, [r0, #0]
163bac: ldr   r2, [r1, #0]          ; &Globals::status
163bae: str   r0, [sp, #92]
163bb0: ldr   r1, [r0, #0]
163bb2: ldr   r0, [r2, #0]          ; Globals::status (value)
163bb4: str   r2, [sp, #112]        ; spill address
163bb6: str   r0, [sp, #96]         ; spill value
```

`top` is **9332 B / 70.8 %** — the right size, but the wrong entry and gamma
codegen. Measured slot map: dt `#116`, guard `#92`, ego `#120`, layout `#108`,
div `#104`, status `#96`; `this` is not spilled. (The w4114 note reports a
different `top` allocation with `this` in `r4` and `this=#104`; that exact
allocation was not reproduced here.)

## 4. Variants measured (official verifier, `autojunk=False`)

| variant | size | fuzzy | notes |
|---|---|---|---|
| HEAD (baseline) | 9332 | 88.8 % | `pad_` + shield `SetColor(savedColor)` anchors |
| clean (both anchors removed) | 9240 | 68.1 % | no `this` slot; frame 216 |
| clean + `st` at gamma (`st`) | 9264 | 83.3 % | exact low group, shield shape, +4 rotation |
| clean + `st` + `Hud *self` in shield | 9264 | 83.3 % | same rotation |
| clean + `st` + `Layout *lay` in shield | 9264 | 83.3 % | same rotation |
| clean + `st` + `PaintCanvas *pc` in shield | 9260 | 78.8 % | worse |
| clean + `st` declared at top | 9332 | 70.8 % | `top`-like, hoists load |
| clean + `gstatus` before `int dt` (`top`) | 9332 | 70.8 % | wrong entry; layout #108, div #104, status #96 |
| clean + `gstatus` before `dividerOffset` | 9292 | 71.0 % | same rotation |
| clean + `int gammaMission = ...` | 9264 | 83.1 % | same rotation |
| HEAD + `st` | 9264 | 83.3 % | same rotation |
| HEAD + shield `SetColor(0xffffffff)` | 9240 | 68.1 % | anchor only helps with `savedColor` |
| HEAD + second `field_0x470` read | 9240 | 68.1 % | worse |
| clean + `st` + use `st` in mission panel | 9152 | 72.0 % | mission panel codegen changes |
| clean + `st` + use `st` for first `getMission` | 9336 | 61.6 % | 4 B too big |

The `st` variant's 83.3 % is lower than HEAD's 88.8 % because HEAD's two
anchors happen to keep `dividerOffset` at `#96` and `status` at `#108` and
reproduce the 224-byte frame; the `st` variant rotates all four high-group
slots. No tested variant exceeds 88.8 %.

## 5. What remains

The remaining gap is **not** source semantics: the `st` variant proves the
original's shield-block branch-local `this` spill is reachable with a faithful
one-line local, and it reproduces the exact low group, the exact shield
instruction/register shape, and the exact `dividerOffset`-in-`sl` /
`this`-in-`r5` assignment. The gap is the **stack-slot-colouring permutation**:
the original merges the status spill slot with the early iPad
`Globals::globals` slot, while the `st` build leaves it as a new slot created at
the gamma block. A source shape that creates the status spill slot before the
iPad block *without* hoisting the status load (or that otherwise makes the
status slot the earliest-starting of the four reused slots) would reproduce the
original's `#96/#100/#104/#108` assignment and should be byte-exact or close.
The `top` variant shows the colouring target is reachable; the open problem is
keeping the load at the gamma block.

## 6. Reproduction

```
cmake --preset default
cmake --build cmake-build-match --target check -j8          # 4498/4498
# apply the st variant to src/game/ui/Hud.cpp, then:
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w4215/vmeas \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

MIR: `ninja -C cmake-build-match -t commands CMakeFiles/gof2.dir/src/game/ui/Hud.cpp.obj | tail -1`
with `-mllvm -print-machineinstrs`, then read the
`After Greedy Register Allocator` and `After Stack Slot Coloring` stages.
