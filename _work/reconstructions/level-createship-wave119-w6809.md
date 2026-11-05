# Level::createShip — wave 119 (w6809): faithful spawn schedule found, 67.37 % still the 2764 B ceiling

**Status:** analysis only, no source change landed (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave119-w6809`, base `319bd4d30`. Worktree left clean; only this
note is committed.

## Result

```
                                   before (HEAD)        after
createShip fuzzy (raw)             67.366317            67.366317
createShip size                    2764 B               2764 B
createShip LSDA record             336 B / 57 rows      336 B / 57 rows  (exact)
createShip ttype                   0xff                 0xff
.ARM.extab size / addr             0x26294 / 0x1a3b78   0x26294 / 0x1a3b78
.ARM.exidx                         0x6f70  (exact)      0x6f70  (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
.rodata                            0x41c27 (post-table) 0x41c27
file size                          2,187,328 B          2,187,328 B
sections                           27                   27
byte_exact / linked_exact          4505 / 4505          4505 / 4505
imports / data / lint / drift      207 / 843 / CLEAN / 0  207 / 843 / CLEAN / 0
```

`check` on the final (unmodified) tree: **RATCHET PASS** — `verify: byte
4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843
of 843`, `sodiff: 0/0 imports 207`, lint CLEAN, drift 0.
The only remaining `.ARM.extab` delta is `Generator::createMission`
(orig 172, ours 168, -4); createShip's own record is exact at 336 B.

## What the residual actually is

The call sequence is **identical** (87 `bl`/`blx`, same targets in the
same order), the jump-table shape is identical, and the literal pool is
**byte-identical** (the two functions' last 260 bytes differ in exactly
one word, the `movw/movt` of a link-dependent GOT address at `+0x100`).
The whole gap is Greedy register allocation:

```
orig   c0220 str r1,[sp,#60]   ; spill race only
       c0226 mov fp,r3         ; type  -> fp
       c0228 mov r5,r2         ; shipClass -> r5
       c0238 mov sl,r5         ; shipClass -> sl (r5 needed for rnd)
       c02b4 mov r6,sl         ; shipClass -> r6
       c022c ldr r4,[r0,#0]    ; &Globals::status -> r4
       jitterX/Y/Z all spilled ([sp,#36]/[32]/[28])
ours   c0220 strd r1,r2,[sp,#56]  ; spill race AND shipClass
       c0228 mov sl,r3         ; type -> sl
       c022c ldr r6,[r0,#0]    ; &Globals::status -> r6
       jitterX -> fp (not spilled)
```

The original uses **7 callee-saved registers** (`r4`=status,
`r5`/`sl`/`r6`=shipClass, `r8`=mission, `r9`=this, `fp`=type) and spills
all three jitter values; ours uses only 4 (`r6`=status, `r8`=mission,
`r9`=this, `sl`=type) plus `fp`=jitterX and spills shipClass.  Everything
downstream (the 24 `BoundingAAB` calls, the `PlayerFighter`/
`PlayerFixedObject` argument setup, the capital-branch permutation, the
EH landing pads) follows from that one assignment.

Encoding consequence: the original is **852 narrow + 265 wide** (2764 B);
HEAD is **826 narrow + 278 wide** (2764 B).  The original's extra 26
narrow encodings are the `mov r5,r2` / `mov sl,r5` / `mov r6,sl` chain
and the low-register `BoundingAAB` argument moves.

## The faithful spawn schedule (new finding)

The original computes the three spawn coordinates as **ints** *before*
`setEmpData`, converting them at each ctor call site (capital branch
`c040c..c0424`, fighter branch `c06c0..c06d8`):

```
c03c8..c03e4  x = wx + jitterX - 20000  -> fp
              y = wy + jitterY - 20000  -> r4
              z = wz + jitterZ - 20000  -> r8
c03e8..c03ee  player->setEmpData(empPoints, empTime)
```

HEAD (`int x` / `float y,z`) lets the machine scheduler sink the spawn
computation *after* `setEmpData`, which costs the whole 13-instruction
block.  Placing the spawn **before** `Player *player = new Player(...)`
in the source with `int x, y, z` makes the scheduler reproduce the
original order exactly:

```
c03c8..c03e4  spawn (ints)
c03e8..c03ec  setEmpData
```

and it also reproduces the original prologue's `mov fp,r3` (type -> fp)
and the spawn register assignment x->fp, y->r4.  It scores **lower**
(62.33 %) only because the RA then picks `sl`=jitterX and `r4`=jitterZ
instead of `sl`=shipClass and `r4`=status, and the function grows to
**2792 B** (28 B over the original), which would change the file size.
So it cannot be landed without regressing the structural state.

## Measurements (official scorer, full gold link)

HEAD = faithful real-new body, `int x` / `float y,z`, waypoint decl
`int wy, wz, wx`, jitter call order X,Y,Z.  `types` is the x/y/z
`int`/`float` mix (`i`=int, `f`=float).  All rows 2764 B unless noted;
`rec` is createShip's `.ARM.extab` record length (336 B = exact in every
row below).

| variant | raw % | size | rec |
|---|---:|---:|---:|
| **HEAD** (`iff`, spawn after `new Player`) | **67.366317** | 2764 | 336 |
| `iff` + spawn before `new Player` | 62.151394 | 2788 | 336 |
| `iii` + spawn before `new Player` (faithful schedule) | 62.332506 | 2792 | 336 |
| `iii` + spawn before `new Player`, wp `wz,wx,wy` | 62.531017 | 2792 | 336 |
| `iii` + spawn before `new Player`, wp `wx,wz,wy` | 62.531017 | 2792 | 336 |
| `iii` + spawn after `new Player` | 63.126551 | 2792 | 336 |
| `iii` + spawn after `new Player`, wp `wz,wx,wy` | 62.990561 | 2788 | 336 |
| `iif` (x,y int; z float) | 64.015905 | 2780 | 336 |
| `ifi` (x,z int; y float) | 63.021869 | 2780 | 336 |
| `fii` (x float; y,z int) | 61.469712 | 2784 | 336 |
| `fif` (x,z float; y int) | 66.266867 | 2764 | 336 |
| `ffi` (x,y float; z int) | 65.567216 | 2764 | 336 |
| `fff` (all float) | 56.858847 | 2784 | 336 |
| `iff` spawn order `xzy` / `zxy` / `zyx` | 67.166417 | 2764 | 336 |
| `iff` spawn order `yxz` / `yzx` | 67.366317 | 2764 | 336 |
| `iff` wp form `ternary` | 66.766617 | 2764 | 336 |
| `iff` wp form `initif` | 67.366317 | 2764 | 336 |
| `iff` jitter form `assign` (decl then assign) | 67.366317 | 2764 | 336 |
| `iff` `level` as `if` instead of ternary | 63.963964 | 2756 | 336 |
| `iff` `long x` / `static_cast<float>` / `20000L` / `40000L` | 67.366317 | 2764 | 336 |
| `iff` `x = wx - 20000 + jitterX` / `jitterX + wx - 20000` | 67.366317 | 2764 | 336 |
| `iff` `double y` | 64.802796 | 2772 | 336 |
| `iff` int temporaries then float locals (`int ix..; int x=ix; float y=iy;`) | 67.366317 | 2764 | 336 |
| `iff` float arithmetic `(float)wy + (float)jitterY - 20000.0f` | 64.175386 | 2796 | 336 |
| `iff` int+float promotion `wy + jitterY - 20000.0f` | 64.303545 | 2776 | 336 |
| `iff` spawn inline in every ctor / BoundingAAB call (no locals) | 57.128762 | 2816 | 336 |
| `iff` spawn right after jitter (before `level`) | 58.893871 | 2792 | 336 |
| `iff` spawn before `armor` | 60.148883 | 2808 | 336 |
| `iff` `armor` before empPoints | 61.020307 | 2768 | 336 |
| `iff` `armor` before `switch (type)` | 63.110668 | 2784 | 336 |
| `iff` `armor` right after jitter | 62.400000 | 2756 | 336 |
| `iff` `armor` before mission check | 62.837163 | 2784 | 336 |
| `iff` `armor = 1000; if (hardCoreMode()) armor = 650;` | 67.366317 | 2764 | 336 |
| `iff` `if (!shipClass)` for the fighter branch | 67.366317 | 2764 | 336 |
| `iff` `field_0x25 = false` / `field_0x40 = true` | 67.366317 | 2764 | 336 |
| `iff` `static_cast<PlayerFixedObject *>` | 67.366317 | 2764 | 336 |
| `iff` `0` instead of `nullptr` in both ctors | 67.366317 | 2764 | 336 |
| `iff` `type == 51` instead of `0x33` | 67.366317 | 2764 | 336 |
| `iff` remove the `kiFighter` temp in the addObject block | 67.366317 | 2764 | 336 |
| `iff` swap the addObject and cargo blocks | 66.966517 | 2764 | 336 |
| `iff` invert the missionPtr condition (`==1 \|\| ==23` / empty else) | 67.366317 | 2764 | 336 |
| `iff` `__builtin_expect(missionPtr != 1 && != 23, 1)` | 67.366317 | 2764 | 336 |
| `iff` `__builtin_expect(type == 0x2c \|\| 0x31, 1)` | 67.366317 | 2764 | 336 |
| `iff` all 6 permutations of the `shipClass == 1` block | 67.366317 | 2764 | 336 |
| `iff` `Status &st = *Globals::status;` reference | 61.700000 | 2768 | 336 |
| `iff` `Status *st = Globals::status;` pointer | 61.700000 | 2768 | 336 |
| `iff` `AERandom &rnd = *Globals::rnd;` reference | 63.804294 | 2768 | 336 |
| `iff` `int wy, wz, wx;` declared before `mission` | 67.366317 | 2764 | 336 |
| `iff` jitter block before the waypoint block | 66.200000 | 2764 | 336 |
| `iff` jitter block before `mission` | 62.133995 | 2760 | 336 |
| `iff` `mission` read after the waypoint block | 64.171657 | 2768 | 336 |

Opcode-only similarity (registers+immediates masked): HEAD 89.91 %.
Raw objdump (pool data included) gives 767 of 1112/1099 instructions
equal, 69.38 % sequence ratio.

## Conclusion

67.366317 % is a very broad local optimum for the faithful real-new
2764 B body.  The one structural lever not previously recorded is the
**spawn-before-`new Player` + `int x,y,z`** shape: it reproduces the
original's pre-`setEmpData` spawn schedule, `type -> fp`, and the
x->fp / y->r4 spawn registers, and lands the instruction count at
1008 vs 1007 — but the Greedy allocator then spills `shipClass` and
keeps `jitterX`/`jitterZ` in `sl`/`r4` instead of `shipClass`/`status`,
so the fuzzy score drops to 62.33 % and the function grows to 2792 B.
No 2764 B variant beat HEAD, so no source change was landed.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6809/{score,run,gen,gen2,splice,tryvar,trybody,diffops,opscore}.py`,
`sweep_iii.py`, `orig.asm`, `ours.asm`, `iii.asm`, `ra_after.txt` (full
`-mllvm -print-after-all` dump of the NDK clang 7.0.2 compile).
