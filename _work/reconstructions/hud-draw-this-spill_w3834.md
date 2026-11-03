# Hud::draw — the `this` spill at `[sp,#104]` (w3834)

**Status:** research note, 2026-11-03. Worktree `wt/w3834`, branch `wave89-w3834`,
base `5710f6d0`. No source change landed: the only source delta that removes the two
known-spurious constructs *lowers* the fuzzy score (see below), so this commit is the
note only.

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` — original 9332 B, ours 9332 B @ 88.8 %
fuzzy. Gate on entry/exit: **byte_exact 4498 / linked_exact 4498**, size-exact
4522/4524, `.text 0x1337c8`, `.rodata/.data/.data.rel.ro/.plt/.got` byte-identical,
lint CLEAN, drift 0, **RATCHET PASS**.

## 1. What the current source gets wrong (confirmed)

The wave-88 note is right about both spurious constructs:

* `canvas->SetColor(savedColor)` in the `hasShieldBar` branch (ours 25 `SetColorEj`,
  original 23).
* `int pad_ = this->field_0x470;` + `if (pad_ != 0) Globals::Canvas->SetColor(savedColor);`
  before the autopilot block. The original reads `field_0x470` only in the decrement
  block (`0x163d5a`/`0x163d6a`).

Removing both gives **9240 B, frame 216, 68.1 %**. The fuzzy drop from 88.8 % to
68.1 % is *not* a code regression: the 88.8 % score is an artifact of the spurious
`pad_` local keeping the frame at 224 so every `[sp,#N]` operand lines up. Once the
frame is 216, difflib mismatches every stack operand in the function. Gate counts are
unchanged (4498/4498) either way.

With both constructs removed the **call sequence is identical to the original except
for one call**: the original has one extra `String::~String` (`StringD1Ev`) plus a
`b.w` in the mission-type-12 branch:

```
orig 0x16469c  blx DrawString
     0x1646a0  add r0, sp, #208
     0x1646a2  blx StringD1Ev        <-- ours does not have this
     0x1646a6  b.w  0x164eba
ours            blx DrawString
                b.n  0x16495a        <-- shared destructor at the mission-panel exit
```

So the 9240 B source is semantically faithful apart from destructor placement /
stack-slot colouring. (A `difflib` diff of the *call targets* of both builds reports
exactly this one deletion and nothing else.)

## 2. The missing `this` spill

Original, around the shield/armour branch (image base 0x10000):

```
163df4: f8d1 a1e8   ldr.w  sl,[r1,#488]      ; dividerOffset = Globals::layout->field_0x1e8
163df8: f04f 31ff   mov.w  r1,#0xffffffff
163dfc: f6fb ee60   blx    SetColor
163e00: f895 021f   ldrb.w r0,[r5,#543]      ; this->hasShieldBar
163e04: 951a        str    r5,[sp,#104]      ; *** spill this ***
163e06: 2800        cmp    r0,#0
163e08: f8cd a060   str.w  sl,[sp,#96]       ; spill dividerOffset
163e0c: d076        beq.n  163efc
163e0e: f8db 0000   ldr.w  r0,[fp]           ; ego->player
163e12: f8d9 6000   ldr.w  r6,[r9]           ; Globals::Canvas
163e16: f6fe ec56   blx    Player::getShieldHP
163e1a: 2802        cmp    r0,#2
163e1c: db07        blt.n  163e2e
163e1e: 981a        ldr    r0,[sp,#104]      ; *** reload this ***
163e20: f890 0244   ldrb.w r0,[r0,#580]      ; this->shieldHitFlash
163e24: b118        cbz    r0,163e2e
163e26: 9d1a        ldr    r5,[sp,#104]      ; *** reload this ***
163e28: f505 702a   add.w  r0,r5,#680        ; &this->shieldFrameHitImage
163e2c: e002        b.n    163e34
163e2e: 9d1a        ldr    r5,[sp,#104]      ; *** reload this ***
163e30: f505 7029   add.w  r0,r5,#676        ; &this->shieldFrameImage
```

Ours (9240 B build, object offsets; identical instruction sequence otherwise):

```
0270: ldrb.w r0,[r5,#543]
0274: str.w  sl,[sp,#92]        ; only dividerOffset is spilled
0278: cmp    r0,#0
027a: beq.n  ...
027c: ldr.w  r0,[fp]
0280: ldr.w  r6,[r9]
0284: bl     Player::getShieldHP
0288: cmp    r0,#2
028a: blt.n  ...
028c: ldrb.w r0,[r5,#580]       ; this still in r5
0290: cbz    r0,...
0292: add.w  r0,r5,#680
0296: b.n    ...
0298: add.w  r0,r5,#676
```

`this` is spilled once and reloaded at **six** sites in the original:
`0x163e1e`, `0x163e26`, `0x163e2e` (shield image selection), `0x164040` (gamma
block), `0x1640ec` (rocket block), `0x16413e` (damage indicators). Everything else
(shield body, armour block, armour-regen block) reads `this` straight out of `r5`.

The same slot `[sp,#104]` is also used at `0x163c3c` for a `GetImage2DWidth` scratch
value, i.e. the slot is coloured/shared.

## 3. Stack-slot maps

Original (frame 224):

```
#60 ?            #64 timeLeft lo  #68 timeLeft hi  #72 fireImageIndex
#76 secondaryKind #80 mining      #84 savedColor   #88 guard
#92 iPad         #96 dividerOffset #100 &Globals::layout  #104 this
#108 Globals::globals  #112 dt    #116 ego         #120 Canvas
#160 String temp #220 canary
```

Ours, 9240 B build (frame 216):

```
#56 ?            #60 timeLeft lo  #64 timeLeft hi  #68 fireImageIndex
#72 secondaryKind #76 mining      #80 savedColor   #84 guard
#88 iPad         #92 dividerOffset **and** &Globals::layout (shared)
#96 this         #100 Globals::globals **and** &Globals::layout (shared)
#104 dt          #108 ego         #112 Canvas
#152 String temp #212 canary
```

So the original is **two** slots bigger than ours, not one:

1. `this` gets its own slot (#104).
2. `dividerOffset` gets its own slot (#96); in ours it is coloured into the same
   slot as `&Globals::layout` (#92).

The high group (`&layout` … `Canvas`) is therefore shifted by 8, not 4.

## 4. Machine-IR evidence (ours, `-mllvm -print-machineinstrs`)

The greedy allocator itself creates the split. In the *After Greedy Register
Allocator* dump (`NoPHIs`) it inserts a copy of `this` in **both** predecessors of the
armour/merge block and uses the copy in the merge block:

```
bb.33 (ir-block.248, shield then):
  ...
  %2926:gprnopc = COPY %2925:gprnopc     ; %2925 = COPY $r0 = this
  %584 = t2LDRHi12 %2926, 1098
  ...
  %2728 = t2ADDri12 %2926, 1092
  %2727 = t2ADDri   %2926, 1096
  t2B %bb.35
bb.34 (ir-block.297, else):
  %2728 = t2ADDri12 %2925, 1090
  %2727 = t2ADDri12 %2925, 1098
  %2926:gprnopc = COPY %2925:gprnopc
bb.35 (ir-block.301, merge):
  %2948 = t2LDRHi12 %2727, 0
  %595  = t2LDRHi12 %2728, 0
  ... every later `this` load in this block uses %2926 ...
```

In ours the copy coalesces back to `$r5` (no store). The original is the same split
with the copy **not** coalesced: it is spilled to `[sp,#104]` and reloaded at each
use. The pre-RA ("After Machine Instruction Scheduler") dump uses `%420` (= `this`)
directly in the merge block; the copy only appears in the RA dump, so it is a live
range split (`tryRegionSplit`), not a source-level PHI.

Note also that in the original's gamma block the allocator keeps **both**
`this` (reloaded into `r6`) and `dividerOffset` (in `sl`) live, while ours keeps
`this` in `sl` (`mov sl,r5`) and reloads `dividerOffset` from the stack. The two
choices are the same decision seen from opposite sides: the original spills `this`
and keeps `dividerOffset` in `sl`; ours spills `dividerOffset` and reuses `sl` for
`this`.

## 5. Variants tried (all measured on top of the 9240 B source)

Success criterion was a `str r5,[sp,#104]` in the shield block. None produced it;
every variant coalesced the copy back to `r5` and stayed at 9238/9240 B.

| construct | result |
|---|---|
| `Hud *hud = this;` at top / after `savedColor` / before shield / inside shield | 9240 B, coalesced |
| `Hud *hud = this;` used only in gamma / rocket / damage / gamma+rocket+damage | 9240 B, coalesced |
| `Hud *hud = this;` used in the inner shield-if **and** gamma+rocket+damage | 9240 B, coalesced |
| `Hud *hud` assigned `this` in both `hasShieldBar` arms (real PHI), used after | 9240 B, coalesced |
| `Hud *const`, `const Hud *`, `Hud &`, `&this[0]`, `(Hud*)((char*)this+0)`, ternary | 9240 B, coalesced |
| `Hud *volatile` (all uses / gamma only) | 9380 B / 9676 B, worse |
| `Hud **volatile` | 9624 B, worse |
| pointer-select `unsigned int *shieldImage = &this->shieldFrameImage` etc. | 9232 B |
| pointer-select `unsigned short *armorY/armorBarY` (Ghidra shape) | 9232 B |
| `int armorY` / `unsigned short armorBarY` / `unsigned int armorY` | 9238 B |
| `int dividerOffset` → `unsigned short` / `unsigned int` | 9238 B |
| reorder `dividerOffset` after `SetColor` / after armour decls | 9234 B |
| hoist `Layout *layout`, `PaintCanvas *canvas`, `Player *player`, `Status *status` | 9208–9250 B |
| `bool/int hasShield = this->hasShieldBar;` | 9238 B |
| `__builtin_expect(this->hasShieldBar, 0/1)` | 9328 / 9240 B |
| `-mllvm -split-spill-mode=size` (scratch) | moves the *passengers* `str r5` 0x1050→0x1054, no shield spill |
| `-mllvm -split-spill-mode=copy/local/block/region` (scratch) | no `str r5` at all |

Also tried in wave-88 (same result, see `hud-draw-88-8-9332-b_w3814.md`): `Hud *self`
copies, `Hud **selfPtr`, `Hud &self`, `volatile unsigned int savedColor`, hoisting,
`if (this->field_0x470 != 0)`, `SetColor(0xffffffff)` in the shield branch.

## 6. Most important remaining root cause

The `this` spill is a **greedy-allocator live-range split at the shield/armour merge
block**, not a source-level copy. The allocator emits `%2926 = COPY %2925` in both
predecessors; whether it coalesces that copy (ours) or spills it (original) is decided
by the register pressure in the region that follows — specifically whether
`dividerOffset` keeps `sl` through the gamma block (original) or is spilled and `sl`
is reused for `this` (ours).

That in turn is coupled to the stack-slot colouring: the original gives `this` and
`dividerOffset` their own slots (frame 224), ours merges `dividerOffset` with
`&Globals::layout` (frame 216). Matching the original therefore needs **both** the
`this` slot and the `dividerOffset` slot to stop sharing, which is one allocator
decision, not two independent source edits.

The only remaining *semantic* delta in the whole function is the duplicated
`String::~String` in the mission-type-12 branch (section 1). That is the most likely
lever left: it adds a call in the middle of the function and changes the CFG/tail
merge, which can perturb the global allocation. It should be attacked first next
pass; the `this` spill may then fall out for free.

## 7. Reproduce

```
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

(`FN=... verify-fn` is broken: it passes an empty `FN` to `--show`.)
