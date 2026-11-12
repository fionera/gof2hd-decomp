# MGame::OnTouchEnd — wave 190 (w13441): campaign-65 statement order (raw 2913 -> 2910)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13441`, branch
`wave190-w13441`, base master `915b26b82`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13441`.

Prior notes: `mgame-ontouchend-wave190-w13421.md` (block-100 getter order),
`-w13405.md` (chain order / IR mining loop), `-w13401.md` (getter-order aligned wins),
`-w13392.md` (3-cycles, block field perms), `-w13378.md` (per-block body recon).

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,913 | **2,910** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 72,878 | **72,875** |
| `.ARM.extab` size / diff | 156,312 / 601 | 156,312 / **602** |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers.

Net total (text+extab+build-id) 73,499 -> 73,497: the +1 extab byte is more
than paid for by the -3 `.text` bytes.

## Win — campaign-mission-65 statement order

The base block-65 body was

```cpp
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::switch_to_target_setting = 1;
Globals::status->field_5c = this->player->player->getShieldHP();
FileRead *reader = new FileRead();
Globals::status->field_68 = this->player->player->getGammaHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Level::initStreamOutPosition = 1;
Globals::status->departStation(reader->loadStation(100));
delete reader;
```

Reordering the four getters to the original HP, Shield, Armor, Gamma order and
moving the `FileRead` allocation ahead of the `switch_to_target_setting` /
`initStreamOutPosition` stores:

```cpp
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
FileRead *reader = new FileRead();
Globals::switch_to_target_setting = 1;
Level::initStreamOutPosition = 1;
Globals::status->departStation(reader->loadStation(100));
delete reader;
```

is behaviour-preserving (independent statements, distinct fields / globals).
It is size-exact (8,080), `.rel.plt` byte-identical, and drops raw
**2,913 -> 2,910** (`.text` 72,878 -> 72,875). `.ARM.extab` content grows by
one byte (601 -> 602), so the total is a net -2.

The winning order was found by a small hand-picked sweep of block-65 statement
orders around the base (getter permutations alone are all >= 2,914; the
original-65 order `switch=1` first is 2,915). The key is that the base block-65
body sits at original block 74's address (`0x17b544` vs `0x17b568` body start),
so the `FileRead`/`loadStation(100)` sequence is what aligns, not the original
65 statement order.

## Re-confirmed negative cells (not re-landed)

- Full 24-permutation getter sweep of block 65 (rest fixed): best 2,914.
- Hand-picked block-65 statement orders: `FileRead` after getters 2,914;
  `FileRead` first 3,949; original-65 `switch=1` first 2,915.
- Mining-loop `icmp eq` forms (`count != 0`, `amount != 0`, unsigned variants,
  `String(-amount)`, separate positive counter): every one flips the ARM
  backend's induction variable from the original negative `r9` counter to a
  positive `r8` counter, changing the whole function (raw 6,584+). The base
  `count >= 0` keeps `r9` negative and matches the original counter, but emits
  `bgt.n` instead of `beq.n` at `+0x48b` and hoists `add.w r8,sp,#80` above the
  first `operator+` (22-byte schedule at `+0x4a4`). These are the same cells
  documented by w13405/w13392; no new lever found.
- Block-100 original getter order (module5 first) is raw-neutral (2,913) and
  only changes the instruction-aligned metric; left unlanded here because the
  task asked for raw wins.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`; then the 22-byte String-ctor schedule at `+0x4a4`
and its literal-pool word at `+0x6b4`. The dominant residual remains the
campaign-chain block placement from `+0xe3e`.
