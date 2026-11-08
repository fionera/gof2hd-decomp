# MGame::OnTouchEnd — wave 152 (w9813): `count >= 0` keeps the decrementing IV and matches `cmp #0`

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9813`,
base `9d491d46a` (wave 151 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9813`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 151) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4392 | **4391** |
| aligned (align_diff) | 904 | **903** |
| guard | `cmp.w r9,#2 ; bcc.n` | **`cmp.w r9,#0 ; bgt.n`** |

## 1. Mission hypotheses (both falsified)

The wave-151 note suggested two new angles; both were measured on the exact base.

1. `bool nonzero = amount != 0; if (nonzero)` — InstCombine folds the i1 back to the
   equality `icmp`, so LSR still creates the `ICmpZero` use and the loop flips to the
   incrementing count: **8060 B** (guard `ldr.w r9,[sp,#24]`, the flipped layout).
   Same for `int nonzero = amount != 0; if (nonzero)` and for a `volatile` copy.
2. `switch (amount) { case 0: break; case 1: break; default: body }` — canonicalises to
   `icmp ugt amount, 1`, i.e. exactly the landed `amount != 0 && amount != 1` shape:
   **8080 B, raw 4392, aligned 904** (no change). `switch` with `case 0` only flips (8060).

The LSR mechanism is now fully explained by LLVM 7 `IVUsers`/`LoopStrengthReduce`:
`AddUsersImpl` records the *negation instruction* (`%count = sub 0, %amount`) as the
`OperandValToReplace`, so the Basic use of the string argument has SCEV `-amount`, the same
SCEV as the `ICmpZero` use (`0 - amount`).  The count addrec therefore serves both uses with
one register, while keeping the decrementing `amount` IV costs two; the flip is strictly
cheaper.  Any source where the string argument is `-amount` (or `count = -amount`) flips.
A `String(amount)` argument does not flip, but is not the original (the original has
`rsb r1,r9,#0` at `0x17a5da`).

## 2. The landed shape

`count = -amount` is already computed for `String(count)`.  Spell the guard as

```cpp
int count = -amount;
if (count >= 0) {
    report += String("\n") + String(count) + String("t ") +
              *Globals::gameText->getText(1274 + refined);
}
```

`count >= 0` is a non-equality signed compare, so LSR creates no `ICmpZero` use and the
decrementing IV survives.  The backend canonicalises `-amount >= 0` to `amount <= 0` and
emits the original's 4-byte `cmp.w r9,#0` (only the branch condition differs:
`bgt.n` vs the original `beq.n`), instead of the landed shape's `cmp.w r9,#2`.

The guard is **observationally exact** in this function: `count == 0` means the inner
`while (hasCargo)` never ran, so `refinedAny` is false and `report` is never read (the
`else` branch shows `getText(292)`).  The extra `report +=` for `count == 0` is therefore
dead.  (`amount != 0` and `count >= 0` differ only on that dead path.)

## 3. Other measured shapes (all 8080 unless noted)

| variant | size | guard at 0x486 | raw | aligned |
|---|---:|---|---:|---:|
| `amount != 0 && amount != 1` (base) | 8080 | `cmp #2; bcc` | 4392 | 904 |
| `count >= 0` (**landed**) | 8080 | `cmp #0; bgt` | **4391** | **903** |
| `amount < 1` / `amount <= 0` | 8080 | `cmp #0; bgt` | 4391 | 903 |
| `amount < 0` / `amount <= -1` | 8080 | `cmp #-1; bgt` | — | — |
| `amount != 0 && amount <= 0` | 8080 | `cmp #-1; bgt` | — | — |
| `switch { case 0: break; case 1: break; default: }` | 8080 | `cmp #2; bcc` | 4392 | 904 |
| `switch { case 0: break; default: }` | 8060 | flipped | 6590 | 1835 |
| `bool/int nonzero = amount != 0; if (nonzero)` | 8060 | flipped | 6590 | 1835 |
| `String(amount)` instead of `String(count)` | 8060 | flipped | — | — |
| `switch { case 0/1: break; default: String(-amount) }` | 8080 | flipped | 4468 | 2077 |
| `-mllvm -disable-lsr` on `amount != 0` (diagnostic only) | 8060 | `cmp #0; beq` | 6590 | 1835 |

`-disable-lsr` confirms the pre-LSR IR is the original (`cmp #0; beq`, decrementing IV),
but the 8060 layout is far from the original's 8080 layout, so it is not a usable target.

## 4. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9813/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4391 / 8080
python3 /dev/shm/w9813/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 903
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9813/{fdiff.py,align_diff.py,gen.py,measure.sh,compile.sh,
objfn.py,diffruns.py,lsr.dbg,ne0.dbg,ne0c.dbg,MGame.base.cpp}` and the isolated
LLVM-7 LSR/IVUsers source under `/dev/shm/w9813/llvm/`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
