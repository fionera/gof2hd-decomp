# HangarWindow::render — ship-branch body order + EH anchor (wave 101, wf107)

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`).
Baseline: byte 4500/4524, linked 4500/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0; `render` 80.8 % (raw 80.848134), 9700 B.

## What was wrong

Reading the original disassembly (not the decompiler's C) shows the ship price
branch is:

```
14953a: ldr  r0,[sp,#108]     ; tab
149540: cmp  r0,#1
149542: bne  1498fe
149546: ldrb r0,[r9,#285]     ; this->upgradeMode
14954a: cmp  r0,#0
14954c: beq  1498fe
149550: <plain price>         ; tab == 1 && upgradeMode
...
1498fe: ldr r0,[sp,#140]      ; &Globals::Canvas
149904: ldr r0,[sp,#108]      ; tab
149906: cmp  r0,#1
149908: beq  1499f4           ; -> difference price
14990a: ldr r0,[sp,#92]       ; i
14990c: cmp  r0,#2
14990e: bcs  1499f4           ; -> difference price
149910: <plain price>         ; fallthrough
149960: b    149a94
1499f4: <difference price>
```

i.e. the faithful source is

```cpp
if (tab == 1 && upgradeMode) {
    plain;                       // tab == 1 && upgradeMode
} else if (tab == 1 || i > 1) {
    difference;                  // price - status->getShip()->getPrice()
} else {
    plain;
}
```

The checked-in source had the second and third bodies swapped (plain in the
`else if`, difference in the `else`), which is semantically wrong.  Swapping
them alone *lowered* the fuzzy score (75.45) because the compiler then
propagates the `tab == 1` fact out of the outer `if` into the `else if` and
splits the second test into two copies, changing the block layout.

Writing the outer test as `if (this->upgradeMode && tab == 1)` prevents that
propagation (the `else` no longer knows `tab != 1`), so the compiler emits the
original's single re-test at `1498fe`.  That alone gives 81.67 but the body is
9728 B (the extra `tab` re-test).

## Getting back to 9700 B and the original .text/.ARM.extab layout

`byte_exact` is extremely sensitive to the layout: `.text` must stay
`0x1337c8`, and `.ARM.extab` must stay in `(0x26288, 0x26298]` so `.rodata`
lands at `0x1c9e10`.  The 3 `description = description;` fillers and the
`volatile int pad0/pad0b` sinks were load-bearing only because the wrong
ship-branch shape was 28 B too small.  With the faithful branch:

* drop the 3 fillers (`description = description;`) -> 9708 B
* drop `pad0`/`pad0b` -> 9700 B, but `.ARM.extab` shrinks to `0x26288`
  (`.rodata` moves to `0x1c9e00`, `byte_exact` 4500 -> 4040)
* drop the `pad3` anchor (`volatile int pad3 = this->field_0x114;`) -> 9700 B
* add one EH sink around the list-mode `setSplitText(price)` call:

```cpp
{
    volatile ChoiceWindow *p = this->dialog;
    (*this->buttons)[12 + count]->setSplitText(price);
}
```

This restores the original's extra LSDA cleanup region
(`.ARM.extab 0x2628c`, `.rodata 0x1c9e10`) at zero net code size.

## Measured

| variant | raw score | size | .text | .ARM.extab | .rodata | gate |
|---|---:|---:|---|---|---|---|
| baseline (git HEAD) | 80.848134 | 9700 | 0x1337c8 | 0x26294 | 0x1c9e10 | PASS |
| faithful branch only | 75.454545 | 9692 | — | — | — | n/a |
| `upgradeMode && tab==1` + faithful | 81.673088 | 9728 | — | — | — | n/a |
| + remove fillers | 78.832117 | 9708 | — | — | — | n/a |
| + remove fillers, pad0/pad0b | 83.344813 | 9700 | 0x1337c8 | 0x26288 | 0x1c9e00 | FAIL byte 4040 |
| + remove pad3 | 83.069293 | 9700 | 0x1337c8 | 0x2628c | 0x1c9e10 | **PASS** |

Final: official `verify` pct **83.1** (raw 83.069293), 9700 B, 3633 insns
(orig 3626).  Full gate: byte 4500, linked 4500, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

## Commands

```
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
cmake --build cmake-build-match --target lint -j8       # CLEAN
cmake --build cmake-build-match --target drift -j8      # 0 fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

## Proven vs hypothesis

* Proven from disassembly: the ship-branch body order and the
  `tab == 1 && upgradeMode` / `tab == 1 || i > 1` structure; the original has
  two separate plain-price copies (not one merged block).
* Proven by measurement: the `upgradeMode && tab == 1` spelling is required to
  keep the original's block layout; the volatile sink restores `.ARM.extab`
  without changing code size.
* Hypothesis: the volatile sink is a codegen anchor, not original source; the
  original's extra LSDA row probably comes from a scope we have not
  reconstructed.  The remaining 16.9 % is register allocation and literal-pool
  placement (3633 vs 3626 instructions).
