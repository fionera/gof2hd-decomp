# `Level::createShip` + `Status::missionCompleted` — wave 187 (w13118):
# remaining order cells are canonicalised; the ordinary-C++ rank table is
# globally unsafe

Worktree `wt/w13118`, branch `wave187-w13118`, base master `8b5d4281c`
(wave 186).  Targets:

* `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
  2,764 B, VMA `0xc0214`), base raw **1,202**, aligned 484.
* `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
  1,432 B, VMA `0xabf38`), base raw **721**, aligned 327.

Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13118`.

**Status: no code win landed; note only.**  The base gate is unchanged:
byte_exact 4507/4524, linked_exact 4507/4524, size-exact 4524/4524,
stub 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.text` diff **74,274**,
`.ARM.extab` 156,312 B @ `0x1a3b78` (diff 647), `.rel.plt` byte-identical,
build-id 20, file size 2,187,328.

## 0. Bottom line

`createShip`: every remaining declaration/statement/coordinate/branch
cell that keeps the two `EmpGuard` scopes and size 2,764 B lands at
**1,202**; the compiler canonicalises the reorderings.  The semantically
correct jitter pairing (first `nextInt` -> `x`) is **1,204** (aligned 399,
insn 1117/1117), i.e. two raw bytes worse than the landed wrong-pairing
form, because the callee-saved/stack-slot allocation shifts.

`Status`: a `static const unsigned char` rank table inside `Status.cpp`
does reproduce the dispatch shape (function raw 1,202, aligned 183,
size 1,436), but it shifts `.rodata` by 192 B and drops
`byte_exact 4507 -> 4389`.  A hidden `extern` table in a last-linked TU
avoids the `.rodata` shift but adds a symbol to gold's hash table and
reorders `.dynsym`/`.dynstr`/`.text` (`.text` diff 135,720).  Only the
wave-185 fixed-address table (no symbol reference) keeps global parity,
and that is the manual-data form the task rules out.

## 1. `createShip` — baseline and the real residual

Base (landed wave-185 coordinate split):

```
raw=1202 aligned=484 size=2764/2764 insn=1116/1117
.text diff 74,274   .ARM.extab 156,312 @ 0x1a3b78 (diff 647)
```

`fn_diff` categories: `operand:ldr` 112, `extra-orig:b.n` 27,
`extra-ours:b.n` 27, `regalloc:mov` 27, `regalloc:ldr.w` 25,
`extra-orig:mov` 22, `extra-ours:mov` 18, `extra-orig:ldr` 14.

The first textual divergence is the stack-slot permutation at `+0x2c`:

| value | original slot | ours slot |
|---|---:|---:|
| `wpx` | 44 | 40 |
| `wpy` | 48 | 36 |
| `wpz` | 40 | 48 |
| first `nextInt` | 36 | 44 |
| second `nextInt` | 32 | 32 |
| third `nextInt` | 28 | 28 |

Downstream the original keeps `ship` in `r9` (spilling `this` at
`+0x11e`), `fighter` in `sl`, and the `x` coordinate in `fp`; ours
coalesces `ship`/`fighter`/`fixed` into `fp` and keeps `x` in `r4`.
That is why the tail (`+0x918`..) differs even though the instruction
streams are otherwise the same shape.

## 2. `createShip` sweeps measured this wave (all size 2,764 / extab exact)

All builds: `cmake --build cmake-build-match --target link_so -j2`
under `ulimit -v 12000000`; `tools/fn_diff.py` for raw/size/insn; the
`.ARM.extab` size read from `objdump -h`.

### 2.1 Declaration / assignment order

| variant | raw | aligned |
|---|---:|---:|
| base (`int wpz, wpx, wpy;`, `jitter3,jitter2,jitter1`) | **1,202** | 484 |
| `int wpz, wpy, wpx;` | 1,202 | 487 |
| `int wpx, wpz, wpy;` / `wpx,wpy,wpz` / `wpy,wpz,wpx` / `wpy,wpx,wpz` | 2,423 | 455-459 |
| all 36 if/else assignment orders (`x,y,z` permutations) | 1,202 | 484 |
| all 36 `(wp declaration) x (jitter declaration)` orders | 1,202 (z-first) / 2,423 | — |
| declaration at function top / before mission / before coords / before armor | 1,202 | 484 |
| `ship = fighter;` before the cargo block / `ship = fixed;` after ctor | 1,202 | 484 |
| `int empTime;` before `empPoints` | 1,202 | 484 |
| `volumes` declared at top / before `fixed` | 1,202 | 484 |
| `mission == 49..56` condition reversed | 1,202 | 484 |
| `hitpoints` switch `0x31`/`0x33` swapped | 1,202 | 484 |
| combinations of 2-3 of the above | 1,202 | 484 |

The compiler canonicalises every one of these; the `.text` image is
byte-identical to the base.

### 2.2 Jitter pairing

The original's disassembly pairs the **first** `nextInt` with `x`
(`x` slot 44 + first slot 36), the second with `y`, the third with `z`.
The landed source pairs the first with `z` (old `jitter3,jitter2,jitter1`
naming).  Rewriting to the natural pairing:

| variant | raw | aligned | insn |
|---|---:|---:|---:|
| base (first -> z) | **1,202** | 484 | 1116/1117 |
| natural (first -> x), split `z` before guard | 1,204 | **399** | **1117/1117** |
| natural, all three before guard | 1,216 | 439 | 1118/1117 |
| natural, all three after guard | 1,358 | 468 | 1118/1117 |
| natural, `x` before guard | 1,207 | 403 | 1117/1117 |
| natural, old call order (`jitter3,jitter2,jitter1`) + natural use | 1,204 | 399 | 1117/1117 |

The natural pairing matches the original's instruction count and is
structurally much closer (aligned 399 vs 484), but the stack-slot
permutation makes it two raw bytes worse.  It is not a gate win.

### 2.3 Coordinate forms

| variant | raw | size |
|---|---:|---:|
| base (`float z` before, `int x` + `float y` after) | **1,202** | 2,764 |
| `int x,y,z` all before guard (implicit float at ctor) | 2,101 | **2,752** |
| `int x,y,z` assigned inside the guard | 2,101 | 2,752 |
| `int x,y,z` all before, all 8 split placements | 2,229-2,501 | 2,752-2,816 |
| `float z = (float)(wpz+jitter3) - 20000.0f` | 2,142 | 2,776 |
| `volatile int x,y,z` | 3,434 | 3,500 |

The all-int form is the original's shape (all three ints before
`setEmpData`, six `vcvt` at the two ctor sites), but clang 7 sinks the
computation below the call and the function is 12 B short.  With
`-mllvm -disable-machine-sink` (not a build-recipe flag) the size is
2,764 but raw is 2,051.

### 2.4 `empPoints` clamp polarity

The original emits `ble` (fall through on `level > 20`); the base emits
`bgt` and spills `this` (`str r9,[sp,#56]`) one block earlier.

| variant | raw | aligned |
|---|---:|---:|
| base `<= 20 ? getLevel() : 20` | **1,202** | 484 |
| `> 20 ? 20 : getLevel()` | 1,209 | 482 |
| `__builtin_expect(<= 20, 1)` / `__builtin_expect(> 20, 0)` | 1,610-2,273 | — |
| `level` local reuse (`level * 5 + 40`) | 2,366 | 533 (size 2,756) |

The `> 20` form fixes the branch polarity but leaves the spill in the
early block; every combination tested is >= 1,202.

### 2.5 `ship`/`fixed` coalescing

The original has three `mov r9,sl` copies (`ship = fighter`) at
`+0x534`, `+0x54a`, `+0x554` and `ship`/`fixed` in `r9`; ours coalesces
`ship`/`fighter`/`fixed` into `fp` and has no copies.  Source attempts:

| variant | raw | size |
|---|---:|---:|
| `ship = fighter;` before cargo block | 1,202 | 2,764 |
| `ship = fighter; fighter = nullptr;` | 1,202 | 2,764 |
| `ship = (KIPlayer *)(intptr_t) fighter;` | 1,202 | 2,764 |
| `ship = fighter;` duplicated in each cargo branch | 2,044 | 2,764 |
| `PlayerFixedObject *ship` + cast | 2,340 | 2,788 |
| `fixed` returned directly from the fixed branch | 2,196 | 2,764 |
| `shipClass == 1` checked first | 2,483 | 2,792 |
| two independent `if (shipClass == ...)` | 2,583 | 2,744 |
| `KIPlayer *ship;` uninitialised | 2,418 | 2,760 |

None is a win.  The base 1,202 is a strong local optimum.

## 3. `Status::missionCompleted` — ordinary-C++ rank table

The wave-185 manual form (`#define MISSION_CASE ((const unsigned char *)0x20b977)`,
table in `tools/mission_case_table.cpp`) reaches raw **449**, aligned 160,
size 1,432, `.rel.plt` identical, `.rodata` +192 B in the gap before
`.fini_array` (26/27 headers).  It is manual data.

### 3.1 `static const unsigned char kMissionCase[192]` inside `Status.cpp`

Same rank body, table defined in `Status.cpp`:

```
_ZN6Status16missionCompletedEbbx  raw=1202 aligned=183 size=1436/1432 insn=531/527
```

Function-local it is close to the manual form, but the table is emitted
in the middle of `Status.cpp`'s `.rodata` (line 1314), so every later
`.rodata` symbol shifts by 192 B.  Full `check`:

```
verify: byte 4389  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
FAIL  byte_exact 4507 -> 4389
```

118 functions lose byte-exactness.  The lost set is exactly the TUs after
`Status.cpp` in `tools/source_order.txt` (Array instantiations,
`StarSystem`, `SpaceLounge`, `TouchButton`, `HangarWindow`,
`PlayerFighter`, ...).  Section state: `.text` diff 83,008,
`.rodata` 269,351 (26/27 headers), `.dynsym` diff 305,
`.data.rel.ro` diff 136, `.ARM.exidx` diff 44, `.ARM.extab` diff 647.

Moving the table to the end of `Status.cpp` does not help: it still
precedes the ~18 TUs linked after `Status.cpp`.  `-fdata-sections` on
`Status.cpp` (per-TU flag, `cmake-build-ab`) merges `.rodata.kMissionCase`
back into `.rodata` in input order; `.text` diff 83,851, same 118 loss.

### 3.2 Hidden `extern` table in the last-linked TU

Table in `src/engine/core/ArrayInstantiations.cpp` (last TU, no other
`.rodata`), `#pragma GCC visibility push(hidden)`, `Status.cpp` declares
`extern const unsigned char kMissionCase[192]`:

```
_ZN6Status16missionCompletedEbbx  raw=1134 aligned=159 size=1436/1432 insn=531/527
```

The table lands at the end of `.rodata` (no data shift), but the new
symbol enters gold's symbol hash table and reorders the dynamic symbol
emission:

```
.text diff 135,720   .dynstr diff 4,417   .dynsym diff 1,001
.plt diff 3,797      .got size 17,168 vs 17,164
.rel.dyn size 28,752 vs 28,744   .rel.plt size 29,392 diff 102
```

This is the wave-185 note's "extern rank table in a last-linked TU"
row (1,134 / 159 / 1,436).  It is not globally safe.

### 3.3 Local rank variable / rearranged switch

A first `switch (mtype)` assigning the rank, then `switch (rank)`:

```
_ZN6Status16missionCompletedEbbx  raw=915 aligned=357 size=1428/1432 insn=531/527
```

No `.rodata` shift (no table), but the function is 4 B short and raw 915
is worse than the plain switch's 721.  The first switch's extra code
shifts the bodies.

### 3.4 Why only the fixed address works

Gold emits `.dynsym` by iterating its symbol hash table over *every*
symbol in input order (see `dynsym_dynstr_hash_order_w3567.md`).  Any
new symbol — even a hidden/local table — changes that iteration order.
The wave-185 form avoids a symbol reference entirely by reading the
table through the constant `0x20b977`, so `.dynsym`/`.dynstr`/`.rel.plt`
stay byte-identical.  An ordinary-C++ table reference necessarily adds
a symbol and reorders the dynamic tables.  The only way to keep both
ordinary C++ and global parity would be to place the table at the end of
`.rodata` *and* reference it without a symbol, which C++ cannot express
without an attribute/pragma section or a fixed address.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13118
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb _ZN6Status16missionCompletedEbbx
# createShip raw=1202 aligned=484 size=2764/2764 insn=1116/1117
# Status     raw= 721 aligned=327 size=1432/1432 insn=529/527
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13118/`: `Level_base.cpp`,
`Status_base.cpp`, `Status_rank_hard.cpp`, `Status_rank_static.cpp`,
`Status_rank_extern.cpp`, `Status_rank_localswitch.cpp`,
`mission_case_table.cpp`, `sweep.py` (declaration/statement/coordinate
sweeps), `measure.sh`, `secdiff.py`, `deltas.py`, `calls.py`,
`sweep_results.txt`.

## 5. Files changed

* `_work/reconstructions/level-createship-status-wave187-w13118.md` —
  this note.  `src/` is unchanged from the base.
