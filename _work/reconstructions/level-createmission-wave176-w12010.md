# w12010 (wave 176): `Level::createMission` — `alienGuns` as a bool multiply,
# size-exact 17,172 B and raw 5,285 -> 5,263 (-22)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w12010`, branch
`wave176-w12010`, base master `b21088985` (wave 175).  Continues
`level-createmission-wave175-w11914.md` (landed the faithful bool/order shape
plus globals/rnd/enemies hoists, raw 5,421 -> 5,285, `.ARM.extab` 750 -> 748)
and `level-createmission-wave175-w11913.md` (the `pirateFortress == 1`
3-slot rotation was found but cost +475 raw and was not landed).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline and landed result

```text
                                     size            raw    aligned  insn
HEAD (base b21088985)                17172/17172     5285   958      6212/6197
landed (this pass)                   17172/17172     5263   962      6212/6197
```

Section bytes, measured in this worktree: `.text` differing bytes
**80,166 -> 80,144** (-22); total differing bytes **80,932 -> 80,910**
(-22); `.ARM.extab` **746** unchanged (size- and base-exact); `.rel.plt`
byte-identical (0).  File size exact (2,187,328 B).  Gate after the change:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
**4524/4524**, stub_zero_size **0**, missing **0**, wrong_type **0**, extra
**0**, data_exact **843/843**, imports **207**, lint **CLEAN ✓**, drift
**0**, symbol parity **5370/5370**.  `RATCHET PASS`; no baseline update was
needed because no ratcheted count changed.

## Landed source shape

One line, at `src/game/world/Level.cpp:1858`:

```cpp
    int alienGuns = alienWave * (bigAlienWave == 1 ? 8 : 7);
```

replacing the semantically equivalent

```cpp
    int alienGuns = alienWave ? (bigAlienWave == 1 ? 8 : 7) : 0;
```

`alienWave` is a `bool` (0/1), so both forms are identical C++: 0 when
`alienWave` is false, otherwise 7 or 8 depending on `bigAlienWave == 1`.

## Why it helps: the same lowering, better stack-slot colouring

The original lowers `alienGuns` with two conditional selects, exactly as our
base does (original @ `0xb2a20`):

```text
ldr r0, [sp, #64]      ; bigAlienWave
movs r4, #7
cmp r0, #0
it ne
movne r4, #8
ldr r0, [sp, #60]      ; alienWave
cmp r0, #0
it eq
moveq r4, r0           ; alienGuns = 0 when alienWave == 0
ldr.w r9, [sp, #56]    ; pirateFortress
```

The multiply spelling makes clang 7.0.2 emit the same two-select shape but
changes StackSlotColoring's live-range weights, which moves the spill slots:

```text
                     stationPirates  extraAliens  fortressGuns
original             sp+48           sp+44        sp+40
HEAD (ternary)       sp+44           sp+48        sp+40
landed (multiply)    sp+48           sp+52        sp+44
```

`stationPirates` now matches the original's `sp+48`; the `extraAliens` and
`fortressGuns` slots are still 4/8 bytes high.  The multiply form also
reorders one load in the `pirateFortress` loop guard (`ldr r6, [sp, #100]`
moves between the `cmp` and the `beq.w`), shifting the loop body by 2 bytes.
The net effect is 22 fewer same-address differing bytes against the original;
the function stays exactly 17,172 B.

## Measured variants (all against the wave-175 base)

Every row below was built and measured with `tools/fn_diff.py`; only rows
with `size == 17172/17172` are landable.

```text
variant                                         raw    aligned  size
base ternary                                    5285   958      17172   (baseline)
alienWave * (bigAlienWave == 1 ? 8 : 7)         5263   962      17172   LANDED
alienWave * (bigAlienWave ? 8 : 7)              5263   962      17172
alienWave * (7 + bigAlienWave)                  5263   962      17172
(bigAlienWave == 1 ? 8 : 7) * alienWave         5263   962      17172
split: int g = bigAlienWave == 1 ? 8 : 7;
       if (!alienWave) g = 0;                   5263   962      17172
g *= alienWave                                  5263   962      17172
alienWave ? 7 + bigAlienWave : 0                5285   958      17172
fortressGuns = pirateFortress * 5               5263   962      17172
alienGuns first / fortressGuns first            5263   962      17172
if (alienGuns) / if (alienGuns > 0)             5263   962      17172
int bigAlienWave/alienWave/pirateFortress       11910  1169     17164   (-8)
int flags + gre hoists + extraAliens truthy     5040   -        17176   (+4)
extraAliens truthy (multiply form)              5204   937      17176   (+4)
pirateFortress == 1 (multiply form)             5263   962      17172   (neutral)
bool big = bigAlienWave == 1; ...               5263   962      17172
```

Declaration-order permutations of the three bool flags on top of the
multiply form (only the current order is competitive):

```text
bigAlienWave, alienWave, pirateFortress         5263   962      17172   LANDED
alienWave, pirateFortress, bigAlienWave         9666   956      17172
alienWave, bigAlienWave, pirateFortress        10516   990      17172
bigAlienWave, pirateFortress, alienWave         7945   989      17180
pirateFortress, alienWave, bigAlienWave        12387  1089      17180
pirateFortress, bigAlienWave, alienWave        12568  1019      17188
```

The `alienWave, pirateFortress, bigAlienWave` order is the only one that
restores `fortressGuns` to the original `sp+40` (aligned 956), but its raw
cost (9,666) is far too high.  The multiply form with the existing
declaration order is the best size-exact raw result found.

## Verification commands

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
# -> raw=5263 aligned=962 size=17172/17172 insn=6212/6197
objdump -s -j .rel.plt cmake-build-match/libgof2hdaa.so | tail -n +4 > /tmp/ours.rel
objdump -s -j .rel.plt ../_work/bins/android_2.0.16_libgof2hdaa.so | tail -n +4 > /tmp/orig.rel
diff /tmp/ours.rel /tmp/orig.rel        # empty
cmake --build cmake-build-match --target check -j2
# -> byte 4507 linked 4507 stubs 0 missing 0 extra 0 avg 99.93
#    data 843/843  imports 207  RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12010/`
(`Level.base.cpp`, `sweep2.py`, `buildvar.py`, `orig.dis`, `base.dis`,
`mul.dis`, and the `v*.json` variant sets).
