# `Generator::createMission` — wave 125 (w7515): `amount`/`padA` slot forms

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**).

Status: **research note, no source change landed.**  HEAD (wave 124) stays at
**90.0 % / 1996 B / frame 104 / LSDA 172 B**.  This pass re-derived the frame
slot map from the linked disassembly and the clang `-mllvm -print-machineinstrs`
spill-slot dump, and measured ~150 further source variants of the
`amount`/`padA`/`galaxy` declarations.  Nothing beats 90.0 % without losing the
172 B record or the 1996 B size.

```
before (HEAD 02513de33):  90.0 %  1996 B  frame 104  LSDA 172 B (exact)
after:                    90.0 %  1996 B  frame 104  LSDA 172 B (exact)
```

Full gate (`check`) on the restored HEAD:

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas (unchanged): `.text` **85,975** differing bytes (size exact
`0x1337c8`); `.ARM.exidx` **0**; `.ARM.extab` **1,474** (size/base exact,
`0x26298`); `.rodata` **0**; `.rel.plt` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**.  File size exact (2,187,328 B).

## 1. The original frame is 7 spill slots and *no* extra frame object

Original slot map (verified against the linked disassembly):

```
sp+16  galaxy (97614 store, 97644 load) / bonus (976d8, 97720, 9783a)
sp+20  &__stack_chk_guard (971f6 store, 978b2 load)
sp+24  agent (97220 store, 97306/97322 load) / difficulty (9761c store, 9764c load)
sp+28  targetStation (97218/9721c store, 97306/97322 load) / amount
sp+32  status (97228 store, 9761a load) / mission (9775e store, 97824 load)
sp+36  race (97228 store, 9726e load) / type
sp+40  rnd (97244 store)
sp+44..92  four `AbyssEngine::String` temporaries
sp+92  stack canary
```

Total = 16 (outgoing args) + 28 (7 slots) + 48 (Strings) + 4 (canary) = **96**.
There is **no** alloca in the original: every local is a register-allocator
spill slot, so `amount` is a plain `int` and `galaxy` is a plain pointer.

HEAD instead has 7 slot *positions* (16..44, with `sp+16` unused), the four
Strings at 44..92, then `amount` as a `volatile int *ap = &amount` alloca at
`sp+92`, the `volatile short padA` at `sp+98`, and the canary at `sp+100`
(frame 104).  The extra frame objects sit *above* the Strings, so they do not
disturb the 16..44 slot map — that is exactly why the wave-122 anchors work.

## 2. The clean `plain amount` shape is byte-identical in the else arm but 8 slots

`int amount;` + `amount = 0;` in the else arm (no padA, `Galaxy *galaxy` value
with `Globals::galaxy->getStation` for the inner call — the "mixed" galaxy form
from wave 124) reproduces the original's `amount` schedule exactly:

```
ours (none/int/plain/mixed, 83.8 %)          original
adds r6,r0,#1                                adds r6,r0,#1
movs r0,#0                                   movs r0,#0
cmp  r1,#11                                  cmp  r1,#11
str  r0,[sp,#32]   ; amount = 0              str  r0,[sp,#28]   ; amount = 0
bhi.n 0x360                                  bhi.n 0x360
mov.w sl,#0                                  mov.w sl,#0
tbb  [pc,r1]                                 tbb  [pc,r1]
...
adds r0,r6,#5                                adds r0,r6,#5
str  r0,[sp,#32]   ; amount = count+5        str  r0,[sp,#28]
mov  r6,r1                                   mov  r6,r1
b.n  0x452                                   b.n  0x452
```

The reward region is also correct (galaxy value spilled, `fromSystemPtr` in r6,
`&Globals::galaxy` in r5, two `systems->data_` loads).  But the frame is
**104** with **8** spill slots:

```
ours  sp+20 galaxy/bonus, 24 guard, 28 difficulty, 32 target/amount,
      36 agent, 40 status, 44 race, 48 rnd   (sp+16 unused)
orig  sp+16 galaxy/bonus, 20 guard, 24 agent/difficulty, 28 target/amount,
      32 status, 36 race, 40 rnd
```

So `difficulty` did **not** get merged into `agent`'s slot.  That single failed
merge is the whole 8-vs-7 delta (and the +8 B frame).

## 3. Root cause: `rem` steals `r9`, so `agent` is spilled across the reward

`-mllvm -print-machineinstrs` (After Virtual Register Rewriter → After Stack
Slot Coloring) shows why:

* `none/int/ptr/mixed` (amount alloca, frame 96): agent's spill slot is stored
  at 97220 and reloaded only at 97306/97322 — dead afterwards, because `r9`
  keeps `agent` for the whole function.  The int `rem` of the bonus computation
  is spilled to its own slot (`%stack.17`).  `difficulty` then merges into
  `agent`'s slot → 6 slots.
* `none/int/plain/mixed` (amount spill, frame 104): the int `rem` is kept in
  `r9` (`mov r9,r1` after the first `__aeabi_idivmod`), so `agent` has to be
  re-spilled and is reloaded from its slot at the `Mission` constructor.
  Agent's slot live range then overlaps `difficulty`'s, so StackSlotColoring
  cannot merge them → 8 slots.

In other words the `amount` alloca (wave-122 anchor) is not just padding: it
changes the register pressure enough that `rem` goes to memory instead of `r9`,
which is what lets `agent` stay in `r9` and `difficulty` share its slot.  Every
attempt to make `amount` a real spill slot loses that property.

## 4. Measured variants (fast harness, one `link_so` per row)

Base = HEAD (`volatile short padA` + `volatile int *ap = &amount` +
`Galaxy *&galaxy`).  All rows keep 1996 B unless noted.

| variant (padA / amount / else-store / galaxy) | pct | usize | frame |
|---|---:|---:|---:|
| **HEAD** (`short` / `int` / ptr / `*&`) | **90.0** | 1996 | 104 |
| `short` / `int` / ptr / value+mixed | 87.6 | 2008 | 104 |
| `short` / `int` / cast / mixed | 88.8 | 2004 | 104 |
| `short` / `int` / plain / mixed | 85.1 | 2008 | 104 |
| `short` / `int` / ptr / value (`val`) | 88.6 | 1996 | 104 |
| `int` / `int` / cast / mixed | 89.2 | 2000 | 104 |
| `int` / `int` / ptr / mixed | 89.1 | 2000 | 104 |
| `none` / `int` / plain / mixed | 83.8 | 2000 | 104 |
| `none` / `int` / ptr / mixed | 81.5 | 1996 | **96** |
| `none` / `int` / cast / mixed | 81.6 | 1992 | **96** |
| `none` / `volatile int` / plain / mixed | 81.6 | 1992 | **96** |
| `none` / `int` / plain / `*&` | 78.8 | 1988 | 104 |
| `none` / `int` / ptr / `*&` | 78.0 | 1988 | **96** |
| `short` / `int` / plain / `*&` | 80.4 | 2000 | 104 |
| `long long` / `int` / ptr / `*&` | 89.9 | 1996 | 104 |
| `bool`/`char` / `int` / ptr / `*&` | 90.0 | 1996 | 104 |
| `none` / `int` / plain / mixed + hoisted `costs` | 82.1 | 2000 | 104 |
| `short` / `int` / plain / mixed + hoisted `costs` | 84.2 | 2008 | 104 |
| `none` / `int` / plain / mixed + hoisted `mission`/`bonus`/`rem` | 83.8 | 2000 | 104 |
| `short` / `int` / plain / mixed + hoisted `mission` | 85.1 | 2008 | 104 |
| `none` / `int` / plain / mixed + `int amount = 0;` at decl | 83.8 | 2000 | 104 |
| `none` / `int` / plain / mixed + `itemId = 0` moved into else | 83.8 | 2000 | 104 |
| `none` / `int` / plain / mixed + `itemId = 0` after the if | 56.6 | 1920 | 96 |
| `none` / `int` / plain / mixed + `galaxy` hoisted to top | 65.8 | 1984 | 104 |
| two aliases (`galaxy` + `galaxy2`) on HEAD shape | 88.6 | 1996 | 104 |
| `none` / `int` / plain / mixed + hoisted int `rem` | 83.8 | 2000 | 104 |

Random 48-cell sample over
`padA ∈ {none,short,int,char} × amount ∈ {int,volatile} ×
store ∈ {plain,ptr,cast,none} × galaxy ∈ {ref,val,const,mixed} ×
hoist ∈ {none,costs,mission,bonus,rem,level}`: best 90.0 % (the HEAD shape).

## 5. What would be needed for a real win

1. `amount` must be an ordinary (non-addressed, non-volatile) `int` so it can
   reuse `targetStation`'s `sp+28` — the plain `amount = 0` store already
   reproduces the original else arm byte-for-byte.
2. `galaxy`'s *value* must be spilled to a fresh slot **below** the guard
   (`sp+16`).  `short`/`plain`/`mixed` does put galaxy at `sp+16`
   (frame-104, 85.1 %), but still with 8 slots because of (3).
3. `agent` must stay in `r9` across the reward so its spill slot dies at
   97306 and `difficulty` can share `sp+24`.  In every plain-`amount` shape the
   int `rem` of the bonus computation takes `r9` instead, re-spilling `agent`
   at the `Mission` constructor and blocking that merge.

(1)+(2)+(3) together would give the original's 7-slot 96-byte frame.  The
`r9` tie-break is a Greedy-RA spill-weight decision (agent: long live range,
two uses after the early spill; `rem`: short live range, two uses), and no
declaration order / `volatile` / `const` / hoist spelling tried so far moves it.

## 6. Reproduction

```sh
cp /dev/shm/w7515/Generator.base.cpp src/game/mission/Generator.cpp   # HEAD wave-124 shape
cmake --build cmake-build-match --target check -j6                    # RATCHET PASS, byte 4506
python3 /dev/shm/w7515/fastscore.py                                   # pct 90.0, usize 1996, frame 104
```

Scratch harness (not committed): `/dev/shm/w7515/`
(`Generator.base.cpp` = HEAD source, `fast.sh`/`fastscore.py` = 1 s scorer,
`score.sh` = full verifier, `mi.sh` = `-mllvm -print-machineinstrs` dump,
`gen4.py`..`gen8.py` = variant generators, `sweep4.sh`..`sweep6.sh`,
`sweep3.txt`, `rand.txt`/`randres.txt`).
