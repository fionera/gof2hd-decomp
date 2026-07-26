# Ship::setMods deferred reconstruction

Target: `_ZN4Ship7setModsEP5ArrayIiE`

- Original: `0x1756ce`, 174 bytes.
- Current: 174 bytes, 98.4% structural match.
- TU: `src/game/ship/Ship.cpp`.
- Relevant layout: `Ship::mods` at `+0x78`, `slots` at `+0x68`,
  `equipment` at `+0x6c`, and `numAddedDeviceSlots` at `+0x7c`.
- `Array<int>` size and data fields are read at `+0x0` and `+0x4`.

The sole executable mismatch is entry scheduling:

```text
original                         current
mov r5, r1                       mov r9, r0
mov r9, r0                       cmp r1, #0
cmp r1, #0                       str r1, [r0, #120]
str r1, [r0, #120]               beq <refreshValue>
beq <refreshValue>               mov r5, r1
```

Everything after this five-instruction window is structurally identical,
apart from expected link-address relocations.

Attempts exhausted:

1. Removed the local pointer alias and used `mods` directly. No byte changed.
2. Combined member assignment, alias creation, and null testing in a condition
   declaration. No byte changed.
3. Replaced the outer null check with a null-aware ternary loop bound. This
   moved `mov r5, r1` to the desired entry position, but emitted an extra
   `ite`/zero-bound path and grew the function from 174 to 176 bytes.
4. Replaced the ternary with a short-circuit `mods != 0 && i < mods->size()`
   loop condition. Clang folded it back to the unchanged 174-byte baseline.

Hypothesis: this is a MachineSink/register-allocation difference tied to the
translation-unit compiler state. The ternary experiment proves that longer
parameter liveness can force the desired early `r5` copy, but no tested legal
source form retained that liveness while preserving the original null-branch
CFG. Revisit after nearby TU changes or with a compiler-state experiment; do
not repeat the four forms above.
