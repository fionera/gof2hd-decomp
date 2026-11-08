# MGame::OnTouchEnd — wave 145 (w9012): tail `cbz` lever is the cold-block order, not the join test

Status: analysis-only, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9012`,
base `eb13ada9b` (wave 144). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9012`. **No source change landed** (base restored; full gate
unchanged: byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0). Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

Baseline re-measured: **8080/8080, raw same-address diff 4399**.

## 1. The 0x17bf00 tail diff is a 2-byte shift, and its cause is the `==161` test

Per-0x100 window (aligned to `0x17bf00`) the base tail is:

```
0x17bf00  103
0x17c000   17
```

`taildiff.py` shows the whole tail is one **+2 byte shift**: ours is 2 B earlier
from `0x17bee0` through `0x17bf48`, then realigns at `0x17bf4a` because the
branch back is `b.n 17b93c` (2 B) where the original has `b.w 17afe2` (4 B).

The shift starts at the `==161` campaign test. Original:

```
17bdc2: cbz   r0, 17be34        ; isCampaignMission()==0 -> ==162 test (112 B away)
```
Ours:
```
17bdc4: cmp   r0, #0
17bdc6: beq.n 17be50            ; ==162 test is 138 B away -> cbz does not fit
```

`cbz` has a ±126 B range. The `==161` body is 110 B in both. The difference is
the **cold block interleaved between `==161` and `==162`**:

| between `==161` body and `==162` test | original | base |
|---|---|---|
| cold block | `mov.w r8,#2; b.n 17bee2` (6 B, `wingmanCommand = 2` from the `0x80000` dispatch case) | `isDockingToDockingPoint` block (32 B, `0x17be30`) |

In the base the `wingmanCommand = 2` block is instead placed at `0x17bd7e`
(between the `==144` body and the `==155` test). So the two cold blocks
(`wingmanCommand=2`, `isDockingToDockingPoint`) are **swapped** relative to the
original. That is the entire cause of the 2-byte tail shift; the join test
itself (`ldr [fp,#248]; cbz flags_body`) and the flags body are byte-identical
to the original, only at `0x17b93c`/`0x17b974` instead of `0x17afe2`/`0x17b01a`.

## 2. The wave-144 `swap_int` tail loss, explained

`swap_int` (`int failed; if (failed != 1) {won} else {failed}`) reproduced:
**8080 B, raw 4691**. Its join test is still `cbz` at `0x17b93a` (2 B earlier
than base), but the tail block (`getEnemies`) lands at `0x17bed2` vs orig
`0x17bee2` — a **-0x10 shift** — so `0x17bf00` 103 -> 242 and `0x17c000`
17 -> 107. The reshape changes the `failed` edge from the zero-heuristic 37.5 %
to 50 %, which re-runs MachineBlockPlacement on the whole cold-block worklist
and moves the tail cluster. It is a global layout perturbation, not a local
join-test regression, so no local tail edit can compensate it (see §3).

## 3. Variants measured (all reverted; base restored)

All are 8080 B unless noted. Raw diff lower is better; base = 4399.

| variant | raw | note |
|---|---:|---|
| base | **4399** | reference |
| `swap200_400` (swap the `0x200`/`0x400` dispatch cases) | 4402 | flips the shared-`blx` survivor to `0x400` (wave-142 `F`), 3 B worse |
| `dockfirst` (move `isDockingToDockingPoint` first in the dock chain) | 4468 | |
| `s200+dockfirst` | 4471 | |
| `mob_expect1` (`__builtin_expect(menuTouchOpen,1)`) | 4478 | |
| `s200+mobexp1` | 4481 | |
| `dockexp1` (`__builtin_expect(isDockingToDockingPoint(),1)`) | 4541 | |
| `dockreorder` (move `isDockingToDockingPoint` after `isDockingToStream`) | 4646 | |
| `swap_int` | 4691 | wave-144 reshape, tail -0x10 |
| `campexp1` (`__builtin_expect(mission->isCampaignMission(),1)`) | 4706 | defers cargo like swap_int, worse |
| `swapint+join_after_disp` | 4761 | |
| `dockexp0` | 4783 | |
| `base+join_after_disp` | 4799 | |
| `failed_expect0` | 5115 | |
| `bare hasWon()` | 6023 | 8068 B, wave-143 dead end |

Byte-identical to base (4399): `hudTouchFlags == 0` -> `!flags` / `== false` /
`0 == flags`; `isDockingToDockingPoint() == true` / `!= false`;
`isDockingToAsteroid() != false`; `isDockingToStream() != false`;
`inAlienOrbit() != false`; `keepObjectives` `!= false` -> bare;
`hasWon() == 1`; `swap_int_eq0`; `swap_bool_not`.

## 4. Why the obvious tail levers fail

- Moving the flat `hudTouchFlags == 0` test (before the weaponSlot body, or
  before the dispatch chain) is 4799-5536 — it re-lays the whole hud cluster.
- `__builtin_expect` on the join test (`flags==0`, 0/1) on top of `swap_int`
  gives 4723/4743 — still the swap_int layout.
- `__builtin_expect` on `menuTouchOpen`/`cutsceneActive`/`_bc8`/`hudMenuOpen`
  re-lays the function (4478-7413); none beats 4399.

## 5. Open questions

1. The `cbz` at the `==161` test needs the `isDockingToDockingPoint` cold block
   to NOT sit between `==161` and `==162`. The original puts `wingmanCommand=2`
   there. A source shape that changes the cold-block worklist order (without
   re-laying the hud cluster) is the lever. `swap200_400` is the closest (4402)
   but still 3 B worse and flips the survivor.
2. The original's join test is the layout successor of the `0x400` body
   (`0x17afd8` falls through to `0x17afe2`). Reproducing that survivor flip
   (`0x400` owns the shared `initHudMenu` `blx`) is the wave-134..137 dead end;
   it would also put the join test at `0x17afe2` and make the tail branch `b.w`.

## 6. Reproduction

```
cp /dev/shm/w9012/MGame.base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9012/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4399 / 8080
python3 /dev/shm/w9012/windows2.py _ZN5MGame10OnTouchEndEiiPv 0x17bf00
python3 /dev/shm/w9012/taildiff.py _ZN5MGame10OnTouchEndEiiPv 0x17bf00 0x17c0d4
python3 /dev/shm/w9012/sweep*.py   # the variant sweeps in §3
```

Scratch: `/dev/shm/w9012/{fdiff.py,windows.py,windows2.py,taildiff.py,
sweep.py..sweep10.py,swap_int.py,try.sh,MGame.base.cpp}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
