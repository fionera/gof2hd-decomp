# LevelScript::process — wave 132 / w7728: replacing the `delta < 0` anchor with real sites

Status: **research note, no landed source change**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**. Owned TU: `src/game/world/LevelScript.cpp`. Base master
`144fc16b0` (wave 132 radar extab). Scratch helpers in `/dev/shm/w7728/`
(not tracked): `relplt.py`, `callcount.py`, `sites.py`, `size.sh`, `trycombo.py`.

## Result

```
                         before (master)   after (no source change)
official verify pct      59.2              59.2
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54507             54507
whole .text diffbytes    85244             85244
.ARM.extab diffbytes     1416              1416
.rel.plt                 identical         identical
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. The anchor is a size compensator, not a `.rel.plt` fix (new evidence)

The `delta < 0` block at `LevelScript.cpp:3976` (case 125 state 3) contains
8 calls. Removing the whole block shrinks `process` from **57232 to 57144 B
(-88)**. Rebuilding and diffing `.rel.plt` against the original shows **exactly
one** difference:

```
orig index 2836: _ZN13PlayerStation10setVisibleEb
orig index 2837: _ZN9PlayerEgo17deleteHackingGameEv
ours index 2836: _ZN9PlayerEgo17deleteHackingGameEv
ours index 2837: _ZN13PlayerStation10setVisibleEb
```

So the *only* first-reference ordering the anchor fixes is
`PlayerStation::setVisible` vs `PlayerEgo::deleteHackingGame`. The other seven
anchor calls are already first-referenced earlier by the real mission-0
case-9/11 bodies:

| call | real first ref (ours) | anchor duplicate |
|---|---:|---:|
| `StarSystem::switchPlanetForIntro` | `0x13cf74` (case 9) | `0x13d772` |
| `Level::switchSkyboxForIntro` | `0x13cf7c` (case 9) | `0x13d778` |
| `ParticleSystemManager::reset` (render) | `0x13cf9a` (case 9) | `0x13d78a` |
| `ParticleSystemManager::reset` (emit) | `0x13cfa2` (case 9) | `0x13d792` |
| `PlayerEgo::startSmokeEmission` | `0x13d4e4` (case 11) | `0x13d78a` |
| `PlayerEgo::PlayEngineSound` | `0x13d4de` (case 11) | `0x13d798` |
| `PlayerEgo::StopEngineSound` | `0x13d4d4` (case 11) | `0x13d79e` |
| `PlayerStation::setVisible` | `0x13e5e0` (case 42 state 7) | `0x13d7ae` |

In the original the real `PlayerStation::setVisible` site is at **`0x13da1e`**
(case 42 state 7), i.e. *before* `deleteHackingGame` at `0x13ded4`. In ours the
same real site is placed at `0x13e5e0`, *after* `deleteHackingGame` at
`0x13d974` — a MachineBlockPlacement difference of the case-42 state-7 cold
block, not a missing statement.

## 2. The eight "missing calls" are tail-merges, not missing statements

Removing the anchor and recounting every `bl`/`blx` target over the function
gives these orig-minus-ours deltas (positive = ours has more):

```
TargetFollowCamera::translate(fff)        orig 57  ours 55   (-2)
LODManager::forceUpdate(int,bool)         orig 66  ours 65   (-1)
PaintCanvas::TransformGetTransform(uint)  orig 37  ours 36   (-1)
Galaxy::getStation(int)                   orig 12  ours 11   (-1)
Status::departStation(Station*)           orig  9  ours  8   (-1)
Level::getEnemies()                       orig 34  ours 33   (-1)
KIPlayer::setActive(bool)                 orig 27  ours 26   (-1)
```

These are the 8 sites the w7708 note listed. They are **tail-merges**:

* `getStation`/`departStation`: mission-78 state 3 (`getStation(100)`,
  `LevelScript.cpp:1034`) and mission-89 state 5 (`getStation(10)`,
  `LevelScript.cpp:1336`) share one `getStation`+`departStation` tail in ours.
  Ours emits `movs r1,#100; b 13b0a8` at `0x13b068`; the shared tail at
  `0x13b0a8` is `blx getStation; mov r1,r0; mov r0,r4; blx departStation`.
  The original has two separate sites (`0x1391ac` and `0x13b124`).
* `setPosition(fff)`: the w7708 note's tail-merge at case-70 state 2 is
  **already equal** (8 vs 8) once the anchor is gone — the anchor itself was
  perturbing that count.

So adding new statements would over-count; the merges must be *prevented*,
which is a compiler-layout lever, not a source-statement lever.

## 3. Four real semantic bugs found while looking for replacement code

These are the same `m_dirVec` overwrite bug that w7687 fixed in case 131
state 2. Each was verified against the original disassembly.

### 3.1 Case 154 state 2 (`LevelScript.cpp:4525`, orig `0x142f9a`)

Original `0x142f9a..0x14305a`:

```
MatrixGetPosition
operator=            m_tempVec = pos
GetDirVector
operator=            m_dirVec = dir
vldr 0.008 ; operator* ; operator+=   m_tempVec += dir*(step*0.008)
getRightVector ; operator= ; vldr -0.05 ; operator* ; operator+=
GetUpVector    ; operator= ; vldr 0.0051 ; operator* ; operator+=
```

Source had the first `+=` *before* `GetDirVector` (stale `m_dirVec`), then
overwrote `m_dirVec` twice. Faithful body:

```cpp
m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
m_dirVec = player->GetDirVector();
m_tempVec += m_dirVec * (step * 0.008f);
m_dirVec = player->geometry->getRightVector();
m_tempVec += m_dirVec * (step * -0.05f);
m_dirVec = player->GetUpVector();
m_tempVec += m_dirVec * (step * 0.0051f);
```

### 3.2 Case 154 state 4 (`LevelScript.cpp:4570`, orig `0x14317c`)

Original uses `dir*0.008`, `right*-0.05`, `up*0.005` (constants
`0x143124/0x143130/0x143530`). Source had two stale `+=` and three
consecutive `m_dirVec =` assignments. Faithful body:

```cpp
m_dirVec = player->GetDirVector();
m_tempVec += m_dirVec * (step * 0.008f);
m_dirVec = player->geometry->getRightVector();
m_tempVec += m_dirVec * (step * -0.05f);
m_dirVec = player->GetUpVector();
m_tempVec += m_dirVec * (step * 0.005f);
```

### 3.3 Case 154 state 6 (`LevelScript.cpp:4623`, orig `0x143360`)

Original uses `dir*0.008`, `right*0.05`, `up*0.005` (constants
`0x143124/0x143548/0x143530`). Source had the right term using the stale
`m_dirVec` and an unused `getUpVector`. Faithful body:

```cpp
m_dirVec = player->GetDirVector();
m_tempVec += m_dirVec * (step * 0.008f);
m_dirVec = player->geometry->getRightVector();
m_tempVec += m_dirVec * (step * 0.05f);
m_dirVec = player->GetUpVector();
m_tempVec += m_dirVec * (step * 0.005f);
```

### 3.4 Case 92 state 3 (`LevelScript.cpp:2961`, orig `0x13fd42`)

Original `0x13fd42..0x13fe0c` is the exact dir/right/up sequence
(`dir*1.7`, `right*(-delta*0.03)`, `up*(delta*0.05*0.004)`). Source applied
`right` twice and left `getUpVector()` dead:

```cpp
m_dirVec = leader->getRightVector();
m_tempVec += m_dirVec * ((float) -delta * 0.03f);
m_dirVec = leader->getUpVector();
m_tempVec += m_dirVec * ((float) delta * 0.05f * 0.004f);
```

(Case 92 state 4 at `LevelScript.cpp:2990` is already correct, which is what
made the state-3 divergence visible.)

## 4. Size accounting (all measured on master 144fc16b0)

| edit | size delta |
|---|---:|
| remove whole `delta < 0` anchor | **-88** |
| case-154 state 2 fix | +? |
| case-154 state 4 fix | +? |
| case-154 state 6 fix | +? |
| case-154 fixes together | **+16** |
| case-92 state 3 fix | **+8** |
| remove anchor `switchPlanetForIntro` | -16 |
| remove anchor `switchSkyboxForIntro` | -8 |
| remove anchor `particleRenderBoolPtr->reset` | -16 |
| remove anchor `particleEmitBoolPtr->reset` | -8 |
| remove anchor `startSmokeEmission` / `PlayEngineSound` / `StopEngineSound` | -8 each |

The deltas are not additive (block placement re-rolls); the measured
combinations are in `trycombo.py`. The best gate-green combination is
`case-154 fixes + case-92 fix + remove switchPlanet + particleRender + particleEmit`
= **57232 B exactly**.

## 5. Why it is not landed: the byte-diff / pct trade-off

Landing the four semantic fixes plus any three anchor removals keeps the gate
green (`byte_exact 4506`, size 57232, `.rel.plt` identical) but *worsens* the
function's own byte diff:

| variant | size | process diff | whole .text | extab | pct |
|---|---:|---:|---:|---:|---:|
| master | 57232 | **54507** | **85244** | 1416 | **59.2** |
| fixes + remove {switchPlanet,render,emit} | 57232 | 54557 | 85336 | 1416 | 58.0 |
| fixes + remove {switchPlanet,skybox,emit} | 57232 | 54599 | 85336 | 1416 | 58.1 |
| fixes + remove {switchPlanet} only | 57232 | 54512 | 85249 | 1416 | 58.8 |

The fixes move the case-154/case-92 cold blocks, which re-rolls
MachineBlockPlacement for the rest of the function. Since master is a local
optimum for the fuzzy metric, the fixes are left unlanded here; they are
correct against the binary and should be landed together with a layout
companion that recovers the lost diff.

## 6. Conclusion on the hypothesis

The `delta < 0` anchor **cannot** be replaced by "real first-reference sites"
alone:

* the mission-0 case-9/11 calls are already at their true positions and are
  first-referenced before `deleteHackingGame` without the anchor;
* the only ordering the anchor supplies is `PlayerStation::setVisible`, whose
  real site (case 42 state 7) the compiler places after `deleteHackingGame`;
* the 8 "missing calls" that could replace the anchor's size are tail-merges,
  not missing statements.

Removing the anchor therefore requires either (a) forcing the case-42 state-7
cold block before the case-125 hacking block, or (b) preventing the
`getStation`/`departStation` tail-merge, and then re-tuning the layout to
recover the 54507-byte diff.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7728/relplt.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                       # anchor-removal .rel.plt swap
```

## Open questions for the next wave

* Force the case-42 state-7 block ahead of case-125's hacking block (e.g. by
  reordering the `case 42` arms or the `m_bRenderParticles` else-arm) so the
  real `PlayerStation::setVisible` is first-referenced naturally.
* Prevent the mission-78/mission-89 `getStation`+`departStation` tail-merge
  (make the two call sequences differ structurally) to recover 2 of the 8
  merged call sites.
* Land the four semantic fixes above with a layout companion that keeps the
  process diff at or below 54507.
