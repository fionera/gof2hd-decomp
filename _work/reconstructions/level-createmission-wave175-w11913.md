# w11913 (wave 175): `Level::createMission` — size-exact 3-slot rotation FOUND
# (`pirateFortress == 1` + move `int guns` after the spawnPosition `if`),
# but it costs +475 raw; nothing landed.

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11913`, branch
`wave175-w11913`, base master `5f599a137` (wave 174).  Continues
`level-createmission-wave173-w11814.md` (PEI spill-slot rotation, landed
5,425 -> 5,421) and `level-createmission-wave174-w11914.md` (faithful bool
shape, not landed).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5421   align 940   insn 6204/6197
slots (stationPirates / blueprint / extraAliens / fortressGuns) = 44 / 36 / 40 / 48
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## Verified slot map (direct disassembly of the original)

```text
b06b4: str r0, [sp, #48]     -> stationPirates = sp+48
b0742: str r0, [sp, #36]     -> blueprint      = sp+36   (NOT in the cycle)
b0848: str r0, [sp, #44]     -> extraAliens    = sp+44
b2a5e: str.w r9, [sp, #40]   -> fortressGuns   = sp+40
```

Ours (HEAD): SP=44, BP=36, EA=40, FG=48 — exactly the 3-cycle
(SP/EA/FG), with `blueprint` correct in both.  The wave-175 task text's
"`blueprint` 48/44" is the wave-172 naming slip; the cycled variable is
`stationPirates`.

## Real MBFI weights (this is what StackSlotColoring actually sorts on)

The wave-173 note's weights came from propagating the printed successor
probabilities; those are NOT the weights SSC uses.  The real ones are
printed by

```text
-mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv
```

which emits **5** dumps; **dump index 1 (the 2nd)** is the one
`StackSlotColoring` runs with.  Matching each dump block's frequency
against the pre-SSC `%stack.N` reference blocks (`pre_ssc.txt`) gives:

```text
HEAD (dump 1):
  stationPirates 0.243176  blocks {56:0.15625, 193:0.063476, 273:0.009601,
                                   275:0.0048, 286:0.004524, 292:0.004524}
  extraAliens    0.240537  blocks {81:0.058594, 83:0.048828, 84:0.048828,
                                   193:0.063476, 284:0.007239, 286:0.004524,
                                   292:0.004524, 298:0.004524}
  fortressGuns   0.263621  blocks {193:0.063476, 537:0.0036, 255:0.006,
                                   258:0.18602, 298:0.004524}
```

So clang 7.0.2's real colour order is **FG > SP > EA**
(FG->colour 92 = sp+48, SP->93 = sp+44, EA->94 = sp+40).  The original
needs **SP > EA > FG**.  `fortressGuns`'s weight is dominated by the
fortress loop body `bb.258` (freq 0.18602 = 0.30723 * 0.625 * 0.96875:
62.5 % branch heuristic times the 124/128 loop-backedge heuristic).
To flip the order, FG's weight must drop below EA's 0.240537, i.e. by
>= 0.0231 — which means lowering `bb.258`'s frequency.

## The lever: `pirateFortress == 1` gives a 50/50 branch

`BranchProbabilityInfo::calcZeroHeuristics` only fires for `RHS == 0`,
`RHS == 1 && ICMP_SLT`, and `RHS == -1`; every other constant falls
through to `calcEstimatedHeuristics`, which yields 50/50.  So:

```text
b3398: ldr r3, [sp, #56]   ; pirateFortress     (original)
b339a: cmp r3, #0
b339c: beq.w b3752                              ; 62.5 / 37.5
```

`if (pirateFortress == 1)` keeps the *machine* compare identical
(ISel folds `seteq x,1` to `x` because the value is known 0/1, so the
code is still `cmp #0; bne`) but flips the IR branch probability to
50/50.  `bb.258` then drops to 0.14881, FG's weight to ~0.2264 < EA,
and the slots rotate to exactly the original's SP=48 / EA=44 / FG=40.

`(pirateFortress & 1) == 1`, `(pirateFortress ^ 1) == 0`,
`pirateFortress * 2 == 2`, `pirateFortress + 1 == 2` and
`(pirateFortress | 0) == 1` are all *identical* to `== 1`
(same 17164 / 5899 / 48-36-44-40).  `pirateFortress != 1`,
`!(pirateFortress == 1)` give 17192; `pirateFortress > 1` gives 16856;
`<= 1` / `< 2` give 17172 but the wrong slots (44/36/40/56).

## The size-exact full-rotation shape (FOUND, but raw regresses)

`pirateFortress == 1` alone is **17,164 B** (8 B short).  The 8 B are
lost entirely in the raid block: the compare now uses a low register
(`ldr r4 / cmp r4` instead of `ldr.w fp / cmp.w fp`), which cascades
through the `if (raid)` register allocation and shifts every later
instruction by up to 14 B.

Moving the raid block's `int guns = alienGuns + fortressGuns;` *after*
the `if (raidRace == 8 && ...)` spawnPosition block recovers exactly
8 B and keeps the rotation:

```cpp
    if (pirateFortress == 1) {                 // was: if (pirateFortress)
        ...
    }
    if (raid) {
        int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
        bool hard = Globals::status->hardCoreMode();
        if (raidRace == 8 && Level::initStreamOutPosition &&
            Globals::rnd->nextInt(100) < (hard ? 40 : 20) + Globals::status->getLevel()) {
            this->spawnPosition = this->player->getPosition() / 3.0f;
            this->field_180->setNewCoords(this->spawnPosition);
        }
        int guns = alienGuns + fortressGuns;   // moved here (was before the if)
        ...
```

This also matches the original's structure: the original computes
`guns` at `b37e6` (`add.w r5, sl, fp`), i.e. *after* the spawnPosition
`if`, whereas HEAD computes it at `b3774`.  `int end = ...` or inlining
`alienGuns + fortressGuns` into the loop bound produce the same 17,172.

Measured (all size-exact unless noted):

| shape | size | raw | aligned | insn | slots SP/BP/EA/FG |
|---|---:|---:|---:|---:|---|
| **HEAD** | 17172 | **5421** | **940** | 6204/6197 | 44/36/40/48 |
| `pf == 1` | 17164 | 5899 | 929 | 6205/6197 | 48/36/44/40 |
| `pf == 1` + move `guns` | **17172** | **5903** | 921 | 6207/6197 | **48/36/44/40** |
| `pf == 1` + move `guns` + cargo swap | 17172 | 5897 | 922 | 6207/6197 | 48/36/44/40 |
| `pf == 1` + move `guns` + loop `cba` | 17172 | 5902 | 917 | 6207/6197 | 48/36/44/40 |
| **`pf == 1` + move `guns` + cargo swap + loop `cba`** | **17172** | **5896** | **918** | 6207/6197 | **48/36/44/40** |
| `pf == 1` + cargo swap + loop `cba` (no move) | 17164 | 5892 | 926 | 6205/6197 | 48/36/44/40 |
| `pf == 1` + `__builtin_expect(extraAliens,1)` | 17160 | 6085 | 913 | 6203/6197 | 48/36/44/40 |
| `pf == 1` + `__builtin_expect(alienGuns!=0,1)` | 17180 | 5810 | 865 | 6205/6197 | 48/40/44/36 |
| `pf == 1` + `__builtin_expect(nameLeader,1)` | 17172 | 13721 | 1030 | 6208/6197 | 48/36/44/? |
| `pf == 1` + `__builtin_expect(raid,1)` | 17172 | 14886 | 1937 | 6210/6197 | ? |
| `__builtin_expect(blueprint,1)` | 17172 | 5591 | 938 | 6204/6197 | 40/36/44/48 |
| `bp expect` + `pf == 1` + move `guns` | 17172 | 6080 | 934 | 6207/6197 | 44/36/48/40 |

`cargo swap` = `gun->shipGroup = 1; gun->cargo = nullptr;` (undoes the
wave-173 order); `loop cba` = `friendCount + field_164 + field_168`
(undoes the wave-173 order).  In the `pf == 1` context both help raw by
a few bytes; they are pure operand-order permutations.

## Why this is not a raw win

The shape is size-exact but the cumulative instruction-size delta
wanders between -14 and +2 across the raid block (`cumdelta` of the
`ours.dis` streams): every later instruction sits at a different byte
address than the original's, so `fn_diff`'s same-address raw metric
counts ~475 extra differing bytes even though the *aligned* instruction
diff improves 940 -> 918.  `.text` differing bytes go
81,209 -> 81,684 (whole-file; `sodiff` parity and the ratchet are
unaffected because `createMission` is not byte-exact either way).

The `guns` move is structurally *more* faithful to the original
(original computes `guns` at `b37e6`), so this shape is a reasonable
stepping stone if the fleet ever prefers structure over the raw metric.
It was not landed because AGENTS.md requires a measured win.

## Full sweep (all size-exact unless noted)

* `pf` condition forms: `== 1` 17164/5899; `!= 1` 17192/6335;
  `!(pf == 1)` 17192/6335; `> 1` 16856/15154; `<= 1`/`< 2` 17172/6533
  (wrong slots); `& 1 == 1` / `^ 1 == 0` / `* 2 == 2` / `| 0 == 1` /
  `+ 1 == 2` all 17164/5899; `(bool)pf` / `(bool)pf != 0` 17172/5421
  (identical to HEAD).
* raid-block statement order: all 6 permutations of
  {raidType, hard, guns} that put `guns` before `hard` give 17164/5899;
  the 3 that put `hard` first give 17160/5948.  `guns` after the
  spawnPosition `if` is the only 17,172.
* `int guns = fortressGuns + alienGuns;`, `const int guns`, `int end`,
  inlining the sum into the loop bound, `int guns` before `if (raid)`:
  all 17164 or 17172 with raw >= 5893.
* fortress loop start order (6 perms) and bound order: all 17164
  (raw 5898..5900).
* `fortressGuns` forms (`pf == 1 ? 5 : 0`, `!= 0 ?`, `* 5`,
  `if (pf) fortressGuns = 5`): all 17164/5899.
* `if (raid)` / `if (alienGuns != 0)` / `if (extraAliens >= 1)` /
  `if (stationPirates)` / `if (convoy)` / `if (blueprint)` /
  `if (nameLeader)` / `if (siegedStation)` rewritten as `!= 0` (or
  `> 0`): all identical to HEAD at 17172/5421.
* `int start = stationPirates + guns + pirateEscorts` permutations:
  `SGE`/`GSE`/`GES`/`EGS` 17172/5421, `SEG`/`ESG` 17364.
* pirates-loop bound permutations (`start/extraAliens/pirates`): best
  `sep` (current) 17172/5421; the others 17164/5431..5434.

## Conclusion

A **size-exact shape that rotates all three slots exists**:
`if (pirateFortress == 1)` plus moving `int guns = alienGuns + fortressGuns;`
after the `if (raidRace == 8 ...)` block (optionally with the cargo/loop
operand swaps) -> 17,172 B, slots 48/36/44/40, aligned 918, raw 5,896.
It is not a raw win (HEAD 5,421), so `src/` is left at the base and this
note is the deliverable.

## Commands / scratch

```text
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w11913/`
(`try2.py` + `v*.json` variant sweeps, `slots.sh` slot-offset checker,
`cumdelta.py` cumulative instruction-size delta, `rawdiff.py` per-region
raw diff, `aligndiff.py` per-region aligned diff, `mbfi_dump.txt` /
`mbfi_eq1.txt` real MBFI dumps, `ssc.cpp` / `pei.cpp` / `armfl.cpp`
LLVM 7.0.1 sources).
