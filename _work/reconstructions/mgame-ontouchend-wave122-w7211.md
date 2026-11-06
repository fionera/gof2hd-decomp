# MGame::OnTouchEnd — wave 122 (w7211): `== true` removal, size-shift blocker

Status: research note (agent killed before commit; patch measured by the orchestrator).
Worktree `wt/w7211`, base `29cd275f6`. Target `_ZN5MGame10OnTouchEndEiiPv`
(`src/game/menu/MGame.cpp`), 8080 B.

## Finding

Two remaining `== true` spellings in `OnTouchEnd` are not faithful:

```diff
-                    if (this->player->goingToWaypoint() == true) {
+                    if (this->player->goingToWaypoint()) {
...
-                } else if (Globals::status->getMission()->hasWon() == true ||
+                } else if (Globals::status->getMission()->hasWon() ||
```

Measured: `OnTouchEnd` **81.7 -> 82.3 %**, but the body shrinks and every
following `.text` symbol shifts, so `byte_exact` collapses **4506 -> 3106**
(`linked_exact` stays 4506). The patch is not landable without a 4-byte size
compensation in `MGame.cpp` (or a shape that keeps the 8080 B body).

The agent's sweep log also recorded `W saves 12 bytes! 8096 -> 8084, score
80.7`, i.e. several `== true` / temporary-removal variants trade score for
size. The next pass should pair the two removals above with a size-neutral
anchor in the same TU.
