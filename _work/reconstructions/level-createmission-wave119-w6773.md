# w6773: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# angle-H sweep: first code-size divergence located (hoisted VFP literals in the
# case-183 convoy loop), turret selector/body, `&&`, case4Tail, loop-bound shapes;
# no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6773`, branch `wave119-w6773`,
based on current master (`775d1901c`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine HEAD, wave-118 `volatile unsigned n` latch)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6065 ours / 6055 orig insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .text 0x1337c8 both.
     .ARM.extab 0x2628c (12 B short of 0x26298) and .rodata 0x41c27
     (0xc0 bigger than 0x41b67) in this build; every other section is
     size- and base-exact.
```

`check` was run before and after this pass; the worktree is left with only this
note (`src/game/world/Level.cpp` is unchanged).

## 1. Result

No source variant beat the baseline.  Best measured score remains
**92.293729 % / 17172 B / 6065 insns**.  This note is the only commit.

## 2. New result: the first *code-size* divergence is the case-183 convoy
##    `offset[2]` constant, hoisted into VFP registers

An instruction-index -> address map (`SequenceMatcher` equal blocks, objdump
addresses of both streams) shows the two functions are byte-for-byte the same
size until normalised insn 1830:

```text
idx 1830/1831  orig 0xb1364  ours 0xb1368  delta +4
idx 1831/1833  orig 0xb1368  ours 0xb136e  delta +6
idx 1833/1837  orig 0xb136e  ours 0xb1378  delta +10
idx 1865/1869  orig 0xb13c0  ours 0xb13d0  delta +16   (start of the k-chain)
```

The divergence is the preheader of the `case 183` convoy loop
(`for (int k = 0; k < 4; k++)`, `Level.cpp:2182`):

```text
orig  b13a8  movs r6,#10
      b13aa  mov.w sl,#0
      b13ac  b.n b145a
ours  b13a8  vldr s16,[pc,#552]      ; case-183 offset[2] constant
      b13ac  movs r6,#10
      b13ae  vldr s18,[pc,#-632]
      b13b2  mov.w sl,#0
      b13b6  vldr s20,[pc,#544]
      b13ba  vldr s22,[pc,#-624]
      b13be  b.n b149a
```

Ours hoists the four `offset[2]` constants out of the loop into `s16`/`s18`/
`s20`/`s22` and each switch body stores them with `vmov.f32 s0,sN` +
a shared `vstr s0,[sp,#380]` at `b1464`.  The original instead loads each
`offset[2]` constant into a GPR inside its body (`ldr r1,[pc]`) and shares a
plain `str r1,[sp,#380]` at `b144e`:

```text
orig  b1438  ldr r1,[pc,#408]   ; 25000
      b143a  str.w sl,[sp,#376] ; offset[1] = 0
      b143e  str r1,[sp,#372]   ; offset[0] = 25000
      b1440  ldr r1,[pc,#404]   ; -36000
      b1442  b.n b144e
      ...
      b144e  str r1,[sp,#380]   ; offset[2]   (shared, GPR)
```

So the base source already emits the VFP-sunk shared tail; the w6756 note's
"default `1,0,2` grows to 17188" describes the same VFP sinking, not a new one.
The residual is `vmov`+`vstr` (VFP) vs `ldr`+`str` (GPR) plus the four hoisted
`vldr`s.  This is the *first* size divergence; every later `ldr [pc,#N]`
offset in the function inherits it (the delta reaches +34 by idx 2019 and only
returns to 0 around idx 5190).

The address-delta map is non-monotonic: it goes to +34 at idx 2019, back to
+14 at idx 1895 (the k-chain bodies are laid out differently), returns to 0 at
idx 5190, goes to -8 around idx 5460 (the pirate-escort loop), and positive
again at the end.

## 3. Variants measured this wave (official verifier)

All are `official % / size / insns`; baseline `92.293729 / 17172 / 6065`.
Every variant keeps 27 sections and file size 2,187,328 B.

### 3.1 alien-turret loop body / selector

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline (`(bigAlienWave + (i < 0))` select, fresh-load `setScaling`)** | **92.293729** | 17172 | 6065 |
| faithful body (cached `gun`, `setScaling` after `setPosition`, cargo then shipGroup) | 92.160423 | 17172 | 6063 |
| faithful body, shipGroup then cargo | 92.176927 | 17172 | 6063 |
| faithful body, `setScaling` at the very end | 90.972108 | 17164 | 6063 |
| statement `if (bigAlienWave) t = ...; else t = ...;` + baseline body | 88.380543 | 17140 | 6054 |
| statement-if + faithful body | 88.351358 | 17140 | 6058 |
| statement-if + `setScaling` first | 88.318336 | 17140 | 6058 |
| no `gun` local (all `(*this->enemies)[i]`) | 88.558938 | 17204 | 6068 |
| anchor `(bigAlienWave \| (i < 0))` | 92.244224 | 17172 | 6065 |
| anchor `(bigAlienWave ^ (i < 0))` / `-` / `* 1` | 92.293729 | 17172 | 6065 |

### 3.2 turret preheader (`this` live-range probes)

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `Level *self = this;` before the `if`, used in the body | 92.293729 | 17172 | 6065 |
| `if (alienGuns > 0)` | 92.293729 | 17172 | 6065 |
| `Array<KIPlayer*> *turretEnemies = this->enemies;` before the `if`, used in the loop | 88.720191 | 17164 | 6064 |
| same, used in both turret and fortress loops | 88.593595 | 17156 | 6061 |
| `if (alienGuns != 0 && this->enemies != nullptr)` | 75.925163 | 17368 | 6078 |
| `Vector base;` hoisted above the `if` (turret only) | 83.982184 | 17196 | 6069 |
| `Vector base;` hoisted above the `if` (both loops) | 83.911932 | 17204 | 6072 |

### 3.3 turret loop bound / index shapes

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `k++, i++` increment order | 92.293729 | 17172 | 6065 |
| `k` declared in the `for` init | 92.293729 | 17172 | 6065 |
| bound `fc + f164 + f168 + alienGuns` | 92.293729 | 17172 | 6065 |
| bound `i - (fc+f164+f168) < alienGuns` | 92.194719 | 17172 | 6065 |
| `int start = fc+f164+f168;` cached | 92.293729 | 17172 | 6065 |
| `int end = ...;` cached | 86.989522 | 17180 | 6066 |
| both cached | 86.989522 | 17180 | 6066 |

### 3.4 case-183 convoy `offset[]` chain

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline (source `2,1,0`, default `1,2,0`)** | **92.293729** | 17172 | 6065 |
| source `0,1,2`, default `1,2,0` | 92.145215 | 17172 | 6065 |
| source `0,1,2`, default `1,0,2` | 89.801980 | 17188 | 6065 |
| source `2,1,0`, default `1,0,2` | 89.653465 | 17188 | 6065 |
| cases `1,0,2`, default `1,2,0` | 91.642604 | 17172 | 6066 |
| cases `1,0,2`, default `1,0,2` | 88.606760 | 17224 | 6075 |
| `Vector offset;` + `.x/.y/.z` | 89.917492 | 17176 | 6065 |
| `Vector offset` + default `1,0,2` | 89.537954 | 17192 | 6065 |
| `float offset[3]` declared before the loop | 92.277228 | 17172 | 6065 |
| `float offset[3] = {0,0,0}` | 89.158416 | 17176 | 6065 |
| `int offset[3]` + float bit patterns, default `1,2,0` | 91.930693 | 17172 | 6065 |
| `int offset[3]` + float bit patterns, default `1,0,2` | 91.963696 | 17172 | 6065 |
| `float offset[3]`, `*(int*)&offset[2] = bits`, default `1,2,0` | 92.062706 | 17172 | 6065 |
| `float offset[3]`, `*(int*)&offset[2] = bits`, default `1,0,2` | 92.227723 | 17172 | 6065 |
| same, source order `0,1,2`, default `1,2,0` | 92.178218 | 17172 | 6065 |
| same, source order `0,1,2`, default `1,0,2` | 89.716078 | 17164 | 6061 |

Note: the *base* source order is `2,1,0` for the tests and the compiler lowers
it to ascending tests `0,1,2` with bodies `2(fallthrough),0,1,default`.  The
original has descending tests `2,1,0` with bodies `0(fallthrough),1,2,default`
and a GPR shared `offset[2]` store.  Source order `0,1,2` reproduces the
original's test order and body layout, but still scores 92.145215 because the
literal-pool order and the `offset[2]` GPR/VFP choice differ.

### 3.5 `&&` at line 1904 (`i == fc+f164 && alienWave`)

All of these are byte-identical to the baseline (92.293729 / 17172 / 6065):
`bool atStart = ...; if (atStart && alienWave)`, `(i==X) && (alienWave!=0)`,
`alienWave && i==X`, `i-(fc+f164)==0 && alienWave`, `!(i != X) && alienWave`,
`(bool)(i==X) & (bool)alienWave`.
`!(i != X | alienWave == 0)` gives 92.062706 (17172 / 6065) - the only form
that reaches the original's `orrs` shape, but it loses elsewhere.

### 3.6 case4Tail hunter loop (`volatile unsigned n` latch)

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n`** | **92.293729** | 17172 | 6065 |
| `unsigned n` / `int n` / `n` declared in the `for` | 92.145215 | 17172 | 6065 |
| `for (i = 0; i < (unsigned) n; i++)` | 92.293729 | 17172 | 6065 |
| `Array<KIPlayer*> *enemies = this->enemies;` local | 91.111661 | 17164 | 6062 |
| direct `this->enemies->size()` | 91.308296 | 17164 | 6060 |
| `volatile Array<KIPlayer*> *enemies` local | 91.221122 | 17168 | 6065 |
| `volatile unsigned n` + direct `size()` bound | 90.867090 | 17176 | 6066 |
| local pointer + `this->enemies->size()` bound | 90.982592 | 17172 | 6066 |

### 3.7 `*bigShipsCount > 0` (line 1888)

| variant | official % | size |
|---|---:|---:|
| **baseline** | **92.293729** | 17172 |
| `*bigShipsCount >= 1` / `0 < *bigShipsCount` / `(int)` / `this->field_168 > 0` / `!localRace` | 92.293729 | 17172 |
| `*bigShipsCount != 0` / `(unsigned)*bigShipsCount > 0u` | 89.666557 | 17164 |
| `localRace == 0 && *bigShipsCount > 0` (operand swap) | 74.190885 | 17164 |

### 3.8 combinations

| variant | official % | size | insns |
|---|---:|---:|---:|
| faithful turret body + `!(i != X \| alienWave == 0)` | 90.790559 | 17172 | 6063 |
| `orrs` form + `turretEnemies` local | 92.071611 | 17172 | 6066 |
| statement-if selector + `orrs` form | 88.387843 | 17140 | 6053 |
| statement-if selector + faithful body + `orrs` | 88.354807 | 17132 | 6053 |

## 4. Residual map (unchanged)

* `this` r4 (orig) vs r5 (ours) from the alien-turret preheader onward;
  `gun` r6 vs r4; `&return-slot` r5 vs r8; `k*24` r9 vs fp; `i` fp vs r9;
  `t` sl vs r6 (dominant, ~2.7 pp).  The w6733 MIR finding is confirmed:
  `%4908` (`this`) is assigned r4 in the preheader, the allocator splits it at
  the loop boundary and reloads into r5, and `gun` takes r4.
* `raid` (orig sp+84 / ours sp+88) vs `&__stack_chk_guard` (orig sp+88 /
  ours sp+84); worth +0.31 pp (w6764 weight audit: 1.197954 vs 1.193761).
* Secondary slot cycle `sp+40/44/48` (`pirateEscorts`, `extraAliens`,
  `stationPirates`).
* case-183 convoy: VFP-hoisted `offset[2]` constants (first size divergence,
  section 2 above).
* `&&` at 1904 and the `&& pirateFortress` else-if: original materialises
  `(i != X) | !alienWave` (`subs/mov/it/movne/eor/orrs/bne`); ours
  short-circuits.
* `*bigShipsCount > 0`: orig `cmp #0; ble`, ours `cmp #1; blt`.
* `.ARM.extab` 12 B short (`0x2628c` vs `0x26298`) and `.rodata` 0xc0 bigger
  (`0x41c27` vs `0x41b67`) in this build; 27 sections and 2,187,328 B file
  size unchanged by every variant above.

## 5. Next step

The one lever not yet tried is forcing the case-183 `offset[2]` constant to
stay in a GPR (original `ldr r1,[pc]` + shared `str r1,[sp,#380]`) instead of
the VFP `vmov`/`vstr` sinking that clang chooses.  That is the first size
divergence, so fixing it would realign every downstream `ldr [pc,#N]` offset
and the k-chain body layout.  `float offset[3]` with source order `1,0,2`
shares the tail but sinks to VFP (17188 B); `int offset[3]` with raw bit
patterns avoids VFP but changes the pool to integers (17160 B, 89.28 %).
Storing only the third component through an integer lvalue
(`*(int *) &offset[2] = 0xc70ca000;`) does give the original's GPR
`ldr`+`str` shared tail and the exact 17172 B size (92.227723 %), but clang
then rewrites the integer constants as a shared base + `add.w r1,r1,#imm`
(0x224000 / 0x800000), which costs as much as it saves.  A *pure float*
spelling that keeps the GPR store and avoids the base+offset rewrite is the
next thing to look for.

## 6. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6773/score.py            # official % / size / insns
python3 /dev/shm/w6773/addrdiff.py         # instruction-index -> address delta map
cmake --build cmake-build-match --target check -j6
```

Scratch (not tracked): `/dev/shm/w6773/{score,dump,diff,classify,inscount,
sweep,addrdiff}.py`, `Level.base.cpp` (== HEAD), `all.txt`
(`-mllvm -print-after-all`), `cm_ssc.txt`, and the variant JSONs
`turret1.json`, `pre.json`, `case4tail.json`, `bs.json`, `and.json`, `g.json`,
`k2.json`, `k3.json`, `k4.json`, `k5.json`, `k6.json`, `k7.json`, `k8.json`,
`k9.json`, `k10.json`, `k11.json`, `k12.json`.
