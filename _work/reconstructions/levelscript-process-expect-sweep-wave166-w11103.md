# LevelScript::process — wave 166 / w11103: continued `__builtin_expect` sweep

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11103`,
base master `85c1f88df` (wave 166).  Scratch `/dev/shm/w11103/`.

## Baseline (wave-166 entry, unchanged)

```
process raw diffbytes    54,247
process aligned insns    17,532
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          83,147
.ARM.extab diffbytes     659
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Final (HEAD after this pass)

```
process raw diffbytes    54,095   (-152)
process aligned insns    17,621   (+89)
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          82,995   (-152)
.ARM.extab diffbytes     661     (+2)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
RATCHET PASS
```

Every landed cell was measured with a full `link_so` + `tools/fn_diff.py` +
a section-byte compare (`/dev/shm/w11103/measure.py`, built on
`tools/verify/elf.py`).  `.rel.plt` stayed byte-identical at every step; a
candidate that rotated it was discarded.

## Landed cells (7 commits)

| # | line | guard | e | raw after | aligned after | size | rel.plt | extab |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| 1 | 4626 | `(*enemies)[i]->shipGroup == 9` (case-body filter) | 0 | 54,238 | 17,532 | 57,232 | 0 | 659 |
| 2 | 4920 | `chaser->getPosition().x > ...` | 1 | 54,235 | 17,538 | 57,232 | 0 | 659 |
| 3 | 3122 (2nd op) | `m_nState == 9 \|\| m_nState == 10` -> `m_nState == 10` | 0 | 54,230 | 17,538 | 57,232 | 0 | 659 |
| 4 | 4626+3751 | shipGroup==9 flipped to e=1 **plus** `m_nScriptTimerA >= 110001` | 1,1 | 54,161 | 17,539 | 57,232 | 0 | 659 |
| 5 | 5231 | `m_nScriptTimerA >= 4001` (4 s escort respawn) | 1 | 54,151 | 17,621 | 57,232 | 0 | 657 |
| 6 | 2215+3702 | `isAlwaysFriend()` cold + `i == 3` hot | 0,1 | 54,149 | 17,624 | 57,232 | 0 | 657 |
| 7 | 4550+4613 | two `m_nScriptTimerA >= 1001` cold (size-compensating pair) | 0,0 | 54,119 | 17,644 | 57,232 | 0 | 660 |
| 8 | 3316+4809+5111+5033+2912 | five-cell hot/cold sweep (see below) | 1 | **54,095** | **17,621** | 57,232 | 0 | 661 |

Cells 4 and 7 are **size-compensating pairs**: each member alone changes the
function size (+4/-4 and -12/+12), but together they restore 57,232 exactly.
Cell 4 also required flipping the already-landed `shipGroup == 9` hint from
`e=0` to `e=1` (the pair is `e=1` + `e=1`; the `e=0`+`e=1` variant is 54,190
with size 57,236, and `e=1`+`e=0` is 54,425 with size 57,248).

### Cell 8 detail (five-cell hot sweep)

| line | guard | e |
|---|---|---:|
| 3316 | `m_nState == 1` (mission state machine arm) | 1 |
| 4809 | `m_nScriptTimerB <= 6999` | 1 |
| 5111 | `m_nScriptTimerA < 6000` | 1 |
| 5033 | `messages[14]->isOver()` | 1 |
| 2912 | `m_nState == 1` (second state machine) | 1 |

Leave-one-out on top of the other four: 3316 dropped -> 54,105; 4809 -> 54,104;
5111 -> 54,101; 5033 -> 54,100; 2912 -> 54,096.  Adding `L4028:1`
(`isDockedToDockingPoint() && hackingWon() && ...`) on top of all five is
54,097 (worse than 54,095), so it was not landed.

## Method

A harness (`/dev/shm/w11103/sweep.py`, `batch.py`, `batch2.py`) patches one
source cell, rebuilds the full `.so` (`link_so`, NDK r18b clang 7.0.2, -Oz),
and measures raw/aligned/size/`.text`/`.rel.plt`/`.ARM.extab`.  A full
incremental cycle is ~3.5 s.

Three sweeps were run on top of the wave-164 landed set:

1. **All 116 size-exact PLT-safe cells** from the wave-164 table, one at a
   time (round 1 on 54,247, then re-run on each new base: 54,238, 54,161,
   54,119).  This is how cells 1, 2, 5, 6 and 8 were found; several cells that
   were neutral alone on the wave-164 base become wins only after an earlier
   cell moves the block layout.
2. **All 182 `for`/`while` loop conditions** (e=0 and e=1 each).  Result:
   49 cells are size-exact/PLT-safe, but **none** beats the base (minimum raw
   == base); 133 are size-changing or rotate `.rel.plt`.  Loop-condition
   hints are a dead end on this function.
3. **All 107 multi-operand `if` conditions, last `&&`/`||` operand hinted
   cold** (e=0).  Result: 29 size-exact/PLT-safe, exactly one win —
   `op3122.1:0` (cell 3, -5).  The other 28 are neutral or worse.

A fourth sweep tested **24 size-compensating pairs** (a `+N` size cell plus a
`-N` size cell from the wave-164 size-changing table).  Two pairs landed
(cells 4 and 7); the rest were worse than the base.  The best non-landed pair
is `L4550:0+L4610:0` (54,144, size 57,240, not exact).

## Full tested-cell table (round 1: 116 size-exact PLT-safe cells on 54,247)

All rows are one cell applied to the wave-164 landed set.  `size` is the
function size in bytes; `rel.plt` is the differing-byte count of `.rel.plt`
(0 = byte-identical).  Rows with `size != 57232` are size-changing and were
only landable as part of a compensating pair.

| line | e | raw | aligned | size | rel.plt | extab |
|---|---:|---:|---:|---:|---:|---:|
| 1638 | 0 | 54205 | 17541 | 57236 | 0 | 1349 |
| 4626 | 0 | 54238 | 17532 | 57232 | 0 | 659 |
| 4920 | 1 | 54244 | 17538 | 57232 | 0 | 659 |
| 2215 | 0 | 54246 | 17535 | 57232 | 0 | 659 |
| 3702 | 1 | 54246 | 17532 | 57232 | 0 | 659 |
| 2848 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 4135 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 2493 | 0 | 54247 | 17532 | 57232 | 0 | 659 |
| 2392 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 3066 | 0 | 54247 | 17532 | 57232 | 0 | 659 |
| 2835 | 1 | 54247 | 17533 | 57232 | 0 | 659 |
| 1083 | 0 | 54247 | 17532 | 57232 | 0 | 659 |
| 3910 | 0 | 54247 | 17539 | 57232 | 0 | 659 |
| 4286 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 1335 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 3775 | 0 | 54247 | 17532 | 57232 | 0 | 659 |
| 4598 | 0 | 54247 | 17532 | 57232 | 0 | 659 |
| 3823 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 3027 | 1 | 54247 | 17533 | 57232 | 0 | 659 |
| 3069 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 4399 | 0 | 54247 | 17532 | 57232 | 0 | 659 |
| 4368 | 1 | 54247 | 17532 | 57232 | 0 | 659 |
| 2058 | 0 | 54247 | 17532 | 57232 | 0 | 659 |
| 2805 | 1 | 54248 | 17532 | 57232 | 0 | 659 |
| 1436 | 0 | 54248 | 17532 | 57232 | 0 | 659 |
| 1772 | 1 | 54248 | 17531 | 57232 | 0 | 659 |
| 2171 | 0 | 54248 | 17533 | 57232 | 0 | 659 |
| 2912 | 1 | 54248 | 17532 | 57232 | 0 | 661 |
| 4422 | 1 | 54248 | 17542 | 57232 | 0 | 659 |
| 5125 | 0 | 54249 | 17531 | 57232 | 0 | 659 |
| 4028 | 1 | 54249 | 17532 | 57232 | 0 | 659 |
| 4210 | 0 | 54249 | 17553 | 57232 | 0 | 659 |
| 5033 | 1 | 54249 | 17543 | 57232 | 0 | 659 |
| 3518 | 0 | 54249 | 17570 | 57232 | 0 | 659 |
| 3316 | 1 | 54249 | 17436 | 57232 | 0 | 659 |
| 1288 | 1 | 54251 | 17532 | 57232 | 0 | 659 |
| 1294 | 1 | 54251 | 17532 | 57232 | 0 | 659 |
| 3746 | 0 | 54252 | 17534 | 57232 | 0 | 659 |
| 2816 | 1 | 54253 | 17532 | 57232 | 0 | 659 |
| 4159 | 1 | 54254 | 17508 | 57232 | 0 | 659 |
| 1016 | 1 | 54256 | 17534 | 57232 | 0 | 659 |
| 5111 | 1 | 54257 | 17540 | 57232 | 0 | 659 |
| 3561 | 1 | 54261 | 17534 | 57232 | 0 | 663 |
| 3553 | 1 | 54261 | 17517 | 57232 | 0 | 659 |
| 4316 | 0 | 54264 | 17725 | 57232 | 0 | 659 |
| 5231 | 1 | 54265 | 17614 | 57232 | 0 | 657 |
| 2038 | 0 | 54267 | 17106 | 57232 | 0 | 661 |
| 2562 | 0 | 54275 | 17525 | 57232 | 0 | 659 |
| 1146 | 0 | 54285 | 17532 | 57232 | 0 | 659 |
| 4293 | 0 | 54302 | 17541 | 57240 | 0 | 1348 |
| 4295 | 0 | 54302 | 17541 | 57240 | 0 | 1348 |
| 5142 | 1 | 54302 | 17541 | 57240 | 0 | 1348 |
| 1687 | 0 | 54304 | 18658 | 57248 | 0 | 1347 |
| 4256 | 0 | 54310 | 17531 | 57228 | 0 | 661 |
| 4257 | 0 | 54310 | 17531 | 57228 | 0 | 661 |
| 767 | 1 | 54320 | 17535 | 57232 | 0 | 659 |
| 4091 | 0 | 54330 | 17534 | 57232 | 0 | 659 |
| 4277 | 0 | 54334 | 17545 | 57232 | 0 | 659 |
| 4205 | 1 | 54336 | 17514 | 57232 | 0 | 659 |
| 4102 | 1 | 54347 | 17481 | 57232 | 0 | 659 |
| 2115 | 1 | 54348 | 17540 | 57232 | 0 | 659 |
| 1676 | 0 | 54361 | 17537 | 57232 | 0 | 659 |
| 1118 | 0 | 54372 | 17533 | 57232 | 0 | 659 |
| 1111 | 1 | 54372 | 17544 | 57232 | 0 | 659 |
| 4238 | 1 | 54374 | 17673 | 57240 | 0 | 1348 |
| 5079 | 1 | 54404 | 19267 | 57228 | 0 | 28822 |
| 3401 | 0 | 54421 | 17838 | 57232 | 0 | 659 |
| 3401 | 1 | 54422 | 17846 | 57232 | 0 | 659 |
| 3350 | 0 | 54422 | 17537 | 57232 | 0 | 659 |
| 5033 | 0 | 54441 | 17584 | 57232 | 0 | 659 |
| 3700 | 1 | 54565 | 18024 | 57236 | 0 | 1347 |
| 4809 | 1 | 54665 | 17532 | 57224 | 0 | 662 |
| 3605 | 1 | 54666 | 17569 | 57236 | 0 | 1347 |
| 713 | 1 | 54668 | 17540 | 57236 | 0 | 1347 |
| 2577 | 0 | 54750 | 17506 | 57244 | 0 | 1353 |
| 2252 | 0 | 54792 | 17539 | 57232 | 0 | 659 |
| 1729 | 1 | 54814 | 17575 | 57232 | 0 | 659 |
| 3047 | 0 | 54823 | 17534 | 57244 | 0 | 1347 |
| 2832 | 1 | 54825 | 17535 | 57240 | 0 | 1347 |
| 2850 | 0 | 54827 | 17536 | 57240 | 0 | 1347 |
| 2705 | 0 | 54832 | 18218 | 57240 | 0 | 1346 |
| 1079 | 0 | 54843 | 17532 | 57240 | 0 | 1347 |
| 1430 | 0 | 54848 | 17532 | 57232 | 0 | 658 |
| 4923 | 0 | 54856 | 17766 | 57240 | 0 | 1346 |
| 2512 | 0 | 54861 | 17495 | 57216 | 0 | 664 |
| 2512 | 1 | 54861 | 17495 | 57216 | 0 | 664 |
| 4517 | 1 | 54863 | 17489 | 57232 | 0 | 658 |
| 4804 | 1 | 54864 | 17537 | 57240 | 0 | 1347 |
| 4285 | 0 | 54874 | 17568 | 57240 | 0 | 1347 |
| 4286 | 0 | 54874 | 17570 | 57240 | 0 | 1347 |
| 5161 | 0 | 54887 | 20536 | 57228 | 0 | 660 |
| 1426 | 0 | 54906 | 17550 | 57232 | 0 | 657 |
| 1177 | 1 | 54909 | 17521 | 57232 | 0 | 663 |
| 1772 | 0 | 54913 | 17560 | 57236 | 0 | 1347 |
| 3203 | 1 | 54915 | 17583 | 57232 | 0 | 661 |
| 1305 | 1 | 54929 | 17542 | 57240 | 0 | 1352 |
| 3331 | 1 | 54935 | 17582 | 57220 | 0 | 667 |
| 3421 | 0 | 54941 | 17379 | 57240 | 0 | 1354 |
| 2624 | 0 | 54948 | 18313 | 57244 | 0 | 1353 |
| 4273 | 0 | 54958 | 17543 | 57240 | 0 | 1347 |
| 4102 | 0 | 54964 | 17572 | 57240 | 0 | 1347 |
| 3423 | 0 | 54964 | 17454 | 57236 | 0 | 1353 |
| 2617 | 0 | 54968 | 18322 | 57244 | 0 | 1353 |
| 2656 | 1 | 54969 | 18059 | 57264 | 0 | 1359 |
| 4245 | 0 | 55001 | 17509 | 57248 | 0 | 1352 |
| 2230 | 0 | 55003 | 17434 | 57248 | 0 | 1346 |
| 1981 | 0 | 55055 | 17069 | 57240 | 0 | 1351 |
| 2456 | 1 | 55067 | 17638 | 57228 | 0 | 665 |
| 4778 | 0 | 55081 | 21581 | 57236 | 0 | 1351 |
| 3291 | 0 | 55081 | 18848 | 57248 | 0 | 29309 |
| 2597 | 0 | 55125 | 18042 | 57252 | 0 | 1354 |
| 4564 | 1 | 55131 | 22233 | 57244 | 0 | 1354 |
| 5276 | 1 | 55164 | 17999 | 57240 | 0 | 1345 |
| 1354 | 0 | 55217 | 17597 | 57244 | 0 | 1351 |
| 1731 | 0 | 55242 | 17596 | 57236 | 0 | 1351 |
| 1673 | 1 | 55428 | 21919 | 57248 | 0 | 1361 |

## Tested but not landed (selected)

| cell | base | raw | aligned | size | rel.plt | extab | why |
|---|---:|---:|---:|---:|---:|---:|---|
| L1638:0 | 54,247 | 54,205 | 17,541 | 57,236 | 0 | 1,349 | size +4 |
| L4610:0 | 54,151 | 54,140 | 17,655 | 57,244 | 0 | 1,346 | size +12 |
| L4613:0 | 54,151 | 54,166 | 17,558 | 57,236 | 0 | 1,345 | size +4 |
| L4565:0 | 54,151 | 54,181 | 17,653 | 57,244 | 0 | 1,343 | size +12 |
| L3836:0 | 54,151 | 54,190 | 17,643 | 57,240 | 0 | 1,345 | size +8 |
| L4720:0 | 54,151 | 54,204 | 17,628 | 57,240 | 0 | 1,346 | size +8 |
| L4550:0 | 54,247 | 54,515 | 17,905 | 57,220 | 0 | 1,347 | size -12 (used in pair) |
| L4028:1 | 54,119 | 54,118 | 17,644 | 57,232 | 0 | 661 | superseded by cell 8 (54,097 in the 6-set) |
| L5079:1 | 54,541 | 54,477 | 19,434 | 57,232 | 0 | 28,832 | `.ARM.extab` explodes |
| L3401:0/1 | 54,541 | 54,507 | 17,903/17,911 | 57,232 | 0 | 725 | aligned much worse |
| L2512:0/1 | 54,541 | 54,657 | 17,011 | 57,232 | 0 | 724 | best aligned, raw +116 |
| L2038:0 | 54,541 | 54,581 | 17,102 | 57,232 | 0 | 725 | aligned -325, raw +40 |

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 /dev/shm/w11103/measure.py      # section bytes incl. .rel.plt
cmake --build cmake-build-match --target check -j2
```

## Open questions

1. The switch-arm block order (case 69/125, range prong 131..145) is still
   MBP-driven by body sizes, not by branch weights; no expect cell moved it.
2. `L4610:0` (54,140, size +12) is the best raw single cell seen on the final
   base.  It needs a genuine -12 B source-shape companion (only `L4550:0` is
   -12, and the pair is 54,144 with size 57,240 — still +8).  A faithful
   -12 B shape in the same block would unlock another ~-79 raw.
3. The literal pool at `0x145890` (300 B run) is data layout, not weights.
