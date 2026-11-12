# MGame::OnTouchEnd — wave 189 (w13357): escort-block 2-swap raw win

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13357`, branch
`wave189-w13357`, base master `26fbed661`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13357`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,920 | **2,913** |
| aligned (instruction) diff | 1,703 | 1,706 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| instructions | 2,861 / 2,865 | 2,861 / 2,865 |
| `.text` differing bytes | 73,565 | **73,558** |
| `.rel.plt` diff | 0 | **0** (byte-identical) |
| `.ARM.extab` size / diff | 156,312 / 610 | **156,312 / 610** (base `0x1a3b78` exact) |
| `.ARM.exidx` diff | 0 | 0 |
| build-id diff | 20 | 20 |
| total differing bytes (text+extab+build-id) | 74,195 | **74,188** |
| file size | 2,187,328 | **2,187,328** |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint
**CLEAN ✓**, drift **0**, parity **5370/5370**, 27/27 section headers.

## Win — escort mission-type-183 block, two independent statement swaps

The w13318 pass had already moved the escort `Mission` construction ahead of
the escort `Item` `makeItem` (raw 2,921 -> 2,920).  This pass continued the
same block with a pair/3-cycle search over the eight independent statements
(the `return` fixed last).  The winning order is:

```cpp
Mission *escort = new Mission(183, String("Client"), Globals::imageFactory->createChar(true, 0), 0,
                              0, Globals::status->getStation()->getIndex(), 1);
Item *escortItem = (*Globals::items)[216]->makeItem(10);
Globals::status->setMission(escort);
this->active = 0;
Globals::status->getShip()->setEquipment(escortItem, 0);
Globals::status->setFreelanceMission(escort);
this->field_0x1e0 = this->level->killCountB;
Globals::appManager->SetCurrentApplicationModule(2);
return;
```

i.e. vs the landed base order it (a) sinks `field_0x1e0 = level->killCountB`
below the two mission setters, (b) moves `active = 0` above `setEquipment`,
and (c) keeps `setMission` before `setFreelanceMission` (the original's
relative order at `0x120e`/`0x1218`).  All eight statements are independent:

- `Status::setMission` writes `Status::mission`; `Status::setFreelanceMission`
  writes `(*missions)[1]` (`src/game/mission/Status.cpp:202,355`) — disjoint.
- `Ship::setEquipment` writes the ship equipment array; it does not read
  `Status::mission` or `MGame::active`.
- `this->field_0x1e0 = this->level->killCountB` is a pure read of `Level`.
- `SetCurrentApplicationModule(2)` stays last before `return`.

Raw **2,920 -> 2,913** (-7), `.text` 73,565 -> 73,558 (-7), `.ARM.extab`
unchanged at 610, `.rel.plt` byte-identical, function size exactly 8,080.

## Search context (this pass)

- Full-body diff map: the only pre-chain content diffs are the known
  `count >= 0` `bgt.n` byte at `+0x48b`, the 22-byte `String("t ")` address
  schedule at `+0x4a4`, and the literal-pool word at `+0x6b4`.  All other
  early diff bytes are branch-target/literal-pool shifts from the chain.
- Re-tested the mining-block expression schedule with named/nested
  `String` temporaries, `operator=` vs `operator+=`, `String(t)` with a
  named `const char *`, `getText(refined + 1274)`, and the
  `amount <= 0`/`0 >= amount` condition forms.  None beat raw 2,920;
  `amount <= 0` is byte-identical code to `count >= 0` but its LSDA grows
  `.ARM.extab` 610 -> 612, so it is not a win.
- Re-tested the failed-mission block's eight statements (guard placement,
  `m_nTimeLimit`/`setMission`/`setRoute` moves): best raw 2,920, no win.
- Re-tested the common "Back to the station" tail order and moving
  `switch_to_target_setting = 0` into block 15: best size-exact raw 2,928,
  no win.
- Reordered chain blocks 65/81/95/126/155/162 to the original emitted order
  (from the Unity-remake oracle): raw 2,922-2,960, no win.  Block 144 is the
  only chain block whose body is address-aligned with the original
  (`+0x1bcc`); its tail is tail-merged with block 126 in our link, and
  un-merging permutations were all raw-worse (3,238+) or size-fatal.
- The escort block search was the productive one: 76 pair-swap/3-cycle
  variants from the landed base order found raw 2,916; a second hill-climb
  round from that order found raw **2,913**; a third round found no further
  improvement.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`.  The dominant residual remains the campaign-chain
block order/placement from `+0xe3e`.
