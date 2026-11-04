# Level::createShip — pass 9 (wave 98, w4807)

**Status:** analysis only, no source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). Verifier score **74.379345 %**
(report rounds 74.4), `usize == osize == 2764`, 757 differing insns.
This pass continues `level-createship-pass8-w4705.md`.

Gate before and after (unchanged, no code change):

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
.ARM.extab 0x26294 (-4)  lint CLEAN  drift 0  RATCHET PASS
```

## 0. What this pass establishes

The committed source is **byte-identical to the original for the whole
prologue and for the whole `Player` ctor region**; the entire remaining
757-insn delta is confined to three places:

1. the placement of the `this` spill (`str.w r9,[sp,#56]`), one basic
   block too early (`0xc031c` vs `0xc0332`);
2. the spawn block `0xc03c8..0xc03f2` (the committed source's spawn is a
   *semantic hack*: two-jitter int + two-waypoint float, not the
   original's three waypoint+jitter ints);
3. the downstream register assignment in the two `shipClass` branches
   (`volumes` in `r9` vs `fp`, `z` in `sl` vs `r4`, `type`/`race`
   swapped between `r4`/`r6`).

No faithful source shape found in this pass beats 74.379345 %. The
committed source is a strict local optimum of the measured space.

## 1. Byte-identical regions (fresh objdump, masked by address)

```text
orig c0214..c031c  ==  ours c0214..c031c   (100 insns, prologue + waypoint
                                            loads + 3x rnd + level + hitpoints
                                            + type switch + mission==49..56)
orig c0336..c03c8  ==  ours c0336..c03c8   (empPoints tail + shipClass branch
                                            + options adjust + mission==0x9a
                                            + hardCoreMode + operator new +
                                            Player ctor)
```

So the parameter register map (`r9=this, fp=type, r5=shipClass, sl=shipClass,
r8=mission, r4=Globals::status, race -> [sp,#60]`), the frame layout, the
waypoint/rnd stack-slot map, the hitpoints/empPoints/empTime arithmetic and
the `Player` ctor call are all exact. "Reconstruct the prologue" is already
done — the prologue is byte-exact.

## 2. The original spawn, decoded exactly

```text
c03c8: ldr r2,[sp,#48]   ; waypoint->y
c03ca: ldr r3,[sp,#32]   ; 2nd rnd
c03cc: ldr r0,[sp,#44]   ; waypoint->x
c03ce: ldr r1,[sp,#36]   ; 1st rnd
c03d0: add r2,r3         ; wy + 2nd
c03d2: ldr r3,[sp,#40]   ; waypoint->z
c03d4: ldr r4,[sp,#28]   ; 3rd rnd
c03d6: add r0,r1         ; wx + 1st
c03d8: movw r1,#20000
c03dc: sub.w fp,r0,r1    ; fp = wx + 1st - 20000
c03e0: add r3,r4         ; wz + 3rd
c03e2: subs r4,r2,r1     ; r4 = wy + 2nd - 20000
c03e4: sub.w r8,r3,r1    ; r8 = wz + 3rd - 20000
c03e8: mov r0,r5
c03ea: mov r1,sl
c03ec: mov r2,r9
c03ee: blx Player::setEmpData(int,int)
```

With the authoritative slot map (see §3) this is exactly the faithful
source

```cpp
int x = wx + jitterX - 20000;   // wx@44 + jX@36
int y = wy + jitterY - 20000;   // wy@48 + jY@32
int z = wz + jitterZ - 20000;   // wz@40 + jZ@28
```

and **all three coordinates are `int`**: the three `vcvt.f32.s32` are
emitted after the call, duplicated in both branches (`0xc0412..0xc0422`
and `0xc06d0..0xc06d8`). The ctor receives `(fp, r4, r8)` =
`(wx+jX, wy+jY, wz+jZ)` = `(x, y, z)` in that order.

The committed source computes instead

```cpp
int x = jitterZ + jitterX - 20000;          // 2nd+3rd, int, sunk past the call
float y = (float)(wy + jitterY - 20000);    // wy + 1st, float
float z = (float)(wz + wx - 20000);         // wx + wz, float
```

i.e. pairs `(48,36) (44,40) (32,28)` instead of `(48,32) (44,36) (40,28)`.
It matches the *slot map* but not the *pairing*; the two `vcvt`s for
`y`/`z` move before `setEmpData`, and the int `x` moves after it. The
committed spawn block is therefore 4 bytes longer than the original's,
which is exactly offset by the 4-byte-earlier `this` spill (§4).

## 3. Slot map / FI order are exact

Original stores (objdump `0xc0240..0xc028c`):

```text
waypoint->x -> [sp,#44]   waypoint->y -> [sp,#48]   waypoint->z -> [sp,#40]
1st rnd     -> [sp,#36]   2nd rnd     -> [sp,#32]   3rd rnd     -> [sp,#28]
```

The committed build's stores are identical instruction-for-instruction.
In committed-source naming (`wz=wp->x, wy=wp->y, wx=wp->z`,
`jitterY=1st, jitterZ=2nd, jitterX=3rd`) the map is
`wy@48 wz@44 wx@40 jitterY@36 jitterZ@32 jitterX@28`, which is the same
physical map. The pass-8 note's FI order
(`wy=3 wz=4 wx=5 jitterZ=6 jitterX=7 jitterY=8`) and the hot/cold
`StackSlotColoring` split are therefore already correct; changing the
jitter call order, the declaration order or the assignment order does not
improve the score (see §6).

## 4. Residual #1 — the `this` spill is one block too early

```text
original                              committed
c0314  ldr   r0,[r4,#0]               c0314  ldr   r0,[r4,#0]
c0316  blx   Status::getLevel         c0316  blx   Status::getLevel
c031a  cmp   r0,#20                   c031a  cmp   r0,#20
c031c  ble.n c0322                    c031c  str.w r9,[sp,#56]   <- here
c031e  movs  r0,#20                   c0320  ble.n c0326
c0320  b.n   c0328                    c0322  movs  r0,#20
c0322  ldr   r0,[r4,#0]               c0324  b.n   c032c
c0324  blx   Status::getLevel         c0326  ldr   r0,[r4,#0]
c0328  add.w r0,r0,r0,lsl#2           c0328  blx   Status::getLevel
c032c  cmp   r6,#1                    c032c  add.w r0,r0,r0,lsl#2
c032e  add.w sl,r0,#40                c0330  cmp   r6,#1
c0332  str.w r9,[sp,#56]   <- here    c0332  add.w sl,r0,#40
c0336  str.w fp,[sp,#52]              c0336  str.w fp,[sp,#52]
c033a  bne.n c0352                    c033a  bne.n c0352
```

The `type` spill (`str.w fp,[sp,#52]`) is at the same address on both
sides; only the `this` spill differs. Because both forms are 4 bytes the
block shift cancels and the code re-syncs exactly at `0xc0336`.

This is **not** the PostRA Machine Sink. A filtered
`-mllvm -print-after-all` run shows the store is already in its final
block in the **Virtual Register Rewriter** dump (right after Greedy RA):

```text
bb.14 (%ir-block.65):                       ; the empPoints getLevel block
  2640B  t2CMPri killed renamable $r0, 20
  2648B  t2STRi12 killed renamable $r9, %stack.0   ; <-- this spill
  2656B  t2Bcc %bb.16, 13
...
bb.17 (%ir-block.73):                       ; the merge block
  2960B  t2CMPri renamable $r6, 1
  2968B  t2STRi12 renamable $r11, %stack.1  ; type spill
  2976B  t2Bcc %bb.19, 0
```

The `PostRA Machine Sink` dump (`-print-after-all`, pass 167466) is
byte-identical to the preceding `Machine Loop Invariant Code Motion`
dump, i.e. the sink does not touch either store. The placement is a
Greedy-RA live-range-split decision: `this` (vreg `%124`, live
`64B..26224B`) is split at the end of `bb.14` so `r9` is free for
`empTime` (`movw r9,#45000` / `#15000` in `bb.18`/`bb.19`). The original
splits the same interval at the end of `bb.17` instead. `(void)this;`,
`Level *self = this;`, `unsigned` on `mission`/`level`/`empPoints`, and
moving `armor` earlier all leave the score at 74.4 (the split does not
move).

## 5. Residual #2 — downstream register assignment

Everything from `0xc03f6` to the epilogue is the same instruction
*sequence* with a different register colouring:

```text
original : volumes = fp   z = r4   x = sl   y = r8   type = r4   race = r6
committed: volumes = r9   z = sl   x = r4   y = r6   type = r6   race = r4
```

This is the bulk of the 757 diffs (the 11 `BoundingAAB` constructions
alone account for ~90 replaced instructions, each `ldr.w r0,[fp,#4]`
vs `ldr.w r0,[r9,#4]`). It is a consequence of the spawn's mixed
`int`/`float` types: the committed `float y, float z` keep two
coordinates in `s16/s18` and free GP registers, so Greedy RA picks a
different colouring than the original's all-`int` spawn. It cannot be
fixed independently of the spawn.

## 6. Sweeps measured this pass (all with the official verifier)

* Faithful all-int spawn, no `try`: **61.7 %**, `usize 2752` (the plain
  `setEmpData` is hoisted above the spawn adds by the DAG scheduler).
* Faithful all-int spawn + the committed `try` anchor: **61.6..62.8 %**,
  `usize 2752..2772`; the prologue changes (`this`/`type` spilled at
  entry, `type` kept in `r9`, `race` in `sl`), so the mixed-type shape is
  what buys the exact prologue.
* Faithful pairing with the committed mixed types: all 6 coordinate
  permutations x 8 int/float combinations (48 variants) -> max **74.1 %**
  (`int x = wx+jX; float y = wy+jZ; float z = wz+jY`, `usize 2764`).
* Two-jitter-int family (int = any 2 of the 3 rnds, one float
  waypoint+jitter, one float two-waypoint, all coordinate orders, 54
  variants) -> max **74.4 %**, uniquely the committed
  `(jitterZ+jitterX, wy+jitterY, wz+wx)`.
* Declaration order / jitter order / assignment order sweeps: the
  committed `int wx, wz, wy;` + `jitterY, jitterZ, jitterX` +
  `wz=wp->x, wy=wp->y, wx=wp->z` is the unique optimum (74.4).
* `empPoints` split into two statements or `empTime = 15000` default +
  override: 74.4 (byte-identical instruction stream).
* `empTime` ternary, `empPoints` if-form, `level` if-form, `armor`
  inlined, `armor` hoisted, `hitpoints` adjust hoisted, `(void)this;` at
  four sites, `unsigned` on `mission`/`level`/`empPoints`: 60.5..74.4,
  none above 74.4.
* `try` variants (empty catch, `return nullptr`, `throw;`): 49.8..66.4.

## 7. Conclusion / next lever

The committed source is a strict local optimum. The prologue, slot map,
FI order, hitpoints/empPoints/empTime arithmetic and `Player` ctor are
already byte-exact. The next real lever is the **Greedy-RA live-range
split of `this`** (residual #1): if it can be moved from the end of
`bb.14` to the end of `bb.17`, the 4-byte block shift disappears and the
downstream colouring (`volumes= fp`, `z = r4`) may follow, which would
recover most of the 757 diffs. That split appears to be driven by the
spawn's `int`/`float` mix (which is itself what buys the exact
prologue), so a shape that keeps the mixed-type prologue *and* the
all-int spawn scheduling is the thing to look for. No such shape was
found in this pass.

The known semantic `PlayerFixedObject(race, type)` swap (74.5 %) is still
rejected: the binary passes `(type, race)` (`0xc0426 ldr r4,[sp,#52]` =
type, `0xc042a ldr r6,[sp,#60]` = race, `mov r1,r4`, `mov r2,r6`).

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4807
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

Scratch helpers under `/dev/shm/w4807`: `score.py` (official ratio),
`aligndiff.py` (opcode-aligned diff), `gen2.py` / `batch.py` /
`enum_spawn.py` / `enum2.py` (source-variant generators), `pacmd.sh`
(filtered `-print-after-all`), `vrw.txt` / `sink_before.txt` /
`sink_after.txt` (the RA dumps quoted in §4).
