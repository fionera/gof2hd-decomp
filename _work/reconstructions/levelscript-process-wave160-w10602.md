# LevelScript::process — wave 160 / w10602: case-16 state-0 `m_nState++`

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w10602`, base master `08af4633b` (wave 159).
Scratch `/dev/shm/w10602/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,055            55,047            (-8)
process aligned insns    17,059            17,077            (+18)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,722            85,714            (-8)
.ARM.extab diffbytes     1,256             1,257             (+1)
.ARM.extab size          156,312           156,312           (exact)
.ARM.exidx               0                 0                 BYTE-IDENTICAL
.rel.plt                 0                 0                 BYTE-IDENTICAL
.rodata / .data / .bss   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
RATCHET                  PASS              PASS
```

## Source change (1 edit)

`src/game/world/LevelScript.cpp:1775` (case 16, the `msg[2]->isTriggered()`
state-0 arm):

```c++
- m_nState = 1;
+ m_nState++;
```

The original emits `adds rX,#1; str rX,[this,#28]` (increment of the live
state) at this transition, not `movs rX,#1`.  This is the "`m_nState++` vs
`= N+1`" lever from the mission brief.  The change is semantically identical
(the arm is guarded by `m_nState == 0`), and the function stays size-exact
because the added `ldr` is absorbed by the literal-pool packing.

## State-machine audit (deliverable)

Enumerated every `m_nState` transition in the original and compared the
compare/branch chains:

* The top-level mission if-chain program order is **identical** in both
  builds: 0, 1, 81, 78, 89, 106, group, then 5, 26, 50, 51, then the
  per-mission switch guard.  The only difference is *block placement*
  (original lays the `m_bStartSequence` + mission-5/26/50/51 chain at
  `0x137f7c`-`0x13879c`, before the mission-81 block at `0x1381b2`; ours
  places mission-81 first).  Re-nesting the chain (`else if` -> `if`, or
  moving it inside the mission-chain `else`) changes the size (+92 / +16 B)
  and is gate-fatal.
* The per-mission `switch (mission)` compare chain is **identical**: the two
  range prongs (`131..145`, `64..73`) then 14,16,21,24,29,40,41,42,56,80,
  91,92,94,102,105,114,125,154,157,158.  All 15 `tbh` jump tables are
  present in both builds in the same relative body order.
* The m_nState dispatch chains checked (mission 0 `cmp 2/1/2`, mission 158
  `cmp 3/2/1`, mission 154 `tbh` states 1..10, mission 157, mission 92
  `tbh` states 1..7) all have identical arm order and condition polarity.
* The original's `adds rX,#1` (state increment) sites are matched by our
  source at lines 2908, 4034, 4502; the case-16 site above was the one
  remaining `movs #1` that should have been an increment.

## Measured, not landed (all size-exact unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 55,055 | 17,059 | 57,232 |
| **case-16 `m_nState++` (landed)** | **55,047** | **17,077** | **57,232** |
| case-92 state-2 `m_nState = 10` -> `m_nState++` | 55,215 | 17,045 | 57,248 (+16, fatal) |
| case-92 state-2 `m_nState = 10` -> `m_nState = 3` | 55,055 | 17,059 | 57,232 (neutral) |
| mission-0 state-0 loop: drop `if (!enemies->data_) continue;` | 55,116 | 17,075 | 57,224 (-8, fatal) |
| mission-0 state-0 loop: drop guard + local `e` | 55,467 | 21,292 | 57,332 (+100, fatal) |
| mission-5 `else if` -> separate `if` | 54,967 | 18,146 | 57,324 (+92, fatal) |

The aligned metric is explicitly indicative for this 57 KB function (the
greedy alignment re-syncs differently after any block shift); the raw
same-address byte diff is the primary metric here.

## Open questions

1. The original's mission-chain block placement (5/26/50/51 before 81) is
   not reproduced by the current source nesting.  A `__builtin_expect` on
   the mission-81/78/89/106/group guards might move the blocks without
   changing the size; not enough time to sweep this wave.
2. The case-92 state-2 arm transitions with `adds` (2 -> 3) in the original
   but our source writes `m_nState = 10`; changing it to `++` costs +16 B
   (literal-pool repack).  A companion that reclaims 16 B would land it.
