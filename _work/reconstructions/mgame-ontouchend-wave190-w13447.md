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

## Win 2 — campaign-mission-65: `initStreamOutPosition` after `departStation` (raw 2909 -> 2907)

After Win 1 the block-65 body is

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

Swapping the two independent stores/calls

```cpp
Globals::switch_to_target_setting = 1;
Globals::status->departStation(reader->loadStation(100));
Level::initStreamOutPosition = 1;
```

is behaviour-preserving (the store is to a global, the call reads only
`reader`), size-exact (8,080), `.rel.plt` byte-identical, `.ARM.extab` content
unchanged (584), and drops raw **2,909 -> 2,907** (`.text` 72,594 -> 72,592),
a net -2. The instruction-aligned metric rises 1,705 -> 1,778; the raw byte
count is the objective.

This was found by an exhaustive adjacent-transposition sweep of every chain
block body (all 18 blocks, ~150 builds). It is the only sub-2,909 cell in that
sweep; every other block's best adjacent transposition is >= 2,909 (block 162,
127, 100, 110, 81, 134, 126 have raw-neutral ties at 2,909).

Cumulative for this pass: raw **2,910 -> 2,907**, `.text` 72,595 -> 72,592,
`.ARM.extab` 584 unchanged, total (text+extab+build-id) 73,199 -> 73,196.

## Win 3 — campaign-mission-65: `delete reader` before `initStreamOutPosition` (raw 2907 -> 2904)

After Win 2 the block-65 tail is

```cpp
Globals::switch_to_target_setting = 1;
Globals::status->departStation(reader->loadStation(100));
Level::initStreamOutPosition = 1;
delete reader;
```

Swapping the independent `Level::initStreamOutPosition = 1;` store and
`delete reader;`:

```cpp
Globals::status->departStation(reader->loadStation(100));
delete reader;
Level::initStreamOutPosition = 1;
```

is behaviour-preserving, size-exact (8,080), `.rel.plt` byte-identical,
`.ARM.extab` content unchanged (584), and drops raw **2,907 -> 2,904**
(`.text` 72,592 -> 72,589), a net -3 (build-id diff also happened to drop
20 -> 19, total 73,196 -> 73,192).

The block-65 adjacent-transposition sweep was re-run after Win 2; this is the
only sub-2,907 cell. Re-running it again after Win 3 is the natural next step.

Cumulative for this pass: raw **2,910 -> 2,904**, `.text` 72,595 -> 72,589,
`.ARM.extab` 584 unchanged.

## Post-win sweeps exhausted (raw 2904 is a deep local minimum)

After the three landed wins the base is raw **2,904** / aligned 1,704 / size
8,080. Every sweep below was run on this post-win base and reverted; the base
is the best cell in each.

### New exhaustive / targeted cells (all reverted)

| cell | builds | best raw | notes |
|---|---:|---:|---|
| chain block 155, **all 120 permutations** of its first 5 statements | 120 | **2,904** (base) | the only chain block whose full body permutation had not been swept (w13323/w13344 only split 96/127) |
| "Back to the station" block (post-chain, 7 statements), **all 720 permutations** | 720 | **2,904** (base) | first full permutation of this block |
| simulated annealing over the 18-block chain order (swap / move / 3-cycle proposals, T0=200, 300 builds) | 300 | **2,904** (base; SA never beat it) | confirms w13421's "1/2/3-cycle local minimum" |
| block 110 reordered to the original 110 order (HP,Shield,Armor,Gamma,switch,initStream,depart,module,enterSpaceLounge,active) | 1 | 2,930 | size-exact, +26 |
| chain swap 95↔110 (alone) | 1 | 3,200 | size-exact |
| chain swap 95↔110 + block-110 original order | 1 | 3,198 | size-exact |
| block 95 reordered to original-110 order (switch after Gamma) | 48 (24 getter perms × 2 tails) | 3,871 | **size-fatal** (+4 or +32) |
| block 110 `enterSpaceLounge` moved after `SetCurrentApplicationModule` | 1 | 2,927 | size-exact |
| block 110 `enterSpaceLounge` after `active` / last | 2 | 3,669 / 3,564 | +4 / -12 size |
| `finishMission` `if (goingToWaypoint())` moved after `setMission`+`setRoute` (matches the original 110/162 block order at `0x112c`) | 1 | 2,913 | size-exact; local fidelity fix, global raw +9 |

Notes on the last row: the original `finishMission` body emits
`removeObjectives; setMission; setRoute; if(goingToWaypoint) setAutoPilot;
removeRoute; setPlayerRoute` (ORIG `0x112c`), while our source tests
`goingToWaypoint()` before `setMission`/`setRoute`.  Reordering to the original
order is size-exact but the block sits at a different address than ORIG
(ours `0x129a`, ORIG `0x112c`), so the same-address raw count rises 2,904 ->
2,913.  Not landed under the raw-first rule.

### Mining-loop condition / schedule (re-confirmed, no new lever)

The 24-byte pre-chain residual is exactly `0x48b` (`beq` vs `bgt`, 1 B),
`0x4a4..0x4ba` (22-byte `String("t ")` schedule) and `0x6b4` (1 B literal).
New spellings measured on the post-win base:

| form | raw | size |
|---|---:|---:|
| base (`count >= 0`, `String(count)`) | **2,904** | 8,080 |
| `amount != 0` + `String(-amount)` | 6,615 | 8,088 |
| `amount < 0` + `String(-amount)` | 6,553 | 8,084 |
| `amount < 0` + `String(count)` | 2,906 | 8,080 |
| `count != 0` + `String(-amount)` | 6,615 | 8,088 |
| `__builtin_expect(count != 0, 1)` | 7,460 | 8,072 |
| `__builtin_expect(count != 0, 0)` | 7,296 | 8,084 |
| `if (count == 0) {} else { ... }` | 6,615 | 8,088 |
| `if (count == 0) goto skip;` | 6,615 | 8,088 |
| `count > -1` (byte-identical to base) | 2,904 | 8,080 |

Mining-report expression shapes (all keep the same string content):

| form | raw | size |
|---|---:|---:|
| base `a + b + String("t ") + *getText(...)` | **2,904** | 8,080 |
| `a + b + (String("t ") + *getText(...))` | 3,222 | 8,072 |
| `(a + b) + (String("t ") + *getText(...))` | 3,222 | 8,072 |
| `a + (b + String("t ")) + *getText(...)` | 6,394 | 8,076 |
| `a + (b + (String("t ") + *getText(...)))` | 6,515 | 8,084 |

The `beq` and the original `add.w r8, sp, #80` schedule are both produced by
`count != 0`, but that spelling flips the global `sl`/`r9` allocation
(`this`↔`Globals::status`) and costs 8 B / raw 6,615.  No source spelling was
found that decouples the schedule from the branch polarity.

### Residual map (unchanged)

First differing byte is `count >= 0` emitting `bgt.n` where the original emits
`beq.n` at `+0x48b`; then the 22-byte String-ctor schedule at `+0x4a4` and its
literal-pool word at `+0x6b4`.  Everything from `+0xba6` to the end is campaign
chain block placement; `cmp r0,#N` test addresses differ from `0xfd0` onward.
Mnemonic-level comparison shows the instruction *set* is identical (no
instruction-length differences at matching addresses outside the mining
schedule), so the remaining raw delta is branch-displacement / literal-pool
addressing driven by block placement.

### Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13447/`: `perm155.py` (block-155
120-perm), `permback.py` (back-to-station 720-perm), `sa.py` (chain SA),
`b110.py`/`b110b.py`/`b110c.py`/`b110d.py` (block-110/95 reorders),
`b95c.py` (block-95 getter×tail sweep), `cond2.py`/`cond3.py`/`cond4.py`
(condition spellings), `minevar.py` (report-expression shapes), `dd.py`
(range disassembly), `mi.txt` (machine-instr dump).  All builds `ninja -j2`
under `ulimit -v 12000000`.
