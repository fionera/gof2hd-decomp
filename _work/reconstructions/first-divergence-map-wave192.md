# First-divergence map of the 15 residual functions — wave 192

Method: disassemble each function from both binaries over its exact size, normalise
operands (absolute addresses -> `X`), and find the first instruction index whose
mnemonic/operand shape differs. Base master `1f44e07f2`. Tool: `/tmp/firstdiv.py`
(regenerate from this note; addresses from `.dynsym`).

| function | first divergence | instructions |
|---|---:|---:|
| `Layout` C1 | **+0x4** (insn 2) | 11,399 |
| `LevelScript::process` | **+0xa** (insn 9) | 137,741 |
| `Radar::draw` | +0x2c | 15,734 |
| `Status::missionCompleted` | +0xb8 | 2,061 |
| `Gun::calcCharacterCollision` | +0xde | 2,560 |
| `Level::createShip` | +0x106 | 4,240 |
| `ModStation` C1 | +0x184 | 1,605 |
| `SpaceLounge::drawLounge` | +0x188 | 2,480 |
| `Level::createMission` | +0x2d4 | 36,788 |
| `MenuTouchWindow::OnTouchBegin` | +0x400 | 2,583 |
| `MGame::OnTouchEnd` | +0x486 | 12,571 |
| `Hud::draw` | +0x5f6 | 14,513 |
| `HangarWindow::render` | +0x604 | 15,415 |
| `Radar` C1 | +0xbf5 | 3,644 |
| `MGame::OnUpdate` | +0x28bc | 27,098 |

**Read this as a work order**: every function is size-exact, so the first divergence is
the root of the whole raw-byte cascade. Fix the earliest ones first; a correct prologue
can collapse tens of thousands of raw bytes.

## Layout C1/C2 (+0x4) — one extra callee-saved register

```
ORIG  d1028  push {r4,r5,r6,r7,lr}
      d102a  add  r7, sp, #12
      d102c  stmdb sp!, {r6,r7,r8,r9,sl,fp}     <-- saves r6
      d1030  ldr  r1, [pc,#716]
      ...
      d1036  ldrb.w sl, [r1]                    <-- flag in sl
      d1046  vld1.64 {d0-d1}, [r1]

OURS  d1028  push {r4,r5,r6,r7,lr}
      d102a  add  r7, sp, #12
      d102c  stmdb sp!, {r7,r8,r9,sl,fp}        <-- no r6
      ...
      d1036  ldrb.w lr, [r1]                    <-- flag in lr
      d1046  vld1.64 {d2-d3}, [r1]
```

The original keeps **one more value live in r6** and uses **sl** where we use **lr**;
the NEON constant-pool register allocation is also shifted (d0-d1 vs d2-d3). All three
follow from the missing r6 live range. The likely cause is a local in the original's
constructor that we do not have (or one of ours that we do). Look for a value the
original keeps across the whole body: e.g. a cached `Globals::layout`/`this` pointer or
a loop-invariant scalar. The `+0x4` position makes this the cheapest row in the table.

## LevelScript::process (+0xa)

See `levelscript-prologue-wave192.md`: the `Level *level` local is wrong, and the
remaining prologue difference is a pure `sl`/`fp` swap of `this` vs `Globals::globals`.

## Layout C1/C2 follow-up: the scalar stores differ, not just the registers

Dumping the first 0x240 bytes of both constructors shows the scalar constant stream
diverges at the **third** immediate (excluding `mvn`):

```
ORIG  movs r6,#161  movw r5,#369  movw r8,#590  mov.w ip,#70  mov.w lr,#35  mov.w r9,#7
OURS  movs r6,#161  movw r5,#369  mov.w sl,#35  mov.w ip,#21  movw fp,#562  mov.w r8,#5  mov.w r9,#2
```

Only 161, 369 and 35 are common; the original's 590 / 70 / 7 have no counterpart in
ours, and ours has 21 / 562 / 5 / 2 the original does not. The immediate multisets over
the whole function differ too (139 distinct vs 144; e.g. 590, 90, 106, 111, 115, 124,
131, 138, 168, 236 only in the original; 562, 600, 800, 840, 420, 400, 360, 300, 281,
243, 220, 210, 189, 147 only in ours).

The store sequence confirms it is structural, not allocation:

| field | original | ours |
|---|---|---|
| `0x74` | `strd r6,r5` (161, 369) | `strd r6,r5` (161, 369) -- matches |
| `0x2c0` | `strd r3,r6` (-2, 161) | **no store at 0x2c0**; only `str.w r6,[r0,#0x2c4]` |
| `0x8c` | `strd r6,r4` | `strd r6,r3` (second value differs) |
| `0xa4` | `strd r9,r3` | `str.w r5,[r0,#0xa4]` + `str.w sl,[r0,#0xa0]` |
| `0x2b4` | `strd lr,r1` | `str.w sl,[r0,#0x2b4]` |

So the `Layout` constructor needs its scalar field writes **re-derived from the binary**
(the values and some store positions are wrong), before any register-allocation work.
The vector stores (`vld1.64`/`vst1.32` from the byte-identical `.rodata`) already match.

## Radar::draw (+0x2c) — one callee-saved register

First 0x2c bytes are identical. The first difference is a single word:

```
ORIG  12e07a  mov.w fp, #0
OURS  12e07a  mov.w sl, #0
```

Both `fp` and `sl` are saved in the prologue; this is a pure allocator choice for a
zero-initialised local, and everything downstream inherits the shift. Same class of
problem as LevelScript's sl/fp swap.

## Status::missionCompleted (+0xb8) — switch jump-table body order

The divergence is inside the compiler-emitted switch jump table (halfword branch
offsets), not in straight-line code:

```
ORIG (from 0xabfe8): 0098 009b 00b5 00b8 00c4 01a2 01b7 00cd 023a 01d1 023a 00fb
                     01e8 01f6 01f8 012e 0028 01fc 020b 0160
OURS (from 0xabfe8): 0098 009b 00b5 00b8 00c4 00cd 00fb 012e 023a 0160 023a 0167
                     0184 01a2 01b7 01d1 0028 01e8 01f6 01f8
```

Ours is essentially ascending (case bodies laid out in case-value order); the original's
is not. Decoding the original's entry -> body mapping and sorting by body offset gives
the original's **source order of the case bodies**:

```
case 16, 0, 1, 2, 3, 4, 7, 11, 15, 19, 5, 6, 9, 12, 13, 14, 17, 18, 8/10(shared)
```

So the fix is to reorder the `switch` arms in `Status::missionCompleted` to that order
(the bodies move, the table entries follow). Do not relabel cases -- reorder the arms.

## Correction: inline literal-pool words

The naive scan decodes inline literal-pool words as instructions. Filtering out
`<UNDEFINED>` and impossible shifts (`lsrs/lsls/asrs ..., #32`) changes one row:

- `Gun::calcCharacterCollision` first divergence is **+0xe8** (insn 115), not +0xde
  (the `+0xde` word is a pool entry).

The other rows are unaffected (their first differing word is real code), e.g.
`createShip` +0x106 is `lsls r0,r5,#4` vs `ldmia r5,{r3,r5}` and `ModStation` +0x184 is
`str r6,[r0,r1]` vs `ldmia r5!,{r1,r2,r6}`.

## SpaceLounge::drawLounge (+0x188) — stack slot

```
ORIG  170ec1  str r2, [sp, #576]   ; 0x240
OURS  170ec1  str r2, [sp, #704]   ; 0x2c0
```

The surrounding instructions match; only the spill slot differs (0x240 vs 0x2c0), i.e. a
local's stack-home assignment, not a source-order issue.
