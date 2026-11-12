# Small tails — wave 189 / w13328: five-target re-sweep, no new size-exact win

Status: **research note, no `src/` change landed.** Worktree `wt/w13328`,
branch `wave189-w13328`, base master `cbcdc0bc4` (wave-189 notes).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13328`.

The five smallest size-exact residuals were re-diffed and re-swept:
`Hud::draw` (32), `Generator::createMission` (40), `ModStation` C1/C2 (11),
`MenuTouchWindow::OnTouchBegin` (28), `MGame::OnUpdate` (10).  Every
candidate that fixes a residual either changes the function size, moves the
literal pool, or re-colours the whole function.  The tree is left clean at
the base.

## 0. Baseline (all other TUs at base)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 | 374/374 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 | 754/754 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **10** | 112 | 4003/4002 |

**Raw is GOT/layout-sensitive.**  Changing another TU's data (even at the
same size) shifts the GOT and changes the PC-relative literal-pool words
that `fn_diff.py` counts.  E.g. with the wave-186 faithful
`Generator::createMission` shape (size 1996, same size but different data)
in the link, `ModStation`'s raw reads 17 instead of 11, and `Hud::draw`'s
reads 481 instead of 32.  Every number below was measured with *all five*
targets at their base source; a candidate must be re-measured on a clean
base link before it is believed.

## 1. `ModStation::ModStation()` — 11 B, nearKey load destination

The residual is entirely the nearKey rotation (the farKey pool words are
identical):

```
orig: 186 ldr.w r3,[r0,r8,lsl#2]   ours: 186 str.w r8,[sp,#184]
      18a movs r0,#2                    18a ldr.w r4,[r0,r8,lsl#2]
      18c str.w r8,[sp,#184]            18e movs r0,#2
      ...                               ...
      19c mov r0,r8                     19c mov r3,r4
      19e mov r4,r2                     19e mov r0,r8
```

The original loads the second table value straight into the call's `r3`
and copies `r2` to `r4` for the farKey call; ours loads into `r4` and
inserts `mov r3,r4`.  `r3` and `r4` are both dead at the load, so this is
a greedy-RA tie.  New shapes measured this wave (all 1008 B, 374 insns):

| shape | raw |
|---|---:|
| landed (bit-pun locals) | **11** |
| bit-pun inline in the call | 11 |
| `float yaw, pitch;` + `__builtin_memcpy` from the int locals | 11 |
| `__builtin_memcpy` straight from the table entries | 672 (375 insns) |
| `const float &yaw` / `const float &pitch` references | 11 |
| non-`const` float bit-pun | 11 |
| `float yaw, pitch; yaw = ...; pitch = ...;` split statements | 11 |
| helper `static inline float bitsToFloat(int)` | 11 |
| `(void) pitchTable;` after the call (keep `pitchTable` live) | 11 |
| `if (pitchTable == yawTable) return;` after the call | 11 |
| `volatile const float *vp = pitchTable;` after the call | 11 |

The load destination is not source-expressible: the `__builtin_memcpy`
and helper forms are inlined to the identical base code.

## 2. `MenuTouchWindow::OnTouchBegin()` — 28 B, two allocation ties

Case 3/8 (`0x401`): `this->listEntryWidth` into `ip` (orig) vs `r4`
(ours) — 2 B.  Case 15 (`0x640`): the original reloads `Globals::layout`
and reads both `field_0xc_leftMargin` (`[r2,#12]`) and `field_0x20_top`
(`[r2,#32]`) from the fresh pointer; ours caches the block-entry value in
`r6` for `field_0xc` — 26 B.

New case-15 shapes measured this wave (all 1896 B, 754 insns):

| shape | raw | aligned |
|---|---:|---:|
| landed `Layout &layout` used for block-2 `field_0xc` | **28** | 16 |
| `Layout *lp = Globals::layout` used for block-2 `field_0xc` | 28 | 16 |
| `const Layout *lp` used for block-2 `field_0xc` | 28 | 16 |
| `lp` used for block-2 `field_0x20_top`, direct for `field_0xc` | 29 | 15 |
| `Layout &layout` used for block-2 `field_0x20_top`, direct for `field_0xc` | 29 | 15 |
| all-direct (`Globals::layout` for all three uses) | 362 | 76 (size 1892) |
| `lp` for block-1 `buttonInsetX`, direct for both block-2 fields | 362 | 76 (size 1892) |

The 29-byte shape moves the cache from `field_0xc` to `field_0x20_top`
(the fresh layout is used for one field, the cached value for the other),
so the two fields cannot both be fresh without the all-direct shape, which
keeps `&Globals::layout` in `fp` across the `GetImage2DWidth` call and
loses 4 B.  The local reference is what forces `&Globals::layout` to be
reloaded at `0x63e`; it cannot be removed without the 4-byte size loss.

## 3. `Hud::draw()` — 32 B, damage anchor + dock `r5`/`r6` swap

Damage block (`hitFlags & 0x24`) is still the unbraced
`Status **volatile p = &Globals::status;` anchor (19 B): the original is
the clean `itt ne; movne r0,#300; strne r0,[sl,#0x4b8]`, ours hoists
`mov.w r0,#300` and adds `strne r8,[sp,#0xd0]` plus an unconditional
`str.w r0,[sl,#0x4b8]`.  Dock block is the pure `Canvas`/`top` `r5`/`r6`
swap (13 B).

New shapes measured this wave (all 9332 B, 3226 insns unless noted):

| shape | raw |
|---|---:|
| landed damage anchor | **32** |
| `(void) *(Status *volatile *) &Globals::status;` in the `0x24` if | 38 |
| `(void) *(Status **volatile *) &Globals::status;` | 38 |
| `volatile int dummy = 300;` in the `0x24` if (braced/unbraced) | 3056 (3218 insns) |
| `volatile int dummy = this->field_0x4b8;` | 7469 (size 9352) |
| `volatile int dummy = hitFlags;` | 7354 (size 9328) |
| `volatile Status *dummy = Globals::status;` | 7494 (size 9320) |
| `Status *volatile dummy = Globals::status;` | 911 |
| `volatile unsigned int *dummy = &this->field_0x4b8;` | 3056 |
| dock `unsigned short top` / `PaintCanvas *canvas = Globals::Canvas` | 32 |
| dock `volatile int top` / `volatile int barYOffset` | 959 / 956 |
| plain damage (no anchor) | 7339 (size 9328, 3216 insns) |
| plain damage + dock `volatile int dockDummy = top` | 7346 (size 9328) |

The damage anchor's store is doing global register-allocation work; no
no-code or scalar anchor reproduces it.  The dock pair is a greedy
colouring tie with no source-order lever.

## 4. `Generator::createMission()` — 40 B, frame 104 vs 96

The landed shape keeps `volatile int bonus` and the dead `volatile int bv`
anchor: frame 104, bonus block
`cbz; ldr r0,[sp,#44]; subs r5,r6,r0; str r6,[sp,#92]; str r5,[sp,#96]`.
The original is frame 96 with
`ldr r0,[sp,#32]; cmp r1,#0; it ne; subne r5,r6,r0; str r5,[sp,#16]`.

The wave-186 faithful shape (`int bonus`, `volatile int rem`,
`int remCopy = rem`) is instruction-for-instruction identical in the body
but **2000 B**: one extra `.ARM.exidx` landing pad (`b.n` at `0x97934`,
a duplicate of the `sp+80` agentName cleanup) plus the literal-pool
alignment `nop`.  New this wave:

| shape | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| landed (`volatile bonus` + `bv`) | **40** | 21 | 1996 | 744 |
| faithful (`int bonus`, `volatile rem`, `remCopy`) | 90 | 31 | 2000 | 746 |
| `rem` in a nested `{}` block | 90 | 31 | 2000 | 746 |
| `int remCopy = *(volatile int *) &remVal` | 90 | 31 | 2000 | 746 |
| `int b2 = cond ? rawBonus - remCopy : b` | 90 | 31 | 2000 | 746 |
| nextInt result hoisted to `int extra` | 382 | 53 | **1996** | 744 |
| `int costs; { int tenth = ...; costs = ... nextInt(tenth); }` | 382 | 53 | **1996** | 744 |
| `int extra` + `float costsF` | 382 | 53 | **1996** | 744 |
| `int tenth` outside the block | 90 | 31 | 2000 | 746 |
| `__builtin_expect(mask, 0)` on the mask test | 1730 | 289 | 1996 | 745 |
| `__builtin_expect(mask, 1)` on the mask test | 40 | 21 | 1996 | 744 |
| lambda / inline helper around `nextInt` | 90 | 31 | 2000 | 746 |

The two 1996-B shapes (`extra`/`tenth`) remove the extra landing pad but
change the `reward`/`rem`/`Mission` schedule around `0x5b4..0x60a`
(`ldrd r0,r1,[sp,#32]` and `fp`/`r9` register swaps), so raw is 382.

The `bhi.n` at `0x1a2` still targets `0x973fa` (ours) vs `0x97388`
(original): `type > 14` implies `type != 13`, so clang folds the
`race < 4 && type == 13` test away.  `__builtin_expect(mask, 0)` moves the
target to `0x973ea` (closer) but costs 1730 raw / 1 insn.  No condition
spelling tried (nested ifs, `<= 14`, `type < 15`, `!= 0U`, mask-swapped,
`type == 13 && race < 4`, `bool` temporaries, `__builtin_expect`
placements/values) changes the fold without changing the size.

## 5. `MGame::OnUpdate()` — 10 B, two independent allocation ties

Landed m122 anchor:
`Status *st = Globals::status; int radioIndex = nextInt(2);
Status *volatile dummy = st; int radioBase = st->getCurrentCampaignMission()...`
gives `mov r5,r0; mov r0,r4; str r4,[sp,#120]` vs the original
`mov r4,r0; ldr.w r0,[r9]` (6 B).  m23 is
`ldr.w r4,[r9]; ldr r5,[pc]; ldrd r2,r3,[r4,#256]` vs the original
`ldr.w r2,[r9]; ldr r4,[pc]; ldrd r2,r3,[r2,#256]` (4 B).

New shapes measured this wave (all 11440 B unless noted):

| shape | raw | aligned | insns |
|---|---:|---:|---:|
| landed anchor | **10** | 112 | 4003 |
| `Status *volatile st = Globals::status;` (volatile pointer local) | 15 | 111 | 4003 |
| `volatile int dummy = radioIndex;` after `nextInt` | 10 | 112 | 4003 |
| `Status *st` before `nextInt`, direct `Globals::status` load, dummy after `radioBase` | 709 | 117 | 4002 |
| `Status *st` before `nextInt`, direct load, dummy after `createRadioMessage` | 1013 | 128 | 4002 |
| `Status *st` before `nextInt`, direct load, no dummy | 5196 | 597 | 3974 (size 11392) |
| `*(Status *volatile *) &Globals::status` in the m122 call | 5512 | 646 | 3974 (size 11392) |
| `Status *st` before `nextInt`, dummy before `nextInt` | 705 | 222 | 4003 |
| `Status *st` before `nextInt`, dummy before `nextInt`, direct load | 1031 | 129 | 4002 |
| `Status *st` before `nextInt`, `volatile Status **dummy = &st` | 5206 | 599 | 3976 (size 11396) |
| `(void) *(volatile Status **) &st` after `nextInt` | 708 | 221 | 4003 |
| `Status *st2 = *(Status *volatile *) &st` | 708 | 221 | 4003 |
| `Status *volatile st` used via a second `st2` | 704 | 221 | 4003 |
| m23 `local`/`local2` (played/last locals) | 10 | 112 | 4003 |
| m23 `>= 3600001`, `3600000 < (a-b)`, `(long long)` casts, `*(long long *)` | 10 | 112 | 4003 |
| m23 `Globals::status->getPlayingTime() > field + 3600000` | 4948 | 414 | 4001 (size 11396) |

The m122 block's `Status *st` local is load-bearing for the *whole*
function's allocation: removing it re-colours the tail (28 fewer insns,
size 11392), and every direct-load shape that keeps the size exact either
leaves the 2-byte store shift (raw 348-1031) or re-colours globally.  The
m23 tie is independent and resistant to every expression form tried.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN10ModStationC1Ev _ZN10ModStationC2Ev \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv _ZN5MGame8OnUpdateEv
# raw=32/40/11/11/28/10, sizes 9332/1996/1008/1008/1896/11440
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Gate on exit: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.text` 74,009, `.ARM.extab` 639 (size/base exact 156,312 B @ `0x1a3b78`),
`.ARM.exidx` 0, `.rel.plt` 0 (byte-identical), `.rodata` 0, build-id 20,
total 74,668; file size 2,187,328 exact.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13328/`
(`dumpdiff.py`, `ctx.py`, `insdiff.py`, `meas.sh`, `mg*.py`, `modsw*.py`,
`mtwsw.py`, `gen*.py`, `hud*.py`, `*.base.cpp`).

## Harvest on wave-189 master (`8225c71a8`)

Note-only: no size-exact win on any of the five targets.  Current master is
`.text` 73,999 / total 74,658; the five tails are unchanged (Hud 32,
Generator 40, ModStation 11, MenuTouchWindow 28, MGame::OnUpdate 10).
