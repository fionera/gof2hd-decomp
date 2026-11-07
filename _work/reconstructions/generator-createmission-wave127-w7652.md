# `Generator::createMission` — wave 127 (w7652): the `rem`/`agent` RA fixed point
# broken by an equivalent `bonus -= rem * 2` rewrite

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, frame 96, `.ARM.extab` LSDA record 172 B).

Status: **landed.**  The function moves **90.0 % -> 97.2 %** (raw 97.178) at the
original size (1996 B) and the original frame (96 B), and the three wave-124/126
source hacks are gone: no `volatile short padA`, no `volatile int *ap = &amount`
alloca, no `Galaxy *&galaxy` alias.  The full gate is unchanged and green.

```
before (HEAD fee528cbe):  90.0 %  1996 B  frame 104  LSDA 172 B
after:                    97.2 %  1996 B  frame  96  LSDA 172 B
```

Full gate (`check`) on the landed tree:

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 1. The clean shape now reproduces the original frame and slot map

The wave-126 "P1" shape (plain `int amount`, `amount = 0; itemId = 0;` in the
else arm, `Galaxy *galaxy = Globals::galaxy` value + direct
`Globals::galaxy->getStation(...)`) already reproduces the original's exact
7-slot map:

```
sp+16  galaxy / bonus          sp+32  status / rem / mission
sp+20  &__stack_chk_guard      sp+36  race / type
sp+24  agent / difficulty      sp+40  rnd
sp+28  targetStation / amount  sp+92  canary
```

`sub sp,#96`, canary at `sp+92`, Strings at `sp+44/56/68/80` — all byte-exact vs
the original.  P1 alone is 83.8 % / 2000 B because the bonus `rem` takes `r9`
and `agent` is then re-spilled to `sp+32` (`9776a ldr r0,[sp,#36]` instead of
`mov r0,r9`), which adds one 4-byte reload and one 4-byte `subne.w`.

## 2. Why `rem` wins `r9` in P1, and how to take it back for free

Greedy RA allocates by spill weight (block-frequency weighted uses).  The bonus
block is cold (0.02 %), so `agent`'s use there (`976e6 mov r0,r9; blx
getRace`) is cheap, while `rem`'s two uses in the same block are a very short,
high-weight live range.  The allocator gives `r9` to `rem` and rematerialises
`agent` from its `sp+24` spill slot at `9776a`.

Wave-126's finding was that *any* extra `agent` use near the bonus block flips
this (`agent != nullptr` -> 95.3 % but 2004 B; `agent != (Agent *)systems` ->
94.9 % but 2000 B), i.e. it buys the RA answer with extra instructions, which
breaks size-exactness and therefore `byte_exact` (every later `.text` function
shifts and its `bl` PLT displacement changes: `check` drops 4506 -> 2045).

The new hypothesis this wave: change the **bonus IR** so `rawBonus` dies at the
`+ rem` and `rem` no longer needs a callee-saved register, instead of adding a
use of `agent`.

## 3. The winning rewrite: `bonus -= rem * 2`

```c++
int rawBonus = (int) (reward * ((Standing *) (intptr_t) Globals::status->getStanding())
                                       ->getMissionBonus(agent->getRace()));
int rem = rawBonus % 50;
bonus = rawBonus + rem;
if (bonus % 50 != 0) {
    bonus -= rem * 2;          // == rawBonus - rem, no overflow (rem in [-49,49])
}
```

`bonus - 2*rem == (rawBonus + rem) - 2*rem == rawBonus - rem` exactly, including
the wraparound case (the `rawBonus + rem` add is still performed first, and
`rem * 2` cannot overflow).  Codegen:

```
orig 9770c  adds r5,r1,r6        ; bonus = rawBonus + rem
     9770e  str  r1,[sp,#32]     ; rem spilled
     97718  ldr  r0,[sp,#32]
     9771e  subne r5,r6,r0       ; bonus = rawBonus - rem
     97720  str  r5,[sp,#16]
ours 9770a  add  r6,r1           ; bonus = rawBonus + rem
     9770c  mov  r5,r1           ; rem in r5
     9771a  subne.w r6,r6,r5,lsl #1   ; bonus -= rem*2
     97768  str  r6,[sp,#16]     ; store deferred past the float-rem block
```

`rawBonus` is dead after the add, so the allocator reuses `r6` for `bonus`
instead of spilling `rem`; `agent` keeps `r9` for the whole function
(`97766 mov r0,r9` before `getImageParts`), and the frame collapses to the
original 96 B with the original slot map.  The remaining 2.8 % is the bonus
block's register choice (`bonus` in `r6` vs `r5`, `rem` in `r5` vs `sp+32`,
deferred vs immediate `str bonus`) plus the `bhi` fold (see §4).

### Measured equivalent spellings (all 1996 B, one `link_so` each)

| variant | pct | note |
|---|---:|---|
| `bonus -= rem * 2` | **97.2** | landed |
| `bonus -= (rem << 1)` | 97.2 | identical code |
| `bonus -= rem + rem` | 97.2 | identical code |
| `bonus = rawBonus + rem - (rem << 1)` | 97.2 | identical code |
| `bonus = rawBonus - rem * 2 + rem` | 93.9 | different fold |
| `volatile int rem` + plain `bonus = rawBonus - rem` | 90.7 | 2000 B |
| plain `bonus = rawBonus - rem` (P1) | 83.8 | 2000 B, `rem` in `r9` |
| `bonusPlus` temp + if/else | 83.8 | 2000 B |
| `bonus = rawBonus + rem; if (...) bonus = rawBonus - rem; else bonus -= rem*2` | 82.3 | 2000 B |
| `int bonus = 0; if ((type|4) != 12) {...}` | 97.2 | identical code |
| `volatile int bonus` | 85.9 | forces stores everywhere |

## 4. Remaining deltas (2.8 %)

1. **Bonus-block register choice.**  The original keeps `bonus` in `r5` and
   spills `rem` to `sp+32`; ours keeps `bonus` in `r6` and `rem` in `r5`, and
   defers the `str bonus,[sp,#16]` past the float-rem block.  The literal
   original form (`bonus = rawBonus - rem`) keeps `rawBonus` live across the
   second `__aeabi_idivmod`, which is what forces `rem` to spill — but that
   same form hands `r9` back to `rem` in the clean shape.  No spelling tried
   gets both (the `rem*2` form trades the literal subtraction for the RA).
2. **`bhi.n` value-range fold.**  `9735a bhi.n 97388` (orig, falls into the
   `race < 4 && type == 13` test) vs `9735a bhi.n 973fa` (ours, folds the test
   away once `type > 14` proves `type != 13`).  `volatile type` (53.6 %),
   `(type & 15) == 13` (70.7 %, 1968 B), `int type`, `typeCopy`, and hint
   reorderings (`__builtin_expect(race < 4, 1) && type == 13` -> 69.4 %,
   1988 B) all move it the wrong way or shrink the body.
3. The `bonus = 0` arm: `movs r0,#0; str r0,[sp,#16]` (orig) vs
   `movs r6,#0` (ours), a direct consequence of (1).

## 5. Reproduction

```sh
cp /dev/shm/w7652/BA1.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6    # RATCHET PASS, byte 4506
```

Scratch (not committed): `/dev/shm/w7652/` — `head.cpp` = old HEAD,
`P1.cpp` = clean shape (83.8 %), `BA1.cpp` = landed source (97.2 %),
`fscore.py` = single-function verify score, `try.sh` = copy + `link_so` +
score, `orig.s` / `ba1.s` = disassembly dumps, `mk*.py` = variant generators.
