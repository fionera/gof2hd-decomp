# `Gun::calcCharacterCollision` pass 9: the F P H L K order and the shipped allocation are
# mutually exclusive in the PIC build (w4261)

Worker: w4261. Branch: `wave92-w4261`, base master `b379d00d`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w4261/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

No code change landed. The tree already carries the w4229 pass-7 body
(`62313153`), which is still the best measured shape.

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `f636a90e`) | **4499 / 4499** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4499 / 4499** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`,
drift 0, `RATCHET PASS`. The target is not byte-exact (`byte: false`,
`linked: false`), so the hard counts are unchanged.

This pass proves the new angle asked for — "perturb the early IR (bb.0/bb.8)
enough to reproduce the non-PIC eviction cascade" — is **not** reachable from
any of ~10,000 measured source variants. The non-PIC eviction is real and
reproducible, but the PIC build's extra stack-guard vreg changes the
live-range splitting of one value (`sp+stack.10`) in a way no source-level
perturbation of the preamble, the loop-local declarations, the radius block,
the damage block, or the statement order reproduces.

## 1. Ground truth: the shipped post-damage block (unchanged)

```
153310  mov   r0, r8
153312  blx   Player::damage
153316  movs  r1, #1
153318  strb.w r1, [r8, #84]        ; F  field_54[0] = 1
15331c  ldrb.w r0, [r9, #240]       ;    this->playerGun
153320  cbz   r0, 15332a
153322  ldr.w r0, [r9, #56]         ;    this->level
153326  strb.w r1, [r0, #48]        ; P  level->field_30 = 1
15332a  add   r3, sp, #176
15332c  ldmia r3, {r0, r2, r3}      ;    -velocity
15332e  eor.w r1, r0, #0x80000000
153332  mov   r0, r8
153334  eor.w r2, r2, #0x80000000
153338  eor.w r3, r3, #0x80000000
15333c  blx   Player::setHitVector  ; H
153340  ldr.w r0, [r9, #60]         ;    this->lifetimes
153344  ldr   r1, [pc, #916]        ;    -1000000
153346  str.w r1, [r0, fp, lsl #2]  ; L
15334a  mov   r0, r8
15334c  blx   Player::getKIPlayer
153350  str.w r0, [r9]              ; K  this->lastHitKIPlayer
153354  ldr   r0, [sp, #20]         ;    explosive
```

The `cbz` at `153320` is a real basic-block boundary between `F` and `H`, so
the shipped **source** order has `playerGun` immediately after `field_54[0]`
(i.e. `F P H L K`; the order of `H/L/K` inside the fallthrough block is free).
`L` before `K` in the emitted code is a within-block scheduler reorder.

Shipped register/stack map (frame 208):

```
this=r9  enemy=r8  i=fp (+spill [sp,#40])  i*3=r4  td=r6 (spilled [sp,#24])
&position=sl   explosive=[sp,#20]   e=[sp,#28]   scatter=[sp,#32]   mine=[sp,#36]
explodeOnContact=[sp,#16]   canary ptr=[sp,#12]   canary=[sp,#204]
```

The shipped `i*3` is `add.w r4, fp, fp, lsl #1` (`15306c`): `r4` is a
loop-local temporary, not `this`.

## 2. The current tree (F H K L P) and its 52 residual hunks

The tree body is the w4229 `local` shape: `int dmg = this->damage;`, then the
five statements in the **F H K L P** order. It reproduces the shipped register
allocation exactly (`this=r9`, `enemy=r8`, `i=fp`, `i*3=r4`, `td` spilled) and
is 1984 B. Its normalized diff against the original is 52 hunks; the
non-branch-offset content is:

| # | shipped | ours |
|---|---|---|
| 1 | `str r2,[sp,#20]` (explosive) | `str r2,[sp,#24]` |
| 2 | `str r6,[sp,#24]` (td) | `str r6,[sp,#20]` |
| 3 | `ldr.w fp,[sp,#40]; ldr r6,[sp,#24]` after `isAsteroid` (2 sites) | absent (i/td stay spilled) |
| 4 | `ldr.w r1,[r9,#96]` (dmg reload) in the plain path | `mov r1,sl` (dmg kept in sl) |
| 5 | `add.w sl,sp,#192` hoisted into both owner branches | `add.w sl,sp,#192` after the call |
| 6 | `F P H L K` emitted | `F H K L P` emitted |
| 7 | `movs r1,#1; strb.w r1,[r8,#84]` | `mov.w fp,#1; strb.w fp,[r8,#84]` |
| 8 | `movs r1,#1` before `strb.w r1,[r0,fp]` | `strb.w r5,[r0,fp]` (const 1 already in r5) |

Everything else is branch-offset and literal-pool fallout. The first ~180
instructions match instruction-for-instruction apart from the two stack-slot
offsets (items 1–2).

## 3. The faithful `F P H L K` order collapses the allocation

Compiling the *same* body with the five statements in the shipped source order
(`F P H L K`) reproduces the shipped statement order and the shipped
`explosive`/`td` slots, but the callee-saved allocation collapses:

| property | shipped | HEAD (`F H K L P`) | `F P H L K` |
|---|---|---|---|
| `this` | **r9** | **r9** | r4 |
| `enemy` | **r8** | **r8** | r6 |
| inner `i` | **fp (r11)** | **fp (spilled [sp,#40])** | r8 |
| `td` | **r6, spilled [sp,#24]** | **r6, spilled [sp,#20]** | r9 (not spilled) |
| `&position` | sl (r10) | sl (r10) | sl (r10) |
| `explosive` slot | **[sp,#20]** | [sp,#24] | **[sp,#20]** |
| size | **1984 B** | **1984 B** | **1912 B** |
| official pct | — | **90.0 %** | **64.5 %** |

`this` moving from `r9` to `r4` is worth 72 bytes: every `ldr/str [r9,#imm]`
becomes a 16-bit `ldr/str [r4,#imm]` (`ldr.w`/`str.w` → `ldr`/`str`). The
function therefore cannot be 1984 B with `this=r4`.

## 4. The flip is a PIC/non-PIC allocator lottery, not a source shape

`llc -O2` on the *same* stripped IR (`/dev/shm/w4261/fphlk_strip.ll`) with the
default static relocation model reproduces the shipped allocation exactly:

```
non-PIC llc -O2:   sub sp,#200;  mov r9,r0;  ...   (this=r9, frame 200)
PIC     llc -O2:   sub sp,#208;  mov r4,r0;  ...   (this=r4, frame 208)
```

The pre-RA machine IR differs only in the stack-guard load at `bb.0`:

```
 PIC (clang -Oz)                              non-PIC (llc -O2)
 %V:tgpr = tLDRLIT_ga_pcrel @__stack_chk_guard
 %V:gpr  = t2LDRi12 %V, 0                     %V:gpr = t2LDRpci %const.0
 %V:gpr  = t2LDRi12 %V, 0                     %V:gpr = t2LDRi12 %V, 0
 t2STRi12 %V:gpr, %stack.0, 0                 t2STRi12 %V:gpr, %stack.0, 0
```

i.e. PIC has one extra instruction and one extra vreg (`tgpr`) before every
source-level value, shifting the whole function's vreg numbering by one.

The greedy allocator trace (`llc -O2 -debug-only=regalloc`) shows the exact
cascade. In both models `this` (`%57`) is first assigned to `$r4`. Then:

```
non-PIC (flips to the shipped allocation):
  selectOrSplit GPR:%454 [15976r,16080r:0) weight:2.818017e-01
  should evict: %57 (this) ... evicting $r4 interference: Cascade 16
  unassigning %57 from $r4
  assigning %454 to $r4
  ... assigning %57 to $r9 ... assigning %11 (td) to $r8 ... enemy to $r8
  ... td finally spilled

PIC (does not flip):
  %454 does not exist at all (grep "2.818017e-01" -> no match)
  %57 keeps $r4; %11 (td) keeps $r9; enemy gets $r6
```

`%454` is a copy of the address of `stack.10` — the shared `Vector` return
temp of `AEMath::VectorNormalize(...)` (line 669) and `enemy->getPosition()`
(line 727). In the non-PIC build the live-range splitter creates three copies
(`%452` for the radius block, `%453` for the geometries block, `%454` for the
scatter block); the scatter copy has weight `2.8e-1` and evicts `this`. In the
PIC build the same value is one long interval that is assigned to `fp`:

```
PIC  fphlk:  rewr %bb.8 1568r %465 = t2ADDri %stack.10 ; one interval, 16 valnos
             queuing new interval: %465 [1568r,1776B)... weight 3.626880e-02
             assigning %465 to $r11  (fp)
non-PIC:     rewr %bb.8 1552r %451 = t2ADDri %stack.10
             rewr %bb.92 %454 = COPY %451   weight 2.818017e-01
```

So the whole 90 % vs 64.5 % difference is: *does the `stack.10` address get
split into a short scatter-loop copy (evicting `this`) or does it stay one
long interval in `fp` (pinning `i` to `r8`)?* The PIC stack-guard vreg decides
it.

## 5. The early-IR perturbation search (the new angle)

The task's hypothesis was that a faithful source change perturbing `bb.0`/`bb.8`
could reproduce the non-PIC eviction cascade in the PIC build without adding a
stack slot. All of the following were measured with the object-level oracle
(same normalization as `tools/verify/verify.py`; current HEAD = **89.8** oracle
/ **90.0** official). None flips the `F P H L K` body to `this=r9`:

### 5.1 Preamble order (1,440 variants)

All permutations of `{weaponType, bomb, mine, scatter, explosive=, if(bomb||mine),
lastHitKIPlayer=0 + td}` (with `weaponType` first, plus the
`explosive`-declared-in-place variants). Best = the original order, 64.5.
Every other order is ≤ 64.3. Moving the `enemies == nullptr` check after the
`weaponType` load is codegen-identical; after `bomb` it drops to 52.1.

### 5.2 Loop-local declaration order (24 variants × 2 bodies)

All permutations of `position, velocity, enemyPosition, radius`. On the current
`F H K L P` body the shipped `P V E R` order is best (89.8); `P V R E` = 89.2,
`P R V E` = 88.6, everything else ≤ 86.5. On `F P H L K` every permutation is
64.5 or worse.

### 5.3 Damage-block shape × statement order (8,640 variants)

`{local dmg, const dmg, unsigned dmg, after-emp dmg, dmg-at-inner-top, 4call}`
× 24 vector orders × 120 statement orders. The global best is the current
`local / P V E R / F H K L P` at 89.8. The best `F P H L K` variant is
`4call` at 85.1 / 1992 B (wrong size) and `local` at 64.5 / 1912 B.

### 5.4 Targeted perturbations (all no-flip on `F P H L K`)

`playerGun & 1`, `playerGun != 0`, `(bool)playerGun`, `field_30 = true`,
`field_30 = (uint8_t)1`, a `Level*` local in the P block, a `playerGun` local;
`td` as `Vector&`, `td` declared in the outer/inner loop, `this->targetDir`
used directly at each site; `*td = enemy->getPosition() - position`,
`Vector ep = ...; ep -= position; *td = ep;`, a named `Vector` for the
`getPosition` temp; `Vector dir = enemyPosition - position;` before
`VectorNormalize`; reordering the scatter-radius branches; `no_ternary`
radius; `ki`/`outer` expression rewrites; `impact_simple` (drop the redundant
`this->impact != nullptr`); `wt`/`bomb`/`mine` copies; `Vector` declarations
hoisted to the function top (this flips to `this=r9` but adds 6 zero-init
stores and drops to 63.5); trivial `Vector()` ctor + hoisted declarations
(61.9).

The only source-level way found to reach `this=r9` with `F P H L K` is to add
instructions at `bb.8` (the hoisted `Vector` zero-inits, or the `Vector`
declarations at function scope). Those change the emitted code, so they are not
an improvement.

## 6. Why the residuals are coupled

* **`explosive`/`td` slot swap.** Stack-slot colouring weights on the current
  body (`llc -debug-only=stack-slot-coloring`): `explosive` 13.770 (rank 6),
  `td` 6.868 (rank 8). The shipped needs `td` above `explosive`, which is the
  `F P H L K` colouring (that body already puts `explosive` at `[sp,#20]` and
  `td` at `[sp,#24]`). The weights are post-RA spill-slot live ranges, so they
  follow the allocation, not the source.
* **`dmg` in `sl` vs `&position` in `sl`.** `sl` can hold only one. The
  pre-RA scheduler hoists `&position` into the owner branches in the shipped
  build and `dmg` in ours. This is the same scheduler/RA interaction.
* **`movs r1,#1` vs `mov.w fp,#1`.** In `F H K L P` the constant `1` is live
  from `F` to `P` across `H/K/L`, so it gets a callee-saved register (`fp`,
  clobbering the spilled `i`). In `F P H L K` `F` and `P` are adjacent, so the
  constant stays in `r1`. Directly coupled to the statement order.

## 7. Conclusion / remaining work

The remaining 10 % is one coupled degree of freedom:

* The shipped **source** order is `F P H L K` (proved by the `cbz` block
  boundary and corroborated by the iOS 1.1.4 build, which has the predicated
  `itt ne` P block between `F` and `H`).
* In the PIC build that order makes clang 7's greedy allocator keep the
  `stack.10` address as one long interval in `fp`, which pins `i` to `r8` and
  `this` to `r4` (1912 B, 64.5 %).
* The tree's `F H K L P` order is the only one that reproduces the shipped
  allocation (`this=r9`, 1984 B) — at the cost of the emitted order.
* The non-PIC build of the *same* `F P H L K` IR reproduces the shipped
  allocation exactly; the sole difference is the PIC stack-guard load's extra
  vreg at `bb.0`.

Remaining levers, in priority order:

1. A source construct that changes the *number of IR values before the
   `stack.10` address* without changing the emitted code. The non-PIC replay
   bounds this: the cascade flips on a single vreg. Candidates: a construct
   that makes the frontend emit one extra (dead) `llvm.dbg.value`/copy at
   `bb.0`, or a different `Vector` temp sharing pattern that changes the
   stack-slot numbering. Every obvious source change tried here also changes
   the emitted code.
2. If (1) lands, the `explosive`/`td` colouring and the `dmg`-in-`sl` reload
   should fall out of the same allocation; re-check the SSC weight ranking
   with `-mllvm -print-after-all` before assuming they are independent.
3. Keep 1984 B: any candidate that changes the size shifts every later
   `.text` symbol and fails the hard gate.

## 8. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# per-function official verifier
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False

# object-level oracle (fast screening; same normalization as verify.py)
TMPDIR=/dev/shm/w4261 python3 /dev/shm/w4261/oracle.py \
  --src src/game/weapons/Gun.cpp --fn _ZN3Gun22calcCharacterCollisionEv

# non-PIC replay (shipped allocation)
/home/debian/llvm7build/build/bin/llc -O2 -filetype=asm fphlk_strip.ll -o llc_static.s
/home/debian/llvm7build/build/bin/llc -O2 -relocation-model=pic -filetype=asm \
  fphlk_strip.ll -o llc_pic.s
/home/debian/llvm7build/build/bin/llc -O2 -debug-only=regalloc \
  -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv fphlk_strip.ll -o /dev/null
```

## 9. Files changed

None under `src/` (note only). Gate: byte 4499 / linked 4499, size-exact
4524/4524, data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, lint CLEAN, drift 0.
