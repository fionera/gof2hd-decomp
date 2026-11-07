# MGame::OnTouchEnd — wave 132 (w7713): exact campaign/hudMenu content vs layout

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7713`,
base `70e451ffe`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w7713`. **No source change landed**: every content-correct shape is smaller
or re-lays-out the function and scores below the base.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.
Baseline (verified this wave, full `check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS          lint CLEAN   drift 0
.text 85,379   .ARM.extab 1,420   .note.gnu.build-id 20
.ARM.exidx 0   .rel.plt 0   .rodata/.data/.bss 0
raw fuzzy (difflib, autojunk off): 81.9607 %   size 8080/8080
```

## 1. Exact content deltas (multiset, base vs original)

Normalised-instruction multiset diff of the whole function (branch targets excluded):

| original only | ours only | meaning |
|---|---|---|
| `blx Status::getCampaignMission` | — | the `campaign` temp saves one call in the `won` body |
| `blx Mission::isCampaignMission` | — | merged `== 96 \|\| == 127` saves the 127 test's call |
| `cmp r4, #0` | `cmp r4, #1` | `hasFailed() == true` spelling |
| `cmp.w r9, #0` | `cmp.w r9, #-1` | mining guard `count > 0` vs `amount != 0` |
| `mov r1, r0` | — | mission-selection condition temp |

plus two `cmp r0, #1` (ours only) for the two `hasWon() == true` spellings. These are the
only non-branch/non-literal-pool content differences left; everything else in the base is
already instruction-identical to the original and differs only by block placement.

## 2. Original campaign/hudMenu control flow (ground truth, objdump)

`won` / `keepObjectives` / mission selection (`0x17adc8`):

```
17adc8  blx hasWon(getMission())
17adcc  cbnz r0, 17ade0                 ; first operand true -> won body
17adce  ldr r0,[r8]; blx getCampaignMission(); blx hasWon()
17adda  cmp r0,#0; beq.w 17b0aa         ; resetStartSequenceOver
17ade0  won body:
17ade0  ldr r0,[r8]; str.w sl,[sp,#24]
17ade8  blx getCampaignMission(); blx hasWon()
17adf0  cmp r0,#0; beq.w 17af5a         ; keepObjectives = false
17adf6  ldr r0,[r8]; blx getMission(); blx isCampaignMission()
17ae02  eor.w sl,r0,#1                  ; keepObjectives = !isCampaignMission()
17ae06  b.n 17af5e
17af5a  mov.w sl,#0                     ; keepObjectives = false
17af5e  mission selection:
17af5e  ldr r0,[r8]; blx getMission(); blx hasWon()
17af6a  mov r1,r0; ldr r0,[r8]
17af70  cbz r1,17af78
17af72  blx getMission(); b.n 17af7c
17af78  blx getCampaignMission()
17af7c  mov r5,r0; blx isInstantActionMission()
```

So the source shape is exactly:

```cpp
bool won = getMission()->hasWon() || getCampaignMission()->hasWon();
if (won) {
    bool keepObjectives = getCampaignMission()->hasWon()
                              ? !getMission()->isCampaignMission() : false;
    Mission *mission = getMission()->hasWon() ? getMission() : getCampaignMission();
```

The 96/127 cases are **two separate `else if`s with identical bodies**; the compiler
tail-merges the 127 body into the 96 body (`17bc36: beq.w 17b722`):

```
17b696  isCampaignMission(); cbz -> 17b708
17b6a4  cmp #95;  bne 17b708
...
17b70a  isCampaignMission(); cmp #0; beq.w 17b93c
17b71c  cmp #96;  bne.w 17b93c
17b722  96 body (getStation(98)) ...
17bc26  isCampaignMission(); cbz -> 17bc3a
17bc34  cmp #127; beq.w 17b722        ; shared 96 body
17bc3a  isCampaignMission(); cmp #0; beq -> 17bd00
17bc4c  cmp #134; bne -> 17bd00
```

## 3. Variants measured this wave (all reverted)

| variant | size | raw fuzzy | `.text` | `.rel.plt` |
|---|---:|---:|---:|---:|
| base (committed) | 8080 | **81.9607** | 85,379 | 0 |
| exact content (bare `hasFailed`/`hasWon`/`menuWindow`, no `campaign` temp) | 8068 | 72.5 | shifted | 0 |
| exact content + split 96/127 | 8096 | 66.7 | shifted | 0 |
| exact content + split + separate `if` | 8100 | 71.1 | shifted | 0 |
| base + split 96/127 | 8088 | 81.7 | shifted | 0 |
| base + split + separate `if` | 8088 | 79.5 | shifted | 0 |
| base + separate `if` + `goto afterHudMenu` (weaponSlot body) | 8080 | 81.9394 | 85,383 | 0 |
| base + separate `if` + `goto afterHudMenu` (wingman body) | 8080 | 79.09 | shifted | 0 |

The exact-content shape reproduces the original's instruction content in the
`failed`/`won`/`keepObjectives`/mission-selection chain instruction-for-instruction, but is
**12 B short** and re-lays-out the campaign chain, dropping the fuzzy score by ~9 points.
The split 96/127 restores the missing 23rd `isCampaignMission` call but costs +8 B on the
base (+28 B on the exact-content shape, because the extra test block changes the block
order and branch widths).

## 4. Size accounting

Base is size-exact only because two errors cancel:

* base test block `ldr; cmp #0; bne.w` (10 B) vs original `ldr; cbz` (6 B): **+4 B**
* `campaign` temp removes one `getCampaignMission` call: **-4 B**
* merged 96/127 removes one `isCampaignMission` call: **-4 B**
* missing `mov r1,r0` in the mission-selection condition: **-2 B**
* remaining +6 B from wider `b.w` branch encodings caused by the different layout

The original's `cmp #0; beq` / `cbz` spellings are all size-neutral against the base's
`cmp #1; bne`; the deficit is entirely the two removed calls plus the missing `mov`.

## 5. Remaining root cause (unchanged, re-confirmed)

`BranchFolder` picks the 0x200 `initHudMenu(1)` body as the shared-tail survivor because it
is the test block's layout predecessor (`TMB: IBB=471 PredBB=434 ... ST: n=2 pred=434
tails: 438(imm=2) 434(imm=1) -> CTOB: chosen=434`). The original needs the 0x400
`initHudMenu(2)` body (bb.438) as PredBB, so the 0x200 body branches forward over the
campaign chain to the shared `blx` at `0x17afde`. That single choice moves the whole
`[test][final tail][canary]` cluster to `0x17afe2` (original) instead of the function end
(base `0x184e`/`0x1976`), and is the source of the ~20 `b.w 0xec0` vs `b.w 0x184e`
structural diffs.

The PredBB is fixed by the first `MachineBlockPlacement` pass, i.e. by branch
probabilities and block sizes, not by source spelling. Every content-correct spelling tried
here changes the block sizes enough to flip the first-pass layout away from the original.

## 6. Reproduction

```
cp /dev/shm/w7713/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7713/rawscore.py        # 81.9607 / 8080
python3 /dev/shm/w7713/secdiff.py         # .text 85,379 / .rel.plt 0
python3 /dev/shm/w7713/multiset2.py       # content deltas in section 1
```
