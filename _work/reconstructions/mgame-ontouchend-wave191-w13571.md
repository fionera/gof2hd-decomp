# MGame::OnTouchEnd — wave191b w13571: pre-chain/block-order reconstruction (note only, no raw win)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13571`, branch
`wave191b-w13571`, base master `cb9c59ca0`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13571`.

Prior notes: `mgame-ontouchend-wave191-w13540.md` (chain order), `-w13539.md`
(95/96/127 body, back-to-station tail), `-w13532.md` (100/126 getter order).

## Result

Base unchanged. `check` stays green (byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370). `MGame::OnTouchEnd` raw **2,882**, aligned **1,561**,
size **8,080/8,080**.

## New evidence this pass

### 1. The original pre-chain tests `isInstantActionMission()` FIRST

Original `0xe16`..`0xfcc` (disassembly, ours is byte-identical up to `0xe3e`):

```
mission = getMission()->hasWon() ? getMission() : getCampaignMission();   // 0xe1a..0xe38
instantAction = mission->isInstantActionMission();                        // 0xe3a
if (instantAction) {                                                      // 0xe3e cbz -> 0xe62
    switch_to_target_setting = 2;                                         // 0xe40..0xe48 (word store)
    active = 0;                                                           // 0xe4a..0xe4c
    applicationManager->SetCurrentApplicationModule(1);                   // 0xe50..0xe60
    return;
}
if (mission->isCampaignMission()) { nextCampaignMission(true); }          // 0xe62..0xe7a
else { setFreelanceMission(empty); showMissionRewardMessage(...); }       // 0xf72..0xf9a
changeCredits(reward + bonus);                                            // 0xf9e..0xfb4
levelScript->m_nTimeLimit = 0;                                            // 0xfb8..0xfbe
// chain starts 0xfc0 (first test 0xfc2, getCurrentCampaignMission call 0xfcc)
```

The base source instead emits `if (isCampaignMission) ... else if (!instantAction) ...
else { instant action }`, i.e. it tests `isCampaignMission()` first.  The
non-campaign arm in the original is `setFreelanceMission` **before**
`showMissionRewardMessage`; the base has the reverse order.  Both are
behaviour-preserving reorderings (independent calls / distinct stores).

### 2. Original campaign-chain emitted order and bodies (confirmed)

Emitted order (from `getCurrentCampaignMission` call sites, `seq.py`):

```
15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120, 126, 127, 134, 144, 155, 161, 162
```

`127` is a separate test placed **after 126** that branches back to the 96 body
(`0x1aec cmp #127; beq.w 0x15de`).  The base merges it into the 96 condition
(`==96 || ==127`), so its `127` test is emitted next to 96 instead.

Original 95/96 bodies (word store = `switch_to_target_setting`, byte store =
`Level::initStreamOutPosition`; resolved via `Globals.h`/`Level.h` types):

```
95: switch=1; HP; SH; AR; GA; init=1; depart(getStation(10)); active=0; module(2)
96: switch=1; HP; SH; AR; GA; init=1; depart(getStation(98)); active=0; module(2)
```

Base 95 has `switch` between AR and GA; base 96 has AR before SH.  (Note: the
w13539 note's "init first, switch after getters" is wrong — the word store is
first and the byte store is after the getters.)

### 3. Why the chain is shifted: the PlayEngineSound block

The base chain starts at `0xffc`; the original at `0xfc0` (+0x3c).  The shift is
caused by one cold block: the `else if (soundsMuted)` `PlayEngineSound` loop
(source ~line 1815) is emitted at `0xf78` in ours but at `0x10a4` in the
original (between the 43 and 42 arms).  The `ResumeEngineSound` block is at
`0xdc2` in both, and `StopEngineSound` at `0x147a` vs `0x145e` (+0x1c).  The
`dockToAsteroid` third call is likewise laid out inline at `0x1e1c` in ours but
tail-merged to a shared block at `0x1bb0` in the original.

## Measured experiments (all reverted; no size-exact raw win)

All builds `cmake --build cmake-build-match --target link_so -j2` under
`ulimit -v 12000000`; raw/aligned/size from `tools/fn_diff.py`.

### Full pre-chain × chain-order × 96/127 × body matrix (24 builds)

| pre | order | split | bodies | raw | aligned | size |
|---|---|---|---|---:|---:|---:|
| base | base | 0 | base | **2,882** | **1,561** | 8,080 |
| base | base | 0 | orig | 2,923 | 1,551 | 8,080 |
| base | base | 1 | base | 4,745 | 1,837 | 8,112 |
| base | base | 1 | orig | 4,748 | 1,828 | 8,112 |
| base | orig | 0 | base | 4,563 | 1,513 | 8,088 |
| base | orig | 0 | orig | 4,565 | 1,520 | 8,088 |
| base | orig | 1 | base | 4,072 | 1,255 | 8,092 |
| base | orig | 1 | orig | 4,070 | 1,262 | 8,092 |
| ia | base | 0 | base | 4,467 | 1,876 | 8,080 |
| ia | base | 0 | orig | 4,471 | 1,876 | 8,080 |
| ia | base | 1 | base | 4,760 | 1,885 | 8,104 |
| ia | base | 1 | orig | 4,764 | 1,885 | 8,104 |
| ia | orig | 0 | base | 4,405 | 1,506 | 8,080 |
| ia | orig | 0 | orig | 4,407 | 1,426 | 8,080 |
| ia | orig | 1 | base | 4,746 | 1,518 | 8,104 |
| ia | orig | 1 | orig | 4,748 | 1,438 | 8,104 |
| iaswap | base | 0 | base | 4,497 | 1,876 | 8,080 |
| iaswap | base | 0 | orig | 4,501 | 1,876 | 8,080 |
| iaswap | base | 1 | base | 4,790 | 1,885 | 8,104 |
| iaswap | base | 1 | orig | 4,794 | 1,885 | 8,104 |
| iaswap | orig | 0 | base | 4,416 | 1,510 | 8,080 |
| iaswap | orig | 0 | orig | 4,418 | 1,430 | 8,080 |
| iaswap | orig | 1 | base | 4,757 | 1,520 | 8,104 |
| iaswap | orig | 1 | orig | 4,759 | 1,440 | 8,104 |

`pre=ia` = instant-action-first; `iaswap` = additionally
`setFreelanceMission` before `showMissionRewardMessage`; `order=orig` = original
emitted chain order; `split` = 96/127 split with 127 after 126; `bodies=orig` =
95/96 store-order fix.  **No combination beats raw 2,882.**

### Original-order (mixed conditions, base bodies) = 8,076 (-4, fatal)

`order=orig`, conditions left as the base's mixed forms: raw 3,945, aligned
1,205, size **8,076**.  This reproduces w13540's "faithful order + current
bodies" number.  Adding the 96/127 split: raw 4,742, size 8,112.

### All-conditions `isCampaignMission()`-first on base order

raw 4,747, aligned 1,736, size 8,080.  Worse.

### Chain-arm rotations (18 rotations of the base order)

Only rotation 0 (the base order) is size-exact; every other rotation is
size-fatal (8,088..8,204) and raw 4,398..4,876.

### Micro-mutations (all size-exact unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base | 2,882 | 1,561 |
| instant-action arm: `switch=2` before `active=0` | 2,883 | 1,557 |
| non-campaign: `setFreelanceMission` before `showReward` | 2,884 | 1,561 |
| both of the above | 2,885 | 1,557 |
| 96 body SH before AR | 2,883 | 1,561 |
| 95 body `switch` first | 2,922 | 1,551 |
| 95+96 corrected bodies | 2,923 | 1,551 |
| 65 body `switch`/getter order | 2,890 | 1,547 |
| 81 body `switch`/getter order | 2,887 | 1,561 |
| 110 body getter order | 2,907 | 1,558 |
| 162 body getter order | 2,883 | 1,592 |
| drop `__builtin_expect` on 162 | 3,741 | 1,356 |
| `__builtin_expect(won,1)` | 4,624 | 1,698 (size 8,088) |
| `__builtin_expect(won,0)` | 5,413 | 2,367 (size 7,840) |
| instant-action arm 3-stmt permutation 102 | 2,883 | 1,557 |
| 102 + 65 body | 2,891 | 1,543 |
| 102 + 65 + tail | 2,901 | 1,530 |
| 102 + 65 + 96 | 2,892 | 1,543 |

The instant-action arm is size-exact only for statement orders 012 (base) and
102; the other four permutations are 8,076.  The `soundsMuted` reference
(`uint8_t &soundsMuted = ...`) can be moved after `resumeSounds()` or inlined
without changing codegen (raw/aligned unchanged); clang still hoists the
`ldrh [fp,#420]` above the `MenuTouchWindow::OnTouchEnd` call, whereas the
original loads `ldrb` after `resumeSounds()`.

## Conclusion

The base is a strict local minimum for raw.  The residual is block *placement*,
not block bodies: the original's cold-block layout puts the non-campaign block
immediately after the won-branch and the `PlayEngineSound`/`dockToAsteroid`
cold blocks inside the chain region, while clang 7.0.2 lays ours out with those
blocks before the chain.  Matching the emitted chain order alone does not help
because the chain start is shifted by +0x3c and block sizes differ.  The next
angle would be to reproduce the original cold-block order exactly (needs the
exact original source shape around the pause-menu `soundsMuted` handling and the
`dockToAsteroid` call sites), not to permute the campaign arms.

## Reproduction

Scratch: `gen.py` (pre/order/split/bodies matrix), `rot.py` (chain rotations),
`mut.py` (95/96/tail/162 mutations), `measure.sh` (build + `fn_diff`),
`seq.py` (emitted chain order), `sidediff.py`/`diffmap.py`.
