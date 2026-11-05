# `Generator::createMission` — wave 115 (w6309): masked type test vs. the store merge

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**).

Status: **research note, no source change landed.**  The wave-114 shape
(`volatile double padA` next to `volatile int amount`, `__builtin_expect(pick == 10, 1)`,
`__builtin_expect(pick == 12, 0)`, `__builtin_expect(((1U << type) & 0x4801U) != 0, 1)`)
remains the unique best variant: **83.3 % official / 1996 B / extab 168 B**.

Gate at the start and end of this pass (full `check` + `lint` + `drift`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.ARM.extab` 0x26298 — all unchanged.
`git status --short` is empty after the note commit.

## 1. New finding: `(type & 0xff) == 13` fixes the `type < 15` branch target

The single most visible source-shape difference in the whole function is the
false target of the `type < 15` test.  Original (`0x97356`):

```
97356: ldr  r1, [sp, #36]     ; type
97358: cmp  r1, #14
9735a: bhi.n 97388            ; type >= 15 -> the race < 4 block
...
97388: cmp.w fp, #3           ; race < 4
9738c: bhi.n 973fa            ; race >= 4 -> getName
9738e: ldr  r0, [sp, #36]     ; type
97390: cmp  r0, #13
97392: bne.n 973fa
```

Ours (`0x9735c`) targets `getName` (`0x9740c`) instead, because LLVM's
`JumpThreading` proves `type >= 15  =>  type != 13` and deletes the
`type<15 -> race-check` edge at the IR level.  The `br i1` is already
`label %119, label %177` (getName) in the post-`Early CSE` IR; the change is
made by the first `Jump Threading` pass.

The only plain-C++ spelling found that **defeats** that proof is a masked
compare:

```c++
if (race < 4 && (type & 0xff) == 13) { ... }
```

The IR keeps `icmp ult i32 %type, 15` and a separate
`and i32 %type, 255; icmp eq i32 %and, 13`, so the two conditions are no longer
the same SSA value and `JumpThreading` cannot correlate them.  The resulting
`bhi.n` targets the race-check block exactly like the original:

```
97348: cmp  r5, #14
9734a: bhi.n 97378            ; -> cmp.w fp,#3   (race < 4)
```

`(uint8_t) type == 13`, `type % 16 == 13`, `(type & 0x1f) == 13` and
`(uint32_t) (type & 0xff) < 15` all behave the same way.

The catch: the `and`/`uxtb` forces `type` into a register, and the register
allocator then keeps it there for the rest of the function.  `type` no longer
lives in `[sp,#36]`, `systems` moves from `r8` to `r4`, `targetStation` from
`r5/r4` to `r8`, and the shared `type` store disappears entirely (a single
`str r5,[sp,#36]` after `getName`).  The official score drops to **64.4 %**
(1972 B, extab 168).  Combining the mask with every frame/`amount`/`padA`/
`pick==12` hint variant measured still stays <= 64.9 %.

| variant | pct | size | extab |
|---|---:|---:|---:|
| **baseline (wave 114)** | **83.3** | 1996 | 168 |
| `(type & 0xff) == 13` | 64.4 | 1972 | 168 |
| `(uint8_t) type == 13` | 64.4 | 1972 | 168 |
| `(type & 0x1f) == 13` | 64.3 | 1972 | 168 |
| `type % 16 == 13` | 65.4 | 1980 | 168 |
| `(uint32_t) (type & 0xff) < 15 && ...` | 64.2 | 1972 | 168 |
| `(type & 0xff) == 13` + no `pick==12` hint | 64.9 | 1972 | 168 |
| `(type & 0xff) == 13` + no mask hint | 64.4 | 1972 | 168 |
| `(type & 0xff) == 13` + `int amount` | 59.7 | 1976 | 168 |
| `(type & 0xff) == 13` + no `padA` | 60.1 | 1960 | 168 |
| `(type & 0xff) == 13` + `volatile int32_t type` | 53.8 | 2040 | 176 |

The winning shape would have to keep the masked compare (for the branch target)
*and* keep `type` spilled to `[sp,#36]` (for the register allocation).  No
spelling tried this pass achieves both.

## 2. Why the store still lands on the `type = 8` arm

The shared `type` store is the post-placement `BranchFolder` inside
`MachineBlockPlacement::runOnMachineFunction` (wave-113 finding, re-confirmed).
The merged block `M` is placed after `bb.32` (type 8) because
`hasBetterLayoutPredecessor(bb.32, M)` is false:

```
PredEdgeFreq  = freq(bb.48 type13) * 1.0
CandEdgeFreq  = freq(bb.32 type8)  * 1.0
conflict iff  PredEdgeFreq * 0.8 >= CandEdgeFreq * 0.2
          iff  freq(type13) >= freq(type8) / 4
```

In the current MIR (`-mllvm -print-machineinstrs=block-placement`):

```
bb.31 getOffer   freq 1.0    succ bb.32 (0.5), bb.33 (0.5)
bb.32 type8      freq 0.5    succ bb.49 (store)
bb.48 type13     freq 0.0025 succ bb.49 (store)
bb.49 store      freq 0.578
```

so `freq(type13)` is ~50x short of the 0.125 threshold.  The path to `bb.48`
is `0.5 (offer!=5) * 0.5 (type<15) * 1/32 (while targetStation) * 0.75
(race<4) * 2/3 (type==13) * 0.625 (routes!=null) * 1/32 (while !found)`;
the two loop back-edge heuristics (`124:4`) dominate.  The exit frequency of
`while (!found)` equals its entry frequency, so changing that loop's branch
probability does not help; the `while (targetStation == ...)` loop would have
to disappear for the ratio to cross 1/4.

`__builtin_expect` on `offer == 5`, `race < 4`, `type == 13`, `type < 15`,
`getRoutes() == nullptr` or `while (!found)` either leaves the emitted body
byte-identical or outlines the `type = 8` arm.  Measured this pass:

| hint | pct | size | store |
|---|---:|---:|---|
| baseline | 83.3 | 1996 | 0x19c (after type8) |
| `expect(offer == 5, 0)` | 63.4 | 1972 | outlined |
| `expect(offer == 5, 1)` | 38.8 | 2052 | outlined |
| `expect(race < 4 && type == 13, 0)` | 44.4 | 2036 | 0x19c |
| `expect(race < 4 && type == 13, 1)` | 64.0 | 1968 | shared, `type` in r5 |
| `expect(type == 13, 1)` | 83.3 | 1996 | 0x19c (identical) |
| `expect(type == 13, 0)` | 44.4 | 2036 | 0x19c |
| `expect(race < 4, 0/1)` | 83.3 | 1996 | 0x19c (identical) |

## 3. LSDA: the missing 4 bytes are the outlined null block

Original call-site table (26 entries, `tlen` 151, record 172 B) vs ours
(25 entries, `tlen` 147, record 168 B):

```
orig 6: (944, 24, 1878, 0)   [0x97568, 0x97580)   case-15 try-range
orig 7: (968, 32,    0, 0)   [0x97580, 0x975A0)   lp=0 gap  <-- extra
orig 8: (1000, 56, 1934, 0)  [0x975A0, 0x975D8)   case-0 invoke range
ours 6: (960, 24, 1876, 0)   [0x97578, 0x97590)
ours 7: (994, 54, 1896, 0)   [0x9759A, 0x975D0)   one merged range
```

The extra `lp=0` entry exists because the original places the
`getRoutes() == nullptr` block (`type = 4; if (nextInt(2) == 0) type = 1;`)
at **`0x97586`**, in the middle of the `switch (type)` case bodies, between
the case-15 try-range and the case-0 invoke range.  Its `nextInt(2)` call is
therefore a throwing call between two try-ranges and gets its own call-site
entry.

Ours places the same block at `0x973f8`, immediately before `getName`, i.e.
inside the first `lp=0` gap `[0x971b8, 0x97416)`, so it is merged into that
entry and costs nothing.  The null block's placement is a direct consequence
of the store merge (wave-113 §5): once `M` follows the `type = 13` arm, the
cold `getRoutes()` block is selected after the `switch` arms instead.

Note that fixing this record to 172 B would grow `.ARM.extab` from `0x26298`
to `0x2629c`, because `Hud::draw`'s record is currently +4 (480 vs 476) and
cancels the shortfall.  Any landing must either accept that section change or
fix `Hud::draw` in the same pass.

## 4. Other measured variants (all 1996 B / extab 168 unless noted)

| variant | pct | notes |
|---|---:|---|
| **baseline** | **83.3** | store 0x19c, null `cbz` 0x240 |
| `&typeRef` / `&typePtr` / `bool needReroute = ...` | 83.3 | SROA promotes; IDENTICAL |
| `13 == type`, `type - 13 == 0`, `type == 13 && race < 4` | 83.3 | IDENTICAL |
| `type >= 0 && type < 15`, `type <= 14` | 83.3 | IDENTICAL |
| `uint32_t type` / `int type` | 83.3 | IDENTICAL |
| `(uint32_t) race < 4`, `int race` | 83.3 / 83.2 | |
| `for (int i ...)` instead of `uint32_t i` | 83.3 | IDENTICAL |
| `while (status->getStation()->getIndex() == targetStation)` | 83.2 | |
| `Galaxy *galaxy` after `fromSystemPtr` | 83.2 | |
| `int bonus = 0` / `int level = 0` | 83.3 | IDENTICAL |
| `(1U << type) & 0x4801U` before `type < 15` | 63.9 | 1968 B |
| `if (offer != 5) {else} else {type8}` (semantics kept) | 63.6 | 1956 B, `type` in r5 |
| `int amount` (no `padA`) | 79.1 | 1980 B, frame 96 |
| `int amount` at top of function | 79.1 | 1980 B, frame 96 |
| `volatile int amount` at top | 71.9 | 1984 B |
| `volatile int32_t type` | 53.5 | 2036 B, extab 176 |
| `*(volatile int32_t *)&type == 13` | 54.2 | 2036 B, extab 164 |
| `*(volatile uint32_t *)&type < 15` | 53.6 | 2012 B, extab 164 |
| `volatile uint32_t race` | 56.1 | 2016 B |
| `volatile int guard` | 52.7 | 1968 B, `type` in fp |
| `volatile bool selected` | 59.6 | 2044 B |
| `int tec = getTecLevel()` temp | 81.0 | 2000 B, extab 176 |
| `pick == 12` without hint | 80.6 | 1996 B, store 0x19e |
| `pick == 12` restructured (`type = 12; if (idx != 25) type = pick`) | 52.8 | 1988 B |

## 5. Ghidra loop shape (re-confirmed)

`rea_body.c` and the Thumb disassembly agree on the `while (!selected)` loop:

```c++
do {
    selected = guard < 1;
    pick = rnd->nextInt(15);
    guard--;
    type = pick;                              // kept in r2, stored after the loop
    if (pick != 8 && (pick != 10 || (type = 10, race < 4))) {
        if (used != nullptr) {
            if (used->data()[pick]) {
                sum = ...;
                if (sum == 14) { clear used; type = pick; continue; }
                type = pick; continue;
            }
            used->data()[pick] = true;
        }
        if (pick == 12) {
            selected = true;
            type = getSystem()->getIndex() == 25 ? 12 : pick;
            continue;
        }
        selected = true;
        type = pick;
    }
} while (!selected);
```

The loop body is **byte-identical** to ours from `0x97248` to `0x972d4`
except for the `pick == 12` common-block placement (original puts
`selected = true; type = pick` *after* the `pick == 12` block and branches to
it; ours puts it before).  The original's `type` is `r2` during the loop and
`[sp,#36]` afterwards, so `race` and `type` share the `[sp,#36]` slot.

## 6. Conclusion

The remaining 16.7 % is still the two coupled `MachineBlockPlacement`
decisions from wave 113 (shared `type` store after the `type = 13` arm, and
therefore the null block outlined into the `switch` region).  The new
`(type & 0xff) == 13` result shows that the `type < 15` branch target *can* be
reproduced in plain C++, but only by making `type` register-resident, which
costs far more than it gains.  No variant measured this pass beats the
wave-114 baseline of **83.3 % / 1996 B / extab 168 B**.

## 7. Reproduction

```sh
# baseline
cp /dev/shm/w6309/Generator.base.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6

# masked type-13 test (branch target fix, regalloc regression)
sed -i 's/if (race < 4 \&\& type == 13)/if (race < 4 \&\& (type \& 0xff) == 13)/' src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6309/score.py          # 64.4 % 1972 B extab 168
```

Scratch files: `/dev/shm/w6309/` (`score.py`, `t2.py`, `nd2.py`, `ndiff.py`,
`cst.py`, `freq2.py`, `cc.sh`, `batch*.py`, `mi_bp2.txt`, `pa_all.txt`).
Not part of the repo.
