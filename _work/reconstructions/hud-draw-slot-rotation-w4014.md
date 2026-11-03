# Hud::draw — the `this` spill and the stack-slot rotation (w4014)

**Status:** research note, 2026-11-03. Worktree `wt/w4014`, branch `wave90-w4014`,
base `360c7467`. No source change landed: no tested variant is byte-exact and none
raises `byte_exact`, so this commit is the note only.

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` — original **9332 B**, ours **9332 B @
88.8 %** fuzzy. Gate on entry/exit: **byte_exact 4498 / linked_exact 4498**,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
lint CLEAN, drift 0, **RATCHET PASS**.

This note extends `hud-draw-this-spill-w3910.md` and
`hud-draw-this-spill_w3834.md`. The new results are:

1. A source variant with a single `Status *gstatus = Globals::status;` used only
   for the `getCurrentCampaignMission()` argument of the gamma condition
   (`gamma_gamma`) reproduces the original's **exact stack-slot set and exact
   per-slot use counts** — but the values are cyclically rotated by one slot.
2. The rotation is caused by the Virtual Register Rewriter coalescing the
   `this` copy differently: both clean and `gamma_gamma` spill `this` to
   `%stack.59` in the Greedy Register Allocator, but clean's VRR coalesces the
   copy back to `$r5` (and stores `dividerOffset` to `%stack.59` instead), while
   `gamma_gamma`'s VRR keeps `$r5` in `%stack.59` and puts `dividerOffset` in
   `%stack.63`.
3. The original's `#108` slot is created early in the iPad-coordinate block and
   reused for `&Globals::status`; in `gamma_gamma` the same early slot is reused
   for `this` and `&Globals::status` gets a new slot (`#96`). The original's
   three early iPad slots (`#108/#104/#100`) are reused by
   `status/this/layout`; `gamma_gamma`'s are reused by `this/layout/div`.
4. Every variant that forces the `this` spill also produces the rotation
   `this=#108, div=#100, layout=#104, status=#96` (or a nearby variant), never
   the original `this=#104, div=#96, layout=#100, status=#108`.

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
```

`this` is reloaded again at `0x164040` (gamma block, into `r6`), `0x1640ec`
(rocket block, into `r5`) and `0x16413e` (damage block, into `sl`).

## 2. Ground truth: original gamma block

```
164008: strd fp,r9,[sp,#116]    ; spill ego, &Globals::Canvas
16400c: ldr r0,[pc,#276]        ; GOT &Globals::status
164010: ldr r6,[r0]             ; r6 = &Globals::status
164012: ldr r4,[r6]             ; Globals::status
164016: blx Status::getStation
16401a: blx Station::getIndex
16401e: mov r5,r0
164020: ldr r0,[r6]             ; Globals::status
164022: str r6,[sp,#108]        ; *** spill &Globals::status ***
164024: blx Status::getCurrentCampaignMission
16402e: blx Status::getGammaRayDamagePerSecond
16403e: ble.n 1640e0
164040: ldr r6,[sp,#104]        ; *** reload this into r6 ***
164042: ldr r5,[sp,#120]        ; &Globals::Canvas
164044: ldrh.w r0,[r6,#1090]    ; this->field_0x442
...
16406a: add.w r3,r4,sl          ; gammaY + dividerOffset
```

The original keeps `dividerOffset` in `sl` through the gamma block and reloads
`this` into `r6`.

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

Its shield block is already the original's register shape, except that the
`this` copy coalesces to `$r5` and no `str r5,[sp,#104]` is emitted:

```
ldrb.w r0,[r5,#543]
str.w  sl,[sp,#92]        ; dividerOffset only
cmp    r0,#0
beq.n  ...
ldr.w  r0,[fp]
ldr.w  r6,[r9]
blx    Player::getShieldHP
...
ldrb.w r0,[r5,#580]       ; this still in r5
```

Clean's gamma block is already the original's shape:

```
ldr r0,[pc,#272]
add r0,pc
ldr r6,[r0]               ; r6 = &Globals::status
ldr r4,[r6]               ; Globals::status
mov r0,r4
blx Status::getStation
blx Station::getIndex
mov r5,r0
ldr r0,[r6]
str r6,[sp,#96]           ; spill &Globals::status (slot differs)
blx Status::getCurrentCampaignMission
```

## 4. The `gamma_gamma` variant

Source delta on top of clean (only the second gamma argument uses the local):

```cpp
    Status *gstatus = Globals::status;
    if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                    gstatus->getCurrentCampaignMission()) > 0.0f) {
```

Official verifier: **9332 B, 83.3 %** (`osize=9332 usize=9264`). Its shield
block is exactly the original's shape:

```
ldrb.w r0,[r5,#543]
str    r5,[sp,#108]       ; *** spill this ***
cmp    r0,#0
str.w  sl,[sp,#100]       ; spill dividerOffset
beq.n  ...
...
ldr    r0,[sp,#108]       ; reload this
ldrb.w r0,[r0,#580]
```

Its gamma block is clean's gamma block, with `&Globals::status` spilled to
`#96`:

```
ldr r0,[r0,#0]
str r0,[sp,#96]           ; &Globals::status
ldr r4,[r0,#0]
...
```

The variant's stack-slot **use counts** are identical to the original's:

| offset | original | gamma_gamma |
|---|---|---|
| #60 | 2 | 2 |
| #64 | 5 | 5 |
| #68 | 10 | 10 |
| #72 | 4 | 4 |
| #76 | 5 | 5 |
| #80 | 6 | 6 |
| #84 | 3 | 3 |
| #88 | 3 | 3 |
| #92 | 9 | 9 |
| #96 | 16 | 16 |
| #100 | 28 | 28 |
| #104 | 26 | 26 |
| #108 | 9 | 9 |
| #112 | 16 | 16 |
| #116 | 9 | 9 |
| #120 | 36 | 36 |
| #160 | 2 | 2 |
| #220 | 3 | 3 |

Only the identities are rotated:

| value | original slot | gamma_gamma slot |
|---|---|---|
| `dividerOffset` | #96 | #100 |
| `&Globals::layout` | #100 | #104 |
| `this` | #104 | #108 |
| `&Globals::status` | #108 | #96 |

This is a cyclic shift by one position in the order
`[div, layout, this, status]` → `[status, div, layout, this]`.

## 5. MIR evidence: the GRA spill is identical, the VRR coalescing differs

Both clean and `gamma_gamma` have the same Greedy Register Allocator output at
the hasShieldBar merge. From the `After Greedy Register Allocator` dump of
`Hud::draw` (clean: `mir_clean_gra.txt`; gamma_gamma: `gg_gra.txt`):

```
bb.28 (%ir-block.225):
  %2925:gprnopc = COPY %2924:gprnopc       ; this
  %2919:gprnopc = t2LDRi12 %543, 488       ; dividerOffset
  ...
  t2STRi12 %2925:gprnopc, %stack.59        ; spill this
  t2STRi12 %2919:gprnopc, %stack.63        ; spill dividerOffset
  t2Bcc %bb.34, 0, killed $cpsr
```

The Virtual Register Rewriter then diverges.

**Clean** (`clean_vrr.txt`, line 556):

```
t2STRi12 renamable $r10, %stack.59         ; dividerOffset, NOT this
t2Bcc %bb.34, 0, killed $cpsr
```

The `this` copy coalesces to `$r5`; the `%stack.59` store is re-used for
`dividerOffset`. The final PEI has `str r10,[sp,#92]` and no `this` store.

**gamma_gamma** (`gg_vrr.txt`, lines 557–558):

```
t2STRi12 renamable $r5, %stack.59          ; this
t2STRi12 renamable $r10, %stack.63         ; dividerOffset
t2Bcc %bb.34, 0, killed $cpsr
```

The `this` copy does not coalesce; the final PEI has
`str r5,[sp,#108]` and `str r10,[sp,#100]`.

So the missing source shape is the one that makes the VRR keep the
`this` copy in a stack slot instead of coalescing it to `$r5`. The GRA already
creates the split; the VRR is the pass that decides whether it survives.

## 6. Stack-slot creation order and the early iPad slots

The PEI frame-object order (high offset → low offset) is the stack-slot
creation order. For clean (`mir_clean_pei.txt`):

```
stack.44 -> 112  Canvas
stack.45 -> 108  ego
stack.46 -> 104  dt
stack.47 -> 100  &Globals::layout
stack.48 ->  96  &Globals::status
stack.49 ->  92  dividerOffset
stack.50 ->  88  iPad
stack.51 ->  84  guard
stack.52 ->  80  savedColor
stack.53 ->  76  mining
stack.54 ->  72  secondaryKind
stack.55 ->  68  fireImageIndex
stack.56 ->  64  timeLeft hi
stack.57 ->  60  timeLeft lo
stack.58 ->  56  frontImage
```

For `gamma_gamma` (`gg_pei.txt`):

```
stack.44 -> 120  Canvas
stack.45 -> 116  ego
stack.46 -> 112  dt
stack.47 -> 108  this
stack.48 -> 104  &Globals::layout
stack.49 -> 100  dividerOffset
stack.50 ->  96  &Globals::status
stack.51 ->  92  iPad
stack.52 ->  88  guard
stack.53 ->  84  savedColor
stack.54 ->  80  mining
stack.55 ->  76  secondaryKind
stack.56 ->  72  fireImageIndex
stack.57 ->  68  timeLeft hi
stack.58 ->  64  timeLeft lo
stack.59 ->  60  frontImage
```

The original's order is the same as `gamma_gamma` except that
`&Globals::status` is `stack.47` (before `this`), `this` is `stack.48`,
`&Globals::layout` is `stack.49` and `dividerOffset` is `stack.50`:

```
status -> 108, this -> 104, layout -> 100, div -> 96
```

The early `#108` slot is created in the iPad-coordinate block. Original
(`0x163c2c`):

```
163c20: str r0,[sp,#120]        ; Globals::globals
163c2c: str r0,[sp,#108]        ; first width / globals value
...
163c50: str r0,[sp,#104]        ; GetImage2DWidth result
...
163c5c: str r0,[sp,#100]        ; GetImage2DWidth result
```

`gamma_gamma` has the same iPad-block stores (`#120`, `#108`, `#104`, `#100`),
but its `#108` is later reused for `this`; the original's `#108` is later reused
for `&Globals::status`. This is the stack-slot coloring choice that produces the
rotation.

## 7. Variants measured with the official verifier

`autojunk=False`, `tools/verify/verify.py --show _ZN3Hud4drawExxP9PlayerEgobjj`.

| variant | size | fuzzy | shield `this` spill |
|---|---|---|---|
| HEAD (baseline) | 9332 | 88.8 % | no |
| clean (both spurious constructs removed) | 9240 | 68.1 % | no |
| `gamma_gamma` (local only for gamma mission arg) | 9264 | 83.3 % | yes, `#108` |
| `g_local_arg2` (same shape) | 9264 | 83.3 % | yes, `#108` |
| `g_local_all` (local for all 3 gamma uses) | 9264 | 83.3 % | yes, `#108` |
| `g_mission_local` (hoist campaign mission) | 9264 | 83.1 % | yes, `#108` |
| `g_idx_local` (hoist station index) | 9264 | 83.2 % | yes, `#108` |
| `w_locals` (iPad width locals) | 9368 | 76.6 % | yes, `#108` |
| `bothtop` (`savedColor`+`dividerOffset` at top) | 9220 | 63.2 % | no |

All of the `this`-spill variants produce the same rotation
`this=#108, div=#100, layout=#104, status=#96`. No variant produced the
original's `this=#104, div=#96, layout=#100, status=#108`.

Other variants that force the spill but do not change the rotation:
`g1_p2` (gamma local + panel local), `status_and_idx` (`Status *st` + index
local), `g2_p0_l1` (gamma mission local + later uses), `w_decl_top`.

Variants that do **not** force the spill: `g_float`, `g_both_local`,
`station_local`, `bool_local`, `int_gamma`, `float_gamma`, `idx_and_mission`,
`panel_status`, `panel_mission`, `panel_mtype`, `panel_ship`, `panel_canvas`,
`ipad_canvas`, `w1`/`w2`/`w3`, `opt84`, `canvas_ptr`, `status_late`,
`kills_late`, `time_late`.

## 8. What remains

The GRA already inserts the `%2925 → %stack.59` split in both clean and
`gamma_gamma`; the difference is the VRR coalescing. The original and
`gamma_gamma` have the same stack-slot **counts**, so the remaining work is not
a missing local or a missing call — it is the stack-slot coloring order:

* the original's `&Globals::status` is `stack.47` (the early iPad `#108` slot);
* `gamma_gamma`'s `this` takes that slot and `&Globals::status` gets a new
  `stack.50` (`#96`).

The most promising direction is a source shape in which a single
`&Globals::status` value is both (a) live across the gamma block in a way that
forces the VRR `this` spill and (b) live to the end of the function (so the
stack-slot coloring reuses the early iPad `#108` slot for it). The tested
single-local variants either force the spill but keep the status value
short-lived (`gamma_gamma`) or make the status value long-lived but lose the
spill (`gamma_gp`/`g2_p1_l0`). A future pass should try to force the status
value into `r8` and let the volatile-goods bar image clobber it, as the
original does at `0x16460a`/`0x16488e`/`0x164a7e`/`0x164c90`.

## 9. Reproduce

```
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w4014/vmeas \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

`FN=... verify-fn` is broken (it passes an empty `FN` to `--show`); use
`tools/verify/verify.py --show` directly.
