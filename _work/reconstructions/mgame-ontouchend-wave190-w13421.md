# MGame::OnTouchEnd — wave 190 (w13421): harvest w13401 block-100 aligned win + cold-block lens

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13421`, branch
`wave190-w13421`, base master `03b9dfe03`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13421`.

Prior notes: `mgame-ontouchend-wave190-w13374.md` (finish/escort permutations, 96/127
split), `-w13378.md` (body reconstruction, chain moves), `-w13392.md` (3-cycles,
field permutations, won shape), `-w13401.md` (getter-order aligned wins — note only,
**code never landed**), `-w13405.md` (original chain order, IR mining loop, expect sweep).

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,913 | **2,913** |
| aligned (instruction) diff | 1,706 | **1,584** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 72,934 | 72,934 |
| `.ARM.extab` size / diff | 156,312 / 603 | 156,312 / 603 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 headers.

## Win — campaign-mission-100 getter order (aligned 1,706 -> 1,584)

The w13401 note measured this cell but only committed the note
(`f0982704f`); the source change was never harvested into master.  The original
100 block at `0x17b94e` loads the four player stats as **HP, Shield, Armor,
Gamma**; the base source had `HP, Armor, Gamma, Shield`.  Reordering the four
independent getter/store pairs to the original order:

```cpp
Globals::appManager->SetCurrentApplicationModule(5);
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
Globals::switch_to_target_setting = 0;
```

is behaviour-preserving (pure getters, distinct `Status` fields).  Function
stays exactly 8,080 B, `.text` diff unchanged at 72,934, `.rel.plt`
byte-identical; the instruction-aligned diff drops 1,706 -> 1,584 (-122).
This is the only size-exact raw-neutral variant among the 48 block-100
permutations (4! getter orders x module5 first/last); `module5 last` variants
are size-fatal (8,048-8,072).

## New lens this pass: cold-block source shape (all reverted)

The residual is dominated by cold-block placement inside the campaign chain.
The `menuTouchOpen` engine-sound block (ours at `0x17b5bc`, between chain
blocks 65 and 120; original at `0x17b5bc`, between 74 and 95) was restructured
to the oracle's nested shape and to a shared-`goto` shape.  Both change the
whole chain layout and are size-fatal:

| variant | raw | size | extab |
|---|---:|---:|---:|
| base (`!enabled && !muted` / `muted` / else) | **2,913** | 8,080 | 603 |
| oracle nested (`if (!enabled) { if (muted) Play else Stop } else if (!muted) Resume else Play`) | 4,037 | 8,120 | 1,394 |
| oracle `goto`-shared Play block | 4,076 | 8,088 | 1,317 |

Also re-measured (all reverted):

- **Original chain order** `15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162`
  on the current base bodies: size **8,084 (+4)**, raw **4,285**, `.text`
  198,592, `.ARM.extab` diff 1,317.  Confirms w13405: the +4 is a
  MachineBlockPlacement tail-merge duplicate, and the faithful order is
  raw-hostile because the base order aligns *similar* bodies to original
  addresses (120->95, 95->110, 144->144, 161->162 exact).
- **instant-action-first won block** (oracle shape, `switch=2; active=0;
  module(1)`): size **8,084 (+4)**, raw **4,706**, `.rel.plt` diff 8.
- **`else if (!mission->isInstantActionMission())` re-test** (no live
  `instantAction` across the campaign call): size 8,080, raw **2,918**,
  aligned 1,255.  Size-exact but +5 raw; not landed.
- **positive-counter mining loop** (`amount++`, `if (amount != 0)`,
  `String(amount)`): size **8,088 (+8)**, raw 6,592 — the same global
  `sl`/`r9` allocation flip w13405 documented for every `!= 0` spelling.

## Search-space note

A random 4-cycle chain-order sweep (300 builds) lands at raw 3,987-4,750:
any 4-block perturbation destroys the base's address alignment.  The base is
a genuine 1/2/3-cycle local minimum, so future passes should not spend time on
larger chain permutations without first reproducing the original cold-block
placement.

## Additional String-schedule probes (all reverted)

The 22-byte schedule at `+0x4a4` (ours hoists `add.w r8, sp, #80` above the
first `operator+` and swaps `mov r0,r8`/`movs r2,#0`) is a register-allocation
choice: ours assigns `r8` to the `String("\n")` temp (`sp+104`), the original
assigns `r8` to the `String("t ")` temp (`sp+80`) and uses direct
`add r0, sp, #104` for the first temp.  New forms measured:

| form | raw | size |
|---|---:|---:|
| base (`report += a + b + String("t ") + c`) | **2,913** | 8,080 |
| `String head = String("\n") + String(count); report += head + String("t ") + c;` | 6,519 | 8,084 |
| `String head = ...; String t("t "); report += head + t + c;` | 6,569 | 8,072 |
| `if (amount != 0)` with body `String(count)` (not `String(-amount)`) | 6,592 | 8,088 |

The last row confirms the `!= 0` allocation flip is independent of whether the
body prints `count` or `-amount`.
