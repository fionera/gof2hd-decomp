# Generator::createMission — shared-latch select survives CVP (w4004)

Status: research note (2026-11-03, wave 90). No source change landed: every variant below
either changes `createMission`'s size (which shifts `.text`/`.got` and collapses the gate's
`byte_exact` count) or scores below the current 73.590096. Worktree clean; `check` passes at
`byte 4498 / linked 4498`, `.text 0x1337c8`, lint CLEAN.

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
