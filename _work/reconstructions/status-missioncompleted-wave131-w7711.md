# `Status::missionCompleted` — wave 131 (w7711): the switch-edge weights are the
# only remaining cause; the case-166 `__builtin_expect` is what forces the
# body-158 tail merge, and a **single** `__builtin_expect` in an if-chain gives
# the required gentle (factor-2) source-order frequency gradient — but the fold
# splits the switch into two tables, so it cannot land at 1432 B

Status: **note-only** (no source change landed; gate kept at byte 4506 /
linked 4506, `.text` diff 85,379).  Worktree `wt/w7711`, base master
`70e451ffe` (wave-131 snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image
base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `70e451ffe`) |
|---|---:|
| function score | **71.0 %** (523 vs 525 normalised insns) |
| function size | 1432 B |
| gate byte_exact | 4506 |
| gate linked_exact | 4506 |
| data_exact | 843/843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |
| `.text` differing bytes | 85,379 |

## 0. Summary (new results this pass)

1. **Reproduced wave-131's `!prof` result on the current (wave-130) source.**
   Injecting `!prof` weights `default=1, A=3, B=2, low=4` into the optimised
   IR `switch i32 %52` gives the exact original body order and **1432 B**.
   The four residual body deltas on the current base are
   **158 −6, 166 +10, 157 −2, 168 −2** (sum 0).
2. **The −6 on 158 and +10 on 166 are one cause: `TailMerging` merges body
   158's tail into body 166's tail.**  Body 158 ends in the original with
   `ldr r6,[sp,#16]; beq.w latch; b.n return`; in the injected build it ends
   with `b.n <body-166-tail>`, and body 166 then has to keep `this` live
   across `getProductionGoodAmount` (`mov r1,r6; mov r6,r0; ldr r0,[r1,#400]`)
   instead of the original `ldr r0,[r6,#400]`.  The merge is forced by the
   **`__builtin_expect(atStation && …, 1)` in case 166**: removing that hint
   (or writing case 166 as `isInList(...) || hasEquipment(...)`) un-merges
   body 158 and shrinks body 166.  With the hint removed the `!prof` build is
   still **1432 B**, order exact, and the residual deltas drop to
   **166 +4, 160 −2, 169 −2**.
3. **A *single* `__builtin_expect` in an if-chain is the source-level
   frequency generator.**  `LowerExpectIntrinsic` is one-hot (2000/1), but
   `SimplifyCFG::FoldValueComparisonIntoPredecessors` scales the folded case
   weights by the successor's *total* weight.  With **one** expected
   comparison the scaling factor is 2, so the folded switch gets a gentle
   geometric profile `2^N, 2^(N-1), …, 1` **in chain order**.  With the chain
   in the original source order (167/174, low, A, B) this is exactly
   `A > B > default` and MBP places the bodies in source order — the first
   source-level mechanism found that produces the required ordering without
   PGO.  Model/repro: `/dev/shm/w7711/fold.py`.
4. **But the fold does not keep the original single switch.**  On the real
   TU the 27-arm if-chain folds into **two** switches (25 cases + a nested
   4-case switch for 168/169/171/172; `SafeToMergeTerminators` fails on a
   PHI in a common successor).  `findJumpTables` then emits a 7-entry table
   for 150..156 and a 40-entry table for 8..47, and the function is
   **1388 B** (or 1396 B with 165 moved after 172).  The single-expect
   if-chain is therefore a correct *frequency* model but not a landable
   *shape* at 1432 B.

## 1. `!prof` reproduction (current base)

```sh
# exact-flag optimised IR (-g0), then inject the switch weights
/dev/shm/w7711/ccll.sh src/game/mission/Status.cpp /dev/shm/w7711/base.ll
python3 /dev/shm/w7711/inject.py        # writes one.ll/one.o, prints size+order
python3 /dev/shm/w7711/order.py /dev/shm/w7711/one.o
```

`inject.py` inserts, in `SwitchInst` case order (source order),
`!9999 = !{!"branch_weights", i32 1, i32 3, i32 3, i32 4, i32 4, i32 4, i32 4,
i32 4, i32 3, …, i32 2, …}` (default first, then 167,174,8,0,11,13,14, A…, B…).
Result: `size 1432`, body order exactly
`167/174, 150 151 152 153 154 155 158 162 166 170 184 189,
156 157 160 163 164 165 168 169 171 172, default`.

## 2. Tail-merge experiment (new)

The four residual bodies are not a weight effect: sweeping the per-case
weights (158=2..5, 166=2..5, 157=3, 168=3, …) leaves all body offsets
unchanged (`sweepw.py`).  The merge is decided by `BranchFolder`'s
`ProfitableToMerge`, not by frequency.

Removing the case-166 hint (or the `||` rewrite) changes the injected layout
to:

```
size 1432
167/174 0xfe
150 0x1d8 151 0x1de 152 0x1e4 153 0x218 154 0x21e 155 0x236
158 0x248 (exact) 162 0x2a4 (exact) 166 0x30a (exact start)
170 0x372 (+4) 184 0x380 (+4) 189 0x3ba (+4) 156 0x3f6 (+4)
157 0x420 (+4) 160 0x452 (+2) 163 0x480 (+2) 164 0x49c (+2)
165 0x4a0 (+2) 168 0x4a8 (+2) 169 0x4c4 171 0x4c8 172 0x4e2
default 0x522
```

i.e. only `166 +4` (the `mov r1,r6; mov r6,r0` register-allocation artifact),
`160 −2`, `169 −2`.  The `||` form (`isInList(...) || hasEquipment(...)`)
gives the same result as deleting the hint, and is the more likely original
source shape, but **on its own (without the weights) it is neutral**: score
stays 71.0 % and `.text` diff goes 85,379 → 85,392 (bodies shift by +6 in
value order).  Not landed.

## 3. Single-expect if-chain (new hypothesis, measured)

`fold.py` models `FoldValueComparisonIntoPredecessors` for a linear chain.
For a chain with `__builtin_expect(cond,1)` on **only one** comparison the
resulting weights are `2^N, 2^(N-1), …, 1` in chain order (default 0/1) —
a factor-2 gradient, no 32-bit overflow.  `genchain.py firstA` emits the
real if-chain in source order with the expect only on case 150; the IR
switch weights are

```
default 1, 167..14 = 262275072, 150 = 262144000,
151 65536, 152 32768, …, 189 16, 156 8, 157 4, 160 2, 163 1,
164 0, 165 0, 168 0, 169 0, 171 0, 172 0
```

which is exactly the required `A > B > default` ordering.  But the fold
splits the switch (25 cases + nested 4-case switch) and `findJumpTables`
emits tables 150..156 and 8..47: **1388 B**.  Moving 165 after 172 gives
1396 B.  So the frequency profile is right but the shape is not the
original single 29-case switch.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506

/dev/shm/w7711/cc.sh   src/game/mission/Status.cpp /dev/shm/w7711/base.o
/dev/shm/w7711/ccll.sh src/game/mission/Status.cpp /dev/shm/w7711/base.ll
python3 /dev/shm/w7711/order.py  /dev/shm/w7711/base.o   # size + TBH body order
python3 /dev/shm/w7711/inject.py                         # !prof A=3/B=2/low=4
python3 /dev/shm/w7711/inject2.py                        # same, branch !prof stripped
python3 /dev/shm/w7711/fold.py                           # chain-fold weight model
python3 /dev/shm/w7711/genchain.py firstA                # single-expect if-chain
python3 /dev/shm/w7711/sweepw.py                         # per-case weight sweep
```

Scratch (not committed): `/dev/shm/w7711/` — `cc.sh`, `ccll.sh`, `order.py`,
`bodies.py`, `inject.py`, `inject2.py`, `fold.py`, `genchain.py`, `rmhint.py`,
`sweepw.py`, `base.ll`, `one.o`, `v_*.cpp`, `v_*.o`.

## 5. Open question

The exact original layout needs the 29-case switch **and** the A>B>low>default
weights.  The only source-level weight generator that produces the right
ordering is a single-`__builtin_expect` if-chain, but `SimplifyCFG` splits
it into two switches.  A shape that keeps one switch (e.g. the shared
167/174 and 0/11 bodies written so `SafeToMergeTerminators` does not split
at 165/168) would land the win; alternatively the original was PGO-built and
the weights are profile data.
