# Level::createShip — wave 119 (w6795): real-new RA sweep, no source win

**Status:** analysis only, no source change landed (2026-11-05). Worktree
`wt/w6795`, branch `wave119-w6795`, base `075f49c72`. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).

## Result

```
                                   before (HEAD)        after
createShip fuzzy (raw)             67.400000            67.400000
createShip size                    2764 B               2764 B
createShip LSDA record             336 B (exact)        336 B (exact)
.ARM.extab size / addr             0x26294 / 0x1a3b78   0x26294 / 0x1a3b78
.ARM.exidx                         0x6f70  (exact)      0x6f70  (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
imports / data / lint / drift      207 / 843 / CLEAN / 0  207 / 843 / CLEAN / 0
```

`check` on the final (unmodified) tree: **RATCHET PASS** — `verify: byte
4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843
of 843`, `sodiff: 0/0 imports 207`, lint CLEAN, drift 0.
`_work/reconstructions/exidx_table.py --diff` reports createShip's
`.ARM.extab` record at 336 B in both binaries; the only remaining record
delta is `Generator::createMission` (orig 172, ours 168, -4).

## What the residual actually is

The call sequence is **identical** (87 `bl`/`blx`, same targets in the
same order), the jump-table shape is identical, and the spawn/stat
arithmetic is instruction-for-instruction the same modulo register
names. The whole 32.6-point gap is Greedy register allocation:

```
orig   c0220 str r1,[sp,#60]   ; spill race only
       c0226 mov fp,r3         ; type  -> fp
       c0228 mov r5,r2         ; shipClass -> r5
       c0238 mov sl,r5         ; shipClass -> sl (r5 needed for rnd)
       c022c ldr r4,[r0,#0]    ; &Globals::status -> r4
ours   c0220 strd r1,r2,[sp,#56]  ; spill race AND shipClass
       c0228 mov sl,r3         ; type -> sl
       c022c ldr r6,[r0,#0]    ; &Globals::status -> r6
```

Everything downstream (the 24 `BoundingAAB` calls, the
`PlayerFixedObject`/`PlayerFighter` argument setup, the capital-branch
permutation, the `movs r6,#0` vs `mov.w sl,#0` rematerialisation of the
`0.0f` BoundingAAB argument) follows from that one assignment. Normalising
the register names alone scores ~83.8 %.

The allocator's choice is driven by LLVM 7 `CalcSpillWeights`:
`weight = UseDefFreq / (Size + 25*InstrDist)` and `RegAllocGreedy`'s
priority `(1<<31) | (1<<29) | Size` for global ranges (longest live
interval first, `| (1<<30)` when a physreg copy hint exists). Because
`this`, `type`, `shipClass` and `race` all carry a physreg hint they are
allocated before `&Globals::status`; in the original `&Globals::status`
still wins r4, which means its live interval (or the hinted ranges'
sizes) differ from ours by a few instructions. The 13-instruction count
delta (1007 vs 994) is entirely in the spawn/`setEmpData` region.

## Measurements (official scorer, full gold link)

HEAD = faithful real-new body, `int x` / `float y,z`, waypoint decl
`int wy, wz, wx`, jitter call order X,Y,Z. All rows 2764 B unless noted.
Every row was rebuilt and gold-linked; `rec` is createShip's
`.ARM.extab` record length.

| variant | raw % | size | rec |
|---|---:|---:|---:|
| **HEAD** | **67.400000** | 2764 | 336 |
| spawn all-`float` (`fff`) | 56.9 | 2784 | 336 |
| spawn `int x,y,z` (`iii`) | 63.1 | 2792 | 336 |
| spawn `float x`, `int y,z` (`fii`) | 61.5 | 2784 | 336 |
| spawn `int x`, `int y`, `float z` (`iif`) | 67.4 | 2764 | 336 |
| spawn `int x`, `float y`, `int z` (`ifi`) | 63.0 | 2764 | 336 |
| spawn `float x`, `int y`, `float z` (`fif`) | 66.3 | 2764 | 336 |
| spawn `float x,y`, `int z` (`ffi`) | 65.6 | 2764 | 336 |
| jitter decl order XYZ / XZY | 67.4 | 2764 | 336 |
| jitter decl order YXZ / YZX / ZXY / ZYX | 67.3 | 2764 | 336 |
| waypoint assignment order (all 6 perms) | 67.4 | 2764 | 336 |
| `hitpoints *= ...` / `empPoints *= 3` / `empTime = 45000` reorders (3) | 67.4 | 2764 | 336 |
| `if (type == 14) hitpoints *= 25; else hitpoints *= 5;` | 67.4 | 2764 | 336 |
| `empTime = 15000; if (shipClass == 1) ...` | 67.4 | 2764 | 336 |
| `switch (shipClass)` for the empTime block | 67.4 | 2764 | 336 |
| `empPoints` split into two statements / `40 + ...` | 67.4 | 2764 | 336 |
| `int elevel` + `if` for empPoints | 64.5 | 2756 | 336 |
| `int level` + `if` (instead of ternary) | 64.0 | 2756 | 336 |
| `level` inverted ternary | 67.3 | 2764 | 336 |
| `hitpoints` split into two statements / reordered | 67.4 | 2764 | 336 |
| `armor` inlined into the `new Player` call | 62.9 | 2752 | 336 |
| `int armor = 1000; if (hardCoreMode()) armor = 650;` | 67.4 | 2764 | 336 |
| mission check as `(>=49 && <=52) \|\| ==56` | 67.4 | 2764 | 336 |
| mission check as `switch (mission)` | 67.4 | 2764 | 336 |
| alien-orbit as `if (mission == 0x9a && (inAlienOrbit() & (race == 9)))` | 67.4 | 2764 | 336 |
| `AEGeometry *geo = nullptr;` for the ctor geometry arg | 67.4 | 2764 | 336 |
| `0` instead of `nullptr` for the geometry arg | 67.4 | 2764 | 336 |
| `KIPlayer *ship = nullptr;` moved before `setEmpData` | 67.4 | 2764 | 336 |
| `bool capital = shipClass == 1;` at function top | 67.4 | 2764 | 336 |
| `int sc = shipClass;` at function top | 67.4 | 2764 | 336 |
| `bool isFighter = shipClass == 0;` at function top | 67.4 | 2764 | 336 |
| all locals declared at the top of the function | 67.4 | 2764 | 336 |
| `float opt` / `float hpf` temporaries for the hitpoints adjust | 67.4 | 2764 | 336 |
| `hitpoints * (1.0f + opt)` | 64.2 | 2772 | 336 |
| `unsigned hitpoints` | 67.1 | 2764 | 336 |
| `unsigned mission` / `(unsigned) shipClass` | 67.4 | 2764 | 336 |
| `if (shipClass != 1)` for the fighter branch | 34.7 | 2700 | 340 |
| `if (empPoints != 0) { setEmpData }` | 65.3 | 2776 | 336 |
| `if (empTime != 0) { setEmpData }` | 67.4 | 2764 | 336 |
| `if (player != nullptr) { setEmpData }` | 67.4 | 2764 | 336 |
| statement order permutations of {alien, armor, new Player, spawn} (18) | 54.7–62.8 | 2752–2800 | 336 |

### `try` anchor (the wave-117 shape) does not fit the exact LSDA

The wave-117/108 shape (`placement new` + `try { setEmpData } catch(...)`)
pins the allocator exactly (`status→r4`, `type→fp`, `shipClass→r5`) and
scores 73.5 %, but its createShip LSDA record is 328 B (`.ARM.extab`
0x2628c). With real `new` the same `try` anchor keeps the RA but grows
the record:

| variant | raw % | size | rec |
|---|---:|---:|---:|
| try/trap around `setEmpData` (real new) | 63.5 | 2768 | 352 |
| try/empty catch around `setEmpData` | 28.6 | 2756 | 352 |
| try/trap around `new Player` | 62.9 | 2780 | 352 |

So the RA fix and the exact 336 B LSDA are mutually exclusive with the
`try` anchor: any `try` adds a call-site row.

## Conclusion

67.4 % is the ceiling found for the faithful real-new body. The plateau
is very broad: declaration order, statement order inside the
`shipClass == 1` block, aliases, `unsigned`/`bool` spellings, and
`nullptr`/`0` are all coalesced away by Greedy RA. The only levers that
move the number are (a) the `int`/`float` mix of the spawn coordinates
(best is `iif`, the current source) and (b) an EH `try` anchor, which
breaks the 336 B LSDA. No variant beat 67.4 % while keeping the record
exact, so no source change is landed.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

Scratch helpers (not committed): `/dev/shm/w6795/{score,side,runv,rec,lsda}.py`,
`sweep1..13.py`, `orig.asm`, `ours.asm`, `iii.asm`, `fff.asm`, `mir_cs.txt`.
