# Generator::createMission — CVP select-fold root cause (w3803)

Status: research note (2026-11-03). No source change landed: none of the variants below is a
strict improvement over the current gate. Worktree clean; `check` passes at
`byte 4498 / linked 4498`, `.text 0x1337c8`.

## 1. Baseline

```
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

`report.json` row:

```
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
  osize 1996  usize 1996  pct 73.590096  byte False  linked False
```

## 2. The single root cause: CVP folds the `pick == 12` select

Original block (`../_work/bins/android_2.0.16_libgof2hdaa.so`, `0x972b0..0x972d4`):

```
972b0: 2e0c        cmp   r6, #12
972b2: d10c        bne.n 972ce
972b4: 9808        ldr   r0, [sp, #32]
972b6: 6800        ldr   r0, [r0]
972b8: f7ca ebaa   blx   Status::getSystem()
972bc: f7ca ebde   blx   SolarSystem::getIndex()
972c0: 220c        movs  r2, #12               ; type = 12
972c2: 2819        cmp   r0, #25
972c4: f04f 0b01   mov.w fp, #1                ; selected = true
972c8: bf18        it    ne
972ca: 4632        movne r2, r6                ; type = pick
972cc: e002        b.n   972d4
972ce: f04f 0b01   mov.w fp, #1
972d2: 4632        mov   r2, r6                  ; type = pick
```

So the original keeps the select `select (getIndex() != 25), pick, 12` all the way to
instruction selection, and keeps `type` in `r2` while `pick` stays in `r6`.

Our build (`cmake-build-match/libgof2hdaa.so`, same region):

```
972b4: f5b1 2f40   cmp.w r1, #786432           ; (short)pick << 16 == 12 << 16
972b8: d109        bne.n 972ce
972ba: 9807        ldr   r0, [sp, #28]
972bc: 6800        ldr   r0, [r0]
972be: f7ca eba8   blx   Status::getSystem()
972c2: f7ca ebdc   blx   SolarSystem::getIndex()
972c6: f04f 0b01   mov.w fp, #1
972ca: 260c        movs  r6, #12                 ; select constant-folded
972cc: e001        b.n   972d2
972ce: f04f 0b01   mov.w fp, #1
972d2: 3d01        subs  r5, #1
```

The select is folded by **Correlated Value Propagation** (the legacy pass name printed in
`-print-after-all` is "Value Propagation"; the pass argument is `correlated-propagation`).

### 2.1 Mechanism, proven from the LLVM 7.0.2 source in `_work` scratch

`CorrelatedValuePropagation::processPHI` (CVP.cpp:184) iterates over the loop-carried `type`
phi's incoming values and calls `LVI->getConstantOnEdge(Incoming, FromBB, ToBB, P)`.

For the `pick == 12` incoming (the select `%83`), `LazyValueInfoImpl::getEdgeValue`
(LVI.cpp:1355) falls through to `getBlockValue(%83, pick12block)`, which runs
`solveBlockValueSelect` (LVI.cpp:833). That queries the block value of the select's true
value (`pick`). `solveBlockValueNonLocal` computes the meet over the block's predecessors,
and the block's only predecessor is the block that ends in `icmp eq i32 %pick, 12`; so the
block value of `pick` there is exactly `12`. Both select arms are then `12`, the select's
block value is `12`, and CVP replaces the phi incoming with the constant `12`.

The same fold for `short pick` is reached through `getEdgeValueLocal` (LVI.cpp:1232): the
branch condition is `%41 = shl i32 %40, 16` (`cmp.w r1,#786432`) and the select's true arm
is `%42 = ashr exact i32 %41, 16`, a foldable `CastInst` whose operand is the branch
condition; `constantFoldUser` (LVI.cpp:1201) proves `%42 == 12` on the true edge.

Either way the select dies before instruction selection. `simplifyCommonValuePhi`
(CVP.cpp:139) is *not* what removes the select; it is skipped because `processPHI` already
set `Changed = true` on the earlier incoming values (and on the select itself).

### 2.2 Why the original kept it

The original's `type` phi is `[pick, 10, pick, pick, select, pick]` — the `pick == 8`
incoming is `pick`, not the constant `8` (the original shares the latch `mov r2,r6` between
the `pick == 8` and `!pick==12` paths). Our `short pick` and `int pick` builds both
constant-fold the `pick == 8` incoming to `8`/`movs r6,#8`. This is a consequence, not a
cause: once the select survives, `type` is forced into `r2` (the select destination), the
shared `mov r2,r6` latch becomes profitable, and the `pick == 8` incoming stays `pick`.

The original's register assignment is exactly what a surviving select forces:

```
type = r2   pick = r6   selected = fp   used = sl   guard = r9   targetStation = r5
race = [sp,#36]   status = [sp,#32]   rnd = [sp,#40]   agent = [sp,#24]   systems = r8
```

Our base coalesces `type` with `pick` into `r6`, spills `used` to `[sp,#32]`, puts `guard`
in `r5`, and `targetStation` on the stack.

## 3. `pick` is `int` in the original (confirmed again)

The original has no `sxth`/`uxtb`/`uxth`/`lsls #16` anywhere in the loop. It compares the
raw `nextInt` result directly:

```
97264: 4606        mov   r6, r0
97266: 2808        cmp   r0, #8
9726a: 2e0a        cmp   r6, #10
972b0: 2e0c        cmp   r6, #12
```

Our `short pick` build needs `cmp.w r9, r0, lsl #16` (with `r9 = 8 << 16`), `sxth r6, r0`,
and `cmp.w r1, #786432`. So the original's `pick` is `int`.

`int pick` alone (with the `0.5000001f` anchor) is 1960 B and official 48.9%: it removes the
`short`-specific instructions but the select still folds and the register allocation moves
`type` into `r6`, so the fuzzy score collapses. `int pick` + `unsigned short type` is
1996 B but official 58.98%.

## 4. The `0.5f` anchor and the literal pool (correction to w3591)

The original has **no** `0.5f` literal in the pool. It emits the 4-byte immediate
`vmov.f32 s2, #96` (0.5) inline at `0x977ce`. The current `0.5000001f` hack compiles to
`vldr s2, [pc, #...]` plus a pool word `0x3f000002`, i.e. +4 bytes at the end of the
function. The original's pool is:

```
0x9795c  0x42be0000  95.0f
0x97960  0x44960000  1200.0f
0x97964  0x45abe000  5500.0f
0x97968  0x3f99999a  1.2f
0x9796c  0x3f333333  0.7f
0x97970  0x3f19999a  0.6f
0x97974  0x3fd9999a  1.7f
```

All seven are used (95.0f is the `type == 11` divisor; 1.7f is the `type == 8` reward
multiplier). `0x97974` is **1.7f**, not a dead `0.5000001f`. The real 4 bytes behind the
size anchor are the surviving select (`movs r2,#12; cmp r0,#25; it ne; movne r2,r6`, 8 B,
vs our folded `movs r6,#12`, 2 B, plus the shared `mov r2,r6` latch). True `0.5f` is 1992 B
and fails the gate by shifting every later function.

## 5. Measured variants (official verifier unless noted)

All edits are in `src/game/mission/Generator.cpp`; `check` run each time. `fast pct` is the
scratch `realtest.py` score (base = 62.200, official = 73.590096).

| variant | size | official pct | gate | notes |
|---|---:|---:|---|---|
| HEAD (`short pick`, hack) | 1996 | 73.590096 | PASS | baseline |
| true `0.5f` | 1992 | 73.8 | FAIL byte 2615 | −4 B shift |
| `int pick` | 1960 | 48.9 | FAIL byte 2045 | select folds, `type`→`r6` |
| `int pick` + `unsigned short type` | 1996 | 58.977901 | PASS | size matches, score worse |
| `int pick` + `unsigned short type` + true `0.5f` | 1992 | (fast 47.8) | FAIL | size −4 |
| `int pick` + `unsigned short type` + hoisted select + true `0.5f` | 1996 | 58.287293 | PASS | select merges with branch (`eor/orr; it eq; moveq`) |
| `unsigned short type` + `short pick` | 2020 | — | FAIL | |
| `short type` + `int pick` | 2012 | 41.1 | FAIL | |
| `while (!selected)` + `--guard` after `nextInt`, `int pick` | 1988 | 66.712612 | FAIL byte 2615 | guard position matches, still folds |
| `type = pick` removed, assigned in each continue/else | 1996 | 73.590096 | PASS | same IR as base |
| `int pick` + `short p = pick` branch | 1960 | 48.9 | FAIL | select survives but `uxth`+inverted |
| `int pick` + `p = pick & 0xff` branch | 2000 | — | FAIL | `uxtb`+inverted select |
| `int pick` + `p = pick & 0xff` branch + true `0.5f` | 1956 | 49.025070 | FAIL | `uxtb r0,r6; cmp r0,#12; it eq; moveq r6,#12` |
| `__builtin_expect` on getRoutes==nullptr / pick==12 / used / race / campaign | 1964–2028 | 30–61 | mostly FAIL | no win |

The only 1996-byte variants found are the base and `int pick + unsigned short type`
(58.98%). A 128-variant sweep over `{pick type} × {type type} × {for/while} ×
{guard in for-increment / in body} × {type=pick at top / in each continue} ×
{select arm pick / type} × {0.5000001f / 0.5f}` produced no 1996-byte variant with a
fast score above 62.200.

## 6. Other confirmed shape facts

* `--guard` is right after `nextInt` in the original (`97260: sub.w r9,r9,#1`), before the
  `continue`s; ours decrements at the latch.
* The original's `pick == 8` path is 4 B (`cmp r0,#8; beq 972d2`) and shares the latch
  `mov r2,r6`; ours is 8 B (`cmp r0,#8; bne; movs r6,#8; b`).
* The original's `pick == 10` path checks `pick` first and materializes `type = 10` before
  the `race > 3` test; ours checks `race` first.
* The original's `used[pick] && sum != 14` path has an explicit `mov r2,r6`
  (`9729a: cmp r3,#14; mov r2,r6; bne 972d4`); ours has none because `type` is coalesced.
* The original's `getRoutes() == nullptr` block is placed far away at `0x97586`
  (`beq.w` from `0x973a8`), after the `type == 8` block; ours is inline (`cbz` to a nearby
  block). This is a block-ordering difference, not a source-logic difference.
* The original's prologue has `movs r2,#0` (type), `mov.w fp,#0` (selected),
  `ldr.w sl,[r0,#80]` (used); ours has `mov.w sl,#0` / `movs r6,#0` / a spill.

## 7. Reproduction

```sh
# baseline
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8

# original / our block
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x971b8 --stop-address=0x97330 /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x971b8 --stop-address=0x97330 cmake-build-match/libgof2hdaa.so
$OD -s --start-address=0x97950 --stop-address=0x97980 /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# IR at CVP for the whole TU (the pass prints as "Value Propagation")
clang++ <Generator.cpp flags> -mllvm -print-after-all 2>&1 | grep -A200 "IR Dump After Value Propagation"
```

## 8. Remaining root cause (one line)

The original's `select (getIndex() != 25), pick, 12` survives CVP because in the original's
IR the `pick == 12` true block's block value of `pick` is not provably `12`; every
straightforward source shape we can write puts the select in a block whose sole predecessor
ends in `icmp eq %pick, 12`, so `solveBlockValueNonLocal`/`getEdgeValueLocal` proves the
select is `12` and CVP folds it. Finding the exact source spelling that blocks that proof
(while keeping `pick` an `int` and `type` in `r2`) is the remaining work.
