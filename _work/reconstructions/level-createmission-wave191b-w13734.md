# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13734

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13734`, base master `dc50e1082`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13715.md` (note-only, raw 2,626),
`...w13702.md`, `...w13691.md`, `...w13685.md`, `...w13680.md`, `...w13671.md`,
`...w13650.md`.  Task: fresh angles on the pirateFortress field order, the
case-4 tail store order, the raid volatile aliases and the case-2 loot order.
**No size-exact raw win was found; note-only, `src/` unchanged.**  Three real
*semantic* divergences were located and measured (they cost raw or size, so
none is landable under the 2,626 constraint).

```text
                                     size            raw    aligned  insn
base dc50e1082 (checked in)          17172/17172     2626    864     6206/6197
```

`check` on the unchanged tree: byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `RATCHET PASS`.  `.text` diff **72,647**, `.ARM.extab`
**564** (size/base exact at `0x1a3b78`), `.ARM.exidx` 0, `.rodata` 0,
`.rel.plt` 0.

## 1. Case-4 tail `setPosition` order (faithful order costs +6 raw)

Source lines 2396-2400 emit `setPosition` on `attackerCount+3` before
`attackerCount+2`.  The binary emits `+2` before `+3`:

```text
orig  b36d0 ldr r0,[r0,#4]   ; enemies[sl+1]  (call b36d8)
      b36e8 ldr r0,[r0,#8]   ; enemies[sl+2]  (call b36f0)
      b3700 ldr r0,[r0,#12]  ; enemies[sl+3]  (call b3708)
      b3718 ldr r0,[r0,#16]  ; enemies[sl+4]  (call b3720)
ours  b36e8 ldr r0,[r0,#12]  ; +3 first
      b3700 ldr r0,[r0,#8]   ; +2 second
```

Swapping the two source lines to the binary order:

| variant | raw | aligned | size | `.text` | extab |
|---|---:|---:|---:|---:|---:|
| checked in (`+3` then `+2`) | **2,626** | **864** | 17,172 | **72,647** | 564 |
| faithful (`+2` then `+3`) | 2,632 | 837 | 17,172 | 72,653 | 564 |

The scheduler in the original evidently reordered the independent virtual
calls, so reproducing the binary's emitted order in source is not free.

## 2. Raid `inPirateLootOrbit`: `spawnPosition.z` before `setPosition` (semantic bug, raw +1)

Source lines 2020-2022 call `setPosition(this->spawnPosition)` **before**
`spawnPosition.z += ...`.  The binary updates all three components first and
then calls `setPosition` with the final vector:

```text
orig  b3926 vldr s2,[r4,#404]     ; spawnPosition.z
      b392a ldr.w r0,[r4,#248]    ; enemies (loaded early)
      b392e vadd.f32 s0,s2,s0
      b3932 vstr s0,[r4,#404]     ; z += ...
      b3936 ldr r0,[r0,#4]
      b3938 ldr r0,[r0,sl,lsl#2]
      b393c ldr r1,[r0,#0]
      b393e ldr r2,[r1,#68]
      b3946 blx r2                ; setPosition(spawnPosition)
ours  b38f6 ... blx setPosition   ; before the z block
      b3904 ... z += ...
```

Moving the `setPosition` line after the `z +=` line is size-exact and matches
the binary instruction-for-instruction (only the register choice differs):

| variant | raw | aligned | size | `.text` | extab |
|---|---:|---:|---:|---:|---:|
| checked in (`setPosition` before `z`) | **2,626** | **864** | 17,172 | **72,647** | 564 |
| faithful (`z` before `setPosition`) | 2,627 | **834** | 17,172 | 72,648 | 564 |

Net **+1 raw / -30 aligned**.  The +1 is a global greedy-RA artifact: the
faithful shape shifts the raid body 4 B earlier (setup `b3752..b37e2` is the
same length, but the body's register allocation changes), and the 4-byte
shift inside the `inPirateLootOrbit` block costs one more byte than the
reordered stores save.  Six bytes become newly mismatched
(`0xb38e0, 0xb3905, 0xb392f, 0xb3931, 0xb3933, 0xb3934`) and five are fixed
(`0xb38df, 0xb38e2, 0xb3901, 0xb3918, 0xb3937`).

### 2a. Statement order of the three `inPirateLootOrbit` head calls

The binary emits `setHitpoints` (b384c), then `setSpeed` (b3860), then
`setRotate` (b386e).  The source order is `setRotate; setSpeed; setHitpoints`.
All six permutations were measured (base and on top of the faithful z fix):

| source order | raw (base) | aligned (base) | raw (z fix) | aligned (z fix) |
|---|---:|---:|---:|---:|
| **RSH (checked in)** | **2,626** | **864** | **2,627** | **834** |
| SRH | 2,627 | 869 | 2,628 | 839 |
| RHS | 2,634 | 863 | 2,635 | 833 |
| SHR | 2,634 | 868 | 2,635 | 838 |
| HRS | 2,634 | 863 | 2,635 | 833 |
| HSR | 2,634 | 854 | 2,635 | 824 |

The compiler reorders the independent calls; the checked-in text order is the
unique raw minimum.

### 2b. `volatile` aliases (all four combinations + pointer forms)

| alias form | raw | aligned | size |
|---|---:|---:|---:|
| **`AERandom *volatile &rnd` + `Globals *volatile &globals` (checked in)** | **2,626** | **864** | 17,172 |
| `rnd` plain ref | 2,627 | 864 | 17,172 |
| `globals` plain ref | 2,627 | 863 | 17,172 |
| both plain refs | 2,628 | 863 | 17,172 |
| `rnd` volatile pointer value | 4,221 | 1,275 | 17,168 |
| `globals` volatile pointer value | 3,650 | 1,001 | 17,196 |
| both volatile pointer values | 4,231 | 1,275 | 17,168 |
| `rnd` plain pointer value | 4,203 | 860 | 17,152 |
| `globals` plain pointer value | 2,627 | 863 | 17,172 |

The checked-in pair of volatile references is the unique raw minimum.

## 3. `stationPirates`: `setToSleep`/`name` are applied to the wrong object (semantic bug, size +4)

Source lines 2040-2047 apply `setToSleep()` and `name = getText(441)` to the
**old** `enemies[i]` ship and only then overwrite `enemies[i]` with
`createStaticObject(...)`.  The binary overwrites first and then applies both
calls to the new static object:

```text
orig  b3b22 Route::getWaypoint
      b3b2e createStaticObject(route->getWaypoint(),14243,true)
      b3b38 str enemies[i]
      b3b3e Route::getWaypoint
      b3b4c getPosition -> pirateBase
      b3b54 KIPlayer::setToSleep     ; on the NEW object
      b3b70 GameText::getText(441)
      b3b80 String::operator=        ; on the NEW object
ours  b3b16 setToSleep               ; on the OLD ship
      b3b30 getText
      b3b42 String::operator=        ; on the OLD ship
      b3b46 getPosition -> pirateBase
      b3b62 createStaticObject / store
```

Reordering to the faithful sequence (create; store; pirateBase; setToSleep;
name) is **size +4** (17,176), because the prologue must hoist
`&pirateBase` into `fp` and the allocator computes `first` into `ip` instead
of `r4` (`add.w ip,...; str.w ip,...; mov r4,ip` = 14 B vs the binary's
`add r4,...; str r4,...; add.w r9,...` = 10 B).  All six body orders and all
prologue/loop-form variants were measured:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| checked in (old-object calls) | **2,626** | **864** | **17,172** |
| faithful create-first, all 6 tail orders | 3,541-3,557 | 833-864 | **17,176** |
| faithful + `pirateBase` before/after `first` | 3,557 | 833 | 17,176 |
| faithful + `const`/`long`/`unsigned first` | 3,557 | 833 | 17,176 |
| faithful + `Vector pirateBase(x,y,z)` / `Vector{...}` | 3,557 | 833 | 17,176 |
| faithful + `for`/`++i`/`i+=1`/`unsigned` | 3,557 | 833 | 17,176 |
| faithful + `while` / `for(;;)` | 3,604 | 950 | 17,192 |
| faithful + `do/while` | 3,703 | 1,048 | 17,188 |
| faithful, `first` removed (inline both uses) | 3,596 | 759 | 17,152 |

The +4 is not recoverable by any prologue or loop-form spelling tried.

## 4. Combination sweep

| combination | raw | aligned | size | `.text` | extab |
|---|---:|---:|---:|---:|---:|
| base | **2,626** | **864** | 17,172 | **72,647** | 564 |
| raid z fix | 2,627 | 834 | 17,172 | 72,648 | 564 |
| case-4 `+2/+3` swap | 2,632 | 837 | 17,172 | 72,653 | 564 |
| raid z + case-4 | 2,633 | **807** | 17,172 | 72,654 | 564 |
| stationPirates faithful | 3,557 | 833 | **17,176** | 873,001 | 150,459* |
| stationPirates + raid z | 3,558 | 803 | 17,176 | 873,002 | 150,459* |
| stationPirates + case-4 | 3,558 | 806 | 17,176 | 873,002 | 150,459* |
| stationPirates + raid z + case-4 | 3,559 | 776 | 17,176 | 873,003 | 150,459* |

`*` the section diffs are large only because the +4 size shift moves every
later function; the function-local extab record is unchanged.

## 5. Conclusion

The checked-in shape remains the unique raw minimum (2,626 / 864 / 17,172).
The three semantic divergences found are real but not landable under the
current raw constraint: the raid z fix is size-exact but +1 raw, the case-4
faithful order is +6 raw, and the stationPirates faithful order is +4 size.
`src/` is left unchanged.

## 6. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13734
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=864 size=17172/17172 insn=6206/6197`,
`.text` diff 72,647, `.ARM.extab` 564, `.ARM.exidx` 0, `.rodata` 0.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13734/`
(`adiff.py`, `sdiff.py`, `calls.py`, `secdiff.py`, `sweep_station.py`,
`sweep_station2.py`, `sweep_prologue.py`, `sweep_loopform.py`,
`sweep_nofirst.py`, `sweep_raidorder.py`, `sweep_raidorder2.py`,
`sweep_volatile.py`, `sweep_combo.py`, `Level.base.cpp`, `base.so`, `raid.so`).
