# Hud::draw — the `this` spill, `dividerOffset` in `sl`, and the duplicated `String::~String` (w3910)

**Status:** research note, 2026-11-03. Worktree `wt/w3910`, branch `wave89-w3910`,
base `36c65db7`. No source change landed: every variant measured at or below the
baseline (see §7), so this commit is the note only.

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` — original **9332 B**, ours **9332 B @ 88.8 %**
fuzzy. Gate on entry/exit: **byte_exact 4498 / linked_exact 4498**, size-exact
4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
lint CLEAN, drift 0, **RATCHET PASS**.

This note supersedes/extends `hud-draw-this-spill_w3834.md` and
`hud-draw-88-8-9332-b_w3814.md`. The new results are:

1. The clean source (both spurious constructs removed) is **9240 B / 68.1 %**, and its
   shield-block register allocation is already **the original's** (`this` in `r5`,
   `dividerOffset` in `sl`). The only difference there is the missing `str r5,[sp,#104]`
   and its reloads.
2. The `dividerOffset`-in-`sl` vs `this`-in-`sl` choice in the gamma block is
   **downstream** of the `this` spill, not an independent allocator decision.
3. The duplicated `String::~String` in the mission-type-12 branch is also
   **downstream**: it is a tail-merge difference caused by the `&Globals::status`
   reload placement, which in turn is downstream of the `this` spill.
4. The original keeps `&Globals::status` in `r8` across the mission panel; the clean
   build spills it to `[sp,#96]` and reloads it. This is a third downstream effect.
5. The corrected stack-slot maps (§4) show the original has **three** slots the clean
   build does not: `#60` (damage-block scratch), `#104` (`this`) and `#108`
   (`&Globals::status`); the frame difference is 8 bytes because `#60` is below the
   String temp.

## 1. Ground truth: original shield block (image base 0x10000)

```
163de6: ldr r0,[pc,#264]        ; GOT &Globals::layout
163de8: add r0,pc
163dea: ldr r0,[r0,#0]
163dec: str r0,[sp,#100]        ; &Globals::layout
163dee: ldr r1,[r0,#0]          ; Globals::layout
163df0: ldr.w r0,[r9]           ; Globals::Canvas
163df4: ldr.w sl,[r1,#488]      ; dividerOffset = layout->field_0x1e8
163df8: mov.w r1,#0xffffffff
163dfc: blx SetColor
163e00: ldrb.w r0,[r5,#543]     ; this->hasShieldBar
163e04: str r5,[sp,#104]        ; *** spill this ***
163e06: cmp r0,#0
163e08: str.w sl,[sp,#96]       ; spill dividerOffset
163e0c: beq.n 163efc
163e0e: ldr.w r0,[fp]           ; ego->player
163e12: ldr.w r6,[r9]           ; canvas
163e16: blx Player::getShieldHP
163e1a: cmp r0,#2
163e1c: blt.n 163e2e
163e1e: ldr r0,[sp,#104]        ; *** reload this ***
163e20: ldrb.w r0,[r0,#580]     ; shieldHitFlash
163e24: cbz r0,163e2e
163e26: ldr r5,[sp,#104]        ; *** reload this ***
163e28: add.w r0,r5,#680        ; &shieldFrameHitImage
163e2c: b.n 163e34
163e2e: ldr r5,[sp,#104]        ; *** reload this ***
163e30: add.w r0,r5,#676        ; &shieldFrameImage
163e34: ...
```

`this` is reloaded again at `0x164040` (gamma block, into `r6`), `0x1640ec`
(rocket block, into `r5`) and `0x16413e` (damage block, into `sl`).

## 2. Ground truth: original gamma block

```
163fae: ldrb.w r0,[r5,#544]     ; hasArmorRegen
163fb2: ldr.w sl,[sp,#96]       ; *** dividerOffset -> sl ***
163fb6: cbz r0,164008
...
164008: strd fp,r9,[sp,#116]    ; spill ego, &Globals::Canvas
16400c: ldr r0,[pc,#276]        ; GOT &Globals::status
164010: ldr r6,[r0]             ; &Globals::status
164012: ldr r4,[r6]             ; Globals::status
164016: blx Status::getStation
16401a: blx Station::getIndex
16401e: mov r5,r0
164020: ldr r0,[r6]
164022: str r6,[sp,#108]        ; spill &Globals::status
164024: blx Status::getCurrentCampaignMission
16402e: blx Status::getGammaRayDamagePerSecond
16403e: ble.n 1640e0
164040: ldr r6,[sp,#104]        ; *** reload this into r6 ***
164042: ldr r5,[sp,#120]        ; &Globals::Canvas
164044: ldrh.w r0,[r6,#1090]    ; this->field_0x442
...
16406a: add.w r3,r4,sl          ; gammaY + dividerOffset
```

So the original keeps **`dividerOffset` in `sl`** through the gamma block and reloads
`this` into `r6`. `r5` is used for `&Globals::Canvas`.

## 3. Clean source (both spurious constructs removed): 9240 B / 68.1 %

The clean source is HEAD minus exactly:

```diff
-        canvas->SetColor(savedColor);          // shield branch
...
-    int pad_ = this->field_0x470;
...
-    if (pad_ != 0)
-        Globals::Canvas->SetColor(savedColor);
```

It builds to **9240 B, frame 216, 68.1 %** (official verifier). Its shield block is
*already* the original's shape:

```
163df4: ldr.w sl,[r1,#488]      ; dividerOffset = sl
163df8: mov.w r1,#0xffffffff
163dfc: blx SetColor
163e00: ldrb.w r0,[r5,#543]     ; this->hasShieldBar
163e04: str.w sl,[sp,#92]       ; spill dividerOffset (no this spill)
163e08: cmp r0,#0
163e0a: beq.n 163ef4
163e0c: ldr.w r0,[fp]
163e10: ldr.w r6,[r9]
163e14: blx Player::getShieldHP
163e18: cmp r0,#2
163e1a: blt.n 163e28
163e1c: ldrb.w r0,[r5,#580]     ; this still in r5
163e20: cbz r0,163e28
163e22: add.w r0,r5,#680
163e26: b.n 163e2c
163e28: add.w r0,r5,#676
```

The gamma block, however, differs:

```
163faa: mov sl,r5               ; *** this -> sl ***
163fac: cbz r0,163ffe
...
164036: ldrh.w r0,[sl,#1090]    ; this->field_0x442
...
16405a: ldr r0,[sp,#92]         ; dividerOffset from stack
164060: adds r3,r4,r0           ; gammaY + dividerOffset
```

and the mission panel spills `&Globals::status`:

```
1647ea: ldr r0,[sp,#96]         ; &Globals::status
1647ec: ldr r0,[r0,#0]          ; Globals::status
```

instead of the original's `ldr.w r0,[r8]`.

## 4. Corrected stack-slot maps

Original (frame 224):

```
#60  damage-block scratch      #64  timeLeft lo   #68  timeLeft hi
#72  fireImageIndex            #76  secondaryKind #80  mining
#84  savedColor                #88  &__stack_chk_guard
#92  Globals::iPad             #96  dividerOffset
#100 &Globals::layout          #104 this          #108 &Globals::status
#112 dt                        #116 ego           #120 Canvas
#160 String temp               #220 canary
```

Clean build (frame 216):

```
#60  timeLeft lo               #64  timeLeft hi   #68  fireImageIndex
#72  secondaryKind             #76  mining        #84  savedColor
#88  &__stack_chk_guard        #92  dividerOffset
#96  &Globals::status          #100 &Globals::layout
#104 dt                        #108 ego           #112 Canvas
#152 String temp               #212 canary
```

Differences: the original has an extra `#60` scratch slot (used only at
`0x164336`/`0x16436a` in the damage block), a `#104 this` slot and a `#108
&Globals::status` slot. The clean build shares `#96` between `&Globals::status`
and (elsewhere) other values. The frame is 8 bytes larger because `#60` sits below
the String temp, while `this`/`&status` shift the high group by 8.

The HEAD build (with `pad_`) uses frame 224 but a different rotation: `pad_` occupies
`#72`, pushing `fireImageIndex/secondaryKind/mining` to `#76/#80/#84`; `savedColor`
is at `#100` and `&Globals::layout` at `#104`. That is why its 88.8 % is partly an
artifact: the frame size matches, but the low group is +4 and the high group is -4.

## 5. The duplicated `String::~String` is downstream of the spill

Original mission-type-12 branch:

```
16469c: blx PaintCanvas::DrawString
1646a0: add r0,sp,#208
1646a2: blx String::~String        <-- its own destructor
1646a6: b.w 164eba                 <-- reload &Globals::status
...
164eba: ldr.w r8,[sp,#108]
164ebe: ...
```

Time branch:

```
1649a2: blx PaintCanvas::DrawString
1649a6: add r0,sp,#208
1649a8: blx String::~String
1649ac: b.n 164ebe                 <-- skip the reload
```

The type-12 branch clobbers `r8` (`ldr.w r8,[sl,#832]` at `0x16460a`, the volatile-goods
bar image). It must reload `&Globals::status` afterwards, and the reload was placed
*after* the destructor, so the two destructors have different successors (`0x164eba`
vs `0x164ebe`) and cannot be tail-merged.

In the clean build both branches jump to the **shared** destructor at `0x16495a`
(`b.n 0x16495a`), and `r8` is reloaded much later (`0x164aa0`). So the missing call is
not an independent semantic error: it is a consequence of the different `r8` live
range, which is itself a consequence of the `this` spill.

## 6. The `this` spill is a greedy-allocator live-range split

Machine IR of the clean build (`-mllvm -print-machineinstrs`, "After Greedy Register
Allocator") shows the allocator inserting a copy of `this` in both predecessors of the
hasShieldBar merge and using it in the merge block:

```
bb.33 (then):   %2926:gprnopc = COPY %2925:gprnopc   ; %2925 = this
bb.34 (else):   %2926:gprnopc = COPY %2925:gprnopc
bb.35 (merge):  ... uses %2926 ...
```

In the clean build the copy coalesces back to `$r5` (the "After Virtual Register
Rewriter" dump has no copy and uses `$r5` in bb.33/bb.35). In the original the copy is
spilled: `str r5,[sp,#104]` in the predecessor and `ldr r5,[sp,#104]` in the merge.

The original's `this` is therefore on the stack for the rest of the function, which is
what lets the gamma block reload it into a *different* register (`r6`) and leave `sl`
for `dividerOffset`, and what lets `&Globals::status` stay in `r8` in the mission
panel. All of the remaining deltas cascade from this one allocation decision.

## 7. Variants measured (official verifier, `autojunk=False`)

Baseline (HEAD, both spurious constructs): **9332 B, 88.8 %, gate byte 4498**.

Clean source (both removed): **9240 B, 68.1 %, gate byte 2944** (the `.text` shrink
shifts every later symbol, so this is a gate regression and cannot be landed without
also restoring the size). Removing only one construct:

| variant | size | fuzzy | note |
|---|---|---|---|
| HEAD (both) | 9332 | 88.8 % | current |
| remove shield `SetColor` (keep `pad_`) | 9324 | 83.9 % | gate byte 4031 |
| remove `pad_` (keep shield `SetColor`) | 9252 | 68.1 % | |
| remove both | 9240 | 68.1 % | gate byte 2944 |

On the clean source, all of the following stayed at 9240 B with no `str r5` in the
shield block (i.e. the copy still coalesced):

| construct | result |
|---|---|
| `Hud *self = this` at top / after `savedColor` / before shield / inside shield | 9240 |
| `Hud *self = this` used only in shield image / only gamma / shield+gamma / gamma+rocket+damage | 9240 |
| `Hud *const self`, `Hud &self`, `Hud **selfPtr`, `(Hud*)((char*)this)`, `&this[0]` | 9240 |
| `Hud *self` used in shield image and `this` in the same call | 9240 |
| `Hud *self` at top used in the whole function | 9240 |
| hoist `PaintCanvas *canvas` before the `if` | 9236 |
| use `canvas` for all shield calls | 9240 |
| `unsigned int *shieldImage` + `*shieldImage` | 9240 |
| ternary `shieldImage = cond ? a : b` | 9240 |
| ternary `armorY`/`armorBarY` | 9148 |
| `unsigned short *armorY`/`armorBarY` | 9232 |
| move `dividerOffset` after `SetColor` / after the armor declarations | 9236 |
| `int dividerOffsetCopy = dividerOffset` used in gamma | 9240 |
| cache `Status *st = Globals::status` in the mission panel | 9252 |
| cache `Layout *layout = Globals::layout` at top | 9208 |
| cache `PaintCanvas *canvas = Globals::Canvas` at top | 9012 |
| `__builtin_expect(this->hasShieldBar, 0)` | 9328 |
| `__builtin_expect(this->hasShieldBar, 1)` | 9240 |
| `__builtin_expect(gamma > 0.0f, 0)` | **9332** (56.8 %) |
| `__builtin_expect(gamma > 0.0f, 1)` | 9272 |
| `__builtin_expect(shield-image cond, 0/1)` | 9244 / 9248 |

`__builtin_expect(gamma > 0.0f, 0)` is the only variant that restores the 9332-byte
frame without the spurious constructs, but it changes the block layout (gamma becomes
cold), moves `this` to `r4`, `dividerOffset` to `fp`, and scores only 56.8 %.

`-O2` / `-Os` / `-O1` on the clean source give 9744 / 9448 / 9392 B, so `-Oz` is
confirmed.

## 8. What remains

The single missing piece is the source construct that makes clang 7.0.2's greedy
allocator spill the `%2926 = COPY %2925` at the hasShieldBar merge. Every natural
source copy of `this` is coalesced. The most promising untried direction is a
construct that raises the register pressure *after* the merge (the gamma block) enough
that the split copy is assigned the stack instead of `r5`; `pad_` did this only
indirectly (it kept the frame at 224 and the allocation in `r5`/`sl`, but did not
spill `this`). Once the spill is reproduced, the gamma block, the `&Globals::status`
`r8` live range, the duplicated destructor and the stack-slot rotation should all
fall into place, and the function should become byte-exact.

## 9. Reproduce

```
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w3910/vmeas \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

(`FN=... verify-fn` is broken: it passes an empty `FN` to `--show`.)
