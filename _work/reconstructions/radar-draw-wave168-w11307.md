# Radar::draw wave 168 (w11307) — campaign/sl, lock-landmark expect, loop block order

**Status:** analysis pass, no source change landed (2026-11-11). Worktree
`wt/w11307`, branch `wave168-w11307`, base `master` `631c26f0b`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-campaign-sl-wave167-w11206.md`,
`radar-draw-wave167-w11207.md`, `radar-draw-campaign-sl-wave166-w11106.md`
and `build-provenance-round7-branch-weights.md` (w11220).

## 0. Baseline (verified on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,487 |
| instruction-index alignment | 1,454 |
| instructions | 3,536 ours / 3,532 orig |
| `.rel.plt` | byte-identical |

## 1. Lock-landmark expect is a regression on this base

`if (__builtin_expect(!this->lockConfirmed, 0))` in the landmark
station-lock block (the `stationLockTimer > field_0x1b8` arm, line ~500)
measures **7,533 raw / 1,498 aligned / 9,916 B** (+46 raw, +8 B).  The
wave-167 task text's "7,522 raw" was an older base; the current base already
has the landmark block's `ldrb [r4,#536]; cbnz` shape matching the original
(`12e3be`-`12e3f4`), so the expect only re-outlines it and adds an extra
EH landing pad.  Not landable.

## 2. The residual is the planetTargets loop block order (confirmed)

The only control-flow difference in the whole function is the
`if (!otherLock && !planetHit && !turretMode)` test at the end of the
`planetTargets` loop (source line 593).  Original (`12e4dc`-`12e4f2`):

```
orrs r0,r1 ; orrs r0,r1 ; lsls r0,r0,#31
beq.n 12e4f2        ; condition true -> body (taken)
add.w fp,fp,#1      ; loop latch = fallthrough
b.n   12e3d4
12e4f2: body
```

Ours (`12e4b0`-`12e4c0`): `bne.w <latch>` with the body as fallthrough.  The
original's profile weight makes the body unlikely and the latch the
fallthrough; MBP's default heuristic picks the body.  This is the 7
direction mismatches w11220 measured.

## 3. Per-TU branch-weight override fixes the loop, but costs +4 B

Adding a per-TU `-mllvm -likely-branch-weight=3500` to Radar.cpp (the
existing `-disable-postra-machine-licm` stays) restores the original loop
block order exactly (`beq.n` to body, latch fallthrough) and drops raw
7,487 -> **7,395** (-92), aligned 1,454 -> 1,586.  **But size becomes
9,912 (+4).**  The +4 is a register-allocation side effect, not the loop:

* frame grows 296 -> 304 (`sub sp,#304`), all slots shift +8;
* the allocator spills `this` (`str r4,[sp,#152]` at `12e38e`, reload
  `ldr r4,[sp,#152]` at `12f536`) instead of the base's
  `mov r6,r4` / `mov r4,r6` save/restore (4 B vs 8 B);
* the cause is the gas-spark temp pointer: base keeps the
  `getPosition()` result address in `r9` (`add.w r9,sp,#220`), the
  3500 build keeps it in `r6` (`add r6,sp,#228`), so `r6` is unavailable
  for the `this` save.

Weight sweep (`likely:unlikely`, full gold link each):

| weight | raw | aligned | size |
|---|---:|---:|---:|
| base (no override) | **7,487** | **1,454** | **9,908** |
| 7:1 | 9,418 | 1,476 | 10,028 |
| 80:1 | 8,835 | 1,493 | 9,920 |
| 950:1 | 7,491 | 1,522 | 9,908 |
| 1000:1 | 7,491 | 1,522 | 9,908 |
| 1200:1 | 7,487 | 1,454 | 9,908 |
| 2000:1 (default) | 7,487 | 1,454 | 9,908 |
| 3000:1 | 8,571 | 2,725 | 9,920 |
| 3370:1 | 7,395 | 1,586 | 9,912 |
| 3500:1 | 7,395 | 1,586 | 9,912 |
| 5000:1 | 7,400 | 2,727 | 9,912 |

`likely=3500` alone (default unlikely) is identical to 3500:1.  The
950-1190 band is size-exact with the loop fixed but raw 7,491 (+4 vs base),
so the loop fix alone does not beat the base.

Source companions tried on top of 3500:1, all still 9,912:
remove campaign-guard expect (7,416), remove `!=120` expect (**7,374**),
remove docking expect (7,395), remove all three (7,401), landmark expect
(7,431), split/rename the spark `getPosition()` temp, swap `mi` operands,
`-regalloc-csr-first-time-cost=0/10/100`, `-disable-spill-hoist`.  None
removes the `this` spill.

## 4. Campaign / `sl` allocation (re-confirmed, no lever found)

Original keeps `campaign` in `sl` (`12e0f8: mov sl,r0`, used at
`12e38a: cmp.w sl,#2`), the shared zero constant in `fp`, and `stations`
in `sl` then `sp+116`.  Ours spills `campaign` to `sp+144` and keeps the
zero in `sl`; the campaign block is otherwise instruction-identical.  The
loop block-order fix does **not** move this allocation (3500 still spills
`campaign`), so the two residuals are independent.  All the wave-166/167
decl-order/type sweeps were re-checked on this base; none moves it.

## 5. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# per-TU weight A/B: add to the Radar.cpp COMPILE_OPTIONS in CMakeLists.txt
#   -mllvm;-likely-branch-weight=3500
cmake --build cmake-build-match --target link_so -j2
# scratch: /dev/shm/w11307/{sweep.py,sweep_bw.py,sweep_combo.py,sweep_flags.py}
```

No source change landed: the only raw win (3500:1, -92 raw) is +4 B and
therefore gate-fatal; every size-exact variant is >= 7,487.  Tree left clean
at `631c26f0b`.
