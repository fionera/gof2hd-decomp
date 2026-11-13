# `Level::createShip` — wave 191b (w13683): the original clamp shape found; no raw win

Worktree `wt/w13683`, branch `wave191b-w13683`, base master `b221b3826`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13683`.

**Status: no landable raw win.  Tree left at the pristine baseline.**
Raw **1,164**, aligned **400**, size **2,764/2,764** exact, insns
1,117/1,117, `.text` contentdiff **71,247**, `.ARM.extab` **156,312**
(contentdiff **577**), `.ARM.exidx` contentdiff **0**, `.rel.plt`
byte-identical.  `check` -> **RATCHET PASS** (byte_exact 4508/4524,
linked 4508, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## 0. New result: the original `empPoints` clamp is `40 + e + e*4`

Every earlier pass concluded the clamp was irreducible: the faithful
`(getLevel() > 20 ? 20 : getLevel()) * 5 + 40` always folds `20 * 5`
into the true arm (`movs r0,#100`), while the original keeps
`movs r0,#20` and a **shared** `add.w r0,r0,r0,lsl#2` after the merge
(`c031e..c0328`).  The fold is an InstCombine canonicalisation, not a
backend size choice.

The algebraic form **`40 + e + e*4`** (where `e` is the clamped level)
is not distributed by InstCombine, so clang 7.0.2 emits the original
shape.  Verified in isolation (`t4.cpp`, `f23`):

```
cmp  r0,#20
ble  .Lget
movs r0,#20
b    .Lmerge
.Lget: bl g
.Lmerge: add.w r0,r0,r0,lsl#2
adds r0,#40
```

and in the real function the whole clamp block becomes byte-identical
to the original **except** the `this` spill:

```
original c0314..c033a            w13683 `emp40` c0314..c033a
ldr  r0,[r4]                     ldr  r0,[r4]
blx  getLevel                    blx  getLevel
cmp  r0,#20                      cmp  r0,#20
ble  c0322                       str.w r9,[sp,#56]   <-- spill (ours)
movs r0,#20                      ble  c0326
b    c0328                       movs r0,#20
ldr  r0,[r4]                     b    c032c
blx  getLevel                    ldr  r0,[r4]
add.w r0,r0,r0,lsl#2             blx  getLevel
cmp  r6,#1                       add.w r0,r0,r0,lsl#2
add.w sl,r0,#40                  cmp  r6,#1
str.w r9,[sp,#56]   <-- spill    add.w sl,r0,#40
str.w fp,[sp,#52]                str.w fp,[sp,#52]
bne  c0352                       bne  c0352
```

If the `str.w r9` were moved from `c031c` to `c0332` the block would be
byte-exact.  The spill is still inserted by Greedy RA at the end of
`bb.17` (the `cmp #20` block), not at the end of the merge block
`bb.20`; the downstream allocation is otherwise **identical to the
current baseline** (per-64B block diff below), so the clamp is the only
regression.

## 1. Per-64B block diff proves the downstream is unchanged

`scratch/w13683/blockdiff.py` (baseline vs `emp40`):

| offset | baseline | emp40 |
|---|---:|---:|
| 0x100 (clamp) | 16 | **26** |
| 0x180 (spawn) | 11 | 11 |
| 0x1c0 | 63 | 63 |
| 0x200..0x9c0 | identical | identical |
| total | 1,164 | 1,174 |

So `emp40` changes **only** the clamp block (+10 raw) and leaves the
hull/spawn register rotation untouched.  A source lever that moves the
`this` spill into `bb.20` would therefore be worth roughly **-26 raw**
(1,164 -> ~1,138) with no other change.

## 2. Measured this session (all size-exact 2,764 unless noted)

Baseline re-verified: raw **1,164**, aligned **400**, `.text` 71,247,
`.ARM.extab` 577, verify pct **70.2**.

### 2.1 The clamp form

| variant | raw | aligned | size | extab | pct |
|---|---:|---:|---:|---:|---:|
| baseline folded `(lv<=20?lv*5:100); +=40` | **1,164** | **400** | 2,764 | 577 | 70.2 |
| `emp40` = `40 + e + e*4` | 1,174 | 396 | 2,764 | 577 | 70.4 |
| `formA` `(lv<=20?lv:20)*5+40` | 1,167 | 398 | 2,764 | 577 | - |
| `formB` `(lv>20?20:lv)*5+40` | 1,174 | 396 | 2,764 | 577 | - |
| `formC..G` (separate `*=5`, `+40`, `5*e`, `cap` var) | 1,174 | 396 | 2,764 | 577 | - |
| `emp_tmp40` (`int empLevel` temp) | 1,174 | 396 | 2,764 | 577 | - |
| `emp_if40` (single call + `if`) | 2,185 | 471 | **2,756** | - | - |

All faithful forms converge on 1,174 / 396: the clamp is fixed but the
spill is still in `bb.17`.

### 2.2 `emp40` + tail/order levers (all size-exact)

| variant | raw | aligned | extab |
|---|---:|---:|---:|
| `emp40` | 1,174 | 396 | 577 |
| `emp40` + `x` before guard | 1,175 | 396 | 577 |
| `emp40` + `x`,`z` before guard | 1,175 | 396 | 577 |
| `emp40` + ASL first (race-0 non-14) | 1,182 | **387** | **576** |
| `emp40` + `setBV` first | 1,180 | 392 | 577 |
| `emp40` + `ship` after `setEmpData` / before spawn / early | 1,174 | 396 | 577 |
| `emp40` + `Level *self = this` (2 positions) | 1,174 | 396 | 577 |
| `emp40` + `(void)this` | 1,174 | 396 | 577 |
| `emp40` + `empTime = 15000; if(...)` | 1,174 | 396 | 577 |
| `emp40` + `int sc = shipClass` | 1,174 | 396 | 577 |

`emp40 + ASL first` is the best structural form seen (aligned 387,
extab 576, pct **71.0**) but raw is +18.

### 2.3 Spawn forms (with the folded clamp unless noted)

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline hack (`z=wx+wy` pre, `x=wz+j3`, `y=j1+j2+j3`) | **1,164** | **400** | 2,764 | 577 |
| w5006 spawn (`x=j1+j2`, `y=wy+j3`, `z=wx+wz` pre) | 1,182 | 400 | 2,764 | 577 |
| `emp40` + w5006 spawn | 1,192 | 396 | 2,764 | 577 |
| faithful all-int `x=wx+j3,y=wy+j1,z=wz+j2` | 2,297 | 489 | **2,752** | 605 |
| `emp40` + faithful all-int | 2,293 | 487 | **2,752** | - |
| faithful `int x,y; float z` | 2,252 | 436 | **2,768** | 1,282 |
| `int xi,yi` pre + copy (`int_pre_copy`) | 1,474 | 436 | 2,764 | - |
| all three hack coords before guard | 1,992 | 397 | **2,760** | - |
| `emp40` + all three before guard | 2,002 | 393 | **2,760** | - |
| `emp40` + `y` before guard | 2,002 | 393 | **2,760** | - |
| 60-cell spawn pairing sweep (`z` pair x `x` pair) | min 1,164 | 400/465 | 2,764 | - |

The all-int spawn is size 2,752 because clang **sinks** the three int
adds below `setEmpData` and drops one set of `vcvt` (the original
duplicates the conversions in both ctor branches).  Moving the
declarations before the guard does not stop the sink.

### 2.4 EH / guard / alias forms

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `try { setEmpData } catch(...) { trap }` | 1,388 | 439 | **2,768** |
| `try` around `getCurrentCampaignMission` | 1,353 | 438 | **2,768** |
| both `try` | 1,403 | 464 | **2,772** |
| `try` around `setShipGroup` | 1,388 | 439 | **2,768** |
| `Status *status = Globals::status` alias | 2,319 | 505 | **2,768** |
| drop `setEmpData` guard | 2,182 | 482 | **2,780** |
| drop all guards | 2,182 | 383 | **2,776** |

`EmpGuard` (empty dtor) is required for size and raw; `try/catch` is
not equivalent.

### 2.5 Neutral (1,164 / 400, byte-identical instruction stream)

`KIPlayer *ship` used for `setShipGroup`/`addObject`/`setLevel` with
casts (`single_ship_var`), dropping the `KIPlayer *kiFighter` temp,
`BoundingVolume *bv` temp reuse in the race-0 non-14 block,
`float fx = x` vs `(float)x`, `fx/fy/fz` copies, `ship = nullptr`
placement, `Level *self = this` alias, `empTime` default-init.

Worse: `no_g_temp` (`addObject(fixed->geometry)` directly) 1,166;
`bv_temp` + ASL first 1,172; `PlayerFixedObject *ship` typed phi
2,337 / size 2,788.

### 2.6 `__builtin_expect` on the clamp / branches (all +size)

| variant | raw | size |
|---|---:|---:|
| `expect(getLevel() > 20, 0)` | 1,580 | 2,772 |
| `expect(getLevel() > 20, 1)` | 2,174 | 2,772 |
| `expect(shipClass == 1, 1)` | 2,144 | 2,768 |
| `expect(shipClass == 1, 0)` | 2,271 | 2,768 |
| `expect(mission == 0x9a, 0)` | 2,223 | 2,772 |
| `expect(waypoint != nullptr, 1)` | 2,390 | 2,772 |
| `expect(mission in 49..56, 0)` | 1,362 | 2,764 |

None moves the spill without growing the body.

## 3. Why the spill does not move

Final MIR (`scratch/w13683/mir40.txt`, `-mllvm -print-machineinstrs`)
shows the split is inserted at the end of `bb.17` (`%ir-block.70`, the
`cmp #20` block) immediately before `tBcc %bb.19`:

```
bb.17:  ... tCMPi8 killed $r0, 20 ...
        t2STRi12 killed renamable $r9, $sp, 56   ; this spill
        tBcc %bb.19, 13
```

The original's store is at the end of the merge block `bb.20` (after
`add.w sl,r0,#40`, before `str.w fp`).  `this` has no use between entry
and the first reload, so the split point is a pure
`RAGreedy::SpillPlacer` compact-region choice.  Every source lever that
adds a use of `this` at the merge (`int mp = missionPtr`,
`LODManager *lm = lodManager`, `Level *self = this`) either adds 8 bytes
or rewrites the whole allocation (2,257 / 2,514).  Moving `int armor`
into the region moves the spill to the prologue (2,245) or the
`mission==0x9a` block (1,551), never to `bb.20`.

## 4. Conclusion / next lever

The clamp is solved: write

```cpp
int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel());
empPoints = 40 + empPoints + empPoints * 4;
```

It is byte-identical to the original's `c0314..c033a` except the `this`
spill.  It is not a raw win today because the spill sits 0x14 bytes
early, but it is the correct source shape and a prerequisite for any
future `bb.20` spill fix (which would be worth ~-26 raw).  A future
pass should target the Greedy RA split point, e.g. by finding a
zero-cost use of `this` in the merge block, rather than the clamp.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13683
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb      # raw=1164 aligned=400 size=2764/2764
python3 scratch/w13683/secdiff.py            # .text 71247, extab 577, exidx 0
```

Scratch helpers (all in `scratch/w13683`): `measure.sh` (build + fn_diff
+ secdiff), `secdiff.py`, `vtry.py` (JSON variant sweep),
`spawnsearch.py` (spawn pairing sweep), `blockdiff.py` (per-64B raw
histogram), `cc.sh` (single-TU compile with arbitrary `-mllvm`),
`t4.cpp` (isolated clamp codegen probe), `mir40.txt` (Greedy-RA MIR).

## 6. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
