# HangarWindow::render — wave 190 (w13379): faithful upgrade-test operand order

Worktree `wt/w13379`, branch `wave190-w13379`, base master `5cd492611`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.
Entry state is the wave-189 w13348 body: raw **1,360**, aligned **530**,
`.text` 73,487, `.ARM.extab` 610, build-id 20, total 74,117.

## Result (landed)

Swap the two operands of the upgrade test from
`upgradeMode && upgTab == 1` to `upgTab == 1 && upgradeMode`:

```cpp
if (__builtin_expect(upgTab == 1 && this->upgradeMode, 1)) { // lint: arm_builtin
```

The original evaluates `tab == 1` first, then `upgradeMode`
(`0x149b96 cmp r0,#1` / `bne`, then `0x149b9c ldrb [r4,#285]` / `cmp`), so
this is also the faithful operand order.  The two operands have no side
effects, so the swap is semantics-preserving.

| metric | wave-189 base | landed |
|---|---:|---:|
| raw same-address diff | 1,360 | **1,355** |
| instruction-index alignment | 530 | 617 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 73,487 | **73,482** |
| `.ARM.extab` section diff | 610 | 613 |
| render LSDA record diff | 54 | 57 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 74,117 | **74,115** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.  `.ARM.extab` grows 3 B for a 5 B raw win
(net −2 B), within the "extab must not grow more than the raw win" rule.

## LSDA

Row 39 is still `(2508, 172, 9664)` (orig `(2508, 150, 9664)`): the ship
body's first call is `setVisible`, so the body cleanup scope starts there.
Row 40 is `(2688, 148, 9574)` (orig `(2696, 140, 9562)`).  Rows 41-44 stay
exact.  The record diff rises 54 -> 57 because the swap perturbs the later
call-site rows (105-113), not rows 39/40.

## Negative results (all size 9,700 unless stated)

The upgrade-test condition was swept with the wave-189 ship body
(`setVisible, upgTab, draw, setPosition, getWidth`) and the landed upgrade
button order `p,v2,v,d`:

| condition form | raw | aligned | text | extab | total |
|---|---:|---:|---:|---:|---:|
| **`expect(upgTab == 1 && upgradeMode, 1)` (landed)** | **1,355** | 617 | 73,482 | 613 | **74,115** |
| `expect(upgradeMode && upgTab == 1, 1)` (wave-189 base) | 1,360 | 530 | 73,487 | 610 | 74,117 |
| `expect(upgTab == 1 && upgradeMode != 0, 1)` | 1,355 | 617 | 73,482 | 613 | 74,115 |
| `expect(upgTab == 1 && upgradeMode == true, 1)` | 1,355 | 617 | 73,482 | 613 | 74,115 |
| `expect(upgTab == 1 && (bool) upgradeMode, 1)` | 1,355 | 617 | 73,482 | 613 | 74,115 |
| `expect(1 == upgTab && upgradeMode, 1)` | 1,355 | 617 | 73,482 | 613 | 74,115 |
| `expect(upgTab > 0 && upgTab < 2 && upgradeMode, 1)` | 1,355 | 617 | 73,482 | 613 | 74,115 |
| `expect(upgTab == 1, 1) && upgradeMode` | 1,446 | 541 | 73,573 | 611 | 74,204 |
| `upgTab == 1 && expect(upgradeMode, 1)` | 1,383 | 530 | 73,510 | 613 | 74,143 |
| plain `(upgTab == 1 && upgradeMode)` | 1,400 | 530 | 73,527 | 616 | 74,163 |

With the landed condition, all 24 upgrade-button orders were re-swept:
`p,v2,v,d` is best (raw 1,355, extab 613, total 74,115); `p,v,v2,d` ties
on total (1,356 / 613 / build-id 19).  The faithful original order
`v2,p,v,d` is raw 1,502, extab 617 (total 74,266).

Ship-body order (24 permutations) under the landed condition: only
`v,d,s,b` (1,355), `d,v,s,b` (1,356), `d,s,v,b` and `v,s,d,b` are
size-exact; `v,d,s,b` is best on total.  The original source order
`s,v,d,b` is 9,704 B with the `upgTab` snapshot after `setPosition`, and
9,700 B only with the snapshot before `setPosition` (raw 1,510, extab 606,
total 74,263) — a net loss, so the wave-189 call order is kept.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
        /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13379/`
(`HangarWindow.base.cpp`, `sweep*.py`, `sweep*.out`).

## Current-master rebase (wave 190 harvest)

Re-applied on current master (`17f1b4a79`). Current master before this change was
raw 1,360; after it is raw **1,355** (-5), aligned 530 -> 617, size 9,700/9,700.
`.text` 73,283 -> 73,277 (-6 across Radar + Hangar), `.ARM.extab` 603 -> 606
(+3), total 73,906 -> 73,903 (-3). `check` green, RATCHET PASS.
