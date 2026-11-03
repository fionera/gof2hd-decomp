# Radar::draw — the "missing 124 bytes" is two tail-merges + block placement (w4223)

**Status:** research note, 2026-11-03. Worktree `wt/w4223`, branch `wave92-w4223`,
base `b379d00d`. No source change landed: no variant that keeps `Radar::draw` at
exactly 9908 bytes beats the baseline's verifier ratio. This commit contains this
note only.

## Baseline (confirmed before and after)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`Radar::draw` = `usize 9908`, official ratio **0.739255** (73.9255 %),
normalized stream 3492 (original) vs 3488 (ours). `Radar.cpp` is compiled with
the scoped `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1`
(`CMakeLists.txt:71`).

## 1. The 9784-byte shape (A+B+C) and its call multiset

The w4105/w4202 patch (drop the duplicate `if (!alienOrbit)` autopilot block,
restore the asteroid `dead` predicate, restore the enemy `inSight`
distance-first order) compiles to **9784 B**, ratio **0.610125** (61.0 %),
normalized stream 3492 vs 3441.

The call multiset is the strongest statement-completeness proof available:

| | original | A+B+C | baseline |
|---|---:|---:|---:|
| `bl`+`blx` (raw body) | 354 | 352 | 361 |
| `blx sym:` (normalized) | 342 | 340 | 349 |

The *only* multiset differences between the original and A+B+C are:

```
orig  abc   callee
  9     8   PaintCanvas::DrawString(uint, String const&, int, int, bool)
  7     6   FModSound::play(int, Vector*, Vector*, float)
```

Every other callee matches one-for-one, including the five `String::~String`
calls in the exception-cleanup landing pad at the end of the function
(original `0x1306ae..0x1306e4`, A+B+C `0x13062e..0x130668`). The cold/error
chains are therefore **present**, not missing:

* `otherLock = field_0x14 != 0` cold block — original `0x12ebc2`, A+B+C
  `0x1304d0` (relocated, not absent).
* `otherLock = lockedStation != 0` cold block — original `0x12ebce`, A+B+C
  `0x12f848` (relocated).
* `missionBlocksPlanets = getType() != 172` cold block — original `0x12ebda`,
  A+B+C `0x12f8d8` (relocated).
* `field_0x14 == 0` block (`planetLockTimer = 0; field_0x18 = 0`) — original
  `0x12ec1c..0x12ec22`, A+B+C `0x12e79c..0x12e7a2` (merged into the planet
  tail).
* The 5 `String` temporaries of the `i == 0 && !alienOrbit` landmark branch
  (tec-level text) all exist in both; the cleanup pad has five destructors in
  both.

So A+B+C is **semantically complete**. The 124-byte deficit is code
generation, not a missing statement.

## 2. The two tail-merges account for the two missing calls

### 2.1 Landmark `i <= 1` distance `DrawString`

The original keeps the `else` branch's distance `DrawString` out of line at
`0x12e9e0` and the `i == 0` branch's second one at `0x12eb36`:

```
12e9ba  ldr  r0,[pc,#572]        ; gameText
12e9bc  ldr.w ip,[r4,r9]         ; originX/originY (offset)
12e9c0  add  r0,pc
12e9c2  ldr  r0,[r0,#0]
12e9c4  ldr  r2,[r0,#0]
12e9c6  ldrd r3,r5,[r4,#252]     ; screenX, screenY
12e9ca  ldr  r0,[sp,#140]        ; canvas
12e9cc  ldr  r2,[r2,#4]          ; font
12e9ce  add  r3,ip               ; x = screenX + offset
12e9d0  ldr.w r1,[fp]
12e9d4  add  r2,r5               ; y = screenY + layout[4]
12e9d6  ldr  r0,[r0,#0]
12e9d8  str  r2,[sp,#0]
12e9da  movs r2,#0
12e9dc  str  r2,[sp,#4]
12e9de  mov  r2,r6               ; lockLabel
12e9e0  blx  PaintCanvas::DrawString
```

A+B+C computes the same arguments at `0x12f67e..0x12f6a6` but ends with
`b.n 12f7c2`, sharing the `i == 0` branch's call instruction:

```
12f67e  ldr  r0,[pc,#56]
12f680  mov  r2,r5
12f682  ldr.w r5,[r4,#256]       ; screenY
12f686  add  r0,pc
12f688  ldr.w ip,[r4,#252]       ; screenX
12f68c  ldr.w r1,[r9]
12f690  ldr  r0,[r0,#0]
12f692  ldr  r3,[r0,#0]
12f694  ldr  r0,[sp,#140]
12f696  ldr  r3,[r3,#4]
12f698  ldr  r0,[r0,#0]
12f69a  add  r3,r5
12f69c  str  r3,[sp,#0]
12f69e  movs r3,#0
12f6a0  str  r3,[sp,#4]
12f6a2  add.w r3,ip,r6
12f6a6  b.n  12f7c2             ; shared call
```

The `i == 0` branch's own call setup is at `0x12f79e..0x12f7c2`:

```
12f79e  ldr  r3,[r6,#0]
12f7a0  mov  r2,r5
12f7a2  ldrd ip,r6,[r4,#252]
12f7a6  ldr  r0,[sp,#140]
12f7a8  ldr  r3,[r3,#4]
12f7aa  ldr.w r5,[r4,#556]       ; originX
12f7ae  ldr.w r1,[r9]
12f7b2  add.w r3,r6,r3,lsl #1    ; y = screenY + fontHeight*2
12f7b6  str  r3,[sp,#0]
12f7b8  movs r3,#0
12f7ba  ldr  r0,[r0,#0]
12f7bc  str  r3,[sp,#4]
12f7be  add.w r3,r5,ip           ; x = screenX + originX
12f7c2  blx  PaintCanvas::DrawString
```

The original does **not** share the call; A+B+C does. Same source shape, two
different tail-merge decisions.

### 2.2 Music `FModSound::play`

The original has two separate play tails: `0x1302d2` (the `field_0x1bc >= 1`
block) and `0x130698` (the `field_0x1bc < 1` block):

```
1302c8  movs r1,#0
1302ca  ldr.w r0,[r9]
1302ce  str  r1,[sp,#0]
1302d0  movs r1,#136
1302d2  movs r2,#0
1302d4  movs r3,#0
1302d6  blx  FModSound::play
...
13068c  ldr  r1,[pc,#112]
13068e  movs r2,#0
130690  str  r2,[sp,#0]
130692  add  r1,pc
130694  ldr.w r1,[r1,r5,lsl #2]   ; kRaceMusic[race]
130698  movs r2,#0
13069a  movs r3,#0
13069c  blx  FModSound::play
```

A+B+C shares one play at `0x130286`; the second branch ends with
`b.n 130286` (`0x130626`):

```
13061a  ldr  r1,[pc,#104]
13061c  movs r2,#0
13061e  str  r2,[sp,#0]
130620  add  r1,pc
130622  ldr.w r1,[r1,r8,lsl #2]
130626  b.n  130286               ; shared play
```

## 3. Block-size accounting (reproduced from w4202)

Call-anchored, non-overlapping region sizes (bytes):

| region | original | A+B+C | delta |
|---|---:|---:|---:|
| prologue/init | 708 | 748 | +40 |
| landmark condition + planet block | 1172 | 1128 | -44 |
| landmark loop | 1076 | 1088 | +12 |
| cold tail (`missionBlocksPlanets` + `field_0x14 == 0`) | 72 | 0 | **-72** |
| enemy loop | 2108 | 2096 | -12 |
| gas clouds | 1758 | 1790 | +32 |
| asteroids | 2780 | 2718 | -62 |
| music | 234 | 216 | -18 |
| **total** | **9908** | **9784** | **-124** |

The single largest item is the **cold tail**: the original places the
`missionBlocksPlanets` block and the `field_0x14 == 0` block immediately before
the `enemies:` label (`0x12ebda` / `0x12ec1c`), so they occupy the 72-byte gap
between the landmark loop and `enemies:`. A+B+C moves the
`missionBlocksPlanets` block to `0x12f8d8` (after the gas region) and merges
the `field_0x14 == 0` block into the planet tail, so that 72-byte region
disappears. It is relocation, not deletion: the instructions exist elsewhere.

The baseline (with the duplicate autopilot block) is the best-scoring shape
precisely because its block layout is within ~0x44 bytes of the original
throughout:

| anchor | original | baseline | A+B+C |
|---|---:|---:|---:|
| planet `getStations` | 0x12e3c2 | 0x12e3c4 | 0x12e3ce |
| top `getAutoPilotTarget` | 0x12e6f4 | 0x12e6d8 | 0x12e6f4 |
| duplicate `getAutoPilotTarget` | — | 0x12e732 | — |
| landmark `isVisible` | 0x12e7f4 | 0x12e7c4 | 0x12f498 |
| cold `Mission::getType` | 0x12ebdc | 0x12eb98 | 0x12f8d8 |
| enemy `KIPlayer::getType` | 0x12ee90 | 0x12eeac | 0x12ea20 |
| gas `getSparks` | 0x12f460 | 0x12f454 | 0x12efd4 |
| asteroid `isMinable` | 0x12fb3e | 0x12fb5a | 0x12fb12 |
| music `play` tail | 0x1302d2 / 0x130698 | 0x1302d4 / 0x13069c | 0x130286 (merged) |

The baseline's `field_0x14 == 0` block is at `0x12ec2e..0x12ec34` (original
`0x12ec1c..0x12ec22`, -0x12); its `lockedStation` otherLock cold block is at
`0x12eb8a` (original `0x12ebce`, -0x44); its `missionBlocksPlanets` cold block
is at `0x12eb98` (original `0x12ebda`, -0x42). Only the *first* `otherLock`
cold block is far away: baseline `0x1304d0`, original `0x12ebc2`.

## 4. Branch-weight sweep (baseline source)

`-mllvm -likely-branch-weight=L -mllvm -unlikely-branch-weight=U`:

| L:U | usize | ratio |
|---|---:|---:|
| none | 9908 | 0.724825 |
| 20:2 / 40:2 / 60:2 | 9904 | 0.708698 |
| 40:1 | 9908 | 0.722819 |
| 60:1 / 64:1 / 70:1 | 9908 | 0.738109 |
| 72:1 | 9908 | 0.739255 |
| **74:1** | 9908 | **0.739255** |
| 76:1 / 78:1 | 9908 | 0.725398 |
| **80:1** | 9908 | **0.739255** |
| 82:1..96:1 | 9908 | 0.739255 |
| 104:1 | 9908 | 0.725398 |
| 110:1 | 9908 | 0.739255 |
| 120:1 | 9908 | 0.725111 |
| 150:1 / 200:1 / 500:1 / 1000:1 | 9908 | 0.738968 |
| 2000:1 | 9908 | 0.724825 |
| 80:2 | 9908 | 0.722819 |
| 80:4 / 80:8 | 9904 | 0.708698 |
| 80:16 | 9904 | 0.721697 |
| 80:32 | 9900 | 0.725032 |

80:1 is a broad plateau (74, 80..96, 110), not a unique magic value. No
weight beats 0.739255.

## 5. Measured source variants (all with the baseline's 80:1 flag)

Every variant below is the baseline `Radar.cpp` plus the listed edit, measured
with the official verifier (`autojunk=False`). Only variants that stay at
`usize 9908` are gate-landable; none beats 0.739255.

| variant | usize | ratio | note |
|---|---:|---:|---|
| baseline | 9908 | **0.739255** | |
| `Sprite *blip` local, planet block only | 9892 | 0.718669 | size shift |
| `Sprite *blip` local, both blip blocks | 9888 | 0.720207 | size shift |
| scoped `blip` local inside each frame branch | 9908 | 0.739255 | neutral |
| `otherLock` `\|\|` form, no expect | 9908 | 0.721697 | |
| `otherLock` `&&` form, no expect | 9908 | 0.721697 | |
| `otherLock` nested-if form | 9908 | 0.721697 | |
| `otherLock` `expect(A\|\|B\|\|C,1)` | 9908 | 0.739255 | neutral |
| `otherLock` per-condition `expect(==null,1)` | 9912 | 0.707825 | |
| `otherLock` as `bool` | 9876 | 0.678151 | |
| inverted landmark `i<=1` condition (else first) | 9908 | 0.738682 | |
| separate `if (i != 0 \|\| alienOrbit)` after the `i==0` branch | 9908 | 0.739255 | neutral |
| `offset` inlined at both call sites | 9920 | 0.734355 | |
| `inSight` distance-first (C) | 9924 | 0.711894 | size shift |
| `inSight` x-pair/y-pair, distance last | 9936 | 0.707111 | |
| `inSight` distance hoisted to a `float dist` local | 9920 | 0.630102 | size shift |
| asteroid `dead` reorder (B) | 9928 | 0.712466 | size shift |
| A+B+C (w4105 patch) | 9784 | 0.610125 | size shift |

The `Sprite *blip` experiments are informative: the original's blip blocks use
`ldr.w r5,[r4,#244]; mov r0,r5` (base `r4`, pointer in `r5`, later reloads
`ldr.w r0,[r4,#244]`), while the baseline uses
`mov r5,r4; ldr.w r0,[r5,#244]!` (writeback, later reloads `ldr r0,[r5,#0]`).
A local `Sprite *blip` gives the original's first two instructions but then
keeps the pointer live across `setFrame`/`setRefPixelPosition`/`draw`, saving
three reloads and shrinking the function by 16 bytes per block. The original
does *not* keep it live (it reloads `[r4,#244]` at `0x13050a` and `0x130522`),
so the local is not the original source shape either.

## 6. Conclusion

* A+B+C (9784 B) is **semantically complete**: every callee matches the
  original except the two tail-merged `DrawString` and `play` calls; the
  exception-cleanup pad has the same five `String::~String` calls; every cold
  block exists.
* The 124-byte deficit is **block placement plus two tail merges**, not a
  missing statement, helper call, or loop body.
* The baseline is a local optimum: its layout is within tens of bytes of the
  original from the planet block through the asteroid loop, and its music
  `play` tails already match the original's last three call addresses exactly
  (`0x1303ea`, `0x13046c`, `0x13069c`).
* No size-neutral source variant tested beats 0.739255. Landing a semantically
  correct shape requires reproducing the original's cold-tail placement
  (`0x12ebc2..0x12ec24`) *without* the duplicate autopilot block, which in turn
  requires a block-placement/codegen match that the current source shapes do
  not reach.

Next pass: the remaining high-value local diffs are (a) the first `otherLock`
cold block (`0x1304d0` vs `0x12ebc2`), (b) the `ldr.w r5,[r4,#244]; mov
r0,r5` vs writeback form in the three blip sites, and (c) the enemy
`and #1` hoist at `0x12f3b6`. All three are codegen/scheduling decisions that
do not change the instruction count, so any of them is a candidate for a
size-neutral strict improvement.

## 7. Evidence locations

Scratch (not committed): `/dev/shm/w4223/` — `Radar.base.cpp`,
`Radar.abc.cpp`, `stream_orig.txt`, `stream_abc.txt`, `stream_base.txt`,
`o_body.txt`, `abc_body2.txt`, `base_body_cur.txt`, `base_smdiff.txt`,
`abc_smdiff.txt`, `sweep_all.txt`, `exact2.py`, `smdiff.py`, `dumpstream.py`,
`try2.sh`, `sweep3.sh`.

---

# Session 2 (w4223, wave 92): the 124 B are codegen, and here is the codegen map

**Status:** research note, 2026-11-03, same worktree/branch. No source change landed:
every variant that keeps `Radar::draw` at 9908 B is neutral or worse than the
baseline's 0.739255. This section is self-contained; it re-states the baseline
and inlines every new measurement.

## 8. Baseline re-confirmed

```sh
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN5Radar4drawEP6PlayerP3Hudi` = `usize 9908`, official ratio **0.739255**,
normalized stream 3492 (original) vs 3488 (ours) → 2580 matched instructions.
`Radar.cpp` compiles with the scoped `-mllvm -likely-branch-weight=80
-mllvm -unlikely-branch-weight=1` (`CMakeLists.txt:70-71`).

## 9. Statement completeness re-proved (call multiset)

Counting `bl`/`blx` with a resolved `<sym>` in the raw objdump of both bodies:

```
total 344 (original) vs 342 (A+B+C)
  9   8  _ZN11AbyssEngine11PaintCanvas10DrawStringEjRKNS_6StringEiib
  7   6  _ZN9FModSound4playEiPN11AbyssEngine6AEMath6VectorES3_f
```

Every other callee occurs the same number of times. So the 9784 B A+B+C shape
(and, a fortiori, the 9908 B baseline) contains every call the original makes;
the deficit is code generation, not a missing statement, helper call or loop
body.

## 10. Where the bytes actually differ: an offset-delta map

Aligning the two bodies with `difflib.SequenceMatcher` on normalized
instructions (addresses/branch targets/`<sym>` stripped) and printing
`(ours_addr - 0x12e050) - (orig_addr - 0x12e050)` at every re-synchronised
run of >4 equal instructions gives the cumulative size gap. Baseline vs
original (abridged; the shape is what matters):

```
O 12e33c / A 12e30c : -0x30     <- otherLock block + constant pool
O 12e3d4 / A 12e3d4 : +0x00     <- re-synced (planet block start)
O 12e47c / A 12e474 : -0x08
O 12e6ee / A 12e6d2 : -0x1c     <- top-level autopilot block
O 12e76e / A 12e72c : -0x42     <- duplicate autopilot block
O 12e7be / A 12e798 : -0x26     <- landmark loop preamble
O 12e864 / A 12e82c : -0x38
O 12e8e4 / A 12e8e4 : +0x00
O 12ea8e / A 12ea4a : -0x44     <- landmark loop body
O 12ebca / A 12eb86 : -0x44     <- cold-block group
O 12ec22 / A 12ec34 : +0x12     <- after the second constant pool
O 12f34c / A 12f37c : +0x30     <- enemy loop
O 12f89a / A 12f8d2 : +0x38     <- gas loop
O 12fca6 / A 12fcba : +0x14     <- asteroid loop
O 130082 / A 130082 : +0x00
O 130532 / A 1304e6 : -0x4c     <- first otherLock cold block (0x1304d0)
O 13066a / A 130672 : +0x08
```

The map is the whole story: the two bodies are within a few tens of bytes of
each other almost everywhere, and the *sign* of the gap flips twice (once at
the cold-block group, once after the second pool). There is no single missing
region — a 124 B "hole" does not exist. What exists is a different block order
and a different stack-slot / register assignment.

## 11. The constant pool is identical; only its placement differs

`Radar::draw`'s first PIC constant pool is five words. Decoding both binaries:

```
original 0x12e340: 000e1c9a 000e2166 000e20bc 000e2224 000e1fda
ours     0x12e310: 000e1c9a 000e2166 000e20bc 000e2224 000e1fda
```

Byte-identical, and the verifier names them the same way
(`pool:pic:got:__stack_chk_guard`, `Globals::Canvas`, `Globals::status`,
`PaintCanvas::DrawImage2D`, `Globals::font`). The pool simply sits `0x30`
earlier in ours. clang places a Thumb constant island after the first
unconditional branch that follows the first `ldr rX,[pc,#N]`:

* original: first `b` after `0x12e064` is `b.n 12e376` at **0x12e33c**;
* ours: first `b` after `0x12e064` is `b.n 12e338` at **0x12e30c**.

So the pool placement is a *consequence* of the `otherLock` block layout, not
an independent difference. Everything downstream that uses `[pc,#N]`
(`ldr r0,[pc,#728]` vs `#680`, `#700` vs `#652`, `#684` vs `#636`,
`#368` vs `#320`, `#152` vs `#104`) then differs by a constant.

## 12. The `otherLock` block: original keeps one shared `movs r5,#1`

Original `0x12e2fc..0x12e314`:

```
12e2fc  mov   r1, r4
12e2fe  ldr.w r0, [r1, #12]!     ; lockedGasCloud (this+0x0c)
12e302  str   r1, [sp, #124]
12e304  cbnz  r0, 12e312         ; != null -> hot
12e306  ldr   r0, [r4, #56]      ; lockedAsteroid (this+0x38)
12e308  cbnz  r0, 12e312
12e30a  ldr   r0, [r4, #28]      ; field_0x1c
12e30c  cmp   r0, #0
12e30e  beq.w 12ebc2             ; all null -> cold block
12e312  movs  r5, #1             ; otherLock = 1
12e314  ldr.w r0, [r4, #312]
```

Ours `0x12e2fc..0x12e338`:

```
12e2fc  mov   r1, r4
12e2fe  ldr.w r0, [r1, #12]!
12e302  str   r1, [sp, #124]
12e304  cbz   r0, 12e324         ; == null -> keep checking
12e306  mov.w r9, #1
12e30a  ldr   r5, [sp, #156]
12e30c  b.n   12e338
12e30e  nop
12e310  <5-word constant pool>
12e324  ldr   r0, [r4, #56]
12e326  ldr   r5, [sp, #156]
12e328  cmp   r0, #0
12e32a  itt   eq
12e32c  ldreq r0, [r4, #28]
12e32e  cmpeq r0, #0
12e330  beq.w 1304d0             ; all null -> cold block (far away)
12e334  mov.w r9, #1
12e338  ldr.w r0, [r4, #312]
```

Two consequences, both visible in the delta map:

1. the hot `otherLock = 1` block is duplicated (two `mov #1`) and the first
   check is inverted (`cbz` instead of `cbnz`), so the block is 0x24 B longer
   and the first unconditional branch moves 0x30 B earlier → the pool moves;
2. the cold `otherLock = field_0x14 != 0` block lands at `0x1304d0` instead of
   `0x12ebc2`, i.e. it is not next to the other two cold blocks
   (`lockedStation` `0x12eb8a` vs `0x12ebce`, `missionBlocksPlanets`
   `0x12eb98` vs `0x12ebda`).

### 12.1 Source variants tried for this block (all `usize 9908` unless noted)

| variant | ratio |
|---|---:|
| baseline `if (__builtin_expect(A && B && C, 0)) { otherLock = f14 != 0; } else { otherLock = 1; }` | **0.739255** |
| `int otherLock = 1; if (expect(A && B && C, 0)) otherLock = f14 != 0;` | 0.739255 (identical bytes) |
| `int otherLock = 0;` + same if/else | 0.739255 (identical bytes) |
| `expect(!(A != 0 \|\| B != 0 \|\| C != 0), 0)` | 0.739255 (identical bytes) |
| `expect(!(A && B && C), 1) { otherLock = 1; } else { otherLock = f14 != 0; }` | 0.739255 (identical bytes) |
| `otherLock = f14 ? 1 : 0` / `f14 == nullptr ? 0 : 1` / `= true` | 0.739255 (identical bytes) |
| `expect(A && B && C, 1)` | 0.707825 |
| condition order gasCloud-first | 0.708632 |
| condition order field_0x1c-first | 0.663134 |
| three separate `__builtin_expect(...,0)` | 0.709677 |
| `\|\|` form `expect(A' \|\| B' \|\| C', 0)` | 0.700933 |
| `\|\|` form `expect(A' \|\| B' \|\| C', 1)` | 0.708632 |
| ternary `otherLock = (A&&B&&C) ? (f14 != 0) : 1` | 0.721697 |
| `int otherLock = 1; if (A && B && C) otherLock = f14 != 0;` (no expect) | 0.721697 |
| three `goto done` guards, `otherLock = 1` initialiser | 0.713138 (9892 B) |
| `bool otherLock` | 0.678151 (9876 B) |

clang canonicalises most of these to the same bytes (verified with a SHA-1 of
the extracted function body), so the `cbz`/`cbnz` split is not reachable from
this source shape.

## 13. The enemy ±24000 chains are three different source orders

There are three `24000.0f` chains. `s18`/`s16` are reloaded at `0x12eccc`
(`s18 = 24000, s16 = -24000`) and again at `0x12f440` (`s16 = 24000,
s18 = -24000`), so the same mnemonics mean different things in the two halves.

| chain | original | ours (baseline) |
|---|---|---|
| enemy in-range `0x12ee36` | `bgt` (`> 24000`) then `bmi` (`< -24000`) at `0x12ee42/0x12ee4c` | `bhi` then `blt` at `0x12ee5e/0x12ee68` |
| gas `0x12f654` | `bgt` then `bmi` at `0x12f660/0x12f66a` | `bgt` then `bmi` — matches |
| lockedEnemy `0x1300ec` | `bmi` (`< -24000`) then `bgt` (`> 24000`) at `0x1300f8/0x130102` | `bgt` then `bmi` at `0x1300ae/0x1300b8` |

`bgt`/`bmi` are the *ordered* forms (`fcmp ogt` / `fcmp olt`); `bhi`/`blt` are
the `fcmp ugt` / `fcmp ult` forms. The original's in-range test is therefore
written as the negation of an OR, not as `<= && >=`:

```cpp
if (!(this->egoPos.x - this->targetPos.x > 24000.0f) &&
    !(this->egoPos.x - this->targetPos.x < -24000.0f) && ... )
```

Measured:

| variant | usize | ratio |
|---|---:|---:|
| baseline | 9908 | 0.739255 |
| in-range chain written `!(> ) && !(< )` (produces `bgt`/`bmi`, exactly like the original) | 9908 | 0.739255 |
| lockedEnemy chain reordered `< -24000` first | 9908 | 0.737536 |
| both | 9908 | 0.737536 |

The `!(...)` rewrite reproduces the original's six branch encodings verbatim
(verified in the disassembly at `0x12ee5e..0x12eea8`), but the metric does not
move because that region is already misaligned by the pool shift — the six
instructions were unmatched before and after.

## 14. VFP register pressure: the original keeps `crosshairPos` in s24/s26

```
original  12e058  vpush {d8-d13}
ours      12e058  vpush {d8-d11}
```

Original `0x12ef4c..0x12ef76` (enemy in-sight test):

```
12ef4c  ldr   r0, [pc, #688]      ; &PlayerEgo::crosshairPos
12ef4e  movs  r6, #0
12ef50  strb.w r6, [r8, #115]     ; enemy->field_0x73 = 0
12ef54  add   r5, sp, #232
12ef56  add   r0, pc
12ef58  ldr   r0, [r0, #0]
12ef5a  vldr  s26, [r0]           ; crosshairPos.x
12ef5e  vldr  s24, [r0, #4]       ; crosshairPos.y
12ef62  mov   r0, r5
12ef64  ldrd  r2, r1, [sp, #108]
12ef68  blx   AEMath::operator-
12ef6c  mov   r0, r5
12ef6e  vcvt.s32.f32 s24, s24
12ef72  vcvt.s32.f32 s26, s26
12ef76  blx   AEMath::VectorLength
```

i.e. `cx`/`cy` are computed **before** the distance and survive the two calls
in callee-saved `s24`/`s26` (hence `d8-d13`). The baseline computes the
distance last, so `cx`/`cy` are `s0`/`s2` and only `d8-d11` are saved.

The "distance-first" variant (C in the previous note) reproduces this exactly:

```
vpush {d8-d13}
12ef76  vldr  s26, [r0]
12ef7a  vldr  s24, [r0, #4]
12ef8a  vcvt.s32.f32 s24, s24
12ef8e  vcvt.s32.f32 s26, s26
```

but it is **9924 B / 0.711894**, and A+B+C is 9784 B / 0.610125. So the
register shape is right and the metric still gets worse — again a layout, not
a semantics, effect.

## 15. The stack frame is the same size; only the slot assignment differs

The set of `[sp,#N]` offsets used by `Radar::draw` is **identical** in both
bodies:

```
0,4,8,12,16,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,
100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,292
```

`sub sp,#296` and `str r0,[sp,#292]` match. Only the *assignment* to
variables differs:

| variable | original slot | ours |
|---|---:|---:|
| `mode` (3rd arg) | `[sp,#100]` | `[sp,#112]` |
| `turretMode` | `[sp,#148]` | `[sp,#152]` |
| `landmarkHit` / `autoPilotTarget` (slot-shared) | `[sp,#152]` | `[sp,#148]` |
| `notTurretMode` | `[sp,#108]` | `[sp,#108]` (match) |
| `notOtherLock` | `[sp,#116]` | `[sp,#116]` (match) |

The landmark-loop preamble is otherwise instruction-for-instruction identical:

```
orig 12e7a8: add.w r0,r4,#376 / str r0,[sp,#132] / add.w r0,r4,#364 / str r0,[sp,#128]
             ldr r0,[sp,#148] / add.w r9,r4,#396 / str.w sl,[sp,#144] / mov.w sl,#0
             eor.w r0,r0,#1 / str r0,[sp,#108] / eor.w r0,r5,#1 / ldr r5,[sp,#156]
             str r0,[sp,#116] / movs r0,#0 / str r0,[sp,#152]
ours 12e786: (same, with #152 and #148 exchanged)
```

Moving the `landmarkHit` declaration above `turretMode`, or moving the
`otherLock` declaration up, produces **byte-identical code** (SHA-1 verified),
so declaration order does not drive `StackSlotColoring` here.

## 16. Everything else tried this session (all `usize 9908`)

| variant | ratio |
|---|---:|
| baseline | **0.739255** |
| `alwaysEnemySeen` as `bool` | 0.715370 (3489 stream) |
| `marked` as `bool` | 0.708901 (3485) |
| `otherLock` as `bool` | 0.678151 (3474) |
| `campaign` as `uint32_t` | 0.739255 (different bytes, same match count) |
| `bossSeen` as `int` | 0.722740 (3487) |
| `landmarkHit` as `int` | 0.702679 (3487) |
| `enemyHit` as `int` | 0.699485 (3496) |
| `planetHit` as `int` | 0.713016 (3484) |
| `wreck` as `int` | 0.718786 (3492) |
| `inSight` as `int` | 0.739255 (identical bytes) |
| `gasHit` as `int` | 0.738968 |
| `asteroidHit` as `int` | 0.688154 |
| `hasDockNav` as `int` | 0.739255 (identical bytes) |
| landmark loop `uint32_t i` | 0.721283 (3490) |
| enemy loop `int i` | 0.739255 (identical bytes) |
| gas loop `int i` | 0.739255 (identical bytes) |
| asteroid loop `int i` | 0.739255 (identical bytes) |
| planet loop `int i` | 0.739255 (identical bytes) |
| `bossSeen` nested `if (shipGroup == 10)` inside the outer `if` | 0.672206 |
| `bossSeen = shipGroup == 10 ? true : false` | 0.726231 (3492/3492 stream) |
| A only (drop duplicate autopilot block) | 0.622903 (3424) |
| A + C | 0.614296 (3433) |
| C only | 0.711894 (3495) |
| C + `!(...)` in-range chain | 0.711894 (3495) |

Note the `campaign`/`inSight`/`hasDockNav`/loop-type variants that are
"neutral" are *byte-identical* to the baseline for most of them; only
`campaign` genuinely changes the emitted code and it still matches exactly
2580 instructions.

## 17. Conclusion of session 2

* The 124 B are **not** missing code. The call multiset is complete; the
  constant pools are byte-identical; the stack frame size and slot set are
  identical; every cold/error block exists.
* The gap is produced by exactly two coupled decisions:
  1. the `otherLock` `cbz`/`cbnz` block shape, which moves the first
     unconditional branch by `0x30` and therefore relocates the first
     constant pool and every `[pc,#N]` operand;
  2. `StackSlotColoring` swapping the `{turretMode}` and
     `{landmarkHit, autoPilotTarget}` slots (`#148`/`#152`), which perturbs
     every `[sp,#N]` instruction in the landmark/planet/enemy blocks.
* Both are invariant under every source spelling tried (17 `otherLock`
  spellings, 14 type spellings, 6 loop-variable spellings, 3 declaration
  orders). clang canonicalises them to the same bytes.
* Two genuine source-fidelity findings that the metric cannot see because
  their regions are already misaligned:
  * the enemy in-range test is the negation of an OR (`!(x > 24000) &&
    !(x < -24000)`), not `x <= 24000 && x >= -24000` — proved by the
    original's `bgt`/`bmi` vs our `bhi`/`blt`;
  * the lockedEnemy chain is ordered `< -24000` before `> 24000` (original
    `bmi` then `bgt`), the opposite of ours.
* The next lever is not a source statement but the *layout*: something must
  make clang keep the three `otherLock` checks in one block with a shared
  `movs r5,#1` fallthrough. Until that happens, no size-neutral source edit
  can raise the ratio above 0.739255.

## 18. Session-2 evidence locations

Scratch (not committed): `/dev/shm/w4223/` — `o_body.txt` (original body
disassembly), `b_body.txt` (baseline body), `abc_body_new.txt` (A+B+C body),
`a_body.txt` (A body), `stream_o.txt`/`stream_u.txt` (verifier streams),
`exact2.py`, `fnsha.py` (function-body SHA-1), `callgaps2.py` (call-anchored
gap table), `histop.py` (mnemonic histogram), `rangecmp.py` (region diff),
`smdiff.py`, `dumpstream.py`, `try2.sh`, and the `v_*.cpp` variant sources
(`v_c1a`, `v_c3`, `v_C`, `v_ol1..v_ol23`, `v_lh1`, `v_lh2`, `vb_*`, `v_X1..3`).
