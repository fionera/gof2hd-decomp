# LevelScript::process — iOS-structure pass 3 (wave 185, w12904)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12904`, branch
`wave185-w12904`, base master `f390b0e68` (wave-184 harvest 10, raw **52,132**,
`.text` 74,591).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12904`.

Continuation of wave-183 w12704 and wave-184 w12804 (iOS 1.1.4 structural
oracle).  This pass re-diffs the iOS Mach-O implementation and the source-layout
dumps against the current state machine, then lands one measured statement-order
win.

## iOS 1.1.4 oracle: state machine, helpers, enum values

`_work/bins/ios_1.1.4_HD` is the stripped armv7 Mach-O.  `llvm-objdump
--triple=thumbv7` disassembles it; `LevelScript::process` is the Thumb function
at `0x134098` (93,948 B from `LC_FUNCTION_STARTS`).  The iOS 1.1.19 source dump
(`source file list dump/gof2hd_1.1.19_ios.cpp`, `Classes/Game/LevelScript.cpp`)
lists the same non-`process` members as the Android dynsym (ctor/dtor,
`setAutoPilotToProgrammedStation`, `resetCamera`, `lookBehind`, `render3D`,
`skipSequence`, `canSkipCutsceneNow`, `skipCutscene`,
`resetStartSequenceOver`, `startSequenceOver`, `startSequence`, `getEvent`,
`setEvent`).  **No per-state helper functions and no state split/collapse**:
every state body is inline in `process`, exactly as in 2.0.16.

The mission-0 intro is structurally identical in iOS 1.1.4 and Android 2.0.16:

* mission-0 entry `messages[2]->isOver() && m_nState == 0` -> state-0 body
  (`m_nState = 1` store first, player `(18000,-12000,-40000)`, camera
  `(-12000,2000,-500)`, `MatrixSetTranslation`, three ships at
  `(-10000,500,0)`, `(-10000,-300,-1700)`, `(-10000,-200,2000)`,
  `setDirection({1,0,0},{0,1,0})` + `setVisible(true)` loop,
  `lodManager->forceUpdate(30,false)`).
* `else if (m_nState <= 2)` wobble loop
  (`layout->getPulseValue(0.0005f) - 0.5f`, three `translate(0,wobble,0)`),
  then the `m_nState == 1` (message 6 -> target/state 2/
  `setPosition(-5000,300,-5000)`) and `m_nState == 2` (sound 142,
  `translate(delta*0.2,0,delta*2.2)`, message 7 -> state 3 + three-ship
  exhaust/visible/awake/`field_0x128=50000`/`enemyFlagsLo=1`) arms.
* `else if (m_nState == 4)`, `else if (m_nState == 3)`,
  `else if (m_nState == 5)`, then `switch (m_nState)` covering **6..15** and a
  final `if (m_nState == 16)`.

The iOS compare order is **4, 3, 5** (0x134500 `cmp #4`, 0x134504 `cmp #3`,
0x134606 `cmp #5`) and the switch is `subs #6; cmp #9; tbh` -- the same as the
Android original.  Same constants (18000/-12000/-40000, -12000/2000/-500,
-10000/+500/0, -10000/-300/-1700, -10000/-200/2000, -5000/300/-5000, 0.2/2.2,
25000/-200/-1000, pi/2, 0.98, 0.3, 0.2, sound events 142/143/157/158/161,
message indices 2/6/7/8/10/12/13/14).  **No enum value differs.**

The mission-1 intro is also identical in emitted order: `forceUpdate(delta,
true)`, `rotate(0,angle,angle)` with `angle = ((delta<<1)&~15) * 2^-16 * 2*pi`,
`enemies[0]->getPosition()`, `setToSleep()`, depth clamp / `moveForward`,
message 2 fade-out, then `translate(delta*0.1,0,0)`.  Our source's statement
order (rotate, setToSleep, getPosition, forceUpdate) is a codegen local
optimum: all 24 permutations of the four statements were measured on the
current body; the current order is the unique best (52,132; every other order
is +1..+7 raw, all size-exact / `.rel.plt`-identical).

### Mission-0 4/5/3 arm order

The one remaining structural divergence is that our source order is 4, 5, 3
(state 3 last), so clang 7 SimplifyCFG merges the state-3 arm into the
`switch`, giving `subs #3; cmp #12` (13 entries 3..15) instead of the
original's `subs #6; cmp #9` (10 entries 6..15).  The faithful source order
4, 3, 5 (state 5, a compound `m_nState == 5 && msg12->isOver()` test, last)
prevents the merge.  All six arm permutations were re-measured on the current
body:

| order | raw | size | `.rel.plt` |
|---|---:|---:|---|
| **4,5,3 (base)** | **52,132** | 57,232 | `289b2a06…` (identical) |
| 4,3,5 (original) | 55,022 | 57,240 (+8) | `a8d49c…` DIFF |
| 5,4,3 | 55,079 | 57,236 (+4) | `e185e7…` DIFF |
| 5,3,4 | 55,079 | 57,236 (+4) | `e185e7…` DIFF |
| 3,4,5 | 55,195 | 57,248 (+16) | `a8d49c…` DIFF |
| 3,5,4 | 53,398 | 57,232 | `614d07…` DIFF |

The faithful order is a net loss (raw +2,890, size +8, `.rel.plt` rotation),
so it was not landed; the merge remains an irreducible codegen delta on this
body.  This confirms w12802's finding.

## Landed win (code): mission-154 state-9 `setFreeze(false)` sink

A K=9..12 non-adjacent statement-move sweep (1,009 candidates, 889 measured,
four workers, `ulimit -v 12000000`) found exactly one size-exact,
`.rel.plt`-identical raw winner.  In the mission-154 `m_nState == 9` exit block
the player freeze release is sunk past the camera reset:

```cpp
// before (source lines 4751-4761)
m_pCamera->setLookAtCam(false);
player->setFreeze(false);
m_pCamera->setTarget(player->geometry);
player->setSpeed(2.0f);
player->setComputerControlled(false);
player->player->setVulnerable(true);
player->setPosition(0.0f, 0.0f, 90000.0f);
m_pHud->visible = 1;
m_pLevel->lodManager->forceUpdate(delta, false);
m_pRadar->enabled = 1;
resetCamera(m_pLevel);
// after
m_pCamera->setLookAtCam(false);
m_pCamera->setTarget(player->geometry);
player->setSpeed(2.0f);
player->setComputerControlled(false);
player->player->setVulnerable(true);
player->setPosition(0.0f, 0.0f, 90000.0f);
m_pHud->visible = 1;
m_pLevel->lodManager->forceUpdate(delta, false);
m_pRadar->enabled = 1;
resetCamera(m_pLevel);
player->setFreeze(false);
```

`setFreeze(false)` is independent of every crossed statement (camera setters,
player speed/control/vulnerability/position, HUD visibility, LOD update, radar
enable, `resetCamera`); the move is behaviour-neutral.

Measured: raw **52,132 -> 52,128** (-4 B), size 57,232/57,232 exact,
`.text` **74,591 -> 74,587**, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04` (byte-identical), `.ARM.extab` unchanged
(size/base exact at `0x1a3b78`, 156,312 B), 27/27 section headers, file size
2,187,328 exact.

```
byte_exact               4507        linked_exact 4507
size-exact               4524/4524   stub_zero_size 0
data_exact               843/843     imports 207
lint                     CLEAN       drift 0
symbol parity            5370/5370   missing/wrong_type/extra 0/0/0
RATCHET                  PASS
```

## Rejected probes

* Mission-1 statement permutations: current order is the unique best (see
  above).
* Mission-0 state-0 `m_nState = 1` placement: current position (after the two
  `setPosition` calls) is the unique best; moving it before the player
  `setPosition` is +4 raw, after `MatrixSetTranslation` / the enemy block is
  +2,947 raw and size-changing.
* Mission-1 `getPosition` before `setToSleep` (the original emitted order):
  +1 raw.
* Mission-0 state-4 body rewritten in the Android emitted call order (19
  statements): +16 raw; rewritten in the iOS emitted order: +24 raw and
  `.rel.plt` DIFF.
* Mission-0 state-6 `m_nScriptTimerA += delta` sunk past
  `updateEvent3DAttributes` (the original emitted order): +1 raw on the landed
  tree.
* Single-use `RadioMessage *` aliases (`introMsg`, `msg6`, `msg2`) removed:
  neutral (coalesced away).
* Mission-0 state-1 `TargetFollowCamera *wcamera` alias removed: neutral raw
  (aligned -10), so not landed.
* K=9..12 statement moves: 1 winner (landed); no other size-exact raw win.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12904
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52128 aligned= 20657 size= 57232/57232 insn= 20725/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52128 size=57232/57232 text=74587 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Sweep harnesses (`harness.py`, `gen_k912.py`, `sweep_k912.py`,
`perm_m1.py`, `perm_state345.py`, `perm_s0.py`, `res_k912.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w12904/` (scratch, not tracked).
