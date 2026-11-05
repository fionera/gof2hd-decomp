# `Generator::createMission` — wave 119 (w6810): hint / record sweep, no source win

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `cslen = 151`,
26 call-site rows; ours **168 B**, `cslen = 147`, 25 rows).

Status: **research note, no source change landed.**  Master keeps the tuned
**83.6 % / 1996 B / 168 B record** shape.

```
before (HEAD):  83.6 %  1996 B  .ARM.extab 0x26294  createMission record 168 B (cslen 147, 25 rows)
after:          83.6 %  1996 B  .ARM.extab 0x26294  (unchanged)
```

Gate at the start and end of this pass: `RATCHET PASS`, `byte 4505`,
`linked 4505`, `data 843/843`, imports 207, `lint CLEAN`, `drift 0`,
27 sections, file size 2,187,328 B.

Supersedes/extends `generator-createmission-wave119-w6791.md` (MBP tie at
`getOffer()==5`) and `generator-createmission-wave118-w6609.md` (172 B record
family).  Everything below is new measurement unless stated.

## 1. New hard constraint: the body must stay exactly 1996 B

`byte_exact` is a *whole-image* property: any change to this function's `.text`
size shifts every later TU and changes the PC-relative `bl` encodings of
thousands of unrelated functions.

```
R71 (see §3), usize 2004  ->  verify byte 2045  linked 4505   RATCHET FAIL
R71 + volatile int padA, usize 1996  ->  byte 4489  linked 4505  RATCHET FAIL
```

So a landable shape needs **usize == 1996 exactly**.  This rules out every
172-record shape found below that is not 1996 B.  (The second row also shows a
second constraint: the shape permutes the `.rel.plt`/GOT order — the R71 reward
chain moves `Item::getMaxPrice`'s relocation — which changes 16 other
functions' literal pools even at the same `.text` size.)

## 2. The original's reward chain order is `9, 7, 8, (3||5), 11`

Disassembly of the original (this was not recorded before):

```
9768e: ldr  r0, [sp, #36]      ; type
97690: cmp  r0, #9
97692: vmul.f32 s18, s0, s2
97696: beq.n 976a4             ; type == 9 -> *1.2
97698: cmp  r0, #7
9769a: bne.w 97862             ; type != 7 -> next
9769e: vldr s0, [pc, #716]     ; 0.7
976a2: b.n  976a8
976a4: vldr s0, [pc, #704]     ; 1.2
976a8: vmul.f32 s18, s18, s0
...
97862: cmp  r0, #8             ; type == 8 -> getMaxPrice
978d0: cmp  r0, #3
978d2: it   ne
978d4: cmpne r0, #5            ; type == 3 || type == 5 -> *2.0
978de: cmp  r0, #11            ; type == 11 -> *0.6 + reward/5*amount
```

i.e. the faithful source order is `9, 7, 8, (3||5), 11`, **not** master's
`7, 9, 8, 11, 3, 5`.  The faithful order measures **80.1 % / 2000 B / 168**
(and 77.9–78.6 % with the padA variants) — lower than the tuned 83.6 %, so the
fuzzy metric prefers the *unfaithful* order.  This is the clearest example in
this function of "a faithful source shape scores lower than a tuned one".

## 3. A 172-byte record IS reachable at 1996 B (but not at >= 83.6 %)

`__builtin_expect(type == 7, 1)` on the first reward-chain branch (call it
`R71`) flips the reward-chain layout and produces a **172 B record**
(`cslen 151`, 26 rows):

| variant | pct | usize | rec | rows | extab |
|---|---:|---:|---:|---:|---:|
| HEAD | **83.6** | **1996** | 168/147 | 25 | 0x26294 |
| `R71` (expect type==7,1) | 67.4 | 2004 | **172/151** | 26 | 0x26298 |
| `R71` + `volatile int padA` + `(void)padA;` (read only) | 69.0 | **1996** | **172/151** | 26 | 0x26298 |
| `R71` + `volatile int padA` + `padA = 0;` | 67.4 | 2000 | 172/151 | 26 | 0x26298 |
| `R71` + `volatile float padA` | 67.0 | 2000 | 172/151 | 26 | 0x26298 |
| `R71` + no padA | 69.4 | 1992 | 172/151 | 26 | 0x26298 |
| `expect(type == 9, 1)` | 68.5 | 2004 | 176/152 | 26 | 0x2629c |
| `expect(type == 8, 0)` | 75.9 | 1992 | 176/152 | 26 | 0x2629c |
| `expect((type\|4) == 12, 1)` | 70.5 | 1992 | 176/152 | 26 | 0x2629c |

The 1996 B / 172 B shape (`R71 + volatile int padA` read-only) is the first
known shape that satisfies *both* size constraints; it fails only on the fuzzy
score (69.0 %) and on the `.rel.plt` permutation (`byte 4489`, see §1).  Its
extra row is **not** the original's null-block gap: the original's row 7 is
`0x3c8..0x3e8 lp=0` (the `getRoutes()==nullptr` block between the case-15 and
case-0 try ranges), while `R71`'s extra row is `0x6ac..0x6fc lp=0` in the
reward chain.  Record *size* matches, record *contents* do not.

## 4. The `pick == 12` hint controls the pick-loop rotation

The pick loop is byte-identical to the original except for the two-block order
of `if (pick == 12)` (original: arm is fallthrough, `selected = true` is
outlined; ours with `expect(pick==12,0)`: the reverse).  Removing the hint (or
`expect(pick==12,1)`, or `switch (pick) case 12:`, or an explicit `else`) gives
the original's arm-fallthrough layout — **but un-rotates the loop** (the
`while (!selected)` condition moves from the latch to the top), which costs
more than the pick==12 block gains:

```
97246: b.n 972d4          (HEAD, rotated, matches original)
97246: movs.w r0,fp,lsl#31  (no hint, not rotated)
```

| variant | pct | usize | rec |
|---|---:|---:|---:|
| HEAD (`expect(pick==12,0)`) | **83.6** | 1996 | 168 |
| no `pick==12` hint | 80.9 | 1996 | 168 |
| `expect(pick==12,1)` | 80.9 | 1996 | 168 |
| `switch (pick) { case 12: ... default: }` | 80.9 | 1996 | 168 |
| `if (pick==12) ... else selected = true;` | 80.9 | 1996 | 168 |
| `if (pick > 11 && pick < 13)` | 80.9 | 1996 | 168 |
| `expect(pick==8,0)` (on top of HEAD) | 56.6 | 1960 | 168 |
| `expect(pick==8,1)` (on top of HEAD) | 54.4 | 2008 | 168 |
| `while (expect(!selected,0))` | 55.7 | 1964 | 168 |
| `while (expect(!selected,1))` | 48.6 | 2016 | 168 |
| `selected = expect(guard < 1, 0/1)` | 83.6 | 1996 | 168 (hint folded away) |

The rotation is decided by `findBestLoopExit`/`rotateLoop` on the block
frequencies, and the `pick==12` weight is the only lever that keeps it.

## 5. The `padA` hack is what makes the frame 112 instead of the original 96

Dropping `volatile double padA` entirely gives the **original's frame size
(96)** and puts `&__stack_chk_guard` at `sp+16` (original: `galaxy` at `sp+16`,
guard at `sp+20`) — but the body drops to **1984 B** and the score to 71.9 %.
The original's `sp+16` slot is `galaxy` early and `bonus` late; ours (no padA)
uses `sp+16` for the guard and `sp+20` for `fromSystemPtr`.  The original spills
`galaxy` (`str r0,[sp,#16]` at 97614, reloads at 97644) and keeps
`fromSystemPtr` in `r6`; ours does the opposite.  Forcing the original's
choice from source was tried and always changes the body size:

| variant | pct | usize | frame |
|---|---:|---:|---:|
| HEAD (`volatile double padA`) | 83.6 | 1996 | 112 |
| no padA | 71.9 | 1984 | 96 |
| no padA + `Globals::galaxy` for `getStation` | 65.7 | 2000 | 104 |
| no padA + `Globals::galaxy` for `distance` | 71.5 | 1988 | 96 |
| no padA + `int base` split | 69.7 | 1984 | 96 |
| no padA + `volatile int fromSystem` | 79.5 | 1988 | 96 |
| no padA + `volatile int level` | 78.7 | 1992 | 96 |
| no padA + `volatile int bonus` | 77.8 | 1980 | 96 |
| no padA + `toSystemPtr` temp | 71.9 | 1984 | 96 |
| `int level` inline (decl at `getLevel()` site) | 81.7 | 2000 | – |
| `int level` inline + `int base` | 77.4 | 2000 | – |
| `int level` inline + `volatile int32_t type` | 52.6 | 2056 | – |

## 6. Full single-hint sweep (each hint added to pristine HEAD)

`X0` = `__builtin_expect(cond, 0)`, `X1` = `..., 1)`.  `getName` is the linked
address of `Agent::getName` (original: `0x973fa`; HEAD: `0x97410`).

| hint site | 0 | 1 |
|---|---|---|
| `agent->getOffer() == 5` | 63.9 / 1956 / 168 | 40.0 / 2020 / 168 |
| `race < 4 && type == 13` | 44.4 / 2036 / 168 | 65.6 / 1960 / 168 |
| `getRoutes() == nullptr` | 63.8 / 1980 / 168 | 48.0 / 2036 / 168 |
| `while (targetStation == getStation()->getIndex())` | 76.4 / 1992 / 168 | 43.7 / 2012 / 168 |
| `while (!found)` | 47.4 / 2036 / 168 | 55.6 / 2008 / 168 |
| `while (!selected)` | 55.7 / 1964 / 168 | 48.6 / 2016 / 168 |
| `((1U<<type) & 0x4801) != 0` | 73.3 / 1996 / 168 | 83.6 (HEAD) |
| `difficulty >= 10` | 83.6 / 1996 / 168 | 83.6 / 1996 / 168 |
| `type == 7` (reward) | 78.7 / 1996 / **164** | 67.4 / 2004 / **172** |
| `type == 9` (reward) | 78.4 / 1996 / 164 | 68.5 / 2004 / 176 |
| `type == 8` (reward) | 75.9 / 1992 / 176 | 69.2 / 2004 / 164 |
| `type == 11` (reward) | 83.2 / 1996 / 168 | 71.8 / 2004 / 168 |
| `type == 3` (reward) | 80.1 / 1992 / 168 | 72.1 / 2008 / 168 |
| `type == 5` (reward) | 80.2 / 2000 / 168 | 80.2 / 2000 / 168 |
| `(type \| 4) == 12` (bonus) | 68.1 / 2012 / 168 | 70.5 / 1992 / 176 |
| `type == 8` (costs) | 81.0 / 1996 / 168 | 72.8 / 1996 / 180 |
| `type == 6` (costs) | 71.8 / 1996 / 180 | 83.5 / 1996 / 168 |
| tail `stationIsInSystem` | 71.1 / 1992 / 176 | 78.0 / 1996 / 168 |

No single hint reaches 172/151/26 at 1996 B *and* >= 83.6 %.

## 7. Other variants measured this pass (all dead ends)

| variant | pct | usize | rec |
|---|---:|---:|---:|
| `amount = 0` plain + `volatile int amount` | 83.5 | 1996 | 168 |
| `*(volatile int32_t *)&amount = 0` | 83.6 | 1996 | 168 |
| `*(volatile unsigned *)&amount = 0` | 83.6 | 1996 | 168 |
| `guard = 1000` / `guard = 998` | 83.5 / 83.5 | 1996 | 168 |
| swap `setCosts`/`setProductionGoods` | 79.4 | 1996 | 180 |
| `setBonus` first | 79.5 | 1996 | 180 |
| faithful reward chain `9,7,8,3\|\|5,11` | 80.1 | 2000 | 168 |
| `int level` inline + `int base` | 77.4 | 2000 | 176 |
| `int level` inline + `volatile int32_t type` | 52.6 | 2056 | 180 |
| `R71` + `int base` | 65.4 | 2004 | 172 |
| `R71` + `int level` inline | 67.9 | 2012 | 180 |
| `R71` + `volatile float padA` | 67.0 | 2000 | 172 |
| `R71` + `volatile int padA` + read only | **69.0** | **1996** | **172** |

## 8. Conclusion

The two remaining deltas are both `MachineBlockPlacement` block-order choices
(the `bb.32`/`bb.48` chain into the shared `type` store, and the `pick==12`
two-block order), and every source lever that fixes one perturbs the other or
changes the body size.  Master's 83.6 % / 1996 B / 168 B shape remains the best
landable one.  The first 1996 B / 172 B shape is recorded above
(`R71 + volatile int padA` read-only, 69.0 %) for a future pass that can also
undo the `.rel.plt` permutation.

## 9. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6810/one.py                           # pct 83.6, usize 1996
python3 /dev/shm/w6810/lsda_info2.py cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE   # 168 / 147 / 25
```

Scratch: `/dev/shm/w6810/` (`pristine.cpp`, `one.py`, `measure.py`,
`lsda_info2.py`, `lsda_rows.py`, `detect.py`, `sweep2.py`, `h1.json`,
`h2.json`, `v5.json`..`v17.json`).  Not part of the repo.
