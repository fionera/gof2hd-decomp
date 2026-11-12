# MGame::OnTouchEnd — wave 190 (w13401): block-100 getter order (aligned win)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13401`, branch
`wave190-w13401`, base master `ca19fd252`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13401`.

Prior notes: `mgame-ontouchend-wave190-w13374.md` (finish/escort permutations and
96/127 split exhausted), `mgame-ontouchend-wave189-w13318.md`,
`mgame-ontouchend-wave189-w13323.md`, `mgame-ontouchend-wave189-w13344.md`,
`mgame-ontouchend-wave189-w13357.md`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,913 | **2,913** |
| aligned (instruction) diff | 1,706 | **1,584** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| instructions | 2,861 / 2,865 | 2,861 / 2,865 |
| `.text` differing bytes | 73,003 | 73,003 |
| `.ARM.extab` size / diff | 156,312 / 603 | 156,312 / 603 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers.

## Win — campaign-mission-100 getter order (aligned 1,706 -> 1,584)

The original 100 block at `0x17b94e` loads the four player stats in the order
**HP, Shield, Armor, Gamma**:

```
17b952 ldr r0,[fp,#88] ; player
17b958 blx getHitpoints  -> str [status,#100] (field_64)
17b968 blx getShieldHP   -> str [status,#92]  (field_5c)
17b978 blx getArmorHP    -> str [status,#96]  (field_60)
17b988 blx getGammaHP    -> str [status,#104] (field_68)
```

Our source had `HP, Armor, Gamma, Shield`. Reordering the four independent
getter/store pairs to the original order:

```cpp
Globals::appManager->SetCurrentApplicationModule(5);
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
Globals::switch_to_target_setting = 0;
Globals::status->departStation(...getStation(120));
Level::initStreamOutPosition = 0;
this->active = 0;
```

is behaviour-preserving (pure getters, distinct `Status` fields). It keeps the
function exactly 8,080 B, `.rel.plt` byte-identical, `.text` diff unchanged at
73,003, and drops the instruction-aligned diff **1,706 -> 1,584** (-122). This
is the only size-exact variant of the 48 block-100 permutations (4! getter
orders x module5 first/last) that improves aligned; all `module5 last` variants
are size-fatal (8,048-8,072), and the other 23 `module5 first` getter orders are
aligned 1,706.

## New angles tried (all reverted)

- **instantAction-first won-block restructure** (the original source shape:
  `if (instantAction) {...return;} if (isCampaignMission()) ... else ...`).
  All three spellings (`if/if`, `if/else if`, nested) emit the original
  `cbz r0, campaign-check` preamble but the block layout moves the won block
  from `0x17af5a` to `0x17afa8` and the function grows to **8,084 B** (+4),
  raw 4,706. Not size-exact; the +4 is a layout/branch-width consequence, not
  a local statement size.
- **String-expression association** in the mining block:
  `(a+b)+(c+d)` size 8,072; `a+(b+(c+d))` 8,084; split `+=` 8,056; named
  `String t("t ")` 8,080 raw 2,951; named `a`/`b` temporaries 8,072/8,080
  raw 6,484-6,569. None beat raw 2,913.
- **`count >= 0 && amount != 0`** (and reversed / nested / `&& count != 0` /
  `&& -count != 0`): size-exact, but clang folds it to `cmp r9,#-1; bgt.n`
  (the `count > 0` form), raw 2,915.
- **`__builtin_expect` on `won` / `failed == 1`**: size 7,836-8,088, raw
  4,610-5,414.
- **back-to-station block**: adding explicit `goto backToStation` for missions
  22 and 74 is byte-identical (the compiler already tail-merges the empty 22
  block and the 74 fall-through to `0x17b27c`). The original order
  `sw=0, HP, Shield, Armor, Gamma, active=0` measures raw 2,923; removing the
  `switch_to_target_setting = 0` store is size 8,068 (the store is real:
  `0x17b182-0x17b18a` writes 0 to the global).
- **Exhaustive 2^6 combination sweep** of the size-exact block body reorders
  (65, 81, 126, 161, 162, block-100) plus `count > 0`: no combination beats
  raw 2,913. Best aligned is `block-100` alone at 1,584.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`; then the 22-byte String-ctor schedule at `+0x4a4`
and its literal-pool word at `+0x6b4`. The dominant residual remains the
campaign-chain block placement from `+0xe3e`; the tail-merge direction at
`0x17bf60` still differs (original merges into the `isDockingToAsteroid` site).

## Win 2 — campaign-mission-126 getter/order (aligned 1,584 -> 1,501)

The original emits the same stat-getter order **HP, Shield, Armor, Gamma** in
*every* campaign-return block (verified from the `cmp r0,#N` bodies: 65, 81, 96,
100, 110, 120, 126, 134, 144, 161, 162 all call getHitpoints -> getShieldHP ->
getArmorHP -> getGammaHP). Block 126 had `Armor, HP, Shield, sw=1, Gamma`; the
original is `sw=1, HP, Shield, Armor, Gamma` (`0x17bbc8` `str r1` for
`switch_to_target_setting=1`, then the four getters at `0x17bbd0/0x17bbe0/
0x17bbf0/0x17bc00`). Reordering the source:

```cpp
Globals::switch_to_target_setting = 1;
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
Level::initStreamOutPosition = 0;
```

is behaviour-preserving (pure getters, distinct `Status` fields). All 24 getter
permutations of block 126 measure **raw 2,915** (constant); the aligned metric is
minimised only by the original order (1,501) — every other permutation is 1,582.
Size 8,080 exact, `.text` diff 73,003 -> 73,005, `.ARM.extab` unchanged,
`.rel.plt` byte-identical. Gate `RATCHET PASS` (byte_exact 4507/4524).

Other campaign blocks still emit a non-original getter order (43, 65, 81, 110,
162); fixing 65 alone gives aligned 1,552 / raw 2,916, 110 gives 1,578 / 2,939,
162 gives 1,614 / 2,917 (worse), 81 gives 1,582 / 2,918, 96 gives 1,584 / 2,914.

## Win 3 — campaign-mission-65 statement/getter order (aligned 1,501 -> 1,469)

Block 65's original emission (`0x17b428`) is `sw=1; HP; Shield; Armor; Gamma;
initStreamOut=1; new FileRead; loadStation(100); departStation; delete; active=0;
module(2)`. Our source had `HP; sw=1; Shield; new FileRead; Gamma; Armor;
initStreamOut=1; departStation(loadStation(100)); ...`. Reordering to the original
statement order:

```cpp
Globals::switch_to_target_setting = 1;
Globals::status->field_64 = ...getHitpoints();
Globals::status->field_5c = ...getShieldHP();
Globals::status->field_60 = ...getArmorHP();
Globals::status->field_68 = ...getGammaHP();
Level::initStreamOutPosition = 1;
FileRead *reader = new FileRead();
Globals::status->departStation(reader->loadStation(100));
delete reader;
```

is behaviour-preserving (independent statements, distinct fields). Size 8,080
exact, `.text` diff 73,005 -> 73,008, `.rel.plt` byte-identical. Gate
`RATCHET PASS`. Cumulative after win 2 + win 3: raw 2,918, aligned 1,469.
