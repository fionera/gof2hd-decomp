# `Generator::createMission` — wave 114 (w6209): bitmask branch hint

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record 172 B).

Status: **source change landed.**  Adding a branch-weight hint to the
`type < 15 && ((1U << type) & 0x4801U) != 0` condition raises the official
single-function score from **82.6 % to 83.3 %** at the same 1996 B size.
The shared `type` store and the `getRoutes()==nullptr` block do **not** move
(store still at `0x19c`, null block at `0x240`); the LSDA record stays 168 B.
The win is in register allocation / spill-slot assignment, not layout.

## Change

```c++
-        if ((uint32_t) type < 15 && ((1U << type) & 0x4801U) != 0) {
+        if ((uint32_t) type < 15 && __builtin_expect(((1U << type) & 0x4801U) != 0, 1)) {  // lint: arm_builtin anchor: Generator::createMission; branch-weight hint
```

The `// lint: arm_builtin` same-line waiver is required (the lint ratchet
baseline has 0 `arm_builtin` sites in `Generator.cpp`; the two existing
`__builtin_expect` calls in this function carry the same waiver).

## Why it helps

The hint changes `BranchProbabilityInfo` for the bitmask compare from the
zero-compare heuristic 20:12 (P(nonzero) = 0.625) to the `!prof` metadata
weights 2000:1 (P(nonzero) ~= 1).  `MachineBlockFrequencyInfo` therefore
re-weights the `type < 15` / bitmask path, which changes the register
allocator's spill-slot choices in this function.  The new allocation matches
the original in the following places (baseline -> new):

```
str r0,[sp,#24]      -> str r0,[sp,#28]        (agent/targetStation swap)
str r5,[sp,#24]      -> str r5,[sp,#28]
str.w r9,[sp,#28]    -> str.w r9,[sp,#24]
ldrd r4,r9,[sp,#24]  -> ldrd r9,r4,[sp,#24]    (x2)
mov r9,r1            -> str r1,[sp,#32]; ldr r0,[sp,#32]  (rem spill)
subne.w r5,r6,r9     -> subne r5,r6,r0
```

The frame remains 112 B (original 96 B), the `type` store remains at
`0x19c`, and the null block remains at `0x240`, so the LSDA record is still
168 B (original 172 B).  `.ARM.extab` stays exact at `0x26298` only because
`Hud::drawExx` is +4 (480 vs 476), as recorded in waves 112/113.

## Measurements (single-function scorer, `tools/verify/verify.py`)

Baseline (wave 110/113 shape): **82.6 %**, 1996 B, extab 168.

Best variant (landed): `__builtin_expect(((1U << type) & 0x4801U) != 0, 1)`
= **83.3 %**, 1996 B, extab 168.

Equivalent spellings (same 83.3 %): `__builtin_expect((1U << type) & 0x4801U, 1)`
(implicit bool), `__builtin_expect(((1U << type) & 0x4801U) != 0, 1)`.

All other hint placements measured this pass were worse or neutral:

| variant | pct | size | extab |
|---|---:|---:|---:|
| **mask `!= 0`, 1 (landed)** | **83.3** | 1996 | 168 |
| mask implicit bool, 1 | 83.3 | 1996 | 168 |
| mask + `type==6`, 1 | 83.2 | 1996 | 168 |
| mask + `type==11`, 0 | 82.9 | 1996 | 168 |
| mask + `rem % 50`, 0/1 | 83.3 | 1996 | 168 |
| mask + `costRem % 50`, 0/1 | 83.3 | 1996 | 168 |
| mask + `while(!found)` hint, 0/1 | 83.3 | 1996 | 168 |
| mask + `while(!selected)` hint, 0/1 | 83.3 | 1996 | 168 |
| mask + `guard < 1` hint, 0/1 | 83.3 | 1996 | 168 |
| mask + `difficulty >= 10`, 0/1 | 83.3 | 1996 | 168 |
| mask + `type==12`, 0/1 | 83.3 | 1996 | 168 |
| mask + `used->data()[pick]`, 1 | 80.7 | 1996 | 168 |
| mask + no `pick==12` hint | 80.6 | 1996 | 168 |
| mask + `pick==12`, 1 | 80.6 | 1996 | 168 |
| mask + `type==7`, 0 | 78.4 | 1996 | 164 |
| mask + `type==9`, 0 | 78.2 | 1996 | 164 |
| mask + `type==9`, 1 | 68.3 | 1996 | 176 |
| mask + `type==8`, 0 | 47.9 | 1996 | 176 |
| mask + `type==8`, 1 | 45.9 | 1996 | 180 |
| mask `!= 0`, 0 | 73.3 | 1996 | 168 |
| mask `== 0`, 0 | 73.3 | 1996 | 168 |
| mask `!`, 0 | 73.3 | 1996 | 168 |
| whole `&&` hint, 1 (`and1`) | 64.0 | 1968 | 168 |
| whole `&&` hint, 0 (`and0`) | 42.2 | 2028 | 168 |
| `type < 15` hint, 1 | 64.0 | 1968 | 168 |
| `type < 15` hint, 0 | 73.3 | 1996 | 168 |
| `getRoutes()==nullptr`, 1 | 48.2 | 2040 | 168 |
| `getRoutes()==nullptr`, 0 | 62.4 | 2000 | 168 |
| mask + `getRoutes()==nullptr`, 0 | 64.5 | 1996 | 168 |
| mask + `getRoutes()==nullptr`, 1 | 48.0 | 2036 | 168 |
| mask + `offer==5`, 0 | 63.4 | 1972 | 168 |
| mask + `offer==5`, 1 | 38.8 | 2052 | 168 |
| mask + no `pick==10` hint | 53.7 | 1992 | 168 |
| mask + `pick==10`, 0 | 53.9 | 1996 | 168 |
| mask + `stationIsInSystem`, 1 | 73.3 | 1992 | 168 |
| mask + `stationIsInSystem`, 0 | 65.6 | 2016 | 168 |
| mask + `getRace()==race`, 1 | 40.8 | 2040 | 168 |
| mask + `getRace()==race`, 0 | 65.9 | 1984 | 168 |
| mask + `used != nullptr`, 1/0 | 56.1 / 56.8 | 1996 / 2016 | 168 |
| mask + `pick==8`, 0/1 | 56.8 / 53.0 | 1960 / 2020 | 168 |
| mask + `campaign < 16`, 0/1 | 70.1 / 67.4 | 2004 / 1996 | 168 |
| mask + `(type|4)==12`, 0/1 | 68.0 / 70.2 | 2012 / 1992 | 176 |
| mask + `padA`/`amount` frame variants | 70.5–79.5 | 1984–1992 | 168 |

## Unchanged structural state

Gate after the change (full `check`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint:   CLEAN ✓ (no new soft violations; hard invariants 0)
drift:  0 field(s) across 0 class(es)
RATCHET PASS
```

27 sections, file size **2,187,328 B**, `.text` `0x1337c8`,
`.ARM.extab` `0x26298` — all unchanged.  `createMission`'s own LSDA record
is still 168 B (original 172 B); the section total remains exact because
`Hud::drawExx` is +4.

## Remaining gap

The store/null-block layout still needs `freq(type13) >= 0.25 * freq(type8)`
in `MachineBlockPlacement::hasBetterLayoutPredecessor`; the measured ratio
is ~0.145 (5/32-ish), so the shared `str r0,[sp,#36]` joins the type-8 chain
and the null block is placed right after `type = 13`.  Crossing 0.25 with a
hint moves the store but outlines the type-8 arm or flips a loop branch,
costing more than it gains (see wave 112/113 notes and the table above).
