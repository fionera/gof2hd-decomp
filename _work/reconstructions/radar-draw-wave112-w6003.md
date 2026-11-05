# Radar::draw pass (wave112, w6003) — chain / hint / duplicate-block variants dead end

**Status:** research note, 2026-11-05. Worktree `wt/w6003`, branch
`wave112-w6003`, base `5d98bd391`. No source change landed; this commit
contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale).

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  raw 74.029509 %
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals). Everything in 73.95..74.04 still rounds to the reported 74.0.

## 1. New facts established this wave (from Thumb + Ghidra)

1. **The original has exactly ONE store to `field_0x1a8`** (`strb.w r0,[r4,#424]`
   at `0x1d0`). Our source stores it twice: the early `onPlanetCourse` block
   (`0x1d0`) *and* the post-planet-loop `if (!alienOrbit) { …recompute…;
   this->field_0x1a8 = onPlanetCourse; }` block (ours at `0xebe8`). The
   original therefore has **no post-loop recompute at all** — its call
   multiset confirms it: 1 `getAutoPilotTarget`, 1 `goingToPlanet`,
   4 `isDockingToPlanet`, 5 `getSystem`, 2 `getStations`,
   1 `getStationEnumIndex`; ours has 2/2/5/7/3/2 (the extra block is
   `getPlayer,getAutoPilotTarget,getSystem,getSystem,getStations,
   getStationEnumIndex,getPlayer,goingToPlanet,getPlayer,isDockingToPlanet`).
   Removing it is semantically correct but *lowers* the fuzzy score to
   62.31 % (the 152 B block accidentally aligns with the original's
   neighbouring planet-lock code). See §3.
2. **The second `otherLock` assignment (source line 528,
   `a||b||c||lockedStation`) is already correct.** Both the original
   (`0x376..0x38e`) and ours (`0x378..0x390`) compile it to the identical
   `ldr [sp,#124]; ldr [r0]; cbnz; ldr [r4,#56]; cbnz; ldr [r4,#28]; cmp;
   beq cold; movs r0,#1; cmp sl,#2; str r0,[sp,#144]` shape; only the cold
   block address differs (`0xbce` original vs `0xb8c` ours, 66 B earlier).
   (An earlier edit of mine that "fixed" the *other* `otherLock`
   assignment at line 953 — the pre-gas-cloud one — is a red herring: that
   assignment is a different source statement and changing it costs 1.8 %.)
3. **The original tail-duplicates the first chain**: it appears twice
   (`0x2fc` and `0x376`), and the cold blocks twice (`0xbc2` reading
   `field_0x14`, `0xbce` reading `lockedStation`). Our build also has two
   copies, so this is not a missing statement.
4. **Call-multiset deltas are all block-placement artefacts except the
   post-loop block:** original 354 `blx`, ours 361. The other three
   differences are `isDead` (17 vs 16), `isDying` (13 vs 12) and
   `DrawString` (9 vs 8). The `isDead`/`isDying` pair is one tail-duplicated
   `cloud->isDead() || cloud->isDying()` block; the missing `DrawString` is
   because our two landmark lock-label `DrawString` calls (generic branch
   and `i==0 && !alienOrbit` branch) are tail-merged into a single
   `blx 5fe14` at `0xeaf2`, whereas the original keeps two separate calls
   (`0x9e0` and `0xb36`) because their register restores differ.
5. **The original keeps `PlayerEgo::crosshairPos.x/.y` live as floats
   (`s26`/`s24`) across `AEMath::operator-`** in the enemy in-sight block
   (`0xef5a`/`0xef5e` load, `0xef6e`/`0xef72` `vcvt` after the call, then
   `VectorLength`, then the four bounds). That is why the original's
   prologue is `vpush {d8-d13}` while ours is `vpush {d8-d11}`. The
   faithful source shape (`float cx/cy` + `int size` local, distance test
   first) reproduces the load/convert split but scores 64 % (§4).
6. **The original's landmark loop header is not rotated**: preheader
   `0x7a8..0x7d2` falls through into the condition at `0x7d4`
   (`cmp sl,r1; bcs exit`), body at `0x7e0`. Ours is rotated: preheader
   ends with `b 0xeb04` (condition) and the body is at `0x7b0` entered by
   `bcc 0x7b0`. Same for the stack-slot histogram: the sets of `[sp,#N]`
   offsets are almost identical (slot counts differ only in 88..152), i.e.
   the frame is the same size and the local layout is within one 4-byte
   slot of the original.
7. **`static-likely-prob` is 80 (percent), not 80:1.** LLVM 7
   `MachineBlockPlacement::getLayoutSuccessorProbThreshold` returns
   `BranchProbability(StaticLikelyProb, 100)` when the function has no
   profile data, i.e. a successor edge must exceed **80 %** to be chosen as
   the layout successor. This is the "80:1" in the task hint. It is a
   compiler default, not a source shape; `-mllvm -static-likely-prob=80`
   (and 90) changes nothing, 50 does.

## 2. First-chain spelling sweep (new placements)

All variants replace only the first `otherLock` block on the pristine
baseline. `p00x` etc. are the *positive* OR form with `__builtin_expect`
on individual terms (`0` = expect false, `x` = no hint):

| spelling | usize | raw % |
|---|---:|---:|
| landed nested-expect negative-AND (baseline) | 9908 | **74.029509** |
| `pos_or_exp1` (`expect(a||b||c,1)`) | 9908 | 74.000859 |
| `p00x` (`expect(a!=0,0) || expect(b!=0,0) || c!=0`) | 9900 | 72.836676 |
| `plain_neg` / `pos_or` / `tern` (no hints) | 9908 | 72.797593 |
| `p0xx` (`expect(a!=0,0) || b || c`) | 9904 | 71.680275 |
| `p0x1` (`a` hinted 0, `c` hinted 1) | 9916 | 71.888873 |
| `p01x` (`a`,`b` hinted 0? no: `b` hinted 1) | 9916 | 71.870524 |
| `px0x` (`b` hinted 0) | 9892 | 71.625107 |
| `p000` (all three hinted 0) | 9912 | 70.915877 |
| `pos_or_exp0` (`expect(a||b||c,0)`) | 9908 | 70.915877 |
| `or4` / `or3_if` / `notnot` (`o = a||b||c||d`) | 9904 | 70.549419 |
| `or3_if_bool` | 9852 | 67.824340 |

The positive-OR + per-term-`0` form (`p00x`) is the only spelling that
produces the original's `cbnz a; cbnz b; cmp c; beq cold` sequence
(cold block at `0x2e6`), but the cold block then lands inline right after
the chain instead of at `0xb72`, and the score falls 1.2 %.

## 3. Removing the (semantically wrong) post-loop recompute

| variant | usize | raw % |
|---|---:|---:|
| baseline (keeps the block) | 9908 | **74.029509** |
| `nodup_store` (`if (!alienOrbit) field_0x1a8 = onPlanetCourse;`) | 9760 | 62.160601 |
| `nodup_none` (block deleted) | 9756 | 62.314026 |

Deleting the block is *correct* against the binary (one `[r4,#424]` store
in the original) and restores the original's call multiset exactly
(354 `blx`), but it shrinks the body by 152 B and re-colours every register:
the largest unmatched run in the original jumps from 32 instructions to
408 (the whole landmark loop). The baseline's extra block is what keeps
the fuzzy alignment together, so it cannot be removed without a much
larger reconstruction of the surrounding layout.

## 4. Enemy in-sight block (crosshair floats live across the call)

| variant | usize | raw % |
|---|---:|---:|
| baseline (`int cx/cy`, bounds first, distance last) | 9908 | **74.029509** |
| `F5` (`float cx/cy`, `int size`, distance first, bounds in `&&`) | 9920 | 64.378664 |
| `F6` (same, bounds in a nested `if`) | 9928 | 64.077948 |
| `F7` (`int size` before `int cx/cy`, distance first) | 9920 | 64.493064 |
| `INS_F4` (wave111: `int cx/cy`, distance first) | 9900 | 67.612496 |
| `INS_F` (wave111: `float cx/cy`, distance-only outer `if`) | 9928 | 64.399028 |

`F5` does reproduce the original's `vldr s26/s24` before `operator-` and
`vcvt` after it (and hence `vpush {d8-d13}`), but the surrounding register
allocation and block layout lose far more than the 12 prologue bytes gain.

## 5. Hint sweeps

* Full **3^5 = 243** sweep over `__builtin_expect` placement on the five
  terms of the landmark on-screen predicate (on top of the pristine
  baseline): the maximum is still **74.029509 %** (9908 B), reached by
  many combinations including the landed `1 n n n 1` and `1 1 n n n`.
  Best non-baseline score is 72.49 % (`n n 1 n 1`, 9900 B). No hint
  placement beats the baseline.
* Removing all hints (`nohints`: plain first chain + plain on-screen
  predicate + plain `else if`) = 9904 B / 71.068462 %.
* Removing only the on-screen hints = 9896 B / 71.154397 %; only the
  `else if` hint = 9912 B / 72.242910 %; both = 9908 B / 72.794960 %.
* Adding hints to other loop branches is uniformly worse:
  `planetTargets->data()[i]==nullptr` expect 0 → 9876 B / 71.153846;
  `!onScreen` continue expect 0 → 9876 B / 71.153846;
  landmark `continue` guard expect 0 → 10148 B / 38.471302;
  enemy `if (onScreen)` expect 1 → 9868 B / 62.850568;
  gas-cloud `if (onScreen)` expect 1 → 9908 B / 71.326268;
  asteroid `if (onScreen)` expect 1 → 9924 B / 68.555079.

## 6. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 %**.
The remaining gap to the original is not a local statement spelling: the
original differs in (a) the absent post-loop `onPlanetCourse` recompute,
(b) the un-merged landmark `DrawString` pair, (c) the un-rotated landmark
loop layout, and (d) a `d8-d13` float allocation forced by the crosshair
floats living across `AEMath::operator-`. Each of these is individually
reproducible from source, but every one of them moves
`MachineBlockPlacement` enough to lose more matched instructions than it
gains. The baseline is a (tuned) local optimum of the fuzzy metric.

The worktree was restored to the pristine `Radar.cpp` before the final
gate. `check` is green: byte 4500, linked 4500, lint CLEAN, drift 0,
data 843/843, imports 207, `RATCHET PASS`.

## 7. Scratch

All scratch under `/dev/shm/w6003` (not committed): `Radar.base.cpp`,
`measure.py`, `vdiff.py`, `blocks.py`, `unmat.py`, `showgap.py`,
`dumpours.sh`, `chainvariants.py`, `chain2.py`, `comb2.py`, `ins5.py`,
`hints2.py`, `sweep_ons.py`, `nohints.py`, `nodup2.py`, `variants.py`,
`calldelta.py`, `orig.dis`, `ours.dis`, `vd_base.txt`,
`vd_chain2fix.txt`.
