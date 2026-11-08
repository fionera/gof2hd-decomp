# w9013 (wave-145): `Level::createMission` — the wave-144 "4-PHI diamond"
# hypothesis is WRONG; the fold is `HoistThenElseCodeToIf`/`HoistTerminator`,
# not `FoldTwoEntryPHINode`

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9013`, based on master
`eb13ada9b` (wave 144). Continues `level-createmission-wave144-w8912.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w9013_fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5592
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## NEW result — the fold is NOT `FoldTwoEntryPHINode` (its `NumPhis > 2`
## check is dead for this shape)

Wave 144 proposed "a diamond with three PHIs in the merge block
(`FoldTwoEntryPHINode` bails at `NumPhis > 2`)". That is **falsified**:

* `FoldTwoEntryPHINode` (LLVM 7.0.2, `SimplifyCFG.cpp:2292`) allows 3 PHIs and
  bails only at 4+, but the real folder for a clean if/else diamond is
  `HoistThenElseCodeToIf` → `HoistTerminator` (`SimplifyCFG.cpp:1243/1330`),
  which creates a `select` for **every** PHI with `BB1V != BB2V` and has **no
  PHI-count limit at all**.  `SpeculativelyExecuteBB` likewise has no limit.
* Empirically (standalone, same flags): a diamond with **4, 5, 6 and 7 live
  PHIs** is still folded to 4/5/6/7 `select`s in the first "Simplify the CFG"
  pass.  A 4-PHI diamond does **not** keep the branch.
* In the real `createMission`, `if (bigAlienWave) {t=A; d1=..; d2=..; d3=..;
  gun->shipGroup=1;} else {t=B; ...; gun->shipGroup=localRace;}` (4-5 live
  PHIs + *different* stores in the arms) still folds to selects and is hoisted
  to the loop preheader.  Size/raw unchanged from candidate A.

## What actually prevents the fold

`HoistThenElseCodeToIf` returns false iff the arms' first non-debug
instructions are not `isIdenticalToWhenDefined`.  If they differ, the diamond
survives the *hoist* path — but `FoldTwoEntryPHINode` (called on the merge
block) then folds the PHIs unless `DominatesMergePoint` fails for an incoming
value (i.e. a non-speculatable arm instruction) or `GetIfCondition` fails.

This explains the one shape that *does* emit the branch — the **`volatile`
pointer** probe from wave 144:

```cpp
const float (*volatile t)[6];
if (bigAlienWave) t = kAlienStationTurrets; else t = kAlienOutpostTurrets;
```

The arms contain two *different* `store volatile` instructions, so
`HoistThenElseCodeToIf` bails; and because the variable is volatile there is no
promotable PHI at all (the merge block reloads `t`), so
`FoldTwoEntryPHINode` has nothing to fold.  The branch survives and becomes
`ldr r0,[sp,#64]; cbz`.  Cost: `str`+`ldr` of `t` (+8 B), which the original
does not have (`mov sl,r1` at the join keeps `t` in a register).

## Candidate shapes tried this wave (all in the real function, `-Oz`)

| shape | result |
|---|---|
| plain ternary `bigAlienWave ? A : B` | select, hoisted to preheader (17136 B) |
| `if/else` | select, hoisted |
| `switch {case 0/default}` (2- and 3-case) | select, hoisted |
| `if/else` + 4 dead dummy ints | still select (dummies DCE'd) |
| `if/else` + 4 live dummy ints (used in calls) | still 4 selects, hoisted |
| `if/else` + 4-5 live PHIs + different `shipGroup` stores in arms | still selects |
| `if/else` with `if (1)` nested in an arm | still select |
| `t = B; if (bigAlienWave) t = A;` | still select |
| `volatile` pointer | **branch + cbz** but +8 B (17,180) |

No non-volatile shape found that keeps the diamond as control flow while
keeping 17,172 B.  The remaining untried lever is a *non-volatile* arm
instruction that is both (a) different between the arms (to stop
`HoistThenElseCodeToIf`) and (b) non-speculatable (to stop
`FoldTwoEntryPHINode`) **without** a store/reload of `t` — e.g. a `volatile`
load of a table pointer from a global.  That would show a load in the arm that
the original does not have, so it is unlikely to be the witnessed source.

## Gate counts (before -> after)

No source change landed; gate re-run is identical.

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv raw diff 5592 -> 5592
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9013_fdiff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9013/{cc.sh,aliencheck.sh,try.sh,mkvar.py,cand_*.txt}`,
`/dev/shm/w9013/t/{x*.cpp,y*.cpp,y6dump.txt}`.  LLVM 7.0.2 Android sources:
`/dev/shm/w9013/{SimplifyCFG_android.cpp,PassManagerBuilder.cpp}`.
