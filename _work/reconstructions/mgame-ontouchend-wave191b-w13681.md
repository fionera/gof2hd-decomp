# MGame::OnTouchEnd — wave191b w13681: campaign-65 + back-to-station store orders

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13681`, branch
`wave191b-w13681`, base master `b221b3826`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13681`.

Prior notes: `mgame-ontouchend-wave191b-w13672.md` (arm-81 condition + arm-126 store),
`-w13662.md` (mining-report `count > 0`, arm-126/cinematic), `-w13626.md` (arm-134 store),
`-w13622.md` (diff-cluster map, adjacent-swap sweep).

## Result (landed)

Two complete store-order restorations, both semantically neutral (pure `Status`
field reads and independent global stores), size-exact:

1. **Campaign-65 body** (original `0x17b42c`): the binary emits
   `switch_to_target_setting = 1` first, then `field_64 = getHitpoints()`,
   `field_5c = getShieldHP()`, `field_60 = getArmorHP()`,
   `field_68 = getGammaHP()`, then `Level::initStreamOutPosition = 1`, then
   `new FileRead` / `loadStation(100)` / `departStation` / `delete`.  The base
   source had the getters in `5c,60,68,64` order with `switch` after the
   `FileRead` allocation and `initStream` after `delete` (the raw-optimized
   order left by w13441/w13447).
2. **Back-to-station common tail** (original `0x17b182`, shared by the
   15/22/43/74 arms): `switch_to_target_setting = 0`, then the four getters
   `64,5c,60,68`, then `active = 0`.  The base source had
   `64, active=0, 5c, switch=0, 60, 68` (w13539's armor-before-gamma swap).

```cpp
} else if (Globals::status->getCurrentCampaignMission() == 65 && mission->isCampaignMission()) {
    Globals::switch_to_target_setting = 1;
    Globals::status->field_64 = this->player->player->getHitpoints();
    Globals::status->field_5c = this->player->player->getShieldHP();
    Globals::status->field_60 = this->player->player->getArmorHP();
    Globals::status->field_68 = this->player->player->getGammaHP();
    Level::initStreamOutPosition = 1;
    FileRead *reader = new FileRead();
    Globals::status->departStation(reader->loadStation(100));
    delete reader;
    ...
// Back to the station.
Globals::switch_to_target_setting = 0;
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
this->active = 0;
this->applicationManager->SetCurrentApplicationModule(5);
```

| | raw | aligned | size | insn | `.text` diff | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base | **2,888** | 1,522 | 8,080/8,080 | 2,862/2,865 | 71,247 | 156,312 (577 diff) |
| **landed (a65+tail)** | **2,910** | **1,479** | **8,080/8,080** | 2,862/2,865 | 71,269 | 156,312 (577) |

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370. `.ARM.extab` size/content unchanged,
`.ARM.exidx` 0 diff, `.rodata` 0 diff, `.rel.plt` unchanged.

## Measured candidates (all size 8,080, `.ARM.extab` 156,312 unchanged)

Single faithful store-order restorations, measured on the base chain:

| variant | raw | aligned |
|---|---:|---:|
| base | **2,888** | 1,522 |
| arm-65 faithful (a65) | 2,900 | 1,492 |
| back-to-station tail faithful (tail) | 2,898 | 1,509 |
| arm-110 faithful (a110) | 2,914 | 1,506 |
| arm-95 faithful (a95) | 2,928 | 1,514 |
| arm-162 faithful | 2,889 | 1,553 |
| arm-96 `5c`/`60` swap | 2,889 | 1,522 (tie) |
| arm-161 switch-before-initStream | 2,890 | 1,524 |
| arm-134 faithful `sw,64,5c,60,68` | 2,888 | 1,634 |
| arm-100 faithful (reordered) | 3,621 | 1,535 (size 8,048, fatal) |

Subsets of the four aligned-winning changes (Pareto frontier):

| subset | raw | aligned |
|---|---:|---:|
| a65 | 2,900 | 1,492 |
| a65+tail | **2,910** | **1,479** |
| a65+a110 | 2,926 | 1,476 |
| a65+tail+a110 | 2,936 | 1,463 |
| a65+tail+a110+a95 | 2,976 | **1,453** |

A 127-way subset sweep over `{a65,tail,a110,a95,a162,a96,a161}` found no raw
improvement (minimum raw 2,889, aligned 1,553).  `a65+tail` is the best
aligned-per-raw trade (43 aligned per 22 raw); `a110`/`a95` are faithful but
cost 26/40 raw for 16/10 aligned.

## Campaign-first condition chain (remaining arms)

The original tests `mission->isCampaignMission()` first in every arm.  The
remaining reversed arms (74, 15, 43, 65, 134, 162, combined 96/127) were each
swapped campaign-first on the current base: all are aligned-neutral
(1,521-1,522) at raw +1..+6.  `co_all` = 2,904 / 1,520.  Not landed (no win
beyond the already-landed arm-81 swap).

Removing the `__builtin_expect(mission->isCampaignMission(), 1)` on arm 162
(original `0x17be36` is a plain `cmp; beq.w`) gives 3,743 / 1,309 — a large
aligned win at a large raw cost (the whole post-Play-block region moves).

## Arms 96/127 (structural finding re-confirmed)

The original has two separate campaign tests — `0x17b708` (`==96`, body at
`0x17b722`) and `0x17bc24` (`==127`, `beq.w 0x17b722`, shared body).  Splitting
the source's combined `(cm==96||cm==127) && isCampaignMission()` arm into two
identical-body arms is size-fatal on the current base:

| split variant | raw | aligned | size |
|---|---:|---:|---:|
| faithful chain order, combined 96/127 | 3,964 | 984 | 8,076 (4 B short) |
| faithful chain order, split 96/127 adjacent | 4,726 | 1,297 | 8,112 |
| faithful chain order, split 127 after 126 | 4,722 | 1,292 | 8,112 |
| faithful order + campaign-first, split after 126 | 4,798 | 1,382 | 8,120 |

The faithful chain order (`15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,
144,155,161,162`, confirmed by the false-branch chain) is 4 B short of 8,080;
the split adds 36 B.  Adjacent-swap probes of the faithful order that do hit
8,080 (`96<->100` 3,952/1,043; `110<->120` 3,665/1,091; `144<->155`
3,949/1,023; `155<->161` 3,665/992) are less faithful and cost +777..+1,064
raw, so none was landed.

## Conclusion

The residual is still dominated by MachineBlockPlacement's Play/Stop block
placement (`0xf72..0x156c`, 1,531 B) and its knock-on shift.  This pass lands
the two remaining full store-order restorations (campaign-65 body and the
back-to-station common tail) for aligned **1,522 -> 1,479** at raw +22, size
exact, `.ARM.extab` unchanged.  The faithful chain order would be a much larger
aligned win (984) but is 4 B short of the size-exact requirement.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13681`: `variant.py` (replacement
harness), `build_chain.py` (identity-checked chain parser/rebuilder),
`clusters.py` (same-address diff clusters), `spec_*.json` (experiment batches),
`orig.asm` / `fc.asm` / `a65tail.so`.
