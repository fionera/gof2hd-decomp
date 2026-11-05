# `Generator::createMission` — wave 118 (w6609): a 172-byte LSDA record that passes the ratchet

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `tlen=151`, 26 call-site rows).

Status: **source change landed.** The wave-114/115/116/117 tuned shape
(`volatile double padA` + `volatile int amount` + the `pick==10/12` and mask
`__builtin_expect` hints) stays at **83.3 % / 1996 B / record 168** but its LSDA
record is 4 B short.  This pass lands a shape that produces the **172-byte
record** (`tlen=151`, 26 rows) with `relplt_diff == 0`, an unchanged `.text`
size, an unchanged file size, and a green ratchet:

```
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
27 sections, file size 2,187,328 B, .text 0x1337c8 (exact)
.ARM.extab 0x2628c -> 0x26290   (12 B short -> 8 B short)
```

The fuzzy score of the landed shape is **50.1 %** (see §5 — the LSDA fix costs
register-allocation similarity).  Both numbers are reported below.

## 1. The real LSDA delta is the null block, not the store block

Walking the original call-site table shows the extra row is a **gap row**
(`lp=0`) covering `0x97580..0x975a0`:

```
orig row 6: 0x97568..0x97580 (24) lp=0x9790e   case-15 invoke range
orig row 7: 0x97580..0x975a0 (32) lp=0         gap  <-- the extra row
orig row 8: 0x975a0..0x975d8 (56) lp=0x97946   case-0 invoke range
```

The original's `getRoutes() == nullptr` block sits at `0x97586`, i.e. **between**
the case-15 and case-0 try-ranges, and its `nextInt(2)` is a *plain call* (the
`AbyssEngine::String agentName` local is not constructed yet, so there is no
cleanup to run).  `EHStreamer::computeCallSiteTable` therefore emits a gap row
for it.  In the baseline the same block sits at `0x973f8`, inside the first
`lp=0` gap row (`0x971b8..0x97416`), so it costs no row.

The null block is a successor of the `getRoutes()` check (`bb.39`).  It is
placed after `bb.48` (the `type = 13` arm) because the store block `bb.49`
(the `type` phi copy) has already been placed after `bb.32` (the `type = 8`
arm).  Fix the store-block placement and the null block falls into the switch
region by itself.

## 2. Why the store block is placed after `bb.32`

`MachineBlockPlacement::hasBetterLayoutPredecessor(bb.32, bb.49)` rejects the
claim iff

```
freq(bb.48) * HotProb >= freq(bb.32) * HotProb.getCompl()      (HotProb = 0.8)
freq(bb.48) >= freq(bb.32) / 4
```

Measured from the `-mllvm -print-machineinstrs=block-placement` dump of the
baseline (`F = freq(bb.31)`):

```
freq(bb.32) = 0.5 F
freq(bb.48) = 0.5 * 0.0005 * 0.03125 * 0.75 * 0.6667 * 0.625 * 0.03125 F
            = 7.7e-5 F                       ratio = 1.53e-4   (need 0.25)
```

The two `124:4` loop heuristics (`LBH_TAKEN_WEIGHT = 124`,
`LBH_NONTAKEN_WEIGHT = 4`, exit = 3.125 %) on the `targetStation` loop and the
`found` loop dominate.  Even with the mask hint removed the ratio is only
`2.6e-3`; with both loop exits hinted to 100 % it reaches ~0.16, still short.
A plain source change cannot flip it — `freq(bb.32)` must drop or `freq(bb.48)`
must rise by a *hinted* branch weight.

## 3. `__builtin_expect` on a `&&` subexpression is silently dropped

`if (__builtin_expect(race < 4, 1) && type == 13)` and
`if (race < 4 && __builtin_expect(type == 13, 1))` compile to **exactly the
baseline object** (83.3 % / 1996 B / 168).  `InstCombine` folds the
`llvm.expect` away before the branch sees it.  Only wrapping the whole
condition (`__builtin_expect(race < 4 && type == 13, 1)`) attaches weights —
and that changes the layout to 64.0 % / 1968 B.

## 4. The `volatile int32_t type` family

`volatile int32_t type` keeps the `type` phi copy memory-resident and does
produce 26-row records, but it moves the reward chain *after* the tail loop, so
`Item::getMaxPrice`'s relocation follows `SolarSystem::getName`'s and gold
permutes three `.rel.plt` slots (`byte_exact 4501 -> 4485`).

```
vol+base            51.1 %  2044 B  rec 172  tlen 151  relplt 3
vol+offer0          52.0 %  1996 B  rec 176  tlen 152  relplt 3
vol+base+offer0     50.4 %  1996 B  rec 172  tlen 151  relplt 3
```

`vol+base+offer0` is the only 1996-byte / 172-record shape, but it fails the
ratchet on `.rel.plt` (and on the missing `// lint: arm_builtin` waiver).

## 5. The landed shape

Adding a *tail-loop* hint
`__builtin_expect(systems->data()[i]->stationIsInSystem(targetStation), 1)`
keeps `getMaxPrice`'s relocation ahead of `getName`'s, so `relplt_diff == 0`:

```diff
-    int32_t type = 0;
+    volatile int32_t type = 0;
...
-    if (agent->getOffer() == 5) {
+    if (__builtin_expect(agent->getOffer() == 5, 0)) {  // lint: arm_builtin
...
+    int base = (int) ((float) difficulty / 10.0f * 5500.0f) + 1500;
     float reward = (galaxy->distance(...) / 1200.0f + 1.0f) *
-                   (float) ((int) ((float) difficulty / 10.0f * 5500.0f) + 1500);
+                   (float) base;
...
-        if (systems->data()[i]->stationIsInSystem(targetStation)) {
+        if (__builtin_expect(systems->data()[i]->stationIsInSystem(targetStation), 1)) {  // lint: arm_builtin
```

`int base` is the original's own shape (the original computes
`r0 = (int)(difficulty/10*5500); addw r0,#1500; vmov s2,r0; vcvt.f32.s32` at
`0x9767e..0x9768a`).

Result:

```
50.1 %  1996 B  rec 172  tlen 151  rows 26  relplt 0  extab 0x26290
```

Section bytes vs the original:

| section | baseline | landed |
|---|---:|---:|
| `.text` differing bytes | 89,185 | 90,022 |
| `.ARM.exidx` differing bytes | 1,704 | 1,327 |
| `.ARM.extab` differing bytes | 120,119 | 108,055 |
| `.ARM.extab` size | 0x2628c | **0x26290** |

So the EH tables gain ~12.4 kB of matching bytes and the `.text` body loses
837 bytes of matching bytes.  Net: **-11,604 differing bytes**.

The landed record's *rows* are close to the original's for rows 0-6 (same
`lp` values, lengths 28/50/24) but the extra row is a different gap
(`0x97844..0x978c0`, 124 B) — the record size is right, the row contents are
not yet identical.  `tlen=151` and 26 rows match exactly.

## 6. Measurements this pass

All on top of the pristine baseline unless stated.  `relplt` counts differing
8-byte `.rel.plt` entries vs the baseline link.

### 6.1 Baseline tweaks (all <= 83.3 %)

| variant | pct | usize | rec |
|---|---:|---:|---:|
| **baseline** | **83.3** | **1996** | **168** |
| `amount` non-volatile | 70.5 | 1992 | 168 |
| `amount` unsigned | 83.1 | 1996 | 168 |
| `padA` float / int / int64 | 79.5 / 79.1 / 79.2 | 1992 / 1992 / 1996 | 168 |
| `padA = 0` at declaration | 75.9 | 2004 | 168 |
| `type` int / uint32 | 83.3 | 1996 | 168 |
| `guard` 1000 / 998 | 83.2 | 1996 | 168 |
| case-3/5 amount-first | 83.3 | 1996 | 168 |
| case-3/5 if/else | 75.9 | 1988 | 168 |
| reward chain 9,7,8,11,3,5 | 82.9 | 1996 | 168 |
| reward chain 9,7,8,3\|\|5,11 | 79.8 | 2000 | 168 |

### 6.2 Branch-weight hints

| variant | pct | usize | rec | relplt |
|---|---:|---:|---:|---:|
| `expect(race<4,1) && type==13` | 83.3 (identical) | 1996 | 168 | 0 |
| `race<4 && expect(type==13,1)` | 83.3 (identical) | 1996 | 168 | 0 |
| nested `expect(race<4,1){expect(type==13,1)}` | 64.0 | 1968 | 168 | 0 |
| `expect(race<4 && type==13, 1)` | 64.0 | 1968 | 168 | 0 |
| `expect(race<4 && type==13, 0)` | 44.4 | 2036 | 168 | 0 |
| no `pick==12` hint | 80.6 | 1996 | 168 | 0 |
| `pick==12` expect 1 | 80.6 | 1996 | 168 | 0 |
| no `pick==10` hint | 53.7 | 1992 | 168 | 0 |
| `pick==10` expect 0 | 53.9 | 1996 | 168 | 0 |
| no mask hint (M0) | 82.6 | 1996 | 168 | 0 |
| M0 + targetStation-loop exit hint | 75.9 | 1992 | 168 | 0 |
| M0 + both loop exit hints | 48.8 | 2048 | 168 | 0 |
| `expect(offer==5, 0)` | 63.4 | 1972 | 168 | 0 |
| `int base` split only | 77.5 | 1996 | 168 | 0 |
| `int base` + `offer0` | 63.3 | 1956 | 168 | 0 |

### 6.3 `volatile type` family and the landed shape

| variant | pct | usize | rec | tlen | relplt |
|---|---:|---:|---:|---:|---:|
| `vol+base` | 51.1 | 2044 | 172 | 151 | 3 |
| `vol+offer0` | 52.0 | 1996 | 176 | 152 | 3 |
| `vol+base+offer0` | 50.4 | 1996 | 172 | 151 | 3 |
| `vol+base+8first` | 50.8 | 2068 | 176 | 152 | 9 |
| `vol+base+expect(type==8,1)` | 51.1 | 2072 | 176 | 152 | 9 |
| `vol+base+offer0+expect(type==7,1)` | 49.9 | 2004 | 172 | 151 | 3 |
| `vol+base+offer0+expect(type==8,1)` | 49.9 | 2004 | 164 | 141 | 9 |
| `vol+base+offer0` + `int t = type` copy | 58.5 | 1976 | 168 | 146 | 0 |
| `vol+base+offer0` + `switch` chain | 49.3 | 1972 | 164 | 141 | 9 |
| `vol+base+offer0` + tail hint 0 | 48.1 | 2000 | 172 | 151 | 0 |
| **`vol+base+offer0` + tail hint 1 (landed)** | **50.1** | **1996** | **172** | **151** | **0** |
| landed, no `base` | 52.1 | 2000 | 176 | 152 | 0 |
| landed, no mask hint | 46.7 | 2008 | 176 | 152 | 0 |
| landed, no `offer0` | 51.2 | 2040 | 168 | 146 | 0 |
| landed, `padA` float | 50.7 | 1992 | 172 | 151 | 0 |
| landed, `padA` int | 50.1 | 1988 | 172 | 151 | 0 |
| landed, `amount` unsigned | 50.0 | 1996 | 172 | 151 | 0 |
| landed, no `padA` store | 54.2 | 1984 | 172 | 151 | 0 |

Only the landed shape gives `rec 172 + relplt 0 + usize 1996` simultaneously.
Every 172-record shape that keeps `.text` at 1996 B needs the tail hint to
avoid the `.rel.plt` permutation.

## 7. Reproduction

```sh
cp /dev/shm/w6509/Generator.base.cpp src/game/mission/Generator.cpp   # wave-117 baseline
# apply the §5 diff, then:
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4501
python3 tools/verify/verify.py --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 /dev/shm/w6609/rows.py cmake-build-match/libgof2hdaa.so 0x971b8   # size 172, tlen 151, 26 rows
```

Scratch files: `/dev/shm/w6609/` (`try.sh`, `score.py`, `rows.py`, `rows2.py`,
`mbp.sh`, `cmpdis2.py`, `batch*.py`, `v_t1.cpp`). Not part of the repo.
