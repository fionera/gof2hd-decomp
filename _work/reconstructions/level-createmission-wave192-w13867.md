# Level::createMission — wave 192 w13867 (second angle)

Target: `_ZN5Level13createMissionEv` (`0xaffd8`, 20,116 B). Base master `f7153965e`
(raw 4,265 / aligned 386 / size 17,172 exact). `.rel.plt` byte-identical in every
variant below. Sibling w13858 works the same function from the alien-turret /
case-4 hunter cluster; this note covers the *other* clusters.

## Residual cluster map (base f7153965e, `cluster.py 12`)

```
w=238 b0516  (register/stack-slot cascade, see below)
w=221 b34dc  (fortress loop tail)
w=199 b3752  (raid block; w13858 dependency)
w=131 b1418
w= 97 b0dca
w= 96 b4152  (shared tail r8<->sl swap)
w= 57 b3274  (alien turret; w13858)
w= 54 b1588
w= 54 b3a5c
w= 40 b2cf4
w= 37 b358c
w= 36 b0458
w= 34 b2a4c
w= 30 b0d2e
w= 25 b26ec
w= 20 b025c
w= 19 b0190
w= 18 b1888
w= 17 b2e8a
w= 16 b3148
w= 16 b3be6
w= 16 b3dda
w= 15 b36aa
```

## Finding: b0516 is an entry-register allocation cascade

Traced with `regdetail.py 493 560` plus raw disasm of O/U indices 0–200 and 425–475.

Root at function entry (O/U indices 10–13):

| | original | ours |
|---|---|---|
| `Globals::globals` | `ldr.w fp,[r0]` -> **fp** | `ldr r6,[r0]` -> **r6** |
| `Globals::status` | `ldr r5,[r1,#0]` -> **r5** | `ldr.w r8,[r1]` -> **r8** |

Resulting main-body stack-slot map (after the alien-orbit loop at b0194):

| slot | original | ours |
|---|---|---|
| `[sp,#84]` | `raid` bool | unused here |
| `[sp,#104]` | `stationIndex` | **status ptr** (lazy spill at b01bc) |
| `[sp,#108]` | **status ptr** (stored b0022, re-stored b00da) | **`security` int** |
| `[sp,#116]` | `introSystem` bool | `introSystem` (same) |
| `[sp,#124]` | `this` | `this` (same) |
| `[sp,#88]` | globals ptr | globals ptr (same) |

Original keeps `security` in callee-saved **r5** (`sub.w r5,r4,r0`) and spills `raid`
immediately; ours spills `security` to `[sp,#108]` and holds `raid` in **r8**
(`mov.w r8,#0` x2, `movlt.w r8,#1`). `[sp,#108]` is *coloured*: `count` during the
alien-orbit loop (b00da `str r6,[sp,#108]`), then `status` in the main body;
`[sp,#116]` is rnd during that loop, then `introSystem`. Literal pool sits 16 B later
in ours (b036c -> b037c), a consequence of the extra instructions, not a separate cause.

=> b0516 is not independently patchable; it needs the *entry* register assignment
(status->r5, globals->fp). Treat as low probability.

## Exact original code for the unowned blocks

### Fortress loop (b33da–b34e8)
`this` in **r4** at block entry (`ldr.w r2,[r4,#248]`), `r5=[sp,#124]` (this reloaded),
loop counter reuses **r4** with `str r4,[sp,#116]` / `ldr r4,[sp,#116]` around the body,
`mov.w r1,#1000` at b3424 (after `str.w r0,[r1,r4,lsl#2]`), `adds r6,#24` at b34ca,
`ldr r4,[sp,#116]` at b34cc, then b34de `ldr r4,[sp,#124]`,
b34e0 `ldr.w r8,[sp,#108]` (status->r8), b34e4 `ldr.w sl,[sp,#52]` (pirates),
b34e8 `b b3756`.
Ours: `this` in **r8** throughout, index in r8 (`add.w r8,r8,#1` at b34cc),
`mov.w r1,#1000` hoisted to b3418, and an **extra `ldr r6,[sp,#100]` (rnd) at b34e6**
the original lacks; ours b34e4 `ldr r4,[sp,#108]` (status->r4 not r8).

### extraAliens init (b3dd0–b3dfa)
```
b3dd0 mov.w r8,#0
b3dd4 mov.w r9,#1
b3dd8 ldmia r3,{r0,r1,r2,r3}
b3dda ldr r6,[sp,#48]   ; pirateEscorts
b3ddc ldr r4,[sp,#92]   ; stationPirates
b3dde add r6,r4
b3de0 ldr r4,[sp,#20]   ; guns
b3de2 add.w ip,r6,r4    ; start
b3de6 ldr r6,[sp,#44]   ; pirates
b3de8 add.w r4,ip,r6
b3dec ldr r6,[sp,#32]   ; extraAliens
b3dee add r4,r6         ; outer bound
b3df0 add r6,ip
b3df2 add r6,r0 ; b3df4 add r6,r1 ; b3df6 add r6,r2 ; b3df8 add r6,r3
b3dfa b.n b3e3e
```
The `ldmia r3,{r0,r1,r2,r3}` is the four contiguous `Level` int fields
`friendCount/field_164/field_168/hostileCount`. Ours hoists `r8 = extraAliens`,
uses a different add order, emits a trailing `nop` and shifts the literal pool.
Source ~lines 2061–2075.

### Tail block b4152 (w=96)
orig `str.w r5,[r8,#248]`, `ldr r0,[pc,#392]`, `mov.w fp,#1`, `mov.w r9,#0`,
`add r0,pc`, `ldr.w sl,[r0]` (sl = cached globals), index r9, this r8.
Ours: `str.w r5,[sl,#248]`, `ldr r0,[pc,#396]`, `mov.w fp,#1`, `mov.w r8,#0`,
`add r0,pc`, `ldr.w r9,[r0]`, index r8, this sl. Pure r8<->sl swap + 1 extra insn;
downstream of the this-in-r8 choice.

## Other original stack slots
`[sp,#100]`=Globals::rnd, `[sp,#68]`=raidRace, `[sp,#84]`=raid, `[sp,#52]`=pirates,
`[sp,#40]`=sum term paired with pirates, `[sp,#56]`=pirateFortress flag,
`[sp,#32]`=hostileCount/extraAliens, `[sp,#112]`=raidType temp,
`[sp,#116]`=spawnPosition addr / loop-index temp, `[sp,#92]`=start index,
`[sp,#96]`=rnd vtable fn ptr, `[sp,#124]`=Level*. Original does **not** keep `this`
in a callee-saved reg across blocks — it reloads from `[sp,#124]` at every block
boundary; ours keeps `this` in r8 (root divergence). Stack canary present in both
(`__stack_chk_fail` at b01ae).

## Experiments tried and reverted (do not repeat)
1. Fortress `Vector p`/`Vector q` removed -> raw 16,648 aligned 2,000 size 17,308 ✗ (+136).
2. Braced `{Vector p…}` / `{Vector q…}` scopes -> same 16,648/2,000/17,308 ✗.
3. Any p/q restructuring reshuffles whole-function register allocation.
4. (w13849) extraAliens `start` reorder -> neutral.
5. (w13773) raid `int guns` moved after `if` -> size 17,164 FAIL; `Vector p` removed ->
   17,312 FAIL; plain ternary 7 spellings -> 17,156 FAIL.
6. (w13849 landed) mission-table select `(bigAlienWave | (i < 0))` + W hunks:
   raw 3,373->4,265, aligned 463->386.
