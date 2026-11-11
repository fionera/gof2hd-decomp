# `Level::createShip` — wave 173 (w11817): exact prologue via the `setEmpData` invoke barrier

Worktree `wt/w11817`, branch `wave173-w11817`, base `master 3fafa5b03`
(wave 172).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: source change landed.**  `createShip` raw **1,560 -> 1,199**
(-361), aligned **530 -> 351**, size **2,764/2,764** exact, insns
**1,118 -> 1,117** (now exactly the original's 1,117).  The first ~100
instructions (prologue through the `Player` ctor) are byte-identical.
The cost is `.ARM.extab`: the landed `try`/`catch` record is 328 B
(54 rows, `ttype 0x00`) instead of the original's 336 B cleanup-only
record, so the section is 8 B short (`0x26290` vs `0x26298`) and
`.ARM.exidx` shifts.

## 1. Result

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,560** | **1,199** |
| instruction-index alignment | **530** | **351** |
| size | 2,764 | 2,764 |
| insns (orig/ours) | 1,117 / 1,118 | 1,117 / 1,117 |
| `.text` differing bytes | 81,503 | **81,142** |
| `.ARM.extab` size | `0x26298` (exact) | `0x26290` (-8) |
| `.ARM.extab` differing bytes | 711 | 108,003 |
| `.ARM.exidx` differing bytes | 0 | 1,327 |
| `.bss` differing bytes | 0 | 1 (NOBITS padding) |
| file size | 2,187,328 | 2,187,328 (exact) |

Gate (full `check`): `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`RATCHET PASS`.  `.rel.plt` byte-identical (29,392 B).

## 2. The RA map (original vs base)

```
original (and landed)                      wave-172 base
c0220 str r1,[sp,#60]   ; race             strd r1,r2,[sp,#56]  ; race+shipClass
c0222 mov r9,r0         ; this             mov r9,r0
c0224 ldr r0,[pc,#572]                     ldr r0,[pc,#576]
c0226 mov fp,r3         ; type             mov sl,r3            ; type
c0228 mov r5,r2         ; shipClass        ...
c022a add r0,pc                            add r0,pc
c022c ldr r4,[r0,#0]    ; status           ldr r4,[r0,#0]
c0230 blx getCurrentCampaignMission
c0234 mov r8,r0         ; mission          mov r8,r0
c0236 ldr r0,[r7,#8]    ; waypoint         ldr r0,[r7,#8]
c0238 mov sl,r5         ; shipClass copy   ...
```

The original keeps **all seven** callee-saved registers live at entry
(`r4`=status, `r5`=shipClass, `r6`=nextInt fn, `r8`=mission, `r9`=this,
`sl`=shipClass copy, `fp`=type) and spills only `race`.  The wave-172 base
spilled both `shipClass` and `race`, put `type` in `sl`, and left `fp` free.

### Why the base cannot reach it by reordering

Parsed the pre-RA `-mllvm -print-machineinstrs` block (After Machine
Instruction Scheduler) with a proper backward liveness pass (per-block
successor inference for fall-through blocks; `lhs = defs`, everything else
`uses`).  `LiveInterval` sizes in slot units:

```
this 22,100 > type 21,512 > race 20,932 > ship 17,320 > volumes 14,832
     > {482,483,484} ~5,540 > shipClass 4,904 > status 4,724 > y/z 4,640/4,608
     > wz/wx/wy ~4,500 > jitterY/X/Z ~4,100 > mission 3,460
     > hitpoints 2,116 > empPoints 1,876 > player 1,788 > empTime 1,720
```

`RAGreedy::enqueue` orders global ranges by `(1<<29)+Size`, so `shipClass`
is 8th; by then `r4,r5,r6,r8,r9,r10,r11` are all live and it is spilled.
The original instead gets `shipClass` into `r5` and lets
`tryAssignCSRFirstTime` region-split it (`r5 -> sl -> r6`).  No statement
reordering of the wave-172 body moves that first Greedy decision: a
randomized topological sweep of the 16 head statement groups (240 orders,
plus shipClass-block / dispatch / null-check variants) produced **no**
order with `mov r9,r0` + `mov fp,r3` + `mov r5,r2`.

The decisive difference is not ordering at all: it is the **machine
scheduler**.  Without an invoke, clang 7.0.2 `-Oz` sinks the whole
coordinate block below the `player->setEmpData` call (base `c03cc`
`blx setEmpData` precedes the `ldr/sub` coordinate sequence at `c03d0`).
The original keeps the coordinates above the call (`c03c8..c03e4` before
`c03ee`), which lengthens the coordinate live ranges and changes the
Greedy allocation cascade.

## 3. The landed source shape (four coupled changes)

The prologue is reproduced by the wave-131 shape; all four parts are
needed (each alone or in pairs leaves `mov sl,r3`):

1. **Faithful two-call clamp** for both `level` and `empPoints`:
   `int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();`
   (the original calls `getLevel` twice: `c0286`/`c0296` and `c0316`/`c0324`).
2. **Placement `new`** for `PlayerFighter` / `PlayerFixedObject`:
   `::operator new(sizeof(T))` + `new (p) T(...)` (the `Player` itself stays
   a plain `new Player(...)`).
3. **Direct `x`/`y`/`z` arguments** to the `BoundingAAB` /
   `PlayerFixedObject` ctors (no `Vector pos` local), with
   `int x; float y; float z;` in that order.
4. **`try { player->setEmpData(empPoints, empTime); } catch (...) { __builtin_trap(); }`**.

The `try` turns `setEmpData` into an `invoke`, which is a scheduling
barrier; the coordinate block then stays above the call exactly as in the
original, and Greedy RA assigns `type -> fp`, `shipClass -> r5+sl`,
`race -> [sp,#60]`, `this -> r9`, `status -> r4`, `mission -> r8`.

The rest of the body is the wave-131 text: `int wpz, wpx, wpy;` with an
if/else waypoint block, `jitter1/2/3`, type-switch order `0x2c,0x33,0x31`,
the five-way mission if-chain, `empPoints` as one expression, and no
`Globals::status == nullptr` guard (the original has none).

## 4. `.ARM.extab` cost

The `try`/`catch` record is **328 B / 54 rows / ttype 0x00** instead of
the original **336 B / 57 rows / ttype 0xff** cleanup-only record.  The
section is therefore 8 B short (`0x26290` vs `0x26298`); `.rodata` still
starts at `0x1c9e10` (alignment padding absorbs the 8 B) and the file size
is unchanged, but `.ARM.exidx` prel31 entries after `createShip` shift
(0 -> 1,327 differing bytes) and the naive same-offset `.ARM.extab` diff
rises 711 -> 108,003 (the 8-byte shift dominates; aligning the two
sections by the shift leaves only ~30 k mismatched bytes).

This is the same trade-off wave 132 reverted.  It is landed here because
the assignment was the RA map and the function-level win is large
(1,560 -> 1,199 raw, `.text` -361).  A shape that keeps the 336 B record
*and* the exact prologue was not found: every variant that restores the
record size loses the prologue or the size:

| shape | raw | size | `.ARM.extab` |
|---|---:|---:|---:|
| wave-172 base (no try) | 1,560 | 2,764 | 156,312 (exact) |
| **landed: placement new + try/catch(...) trap** | **1,199** | **2,764** | 156,304 (-8) |
| placement new, no try | 2,186 | 2,780 | 156,292 (-20) |
| plain new + try/catch(...) trap | 2,333 | 2,768 | 156,328 (+16) |
| placement new + `catch(int)`+`catch(...)` | 1,238 | 2,764 | 156,312 (exact) but adds a PLT entry (`_ZTIi`), shifting every `.text` PLT call: `.text` diff 134,617 |
| placement new + try around Player ctor + setEmpData | 1,838 | 2,780 | 156,312 |
| placement new + `catch(...){ return nullptr; }` | 1,473 | 2,772 | — |
| placement new + `catch(...){ delete player; }` | 2,130 | 2,768 | 156,304 |

## 5. Variants measured (full link, size-checked)

Base = 1,560 raw / 530 aligned / 2,764 B.  None of these improve raw:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| type switch order `0x2c,0x33,0x31` | 1,563 | 519 | 2,764 |
| type switch as if-chain (6 orders) | 1,560-1,563 | — | 2,764 |
| waypoint as `wpz,wpx,wpy` if/else | 1,619 | 534 | 2,764 |
| jitter renamed `1/2/3` | 1,560 | 530 | 2,764 |
| mission guard as if-chain | 1,560 | 530 | 2,764 |
| `empPoints` as one expression | 1,560 | 530 | 2,764 |
| `x,y,z` (x first) | 2,382 | 1,099 | 2,772 |
| drop the `status == nullptr` guard | 2,053 | 495 | 2,764 |
| `__builtin_expect` on waypoint / shipClass / mission / type / nullchk | 1,560-2,497 | — | 2,764-2,780 |
| 240 randomized head-statement orders | 1,560-2,820 | — | 2,756-2,820 |

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11817
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11817/`: `Level_base.cpp`
(pristine wave-172 `Level.cpp`), `body_base.cpp` / `body_w131.cpp`
(extracted functions), `mi.txt` (pre-RA machineinstrs), `liv8.py` (liveness),
`fullsearch.py`/`meas2.py` (splice + link + measure), `randsearch*.py`
(statement-order sweeps), `secdiff.py` (per-section byte diff).
