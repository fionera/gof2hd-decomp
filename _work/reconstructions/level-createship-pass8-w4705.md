# Level::createShip — pass 8 (wave 97, w4705)

**Status:** source change landed. Target `_ZN5Level10createShipEiiiP8Waypointbb`
(original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Verifier score
**74.1439 % -> 74.3793 %** (report rounds 74.1 -> 74.4). The stack-slot map
now matches the original exactly; `.text` is size-neutral (2764 B, 1007
instructions on both sides).

Gate before and after (unchanged):

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
.ARM.extab 0x26294 (-4)  lint CLEAN  drift 0  RATCHET PASS
```

This pass continues `level-createship-pass7-w4507.md`. It closes the residual
`StackSlotColoring` 3-cycle identified there: the committed build coloured
`C, 1st, 2nd` as `32, 40, 36`; the original colours them `40, 36, 32`.

## 1. The change

`src/game/world/Level.cpp`, `Level::createShip`, spawn block only:

```cpp
-    int x = wx + jitterX - 20000;
-    float z = (float) (wz + jitterZ - 20000);
-    float y = (float) (wy + jitterY - 20000);
+    int x = jitterZ + jitterX - 20000;
+    float y = (float) (wy + jitterY - 20000);
+    float z = (float) (wz + wx - 20000);
```

The declaration block (`int wx, wz, wy;` with `wz=A, wy=B, wx=C`), the jitter
declaration/call order (`jitterY, jitterZ, jitterX` = 1st/2nd/3rd rnd), the
`try { setEmpData } catch (...) { __builtin_trap(); }` anchor, the `new Player`
placement and the downstream ctor/volume code are untouched.

The change is size-neutral: `usize == osize == 2764`, and the instruction
count now matches too (`uinsn == oinsn == 1007`; the committed shape had
1008).

## 2. Why it works — the hot/cold split is the lever

`StackSlotColoring` colours the RA spill slots in descending
`LiveIntervals::getSpillWeight`, stable-sorted by frame-index (FI) order.
With the `try` around `setEmpData` the spawn is split by the invoke:

* values **loaded in the pre-invoke block** get weight `1 + f(hot block)`;
* values only loaded in the post-invoke block get `1 + f(cold block)`,
  which is `~2^-20` smaller.

So the pre-invoke values are coloured first, in FI order, then the
post-invoke values, in FI order. The target colour order is
`B, A, C, 1st, 2nd, 3rd`, i.e. the three waypoint values and the *first*
jitter must be hot, and the 2nd/3rd jitters must be cold.

The committed spawn (`x = C + 3rd` int, `z = A + 2nd` float,
`y = B + 1st` float) puts `{A, 2nd, B, 1st}` hot and `{C, 3rd}` cold, so
`C` can never reach slot 40 with the committed FI order
`C, B, A, 3rd, 1st, 2nd`.

The new spawn puts the two-waypoint sum in a float coordinate and the
two-jitter sum in the int coordinate:

```text
pre-invoke  (float) y = wy + jitterY   ->  {B, 1st}
pre-invoke  (float) z = wz + wx        ->  {A, C}
post-invoke (int)   x = jitterZ + jitterX -> {2nd, 3rd}
```

Pre-invoke therefore references exactly `{B, A, C, 1st}` and post-invoke
exactly `{2nd, 3rd}`.

## 3. Measured FI / colour order (fresh `-print-after-all` GRA + SSC)

`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb`
on the landed source, GRA frame-object DBG_VALUEs plus the entry-block
stores:

```text
FI order : wy=3  wz=4  wx=5  jitterZ=6  jitterX=7  jitterY=8
           (B)   (A)   (C)   (2nd)      (3rd)      (1st)
hot      : {wy, wz, wx, jitterY} = {B, A, C, 1st}
cold     : {jitterZ, jitterX}    = {2nd, 3rd}
```

`PrologEpilogInserter` lays the nine 4-byte objects out at `[sp, 60-4k]`
(frame `push {r4,r5,r6,r7,lr}` 20 + `stmdb {r7,r8,r9,sl,fp}` 20 +
`vpush {d0-d10}` 88 = 128), so the resulting map is

```text
original : wy@48  wz@44  wx@40  j1@36  j2@32  j3@28
landed   : wy@48  wz@44  wx@40  j1@36  j2@32  j3@28   (exact match)
committed: wy@48  wz@44  jY@40  jZ@36  wx@32  jX@28   (before)
```

The first three colour slots (`race@60, this@56, type@52`) and the prologue
`str r1,[sp,#60]; mov r9,r0; mov fp,r3; mov r5,r2` are unchanged, so the
`type->fp` prologue that the committed `try` shape buys is preserved.

## 4. Effect on the instruction stream

The zero-fill stores, the three rnd stores and the three spawn slot loads now
all match the original byte-for-byte in slot operands. The aligned diff
(difflib, autojunk=False) drops from 955 to 757 differing instructions; the
first remaining structural difference is the `this` spill placement at
`c031c` (ours) vs `c0332` (original):

```text
c031a  cmp   r0, #20
c031c  str.w r9, [sp, #56]     <- ours hoists the `this` spill here
c031e  ble.n ...
...
c0332  str.w r9, [sp, #56]     <- original spills it after add.w sl,r0,#40
```

That is a scheduler/RA placement difference in the `empPoints` region, not a
slot-colouring difference, and is left for a later pass.

## 5. Sweep evidence

All measurements use the official verifier (`tools/verify/verify.py`,
`autojunk=False`) on the full gold link.

* Declaration order / jitter order: the committed `decl = xzy`
  (`int wx, wz, wy;`) + `jitterY, jitterZ, jitterX` is still the unique
  optimum (all 36 combinations swept with the new spawn; only
  `xzy`+`yxz`/`yzx` reach 74.4).
* Spawn statement order: `x,y,z` (original Ghidra order), `y,z,x` and
  `x,z,y` all reach raw 74.379345; `z,y,x`/`z,x,y` are 74.28.
* Expression grouping: the target slot map needs the two-jitter int
  coordinate. Sweeping all `int = (j1,j2)/(j1,j3)/(j2,j3)` groupings x all
  two-waypoint float pairings x statement orders (108 variants) gives a
  ceiling of 74.4 and only `int = (2nd,3rd)` reaches it. Keeping one
  waypoint + one jitter per coordinate (the committed grouping, 90
  pairings) never reaches `C = 40`.
* `(void) x;` / `if (x == 0x12345678) {}` before the try: no change (74.4).
* Semantic `PlayerFixedObject(race, type)` swap: 74.5, but the original
  passes `(type, race)` (`0xc0426 ldr r4,[sp,#52]` = type,
  `0xc042a ldr r6,[sp,#60]` = race, `mov r1,r4`, `mov r2,r6`), so it is
  still not landed.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4705
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers under `/dev/shm/w4705`: `score.py` (official ratio for one
symbol), `score_raw.py` (unrounded ratio + insn counts), `slots.py`
(prologue slot-map parser), `harness.py`/`batch.py` (single-variant
build+score), `cmd.sh`/`probe_cur.py` (filtered `-print-after-all` GRA/SSC
dump and FI-order extraction).
