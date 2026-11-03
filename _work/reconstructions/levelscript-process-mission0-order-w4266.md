# LevelScript::process — mission-0 state-machine source order (w4266)

Status: **note-only pass, 2026-11-03**. Branch `wave92-w4266`, base master
`b379d00d`.  No code change landed: the best source shape that reproduces the
original mission-0 block order is **4 bytes short** (57228 vs 57232), and
`LevelScript::process` sits in the middle of `.text`, so a 4-byte shrink shifts
every following function and drops `byte_exact` 4499 -> 4317 (measured).  The
committed pass-4 source is kept.

Target: `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**, 20169 instructions.  Committed baseline:
raw fuzzy **51.997113** (autojunk=False), size-exact.

```
gate before (HEAD bcbbed79)          gate after (unchanged)
byte_exact       4499                4499
linked_exact     4499                4499
size-exact       4524/4524           4524/4524
stub_zero_size   0                   0
missing/wrong/extra 0/0/0            0/0/0
data_exact       843/843             843/843
imports          207                 207
lint             CLEAN               CLEAN
drift            0                   0
RATCHET          PASS                PASS
```

## 1. The original mission-0 state machine (disassembly)

The mission-0 branch is entered at `0x137e84` (`beq.w` from the top-level
mission dispatch at `0x137d72`).  Its structure, read straight off the Thumb
disassembly (`../_work/bins/android_2.0.16_libgof2hdaa.so`), is a **chain of
tests**, not a switch:

```
0x137e84  messages[2]->isOver()
0x137e8c  ldr r1,[sl,#28]            ; m_nState
0x137e96  cmp r1,#0
0x137e9c  <state-0 setup>            ; state==0 && msg2 over
0x137e... falls to 0x1383fe
0x1383fe  cmp r1,#2 ; bgt 0x1389c8   ; state <= 2
0x138404  <pulse wobble loop>
0x138440  ldr r0,[sl,#28]; cmp r0,#1 ; state == 1
0x138478  ldr r0,[sl,#28]
0x13847c  cmp r0,#2 ; bne tail       ; state == 2
0x138538  b tail
0x1389c8  cmp r1,#4 ; beq 0x138d2a   ; state == 4
0x1389ce  cmp r1,#3 ; bne 0x138fd8   ; state == 3
0x1389d4  <state-3 body>
0x138d2a  <state-4 body>
0x138fd4  ldr r1,[sl,#28]            ; state-4 "msg10 not over" path
0x138fd8  cmp r1,#5 ; bne 0x139b1a   ; state == 5
0x138fde  messages[12]->isOver()
0x139b1a  <default switch(m_nState)>
```

So the **original test order is `4, 3, 5`** and the source order implied by the
chain is `state<=2 / state==4 / state==3 / state==5` — exactly the committed
source order (`435`).  The *generated* test order nevertheless differs because
of MachineBlockPlacement (see §3).

## 2. The 354 source order

`_work/reconstructions/levelscript-process-blockorder-w4252.md` landed the
mission-125 independent-`if` shape and removed the 99-store volatile anchor,
giving 51.997113 / 57232 B.  The mission-0 chain in that source is still
`435` (`state<=2`, `state==4`, `state==3`, `state==5 && msg12`).

Re-ordering **only the three state-3/4/5 tests** to source order `354`
(`state==3`, `state==5 && msg12`, `state==4`) changes the emitted test order and
the block layout and raises the raw score by ~1.9 points, but changes the
function size:

| variant (mission-0 chain order) | size | raw % |
|---|---:|---:|
| `435` committed (+ volatile pad) | **57232** | **51.997113** |
| `354` + state-4 `getPlayer()` re-fetch, pad removed | 57228 | **53.913043** |
| `354` + state-3 `getPlayer()` re-fetch, pad removed | 57228 | 53.886785 |
| `354` base, pad removed | 57200 | 53.479051 |
| `354` + both re-fetches, pad removed | 57244 | 53.870108 |
| `354` + state-4 re-fetch + pad | 57236 | 53.892723 |
| `345` pad removed | 57232 | 51.752115 |
| `435` + state-4 re-fetch, pad removed | 57236 | 52.181279 |

Measured test order (first `cmp rX,#N` sequence after the pulse block):

| source order | emitted test order |
|---|---|
| `435` | 3, 5, 4 |
| `354` | 5, 3, (4 later) |
| `345` | 5, 4, 3 |

None of the six permutations emits the original `4, 3, 5`; the committed `435`
is the only shape that is simultaneously size-exact and `.rel.plt`-exact.

## 3. Where the 4 bytes are (prologue +4, case-12 tail -8)

For the best variant (`354`, state-4 re-fetch, pad removed, 57228 B) the 4-byte
deficit is fully accounted for by two independent register-allocation
differences:

**(a) Prologue: ours is +4.**  Both builds agree byte-for-byte for the first 24
instructions (`push/add/stmdb/vpush/sub`, `mov sl,r0`, `mov r4,sl`, `mov r6,r1`,
`ldr.w fp,[r0]`, guard store, `ldr.w r0,[r4,#24]!`, `getMessages`, `mov r5,r0`,
`getPlayer`, `strd r4,r0,[sp,#80]`, `getEnemies`, `mov r8,r0`).  Then:

```
orig  0x137d5a: ldr     r4, [r0, #0]     ; 2 B  status -> r4
      0x137d5c: ldr     r0, [r4, #0]     ; 2 B
ours  0x137d5a: ldr.w   r9, [r0]         ; 4 B  status -> r9
      0x137d5e: ldr.w   r0, [r9]         ; 4 B
```

`r4` is dead after `getEnemies()` in both builds (it holds `&m_pLevel`, already
spilled to `[sp,#80]`), yet clang picks the high register `r9` in ours.  The
choice costs exactly 4 bytes (two `ldr.w` vs two 16-bit `ldr`).  This is the
"`Globals::status` r4-vs-r9 prologue allocation" left open by pass 4.  Six
source permutations of the three top-level locals
(`messages`/`player`/`enemies`) were measured; `messages,player,enemies` (the
committed order) is the only one that produces the original call order
`getMessages, getPlayer, getEnemies`, and all six still put `status` in `r9`
for the `354` chain (only the `435`+pad shape changes the whole prologue to
`fp`/`r6`, which is *less* faithful).

**(b) Case-12 tail: ours is -8.**  In the mission-0 `switch` case 12
(`m_nScriptTimerA` supernova block, source lines ~3455-3490) the original
re-loads the array index (`r9`) from `[sp,#60]` on the `m_nScriptTimerA < 5000`
edge path:

```
orig  0x14594c: b.n     0x145952
      0x14594e: ldr.w   r9, [sp, #60]   ; reload index
      0x145952: blx     __aeabi_l2f
ours  0x14594c: blx     __aeabi_l2f     ; r9 already live
```

plus a dead `ldr.w r9,[sp,#80]` at `0x1458b4` in ours.  Net -8 bytes.  This is
the same "index stack-slot / live-range" issue that pass 4 called the
"mission-1 `player->rotate` stack-slot swap"; it is really a whole-function
stack-slot assignment difference (`[sp,#60]` vs `[sp,#80]`), not a mission-1
statement.

`+4` (prologue) `-8` (tail) = `-4`, the exact size gap.  Fixing *both* would
make the `354` shape exactly 57232 B and raise the raw score to ~54; fixing
either alone overshoots.

## 4. The mission-89 `tbh` (correction to the wave-92 brief)

The brief's "strongest untested lead" — a real `tbh` at `0x13866a`
(`subs r0,#1; cmp r0,#4; tbh`) dispatching "mission-0 states 1..5" — is
**mission 89, not mission 0**.  `0x13866a` is only reachable from
`0x138574`, which is guarded by `getCurrentCampaignMission() == 89`
(`0x13857a: cmp r0,#89`).  Its table targets are the mission-89 bodies:

| table entry | target | body |
|---|---|---|
| state 1 | `0x138678` | `m_nScriptTime >= 30001` camera/`m_pGeometry6` setup |
| state 2 | `0x13b218` | distance/rumble/`m_pGeometry6` |
| state 3 | `0x13b15e` | `Layout::isFading` / planet scaling |
| state 4 | `0x13b3ba` | `scaleSunDuringSupernovaIntro` |
| state 5 | `0x13b0ea` | `enableFillScreen`, `nextCampaignMission(true)`, `Galaxy::getStation(10)`, `SetCurrentApplicationModule(5)` |

Our source already implements this as `switch (m_nState)` in the mission-89
branch (source line 1237), and the committed build already emits a `tbh` at
`0x138658` (0x12 bytes earlier than the original because the surrounding
mission-89 block is shorter).  There is no missing mission-0 switch to add.

## 5. Literal-pool order evidence

The final constant pool differs in exactly one pair:

```
orig 0x145c70: 100.0, -3000.0, 0.8, 3000.0, 6500.0, 25.0, 1.7, ...
ours 0x145c6c: 100.0, -3000.0, 3000.0, 0.8, 6500.0, 25.0, 1.7, ...
```

`0.8f` (case-12 planet scaling) and `3000.0f` (case-12 explosion start) are
swapped in the pool.  The final instruction stream uses `3000.0` first
(`0x145a1c`) and `0.8` second (`0x145b1e`) in *both* builds, so the pool order
tracks the selection order, not the final block order.  Moving the planet
scaling block before the explosion block in the source swaps the pool but
changes the size to 57224/57236 and lowers the score (51.76/51.96 on `435`,
53.70 on `354`), so it is not the missing 4 bytes.

## 6. Variant grid (official verifier, raw difflib ratio, autojunk off)

All builds use `cmake --build cmake-build-match --target link_so` and
`score.py` (verify.disassemble + difflib).  `r3`/`r4` = add a
`player = m_pLevel->getPlayer();` re-fetch before the state-3 / state-4
`setCollide` call (the original re-fetches at `0x138a38` and `0x138d90`).
`pad` = the committed `volatile int pad = delta; (void) pad;` in the state-0
setup.

| order | r3 | r4 | pad | size | raw % |
|---|---:|---:|---:|---:|---:|
| 435 | 0 | 0 | 1 | **57232** | **51.997113** |
| 435 | 0 | 0 | 0 | 57224 | 51.604072 |
| 435 | 0 | 1 | 0 | 57236 | 52.181279 |
| 435 | 0 | 1 | 1 | 57244 | 52.181062 |
| 435 | 1 | 0 | 0 | 57244 | 52.176079 |
| 435 | 1 | 0 | 1 | 57252 | 52.180844 |
| 435 | 1 | 1 | 0 | 57260 | 51.174492 |
| 435 | 1 | 1 | 1 | 57268 | 51.169394 |
| 345 | 0 | 0 | 0 | 57232 | 51.752115 |
| 345 | 0 | 0 | 1 | 57240 | 51.965957 |
| 345 | 0 | 1 | 0 | 57252 | 52.143587 |
| 345 | 1 | 0 | 0 | 57244 | 52.123870 |
| 354 | 0 | 0 | 0 | 57200 | 53.479051 |
| 354 | 0 | 0 | 1 | 57208 | 53.737585 |
| 354 | 0 | 1 | 0 | 57228 | **53.913043** |
| 354 | 0 | 1 | 1 | 57236 | 53.892723 |
| 354 | 1 | 0 | 0 | 57228 | 53.886785 |
| 354 | 1 | 0 | 1 | 57236 | 53.866467 |
| 354 | 1 | 1 | 0 | 57244 | 53.870108 |
| 354 | 1 | 1 | 1 | 57252 | 53.879686 |
| 453 | 0 | 0 | 0 | 57184 | 47.457374 |
| 534 | 0 | 0 | 0 | 57192 | 49.357698 |
| 543 | 0 | 0 | 0 | 57192 | 49.357698 |

Only two shapes in the whole 48-build grid are exactly 57232 B: the committed
`435/0/0/pad` (51.997113) and `345/0/0/nopad` (51.752115).  Every `354` shape is
57200-57252.

Pad-form sweep on `354 r4=1 pad=0` (57228): every form adds 8 or 16 bytes and
none adds 4 (`volatile int`, `volatile int; pad=delta`, `volatile char`,
`volatile short`, `volatile long long`, `volatile float`, `volatile double`,
`volatile int[1]`, pointer store).  Pad *location* sweep (state-0 setup,
mission-1, state-3, state-4, state-5, end of mission-0) also never yields
57232.  A `messages,player,enemies` -> `player,enemies,messages` reorder of the
top-level locals raises the `354` score to **54.152650** but shrinks the
function to 57180 B (and up to 57320 with pads), still not 57232.

## 7. Why nothing was landed

`LevelScript::process` is a mid-`.text` function.  Landing the 57228 B variant
was measured end-to-end with the official gate:

```
verify: byte 4317  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
  FAIL  byte_exact 4499 -> 4317
RATCHET FAIL
```

The 4-byte shrink shifts every following function by 4 bytes, so 182
functions stop being byte-exact.  `linked_exact` stays 4499 (symbol-resolved
calls), but `byte_exact` is a hard ratchet.  Only a shape that is *exactly*
57232 B can carry the 53.9 raw score.

## 8. What remains

1. Force `Globals::status` into `r4` (not `r9`) in the prologue.  This is worth
   exactly 4 bytes and would close the gap on its own if the case-12 tail
   reload is also reproduced.  The allocator choice is not steered by any of
   the six top-level local permutations; it likely needs the `&m_pLevel`
   live-range to end one instruction earlier (the original reuses `r4`
   immediately after `getEnemies`).
2. Reproduce the case-12 `r9` reload (`ldr.w r9,[sp,#60]` at `0x14594e`) and
   the `[sp,#60]` index slot instead of `[sp,#80]`.  This is a whole-function
   stack-slot assignment difference.
3. The mission-0 default switch case 12 literal-pool order (`0.8` before
   `3000.0`) is a selection-order difference; swapping the source blocks is
   not sufficient.

## 9. Reproduction

```
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8       # RATCHET PASS
FN=_ZN11LevelScript7processEi cmake --build cmake-build-match --target verify-fn
```

The `354` variant used for the 53.913 measurement is the committed source with
the three mission-0 state tests re-ordered to `state==3`, `state==5 && msg12`,
`state==4`, the state-4 `setCollide` preceded by
`player = m_pLevel->getPlayer();`, and the `volatile int pad` removed.  It is
**not** committed because it is 4 B short.
