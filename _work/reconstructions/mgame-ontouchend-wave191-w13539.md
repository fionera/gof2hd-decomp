# MGame::OnTouchEnd — wave191b w13539: back-to-station tail store order (aligned 1570 -> 1561)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13539`, branch
`wave191b-w13539`, base master `23a6ebf8b`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13539`.

Prior notes: `mgame-ontouchend-wave191-w13532.md` (campaign-100/126 getter order),
`-w13514.md` (case-0 tail permutation sweep), `-w13497.md` (chain-order net analysis).

## Landed — back-to-station tail: Armor before Gamma

The shared back-to-station tail after the campaign chain (original `0x17bf2a`..) loads
the four player stats in the order Hitpoints, Shield, **Armor, Gamma**.  The base source
had `field_68` (Gamma) before `field_60` (Armor).  Swapping the two independent
getter/store pairs:

```cpp
Globals::status->field_64 = this->player->player->getHitpoints();
this->active = 0;
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::switch_to_target_setting = 0;
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
this->applicationManager->SetCurrentApplicationModule(5);
return;
```

This is behaviour-preserving (pure getters, distinct `Status` fields).  The function
body bytes change but the same-address raw diff is unchanged (the moved store lands on
a byte that already differed); the instruction-aligned diff drops.

| metric | base (23a6ebf8b) | after |
|---|---:|---:|
| size | 8,080 | 8,080 (size-exact) |
| raw same-address diff | 2,882 | **2,882** (tie) |
| aligned (instruction) diff | 1,570 | **1,561** (-9) |
| `.text` diff bytes | 71,487 | 71,487 |
| `.ARM.extab` size / diff | 156,312 / 586 | 156,312 / 586 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| `.ARM.exidx` diff | 0 | 0 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**.

## Swept and reverted this wave

- Full 600-permutation back-to-station tail sweep: only raw-2,882 ties; the landed
  `AR`-before-`GA` order is the best aligned (1,561).  `v_tail2_i1` (plus case-1
  `switch` before `active`) is raw 2,883 / aligned 1,557 — raw regression, not landed.
- `count >= 0` -> `count != 0` / `count > 0` mining-loop condition (original emits
  `cmp r9,#0; beq`, base emits `bgt`): the semantically-correct `!= 0` form makes
  clang 7 materialise `count` in r8 and costs **+8 B** (8,088, size-fatal); the
  `> 0` form is size-exact but encodes `cmp r9,#-1; bgt` (raw 2,884).  Not landed.
- Block-95/110/126/162 single-statement moves and the block-162 125-permutation
  sweep: raw-2,882 ties only.
