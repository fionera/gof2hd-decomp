# `Gun::calcCharacterCollision` wave 117 (w6515): the residual is one greedy-RA
# eviction *ordering* plus the MBP loop rotation; no source shape beats 93.3846 %

Worker: w6515. Branch: `wave117-w6515`, base master `c22678d64`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w6515/`.

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `c22678d64`) | **4501 / 4501** | **93.3846** (report 93.4) | 607/650 | 1984 |
| after this pass | **4501 / 4501** | **93.3846** | 607/650 | 1984 |

No code change: this pass is analysis only. `check` after: byte 4501/4524,
linked 4501/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, `RATCHET PASS`. 27 ELF sections, `.ARM.extab 0x26298`, file size
2,187,328 B — unchanged.

## 1. The residual is exactly two compiler decisions

`tools/verify` reconcile diff of the 43 mismatched instructions (86 diff lines):

| # | cluster | insns | root cause |
|---|---|---:|---|
| 1 | `i*3` in `r5`, shipped `r4` | 9 (77, 80, 86, 108, 153, 392, 536, 541, 551) | greedy-RA eviction *order* |
| 2 | scatter temps swapped: shipped `r4=sp+44` (playerPosition) / `r5=sp+104` (delta), ours reversed | 2 (110, 114) | follows from #1 |
| 3 | `isAsteroid` join reloads | 2 (178, 242) + 1 shift | follows from #1 |
| 4 | Matrix block re-materialises `add r0/r1/r4,sp,#104` instead of reusing `r5` | 5 (356–371) | follows from #1 |
| 5 | inner `k`-loop rotation | ~10 (395–457) | `MachineBlockPlacement::rotateLoop` bails |
| 6 | branch/literal offsets | rest | address shifts from #1/#5 |

The register map is otherwise identical to the shipped binary:

| value | shipped | ours |
|---|---|---|
| `this` | r9 | r9 |
| `enemy` | r8 | r8 |
| `i` (outer loop) | fp | fp |
| `td` (`&this->targetDir`) | spilled `[sp,#24]` | spilled `[sp,#24]` |
| `i*3` | **r4** | **r5** |
| `&playerPosition` / `&delta` | **r5 / r4** | **r4 / r5** |

Verified by raw objdump at `0x15305a`–`0x15309e` and `0x1530c8`–`0x15315c`:
both builds emit `mov.w fp,#0; ldr.w r0,[r9,#8]; cmp fp,r0` for the outer
loop, then `add.w rX, fp, fp, lsl #1` with `X = r4` (shipped) vs `r5` (ours).

## 2. Cluster 1 mechanism: the `this` eviction fires *after* `i*3` is allocated

From the greedy-RA trace (`llc -debug-only=regalloc`, function section; the
real clang build reproduces it with one slot of noise), in priority order
(`Prio = (1<<31) | (1<<30 if hinted) | (1<<29) + size`, larger first):

```
assigning %56 to $r4   ; this,      size 16616, prio 0xE0000000+16616 (hinted)
assigning %11 to $r5   ; td,        size 12174, prio 0xA0000000+12174
assigning %98 to $r9   ; i*3,       size  9746, evicts %151 (w 4.6e-3)
...
assigning %403 to $r5  ; one-block local, weight 1.88e-1, evicts %11
selectOrSplit %11 ... hints: $r0 $r0 $r0 $r4
assigning %11 to $r9   ; r4 still held by %56 -> takes r9, evicts %98
assigning %98 to $r5   ; i*3 re-queued -> r5
...
should evict: %56 ... weight 2.80e-2
assigning %455 to $r4  ; split child of the spilled stack addr %306 (w 2.8e-1)
assigning %56 to $r9   ; this re-queued -> r9, evicts %11
assigning %11 to $r8   ; td -> r8
```

So `this = r9` is a **late** eviction: `%455` is a single-block local-range
spill child of `%306` (the `Vector` sret address used by the k-loop's
`Player::getPosition()`), and *locals are allocated after all global ranges*.
`i*3` (`%98`) is a global range (live `0x15306c` → `0x153436`), so it is
allocated while `%56` still owns `r4` and therefore can only get `r5`.

To move `i*3` to `r4` the `this` eviction must happen **before** `%98` is
allocated — but the only thing that evicts `this` is a local-range spill
child, which by construction is enqueued after every global. That is why
every source spelling that keeps `this = r9` also keeps `i*3 = r5`.

## 3. Cluster 5 mechanism: rotation needs a hot `distance < magnitude` edge

`-Oz` disables the IR `loop-rotate` pass (`createLoopRotatePass(0)`), so the
shipped `movs r5,#0; b cond; body; adds r5,#1; cond: cmp; bcc body` layout is
produced by `MachineBlockPlacement::rotateLoop`, not by the IR pass.

With `__builtin_expect(distance < magnitude, 0)` the if-body edge is 0.05 %, so
`selectBestSuccessor(bb.90)` picks the latch (99.95 %); the cold if-body is
appended by `getFirstUnplacedBlock`:

```
chain = [bb.89(cond), bb.90(body), bb.95(latch), bb.91..bb.94(if-body)]
Bottom = bb.94, Top = bb.89, bb.94 -> bb.89 is false
rotateLoop bails on: ExitingBB->isSuccessor(NextBlockInChain) && !Bottom->isSuccessor(Top)
```

With a plain `if` (50/50) the chain is `[cond, body, if-body…, latch]`, ends
with the latch, and `rotateLoop` moves the header to the bottom — exactly the
shipped layout. But the plain `if` also removes the `%306` spill, so no `%455`
exists, `this` is never evicted from `r4`, and the whole register map shifts
(`this=r4`, `enemy=r6`, frame 1908 B, 64.54 %).

The two decisions are therefore coupled through the same spill: **the cold
branch is what creates the spill that fixes `this = r9`.**

## 4. Measurements (official verifier path; `link_so` + `tools/verify`)

Scratch harnesses: `qscore.sh` (`link_so` + single-function score + register
probe), `regof.py` (`this`/`enemy`/`i*3` register), `ndiff.py`
(reconcile diff), `sweep*.py` (variant generators). All numbers are
`pct / size`; `this/enemy/i3` are the raw objdump registers.

### 4.1 Hot body (plain `if`) — block layout right, RA wrong

| variant | pct | size | this | enemy | i3 |
|---|---:|---:|---|---|---|
| plain `if` | 64.5412 | 1908 | r4 | r6 | – |
| `int k` | 64.5412 | 1908 | r4 | r6 | – |
| `Player *enemy` hoisted out of outer loop | 64.5412 | 1908 | r4 | r6 | – |
| `enemies->data()[k]` / `data()[e]` | 64.5412 | 1908 | r4 | r6 | – |
| `Vector &td` / `td` local to scatter / `this->targetDir` direct | 64.5412 | 1908 | r4 | r6 | – |
| `Vector ep = enemy->getPosition()` | 64.5412 | 1908 | r4 | r6 | – |
| `while (k < size)` | 64.5412 | 1908 | r4 | r6 | – |
| `k++` / `const bool scatter` / magnitude local | 64.5412 | 1908 | r4 | r6 | – |
| `float distance` | 63.5 | 1896 | r4 | r6 | – |
| extra stack `Vector` in k-loop (`ep2`) | 62.0 | 1960 | fp | – | – |
| `for (k=0, n=size(); k<n; ++k)` | 67.9 | 1956 | **r9** | r6 | – |
| `Array<Player*>* enemies` hoisted | 49.7 | 1924 | r4 | – | – |

`for (k=0, n=size(); ...)` is the only hot shape that reaches `this = r9`, but
hoisting `size()` changes the loop condition (the shipped code reloads
`enemies->size()` every iteration) and the frame grows to 1956 B.

### 4.2 Cold body (`expect(distance < magnitude, 0)`) — RA right, layout wrong

All of the following are **93.3846 / 1984, this=r9 enemy=r8 i3=r5**
(codegen-identical to HEAD):

`Vector &td`; `td` declared inside the scatter block; `this->targetDir` used
directly; `hitPositions[i]`; `positions[i]`; `velocities[i]`;
`hitPositions.data()[i]` vs `hitFlags[i]` order kept; `lastHitKIPlayer` after
`td`; `int radius; if (useCustomRadius) …`; `const Vector enemyPosition`;
`const Vector position`; `int k`; `int e`; both `int`; `enemies->data()[k]`;
`enemies->data()[e]`; `float fdistance`; `this->enemies->size() > k`;
`(unsigned) this->enemies->size()`; `k += 1`; `Vector ep = …; *td = ep;`;
`*td = (*this->enemies)[k]->getPosition()`; `Player *e2 = …; enemy = e2;`;
`int mag = this->magnitude` in the falloff; `int dmg = this->damage`;
`(unsigned) distance < (unsigned) this->magnitude`; `bool hit = distance < …`;
`__builtin_expect(cond,0) != 0`; `if (expect(distance >= magnitude,1)) continue;`;
`if (!expect(distance < magnitude,0)) continue;`; `expect(k < size,{0,1})` on
the loop condition is *not* identical (54.9 / 62.1).

Slightly worse: `hitPositions` before `hitFlags` 92.8; `unsigned distance`
93.1; `unsigned idx = i*3` 74.5/1996; `for (k=0,n=size(); …)` 67.9/1960;
`Level *level` local 66.5/1928; `Vector &position` 55.0/1912;
`Array<Player*>* const enemies` 45.4/1964.

Declaration-order sweep of the four inner-loop locals (`position`, `velocity`,
`enemyPosition`, `radius`), 24 permutations: `pver` (HEAD) is the unique best
at 93.3846; next `pvre` 92.8, `prve` 92.2, `pevr` 90.2, `perv` 90.0,
`prev` 89.5, `vper` 89.8, `vpre` 89.2, `vepr`/`verp` 87.8, `vrpe` 88.6,
`vrep` 87.4, `epvr` 87.1, `eprv` 86.6, `evpr` 86.9, `evrp` 86.6; every
`radius`-first order drops to 67–74 and loses `enemy = r8`.

### 4.3 Full `if`-condition `__builtin_expect` sweep on the cold body

39 conditions × {0,1}. Only two move `i*3` to `r4`, and both wreck the layout:

| site | value | pct | size | this | enemy | i3 |
|---|---|---:|---:|---|---|---|
| HEAD (distance) | 0 | 93.3846 | 1984 | r9 | r8 | r5 |
| `if (mine) radius *= 5` | 0 | 88.8376 | 1984 | r9 | r8 | r6 |
| `if (mine) radius *= 5` | 1 | 73.3538 | 1996 | r9 | r8 | **r4** |
| `if (scatter)` (radius, `0x1530c2`) | 1 | 80.5000 | 1980 | r9 | r6 | **r4** |
| `if (scatter)` (radius, `0x1530c2`) | 0 | 32.2 | 1896 | r4 | r6 | – |
| `if (scatter)` (k-loop, `0x15341e`) | 0 | 56.6 | 1924 | sl | r5 | r9 |
| `if (scatter)` (k-loop, `0x15341e`) | 1 | 61.6 | 1888 | r4 | r6 | – |
| `if (!enemy->isActive())` | 0/1 | 61.5 / 45.8 | 1908/1876 | r4 | r6 | – |
| `if (enemy->getHitpoints() < 1)` | 0/1 | 61.8 / 47.0 | 1908/1876 | r4 | r6 | – |
| `if (k < size)` (loop cond) | 0/1 | 62.1 / 54.9 | 1944/1948 | r9/sl | r5/r6 | sl/– |
| everything else | 0/1 | ≤ 64.5 | 1908 | r4 | r6 | – |

Pairwise combinations:

| variant | pct | size | this | enemy | i3 |
|---|---:|---:|---|---|---|
| `mine`(0) + distance `expect`(0) | 88.8376 | 1984 | r9 | r8 | r6 |
| `mine`(0) + plain distance | 82.6 | 1980 | r9 | r6 | r4 |
| `mine`(1) + distance `expect`(0) | 73.3538 | 1996 | r9 | r8 | r4 |
| `mine`(1) + plain distance | 32.6 | 1896 | r4 | r6 | – |

`mine`(1) is the only shape seen that keeps `this = r9` **and** `enemy = r8`
**and** `i*3 = r4`, but it costs 20 points because it re-lays-out the whole
`if (mine) … else if (scatter) …` region (the shipped code is
`ldr; cbz; add.w fp,fp,fp,lsl #2; b end` — a cold `mine` body).

## 5. Conclusion

The residual is not a source-semantics difference. It is two coupled compiler
decisions:

1. `i*3 = r4` requires the `this` eviction from `r4` to fire before `i*3` is
   allocated, but the evictor (`%455`) is a local-range spill child of the
   k-loop sret stack address and is enqueued after every global range. Every
   source spelling that keeps `this = r9` keeps `i*3 = r5`; the only shapes
   that reach `i*3 = r4` (`mine`(0/1), `scatter`(1)) re-lay-out a whole
   branch region and score 73–89 %.
2. `MachineBlockPlacement::rotateLoop` needs the `distance < magnitude` edge
   to be ≥ 50 % (plain `if`), but the cold edge is exactly what creates the
   `%306` spill whose child evicts `this` to `r9`. Making the edge hot removes
   the spill and demotes `this` to `r4` (1908 B, 64.54 %).

Best measured variant remains the HEAD body at **93.3846 % / 1984 B**.
≈ 130 variants were measured this wave on top of the ≈ 400 from waves
106–116; none exceeded 93.3846.

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

Register probe: `arm-linux-androideabi-objdump -d cmake-build-match/libgof2hdaa.so`
then read `mov.w fp,#0` (outer index), `add.w rX, fp, fp, lsl #1` (`i*3`),
`ldr.w rY, [r0, r2, lsl #2]` (`enemy`) and the first `mov rZ, r0` (`this`).
