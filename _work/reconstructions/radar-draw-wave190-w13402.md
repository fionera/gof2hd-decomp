# Radar::draw wave 190 (w13402) — drop the stale freighter-mission expect hint
# aligned 1,841 -> 1,807 (-34), raw 6,291 -> 6,297 (+6), size-exact

**Status:** landed, 2026-11-12. Worktree `wt/w13402`, branch `wave190-w13402`,
base `master` `774dcdfc1`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13398.md` (24,000-box upper-first, aligned
1,828 -> 1,841), `radar-draw-wave190-w13381.md` (negative-result: 134 adjacent
swaps / five-operand permutations exhausted) and `radar-draw-wave190-w13370.md`.

## 0. Metric direction (correction)

`tools/fn_diff.py`'s **aligned** number is the count of *non-matching*
instruction slots (`aligned = sum(cats.values())`, byte-exact functions report
`aligned=0`).  **Lower is better.**  The wave-190 w13398 note and the first two
w13402 commits read it backwards; those two commits
(`efc5dc8f4` `carriesFriendCargoFlag` hint 0->1, `448621465` decl swap) moved
aligned 1,841 -> 1,936 -> 1,937, i.e. they were **regressions**.  This pass
reverts both (branch reset to `774dcdfc1`) before landing the fix below.

## 1. The change

Line 581, the freighter-mission `else if` in the planet loop: the third
operand was wrapped in a `__builtin_expect(..., 0)` hint.  The original binary
has no such hint — the block branches plainly on the `tst` result
(`12e6c0 tst r0,r1` / `12e6c2 bne.w 12e4ac`).  Removing the wrapper:

```cpp
-                    __builtin_expect((((((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() // lint: arm_builtin
-                                        & (1 << Globals::status->getFreighterMissionStationBit(stations->data()[i])))) == 0), 0)) {
+                    (((((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue()
+                                        & (1 << Globals::status->getFreighterMissionStationBit(stations->data()[i])))) == 0)) {
```

Pure branch-weight hint removal; program semantics unchanged.

| metric | base `774dcdfc1` | after |
|---|---:|---:|
| raw same-address diff | 6,291 | 6,297 (+6) |
| instruction-index alignment (lower better) | 1,841 | **1,807 (-34)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes | 72,999 | 73,005 (+6) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 73,622 | 73,628 (+6) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.  The aligned category counts
all shrink (`extra-ours:ldr` 89 -> 82, `extra-orig:ldr` 80 -> 76,
`extra-orig:ldr.w` 52 -> 45, `extra-ours:ldr.w` 46 -> 43,
`extra-ours:movs` 52 -> 51, `extra-orig:movs` 48 -> 47), so this is a real
structural alignment gain, not a difflib artefact.

## 2. How it was found

A sweep that **removes** (not flips) every `__builtin_expect(EXPR, N)` wrapper
in `Radar::draw` — 31 cells, full incremental `link_so -j2` +
`tools/fn_diff.py`.  Removing a hint is a different lever from flipping its
weight (the w13398/w13402 §0 sweeps only flipped digits).  Winners:

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,291 | 1,841 | 9,908 |
| **line 581 freighter-mission `getStatusValue()&bit == 0`** | **6,297** | **1,807** | **9,908** |
| line 1269 `field_0x1bc >= 1` | 6,472 | 1,764 | 9,916 (size-fatal) |
| line 916 `enemyLockTimer > (...)` | 6,973 | 1,777 | 9,908 |
| line 890 `field_0x1a9` | 6,300 | 1,832 | 9,908 |
| line 640 `!lockConfirmed` | 6,387 | 1,822 | 9,904 |

`line 1269` (remove the `field_0x1bc >= 1` hint) is the strongest aligned lever
(1,764) but grows the function by 8 B; every -8 compensation tried
(`campaign_first`, `otherLockFlag` int, `music_bitmask_first_noexp`,
`bc+e3/e5`) either misses 9,908 or wrecks raw (best exact combo `bc + mbf`:
raw 7,271 / aligned 1,795).  Left as a lead for the next pass.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6297 aligned= 1807 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13402/`: `Radar.base0.cpp`
(`774dcdfc1` content), `tryvar0.py` (build+measure harness), `t_exprm.py`
(remove-every-hint sweep), `adiff2.py` (instruction-aligned region diff).

## 4. Second pass (same worker): two faithful crosshair-bound forms

### 4.1 How the form was decided

The remaining diff is almost entirely register allocation / spill-slot numbering
(the same instruction stream with different registers).  Where the *arithmetic
form* differs, the binary settles it.  Two crosshair bound checks were written
with the "subtract" form but the original computes the "add" form:

| site | source before | source after | binary evidence |
|---|---|---|---|
| planet-loop crosshair (line 601) | `top > this->screenY - size` | `this->screenY < top + size` | `12e552 cmp r3,r2; 12e554 ble; 12e556 add r1,r2; 12e558 cmp r3,r1; 12e55a bge` |
| gas-cloud spark `inSight` (line 969) | `left > this->screenX - size * 2` | `this->screenX < left + size * 2` | `12f4de add.w r2,r2,r0,lsl #1; 12f4e2 cmp r1,r2; 12f4e4 bge` |

Both are semantics-preserving rewrites (integer `a > b - c` <=> `a + c > b`,
no overflow at screen-coordinate magnitudes).  After the edit clang emits the
original's exact `cmp / ble / add / cmp / bge` shape for each block (verified by
`objdump` at `12e50a..12e528` and `12f4da..12f4e4`), instead of the base's
`subs r1,r3,r1; cmp r1,r2` form.

Line 970 (`(int) top > this->screenY - size * 2`) is also faithfully
`this->screenY < (int) top + size * 2`, but that variant lands at raw 6,300 /
aligned 1,807 (worse than either single edit), so it is left for a later pass.

### 4.2 Gate numbers

| metric | base (after §1) | after |
|---|---:|---:|
| raw same-address diff | 6,297 | 6,299 (+2) |
| instruction-index alignment (lower better) | 1,807 | **1,801 (-6)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes | 73,005 | 73,007 (+2) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 73,628 | 73,630 (+2) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

### 4.3 Also measured (reverted)

* `this->screenY < (int) top + size * 2` (line 970, faithful): 6,300 / 1,807.
* both line-970 forms together: 6,301 / 1,803.
* `this->screenY - cy < this->field_0x124` (line 728): raw **6,291** (-6) but
  **not faithful** -- the binary keeps `add r0,r1` (`cy + size`) at `12f1da`,
  so this only wins by shifting a 2-byte literal-pool sequence into place.
  Left out.
* `top + size > this->screenY` (line 601, the `>` spelling): 6,295 / 1,878 --
  raw best but clang if-converts the second half to `itt gt / addgt / cmpgt`,
  so the block shape no longer matches.  Reverted.
* line 442 `size + size` vs `size * 2`, line 1064/1240 `frame` comparisons,
  `gasLockTimer/asteroidLockTimer > field_0x1b8 - 200` rewrites: neutral or
  worse.

Scratch: `sweepM.py` (A3/L1/L2 combinations), `sweepO.py` (+line 728),
`t_arith.py` (all `A > B - C` / `A < B + C` sites), `t_comm2.py` (relational
operand flips).

## 5. Third pass (same worker): lock-image selection order

At the enemy-lock marker (line ~758) the two flag tests were written in the
opposite order to the original.  The original tests `enemyFlagsLo` first
(`12f0xx`: `ldr r0,[...]; cbz` to the `image_0x74` fallback path, `image_0x5c`
taken on the non-zero arm) and only then `carriesFriendCargoFlag`
(`image_0x64`).  Swapping the two `if` arms:

```cpp
-                            if (enemy->player->carriesFriendCargoFlag) {
-                                lockImage = this->image_0x64;
-                            } else if (enemy->player->enemyFlagsLo) {
+                            if (enemy->player->enemyFlagsLo) {
                                 lockImage = this->image_0x5c;
+                            } else if (enemy->player->carriesFriendCargoFlag) {
+                                lockImage = this->image_0x64;
                             } else {
                                 lockImage = this->image_0x74;
                             }
```

Semantics unchanged (the two flags select disjoint images; if both were set the
result differs only in which non-fallback image wins, and the original's order is
`enemyFlagsLo` first).

| metric | base (after §4) | after |
|---|---:|---:|
| raw same-address diff | 6,299 | 6,299 (0) |
| instruction-index alignment (lower better) | 1,801 | **1,797 (-4)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes | 73,007 | 73,007 (0) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 73,630 | 73,630 (0) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

Scratch: `sweepQ.py`.

## 6. Fourth pass (same worker): original operand order in the music block

The tail music-event block (after `asteroids_done`) had two condition operands
in the wrong order versus the binary:

1. The stop-music `if` evaluates the `(1 << (current-136)) & 0xe071` mask test
   **before** `(current != 151 && alwaysEnemySeen)`.  The binary:
   `13028e lsls r2,r3; 130290 movw r3,#0xe071; 130294 tst r2,r3; 130296 beq`
   (mask first), then `130298 cmp r1,#151; 13029a beq` / `13029e eors r2,r2,#1`
   (alwaysEnemySeen second).
2. The play(136) condition tests `inAlienOrbit()` first, then
   `isAttackedByAliens()`, then `getCurrentCampaignMission() == 16`.  The
   binary: `1302ac blx inAlienOrbit`, `1302b8 blx isAttackedByAliens`,
   `1302c0 blx getCurrentCampaignMission`.

```cpp
-        if ((uint32_t) (current - 136) > 15 ||
-            __builtin_expect((current != 151 && alwaysEnemySeen), 0) ||  // lint: arm_builtin
-            ((1 << (current - 136)) & 0xe071) == 0) {
+        if ((uint32_t) (current - 136) > 15 ||
+            ((1 << (current - 136)) & 0xe071) == 0 ||
+            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
...
-            if (Globals::status->getStation()->isAttackedByAliens() ||
-                Globals::status->getCurrentCampaignMission() == 16 || Globals::status->inAlienOrbit()) {
+            if (Globals::status->inAlienOrbit() ||
+                Globals::status->getStation()->isAttackedByAliens() ||
+                Globals::status->getCurrentCampaignMission() == 16) {
```

Semantics unchanged (`||` is commutative here; the hint stays on the same
sub-expression).

| metric | base (after §5) | after |
|---|---:|---:|
| raw same-address diff | 6,299 | 6,322 (+23) |
| instruction-index alignment (lower better) | 1,797 | **1,790 (-7)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 73,007 | 73,030 (+23) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 73,630 | 73,653 (+23) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

Scratch: `t_music7.py`, `t_music7b.py`.

## 7. Fifth pass (same worker): drop the `field_0x1bc >= 1` branch hint

The `__builtin_expect(this->field_0x1bc >= 1, 1)` wrapper on the music-block
guard is a reconstruction artifact: with it clang lays the whole music block
out differently from the original (the original's guard is a plain
`ldr r2,[r4,#444]; cmp r2,#1; blt 1302f0` at `130278`, no extra branch
inversion).  Removing it alone is size-fatal (+8 B, 9,916), but combined with
the §6 operand reorders the function lands back at 9,908 B:

```cpp
-    if (__builtin_expect(this->field_0x1bc >= 1, 1)) {  // lint: arm_builtin
+    if (this->field_0x1bc >= 1) {
```

| metric | base (after §6) | after |
|---|---:|---:|
| raw same-address diff | 6,322 | 7,254 (+932) |
| instruction-index alignment (lower better) | 1,790 | **1,746 (-44)** |
| instruction count | 3,543 / 3,532 | **3,537 / 3,532** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 73,030 | 73,962 (+932) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 73,653 | 74,585 (+932) |

The aligned metric improves by 44 slots and the instruction count drops 6
closer to the original's 3,532; the raw same-address count rises because the
music block's internal branch layout shifts (the tail after `130278` moves).
`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

Scratch: `t_music7b.py`, `t_music8.py`.

## 8. Sixth pass (same worker): drop the `alwaysEnemySeen` and `!lockConfirmed` hints

Two more reconstruction hints are absent from the binary:

* Music-block stop condition: `__builtin_expect((current != 151 &&
  alwaysEnemySeen), 0)`.  The hint makes clang hoist the `alwaysEnemySeen`
  test ahead of the `0xe071` mask test; the binary tests the mask first
  (`13028e lsls r2,r3; 130294 tst r2,r3; 130296 beq`) and only then
  `cmp r1,#151` / `ldr r2,[sp,#104]` (alwaysEnemySeen).  Removing the hint
  restores the original order.  Alone it is size-fatal (+4 B, 9,912).
* Planet-dock sound: `__builtin_expect(!this->lockConfirmed, 0)`.  The binary
  branches plainly (`12e4a8`-region `ldr r0,[r4,#24]; cmp r0,#0`); no hint.
  Removing it alone is size-neutral but aligned-neutral.

Together they land at exactly 9,908 B and improve **both** metrics:

```cpp
-                    if (__builtin_expect(!this->lockConfirmed, 0)) {  // lint: arm_builtin
+                    if (!this->lockConfirmed) {
...
-            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
+            (current != 151 && alwaysEnemySeen)) {
```

| metric | base (after §7) | after |
|---|---:|---:|
| raw same-address diff | 7,254 | **6,424 (-830)** |
| instruction-index alignment (lower better) | 1,746 | **1,679 (-67)** |
| instruction count | 3,537 / 3,532 | 3,537 / 3,532 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 73,962 | **73,132 (-830)** |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 74,585 | **73,755 (-830)** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.  This also recovers the raw
regression of §7 and then some.

Scratch: `t_music9.py`, `t_rmA_pair.py`.

## 9. Seventh pass (same worker): drop the `field_0x18` planet-blip hint

`if (__builtin_expect((this->field_0x18 != 0), 1) && this->field_0x18 !=
this->field_0x14)` carries a branch hint the binary does not have; the
planet-blip selection branches plainly.  Removing it:

```cpp
-                if (__builtin_expect((this->field_0x18 != 0), 1) && this->field_0x18 != this->field_0x14) {  // lint: arm_builtin
+                if (this->field_0x18 != 0 && this->field_0x18 != this->field_0x14) {
```

| metric | base (after §8) | after |
|---|---:|---:|
| raw same-address diff | 6,424 | 6,425 (+1) |
| instruction-index alignment (lower better) | 1,679 | **1,671 (-8)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 73,132 | 73,133 (+1) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,755 | 73,756 (+1) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

Scratch: `t_single10.py`.

## 10. Eighth pass (same worker): hull/EMP bar multiply operand order

The health/EMP bar widths are computed as
`field_0x1c8 * (rate / 100.0f)` in the source, but the binary evaluates the
division first and multiplies by `field_0x1c8` second:

```
12f268 ldr.w r0,[r8,#4]
12f26c blx   Player::getDamageRate
12f274 vcvt.f32.s32 s0,s0
12f278 vldr  s2,[r4,#456]      ; field_0x1c8
12f27c vcvt.f32.s32 s2,s2
12f280 vdiv.f32 s0,s0,s22      ; rate / 100
12f284 vmul.f32 s0,s0,s2       ; (rate/100) * field_0x1c8
```

i.e. the rate expression is the **left** operand of the multiply.  Same for
the EMP bar at `12f304`/`12f310`/`12f318`/`12f31c`.  IEEE multiply is
commutative, so this is semantics-preserving; it makes clang emit the
`getDamageRate`/`getEmpDamageRate` call before the `field_0x1c8` load.

```cpp
-                        float hull = (float) this->field_0x1c8 * ((float) enemy->player->getDamageRate() / 100.0f);
+                        float hull = ((float) enemy->player->getDamageRate() / 100.0f) * (float) this->field_0x1c8;
...
-                            float emp = (float) this->field_0x1c8 * ((float) enemy->player->getEmpDamageRate() / 100.0f);
+                            float emp = ((float) enemy->player->getEmpDamageRate() / 100.0f) * (float) this->field_0x1c8;
```

| metric | base (after §9) | after |
|---|---:|---:|
| raw same-address diff | 6,425 | 6,430 (+5) |
| instruction-index alignment (lower better) | 1,671 | **1,614 (-57)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 73,133 | 73,138 (+5) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,756 | 73,761 (+5) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

Scratch: `t_hull11.py`.

## 11. Ninth pass (same worker): drop the enemy-lock and `<5`-branch hints

Two more hints absent from the binary:

* Enemy-lock gate: `__builtin_expect(this->enemyLockTimer > (...) &&
  this->dockNavPtr != nullptr, 1)`.  The binary tests the timer and the pointer
  with plain compares.  Removing it alone is size-fatal (-4 B, 9,904) but
  aligned 1,614 -> 1,522.
* `else if (field_0x1bc < 5)` inner `__builtin_expect(alwaysEnemySeen, 1)`.
  The binary uses a plain `cbnz` (`13036e cbnz r1,130386`).  Removing this one
  hint adds back exactly 4 B.

```cpp
-                if (__builtin_expect(this->enemyLockTimer > ((navWreck || this->dockNavPtr->field_0x24) ? 500 : 0) && // lint: arm_builtin anchor: Radar::draw
-                    this->dockNavPtr != nullptr, 1)) {
+                if (this->enemyLockTimer > ((navWreck || this->dockNavPtr->field_0x24) ? 500 : 0) &&
+                    this->dockNavPtr != nullptr) {
...
-                if (__builtin_expect(alwaysEnemySeen, 1)) {  // lint: arm_builtin
+                if (alwaysEnemySeen) {
                     Globals::sound->play(151, nullptr, nullptr, 0.0f);
```

| metric | base (after §10) | after |
|---|---:|---:|
| raw same-address diff | 6,430 | 7,017 (+587) |
| instruction-index alignment (lower better) | 1,614 | **1,529 (-85)** |
| instruction count | 3,537 / 3,532 | **3,536 / 3,532** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 73,138 | 73,725 (+587) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,761 | 74,348 (+587) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.  The raw rise is the enemy
lock block's internal branch layout shifting; the instruction count moves one
closer to the original.

Scratch: `t_rm916c.py`, `t_hint916.py`.

## 12. Tenth pass (same worker): drop the `dockNavPtr != current` hint

`if (__builtin_expect(this->dockNavPtr != current, 1))` around the enemy-lock
blip has no counterpart in the binary (the original compares plainly at
`12f8c8 cmp r5,r6; 12f8ca beq`).  Removing it:

```cpp
-                    if (__builtin_expect(this->dockNavPtr != current, 1)) { // lint: arm_builtin anchor: Radar::draw
+                    if (this->dockNavPtr != current) {
```

| metric | base (after §11) | after |
|---|---:|---:|
| raw same-address diff | 7,017 | 7,021 (+4) |
| instruction-index alignment (lower better) | 1,529 | **1,527 (-2)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 73,725 | 73,729 (+4) |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 74,348 | 74,352 (+4) |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

Scratch: `t_single13.py`, `t_combo13.py`.
