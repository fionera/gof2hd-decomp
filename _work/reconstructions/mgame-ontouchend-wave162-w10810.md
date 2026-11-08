# MGame::OnTouchEnd — wave 162 (w10810): the missing 23rd `isCampaignMission` call

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10810`, base `29248c1d0` (wave-161 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10810`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 161) | this wave |
|---|---:|---:|
| size | 8080 | **8080** (exact) |
| raw same-address diff | 4127 | **5816** (+1689) |
| aligned (`align_diff.py`) | 906 | **1379** (+473) |
| `isCampaignMission` calls | 22 | **23** (== original) |
| `.rel.plt` differing bytes | 0 | **0** (byte-identical) |
| `.text` differing bytes | 85,374 | **87,063** (+1689) |
| `.ARM.extab` differing bytes | 725 | **735** (+10) |
| byte_exact | 4507/4524 | **4507/4524** |
| linked_exact | 4507/4524 | 4507/4524 |

`check` green: byte 4507, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.

## 1. The missing 23rd call (confirmed)

The original has **19** campaign-chain `isCampaignMission` sites plus 4
outside the chain (failed branch, `keepObjectives`, `nextCampaignMission`,
type-183 escort) = **23**.  Our source had 22 because the chain block

```cpp
} else if (mission->isCampaignMission() &&
           (getCurrentCampaignMission() == 96 || getCurrentCampaignMission() == 127)) {
```

calls `isCampaignMission()` once and `getCurrentCampaignMission()` twice.
The original instead tests the two missions in **two different places in the
chain** and tail-merges the identical bodies:

* `0x17b70a` `... && getCurrentCampaignMission() == 96` -> body `0x17b722`
  (falls through to the shared `departStation(98)` tail at `0x17b77c`);
* `0x17bc26` `... && getCurrentCampaignMission() == 127` -> `beq.w 0x17b722`
  (a *backward* jump into the 96 body), placed **after 126 and before 134**.

So the original source order is
`15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120, 126, 127, 134, 144, 155, 161, 162`
— our HEAD has `161` right after `95` and the combined `96||127` block just
before `162`.  Splitting the block and restoring the original order produces
the 23rd call.

## 2. Size: the split costs +8..+12 unless `failed` is also truthy

A 162-placement sweep (96/127/161 positions, split form) on the wave-161 base
never reached 8080: sizes were 8088 (42x), 8092 (38x), 8096 (72x), 8100 (10x).
The compiler tail-merges the two identical bodies, so the split itself costs
only the extra `mov r0,r5; blx isCampaignMission` guard (+6 B) plus the
branch-width reshuffle.

The original's `failed` guard is **truthy** (`0x17ab36: cmp r4,#0; beq.w`),
HEAD's is `if (failed == 1)` (`cmp r4,#1; bne.w`).  Changing it to
`if (failed)` is semantically identical and re-lays the function by exactly
-12 B, which makes the faithful order + split land on **8080**:

| variant (all split) | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| faithful order, `failed == 1` | 4667 | 894 | 8092 | 0 |
| **faithful order, `if (failed)` (landed)** | **5816** | **1379** | **8080** | **0** |
| base order, `if (failed)` | 6079 | 2007 | 8076 | 0 |
| faithful order + truthy `won` + `if (failed)` | 4663 | 1428 | 8096 | 0 |
| faithful order + `arm800` + `if (failed)` | 5829 | 1399 | 8080 | 4 |
| faithful order + `arm800` + starMap cold + `if (failed)` | 5830 | 1441 | 8080 | 0 |
| faithful order + `arm800` + starMap cold + `expect(won,1)` | 4440 | **682** | 8084 | 18 |

A further 162-placement sweep with `if (failed)` found 64 shapes at 8080 B /
`.rel.plt` 0; the best raw is 5761
(`[95,161,100,110,120,126,134,127,144,155,96,162]`, aligned 2008), i.e. the
faithful order is *not* the raw-best but is the only order that matches the
original's chain sequence.

## 3. What the landed shape reproduces

* 23 `isCampaignMission` calls (was 22).
* The original chain order: 96 immediately after 95, 127 after 126.
* The 127 block is a backward `beq.w` into the 96 body (tail merge).
* The **late join**: `ldr.w r0,[fp,#248]; cbz r0, <closeHudMenu body>` is
  emitted after the `useCloak` / `initHudMenu` bodies (ours `0x17af60` /
  `0x17afb0`, original `0x17afe2` / `0x17b01a`).
* `.rel.plt` byte-identical (`dockToStream` before `useCloak`).

The residual is block placement: the shared return epilogue sits at
`0x17af82` in ours vs `0x17b004` in the original (a 130 B shift of the middle
region), which is what inflates the raw metric from 4127 to 5816 even though
the chain sequence now matches.  Every 8080 B split shape found has
raw >= 5761; no shape with the split and raw < 4127 was found.

## 4. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10810/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 5816 / 8080
python3 /dev/shm/w10810/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 1379
python3 /dev/shm/w10810/secdiff.py                                # .rel.plt 0
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10810/{fdiff.py,align_diff.py,secdiff.py,gen2.py,
batch2.py,sweep2.py,sweep2.json,v5..v10.json,blocks.py,seqdiff.py}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
