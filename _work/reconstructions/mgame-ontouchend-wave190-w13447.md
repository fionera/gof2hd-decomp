# MGame::OnTouchEnd — wave 190 (w13447): campaign-65 getter order (raw 2910 -> 2909)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13447`, branch
`wave190-w13447`, base master `8d8f43bcd`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13447`.

Prior notes: `mgame-ontouchend-wave190-w13441.md` (campaign-65 statement order,
raw 2913 -> 2910), `-w13421.md` (block-100 getter order), `-w13405.md` (chain
order / IR mining loop), `-w13401.md` (getter-order aligned wins), `-w13392.md`
(3-cycles, block field perms), `-w13378.md` / `-w13374.md` (body reconstruction).

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,910 | **2,909** |
| aligned (instruction) diff | 1,698 | 1,705 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 72,595 | **72,594** |
| `.ARM.extab` size / diff | 156,312 / 584 | 156,312 / 584 |
| `.note.gnu.build-id` diff | 20 | 20 |
| total (text+extab+build-id) | 73,199 | **73,198** |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers.

## Win — campaign-mission-65 getter order, HP moved last

After the w13441 statement reorder (FileRead before `switch_to_target_setting`
/ `initStreamOutPosition`), the full 24-permutation getter sweep of block 65 was
re-run on the *new* base. w13441's note recorded "best 2,914" only for the
pre-reorder base; the post-reorder base has a different raw minimum.

The winning order is

```cpp
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
Globals::status->field_64 = this->player->player->getHitpoints();
FileRead *reader = new FileRead();
Globals::switch_to_target_setting = 1;
Level::initStreamOutPosition = 1;
Globals::status->departStation(reader->loadStation(100));
delete reader;
```

i.e. move the `field_64 = getHitpoints()` line from first to last. This is
behaviour-preserving (four pure getters into four distinct `Status` fields).
It is size-exact (8,080), `.rel.plt` byte-identical, `.ARM.extab` content
unchanged (584), and drops raw **2,910 -> 2,909** (`.text` 72,595 -> 72,594),
a net -1 total. The instruction-aligned metric rises 1,698 -> 1,705; that is
not a gate metric and the raw byte count is the objective.

Two permutations tie at 2,909: `Shield, Armor, Gamma, HP` (landed) and
`HP, Armor, Gamma, Shield`; both give identical section diffs. All other 22
permutations are >= 2,912.

## Exhaustive getter sweep of the other campaign blocks (all reverted)

Full 24-permutation sweep of every chain block with four stat getters, on the
current base (other statements fixed). Best raw per block:

| block | best raw | best order (field_5c/60/64/68) |
|---|---:|---|
| 162 | 2,910 | several ties (base included) |
| 65  | **2,909** | Shield, Armor, Gamma, HP (landed) |
| 120 | 2,910 | Shield, HP, Armor, Gamma |
| 96/127 | 2,910 | several ties |
| 100 | 2,910 (aligned 1,576) | HP, Shield, Armor, Gamma |
| 95  | 2,910 | HP, Shield, Armor, Gamma |
| 110 | 2,910 | Shield, Armor, Gamma, HP |
| 81  | 2,910 | HP, Shield, Armor, Gamma |
| 134 | 2,910 (aligned 1,586) | HP, Shield, Armor, Gamma |
| 144 | 2,910 | HP, Shield, Armor, Gamma |
| 126 | 2,910 | several ties |
| 161 | 2,910 | HP, Shield, Armor, Gamma |

Only block 65 has a sub-2,910 permutation. (Block 100's `HP,Shield,Armor,Gamma`
order is raw-neutral but drops aligned 1,698 -> 1,576; it is not a raw win and
was left unlanded.)

## Other cells re-measured (all reverted)

- **`won` block store order** (`switch_to_target_setting = 2;` before
  `active = 0;`): 2,911 / 1,694. All six orders of
  {`active=0`, `switch=2`, `module(1)`}: only `active,switch,module` (base) and
  `switch,active,module` are size-exact; the latter is 2,911, the rest are
  8,076.
- **Nested campaign-first spellings** (`if (campaign) ... else { if
  (instantAction) {...return;} show; setFreelance; }`, and the `!campaign`
  mirror): byte-identical to base (2,910).
- **`instantAction` declared inside the `else`** (call after the campaign
  test): 2,915.
- **instant-action-first restructure** (original shape): 4,707 / 8,084 (+4),
  confirming w13405/w13392. The +4 is not a local statement size: the whole
  cold-block layout changes (the `Hud::initHudMenu(2)` block moves from
  `0xe94` to `0xc86`), so it is not a one-instruction compensation.
- **Cross-mapping block bodies to the original block that occupies the same
  address** (e.g. our 95 -> original 110's order, 110 -> 120, 81 -> 126,
  126 -> 155, 161 -> 162): 2,982-3,885, all worse. The base body order is the
  raw optimum at the address each block actually occupies.
- **Block-161 variants** (Gamma/Armor/HP/Shield, switch-first, init+switch
  first): 2,921-3,467, worse.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`; then the 22-byte String-ctor schedule at `+0x4a4`
and its literal-pool word at `+0x6b4`. The dominant residual remains the
campaign-chain block placement from `+0xe3e`.

## Reproduction

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13447/`): `h.py` (patch/build/
measure), `chainlib.py` (chain block parser), `bodyperm.py` (per-block body
permutation), `gettersweep.py` (24-permutation getter sweep), `secdiff.py`
(section diffs), `rangediff.py` / `nonbranch.txt` (residual classification).
All builds `ninja -j2` under `ulimit -v 12000000`.
