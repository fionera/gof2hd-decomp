# MGame::OnTouchEnd — wave 184 (w12816): store-order swaps

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w12816`, branch
`wave184-w12816`, base master `4c06c2662`. Scratch `/home/debian/GalaxyOnFire2/scratch/w12816`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 3,030 | **2,997** |
| aligned (instruction) diff | 1,446 | 1,459 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 74,842 | **74,809** |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.extab` diff | 646 | 646 |
| build-id diff | 20 | 20 |
| total differing bytes | 75,508 | **75,475** |

Gate after the change (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers, file size 2,187,328 exact.

## The change

In the instant-action branch of the `won` mission dispatch, the two independent
stores were reordered:

```cpp
} else {
    this->active = 0;                          // was second
    Globals::switch_to_target_setting = 2;     // was first
    this->applicationManager->SetCurrentApplicationModule(1);
    return;
}
```

`this->active = 0` and `Globals::switch_to_target_setting = 2` are independent
stores, so the swap is behaviour-preserving.  It lets the scheduler emit the
`active` store before the global store, matching the original's store sequence and
removing one differing byte from the region.  Raw 3,030 -> 3,029, size exact.

Second, in the campaign-162 branch, the shield-HP store was moved from directly
after the hitpoints store to after the three independent flag stores:

```cpp
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
Level::initStreamOutPosition = 0;
Globals::switch_to_target_setting = 0;
Globals::status->field_5c = this->player->player->getShieldHP();  // was 2nd
Globals::status->departStation(...);
```

All six stores/getters are independent and `departStation` still follows them, so
the swap is behaviour-preserving.  Raw 3,029 -> 3,028, size exact.

Third, in the campaign-95 branch, the `switch_to_target_setting = 1` store was
moved from the head of the block to after the three HP getter/store pairs (it now
sits just before `Level::initStreamOutPosition = 1;`):

```cpp
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::switch_to_target_setting = 1;   // was first
Globals::status->field_68 = this->player->player->getGammaHP();
Level::initStreamOutPosition = 1;
...
```

Again all stores/getters are independent.  This is the single largest residual win
found: raw 3,028 -> 2,997 (31 B, `.text` 74,840 -> 74,809), size exact.

## Search context (this wave)

- Re-derived the original campaign-chain order from the `cmp #N` immediates:
  `15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162`; source order is
  `74,162,43,15,42,22,65,120,96,100,95,110,81,134,144,126,155,161`.  Reordering the
  source to the original order is size-fatal (8,084 B) and raw 4,276, so not landed.
- `ifirst` dispatch shape (`isInstantActionMission()` tested first, campaign only on
  the false edge) with base chain order: raw 4,455, size exact; with original chain
  order: raw 4,708, size 8,092.  Not landed.
- Full adjacent-transposition sweep of the chain order from the base order: best
  size-exact raw 3,030 (order `74,162,15,43,42,22,65,120,96,100,95,110,81,134,144,126,155,161`,
  aligned 1,417) — same raw, not a win.  Random 250 two-swap search: no better.
  Transposition sweep from the original order: best size-exact raw 3,455.
- Adjacent-statement transposition sweep over the `won`/campaign block found the
  first swap as the only size-exact raw improvement (3,029).  A same-block
  distance-1..4 transposition sweep (brace-depth and intervening-statement checks)
  found the second swap (3,028).  A full-function same-block distance-1..3
  transposition sweep (613 candidates over the whole `OnTouchEnd` body) found the
  third swap (2,997); the same relative move in the campaign-65 block was only
  raw 3,027, so the win is block-specific register allocation.  Other measured swaps:
  block-100 hitpoints/shield order -> 3,031; block-96/127 shield/armor order ->
  3,031; block-65 `initStreamOutPosition`/`FileRead` order -> 3,031;
  `count != 0` -> size-fatal 8,088; `count > 0` -> size-exact raw 3,032.  String
  expression re-associations all worse.

## Residual map (unchanged)

First differing byte remains `count >= 0` emitting `bgt.n` where the original emits
`beq.n` at `+0x48b`, then the 22-byte String-ctor schedule diff at `+0x4a4`, the
literal-pool pointer diff at `+0x6b4`, and the dominant campaign-chain block
order/placement residual from `+0xe3e` (the `soundsMuted` Play block sits at
`0x17b0c2` vs the original `0x17b1ee`).  Tail-merge direction at `0x17bf60` still
differs (original merges into the `isDockingToAsteroid` site).
