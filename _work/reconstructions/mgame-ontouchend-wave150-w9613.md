# MGame::OnTouchEnd — wave 150 (w9613): the `0x17a5ca` compare, backend LSR, and the `cmp #2` landing

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9613`,
base `7eed2966e` (wave 149 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9613`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 149) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4393 | **4392** |
| aligned (align_diff) | 904 | **904** |
| `.text` differing bytes | 86,449 | **86,448** |
| `.ARM.extab` differing bytes | 1,309 | **1,309** |
| `.rel.plt` differing bytes | 4 | 4 |

## 1. The residual

Original `0x17a5ca`:

```
17a5ca: f1b9 0f00   cmp.w r9, #0
17a5ce: d040        beq.n 17a652          ; skip if amount == 0
```

Base (wave 149) and every `!= 0` spelling:

```
17a5ca: f1b9 3fff   cmp.w r9, #-1
17a5ce: dc40        bgt.n 17a652          ; skip if amount >= 0
```

`r9` is the decrementing `amount` (`sub.w r9, r9, #1` at `0x17a5b6`).  The two tests are
semantically identical because `amount <= 0` after the loop, but the encodings differ.

## 2. Why `cmp #0` re-lays: the *second* LSR run

`-emit-llvm` shows the IR we want:

```
%245 = phi i32 [ 0, %242 ], [ %260, %259 ]
%260 = add nsw i32 %245, -1
%270 = icmp eq i32 %245, 0
```

But `-c` runs a second `Loop Strength Reduction` in the backend pipeline, after
`Canonicalize natural loops` / `LCSSA`.  `-mllvm -print-after-all` localises the flip:

```
IR Dump After Canonicalize natural loops:
  %260 = add nsw i32 %245, -1
  %270 = icmp eq i32 %245, 0
IR Dump After Loop Strength Reduction:
  %260 = add i32 %245, 1          ; incrementing
  %270 = icmp eq i32 %245, 0
```

LLVM 7 `LoopStrengthReduce.cpp` only builds an `LSRUse::ICmpZero` use for **equality**
icmps (`if (CI->isEquality())`).  The `-amount` formula then serves both the compare and
the `String(-amount)` negate, so LSR flips the induction variable to the incrementing
count.  `icmp slt amount, 0` is not equality and is left alone (hence the base `cmp #-1`).
`-mllvm -disable-lsr` gives exactly the original `sub.w r9,r9,#1; cmp.w r9,#0; beq`,
confirming the pre-LSR IR is correct; flags are not allowed, so the fix has to be in source.

## 3. What does *not* work

Every spelling that leaves a single `icmp eq amount, 0` at the backend LSR re-lays to
8060 B (raw 6590, aligned 1835):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `int n = count; if (n != 0)` | 8060 | 6590 | 1835 |
| `if (amount != 0)` / `if (count != 0)` / `if (count)` | 8060 | 6590 | 1835 |
| `if (count >= 1)` / `unsigned` / `long long` / `do{}while` / `for` | 8060 | 6590 | 1835 |
| `switch (amount) { case 0: ... default: ... }` | 8060 | 6590 | 1835 |
| `if (amount == 12345) __builtin_unreachable();` / `__builtin_assume` | 8060 | 6590 | 1835 |
| `if ((amount >> 1) != 0)` | 8080 | 4392 | 904 | (`cmp #2; bcc`) |
| `if ((amount & -2) != 0)` | 8080 | 4392 | 904 | (`cmp #2; bcc`) |
| `if ((amount | 1) == 1) {} else ...` | 8080 | 4392 | 904 | (`cmp #2; bcc`) |

The last three are equivalent to `amount != 0` for `amount <= 0`, but the compiler
canonicalises them to `icmp ugt amount, 1`, so LSR's equality path is bypassed.  They give
`cmp.w r9, #2; bcc`, not `cmp #0`.  `(amount >> 1)`/`(amount & -2)`/`(amount | 1)` all
regress `.ARM.extab` from 1,309 to 1,311 differing bytes.

`if (amount != 0 && amount != -1)` is 8084 B (`add r0, r9, #1; cmp r0, #2`).
Two independent IVs (`amount--; count++`) is 8212 B.

## 4. The landed shape

Keep the existing `count` variable and `String(count)`, and spell the guard as two
equality tests:

```cpp
int count = -amount;
if (amount != 0 && amount != 1) {
    report += String("\n") + String(count) + String("t ") +
              *Globals::gameText->getText(1274 + refined);
}
```

For `amount <= 0` this is exactly `amount != 0` (`amount == 1` is impossible), so it is
semantics-faithful.  InstCombine/backend combine the two equality tests into
`icmp ugt amount, 1` (non-equality), LSR declines to flip, and codegen emits:

```
17a5ca: f1b9 0f02   cmp.w r9, #2
17a5ce: d340        bcc.n 17a652
```

which is the same 6-byte size as the original and keeps the decrementing IV and the
`rsb r1, r9, #0` negate.  It is one raw byte closer than the base (`#2` vs `#-1`/`bgt`),
and it keeps `.ARM.extab` at 1,309.  The `String(-amount)` form of the same guard is
equally good on `.text` but moves `.ARM.extab` to 1,311; the `count` form is the one
landed.

## 5. Open question

A true `cmp #0; beq` needs the backend LSR to decline the `ICmpZero` rewrite while the
compare stays `icmp eq amount, 0`.  The only source-level lever found is to make the
compare non-equality (which changes the immediate to `#2`/`#-1`).  A source shape that
adds a free second use visible to LSR but removed before ISel was not found: `llvm.assume`,
`__builtin_unreachable`, dead branches and `switch` are all simplified before the backend
LSR.  A future wave could try a use that survives LSR but folds in `CodeGenPrepare`.

## 6. Reproduction

```
python3 /dev/shm/w9613/gen2.py and_ne0_ne1_countstr
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9613/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4392 / 8080
python3 /dev/shm/w9613/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 904
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9613/{fdiff.py,align_diff.py,gen.py,gen2.py,measure.sh,ir.sh,
MGame.base.cpp,lsr_src.cpp,allcg.txt}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
