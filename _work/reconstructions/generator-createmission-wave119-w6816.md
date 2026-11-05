# `Generator::createMission` — wave 119 (w6816): the LSDA gap row is a *placement* artifact

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `cslen = 151`,
26 call-site rows).

Status: **research note, no source change landed.**  Master keeps the tuned
**83.6 % / 1996 B / 168 B record** shape.  This pass found the *mechanism* that
produces the missing 4 B (it is a `EHStreamer` "gap" entry, not a landing pad),
re-derived the exact MBP condition that would move the block, and measured
another ~60 source variants plus an exhaustive 729-point IR branch-weight
sweep.  No shape is simultaneously `>= 83.6 %`, 1996 B and a 172 B record.

```
before (HEAD):  83.6 %  1996 B  .ARM.extab 0x26294  createMission record 168 B (cslen 147, 25 rows)
after:          83.6 %  1996 B  .ARM.extab 0x26294  (unchanged)
```

Gate at the start and end of this pass: `RATCHET PASS`, `byte 4505`,
`linked 4505`, `data 843/843`, imports 207, `lint CLEAN`, `drift 0`,
27 sections, file size 2,187,328 B.

---

## 1. New result: the missing row is an EHStreamer *gap* entry

`lib/CodeGen/EHStreamer.cpp: computeCallSiteTable()` (LLVM 7.0.2) walks the
machine instructions in address order and keeps `LastLabel` = the end label of
the previous invoke try-range plus a `SawPotentiallyThrowing` flag that is set
by any **ordinary** (non-`nounwind`) call.  When the next invoke try-range
opens it emits

```c++
if (SawPotentiallyThrowing && Asm->MAI->usesCFIForEH()) {
  CallSiteEntry Site = { LastLabel, BeginLabel, nullptr, 0 };   // lp = 0 !
  CallSites.push_back(Site);
}
```

i.e. a row `(start, length, landing_pad = 0, action = 0)` spanning *from the end
of the previous invoke try-range to the start of the next one*.  That is
exactly the original's extra row:

```
orig row  (968, 32, 0, 0)   ->  [0x97580, 0x975a0)
   0x97580  ldr.w sl, [r6, r0, lsl #3]      (tail of the type==15 arm)
   0x97584  b.n 97908
   0x97586  ldr r0, [sp,#40]                 <-- the getRoutes()==nullptr block
   0x9758a  ldr r0, [r0,#0]
   0x9758c  blx AERandom::nextInt(int)       <-- the only call, an ordinary call
   0x97598  str r1, [sp,#36]
   0x9759a  b.n 973fa
   0x9759c  <type==0 arm>                    <-- next invoke try-range starts at 1000
```

So the 4 B are **not** a landing pad and **not** a new call site: they exist
only because the `getRoutes()==nullptr` block (the "null block", `bb.50`)
happens to be laid out *between two invoke try-ranges*.

In our build the null block sits at `0x973f8` (offset **576**), i.e. **before**
the first invoke (`Status::getCurrentCampaignMission` at offset **606**, the
first call after `agentName` is constructed).  Everything before that invoke
collapses into one gap row `(0, 606, 0, 0)`, so the null block's ordinary
`nextInt(2)` is absorbed and no row appears.

**Therefore the whole 4 B question is: can `bb.50` be laid out after
`getName` (offset >= 606)?**  It does not have to land at 968 like the
original — *any* address after the first invoke try-range produces the row.

## 2. Why `bb.50` is placed immediately before `getName`

`-print-machineinstrs=block-placement` chain trace of HEAD:

```
chain1: bb.31 (getOffer test) -> bb.32 (offer arm) -> bb.49 (type spill block)
        then bb.51 (getName) is REJECTED by hasBetterLayoutPredecessor(bb.49, bb.51)
        because bb.33 / bb.37 / bb.38 are still unplaced and hot
chain2: bb.33 -> ... -> bb.48 (movs r0,#13); bb.49 already placed -> chain ends
chain3: bb.50 (null block) -> bb.51 -> ...          <-- null block right before getName
```

Original (from its disassembly):

```
chain1: bb.31 -> bb.32                              (bb.49 rejected here instead)
chain2: bb.33 -> ... -> bb.48 -> bb.49 -> bb.51 -> ... -> type==15 chain
chain3: bb.50                                        (placed at 0x97586, later)
```

So both layouts come from the *same* MBP decision: whether `bb.49` (the
`str r0,[sp,#36]` spill block, preds `bb.32` and `bb.48` only) is chained after
`bb.32` (ours) or after `bb.48` (original).

## 3. The exact MBP condition

`MachineBlockPlacement::hasBetterLayoutPredecessor(bb.32, bb.49)` iterates
`bb.49`'s other unplaced predecessors and rejects the edge when

```
freq(Pred) * HotProb >= freq(bb.32) * (1 - HotProb)      HotProb = StaticLikelyProb/100 = 0.8
=>  freq(bb.48) >= freq(bb.32) / 4
```

Measured with MBFI frequencies recomputed from the block-placement dump:

```
freq(bb.32) = 0.303364
freq(bb.48) = 0.004708
ratio       = 0.015518          (16x short of 0.25)
```

Path decomposition (`0.5 * 0.5 * 0.9995 * 0.0312 * 0.75 * 0.6667 * 0.625 * 0.0312`):

```
0.5    getOffer() != 5
0.5    (uint32_t)type < 15
0.9995 (1u << type) & 0x4801
0.0312 while (targetStation == getStation()->getIndex())   (31:1 backedge)
0.75   race < 4
0.6667 type == 13
0.625  getRoutes() != nullptr
0.0312 while (!found)                                      (31:1 backedge)
```

Dropping **both** loop factors only reaches **0.156** — still under 0.25.  The
only single knob that pushes it over is `type < 15` -> ~1 as well (0.31).

## 4. Exhaustive IR branch-weight sweep (729 points)

`{none, 1:2000, 2000:1}` on each of
`offer / type15 / mask / race / routes / loopexit` (`!prof` metadata on the
`br`, recompiled from `gen.ll`).  Exactly **one** combination produces a 172 B
record:

```
(offer 2000:1, type15 none, mask none, race 1:2000, routes 1:2000, loopexit 2000:1)
    text 2052 B   extab 172 B
```

Every other combination is 168 B (or 176/180).  **No combination gives
1996 + 172.**

Single-knob fine sweep (IR weights 1:2 .. 8:1) reproduces the prior note:
`offer 1:1 1996/168`, `4:3 1992/168`, `3:2 1992/168`, `2:1 1972/168`,
`3:1 1988/176`, `4:1 2072/176`, `8:1 2052/176`; `type15 1:2 1992/176`,
`1:3 1976/176`, `1:4 2068/176`.

## 5. Source-level sweeps

### 5.1 All 32 subsets of five `__builtin_expect` sites

`A = expect((uint32_t)type < 15, 1)`, `B = expect(targetStation ==
getStation()->getIndex(), 0)`, `C = expect(race < 4 && type == 13, 1)`,
`D = expect(getRoutes() == nullptr, 0)`, `E = expect(!found, 0)`

| combo | text | extab | pct |
|---|---:|---:|---:|
| BASE | 1996 | 168 | 83.6 |
| A | 1960 | 168 | 65.6 |
| B | 1992 | 168 | 76.4 |
| C | 1960 | 168 | 65.6 |
| D | 1980 | 168 | 63.8 |
| E | 2036 | 168 | 47.4 |
| A+B | 1976 | 168 | 62.2 |
| **A+C** | **2000** | **176** | **60.6** |
| A+D | 1968 | 168 | 61.2 |
| A+E | 1992 | 168 | 43.8 |
| B+C | 1976 | 168 | 62.2 |
| B+D | 1984 | 168 | 59.6 |
| B+E | 2040 | 168 | 48.9 |
| C+D | 1968 | 168 | 61.2 |
| C+E | 2036 | 168 | 47.0 |
| D+E | 2036 | 168 | 44.1 |
| A+B+C | 2000 | 176 | 60.6 |
| A+B+D | 1968 | 168 | 60.8 |
| A+B+E | 2032 | 168 | 48.6 |
| A+C+D | 1968 | 168 | 54.9 |
| A+C+E | 1992 | 168 | 43.5 |
| A+D+E | 1984 | 168 | 44.1 |
| B+C+D | 1968 | 168 | 60.8 |
| B+C+E | 2028 | 168 | 45.3 |
| B+D+E | 2052 | 168 | 44.8 |
| C+D+E | 2032 | 168 | 49.4 |
| A+B+C+D | 1968 | 168 | 54.9 |
| A+B+C+E | 2044 | 168 | 47.4 |
| A+B+D+E | 2028 | 168 | 45.6 |
| A+C+D+E | 1984 | 168 | 44.1 |
| B+C+D+E | 2016 | 168 | 47.4 |
| A+B+C+D+E | 2028 | 168 | 45.6 |

### 5.2 Structural variants (all `text / extab / pct`)

| variant | text | extab | pct |
|---|---:|---:|---:|
| HEAD | 1996 | 168 | 83.6 |
| `type = 13;` moved before the `while (!found)` loop | 1996 | 168 | 83.6 |
| `if (getRoutes() != nullptr) {loop} else {null}` (inverted) | 1996 | 168 | 83.6 |
| nested `if (race < 4) { if (type == 13) {...} }` | 1996 | 168 | 83.6 |
| nested `if (type == 13) { if (race < 4) {...} }` | 1996 | 168 | 83.6 |
| `expect(race < 4, 1) && type == 13` | 1996 | 168 | 83.6 |
| `race < 4 && expect(type == 13, 1)` | 1996 | 168 | 83.6 |
| `expect(race < 4, 0) && type == 13` | 1996 | 168 | 83.6 |
| `race < 4 && expect(type == 13, 0)` | 2036 | 168 | 44.4 |
| `int found` instead of `bool found` | 1996 | 168 | 83.3 |
| `for (;;) { ... if (found) break; }` | 2032 | 168 | 43.7 |
| `do { ... } while (!found)` | 2032 | 168 | 43.7 |
| `found` declared inside the loop | 2032 | 168 | 43.7 |
| `expect(pick == 12, 1)` | 1996 | 168 | 80.9 |
| plain `if (pick == 12)` (hint removed) | 1996 | 168 | 80.9 |
| `expect(i < systems->size(), 1)` on the inner `for` | 1960 | 168 | 65.6 |
| `expect(i < systems->size(), 0)` on the inner `for` | 2032 | 168 | 48.3 |
| `expect(stationIsInSystem && getRace()==race, 1)` | 2028 | 168 | 48.3 |
| `volatile int32_t type` (V) | 2044 | 176 | 52.9 |
| V + `expect(getOffer()==5, 0)` | 1960 | 180 | 52.4 |
| V + `expect(!found, 0)` | 2032 | 176 | 44.6 |
| V + `expect(getOffer()==5,0)` + `expect(!found,0)` | 1980 | 168 | 63.8 |
| V + offer/found/routes hints | 2016 | 180 | 57.2 |
| `expect(getOffer()==5,1)` + `expect(race<4&&type==13,0)` + `expect(routes==nullptr,0)` + `expect(found,1)` | 1996* | 168 | 42.8 |

(*object 1996 B, linked 1956 B after branch relaxation.)

### 5.3 The `type` spill block is fragile

Any hint that lifts `freq(bb.48)` enough also makes `type` **register
resident**, which deletes the shared spill block `bb.49` altogether.  The MBP
dump reports no `str r0,[sp,#36]` block (`store None`) for `A`, `D`, `D+E`,
`B+D+E` and `A+B+C+D+E`; `A+B+C+D+E` reaches ratio 0.988 (well over 0.25) but
the layout collapses to the plain IR order `0..162` and the text is 2028 B.
The original keeps `type` partly register-resident (`r0` for the `type = 8`
and `type = 13` arms) *and* still spills through a shared block — that
combination is what we cannot reproduce.

## 6. Conclusion

The 4 B are recoverable only if `bb.49` is chained after `bb.48` instead of
`bb.32`, i.e. `freq(bb.48) >= freq(bb.32)/4`.  With the branch probabilities
this CFG produces, the path product tops out at 0.156 even with both loops at
100 %; the extra 1.6x needs `type < 15` at ~1 as well, and every hint that
does that (or that lowers `freq(bb.32)`) changes `type`'s register allocation
enough to delete the spill block.  The exhaustive 729-point weight sweep
confirms that the only 172 B shape in reach is 2052 B.

Master therefore keeps `83.6 % / 1996 B / 168 B`; `.ARM.extab` stays 4 B short
(`0x26294` vs `0x26298`), from this function alone.

## 7. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6816/score.py                         # pct 83.6, usize 1996
python3 /dev/shm/w6816/lsda2.py                         # cslen 147, 25 rows (orig 151, 26)
python3 /dev/shm/w6816/mbpfreq.py /dev/shm/w6816/base.cpp   # ratio 0.01552
python3 /dev/shm/w6816/search_weights.py                # 729-point weight sweep
```

Scratch: `/dev/shm/w6816/` (`base.cpp`, `meas.py`, `score.py`, `lsda2.py`,
`mbpfreq.py`, `search_weights.py`, `sw1.out`, `sw2_single.out`, `gen.ll`,
`allpass_new.txt`, `mbp_after.txt`).  Not part of the repo.
