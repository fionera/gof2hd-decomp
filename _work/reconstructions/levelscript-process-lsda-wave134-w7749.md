# LevelScript::process — wave 134 / w7749: LSDA record decode + prologue/getPlayer experiments

Status: **research note, no landed source change**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7749`. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Owned TU: `src/game/world/LevelScript.cpp`.
Base master `ec0acea1e` (wave-133 docs sync).

## Result

```
                         before (master)   after (no change)
raw difflib ratio        58.851615149      58.851615149
official verify pct      58.9              58.9
orig / ours insns        20169 / 20044     20169 / 20044
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process differing bytes  54465             54465
.ARM.extab differing     1416              1416
.ARM.exidx               0                 0
.rel.plt                 0                 0
.rodata/.data/.bss       0                 0
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Note: the repo-root `report.json` can lag the build-dir report. The
authoritative row is `cmake-build-match/verify/report.json`; on this base it
says `pct 58.9`, while the committed root copy still says `59.9`. The
`59.879` raw figure quoted by the w7688/w7708 notes is likewise stale.

## 1. The single `.ARM.extab` record owned by `process`

`.ARM.exidx` has exactly one entry for the function (`0x1a12a0`), pointing at
extab record `0x1c2af8`, record length **348 B** in both. Decoded with the
committed `_work/reconstructions/extab_lsda_decode.py`:

| field | original | ours |
|---|---:|---:|
| personality | `0x5eb60` | `0x5eb60` |
| unwind words | 3 | 3 |
| `lpStart` encoding | `0xff` (omit) | `0xff` |
| `ttype` encoding | `0xff` (cleanup only) | `0xff` |
| call-site encoding | `0x01` (ULEB) | `0x01` |
| call-site table bytes | 324 | 326 |
| **rows** | **46** | **46** |
| rows with a landing pad | **24** | **24** |
| distinct landing pads | **24** | **24** |
| raw differing bytes | — | **291 / 348** |

Same record size, same row count, same landing-pad count, same single
cleanup-only personality, same ttype encoding. Every differing byte is a ULEB
row start / length / landing-pad offset, i.e. code-layout derived.

## 2. Row layout: the first row is the prologue accessor order

Full row table (start, length, landing-pad offset, action index):

```
ORIGINAL (fn 0x137d18)                          OURS (fn 0x137d18)
row start  len  lp    action                    row start  len  lp    action
  1   5832   6  57116 delete r6                  1   5650   6  57094 delete r5
  3   6586   8  57080 String sp+112              3   6422   8  57080 String sp+112
  5   8226   8  57106 delete r5                  5   7658   8  57096 delete r5
  7   8898   8  57104 delete r5                  7   8298   8  57106 delete r5
  9   9624   8  57074 String sp+220              9   8436  10  57162 String sp+280
 11   9942   6  57114 delete r5                 10   8448  10  57152 String sp+368
 13  12162   8  57098 String sp+232             11   8458  14  57142 String sp+268
 15  14224  14  57162 String sp+280             12   8472   8  57132 String sp+136
 16  14242   8  57152 String sp+368             14   9988   8  57072 String sp+220
 17  14250  12  57142 String sp+268             16  10246   6  57108 delete r5
 18  14262   8  57132 String sp+136             18  19600   8  57100 String sp+232
 20  19988   6  57086 delete r5                 20  23672   8  57058 String sp+124
 22  24978   8  57058 String sp+124             22  25182  12  57098 delete r5
 24  26118  12  57096 delete r5                 24  27726   8  57116 delete r6
 26  28924   8  57108 delete r8                 26  29670  10  57088 delete [sp+72]
 28  29260   8  57064 delete r6                 28  30688   6  57078 delete r5
 30  29820  10  57090 delete [sp+72]            30  31966   8  57110 delete r8
 32  36488   4  57122 delete r5                 32  36784   4  57122 delete r5
 34  39118   6  57088 delete r6                 34  38912   6  57086 delete r6
 36  41624   8  57068 delete r4                 36  41716   8  57066 delete r4
 38  41640   8  57066 delete r5                 38  41732   8  57064 delete r5
 40  47204   8  57056 delete r5                 40  45900   8  57056 delete r5
 42  49682  16  57050 delete fp                 42  49888  16  57054 delete r6
 44  54454   8  57048 delete r5                 44  54430   8  57052 delete r5
```

The first row is the only row whose **length** differs by a structural amount:
`5832` vs `5650`, a **182 B** delta. The first throwing call is
`Explosion::Explosion(int)` after `operator new(104)`:

```
ORIG  0x1393d8 movs r0,#104 ; 0x1393da blx _Znwj ; 0x1393e0 blx Explosion::Explosion
OURS  0x139322 movs r0,#104 ; 0x139324 blx _Znwj ; 0x13932c blx Explosion::Explosion
```

The original prologue calls the accessors in the order **messages, player,
enemies**; ours calls **enemies, player, messages**:

```
ORIG  137d3a ldr.w r0,[r4,#24]! ; r4 = &m_pLevel
      137d3e blx Level::getMessages  ; r5 = messages
      137d44 ldr r0,[r4,#0]
      137d46 blx Level::getPlayer
      137d4a strd r4,r0,[sp,#80]     ; spill &m_pLevel, player
      137d4e ldr r0,[r4,#0]
      137d50 blx Level::getEnemies   ; r8 = enemies
      137d56 ldr r0,[pc] ; status
      137d5a ldr r4,[r0]             ; status in r4
OURS  137d3a ldr.w r0,[r4,#24]!
      137d3e blx Level::getEnemies   ; r8 = enemies
      137d44 ldr r0,[r4,#0]
      137d46 blx Level::getPlayer
      137d4a strd r4,r0,[sp,#80]     ; same spill
      137d4e ldr r0,[r4,#0]
      137d50 blx Level::getMessages  ; r4 = messages
      137d56 ldr r0,[pc] ; status
      137d5a ldr.w r9,[r0]           ; status in r9
```

So the original keeps `messages` in `r5` and reuses `r4` for `status`; ours
puts `messages` in `r4` and `status` in `r9`. This is the whole row-0 delta.

## 3. Cleanup-action multiset

The 24 landing pads resolve to the same destructor/delete multiset as w7708:

| action | original | ours |
|---|---:|---:|
| `delete r5` | 9 | 10 |
| `delete r6` | 3 | 3 |
| `delete r4` | 1 | 1 |
| `delete r8` | 1 | 1 |
| `delete [sp+72]` | 1 | 1 |
| `delete fp` | 1 | **0** |
| `String sp+112` | 1 | 1 |
| `String sp+220` | 1 | 1 |
| `String sp+232` | 1 | 1 |
| `String sp+124` | 1 | 1 |
| `String sp+280/368/268/136` cascade | 4 | 4 |

The only action difference is the known register-colouring swap: the original
has a dedicated `delete fp` trampoline for the case-125 state-11
`new AEGeometry(0x37a7, Canvas, false)` marker, while ours deletes the same
object through the shared `delete r6` stub and has one extra shared
`delete r5` stub. The destructor chains and stack slots are otherwise
identical. There is no missing or extra source-level destructor scope.

## 4. New experiments (all compiled with the real project flags, linked, scored)

Baseline: size 57232, process diff bytes 54465, raw 58.8516, process LSDA record diff 291.

| # | variant | size | process diff | raw % | process LSDA diff | note |
|---|---|---:|---:|---:|---:|---|
| 0 | baseline | 57232 | 54465 | 58.85 | 291 | |
| 1 | prologue order `messages, player, enemies` | **57412** | (size mismatch) | 57.50 | 298 | row 0 length becomes 5830/5832, but +180 B downstream |
| 2 | declare-then-assign same order | 57412 | (size mismatch) | 57.50 | 298 | same IR as 1 |
| 3 | `Level *level = m_pLevel;` + same order | 57412 | (size mismatch) | 57.50 | 298 | same IR as 1 |
| 4 | prologue order `messages, enemies, player` | 57412 | (size mismatch) | 57.50 | 298 | `messages` in r5, but player slot sp+72 vs sp+80 |
| 5 | marker via `m_pGeometry6` (no local) | 57240 | (size mismatch) | 58.80 | — | +8 B |
| 6 | marker declared before `if` (`= nullptr`) | 57232 | 54465 | 58.85 | 291 | identical to baseline |
| 7 | marker `m_pGeometry6 = marker` moved after setDirection | 57232 | 54468 | 58.85 | 291 | +3 B worse |
| 8 | marker store moved to end of block | 57232 | 54467 | 58.85 | 291 | +2 B worse |

### 4.1 The three extra `Level::getPlayer` calls

A call-site pair analysis (`prev call, next call`) against the original found
exactly three source-level extra `getPlayer` calls; fixing all three makes the
`getPlayer` count **23/23**:

* `LevelScript.cpp:947` — `m_pLevel->getPlayer()->rotate(0.0f, angle, angle);`
  should be `player->rotate(...)` (original `0x137da6` loads `[sp,#84]`).
* `LevelScript.cpp:1786` — `Vector camPos = m_pLevel->getPlayer()->geometry->getPosition();`
  should be `player->geometry->getPosition()` (preceded by
  `m_pCamera->setTarget(player->geometry)`).
* `LevelScript.cpp:3241` — `if (m_pLevel->getPlayer()->isInRocketControl())`
  should be `if (player->isInRocketControl())` (preceded by
  `resetCamera(m_pLevel)`).

Measured:

| variant | size | process diff | raw % |
|---|---:|---:|---:|
| all 3 getPlayer fixes | **57312** | (size mismatch) | 56.70 |
| all 3 fixes + remove `delta < 0` anchor | 57232 | **55272** | 56.70 |
| line 947 only | 57328 | (size mismatch) | 56.60 |
| line 947 + `delta<0` + auto-turret anchors removed | 57232 | 55270 | 56.70 |
| line 947 + `delta<0` + standing anchors removed | 57232 | 55239 | 56.70 |

The fixes are semantically correct but each one re-rolls the block layout and
**grows** the function (+80 B for all three), so they cannot land without a
faithful -80 B companion. Removing the `delta < 0` anchor happens to be exactly
-80 B and restores size 57232, but the resulting layout is 807 bytes worse than
baseline (55272 vs 54465).

### 4.2 Anchor size accounting

* `delta < 0` first-reference anchor: **-80 B** when removed.
* auto-turret PLT anchor (`player->setAutoTurret(player->autoTurretIsEnabled())`):
  **-16 B**.
* standing PLT anchor (`if (delta == 0x7d) getStanding()->setStanding(0,100)`):
  **-16 B**.
* all three removed: **-112 B** (57232 -> 57120 with the baseline prologue).
* all three removed + prologue reorder: **57300 B** (+68 over target).

No combination of these levers produced a size-exact variant with a smaller
process diff than 54465.

## 5. Conclusion

The `.ARM.extab`/LSDA record is already structurally faithful: same size, same
personality, same ttype, same 46 rows, same 24 landing pads, same cleanup
chains and stack slots. The only action-multiset difference is the `delete fp`
vs `delete r5` register colouring of the case-125 marker. Every remaining
record byte is derived from the `.text` block placement. The row-0 length delta
is the prologue accessor order, and reproducing that order costs +180 B because
it changes the downstream register/stack assignment (`status` r4 vs r9, player
slot sp+84 vs sp+72). This pass found no size-exact source change that improves
the function.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                       # record diff
python3 /dev/shm/w7749/score.py                          # raw score + size
python3 /dev/shm/w7749/callcount.py                      # per-symbol call delta
python3 /dev/shm/w7749/gp_cmp.py                         # getPlayer pair delta
```

Scratch helpers in `/dev/shm/w7749/`: `score.py` (exact verify score),
`callseq.py` (call-sequence diff), `callcount.py` (per-symbol counts),
`gp_cmp.py` (getPlayer prev/next pair analysis), `variant.sh` (build+score
harness), `LevelScript.base.cpp` (pristine baseline).

## Open questions for the next wave

* Find a source shape that keeps the original prologue order
  (`messages, player, enemies`) **and** the original `strd r4,r0,[sp,#80]`
  pairing / `status` in r4 without the +180 B downstream growth. The missing
  piece is the stack-slot/register assignment of `player` (original sp+84,
  reordered variants sp+72).
* The three extra `getPlayer` calls are real semantic bugs (lines 947, 1786,
  3241). They need a faithful -80 B companion that does not destroy the
  block layout; the `delta < 0` anchor is exactly -80 B but is layout-critical.
