# MenuTouchWindow C1/C2 wave 119 (w6724): row2 hoisted before the addButton(19) call

Date: 2026-11-05. Worktree `wt/w6724`, branch `wave119-w6724`, base
`432167221` (wave-119 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Target:
`_ZN15MenuTouchWindowC1Ei` / `_ZN15MenuTouchWindowC2Ei` (weak alias, same
address/size/score).

## TL;DR

* Ctor: **100.000000 %**, 9600 B, byte- and linked-exact (was 99.819277 %,
  9600 B after w6718; 99.668675 % before wave 119).
* The last 8 differing instructions were the mission `row + 2` ADD.  The fix
  is a source-ordering fact, not a new expression: declare `int row2 =
  row + 2;` **before** the preceding `addButton(19, ...)` call and use it
  inside the mission `if`.  The old code declared `char row2` *inside* the
  `if`, which let SelectionDAG sink the ADD to the addButton argument after
  the `String` temporary was built, and forced an extra `uxtb`.
* Gate: **byte_exact 4503 -> 4505, linked_exact 4503 -> 4505** (both C1 and
  C2), size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS** (baseline intentionally not updated by this worker; the
  check reports `GAIN ... (lock with --update-baseline)`).
* 27 sections, `.so` exactly 2,187,328 B.  `.text` 0x1337c8 @0x69440,
  `.rodata` 0x41c27 @0x1c9e10, `.ARM.extab` 0x26290 @0x1a3b78 (still the
  known 8 B short), all unchanged.

## 1. Remaining delta after w6718

Only one region differed (8 instructions, same 54 B block):

```
orig: 122da6 adds r4,r5,#2      ; row2 = row + 2 into callee-saved r4
      122da8 blx  getText(395)
      122dac add  r5,sp,#372    ; String temp address reuses r5
      ...
      122dc6 mov  r2,r5
      122dc8 mov  r3,r4

ours: 122da6 blx  getText(395)
      122daa add  r4,sp,#372
      ...
      122dc0 adds r0,r5,#2
      122dc6 uxtb r3,r0
```

`r5` held `row` in both.  The original materialises `row + 2` into `r4`
*before* `getText`, freeing `r5` for the `String` temporary; ours keeps
`row` in `r5`, uses `r4` for the temporary, and rematerialises the ADD at the
call (plus a `uxtb` for the `char`).

## 2. Root cause and fix

The placement of the *declaration* decides the machine schedule.  A local
`int row2 = row + 2;` declared immediately before the use is still sunk by
SelectionDAG next to the `addButton` argument (after the `String` ctor).
Declaring it before the previous `addButton(19, ...)` call puts the ADD in
the predecessor block, where it is live across that call and must be kept in
a callee-saved register (`r4`); the machine-sinking pass then moves it to the
top of the mission `if` block, i.e. before `getText`.  `r5` is then dead and
is reused for the `String` temporary address.  That reproduces the original
instruction for instruction.

Final source (case 1, in-game pause menu):

```cpp
        this->addButton(6, *Globals::gameText->getText(522), row, this->buttons, 0);
        int row2 = row + 2;
        this->addButton(19, *Globals::gameText->getText(59), row + 1, this->buttons, 0);
        int mission = Globals::status->getCurrentCampaignMission();
        if (mission == 154 || mission == 157 || mission == 158) {
            this->addButton(18, *Globals::gameText->getText(395), row2, this->buttons, 0);
        }
```

Semantics are unchanged: `row` is not modified between the declaration and
the use.

## 3. Measurements (all on top of the w6718 tree)

| variant | size | fuzzy |
|---|---:|---:|
| w6718 committed `char row2` inside the `if` | 9600 | 99.819277 |
| `int row2` inside the `if` | 9596 | 94.877975 |
| inline `row + 2` as the 3rd argument | 9596 | 94.877975 |
| `int row2` between `addButton(19)` and `int mission` | 9596 | 94.877975 |
| `int row2` + `String *text = getText(395)` (either order) | 9596 | 94.877975 |
| **`int row2` before `addButton(19)`** | **9600** | **100.000000** |

Miniature reproducer (`struct String` by value, `getText` external) shows the
same effect: declaring `row2` before the preceding `addButton` call yields

```
movw r0,#395
adds r4,r5,#2
bl   getText
add  r5,sp,#8
...
mov  r2,r5
mov  r3,r4
```

while every variant declared at/after the mission check sinks the ADD below
the `String` ctor.  Compiling the miniature with `-mllvm
-pre-RA-sched=source` also keeps the ADD before `getText`, confirming the
effect is the default SelectionDAG list scheduler sinking a rematerialisable
ADD; the predecessor-block live range is what pins it.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6724
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
```
