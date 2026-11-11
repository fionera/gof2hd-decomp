# LevelScript::process — iOS-structure pass (wave 183, w12704)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12704`, branch
`wave183-w12704`, base master `9555474d8` (wave-182 harvest 5, raw **52,412**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12704`.

Continuation of wave-182 w12608 (iOS structural oracle).  w12608's note was
not landed; this pass re-derives the iOS comparison from the binary.

## Locating `LevelScript::process` in the iOS 1.1.4 Mach-O

`_work/bins/ios_1.1.4_HD` is stripped to 1,528 external symbols, so the
function is not named.  It is found by its literal pool: the file contains
`0.2f` / `2.2f` / `0.98f` adjacent at file offset `0x133830` (`vmaddr
0x134830`).  That is inside the Thumb function at `0x134098`, whose size from
`LC_FUNCTION_STARTS` is **93,948 B** (`0x16f14`).  (Note: `LC_FUNCTION_STARTS`
entries have bit 0 set for Thumb functions, so the real start is
`entry & ~1`.)  The function's prologue and first calls match the Android
function exactly:

```
1340ce ldr r0,[r11,#0x14]   ; m_pLevel (iOS layout: m_pCamera is the 2nd arg)
1340d2 bl  getMessages
1340d8 ldr r0,[r11,#0x14] ; getPlayer
1340e2 ldr r0,[r11,#0x14] ; getEnemies
1340ec ldr Globals::status
1340fe bl  getCurrentCampaignMission
134152 cmp r4,#0            ; mission == 0
134154 bne 0x1353d6         ; else mission == 1
```

The iOS 1.1.4 signature is `LevelScript::process(int, TargetFollowCamera*)`
(the source dump lists only ctor/dtor/`resetCamera(TargetFollowCamera*)`/
`process(int, TargetFollowCamera*)` in `LevelScript.cpp`).  The Android
2.0.16 signature is `process(int)` with the camera in `m_pCamera`; the
Android layout has `m_pLevel` at `0x18`, `m_nState` at `0x1c` (iOS: `0x14`
and `0x18`).  The comparison below uses the iOS function only as a structural
oracle; all byte work is against the Android binary.

## State-machine structure (iOS vs Android vs ours)

The mission-0 intro is structurally identical in iOS 1.1.4 and Android
2.0.16:

* mission-0 entry: `messages[2]->isOver() && m_nState == 0` -> state-0 body
  (`player->setPosition(18000,-12000,-40000)`, camera
  `(-12000,2000,-500)`, `MatrixSetTranslation`, three ships at
  `(-10000,500,0)`, `(-10000,-300,-1700)`, `(-10000,-200,2000)`, loop
  `setDirection({1,0,0},{0,1,0})` + `setVisible(true)`,
  `lodManager->forceUpdate(30,false)`).
* `else if (m_nState <= 2)`: wobble loop
  (`layout->getPulseValue(0.0005f) - 0.5f`, three `translate(0,wobble,0)`),
  then `m_nState==1` (message 6 -> target/state 2/`setPosition(-5000,300,-5000)`)
  and `m_nState==2` (sound 142, `translate(delta*0.2,0,delta*2.2)`, message 7
  -> state 3 + three-ship exhaust/visible/awake/`field_0x128=50000`/
  `enemyFlagsLo=1`).
* `else if (m_nState == 4)`, `else if (m_nState == 3)`,
  `else if (m_nState == 5)`, then `switch (m_nState)` covering **6..15**,
  then `if (m_nState == 16)`.

The Android disassembly confirms the compare order **4, 3, 5** (`0x1389c8
cmp #4 / beq 0x138d2a`, `0x1389ce cmp #3 / bne 0x138fd8`, `0x138fd8 cmp #5`)
and the switch range **6..15** (`0x139b1a subs r0,r1,#6; cmp #9; tbh`).
The iOS function has the same order (`0x134500 cmp #4`, `0x134504 cmp #3`,
`0x134606 cmp #5`, `0x134850 subs r1,#6; cmp #9; tbh`) and the same state-16
test (`0x135316 cmp r1,#0x10`).  Both also use the same constants
(18000/-12000/-40000, -12000/2000/-500, -10000/+500/0, -10000/-300/-1700,
-10000/-200/2000, -5000/300/-5000, 0.2/2.2, 25000/-200/-1000, pi/2, 0.98,
0.3, 0.2, sound events 142/143/157/158/161, message indices 2/6/7/8/10/12/13/14).

No per-state helper functions exist in either build: the iOS source dump and
the Android dynsym both contain only `process`, `resetCamera`, and the small
sequence helpers.  No state was collapsed or split.

## Landed win

Reordering the case-11 tail (`0x13bd2c`, mission 0 state 11).  Note: the
original disassembly actually emits `setTarget` *before* `getPosition`
(`0x13be16` then `0x13be2c`), so this is a codegen-shape win, not a
source-order match:

```cpp
// before
m_pCamera->setTarget((*enemies)[enemies->count - 10]->geometry);
m_tempVec = (*enemies)[enemies->count - 10]->geometry->getPosition();
m_tempVec.x += 2000.0f;   // @0x13bdd0
// after
m_tempVec = (*enemies)[enemies->count - 10]->geometry->getPosition();
m_tempVec.x += 2000.0f;   // @0x13bdd0
m_pCamera->setTarget((*enemies)[enemies->count - 10]->geometry);
```

Measured: raw **52,412 -> 52,408** (-4 B), aligned 22,050 -> 22,063,
size 57,232/57,232 exact, insn 20,728/20,830.  `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04` (byte-identical).  `.text` 75,015 ->
75,011.  The move is semantically safe: `setTarget` reads the geometry
pointer from `enemies[count-10]`, while the moved-across statements only
touch `m_tempVec`.

## Rejected probes

* Full 4/3/5 source reorder of the mission-0 state chain (to match the
  original compare order): size 57,256 (+24) and raw 55,051; with the
  `messages[12]->isOver()` test moved inside the state-5 body: size
  57,220 (-12), raw 55,268.  The baseline compiler layout is already closer
  in raw bytes.
* Case-11 `m_pCamera->setPosition(m_tempVec)` moved before `forceUpdate`
  (the original's order): raw 52,413 (+1), size exact.
* Mission-1 prologue reorder (`forceUpdate` first, then rotate,
  `getPosition`, `setToSleep`): raw 52,416 (+4); individual halves +3/+1.
* State-3 `setTarget`/`setLookAtCam` swap: raw 52,414 (+2).
* State-3 `m_bStartSequence=0` / `field_0x12=1` swap: neutral (52,412).
* State-4 statement reorder to the original order (`resetCamera` after
  `setTurretMode`, `setComputerControlled` before `removeAllGuns`,
  `setRotation` after `setTarget`): raw 52,425 (+13); each single move
  +1..+15.

## Continuation: statement-order sweep (same pass)

A systematic "move one simple statement down past 1-3 statements at the same
brace depth" sweep over the whole `process()` body (319 candidates, 4-way
parallel, size + `.rel.plt` checked) found 11 independent single-statement
moves that each reduce raw bytes with size and `.rel.plt` unchanged.  Their
deltas are exactly additive, so they landed together:

| body lines (base) | move | raw |
|---|---|---:|
| 3316-3318 | `m_nScriptTimerA += delta;` after `player->rotate(...)` | -5 |
| 880-881 | `Status *st = Globals::status;` after `m_bStartSequence = 0;` | -2 |
| 1205-1208 | `m_pCamera->setTarget(player->geometry);` after `translate(10500,700,1000)` | -2 |
| 1862-1864 | `enemies[0]->setActive(false)` after `setPosition(0,0,-5000000)` | -2 |
| 3107-3110 | `m_pCamera->setLookAtCam(true)` after `m_bCinematicBreak = 1` | -2 |
| 137-139 | `m_nScriptTimerA = 0;` after `setComputerControlled(false)` | -1 |
| 566-568 | `camera10->translate(...)` after the timer/rumble init | -1 |
| 1197-1200 | `player->stopShooting(0);` after `setRotation(...)` | -1 |
| 1304-1306 | `m_bCinematicBreak = 0;` after `setComputerControlled(false)` | -1 |
| 1918-1921 | `field_0x12d = 0;` after `Vector camPos = ...getPosition()` | -1 |
| 3505-3507 | `enemies[i]->awake()` after `setEnemy(player->player)` | -1 |

All moves are semantically safe (the moved statement neither reads nor writes
anything the statements it crosses touch, and no callee can observe the
difference).  Measured: raw **52,408 -> 52,389** (-19 B), size
57,232/57,232 exact, `.rel.plt` md5 unchanged
`289b2a069f8504d1e8a3919e43915e04`, `.text` 75,011 -> 74,992.

The state-13 block (`0x144c38`) original instruction order is
spin-compute, timer-add, rotate, moveForward.  Our source has the timer add
first; moving it after `rotate` (not all the way past `moveForward`, which is
-3) is the best codegen shape found (-5).
