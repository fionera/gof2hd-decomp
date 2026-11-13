# MGame::OnTouchEnd — wave191b w13729: eight size-exact store/order wins

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original
8,080 B @ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`).  Worktree `wt/w13729`,
branch `wave191b-w13729`, base master `ae45a4ed6`.  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13729`.

Prior notes: `mgame-ontouchend-wave191b-w13692.md` (faithful campaign chain
order), `-w13681.md`, `-w13672.md`, `-w13662.md`, `-w13626.md`.

Ghidra MCP was unavailable this pass; all evidence is `objdump -d` of the
original at `0x17a144..0x17c0d4` plus `tools/fn_diff.py`.

## Result (landed)

Eight independent, measured, size-exact improvements.  Every one was built and
measured in sequence; each is a pure statement-order change over independent
`Status` fields / pure getters, so behaviour is unchanged.

| step | raw | aligned | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|---:|---:|
| base | 3,648 | 665 | 72,300 | 564 |
| 1. campaign-100 faithful reorder | 3,646 | 649 | 72,298 | 564 |
| 2. campaign-155 `switch` before `initStream` | 3,646 | 647 | 72,298 | 564 |
| 3. `finishMission` reorder | 3,647 | 634 | 72,299 | 564 |
| 4. escort block reorder | 3,660 | 598 | 72,312 | 563 |
| 5. particle/`initLight` order | 3,660 | 576 | 72,312 | 563 |
| 6. cinematic case 1/0 store order | 3,662 | 561 | 72,314 | 563 |
| 7. `_bc8` else-tail reorder | 3,663 | 556 | 72,315 | 563 |
| **8. weapon-slot tail reorder** | **3,660** | **556** | **72,312** | **563** |

Final: raw **3,648 -> 3,660** (+12), aligned **665 -> 556** (-109), size
**8,080/8,080** exact, insn **2,865/2,865**.  `.rel.plt` diff 0, `.ARM.exidx`
diff 0, `.rodata` diff 0.  `.text` grows 72,300 -> 72,312 (+12) while
`.ARM.extab` content diff improves 564 -> 563; byte_exact is unchanged at
4508/4524.  `check` green: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub 0, missing 0, extra 0, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## The eight edits

1. **Campaign 100** (`0x17b952`): getters `64,5c,60,68`, then `switch=0`,
   `initStream=0`, `departStation(getStation(120))`, module 5, `active=0`.
   The base had module 5 first and `departStation` before `initStream`.
2. **Campaign 155**: `switch_to_target_setting = 1;` before
   `Level::initStreamOutPosition = 1;`.
3. **`finishMission` / `!keepObjectives`**:
   `removeObjectives(); setMission(Mission::empty); player->setRoute(nullptr);`
   then the `goingToWaypoint()` / `setAutoPilot(nullptr)` guard, then
   `removeRoute(); level->setPlayerRoute(nullptr);`.
4. **Escort block (mission type 183)**:
   `field_0x1e0 = level->killCountB;` first, then `makeItem(10)`,
   `getShip()->setEquipment`, `new Mission(...)`, `setMission`,
   `setFreelanceMission`, `active=0`, module 2.
5. **Pause-menu-close particle block**: `menuTouchOpen=0; touch0Id=nullptr;
   touch1Id=nullptr; activeTouchId=nullptr; level->getStarSystem()->initLight();`
   then `float particleQuality = ...options[40];` and
   `level->enableParticleEffects(...)`.
6. **Cinematic steer switch** (`0x17b8xx`): case 1 is
   `switch=0; active=0; SetCurrentApplicationModule(5);`; case 0 is
   `nextCampaignMission(true); setKills(3); switch=1; active=0;
   SetCurrentApplicationModule(2); initStream=0;`.
7. **`} else if (this->_bc8)` tail**: `hudMenuOpen=0; _bc8=0; pauseOpen=0;
   resumeSounds(); closeHudMenu();`.
8. **Weapon-slot tail**: `pauseOpen=0;` before `hudMenuOpen=0; resumeSounds();
   closeHudMenu();`.

## Rejected / measured negatives

* Arm-95 faithful (`switch` first): **8,088 (size-fatal)**.  A full
  120-permutation sweep showed every permutation with `switch` before
  `getGamma` is 8,088; all size-exact permutations keep `switch` after armor
  (current order).
* Fully faithful chain order `15,22,43,42,65,81,74,95,96,100,110,120,126,127,
  134,144,155,161,162` with combined 96/127: **8,084 (4 B over)**, even with
  the arm-95 fix.  The original duplicates the 96/127 getter body
  (`0x17b708..0x17b77c`) and shares only the `getStation`/`departStation` tail
  with campaign 110.
* Single-arm relocation sweep from the current order: best size-exact aligned
  **562** (`110` before `100`), worse than 556, so the current order is kept.
* Size-fatal: `won_restructure` (instantAction first) 8,092;
  `supernova_swap` 8,076; `remove_bc8_elseif` 8,040; `bc8_cond_swap` 8,072.
* Aligned-worse: `jumpmap_swap`, `pause_swap`, `starmap_swap`, `bc8_open`,
  `bc8_tail`.

## Remaining clusters

The largest aligned clusters left are reorder artifacts, not missing code:
the 96/127 duplicate getter body (44 insns), the campaign-110 shared tail, and
the merged `getStation`/`departStation` epilogues around `0x17b6ec`/`0x17b77c`.
Closing them requires a size-exact spelling of the duplicated 96/127 body,
which no permutation tried so far achieves (all are 8,084).

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13729`:
`MGame.base.cpp` (landed snapshot), `variant.py` (JSON exact-string replacement
harness), `reorder.py` (else-if arm reorder), `sweep_a95.py`, `sweep_order.py`,
`clusters.py`, `secdiff.py`, `calls.py`, `calldiff.py`, `chain.py`, `arms.py`,
`lit.py`, `tests.py`.  Build:
`(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)`.
