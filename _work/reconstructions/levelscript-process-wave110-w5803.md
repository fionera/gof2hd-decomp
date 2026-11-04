# LevelScript::process — wave 110 (w5803): ten statement-order/block-layout moves

Status: **code change landed, 2026-11-04**. Branch `wave110-w5803`, base master
`390f58052`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (HEAD)     after
official raw ratio       56.086578         57.189063      (+1.102485)
orig / ours insns        20169 / 20026     20169 / 20024
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
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
.text differing bytes    96,517            95,949          (-568)
.ARM.extab               0x26294 (4 B short) 0x26294 (unchanged)
```

The raw number is `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.  Structural deltas are unchanged: 28 sections, file size +44,
`.text`/`.rodata`/`.ARM.exidx` size- and base-exact, `.ARM.extab` 4 B short,
`.rodata`/`.rel.dyn`/`.rel.plt`/`.data` byte-identical.

## What changed (one file, `src/game/world/LevelScript.cpp`)

Ten semantically-neutral statement/declaration reorderings.  Line numbers are
pre-change (HEAD).  Each row is the official linked raw ratio when that single
move is applied to the then-current best base; the "cumulative" column is the
score after landing it.

| # | HEAD line | move | single-move % | cumulative % |
|---|---|---|---:|---:|
| 1 | 736 | swap `player->setTurretMode(false)` / `Globals::sound->play(143,…)` | 56.096529 | 56.096529 |
| 2 | 2932 | swap `m_nScriptTimerB = …` / `AEGeometry *leader = …` | 56.108665 | — |
| 3 | 2964 | swap `field_0x128 = 50000` / `field_0x38 = 4` | 56.088763 | — |
| 4 | 3062 | swap `m_pRadar->enabled = 1` / `resetCamera(m_pLevel)` | 56.087973 | — |
| 5 | 2932+2964+3062 | (combine 2–4 with 1) | — | 56.100109 |
| 6 | 3625 | rotate `m_pRadar->enabled = 1` after `resetCamera`+`forceUpdate` | 57.030550 | 57.030550 |
| 7 | 3635 | rotate `field_0x74 = 1` after `deleteHackingGame`+`m_nState++` | 57.031969 | 57.031969 |
| 8 | 3828 | rotate `m_bCinematicBreak = 1` after `m_pHud`/`m_pRadar` stores + `leaderShip` decl | 57.085282 | 57.085282 |
| 9 | 3238 | rotate `player->setFreeze(true)` after `setLookAtCam`/`chaser`/`stationShip` decls | 57.095233 | 57.095233 |
| 10 | 4061 | rotate `m_nState = 2`/`forceUpdate`/`m_nScriptTimerA = 0` before `setPosition`/`setFixed` | 57.098074 | 57.098074 |
| 11 | 4598 | rotate `m_dirVec = getDirection()` (dead store) to after `setHitpoints(0)` | 57.184087 | 57.184087 |
| 12 | 3251 | rotate `chaser->setVisible(true)` after `forceUpdate` | 57.189063 | 57.189063 |

Moves 1–5 are the wave-109 adjacent swaps re-tested against the *current*
master (wave 109's swaps #1, #3, #4 are now counterproductive: un-swapping
them gains +0.010, +0.002, +0.009 respectively).  Moves 6–12 were found by
direct official-metric parallel sweeps of 3-, 4-, 5- and 6-line rotations over
every window of consecutive simple statements in the function body (see
"Method").  Every move is semantically neutral w.r.t. the HEAD source: the
moved stores/declarations are independent of the statements they cross, and
the `m_dirVec = getDirection()` line moved in #11 was already dead (it is
overwritten by `getRightVector()` before any read).

## Method (official-metric search, not the oracle)

The wave-109 oracle (`_work/reconstructions/w3692_oracle.py`) is ~1.08 points
optimistic on this function and its ranking of adjacent swaps was noisy.  This
pass screened nothing: every candidate was compiled with the exact
`compile_commands.json` command, gold-linked into a private `.so` with the
checked-in link line, and scored with the same `difflib` metric the gate uses.
Scratch harness (not tracked) lives in `/dev/shm/w5803/fast.py`:

* compile one `LevelScript.cpp` copy with the recorded flags,
* substitute its `.o` for the real object in `ninja -t commands link_so`'s gold
  link + strip + `reorder_shstrtab.py` pipeline,
* disassemble `_ZN11LevelScript7processEi` from the resulting `.so` with
  `tools/verify/verify.py`'s resolver and compare to the cached original
  stream.

With 28 workers a candidate costs ~2.7 s wall / ~2.7 s CPU; a full 1686-pair
adjacent sweep takes ~3 min, a 968-window 4-rotation sweep ~6 min.

## Faithful-vs-tuned: the `m_dirVec` block (case 125 state-5 tail, 0x13f4ca)

The original Thumb at `0x13f4ca..0x13f50e` interleaves the two adds:

```
getDirection() -> m_dirVec ; m_tempVec += m_dirVec * 10000
getRightVector() -> m_dirVec ; m_tempVec += m_dirVec * 700
setPosition(m_tempVec) ; setHitpoints(0) ; m_nState = 6
```

HEAD instead groups both getters first (`getDirection` is immediately
overwritten and dead).  The faithful reconstruction is therefore

```cpp
m_dirVec = player->geometry->getDirection();
m_tempVec += m_dirVec * 10000.0f;
m_dirVec = player->geometry->getRightVector();
m_tempVec += m_dirVec * 700.0f;
```

Measured: **56.068070 % at 57232 B** on HEAD (size-exact but −0.019 vs HEAD)
and **57.084567 % at 57232 B** on the landed base.  The landed tuned shape
(dead `getDirection` moved to the end) is **57.189063 %**, i.e. +0.1045 over
the faithful shape.  Both are size-exact; the tuned one is landed and the
faithful one is recorded here as the real content fix to revisit once the
surrounding block layout matches the original.

## Other content findings (measured, not landed)

* Mission-1 state-3 `getStation` argument is `10`, not `100`
  (`0x13b122: movs r1,#10`).  Alone: **55.779908 % at 57160 B** (−72 B,
  size-negative on the gate).
* Removing the mission-125 "first-reference anchor" block (`if (delta < 0)`,
  lines 3588–3597) makes the call counts match the original exactly
  (`Level::getStarSystem` 17→15, `Level::getLandmarks` 17→15,
  `switchPlanetForIntro`/`switchSkyboxForIntro`/`ParticleSystemManager::reset`/
  `StopEngineSound`/`PlayEngineSound`/`startSmokeEmission`/`PlayerStation::setVisible`
  all 2→1) but costs exactly 72 B: **55.949418 % at 57160 B**.  It is the
  natural +72 B compensator for the `getStation(10)` fix (net 57088 B, still
  144 B short of size-exact).
* `Globals::status` register split is unchanged (ours `r9`, original `r4`;
  original reloads `[sp+92]` 45×, ours 24×).  The register-normalized ratio is
  63.31 % before and after, so the +1.10 gain is block placement/scheduling,
  not register colouring.  None of the prologue shapes tested in waves 106/109
  moves `status` to `r4`; the body's higher scratch pressure is the cause.

## Proven vs hypothesis

* **Proven**: the twelve moves above are semantically neutral, size-exact
  (57232 B), keep `.rel.plt` byte-identical, and raise the official raw ratio
  56.086578 → 57.189063.
* **Proven**: the case-body `.text` order already matches the original
  (decoded the two `tbh` tables: ours and the original both lay out
  158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,157,40,102,
  154,24,14,21,42,91,94,105,114,16).
* **Proven**: the `getDirection`/`getRightVector` interleave is a real content
  bug in HEAD, but fixing it faithfully is score-negative on both bases.
* **Hypothesis**: the remaining ~43 % loss is the incomplete case bodies
  (ours is 1400 B shorter than the original by case 16 and makes it up in the
  shared tails) plus the `status` spill difference; both need body-content
  reconstruction, not more statement reordering.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target lint -j6       # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields

python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

## Gate at the end of this pass

```
byte_exact 4500  linked_exact 4500  size-exact 4524/4524  data_exact 843/843
imports 207  lint CLEAN  drift 0  RATCHET PASS
```
