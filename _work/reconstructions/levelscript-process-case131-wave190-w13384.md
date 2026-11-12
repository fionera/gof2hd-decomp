# LevelScript::process — wave 190 / w13384: case-131 state-2 up-vector hoist

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13384`, branch
`wave190-w13384`.  Base before this pass: raw **51,766**, aligned 22,433,
`.text` 73,263, `.ARM.extab` 603.

## Win: `GetUpVector()` hoisted ahead of the direction/right creep terms

In the case-131 state-2 cinematic camera creep the source had

```cpp
*scratch = player->GetDirVector();
*tempVec += *scratch * ((float) delta * 2.01f);
*scratch = player->geometry->getRightVector();
*tempVec += *scratch * ((float) delta * 0.03f);
AbyssEngine::AEMath::MatrixSetTranslation(local, *tempVec);
*scratch = player->GetUpVector();
*tempVec += *scratch * ((float) delta * 0.01f);
```

The original (`0x13c938`-`0x13c9ee`) emits the three creep terms as
`dir*2.01`, `right*0.03`, `up*0.01` and only then `setFixed(true)` /
`MatrixSetTranslation`.  Hoisting the `GetUpVector()` assignment to the top
of the block lets the scheduler emit that order and is a measured win:

* raw **51,766 -> 51,764** (-2), aligned **22,433 -> 21,825** (-608),
* `.text` 73,263 -> 73,261 (-2), `.ARM.extab` 603 unchanged,
  build-id 20, total 73,886 -> 73,884,
* size 57,232/57,232 exact, `.rel.plt` md5
  `289b2a069f8504d1e8a3919e43915e04` unchanged, `.ARM.exidx` 0.

The large aligned drop (608 instructions) shows the emitted block now tracks
the original's `getDirection`/`getRightVector`/`getUpVector` call order; the
remaining raw delta is the global literal-pool shift.
