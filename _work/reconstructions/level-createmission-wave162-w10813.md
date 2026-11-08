# w10813 (wave 162): `Level::createMission` — case-183 if-conversion attack
# (no source win; raw 5,454 / aligned 786 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10813`, base `29248c1d0`
(wave 161).  Continues `wave161-merge-tests.md`,
`level-createmission-wave161-w10713.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).  Ghidra MCP
unavailable; NDK `objdump` is the authority.  Raw metric
`/dev/shm/w10813/fdiff.py`, aligned metric `/dev/shm/w10813/align_diff.py`.

## Baseline (re-measured, pristine HEAD)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5454   align 786
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Ground truth for the two branchless diamonds (new)

Original `0xb2e04` (alienWave) and `0xb2f2a` (pirateFortress) are both
if-converted:

```text
b2e04: subs.w r0, fp, r3      ; i - firstIndex
b2e08: mov    r1, r0
b2e0a: it     ne
b2e0c: movne  r1, #1          ; r1 = (i != firstIndex)
b2e0e: ldr    r2, [sp, #60]   ; alienWave
b2e10: eor.w  r2, r2, #1
b2e14: orrs   r1, r2          ; !(i==first && alienWave)
b2e16: bne.w  b2f2a
...
b2f2a: cmp    fp, r3          ; same shape for pirateFortress ([sp,#56])
b2f2c: it     ne
b2f2e: movne  r0, #1
b2f30: ldr    r1, [sp, #56]
b2f32: eor.w  r1, r1, #1
b2f36: orrs   r0, r1
b2f38: bne.w  b30a4
```

Ours emits two short-circuit branches in the **opposite order**:

```text
b2e04: ldr r0, [sp, #60]      ; alienWave
b2e06: cmp r0, #0
b2e08: beq.w b2f20
b2e0c: cmp fp, r3             ; i == firstIndex
b2e0e: bne.w b2f20
```

### Root cause (confirmed in the LLVM 7.0.0 sources)

1. `SelectionDAGBuilder::visitBr` (`SelectionDAGBuilder.cpp:2005`) calls
   `FindMergedConditions` for a `br i1 (and a, b)` when
   `!isJumpExpensive() && BOp->hasOneUse()`.  `FindMergedConditions`
   (`:1811`) emits **operand 0's compare first**.
2. Both operands must be `InBlock` in the branch block, otherwise a single
   branch on the whole `and` is emitted (this is the only way to get a
   branchless form from source).
3. `ARMBaseInstrInfo::isProfitableToIfCvt` (`ARMBaseInstrInfo.cpp:1878`)
   has an `optForSize()` early-out: if the predecessor's last MI is
   `t2Bcc` preceded by `tCMPi8`/`t2CMPri` with **imm 0** on a **low
   register**, it returns false (the `cbnz` case).  Our first branch is
   `cmp r0, #0` (alienWave, imm 0) ⇒ the second (ifirst) diamond is never
   if-converted.  The original's first branch is `cmp fp, r3` (register
   compare) ⇒ no early-out ⇒ the alienWave diamond is if-converted.

So the whole residual is **which `and` operand is operand 0**.

### Why the operand order is pinned

`clang -Xclang -disable-llvm-passes` shows the frontend emits the `&&` as
two branches, ifirst first (`%1954 icmp eq` then `%1957 icmp ne`), exactly
as the source.  `SimplifyCFG` (pass 1) then folds them into
`and(ifirst, alienWave)` — still ifirst first.  **`Reassociate` (pass 25)
swaps it**: `ReassociatePass::canonicalizeOperands` sorts ascending by
`getRank`, and by then `EarlyCSE` has CSE'd the loop-local alienWave
`icmp ne %x, 0` with the pre-loop `alienGuns` comparison, which has a much
lower rank.  Result: `and(alienWave, ifirst)` for the rest of the pipeline
(confirmed in the `-print-after-all` dump and in the `CodeGenPrepare` dump,
`%1724 = and i1 %1722, %1723` with `%1722 = icmp ne i32 %1367, 0`).

`getComplexity` is 5 for both ICmps, so `InstCombine`'s
`SimplifyAssociativeOrCommutative` is not the culprit; it is purely
Reassociate's rank sort.

## 2. Sweep (all measured; size 17,172 B unless noted)

Baseline **5,454 / 786 / 17,172**.

| candidate (case-183 guard) | raw | align | size |
|---|---:|---:|---:|
| `A && alienWave` (baseline) | 5454 | 786 | 17172 |
| `A && alienWave == 1` | 5465 | 809 | 17172 |
| `A && (alienWave & 1)` | 5465 | 809 | 17172 |
| `A && alienWave >= 1` | 5465 | 809 | 17172 |
| `A && alienWave > 0` | 5465 | 809 | 17172 |
| `A && alienWave != 0` | 5454 | 786 | 17172 |
| `alienWave && A` | 5454 | 786 | 17172 |
| `i - first == 0 && alienWave` | 5454 | 786 | 17172 |
| `(A) & (alienWave != 0)` | 5454 | 786 | 17172 |
| `A ? alienWave : 0` | 5465 | 809 | 17172 |
| `alienWave ? A : 0` | 5454 | 786 | 17172 |
| `__builtin_unpredictable(A && alienWave)` | 5454 | 786 | 17172 |
| `__builtin_expect(A, 1) && alienWave >= 1` | 5465 | 809 | 17172 |
| `__builtin_expect(A, 0) && alienWave >= 1` | 5465 | 809 | 17172 |
| `A && __builtin_expect(alienWave >= 1, 1)` | 13420 | 956 | 17180 |
| `__builtin_expect(A && alienWave, 1)` | 12707 | 1179 | 17140 |
| `__builtin_expect(A && alienWave >= 1, 0)` | 15917 | 1947 | 17200 |
| `A && alienWave != 2` | 13582 | 1744 | **16820** |
| `A && alienWave != -1` / `> -1` | 13582 | 1744 | **16820** |

`A` = `i == this->friendCount + this->field_164`.

Every size-exact variant still emits `cmp r0, #0` (or `#1`) on alienWave
**first** at `0xb2e04`; none produces `subs/it ne/movne/eor/orrs`.
`__builtin_expect` in all four polarities (w10713 + this wave) does not
change the `and` operand order, only the block weights, and every
size-exact expect form is worse.

Notable: `alienWave != 2` (imm 2 ⇒ the `cmp #0` early-out cannot fire)
**does** trigger the if-conversion (`.text` shrinks 352 B) — direct proof
that the `isProfitableToIfCvt` early-out is the blocker and that the
if-conversion itself is reachable at `-Oz` in this TU.  But the imm-2
shape changes the function size to 16,820 B (gate-fatal) and also
collapses the already-correct `hasWanted && i == 0` branchless block.

## 3. Why no source form flips it

The only source-level lever on the `and` order is Reassociate's rank sort.
`rank(alienWave icmp)` is pinned low because the comparison is CSE'd to the
pre-loop `int alienGuns = alienWave ? ...` comparison; `rank(ifirst icmp)`
is pinned high because `firstIndex = friendCount + field_164` is
recomputed in the loop block.  Making the alienWave comparison distinct
(`== 1`, `& 1`, `>= 1`, `> 0`) either gets canonicalised back to
`icmp ne %x, 0` or still sorts before ifirst; hoisting `firstIndex`
(`const int firstIndex = ...`) is size-fatal (17,252 B, w10713).

Conclusion: reproducing `0xb2e04`/`0xb2f2a` needs the original's PGO-driven
`and` operand order (or a clang flag), not a source shape.  No change is
landed; baseline is the best measured variant.

## 4. Gate (before -> after)

```text
_ZN5Level13createMissionEv  raw 5454 -> 5454   align 786 -> 786
                            size 17172 -> 17172 B
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
RATCHET PASS
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10813/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10813/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10813/{fdiff.py,align_diff.py,harness.py,try.py,batch.py,
probe_cond*.py,cc_ir.sh,cur.cpp,cgp2.txt,sdb.cpp,reassoc.cpp}`.
