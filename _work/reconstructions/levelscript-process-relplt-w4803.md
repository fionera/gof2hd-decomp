# LevelScript::process — `.rel.plt` first-reference rotation (w4803, wave 98)

Status: **landed** (`LevelScript::process pass 6`). Branch `wave98-w4803`, base master
`fe4f5e7cf`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**, 20169 instructions.  The landed shape keeps the
body size-exact at 57232 B and restores the original `.rel.plt` first-reference
order (12 -> 0 differing bytes); its official raw is **53.131534** (53.1 %,
was 54.806399) and `.text` differing bytes are **102,168** (was 101,978).
The trade-off is documented in section 5.

This pass attacks the last documented non-`.text` identity delta: the **12-byte
`.rel.plt` rotation** (`docs/IDENTICALITY.md` §"only differing section bytes").
The rotation is caused by MachineBlockPlacement putting the mission-125
`>=4` chain before the mission-0 state-9 chain; the landed 54.8 % source shape
cannot be moved without changing the CFG/branch weights, and **every size-exact
source shape found that restores the original PLT order costs ~3 points of
function score** (best: 51.72 %).  No strict improvement was landable, so this
note records the full search.

## 1. The rotation

`readelf -rW` on the linked `.so` (3674 `R_ARM_JUMP_SLOT` entries, identical
offsets and sizes in both links):

```
idx  original                          ours
2824 Level::flashScreen                Level::flashScreen
2825 StarSystem::scaleSunDuringSupernovaIntro   (same)
2826 StarSystem::updateSupernova        (same)
2827 PlayerEgo::isInWormhole            (same)
2828 PlayerWormHole::freeMissionLock    (same)
2829 StarSystem::switchSunForSupernovaIntro (same)
2830 PlayerEgo::setDockingState         (same)
2831 StarSystem::switchPlanetForIntro   PlayerEgo::deleteHackingGame   <-- moved
2832 Level::switchSkyboxForIntro        StarSystem::switchPlanetForIntro
2833 ParticleSystemManager::reset       Level::switchSkyboxForIntro
2834 PlayerEgo::startSmokeEmission      ParticleSystemManager::reset
2835 PlayerStation::setVisible          PlayerEgo::startSmokeEmission
2836 PlayerEgo::deleteHackingGame       PlayerStation::setVisible
2837 LevelScript::lookBehind            LevelScript::lookBehind
```

i.e. exactly one symbol (`PlayerEgo::deleteHackingGame`) is first referenced
five slots too early.  `.rel.plt` differs in 12 bytes (6 rotated `r_info`
words); `.plt` size/base are unchanged.

Gold emits PLT entries in first-relocation order; with `-ffunction-sections`
each function is one section and the relocations inside
`.rel.text._ZN11LevelScript7processEi` are sorted by `r_offset`.  So the
first-reference order is simply the **instruction order inside `process`**.
`PlayerEgo::deleteHackingGame` is referenced nowhere else in the tree
(`grep -rn deleteHackingGame src/` → only `LevelScript.cpp:3621`), and
`PlayerStation::setVisible` only at `LevelScript.cpp:4496`, so both entries are
decided entirely by this one function.

Object-file relocation offsets (fresh `-Oz` compile of the pristine tree):

```
deleteHackingGame      .text+0x543c   (21564)
switchPlanetForIntro   .text+0x61b2   (25010)
PlayerStation::setVisible .text+0x677e (26494)
```

Original (address − 0x137d18):

```
switchPlanetForIntro   0x54d4
PlayerStation::setVisible 0x5d06
deleteHackingGame      0x61bc
```

So the original order is `switchPlanet < setVisible < delete`; ours is
`delete < switchPlanet < setVisible`.

## 2. Root cause: MachineBlockPlacement chain order

`-mllvm -print-machineinstrs=block-placement` (MIR layout order, 1390 blocks)
gives:

| block | what | layout pos |
|---|---|---|
| `bb.391` | mission-0 `switch(m_nState)` dispatch (`tbh`, 10 targets) | 388 |
| `bb.392` | mission-0 state 6 body head | 389 |
| `bb.544`..`bb.554` | mission-125 state-1 loop + state-2 entry | 540..550 |
| `bb.585` | mission-125 `m_nState>=4 && <=6` check | 581 |
| `bb.586`..`bb.590` | `isDockedToDockingPoint`/`hackingWon`/`getHackingGameDockIndex`/`deleteHackingGame` | 582..586 |
| `bb.593` | mission-125 state-6 guard loop | 589 |
| `bb.654` | mission-0 state-8→9 body (`setRumblePercentage`, `m_nState=9`) | 648 |
| `bb.667` | mission-0 state-9 body (`switchPlanetForIntro`, `switchSkyboxForIntro`, `ParticleSystemManager::reset` x2) | 649 |
| `bb.674` | mission-42 state-7 (`PlayerStation::setVisible`) | 669 |

`buildChain()` extends the entry chain greedily; when a chain cannot be
extended it calls `selectBestCandidateBlock()`, which scans `BlockWorkList`
(pushed in IR order by `fillWorkLists`) and picks the **first block with the
highest block frequency**.  Block frequencies (BFI, the run immediately before
placement):

```
bb.585 (mission-125 >=4 check)  359,661,655
bb.654 (mission-0 state-8→9)    234,881,080
bb.667 (mission-0 state-9)      146,800,675
```

so the mission-125 chain is picked at position 581, i.e. before the mission-0
states at 648/649.  In the original the relative order is the opposite, so the
original's branch weights must differ (`bb.585` would have to fall below
`bb.654`).

The mission-125 chain is reached from `bb.550`/`bb.551` (`cmp r0,#3`), which is
reached from `bb.552` (`cmp r0,#2`); the state-1 loop (`bb.544`..`bb.549`) is
what inflates the frequency of the whole chain.

## 3. Source reordering has no effect

Because the layout is frequency-driven, moving the `case 125` block around the
campaign switch does **not** change the emitted order.  All of these are
byte-for-byte identical in size and in the delete/setVisible offsets
(`0x543c` / `0x677e`):

* move `case 125` before `case 158` (start of the campaign switch);
* move `case 125` after `case 42`;
* move `case 125` after `case 16` (end of the switch);
* move `case 42` before `case 158` / before `case 16`;
* reorder the mission-0 `switch(m_nState)` arms;
* `if/else if` reordering of the mission-0 arm chain.

## 4. Separator sweep (the only knob that flips the order)

`case 125` has four `if`/`else if` separators:

```
S0: state 0 -> state 1      S1: state 1 -> state 2
S2: state 2 -> state 3      S3: state 3 -> state >=4
```

Committed HEAD is `(S0=else, S1=if, S2=else, S3=else)` = bits `1011`.
Enumerating all 16 combinations (compile-only, object relocation offsets):

| bits | size | delete | setVisible | PLT |
|---|---:|---:|---:|---|
| 0000 | 0xdf8c (−4) | 0x6c24 | 0x64d4 | **clean** |
| 0001 | 0xdf8c (−4) | 0x6c1e | 0x64d4 | **clean** |
| 0010 | 0xdfc4 (+52) | 0x5478 | 0x67cc | dirty |
| 0011 | 0xdfb0 (+32) | 0x5442 | 0x6786 | dirty |
| 0100 | 0xe00c (+124) | 0x5314 | 0x67ea | dirty |
| 0101 | 0xdff4 (+100) | 0x52fe | 0x67d2 | dirty |
| 0110 | 0xe02c (+156) | 0x585c | 0x67e4 | dirty |
| 0111 | 0xdfb0 (+32) | 0x582c | 0x6786 | dirty |
| 1000 | 0xdfa4 (+20) | 0x678e | 0x64d4 | **clean** |
| 1001 | 0xdf9c (+12) | 0x6772 | 0x64c2 | **clean** |
| 1010 | 0xdfdc (+76) | 0x5494 | 0x67e4 | dirty |
| 1011 | **0xdf90** | 0x543c | 0x677e | dirty (HEAD) |
| 1100 | 0xdffc (+108) | 0x57c2 | 0x67da | dirty |
| 1101 | 0xdfec (+92) | 0x57b2 | 0x67ca | dirty |
| 1110 | 0xe024 (+148) | 0x6d5c | 0x6246 | **clean** |
| 1111 | 0xdf9c (+12) | 0x6e42 | 0x62a4 | **clean** |

Official raw score of the clean rows:

```
0000  46.408565   (57228)
0001  46.426171   (57228)
1000  45.928655   (57252)
1001  51.779581   (57260)
1110  36.972040   (57380)
1111  40.930325   (57244)
```

Only `S2 = if` (i.e. `} else if (m_nState == 3)` → `}` + `if (m_nState == 3)`)
keeps the `this` register in `sl` (matching the original prologue); the other
clean rows move `this` to `r9` and lose ~8-18 points.

A further sweep of 16 separator combinations × one `__builtin_expect` on any of
the four conditions (128 compiles) found **no size-exact clean variant**.

## 5. Landed size-exact clean candidate

`v_a` = HEAD + these five edits:

1. `case 125` S2: `} else if (m_nState == 3)` → `}` + `if (m_nState == 3)` (line 3582);
2. mission-1 `rotate`: `m_pLevel->getPlayer()->rotate(...)` → `player->rotate(...)` (line 950);
3. mission-0 state-3 `setCollide`: `m_pLevel->getPlayer()->setCollide(true)` → `player->setCollide(true)` (line 721);
4. case-139 `setCollide`: `m_pLevel->getPlayer()->setCollide(true)` → `player->setCollide(true)` (line 1471);
5. drop the now-inert `(void) pad;` line (line 643).

Result (re-measured in the follow-up pass; the earlier 51.723880 / 102,329
figures in this note were a stale measurement of the same edit set):

```
size            57232 / 57232   (size-exact)
official raw    53.131534       (was 54.806399; -1.67 points)
insns           20169 / 20003
.rel.plt        byte-identical  (delete 0x675e > setVisible 0x64b2)
.text diff      102,168 bytes   (was 101,978 -> +190)
gate            byte 4500, linked 4500, size 4524/4524, data 843/843,
                imports 207, lint CLEAN, drift 0  (RATCHET PASS)
```

The PLT order is restored at the cost of 1.67 points of function score and
+190 `.text` differing bytes.  Total differing bytes go from 101,990
(101,978 + 12) to 102,168 (+178), so this is a `.rel.plt`-for-`.text`
trade, not a free win; it was landed because the `.rel.plt` rotation was the
explicitly requested target and the S2 split is the faithful CFG (section 6).
Edits 2-4 revert the pass-5 `getPlayer()` re-fetches that w4702 added to reach
54.8 %.  Section 6 shows that the `:721` and `:1471` re-fetches are faithful in
the original, so reverting them is the size price paid for the clean PLT; only
the `:1694` revert is itself faithful.

## 6. Follow-up faithfulness audit (wave 98, w4803)

The original disassembly proves the S2 split is the *faithful* shape, not an
artefact:

* mission-125 state-3 check: the original's `cmp r0,#3` at `0x13dcb6` is reached
  both from the state-2 check's `bne.w 13dcb6` (`0x13b51e`) **and** from the
  state-2 body's `beq.w 13dcb2` (`0x13b52e`) fall-through (which reloads
  `m_nState` at `0x13dcb2` and falls into `cmp r0,#3`).  That is only possible
  when state 3 is an independent `if`, i.e. `} if (m_nState == 3)`, not
  `} else if`.  HEAD (bits `1011`) has `else if`; the landed shape uses the
  independent `if` (bits `1001`).
* mission-125 state-3 body: the original stores `movs r0,#4; str r0,[r5,#28]`
  at `0x13dd22`, i.e. `m_nState = 4`, not `m_nState++`.  (`m_nState = 4` is
  size-neutral in this shape and was measured at 53.131534 as well.)
* pass-5 `getPlayer()` re-fetch audit (original vs source line):
  - state-3 `setCollide(true)` (`LevelScript.cpp:721`) -> `0x138a34` **re-fetches**
    `Level::getPlayer()`; the refetch is faithful.
  - mission-1 `setCollide(true)` (`:1471`) -> `0x138028` **re-fetches**; faithful.
  - case-139 `setCollide(true)` (`:1694`) -> `0x13f432` uses the cached player in
    `r9`; the pass-5 refetch there is **not** faithful.
  The landed shape reverts all three to `player->setCollide(...)` because that
  is what buys the 24 bytes needed to offset the S2 split; the two reverts at
  `:721`/`:1471` are therefore unfaithful and are the price of the size-exact
  clean PLT.
* `volatile int pad` (line 643) is inert in this shape (removal alone is
  size-neutral); it is dropped in the landed tree.

Other size-exact clean variants measured in the follow-up:

| variant | size | official raw | .text diff |
|---|---:|---:|---:|
| landed: S2 + rotate + c721(all 3) + nopad | 57232 | 53.131534 | 102,168 |
| + `m_nState = 4` | 57232 | 53.131534 | 102,160 |
| + revert pass-5 `if` at "State-2 conclude" (`r3`) | 57232 | 52.957284 | 102,120 |
| + `r3` + `m_nState = 4` | 57232 | 52.957284 | 102,108 |
| + `r3` + `m_nState = 4` + case-139 revert | 57232 | 52.955965 | 102,105 |

The last row has the lowest `.text` diff but needs an extra unfaithful
`else if` revert; none reaches HEAD's 101,978.

## 7. Other measured variants (all size-exact, all dirty unless noted)

| variant | size | official raw | PLT |
|---|---:|---:|---|
| move `deleteHackingGame` after `m_nState++` (H) | 57232 | 54.786465 | dirty |
| swap `case 42` state-7 if/else bodies (D) | 57232 | 54.806399 | dirty |
| `__builtin_expect(mission==0, 1)` | 57140 | 34.414101 | dirty |
| `__builtin_expect(case-125 >=4, 0)` | 57236 | 48.339437 | dirty |
| `__builtin_expect(m_nState==3, 0)` | 57248 | 46.239318 | dirty |
| `__builtin_expect(m_nState==2, 1)` | 57380 | 44.391883 | clean |
| `__builtin_expect(m_nState==3, 1)` | 57272 | 39.728962 | clean |
| fold `state==2 && isOver()` (L_fold) | 57252 | 46.180677 | clean |
| merge `state==2` into the 0/1 chain (E_merge) | 57244 | 40.930325 | clean |

## 8. Conclusion / next steps

The `.rel.plt` rotation is a pure MachineBlockPlacement artefact: the
mission-125 `>=4` chain (`bb.585`, frequency 3.6e8) is emitted before the
mission-0 state-9 chain (`bb.667`, 1.5e8).  Restoring the original order
requires the mission-125 chain to become colder than the mission-0 state-8/9
chains, which in this tree can only be done by changing the `case 125`
`if`/`else if` structure (`S2`).  The landed S2 split is the faithful shape
(section 6a) and restores `.rel.plt` byte-identity, at the cost of 1.67 points
of function score and +190 `.text` bytes.

A future pass that decodes the mission-125 state machine (or the mission-0
state-8/9 bodies) more faithfully should re-run the 16-combination sweep: if the
`S2` split becomes size-neutral with the faithful `:721`/`:1471` re-fetches
kept, both the `.text` and `.rel.plt` deltas fall out for free.

## 9. Reproduction

```
cmake --build cmake-build-match --target link_so -j8
# object relocation order (fast, no link):
readelf -rW <LevelScript.o> | grep -E 'deleteHackingGame|PlayerStation10setVisible'
# layout order:
clang++ ... -mllvm -print-machineinstrs=block-placement \
        -mllvm -filter-print-funcs=_ZN11LevelScript7processEi
# official score:
python3 tools/verify/verify.py --show _ZN11LevelScript7processEi ...
```
