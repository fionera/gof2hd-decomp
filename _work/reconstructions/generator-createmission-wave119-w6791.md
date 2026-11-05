# `Generator::createMission` — wave 119 (w6791): the MBP 50/50 tie at `getOffer()`

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `cslen = 151`,
26 call-site rows).

Status: **research note, no source change landed.**  Master keeps the tuned
**83.6 % / 1996 B / 168 B record** shape.  This pass re-derived *why* the
remaining 4 B of `.ARM.extab` cannot be recovered from source, measured ~120
source variants, and found no shape that is simultaneously

* `>= 83.6 %` fuzzy,
* body exactly 1996 B,
* 172 B LSDA record (`.ARM.extab` size-exact `0x26298`).

```
before (HEAD):  83.6 %  1996 B  .ARM.extab 0x26294  createMission record 168 B (cslen 147, 25 rows)
after:          83.6 %  1996 B  .ARM.extab 0x26294  (unchanged)
```

Gate at the start and end of this pass: `RATCHET PASS`, `byte 4505`,
`linked 4505`, `data 843/843`, imports 207, `lint CLEAN`, `drift 0`,
27 sections, file size 2,187,328 B.

## 1. The whole delta is one `MachineBlockPlacement` decision

The linked baseline is instruction-for-instruction identical to the original
except for **two** block-placement choices, both downstream of one 50/50 branch:

```
orig                                      ours
97348: bne.n 97356   (offer != 5)         97348: bne.n 97358
9734a: <offer arm: getStation; movs r0,#8> 9734a: <offer arm: getStation; movs r0,#8>
97354: b.n 973f8     <-- spill block      97354: str r0,[sp,#36]  <-- spill inline
...                                       ...
973f6: movs r0,#13                        973f4: movs r0,#13
973f8: str r0,[sp,#36]  <-- spill block   973f6: b.n 97354       <-- back to spill
973fa: blx Agent::getName                 973f8: <null block, nextInt(2)>
97586: <null block, nextInt(2)>           9740c: blx Agent::getName
```

Everything after the null block is shifted by `+0x12` (18 B) in ours, which is
why the rest of the function shows a large number of "offset-only" mismatches.
Fixing the spill-block placement would fix the null block too (they are the same
decision: the spilled `type` store is `bb.49`; the null block is `bb.50`; the
`getName` block is `bb.51`).

## 2. The CFG, with the `-print-machineinstrs=block-placement` dump

`bb.31` (`%ir-block.114`, the `getOffer()` test) has a **50/50** branch:

```
bb.31  successors: bb.32(50%), bb.33(50%)     ; offer == 5 ?
bb.32  successors: bb.49(100%)                ; offer arm: getStation(); type = 8
bb.33  successors: bb.34(50%), bb.51(50%)     ; (uint32_t)type < 15
bb.34  successors: bb.37(0.05%), bb.36(99.95%); mask (1<<type)&0x4801
bb.36  successors: bb.35(96.88%), bb.37(3.12%); while (targetStation == getIndex())
bb.37  successors: bb.38(75%), bb.51(25%)     ; race < 4
bb.38  successors: bb.39(66.67%), bb.51(33.33%); type == 13
bb.39  successors: bb.50(37.5%), bb.40(62.5%); getRoutes() == nullptr
bb.40..bb.43                                  ; while (!found) loop
bb.47  successors: bb.48(3.12%), bb.41(96.88%); loop exit
bb.48  successors: bb.49(100%)                ; movs r0,#13
bb.49  successors: bb.51(100%)                ; str r0,[sp,#36]   <-- shared store
bb.50  successors: bb.51(100%)                ; null block (type = 4/1)
bb.51  predecessors: bb.50, bb.33, bb.37, bb.38, bb.49   ; getName
```

Measured frequencies (branch weights propagated from the dump; `F31` = 1.302e-3):

```
freq(bb.32) = 0.5 * F31          = 6.51e-4   (offer arm)
freq(bb.48) = F31 * 1.52e-4      = 9.93e-8   (type = 13 arm)
ratio       = 1.525e-4                       threshold 0.25 (HotProb = 80 %)
```

`hasBetterLayoutPredecessor(bb.32 -> bb.49)` rejects `bb.32`'s claim only when
`freq(bb.48) >= freq(bb.32)/4`.  Ours is **1640x** short, so `bb.49` is chained
after `bb.32`; `bb.50` then falls through into `bb.51`, so both land before
`getName` and the extra `lp = 0` row never appears.

The path to `bb.48` is
`0.5 (offer!=5) * 0.5 (type<15) * 0.9995 (mask) * 1/32 (while targetStation) *
 0.75 (race<4) * 2/3 (type==13) * 0.625 (routes!=null) * 1/32 (while !found)`.
The two loop-backedge heuristics (LBH, 124:4 = 31:1) dominate.  Even with both
loop exits at 100 % the ratio is only **0.156**, so no plain source change can
flip it; `freq(bb.32)` must also drop, which requires a branch-weight hint on
`getOffer()` — and every such hint also makes `type` register-resident, which
deletes the shared store block entirely.

Note also that the tie at `bb.31` itself is 50/50.  In the original MBP keeps
`bb.32` as a single-block chain (it ends in `b 973f8`) and lays out `bb.33`'s
chain (`bb.33..bb.48, bb.49, bb.51`) after it.  In ours the chain is
`bb.31 -> bb.32 -> bb.49`, then `bb.33`'s chain.  Both are legal MBP outputs
for the same CFG; the tie-break at `bb.31` is not reachable from source
(canonicalisation makes `== 5`, `!= 5`, `bool isOffer = ...`, and a `getOffer()`
temp all compile to the identical object).

## 3. Frame layout: the second half of the delta

`-print-machineinstrs=prologepilog` frame objects, and the linked disassembly,
give the two layouts:

```
original (frame 96)                 ours (frame 112, `volatile double padA`)
sp+16  galaxy                       sp+20  &__stack_chk_guard
sp+20  &__stack_chk_guard           sp+24  agent / galaxy
sp+24  agent / level                sp+28  targetStation / level
sp+28  targetStation / amount       sp+32  status
sp+32  status                       sp+36  race / type
sp+36  race / type                  sp+40  rnd
sp+40  rnd                          sp+44..92  four AbyssEngine::String
sp+44..92  four AbyssEngine::String sp+92  amount
sp+92  canary                       sp+96  padA
                                    sp+108 canary
```

The original fits `amount` into `targetStation`'s dead early slot (`sp+28`) and
gives `galaxy` its own slot (`sp+16`); ours reuses `sp+24` (agent's slot) for
`galaxy`, pushes `level` to `sp+28`, and spills `amount` to `sp+92`.  The
`padA` hack exists only to shift the whole object area by +4 (so the five
source-level int slots land at `sp+20..40`, matching the original) and to add
the 12 missing bytes of body.

The one source shape that *does* reproduce the original's `galaxy` handling is
`galaxy->distance(..., systems->data()[Globals::galaxy->getStation(...)...])`
(local `galaxy` for `distance`, `Globals::galaxy` for `getStation`): it emits
the exact `ldr r5,[&Globals::galaxy]; ldr r0,[r5]; str r0,[sp,#16]` sequence
and the second `ldr r0,[r5]` before `getStation`.  But it also makes `type`
register-resident, and scores **68.8 % / 2008 B**.

## 4. Measurements (all on top of the pristine 83.6 % shape unless stated)

`V` = `volatile int32_t type`, `O` = `expect(getOffer()==5, 0)`,
`B` = `int base` split, `T` = tail-loop hint.

| variant | pct | usize | cslen/rows | extab |
|---|---:|---:|---:|---:|
| **HEAD** | **83.6** | **1996** | **147/25** | **0x26294** |
| O | 63.9 | 1956 | 147/25 | 0x26294 |
| B | 77.7 | 1996 | 146/25 | 0x26294 |
| T | 78.0 | 1996 | 147/25 | 0x26294 |
| OB | 63.3 | 1956 | 146/25 | 0x26294 |
| OT | 62.1 | 1960 | 152/26 | 0x2629c |
| **OBT** | **61.3** | **1956** | **151/26** | **0x26298** |
| V | 52.9 | 2044 | 152/26 | 0x2629c |
| VB | 51.1 | 2044 | 151/26 | 0x26298 |
| VOB | 50.5 | 1996 | 151/26 | 0x26298 |
| VOBT | 50.2 | 1996 | 151/26 | 0x26298 |
| VB, no O/T | 51.1 | 2044 | 151/26 | 0x26298 |
| `expect(race<4 && type==13, 1)` | 65.6 | 1960 | 147/25 | 0x26294 |
| `expect(race<4 && type==13, 0)` | 44.4 | 2036 | 147/25 | 0x26294 |
| `expect(type==13, 1)` / `expect(race<4,1)` / `expect(race<4,0)` | 83.6 | 1996 | 147/25 | 0x26294 |
| `expect(pick==12,0)` removed (plain `if`) | 80.9 | 1996 | 147/25 | 0x26294 |
| `expect(pick==12,1)` | 80.9 | 1996 | 147/25 | 0x26294 |
| `expect(pick==10,1)` removed | 53.8 | 1992 | 147/25 | 0x26294 |
| `expect(pick==10,0)` | 54.0 | 1996 | 147/25 | 0x26294 |
| swap mask-check / type13-check | 62.0 | 1960 | 147/25 | 0x26294 |
| `if (type==12)` after `getName` | 64.1 | 1976 | 153/26 | 0x2629c |
| `if (type==12)` after clamp | 67.0 | 2004 | 157/26 | 0x262a0 |
| `if (type==12)` after reward chain | 66.6 | 1980 | 156/26 | 0x262a0 |
| `if (type==12)` before `getName` | 67.9 | 1972 | 147/25 | 0x26294 |
| `expect(type==8,1)` | 45.9 | 1996 | 159/26 | 0x262a0 |
| `expect(type==7,0)` | 78.7 | 1996 | 141/25 | 0x26290 |
| `expect((type|4)==12,0)` | 68.1 | 2012 | 147/25 | 0x26294 |
| `expect(difficulty>=10,0)` / `expect(type==12,0/1)` | 83.6 | 1996 | 147/25 | 0x26294 |
| `while (!found)` -> `do/while`, `while(true)+break` | 43.7 | 2032 | 147/25 | 0x26294 |
| `while (!found)` -> `for (;!found;)` | 83.6 | 1996 | 147/25 | 0x26294 |
| `while (targetStation==...)` -> `for` | 83.6 | 1996 | 147/25 | 0x26294 |
| `expect(targetStation==...,0)` | 76.4 | 1992 | 147/25 | 0x26294 |
| `expect(targetStation==...,1)` | 43.7 | 2012 | 147/25 | 0x26294 |
| `expect(!found,0)` | 47.4 | 2036 | 147/25 | 0x26294 |
| `expect(found,1)` on loop exit | 83.6 | 1996 | 147/25 | 0x26294 |

### 4.1 `padA` variants

| variant | pct | usize | extab |
|---|---:|---:|---:|
| **`volatile double padA` (HEAD)** | **83.6** | **1996** | **0x26294** |
| `volatile double padA[2]` | 83.6 | 1996 | 0x26294 |
| `volatile long long padA` | 79.5 | 1996 | 0x26294 |
| `volatile float padA[2]` | 79.8 | 1992 | 0x26294 |
| `volatile int padA[2]` | 79.4 | 1992 | 0x26294 |
| `volatile int padA` | 79.4 | 1992 | 0x26294 |
| no `padA` at all | 71.9 | 1984 | 0x26294 |
| store only (`padA = 0;`) | 79.8 | 1992 | 0x26294 |
| read only (`(void) padA;`) | 80.1 | 1988 | 0x26294 |
| store `padA = 0.0` / `-1` / via `volatile*` | 83.6 | 1996 | 0x26294 |
| padA before `galaxy` | 83.5 | 1996 | 0x26294 |
| padA before `fromSystem` | 83.3 | 1996 | 0x26294 |
| padA before `fromSystemPtr` (HEAD) | **83.6** | 1996 | 0x26294 |
| padA before `level = getLevel()` | 82.2 | 1996 | 0x26294 |
| padA after `getName` / before `itemId` | 81.1 | 1996 | 0x26294 |
| padA before `new Mission` | 82.0 | 1996 | 0x26294 |
| padA after difficulty clamp | 83.5 | 1996 | 0x26294 |

### 4.2 Declaration order / types / expression shapes

| variant | pct | usize |
|---|---:|---:|
| **HEAD (level, padA, amount)** | **83.6** | **1996** |
| amount, level, padA | 75.5 | 1996 |
| padA, level, amount | 83.6 | 1996 |
| level, amount, padA | 75.5 | 1996 |
| amount declared at top / after targetStation / after race | 75.5 | 1996 |
| `Galaxy *galaxy` declared at top, assigned later | 83.6 | 1996 |
| `Globals::galaxy` used directly for both calls | 83.3 | 1996 |
| local `galaxy` + `Globals::galaxy` for `getStation` | 68.8 | 2008 |
| `int32_t type` -> `int` / `uint32_t` | 83.6 | 1996 |
| `uint32_t race` -> `int` | 83.5 | 1996 |
| `bool selected` -> `int` | 83.5 | 1996 |
| `int guard` -> `uint32_t` | 56.6 | 1988 |
| `int32_t type` -> `short` | 57.8 | 2008 |
| `volatile int amount` | 83.5 | 1996 |
| `volatile int level` | 79.5 | 2004 |
| `volatile uint32_t race` | 58.2 | 2012 |
| `volatile int difficulty` | 68.6 | 1992 |
| `volatile int itemId` | 47.5 | 2000 |
| `volatile int targetStation` | 48.4 | 2000 |
| reward chain `7,9,8,11,3||5` (HEAD) | **83.6** | 1996 |
| reward chain `9,7,8,11,3||5` | 83.2 | 1996 |
| reward chain `9,7,8,3||5,11` | 80.1 | 2000 |
| reward chain `7,9,8,3||5,11` | 80.5 | 2000 |
| reward chain `8,7,9,11,3||5` | 74.0 | 1992 |
| reward chain `7,8,9,11,3||5` | 75.7 | 1996 |
| `count`/`amount`/`difficulty` reorder in the `type==8` block | 83.3 | 1996 |
| `amount = 0` (plain) instead of `*(volatile int*)&amount = 0` | 70.2 | 1992 |
| `if (pick==8) { type = pick; continue; }` instead of `goto` | 70.9 | 1992 |
| `if (getOffer() != 5)` inverted / `bool` temp | 83.6 | 1996 |
| `if (type==12)` condition hint 0/1 | 83.6 | 1996 |
| campaign `<16` -> `<=15` / temp / options-first | 83.6 | 1996 |

## 5. Conclusion

The 172 B record is reachable only by flipping the `bb.49` layout predecessor
from `bb.32` to `bb.48`, which needs `freq(bb.48) >= freq(bb.32)/4` — a 1640x
gap that no plain source change can close.  The two ways to close it
(`volatile int32_t type`, or a `getOffer()` branch-weight hint) both change the
register allocation so much that the fuzzy score drops from 83.6 % to 50-62 %.
The only 1996 B + 172 B variants known remain `VOBT` (50.2 %) and `VB`-without
hints (51.1 %, 2044 B); `OBT` reaches 172 B at 1956 B (40 B short of the body).

The `.ARM.extab` section therefore stays 4 B short (`0x26294` vs `0x26298`),
from this function alone.

## 6. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6791/one.py                           # pct 83.6, usize 1996, extab 0x26294
python3 /dev/shm/w6791/lsda2.py cmake-build-match/libgof2hdaa.so   # cslen 147, 25 rows
```

Scratch: `/dev/shm/w6791/` (`pristine.cpp`, `one.py`, `harness.py`, `rows.py`,
`lsda2.py`, `sweep5.py`..`sweep39.py`, `mbp2.txt`, `pei.txt`, `orig_addr.asm`,
`ours_addr.asm`).  Not part of the repo.
