# `Status::missionCompleted` — wave 191b (w13569): direct `getType()` dispatch

Worktree `wt/w13569`, branch `wave191b-w13569`, base master `fa418d267`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13569`.

**Status: landed (two passes).**  Pass 1: raw **74 -> 71** (-3), aligned 38
unchanged, size **1,432/1,432** exact, 527/527 instructions.  `.text`
differing bytes 71,233 -> 71,230 (-3).  `.ARM.extab` 577, `.ARM.exidx` 0,
`.rodata` +485, build-id 20 unchanged.  Pass 2 (same wave): delete the
now-dead `.rodata` helper, `.rodata` sh_size +485 -> **+0** (content
byte-identical), raw still 71.  All 27 section headers byte-identical.  Gate: `RATCHET PASS`
(byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0,
parity 5370/5370).  No baseline update (byte_exact unchanged).

## 1. What changed

The caller previously dispatched through the cross-TU helper:

```cpp
switch (__emutls_unregister_key(m)) { ... }
```

The helper (`src/game/mission/StatusMissionTable.cpp`, emitted into
`.rodata`) maps `m->id` to the same value, so the switch is equivalent,
but the call target differs: the original calls `getType@plt` at
`0x62874`, while the helper lives at `.rodata` `0x20b978`.  The `blx`
encoding is 4 B and only 3 B differed (`0x09e..0x0a1`).

Switching directly on the member function:

```cpp
switch (m->getType()) { ... }
```

reproduces the original's `blx getType@plt` byte-for-byte.  The helper
definition is retained in `StatusMissionTable.cpp` (unused) because it
fills the `.rodata`..`.fini_array` gap and keeps the `.rodata` layout
(+485) and `data_exact 843/843` unchanged.

## 2. Remaining raw bytes (71)

| offset | bytes | cause |
|---|---:|---|
| `0x0ba`..`0x0fd` | 24 | 40-entry TBH permutation (DP-optimal for the current body layout) |
| `0x32e`..`0x36d` | 46 | case-158 register juggling (`index` r5 vs r9, `amount` r1 vs r5, `cargo` r6 vs r2, `this` stack reload) |
| `0x58c` | 1 | literal-pool PC-relative offset |

## 3. TBH investigation (not landed)

The original's TBH is a permutation because its source attached bodies to
permuted case labels while MBP laid the bodies out in ascending *type*
order.  Ours lays them out in ascending *label* order (MBP follows the
jump-table successor order), so the TBH is the identity mapping.
Reordering/relabelling the case blocks to the original assignment
(`label_orig`: 156->158, 157->162, 158->166, 160->170, 162->184,
163->189, 164->156, 165->157, 166->160, 168->163, 169->164, 170->165,
171->168, 172->169, 184->171, 189->172) makes MBP reorder the bodies
and drops the size to 1,424 (raw 907).  An `if`/`else` chain still
lowers to a `tbh` jump table but also changes the layout (raw 1,306 /
size 1,432).  The 24-byte TBH is therefore only reducible if MBP can be
made to preserve source order, which no source reshuffle tried here did.

## 4. Case-158 investigation (not landed)

The original keeps `index` in r9, `amount` in r5, `cargo` in r2 and
`this` in r6.  Ours uses r5/r1/r6 and reloads `this` from `[sp,#16]`.
The faithful order (`index, amount, cargo`) lets `cargo` land in r2 but
then `this` is spilled and `amount` takes r6 (size 1,436/1,440).  The
closest size-exact variants are `gs` hoists (raw 84/88/98, 528 insns)
and `i_c_a_pos` (raw 387).  A 216-variant sweep over call order, cargo
and `hasEquipment` sources, positive/negative tests and `index` vs
re-query found no size-exact variant below raw 74.

## 5. Pass 2: remove the dead `.rodata` helper

After pass 1 the `__emutls_unregister_key` helper in
`StatusMissionTable.cpp` had no caller.  It was emitted into `.rodata` via
`#pragma clang section text=".rodata"` and its 480-byte switch body was
the entire `.rodata` sh_size delta (+485 = 1 pad + 4-byte `bx lr` + 480).
Deleting the definition and the `Status.cpp` extern declaration leaves
`.rodata` at exactly the original 269,159 B with byte-identical content,
and keeps every other section header unchanged.  The TU itself stays in
`tools/source_order.txt` (it is a witnessed original TU) and now compiles
to an empty object.  Raw 71 unchanged, size 1,432/1,432, insn 527/527.
