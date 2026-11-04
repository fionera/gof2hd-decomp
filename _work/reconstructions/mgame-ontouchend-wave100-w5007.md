# MGame::OnTouchEnd (8080 B) — w5007 (wave 100): the 0x200/0x400→shared-blx tie
# is a period-8 rounding pattern, not a knife-edge; every tie flip found costs the layout

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144,
8080 B).  Baseline (unchanged by this pass): **78.3565 %**, 8080 B, 2775 original /
2774 ours normalised instructions.  Gate re-verified green at the end of the pass:
byte_exact **4500** / linked_exact **4500** / size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS, `.text` 0x1337c8.

This pass lands **no code change**.  It (1) corrects the wave-98 claim that the
tie is a wide plateau in the dispatch frequency `E` — the real model is a
*chain of floor divisions* and the conflict pattern is period-8 in the 0x400-body
frequency `F400`; (2) shows the nearest tie-flip is **one unit away in `F400`**
(`463615365 → 463615364`), i.e. the tie is a genuine 1-unit near-miss but the
frequency cannot be nudged there without moving the 0x400 body; (3) measures the
tie-flip variants (`x800_0` flips at 8072 B / 73.53 %, `e_sm_0` at 8100 B /
64.27 %, `exp22_0` at 8080 B / 75.20 %); (4) proves the compiler canonicalises
*every* spelling of the `if (hudMenuOpen)` test, so the 5/8 zero-heuristic
probability cannot be changed from source; and (5) quantifies the residual:
after normalising branch targets away the content score is **81.28 %**
(2255/2775), so ~520 instructions differ for reasons other than branch-target
naming.

Prior notes: `mgame-ontouchend-wave98-w4809.md` (newest),
`mgame-ontouchend-wave97-w4707.md`, `mgame-ontouchend-wave96-w4604.md`,
`mgame-ontouchend-wave95-w4509.md`, `mgame-ontouchend-wave94-w4409.md`,
`mgame-ontouchend-hblp-1unit-w4107.md`,
`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`,
`mgame-ontouchend-78-1-8080-b_w3807.md`.

## 0. TU / source-layout witness

`source file list dump/gof2hd_1.1.19_ios.cpp` confirms only that
`Classes/Game/MGame.cpp` owns `_MGame::OnTouchEnd(int, int, void*)`; no TU-local
statics.  Everything below is reconstructed from the Android 2.0.16 binary
(Ghidra + objdump) and the exact repo build.

## 1. The tie, re-derived exactly (corrects wave-98 §3 / §7)

The MBP decision is `MachineBlockPlacement::hasBetterLayoutPredecessor` on the
shared `blx Hud::initHudMenu` block (bb.205, `f = F200+F400 = 2318076825`):

```
prededge = F400 * HotProb            HotProb = 80/100
candedge = F200 * HotProb.getCompl() compl   = 20/100
conflict = prededge >= candedge
```

`BranchProbability` stores `N` fixed-point with `D = 1<<31`:
`N80 = round(80*D/100) = 1717986918`, `N20 = round(20*D/100) = 429496730`
(`4*N20 = 1717986920 = N80 + 2`).  `BlockFrequency::operator*` is
`scale(F, N, D) = floor(F*N/D)` (LLVM 7 `BranchProbability.cpp::scale<D>`).

The 0x200 body `F200` and 0x400 body `F400` are **not** `E/2` and `E/8` directly —
they are produced by repeated `floor` on the dispatch chain (each bit test is a
50/50 branch that `calcZeroHeuristics` explicitly declines to predict because
the LHS is an `and` with a power-of-two mask):

```python
def freqs(E):
    F200 = E // 2            # 0x200 true arm
    f1   = E - F200          # 0x200 false arm
    t2   = f1 // 2           # 0x800 true arm
    f2   = f1 - t2           # 0x800 false arm
    return F200, f2 // 2     # 0x400 true arm
```

With the actual `E = 3708922921` this gives exactly the trace values
`F200 = 1854461460`, `F400 = 463615365`, and `F200 == 4*F400`, so

```
prededge = floor(463615365*1717986918/2^31) = 370892291
candedge = floor(1854461460*429496730/2^31) = 370892292   -> no conflict
```

The wave-98 note modelled `F400 = E/8` and concluded the tie is a flat plateau
`E ∈ 3708922918..3708922935`.  That is wrong: the chain-of-floors model gives a
**period-8** pattern, and e.g. `E = 3708922925` *does* conflict:

| E | F200 | F400 | conflict |
|---|---|---|---|
| 3708922910..2916 | 1854461455..1854461458 | 463615364 | **True** |
| 3708922917 | 1854461458 | 463615365 | **True** |
| 3708922918..2924 | 1854461459..1854461462 | 463615365 | False |
| **3708922921 (actual)** | **1854461460** | **463615365** | **False** |
| 3708922925..2927 | 1854461462..1854461463 | 463615366 | **True** |
| 3708922928..2932 | 1854461464..1854461466 | 463615366 | False |
| 3708922933..2935 | 1854461466..1854461467 | 463615367 | **True** |

Equivalently, fixing `F200 = 4*F400` the conflict is a period-8 function of
`F400`; the current value sits in the "no conflict" residue:

```
F400 = 463615357..359  conflict True
F400 = 463615360..361  conflict False
F400 = 463615362..364  conflict True
F400 = 463615365       conflict False   <-- ours
F400 = 463615366       conflict False
F400 = 463615367..369  conflict True
```

So the nearest flip is **`F400 -= 1`** (or `+= 2`).  This is a real 1-unit
near-miss, but `F400` is a deterministic function of `E`, and `E` is itself
determined by the whole pre-dispatch CFG.  The 0x400 body's frequency cannot be
nudged without also moving the block (see §3).

## 2. Why `E` cannot be nudged from source

`E` is `bb.188` (the `if (this->hudMenuOpen)` block, `L2052`).  Its predecessor
`bb.187` has `f = 5934276674`, and

```
E = scale(5934276674, N, 2^31) = 5934276674 * 5 // 8 = 3708922921
```

because `if (hudMenuOpen)` is an `icmp ne i8 %x, 0` and `calcZeroHeuristics`
assigns `ZH_TAKEN_WEIGHT/ZH_(NON)TAKEN = 20/32 = 5/8` to `X != 0`
(`ndk_BranchProbabilityInfo.cpp` lines 115-116, 664-760).  Every source spelling
that survives InstCombine produces the identical IR, hence the identical
frequency:

| spelling | size | exact % | tie |
|---|---|---|---|
| `if (this->hudMenuOpen)` (baseline) | 8080 | 78.3565 | not flipped |
| `if (this->hudMenuOpen != 0)` | 8080 | 78.3565 | not flipped |
| `if (this->hudMenuOpen == 1)` | 8080 | 78.3565 | not flipped |
| `if (this->hudMenuOpen == true)` | 8080 | 78.3565 | not flipped |
| `if (this->hudMenuOpen != false)` | 8080 | 78.3565 | not flipped |
| `if (this->hudMenuOpen > 0)` | 8080 | 78.3565 | not flipped |
| `if (this->hudMenuOpen >= 1)` | 8080 | 78.3565 | not flipped |
| `if (this->hudMenuOpen & 1)` | 8080 | 78.3565 | not flipped |
| `bool hudOpen = this->hudMenuOpen; if (hudOpen)` | 8080 | 78.3565 | not flipped |
| `if (1 == this->hudMenuOpen)` | 8080 | 78.3565 | not flipped |

`__builtin_expect_with_probability` is **not** available in NDK clang 7.0.2
(`error: use of undeclared identifier`), so a sub-1 % probability nudge is not
expressible either.  (`__builtin_expect(x, 1)` / `(x, 0)` exist but jump the
probability straight to 9:1 / 1:9 and change the size to 8088 — wave-98
`e_hud_1` / `e_hud_0`.)

## 3. Measured tie-flip variants

`checktie2.py` prints every `ldrd r0, r2, [fp, #116]` window and the following
instruction.  Baseline (ours) and original:

```
original: 0c7c ldrd; movs r1,#1; b.n 0xe9a     (0x200 body branches)
          0e94 ldrd; movs r1,#2; [fallthrough]  (0x400 body falls into blx)
baseline: 0c80 ldrd; movs r1,#1; blx; b.w tail (0x200 body falls into blx)
          0e28 ldrd; movs r1,#2; b.n 0xc88      (0x400 body branches back)
```

Tie-flip experiments (official `score2.py` exact ratio):

| variant | source change | size | exact % | tie |
|---|---|---|---|---|
| baseline | — | 8080 | **78.3565** | no |
| `x800_0` | `__builtin_expect(flags & 0x800, 0)` | 8072 | 73.5268 | **yes** |
| `x800_1` | `__builtin_expect(flags & 0x800, 1)` | 8044 | 70.5500 | no |
| `x200_0` | `__builtin_expect(flags & 0x200, 0)` | 8088 | 74.6532 | no |
| `x400_0` | `__builtin_expect(flags & 0x400, 0)` | 8076 | 72.5536 | no |
| `x400_1` | `__builtin_expect(flags & 0x400, 1)` | 8028 | 70.6477 | no |
| `m4_0` | expect(!isMining, 0) in the `&4` test | 8092 | 75.6844 | no |
| `m80_0` | expect(!isMining, 0) in the `&0x80` test | 8088 | 76.0584 | no |
| `b4_0` | expect(flags & 4, 0) | 8088 | 75.5451 | no |
| `b80_0` | expect(flags & 0x80, 0) | 8092 | 76.0807 | no |
| `e_sm_0` | expect(starMapOpen, 0) (wave-98) | 8100 | 64.2677 | **yes** |
| `exp22_0` | expect(campaignMission == 22, 0) (wave-98) | 8080 | 75.1983 | **yes** |

`x800_0` is the cleanest flip: `__builtin_expect(flags & 0x800, 0)` drops the
0x800 arm to 10 %, so the 0x400 arm's frequency rises from `E/8` to `0.45*E` and
the tie resolves in favour of the 0x400 body.  But the same frequency rise makes
MBP place the 0x400 body **early** — at 0xd4e, immediately before
`maneuverTouchEnd` (0xd5a) — whereas the original places it at 0xe94, after the
whole mission-success / MC chain.  The score falls to 73.53 and the size to 8072.
Combining `x800_0` with every faithful content knob does not recover it:

| variant | size | exact % |
|---|---|---|
| `x800_0 + A1` (re-fetch campaign mission) | 8076 | 73.4775 |
| `x800_0 + B1` (unguarded hud0) | 8068 | 73.5172 |
| `x800_0 + C1` (pauseOpen/hudMenuOpen order) | 8072 | 73.6349 |
| `x800_0 + A1 + B1` | 8072 | 73.5400 |
| `x800_0 + A1 + B1 + C1` | 8072 | 73.6482 |
| `x800_0 + A1 + B1 + nest` | 8080 | 70.1261 |

Conclusion: the tie is flippable, but *only* by a change that also moves the
0x400 body's frequency, and that movement is worth far more than the tie.

## 4. The nested-`else` tail shape (`vnest`) is byte-identical to `t_nested`

The original's weaponSlot body ends `b.w 0x17afe8` (the isAutoPilot tail),
skipping the `hudTouchFlags == 0` test; the wingman body ends `b.w 0x17afe2`
(the test).  The natural source shape that reproduces this CFG is to put the
`hudTouchFlags == 0` block inside the `else` of `if (weaponSlot >= 0)`:

```cpp
if (weaponSlot >= 0) {
    ...                       // ends -> tail
} else {
    if (wingmanCommand >= 0) {
        ...                   // ends -> test
    }
    if (this->hudTouchFlags == 0) { ... }
}
```

This is semantically equivalent (weaponSlot >= 0 implies one of bits
0x2000..0x10000 is set, so hudTouchFlags != 0).  Measured: **75.0766 % at 8084 B**
— exactly wave-98's `t_nested`, so the earlier note had already found this shape.
It moves the test/tail cluster early (0xd70) but leaves the shared blx at 0xc86,
so the 0x400 body still branches back and the 0x200 body still falls through.
Combining with the content fixes does not help (`A1+B1+nest` = 73.79 / 8084,
`B1+nest` = 74.85 / 8080, `A1+...` = 75.10 / 8088).

## 5. The residual is content, not just branch-target naming

`tools/verify` compares intra-function branch targets literally (`loc_ec0` vs
`loc_1836`).  Normalising `loc_[0-9a-f]+` and absolute hex operands to `#T`:

```
raw SequenceMatcher ratio   78.3565 %
normalised ratio            81.2759 %
equal instructions          2255 / 2775
```

So branch-target naming costs only ~2.9 pts; ~520 instructions are genuinely
different.  The non-layout content differences are small and already known:

1. **Mining guard** — original `cmp.w r9,#0; beq.n` (from `if (amount != 0)`),
   ours `cmp.w r9,#-1; bgt.n` (from `int count = -amount; if (count > 0)`).  Both
   6 B.  The faithful `if (amount != 0)` spelling scores 71.45 / 8076 (identical
   with `count = -amount` or `count = amount`); `if (amount < 0)` is
   canonicalised back to the baseline (`cmp r9,#-1; bgt`) and scores 78.3565, so
   the cheaper spelling stays.
2. **`add.w r8, sp, #80` scheduling** — ours hoists the `String report` address
   before the `String("\n")` constructor; the original emits it after.  This is
   a scheduler side effect of the `count > 0` vs `amount != 0` shape.
3. **Shared-blx placement** (§1–§3).

Everything else in the normalised diff is block reordering: the mission-success
region (`0xdd6..0xef6`), the menuWindow cluster, and the campaign-switch chain
(`0x11d8..0x1212`).  These follow from MBP work-list order and are not
independently addressable.

## 6. Conclusion

The baseline **78.3565 % / 8080 B is a strict local optimum**.  The tie that
decides the whole tail cluster is a genuine 1-unit `BlockFrequency` near-miss
(`F400 = 463615365` vs the nearest conflicting `463615364`), but:

* `E` (the dispatch frequency) is fixed by the 5/8 zero-heuristic on
  `hudMenuOpen`, which the compiler canonicalises for every source spelling;
* `__builtin_expect_with_probability` is unavailable in NDK clang 7.0.2;
* every `__builtin_expect` that flips the tie changes the 0x400 body's frequency
  enough to move the body itself, losing 5–14 pts.

No source shape found in this pass improves on 78.3565.

## 7. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4500
# exact per-function ratio
python3 /dev/shm/w5007/score2.py
# tie detector (prints size + every ldrd r0,r2,[fp,#116] window)
python3 /dev/shm/w5007/checktie2.py
# block-order side-by-side
python3 /dev/shm/w5007/blockside.py [so]
# normalised (branch-target-free) content score
python3 /dev/shm/w5007/normdiff.py
# MBP trace (llc 7.0.0, approximate: NDK clang 7.0.2 has 433 blocks, llc 432)
clang++ <repo flags> -S -emit-llvm -o MGame.ll src/game/menu/MGame.cpp
sed 's/, variables: ![0-9]*//' MGame.ll > MGame2.ll
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-androideabi16 -O2 -debug-only=block-placement \
  MGame2.ll -o /dev/null 2>mbp.txt
```

Scratch harness (not tracked): `/dev/shm/w5007/` — `score2.py` (exact ratio),
`checktie2.py` (tie detector), `blockside.py` / `blockperm.py` / `blockalign.py`
(block-order comparison), `normdiff.py` (branch-target-free diff), `batchE.py` /
`batchF.py` / `batchG.py` / `batchH.py` / `batchI.py` / `batchJ.py` /
`batchK.py` / `batchL.py` (variant runners), `knobs.py` (named source edits),
`mbp_ote.txt` (baseline MBP trace), `orig_fn.s` / `ours_fn.s` (disassembly).
