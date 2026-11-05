# w6733: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# angle-B sweep: turret-loop register allocation, selector anchor, case-183 chain,
# `&&` materialisation, extraAliens/raid loop shapes; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6733`, branch `wave119-w6733`,
based on current master (`fcd88b45b`). Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine HEAD, wave-118 `volatile unsigned n` latch)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
                             instr mix orig 3524x16b + 2355x32b + 175 pool
                                       ours 3544x16b + 2344x32b + 176 pool
gate: byte 4503/4524, linked 4503/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .text 0x1337c8 both,
     .ARM.extab 0x26290 (8 B short, createShip -8 only).
```

`check` was run before and after this pass; the worktree is left with only this
note (`src/game/world/Level.cpp` is unchanged).

## 1. Result

No source variant beat the baseline. Best measured score remains
**92.293729 % / 17172 B / 6065 insns**. This note is the only commit.

## 2. What the original actually looks like (re-confirmed with REA/Ghidra 12.1.2)

REA decompile (`rea decompile ... --procedure _ZN5Level13createMissionEv`)
plus the Thumb disassembly confirm the source shape already encoded in
`Level.cpp` for every region touched below:

* alien-turret loop: `createStaticObject(nullptr,6772,true)` -> store into
  `(*this->enemies)[i]` -> reload `gun` from the array -> `setMaxHitpoints(1000)`
  -> plain `bigAlienWave ? kAlienStationTurrets : kAlienOutpostTurrets`
  (a **branch**, `ldr r0,[sp,#64]; cbz`) -> two `MatrixRotateVector` calls ->
  `geometry->setRotation` -> `setPosition` -> `setScaling` on the **cached**
  `gun` -> `shipGroup` / `cargo`.
* pirate-fortress loop: same shape with `kBaseTurrets[k]` and `shipGroup = 1`.
* case-183 convoy loop: `for (k=0;k<4;k++)`, `float offset[3]`, source order
  `k==2 / k==1 / k==0 / else`.
* extraAliens `for` loop and the raid `for` loop as written.

The current source deliberately deviates in two places (both needed for the
17172 B size / block layout):
1. the turret selector carries the fake `i < 0` anchor
   (`(bigAlienWave + (i < 0))`) which turns the original's in-loop `cbz`
   branch into `cmp.w r1,r9,asr #31; it ne; movne`;
2. `setScaling` is issued through a **fresh** `(*this->enemies)[i]` load
   before `setRotation` instead of the cached `gun` after `setPosition`.

## 3. Residual analysis (unchanged by this pass)

### 3.1 `this` register (dominant, ~2.7 pp)

Everything up to normalised insn 4488 is register-identical between the two
binaries (the first real code difference is insn 1368, the known
`cmp #0;ble` vs `cmp #1;blt`). The divergence starts exactly at the
alien-turret preheader:

```text
orig  b322e  ldr.w sl,[sp,#52]      ; alienGuns
      b3232  ldr r4,[sp,#124]       ; this  <- in the CONDITION block
      b3234  cmp.w sl,#0
      b3238  beq.w b3398            ; -> pirateFortress
      ...
      b329a  mov r0,r4              ; loop body uses r4
      b32da  add r4,sp,#312         ; r4 clobbered as &rotation
      b336e  ldr r4,[sp,#124]       ; reload this for the loop bound

ours  4538  ldr r6,[sp,#52]
      4539  cmp r6,#0
      4540  beq loc_33bc
      4558  ldr r4,[sp,#124]        ; this  <- inside the body
      4570  ldr r5,[sp,#124]        ; second reload, loop body uses r5
```

Post-RA MIR (`-mllvm -print-after-all`, LLVM 7.2) for ours:

```text
bb.249 (turret preheader):  %4908:gpr = t2LDRi12 %stack.75, 0   -> $r4
bb.252 (turret body):       renamable $r5 = t2LDRi12 %stack.75  (new reload)
                            ... $r4 = t2LDRs ... (gun)
```

So `%4908` (`this`) *is* assigned r4 in the preheader; the allocator then
splits it at the loop boundary and reloads into r5, and `gun` (%3065) takes
r4. In the original `this` keeps r4 through the loop body and gets clobbered
by `&rotation`, which forces the in-loop reload at b336e; `gun` is r6.
The whole turret/pirateFortress/raid allocation permutation
(`this` r4 vs r5, `gun` r6 vs r4, `&return-slot` r5 vs r8, `k*24` r9 vs fp,
`i` fp vs r9, `t` sl vs r6) follows from this one split decision.

### 3.2 First mismatch is the `raid` spill slot, not `&Globals::rnd`

Confirmed against the w3835 correction: the swapped 4-byte RA spill slots are
`raid` (orig fi#84 / ours fi#83) and the **`&__stack_chk_guard` address**
(orig fi#83 / ours fi#84). `&Globals::rnd` is at sp+120 in both. This is
allocator frame-index ordering, not declaration order.

### 3.3 Other confirmed residuals

* `if (i == fc+164 && alienWave)` at 1904 and the `&& pirateFortress`
  else-if: original materialises `(i != X) | !alienWave` and branches once
  (`subs.w; mov; it ne; movne #1; ldr; eor #1; orrs; bne`); ours
  short-circuits (`ldr; cmp #0; beq; cmp; bne`).
* `*bigShipsCount > 0`: orig `cmp #0; ble`, ours `cmp #1; blt` (same size).
* case-183 chain: orig tests `2,1,0` with bodies `0,1,2,default`; ours
  (source order `2,1,0`) tests `0,1,2` with bodies `2,0,1,default`.
  This is the first code-size divergence (2 B, at `0x144e`).
* extraAliens preheader: orig 11 insns, ours 13 (different reassociation).
* raid-loot / extraAliens `spawnPosition` components stay in VFP regs in the
  original, are reloaded from `sp+312/316/320` in ours.
* `.ARM.extab` 8 B short (`0x26290` vs `0x26298`, createShip -8 only);
  27 sections and 2,187,328 B file size unchanged by every variant below.

## 4. Variants measured this wave (official verifier)

All measurements are `official % / size`; baseline is `92.293729 / 17172`.
Every variant keeps 27 sections, file size 2,187,328 B and `.ARM.extab`
0x26290.

### 4.1 Turret block (selector / setScaling / declaration order)

| variant | official % | size |
|---|---|---|
| **baseline** (anchor `i<0`, fresh setScaling before setRotation, shipGroup then cargo) | **92.293729** | 17172 |
| faithful (plain ternary, cached gun, setScaling after setPosition) | 88.367869 | 17140 |
| plain ternary + fresh setScaling | 88.380543 | 17140 |
| cached gun, current order | 90.922594 | 17164 |
| cached gun, faithful order | 92.176927 | 17172 |
| setScaling between setRotation and 2nd rotate (fresh) | 92.293729 | 17172 |
| setScaling at end (fresh) | 92.244224 | 17172 |
| `cargo = nullptr` before `shipGroup` | 92.277228 | 17172 |
| `int guns = alienGuns + fortressGuns;` hoisted above the turret blocks | 92.293729 | 17172 |
| `int guns = fortressGuns + alienGuns;` | 92.293729 | 17172 |
| `int k = 0;` before `Vector base` | 92.293729 | 17172 |
| `Vector v` before `Matrix rotation` | 92.161716 | 17172 |
| `Vector base` before `Matrix rotation` | 92.062706 | 17172 |
| `Matrix rotation` after base / after k | 92.128713 | 17172 |
| `Vector base; base = ...;` | 90.483259 | 17188 |
| `Vector v(0,0,0)` | 92.293729 | 17172 |
| `const int alienGuns/fortressGuns` | 92.293729 | 17172 |
| `int i` declared outside the `for` | 92.293729 | 17172 |
| `while (i < bound) { ...; i++; k++; }` | 92.293729 | 17172 |
| `int first = fc+f164;` reused for base+init+`i==first` | 79.733070 | 17248 |
| `int first` only for the base index | 92.293729 | 17172 |
| `PlayerTurret *gun = (PlayerTurret *) (*enemies)[i];` | 92.293729 | 17172 |
| `t` declared before `gun` | 92.293729 | 17172 |
| `Array<KIPlayer*> *enemies = this->enemies;` inside the loop body | 90.906090 | 17164 (`this`->sl) |
| no `gun` local (all `(*enemies)[i]`) | 88.558938 | 17204 (`this`->sl) |
| `Vector v` declared inside the loop | 88.542183 | 17156 |
| `if (alienGuns != 0 && this != nullptr)` | 92.062706 | 17172 |
| `if (alienGuns != 0 && this->enemies != nullptr)` | 75.925163 | 17368 (`this`->sl) |
| pirateFortress block swapped before alienGuns block | 88.048861 | 17164 |
| pirateFortress `Vector base` before `Matrix rotation` | 90.922582 | 17192 |

Selector-anchor alternatives (all with the baseline body):

| anchor | official % | size |
|---|---|---|
| `(bigAlienWave + (i < 0))` (baseline) | **92.293729** | 17172 |
| `(bigAlienWave + (i >> 31))` | 92.293729 | 17172 |
| `(bigAlienWave - (i < 0))` | 92.293729 | 17172 |
| `(bigAlienWave ^ (i < 0))` | 92.293729 | 17172 |
| `(bigAlienWave + (i < 0) + 0)` | 92.293729 | 17172 |
| `(bigAlienWave + !(i >= 0))` | 92.293729 | 17172 |
| `(bigAlienWave + (i <= -1))` | 92.293729 | 17172 |
| `(bigAlienWave \| (i < 0))` | 92.244224 | 17172 |
| `((i < 0) + bigAlienWave)` | 91.848185 | 17172 |
| `(bigAlienWave + (k < 0))` (folds) | 88.380543 | 17140 |
| plain `bigAlienWave ? ... : ...` | 88.380543 | 17140 |

Full 32-way sweep of `{anchor=a/p} x {setScaling=fresh/gun} x
{position=before_rot/mid/end/last} x {shipGroup-then-cargo / cargo-then-shipGroup}`:
best is the baseline combination `afbefore_rotsc` at 92.293729; second best
`afmidsc` (also 92.293729); every `plain`-anchor variant is 88.30–88.38 /
17140–17148.

Pirate-fortress loop 16-way sweep (`{setScaling=fresh/gun} x
{before_rot/mid/end/last} x {shipGroup-then-cargo / cargo-then-shipGroup}`):

| variant | official % | size |
|---|---|---|
| **baseline** (cached `gun`, after `setPosition`, shipGroup then cargo) | **92.293729** | 17172 |
| cached gun, before setRotation | 92.260726 | 17172 |
| cached gun, between the two rotates | 92.260726 | 17172 |
| cached gun, last | 92.244224 | 17172 |
| cached gun, after setPosition, cargo then shipGroup | 92.244224 | 17172 |
| fresh `(*enemies)[i]`, before setRotation | 92.102005 | 17172 |
| fresh, between the two rotates | 92.102005 | 17172 |
| fresh, after setPosition | 90.909091 | 17180 |
| fresh, last | 90.876093 | 17180 |

### 4.2 `alienGuns` / `fortressGuns` spellings

| variant | official % | size |
|---|---|---|
| baseline `alienWave ? (bigAlienWave == 1 ? 8 : 7) : 0` / `pirateFortress ? 5 : 0` | **92.293729** | 17172 |
| `alienWave ? (bigAlienWave ? 8 : 7) : 0` | 92.244224 | 17172 |
| `alienWave * (bigAlienWave == 1 ? 8 : 7)` | 88.210040 | 17164 |
| `alienWave ? 7 + bigAlienWave : 0` | 90.310333 | 17164 |
| `pirateFortress * 5` | 88.072637 | 17156 |

### 4.3 Case-183 convoy if-chain (all 6 source orders)

| source order | official % | size |
|---|---|---|
| `2,1,0` (baseline) | **92.293729** | 17172 |
| `2,0,1` | 92.293729 | 17172 |
| `1,2,0` | 92.161716 | 17172 |
| `1,0,2` | 92.161716 | 17172 |
| `0,1,2` | 92.145215 | 17172 |
| `0,2,1` | 92.145215 | 17172 |

Source order `0,1,2` reproduces the original's *test* chain (`cmp #2; beq;
cmp #1; beq; cbnz`) but lays the bodies out `0,2,1,default` instead of
`0,1,2,default`; the body swap costs more than the test chain gains.

### 4.4 `&&` materialisation (`Level.cpp:1904`)

| variant | official % | size |
|---|---|---|
| baseline `i == fc+f164 && alienWave` | **92.293729** | 17172 |
| `!(i != fc+f164 \| !alienWave)` (De Morgan) | 92.293729 | 17172 |
| `(i == fc+f164) & (alienWave != 0)` | 92.293729 | 17172 |
| `bool atStart = i == fc+f164; if (atStart && alienWave)` | 92.293729 | 17172 |
| `alienWave != 0` | 92.293729 | 17172 |
| `i - fc - f164 == 0 && alienWave` | 84.132354 | 17188 |
| `!((i != fc+f164) \| !alienWave)` with explicit `\|` | 82.981182 | 17168 (`this`->sl) |

### 4.5 `raid` initialisation spellings (frame slot stays sp+84)

| variant | official % | size |
|---|---|---|
| baseline `bool raid;` + if/else | **92.293729** | 17172 |
| single expression `bool raid = !(...) && raidRoll < ...` | 92.293729 | 17172 |
| `bool raid = false;` + negated if | 92.293729 | 17172 |
| `bool raid = cond ? false : ...` | 92.293729 | 17172 |

### 4.6 extraAliens loop bound association

| variant | official % | size |
|---|---|---|
| baseline `start + extraAliens + pirates + fc + f164 + f168 + hostile` | **92.293729** | 17172 |
| Ghidra order `fc + start + extraAliens + pirates + f164 + f168 + hostile` | 92.293729 | 17172 |

## 5. Conclusion / next step

The function is byte-identical to the original up to normalised insn 1368
(offset `0x144e` is the first size divergence, inside the case-183 chain).
The score is dominated by one register-allocator decision: `%4908` (`this`
reload) is split at the alien-turret loop boundary in ours but kept live in
r4 in the original, which then permutes `this`/`gun`/`&return-slot`/`k*24`/`i`
through the turret, pirateFortress and raid blocks. Every source reshape tried
this wave (32-way setScaling/anchor/order sweep, declaration reorders, loop
forms, `enemies`/`gun` localisations, `PlayerTurret*` typing, `first` hoists)
either leaves the emitted code byte-identical or regresses.

Next step: find what makes `RegAllocGreedy` keep the turret-preheader `this`
value live across the loop body start instead of splitting it, and what makes
it create the `raid` spill slot before the `&__stack_chk_guard` slot.

## 6. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6733/score.sh          # official %, size
python3 /dev/shm/w6733/diff.py top N     # clustered verify-normalised diff
python3 /dev/shm/w6733/addrs.py A B      # side-by-side with function offsets
python3 /dev/shm/w6733/check2.py         # locates the turret `mov r0,rX`
cmake --build cmake-build-match --target check -j6
```

Scratch (not tracked): `/dev/shm/w6733/{score,diff,addrs,check2,sweep,vt}.py`,
`Level.base.cpp` (== HEAD), `Level.ll`, `ra.txt` (`-mllvm -print-after-all`),
`mir_greedy.txt` / `mir_vrr.txt` (createMission Greedy RA / VirtRegRewriter
dumps), `rea.c`, `rea.json`, `orig.dis`, `ours.dis`.
