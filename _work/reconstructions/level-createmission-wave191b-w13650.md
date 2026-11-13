# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13650

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13650`, base master `0f23322e4`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13638.md` (§3f/§3g), `...w13625.md`,
`...w13615.md`, `...w13601.md`.

```text
                                     size            raw    aligned  insn
base 0f23322e4                       17172/17172     2626    845     6206/6197
faithful order (cargo after S)       17172/17172     2642    1022    6206/6197
this pass (landed)                   17172/17172     2636    841     6206/6197
```

Landed: the **semantic** alienGuns fix.  `gun->geometry->setRotation(v)` moved
from after the second `MatrixRotateVector` to after the first one, so
`setRotation` receives the `t[k][3..5]` rotation and `setPosition` uses the
`t[k][0..2]` rotation — exactly the binary's call sequence at
`0xb32f6/0xb3308/0xb3320` (w13638 §3f).  The source diff is one moved line.

Net: raw **2626 -> 2636 (+10)**, aligned **845 -> 841 (-4)**, size exact
17,172/17,172, `.ARM.extab` unchanged (577 differing bytes, size/base exact at
`0x1a3b78`), `.text` diff 71,197 -> 71,207, `.ARM.exidx` 0, `.rodata` 0.
`check` green: byte_exact 4508, linked_exact 4508, data_exact 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370.

## 1. What the binary actually does (re-confirmed)

alienGuns loop (`0xb32f6..0xb3384`):

```text
v = MatrixRotateVector(rotation, {t[k][3], t[k][4], t[k][5]});
gun->geometry->setRotation(v);                 // FIRST rotation
v = MatrixRotateVector(rotation, {t[k][0], t[k][1], t[k][2]});
gun->setPosition(base + v);                    // SECOND rotation
((PlayerTurret *)gun)->setScaling(6.0f);
gun->cargo = nullptr;
gun->shipGroup = localRace;
```

The checked-in source had both rotations before `setRotation`, so the first
rotation was dead and `setRotation` got the second vector — a real semantic
divergence, not just scheduling.

## 2. Store order is the compensation lever

The binary's `cargo = nullptr` store sits *after* `setScaling`, but the store is
not observable (no call reads `cargo` between `setPosition`/`setScaling` and the
store).  Keeping it where the source already had it (before `setPosition`)
recovers the whole aligned cost and most of the raw cost:

| variant (alienGuns tail order) | raw | aligned | size |
|---|---:|---:|---:|
| base (both rotations, setRotation=second) | 2626 | 845 | 17,172 |
| interleave + `cargo` after `setScaling` (task baseline) | 2642 | 1,022 | 17,172 |
| **interleave + `cargo` before `setPosition` (landed)** | **2636** | **841** | **17,172** |

All 24 permutations of `{setPosition, setScaling, cargo, shipGroup}` were
measured (with the interleave and the base pirateFortress order).  Every
permutation with `cargo` before `setScaling` scores aligned 841–847; every one
with `cargo` after `setScaling` scores aligned 1022.  Best raw is `CPSG`/`GPSC`
= **2636 / 841**; the landed source is `CPSG`.

## 3. pirateFortress: the checked-in source order is already right *in codegen*

The binary emits `cargo` then `shipGroup` (`0xb34c4` `str [r4,#0x50]`,
`0xb34c8` `str [r4,#0x28]`).  The checked-in source writes
`setScaling; shipGroup; cargo;` and **already emits** `cargo` then `shipGroup`
(the scheduler inverts the two independent stores).  Swapping the source to
`cargo; shipGroup` makes the compiler emit `shipGroup` then `cargo` (wrong) and
costs +6 raw:

| pirateFortress source order | emitted stores | raw | aligned |
|---|---|---:|---:|
| `shipGroup; cargo` (checked in, landed) | cargo, shipGroup | 2636 | 841 |
| `cargo; shipGroup` (task's "faithful text") | shipGroup, cargo | 2642 | 846 |

So no pirateFortress source change is landed: the current text already
reproduces the binary's store order.

## 4. Other measured shapes (all size 17,172 unless noted)

On top of the landed interleave (`CPSG`):

| variant | raw | aligned | note |
|---|---:|---:|---|
| `gun` loaded after the `enemies[i]` store (w13638 §3g) | 2637 | 837 | binary reloads the element; +1 raw |
| case-2 loot `GCSR` order (w13638 §3d) | 2641 | 837 | |
| `AERandom *const &rnd` | 2637 | 841 | |
| `Vector v;` / `k` after `base` | 2636 | 841 | neutral |
| plain `bigAlienWave ? ... : ...` (drop `(i<0)` crutch) | 4371 | 874 | **17,168**, extab +38 |
| `Vector vr = first; v = second; setRotation(vr)` | 2926 | 857 | temp form |
| `int coords[10]`/`coords[8] = enemyRace` (block LICM hoist) | 12,046 | 1,513 | **17,180** |
| `-regalloc-csr-first-time-cost=0..20` | 2636 | 841 | neutral |
| `-misched-postra` | 3502 | 912 | |
| `-disable-post-ra` | 13,614 | 2,310 | **17,180** |
| `-enable-deferred-spilling` | 16,231 | 1,541 | **17,092** |

The case-1 `enemyRace` post-RA MachineLICM hoist (region `0x1500..0x178e`,
300 raw bytes) remains the single big residual; every shape that blocks it
(`coords[10]`/`coords[8]`) grows the frame by 8 B and re-allocates the whole
function, so it is not landable size-exact.  The alienGuns loop body itself is
still 10 raw bytes worse than base because the crutch table select is 10 B
longer than the binary's `cbz` branch; the crutch is required for size 17,172.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13650
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2636 aligned=841 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13650/`
(`measure.sh`, `try.py`, `perm.py`, `perm2.py`, `region2.py`, `diffoff.py`,
`sec.py`, `Level.base.cpp`, `Level.faithful.cpp`, `base.so`, `cpsg.so`).
