# Small tails — wave 185 / w12919: five residuals re-swept, no `src/` win

Status: **research note, no `src/` change landed.**  Worktree `wt/w12919`,
branch `wave185-w12919`, base master `f390b0e68` (wave-184 harvest 10).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12919`.

The five smallest residuals were re-diffed instruction-by-instruction and
re-swept on the wave-184 tree.  Every target is a genuine clang-7.0.2
`-Oz` register-allocation / stack-colouring plateau: each candidate that
fixes the residual instruction either changes the function size (gate-fatal)
or re-schedules an adjacent block and raises raw.  The tree is left clean at
the base.

## 0. Baselines (re-confirmed on the base link)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 | 374/374 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 | 754/754 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **15** | 111 | 4003/4002 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS** — `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

## 1. `Hud::draw` — 32 = damage 20 + dock 12

### 1a. Damage block (`hitFlags & 0x24`), 20 B

Original vs ours (`0x164184`):

```
O: 164184 tst.w  r1,#36
O: 164188 itt    ne
O: 16418a movne.w r0,#300
O: 16418e strne.w r0,[sl,#0x4b8]
O: 164192 tst.w  r1,#24
O: 164196 itt    ne
O: 164198 movne.w r0,#300
O: 16419c strne.w r0,[sl,#0x4b4]

U: 164184 tst.w  r1,#36
U: 164188 mov.w  r0,#300
U: 16418c it     ne
U: 16418e strne.w r8,[sp,#0xd0]   ; dead volatile address store (anchor)
U: 164192 str.w  r0,[sl,#0x4b8]   ; unconditional
U: 164196 tst.w  r1,#24
U: 16419a it     ne
U: 16419c strne.w r0,[sl,#0x4b4]  ; shares r0
```

Both blocks are 28 B (including the two `tst`s) and the function is size
9332.  The landed anchor is `Status **volatile p = &Globals::status;` (an
unbraced `if` body, so the `field_0x4b8` store stays unconditional).  The
anchor's 4 B store is exactly what keeps the dead `ldr.w r8,[sp,#108]` at
`0x164176` alive; the plain conditional form is 9328 B (the load is DCE'd).
The original has the dead load and two conditional `movne/strne` pairs.

Swept this wave (all size 9332 unless noted; `raw` for the whole function):

| damage shape | raw | size |
|---|---:|---:|
| landed address-store anchor (unbraced) | **32** | 9332 |
| braced non-volatile `Status **p = &Globals::status;` | 7339 | 9328 |
| braced `volatile` anchor | 7091 | 9336 |
| `(void) Globals::status;` / `(void) &Globals::status;` (braced) | 7339 | 9328 |
| `Status *p = Globals::status; (void)p;` | 7339 | 9328 |
| `(void) *(Status ** volatile *) &Globals::status;` | 7092 | 9336 |
| `__builtin_prefetch(&Globals::status)` braced / unbraced | 7091 / 32 | 9336 / 9332 |
| `volatile int x = (int)(uintptr_t)&Globals::status;` braced | 7127 | 9336 |
| `volatile int x;` + conditional store | 7133 | 9336 |
| `Globals::status ? 300 : 300` / comma / `==` self / `!=0` ternary | 7339 | 9328 |
| `field_0x4b8 >= 1 && Globals::status` (wave-163 form) | 9368 | 9368 |
| `field_0x4b8 >= 1 && (Globals::status, true)` etc. | 7339 | 9328 |
| `*(volatile int *)&this->field_0x4b8 = 300;` + plain if | 7446 | 9324 |
| `*(volatile Status **)&Globals::status = Globals::status;` | 7339 | 9328 |

The missing 4 B in every plain shape is exactly the dead reload; every
non-DCE'd use of `&Globals::status` costs a store (4 B) as well as the
reload, and the store is what forces the shared unconditional `mov r0,#300`.
No source spelling found gives the original's dead load *and* the two
conditional `movne/strne` pairs at size 9332.

### 1b. Dock-transfer block, 12 B

The residual is a single consistent `r5`/`r6` swap of the two long-lived
values `&Globals::Canvas` and `top` (`0x16594c`..`0x165a5a`):

```
O: 16594c mov  r5,fp          U: mov  r6,fp          ; Canvas -> r5 / r6
O: 165984 ldrh.w r6,[sl,#0x3e2] U: ldrh.w r5,[sl,#0x3e2] ; top -> r6 / r5
O: 16599e ldr  r0,[r5,#0]     U: ldr  r0,[r6,#0]
O: 1659a6 mov.w r9,r6,lsl#1   U: mov.w r9,r5,lsl#1
O: 1659bc movs r5,#17         U: movs r6,#17
O: 1659be strd r5,r3,[sp]     U: strd r6,r3,[sp]
O: 1659ce add.w r2,r4,r6,lsl#1 U: add.w r2,r4,r5,lsl#1
O: 165a14 ldr  r5,[r0]        U: ldr  r6,[r0]        ; font / Canvas swap
O: 165a18 ldr  r6,[r0]        U: ldr  r5,[r0]
O: 165a1c mov  r0,r6          U: mov  r0,r5
O: 165a1e mov  r1,r5          U: mov  r1,r6
```

Every other instruction in the block is identical.  The compiler copies
`fp` (`&Globals::Canvas`) to a callee-saved register at `0x16594c` before
overwriting `fp` with `barH`; it picks `r6` in our build and `r5` in the
original.  `top` then takes the other register.

Swept this wave (all size 9332 unless noted; `raw` for the whole function):

| dock shape | raw |
|---|---:|
| landed (`int top = field_0x3e2; int y = top*2; int y2 = barYOffset+y;`) | **32** |
| `barH` declared before `barW` (also swaps the two image calls) | 33 (aligned 17) |
| `int y = top + top` / `top << 1` / `2 * top` | 32 |
| `unsigned short top` / `const int top` | 32 |
| `int y = this->field_0x3e2 * 2;` (no `top`) | 32 |
| `y2 = y + barYOffset` / `unsigned y2` / `const y2` | 32 |
| inline `barYOffset + y` at the call | 1477 (9328) |
| progress computed after the `String text` | 32 |
| `String *gt = getText(...)` local | 32 |
| `String text` moved after `barW`/`barH`/`top`/`y` | 1791-1821 (9336-9340) |
| `String text(*getText(...)); text += String(" ");` | 1751 (9320) |
| `PaintCanvas *canvas = Globals::Canvas;` local | 32-48 |
| `top` before the two image calls / between them / after fade / after SetColor | 110 / 96 / 50 / 1492 |

The pair is a greedy-colouring tie: the two values are both live across the
same calls and every source spelling that changes the allocation order also
changes the size or re-schedules the block.

## 2. `ModStation::ModStation()` — 11 B, 5 aligned

Function-relative diff (original vs ours):

```
+0x186  O: f850 3028   ldr.w r3,[r0,r8,lsl#2]   | U: f8cd 80b8  str.w r8,[sp,#184]
+0x18a  O: 2002        movs  r0,#2              | U: f850 4028  ldr.w r4,[r0,r8,lsl#2]
+0x18c  O: f8cd 80b8   str.w r8,[sp,#184]      | U: (same)
+0x18e  O: (same)                              | U: 2002       movs r0,#2
+0x19c  O: 4640        mov   r0,r8              | U: 4623       mov  r3,r4
+0x19e  O: 4614        mov   r4,r2              | U: 4640       mov  r0,r8
```

The original loads `yawBits` (yawTable value) straight into the nearKey
call's `r3`, then copies `pitchBits` (`r2`) to the callee-saved `r4` for
the farKey call.  Ours keeps the farKey value (`pitch` = `yawBits`) in
`r4`, stores the `race` spill first, and inserts `mov r3,r4`.  This is a
greedy-RA/scheduler tie: every source spelling that makes the farKey value
`pitchBits` (the faithful form) re-schedules the whole block to raw 43.

Swept this wave (all size 1008 unless noted):

| shape | raw |
|---|---:|
| landed (no fence, nearKey `(yaw,pitch)`, farKey `pitch`) | **11** |
| farKey first arg `yaw` (pitchBits) | 43 |
| farKey `yaw` + swapped table declarations | 46 |
| farKey `yaw` + swapped nearKey args | 44 |
| farKey `yaw` + swapped `pitchBits`/`yawBits` declarations | 752 (size 1000) |
| farKey `yaw` + `if (Globals::iPad)` table select | 43 |
| farKey `*(const float *)&pitchBits` inline | 43 |
| farKey `yaw` + `(void)pitch` / `float keep=pitch` | 43 |
| `RotationOrder order = ...;` local / `const` | 11 |
| `float zero = 0.0f;` local | 11 |
| `const float pitch` / `const float yaw` / both | 11 |
| `float yawCopy = yaw;` used by nearKey | 11 |
| `Matrix farKey;` declared early / both matrices early | 964 (size 968) |
| `nearKey` declared after the race computation | 803 (size 1000) |
| `const int pitchBits/yawBits` | 11 |
| `volatile` removed from pitch / yaw / both | 701 / 672 / 672 |

The 11 B are a pure allocator tie; `11` remains the raw minimum.

## 3. `MenuTouchWindow::OnTouchBegin` — 28 B

Two independent groups.

### 3a. case 3/8 (2 B)

```
+0x3fe  O: f8da c154  ldr.w ip,[sl,#0x154]   | U: f8da 4154  ldr.w r4,[sl,#0x154]
+0x404  O: 4463       add   r3,ip             | U: 4423       add   r3,r4
```

`this->listEntryWidth` is short-lived and dead in both; the allocator picks
`ip` (original) vs `r4` (ours).  A pure tie-break.

### 3b. case 15 scrollbar (26 B)

```
+0x60c  O: ldr r3,[sl,#0x138]       | U: ldr r2,[sl,#0x138]
+0x612  O: ldr r6,[sl,#0x1e0]       | U: ldr r3,[sl,#0x1e0]
+0x61c  O: ldr r2,[r4,#0]           | U: ldr r6,[r4,#0]
+0x61e  O: ldr r1,[r3,r6,lsl#2]     | U: ldr r1,[r2,r3,lsl#2]
+0x622  O: ldr r5,[r0,#0]           | U: ldr r4,[r0,#0]
+0x628  O: ldr r2,[r2,#40]          | U: ldr r5,[r6,#40]
+0x62a  O: subs r5,r5,r2            | U: subs r4,r4,r5
+0x630  O: subs r0,r5,r0            | U: subs r0,r4,r0
+0x640  O: ldr r1,[r0,r1,lsl#2]     | U: ldr r4,[r6,#12]
+0x642  O: -                        | U: ldr r1,[r0,r1,lsl#2]
+0x644  O: ldr r2,[r2,#0]           | U: -
+0x646  O: ldr r0,[fp]              | U: ldr r2,[r2,#0]
+0x648  O: -                        | U: ldr r0,[fp]
+0x64a  O: ldr r5,[r2,#12]          | U: -
+0x64c  O: ldr r6,[r2,#32]          | U: ldr r5,[r2,#32]
+0x652  O: adds r1,r6,r5            | U: adds r1,r4,r5
+0x672  O: ldr r5,[r2,#40]          | U: ldr r4,[r2,#40]
+0x678  O: add r0,r5                | U: add r0,r4
```

The original reloads `Globals::layout` after the `GetImage2DWidth` call for
both `field_0xc_leftMargin` and `field_0x20_top`; ours caches the pointer in
`r6` across the call and uses it for `field_0xc_leftMargin` (the wave-112
local optimum).  The landed `Layout &layout = *Globals::layout;` is what
keeps the block at size 1896.

Swept this wave (all size 1896 unless noted):

| case-15 shape | raw |
|---|---:|
| landed `Layout &layout` for block-2 leftMargin | **28** |
| all-direct (no local) | 362 (size 1892) |
| local for block-2 top only | 29 |
| local for block-2 top+leftMargin | 308 (size 1892) |
| `Layout *lp = Globals::layout` for block-2 leftMargin | 28 |
| `Layout *lp` for all three blocks | 316 (size 1888) |
| `Layout **lp = &Globals::layout` | 362 (size 1892) |
| `Layout *volatile *lp` | 127 |
| `volatile Layout &layout` | 35 |
| `Layout *volatile lp` | 1752 (size 1916) |
| case-3 `listEntryWidth` local before the second if | 35 |
| case-3 `int lw` local for both ifs | 1417 (size 1892) |
| case-3 expression reorder | 533 |

The all-direct shape has the original's exact pre-RA data flow but greedy
assigns `&Globals::layout` to `fp`, saving 4 B; the local-reference shape
raises register pressure enough to spill the address but caches the value.
The two effects remain coupled.

## 4. `Generator::createMission` — 40 B, frame 104 vs 96

The 40 bytes are 12 same-size substitution runs (no size shift):

```
+0x00c  1  sub sp,#96            vs sub sp,#104
+0x01a  1  str r0,[sp,#92]       vs str r0,[sp,#100]        (canary)
+0x1a2  1  bhi 0x97388           vs bhi 0x973fa             (type>=15 fold)
+0x3a6  1  ldr r0,[pc,#964]      vs ldr r0,[pc,#952]
+0x520  1  str r0,[sp,#16]       vs str r0,[sp,#96]         (bonus=0)
+0x556  1  str r1,[sp,#32]       vs str r1,[sp,#44]         (rem spill)
+0x560  9  ldr/cmp/it/subne/str  vs cbz/ldr/subs/str/str    (predicated subne)
+0x682  1  ldr r1,[sp,#16]       vs ldr r1,[sp,#96]
+0x6f8  1  ldr r0,[sp,#92]       vs ldr r0,[sp,#100]        (canary)
+0x704  1  addeq sp,#96          vs addeq sp,#104
+0x75e  1  b.n 0x9791c           vs b.n 0x97920
+0x760 21  String-D1 cleanup / embedded literal-pool order
```

The current shape (`volatile int bonus`, `volatile int bv`, `volatile int
rem`) is the only one found that reproduces the original's `rawBonus=r6`,
`b=r5` assignment and the `str rem` / reload / predicated `subne` order.
Its price is the frame: the volatile locals occupy `sp+92`/`sp+96` and push
the canary to `sp+100`, so `sub sp,#104` instead of `#96`.

Swept this wave (all size 1996 unless noted):

| shape | raw | frame |
|---|---:|---:|
| landed (`volatile bonus` + `volatile bv` + `volatile rem`) | **40** | 104 |
| `int rem` (plain) | 659 (2004) | 104 |
| `int rem` + `int *remp=&rem` / `int &remr` / `int rem[1]` / struct | 659 (2004) | 104 |
| `int rem` + `volatile int *remp=&rem`, unconditional read | 672 (2008) | 104 |
| `int rem` + `volatile int *remp=&rem`, read inside if | 40 | 104 |
| `volatile int rem` + unconditional `remCopy=rem` | 672 (2008) | 104 |
| `bonus = rawBonus;` as anchor, `bv` removed | 114 | 104 |
| `int bonus` + `volatile int bv` | 758 (2000) | 104 |
| all plain (`int bonus`, no `bv`, `int rem`) | 181 (2000) | 96 |
| `int bonus; volatile int *bonusPtr=&bonus;` | 40 | 104 |
| `int bonus; volatile int &bonusRef=bonus;` | 40 | 104 |
| `int bonus` + `volatile int dummy` | 722 (2008) | 104 |

The two structural levers are independent compiler decisions:
(1) frame 96 requires no volatile local, but without one the allocator keeps
`bonus` in `r6` and the `bonus = 0` arm collapses to `movs r6,#0` (4 B
instead of 8 B), shifting every following byte; (2) the `r6`/`r5`
assignment and rem spill require the volatile anchors.  No shape found gives
both.

## 5. `MGame::OnUpdate` — 15 B

Four runs:

```
+0x19e4  2  ldrd r2,r3,[r2,#256]      vs ldrd r3,r5,[r2,#256]   (m23 64-bit)
+0x19e8  1  subs r0,r0,r2             vs subs r0,r0,r3
+0x19ea  1  sbcs r1,r3                vs sbcs r1,r5
+0x19ee  2  mov.w r2,#0 / sbcs.w      vs mov.w r3,#0 / sbcs.w
+0x28b4 10  ldr.w r0,[r9] + blx        vs str r2,[sp,#120] + blx + ldr r0,[sp,#120]
```

The first three are the m23 64-bit idle-time compare
(`getPlayingTime() - field_0x100 > 3600000`).  The last is the m122 radio
body: the `Status *volatile st` anchor keeps `&Globals::status` in `r9`
(as the original) but spills `st` to `sp+120`, replacing the original's
single `ldr.w r0,[r9]`.

Swept this wave (all size 11440 unless noted):

| shape | raw | aligned |
|---|---:|---:|
| landed (`volatile st` + `Level *level` before ccm) | **15** | 111 |
| wave-163 `Level *level` before ccm, direct `Globals::status` | 16 | 6 |
| `this->level` direct (no level local) | 15 | 111 |
| `Level *level` after ccm / `Level &` / ternary / cast | 5196 (11392) | 597 |
| `Level *level = radioBase ? this->level : this->level;` | 15 | 111 |
| `if (radioBase==3164) level=this->level;` | 15 | 111 |
| `(Level*)(uintptr_t)this->level` | 15 | 111 |
| `Status *st` plain (no volatile) | 918 (11436) | 109 |
| `Status *st` plain + direct level | 918 (11436) | 109 |
| m122 jump-condition swap | 17 | 115 |
| m122 jump conditions first | 4641 (11436) | 427 |
| m122 jump before time | 4600 (11432) | 425 |
| m122 mission first | 55 | 119 |
| m122 time before mission | 3608 | 378 |
| m23 swap 2/3 | 35 | 119 |
| m23 swap 1/2 | 5364 (11516) | 1163 |
| m23 swap 3/4 | 4971 (11436) | 297 |
| outer guard jump/cutscene swap | 17 | 115 |

The m23 tie is a side effect of the volatile `st` frame slot: the wave-163
shape (no volatile local) makes m23 byte-exact but hoists `this->level`
before the `getCurrentCampaignMission` call (16 raw).  The volatile shape
keeps the level load after the call but spills `st` (15 raw).  No shape
found gives both; `15` is the raw minimum.

## 6. Gate

No `src/` change.  Full `check` on the base: **RATCHET PASS** —
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj _ZN10ModStationC1Ev _ZN10ModStationC2Ev \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN5MGame8OnUpdateEv
# raw=32/11/11/28/40/15, sizes 9332/1008/1008/1896/1996/11440
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12919/`
(`diff.py`, `sweep_mod*.py`, `sweep_hud_*.py`, `sweep_gen*.py`,
`sweep_ou*.py`, `Hud.base.cpp`, `ModStation.base.cpp`, `MTW.base.cpp`,
`Generator.base.cpp`, `MGame.base.cpp`).
