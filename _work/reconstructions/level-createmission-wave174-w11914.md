# w11914 (wave 174): `Level::createMission` — faithful bool/order shape is
# size-exact as `bool alienWave + bool pirateFortress + bool-first guards +
# convoy 1,0,2` (17,172 B, exact 18-byte guard, insn 6197/6197), but raw
# regresses 5,421 -> 7,147; no size-exact raw win, nothing landed.

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11914`, branch
`wave174-w11914`, base master `be9dd429b` (wave 173).  Continues
`level-createmission-wave173-w11814.md` (PEI spill-slot rotation, landed
5,425 -> 5,421) and `level-createmission-wave172-w11711.md` (bool-first
operand order, exact guard but raw 7,147).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5421   align 940   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## The faithful shape

The original's flag block is `bool`-typed, not `int`:

* `b2e04` (`i == first && alienWave`) is the branchless `and` form
  `subs.w r0,fp,r3 / mov r1,r0 / it ne / movne r1,#1 /
  ldr r2,[sp,#60] / eor r2,#1 / orrs r1,r2 / bne` — `eor #1` is the
  i1 `!` idiom.
* `b2f2a` (`i == first && pirateFortress`) is the same form with
  `ldr r1,[sp,#56] / eor r1,#1 / orrs r0,r1 / bne`.
* The three flags are written with 4-byte `str` because clang promotes
  the i1 values to i32 spill slots (they are used as ints in
  `alienGuns = alienWave ? (bigAlienWave == 1 ? 8 : 7) : 0` and
  `(bigAlienWave + (i < 0))`).

The faithful source shape is therefore:

```cpp
bool bigAlienWave = false;
bool alienWave = false;
bool pirateFortress = false;
if (*bigShipsCount <= 0 || localRace != 0) {
    if (localRace == 1 && *bigShipsCount >= 1 && Globals::rnd->nextInt(100) <= 29)
        pirateFortress = Globals::status->getCurrentCampaignMission() > 140;
} else {
    if (Globals::rnd->nextInt(100) <= 29) {
        if (Globals::rnd->nextInt(100) <= 29)
            bigAlienWave = Globals::status->getCurrentCampaignMission() > 103;
        alienWave = 1;
    }
}
...
if (alienWave && i == this->friendCount + this->field_164) {      // bool first
...
} else if (pirateFortress && i == this->friendCount + this->field_164) {
...
```

`bool first` is required: `if (i == first && alienWave)` (icmp first)
instcombines to `or (i != first), !alienWave` and the backend lowers it
with a short-circuit branch; `if (alienWave && i == first)` keeps
`and i1 (phi), (icmp)` and if-converts to the original's 18-byte guard.

## The wave-173 experiment was size-fatal 17,184

Reproduced on the wave-174 base with the all-three-bool shape plus the
outer guard operand swap:

| shape | size | raw | aligned | insn |
|---|---:|---:|---:|---:|
| all three `bool`, guard `localRace != 0 \|\| *bigShipsCount <= 0` | **17184** | 15870 | 3389 | 6206/6197 |
| all three `bool`, guard operand swap + bool-first alien, convoy 0,1,2 | **17184** | 13376 | 2906 | 6202/6197 |
| `int` flags, guard operand swap only | **17184** | 13682 | 2889 | 6204/6197 |

The `bool,guard` pair alone is already size-exact at 17,172 (raw 15870);
the +12 B that makes it 17,184 comes from the convoy default order or the
second guard's bool-first rewrite.

## Size-exact faithful variants (all 17,172 B)

| shape | raw | aligned | insn |
|---|---:|---:|---:|
| **HEAD (`int` flags, `int`-first guards, convoy 2,0,1)** | **5421** | **940** | 6204/6197 |
| `bool alienWave` + `bool pirateFortress` + bool-first + convoy 1,0,2 | 7147 | 965 | 6202/6197 |
| `bool alienWave` only + convoy 1,0,2 | 6779 | 1013 | 6197/6197 |
| all three `bool` + bool-first + outer `case 2` first | 10400 | 904 | 6208/6197 |
| all three `bool` + bool-first + inner `case 2` first + convoy 0,1,2 | 11374 | 1008 | 6201/6197 |
| all three `bool` + guard operand swap + outer `case 2` first | 15869 | 3385 | 6206/6197 |

`bool alienWave` + `bool pirateFortress` + bool-first + convoy 1,0,2 is
the faithful shape with the *exact* 18-byte guard and the original's
shared switch tail; it is size-exact but costs +1,726 raw bytes
(`.text` 80,897 -> 82,623).  `bool alienWave` only + convoy 1,0,2 is the
w11711 shape (insn 6197/6197, raw 6,779).

## Full sweep (size-exact rows only, best first)

224 combinations of `{int, bool alienWave, bool pirateFortress,
bool bigAlienWave, ...}` x `{icmp-first, bool-first}` x
`{convoy 2,0,1 / 1,0,2 / 0,1,2 / 0,2,1 / 1,2,0 / 2,1,0 / 2,0,1}`:

```text
HEAD / convoy 2,0,1 / bool-first(any)          17172  5421  940
convoy 2,0,1 (other operand orders)            17172  5421  940
convoy 2,1,0                                   17172  5424  937
convoy 0,2,1                                   17172  5427  940
convoy 1,2,0                                   17172  5428  935
bool alienWave only + convoy 1,0,2             17172  6779 1013
bool pirateFortress only + convoy 1,0,2        17164  7103  978
bool alien+pirate + bool-first + convoy 1,0,2  17172  7147  965
bool alien+big + bool-first + convoy 1,0,2     17180  6928 1027
bool big only + convoy 1,0,2                   17172  6789 1028
bool alien+pirate+big + bool-first             17180  5815  953
bool alien+pirate+big + bool-first + convoy 0,1,2  17172  11374 1008
bool alien+pirate+big + bool-first + outer case2   17172  10400  904
```

Nothing in the bool/order space beats 5,421 raw.  The closest
non-winning size-exact shapes are `convoy 2,1,0` (raw 5,424, aligned 937)
and inner `case 2` first (raw 5,422, aligned 935); both trade +1..3 raw
for -3..5 aligned.

## Why the bool shape cannot win raw

The bool type gives the exact guard but lets LLVM prove `alienWave` and
`bigAlienWave` are zero on the pirate branch, so the redundant spill
stores the original keeps at `b0f1e` (`str r0,[sp,#60]`) and `b0f26`
(`str r0,[sp,#64]`) are deleted.  The block at `b0f30` then sits 14 B
earlier and the whole `b0f3e..b1a00` region shifts; a second 2 B RA
difference (`adds r1,#80` vs `add.w r3,r1,#80`) shifts the else block.
Net raw 7,147 despite the byte-identical guard.

Attempts to keep the stores alive (all measured, all fail):

* explicit `bigAlienWave = false; alienWave = false;` in both branches
  (`explicitinit`): 17,172 B / raw 13,496 (the source stores are folded
  and the branch layout changes further);
* nested pirate `if` (`nestpirate`): no change;
* `bool alienWaveB = alienWave != 0;` temporaries: size collapses to
  16,316 B (clang deletes the whole flag block);
* `int` flags + `(bool)` / `!!` / `!= 0` bool-first guards: identical to
  HEAD (6,204 insns), the cast is canonicalised back to `icmp ne i32`.

The remaining lever is the register allocator's rematerialisation of the
zero phi, not a source-level store.

## `int offset[3]` experiment (aligned-only win, not landed)

Replacing the convoy `float offset[3]` with `unsigned offset[3]` and the
float literals with their bit patterns keeps size 17,172 B and improves
*aligned* 940 -> 795, but raw rises 5,421 -> 5,425 because clang
synthesises `add.w r1,r1,#0x800000` (`12000.0f` from `6000.0f`) and
`#0x1f4000` (`-30000.0f` from `-36000.0f`) instead of the original's
separate literal loads.  Not a raw win.

## Result

No size-exact variant of the faithful bool/order shape beats the 5,421
raw baseline.  `src/` is left at the base; this note is the deliverable.
Best faithful size-exact shape (if the fleet ever prefers the exact guard
over raw): `bool alienWave` + `bool pirateFortress` + bool-first guards +
convoy default store order 1,0,2 -> 17,172 B / raw 7,147 / aligned 965 /
insn 6202/6197 (the 18-byte guard region is byte-identical; the total
instruction count is 5 above the original's 6197 because of the wave-173
operand-order change elsewhere).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w11914/`
(`variant.py` variant generator, `sweep2.py`/`sweep2.log` 224-shape
matrix, `level_base.ll`/`level_bf.ll` optimized IR, `orig.txt`/
`ours_base.txt` disassembly).
