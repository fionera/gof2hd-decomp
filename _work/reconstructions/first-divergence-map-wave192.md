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
