# Small tails — wave 189 / w13366: five-target re-sweep, no new size-exact win

Status: **research note, no `src/` change landed.** Worktree `wt/w13366`,
branch `wave189-w13366`, base master `30caae150`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13366`.

The five smallest size-exact residuals were re-derived from the original
disassembly with a fresh semantic/operand lens.  Every candidate that fixes a
residual either changes the function size, re-colours the whole body, or loses
to the landed shape.  The tree is left clean at the base.

## 0. Baseline (re-confirmed on the base link)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 | 374/374 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 | 754/754 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **10** | 112 | 4003/4002 |

Raw is GOT/layout-sensitive; every number below was measured with *all five*
targets at base source.

## 1. `ModStation::ModStation()` — the faithful farKey operand is a raw loss

The landed source computes `float yaw = bits(pitchBits); float pitch =
bits(yawBits);` and passes `pitch` (bits(yawBits)) as the farKey x.  The
original passes **bits(pitchBits)** (`d546e mov r4,r2` copies
`r2 = pitchTable[race]`; `d54f0 mov r2,r4`).  The one-token semantic fix
(`MatrixSetRotation(farKey, yaw, ...)` with the landed names) is raw **43** /
aligned 12 / 1008 / 374 — the correct nearKey register colouring
(`ldr.w r3` for yawBits, `mov r4,r2` for pitchBits) but the table-pointer
loads land in swapped registers (`r4=yawIPad, r3=yaw` vs the original
`r3=yawIPad, r4=yaw`) and `mov r4,r2` is scheduled 7 instructions early.

New cells this wave (all 1008 B / 374 insn unless noted):

| shape | raw | aligned |
|---|---:|---:|
| landed (farKey = bits(yawBits)) | **11** | 5 |
| farKey `yaw` (faithful, landed names) | 43 | 12 |
| faithful + `float pitch` declared first | 47 | 15 |
| faithful + `yawTable` declared first | 49 | 15 |
| faithful + inline `*(const float *) &pitchBits` | 43 | 12 |
| faithful + `const float &` bit-pun refs | 43 | 12 |
| faithful + `yawBits` loaded first | 751 | 40 (size 1000) |
| inline table expressions (no pointer locals) | 749 | 41 (size 1000) |
| `int`/`unsigned`/`uint32_t`/`long`/`int32_t race` | 11 | 5 |
| `short race` | 727 | 62 (size 1000) |
| `volatile int race` | 912 | 139 (size 1016) |
| `race` declared before `Matrix nearKey` | 11 | 5 |

The 11-byte residual is the greedy-RA tie on the `ldr.w r3`/`ldr.w r4` load
destination plus the `mov r4,r2`/`mov r3,r4` copy; the faithful form moves the
tie into the table-pointer loads and the copy schedule.  11 remains the floor.

## 2. `Generator::createMission()` — the faithful body is instruction-identical but +4 B

The landed body (`volatile int bonus; volatile int bv;` + `bv = rawBonus;`)
is 744 insn / 1996 B.  The semantically faithful body (`int bonus;`,
`volatile int rem`, `int remCopy = rem`) is **instruction-for-instruction
identical** to the original (frame 96, same bonus block
`ldr r0,[sp,#32]; cmp r1,#0; it ne; subne r5,r6,r0; str r5,[sp,#16]`) but
**2000 B / 746 insn**: one extra cold `b.n` landing-pad stub in the
sp+68-String cleanup group and the literal-pool alignment `nop` it forces.
The LSDA has the same 26 rows; only the pad offsets shift (rows 10/13/14/17/
23/24).  Decoder: `_work/reconstructions/extab_lsda_decode.py`.

New cells this wave (all size-exact unless noted):

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| landed (`volatile bonus` + `bv`) | **40** | 21 | 1996 | 744 |
| faithful (`int bonus` + `remCopy`) | 90 | 31 | 2000 | 746 |
| `int bonus` + plain `int rem` | 181 | 116 | 2000 | 745 |
| `int bonus` + `volatile rem` + `remCopy` | 90 | 31 | 2000 | 746 |
| `volatile bonus` + `remCopy` (no `bv`) | 169 | 113 | 2000 | 746 |
| `int bonus` + `bv` + landed block | 758 | 171 | 2000 | 746 |
| `volatile bonus` + `bv = rawBonus` (anchor on bonus) | 114 | 96 | 1996 | 744 |
| `volatile bonus`, no `bv` | 582 | 126 | 1996 | 744 |
| `volatile short`/`char bv` | 605 | 52 | 2000 | 745 |
| `int bonus` + `remCopy` + `bv` | 672 | 147 | 2008 | 748 |
| ternary `b = (b%50) ? rawBonus-rem : b` | 40 | 21 | 1996 | 744 |
| `nextInt` split (`int randomCost = ...`) | 347 | 51 | 1996 | 744 |
| `uint32_t t = type` (first if only) | 40 | 21 | 1996 | 744 |
| `uint32_t *tp = &t` (first if only) | 40 | 21 | 1996 | 744 |
| `(int) type < 15` | 41 | 21 | 1996 | 744 |
| nested `if (type < 15) { if (mask) ... }` | 40 | 21 | 1996 | 744 |
| `__builtin_expect(mask, 1)` on first if | 40 | 21 | 1996 | 744 |
| `__builtin_expect(mask, 0)` on first if | 1546 | 228 | 1988 | 739 |
| `__builtin_expect(type<15, 0)` | 1886 | 623 | 2020 | 728 |
| `__builtin_expect(type<15, 1)` | 1816 | 250 | 1960 | 734 |

The residual is the bonus-block frame (96 vs 104), the `cbz` vs
`cmp/it/subne` if-conversion, the `bhi` fold (`type > 14 ⇒ type != 13`), and
the cold landing-pad ordering.  The faithful body is the only shape that
matches the original's bonus block, and it cannot shed the +4 B EH stub.

## 3. `Hud::draw()` — the damage anchor is load-bearing and cannot be cleaned

The landed damage block has the unbraced volatile-pointer anchor, so ours
emits `mov.w r0,#300; it ne; strne r8,[sp,#208]; str.w r0,[sl,#1208]` for
0x24 and reuses `r0=300` for 0x18.  The original if-converts both blocks
(`itt ne; movne r0,#300; strne r0,[sl,#1208/1204]`).  Removing the anchor
re-colours the whole function (frame 224→216, 3226→3216 insn, 9332→9328 B).
New cells this wave:

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| landed unbraced anchor | **32** | 21 | 9332 | 3226 |
| `__builtin_expect(hitFlags & 0x24, 0/1)` on the anchor if | 32 | 21 | 9332 | 3226 |
| `int c = 300;` + anchor (reuse `c`) | 43 | 31 | 9332 | 3226 |
| `Status **volatile p;` declared before, assigned in the if | 382 | 377 | 9332 | 3226 |
| `Status **volatile p = &Globals::status;` before both ifs | 7134 | 829 | 9336 | 3227 |
| anchor after both ifs | 7111 | 823 | 9336 | 3227 |
| anchor on the 0x18 if | 7075 | 425 | 9336 | 3227 |
| braced anchor | 7091 | 423 | 9336 | 3227 |
| `*(volatile int *) &field = 300` (clean conditional store) | 7339 | 928 | 9328 | 3216 |
| `volatile int pad; pad = 0;` at function end | 8898 | 1046 | 9296 | 3201 |
| `volatile int pad[2]` at end | 8898 | 1046 | 9296 | 3201 |
| `Status **volatile pad` at end | 8920 | 1032 | 9284 | 3197 |
| `volatile int pad; pad = 0;` before the damage ifs | 8904 | 1002 | 9292 | 3200 |

The anchor's volatile store is what pins the 224-byte frame and the 3226-insn
allocation; every anchor shape that cleans the damage block either changes the
frame or re-colours the tail.  32 remains the floor.

## 4. `MenuTouchWindow::OnTouchBegin()` — case-15 fresh vs cached layout

The original reloads `Globals::layout` after the first `GetImage2DWidth` call
and reads both `field_0xc_leftMargin` (`[r2,#12]`) and `field_0x20_top`
(`[r2,#32]`) from the fresh pointer; the landed shape keeps the block-entry
`Layout &layout` live across the call and uses it for `field_0xc`.  New cells
(all 1896 B / 754 insn unless noted):

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| landed (local for `field_0xc`, direct for top) | **28** | 16 | 1896 | 754 |
| local for top, direct for `field_0xc` | 29 | 15 | 1896 | 754 |
| fresh `Layout *l`/`Layout &l` for both fields | 308 | 73 | 1892 | 752 |
| fresh `PaintCanvas *canvas` + cached layout | 360 | 80 | 1892 | 752 |
| fresh `PaintCanvas *canvas` + fresh fields | 359 | 75 | 1892 | 752 |
| all-direct (no local) | 362 | 76 | 1892 | 752 |

Using the fresh pointer for both fields costs 4 B (the original's
`ldr r2,[sp,#16]; ldr r2,[r2,#0]` pair vs ours `ldr.w r2,[fp]`) and drops the
function to 1892.  The landed 28 is the floor.

## 5. `MGame::OnUpdate()` — m23 is a pure greedy-RA tie

The 10-byte residual is exactly m23 (4 B: `ldr.w r2,[r9]`/`ldr r4,[pc]` vs
ours `ldr.w r4,[r9]`/`ldr r5,[pc]` and `ldrd r2,r3,[r2]` vs `[r4]`) plus m122
(6 B, w13354's cell).  New m23 cells (all size-exact):

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| landed m23 | **10** | 112 | 11440 | 4003 |
| parenthesised `(a - b) > 3600000` | 10 | 112 | 11440 | 4003 |
| `3600000LL` | 10 | 112 | 11440 | 4003 |
| `3600000 < a - b` | 10 | 112 | 11440 | 4003 |
| `field < getPlayingTime() - 3600000` | 4955 | 426 | 11436 | 4001 |
| `Status *volatile st23` before the m23 if | 6192 | 961 | 11340 | 3940 |
| `Status *volatile st23` used in the condition | 6247 | 933 | 11344 | 3942 |
| `volatile int a23` before the m23 if | 6248 | 933 | 11344 | 3942 |

The m23/m122 ties are independent greedy-RA choices; the landed 10 is the
floor.

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
`.text` diff **73,554**, `.ARM.extab` 610 (size/base exact at `0x1a3b78`),
build-id 20, total **74,184**; file size 2,187,328 exact.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13366/`
(`sdiff.py`, `bytediff.py`, `meas.sh`, `frame.py`, `sw_*.py`, `*.base.cpp`).
