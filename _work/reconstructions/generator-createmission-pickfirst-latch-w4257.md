# Generator::createMission — pick-first `pick == 10` + shared `assignType` latch (w4257)

Status: **landed** (wave 92, 2026-11-03, worktree `w4257`, branch `wave92-w4257`).
`createMission` fuzzy score **73.6 -> 74.3** (`osize 1996  usize 1996`). No gate count moved:

```
before: verify byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
        data 843/843  sodiff extra 0 missing 0  imports 207  lint CLEAN  drift 0
after : verify byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
        data 843/843  sodiff extra 0 missing 0  imports 207  lint CLEAN  drift 0
        .text 0x1337c8  .rodata 0x41b67  size-exact 4524/4524
```

The function stays exactly **1996 B**, so no section address shifts. The base of this worktree
(`e3379c7c`, master `b379d00d` lineage) did **not** yet contain the equivalent `36e73b43` commit
from the main repo; the diff below is the same source shape, re-measured here.

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

This pass supersedes the "systems/used swap is the blocker" conclusion of
`generator-createmission-nested-pickfirst-w4104.md`: the pick-first `pick == 10` IR **is**
reachable together with `systems -> r8`, `agent -> r9`, frame 96 and `rnd -> [sp,#40]`. The
missing ingredient was the **shared `assignType` latch** (`goto`), which reproduces the
original's `972d2: mov r2,r6` block and keeps the greedy allocator from spilling `agent`.

## 0. Before / after

```
before (HEAD e3379c7c): pct=73.6  osize=1996  usize=1996  n_orig=727  n_ours=727
after  (this commit):   pct=74.3  osize=1996  usize=1996  n_orig=727  n_ours=727
```

## 1. The landed source shape

Only the mission-type selection loop changed (plus a trailing newline):

```c++
    for (; !selected; --guard) {
        selected = guard < 1;
        short pick = Globals::rnd->nextInt(15);
        type = pick;
        if (pick == 8) goto assignType;
        if (__builtin_expect(pick == 10, 1)) {  // lint: arm_builtin
            type = 10;
            if (race > 3) continue;
        }
        if (used != nullptr) {
            if (used->data()[pick]) {
                int sum = 0;
                uint32_t i;
                for (i = 0; i < used->size(); ++i) {
                    sum += used->data()[i];
                }
                if (sum == 14) {
                    for (uint32_t i = 0; i < used->size(); ++i) {
                        used->data()[i] = false;
                    }
                    goto assignType;
                }
                type = pick;
                continue;
            }
            used->data()[pick] = true;
        }
        if (pick == 12) {
            selected = true;
            if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 25) {
                type = 12;
            } else {
                goto assignType;
            }
            continue;
        }
        selected = true;
    assignType:
        type = pick;
    }
```

Three things matter and were each measured independently (see the sweep tables in §4):

1. **`short pick`, not `int pick`.** This is the single biggest lever. `int pick` changes the
   greedy allocator's ordering and moves `systems` off `r8`/`used` off `sl`/frame to 104; the
   whole 600-instruction tail then loses its slot/register matches. `short pick` keeps
   `systems -> r8`, `agent -> r9`, `rnd -> [sp,#40]`, frame 96.
2. **Nested pick-first `pick == 10`, not `pick == 10 && race > 3`.** The original assigns
   `type = 10` *before* the race test (`movs r2,#10` sits between `ldr race` and `cmp race,#3`),
   so the source must be the nested form. The `&&` form compiles race-first and scores 73.6;
   the nested form without the branch hint scores 60.0.
3. **`goto assignType` shared latch.** The original has one `mov r2,r6` at `972d2` shared by
   `pick == 8`, the `sum == 14` reset path and the `pick != 12` else path. Spelling those as
   `continue` / fall-through moves the latch and re-orders the loop tail; the `goto` form
   reproduces it and is what makes the `pick == 12` select survive non-folded.

### 1.1 The `__builtin_expect(pick == 10, 1)` hint

The binary proves the weight: the original lays the `pick == 10` block out as the **fall-through**
(`9726a: cmp r6,#10; 9726c: bne.n 97276`) and branches away for `pick != 10`, i.e. it treats
`pick == 10` as the likely path. Without the hint clang 7.0.2 at `-Oz` picks the opposite layout
and the loop loses the pick-first order (60.0 %). The hint is therefore reproducing a measured
branch weight, not papering over one. It needs the same-line `// lint: arm_builtin` waiver
(the file already carries one at the `nextInt(5) == 0` cold-path hint).

## 2. Original loop (`0x97248..0x972da`, 146 B)

```
97248: f04f 0b00   mov.w   fp, #0
9724c: f1b9 0f01   cmp.w   r9, #1
97250: bfb8        it      lt
97252: f04f 0b01   movlt.w fp, #1
97256: 980a        ldr     r0, [sp, #40]        ; rnd
97258: 210f        movs    r1, #15
9725a: 6800        ldr     r0, [r0, #0]
9725c: f7ca eaf4   blx     nextInt
97260: f1a9 0901   sub.w   r9, r9, #1           ; --guard
97264: 4606        mov     r6, r0               ; pick
97266: 2808        cmp     r0, #8
97268: d033        beq.n   972d2                ; pick==8 -> shared latch
9726a: 2e0a        cmp     r6, #10
9726c: d103        bne.n   97276                ; pick!=10 -> used check
9726e: 9809        ldr     r0, [sp, #36]        ; race
97270: 220a        movs    r2, #10              ; type = 10  (BEFORE the test)
97272: 2803        cmp     r0, #3
97274: d82e        bhi.n   972d4                ; race>3 -> condition
97276: f1ba 0f00   cmp.w   sl, #0               ; used != nullptr
9727a: d019        beq.n   972b0
9727c: f8da 0004   ldr.w   r0, [sl, #4]
97280: 5d81        ldrb    r1, [r0, r6]
97282: b199        cbz     r1, 972ac
97284: f8da 1000   ldr.w   r1, [sl]
97288: 2200        movs    r2, #0
9728a: 2300        movs    r3, #0
9728c: e002        b.n     97294
9728e: 5c84        ldrb    r4, [r0, r2]
97290: 3201        adds    r2, #1
97292: 4423        add     r3, r4
97294: 4291        cmp     r1, r2
97296: d1fa        bne.n   9728e
97298: 2b0e        cmp     r3, #14
9729a: 4632        mov     r2, r6               ; type = pick (own copy)
9729c: d11a        bne.n   972d4
9729e: e002        b.n     972a6
972a0: f800 5b01   strb.w  r5, [r0], #1
972a4: 3901        subs    r1, #1
972a6: 2900        cmp     r1, #0
972a8: d1fa        bne.n   972a0
972aa: e012        b.n     972d2                ; -> shared latch
972ac: 2101        movs    r1, #1
972ae: 5581        strb    r1, [r0, r6]
972b0: 2e0c        cmp     r6, #12
972b2: d10c        bne.n   972ce
972b4: 9808        ldr     r0, [sp, #32]        ; status
972b6: 6800        ldr     r0, [r0, #0]
972b8: f7ca ebaa   blx     getSystem
972bc: f7ca ebde   blx     getIndex
972c0: 220c        movs    r2, #12
972c2: 2819        cmp     r0, #25
972c4: f04f 0b01   mov.w   fp, #1
972c8: bf18        it      ne
972ca: 4632        movne   r2, r6              ; non-folded select
972cc: e002        b.n     972d4
972ce: f04f 0b01   mov.w   fp, #1              ; else: selected = true
972d2: 4632        mov     r2, r6              ; <<< SHARED LATCH: type = pick
972d4: ea5f 70cb   movs.w  r0, fp, lsl #31
972d8: d0b6        beq.n   97248               ; while (!selected)
```

The three edges into `972d2` are exactly the three `goto assignType` sites in the landed
source (pick==8, sum==14, pick!=12). The `sum != 14` path keeps its own `mov r2,r6` at `9729a`
because it falls straight to `972d4`; the landed source spells that as `type = pick; continue;`
inside the `used->data()[pick]` branch.

Original register map: `systems->r8`, `agent->r9` (spilled to `[sp,#24]`, r9 reused for
`guard`), `used->sl`, `selected->fp`, `type->r2`, `pick->r6`, `guard->r9`, `race->[sp,#36]`,
`status->[sp,#32]`, `rnd->[sp,#40]`, memset zero `->r5`, `targetStation->r5` (tail),
frame 96.

## 3. Reconstructed loop (landed, `0x96..0x120`-equivalent)

```
mov.w   fp, #0
cmp     r5, #1               ; guard (original r9)
it      lt
movlt.w fp, #1
ldr     r0, [sp, #40]        ; rnd   (matches)
movs    r1, #15
ldr     r0, [r0, #0]
blx     nextInt
sxth    r6, r0               ; pick  (short -> sxth)
cmp.w   r9, r0, lsl #16      ; pick == 8   (short compare, r9 = 0x80000)
beq.n   loc_11a              ; -> shared latch
lsls    r1, r0, #16
cmp.w   r1, #655360          ; pick == 10  (10 << 16)
bne.n   loc_c6               ; -> used check
ldr     r0, [sp, #36]        ; race  (matches)
cmp     r0, #3               ; matches
bls.n   loc_c6
movs    r6, #10              ; type = 10 (folded into pick)
b.n     loc_11a              ; -> shared latch
ldr     r3, [sp, #32]        ; used  (original sl)
cbz     r3, loc_f8
ldr     r0, [r3, #4]
ldrb    r2, [r0, r6]
cbz     r2, loc_f4
ldr     r1, [r3, #0]
movs    r2, #0
movs    r3, #0
b.n     loc_de
ldrb    r4, [r0, r2]
adds    r2, #1
add     r3, r4
cmp     r1, r2
bne.n   loc_d8
cmp     r3, #14
beq.n   loc_ee
b.n     loc_11a
strb.w  sl, [r0], #1         ; memset zero in sl (original r5)
subs    r1, #1
cmp     r1, #0
bne.n   loc_e8
b.n     loc_11a
movs    r2, #1
strb    r2, [r0, r6]
cmp.w   r1, #786432          ; pick == 12  (12 << 16)
bne.n   loc_116
ldr     r0, [sp, #28]        ; status  (original [sp,#32])
ldr     r0, [r0, #0]
blx     getSystem
blx     getIndex
cmp     r0, #25
mov.w   fp, #1
it      eq
moveq   r6, #12              ; select survives (folded into pick)
b.n     loc_11a
mov.w   fp, #1               ; else: selected = true
subs    r5, #1               ; --guard (original at 97260, in-body)
movs.w  r0, fp, lsl #31
beq.n   loc_96               ; while (!selected)
```

Differences that remain in the loop, all `short`-encoding artefacts (the original's `pick` is a
full `int`, so it gets `cmp r0,#8` / `cmp r6,#10` / `cmp r6,#12` and `mov r6,r0`):

* `sxth r6,r0` + `cmp.w r9,r0,lsl#16` instead of `mov r6,r0` + `cmp r0,#8`;
* `lsls r1,r0,#16` + `cmp.w r1,#655360` instead of `cmp r6,#10`;
* `cmp.w r1,#786432` instead of `cmp r6,#12`;
* the select is `cmp r0,#25; it eq; moveq r6,#12` instead of
  `movs r2,#12; cmp r0,#25; it ne; movne r2,r6`.

Everything else — the `pick == 8` first test, the `type = 10` before the race test, the
`used`/`sum`/memset block, the `pick == 12` select, the bottom-of-loop condition and the shared
`type = pick` latch — is in the original order.

### 3.1 Why the `short` spelling is what keeps frame 96

`short pick` makes clang emit `%45 = shl i32 %44, 16` / `%46 = ashr exact i32 %45, 16` and fold
the two equality tests into a `switch i32 %45` with cases `524288` (8<<16) and `655360` (10<<16).
The switch needs a materialised `0x80000` constant, which the allocator parks in **r9** for the
whole loop (`mov.w r9,#524288` in the prologue). That pushes `guard` from `r9` to `r5`, which
pushes the memset zero from `r5` to `sl`, which pushes `used` off `sl` onto the stack. The chain
is exactly:

```
short pick -> 0x80000 constant in r9 -> guard in r5 -> zero in sl -> used in [sp,#32]
int   pick -> r9 free                -> guard in r9 -> zero in r5 -> used in sl
```

This is the w4104 "systems/used swap" in its true form: it is not `systems` that moves, it is the
`0x80000` constant that steals `r9`. With `int pick` the whole chain reverses and the original
register map appears — but the tail then grows to frame 104 (see §5).

## 4. Sweep evidence (official verifier, autojunk=False)

One-file compile (`/dev/shm/w4257/cc.sh`, the exact `compile_commands.json` command) plus the
official `verify.py` score.

| variant | pct | usize | features |
|---|---:|---:|---|
| HEAD (`short pick`, `&& race>3`, ternary select) | 73.6 | 1996 | sys_r8 agent_r9 frame96 rnd_sp40 |
| **landed: `short pick` + `for` + nested p10 + p10 expect + gotos** | **74.3** | **1996** | sys_r8 agent_r9 frame96 rnd_sp40 |
| landed + `__builtin_expect(pick==12,0)` | 73.3 | 1996 | sys_r8 agent_r9 frame96 rnd_sp40 |
| landed + `while` instead of `for` | 65.2 | 2004 | agent_r9 frame96 rnd_sp40 |
| `int pick` + same structure | 69.0 | 1996 | sys_r8 agent_r9 used_sl frame104 pick10_first sel |
| `int pick` + base743 tail (`amount=0`, galaxy before clamp) | 70.9 | 1996 | sys_r8 agent_r9 used_sl frame104 |
| `int pick` + cand1 tail (galaxy first, toSystem before fromSystemPtr) | 71.1 | 1996 | sys_r8 agent_r9 used_sl frame104 |

Loop-shape sweep (1600 = 10 pick types x 5 p10 forms x top-type x p12 form x sum-index x
used-check x clear-loop): **no** shape with `sys_r8 + used_sl + rnd_stack`; the 192 shapes with
`sys_r8 + rnd_stack` at size 1996 are all signed-narrow pick types (`short`, `int16_t`,
`signed char`, `int8_t`), and every `int`/unsigned/32-bit pick type gives frame 104.
Declaration-permutation sweep (720 orders of `{race, level, selected, type, guard, used}` on the
landed loop): **no** permutation with `sys_r8 + used_sl + rnd_stack`.

## 5. New angle: `int pick` + `Globals::galaxy` used directly reaches the exact target allocation

The w4232/§3 conclusion was that `int pick` cannot reach frame 96 because the tail keeps
`galaxy` in `r6` and `difficulty` in `r5`. That is true for the tail written as

```c++
    Galaxy *galaxy = Globals::galaxy;
    int fromSystem = Globals::status->getStation()->getSystem();
    SolarSystem *fromSystemPtr = systems->data()[fromSystem];
    SolarSystem *toSystem = systems->data()[galaxy->getStation(targetStation)->getSystem()];
    float reward = (galaxy->distance(fromSystemPtr, toSystem) / 1200.0f + 1.0f) * ...
```

but **not** if the `galaxy` local is removed and `Globals::galaxy` is used directly:

```c++
    if (difficulty >= 10) {
        difficulty = 10;
    }
    int fromSystem = Globals::status->getStation()->getSystem();
    SolarSystem *fromSystemPtr = systems->data()[fromSystem];
    SolarSystem *toSystem = systems->data()[Globals::galaxy->getStation(targetStation)->getSystem()];
    float reward = (Globals::galaxy->distance(fromSystemPtr, toSystem) / 1200.0f + 1.0f) * ...
```

With `int pick`, the pick-first nested `pick == 10`, the shared latch and this tail, the official
verifier gives:

```
osize 1996  usize 1996  pct 68.7
frame96=1  sys_r8=1  agent_r9=1  used_sl=1  rnd_stack=1  type_r4_exit=1  pick10_first=1  sel=1
```

i.e. **every** target feature the prompt asked for — `systems->r8`, `agent->r9`, `used->sl`,
`rnd->[sp,#40]`, frame 96, at exactly 1996 B — plus the full-`int` loop encoding
(`mov r6,r0; cmp r0,#8; cmp r6,#10; cmp r6,#12`) and the non-folded
`movs r2,#12; cmp r0,#25; it ne; movne r2,r6` select.

The exact source (loop + tail) is:

```c++
    while (!selected) {
        selected = guard < 1;
        int pick = Globals::rnd->nextInt(15);
        --guard;
        type = pick;
        if (pick == 8) goto assignType;
        if (__builtin_expect(pick == 10, 1)) {
            type = 10;
            if (race > 3) continue;
        }
        if (used != nullptr) {
            if (used->data()[pick]) {
                int sum = 0;
                for (uint32_t i = 0; i < used->size(); ++i) {
                    sum += used->data()[i];
                }
                if (sum == 14) {
                    for (uint32_t i = 0; i < used->size(); ++i) {
                        used->data()[i] = false;
                    }
                    goto assignType;
                }
                type = pick;
                continue;
            }
            used->data()[pick] = true;
        }
        if (__builtin_expect(pick == 12, 0)) {
            selected = true;
            if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 25) {
                type = 12;
            } else {
                goto assignType;
            }
            continue;
        }
        selected = true;
    assignType:
        type = pick;
    }
    ...
    if (difficulty >= 10) {
        difficulty = 10;
    }
    int fromSystem = Globals::status->getStation()->getSystem();
    SolarSystem *fromSystemPtr = systems->data()[fromSystem];
    SolarSystem *toSystem = systems->data()[Globals::galaxy->getStation(targetStation)->getSystem()];
    float reward = (Globals::galaxy->distance(fromSystemPtr, toSystem) / 1200.0f + 1.0f) * ...
```

**But it scores 68.7, below the landed 74.3**, because the tail's stack-slot layout diverges:
`targetStation` lands in `[sp,#32]` (original `[sp,#28]`), `status` in `[sp,#28]` (original
`[sp,#32]`), the `amount` store is still sunk to the join, and the `type == 8` switch table /
`getRoutes()` test differ. The 74.3 short-pick build keeps 191 differing instructions; this
int-pick build has 246. The score is dominated by the ~600-instruction tail, not the 60-instruction
loop, so the loop/register win does not pay for the tail loss.

Tail-order sweep on the int-pick loop (24 permutations of `{clamp, galaxy, fromSystem,
fromSystemPtr, toSystem}` x `amount` initialised/uninitialised x `galaxy` local/global, 96
variants): 8 shapes reach size 1996 with `frame96 + sys_r8 + agent_r9 + used_sl + rnd_stack`,
all with `Globals::galaxy` used directly. Their official scores are **68.3–68.7**:

| tail order | amount | galaxy | size | pct |
|---|---|---:|---:|---:|
| galaxy, fromSystem, toSystem, fromSystemPtr | `=0` | global | 1996 | 68.3 |
| fromSystem, galaxy, toSystem, fromSystemPtr | `=0` | global | 1996 | 68.3 |
| fromSystem, toSystem, galaxy, fromSystemPtr | `=0` | global | 1996 | 68.3 |
| fromSystem, toSystem, fromSystemPtr, galaxy | `=0` | global | 1996 | 68.3 |
| clamp, fromSystem, galaxy, fromSystemPtr, toSystem (nogl2) | `=0` | global | 1996 | 68.7 |

Hoisting `items` out of the `type == 8` branch (the w4004 §9.3 trick for keeping `difficulty` in
`r6`) gives 2000 B / 63.2–63.6 on this family, i.e. it costs more than it gains.

## 6. What remains

* `used` is still in `[sp,#32]` (loaded into `r3`) instead of the original's `sl`; `sl` is the
  memset zero. Slot order is therefore `16=&guard, 20=targetStation, 24=agent, 28=status,
  32=used, 36=race, 40=rnd` vs the original's `16=galaxy/bonus, 20=&guard, 24=agent/difficulty,
  28=targetStation/amount, 32=status, 36=race/type, 40=rnd`.
* The `type == 8` block still sinks the `amount` store to the join instead of storing it inside
  the block (`str r0,[sp,#28]` before `mov r6,r1`), and `difficulty` lands in `r5` not `r6`.
* The `type == 13 / getRoutes()==nullptr` block is placed inline before `agent->getName()`
  instead of out-of-line at `97586`, and the `getRoutes()==nullptr` test is `cbz` not
  `cmp/beq.w`.
* The `pick == 12` select is `it eq / moveq r6,#12` rather than the original's
  `movs r2,#12; it ne; movne r2,r6` (a direct consequence of `short pick`).
* The `0.5000001f` costs anchor is still present. The original really is
  `vmov.f32 s2,#0.5` (`0x3f000000`), so the anchor is only holding the 4 bytes that the folded
  `pick == 12` select lost. Removing it requires the full-`int` select, i.e. the frame-104
  family — and the int-pick family that reaches frame 96 (§5) scores lower overall.

The remaining task is therefore a **tail** problem, not a loop problem: a `short pick`-class loop
(the only family that keeps the 74.3 tail slot layout) with `used` in `sl` instead of the memset
zero. That requires the allocator to put the `0x80000` constant somewhere other than `r9`
without switching `pick` to `int`, or a tail source shape that makes the frame-104 int-pick family
score above 74.3.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4257
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4499
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --fail-on-wrong-type --top 0
# createMission row: osize 1996  usize 1996  fuzzy 74.3
```

Scratch harness (not committed, `/dev/shm/w4257`): `cc.sh` (one-file compile from
`compile_commands.json`), `score.py` (official verifier for one symbol), `feat.py` /
`feat_obj.py` (feature scan), `gen743.py` (landed-loop generator), `tailvar.py` / `swtail.py`
(tail-order sweep), `swdecl743.py` (720 declaration permutations), `sw743.py` (1600 loop shapes).
