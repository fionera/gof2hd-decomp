# `Generator::createMission` — wave 119 (w6741): restore the faithful 83.3 % shape

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `tlen=151`, 26 call-site rows).

Status: **source change landed.**  The wave-119 tuning (`volatile int32_t type` + pad-store
position + reward-chain reorder) had traded the faithful shape's fuzzy score away for a 176-byte
LSDA record (53.3 %).  This pass restores the wave-117 faithful shape (plain `int32_t type`,
`volatile double padA` store at the reward block, reward chain 7,9,8,11,3,5, no offer/tail
`__builtin_expect`) at **83.3 %**, body **1996 B**, frame **112 B**.

Gate at the end of the pass (full `check`):

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B (exact), `.text` `0x1337c8` (exact), `.ARM.exidx` `0x6f70`
(exact), `.rodata` `0x41c27` (unchanged; appended rank table).  `.ARM.extab` changes
`0x26294 -> 0x2628c` (was 4 B short of the original `0x26298`, now 12 B short): the createMission
record is **168 B** instead of the original 172 B.  This is the known wave-117/118 trade-off: the
faithful shape places the `getRoutes() == nullptr` block inline (`cbz`, record 168), while the
`volatile type` shape forces it after the `type = 13` arm (`cmp/beq.w`, record 176).  The fuzzy
score is the primary metric; the extab delta is reported rather than hidden.

## 1. The landed delta (vs. the wave-119 Q1/Q9 shape)

```diff
-    volatile int32_t type = 0;
+    int32_t type = 0;
...
-    padA = 0; (void) padA;            // inside the selection loop
...
-    if (__builtin_expect(agent->getOffer() == 5, 0)) {
+    if (agent->getOffer() == 5) {
...
     SolarSystem *fromSystemPtr = systems->data()[fromSystem];
+    padA = 0; (void) padA;
     float reward = ...
-    if (type == 11) { ... } else if (type == 5) { ... } else if (type == 3) { ... }
-    else if (type == 8) { ... } else if (type == 7) { ... } else if (type == 9) { ... }
+    if (type == 7) { reward *= 0.7f; }
+    else if (type == 9) { reward *= 1.2f; }
+    else if (type == 8) { ... getMaxPrice ... }
+    else if (type == 11) { ... }
+    else if (type == 3) { reward *= 2.0f; }
+    else if (type == 5) { reward *= 2.0f; }
...
-        if (__builtin_expect(systems->data()[i]->stationIsInSystem(targetStation), 1)) {
+        if (systems->data()[i]->stationIsInSystem(targetStation)) {   // (real source: stationIsInSystem)
```

The shape is the wave-117 commit `a371db01c` (`Generator::createMission pass: bitmask branch
hint fixes spill-slot allocation (82.6 -> 83.3%)`) restored verbatim on top of the wave-119 tree.

## 2. Why this shape is the local optimum

The original's stack frame is 96 B with the scalars at
`sp+16 bonus/galaxy, sp+20 &guard, sp+24 difficulty, sp+28 amount, sp+32 status, sp+36 type,
sp+40 rnd` and the four `String` temporaries at `sp+44..sp+92`.  The faithful shape keeps
`status/type/rnd` at exactly those offsets; the `volatile double padA` anchor occupies `sp+96`
and only pads the frame to 112 B.  Removing `padA` drops the body to 1984 B (12 B short) and
re-colours `amount` to `sp+88`, which costs far more than the pad store itself:

| variant | pct | usize | frame | rec |
|---|---:|---:|---:|---:|
| **faithful, `padA` at reward block (landed)** | **83.3** | **1996** | 112 | 168 |
| faithful, no `padA` | 71.9 | 1984 | 96 | 168 |
| faithful, `int amount` (non-volatile) | 70.5 | 1992 | 112 | 168 |
| faithful, `volatile unsigned amount` | 83.1 | 1996 | 112 | 168 |
| faithful, `volatile long amount` | 83.3 | 1996 | 112 | 168 |
| faithful, `uint32_t type` | 83.3 | 1996 | 112 | 168 |

The 12 B the pad store contributes are exactly the four RA differences the original has and the
pad-free shape lacks: the `ldr r5,[sp,#24]` difficulty reload, the `ldr r0,[sp,#28]; vmov s2,r0`
amount-to-FP move in the `type == 11` arm, the `mov r1,r0` in the `type == 8` arm, and the extra
landing-pad `nop`/branch.  The pad store lands on the same 12 B without changing the scalar
offsets.

## 3. Measurements (all on the faithful shape unless stated)

| variant | pct | usize | rec | note |
|---|---:|---:|---:|---|
| **landed** | **83.3** | **1996** | 168 | padA at reward block |
| padA at function top | 78.2 | 1996 | 168 | |
| padA after the `used` block (Q1) | 78.2 | 1996 | 168 | |
| padA after `assignType:` (Q9) | 75.7 | 1992 | 168 | |
| padA at `return mission` | 80.4 | 1996 | 168 | |
| padA after `guard` / after `selected` | 78.0 | 1996 | 168 | |
| padA in the `getRoutes()==nullptr` block | 81.0 | 1996 | 168 | |
| padA in the `type==13` else block | 79.0 | 2000 | 168 | |
| padA inside the `while (!found)` loop | 79.0 | 2000 | 168 | |
| padA in the offer `else` block | 81.0 | 1996 | 168 | |
| `volatile float/int/long long padA` | 79.5/79.1/79.2 | 1992/1992/1996 | 168 | |
| `padA = 0.0` instead of `padA = 0` | 79.5 | 1992 | 168 | |
| offer arm `type = 8` before `getStation` | 83.3 | 1996 | 168 | |
| `int offer = agent->getOffer()` | 83.3 | 1996 | 168 | |
| `__builtin_expect(offer == 5, 0)` | 63.4 | 1972 | 168 | |
| `__builtin_expect(offer == 5, 1)` | 38.8 | 2052 | 168 | |
| `__builtin_expect(getRoutes()==nullptr, 0)` | 64.5 | 1996 | 168 | |
| `__builtin_expect(getRoutes()==nullptr, 1)` | 48.0 | 2036 | 168 | |
| `__builtin_expect(getRoutes()!=nullptr, 1)` | 48.0 | 2036 | 168 | |
| `while (expect(!found,1))` | 57.5 | 2000 | 168 | |
| `expect(stationIsInSystem,1)` | 73.3 | 1992 | 168 | |
| `expect(race match,1)` | 40.8 | 2040 | 168 | |
| `expect(targetStation==status station,1)` | 43.7 | 2012 | 168 | |
| both loop hints | 54.1 | 1988 | 168 | |
| nested `if (race<4) if (type==13)` | 83.3 | 1996 | 168 | |
| `goto noRoutes` restructure | 83.3 | 1996 | 168 | |
| ternary `type = nextInt(2) ? 4 : 1` | 83.3 | 1996 | 168 | |
| `int selected` | 83.2 | 1996 | 168 | |
| `uint32_t guard` | 56.8 | 1984 | 168 | |
| `uint32_t level` | 83.2 | 1996 | 168 | |
| `uint32_t itemId` | 83.3 | 1996 | 168 | |
| `uint32_t difficulty` | 82.6 | 1996 | 168 | |
| `uint32_t bonus` | 83.3 | 1996 | 168 | |
| `uint32_t costs` | 83.2 | 1996 | 168 | |
| `(void)&amount` / `&amount` pointer | 83.3 | 1996 | 168 | |
| `volatile int amount = 0` | 80.6 | 1996 | 168 | |

Every variant that moves the pad store or changes the offer hint keeps the body at 1996 B but
loses 2–20 fuzzy points; the faithful shape is a wide local optimum.

## 4. The 172-byte LSDA trade-off

The wave-118 analysis (`generator-createmission-wave118-w6609.md`) established that the 172-byte
record needs the `getRoutes() == nullptr` block placed after the `type = 13` arm, which only the
`volatile int32_t type` shape achieves.  The plain-`type` faithful shape places that block inline
(`cbz r0, ...`), so its record is 168 B.  Measured on the faithful shape:

| shape | pct | usize | rec | extab |
|---|---:|---:|---:|---:|
| faithful, plain `type` (landed) | 83.3 | 1996 | 168 | 0x2628c |
| faithful, `volatile type` (wave-118/119) | 50.1–53.3 | 1996 | 176 | 0x26294 |

The 172-byte record is therefore still an open item: it costs ~30 fuzzy points with the current
source shapes.  The landed commit keeps the faithful 83.3 % body and reports the 4 B record
shortfall.

## 5. Reproduction

```sh
cp /dev/shm/w6741/Generator.a371.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
python3 /dev/shm/w6741/score.py                        # pct 83.3, usize 1996
python3 /dev/shm/w6741/recsize.py cmake-build-match/libgof2hdaa.so 971b8   # rec 168
```

Scratch files: `/dev/shm/w6741/` (`Generator.a371.cpp`, `Generator.nopad.cpp`, `score.py`,
`recsize.py`, `measure.sh`, and the `p*/q*/o*/n*/h*/r*/w*/m*/t*/u*/x*/y*` variant files).
Not part of the repo.
