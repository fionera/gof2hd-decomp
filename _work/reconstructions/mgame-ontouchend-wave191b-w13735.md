# MGame::OnTouchEnd — wave191b w13735: won-block, tail and pause-close store orders

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original
8,080 B @ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree
`wt/w13735`, branch `wave191b-w13735`, base master `dc50e1082`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w13735`.

Prior notes: `mgame-ontouchend-wave191b-w13692.md` (campaign-chain order),
`-w13681.md` (campaign-65 + back-to-station), `-w13672.md` (arm-81/126),
`-w13662.md` (mining report + cinematic), `-w13626.md` (arm-134).

## Result (landed)

Eight cumulative, semantically-neutral statement-order restorations.  All are
independent stores / pure getters, and every variant was size-checked; only the
wins below were kept.  The function stays exactly 8,080 B and the gate counts
are unchanged.

1. **`finishMission` order** (`0x17b89c`): `removeObjectives`, then
   `setMission(empty)`, `setRoute(nullptr)`, `goingToWaypoint`, `setAutoPilot`,
   `removeRoute`, `setPlayerRoute(nullptr)`.  The base had `setMission`/
   `setRoute` after the `goingToWaypoint` test.
2. **Escort job (type 183) order** (`0x17b2d6`): `field_0x1e0 =
   level->killCountB`, `items[216]->makeItem(10)`, `getShip()->setEquipment`,
   then `new Mission(183, ...)`, `setMission`, `setFreelanceMission`,
   `active = 0`, module 2.  The base constructed the `Mission` before the item.
3. **Cinematic switch bodies** (`0x17b7f4` / `0x17b658`): case 1 is
   `switch_to_target_setting = 0`, `active = 0`, module 5; case 0 is
   `nextCampaignMission(true)`, `setKills(3)`, `switch = 1`, `active = 0`,
   module 2, `initStreamOutPosition = 0`.
4. **Pause-close particle block** (`0x17b5c8`): `menuTouchOpen = 0`,
   `touch0Id = nullptr`, `touch1Id = nullptr`, `activeTouchId = nullptr`,
   `getStarSystem()->initLight()`, then the `particleQuality` load and
   `enableParticleEffects`.  Moving the `particleQuality` *declaration* after
   `initLight()` is what makes this size-exact; moving only the two statements
   is +60 B.
5. **Campaign-100 body** (`0x17b952`): getters `64,5c,60,68`, `switch = 0`,
   `initStreamOutPosition = 0`, `departStation(getStation(120))`, module 5,
   `active = 0`.  The base had module 5 first and `departStation` before
   `initStreamOutPosition`.
6. **Campaign-110 body** (`0x17ba3e`): the compiler emits `switch = 0` before
   `initStreamOutPosition = 0`; the source spelling is the reverse
   (`initStream`, then `switch`).  This is a codegen-order fix, not a literal
   source-order match, and is worth -2 aligned.
7. **Instant-action branch** (`0x17af84`): `switch_to_target_setting = 2`
   before `active = 0`, then module 1.
8. **Pause-close `_bc8` block** (`0x17b52a`): `_bc8 = 0`, `hudMenuOpen = 0`,
   `pauseOpen = 0`, `resumeSounds()`.  The original's literal store order is
   `hudMenuOpen` (0xd6), `_bc8` (0xc8), `pauseOpen` (0x5d), `resumeSounds`;
   the landed spelling is the best size-exact aligned form.

## Measurements

Cumulative variants (all size 8,080/8,080, `.ARM.exidx` and `.rodata` 0 diff):

| variant | raw | aligned | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|---:|---:|
| base | **3,648** | 665 | 72,647 | 564 |
| comb3 (finish/escort/cinematic) | 3,664 | 601 | 72,663 | 563 |
| comb4 (+particle block) | 3,664 | 581 | 72,663 | 563 |
| comb5 (+`activeTouchId`) | 3,664 | 579 | 72,663 | 563 |
| comb6 (+campaign-100) | 3,662 | 563 | 72,661 | 563 |
| comb7 (+campaign-110) | 3,662 | 561 | 72,661 | 563 |
| comb8 (+instant-action) | 3,663 | 557 | 72,662 | 563 |
| comb9 (+`resumeSounds` tail) | 3,664 | 552 | 72,663 | 563 |
| **comb10 (+`_bc8` swap)** | **3,664** | **550** | **72,663** | **563** |

Net: raw **3,648 -> 3,664** (+16), aligned **665 -> 550** (-115),
`.text` **72,647 -> 72,663** (+16), `.ARM.extab` content diff **564 -> 563**
(-1).  `.ARM.exidx` 0, `.rodata` 0, `.rel.plt` unchanged.

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact
4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Rejected candidates (all measured on comb10 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| arm-161 `switch` before `initStream` | 3,665 | 552 | 8,080 |
| arm-162 `switch` before `initStream` | 3,664 | 552 | 8,080 |
| `_bc8` faithful (`hudMenuOpen` first) | 3,664 | 552 | 8,080 |
| arm-95 faithful (`switch` before getters) | 4,253 | 738 | **8,088** (fatal) |
| won-block instant-action-first (whole chain) | 4,733 | 1,146 | **8,092** (fatal) |
| `setFreelanceMission`/`showMissionRewardMessage` swap | 3,665 | 557 | 8,080 (on comb8) |

The residual is still dominated by MachineBlockPlacement's Play/Stop block
placement (`0x17af82..0x17b218`) and the mining-report stack-slot schedule
(`0x17a5e8`).  The mining `count != 0` spelling is size-fatal (+8) because it
flips the induction variable; `count > 0` is the size-exact form (w13662).

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13735`: `measure.sh` (build +
`tools/fn_diff.py` + section diffs), `MGame.cpp.base` and `MGame.cpp.comb3` ..
`MGame.cpp.comb10` (cumulative variants), `clusters*.txt` (aligned diff
clusters), `orig_full.asm` / `ours_full.asm` (Thumb disassembly), `run.log`
(full experiment log).  Builds use `-j2` under `ulimit -v 12000000`.
