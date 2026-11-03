# Generator::createMission — nested pick-first loop is five differences from the original (w4104)

Status: research note (2026-11-03, wave 91). **No source change landed**: every shape below
either changes `createMission`'s size (which shifts `.text`/`.got` and collapses the gate's
`byte_exact` count) or scores below the current 73.590096. Worktree clean; `check` passes at
`byte 4498 / linked 4498`, `.text 0x1337c8`, data 843/843, imports 207, lint CLEAN, drift 0.

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
`osize 1996  usize 1996  pct 73.590096` (HEAD).

This pass isolates the *loop* completely: the wave-90 `nested` pick-first shape reproduces the
original loop body instruction-for-instruction **except for five content differences**, all of
which are the `systems`/`used`/`rnd` register-allocation choice.  It also shows the `galaxy_after_from`
tail gives the original's `difficulty -> r6` / `[sp,#24]` clamp, and that a `none` +
`nested_after` spelling reproduces the original `type == 8` tail block *exactly*
(`mov r1,r0; adds r0,r6,#5; str r0,[sp,#N]; mov r6,r1`).  The remaining blocker is that the
loop shape that produces the pick-first `pick == 10` code also swaps `systems`/`used`.

## 0. Baseline

```
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 1. Summary of the new facts

1. **The `nested` pick-first loop is five content differences from the original.**  With
   `if (pick == 10) { type = 10; if (race > 3) continue; }` (the wave-90 `nested` form) the loop
   is the same 146 bytes as the original (`0x90..0x122` vs `0x97248..0x972da`) and differs only
   in the `rnd` access (the variant keeps `&Globals::rnd` in `r4`, so its dereference is one
   instruction instead of the original's `ldr r0,[sp,#40]; ldr r0,[r0,#0]`, compensated by one
   `ldr r4,[sp,#40]` reload after the sum loop) and the `used` register (`r8` vs `sl`,
   3 instructions).  **All spill offsets inside the loop match** (`[sp,#36]` race,
   `[sp,#32]` status, `[sp,#40]` rnd slot).
2. **`galaxy_after_from` gives the original tail.**  Moving `Galaxy *galaxy = Globals::galaxy;`
   *after* `int fromSystem = ...` makes `difficulty` live in `r6`, spills it to `[sp,#24]`, and
   emits the original clamp `cmp r6,#10; it ge; movge r6,#10; str r6,[sp,#24]`.  Frame is 96.
3. **The exact `type == 8` tail block is reachable.**  `typeassign='none'` + `nested_after`
   (`if (pick == 10) { if (race > 3) continue; type = 10; }`) emits
   `mov r1,r0; adds r0,r6,#5; str r0,[sp,#24]; mov r6,r1` — the original's
   `mov r1,r0; adds r0,r6,#5; str r0,[sp,#28]; mov r6,r1` with only the slot number differing.
   But that spelling also moves the loop condition to the top and reorders the `pick == 8/10`
   tests, so it scores worse.
4. **`X4 + volatile int amount` produces every target feature except pick-first**
   (`systems->r8`, `used->sl`, frame 96, `difficulty->r6`, surviving `pick == 12` select) but is
   only 64.6 % because the volatile anchor perturbs the rest of the tail.
5. **The `systems`/`used` swap is robust.**  1440 declaration permutations (720 orders x
   `int level;`/`int level = 0;`) on the `nested` + `galaxy_after_from` source, 324
   type/declaration variants, and a 192-shape loop sweep all leave the pick-first shapes at
   `systems->sl`, `used->r8`.

## 2. The nested pick-first loop

Source (loop only; the rest is the wave-89/90 source with `int pick`):

```c++
    while (!selected) {
        selected = guard < 1;
        int pick = Globals::rnd->nextInt(15);
        --guard;
        type = pick;
        if (pick == 8) goto assignType;
        if (pick == 10) {
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

### 2.1 Original loop (`0x97248..0x972da`, 146 B)

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

### 2.2 Nested variant loop (`0x90..0x122`, 146 B)

Same instruction stream; the content differences are marked `<<` (the original has an extra
`ldr r0,[r0,#0]` at `9725a`; the variant has an extra `ldr r4,[sp,#40]` at `de`):

```
  90: f04f 0b00   mov.w   fp, #0
  94: f1b9 0f01   cmp.w   r9, #1
  98: bfb8        it      lt
  9a: f04f 0b01   movlt.w fp, #1
  9e: 6820        ldr     r0, [r4, #0]          << original: ldr r0,[sp,#40]; ldr r0,[r0,#0]
  a0: 210f        movs    r1, #15
  a2: f7ff fffe   bl      nextInt
  a6: f1a9 0901   sub.w   r9, r9, #1
  aa: 4606        mov     r6, r0
  ac: 2808        cmp     r0, #8
  ae: d034        beq.n   11a
  b0: 2e0a        cmp     r6, #10
  b2: d103        bne.n   bc
  b4: 9809        ldr     r0, [sp, #36]
  b6: 220a        movs    r2, #10
  b8: 2803        cmp     r0, #3
  ba: d82f        bhi.n   11c
  bc: f1b8 0f00   cmp.w   r8, #0               << original: cmp.w sl,#0
  c0: d019        beq.n   f8
  c2: f8d8 0004   ldr.w   r0, [r8, #4]         << original: [sl,#4]
  c6: 5d81        ldrb    r1, [r0, r6]
  c8: b199        cbz     r1, f4
  ca: f8d8 1000   ldr.w   r1, [r8]             << original: [sl]
  ce: 2200        movs    r2, #0
  d0: 2300        movs    r3, #0
  d2: e002        b.n     da
  d4: 5c84        ldrb    r4, [r0, r2]
  d6: 3201        adds    r2, #1
  d8: 4423        add     r3, r4
  da: 4291        cmp     r1, r2
  dc: d1fa        bne.n   d4
  de: 9c0a        ldr     r4, [sp, #40]         << extra reload (original: none)
  e0: 2b0e        cmp     r3, #14
  e2: 4632        mov     r2, r6
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
  fc: 9808        ldr     r0, [sp, #32]         ; status slot matches!
  fe: 6800        ldr     r0, [r0, #0]
 100: f7ff fffe   bl      getSystem
 104: f7ff fffe   bl      getIndex
 108: 220c        movs    r2, #12
 10a: 2819        cmp     r0, #25
 10c: f04f 0b01   mov.w   fp, #1
 110: bf18        it      ne
 112: 4632        movne   r2, r6
 114: e002        b.n     11c
 116: f04f 0b01   mov.w   fp, #1
 11a: 4632        mov     r2, r6
 11c: ea5f 70cb   movs.w  r0, fp, #31
 120: d0b6        beq.n   90
```

The loop is byte-identical apart from `rnd` (kept in `r4` vs reloaded from `[sp,#40]`, with the
compensating `ldr r4,[sp,#40]` after the sum loop) and `used` (`r8` vs `sl`, 3 instructions).  The `race` slot (`[sp,#36]`) and the
`status` slot (`[sp,#32]`) already match the original.  The `pick == 10` code is the original's
pick-first form (`cmp r6,#10; bne; ldr race; movs r2,#10; cmp race,#3; bhi`), and the
`pick == 12` select survives (`movs r2,#12; cmp r0,#25; it ne; movne r2,r6`).

## 3. The `galaxy_after_from` tail (difficulty -> r6, frame 96)

```c++
    if (difficulty >= 10) {
        difficulty = 10;
    }

    int fromSystem = Globals::status->getStation()->getSystem();
    Galaxy *galaxy = Globals::galaxy;
    SolarSystem *fromSystemPtr = systems->data()[fromSystem];
    SolarSystem *toSystem = systems->data()[galaxy->getStation(targetStation)->getSystem()];
```

Original tail (`0x9748e..0x974ac`, then `0x9760a..0x9761c`):

```
9748e: 980a        ldr     r0, [sp, #40]      ; rnd
97490: 6800        ldr     r0, [r0, #0]
97492: 210f        movs    r1, #15
97494: f7ca eaf4   blx     nextInt
97498: 4606        mov     r6, r0             ; amount temp
9749a: 6868        ldr     r0, [r5, #4]       ; items->data
9749c: f85a 002a   ldr.w   r0, [r0, sl, lsl #2]
974a0: f7ca eafe   blx     getTecLevel
974a4: 4601        mov     r1, r0             ; tec temp
974a6: 1d70        adds    r0, r6, #5         ; amount
974a8: 9007        str     r0, [sp, #28]      ; STORE IN BLOCK
974aa: 460e        mov     r6, r1             ; difficulty -> r6
974ac: e0ad        b.n     9760a
...
9760a: 48db        ldr     r0, [pc, #876]     ; &Globals::galaxy
9760c: 2e0a        cmp     r6, #10
9760e: 4478        add     r0, pc
97610: 6805        ldr     r5, [r0, #0]
97612: 6800        ldr     r0, [r5, #0]
97614: 9004        str     r0, [sp, #16]
97616: bf48        it      ge
97618: f04f 060a   movge   r6, #10
9761a: 9808        ldr     r0, [sp, #32]
9761c: 9606        str     r6, [sp, #24]      ; difficulty spill
```

The `nested` + `galaxy_after_from` variant emits the same `difficulty -> r6` clamp and the same
`str r6,[sp,#24]` spill, but its `type == 8` block still sinks the `amount` store to the join:

```
 2f6: bl   getTecLevel
 2fa: adds r5, r6, #5          ; amount -> r5
 2fc: mov  r6, r0              ; difficulty -> r6
 2fe: b    43a                 ; store sunk to join
...
 43a: str  r5, [sp, #16]       ; amount store
 43c: cmp  r6, #10
 43e: it   ge
 440: movge r6, #10
 446: str  r6, [sp, #24]       ; difficulty spill (matches original)
```

So `galaxy_after_from` fixes the `difficulty` register and the clamp, but not the store sinking.

## 4. The exact `type == 8` tail block (`none` + `nested_after`)

Changing only the loop spelling to `typeassign='none'` + `nested_after`
(`if (pick == 10) { if (race > 3) continue; type = 10; }`, i.e. no `type = pick` at the top)
makes the `amount` store stay in the block and reproduces the original instruction sequence:

```
 2d2: bl   getTecLevel
 2d6: mov  r1, r0              ; tec temp
 2d8: adds r0, r6, #5          ; amount
 2da: str  r0, [sp, #24]       ; STORE IN BLOCK  (original [sp,#28])
 2dc: mov  r6, r1              ; difficulty -> r6
 2de: b    424
```

This is the original's `mov r1,r0; adds r0,r6,#5; str r0,[sp,#N]; mov r6,r1` exactly, with only
the spill slot differing (`#24` vs `#28`).  But the same spelling moves the loop condition to the
top and checks `pick == 10` before `pick == 8`, so the loop no longer matches; official score
58.7 %, `usize 1932`.

## 5. The `systems`/`used`/`rnd` swap is the blocker

The original allocates `systems -> r8`, `agent -> r9`, `used -> sl`, `selected -> fp`,
`rnd -> [sp,#40]`.  Every pick-first shape found allocates `agent -> r8` (spilled to `[sp,#24]`),
`systems -> sl`, `used -> r8` (reusing agent's register), `selected -> fp`, and keeps `rnd` in
`r4` (spilling it to `[sp,#40]` only for the sum-loop clobber).  The change is visible in the
first two prologue instructions:

```
original: 971c8: mov r8, r2 ; 971ca: mov r9, r1
nested:     10: mov sl, r2 ;   12: mov r8, r1
```

Because the greedy allocator assigns `r8` to the first of `{systems, agent}` it processes, and
the pick-first loop IR makes `agent` win, the whole tail inherits `systems -> sl`.  This is the
single root cause of the remaining 137 diff hunks in the `nested` + `galaxy_after_from` shape.

### 5.1 Sweeps that do not flip it

* **1440 declaration permutations** (720 orders of `{race, level, selected, type, guard, used}`
  x `int level;` / `int level = 0;`) on `nested` + `galaxy_after_from`: every one gives
  `(sys_r8=0, used_sl=0, frame96=1, diff_r6=1)`, size 1968 (720) / 1956 (480) / 1952 (240).
* **324 type/declaration variants** (`race` in `{uint32_t,int,unsigned}`, `level` in
  `{int level;, int level = 0; ...}`, `selected` in `{bool,int,char}`, `type` in
  `{int32_t,int,short}`, `guard` in `{int,uint32_t}`, `used` before/after `race`): no hit.
* **192 loop shapes** (`while/for/do/for(;;)` x guard position x `pick == 8` goto/continue x
  `pick == 10` nested/nested_after/and/and_cont x `pick == 12` goto/inline x latch): only the
  race-first `and`/`and_cont`/`twice` forms give `sys_r8=1, used_sl=1`, and they are all
  `pick10_first=0`.
* **Prologue aliases** (`Array<SolarSystem*> *sys = systems;`, `int agentStation =
  agent->getStation();`, `(void)systems;`, `(void)agent;`) and **`used` spellings**
  (`used != nullptr` vs `used`, direct `Globals::status->field_50`, renamed pointer): no change.
* **`forever` / `whilebreak`** loops with the nested form give `sys_r8=1` but fold the
  `pick == 12` select (`sel=0`) and lose `difficulty -> r6`.

## 6. `X4 + volatile int amount` — all features but pick-first

`X4` is the wave-90 race-first shape (`if (pick == 10 && race > 3) { type = 10; continue; }` +
`__builtin_expect(pick == 12, 0)`) with the `galaxy_after_from` tail.  Adding
`volatile int amount = 0;` gives every target feature:

```
size=1996(fast)/1984(linked)  sys_r8=1  used_sl=1  frame96=1  diff_r6=1  sel=1  pick10_first=0
```

but the official score is only **64.6 %** — the volatile anchor perturbs the tail far more than
the register match gains.  `volatile int itemId` and `volatile` on both are worse.

## 7. Official measurements (autojunk=False)

| variant | usize | pct |
|---|---:|---:|
| **HEAD** (`short pick`, folded select, `0.5000001f`) | **1996** | **73.6** |
| X4 (race-first, base tail) | 1996 | 70.4 |
| X4 + `galaxy_after_from` | 1996 | 68.0 |
| X4 + `galaxy_after_from` + `volatile int amount` | 1984 | 64.6 |
| nested + `galaxy_after_from` | 1968 | 61.5 |
| `none` + `nested_after` + `galaxy_after_from` | 1932 | 58.7 |
| nested + base tail | 1972 | 57.4 |
| nested + `galaxy_after_from` + `volatile int amount` | 1980 | 56.5 |
| HEAD + `int pick` | 1960 | 48.9 |

The score is dominated by the tail's stack-slot / register allocation, not by the loop.  HEAD's
`short pick` keeps `systems -> r8` and the tail close; any `int pick` shape that fixes the loop
also swaps `systems`/`used` and loses more in the tail than it gains in the loop.

## 8. Conclusion / remaining work

The loop is solved: `nested` + `galaxy_after_from` is five content differences from the original loop and
already has the original `difficulty -> r6` / `[sp,#24]` clamp.  The remaining task is to make the
greedy allocator assign `systems -> r8` / `used -> sl` / `rnd -> [sp,#40]` while keeping the
pick-first `pick == 10` IR.  Every source spelling tried either keeps the pick-first loop with
`systems -> sl`, or keeps `systems -> r8` with the race-first `pick == 10` code.  A byte-exact
shape must also keep the function at exactly 1996 B (the `0.5000001f` anchor is still present;
the real 4 bytes are the non-folded `pick == 12` select).

## 9. Reproduction

```sh
# one-file signature (fast, ~0.1 s)
python3 /dev/shm/w4104/sweep_p10b.py        # 13 pick==10 forms x 2 tails
python3 /dev/shm/w4104/sweep_prologue.py    # 1440 declaration permutations
python3 /dev/shm/w4104/sweep_vol.py         # volatile amount/itemId x loop shapes

# official per-function verifier
cp /dev/shm/w4104/nested_after.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w4104/score.py             # osize=1996 usize=1968 pct=61.5
```

Scratch helpers live in `/dev/shm/w4104` (`gen2.py`, `tailvar.py`, `fast.py`, `score.py`,
`ndiff.py`, `sweep_*.py`).
