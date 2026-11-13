# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13560

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13560`, base master `2baef717d`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13537.md` (alien-gun tail) and
`...wave191b-w13538.md` (case-183 expect + case-2 volatiles).

```text
                                     size            raw    aligned  insn
base 2baef717d                       17172/17172     2628    844     6206/6197
landed (pass 1)                      17172/17172     2627    844     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,628 | **2,627** (-1) |
| aligned | 844 | 844 |
| insn | 6206/6197 | 6206/6197 |
| `.text` diff | 71,240 | **71,239** (-1) |
| `.ARM.extab` diff | 576 | 576 |
| `.rodata` diff | 485 | 485 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| `.ARM.exidx` diff | 0 | 0 |

`check`: **RATCHET PASS** (byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

## 2. The change: volatile reference to `Globals::rnd` in the raid block

```cpp
if (raid) {
    AERandom *volatile &rnd = Globals::rnd;
    int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
    ...
    rnd->nextInt(100) ...
```

All seven `Globals::rnd->` uses inside the `if (raid)` block go through the
volatile reference.  The volatile read forbids CSE of the *pointer value*
across the block, which reorders the raid loot body's register allocation:
the `0x3800` region drops from 223 to 222 differing bytes.  `const volatile`
(`AERandom *const volatile &rnd`) measures identically.  Plain
`AERandom *rnd = Globals::rnd;` is **fatal** (raw 4204, size 17,152) and
`volatile AERandom *rnd` (pointer-to-volatile, not volatile pointer) does
not compile against the existing `Globals::rnd` type.

## 3. The `[sp,#100]` slot is `&Globals::rnd`, not `Globals::rnd`

`0xb01bc` loads the GOT entry `0x2101f8` (`_ZN7Globals3rndE`) and stores it
to `[sp,#100]`; the following `ldr r0,[r0,#0]` dereferences it to get the
`AERandom *` for `Globals::rnd->reset()`.  So `[sp,#100]` holds the
**address** of the global pointer (a rematerializable GOT address), which is
why LLVM can hoist that load freely.

Consequently the "extra dead `ldr r4,[sp,#100]`" at `0xb3234` is a hoisted
reload of `&Globals::rnd` for the first raid-block `nextInt` use
(`0xb378c` becomes `ldr r0,[r4,#0]` where the original does
`ldr r0,[sp,#100]; ldr r0,[r0,#0]`).  It is not `this`; `this` lives in
`%stack.73` (`[sp,#124]`) and is loaded once at `0xb3232` in both binaries.
The alien/fortress register permutation (`this` r8 vs r4, `alienGuns` r6 vs
sl, `gun` r4 vs r6, `i` sl vs fp) is a downstream effect of that hoist.

## 4. Measurements (all size 17,172 unless noted; raw/aligned)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,628 | 844 | 17,172 |
| **raid `AERandom *volatile &rnd` (landed)** | **2,627** | 844 | 17,172 |
| `AERandom *const volatile &rnd` | 2,627 | 844 | 17,172 |
| `AERandom *&rnd` / `AERandom *const &rnd` | 2,628 | 844 | 17,172 |
| `AERandom *rnd = Globals::rnd;` at raid top | 4,204 | 841 | 17,152 |
| `AERandom *volatile rnd = Globals::rnd;` | 4,232 | 1,276 | 17,168 |
| `AERandom *rnd` after `hard`, all 7 uses | 4,314 | 917 | 17,168 |
| `AERandom *rnd` after `int guns`, all 7 uses | 4,314 | 917 | 17,168 |
| `rnd` local, only the `if`-condition use | 2,628 | 844 | 17,172 |
| `int roll = Globals::rnd->nextInt(100);` before the `if` | 4,175 | 855 | 17,164 |
| nested `if (raidRace==8 && initStreamOutPosition) { int roll = ...; if (roll < ...) }` | 2,628 | 844 | 17,172 |

Alien-loop selector (`const float (*t)[6] = ...`): plain `bigAlienWave`,
`bigAlienWave != 0`, `bigAlienWave == 1` are all **fatal** (raw 4,381,
size 17,168).  The `(bigAlienWave + (i < 0) + (i < 0))` anchor is a local
optimum (w13537 §3 / w13538 §4).

Alien loop bound: `alienGuns + this->friendCount + ...` neutral (2,628);
`this->field_168 + this->field_164 + this->friendCount + alienGuns` 2,629
(worse).  `int alienEnd = ...` hoisted out of the loop is fatal
(raw 5,610, size 17,184).

Fortress `Vector p`: declaring `p` before `v`/`base` (block scope) is fatal
(raw 4,307, size 17,196); `Vector p; p = ...` in the loop is fatal
(raw 16,017, size 17,232).

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13560
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2627 aligned=844 size=17172/17172`, `.text` diff 71,239,
`.ARM.extab` diff 576, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13560/`
(`tryrun.py`, `tailraw.py`, `normdiff.py`, `dis.py`, `batch_rnd.py`,
`batch_q.py`, `batch_q2.py`, `apply_q4.py`, `run.log`).
