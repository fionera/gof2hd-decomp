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

## Additional negative sweeps (corrected original-body evidence)

Re-derived the original campaign-chain bodies directly from the disassembly
(`0x17b0b6`..`0x17beaa`).  The w13496/w13532 reconstruction of blocks
**95/96/127 was wrong**: the original order is

```
Level::initStreamOutPosition = 1;      // first, before the getters
HP; SH; AR; GA;                        // HP, Shield, Armor, Gamma
Globals::switch_to_target_setting = 1; // after the getters
departStation(getStation(10 / 98));    // 95 -> 10, 96/127 -> 98
this->active = 0;
SetCurrentApplicationModule(2);
```

(95 at `0x17b6a8`, 96 at `0x17b722`; 127 at `0x17bc24` branches to the 96 body.)

All size-exact unless noted, `.rel.plt` identical:

- Corrected 95+96/127 bodies (`init` first, `switch` after getters):
  raw **2,924** (+42), aligned 1,550 (-11).  Not a raw win.
  Individually each is +4 B (size-fatal); together they cancel to 8,080.
- Block 100 exact original order (getters, switch=0, init=0, depart(120),
  module(5), active=0): size **8,048** (-32, fatal).
  Same but `active=0` before module(5): size 8,080, raw **2,987** (+105).
- Back-to-station tail exact original order (switch=0, HP, SH, AR, GA,
  active=0, module(5)): raw **2,892** (+10), aligned 1,548 (-13).
- Case 0: full 24-permutation tail sweep on this base — the landed
  `active, switch, init, module` order is the unique raw-2,882 minimum.
- Case 1: 6 permutations — `module, switch, active` (landed) and
  `module, active, switch` tie at raw 2,882 / aligned 1,561; the other four
  are 2,886 or size-fatal (8,076).
- Block 162: 125-permutation sweep — every permutation gives raw 2,882 /
  aligned 1,561 (fully neutral).
- Split 96 and 127 into two separate `else if` blocks (127 after 126, same
  body): size **8,092** (+12, fatal), raw 3,840, aligned 1,521.
- Random full permutations of the 18 chain blocks: size 8,116-8,188 (fatal),
  raw ~4,800.  The landed order is one of the few that is size-exact.
- Mining-report `String` expression (the 22-byte schedule diff at `+0x4a4`):
  named `String("t ")` temp (raw 2,920), `const String&` to getText
  (size 8,084), `String line = ...` (raw 6,547), `refined + 1274` and
  single-line forms (raw 2,882, unchanged).  The `add r8, sp, #80`
  scheduling difference is not reachable from the expression shape.
