# LevelScript::process — wave 119 / w6797 (adjacent-statement hill-climb, round 3)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6797`, base master
`251d2c254` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.380284         59.471289      (+0.091005)
orig / ours entries      20169 / 20043     20169 / 20042
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
.rel.plt / .rel.dyn      exact             exact (content, not just size)
.ARM.extab size          156308            156308 (unchanged; still 4 B short of 156312)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6797/score.py`, same definition as the wave-118/119 notes).
The master base reads **59.380284** with this helper.

Size-exactness is mandatory: a +-4/8 B change to `process` shifts every later
symbol and drops `byte_exact`. Every landed reorder is size-neutral (57232 B).

## Landed changes

All are statement-level reorders; none adds/removes a call or a symbol, so
`.rel.plt`, `.rel.dyn`, `.dynsym`, `.text` size and all section sizes are
unchanged. Source order below is the final file order.

| # | source lines (base file) | change | faithful? |
|---:|---|---|---|
| 1 | 931/932 | `enableFillScreen(true)` / `nextCampaignMission(true)` / `SetCurrentApplicationModule(2)` rotated so `SetCurrentApplicationModule(2)` comes first | tuned (call order) |
| 2 | 2081 | `objectivesB = nullptr;` moved after `delete objectivesA;` | tuned (independent stores) |
| 3 | 2132 | two `m_pCamera->translate(...)` calls swapped | tuned (call order) |
| 4 | 2396 | `m_pCamera->setActive(true)` moved before `setTarget(enemies[0]->geometry)` | tuned (call order) |
| 5 | 2644 | `m_nState = 9;` moved before `m_nScriptTimerA = 0;` | tuned (independent stores) |
| 6 | 2926 | `m_nState++;` moved before `m_pHud->visible = 0;` | tuned (independent stores) |
| 7 | 3980 | `player->StopEngineSound()` moved before `PlayerStation::setVisible(false)` | tuned (call order) |
| 8 | 4835/4836 | `offset.z` / `offset.x` / `offset.y` rotated to `offset.y`, `offset.z`, `offset.x` | tuned (independent stores) |

The running score after each round:

```
master                                      59.380284
+ (2396)                                    59.395205
+ (932),(2132),(2926),(4836)                59.434995
+ (931),(3980),(2081),(2644)                59.447899
+ (4835)                                    59.471289   <- landed
```

## Method

1. Built and confirmed the master baseline (`RATCHET PASS`, byte 4505).
2. Generated every adjacent pair of same-indent single-line statements in
   `process` (1808 candidates) and scored each variant by compiling only
   `LevelScript.cpp` with the exact `compile_commands.json` flags and gold-linking
   a variant `.so` against the checked-in objects (`/dev/shm/w6797/fastscore.py`).
3. Repeated the sweep from each improved base until no adjacent swap won
   (5 rounds). The first round found the `setActive`/`setTarget` swap; later
   rounds found the additive groups above.
4. Also sampled 4000 random same-indent statement-pair swaps at distance 2..80;
   none beat the final adjacent-swap base.
5. Only size-57232 variants with `.rel.plt`/`.rel.dyn` byte-identical to the
   original were considered landable.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6797/score.py                         # 59.471289
```

`/dev/shm` helpers are scratch only; the landed source is the sole copy of the
change. The `.ARM.extab` size is unchanged by this pass (HEAD and landed both
156308 B, 4 B short of the original 156312 B), and `.rel.plt`/`.rel.dyn` remain
byte-identical.
