# LevelScript::process — wave 183 harvest 4 expect cells

Target: `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, 57,232 B).

Base: `d0004f490` (raw 52,320, `.text` diff 74,885, total 75,551).
Result: raw **52,309** (-11), aligned 22,316 -> 22,317, size 57,232/57,232 exact,
insns 20,724/20,830. `.text` diff **74,874**, `.ARM.extab` **646** (size/base exact
156,312 B @ `0x1a3b78`), build-id 20, total **75,540**. `.rel.plt` byte-identical
(`289b2a069f8504d1e8a3919e43915e04`).

## Cells

1. Line 1448: add `__builtin_expect((m_nState == 2 && m_nScriptTimerA >= 2000), 0)`
   on the post-intro mission-0 timer guard.
2. Line 3151: remove `__builtin_expect(..., 1)` from the state-8 camera-move
   `m_nScriptTimerA >= 8001` guard.
3. Line 4735: add `__builtin_expect((ship->shipGroup == 9), 1)` in the
   wingman/group-9 activation loop.
4. Line 5184: add `__builtin_expect((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700), 0)`
   on the 5700-frame station-departure transition.

All four are whole-condition hints; each carries the same-line `// lint: arm_builtin`
waiver. The pair from cells 1+2 is size-neutral; cells 3 and 4 are the additional
whole-condition cells that the w12708 sweep did not land. This variant is better
than w12708's committed 52,402 (which used the 5177 cell instead of 4708/5174).

## Verification

`check` on the combined tree: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, stub/missing/wrong_type/extra **0**.
