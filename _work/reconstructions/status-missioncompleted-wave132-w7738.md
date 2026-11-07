# `Status::missionCompleted` — wave 132 (w7738): the single-expect if-chain
# reproduces the exact A-run/B-run body order but its geometric weights move
# the shared compare tail and the loop latch, so the function is 1352–1368 B,
# never 1432 B; the switch source + `!prof` weights is still the only 1432 B
# exact-order build

Status: **note-only** (no source change landed; gate kept at byte 4506 /
linked 4506, `.text` diff 85,244).  Worktree `wt/w7738`, base master
`144fc16b0` (wave-132 snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`,
image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `144fc16b0`) |
|---|---:|
| function score | **71.0 %** (523 vs 525 normalised insns) |
| function size | 1432 B |
| gate byte_exact | 4506 |
| gate linked_exact | 4506 |
| data_exact | 843/843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |
| `.text` differing bytes | 85,244 |

## 0. Summary (new results this pass)

1. **The full if-chain *does* fold into one 40-entry high table** (plus the
   original 7-entry low table) when the low cases 8/0/11/13/14 are kept in
   the chain.  w7711's two-table split was an artefact of the chain shape;
   with the case groups in the original source order the fold is a perfect
   no-op (plain if-chain `.o` is byte-identical to the switch `.o`).
2. **A single `__builtin_expect(...,1)` on any case makes the folded switch
   weights monotonic in chain order**, so the high-table body order becomes
   the chain order.  With the chain in the original source order
   (167/174, low, A, B) the body order is **exactly the original**
   (167/174, 150 151 152 153 154 155 158 162 166 170 184 189,
   156 157 160 163 164 165 168 169 171 172).  This confirms w7711's
   frequency model.
3. **But the folded build is never 1432 B.**  Sweeping the single expect over
   all 27 case values gives sizes 1352–1436; the exact-order ones are
   1352/1356/1364/1368 B.  The reason is not the body order: the fold's
   geometric weights (2^N with a 2000× step at the expect) change
   `SimplifyCFG`/`BranchFolder`'s tail-merging decision.  The shared
   `cmp; blt latch; b setWon` tail is placed *immediately after the
   167/174 body* (at 0x108 in the fold build) instead of at the original
   0x22e after the A-run, and the loop latch (switch default) is placed at
   0x124/0x12a instead of 0x522.  All branches to the latch therefore use
   16-bit forms and the function shrinks by ~76 B.
4. **The switch source + `!prof` weights is still the only 1432 B exact-order
   build.**  Re-injecting `default=1, A=3, B=2, low=4` (A includes 167/174)
   into the *current* optimised IR reproduces the original body order and
   **1432 B** exactly (default at 0x522).  The same weights injected into the
   *if-chain* IR (unsorted switch) also give 1432 B — so the case order is
   irrelevant; only the weight *magnitudes* matter.  The fold's weights are
   too steep (factor 2 per case, 2000× at the expect, overflow to negative
   `i32`), and that is what moves the shared tail.
5. **Tested shapes that still fail at 1432 B** (all measured on the real TU):
   * full if-chain, single expect on each of the 27 cases (sizes above);
   * full if-chain with the case-166 body `__builtin_expect` removed
     (same sizes);
   * high-only if-chain + separate low `switch` (`highfirst`): only
     expect-158 gives 1432 B, body order
     `152 153 154 155 151 150 158 …` (first six wrong); expect-166 gives
     1440 B;
   * low `switch` first + high-only if-chain (`lowfirst`): expect-168/169
     give 1432 B but the high bodies stay value-order with only
     168/169/171/172 moved to the end;
   * permuting the A-run chain order (150…189) and the expect position:
     no permutation found that gives 1432 B + exact order.
   * every 1432 B variant is byte-wise *worse* than the baseline
     (487–507 structural diff lines vs 333 for the plain value-order
     switch), because the tail-merge changes cost more instructions than
     the body reorder saves.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506

# exact-flag compile of one TU (same flags as the build)
/dev/shm/w7738/cc.sh   src/game/mission/Status.cpp /dev/shm/w7738/base.o
/dev/shm/w7738/ccll.sh src/game/mission/Status.cpp /dev/shm/w7738/base.ll

# body-order decoder (TBH -> case body offsets)
python3 /dev/shm/w7738/order2.py /dev/shm/w7738/base.o

# if-chain generator: mkif.py <mode> <expect-case|all|set:...>
python3 /dev/shm/w7738/mkif.py plain 150      # full chain, expect on 150
python3 /dev/shm/w7738/mkif.py plain all      # expect on every arm
python3 /dev/shm/w7738/mkif.py plain set:150,151,152,153,154,155,158,162,166,170,184,189

# high/low split generator: mksplit.py <highfirst|lowfirst> <expect> [order]
python3 /dev/shm/w7738/mksplit.py highfirst 158
python3 /dev/shm/w7738/mksplit.py lowfirst 168

# !prof injection into the switch IR (proves the weight mechanism)
python3 /dev/shm/w7738/inject.py    # writes inj.ll with A=3/B=2/low=4/default=1
# then compile inj.ll with the NDK clang++ backend and order2.py it

# expect sweep
python3 /dev/shm/w7738/sweep_expect.py
python3 /dev/shm/w7738/sweep_split.py
```

Scratch (not committed): `/dev/shm/w7738/` — `cc.sh`, `ccll.sh`, `order.py`,
`order2.py`, `mkif.py`, `mksplit.py`, `mkorder.py`, `inject.py`,
`inject2.py`, `sweep_expect.py`, `sweep_split.py`, `check_split.py`,
`check_order.py`, `search.py`, `base.ll`, `plain.ll`, `inj.ll`, `*.o`.

## 2. Key measurements

### 2a. Full if-chain, single expect, original chain order

| expect case | size | high-table body order |
|---|---:|---|
| none | 1432 | value order (same as switch) |
| 13 / 14 | 1352 | **exact** |
| 150 | 1356 | **exact** |
| 151 | 1364 | **exact** |
| 153 | 1368 | **exact** |
| 189 | 1356 | **exact** |
| 166 | 1388 | 150 152 153 162 151 154 155 158 166 170 184 189 156 … |
| 158 | 1436 | 155 151 150 152 153 154 158 162 … |
| 167 / 174 / 0 / 11 | 1432 | value order (weights have no effect) |

### 2b. `!prof` injection (switch IR), A=3 B=2 low=4 default=1

```
size 0x598 (1432 B)
167/174 0xfe
150 0x1d8 151 0x1de 152 0x1e4 153 0x218 154 0x21e 155 0x236
158 0x248 162 0x29e 166 0x304 170 0x372 184 0x380 189 0x3ba
156 0x3f6 157 0x420 160 0x452 163 0x480 164 0x49c 165 0x4a0
168 0x4a8 169 0x4c4 171 0x4c8 172 0x4e2
default 0x522
```

The same weights injected into the *if-chain* IR (unsorted switch, chain
order) also give 1432 B and the same order — proving the case order is not
the issue, only the weight magnitudes.

### 2c. Why the fold shrinks the function

Original shared compare tail (0x22e, after the A-run):

```
ac166: cmp r5, r0
ac168: blt.w ac45a        ; <  -> loop latch (0x522)
ac16c: b.n   ac4a8        ; >= -> setWon tail
```

Fold build (0x108, immediately after 167/174):

```
108: cmp r5, r0
10a: bge.w 0x522          ; >= -> setWon tail
10e: b.n   0x12a          ; <  -> loop latch (0x12a, adjacent)
```

The latch is adjacent to the shared tail, so every `break` branch narrows
from `b.w`/`blt.w` to 16-bit forms.  That is the entire ~76 B delta.

## 3. Open question

The original needs the 29-case switch *and* the A>B>low>default weights.
The only source-level weight generator in clang 7.0.2 is the if-chain fold,
and its weights are geometric (2^N, 2000× step).  A source construct that
emits *gentle* multi-case switch weights (e.g. `__builtin_expect_with_probability`,
which is clang 11+) would land the win; with clang 7.0.2 the fold always
moves the shared tail and the latch.  Alternatively the original was
PGO-built (`-fprofile-use`) and the weights are profile data.
