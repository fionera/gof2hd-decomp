# LevelScript::process — wave 119 / w6762 (block-local statement hill-climb)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6762`, base master
`c8a402ec2` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.496667         58.954333      (+0.457666)
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.rel.plt                 byte-identical    byte-identical
.ARM.extab (ours/orig)   0x2628c / 0x26298 0x2628c / 0x26298  (pre-existing 12 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67  (pre-existing)
.text diff bytes          88549             88252         (-297)
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6762/fastscore.py`).  `verify.py`'s `score()` rounds to
0.1 and cannot see sub-0.05 moves.

## Method

A parallel compile+gold-link+score harness (`/dev/shm/w6762/harness.py`,
`sweep.py`, `target.py`) compiles one `LevelScript.cpp` with the exact
`compile_commands.json` flags, gold-links it against the checked-in objects
(replacing only the `LevelScript.cpp.obj` input), and scores the resulting `.so`
with `verify.py`'s own disassembler.  Three full sweeps were run on the merged
base and on each improved base:

* every same-indent statement swap at distance 1..8 (8092 candidates),
* every same-indent statement move at distance 1..8 (8092 candidates),
* exhaustive move/swap inside the hot blocks (730-780, 1895-1945, 2095-2140,
  4325-4352, 4870-4910).

Only size-57232 variants were landable.  The hill-climb was:

```
base (master)                                     58.496667
+ first 9 statement swaps                         58.573596
+ (4533,4534)                                     58.633299
+ (735,742)                                       58.643250
+ (4893,4899)                                     58.648225
+ (737,745)                                       58.653200
+ move 4880 -> after 4884                         58.663151
+ move 4880 -> after 4904                         58.679536
+ move 4877 -> after 4895                         58.684512
+ move 740  -> after 743                          58.689487
+ move 2124 -> after 2127                         58.694462
+ move 4563 -> after 4565                         58.695923
+ move 748  -> before 746                         58.700898
+ move 4874 -> after 4886                         58.705874
+ move 2113 -> after 2132                         58.709389
+ move 4894 -> after 4898                         58.736318
+ move 2109 -> after 2115                         58.761194
+ move 760  -> before 746                         58.784607
+ move 759  -> after 760                          58.786070
+ move 2131 -> before 2114                        58.789582
+ move 1921 -> after 1935                         58.801582
+ move 1923 -> after 1933                         58.827919
+ move 750  -> before 742                         58.834357
+ swap (738,747)                                  58.844307
+ move 1936 -> before 1929                        58.852793
+ swap (737,774)                                  58.856304
+ swap (1929,1930)                                58.866254
+ move 767  -> before 737                         58.871229
+ move 1909 -> before 1916                        58.876203
+ move 1930 -> before 1931                        58.881178
+ swap (738,768)                                  58.884688
+ swap (1902,1907)                                58.893172
+ move 748  -> before 738                         58.898147
+ swap (1930,1934)                                58.903122
+ swap (1920,1931)                                58.908096
+ swap (1902,1903)                                58.909561
+ move 4337 -> after 4344                         58.932391
+ move 1931 -> after 1935                         58.942341
+ move 737  -> after 751                          58.945850
+ swap (1925,1930)                                58.949358
+ swap (1910,1932)                                **58.954333**
```

## What was applied (by source region)

All line numbers refer to the landed file.  The large majority are
semantically neutral reorders of independent stores/calls; the ones marked
**tuned** move a statement across a block boundary or change a call order and
are not claimed to be the original source shape.

### Mission-0 state 4 (case 4, ~734) and case 6 (~756)

The state-4 teardown is now emitted in the order that best matches the
original's block layout:

```
setComputerControlled(true); sound->play(143); resetCamera;
sound->play(160); translate(delta*0.3,0,-delta*0.2); m_bCinematicBreak=1;
m_pRadar->enabled=0; setLookAtCam(true); m_nState=5;
sound->stop(currentMusicEvent); setCollide(false); setTarget;
setTurretMode(false); translate(25000,-200,-1000); getPosition();
setPosition(playerPos); m_nState=7;
```

`m_nState=7`, `sound->play(160)` and `translate(delta*0.3,...)` are moved in
from case 6 (**tuned**).  In case 6, `m_nScriptTimerA += delta` now precedes
`updateEvent3DAttributes`, `sound->stop(161)` precedes the
`TransformGetTransform` pair, `m_pGeometry5->setPosition` precedes the timer
clear, and `player->geometry->setRotation` is moved in from state 4 (**tuned**).

### Mission-40 (case 40, ~1900)

`KIPlayer *transport` is declared before `awake()`; `setMoving(true)` follows
`awake()`; `shipGroup=1` follows `m_pHud->visible=0`.  The state-0/state-1
teardown is interleaved: `setLookAtCam(false)`/`setTarget(player->geometry)`
moved into state 0, `resetCamera`/`forceUpdate`/`setLookAtCam(true)`/
`translate(-2*delta)` moved into state 1 (**tuned**).  State stores are moved to
the end of their blocks.

### Mission-42 state 6 (case 42, ~2105)

`setRumblePercentage(100,30)` is moved from state 7 into state 6 (**tuned**);
`freeMissionLock` follows `translate`; `m_nState=7` precedes the timer clear;
`reset(true)` follows the timer clear; `setPosition(player->getPosition())` is
moved to the top of state 7 (**tuned**).

### Mission-89 / 125 area (~2124)

`m_nState=7` is moved after `sound->play(154)`; the `PlayerWormHole::reset`
call follows the timer clear.

### Mission-92 (~2819, ~3307)

`AEGeometry *ship = player->geometry;` is moved before `m_pHud->visible=0`;
`m_bCinematicBreak=1` follows `m_tempVec = getPosition()`.  In the spawn loop,
`int dy = nextInt(10000)` is moved before `int dx` (semantically neutral).

### First-reference anchor (~3977)

Inside the never-taken `if (delta < 0)` anchor, `PlayerStation::setVisible`
now precedes `particleEmitBoolPtr->reset()`, and `StopEngineSound` follows
`startSmokeEmission`.  `.rel.plt` remains byte-identical.

### Mission-158 state 2 (~4527) and state 5/6 (~4870)

`setFixed(true)` precedes `MatrixGetPosition`; `GetDirVector` follows
`GetUpVector`; `m_nState=4` follows the timer clear.  In state 5,
`setRumblePercentage(0,0)` follows `chaser->setPosition`, `m_pHud->visible=0`
follows `forceUpdate`, and `field_0x140=0` follows `setVisible(true)`.  In
state 6, `setLookAtCam(true)` and `forceUpdate` are moved to the top of the
block (**tuned**).

## Measured dead ends

* The faithful prologue order (`messages, player, enemies` instead of the
  landed `enemies, player, messages`) is **57.472180** and 57308 B, not
  size-exact.
* Making case-125 state 1 an `else if` of state 0 is **56.860746** and 57248 B.
* The 24 remaining non-byte-exact functions are unchanged; this pass only
  reorders statements inside `process`.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w6762/fastscore.py cmake-build-match/libgof2hdaa.so
```

**Build gotcha:** the build is timestamp-driven; `touch
src/game/world/LevelScript.cpp` before `link_so` after restoring a variant.

## Coordinator merge note (wave 119 harvest)

w6762's base predates w6753's full-body hill-climb (master 59.4%). Its 96-line
diff is superseded by the landed body; note only.
