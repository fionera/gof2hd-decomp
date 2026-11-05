# w6756: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119:
# convoy-body literal-pool confirmation, `this`-reload sweep, stack-slot rotation;
# no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6756`, branch `wave119-w6756`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine HEAD)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. Result

No source variant beat the baseline. Best measured score remains
**92.293729 % / 17172 B / 6065 insns**. `src/game/world/Level.cpp` is
unchanged; this note is the only commit.

## 2. New finding: all four convoy bodies store `offset[1]; offset[0]; offset[2]`

The original's four `k`-chain bodies (case-183 convoy loop, `Level.cpp:2193`)
were decoded against the literal pool at `b15bc`-`b15e4`:

```text
pool   b15bc c53b8000 -3000    b15c0 469c4000 20000   b15c4 c7435000 -50000
       b15c8 c5dac000 -7000    b15cc 461c4000 10000   b15d0 c6ea6000 -30000
       b15d4 46c35000 25000    b15d8 c70ca000 -36000  b15dc 45bb8000 6000
       b15e0 463b8000 12000    b15e4 c72fc800 -45000
```

Every body issues `str [sp,#376]` (offset[1]), `str [sp,#372]` (offset[0]),
then `b b144e`/fall-through to the shared `str [sp,#380]` (offset[2]).
So the original's source order is `offset[1]; offset[0]; offset[2]` for
**case 2, case 1, case 0 and the default alike** (the shared tail is
offset[2] for all four).

Our source writes `offset[0]; offset[1]; offset[2]` for the three cases and
`offset[1]; offset[2]; offset[0]` for the default. Clang reorders the three
cases to `1,0,2` (matching), but the default to `2,1,0`, so its offset[2]
store is *not* the shared tail and the default block is placed out of line
(`b1444 str [sp,#380]; b1446 b b1454; b1448 default`). This is the first
size divergence (`0x144e`, previously documented in the w6737 note).

Making the default body `offset[1]; offset[0]; offset[2]` (the original's
order) does share the tail, but clang then **sinks the offset[2] store into
a VFP register**: `vmov.f32 s0,s16/s18/s20/s22` per body + `vstr s0,[sp,#380]`
in the tail, growing the function to 17188 B.

| convoy default body source order | official % | size | insns |
|---|---:|---:|---:|
| `1,2,0` (baseline) | **92.293729** | 17172 | 6065 |
| `1,0,2` (matches original) | 89.653465 | 17188 | 6065 |
| all four bodies `1,0,2` | 88.606760 | 17224 | 6075 |

`switch (k)` was re-measured for all 12 case/default permutations: it is
byte-identical to the if-chain in every case (confirming w6314), best
92.293729.

## 3. New finding: a second stack-slot rotation (`sp+40/44/48`)

Besides the known `raid` / `&__stack_chk_guard` swap (`sp+84`/`sp+88`),
`stationPirates`, `extraAliens` and `pirateEscorts` are a **3-cycle**:

| variable | original | ours |
|---|---:|---:|
| `pirateEscorts` (def `b2a38`) | sp+40 | sp+48 |
| `extraAliens` (def `b0848`) | sp+44 | sp+40 |
| `stationPirates` (def `b06b4`) | sp+48 | sp+44 |

All are 4-byte `[SP]` slots; the refs are at identical instruction addresses
in both builds (`stationPirates` 6, `extraAliens` 6, `pirateEscorts` 4), so
this is again `StackSlotColoring` / pre-coloring frame-index order, not
source declaration order. Moving the declarations/definitions later
(`pe_late`, `sp_late`, `both_late`, `ea_late`) is catastrophic (64-74 %).

## 4. `this` reload at the alien-turret preheader (dominant residual)

Pre-RA MIR confirms `this` is a single value `%666` used 551 times; the RA
splits it. Original:

```text
b322e ldr.w sl,[sp,#52]   ; alienGuns
b3232 ldr r4,[sp,#124]    ; this   <-- hoisted above the branch
b3234 cmp.w sl,#0
b3238 beq.w b3398
...
b3274 ldr.w r0,[r4,#248]  ; base getPosition
b3286 add.w r2,r4,#352    ; loop init
b329a mov r0,r4           ; body createStaticObject uses r4
b32da add r4,sp,#312      ; r4 clobbered by &rotation
b336e ldr r4,[sp,#124]    ; reload for loop bound
```

Ours loads `this` after the branch (`b3260`) and reloads it again at the body
start (`b3280 ldr r5,[sp,#124]`), so the whole turret/pirateFortress/raid
allocation permutes (`this` r4 vs r5, `gun` r6 vs r4, `&return-slot` r5 vs
r8, `k*24` r9 vs fp, `i` fp vs r9, `t` sl vs r6). The pirateFortress block
reloads into r5 in *both* builds; only the turret block differs.

Variants measured this wave (all keep 27 sections / 2,187,328 B):

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `Level *self = this;` inside the turret block (all uses via `self`) | 92.293729 | 17172 | 6065 |
| `Array<KIPlayer *> *enemies = this->enemies;` in the block | 90.865583 | 17164 | 6064 |
| `enemies` + `fc/f164/f168` locals | 82.011062 | 17148 | 6058 |
| `fc/f164/f168` locals only | 81.848948 | 17156 | 6060 |
| faithful body (cached `gun`, `setScaling` after `setPosition`) | 92.176927 | 17172 | 6063 |
| `setScaling` after `setRotation` (fresh load) | 92.293729 | 17172 | 6065 |
| `cargo = nullptr` before `shipGroup` | 92.277228 | 17172 | 6065 |
| `Vector v;` / `Vector v(0,0,0)` / `Matrix rotation = Matrix()` | 92.293729 | 17172 | 6065 |
| `Vector base(...)` direct-init | 92.293729 | 17172 | 6065 |
| `const Vector base` | 92.293729 | 17172 | 6065 |
| `Vector v` before `Matrix rotation` | 92.161716 | 17172 | 6065 |
| `int k` derived from `i` inside the loop | 70.031286 | 17316 | 6091 |

## 5. `&&` materialisation (line 1904) still short-circuits

Original `b2e04`: `subs.w r0,fp,r3; mov r1,r0; it ne; movne r1,#1;
ldr r2,[sp,#60]; eor.w r2,r2,#1; orrs r1,r2; bne.w` — a single branch on
`(i != X) | !alienWave`. Ours emits two short-circuit branches. All spellings
tried are score-identical (92.293729 / 17172): `&&`, `&`, `alienWave != 0`,
`alienWave && i == X`, `pirateFortress != 0`. The w6314 `SinkCmpExpression`
conclusion stands.

## 6. Other measured variants (all <= baseline)

| variant | official % | size |
|---|---:|---:|
| `bool raid, resetStation;` grouped declaration | 92.293729 | 17172 |
| `bool raid = false;` + negated if | 92.293729 | 17172 |
| `int raid` | 92.128713 | 17172 |
| `bool raid;` moved above `raidRoll` | 91.452145 | 17172 |
| `bool raid;` at function top | 91.452145 | 17172 |
| `float pirateEscortsF; int pirateEscorts = (int) pirateEscortsF;` | 92.293729 | 17172 |
| `pirateEscorts` via a second `stationHasPirateBase()` call | 88.149860 | 17188 |
| `bool stationPirates` | 86.319782 | 17176 |
| `unsigned extraAliens` | 92.277228 | 17172 |
| `unsigned security` | 92.169321 | 17172 |
| `unsigned raidRoll` | 92.260726 | 17172 |
| `*bigShipsCount >= 1` / `0 < *bigShipsCount` | 92.293729 | 17172 |
| `*bigShipsCount != 0` | 89.666557 | 17164 |
| `pirateFortress` test `> 0` instead of `>= 1` | 92.293729 | 17172 |
| `extraAliens` declared before `alienChance`/`alienPercent` | 92.293729 | 17172 |
| `blueprint` declared before `pirateEscorts` | 75.489712 | 17408 |
| ArraySetLength sum: `stationPirates + pirateEscorts + ...` | 92.293729 | 17172 |
| ArraySetLength sum reversed | 91.641225 | 17172 |
| ArraySetLength sum: `extraAliens` first | 68.988282 | 17304 |

## 7. Residual map (unchanged)

* `this` r4 (orig) vs r5 (ours) from the alien-turret preheader onward
  (dominant, ~2.7 pp).
* `raid`/`&__stack_chk_guard` slot swap (`sp+84`/`sp+88`, 19 diff regions)
  and the `pirateEscorts`/`extraAliens`/`stationPirates` 3-cycle
  (`sp+40/44/48`, 6 diff regions).
* Convoy `k`-chain default body out of line (`0x144e`, 4 B) -> all
  subsequent `ldr/vldr [pc,#N]` offsets (48 diff regions).
* `&&` at 1904/1907 short-circuits vs the original's `orrs`.
* `cmp #0;ble` (orig) vs `cmp #1;blt` (ours) at `b0eac` (`*bigShipsCount > 0`).
* `.ARM.extab` still 8 B short (`0x26290` vs `0x26298`, createShip only).

## 8. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6756/score.py                 # official %, size, insns
python3 /dev/shm/w6756/diff.py > diff.txt       # per-cluster opcode diff
python3 /dev/shm/w6756/sweep.py <variants.json> # apply/build/score sweep
cmake --build cmake-build-match --target check -j6
```

Scratch (not tracked): `/dev/shm/w6756/{score,diff,sweep,tryshow,addrdiff}.py`,
`Level.base.cpp` (== HEAD), `orig_full.dis`, `ours_full.dis`, `all.txt`
(`-mllvm -print-after-all` for `_ZN5Level13createMissionEv`), `rea.c`,
`v_183.json`, `v_final{,2,3,4}.json`, `v_this.json`, `v_102.json`.
