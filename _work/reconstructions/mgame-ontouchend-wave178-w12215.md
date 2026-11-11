# `MGame::OnTouchEnd` — wave 178 (w12215): nested hud-menu dispatch + weapon-slot tail order

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp`
(original 8,080 B @ `0x17a144`). Worktree `/home/debian/GalaxyOnFire2/wt/w12215`,
branch `wave178-w12215`, base `1b48f990f` (wave-177 merge). Scratch
`/home/debian/GalaxyOnFire2/scratch/w12215`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | **3,267** | **3,265** |
| aligned (instruction) diff | 1,164 | **1,163** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| epilogue `popeq` (ours) | `0x17b014` | `0x17b014` |
| `.rel.plt` diff | 0 | 0 |

Gate after the change (full `check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
`.text` differing bytes **79,251 -> 79,249** (-2); `.ARM.exidx` and
`.ARM.extab` (731) and `.rel.plt` unchanged.

## The change

In the `if (this->hudMenuOpen)` touch dispatcher, the three-arm
`else if` chain is turned into nested `if`s, and the weapon-slot tail is
reordered from `closeHudMenu; hudMenuOpen=0; resumeSounds; pauseOpen=0`
(ABCD) to `hudMenuOpen=0; closeHudMenu; resumeSounds; pauseOpen=0` (BACD):

```cpp
        if (weaponSlot >= 0) {
            // Select a secondary weapon.
            Array<Item *> *secondaries = Globals::status->getShip()->getEquipment(1);
            if (secondaries != nullptr) {
                this->level->getPlayer()->setCurrentSecondaryWeaponIndex((*secondaries)[weaponSlot]->getIndex());
                this->hud->setCurrentSecondaryWeapon((*secondaries)[weaponSlot]);
            }
            this->hudMenuOpen = 0;
            this->hud->closeHudMenu();
            resumeSounds();
            this->pauseOpen = 0;
        } else {
            if (wingmanCommand >= 0) {
                // Order the wingmen.
                ...
                this->hudMenuOpen = 0;
                this->hud->closeHudMenu();
                this->pauseOpen = 0;
                resumeSounds();
            }
            if (this->hudTouchFlags == 0) {
                ...
            }
        }
```

The wingman tail keeps the base order (`hudMenuOpen=0; closeHudMenu;
pauseOpen=0; resumeSounds`); permuting it to BACD regresses to 3,276.

## Why it is semantically identical

`weaponSlot` and `wingmanCommand` are both initialised to `-1` and are set by
mutually exclusive arms of the `hudTouchFlags` test chain above. The
`hudTouchFlags == 0` arm is only reachable when *no* flag bit is set, so
`wingmanCommand` is still `-1` there; conversely any `wingmanCommand >= 0`
implies `hudTouchFlags != 0`. Therefore the two independent `if`s are
equivalent to the original `else if` chain, and the outer `weaponSlot >= 0`
guard still short-circuits the rest exactly as before.

## Why it works

The original shares the `closeHudMenu` / `resumeSounds` tail between the
weapon-slot arm and the wingman arm. In the base source the weapon-slot arm
starts with `closeHudMenu`, so its tail block is ordered differently from the
wingman arm and the shared-tail branch lands at `0x17afe8`. Reordering the
weapon-slot tail to BACD makes `closeHudMenu` and `resumeSounds` line up with
the wingman arm; `BranchFolder` then tail-merges the two arms and the branch
becomes `b.w 0x17afe2`, matching the original. The nested `if` form is needed
for the two arms to be considered independently mergeable (it is worth -1 raw
on its own); the two changes together are worth -2 raw / -1 aligned.

## Measured variants (all size 8,080 / `.rel.plt` 0 unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base | 3,267 | 1,164 |
| nested-if only | 3,266 | 1,163 |
| **nested + weaponSlot BACD (landed)** | **3,265** | **1,163** |
| weaponSlot BACD only (no nesting) | 3,266 | 1,163 |
| nested + wingman BACD | 3,276 | 1,165 |
| original campaign chain + nested | 3,697 | 795 |
| `expect(0x2000,0)` | 3,915 | 1,245 |
| `expect(0x20000,0)` | 3,820 | 1,246 |
| `expect(0x100000,0)` | 3,840 | 1,243 |
| `expect(0x1000,0)` | 3,266 | 1,163 |
| `expect(0x20000,1)` | 4,760 | — (size 8,076) |
| `count == 0` / `count != 0` | 6,555 / 6,307 | — |
| `count > -1`, `amount <= 0` | 3,267 | 1,164 |
| bool `Mission::instantAction` + accessor | 4,605-4,794 | — (size 8,084-8,088) |
| `isInstantActionMission() < 1 / <= 0 / == 0` | 3,267 | 1,164 |
| `isInstantActionMission() != 1` | 4,794 | — (size 8,084) |
| `(isInstantActionMission() & 1) == 0` | 4,679 | — (size 8,088) |
| swap cutsceneActive/menuTouchOpen blocks | 5,325 | — (size 8,096, relplt 16) |

## Remaining residuals (not landed)

1. **Campaign-chain block order.** Ours emits the `setFreelanceMission` block
   at `0xfaa` and the Play block at `0xf7e`; the original emits
   `setFreelanceMission` at `0xf72` and Play at `0x10a4`. This is MBP
   `selectBestCandidateBlock` ordering (bb.343 `0x2000` chain head freq
   6.18e-4 placed before bb.214 Play 3.35e-4 / bb.219 setFreelanceMission
   2.72e-4), not raw frequency. w12214's dispatch/order area; not touched here.
2. **Shared `Hud::initHudMenu` tail** for the `0x200`/`0x400` arms is a
   `BranchFolder` layout artifact.
3. **Hot-region string-ctor scheduling** at `0x4a4..0x4ba` (22 B:
   `r8 = sp+80` hoisted before the first `operator+` in ours) is a post-RA
   scheduler artifact.
4. **`.rodata` `"t "` pointer** at `0x6b4` is a TU/rodata layout issue.

## Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w12215
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 /home/debian/GalaxyOnFire2/scratch/w12215/measure.py   # 3265/1163/8080/0
cmake --build cmake-build-match --target check -j2            # RATCHET PASS
```

Scratch helpers: `measure.py`, `dump.py`, `side.py`, `deltas.py`,
`showblocks.py`, `nestlib.py`/`runnest.py`, `perm4.py`/`perm4n.py`/`perm4w.py`,
`tryrep.py`, `MGame.base.cpp`.
