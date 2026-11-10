# Small tail wave 165 (w11117): MGame m122 volatile-status raw byte

Status: **1 `src/` change landed** (`MGame::OnUpdate`).  Worktree
`wt/w11117`, branch `wave165-w11117`, base master `3633505f8`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11117`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0).
`.text` differing bytes **83,177 -> 83,176** (-1), `.ARM.extab` 659 unchanged.

Baseline re-verified on entry: `Gun::calcCharacterCollision` 463/57/1984,
`SpaceLounge::drawLounge` 175/41/1948, `ModStation::ModStation` C1/C2
43/12/1008, `MenuTouchWindow::OnTouchBegin` 28/16/1896,
`MGame::OnUpdate` 16/6/11440.

## Landed: `MGame::OnUpdate` m122 body, `Status *volatile st`

The wave-163/164 shape keeps a `Level *level` local before the ccm call,
which pins the frame and the `&Globals::status` GOT base in `r9` (the rest
of the 11,440-byte function is byte-identical to the original), but loads
`this->level` before the ccm into `r5`; the original loads it after the
call into `r1`.  Moving the `Level *level` declaration after the ccm folds
the local into a direct `this->level` load and shrinks the function to
11,392 (a global RA cascade, gate-fatal).

The landed shape keeps the frame/GOT-base assignment with a `Status
*volatile st` local instead of the `Level*` anchor:

```cpp
Status *volatile st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = st->getCurrentCampaignMission() == 122 ? 3164 : 3166;
Level *level = this->level;
level->createRadioMessage(28, radioIndex + radioBase);
reinterpret_cast<long long &>(Globals::status->field_0x100) = Globals::status->getPlayingTime();
```

Measured: `raw 16 -> 15`, `aligned 6 -> 111`, `11,440 B` exact,
`.text` differing bytes `83,177 -> 83,176` (**-1**).  The volatile stack
slot keeps the frame at 11,440 but changes the m122 condition's register
allocation, which is why the aligned count rises; raw/`.text` (the
byte-identicality ground truth) both improve by one byte.

## Sweep (all measured, size-exact unless noted)

### MGame::OnUpdate (m122 body)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base (`Level *level` before ccm) | 16 | 6 | 11,440 |
| **LANDED: `Status *volatile st` + level before ccm** | **15** | **111** | **11,440** |
| `Status *volatile st` ccm-only, level after ccm | 15 | 111 | 11,440 |
| `Level *level` after ccm (`idxfirst`, `const`, `ref`, two locals) | 5,196 | 597 | 11,392 |
| `Level **levelp = &this->level` / `Level *&level` before or after | 5,196 | 597 | 11,392 |
| `void *` / `intptr_t` local after ccm | 5,196 | 597 | 11,392 |
| `Level *level; ... level = this->level;` (decl/assign) | 5,539 | 657 | 11,392 |
| `Status *st` ccm-only, store plain | 918 | 109 | 11,436 |
| `Status *st` full (ccm + tail store) | 5,254 | 758 | 11,488 |
| `Status *volatile st` full | 937 | 141 | 11,448 |
| `Status *volatile st` ccm-only, level before `radioBase` | 31 | 117 | 11,440 |
| `volatile Status *st` | build fail | | |

### ModStation::ModStation C1/C2

All size-exact at `raw 43 / aligned 12 / 1008 B`:
`base`; `!iPad ? Yaw : YawIPad`; `const float *farYawTable` before the
call (normal and `!`); `float farYaw` local (normal and `!`); explicit
`if (iPad) ... else ...`; `gHangarCamPitch[race]` in the farKey call;
`RotationOrder`/`0.0f` locals; nearKey `pitch`/`yaw` declaration swaps.
`yaw`/`yawTable` reuse is `604/71/984`; volatile far-yaw is
`696/59/1000`; swapping the nearKey `yawBits`/`pitchBits` order is
`752/40/1000`.  Residual stays the farKey yaw-table load-order /
`r3`/`r4` pool-materialisation tie.

### MenuTouchWindow::OnTouchBegin

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | 28 | 16 | 1,896 |
| `layout.buttonInsetX` (reference) in case 15 | 316 | 76 | 1,888 |
| `Layout *layout` pointer in case 15 | 316 | 76 | 1,888 |
| drop the shadowing `Layout &layout` | 362 | 76 | 1,892 |
| local `int inset` for `buttonInsetX` | 28 | 16 | 1,896 |

Residual is the case-15 `buttonInsetX` `ip` vs `r4` tie and the case-8
`field_0xc`/`field_0x20` `r5`/`r6` vs `r4`/`r5` tie.

### SpaceLounge::drawLounge

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | 175 | 41 | 1,948 |
| `- (width + pad + pad)` | 176 | 42 | 1,948 |
| `- (pad + pad + width)` / `- (pad * 2 + width)` / `- (width + (pad + pad))` | 445 | 54 | 1,948 |
| `+ (-(pad + pad) - width)` | 445 | 54 | 1,948 |
| `- width - pad - pad` | 468 | 64 | 1,944 |
| condition `x + width + pad * 2` | 490 | 72 | 1,944 |

Base remains best; the clamp is a `r5`/`r9` allocation tie.

### Gun::calcCharacterCollision

All size-exact at `raw 463 / aligned 57 / 1984 B` unless noted:
`base`; `Vector delta` temp; `PlayerEgo *player` local; `i` as `int`,
`int` post-inc, `unsigned int`; `VectorLength(playerPosition -
enemyPosition)` (465/59); `VectorLength` with the getPosition inlined
(467/67).  Hoisting `playerPosition` before the branch is
`1785/292/1940`; declaring it uninitialised before the branch is
`1567/292/1964`.  Residual is the `i` `r4` vs `r5` and
`playerPosition`/subtraction-temp `sp+44`/`sp+104` slot swap.

## Files changed

* `src/game/menu/MGame.cpp` (m122 body: `Status *volatile st` anchor).
* `_work/reconstructions/small-tail-wave165-w11117.md` (this note).
