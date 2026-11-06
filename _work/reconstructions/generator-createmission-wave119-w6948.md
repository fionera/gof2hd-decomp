# `Generator::createMission` — wave 119 (w6948): `count` guard, LSDA 172 B audit

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B; original `.ARM.extab` LSDA record **172 B**).

```
before (HEAD e2cd5f80f):  84.3000 %  1996 B  createMission LSDA 176 B (cslen 152, 26 rows)
after:                    84.7000 %  1996 B  createMission LSDA 176 B (cslen 152, 26 rows)
```

Full `check` after the change:

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (lint CLEAN, drift 0)
```

27 sections, file size 2,187,328 B; `.text` `0x1337c8`, `.ARM.exidx` `0x6f70`,
`.ARM.extab` `0x26298` (size- and base-exact), `.rodata` `0x41c27` — all unchanged by
this pass.  `.ARM.extab` stays size-exact because the `HangarWindow::render` −4 and
`Generator::createMission` +4 records still cancel.

## 1. Exact LSDA structure (parsed with `_work/reconstructions/exidx_table.py`)

The record is at `0x1a78c4` for both binaries; header is `ff ff 01 <uleb cslen>` and
the record size is `round_up(21 + cslen, 4)`.

| | original | ours (HEAD and after) |
|---|---:|---:|
| call-site table length (`cslen`) | **151** | **152** |
| call-site entries | **26** | **26** |
| record size | **172** (`0xac`) | **176** (`0xb0`) |

Original rows (start, len, pad), offsets from function start:

```
[0] 0x000 0x24c 0     [1] 0x24c 0x01c 0x78e  [2] 0x27c 0x032 0x796
[3] 0x2da 0x014 0x754 [4] 0x368 0x006 0x78e  [5] 0x39a 0x006 0x792
[6] 0x3b0 0x018 0x756 [7] 0x3c8 0x020 0     [8] 0x3e8 0x038 0x78e
[9] 0x468 0x02c 0x790 [10] 0x4f8 0x004 0x778 [11] 0x528 0x014 0x77a
[12] 0x53c 0x064 0   [13] 0x5a0 0x008 0x776 [14] 0x5a8 0x00a 0x772
[15] 0x5b2 0x02a 0x77c [16] 0x5dc 0x018 0  [17] 0x5f4 0x04e 0x78c
[18] 0x642 0x008 0x75a [19] 0x64a 0x022 0 [20] 0x66c 0x01e 0x78c
[21] 0x696 0x006 0x794 [22] 0x6bc 0x004 0x758 [23] 0x6de 0x006 0x770
[24] 0x6e4 0x008 0x760 [25] 0x6ec 0x0e0 0
```

Ours (HEAD) rows:

```
[0] 0x000 0x25e 0     [1] 0x25e 0x01c 0x790  [2] 0x28e 0x032 0x796
[3] 0x2ec 0x012 0x754 [4] 0x378 0x006 0x790  [5] 0x3aa 0x006 0x792
[6] 0x3c0 0x018 0x756 [7] 0x3e2 0x036 0x790  [8] 0x460 0x008 0x78a
[9] 0x47c 0x018 0x78e [10] 0x4f8 0x004 0x770 [11] 0x528 0x014 0x778
[12] 0x53c 0x064 0   [13] 0x5a0 0x008 0x76e [14] 0x5a8 0x00a 0x76a
[15] 0x5b2 0x02a 0x77a [16] 0x5dc 0x018 0  [17] 0x5f4 0x04e 0x772
[18] 0x642 0x008 0x758 [19] 0x64a 0x022 0 [20] 0x66c 0x01e 0x78c
[21] 0x696 0x006 0x794 [22] 0x6bc 0x004 0x78e [23] 0x6e0 0x006 0x768
[24] 0x6e6 0x008 0x75e [25] 0x6ee 0x0de 0
```

Both tables have 26 rows.  The original has **five** 5-byte pad=0 rows
(`[0]`, `[7]`, `[12]`, `[16]`, `[19]`) and twenty-one 6-byte rows; ours has **four**
5-byte pad=0 rows (`[0]`, `[12]`, `[16]`, `[19]`) and twenty-two 6-byte rows.  The
extra original row is `[7]` `(0x3c8, 0x20, pad=0)`, a 5-byte null row; it replaces one
of our 6-byte rows, so the whole table is exactly **1 byte shorter** (151 vs 152).

## 2. Root cause: the cold `getRoutes() == nullptr` block

Original `[7]` covers `0x97580..0x975a0`, i.e. the tail of the `type == 15` switch arm
plus the cold `systems->data()[agentSystem]->getRoutes() == nullptr` block at
`+0x3ce` (`0x97586`).  The original lays that block out **after `getName()`**
(`0x973fa`) and after the `type == 15` arm, between two try-ranges, so its
`nextInt(2)` call is an ordinary potentially-throwing call between try-ranges and
`EHStreamer::computeCallSiteTable` emits a pad=0 entry for it.

Ours lays the same block out inline at `0x973f8`, immediately before `getName()`
(`0x9740c`).  That address is inside the initial gap (`[0] 0x000..0x25e`), which is
already a pad=0 entry, so the cold block creates no separate row.  The `+4` is
therefore a **block-placement consequence**, not a source spelling: the original
chains the shared `type` store block (`%177`) with the *else* arm (the else arm falls
through to `str r0,[sp,#36]` at `0x973f8`), while ours chains it with the *offer* arm
(`str r0,[sp,#36]` at `0x97354`, the else arm branches back to it).  This is the same
`%177` observation as `generator-createmission-wave119-w6780.md`.

## 3. Landed change

```diff
 Mission *Generator::createMission(Agent *agent, Array<SolarSystem *> *systems) {
+    struct NoopGuard { ~NoopGuard() {} };
     ...
-        int count = Globals::rnd->nextInt(15);
+        int count; { NoopGuard g; count = Globals::rnd->nextInt(15); }
```

The empty-destructor guard turns the `nextInt(15)` call into an `invoke` with a new
cleanup landing pad.  It splits the old `[3] (0x2ec, 0x12, 0x754)` row into
`(0x2ec, 0x6, 0x756) + (0x2fa, 0x4, 0x754)` (+6 bytes) and the comma2 reward rewrite
already merges the old reward rows `[8]+[9]` into `(0x460, 0x34, 0x790)` (−6 bytes),
so `cslen` stays 152 and the record stays 176 B.  The score rises 84.30 → 84.70.

## 4. Measured variants

All variants are 1996 B unless noted; `cslen`/rows are parsed from the built `.so`.
`base` = HEAD (84.3000, cslen 152/26).  `comma2` = the `fromSystem`/`fromSystemPtr`
comma-expression reward rewrite (82.4000, cslen 146/25).  `NoopGuard` is the local
`struct NoopGuard { ~NoopGuard() {} };` described above.

### 4.1 Improvements that keep the 176 B record

| variant | % | cslen/rows |
|---|---:|---:|
| **`count` guard (landed)** | **84.70** | 152/26 |
| `getTecLevel` guard (`guard_tec`) | 84.70 | 152/26 |
| `count` guard + `level` guard | 84.70 | 152/26 |
| `count` guard + `mission`/`standing`/`finalname`/`asteroid`/`level2` guard | 84.70 | 152/26 |
| `diff` guard (difficulty if/else) | 84.40 | **146/25** (168 B) |
| `diff` + `mission`/`standing`/`finalname`/`asteroid`/`level2` guard | 84.40 | 146/25 |
| `diff` + `newtype` / `cold_dowhile` / `cold_switch` | 84.40 | 146/25 |
| `level` guard (no comma) | 84.30 | 152/26 |

### 4.2 Variants that produce a 172 B record (cslen 147..151)

| variant | % | cslen/rows | usize |
|---|---:|---:|---:|
| `getStation` guard + comma2 (`guard_getstation`) | 81.60 | **151/26** | 2000 |
| `diff` + `getStation` guard (`diff+getstation`) | 81.20 | **151/26** | 2000 |
| `diff` + `(void) Globals::status->getLevel();` after the clamp | 78.90 | **147/25** | 2016 |
| `diff` + `(void) getSystem()/getCurrentCampaignMission()` after the clamp | 78.90 | **147/25** | 2016 |
| `diff` + `difficulty = difficulty % 100;` after the clamp | 78.70 | **151/26** | 2000 |
| `diff` + `exp_race13_1` (`__builtin_expect(race < 4 && type == 13, 1)`) | 67.00 | **151/26** | 1980 |
| `diff` + `volatile uint32_t type` | 52.60 | **151/26** | 2052 |

The `getStation` guard is the cheapest way to add a 5-byte row: it wraps the very
first call (`agent->getStation()` at `+0x1c`) and produces
`[0] (0x1c, 0x6, 0x790)` + `[1] (0x22, 0x23c, 0)` in place of the old
`[0] (0x0, 0x25e, 0)` — a net +5 bytes (both new starts are 1-byte ULEBs).  It costs
~3 points because the extra landing pad shifts the literal pool by 4 bytes.  Adding a
`(void) getLevel();` call in the difficulty-clamp gap instead creates the missing
pad=0 row directly (`cslen` 146 → 147) at a ~5.5-point cost.

### 4.3 Other probes (all keep 176 B unless noted)

| variant | % | cslen/rows |
|---|---:|---:|
| `faithful` (no `padA`, plain `amount = 0`) | 82.00 | 152/26 (1988 B) |
| `comma2` alone | 82.40 | 146/25 |
| `invert` (`offer != 5`, arms swapped) | 84.30 | 152/26 |
| `invert` + `count` guard | 84.70 | 152/26 |
| `newtype` (single `type = newType` store at the merge) | 84.40 | 146/25 |
| `offervar` (hoist `getOffer()` + separate `if`) | 76.10 | 152/26 |
| `name_top` / `name_before_offer` | 37.9 / 46.7 | 209/36, 177/30 |
| `coldflag` (cold block after the type-13 block) | 77.00 | 184 |
| `coldflag` + `diff` guard | 81.70 | 152/26 |
| `coldflag` + `count` guard | 81.80 | 158/27 |
| `whole_guard` (guard scoped over the whole body) | 45.50 | 206/35 |
| `exp_null0` on base | 65.60 | 152/26 |
| `exp_offer0` / `exp_offer1` on base | 64.40 / 39.00 | 176 / 168 |
| `exp_race13_1` on base | 66.00 | 176 |
| `exp_race13_0` on base | 41.80 | 168 |
| `volatile uint32_t type` on base | 53.60 | 172 |
| `guard_getstation` on base (no comma) | 72.60 | 157/27 |
| `genstation` guard (whole first statement) + `diff` | 72.00 | 151/26 |

## 5. Conclusion

The 172 B record needs the original's **cold-block null row**, i.e. the cold
`getRoutes() == nullptr` block laid out after `getName()` between two try-ranges.
Every source-level lever that reproduces it (`getStation` guard, extra
`getLevel()`/`getSystem()` call, dummy `%`, `volatile type`, `expect`) either shifts
the literal pool or changes the hot path enough to cost 2.7–31 points.  The best
172 B variant is `guard_getstation` at **81.60 %** (151/26, 2000 B); the best
score-preserving change is the landed `count` guard at **84.70 %** (176 B).
Landing a 172 B variant would also shrink `.ARM.extab` to `0x26294` (4 B short)
because `HangarWindow::render` is still −4.

## 6. Reproduction

```sh
git checkout -- src/game/mission/Generator.cpp   # then apply the 2-line diff above
cmake --build cmake-build-match --target check -j6  # RATCHET PASS, byte 4505
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch variants: `/dev/shm/w6948/Generator.*.cpp` (not part of the repo).
