# w11511 (wave 169): `Level::createMission` — guard `icmp` de-sinking,
# ARM `combineSelectAndUseCommutative` gate (no raw win; 5,435 / 767 / 17,172 B)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11511`, branch
`wave169-w11511`, base master `00c403d5a`.  Continues
`level-createmission-wave169-w11411.md` (guard `icmp` de-sinking sweep) and
`level-createmission-wave168-w11311.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. New mechanism: the OR form folds back to a `select` at isel

w11411 established the original guard is an i32 `or` of `zext(i != first)`
and `xor alienWave, 1`, branched on `!= 0`.  This wave found *why* the
plain i32-OR source still does not reproduce it:

`ARMTargetLowering::PerformORCombine` -> `combineSelectAndUseCommutative`
(`ARMISelLowering.cpp:9775`, called at `:11026`) rewrites

```text
(or (zext cc), x)  ->  (select cc, (or x, 1), x)
```

when the `zext`/`select` node has **one use**.  With `x = xor(alienWave,1)`
known 0/1, `x | 1` folds to `1`, so the whole guard becomes
`select cc, 1, xor` and codegen is

```text
ldr r0, [sp,#116]   ; hoisted xor (spilled)
cmp fp, r3
it ne
movne r0, #1        ; select
cmp r0, #0
beq ...
```

not the original `orrs`.  This is the real gate after `SinkCmpExpression`
and `FindMergedConditions` (both already diagnosed in w11411).

The original's `mov r1,r0; it ne; movne r1,#1` is the ARM lowering of a
`select`/`zext` whose false value is the `i - first` subtraction itself
(`subs.w r0, fp, r3`), i.e. `zext(icmp ne (sub), 0)`; the second guard's
`cmp fp,r3; it ne; movne r0,#1` is the same thing without the shared sub.

## 2. Measured sweep (all built with `link_so -j2`, `tools/fn_diff.py`)

| # | guard source shape | size | raw | aligned |
|---|---|---:|---:|---:|
| HEAD | `i == first && alienWave` | 17172 | **5435** | **767** |
| 1 | `!((i != first) \| (alienWave ^ 1))` (first only) | 17180 | 14237 | 1772 |
| 2 | both guards zero-form OR | 17184 | 12621 | 1171 |
| 3 | restructured positive `if (OR1) { if (OR2) B3 else B2 } else B1` | 17184 | 12621 | 1171 |
| 4 | `bool atFirst = i == first;` used by both ORs | 17184 | 12617 | 1214 |
| 5 | `bool skipAlien/skipPirate` computed in one block | 17184 | 12617 | 1214 |
| 6 | `!((i != first) \| (1 - alienWave))` | 17180 | 14237 | 1772 |
| 7 | `!((i != first) \| (alienWave == 0))` | 17172 | 5446 | 790 |
| 8 | `!((i != first) \| (alienGuns == 0))` | 17172 | 5446 | 790 |
| 9 | `!((i != first) \| !alienGuns)` | 17172 | 5446 | 790 |
| 10 | `(i == first) & (alienWave != 0)` | 17172 | 5435 | 767 |
| 11 | `(i == first) & alienGuns` | 17160 | 13011 | 3282 |
| 12 | `!((i != first) \| (alienGuns ^ 1))` | 17180 | 14203 | 1771 |
| 13 | `!((i != first) \| (alienGuns - 7))` | 17180 | 14233 | 1774 |
| 14 | `volatile int alienWave` + OR | 17168 | 9932 | 2173 |
| 15 | `bool alienWave` (B,a,P) + OR | 17156 | 12652 | 1690 |
| 16 | `bool alienWave`, P,B,a decl order | 17172 | 11484 | 798 |
| 17 | `unsigned int alienWave` + OR | 17180 | 14237 | 1772 |
| 18 | `unsigned int i` + OR | 17180 | 14238 | 1772 |
| 19 | `(i - friendCount - field_164) != 0` + OR | 17216 | 15054 | 3336 |
| 20 | `const int firstStation` local + OR | 17312 | 16015 | 3587 |

### Findings

* Rows 1/6/17/18: `^ 1` (or `1 - x`) is required to keep the `or` at i32
  width (a `bool`/`== 0` second operand lets InstCombine fold
  `or i32 (zext a),(zext b)` -> `or i1 a,b`, which `FindMergedConditions`
  splits again).  But the `xor` is loop-invariant and speculatable, so
  LICM hoists it to the preheader; the guard then uses the spilled xor
  and the ARM OR-combine turns it into `select cc,1,xor`.  Size +8.
* Rows 4/5: giving the `zext` **two uses** blocks
  `combineSelectAndUseCommutative`, and the guard becomes a genuine
  `orrs`.  But the two-use `zext` is lowered as `clz; lsr #5` (not the
  original `subs; mov; it ne; movne` select) and the xors are still
  hoisted (sp+104/sp+116); size +12.
* Row 7/8/9: `== 0` / `alienGuns` forms canonicalise back to an i1 `or`
  and stay split; size exact but +11 raw (polarity/order only).
* Row 10: `(i == first) & (alienWave != 0)` is byte-identical to HEAD.
* Row 16: the only size-exact OR-form shape is `bool alienWave` with the
  P,B,a declaration order (guard exact, `eor` inline from sp+60).  But
  the bool zero-init/RA shuffles the init block: `pirateFortress = 0`
  is emitted at `0xb0ee6` instead of the original `0xb0ecc`, and the
  three 32-bit zero stores collapse, so every following PC-relative
  literal re-aligns -> raw 11,484.

## 3. Why size is the blocker

The original guard is 18 B (`subs; mov; it; movne; ldr; eor; orrs; bne`);
our split guard is 16 B (`ldr; cmp; beq; cmp; bne`).  So an exact guard
is **+2 B**, and every OR shape also perturbs register allocation in the
mission-type switch body (first code diff at `0xb0548`: the original has
an extra `ldr r4,[sp,#104]`), adding another +6..+10.  No shape reached
17,172 B with raw < 5,435.

## 4. Conclusion

The de-sinking gate is now fully explained: after
`SinkCmpExpression`/`FindMergedConditions`, the last hurdle is ARM's
`combineSelectAndUseCommutative` folding the i32 `or` into a `select`.
Blocking it (two-use `zext`) does produce `orrs`, but the two-use `zext`
lowers as `clz; lsr` and the loop-invariant `xor` is hoisted, so size
grows.  No size-exact shape reduces raw below 5,435.  Nothing landed;
tree left clean at the baseline.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11511/`: `Level.cpp.base`, `measure.sh`, `mkll.sh`,
`try.py`, `irtry.py`, `batch.py`, `batch2.py`, `restructure.py`,
`idiff.py`, `Level*.ll`, `varA.so`, plus LLVM 7.0.2 sources `cgp.cpp`,
`LICM.cpp`, `DAGCombiner.cpp`, `ARMISelLowering.cpp`.
