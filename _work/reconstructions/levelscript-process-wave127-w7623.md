# LevelScript::process — wave 127 / w7623 (case-41 statement order + misplaced translate)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Owned TU: `src/game/world/LevelScript.cpp`.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.238266968      60.077596498     (-0.160670470)
orig / ours insns        20169 / 20038     20169 / 20039
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54566             54559            (-7)
whole .text diffbytes    85520             85513            (-7)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.ARM.exidx diffbytes     0                 0                (byte-identical)
.ARM.extab size          156312            156312           (size/base exact)
.rel.plt diffbytes       0                 0                (byte-identical)
.rodata/.data/.got       0 / 0 / 0         0 / 0 / 0
.dynsym/.dynstr/.hash/.rel.dyn  0 / 0 / 0 / 0
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (helper `/dev/shm/w7623/score.py`).

## Landed changes (case 41, `m_nState == 1` / `== 3` / `== 4`)

All three are statement-order / statement-placement corrections confirmed
against the original disassembly and the Ghidra decompilation of
`LevelScript::process`.

### 1. `TargetFollowCamera::translate(3000, 1000, 2000)` belongs to state 3

Our source had this call at the **end of state 1**, after
`LODManager::forceUpdate`.  The original state-1 body ends at `0x139e30`
with `Player::StopEngineSound` + `m_nScriptTimerA = 0` (no translate); the
`(3000, 1000, 2000)` call is in the **state-3** body at `0x1412f4..0x1412fe`,
immediately after `TargetFollowCamera::setPosition(freighter->getPosition())`
and before `useTargetsUpVector(false)`:

```
1412f0  blx  TargetFollowCamera::setPosition(Vector const&)
1412f4  ldr  r1,[pc,#936]     ; 0x1416a0 = 0x453b8000 (3000.0)
1412f6  ldr  r2,[pc,#940]     ; 0x1416a4 = 0x447a0000 (1000.0)
1412f8  ldr  r0,[r9,#20]      ; m_pCamera
1412fc  ldr  r3,[pc,#936]     ; 0x1416a8 = 0x44fa0000 (2000.0)
1412fe  blx  TargetFollowCamera::translate(float,float,float)
141302  ldr  r0,[r9,#20]
141306  movs r1,#0
141308  blx  TargetFollowCamera::useTargetsUpVector(bool)
```

Ghidra confirms: `fVar55 = (float)TargetFollowCamera::setPosition(...);
TargetFollowCamera::translate(...,fVar55,...); ... useTargetsUpVector(false);`.

### 2. `FModSound::play(155, ...)` comes after the state-1 camera translate

The original state-1 tail `0x139dfe..0x139e2c` is:

```
139dfe  blx  TargetFollowCamera::translate(-3000,-2000,12000)
139e0a  blx  FModSound::play(155, 0, 0, 0.0)
139e20  blx  Player::StopEngineSound
139e2c  strd r8, r8, [r6, #144]      ; m_nScriptTimerA = 0
```

Our source played 155 before the translate.

### 3. Duplicate `m_nState = 5` removed from state 4

The original state-4 `>= 15001` block stores `m_nState = 5` exactly once
(`0x141206..0x14120a`); our source had a second `m_nState = 5` later in the
same block.  Removing it is codegen-neutral and byte-identical in size.

## Case-40 findings measured this pass (not landed)

Case 40 (`0x13a4d8` state 0, `0x13e6b4..0x13e80c` states 1-4) has three
genuine fidelity bugs, all confirmed by disassembly and Ghidra:

1. **State 0 has a bogus `m_nScriptTime = 0`.**  The original state-0 body
   `0x13a4d8..0x13a622` contains no store to `this+8` (only
   `str r6,[r0,#40]` shipGroup, the two `strb` HUD/radar stores,
   `strb r6,[r5,#17]` cinematicBreak and `str [r5,#28]` m_nState).
   Our source emits `strd r9,r9,[sl,#8]` at `0x13c598`.
2. **`KIPlayer::setVisible(true)` belongs to state 0, not state 1.**
   Original `0x13a56c..0x13a574` calls it in state 0 (after `awake()`);
   the state-1 body at `0x13e6c0` starts with
   `translate(0, 0, (float)(-2*delta))` and has **no** `setVisible`.
   Our source has the call in state 1 and the translate missing.
3. **State 4 must set `m_nState = 5`, not `speed = 0`.**  Original
   `0x13e7ea..0x13e808`: `setPosition(0,0,-200000); setActive(false);
   b.w 0x141b54` where `0x141b54` is `movs r0,#5; b.w 0x1438d4`
   (`str [this+0x1c]`).  Our source stores `(*enemies)[0]->speed = 0`
   and never advances the state, so the mission would hang at state 4.
   (The `speed = 0` store is real but lives in **case 41** state 4 at
   `0x141222`, `str r8,[r0,#100]`.)

These are *not* size-neutral: applying the full case-40 correction lands on
57256 B (+24).  The measured variants were:

| variant | raw % | size | process diffbytes |
|---|---:|---:|---:|
| **master** | **60.238267** | **57232** | **54566** |
| remove case-40 `m_nScriptTime=0` only | 60.236769 | 57232 | 54576 |
| + case-40 state-0 setVisible (keeping state-1 copy) + state-4 `m_nState=5` | 60.199970 | 57232 | 54611 |
| faithful case-40 state 0/1/4 (move setVisible, add translate) | 59.428543 | 57256 | — |
| landed case-41 trio | 60.077596 | 57232 | 54559 |

The case-40 fix needs a -24 B companion before it can be landed under the
size-exact gate; it is left for a later pass.

## Remaining root cause

Unchanged: global MachineBlockPlacement permutation plus the prologue
register map (`messages` r4 vs r5, `&Globals::status` r9 vs r4, `this` in
`sl` vs `[sp,#88]`).  This pass is a local content/order correction.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7623/score.py                         # 60.077596498
python3 /dev/shm/w7623/bytediff.py                      # process diffbytes 54559
```

Scratch helpers (`score.py`, `bytediff.py`, `prologue_map.py`, `try*.py`,
`search.py`, `ghidra_process.md`) live in `/dev/shm/w7623/` and are not part
of the tree.
