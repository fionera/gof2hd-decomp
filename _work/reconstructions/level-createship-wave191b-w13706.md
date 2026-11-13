# `Level::createShip` — wave 191b (w13706): spawn/allocator sweep, no landable win

Worktree `wt/w13706`, branch `wave191b-w13706`, base master `a51f7dc11`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13706`.

**Status: no landable win.  Tree left at the pristine baseline.**
Raw **1,165**, aligned **385**, size **2,764/2,764** exact, insns
1,117/1,117, `.text` contentdiff **71,354**, `.ARM.extab` **156,312**
(contentdiff **577**), `.ARM.exidx` contentdiff **0**, `.rodata`/`.data`/
`.got`/`.rel.plt` byte-identical.  `check` -> **RATCHET PASS**
(byte_exact 4508/4524, linked 4508, imports 207, lint CLEAN, drift 0,
parity 5370/5370).

## 0. Baseline re-verified

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1165 aligned=385 size=2764/2764
python3 scratch/w13706/secdiff.py cmake-build-match/libgof2hdaa.so
# .text 71354  .ARM.extab 577  .ARM.exidx 0
```

## 1. Exact byte map of the 1,165 residual bytes (16-B block histogram)

`scratch/w13706/bdiff.py` (offsets are from `0xc0214`):

| offset | bytes | block |
|---|---:|---|
| `0x0100..0x012f` | 16 | `empPoints` clamp + `this` spill hoist |
| `0x01b0..0x024f` | 127 | spawn coords + `Player` ctor + `new` + `Array` ctor |
| `0x0260..0x056f` | 696 | race-0 type-14 hull: **register rotation** |
| `0x0620..0x0850` | 14 | race-1 hull: literal-pool addends only |
| `0x0860..0x09cf` | 307 | race-2/3 hull: register rotation + ASL reorder |
| `0x0ac0` | 1 | literal-pool addend |

The race-1 hull block (`0xc0834..0xc0a64`) is **instruction-for-instruction
identical** apart from `volumes`: the original keeps it in `fp`, ours in
`r9` (`ldr.w r0,[fp,#4]` vs `ldr.w r0,[r9,#4]`), plus one `ldr r4,[sp,#52]`
(type) vs `[sp,#60]` (race) and the resulting `b.n` displacement.  All 14
residual bytes there are literal-pool addends.  So the hull code itself is
already correct; only the register assignment of `fixed`/`volumes`/`x`/`z`
is wrong.

## 2. Root cause: `this` lands in `sl`, not `r6`

The original keeps `this` in `r6` after the `shipClass` compares
(`c03f6 cmp r6,#0; c03f8 beq; c03fc cmp r6,#1; c03fe ldr r6,[sp,#56]`)
and spills it at `c0332` (merge block, after `add.w sl,r0,#40`).  Ours
keeps `this` in `sl` (`c0400 ldr.w sl,[sp,#56]`) and spills it at
`c031c` (inside the `cmp #20` block).  Because `sl` is taken by `this`,
`x` gets `r6` and `z` gets `sl` (via `vmov r3,s16; mov sl,r3`), which
rotates the whole race-0 type-14 hull block (696 B).  In race-1/2/3 `this`
is dead by then, so `x` does get `sl` and the block matches.

The chain is: spawn block after `setEmpData` -> `shipClass` still in `r6`
at `c0410` -> `this` must use `sl` -> `x`/`z` rotate -> hull diff.

## 3. The spawn form is a unique size-2,764 point

The landed source is deliberately non-faithful:
`float z = (float)(wpx+wpy-20000)` **before** `setEmpData`, then
`int x = wpz+jitter3-20000; float y = (float)(jitter1+jitter2+jitter3-20000)`
**after**.  Every faithful all-int form (x=wpx+j3, y=wpy+j1, z=wpz+j2,
computed before `setEmpData`) is **size 2,752**: clang sinks the three
int adds past the call and drops one set of `vcvt` (original duplicates
the conversions in both ctor branches).  Reordering the three
declarations around the guard is also size-fatal:

| spawn form | raw | aligned | size |
|---|---:|---:|---:|
| baseline (`z` float before; `x` int, `y` float after) | **1,165** | **385** | **2,764** |
| `x` int before (vC) | 1,166 | 385 | 2,764 |
| `x`,`z` before (z4) | 1,166 | 385 | 2,764 |
| `x`,`y`,`z` all before, faithful ints (vB) | 2,297 | 487 | 2,752 |
| `x`,`y`,`z` all after, faithful ints (vA) | 2,297 | 487 | 2,752 |
| `x`,`y` before (vD) | 1,990 | 382 | 2,760 |
| `z` int before (t1) | 2,518 | 644 | 2,740 |
| `z`,`x` int before (t3) | 2,279 | 636 | 2,764 |
| `y` faithful float after (t4) | 2,362 | 485 | 2,760 |
| `y` int after (t5/t6) | 2,292 | 514 | 2,756 |
| `y = j1+j2-20000` (ya) | 1,166 | 450 | 2,764 |
| `y = j2+j3-20000` (ye) | 2,054 | 511 | 2,764 |
| `x = wpz+j1-20000` (xf) | 1,165 | 385 | 2,764 |
| all 5 other spawn declaration orders | 1,990 | 382 | 2,760 |
| `z` = wpz+j2 / wpx+j3 / wpy+j1 / ... (za..zf) | 2,152..2,573 | 485..1,351 | 2,700..2,752 |

`x = wpz+jitter3` and `x = wpz+jitter1` are codegen-identical (1,165/385);
the compiler only cares about the operand *shape*, not which stack slot.
So the current spawn is a strong local optimum.

## 4. Clamp: `emp40` matches the original shape but not the spill

The original clamp is `min(getLevel(),20)*5+40`; ours is the folded
`(lv<=20 ? lv*5 : 100)+40`.  `40 + e + e*4` (w13683's `emp40`) reproduces
the original instruction sequence exactly, but the `this` spill stays at
`c031c`, so the block costs 26 diff bytes instead of 16:

| clamp form | raw | aligned | extab |
|---|---:|---:|---:|
| baseline folded | **1,165** | **385** | 577 |
| `emp40` (`40+e+e*4`) | 1,175 | 381 | 577 |
| `emp40` + `setBV` first | 1,181 | 377 | 577 |
| `emp40` + `setBV`+ASL first | 1,189 | **368** | 576 |
| `emp40` + `x`,`z` before | 1,176 | 381 | 577 |
| `emp40` + `lv` temp | 2,176 | 463 | - |
| `int armor` before clamp | 2,464 | 543 | - |

`emp40` is the correct source shape (and a prerequisite for any future
`bb.20` spill fix worth ~-26 raw) but is not a win on its own.

## 5. Tail order

`setBV` is called last in our source but immediately after the hull switch
in the original (`c0b38`, before `getShipGroup`).  The tail
(`0xc0b2c..0xc0b8e`) is already byte-identical in the baseline, so the
faithful order is not free:

| tail form | raw | aligned | extab |
|---|---:|---:|---:|
| baseline (`setBV` last) | **1,165** | **385** | 577 |
| `setBV` first (s1) | 1,171 | 381 | 577 |
| `setBV`+`ArraySetLength(3)` first (s14) | 1,179 | 372 | 576 |
| inline `fixed->geometry` (s8) | 1,167 | 385 | 577 |
| inline `(float)x` (s6) | 1,170 | 386 | 577 |

`setBV` first perturbs the race-1 allocation (+6 raw) without touching
the already-exact tail.

## 6. Guards / LSDA

The original has **no** `EmpGuard`s (its 28 landing pads are exactly the
28 `new`s).  Our three fake guards are an LSDA-size hack.  Moving the
`setShipGroup` guard onto `getShipGroup` gives a near-tie:

| guard form | raw | aligned | extab | exidx |
|---|---:|---:|---:|---:|
| baseline | **1,165** | **385** | 577 | 0 |
| guard `getShipGroup` instead of `setShipGroup` (h1) | 1,166 | 383 | **576** | 0 |
| h1 + `setBV` first | 1,172 | 379 | 576 | 0 |
| h1 + ASL first | 1,174 | 374 | 576 | 0 |
| h1 + `setBV`+ASL first | 1,180 | 370 | 576 | 0 |
| h1 + `setBV`+ASL+`emp40` | 1,190 | **366** | 576 | 0 |
| drop `setShipGroup` guard (g3) | **1,161** | 405 | 156,304 | 1,327 |
| no `setEmpData` guard (g1) | 2,182 | 483 | - | - |

`g3` is the only raw improvement (1,161) but shrinks `.ARM.extab` by 8 B
and makes `.ARM.exidx` non-zero, so it is not landable.  `h1` is the best
admissible guard form: raw +1, aligned -2, extab -1 (a wash, not landed).

## 7. Cargo / race-2 ASL / declaration forms (all neutral or worse)

* Cargo `if` chain: the original tests `0x2c, 0x33, 0x31`; ours tests
  `0x33, 0x31, 0x2c` (the two `delete cargo` bodies are merged and the
  `ship = fighter` move is free because both live in `fp`).  Reordering
  the source (`r1`, `r4`, `r7`, `r8`) all canonicalise to 1,165/385;
  `r2` (0x33 first) explodes to 2,032/1,288.
* Race-2 `ArraySetLength(3)` is emitted **after** the first
  `BoundingAAB` store in ours but **first** in the original.  Forcing it
  with `volumes->data()` (e3), a local `vd` (e1, size 2,760), or
  temporaries (e4) does not reproduce the original order; moving it in
  the source (e2/e5) is worse (1,230/1,660).
* `const` on `x`/`y`/`z`/`fx`/`fixed`/`volumes`, `auto`, `static_cast`,
  `Level *self = this`, `this->lodManager`, `this->missionPtr`,
  `int mp = missionPtr`, `LODManager *lm = lodManager`, `bool isFighter/
  isFixed`, local `race`/`type` copies, `!shipClass`, `ship` declared
  before the clamp, `ship = nullptr` moved: **all byte-identical to the
  baseline** (1,165/385/2,764).
* `PlayerFixedObject *ship` (typed phi) 2,337/2,788; `KIPlayer *fixed`
  1,165/385; regular `new` both 2,096/580/2,764.

### 7.1 Faithful-semantics spawn forms (final sweep)

The landed spawn block is deliberately non-faithful (`z = wpx+wpy`,
`x = wpz+j3`, `y = j1+j2+j3`).  Re-running the sweep with the *original*
expressions (`x = wpx+j3`, `y = wpy+j1`, `z = wpz+j2`) confirms the
non-faithful block is the only size-2,764 point near the baseline:

| spawn form | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline (non-faithful, `z` before; `x` int, `y` float after) | **1,165** | **385** | **2,764** | 577 |
| faithful `z` before; `x` int, `y` float after | 1,174 | 395 | 2,764 | 577 |
| faithful `y`,`z` before; `x` int after | 1,191 | 432 | 2,764 | 577 |
| faithful all-int before (`vB`) | 2,297 | 487 | **2,752** | 605 |
| all-int before + `emp40` | 2,293 | 485 | **2,752** | 605 |
| all-int before + `emp40` + `setBV` first | 2,291 | 482 | **2,752** | 605 |
| all-int before + `setBV` first | 2,295 | 484 | **2,752** | 605 |
| all-**float** before (`float x/y/z = (float)(...)`) | 2,467 | 989 | **2,732** | 1326 |

Every all-int form is exactly 12 B / 7 insns short of the original
regardless of clamp/tail order: clang sinks the three adds past the
`setEmpData` invoke and the block never recovers the original
pre-invoke placement.  The all-float form is 32 B short.  So the
size-2,764 constraint really does pin the current mixed `float z` /
`int x` / `float y` shape, and with it the `this`-in-`sl` allocation
that rotates the hull blocks.

## 8. Conclusion

The remaining 1,165 bytes are two register rotations (race-0 type-14
696 B, race-2/3 307 B) and the spawn block (127 B), all driven by the
`this`-in-`sl` allocation, which is itself locked by the size-2,764
spawn form.  The only aligned win at raw cost +1 is `h1` (guard
`getShipGroup`), aligned 385 -> 383 and extab 577 -> 576; it is a wash,
so nothing was landed.  A future pass should target the Greedy-RA spill
split point (`bb.17` -> `bb.20`) or a way to keep `this` in `r6` without
moving the spawn block.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13706
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1165 aligned=385 size=2764/2764
python3 scratch/w13706/secdiff.py cmake-build-match/libgof2hdaa.so
python3 scratch/w13706/bdiff.py cmake-build-match/libgof2hdaa.so
python3 scratch/w13706/adiff.py cmake-build-match/libgof2hdaa.so
```

Scratch helpers (all in `scratch/w13706`): `secdiff.py` (section content
diff), `bdiff.py` (16-B raw histogram), `adiff.py` (instruction-category
diff), `idiff.py`, `try.py` (splice + build + measure), `sweep.py`
(batch variant runner), `cc.sh` (single-TU compile with arbitrary
`-mllvm` flags), and the `vA/vB/vC/.../hh5` plus `fl_*`/`g_*`
spawn-sweep variant files.

## 10. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
