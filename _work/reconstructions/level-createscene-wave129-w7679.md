# `Level::createScene` — wave-129 w7679: drop the `cap` split and reverse the
# retry-loop condition; ship-loop register map now matches the original

**Status:** landed, 2026-11-07. Worktree `wt/w7679`, base `003a9623a`.
Target `_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`, `createScene`
only). Gate re-measured green: **byte_exact 4506 / linked_exact 4506**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, symbol
parity 5370/5370.

| | pct | size | insns | own | capacity | spotUsed | status |
|---|---|---|---|---|---|---|---|
| before (wave-124) | 93.554563 | 2252 | 783/784 | r5 | r4 | r6 | sl |
| **after (this pass)** | **96.490108** | 2252 | 783/784 | **r4** | **r6** | **r4** | **sl** |

`.text` differing bytes: **85,441 → 85,315** (−126). `.ARM.extab`
1,427 → 1,436 (+9, the createScene LSDA shifts); `.ARM.exidx`,
`.rodata`, `.data`, `.bss`, `.rel.plt` stay exact, build-id 20.

## The change

Two edits, both in the ship-loop preamble:

```diff
-            unsigned short cap = (unsigned short) capacity;
             int shipCount = rnd->nextInt(capacity + 1);
...
-                int spot = rnd->nextInt(cap);
-                for (int tries = 100; tries != 0 && spotUsed[spot] != 0; tries--)
-                    spot = rnd->nextInt(cap);
+                int spot = rnd->nextInt(capacity);
+                for (int tries = 100; spotUsed[spot] != 0 && tries != 0; tries--)
+                    spot = rnd->nextInt(capacity);
```

The `cap` split is gone, so there is no `uxth` and `capacity` is one long
int live range. Reversing the `&&` operands in the retry loop is
semantically identical (`spot` is always in range) and does not change the
emitted retry loop at all — but it changes the IR order enough that the
greedy allocator no longer evicts `capacity` from `r6` for the `tries`
counter.

## Why this closes the wave-128 "2-cycle"

The wave-128 note described the residual as a `{status, capacity}` swap over
`{r6, sl}`. The mechanism, measured from
`/dev/shm/w7679/regalloc_nocap.txt`:

* In the plain no-`cap` shape the greedy allocator first puts `capacity`
  (`%27`, interval `[6816,12000)`, weight 3.66e-3) in `r6`, but the retry
  counter `%594` (`tries`, loop weight 6.2e-2) then evicts it at cascade 20
  and takes `r6`; `capacity` falls to `r10` (`sl`) and `status` takes `r6`.
* Reversing the `&&` operands changes the order of the retry-loop blocks
  enough that `tries` is allocated `r5` (as in the original) instead of
  `r6`. `capacity` keeps `r6`, `status` splits to `sl`, `ownHangar` gets
  `r4` and `spotUsed` reuses `r4` after the ownHangar spill.
* The landed `unsigned short cap` split was a *different* way to move
  `status` to `sl` (it shortens `capacity` so it is allocated late and
  takes `r4`), but it rotates `ownHangar` to `r5` / `spotUsed` to `r6` and
  adds the `uxth`.

## Remaining delta (96.49 %)

The instruction stream is now identical except for:

1. **Spill-slot numbering** — the same set of stack offsets is used, but
   `&__stack_chk_guard` and `&Globals::status` are swapped (`[sp,#36]` vs
   `[sp,#40]`), `shipCount`/`spots`/`kBarChildMeshes` rotate over
   `[sp,#28]/[sp,#32]/[sp,#36]`. This is `StackSlotColoring` order, not a
   register difference.
2. **`ldrd r0, fp, [sl, #4]`** — ours fuses `ship->player` and
   `ship->geometry` into one `ldrd`; the original has two separate
   `ldr.w`. Moving the `geo` declaration after `setToSleep()` splits the
   loads but reverts the whole ship-loop map to the wave-124 assignment
   (89.9 %), so the early `geo` declaration is load-bearing for the map.
3. **One extra `b.n` in the `spots` switch** (block layout of the
   bitmask-merged `case 0/1/3/8`), plus a trailing 2-byte `nop`.

## Variants measured this pass (all ≤ 96.490108 %)

* `cap` declared after the zero loop / after `spots` / before the ship
  loop / inside the loop / after `shipCount` / as `int` / as
  `unsigned int` / inline `(unsigned short)` cast — all byte-identical to
  the landed wave-124 shape (93.554563 %).
* no-`cap` plus `const`, `unsigned int capacity`, `short`/`char`/`int`
  ownHangar, `bool*`/`char*` spotUsed, `unsigned` race/tries/spot,
  if/else min, reversed min, `i` reuse, `geo` inline, zero loop after
  `spots`, `spots` before zero loop — all stay in the 2228 B no-`cap`
  basin (80.305927 %).
* no-`cap` + reversed `&&` but `geo` moved after `setToSleep()` /
  between the calls / inline — 89.3–89.9 %, map reverts to
  `own=r5 cap=r4 spot=r6`.
* `switch` case reorder (`8,3,1,0` or `default` first) or `if` chain —
  map reverts to wave-124 (93.55 %) or worse (76.9 %).
* retry condition spellings `spotUsed[spot] && tries != 0`,
  `... && tries`, `while (...)`, `... && tries > 0` — all 96.490108 %.

## Reproduction

Scratch is `/dev/shm/w7679`:

* `Level.cpp.base` — pristine wave-124 source;
* `Level.cpp.rev` — the landed wave-129 source;
* `fast.py` — unrounded single-function score + size/insns + register map;
* `batch.py` / `batch_rev.py` — apply `[[old,new],…]` to the base and score;
* `v4.json` … `v11.json` — the variant sets above;
* `regalloc_base.txt` / `regalloc_nocap.txt` — LLVM 7 `-debug-only=regalloc`
  dumps (assertions-enabled `llc` at `/dev/shm/wf110/llvm702build/bin/llc`).

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump $OBJDUMP --show _ZN5Level11createSceneEv
cmake --build cmake-build-match --target check -j6   # byte 4506 linked 4506
```
