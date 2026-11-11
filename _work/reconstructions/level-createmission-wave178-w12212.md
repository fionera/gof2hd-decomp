# `Level::createMission` — wave 178 / worker w12212

Target `_ZN5Level13createMissionEv` (17,172 B) in `src/game/world/Level.cpp`.
Branch `wave178-w12212`, base master `1b48f990f` (post-wave-177 merge).

**Exit: no code change landed.**  The wave-177 shape is a local optimum on the
`fn_diff` raw metric.  Baseline in this worktree:

```
_ZN5Level13createMissionEv   raw=5143  aligned=952  size=17172/17172  insn=6208/6197
```

`src/` is left at the base.  The gate is unchanged and green (see bottom).

This note records the bool/order sweep, the measured variants, and one
structural finding that is worth carrying into the next wave: the
`__builtin_expect(extraAliens >= 1, 1)` hint is what forces the extraAliens
loop to an **unsigned** compare, while the original uses a **signed** one.
Removing the hint drops raw by 658 B but costs +4 B of size via a
register-allocation swap; every compensating shape tried either breaks size
exactness or loses the raw gain.

## What was swept

All variants are real `link_so` builds (`-j2`, `ulimit -v 12000000`) measured
with `tools/fn_diff.py`.  "size" is `ours/orig`.

### 1. `bool` vs `int` / `char` for the three wave flags

The three flags are declared (base) as

```c++
    bool bigAlienWave = false;
    bool alienWave = false;
    bool pirateFortress = false;
```

Declaration-order permutations (all `bool`, all size-exact except where noted):

| order | raw | aligned | size |
|---|---:|---:|---:|
| **BAP (base)** | **5143** | **952** | 17172 |
| BPA | 7392 | 984 | 17172 |
| ABP | 10102 | 959 | 17172 |
| APB | 10416 | 951 | 17164 |
| PBA | 12268 | 1022 | 17180 |
| PAB | 12139 | 1021 | 17180 |

`awfirst` (assign `alienWave = 1` before the inner `rnd`/`bigAlienWave`
store) is codegen-identical for every order (compiler reorders the stores).

Type permutations: all-`int` = raw 5219 / size 17176 (+4); all-`char` =
5937 / 17184; every mixed bool/int/char/unsigned/short/long combination is
either wrong-size or raw >= 12599.  `int` for only `raid` = 5914 / 17168.

### 2. `alienGuns` / `fortressGuns`: branch vs multiply

Base:

```c++
    int alienGuns = alienWave ? (bigAlienWave == 1 ? 8 : 7) : 0;
    int fortressGuns = pirateFortress ? 5 : 0;
```

| variant | raw | size |
|---|---:|---:|
| **base** | **5143** | 17172 |
| `alienWave * (bigAlienWave == 1 ? 8 : 7)` | 5689 | 17168 |
| `alienWave ? 7 + (bigAlienWave == 1) : 0` | 5143 | 17172 |
| `!alienWave ? 0 : (bigAlienWave == 1 ? 8 : 7)` | 5143 | 17172 |
| `bigAlienWave ? 8 : 7` (drop `== 1`) | 5143 | 17172 |
| `fortressGuns = pirateFortress * 5` | 5143 | 17172 |
| `int alienGuns = 0; if (alienWave) alienGuns = ...` | 5143 | 17172 |
| `int fortressGuns = 0; if (pirateFortress) fortressGuns = 5` | 5143 | 17172 |

The original at `0xb2a20` is a two-level conditional (branch), which the base
already reproduces; the multiply forms are size-`-4` and raw-worse.

### 3. `Globals::*` hoists and local/type nudges

All of these are codegen-identical (raw 5143, size 17172):

- `AERandom *rnd = Globals::rnd;` hoisted into the alien-gun / pirate /
  raid / blueprint blocks (and the existing case4Tail hoist).
- `Status *status = Globals::status;` hoisted at the friendCount line and at
  the extraAliens block.
- `Array<KIPlayer *> *enemies = this->enemies;` at the extraAliens block.
- `stationPirates` `unsigned`->`int`; `blueprint` `unsigned`->`bool` is
  raw-worse (6805/17172); `stationPirates` `bool` is raw-worse
  (12215/17176).
- `raid`/`convoy`/`guns`/`alienGuns`/`fortressGuns`/`localRace`/`raidRace`/
  `security`/`stationIndex`/`extraFriend`/`race`/`wingmanHp` -> `unsigned`
  (all 5143 except `security` 5168 and `raid` 5914/17168).
- `hasWanted` initialised as `wanted != nullptr` and the `if` rewritten
  around it: 14568 / 17144.
- Reordering the `inSupernovaSystem` body (all 6 positions for
  `alienWave = 0`, all 6 for `raid = false`, plus full reverse; 13 builds):
  all 5143.
- Reordering the `bigShipsCount` guard (`localRace != 0 || ...`, inverted
  branches, `>= 1`, `else if`): 13021 / 5145 / 5145 / 5143.
- Inner pirate guard `*bigShipsCount > 0` / `>= 1` / `!= 0`, operand swaps,
  `>= 141`, `? true : false`: 5143 / 5143 / 5146 / 5143 / 5143 / 5143.
- `hard ? 40 : 20` -> `20 + hard * 20`; `raiders` multiply; friendCount
  operand order; raid-guard operand order: 5143 (except `raiders` multiply
  14505 / 17180).
- `alienPercent` shift -> multiply: 13956 / 17188.
- `Vector offset` / `float offset[3] = {0,0,0}` / separate `ox,oy,oz` /
  `setPosition(offset[0],offset[1],offset[2])` / `setPosition(Vector{...})`
  in the case-183 convoy `switch (k)`: all 5143/17172 (the compiler
  canonicalises the store order).

### 4. The `extraAliens` guard: the real structural residual

Base:

```c++
    if (__builtin_expect(extraAliens >= 1, 1)) { // lint: arm_builtin
```

| guard form | raw | aligned | size |
|---|---:|---:|---:|
| **`__builtin_expect(extraAliens >= 1, 1)` (base)** | **5143** | **952** | 17172 |
| `if (extraAliens >= 1)` (plain) | **4485** | 975 | **17176 (+4)** |
| `if (extraAliens > 0)` | 4485 | 975 | 17176 |
| `if (extraAliens != 0)` | 4485 | 976 | 17176 |
| `if (extraAliens)` | 4485 | 976 | 17176 |
| `__builtin_expect(extraAliens > 0, 1)` | 5143 | 952 | 17172 |
| `__builtin_expect(extraAliens != 0, 1)` | 5143 | 953 | 17172 |
| `__builtin_expect(extraAliens, 1)` | 5198 | 706 | 17180 |
| `__builtin_expect(extraAliens >= 1, 0)` | 13869 | 2638 | 17180 |

`(extraAliens & 0x7fffffff) >= 1`, `extraAliens + 0 >= 1`,
`extraAliens - 1 >= 0`, duplicated `>= 1`, `(int)` cast, `>= 1u`,
`!= 0 && >= 1`: all reproduce the base raw 5143 and size 17172 (aligned
952-953; the range hint propagates through every spelling).

Why the plain form is interesting: the original's extraAliens loop
(`0xb3e46`) ends with

```
cmp  r6, r0
blt.n 0xb3e00        ; signed
```

while the base emits `bcc.n` (unsigned).  The `__builtin_expect(x >= 1, 1)`
tells clang the bound is positive, so it switches the loop test to an
unsigned compare.  Removing the hint restores the original signed `blt`
and drops raw by 658 B.  The hint was introduced in wave 175
(commit `a6aba7fb1`, "faithful bool/order + globals/rnd/enemies hoists").

The +4 B is *not* in the extraAliens block itself.  It is a register
allocation swap in the preceding pirate-escort loop: the original keeps
`this` in `sl` and materialises `10000` with `movw fp,#10000` (4 B), while
the plain form keeps `this` in `fp` and emits
`mov fp,sl; movw sl,#10000` (6 B) plus a later `mov sl,fp` (2 B) — +4 B net.
The extraAliens block itself then matches the original instruction for
instruction apart from the stack-slot rotation below.

Compensating shapes tried on top of the plain guard (all real builds):

| combination | raw | size |
|---|---:|---:|
| plain + `guns` hoisted before `if (raid)` (`guns_single`) | 5154 | 17172 |
| plain + `alienGuns` multiply | 5590 | 17172 |
| plain + `raid` `unsigned` | 5815 | 17172 |
| plain + `guns` fully inlined | 5750 | 17172 |
| plain + `escortsEnd` inlined | 5297 | 17156 |
| plain + `start` inlined | 5280 | 17168 |
| plain + hunter-tail `ship` local (pre-wave-177) | 4638 | 17164 |
| plain + pre-wave-177 redundant loop condition | 5837 | 17188 |
| plain + `hp` `unsigned` | 6797 | 17168 |
| plain + convoy all-`102` store order | 11346 | 17168 |
| plain + bool decl `APB` | 10410 | 17168 |

Only `plain + guns_single` is size-exact, and it is raw **5154**, 11 B worse
than the base.  Every other size-exact pairing is much worse.

## Convoy `switch (k)` store-order note (negative result)

The original's case-183 convoy loop stores the three `offset` components in
the order `offset[1], offset[0], offset[2]` for **all four** cases
(`0xb1420`, `0xb142c`, `0xb1438`, `0xb1444`).  The base source uses mixed
orders, but the compiler canonicalises them: `Vector offset`, separate
`float ox,oy,oz`, and `float offset[3] = {0,0,0}` with the all-`102` order
all compile to the base code (5143/17172).  Writing `float offset[3]` with
all four cases in `102` order instead triggers clang's constant-table
lowering (`vldr` from a rodata table, size 17164, raw 11352), so the source
store order is not the degree of freedom that explains the block.

## Stack-slot rotation (unchanged residual)

Original flag slots: `pirateFortress = sp+56`, `alienWave = sp+60`,
`bigAlienWave = sp+64`.  Ours: `bigAlienWave = sp+56`,
`pirateFortress = sp+60`, `alienWave = sp+64`.  The rotation is visible at
`0xb2e0e` (`ldr r2,[sp,#60]` vs `[sp,#64]`) and `0xb2f30`
(`ldr r1,[sp,#56]` vs `[sp,#60]`).  Declaration order, type, declaration
position (before `convoy`, before `siegedStation`, after `missionPtr`,
before `hasWanted`, before `blueprint`, before `extraAliens`) and
assignment order all leave the rotation in place; the permutations that do
move it change size or raw badly (table above).

## Gate (base, unchanged)

- `byte_exact` **4507 / 4524**
- `linked_exact` **4507 / 4524**
- `size_exact` **4524 / 4524**
- `stub_zero_size` 0, `missing` 0, `wrong_type` 0, `extra` 0
- `data_exact` **843 / 843**, imports **207**
- lint **CLEAN**, drift **0**, symbol parity **5370 / 5370**
- `.rel.plt` byte-identical (ratchet PASS)
- `_ZN5Level13createMissionEv` raw **5143**, aligned **952**, size
  **17172 / 17172**

## Files

- `src/game/world/Level.cpp` — unchanged from base `1b48f990f`.
- Measurements are reproducible with
  `python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv`.
