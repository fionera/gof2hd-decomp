# Small tail wave 155 (w10017): MGame store-site — st-ccm base landed
# (raw 318 -> 37); Gun r4/r5 swap re-probed, no win

Status: **one landed `src/` change** (`MGame::OnUpdate`, raw improves
318 -> 37). Worktree `wt/w10017`, base master `4417db0b4` (wave 153).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave154-w9917.md`, `small-tail-wave152-w9817.md`,
`small-tail-wave151-w9717.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
RATCHET PASS, avg 99.93. Both targets stay size-exact (1984 / 11440 B).

## 0. Residuals (raw same-address bytes / aligned instructions)

| target | size | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 463 | 57 | 57 |
| `MGame::OnUpdate` | 11440 B | 318 | **37** | 19 | 109 |

Mangled names: `_ZN3Gun22calcCharacterCollisionEv`, `_ZN5MGame8OnUpdateEv`.

## 1. MGame::OnUpdate — LANDED: use `st` for the radioBase ccm as well

The wave-154 note already measured this shape (`st` ccm+store via st, block
scope) at raw 37 / aligned 109 but did not land it.  It is the closest raw
shape found: the whole 11440-byte function is now byte-identical to the
original except for two `mov r0,r5` sites (the mission-122
`getCurrentCampaignMission` and `getPlayingTime` calls) and the branch-target
shifts they cause.

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = st->getCurrentCampaignMission() == 122 ? 3164 : 3166;
this->level->createRadioMessage(28, radioIndex + radioBase);
reinterpret_cast<long long &>(Globals::status->field_0x100) = st->getPlayingTime();
```

The original uses `ldr.w r0,[r9]` at both sites (r9 = `&Globals::status`),
so the remaining delta is exactly `mov r0,r5` (2 B) at each site plus the
2-byte branch displacements.  The raw diff falls from 318 to 37.

Trade-off: the aligned metric rises 19 -> 109 because the two 2-byte movs
shift the literal-pool and branch relocation offsets; `align_diff.py`
normalises branch targets to instruction indices but not literal-pool words.
The raw metric is the byte-matching metric, so this is landed as the new
base.  Byte_exact is unchanged (the function is still not exact).

### Why the mov cannot yet be removed

The mov exists because `st` must stay live across `createRadioMessage` to
force `&Globals::status` into r9.  Replacing either call receiver with
`Globals::status` makes `st` dead at the call and the allocator
rematerialises the GOT address into r5 (function shrinks to 11392 B):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (radioBase direct, store via st) | 318 | **19** | 11440 |
| **landed: radioBase via st, store via st** | **37** | 109 | 11440 |
| radioBase via st, store direct | 914 | 109 | 11436 |
| both direct, `(void) st` | 5148 | 597 | 11392 |
| store base `st`, call direct | 5253 | 761 | 11440 |
| `st` before chain, used in inAlienOrbit cond, store direct | 589 | 354 | 11440 |

The chain-scope variant does remove both movs from the mission-122 block
(it jumps to the shared tail at `ldr r0,[r9]`), but the compiler then also
routes the mission-23 `getCurrentCampaignMission` through r5, adding a mov
there and shifting the whole chain (raw 589).  A source shape that keeps
`&Globals::status` in r9 without a live `Status*` value in r5 is still open.

## 2. Gun::calcCharacterCollision — no win; confirmed the swap is a single
##    greedy-allocation choice

HEAD remains 463 / 57 / 1984 B.  Reproduced the wave-154 inline-`getPosition`
shape (467 / 67): it gives the original stack layout (`getPosition` sret at
sp+44, subtraction temp at sp+104) but keeps `i*3` in r5 and the delta
address in r4, the reverse of the original (i*3 r4, sret r4, delta r5).

New MIR evidence: the `i*3` virtual register (`%99` / `%526`) is defined at
slot 2880B and is live to the end of the i-loop body (saved to r6 across
calls).  The greedy allocator assigns it r5, even though the block live-ins
are only r8/r9/r11.  All downstream differences (k-loop recomputes
`add r0/r1/r4, sp, #104` instead of keeping the view address in r5; the
isAsteroid/mine blocks reload i instead of recomputing sp+104) are
consequences of that one register choice.

New source shapes measured this wave (all 1984 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD (named `playerPosition`) | **463** | **57** |
| inline `getPosition`, direct subtraction | 467 | 67 |
| inline + named `Vector delta` | 467 | 67 |
| inline + `Matrix translated;` before view | 467 | 67 |
| explicit `unsigned i3 = i*3` used for both GEPs | 463 | 57 |
| explicit `int i3 = i*3` used for both GEPs | 463 | 57 |
| `int i` loop variable | 464 | 58 |
| `size_t i` loop variable | 463 | 57 |
| `positions[i]` / `velocities[i]` operator[] | 463 | 57 |
| `*(i + data)` / `*(data + i)` | 463 | 57 |
| hoist `Vector *posData/velData` | 1649 | 182 |
| `const Vector &position/velocity` | 1732 | 391 |
| `Matrix &view` | 1197 | 152 |

The explicit `i3` forms fold straight back into the same `t2ADDrs` GEP
scaling, so they do not change the allocation.  Forcing `i*3` into r4
without changing the 1984-byte size is still open.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10017
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10017/fdiff.py <mangled>
python3 /dev/shm/w10017/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w10017/`: `fdiff.py`, `align_diff.py`,
`cc.sh`, `sweep_gun.py`, `sweep_mg.py`, `run1..7.py`, `runmg1..5.py`,
`Gun.base.cpp`, `MGame.base.cpp`, `gun.*.asm`, `mg.*.asm`, `gun.fn.txt`,
`gun.inl.all.txt`, `ARMBaseRegisterInfo70.cpp`, `ARMRegisterInfo.td`,
`RegAllocGreedy.cpp`.

## 4. Files changed

* `src/game/menu/MGame.cpp` — mission-122 `radioBase` now reads
  `st->getCurrentCampaignMission()` instead of `Globals::status->...`.
  Size 11440 unchanged; raw 318 -> 37, aligned 19 -> 109.
* `_work/reconstructions/small-tail-wave155-w10017.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, avg 99.93.
