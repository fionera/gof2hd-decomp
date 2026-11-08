# `Level::createShip` — wave 152 / worker w9816: status→r4 with the
# hitpoints-first order is *not* reachable at 2,764 B from any source shape
# tried; the closest is +4 B (2,768).  Note-only.

Status: **note-only** (no `src/` change landed; master body kept).
Gate on the unchanged tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  Worktree `wt/w9816`, base master `9d491d46a` (wave 151).

Target `_ZN5Level10createShipEiiiP8Waypointbb`, original 2,764 B @ `0xc0214`.
Baseline (master, `wy wz wx`, hitpoints-first): raw **2,056**, aligned **480**,
size 2,764.  Wave-151's `empPoints`-first + `expect(shipClass==1,1)` shape is
2,764 B, raw 2,146, aligned 501, status r4 — but its binary order is
`getLevel(empPoints)` before `gameWon()` (c029c vs c02b2), not the original's
`gameWon()` (c029a) then `getLevel(empPoints)` (c0314).

## What was swept (all full link, exact TU, `-Oz`, clang 7.0.2)

### Mission's suggested levers, hitpoints-first (all 2,764 B unless noted)

| variant | raw | size | aligned | status |
|---|---:|---:|---:|---|
| master (baseline) | 2,056 | 2,764 | 480 | r6 |
| `expect(shipClass==1,1)` alone | 2,303 | 2,768 | 487 | r6 |
| `expect(shipClass==1,0)` alone | 2,349 | 2,768 | 482 | r6 |
| `Status *st` all uses | 2,488 | 2,768 | 498 | r5 |
| `Status *st` mission only | 2,056 | 2,764 | 480 | r6 (coalesced) |
| `Status *st` late (inAlienOrbit+hardCoreMode), declared at top | 2,291 | 2,764 | 487 | **r4** |
| `do { mission = …; } while (mission == -1);` | 2,417 | 2,788 | 705 | **r4** |
| `while (mission == -1) { … }` | 2,417 | 2,788 | 705 | **r4** |
| `do/while` around all entry calls | 2,056 | 2,764 | 480 | r6 (coalesced) |

`Status *st` used only for `inAlienOrbit`+`hardCoreMode` is the only faithful
lever that reaches status r4 at 2,764 B — but it re-rolls the whole entry map
(`this`+`race` spilled, `type` r9, `mission` sl) and costs raw 2,291 / aligned
487.  `expect(shipClass==1,1)` alone does **not** move status off r6.

### The r4 producers are all non-size-exact

Placing a (non-witnessed) null guard `if (Globals::status == nullptr) { … }`
after `setEmpData` reproduces status r4 (wave-148's lever) but every body
overshoots 2,764:

| guard body (after `setEmpData`) | raw | size | aligned |
|---|---:|---:|---:|
| `race = 0;` | 2,432 | 2,772 | 479 |
| `hostile = false;` | 2,424 | 2,768 | 530 |
| `return nullptr;` | 2,441 | 2,780 | 481 |
| `group = false;` | 2,425 | 2,788 | 431 |
| `type = 0;` | 2,429 | 2,780 | 501 |
| `empPoints = 0;` / `mission = 0;` | 2,056 | 2,764 | 480 (coalesced, r6) |

Guard after the coordinate block: `group = false;` 2,442 / 2,792 / **428**;
`return nullptr;` 2,470 / 2,788 / 462.  Combining the guard with
`expect(mission==49..56,1)` (a −8 B lever on the bare body) still lands at
2,768–2,776, never 2,764.

### `__builtin_expect` on the 4 main sites (single + pairs)

On the hitpoints-first body, every expect that reaches status r4 changes the
size: `expect(mission==0x9a,*)` 2,804/2,812; `expect(shipClass==0,*)` 2,724/728
(codegen collapse); `expect(type==0x2c/0x31/0x33,*)` 1,912;
`expect(race==0,*)` 1,952/1,964; `expect(hostile,*)` 2,756/2,752;
`expect(mission==49..56,*)` 2,756.  `expect(mission==0x9a,1)` on the
`iif` (int x, int y, float z) coordinate shape is the only 2,764 B expect
shape with a big raw win — raw **1,455** — but aligned jumps to **850**
(the branch inversion moves whole blocks) and the official fuzzy drops.

### Coordinate-type permutations (current `wy wz wx` order)

| x/y/z | raw | size | aligned |
|---|---:|---:|---:|
| `int x, float y, float z` (`iff`) | 2,055 | 2,764 | 480 |
| `float x, int y, float z` (`fif`) | 2,057 | 2,764 | 494 |
| `float x, float y, int z` (`ffi`) | 2,056 | 2,764 | 496 |
| `int x, int y, float z` (`iif`) | 1,682 | 2,768 | 486 |
| `int x, int y, int z` (`iii`) | 2,386 | 2,792 | 463 |
| `float x, float y, float z` | 2,392 | 2,784 | 556 |

The original converts **all three** coordinates to float at the constructor
(`vcvt.f32.s32 s20/s18/s16` at `c0412/c041a/c0422`), which points at
`int x, y, z`; but `iii` is +28 B and does not reach status r4.

## Conclusion / open item

No source shape tried keeps **both** the original status-r4 map and the
original hitpoints-first binary order at exactly 2,764 B.  The closest
status-r4 hitpoints-first shapes are +4 B (`expect(shipClass==1,1)` +
`guard(hostile=false)` 2,768 / raw 2,423 / aligned 529; guard `hostile=false`
alone 2,768 / raw 2,424 / aligned 530).  The r4 state is a genuine RA state
(guard-after-coords reproduces the exact original map at 2,792–2,800), so the
remaining gap is the original's PGO branch weights, which no flat
`__builtin_expect` combination on this body reproduces at the exact size.

Master body kept (raw 2,056 / aligned 480 / 2,764 B); landing a +4 B variant
would break the 4,524/4,524 size-exact gate.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9816/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w9816/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
```
Scratch `/dev/shm/w9816/`: `harness.py` (body splice + build + raw/aligned +
entry-map), `regs.py` (status-register extractor), `batch.py`/`batch2.py`/
`batch3.py` (levers), `sweep.py`…`sweep5.py` (guards / expects / coordinate
types / combinations), `quick.py`/`quick2.py` (last combos).
