# `Status::missionCompleted` — wave 191b (w13506): aligned-optimal rank set

Worktree `wt/w13506`, branch `wave191b-w13506`, base master `125c8899b`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13506`.

**Status: landed.**  Raw **86 -> 86** (unchanged), aligned **59 -> 55**
(-4), size **1,432/1,432** exact, 527/527 instructions.  `.rel.plt`
byte-identical (29,392 B, 0 differing bytes); `.rel.dyn` 0;
`.ARM.exidx` 0; `.ARM.extab` 577 unchanged; `.text` 71,491
unchanged; `.rodata` size 269,644 unchanged; file size 2,187,328
exact.  Gate: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, stub/missing/extra 0, lint
CLEAN, drift 0, parity 5370/5370 (`RATCHET PASS`).  No baseline
update (byte_exact is unchanged).

## 1. What changed

Only the rank *values* were re-permuted (bodies, source order, helper
shape and the low-group ranks 200/208/211/213/214 are unchanged).
This is the first of the **55 DP-optimal rank sets** (all sets have
the same 24 differing TBH bytes, i.e. raw 86); it is the unique one
that also minimises the instruction-alignment diff.

| type | w13503 rank | new rank | | type | w13503 rank | new rank |
|---|---:|---:|---|---|---:|---:|
| 150 | 0 | 0 | | 163 | 20 | 18 |
| 151 | 1 | 1 | | 164 | 21 | 19 |
| 152 | 2 | 2 | | 165 | 22 | 20 |
| 153 | 3 | 3 | | 166 | 10 | 8 |
| 154 | 4 | 4 | | 167 | 17 | 17 |
| 155 | 5 | 5 | | 168 | 34 | 21 |
| 156 | 15 | 14 | | 169 | 37 | 22 |
| 157 | 16 | 15 | | 170 | 12 | 10 |
| 158 | 7 | 6 | | 171 | 38 | 34 |
| 160 | 18 | 16 | | 172 | 39 | 39 |
| 162 | 8 | 7 | | 174 | 24 | 24 |
| 184 | 13 | 12 | | 189 | 14 | 13 |

## 2. Why this is a win

The raw byte count is fixed by the 40-entry TBH permutation (24 bytes
for every DP-optimal set).  Among the 55 sets, this one yields the
smallest instruction-alignment diff (55 vs 59..64 for the others),
i.e. the fewest structurally-different instructions.  It keeps the
original body layout exactly (`ac036`, `ac110`..`ac41a`, end pool
`ac4c4`) and the second dispatch byte-identical.

## 3. What was ruled out (do not redo)

* Identity rank (`rank = type-150`) would make the TBH table
  byte-identical but changes the body layout: clang's
  MachineBlockPlacement lays the main bodies out in *sorted case-value*
  order, so identity gives `size 1424` and `raw 916`.
* A direct `switch (m->getType())` (no helper) gives `size 1424/1436`
  and `raw >= 687`; the helper is required for the 1432 size.
* The faithful case-166 body (`index, amount, cargo` order, second
  `getProductionGoodIndex`, no `(void) getStatusValue()`) is
  `size 1436` (+4: an extra `mov r1,r6` and `cmp r0,#0`) and raw 689.
* Source-block reordering does not help: the main-body layout follows
  the sorted case values, not the source order.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13506
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=86 aligned=55 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)  # RATCHET PASS
```

## 5. Files changed

* `src/game/mission/Status.cpp` — first-switch `case` labels re-permuted.
* `src/game/mission/StatusMissionTable.cpp` — helper return values re-permuted.
* this note.
