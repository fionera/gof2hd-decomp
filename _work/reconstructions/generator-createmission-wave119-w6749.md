# `Generator::createMission` — wave 119 (w6749): revert to the faithful shape, pad store before `fromSystemPtr`

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `tlen=151`, 26 call-site rows).

Status: **source change landed.**  The wave-119/w6703 tuned shape (`volatile int32_t type`,
`padA` inside the mission-type loop, reward chain `11,5,3,8,7,9`, offer/tail hints) is
replaced by the *faithful* wave-113 shape (`int32_t type`, no offer/tail hints) with the
`volatile double padA` store moved from just before the reward expression to just before
`fromSystemPtr`.  `.text` stays exactly 1996 B and the ratchet is green.

```
before (HEAD):  53.3 %  1996 B  .ARM.extab 0x26294  createMission LSDA 176 B
after:          83.5 %  1996 B  .ARM.extab 0x2628c  createMission LSDA 168 B
```

Full `check` at the end of the pass:

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.text` `0x1337c8` (exact), `.rodata` `0x41c27`
(unchanged; `.ARM.extab` shrinks 8 B but the 16-byte `.rodata` alignment absorbs it, so
no `.rodata` or later-section address moves).

## 1. The landed delta

```diff
-    volatile int32_t type = 0;
+    int32_t type = 0;
...
-    padA = 0; (void) padA;          // inside the mission-type loop
...
-    if (__builtin_expect(agent->getOffer() == 5, 0)) {  // lint: arm_builtin
+    if (agent->getOffer() == 5) {
...
     Galaxy *galaxy = Globals::galaxy;
     int fromSystem = Globals::status->getStation()->getSystem();
+    padA = 0; (void) padA;
     SolarSystem *fromSystemPtr = systems->data()[fromSystem];
...
-    if (type == 11) { ... } else if (type == 5) ... else if (type == 3) ...
-    else if (type == 8) ... else if (type == 7) ... else if (type == 9) ...
+    if (type == 7) { ... } else if (type == 9) ... else if (type == 8) ...
+    else if (type == 11) ... else if (type == 3) ... else if (type == 5) ...
...
-        if (__builtin_expect(systems->data()[i]->stationIsInSystem(targetStation), 1)) {  // lint: arm_builtin
+        if (systems->data()[i]->stationIsInSystem(targetStation)) {
```

The reward chain order and the offer/tail hints are *code-identical* to the original: the
whole reward region `0x97696..0x978fe` matches instruction-for-instruction.  The
`padA` store position is the only remaining knob that changes the post-RA schedule: putting
it before `fromSystemPtr` (instead of before the reward expression) is worth **+0.2**
(83.3 -> 83.5).

## 2. Measurements (all `usize == 1996` unless noted)

| variant | pct | frame | extab | note |
|---|---:|---:|---:|---|
| HEAD (wave-119/w6703 tuned) | 53.3 | 120 | 0x26294 | `volatile type`, pad in loop |
| faithful shape (`a371db01c`) | 83.3 | 112 | 0x2628c | pad before reward |
| **pad before `fromSystemPtr` (landed)** | **83.5** | **112** | **0x2628c** | |
| pad after `fromSystemPtr` | 83.3 | 112 | 0x2628c | |
| pad after reward chain | 82.1 | 112 | 0x2628c | |
| pad after `level` | 82.1 | 112 | 0x2628c | |
| pad after `bonus` / before `new Mission` | 81.8 | 112 | 0x2628c | |
| pad after `used[...] = true` (in loop) | 78.2 | 112 | 0x2628c | |
| pad after `assignType:` | 75.7 | 112 | 0x2628c | usize 1992 |
| pad before `type == 12` | 77.8 | 112 | 0x2628c | usize 2020 |
| pad before `agentName` / `difficulty` / `itemId` | 81.0 | 112 | 0x2628c | |
| no `__builtin_expect(pick == 12, 0)` | 80.6 | 112 | 0x2628c | then-block fall-through matches orig, but loop head moves |
| `__builtin_expect(pick == 12, 1)` | 80.6 | 112 | 0x2628c | same code as no-hint |
| no mask hint | 82.6 | 112 | 0x2628c | |
| no `pick == 10` hint | 53.7 | 112 | 0x2628c | usize 1992 |
| `__builtin_expect(getRoutes()==nullptr, 0)` | 64.5 | 112 | 0x2628c | moves regalloc, not the null block |
| inverted `getRoutes() != nullptr` if/else | 83.3 | 112 | 0x2628c | byte-identical to base |
| `int`/`uint32_t` instead of `int32_t` for `type` | 83.5 | 112 | 0x2628c | byte-identical |
| `volatile int amount` only, no `padA` | 71.9 | **96** | 0x2628c | usize 1984 |
| no volatile locals at all | 79.1 | **96** | 0x2628c | usize 1980 |
| `volatile int amount` before `volatile double padA` | 75.5 | 104 | 0x2628c | |
| `volatile float padA` + `volatile int amount` | 79.6 | 104 | 0x2628c | usize 1992 |
| `volatile int padA` + `volatile int amount` | 79.3 | 104 | 0x2628c | usize 1992 |
| `volatile double padA` + `volatile long long amount` | 68.1 | 120 | **0x26294** | usize 2036 |
| `volatile int32_t type` + faithful chain (no base split) | 52.9 | 112 | 0x26294 | usize 2044 |
| `volatile int32_t type` + pad at faithful position | 53.5 | 112 | 0x26294 | usize 2036 |

## 3. Why 83.5 and not 100

The remaining delta is entirely block placement / register allocation:

* **frame 112 vs 96.**  The faithful shape keeps `volatile double padA` (8 B) +
  `volatile int amount` (4 B) as dedicated stack slots; the original has no 8-byte local
  and puts `amount` in the reused `[sp,#28]` slot (the original has no `vstr d16`
  anywhere).  Removing those locals drops the frame to the exact 96 but shrinks `.text`
  to 1980/1984 B, which shifts every later function and fails the size constraint.
* **`type` phi store block.**  `MachineBlockPlacement` places the shared `type` copy after
  `bb.32` (the `offer == 5` / `type = 8` arm) instead of after `bb.48` (the `type = 13`
  arm).  This is the wave-117/118 frequency decision; flipping it needs a branch-weight
  hint (`offer0`, `race<4 && type==13`) that costs 24-40 B of `.text`.
* **`getRoutes() == nullptr` block.**  Ours is placed inline at `+0x240`; the original
  places it cold at `+0x3ce`.  This is a consequence of the store-block placement, and is
  exactly the extra LSDA gap row that makes the original record 172 B instead of 168 B.
* **`pick == 12` block.**  The original falls through into the then-block; ours falls
  through into the else-block because of the `__builtin_expect(pick == 12, 0)` hint that
  is needed to keep the rest of the loop layout.  Removing the hint matches this block
  but moves the loop head from `+0x90` to `+0x8e` (80.6 %).

The two layouts are instruction-for-instruction identical in the reward chain, the
`type == 8` item loop, the switch, the bonus/reward rounding and the tail loop; only the
stack offsets and the two block placements differ.

## 4. Section / extab change (reported, not a ratchet input)

`.ARM.extab` `0x26294 -> 0x2628c` (4 B short -> 12 B short of the original `0x26298`).
`createMission`'s LSDA record goes 176 B -> 168 B (the original is 172 B); `createShip`
stays 8 B short.  `.text`, `.rodata`, `.data`, `.bss`, `.rel.plt`, `.dynsym`, `.dynstr`,
`.hash`, `.rel.dyn` and the 27-section count are unchanged; `.rodata` does not move
(16-byte alignment absorbs the 8-byte shrink).  Ratchet `byte_exact` / `linked_exact`
stay at 4505/4505.

## 5. Reproduction

```sh
cp /dev/shm/w6749/Generator.best.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6          # RATCHET PASS, byte 4505
python3 /dev/shm/w6749/score.py                             # pct 83.5, usize 1996
```

Scratch: `/dev/shm/w6749/` (`Generator.faithful.cpp`, `Generator.best.cpp`,
`Generator.landed.cpp`, `runvar.py`, `score.py`, `ndiff.py`, `lsda.py`, `sweep*.py`).
Not part of the repo.
