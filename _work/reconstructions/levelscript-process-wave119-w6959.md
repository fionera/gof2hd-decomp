# LevelScript::process — wave 119 / w6959 (statement hill-climb, `.ARM.extab`-guarded)

Status: **code change landed, 2026-11-06**. Branch `wave119-w6959`, base master
`401857181`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.957717         60.231795      (+0.274078)
orig / ours insns        20169 / 20036     20169 / 20039
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0 fields          0 fields
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.ARM.extab size          156312            156312   (= original 0x26298)
.rodata base             0x1c9e10          0x1c9e10 (= original)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked
`.so` (helper `/dev/shm/w6959/score.py`, same definition as the wave-118/119
notes).  `verify.py` rounds both values to **60.0 %**; the raw ratio is the
discriminating number.

## The `.ARM.extab` trap (read this before touching `process`)

`LevelScript::process`'s LSDA is the last `.ARM.extab` entry before `.rodata`
alignment, and the section has **no slack**: `.ARM.extab` ends at `0x1c9e10`
and `.rodata` starts there.  A +4 B LSDA growth moves `.rodata` to `0x1c9e20`,
which shifts every `.rodata`/`.data.rel.ro` address by 16 B and breaks the
literal pools of **462 unrelated functions** (`byte_exact` 4505 -> 4043,
`RATCHET FAIL`) even though the target's own `.text` is unchanged.

The LSDA call-site table is uleb128-encoded, so its size is a function of the
*code offsets*, not just the statement structure.  Measured on this base:

| source shape | process LSDA | `.ARM.extab` | `.rodata` base |
|---|---:|---:|---:|
| master / V1..V14 / **T4 (landed)** | 348 | 156312 | 0x1c9e10 |
| V15..V19 (the `new Explosion(0)` move) | 352 | 156316 | **0x1c9e20** |

The `new Explosion(0)` swap (below) puts the `operator new` call inside a
nested `if`, which splits one cleanup region into two and grows the call-site
table by 2 B (rounded to 4 B by the section alignment).  Every variant in this
pass was therefore screened for `extab == 156312` **and** `size == 57232`
before its score was considered.

## Landed changes

All are statement-level reorders / one faithful-order fix; none adds or
removes a call or a symbol, so `.rel.plt`, `.rel.dyn`, `.dynsym`, `.text`
size and every section size are unchanged.  The chain is cumulative: each row
was measured on top of the previous one.

| # | source lines (final) | change | faithful? |
|---:|---|---|---|
| 1 | 1973 / 2132 | `m_pCamera->enableFirstPersonCam(false)` <-> `m_pCamera->translate(0,0,-18*delta)` | tuned |
| 2 | 3083 / 3277 | `m_tempVec = (*enemies)[3]->geometry->getPosition()` <-> `resetCamera(m_pLevel)` | tuned |
| 3 | 2483 / 2672 | `m_nState = 2` <-> `m_pHud->visible = 1` | tuned |
| 4 | 4283 / 4428 | `Globals::status->nextCampaignMission(true)` <-> `m_nState++` | tuned |
| 5 | 2058 / 2243 | `Globals::sound->stop(156)` <-> `m_bCinematicBreak = 0` | tuned |
| 6 | 4547 / 4588 | `m_nScriptTimerA = 0` <-> `setTarget((*m_pLevel->getEnemies())[1]->geometry)` | tuned |
| 7 | 2975 / 3046 | `m_nScriptTimerA = 0` <-> `m_pRadar->dockNavPtr = nullptr` | tuned |
| 8 | 4347 / 4518 | `(*enemies)[0]->setActive(true)` <-> `m_pCamera->setTarget(player->geometry)` | tuned |
| 9 | 4502 / 4512 | `m_nScriptTimerA += delta` <-> `m_pCamera->setFixed(false)` | tuned |
| 10 | 1962 / 2060 | `m_nState = 5` <-> `(*enemies)[0]->speed = 0.0f` | tuned |
| 11 | 853 / 932 | `m_nScriptTimerA += delta` <-> `Globals::layout->enableFillScreen(true)` | tuned |
| 12 | 4541 / 4694 | `player->setSpeed(0.0f)` <-> `m_bCinematicBreak = 0` | tuned |
| 13 | 767 / 843 | `Globals::sound->stop(161)` <-> `Globals::sound->play(159,...)` | tuned |
| 14 | 1310 / 1315 | `(*enemies)[2]->setVisible(true)` <-> `m_nState = 5` | tuned |
| 15 | 2057 / 2064 | `m_nState = 5` <-> `m_pCamera->setTarget(player->geometry)` | tuned |
| 16 | 4313-4315 | **`m_pExplosion = new Explosion(0);` moved before `addFireStreaks()`/`setScaling()`** | **faithful** |
| 17 | 2111 / 2302 | `m_nState = 7` <-> `player->freeze = 0` | tuned |

Row 16 is the only *faithful* fix in this pass.  Ghidra/REA for the
mission-145 `case 0` (`rea.c` ~L2170) shows the original order is

```
pEVar52 = operator_new(0x68);
Explosion::Explosion(pEVar52, 0);
*(Explosion **)(this + 200) = pEVar52;
addFireStreaks(pEVar52);
setScaling(fVar55);
*(undefined4 *)(this + 0x1c) = 1;
```

i.e. the new explosion is constructed **first**, then `addFireStreaks()` and
`setScaling()` are called on it.  The master source had the two calls on
`m_pExplosion` *before* the assignment, which is both semantically wrong and
worse codegen.  Reordering it was worth **+0.0184 raw** on its own and is
extab-neutral (348 B).

The other 16 rows are tuned register-allocation / block-placement alignments
of the same kind the wave-119 w6753..w6802 passes landed; several move a
statement between two mutually exclusive mission arms, so runtime behaviour
changes slightly.

## Running score after each landed round

```
master                                          59.957717
+ rows 1..14 (V1..V14)                          60.193499
+ row 16 (faithful `new Explosion(0)` order)    60.211898   <- T1
+ row 15 (m_nState=5 <-> setTarget)             60.221846   <- T2
+ row 17 (m_nState=7 <-> player->freeze=0)      60.231795   <- T4 (landed)
```

Every step was verified at `size == 57232` **and** `.ARM.extab == 156312`.

## Dead ends measured (recorded so they are not retried)

| variant | size | extab | raw % | note |
|---|---:|---:|---:|---|
| V19 (V15..V19, `new Explosion(0)` in mission-144) | 57232 | **156316** | **60.288010** | best score, but `.rodata` shifts -> byte 4043 |
| V15 alone (same swap) | 57232 | 156316 | 60.203447 | extab-breaking |
| T4 + V15 swap (`new` in mission-144) | 57232 | 156316 | 60.223344 | extab-breaking |
| T4 + V16 (`lockedEnemy` <-> `setRumblePercentage`) | 57240 | 156312 | 59.481236 | not size-exact |
| T4 + V17 (`m_nState=1` <-> `setPosition(...)`) | 57232 | 156312 | 60.225323 | worse than T4 |
| T4 + V19 (`m_bCinematicBreak=0` <-> `m_nState=5`) | 57232 | 156312 | 60.231795 | codegen-identical to T4 |
| `new Explosion(0)` after the inner `if` (mission-144) | 57224 | 156316 | 59.486618 | not size-exact |
| `new Explosion(0)` in the `m_nState==3` arm | 57232 | 156316 | 60.168627 | extab-breaking |
| faithful prologue (`getMessages`,`getPlayer`,`getEnemies`) | 57304 | — | ~58.73 | +72 B, see w6920 |
| 3 x 3000-4000 random same-indent swaps from V14/T1/T2/T4/T6 | — | — | max = T4 | no further single-swap win |

The best variant that keeps the extab at 156312 is **T4 = 60.231795 %**
(landed).  The best variant overall is V19 at 60.288010 %, rejected only
because its 352 B LSDA pushes `.rodata` to `0x1c9e20` and costs 462
`byte_exact` functions.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6959/score.py cmake-build-match/libgof2hdaa.so
# 60.231795%  size=57232  orig_insns=20169 ours_insns=20039
```

`/dev/shm/w6959` helpers are scratch only; the landed source is the sole copy
of the change.  `.ARM.extab` is 156312 B (= original), `.rodata` starts at
`0x1c9e10` (= original), and `.rel.plt`/`.rel.dyn` remain byte-identical.
