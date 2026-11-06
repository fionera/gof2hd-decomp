# `Status::missionCompleted` — wave 121 (w7113): remove the manually linked
# 192-byte rank table, make `.rodata` byte-identical, plain type switch

Status: **source change landed**.  Worktree `wt/w7113`, base master
`bd061a489` (wave-120 docs).  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | before (HEAD `bd061a489`) | after (this pass) |
|---|---:|---:|
| function score | **92.454632 %** | **59.372027 %** |
| function size | 1432 B | 1432 B |
| `.rodata` size | 0x41c27 (269,351, +192) | **0x41b67 (269,159, exact)** |
| `.rodata` bytes | 192 B rank table appended | **byte-identical (0 diff)** |
| `.text` size/base | 0x1337c8 @ 0x69440 | **0x1337c8 @ 0x69440** |
| `.text` content diff | 88,116 B | 88,549 B |
| `.ARM.extab` | 0x26294 (4 B short) | 0x26294 (4 B short) |
| gate | byte 4505 / linked 4505 | **byte 4505 / linked 4505** |

Everything else is unchanged: `.ARM.exidx` size/base, `.data`, `.bss`,
`.got`, `.data.rel.ro`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`,
`.rel.plt`, 27 sections, exact file size 2,187,328 B, imports 207,
data 843/843, lint CLEAN, drift 0.

## 1. What changed

The manually linked table `tools/mission_case_table.cpp` (a 192-byte
`unsigned char[192]` mission-type -> case-rank table, linked as the last
`.rodata` input and read through the fixed address `0x20b977`) is deleted.
`CMakeLists.txt` no longer builds or links the `mission_table` object.

`Status::missionCompleted` now switches directly on the int returned by
`m->getType()` with the real mission-type labels in the original body order:

```cpp
switch (m->getType()) {
    case 167:
    case 174: ...
    case 8:   ...
    case 0:
    case 11:  ...
    case 13:  ...
    case 14:  ...
    case 150: ...
    case 151: ...
    ...
    case 189: ...
    case 156: ...
    ...
    case 172: ...
}
```

The two faithful source details from the w6935 plain-switch tree are kept:
body 166 (case 10 in the old numbering) re-queries
`m->getProductionGoodIndex()` for the `hasEquipment` call, and body 168
(case 20) keeps the `(unsigned short)` cast on `m->getStatusValue()` as a
two-byte size device.  Without the cast the function is 1428 B and every
following `.text` symbol shifts, collapsing `byte_exact` from 4505 to 4466.

## 2. Gate / structural result

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
cmake --build cmake-build-match --target drift -j6   # DRIFT: 0 field(s)
cmake --build cmake-build-match --target lint  -j6   # CLEAN
```

Section table measured after the change (original vs ours):

| section | original | ours | |
|---|---:|---:|---|
| `.text` | 0x69440 / 0x1337c8 | 0x69440 / 0x1337c8 | exact size/base |
| `.rodata` | 0x1c9e10 / 0x41b67 | 0x1c9e10 / 0x41b67 | **exact, 0 differing bytes** |
| `.ARM.exidx` | 0x19cc08 / 0x6f70 | 0x19cc08 / 0x6f70 | exact size/base |
| `.ARM.extab` | 0x1a3b78 / 0x26298 | 0x1a3b78 / 0x26294 | **-4 B (unchanged)** |
| `.data` | 0x214000 / 0x296c | 0x214000 / 0x296c | exact |
| file size | 2,187,328 | 2,187,328 | exact |

`.rodata` is now **byte-identical** (269,159/269,159 bytes compare equal).
This removes the last known `.rodata` delta.  The function still compiles to
1432 B, so `.text` size/base and every following function address are
unchanged (`Status::setJumpgateUsed` is still at `0xac4d0`).

## 3. Why the function score drops (and why the table cannot be removed
##    without that drop)

The original's 40-entry TBH is value-indexed (`sub.w r1,r0,#150; cmp #39;
bhi; tbh` at `0xaa`, table at `0xae`; slot `i` is type `150+i`).  Its body
layout, however, is two ascending runs:

```
A = 150 151 152 153 154 155 158 162 166 170 184 189
B = 156 157 160 163 164 165 168 169 171 172
```

with the peeled shared body `167/174` first and the low group
`8, 0-check, 11, 13, 14` before A.  A plain `switch (m->getType())` emits
the same dispatch, the same 40-entry TBH and the same body bytes, but
`MachineBlockPlacement` places the case bodies in the jump table's
value-sorted successor order:

```
167/174, low group, 150 151 152 153 154 155 156 157 158 160 162 163
         164 165 166 168 169 170 171 172 184 189, latch
```

The rank table made the *sorted case values* A-then-B, which is why it
reproduced the original body layout.  Without it, the A/B bodies are
value-interleaved and the verifier's LCS drops from 92.5 % to 59.4 %.

The table is the only plain-C++ mechanism found that reproduces the original
body order.  The prior wave-119/120 notes established the same result with
an instrumented clang: the worklist is value-ordered, all case-body
frequencies are equal, and only unequal switch-edge frequencies (the rank
table, duplicate case labels, or `!prof` IR) or a fallthrough chain can move
the bodies.  Duplicate labels reproduce A-then-B but shrink the function to
1376 B and score 45.8-47.7 %; a full-A fallthrough scores 49.7 %; two
sequential switches score 52.6 %; `!prof` is not expressible from C++.
See `status-missioncompleted-wave119-w6930.md`, `-w6943.md`, `-w6971.md`
and `status-missioncompleted-wave119-w6935.md`.

## 4. Variants measured this pass (same `link_so` + single-symbol score)

| variant | usize | pct | body order | `.rodata` |
|---|---:|---:|---|---|
| **landed rank table (before)** | **1432** | **92.454632** | original A-then-B | +192 |
| **plain switch (landed)** | **1432** | **59.372027** | value | **exact** |
| plain switch, no body-168 cast | 1428 | 59.5 | value | exact |
| static rank table in `Status.cpp` | 1436 | 70.9 | original A-then-B | shifts (byte 4388) |
| computed rank expression (B set +40) | 1476 | 40.8 | A-then-B-ish | exact |
| duplicate A case labels (`dupA`) | 1376 | 45.8 | A-then-B-ish | exact |
| duplicate A + peeled labels (`dupB`) | 1376 | 47.7 | A-then-B-ish | exact |
| if-chain (no switch) | 1432 | 29.1 | source order | exact |
| `__builtin_expect` on the switch operand | 1384 | 22.6 | value | exact |

The static table in `Status.cpp` is the only variant that keeps the 92.5 %
body order, but it lands in the middle of `.rodata` (Status.cpp's `.rodata`
is at `0x1fc330`), shifts every later `.rodata` byte and drops `byte_exact`
4505 -> 4388.  It is not gate-green.

## 5. Proven vs hypothesis

**Proven (built and measured in this worktree):**

1. Deleting `tools/mission_case_table.cpp` and switching directly on
   `m->getType()` makes `.rodata` **byte-identical** (0x41b67,
   269,159/269,159 bytes) with `.text` size/base, `.ARM.exidx`, `.ARM.extab`
   (still 0x26294), `.data`, the 27-section layout and the 2,187,328 B file
   size unchanged; the gate stays byte 4505 / linked 4505, lint CLEAN,
   drift 0.
2. The plain switch matches the original's dispatch, TBH size/base, low
   group and shared tails, but lays the A/B bodies out in value order; the
   function is 59.372027 % at 1432 B.
3. The rank table is the only plain-C++ mechanism found that reproduces the
   original A-then-B body order; removing it necessarily costs the fuzzy
   score.  The `.text` content delta grows 88,116 -> 88,549 B while
   `.rodata` drops 192 -> 0 B.
4. A static rank table in `Status.cpp` keeps 70.9 % but shifts `.rodata`
   and is not gate-green (byte 4505 -> 4388).

**Hypothesis / open:** the original's A-then-B layout is reachable with a
plain switch only if the high jump table's successor list is A-then-B
(unequal edge frequencies or a second in-loop predecessor for the B bodies).
No plain-C++ source shape found so far provides that without changing the
body code or the TBH.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target drift -j6     # 0 fields
cmake --build cmake-build-match --target lint  -j6     # CLEAN

# single-function score
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN6Status16missionCompletedEbbx
```

Scratch (not committed): `/dev/shm/w7113/` — `Status_rank.cpp` (before),
`Status_plain.cpp` (landed), `Status_statictable.cpp`, `Status_rankexpr.cpp`,
`Status_ifchain2.cpp`, `Status_expect.cpp`, `score.py`, `try.sh`.

## 7. Files

- `src/game/mission/Status.cpp` — `missionCompleted` is a plain
  `switch (m->getType())` with the real type labels; the `MISSION_CASE`
  macro/comment block is gone.
- `CMakeLists.txt` — the `mission_table` object target and link input are
  removed.
- `tools/mission_case_table.cpp` — deleted.
- `docs/VALIDATION.md`, `docs/IDENTICALITY.md` — `.rodata` is now exact;
  the function score and `.text` delta are updated.
