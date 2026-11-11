# Radar::draw wave 181 (w12508) pass 2 — dock/sound/store order + stop-test operand order (raw 6,372 -> 6,356)

**Status:** landed, 2026-11-11. Worktree `wt/w12508`, branch `wave181-w12508`,
base `master` `1e270aa0e`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Pass 1 is `radar-draw-wave181-w12508.md` (raw 6,379 -> 6,372). This note is
self-contained for pass 2; every number was measured on this worktree.

## 0. Baseline and landed result

| metric | pass 1 (base of this pass) | pass 2 (landed) |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,372 | **6,356** (-16) |
| instruction-index alignment | 1,752 | 1,756 |
| instruction count | 3,542 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 76,193 | **76,177** (-16) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 713 | **713** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| total differing bytes | 76,926 | **76,910** |
| file size | 2,187,328 | 2,187,328 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (two cells, net size 0)

```diff
@@ planet-lock body (line ~635)
-                    this->field_0x14 = this->field_0x18;
                     if (__builtin_expect(this->field_0x1a8, 0)) {  // lint: arm_builtin
                         this->level->getPlayer()->dockToPlanet();
                     }
                     if (!this->lockConfirmed) {
                         Globals::sound->play(26, nullptr, nullptr, 0.0f);
                     }
+                    this->field_0x14 = this->field_0x18;

@@ enemy-music stop guard (line ~1271)
-            if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens() ||
-                Globals::status->getCurrentCampaignMission() == 16) {
+            if (Globals::status->getStation()->isAttackedByAliens() ||
+                Globals::status->getCurrentCampaignMission() == 16 || Globals::status->inAlienOrbit()) {
```

Both cells are pure statement/operand reorderings. The three `Status` queries
are side-effect-free predicates and the OR is commutative, so the result is
identical; the store `field_0x14 = field_0x18` is a plain member write that
neither `dockToPlanet()` nor `FModSound::play` reads.

## 2. Why this moves bytes

The original's stop guard calls the three queries in the order
`inAlienOrbit` (`1302ac`) -> `getStation`/`isAttackedByAliens`
(`1302b4`/`1302b8`) -> `getCurrentCampaignMission` (`1302c0`). Pass 1's
`inAlien`-first source order reproduces that call sequence but at `+8`
addresses (`1302b4`/`1302c0`/`1302c8`), because the mask/expect block just
above is 8 B longer than the original's. Writing the OR as
`isAttacked -> campaign -> inAlien` makes clang emit `getStation` at
`1302b4`, `isAttackedByAliens` at `1302b8` and
`getCurrentCampaignMission` at `1302c0` — exactly the original's addresses —
and moves only `inAlienOrbit` to the tail (`1302ca`). Three of the four call
sites then match byte-for-byte, which is the -16 raw / -16 `.text`.

The planet-lock body is the second cell. The original emits the
`lockConfirmed`/`play(26)` test first, then `field_0x14 = field_0x18`, then
the `field_0x1a8` dock branch (`130454`-`130482`). Moving the store after the
sound block makes clang schedule the store next to the `field_0x18` load in
the dock arm and removes the extra `str`/`ldr` pair around the branch. This
cell alone is -1 raw (`6,372 -> 6,371`); it is kept because it is size-exact
and additive with the stop-test cell.

## 3. Search evidence (all size-exact unless noted)

Every measurement is a full `link_so` + `tools/fn_diff.py` on this worktree
(`-j2`, `ulimit -v 12000000`), starting from the pass-1 source.

### 3.1 Planet-lock body statement orders (6 permutations)

| order (store / dock / sound) | raw | size |
|---|---:|---:|
| store, dock, sound (pass 1) | 6,372 | 9,908 |
| sound, store, dock (original source order) | 6,372 | 9,908 |
| sound, dock, store | 6,372 | 9,908 |
| store, sound, dock | 6,462 | **9,912** |
| dock, store, sound | 6,481 | **9,912** |
| **dock, sound, store (landed)** | **6,371** | **9,908** |

### 3.2 Stop-test operand orders (6 permutations, on top of the landed planet order)

| order | raw | aligned | size |
|---|---:|---:|---:|
| `campaign, isAttacked, inAlien` | 6,371 | 1,753 | 9,908 |
| `campaign, inAlien, isAttacked` | 6,371 | 1,758 | 9,908 |
| `inAlien, isAttacked, campaign` (pass 1) | 6,371 | 1,751 | 9,908 |
| `inAlien, campaign, isAttacked` | 6,372 | 1,758 | 9,908 |
| `isAttacked, inAlien, campaign` | 6,361 | 1,753 | 9,908 |
| **`isAttacked, campaign, inAlien` (landed)** | **6,356** | **1,756** | **9,908** |

The landed order is the unique minimum. On the pass-1 planet order it scores
6,357 (the extra byte is the planet cell above).

### 3.3 Single `__builtin_expect` removals on the landed shape (30 sites)

All 30 sites were removed one at a time. The best are neutral at 6,356
(sites 4, 12, 15); every other removal is worse (raw 6,357-8,534) or
size-fatal (15 of the 30 sites change the 9,908 B size). All pairs/triples of
the three neutral sites are also neutral at 6,356, so the landed shape is a
local optimum in the hint space.

### 3.4 Other cells re-confirmed, not landed

| variant | raw | size |
|---|---:|---:|
| wreck/update block `update; wreck; field_0x75 = 0` | 6,358 | 9,908 |
| gas-cloud guard `isDead() || isDying()` | 6,357 | 9,908 |
| landmark crosshair `float top` | 8,747 | **9,932** |

## 4. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12508/R1.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6356 aligned= 1756 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2    # RATCHET PASS
cmake --build cmake-build-match --target drift -j2    # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12508/` — `L.cpp` (pass-1
source), `R1.cpp` (landed pass-2 source), `genp.py` / `gen2.py` / `gen3.py` /
`gen5.py` (planet/stop-order generators), `expR1.py` (hint-removal sweep),
`gen6.py` (neutral-removal pairs), `measure.sh`, `secdiff.py`.
