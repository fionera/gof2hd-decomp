# LevelScript::process — wave 190 / w13480: d=2 up-rotation, timer + pirate hoists

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13480`, branch
`wave190-w13480`, base after the d=2 fighter-wake rotation: raw **51,688**,
`.text` 71,858.

## Landed changes (two independent `up` rotations, compose to -2)

Source lines 2955-2958 (mission-158-ish enemy park block):

```cpp
                         Vector ref = (*enemies)[3]->getPosition();
+                        m_nScriptTimerA = 0;
                         (*enemies)[1]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 7500.0f});
                         (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
-                        m_nScriptTimerA = 0;
                         (*enemies)[2]->setPosition(ref + Vector{-79000.0f, 4900.0f, 2500.0f});
```

`m_nScriptTimerA = 0` is a plain store to a `this` field; the three
`setPosition` calls write enemy geometry/transforms and read only their own
arguments.  Independent, semantics-preserving.

Source lines 4000-4002 (pirate spawn loop):

```cpp
                         for (int i = 2; i < 6; i++) {
+                            KIPlayer *pirate = (*enemies)[i];
                             Vector playerPos = player->getPosition();
                             (*enemies)[i]->awake();
-                            KIPlayer *pirate = (*enemies)[i];
```

Both statements are pure reads (`(*enemies)[i]` array load and
`player->getPosition()`); the RNG call order (`sideX`/`offsetY`/`offsetZ`) is
unchanged.  Semantics-preserving.

Measured together on the real build:

```
raw        51,688 -> 51,686  (-2)
aligned    21,994 -> 21,994
size       57,232 / 57,232   (exact)
.text      71,858 -> 71,856  (-2)
.ARM.extab 575 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.

## Sweep notes

Full `d=2` up-rotation sweep on the 51,688 body (1,408 candidates, exact NDK
r18b `-Oz` compile + gold link, 4 workers).  Cells below the base:

| move | source lines | raw | verdict |
|---|---|---:|---|
| `up 828,830` | 828-830 | 51,687 | `.rel.plt` `75ad6587…` (fatal) |
| `up 889,891` | 889-891 | 51,687 | `m_tempVec` read-after-write |
| `up 1728,1730` | 1728-1730 | 51,686 | `setPosition` before the `camPos +=` offset (wrong) |
| `up 2411,2413` | 2411-2413 | 51,687 | swaps the `dy`/`dz` RNG call order (wrong) |
| `up 2955,2957` | 2955-2957 | 51,687 | landed |
| `up 4000,4002` | 4000-4002 | 51,687 | landed |
| `up 4542,4544` | 4542-4544 | 51,687 | `m_tempVec` read-after-write |
| `up 4604,4606` | 4604-4606 | 51,687 | `MatrixSetTranslation` before the second `m_tempVec +=` (wrong) |
