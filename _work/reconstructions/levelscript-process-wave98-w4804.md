# LevelScript::process — statement-order pass 6 (w4804, wave 98)

Status: **code change landed, 2026-11-04**. Branch `wave98-w4804`, base master
`fe4f5e7cf`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

This pass is a second independent attack on `process` after w4702 (54.8%). It
finds a further set of **size-neutral source edits** that each restore a matching
instruction run, on top of the w4702 tree.  The official raw fuzzy ratio moves
**54.806399 -> 54.898346**; the size stays exactly `0xdf90`, `.rel.plt` stays
byte-identical, and every gate count is unchanged.

## Result

```
                         before            after
official raw ratio       54.806399         54.898346
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        20169 / 19965     20169 / 19965
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
```

"before" is HEAD `fe4f5e7cf` (w4702 + w4504 + the wave-97 fixes).  The raw
number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False).ratio()*100`
over `tools/verify/verify.py`'s reconciled instruction streams of the linked
`.so`.

## The edits

All three are pure source-statement moves: no anchor added, no `volatile`
added, no call added or removed.  Each was found by diffing the normalized
instruction streams and looking for a store that the original emits in a
different position inside an otherwise equal run.

### 1. mission-125 lead-ship camera: `m_pRadar->enabled = 0` before the leader load

`src` line ~2868 (the `isInTurretMode()` cutscene-entry block):

```cpp
m_pHud->visible = 0;
AEGeometry *leader = (*enemies)[0]->geometry;   // was here
m_pRadar->enabled = 0;
m_pCamera->setTarget(leader);
```
->
```cpp
m_pHud->visible = 0;
m_pRadar->enabled = 0;
AEGeometry *leader = (*enemies)[0]->geometry;
m_pCamera->setTarget(leader);
```

Original `0x13a1c6..0x13a1e4` stores `m_pHud->visible`, `m_bCinematicBreak`,
`m_pHud->visible`, then `m_pRadar->enabled`, and only afterwards loads
`(*enemies)[0]->geometry`; ours had the leader load before the radar store.
This is the same shape as the w4702 mission-0 fix and is worth ~+0.02 alone.

### 2. state-1 conclude: `m_nState = 2` before `m_bCinematicBreak = 0`

`src` line ~2163 (case 72 state-1 teardown, original `0x13bffc`):

```cpp
m_bCinematicBreak = 0;
m_nState = 2;
```
->
```cpp
m_nState = 2;
m_bCinematicBreak = 0;
```

The original emits `str r1,[r4,#28]` (m_nState) before `strb r6,[r4,#17]`
(m_bCinematicBreak); ours had them the other way.  +0.005.

### 3. mission-125 state-3: `m_nState = 4` instead of `m_nState++`

`src` line ~3594 (mission 125 `m_nState == 3` block, original `0x13dd20`):

```cpp
m_nState++;          // was
```
->
```cpp
m_nState = 4;        // original emits movs r0,#4; str r0,[r5,#28]
```

The original emits a constant store here, not a load-add-store.  This is the
largest single win of the pass (+0.010).

### 4. three more `m_nState++` -> constant stores

A full sweep of every `m_nState++` in `process` (36 sites, each replaced by
`m_nState = <state+1>`) found three more size-exact wins:

| source line | block | change | raw delta |
|---|---|---|---|
| 1832 | `case 1` (0x139...) | `m_nState++` -> `m_nState = 2` | +0.010 |
| 1923 | `case 5` | `m_nState++` -> `m_nState = 6` | +0.005 |
| 2885 | mission-0 state-1 | `m_nState++` -> `m_nState = 2` | +0.012 |

All three are semantically identical (the enclosing `case`/`if` pins the old
value) and the original emits a constant store at each.  Combined they take the
raw ratio from 54.871182 to **54.898346**.

### 5. (already in the tree from the w4804 experiment) case-92 / case-102 reorders

These were the two reorders that the earlier w4804 attempt had measured but not
landed:

* case 92 (`~line 3024`): `m_pCamera->setTarget(player->geometry);` now comes
  before `m_tempVec = player->geometry->getPosition();`.
* case 102 (`~line 3852`): `m_tempVec = AEMath::MatrixGetPosition(local);` now
  comes before `m_dirVec = leaderShip->getDirection();`.

The case-102 move costs 8 B on its own, so it is paired with the 8-byte
`volatile int pad2 = delta;` anchor before the mission-0 facing loop (the same
anchor the earlier w4804 experiment used).  Both are documented here.

## What was tried and rejected (all measured with the official verifier)

| variant | size | raw % | note |
|---|---:|---:|---|
| HEAD | 57232 | 54.806399 | base |
| + reorder 1 (radar/leader) | 57232 | 54.836298 | size-exact |
| + reorder 2 (state/break) | 57232 | 54.861215 | |
| + `m_nState = 4` (mission 125) | 57232 | 54.871182 | |
| + three more `m_nState++` -> const | 57232 | **54.898346** | landed |
| mission-0 arm order `345` on top | 57240 | 54.651 | 8 B too big |
| `m_nState = 10` before `m_nScriptTimerA = 0` | 57224 | 54.869 | -8 B (tail merge) |
| `m_nState++` at the three `setFixed(false)` sites | 57232/57236 | 54.816/54.795 | worse |
| `m_nState = m_nState + 1` -> const (10 case-125 sites) | 57224-57236 | 54.44-54.89 | all worse |
| `player->freeze = 0` before `m_nState = 2` | 57232 | 54.856 | worse |
| `setActive(true)` before `setTarget` (line 2143) | 57232 | 54.722 | worse |
| `m_bCinematicBreak=0` before `m_nState=3` (line 2277) | 57240 | 54.745 | +8 B |
| remove `pad` (state-0 tail anchor) | 57232 | 53.494 | -1.37 raw |
| remove `pad2` | 57224 | 54.830 | -8 B |
| mission-125 `state>=4` statement order | — | — | already instruction-identical |
| mission-125 separator bits `1011` | — | — | already best |

## The mission-125 `state>=4` block is already exact

The open item in the w4804 handoff (permute `setDockingType` / `setName` /
`field_0x74 = 1` / `deleteHackingGame`) is a dead end: our block at
`0x13d0cc..0x13d168` is instruction-for-instruction identical to the original
`0x13de44..0x13dee0` (only the `String` temp stack slot differs, `sp+172` vs
`sp+124`, which is a whole-function stack-layout effect).  The source order is
already the original order.

## Still open

* `Globals::status` is loaded into `r4` in the original and `r9` in ours
  (prologue `0x137d5a`); it is a global RAGreedy choice and did not move with
  any local source edit tried here.
* The `delta` / `&m_pLevel` / `player` stack-slot 3-cycle is unchanged.
* Several `m_nState = X; m_nScriptTimerA = 0;` tails are **tail-merged** in our
  build but emitted inline in the original (e.g. `m_nState = 10` at
  `0x13b642`, `m_nState = 2` at `0x13c480`).  Reordering the source to break
  the merge costs 4-8 B and lowers the raw ratio, so the merged form is kept.
  Recovering those bytes with a real statement is the next lever.

## Reproduction

```
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```
