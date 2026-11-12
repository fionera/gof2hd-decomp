# MGame::OnTouchEnd — wave 191 (w13532): campaign-100/126 getter order (aligned 1717 -> 1556)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13532`, branch
`wave191b-w13532`, base master `a303f439e`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13532`.

Prior notes: `mgame-ontouchend-wave191-w13497.md` (campaign-15/162 chain move,
126/162 store rotations), `-w13496.md` (chain-order net analysis), `-w13421.md`
(block-100 getter-order aligned win — **code never landed**, only the note),
`-w13447.md` (block-65 getter order), `-w13405.md` (chain order, IR mining loop).

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,885 | **2,885** (neutral) |
| aligned (instruction) diff | 1,717 | **1,556** (-161) |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 71,251 | 71,251 |
| `.ARM.extab` size / diff | 156,312 / 1,216 | 156,312 / 1,216 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| `.rodata` size / diff | 269,159 / 269,644 / 485 | unchanged |
| `.ARM.exidx` diff | 0 | 0 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**.

## Win — campaign-100 getter order HP, Shield, Armor, Gamma

The original block 100 at `0x17b94e` loads the four player stats as
**Hitpoints, ShieldHP, ArmorHP, GammaHP** (`0x17b958`/`17b968`/`17b978`/`17b988`).
The base source had `HP, Armor, Gamma, Shield`.  This is the w13421 aligned win
that was measured (aligned 1,706 -> 1,584) but only committed as a note
(`12c2a6347`); the `src/` change never reached master.  Reordering the four
independent getter/store pairs is behaviour-preserving (pure getters, distinct
`Status` fields):

```cpp
Globals::appManager->SetCurrentApplicationModule(5);
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
```

Function stays exactly 8,080 B, `.rel.plt` byte-identical, `.text`/`.ARM.extab`
content unchanged; aligned 1,717 -> 1,617.

## Win 2 — campaign-126 getter order HP, Shield, Armor, Gamma

On top of win 1, the original 126 block (`0x17bbc0`..`17bc00`) also loads
**Hitpoints, ShieldHP, ArmorHP, GammaHP**; the base had `Armor, Hitpoints,
Shield, Gamma`.  Reordering the four independent getter/store pairs:

```cpp
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
Level::initStreamOutPosition = 0;
Globals::switch_to_target_setting = 1;
```

Size 8,080, `.rel.plt` identical, `.text`/extab unchanged; aligned
1,617 -> 1,556.  The switch/initStream store order is left as landed by w13497
(moving `switch_to_target_setting` back to the front of the block is
raw-hostile: 2,888).

## Negative sweeps on the new base (all reverted)

All size-exact and `.rel.plt`-identical unless noted.

- **Block 162 getter order** (HP, Shield, Armor, Gamma): raw 2,885 (tie),
  aligned 1,556 (tie).  Neutral.
- **Block 81 getter order** (HP, Shield, Armor, Gamma): raw 2,885 (tie),
  aligned 1,556 (tie).  Neutral.
- **Block 65 getter order** (HP, Shield, Armor, Gamma): raw 2,886 (+1),
  aligned 1,549.  Not landed.
- **Block 126 switch-first** (original store position, switch before getters):
  raw 2,888, aligned 1,634.  Both getter orders tested.
- **Block 100 full original shape** (getters, switch, initStream, depart,
  module5 last, active): size **8,048 (-32)**, fatal.
- **Block 100 `active=0` moved before switch/depart**: size 8,088 (+8), fatal.
- **Block 100 `initStreamOutPosition` before departStation**: raw 2,887.
- **Chain order**: re-ran all 153 pair swaps, all 306 single-block moves, and
  120 random 2-3 transposition permutations on top of the 100+126 getter
  reorders.  Best raw tie 2,885 (`15`<->`43`, aligned 1,585); no permutation
  beats 2,885.  The w13497 order remains a strong local minimum.
- **Faithful original chain order** `15,22,43,42,65,81,74,95,96,100,110,120,
  126,134,144,155,161,162`: size **8,076 (-4)** on the current bodies (the
  w13405 +4 has flipped sign as bodies changed), raw 3,946, aligned 1,222.
  Size-fatal, not landed.
- **Mining-report String expression** (`17a5e8` schedule, 22 B diff): named
  `String t("t ")` temp (raw 2,923), split `report +=` statements (size
  8,016-8,064), parenthesised forms (size 8,076) — the single expression is
  the size/raw minimum.
- **Won-block instant-action-first** (oracle shape): size 8,080 exact but
  `.rel.plt` diff 8 (the `LevelScript::skipCutscene` PLT slot rotates to the
  front) and raw 4,469.  Rejected.

## Reproduction

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13532/`): `measure.sh` (build +
raw/aligned/size + section diffs), `try_.py`, `getters.py` (per-block getter
reorder), `chain.py` (chain reorder), `sweep_pairs.py` / `sweep_moves.py` /
`rand_orders.py` (order sweeps), `*.log` / `*.json`.  All builds
`cmake --build cmake-build-match --target link_so -j2` under
`ulimit -v 12000000`.
