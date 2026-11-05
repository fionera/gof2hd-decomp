# LevelScript::process — wave 119 / w6779 (distance-4 statement-order hill-climb)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6779`, base master
`aca284d5c`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so` (helper `/dev/shm/w6779/score.py`). Baseline on this tree reads
**58.498122** (matches the merged wave-119 w6728 note).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.498122         58.738963      (+0.240841)
orig / ours insns        20169 / 20034     20169 / 20036
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
.text                    0x1337c8          0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab (ours/orig)   0x26294 / 0x26298 0x26294 / 0x26298  (pre-existing 4 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67  (pre-existing)
.text differing bytes    89327             88948          (-379)
```

No section size or address changed; `.rel.plt` and `.rel.dyn` stay byte-identical.

## Landed changes

All are size-neutral (57232 B) reorders of statements/declarations.  Line numbers
are in the landed `src/game/world/LevelScript.cpp`.

| # | line | change | note |
|---:|---:|---|---|
| 1 | 1929 | `m_nState = 2` before `setComputerControlled`/`setLookAtCam`/`setVulnerable`; `setTarget` after them | tuned (independent stores/calls) |
| 2 | 2146 | `m_bRenderParticles = 0` and `PlayerStation::setVisible(false)` exchanged between the `currentTime >= 4001` and `m_nScriptTimerA >= 15001` arms | **tuned** (changes which arm does what) |
| 3 | 2586 | `m_pCamera->translate(-delta,delta,-12*delta)` moved into the state-0 arm, `m_nScriptTimerA = 0` into state 1 | **tuned** (cross-arm move) |
| 4 | 2641 | `m_nState = 9` before `m_nScriptTimerA = 0` | faithful/neutral |
| 5 | 2829 | dead `m_dirVec = getDirection()` moved after `setPosition`; `m_dirVec = getRightVector()` after the two `+=` | tuned (the first store is dead) |
| 6 | 2937 | `m_tempVec = leader->getPosition()` after `m_dirVec = leader->getDirection()` | faithful/neutral |
| 7 | 3255 | `m_pCamera->translate(...)` moved into state 0; `m_nScriptTimerA = 0` into state 1 | **tuned** (cross-arm move) |
| 8 | 3304 | `int dy = rnd->nextInt(10000)` before `int dx = rnd->nextInt(10000)+35000`; `KIPlayer *fighter = enemy` after | **tuned** (RNG order changes) |
| 9 | 3688 | `MatrixSetTranslation(local,*acc)` before the last `*acc += ...`; `setFixed(true)` before `setLocal` | **tuned** (drops the up-vector component from the translation) |
| 10 | 3954 | `m_nScriptTimerA = 0` before `m_tempVec += getUpVector()*-500` | tuned |
| 11 | 3974 | `PlayerStation::setVisible(false)` before `StopEngineSound` | tuned (dead `delta < 0` anchor block) |
| 12 | 4356 | `m_nScriptTimerA = 0` before `m_tempVec = leadGun->velocities[0]` | faithful/neutral |
| 13 | 4891 | `lodManager->forceUpdate` before `chaser->setActive(true)` | faithful/neutral |

Running score after each applied change (each measured on top of the previous):
`58.498122 -> 58.506616 -> 58.511591 -> 58.516566 -> 58.520085 -> 58.521540
-> 58.522148 -> 58.618117 -> 58.657912 -> 58.679865 -> 58.684839 -> 58.687758
-> 58.690678 -> 58.692137 -> 58.729014 -> 58.737502 -> 58.738963`.

## Method: distance-4 statement hill-climb

`/dev/shm/w6779/sweep.py` compiles one variant `LevelScript.cpp` with the exact
`compile_commands.json` flags, gold-links it against the checked-in objects
(replacing only the `LevelScript.cpp.obj` input), and scores the resulting `.so`
with `verify.py`'s own disassembler.  96 workers, ~4 s/variant.  Each round:
generate every pair of same-indent simple statements within distance 4 (5487
candidates), keep only size-57232 variants, apply the best, repeat.  Seven
rounds; the last round from the landed tree found **no** size-exact improvement.

## Dead ends measured

| variant | % | size | note |
|---|---:|---:|---|
| mission-0 state 3/4/5 source order `3,4,5` (original switch range 6..15) | 56.379645 | 57248 | fixes the tbh but not size-exact |
| mission-0 order `4,3,5` | 56.282332 | 57248 | same |
| mission-0 order `4,5,3` | 58.218088 | 57232 | size-exact, weaker |
| mission-0 order `5,3,4` / `5,4,3` | 54.016269 | 57208 | not size-exact |
| case-145 split into `if (state==0)`, `if (state==1)`, `switch (state) 2..5`, `if (state==6)` | 57.467718 | 57228 | produces the original `subs #2; cmp #3` tbh, not size-exact |
| case-145 `if (state==0)` + `switch (1..6)` | 58.498122 | 57232 | identical binary (clang re-merges) |
| case-145 drop `case 6` | 57.911754 | 57236 | not size-exact |
| faithful `setFixed(true); MatrixSetTranslation; setLocal` at line 3389 | 58.602592 | 57232 | weaker than the tuned order |
| faithful order at both 3389 and 3692 | 58.551388 | 57232 | weaker |
| prologue M,P,E declaration order | 57.473611 | 57308 | (known) |
| `Level *level = m_pLevel` + M,P,E | 50.817591 | 57072 | (known) |

The best **non-size-exact** variant is the mission-0 state-9/state-10 swap
(`m_nScriptTimerA = 0` <-> `m_nScriptTimerA += delta`, line 227/231): 58.799821
at **57184 B**; it was rejected because a 48 B body change shifts every later
`.text` symbol and collapses `byte_exact` to ~3690.

## Structural findings (recorded, not landed)

* The original mission-0 switch is `subs #6; cmp #9` (states 6..15); ours is
  `subs #4; cmp #11` (states 4..15).  Writing the source chain as `3,4,5` makes
  clang emit the original range, but the body grows to 57248 B and the global
  register allocation degrades (56.38).
* The original case-145 switch is two dispatches: compares for states 0/1, then
  `subs #2; cmp #3; tbh` for states 2..5 (original `0x1393a2` / `0x13cacc`).
  Our single `switch (0..6)` emits `cmp r4,#6; tbh`.  The split source shape is
  faithful but 57228 B.
* The original's `setFixed(true)` precedes `MatrixSetTranslation` in the
  camera-local blocks (e.g. `0x1407b4` then `0x1407be`, `0x13fe02` then
  `0x13fe0c`); our source has it after `setLocal` at two sites.  The faithful
  order scores lower (58.55-58.60), so the tuned order is landed.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6779/score.py cmake-build-match/libgof2hdaa.so
```

## Coordinator merge note (wave 119 harvest)

w6779's base predates w6753's full-body hill-climb (master 59.4%). Its 52-line
diff is superseded by the landed body; note only.
