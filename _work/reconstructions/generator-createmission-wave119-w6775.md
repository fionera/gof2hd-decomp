# `Generator::createMission` — wave 119 (w6775): 172 B LSDA record restored

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, original `.ARM.extab` LSDA record **172 B**,
`cslen = 151`, 26 call-site rows).

Status: **source change landed.**  The wave-119/w6752 tuned shape is **83.6 %**,
1996 B, but its LSDA record is 168 B (`cslen = 147`, 25 rows) and `.ARM.extab`
is 4 B short (`0x26294` vs `0x26298`).  This pass lands the `volatile int32_t
type` shape, which restores the **172 B record** and makes `.ARM.extab`
size-exact at `0x26298`.  The fuzzy score drops to **50.2 %** (the memory-resident
`type` dominates register allocation); the ratchet is unchanged.

```
before (HEAD):  83.6 %  1996 B  .ARM.extab 0x26294  createMission record 168 B
after:          50.2 %  1996 B  .ARM.extab 0x26298  createMission record 172 B  (size-exact)
```

Full `check` with the landed source:

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.text` `0x1337c8` (exact), `.ARM.exidx`
`0x6f70` (exact), `.ARM.extab` `0x26298` (**exact**, was `0x26294`), `.rodata`
`0x41c27` (unchanged).  `.rel.plt` is byte-identical: the tail-loop hint is
required, without it gold permutes three `.rel.plt` slots (`byte_exact` 4505 ->
4485).

## 1. The landed diff

```diff
-    int32_t type = 0;
+    volatile int32_t type = 0;
...
-    if (agent->getOffer() == 5) {
+    if (__builtin_expect(agent->getOffer() == 5, 0)) {  // lint: arm_builtin anchor: Generator::createMission
...
+    int base = (int) ((float) difficulty / 10.0f * 5500.0f) + 1500;
     float reward = (galaxy->distance(fromSystemPtr, systems->data()[galaxy->getStation(targetStation)->getSystem()]) / 1200.0f + 1.0f) *
-                   (float) ((int) ((float) difficulty / 10.0f * 5500.0f) + 1500);
+                   (float) base;
...
-        if (systems->data()[i]->stationIsInSystem(targetStation)) {
+        if (__builtin_expect(systems->data()[i]->stationIsInSystem(targetStation), 1)) {  // lint: arm_builtin anchor: Generator::createMission
```

This is the wave-118/w6609 shape re-applied on top of the current master source
(reward-chain order `7, 9, 8, 11, 3, 5`, `padA` store before `fromSystemPtr`).

## 2. Why the record is short without it

The original call-site table has a **gap row** (`lp = 0`) covering
`0x97580..0x975a0`: the `getRoutes() == nullptr` block sits at `0x97586`
(between the case-15 and case-0 try ranges) and its `nextInt(2)` is a plain call
— `AbyssEngine::String agentName` is not constructed yet, so there is no
cleanup to run.  `EHStreamer::computeCallSiteTable` therefore emits a separate
`lp = 0` row.

With a plain `int32_t type` the two `type` assignments (`type = 8` in the offer
arm, `type = 13` in the else arm) are merged by SimplifyCFG into one shared
store block.  `MachineBlockPlacement` chains that store with the hot offer arm
(`freq(offer) = 0.5 F` vs `freq(type13) = 7.7e-5 F`; `hasBetterLayoutPredecessor`
needs `freq(type13) >= freq(offer)/4`), so the store is emitted inline in the
offer arm and the null block is placed immediately before `getName`
(`0x973f8`), inside the initial `lp = 0` gap row — no extra row.

With `volatile int32_t type` each assignment is a direct store (no phi/store
block); MBP's layout changes, the null block moves out of the initial gap, and
the call-site table gains the extra `lp = 0` gap row.  Row count and total
length (`26`, `151`) then match the original exactly; the individual
starts/lengths still differ with the body layout.

## 3. Measurements (all 1996 B unless noted)

`V` = volatile type, `O` = `expect(offer==5,0)`, `B` = `int base` split,
`T` = tail-loop hint.  `rel` = `.rel.plt` order of `Item::getMaxPrice` vs
`SolarSystem::getName`.

| variant | pct | usize | cslen/rows | extab | rel |
|---|---:|---:|---:|---:|---|
| **HEAD (plain)** | **83.6** | 1996 | 147/25 | 0x26294 | exact |
| V | 52.9 | 2044 | 152/26 | 0x2629c | exact |
| VB | 51.1 | 2044 | 151/26 | 0x26298 | exact |
| VOB | 50.5 | 1996 | 151/26 | 0x26298 | **differs** |
| VOT | 52.5 | 2000 | 152/26 | 0x2629c | exact |
| **VOBT (landed)** | **50.2** | **1996** | **151/26** | **0x26298** | **exact** |
| VOBT + `volatile int amount` | 50.1 | 1996 | 151/26 | 0x26298 | exact |
| VOBT, no `padA` | 54.2 | 1984 | 151/26 | 0x26298 | exact |
| VOBT, `padA` float / int | 50.7 / 50.1 | 1992 / 1988 | 151/26 | 0x26298 | exact |
| OBT (no volatile) | 61.5 | 1940 | 151/26 | 0x26298 | exact |
| OT (no volatile, no base) | 62.1 | 1960 | 152/26 | 0x2629c | exact |
| O (offer hint only) | 63.9 | 1956 | 147/25 | 0x26294 | exact |
| T (tail hint only) | 78.0 | 1996 | 147/25 | 0x26294 | exact |
| B (base split only) | 77.7 | 1996 | 146/25 | 0x26294 | exact |
| `expect(type==8,1)` on VOB | 49.9 | 2004 | 141/24 | 0x26298 | differs |
| VOB + reordered reward chain | 51.3 | 1992 | 151/26 | 0x26298 | exact |
| VOBT, no mask hint | 46.7 | 2008 | 152/26 | 0x2629c | exact |

`VOBT` is the only variant that is simultaneously 1996 B, 172 B record and
`.rel.plt`-exact.  `OBT` also reaches the 172 B record but is 56 B short of the
original body (1940 B), which would break the size-exact state.

## 4. Plain-type attempts that do not reach 172 B

Every plain-type combination of `{offer hint, base split, tail hint, volatile
amount, padA position/type, loop-exit hints, `pick==12` polarity}` was measured;
none produces a 26-row record at 1996 B:

* `__builtin_expect(getRoutes() == nullptr, 0)` alone: 1980 B, 147/25 — the hint
  does not move the null block, it only changes register allocation (63.8 %).
* Moving the null block after `getName` (flag + `noRoutes`): 2008 B, 180 B
  record (28 rows) or 168 B, never 172.
* Inverting `getRoutes() != nullptr`, `!routes`, `routes == 0`, a `bool` temp,
  and `goto`-based reorderings all compile to the identical baseline object.
* `__builtin_expect` on the `found` loop exit (`while (!expect(found,1))`)
  keeps 1996 B but stays at 147/25.

## 5. Why the score drops

`volatile int32_t type` forces every `type` access through memory and shifts the
whole register allocation: frame 120 vs 96, `type` at `sp+96` (original
`sp+36`), and `r4/r5/r8` roles swapped at entry (`r4 = systems` vs original
`r8 = systems`).  The original's `type` is a plain memory-resident spill slot,
not volatile.  The 83.6 % tuned shape remains the best *fuzzy* variant but is
4 B short in `.ARM.extab`.

## 6. Reproduction

```sh
cp /dev/shm/w6775/w6780.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
python3 /dev/shm/w6775/score.sh                        # pct 50.2, usize 1996
python3 /dev/shm/w6775/lsda2.py cmake-build-match/libgof2hdaa.so 0x1a78c4
                                                       # cslen 151, 26 rows
```

Scratch: `/dev/shm/w6775/` (`pristine.cpp`, `w6780.cpp`, `fastcheck.sh`,
`score.sh`, `relcheck.sh`, `layout.py`, `lsda2.py`, `ndiff2.py`,
`sweepall.py`, `sweep_plain.py`, `try_*.py`).  Not part of the repo.

## Coordinator harvest note

The 172 B LSDA shape costs 33 fuzzy points (83.6 -> 50.2). Master keeps the
83.6% shape with the 168 B record; the 172 B variant is preserved here as the
known exact-extab alternative. `.ARM.extab` therefore stays 4 B short
(0x26294 vs 0x26298), from this function alone.
