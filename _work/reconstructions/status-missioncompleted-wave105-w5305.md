# `Status::missionCompleted` — wave 105: re-land the source-order rank switch
# (60.4% -> 87.0%, 1432 B, byte/linked unchanged; 28 sections)

Status: **landed pass**. Worktree `wt/w5305`, branch `wave105-w5305`, base
`835748c18`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).
Target: `_ZN6Status16missionCompletedEbbx`
(`Status::missionCompleted(bool, bool, long long)`), original `0xabf38`,
size **1432 (0x598)**.

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

Every gate count is unchanged (`byte_exact` 4500, `linked_exact` 4500,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0). The
target's fuzzy score moves **60.4 -> 87.0** (`report.json` row: `pct 87.0`,
`osize 1432`, `usize 1432`). No baseline update is needed (the baseline tracks
aggregate counts, not per-function fuzzy scores).

## 1. What changed

`src/game/mission/Status.cpp` re-adds the wf108 32-entry type -> rank table
(`kMissionCase[192]`, file-static, `#pragma clang section
rodata=".missioncase"`) and switches on `kMissionCase[m->getType()]` with rank
case labels. The rank permutation is the wf108 one:

```
0,8,11,13,14, 150..171, 181
```

and maps the 23 mission bodies onto the original physical body order
(167/174, 8, 0/11, 13, 14, 150..155, 158, 162, 166, 170, 184, 189,
156, 157, 160, 163, 164, 165, 168, 169, 171, 172). The case-159 null anchor
(`Ship *s = ship; if (s == 0) continue;`) keeps the function at 1432 B and
forces the original `(index r9, amount r5, ship r0/[r6,#400])` allocation.

This is the wf108 variant (`_work/reconstructions/status-missioncompleted-wave101-wf108.md`),
which wave102 removed to restore the 27-section table. Wave105 re-lands it
because the target's fuzzy score is the assigned objective and every ratchet
count is unchanged.

## 2. Why the rank table is still the only mechanism

The original dispatch is a plain `int` switch on `getType()`:

```
abfda  sub.w r1, r0, #150
abfde  cmp   r1, #39
abfe0  bhi.n ac046
abfe2  tbh   [pc, r1, lsl #1]   ; 40 halfwords @ 0xabfe6
...
ac046  sub.w r1, r0, #8
ac04a  cmp   r1, #6
ac04c  bhi.n ac0be
ac04e  tbh   [pc, r1, lsl #1]   ; 7 halfwords @ 0xac052
ac0be  cmp   r0, #0
```

The physical body order is the original source order, not ascending type
value. clang 7's `SelectionDAGBuilder::buildJumpTable` adds the jump-table
successors in value-sorted table order, and
`MachineBlockPlacement::selectBestCandidateBlock` keeps the first maximum on
a frequency tie, so a direct switch lays the bodies out in ascending type
order. The rank table makes the value order equal the source order; it is a
codegen device, not original data.

The direct switch (60.4%) is size-exact and gate-green, but the rank table is
the only source-only shape that recovers the source-order body layout. The
two-switch form reproduces the order at 1512 B; nested switches are merged
back to value order; `__builtin_expect`, loop/CFG wrappers and source case
reordering do not change the high-group layout. See
`status-missioncompleted-wave101-wf108.md`, `status-missioncompleted-wave99-w4903.md`
and `status-missioncompleted-source-order-tbh-w3906.md`.

## 3. Cost (documented, not a gate count)

The rank table lives in its own `.missioncase` section in the 0x1229-byte gap
between `.rodata` (ends `0x20b977`) and `.fini_array` (`0x20cba0`), so no
existing address moves and `.rodata` stays byte-identical. The cost is:

* section count **27 -> 28** (the original has 27);
* `.shstrtab` **0x117 -> 0x124** (73 differing bytes, the `.missioncase` name);
* file size **2,187,328 -> 2,187,380** (+52 bytes).

`.text` stays `0x1337c8`, `.rodata` 0 differing bytes, `.rel.plt` content
byte-identical, and every ratchet count is unchanged. This is the same
trade-off wave100/wave101 accepted and wave102 reverted.

## 4. Residual (87.0 -> 100)

The body order now matches. The remaining 13 points are:

1. the `ldr/add/ldrb` rank-table load (8 B) vs the original's direct
   `sub/cmp/bhi/tbh`;
2. the 32-entry rank TBH vs the original's 40-entry type TBH (the table
   entries encode different values);
3. the 8-byte shift of the group-A bodies (the group-B bodies land at the
   original offsets, which is why wf108 scores 87.0).

Removing the load requires a compiler-side change (source-order jump-table
successors or the MBP worklist sorted by MBB number), not a source shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5305
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target verify -j6  # report.json: pct 87.0
cmake --build cmake-build-match --target lint -j6    # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```
