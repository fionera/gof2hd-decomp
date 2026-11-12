# Small tails — wave 190 (w13407): five-function re-sweep, no size-exact win

Worker `w13407`, branch `wave190-w13407`, base master `f0982704f`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13407`.

Status: **research note, no `src/` change landed.**  The five smallest tails
were re-attacked with fresh source shapes; every candidate either changes the
function size, re-colours the whole function, or loses raw to the landed body.
The tree is clean at the base and `check` is **RATCHET PASS**.

## 0. Baseline (re-confirmed on the committed source)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 | 374/374 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 | 754/754 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **10** | 112 | 4003/4002 |

Gate on exit: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

**Process trap (cost one false measurement this wave).**  A variant that
changes a function's size shifts every later function in `.text`; `fn_diff`
raw for a downstream function then explodes (e.g. `MGame::OnUpdate` read
700 instead of 10 while a `MenuTouchWindow` variant was 4 B short).  Always
restore the base TU, rebuild `link_so`, and re-measure before comparing.

## 1. `Hud::draw` — damage-block anchor is the unique 9332 B shape

The landed unbraced anchor

```cpp
if (hitFlags & 0x24)
    Status **volatile p = &Globals::status;
    this->field_0x4b8 = 300;
```

emits `mov.w r0,#300; it ne; strne.w r8,[sp,#208]; str.w r0,[sl,#1208]`
(14 B) vs the original `itt ne; movne.w r0,#300; strne.w r0,[sl,#1208]`
(10 B).  The 4 B surplus is compensated by padding elsewhere; removing the
anchor gives the exact 10 B block but shrinks the function to 9328 B and
re-colours the whole body (raw 7339).

New anchor forms swept this wave (all size 9332, raw 32 = landed):

| anchor inside `if (0x24)` | raw | size |
|---|---:|---:|
| landed `Status **volatile p = &Globals::status` | **32** | 9332 |
| `volatile short p = (short)(intptr_t)&Globals::status` | 32 | 9332 |
| `volatile int p = (int)(intptr_t)&Globals::status` | 32 | 9332 |
| `volatile char p = (char)(intptr_t)&Globals::status` | 32 | 9332 |
| `volatile long long p = (long long)(intptr_t)&Globals::status` | 7120 | 9336 |

The short/int/char forms do **not** shrink the store: clang emits the wide
4-byte `strhne.w`/`strbne.w` because the stack slot is `sp+208` (offset > 62),
so the damage block stays 14 B and the bytes are identical to the landed
shape.  A function-scope `volatile short/int/char hudDummy` assigned inside
the `if` (to force a low stack slot) changes RA: raw 382 at 9332 B.

Dock-block `r5`/`r6` swap (13 B): re-swept `top`/`y`/`barYOffset`/`y2`
declaration orders and `short`/`unsigned short`/`unsigned int` types for
`top`/`y`; every variant is byte-identical to the landed body (raw 32) or
inflates it (raw 33/96/110).  The landed shape remains best.

## 2. `Generator::createMission` — bonus block still needs the `bv` store

The 40 raw bytes are the 9 B bonus block (`0x97718`) plus the 21 B cleanup
table it shifts (`0x97918`).  The original block is 26 B:

```
adds r5,r1,r6 ; str r1,[sp,#32] ; movs r1,#50 ; mov r0,r5 ; blx idivmod
ldr r0,[sp,#32] ; cmp r1,#0 ; it ne ; subne r5,r6,r0 ; str r5,[sp,#16]
```

The landed shape is 30 B (`cbz` + conditional reload + `bv` store) and keeps
the function at 1996 B / frame 104.  New shapes swept this wave:

| bonus shape | raw | size |
|---|---:|---:|
| landed (`volatile bonus`, `volatile bv`, `volatile rem`) | **40** | 1996 |
| `int remCopy = rem;` use copy, keep `bv` | 672 | 2008 |
| `int remCopy = rem;` use copy, no `bv` | 169 | 2000 |
| `int remCopy = rem;` use copy, `bv = b` | 597 | 2000 |
| non-volatile `rem` + `remCopy`, keep `bv` | 659 | 2004 |

The faithful `it ne` body needs a non-volatile `bonus` (frame 96), which adds
one cleanup landing pad and a pool `nop` (+4 B).  No frame-104 shape removes
the conditional reload; the landed 40 stays.

## 3. `ModStation::ModStation` — 11 B is the near/far rotation tie only

The literal-pool `+0x20` entries seen in earlier sessions are *not* a source
bug: they track the `.rodata` base shift whenever `.text` is 0x20 larger
(a size-changing variant in another TU).  At the committed base `.text` is
size-exact (`0x1337c8`) and the ModStation pool is byte-identical; the 11 raw
bytes are only `0xd5456..0xd546e` (near yaw loaded into `r4` + copied to `r3`
instead of loaded directly into `r3`, with `pitch` copied to `r4`).

New faithful shapes swept this wave:

| near/far shape | raw | size |
|---|---:|---:|
| landed (far x = `pitch` = bits(yawBits)) | **11** | 1008 |
| near y fresh yaw-table load, far x = `pitch` | 11 | 1008 |
| near y fresh, far x = `yaw` (faithful) | 697 | 1000 |
| far x = `yaw` only (faithful) | 43 | 1008 |
| direct float loads, faithful far | 667 | 1008 |
| direct float loads, far `yawTable[race]` | 725 | 984 |
| bit-pun renamed, far `yawTable[race]` | 604 | 984 |

The faithful form swaps the yaw-table pointer loads (`r4=yawIPad`, `r3=yaw`)
and hoists `mov r4,r2` before the yaw load, costing 32 raw bytes.  The landed
11-byte shape is the unique size-exact minimum.

## 4. `MenuTouchWindow::OnTouchBegin` — three RA ties unchanged

Region A (case 8, `ldr.w ip,[sl,#340]` vs `r4`, 2 B), region B/C (case 15
`r3/r6/r2/r5/r4` rotation, 26 B).  New this wave:

| variant | raw | size |
|---|---:|---:|
| landed | **28** | 1896 |
| `int entryW = this->listEntryWidth;` local in case 8 | 62 | 1896 |
| `unsigned int entryW` local | 63 | 1896 |
| parenthesised `(top + entry)` | 28 | 1896 |
| `layout = *(Layout **)&Globals::layout;` reload | 28 | 1896 |
| case-8 second `if` uses `Globals::layout->` directly | 28 | 1896 |
| `(void)&Globals::layout;` after the reload | 28 | 1896 |
| case-15 `Layout *layout = Globals::layout;` pointer | 316 | 1888 |
| case-15 all-`Globals::layout->` direct | 362 | 1892 |
| case-15 `int w = Globals::w;` local | 28 | 1896 |

The `Layout &layout` reference in case 15 is load-bearing for the 1896 B
size: any shape that removes its cached `field_0xc_leftMargin` read drops to
1892 B and re-colours the function (raw 308-362).  The landed shape is best.

## 5. `MGame::OnUpdate` — m23/m122 both live in the m122 block

The two 10 raw bytes are both in the mission-122-124 branch:

* m23 (`0x17e2b6`): original `ldr r2,[r9]` + `ldr r4,[pc]`; ours
  `ldr r4,[r9]` + `ldr r5,[pc]` (the `ldrd r2,r3` itself matches).
* m122 (`0x17f190`): original `mov r4,r0; ldr r0,[r9]`; ours
  `mov r5,r0; mov r0,r4; str r4,[sp,#120]`.

The `Status *st` / `Status *volatile dummy` anchor is load-bearing for both
the 11440 B size and the `r2:r3` ldrd pair.  New shapes swept this wave:

| m122 shape | raw | size |
|---|---:|---:|
| landed (`st` + dummy + `st->getCurrentCampaignMission()`) | **10** | 11440 |
| `Globals::status->getCurrentCampaignMission()` + dummy | 348 | 11440 |
| same + `volatile int dummy = (int)(intptr_t)st` | 348 | 11440 |
| same + `(void)dummy` | 922 | 11444 |
| `volatile short dummy = (short)(intptr_t)st`, fresh radioBase | 921 | 11444 |
| `volatile short dummy`, `st` radioBase | 355 | 11440 |
| `Level *volatile dummy = level` before `nextInt` | 362 | 11440 |
| `Status *volatile dummy = Globals::status` before `nextInt` | 5212 | 11396 |
| `Status *const st` | 701 | 11440 |
| `Status *volatile st` | 1549 | 11444 |
| m23 cast variants (`*(long long*)&field_0x100`, `(char*)+0x100`, `[0]`) | 10 | 11440 |
| m23 `volatile long long` read | 4487 | 11444 |

The landed 10-byte shape remains the unique size-exact minimum.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13407
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN10ModStationC1Ev _ZN10ModStationC2Ev \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv _ZN5MGame8OnUpdateEv
# -> 32 / 40 / 11 / 11 / 28 / 10, all size-exact
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13407/`
(`htry.py`, `gtry.py`, `mtry.py`, `mttry.py`, `mgtry.py`, `insdiff.py`,
`bytediff2.py`, `shift.py`, `hudsweep*.py`, `gsweep*.py`, `msweep*.py`,
`mtsweep*.py`, `mgsweep*.py`, `*.base.cpp`).

## 7. Files changed

* this note only.
