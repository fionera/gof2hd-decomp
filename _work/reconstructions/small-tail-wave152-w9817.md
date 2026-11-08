# Small tail wave 152 (w9817): Generator bonus spelling landed
# (raw 445 -> 393, aligned 59 -> 52); Gun / MGame residuals re-probed

Status: **one landed `src/` change** (`Generator::createMission`, both metrics
improve). Worktree `wt/w9817`, base master `9d491d46a` (wave 151). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave151-w9717.md`, `small-tail-wave150-w9617.md`,
`small-tail-wave149-w9516.md`, `gun-calccharactercollision-wave133-w7739.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
RATCHET PASS, avg 99.93.  All three targets stay size-exact.

## 0. Residuals (raw same-address bytes / aligned instructions)

| target | size | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 463 | 57 | 57 |
| `Generator::createMission` | 1996 B | 445 | **393** | 59 | **52** |
| `MGame::OnUpdate` | 11440 B | 318 | 318 | 19 | 19 |

Mangled names:
`_ZN3Gun22calcCharacterCollisionEv`,
`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`,
`_ZN5MGame8OnUpdateEv`.

## 1. Generator::createMission — LANDED: `rawBonus + rawBonus - bonus`

The faithful bonus arithmetic is `bonus = rawBonus - rem`.  Written that way
clang emits `mov r9,r1` (rem kept in r9) and evicts `agent` (frame 104); the
original instead spills `rem` to `[sp,#32]` and keeps `agent` in r9 (frame 96).

New spelling that is size-exact and improves both metrics:

```cpp
int rem = rawBonus % 50;
bonus = rawBonus + rem;
if (bonus % 50 != 0) {
    bonus = rawBonus + rawBonus - bonus;   // == rawBonus - rem
}
```

Codegen: `rsbne r5,r5,r6,lsl #1` (the `rawBonus*2 - bonus` form) instead of
HEAD's `subne r5,r5,r0` on the algebraic `bonus -= rem*2`.  Size stays 1996 B.
Raw 445 -> **393**, aligned 59 -> **52**.

Variants measured this wave (all 1996 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD (`bonus -= rem * 2`) | 445 | 59 |
| **`bonus = rawBonus + rawBonus - bonus` (LANDED)** | **393** | **52** |
| faithful `bonus = rawBonus - rem` | 304 | 112 (2000 B) |
| `bonus -= rem; bonus -= rem;` | 304 | 112 |
| `bonus = rawBonus + (rawBonus - bonus)` | 304 | 112 |
| `bonus = rawBonus - (bonus - rawBonus)` | 304 | 112 |
| `bonus -= -rem * 2` | 304 | 112 |
| `int r2 = rem; bonus -= r2*2` | 304 | 112 |
| `unsigned` rem | 304 | 112 |
| `rem` computed inside the `if` | 304 | 112 |
| `agent` cached in a local | 304 | 112 |
| `bonus = rawBonus - rem` with `rem` const | 304 | 112 |

The exact original sequence (`str r1,[sp,#32]` spill of `rem`, then
`subne r5,r6,r0` reload) is still not reproduced; the landed form is the
closest size-exact spelling.

## 2. Gun::calcCharacterCollision — no win; root cause narrowed to
##    `StackColoring` lifetime overlap, not the sret hoist itself

Confirmed with the full pass dump (`-mllvm -print-after-all
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv`, kept in
`/dev/shm/w9817/gun_all.txt`):

* `%stack.10` is shared by `playerPosition` (12 B) and `view` (60 B);
  `%stack.11` is the discarded `MatrixSetTranslation` sret temp (60 B).
* The `Merge disjoint stack slots` (`StackColoring`) pass sorts slots by size
  descending and merges the first smaller slot that is lifetime-disjoint into
  the larger one.  In our build `playerPosition` (fi#4) is disjoint from
  `view` and is merged into fi#10; `delta` (fi#5) then overlaps
  `playerPosition` and lands in fi#11.
* The original has the **opposite** assignment: `playerPosition` at `sp+44`
  (the `MatrixSetTranslation` temp slot), `delta` at `sp+104` (the `view`
  slot).  That is only possible if `playerPosition`'s lifetime **overlaps**
  `view`'s, i.e. its lifetime.start is before the `view` block.
* The downstream `r4 = &stack.10` hoist in bb.43 (`isAsteroid`) and the
  `i*3`-in-r5 / `ldr fp` swap are *consequences* of that one slot decision.

Variants measured this wave (all 1984 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD | 463 | 57 |
| inline getPosition temp in the subtraction | 467 | 67 |
| named `Vector delta = enemyPosition - playerPosition` | 463 | 57 |
| `const Vector playerPosition` | 463 | 57 |
| `PlayerEgo *pl = getPlayer(); Vector pp = pl->getPosition()` | 463 | 57 |
| `Vector enemyPos = enemyPosition` copy | 463 | 57 (1956 B) |
| `Matrix translated = MatrixSetTranslation(...)` | 463 | 57 |
| `float *cam = CameraGetLocal(...); Matrix view = *(Matrix*)cam` | 463 | 57 |
| `Matrix tmp;` dummy before the call | 463 | 57 |
| `Vector playerPosition;` at i-loop scope + assign | 1547 | 292 (1964 B) |
| `Matrix view;` at i-loop scope + assign | 1831 | 348 (2024 B) |
| `view = MatrixSetTranslation(view, ...)` | 1087 | 101 (1992 B) |
| `Matrix view; view = *(...)` | 1817 | 243 (2056 B) |

Remaining hypothesis (not yet measured): make `playerPosition`'s
lifetime.start precede the `view` block *without* adding a copy — e.g. a
function-scope `Vector playerPosition;` whose only assignment is in the
scatter branch, or a `Matrix`-typed local declared before it to shift the
stack-object order.  Both need the frame to stay 208 B.

## 3. MGame::OnUpdate — no win; the store must merge with the 23/24 tail

The 19 aligned residual is still the mission-122 store.  The original's store
is the *same* statement as the mission-23/24 blocks, so the compiler merges
all three into the shared tail at `0x17e26c`
(`ldr.w r0,[r9]; blx getPlayingTime; ldr.w r2,[r9]; strd r0,r1,[r2,#256]`).
Our `Status *st` value anchor is what keeps the GOT base `%1053` in `r9`
(wave 150), but it also makes the store `st->getPlayingTime()` non-identical,
so it cannot merge and emits `mov r0,r5` (2 bytes shorter, shifting the block).

Variants measured this wave (all 11440 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD (anchor at pt store) | **318** | **19** |
| anchor + direct store, `st` used in `radioBase` | 914 | 109 (11436 B) |
| anchor + direct store, `st` unused (dead) | 5148 | 597 (11392 B) |
| `st->field_0x100 = Globals::status->getPlayingTime()` | 5253 | 761 (11492 B) |

The anchor must be live *at* the store to keep `r9`, but the store must be
direct to merge — these are mutually exclusive in straight-line code because
the store is the last statement of the block.  A variant where the anchor is
live *past* the store (e.g. a use in the following `inAlienOrbit` block) is
the remaining untested shape.

## 4. Files changed

* `src/game/mission/Generator.cpp` — bonus correction now spelled
  `bonus = rawBonus + rawBonus - bonus;` (arithmetically `rawBonus - rem`).
  Size 1996 unchanged; raw 445 -> 393, aligned 59 -> 52.
* `_work/reconstructions/small-tail-wave152-w9817.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, avg 99.93.

Scratch tools and full pass dumps live in `/dev/shm/w9817/`
(`gun_all.txt`, `ssc.cpp`, `sc.cpp`, `gun_sweep6..9.py`).
