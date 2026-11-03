# `Gun::calcCharacterCollision` pass 2: anchor root cause (w3829)

Worker: w3829. Branch: `wave89-w3829`, based on master `5710f6d0`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch output under
`/dev/shm/w3829/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `5710f6d0`) | **4498 / 4498** | **82.7%** | 1984 B |
| after (this pass) | **4498 / 4498** | **82.7%** | 1984 B |

No code change. The tree is left exactly as HEAD. This note records why the two
wave-88 anchors are still needed and what the real remaining root cause is.

## 1. What the original actually does (Ghidra + Thumb)

Register assignment of the shipped body:

| value | original | HEAD (with anchors) |
|---|---|---|
| `this` | `r9` | `r9` |
| `enemy` (inner loop) | `r8` | `r8` (via `ldr r8,[sp,#208]`) |
| `enemy` (outer checks) | `r8` | **`r6`** |
| `i` | `fp`, spilled `[sp,#40]` | `r1`, spilled `[sp,#40]` |
| `i*3` | `r4` | `r4` |
| `mine` | `r5` | `r5` |
| `&enemy->radius` | **`sl`** (`add.w sl,r8,#64` @ `1530a6`) | **`r6`** |
| `&this->targetDir` | **spilled `[sp,#24]`** (`str r6,[sp,#24]` @ `153010`) | **`sl`** (`add.w sl,r9,#216` @ `152ffe`) |
| `&position` (`sp+192`) | cached in **`sl`** after the damage block (`add.w sl,sp,#192` @ `15329a`, `1532ce`) | rematerialised `add rX,sp,#192` at every use |

So the whole residual diff is a **register-allocation swap**: the original gives
`sl` to `&enemy->radius` / `&position` and spills `&this->targetDir`; HEAD gives
`sl` to `&this->targetDir` and puts `&enemy->radius` in `r6`.

That single swap cascades into every other difference:

* **Outer-loop enemy register.** Because `&targetDir` owns `sl`, HEAD's outer
  `enemy` load lands in `r6` (`ldr.w r6,[r0,r2,lsl#2]` @ `153020`) and is then
  copied to `[sp,#208]` for the inner loop. The original loads it straight into
  `r8` (`ldr.w r8,[r0,r2,lsl#2]` @ `15301a`). All ~10 outer-check uses
  (`mov r0,r6` / `ldrb r0,[r6,#92]` / `mov r0,r8` / `ldrb r0,[r8,#92]` ...)
  therefore mismatch.
* **`this->damage` hoist.** With `r6`/`sl` occupied differently the scheduler
  hoists `ldr r1,[r9,#96]` above the `enemy->egoFlag` test in HEAD
  (`1532ce`), so the egoFlag path uses `vmov s0,r1` instead of the original's
  `vldr s0,[r9,#96]` (`1532d4`), and the normal path reuses the hoisted `r1`
  instead of reloading it (`ldr.w r1,[r9,#96]` @ `153308`).
* **Two normal damage blocks instead of one.** Because the source keeps a
  `const int itemIndex` local, the owner-branch `else` uses `[sp,#12]`
  (`ldr r3,[sp,#12]` @ `1532c6`) while the final `else` uses `[r9,#88]`
  (`ldr r3,[r9,#88]` @ `153304`). The two blocks are therefore not identical
  and do not tail-merge; HEAD carries an extra 16-byte normal path at
  `1532bc`. The original has one shared block at `153304`
  (`ldr r3,[r9,#88]; ldr r1,[r9,#96]; ldrb r2,[r9,#249]; mov r0,r8; blx`).
  The `itemIndex` local is a *size* anchor: removing it merges the blocks and
  drops the function to 1972 B / 74.6%.
* **Inner-loop rotation.** The original is top-tested (`mov fp,#0` @
  `15305a`, header `15305e`, latch `add fp,#1; b 15305e` @ `15325a`). HEAD is
  bottom-tested (`movs r1,#0; b 153500` @ `153062`, header
  `1534fc..153506`). The `continue`s target the latch in both, so this is pure
  MachineBlockPlacement order.
* **`hitFlag` anchor.** The original's `playerGun` block is 4 instructions
  (`ldrb [r9,#240]; cbz; ldr [r9,#56]; strb [r0,#48]` @ `15331c..153326`).
  HEAD inserts `ldrb r0,[r8,#84]` between the `cbz` and the `ldr`. Removing it
  gives 1980 B / 79.4% (the 4-byte shift re-runs the allocator).

## 2. Experiments (official verifier, `autojunk=False`)

Baseline HEAD: **82.7 / 1984**.

| variant | pct | size |
|---|---:|---:|
| `this->itemIndex` in both normal paths (no local) | 74.6 | 1972 |
| same + `hitFlag` removed | 74.7 | 1968 |
| local `itemIndex` in both normal paths | 78.9 | 1984 |
| volatile `this->damage` in both normal paths | 82.6 | 1984 |
| volatile damage + no local | 76.0 | 1956 |
| volatile damage + no local + no hitFlag | 76.1 | 1952 |
| `int dmg;` single shared `enemy->damage(dmg,...)` | 75.5 | 1956 |
| inverted outer `else if (!egoFlag)` / inner `if (!(...))` | 82.7 | 1984 |
| swapped owner-branch/else order | 82.7 | 1984 |
| `egoFlag` read into a local | 78.3 | 1980 |
| `Player *volatile` slot + early reload (outer) | 55.2 | 1912 |
| slot + early reload + inner reload | 80.2 | 1984 |
| slot read in the outer checks (`enemySlot->...`) | 79.5 | 1984 |
| reload before the inner loop only | 54.2 | 1912 |
| double load (`candidate` outer, `enemy` inner) | 68.2 | 1988 |
| `Vector *td`/`Vector &td` hoisted before the loop | 74.8 | 1980 |
| local `Vector targetDir` in the scatter loop | 61.2 | 1960 |
| `Vector *pp = &position` used at every call site | 82.7 | 1984 |
| `Vector &positionRef` used at every call site | 82.7 | 1984 |
| `Level *level` local (used) | 55.5 | 1960 |
| `__builtin_expect` on playerGun | 82.7 | 1984 |
| `__builtin_expect` on owner / scatter / asteroid / emp | 75.9 / 60.2 / 58.0 / 77.1 | — |
| `hitFlag` anchor as 1/2/4-byte read of `field_54`, `egoFlag`, `hitSmall`, `damage`, `itemIndex` | 82.2–82.7 | 1984 |

No source shape tried beats 82.7 / 1984.

## 3. Most important remaining root cause

**`&this->targetDir` is kept in `sl` by HEAD but spilled to `[sp,#24]` by the
original.** That single allocator choice is what forces `enemy` out of `r8` in
the outer checks, forces `&enemy->radius` into `r6`, prevents `&position` from
being cached in `sl`, and (through the extra pressure) makes the scheduler hoist
`this->damage` above the `egoFlag` branch. The `itemIndex` local and the
`hitFlag` read are only compensating size/order anchors for that.

The original materialises `&targetDir` for free with the post-increment store
idiom `mov r6,r9; str.w r2,[r6],#216; str r6,[sp,#24]` (store
`lastHitKIPlayer=0` + compute `this+0xd8`) and then spills it. HEAD does the
same store non-post-increment (`str.w r2,[r9]`) and keeps `this+0xd8` in `sl`.
Reproducing the spill without changing the rest of the allocation is the open
problem; every source-level nudge tried (moving the `itemIndex` local,
`Vector *td`/`Vector &td` locals, `Level *level`, scatter-loop rewrites,
`__builtin_expect` hints) perturbs the whole allocation and loses more than it
gains.
