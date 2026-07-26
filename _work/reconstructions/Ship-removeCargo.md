# Ship::removeCargo(int, int) deferred reconstruction

Target: `_ZN4Ship11removeCargoEii`

- Original: `0x174e64`, 118 bytes.
- Current: 118 bytes, 97.9% structural match.
- TU: `src/game/ship/Ship.cpp`.
- `Ship::cargo` is at `+0x70`.

The sole executable mismatch is entry scheduling:

```text
original                         current
ldr r1, [r0, #0x70]              ldr r1, [r0, #0x70]
mov r8, r2                        cbz r1, <null path>
mov r4, r0                        mov r8, r2
cbz r1, <null path>              mov r4, r0
```

The saved `amount` in `r8` is first used by `rsb r1, r8, #0` at original
`0x174e96`. The saved `this` in `r4` is first used after the null branch at
`0x174e88`. Both copies are therefore dead on the null path, so LLVM may
legally sink them. Ordinary pointer/integer aliases, condition inversion,
unified results, and goto-style exits collapse to the same SSA and liveness.

Forcing the copies before the branch would require an observable or volatile
null-path use, an extra call/member access, or another artificial compiler
barrier. Those forms either change behavior/code size or violate campaign
rules. Revisit only after translation-unit compiler state changes or new
source-provenance evidence.
