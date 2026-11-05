# LevelScript::process — wave 119 / w6786 (mission-0 state-3 store reorders)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6786`, base master
`109d153e9` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.498122         58.522996      (+0.024874)
orig / ours insns        20169 / 20034     20169 / 20034
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.text                    0x1337c8          0x1337c8
.ARM.exidx               0x6f70             0x6f70
.ARM.extab (ours/orig)   0x26294 / 0x26298 0x26294 / 0x26298  (pre-existing 4 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6786/score.py`).

## Landed changes

Both are size-neutral (57232 B) reorders of independent stores inside the
mission-0 `m_nState == 3` exit block (source ~line 689).

| # | change | note |
|---:|---|---|
| 1 | `m_pRadar->enabled = 1` moved before `m_pLevel->getPlayer()->setCollide(true)` | tuned |
| 2 | `reinterpret_cast<uint8_t &>(field_0x12) = 1` moved before `m_bStartSequence = 0` | tuned |

Running score: `58.498122 -> 58.518021 (hunk 1) -> 58.522996 (hunk 2)`.
Both reorders are register-allocation / store-scheduling alignments, not
behavioural reconstruction; the original store order is not recoverable from
the dropped-`>= 8000`-arm shape of this block (see the wave-118 w6603 note).

## Measured dead ends (all 57232 B unless noted)

| variant | % | note |
|---|---:|---|
| swap case-50/51 condition operand order | 58.522996 | identical binary |
| swap mission-26 / mission-5 switch arms | 58.418526 | |
| move `removeAllGuns` into state 4 | 58.506616 | |
| move freeLook/enableFirstPerson/hideShip into state 4 | 58.125016 | |
| move all four into state 4 | 58.133519 | |
| prologue declarations M,P,E | 57.473611 | **57308 B** |
| prologue M,P,E + no anchor | 57.472410 | **57228 B** |
| prologue E,P,M + cached `player->rotate` | 54.316994 | **57412 B** |
| remove mission-0 anchor block | 58.278574 | **57152 B** |
| move state 4/5 blocks | 51.260086 | **57396 B** |
| cached `Status *status` local | 45.101120 | **57296 B** |
| replace all `m_pLevel->getPlayer()` with `player` | 28.08 | **53108 B** |
| fresh `getSpeed()` after `setSpeed` | 58.221614 | **57240 B** |
| drop dead `speedBits = getSpeed()` | 58.308458 | **57224 B** |
| case-91 state-0 faithful `m_nState=1` + `setActive` | 58.339551 | **57240 B** |

An adjacent-statement swap sweep over the whole function (source lines 614-5293,
1756 candidates) found no further win over 58.522996.  A single-statement move
sweep (110 candidates) and a pairwise swap sweep (55 candidates) over the
mission-0 state-3 block (lines 686-696) also found nothing better; the two
landed adjacent swaps are the optimum of that block.  A distance-2/3 swap sweep
over the eight largest-diff source regions (1130-1155, 2875-2930, 3125-3145,
3938-3968, 4470-4495, 4718-4750, 4795-4820, 495-510) produced only ties.

## Structural findings (recorded, not landed)

* Original prologue call order is **M,P,E** (`getMessages`, `getPlayer`,
  `getEnemies`); the current source is E,P,M.  The original's stack slots still
  match the E,P,M declaration order, and every source permutation that makes
  the calls M,P,E changes the body size (57228-57472 B), so the divergence is
  left in place.
* Original pre-switch mission order is
  `0, 1, 5, 81, 26, 78, 50, 89, 51, 106, 95...`; the source has 26 before 5 and
  50 before 89.  Reordering measured worse.
* Original mission-0 state-4 body contains the
  freeLook/enableFirstPerson/hideShip calls between `resetCamera` and
  `setCollide`, and `removeAllGuns` after `setComputerControlled`; the source
  has all four in state 5.  The faithful move measured worse (see table).
* Original mission-91 state 0 sets `m_nState = 1` and calls
  `enemies[0]->setActive(true)` before `setName(...)`; the source sets
  `m_nState = 2` and omits `setActive`.  The faithful variant grows to 57240 B.
* The mission-0 `switch (m_nState)` still merges states 4/5: the original table
  covers 6..15 (`subs #6`, 10 entries), ours covers 4..15 (`subs #4`, 12
  entries).  `tbh` inventory: orig `{12:1,5:2,15:1,10:5,4:3,7:1,6:2}`,
  ours `{12:2,5:2,15:1,10:4,4:2,7:2,6:2}`.
* The remaining ~41.5 % sequence loss is dominated by MachineBlockPlacement
  order and global register allocation, not by missing statements.  The
  largest single divergence is a ~297-instruction original-only run at
  normalized offset 9459 (a whole mission arm placed before the literal pool,
  which our binary places elsewhere).
