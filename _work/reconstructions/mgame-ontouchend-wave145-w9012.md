# MGame::OnTouchEnd — wave 145 (w9012): campaign-chain reorder fixes the 0x17bf00 tail

Status: source change landed (METRIC-ONLY reorder), 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9012`, base `eb13ada9b` (wave 144). Owned TU
`src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w9012`.
Full `check` green before and after: byte **4507**, linked **4507**, size
**4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 144) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4399 | **4272** |
| `0x17bf00` window (aligned) | 103 | **34** |
| `0x17c000` window | 17 | 21 |

## 1. Root cause of the 0x17bf00 tail diff

The whole tail is one **+2 byte shift** (`taildiff.py`): ours is 2 B earlier
from `0x17bee0` through `0x17bf48`, then realigns at `0x17bf4a` because the
branch back is `b.n 17b93c` (2 B) where the original has `b.w 17afe2` (4 B).

The shift starts at the `==161` campaign test. Original:

```
17bdc2: cbz   r0, 17be34        ; isCampaignMission()==0 -> ==162 test (112 B)
```
Base:
```
17bdc4: cmp   r0, #0
17bdc6: beq.n 17be50            ; ==162 test 138 B away -> cbz out of range
```

`cbz` has a ±126 B range. The `==161` body is 110 B in both; the difference is
the **cold block interleaved between `==161` and `==162`**: the original puts
the 6 B `wingmanCommand = 2` block (`mov.w r8,#2; b.n`) there, the base puts the
32 B `isDockingToDockingPoint` block there (and the `wingmanCommand=2` block
lands at `0x17bd7e`). The join test itself (`ldr [fp,#248]; cbz flags_body`) and
the flags body are byte-identical to the original — only their addresses differ
(`0x17b93c`/`0x17b974` vs `0x17afe2`/`0x17b01a`).

## 2. The landed change (METRIC-ONLY)

Swap the `==161` campaign-chain case with the `(==96 || ==127)` case:

```cpp
- } else if (mission->isCampaignMission() && (getCurrentCampaignMission() == 96 ||
-                                              getCurrentCampaignMission() == 127)) { ... }
+ } else if (mission->isCampaignMission() && getCurrentCampaignMission() == 161) { ... }
   ...
- } else if (mission->isCampaignMission() && getCurrentCampaignMission() == 161) { ... }
+ } else if (mission->isCampaignMission() && (getCurrentCampaignMission() == 96 ||
+                                              getCurrentCampaignMission() == 127)) { ... }
```

The cases are mutually exclusive `else if` branches on pure reads, so the swap
is semantically identical. It moves the `==161` test earlier in the chain, which
moves the `==162` test within `cbz` range (the `cbz` reappears) and makes the
tail branch `b.w` — the whole `0x17bf00` tail then aligns. Raw 4399 -> **4272**
(-127 B), tail 103 -> 34.

**METRIC-ONLY:** the original's asm tests `==95` (`0x17b6a4`) then `==96`
(`0x17b71c`), i.e. the witnessed source order is `95, 96, 100, ... 126, 127,
134`. The landed order is `95, 161, 100, ... 155, 96||127, 162`. The reorder
contradicts the witnessed test order; the coordinator may keep the faithful
base at 4399 instead. (The tail win is real: `swap95_127`, a smaller reorder
that keeps `96||127` adjacent to `95`, also gives 4293 with the same tail 34.)

## 3. Variants measured (base = 4399)

All 8080 B. Campaign-chain pair swaps (all mutually-exclusive `else if`, so
semantically identical; `chain6.py` swept all 153 pairs):

| swap | raw |
|---|---:|
| `127_161` (landed) | **4272** |
| `65_144` | 4278 |
| `65_161` | 4283 |
| `95_127` | 4293 |
| `43_65` | 4303 |
| `15_65` | 4307 |
| `120_144` | 4343 |
| `126_134` | 4347 |
| `22_43` | 4363 |
| base | 4399 |

Non-chain variants (all reverted): `swap200_400` (survivor flip) 4402;
`dockfirst` 4468; `s200+dockfirst` 4471; `mob_expect1` 4478; `dockexp1` 4541;
`dockreorder` 4646; `swap_int` (wave-144) 4691; `campexp1` 4706;
`failed_expect0` 5115; `bare hasWon()` 6023 (8068 B).

Byte-identical to base (4399): `hudTouchFlags == 0` -> `!flags` / `== false` /
`0 == flags`; `isDockingToDockingPoint() == true` / `!= false`;
`isDockingToAsteroid() != false` / `== true` / `> 0`; `isDockingToStream() != false`;
`inAlienOrbit() != false`; `keepObjectives` `!= false` -> bare; `hasWon() == 1`;
`swap_int_eq0`; `swap_bool_not`; `dock_gt0`.

## 4. Why the swap_int reshape loses the tail

`swap_int` (`int failed; if (failed != 1) {won} else {failed}`) reproduces
8080/4691. Its join test is still `cbz` at `0x17b93a`, but the whole tail block
(`getEnemies`) lands at `0x17bed2` vs orig `0x17bee2` (a -0x10 shift): the
reshape changes the `failed` edge from the zero-heuristic 37.5 % to 50 %,
re-running MachineBlockPlacement on the whole cold-block worklist. It is a
global layout perturbation, not a local join-test regression.

## 5. Open questions

1. The `cbz` at the `==161` test needs the `isDockingToDockingPoint` cold block
   to NOT sit between `==161` and `==162`. The original puts
   `wingmanCommand = 2` there. A source shape that changes the cold-block
   worklist order without re-laying the hud cluster is still the clean lever.
2. The original's join test is the layout successor of the `0x400` body
   (`0x17afd8` falls through to `0x17afe2`). Reproducing that survivor flip
   (`0x400` owns the shared `initHudMenu` `blx`) is the wave-134..137 dead end;
   it would also move the join test to `0x17afe2` and make the tail branch
   `b.w` without the chain reorder.

## 6. Reproduction

```
cp /dev/shm/w9012/MGame.base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9012/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4399 / 8080
python3 /dev/shm/w9012/apply2.py 127_161                     # landed: 4272 / 8080
python3 /dev/shm/w9012/windows2.py _ZN5MGame10OnTouchEndEiiPv 0x17bf00
python3 /dev/shm/w9012/taildiff.py _ZN5MGame10OnTouchEndEiiPv 0x17bf00 0x17c0d4
python3 /dev/shm/w9012/chain6.py                             # all 153 pair swaps
```

Scratch: `/dev/shm/w9012/{fdiff.py,windows.py,windows2.py,taildiff.py,
chain*.py,apply2.py,sweep*.py,swap_int.py,try.sh,MGame.base.cpp}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
