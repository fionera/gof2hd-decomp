# w11411 (wave 169): `Level::createMission` — guard `icmp` de-sinking sweep
# (no raw win; 5,435 / 767 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11411`, branch
`wave169-w11411`, base master `ec3ceb07c`.  Continues
`level-createmission-wave168-w11311.md` (bool/alienGuns/icmp sweep, no win).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## Why the split happens (exact mechanism, LLVM 7.0.2 source)

The `and i1` survives all IR passes (`%1411 = and i1 %1124, %1410` in the
post-opt IR).  The split is done at **isel** by
`SelectionDAGBuilder::visitBr` -> `FindMergedConditions`
(`lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:1968-2037`), which
turns any `and`/`or` of two in-block instructions into two branches
unless `ShouldEmitAsBranches()` (line 1930) vetoes it:

```cpp
  if (Cases.size() != 2) return true;
  ...
  // Handle: (X != null) | (Y != null) --> (X|Y) != 0
  if (Cases[0].CmpRHS == Cases[1].CmpRHS && Cases[0].CC == Cases[1].CC &&
      isa<Constant>(Cases[0].CmpRHS) && ...isNullValue()) {
    if (Cases[0].CC == ISD::SETEQ && Cases[0].TrueBB == Cases[1].ThisBB) return false;
    if (Cases[0].CC == ISD::SETNE && Cases[0].FalseBB == Cases[1].ThisBB) return false;
  }
```

The `SETNE` arm is only reachable for an **`or`** (for `and`,
`Cases[0].FalseBB != Cases[1].ThisBB`), and only when *both* operands
are `icmp ne <x>, 0` with a **null** RHS.  When it fires, the whole
`or` is emitted as one `brcond`, i.e. `orrs` — exactly the original.

`CodeGenPrepare::SinkCmpExpression` (cgp.cpp:1187) is a red herring for
the split itself (it only sinks the `icmp` into the loop block); the
gate is `ShouldEmitAsBranches`.

The original guard at `0xb2e04` is therefore
`or i32 (zext (i != first)), (xor i32 alienWave, 1)` fed to
`icmp ne ..., 0` — i.e. the *negation* of `i == first && alienWave`
computed at i32 width, so `visitBr` sees an `icmp` condition and never
enters `FindMergedConditions` at all.

## Measured sweep (all size-exact unless noted)

Every row built with `link_so -j2`, measured with `tools/fn_diff.py`.

| # | source shape | size | raw | aligned |
|---|---|---:|---:|---:|
| HEAD | `i == first && alienWave` (int) | 17172 | **5435** | **767** |
| 1 | `!((i - first != 0) \|\| (alienWave ^ 1) != 0)` | 17172 | 5446 | 790 |
| 2 | `!((i - first != 0) \|\| (1 - alienWave) != 0)` | 17172 | 5446 | 790 |
| 3 | `!((i - first != 0) \|\| alienWave != 1)` | 17172 | 5446 | 790 |
| 4 | `!((i != first) \|\| (alienWave ^ 1) != 0)` | 17172 | 5446 | 790 |
| 5 | `!((i - first != 0) \| (alienWave ^ 1))` (i32 or) | 17180 | 14237 | 1772 |
| 6 | `!((i - first != 0) \| !alienWave)` | 17172 | 12459 | 1685 |
| 7 | `(i - first == 0) & (alienWave != 0)` | 17172 | 5435 | 767 |
| 8 | `(i - first == 0) && alienWave` | 17172 | 5435 | 767 |
| 9 | `int *awp = &alienWave;` + `*awp` guard | 17172 | 7159 | 794 |
| 10 | restructured `if (A \|\| B) {else-if} else {body1}` | 17172 | 5446 | 790 |
| 11 | `volatile int alienWave` + plain guard | 17180 | 14992 | 2278 |
| 12 | `volatile int alienWave` + i32-or guard | 17168 | 9932 | 2173 |
| 13 | `bool alienWave` (B,a,P order) | 17156 | 12403 | 818 |
| 14 | `bool alienWave` + restructured `\|\|` guard | 17156 | 12394 | 820 |
| 15 | `int pirateFortress; int bigAlienWave; bool alienWave;` (P,B,a) | 17172 | 11484 | 798 |
| 16 | `bool alienWave` + `!((i-first!=0) \| !alienWave)` (B,a,P) | 17156 | 12652 | 1690 |
| 17 | P,B,a + `!((i-first!=0) \| !alienWave)` | 17164 | 12382 | 1690 |
| 18 | all three `bool` | 17180 | 15695 | 2033 |

Full 3!-permutation x {bool alienWave, bool pirateFortress, both} sweep
(18 rows) was also run; the only size-exact bool shape is P,B,a (#15).

### Findings

* Rows 1-4, 10 all canonicalise to the same IR (`%1348 = icmp eq i32
  %alienWave, 1` + `and i1`) — InstCombine rewrites
  `(alienWave ^ 1) != 0` -> `alienWave == 1` and De-Morgans the
  negated `or` back into an `and`, so `ShouldEmitAsBranches` never sees
  the null-RHS `SETNE` pair.  The guard stays split and the only change
  is a **frame-slot swap** (alienWave moves from `sp+60` to `sp+64`,
  bigAlienWave the other way), costing +11 raw.
* Row 5 (i32 `or`) folds to `select (i != first), 1, (xor)` because
  `xor alienWave, 1` is known to be 0/1 (SROA-promoted SSA value), and
  the `xor` is LICM-hoisted to `sp+116` — the w168 diagnosis, unchanged.
* Row 7/8: `i - first == 0` is folded straight back to `i == first`,
  byte-identical to HEAD.
* Row 9: forcing the alloca to escape (`&alienWave`) keeps size exact
  but the guard is still `ldr; cmp #0` (an int load is still an `icmp`
  leaf), raw 7159.
* Row 13/15: a `bool alienWave` reproduces the original OR-form guard
  **byte-for-byte** (`subs.w; it ne; movne; ldr [sp,#60]; eor.w; orrs;
  bne.w`) because the operand is then a `load i1`, not an `icmp`.  But
  the bool changes the zero-init/register-allocation: the `alienWave`
  and `bigAlienWave` zero stores get hoisted *before* the `rnd <= 29`
  `blt` (`mov.w r0,#0; str r0,[sp,#60]; mov.w r0,#0; str r0,[sp,#64]`)
  instead of staying in the single later block with a shared zero
  register, and the total shrinks by 16 B.  Only the P,B,a declaration
  order keeps 17,172 B, and there the `pirateFortress = 0` store is
  emitted before `alienWave = 1` (original emits it after), a -2 B shift
  at `0xb0ee4` that re-aligns every PC-relative literal load and blows
  raw up to 11,484.

## Conclusion

The original guard is an i32 `or` of `zext(i != first)` and
`xor alienWave, 1`, branched on `!= 0`.  It is reproducible exactly
either by declaring `alienWave` as `bool` (operand becomes a load) or by
an i32 `or` whose second operand is a *load with unknown range*.
Neither is size-neutral: the bool form costs -16 B (zero-init/RA), and
the i32 form needs an unpromoted alloca, which SROA/clang will not give
without changing the frame.  No size-exact shape reduces raw below
5,435.  Nothing landed; tree left clean at the baseline.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11411/`: `Level.cpp.base`, `measure.sh`, `setguard.py`,
`batch{1..5}.py`, `sweep_decl.py`, `adiff.py`, `sizediff.py`, `rawhist.py`,
`firstshift.py`, `cmp2.py`, `mkll.sh`, `head.so`, `bool.so`, `v5446.so`,
`orig.dis`, `sdb.cpp` (LLVM 7.0.2 SelectionDAGBuilder.cpp), `cgp.cpp`.
