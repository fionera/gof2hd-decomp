# w6414: `Level::createMission` (92.145215 %, 17172 B) — wave-116:
# stack-slot coloring is the first and largest residual; convoy body order unreachable

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6414`, branch `wave116-w6414`,
base `d672d349f`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`–`0xb431c`, 17172 B).

**No `src/` change is committed.** Every variant below is neutral or a
regression against the committed wave-115 baseline. The deliverable is this
note. The worktree is clean.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   official 92.145215 %   size 17172 B (size-exact)
                             shape (regs+pc collapsed) 96.039604 %
                             nopc (pc collapsed only)  92.821782 %
                             6055 orig insns / 6065 ours
gate: byte 4501/4524, linked 4501/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF shape unchanged: 27 sections, file 2,187,328 B.
```

`shape`/`nopc` use the same normalisation as `/dev/shm/w6414/score.py`
(register names + `[pc,#N]` collapsed / only `[pc,#N]` collapsed).

## 0. The first difference in the whole function is a stack-slot swap

The reconciled instruction streams are **identical for the first 28
instructions**; instruction 28 is the first mismatch:

```text
idx 28   O  str.w fp, [sp, #88]
         U  str.w fp, [sp, #84]        ; fp = Globals::status, spilled at line 1529
```

The two variables involved are `Globals::status` (2 uses: idx 28 store,
idx 272 reload) and `raid` (17 uses: idx 451, 463, 504, 553, 616, 674, 719,
799, 850, 859, 884, 900, 926, 953, 968, 3812, 5014).  In the original
`status` sits at `sp+88` and `raid` at `sp+84`; in our build they are
**swapped** (`status` at `sp+84`, `raid` at `sp+88`).  Every other 4-byte
slot in the frame matches except a handful of small ones (sp+44/48,
sp+40, sp+104/92, sp+112/116/120).

Evidence from `-mllvm -print-machineinstrs` (scratch `/dev/shm/w6414/mi2/all.txt`):

* At `After Greedy Register Allocator`, `status` owns `%stack.73`
  (first 4-byte slot) and `raid` owns `%stack.85`; `localRace` is
  `%stack.82`, `raidRace` `%stack.83`.
* `StackSlotColoring` (`llvm-7 StackSlotColoring.cpp`) sorts the spill
  intervals **by weight** (`std::stable_sort(..., IntervalSorter())`,
  weight = sum of `getSpillWeight` over the slot's references, i.e. block
  frequency) and `ColorSlot()` gives each interval the *first
  non-overlapping used colour*.  Colours are then laid out in colour-index
  order: colour 73 → `sp+124`, 82 → `sp+88`, 83 → `sp+84`.
* Our colouring therefore assigns `raid` → colour 82 (`sp+88`) and
  `status` → colour 83 (`sp+84`).  The original must have had them the
  other way round (colour 82 = `status`, 83 = `raid`), i.e. a different
  relative spill weight / interval split.

All 17 `raid` references move together, so fixing the swap is worth up to
`17/6055*2 ≈ 0.56 pp` directly, plus whatever cascades.  It is **not
source-reachable by spelling `raid` differently**: every variant below left
the slot map byte-identical to the baseline.

| variant | official |
|---|---:|
| **baseline** | **92.145215** |
| `bool raid = false;` at declaration | 92.145215 |
| `raid` declared after `security` | 92.145215 |
| `raid` declared before `introSystem` | 91.303630 |
| `raid` declared after `playerPos` / after `missionPtr = 3` | 91.303630 |
| `int raid` (0/1) | 91.980198 |
| `int raid` ternary `? 0 : cmp` | 91.980198 |
| `int8_t` / `short` / `unsigned` / `uint8_t` `raid` | 91.980198 |
| ternary `raid = cond ? false : cmp` | 92.145215 |
| `raid = !(introSystem && !hardCoreMode()) && cmp` | 92.145215 |
| `raid` computed then overridden (`raid = cmp; if (...) raid = false;`) | 84.736538 (17228 B) |
| `raid` moved after `localRace`/`raidRace` | 73.606869 (17408 B) |
| `localRace` moved before `raid` | 77.952106 (17336 B) |
| `raidCoords`/`field_180` moved after `localRace` | 86.841020 (17180 B) |

## 1. `if ((security == 3) & raid)` — `eor/orrs` vs short-circuit

Original (`0x2e2c`, inside the `raid` block):

```text
subs.w r0, fp, r3      ; security - 3
mov    r1, r0
it     ne
movne  r1, #1          ; security != 3
ldr    r2, [sp, #84]   ; raid
eor.w  r2, r2, #1      ; !raid
orrs   r1, r2
bne.w  skip
```

Ours emits the short-circuit form (`ldr r0,[sp,#88]; cmp r0,#0; beq skip;
cmp fp,r3; bne skip`).  This is the `SinkCmpExpression` cluster; the
handoff's list of spellings was re-measured and extended — all neutral:

```text
(security == 3) & raid            (baseline)   92.145215
raid & (security == 3)                         92.145215
!(!raid || security != 3)                      92.145215
(security == 3) & (raid ? 1 : 0)               92.145215
(security == 3) & (raid == true)               92.145215
(security == 3) & (raid != 0)                  92.145215
bool raidAtMax = (security == 3) & raid; if (raidAtMax) ...  92.145215
int  raidAtMax = ...; if (raidAtMax) ...       92.145215
if (!cond) {} else { ... }                     92.145215
while (cond) { ...; break; }                   92.145215
security == 3 && raid                           87.114338 (17196 B)
```

The same `&&`/`&` experiment on the other merged condition
(`if (i == friendCount + field_164 && alienWave)`, line 1904) is much
worse: `(i == ...) & alienWave` → **75.540518** (6063 insns);
`alienWave & (i == ...)` → 75.540518.  `&& alienWave != 0`, `&& !!alienWave`,
`&& (bool) alienWave`, `friendCount + field_164 == i && alienWave` are all
neutral (92.145215).

## 2. Convoy `k` chain (line 2192) — body layout `(0,1,2)` is unreachable

Fresh measurement of all six source permutations, dumping the literal pool
at `0x15e8` in our link (orig pool starts at `0x15e4`):

| source order | our pool order (case triples) | official | shape | nopc |
|---|---|---|---:|---:|---:|
| **210 (baseline)** | case2, case0, case1, default | **92.145215** | 96.039604 | 92.821782 |
| 201 | case2, case0, case1, default | 92.145215 | 96.039604 | 92.821782 |
| 120 | case1, case0, case2, default | 92.013201 | 95.924092 | 92.920792 |
| 102 | case1, case0, case2, default | 92.013201 | 95.924092 | 92.920792 |
| 012 | case0, case2, case1, default | 91.996700 | 95.924092 | 92.920792 |
| 021 | case0, case2, case1, default | 91.996700 | 95.924092 | 92.920792 |

The original's body layout is `case0, case1, case2, default` (pool
`-3000,20000,-50000 | -7000,10000,-30000 | 25000,-36000 | 6000,12000,-45000`).
A `switch` with the same case order is byte-identical to the if-chain
(`switch 0,1,2` → 91.996700, body `(0,2,1)`), confirming that the
`(0,1,2)` layout is not reachable from clang 7.0.2's `MachineBlockPlacement`
for this chain.  The 4-byte pool shift caused by the body-layout difference
is the single largest literal-pool defect and is *downstream* of this.

## 3. Other source-level probes (all neutral unless noted)

```text
if ((int) raiders >= 1)  ->  > 0                        92.145215
*bigShipsCount > 0       ->  >= 1                       92.145215
*bigShipsCount > 0       ->  != 0                       89.517993 (17164 B)
localRace == 0 && *bigShipsCount > 0 (swapped &&)       74.025760 (17164 B)
} else if (localRace == 1 && *bigShipsCount > 0 && ...) 92.145215
struct Coords6 {int a..f} -> int coords[6]              92.145215
struct RaidCoords3        -> int raidCoords[3]          92.145215
convoyShip->cargo / crateCaptured() source order swap   92.145215
if (mission == nullptr) -> if (!mission)                92.145215
if (freelance != nullptr) -> if (freelance)             92.145215
int stationIndex -> unsigned                            92.145215
int pirates -> unsigned                                 92.145215
int localRace -> unsigned                               92.145215
int raidRoll -> unsigned                                92.112211
int security -> unsigned                                92.020794 (6064 insns)
bool introSystem -> int introSystem                     80.079286 (17164 B)
int i = 10 + k -> k + 10                                92.145215
for (int k...) -> for (unsigned k...) in convoy loop    92.145215
```

## 4. Structural facts confirmed this wave

* The reconciled streams are identical up to instruction 28; the first
  mismatch is the `status`/`raid` stack-slot swap (§0).
* The `.ARM.exidx` compact entries for this function differ
  (`0x0903/0x0a14/0x0b8a` vs `0x0905/0x0a15/0x0b89`) purely because the
  `.text` layout differs; they are not independently source-controllable.
* `Level::createScene`, `Level::createSpace` and `Level::init` are all
  inlined into this TU's `createMission` section (the 17172 B section is
  exactly the function); the `objdump -l` line attribution is unreliable
  for the inlined bodies (it reports e.g. `Level.cpp:861` for createSpace
  code), so use the `clang -S` `.loc` stream plus `DEBUG_VALUE` comments
  (`/dev/shm/w6414/Level.s`) when mapping instructions back to source.
* A `clang -S` build with the exact CMake flags reproduces the linked
  object instruction-for-instruction, and its `.loc` stream is the only
  reliable source-line map for this function.

## 5. Conclusion

The residual is `RegAllocGreedy` register allocation plus the
`StackSlotColoring` colour assignment and the literal-pool placement that
follows from block lengths.  The first and largest single defect is the
`status`/`raid` slot swap (17 instructions, ≈0.56 pp), which is a
downstream effect of the colouring weight order rather than a spelling of
`raid`; the convoy body layout `(0,1,2)` that would fix the biggest pool
shift is not reachable from clang 7.0.2's block placement.  No variant
measured this wave beat the committed baseline's **92.145215 % / 17172 B**.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6414/score.py _ZN5Level13createMissionEv
```

Scratch tooling (not committed): `/dev/shm/w6414/score.py`
(official/shape/nopc), `harness.py` (apply/build/score/restore),
`ctxaddr.py` (side-by-side disasm), `slotuses.py` (per-`sp`-offset use
list), `linemap2.py`/`linemap3.py` (diff regions with line attribution),
`convtest.py` (convoy permutation sweep with pool dump), `batch*.py`
(variant sweeps), `Level.s` (clang `-S` with `.loc`/`DEBUG_VALUE`),
`mi2/all.txt` (`-mllvm -print-machineinstrs`).
