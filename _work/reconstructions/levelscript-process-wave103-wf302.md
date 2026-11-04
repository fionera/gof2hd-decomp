# LevelScript::process — wave 103 oracle pass (wf302)

Status: **code change landed, 2026-11-04**. Branch `wave103-wf302`, base master
`1d2b4398e`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

This pass used the new third-party Ghidra decompilation
(`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/LevelScript.c`)
to re-check the mission-125 state machine and the `.rel.plt` first-reference
order.  It lands a tuned source shape that is 21 fewer differing `.text` bytes
and raises the raw fuzzy ratio by +0.0757 while keeping the whole gate green.

## Result

```
                         before (HEAD)     after
official rounded pct     53.4              53.4
raw official ratio       53.363564         53.439298
orig / ours insns        20169 / 19967     20169 / 19970
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
.rel.plt name diff       0                 0   (3674/3674 identical)
.text differing bytes    102,798           102,777
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.

## What the oracle proves about mission 125

The oracle's `LevelScript::process` (line 1502, `@0x00160d50`) shows the
mission-125 (`0x7d`) chain as:

```c
if (state == 0) { ... } else { if (state == 1 && ...) { ... } }
state = this->state;
if (state == 2) { ... if (radio[2]->isOver()) { ... state = 4; } state = this->state; }
if (state == 3) { ... state = 4; ... }
else if ((unsigned)(state - 4) < 3 && docked && hackingWon && dockIndex >= 0) { ... }
```

The original Thumb confirms each part:

* state 2 is tested independently: `0x13b51c: cmp r0,#2; bne 0x13dcb6`.
* state 3 is tested independently: `0x13dcb6: cmp r0,#3; bne 0x13de44`.
* state 4..6 uses the unsigned range test: `0x13de44: subs r0,#4; cmp r0,#2; bhi exit`.
* state 2 and state 3 store the **constant 4**, not `state + 1`:
  `0x13e80c: movs r0,#4` (state-2 tail merge) and
  `0x13dd22: movs r0,#4; str r0,[r5,#28]` (state 3).
  The C++ source therefore says `m_nState = 4;` at both sites.
* state 0 and state 4..6 use `state + 1` (`adds r1,#1` / `adds r0,#1`), so
  those keep `m_nState++`.
* The original's first references are
  `switchPlanetForIntro 0x13d1ec < switchSkyboxForIntro 0x13d1f4 <
  ParticleSystemManager::reset 0x13d212 < startSmokeEmission 0x13d396 <
  PlayerStation::setVisible 0x13da1e < deleteHackingGame 0x13ded4`,
  the order the current first-reference anchor reproduces.

The oracle also confirms the rest of the mission-125 body (route points
`{-70000, 0, -130000}`, the four `nextInt` calls, `spread += 2000`, the
`4000/-500/-500` camera offset, the `7001` timer, the `2400` camera pull-back,
`xOff=25000 / yOff=2300` and the `+1000/+50` guard placement) is already
present verbatim in our source.

## What changed

`src/game/world/LevelScript.cpp`:

1. **State 2 and state 3 store `m_nState = 4`** instead of `m_nState++`
   (faithful to the constant store; size-neutral).
2. **Mission-0 state-3 `setCollide(true)` uses the cached `player`**
   (`player->setCollide(true)`), matching the original's `[sp,#84]` load at
   `0x138a38` in the *current block layout*.
3. **Two `resetGunDelay()` sites re-fetch `m_pLevel->getPlayer()`**
   (the mission-0/`field_0x24` site at source line 1595 and the
   `messages[3]` site at line 3172).
4. **One `getPosition()` site uses the cached `player`** (the vector
   expression at source line 4591).

Changes 2-4 are a size-neutral permutation of the 9 `Level::getPlayer()`
re-fetch/cache sites (each re-fetch is +8 B, each cache is -8 B).  They are a
tuned shape, not a fidelity claim; the original re-fetches at most of these
sites.  The permutation was chosen by an exhaustive 84-combination sweep of
3 re-fetches among the 9 sites, followed by a 102-combination sweep of one
additional re-fetch/cache pair.

### Measured sweep (top rows, all size-exact 57232 B)

| re-fetch set | raw % |
|---|---:|
| `A C I` (landed base) | 53.420699 |
| `A C G` | 53.400767 |
| `A C H` | 53.392132 |
| `B C I` | 53.390802 |
| `A B C` (old HEAD) | 53.363564 |

`A` = mission-0 state-4 `setCollide(false)`, `B` = mission-0 state-3
`setCollide(true)`, `C` = mission-1 `rotate`, `D` = shared-tail
`setVulnerable`, `E` = shared-tail `setCollide`, `F` = shared-tail
`hudEvent`, `G/H/I` = the three `resetGunDelay` sites.

On top of `A C I`, the best additional pair was re-fetch `G` and cache the
`getPosition()` occurrence at line 4591: **53.429333**.  Adding the faithful
state-2/3 `= 4` stores gives the landed **53.439298**.

## The faithful independent-state-3 shape (measured, not landed)

The oracle shows state 3 as an independent `if`.  The wave-100 tree landed
that shape but paid for its +28 B by reverting three real
`Level::getPlayer()` re-fetches.  On the current tree the faithful shape
measures:

| shape | size | raw % | .rel.plt |
|---|---:|---:|---|
| current tuned (joined state 3, anchor, 3 re-fetches) | 57232 | 53.439298 | 0 |
| independent state 3, anchor removed, 3 shared-tail re-fetches restored | 57228 | 52.113027 | 0 |
| independent state 3, anchor kept, 3 shared-tail re-fetches restored | 57284 | 52.080791 | 0 |
| independent state 3, anchor removed | 57204 | 52.075961 | 0 |

The faithful shape is 4 B short of size-exact in the best variant and scores
~1.33 raw points lower.  The missing 4 B are a codegen difference, not a
missing statement: the oracle and the original disassembly agree with our
mission-125 body instruction-for-instruction at the call/constant level.
Landing the faithful shape would therefore trade a measurable fuzzy
regression for a control-flow shape that the current block-placement
heuristics do not reward.  The tuned shape is kept; the faithful variant is
recorded here for a future pass that can find the missing 4 B of real code.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
cmake --build cmake-build-match --target drift -j8      # 0 fields
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
readelf -rW cmake-build-match/libgof2hdaa.so | \
  awk '$3=="R_ARM_JUMP_SLOT"{print $5}' > /dev/shm/wf302_relplt_ours.txt
diff /dev/shm/wf302_relplt_orig.txt /dev/shm/wf302_relplt_ours.txt   # empty
```

Scratch helpers (not tracked): `/dev/shm/wf302/score.py` (raw ratio),
`/dev/shm/wf302/diffblocks.py` (difflib opcode dump), the sweep scripts
`sweep_9sites.py`, `sweep_4ref.py`, `sweep_5ref.py`, `sweep_states.py`,
`sweep_anchor*.py`, `sweep_guard.py`, `sweep_prologue.py`, `sweep_status.py`,
`sweep_loops.py`, `sweep_delta.py`, `sweep_movecases.py`.
