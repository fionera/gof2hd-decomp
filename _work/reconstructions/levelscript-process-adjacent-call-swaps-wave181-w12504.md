# LevelScript::process — wave 181 / w12504: nine adjacent call/field swaps (raw 52,776 -> 52,764)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12504`, branch `wave181-w12504`, base master
`1e270aa0e` (wave-180 harvest).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12504`.

Follow-up to the same-wave relaxed declaration-move and alias-removal passes
(raw 52,795 -> 52,787 -> 52,776).  The wave-180 adjacent-statement sweep
(`scratch/w12504/stmt_pairs.py`) skips pairs where both statements are calls
with the same receiver base and pairs where either is a bare call.  This pass
re-runs the adjacent-statement swap with that restriction removed (1,656 pairs
instead of 1,209) and lands nine additional swaps.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,776           52,764            (-12)
process aligned insns    20,124           20,154            (+30)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,728
.text diffbytes          76,158           76,146            (-12)
.ARM.extab diffbytes     713              713               (size/base exact)
.ARM.exidx diffbytes     0                0
.note.gnu.build-id       20               20
total differing bytes    76,891           76,879            (-12)
.rel.plt                 md5 289b2a06…    md5 289b2a06…     (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`; it is the primary metric.  Size,
`.rel.plt`, `.ARM.extab` and `.ARM.exidx` are unchanged, so the -12 `.text`
bytes are a net win.

## The landed source change

Nine adjacent-statement swaps.  Each pair is two independent calls/field writes
on the same or different objects with no data dependency; every call is a
setter that touches only its own flag/field.

| # | base line | first statement | second statement | raw |
|---:|---:|---|---|---:|
| 1 | 750 | `m_pCamera->setLookAtCam(false);` | `reinterpret_cast<uint8_t &>(field_0x12) = 1;` | 52,775 |
| 2 | 2985 | `((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setDockingType(0);` | `((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setName(String("", false));` | 52,775 |
| 3 | 3119 | `player->setRocketControl(nullptr, nullptr);` | `player->killLiberator();` | 52,775 |
| 4 | 3133 | `m_pCamera->setLookAtCam(true);` | `m_pCamera->setTarget(player->geometry);` | 52,774 |
| 5 | 3397 | `player->setFreeze(true);` | `player->setVisible(false);` | 52,775 |
| 6 | 3835 | `player->setFreeLookMode(false);` | `player->hideShipForFirstPersonCameraView(false);` | 52,775 |
| 7 | 4058 | `m_pCamera->setTarget(player->geometry);` | `m_pCamera->setLookAtCam(false);` | 52,775 |
| 8 | 4389 | `player->stopShooting(0);` | `player->player->setVulnerable(false);` | 52,775 |
| 9 | 4403 | `(*enemies)[0]->setActive(true);` | `m_nScriptTimerA = 0;` | 52,773 |
| all nine | | | | **52,764** |

Each swap was measured individually and the nine are additive.

### Why the swaps are semantics-preserving

- `setLookAtCam` toggles a camera flag; `setTarget` stores the camera target
  geometry; `field_0x12` is an unrelated LevelScript byte.
- `setDockingType` and `setName` write unrelated `PlayerFixedObject` fields.
- `killLiberator()` (PlayerEgo.cpp) reads only `player->guns` and
  `currentSecondaryWeaponIndex`; it never reads the rocket-control pointer
  cleared by `setRocketControl`, so the two calls commute.
- `setFreeze` and `setVisible` set independent flags; `setFreeLookMode` and
  `hideShipForFirstPersonCameraView` likewise.
- `stopShooting` and `setVulnerable` touch independent PlayerEgo state.
- `setActive` on the enemy and the `m_nScriptTimerA` store are unrelated.

## Rejected (read-after-write, not landed)

Two further candidates in the relaxed pair set are **not** landed because the
second statement reads what the first writes; the pair filter's name
normalisation (`m_tempVec.z` -> `z`) misses the dependency:

| base line | pair | why rejected |
|---:|---|---|
| 1239 | `m_tempVec.z += -20000.0f;` / `m_pCamera->setPosition(m_tempVec);` | original stores x,y,z at `0x13be66/6a/6e` **before** `setPosition` at `0x13be72` |
| 2670 | `frame->setPosition(m_tempVec);` / `canvas->CameraSetLocal(..., frame->getMatrix());` | `CameraSetLocal` consumes the matrix updated by `setPosition` (w12403 already rejected this) |

A third candidate, the `m_pCamera->setLookAtCam(true)` /
`m_pCamera->setTarget(...)` swap at line 4397, is size-exact alone (-1) but
interacts with the line-4389 swap: the combined tree grows to 57,240 B.  The
nine-set without it is the best size-exact combination (52,764).

## Method

`run_pairs_relaxed.py` reuses `stmt_pairs.py`'s statement classifier and
data-dependency filter but drops the "same-receiver call pair" and "bare call"
exclusions, producing 1,656 adjacent pairs.  Each candidate is rebuilt with the
exact `compile_commands.json` clang command, gold-linked with the exact
`build.ninja` command, and measured for same-address raw diff, function size and
`.rel.plt` md5.  `combo_pairs.py` then measures the cumulative set and every
leave-one-out subset; the nine-set is the best size-exact combination.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12504
ulimit -v 12000000
ninja -C cmake-build-match libgof2hdaa.so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=52764 aligned=20154 size=57232/57232 insn=20728/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52764 size=57232/57232 text=76146 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses (`run_pairs_relaxed.py`, `combo_pairs.py`, `apply_pairs.py`,
`pairs_relaxed_results.tsv`) are scratch, not tracked.
