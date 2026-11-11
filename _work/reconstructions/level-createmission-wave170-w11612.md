# w11612 (wave 170): `Level::createMission` — drop the cached `Status *status`
# local in the raid block (raw 5,435 -> 5,425; 17,172 B; aligned 767 -> 936)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11612`, branch
`wave171-w11612`, base master `50716a763`.  Continues
`level-createmission-wave169-w11512.md` (bool `alienWave` / sp84+sp88 re-check)
and `level-createmission-wave166-w11111.md` (frame map).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## Landed: remove the cached `Status *status` local (raw -10)

The raid prologue used

```cpp
Status *status = *(Status *volatile *) &Globals::status;
int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
bool hard = status->hardCoreMode();
int guns = alienGuns + fortressGuns;
if (raidRace == 8 && Level::initStreamOutPosition &&
    Globals::rnd->nextInt(100) < (hard ? 40 : 20) + status->getLevel()) {
```

The original dereferences `Globals::status` **twice**, independently:

```text
b3798  ldr.w r0, [r8]        ; Globals::status -- hardCoreMode()
b379c  blx   hardCoreMode
...
b37bc  ldr   r0, [sp,#108]   ; &Globals::status, reloaded
b37be  movs  r5, #20
b37c0  cmp.w r8, #0
b37c4  ldr   r0, [r0,#0]     ; Globals::status -- getLevel()
b37ca  blx   getLevel
```

i.e. the original source has no `Status *status` cache; it writes
`Globals::status->hardCoreMode()` and `Globals::status->getLevel()` directly.
Dropping the local reproduces the two independent derefs and removes the
`ldr r0,[sp,#108]; ldr.w fp,[r0]` hoist:

```text
b376a  ldr   r0, [sp,#108]
b376c  ldr   r0, [r0,#0]
b376e  blx   hardCoreMode
...
b37bc  ldr   r0, [sp,#108]   ; second independent deref
```

Measured: **raw 5435 -> 5425 (-10)**, size exact 17,172 B, insn 6204/6197.
Aligned rises 767 -> 936: the freed register forces a short shuffle
(`mov r1,r4; add r4,fp` for `guns`) and the extra `ldr` pair shifts the
raid-block register allocation; the aligned diff is dominated by branch
targets (`operand:b.n` 119, unchanged) and `operand:ldr` 129 -> 126.

## Sweeps measured this wave (all size-exact unless noted)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (cached volatile `status`) | 17172 | 5435 | 767 |
| **drop `status` local (landed)** | 17172 | **5425** | 936 |
| drop local + `guns` first | 17172 | 5425 | 936 |
| drop local + `raidType,guns,hard` | 17172 | 5425 | 936 |
| drop local + `hard` first | 17172 | 5448 | 931 |
| `Status *status = Globals::status` (non-volatile) | 17172 | 5443 | 770 |
| `int pirateFortress` -> `bool` | 17152 | 12471 | 974 |
| `int bigAlienWave` -> `bool` | 17156 | 12426 | 773 |
| `bool alienWave` | 17156 | 12403 | 818 |
| `bool raid` | 17172 | 5663 | 779 |
| `int nameLeader` | 17180 | 13441 | 870 |
| `unsigned stationIndex` | 17172 | 5435 | 767 |
| `RaidCoords3` -> `int raidCoords[3]` | 17172 | 5435 | 767 |
| flags uninitialised + explicit zeros in both branches | 17172 | 5435 | 767 |
| flags uninit + zeros only in `else` | 17156 | 11333 | 849 |
| flags uninit + zeros only in `if` | 17132 | 12778 | 867 |
| flag decl order `PF,BAW,AW` | 17180 | 11985 | 844 |
| convoy default source order `1,0,2` | 17188 | 11346 | 805 |
| `int offset[3]` + hex literals, default `2,0,1` | 17172 | 5855 | 791 |
| `int offset[3]` + hex literals, default `1,0,2` | 17164 | 11631 | 823 |
| `int offset[3]` + hex literals, all cases `1,0,2` | 17164 | 11214 | 807 |
| remove the alien-gun `end` hack (faithful bound) | 17172 | 5633 | 690 |
| drop `status` local + remove `end` hack | 17172 | 5670 | 761 |
| remove `(bigAlienWave + (i < 0))` ternary hack | 17156 | 6544 | 1221 |
| drop `status` local + remove turret hack | 17148 | 6566 | 1234 |

## Notes for the next pass

* The `end` hack removal is the best **aligned** shape seen (690 vs 767) and
  reproduces the original's single `cmp fp,r3` loop bound, but it is raw
  +198 because it shifts the alien-gun loop latch by 6 B (the `i` variable
  spills to `sp+116`; the original keeps it in `fp`).  A pass that can pin
  `i` to `fp` (i.e. stop `this` from taking `fp`) should land both.
* The turret-table `(bigAlienWave + (i < 0))` hack is provably wrong: the
  original is a plain `cbz bigAlienWave` (0x32ee).  Removing it is -16 B, so
  the missing 16 B are elsewhere in the alien-gun loop.
* The convoy default body is the only case whose machine store order
  (`0,2,1`) differs from the original's shared GPR tail (`1,0,2`); source
  order `1,0,2` merges all four cases but LICM hoists the constants into
  `s16..s22` and the tail becomes `vstr s0` (+16 B).

## Commands

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11612/{regions.py,regions2.py,idis.py,dump.py,try*.py,measure.sh,Level.cpp.base}`.
