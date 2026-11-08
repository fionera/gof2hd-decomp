# MGame::OnTouchEnd — wave 163 (w10911): `bool Mission::hasWon` kept, raw 6,053 -> 5,742

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10911`, base `e51db2fc7` (wave-162 final).
Scratch `/dev/shm/w10911`. Target `_ZN5MGame10OnTouchEndEiiPv`,
original 8,080 B @ 0x17a144.

| | base (wave 162) | this wave |
|---|---:|---:|
| size | 8,080 | **8,080** (exact) |
| raw same-address diff | 4,127 | **5,742** (+1,615) |
| aligned (`align_diff.py`) | 906 | **1,109** (+203) |
| `.rel.plt` diff | 0 | **0** (29,392 B byte-identical) |
| `.text` diff | 85,042 | **86,657** (+1,615) |
| `.ARM.extab` diff | 725 | **759** (+34; the bool return changes the function's LSDA) |

Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS.  All section sizes exact, file size exact 2,187,328 B.

## 1. What was landed

`Mission::hasWon()` now returns `bool` (and the `won` field is `bool`), which is
the original's shape: clang emits `cmp r0,#0; beq.w 17b0aa` on the i1 directly
(no `ICmpInst`, so no zero-heuristic) instead of `cmp r0,#1; bne.w`.  This is
w10811's finding and it is correct.

The w10811 note attributed the raw regression to a mining-block RA hoist of
`add.w r8, sp, #80` (0x17a5f6 -> 0x17a5e8).  **That hoist is already present in
the wave-162 base build** (unsigned-char `hasWon`): both base and bool put
`add.w r8, sp, #80` at 0x17a5e8.  The mining block is *identical* between the
two builds; the bool type does not perturb it.  The note's diagnosis is wrong.

## 2. Where the +1,926 raw actually comes from (canonical bool = 6,053)

Splitting the function into fixed address bands and counting same-address byte
mismatches (base e51db2fc7 vs original):

| band | base | canonical bool |
|---|---:|---:|
| 17a144..17a5e8 | 18 | 11 |
| 17a5e8..17a7f2 | 27 | 32 |
| 17a7f2..17adc8 | **77** | **1,426** |
| 17adc8..17b004 | 549 | 542 |
| 17b004..17ba18 | 2,441 | 2,435 |
| 17ba18..17c0d4 | 1,015 | 1,607 |

The whole regression is the 17a7f2..17adc8 band.  With `bool hasWon` the
compiler places the stack-canary epilogue at **0x17afec** instead of the
original **0x17b004**.  The `releaseAllKeys` block's tail branch at 0x17a7f2 is
then only 0x7fa = 2,042 B away and shrinks from `b.w` (4 B) to `b.n` (2 B),
which shifts the literal pool and every following instruction by 2 B -> 1,426
mismatched bytes.  The original is 0x812 = 2,066 B away, just outside `b.n`
range, so it keeps `b.w`.

The epilogue moves 24 B earlier because the compiler moves the
`else if (mission->isCampaignMission())` reward block *out* of the hot region
(to 0x17b05a, after the epilogue) whereas the original keeps it inline at
0x17afa6.  That is a branch-weight/block-placement difference, not a mining
block difference.

## 3. The landed shape

Keeping `bool hasWon` and marking the reward-chain campaign test cold:

```cpp
} else if (__builtin_expect(mission->isCampaignMission(), 0)) { // lint: arm_builtin
    Globals::status->nextCampaignMission(true);
}
```

Measured: raw **5,742** (from 6,053), aligned **1,109**, size **8,080** exact,
`.rel.plt` diff **0**.  The hint moves the tail blocks enough to recover 311
raw bytes (the 17ba18..17c0d4 band drops 1,607 -> 1,258) without changing the
function size.  `__builtin_expect` needs the same-line `// lint: arm_builtin`
waiver (goal_lint's baseline for MGame.cpp is 0).

## 4. Measured alternatives (all `bool hasWon` unless noted)

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| base `B == true` (unsigned char, wave 162) | 4,127 | 906 | 8,080 | 0 |
| canonical bool `A \|\| B == true` | 6,053 | 1,085 | 8,080 | 0 |
| truthy `A \|\| B`, `(bool)B`, `B != false`, `B == 1`, `!(!A&&!B)`, `A?true:B`, `(A\|\|B)==true`, `bool a=A; a\|\|B` | 6,053 | 1,085 | 8,080 | 0 |
| `bool b=B; A\|\|b` (B evaluated first) | 4,703 | 1,470 | 8,096 | 17 |
| `A \| B` | 4,708 | 1,469 | 8,092 | 17 |
| `B ? true : A` | 4,679 | 985 | 8,088 | 0 |
| `expect(B,1)` / `expect(B,0)` | 6,053 | 1,085 | 8,080 | 0 |
| **`expect(isCampaignMission(),0)` (landed)** | **5,742** | **1,109** | **8,080** | **0** |
| `expect(isCampaignMission(),1)` | 6,136 | 1,111 | 8,084 | 0 |
| `expect(isInstantActionMission(),0)` | 4,218 | 1,907 | **8,088** | 8 |
| `expect(isInstantActionMission(),0)` + campaign 0 | 4,319 | 1,873 | **8,088** | 8 |
| `expect(isInstantActionMission(),1)` | 5,195 | 2,040 | 7,852 | 12 |

`expect(isInstantActionMission(),0)` is the closest raw (4,218) and restores the
epilogue to 0x17b004 (the 17a7f2..17adc8 band drops to 43), but it is **+8 B**
(8,088): the EH cleanup path gains two `String::~String` landing-pad calls, so
it is not landable until a -8 B companion is found.  That is the top open
question for the next wave.

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10911/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 5742 / 8080
python3 /dev/shm/w10911/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 1109
python3 /dev/shm/w10911/relplt.py                                # diff 0
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10911/{fdiff.py,align_diff.py,relplt.py,regiondiff.py,
shiftprof.py,sweep*.sh,batch*.py,bak/}`.
