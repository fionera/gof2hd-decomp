# `Status::missionCompleted` — wave 119 (w6935): drop the rank table, make
# `.rodata` byte-identical, keep the 1432 B function size

Status: **source change landed**.  Worktree `wt/w6935`, branch
`wave119-w6935`, base master `59184fd11` (wave 119).  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

## 1. What changed

`tools/mission_case_table.cpp` (the 192-byte mission-type -> case-rank table)
is deleted and `Status::missionCompleted` now switches directly on
`m->getType()` with the real mission-type case labels in the original body
order.  Two faithful source details are kept/restored so the function still
compiles to exactly 1432 B:

* body 166 re-queries `m->getProductionGoodIndex()` for the `hasEquipment`
  call (the original does this; the old rank-table source reused the cached
  `index`);
* body 168 keeps the `(unsigned short)` cast on `m->getStatusValue()` as a
  two-byte size device (the original has no `uxth`; without the cast the
  function is 1428 B and shifts `.text`).

`CMakeLists.txt` no longer builds or links the `mission_table` object.

## 2. Gate and structural result

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
cmake --build cmake-build-match --target drift -j6
# DRIFT: 0 field(s) across 0 class(es)
```

Section state after the change (original vs ours):

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 | exact (size/base) |
| `.rodata` | 0x41b67 | 0x41b67 | **exact, 0 differing bytes** |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | exact (size/base) |
| `.ARM.extab` | 0x26298 | 0x26290 | **-8 B (unchanged)** |
| `.data` | 0x296c | 0x296c | exact |
| file size | 2,187,328 | 2,187,328 | exact |

`.rodata` is now byte-identical: the 192-byte table that used to be appended
at `0x20b977` is gone.  `.text` size/base and the 27-section layout are
unchanged, and every function after `missionCompleted` stays at its original
address because the function is still 1432 B (`Status::setJumpgateUsed` is
still at `0xac4d0`).  `.ARM.extab` remains 8 B short (0x26290 vs 0x26298), the
known `Generator::createMission` LSDA delta; this pass does not touch it.

## 3. Function score trade-off

The function's fuzzy match drops **92.5 % -> 59.4 %** (1432 B -> 1432 B,
byte/linked still false).  This is expected: the rank table was a codegen
device that made clang 7's MachineBlockPlacement lay the big switch bodies
out in the original A-then-B order.  A plain `switch (m->getType())` emits the
big 40-entry TBH with the bodies in *value* order, so the body addresses and
the TBH entries differ from the original even though the dispatch
(`sub.w r1,r0,#150; cmp r1,#39; bhi; tbh`) and the body code are otherwise
faithful.  The function size is kept at 1432 B so the rest of `.text` does
not shift; without that, `byte_exact` collapses because every following
function's PC-relative branches move.

Measured variants (all with the same `link_so` + single-symbol score):

| variant | usize | pct | byte_exact |
|---|---:|---:|---:|
| rank table (before) | 1432 | 92.5 | 4505 |
| plain switch, original body order | 1424 | 58.4 | 3043 |
| plain switch, source order low/167/A/B | 1424 | 58.4 | 3043 |
| plain + re-query, no cast | 1428 | 59.5 | 4466 |
| **plain + re-query + cast 168** | **1432** | **59.4** | **4505** |
| plain + re-query + cast 184 | 1436 | 59.3 | 4466 |
| nested switch (`__builtin_expect` outer A, inner B, inner-low) | 1416-1524 | 28-30 | 4466/3043 |
| nested switch with dummy default cases | 1440-1448 | 29-30 | 4466/3043 |
| plain + extra case labels (A weight 2) | 1376 | 42-44 | 3043 |

The nested-switch experiments are the closest C++-only route to the original
A-then-B layout: an outer `switch (__builtin_expect(m->getType(), 150))` with
the A cases, an inner switch for B, and an inner-inner switch for the low
group.  In a small loop test clang merges the three switches and produces
A-then-B with weights A=78/B=6/low=1, but in the real function `SimplifyCFG`
keeps three separate switches (the bodies are too large), so the backend emits
a 40-entry table whose B entries point at a second B dispatch.  That is not
the original layout and costs 8-100+ bytes.  The rank table remains the only
way found so far to reproduce the original body order in plain C++.

## 4. Follow-up

`docs/VALIDATION.md` and `docs/IDENTICALITY.md` still describe the rank table
and the `+192` `.rodata` delta.  They need a follow-up update to say `.rodata`
is now exact and the rank table is gone; this pass deliberately does not touch
docs (the gate does not require it).

Reproduction: `/dev/shm/w6935/` (scratch) contains `score.py` (single-symbol
verify score), `Status_plain_current.cpp` (plain switch baseline),
`Status_nested*.cpp` (nested-switch variants), `Status_extralabels*.cpp`
(extra-case-label variants), and the objdump dumps used above.
