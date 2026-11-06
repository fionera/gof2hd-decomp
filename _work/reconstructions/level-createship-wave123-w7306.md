# `Level::createShip` — wave 123 (w7306): real-`new` body closes the last `.ARM.extab` deficit

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `1c8c9b20e` (wave-122 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7306`.

**Status: source change landed.** The two ship allocations are real `new`
expressions again (no placement `new`, no `try/catch` anchor), which reproduces
the original's **336 B cleanup-only LSDA record** (`ttype 0xff`, 57 rows, all
action 0). `.ARM.extab` becomes size- and base-exact (`0x26298` @ `0x1a3b78`)
and `.ARM.exidx` becomes **byte-identical**. The function's fuzzy score drops
73.9 -> 70.3 because the four live cleanup pointers re-seat Greedy RA's first
decision; that RA cascade is the remaining root cause (below).

## Result

```
                                   original        HEAD (placement+try)   landed (real new + Vector pos)
createShip fuzzy                   —               73.9                   70.3
createShip size                    2764 B          2764 B                 2764 B
createShip insns                   1007            1008                   1005
createShip LSDA record             336 B           328 B                  336 B
createShip LSDA rows               57              54                     57
createShip LSDA ttype              0xff            0x00 (type table)      0xff
.ARM.extab size/base               0x26298        0x26290 (8 B short)    0x26298 (exact)
.ARM.exidx differing bytes         0               1,327                  0
.text differing bytes              0               86,947                 87,197
.bss differing bytes               0               1                      0
avg_match                          —               99.94                  99.93
byte_exact / linked_exact          —               4506 / 4506            4506 / 4506
```

Full `check` with the landed body: `RATCHET PASS` — byte **4506**, linked
**4506**, size 4524/4524, data 843/843, imports 207, stubs/missing/extra 0,
lint `CLEAN ✓`, drift 0, symbol parity 5370/5370. `.rodata`/`.data`/`.got`/
`.dynsym`/`.dynstr`/`.hash`/`.rel.dyn` stay byte-identical; all 27 section
headers stay exact. The only differing section bytes are `.text` (87,197),
`.ARM.extab` (1,446), `.rel.plt` (6, pre-existing 3-entry addend permutation)
and `.note.gnu.build-id` (20).

## 1. Ground truth: the original is real `new`

Parsed with `_work/reconstructions/exidx_table.py` / `_work/reconstructions/`
`exidx_table.py --diff` (record 336 B @ `0x1ab09c`, LSDA `+0x0c`,
`lps=0xb0`, `ttype=0xff`, call-site table length 315, 57 rows, action 0):

```
orig : rec=336  rows=57  ttype=0xff  table_end=+336
landed: rec=336  rows=57  ttype=0xff  table_end=+336   (same address 0x1ab09c)
```

The four cleanup landing pads at `0xc0bba..0xc0bd6` are the classic
`new`-expression cleanups (`mov r8,r0; mov r0,{sl|fp|r9|r5}; b c0bd0;
bl operator delete; mov r0,r8; bl _Unwind_Resume`); no `__cxa_begin_catch`
anywhere. The original's cleanup registers are:

| allocation | register |
|---|---|
| `Player` / each `BoundingAAB` | `r5` |
| `PlayerFixedObject` | `r9` |
| `Array<BoundingVolume*>` | `fp` |
| `PlayerFighter` | `sl` |

`exidx_table.py --diff` now reports **0 functions with a differing extab
record length** (`total delta +0`), 3566 entries, CANTUNWIND 272 / inline 856 /
extab 2438 in both.

## 2. Landed source shape

`src/game/world/Level.cpp` (createShip only):

* `PlayerFighter *fighter = new PlayerFighter(...)`
* `PlayerFixedObject *fixed = new PlayerFixedObject(...)`
* `player->setEmpData(empPoints, empTime);` — no `try/catch` anchor
* `Vector pos = {(float) x, (float) y, (float) z};` and `pos.x/y/z` at the
  `PlayerFixedObject` / `BoundingAAB` ctor sites (the wave-122 shape)
* `fighter->setShipGroup(Globals::globals->getShipGroup(type, race, group),
  type, hostile)` — third argument is `hostile` (`[r7,#12]`), not `group`
  (`[r7,#16]`); the fixed ship keeps `false`/`false`
* the `if (Globals::status == nullptr) return nullptr;` RA anchor is kept
  (removing it shrinks the body 2764 -> 2748 and drops the score to 63.5)

The `try/catch` in HEAD was the RA anchor that kept `type` in `fp` and
`shipClass` in `r5 -> sl`; it necessarily carries a 328 B type-table LSDA.
Removing it is what buys the faithful 336 B cleanup-only record.

## 3. Variants tried this pass (official one-function scorer)

All keep the 336 B LSDA unless noted. `pos` = `Vector pos` at the ctor sites.

| variant | % | size | insns | prologue prefix |
|---|---:|---:|---:|---:|
| **landed: real new, `Vector pos`, if/else level/empPoints** | **70.3** | 2764 | 1005 | 4 |
| real new, no `Vector pos` (direct `x,y,z`) | 70.0 | 2764 | 1005 | 4 |
| landed + jitter declaration X,Y,Z (faithful y=2nd/z=3rd pairing) | 70.0 | 2764 | 1006 | 4 |
| landed + `zyx` coordinate declaration order | 70.3 | 2764 | 1005 | 4 |
| landed + `pos.x/y/z` in BoundingAAB | 70.3 | 2764 | 1005 | 4 |
| landed, no `Globals::status == nullptr` guard | 63.3 | 2748 | 988 | 4 |
| landed + ternary `level` | 68.3 | 2772 | 1008 | 4 |
| landed + ternary `empPoints` | 68.0 | 2772 | 1008 | 4 |
| landed + both ternaries | 68.4 | 2780 | 1011 | 4 |
| landed + both ternaries, no guard | 66.6 | 2764 | 994 | 4 |
| all-`int` coordinates (+ `(float)` at the ctors) | 61.4 | 2772 | 998 | 4 |
| `volumes` allocated before `fixed` | 61.5 | 2756 | 992 | 4 |
| `Vector pos` computed before `setEmpData` | 57.5 | 2776 | 1004 | 4 |
| real new + `try/catch` (any catch form) | 63.3 | 2772 | — | type-table LSDA |

The `Vector pos` form is the best real-`new` shape that keeps size 2764 and the
336 B record; the faithful all-`int` coordinates and the faithful jitter
pairing both cost score without recovering the prologue.

## 4. Remaining root cause (RA cascade)

The original's prologue keeps `type` in `fp`, `shipClass` in `r5 -> sl`, and
spills only `race`:

```
str r1,[sp,#60]   ; race
mov r9,r0         ; this
ldr r0,[pc,#572]
mov fp,r3         ; type
mov r5,r2         ; shipClass
add r0,pc
ldr r4,[r0]       ; status
...
ldr r0,[r7,#8]    ; waypoint
mov sl,r5         ; shipClass -> sl
cbz r0, ...
```

The landed body spills both `race` and `shipClass` and puts `type` in `sl`
(first difference at instruction 4: `str r2,[sp,#60]` vs `str r1,[sp,#60]`).
The four live cleanup pointers (`r5` Player/BoundingAAB, `r9` Fixed, `fp`
volumes, `sl` Fighter) change Greedy RA's first decision; the original reuses
the early `this`/`type`/`shipClass` registers for the later cleanup pointers
(`r9` this -> fixed, `fp` type -> x -> volumes, `sl` shipClass -> empPoints ->
fighter), whereas the landed body cannot. Downstream that also swaps
`fixed`/`volumes` (`fp`/`r9` vs the original's `r9`/`fp`) and re-seats the
waypoint/jitter stack slots (`wx/wy/wz` 28/48/44 vs 44/48/40; jitters
24/40/36 vs 36/32/28). No source-level declaration/ordering variant tried in
waves 119-123 pins the first Greedy decision while keeping the 336 B record;
the placement-`new` + `try/catch` body remains the only shape that matches the
prologue (first 94 instructions), but it costs the 328 B type-table LSDA.

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```
