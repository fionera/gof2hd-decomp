# Radar::draw wave 191b (w13732) — asteroid timer / planet / music / otherLock order: aligned 1805 → 1742

**Status:** complete, 2026-11-13. Worktree `wt/w13732`, branch `wave191b-w13732`,
base `master` `dc50e1082`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13730.md` (landmark/gas/bar store order, the
current base), `-w13727.md`, `-w13720.md`, `-w13717.md`, `-w13714.md`,
`-w13699.md`, `-w13689.md`, `-w13678.md`, `-w13669.md`, `-w13665.md`,
`-w13660.md`, `-w13656.md`.

**Landed:** seven independent statement/condition-order reconstructions, all
size-neutral in combination (9,908 B exact):

1. **`A`** — asteroid candidate timer reset.  In `if (best > -1)` the original
   (`0x12fd74`-`0x12fd94`) stores 0 to `asteroidLockTimer` (`strne [r4,#412]`),
   not `+= mode`.
2. **`C`** — asteroid lock-timer add before the threshold.  Original
   `0x12fe34`-`0x12fe48` loads `field_0x1b8`, `subs #200`, adds `mode`, stores
   `asteroidLockTimer`, then compares.  The source now does
   `auto hasScanner = hasScanner19; asteroidLockTimer += mode;` before the
   `> field_0x1b8 - 200` test, and no longer clears `lockedAsteroid` there.
3. **`P`** — planet store order: `planetDockIndex = i;` before
   `field_0x18 = planetTargets->data()[i]; planetHit = true;`.  Original
   `0x12e594`-`0x12e5b4` stores `[r4,#24]` then `[r4,#64]`.
4. **`M`** — music outer condition order: range, bitmask, then
   `current != 151 && alwaysEnemySeen`.  Original `0x130280`-`0x1302a4`.
5. **`T`** — asteroid tractor condition order:
   `tractorMode2 && field_0x1c == nullptr && wreck`.
6. **`CARGO`** — `__builtin_expect(cargo == nullptr, 0)` around the locked-enemy
   cargo scan.  Original `0x130146`-`0x130152` branches far to `hudEvent(22)`.
7. **`OL`** — `otherLock` operand order: `dockNavPtr && lockedEnemy == nullptr`
   first, then `field_0x1c`, `lockedStation`, `field_0x14`.  Original
   `0x12f40e`-`0x12f424`.
8. **`MUS`** — music inner condition order: `inAlienOrbit()`,
   `getStation()->isAttackedByAliens()`, `getCurrentCampaignMission() == 16`.
   Original `0x1302a8`-`0x1302d6`.

`raw` 6,208 → 6,434 (+226), `aligned` 1,805 → **1,742** (−63), size stays
9,908/9,908, `.text` differing bytes 72,647 → 72,873 (+226), `.ARM.extab` 564
(unchanged), `.ARM.exidx` / `.rodata` / `.rel.plt` 0 / 0 / 0.  Full `check`
gate green (byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843,
imports 207, lint CLEAN, parity 5370/5370, drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,208 |
| instruction-index alignment | 1,805 |
| function size | 9,908 / 9,908 |
| `.text` differing bytes | 72,647 |
| `.ARM.extab` differing bytes | 564 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |

Build: `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Per-cell measurements (all size 9,908 unless noted)

| cell | raw | aligned | notes |
|---|---:|---:|---|
| base | 6,208 | 1,805 | |
| `P` only | 6,209 | 1,804 | small win |
| `M` only | 6,231 | 1,803 | small win |
| `T` only | 6,208 | 1,805 | neutral |
| `A+C` | 6,384 | 1,800 | |
| `A+C+P` | 6,385 | 1,799 | |
| `A+C+M` | 6,407 | 1,798 | |
| `A+C+P+M` | 6,408 | 1,797 | |
| `+T` | 6,410 | 1,787 | |
| `+CARGO` | 6,436 | 1,750 | big aligned win |
| `+OL` | 6,434 | 1,747 | |
| `+MUS` | **6,434** | **1,742** | landed |

## 2. Rejected cells (measured, reverted)

| cell | raw | aligned | size | note |
|---|---:|---:|---:|---|
| `MUS` with `expect(...,1)` on `current != 151 && alwaysEnemySeen` | 6,434 | 1,742 | 9,908 | neutral |
| `MUS` with no expect on that subcondition | 6,537 | 1,734 | **9,904** | −4 fatal |
| `field_0x1bc >= 1` expect removed | 6,545 | 1,672 | **9,912** | +4 fatal |
| `field_0x1bc >= 1` expect `0` | 6,575 | 1,822 | 9,908 | worse |
| enemy-lock first condition de-expect (both) | 6,987 | 1,654 | **9,912** | +4 fatal |
| enemy-lock first condition de-expect (first only) | 7,002 | 1,656 | **9,912** | +4 fatal |
| enemy-lock second condition de-expect | 6,437 | 1,742 | 9,908 | neutral |
| `bossSeen = enemy->shipGroup == 10` → `if (== 10) bossSeen = true` | 9,239 | 2,156 | **9,980** | +72 fatal |
| `missionBlocksPlanets` expect on previous best | 7,132 | 1,832 | 9,908 | worse (w13732 handoff) |
| `campaign` expect 1 | — | 2,228 | **9,952** | +44 fatal |
| `enemyFlagsLo` expect 1 | — | 1,971 | **9,900** | −8 fatal |
| plain `if (enemyFlagsLo)` | — | 1,857 | **9,912** | +4 fatal |
| `A` only | 9,062 | 2,080 | **9,888** | −20 fatal |

## 3. Remaining largest aligned clusters (best, aligned 1,742)

Instruction-index alignment (`align.py`), merged diff blocks:

1. `insert` orig 3,397 / ours 3,334-3,392 `~58` — music tail: our `else`
   branch (`field_0x54 = 0` … ambient switch … `stop` … `play(145)`) is laid
   out at `0x1304fa`-`0x130582`, the original at `0x1302f0`-`0x130324`
   (immediately after the if-body's `play(136)` and before the shared tail at
   `0x130326`).
2. `replace` orig 2,838-2,885 `~47` — enemy-lock cold block.  Original keeps
   the asteroid `hasScanner19`/`lockConfirmed`/`isPlaying(0)`/`play(26)` block
   at `0x12ff4c`-`0x12ff8c` interleaved between the enemy-lock compare and the
   blip block; ours places it at `0x130006`-`0x13007e`.
3. `replace` orig 2,941-2,942 `~46` — the same cold block's other end
   (`0x130082` vs ours `0x130002`).
4. `replace` orig 603-609 `~36` — planet `campaignMission == 125` freighter
   block: original normalizes `marked` (`cmp #0; it ne; movne #1;
   str [sp,#132]`) *before* `cmp #125` (`0x12e67a`-`0x12e686`); ours does it
   after the block (`0x12e6c2`).
5. `replace` orig 563-594 `~31` — same freighter block body.
6. `delete` orig 3,148-3,179 `~31` — the original's 20-byte Thumb jump table
   at `0x1302dc`-`0x1302f0` (ours at `0x130338`).
7. `replace` orig 371-393 `~22`
8. `replace` orig 854-876 `~22`
9. `delete` orig 1,623-1,645 `~22`
10. `replace` orig 3,349-3,350 `~22`

## 4. Known register-allocation artefacts

- `campaign` is kept in `sl` from `0x12e0f8` to `0x12e38a` in the original;
  ours spills it to `sp+0x90`.  This is the dominant raw-diff cause.  Source
  reorder attempts have not moved it.
- `Globals::Canvas` is in `fp` (ours) vs `r5` (original).
- `inSight` is in `fp` (ours) vs `r6` (original).
- `bossSeen` materializes via `subs/clz/lsrs` (ours) vs `cmp/it/moveq`
  (original); the `if (== 10) bossSeen = true` spelling that would fix it
  changes the function size (+72).
