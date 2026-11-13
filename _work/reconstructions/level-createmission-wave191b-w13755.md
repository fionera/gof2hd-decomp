# Level::createMission — wave 191b w13755

**Coordinator merge note (2026-11-13):** harvested onto `4facb0b48`; the
claimed raw 2,624 -> 2,593 / aligned 858 -> 702 reproduced exactly.


Target: `_ZN5Level13createMissionEv` (`src/game/world/Level.cpp`), size 17,172 B.
Base master `2655cec94` (wave 191b); pristine baseline raw **2,624** / aligned
**858** / size 17,172.

## Landed change: case-2 loot tail store order + shared `data_` load

The binary at `0xb1930` performs the case-2 loot tail with a **single**
`enemies->data_` load into a register, then uses that ship pointer for the
`crateCaptured` and `cargo` stores, before going back through the array for the
`setSpeed` / `setRotationSpeed` / `player` sequence:

```
ldr   r0, [enemies, #data_]     ; one shared data_ load
...
str   [ship, #0x50]             ; cargo = nullptr
strb  [ship, #0x4c]             ; crateCaptured() = 0
bl    KIPlayer::setSpeed(0)
bl    KIPlayer::setRotationSpeed(0)
ldr   player
```

The checked-in source reloaded `(*enemies)[i]` for every statement and stored
`crateCaptured` before `cargo`.  Replacing the block with

```cpp
KIPlayer **ships = this->enemies->data_;
KIPlayer *ship = ships[convoyEnd + j];
((PlayerFighter *) ship)->crateCaptured() = 0;
ship->cargo = nullptr;
ships[convoyEnd + j]->setSpeed(0.0f);
(*this->enemies)[convoyEnd + j]->setRotationSpeed(0.0f);
Player *p = (*this->enemies)[convoyEnd + j]->player;
```

makes `0xb18ac..0xb1930` byte-identical instruction-for-instruction.  `data_`
and `data()` are equivalent; `data()` is used in the landed source.

## Measurement

| variant | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| pristine baseline | 2,624 | 858 | 17,172 | 74,637 | 564 |
| b1: cargo before setSpeed, crateCaptured first | 2,632 | 855 | 17,172 | — | 564 |
| b2: `KIPlayer *ship` local for crate+cargo only | 10,280 | 926 | **17,164** | — | 564 |
| cargo-first, no local (separate reloads) | 9,971 | 862 | 17,180 | — | 564 |
| `KIPlayer *ship` local, crate then cargo | 9,941 | 885 | 17,180 | — | 564 |
| `KIPlayer *&ship` reference local | 9,973 | 863 | 17,180 | — | 564 |
| **`data()` + `KIPlayer *ship`, crate then cargo (landed)** | **2,593** | **702** | **17,172** | **74,606** | **564** |
| same but `data_` | 2,593 | 702 | 17,172 | — | 564 |
| `ship->setSpeed` too | 15,964 | 1,301 | 17,196 | — | 564 |
| `Player *p = ship->player` | 15,962 | 1,307 | 17,196 | — | 564 |

The landed shape is the unique raw minimum found.  `.ARM.extab` is unchanged
(564), `.ARM.exidx` 0, `.rodata` 0.

## Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13755
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2593 aligned=702 size=17172/17172 insn=6206/6197`.

## Remaining residual clusters

After this change the aligned mismatches are concentrated at
`0xb3200-0xb3a00` (raid / case-2), `0xb4100-0xb4200`, and small spots at
`0xb0200`, `0xb1500-0xb1a00`, `0xb2a00`, `0xb3100`.  The next uncovered block
is the raid setup / case-2 `coords`/`goods`/`Route` order around
`0xb16d0-0xb1790`: the binary emits
`movs r4,#0; movw r1,#20000; strd r4,r4,[sp,#272]; ldr r0,[fp]; str r4,[sp,#280]`
before the first `nextInt`, while ours zeroes `coords[3..5]`/`coords[1]` later.
All declaration-order variants tried so far cost raw or are neutral.
