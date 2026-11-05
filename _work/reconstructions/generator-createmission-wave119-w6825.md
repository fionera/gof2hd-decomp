# `Generator::createMission` — wave 119 (w6825): permutation / `volatile amount` / EH-placement sweep, no source win

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `cslen = 151`,
26 call-site rows; ours **168 B**, `cslen = 147`, 25 rows).

Status: **research note, no source change landed.**  Master keeps the tuned
**83.6 % / 1996 B / 168 B record** shape.

```
before (HEAD):  83.6088 %  1996 B  .ARM.extab 156308 B (orig 156312)  record 168 (cslen 147, 25 rows)  frame 112
after:          83.6088 %  1996 B  .ARM.extab 156308 B                 record 168                       frame 112
```

Gate at the start and end of this pass: `RATCHET PASS`, `byte 4505`,
`linked 4505`, `data 843/843`, imports 207, `lint CLEAN`, `drift 0`,
27 sections, file size 2,187,328 B.

Supersedes/extends `generator-createmission-wave119-w6806.md` (the `amount`
alloca root cause) and `generator-createmission-wave119-w6810.md` (the
172-record family).  Everything below is new measurement unless stated.

## 1. The two remaining deltas are both `MachineBlockPlacement` choices

MIR block names are the ones in the `-print-machineinstrs=block-placement`
dump of the current TU (`/dev/shm/w6825/mbp_base.txt`).

* **Shared `type` store (`bb.49`, `%ir-block.186`).**  Its predecessors are the
  offer arm `bb.32` and the `type = 13` block `bb.48`.  Master places it right
  after `bb.32` (`97354 str r0,[sp,#36]; 97356 b.n 9740c`); the original places
  it after `bb.48` (`973f6 movs r0,#13` falls through to `973f8 str r0,[sp,#36]`,
  and the offer arm branches `97354 b.n 973f8`).
  `hasBetterLayoutPredecessor` accepts the `bb.32 -> bb.49` edge unless
  `freq(bb.48) >= freq(bb.32)/4` (LLVM 7 `HotProb = 80 %`).  `bb.48` is the
  `type = 13` path behind `race < 4 && type == 13` and the `while (!found)`
  loop, so it is ~50–200× colder than the 50 % offer arm.  Every source lever
  that raises it (`__builtin_expect` on `race < 4 && type == 13`,
  `getRoutes() == nullptr`, `while (!found)`) changes the body size.

* **Cold `getRoutes() == nullptr` block (`bb.50`, `%ir-block.151`).**  It is a
  chain head picked by `getFirstUnplacedBlock` in IR order, so master puts it
  immediately before `getName` (`973f8..9740a`, inside LSDA row 0 `lp=0`).
  The original puts it inside the switch region between the case-15 block and
  the case-0 block (`97586..9759a`), which is exactly the extra `lp=0` LSDA
  row 7 (`0x3c8..0x3e8`) that makes the record 172 B.  Moving the source
  `if (race < 4 && type == 13)` block later does move the cold block, but the
  LSDA grows to 192 B and the score collapses (§4).

## 2. Reward-chain permutation sweep (120 of 720, seed 119)

The six-block chain `7, 9, 8, 11, 3, 5` (master) was permuted and rebuilt.
Only permutations that keep `usize == 1996` are landable (a size change shifts
the whole image).  Best sampled permutation:

| permutation | pct | usize | record |
|---|---:|---:|---:|
| **7, 9, 8, 11, 3, 5 (HEAD)** | **83.6088** | 1996 | 168 |
| 9, 7, 8, 11, 5, 3 | 83.1956 | 1996 | 168 |
| 9, 7, 8, 11, 3, 5 | 83.1956 | 1996 | 168 |
| 7, 9, 8, 11, 5, 3 | 79.34 | 1996 | 168 |
| faithful `9, 7, 8, (3\|\|5), 11` | 80.1 | 2000 | 168 |

No permutation beats HEAD.  (The faithful order is still the clearest example
of "a faithful source shape scores lower than the tuned one".)

## 3. `volatile int amount` reproduces the original frame, but not its slots

`volatile int amount` + plain `amount = 0;` + no `padA` gives **frame 96**
(the original's frame) and a prologue that is byte-identical to the original
for the first 5 instructions (`str r0,[sp,#92]` canary included); the literal
pool PC offsets then diverge because the body is 12 B short.

```
volatile int amount, no padA:  pct 71.9117  usize 1984  frame 96   record 168
HEAD (alloca amount + padA):   pct 83.6088  usize 1996  frame 112  record 168
```

The reason it still scores only 71.9 %: the original's stack slots are
`sp+16 galaxy/bonus, sp+20 guard, sp+24 agent/difficulty, sp+28 targetStation/amount,
sp+32 status, sp+36 race/type, sp+40 rnd, sp+44..92 Strings, sp+92 canary`.
The `volatile` variant is shifted one slot (`sp+16 guard, sp+20 agent,
sp+24 amount, sp+28 status, sp+32 race/type, sp+36 rnd, sp+40 String...`) and
spills `mission` at `sp+88` where the original keeps it in `r9`.  Forcing the
original's `galaxy` spill (`Globals::galaxy->getStation(...)` instead of the
local `galaxy`) adds a slot (frame 104) and loses more:

| variant | pct | usize | frame | record |
|---|---:|---:|---:|---:|
| `volatile int amount`, no padA | 71.91 | 1984 | 96 | 168 |
| `volatile int amount` + `volatile double padA` | 83.47 | 1996 | 112 | 168 |
| `volatile int amount` + `volatile short/char padA` | 79.28 | 1996 | 104 | 168 |
| `volatile int amount` + `volatile int padA` | 79.15 | 1992 | 104 | 168 |
| `volatile int amount` + `volatile long long padA` | 70.65 | 1996 | 104 | 168 |
| `volatile int amount` + `volatile int/short padA` read-only | 79.53 / 79.86 | 1988 | 104 | 168 |
| `volatile int amount` + `Globals::galaxy->getStation` | 65.66 | 2000 | 104 | 168 |
| `volatile int amount` + `Globals::galaxy->distance` | 71.54 | 1988 | 104 | 168 |
| `volatile int amount` + `volatile int difficulty` | 75.47 | 1980 | 104 | 168 |
| `volatile int amount` + `volatile int bonus` | 77.68 | 1980 | 104 | 168 |
| `volatile int amount` + `volatile int itemId` | 51.23 | 1992 | 104 | 168 |
| plain `amount = 0`, no padA | 79.06 | 1980 | 96 | 168 |
| plain + `count` hoisted to function scope | 79.06 | 1980 | 96 | 168 |
| plain + `difficulty` hoisted | 79.06 | 1980 | 96 | 168 |
| plain + local `items` hoisted | 65.70 | 1980 | 104 | 168 |

The `volatile double padA` variant is the closest to HEAD (83.47) but is still
a 0.14-point regression, so it is not landable.

## 4. Moving the `type == 13` block moves the cold block (and the record)

| variant | pct | usize | frame | record |
|---|---:|---:|---:|---:|
| HEAD | 83.61 | 1996 | 112 | 168 |
| `type==13` block after `difficulty` | 44.44 | 2076 | 120 | **192** |
| `type==13` block after `getName` | 48.08 | 1980 | 112 | **192** |

Moving the block does relocate the cold block into the switch region, but it
adds several EH regions and the body size moves, so it is not landable.

## 5. Inverting the offer test changes nothing

`if (agent->getOffer() != 5) { <else body> } else { <offer body> }` compiles to
the **identical** instruction sequence (83.6088 / 1996 / 168): clang
canonicalises the branch and MBP still chains `bb.32 -> bb.49`.

## 6. Hint sweep on the untried sites

Each hint was added to pristine HEAD.  `X0` = `..., 0)`, `X1` = `..., 1)`.

| hint site | X0 | X1 |
|---|---|---|
| `type == 12` | 83.61 / 1996 / 168 | 83.61 / 1996 / 168 |
| `bonus % 50 != 0` | 83.61 / 1996 / 168 | 83.61 / 1996 / 168 |
| `(int)(reward + rem) % 50 == 0` | 83.61 / 1996 / 168 | 83.61 / 1996 / 168 |
| `(costs + costRem) % 50 == 0` | 83.61 / 1996 / 168 | 83.61 / 1996 / 168 |
| `pick == 10` | 53.99 / 1996 / 168 | 83.61 / 1996 / 168 |
| `pick == 8` | 56.63 / 1960 / 168 | 54.41 / 2008 / 168 |
| `used != nullptr` | 57.18 / 2016 / 168 | 56.20 / 1996 / 168 |

The first four are folded away (identical code); the rest lose badly.

## 7. `pick == 12` block order

The only form that reproduces the original's pick-arm fallthrough
(`972b0 cmp r6,#12; 972b2 bne.n 972ce`) is the *semantically wrong*
`if (__builtin_expect(pick != 12, 0)) { <12-body> } selected = true;`
(83.6088 / 1996 / 168, but the 12-body runs when `pick != 12`).  The correct
swap

```c++
if (__builtin_expect(pick != 12, 0)) {
    selected = true;
} else {
    selected = true;
    if (systemIndex == 25) type = 12; else goto assignType;
    continue;
}
```

gives the original's block order but un-rotates the pick loop:
**80.8540 / 1996 / 168**.  HEAD's `expect(pick == 12, 0)` keeps the rotation
and is worth more than the pick-block order.

## 8. 172 B record shapes (for a future pass)

These are the only shapes found that satisfy `usize == 1996` **and** record
172 B; all are well below 83.6 %:

| variant | pct | usize | frame | record |
|---|---:|---:|---:|---:|
| `R71` (`expect(type==7,1)`) + `t9/0` | 78.7 | 1996 | 112 | **172** |
| `R71` + `volatile int padA` read-only | 69.0 | 1996 | 112 | **172** |
| `volatile int amount` + `R71` | 69.24 | 1992 | 104 | **172** |
| `volatile int amount` + `R71` + `t9/0` | 69.29 | 1984 | 96 | **172** |

The `R71` extra row is in the reward chain, not the original's
`getRoutes()==nullptr` gap, so even the record *contents* differ.

## 9. Conclusion

No shape found in this pass keeps `pct >= 83.6088`, `usize == 1996` and restores
the 172 B record.  The 4-byte `.ARM.extab` shortfall therefore remains
(`156308` vs `156312`).  The two blockers are the MBP block-order choices of
§1; both are decided by block frequencies that cannot be moved from source
without also changing the body size or the register allocation.

## 10. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6825/fast.py                          # pct 83.6088 / 1996 / 727/725 / 172/168
python3 /dev/shm/w6825/lsda.py cmake-build-match/libgof2hdaa.so   # 168 / 147 / 25
```

Scratch (not part of the repo): `/dev/shm/w6825/` — `base.cpp` (pristine
HEAD), `fast.py` (fuzzy score), `lsda.py` (record rows), `measure.py`
(pct + frame), `h.py` (build+score one TU), `perm.py` (reward-chain
permutation sweep), `grid_offer.py`, `gridswap.py`, `mbp_base.txt`
(`-print-machineinstrs=block-placement` MIR dump), `mbp.cpp` (LLVM 7
`MachineBlockPlacement.cpp`).
