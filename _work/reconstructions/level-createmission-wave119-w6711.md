# w6711: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119:
# variant sweep around the stack-slot swap, the case-183 chain, the alien-turret
# register allocation and the extraAliens/raid loop bounds; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6711`, branch `wave119-w6711`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine, wave-118 `volatile unsigned n` latch)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             shape    96.006601 %    (registers + [pc,#N] + loc_* collapsed)
                             regonly  95.000000 %    (registers collapsed only)
                             nopc     92.986799 %    ([pc,#N] collapsed only)
                             6055 orig / 6065 ours insns
gate: byte 4503/4524, linked 4503/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip -8 only).
```

`shape`/`regonly`/`nopc` are computed with `/dev/shm/w6711/shape.py` (difflib over the
`verify.py` normalized stream).  The gap official->regonly (2.7 points) is *entirely*
register-name selection; official->shape (3.7 points) adds the pc-relative literal
offsets and intra-function `loc_*` targets.

## 1. Result

No source variant beat the baseline.  The best measured score remains
**92.293729 % / 17172 B / 6065 insns**.  `src/game/world/Level.cpp` is unchanged
(this note is the only commit).

## 2. Diff census (baseline)

`/dev/shm/w6711/diffmap.py` (uses the same normalisation as `tools/verify/verify.py`
and maps each cluster to a `Level.cpp` line via `addr2line` on the -g object):

```text
total diff 542 instructions in 308 clusters
  register-name-only diffs : 140
  everything else          : 402  (of which most are `loc_*` branch targets that
                                   shift because of the size deltas below)
```

Largest clusters (section offsets are relative to the function start 0xaffd8):

```text
len 13 @0x32ee  Level.cpp:1963   alien-turret table select + setScaling order
len 12 @0x397c  Level.cpp:2005   raid loop preheader (this in r5 vs r4)
len  8 @0x2e2c  Level.cpp:1904   `i == fc+164 && alienWave` lowered as two branches
len  8 @0x1444  Level.cpp:2192   case-183 convoy offset if-chain (test/body order)
len  7 @0x3e02  Level.cpp:2062   extraAliens loop bound (ours 2 insns longer)
len  7 @0x3c94  Array.h:41       raid loot spawnPosition kept in VFP regs in orig
len  7 @0x332c  Array.h:41       setScaling issued through a fresh enemies load
len  6 @0x3502  Level.cpp:1982   alien-turret loop body
len  6 @0x2f54  Level.cpp:1924   alienWave body
len  5 @0x3256  Matrix.h:19      turret Matrix zero-init
len  5 @0x1960  Level.cpp:2293   case-183 convoy loop
```

First *code* size divergence: `0xb142a` (inside the case-183 offset if-chain).
Ours' test chain is 2 bytes longer there (`cmp #0;beq;cmp #1;beq;cmp #2;bne` vs the
original's `cmp #2;beq;cmp #1;beq;cbnz`), which shifts every later `loc_*` target by
2..8 bytes and is responsible for most of the 402 non-register diffs.

## 3. What the original actually looks like

Re-decompiled with REA/Ghidra 12.1.2 and cross-checked against the Thumb disassembly.

### 3.1 Stack-slot swap (`raid` vs the cached `Globals::rnd`)

First mismatch is instruction 28: original `str.w fp,[sp,#88]` (`Globals::rnd` spill at
sp+88, `raid` at sp+84); ours `str.w fp,[sp,#84]` (reversed).  17 refs each way.
The `raid` computation itself (`b04a8`-`b04d8`) is byte-identical apart from the slot.

MIR evidence (LLVM 7.2 `-mllvm -print-after-all`): after Stack Slot Coloring
`raid = %stack.83`, `rnd = %stack.80`; higher frame index lands at the higher address,
so the original must have coloured `raid` *before* `rnd` (i.e. the `raid` spill slot
was created first).  The two slots are otherwise interchangeable 4-byte spills.

Variants tried (all keep 17172 B unless noted):

| variant | official % | slots 84/88 |
|---|---|---|
| baseline `bool raid;` + if/else | **92.293729** | 2 / 17 |
| `int raid;` | 92.128713 | 2 / 17 |
| `unsigned raid;` / `char` / `short` / `long` | 92.128713 | 2 / 17 |
| `int raid = 0;` | 92.128713 | 2 / 17 |
| `volatile bool raid;` | 82.483077 (17196 B) | 2 / 11 |
| `volatile int raid;` | 83.034756 (17156 B) | 2 / 11 |
| `bool raid = false;` + if/else | 92.293729 | 2 / 17 |
| `bool raid = cond ? false : ...` | 92.293729 | 2 / 17 |
| `bool raid; raid = cond ? ... : ...;` | 92.293729 | 2 / 17 |
| declaration moved before `security` | 92.293729 | 2 / 17 |
| declaration moved after `pirates` | 92.293729 | 2 / 17 |
| declaration moved before `raidRoll` | 91.452145 | 2 / 17 |
| declaration moved before `stationIndex` | 91.452145 | 2 / 17 |
| declaration at top of `if (mission->isEmpty())` | 91.452145 | 2 / 17 |

The only variant that flipped the slot order was caching `this->enemies` in a local
before the alien-turret loop (`84:13 / 88:2`), but that costs +44 B (17216 B) and
drops the score to 73.697148 (a size change shifts every later literal-pool offset).

### 3.2 Alien-turret register allocation (dominant residual)

Original keeps `this` in **r4** from the turret preheader through the loop
(`gun`=r6, `&return-slot`=r5, `k*24`=r9, `i`=fp, `t`=sl).  Ours reloads `this` into
**r5** at `b3280` (`gun`=r4, `&return-slot`=r8, `k*24`=fp, `i`=r9, `t`=r6).  The same
swap then propagates through the `pirateFortress` loop and the whole `if (raid)` block
(original r4=this, ours r5=this) — that is the 46/23/21/18/15-instruction clusters.

The original *also* reloads `this` from `sp+124` inside the loop (`b336e`, just before
the loop-condition `ldmia r2,{r0,r1,r2}`), so this is purely a greedy-allocator
tie-break between two callee-saved registers, not a live-range difference.

Variants tried (all neutral at 92.293729 / 17172 B unless noted):

* `Vector base` declared before `Matrix rotation` -> 92.062706
* `Vector v` declared before `Matrix rotation` -> 92.161716
* `setScaling` on the cached `gun` (no fresh `(*enemies)[i]`) -> 90.922594 (17164 B)
* faithful order (setScaling after setPosition) + fresh load -> 92.244224
* faithful order + cached gun -> 92.176927
* plain ternary (no `i<0` anchor) -> 88.380543 (17140 B)
* `setPosition`/`setRotation` issued through a fresh `(*enemies)[i]` -> 90.90 (17180 B)
* `gun->cargo` before `gun->shipGroup` -> 92.277228
* `gun->cargo = nullptr` before `setScaling` -> 92.178218
* `for (int i=..., k=0; ...)`, `unsigned k`, `i != bound`, `Vector v;` -> neutral
* `unsigned i` / anchor folds -> 88.380543 (17140 B)

### 3.3 Case-183 convoy offset if-chain (`Level.cpp:2192-2210`)

Original: tests `2,1,0` (`cmp #2;beq;cmp #1;beq;cbnz r1,default`), bodies laid out
`0,1,2,default`.  Ours (baseline source order `2,1,0`): tests `0,1,2`
(`cmp #0;beq;cmp #1;beq;cmp #2;bne`), bodies `2,0,1,default`.

| variant | official % | size |
|---|---|---|
| baseline `if (k==2)...else if(k==1)...else if(k==0)...else` | **92.293729** | 17172 |
| source order `0,1,2` (tests become 2,1,0 = original!) | 92.145215 | 17172 |
| `switch (k) { case 0,1,2, default }` | 92.145215 | 17172 |
| `switch (k) { case 2,1,0, default }` | 92.293729 | 17172 |
| `switch (k) { case 0,1,2,3 }` (jump table `tbb`) | 92.218830 | 17172 |
| nested `if/else { if/else { if/else } }` | 92.293729 | 17172 |
| `switch` default-first / `k != 0/1/2` nested chains | 92.293729 | 17172 |
| `k >= 2` / `k > 1` nested | 89.668262 | 17164 |
| 36 body-statement permutations | best non-baseline 88.6 | 17224 |

Key finding: source order `0,1,2` reproduces the original's *test chain exactly*
(`cmp #2;beq;cmp #1;beq;cbnz`), but clang then lays the bodies out `0,2,1,default`
instead of `0,1,2,default`, and the body swap costs more than the test chain gains
(92.145215 < 92.293729).  A `switch` with cases `0,1,2` produces identical code.

### 3.4 `i == fc+164 && alienWave` (`Level.cpp:1904`) and the second `&&`

Original computes `(i != fc+164) | !alienWave` and branches once
(`subs; it ne; movne 1; ldr alienWave; eor #1; orrs; bne`).  Ours short-circuits
(`ldr alienWave; cmp #0; beq; cmp fp,r3; bne`).  Same for the `&& pirateFortress`
else-if at `b2f2a`.

The optimized IR is identical in both (`icmp sgt` + `and i1`), so this is a
SimplifyCFG/block-placement difference driven by the rest of the function.  Variants
tried, all neutral at 92.293729: operand swap `alienWave && i==...`, named
`bool atStart = i == ...`, `alienWave != 0`, `(i == ...) & alienWave` (75.689058,
17172 B), `(i == ...) & pirateFortress` (63.045811, 17312 B).

### 3.5 `*bigShipsCount > 0` (`Level.cpp:1837`)

Original `cmp r0,#0; ble`; ours `cmp r0,#1; blt` (both 2-byte, semantically equal).
The IR is `icmp sgt i32 %x, 0` in both.  Tried and neutral (92.293729):
`>= 1`, `0 < *p`, `!localRace`, `localRace < 1`, `field_168 > 0`, `field_168 >= 1`,
`int *const`, `const int*`.  `!= 0` -> 89.67 (17164 B); swapping the `&&` operands ->
74.19 (17164 B).

### 3.6 extraAliens loop bound (`Level.cpp:2058-2063`)

Original preheader is 11 instructions, ours 13:

```text
orig:  ldmia r3,{r0..r3}; ldr r6,[sp,#48]; ldr r4,[sp,#92]; add r6,r4;
       ldr r4,[sp,#20]; add ip,r6,r4; ldr r6,[sp,#44]; add r4,ip,r6;
       ldr r6,[sp,#32]; add r4,r6; add r6,ip; add r6,r0..r3
ours:  ldmia r3,{r0..r3}; ldr lr,[sp,#32]; ldr r6,[sp,#40]; ldr r4,[sp,#44];
       add ip,lr,r6; ldr r6,[sp,#112]; add r4,r6; ldr r6,[sp,#20]; add r6,r4;
       add r4,ip,r6; add r6,lr; add r6,r0..r3
```

The original groups its base as `(A+B+C)+E` and derives the end as `base+extraAliens`,
i.e. `pirates + start` is CSE'd.  Source variants tried:

| variant | official % | size |
|---|---|---|
| baseline (`int start` + two long expressions) | **92.293729** | 17172 |
| `int alienBase = pirates + start + fc + 164 + 168 + hostile; for (i=alienBase; i<alienBase+extraAliens; i++)` | 90.628609 | 17168 |
| `int alienBase = pirates + start;` + two expressions | 91.056106 | 17164 |
| `int start = stationPirates+guns+pirateEscorts+pirates;` + two expressions | 91.056106 | 17164 |
| inline `start` (no local) + two expressions | 91.131095 | 17168 |

All are *smaller* than 17172, so the original must keep the "inefficient" recomputation
with a different association, not a cached base.

### 3.7 Raid loot `spawnPosition` (`Level.cpp:2016-2023`, cluster @0x3c94)

Original keeps the `spawnPosition` components in VFP registers
(`vadd.f32 s2,s16,s22` / `vadd.f32 s4,s18,s20`); ours reloads `sp+312/316/320`
(`vldr s2,[r,#312]` ...).  4 extra instructions.  This is inside the
`if (inPirateLootOrbit())` block of the raid loop and is a downstream effect of the
`this`-in-r5 allocation (same block, same registers).

## 4. Residual analysis (unchanged)

* **`this` register** r4 (orig) vs r5 (ours) from the alien-turret preheader onward;
  `gun` r6 vs r4; `&return-slot` r5 vs r8; `k*24` r9 vs fp; `i` fp vs r9; `t` sl vs r6.
  Dominant residual (~2.7 points).
* **Stack-slot swap** `raid` (orig sp+84 / ours sp+88) vs `Globals::rnd`
  (orig sp+88 / ours sp+84); first mismatch at insn 28, 34 refs total.
* **Alien-turret selector**: original in-loop `ldr r0,[sp,#64]; cbz`; ours in-loop
  `cmp.w r1,r9,asr #31; it ne; movne`.  The `i<0` anchor is required to keep the
  select in-loop and preserve 17172 B (plain ternary -> 17140 B / 88.38 %).
* **Case-183 convoy chain**: test order 2,1,0 (orig) vs 0,1,2 (ours); the 2-byte test
  chain delta shifts every later `loc_*` target.
* **`&&` conditions** at 1904/1907: original ORs both negated operands, ours
  short-circuits.
* **`cmp #0;ble` vs `cmp #1;blt`** at 1837.
* `.ARM.extab` still 8 B short (`0x26290` vs `0x26298`, createShip -8); 27 sections and
  `2,187,328 B` file size unchanged by every variant above.

## 5. Commands

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6711/score.py            # official %
python3 /dev/shm/w6711/shape.py            # shape / regonly / nopc %
python3 /dev/shm/w6711/diffmap.py          # per-cluster census + source lines
python3 /dev/shm/w6711/topdiff.py N        # dump the N-instruction clusters
cmake --build cmake-build-match --target check -j6
```

Scratch tools (not tracked): `/dev/shm/w6711/{score,shape,diffmap,topdiff,classify,
idxaddr,slots.sh,vt,vt2}.py`, `Level.base.cpp` (== HEAD), `Level.ll` (optimized IR),
`irpass.txt` (`-mllvm -print-after-all` MIR for createMission), `orig.dis`, `ours.dis`,
`rea.c`, `rea.json`.
