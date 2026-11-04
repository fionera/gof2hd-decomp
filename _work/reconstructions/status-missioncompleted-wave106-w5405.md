# `Status::missionCompleted` — wave 106: rank table (87.0%) vs a table-free
# source-order dispatch (76.4%) (w5405)

Status: **landed pass** (rank table). Worktree `wt/w5405`, branch
`wave106-w5405`, base master `486f8a119`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`, LLVM `1d739ffb…`, `-Oz`). Target:
`_ZN6Status16missionCompletedEbbx`, original `0xabf38`, size **1432
(0x598)**.

## 0. Gate (before -> after)

```
cmake --build cmake-build-match --target check -j6
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Every gate count is unchanged from the direct-switch baseline (`byte_exact`
4500, `linked_exact` 4500, size-exact 4524/4524, data 843/843, imports 207).
The target's fuzzy score moves **60.4 -> 87.0** (`report.json` row: `pct
87.0`, `osize 1432`, `usize 1432`).

## 1. What was tried

The wave-105 handoff target was a *table-free* source-order dispatch with the
original's direct `sub/cmp/bhi/tbh` and a 40-entry type TBH. Two shapes were
built and measured with the real NDK clang + gold link + official verifier
(`tools/verify/verify.py --show`):

| variant | pct | usize | sections | body order |
|---|---:|---:|---:|---|
| direct `switch (m->getType())` (HEAD) | 60.4 | 1432 | 27 | ascending value |
| **rank table `kMissionCase[192]` in `.missioncase`** | **87.0** | **1432** | **28** | **original** |
| table-free `after189` second-predecessor switch | 76.4 | 1472 | 27 | original |
| `after189` without the case-166 anchor | 75.3 | 1468 | 27 | original |
| rank table as `unsigned int` / `unsigned short` | 85.7 | 1432 | 28 | original |
| rank table 33..40 entries | 71.9-73.6 | 1436-1448 | 28 | original |
| rank table `[m->getType() & 0xff]` | 85.6 | 1432 | 28 | original |
| nested `default: switch` | 39.1 | 1408 | 27 | merged |

The landed source is the wave-105 rank-table variant (the highest measured
score); the table-free `after189` shape is recorded here as the faithful
source-order alternative.

## 2. The table-free `after189` shape (76.4%, 1472 B)

Starting from the HEAD direct switch, label the group-B cases (`l156:` …
`l172:`), replace case 189's `break` with `goto after189`, and add after the
switch (before the loop close):

```cpp
        }
        goto latch;
after189:
        switch (m->getType()) {
            case 156: goto l156;
            case 157: goto l157;
            case 160: goto l160;
            case 163: goto l163;
            case 164: goto l164;
            case 165: goto l165;
            case 168: goto l168;
            case 169: goto l169;
            case 171: goto l171;
            case 172: goto l172;
            default: break;
        }
latch:;
```

The `goto latch` is essential: without it the C++ `default` falls through to
`after189` and the high table's default (16/40 probability) is placed first,
which puts group B *before* group A (measured 1476 B, group B reverse
order). With it, the main table is exactly the original's 40-entry TBH and
the group-A bodies land at the **exact original offsets** (`150` at `0x1d8`,
`151` `0x1de`, `152` `0x1e4`, `153` `0x218`, `154` `0x21e`, `155` `0x236`,
`158` `0x248`, `162` `0x2a4`). The `after189` switch gives the ten group-B
bodies a second unplaced predecessor, so `MachineBlockPlacement` does not
push them when the high jump table is merged; they become available only
after `after189` is placed (after case 189) and are then laid out in
ascending order — the original group-B order.

Why it is 40 B too big: the `after189` TBB dispatch is 36 B
(`mov/bl/subs/cmp/bhi.w/tbb` + 17-byte table) and the case-166 anchor costs
6 B (the un-anchored body is 4 B longer than the original's 100 B because
the allocator picks `r5/r6` instead of `r9/r5`). The extra 40 B moves the
return/latch and every group-B body, so every intra-function branch target
differs and the difflib ratio drops to 76.4 despite the body order being
correct. The function is not size-exact, so it is not landed: the 40-byte
growth shifts every later function in `.text`, and the full `check` fails the
ratchet with `byte_exact 4500 -> 2326` (linked_exact stays 4500). The
size-exact rank table is therefore the only landable variant.

## 3. The landed rank-table shape (87.0%, 1432 B)

`src/game/mission/Status.cpp` keeps the wave-105 `kMissionCase[192]`
file-static rank table in its own `.missioncase` section and switches on
`kMissionCase[m->getType()]` with rank case labels:

```
0,8,11,13,14, 150..171, 181
```

The ranks span 150..181 (a 32-entry TBH) rather than the full 150..189, and
the case-159 null anchor (`Ship *s = ship; if (s == 0) continue;`) keeps the
function at 1432 B and forces the original `(index r9, amount r5, ship
r0)` allocation. The rank table makes clang's value-sorted jump-table
successor list equal to the original source order, so MBP lays the bodies
out in the original order.

Cost (documented, not a gate count): section count **27 -> 28**, `.shstrtab`
`0x117 -> 0x124`, file size **2,187,328 -> 2,187,380** (+52 B). `.text`
stays `0x1337c8`, `.rodata` 0 differing bytes, every ratchet count
unchanged. This is the same trade-off wave-100/wave-101 accepted and wave-102
reverted; wave-106 re-lands it because the fuzzy score is the objective and
every ratchet count is unchanged.

## 4. Proven vs hypothesis

**Proven (command-backed):**

* The table-free `after189` shape reproduces the original 40-entry TBH and
  the exact group-A body offsets, but is 1472 B (40 over) because of the
  36-byte second TBB and the 6-byte case-166 anchor; it scores 76.4.
* The rank table is size-exact (1432 B) and scores 87.0; it is the highest
  measured variant.
* Enlarging the rank table to 33..40 entries (group A shifts toward 0 but
  the function grows to 1436..1448) scores 71.9-73.6; the 4-byte alignment
  step dominates.
* The `unsigned int`/`unsigned short` rank table (group A at -8) scores
  85.7; `[m->getType() & 0xff]` (also -8) scores 85.6; the `unsigned char`
  table (group A at -10) scores 87.0.
* The nested `default: switch` shape merges in SimplifyCFG and scores 39.1.

**Hypothesis / open:**

* A table-free shape that makes the group-B bodies' second predecessor free
  (or places it after group B without a cycle) would be size-exact and
  should score well above 87. The `after189` switch is the minimal
  source-level edge generator found; a 10-edge jump table costs 36 B.
* The original's single-switch source-order layout remains a
  `MachineBlockPlacement` tie-break artifact: the high JT's successors are
  value-sorted and all body frequencies are exactly equal
  (`0.1864530226`), so `selectBestCandidateBlock` keeps the first max.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5405
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target lint -j6    # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

Scratch (not committed): `/dev/shm/w5405/` — `Status.head.cpp` (direct),
`Status.rank.cpp` (landed), `v_after189b.cpp` (table-free), `cc.sh`,
`decode.py`, `genll.sh`, `measure.sh`, `show_rank.txt`, `show_a189.txt`.
