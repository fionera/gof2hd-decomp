# `Level::createShip` — wave 191b (w13663): fresh spawn/hull/allocator angles, no landable win

Worktree `wt/w13663`, branch `wave191b-w13663`, base master `a3f6a5b88`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13663`.

**Status: no landable win.  Tree left at the pristine baseline.**
Raw **1,164**, aligned **400**, insns **1,117/1,117**, size **2,764/2,764** exact,
`.text` content diff **71,210**, `.ARM.extab` content diff **577** (size `0x26298`),
`.ARM.exidx` diff **0**, `.rel.plt` byte-identical.
`check` -> **RATCHET PASS** (byte_exact 4508/4524, linked_exact 4508/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370).

## 0. Baseline re-verified

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1164 aligned=400 size=2764/2764 insn=1117/1117
python3 scratch/w13663/secdiff.py cmake-build-match/libgof2hdaa.so
# .note.gnu.build-id 20  .text 71210  .ARM.extab 577
```

Note: the current master baseline is **newer** than the one the w13585/w13596
notes measured.  Its first 98 instructions (everything through the `hitpoints`
switch and the `mission == 49..52,56` range test) are byte-identical to the
original, the `missionPtr` test already has the original's
`cmp #1 / it ne / cmpne #23` order (so w13596's `mp23first` lever is already
landed/obsolete), and the whole race-0 type-14 hull block
(`0xc0770..0xc0a76`, 325 instructions) is **instruction-sequence identical**
under register normalisation — only register *names* differ.

## 1. Where the 1,164 raw bytes are (current baseline)

`scratch/w13663/bdiff.py` (16-byte-block histogram of the function body):

| offset | bytes | block |
|---|---:|---|
| `0x0100..0x0130` | 16 | `empPoints` clamp + `this` spill placement |
| `0x01b0..0x0250` | 127 | spawn coordinates + `Player` ctor + `new` + `Array` ctor |
| `0x0260..0x0560` | 696 | race-0 type-14 hull block: **register rotation only** |
| `0x0620..0x0826` | 14 | literal-pool / GOT `ldr` addends (1–2 B each) |
| `0x0850..0x09d0` | 307 | race-1 hull block: 6-byte shift + register rotation |
| `0x0ac0` | 1 | literal-pool addend |

The 696-byte hull residual is *not* a source-shape residual any more: the
instruction sequence matches; `fixed`/`volumes`/`x`/`y`/`z`/`0` simply sit in
different registers (`fp`/`r9`/`r6`/`r8`/`sl`/`r0` vs the original's
`r9`/`fp`/`sl`/`r8`/`r4`/`r6`).

## 2. New evidence: the register rotation is driven by the spawn block

The original keeps **all three spawn coordinates as `int`s live across
`player->setEmpData`** and converts them at each ctor call:

```
c03c8: ldr r2,[sp,#48]   ; wpy      c03d2: ldr r3,[sp,#40]  ; wpz
c03ca: ldr r3,[sp,#32]   ; jitter1  c03d4: ldr r4,[sp,#28]  ; jitter2
c03cc: ldr r0,[sp,#44]   ; wpx      c03d6: add r0,r1
c03ce: ldr r1,[sp,#36]   ; jitter3  c03d8: movw r1,#20000
c03d0: add r2,r3                    c03da: add r3,r4
c03dc: sub.w fp,r0,r1   ; x = (wpx+jitter3)-20000
c03e0: subs r4,r2,r1    ; y = (wpy+jitter1)-20000
c03e2: sub.w r8,r3,r1   ; z = (wpz+jitter2)-20000
c03e4: mov r0,r5 / mov r1,sl / mov r2,r9 / blx setEmpData
```

The three `vcvt.f32.s32` conversions happen **after** the `new` in each branch
(`c040c..c0422` fixed, `c06c0..c06d8` fighter), and the ints stay in
`fp`/`r4`/`r8` across the whole hull block.  That is exactly why the original's
hull block has `fixed=r9, volumes=fp, x=sl, y=r8, z=r4` while ours has
`fixed=fp, volumes=r9, x=r6, y=r8, z=sl`: our `z` is a `float` (s16) computed
before the call and our `x`/`y` are computed after it, so `fp` is free and the
allocator hands it to `fixed`.

The original's arithmetic is `(wpx + jitter3) - 20000` (**add then sub**), not
the current source's `wpx + jitter3 - 20000` (which clang canonicalises to
`(wpx - 20000) + jitter3`).

## 3. Why the faithful spawn block cannot be landed

Writing `int x = wpx+jitter3-20000; int y = wpy+jitter1-20000;
int z = wpz+jitter2-20000;` before `setEmpData` reproduces the original's
machine-MIR arithmetic (`t2ADDrr` then `t2SUBrr`, order z,y,x) but **not its
position**: the pre-RA machine scheduler moves all three `add/sub` pairs past
the call.

* The `EmpGuard` around `setEmpData` makes the call an `invoke`, splitting the
  basic block; MachineSink then sinks the ints into the post-call block.
* Even **without** the guard (plain `call`, coords and the `shipClass` test in
  one block) the pre-RA scheduler still schedules the ints after the call
  (`-mllvm -disable-machine-sink` does not change it).
* `-mllvm -pre-RA-sched=source` *does* keep them before the call (matching the
  original's block), but the function then grows to **2,808 B** (register
  pressure), so it is not size-exact and the flag is not ours to change.

Result: every faithful all-int / array / struct form is **size 2,752** (-12),
`.ARM.extab` 593, `.ARM.exidx` 1,625 — not landable.

## 4. The 16-byte clamp block: `this` spill hoist

Greedy-RA MIR (`-mllvm -print-after-all`; saved as `scratch/w13663/greedy_createShip.mir`) shows
the `this` spill is emitted **in bb.16**, immediately after
`t2CMPri %205, 20`:

```
bb.16: ... t2CMPri %205:gprnopc, 20 ...
       t2STRi12 %693:gpr, %stack.0        ; str.w r9,[sp,#56]   <- hoisted
       t2Bcc %bb.18, 13, killed $cpsr
bb.19: %686 = t2ADDri %684, 40
       t2CMPri %714, 1
       t2STRi12 %696:gprnopc, %stack.1    ; str.w fp,[sp,#52]
       t2Bcc %bb.21, 0
```

The original has **both** spills in the merge block (`c0332`, `c0336`, after
`add.w sl,r0,#40` and `cmp r6,#1`).  `%693` is `this` (only use is that store);
`%696` is `type`.  The split point is a Greedy-RA choice, not a scheduler
artifact (the "Machine Instruction Scheduler" dump is byte-identical to the
"Simple Register Coalescing" dump for this function).

`formB` (`(getLevel() > 20 ? 20 : getLevel()) * 5 + 40`) reproduces the
original's clamp *shape* exactly (`ble` to the reload, `movs #20` fall-through,
`add.w r0,r0,lsl#2`, `cmp r6,#1`, `add.w sl,r0,#40`) but the spill is still
hoisted, so the block costs 26 diff bytes instead of 16.

## 5. Measured this session (all size-exact 2,764 unless noted)

| variant | raw | aligned | extab | notes |
|---|---:|---:|---:|---|
| baseline | **1,164** | **400** | 577 | |
| `shipnull` (ship=nullptr right after `setEmpData`) | 1,164 | 400 | 577 | neutral |
| `xyorder` (swap `y`/`x` declarations) | 1,164 | 400 | 577 | neutral |
| `formA` `(lv<=20?lv:20)*5+40` | 1,167 | 398 | 577 | |
| `bvfirst` (`setBV` before `getShipGroup`) | 1,170 | 396 | 577 | faithful tail order, +6 |
| `aslfirst` (`ArraySetLength(3)` first) | 1,172 | 391 | **576** | faithful, +8 |
| `formB` `(lv>20?20:lv)*5+40` | 1,174 | 396 | 577 | clamp shape matches, spill still hoisted |
| `gswap` (drop `setShipGroup` guard, guard `getShipGroup`) | 1,165 | 400 | **576** | text+extab tie (1741 vs 1741) |
| `y_j1j2` (`y = jitter1+jitter2-20000`) | 1,165 | 465 | 577 | |
| `x_wz_j1` (`x = wpz+jitter1`, `y = jitter2+jitter3`) | 1,165 | 400 | 577 | |
| `levelafter` (`level` folded into `hitpoints`) | 1,245 | 427 | 577 | |
| `zafter` (`z` computed after the guard) | 1,346 | 435 | 577 | |
| `xybefore` (`x`,`y` before the guard) | 1,992 | 397 | 592 | **size 2,760** |
| `ifclamp` (`if (lv>20) lv=20;`) | 2,185 | 471 | 589 | **size 2,756** |
| `ifclamp2` (`lv=getLevel()*5; if(lv>100)lv=100;`) | 2,183 | 477 | 589 | **size 2,756** |
| `pfoship` (`PlayerFixedObject *ship`) | 2,337 | 926 | 1,296 | **size 2,788** |
| `shipuninit` (`KIPlayer *ship;`) | 2,404 | 491 | 576 | **size 2,760** |
| `forclamp` | 2,273 | 474 | 1,286 | **size 2,776** |
| `z = wpx+wpz` / `wpy+wpz` / `wpz+wpy` / `wpz+wpx` | 2,372/2,373/2,373/2,372 | 525/524/524/525 | 589 | **size 2,756** |
| `xy_swap` (`x=wpx+j3`, `y=wpy+j1+j2`) | 2,368 | 509 | 589 | **size 2,760** |
| `v_arr` (`int xyz[3]` faithful coords) | 2,112 | 521 | 593 | **size 2,752**, exidx 1,625 |
| `v_st` (`struct {int x,y,z;} pos`) | 2,112 | 521 | 593 | **size 2,752**, exidx 1,625 |
| faithful all-int + guard | 2,297 | 489 | 605 | **size 2,752** |
| faithful all-int, no `setEmpData` guard | 2,194 | 527 | 156,308 | **size 2,768** |
| fully faithful (regular `new` both, no guards, ASL first, setBV first, all-int coords) | 2,448 | 484 | 156,320 | **size 2,792** |

Every candidate was measured with
`python3 scratch/w13663/try.py <variant>.cpp`, which splices the body into
`src/game/world/Level.cpp`, relinks, and prints
`raw/aligned/size/.text/.ARM.extab`; `.rel.plt` stayed byte-identical
throughout.

## 6. Conclusion

The current baseline is a strong local optimum.  The two remaining
source-addressable residuals are:

1. the `this`-spill split point (Greedy RA puts it in the clamp entry block,
   the original in the merge block) — 16 diff bytes;
2. the spawn-coordinate form (float `z` before the call, `x`/`y` after) —
   127 diff bytes, and the *root cause* of the 696+307-byte hull register
   rotation.

Both are locked in by the size-2,764 constraint: every faithful form changes
the allocator and lands at 2,752/2,756/2,760/2,768/2,792.  The one
extab-improving admissible variant (`gswap`, extab 576) is +1 raw, i.e. a
text+extab tie, so it was not landed.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13663
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb      # raw=1164 aligned=400 size=2764/2764
python3 scratch/w13663/secdiff.py cmake-build-match/libgof2hdaa.so
python3 scratch/w13663/bdiff.py              # 16-byte-block raw histogram
python3 scratch/w13663/idiff.py              # register-normalised insn diff
python3 scratch/w13663/try.py scratch/w13663/v_<name>.cpp   # build+measure one variant
```

Scratch helpers (all in `scratch/w13663`): `try.py` (splice+build+measure),
`secdiff.py`, `bdiff.py`, `idiff.py`, `bytediff.py`, `regs.py`, `cc.sh`
(single-TU compile with arbitrary `-mllvm` flags), `ir.sh` (LLVM IR),
`dumpfn.sh`, `greedy_createShip.mir` (Greedy-RA MIR of the baseline),
`misched.mir` (post-scheduler MIR of the no-guard faithful variant), and
`coalesce.mir`/`isel.mir`/`sink.mir`/`peep.mir`/`mcse.mir`/`licm.mir`.  The
full `-print-after-all` dumps (`mir_all.txt`, `ng_all.txt`, ~886 MB each) were
deleted to keep scratch small; regenerate with
`(ulimit -v 12000000; scratch/w13663/cc.sh out.o -mllvm -print-after-all > dump.txt 2>&1)`.

## 8. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
