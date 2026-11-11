# LevelScript::process — wave 177 / w12106: mission-0 dispatch, pool placement,
# data-flow rewrites (no size-exact `.rel.plt`-identical raw win found; note only)

Target `_ZN11LevelScript7processEi`, original `0x137d18`, **57,232 B (0xdf90)**,
size-exact. Owned TU `src/game/world/LevelScript.cpp`. Worktree `wt/w12106`,
branch `wave177-w12106`, base master `9445ac5fb` (wave 176). Scratch
`/home/debian/GalaxyOnFire2/scratch/w12106/`.

`src/` is left at the base (no code change landed). The base is a strong local
optimum: every size-exact, `.rel.plt`-identical shape tried this wave is
raw-neutral or raw-negative.

## Baseline (wave-176 master, gate green)

```
process raw diffbytes    53,568
process aligned insns    18,498
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726
.rel.plt                 byte-identical (md5 289b2a069f8504d1e8a3919e43915e04)
```

## 1. Original mission-0 dispatch (ground truth)

```
0x1389c8  cmp r1,#4 ; beq.w 0x138d2a          (state 4 body)
0x1389ce  cmp r1,#3 ; bne.w 0x138fd8          (state 3 fall-through)
0x1389d4  <state 3 body>                       ... beq.w 0x137f7c (tail)
0x138d2a  <state 4 body>                       ... beq.w 0x138fd4 (check5)
0x138fd4  ldr.w r1,[sl,#28]                    (reload m_nState)
0x138fd8  cmp r1,#5 ; bne.w 0x139b1a          (state 5)
0x138fde  <isOver(12)> ; beq.w 0x139b16
0x139b16  ldr.w r1,[sl,#28]                    (reload after state-5 body)
0x139b1a  subs r0,r1,#6 ; cmp r0,#9 ; bhi.w 0x13f906
0x139b22  tbh [pc,r0,lsl #1]                   10 entries -> states 6..15
0x13f906  cmp r1,#16 ; bne.w 0x137f7c          (state 16 default)
```

Ours merges the state-3 `else if` into the switch:

```
0x1389b0  cmp r1,#5 ; beq.w 0x138d88          (state 5 body)
0x1389b6  cmp r1,#4 ; bne.w 0x139048          (state 4 fall-through)
0x1389bc  <state 4 body>
0x139048  subs r0,r1,#3 ; cmp r0,#12 ; bhi.w 0x13d598
0x139050  tbh [pc,r0,lsl #1]                   13 entries -> states 3,6..15
0x13d598  cmp r0,#16 ; bne.w 0x137f76          (state 16 default)
```

The merge is `SimplifyCFG::FoldValueComparisonIntoPredecessors` (LLVM 7
`SimplifyCFG.cpp:1005`): the state-3 compare block is the switch's
single-block predecessor and its value is GVN-merged with the switch's
`m_nState` load, so case 3 is folded into the switch. The original avoids
this because the state-5 compare sits between the state-3 compare and the
switch, so the switch condition is a PHI of two different reloads.

## 2. SSA-copy hypothesis (the untried lever from the wave-176 handoff)

Make the switch condition a different SSA value from the state-3 compare by
introducing a local copy. Result: **neutral**. GVN merges the two loads
(no intervening store on the fall-through path), so the fold still happens.

| variant | raw | aligned | size | rel.plt |
|---|---:|---:|---|---|
| base | 53,568 | 18,498 | 57,232 | ident |
| `int st = m_nState; switch (st)` (base arm order) | 53,568 | 18,498 | 57,232 | ident |
| `int st = m_nState; switch (st)` + arm order 4,3,5 | 54,339 | 19,007 | 57,232 | **DIFF** (5-entry rotation) |
| `int st` at function top, state-3 check uses `st` | 55,624 | 22,869 | 57,460 | DIFF |
| ... state-4 + state-3 use `st` | 55,509 | 22,447 | 57,440 | DIFF |
| ... state-5 + state-4 + state-3 use `st` | 55,571 | 22,197 | 57,448 | DIFF |

The `int st`-at-top shapes are the only ones measured that bring the
instruction count to the original's 20,830 (20,831 / 20,826 / 20,832), i.e.
the structural shape is close, but they grow the frame by +216..+228 B
(the extra live value spills `&m_pLevel`/`&Globals::status`), so they are not
size-exact.

## 3. Switch-case ordering is a block-placement artefact, not source order

Reordering the `case 6..15` bodies in the source to the original's *body*
order (`6,10,7,8,9,11,12,13,15,14`, decoded from the original's `tbh` at
`0x139b22`) is **byte-identical** to the base: MachineBlockPlacement ignores
the source case order for equal-probability cases.

Original body order (decoded `tbh`): `6@0x139b70, 10@0x139cf76, 7@0x13d022,
8@0x13d13c, 9@0x13d1de, 11@0x13d2d4, 12@0x13d3b8, 13@0x13d440, 15@0x13d4c6,
14@0x13d4f4`. Base body order: `3@0x13906e, 9@0x13ce36, 15@0x13cecc,
6@0x13cef8, 7@0x13d024, 8@0x13d198, 10@0x13d24e, 11@0x13d2fe, 12@0x13d3e6,
13@0x13d470, 14@0x13d4f4`. The difference is a consequence of case 3 being
in the switch (it becomes the fall-through) and of the CFG, not of the
source case order.

## 4. Full measurement table (all size-exact + `.rel.plt`-identical unless noted)

Base raw 53,568 / aligned 18,498 / size 57,232.

| variant | raw | aligned | size | rel.plt |
|---|---:|---:|---|---|
| base | 53,568 | 18,498 | 57,232 | ident |
| arm order 4,3,5 (handoff) | 54,339 | 19,007 | 57,232 | DIFF |
| arm order 3,5,4 (handoff) | 53,940 | — | 57,232 | ident |
| arm order 3,4,5 (handoff) | 55,292 | — | 57,256 | ident |
| arm order 5,3,4 / 5,4,3 (handoff) | 55,026 | — | 57,244 | ident |
| 4,3,5 + nested state-5 `if` | 55,068 | 19,943 | 57,228 | DIFF |
| state-4 as standalone `if` (falls to check3) | 55,634 | 22,632 | 57,304 | ident |
| case reorder 6,10,7,8,9,11,12,13,15,14 | 53,568 | 18,498 | 57,232 | ident |
| `m_nState <= 2` -> `< 3` | 53,568 | 18,498 | 57,232 | ident |
| `m_nState == 16` -> `>= 16` | 53,568 | 18,498 | 57,232 | ident |
| `m_pRadar->enabled` 0/1 -> false/true | 53,568 | 18,498 | 57,232 | ident |
| `m_bCinematicBreak` 0/1 -> false/true | 53,568 | 18,498 | 57,232 | ident |
| `Vector zero{0,0,0}` -> explicit `.x/.y/.z` writes | 53,568 | 18,498 | 57,232 | ident |
| `!(rumble < 1.0f)` -> `rumble > 1.0f` | 54,321 | 18,496 | 57,224 | ident |
| `getSpeed() == 0x40000000` (int compare) | 54,805 | 18,530 | 57,216 | ident |
| drop `Status *status` local (`Globals::status->`) | 53,569 | 18,493 | 57,232 | ident |
| drop `Status` local + tempVec local | 54,489 | 18,509 | 57,216 | ident |
| tempVec local in cases 12/13/14 | 54,664 | 18,564 | 57,200 | ident |
| `setPosition(m_tempVec)` -> `(x,y,z)` | 54,998 | 18,801 | 57,316 | ident |
| `Level *self = m_pLevel` n=2 | 53,596 | 18,502 | 57,232 | ident |
| `Level *self = m_pLevel` n=3 | 55,081 | 21,424 | 57,308 | ident |
| `Level *self = m_pLevel` n=5 | 55,064 | 20,579 | 57,312 | ident |
| `StarSystem *ss = m_pLevel->getStarSystem()` (line 1329) | 53,786 | 18,502 | 57,232 | ident |
| wingman-loop expect strip/flip (line 1471) | 53,635 | 18,490 | 57,232 | ident |
| state-16 `initStreamOutPosition = 0` (original position) | 54,092 | 18,495 | 57,224 | ident |
| state-16 `initStreamOutPosition = 1` moved after appManager | 53,568 | 18,498 | 57,232 | ident |
| state-16 `initStreamOutPosition = 0` moved after appManager | 53,797 | 18,500 | 57,232 | ident |

Handoff (wave-176) measurements on the same base: arm order 4,3,5 = 54,339
(rel.plt DIFF), 3,5,4 = 53,940 (ident), 3,4,5 = 55,292 (size 57,256),
5,3,4 / 5,4,3 = 55,026 (size 57,244), nested switch 4/5/3 + default =
54,999 (size 57,224, DIFF), `status` local drop = 53,569, `status` declared
first = 55,576 (size 57,308), `stationList` local removed = 55,666 (size
57,368), `expect` on stationList compare = 53,641 (size 57,236).

## 5. Two genuine source differences found (not raw wins on their own)

### 5a. State-16 `Level::initStreamOutPosition` value and position

The original's state-16 block (`0x13f906`) stores `switch_to_target_setting
= 1` with a 32-bit `str`, calls `SetCurrentApplicationModule(2)`, then on the
shared return path (`0x13b156`) does `strb r5,[r0]` with `r5 = 0` to
`Level::initStreamOutPosition` (GOT `0x2102f8`). The base source instead has
`Level::initStreamOutPosition = 1;` *before* the appManager call. So the
original value is **0**, not 1, and the store is shared with the return path.

Moving the store after the call with value 1 is raw-neutral (53,568). Setting
it to 0 at the original position gives size 57,224 (-8). Setting it to 0 and
moving it after the call gives size-exact 57,232 but raw 53,797 (+229): the
state-16 block itself gets closer to the original (the `0x13d018` 256-byte
window improves by 104 bytes) but the -8/+8 repacking shifts the block layout
and loses more elsewhere.

### 5b. Literal-pool placement after the line-1505 wingman loop

The original's first literal pool is at `0x1380a4` (right after the `b.n` at
`0x1380a0`, before the loop body); ours is at `0x1380c0` (after the loop
body). The line-1505 wingman loop body is **byte-identical** in both
(`ldr r0,[r4,#4]; ldr.w r5,[r0,r6,lsl #2]; mov r0,r5; blx isWingMan; cbz;
strb.w r8,[r5,#301]; adds r6,#1; ldr r0,[r4,#0]; cmp r6,r0; bcc`) but sits at
`0x138110` in the original and `0x1380a2` in ours. This is ARM constant-pool
placement (ARMConstantIslandPass) choosing a later candidate block in ours;
it is not reachable from the source shape (the `__builtin_expect` at line
1471 is a separate, already-swept cell).

### 5c. Prologue register map (unchanged from wave 176)

Original: `this`=sl, `&__stack_chk_guard`=fp, `&Globals::status`=r4 (16-bit
`ldr`, value reloaded from `[r4]` per use). Ours: `this`=fp, guard=sl,
`&Globals::status`=r9 (32-bit `ldr.w`, value cached in r4). The +6 B prologue
shift propagates into the first literal pool (`orig 0x1380a4` vs
`ours 0x1380c0`). The `self`-alias lever flips `this` to sl but costs
+40..+56 B (wave 159) and is not size-exact on this base (n=2 is size-exact
but does not flip the map; n=3/n=5 are +76/+80 B).

## 6. Conclusion

No size-exact, `.rel.plt`-identical raw win was found. `src/` is left at the
base (`git status --porcelain` clean). The residual is per-block codegen
(the merged 13-entry jump table, the +6 B prologue register map, and the
literal-pool placement), not a single local source difference.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12106
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# raw= 53568 aligned=18498 size=57232/57232 insn=20726/20830
```

Scratch harnesses: `measure.sh`, `variant.py <order>`, `ssa.py <mode>`,
`stvar.py <mode>`, `casereorder.py <order>`, `s16.py`, `wm.py <mode>`,
`sweep.py <expr> <decl>`, `tbh.py` (decode the mission-0 jump table).
