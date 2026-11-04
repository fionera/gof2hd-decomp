# PlayerEgo::calcCollision — wave 105 (w5316): the greedy enqueue is by
# live-range *size*, not spill weight — the A2 anchor flips dock/i by
# lengthening dock's interval

**Status:** research note, 2026-11-04. Worktree `wt/w5316`, branch
`wave105-w5316`, base `835748c18`. **No source change landed** — the
checked-in 87.6 % / 968 B shape is still the best measured. This note adds
the LLVM-7.0.2 `RAGreedy` enqueue rule (read from
`/dev/shm/wf303/llvm-7.0.0.src/lib/CodeGen/RegAllocGreedy.cpp`) and ~40 new
measured builds (surface forms, loop-index types, cheap anchors, dock-weight
probes).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B,
347 instructions, 87.6 %).

## TL;DR

The greedy allocator does **not** order its queue by spill weight. Its
`enqueue()` (`RegAllocGreedy.cpp:680`) pushes `(Prio, ~Reg)` where `Prio`
is `(1u<<29) + LI->getSize()` for global ranges (and a local-distance
priority for single-MBB ranges). So the register file is decided by
**which of the two loop-spanning intervals has the larger total live-range
size**, with the lower vreg number as tie-breaker.

That is exactly the observed fixed point:

* the committed two-anchor body makes **dock's** interval larger, so dock
  is dequeued first and takes `r9`; `i` takes `fp`;
* the wrap body leaves **i's** interval larger, so `i` takes `r9`; dock
  takes `fp`.

The single source construct that flips it is the **A2 anchor**
(`if (objects->size() == i) continue;` in the wormhole arm): the extra
`objects->size()` load there extends dock's interval past i's. A cheap
register/constant anchor (`if (i == 0xFFFFFFFFu) continue;`,
`if (this->miningGame == 0xFFFFFFFFu)`) does **not** flip it (and is folded
away, identical to the wrap). `if (objects->size() == 0) continue;` in the
wormhole arm flips it identically to A2.

No new source spelling beats **87.6 % / 968 B**.

## 1. Original binary facts (re-confirmed)

Manual Thumb disassembly of `android_2.0.16_libgof2hdaa.so`
`0x9f0cc`–`0x9f492`:

* preheader spills `&dockOffsetVec` once: `0x9f110 add.w r9,r5,#328;
  0x9f12a str.w r9,[sp,#20]`; reloaded at `0x9f362`, `0x9f392`, `0x9f44c`.
* top-tested loop: header `0x9f134`, latch `0x9f38c`
  (`add.w fp,fp,#1; b.n 0x9f134`). One `[r4,#0]` size load per iteration.
* register file `this=r5 objects=r4 object=r8 dock=r9 i=fp surface=r6`;
  canary pointer spilled to `[sp,#24]`; frame 112.
* two `cmp fp,#0`: `0x9f14a` (i==0 && !inAlienOrbit …) and `0x9f374`
  (i==0 && object->field_0x71, the collision-arm guard).
* collision-arm register shuffle is real:
  `0x9f30c mov sl,r5; 0x9f30e mov r5,r6; 0x9f310 add r6,sp,#96; …;
  0x9f326 mov r6,r5; 0x9f328 mov r5,sl`.
* literal pool: `16000.0f`, `0.2f`, `0.1f`, then the GOT slots for
  `__stack_chk_guard`, `Globals::status`, `Globals::sound`,
  `Globals::Canvas` ×2.

REA (`rea decompile … --procedure <MANGLED> --format json`) confirms the
nested source shape:

```cpp
for (uVar6 = 0; uVar6 < *param_1; uVar6++) {
    if (this_01 != 0) {
        if (outerCollide(...) != 0) {
            if (wormhole) ... else if (stealFlag == 0) ... else ...
            if ((uVar6 == 0) && this_01[0x71]) this[0x234] = 1;
            hitCamera();
        }
    }
}
```

## 2. The three attractors

| variant | pct | size | insns | this | dock | i | surface | latch |
|---|---:|---:|---:|---|---|---|---|---|
| **checked-in (A1+A2, no guard)** | **87.6** | **968** | **349** | r5 | **r9** | **fp** | r6 | rotated (after A1) |
| wrap (`if (i < size)` after outerCollide) | 87.3 | 968 | 345 | r5 | fp | r9 | r6 | at end |
| wrap + A2 | 85.5 | 976 | 348 | r5 | **r9** | **fp** | r6 | at end |
| wrap + A2 + real guard | 85.7 | 984 | 350 | r5 | r9 | fp | r6 | at end |
| NAT (no anchors, real guard) | 59.8 | 996 | 352 | **sl** | r9 | fp | **r5** | rotated |
| nested + scoped surface + bool | 59.8 | 996 | 352 | sl | r9 | fp | r5 | rotated |

The wrap is the original's **block layout** (top-tested, latch after
`hitCamera`) at the original size, but with `dock`/`i` swapped. The
committed shape is the original's **register file** but with the latch
rotated after A1 and two redundant bounds checks.

## 3. Why the A2 anchor flips the pair (new this wave)

`RAGreedy::enqueue` (`RegAllocGreedy.cpp:680-737`):

```cpp
const unsigned Size = LI->getSize();
...
Prio = (1u << 29) + Size;      // global/split ranges
...
CurQueue.push(std::make_pair(Prio, ~Reg));   // larger Size first, lower vreg first
```

`LI->getSize()` is the sum of the segment lengths, not the span. Both
`dock` (`%20`/`%0`) and `i` (`%3`/`%177`) span the loop, but their segment
sums are close enough that one extra block use flips the order:

* **wrap**: `i` (`%3`) span 752–8320 (7568), dock (`%0`) only 464–1184
  in the LIA dump but the loop-carried copy keeps it live to the back
  edge; `i` is enqueued first → `r9`, dock → `fp`.
* **committed / wrap+A2**: the extra `objects->size()` load in the
  wormhole arm lengthens dock's loop-carried segment past `i`'s; dock is
  enqueued first → `r9`, `i` → `fp`.

Measured probes that confirm the rule:

| probe | flips? | pct | size | note |
|---|---|---|---|---|
| wrap + `if (objects->size() == i) continue;` (A2) | **yes** | 85.5 | 976 | dock=r9 |
| wrap + `if (objects->size() == 0) continue;` | **yes** | 85.5 | 976 | dock=r9 |
| wrap + `if (i == 0xFFFFFFFFu) continue;` | no | 87.3 | 968 | folded away |
| wrap + `if (this->miningGame == 0xFFFFFFFFu) continue;` | no | 87.3 | 968 | folded away |
| wrap + `if (i == 0xFFFFFFFEu) continue;` | no | 80.8 | 972 | dock=fp |
| wrap + `dock.x == dock.x` in wrap cond | no | 74.4 | 996 | dock=fp |
| wrap + `dock.x < 16000.0f` in alien cond | no | 79.2 | 984 | dock=fp |
| wrap + `dock.z < 16000.0f` in bottom guard | no | 78.3 | 984 | dock=fp |
| wrap + `Vector dock = this->dockOffsetVec` local | no | 87.3 | 968 | identical |
| wrap + `Vector *dock = &…` local | no | 87.3 | 968 | identical |
| wrap + `unsigned long` / `size_t` / `++i` index | no | 87.3 | 968 | identical |
| wrap + `Vector surface(geometry->getPosition())` | no | 87.3 | 968 | identical |
| wrap + direct `outerCollide(geometry->getPosition())` | no | 87.3 | 968 | identical |
| NAT + `if (i == 0xFFFFFFFFu) continue;` | — | 67.6 | 952 | this=r5, i=r9 |

## 4. Exact remaining diff (committed vs original, normalized)

Branch targets and PC-relative literals normalized; `TGT` = any address.
`-` original, `+` committed:

```
 ldr r0, [pc, #860]
-str.w r9, [sp, #20]                       <- missing preheader dock spill
 add r0, pc
 ...
 cmp.w r8, #0
-beq.w TGT
+beq.n TGT
 cmp.w fp, #0
 ...
 blx r2
-cmp r0, #0
-beq.w TGT
+cbz r0, TGT                               <- A1 (5 insns)
+ldr r0, [r4, #0]
+subs r0, #1
+cmp fp, r0
+bls.n TGT
+add.w fp, fp, #1                          <- rotated latch
+b.n TGT
 mov r0, r8
 ...
 cmp r0, #0
-bne.w TGT
+bne.n TGT
 ldr.w r0, [r5, #484]
 cmp r0, #0
-bne.w TGT
-ldr.w r0, [r8]
-mov r1, r8
+bne.n TGT
+ldr r0, [r4, #0]                          <- A2 (3 insns)
+cmp fp, r0
+beq.n TGT
+ldr.w r0, [r8]
+mov r1, r8
+str.w r9, [sp, #20]                       <- lazy dock spill
 ...
 blx TGT
 ldr.w r0, [r8]
-mov sl, r5                                <- collision-arm shuffle (4 insns)
-mov r5, r6
-add r6, sp, #96
-mov r1, r8
-mov r2, r5
+add.w sl, sp, #96
+mov r1, r8
+mov r2, r6
 ldr r3, [r0, #88]
-mov r0, r6
+mov r0, sl
 blx r3
 movs r0, #0
+mov r1, sl
 strd r0, r0, [sp, #32]
 str r0, [sp, #40]
 mov r0, r6
-mov r6, r5
-mov r5, sl
-mov r1, r6
 blx TGT
 ...
-cmp.w fp, #0                              <- missing bottom guard (2 insns)
-bne.n TGT
 ldrb.w r0, [r8, #113]
```

The committed body is 968 B despite the 5 A1 + 3 A2 instructions because
the rotated latch keeps every `continue` target close, so clang emits
`beq.n`/`bne.n` (2 B) where the original's end-of-body latch forces
`beq.w`/`bne.w` (4 B). The original's un-rotated layout is only reachable
with the wrap, and the wrap's extra `i` use is what swaps `dock`/`i`.

## 5. Proven vs. hypothesis

**Proven (measured):**

* `RAGreedy::enqueue` orders by `LI->getSize()`, not spill weight; the
  lower vreg number is the tie-breaker.
* The A2 anchor flips `dock`↔`i` by lengthening dock's loop-carried
  segment; a constant/register anchor does not.
* `wrap + A2` is the closest un-rotated shape (right register file, right
  latch position) but is 8 B too long (976 vs 968).
* ~40 new builds this wave (surface forms, index types, cheap anchors,
  dock-weight probes, explicit `continue`, anchor placement) do not beat
  87.6 % / 968 B.

**Hypothesis (not proven):**

* The original source is the REA nested shape compiled with the
  un-rotated block layout. The 8 B that `wrap + A2` spends on the
  redundant `objects->size()` anchor must come from somewhere the
  original does not pay: the original's `i == 0 && field_0x71` guard and
  the collision-arm shuffle are *cheaper* than the wrap's `.w` branch
  widths, but on this clang 7.0.2 `-Oz` build the two attractors
  (right register file vs. right block layout) remain mutually exclusive
  from source alone.
* Next hypothesis: a source form that makes dock's loop-carried segment
  longer than i's **without** adding a block — e.g. a different
  `dockOffsetVec` access that the scheduler sinks past the wormhole arm
  (a `const Vector &`/`Vector` copy hoisted before the loop was tried and
  is identical), or a reordering of the alien-orbit condition that moves
  the dock use later in the block.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5316
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch (not committed): `/dev/shm/w5316/` — `body_*.txt` variant bodies,
`genwrap.py`/`genwrap2.py`, `batch.sh`, `score1.py`, `regs.py`,
`orig_dis.txt`, `committed_dis.txt`, `h1_dis.txt`, `wrap_lia.txt`,
`committed_lia.txt`, `paa.txt` (wrap `-print-after-all`),
`paa_committed.txt` (committed `-print-after-all`).

## 7. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0, `.text 0x1337c8`.
`RATCHET PASS`.
