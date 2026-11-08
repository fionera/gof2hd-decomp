# MGame::OnTouchEnd — wave 164 (w11012): reward-chain 100/110 reorder, raw 4,127 -> 4,089

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w11012`, base `b3d3a1f68` (wave-163 final).
Scratch `/dev/shm/w11012`. Target `_ZN5MGame10OnTouchEndEiiPv`,
original 8,080 B @ `0x17a144`.

| | base (wave 163) | this wave |
|---|---:|---:|
| size | 8,080 | **8,080** (exact) |
| raw same-address diff | 4,127 | **4,089** (-38) |
| aligned (`fn_diff.py`) | 906 | 918 (+12) |
| `.rel.plt` diff | 0 | **0** (29,392 B byte-identical) |
| `.text` diff | 84,652 | **84,614** (-38) |

`check` green: byte **4507/4524**, linked **4507/4524**, size **4524/4524**,
data **843/843**, imports **207**, lint CLEAN, drift 0, RATCHET PASS.

## 1. Original chain order (re-derived from the binary)

The reward chain is a plain `else if` ladder.  The original's `cmp r0,#N`
sites appear in this order:

```
15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120, 126, 127, 134, 144,
155, 161, 162
```

* `96` check at `0x17b70a` falls into the body at `0x17b722`, which falls
  through to the shared `departStation(getStation(98))` tail at `0x17b77c`
  (95 sets `r1=10` and branches to the same tail).
* `127` check at `0x17bc24` is `beq.w 0x17b722` — a **backward** jump into the
  96 body (clang tail-merged the two identical bodies).  It sits after 126
  and before 134.
* `161` sits after `155`, not after `95`.
* 19 chain `isCampaignMission` calls + 4 outside the chain = 23.

HEAD has `161` after `95` and a combined
`(getCurrentCampaignMission() == 96 || ... == 127)` block just before `162`
(22 calls).  The faithful order + split (with `if (failed)` truthy, which is
the original's `cmp r4,#0; beq.w`) measures **5,816 / 1,379 / 8,080 / 0** —
a raw regression, because the shared return epilogue moves from the original
`0x17b004` to `0x17af82`.  That block placement is the epilogue worker's
territory; this pass leaves it alone.

## 2. Predicate reads (all verified against the binary)

* `int failed = getMission()->hasFailed();` — original tests it truthy
  (`0x17ab36: cmp r4,#0; beq.w`).  HEAD's `failed == 1` is `cmp #1; bne.w`.
* `won = getMission()->hasWon() || campaignMission->hasWon()` — original
  `0x17adc8: blx hasWon; cbnz r0, body`, then
  `0x17add6: blx hasWon; cmp r0,#0; beq.w else`.  HEAD's `== true` on the
  `unsigned char` return emits `cmp #1`; the truthy form re-lays the function
  to 8,064 B (the `icmp eq i8 X,0` 20:12 `calcZeroHeuristics` weight).
  `Mission::hasWon` is `ldrb r0,[r0,#5]; bx lr` (4 B), consistent with
  `bool hasWon()` + `bool won` (w10811), but the bool form measures 6,053 raw
  because it moves the epilogue — so the `unsigned char + == true` tuning is
  kept here.
* `keepObjectives = campaignMission->hasWon() ? !getMission()->isCampaignMission() : false`
  — original `0x17ade0..0x17ae06`; HEAD's `!= false` already produces
  `cmp r0,#0; beq.w` and is exact.  `? :`, `&&`, `!= 0` spellings are all
  raw-neutral.
* `mission = getMission()->hasWon() ? getMission() : getCampaignMission()`
  — original `0x17af5e..0x17af7c` (`cbz r1`); HEAD matches.
* `mission->isInstantActionMission()` then `mission->isCampaignMission()`
  order matches (`0x17af7e`, `0x17afa6`).

## 3. What landed

Swap the `== 100` and `== 110` chain blocks (the two bodies differ only in
`getStation(120)` vs `getStation(10)` and 110's `enterSpaceLounge = 1`).
The swap moves the 17b944..17bd44 band from 945 to 907 differing bytes.

```cpp
} else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 110) {
    ... departStation(getStation(10)); appManager->SetCurrentApplicationModule(5);
    enterSpaceLounge = 1; ...
} else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 100) {
    ... departStation(getStation(120)); appManager->SetCurrentApplicationModule(5); ...
```

This is a layout tuning, not the original source order (the binary has 100
before 110).  It is landed because it is a measured -38 raw win at exact size
with `.rel.plt` byte-identical.

## 4. Measured alternatives (all size 8,080 / relplt 0 unless noted)

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| base (wave 163) | 4,127 | 906 | 8,080 | 0 |
| **100/110 swap (landed)** | **4,089** | 918 | 8,080 | 0 |
| 100/110 swap + 126/120 swap | 4,089 | 922 | 8,080 | 0 |
| 100/110 swap, `(B & 1)` won | 4,069 | 919 | 8,080 | **9** |
| 161 after 155 (faithful-combined) | 4,409 | 995 | 8,080 | 0 |
| faithful order + split + `if (failed)` | 5,816 | 1,379 | 8,080 | 0 |
| faithful order + split, `failed == 1` | 4,667 | 894 | 8,092 | 0 |
| split-in-place 96/127 + 100/110 swap | 4,657 | **878** | 8,088 | 0 |
| truthy `won` on base order | 6,082 | 1,908 | 8,064 | 0 |
| `if (failed)` truthy on base order | 4,745 | 2,007 | 8,076 | 0 |

`(B & 1)` is the raw-best form (-58 vs base) but it emits
`resetStartSequenceOver` before `dockToStream` and rotates five `.rel.plt`
slots (`resetStartSequenceOver`, `dockToStream`, `useCloak`,
`maneuverTouchEnd`, `isInstantActionMission`) — not landable under the
byte-identical `.rel.plt` invariant.  `split-in-place + 100/110` has the best
aligned score (878) but is +8 B; no -8 B companion was found
(`if (failed)` truthy gives 8,076, `won` truthy leaves 8,088).

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
python3 /dev/shm/w11012/relplt.py
cmake --build cmake-build-match --target check -j8
```

Scratch: `/dev/shm/w11012/{chain2.py,sweep2..9.py,sweep3.json,sweep4.json,
seqdiff.py,multiset.py,bands.py,relplt_syms.py,MGame.base.cpp}`.
