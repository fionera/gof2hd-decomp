# Small tail wave 150 (w9617): Gun / Generator / MGame::OnUpdate —
# MGame anchor moved to the store site (aligned 109 -> 19, raw 37 -> 318)

Status: **one landed `src/` change** (`MGame::OnUpdate`, metric-only on the
instruction-alignment metric). Worktree `wt/w9617`, base master `7eed2966e`
(wave 149). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave149-w9516.md`, `mgame-onupdate-wave149-w9519.md`,
`gun-calccharactercollision-wave133-w7739.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
RATCHET PASS, avg 99.93.  All three targets stay size-exact.

## 0. Residuals (raw same-address bytes / aligned instructions)

| target | size | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 463 | 57 | 57 |
| `Generator::createMission` | 1996 B | 445 | 445 | 59 | 59 |
| `MGame::OnUpdate` | 11440 B | 37 | **318** | 109 | **19** |

Mangled names:
`_ZN3Gun22calcCharacterCollisionEv`,
`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`,
`_ZN5MGame8OnUpdateEv`.

## 1. MGame::OnUpdate — LANDED: anchor moved from the ccm site to the store site

Wave 149 (`w9519`) established that the mission-122 `Status *st` anchor is the
only source spelling that keeps the GOT base `%1053` in `r9` (the original),
but its `mov r0,r5` at the `getCurrentCampaignMission` (ccm) site shifts the
whole 34-byte mission-122 block and leaves the 37-byte / 109-aligned residual.

New hypothesis this wave (from the mission): use `Globals::status` **directly**
at the ccm site and keep the anchor only for the `getPlayingTime` (pt) store.
This is the exact source:

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
this->level->createRadioMessage(28, radioIndex + radioBase);
reinterpret_cast<long long &>(Globals::status->field_0x100) = st->getPlayingTime();
```

Result: **11440 B, raw 318, aligned 19**.  The ccm site now matches the
original exactly (`ldr.w r0,[r9]` at `0x28ba`); the only remaining structural
difference is the store site, where ours emits `mov r0,r5; b.w getPlayingTime`
instead of the original's shared tail `b.w 0x17e26c`
(`ldr.w r0,[r9]; blx getPlayingTime; ldr.w r2,[r9]; strd r0,r1,[r2,#256]`).
The raw count jumps because the single extra `mov` at `0x28dc` shifts every
instruction after it by 2 bytes; the instruction-index alignment normalises
that shift and falls from 109 to 19.

The raw/aligned trade-off is real and is the reason this is a **metric-only**
landing: the gate's `byte_exact`/`linked_exact` are unchanged (the function is
non-exact either way) and `avg` stays 99.93.

Variants measured this wave (all 11440 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD (anchor at ccm + pt) | **37** | **109** |
| **ccm direct, anchor at pt (LANDED)** | **318** | **19** |
| `Status &st = *Globals::status` reference | 37 | 109 |
| anchor after `radioIndex` | 922 | 127 |
| ccm direct, pt direct (no anchor; GOT splits) | 5148 | 597 |
| anchor at ccm, pt direct | 914 | 109 |

`Status &st` is IR-identical to the pointer form.  The reference form cannot
remove the store-site `mov` either: the anchor value still lives in `r5`.

Open question: the original's store site reloads `Globals::status` twice
(call + store target) from the GOT.  A spelling that keeps `%1053` in `r9`
*without* a live `Status*` value in `r5` would make both sites match; the
wave-149 candidate (a rematerialisable local inside `%1053`'s interval) is
still untested.

## 2. Gun::calcCharacterCollision — no source win; new probes all worse

Re-confirmed HEAD = **1984 B, raw 463, aligned 57**.  The residual is the
`i*3` register (`r4` original vs `r5` ours) plus the k-loop block placement
(original rotated, ours top-tested).  The mission's hint is that the `i*3`
swap is the k-loop sret-address hoist (`add.w r4,fp,fp,lsl #1`): the hoisted
`%stack.10` address occupies `r4` across the i-loop, so `i*3` is forced into
`r5`.

New hypotheses measured this wave (all 1984 B):

| variant | raw | aligned |
|---|---:|---:|
| HEAD | **463** | **57** |
| `expr` (inline `getPlayer()->getPosition()` into `operator-`) | 467 | 67 |
| `expr` with operands reversed (`pp - enemyPosition`) | 468 | 67 |
| named pp, `VectorLength(playerPosition - enemyPosition)` | 465 | 59 |
| `const Vector &playerPosition` / `auto` | 463 | 57 |
| named `Vector delta = enemyPosition - playerPosition` | 463 | 57 |
| k-loop temp named (`Vector enemyPos = enemy->getPosition(); *td = enemyPos;`) | 463 | 57 |
| k-loop `Vector &enemyPos = *td` | 463 | 57 |
| k-loop `enemies->data()[k]` / `int k` | 463 | 57 |
| k-loop `for (k=0,n=size; k<n; ++k)` | 1691 | 266 |
| k-loop `__builtin_expect(k < size, 1)` | 1764 | 355 |
| k-loop `__builtin_expect(k < size, 0)` | 1677 | 303 |
| `if (size != 0) for (...)` | 1774 | 279 |
| function-scope `Vector kpos` reused in k-loop | 1814 | 292 |
| `__builtin_expect(scatter, 1)` | 1601 | 119 |
| `__builtin_expect(geometries != nullptr, 0)` | 1364 | 379 |
| `__builtin_expect(isAsteroid(), 0)` | 1807 | 352 |
| `__builtin_expect(empDamage != kNoAttribute, 0)` | 1348 | 99 |
| `__builtin_expect(ki guard, 0)` | 1778 | 305 |

The `expr` form gets the shipped stack-slot colours (pp `sp+44`,
delta/k-loop `sp+104`) but the wrong hoist order (`%stack.10` first -> `r4`);
the `stmt` form (HEAD) gets the right hoist order but merges pp with the
k-loop slot.  Every spelling that keeps pp as a named local is 463/57; every
spelling that makes pp a temporary is 465-468/67.  No source form produced
shipped slots **and** shipped registers.

## 3. Generator::createMission — no source win; `rem` still refuses to spill

Re-confirmed HEAD algebraic `bonus -= rem * 2` = **1996 B, raw 445, aligned
59**.  The faithful `bonus = rawBonus - rem` (the original's arithmetic:
`adds r5,r1,r6` then `subne r5,r6,r0`) is 2000 B, raw 304, aligned 112: the
register allocator moves `rem` to `r9` (`mov r9,r1`) and spills `agent`
instead, while the original spills `rem` to the dead `[sp,#32]` and keeps
`agent` in `r9` (frame 96 vs 104).

New hypotheses measured this wave (all 2000 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD algebraic (`bonus -= rem * 2`) | **445** | **59** |
| faithful (`bonus = rawBonus - rem`) | 304 | 112 |
| faithful, `bonus -= rem + rem` | 445 | 59 |
| faithful, `bonus = bonus - rem - rem` | 304 | 112 |
| faithful, `int check = bonus % 50; if (check != 0)` | 304 | 112 |
| faithful, `if (bonus % 50)` | 304 | 112 |
| faithful, `bonus = rawBonus; bonus -= rem;` | 304 | 112 |
| faithful, `int &r = rem` | 304 | 112 |
| faithful, `const int rem` | 304 | 112 |
| faithful, `unsigned rem` | 304 | 112 |
| `int bonus = 0; if ((type|4) != 12) {...}` | 304 | 112 |
| ... algebraic variant of the above | 445 | 59 |
| `__builtin_expect((type|4)==12, 0)` | 386 | 142 | (2004 B) |
| `__builtin_expect((type|4)==12, 1)` | 1801 | 176 | (1996 B) |
| `__builtin_expect(bonus % 50 != 0, 0/1)` | 304 | 112 |

The allocator's eviction is weight-driven: `agent`'s live interval is huge
(whole function) so its spill weight is low, and `tryEvict` evicts it for
`rem` (short, hot block).  The original's bonus block must have been cold
enough (PGO branch weight) that `rem`'s weight fell below `agent`'s and it
spilled instead.  `__builtin_expect((type|4)==12, 1)` does make `rem` spill
(`str r1,[sp,#32]` / `subne r5,r6,r0` reappear) but it also reorders the
bonus and reward-rem blocks and costs 1801 raw bytes.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9617
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9617/fdiff.py <mangled>
python3 /dev/shm/w9617/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w9617/`: `fdiff.py`, `align_diff.py`,
`adiff.py`, `norm.py`, `ccgun.sh`, `ccgen.sh`, `gunvar*.py`, `genvar*.py`,
`mgvar.py`, `Gun.base.cpp`, `Generator.base.cpp`, `MGame.base.cpp`.

## 5. Files changed

* `src/game/menu/MGame.cpp` — mission-122 body: `radioBase` now reads
  `Globals::status->getCurrentCampaignMission()` directly; the `Status *st`
  anchor is kept only for the `getPlayingTime` store.  Size 11440 unchanged;
  raw 37 -> 318, aligned 109 -> 19.
* `_work/reconstructions/small-tail-wave150-w9617.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
