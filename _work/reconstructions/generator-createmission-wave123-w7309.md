# `Generator::createMission` — wave 123 (w7309): frame-slot / `systems->data()` CSE analysis

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**).

Status: **research note, no source change landed.**  HEAD keeps the wave-122
shape (`volatile short padA` + `volatile int *ap = &amount`) at **89.9 % /
1996 B / frame 104 / LSDA 172 B**.  This pass re-derived the frame layout and
the register-allocation cascade from the linked disassembly and the clang
`-emit-llvm` / `-print-machineinstrs` IR, and measured ~250 further source
variants.  No shape beats 89.9 % while keeping 1996 B and the 172 B record.

```
before (HEAD 1c8c9b20e):  89.9 %  1996 B  frame 104  LSDA 172 B (exact)
after:                    89.9 %  1996 B  frame 104  LSDA 172 B (exact)
```

Full gate (`check`, `lint`, `drift`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (lint CLEAN, drift 0)
```

## 1. Frame objects (from `-print-machineinstrs`, last pass)

HEAD `createMission` frame objects:

```
fi#0: size=4,  align=4   <- amount  (volatile-pointer alloca)
fi#1: size=2,  align=2   <- padA    (volatile short)
fi#2: size=4,  align=4   <- stack canary
fi#3: size=12, align=4   <- String 1
fi#4: size=12, align=4   <- String 2
fi#5: size=12, align=4   <- String 3
fi#6: size=12, align=4   <- String 4
```

The six ordinary int spills (`&__stack_chk_guard`, agent/difficulty,
targetStation/galaxy/bonus, status, race/type, rnd) are spill slots at
`sp+20..40`.  Together: `4+2+4+48` frame bytes + 24 spill bytes = 82, padded to
**104**.

The original has **no frame objects except the canary and the four Strings**
(`4+48` = 52) plus **seven** int spill slots (28) = 80, padded to **96**.  The
original's extra slot is `sp+16 = galaxy/bonus`; the Strings still start at
`sp+44` in both.  So the whole 8-byte delta is exactly:

* one extra spill slot (`fromSystemPtr` at `sp+20` in the no-alloca shape), and
* the 4-byte pad that follows from it.

```
original (96)                     HEAD (104)
sp+16  galaxy / bonus             sp+16  (unused)
sp+20  &__stack_chk_guard         sp+20  &__stack_chk_guard
sp+24  agent / difficulty         sp+24  agent / difficulty
sp+28  targetStation / amount     sp+28  targetStation / galaxy / bonus
sp+32  status                     sp+32  status
sp+36  race / type                sp+36  race / type
sp+40  rnd                        sp+40  rnd
sp+44..92  four Strings           sp+44..92  four Strings
sp+92  canary                     sp+92  amount alloca
                                  sp+98  padA
                                  sp+100 canary
```

The Strings are at `sp+44/56/68/80` in **both** (verified by the
`add r0, sp, #44/56/68/80` sites), so the wave-122 `padA` is *not* shifting the
String area; it is only adding the alloca region past `sp+92`.

## 2. The cascade: `amount` slot -> `galaxy` slot -> frame

`amount` is at `sp+28` in the original because it reuses `targetStation`'s dead
slot.  `galaxy` then takes the next free slot, `sp+16`.  In HEAD, `amount` is a
fixed alloca at `sp+92`, so `targetStation`'s `sp+28` is free and `galaxy`
takes **it**, leaving `sp+16` empty.

If `amount` is made an ordinary variable (plain `amount = 0`), it *does* reuse
`targetStation`'s slot, but the compiler then spills `fromSystemPtr` to a new
`sp+20` slot, which pushes the String area up by 8.  Net: frame stays **104**.

Removing `padA` alone restores frame **96** but the slot base drops by 4
(`&__stack_chk_guard` moves `sp+20 -> sp+16`, everything shifts `-4`) and the
score falls to **77.5 %** (`amount` alloca at `sp+88`, six int slots).

## 3. Why `fromSystemPtr` spills: `systems->data()` is CSE'd

The reward expression in the IR is:

```
%350 = load  systems->data_          ; ONE load
%352 = load  systems->data_[fromSystem]
%353 = invoke Galaxy::getStation(...)
...
%357 = getelementptr %350, system    ; reuses %350
%358 = load  systems->data_[system]
%359 = invoke Galaxy::distance(...)
```

`systems` is `%class.Array.10* nocapture readonly`, so GVN keeps the
`systems->data_` field load live across the `getStation`/`getSystem` calls.
That pins `r5` for the whole reward and forces `fromSystemPtr` to memory.
The original reloads the field:

```
97628: ldr.w r1, [r8, #4]   ; systems->data_
9762c: ldr.w r6, [r1, r0, lsl #2]   ; fromSystemPtr -> r6
...
9763c: ldr.w r1, [r8, #4]   ; systems->data_ again
97640: ldr.w r2, [r1, r0, lsl #2]
```

so `r6` holds `fromSystemPtr` and `r5` holds `&Globals::galaxy`.  Forcing a
second load with a `volatile` read of `systems->count` / `systems->data_`
between the two uses did **not** change the reward codegen (still one `r5`
load) and only added instructions (76.7 / 78.3 %).

Note the loop at `973c4..973f4` *does* reload `systems->data_` after
`stationIsInSystem` in both original and HEAD (that region is byte-equal), so
the CSE is specific to the reward, where the intervening call is on `galaxy`
rather than on `systems->data_[i]`.

## 4. Galaxy load placement (IR-level)

`Galaxy *galaxy = Globals::galaxy;` (HEAD) emits the `Globals::galaxy` load
**before** the `Globals::status` load, matching the original's `97610` position.
Using `Globals::galaxy` directly moves the load after `getSystem` (the IR
diff), which is why direct scores 89.4 rather than 89.9.  The local form is the
correct one for load placement; the remaining difference is only that the
backend keeps the *value* (spilled to `sp+28`) instead of the *address*
(`r5 = &Globals::galaxy`, value reloaded).

The wave-119 "local `galaxy` for `distance` + `Globals::galaxy` for
`getStation`" pattern does reproduce the original `ldr r5,[&Globals::galaxy];
ldr r0,[r5]; str r0,[sp,#16]` sequence, but on the current shape it scores
**76.2 % / 2016 B** (it makes `type` register-resident and re-shuffles the
whole prologue).

## 5. Other measured variants (all on the HEAD shape unless stated)

| variant | pct | usize | frame |
|---|---:|---:|---:|
| **HEAD (padA short + volatile-pointer amount)** | **89.9** | **1996** | **104** |
| no padA, plain amount, no `itemId=0` | 88.4 | 1980 | 96 |
| no padA, plain amount, `itemId=0` before if | 78.3 | 1988 | 104 |
| no padA, pointer amount, `itemId=0` | 77.5 | 1988 | 96 |
| no padA, `volatile int amount`, `itemId=0` | 76.5 | 1984 | 96 |
| padA short + `volatile int amount` | 88.0 | 1996 | 104 |
| padA int + volatile-pointer amount | 84.9 | 1996 | 104 |
| padA long long + volatile-pointer amount | 89.8 | 1996 | 112 |
| direct `Globals::galaxy` (both uses) | 89.4 | 1996 | 104 |
| `Galaxy *&galaxy` / `Galaxy **galaxy` | 89.4 | 1996 | 104 |
| local/direct galaxy (wave-119 pattern) | 76.2 | 2016 | 104 |
| `int race` / `int selected` | 89.7 / 89.8 | 1996 | 104 |
| `uint32_t guard` | 59.2 | 1988 | 104 |
| reward chain `9,7,...` | 89.5 | 1996 | 104 |
| mask-first `if ((mask) && type<15)` | 67.2 | 1972 | 104 |
| nested `if (type<15) if (mask)` | 89.9 | 1996 | 104 |
| `bool inMask = ...; if (inMask)` | 89.9 | 1996 | 104 |
| `type <= 14` instead of `< 15` | 89.9 | 1996 | 104 |
| else-arm order `++diff; amount=0; itemId=0` (all 5 orders) | 89.9 | 1996 | 104 |
| `itemId=0` at declaration / in else / at end of else | 78.3 / 78.3 / 86.1 | 1988 / 1988 / 1944 | 104 / 104 / 96 |
| function-scope `items` (store-in-arm) | 85.5 | 1996 | 104 |
| `volatile` galaxy / `Galaxy *const` | 78.7 / 89.9 | 1996 | 104 |
| random 400-cell search over padA x amount x itemId x galaxy x race x selected x t8 | best **89.9** | 1996 | 104 |

## 6. Remaining root cause

1. **`amount` is an alloca, not a spill.**  The `volatile int *ap = &amount`
   store is what reproduces the wave-122 block order, but it makes `amount` a
   frame object (`fi#0`), so it cannot reuse `targetStation`'s `sp+28`.  Every
   non-alloca spelling restores the reuse but adds the `fromSystemPtr` spill.
2. **`systems->data_` field CSE.**  Because `systems` is `nocapture readonly`,
   GVN keeps one `systems->data_` load live across `Galaxy::getStation`, which
   occupies `r5` and forces `fromSystemPtr` to `sp+20`.  The original has two
   field loads.  A `volatile` read between the uses did not reproduce it.
3. **`galaxy` value vs address.**  The local form gives the right load
   placement, but the backend spills the value to `sp+28`; the original keeps
   `&Globals::galaxy` in `r5` and reloads.  This is a backend rematerialisation
   tie-break, not reachable by the declaration/`const`/`volatile` spellings
   tried.
4. **`type > 14` branch target.**  Ours `bhi.n loc_242` (direct to `getName`);
   the original `bhi.n loc_1d0` (through the `race < 4 && type == 13` test).
   Only the mask-first reorder produces `loc_1d0`, but it changes the mask
   codegen (`ands` instead of `tst`) and the prologue register assignment
   (67.2 %).
5. **`type == 8` store-in-arm.**  The original stores `amount` inline
   (`str r0,[sp,#28]`) and then moves `difficulty` to `r6`; ours keeps
   `difficulty` in `r5` and defers the store to the merge.  Only a
   function-scope `items` temp forces the inline store, and that costs the
   reward's `Globals::items` uses (85.5 %).

## 7. Reproduction

```sh
cp /dev/shm/w7309/base.cpp src/game/mission/Generator.cpp   # HEAD wave-122 shape
cmake --build cmake-build-match --target check -j6           # RATCHET PASS, byte 4506
python3 /dev/shm/w7309/score2.py                             # pct 89.9, usize 1996, frame 104
```

Scratch variants: `/dev/shm/w7309/` (`base.cpp` is the landed source;
`sweep*.py`, `search.py`, `dump.py`, `align.py`, `ir_test*.py`,
`ir_diff.py`).  Not part of the repo.
