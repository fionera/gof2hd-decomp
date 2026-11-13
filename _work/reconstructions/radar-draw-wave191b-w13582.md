# Radar::draw wave 191b (w13582) — full RA queue enumerated, no size-exact flip

**Status:** complete, 2026-11-13. Worktree `wt/w13582`, branch `wave191b-w13582`,
base `master` `fd3394f72`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13562.md` (RA root cause) and
`radar-draw-wave191b-w13574.md` (eviction weights). **No source change landed**:
≈350 cells measured, every size-exact cell is raw-neutral (6,013) or worse.
`src/` untouched.

## 0. Baseline (verified)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |

## 1. New evidence: the real `getSize()` queue (not spans)

w13562/w13574 used first-def→last-use *spans*.  Recomputed the actual
`LiveInterval::getSize()` (sum of live segments, def→last-use-before-next-def)
from the pre-RA MIR (`cmake -mllvm -print-machineinstrs`, dump at
`mir.txt:395123`).  The greedy queue is **(hint bit 30) → (size) → (vreg no.)**,
so the true order is:

| # | vreg | hint | `getSize()` | what |
|---:|---:|---|---:|---|
| 1 | `%380` | r0 | 105,776 | `this` (live-in copy) |
| 2 | `%382` | r2 | 93,088 | `hud` |
| 3 | `%383` | r3 | 89,408 | `mode` |
| 4 | `%381` | r1 | 88,592 | `player` |
| 5 | `%416` | r0 | 87,632 | `turretMode` |
| 6 | `%420` | r0 | 22,528 | `alienOrbit` |
| 7 | `%422` | r0 | 22,320 | `campaign` |
| 8 | `%385` | – | **105,792** | `__stack_chk_guard` GOT load |
| 9 | `%395` | – | **104,000** | `Globals::status` GOT load |
| 10 | `%391` | – | **96,208** | `Globals::Canvas` GOT load |
| 11 | `%15` | – | **86,112** | multi-def GOT/global value |
| 12 | `%2549` | – | 59,808 | |
| 13 | `%2550` | – | 55,984 | |
| 14 | `%2548` | – | 42,768 | |
| 15 | `%122` | – | 40,208 | |
| 16 | `%2487` | – | **34,992** | shared zero + `missionBlocksPlanets` |

This is the key new fact: `%2487` is **not** the first unhinted range.  Eight
larger unhinted ranges (`%385`, `%395`, `%391`, `%15`, `%2549`, `%2550`,
`%2548`, `%122`) are dequeued before it and each can evict `campaign`
(weight 0.02551238) before `%2487` ever runs.  So the "`%2487` evicts
`campaign`" story of w13574 is only the *last* eviction in a cascade; the
first loss of `campaign` almost certainly happens at `%385`/`%395`/`%391`.

Also recovered: **`r7` is the frame pointer** (`push {r4,r5,r6,r7,lr}` +
`add r7,sp,#12`), so only **7** callee-saved registers are allocatable
(`r4,r5,r6,r8,r9,r10,r11`).  The 7 hinted long ranges above exactly fill them,
which is why `campaign` (7th) lands on the last one in the original (`sl`) and
why `%2487` (16th) then takes `fp`.

## 2. Cells measured this pass (all reverted; base 6,013 / 9,908)

### 2.1 Early-statement permutation (fresh angle)

All 160 random permutations of the 7-statement block
`{plasmaInRange=0, lockConfirmed=0, SetColor, mission=getMission,
missionBlocksPlanets=false, alwaysEnemySeen=false, alwaysEnemy=false}`:
best **6,013** (neutral); the rest 6,027–6,703, all size-exact.

### 2.2 `bool`→`int`/`unsigned char` sweep (all 16 bool locals)

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `alwaysEnemySeen` int / uchar | 9,191 | 9,952 (+44) |
| `alwaysEnemy` int / uchar | 8,599 / 8,612 | 9,920 (+12) |
| `turretMode` int / uchar | 9,226 | 9,860 (−48) |
| `landmarkHit` int / uchar | 8,730 / 8,672 | 9,936 / 9,944 |
| `planetHit` int / uchar | 8,460 / 8,481 | 9,912 / 9,916 |
| `marked` int / uchar | 8,464 | 9,912 |
| `bossSeen` int / uchar | 9,186 | 9,924 |
| `enemyHit` int / uchar | 8,615 / 8,633 | 9,940 / 9,944 |
| `wreck` int / uchar | 8,236 | 9,912 |
| `gasHit` int / uchar | 7,861 / **6,022** | 9,904 / **9,908** |
| `asteroidHit` int / uchar | 7,535 / 7,603 | 9,924 / 9,928 |
| `topOk`/`isTarget`/`inSight`/`navWreck`/`hasDockNav`/`candidateWreck` | 6,013 | 9,908 |

`gasHit` as `unsigned char` is the only size-exact retype and is +9 raw.
`int alwaysEnemySeen` does flip the zero to `fp`, but moves `campaign` to
`r5` (not `sl`) and is +44 B — confirms w13544 §5.1.

### 2.3 `__builtin_expect` on the mission-type chain (block frequencies → weights)

Adding an expect to any of the four `getType()` comparisons, or to the whole
chain, or to the campaign guard / `planetTargets`:

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `expect(type != 0, 1)` | 6,044 | 9,908 |
| `expect(type != 171, 1)` | 6,762 | 9,908 |
| `expect(type != 13, 0)` | 9,175 | 9,908 |
| `expect(planetTargets != nullptr, 0/1)` | 6,013 | 9,908 |
| others | 8,467–9,548 | mostly size-fatal |

### 2.4 Music tail (`A = (uint32_t)(cur-136) > 15`, `B = expect(cur != 151 && alwaysEnemySeen, 0)`, `C = ((1<<(cur-136)) & 0xe071) == 0`)

The original evaluates **A, C, B** (`0x130280`–`0x1302a2`); ours emits A, B, C.
The correct source order `A || C || B` is size-exact but raw **6,036**; a
`__builtin_expect` around the whole disjunction gives 6,036/9,908;
`A || C || B` without the expect is size-fatal (9,900).  Confirms w13544 §5.3.

### 2.5 Asteroid candidate-change arm

The original is `asteroidLockTimer = 0` in the `if (best > -1)` candidate block
(`movne r1,#0; strne.w r1,[r4,#412]` at `0x12fd8a`) and `+= mode` in the
`else` arm immediately before the `> field_0x1b8 - 200` test
(`ldr; ldr; add; str` at `0x12fe38`).  Our source has `+= mode` in the candidate
block.  Moving it to match the binary is size-exact but raw **6,188**; each half
alone is size-fatal (`= 0` only 9,888; `+= mode` only 9,916).  Combined with the
A,C,B music order: 6,211.

### 2.6 Hint-status experiments on `%2487`

Initialising `missionBlocksPlanets` from a physical register (a COPY hint) while
keeping the value 0 on the false path:

```cpp
int missionBlocksPlanets = (int) (intptr_t) mission;   // hint = r5
if (...) missionBlocksPlanets = mission->getType() != 172;
else     missionBlocksPlanets = 0;
```

is size-exact and raw **6,013** (neutral): the hint does not move the eviction.
Without the `else` it is size-fatal (9,896, raw 9,401).

### 2.7 Other

* chained zero store `lockConfirmed = plasmaInRange = 0` → 6,013;
  `plasmaInRange = lockConfirmed = 0` → 6,015.
* zero stores moved after `SetColor` → 6,044; after `getMission` → 6,055;
  after the type check / campaign / planetTargets / onPlanetCourse → 9,387–9,441
  and −16 B (size-fatal).
* `SetColor` moved after the declarations → 6,701; after the type check → 9,270
  (size-fatal).
* `bossSeen = shipGroup == 10` → `true` / `if (...) true` / ternary / `==1` /
  `!(...!=10)`: 8,532 / 9,136 / 6,013 / 6,013 / 6,013.
* `alwaysEnemySeen` spellings (`(bool)`, `!!`, `&1`, `==1`, `!=0`, `!=false`,
  ternary) and use-site `!=0`/`==1`: all 6,013.
* 120 random combinations of 2–4 neutral cells: all 6,013.
* `Status *status = Globals::status` local: 9,226 / 9,900 (size-fatal).
* `PaintCanvas *canvas = Globals::Canvas` local: 9,473 / 10,004.
* campaign guard operand spellings (`>1`, `(unsigned)>=2u`, `NULL`, `==0`):
  6,013–6,022.

## 3. Conclusion

The residual is still the single `campaign`/`%2487` colouring, but the new queue
table shows it is a **multi-eviction cascade started by the eight large unhinted
GOT-load ranges**, not a single `%2487` eviction.  Every source spelling tried
across waves 162–191b and this pass leaves the pre-RA IR byte-stable; the one
retype that reaches the allocator (`int alwaysEnemySeen`) is +44 B and lands
`campaign` in `r5`, not `sl`.  The two known semantic mismatches (music A,B,C vs
A,C,B; asteroid `+= mode` vs `= 0`) are size-exact fixes but raw-worse because
the RA cascade dominates.  No size-exact raw win was found; tree left clean.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13582/`: `cell.py` (1 s
compile+link+strip+fn_diff cell; replaces the Radar obj path in both the compile
and link commands), `batch2.py`–`batch14.py` (the sweeps above), `mir.txt`
(`-print-machineinstrs`), `prera.mir` (pre-RA region), `o.dis`/`u.dis`
(original/ours disassembly).
