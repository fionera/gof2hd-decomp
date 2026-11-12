# LevelScript::process — wave 185 / w12902: branch-layout pass (branch-shape map + safe statement-layout harvest)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12902`, branch `wave185-w12902`, base master
`f390b0e68` (wave-184 harvest 10, raw **52,132**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12902`.

Deeper branch-layout pass than w12702/w12603: build the complete branch map of
both bodies, group the target/offset differences by state block, then harvest
size-exact, `.rel.plt`-identical raw wins from semantically-independent
consecutive-statement runs.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,132           52,086     (-46)
process aligned insns    20,658           21,847     (+1,189)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,725  20,830 / 20,727
.text diffbytes          74,591           74,545     (-46)
.ARM.extab               unchanged (size/base exact)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
RATCHET                  PASS             PASS
```

## 1. Branch census (both bodies)

`objdump -d` over `0x137d18..0x145ca8`, counting every control-transfer
instruction except `bl`:

| mnemonic | original | ours | delta |
|---|---:|---:|---:|
| blx  | 2900 | 2900 | 0 |
| b    | 369  | 369  | 0 |
| tbh  | 15   | 15   | 0 |
| bx   | 8    | 8    | 0 |
| beq  | 200  | 189  | -11 |
| bne  | 182  | 179  | -3 |
| cbz  | 65   | 72   | +7 |
| cbnz | 24   | 26   | +2 |
| bcc  | 51   | 45   | -6 |
| bcs  | 3    | 8    | +5 |
| blt  | 139  | 132  | -7 |
| bge  | 18   | 24   | +6 |
| bhi  | 22   | 21   | -1 |
| bgt  | 12   | 12   | 0 |
| bls  | 4    | 4    | 0 |
| ble  | 4    | 2    | -2 |

Total branches 4,016 vs 4,006.  The 15 `tbh` tables match in count and range
(only the mission-0 `switch (m_nState)` differs — 10 entries 6..15 vs our
13 entries 3..15 — exactly as w12702 documented; the `else if (m_nState == 3)`
arm is merged into the switch by clang 7 SimplifyCFG).

The mnemonic deltas are **branch-shape** differences, not missing branches:
`cbz`/`cbnz` (+9) vs `beq`/`bne` (-14) and `bcs` (+5) vs `bcc` (-6), `bge`
(+6) vs `blt` (-7) are the same comparisons lowered with a different polarity
(pointer-vs-int test, `>=` vs `<`).  They are spread across the whole
per-mission switch and are not localised to one state block.

## 2. Branch-target/offset map

An opcode-normalised instruction alignment (registers erased, branch targets
replaced by target instruction index) matches 12,473 of 20,725 instructions
and reports 131 branch-target-opcode mismatches; a call-anchored alignment
(1,846 matched direct-call symbols) matches 8,475.  Both confirm the residual
is **block placement + register allocation**, not a missing or extra branch:
the two bodies are instruction-for-instruction equivalent in the prologue and
in the mission-0 dispatch, and every `tbh` has a counterpart.

## 3. Landed change — independent-statement layout harvest

A full permutation sweep of every 3-statement window (6,560 candidates) and
every whole 4-statement run (1,229 candidates), each compiled + gold-linked +
stripped and scored with `tools/fn_diff.py`'s raw metric, was filtered by a
statement-dependency model (a permutation is admissible only if it preserves
the relative order of every pair of statements that share a written location,
a call token `receiver->method`, or a read/write of the same identifier) and
then greedily composed.  The 30 composing moves below take raw
**52,132 → 52,086** (-46), size-exact and `.rel.plt`-identical:

| lines | move (new order) |
|---|---|
| 1614-1616 | translate, setTarget, setPosition |
| 3567-3569 | setPosition(m_dirVec), setPosition(pos-dir*2400), resetGunDelay |
| 3734-3736 | stopShooting, timerA=0, timerB=0 |
| 691-693 | setFreeLookMode, resetCamera, enableFirstPersonCam |
| 2363-2365 | forceUpdate, setLookAtCam, setSpeed |
| 2909-2911 | setHitpoints(0), ship=geometry, m_tempVec=getPosition |
| 3153-3155 | resetCamera, m_tempVec+=, setTarget |
| 3400-3402 | resetCamera, setVisible, setTurretMode |
| 4408-4410 | m_tempVec+=right, setPosition(m_tempVec), m_tempVec+=dir |
| 4828-4830 | setPosition, setTarget, setLookAtCam |
| 1655-1657 | SetCurrentApplicationModule(5), switch_to_target_setting=0, nextCampaignMission |
| 2011-2013 | setComputerControlled, radar->enabled=0, stopShooting |
| 2071-2073 | setExhaustVisible, setPosition, setRotation |
| 2111-2113 | timerA=0, sound->play(153), m_nScriptTime=0 |
| 2280-2282 | setTarget, setComputerControlled, setLookAtCam |
| 2325-2327 | setFreeLookMode, setTurretMode, resetCamera |
| 2395-2397 | setLookAtCam, setComputerControlled, m_bCinematicBreak=1 |
| 2440-2442 | setTarget, setLookAtCam, setActive |
| 2530-2532 | camPos.x+=, camPos.y+=, setPosition(camPos) |
| 2673-2675 | CameraSetLocal, frame->setPosition, delete frame |
| 2694-2696 | m_tempVec.z+=, MatrixSetTranslation, m_tempVec.x+= |
| 2921-2923 | m_dirVec=right, m_tempVec+=700, m_tempVec+=10000 |
| 3156-3158 | (3-stmt rotate) |
| 3186-3188 | (3-stmt rotate) |
| 3573-3575 | (3-stmt rotate) |
| 4455-4457 | (3-stmt rotate) |
| 4524-4526 | (3-stmt rotate) |
| 4598-4600 | (3-stmt rotate) |
| 4753-4755 | (3-stmt rotate) |
| 3974-3977 | timerA=0, new Route, setPlayerRoute, setRoute |

All moves are permutations of consecutive statements in the same basic block;
the two `setPosition` calls in the mission-102 state-6 block keep their
original relative order (only `resetGunDelay` moves after them).  The aligned
instruction count rises (20,658 → 21,847) because the raw metric rewards
same-offset byte coincidences, not instruction alignment; the gate metric for
this function is raw.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12902
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52086 aligned= 21847 size= 57232/57232
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`harness.py`, `sweep.py`, `sweep2.py`, `safety.py`,
`greedy.py`, `branchdiff2.py`, `opalign.py`, `tbh.py`, `eqruns.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w12902/`; they are scratch, not tracked.
