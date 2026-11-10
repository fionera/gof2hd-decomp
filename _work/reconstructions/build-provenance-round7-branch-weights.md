# Build provenance round 7 (w11220): branch-layout dump for LevelScript and
# Radar; the residual is profile weights, not a source-order knob

Status: **research note only**, 2026-11-10. Worktree `wt/w11220`, branch
`wave167-w11220`, base master `d00057e6d` (wave 166). Scratch
`/dev/shm/w11220`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (link_so + `fn_diff` on both targets): `Radar::draw`
**7,487 raw / 1,454 aligned / 9,908 B**, `LevelScript::process`
**54,095 raw / 17,621 aligned / 57,232 B**. No `src/`, `CMakeLists.txt`,
baseline or allowlist change is landed.

Targets:

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 7,487 | 1,454 | 9,908 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,095 | 17,621 | 57,232 |

## 0. Bottom line

1. **Branch weights are observable as block layout.**  For every conditional
   branch the compiler puts the higher-probability successor in the
   fallthrough.  Comparing, branch by branch, whether the taken target lies
   forward (`F`) or backward (`B`) relative to the branch isolates
   block-placement differences from register-allocation/stack-slot noise.
   The new tool `_work/reconstructions/branch_layout_w11220.py` does exactly
   that on two `objdump` dumps; it is committed with this note.
2. **Radar::draw is already layout-correct except for one loop.**  360
   conditional branches, **7 direction mismatches**, all inside the
   `planetTargets` loop (0x12e4ec..0x12e6c2 band).  The original places the
   loop latch (`++i; goto header`) *before* the `if (!otherLock && !planetHit
   && !turretMode)` body, so the latch is the fallthrough of that test; ours
   places the body in the fallthrough and the latch after it.  This is the
   only control-flow/block-order difference in the whole 9.9 KB function.
3. **`__builtin_expect(cond, 0)` reproduces Radar's layout exactly** (0
   direction mismatches) but materialises the `&&` chain into a `bool`
   temporary: frame 296 -> 304 B, `.text` 9,908 -> 9,956 B, raw 7,487 ->
   9,263.  Every placement of the expect on a sub-term was measured
   (14 variants); none is size-exact.  So the original's weight is a real
   profile value, not a two-level `__builtin_expect`.
4. **LevelScript::process has 17 direction mismatches** (736 branches), all
   in the mission-81 / mission-tail region, plus two switch-body
   permutations.  The 12-case `switch (m_nState)` body order is
   `[1,12,5,3,10,2,4,8,6,7,9,11]` in the original vs
   `[4,9,5,8,2,10,11,1,7,6,3,12]` in ours.  The 30-case mission switch is
   likewise a permutation (harness output below).
5. **Source case-label order does not control jump-table body order.**  A
   synthetic 12-case switch written in the original's physical order
   `[1,12,5,3,10,2,4,8,6,7,9,11]` still compiles to ascending physical order
   `[1,2,...,12]`; `switch (__builtin_expect(x,12))` gives
   `[12,1,2,...,11]`.  A full permutation therefore needs per-case profile
   weights -- exactly the round-6 conclusion, now measured for a non-switch
   loop as well.

## 1. Method: branch-layout dump

`tools/fn_diff.py --json` already reports operand-level categories but not
*which* successor the compiler chose.  The new tool compares the two
instruction streams after normalising branch targets to instruction indices
and registers/immediates to placeholders, then reports every conditional
branch whose taken target is forward in one build and backward in the other:

```sh
# dumps (ADDR INSN lines, function body only)
python3 /dev/shm/w11220/dump.py _ZN5Radar4drawEP6PlayerP3Hudi orig  > radar_orig.txt
python3 /dev/shm/w11220/dump.py _ZN5Radar4drawEP6PlayerP3Hudi ours  > radar_ours.txt
python3 _work/reconstructions/branch_layout_w11220.py radar_orig.txt radar_ours.txt
# orig cond branches: 360  ours: 359
# direction mismatches: 7
```

`dump.py` is the 20-line objdump wrapper used for the dumps (reads the
`.text` VMA/offset from `objdump -h`, prints `%08x %s` per instruction); it
is reproduced at the end of this note so the analysis is self-contained.

The metric deliberately ignores register names and stack offsets: it answers
"is the *block order* the same?"  A mismatch is a block-placement
(weight) difference; a match with high raw diff is register allocation /
frame layout.

## 2. Radar::draw

### 2.1 The seven mismatches

```
orig idx445 @0012e53e ble -> I417 (-28) | ours F I417 (+73) @0012e50c
orig idx448 @0012e544 bge -> I417 (-31) | ours F I417 (+70) @0012e512
orig idx454 @0012e554 ble -> I417 (-37) | ours F I417 (+64) @0012e522
orig idx457 @0012e55a bge -> I417 (-40) | ours F I417 (+61) @0012e528
orig idx466 @0012e56e beq -> I522 (+56) | ours B I522 (-53) @0012e53c
orig idx494 @0012e5b6 beq -> I417 (-77) | ours F I417 (+26) @0012e580
orig idx593 @0012e6c2 bne -> I392 (-201) | ours F I392 (+11) @0012e6a2
```

All seven point at the same two blocks: `I417` is the loop latch
(`add.w fp,fp,#1; b.n header`) and `I392`/`I522` are the on-screen test and
the `if (!otherLock && !planetHit && !turretMode)` test.  In the original
both tests branch *over* the latch to the body and fall through into the
latch; in ours the body is the fallthrough and the latch is after it.

### 2.2 The loop shape (original, 0x12e4dc..0x12e4f2)

```
12e4dc  ldr r5,[sp,#156] ; ldr r0,[sp,#144] ; ldr r1,[sp,#152] ; orrs r0,r1
12e4e4  ldr r1,[sp,#148] ; orrs r0,r1 ; lsls r0,r0,#31
12e4ea  beq.n 12e4f2          ; condition true -> body (taken)
12e4ec  add.w fp,fp,#1        ; latch (fallthrough)
12e4f0  b.n   12e3d4          ; back to header
12e4f2  ... body ...
```

The `orrs`/`lsls`/`beq` is the `if (!otherLock && !planetHit &&
!turretMode)` test.  The original's fallthrough is the latch, i.e. the
profile says the condition is **unlikely**.  50/50 (no profile) makes MBP
pick the first successor (the body) -- ours.

### 2.3 Source-construct sweep

Every placement of `__builtin_expect` on the three terms was measured
(`-j2`, full gold link; each row is a fresh build):

| source shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | **7,487** | **1,454** | **9,908** |
| `expect(!otherLock && !planetHit && !turretMode, 0)` | 9,263 | 1,779 | 9,956 |
| `expect(!otherLock,0) && ...` | 9,198 | 1,668 | 9,948 |
| `... && expect(!planetHit,0) && ...` | 9,198 | 1,668 | 9,948 |
| `... && expect(!turretMode,0)` | 8,967 | 2,793 | 9,936 |
| `expect(!otherLock,1) && ...` | 9,230 | 1,521 | 9,940 |
| `... && expect(!planetHit,1) && ...` | 8,203 | 1,519 | 9,904 |
| `... && expect(!turretMode,1)` | 8,198 | 1,475 | 9,904 |
| `expect(!otherLock,1) && expect(!planetHit,1) && ...` | 9,119 | 1,500 | 9,900 |
| all three `expect(...,1)` | 8,812 | 1,497 | 9,960 |

The `expect(...,0)` rows give **0 direction mismatches** -- the block order
then matches the original exactly -- but the `&&` chain is materialised as a
`bool` value (a stack temporary), so `sub sp,#296` becomes `sub sp,#304`
and the 8-byte frame growth costs 48 B of `.text` (size 9,956) and raises
raw.  `expect(...,1)` on the last two terms is the closest (9,904, 4 B
short) but still not size-exact.  No size-exact row beats 7,487 raw.

The residual is therefore not a source-shape problem: the original's edge
weight for this test is a profile count (condition taken rarely), and the
only clang-7 source producer (`__builtin_expect`) is two-level and costs a
temporary.  What is left of Radar's 7,487 raw bytes after the layout is
fixed is register allocation (original keeps the loop counter in `fp` and
`stations` in `sl`; ours keeps the counter in `sl` and spills `stations`)
and the resulting frame-slot numbering -- a separate, non-branch residual.

## 3. LevelScript::process

### 3.1 Direction mismatches (17 of 736)

```
orig idx593  @00138380 bhi -> I7021 (+6428) | ours B I7021 (-338)  @00138342
orig idx5568 @0013b85a blt -> I7021 (+1453) | ours B I7021 (-5101) @0013b620
orig idx5605 @0013b8ce blt -> I7021 (+1416) | ours B I7021 (-5136) @0013b68e
orig idx5754 @0013baa2 blt -> I7021 (+1267) | ours B I7021 (-5227) @0013b7b6
orig idx5766 @0013bac8 blt -> I7021 (+1255) | ours B I7021 (-5253) @0013b804
orig idx5922 @0013bcc4 blt -> I7021 (+1099) | ours B I7021 (-5433) @0013ba52
orig idx5985 @0013bd84 blt -> I7021 (+1036) | ours B I7021 (-5494) @0013bb0a
orig idx8024 @0013d378 blt -> I219  (-7805) | ours F I219  (+205)  @0013d3b6
orig idx8073 @0013d402 beq -> I219  (-7854) | ours F I219  (+156)  @0013d43e
orig idx8115 @0013d480 blt -> I219  (-7896) | ours F I219  (+112)  @0013d4c6
orig idx8186 @0013d554 blt -> I219  (-7967) | ours F I219  (+58)   @0013d56c
orig idx8425 @0013d7e0 blt -> I20418(+11993)| ours B I20418(-7998) @0013d60c
orig idx12420@00140230 bge -> I15018(+2598) | ours B I15018(-1851) @0013fbd6
orig idx16173@00142a6c bvs -> I16097(-76)   | ours F I16097(+82)   @00142a98
orig idx16189@00142a8c bvc -> I16112(-77)   | ours F I16112(+78)   @00142ab8
orig idx16278@00142b9c bne -> I16193(-85)   | ours F I16193(+2805) @00142bca
orig idx16619@00142f12 bcs -> I16629(+10)   | ours B I16629(-11)   @00143060
```

`I7021` is the shared mission-switch default/exit block; the original puts
it far forward, ours puts it just behind the first `tbh`.  `I219` is the
mission-0 state-machine exit.  These are the same class of weight-driven
placement as Radar, but spread over a 57 KB function.

### 3.2 The 12-case `switch (m_nState)` (mission-81 chain, 0x138378)

Original `subs r0,#1; cmp r0,#11; tbh [pc,r0,lsl#1]` at 0x138380, table
base 0x138388.  Decoding the halfwords gives the physical body order

```
orig: 1, 12, 5, 3, 10, 2, 4, 8, 6, 7, 9, 11
ours: 4, 9, 5, 8, 2, 10, 11, 1, 7, 6, 3, 12
```

(ours' `tbh` is at 0x138346, table base 0x13834a).  Neither is ascending,
neither is a rotation of the other: both are weight orders.

### 3.3 The 30-case mission switch

`prof_reconstruct_w10820.py extract` still works on this base and prints the
documented order:

```
[131, 64, 135, 145, 142, 144, 139, 69, 73, 67, 70, 29, 41, 80, 56,
 125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16]
```

The compare-chain part of that order is **ascending in both builds**
(`14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,114,125,154,157,158`);
only the jump-table body order differs.  Decoding our two `tbh` tables
(0x138aae / 0x138c62) gives

```
ours tbh bodies: [139, 72, 143, 67, 134, 65, 131, 73, 140, 141, 142, 144,
                  145, 64, 136, 69, 137, 70, 66, 133, 135, 138, 68, 71, 132]
```

vs the original's `[131, 64, 135, 145, 142, 144, 139, 69, 73, 67, 70, 65,
66, 68, 71, 72, 132, 133, 134, 136, 137, 138, 140, 141, 143]`.  Any
source-order hypothesis has to explain both permutations at once.

## 4. Synthetic control: source order does not drive tbh order

A 12-case switch with distinct call bodies, written in the original's
physical order `[1,12,5,3,10,2,4,8,6,7,9,11]`, compiled with the exact
project flags (`-target armv7-none-linux-androideabi16 -march=armv7-a
-mthumb -mfpu=neon -mfloat-abi=softfp -std=c++11 -Oz`), produces a `tbb`
whose physical body order is **ascending**:

```
tbb entries -> body order: 1,2,3,4,5,6,7,8,9,10,11,12
```

and with `switch (__builtin_expect(x, 12))`:

```
body order: 12,1,2,3,4,5,6,7,8,9,10,11
```

So the IR successor order is by case value, and `__builtin_expect` can only
move one case to the front.  It cannot produce `[1,12,5,3,10,2,4,8,6,7,9,11]`.

## 5. Verdict

* Branch layout is the right lens: it separates the two residuals cleanly.
  Radar has **one** layout defect (the planetTargets latch) out of 360
  branches; everything else is regalloc/frame.  LevelScript has 17 layout
  defects plus two switch permutations.
* The original's weights are profile counts.  The only clang-7.0.2
  source-level producer is `__builtin_expect`, which is two-level and, on a
  compound `&&` predicate, costs a `bool` temporary -- so it cannot hit the
  original's multi-level order without changing size.
* No size-exact `src/` win was found; nothing landed.  The next useful
  angle is the *non-branch* Radar residual (loop counter in `fp` vs `sl`,
  `stations` spilled vs kept in `sl`, frame slots 100/104, 84/80, 148/152),
  which is a live-range problem, not a weight problem.  A first probe of
  the obvious declaration-order knobs (`stations`/`planetHit` swapped,
  `planetHit` hoisted out of the `if`, stations computed after the flag)
  is **exactly neutral** (7,487 / 1,454 / 9,908 for all four shapes), so
  clang canonicalises those away; the live-range difference has to come
  from a use-site change, not a declaration move.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11220
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Radar4drawEP6PlayerP3Hudi _ZN11LevelScript7processEi

# branch-layout dump (tool committed alongside this note)
python3 /dev/shm/w11220/dump.py _ZN5Radar4drawEP6PlayerP3Hudi orig > radar_orig.txt
python3 /dev/shm/w11220/dump.py _ZN5Radar4drawEP6PlayerP3Hudi ours > radar_ours.txt
python3 _work/reconstructions/branch_layout_w11220.py radar_orig.txt radar_ours.txt

# mission-switch body order
W10820_SCRATCH=/dev/shm/w11220/run \
  python3 _work/reconstructions/prof_reconstruct_w10820.py extract
```

`dump.py` (scratch helper, 20 lines):

```python
import subprocess, re, sys
ROOT='/home/debian/GalaxyOnFire2/wt/w11220'
OD=ROOT+'/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS=ROOT+'/cmake-build-match/libgof2hdaa.so'
def sym(path,name):
    for l in subprocess.check_output([OD,'-T',path],text=True).splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name: return int(p[0],16),int(p[4],16)
def dis(path,addr,size):
    out=subprocess.check_output([OD,'-d','--start-address=%d'%addr,
        '--stop-address=%d'%(addr+size),path],text=True)
    for l in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$',l)
        if m: print('%08x %s'%(int(m.group(1),16),m.group(2).strip()))
path = ORIG if sys.argv[2]=='orig' else OURS
dis(path,*sym(path,sys.argv[1]))
```

## 7. Files changed

* `_work/reconstructions/build-provenance-round7-branch-weights.md` -- this note.
* `_work/reconstructions/branch_layout_w11220.py` -- the branch-layout dump tool.
* No `src/`, `CMakeLists.txt`, baseline or allowlist change.
