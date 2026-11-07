# LevelScript::process — wave 131 / w7690 (all-`tbh` body-order audit: case-154 differs)

Status: **note only, no source change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base `7c6933c2d`.

## Result

```
                         before (master)   after (no code change)
raw ratio (difflib)      59.879643905      59.879643905
official verify pct      59.9              59.9
orig / ours insns        20169 / 20045     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100` on
`tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7690/score.py`).

## New hypothesis tested: every `tbh` table's *body order*, not just the two main ones

Wave 130 compared the two main switches' body order and the mission-0 table shape. This pass
decoded **all 15 `tbh` tables** on both sides with the correct `2*halfword` scaling
(`/dev/shm/w7690/tbh2.py`) and compared the case→body permutation of every one. Fourteen of the
fifteen tables have **identical permutations**. The one that differs is new:

```
case 154 state machine (orig tbh @0x13e8f0, ours @0x13a6dc, guard `subs #1; cmp #9`):
  orig body order by address:  1,2,3,4,5,6,7,8,9,10   (ascending)
  ours body order by address:  1,3,5,10,2,4,6,7,8,9
```

(The other tables, including mission 0x4e `12,5,3,10,2,4,8,6,7,9,11`, mission 0 `6..15`
ascending, the two main switches, and all per-mission state machines, match exactly.)

## Experiments (all reverted; function stays 57232 B)

1. **Converted the case-154 `if/else if` chain (states 1..10) to a literal
   `else switch (m_nState) { case 1: ... }`.** Output is **byte-identical** (same 20045 insns,
   same tbh permutation, same 59.879643905). clang already canonicalises the chain to the same
   `SwitchInst`, so *source case order is not a lever* for this switch. (A standalone clang-7.0.2
   `-Oz` probe confirms: a 10-case `switch` emits the same ascending `tbb` body order for both
   ascending and shuffled source case order.)
2. **Removed the bogus `case 6: m_nState = 2; break;` from the case-145 state machine.** The
   original's case-145 dispatch is `cmp #1; beq case1; cbz r4, case0; b -> 0x13cacc`, then a
   `subs #2; cmp #3` `tbh` over states **2..5** — i.e. a 2-cluster switch (compare 0/1 + table
   2..5), while ours is a single `tbh` over states **0..6** (n=7). Removing our extra case 6
   shrinks the table to n=6 (states 0..5) but grows the body to **57236 B** and drops the raw
   score to **58.507596**, so it was reverted. The original's 2-cluster shape is not reproduced
   by any source spelling tried (it needs clang to split {0,1} from {2..5}; our case-0 fallthrough
   into case 1 does not trigger the split).
3. The case-154 permutation is therefore a **MachineBlockPlacement** difference driven by the
   reconstructed bodies' CFG/size, not by source case order. No size-exact spelling moved it.

## Open lead for the next wave

The case-154 state machine is the only remaining `tbh` whose body permutation differs. Its bodies
should be diffed instruction-for-instruction against the original (orig bodies at
`0x142f34,0x1430d6,0x143138,0x1432c2,0x14330e,0x14349c,0x1434c0,0x1434f2,0x14354c` for states
2..10); a body-content difference (extra/missing statement) is the most likely reason
MachineBlockPlacement settles on `1,3,5,10,2,4,6,7,8,9` instead of ascending.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6          # RATCHET PASS, byte 4506
python3 /dev/shm/w7690/score.py                             # 59.879643905
python3 /dev/shm/w7690/tbh2.py <so> <objdump> 0x137d18 0x145ca8
python3 /dev/shm/w7690/tbhctx.py <so> 0x137d18 0x145ca8     # 6-insn guard context
```

Scratch helpers in `/dev/shm/w7690/` (not tracked): `score.py`, `tbh2.py`, `tbhctx.py`,
`diffruns.py`, `LevelScript.base.cpp`.
