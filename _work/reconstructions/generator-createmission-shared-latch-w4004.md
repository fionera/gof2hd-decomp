# Generator::createMission — shared-latch select survives CVP (w4004)

Status: research note (2026-11-03, wave 90). No source change landed: every variant below
either changes `createMission`'s size (which shifts `.text`/`.got` and collapses the gate's
`byte_exact` count) or scores below the current 73.590096. Worktree clean; `check` passes at
`byte 4498 / linked 4498`, `.text 0x1337c8`, lint CLEAN.

**Wave-90 second pass (§8):** found two new local optima that bracket the original.  Shape V8
(1996 B, 68.7 %) reproduces the original's *entire register allocation* (`systems->r8`,
`agent->r9`, `used->sl`, `selected->fp`, `type->r2`, exit `mov r4,r2`, `status->r6`) but has a
104-byte frame and a top-of-loop condition; shape V6 (1992 B, 69.6 %) reproduces the original's
96-byte frame and bottom-of-loop condition but swaps `systems`/`selected` and folds the
`pick == 8` incoming to the constant 8.  No 1996-byte variant is byte-exact, so no code landed.

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
`osize 1996  usize 1996  pct 73.590096`.

## 1. Summary of the two new facts

1. **The `pick == 12` select survives CVP when the not-25 path jumps to a *shared* `type = pick`
   block.**  The wave-89 note (`generator-createmission-cvp-asymmetry-w3823.md`) established that
   a plain ternary `type = cond ? 12 : pick` is folded because the select lives in a block whose
   only predecessor ends in `icmp eq %pick, 12`.  Writing the not-25 path as
   `else goto assignType;` where `assignType:` is *also* the target of the `pick == 8` and
   `sum == 14` paths gives that block three predecessors, so SimplifyCFG cannot form a select
   before CVP and CVP cannot prove `pick == 12` there.  The select is formed later (if-conversion)
   and reaches instruction selection.  With this shape the *entire loop body* becomes
   instruction-for-instruction identical to the original except for two register choices:
   `type` lives in `r4` instead of `r2`, and `used` lives in `r8` instead of `sl` (the sum-loop
   `i`/temp registers are `r2`/`r4` in both, so `type`'s `r4` aliases the original's temp).
2. **Declaring the sum-loop index in the `for` statement moves `type` into `r2`.**  Changing
   ```
   int sum = 0;
   uint32_t i;
   for (i = 0; i < used->size(); ++i) sum += used->data()[i];
   ```
   to
   ```
   int sum = 0;
   for (uint32_t i = 0; i < used->size(); ++i) sum += used->data()[i];
   ```
   (same IR semantics, same instructions) changes the greedy allocator's choice for the loop's
   `type` value from `r4` to `r2`.  The loop body then matches the original except that `used`
   is in `r8` instead of `sl`.

Both facts are reproducible with the one-file harness in §6.

## 2. Baseline

```
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Build setup note (unchanged from w3823): in this worktree `cmake --preset default` alone resolves
`../_work` to the non-existent `/home/debian/GalaxyOnFire2/wt/_work`; configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` and
`-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.

## 3. Variant A — shared `assignType` latch (the surviving select)

Loop body (everything outside the loop is the wave-89 source):

```c++
    while (!selected) {
        selected = guard < 1;
        int pick = Globals::rnd->nextInt(15);
        --guard;
        type = pick;
        if (pick == 8) goto assignType;
        if (pick == 10) {
            type = 10;
            if (race > 3) goto nextIteration;
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
                goto nextIteration;
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
            goto nextIteration;
        }
        selected = true;
    assignType:
        type = pick;
    nextIteration:
        ;
    }
```

The IR keeps a two-level phi (`%37` header phi `[0, %27], [%68, %67]` plus a `%67` latch phi
`[%44,%65],[12,%79],[10,%46],[%44,%85]`), so `LazyValueInfoImpl::solveBlockValueSelect` /
`getEdgeValueLocal` never prove the latch value is `12`.  The select becomes the conditional
move at the latch.

### 3.1 Disassembly — original loop (`0x97248..0x972d4`, 140 B)

```
97248: f04f 0b00   mov.w   fp, #0
9724c: f1b9 0f01   cmp.w   r9, #1
97250: bfb8        it      lt
97252: f04f 0b01   movlt.w fp, #1
97256: 980a        ldr     r0, [sp, #40]
97258: 210f        movs    r1, #15
9725a: 6800        ldr     r0, [r0, #0]
9725c: f7ca eaf4   blx     nextInt
97260: f1a9 0901   sub.w   r9, r9, #1
97264: 4606        mov     r6, r0
97266: 2808        cmp     r0, #8
97268: d033        beq.n   972d2
9726a: 2e0a        cmp     r6, #10
9726c: d103        bne.n   97276
9726e: 9809        ldr     r0, [sp, #36]
97270: 220a        movs    r2, #10
97272: 2803        cmp     r0, #3
97274: d82e        bhi.n   972d4
97276: f1ba 0f00   cmp.w   sl, #0
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
9729a: 4632        mov     r2, r6
9729c: d11a        bne.n   972d4
9729e: e002        b.n     972a6
972a0: f800 5b01   strb.w  r5, [r0], #1
972a4: 3901        subs    r1, #1
972a6: 2900        cmp     r1, #0
972a8: d1fa        bne.n   972a0
972aa: e012        b.n     972d2
972ac: 2101        movs    r1, #1
972ae: 5581        strb    r1, [r0, r6]
972b0: 2e0c        cmp     r6, #12
972b2: d10c        bne.n   972ce
972b4: 9808        ldr     r0, [sp, #32]
972b6: 6800        ldr     r0, [r0, #0]
972b8: f7ca ebaa   blx     getSystem
972bc: f7ca ebde   blx     getIndex
972c0: 220c        movs    r2, #12
972c2: 2819        cmp     r0, #25
972c4: f04f 0b01   mov.w   fp, #1
972c8: bf18        it      ne
972ca: 4632        movne   r2, r6
972cc: e002        b.n     972d4
972ce: f04f 0b01   mov.w   fp, #1
972d2: 4632        mov     r2, r6
972d4: ea5f 70cb   movs.w  r0, fp, lsl #31
972d8: d0b6        beq.n   97248
```

### 3.2 Disassembly — variant A loop (`0x90..0x11c`, 140 B)

Byte-identical to §3.1 with `type`'s `r2` replaced by `r4`, `used`'s `sl` replaced by `r8`, and
the three spill slots shifted by +8 (`[sp,#48]` rnd, `[sp,#44]` race, `[sp,#40]` status):

```
  90: f04f 0b00   mov.w   fp, #0
  94: f1b9 0f01   cmp.w   r9, #1
  98: bfb8        it      lt
  9a: f04f 0b01   movlt.w fp, #1
  9e: 980c        ldr     r0, [sp, #48]
  a0: 210f        movs    r1, #15
  a2: 6800        ldr     r0, [r0, #0]
  a4: f7ff fffe   bl      nextInt
  a8: f1a9 0901   sub.w   r9, r9, #1
  ac: 4606        mov     r6, r0
  ae: 2808        cmp     r0, #8
  b0: d033        beq.n   11a
  b2: 2e0a        cmp     r6, #10
  b4: d103        bne.n   be
  b6: 980b        ldr     r0, [sp, #44]
  b8: 240a        movs    r4, #10
  ba: 2803        cmp     r0, #3
  bc: d82e        bhi.n   11c
  be: f1b8 0f00   cmp.w   r8, #0
  c2: d019        beq.n   f8
  c4: f8d8 0004   ldr.w   r0, [r8, #4]
  c8: 5d81        ldrb    r1, [r0, r6]
  ca: b199        cbz     r1, f4
  cc: f8d8 1000   ldr.w   r1, [r8]
  d0: 2200        movs    r2, #0
  d2: 2300        movs    r3, #0
  d4: e002        b.n     dc
  d6: 5c84        ldrb    r4, [r0, r2]
  d8: 3201        adds    r2, #1
  da: 4423        add     r3, r4
  dc: 4291        cmp     r1, r2
  de: d1fa        bne.n   d6
  e0: 2b0e        cmp     r3, #14
  e2: 4634        mov     r4, r6
  e4: d11a        bne.n   11c
  e6: e002        b.n     ee
  e8: f800 5b01   strb.w  r5, [r0], #1
  ec: 3901        subs    r1, #1
  ee: 2900        cmp     r1, #0
  f0: d1fa        bne.n   e8
  f2: e012        b.n     11a
  f4: 2101        movs    r1, #1
  f6: 5581        strb    r1, [r0, r6]
  f8: 2e0c        cmp     r6, #12
  fa: d10c        bne.n   116
  fc: 980a        ldr     r0, [sp, #40]
  fe: 6800        ldr     r0, [r0, #0]
 100: f7ff fffe   bl      getSystem
 104: f7ff fffe   bl      getIndex
 108: 240c        movs    r4, #12
 10a: 2819        cmp     r0, #25
 10c: f04f 0b01   mov.w   fp, #1
 110: bf18        it      ne
 112: 4634        movne   r4, r6
 114: e002        b.n     11c
 116: f04f 0b01   mov.w   fp, #1
 11a: 4634        mov     r4, r6
 11c: ea5f 70cb   movs.w  r0, fp, #31
```

Result: `usize 1960` (36 B short), official pct **56.9**.  The 36-byte deficit is entirely in the
tail: the surviving select raises callee-saved pressure by one, the allocator moves `used` from
`sl` to `r8` and `systems` from `r8` to `sl`, the frame grows 96 -> 104, and the tail keeps `type`
in `r4` across the tail calls instead of copying `r2 -> r4` at the loop exit.

## 4. Variant B — `for (uint32_t i = 0; ...)` moves `type` to `r2`

Take variant A and change only the sum loop's index declaration:

```c++
-                int sum = 0;
-                uint32_t i;
-                for (i = 0; i < used->size(); ++i) {
+                int sum = 0;
+                for (uint32_t i = 0; i < used->size(); ++i) {
                     sum += used->data()[i];
                 }
```

The IR is unchanged; only the source-level scoping differs.  The allocator now puts the loop's
`type` in `r2`:

```
 108: 220c        movs    r2, #12
 10a: 2819        cmp     r0, #25
 10c: f04f 0b01   mov.w   fp, #1
 110: bf18        it      ne
 112: 4632        movne   r2, r6
 114: e002        b.n     11c
 116: f04f 0b01   mov.w   fp, #1
 11a: 4632        mov     r2, r6
 11c: ea5f 70cb   movs.w  r0, fp, #31
```

This is the original's `movs r2,#12 / cmp r0,#25 / it ne / movne r2,r6 / ... / mov r2,r6` shape.
The loop is now identical to §3.1 except that `used` is `r8` (original `sl`), so
`cmp.w r8,#0`, `ldr.w r0,[r8,#4]`, `ldr.w r1,[r8]` replace the `sl` forms.

Result: `usize 1976` (20 B short), official pct **59.9**.  Frame is still 104; at the loop exit
the tail copies `mov r6,r2` (original `mov r4,r2`) because `r4` is the GOT pointer in this build.

## 5. Measurements (official verifier, `autojunk=False`)

| variant | usize | pct | gate |
|---|---:|---:|---|
| HEAD (wave-89 source, `short pick`, `0.5000001f`) | 1996 | **73.590096** | PASS |
| A: shared `assignType` latch (`int pick`) | 1960 | 56.9 | FAIL size (byte 2045) |
| B: A + inline sum-loop `i` | 1976 | 59.9 | FAIL size |
| B + `used` declared before `race` | 1972 | 58.6 | FAIL size |
| `tern` pick==12 (select folds) | 1964 | 54.8 | FAIL size |
| HEAD + inline sum-loop `i` | 1996 | 73.6 | PASS (no score change) |

**The gate is size-sensitive.**  `createMission` must be exactly 1996 B: a 36-byte shrink moves
`.text`/`.got`, changes the absolute GOT words in every later function's literal pool, and drops
`byte_exact` from 4498 to 2045 (measured).  So a variant with a better loop but a 20–36 B shorter
tail cannot land even though its loop matches.

## 6. Reproduction harness

One-file compile (no link needed) from `compile_commands.json`:

```sh
CC=$(python3 - <<'PY'
import json
cc=json.load(open('cmake-build-match/compile_commands.json'))
for e in cc:
    if e['file'].endswith('mission/Generator.cpp'):
        print(e['command']); break
PY
)
CC=$(echo "$CC" | sed -E "s#-o [^ ]+#-o /dev/shm/w4004/x.o#; s#-c [^ ]+#-c $PWD/src/game/mission/Generator.cpp#")
eval "$CC"
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .text._ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE /dev/shm/w4004/x.o
```

Full gate: `cmake --build cmake-build-match --target check -j8`.
Official per-function row: run `tools/verify/verify.py` (see §2) and read
`cmake-build-match/verify/report.json`.

## 7. Remaining work (one line)

Keep the shared-latch select (variant B) **and** make the tail allocate `systems -> r8`,
`used -> sl`, `type -> r4` at the loop exit, so the function is 1996 B.  The greedy allocator
currently prefers `used -> r8` whenever the select survives; `used`'s loop uses dominate its
spill weight.  Finding the source shape that reverses that one assignment is the remaining task.

## 8. Wave-90 second pass: the register-allocation / loop-layout trade-off

This section records a second, independent sweep run after §1–§7.  It found two new local optima
that bracket the original: one reproduces the original's **register allocation exactly** but has
the wrong frame size and loop layout (1996 B, 68.7 %), the other reproduces the original's
**frame size and loop layout** but permutes three callee-saved registers (1992 B, 69.6 %).  No
variant is byte-exact and none scores above 73.590096, so no code change landed.

### 8.1 Shape V8 — exact original register allocation, wrong frame/loop layout

Source (the only changes vs variant B in §3 are the `pick == 10` test and `continue` in the
`used`/`pick == 12` exits):

```c++
    while (!selected) {
        selected = guard < 1;
        int pick = Globals::rnd->nextInt(15);
        --guard;
        type = pick;
        if (pick == 8) goto assignType;
        if (pick == 10 && race > 3) {
            type = 10;
            continue;
        }
        if (pick == 10) type = 10;
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

Measured: `usize 1996`, official pct **68.700000**, `byte=False linked=False`.

The **register allocation is the original's**:

| value | original | V8 |
|---|---|---|
| `systems` | r8 | **r8** |
| `agent` | r9 (spilled) | **r9 (spilled)** |
| `used` | sl | **sl** |
| `selected` | fp | **fp** |
| `type` (in loop) | r2 | **r2** |
| `type` (at exit) | r4 (`mov r4,r2`) | **r4 (`mov r4,r2`)** |
| `status` (at exit) | r6 (`ldr r6,[sp,#32]`) | **r6 (`ldr r6,[sp,#40]`)** |
| `pick` | r6 | r6 |
| `guard` | r9 | r9 |
| `targetStation` | r5 | r5 |

Prologue (V8, object offsets):

```
   c: b09a        sub  sp, #104          ; original: #96
  10: 4690        mov  r8, r2            ; systems -> r8   (original identical)
  12: 4689        mov  r9, r1            ; agent   -> r9   (original identical)
  68: f8cd 9024   str.w r9, [sp, #36]    ; original: [sp,#24]
  82: f8d0 a050   ldr.w sl, [r0, #80]    ; used -> sl      (original identical)
```

Loop exit (V8):

```
 124: 9e08        ldr  r6, [sp, #40]     ; original: [sp,#32]
 126: 4614        mov  r4, r2            ; original identical
 128: 6830        ldr  r0, [r6, #0]
 12a: f7ff fffe   bl   Status::getCurrentCampaignMission
```

So the only remaining differences are (a) the frame is **104 vs 96** (all spill offsets +8) and
(b) the loop condition block is laid out **before** the body (`movs.w r0,fp,lsl#31; bne.n 124`
at `0x8e`) instead of after it (`beq.n 90` at `0x11e` in the original).

The loop body itself:

```
  b2: 2808        cmp  r0, #8
  b4: d101        bne.n ba
  b6: 4632        mov  r2, r6            ; pick==8 -> shared latch (original identical)
  b8: e7e9        b.n  8e                ; ... but jumps to the *top* condition
  ba: 980b        ldr  r0, [sp, #44]     ; race
  bc: 2804        cmp  r0, #4
  be: d302        bcc.n c0
  c0: 220a        movs r2, #10           ; type = 10
  c2: 2e0a        cmp  r6, #10
  c4: d0e3        beq.n 8e
```

The original checks `pick` first (`cmp r6,#10; bne; ldr race; movs r2,#10; cmp race,#3; bhi`),
V8 checks `race` first (the `&&` is re-ordered around the `type = 10` store).

### 8.2 Shape V6 — exact frame + loop layout, permuted registers

Take V8 and change only `if (pick == 8) goto assignType;` to `if (pick == 8) continue;`.
Measured: `usize 1992`, official pct **69.6**, `byte=False linked=False`.

This flips the allocator into the original's frame/layout:

```
   c: b098        sub  sp, #96           ; original identical
  10: 4693        mov  fp, r2            ; systems -> fp   (original: r8)
  12: 4688        mov  r8, r1            ; agent   -> r8   (original: r9)
  7e: f04f 0800   mov.w r8, #0           ; selected -> r8  (original: fp)
  82: f8d0 a050   ldr.w sl, [r0, #80]    ; used -> sl      (original identical)
 ...
  ac: 4606        mov  r6, r0
  ae: 2208        movs r2, #8            ; pick==8 incoming folded to 8 (original: mov r2,r6)
  b0: 2808        cmp  r0, #8
  b2: d034        beq.n 11e              ; -> bottom condition
 ...
 11c: 4632        mov  r2, r6            ; shared latch (sum==14 / else)
 11e: ea5f 70c8   movs.w r0, r8, lsl #31
 122: d0b5        beq.n 90               ; back edge
 124: 9e08        ldr  r6, [sp, #32]     ; original identical
 126: 4614        mov  r4, r2            ; original identical
```

So the loop layout and the tail now match the original, but `systems`/`selected` are swapped
(fp/r8) and the `pick == 8` phi incoming is the constant 8 (`movs r2,#8`) instead of `pick`
(`mov r2,r6`).

V6 is 1992 B: its code is 1948 B (8 B shorter than the original's 1956 B) and its pool is 44 B
(4 B longer, because the `0.5000001f` anchor is still present).  The 8-byte code deficit is
entirely in the tail (the register permutation changes scheduling: e.g. V6 emits an extra
`mov.w sl,#0` at `0x438`, `vcvt.f32.s32 s2,s2` at `0x490`, `vldr s4,[pc,#768]` at `0x4a0`,
`str.w r8,[sp,#16]` at `0x4ee`, `ldr r0,[sp,#16]` at `0x5ba` and `vldr s2,[pc,#424]` at `0x60e`,
and drops `vldr s4`, `vmov s2,r5`, `vcvt.f32.s32 s2,s2`, `strd r4,r5,[sp,#8]` and `mov r0,r9`).

**Gate consequence:** a 4-byte size change is fatal.  Running the full `check` on V6 gives
`byte_exact 4498 -> 2615` (measured), so V6 cannot land even though its fuzzy score is higher
than V8's.

### 8.3 Why V8's frame is 8 bytes larger (stack-slot colouring)

The extra 8 bytes are one 4-byte spill plus 4 bytes of alignment padding.  V8's slots:

```
[20] difficulty (r5) spill      <- extra vs original
[24] bonus
[28] GOT base
[32] targetStation
[36] agent
[40] status
[44] race
[48] rnd
```

The original's slots:

```
[16] bonus / difficulty / r5 spill (shared)
[20] GOT base
[24] agent / difficulty (shared)
[28] targetStation
[32] status
[36] race
[40] rnd
```

In the original, `agent` is restored to r9 at `0x97306` (`ldrd r9,r4,[sp,#24]`), which kills the
`[sp,#24]` slot, so the later `difficulty` spill at `0x9752a` (`str r6,[sp,#24]`) reuses it.  In
V8 the agent slot `[sp,#36]` is reloaded again at `0x482` and `0x5a8`, so it is still live when
`difficulty` is spilled at `0x37c` (`str r5,[sp,#20]`); the colourer cannot merge them.

The `[16]` slot in the original is shared by the `type == 15` loop's r5 spill (`0x97614`),
`bonus` (`0x976d8`/`0x97720`) and the `distance` argument (`0x97644`/`0x9783a`) because those
live ranges are disjoint in execution order (the `type == 15` case is laid out at the end of the
function but executes before the common path).

### 8.4 Full sweep of loop shapes (fit score)

A 7-feature fit was used to screen shapes quickly (one-file compile, ~0.24 s each):

* `sys_r8`   — prologue has `mov r8, r2`
* `agent_r9` — prologue has `mov r9, r1`
* `used_sl`  — prologue has `ldr.w sl, [r0, #80]`
* `frame96`  — `sub sp, #96`
* `type_r4_exit` — a `mov r4, r2` exists
* `sel`      — `movs r2,#12` + `cmp r0,#25` + `movne r2,r6` in the loop
* `pick8_beq`— `cmp r0,#8` immediately followed by `beq.n`

| shape | sys_r8 | agent_r9 | used_sl | frame96 | type_r4_exit | sel | pick8_beq | score |
|---|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| HEAD (`short pick`, folded select) | 1 | 1 | 0 | 1 | 0 | 0 | 0 | 3 |
| variant B (§3, `pick==8 goto`, nested `pick==10`) | 0 | 1 | 0 | 0 | 0 | 1 | 1 | 3 |
| V8 (`pick==8 goto`, `pick==10 &&`, `continue`) | 1 | 1 | 1 | 0 | 1 | 1 | 0 | **5** |
| V6 (`pick==8 continue`, `pick==10 &&`, `continue`) | 0 | 1 | 1 | 1 | 1 | 1 | 1 | **6** |

A sweep of `{while,for,do,for(;;)} × {pick==8: goto/continue} × {pick==10: nested/nested_cont/and/and_cont} ×
{pick==12: goto/inline} × {latch: shared/shared_next}` (128 shapes) produced exactly four
score-6 shapes, all of them `pick==8 continue` + `pick==10 && ... continue` (V6 and its
`for`/`shared_next` variants).  No shape scored 7.

Changing `bool selected` to `int`/`char`, `int32_t type` to `int`, `uint32_t race` to `int`,
`int guard` to `uint32_t`, moving `level`/`used`/`difficulty` declarations, and moving
`difficulty` to function scope all leave the fit unchanged for V8 and V6.

### 8.5 Conclusion of the second pass

The two requirements are mutually exclusive under every source spelling tried:

* to keep the original's `pick == 8` phi incoming (`pick`, shared latch) the branch must be
  `goto assignType`, which makes the allocator choose `systems -> r8` (good) but also
  `frame 104` and a top-of-loop condition (bad);
* to get `frame 96` and the bottom-of-loop condition the branch must be `continue`, which folds
  the `pick == 8` incoming to the constant 8 and swaps `systems`/`selected` (bad).

A byte-exact reconstruction needs a spelling that keeps `goto assignType` for the `pick == 8`
path **and** gets the original's 96-byte frame, i.e. one where the agent spill slot dies before
the `difficulty` spill (so the colourer merges them).  That is the remaining task.

### 8.6 Reproduction (second pass)

```sh
# V8
python3 /dev/shm/w4004/v8.py and_cont goto shared   # writes /dev/shm/w4004/var8.cpp
cp /dev/shm/w4004/var8.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w4004/scorefn.py                   # osize=1996 usize=1996 pct=68.7

# V6
python3 /dev/shm/w4004/gen2.py while top cont and_cont goto shared
cp /dev/shm/w4004/var2.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w4004/scorefn.py                   # osize=1996 usize=1992 pct=69.6
cmake --build cmake-build-match --target check -j8  # byte_exact 4498 -> 2615 (FAIL)
```

(The scratch helpers live in `/dev/shm/w4004`; `gen2.py`/`v8.py`/`gen3.py` are the generators,
`fit.py` the 7-feature screen, `scorefn.py` the official per-function verifier.)

## 9. Third pass (wave 90): X4 — bottom-of-loop condition + shared latch; the tail's
## `difficulty` register is the remaining blocker

Status: still no byte-exact shape. Best new local optimum is **X4: 1996 B, official pct
70.400000** (vs V8 68.7, HEAD 73.590096). X4 is the first shape that reproduces the
original's *loop* layout exactly: the loop condition is at the bottom
(`movs.w r0,fp,lsl#31; beq.n ...` at the latch), the `pick == 8` path joins the shared
`mov r2,r6` latch, and the `pick == 12` select survives CVP. Its tail is still V8's tail
(104-byte frame, `difficulty` in `r5`, agent spilled), so it cannot land.

### 9.1 The X4 shape

X4 is V8 with exactly one edit:

```c++
        if (__builtin_expect(pick == 12, 0)) {
```

instead of `if (pick == 12) {`.  Everything else is the §8.1 V8 loop and the wave-89
tail.  (The `__builtin_expect` is a real branch-weight statement: the original's `pick == 12`
select block is laid out after the `selected = true` path and reached with a forward
conditional branch; without the expect clang sinks the whole block and the loop condition
returns to the top.)

Measured with the official verifier (`scorefn.py`, `autojunk=False`):

```
osize=1996 usize=1996 pct=70.400000 byte=False linked=False
```

Loop region (original vs X4, addresses normalised):

```
  97264: mov  r6, r0            mov  r6, r0
  97266: cmp  r0, #8            cmp  r0, #8
  97268: beq.n 972d2            beq.n 972b8      ; shared latch (target differs)
  9726a: cmp  r6, #10           ldr  r0, [sp, #44]   ; (c) pick-first vs race-first
  9726c: bne.n 97276            cmp  r0, #4
  9726e: ldr  r0, [sp, #36]     bcc.n 97276
  97270: movs r2, #10           movs r2, #10
  97272: cmp  r0, #3            cmp  r6, #10
  97274: bhi.n 972d4            beq.n 972d4
  ...
  972b0: cmp  r6, #12           cmp  r6, #12
  972b2: bne.n 972ce           beq.n 972bc      ; (d) select block inline vs out-of-line
  972b4: ... select inline      ... not-12 path inline
  972ce: mov.w fp, #1           mov.w fp, #1
  972d2: mov  r2, r6            mov  r2, r6
  972d4: movs.w r0, fp, lsl #31 movs.w r0, fp, lsl #31
  972d8: beq.n 97248            beq.n 97248
```

Remaining loop diffs:

* **(a)** the `pick == 12` select block is out-of-line in X4
  (`cmp r6,#12; beq 972bc`), inline in the original (`cmp r6,#12; bne 972ce`); the
  non-select `mov.w fp,#1` is inline in X4, out-of-line in the original.  This is the
  direct effect of `__builtin_expect(pick == 12, 0)`.
* **(b)** the `pick == 10` test is race-first in X4
  (`ldr r0,[sp,#44]; cmp r0,#4; bcc; movs r2,#10; cmp r6,#10; beq`) but pick-first in
  the original (`cmp r6,#10; bne; ldr race; movs r2,#10; cmp race,#3; bhi`).
* **(c)** the frame is 104 (original 96); every spill offset is +8.
* **(d)** the tail's `difficulty` lives in `r5` (X4) instead of `r6` (original), so the
  agent spill slot `[sp,#36]` stays live and the colourer cannot merge it with
  `difficulty` (the §8.3 mechanism).

Normalised diff hunks (instruction text with `[sp,#N]`, `[pc,#N]`, branch targets and
small immediates folded): **X4 88, HEAD 98, V8 89**.  Raw official diff hunks: X4 136,
V8 139.

### 9.2 The tail's `difficulty` register — root cause is store sinking

The original's tail allocates `difficulty` to `r6` and spills it to `[sp,#24]`; X4/HEAD/V6
allocate it to `r5` and never spill it.  Both have the same free callee-saved set at the
`++difficulty` point (`r5` and `r6` are both dead), so the choice is decided by the
*scheduling* of the `type == 8` block:

```
original (9748e-974ac)                 X4/HEAD (974a0-974bc)
  bl nextInt(15)                         bl nextInt(15)
  mov  r6, r0        ; amount temp       mov  r6, r0        ; amount temp
  ldr  r0, [r5, #4]                      ldr  r0, [r5, #4]
  bl   getTecLevel                       bl   getTecLevel
  mov  r1, r0        ; tec temp          mov  r5, r0        ; difficulty -> r5
  adds r0, r6, #5    ; amount             adds r0, r6, #5    ; amount
  str  r0, [sp, #28] ; STORE in block     b    975fa        ; store SUNK to join
  mov  r6, r1        ; difficulty -> r6   (str r0,[sp,#20] at the join)
  b    9760a
```

Because the original stores `amount` *inside* the block, `r0` is free for the amount and
`r6` (the just-dead amount temp) is reused for `difficulty`.  In X4/HEAD the amount store
is sunk by `machine-sink` to the common join block, so `r0` keeps the `getTecLevel`
result and `difficulty` is scheduled earlier into `r5`.  Once `difficulty` is in `r5`,
`r5` is needed for `fromSystemPtr`/`galaxy`, `difficulty` moves to `r9`, and the agent is
spilled instead — the whole 104-byte tail follows.

The store is sunk only when every path reaches a single join block that can hold the
store.  In the original the join is at `9760a` (`ldr r0,[pc,...]` galaxy) and the stores
are in the predecessor blocks (`974a8`, `975ca`, `97604`, `97548`); in X4/HEAD the join is
at `975fa` (`str r0,[sp,#N]`) and the galaxy load follows at `975fc`.

### 9.3 Forcing `difficulty` into `r6` (and why it still fails)

The only source change found that moves `difficulty` to `r6` is to keep the `items` local
live across the `else` block, e.g.

```c++
    int itemId = 0;
    int amount = 0;
    Array<Item *> *items = Globals::items;   // hoisted out of the type == 8 branch
    if (type == 8) {
        ...
        amount = Globals::rnd->nextInt(15) + 5;
        int tec = (*items)[itemId]->getTecLevel();
        difficulty = tec;
    } else { ... }
    ...
    } else if (type == 8) {
        reward = (float) ((*items)[itemId]->getMaxPrice() * amount) * 1.7f;
    }
```

Measured: **1996 B, 63.2 %** (frame 104, 716 instructions).  The hoisted `items` live
range spans the `else` block, so the colourer reserves `r5` for it and gives `difficulty`
`r6` (`mov r6,r0` / `adds r6,r0,#1` appear).  But `items` itself then needs a spill slot
(`str r5,[sp,#40]` / `ldr r1,[sp,#40]`), the frame grows to 104, and the tail is *worse*
than X4's.  Adding the X4 expect to that shape gives **1992 B, 64.1 %**.

A 144-combination sweep over `{itemId/amount declaration order} × {items local vs hoisted}
× {amount-then-difficulty, difficulty-then-amount, amount/tec/difficulty, tec/difficulty/amount}
× {++difficulty, difficulty++, difficulty = difficulty + 1} × {reward uses Globals::items vs
the local}` found 36 shapes with `difficulty` in `r6`; **all 36 hoist `items` and all 36 have
a 104-byte frame**.  No shape has both `r6` and frame 96.

### 9.4 Other tail sweeps (all official verifier, `createMission` size noted)

| shape | size | pct |
|---|---:|---:|
| X4 (`expect(pick==12,0)` on V8) | 1996 | **70.4** |
| X4 + `int tec` temp + `0.5f` true arm | 1996 | 68.7 |
| X4 + `0.5f` true arm only | 1992 | 70.6 |
| X4 + amount temp | 1996 | 70.4 |
| X4 + difficulty init/ternary/++/galaxy order | 1996 | 70.4 |
| X4 + `items` hoisted + local reward use | 1996 | 63.2 |
| X4 + `items` hoisted + local reward use + expect | 1992 | 64.1 |
| HEAD + `expect(pick==12,0)` | 1996 | 72.9 |
| HEAD + `int pick` + expect | 1960 | 48.3 |
| `amount` short/int16/char/int8 (HEAD) | 2000/2000/2000/2000 | 69.0 |
| `difficulty` volatile (HEAD) | 1988 | 68.0 |

Loop-shape sweep: 192 `{while,for,do,for(;;)} × {guard top/before} × {pick8 goto/cont} ×
{pick10 nested/nested_cont/and/and_cont} × {pick12 goto/inline} × {latch shared/shared_next}`
shapes; 56 have a 96-byte frame, 136 have 104, **none** has `difficulty` in `r6`.

### 9.5 Conclusion of the third pass

X4 proves the loop can be reproduced exactly with a real branch-weight statement, and
isolates the remaining blocker to the tail's `difficulty` register.  That register is a
downstream consequence of `machine-sink` moving the `amount` store to the join block; the
only way found to keep `difficulty` in `r6` is to keep `items` live across the `else`
block, which costs an extra spill slot and a 104-byte frame.  A byte-exact shape needs a
source form in which the `amount` store stays in the `type == 8` block (so the amount temp
in `r6` is reused for `difficulty`) **without** keeping `items` live.  That is the
remaining task.

### 9.6 Reproduction (third pass)

```sh
git checkout -- src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j8      # baseline PASS

# X4
python3 /dev/shm/w4004/genX.py X4
cp /dev/shm/w4004/varX.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w4004/scorefn.py                       # 1996/1996, pct 70.4
python3 /dev/shm/w4004/ndiff.py > /dev/shm/w4004/n_x4.txt   # 88 normalised hunks
```

Scratch helpers: `genX.py` (X4 generator), `scorefn.py` (official verifier),
`ndiff.py` (normalised diff), `tsig.py` (one-TU compile + signature),
`brute_tail.py`/`t5.py`/`t6.py` (tail sweeps), `sweep_sig.py` (192-shape loop sweep).
