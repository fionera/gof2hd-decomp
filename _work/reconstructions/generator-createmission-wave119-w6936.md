# `Generator::createMission` — wave 119 (w6936): 172-byte LSDA landed

> **Not landed as source (orchestrator decision).** The 172 B LSDA is exact, but it
> moves the section's -4 B to `HangarWindow::render` and drops `createMission` 84.7 -> 73.3 %.
> The current master keeps `createMission` at 84.7 % with a 168 B record; this note is the
> reference for the alternative shape.

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B).

```
before (HEAD 98f531de9):  84.3 %  1996 B  LSDA 176 B (cslen 152, 26 rows)  .ARM.extab 0x26298
after:                    73.3 %  1996 B  LSDA 172 B (cslen 151, 26 rows)  .ARM.extab 0x26294
```

Full `check` after the change:

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (lint CLEAN, drift 0)
```

27 sections, file size still 2,187,328 B, `.rodata` still at `0x1c9e10` (the 4-byte
`.ARM.extab` shortfall fits the 16-byte `.rodata` alignment gap, so no address moves).
`.ARM.extab` is now **4 B short** (`0x26294`); the only remaining record delta is
`HangarWindow::render` -4 (944 vs 948).  `Generator::createMission`'s LSDA record is exact.

## 1. What the record is made of

`.ARM.extab` record = `personality prel31 (4) + unwind opcodes (12) + LSDA`.  The 12
unwind-opcode bytes are identical in both binaries (`c9 4f 97 02 f0 80 00 83 b0 b0 b0 ab`),
so the whole 4-byte delta is the LSDA:

```
LSDA = ff ff 01 <uleb cslen> <call-site table> [action table] , padded to 4
```

`cslen >= 128` ⇒ the length is a 2-byte uleb and the record is `21 + cslen` padded to 4.
Exact 172 therefore needs **`cslen` in 148..151** (5 + cslen = 153..156 → 156).

Original: 26 rows, `cslen` 151 (`5*5 + 21*6`).  HEAD: 26 rows, `cslen` 152
(`4*5 + 22*6`).  Only one row differs in encoded size — the row whose landing pad is
`0x790` (2 bytes) instead of `0` (1 byte).

## 2. The two structural differences (row-by-row)

Rows 10..25 are identical apart from a few bytes of address shift; the whole delta lives
in rows 0..9.

| # | original (start..end, lp) | HEAD (start..end, lp) |
|---|---|---|
| 0 | `0x971b8..0x97404` lp=0 | `0x971b8..0x97416` lp=0 |
| 1 | `0x97404..0x97420` lp=0x78e | `0x97416..0x97432` lp=0x790 |
| 7 | `0x97580..0x975a0` **lp=0** (cold `getRoutes()==nullptr` block) | `0x9759a..0x975d0` lp=0x790 (switch arms) |
| 8 | `0x975a0..0x975d8` lp=0x78e (switch arms) | `0x97618..0x97620` lp=0x78a (reward chain part 1) |
| 9 | `0x97620..0x9764c` lp=0x790 (whole reward chain) | `0x97634..0x9764c` lp=0x78e (reward chain part 2) |

So HEAD differs in **two** ways:

1. **The cold `getRoutes()==nullptr` block is not a separate try-range.**  The original
   lays it out at `0x97586` — after the `type == 15` switch arm and before the `type == 0`
   arm — so its `nextInt(2)` is an ordinary (non-invoke) call *between* two try-ranges and
   gets a 5-byte `lp=0` row.  HEAD lays it out inline at `0x973f8`, immediately before
   `getName`, i.e. inside the initial `lp=0` gap, so it creates no row.
2. **The reward chain is split into two rows.**  In the IR the chain is
   `invoke getStation / invoke getSystem` → landingpad `%368`, then
   `invoke getStation / invoke getSystem / invoke distance` → landingpad `%372`.  The
   boundary is the *statement* boundary between
   `int fromSystem = Globals::status->getStation()->getSystem();` and the
   `float reward = ...` statement (removing `padA` does **not** merge them; the two
   landingpads are still emitted).  The original has one landingpad for the whole chain.

Original = 5 zero rows + 21 real rows = 151.  HEAD = 4 zero rows + 22 real rows = 152.

## 3. The landed shape

Two independent, real-C++ edits:

```diff
-    int targetStation = generateStationIndex(systems, agent->getStation());
+    int targetStation;
+    { struct NoopGuard { ~NoopGuard() {} } guard;
+      targetStation = generateStationIndex(systems, agent->getStation()); }
...
-    volatile double padA;
+    volatile long long padA;
...
-    int fromSystem = Globals::status->getStation()->getSystem();
     padA = 0; (void) padA;
-    SolarSystem *fromSystemPtr = systems->data()[fromSystem];
-    float reward = (galaxy->distance(fromSystemPtr, systems->data()[galaxy->getStation(targetStation)->getSystem()]) / 1200.0f + 1.0f) *
+    float reward = (galaxy->distance(systems->data()[Globals::status->getStation()->getSystem()],
+                                     systems->data()[galaxy->getStation(targetStation)->getSystem()]) / 1200.0f + 1.0f) *
```

* Inlining the `getSystem()` lookup into the reward expression puts the whole chain in one
  full-expression, which merges `%368`/`%372` into a single landing pad: 26 → **25 rows,
  `cslen` 146** (the `comma` form, `82.4 %`, 1996 B, is an equivalent spelling).
* The `NoopGuard` (an empty-destructor scope guard — a real C++ idiom, no attribute, no
  asm) makes `generateStationIndex` an invoke at `+0x1e`.  That adds one **5-byte**
  `lp=0x78e` row (`start 0x1e`, `len 0xe`), and the old initial gap becomes a 5-byte row:
  146 + 5 = **151**, 26 rows → **172 B**.
* `volatile long long padA` (instead of `double`) keeps the function at exactly 1996 B;
  it is the only padA type that lands on 1996 with `cslen` 151.

## 4. Measured variant table

All measurements on this worktree, `link_so` + `verify.py` + a full LSDA parse.

| variant | % | usize | cslen/rows | note |
|---|---:|---:|---:|---|
| **HEAD** | **84.3** | 1996 | 152/26 | 176 B |
| `padA = 1` | 84.3 | 1996 | 152/26 | value irrelevant |
| `direct_idx` (fromSystem temp, index without ptr temp) | 84.3 | 1996 | 152/26 | |
| `no_fromsys_temp` | 79.3 | 2004 | 152/26 | |
| `inline` (`getSystem()` inside reward expr) | 79.5 | 2004 | **146/25** | merges the chain |
| `comma` (comma-expression form of the same) | **82.4** | 1996 | **146/25** | best merge-only shape |
| `comma2` | 82.4 | 1996 | 146/25 | |
| `comma` + `padA=1` / `padA_ll` / `padA_ro` | 82.4 / 80.3 / 80.4 | 1996 / 1996 / 1988 | 146/25 | |
| `inline` + `expect(routes==nullptr,0)` | 64.7 | 2000 | 146/25 | null block lands in the final gap |
| `inline` + `expect(routes==nullptr,1)` | 44.3 | 2064 | 141/24 | |
| `inline` + `expect(race<4&&type==13,0/1)` | 41.1 / 62.7 | 2068 / 1968 | 141/24 / 146/25 | |
| `inline` + `expect(offer==5,0)` | 62.1 | 1972 | 146/25 | |
| `inline` + `NoopGuard` around `getRace` | 75.2 | 2000 | 155/27 | |
| `inline` + `NoopGuard` around `getStation` | 73.8 | 2000 | 151/26 | 172 B, wrong size |
| `inline` + `NoopGuard` + `padA int` | 73.4 | 1992 | 151/26 | 172 B, wrong size |
| `inline` + `NoopGuard` + `padA int` read-only | 73.8 | 1988 | 151/26 | 172 B, wrong size |
| `inline` + `NoopGuard` + `padA float` | 74.2 | 1992 | 151/26 | 172 B, wrong size |
| `inline` + `NoopGuard` + no padA | 67.7 | 1984 | 151/26 | |
| **`inline` + `NoopGuard` + `padA long long`** | **73.3** | **1996** | **151/26** | **landed** |
| `comma` + `NoopGuard` | 72.0 | 2004 | 151/26 | |
| `comma` + `NoopGuard` + `padA long long` | 70.6 | 2016 | 151/26 | |
| HEAD + `NoopGuard` around `generateStationIndex` | 72.6 | 2004 | 157/27 | |
| HEAD + `NoopGuard` around `getRace` | 77.1 | 1992 | 161/28 | |
| `getName` at function top / after `getRace` | 38.5 / 44.4 | 2024 / 2028 | 215/37 / 206/35 | |
| `padA int/float/ll/plain/ro` on HEAD | 80.0 / 79.4 / 81.9 / 71.7 / 79.3 | 1992 / 1996 / 1996 / 1988 / 1992 | 152/26 | |
| `tosys_temp` / `dist_temp` / `base_temp` | 81.7 / 82.0 / 82.0 | 2000 / 2000 / 2000 | 158/27 / 152/26 / 152/26 | |
| `padA_last` / `galaxy_then_ptr` | 83.6 / 83.9 | 1996 / 1996 | 152/26 | |

Every variant that reaches `cslen` 151 costs between 10 and 33 fuzzy points; `82.4 %` is
the ceiling of the chain-merge alone (still 146, i.e. 168 B).

## 5. Why the cold block cannot be moved cheaply

The cold block's placement is an MBP decision: in the `-print-after` dump the store block
(`bb.49`, the `type` phi store) is laid out between the `type == 13` arm (`bb.48`) and the
cold block (`bb.50`), and `getName` (`bb.51`) follows.  All frequency hints
(`expect(routes==nullptr,0/1)`, `expect(offer==5,0)`, `expect(race<4&&type==13,0/1)`)
either leave the block inside the initial gap or push it into the **final** `lp=0` gap
(row 25), never between two try-ranges.  The only reliable lever that produces a separate
`lp=0` row at a low address is the empty-destructor scope guard above.

## 6. Trade-off / follow-up

The landed shape makes `Generator::createMission`'s LSDA **exact** (172 B) and leaves the
`.ARM.extab` section 4 B short (`0x26294`), the shortfall now being solely
`HangarWindow::render` (-4).  The fuzzy score drops 84.3 → 73.3 because the guard changes
the prologue/register allocation.  If the fuzzy score is preferred over LSDA exactness,
`HEAD` (176 B, 84.3 %) or the `comma` form (168 B, 82.4 %) are the alternatives.

A cleaner 172 B would need the cold `getRoutes()==nullptr` block laid out between the
`type == 15` and `type == 0` switch arms (the original's `%177` chain selection), which no
source spelling tried so far reproduces without a large match regression.

## 7. Reproduction

```sh
git checkout -- src/game/mission/Generator.cpp   # HEAD
cmake --build cmake-build-match --target check -j6
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch: `/dev/shm/w6936/` (`head_gen.cpp`, `t_inline.cpp`, `t_comma.cpp`, `t_gll.cpp`,
`batch2.py`, `m1.py`, `lsda3.py`, `ircheck.sh`).  Not part of the repo.
