# `Level::createShip` — wave 191b (w13596): spawn-block / hull-block sweep, no landable win

Worktree `wt/w13596`, branch `wave191b-w13596`, base master `ff06d9e59`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13596`.

**Status: no landable win.  Tree left at the pristine baseline.**
Raw **1,164**, aligned **400**, size **2,764/2,764** exact,
`.text` diff **71,214**, `.ARM.extab` diff **577** (size `0x26298` @ `0x1a3b78`),
`.ARM.exidx` diff **0**, `.rel.plt` byte-identical.

## 0. Baseline re-verified

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1164 aligned=400 size=2764/2764 insn=1117/1117
python3 scratch/w13596/secdiff.py cmake-build-match/libgof2hdaa.so
# .text 71214  .ARM.extab 577  .ARM.exidx 0  .rodata 485  .rel.plt 0
```

The pristine source is saved at `scratch/w13596/Level.cpp.base`; every
experiment below is a `str.replace` on that file.

## 1. Where the 1,164 raw bytes are

`scratch/w13596/branges.py` (108 runs).  The two clusters that matter:

* **spawn-coordinate block `0x1b4..0x220` ≈ 98 raw bytes** — the single
  biggest cluster.  Original computes *three integers* between the
  `new Player(...)` ctor and the `setEmpData` call:

  ```
  c03c8: ldr r2,[sp,#48]   ; wpy      c03d2: ldr r3,[sp,#40]  ; wpz
  c03ca: ldr r3,[sp,#32]   ; jitter1  c03d4: ldr r4,[sp,#28]  ; jitter2
  c03cc: ldr r0,[sp,#44]   ; wpx      c03d6: add r0,r1
  c03ce: ldr r1,[sp,#36]   ; jitter3  c03d8: movw r1,#20000
  c03d0: add r2,r3                    c03dc: sub.w fp,r0,r1   ; x = wpx+j3-20000
  c03e0: add r3,r4                    c03e2: subs r4,r2,r1    ; y = wpy+j1-20000
  c03e4: sub.w r8,r3,r1               ; z = wpz+j2-20000
  c03e8: mov r0,r5 / mov r1,sl / mov r2,r9 / blx setEmpData
  ```

  i.e. the *faithful* `int x = wpx+jitter3-20000; int y = wpy+jitter1-20000;
  int z = wpz+jitter2-20000;` with the `(float)` casts at the two ctor call
  sites (the conversion is duplicated in the fighter and the capital branch).
  Our source instead has the (unfaithful) shape

  ```cpp
  float z = (float) (wpx + wpy - 20000);      // before the guard
  { EmpGuard guard; player->setEmpData(...); }
  int x = wpz + jitter3 - 20000;              // after
  float y = (float) (jitter1 + jitter2 + jitter3 - 20000);
  ```

  which only keeps *one* value live across the call.

* **hull/volume setup `0x264..0x55c`** — ~500 raw bytes, all of it the
  register-allocation swap described in w13542: original
  `fighter=sl, fixed=r9, volumes=fp, ship=r9`; ours
  `fighter=fp, fixed=fp, volumes=r9, ship=fp`.  The instruction *sequence*
  is identical; only the register names (and the two-byte penalties for
  high-register `ldr.w`/`str.w` forms) differ.

## 2. All-int spawn forms change the whole allocation (size-exact killer)

Every "three ints before the guard" form (S2/S12/S13 below) makes clang
**sink** the three computations past the `setEmpData` call and, more
importantly, removes the `this` spill: function shrinks to **2,752 B**
(-12), LSDA record grows (`.ARM.extab` diff 593, `.ARM.exidx` 1,625).
Not size-exact, so not landable.  Putting the ints before `new Player` or
before `int armor` gives byte-identical output (clang sinks them anyway).

## 3. Measurements (all size-exact 2,764 unless noted; extab 577 / exidx 0
unless noted; `.rel.plt` identical throughout)

| variant | raw | aligned | notes |
|---|---:|---:|---|
| baseline | **1,164** | **400** | |
| `mp23first` (`expect(mp != 23,1) && mp != 1`) | 1,165 | **385** | only diff vs base is `0x508..0x515` (+1 B); fixes the `cmp #1 / cmpne #23` order |
| `X1_exp2` (`expect` on the 2nd operand) | 1,165 | 385 | same code as `mp23first` |
| `X4_exp2_23first` | 1,164 | 400 | tie |
| `mp23first_noexp` / `mp1first_noexp` | 1,171 | 391 | |
| `S1` faithful exprs, keep `int x`/`float y`/`float z` | 1,173 | 410 | spawn block becomes one 68-byte run |
| `S7` all three before guard, faithful, int/float/float | 1,190 | 447 | |
| `S2`/`S12`/`S13` faithful all-int | 2,112 | 521 | **size 2,752**, extab 593, exidx 1,625 |
| `S15` faithful all-float after guard | 2,518 | 1,012 | size 2,740 |
| `S16` faithful all-float before guard | 2,467 | 989 | size 2,732 |
| `P2` (`z`,`x` before guard, `y` after) | 1,165 | 400 | |
| `P5` (`z` before, `y`,`x` after) | 1,164 | 400 | tie |
| `x_before_z` | 1,165 | 400 | |
| `formB` (`(lv>20?20:lv)*5+40`) | 1,174 | 396 | matches original `ble` polarity, spill still `0x108` |
| `formA` (`(lv<=20?lv:20)*5+40`) | 1,167 | 398 | |
| `g_inline` (`addObject(fixed->geometry)`) | 1,166 | 400 | |
| `bv_last` (`setBV` before `field_0x40`) | 1,167 | 399 | |
| `mp23first + bv_last` | 1,168 | 384 | |
| `mp23first + g_inline` | 1,167 | 385 | |
| `mp23first + P2` | 1,166 | 385 | |
| `mp23first + formA` | 1,168 | 383 | best aligned seen, raw +4 |

### Neutral (byte-identical `.text`, 1,164 / 400)

`armor_split`, `empTime_init`, `level_split`, `hitpoints_split`,
`mission_range` (`(>=49 && <=52) || ==56`), `player_split`,
`ship_top_null`, `ship_before_player`, `ship_zero`, `ship_after_ctor`,
`ship_after_alloc`, `ship_is_alloc`, `ship_alloc_cast`, `cargo_2c_33_31`,
`R1_default_end`, `Q1_fold40`, `Q4_40first`, `Q5_mul3_40`, `Y1_no_ki`,
`Y2_ptr`, `Y3_if`, `V1_andand`, `V2_intand`, `V3_bool`, `N1_yoda`,
`N2_sub`, `W1_fx_before_alloc`, `zero_local` (a named `float zero = 0.0f`
used for the 24 `0.0f` BoundingAAB args).

### Worse

`else` (drop `shipClass == 1` test) 2,337 / size 2,788; `ship_top_uninit`
2,404 / size 2,760; `cargo_33_2c_31` 2,031; `R2_case1_first` 1,318;
`Q2_fold_in` 2,231 / size 2,768; `Q3_lvl_var` 2,178 / size 2,760;
`W2_fx_after_ctor` 1,169; `Z2_armor_after_player` 2,151 / size 2,760.

### EmpGuard / LSDA experiments (all worse or not size-exact)

| variant | raw | size | extab | exidx |
|---|---:|---:|---:|---:|
| `G2` guard `getShipGroup` only (drop `setShipGroup` guard) | 1,165 | 2,764 | **576** | 0 |
| `E1` guard around placement-`new PlayerFixedObject` | 1,370 | 2,768 | 106,787 | 4,900 |
| `E4` drop top + guard placement-`new fixed` | 1,176 | 2,764 | 107,903 | 1,325 |
| `H1` guard `new Player` (drop `setEmpData` guard) | 2,508 | 2,784 | 122,593 | 4,756 |
| `H2` guard `new Player`, keep `setEmpData` guard | 2,474 | 2,772 | 108,179 | 4,878 |
| `T1` drop top + guard `new Player` | 2,465 | 2,768 | 122,589 | 4,857 |
| `G5` guard `ship->setLevel` | 1,358 | 2,768 | 106,717 | 4,900 |
| `G4` guard `lodManager->addObject` | 2,358 | 2,784 | 106,744 | 4,799 |

## 4. The `this` spill: it *can* be moved, but not for free

MIR (`scratch/w13596/mir_all.txt`) shows the `str.w r9,[sp,#56]` store is
inserted by **Greedy RA** as `t2STRi12 %696, %stack.1` at the *end of bb.16*
(between `cmp #20` and the `bcc`), i.e. it is a live-range-split choice, not
a MachineSink choice.  Original puts it at `0xc0332` (after `add sl`), ours at
`0xc031c` (before the branch) — a 4-byte local shift worth ~16 raw bytes
(`0x108..0x122`).

The one lever found that moves it: force a *use* of `this` after the clamp.

```cpp
int empPoints = ...;
empPoints += 40;
int mp = missionPtr;                 // new
... if (__builtin_expect(mp != 1, 1) && mp != 23) { ... }
```

gives `r9spill = 0x122` (original `0x11e`) but **size 2,772** (+8) because of
the extra `ldr.w r1,[r9,#192]` / `str r1,[sp,#24]` pair, so raw is 2,272.
`bool addLOD = !(...)` variants are worse (2,384 / 2,610) and also size 2,772.
No size-neutral way to force the use was found.

## 5. Landing-pad / EH structure (why the `.ARM.exidx` is exact but `.text` is not)

Both forms have 57 LSDA rows / cst 315 / record 336, so `.ARM.exidx` is
byte-identical.  The row *contents* differ:

```
orig: (0,406,0) (406,30,2488) (436,94,0) (530,32,2482) (562,8,0) (570,4,2476) ...
ours: (28,4,2496) (32,374,0) (406,30,2486) (456,10,2484) (466,102,0) (568,4,2476) ...
```

Ours spends two 5-byte rows on `getCurrentCampaignMission` and
`setEmpData`; the original spends them on the `new PlayerFixedObject`
cleanup and the `new Array` cleanup.  Consequence: our main body ends at
`0xc0b90` (2,428 B) vs the original `0xc0b8a` (2,422 B) — **+6** — and our
landing-pad tail is 76 B vs 82 B (**-6**), because the original has four
delete pads (`sl`, `fp`, `r9`, `r5`) and ours only two (`r9`, `r5`).
The 32 `operand:b.n` + 50 `operand:ldr` raw diffs are all downstream of this
6-byte body/pad split and the register swap.  Every attempt to add a pad
(guards, regular `new`) changed the size or the record.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13596
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb      # raw=1164 aligned=400 size=2764/2764
python3 scratch/w13596/secdiff.py cmake-build-match/libgof2hdaa.so
```

Scratch helpers (all in `scratch/w13596`): `runvar.py` (build+measure one
variant, prints raw/aligned/size/.text/.ARM.extab/.ARM.exidx and the
`str.w r9,[sp,#56]` offset), `probe.py <json>`, `branges.py`,
`bytediff.py <so> <vma> <from> <to>`, `lendiff.py`, `cats.py`,
`insdiff.py`, `lsda.py`, `secdiff.py`, `mir_all.txt`, `orig.dis.txt`.

## 7. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
