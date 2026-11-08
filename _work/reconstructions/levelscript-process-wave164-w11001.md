# LevelScript::process — wave 164 / w11001: mission-0 switch case order + tbh tables

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11001`, base `b3d3a1f68` (wave 163).  Scratch
`/dev/shm/w11001/`.

## Baseline (HEAD, unchanged)

```
process fuzzy (official)  50.3 %   (report.json / verify.py)
process raw diffbytes     54,541
process aligned insns     17,427
orig / ours size          57232 / 57232   (size-exact)
.text diffbytes           84,652
.rel.plt                  byte-identical to the original
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

Note: the task brief quotes "57.4 % fuzzy" — that is the *stale* value in the
checked-in `report.json` (last regenerated at `1a4008de4`).  A fresh `verify`
on this base reports **50.3 %**.  The raw/aligned numbers in the brief are
current.

## Original dispatch tables (decoded from the literal pool)

All `tbh` bases are `tbh_addr + 4` (Thumb PC); `target = base + 2*entry`.

### Mission-0 state switch — `switch (m_nState)` states 6..15

`0x139b16: ldr r1,[sl,#28]; subs r0,r1,#6; cmp r0,#9; bhi 0x13f906;`
`0x139b22: tbh [pc,r0,lsl#1]`  (10 entries, table @0x139b26)

```
raw:      0025 1a7e 1b0b 1b5c 1a28 1bd7 1c49 1c8d 1ce7 1cd0
targets:  6->0x139b70 7->0x13d022 8->0x13d13c 9->0x13d1de 10->0x13cf76
          11->0x13d2d4 12->0x13d3b8 13->0x13d440 14->0x13d4f4 15->0x13d4c6
physical order (by target address):  6, 10, 7, 8, 9, 11, 12, 13, 15, 14
```

The default (`bhi`) is `0x13f906` = the `if (m_nState == 16)` handler
(`cmp r1,#16; bne 137f7c`), i.e. state 16 is the switch default, not a case.
The case bodies' `break` goes to the shared active tail `0x137f7c`; the shared
tails are `0x13d4ba` (set state + zero timer; cases 7/13), `0x13d4ec`
(set state only; cases 12/15), `0x13d592` (zero timer; case 10) and
`0x137f7c` (cases 8/11/14).

### Outer arm chain (mission 0, before the switch)

The original tests **4 first, then 3, then 5** and falls through into the
state-3 body:

```
0x1389c8: cmp r1,#4 ; beq.w 0x138d2a      ; state-4 body (out of line)
0x1389ce: cmp r1,#3 ; bne.w 0x138fd8      ; -> state-5 test
0x1389d4: <state-3 body>                  ; inline, falls through
0x138fd8: cmp r1,#5 ; bne.w 0x139b1a      ; -> switch
0x138fde: <state-5 body>                  ; inline
0x139b1a: <switch 6..15>
```

so the source order is `if (4) … else if (3) … else if (5 && msg12->isOver()) … else switch`.

### Outer campaign-mission switch (unchanged, verified)

Linear compare chain order (original == ours):
`14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,114,125,154,157,158`.

Range prong `131..145`, `tbh @0x138a8a` (15 entries, base 0x138a8e):
```
raw:      000f 61d4 61d4 61d4 0399 61d4 61d4 61d4 05e7 61d4 61d4 055b 61d4 05a9 0434
physical: 131,135,145,142,144,139, (132,133,134,136,137,138,140,141,143 -> tail)
```
Range prong `64..73`, `tbh @0x138c3e` (10 entries, base 0x138c42):
```
raw:      000a 60fa 60fa 0628 60fa 056f 06b0 60fa 60fa 05f9
physical: 64,69,73,67,70, (65,66,68,71,72 -> tail)
```
Both range tables' entry order and physical order are **identical** in our
build (ours `tbh` sit at `0x138a8a` and `0x138c40`; only the 2-byte table
base shift differs).  The outer mission switch is structurally matched.

## Why the mission-0 physical order cannot be reproduced from source order

Clang sorts the `switch` case blocks in the IR: `LS.ll`/`LS435.ll` both emit
`switch i32 %state [ i32 6, label %959; i32 7, label %1005; … ]` with the
blocks in value order regardless of the source `case` order.  Reordering the
`case 6..15` labels (all 5 permutations measured) is **codegen-neutral**
(identical raw/aligned/size).  The physical order is therefore decided by
`MachineBlockPlacement` on the CFG, which depends on the *body contents*
(which still differ from the original), not on the source label order.

## Measured shapes (all full links, `tools/fn_diff.py` + `verify.py`)

| shape | fuzzy | raw | aligned | size | rel.plt | byte_exact |
|---|---:|---:|---:|---:|---|---:|
| **HEAD 4,5,3** (13-entry tbh 3..15) | **50.3** | **54,541** | 17,427 | 57,232 | **identical** | **4507** |
| 4,3,5 (10-entry tbh 6..15, matches original table) | 49.2 | 55,204 | 18,010 | 57,240 (+8) | differs (1 swap) | 2741 |
| 3,5,4 (12-entry tbh 4..15) | **50.6** | 54,921 | **17,328** | 57,232 | differs (1 swap) | 4503 |
| 3,4,5 (10-entry tbh 6..15) | 49.2 | 55,316 | 18,015 | 57,256 (+24) | differs (1 swap) | — |
| 5,4,3 (12-entry tbh 4..15) | 46.2 | 55,372 | 19,580 | 57,240 (+8) | differs (5 swaps) | — |
| 5,3,4 | 46.2 | 55,372 | 19,580 | 57,240 (+8) | differs (5 swaps) | — |
| 4,3,5 + `m_bCinematicBreak=0` removed | 49.2 | 55,152 | 18,008 | 57,236 | differs | — |
| 3,5,4 + `m_bCinematicBreak=0` removed | 50.6 | 54,930 | 17,326 | 57,228 | differs | — |
| 4,5,3 + `Status *&status` MPES | 50.3 | 54,542 | 17,424 | 57,232 | identical | 4507 |
| 4,3,5 + `Status *&status` MPES | 49.2 | 55,205 | 18,005 | 57,240 | differs | — |
| 3,5,4 + `Status *&status` MPES | 50.6 | 54,922 | 17,323 | 57,232 | differs | — |
| nest5 (state-5 body nested in `if (state==5)`) | 48.1 | 54,925 | 18,989 | 57,232 | differs | — |
| 4,5,3 + `expect(state==4,1)` | 27.0 | 55,690 | 28,100 | 57,424 | differs | — |
| 4,5,3 + `expect(state==5,0)` | 49.0 | 55,090 | 18,062 | 57,248 | differs | — |

`rel.plt` diff for the 2-line cases is exactly one swap:
`PlayerEgo::getSpeed` moves from after `Player::removeAllGuns` to before
`PlayerEgo::setFreeLookMode` (the state-5 arm is emitted before the state-4
arm in every non-4,5,3 order).  That swap is what drops `byte_exact`
4507 -> 4503 (MGame::successCheck / switchCamera / dialogueEvent /
gameOverCheck each lose 3-4 raw bytes at their `blx` PLT offsets).

## Conclusion

* The original mission-0 switch is `switch (m_nState)` **states 6..15**, with
  physical case-body order **6, 10, 7, 8, 9, 11, 12, 13, 15, 14**; the outer
  arm chain is source order **4, 3, 5**.
* The 4,3,5 shape reproduces the 10-entry `tbh` (the original's table shape)
  but is **+8 B** and a fuzzy/raw regression (49.2 / 55,204).
* The fuzzy-best shape is 3,5,4 (50.6 %, aligned 17,328) but it has a
  12-entry `tbh` (state 4 merged into the switch), is **not** the original
  table shape, and rotates one `.rel.plt` slot (byte_exact 4503).
* The only shape that keeps `.rel.plt` byte-identical, `byte_exact` 4507 and
  size 57,232 is the HEAD 4,5,3 shape.  No size-exact, gate-green shape
  improves the fuzzy, so **nothing was landed**; the tree is left at HEAD.

Reproduce any row with `/dev/shm/w11001/gen.py plain <order>` (rewrites the
mission-0 arm order in `src/game/world/LevelScript.cpp`) followed by
`cmake --build cmake-build-match --target link_so` and
`python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi`.

## `.rel.plt` compensation (for a future pass)

The non-4,5,3 arm orders rotate one or five `.rel.plt` slots because the first
reference to an imported symbol moves with the block layout:

* `4,3,5` (and `3,4,5`): one swap — `PlayerStation::setVisible` moves earlier.
  Reordering the mission-125 arms from `6,0,7` to **`0,6,7`** restores
  `.rel.plt` byte-identity (measured: `4,3,5` + `0,6,7` -> rel.plt OK,
  fuzzy 49.1, raw 55,393, size 57,240).
* `3,5,4`: one swap — `PlayerEgo::getSpeed` moves before
  `PlayerEgo::setFreeLookMode` (the state-5 arm is emitted before the state-4
  arm).  The mission-125 reorder does **not** fix this one.
* `5,4,3` / `5,3,4`: five swaps.

So a future pass that wants to land `4,3,5` must (a) find a -8 B size
compensation (the dead `m_bCinematicBreak = 0` in the mission-2 3001 block is
worth -4 B) and (b) apply the mission-125 `0,6,7` reorder.  Even then the
fuzzy is 49.1 (< HEAD 50.3), so it is a structural-match-only change.
