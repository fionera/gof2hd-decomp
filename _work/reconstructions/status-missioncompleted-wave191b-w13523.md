# `Status::missionCompleted` — wave 191b (w13523): helper takes `Mission*`, labels `150+rank`

Worktree `wt/w13523`, branch `wave191b-w13523`, base master `5c99979c4`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13523`.

**Status: landed.**  Raw **86 -> 82** (-4), aligned **55 -> 50** (-5),
size **1,432/1,432** exact, 527/527 instructions.  `.rel.plt`
byte-identical (29,392 B); `.ARM.exidx` byte-identical (28,528 B);
`.text` size/VMA unchanged (`0x69440`, `0x1337c8`); `.rodata` size
269,644 unchanged; file size 2,187,328 exact.  Gate: byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, stub/missing/extra 0, lint CLEAN, drift 0, parity
5370/5370 (`RATCHET PASS`).  No baseline update (byte_exact is
unchanged).

## 1. What changed

The rank helper now takes the `Mission *` and returns the **case
label** directly, instead of taking an `int` type and returning a
rank:

* main types: `150 + rank` (rank is the w13506 DP-optimal
  permutation), so the caller's switch range is `150..189` and clang
  emits the original's `sub.w r1,r0,#150; cmp r1,#39; bhi; tbh`
  byte-for-byte;
* low types keep their raw id (`0, 8, 11, 13, 14`), so the second
  level is the original's `sub.w r1,r0,#8; cmp r1,#6; bhi; tbh` plus
  the `cmp r0,#0` fall-through.

The caller's `case` labels are the corresponding labels (`150+rank`
for main, raw type for low).  The helper body reads `Mission::id`
directly rather than calling `getType()`: the helper lives in
`.rodata`, and a call would give it its own `.ARM.exidx` entry,
growing `.ARM.exidx` by 0x10 and shifting `.rodata` by 0x10 (that
shift alone drops `byte_exact` from 4507 to 4043 because every
`.rodata`-relative literal pool changes).

## 2. Where the 4 raw bytes come from

Before this pass the caller did `bl getType; blx helper; cmp r0,#39`
and the helper returned a 0..39 rank.  The `sub #150` slot was
occupied by the helper `blx`, and the low group was
`sub #208; cmp #6` + `cmp r0,#200`.  Now the caller does
`blx helper; sub #150; cmp r1,#39` (the helper performs the
`getType` work), and the low group is `sub #8; cmp #6` + `cmp r0,#0`.
The `sub #150` and the whole low dispatch become byte-identical; the
only remaining dispatch difference is the 3 differing bytes of the
call target (`getType` PLT vs the `.rodata` helper).

Remaining 82 raw bytes (same-address byte diff):

| offset | bytes | cause |
|---|---:|---|
| `0x09e` | 3 | `blx getType` vs `blx __emutls_unregister_key` |
| `0x0ba`..`0x0fd` | 24 | 40-entry TBH permutation (DP-optimal; irreducible) |
| `0x298` | 6 | case-6 (type 158) epilogue schedule: `ldr r6,[sp,#16]` before `mov r8,r4` in the original |
| `0x32e`..`0x36d` | 48 | case-8 (type 166) body: current codegen device is `index, cargo, amount` + `(void) getStatusValue()` + `hasEquipment(index,1)`; the faithful body is `index, amount, cargo` + `hasEquipment(getProductionGoodIndex(),1)` and is size-exact but reorders the whole body layout (raw 391) |
| `0x58c` | 1 | `.rodata`-relative literal pool entry (consequence of the case-8 body) |

## 3. What was ruled out (do not redo)

* Helper taking `Mission *` but calling `getType()`: same raw 82, but
  `.ARM.exidx` grows 0x6f70 -> 0x6f80 and `.rodata` shifts by 0x10,
  which drops `byte_exact` to 4043.  Read `m->id` instead.
* Low ranks `40, 48, 51, 53, 54` (or any values inside/next to the
  main 0..39 cluster): clang collapses the switch to a single
  55-entry TBH (`cmp #54`), size 1424, raw 1204.
* Faithful case-8 body (`index, amount, cargo` + second
  `getProductionGoodIndex`): size 1432 but raw 391 because the body
  layout (TBH targets) moves.
* Faithful case-8 body with `ship->getCargo()` inline: size 1436,
  raw 689.
* Changing the case-6 declaration order (`hasGood` before
  `hasSpecial`) or `if (hasGood) { if (hasSpecial) ... }`: raw 93 / 86.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13523
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=82 aligned=50 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)  # RATCHET PASS
```

## 5. Files changed

* `src/game/mission/Status.cpp` — helper declaration takes `Mission *`;
  first-switch labels remapped to `150+rank` / raw low type.
* `src/game/mission/StatusMissionTable.cpp` — helper takes `Mission *`,
  reads `m->id`, returns `150+rank` / raw low type.
* this note.
