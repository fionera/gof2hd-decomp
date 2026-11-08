# MGame::OnTouchEnd — wave 158 (w10411): folding the dead `hudTouchFlags == 0` block is a both-metric win at 8080 B

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10411`,
base `9e4795206` (wave 157 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10411`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 157) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4391 | **4387** |
| aligned (align_diff) | 903 | **901** |
| `dockToStream` PLT slot | `0x6838c` | **`0x6838c`** (identical) |
| `MGame::dockEvent` byte diff | 0 | **0** (1300 B) |

## 1. Landed source (one hunk)

The `hudTouchFlags == 0` block is dead after the wingman arm: `wingmanCommand >= 0`
means one of the wingman bits (`0x20000/0x40000/0x80000/0x100000`) was set, so
`hudTouchFlags != 0`.  The block was a separate `if` inside the `else` of
`if (weaponSlot >= 0)`.  Fold it into the same `else if` chain:

```cpp
        if (weaponSlot >= 0) {
            ...
        } else if (wingmanCommand >= 0) {      // was: } else { if (wingmanCommand >= 0) {
            ...
        } else if (this->hudTouchFlags == 0) { // was: } if (this->hudTouchFlags == 0) {
            ...
        }                                      // was: } (extra outer else close)
```

This is a pure dead-code fold: the `hudTouchFlags == 0` arm is unreachable when
`wingmanCommand >= 0`, and the weaponSlot arm never entered the old `else` either.
The `won`/`failed` spellings are untouched (`failed == 1`, `hasWon() == true`), so the
50:50 branch weights and the original block order are preserved.

## 2. PLT map (identical to the original)

`objdump -R` `.rel.plt` order, entries around the affected run (base `0x5e7ec`, 3674
jump slots; `dockToStream` index 3320):

| slot | original | ours |
|---|---|---|
| 0x68374 | `MGame::switchCamera(int)` | same |
| 0x68380 | `MGame::UseKhadorDrive()` | same |
| 0x6838c | `PlayerEgo::dockToStream(bool)` | **same** |
| 0x68398 | `MGame::useCloak()` | same |
| 0x683a4 | `MGame::maneuverTouchEnd(int,int,void*)` | same |
| 0x683b0 | `Mission::isInstantActionMission()` | same |
| 0x683bc | `LevelScript::resetStartSequenceOver()` | same |
| 0x683c8 | `Level::removeObjectives()` | same |

`MGame::dockEvent` is byte-exact (`fdiff`: 0/1300).  The fold does not move any
relocation, so the whole PLT run is byte-identical.

## 3. Why the AND-form `won` cannot be landed with this fold

The wave-157/158 AND exception `(hasWon() & 1) != 0` keeps the 50:50 weight and gives
raw **4367/902** with this fold, but the backend lowers it to `lsls #31; bne.n body`
instead of `cmp #1; bne.w else`: the `won` **else** block becomes the fall-through and
MachineBlockPlacement puts it immediately after the guard, *before* the starMap block.
`resetStartSequenceOver` (in that else) then precedes `dockToStream` in relocation
order, so gold gives `resetStartSequenceOver` the earlier PLT slot and
`PlayerEgo::dockToStream` moves `0x6838c -> 0x68398`; `MGame::dockEvent`'s `blx`
becomes 1 byte different (byte_exact 4507 -> 4506).  The wave-158 w10311 `starMap`
cold hint repairs the PLT order but re-lays the hot region: `fold + AND + star0`
is **4375/956**, i.e. raw better but aligned 55 worse than the base.

Measured on this wave's base (all 8080 B unless noted; PLT = `dockToStream` address):

| variant | raw | aligned | PLT | note |
|---|---:|---:|---|---|
| base | 4391 | 903 | 0x6838c | wave 157 |
| **landed: fold** | **4387** | **901** | **0x6838c** | **this wave** |
| fold + AND `(B&1)!=0` | 4367 | 902 | 0x68398 | dockEvent -1 |
| fold + AND + starMap `expect(!OnTouchEnd,0)` | 4375 | 956 | 0x6838c | w10311 shape + fold |
| AND alone | 4371 | 904 | 0x68398 | w10311 un-hinted |
| AND + starMap expect | 4379 | 949 | 0x6838c | w10311 landed |
| fold + faithful dock owners | 4404 | 920 | 0x6838c | w10312 shape |
| fold + dock owners + AND | 4384 | 921 | 0x6838c | |
| fold + `won < 2` / `== 1` / `!= false` | 4387 | 901 | 0x6838c | canonicalised |
| fold + `won > 0` / `!= 0` / `!won` body swap | 8060 B | — | — | zero heuristic |
| fold + `(won & 1) == 0` body swap | 4387 | 901 | 0x6838c | InstCombine folds `& 1` |
| fold + `int won = A + B; (won&1)==0` | 4412 | 1451 | 0x68398 | no short-circuit |
| fold + `int won = A ? 1 : B; (won&1)==0` | 4367 | 902 | 0x68398 | still else fall-through |
| fold + `expect(won,1)` | 4752 | 835 | 0x6838c | size 8092 |
| fold + `expect(hudTouchFlags==0,0)` | 5777 | 1035 | 0x6838c | size 8068 |

The `(won & 1) == 0` inverted forms all canonicalise back to the same `lsls; bne body`
layout (the compiler knows `won` is 0/1), so the fall-through direction cannot be
flipped in source without a wide `cmp; bne` branch.  The only source escape that keeps
the else late is the original `cmp #1; bne.w` spelling, which is the base `== true`
form.  Hence the fold is the Pareto point: it removes the dead test, matches raw -4 and
aligned -2, and leaves the PLT and `dockEvent` untouched.

## 4. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10411/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4387 / 8080
python3 /dev/shm/w10411/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 901
python3 /dev/shm/w10411/pltmap3.py                                # dockToStream 0x6838c
python3 /dev/shm/w10411/fdiff.py _ZN5MGame9dockEventEii           # 0 / 1300
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10411/{fdiff.py,align_diff.py,pltmap3.py,calls.py,firstdiff.py,
gen.py,sweep.py,sweep2.py,measure.sh,MGame.base.cpp}`.
The authoritative binary is `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
