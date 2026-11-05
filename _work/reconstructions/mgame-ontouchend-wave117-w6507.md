# MGame::OnTouchEnd (8080 B) — w6507 (wave 117): gate-safe 78.9189 -> 79.0488 %

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `c22678d64`, branch
`wave117-w6507`.

## Result

**Gate-safe improvement landed.**  HEAD was **8080 B / 78.9189 %**
(official verify 78.9).  The new source is **8080 B / 79.0488 %**
(official verify 79.0), `.rel.plt` diff **0**, `RATCHET PASS`
(byte_exact 4501, linked_exact 4501, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0).  All 27 ELF sections keep their
original name/address/size and the file stays 2,187,328 B.

The gain is small (+0.13 pp) because the remaining gap is pure block
ordering, but it is a real measured improvement and it clears the
ratchet.

## The three edits

```cpp
} else if (Globals::status->getMission()->hasWon() == true ||   // (1)
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
    Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
    bool keepObjectives = campaign->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
    Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : campaign;  // (2)
    if (mission->isCampaignMission()) {                        // (3) campaign-first dispatch
        Globals::status->nextCampaignMission(true);
    } else if (mission->isInstantActionMission()) {
        Globals::switch_to_target_setting = 2;
        this->active = 0;
        this->applicationManager->SetCurrentApplicationModule(1);
        return;
    } else {
        Globals::status->setFreelanceMission(Mission::empty);
        Globals::layout->showMissionRewardMessage(mission->getReward() + mission->getBonus(), false);
    }
```

1. **`wontrue1`** — write the first `hasWon()` test as `== true`.
2. **`missionvar`** — reuse the already-computed `campaign` pointer
   instead of re-calling `getCampaignMission()`.
3. **`disp`** — swap the completion dispatch to campaign-first.

All three are behaviour-neutral.  Edit (3) is *not* the original's
source order (the original tests `isInstantActionMission` first at
0x17af7e and `isCampaignMission` at 0x17afa6), but it aligns the long
campaign-case chain far better; see §4.

## 1. The new lever: `hasWon() == true` changes the branch probability

`Mission::hasWon()` returns `unsigned char`, so the IR for
`if (hasWon())` is `icmp eq i8 %x, 0` and `BranchProbabilityInfo::
calcZeroHeuristics` gives **37.5 % / 62.5 %**.  Writing
`hasWon() == true` emits `icmp eq i8 %x, 1`, which is *not* a
zero comparison and therefore falls back to **50 % / 50 %**.

That probability change is what finally moves the cold
`else { resetStartSequenceOver(); }` block: with 50/50 the reset
chain's frequency is high enough for `MachineBlockPlacement::
selectBestCandidateBlock` to pick it before the chain that carries
`Level::removeObjectives`, so the first reference to
`resetStartSequenceOver` in the emitted function precedes the first
`removeObjectives` and the 4-cycle `.rel.plt` rotation disappears.

| build | size | fuzzy (4 dp) | `.rel.plt` |
|---|---:|---:|---|
| HEAD | 8080 | 78.9189 | OK |
| `wontrue1` alone | 8072 | 78.9474 | OK |
| **`disp + missionvar + wontrue1`** | **8080** | **79.0488** | **OK** |
| `disp + missionvar` | 8084 | 79.0631 | ROT (4) |
| `disp + merge127 + missionvar` | 8080 | 82.3869 | ROT (4) |
| `disp + merge127 + missionvar + wontrue1` | 8072 | 78.9000 | OK |

`wonone1` (`hasWon() == 1`) is byte-identical to `wontrue1`; the
`== true` spelling was kept because it reads naturally.

## 2. The 82.4 % variant is still the best fuzzy score, still PLT-locked

`disp + merge127 + missionvar` remains **8080 B / 82.3869 %** but rotates
`.rel.plt` by one (`resetStartSequenceOver` moves from index 3324 to
3327), which drops `byte_exact` 4501 -> 4500 (`MGame::successCheck`
loses its byte-exact `blx` targets) and fails the ratchet.  Full check
with that variant:

```
verify: byte 4500  linked 4501
FAIL  byte_exact 4501 -> 4500
```

So the highest *gate-safe* score is 79.0488 %, not 82.4 %.

## 3. Measurements collected this wave (all at the required 8080 B unless noted)

| variant | size | fuzzy | plt |
|---|---:|---:|---:|
| HEAD | 8080 | 78.9189 | OK |
| **`disp + missionvar + wontrue1`** | **8080** | **79.0488** | **OK** |
| `disp + missionvar` | 8084 | 79.0631 | 4 |
| `disp + missionvar + wontrue1 + keepobj2/failne/expcamp0/expcamp1` | 8080 | 79.0488 | OK |
| `disp + missionvar + wontrue1 + creditfirst` | 8080 | 78.9407 | OK |
| `disp + missionvar + wontrue1 + exp65_1 + mv65first` | 8080 | 74.0807 | OK |
| `disp + missionvar + wontrue1 + inlinekeep` | 8080 | 71.9178 | OK |
| `disp + missionvar + wontrue1 + creditfirst + mv65first` | 8116 | 75.2785 | 6 |
| `disp + merge127 + missionvar` | 8080 | 82.3869 | 4 |
| `disp + merge127 + missionvar + expcw0 + expinst0` | 8080 | 82.5734 | 2 |
| `disp + merge127 + missionvar + exphw0 + expinst0` | 8080 | 82.5734 | 2 |
| `disp + merge127 + missionvar + wonone1` | 8072 | 78.9474 | OK |
| `disp + merge127 + missionvar + wonone` (both operands) | 8076 | 77.2170 | OK |
| `disp + merge127 + missionvar + expcw0` | 8072 | 75.5948 | 2 |
| `disp + merge127 + missionvar + exphw0` | 8068 | 75.6084 | 2 |
| `wonone` alone (HEAD dispatch) | 8072 | 73.3694 | OK |
| `disp + wonone` | 8084 | 75.3017 | OK |
| `disp + merge127 + wonone` | 8076 | 77.1949 | OK |
| `disp + merge127 + missionvar + swap4265/mv42after65` | 8084 | 79.0773 | 4 |
| `disp + merge127 + missionvar + mv42last` | 8112 | 74.9730 | 6 |
| `disp + merge127 + missionvar + mv42first` | 8112 | 74.9730 | 6 |
| `disp + merge127 + missionvar + mv15last/mv22first/mv65first` | 8084 | 79.0773 | 4 |
| `disp + merge127 + missionvar + exp42_0` | 8076 | 50.8022 | 4 |
| `disp + merge127 + missionvar + exp65_0` | 8080 | 53.8767 | 4 |
| `hudTouchFlags & 0x400` branch moved last (HEAD) | 8064 | 74.4907 | OK |
| `hudTouchFlags & 0x400` branch moved first (HEAD) | 8064 | 71.9077 | 2 |

Ops `nested`, `first`, `andnot`, `keepobj2`, `failne`, `wonneq`,
`expcamp0/1` are code-identical to their base (they only re-spell the
same CFG), `swapwon` costs 4 B, `hoistwon` shrinks by 4 B and scores
72.9.

## 4. Why `disp` (campaign-first) helps even though the original is instant-first

The original's dispatch order is instant-first
(`isInstantActionMission` 0x17af7e, `isCampaignMission` 0x17afa6),
matching HEAD.  Campaign-first nevertheless aligns the long
`case 15 ... case 162` chain closer to the original addresses: it moves
the whole `changeCredits` + campaign-switch chain earlier, and the
`goto finishMission` tail then lands nearer its original position.
`missionvar` removes the redundant `getCampaignMission()` re-fetch and
`wontrue1` restores the 8080 B size while keeping the reset block in
front of `removeObjectives`.

## 5. Gate / section state (verified)

27 sections (exact), file size 2,187,328 B, `.text` 0x1337c8,
`.ARM.extab` 0x26294 (the same 4 B short as HEAD), `.rodata` 0x41c27
(same as HEAD), `.rel.plt` diff 0, imports 207, lint CLEAN, drift 0,
`RATCHET PASS` (byte 4501 / linked 4501, size 4524/4524, data 843/843).
No section name/address/size changed relative to HEAD.

```bash
cd /home/debian/GalaxyOnFire2/wt/w6507
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch (not tracked): `/dev/shm/w6507/` — `score.py`/`score2.py`
(size/fuzzy/PLT), `rebuild.sh` (~1.3 s), `v2.py`/`v3.py`/`exp4.py`/
`cases.py` (variant appliers), `sweep*.py` (combination sweeps),
`blockcmp.py`/`blocks3.py` (basic-block alignment), `mir_head.txt`/
`mir_var.txt` (LLVM `-print-machineinstrs` dumps).
