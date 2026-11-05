# MGame::OnTouchEnd (8080 B) — w6407 (wave 116): 82.4 % at 8080 B exists,
# but every campaign-first layout rotates `.rel.plt` (reset block is MBP-locked)

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `d672d349f`, branch
`wave116-w6407`.

## Result

**No gate-safe improvement found.**  HEAD stays at **8080 B /
78.9189 % / `.rel.plt` diff 0 / RATCHET PASS** (byte 4501, linked 4501,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
27 sections, file 2,187,328 B, `.text` 0x1337c8, `.ARM.extab` 0x26298).

This wave found a **new best fuzzy score at the required 8080 B:
82.4000 %** (up from 78.9189 %), but like every campaign-first variant it
rotates the 4-cycle `resetStartSequenceOver → removeObjectives →
goingToWaypoint → removeRoute` in `.rel.plt`, which drops `byte_exact`
4501 → 4500 (`MGame::successCheck` loses its byte-exact `bl` targets) and
fails the ratchet.  The note records the exact variants, the reason the
reset block cannot be moved, and all measurements.

## 1. New best 8080-byte variant: 82.4000 % (PLT rotated)

Starting from `HEAD`, apply three independent, behaviour-neutral edits:

1. **`disp`** — swap the mission-completion dispatch to campaign-first:

```cpp
if (mission->isCampaignMission()) {
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

2. **`merge127`** — collapse the `96` case and the separate `127` case
   back into one condition (i.e. undo wave-114's `split127`), dropping the
   redundant inner `mission->isCampaignMission()`:

```cpp
} else if (mission->isCampaignMission() &&
           (Globals::status->getCurrentCampaignMission() == 96 ||
            Globals::status->getCurrentCampaignMission() == 127)) {
```

3. **`missionvar`** — reuse the already-computed `campaign` pointer in the
   mission selection:

```cpp
Mission *mission = Globals::status->getMission()->hasWon()
                       ? Globals::status->getMission() : campaign;
```

Measured (`tools/verify` machinery, `/dev/shm/w6407/score.py`):

| variant | size | fuzzy (4 dp) | `.rel.plt` |
|---|---:|---:|---|
| **HEAD** | **8080** | **78.9189** | **OK** |
| `disp + merge127` | 8084 | 81.3000 | ROT |
| **`disp + merge127 + missionvar`** | **8080** | **82.4000** | **ROT** |
| `disp + merge127 + missionvar` + `notfailed` | 8080 | 82.3000 | ROT |
| `disp + merge127 + missionvar` + `first` | 8080 | 82.4000 | ROT |
| `disp + merge127 + missionvar` + `keepObjectives &&` | 8084 | 81.3000 | ROT |
| `disp + merge127 + missionvar` + `m_nTimeLimit` first | 8084 | 81.1000 | ROT |
| `disp + merge127 + missionvar` + `hud 0x400 arm first` | 8084 | 81.1000 | ROT |

So the fuzzy metric **does** reach 82.4 % at exactly 8080 B; only the PLT
order blocks the gate.

## 2. Why the PLT rotates: the reset block always lands after the
##    finishMission `removeObjectives`

`.rel.plt` entries are created in relocation order, and gold processes the
OnTouchEnd section's relocations in offset order, so the PLT order is the
order of the *first* reference to each symbol inside OnTouchEnd.  The
required order is `reset, removeObjectives, goingToWaypoint, removeRoute`.

| build | reset `blx` | first `removeObjectives` | PLT |
|---|---:|---:|---|
| original | 0x17b0ae | 0x17b270 | reset first |
| HEAD | 0x17b22a | 0x17b286 | reset first |
| `disp+merge127+missionvar` (82.4 %) | 0x17b43a | 0x17b27c | **removeObjectives first** |

The first `removeObjectives` is the `if (!keepObjectives)` "finishMission"
block; the second (failed path) is at 0x17b842, after the reset.  So the
gate needs the reset block emitted **below the finishMission block**, i.e.
before the campaign switch.

In the original the reset block sits at 0x17b0aa, immediately after the
failed-type (`getType()==12`) dispatch and before the freelance-won block:

```
orig  0x17b07e  failed-type dispatch (getType==12)
      0x17b0aa  resetStartSequenceOver        <- first reference
      0x17b0b6  freelance-won (setFreelanceMission + reward msg)
      0x17b0e2  changeCredits
      0x17b104  campaign switch (15, 22, 43, 42, ...)
```

In every campaign-first variant the reset block is instead emitted right
after the **case-65 body** and before the case-81 test
(`disp+merge127+missionvar`: 0x17b434 `b.n` tail of case 65, 0x17b436
`ldr r0,[fp,#124]`, 0x17b43a `blx resetStartSequenceOver`, 0x17b440
case-81 test) — i.e. ~446 B *after* the finishMission `removeObjectives`
at 0x17b27c.

## 3. The reset block is placed by MachineBlockPlacement and cannot be
##    moved from source

The reset block is the `else` of the won test (`hasWon() ||
campaign->hasWon()`), a cold chain whose only predecessor is the won-test
block.  `MachineBlockPlacement::buildChain` follows the hot fall-through
successors first; cold chains are then taken from `BlockWorkList` by
`selectBestCandidateBlock` (highest `MBFI` block frequency).  The reset
chain's frequency is ~0.000204, so it is only selected once every hotter
cold chain has been placed — in our CFG that is after the case-65 body.

Every source spelling of the reset arm produces the *same* CFG and the
*same* emitted layout (verified byte-identical disassembly):

* `} else { reset; }` (HEAD)
* `} else if (!won) { reset; } else { body }` (`first`)
* `} else { if (won) { body } else { reset; } }` (`nested`)
* `bool won = ...; if (won) { body } else { reset; }` (`boolwon`)
* `if (!failed) { if (won) {...} else { reset; } } else { failed }` (`notfailed`)
* `if (!(isCampaignMission() || cur == 42)) { ... } else { ... }` (failed-arm inversion)

and all of them rotate the PLT once the dispatch is campaign-first.

Measured this wave (all on top of `HEAD` unless noted):

| variant | size | fuzzy (4 dp) | `.rel.plt` |
|---|---:|---:|---|
| **HEAD** | **8080** | **78.9189** | **OK** |
| `split127 + disp` (wave-115 reproduction) | 8080 | 79.9063 | ROT |
| `split127 + disp + nested` / `first` / `boolwon` / `disp_nested` | 8080 | 79.9063 | ROT |
| `split127 + disp + keepObjectives &&` | 8080 | 79.9063 | ROT |
| `split127 + disp + reset_elsefailed` | 8080 | 79.9063 | ROT |
| `split127 + disp + notfailed` | 8080 | 79.8000 | ROT |
| `split127 + disp + hud 0x400 arm first` | 8080 | 79.8000 | ROT |
| `split127 + disp + freelance arm first` | 8080 | 79.9063 | ROT |
| `split127 + disp + mission = campaign` | 8076 | 78.3000 | ROT |
| `split127 + disp + sepif` | 8120 | 78.2000 | ROT |
| `split127 + disp + resetfirst` (dup won test) | 8120 | 78.4000 | **OK** |
| `split127 + disp + reset_pre` | 8112 | 78.3000 | **OK** |
| `split127 + disp + hoistwon` | 8096 | 73.3000 | **OK** |
| `split127 + disp + hoistwon2` | 8100 | 73.1000 | **OK** |
| `split127 + disp + swap42/65` | 8096 | 77.8000 | ROT |
| `split127 + disp + case65 first` | 8052 | 70.1000 | ROT |
| `split127 + disp + case42 last` | 8164 | 72.2000 | **OK** |
| `split127 + disp + wonfirst` / `wonfirst_nested` | 8096 | 61.3000 | ROT |
| `disp + first` / `disp + nested` | 8088 | 79.1000 | ROT |
| `disp + notfailed` | 8088 | 79.0000 | ROT |
| `HEAD + notfailed` | 8080 | 78.8000 | **OK** |
| `HEAD + wonfirst` | 8092 | 63.4000 | ROT |
| `HEAD + keepObjectives &&` / `inline failed` / `failed-arm inversion` / `freelance arm first` | 8080 | 78.9189 | **OK** |
| `HEAD + m_nTimeLimit first` | 8080 | 78.8000 | **OK** |
| `HEAD + hud 0x400 arm first` | 8080 | 78.8000 | **OK** |
| `HEAD + changeCredits first` | 8076 | 76.8000 | **OK** |
| `HEAD + mission = campaign` | 8076 | 77.1000 | **OK** |
| `HEAD + 96||127 without inner isCampaignMission` | 8068 | 73.7000 | **OK** |
| `HEAD + pause/resume/cutscene permutation 210` | 8080 | 78.8000 | **OK** |
| `HEAD + pause/resume/cutscene permutations 021/102/120/201` | 8076 | 76.0000 | **OK** |
| `HEAD + hud400 & tl` / `hud400 & perm` / `tl & perm` pairs | 8080 | 78.6–78.7 | **OK** |

Only 8080-byte builds are gate candidates: any other size shifts `.text`
and drops `byte_exact` to ~3102.

## 4. Why `disp` scores higher although the original is instant-first

The original dispatch order is instant-first (`isInstantActionMission` at
0x17af7e, then `isCampaignMission` at 0x17afa6), matching HEAD.  The
campaign-first spelling nevertheless aligns ~1 pp better because it moves
the whole `changeCredits` + campaign-switch chain so that the long
`case 96 … case 162` bodies land closer to the original's addresses.
`disp + merge127 + missionvar` goes further (82.4 %) because collapsing
the `96`/`127` pair removes the extra `isCampaignMission()` call that
wave-114's `split127` had introduced.  Note the original really does have
a separate `127` test at 0x17bc34 (`cmp r0,#127; beq.w 0x17b722`), so
`split127` is the semantically faithful shape; `merge127` is the
fuzzy-metric-friendlier one.

## 5. Instruction multiset is still essentially exact

Normalised instruction multiset diff (registers/pool offsets masked):
79 differing entries out of 2865 raw lines, and they are almost all
`ldr rX,[pc,#N]` literal-pool offsets, `b.n`/`b.w` width differences and
the known mining-loop `cmp #0` / `cmp #-1` (`count > 0` vs `amount != 0`)
pair.  Writing `amount != 0` reproduces the original instruction but
shrinks the function to 8072 B (wave-115 measurement, 70.9782 %).  So the
whole remaining gap is block/literal-pool *ordering*, not code shape.

## 6. Gate / section state (unchanged, verified this wave)

27 sections (exact), file size 2,187,328 B, `.text` 0x1337c8 (size and
base exact), `.ARM.extab` 0x26298 (exact), `.rel.plt` diff 0, imports
207, lint CLEAN, drift 0, `RATCHET PASS` (byte 4501 / linked 4501,
size 4524/4524, data 843/843).

```bash
cd /home/debian/GalaxyOnFire2/wt/w6407
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch (not tracked): `/dev/shm/w6407/` — `score.py` (size/fuzzy/PLT),
`rebuild.sh` (touch + link + score, ~1.2 s), `harness.py`
(`T.build(text, name)` prints `size pct plt`), `batch4.py` … `batch23.py`
(the variant generators), `regions.py`/`regions3.py` (difflib opcode
alignment of the normalised instruction streams), `mset4.py` (masked
instruction multiset), `orig.asm`/`ours.asm`/`sd.asm`/`mv.asm` (objdump
of 0x17a144..0x17c0d4), `v/` (MIR dumps, `mbp.cpp` LLVM 7
MachineBlockPlacement source, `ord.py` block-order extraction).
