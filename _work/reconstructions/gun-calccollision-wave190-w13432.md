# `Gun::calcCharacterCollision` wave 190 (w13432): the original `cmp r0,r1`
# form is reachable, but only 5 B worse; the rotated k-loop needs the
# emp-first body — no landed win

Worker: w13432. Branch `wave190-w13432`, base master `7e34b44b1`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13432`.

## 0. Result

**No `src/` change landed.**  The committed body (wave-190 w13388/w13406) remains
the best measured shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.
Tree clean apart from this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `7e34b44b1`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

Gate on entry/exit: `check` green (byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `.text` diff 72,885).

## 1. Exact 455-byte decomposition (re-measured byte-by-byte)

`python3 pdiff.py orig.txt ours.txt` (scratch) — the committed residual is
exactly:

| region | bytes | cause |
|---|---:|---|
| `0x153198`..`0x15327b` | **226** | 2-byte `add r4,sp,#104` remat vs orig 4-byte `ldr.w fp,[sp,#40]` reload (two sites) |
| `0x153440`..`0x153507` | **167** | k-loop: top-test + out-of-line `if.then` vs orig rotated loop |
| `0x1533c8`..`0x1533f5` | **44** | view block: ours remats `sp+104` three times, orig keeps it in r5 |
| small | 18 | 7 x `i*3` r4/r5 swap, 3 x scatter stack-slot swap, 8 x branch offsets |

## 2. New evidence: the original `cmp r0,r1` form is reachable

The committed source computes `int diff = magnitude - distance;` and branches on
`expect(diff > 0, 0)`, which clang lowers to `subs r1,r0,r1; cmp r1,#1; bge`.
The original instead emits `cmp r0,r1; ble.n` (compare `magnitude` vs
`distance` directly, subtract only after the branch).

Writing the condition as the comparison and computing `diff` inside the body

```c++
if (__builtin_expect(this->magnitude > distance, 0)) {  // lint: arm_builtin
    int diff = this->magnitude - distance;
    ...
}
```

**does** reproduce the shipped `cmp r0,r1` form (verified in the objdump:
`0x153480 cmp r0,r1; 0x153482 bgt.n`).  But it is a net loss:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **455** | **93** | 1984 |
| `magnitude > distance` + diff inside (emp-first) | 460 | 89 | 1984 |
| same, faithful C-D-E body order | 463 | **57** | 1984 |

The +5 is entirely inside the k-loop: the body starts at `0x153488` instead of
`0x15348a`, and the `cmp` block differs more.  The aligned metric improves
(93 -> 57) but size stays exact, so this is a shape improvement, not a raw win.

## 3. The rotated k-loop only fires with the emp-first body

With `-mllvm -likely-branch-weight=3 -mllvm -unlikely-branch-weight=1` (the
wave-190 w13417 "l3u1" knob) the loop *does* rotate, but only for the committed
emp-first body order:

| variant | raw | aligned | size | loop |
|---|---:|---:|---:|---|
| committed + l3u1 (w13417) | 1043 | 100 | 1980 | rotated |
| `magnitude > distance` + l3u1 (emp-first) | 1048 | 95 | 1980 | rotated, `cmp r0,r1` |
| `magnitude > distance` + faithful + l3u1 | 463 | 57 | 1984 | top-test |
| `magnitude > distance` + faithful, default | 463 | 57 | 1984 | top-test |

A full `(L,U)` sweep for the faithful body (L in
{1,2,3,4,5,6,8,10,14,20,50,200,2000,5000} x U in {1,2,3,4}) gives **463/57/1984
for every pair** — the faithful body never rotates.  So the original's rotated
loop + faithful damage-before-damageEmp order cannot be produced by the branch
weights alone.

### 3.1 Byte-level simulation of the rotated path

`python3 sim2.py` (scratch) inserts the missing 2-byte `add r4,sp,#104` at
`0x153444` into the md+l3u1 bytes: raw drops 1047 -> **526**, still worse than
455.  So even with the missing k-loop remat repaired, the emp-first body layout
costs more than the rotation saves.  The rotated path is a dead end for raw.

## 4. Root cause of the 226-byte phase shift (from the machine IR)

`-mllvm -print-after-all` (scratch `all.log`) shows the shared 60-byte frame
slot `%stack.10` (`fi#10`, `sp+104`) is used by **`playerPosition`, `view` and
the k-loop `getPosition()` sret temp**.  In `After Greedy Register Allocator`
the k-loop pointer `%513 = COPY(t2ADDri %stack.10, 0)` is materialised in
`bb.43` (`%ir-block.225`, the asteroid/`mine` check) even though it is only
used in the later scatter k-loop.  That is the `add r4,sp,#104` at
`0x153198`; it is dead (r4 is redefined at `0x1533ea` before any use) but it
occupies the slot where the shipped body reloads `i` into `fp`
(`ldr.w fp,[sp,#40]`, 4 B), producing the -2/-4 phase shift.

The pre-RA scheduler dump already has no `%stack.10` in `bb.43` — the remat is
inserted by the greedy allocator, so it cannot be moved by a scheduling flag.
It only goes away if `&view`/`&playerPosition`/the k-loop temp stop sharing one
slot, and every frame-neutral form tried (w13406/w13417 and this pass) either
folds to the committed codegen or grows the frame.

## 5. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

All size-exact unless stated; none beats 455.

- k-loop condition forms: `magnitude > distance` (460), `magnitude > distance`
  + faithful (463), inline `(float)(magnitude-distance)/magnitude` (460),
  `diff <= 0` continue (455), `++k` before the if (455), explicit `k = k + 1`
  (455), `while` (460/463), `do-while` (1948-1952), `expect(k<size,1)`
  (1948).
- `Vector kpos` in the k-loop body (455), `Vector kpos` at scatter scope
  (2004), `Vector kpos` via `enemyPosition` (1992).
- scatter: `Vector delta = enemyPosition - playerPosition` (455), `Vector tmp`
  (455), `const Vector &playerPosition` (455), assignment form (2000).
- `Matrix view` via `Matrix *vp` (2056), `Matrix &vp` (2056), `char *vp`
  (2064), `memcpy` (2064).
- view block order 1,3,copy (457), md + 1,3,copy (462).
- `unsigned i3 = i*3` (1964), cached `positions.data()`/`velocities.data()`
  (477), `int i` (455), `int e` (455).
- mine-block `expect` polarity 0 (2000), full-condition expect (1996),
  `(float) radius` instead of `(float) enemy->radius` (1944).

### 5.1 Flag sweeps (per-TU `-mllvm`, all on Gun.cpp)

No flag moved the function at all (455/93/1984 for every value):
`regalloc-csr-first-time-cost=0..100`, `condsider-local-interval-cost`,
`early-live-intervals`, `disable-spill-hoist`, `enable-post-misched`,
`misched-postra`, `misched-bottomup`, `misched-cluster=false`,
`misched-regpressure=false`, `misched-cyclicpath`, `misched-fusion`,
`disable-tail-duplicate`, `disable-tail-merge`, `tail-dup-*`,
`enable-early-ifcvt`, `disable-ifcvt-triangle-reversal`,
`enable-copy-propagation`, `enable-local-reassoc`, `avoid-speculation`,
`hoist-cheap-insts`, `disable-constant-hoisting`, `disable-licm-promotion`,
`force-loop-cold-block`, `loop-to-cold-block-ratio=0..100000`,
`force-precise-rotation-cost`, `block-placement-exit-block-bias=2000..100000`,
`branch-fold-placement`, `precise-rotation-cost`, `machine-sink-bfi`,
`disable-ssc`, `no-stack-slot-sharing`, `enable-deferred-spilling`,
`split-spill-mode=0..2`, `stress-regalloc=13..20`, `disable-postra-machine-licm`,
`disable-machine-licm`, `disable-machine-cse`, `disable-post-ra`,
`disable-cgp*`, `arm-disable-cgp`, `arm-load-store-opt=false`,
`arm-promote-constant=false`, `disable-lsr`, `enable-lsr-term-folding`,
`enable-earlycse-memssa=false`, `disable-gvn`, `enable-gvn-hoist=false`,
`pre-RA-sched=source|linearize|list-burr|list-hybrid`.

## 6. Conclusion

The committed **455 / 93 / 1984** body is a hard plateau.  The two remaining
levers are now both characterised exactly:

1. the 226-byte phase shift needs `&view` to stop sharing `%stack.10` with the
   k-loop sret temp *without* growing the 208-byte frame — no such form found;
2. the rotated k-loop is reachable (md + l3u1) but only with the emp-first body,
   and the byte simulation puts it at 526 even after repairing the missing
   remat.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13432
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 8. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13432.md` only.
