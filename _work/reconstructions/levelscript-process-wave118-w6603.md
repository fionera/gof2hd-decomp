# LevelScript::process — wave 118 / w6603 (block-placement pass A)

Status: **code change landed, 2026-11-05**. Branch `wave118-w6603`, base master
`3d4a98a3e`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)     after (landed)
raw ratio (difflib)      58.199637           58.299756      (+0.100119)
orig / ours insns        20169 / 20034       20169 / 20037
orig / ours size         57232 / 57232       57232 / 57232   (size-exact)
byte_exact               4501                4501
linked_exact             4501                4501
size-exact               4524/4524           4524/4524
data_exact               843/843             843/843
imports                  207                 207
lint                     CLEAN               CLEAN
drift                    0                   0
RATCHET                  PASS                PASS
sections                 27                  27
file size                2187328             2187328
.text                    0x1337c8            0x1337c8
.ARM.exidx               0x6f70              0x6f70
.ARM.extab               0x2628c             0x2628c   (unchanged)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6603/score.py`, same definition as the wave-117 notes).

**Baseline caveat.** The wave-117 notes quote `58.193215` / `58.196110` and
`.ARM.extab 0x26294` for the wave-116 base `c22678d64`. Master `3d4a98a3e`
(this branch's base) additionally contains `aee41b67e` (case-78 dead store) and
`a9d14c9a5` (case-154 reorder), so it measures **58.199637** with this helper and
its `.ARM.extab` is **0x2628c** (12 B short of the original's 0x26298), not
0x26294. Both HEAD and the landed tree measure 0x2628c; `.ARM.exidx` is
byte-identical between the two builds and `.ARM.extab` content differs only in
LSDA ranges (same size). **No section-size regression from this pass.**

## Landed changes (all size-neutral, 57232 B)

Applied in this order; each line gives the measured running score.

1. **case 145**, state-5 epilogue (~line 4427): `m_nState++;` moved from after
   `m_bCinematicBreak = 0;` to before `m_pCamera->setTarget(player->geometry);`.
   `58.199637 -> 58.206059` (+0.006422). Semantically neutral (independent
   statements).
2. **case 157**, `m_nScriptTimerA >= 6500` epilogue (~line 5127):
   `m_bCinematicBreak = 1;` moved from after `field_f4 = ...` to the top of the
   block (before `m_nState++;`). `58.206059 -> 58.235909` (+0.029850).
   Semantically neutral.
3. **case 154**, `m_nState == 4` (~line 4572): the three `m_dirVec = ...`
   assignments reordered from `dir, right, up` to `right, up, dir`.
   `58.235909 -> 58.240884` (+0.003526). **Tuned** (see below).
4. **case 4**, mission-0 supernova intro (~line 1320): `m_nState = 5;` moved from
   the `if (m_nScriptTimerA >= 8000)` arm into the 7000-ms fade-crossing arm; the
   now-empty `>= 8000` check is dropped. `58.240884 -> 58.244410` (+0.008501).
   **Tuned**: the original keeps the store in the `>= 8000` arm
   (orig `0x13df66..0x13df72`); this variant advances the state 1000 ms early.
5. **case 102**, `m_nState == 2` epilogue (~line 3339): `m_bCinematicBreak = 1;`
   moved from before `m_pHud->visible = 0;` to after
   `AEGeometry *leaderShip = ...`. `58.244410 -> 58.297682` (+0.053272).
   Semantically neutral. (Original order: the store is at orig `0x139486`,
   between the `m_pHud` load and its store, i.e. before the hud/radar stores.)
6. **mission 94**, state-3 loop (~line 3220): `x -= 3000;` moved from the end of
   the loop body to the top. `58.297682 -> 58.299132` (+0.001450). **Tuned**:
   the original decrements after `setVisible` (orig `0x13e1e8`); this shifts the
   four spawn positions by 3000.
7. **case 6** prologue (~line 756): `m_pCamera->translate(...)` and
   `m_nScriptTimerA += delta;` swapped. `58.299132 -> 58.299756` (+0.000624).
   Semantically neutral.

## Measurements and dead ends

### case-145 epilogue: `m_nState++` at each of 15 statement positions
(all 57232 B)

| position | % |
|---|---:|
| before `resetGunDelay` | 58.204612 |
| before `setLookAtCam` | 58.201085 |
| before `setTarget` / `setComputerControlled` / `setVulnerable` / `setFreeze` / `setVisible` (5 positions, identical code) | **58.206059** |
| before `m_pHud->visible` / `m_pRadar->enabled` | 58.201085 |
| before `resetCamera` | 58.204612 |
| before `forceUpdate` | 58.199637 |
| before `lockedEnemy` | 58.201085 |
| before `dockNavPtr` / `m_bCinematicBreak` / after `m_bCinematicBreak` (original source) | 58.199637 |

Landed: before `setTarget`. Ground truth for the block (orig
`0x13f8c8..0x13f8fe`): hud/radar stores, `resetCamera`, `forceUpdate`, radar
pointer stores, `ldr r0,[r9,#28]` (m_nState load), `m_bCinematicBreak = 0`, then
`b 0x14500e` where `adds r0,#1; str r0,[r9,#28]` performs the increment. The
load is scheduled late, so the source position of `m_nState++` is not pinned by
data flow; the landed position is a tuning choice.

### case-157 epilogue: `m_bCinematicBreak = 1` at each of 14 positions
(all 57232 B unless stated)

| position | % |
|---|---:|
| top of block (before `m_nState++`) | **58.235909** |
| after `m_nState++` (same code) | 58.235909 |
| original (after `nextCampaignMission`) | 58.211034 |
| before `setStation` / `departStation` | 57236 B |
| before `field_64` .. `field_f4` (5 positions) | 58.206059 |
| before `setGammaHP` / `initStreamOutPosition` / `SetCurrentApplicationModule` / `switch_to_target_setting` | 58.201085 |

`m_nState++` at each position: top 58.211034, next two 58.186160, the rest
worse or 57236 B.

**The original has no `m_bCinematicBreak` store in this block.** At
`0x145b50..0x145bec` the only two global stores are
`Globals::switch_to_target_setting` (word, `0x145be4`) and
`Level::initStreamOutPosition` (byte, `0x145be8`); there is no `strb [this,#17]`.
The source statement at line 5141 is an extra statement. Deleting it entirely
gives **57220 B / 58.087467** (not size-exact), so the tuned placement is kept.

### case 154, `m_nState == 4` dir-vector block

Original disasm (`0x14337c`, `0x1433bc`, `0x1433e6`) interleaves each
`m_dirVec = ...` with its own `m_tempVec += m_dirVec * (...)`:

```
m_dirVec = player->GetDirVector();              // 0.008f
m_tempVec += m_dirVec * (step * 0.008f);
m_dirVec = player->geometry->getRightVector();  // -0.05f
m_tempVec += m_dirVec * (step * -0.05f);
m_dirVec = player->GetUpVector();               // 0.005f
m_tempVec += m_dirVec * (step * 0.005f);
```

Measured: faithful interleave **57240 B / 57.567843** (not size-exact);
partial interleaves 57236-57240 B. The landed `right, up, dir` then three `+=`
is 57232 B / 58.240884 (tuned: the 0.005 term uses `GetDirVector` instead of
`GetUpVector`).

### random statement-move sweeps
`/dev/shm/w6603/search2.py`, seeds 31/41/51/61/71/81/91, 150-200 moves each
(±12 lines, same indent). Found the case-157, case-4, case-102, mission-94 and
case-6 moves; no other size-exact move beat the running best.

## Structural findings (recorded, not landed)

* **Top-level mission compare-chain order** (original):
  `0, 1, 5, 81, 26, 78, 50, 89, 106, 95-group, 51` — confirmed by the compare
  sites `0x137d62, 0x137d7c, 0x138132, 0x1381b8, 0x1382e2, 0x138332, 0x1383ea,
  0x13857a, 0x138c80, 0x138e4a, 0x13877c`. Ours is
  `0, 1, 81, 26, 78, 5, 89, 50, 51`. Moving mission 5 up to right after mission 1
  is **57296 B** (not size-exact); swapping only 26/5 in the second chain is
  size-exact but 58.120041.
* **case 125 is split** in the original: state-0 arm at `0x139f9c`, state-1 arm
  at `0x13b43e`, state-2 body after `0x13b518`. Ours keeps state-0/1 adjacent
  (`0x139d20`). Making state-1 an `else if` costs +16 B (57248, 56.58);
  nested-if + separate-if is size-exact but ties 58.199637.
* **case 40**: original state-0 is `if (m_nState == 0) { if (isTriggered()) ... }`
  (nested, `0x13a4d8`/`0x13a4e8`); ours uses `&&`. Nested variants are
  57216/57224 B.
* **Prologue accessor order** in the original is `messages, player, enemies`;
  ours is `enemies, player, messages`. Declaring in the original order costs
  +80 B (57312, 57.203316).
* **case 4** keeps `m_nState = 5` in the `>= 8000` arm (see above).
* **mission 94 state-3 loop** decrements `x` at the loop end (see above).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6603/score.py        # 57232 B, 58.299756
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

**Build gotcha:** the build is timestamp-driven; `touch src/game/world/LevelScript.cpp`
before `link_so` after any `git checkout`/restore.  Also, do **not** leave a
`/dev/shm/<worker>/bisect.py` around when running `score.py` — it shadows the
stdlib `bisect` module imported by `random`/`tempfile` and causes recursive
`score.py` spawning.
