# `Gun::calcCharacterCollision` pass 4: faithful shape + greedy-RA priority trace (w4010)

Worker: w4010. Branch: `wave90-w4010`, base master `360c7467`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w4010/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `360c7467`) | **4498 / 4498** | **82.7%** | 1984 B |
| after (this pass) | **4498 / 4498** | **82.7%** | 1984 B |

No code change. The tree is left exactly as HEAD. This pass re-confirms the
faithful source shape from Ghidra and adds the missing *allocator-level* proof:
the residual 17.3% is a single greedy-RA priority decision. The faithful shape
already produces the shipped `str.w r2,[r6],#216; str r6,[sp,#24]` post-increment
idiom, but clang assigns `this` to `r4` and `&targetDir` to `r9`; the original
assigns `this` to `r9` and spills `&targetDir`.

## 1. Ghidra ground truth (image base 0x10000, function @ `0x162f90`)

`DecompAt2.java` (headless, `ADDR=0x162f90`, no analysis) emits exactly the
source shape we already have:

```c
bVar6 = iVar18 == 0xb;                       /* mine */
this_00 = (Vector *)(this + 0xd8);           /* &this->targetDir, real local pointer */
*(undefined4 *)this = 0;                     /* lastHitKIPlayer = 0 */
for (uVar19 = 0; uVar19 < *puVar9; uVar19 = uVar19 + 1) {
  this_01 = *(Player **)(puVar9[1] + uVar19 * 4);   /* enemy */
  ...
  for (uVar23 = 0; uVar23 < *(uint *)(this + 8); uVar23 = uVar23 + 1) {
    ...
    if (iVar18 == 0x19) {                    /* scatter */
      ...
      for (uVar21 = 0; uVar21 < **(uint **)(this + 0xb4); uVar21 = uVar21 + 1) {
        this_01 = *(Player **)((*(uint **)(this + 0xb4))[1] + uVar21 * 4);  /* enemy reused */
        Player::getPosition();
        AbyssEngine::AEMath::Vector::operator=(this_00, (Vector *)aAStack_d0);
        AbyssEngine::AEMath::Vector::operator-=(this_00, (Vector *)&local_78);
        fVar24 = (float)AbyssEngine::AEMath::VectorLength(this_00);
        ...
```

Three facts confirmed:

1. `this_00` is a real local `Vector *` to `this->targetDir` (offset `0xd8`).
2. `this_01` (enemy) is reused as the scatter-loop `other`; there is no separate
   `other` variable.
3. There is no `itemIndex` local: both normal damage paths reload
   `*(int *)(this + 0x58)` (`ldr.w r3,[r9,#88]` at `1532e0` and `153304`).

## 2. Shipped Thumb register map (objdump `0x152f90`, 1984 B)

| value | original | HEAD (anchored) | faithful shape (this pass) |
|---|---|---|---|
| `this` | **`r9`** | `r9` | **`r4`** |
| `enemy` outer | **`r8`** | `r6` | `fp` |
| `enemy` inner | `r8` | `r8` via `[sp,#208]` | `fp` |
| `mine` | `r5` (then scratch) | `r5` | `r6` |
| `i` (inner counter) | `fp` (+ spill `[sp,#40]`) | `r1` (+ spill) | `sl` |
| `i*3` | **`r4`** | `r4` | `r8` |
| `&enemy->radius` | `sl` (scratch) | `r6` | spill `[sp,#36]` |
| `&this->targetDir` | **spill `[sp,#24]`** | `sl` | **`r9`** |
| `&position` (`sp+192`) | `sl` (scratch) | rematerialised | rematerialised |
| frame | **208 B** | 216 B | **208 B** |

Shipped prologue + post-increment:

```
152f9c  sub  sp, #208
152f9e  mov  r9, r0                 ; this = r9
...
152ffa  movs r2, #0
153000  mov  r6, r9
153006  str.w r2, [r6], #216        ; lastHitKIPlayer = 0; r6 = this+0xd8
153010  str  r6, [sp, #24]          ; spill &targetDir
```

Shipped scatter receiver (targetDir reloaded from `[sp,#24]`):

```
15343e  movs r5, #0
153440  b.n  1534fc
153442  ldr  r0, [r0, #4]
153446  ldr.w r8, [r0, r5, lsl #2]  ; enemy/other = enemies[k]
15344a  mov  r0, r4
15344c  mov  r1, r8
15344e  blx  Player::getPosition()
153452  mov  r0, r6                 ; r6 = &targetDir (loaded from [sp,#24])
153454  mov  r1, r4
153456  blx  Vector::operator=
15345a  mov  r0, r6
15345c  mov  r1, sl                 ; sl = &position
15345e  blx  Vector::operator-=
153462  mov  r0, r6
153464  blx  VectorLength
```

## 3. The faithful shape already produces the post-increment idiom

Removing the three HEAD anchors (`itemIndex` local, `Player *volatile enemySlot`,
`hitFlag` read) and using `Vector *td = &this->targetDir;` gives (fast scorer
57.0%, 1892 B, frame 208):

```
sub  sp, #208
mov  r4, r0                 ; this = r4
...
movs r2, #0
mov  r9, r4
str.w r2, [r9], #216        ; post-increment, td = r9
```

So `str.w r2,[r6],#216` is **not** an anchor: it falls out naturally from
`lastHitKIPlayer = 0; Vector *td = &this->targetDir;`. The only difference from
the original is which callee-saved register the post-increment result gets
(`r9` here, `r6`+spill in the original) and consequently which register `this`
gets (`r4` here, `r9` in the original).

## 4. New evidence: greedy-RA priority trace

I compiled the faithful body to LLVM IR with the project clang and ran the
project's instrumented LLVM 7 `llc` (assertions enabled):

```
clang++ ... -Oz -emit-llvm -S -o gun_A_nog.ll src/game/weapons/Gun.cpp
/home/debian/llvm7build/build/bin/llc -O2 \
  -debug-only=regalloc -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  gun_A_nog.ll -o gun_A.s 2> regalloc.txt
```

The llc output reproduces the real clang build exactly (`mov r4,r0; mov r9,r4`).
The `regalloc` trace for the function (vreg `%55` = `this`, `%11` = `td`):

```
selectOrSplit GPRnopc:%55 [16r,7728r:0)...[17104B,17216B:0)  weight:4.188202e-02
hints: $r0 $r0 $r0
missed hint $r0
assigning %55 to $r4: R4 ...

selectOrSplit GPRnopc:%11 [1328r,1344e:1)...[17104B,17216B:0)  weight:2.345774e-02
hints: $r0 $r0 $r0 $r4
missed hint $r0
assigning %11 to $r5: R5 ...
...
selectOrSplit rGPR:%476 (i*3) ... weight:1.452280e-02
...
REGISTER MAP: [%55 -> $r4]  [%11 -> $r9]  [%15 -> $r11]  [%18 -> $r10]
              [%476 -> $r8]  [%19 -> $r6]
```

The allocator's priority is (LLVM 7 `RegAllocGreedy::enqueue`, verified in the
source tree at `/home/debian/llvm7build/llvm-7.0.0.src`):

```cpp
Prio = (1u << 29) + LI->getSize();       // global ranges: long -> short
Prio |= (1u << 31);
if (VRM->hasKnownPreference(Reg)) Prio |= (1u << 30);   // copy hint
```

`this` is the largest global live range (size 16216) **and** carries the `r0`
copy hint, so it is popped first and takes the first callee-saved register in
the ARM allocation order (`r4`). `td` (size 11790, hint `r4`) is popped second
and takes `r5`, then is evicted to `r9` later. `i*3` (size ~8640) is popped
much later and takes `r8`.

For the original to have `this = r9`, four other global ranges would have to be
allocated before it (`r4,r5,r6,r8`), i.e. `i*3`/`mine`/`enemy`/`td` would need
larger live-range sizes than `this` in the original's pre-RA block layout. The
live-range size is the sum of segment lengths in the pre-RA slot numbering, so
this is a block-layout/IR-order property, not a source-semantics property.

## 5. Experiments

### 5.1 Faithful-shape variants (fast scorer, tracks official within ~1 pt)

Baseline HEAD 82.7 / 1984. Faithful shape (no anchors, `td` local) = 57.0 /
1892. All of the following were measured on the faithful shape:

| variant | pct | size | notes |
|---|---:|---:|---|
| faithful (td local, no anchors) | 57.0 | 1892 | `this=r4`, `td=r9`, frame 208 |
| no `td` local (`this->targetDir` direct) | 57.0 | 1892 | identical codegen |
| `td` before store / assigned / const / ref / char-cast / two aliases | 57.0 | 1892 | identical |
| `i` declared outside / function scope / before store / `int` | 57.0 | 1892 | identical |
| `e`/`k`/`enemy` declared outside; `enemy` reference | 57.0 | 1892 | identical |
| function-scope `radius`, `mine`, `scatter`, `i`, `enemy`, `e`, `k` | 64.5* | 1976 | *with `-sink` |
| `const bool mine/scatter`, `i++`/`e++`/`k++`, `explosive` at decl | 57.0 | 1892 | identical |
| cached `positions.count` local (`n`) | 50.7 | 1896 | no sink |
| cached `n` + `-sink-insts-to-avoid-spills` | 66.7 | 1980 | `this=r9`, `enemy=r8`, **td spill `[sp,#24]`**, `i*3=r4`, frame 224 |
| cached `n` + `hitFlag` + `-sink` | 66.8 | 1984 | size-exact but frame 224 |
| volatile `td` slot + `-sink` | 66.0 | 1984 | td spill `[sp,#216]` |
| volatile `mine` slot | 51.6 | 1912 | `&position=sl` |
| volatile `scatter`/`explosive`/`eoc` slot | 48.2-55.6 | 1856-1912 | worse |

### 5.2 Compiler-flag probes (llc, faithful IR)

`-sink-insts-to-avoid-spills` is the only flag that moves `this` to `r9`:
`this=r9`, `enemy=r8`, but `td=fp` (spilled to `[sp,#20]`), frame 216, `i=r1`.
`-enable-deferred-spilling`, `-disable-spill-hoist`, `-split-spill-mode`,
`-spp-rematerialization-threshold`, `-regalloc-csr-first-time-cost=0..65536`,
`-misched-postra`, `-disable-post-ra`, `-align-neon-spills` all leave the
allocation unchanged (`this=r4`, `td=r9`). `-regalloc=basic` gives `this=r6`,
frame 200; `-regalloc=fast` and `-optimize-regalloc=false` are far off.

The `n`-local + `-sink` result is the closest reproduction of the shipped
prologue: `this=r9`, `enemy=r8`, `i*3=r4`, and `str.w fp,[sp,#24]` (the shipped
slot), but the post-increment base is `fp` instead of `r6` and the frame is
224 B because the cached count occupies a stack slot. The shipped code reloads
`this->positions.count` every iteration (`ldr.w r0,[r9,#8]` at `15305e`), so the
cached-count local is **not** faithful.

## 6. Conclusion

The faithful source shape is confirmed and already reproduces the shipped
post-increment idiom and the 208-byte frame. The whole residual diff is one
greedy-RA priority decision: in the original `this` is allocated to `r9` and
`&targetDir` is spilled to `[sp,#24]`; in our build `this` is the highest
priority global range and takes `r4`, while `&targetDir` takes `r9`. The
allocator priority is `(1<<29)+live-range-size` plus a `(1<<30)` copy-hint bit,
so reproducing the original requires the original's pre-RA block layout /
live-range sizes, not a different C++ expression. Every natural source-shape
variant tried (this pass plus w3805/w3829/w3909, ~150 total) either keeps
`this=r4` or, with `-sink-insts-to-avoid-spills`, gets `this=r9`/`enemy=r8` but
loses the `[sp,#24]` spill and the 208-byte frame.

Remaining work: find a source/IR-order shape that makes `i*3`, `mine`, `enemy`
and `td` outrank `this` in live-range size (or removes `this`'s copy hint) while
keeping the faithful semantics. The `n`-local + `-sink` result shows the target
prologue is reachable; the missing piece is keeping the count reload (no stack
slot) while still spilling `td` to `[sp,#24]`.

## 7. Files changed

None (note only). Gate: byte 4498 / linked 4498, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26290`, lint CLEAN, drift 0.
