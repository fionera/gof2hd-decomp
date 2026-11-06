# w6970: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-119
# adjacent-statement swap sweep + case-183 chain×default-body grid; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6970`, branch `wave119-w6970`,
based on current master (`d512ec24e`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
Continues `level-createmission-wave119-w6946.md` (Ghidra re-decompile +
anchor/order sweep), `…-w6925.md` (stack-slot / tail / convoy / turret sweep)
and `…-w6918.md` (negated `bigShipsCount`, 92.326733).
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`; one extra matching
instruction is `+0.0165 pp` (92.3267 -> 92.3432).

## 1. New evidence: the first quarter is mnemonic-identical

An address-aligned mnemonic-only comparison (`mnem.py`) shows **no mnemonic
difference at all before `+0x1446`** (the case-183 `offset` chain).  The whole
first quarter differs only in 45 operand slots:

| cluster | sites | what |
|---|---:|---|
| `sp+84` vs `sp+88` | 17 | `raid` / `&Globals::rnd` spill slots swapped |
| `sp+40/44/48` | 4 | `fortressGuns` / `extraAliens` / `stationPirates` 3-cycle |
| branch offsets | 5 | downstream of the case-183 +2 B |
| literal-pool `ldr` imm | 7 | downstream of the case-183 +2 B |
| `r1`↔`r2` rename | 8 | `enemies[8]->shipGroup = 10` / `delete cargo` block |
| `sp+48` vs `sp+44` | 2 | `stationPirates` store / sum read |

So the case-183 2-byte block-order difference is the *only* size-changing
structural delta before `+0x1446`, and it drags the literal-pool offsets and
7 downstream stack-slot references with it.

## 2. Abstracted (register/slot-free) diff: 117 blocks, 6 real shapes

Stripping registers, `[sp,#N]`, immediates and branch targets (`abstract.py`)
leaves 117 differing blocks.  Almost all are instruction scheduling.  The
genuine source-shape differences are the six already catalogued:

1. `+0x1446` case-183 `offset` chain: orig tests `[2,1,0]` with bodies laid
   out `[0][1][2][default]`; ours tests `[0,1,2]` with bodies
   `[2][0][1][default]` (MachineBlockPlacement, not source).
2. `+0x1964` case-2 convoy: orig stores `cargo` (+80) before `crateCaptured`
   (+76) and reloads `this->enemies` per statement; ours caches `enemies` in
   `r4` and stores `crateCaptured` first.
3. `+0x2e2c` / `+0x2f52`: orig branchless `(i != X) | !alienWave` and
   `(i != X) | !pirateFortress`; ours short-circuits.
4. `+0x32f0` alien-turret table select: orig real in-loop `cbz`; ours
   `cmp.w r1,r9,asr #31; it ne; movne` cmov (the `(i < 0)` anchor).
5. `+0x3326` alien-turret `setScaling` is issued *before* `setRotation` in
   ours but *after* `setPosition` in the original.
6. `+0x4186` case4Tail: orig keeps `fp = 1` (hostile arg) and
   `sl = &Globals::globals`, spilling `&Globals::rnd` at `sp+120`; ours keeps
   `fp = &Globals::rnd`.

## 3. Variants measured this session (all 17172 B unless noted)

Baseline 92.326733 %.

### 3a. Declaration / early-block reorderings (neutral unless noted)

| variant | % |
|---|---:|
| `fortressGuns` declared before `stationPirates` | 92.326733 |
| `alienGuns` declared before `stationPirates` | 92.326733 |
| both declared before `stationPirates` | 92.326733 |
| `alienGuns`/`fortressGuns` swapped | 92.326733 |
| `fortressGuns` / `alienGuns` / both after `extraAliens` | 92.326733 |
| `raid` declared before `security` (assignment stays put) | 92.326733 |
| `raid` declared after `raidCoords` | 75.721253 (17332 B) |
| `raid` single-expression `!(intro && !hc) && roll < …` | 92.326733 |
| `raid` ternary `(intro && !hc) ? false : roll < …` | 92.326733 |
| `raid = false; if (!(…)) raid = …` | 92.326733 |
| `raid = 0` / `int raid = 0` / `unsigned raid` | 92.326733 / 92.161716 / 92.161716 |
| `raid = !(intro && !hc) & (roll < …)` | 76.432700 (17332 B) |

### 3b. Case-183 `offset` chain × default-body order grid (36 runs)

All 6 if-chain permutations (`210`, `201`, `120`, `102`, `021`, `012`) crossed
with all 6 default-body store orders (`120`, `012`, `021`, `201`, `102`,
`210`).  Best is the baseline `chain210-d120` at **92.326733**; every
`d012` form is 88.5–88.6 (17224 B), `d021` 89.5–91.8, `d201`/`d210`
92.18–92.31, `d102` 89.7–89.8 (17188 B).  No combination beats HEAD.

### 3c. Turret loop / table select

| variant | % |
|---|---:|
| **baseline (fresh load, `setScaling` before `setRotation`)** | **92.326733** |
| `setScaling` after `setRotation` (fresh load) | 92.326733 |
| `setScaling` after `setPosition` (fresh load) | 92.277228 |
| `setScaling` after `setPosition`, `gun` not fresh load | 92.209936 |
| `setScaling` before `setRotation`, `gun` | 90.955603 (17164 B) |
| `setScaling` after `setRotation`, `gun` | 90.939099 (17164 B) |
| `if/else` table select with anchor | 92.326733 |
| `switch (bigAlienWave)` table select (anchor dropped) | 88.413577 (17140 B) |
| plain `bigAlienWave ? A : B` | 88.386298 (17148 B) |
| swap the two turret `if` blocks | 88.081875 (17164 B) |

### 3d. Case-2 convoy cargo/crateCaptured block

| variant | % |
|---|---:|
| **baseline (`enemies` cache, `convoyShip`, cargo then crate)** | **92.326733** |
| `convoyShip` cache, crate first | 92.326733 |
| `enemies` cache + `convoyShip` + `(*enemies)[i]` crate | 92.103309 (17164 B) |
| no `enemies` cache, `convoyShip` local | 90.009075 (17180 B) |
| no `enemies` cache, no `convoyShip` | 90.151765 (17188 B) |

### 3e. `case4Tail` loop

| variant | % |
|---|---:|
| **baseline `volatile unsigned n` latch** | **92.326733** |
| `true,false` -> `1,0` / `true,0` | 92.326733 |
| faithful `i < this->enemies->size()` | 91.341312 (17164 B) |
| faithful + `1,0` / `(unsigned)` cast | 91.341312 (17164 B) |
| `i < (unsigned) hunterCount` | 91.309730 (17164 B) |

### 3f. Adjacent-statement swap sweep (162 runs)

Every pair of adjacent single-line statements (same indent, balanced parens,
no control-flow keyword) inside the `mission->isEmpty()` block (source lines
1545–2140) was swapped and measured.  **None beat 92.326733.**  The best
non-baseline results were 92.310231 (`swap1611_1612`, `swap1772_1773`,
`swap1973_1974`, `swap1999_2000`) and 92.293729 (`swap1553_1554`,
`swap1820_1821`, `swap1870_1871`, `swap1876_1877`, `swap1885_1886`,
`swap1992_1993`, `swap2003_2004`); the rest were lower, and 11 variants failed
to build (dependent statements reordered).

### 3g. Misc

| variant | % |
|---|---:|
| `if (alienGuns != 0)` / `if (alienGuns)` | 92.326733 |
| `if (pirateFortress != 0)` / `stationPirates != 0` / `convoy != 0` / `blueprint != 0` / `nameLeader != 0` | 92.326733 |
| `if (extraAliens > 0)` / `if (extraAliens)` | 92.326733 / 92.310231 |
| `enemies[8]` `shipGroup`/`field_0x74` swapped | 92.326733 |
| `enemies[9]` `shipGroup`/`field_0x74` swapped | 92.310231 |
| `KIPlayer *ship8 = (*enemies)[8]` local | 89.980191 (17156 B) |
| `Status *status = Globals::status;` cached at fn top | 60.683972 (17252 B) |
| `&` / explicit `!((i != X) \| !alienWave)` for the `&&` | 75.705562 / 82.997689 |

## 4. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~260 new variants were measured this session on top of the ~500 in the
preceding wave-119 notes; none beat the baseline.  The residual is compiler
register allocation, stack-slot colouring, if-conversion and
MachineBlockPlacement, not source shape.  The single largest addressable
cluster remains the `raid` / `&Globals::rnd` `sp+84`/`sp+88` swap (17 sites,
+0.28 pp if isolated), which no source spelling moves.

## 5. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6970/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w6970/dump2.py _ZN5Level13createMissionEv 0x4140 0x41a0   # address-aligned
python3 /dev/shm/w6970/mnem.py             # mnemonic-only address-aligned diff
python3 /dev/shm/w6970/abstract.py         # register/slot-free structural diff
python3 /dev/shm/w6970/search.py sN.json   # apply/build/score/restore a variant batch
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w6970` (`Level.base.cpp` = pristine HEAD copy;
`s1.json`…`s13.json` = variant batches; `s6a/s6b/s6c.json` = the 162-swap
sweep; `abstract.txt`, `mnem.txt`, `addrcmp.txt` = diffs).
