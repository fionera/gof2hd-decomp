# LevelScript::process — wave 189 / w13355: distance-3 adjacent-statement sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13355`, branch
`wave189-w13355`, base master `6f5e6a957`.  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13355`.

Task: an independent pass, not duplicating w13352's state-machine dispatch /
register-allocation work.  The wave-189 notes had already swept distance-1 and
distance-2 adjacent swaps (`w13330`), the whole `__builtin_expect` space,
comparison-operand swaps and constant-bearing adjacent pairs (`w13338`), and a
permutation/2-opt/3-opt space at an older base (`w13310`).  This pass runs a
**fresh distance-3 adjacent-statement swap sweep over the entire `process`
body** on the current 51,844 body: 1,252 same-indent triples, exact NDK r18b
`-Oz` compile + gold link, 4 workers.

One size-exact, `.rel.plt`-identical, semantics-preserving raw win landed.

```
                         base (HEAD)      this pass
process raw diffbytes    51,844           51,842     (-2)
process aligned insns    20,992           21,001     (+9, not a gate metric)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          73,939           73,937     (-2)
.ARM.extab diffbytes     614              614        (size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.note.gnu.build-id       20               20
total differing bytes    74,573           74,571     (-2)
file size                2,187,328        2,187,328  (exact)
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

## Landed change

Source lines 1598 / 1601 (mission-0 state-1 first-person hand-off block):

```cpp
-                    Vector pos = player->getPosition();
                     player->setTurretMode(false);
                     Vector zero{0.0f, 0.0f, 0.0f};
+                    Vector pos = player->getPosition();
                     player->setFreeLookMode(false);
```

i.e. `Vector pos = player->getPosition();` moves from before
`setTurretMode(false)` to after the `zero` declaration, and
`setFreeLookMode(false)` moves ahead of the pair.  `pos` is a local copy used
only later by `updateEvent3DAttributes(15, &pos, &zero, false)`; neither
`setTurretMode` nor `setFreeLookMode` mutates the player position, so the swap
is semantics-preserving.  It also matches the original's relative order better:
the original (`0x13a9e2`) samples `getPosition` before the sound update and
calls `setFreeLookMode` at `0x13aa1a`, after `resetCamera`.

Measured alone on the wave-189 base: raw **51,844 -> 51,842** (-2), size
57,232/57,232 exact, `.text` 73,939 -> 73,937, `.ARM.extab` 614 unchanged,
`.rel.plt` md5 unchanged.

## Sweep results (for future passes)

The distance-3 sweep found 17 size-exact, `.rel.plt`-identical raw wins.
Only the landed one is semantics-preserving; the rest are read-after-write or
branch-placement changes and were left alone:

| lines | raw | why not landed |
|---|---:|---|
| 4984/4987 | 51,832 | moves `moveForward` into the `isTriggered` block (semantic) |
| 1276/1279 | 51,840 | `setPosition` before `moveForward` reads a changed position (semantic) |
| **1598/1601** | **51,842** | **landed — independent local copy** |
| 1039/1042 | 51,843 | independent, but `departStation` ordering is mission-significant; left for a focused pass |
| 1490/1493, 1607/1610, 1608/1611, 1942/1945, 2371/2374, 3049/3052, 3267/3270, 3536/3539, 4397/4400, 4524/4527, 4613/4616 | 51,843 | -1 each; independent, candidates for a follow-up |
| 1707/1710 | 51,843 | two stores to `m_nState` — order changes the winner |
| 3024/3027 | 51,843 | two stores to `m_dirVec` — order changes the winner |

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13355
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51842 aligned= 21001 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harness: `fast.py` (single-candidate compile+link+measure) and
`sweep_range.py <lo> <hi> <dist>` (adjacent distance-N swap sweep).

## Follow-up: ten independent -1 pairs composed (raw 51,842 -> 51,832)

The remaining independent (no read-after-write, disjoint receivers) distance-3
pairs from the same sweep were measured one-by-one on the post-51,842 body:
each is a size-exact, `.rel.plt`-identical **-1**.  Composed they are additive:

```
                         after first win   after follow-up
process raw diffbytes    51,842            51,832     (-10)
process aligned insns    21,001            21,046     (+45, not a gate metric)
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
.text diffbytes          73,937            73,927     (-10)
.ARM.extab diffbytes     614               614        (unchanged)
.rel.plt                 md5 289b2a06…     md5 289b2a06…   (byte-identical)
total differing bytes    74,571            74,561     (-10)
RATCHET                  PASS              PASS
```

Landed pairs (line numbers on the 51,842 body; each swap is
semantics-preserving — the two statements have disjoint receivers and nothing
between them reads what the other writes):

| lines | statements swapped |
|---|---|
| 1490/1493 | `lvl->getPlayer()->player->setVulnerable(true)` / `lvl->getPlayer()->setCollide(true)` |
| 1607/1610 | `player->stopShooting(0)` / `player->setSpeed(0.0f)` |
| 1608/1611 | `m_bCinematicBreak = 1` / `m_pHud->visible = 0` |
| 1942/1945 | `m_pCamera->enableFirstPersonCam(false)` / `player->player->setVulnerable(false)` |
| 2371/2374 | `m_nState = 6` / `self->lodManager->forceUpdate(delta, false)` |
| 3049/3052 | `m_nScriptTimerB = ...` / `wcamera->setTarget(leader)` |
| 3267/3270 | `m_pHud->visible = 1` / `m_pLevel->lodManager->forceUpdate(delta, false)` |
| 3536/3539 | `player->player->setVulnerable(false)` / `m_pRadar->enabled = 0` |
| 4397/4400 | `player->setFreeLookMode(false)` / `player->setVisible(false)` |
| 4524/4527 | `player->setFreeLookMode(false)` / `player->setSpeed(0.0f)` |

Deliberately **not** landed (semantic, despite the -1 raw each):

* 1039/1042 — moves `departStation` ahead of `nextCampaignMission`; the
  original (`0x139182..0x1391b4`) is `enableFillScreen, nextCampaignMission,
  switch_to_target_setting=0, departStation`, i.e. the current order.
* 4613/4616 — `setPosition(m_tempVec)` would move ahead of the `m_tempVec`
  assignment it reads.
* 1707/1710 and 3024/3027 — pairs of stores to the same field (`m_nState`,
  `m_dirVec`); order decides the winner.

## Follow-up 2: distance-4 sweep, nine independent pairs (raw 51,832 -> 51,820)

A fresh distance-4 adjacent-statement sweep over the whole `process` body on
the 51,832 body (1,144 same-indent quadruples, exact compile+link, 4 workers)
found one large but semantic-changing pair and a set of independent -1/-2
pairs.  The independent ones compose additively:

```
                         after follow-up 1  after follow-up 2
process raw diffbytes    51,832             51,820     (-12)
process aligned insns    21,046             20,984     (-62, not a gate metric)
orig / ours size         57232 / 57232      57232 / 57232   (size-exact)
.text diffbytes          73,927             73,915     (-12)
.ARM.extab diffbytes     614                614        (unchanged)
.rel.plt                 md5 289b2a06…      md5 289b2a06…   (byte-identical)
total differing bytes    74,561             74,549     (-12)
RATCHET                  PASS               PASS
```

Landed pairs (line numbers on the 51,832 body; disjoint receivers, nothing
between them reads what the other writes):

| lines | statements swapped | alone |
|---|---|---:|
| 3297/3301 | `(*enemies)[i]->setVisible(true)` / `(*enemies)[i]->awake()` | -2 |
| 3877/3881 | `marker->setPosition(...)` / `Vector camPos = *wcamera->getPosition()` | -2 |
| 4524/4528 | `player->setSpeed(0.0f)` / `player->player->setVulnerable(false)` | -2 |
| 1206/1210 | `m_pCamera->setTarget(player->geometry)` / `player->setComputerControlled(false)` | -1 |
| 2259/2263 | `m_pCamera->enableFirstPersonCam(false)` / `m_pCamera->setLookAtCam(true)` | -1 |
| 2389/2393 | `player->hideShipForFirstPersonCameraView(false)` / `resetCamera(m_pLevel)` | -1 |
| 2492/2496 | `player->setFreeLookMode(false)` / `m_pCamera->setActive(true)` | -1 |
| 3134/3138 | `player->setComputerControlled(true)` / `player->setCollide(false)` | -1 |
| 5034/5038 | `fighter->setVisible(false)` / `fighter->setActive(false)` | -1 |

Not landed (semantic, despite the raw win):

* 4984/4988 — **-22**, but it moves `moveForward` into the `isTriggered`
  block and `setActive(false)` into the `expect` block (the original at
  `0x141bdc..0x141c12` has `moveForward` in the `expect` block and
  `setVisible/setActive` in the `isTriggered` block).
* 3459/3463 — two stores to `m_dirVec` (`getDirection` / `getUpVector`);
  order decides the final value.
* 3752/3756 — the two `moveForward(2.5)` / `moveForward(2.3)` constants live in
  different `if` arms; swapping the text moves the constants across arms.
