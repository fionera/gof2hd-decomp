# `Gun::calcCharacterCollision` wave 116 (w6415): residual decomposed to the
# MachineBlockPlacement / greedy-RA decisions; no source shape beats 93.3846 %

Worker: w6415. Branch: `wave116-w6415`, base master `d672d349f`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w6415/`.

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `d672d349f`) | **4501 / 4501** | **93.3846** (report 93.4) | 607/650 | 1984 |
| after this pass | **4501 / 4501** | **93.3846** | 607/650 | 1984 |

No code change: this pass is analysis only. The tree is left exactly as HEAD.
Gate after: byte 4501/4524, linked 4501/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab`
change (27 sections, `.ARM.extab 0x26298`, file size 2,187,328 B).

## 1. The residual is five independent codegen artefacts

`tools/verify` reconcile diff of the 43 mismatched instructions, by cluster
(instruction indices are the reconciled stream):

| # | cluster | insns | root cause |
|---|---|---:|---|
| 1 | `i*3` in `r5`, shipped `r4` | 9 (77, 80, 86, 108, 153, 392, 536, 541, 551) | greedy-RA eviction order |
| 2 | scatter temps swapped: shipped `r4=sp+44` (playerPosition) / `r5=sp+104` (delta), ours reversed | 2 (110, 114) | `StackSlotColoring` merge target |
| 3 | `isAsteroid` join reloads: shipped `ldr.w fp,[sp,#40]` + `ldr r6,[sp,#24]`, ours rematerialises `add r4,sp,#104` and reloads `fp` 3 insns later | 2 (178, 242) + 1 shift | consequence of cluster 1 |
| 4 | Matrix block: shipped reuses `r5=sp+104`, ours re-materialises `add r0/r1/r4,sp,#104` | 5 (356–371) | consequence of cluster 1 |
| 5 | inner `k`-loop rotation: shipped condition at the bottom (`movs r5,#0; b cond; body; adds r5,#1; cond: cmp; bcc body`), ours at the top (`cond: cmp; bcs exit; body; adds r5,#1; b cond`) | ~10 (395–456) | `MachineBlockPlacement::rotateLoop` bails |
| 6 | branch/literal offsets that follow from 1–5 | rest | address shifts |

## 2. Cluster 5 root cause: `findBestLoopExit` returns null

`loop-rotate` is a no-op at `-Oz` (`createLoopRotatePass(SizeLevel == 2 ? 0 : -1)`
in `PassManagerBuilder.cpp`; `MaxHeaderSize == 0` rejects every non-empty header).
The rotated layout is produced by **`MachineBlockPlacement::rotateLoop`**, not by
the IR pass. Verified with `-mllvm -print-after=loop-rotate`-equivalent dumps
(`-print-after-all -filter-print-funcs=...`): the `Rotate Loops` dump still shows
the k-loop header `%463` as `%469 = icmp ult; br i1 %469, label %470, label %514`
(i.e. unrotated).

The k-loop blocks in the MBP dump are `bb.89` (cond, `k < enemies->size()`),
`bb.90` (body), `bb.95` (latch) and `bb.91..bb.94` (the `distance < magnitude`
body). Final MBP order is `bb.88, bb.89, bb.90, bb.95, bb.91, bb.92, bb.93, bb.94`.

* `buildLoopChains` starts the chain at `LoopTop = L.getHeader() = bb.89`
  (`findBestLoopTop` short-circuits to the header under `optForSize()`).
* `selectBestSuccessor(bb.89)` → `bb.90` (96.88 %), `selectBestSuccessor(bb.90)`
  → `bb.95` (99.95 %, because `__builtin_expect(distance < magnitude, 0)` gives
  the if-body edge weight 0x00106035 ≈ 0.05 %). Chain = `[bb.89, bb.90, bb.95]`.
* `selectBestSuccessor(bb.95)` has only `bb.89` (same chain) → nullptr, so
  `getFirstUnplacedBlock` appends the cold if-body: chain =
  `[bb.89, bb.90, bb.95, bb.91, bb.92, bb.93, bb.94]`.
* `rotateLoop` bails on
  `ExitingBB->isSuccessor(NextBlockInChain) && !Bottom->isSuccessor(Top)`:
  `ExitingBB = bb.89`, `NextBlockInChain = bb.90` (successor → true),
  `Bottom = bb.94`, `Top = bb.89`, and `bb.94 → bb.89` is false → **return**.

Shipped order `[bb.90, bb.91..94, bb.95, bb.89]` requires the if-body to be
*inside* the chain, i.e. `selectBestSuccessor(bb.90)` must pick `bb.91`, i.e. the
`distance < magnitude` edge must be ≥ 50 %. That is the "hot if-body" layout.

Confirmed empirically: with the `expect(...,0)` removed (or flipped to `1`) the
k-loop becomes exactly the shipped shape (`adds r5,#1` immediately followed by
the `ldr.w r0,[r9,#180]` condition reload). But the same weight change also
re-colours the whole function: `this` drops from `r9` to `r4`, the frame shrinks
1984 → 1908 B and the score collapses to 64.54 %. Conversely every shape that
keeps `this = r9` (and 1984 B) keeps the if-body cold and the cond at the top.

## 3. Cluster 1 root cause: greedy-RA priority is `(1<<31)+(1<<29)+size`

`RegAllocGreedy::enqueue` (LLVM 7 `RegAllocGreedy.cpp:678`):

```cpp
Prio = (1u << 29) + LI->getSize();       // global ranges: long -> short
Prio |= (1u << 31);
if (VRM->hasKnownPreference(Reg)) Prio |= (1u << 30);   // copy hint
CurQueue.push(std::make_pair(Prio, ~Reg));               // lower vreg wins ties
```

`i*3` is a *global* range (defined in `bb.18`, redefined in `bb.23` after the
`%525 = COPY %526` back-copy), so it is allocated long-before-short, long before
the local `&playerPosition` range. In the shipped build it gets `r4`; in ours it
gets `r5` while `r4` is taken later by `&playerPosition` (`%467`). The choice is
pure eviction order inside `R4..R11`; every register the value can legally live
in is callee-saved because `i*3` is live across the whole scatter block
(`hitPositions.data()[i]` at `0x153436` reuses it).

Instrumented-llc trace (`/home/debian/llvm7build/build/bin/llc
-debug-only=regalloc`) shows the mechanism directly: the `i*3` interval is
`prio=2684362166` (size 7606) and is assigned with `assigning %503 to $r6`,
then `%502` (a split component) takes `r3`. The real clang build differs from
llc by one slot (`i*3` = `r5` vs `r6`, frame 208 vs 200), i.e. the decision is
already at the noise floor of the allocator.

## 4. Measurements (official verifier path; `link_so` + `tools/verify` scoring)

All variants below are applied to the HEAD body (`/dev/shm/w6415/Gun.cpp.base`,
93.384615 % / 1984 B) unless stated. `i3` = register of `i*3`; `join` = whether
the `isAsteroid` join reloads `fp` from `[sp,#40]` (shipped = yes).

### 4.1 The `distance < magnitude` branch weight is the pivot

| form | pct | size | i3 | join | k-loop |
|---|---:|---:|---|---|---|
| **`expect(distance < magnitude, 0)` (HEAD)** | **93.3846** | **1984** | r5 | yes | top-tested |
| no expect | 64.5412 | 1908 | r0 | no | rotated |
| `expect(...,1)` | 64.5412 | 1908 | r0 | no | rotated |
| `this->magnitude > distance` (plain) | 64.5412 | 1908 | r0 | no | rotated |
| `this->magnitude > distance` + `expect(...,0)` | 93.3846 | 1984 | r5 | yes | top-tested |
| `!(distance >= magnitude)` (plain / +expect 0) | 64.5412 / 93.3846 | 1908 / 1984 | r0 / r5 | no / yes | rotated / top |
| `if (distance >= magnitude) continue;` (plain / exp1 / exp0) | 64.5412 / 93.3846 / 64.5412 | 1908 / 1984 / 1908 | r0 / r5 / r0 | no / yes / no | rotated / top / rotated |
| empty then-block + `expect(distance >= magnitude, 1)` | 93.3846 | 1984 | r5 | yes | top-tested |
| empty else-block | 64.5412 | 1908 | r0 | no | rotated |

### 4.2 `__builtin_expect` site sweep on top of the HEAD expect (29 sites × {0,1})

Codegen-identical (93.3846 / 1984): `enemies == nullptr`(0), `bomb || mine`(0,1),
`playerGun`(0,1), `impact->isRocket()`(1), `falloff > 1.0f`(0,1),
`empDamage > 0`(0). Best non-identical: `empDamage != kNoAttribute`(1)
92.7692 / 1984; `owner == nullptr || owner->enemyFlagsLo`(1) 90.3077 / 1984;
`mine` radius(0) 88.8376 / 1984 (**i3=r6**); `impact->isRocket()`(0)
87.7112 / 1988; `empDamage > 0`(1) 87.6923 / 1984; `explodeOnContact`(0)
85.1424 / 1984; `getKIPlayer()->field_0x71`(0) 83.7567 / 1984.
`i3=r4` appears only at `mine` radius(1) 73.3538 / 1996, `scatter`(0)
72.6437 / 1992, `scatter`(1) 84.8998 / 1980 (`i3=r6`).

### 4.3 Expect moved *off* the distance branch (22 sites × {0,1})

Best: `scatter`(1) **80.5235 / 1980 / i3=r4 / join=yes** (k-loop rotated);
`scatter`(1) with the distance expect kept → 84.8998 / 1980 / i3=r6.
`egoFlag`(1) 55.5298 / 1928 / i3=r4. Everything else ≤ 65.8.
`expect(distance,1)` + one other site (18 × 2): best `scatter`(1)
73.4411 / 1924 / i3=r6 / join=yes.

### 4.4 Tail-order sweep (120 permutations) with **no** distance expect

Best with the shipped k-loop shape (rotated): `P H K F L` **85.6703 / 1980 /
i3=r4 / join=yes**; `P K H F L` and `F H P L K` 85.5162 / 1980; `F H P K L`,
`F H K P L` 85.2080; `K F H P L`, `F K H P L` 85.0539; `F H K L P`
84.8345 / 1984; `K F H L P`, `F K H L P` 84.5266 / 1984; everything else
≤ 75.64. (With the distance expect kept, wave 115's sweep found `F P H L K`
93.3846 best, next `F P L H K` / `F P H K L` 92.9231.)

### 4.5 Other spellings — all 93.384615 / 1984 (codegen-identical)

`Vector position(this->positions.data()[i])` copy-ctor; `this->positions[i]`
(`Array::operator[]`); `*(this->positions.data() + i)`; `const KIPlayer *ki`;
`const Vector enemyPosition`; `(int) enemy->radius`; `float r = radius;`;
`float nr = (float) -radius;` + use; `bool ego = enemy->egoFlag;` + use;
`int radius = enemy->radius; if (useCustomRadius) radius = customRadius;`;
`int radius; if/else`; `bool outer = ki && ki->field_0x40;` /
`ki != 0 && ki->field_0x40 != 0`; `const float r`;
`float fdistance = ...; int distance = (int) fdistance;`;
`bool explodeOnContact`; `bool explosive`; `const bool mine/scatter`;
`int k`; `Vector ep = enemy->getPosition(); *td = ep;`.

Worse: `Player *other` in the k-loop 56.0806 / 1896; `*td = *td - position`
54.7417 / 1936; `float distance` in the k-loop 87.6543 / 1972; `int e`
93.2308 / 1984; two `if`s instead of `if/else if` 61.6923 / 1976;
scatter-first 60.3077 / 1976.

## 5. Conclusion

The remaining 43 instructions are not a source-semantics difference. They are
two compiler decisions that are individually reachable but mutually exclusive
from every source shape tried (≈ 400 variants across waves 106–116):

1. the `distance < magnitude` branch must be *cold* to keep `this` in `r9` and
   the 1984-byte frame (which fixes clusters 1, 3, 4 and the tail), but that
   makes `MachineBlockPlacement::rotateLoop` bail and leaves the k-loop
   top-tested (cluster 5);
2. the same branch must be *hot* to rotate the k-loop into the shipped shape,
   which demotes `this` to `r4`, shrinks the frame to 1908 B and costs ~29 points.

`i*3 = r4` (cluster 1) is likewise only reachable with a wholesale register-map
change. The best hybrid found is `scatter`(1)-only at 80.52 % with
`i3=r4 ∧ join=yes ∧ rotated k-loop`; the best overall remains the HEAD body at
**93.3846 % / 1984 B**.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

Scratch harnesses (`/dev/shm/w6415/`): `score.py` (full-precision verifier
metric), `v.py` / `v2.py` (apply variant + `link_so` + score + register
signature), `diff.py` (reconciled instruction diff), `sweep1.py`–`sweep4.py`
(expect-site sweeps), `tailsweep2.py` (120 tail permutations, no-expect body),
`opt.dump` (`-mllvm -print-after-all` for this function, baseline),
`ra2.txt` (instrumented-llc `-debug-only=regalloc` trace).

## 7. References

`_work/reconstructions/gun-calccharactercollision-wave115-w6315.md` (landed
shape), `...-wave114-w6213.md`, `...-wave113-w6112.md`, `...-wave112-w6011.md`,
`...-wave111-w5914.md`, `...-wave110-w5814.md`,
`...-faithful-shape-w4010.md` (greedy-RA priority trace).
