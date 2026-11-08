# MGame::OnTouchEnd — wave 153 (w9913): the mining guard is an LSR cost tie, and no free `+amount` use exists

Status: analysis note, **no source change landed**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9913`, base `4a6104a43` (wave 152 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w9913`.
Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 152) | this wave |
|---|---:|---:|
| size | 8080 | 8080 |
| raw same-address diff | 4391 | **4391** |
| aligned (align_diff) | 903 | **903** |
| guard | `cmp.w r9,#0 ; bgt.n` | unchanged |

## 1. Mission hypotheses (all three falsified)

The wave-152 note asked for `count > 0` with an unsigned `count`, a `do/while`, or a
separate `if (count == 0) goto` that keeps the decrementing IV.  All three leave an
`icmp ne`/`icmp eq` on the IV, so the backend LSR `ICmpZero` path still fires and the
whole function re-lays to 8060 B (raw 6590, aligned 1835):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `unsigned count = -amount; if (count > 0)` | 8060 | 6590 | 1835 |
| `do { if (count == 0) break; ... } while (false)` | 8060 | 6590 | 1835 |
| `if (count == 0) goto skip; report...; skip:;` | 8060 | 6590 | 1835 |
| `if (amount == 0) goto skip; ...` | 8060 | 6590 | 1835 |

## 2. Why: the LSR cost model, exactly

The backend `LoopStrengthReduce` (LLVM 7.0.2, `lib/Transforms/Scalar/LoopStrengthReduce.cpp`)
builds an `LSRUse::ICmpZero` for the equality compare.  Both the compare and the
`String(-amount)` negate have SCEV `-amount`:

* ICmpZero use: `S = N - S = 0 - amount = -amount`
* negate (`mul i32 %amount, -1`) use: `S = -amount`

`Cost::isLess` compares `Insns` first only when `-lsr-insns-cost` was *explicitly* passed
(`InsnsCost.getNumOccurrences() > 0`); by default it uses
`TargetTransformInfoImpl::isLSRCostLess`, which compares
`(NumRegs, AddRecCost, NumIVMuls, NumBaseAdds, ScaleCost, ImmCost, SetupCost)`.

For the two candidate formulae:

| | amount formula (`-1*amount`) | count formula (`1*count`) |
|---|---|---|
| NumRegs | 1 | 1 |
| AddRecCost | 1 | 1 |
| NumIVMuls | 0 | 0 |
| NumBaseAdds | 0 | 0 |
| **ScaleCost** | **1** (Basic use, scale -1 ⇒ `getScalingFactorCost` returns `F.Scale != 1`) | **0** (scale +1) |

So the count formula wins by one `ScaleCost` point and the IV flips.  The non-equality
base (`icmp slt amount, 1`, `S = +amount`) ties — amount scale +1 cost 0, negate scale -1
cost 1 ⇒ 1 vs count 1 — and the first (amount) formula wins, which is why the landed
`count >= 0` keeps the decrementing IV.

## 3. New hypothesis: add a free `+amount` Basic use (tie-break) — confirmed but costly

If a Basic use with `S = +amount` exists, the amount formula's ScaleCost drops to 0 and
the count formula's rises to 1, so the tie flips back to the amount formula.  Adding
`&& amount >= -1000000` to the loop condition does exactly that:

```cpp
while (ship->hasCargo(ore, fee) && amount >= -1000000) { ...; amount--; }
if (amount != 0) { report += String("\n") + String(-amount) + ...; }
```

Codegen at the guard becomes the original `cmp.w r9,#0 ; beq.n` with the decrementing IV
and the `rsb` — the tie hypothesis is confirmed.  But the extra condition is emitted
branchlessly inside the loop (`eor/cmp/mov/it/movlt/orrs/beq`, 7 instructions instead of
3), which re-lays the whole function: **size 8080, raw 7520, aligned 2248** (worse than
the base's 4391/903).

Variants of the same idea that also fail (all re-lay or regress):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `while (amount >= -1000000 && hasCargo)` | 8072 | 7507 | 2358 |
| `if (amount != 0) { if (amount >= -1000000) { report... } }` | 8084 | 7488 | 2238 |
| `if (amount <= -1000000) break;` in the body | 8056 | 7384 | 2764 |
| `refinedAny = (amount < 0)` after `amount--` | 8060 | 6590 | 1835 (compare folded before LSR) |
| `refinedAny = (amount != 0)` after `amount--` | 8052 | 7043 | 1247 |

## 4. Other shapes measured (all 8060 flip unless noted)

`count != 0`, `count`, `!count` (8076), `count != 0 && count != ±1/2/3`, `count > 0`
signed (8080, `cmp #-1; bgt`, 4393/904), `count >= 1` (same), `count >= 0` (landed,
4391/903), `count > -1` (4391/903), `count <= -1`/`count < 0` (8060), `count != 0 && ...`
all fold to a non-equality, `(count|1) != 1`, `(count & -2) != 0`, `(count >> 1) != 0`,
`(count & 0x7fffffff) != 0`, `(count & 0x80000000) != 0`, `(count & 0x8000) != 0`,
`(count & 0xffff) != 0`, `unsigned amount` + `String(-(int)amount)`, `unsigned count`,
`(int)(0UL-(unsigned long)amount)`, `(int)(0LL-(long long)amount)`,
`(int)(-(unsigned)amount)`, `amount * -1`, `~amount + 1`, `0 - amount`,
`__builtin_expect(count != 0, 1)` (8004), `__builtin_expect(amount != 0, 1)` (8004),
`-fwrapv` / `-fno-strict-overflow` (8060), `-mllvm -lsr-insns-cost`,
`-mllvm -lsr-exp-narrow`, `-mllvm -lsr-filter-same-scaled-reg=false`,
`-mllvm -enable-lsr-phielim=false` (all 8060).

## 5. Open questions

1. The tie needs a `+amount` Basic use that costs zero instructions.  No such source
   construct was found: every surviving compare adds at least one instruction and
   re-lays the function.  A future wave could look for a use that survives to the
   backend LSR but is folded by `CodeGenPrepare`/ISel (e.g. a load the compiler can
   prove dead, or an `icmp` on a `select` that folds later).
2. Independent of the mining guard, the function still differs structurally at
   `0x17ab36` (`orig cmp r4,#0; beq` vs ours `cmp r4,#1; bne`) and `0x17add4`
   (`orig cmp r0,#0; beq` vs ours `cmp r0,#1; bne`), plus a 20-instruction block at
   `0x17aeaa`.  These are the `failed == 1` / `hasWon` region, not the mining guard, and
   they dominate the 903 aligned residual.  `if (failed)` is known to shrink the
   function to 8076 (wave-149 note), so the `cmp #0; beq` form needs a size-neutral
   spelling — a separate target.

## 6. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9913/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4391 / 8080
python3 /dev/shm/w9913/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 903
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9913/{fdiff.py,align_diff.py,idiff.py,firstdiff.py,gen.py,
measure.sh,compile.sh,MGame.base.cpp,variants/}` plus the LLVM 7.0.2
`LoopStrengthReduce.cpp`, `IVUsers.cpp`, `InstructionSimplify.cpp`,
`TargetTransformInfoImpl.h` fetched for the cost analysis.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## 7. Wave-153 continuation: more mining-guard spellings (all falsified)

Continuation of the same worktree/session.  Base unchanged (`count >= 0`,
4391/903/8080).  New hypotheses measured on the exact base:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `__builtin_assume(amount >= -1000000)` before `if (amount != 0)` | 8060 | 6024 | 1032 |
| `__builtin_assume(amount <= 0)` | 8060 | 6024 | 1032 |
| `__builtin_expect(amount, 0) != 0` | 8076 | 7353 | 2347 |
| `(int)__builtin_expect(amount, 0) != 0` | 8076 | 7353 | 2347 |
| `(void)__builtin_expect(amount, 0);` (dead) | 8060 | 6590 | 1835 |
| `amount != 0 && (amount & 0x7fffffff) != 0` | 8068 | 6506 | 1058 |
| `(amount & 0x7fffffff) != 0` alone | 8032 | 7525 | 2294 |
| `(amount \| 0x80000000) != 0` | 8060 | 6589 | 1837 |
| `volatile int dummy = (amount < 0) ? amount : 0` | 8000 | 7423 | 3609 |
| `amount != 0 && amount <= 0` / `amount < 1` | 8080 | 4393 | 904 |
| `amount != 0 && amount >= 0` / `amount > -1` | 8060 | 6026 | 1034 |
| `amount != 0 && (long long)amount != 0` | 8060 | 6590 | 1835 |
| `amount != 0 && -amount != 0` / `(amount == amount)` | 8060 | 6590 | 1835 |
| `switch (amount) { case 0: goto skip; default: ... }` | 8060 | 6590 | 1835 |
| `String(amount / -1)` / `amount % -1 - amount` / `(amount ^ -1) + 1` | 8060 | 6590 | 1835 |
| `String((int)(0u - (unsigned)amount))` | 8060 | 6590 | 1835 |
| `count >= 0 && amount == 0` | 8076 | 6528 | 2058 |
| `count >= 0 \|\| amount == 0` | 8080 | **4391** | **903** (folds to base) |
| `while (__builtin_expect(hasCargo, 1))` + `amount != 0` | 8076 | 7077 | 2141 |
| `while (__builtin_expect(hasCargo, 0))` + `amount != 0` | 8056 | 7512 | 2746 |
| `while (__builtin_expect(hasCargo, 1))` + `count >= 0` | 8072 | 6855 | 1060 |
| `while (__builtin_expect(hasCargo, 0))` + `count >= 0` | 8032 | 7511 | 2698 |
| `__builtin_expect(amount != 0, 0)` | 8076 | 7353 | 2347 |
| `(amount & 0x4000) != 0` | 8080 | 4394 | 903 |
| `(amount & 0xc000) != 0` / `(amount & 0xff00) != 0` | 8060 | 6028 | 1033 |
| `(amount & 0x80000000) != 0` | 8080 | 4393 | 904 |
| `(amount & 0xffff8000) != 0` | 8080 | 4393 | 904 |
| `(amount & 0xffff) != 0` / `(amount & 0x7fff) != 0` | 8028 | 7284 | 2281 |

The `llvm.assume` operand is *ephemeral* (`CodeMetrics::collectEphemeralValues`
collects `llvm.assume` operands), so it is skipped by IVUsers and does not
provide the free `+amount` Basic use.  `llvm.expect` is not skipped but its
`i64` form perturbs the IR enough to shrink/re-lay the function.

The only 8080-byte spelling with the original `beq` and decrementing IV remains
`(amount & 0x8000) != 0` (`tst.w r9,#0x8000 ; beq.n`, raw 4393 / aligned 903,
wave-151).  It is not a raw win over the landed `count >= 0`
(`cmp.w r9,#0 ; bgt.n`, raw 4391 / aligned 903), so it stays unlanded.

## 8. Independent finding: the `won` guard is `hasWon() == true`

`src/game/menu/MGame.cpp:1554` currently reads

```cpp
bool won = Globals::status->getMission()->hasWon() ||
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() == true;
```

`Mission::hasWon()` returns `unsigned char`, so `== true` emits `cmp r0,#1 ;
bne` at `0x17add4`; the original emits `cmp r0,#0 ; beq`.  Dropping `== true`
(or using `!= false`) reproduces the original guard exactly, but the function
then shrinks to **8060** (raw 6069, aligned 1871): the literal pool at the tail
loses four entries (48 -> 28 bytes) and the whole block layout re-lays.  So the
`== true` form is *load-bearing* for the 8080 layout even though it mismatches
the original instruction.  A future wave must find the extra constant uses that
restore the four literals together with the `cmp #0` guard.

Same story for `failed == 1` at `MGame.cpp:1518`: `if (failed)` gives the
original `cmp r4,#0 ; beq` but shrinks to 8076 (raw 4711, aligned 1968).
