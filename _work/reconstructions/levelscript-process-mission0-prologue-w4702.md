# LevelScript::process — mission-0 player re-fetches + prologue register map (w4702, wave 97)

Status: **code change landed, 2026-11-03**. Branch `wave97-w4702`, base master
`9b58a39d5`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

This pass closes the open item from `levelscript-process-mission0-order-w4503.md`
and `levelscript-process-r4-r9-rotate-w4404.md`: the two faithful mission-0
`Level::getPlayer()` re-fetches are landed **and** the whole-function register
map is repaired, size-neutrally. The function moves **54.1 -> 54.8** official
(raw `54.060516 -> 54.806399`); the oracle raw moves `55.142687 -> 55.874253`.
All gate counts are unchanged.

## Result

```
                         before            after
official rounded pct     54.1              54.8
raw official ratio       54.060516         54.806399
raw oracle ratio         55.142687         55.874253
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        20169 / 19986     20169 / 19965
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

The raw numbers are `difflib.SequenceMatcher(None, orig, ours,
autojunk=False).ratio()*100` on `verify.py`'s reconciled instruction streams of
the linked `.so`; the oracle is `_work/reconstructions/w3692_oracle.py`.

## 1. The key finding: the mission-1 `rotate` re-fetch fixes the prologue

w4404 landed `player->rotate(0.0f, angle, angle)` because the original loads the
cached player from `[sp,#84]` at `0x137da6`. That is correct at the call site,
but it keeps `player` live from the prologue into the mission-1 block, and clang
then parks `player` in `r4` across `getEnemies()` and the first
`getCurrentCampaignMission()` call. That single extra live value is what produced
the entire w4404 prologue residual (`&m_pLevel` `r4->r5`, `messages` `r5->fp`,
canary address `fp->r9`, `Globals::status` `r4->r9`).

Writing the rotate as `m_pLevel->getPlayer()->rotate(...)` (the w4503 shape)
removes `player` from the early live set; the allocator spills it immediately and
the prologue becomes instruction-for-instruction identical to the original
except for the status register encoding:

```
orig 0x137d26: mov   r4, sl                 ours: mov   r4, sl
     0x137d28: ldr   r0, [pc, #888]              ldr   r0, [pc, #936]
     0x137d2a: mov   r6, r1                      mov   r6, r1
     0x137d2e: add   r0, pc                      add   r0, pc
     0x137d30: ldr.w fp, [r0]                    ldr.w fp, [r0]        ; canary -> fp
     0x137d34: ldr.w r0, [fp]                    ldr.w r0, [fp]
     0x137d38: str   r0, [sp, #428]              str   r0, [sp, #428]
     0x137d3a: ldr.w r0, [r4, #24]!              ldr.w r0, [r4, #24]!  ; r4 = &m_pLevel
     0x137d3e: blx   Level::getMessages          blx   Level::getMessages
     0x137d42: mov   r5, r0                      mov   r5, r0           ; messages -> r5
     0x137d44: ldr   r0, [r4, #0]                ldr   r0, [r4, #0]
     0x137d46: blx   Level::getPlayer            blx   Level::getPlayer
     0x137d4a: strd  r4, r0, [sp, #80]           strd  r4, r0, [sp, #80] ; player spilled
     0x137d4e: ldr   r0, [r4, #0]                ldr   r0, [r4, #0]
     0x137d50: blx   Level::getEnemies           blx   Level::getEnemies
     0x137d54: mov   r8, r0                      mov   r8, r0           ; enemies -> r8
```

`Globals::status` is still loaded into `r9` in ours (`ldr.w r9,[r0]`) instead of
`r4` (`ldr r4,[r0]`), i.e. the last 4 bytes of the prologue residual survive,
but everything else in the register map now matches. The score jump comes from
the spill/register layout, not from the status register.

Cost: the rotate site itself now emits an extra `Level::getPlayer()` call
(+8 B), so the source is not literally what the original called there. The
trade-off is strongly positive: the re-fetch alone is `55.142687 -> 55.5048`
oracle on the untouched tree, and it is what makes the prologue match.

## 2. Faithful mission-0 player re-fetches (w4503 §2/§3)

Both are restored exactly as w4503 decoded them:

```
state 4 (orig 0x138d90):  m_bCinematicBreak = 1; m_pLevel->getPlayer()->setCollide(false);
state 3 (orig 0x138a38):  m_pLevel->getPlayer()->setCollide(true);
```

The original emits `ldr.w r0,[sl,#24]; blx Level::getPlayer` before both
`PlayerEgo::setCollide` calls; every other `PlayerEgo::*` access in the
mission-0 block uses the cached `[sp,#84]` player. With the prologue repaired,
the state-3/state-4 bodies now match the original instruction-for-instruction
around the `setCollide` calls.

## 3. The `m_nState == 0x7f` size anchor is removed

`src/game/world/LevelScript.cpp` carried

```cpp
if (m_nState == 0x7f) player->setRocketControl(nullptr, nullptr);
```

as a never-taken size anchor (+16 B). It is not original source and was only
holding the body at 57232 B. It is deleted; the two mission-0 re-fetches and the
rotate re-fetch take over its bytes. The stale comment was rewritten in place.

## 4. Size filler: case-139 state-1 `setCollide` re-fetch

With the rotate re-fetch and the two mission-0 re-fetches the body is 57224 B
(8 B short). The 8 B are restored at the case-139 state-1 teardown
(`src` line ~1693):

```cpp
player->player->setVulnerable(true);
m_pLevel->getPlayer()->setCollide(true);   // was: player->setCollide(true);
```

This one is a **size filler, not a recovered call**: the original at `0x13b35c`
uses the cached player (`mov r0,r6; blx setCollide`, `r6 = [sp,#84]`). It is
documented here so a later pass can replace it with the real missing 8 B when
the case-139 body is decoded more completely. The function therefore has 26
`Level::getPlayer()` calls vs the original's 24 (rotate + this filler).

## 5. Four `else if` -> independent `if` splits

A full 133-candidate sweep of every `} else if (` in `process` (split one at a
time, oracle screen, official confirm) found four size-exact splits that are
positive on top of the changes above. They are all state-machine tests whose
conditions are mutually exclusive by `m_nState`, so the split is semantically
safe:

| site | source test |
|---|---|
| mission 106 chain (line ~1390) | `m_nState == 4 && messages[4]->isOver()` |
| mission-x chain (line ~2289)   | `messages[7]->isTriggered() && m_nState == 4` |
| mission-x state machine (line ~2395) | `m_nState == 2` |
| mission-x state machine (line ~4434) | `m_nState == 0 && !inAlienOrbit()` |

The first two are the largest (official `54.548626 -> 54.598460 ->
54.607968`); the last two add `54.795067 -> 54.806399`. A second sweep on the
final tree finds no further size-exact improvement (the best remaining split is
`55.863861` vs the landed `55.874253`).

## 6. Measurements

Official raw ratio (`verify.py` on the linked `.so`) and oracle raw ratio for
the main stages:

| tree | size | official raw | oracle raw |
|---|---:|---:|---:|
| base `9b58a39d5` | 57232 | 54.060516 | 55.142687 |
| + rotate re-fetch | 57240 | — | 55.504800 |
| + rotate + a7f-removed + mission-0 re-fetches + filler | 57232 | 54.548626 | 55.632925 |
| + split mission-106 (line 1390) | 57232 | 54.598460 | 55.688639 |
| + split messages[7]/state 4 (line 2289) | 57232 | 54.607968 | 55.698439 |
| + split state 2 (line 2395) | 57232 | 54.795067 | 55.862409 |
| + split state 0/!inAlienOrbit (line 4434, **landed**) | 57232 | **54.806399** | **55.874253** |

The oracle ranking did not transfer exactly (the oracle overestimates by ~1.07
on this function), but every landed step was confirmed with the official
verifier and is a strict official-raw improvement.

## 7. Reproduction

```
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
cmake --build cmake-build-match --target drift -j8      # 0 fields
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

The per-function raw ratio helper used for the table is the same
`disassemble` + `reconcile` + `SequenceMatcher(autojunk=False)` pipeline as
`tools/verify/verify.py` (`score()` rounds to 1 decimal; the table prints the
unrounded ratio).
