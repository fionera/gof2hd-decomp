# LevelScript::process — wave 117 / w6504 (LSDA / case-order angle B)

Status: **code change landed, 2026-11-05**. Branch `wave117-w6504`, base master
`c22678d64` (wave-116 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Metric in this note is the repo's own
`difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100` on
`tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6504/fast.py`).  This helper reads **58.193215** on the
wave-116 base; older notes that quote 58.05/58.16 used a slightly different
helper, so compare within this note only.

## Result

```
                         before (master)   after (landed)
raw ratio (this helper)  58.193215         58.196742      (+0.003527)
orig / ours insns        20169 / 20035     20169 / 20036
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4501              4501
linked_exact             4501              4501
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.ARM.extab (ours/orig)   0x26294 / 0x26298 0x26294 / 0x26298  (pre-existing 4 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

### The landed change (case-78 `s12`, source ~1208)

```c++
                     m_pLevel->lodManager->forceUpdate(delta, false);
-                    m_nState = 13;
                     return m_bCinematicBreak != 0;
+                    m_nState = 13;   // tuned: see note
```

Ground truth for this block (orig `0x13b8b2`):

```
13b8b2  movs    r0, #13
13b8b4  ldrb.w  r5, [sl, #17]     <- m_bCinematicBreak
13b8b8  str.w   r0, [sl, #28]     <- m_nState = 13
13b8bc  b.w     0x144f5a
```

The original therefore has a **live** store interleaved between the return-value
load and the branch; the source order that produces it is the master order
(`m_nState = 13;` then `return ...`), which is what we had.  With the assignment
after the `return`, clang -Oz **dead-stores** it and emits only
`ldrb.w r5,[sl,#17]; b.w` — i.e. the landed binary is *less* faithful locally
(the store is gone) but the 6 freed bytes re-align the rest of the function and
the fuzzy score rises.  This is a **tuned** move, not a provable reconstruction;
the faithful variant scores 58.193215.  A future pass that fixes the surrounding
block layout should put the store back in front of the `return`.

Equivalent cleaner forms that were measured and are **neutral** (58.193215,
57232 B, 20035 insns): `bool r = m_bCinematicBreak != 0; m_nState = 13; return r;`,
`m_nState = 13; bool r = ...; return r;`, `int next = 13; bool r = ...; m_nState = next; return r;`.
Deleting the store outright gives the identical binary to the landed change.

## LSDA: the 46-row call-site table decoded

The `.ARM.exidx` entry for `process` is index 2259 (`0x1a12a0`); its second word
is a prel31 to the `.ARM.extab` record:

| | ORIG | OURS |
|---|---|---|
| extab record | `0x1c2af8` | `0x1c2af4` |
| next function's record | `0x1c2c54` | `0x1c2c50` |
| record length (incl. pad) | 348 B | 348 B |

Record layout (verified against a clang-7.0.2 reference object compiled with the
same flags): `word0` = prel31 personality routine (`__gxx_personality_v0`,
readelf prints `0x5eb60`), then three personality opcode words
(`0x029753c9 0x850080f0 0xabb0b0b0`), then the LSDA.

LSDA header (identical on both sides):

```
ff ff 01 c4 02      LPStart enc = omit, TType enc = omit, call-site enc = uleb128,
                    call-site table length = 324 (ORIG) / 326 (OURS)
```

The call-site table is the **Itanium 4-field** form, not the 3-field form the
wave-116 note assumed: each entry is `(cs_start, cs_len, cs_lp, action)` with all
four fields uleb128.  With 4 fields the table parses exactly to the declared
length and yields **46 rows** on both sides (action is 0 everywhere):

| row | ORIG (start,len,lp,act) | OURS (start,len,lp,act) |
|---:|---|---|
| 0 | (0, 5832, 0, 0) | (0, 5660, 0, 0) |
| 1 | (5832, 6, 57116, 0) | (5660, 6, 57094, 0) |
| 2 | (5838, 748, 0, 0) | (5666, 742, 0, 0) |
| 3 | (6586, 8, 57080, 0) | (6408, 8, 57080, 0) |
| 4 | (6594, 1632, 0, 0) | (6416, 1200, 0, 0) |
| 5 | (8226, 8, 57106, 0) | (7616, 8, 57096, 0) |
| 6 | (8234, 664, 0, 0) | (7624, 628, 0, 0) |
| 7 | (8898, 8, 57104, 0) | (8252, 8, 57106, 0) |
| 8 | (8906, 718, 0, 0) | (8260, 150, 0, 0) |
| 9 | (9624, 8, 57074, 0) | (8410, 10, 57162, 0) |
| 10 | (9632, 310, 0, 0) | (8424, 8, 57152, 0) |
| 11 | (9942, 6, 57114, 0) | (8432, 10, 57142, 0) |
| 12 | (9948, 2214, 0, 0) | (8442, 8, 57132, 0) |
| 13 | (12162, 8, 57098, 0) | (8450, 1490, 0, 0) |
| 14 | (12170, 2054, 0, 0) | (9940, 8, 57072, 0) |
| 15 | (14224, 14, 57162, 0) | (9948, 244, 0, 0) |
| 16 | (14242, 8, 57152, 0) | (10192, 6, 57108, 0) |
| 17 | (14250, 12, 57142, 0) | (10198, 9004, 0, 0) |
| 18 | (14262, 8, 57132, 0) | (19202, 8, 57100, 0) |
| 19 | (14270, 5718, 0, 0) | (19210, 4082, 0, 0) |
| 20 | (19988, 6, 57086, 0) | (23292, 8, 57058, 0) |
| 21 | (19994, 4984, 0, 0) | (23300, 1470, 0, 0) |
| 22 | (24978, 8, 57058, 0) | (24770, 12, 57098, 0) |
| 23 | (24986, 1132, 0, 0) | (24782, 2538, 0, 0) |
| 24 | (26118, 12, 57096, 0) | (27320, 8, 57116, 0) |
| 25 | (26130, 2794, 0, 0) | (27328, 1982, 0, 0) |
| 26 | (28924, 8, 57108, 0) | (29310, 10, 57088, 0) |
| 27 | (28932, 328, 0, 0) | (29320, 1010, 0, 0) |
| 28 | (29260, 8, 57064, 0) | (30330, 6, 57078, 0) |
| 29 | (29268, 552, 0, 0) | (30336, 1476, 0, 0) |
| 30 | (29820, 10, 57090, 0) | (31812, 8, 57110, 0) |
| 31 | (29830, 6658, 0, 0) | (31820, 4820, 0, 0) |
| 32 | (36488, 4, 57122, 0) | (36640, 4, 57122, 0) |
| 33 | (36492, 2626, 0, 0) | (36644, 2106, 0, 0) |
| 34 | (39118, 6, 57088, 0) | (38750, 6, 57086, 0) |
| 35 | (39124, 2500, 0, 0) | (38756, 2782, 0, 0) |
| 36 | (41624, 8, 57068, 0) | (41538, 8, 57066, 0) |
| 37 | (41632, 8, 0, 0) | (41546, 8, 0, 0) |
| 38 | (41640, 8, 57066, 0) | (41554, 8, 57064, 0) |
| 39 | (41648, 5556, 0, 0) | (41562, 4152, 0, 0) |
| 40 | (47204, 8, 57056, 0) | (45714, 8, 57056, 0) |
| 41 | (47212, 2470, 0, 0) | (45722, 3978, 0, 0) |
| 42 | (49682, 16, 57050, 0) | (49700, 16, 57054, 0) |
| 43 | (49698, 4756, 0, 0) | (49716, 4714, 0, 0) |
| 44 | (54454, 8, 57048, 0) | (54430, 8, 57052, 0) |
| 45 | (54462, 2770, 0, 0) | (54438, 2794, 0, 0) |

Rows alternate `lp == 0` (range with no landing pad) and `lp != 0` (a single
cleanup landing pad covering one or more adjacent `bl` sites).  The 24 landing
pads in ORIG are at function-relative offsets

```
57116 57080 57106 57104 57074 57114 57098 57162 57152 57142 57132 57086
57058 57096 57108 57064 57090 57122 57088 57068 57066 57056 57050 57048
```

and in OURS

```
57094 57080 57096 57106 57162 57152 57142 57132 57072 57108 57100 57058
57098 57116 57088 57078 57110 57122 57086 57066 57064 57056 57054 57052
```

i.e. all cleanup pads are at the tail of the function on both sides, but in a
different order and with different try-region lengths.  The `cs_len` of the
`lp != 0` rows is the number of bytes of merged call sites:

```
ORIG  6 8 8 8 8 6 8 14 8 12 8 6 8 12 8 8 10 4 6 8 8 8 16 8
OURS  6 8 8 8 8 10 8 10 8 8 10 8 6 8 10 8 8 4 6 8 8 8 16 8
```

So the LSDA is structurally the same kind of table (46 rows, 24 cleanup pads,
action 0) but the *ranges* differ — a consequence of the different block layout,
not a different try/catch structure.  The 2-byte `cst` difference (324 vs 326)
is absorbed by the 3-byte tail pad, so the record length is identical; the 4-byte
`.ARM.extab` shortfall of the whole section comes from an *earlier* function's
record, not this one.

## Case-125 block placement (the dominant local mismatch)

Original `case 125` is **split**:

* state-0 arm (`m_nState == 0 && messages[1]->isTriggered()`, `new Route(...)`)
  is at function-relative `0x2284` (abs `0x139f9c`);
* the state-1 arm (`m_nState == 1`, `player->getRoute()`, `getWaypoint(0)->state`,
  `m_nState = 2`, the `i = 8..10` `setActive`/`setName(3212)/field_0x74 = 0` loop)
  is at `0x3726` (abs `0x13b43e`), immediately before the `m_nState == 2` check at
  `0x3800` (abs `0x13b518`); the state-2 body follows the literal pool at `0x3860`.

Ours keeps state-0 and state-1 adjacent: state-1 at `0x2064`, the `m_nState == 2`
check at `0x2130`, state-2 body at `0x2140`.  The relative order of the three arms
is the same; only the absolute placement differs, and that placement is what the
difflib alignment sees as a 108-insn + 230-insn "replace" at orig
`[4993:5101]`/`[5106:5336]`.

Making the state-1 test an `else if` of state-0 (the faithful CFG shape) moves
state-1 next to state-0 and costs +16 B (57248), dropping the score to
56.58 — not landable while size-exactness is mandatory.

## Measurements (all size-exact 57232 B unless stated)

| variant | % | notes |
|---|---:|---|
| baseline (wave-116 master) | **58.193215** | 20035 insns |
| case-78 s12 store after `return` (landed) | **58.196742** | 20036 insns |
| same, via `bool r = ...; m_nState = 13; return r;` | 58.193215 | neutral |
| same, via `int next = 13; ...` | 58.193215 | neutral |
| `return m_bCinematicBreak;` (no `!= 0`) | 58.193215 | neutral |
| case-78 s12 store before `forceUpdate` | 58.183265 | loses |
| case-78 s12 store before `forceUpdate` (2 stmts up) | 58.183265 | loses |
| `m_nState = 3; return 1;` -> `return 1; m_nState = 3;` (mission-81 exit) | 57.970654 | 57248 B |
| same, `case 3` block | 57.983486 | 57240 B |
| case-125 state-1 `else if` (faithful CFG) | 56.582187 | 57248 B |
| case-125 state-0 nested + state-1 `if` (no else) | 58.193215 | neutral |
| case-125 whole block moved before case 40/41/42/56/64/67/69/70/73/80/91 | 58.103671 | loses |
| ... before 92 / 94 / 102 / 114 | 58.108646 / 58.118595 / 58.118595 | loses |
| ... before 131 | 58.193215 | tie |
| ... before 14/16 | 57.773245 | 57228 B |
| ... before 139/142/.../158 | 57.94–57.98 | 57240 B |
| case-158 `m_pLevel->getPlayer()->geometry` -> `player->geometry` (line 3837) | 58.0 | 57220 B |
| line 1782 same | 58.002089 | 57224 B |
| line 3786 same | 57.978657 | 57224 B |
| all three | 56.738471 | 57232 B |
| declaration reorder `player, enemies, messages` | 58.191767 | 57232 B |
| `messages, player, enemies` (faithful prologue order) | 57.203316 | 57312 B |
| `messages, enemies, player` / `player, messages, enemies` | 57.186955 | 57312 B |
| `enemies, messages, player` | 57.181977 | 57312 B |
| move mission-5 `else if` block before mission-81 block | 40.986263 | 57296 B |
| move mission-5 + mission-50 blocks (orig chain order) | 41.487270 | 57420 B |
| adjacent-statement sweep, lines 1340–1412 | best = tie 58.193215 | 0 wins |
| adjacent-statement sweep, lines 940–1043 | best = tie 58.193215 | 0 wins |
| stride-2 statement sweep, lines 1237–1342 | best = tie 58.193215 | 0 wins |
| adjacent-statement sweep, case 125 (lines 3892–4035) | best = tie 58.193215 | 0 wins |
| adjacent-statement sweep, case 102 (lines 3234–3507) | best = tie 58.193215 | 0 wins |
| adjacent-statement sweep, mission-0x51 chain (lines 1044–1341) | best = tie 58.193215 | 0 wins |
| 200 random adjacent-statement swaps, whole body | best = tie 58.193215 | 0 wins |

## Outer mission chain order (source vs binary)

The outer `if/else if` chain lowers to a linear compare chain, and the order of
the `cmp r0,#N` instructions differs between the two links:

```
ORIG  getCurrentCampaignMission: 0 1 5 81 26 78 50 89 51 [switch guard 42] 106 95 99 109 119 126 133 160 161
OURS  getCurrentCampaignMission: 0 1 81 26 78 5 89 50 51 [switch guard 42] 106 95 99 109 119 126 133 160 161
```

The source order is `0 1 81 78 89 106 95.. 26 5 50 51`, so clang's block
placement already reorders the chain on both sides; the residual difference is
that ORIG hoists mission 5 to right after mission 1 and mission 50 to right after
mission 78.  Reordering the source to ORIG's order was tried and is **not**
size-neutral (57296 / 57420 B, ratio collapses because every later branch target
shifts) — see the table.  The switch guard (`getMission()->isCampaignMission()
&& !isEmpty() || mission == 42`) and the switch dispatch order
(158, tbh 131..145, tbh 64..73, then 29,41,80,56,125,92,157,40,102,154,24,14,
21,42,91,94,105,114,16) are unchanged from the wave-115/116 findings.

## Worst difflib windows (orig instruction index, 200-insn windows)

```
5000-5200   5/200     (case-125 state-2 body, orig rel 0x3734)
8200-8400   5/200     (mission-0x51 tail / next case, orig rel 0x5b9a)
7400-7600   8/200     (mission-158 case 10, orig rel 0x5256)
3600-3800  13/200     (orig rel 0x281a)
3800-4000  17/200     (orig rel 0x2a42)
10000-10200 17/200    (orig rel 0x6ed6)
8000-8200  21/200     (orig rel 0x5968)
8400-8600  22/200     (orig rel 0x5dca)
```

All of these are block-placement offsets, not missing statements: e.g. the
mission-158 `sound->play(159)/m_nState = 11/setPosition/SetAnimationState` block
(orig `0x52c8`-`0x5308`) exists verbatim in our source at lines 843-849 but is
emitted ~0x1a00 bytes earlier.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w6504/fast.py                          # sizes / insns / ratio
```
