# MGame::OnTouchEnd — wave191b w13577: case-100 statement order (raw 2882 -> 2880)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13577`, branch
`wave191b-w13577`, base master `01d6d58cc`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13577`.

Prior notes: `mgame-ontouchend-wave191-w13571.md` (pre-chain/block-order),
`-w13540.md` (faithful chain order), `-w13539.md` (back-to-station tail),
`-w13532.md` (100/126 getter order).

## Landed — case 100 statement order

Re-derived the original case-100 body from the disassembly (`0x17b93c`..`0x17ba28`):

```
HP; SH; AR; GA; switch=0; init=0; depart(getStation(120)); module(5); active=0
```

The base body was `module(5); HP; SH; AR; GA; switch=0; depart; init=0; active=0`.
A random permutation search over the nine independent statements found a
size-exact body that lowers the raw same-address diff by 2 bytes:

```cpp
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::appManager->SetCurrentApplicationModule(5);
Globals::status->field_68 = this->player->player->getGammaHP();
Level::initStreamOutPosition = 0;
Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(120));
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::switch_to_target_setting = 0;
this->active = 0;
```

Behaviour-preserving: pure getters, distinct `Status` fields, independent stores.

| metric | base | after |
|---|---:|---:|
| size | 8,080 | 8,080 (size-exact) |
| raw same-address diff | 2,882 | **2,880** (-2) |
| aligned (instruction) diff | 1,561 | 1,661 (+100) |
| `.rel.plt` diff | 0 | 0 (byte-identical) |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**.

## Re-derived original statement order (Ghidra/objdump, GOT-relocation aware)

The original campaign-chain arms test `mission->isCampaignMission()` first, then
`getCurrentCampaignMission()==N`.  Bodies (HP/SH/AR/GA = the four player-stat
getter/store pairs):

- **95** (`0x17b694`): `switch=1; HP; SH; AR; GA; init=1; depart(getStation(10)); active=0; module(2)`
- **96** (`0x17b708`): `switch=1; HP; SH; AR; GA; init=1; depart(getStation(98)); active=0; module(2)`
- **127** (`0x17bc24`): test after 126, branches to the 96 body
- **100** (`0x17b93c`): `HP; SH; AR; GA; switch=0; init=0; depart(getStation(120)); module(5); active=0`
- **110** (`0x17ba28`): `HP; SH; AR; GA; switch=0; init=0; depart(getStation(10)); module(5); enterSpaceLounge=1; active=0`
- **126** (`0x17bbac`): `switch=1; HP; SH; AR; GA; init=0; depart(getStation(120)); active=0; module(2)`
- **155** (`0x17bd84`): `init=1; switch=1; depart(getStation(status->field_84)); active=0; module(2)`
- **case 0** (`0x17b658`): `nextCampaignMission(true); setKills(3); switch=1; active=0; module(2); init=0`
- **case 1** (`0x17b7f4`): `switch=0; active=0; module(5)`

All individually size-exact except case 1 (`8,076`, fatal) and the exact
case-100 order (`8,048`, fatal).  The base bodies remain the raw minimum
individually; the landed case-100 permutation is the only size-exact raw win
found.

## Swept and reverted (all size-exact unless noted)

- Individual exact-original body orders: 95 raw 2,922 / aligned 1,551;
  96 raw 2,883 / aligned 1,561; 110 raw 2,908 / aligned 1,533;
  126 raw 2,885 / aligned 1,539; case 0 raw 2,885 / aligned 1,547;
  case 1 size 8,076 (fatal); case 100 exact size 8,048 (fatal).
- All pairs/triples of those body reorders: best raw 2,886 (96+126, 96+0);
  best aligned 1,497 (110+126+0, raw 2,914).  No raw win.
- Condition-form flips (`getCurrent==N && campaign` -> `campaign && getCurrent==N`)
  for 74/162/15/43/65/96/81/134, individually and in pairs: all raw >= 2,883.
- Random 150-permutation sweeps of case 110/126/95/96/100 bodies: only the
  landed case-100 permutation gave raw < 2,882.
- Faithful original chain order (15,22,43,42,65,81,74,95,96,100,110,120,126,
  134,144,155,161,162) with base bodies: size 8,076 (-4, fatal), raw 3,945,
  aligned 1,205.  With exact bodies: 8,112..8,124 (fatal).  The chain start
  stays shifted by +0x3a because clang places the `PlayEngineSound` cold block
  before the non-campaign block (original places it at `0x17b1e8`, inside the
  chain region), so matching emitted order alone cannot lower the same-address
  raw diff.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13577`: `measure.sh` (build +
`fn_diff`), `harness.py`, `bodies.py`, `conds.py`, `pairs.py`, `triples.py`,
`randperm.py`, `diffmap.py`, `arms.py`, `reorder.py`.
