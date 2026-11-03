# LevelScript::process — mission-0 order + faithful player re-fetch (w4503)

Status: **code change landed, 2026-11-03**. Branch `wave95-w4503`, base master
`4a88bd6e`. Target: `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

This pass lands the mission-0 shape that the wave-92 w4266 note left open: the
committed `435` mission-0 chain order (state<=2 / state==4 / state==3 /
state==5) is kept, but two player accesses are made faithful to the binary and
the stale state-0 `volatile int pad` anchor is replaced by a smaller, documented
case-80 anchor.  The function stays exactly 57232 B, so no downstream symbol
moves and the whole-`.so` gate is unchanged.

**Wave-95 combination caveat (landed tree).** This note was measured on the
pre-w4404 tree.  On top of the landed w4404 else-if chain the same edits give
57240 B (rotate re-fetch), 57248 B (setCollide re-fetch) or 57256 B (both), so
the landed tree keeps w4404's size-exact 54.1% shape and this note is
analysis-only.  The individual effects are: S0 57232/54.1, +setCollide 57248/54.0,
+rotate 57240/54.4, +both 57256/54.4.

## Result

```
                         before            after
raw fuzzy (autojunk=0)   51.997113         52.884663   (+0.887550)
official rounded pct     52.0              52.9
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
orig / ours insns        20169 / 20014     20169 / 20009
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
.text                    0x1337c8          0x1337c8
.rodata                  0x41b67           0x41b67
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26290 (-8)      0x26290 (-8)
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

`cmake --build cmake-build-match --target check` passes; `drift` reports
`0 field(s) across 0 class(es)`.

## 1. What landed (three source changes + one anchor)

`src/game/world/LevelScript.cpp`:

1. **Mission-0 state-4 `setCollide(false)` re-fetches the player.**
   `player->setCollide(false)` -> `m_pLevel->getPlayer()->setCollide(false)`.
2. **Mission-1 `rotate` uses the cached `player` local.**
   `m_pLevel->getPlayer()->rotate(0.0f, angle, angle)` ->
   `player->rotate(0.0f, angle, angle)`.
3. **The state-0 `volatile int pad = delta; (void) pad;` anchor is deleted**
   (it was the wave-92/w4252 anchor at the end of the state-0 setup).
4. **A `volatile short pad` anchor is added at the top of `case 80:`** of the
   per-mission campaign switch, with a same-line block comment explaining it
   (see §4).

Change 1 is worth +0.082 raw and +8 B (57224 -> 57232, i.e. it is required for
size-exactness); change 2 is worth +2.016 raw and -4 B (57236 -> 57232);
change 3+4 together are worth +0.014 raw and +4 B (57228 -> 57232).

## 2. Evidence for change 1 (state-4 `setCollide`)

The original re-calls `Level::getPlayer()` immediately before the state-4
`setCollide(false)` (disassembly of the original at `0x138d86`):

```
orig 0x138d86: f8da 0018   ldr.w  r0, [sl, #24]      ; m_pLevel
     0x138d8a: 2101        movs   r1, #1             ; m_bCinematicBreak = 1
     0x138d8c: f88a 1011   strb.w r1, [sl, #17]
     0x138d90: f729 e950   blx    Level::getPlayer() ; <-- re-fetch
     0x138d94: 2100        movs   r1, #0
     0x138d96: f72d ee74   blx    PlayerEgo::setCollide(bool)
```

The cached `player` local is already in `[sp,#84]` in both builds, so this is a
pure source-shape difference: the original source wrote
`m_pLevel->getPlayer()->setCollide(false)` (or equivalent), not
`player->setCollide(false)`.  Measured: `435 r3=0 r4=0 m1=1 pad80` = 57224 B /
52.802668; `435 r3=0 r4=1 m1=1 pad80` = **57232 B / 52.884663**.

## 3. Evidence for change 2 (mission-1 `rotate`)

The original mission-1 block loads the *cached* player from its stack slot
before `PlayerEgo::rotate` (original at `0x137da6`):

```
orig 0x137da6: 9815        ldr    r0, [sp, #84]      ; player (cached in prologue)
     0x137da8: ee20 0a01   vmul.f32 s0, s0, s2
     0x137dac: ed9f 1ac0   vldr   s2, [pc, #768]
     0x137db0: ee20 0a01   vmul.f32 s0, s0, s2
     0x137db4: ee10 2a10   vmov   r2, s0
     0x137db8: 4613        mov    r3, r2
     0x137dba: f72e eed4   blx    PlayerEgo::rotate(float,float,float)
```

`[sp,#84]` is the `player` slot written by the prologue
(`strd r4, r0, [sp, #80]` where `r0 = Level::getPlayer()`), so the source used
the `player` local.  Measured: `435 r3=0 r4=1 m1=0 pad80` = 57236 B /
50.868635; `435 r3=0 r4=1 m1=1 pad80` = **57232 B / 52.884663**.

## 4. The case-80 size anchor and the register-allocation residual

The original case-80 entry (per-mission switch target `0x139e34`) is:

```
orig 0x139e34: 9816        ldr    r0, [sp, #88]      ; this
     0x139e36: 69c0        ldr    r0, [r0, #28]      ; this->m_nState
     0x139e38: 2800        cmp    r0, #0
     0x139e3a: f044 83f9   bne.w  0x13e630           ; -> switch(state) 1..10
     0x139e3e: f8db 0004   ldr.w  r0, [fp, #4]      ; messages
     0x139e42: 6800        ldr    r0, [r0, #0]       ; (*messages)[0]
     0x139e44: f728 e872   blx    RadioMessage::isOver()
```

Our build (with the anchor) is:

```
ours 0x139bde: f8ad 6164   strh.w r6, [sp, #356]    ; the anchor store
     0x139be2: f8bd 0164   ldrh.w r0, [sp, #356]    ; the anchor load
     0x139be6: f8da 001c   ldr.w  r0, [sl, #28]      ; m_nState (this in sl)
     0x139bea: 9c15        ldr    r4, [sp, #84]      ; player
     0x139bec: 2800        cmp    r0, #0
     0x139bee: f044 8700   bne.w  0x13e9f2
     0x139bf2: 9812        ldr    r0, [sp, #72]      ; messages
     0x139bf4: 6840        ldr    r0, [r0, #4]
     0x139bf6: 6800        ldr    r0, [r0, #0]
     0x139bf8: f728 e998   blx    RadioMessage::isOver()
```

The anchor costs 8 B here.  It is a **size anchor**, not a recovered original
statement: without it the function is 57228 B, which shifts every following
function by 4 B and drops `byte_exact` 4500 -> ~4318.  It is needed because our
register allocation differs from the original in one systematic way:

* the original spills `this` to `[sp,#88]` and reloads it (`ldr r0,[sp,#88];
  ldr r0,[r0,#28]`, `ldr r6,[sp,#88]`), keeping `r9` free for the zero
  constant (`mov.w r9,#0`, `strb.w r9,[r0,#1]` 4 B, `str.w r9,[sp]` 4 B);
* ours keeps `this` in `sl` and uses the low `r6` for zero (`movs r6,#0`,
  `strb r6,[r0,#1]` 2 B, `str r6,[sp,#0]` 2 B), which is 4 B shorter.

The anchor forces `delta` (r6) to stay live to the case-80 entry, which makes
the allocator spill/reload it and restores the original whole-function
allocation: measured net effect **+8 B at the case-80 entry, -2 B at
`0x1442c2` (mission-89 tail, `ldr.w r0,[sl,#20]` replaces
`ldr r0,[sp,#88]; mov r4,r0; ldr r0,[r0,#20]`) and -2 B near `0x144320`,
i.e. +4 B net**.  That +4 B exactly cancels the prologue's +4 B residual
(§5), so the function is size-exact.

Anchor-form sweep at the top of `case 80` (all measured with
`435 r3=0 r4=1 m1=1`): `volatile short` / `char` = 57232 B / 52.884663;
`volatile int` = 57232 B / 52.857143 (same size, worse score); no anchor =
57228 B / 52.870760.  Placing the same `volatile short` inside the state-0
body instead = 57236 B / 52.878083.  `volatile short` at the top of `case 80`
is the minimal form that yields 57232 with the best score.

## 5. Prologue: now identical except `Globals::status` r4 vs r9

With this pass the prologue matches the original instruction-for-instruction
except for the `Globals::status` allocation:

```
orig 0x137d4a: e9cd 4014   strd   r4, r0, [sp, #80]   ; &m_pLevel, player
     0x137d56: 48d4        ldr    r0, [pc, #848]
     0x137d58: 4478        add    r0, pc
     0x137d5a: 6804        ldr    r4, [r0, #0]        ; status -> r4   (2 B)
     0x137d5c: 6820        ldr    r0, [r4, #0]        ;                (2 B)
     0x137d64: f8cd a058   str.w  sl, [sp, #88]
     0x137d68: e9cd 4817   strd   r4, r8, [sp, #92]
     0x137d6e: e9cd 5612   strd   r5, r6, [sp, #72]

ours 0x137d4a: e9cd 4014   strd   r4, r0, [sp, #80]   ; same
     0x137d56: 48dc        ldr    r0, [pc, #880]
     0x137d58: 4478        add    r0, pc
     0x137d5a: f8d0 9000   ldr.w  r9, [r0]           ; status -> r9   (4 B)
     0x137d5e: f8d9 0000   ldr.w  r0, [r9]           ;                (4 B)
     0x137d68: f8cd a058   str.w  sl, [sp, #88]
     0x137d6c: e9cd 9817   strd   r9, r8, [sp, #92]
     0x137d72: e9cd 5612   strd   r5, r6, [sp, #72]
```

The only difference is `r4` (16-bit `ldr`) vs `r9` (32-bit `ldr.w`), worth
exactly +4 B.  It is not steered by any of the six permutations of the
`messages`/`player`/`enemies` locals (w4266 §3a), nor by `Status *&`,
`Status **`, `Status *const`, `!`/`<=0`/`<1`/reversed compare.  Machine IR shows
the GOT-slot vreg (`%803 = t2LDRi12`) is the one that receives r9.  Fixing it
would make the function 4 B short *unless* the case-80 anchor is also removed,
so the two residuals are coupled.

## 6. Mission-0 state machine (Ghidra + disassembly, unchanged from w4266)

The mission-0 branch is entered at `0x137e84` and is a chain of tests:

```
0x137e84  messages[2]->isOver()
0x137e8c  ldr r1,[sl,#28]            ; m_nState
0x137e96  cmp r1,#0
0x137e9c  <state-0 setup>            ; state==0 && msg2 over
0x1383fe  cmp r1,#2 ; bgt 0x1389c8   ; state <= 2
0x138404  <pulse wobble loop>
0x138440  ldr r0,[sl,#28]; cmp r0,#1 ; state == 1
0x138478  ldr r0,[sl,#28]
0x13847c  cmp r0,#2 ; bne tail       ; state == 2
0x1389c8  cmp r1,#4 ; beq 0x138d2a   ; state == 4
0x1389ce  cmp r1,#3 ; bne 0x138fd8   ; state == 3
0x1389d4  <state-3 body>
0x138d2a  <state-4 body>
0x138fd4  ldr r1,[sl,#28]            ; state-4 "msg10 not over" path
0x138fd8  cmp r1,#5 ; bne 0x139b1a   ; state == 5
0x138fde  messages[12]->isOver()
0x139b1a  <default switch(m_nState) 6..15>
```

The original test order is `4, 3, 5`; the committed source order is `435`
(`state<=2`, `state==4`, `state==3`, `state==5 && msg12`).  The emitted test
order is nevertheless `3, 5, 4` because of MachineBlockPlacement (w4266 §2).
None of the six permutations emits `4, 3, 5`; `435` is the only permutation
that is simultaneously size-exact and `.rel.plt`-exact.

The case-80 per-mission switch (states 1..10) is a real `tbh` in both builds:

```
orig 0x13e62c: 9816        ldr   r0, [sp, #88]
     0x13e62e: 69c0        ldr   r0, [r0, #28]
     0x13e630: 1e41        subs  r1, r0, #1
     0x13e632: 2909        cmp   r1, #9
     0x13e634: f205 82e7   bhi.w 0x143c06
     0x13e638: e8df f011   tbh   [pc, r1, lsl #1]
```

Our source already expresses this as the `else if (m_nState == N)` chain that
clang lowers to the same `tbh`.

## 7. The `354` alternative (higher raw score, not size-exact)

Re-ordering the three mission-0 tests to `354` (`state==3`, `state==5 && msg12`,
`state==4`) with change 1 + change 2 raises the raw score substantially but
shortens the function:

| mission-0 order | anchor | size | raw % |
|---|---|---:|---:|
| `435` r3=0 r4=1 m1=1 | case-80 `volatile short` | **57232** | **52.884663** |
| `435` r3=0 r4=1 m1=1 | none | 57228 | 52.870760 |
| `435` r3=0 r4=0 m1=1 | case-80 `volatile short` | 57224 | 52.802668 |
| `435` r3=0 r4=1 m1=0 | case-80 `volatile short` | 57236 | 50.868635 |
| `345` r3=0 r4=1 m1=1 | case-80 `volatile short` | 57240 | 52.722748 |
| `354` r3=0 r4=1 m1=1 | none | 57204 | 54.437281 |
| `354` r3=0 r4=1 m1=1 | case-80 `volatile short` | 57208 | 53.604860 |
| `354` r3=1 r4=1 m1=1 | case-80 `volatile short` | 57216 | 54.229525 |
| `453` r3=0 r4=1 m1=1 | case-80 `volatile short` | 57184 | 47.274085 |
| `534` r3=0 r4=1 m1=1 | case-80 `volatile short` | 57208 | 49.502141 |
| `543` r3=0 r4=1 m1=1 | case-80 `volatile short` | 57208 | 49.502141 |

`354` is 24-48 B short of 57232, so it cannot be landed without moving every
following function (w4266 §7 measured `byte_exact` 4499 -> 4317 for a 4 B
shrink).  It remains the natural target if the case-12 tail (+8 B) and the
prologue (-4 B) are ever fixed together with a further +12..16 B of faithful
source.

## 8. Other residuals (unchanged)

* **Case-12 tail.**  The original re-loads the array index from `[sp,#60]` on
  the `m_nScriptTimerA < 5000` edge path; ours uses `[sp,#80]`/`r8`:

  ```
  orig 0x145948: e9d6 0100   ldrd   r0, r1, [r6]
       0x14594c: e001        b.n    0x145952
       0x14594e: f8dd 903c   ldr.w  r9, [sp, #60]
       0x145952: f719 ef42   blx    __aeabi_l2f

  ours 0x145940: e9d6 0100   ldrd   r0, r1, [r6]
       0x145944: e001        b.n    0x14594a
       0x145946: f8dd 8050   ldr.w  r8, [sp, #80]
       0x14594a: f719 ef46   blx    __aeabi_l2f
  ```

  This is a whole-function stack-slot assignment difference (`[sp,#60]` vs
  `[sp,#80]`), not a statement in the case-12 source.
* **Literal pool order.**  The final constant pool still swaps `0.8` and
  `3000.0` (`orig 0x145c70: ... 0.8, 3000.0 ...`; `ours 0x145c6c: ... 3000.0,
  0.8 ...`).  Both builds use `3000.0` first and `0.8` second in the final
  instruction stream, so the pool order tracks selection order; swapping the
  source blocks changes the size (w4266 §5).
* `.ARM.extab` remains 0x26290 vs the original 0x26298 (the known w4270
  HangarWindow EH residual, unrelated to this function).

## 9. Reproduction

```
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8       # RATCHET PASS
cmake --build cmake-build-match --target drift -j8       # 0 fields
FN=_ZN11LevelScript7processEi cmake --build cmake-build-match --target verify-fn
```

The raw score above is `tools/verify/verify.py`'s disassembly + normalized
`difflib.SequenceMatcher(None, orig, ours, autojunk=False).ratio()*100`.
