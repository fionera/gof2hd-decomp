# `Gun::calcCharacterCollision` pass 5: the free `[sp,#24]` slot proves the original is the faithful shape + a `td` spill (w4110)

Worker: w4110. Branch: `wave91-w4110`, base master `a4557300`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w4110/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `a4557300`) | **4498 / 4498** | **82.7%** | 1984 B |
| after (this pass) | **4498 / 4498** | **82.7%** | 1984 B |

No code change: every source-shape variant tried below either keeps the
volatile-slot build at 82.4 (oracle) or loses the `this = r9` allocation. The
new evidence is a *stack-slot* proof that the original is exactly the faithful
body with `Vector *td` spilled to the one free 4-byte slot at `[sp,#24]`.

## 1. New evidence: the faithful body already has a free `[sp,#24]`

Compile the faithful body (w4010's shape: `Vector *td = &this->targetDir;`, no
`itemIndex` local, no volatile slot, no `hitFlag`) and dump every prologue
store. Frame is **208 B** and the slots are:

```
sub sp, #208
str r0, [sp, #8]        ; canary pointer
str r0, [sp, #204]      ; canary
str r1, [sp, #28]       ; outer loop index e
str r2, [sp, #16]       ; bomb||mine
str r1, [sp, #12]       ; explosive
str r6, [sp, #40]       ; i
str r2, [sp, #20]       ; scatter
str r0, [sp, #36]       ; mine
str r0, [sp, #32]       ; (i*3 / address temp)
... vectors at [sp,#44], [sp,#104], [sp,#164], [sp,#176], [sp,#192]
```

`[sp,#24]` is **unused**. The shipped original's frame is also 208 B, and its
`td` spill lives in exactly that hole:

```
152f9c  sub  sp, #208
152f9e  mov  r9, r0                 ; this = r9
...
153000  mov  r6, r9
153006  str.w r2, [r6], #216        ; lastHitKIPlayer = 0; r6 = this+0xd8
153010  str  r6, [sp, #24]          ; td spill into the faithful free slot
...
153018  str  r2, [sp, #28]          ; e, one slot later (faithful has e here)
```

The original reloads `td` from `[sp,#24]` at `15319c`, `153266`, `1533f6`.
This is the strongest evidence yet that the source is the faithful body with a
spilled `Vector *td = &this->targetDir;`: the faithful body's stack layout
already reserves the exact slot the original uses, and the original's `e`
moves from the faithful `[sp,#28]` to `[sp,#24]` only in the *anchored* build
(which has no `td` spill).

## 2. Why the current anchored build wins 82.7

The current HEAD keeps three anchors: `const int itemIndex`, the volatile
`enemySlot`, and the `hitFlag` read. The volatile slot is what makes
`this = r9`. Stack maps:

| slot | original | HEAD (anchored) | faithful |
|---|---|---|---|
| canary ptr | `[sp,#12]` | `[sp,#8]` | `[sp,#8]` |
| canary | `[sp,#204]` | `[sp,#212]` | `[sp,#204]` |
| `td` | **`[sp,#24]`** | — (`sl`) | — (`r9`) |
| outer `e` | `[sp,#28]` | `[sp,#24]` | `[sp,#28]` |
| `i*3` | — (`r4`) | `[sp,#28]` | — (`r8`) |
| `i` | `[sp,#40]` | `[sp,#40]` | `[sp,#40]` |
| `enemySlot` | — (`r8`) | **`[sp,#208]`** | — |
| frame | **208** | **216** | **208** |

The volatile slot adds 8 B (4 B slot + 8-byte frame alignment), so HEAD is
216 B. It also adds a seventh long-lived value (the outer enemy), which is what
pushes `this` from `r4` to `r9`, but it leaves `td` in `r10` (`sl`).

## 3. Greedy-RA priority traces (LLVM 7, instrumented `llc`)

Priority is `(1<<29) + live-range-size`, plus `(1<<30)` for a copy hint and
`(1<<31)` for global ranges. All of `this`, `td`, `enemy`, `mine`, `i*3` carry
the `r0` copy hint, so among them the order is pure live-range size.

### Faithful body (frame 208, `this = r4`)

```
selectOrSplit %55 (this)  size 16216 -> r4
selectOrSplit %12 (td)    size 11790 -> r5, evicted to r9
selectOrSplit %7  (mine)  size 12540 -> r6
selectOrSplit %476 (i*3)  size ~12416 -> r8
selectOrSplit %18 (i)     size ~11168 -> r10
selectOrSplit %15 (enemy) size 11168 -> r11
REGISTER MAP: %55->r4, %12->r9, %15->r11, %18->r10, %19->r6, %476->r8
```

`this` is the largest hinted range, so it is popped first and takes `r4`;
`td` is second and takes `r9`.

### Anchored HEAD (frame 216, `this = r9`, `td = r10`)

```
selectOrSplit %56 (this)        size 16216 -> r4
selectOrSplit %13 (td)          size 11790 -> r5, evicted to r10 (sl)
selectOrSplit %11 (outer enemy) size 11710 -> r6, evicted to r8, then spilled
selectOrSplit %18 (inner enemy) size ~11168 -> r8
selectOrSplit %20 (radius)      size ~10318 -> r11
selectOrSplit %46 (i*3)         size ~9842  -> r6
REGISTER MAP: %56->r9, %13->r10, %18->r8, %20->r11, %46->r6, %11->fi#19
```

The volatile slot forces the outer and inner enemy to be two separate values
(`%11` and `%18`). That extra long-lived value changes the eviction cascade:
`this` eventually lands in `r9`, but `td` still gets `r10` because the
allocator has a free callee-saved register for it.

## 4. Experiments

All pct/size/frame/`this`/`td` below are from the object-level oracle
(`_work/reconstructions/w3692_oracle.py`, adapted to this worktree; it tracks
the official verifier within ~0.3 points). HEAD oracle = **82.4 / 1984 /
frame 216 / this=r9 / td=sl**.

| variant | oracle pct | size | frame | this | td |
|---|---:|---:|---:|---|---|
| HEAD (anchors) | **82.4** | 1984 | 216 | r9 | sl |
| faithful (no anchors) | 57.3 | 1892 | 208 | r4 | r9 |
| faithful + `td` local (top / outer / inner / scatter) | 57.3 | 1892 | 208 | r4 | r9 |
| faithful + `Vector &td` | 57.3 | 1892 | 208 | r4 | r9 |
| faithful + `Vector **tdp` | 57.3 | 1892 | 208 | r4 | r9 |
| faithful + `reinterpret_cast`/offset `td` | 57.3 | 1892 | 208 | r4 | r9 |
| faithful + volatile `td` | 59.1 | 1896 | 216 | r4 | spill |
| faithful + volatile dummy slot | 55.6-56.2 | 1896-1908 | 216 | r4 | r9 |
| faithful + cached `damage`/`emp`/`friend`/`magnitude` | 50.3-55.8 | 1896-1904 | 216 | r4 | spill |
| HEAD + `td` local (top / after null / outer / inner / scatter) | 74.5 | 1972-1980 | 216 | r9 | sl |
| HEAD + `Vector &td` / `Vector **tdp` / cast `td` | 74.5 | 1972-1980 | 216 | r9 | sl |
| HEAD + volatile `td` | 55.4 | 1900 | 216 | r4 | spill |
| HEAD + no `itemIndex` | 74.8 | 1972 | 216 | r9 | r8 |
| HEAD + no `hitFlag` | 79.0 | 1980 | 216 | r9 | sl |
| HEAD + volatile final-else damage | 82.3 | 1984 | 216 | r9 | sl |
| HEAD + volatile enemy slot declared at top/after-null/before-loop | 71.3-79.3 | 1980-1988 | 216 | r9 | sl |
| HEAD + cached `damage`/`emp`/`friend`/`magnitude` | 49.2-67.8 | 1984-2012 | 224-240 | r1/r9 | sl |
| HEAD + cached `positions`/`velocities`/`hitPositions` refs | 82.4 | 1984 | 216 | r9 | sl |
| HEAD + `Gun *self` used for all member accesses | 82.4 | 1984 | 216 | r9 | sl |
| HEAD + `__builtin_expect` on other branches | 33.5-76.7 | 1920-1992 | 216-224 | r1/r4/r9 | sl |
| HEAD + reordered bool/check/damage statements | 73.1-82.4 | 1972-1992 | 216 | r9 | sl |

Type/const/`register`/comment/blank-line sweeps are codegen-identical to HEAD
(82.4). Global `-mllvm -sink-insts-to-avoid-spills` is not viable: it moves
`this` to `r9` on the faithful body (td spilled to `[sp,#20]`, frame 216) but
drops the whole-so gate to `byte_exact 1477 / linked_exact 4345`.

## 5. Conclusion

The original is the faithful body with `Vector *td = &this->targetDir;` spilled
into the free `[sp,#24]` slot. The faithful body's frame is already 208 B and
its stack map has a hole exactly at `[sp,#24]`; the original's `td` spill fills
that hole and moves `e` to `[sp,#28]`. The remaining blocker is purely the
greedy allocator's priority: in the faithful body `this` (16216) is the largest
hinted global range and takes `r4`, while `td` (11790) takes `r9`. To reproduce
the original we need `this` to be allocated after `i*3`, `mine`, `enemy` and
`td` (or to lose its `r0` copy hint) while keeping the faithful count reload
and the 208-byte frame. Every source shape tried this pass either keeps
`this = r4` (faithful family) or keeps `td` in a register (anchored family).

## 6. Files changed

None (note only). Gate: byte 4498 / linked 4498, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26290`, lint CLEAN, drift 0.
