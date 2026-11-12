# LevelScript::process — wave 191b / w13517: `&&` operand-order lens

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13517`, branch
`wave191b-w13517`, base `929eb76f8` (raw **51,611**, `.text` 71,388,
`.ARM.extab` 577).

## Lens

New angle not covered by the statement-order sweeps: swap the operands of
every top-level `&&`/`||` in an `if`/`while` condition, and flip every
top-level relational comparison (`a < b` -> `b > a`, etc.).  210 candidates
(exact NDK r18b `-Oz` compile + gold link).  Most comparison flips are
codegen-neutral (identical bytes); one `&&` swap is a size-exact,
`.rel.plt`-identical raw win.

## Landed change (line 1537, mission-50 wormhole-exit branch)

```cpp
-    } else if (messages != nullptr && Globals::status->getCurrentCampaignMission() == 50) {
+    } else if (Globals::status->getCurrentCampaignMission() == 50 && messages != nullptr) {
```

Both operands are pure (a local pointer null test and a getter), so the
short-circuit reorder is behaviour-neutral.  The original's mission-50 branch
tests the message array first (comment above), so this is a codegen-coincidence
win, not a faithfulness fix.

Rejected: `player->getRoute() != 0 && getWaypoint(0)->state` ->
`getWaypoint(0)->state && getRoute() != 0` (raw -1) because it dereferences
the route before the null test.

## Measured result

```
                         base (929eb76f8)   this pass
process raw diffbytes    51,611             51,609     (-2)
orig / ours size         57232 / 57232      57232 / 57232  (exact)
.text diffbytes          71,388             71,386     (-2)
.ARM.extab diffbytes     577                577        (unchanged)
.ARM.exidx diffbytes     0                  0          (byte-identical)
.rodata diffbytes        485                485        (unchanged)
.rel.plt                 md5 289b2a06…      md5 289b2a06…  (byte-identical)
file size                2,187,328          2,187,328  (exact)
RATCHET                  PASS               PASS
```

Cumulative for w13517: raw **51,639 -> 51,609 (-30)**, `.text`
71,416 -> 71,386 (-30), `.ARM.extab` 577 unchanged, `.rel.plt` identical.
