# LevelScript::process — iOS-structure pass 2 (wave 184, w12804)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12804`, branch
`wave184-w12804`, base master `4c06c2662` (wave-183 harvest 6, raw **52,281**,
`.text` 74,842).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12804`.

Continuation of wave-183 w12704 (iOS structural oracle) and w12608.  This pass
re-derives the iOS comparison, checks the source-layout dumps for per-state
helpers, and lands a measured statement-order win.

## iOS 1.1.4 oracle: per-state structure, enum values, split/collapsed states

`_work/bins/ios_1.1.4_HD` is a stripped Mach-O armv7; `llvm-objdump
--triple=thumbv7` disassembles it (GNU objdump cannot read the container).
`LevelScript::process` is the Thumb function at `0x134098`, size **93,948 B**
(`0x16f14`) from `LC_FUNCTION_STARTS` (entry bit 0 = Thumb).

The iOS source-layout dump (`source file list dump/gof2hd_1.1.19_ios.cpp`,
`Classes/Game/LevelScript.cpp`) lists exactly the same non-`process` members as
the Android dynsym: ctor/dtor, `setAutoPilotToProgrammedStation`, `resetCamera`,
`lookBehind`, `render3D`, `skipSequence`, `canSkipCutsceneNow`,
`skipCutscene`, `resetStartSequenceOver`, `startSequenceOver`,
`startSequence`, `getEvent`, `setEvent`.  **There are no per-state helper
functions in either build**; every state body is inline in `process`.

The mission-0 intro is structurally identical in both builds:

* entry `messages[2]->isOver() && m_nState == 0` -> state-0 body (player
  `(18000,-12000,-40000)`, camera `(-12000,2000,-500)`,
  `MatrixSetTranslation`, three ships at `(-10000,500,0)`,
  `(-10000,-300,-1700)`, `(-10000,-200,2000)`, `setDirection({1,0,0},{0,1,0})`
  + `setVisible(true)`, `lodManager->forceUpdate(30,false)`).
* `else if (m_nState <= 2)` wobble loop
  (`layout->getPulseValue(0.0005f) - 0.5f`, three
  `translate(0,wobble,0)`), then the `m_nState == 1` (message 6 ->
  target/state 2/`setPosition(-5000,300,-5000)`) and `m_nState == 2`
  (sound 142, `translate(delta*0.2,0,delta*2.2)`, message 7 -> state 3 +
  three-ship exhaust/visible/awake/`field_0x128=50000`/`enemyFlagsLo=1`)
  arms.
* `else if (m_nState == 4)`, `else if (m_nState == 3)`,
  `else if (m_nState == 5)`, then a `switch (m_nState)` covering **6..15**
  and a final `if (m_nState == 16)`.

The iOS disassembly has the same compare order **4, 3, 5** and the same
`subs #6; cmp #9; tbh` switch range, and the same state-16 test.  Same
constants (18000/-12000/-40000, -12000/2000/-500, -10000/+500/0,
-10000/-300/-1700, -10000/-200/2000, -5000/300/-5000, 0.2/2.2,
25000/-200/-1000, pi/2, 0.98, 0.3, 0.2, sound events 142/143/157/158/161,
message indices 2/6/7/8/10/12/13/14).  The mission-1 intro is also identical
(`(delta<<1)&-16` angle, `enemy[0]` sleep/creep, message 2 fade-out).

**No state was collapsed or split, and no enum value differs.**  The iOS
function is larger (93,948 B) only because 1.1.4 carries more per-mission
cutscene bodies; the mission-0/1 state machine itself matches 2.0.16.

## Pre-switch mission specials: separate `if`, not `else if`

The one structural divergence found is the reachability of the pre-switch
mission specials.  In the original, `0x137f82` (`beq.w 0x13812a` when
`m_bStartSequence == 0`) and the deactivation block's fall-through both land
on the mission-5 check at `0x13812a`; the block therefore ends at `0x138128`
with a plain fall-through and the check is a **separate `if`**:

```cpp
if (m_bStartSequence != 0) { ... }
if (Globals::status->getCurrentCampaignMission() == 5) { ... }
else if (... mission 26 ...) ...
```

Our tree writes the mission-5 arm as `else if`, so when the sequence is active
the specials are skipped (semantically wrong) and the check is emitted at
`0x13824e` instead of `0x13812a`.  Changing it to a separate `if` was measured:

| variant | size | raw |
|---|---:|---:|
| current `else if` | 57,232 | 52,281 |
| separate `if` | **57,196** | 55,360 |

The separate-`if` variant is size-short by 36 B (31 fewer instructions) and
its mission-81/mission-26 block order becomes 5, 26, 81 whereas the original
lays them out 5, 81, 26; the `else if` layout is a codegen local optimum, so
the structural fix was **not landed**.  It is recorded here for a future pass
that can recover the missing 36 B (the mission-50/51 `goto` chain is the
suspect).

## Landed win (code): targeted statement-order moves

A statement-move sweep (move one simple statement down past 1-3 statements at
the same brace depth) was restricted to the 674 source lines that own
mismatched instructions in the aligned diff (12,400 mismatched instructions
including literal-pool words; ~1,983 real code instructions).  4,181 raw
candidates were generated, 833 hot candidates measured; four independent
size-exact, `.rel.plt`-identical raw wins were found.  Three compose:

| site (base lines) | move | alone raw |
|---|---|---:|
| 2262 `player->hideShipForFirstPersonCameraView(false);` | after `m_pCamera->setLookAtCam(true)` | 52,278 |
| 2502 `camPos += dir * 3000.0f;` | after `m_pLevel->lodManager->forceUpdate(delta,false)` (still before `setPosition`) | 52,276 |
| 2533 `camPos += dir * 2000.0f;` | after `m_pCamera->setPosition(camPos)` | 52,277 |

Measured together: raw **52,281 -> 52,253** (-28 B), size 57,232/57,232 exact,
`.text` **74,842 -> 74,814** (-28), `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04` (byte-identical).  The first two moves are
behaviour-neutral (the `setPosition` calls still see every offset).  The third
is a codegen-shape move: the original disassembly applies `dir*2000` before
`setPosition` (`0x13c10c-0x13c150`), but the compiler may legally sink the
local `camPos` addition past the const-ref call; the moved form matches one
more byte.  A behaviour-neutral variant (move after `camPos.y += 800.0f`,
before `setPosition`) also wins but only 1 B (raw 52,274).

## Second sweep (non-hot candidates): case-42 delete order

The same sweep was then run over a 1-in-11 sample (305) of the 3,348
candidate moves whose source line is *not* in the mismatch map.  It found
150 raw-lowering moves (most are neutral 1-2 B codegen-shape permutations).
One is a genuine structural fix worth 20 B, in the mission-42 wormhole-exit
arm:

```cpp
// before
delete m_pLevel->objectivesB;
delete m_pLevel->objectivesA;
m_pLevel->objectivesB = nullptr;
m_pLevel->objectivesA = nullptr;
// after (matches the original 0x13abe6-0x13ac12 exactly)
delete m_pLevel->objectivesB;
m_pLevel->objectivesB = nullptr;
delete m_pLevel->objectivesA;
m_pLevel->objectivesA = nullptr;
```

The original disassembly deletes `objectivesB` (`+0x2c`), stores
`objectivesB = nullptr`, then deletes `objectivesA` (`+0x28`), then stores
`objectivesA = nullptr` -- i.e. each pointer is nulled immediately after its
own delete, not both deletes first.  Measured: raw **52,273 -> 52,253**
(-20 B), size 57,232/57,232 exact, `.text` **74,834 -> 74,814**, `.rel.plt`
unchanged.

## Gate (after the win)

```
byte_exact               4507        linked_exact 4507
size-exact               4524/4524   stub_zero_size 0
data_exact               843/843     imports 207
lint                     CLEAN       drift 0
symbol parity            5370/5370   missing/wrong_type/extra 0/0/0
RATCHET                  PASS
```

`.text` 74,814; `.ARM.extab` unchanged (size/base exact at `0x1a3b78`,
156,312 B); `.rel.plt` byte-identical; 27/27 section headers; file size
2,187,328 exact.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12804
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52253 size=57232/57232 text=74814 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```
