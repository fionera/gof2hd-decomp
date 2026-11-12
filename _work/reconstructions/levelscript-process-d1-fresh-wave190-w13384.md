# LevelScript::process — wave 190 / w13384: fresh d1 sweep on the 51,775 body

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13384`, branch
`wave190-w13384`.  Base after applying the seven d2 rotations: raw **51,775**,
`.text` 73,272, `.rel.plt` md5 `289b2a06…`.

A fresh full distance-1 adjacent-statement sweep (1,530 unique independent
pairs, exact NDK r18b `-Oz` compile + gold link, 4 workers,
`scratch/w13384/h/sweep.py 1 1`, results `res_d1b.jsonl`) on the post-d2 body
found one new semantics-preserving, `.rel.plt`-clean `-1` cell.

## Landed change

Source lines 3881/3882 (mission-89 marker block):

```cpp
-                            Vector zero{0.0f, 0.0f, 0.0f};
                             marker->setPosition(player->geometry->getPosition());
+                            Vector zero{0.0f, 0.0f, 0.0f};
```

`zero` is a local initialiser consumed later by
`Globals::sound->updateEvent3DAttributes(14, &camPos, &zero, false)`; moving
its declaration past the independent `marker->setPosition` call is
semantics-preserving.  Measured alone: raw **51,775 -> 51,774** (-1), size
57,232/57,232 exact, `.text` 73,272 -> 73,271, `.rel.plt` md5 unchanged.

## Rejected cells (same sweep)

* down 828,829 (`switchSkyboxForIntro` after `particleRenderBoolPtr->reset()`)
  is `-1` but moves `.rel.plt` (md5 `d997d11f…`).
* down 889,890 and down 4541,4542 are `m_tempVec += …` moved past a
  `setPosition(m_tempVec)` reader — read-after-write, semantic.
* down 3734,3735 is `*tmp = getUpVector()` / `*camPos += *tmp * 400` — RAW.

## Measured result

```
                         base (HEAD)      this pass
process raw diffbytes    51,775           51,774     (-1)
process aligned insns    22,413           22,414
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          73,272           73,271     (-1)
.ARM.extab diffbytes     603              603        (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
file size                2,187,328        2,187,328  (exact)
```

## Current-master harvest (wave 190)

Applied on master `21242c17f`: raw **51,744 -> 51,743** (-1), aligned
22,482 -> 22,483 (+1), size exact 57,232 B. `.text` **73,000 -> 72,999**
(-1, combined with the w13398 Radar aligned win); `.ARM.extab` 603 and
build-id 20 unchanged. `check` -> RATCHET PASS.
