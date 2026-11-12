# LevelScript::process — wave 190 / w13455: escort-revive sink

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13455`, branch
`wave190-w13455`.  Base after the state-6 `facing` hoist: raw **51,699**,
`.text` 72,255.

A full `d=2` rotation sweep on the current body (2,978 candidates) found this
second safe cell (the other sub-51,699 cells are the documented read-after-write
rotations, the RNG-order swap, and the `.rel.plt`-fatal mission-0 state-9
family).

## Landed change

Source lines 4155-4157 (dead group-8 escort revive loop):

```cpp
-                        (*enemies)[i]->revive();          // vtable+0x18
                         Vector playerPos = player->getPosition();
                         (*enemies)[i]->setPosition(playerPos + Vector{(float) xOff, 5000.0f, 25000.0f}); // vtable+0x44
+                        (*enemies)[i]->revive();          // vtable+0x18
```

`KIPlayer::revive` is the empty 2-byte `bx lr` in the original
(`0xa635e`); the `PlayerFighter::revive` override resets hitpoints/state and
calls `setActive`/`setExhaustVisible`/`AEGeometry::setVisible`, but never
writes or reads the geometry position or the player transform.  `setPosition`
writes `geometry->position` and `player->transform`; `player->getPosition()` is
a pure read of a disjoint object.  The reorder is therefore
semantics-preserving.

Measured alone on the real build:

```
raw        51,699 -> 51,698  (-1)
aligned    21,925 -> 21,955
size       57,232 / 57,232   (exact)
.text      72,255 -> 72,254  (-1)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
