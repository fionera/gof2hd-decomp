# `Status::missionCompleted` — wave 190 (w13396): case-body source order
# 10-20-12 restores the literal-pool order

Worktree `wt/w13396`, branch `wave190-w13396`, base master `8ffa01f05`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13396`.

**Status: landed.**  Raw **468 -> 466** (-2), aligned 193 (unchanged),
size 1,432/1,432 exact.  `.text` differing bytes **73,014 -> 73,012**
(-2); `.ARM.extab` 603 (unchanged, size/base exact at `0x1a3b78`,
156,312 B); build-id 20.  Total (text+extab+build-id)
**73,637 -> 73,635** (-2).  `.rodata` unchanged (size 269,351, +0xc0;
original 269,159 B content exact), `.fini_array` 8 B @ `0x20cba0`,
`.dynamic`/`.rel.dyn`/`.rel.plt`/`.got`/`.data.rel.ro` byte-identical,
27/27 section headers, file size 2,187,328 exact.  Gate: byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, stub/missing/extra 0, lint CLEAN, drift 0, parity
5370/5370 (`RATCHET PASS`).

## 1. What changed

The `case 10:` block (type 166) moved in the *source* switch from its
committed position (between `case 9` and `case 11`) to just ahead of
`case 12:`, i.e. the source order of the three `Globals::status` users
is now `10, 20, 12` instead of `10, 12, 20`.  The switch is still on
`__emutls_unregister_key[m->getType()]`, the rank table is unchanged,
and the case bodies are byte-for-byte the same code.  The physical
body order is fixed by the rank values, so this is not a body-order
change.

## 2. Why this is a win

The three `Globals::status` GOT loads in the function share one
literal pool at the end of `.text` (`ac4c4`/`ac4c8`/`ac4cc`).  The
pool order follows the IR first-use order of the constants, which is
the source order of the case blocks.  The original's pool order is
`case 10, case 20, case 12` (values `0x3ece`, `0x3d78`, `0x3e96`);
the committed source emitted `case 10, case 12, case 20`.  Reordering
the source blocks to `10, 20, 12` makes the pool order match, so the
three PC-relative literal words now differ only by the `add r0,pc`
address delta (1 byte each instead of 2 on two of them).

Measured permutations of the three blocks (all size 1,432):

| source order of 10/12/20 | raw | aligned |
|---|---:|---:|
| **10, 20, 12 (landed)** | **466** | 193 |
| 12, 20, 10 | 467 | 193 |
| 10, 12, 20 (base) | 468 | 193 |
| 12, 10, 20 | 469 | 193 |
| 20, 10, 12 | 469 | 193 |
| 20, 12, 10 | 469 | 193 |

The residual is still dominated by the 6-byte case-10 body deficit
(the `-Oz` build drops the faithful `getProductionGoodIndex()` re-query
and keeps a dead `cmp r0,#0` before the shared bool tail), which shifts
every body after `ac28a` by -6.  The literal-pool order is the only
free lever left in this area.

## 3. Structural checks

* `.text` 0x1337c8 exact, 73,012 differing bytes (was 73,014).
* `.ARM.extab` size/base exact, 603 differing bytes (unchanged);
  `.ARM.exidx` 0.
* `.rodata` sh_size 269,351 (+0xc0), original 269,159 B content exact;
  `.fini_array` 8 B @ `0x20cba0`; `.dynamic`, `.rel.dyn`, `.rel.plt`,
  `.got`, `.data.rel.ro`, `.dynsym`, `.dynstr`, `.hash`, `.init_array`
  byte-identical; 27/27 headers; file size 2,187,328 exact.
* `check` RATCHET PASS, `lint` CLEAN, `drift` 0 fields.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13396
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=466 aligned=193 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 5. Files changed

* `src/game/mission/Status.cpp` — `case 10:` block moved ahead of
  `case 12:` (source order 10-20-12).
* `docs/IDENTICALITY.md` — wave-190 residual table and section totals.
* this note.

## Current-master harvest (wave 190)

Applied on master `a59477829`: raw **468 -> 466** (-2), aligned 193
unchanged, size exact 1,432 B. `.text` **73,006 -> 73,003** (-3, combined
with the w13384 LevelScript d2 harvest); `.ARM.extab` 603 and build-id 20
unchanged; total (text+extab+build-id) **73,629 -> 73,626** (-3).
`check` -> RATCHET PASS.
