# `Generator::createMission` — wave 124 (w7416): temp-index reward, 89.9 -> 90.0 %

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**).

```
before (HEAD 6f8ee2320):  89.9 %  1996 B  frame 104  LSDA 172 B (exact)
after:                    90.0 %  1996 B  frame 104  LSDA 172 B (exact)
```

Full gate (`check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas: `.text` **87,002** differing bytes (was 87,014; -12), `.ARM.exidx` **0**,
`.ARM.extab` **1,446** (size/base exact), `.rel.plt` **4**, `.note.gnu.build-id` **20**,
`.rodata` **0**, file size exact (2,187,328 B).

## 1. The landed change (4 lines)

```diff
-    Galaxy *galaxy = Globals::galaxy;
+    Galaxy *&galaxy = Globals::galaxy;
     int fromSystem;
+    int toSystem;
     SolarSystem *fromSystemPtr;
     float reward = (fromSystem = Globals::status->getStation()->getSystem(),
                     padA = 0, (void) padA,
                     fromSystemPtr = systems->data()[fromSystem],
-                    (galaxy->distance(fromSystemPtr, systems->data()[galaxy->getStation(targetStation)->getSystem()]) / 1200.0f + 1.0f) *
+                    toSystem = galaxy->getStation(targetStation)->getSystem(),
+                    (galaxy->distance(fromSystemPtr, systems->data()[toSystem]) / 1200.0f + 1.0f) *
                    (float) ((int) ((float) difficulty / 10.0f * 5500.0f) + 1500));
```

## 2. Why it works: the second `systems->data()` is no longer CSE'd

Wave 123 identified that the original loads `systems->data_` **twice** (at `97628` and
`9763c`), while HEAD kept one load live across `Galaxy::getStation` and reused it.  The
`-print-after-all` IR dumps pin the merge to **Early CSE w/ MemorySSA**:

* at the `Function Integration/Inlining` dump the reward has two loads:
  `%365 = load systems->data_` (for `fromSystemPtr`) and `%369 = load systems->data_`
  (for the distance's second system), with **no call between them**;
* `Early CSE w/ MemorySSA` trivially merges `%369` into `%365` (same block, no clobber),
  and the merged value then has to stay live across the two invokes.

The reason there is no call between the two loads is C++ evaluation order: in
`systems->data()[galaxy->getStation(...)->getSystem()]` clang evaluates the subscript base
(`systems->data()`) **before** the index expression.  Hoisting the index into a named local
inside the same comma expression

```
fromSystemPtr = systems->data()[fromSystem],
toSystem = galaxy->getStation(targetStation)->getSystem(),
```

forces the second `data()` load to happen **after** `Galaxy::getStation`/`Station::getSystem`.
The intervening invokes are memory clobbers, so Early CSE cannot merge the loads and the
original's two-load shape is recovered.

The switch from `Galaxy *galaxy` to `Galaxy *&galaxy` keeps the *address* `&Globals::galaxy`
in `r5` (the original's `ldr r5,[GOT]; ldr r0,[r5]` idiom) instead of spilling the value.

## 3. Resulting reward region (linked disassembly)

```
ours (90.0)                                    original
97618 ldr r1,[pc]                             9760a ldr r0,[pc]
97624 add r1,pc                               9760e add r0,pc
97626 ldr.w r2,[r8,#4]   ; data_ #1           97610 ldr r5,[r0,#0]   ; &Globals::galaxy
9762a ldr r5,[r1,#0]     ; &Globals::galaxy   97612 ldr r0,[r5,#0]   ; value
9762c ldr.w r6,[r2,r0,lsl#2] ; fromSystemPtr  97614 str r0,[sp,#16]
97630 ldr r0,[r5,#0]                          97628 ldr.w r1,[r8,#4] ; data_ #1
97634 blx Galaxy::getStation                  9762c ldr.w r6,[r1,r0,lsl#2]
97638 blx Station::getSystem                  97630 ldr r0,[r5,#0]
9763c ldr.w r1,[r8,#4]   ; data_ #2           97634 blx Galaxy::getStation
97640 ldr.w r2,[r1,r0,lsl#2]                  97638 blx Station::getSystem
97644 ldr r0,[r5,#0]                          9763c ldr.w r1,[r8,#4] ; data_ #2
97648 blx Galaxy::distance                    97640 ldr.w r2,[r1,r0,lsl#2]
                                              97644 ldr r0,[sp,#16]
                                              97648 blx Galaxy::distance
```

Both now keep `&Globals::galaxy` in `r5`, `fromSystemPtr` in `r6`, and issue two
`systems->data_` loads.  The only remaining reward delta is that ours reloads the galaxy
value from `r5` for `distance` where the original reloads it from the `sp+16` spill.

## 4. Measured variants

Base = HEAD wave-123 shape (`volatile short padA` + `volatile int *ap = &amount`).

| variant | pct | size | frame |
|---|---:|---:|---:|
| **`Galaxy *&` + named `toSystem` (landed)** | **90.0** | **1996** | **104** |
| same but `Galaxy **galaxy = &Globals::galaxy` | 90.0 | 1996 | 104 |
| same but `systems->operator[](toSystem)` | 90.0 | 1996 | 104 |
| same but `*(systems->data() + toSystem)` | 90.0 | 1996 | 104 |
| same but two locals (`toSystemPtr = systems->data()[toSystem]`) | 90.0 | 1996 | 104 |
| `Galaxy *galaxy` + named `toSystem` (no reference) | 88.6 | 1996 | 104 |
| `Galaxy *const galaxy` + named `toSystem` | 88.6 | 1996 | 104 |
| named `toSystem` before the reward statement | 88.9 | 2000 | 104 |
| `Galaxy *&` + `toSystemPtr = systems->data()[...]` temp | 73.9 | 2016 | 104 |
| `toSystem` as `uint32_t` / `unsigned int` | 90.0 | 1996 | 104 |
| `type` as signed `int` | 90.0 | 1996 | 104 |
| `race` as signed `int` | 89.7 | 1996 | 104 |
| `volatile short padA` -> `char` | 90.0 | 1996 | 104 |
| `volatile short padA` -> `int` | 84.9 | 1996 | 104 |
| no `padA` | 78.0 | 1988 | 96 |
| `*(volatile int *)&amount = 0` instead of pointer | 88.2 | 1996 | 104 |
| `volatile int amount` | 88.1 | 1996 | 104 |

Plain `amount = 0` (no alloca) fixes the else arm (`str r0,[sp,#28]`, `itemId=0` after the
switch) but the allocator then moves the guard to `sp+16`, spills `agent` to `sp+32` and
`difficulty` to `sp+20`, so the whole slot base shifts +4 and the score falls to ~80.  The
volatile-pointer `amount` alloca is what keeps the guard at `sp+20` and the slots matching.

## 5. Remaining root cause (90.0 %)

1. **Frame 104 vs 96.** `amount` is still an alloca at `sp+92` (`volatile int *ap =
   &amount`) plus the `volatile short padA` at `sp+98`; the original has `amount` as a
   spill slot at `sp+28` and no `padA`.
2. **Else-arm `amount=0`** stores to `sp+92` (32-bit `str.w`) rather than `sp+28`, and
   `itemId=0` is materialised in `sl` before the switch instead of `r0`/after it.
3. **`type == 8` arm** keeps `difficulty` in `r5` and defers the `amount` store to the
   shared merge block; the original stores `amount` inline and moves tecLevel to `r6`.
4. **`type > 14` branch target** ours `bhi.n 0x242` (skips the `race < 4 && type == 13`
   test, value-range fold) vs original `bhi.n 0x1d0`.
5. **Galaxy value spill** ours reloads `[r5]` for `distance`; original reloads the `sp+16`
   spill.

## 6. Reproduction

```sh
cp /dev/shm/w7416/Generator.base.cpp src/game/mission/Generator.cpp   # HEAD wave-123 shape
# apply the 4-line diff in section 1
cmake --build cmake-build-match --target check -j6                    # RATCHET PASS, byte 4506
python3 /dev/shm/w7416/score.py                                       # pct 90.0, usize 1996
```

Scratch variants: `/dev/shm/w7416/` (`Generator.base.cpp` is the landed wave-123 source;
`sweep*.py`, `matrix.py`, `decl.py`, `score.py`, `afterall.txt`).  Not part of the repo.
