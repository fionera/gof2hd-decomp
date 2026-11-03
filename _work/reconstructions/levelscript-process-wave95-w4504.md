# LevelScript::process — second attack: prologue register, stack slots, rotate site (w4504)

Status: **analysis-only in the landed tree** (the arm swap is superseded by
w4702's 54.8% shape; on top of it the same swap gives 57240 B / 54.6%).
Branch `wave95-w4504`, base master `4a88bd6e`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Committed change: mission-0 arms reordered `4,3,5 -> 3,4,5` (source order
`345`) plus the mission-125 `state>=4` separator changed from a separate `if`
to `else if`.  Raw fuzzy **51.997113 -> 52.030457** (`autojunk=False`), size
stays exactly `0xdf90`, `.rel.plt` stays byte-identical.

```
gate before (HEAD 4a88bd6e)          gate after
byte_exact       4500                4500
linked_exact     4500                4500
size-exact       4524/4524           4524/4524
stub_zero_size   0                   0
missing/wrong/extra 0/0/0            0/0/0
data_exact       843/843             843/843
imports          207                 207
lint             CLEAN               CLEAN
drift            0                   0
RATCHET          PASS                PASS
```

All measurements are the official verifier
(`tools/verify/verify.py`, `difflib.SequenceMatcher(..., autojunk=False)`),
not a local approximation.

---

## 1. Prologue: `Globals::status` is in r4 in the original, r9 in ours

Original, `0x137d18`..`0x137d7c`:

```
137d18  b5f0        push  {r4, r5, r6, r7, lr}
137d1a  af03        add   r7, sp, #12
137d1c  e92d 0f80   stmdb sp!, {r7, r8, r9, sl, fp}
137d20  ed2d 8b0c   vpush {d8-d13}
137d24  b0ec        sub   sp, #432
137d26  4682        mov   sl, r0                  ; this
137d28  48de        ldr   r0, [pc, #888]
137d2a  4654        mov   r4, sl
137d2c  460e        mov   r6, r1                  ; delta
137d2e  4478        add   r0, pc
137d30  f8d0 b000   ldr.w fp, [r0]              ; __stack_chk_guard value
137d34  f8db 0000   ldr.w r0, [fp]
137d38  906b        str   r0, [sp, #428]
137d3a  f854 0f18   ldr.w r0, [r4, #24]!        ; r4 = &m_pLevel
137d3e  ...         blx   Level::getMessages
137d42  4605        mov   r5, r0                  ; messages
137d44  6820        ldr   r0, [r4, #0]
137d46  ...         blx   Level::getPlayer
137d4a  e9cd 4014   strd  r4, r0, [sp, #80]      ; &m_pLevel=80, player=84
137d4e  6820        ldr   r0, [r4, #0]
137d50  ...         blx   Level::getEnemies
137d54  4680        mov   r8, r0                  ; enemies
137d56  48d4        ldr   r0, [pc, #848]
137d58  4478        add   r0, pc
137d5a  6804        ldr   r4, [r0, #0]            ; r4 = Globals::status   <-- 2 B
137d5c  6820        ldr   r0, [r4, #0]            ; status->vtable          <-- 2 B
137d5e  ...         blx   Status::getCurrentCampaignMission
137d62  2800        cmp   r0, #0
137d64  f8cd a058   str.w sl, [sp, #88]          ; this=88
137d68  e9cd 4817   strd  r4, r8, [sp, #92]      ; status=92, enemies=96
137d6c  4640        mov   r0, r8
137d6e  e9cd 5612   strd  r5, r6, [sp, #72]      ; messages=72, delta=76
137d72  f000 8087   beq.w 137e84
137d76  6820        ldr   r0, [r4, #0]            ; second mission test, still r4
137d78  ...         blx   Status::getCurrentCampaignMission
```

Ours (HEAD build), same window:

```
137d18  b5f0        push  {r4, r5, r6, r7, lr}
...
137d3a  f854 4f18   ldr.w r4, [r4, #24]!        ; identical
...
137d4a  e9cd 4013   strd  r4, r0, [sp, #76]      ; &m_pLevel=76, player=80
...
137d56  48dd        ldr   r0, [pc, #884]
137d58  4478        add   r0, pc
137d5a  f8d0 9000   ldr.w r9, [r0]              ; r9 = Globals::status   <-- 4 B
137d5e  f8d9 0000   ldr.w r0, [r9]              ; status->vtable          <-- 4 B
137d62  ...         blx   Status::getCurrentCampaignMission
137d66  2800        cmp   r0, #0
137d68  f8cd a058   str.w sl, [sp, #88]
137d6c  9615        str   r6, [sp, #84]            ; delta=84
137d6e  4640        mov   r0, r8
137d70  e9cd 9817   strd  r9, r8, [sp, #92]      ; status=92, enemies=96
137d74  9512        str   r5, [sp, #72]            ; messages=72
137d76  f000 8088   beq.w 137e8a
137d7a  f8d9 0000   ldr.w r0, [r9]
137d7e  ...         blx   Status::getCurrentCampaignMission
```

**What is actually different.**  The *shape* is identical: both load
`Globals::status` once, keep the pointer in a callee-saved register across the
two `getCurrentCampaignMission()` calls, and spill it to `sp+92`.  The only
difference is the register: original `r4`, ours `r9`.  Because `r4` is a low
register the two instructions are 2 B each; `r9` forces the 4 B `.w` forms,
which is the +4 B that shows up in the size delta of the whole prologue.

`r4` is dead in *both* builds at the point of the status load: it holds
`&m_pLevel` from `0x137d3a` and dies at the `getEnemies` argument load
(`ldr r0,[r4,#0]`, orig `0x137d4e`, ours `0x137d4e`).  So this is not a
live-range conflict; it is purely RAGreedy's choice among free callee-saved
registers.  The MIR (post-RA, `-mllvm -print-machineinstrs`) confirms the
interval is short and has no copy hint:

```
renamable $r0 = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
renamable $r9 = t2LDRi12 killed renamable $r0, 0 :: (load 4 from got)
renamable $r0 = t2LDRi12 renamable $r9, 0 :: (dereferenceable load 4 from @_ZN7Globals6statusE)
tBL ... @_ZN6Status25getCurrentCampaignMissionEv
```

The register is chosen globally (allocation order / priority queue), so it
cannot be steered by a local source edit; it tracks the whole-function live
ranges, which are still ~48% different.  **Not fixed by this pass.**

## 2. The "mission-1 rotate argument slot swap" is really the `player` stack slot

Original, mission-1 rotate site (`0x137d90`..`0x137dba`):

```
137d90  f06f 000f   mvn.w  r0, #15
137d94  ed9f 1ac5   vldr   s2, [pc, #788]
137d98  ea00 0046   and.w  r0, r0, r6, lsl #1
137d9c  2100        movs   r1, #0                 ; arg1 = 0.0f
137d9e  ee00 0a10   vmov   s0, r0
137da2  eeb8 0ac0   vcvt.f32.s32 s0, s0
137da6  9815        ldr    r0, [sp, #84]          ; player  <-- sp+84
137da8  ee20 0a01   vmul.f32 s0, s0, s2
137dac  ed9f 1ac0   vldr   s2, [pc, #768]
137db0  ee20 0a01   vmul.f32 s0, s0, s2
137db4  ee10 2a10   vmov   r2, s0                  ; arg2 = angle
137db8  4613        mov    r3, r2                  ; arg3 = angle
137dba  ...         blx    PlayerEgo::rotate
```

Ours (HEAD build), same site:

```
137d96  f06f 000f   mvn.w  r0, #15
137d9a  ed9f 1acd   vldr   s2, [pc, #820]
137d9e  ea00 0046   and.w  r0, r0, r6, lsl #1
137da2  2100        movs   r1, #0                 ; arg1 = 0.0f
137da4  ee00 0a10   vmov   s0, r0
137da8  eeb8 0ac0   vcvt.f32.s32 s0, s0
137dac  9814        ldr    r0, [sp, #80]          ; player  <-- sp+80
137dae  ee20 0a01   vmul.f32 s0, s0, s2
137db2  ed9f 1ac8   vldr   s2, [pc, #800]
137db6  ee20 0a01   vmul.f32 s0, s0, s2
137dba  ee10 2a10   vmov   r2, s0                  ; arg2 = angle
137dbe  4613        mov    r3, r2                  ; arg3 = angle
137dc0  ...         blx    PlayerEgo::rotate
```

**The argument order is already correct** (`r1=0.0f`, `r2=r3=angle`) in both.
The visible difference is the *stack slot* the `player` local lives in:
original `sp+84`, ours `sp+80`.  The source already writes
`m_pLevel->getPlayer()->rotate(...)`, but clang CSEs that call with the
prologue `getPlayer()` and emits a plain stack load, so there is no extra call
here in either build.

### 2.1 The three-slot permutation

Reading the prologue stores (and confirmed by counting `[sp,#N]` operands over
the whole function):

| local      | original | ours (HEAD) |
|------------|----------|-------------|
| `messages` | sp+72    | sp+72       |
| `delta`    | **sp+76**| **sp+84**   |
| `&m_pLevel`| **sp+80**| **sp+76**   |
| `player`   | **sp+84**| **sp+80**   |
| `this`     | sp+88    | sp+88       |
| `status`   | sp+92    | sp+92       |
| `enemies`  | sp+96    | sp+96       |

Reference counts (`objdump` of the whole function):

```
orig: [sp,#72]=62  [sp,#76]=171 [sp,#80]=97  [sp,#84]=163 [sp,#88]=328 [sp,#92]=48  [sp,#96]=167
ours: [sp,#72]=113 [sp,#76]=84  [sp,#80]=160 [sp,#84]=182 [sp,#88]=194 [sp,#92]=25  [sp,#96]=179
```

So the original colour order (lowest offset first) for the three locals is
`delta < &m_pLevel < player`; ours is `&m_pLevel < player < delta`.  It is a
3-cycle, not a single bad statement.

Mechanism: LLVM 7 `StackSlotColoring` sorts the pre-coloured slots by spill
weight (`(isDef+isUse) * blockFreq / entryFreq`, summed over every use) and
hands out colours in that order; PEI then lays the surviving stack objects out
in colour order, so colour 0 -> `sp+72`, 1 -> `sp+76`, 2 -> `sp+80`, 3 ->
`sp+84`, ... .  Pre-colouring slot ids in the MIR are
`this=677, enemies=678, statusGOT=679, player=680, delta=681, m_pLevel=682,
messages=683`.  The weight order is therefore what decides the permutation,
and it is driven by **block frequencies**, i.e. by the branch probabilities of
the whole 57 KB function.

Evidence that this is weight-driven and not a source-order artefact: moving
the 8-byte `volatile int pad = delta;` anchor out of the mission-0 state-0
block (or deleting it) re-colours all three slots.  Measured permutations:

| build                                   | `player` | `delta` | `&m_pLevel` | size |
|-----------------------------------------|----------|---------|-------------|------|
| original                                | 84       | 76      | 80          | df90 |
| HEAD (pad in state-0)                   | 80       | 84      | 76          | df90 |
| HEAD minus pad                          | 76       | 80      | 84          | df88 |
| HEAD with `forceUpdate(30)` -> `(delta)`| 76       | 84      | 80          | df90 |

The last row is the only shape found that puts `&m_pLevel` back at `sp+80`
(original), but it swaps `player`/`delta` the other way and scores *lower*
(51.990644).  `forceUpdate(30,false)` is also what the binary says
(`0x137f6a movs r1,#30`), so that row is not faithful anyway.

**Not fixed by this pass**; it needs the branch probabilities of the whole
function to match, which is the same problem as the remaining 48% of the body.

## 3. `getPlayer()` call count: original 23, ours 21

`blx _ZN5Level9getPlayerEv` occurrences in the function body:

```
orig 23   ours 21
```

The two missing calls are exactly the mission-0 `setCollide` sites, which the
original re-fetches instead of using the `player` local:

```
orig 138a34  f8da 0018   ldr.w r0, [sl, #24]     ; m_pLevel
orig 138a38  ...         blx   Level::getPlayer
orig 138a3c  2101        movs  r1, #1
orig 138a3e  ...         blx   PlayerEgo::setCollide

ours 138a2e  4628        mov   r0, r5             ; r5 = cached player
ours 138a30  ...         blx   PlayerEgo::setCollide
```

and the state-4 counterpart:

```
orig 138d86  f8da 0018   ldr.w r0, [sl, #24]
orig 138d90  ...         blx   Level::getPlayer
orig 138d94  2100        movs  r1, #0
orig 138d96  ...         blx   PlayerEgo::setCollide

ours 1390c6  4630        mov   r0, r6
ours 1390c8  ...         blx   PlayerEgo::setCollide
```

Re-fetching both (`m_pLevel->getPlayer()->setCollide(...)`) restores the call
count to 23, but each site costs +4 B (the `ldr`+`blx` replaces a `mov`), so
the function becomes `0xdf98` unless the 8-byte `pad` anchor is removed at the
same time.  Every size-exact combination of that change was measured; the best
is **51.445633** (and the `else if` shapes that score 53-54% with this change
are `.rel.plt`-dirty, see §5).  Not landed.

## 4. Hard constraints on any shape change

1. **Size must stay exactly `0xdf90`.**  `process` is in the middle of
   `.text`; a different size shifts every later function and the ratchet's
   `byte_exact` collapses (measured: `0xdf88` -> 4496, `0xdffc` -> 3688).
2. **`.rel.plt` first-reference order must not change.**  Gold emits PLT
   relocations in input-relocation order, which for this TU is instruction
   order inside `process`.  The original order is

   ```
   StarSystem::switchPlanetForIntro
   Level::switchSkyboxForIntro
   ParticleSystemManager::reset      (x2)
   PlayerEgo::startSmokeEmission
   PlayerStation::setVisible
   PlayerEgo::deleteHackingGame
   ```

   Any shape that lays the mission-125 `deleteHackingGame` block out before
   the mission-0 state-3/4 block rotates six `.rel.plt` entries by 12 bytes,
   which moves every PLT stub and drops `byte_exact` 4500 -> 3688
   (`MGame::successCheck/switchCamera/dialogueEvent/gameOverCheck` are the
   first casualties).  This is why the highest-scoring shapes (53-54%) are
   unusable.

## 5. Variant sweep (all official verifier, size / `.rel.plt` checked)

Mission-0 arm order is a permutation of the source arms `state==4`,
`state==3`, `state==5`; the mission-125 knobs are the four `if`/`else if`
separators (S0 = state0|state1, S1 = state1|state2, S2 = state2|state3,
S3 = state3|state>=4).  Committed HEAD is order `435`, bits `(1,0,0,0)`.

| variant (order, m125 bits, other knobs)   | size  | raw %     | rel.plt |
|-------------------------------------------|-------|-----------|---------|
| HEAD: 435, bits (1,0,0,0)                 | df90  | 51.997113 | ok      |
| **345, bits (1,0,0,1)  (landed)**         | df90  | **52.030457** | ok  |
| 345, bits (1,1,0,1)                       | df90  | 52.030457 | ok      |
| 345, bits (1,0,0,0), pad removed          | df88  | 51.752115 | ok      |
| 345, bits (0,0,0,1)                       | df90  | 51.863462 | ok      |
| 345, bits (0,1,0,1)                       | df90  | 51.863462 | ok      |
| 345, bits (1,0,0,1) + `player->rotate`    | df90  | 51.649828 | ok      |
| 345, bits (0,0,0,1) + re-fetch + no pad   | df90  | 51.445633 | ok      |
| 345, bits (1,0,0,1) + `forceUpdate(delta)`| df90  | 51.990644 | ok      |
| 435, bits (1,0,0,1) + re-fetch + no pad   | df90  | 51.019037 | ok      |
| 345, bits (0,0,1,1) + rotate + re-fetch   | df90  | 53.745020 | **12**  |
| 435, bits (1,0,1,1) + rotate + re-fetch   | df90  | 54.060516 | **12**  |
| 435, bits (1,0,1,1) + rotate, no anchor   | df90  | 54.089460 | **12**  |

(`re-fetch` = both mission-0 `setCollide` sites changed back to
`m_pLevel->getPlayer()->setCollide(...)`; `rotate` = the mission-1 site
changed to `player->rotate(...)`; `no pad` = the 8-byte
`volatile int pad = delta;` anchor removed, which is what keeps those rows
size-exact.  `rotate` is -8 B and `re-fetch` is +8 B, so the pair cancels and
the `pad` anchor stays.)

The 53-54% shapes are the ones that put `deleteHackingGame` first in the
instruction stream.

## 6. What was landed

`src/game/world/LevelScript.cpp`:

1. mission-0 arms reordered `state<=2 / state==4 / state==3 / state==5`
   (HEAD) -> `state<=2 / state==3 / state==4 / state==5` (order `345`);
2. the mission-125 separator between the `state==3` arm and the
   `state>=4 && state<=6` arm changed from a separate `if` to `else if`.

Both are source-shape-only (no anchors added, no `volatile` added, nothing
removed).  The size stays `0xdf90`, `.rel.plt` is byte-identical, and the
official raw fuzzy rises `51.997113 -> 52.030457`.

## 7. What remains

* `Globals::status` r4/r9: purely a global RAGreedy choice; needs the whole
  function's live ranges to match.
* `delta`/`&m_pLevel`/`player` stack-slot 3-cycle: driven by
  `StackSlotColoring` spill weights, i.e. by block frequencies of the whole
  function.  Local anchors move the permutation but not towards the original
  (measured table in §2.1).
* The two mission-0 `setCollide` sites should re-fetch
  `m_pLevel->getPlayer()` to match the original's 23 calls, but that is only
  affordable if 8 bytes are found elsewhere (the `pad` anchor is the current
  8-byte filler).
* The 53-54% block layouts are real improvements that are blocked only by the
  `.rel.plt` first-reference order; matching the original's mission-0 vs
  mission-125 block placement would unlock them.
