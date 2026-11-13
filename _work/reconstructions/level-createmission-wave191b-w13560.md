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
landed (pass 2)                      17172/17172     2626    845     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,628 | **2,626** (-2) |
| aligned | 844 | 845 |
| insn | 6206/6197 | 6206/6197 |
| `.text` diff | 71,240 | **71,238** (-2) |
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

## 2b. Pass 2: volatile reference to `Globals::globals` (raw 2,627 -> 2,626)

```cpp
if (raid) {
    AERandom *volatile &rnd = Globals::rnd;
    Globals *volatile &globals = Globals::globals;
    int raidType = globals->getRandomEnemyFighter(raidRace);
```

The volatile read of `Globals::globals` for the `raidType` call drops the
`0x3700` region from 125 to 124 differing bytes (raw -1, `.text` -1).
`Globals::status` through the same volatile-reference spelling is neutral on
top of this.  Scoping the `globals` reference at function scope (covering the
raid block *and* the pirates/station-pirates loops) is worse: raw 2,627 /
aligned 845.  The `rnd` reference is redundant once `globals` is present
(`globals`-only also measures 2,626), but it is kept because it is already
committed and does not interact.

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

Expected: `raw=2626 aligned=845 size=17172/17172`, `.text` diff 71,238,
`.ARM.extab` diff 576, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13560/`
(`tryrun.py`, `tailraw.py`, `normdiff.py`, `dis.py`, `batch_rnd.py`,
`batch_q.py`, `batch_q2.py`, `apply_q4.py`, `run.log`).

## 6. Further tail measurements (all size 17,172 unless noted)

Starting from the pass-2 tree (raw 2,626 / aligned 845):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (pass 2) | 2,626 | 845 | 17,172 |
| raid `Status *volatile &status` on top | 2,626 | 845 | 17,172 |
| `globals` volatile ref scoped over raid+pirates+station-pirates | 2,627 | 845 | 17,172 |
| pirates loop `Globals *volatile &globals` | 2,626 | 845 | 17,172 |
| pirates loop `AERandom *volatile &rnd` | 2,626 | 858 | 17,172 |
| station-pirates `status`/`gameText` volatile refs | 2,626 | 845 | 17,172 |
| extra-aliens `rnd`/`status`/`globals` volatile refs | 2,626 | 845 | 17,172 |
| alien `gun` loaded after `createStaticObject` (semantic fix) | 2,627 | 841 | 17,172 |
| fortress `gun` loaded after `createStaticObject` | 2,632 | 842 | 17,172 |
| both `gun` after | 2,633 | 838 | 17,172 |
| case-4 `hunters` before `opt44` | 2,629 | 839 | 17,172 |
| case-4 `hunterCount` before `new Array` | 2,627 | 836 | 17,172 |
| both | 2,630 | 828 | 17,172 |
| case-4 tail `Globals *volatile &globals` | 2,732 | 838 | 17,172 |
| case-4 tail `Array<KIPlayer *> *volatile &enemies` | 2,782 | 836 | 17,168 |
| case-4 tail `enemies` local unused in loop bound | 2,782 | 836 | 17,168 |
| case-4 tail no `enemies` local | 2,782 | 836 | 17,168 |
| case-4 tail no `globals` local | 2,785 | 838 | 17,168 |

Station-pirates body order — full 24-permutation sweep of
`{C=createStaticObject+store, P=pirateBase, S=setToSleep, N=name}`:

* `SNPC` (current) = 2,628 / 844 / 17,172 on the pre-pass base — best.
* `PSNC` = 2,633 / 825, `PCSN` = 2,634 / 824, `SPNC` = 2,634 / 830.
* Every permutation that puts `C` first (`CPSN` … `CNSP`) is **fatal**
  (size 17,176, raw 3,544–3,560), i.e. `createStaticObject` must be the
  last statement in source order for the frame to stay 17,172.
  Note the binary's order is `C, P, S, N` (createStaticObject, then
  `pirateBase`, then `setToSleep`, then `name`) — semantically ours calls
  `setToSleep`/`name` on the *old* `enemies[i]`, but the faithful order is
  size-fatal, so the current order is kept.

Case-1 `enemyRace` reload: `enemyRace = enemyRace;`, `enemyRace += 0;`,
`int race = enemyRace; enemyRace = race;` in the loop body are all
optimised away (neutral 2,626).  `if (attackerCount < 0) enemyRace = 0;`
is fatal (raw 11,977, size 17,188).

`[sp,#120]` in the case-4 tail is `&Globals::globals` (stored at
`0xb2d1c` from the literal at `0xb2dec`), so the existing
`((AERandom *) Globals::globals)->nextInt(...)` cast is correct — the
`Globals::rnd` spelling used by the w13560 Z1 probe is semantically wrong.
