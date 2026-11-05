# MGame::OnTouchEnd (8080 B) — w6707 (wave 119): exhaustive variant sweep, no gate-safe win

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Branch `wave119-w6707`, base master
`47a1396bd` (wave-118 landed result).  This pass is **analysis only**:
no source change landed, because every measured variant that improved
the fuzzy score rotated `.rel.plt` and therefore failed the ratchet by
exactly one `byte_exact`.

## Result

* Baseline (HEAD) is **8080 B / 80.1441 %** (official verify 80.1),
  `.rel.plt` diff **0**, `RATCHET PASS` (byte_exact 4503, linked_exact
  4503, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).
* Best fuzzy variant found: **83.3303 %**, 8080 B — but `.rel.plt` DIFF,
  so `byte_exact` drops 4503 -> 4502 and the ratchet FAILs.
* Best **8080 B + PLT-OK** variant is still the HEAD source
  (80.1441 %).  Every other 8080/PLT-OK combination measured is either
  code-identical to HEAD or lower.
* No source change was committed; this note is the deliverable.

## 1. The hard 8080 B / PLT-order constraint

`byte_exact` is extremely sensitive to (a) the function size and (b) the
`.rel.plt` order:

* the function must be exactly **8080 B** or `.text` shifts and
  `byte_exact` collapses (~3103, ratchet FAIL);
* the original `.rel.plt` order for the four symbols first referenced
  inside OnTouchEnd is
  `resetStartSequenceOver` (0x213a88), `removeObjectives` (0x213a8c),
  `goingToWaypoint` (0x213a90), `removeRoute` (0x213a94).
  The order is first-reference order in the emitted function, so
  `resetStartSequenceOver` must be called at a **lower code offset**
  than the first `removeObjectives`.
* Rotating those four entries changes `MGame::successCheck`'s `blx`
  offsets (it calls `removeObjectives`, `goingToWaypoint`,
  `removeRoute`), so `successCheck` stops being byte-exact: the ratchet
  sees `byte_exact 4503 -> 4502`.  Verified by running `check` on the
  83.3303 % variant.

HEAD emits `reset` at 0x17b160 and the first `removeObjectives` at
0x17b2ee, so the order is preserved.

## 2. Bare-vs-`== true` `hasWon` finding

The original's five `hasWon()` call sites in OnTouchEnd all use the
**bare** form (`cbnz`/`cbz` on the returned `unsigned char`):
0x17adc8 (`cbnz r0`), 0x17af66 (`cbz r1`), 0x17afec...  HEAD instead
spells the OR and the mission-select ternary as `hasWon() == true`,
which emits `cmp r0,#1; beq` and changes the branch-probability
heuristic (50/50 vs the `isZero` 37.5/62.5).  That probability change
is what wave 118 used to move the cold block chain and gain 1.1 pp;
it is a **tuned** shape, not the original source shape.

The faithful bare form (`D1`, `D2` below) is 8084 B and 81.2545 %
(PLT DIFF), i.e. not gate-safe either.

## 3. Block order (original, from Thumb disassembly)

```
hasWon (0x17adc8) -> keepObjectives (0x17ade0) -> mission select (0x17af5e)
-> instant dispatch (0x17af7e) -> campaign dispatch (0x17afa6)
-> continuation (0x17afe2) -> menuTouch block (0x17afd8..0x17b07a)
-> failed branch type12 (0x17b07e) -> reset block (0x17b0aa)
-> freelance body (0x17b0b6) -> changeCredits (0x17b0e2)
-> campaign chain 15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,
   144,155,161,162
-> finishMission removeObjectives (0x17b26c) -> failed cargo (0x17b374)
-> setFreelanceMission 183 (0x17b888) -> shared tail (0x17b8e2)
```

The original campaign chain is emitted in source order and **splits
96 and 127 into two separate branches** (96 at 0x17b71c, 127 at
0x17bc34, between 126 and 134).  `M127` (split) is therefore the
faithful spelling; wave 118's merge is a size/placement tuning.

HEAD's layout differs: it emits the menuTouch block and the
failed-type12 block before the won dispatch, and places `reset` at
0x17b160 (the original has it at 0x17b0aa).  The 83.33 % variant moves
`reset` to 0x17b512, *after* the first `removeObjectives` (0x17b266),
which is what rotates the PLT.

## 4. Measurement table (fuzzy %, size, `.rel.plt`)

Base = HEAD (`git show HEAD:src/game/menu/MGame.cpp`).  `OK` = `.rel.plt`
matches the original.  All numbers re-measured on the HEAD base this
wave.

| variant | size | pct | plt |
|---|---:|---:|---|
| HEAD | 8080 | 80.1441 | OK |
| D1 (bare OR) | 8088 | 81.1532 | DIFF |
| D2 (bare ternary) | 8076 | 78.9474 | OK |
| D1+D2 (bare both) | 8084 | 81.2545 | DIFF |
| X / MI (`isInstantActionMission() == true`) | 8088 | 83.1712 | DIFF |
| B (`bool refinedAny`) | 8088 | 78.0620 | OK |
| M127 (split 96/127) | 8096 | 80.2738 | OK |
| E (instant-first dispatch) | 8072 | 73.3333 | OK |
| **D1+D2+X+B+M127** | **8080** | **83.3303** | **DIFF** |
| D2+X+B+M127+SW2 | 8080 | 83.3303 | DIFF |
| D2+X+M127 | 8080 | 81.6216 | DIFF |
| D2+X+B+M127 | 8092 | 78.4660 | DIFF |
| D1+D2+X+M127 | 8092 | 82.5014 | DIFF |
| D2+X+B+M127+KEEP2 | 8080 | 82.6455 | DIFF |
| D1+D2+X+B+M127+KEEP2 | 8080 | 82.6455 | DIFF |
| D1+D2+M127 | 8080 | 79.9063 | DIFF |
| D2+B+M127+KEEP2 | 8100 | 78.9123 | OK |
| E+MI2 | 8084 | 76.3329 | OK |
| E+MI2+M127 | 8092 | 78.1920 | OK |
| E+MI2+B+M127 | 8092 | 77.3276 | OK |
| E+MI2+D2+M127 | 8088 | 78.1841 | OK |
| FT (`m_nTimeLimit=0` before `changeCredits`) | 8080 | 80.0360 | OK |
| SW (reset as `else if (!won)`) | 8080 | 80.1441 | OK |
| SW2 (inverted, no `== true`) | 8088 | 81.1532 | DIFF |
| RS (standalone `if` reset) | 8104 | 78.7835 | DIFF |
| WS (won branch before failed branch) | 8096 | 64.3256 | DIFF |
| HW / HW2 (hoist `won` before `failed`) | 8108/8104 | 78.48/78.51 | OK |
| EI1 (`expect(isInstantActionMission(),1)`) | 8080 | 73.6349 | OK |
| EI0 | 8080 | 77.6136 | DIFF |
| EC1 (`expect(isCampaignMission(),1)`) | 8088 | 77.1393 | DIFF |
| EC0 | 8096 | 72.1412 | OK |
| EH1 (`expect(won,1)`) | 8076 | 75.7477 | DIFF |
| EH0 (`expect(won,0)`) | 7828 | 60.5045 | DIFF |
| ET1 / ET0 (expect on ternary) | 8080 | 79.7117 | OK |
| C2 (`count != 0`) | 8084 | 74.5628 | OK |
| FCH / FUCH / FINT (`failed` as char/uchar/int) | 8080 | 80.1441 | OK |
| KO / KOT / FA / FB / MC / MIZ / E183 / KEEP1 / RG | 8080 | 80.1441 | OK |

Code-identical-to-HEAD spellings (all 8080 / 80.1441 / OK):
`KO`, `KOT`, `FA`, `FB`, `MC`, `MIZ`, `E183`, `KEEP1`, `RG`,
`SW`, `AA`..`AF`, `T`, `U`, `V`, `W`, `Y`, `Z`, `R`, `FCH`, `FUCH`,
`FINT`.

## 5. Search coverage

* Exhaustive subset enumeration of `{D1,D2,X,B,E,M127,REF}` (128
  combos, wave 118): every 8080 B / PLT-OK result is exactly
  **80.1441 %** (code-identical to HEAD).
* This wave: exhaustive subsets of
  `{D1,D2,MI,B,M127,SW,SW2,KEEP2}` (256 combos, HEAD base) -> the only
  8080/PLT-OK results are `BASE`, `SW`, `D1+SW`, all **80.1441 %**.
  The best 8080/PLT-DIFF results are
  `D1+D2+MI+B+M127` and `D2+MI+B+M127+SW2` at **83.3303 %**.
* This wave: 339 random combinations (size 1-4) from a 50-op pool
  (`AA..AF`, `E`, `B`, `C2`, `D*`, `P*`, `R,T,U,V,W,X,Y,Z`, `M127`,
  `RG,FT,KO,KOT,FA,FB,MC,MI,MIZ,E183,OR1,SEL1,KEEP1,KEEP2,RS,SW,SW2,
  EI1,EI0,EC1,EC0,EH1,EH0,ET1,ET0,MI2`) -> best 8080/PLT-OK is
  80.1441 %; the only 8080/PLT-OK variants that change code at all are
  `FT` (80.0360) and `AE`/`AF` combinations (80.1081/80.0721).

## 6. Dead ends recorded

* `count > 0` -> `count != 0` in the mining refine loop fixes the
  `cmp r9,#0; beq` vs `cmp r9,#-1; bgt` encoding at 0x17a5ca, but
  changes register allocation: 8084 B / 74.56 % (and 80.11 % when
  combined with the 83.33 % ops).
* `failed`/`keepObjectives`/`refinedAny` predicate spellings and
  `bool`/`char`/`unsigned char`/`int` local types: all code-identical
  to HEAD.
* `__builtin_expect` on `hasWon`, `isCampaignMission`,
  `isInstantActionMission`, `failed`, `supernovaChallenge`: none gives
  8080/PLT-OK above 80.1441 %.
* Reordering `m_nTimeLimit = 0` before `changeCredits` (`FT`): 80.0360 %.
* Swapping the won/failed branches (`WS`): 64.33 %.
* Hoisting `won` before the `failed` check (`HW`/`HW2`): 78.5 %.
* Instant-action-first dispatch (faithful `E`, with or without `== true`
  or `M127`): 73.3-78.2 %.
* The 83.33 % variant is unreachable gate-safely: its `reset` block
  lands after the first `removeObjectives`, and pulling it earlier
  (`RG`, `SW`, `RS`, `HW`) either is code-identical to HEAD or changes
  the size.

## 7. Structural state (unchanged)

27 ELF sections, file **2,187,328 B**.  `.text` 0x1337c8 @0x69440,
`.ARM.exidx` 0x6f70 @0x19cc08, `.rodata` 0x41c27 @0x1c9e10 (192 B
larger than the original 0x41b67; the appended `Status::missionCompleted`
rank table is the last `.rodata` input), `.ARM.extab` **0x26290** vs
the original **0x26298** (8 B short: `createShip` -8 only;
`Generator::createMission` is exact at 172 B).  No section or extab
number changed this pass.
