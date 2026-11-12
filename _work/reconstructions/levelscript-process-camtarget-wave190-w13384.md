# LevelScript::process — wave 190 / w13384: camera setTarget/setPosition order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13384`, branch
`wave190-w13384`.  Base before this pass: raw **51,770**, `.text` 73,267,
`.ARM.extab` 603.

## Win: `wcamera->setTarget` moved after `setPosition`

In the case-16-style camera hand-off block the source had

```cpp
wcamera->setLookAtCam(true);
wcamera->setTarget((*enemies)[0]->geometry);
m_nScriptTimerA = 0;
wcamera->setPosition((*enemies)[0]->getPosition() + Vector{9000.0f, -7000.0f, 40000.0f});
```

`setTarget` and `setPosition` are independent camera setters (different
`TargetFollowCamera` members), so moving `setTarget` after the
`setPosition(...)` call is semantics-preserving.  Measured win:

* raw **51,770 -> 51,767** (-3), aligned 22,424 -> 22,431,
* `.text` 73,267 -> 73,264 (-3), `.ARM.extab` 603 unchanged,
  build-id 20, total 73,890 -> 73,887,
* size 57,232/57,232 exact, `.rel.plt` md5
  `289b2a069f8504d1e8a3919e43915e04` unchanged, `.ARM.exidx` 0.

## Full rejected-pair d2 sweep

`sweeprej2.py` measured all 1,818 distance-2 statement moves that the
access-chain model rejects (same-receiver calls / shared locals).  Besides
the win above, the only other `-1..-4` cells were read-after-write swaps
(`*acc += *tmp * 1.96f` vs `*acc += *tmp * 0.03f` across the
`*tmp = getRightVector()` reassignment, and
`m_nScriptTimerA += delta; approach = timer / -34000 + 1`) and were not
landed.
