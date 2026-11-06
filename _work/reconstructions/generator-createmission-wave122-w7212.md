# `Generator::createMission` — wave 122 (w7212): 88.0 -> 89.9 %

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**).

```
before (HEAD 29cd275f6):  88.0 %  1996 B  frame 112  LSDA 172 B
after:                    89.9 %  1996 B  frame 104  LSDA 172 B (exact)
```

Full gate after the change (`check`, `lint`, `drift`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (lint CLEAN, drift 0)
```

`createMission` is not byte-exact yet, so `byte_exact` is unchanged at 4506/4524;
this pass only raises the per-function fuzzy score (88.0 -> 89.9) and shrinks the
frame from 112 to 104 bytes.  The `createMission` LSDA record stays **172 B**
(`cslen 151`, 26 rows, five pad=0 rows), and `.ARM.extab` size is unchanged.

## 1. The landed shape (2 hunks)

```diff
-    volatile long long padA;
+    volatile short padA;
...
-        *(volatile int *) &amount = 0;
+        volatile int *ap = &amount;
+        *ap = 0;
```

Both are register-allocation / stack-slot-colouring anchors, not behaviour:

* `padA` is a volatile temporary in the reward comma expression
  (`fromSystem = ..., padA = 0, (void) padA, fromSystemPtr = ..., reward ...`).
  Its width controls which stack slot the canary and the addressed objects land on.
  A 1- or 2-byte type (`short`, `char`, `bool`, `int16_t`, `uint8_t`, ...) all give
  the same code; `long long` gives frame 112 / 88.0 %, `short` gives frame 104 / 88.2 %.
* The `else` arm's `amount = 0` is a volatile store through a local pointer.  The
  pointer form makes clang schedule the store as `str.w sl,[sp,#92]` with `sl = 0`
  and, together with the `short padA`, reproduces the original's `movs r0,#0;
  cmp; str r0,[sp,#28]` block position.  The plain cast form
  (`*(volatile int *)&amount = 0`) scores 88.2 %; plain `amount = 0` scores 79.1 %
  (2000 B).

## 2. Measured variants

Base = HEAD shape (88.0 %, frame 112).  All 1996 B unless noted; `rec` is the
`createMission` LSDA record size.

### 2.1 padA type x amount-store spelling (full 4x4x2 sweep)

| padA | amount store | pct | size | rec |
|---|---|---|---:|---:|---:|
| **`short`** | **`volatile int *ap = &amount; *ap = 0;`** | **89.9** | **1996** | **172** |
| `short` | `volatile int &ar = amount; ar = 0;` | 89.9 | 1996 | 172 |
| `short` | `*(volatile int *) &amount = 0;` | 88.2 | 1996 | 172 |
| `short` | `amount = 0;` | 79.1 | 2000 | 172 |
| `long long` | pointer form | 89.8 | 1996 | 172 |
| `long long` | cast form | 88.0 | 1996 | 172 |
| `int` | pointer form | 84.9 | 1996 | 172 |
| `int` | cast form | 86.1 | 1992 | 172 |
| none | pointer form | 77.5 | 1988 | 172 |
| none | plain | 78.3 | 1988 | 172 |

`galaxy` hoisted to the top of the body vs declared at the reward expression makes
no difference (identical code).

### 2.2 padA width

`short`, `unsigned short`, `char`, `signed char`, `unsigned char`, `bool`,
`int16_t`, `uint16_t`, `int8_t`, `uint8_t` all score 89.9 % / 1996 B / 172 B.

### 2.3 branch-hint placement (16-cell sweep on the 89.9 % shape)

The wave-121 `__builtin_expect(race < 4 && type == 13, 1)` anchor is still required.
Adding `__builtin_expect` to `pick == 10` collapses the score to ~66.8 %; adding it
to `pick == 12` gives 89.1 %; adding it to the `0x4801` mask makes no difference.
Removing the race hint drops to 52-82 % and the LSDA to 168 B.

### 2.4 type==8 arm order

| arm spelling | pct | size | rec |
|---|---:|---:|---:|
| `difficulty = getTecLevel(); amount = count + 5;` (landed) | **89.9** | 1996 | 172 |
| `amount = count + 5; difficulty = getTecLevel();` | 89.7 | 1996 | 172 |
| `int tec = getTecLevel(); amount = count + 5; difficulty = tec;` | 86.8 | 2000 | 180 |
| `int tec = ...; difficulty = tec; amount = count + 5;` | 86.8 | 2000 | 180 |
| `Item *it = ...; difficulty = it->getTecLevel(); amount = count + 5;` | 87.5 | 2000 | 172 |

The `int tec` forms regress the LSDA to 180 B, so they are not usable.

### 2.5 reward expression

| reward shape | pct | size | rec |
|---|---:|---:|---:|
| local `galaxy` + `padA` (landed) | **89.9** | 1996 | 172 |
| `Globals::galaxy` used directly in both calls | 89.4 | 1996 | 172 |
| `Galaxy *&galaxy = Globals::galaxy;` | 89.4 | 1996 | 172 |
| `Galaxy **galaxy = &Globals::galaxy;` | 89.4 | 1996 | 172 |
| two aliases (`galaxy` + `galaxy2`) | 89.9 | 1996 | 172 |
| no `padA` | 77.5 | 1988 | 172 |
| `padA` store without the `(void)padA` load | 84.9 | 1996 | 172 |
| `padA` after `fromSystemPtr` | 89.5 | 1996 | 172 |

## 3. Remaining root cause (89.9 %)

Still ~433 differing lines in `verify --show`.  The residual is entirely
stack-slot colouring and one branch target:

1. **Frame 104 vs original 96.**  `amount` is an alloca (addressed by
   `volatile int *ap = &amount`) at `sp+92`; `padA` sits at `sp+98` and the canary
   at `sp+100`.  The original has no `padA` and keeps `amount` as an ordinary
   register-allocator spill slot at `sp+28` (sharing `targetStation`'s dead slot),
   with the canary at `sp+92`.  Any volatile local is a fixed frame object that
   `StackSlotColoring` cannot merge, so keeping the `padA` anchor necessarily costs
   a slot; removing it restores frame 96 but drops the score to ~77 %.
2. **`amount` at `sp+92` vs `sp+28`**, and consequently `galaxy` at `sp+28` vs
   `sp+16`, `bonus` at `sp+28` vs `sp+16`, `difficulty` at `sp+24` in both but in
   `r5` vs `r6`.
3. **`difficulty` in `r5` vs `r6`.**  In the original the type==8 arm stores
   `amount` inline (`str r0,[sp,#28]`) and only then moves `tecLevel` into `r6`
   (`mov r6,r1`); ours keeps `count` in `r6` and puts `difficulty` in `r5`.  Every
   source spelling tried (`int tec`, amount-first, `Item *it`, `nocount`) either
   keeps `r5` or regresses the LSDA.
4. **`type > 14` branch target.**  Ours `bhi.n +0x242` (straight to `getName`);
   the original `bhi.n +0x1d0` (through the `race < 4 && type == 13` test).  The
   compiler proves `type > 14` implies `type != 13` and folds the path; nesting the
   `type < 15` / `0x4801` tests, `__builtin_expect` on either half, and `int type`
   all leave the target unchanged.
5. **`galaxy`/`fromSystemPtr` spill swap.**  The original keeps `&Globals::galaxy`
   in `r5` and reloads the pointer for `getStation`, spilling the value to `sp+16`
   for `distance`; ours spills `galaxy` to `sp+28` and reloads it twice, keeping
   `systems->data` in `r5`.
6. Landing-pad branch targets differ by 2-4 bytes because the frame is 8 B larger.

## 4. Reproduction

```sh
cp /dev/shm/w7212/Generator.base.cpp src/game/mission/Generator.cpp
# apply the two hunks in section 1
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
python3 /dev/shm/w7212/score.py -v                   # pct 89.9, usize 1996
python3 _work/reconstructions/exidx_table.py --diff <orig> <ours>   # 172 B
```

Scratch variants: `/dev/shm/w7212/` (`best_899.cpp` is the landed source;
`grid18.py`..`grid29.py` are the sweeps).  Not part of the repo.
