# w10912 (wave 163): `Level::createMission` — beating `Reassociate` on the
# `and` operand order (measured; no size-exact source win landed)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10912`, base `e51db2fc7`
(wave 162).  Continues `wave162-merge-tests.md`,
`level-createmission-wave162-w10813.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).  NDK
`objdump` is the authority (Ghidra MCP not used).  Raw metric
`/dev/shm/w10912/fdiff.py`, aligned metric `/dev/shm/w10912/align_diff.py`.

## Baseline (re-measured, pristine HEAD)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5454   align 786
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. The rank model (new, from the LLVM 7.0.0 sources)

`ReassociatePass::BuildRankMap` (`Reassociate.cpp:165`) assigns a rank to
every **memory-dependent** instruction (loads/stores/calls) in RPO order:
`ValueRankMap[I] = ++BBRank`, where `BBRank` starts at
`(blockIndex << 16)`.  `getRank` (`:189`) then computes
`1 + max(rank(operands))` for everything else, with a per-parent-block
`MaxRank` cap that breaks phi cycles.

For the case-183 guard:

```text
%1116 = phi i32 [0, %1114], [%1110, %1108]     ; alienWave
%1110 = phi i32 [0,...], [1,...]               ; all constants -> rank 1
%1124 = icmp ne i32 %1116, 0                   ; rank 3
%1410 = icmp eq i32 %1402, %1403               ; ifirst
%1403 = add nsw i32 %1400, %1401               ; friendCount + field_164
%1400/%1401 = phis with loop-carried loads in block 1673
                                               ; rank(ifirst) ~ BBRank(1673)
%1411 = and i1 %1124, %1410                    ; alienWave first
```

`canonicalizeOperands` (`:220`) swaps iff `getRank(RHS) < getRank(LHS)`.
`rank(alienWave)=3` is always below `rank(ifirst)`, so the swap is
unconditional for any source spelling of `i == first && alienWave`.

## 2. The `or` form: a genuine order flip (new)

The guard is lowered as
`(i != first) | !alienWave` (this is exactly the original's
`subs / it ne / movne / ldr / eor / orrs` block).  If the guard is written
through a pointer/reference to the flag, clang emits the **`or` form**
instead of the `and` form:

```cpp
int *awp = &alienWave;
int *pfp = &pirateFortress;
...
if (i == this->friendCount + this->field_164 && *awp) { ... }
else if (i == this->friendCount + this->field_164 && *pfp) { ... }
```

Final IR: `%1413 = or i1 %1412, %1348` with `%1412 = icmp ne i32 %1404, %1405`
(ifirst, **operand 0**) and `%1348 = xor i1 %1124, true`.  The `or` is
formed after `Reassociate`, so the rank sort never sees it.  The emitted
block is the original's branchless form:

```text
b2dce: subs.w r0, fp, r3      ; ifirst
b2dd2: mov    r1, r0
b2dd4: it     ne
b2dd6: movne  r1, #1
b2dd8: ldr    r2, [sp, #116]  ; !alienWave (precomputed)
b2dda: orrs   r1, r2
b2ddc: beq.w  b2ef0           ; body
```

i.e. `isProfitableToIfCvt`'s `cmp #0` early-out is avoided: the first
compare is `cmp fp, r3`, and the second diamond is if-converted.

## 3. Why it is not a size-exact win

The `or` form makes `LICM` hoist the two negations
(`xor(icmp ne alienWave,0,true)`, `xor(icmp ne pirateFortress,0,true)`)
out of the loop and spill them to two new stack slots.  The frame grows
`400 -> 408`, which shifts every stack offset in the function:

| candidate | raw | align | size |
|---|---:|---:|---:|
| baseline `i == first && alienWave` | **5454** | **786** | **17172** |
| `int *awp`/`int *pfp`, `*awp` / `*pfp` | 12630 | 1727 | **17168** |
| `int &aw`/`int &pf`, `aw` / `pf` | 12630 | 1727 | **17168** |
| `const int &aw`/`pf` | 12630 | 1727 | **17168** |
| `int *awp` only, `*awp` | 7177 | 813 | 17172 |
| `int *awp`/`pfp`, `*awp == 1` | 5465 | 809 | 17172 |
| `int *awp`/`pfp`, `*awp > 0` | 5465 | 809 | 17172 |
| `int *volatile awp`/`pfp`, `*awp` | 13301 | 3007 | 17128 |
| volatile slot + plain ptr | 15781 | 3104 | 17152 |
| `*(volatile int*)&alienWave` before if | 13404 | 3191 | 17152 |
| `volatile int alienWave/pirateFortress` | 13350 | 3467 | 17116 |
| `*(volatile int*)&alienWave = alienWave` | 13347 | 2948 | 17136 |
| `!(i != first || !alienWave)` | 5454 | 786 | 17172 |
| `(i == first) & (alienWave != 0)` | 13338 | 1388 | 17192 |
| `(i == first) & alienWave` | 14528 | 1758 | 17188 |
| `i == first ? alienWave : false` | 5465 | 809 | 17172 |
| `(i == first) && (alienWave != 0)` | 5454 | 786 | 17172 |
| `__builtin_expect(i == first, 1) && alienWave` | 5454 | 786 | 17172 |
| `__builtin_expect(i == first, 0) && alienWave` | 5454 | 786 | 17172 |
| `__builtin_expect(alienWave, 1)` (2nd operand) | 15999 | 2921 | 17168 |
| `__builtin_expect(i == first && alienWave, 1)` | 15919 | 3116 | 17176 |
| `bool isAlien = i == first && alienWave` | 5454 | 786 | 17172 |
| hoist `firstIndex` + `alienWave && i == fi` | 6540 | 1472 | 17196 |
| `i == first && alienWave && i >= first` | 5454 | 786 | 17172 |
| `i == first && alienWave && i < first + f168` | 5454 | 786 | 17172 |
| `i == first && alienWave && this->field_168 >= 0` | 5684 | 789 | 17172 |
| `(Globals::rnd->nextInt(1), alienWave)` | 6733 | 875 | 17196 |
| `(i == first) && (alienWave & 1)` | 5465 | 809 | 17172 |
| `(i == first) && alienWave == 1` | 5465 | 809 | 17172 |

Every size-exact variant is `>= 5454` raw.  The only shapes that produce
the branchless block (`int *awp` / `int &aw`) are 4 B short (17,168) and
re-lay the whole stack frame.

## 4. The remaining fix (for the next wave)

The `or` form is right; the missing 4 B is the inline `eor` that `LICM`
hoisted.  If the `xor`'s operand is made loop-variant (a real load of
`alienWave` inside the loop, not a promoted phi), `LICM` cannot hoist the
`xor`, the `eor` stays inline, and the frame stays 400 B.  The escape must
not add a stack slot: the two pointer locals are 8 B of frame.  Candidates
to try next:

* pass `&alienWave` to an already-imported pointer-taking callee in the
  loop (no new import, no new local);
* a `ptrtoint` escape that `SROA` refuses to promote but that does not
  spill a pointer;
* a source shape that forms the `or` after `LICM` rather than before it.

## 5. Gate (before -> after)

```text
_ZN5Level13createMissionEv  raw 5454 -> 5454   align 786 -> 786
                            size 17172 -> 17172 B
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
RATCHET PASS
```

No source change is landed; the baseline is the best measured variant.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10912/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10912/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10912/{fdiff.py,align_diff.py,cc.sh,Level.pristine.cpp,
measure.py,try_.py,check2.py,orig.txt,ours_base.txt,ours_ptr.txt,pa.txt}`.
