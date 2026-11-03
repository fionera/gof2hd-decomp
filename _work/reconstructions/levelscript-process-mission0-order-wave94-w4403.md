# LevelScript::process — mission-0 block order + top-level mission-dispatch order (w4403, wave 94)

Status: **note-only pass, 2026-11-03**.  Branch `wave94-w4403`, base master
`f262bd4b`.  No code change landed: every source shape that scores better than
the committed one either (a) changes the function size away from 57232 B, or
(b) reorders `.rel.plt`/`.dynsym` and thereby drops `byte_exact` 4499 -> 4495.
The committed source is a measured local optimum on the gate metrics.

Target: `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**, 20169 instructions, 52.0 raw fuzzy.

```
gate before == gate after (no source change)
byte_exact       4499
linked_exact     4499
size-exact       4524/4524
stub_zero_size   0
missing/wrong/extra 0/0/0
data_exact       843/843
imports          207
lint             CLEAN
drift            0
.text            0x1337c8
```

Tools used (all under `/dev/shm/w4403`, nothing in the repo):

* `score.py` — `tools/verify/verify.py` `disassemble` + `reconcile` + `score`
  (difflib `SequenceMatcher`, `autojunk=False`) for one function.
* `order4.py` — prints every `cmp r1,#N` (N in 2..5) with its following branch,
  in address order, function-relative.
* `missions2.py` — prints every `blx Status::getCurrentCampaignMission` whose
  *next* instruction is `cmp r0,#N` followed by `beq`/`bne`.
* `linkcheck.py` — compares `.rel.plt`, `.dynsym`, `.dynstr`, `.hash`,
  `.rodata`, `.data`, `.text`, `.ARM.exidx`, `.ARM.extab`, `.got` against the
  original.
* `delta.py` / `regions.py` — aligned instruction diff with per-region byte
  counts.

---

## 1. Mission-0 chain: the original source order is the committed `435`

Original Thumb disassembly (`android_2.0.16_libgof2hdaa.so`), function-relative
offsets are `addr - 0x137d18`:

```
0x6e6  cmp  r1,#2 ; bgt.w 0xcb0        ; state <= 2  (wobble / state 1 / state 2)
       <state<=2 body, ends with a jump to the shared tail 0x137f7c>
0xcb0  cmp  r1,#4 ; beq.w 0x1012       ; state == 4
0xcb6  cmp  r1,#3 ; bne.w 0x12c0       ; state == 3
0xcb8  <state-3 body 0x1389d4..0x138d26>
0x1012 <state-4 body 0x138d2a..0x138dfa>
0x12bc <0x138fd4: ldr.w r1,[sl,#28]>   ; state-4 "msg10 not over" lands here
0x12c0 cmp  r1,#5 ; bne.w 0x1e02       ; state == 5
0x12c6 <0x138fde: messages[12]->isOver() ...>
0x1e02 <0x139b1a: subs r0,r1,#6; cmp r0,#9; bhi default; tbh>  ; switch 6..15
```

So the **emitted** equality-test order is `4, 3, 5` and the source order it
implies is `state<=2 / state==4 / state==3 / state==5` — exactly the committed
`435` (`src/game/world/LevelScript.cpp`, mission-0 block, lines 615..942).

This **confirms** the w4266 note and re-confirms that the earlier "the original
source order is 354" reading was wrong.

### 1.1 The state-4 "not over" edge is the key structural witness

`0x138d36: beq.w 0x138fd4` — when radio message #10 is *not* over, the original
does **not** fall out of the mission-0 chain to the shared tail; it reloads
`m_nState` and re-tests `state == 5`:

```
0x138d34   cmp  r0,#0
0x138d36   beq.w 138fd4
...
0x138fd4   ldr.w r1,[sl,#28]      ; reload m_nState (the isOver() call may alias)
0x138fd8   cmp  r1,#5
0x138fda   bne.w 139b1a           ; -> switch
```

Ghidra renders the same edge explicitly (`decompile_function` of `0x147d18`,
saved at `/dev/shm/w4403/decomp2.txt` lines 1292-1330):

```c
  if (iVar8 == 4) {
    iVar7 = RadioMessage::isOver(*(RadioMessage **)(puVar5[1] + 0x28));
    if (iVar7 == 0) {
      iVar8 = *(int *)(this + 0x1c);
      goto LAB_00148fd8;              /* == if (iVar8 == 5) */
    }
    ...  uVar18 = 5;  goto LAB_0014d4ec;   /* shared *(this+0x1c)=uVar18 store */
  }
```

and the state-5 body is a *nested* `if`, not a `&&` in the `else if` header:

```c
  if (iVar8 == 5) {
    iVar7 = RadioMessage::isOver(*(RadioMessage **)(puVar5[1] + 0x30));
    if (iVar7 != 0) { ... }
    iVar8 = *(int *)(this + 0x1c);
  }
  switch (iVar8) { case 6: ... }
```

Our committed source already has the nested `if` inside a `state == 5 && ...`
`else if`; making the `&&` a real nested `if` (variant `nested5`) gives
57180 B / 50.3 and is worse.

### 1.2 No source permutation emits the original `2,4,3,5`

`order4.py` on the six `state<=2`-first permutations of `{3,4,5}` (all builds
`cmake --build cmake-build-match --target link_so`, committed source otherwise):

| source order | emitted `cmp r1,#N` order | size | raw % |
|---|---|---:|---:|
| `345` | 2, 5, 4, 3 | 57240 | 52.0 |
| `354` | 2, 5, 3, (4 later) | 57208 | 53.7 |
| **`435` (committed)** | **2, 3, 5, 4** | **57232** | **52.0** |
| `453` | 2, 5, 4, (3 later) | 57184 | 47.2 |
| `534` | 2, 5, ... | 57200 | 49.4 |
| `543` | 2, 5, ... | 57200 | 49.4 |

Original = `2, 4, 3, 5`.  None of the six reproduces it, so the residual is
MachineBlockPlacement, not the source order.

---

## 2. NEW: the `354` reorder breaks `.rel.plt` / `.dynsym`

The w4266 note already called `435` "the only shape that is simultaneously
size-exact and `.rel.plt`-exact", but did not record *why*.  This pass pinned
it down.

`354` (`state==3`, `state==5 && msg12`, `state==4`) moves the state-5 block
ahead of the state-4 block in the instruction stream.  The state-5 body is the
first place `PlayerEgo::getSpeed` is referenced, so gold gives it the first PLT
slot instead of the last:

```
symbol                                                    orig PLT   354 PLT
_ZN9PlayerEgo15setFreeLookModeEb                          0x66b94    0x66ba0
_ZN18TargetFollowCamera20enableFirstPersonCamEb           0x66ba0    0x66bac
_ZN9PlayerEgo32hideShipForFirstPersonCameraViewEb         0x66bac    0x66bb8
_ZN6Player13removeAllGunsEv                               0x66bb8    0x66bc4
_ZN9PlayerEgo8getSpeedEv                                  0x66bc4    0x66b94   <-- moved
```

`linkcheck.py` on the plain `354` build:

```
.rel.plt  29392 29392 equal=False
.dynsym   89248 89248 equal=False
.dynstr  189208 189208 equal=True
.hash     38716 38716 equal=True
.rodata  269159 269159 equal=True
.data     10604 10604 equal=True
.text   1259464 1259464 equal=False      (LevelScript::process itself)
.ARM.exidx 28528 28528 equal=False
.ARM.extab 156304 156304 equal=False
.got      17164 17164 equal=True
```

Consequence: the four `MGame` functions that call those PLT stubs stop being
byte-exact.  Measured with `report.json` diff (base vs `354`):

```
_ZN5MGame12successCheckEv      byte True -> False   (4 bytes differ, first at +0x12e)
_ZN5MGame12switchCameraEi      byte True -> False   (4 bytes differ, first at +0xa0)
_ZN5MGame13dialogueEventEv     byte True -> False   (3 bytes differ, first at +0xde)
_ZN5MGame13gameOverCheckEv     byte True -> False   (3 bytes differ, first at +0x14e)
```

e.g. `MGame::successCheckEv+0x12e`:

```
base  180084: f6e6 ed86  blx 66b94 <PlayerEgo::setFreeLookModeEb@plt>
354   180084: f6e6 ed8c  blx 66ba0 <PlayerEgo::setFreeLookModeEb@plt>
```

`byte_exact 4499 -> 4495`, which is a hard ratchet failure.  So the ~1.7 raw
points the `354` reorder buys are not free — the mission-0 chain must keep
`state==4` before `state==5` in the emitted instruction stream.

---

## 3. NEW: the top-level mission-dispatch order differs (mission 5 / 50)

`missions2.py` on the committed build vs the original — every
`blx Status::getCurrentCampaignMission()` whose next instruction is
`cmp r0,#N` + `beq`/`bne`, in address order:

```
ORIG   1, 81, 26, 78, 50, 89, 42, 106, 95, 99, 109, 119, 126, 133, 160, 161
OURS   1, 81, 26, 78,  5, 89, 42, 106, 95, 99, 109, 119, 126, 133, 160, 161
```

The two chains are identical except at one slot: **the original tests mission
`50` where ours tests mission `5`.**  The raw test sites:

```
ORIG  0x138132: cmp r0,#5      (mission 5, immediately after the shared tail 0x137f7c)
      0x1383ea: cmp r0,#50     (mission 50, after the mission-78 block)
      0x13877c: cmp r0,#51
OURS  0x13836e: cmp r0,#5      (mission 5, after the mission-78 block)
      0x138766: cmp r0,#50
      0x138776: cmp r0,#51
```

So in the original the mission-5 branch is emitted **before** the mission-81
branch (`0x1381b4`), while ours emits it after mission 78.  This is a concrete,
previously-unrecorded source-order divergence in the pre-switch specials.

It is *not* a local fix: the source's pre-switch specials are written
`26, 5, 50, 51` after the `81, 78, 89, 106, 95..161` chain, and clang's block
placement reorders them heavily.  Moving the mission-5 block ahead of the
mission-81 block in the source (variant `move5`) produces

```
OURS  1, 5, 81, 78, 89, 106, 99, 109, 119, 126, 133, 160, 161, 26, 42
size 57220, raw 42.4   (vs 57232 / 52.0)
```

i.e. it matches the original's first two entries and then diverges much worse
and loses size-exactness.  The dispatch order therefore has to be fixed
together with the block bodies, not by moving one `else if`.

---

## 4. Faithful `getPlayer()` re-fetches in state 3 and state 4

The original re-fetches the player through `Level::getPlayer()` in exactly two
places in the mission-0 block:

```
0x138a34  ldr.w r0,[sl,#24]      ; state 3
0x138a38  blx   Level::getPlayer
0x138a3c  movs  r1,#1
0x138a3e  blx   PlayerEgo::setCollide

0x138d86  ldr.w r0,[sl,#24]      ; state 4
0x138d8a  movs  r1,#1
0x138d8c  strb.w r1,[sl,#17]     ; m_bCinematicBreak = 1
0x138d90  blx   Level::getPlayer
0x138d94  movs  r1,#0
0x138d96  blx   PlayerEgo::setCollide
```

Every other `PlayerEgo::*` call in the block uses the cached `player` local
(`[sp,#84]` in the original).  Our committed source uses the cached local for
both `setCollide` calls, i.e. it is missing two `Level::getPlayer()` calls.

Applying them (`apply.py r4 r3`) makes the state-4 body match the original
instruction-for-instruction around `m_bCinematicBreak`/`setCollide`, but it is
score-neutral-to-slightly-negative because of block placement:

| variant | size | raw % |
|---|---:|---:|
| committed | **57232** | **52.0** |
| `+r4` (state-4 re-fetch) | 57232 | 51.9 |
| `+r3` (state-3 re-fetch) | 57240 | 51.9 |
| `+r4 +r3` | 57240 | 51.9 |
| `+r4 +r3` with `m1`/`state4` pads | 57256 | 51.9 |

A 28-combination grid (`r4` x `r3` x pads at `m1`/`state4`/`case6`) was swept;
**every** non-committed combination is <= the committed 52.0 and most change
the size.  So the two re-fetches should land in the pass that also fixes the
prologue/stack-slot residuals (§5), not on their own.

---

## 5. Residuals (unchanged from w4266, re-verified)

1. **Prologue `Globals::status` in `r9` vs `r4`** (+4 B).  Original
   `0x137d5a: ldr r4,[r0,#0]; ldr r0,[r4,#0]` (two 16-bit), ours
   `ldr.w r9,[r0]; ldr.w r0,[r9]` (two 32-bit).  `r4` is dead after
   `getEnemies()` in both builds yet clang keeps `status` in the high register
   in ours.  The natural declaration order was re-measured this pass (w4266 measured
   six permutations); none moves `status` into `r4` for the `435` chain.
2. **Whole-function stack-slot swap**: original spills `player` at `[sp,#84]`
   and `delta` at `[sp,#76]`; ours spills `player` at `[sp,#76]` and `delta`
   at `[sp,#84]`.  This is what makes every `ldr r0,[sp,#84]` in the original
   compare against `ldr r0,[sp,#76]` in ours (hundreds of lines) and is also
   the case-12 tail -8 B.  Declaration order, `Level *level = m_pLevel;` and
   `Status *status = Globals::status;` variants were all measured; none flips
   the pair.
3. **Literal pool**: `0.8f` and `3000.0f` are swapped in the final pool
   (selection order, not block order).

---

## 6. What was proved / what remains

Proved:

* The original mission-0 source order is `state<=2 / state==4 / state==3 /
  state==5` (committed `435`); the emitted equality-test order is `2,4,3,5`.
  The state-4 "msg10 not over" edge re-tests `state == 5`
  (`0x138d36 -> 0x138fd4`), and state 5 is a nested `if`, not a `&&`.
* No permutation of the three `else if` tests emits `2,4,3,5`.
* The `354` reorder that scores 53.7 is a link-level regression: it moves
  `PlayerEgo::getSpeed`'s PLT slot and costs 4 `MGame` functions their
  byte-exactness (`4499 -> 4495`).
* The original re-fetches the player via `Level::getPlayer()` before both
  mission-0 `setCollide` calls (state 3 at `0x138a38`, state 4 at `0x138d90`);
  the committed source does not.
* The original's top-level mission dispatch emits mission `5` at `0x138132`
  (before mission 81) and mission `50` at `0x1383ea`; ours emits `5` at
  `0x13836e` (after mission 78) and `50` at `0x138766`.

Remains (in priority order):

1. Fix the prologue `r4`-vs-`r9` and the `player`/`delta` stack-slot swap
   together — they are the same register-allocation problem and account for
   the whole byte gap (-4 net).
2. Once the stack slots match, re-apply the two faithful `Level::getPlayer()`
   re-fetches and re-measure; they should then be score-positive.
3. Re-derive the pre-switch mission-special order (`5` before `81`, `50`
   between `78` and `89`) from the original block layout and re-measure.
4. Only after (1)-(3) should the mission-0 `else if` order be revisited: with
   matching bodies the `435` chain may well emit `2,4,3,5` naturally.
