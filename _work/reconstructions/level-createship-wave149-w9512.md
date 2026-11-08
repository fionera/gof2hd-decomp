# `Level::createShip` — wave 149 / worker w9512

Target `_ZN5Level10createShipEiiiP8Waypointbb`, original 2,764 B.
Baseline (wave-148 master `fece77a64`): raw same-address diff **1,978**,
instruction-index alignment **486**, size 2,764.

## Landed: declare `wy` before `wx`

The original's null-waypoint branch spills `y`'s slot (`sp#48`) before
`x`'s (`sp#44`).  With `wx` declared first clang emits the stores in the
opposite order.  Swapping the two declarations makes the branch match:

```cpp
int wy = waypoint != nullptr ? waypoint->y : 0;
int wx = waypoint != nullptr ? waypoint->x : 0;
```

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (`wx` first) | 2,764 | 1,978 | 486 |
| **landed (`wy` first)** | **2,764** | **1,978** | **482** |

Gate: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
RATCHET PASS.

## Tried and not committed

Hoisting `int armor;` to the top of the function and assigning it at the
original `hardCoreMode()` site (`armor = ...`) was measured but did not
improve either metric; it was left uncommitted in the worktree.  The
remaining r4/r6 rotation of `status`/`nextInt` at entry is still open.
