# Generator::createMission — wave 191b / w13584: raw 1 -> 0 (byte-exact)

Status: **byte-exact win landed.**  Worktree `wt/w13584`, branch
`wave191b-w13584`, base master `8bae955f7`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` (1,996 B,
744 insn) went from **raw 1 / aligned 1** to **raw 0 / aligned 0** with
the function size unchanged (1,996/1,996, 744/744 insn).  The whole
`.text` slice is now byte-identical to the original.

## The last byte

| offset | original | before |
|---|---|---|
| `0x9735a` (`bhi.n`) | `d815` -> `0x97388` (race-check block) | `d84e` -> `0x973fa` (merge/`getName`) |

On the `type >= 15` edge of the mask check (`0x97356 ldr r1,[sp,#36];
0x97358 cmp r1,#14; 0x9735a bhi`), LVI knows `type != 13`, so
`JumpThreading` retargets the edge straight to the merge block and drops
the redundant `type == 13` test.  The original keeps the test.

## Root cause

The source `race < 4 && type == 13` is emitted by clang IRGen as two
conditional branches to a common destination.  `SimplifyCFG` merges them
into one `and i1` in the race-check block, and `JumpThreading`'s
`ComputeValueKnownInPredecessors` (the 1-bit `and` special case) then
folds the `type == 13` operand to false on the `type >= 15` edge.

## Fix

Keep the two tests in **separate basic blocks** with a free,
side-effecting-but-codegen-free statement between them.  `llvm.assume`
is a real call at `-Oz` (so `SimplifyCFG` cannot merge the two branches)
but lowers to no instructions.

```cpp
if (__builtin_expect(race < 4, 1)) {  // lint: arm_builtin
    // `type` is a mission-type code, never the all-ones sentinel.  The
    // assume is free at run time but keeps the two tests in separate
    // basic blocks, as in the original (JumpThreading would otherwise
    // fold the type>=15 edge past the `type == 13` test).
    __builtin_assume(type != 0xffffffffU);  // lint: arm_builtin
    if (type == 13) {
        ... unchanged body ...
    }
}
```

`type` is `uint32_t` and is only ever assigned `0`, `1`, `4`, `7`, `8`,
`10`, `11`, `12`, `13`, or `Globals::rnd->nextInt(15)` (which returns
`0..14`), so `type != 0xffffffffU` is always true — the assume is
semantically faithful and cannot fire.

Post-`Early CSE` IR (the shape that matters for `JumpThreading`):

```
; <label>:128:                       ; preds = %119, %115
  %130 = icmp ult i32 %29, 4
  br i1 %130, label %131, label %174
; <label>:131:                       ; preds = %128
  %132 = icmp ne i32 %107, 2147483647
  tail call void @llvm.assume(i1 %132)
  %133 = icmp eq i32 %107, 13
  br i1 %133, label %134, label %174
```

The mask-false edge now lands on block 128, whose terminator is
`icmp ult %race, 4` — LVI cannot derive anything about `race` from
`type >= 15`, so no threading happens and the emitted `bhi` targets the
race-check block exactly like the original.

## Measured variants (all with the baseline `rem` shape)

| variant | raw | size | notes |
|---|---:|---:|---|
| HEAD (`expect(race<4 && type==13,1)`) | 1 | 1,996 | `bhi` -> merge |
| `(type & 0xffffU) == 13` | 366 | 1,996 | inserts `uxth`, shifts tail |
| `(type & 0xffU) == 13` | ~366 | 1,964 | `uxtb`, `type` forced to `r5` |
| `(type < 15) & (mask != 0)` (`&`, no expect) | 1 | 1,996 | fixes `bhi`, but `ands` vs `tst` at `0x97364` |
| `assume(type != 0xffffffffU)` in comma form | **0** | **1,996** | byte-exact |
| `expect(race<4,1) { assume(...); if (type==13) ... }` | **0** | **1,996** | nested form, byte-exact (landed) |
| same nested, `assume(type != 0x7fffffffU)` | **0** | **1,996** | byte-exact |
| nested, no `expect` on `race < 4` | 1,718 | 1,984 | branch layout/RA changes |
| one `__builtin_assume` only (cost 5 <= 6) | 1 | 1,996 | still threads |
| two assumes in the comma form | **0** | **1,996** | also byte-exact |

The `&`-mask form (`(type < 15) & (mask != 0)`) also defeats
`JumpThreading` (the false edge of an `and i1` is unconstrained for LVI),
but the mask `and i32` is selected in the first split MBB and the
`cmp ...,0` in the second, so `optimizeCompareInstr` emits `ands`
instead of the ISel `and_su`/`CMPZ` pattern's `tst` — one byte off at
`0x97364`.  The `llvm.assume` barrier avoids that entirely.

## Gate

| metric | before | after |
|---|---:|---:|
| `createMission` raw / aligned | 1 / 1 | **0 / 0** |
| size / insn | 1,996 / 744 | 1,996 / 744 |
| `byte_exact` | 4507 / 4524 | **4508 / 4524** |
| `linked_exact` | 4507 / 4524 | **4508 / 4524** |
| `.text` differing bytes | 71,221 | **71,220** |
| `.ARM.extab` differing bytes | 577 | 577 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rodata` size delta | 485 | 485 |
| `.rel.plt` | byte-identical | byte-identical |

`check` RATCHET PASS (GAIN byte_exact 4507 -> 4508, linked_exact
4507 -> 4508); lint CLEAN; drift 0; data 843/843; imports 207; symbol
parity 5370/5370.  `tools/verify_baseline.json` is left at 4507 per the
worker instruction; the ratchet reports the GAIN.

## Reproduction

```sh
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# raw=0 aligned=0 size=1996/1996 insn=744/744
```
