# `Status::missionCompleted` — wave 191 (w13503): optimal rank permutation

Worktree `wt/w13503`, branch `wave191b-w13503`, base master `125c8899b`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13503`.

**Status: landed.**  Raw **97 -> 86** (-11), aligned **54 -> 59**,
size **1,432/1,432** exact, 527/527 instructions.  `.text` differing
bytes **71,803 -> 71,792** (-11); `.ARM.extab` **582** unchanged
(size/base exact at `0x1a3b78`, 156,312 B); `.ARM.exidx` **0**; build-id
20 unchanged.  `.rodata` sh_size **269,644** unchanged (the 484 B rank
helper is the same size; original 269,159 B content still exact),
`.rel.plt` **byte-identical** (29,392 B, 0 differing bytes), 27/27
section headers, file size 2,187,328 exact.  Gate: byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, stub/missing/extra 0, lint CLEAN, drift 0, parity
5370/5370 (`RATCHET PASS`).

## 1. What changed

The cross-TU rank helper (`src/game/mission/StatusMissionTable.cpp`)
and the caller's `case` labels were re-permuted.  The helper still maps
mission type -> rank, and the caller still switches on the rank; only
the rank *values* changed.  The high-type rank order (which fixes the
body layout) is unchanged, so every case body stays at its original
address.  The new mapping:

| type | old rank | new rank | | type | old rank | new rank |
|---|---:|---:|---|---|---:|---:|
| 150 | 0 | 0 | | 163 | 17 | 20 |
| 151 | 1 | 1 | | 164 | 18 | 21 |
| 152 | 2 | 2 | | 165 | 19 | 22 |
| 153 | 3 | 3 | | 166 | 10 | 10 |
| 154 | 4 | 4 | | 167 | 21 | **17** |
| 155 | 5 | 5 | | 168 | 20 | **34** |
| 156 | 14 | 15 | | 169 | 22 | **37** |
| 157 | 15 | 16 | | 170 | 11 | 12 |
| 158 | 6 | 7 | | 171 | 23 | **38** |
| 160 | 16 | 18 | | 172 | 39 | 39 |
| 162 | 9 | 8 | | 174 | 25 | **24** |
| 184 | 12 | 13 | | 189 | 13 | 14 |

The low-group ranks (types 0/8/11/13/14 -> 200/208/211/213/214) are
unchanged, so the second dispatch is byte-for-byte the same as before.

## 2. Why this is a win

The first TBH is 40 halfwords (indices 0..39).  The original table is
*type*-indexed (`entry[i]` is the body for type `150+i`), while ours is
*rank*-indexed (`entry[r]` is the body for rank `r`).  The body layout
is fixed by the rank order, but the rank values themselves are free as
long as the order is preserved.  The old assignment was a compact
`0..23` + `39` run, which matched the original entry at only 6 of the
40 indices (the 150..155 run).  The new assignment is the permutation
that maximises the number of matching table bytes subject to:

* the 22 non-peeled high types keep their original body order (strictly
  increasing ranks along `150 151 152 153 154 155 158 162 166 170 184
  189 156 157 160 163 164 165 168 169 171 172`);
* `167`/`174` are the peeled shared body (ranks may be anywhere);
* all 24 case ranks are distinct in `0..39` and rank 39 is used, so the
  dispatch stays `cmp r0,#39; bhi; tbh` with a 40-entry table;
* the 16 unused ranks are the table's default slots.

A DP over the 40 table slots found the optimum: **24 differing table
bytes** instead of **35** (out of 80).  The biggest single gain is
`167 -> 17` and `174 -> 24`, which makes the peeled-body entries at
indices 17 and 24 match the original's `0x0028` exactly (the old
`21`/`25` did not).  The remaining table diff is the unavoidable
permutation (the original's body order is not type order).

The body layout is unchanged: the case-10 body is still at
`ac242..ac2a6`, case 6 at `ac180`, the 156..172 run at `ac32a..`, and
the end literal pool at `ac4c4`.  The residual is now:

* dispatch 5 B: `blx helper` vs `sub.w r1,r0,#150` and `cmp r0` vs
  `cmp r1`;
* first TBH 24 B (was 35): rank-indexed vs type-indexed permutation;
* second dispatch 2 B: `sub #208` vs `sub #8`, `cmp #200` vs `cmp #0`;
* case-6 tail merge 6 B: `mov r8,r4; lsls; ldr r6` vs the original's
  `ldr r6; mov r8,r4; lsls` (the shared tail block with case 10);
* case-10 body 48 B: index/cargo/amount register order (`r5/r6/r1` vs
  the original's `r9/r5/r2`) and the shared `hasEquipment` tail
  (`cmp; b ac1d4` vs `b ac458`);
* end pool 1 B: the case-10 `Globals` literal PC-relative offset.

## 3. Measured variants

The rank permutation was chosen by a DP over the table bytes; the
`167`/`174` placement and the default slots are part of the
optimisation.  The landed assignment is the unique optimum at **86**
raw.  Reordering the *source* case blocks to the original body order
gives 92 (the `Globals::status` literal-pool order becomes `10,12,20`);
the current source order (pool order `10,20,12`) is kept.  All
case-10 source-shape variants tried (faithful `index, amount, cargo`
order, re-query `hasEquipment`, `this->ship`, `Ship *` locals,
`register`/`long` types, cached/dead re-query) are either not
size-exact (1,428/1,436) or worse (raw >= 87).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13503
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=86 aligned=59 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)  # RATCHET PASS
```

## 5. Files changed

* `src/game/mission/Status.cpp` — first-switch `case` labels re-permuted
  to the new ranks (bodies unchanged, same source order).
* `src/game/mission/StatusMissionTable.cpp` — helper return values
  re-permuted; same switch shape/size.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-191 numbers.
* this note.
