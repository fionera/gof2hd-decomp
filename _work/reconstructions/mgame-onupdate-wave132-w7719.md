# MGame::OnUpdate wave 132 (w7719): body `Globals::status` use is elided (no source win)

Date: 2026-11-07. Worktree `wt/w7719`. Target `_ZN5MGame8OnUpdateEv`, 11440 B,
41 diff bytes at HEAD. Extends `mgame-onupdate-wave129-w7661.md`.

## 1. Tested this wave

Wave 129's open question #2 was "a source construct in the body that adds a
`Globals::status` use inside `bb.242` without changing the emitted body bytes".
Three placements of a discarded use were tried at the mission-23 idle block:

```cpp
(void) Globals::status;      // before pauseOpen = 1 / between / after pauseSounds()
```

All three are optimised away (`Globals::status` is a non-volatile global pointer and
the load is dead), and the emitted code is unchanged: the condition still uses the
`Status *st = Globals::status` anchor (`ldr.w r5,[r9]; mov r0,r5`) and the same 41-byte
residual. Full-link `--show` is byte-identical to the pristine source.

## 2. Conclusion

An IR-only body use cannot be introduced through dead C++ (the optimiser removes it).
The remaining levers are still the two from wave 129: the cross-TU
`src/game/core/Globals.h` `&Globals::status` allocation hint, or a body construct that
is *not* dead but still emits the same bytes (none found). Committed source unchanged.

Gate before/after: byte 4506 -> 4506, linked 4506 -> 4506, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.
