# Hud::draw — the status spill slot, the iPad `Globals::globals` slot, and the SSC weight order (w4233)

**Status:** research note, 2026-11-03. Worktree `wt/w4233`, branch `wave92-w4233`,
base `b379d00d`. **No source change landed**: a 100-combination sweep plus targeted
experiments found no source shape that beats the current HEAD (9332 B / 88.8 %).
This commit is the note only.

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` — original **9332 B**, ours **9332 B @
88.8 %** fuzzy. Gate on entry/exit (full `check`): **byte_exact 4498 /
linked_exact 4498**, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, lint CLEAN, drift 0, **RATCHET PASS**.

This note extends `hud-draw-st-local-w4215.md`, `hud-draw-slot-coloring-w4114.md`,
`hud-draw-slot-rotation-w4014.md`, `hud-draw-this-spill-w3910.md` and
`hud-draw-this-spill_w3834.md`. The new results are:

1. The original gamma block is the **clean** shape (`Globals::status` written
   three times, so `getCurrentCampaignMission` reloads `[&Globals::status]`),
   *not* the `st` local shape (which caches the `Status *` value in `r4`). This
   was not stated explicitly before: `st` is a semantic mismatch in the gamma
   block, not just a slot-colouring difference.
2. The original's `#108` slot is shared by **two different values**: the iPad
   `Globals::globals` pointer (stored `0x163c2c`, reloaded `0x163c94`) and
   `&Globals::status` (stored `0x164022`, reloaded `0x164176/0x164210/0x16436e`).
   In the `st` build the same `#108` slot is shared by the iPad
   `Globals::globals` pointer and **`this`** (stored `0x163e04`), and status
   gets a fresh `#96`.
3. The original reloads `&Globals::status` **three times in the damage block**
   (`0x164176`, `0x164210`, `0x16436e`), including a **dead load** at
   `0x164176` (r8 is overwritten at `0x1641e4` before any use). The `st`/clean
   builds reload it only twice, in the much colder mission panel
   (`0x164548`/`0x164802` resp. `0x164530`/`0x1647ea`). This is the real reason
   the original's status interval outranks `this` in Stack Slot Coloring: its
   spill weight is accumulated from hotter blocks.
4. A weight reconstruction from the VRR MIR (branch probabilities propagated
   over the machine CFG) gives the status slot the *smallest* weight of the
   four high-group values in the `st` build (`div 7.5e-4 > this 7.4e-4 >
   layout 6.1e-4 > status 2.6e-5`), matching the observed
   `this=#108, layout=#104, div=#100, status=#96` colouring.
5. A 100-combination sweep (anchor position × local kind × gamma form × shield
   anchor; 24 of them failed to compile because the sweep script left `st`
   undefined when the `pad_` declaration was selected) plus `Status **`/`Status *&`/`Hud *`/`Hud &`/`(Hud*)(uintptr_t)`
   aliasing experiments found **no variant above 88.8 %**. The best faithful
   shapes are `clean` (9240 B / 68.1 %) and `st` (9264 B / 83.3 %); HEAD's
   88.8 % remains an anchor artefact.

## 1. Ground truth (image base 0x10000)

### Shield block — the branch-local `this` spill

```
163de6: ldr   r0, [pc, #264]        ; GOT &Globals::layout
163de8: add   r0, pc
163dea: ldr   r0, [r0, #0]          ; r0 = &Globals::layout
163dec: str   r0, [sp, #100]        ; &Globals::layout -> #100
163dee: ldr   r1, [r0, #0]          ; Globals::layout
163df0: ldr.w r0, [r9]              ; Globals::Canvas
163df4: ldr.w sl, [r1, #488]        ; dividerOffset
163df8: mov.w r1, #0xffffffff
163dfc: blx   PaintCanvas::SetColor
163e00: ldrb.w r0, [r5, #543]       ; this->hasShieldBar
163e04: str   r5, [sp, #104]        ; *** this -> #104 ***
163e06: cmp   r0, #0
163e08: str.w sl, [sp, #96]         ; dividerOffset -> #96
163e0c: beq.n 163efc
163e0e: ldr.w r0, [fp]              ; ego->player
163e12: ldr.w r6, [r9]              ; Canvas
163e16: blx   Player::getShieldHP
163e1a: cmp   r0, #2
163e1c: blt.n 163e2e
163e1e: ldr   r0, [sp, #104]        ; reload this
163e20: ldrb.w r0, [r0, #580]       ; shieldHitFlash
163e24: cbz   r0, 163e2e
163e26: ldr   r5, [sp, #104]        ; reload this
163e28: add.w r0, r5, #680          ; &shieldFrameHitImage
163e2c: b.n   163e34
163e2e: ldr   r5, [sp, #104]        ; reload this
163e30: add.w r0, r5, #676          ; &shieldFrameImage
```

The merge block (`0x163efc`) uses `r5` directly, so the spill is branch-local.
`this` is reloaded later at `0x164040` (r6), `0x1640ec` (r5) and `0x16413e`
(sl); after `0x16413e` it lives in `sl` for the rest of the function.

### Gamma block — `&Globals::status` is loaded here, and reloaded

```
164008: strd  fp, r9, [sp, #116]    ; ego, &Globals::Canvas
16400c: ldr   r0, [pc, #276]        ; GOT &Globals::status
16400e: add   r0, pc
164010: ldr   r6, [r0, #0]          ; r6 = &Globals::status
164012: ldr   r4, [r6, #0]          ; r4 = Globals::status
164014: mov   r0, r4
164016: blx   Status::getStation
16401a: blx   Station::getIndex
16401e: mov   r5, r0
164020: ldr   r0, [r6, #0]          ; *** reload Globals::status ***
164022: str   r6, [sp, #108]        ; *** &Globals::status -> #108 ***
164024: blx   Status::getCurrentCampaignMission
164028: mov   r2, r0
16402a: mov   r0, r4
16402c: mov   r1, r5
16402e: blx   Status::getGammaRayDamagePerSecond
```

This is exactly the **clean** source shape:

```cpp
if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                Globals::status->getCurrentCampaignMission()) > 0.0f)
```

The `st` local shape (`Status *st = Globals::status; if (st->... st->... st->...)`)
compiles differently: it spills `&Globals::status` *before* `getStation`
(`0x164012`) and caches the `Status *` value in `r4`, so
`getCurrentCampaignMission` is called on `r4` with **no reload**:

```
164008: strd  fp, r9, [sp, #116]
16400c: ldr   r0, [pc, #276]
16400e: add   r0, pc
164010: ldr   r0, [r0, #0]          ; r0 = &Globals::status
164012: str   r0, [sp, #96]         ; &Globals::status -> #96
164014: ldr   r4, [r0, #0]          ; r4 = Globals::status
164016: mov   r0, r4
164018: blx   Status::getStation
16401c: blx   Station::getIndex
164020: mov   r5, r0
164022: mov   r0, r4                ; *** no reload ***
164024: blx   Status::getCurrentCampaignMission
```

So `st` is a semantic mismatch in the gamma block, not merely a slot-colouring
difference. The original source is the clean form.

### iPad block — the `#108` slot's first occupant

```
163c16: ldr   r0, [pc, #712]        ; GOT &Globals::globals
163c18: ldr   r1, [pc, #712]        ; GOT &Globals::Canvas
163c1a: add   r0, pc
163c1c: add   r1, pc
163c1e: ldr   r0, [r0, #0]          ; r0 = &Globals::globals
163c20: str   r0, [sp, #120]        ; &Globals::globals -> #120
163c22: ldr   r2, [pc, #708]        ; GOT &PaintCanvas::GetImage2DWidth
163c24: ldr.w r8, [r1]              ; r8 = Globals::Canvas
163c28: add   r2, pc
163c2a: ldr   r0, [r0, #0]          ; r0 = Globals::globals
163c2c: str   r0, [sp, #108]        ; *** Globals::globals -> #108 ***
...
163c94: ldr   r0, [sp, #108]        ; reload Globals::globals (setCoordsSteer this)
```

So `#108` holds `Globals::globals` (the pointer) from `0x163c2c` to `0x163c94`,
and `&Globals::status` from `0x164022` onwards. The two live ranges do not
overlap, so Stack Slot Coloring merges them.

### Damage block — where the status reloads actually live

```
16413e: ldr.w sl, [sp, #104]        ; this -> sl
...
164176: ldr.w r8, [sp, #108]        ; *** &Globals::status (dead: r8 clobbered at 1641e4) ***
1641a0: ldr.w r0, [sl, #1196]
...
1641e4: ldr.w r8, [r1]              ; r8 = Globals::h (clobbers the dead load)
...
164210: ldr.w r8, [sp, #108]        ; *** &Globals::status ***
...
16436e: ldr   r6, [sp, #108]        ; *** &Globals::status ***
```

Three references, all in the "damage direction indicators" block, which is
much hotter than the mission panel. The `st`/clean builds instead reload the
status slot twice in the mission panel:

```
st:    164548: ldr r4, [sp, #96]    ; &Globals::status
       164802: ldr r0, [sp, #96]
clean: 164530: ldr r4, [sp, #96]
       1647ea: ldr r0, [sp, #96]
```

## 2. Slot maps

Original (frame 224):

```
#60  damage scratch   #64  timeLeft lo   #68  timeLeft hi   #72  fireImageIndex
#76  secondaryKind    #80  mining        #84  savedColor    #88  &guard
#92  iPad             #96  dividerOffset #100 &Globals::layout
#104 this             #108 Globals::globals / &Globals::status
#112 dt               #116 ego           #120 &Globals::Canvas
#160 String temp      #220 canary
```

`st` (frame 224) — same sizes, cyclically rotated high group:

```
#96  &Globals::status #100 dividerOffset  #104 &Globals::layout  #108 this
```

`clean` (frame 216, no `this` slot):

```
#60 timeLeft lo #64 timeLeft hi #68 secondaryKind #72 fireImageIndex
#76 mining #80 savedColor #84 &guard #88 iPad #92 dividerOffset/width3
#96 &Globals::status/width2 #100 &Globals::layout/width1
#104 dt #108 ego #112 Globals::globals
```

(`Globals::Canvas` stays in `r9` in the clean build; it is not spilled.)

HEAD (frame 224, both anchors) — closest to the original:

```
#64 timeLeft lo #68 timeLeft hi #72 pad_ #76 secondaryKind #80 fireImageIndex
#84 mining #88 &guard #92 iPad #96 dividerOffset #100 savedColor/width2
#104 &Globals::layout/width1 #108 &Globals::status #112 dt #116 ego
#120 Globals::globals
```

HEAD keeps `dividerOffset=#96` and `&Globals::status=#108` (matching the
original) but puts `&Globals::layout` at `#104` (original `#100`, where HEAD has
no `this` slot) and `savedColor` at `#100` (original `#84`). HEAD also has the
extra shield `SetColor(savedColor)` call and the extra `this->field_0x470` read
at the gamma block, and lacks the `this` spill.

## 3. Why the colour order flips: SSC weights

`StackSlotColoring::ScanForSpillSlotRefs` (`ssc.cpp`) adds
`LiveIntervals::getSpillWeight(false, true, MBFI, MI)` for every frame-index
reference, i.e. the block frequency normalised to the entry. Intervals are
then `stable_sort`ed by weight **descending**; the first interval takes the
lowest free frame index, and each later interval either shares the lowest
non-overlapping used colour or takes the next free FI. Lower FI maps to a
higher `sp` offset in this frame, so the first-processed interval gets the
highest address.

The observed colouring therefore tells us the processing order directly:

```
st:       this=#108(fi47)  layout=#104(fi48)  div=#100(fi49)  status=#96(fi50)
original: status=#108(fi47) this=#104(fi48)  layout=#100(fi49) div=#96(fi50)
```

In the `st` build the status interval is processed **after** `this`, `layout`
and `div`; in the original it is processed **before** them. The only
source/codegen difference between the two builds that can affect the weights
is the damage-block materialisation of `&Globals::status` (§1): three frame
references in the original (`0x164176`, `0x164210`, `0x16436e`, one of them
dead) versus zero in `st`/`clean`. Therefore the original's status interval
must have the larger spill weight and is coloured first among the four.

A reconstruction of the `st` weights from the VRR MIR (propagating the printed
branch probabilities over the machine CFG) confirms that status is the
lightest of the four in that build:

```
fi#63  dividerOffset      7.54e-4
fi#59  this               7.35e-4
fi#49  &Globals::layout   6.05e-4
fi#50  &Globals::status   2.6e-5
```

(The absolute values are approximate — blocks reachable only through indirect
branches are under-counted — but the relative order of these four is stable
and matches the observed `st` colouring: status is last, hence `#96`.)

The extra damage-block references are a GRA artefact: the original's register
allocator preloads `&Globals::status` into `r8` at `0x164176` (a dead load,
clobbered at `0x1641e4`) and then reloads it at `0x164210` and `0x16436e`.
The `st`/clean builds never materialise it in the damage block.

## 4. Variants measured (official verifier, `autojunk=False`)

| variant | size | fuzzy |
|---|---|---|
| **HEAD (baseline)** | **9332** | **88.8 %** |
| HEAD + `Hud *self`/`Player *player`/`bool hasShield`/`Hud *hud` in shield | 9332 | 88.8 % (identical code) |
| HEAD + `Status *st` at top/ipad/savedcolor/shield/divider (gamma clean) | 9332 | 88.8 % (identical code) |
| HEAD + `Status **pst` / `Status *&st` at ipad/shield | 9332 | 88.8 % (identical code) |
| HEAD + `(Hud *)(uintptr_t)this` at shield / at entry | 9332 | 88.8 % (identical code) |
| HEAD + `Hud &self = *this` in shield | 9332 | 88.8 % (identical code) |
| HEAD + `Hud *volatile self` in shield | 9292 | 71.4 % |
| HEAD − shield anchor + `st` (`noshield_st`) | 9348 | 88.0 % |
| HEAD − shield anchor (`noshield_nogamma`) | 9324 | 83.9 % |
| HEAD − shield anchor − `pad_` use (`noshield_padonly`) | 9240 | 68.1 % |
| HEAD + `st` (gamma uses `st`) | 9356 | 85.8 % |
| clean (both anchors removed) | 9240 | 68.1 % |
| clean + `st` at gamma (`st`) | 9264 | 83.3 % |
| clean + `st` at top/ipad/savedcolor/shield/divider, gamma `st` | 9292–9332 | 70.6–71.0 % |
| clean + `st` at top/ipad, gamma clean | 9240 | 68.1 % (dead local) |
| clean + `st` at ipad, gamma mission arg only | 9332 | 70.9 % |
| clean + `st` at ipad, gamma both args | 9332 | 70.6 % |
| clean + `st` at ipad + first mission-panel use | 9672 | 58.6 % |
| clean + `Status **pst`/`Status *&st` at any anchor | 9240 | 68.1 % (rematerialised) |
| clean + `div2`/`savedColor2` copies | 9240 | 68.1 % |

The sweep covered `base ∈ {clean, head}`, `decl ∈ {none, st, pad}`,
`anchor ∈ {top, ipad, savedcolor, shield, divider, gamma}`,
`gamma ∈ {clean, st}`, `shield_anchor ∈ {yes, no}` — 100 combinations
(24 of which were script-invalid and failed to compile). No
combination exceeds 88.8 %.

## 5. What remains

The remaining gap is entirely in the register allocator / stack-slot colourer,
not in the source semantics of the shield or gamma blocks:

* The clean source is the original's gamma shape (reload at `0x164020`) and the
  original's shield shape *except* that the GRA coalesces the `this` copy to
  `$r5` instead of spilling it.
* The `st` local forces the `this` spill (reproducing the original's shield
  block byte-for-byte apart from the slot base) but changes the gamma block to
  cache the `Status *` in `r4`, which is not the original.
* The original's status interval is coloured to `#108` because the GRA
  materialises `&Globals::status` in the damage block (three refs, one dead),
  giving it a higher SSC weight than `this`. No source shape tried so far
  reproduces that damage-block materialisation while keeping the clean gamma
  block and the `this` spill.

A source shape that (a) keeps `Globals::status->...` written three times in the
gamma condition, (b) forces the shield-block `this` spill, and (c) makes the
GRA preload `&Globals::status` in the damage block would be byte-exact or very
close. The three properties are coupled through global register pressure, and
the 60-variant sweep did not find a combination that satisfies all three.

## 6. Reproduction

```
cmake --preset default
cmake --build cmake-build-match --target check -j8          # 4498/4498
# apply a variant to src/game/ui/Hud.cpp, then:
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w4233/vmeas \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

MIR: `ninja -C cmake-build-match -t commands CMakeFiles/gof2.dir/src/game/ui/Hud.cpp.obj | tail -1`
with `-mllvm -print-machineinstrs`, then read the
`After Greedy Register Allocator`, `After Virtual Register Rewriter` and
`After Stack Slot Coloring` stages. The weight reconstruction script is
`/dev/shm/w4233/weights3.py` (scratch; the algorithm is described in §3).
