# LevelScript::process — wave 119 / w6828 (case-135 delete order + anchor trim)

Status: **analysis landed, code change reverted on rebase, 2026-11-05**. Branch
`wave119-w6828`, base master `4f36e6260` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.471289         59.492664      (+0.021375)
orig / ours insns        20169 / 20042     20169 / 20041
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.rel.plt / .rel.dyn      exact             exact (byte-identical)
.ARM.extab size          156308            156308 (unchanged; still 4 B short)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6828/score.py`, same definition as the wave-118/119 notes).

## Landed changes

| # | source | change | kind |
|---:|---|---|---|
| 1 | case 135, second revive loop | `cargo = nullptr; delete cargo;` -> `delete cargo; cargo = nullptr;` | faithful (matches original 0x1392bc: load cargo, null-check, `Array<int>::~D2`, then store 0) |
| 2 | case 125 PLT-order anchor | drop `switchSkyboxForIntro()` and `particleEmitBoolPtr->reset()` from the `if (delta < 0)` anchor | tuning (redundant anchor calls; `.rel.plt` verified byte-identical) |

Change 1 is a real reconstruction fix: the old order deleted a null pointer (a
no-op), so the original's `Array<int>::~D2`/`operator delete` pair was missing.
It costs +16 B, which change 2's two removed anchor calls exactly cancel.

The anchor is a reconstruction artifact (guarded on a never-negative `delta`).
Removing individual calls was checked against `.rel.plt`:

```
removed call                      size    score      .rel.plt
none (base)                       57232   59.471289  exact
switchPlanetForIntro              57224   59.170832  exact
switchSkyboxForIntro              57224   59.182252  exact
particleRenderBoolPtr->reset      57224   59.170832  exact
startSmokeEmission                57216   59.171745  NOT exact
particleEmitBoolPtr->reset        57224   59.172304  exact
PlayEngineSound                   57216   59.171745  exact
StopEngineSound                   57216   59.171745  exact
PlayerStation::setVisible         57176   59.146099  NOT exact
```

All 28 pairs were then measured with the case-135 fix applied. The best
`.rel.plt`-exact pair is `switchSkyboxForIntro` + `particleEmitBoolPtr->reset`
(landed): **59.492664**. Other exact pairs: `{0,1}` 59.491184, `{0,4}`
59.491184, `{1,2}` 59.491184, `{2,4}` 59.491184. Triples lose more than they
gain (best `{0,1,4}` = 59.195145 at 57224 B).

## Variants measured (all 57232 B unless stated)

| variant | score | note |
|---|---:|---|
| master base | **59.471289** | |
| case-135 delete order only | 59.185399 | 57248 B (+16) |
| case-135 delete order via temp (`cargo` local) | 59.469810 | size-neutral, tie/loss |
| case-125 state 0/1 `else if` | 57.689247 | 57248 B (+16) |
| case-125 nested-if + `else if` | 57.795131 | 57248 B (+16) |
| case-125 separate `if (state==3)` | 57.549234 | 57272 B (+40) |
| case-40 duplicate `awake()` removed | 59.105606 | 57216 B (-16) |
| prologue decl order `messages,player,enemies` | 58.723086 | 57304 B (+72) |
| prologue decl order `player,enemies,messages` | 59.469810 | size-neutral |
| mission-1 `player->rotate` instead of `getPlayer()` | 55.533439 | 57372 B (+140) |
| anchor removed entirely | 59.167828 | 57152 B (-80), `.rel.plt` differs |
| anchor removed + `messages,player,enemies` | 58.717744 | 57224 B, `.rel.plt` differs |

The prologue call order in the original is `getMessages`, `getPlayer`,
`getEnemies` (0x137d3e/46/50); our source currently emits
`getEnemies`, `getPlayer`, `getMessages`. Reordering to the faithful order
matches the original prologue instruction-for-instruction but changes the
stack-slot assignment (`delta`/`messages` swap at `[sp,#72]`) and grows the
body by 72 B, so it cannot be landed without a compensating reduction.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6828/score.py                         # 59.492664
```

`/dev/shm` helpers are scratch only; the landed source is the sole copy of the
change. `.ARM.extab` remains 156308 B (4 B short of the original 156312 B), and
`.rel.plt`/`.rel.dyn` remain byte-identical.

## Rebase note (current master `fdb68f8ee`)

The branch was based on `4f36e6260`; the harvest target had moved to
`fdb68f8ee`. On that base the same two edits do **not** reproduce 57232 B:

```
base fdb68f8ee (reverted)                57232 B  59.474784
case-135 delete order only               57248 B  59.188900
case-135 delete order + anchor trim      57240 B  59.2      <- gate byte 2740
```

The anchor trim saves only 8 B on this base instead of 16 B, so the body lands
at 57240 B and `byte_exact` collapses to 2740. The src hunk was therefore
dropped; only this analysis is landed. `LevelScript::process` stays at
**57232 B / 59.474784%**. The case-135 delete order is still the faithful shape
(original 0x1392bc) and is the first thing to re-apply once a compensating
16-byte reduction exists on the current base.
