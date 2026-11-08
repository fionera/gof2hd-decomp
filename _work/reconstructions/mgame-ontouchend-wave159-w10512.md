# MGame::OnTouchEnd — wave 159 (w10512): `cmp #0; beq` sweep; the base stays the Pareto point

Status: analysis-only, no source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10512`, base `cfde1f5f2` (wave 158 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10512`.
Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 158) | this wave |
|---|---:|---:|
| size | 8080 | **8080** (unchanged) |
| raw same-address diff | 4387 | **4387** |
| aligned (align_diff) | 901 | **901** |
| `won` guard | `cmp #1; bne.w` | **`cmp #1; bne.w`** |
| `dockToStream` PLT slot | `0x683a0` | **`0x683a0`** |
| `MGame::dockEvent` byte diff | 0 | **0** (1300 B) |

## 1. The guard in the original

The original's `won` guard is the second leg of the `||` short-circuit:

```
17adc8: blx  Mission::hasWon            ; A
17adcc: cbnz r0, 17ade0                 ; A != 0 -> body
17adce: ldr  r0, [r8]
17add2: blx  Status::getCampaignMission
17add6: blx  Mission::hasWon            ; B
17adda: cmp  r0, #0
17addc: beq.w 17b0aa                    ; B == 0 -> else (resetStartSequenceOver)
17ade0: body...
```

The base reproduces everything except the last compare: `cmp r0,#1; bne.w`
(`icmp eq i8 %B, 1`, 50:50 default weight).  The truthy spelling
`bool won = A || B;` produces exactly `icmp eq i8 %B, 0; br %cmp, else, body`
(`cmp #0; beq`), but LLVM 7 `BranchProbabilityInfo::calcZeroHeuristics`
(`llvm-7.0.0/.../BranchProbabilityInfo.cpp:653`) assigns `X == 0` the 20:12
"unlikely taken" weight, which re-lays the function to **8060 B** (20 short)
and explodes the aligned diff to 1871.

The only static escape from `calcZeroHeuristics` is its AND exception:

```cpp
if (Instruction *LHS = dyn_cast<Instruction>(CI->getOperand(0)))
  if (LHS->getOpcode() == Instruction::AND)
    if (ConstantInt *AndRHS = dyn_cast<ConstantInt>(LHS->getOperand(1)))
      if (AndRHS->getValue().isPowerOf2())
        return false;                       // -> default 50:50
```

So `(B & 1) != 0` keeps 50:50 and the 8080 B layout, but the ARM backend's
`SelectCMPZ` (`ARMISelDAGToDAG.cpp:2497`) rewrites `cmpz(and X,1), 0` into
`lsls X, #31` when `And->hasOneUse()`, and the branch becomes `bne.n body`
instead of `cmp #0; beq.w else`.  Because the IR successor order is
`else, body`, 50:50 makes the else the fall-through, so
`resetStartSequenceOver`'s `bl` (0x17adea) precedes `dockToStream`'s
(0x17ae28) in relocation order and gold gives reset the earlier PLT slot
(`0x683a0`), moving `dockToStream` to `0x683ac`; `MGame::dockEvent`'s
`blx dockToStream` then differs by 1 byte (byte_exact 4507 -> 4506).

## 2. Measured spellings (all on the wave-158 base)

`raw` = `fdiff.py` same-address bytes; `aligned` = `align_diff.py`
instruction-index non-matches; `PLT` = `dockToStream` `.plt` address;
`dockEvent` = `fdiff.py _ZN5MGame9dockEventEii`.

| variant | raw | aligned | size | dockEvent | PLT | note |
|---|---:|---:|---:|---:|---|---|
| **base `== true`** | **4387** | **901** | **8080** | **0** | **0x683a0** | `cmp #1; bne.w` |
| `(B & 1) != 0` (AND) | 4367 | 902 | 8080 | 1 | 0x683ac | `lsls #31; bne.n`; ratchet FAIL (byte 4506) |
| AND + starMap `expect(!OnTouchEnd,0)` | 4375 | 956 | 8080 | 0 | 0x683a0 | w10311 shape; gate PASS |
| truthy `A \|\| B` | 6069 | 1871 | 8060 | 107 | 0x683a0 | `cmp #0; beq`, 20 B short |
| `(B ^ 1) == 0` | 4387 | 901 | 8080 | 0 | 0x683a0 | canonicalises to base |
| `switch (won) { case 0: reset; default: body; }` | 4387 | 901 | 8080 | 0 | 0x683a0 | SimplifyCFG -> base branch |
| `expect(A,1) \|\| (B&1)!=0` | 4748 | 867 | 8096 | 105 | 0x683a0 | +16 B, dockEvent moves |
| `expect(won,1)` on AND | 4752 | 835 | 8092 | 105 | 0x683a0 | +12 B, best aligned but size |
| `expect(won,0)` on AND | 5146 | 2162 | 7832 | 142 | 0x683ac | 248 B short |
| `expect((won&1)!=0,0)` on AND | 4736 | 1299 | 8076 | 103 | 0x683a0 | 4 B short |
| second use: `campaignWon` + `&` in keepObjectives | 4654 | 924 | 8084 | 103 | 0x683a0 | +4 B |
| second use: nested ternary in keepObjectives | 4600 | 902 | 8092 | 105 | 0x683a0 | +12 B |
| second use: `volatile` store of AND | 4633 | 906 | 8088 | 103 | 0x683a0 | +8 B |

Other `B` comparison spellings that canonicalise back to the base
(`raw 4387 / aligned 901 / size 8080 / dockEvent 0`): `B < 2`, `B > 1`,
`B <= 1`, `B >= 2`, `B == 2`, `(B - 1) == 0`, `B % 2 == 0`.
Spellings that canonicalise to the truthy 8060 B shape: `B != 0`, `B > 0`,
`B >= 1`, `B != false`, `B == (bool)0`, `(int)B == 0`, `(unsigned char)B == 0`,
`B == 0.0f`, `!B`.

## 3. Why the AND form cannot reach `cmp #0` at 8080 B

`SelectCMPZ` fires only when the AND has **one use**.  Giving the AND a second
use (so the backend keeps `and.w` + `cmp #0`) costs 4 bytes at the guard and
every such spelling measured 8084-8092 B, i.e. it moves `dockEvent`.  The
`(B & 1) != 0` form is size-neutral only because `lsls #31` replaces the
2-byte `cmp`, and its two-byte `bne.n` is offset by the else-block layout.

The truthy form already reaches `cmp #0; beq` with the correct PLT order, but
the 20:12 weight changes the literal-pool order (the original's `nop` +
`pool:pic:str:t` run becomes `pool:pic:got:_ZN7Globals8gameTextE` first) and
the `hudTouchFlags == 0` epilogue placement, so it is 20 B short and its
aligned diff is 1871.  A `__builtin_expect` cannot express the original's
mild PGO weight: `expect(won,1)` gives 2000:1 and 8092 B, `expect(won,0)`
gives 7832 B.

**Conclusion:** no source spelling reaches `cmp #0; beq` with the
`dockToStream` PLT slot intact AND both metrics <= 4387/901.  The base
(`== true`, 4387/901, 8080 B, dockEvent 0) remains the Pareto point.  The
AND form is the only raw win (4367) but drops `dockEvent` by 1 byte (gate
FAIL); the starMap cold hint repairs the PLT (4375) but costs 55 aligned.
The last unrecoverable signature is the original's PGO weight on the
`won` zero-compare.

## 4. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10512/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4387 / 8080
python3 /dev/shm/w10512/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 901
python3 /dev/shm/w10512/fdiff.py _ZN5MGame9dockEventEii           # 0 / 1300
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10512/{fdiff.py,align_diff.py,structdiff.py,measure.sh,
setvariant.py,batch.py,batch2.py,batch3.py,batch4.py,batch5.py,switchify.py,
bpi.cpp,ARMISelDAGToDAG.cpp,TargetLowering.cpp,DAGCombiner.cpp,MGame.base.cpp}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
