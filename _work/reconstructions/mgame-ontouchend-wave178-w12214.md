# `MGame::OnTouchEnd` — wave 178 (w12214): touch-id/particle statement order, raw 3,267 -> 3,263

Status: **code win landed**, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w12214`, branch `wave178-w12214`, base
`1b48f990f` (wave-177 merge). Target `_ZN5MGame10OnTouchEndEiiPv`, original
8,080 B @ `0x17a144`. Scratch `/home/debian/GalaxyOnFire2/scratch/w12214`.

## Result

| | raw | aligned | size | `.rel.plt` | `.text` diff |
|---|---:|---:|---:|---:|---:|
| base (wave 177) | 3,267 | 1,164 | 8,080/8,080 | 0 | 79,251 |
| **landed** | **3,263** | **1,186** | **8,080/8,080** | **0** | **79,247** |

Gate after the change (full `check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact **843/843**, imports **207**,
lint **CLEAN**, drift **0**, symbol parity **5370/5370**. `.ARM.extab` stays
731 (size/base exact `0x26298` @ `0x1a3b78`), `.rodata` byte-identical,
`.rel.plt` byte-identical (29,392 B).

## The landed change

Inside the `if (this->menuWindow->OnTouchEnd(x, y, touchId) == 1)` block the
statement group

```cpp
this->menuTouchOpen = 0;
this->touch0Id = nullptr;
this->touch1Id = nullptr;
this->activeTouchId = nullptr;
this->level->getStarSystem()->initLight();
float particleQuality = reinterpret_cast<const float &>(Globals::options[40]);
this->level->enableParticleEffects(particleQuality > 0.0f, particleQuality > 0.7f);
```

is reordered to

```cpp
float particleQuality = reinterpret_cast<const float &>(Globals::options[40]);
this->activeTouchId = nullptr;
this->menuTouchOpen = 0;
this->touch0Id = nullptr;
this->touch1Id = nullptr;
this->level->enableParticleEffects(particleQuality > 0.0f, particleQuality > 0.7f);
this->level->getStarSystem()->initLight();
```

All seven statements are mutually independent (plain field stores, a global
`options[40]` read, and two level/star-system calls), so the reorder is
semantically inert. It changes the scheduler's store/call interleaving in the
cold `menuTouchOpen` block: the original binary emits the four field stores
(`menuTouchOpen`, `touch0Id`/`touch1Id`, `activeTouchId`) *before*
`initLight`, while the base build hoisted `getStarSystem()->initLight()` ahead
of them. The new source order reproduces the original's store-before-call
shape closely enough to remove 4 same-address bytes.

## Sweep results (all size 8,080 / `.rel.plt` 0 unless noted)

The whole 7-statement space was sampled (300 + 250 random valid permutations,
plus all 120 single moves/adjacent swaps around the four best 3,263 orders).
The minimum is **3,263**; the next best is 3,264 and the base is 3,267.

| shape | raw | aligned |
|---|---:|---:|
| base (wave 177) | 3,267 | 1,164 |
| initLight first, menuTouchOpen last (p34012) | 3,264 | 1,168 |
| **landed order** | **3,263** | **1,186** |
| best of 300 random perms (seed 12214) | 3,263 | 1,186 |
| best of 250 random perms (seed 12215) | 3,263 | 1,186 |

The 3,263 minimum is a local minimum under all single-statement moves and
adjacent swaps from the four best orders.

## Other levers re-measured on the wave-177 base (no win)

All size 8,080 / `.rel.plt` 0 unless noted.

* **Campaign-first vs mission-first.** Dispatch orders: campaign-first
  (base) 3,267; `campaign, instant, freelance` 3,267 (codegen-identical);
  instant-first 4,759 / size 8,072; freelance-first 4,697 / size 8,088.
  `Mission *mission` declared before `keepObjectives` 3,652.
  `campaign->hasWon() || getMission()->hasWon()` 4,686 / size 8,084.
* **`hasWon` bool.** `== true` removal, `!= false`, direct `if (won)`,
  `bool won = ...` two-step, `keepObjectives` as `&&`: all 3,267
  (codegen-identical). `campaign` pointer hoisted first 3,270.
* **Chain order.** Base order is a local minimum under all 153
  transpositions, all 306 single-block moves, and all 352 three-cycles
  within the first 10 and last 8 positions. Best ties are 3,267 with
  aligned 1,193 (worse); the original combined order is 3,737.
* **Touch-dispatch switch** (`switch (this->cameraMode)`): `case 0` first
  3,727 with `.rel.plt` **1** (not landable); `if (cameraMode == 3) ... else
  if (cameraMode == 0)` and `default:` forms are codegen-identical (3,267).
  The `switch` call sites (`freeCamTouchEnd`/`maneuverTouchEnd`) are already
  at the original addresses.
* **Choice switches.** Black-market `case 1` first 7,477 / size 8,076;
  mining `case 1` first 7,144 / size 8,084; supernova `case 2,0,1` 3,270;
  autopilot `case 1` first 3,764 / `.rel.plt` 1; cinematic-steer group
  reorders 3,267-3,296.
* **Sound arms.** `expect(soundsMuted,0)` 3,267; `expect(soundsMuted,1)`
  4,378 / size 8,096 / `.rel.plt` 1; `expect(!IsCategoryEnabled(2) &&
  !soundsMuted, 0/1)` 3,986 / 4,214 / size 8,084; `play_first` 5,379;
  `resume_first` 3,267; separate-`if` Play forms 3,817-6,037; moving the
  whole sound block to 5 sites 4,298-7,742.
* **`won`/`failed` branch weights.** `expect(won,1)` 4,029 / size 8,072;
  `expect(failed==1,0)` 5,118 / size 8,064; `expect(mission->hasWon(),1)`
  4,628 / size 8,072; `expect(cutsceneActive,1/0)` 6,129 / 5,565.
* **Ore guard.** `count > 0` 3,269; `count != 0` / `amount != 0` 6,307
  (the original's `beq` shape re-lays the loop).
* **Function head.** All 24 permutations of
  `wasAutoPilot`/`flFastForwardWeight`/`setFastForwardMode`/`field_0x84`
  are >= 3,267 (current order best).
* **`pauseOpen`/`pauseSnapshot`/`resumeSounds`** permutations: current
  order best (3,263 on the landed base).

## Remaining residual

The dominant residual is still the cold-block shift described in
`mgame-ontouchend-wave177-w12115.md`: ours emits the `soundsMuted` Play block
at `0x17b0bc` (before the freelance/reward chain), the original at
`0x17b1ee` (after the campaign chain's `== 43` block). That 0x3c shift is
responsible for the bulk of the cold-region diff; the statement reorder above
only recovers 4 bytes of it. The hot-region residuals (the 22-byte `String`
temp scheduling at `0x04a4`, the `count` `bgt`/`beq` opcode at `0x048b`, and
the deliberate campaign/instant dispatch swap at `0x0e3a..0x0e7c`) are
unchanged.

## Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w12214
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 /home/debian/GalaxyOnFire2/scratch/w12214/measure.py   # 3263/1186/8080/0
cmake --build cmake-build-match --target check -j2             # RATCHET PASS
cmake --build cmake-build-match --target drift -j2             # 0 fields
```

Scratch helpers (all under `/home/debian/GalaxyOnFire2/scratch/w12214/`):
`measure.py`/`measurelib.py` (raw/aligned/size/`.rel.plt`), `chainlib.py`
(campaign-chain parse/reorder), `multisweep.py` (batch source-variant
runner), `v_stores_rand.py`/`v_stores_rand2.py` (7-statement permutation
samples), `sweep_stores_local.py` (single-move/adjacent-swap local search),
`MGame.win3263.cpp` (landed source snapshot).
