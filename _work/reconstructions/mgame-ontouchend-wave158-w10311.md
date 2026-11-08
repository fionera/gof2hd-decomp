# MGame::OnTouchEnd — wave 158 (w10311): the `(B & 1)` AND form lands at 8080 B with the `dockToStream` PLT entry intact

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10311`,
base `08575efff` (wave 156 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10311`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 156) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4391 | **4379** |
| aligned (align_diff) | 903 | **949** |
| `won` guard | `cmp #1; bne.w` | **`lsls #31; bne.n`** (AND form) |
| `dockEvent` blx `dockToStream` | 0x683a0 | **0x683a0** (exact) |

## 1. Landed source

Two edits in `MGame::OnTouchEnd`:

```cpp
// starMap block, line 1482: a mild cold hint on the starMap tap
if (__builtin_expect(!this->starMap->OnTouchEnd(x, y), 0)) { // lint: arm_builtin
    return;
}

// won guard, line 1554: the AND exception to calcZeroHeuristics
bool won = Globals::status->getMission()->hasWon() ||
           (((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() & 1) != 0;
```

The `& 1` form is the wave-157 "AND exception": `BranchProbabilityInfo::calcZeroHeuristics`
bails when the compare LHS is an `and` with a power-of-2 mask, so the branch keeps the
default 50:50 weight and the 8080 B block layout.  The backend lowers it to
`lsls r0, r0, #31; bne.n body` (2 B + 2 B) instead of the original `cmp r0, #0; beq.w else`
(2 B + 4 B); both guards are 6 B, so the function size is unchanged.

The un-hinted AND form (wave 157) measures raw **4371** / aligned **904**, but puts the
`resetStartSequenceOver` relocation at `0xca6`, before the starMap block's `dockToStream`
relocation at `0xce4`.  Gold then creates the `resetStartSequenceOver` PLT entry first and
`MGame::dockEvent`'s `blx dockToStream` moves `0x683a0 -> 0x683ac` (byte_exact 4507 -> 4506).

## 2. The PLT fix: a cold hint on the starMap tap

`__builtin_expect(!starMap->OnTouchEnd(x,y), 0)` is a semantic no-op, but it makes the
starMap block cold.  MachineBlockPlacement moves the whole starMap block *before* the
cutscene guard, so `dockToStream`'s relocation (`0xc8c`) now precedes
`resetStartSequenceOver`'s (`0xd28`).  `dockToStream` keeps PLT slot `0x683a0` and
`MGame::dockEvent` is byte-exact again.  The hint costs **+8 raw / +45 aligned** inside
`OnTouchEnd` relative to the un-hinted AND form, but the net against the wave-156 base is
**-12 raw / +46 aligned** and the gate is green.

PLT map (`objdump -d -j .plt`, entries around the affected run):

| slot | original | ours (this wave) |
|---|---|---|
| 0x683a0 | `PlayerEgo::dockToStream(bool)` | **same** |
| 0x683ac | `MGame::useCloak()` | `LevelScript::resetStartSequenceOver()` |
| 0x683b8 | `MGame::maneuverTouchEnd(int,int,void*)` | `MGame::useCloak()` |
| 0x683c4 | `Mission::isInstantActionMission()` | `MGame::maneuverTouchEnd(int,int,void*)` |
| 0x683d0 | `LevelScript::resetStartSequenceOver()` | `Mission::isInstantActionMission()` |

`resetStartSequenceOver` is only referenced from `OnTouchEnd`, and `useCloak` /
`maneuverTouchEnd` / `isInstantActionMission` are only referenced from `OnTouchEnd` too,
so the partial shift of those three entries does not touch any byte-exact function.
`dockEvent` is byte-exact (`verify --show _ZN5MGame9dockEventEii`: same 1300 B @0x17f920).

## 3. Measured spellings (all on the wave-156 base)

| variant | size | raw | aligned | PLT dockToStream | note |
|---|---:|---:|---:|---|---|
| base (`== true`) | 8080 | 4391 | 903 | 0x683a0 | wave-156 |
| `A \|\| B` (truthy) | 8060 | 6069 | 1871 | 0x683a0 | 20:12 zero heuristic |
| `A \|\| (B & 1) != 0` (AND) | 8080 | **4371** | **904** | **0x683ac** | wave-157, dockEvent broken |
| `A \|\| (B & 1) != 0` + starMap `expect(...,0)` | 8080 | **4379** | 949 | **0x683a0** | **landed** |
| `A \|\| (B & 1) != 0` + starMap `expect(...,1)` | 8072 | — | — | 0x683ac | re-lay |
| `A \|\| (B & 1) != 0` + `expect(won,1)` | 8096 | 4787 | 864 | 0x683a0 | size regression |
| `A \|\| (B & 1) != 0` + `expect(won,0)` | 7832 | 5146 | 2162 | 0x683ac | re-lay |
| `if (failed)` + AND + `expect(won,1)` | 8080 | 4782 | 1979 | 0x683a0 | failed guard truthy |
| `if (failed)` + AND | 8080 | 4723 | 2023 | 0x683ac | re-lay |
| `if (failed)` alone | 8076 | 4711 | 1968 | 0x683a0 | 4 B short |
| `if ((failed & 1) != 0)` | 8080 | 4958 | 987 | 0x683a0 | `lsls r4,#31` |
| `bool a=A; bool b=B; bool won=a\|\|b` | 8080 | 4752 | 2001 | 0x683a0 | `orrs`, no short-circuit |
| `unsigned char won=A\|B` | 8080 | 4752 | 2001 | 0x683a0 | `orrs`, no short-circuit |
| `if (!(A \|\| (B&1)!=0)) {reset} else {body}` | 8080 | 4371 | 904 | 0x683ac | explicit form, same as AND |
| `switch (won)` / `do/while` / `while (won)` | 8080 | 4391 | 903 | 0x683a0 | folded to `== true` |
| `if (won == false)` / `!= true` / `!!won` | 8080 | 4391 | 903 | 0x683a0 | folded to `== true` |

Other `__builtin_expect` sites tried on the starMap block:
`expect(starMap->exitRequested,0)` (raw 5202 / aligned 1259, PLT fixed),
`expect(!starMap->OnTouchEnd,0)` on the un-negated call (raw 5040 / aligned 3209, size 8072),
`expect(starMapOpen,0/1)`, `expect(cutsceneActive,0/1)`,
`expect(layout->choiceWindowOpen,0/1)` (PLT still reset-first),
`expect(touchesStream,0/1)` (PLT still reset-first).

## 4. Why no exact `cmp #0; beq` at 8080 B

`calcZeroHeuristics` assigns `icmp eq i8 %B, 0` the 20:12 weight; the original's PGO profile
gives it a 50:50-equivalent layout.  The only static 50:50 escape is the `and pow2` bail,
which the ARM Thumb1 backend lowers to `lsls #31` (Thumb1 has no cheap `tst #1`).  Every
full-value `cmp #0` spelling (`A || B`, `if (failed)`, `bool won=A; if(!won) won=B;`) is
20:12 and re-lays to 8060/8076.  The `orrs` spelling reaches a full-value Z test at 8080 B
but drops the `||` short-circuit and re-lays the whole function (raw 4752 / aligned 2001).
So the landed AND form is the best 8080 B `cmp #0`-family shape; the literal
`cmp r0,#0; beq.w` remains the last unrecoverable PGO signature in this guard.

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10311/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4379 / 8080
python3 /dev/shm/w10311/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 949
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w10311/{fdiff.py,align_diff.py,build_obj.sh,batch_inspect.py,
inspect_failed.py,measure.sh,variants.py,gen2..gen23.py,g2..g23/}`.
The authoritative binary is `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
