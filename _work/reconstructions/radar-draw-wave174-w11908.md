# Radar::draw wave 174 (w11908) — lock-image friend-first + far-plain else-first

**Status:** complete, 2026-11-11. Worktree `wt/w11908`, branch `wave174-w11908`,
base `master` `be9dd429b`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave173-w11809.md` (planet-lock OR order + frame expect,
raw 7,054 -> 6,849) and `radar-draw-wave173-w11807.md` (image-chain /
exclude-flag / shipGroup re-sweep, no win).

## 0. Baseline (verified on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 6,849 |
| instruction-index alignment | 1,577 |
| instruction count | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical (0) |
| `.ARM.extab` differing bytes | 675 (size/base exact) |
| `.text` differing bytes | 80,897 |
| total differing bytes | 81,592 |
| gate | RATCHET PASS (byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, drift 0) |

## 1. Landed (one commit, two sites)

Both sites are in the same enemy/asteroid tail and are **size-exact**; the
combined edit is 9,908 B and leaves `.rel.plt` byte-identical.

| # | site | change | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | lock-image chain (lines 754-761) | friend test before enemy test | 6,849 -> **6,847** | 1,577 -> 1,581 | 9,908 |
| 2 | off-screen "far plain" image chain (lines 777-783) | else-branch first, friend second, enemy last | 6,847 -> **6,833** | 1,581 -> 1,611 | 9,908 |

Combined: raw **6,849 -> 6,833** (-16), aligned 1,577 -> 1,611, size
9,908/9,908. Whole-`.so` `.text` differing bytes 80,897 -> **80,881**
(-16); total differing bytes 81,592 -> **81,576** (-16). `.ARM.extab`
stays 675, `.rel.plt` stays byte-identical (0), `.ARM.exidx` stays 0.
`check` green after the commit (byte 4,507 / linked 4,507 / size 4,524 /
data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370).

The edit changes 878 function bytes in the range fn+0xbda..fn+0x135c
(instruction displacements and register/slot shuffles move around); of
those, 57 bytes become equal to the original and 41 become different, for
the net -16. No `.text` bytes outside the function change.

### 1.1 Lock-image chain (on-screen, `enemy == locked`)

Base (enemy-first):

```cpp
unsigned int lockImage;
if (enemy->player->enemyFlagsLo) {
    lockImage = this->image_0x5c;
} else if (enemy->player->carriesFriendCargoFlag) {
    lockImage = this->image_0x64;
} else {
    lockImage = this->image_0x74;
}
```

Landed (friend-first):

```cpp
unsigned int lockImage;
if (enemy->player->carriesFriendCargoFlag) {
    lockImage = this->image_0x64;
} else if (enemy->player->enemyFlagsLo) {
    lockImage = this->image_0x5c;
} else {
    lockImage = this->image_0x74;
}
```

The original emits the friend test first, then the enemy test, then the
plain lock image. The two predicates are mutually exclusive in the
original data model, so the reorder is semantics-preserving:

* `Standing::isEnemy` / `Standing::isFriend` return disjoint results for
  every race (including the `currentRace >= 0` case), and every writer
  that derives both flags from one standing (`PlayerFighter::update`,
  `PlayerFixedObject::update`, `PlayerTurret::update`) sets exactly one.
* The shipGroup 8/9 branch sets `enemyFlagsLo = true` and
  `carriesFriendCargoFlag = false`; `turnedEnemy()` sets `true/false`;
  `isAlwaysFriend()` sets `false/true`; `Player::setAlwaysFriend` sets
  `enemyFlagsLo = 0, carriesFriendCargoFlag = 1`; the 16-bit
  `reinterpret_cast<uint16_t &>(enemyFlagsLo)` writes are `1` (enemy) or
  `0x100` (friend), i.e. one-hot.
* `LevelScript` / `Level` write only `enemyFlagsLo` (0 or 1) on the three
  intro pirate fighters, whose `carriesFriendCargoFlag` is already false
  because they are the hostile faction.

So at the lock-image point at most one flag is set; enemy-first and
friend-first select the same image.

### 1.2 Off-screen "far plain" image chain

Base:

```cpp
} else if (__builtin_expect(enemy->player->enemyFlagsLo, 1)) {  // lint: arm_builtin
    image = this->image_0x60;
} else if (enemy->player->carriesFriendCargoFlag) {
    image = this->image_0x68;
} else {
    image = this->image_0x78;
}
```

Landed (permutation 210: else branch first, friend second, enemy last):

```cpp
} else if (!enemy->player->enemyFlagsLo && !enemy->player->carriesFriendCargoFlag) {
    image = this->image_0x78;
} else if (enemy->player->carriesFriendCargoFlag) {
    image = this->image_0x68;
} else if (__builtin_expect(enemy->player->enemyFlagsLo, 1)) {  // lint: arm_builtin
    image = this->image_0x60;
}
```

This is the same three-way selector with the branches emitted in the
original's order. Under the mutual-exclusivity argument above it selects
the same image in every reachable state, and `image` is still assigned on
every path (if neither flag is set the first arm fires; otherwise exactly
one of the friend/enemy arms fires), so there is no fall-through UB.

## 2. Sweeps run (all size-exact cells only)

Base for every cell is 6,849 / 1,577 / 9,908. All builds were `-j2` under
`ulimit -v 12000000`. Scratch scripts: `sweep*.py` in
`/home/debian/GalaxyOnFire2/scratch/w11908/`.

### 2.1 Whole-chain permutations

Each image chain was rewritten in all 6 branch orders (notation = source
order of the three arms; the base order is `012`). Only the two landed
cells beat the base; everything else is neutral or worse.

| chain | 012 | 021 | 102 | 120 | 201 | 210 |
|---|---:|---:|---:|---:|---:|---:|
| lock (754-761) | 6,849 | 6,849 | **6,847** | **6,847** | 6,849 | 8,711 (9,892) |
| far plain (777-783) | 6,849 | 6,849 | 8,400 (9,912) | 8,400 (9,912) | 6,849 | **6,835** |
| far locked (770-776) | 6,849 | 6,849 | 8,399 (9,912) | 8,399 (9,912) | 6,849 | 8,512 (9,904) |
| off-screen (806-818) | 6,849 | 6,849 | 8,399 (9,904) | 8,385 (9,912) | 8,385 (9,912) | 8,381 (9,912) |
| frame/bar (727-733) | 6,849 | 6,849 | 6,849 | 6,849 | 6,849 | 8,696 (9,872) |
| wreck (808-812) | 6,849 | 6,849 | 6,850 | 6,850 | 6,849 | 8,077 (9,900) |

Parenthesised cells are non-size-exact and are not eligible. The far-plain
`210` cell is the only off-screen-chain win; the frame, far-locked,
off-screen and wreck chains have none.

Combined cells: far-plain `210` alone 6,835; far-plain `210` + lock `102`
(or `120`) **6,833**; adding frame `102` stays 6,833 but aligned 1,622
(worse), so it was not kept.

### 2.2 Expect sweeps

* `sweep_expect.py/.log` — `__builtin_expect(cond, 0/1)` on every named
  condition (681/694/695/697/727/730/755/757/770/772/777/779/808/810/
  816/818/829): only neutral cells; 818 and 829 are size-fatal.
* `sweep_allconds.py/.log` — all 36 `if` conditions in lines 676-860,
  whole-condition expect 0/1: no cell below 6,849.
* `sweep_lock*.py` — lock-chain re-spellings: `!friend && enemy` (the
  friend-first form) 6,847; ternary 6,850; bool temps / nested 6,849;
  friend-first nested enemy-priority 8,124 (9,900).
* `sweep_campaign*.py`, `sweep_mbp.py`, `sweep_forms*.py`, `sweep_sg10.py`,
  `sweep_struct.py`, `sweep_more.py`, `sweep_zero*.py`, `sweep_swap.py` —
  campaign/missionBlocksPlanets allocation, `shipGroup == 10` re-spellings,
  zero-init forms and adjacent `&&`/`||` swaps: all neutral or worse.

## 3. Remaining residual

The dominant residual is unchanged from wave 173: the original keeps
`campaign` in `sl` (`12e0f8: mov sl,r0`, `12e38e: cmp.w sl,#2`) and
`missionBlocksPlanets` in `fp`, whereas ours spills `campaign` to
`sp+144` and keeps `missionBlocksPlanets` in `sl`. That cascades through
the landmark/planet loops and the whole enemy/asteroid/music tail (the
`fp`/`sl`/`r5`/`r6`/`r9` shuffles and the `sp+72/76/84/92` image-pointer
slot swaps). Declaration-order, type and call-order experiments on those
two variables were re-run in wave 173 and are all neutral or worse; this
pass did not find a source-reachable allocation knob.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /home/debian/GalaxyOnFire2/scratch/w11908/
#   sweep.py, sweep_perm.py, sweep_perm_far.py, sweep_perm_frame.py,
#   sweep_perm_off.py, sweep_lock*.py, sweep_combo*.py, sweep_expect.py,
#   sweep_allconds.py, Radar.cpp.base, base.so
```
