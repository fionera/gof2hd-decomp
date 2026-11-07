# `Generator::createMission` — wave 126 (w7631): plain-`amount` slot map vs the
# `rem`/`agent` register-allocation fixed point

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**).

Status: **research note, no source change landed.**  HEAD stays at
**90.0 % / 1996 B / frame 104 / LSDA 172 B**.  This pass rebuilt the exact
original slot map from the linked disassembly, then tested the `amount`
storage-class × `galaxy`-form × declaration-order matrix against it.  The
remaining delta is a single register-allocation fixed point: the bonus `rem`
must spill so `agent` stays in `r9`, and the `amount` spill must occupy
`sp+28` so `galaxy` moves to `sp+16`.  Every shape that achieves one loses the
other.

```
before (HEAD 4bbe88776):  90.0 %  1996 B  frame 104  LSDA 172 B (exact)
after:                    90.0 %  1996 B  frame 104  LSDA 172 B (exact)
```

Full gate (`check`) on the restored HEAD:

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 1. The original slot map, re-derived instruction by instruction

```
sp+16  galaxy (97614 str, 97644 ldr) / bonus (976d8, 97720 str, 9783a ldr)
sp+20  &__stack_chk_guard (971f6 str, 978b2 ldr)
sp+24  agent (97220 str, 97306/97322 ldrd) / difficulty (9761c str, 9764c ldr)
sp+28  targetStation (97218/9721c str) / amount (974a8, 974b4, 9753e, 975ca, 97604 str)
sp+32  status (97228 str, 972b4/972da/97536/9761a/976ac/976dc ldr)
       / rem (9770e str, 97718 ldr) / mission (9775e str, 9776c/97824/9793c ldr)
sp+36  race (9726e, 9730c/97326 ldr) / type (97340 str, 97356/9738e/97420 ldr)
sp+40  rnd (97244 str, 11 loads)
```

The 4 Strings are at `sp+44/56/68/80`, the canary at `sp+92`: **7 spill slots,
no frame objects except canary + Strings**, frame 96.  The original's SSC
colour order is `rnd > race > status > targetStation > agent > guard > galaxy`
(w4508 §2), i.e. `40, 36, 32, 28, 24, 20, 16`.

## 2. HEAD is 6 of 7 slots correct

HEAD (`volatile short padA` + `volatile int *ap = &amount` + `Galaxy *&galaxy`):

```
sp+20 guard    sp+24 agent/difficulty    sp+28 targetStation/galaxy/bonus
sp+32 status   sp+36 race/type           sp+40 rnd      sp+92 amount alloca
```

So guard(20), agent/difficulty(24), status(32), race/type(36), rnd(40) already
match.  The only mismatches are **galaxy at 28 instead of 16** and **amount at
92 instead of 28**.  Because `amount` is a frame object, `targetStation`'s
`sp+28` is free after `97306`, and `galaxy`/`bonus` take it.  If `amount` were
a real spill at `sp+28`, `galaxy` would fall through to `sp+16` — exactly the
original.

## 3. The clean plain-`amount` shape (C) gets the slots right but loses `agent`

`C` = HEAD with `int amount;` + `amount = 0;` (no alloca), `itemId = 0` moved
into the else arm, and `Galaxy *galaxy = Globals::galaxy` + direct
`Globals::galaxy->getStation(...)` ("mixed"):

```
sp+16 galaxy/bonus    sp+20 guard    sp+24 difficulty    sp+28 targetStation/amount
sp+32 agent           sp+36 status   sp+40 race/type    sp+44 rnd
```

`galaxy`(16), `guard`(20), `targetStation/amount`(28) are now exact.  But
`agent` lands at `sp+32` (a new slot) and `status/race/rnd` shift `+4`, because
the bonus `rem` takes `r9`:

```
C  9770c  adds r5,r1,r6
   9770e  mov  r9,r1          ; rem in r9
   9771c  subne.w r5,r6,r9
   9776a  ldr r0,[sp,#36]     ; agent reloaded at the Mission ctor
orig 9770c  adds r5,r1,r6
     9770e  str  r1,[sp,#32]  ; rem spilled
     97718  ldr  r0,[sp,#32]
     9771e  subne r5,r6,r0
     9776a  mov  r0,r9        ; agent still in r9
```

`agent`'s spill slot is then live from `97220` to `9776a`, overlaps
`difficulty` (`9761c..9764c`), and cannot share `sp+24`.

## 4. The alloca is exactly what makes `rem` spill — but it also frees `sp+28`

`-mllvm -print-machineinstrs` (After Virtual Register Rewriter) shows the
fixed point:

* HEAD: `t2STRi12 $r1, %stack.17` (rem spilled); `bb.129 liveins: $r9` (agent
  live); `9776a mov r0,r9`.
* C: `renamable $r9 = COPY $r1` (rem in r9); `bb.128 liveins` has no `$r9`
  (agent spilled); `9776a ldr r0,[sp,#36]`.

The `volatile int *ap = &amount` alloca changes the register pressure enough
that Greedy RA spills `rem` and keeps `agent` in `r9` (wave-125 §3).  But the
same alloca removes `amount` from the spill-slot set, so `galaxy` takes
`sp+28`.

## 5. `volatile int amount` has the right RA but the wrong slots

`a_volint` = C with `volatile int amount;` (no pointer):

```
sp+20 guard    sp+24 agent/galaxy/bonus    sp+28 targetStation/difficulty
sp+32 status/rem    sp+36 race/type    sp+40 rnd    sp+92 amount (frame object)
```

This *does* spill `rem` and keep `agent` in `r9` (`97772 mov r0,r9`), but
`amount` is a fixed frame object at `sp+92`, so `difficulty` takes `sp+28` and
`galaxy` shares `agent`'s `sp+24`.  Score **88.7 % / 2004 B**.

## 6. Measured matrix (one `link_so` per row)

Base = HEAD (`padA short`).  `amount` = `plain` (`amount = 0`), `ptr`
(`volatile int *ap = &amount`), `volint` (`volatile int amount`).
`galaxy` = `ref` (`Galaxy *&`), `mixed` (value + direct `Globals::galaxy` for
`getStation`), `both` (value for both).  All keep `itemId = 0` in the else arm.

| padA | amount | galaxy | pct | usize | frame | bonus `rem` |
|---|---|---|---|---:|---:|---|
| short | ptr  | ref   | **90.0** | 1996 | 104 | spill |
| short | ptr  | both  | 88.6 | 1996 | 104 | spill |
| short | ptr  | mixed | 87.6 | 2008 | 104 | spill |
| short | volint | ref | 89.9 | 1996 | 104 | spill |
| short | volint | both | 88.5 | 1996 | 104 | spill |
| short | volint | mixed | 87.4 | 2008 | 104 | spill |
| short | volint | ref (plain) | 88.1 | 1996 | 104 | spill |
| short | plain | mixed | 85.1 | 2008 | 104 | **r9** |
| short | plain | ref   | 80.4 | 2000 | 104 | **r9** |
| short | plain | both  | 78.5 | 2000 | 104 | **r9** |
| none  | ptr  | mixed | 81.5 | 1996 | 96  | spill |
| none  | ptr  | ref   | 78.0 | 1988 | 96  | spill |
| none  | plain | mixed | 83.8 | 2000 | 104 | **r9** |
| none  | plain | ref   | 78.8 | 1988 | 104 | **r9** |

`a_volint` (C + `volatile int amount`) = 88.7 / 2004, frame 104, rem spill.
`H_volint_ptr_ref` (`volatile int amount` + pointer store) = 89.9 / 1996, but
its slot map is identical to HEAD (amount still a frame object at `sp+92`).

### 6.1 Declaration-order sweep (C base, 10 orders)

Moving `itemId`/`padA`/`amount`/`selected`/`type`/`guard`/`used` around (10
orders) leaves the score at **85.1** and `rem` in `r9` for every order except
`amount` before `itemId` (80.4).  Declaration order does not flip the RA here.

### 6.2 Else-arm statement order (6 permutations × 5 declaration forms)

All 30 combinations score **85.1 / 2008** (or 80.4 for the `itemId = 0`
declaration form); the compiler reorders `++difficulty; amount = 0;
itemId = 0;` freely, and `rem` stays in `r9`.

### 6.3 Volatile dummy stores (C base)

Adding a `volatile int padB` stored in the else arm, after `amount = 0`, or
read from `amount` after the switch does **not** flip `rem` to memory
(78.9 / 81.7 / 82.1); only making `amount` itself volatile/alloca does.

## 7. The `type == 8` arm is a second, independent symptom

HEAD still emits the wave-101 shape:

```
ours  974a4  mov  r5,r0          ; difficulty -> r5
      974a6  adds r0,r6,#5       ; amount = count+5
      974a8  b.n  97602          ; tail-merged store with case 11
orig  974a4  mov  r1,r0
      974a6  adds r0,r6,#5
      974a8  str  r0,[sp,#28]    ; inline store
      974aa  mov  r6,r1          ; difficulty -> r6
```

Because ours defers the store to the shared `str r0,[sp,#92]` block, it copies
`difficulty` to `r5` *before* the branch.  The original stores inline and only
then moves the `getTecLevel()` result into `r6`; that `r6` copy is what later
keeps `galaxy` out of `r6` and spills it to `sp+16`.  Tried and measured
(no change to the `mov r5,r0` + `b.n` shape):

* reorder to `amount = count + 5; difficulty = getTecLevel();` — 89.7 / 1996
* `int tecLevel = getTecLevel(); amount = count+5; difficulty = tecLevel;` — 86.9 / 2000
* volatile store `*ap = count + 5;` in the arm — 88.2 / 1996
* `(void) amount;` after the store — 90.0 / 1996 (store still tail-merged)

## 8. The `bhi` value-range fold is unchanged

`if ((uint32_t) type < 15 && ((1U << type) & 0x4801U) != 0)` still folds the
`type > 14` edge directly to `getName` (`bhi.n 0x242`), while the original
branches to the `race < 4 && type == 13` test (`bhi.n 0x1d0`).  Nesting the
two conditions, `type <= 14`, and `int type` all keep `0x242` and 90.0 %.

## 9. What would be needed for the last 10 %

A single source shape must simultaneously give:

1. `amount` a plain spill at `sp+28` (so `galaxy` falls to `sp+16`), and
2. the bonus `rem` a stack slot (so `agent` stays in `r9` and its `sp+24`
   spill slot dies at `97322`, letting `difficulty` reuse it), and
3. the `type == 8` arm an inline `amount` store with `difficulty` moved to
   `r6` after it.

(1) and (2) are the fixed point: the alloca that causes (2) prevents (1), and
the plain spill that gives (1) loses (2).  The original has no frame objects
beyond the canary and Strings, so the original source must achieve (2) without
an alloca — likely via the `type == 8` arm's `r6` difficulty copy (3), which
would then keep `r6` occupied at the `Globals::galaxy` load and force the
`galaxy` value to `sp+16`.  No spelling tried in waves 101/123/125/126 makes
clang 7.0.2 emit that inline store.

## 10. Reproduction

```sh
cp /dev/shm/w7631/base.cpp src/game/mission/Generator.cpp   # HEAD
cmake --build cmake-build-match --target check -j6           # RATCHET PASS, byte 4506
python3 /dev/shm/w7631/score.py                              # pct 90.0, usize 1996
```

Scratch (not committed): `/dev/shm/w7631/` — `base.cpp` = HEAD, `score.py` =
0.1 s single-function scorer (imports `tools/verify/verify.py`), `try.sh` =
compile+link+score, `C.cpp`/`a_volint.cpp`/`H_*.cpp`/`S_*`/`D_*`/`E_*`/`F_*`/
`G_*`/`I_*`/`J1.cpp` = measured variants, `C.mi`/`HEAD.mi` =
`-mllvm -print-machineinstrs` dumps.
