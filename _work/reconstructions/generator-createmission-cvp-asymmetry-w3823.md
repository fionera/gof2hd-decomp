# Generator::createMission — CVP fold asymmetry and declaration-order sweep (w3823)

Status: research note (2026-11-03). No source change landed: nothing below beats the current
gate. Worktree clean; `check` passes at `byte 4498 / linked 4498`, `.text 0x1337c8`.

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
`osize 1996  usize 1996  pct 73.590096`.

## 1. Baseline

```
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.91
RATCHET PASS
```

Build setup note: in this worktree `cmake --preset default` alone resolves `../_work` to the
non-existent `/home/debian/GalaxyOnFire2/wt/_work`; configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
and `-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.

## 2. Exact LVI mechanism for the fold (re-derived from LLVM 7.0.1 sources)

The loop's merge phi (after CVP, our build) is

```
%87 = phi i32 [ 8, %37 ], [ 10, %44 ], [ %42, %70 ], [ %42, %68 ], [ 12, %79 ], [ %42, %77 ]
```

`%42 = ashr exact i32 %41, 16`, `%41 = shl i32 %40, 16`, `%40 = nextInt(15)`.
`%37` ends in `%43 = icmp eq i32 %41, 524288` (pick == 8); `%79` is the pick==12 block with
`%85 = select i1 %84, i32 12, i32 %42`.

`LazyValueInfoImpl::solveBlockValueSelect` (LVI.cpp:833) asks for the block value of the true arm
`%42` in the select block `%79`. `%79` has one predecessor `%77`, whose branch condition is
`%78 = icmp eq i32 %41, 786432`. `getEdgeValueLocal` (LVI.cpp:1232) does **not** match `%42`
against the condition directly (`%42` is not `%41` and not `add %41, C`), but then takes the
`User *Usr = dyn_cast<User>(Val)` branch: `%42` is a `CastInst`, its operand `%41` is the branch
condition operand, `getValueFromCondition(%41, %78, true)` returns `786432`, and
`constantFoldUser(%42, %41, 786432)` = `SimplifyCastInst(ashr, 786432, i32)` = **12**.
Both select arms are then 12, so `solveBlockValueSelect` returns 12 and `processPHI`
(CVP.cpp:184) replaces the phi incoming with the constant 12.

For `int pick` the same edge is proved even more directly: `getValueFromICmpCondition(%40,
icmp eq %40, 12, true)` returns 12. So both `short pick` and `int pick` fold.

## 3. New finding: the original's fold is *asymmetric*

Original (`0x97266..0x972d4`):

```
97266: cmp r0,#8
97268: beq.n 972d2          ; pick==8 -> shared latch
9726a: cmp r6,#10
9726c: bne.n 97276
9726e: ldr r0,[sp,#36]      ; race
97270: movs r2,#10          ; type = 10
97272: cmp r0,#3
97274: bhi.n 972d4          ; race>3 -> loop latch
...
972b0: cmp r6,#12
...
972ce: mov.w fp,#1
972d2: mov r2,r6            ; type = pick (shared by pick==8 and else)
972d4: loop condition
```

The pick==8 phi incoming is `pick` (it shares `mov r2,r6` with the non-12 else path), while the
pick==10 incoming is the constant `10` (`movs r2,#10`). Our build folds *both* (`[8, %37]`,
`[10, %44]`). This is direct evidence that the original's CVP did **not** fold the pick==8
incoming at all: if it had, the copy would have to be `movs r2,#8`, not the shared `mov r2,r6`.
So in the original's IR the pick==8 branch condition and the phi incoming were different SSA
values (a copy the register allocator later coalesced into r6), which is exactly the condition
under which `getEdgeValueLocal` returns overdefined.

## 4. Declaration-order sweep (new, measured)

The base order is the unique best of the 5! local orders (rotations + 40 random permutations,
measured with the official verifier):

| declaration order | size | pct |
|---|---:|---:|
| `level, selected, type, guard, used` (HEAD) | 1996 | **73.590096** |
| `selected, type, guard, used, level` | 1996 | 73.590096 |
| `used, level, selected, type, guard` | 1996 | 73.590096 |
| `type, guard, used, level, selected` | 1988 | 70.572019 |
| `guard, used, level, selected, type` | 1992 | 70.661157 |
| `guard, level, type, used, selected` | 1988 | 70.158511 |

So the wave-88 lead "`type` declared before `level` gives `movs r2,#12`" is a false positive:
that `movs r2,#12` is the campaign `switch` case 4, not the pick==12 select, and every
`type`-first order scores 70.16–70.66 at 1988–1992 B.

## 5. `int pick` has the original's register allocation but the folded select

`int pick` (anchor kept) is 1960 B / 48.9%, but its *register allocation* is the original's:

| | original | `int pick` | HEAD (`short pick`) |
|---|---|---|---|
| type | r2 | r6 (coalesced with pick) | r6 (coalesced with pick) |
| pick | r6 | r6 | r6 |
| guard | r9 | r9 | r5 |
| used | sl | sl | spilled `[sp,#32]` |
| constant 0 | r5 | r5 | sl |
| `8<<16` temp | — | — | r9 |

So the only thing `int pick` is missing is the surviving select (and the ~32-byte loop body it
forces). This is why the `int pick` variants collapse to ~48.9% even though their allocation is
closer: the loop's instruction stream and the whole downstream layout shift.

## 6. Variants measured this pass (official verifier; all `check`-safe only where size = 1996)

| variant | size | pct |
|---|---:|---:|
| HEAD (`short pick`, `0.5000001f`) | 1996 | **73.590096** |
| `int pick` | 1960 | 48.9 |
| `int pick` + `unsigned short type` | 1996 | 58.98 |
| `short pick` + `type` first | 1988 | 70.57 |
| `short p = pick`, branch on p, arm pick | 1996 | 53.6 |
| `int pick` + `short p = pick`, branch on p, arm pick | 1960 | 48.9 |
| `short pick` outside loop | 1996 | 73.590096 |
| `short pick` + `--guard` after nextInt (`for(;;)`/`while`) | 1988 | 66.7 |
| `short pick` + do-while + `--guard` after nextInt | 1976 | 55.2 |
| `int pick` + `--guard` after nextInt | 1976 | 45.4 |
| `int pick` + do-while + `--guard` after nextInt | 1996 | 42.1 |
| `uint32_t pick` | 1960 | 48.9 |
| `int pick` + `short p` + `--guard` after nextInt | 1968 | 63.0 |
| `__builtin_assume(pick in [0,15))` + `short pick` | 1996 | 73.6 (fold unchanged) |
| `int rawpick; short pick = rawpick;` + assume, arm rawpick | 1996 | 48.9 (fold unchanged) |
| shared end-of-body `type = pick` block (goto/else-if form) | 1992 | 53.2 |
| `type = pick; if (getIndex()==25) type = 12;` (branchy select) | 1996 | 73.590096 |

No variant reaches 1996 B with a higher score than HEAD. The shared end-of-body `type = pick`
block (`if (pick==8) { /*fall through*/ } else if (...) { type=10; continue; } else { ... }`
then `type = pick;`) is the shape that gives the original's `mov r2,r6` shared latch, but the
pick==12 select still folds and the function shrinks by 4 bytes (1992 B / 53.2%). The branchy
`type = pick; if (getIndex()==25) type = 12;` spelling is canonicalised back to the same
`select` and folds identically (1996 B / 73.590096).

## 7. Remaining root cause

The original's pick==8 phi incoming is `pick`, i.e. the original's CVP did **not** prove
`pick == 8` on the pick==8 edge, and consequently did not prove `pick == 12` on the pick==12
edge either; the select `select (getIndex()==25), 12, pick` survived and forced `type` into r2
and `pick` into r6. With clang 7.0.2's LVI every straightforward spelling makes the branch
condition and the phi incoming the *same* SSA value (or a foldable cast of it), so
`getEdgeValueLocal`/`constantFoldUser` proves the edge value and CVP folds it. Reproducing the
original needs a source shape where the branch operand and the select's true arm are two
different SSA values that the register allocator still coalesces into r6 (the original's shared
`mov r2,r6` latch). No such shape was found in this pass.
