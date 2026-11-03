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
