# Hud::draw — stack-slot coloring, the `this` spill, and the `top` variant (w4114)

**Status:** research note, 2026-11-03. Worktree `wt/w4114`, branch `wave91-w4114`,
base `a4557300`. **No source change landed**: no tested variant makes `Hud::draw`
byte-exact, and the only gate-neutral variant (`top`) is *less* fuzzy than the
current HEAD, so this commit is the note only.

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` — original **9332 B**, ours **9332 B @
88.8 %** fuzzy. Gate on entry/exit (full `check`): **byte_exact 4498 /
linked_exact 4498**, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports 207, `.text 0x1337c8`,
lint CLEAN, drift 0, **RATCHET PASS**.

This note extends `hud-draw-slot-rotation-w4014.md` and
`hud-draw-this-spill-w3910.md`. The new results are:

1. A variant that puts `Status *gstatus = Globals::status;` at the **top** of
   `Hud::draw` (`top`) reproduces the original's **exact stack-slot assignment**
   (`layout=#100`, `this=#104`, `dividerOffset=#96`, `status=#108`) and the
   original's 9332-byte size. It is gate-neutral (`byte_exact 4498`), but it
   loads `&Globals::status` at the top (spilling it to `#108` there) and puts
   `this` in `r4` (`mov r4,r0`) instead of the original's `r5`.
2. A variant that declares the same local just before `dividerOffset`
   (`g_before_shield`) keeps `this` in **`r5`** *and* reproduces the exact slot
   assignment, but is **36 B short** (9296 B) and adds extra early spills
   (`str.w r8,[sp,#116]`, `str r4,[sp,#88]`).
3. The clean source (HEAD minus the shield `SetColor(savedColor)` and the
   `pad_` anchor) is **9240 B / 68.1 %**. It is already the original's shape in
   the entry, shield, and gamma blocks **except** that the `this` copy at the
   `hasShieldBar` merge is coalesced to `$r5` by the Virtual Register Rewriter;
   the original keeps it in a stack slot.
4. The `this` stack slot's live range in the original ends at `0x16413e`
   (`ldr.w sl,[sp,#104]`), after which `this` lives in `sl` for the mission
   panel. `&Globals::status`'s slot lives to the end (`0x165d48`). This is why
   the original colours `status` to the early iPad `#108` slot and `this` to
   `#104`; `gamma_gamma` keeps `this` in the *stack* for the whole mission panel
   and so colours `this` to `#108`.
5. Every variant that forces the `this` spill by adding a long-lived local
   either changes the entry (loads `&Globals::status` early) or changes the
   mission-panel codegen. No tested source shape reproduces the original's
   `str r5,[sp,#104]` with the clean entry and clean gamma block.

## 1. Ground truth (image base 0x10000)

### Entry

```
163b90: push  {r4, r5, r6, r7, lr}
163b92: add   r7, sp, #12
163b94: stmdb sp!, {r7, r8, r9, sl, fp}
163b98: vpush {d8-d11}
163b9c: sub   sp, #224
163b9e: str   r2, [sp, #112]        ; dt
163ba0: mov   r5, r0                ; *** this = r5 ***
163ba2: ldr   r0, [pc, #816]        ; &__stack_chk_guard
163ba4: add   r0, pc
163ba6: ldr   r0, [r0, #0]
163ba8: str   r0, [sp, #88]
163baa: ldr   r1, [r0, #0]
163bac: ldr   r0, [r7, #16]         ; ego
163bae: str   r1, [sp, #220]        ; guard
163bb0: str   r0, [sp, #116]        ; ego -> stack
163bb2: blx   PlayerEgo::isMining
```

### Shield block — the `this` spill

```
163de6: ldr   r0, [pc, #264]        ; &Globals::layout
163de8: add   r0, pc
163dea: ldr   r0, [r0, #0]
163dec: str   r0, [sp, #100]        ; &Globals::layout
163dee: ldr   r1, [r0, #0]          ; Globals::layout
163df0: ldr.w r0, [r9]              ; Globals::Canvas
163df4: ldr.w sl, [r1, #488]        ; dividerOffset
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
spill is a branch-local split; `this` is *not* in the stack slot after the
merge.

### Gamma block — `&Globals::status` is loaded here, not at the top

```
164008: strd  fp, r9, [sp, #116]    ; spill ego, &Globals::Canvas
16400c: ldr   r0, [pc, #276]        ; GOT &Globals::status
16400e: add   r0, pc
164010: ldr   r6, [r0, #0]          ; r6 = &Globals::status
164012: ldr   r4, [r6, #0]          ; Globals::status
164014: mov   r0, r4
164016: blx   Status::getStation
16401a: blx   Station::getIndex
16401e: mov   r5, r0
164020: ldr   r0, [r6, #0]          ; Globals::status
164022: str   r6, [sp, #108]        ; *** spill &Globals::status ***
164024: blx   Status::getCurrentCampaignMission
164028: mov   r2, r0
16402a: mov   r0, r4
16402c: mov   r1, r5
16402e: blx   Status::getGammaRayDamagePerSecond
164032: vmov  s0, r0
164036: vcmpe.f32 s0, #0.0
16403a: vmrs  APSR_nzcv, fpscr
16403e: ble.n 1640e0
164040: ldr   r6, [sp, #104]        ; *** reload this into r6 ***
164042: ldr   r5, [sp, #120]        ; &Globals::Canvas
164044: ldrh.w r0, [r6, #1090]
```

### Damage block — the `#60` scratch slot

```
16431a: ldr   r0, [pc, #512]
16431c: mov   r8, sl
16431e: ldr   r1, [pc, #512]
164320: mov   sl, fp
164322: add   r0, pc
164324: ldr.w r2, [fp, #20]
164328: add   r1, pc
16432a: ldr   r0, [r0, #0]
16432c: ldr   r1, [r1, #0]
16432e: ldr   r6, [r2, #80]
164330: ldr.w r9, [r0]              ; Globals::w
164334: ldr   r0, [r1, #0]          ; Globals::h
164336: str   r0, [sp, #60]         ; *** scratch: Globals::h ***
...
16436a: ldr   r0, [sp, #60]         ; *** reload Globals::h ***
```

### Mission panel — `&Globals::status` is kept in `r8` and reloaded from `#108`

```
164176: ldr.w r8, [sp, #108]        ; &Globals::status
...
164558: ldr.w r0, [r8]              ; Globals::status
16455c: blx   Status::getMission
...
16460a: ldr.w r8, [sl, #832]        ; clobber r8 (volatile-goods bar image)
...
164eba: ldr.w r8, [sp, #108]        ; reload &Globals::status
...
165d48: ldr.w r8, [sp, #108]        ; last reload
```

### Epilogue — unconditional `SetColor(savedColor)`

```
165e66: ldr.w r0, [r9]              ; Globals::Canvas
165e6a: ldr   r1, [sp, #84]         ; savedColor
165e6c: blx   PaintCanvas::SetColor
165e70: ldr   r0, [sp, #220]        ; canary
165e72: ldr   r1, [sp, #88]
165e74: ldr   r1, [r1, #0]
165e76: subs  r0, r1, r0
165e78: itttt eq
165e7a: addeq sp, #224
165e7c: vpopeq {d8-d11}
165e80: addeq sp, #4
165e82: ldmiaeq.w sp!, {r8, r9, sl, fp}
165e86: it    eq
165e88: popeq {r4, r5, r6, r7, pc}
165e8a: blx   __stack_chk_fail
```

The original uses `savedColor` (#84) only at `0x16479a` (rocket-block early
return) and `0x165e6a` (epilogue). HEAD's shield-branch `SetColor(savedColor)`
and its `if (pad_ != 0) SetColor(savedColor)` are extra anchors; the clean
source (both removed) matches the original's `savedColor` uses exactly.

## 2. Slot maps

Original (frame 224):

```
#60  damage scratch   #64  timeLeft lo   #68  timeLeft hi   #72  fireImageIndex
#76  secondaryKind    #80  mining        #84  savedColor    #88  &guard
#92  iPad             #96  dividerOffset #100 &Globals::layout
#104 this             #108 &Globals::status
#112 dt               #116 ego           #120 &Globals::Canvas
#160 String temp      #220 canary
```

Clean (frame 216, no `this` slot, no `#60` scratch):

```
#60  timeLeft lo      #64  timeLeft hi   #68  fireImageIndex #72  secondaryKind
#76  mining           #80  savedColor    #84  &guard         #88  iPad
#92  dividerOffset    #96  &Globals::status
#100 &Globals::layout #104 dt            #108 ego           #112 &Globals::Canvas
#152 String temp      #212 canary
```

`top` (frame 224) — **exact original high-group slot assignment**; the low
group is clean-like (`#60` is timeLeft lo, not the damage scratch):

```
#60  timeLeft lo      #64  timeLeft hi   #68  fireImageIndex #72  secondaryKind
#76  mining           #80  savedColor    #84  &guard         #88  iPad
#92  (iPad temp)      #96  dividerOffset #100 &Globals::layout
#104 this             #108 &Globals::status
#112 dt               #116 ego           #120 &Globals::Canvas
#160 String temp      #220 canary
```

`gamma_gamma` (frame 224) — same slot *set*, cyclically rotated:

```
#96  &Globals::status  #100 dividerOffset  #104 &Globals::layout  #108 this
```

`g_before_shield` (frame 224) — exact original slot assignment, `this` in `r5`:

```
#96  dividerOffset  #100 &Globals::layout  #104 this  #108 &Globals::status
```

## 3. Variants measured with the official verifier

`autojunk=False`, `tools/verify/verify.py --show _ZN3Hud4drawExxP9PlayerEgobjj`.

| variant | size | fuzzy | byte_exact | slots (this/div/status/layout) |
|---|---|---|---|---|
| HEAD (baseline) | 9332 | 88.8 % | 4498 | no `this` spill; `pad_` anchor |
| clean (both anchors removed) | 9240 | 68.1 % | 2944 | no `this` spill |
| `gamma_gamma` (local only for gamma mission arg) | 9264 | 83.3 % | 2994 | 108/100/96/104 |
| `top` (`gstatus` at function top) | **9332** | **70.9 %** | **4498** | **104/96/108/100** |
| `g_before_shield` (`gstatus` before `dividerOffset`) | 9296 | 67.7 % | 2994 | 104/96/108/100 |
| `val_gamma_all` (local used throughout mission panel) | 9260 | 65.1 % | 2944 | 108/96/104/100 |
| `val_before_shield_all` | 9252 | 51.1 % | 2944 | div=92 |
| `val_before_shield_gamma` | 9296 | 67.7 % | 2994 | 104/96/108/100 |
| `head_gbs` (HEAD + `gstatus` before `dividerOffset`) | 9328 | — | — | this=112, div=124 |
| `ref_top` / `ref_gamma` / `ref_top_all` (`Status *&`) | 9240 | — | — | no spill |
| `v1_bool` / `v2_self` | 9240 | — | — | no spill |
| `v3_canvas_hoist` / `v4_div_after` | 9236 | — | — | no spill |

The `top` variant is the only one that is both 9332 B and has the exact slot
assignment. It is gate-neutral (`byte_exact 4498`), not an improvement.

## 4. `top`: exact slots, wrong register

`top` = clean + `Status *gstatus = Globals::status;` before `int dt`, with the
gamma mission argument using `gstatus`. Its entry:

```
163b9c: sub   sp, #224
163b9e: str   r2, [sp, #112]        ; dt
163ba0: mov   r4, r0                ; *** this = r4 (original: r5) ***
163ba2: ldr   r0, [pc, #824]        ; &__stack_chk_guard
163ba4: ldr   r1, [pc, #824]        ; &Globals::status
163ba6: add   r0, pc
163ba8: add   r1, pc
163baa: ldr   r0, [r0, #0]
163bac: ldr   r2, [r1, #0]          ; &Globals::status
163bae: str   r0, [sp, #84]
163bb0: ldr   r1, [r0, #0]
163bb2: ldr   r0, [r2, #0]          ; Globals::status
163bb4: str   r2, [sp, #108]        ; *** status spilled at the top ***
163bb6: str   r0, [sp, #88]
163bb8: ldr   r0, [r7, #16]
163bba: str   r1, [sp, #220]
163bbc: str   r0, [sp, #120]
163bbe: blx   PlayerEgo::isMining
```

Its shield block is the original's shape with `r4` for `this`:

```
163e06: ldrb.w r0, [r4, #543]
163e0a: str   r4, [sp, #104]        ; this spill
163e0c: cmp   r0, #0
163e0e: str.w sl, [sp, #96]         ; dividerOffset
163e12: beq.n 163f08
...
163e24: ldr   r0, [sp, #104]
163e26: ldrb.w r0, [r0, #580]
163e2a: cbz   r0, 163e34
163e2c: ldr   r4, [sp, #104]
163e2e: add.w r0, r4, #680
```

Its gamma block reloads `&Globals::status` from the stack instead of loading the
GOT address in the gamma block:

```
164018: strd  r8, r9, [sp, #116]
16401c: ldr   r0, [sp, #108]        ; &Globals::status from the top spill
16401e: ldr   r4, [r0, #0]
164020: mov   r0, r4
164022: blx   Status::getStation
```

So `top` proves the slot-colouring target is reachable (`status=#108`,
`this=#104`, `layout=#100`, `div=#96`), but it is not the original's source
shape: the status load is hoisted to the function top and `this` is forced into
`r4`.

## 5. `g_before_shield`: exact slots and `this` in `r5`, but 36 B short

`g_before_shield` = clean + `Status *gstatus = Globals::status;` immediately
before `int dividerOffset`. It keeps `this` in `r5`:

```
ldrb.w r0, [r5, #543]
str    r5, [sp, #104]        ; this spill
cmp    r0, #0
str.w  sl, [sp, #96]         ; dividerOffset
```

and has the exact slot assignment `this=#104, div=#96, status=#108,
layout=#100`, but it is **9296 B**, 36 B short of the original, and it adds
extra early spills (`str.w r8,[sp,#116]`, `str r4,[sp,#88]`) and moves `ego`
into `fp` at the entry (`ldr.w fp,[r7,#16]`) instead of the original's
`str r0,[sp,#116]`. The 36 missing bytes are not a single missing call: the
`blx` call sequence of the clean build matches the original's exactly except
for the duplicated `String::~String` at `0x1646a2` (see
`hud-draw-this-spill-w3910.md`), which is downstream of the `r8` reload
placement.

## 6. MIR evidence: the GRA placement differs, the VRR coalescing decides

From `-mllvm -print-machineinstrs` on the clean source (`clean_gra.txt`) and
`gamma_gamma` (`gg_gra.txt`):

**Clean** — the GRA keeps `this` live through `bb.28` and splits it at the
merge; the two predecessor copies are:

```
bb.33:  %2926:gprnopc = COPY %2925:gprnopc     ; clean_gra.txt:6772B
bb.34:  %2926:gprnopc = COPY %2925:gprnopc     ; clean_gra.txt:7348B
```

The Virtual Register Rewriter coalesces both copies back to `$r5`
(`clean_vrr.txt:556` stores `dividerOffset` to `%stack.59`, not `this`), so the
final code has no `this` store.

**gamma_gamma** — the GRA spills `this` early in `bb.28`:

```
%2925:gprnopc = COPY %2924:gprnopc             ; gg_gra.txt:5320B
t2STRi12 %2925:gprnopc, %stack.59              ; gg_gra.txt:5464B (this)
t2STRi12 %2919:gprnopc, %stack.63              ; gg_gra.txt:5468B (dividerOffset)
```

and the VRR keeps both stores (`gg_vrr.txt:557-558`). The final PEI has
`str r5,[sp,#108]` (this) and `str r10,[sp,#100]` (dividerOffset).

So the missing source shape is the one that makes the VRR *not* coalesce the
`this` copy — the GRA already wants the split in both builds.

## 7. Why `status` gets the early iPad `#108` slot in the original

The `#108`/`#104`/`#100` physical slots are created early in the iPad-coordinate
block (`0x163c20`-`0x163c5c`: `Globals::globals`, width1, width2). Stack-slot
colouring then reuses them for the long-lived values. The original's `this`
stack slot lives only from the shield spill (`0x163e04`) to the damage block
reload (`0x16413e: ldr.w sl,[sp,#104]`); after that `this` is in `sl` and the
slot is dead. `&Globals::status` lives from the gamma block (`0x164022`) to the
last mission-panel reload (`0x165d48`), i.e. across many more blocks. That is
why the original colours `status` to `#108` and `this` to `#104`.

`gamma_gamma` also loads `this` into `sl` at `0x16413e`
(`ldr.w sl,[sp,#108]`), but its `&Globals::status` is only the gamma-condition
value (the mission panel uses `Globals::status` directly), so `this` ends up
with the higher weight and gets `#108`.

Per-slot reference counts (`[sp,#N]` operands over the whole function,
`objdump --no-show-raw-insn`):

| offset | original | clean | top |
|---|---|---|---|
| #60 | 2 | 5 | 3 |
| #64 | 5 | 10 | 2 |
| #68 | 10 | 4 | 4 |
| #72 | 4 | 5 | 5 |
| #76 | 5 | 6 | 3 |
| #80 | 3 | 3 | 3 |
| #84 | 3 | 3 | 3 |
| #88 | 3 | 9 | 6 |
| #92 | 10 | 28 | 7 |
| #96 | 27 | 17 | 23 |
| #100 | 27 | 27 | 26 |
| #104 | 8 | 16 | 11 |
| #108 | 23 | 9 | 19 |
| #112 | 16 | 36 | 16 |
| #116 | 9 | 9 | 32 |
| #120 | 38 | 36 | 24 |

The `#116`/`#120` counts differ because `top` spills `r8`/`r9` in the gamma
block (`strd r8,r9,[sp,#116]`) while the original spills `fp`/`r9`
(`strd fp,r9,[sp,#116]`) and reloads `ego` from `#116`.

## 8. What remains

The GRA already inserts the `this` split in the clean build; the VRR coalesces
it. The original keeps it. Every tested source shape that forces the VRR split
does so by adding a long-lived local, which either

* hoists the `&Globals::status` load to the function top (`top`: exact slots,
  `this` in `r4`, 9332 B), or
* hoists `ego` into `fp` and adds early spills (`g_before_shield`: exact slots,
  `this` in `r5`, 9296 B).

The most promising next direction is a source shape that forces the `this` copy
to survive the VRR **without** adding a value live across the entry — e.g. a
construct that raises register pressure only in `bb.28` (between
`SetColor(0xffffffff)` and the `hasShieldBar` test) while leaving the entry and
the gamma block untouched. `g_before_shield` shows that the target slot
assignment and `this`-in-`r5` are simultaneously reachable; the remaining 36
bytes (the `#60` scratch, the duplicated `String::~String`, and the
`this`-reload schedule) are downstream of that split.

## 9. Reproduce

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w4114/vmeas \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

`FN=... verify-fn` is broken (it passes an empty `FN` to `--show`); use
`tools/verify/verify.py --show` directly.
